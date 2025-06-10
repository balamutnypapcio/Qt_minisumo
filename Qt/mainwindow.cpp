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

    // Pobieranie i konfiguracja widgetów
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

    // Połączenie przycisku wyjścia z oknem potwierdzenia
    connect(ui->buttEXIT, &QPushButton::clicked, this, &MainWindow::showExitConfirmation);

    // Tworzenie obiektów zarządzających danymi i wizualizacją
    m_sensorData = new SensorData(this);
    m_csvManager = new CSVManager(m_sensorData, this);
    m_tofChartManager = new TofChartManager(m_sensorData, ui->chartTof, this);
    m_lineChartManager = new LineChartMenager(m_sensorData, ui->chartLS, this);
    m_motorChartManager = new MotorChartManager(m_sensorData, ui->chartMotors, this);
    m_imuChartManager = new ImuChartManager(m_sensorData, ui->chartIMU, this);
    m_visualManager = new VisualizationManager(m_sensorData, ui, this);

    // Inicjalizacja wykresów
    m_tofChartManager->setupChart();
    m_lineChartManager->setupChart();
    m_motorChartManager->setupChart();
    m_imuChartManager->setupChart();

    // Połączenia sygnałów i slotów
    m_tcpManager = new TCPManager(m_sensorData, this);
    connect(m_csvManager, &CSVManager::dataUpdated, this, &MainWindow::handleDataUpdated);
    connect(m_sensorData, &SensorData::dataChanged, m_visualManager, &VisualizationManager::updateAll);
    connect(m_tcpManager, &TCPManager::connectionStatusChanged, this, &MainWindow::handleConnectionStatusChanged);
    connect(m_tcpManager, &TCPManager::dataUpdated, this, &MainWindow::handleDataUpdated);

    // Początkowa konfiguracja wizualizacji
    m_visualManager->setupArrows();
    m_visualManager->updateAll();

    // Wczytanie preferencji języka i inicjalizacja tłumaczeń
    loadLanguagePreference();
    initializeTranslations();

    // Konfiguracja przycisku języka
    QPushButton* languageButton = findChild<QPushButton*>("languageButton");
    if (languageButton) {
        languageButton->setIcon(QIcon(m_isEnglish ? ":/new/image/img/eng.png" : ":/new/image/img/pl.png"));
        languageButton->setIconSize(QSize(32, 32));
        connect(languageButton, &QPushButton::clicked, this, &MainWindow::switchLanguage);
        languageButton->setStyleSheet("QPushButton { border-radius: 16px; min-width: 32px; max-width: 32px; min-height: 32px; max-height: 32px; }");
    }

    // Tworzenie obrotowych etykiet
    setupRotatedLabels();

    // Jeżeli zapisany język to polski, zastosuj tłumaczenie
    if (!m_isEnglish) {
        applyPolishTranslation();
    }
}

MainWindow::~MainWindow() {
    if (m_tcpManager && m_tcpManager->isConnected()) {
        m_tcpManager->disconnectFromDevice();
    }
    delete ui;
}

void MainWindow::on_buttLOADcsv_clicked() {
    QString filePath = QFileDialog::getOpenFileName(this, "Wybierz plik CSV", "", "Pliki CSV (*.csv);;Wszystkie pliki (*)");
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

void MainWindow::on_buttCONN_clicked()
{
    m_tofChartManager->clearChart();
    m_lineChartManager->clearChart();
    m_motorChartManager->clearChart();
    m_imuChartManager->clearChart();

    QString ipAddress = ui->lineIP->text();
    QString portText = ui->linePORT->text();
    QTextStream ts(&portText);
    quint16 port = 0;
    ts >> port;

    if (m_tcpManager->connectToDevice(ipAddress, port)) {
        ui->statusBar->showMessage("Próba połączenia z ESP...");
    } else {
        ui->statusBar->showMessage("Błąd podczas inicjowania połączenia");
    }
}

void MainWindow::on_buttDISS_clicked()
{
    m_tcpManager->disconnectFromDevice();
    ui->statusBar->showMessage("Rozłączono z ESP");
}

void MainWindow::handleConnectionStatusChanged(bool connected)
{
    m_visualManager->updateConnectionStatusUI(connected);
    if (connected) {
        ui->statusBar->showMessage("Połączono z ESP");
    } else {
        ui->statusBar->showMessage("Rozłączono z ESP");
    }
}

void MainWindow::on_buttSTOPcsv_clicked()
{
    if (m_csvManager) {
        m_csvManager->stopPlayback();
        ui->statusBar->showMessage("Odtwarzanie CSV zatrzymane");
    }
}

void MainWindow::handleTcpError(const QString &errorMessage)
{
    ui->statusBar->showMessage("Błąd: " + errorMessage);
    QMessageBox::warning(this, "Błąd połączenia TCP", errorMessage);
}

void MainWindow::showExitConfirmation()
{
    QMessageBox messageBox(this);
    if(m_isEnglish){
        messageBox.setWindowTitle("Do you want to quit?");
        messageBox.setText("Are you sure that you want to quit?");
        messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        messageBox.setDefaultButton(QMessageBox::No);
        messageBox.setButtonText(QMessageBox::Yes, "Yes");
        messageBox.setButtonText(QMessageBox::No, "No");
        QFont font = messageBox.font();
        font.setFamily("Arial");
        messageBox.setFont(font);
    }else{
        messageBox.setWindowTitle("Czy chcesz wyjsc?");
        messageBox.setText("Czy na pewno chcesz wyjść?");
        messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        messageBox.setDefaultButton(QMessageBox::No);
        messageBox.setButtonText(QMessageBox::Yes, "Tak");
        messageBox.setButtonText(QMessageBox::No, "Nie");
        QFont font = messageBox.font();
        font.setFamily("Arial");
        messageBox.setFont(font);
    }

    int ret = messageBox.exec();
    if (ret == QMessageBox::Yes) {
        qApp->quit();
    }
}

void MainWindow::setupProportionalWidgets()
{
    leftSensorWidget = new ProportionalWidget(this);
    rightSensorWidget = new ProportionalWidget(this);
    QStackedWidget* tofLeft = ui->tofLEFT;
    QStackedWidget* tofRight = ui->tofRIGHT;
    QStackedWidget* arrowLeft = ui->M1_arrow;
    QStackedWidget* arrowRight = ui->M2_arrow;

    if (tofLeft && arrowLeft) {
        leftSensorWidget->setupTofWidget(tofLeft);
        leftSensorWidget->setupArrowWidget(arrowLeft, true);
    }
    if (tofRight && arrowRight) {
        rightSensorWidget->setupTofWidget(tofRight);
        rightSensorWidget->setupArrowWidget(arrowRight, false);
    }

    QWidget* widget16 = ui->widget_16;
    QWidget* widget18 = ui->widget_18;

    if (widget16 && widget18) {
        QWidget* parentWidget = widget16->parentWidget();
        QHBoxLayout* parentLayout = qobject_cast<QHBoxLayout*>(parentWidget->layout());
        if (parentLayout) {
            int index16 = parentLayout->indexOf(widget16);
            int index18 = parentLayout->indexOf(widget18);

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
            parentLayout->update();
        }
    }
    leftSensorWidget->show();
    rightSensorWidget->show();
}

void MainWindow::adjustGridLayoutProportions()
{
    QGridLayout* grid = qobject_cast<QGridLayout*>(ui->centralwidget->layout());
    if (grid) {
        grid->setRowStretch(0, 10);
        grid->setRowStretch(1, 30);
        grid->setRowStretch(2, 35);
        grid->setRowStretch(3, 6);
        grid->setRowStretch(4, 5);
    }
}

void MainWindow::initializeTranslations()
{
    QList<QLabel*> labels = findChildren<QLabel*>();
    foreach (QLabel* label, labels) {
        m_englishTexts[label] = label->text();

        if (label->objectName() == "labelMotors") {
            m_polishTexts[label] = "PRĘDKOŚCI SILNIKÓW:";
        }
        else if(label->objectName() == "labelTof"){
            m_polishTexts[label] = "CZUJNIKI ODBICIOWE";
        }
        else if(label->objectName() == "labelLine"){
            m_polishTexts[label] = "CZUJNIKI LINI";
        }
        else if(label->objectName() == "labelImu"){
            m_polishTexts[label] = "CZUJNIK IMU";
        }
        else if(label->objectName() == "labelIMU"){
            m_polishTexts[label] = "WARTOSC";
        }
        else if(label->objectName() == "labelMotorsY"){
            m_polishTexts[label] = "WARTOŚĆ";
        }
        else if(label->objectName() == "labelTofY"){
            m_polishTexts[label] = "WARTOŚĆ";
        }
        else if(label->objectName() == "labelLSY"){
            m_polishTexts[label] = "WARTOŚĆ";
        }
        else if(label->objectName() == "labelIMUX"){
            m_polishTexts[label] = "CZAS (ms)";
        }
        else if(label->objectName() == "labelMotorsX"){
            m_polishTexts[label] = "CZAS (ms)";
        }
        else if(label->objectName() == "labelLSX"){
            m_polishTexts[label] = "CZAS (ms)";
        }
        else if(label->objectName() == "labelTofX"){
            m_polishTexts[label] = "CZAS (ms)";
        }
    }

    QList<QPushButton*> buttons = findChildren<QPushButton*>();
    foreach (QPushButton* button, buttons) {
        if (button->objectName() == "languageButton") continue;
        m_englishButtonTexts[button] = button->text();

        if (button->objectName() == "buttSTOPcsv") {
            m_polishButtonTexts[button] = "WSZYMAJ CSV";
        }
        else if (button->objectName() == "buttLOADcsv") {
            m_polishButtonTexts[button] = "WCZYTAJ CSV";
        }
        else if (button->objectName() == "buttCONN") {
            m_polishButtonTexts[button] = "POŁĄCZ";
        }
        else if (button->objectName() == "buttDISS") {
            m_polishButtonTexts[button] = "ROZŁĄCZ";
        }
        else if (button->objectName() == "buttEXIT") {
            m_polishButtonTexts[button] = "WYJŚCIE";
        }
    }
}

void MainWindow::switchLanguage()
{
    QPushButton* languageButton = qobject_cast<QPushButton*>(sender());
    if (!languageButton)
        return;

    m_isEnglish = !m_isEnglish;

    if (m_isEnglish) {
        languageButton->setIcon(QIcon(":/new/image/img/eng.png"));
    } else {
        languageButton->setIcon(QIcon(":/new/image/img/pl.png"));
    }

    QList<QLabel*> labels = findChildren<QLabel*>();
    foreach (QLabel* label, labels) {
        if (m_isEnglish) {
            if (m_englishTexts.contains(label)) {
                label->setText(m_englishTexts[label]);
            }
        } else {
            if (m_polishTexts.contains(label)) {
                label->setText(m_polishTexts[label]);
            }
        }
    }

    QList<QPushButton*> buttons = findChildren<QPushButton*>();
    foreach (QPushButton* button, buttons) {
        if (button == languageButton) continue;
        if (m_isEnglish) {
            if (m_englishButtonTexts.contains(button)) {
                button->setText(m_englishButtonTexts[button]);
            }
        } else {
            if (m_polishButtonTexts.contains(button)) {
                button->setText(m_polishButtonTexts[button]);
            }
        }
    }

    refreshParentWidgets();
    saveLanguagePreference();
}

void MainWindow::saveLanguagePreference()
{
    QSettings settings("Jakub Wilczynski", "SumoWilusRobotVisualizer");
    settings.setValue("language", m_isEnglish ? "en" : "pl");
}

void MainWindow::loadLanguagePreference()
{
    QSettings settings("Jakub Wilczynski", "SumoWilusRobotVisualizer");
    QString language = settings.value("language", "en").toString();
    m_isEnglish = (language == "en");
}

void MainWindow::applyPolishTranslation()
{
    QList<QLabel*> labels = findChildren<QLabel*>();
    foreach (QLabel* label, labels) {
        if (m_polishTexts.contains(label)) {
            label->setText(m_polishTexts[label]);
        }
    }

    QList<QPushButton*> buttons = findChildren<QPushButton*>();
    foreach (QPushButton* button, buttons) {
        if (button->objectName() == "languageButton") continue;
        if (m_polishButtonTexts.contains(button)) {
            button->setText(m_polishButtonTexts[button]);
        }
    }

    refreshParentWidgets();
}

void MainWindow::setupRotatedLabels(){
    RotatedLabel* labelIMU = new RotatedLabel(m_isEnglish ? "VALUE" : "WARTOŚĆ");
    labelIMU->setObjectName("rotatedLabelIMU");
    replaceLabel(ui->labelIMU, labelIMU);

    RotatedLabel* labelMotorsY = new RotatedLabel(m_isEnglish ? "VALUE" : "WARTOŚĆ");
    labelMotorsY->setObjectName("rotatedLabelMotorsY");
    replaceLabel(ui->labelMotorsY, labelMotorsY);

    RotatedLabel* labelLSY = new RotatedLabel(m_isEnglish ? "VALUE" : "WARTOŚĆ");
    labelLSY->setObjectName("rotatedLabelLSY");
    replaceLabel(ui->labelLSY, labelLSY);

    RotatedLabel* labelTofY = new RotatedLabel(m_isEnglish ? "VALUE" : "WARTOŚĆ");
    labelTofY->setObjectName("rotatedLabelTofY");
    replaceLabel(ui->labelTofY, labelTofY);

    m_englishTexts[labelIMU] = "VALUE";
    m_polishTexts[labelIMU] = "WARTOŚĆ";

    m_englishTexts[labelMotorsY] = "VALUE";
    m_polishTexts[labelMotorsY] = "WARTOŚĆ";

    m_englishTexts[labelLSY] = "VALUE";
    m_polishTexts[labelLSY] = "WARTOŚĆ";

    m_englishTexts[labelTofY] = "VALUE";
    m_polishTexts[labelTofY] = "WARTOŚĆ";
}

void MainWindow::replaceLabel(QLabel* oldLabel, QLabel* newLabel)
{
    if (!oldLabel || !newLabel) return;

    QWidget* parent = oldLabel->parentWidget();
    QLayout* layout = parent->layout();
    if (!layout) return;

    newLabel->setFont(oldLabel->font());
    newLabel->setStyleSheet(oldLabel->styleSheet());
    newLabel->setAlignment(oldLabel->alignment());

    if (QHBoxLayout* hLayout = qobject_cast<QHBoxLayout*>(layout)) {
        int index = layout->indexOf(oldLabel);
        if (index != -1) {
            oldLabel->hide();
            layout->removeWidget(oldLabel);
            hLayout->insertWidget(index, newLabel);
        }
    }
}

void MainWindow::refreshParentWidgets()
{
    QList<RotatedLabel*> rotatedLabels = findChildren<RotatedLabel*>();
    foreach(RotatedLabel* label, rotatedLabels) {
        if (label) {
            QWidget* parent = label->parentWidget();
            if (parent) {
                parent->update();
                QWidget* grandparent = parent->parentWidget();
                if (grandparent) {
                    grandparent->update();
                }
            }
        }
    }
}
