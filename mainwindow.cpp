#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Tworzenie obiektów zarządzających
    m_sensorData = new SensorData(this);
    m_csvManager = new CSVManager(m_sensorData, this);

    // Tworzenie managerów wykresów
    m_tofChartManager = new TofChartManager(m_sensorData, ui->tof_widget, this);
    m_lineChartManager = new LineChartMenager(m_sensorData, ui->lineS_widget, this);
    m_motorChartManager = new MotorChartManager(m_sensorData, ui->motors_widget, this);
    m_imuChartManager = new ImuChartManager(m_sensorData, ui->imu_widget, this);

    // Tworzenie managera wizualizacji
    m_visualManager = new VisualizationManager(m_sensorData, ui, this);

    // Inicjalizacja
    m_tofChartManager->setupChart();
    m_lineChartManager->setupChart();
    m_motorChartManager->setupChart();
    m_imuChartManager->setupChart();

    // Połączenia sygnałów i slotów
    connect(m_csvManager, &CSVManager::dataUpdated, this, &MainWindow::handleDataUpdated);
    connect(m_sensorData, &SensorData::dataChanged, m_visualManager, &VisualizationManager::updateAll);

    // Początkowa konfiguracja
    m_visualManager->setupArrows();
    m_visualManager->updateAll();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_load_fButton_clicked() {
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

void MainWindow::on_line1_button_clicked() {
    m_sensorData->setLineS1Active(!m_sensorData->getLineS1Active());
}

void MainWindow::on_tof1_button_clicked() {
    m_sensorData->setTof1(!m_sensorData->getTof1());
}

void MainWindow::on_dial_valueChanged(int value) {
    m_sensorData->setMotor1Speed(value);
}

void MainWindow::on_dial_2_valueChanged(int value) {
    m_sensorData->setMotor2Speed(value);
}

void MainWindow::on_x_dial_valueChanged(int value) {
    m_sensorData->setImuX(value);
}

void MainWindow::on_y_dial_valueChanged(int value) {
    m_sensorData->setImuY(value);
}
