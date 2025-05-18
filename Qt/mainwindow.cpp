#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupProportionalWidgets();
    adjustGridLayoutProportions();

    // Po setupUi(this);
    QWidget* widget_14 = findChild<QWidget*>("widget_14");
    QWidget* widget_16 = findChild<QWidget*>("widget_16");
    QWidget* widget_18 = findChild<QWidget*>("widget_18");
    QWidget* robotView = findChild<QWidget*>("robotView");

    QHBoxLayout* hLayout = qobject_cast<QHBoxLayout*>(widget_14->layout());
    if (hLayout) {
        hLayout->setStretch(0, 1);
        hLayout->setStretch(1, 1);
        hLayout->setStretch(2, 1);

        widget_16->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        widget_18->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        robotView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        widget_16->setMinimumWidth(60);
        widget_18->setMinimumWidth(60);
    }

    // Dodaj to zaraz po setupUi
    connect(ui->buttEXIT, &QPushButton::clicked, this, &QWidget::close);
    // lub
    connect(ui->buttEXIT, &QPushButton::clicked, qApp, &QApplication::quit);

    // Tworzenie obiektów zarządzających
    m_sensorData = new SensorData(this);
    m_csvManager = new CSVManager(m_sensorData, this);

    // Tworzenie managerów wykresów
    m_tofChartManager = new TofChartManager(m_sensorData, ui->widTof, this);
    m_lineChartManager = new LineChartMenager(m_sensorData, ui->widLS, this);
    m_motorChartManager = new MotorChartManager(m_sensorData, ui->widMOTORS, this);
    m_imuChartManager = new ImuChartManager(m_sensorData, ui->widIMU, this);

    // Tworzenie managera wizualizacji
    m_visualManager = new VisualizationManager(m_sensorData, ui, this);

    // Inicjalizacja
    m_tofChartManager->setupChart();
    m_lineChartManager->setupChart();
    m_motorChartManager->setupChart();
    m_imuChartManager->setupChart();

    // Połączenia sygnałów i slotów
    m_tcpManager = new TCPManager(m_sensorData, this);
    connect(m_csvManager, &CSVManager::dataUpdated, this, &MainWindow::handleDataUpdated);
    connect(m_sensorData, &SensorData::dataChanged, m_visualManager, &VisualizationManager::updateAll);

    // Początkowa konfiguracja
    m_visualManager->setupArrows();
    m_visualManager->updateAll();
}

MainWindow::~MainWindow() {
    if (m_tcpManager && m_tcpManager->isConnected()) {
        m_tcpManager->disconnectFromDevice();
    }

    delete ui;
}

void MainWindow::on_buttLOADcsv_clicked() {
    // Otwórz dialog pliku
    QString filePath = QFileDialog::getOpenFileName(this, "Wybierz plik CSV", "", "Pliki CSV (*.csv);;Wszystkie pliki (*)");

    // Jeśli użytkownik wybrał plik (nie anulował)
    if (!filePath.isEmpty()) {
        if (m_csvManager->loadFromFile(filePath)) {
            m_tofChartManager->clearChart();
            m_lineChartManager->clearChart();
            m_motorChartManager->clearChart();
            m_imuChartManager->clearChart();
            m_csvManager->startPlayback();
        }
    }
}

void MainWindow::handleDataUpdated(int time) {
    m_tofChartManager->updateChart(time);
    m_lineChartManager->updateChart(time);
    m_motorChartManager->updateChart(time);
    m_imuChartManager->updateChart(time);
}


// Implement TCP connection button handler
void MainWindow::on_buttCONN_clicked()
{
    // Get IP address and port from UI elements
    // Adjust these lines to match your actual UI element names
    QString ipAddress = ui->lineIP->text();
    QString portText = ui->linePORT->text();
    QTextStream ts(&portText);
    quint16 port = 0;
    ts >> port;


       // Try to connect
    if (m_tcpManager->connectToDevice(ipAddress, port)) {
        ui->statusBar->showMessage("Próba połączenia z ESP...");
    } else {
        ui->statusBar->showMessage("Błąd podczas inicjowania połączenia");
    }
}

// Implement TCP disconnection button handler
void MainWindow::on_buttDISS_clicked()
{
    m_tcpManager->disconnectFromDevice();
    ui->statusBar->showMessage("Rozłączono z ESP");
}

// Handle connection status changes
void MainWindow::handleConnectionStatusChanged(bool connected)
{
    m_visualManager->updateConnectionStatusUI(connected);

    if (connected) {
        ui->statusBar->showMessage("Połączono z ESP");
    } else {
        ui->statusBar->showMessage("Rozłączono z ESP");
    }
}

// Handle TCP errors
void MainWindow::handleTcpError(const QString &errorMessage)
{
    ui->statusBar->showMessage("Błąd: " + errorMessage);

    // Optionally show a message box for critical errors
    QMessageBox::warning(this, "Błąd połączenia TCP", errorMessage);
}


void MainWindow::setupProportionalWidgets()
{
    // Create left and right sensor widgets
    leftSensorWidget = new ProportionalWidget(this);
    rightSensorWidget = new ProportionalWidget(this);

    // Find the tof widgets
    QStackedWidget* tofLeft = ui->tofLEFT;
    QStackedWidget* tofRight = ui->tofRIGHT;

    // Find the arrow widgets
    QStackedWidget* arrowLeft = ui->M1_arrow;
    QStackedWidget* arrowRight = ui->M2_arrow;

    // Setup the left sensor widget
    if (tofLeft && arrowLeft) {
        leftSensorWidget->setupTofWidget(tofLeft);
        leftSensorWidget->setupArrowWidget(arrowLeft, true);
    }

    // Setup the right sensor widget
    if (tofRight && arrowRight) {
        rightSensorWidget->setupTofWidget(tofRight);
        rightSensorWidget->setupArrowWidget(arrowRight, false);
    }

    // Find the original widgets to replace
    QWidget* widget16 = ui->widget_16;
    QWidget* widget18 = ui->widget_18;

    // Find the parent layout
    if (widget16 && widget18) {
        QWidget* parentWidget = widget16->parentWidget();
        QHBoxLayout* parentLayout = qobject_cast<QHBoxLayout*>(parentWidget->layout());

        if (parentLayout) {
            // Get the indices of the widgets to replace
            int index16 = parentLayout->indexOf(widget16);
            int index18 = parentLayout->indexOf(widget18);

            // Replace the widgets
            if (index16 >= 0) {
                parentLayout->removeWidget(widget16);
                parentLayout->insertWidget(index16, leftSensorWidget);
                widget16->hide();
            }

            if (index18 >= 0) {
                parentLayout->removeWidget(widget18);
                parentLayout->insertWidget(index18, rightSensorWidget);
                widget18->hide();
            }

            // Force layout update
            parentLayout->update();
        }
    }
    // QGridLayout* grid = qobject_cast<QGridLayout*>(ui->widRobot->layout());
    // if (grid) {
    //     grid->setRowStretch(0, 10);
    //     grid->setRowStretch(1, 30);
    //     grid->setRowStretch(2, 35);
    //     grid->setRowStretch(3, 6);
    //     grid->setRowStretch(4, 5);
    // }
    // Make sure the new widgets are visible
    leftSensorWidget->show();
    rightSensorWidget->show();
}



void MainWindow::adjustGridLayoutProportions()
{
    // Zakładamy, że używasz centralWidget i jego layoutem jest QGridLayout
    QGridLayout* grid = qobject_cast<QGridLayout*>(ui->centralwidget->layout());
    if (grid) {
        grid->setRowStretch(0, 10);
        grid->setRowStretch(1, 30);
        grid->setRowStretch(2, 35);
        grid->setRowStretch(3, 6);
        grid->setRowStretch(4, 5);
    }
}

