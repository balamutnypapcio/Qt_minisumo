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
    // // Initialize arrows with default properties
    // if (m_ui->motor1_arrow && !m_originalArrowPixmap.isNull()) {
    //     m_ui->motor1_arrow->setPixmap(m_originalArrowPixmap.scaled(
    //         m_ui->motor1_arrow->width(),
    //         m_ui->motor1_arrow->height(),
    //         Qt::KeepAspectRatio,
    //         Qt::SmoothTransformation));
    // }

    // if (m_ui->motor2_arrow && !m_originalArrowPixmap.isNull()) {
    //     m_ui->motor2_arrow->setPixmap(m_originalArrowPixmap.scaled(
    //         m_ui->motor2_arrow->width(),
    //         m_ui->motor2_arrow->height(),
    //         Qt::KeepAspectRatio,
    //         Qt::SmoothTransformation));
    // }

    // if (m_ui->imu_arrow && !m_originalArrowIMU.isNull()) {
    //     m_ui->imu_arrow->setPixmap(m_originalArrowIMU.scaled(
    //         m_ui->imu_arrow->width(),
    //         m_ui->imu_arrow->height(),
    //         Qt::KeepAspectRatio,
    //         Qt::SmoothTransformation));
    //     setArrowOpacity(m_ui->imu_arrow);
    // }

    // Update initial visibility based on sensor data
    updateVisibility();
    updateMotorArrows();
    updateImuArrow();
    updateMotorSpeed();
}

void VisualizationManager::updateVisibility()
{
    // // Update TOF sensors visibility
    // if (m_ui->tof_Red_1) {
    //     m_ui->tof_Red_1->setVisible(m_sensorData->getTof1());
    // }

    // if (m_ui->tof_Red_2) {
    //     m_ui->tof_Red_2->setVisible(m_sensorData->getTof2());
    // }

    // if (m_ui->tof_Red_3) {
    //     m_ui->tof_Red_3->setVisible(m_sensorData->getTof3());
    // }

    // if (m_ui->tof_Red_4) {
    //     m_ui->tof_Red_4->setVisible(m_sensorData->getTof4());
    // }

    // // Update Line sensors visibility
    // if (m_ui->lineS_Active_1) {
    //     m_ui->lineS_Active_1->setVisible(m_sensorData->getLineS1Active());
    // }

    // if (m_ui->lineS_Active_2) {
    //     m_ui->lineS_Active_2->setVisible(m_sensorData->getLineS2Active());
    // }

    // if (m_ui->lineS_Active_3) {
    //     m_ui->lineS_Active_3->setVisible(m_sensorData->getLineS3Active());
    // }
}

void VisualizationManager::updateMotorArrows()
{
    updateArrowSize1();
    updateArrowSize2();

    // Set opacity for motor arrows based on speed
    // if (m_ui->motor1_arrow) {
    //     setArrowOpacity(m_ui->motor1_arrow);
    // }

    // if (m_ui->motor2_arrow) {
    //     setArrowOpacity(m_ui->motor2_arrow);
    // }
}

void VisualizationManager::updateArrowSize1()
{
    // if (!m_ui->motor1_arrow || m_originalArrowPixmap.isNull()) {
    //     return;
    // }

    // if(m_sensorData->getMotor1Speed() == 0){
    //     m_ui->motor1_arrow->setVisible(false);
    //     return;
    // }

    // // Create transformed pixmap
    // QTransform transform;

    // // Rotate if motor speed is negative
    // if (m_sensorData->getMotor1Speed() < 0) {
    //     transform.rotate(180);
    // }

    // QPixmap rotatedPixmap = m_originalArrowPixmap.transformed(transform);

    // // Get label dimensions
    // int labelWidth = m_ui->motor1_arrow->width();
    // int labelHeight = m_ui->motor1_arrow->height();

    // // Calculate scale factor based on motor speed (0-100%)
    // float speedPercentage = std::min(std::abs(m_sensorData->getMotor1Speed()), 100) / 100.0f;

    // // Calculate maximum possible size to fit within the label
    // // We'll scale to a maximum of 95% of label size to ensure it fits
    // int maxWidth = labelWidth * 0.95;
    // int maxHeight = labelHeight * 0.95;

    // // Calculate scaled dimensions while maintaining aspect ratio
    // float originalAspectRatio = (float)rotatedPixmap.width() / rotatedPixmap.height();

    // // Calculate max size while maintaining aspect ratio and scaling by speed
    // int newWidth, newHeight;

    // if (labelWidth / originalAspectRatio <= labelHeight) {
    //     // Width is the limiting factor
    //     newWidth = maxWidth * speedPercentage;
    //     newHeight = newWidth / originalAspectRatio;
    // } else {
    //     // Height is the limiting factor
    //     newHeight = maxHeight * speedPercentage;
    //     newWidth = newHeight * originalAspectRatio;
    // }

    // // Ensure minimum visibility when speed is not zero
    // if (speedPercentage > 0) {
    //     // Set minimum size to 10% of maximum when speed is not zero
    //     newWidth = std::max(newWidth, (int)(maxWidth * 0.1));
    //     newHeight = std::max(newHeight, (int)(maxHeight * 0.1));
    // }

    // // Scale the pixmap to fit
    // QPixmap scaledPixmap = rotatedPixmap.scaled(
    //     newWidth,
    //     newHeight,
    //     Qt::KeepAspectRatio,
    //     Qt::SmoothTransformation);

    // m_ui->motor1_arrow->setPixmap(scaledPixmap);

    // // Center the pixmap in the label
    // m_ui->motor1_arrow->setAlignment(Qt::AlignCenter);
    // m_ui->motor1_arrow->setVisible(true);
}

void VisualizationManager::updateArrowSize2()
{
    // if (!m_ui->motor2_arrow || m_originalArrowPixmap.isNull()) {
    //     return;
    // }

    // if(m_sensorData->getMotor1Speed() == 0){
    //     m_ui->motor2_arrow->setVisible(false);
    //     return;
    // }

    // // Create transformed pixmap
    // QTransform transform;

    // // Rotate if motor speed is negative
    // if (m_sensorData->getMotor2Speed() < 0) {
    //     transform.rotate(180);
    // }

    // QPixmap rotatedPixmap = m_originalArrowPixmap.transformed(transform);

    // // Get label dimensions
    // int labelWidth = m_ui->motor2_arrow->width();
    // int labelHeight = m_ui->motor2_arrow->height();

    // // Calculate scale factor based on motor speed (0-100%)
    // float speedPercentage = std::min(std::abs(m_sensorData->getMotor2Speed()), 100) / 100.0f;

    // // Calculate maximum possible size to fit within the label
    // // We'll scale to a maximum of 95% of label size to ensure it fits
    // int maxWidth = labelWidth * 0.95;
    // int maxHeight = labelHeight * 0.95;

    // // Calculate scaled dimensions while maintaining aspect ratio
    // float originalAspectRatio = (float)rotatedPixmap.width() / rotatedPixmap.height();

    // // Calculate max size while maintaining aspect ratio and scaling by speed
    // int newWidth, newHeight;

    // if (labelWidth / originalAspectRatio <= labelHeight) {
    //     // Width is the limiting factor
    //     newWidth = maxWidth * speedPercentage;
    //     newHeight = newWidth / originalAspectRatio;
    // } else {
    //     // Height is the limiting factor
    //     newHeight = maxHeight * speedPercentage;
    //     newWidth = newHeight * originalAspectRatio;
    // }

    // // Ensure minimum visibility when speed is not zero
    // if (speedPercentage > 0) {
    //     // Set minimum size to 10% of maximum when speed is not zero
    //     newWidth = std::max(newWidth, (int)(maxWidth * 0.1));
    //     newHeight = std::max(newHeight, (int)(maxHeight * 0.1));
    // }

    // // Scale the pixmap to fit
    // QPixmap scaledPixmap = rotatedPixmap.scaled(
    //     newWidth,
    //     newHeight,
    //     Qt::KeepAspectRatio,
    //     Qt::SmoothTransformation);

    // m_ui->motor2_arrow->setPixmap(scaledPixmap);

    // // Center the pixmap in the label
    // m_ui->motor2_arrow->setAlignment(Qt::AlignCenter);
    // m_ui->motor2_arrow->setVisible(true);
}

void VisualizationManager::setArrowOpacity(QLabel* arrowLabel)
{
    if (!arrowLabel) {
        return;
    }

    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(this);
    effect->setOpacity(0.7);  // przezroczystość na 70%

    arrowLabel->setGraphicsEffect(effect);
}

void VisualizationManager::updateImuArrow()
{
    // if (!m_ui->imu_arrow || m_originalArrowIMU.isNull()) {
    //     return;
    // }

    // // Check if both IMU values are zero
    // if (m_sensorData->getImuX() == 0 && m_sensorData->getImuY() == 0) {
    //     m_ui->imu_arrow->setVisible(false);
    //     return;
    // }

    // // Calculate length of the vector
    // float length = sqrt(m_sensorData->getImuX() * m_sensorData->getImuX() +
    //                     m_sensorData->getImuY() * m_sensorData->getImuY());

    // // Calculate scale factor based on length (10% to 80% of max size)
    // float scaleFactor = 0.10 + (std::min(length, 100.0f) / 100.0) * 0.70;

    // // Calculate rotation angle based on IMU data
    // float angle = atan2(m_sensorData->getImuY(), m_sensorData->getImuX()) * 180 / M_PI;

    // // Create transformed pixmap
    // QTransform transform;
    // transform.rotate(angle);
    // QPixmap rotatedPixmap = m_originalArrowIMU.transformed(transform);

    // // Get label dimensions
    // int labelWidth = m_ui->imu_arrow->width();
    // int labelHeight = m_ui->imu_arrow->height();

    // // Calculate maximum possible size to fit within the label (95% of label size)
    // int maxWidth = labelWidth * 0.95;
    // int maxHeight = labelHeight * 0.95;

    // // Calculate scaled dimensions while maintaining aspect ratio
    // float originalAspectRatio = (float)rotatedPixmap.width() / rotatedPixmap.height();

    // // Calculate max size while maintaining aspect ratio and scaling by length
    // int newWidth, newHeight;

    // if (labelWidth / originalAspectRatio <= labelHeight) {
    //     // Width is the limiting factor
    //     newWidth = maxWidth * scaleFactor;
    //     newHeight = newWidth / originalAspectRatio;
    // } else {
    //     // Height is the limiting factor
    //     newHeight = maxHeight * scaleFactor;
    //     newWidth = newHeight * originalAspectRatio;
    // }

    // // Ensure minimum visibility when length is not zero
    // if (length > 0) {
    //     // Set minimum size to 10% of maximum when length is not zero
    //     newWidth = std::max(newWidth, (int)(maxWidth * 0.1));
    //     newHeight = std::max(newHeight, (int)(maxHeight * 0.1));
    // }

    // // Scale the pixmap with our calculated dimensions
    // QPixmap scaledPixmap = rotatedPixmap.scaled(
    //     newWidth,
    //     newHeight,
    //     Qt::KeepAspectRatio,
    //     Qt::SmoothTransformation);

    // // Set the pixmap to the label
    // m_ui->imu_arrow->setPixmap(scaledPixmap);

    // // Center the pixmap in the label
    // m_ui->imu_arrow->setAlignment(Qt::AlignCenter);
    // m_ui->imu_arrow->setVisible(true);
}

void VisualizationManager::updateMotorSpeed()
{
    // Update motor speed labels if they exist
    if (m_ui->labelMotor1PWM) {
        m_ui->labelMotor1PWM->setText(QString::number(m_sensorData->getMotor1Speed()));
    }

    if (m_ui->labelMotor2PWM) {
        m_ui->labelMotor2PWM->setText(QString::number(m_sensorData->getMotor2Speed()));
    }
}

void VisualizationManager::updateAll()
{
    // Update all visualization elements
    updateVisibility();
    updateMotorArrows();
    updateImuArrow();
    updateMotorSpeed();
}


// Update UI based on connection status
void VisualizationManager::updateConnectionStatusUI(bool connected)
{
    // Update button states based on connection status
    // Adjust these to match your actual UI element names
    m_ui->buttCONN->setEnabled(!connected);
    m_ui->buttDISS->setEnabled(connected);

    // If you have a connection indicator, update it
    // For example:
    // if(connected == true){
    //     m_ui->connectRed->setVisible(false);
    // }else{
    //      m_ui->connectRed->setVisible(true);
    // }
}


// void VisualizationManager::on_buttEXIT_clicked()
// {
//     QApplication::quit();
// }
