#include "visualizationmanager.h"
#include "sensordata.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QStackedWidget>
#include <QLabel>
#include <QDebug>
#include <QtMath>

VisualizationManager::VisualizationManager(SensorData* sensorData, Ui::MainWindow* ui, QObject* parent)
    : QObject(parent), m_sensorData(sensorData), m_ui(ui)
{
    updateConnectionStatusUI(false);
    m_ui->imuArrow->installEventFilter(this);
}

void VisualizationManager::setupArrows()
{
    // Silnik 1 (lewy) - domyślnie do góry
    if (m_ui->M1_arrow) {
        m_ui->M1_arrow->setCurrentIndex(0);
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

    // Silnik 2 (prawy) - domyślnie do góry
    if (m_ui->M2_arrow) {
        m_ui->M2_arrow->setCurrentIndex(0);
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
    if (m_ui->wifi) {
        m_ui->wifi->setCurrentIndex(connected ? 0 : 1); // 0 = zielona, 1 = czerwona
    }
}

void VisualizationManager::updateAll()
{
    updateMotorArrows();
    updateTofSensors();
    updateMotorLabels();
    updateLsSensors();
    updateImuArrow();
}

void VisualizationManager::updateMotorArrows()
{
    int motor1 = m_sensorData->getMotor1Speed();
    int motor2 = m_sensorData->getMotor2Speed();

    if (m_ui->M1_arrow) {
        m_ui->M1_arrow->setCurrentIndex(motor1 >= 0 ? 0 : 1);
    }
    if (m_ui->M2_arrow) {
        m_ui->M2_arrow->setCurrentIndex(motor2 >= 0 ? 0 : 1);
    }
}

void VisualizationManager::updateTofSensors()
{
    int tofLeft = m_sensorData->getTof1();
    int tofUpL = m_sensorData->getTof2();
    int tofUpR = m_sensorData->getTof3();
    int tofRight = m_sensorData->getTof4();

    if (m_ui->tofLEFT) {
        m_ui->tofLEFT->setCurrentIndex(tofLeft == 1 ? 1 : 0);
    }
    if (m_ui->tofRIGHT) {
        m_ui->tofRIGHT->setCurrentIndex(tofRight == 1 ? 0 : 1);
    }
    if (m_ui->tofUpL) {
        m_ui->tofUpL->setCurrentIndex(tofUpL == 1 ? 1 : 0);
    }
    if (m_ui->tofUpR) {
        m_ui->tofUpR->setCurrentIndex(tofUpR == 1 ? 1 : 0);
    }
}

void VisualizationManager::updateLsSensors()
{
    bool lineLeft = m_sensorData->getLineS1Active();
    bool lineRight = m_sensorData->getLineS2Active();
    bool lineBottom = m_sensorData->getLineS3Active();

    QString imagePath;
    if (lineLeft && lineRight && lineBottom)
        imagePath = ":/new/image/img/robotAllLS.png";
    else if (lineLeft && lineRight)
        imagePath = ":/new/image/img/robotLeftRightLS.png";
    else if (lineLeft && lineBottom)
        imagePath = ":/new/image/img/robotLeftBottomLS.png";
    else if (lineRight && lineBottom)
        imagePath = ":/new/image/img/robotRightBottomLS.png";
    else if (lineLeft)
        imagePath = ":/new/image/img/robotLeftLS.png";
    else if (lineRight)
        imagePath = ":/new/image/img/robotRightLS.png";
    else if (lineBottom)
        imagePath = ":/new/image/img/robotBottomLS.png";
    else
        imagePath = ":/new/image/img/robotNoLs.png";

    QString styleSheet = QString("image: url(%1); image-position: top center; border: none;").arg(imagePath);
    m_ui->robotView->setStyleSheet(styleSheet);
}

void VisualizationManager::updateMotorLabels()
{
    int motor1 = m_sensorData->getMotor1Speed();
    int motor2 = m_sensorData->getMotor2Speed();

    if (m_ui->labelMotor1PWM) {
        m_ui->labelMotor1PWM->setText("PWM 1: " + QString::number(abs(motor1)) + "%");
    }
    if (m_ui->labelMotor2PWM) {
        m_ui->labelMotor2PWM->setText("PWM 2: " + QString::number(abs(motor2)) + "%");
    }
}

void VisualizationManager::updateImuArrow()
{
    if (!m_ui->imuArrow) {
        qDebug() << "Błąd: Widget imuArrow nie istnieje!";
        return;
    }

    float imuX = m_sensorData->getImuX();
    float imuY = m_sensorData->getImuY();

    float magnitude = sqrt(imuX*imuX + imuY*imuY);

    if (magnitude == 0) {
        m_ui->imuArrow->setPixmap(QPixmap());
        return;
    }

    float angle = atan2(imuY, imuX) * (180.0 / M_PI);

    float minScale = 0.1;
    float maxScale = 0.8;
    magnitude = qMax(1.0f, magnitude);

    float scaleFactor;
    if (magnitude <= 1.0f) {
        scaleFactor = minScale;
    } else {
        scaleFactor = minScale + (maxScale - minScale) * (magnitude - 1.0f) / 99.0f;
    }
    scaleFactor = qMin(scaleFactor, maxScale);

    if (m_originalImuPixmap.isNull()) {
        m_originalImuPixmap = QPixmap(":/new/image/img/arrowIMU.png");
        if (m_originalImuPixmap.isNull()) {
            qDebug() << "Błąd: Nie można załadować obrazka arrowIMU!";
            return;
        }
    }

    int labelWidth = m_ui->imuArrow->width();
    int labelHeight = m_ui->imuArrow->height();
    if (labelWidth <= 0 || labelHeight <= 0) {
        qDebug() << "Ostrzeżenie: QLabel ma zerowy rozmiar!";
        return;
    }

    float baseScaleFactor;
    if (labelWidth < 20 || labelHeight < 20) {
        baseScaleFactor = 1.0;
    } else {
        baseScaleFactor = qMin(labelWidth, labelHeight) /
                          qMax((float)m_originalImuPixmap.width(), (float)m_originalImuPixmap.height());
    }

    float combinedScaleFactor = baseScaleFactor * scaleFactor;
    combinedScaleFactor = qMax(combinedScaleFactor, 0.1f);

    QTransform transform;
    transform.translate(m_originalImuPixmap.width() / 2.0, m_originalImuPixmap.height() / 2.0);
    transform.rotate(angle);
    transform.translate(-m_originalImuPixmap.width() / 2.0, -m_originalImuPixmap.height() / 2.0);
    transform.scale(combinedScaleFactor, combinedScaleFactor);

    QPixmap transformedPixmap = m_originalImuPixmap.transformed(transform, Qt::SmoothTransformation);

    QPixmap finalPixmap(qMax(1, labelWidth), qMax(1, labelHeight));
    finalPixmap.fill(Qt::transparent);

    int xPos = (labelWidth - transformedPixmap.width()) / 2;
    int yPos;
    if (labelHeight > labelWidth) {
        int squareSize = labelWidth;
        int verticalOffset = (squareSize - transformedPixmap.height()) / 2;
        yPos = verticalOffset;
    } else {
        yPos = (labelHeight - transformedPixmap.height()) / 2;
    }
    xPos = qMax(0, xPos);
    yPos = qMax(0, yPos);

    QPainter painter(&finalPixmap);
    painter.setOpacity(0.7);
    painter.drawPixmap(xPos, yPos, transformedPixmap);
    painter.end();

    m_ui->imuArrow->setPixmap(finalPixmap);
}

bool VisualizationManager::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_ui->imuArrow && event->type() == QEvent::Resize) {
        updateImuArrow();
        return true;
    }
    return QObject::eventFilter(obj, event);
}
