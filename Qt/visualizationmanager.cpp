#include "visualizationmanager.h"
#include "sensordata.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QStackedWidget>
#include <QLabel>
#include <QDebug>

VisualizationManager::VisualizationManager(SensorData* sensorData, Ui::MainWindow* ui, QObject* parent)
    : QObject(parent), m_sensorData(sensorData), m_ui(ui)
{
    // Initial setup of connection status
    updateConnectionStatusUI(false);
}

void VisualizationManager::setupArrows()
{
    // Set up the motor arrows on startup
    if (m_ui->M1_arrow) {
        m_ui->M1_arrow->setCurrentIndex(0); // Default to up
    }

    if (m_ui->M2_arrow) {
        m_ui->M2_arrow->setCurrentIndex(0); // Default to up
    }

    // Make sure arrows are visible
    if (m_ui->M1_arrow) {
        for (int i = 0; i < m_ui->M1_arrow->count(); i++) {
            QWidget* page = m_ui->M1_arrow->widget(i);
            page->setStyleSheet(
                "border-image: none; "
                "background: transparent; "
                "border: none; "
                "image: url(:/new/image/img/arrowUp.png); "
                "image-position: center;"
                );
        }
        m_ui->M1_arrow->widget(1)->setStyleSheet(
            "border-image: none; "
            "background: transparent; "
            "border: none; "
            "image: url(:/new/image/img/arrowDown.png); "
            "image-position: center;"
            );
    }

    if (m_ui->M2_arrow) {
        for (int i = 0; i < m_ui->M2_arrow->count(); i++) {
            QWidget* page = m_ui->M2_arrow->widget(i);
            page->setStyleSheet(
                "border-image: none; "
                "background: transparent; "
                "border: none; "
                "image: url(:/new/image/img/arrowUp.png); "
                "image-position: center;"
                );
        }
        m_ui->M2_arrow->widget(1)->setStyleSheet(
            "border-image: none; "
            "background: transparent; "
            "border: none; "
            "image: url(:/new/image/img/arrowDown.png); "
            "image-position: center;"
            );
    }
}

void VisualizationManager::updateConnectionStatusUI(bool connected)
{
    // Update WiFi indicator
    if (m_ui->wifi) {
        m_ui->wifi->setCurrentIndex(connected ? 0 : 1); // 0 = green, 1 = red
    }
}

void VisualizationManager::updateAll()
{
    // Update all visual elements based on current sensor data
    updateMotorArrows();
    updateTofSensors();
    updateMotorLabels();
    updateLsSensors();
}

void VisualizationManager::updateMotorArrows()
{
    // Get motor values from sensor data
    int motor1 = m_sensorData->getMotor1Speed();
    int motor2 = m_sensorData->getMotor2Speed();

    // Update Motor 1 arrow (left motor)
    if (m_ui->M1_arrow) {
        // Set appropriate arrow direction (up = forward, down = backward)
        m_ui->M1_arrow->setCurrentIndex(motor1 >= 0 ? 0 : 1);
    }

    // Update Motor 2 arrow (right motor)
    if (m_ui->M2_arrow) {
        // Set appropriate arrow direction (up = forward, down = backward)
        m_ui->M2_arrow->setCurrentIndex(motor2 >= 0 ? 0 : 1);
    }
}

void VisualizationManager::updateTofSensors()
{
    // Get TOF sensor values
    int tofLeft = m_sensorData->getTof1();
    int tofUpL = m_sensorData->getTof2();
    int tofUpR = m_sensorData->getTof3();
    int tofRight = m_sensorData->getTof4();


    // Update left TOF sensor visual
    if (m_ui->tofLEFT) {
        m_ui->tofLEFT->setCurrentIndex(tofLeft == 1 ? 1 : 0); // 0 = green, 1 = red
    }

    // Update right TOF sensor visual
    if (m_ui->tofRIGHT) {
        m_ui->tofRIGHT->setCurrentIndex(tofRight == 1 ? 0 : 1); // 0 = red, 1 = green
    }

    // Update upper-left TOF sensor visual
    if (m_ui->tofUpL) {
        m_ui->tofUpL->setCurrentIndex(tofUpL == 1 ? 1 : 0); // 0 = green, 1 = red
    }

    // Update upper-right TOF sensor visual
    if (m_ui->tofUpR) {
        m_ui->tofUpR->setCurrentIndex(tofUpR == 1 ? 1 : 0); // 0 = green, 1 = red
    }
}

void VisualizationManager::updateLsSensors()
{
    // Get Line sensor values
    bool lineLeft = m_sensorData->getLineS1Active();
    bool lineRight = m_sensorData->getLineS2Active();
    bool lineBottom = m_sensorData->getLineS3Active();

    // Update robot image based on active line sensors
    if (m_ui->stackedWidget) {
        if (lineLeft && lineRight && lineBottom) {
            // All sensors active
            m_ui->stackedWidget->setCurrentIndex(4); // AllLS
        } else if (lineLeft && lineRight) {
            // Left and right sensors active
            m_ui->stackedWidget->setCurrentIndex(1); // leftRightLS
        } else if (lineLeft && lineBottom) {
            // Left and bottom sensors active
            m_ui->stackedWidget->setCurrentIndex(2); // leftBottomLS
        } else if (lineRight && lineBottom) {
            // Right and bottom sensors active
            m_ui->stackedWidget->setCurrentIndex(3); // rightBottomLS
        } else if (lineLeft) {
            // Only left sensor active
            m_ui->stackedWidget->setCurrentIndex(7); // leftLS
        } else if (lineRight) {
            // Only right sensor active
            m_ui->stackedWidget->setCurrentIndex(6); // rightLS
        } else if (lineBottom) {
            // Only bottom sensor active
            m_ui->stackedWidget->setCurrentIndex(0); // bottomLS
        } else {
            // No line sensors active
            m_ui->stackedWidget->setCurrentIndex(5); // noLS
        }
    }
}



void VisualizationManager::updateMotorLabels()
{
    // Get motor values
    int motor1 = m_sensorData->getMotor1Speed();
    int motor2 = m_sensorData->getMotor2Speed();

    // Update motor PWM labels
    if (m_ui->labelMotor1PWM) {
        m_ui->labelMotor1PWM->setText(QString::number(abs(motor1)));
    }

    if (m_ui->labelMotor2PWM) {
        m_ui->labelMotor2PWM->setText(QString::number(abs(motor2)));
    }
}
