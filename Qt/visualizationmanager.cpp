#include "visualizationmanager.h"
#include <QTransform>
#include <QPainter>
#include <QDebug>

VisualizationManager::VisualizationManager(SensorData* sensorData, Ui::MainWindow* ui, QObject *parent)
    : QObject(parent), m_sensorData(sensorData), m_ui(ui)
{
    // Load original arrow pixmaps
    m_originalArrowPixmap = QPixmap(":/new/image/img/arrowUp.png");
    m_originalArrowIMU = QPixmap(":/new/image/img/arrowIMU.png");

    if (m_originalArrowPixmap.isNull()) {
        qDebug() << "Failed to load arrowUP.png";
    }

    if (m_originalArrowIMU.isNull()) {
        qDebug() << "Failed to load arrow_imu.png";
    }

    // Set up initial visibility and state
    setupArrows();

    // Connect signals and slots for data updates
    connect(m_sensorData, &SensorData::motorDataChanged, this, &VisualizationManager::updateMotorArrows);
    connect(m_sensorData, &SensorData::motorDataChanged, this, &VisualizationManager::updateMotorSpeed);
    connect(m_sensorData, &SensorData::imuDataChanged, this, &VisualizationManager::updateImuArrow);
    connect(m_sensorData, &SensorData::tofDataChanged, this, &VisualizationManager::updateVisibility);
    connect(m_sensorData, &SensorData::lineSDataChanged, this, &VisualizationManager::updateVisibility);
    connect(m_sensorData, &SensorData::dataChanged, this, &VisualizationManager::updateAll);
}

void VisualizationManager::setupArrows()
{
    // Initialize arrows with default properties
    updateVisibility();
    updateMotorArrows();
    updateImuArrow();
    updateMotorSpeed();
}

void VisualizationManager::updateVisibility()
{
    // Update TOF sensors visibility using QStackedWidget
    if (m_ui->tofUpL) {
        m_ui->tofUpL->setCurrentIndex(m_sensorData->getTof1() ? 1 : 0);
    }

    if (m_ui->tofUpR) {
        m_ui->tofUpR->setCurrentIndex(m_sensorData->getTof2() ? 1 : 0);
    }

    if (m_ui->tofLEFT) {
        m_ui->tofLEFT->setCurrentIndex(m_sensorData->getTof3() ? 1 : 0);
    }

    if (m_ui->tofRIGHT) {
        m_ui->tofRIGHT->setCurrentIndex(m_sensorData->getTof4() ? 1 : 0);
    }
}

void VisualizationManager::updateMotorArrows()
{
    updateArrowSize1();
    updateArrowSize2();
}

void VisualizationManager::updateArrowSize1()
{
    if (!m_ui->M1_arrow) {
        return;
    }

    int speed = m_sensorData->getMotor1Speed();

    if (speed == 0) {
        m_ui->M1_arrow->setVisible(false);
        return;
    }

    m_ui->M1_arrow->setVisible(true);

    // Ustaw kierunek strzałki (UP/DOWN)
    m_ui->M1_arrow->setCurrentIndex(speed > 0 ? 0 : 1);

    // Oblicz skalę na podstawie bezwzględnej wartości prędkości (0-100%)
    float scale = std::min(std::abs(speed), 100) / 100.0f;

    // Minimalna skala to 30%, maksymalna 100%
    scale = 0.3f + (scale * 0.7f);

    // Pobierz aktualny widget ze stosu
    QWidget* currentWidget = m_ui->M1_arrow->currentWidget();
    if (currentWidget) {
        // Ustaw minimalny rozmiar widgetu
        int baseSize = 30; // bazowy rozmiar w pikselach
        int scaledSize = baseSize + (int)(baseSize * scale);

        currentWidget->setMinimumSize(scaledSize, scaledSize);
        currentWidget->setMaximumSize(scaledSize, scaledSize);

        // Wymuś przerysowanie
        currentWidget->updateGeometry();
    }
}

void VisualizationManager::updateArrowSize2()
{
    if (!m_ui->M2_arrow) {
        return;
    }

    int speed = m_sensorData->getMotor2Speed();

    if (speed == 0) {
        m_ui->M2_arrow->setVisible(false);
        return;
    }

    m_ui->M2_arrow->setVisible(true);

    // Ustaw kierunek strzałki (UP/DOWN)
    m_ui->M2_arrow->setCurrentIndex(speed > 0 ? 0 : 1);

    // Oblicz skalę na podstawie bezwzględnej wartości prędkości (0-100%)
    float scale = std::min(std::abs(speed), 100) / 100.0f;

    // Minimalna skala to 30%, maksymalna 100%
    scale = 0.3f + (scale * 0.7f);

    // Pobierz aktualny widget ze stosu
    QWidget* currentWidget = m_ui->M2_arrow->currentWidget();
    if (currentWidget) {
        // Ustaw minimalny rozmiar widgetu
        int baseSize = 30; // bazowy rozmiar w pikselach
        int scaledSize = baseSize + (int)(baseSize * scale);

        currentWidget->setMinimumSize(scaledSize, scaledSize);
        currentWidget->setMaximumSize(scaledSize, scaledSize);

        // Wymuś przerysowanie
        currentWidget->updateGeometry();
    }
}

void VisualizationManager::updateImuArrow()
{
    // IMU functionality can be implemented later if needed
}

void VisualizationManager::updateMotorSpeed()
{
    // Update motor speed labels
    if (m_ui->labelMotor1PWM) {
        m_ui->labelMotor1PWM->setText(QString::number(m_sensorData->getMotor1Speed()));
    }

    if (m_ui->labelMotor2PWM) {
        m_ui->labelMotor2PWM->setText(QString::number(m_sensorData->getMotor2Speed()));
    }
}

void VisualizationManager::updateAll()
{
    updateVisibility();
    updateMotorArrows();
    updateImuArrow();
    updateMotorSpeed();
}

void VisualizationManager::updateConnectionStatusUI(bool connected)
{
    // Update button states based on connection status
    m_ui->buttCONN->setEnabled(!connected);
    m_ui->buttDISS->setEnabled(connected);

    // Update connection indicator using QStackedWidget
    if (m_ui->wifi) {
        m_ui->wifi->setCurrentIndex(connected ? 0 : 1); // 0 = green, 1 = red
    }
}
