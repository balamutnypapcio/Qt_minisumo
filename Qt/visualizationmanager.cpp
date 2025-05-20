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
    m_ui->imuArrow->installEventFilter(this);
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
    updateImuArrow();
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

    // Wybierz odpowiednią ścieżkę obrazu na podstawie aktywnych czujników
    QString imagePath;

    if (lineLeft && lineRight && lineBottom) {
        imagePath = ":/new/image/img/robotAllLS.png"; // Wszystkie czujniki aktywne
    } else if (lineLeft && lineRight) {
        imagePath = ":/new/image/img/robotLeftRightLS.png"; // Lewy i prawy aktywne
    } else if (lineLeft && lineBottom) {
        imagePath = ":/new/image/img/robotLeftBottomLS.png"; // Lewy i dolny aktywne
    } else if (lineRight && lineBottom) {
        imagePath = ":/new/image/img/robotRightBottomLS.png"; // Prawy i dolny aktywne
    } else if (lineLeft) {
        imagePath = ":/new/image/img/robotLeftLS.png"; // Tylko lewy aktywny
    } else if (lineRight) {
        imagePath = ":/new/image/img/robotRightLS.png"; // Tylko prawy aktywny
    } else if (lineBottom) {
        imagePath = ":/new/image/img/robotBottomLS.png"; // Tylko dolny aktywny
    } else {
        imagePath = ":/new/image/img/robotNoLs.png"; // Żaden czujnik nie jest aktywny
    }

    // Plik istnieje - utwórz arkusz stylów CSS z obrazem
    QString styleSheet = QString("image: url(%1); "
                               "image-position: top center; "
                               "border: none;").arg(imagePath);

    // Ustaw arkusz stylów dla widgetu robotView
    m_ui->robotView->setStyleSheet(styleSheet);
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



void VisualizationManager::updateImuArrow()
{
    // Sprawdź czy widget imuArrow istnieje
    if (!m_ui->imuArrow) {
        qDebug() << "Błąd: Widget imuArrow nie istnieje!";
        return;
    }

    // Pobierz dane z sensorów IMU
    float imuX = m_sensorData->getImuX();
    float imuY = m_sensorData->getImuY();

    // Oblicz długość wektora (magnitudę)
    float magnitude = sqrt(imuX*imuX + imuY*imuY);

    // Jeśli długość jest 0, nie wyświetlaj strzałki
    if (magnitude == 0) {
        // Ustawiamy pustą pixmapę
        m_ui->imuArrow->setPixmap(QPixmap());
        return;
    }

    // Oblicz kąt na podstawie wartości X i Y (w stopniach)
    float angle = atan2(imuY, imuX) * (180.0 / M_PI);

    // Normalizacja długości do wartości skali (od 10% do 80% przy wartości 100)
    // Skalowanie liniowe: 0->0, 1->0.1, 100->0.8
    float minScale = 0.1; // 10% maksymalnej długości
    float maxScale = 0.8; // 80% maksymalnej długości przy wartości 100

    // Zapewniamy, że magnitude będzie co najmniej 1 (dla minimalnego rozmiaru)
    magnitude = qMax(1.0f, magnitude);

    // Normalizacja długości do wartości skali
    float scaleFactor;
    if (magnitude <= 1.0f) {
        scaleFactor = minScale;
    } else {
        scaleFactor = minScale + (maxScale - minScale) * (magnitude - 1.0f) / 99.0f;
    }

    // Wartość nie może przekroczyć maxScale
    scaleFactor = qMin(scaleFactor, maxScale);

    // Pobierz oryginalny obrazek, jeśli nie jest jeszcze przechowywany
    if (m_originalImuPixmap.isNull()) {
        m_originalImuPixmap = QPixmap(":/new/image/img/arrowIMU.png");

        if (m_originalImuPixmap.isNull()) {
            qDebug() << "Błąd: Nie można załadować obrazka arrowIMU!";
            return;
        }
    }

    // Pobierz wymiary QLabela
    int labelWidth = m_ui->imuArrow->width();
    int labelHeight = m_ui->imuArrow->height();

    // Upewnij się, że QLabel ma niezerowy rozmiar
    if (labelWidth <= 0 || labelHeight <= 0) {
        qDebug() << "Ostrzeżenie: QLabel ma zerowy rozmiar!";
        return;
    }

    // Ustaw minimalny rozmiar, aby strzałka nie zniknęła w bardzo małym oknie
    // Jeśli rozmiar QLabel jest mniejszy niż 20x20 pikseli, użyj stałej wartości skalowania
    float baseScaleFactor;
    if (labelWidth < 20 || labelHeight < 20) {
        // Użyj minimalnej skali dla małego QLabel
        baseScaleFactor = 1.0; // Minimalna skala
    } else {
        // Normalne obliczenie skali dla odpowiedniego rozmiaru QLabel
        baseScaleFactor = qMin(labelWidth, labelHeight) /
                          qMax((float)m_originalImuPixmap.width(), (float)m_originalImuPixmap.height());
    }

    // Ostateczny współczynnik skalowania to iloczyn bazowego współczynnika i współczynnika długości
    float combinedScaleFactor = baseScaleFactor * scaleFactor;

    // Ustaw minimalny współczynnik skalowania, aby obrazek był zawsze widoczny
    combinedScaleFactor = qMax(combinedScaleFactor, 0.1f);

    // Utwórz obiekt transformacji
    QTransform transform;

    // Obróć wokół środka obrazka
    transform.translate(m_originalImuPixmap.width() / 2.0, m_originalImuPixmap.height() / 2.0);
    transform.rotate(angle);
    transform.translate(-m_originalImuPixmap.width() / 2.0, -m_originalImuPixmap.height() / 2.0);

    // Zastosuj skalowanie
    transform.scale(combinedScaleFactor, combinedScaleFactor);

    // Zastosuj transformację do oryginalnego obrazka
    QPixmap transformedPixmap = m_originalImuPixmap.transformed(transform, Qt::SmoothTransformation);

    // Utworzenie przezroczystego obrazka o wymiarach QLabela
    QPixmap finalPixmap(qMax(1, labelWidth), qMax(1, labelHeight));
    finalPixmap.fill(Qt::transparent);

    // Oblicz pozycję obrazka tak, aby był wyśrodkowany w QLabelu
    int xPos = (labelWidth - transformedPixmap.width()) / 2;
    int yPos = (labelHeight - transformedPixmap.height()) / 2;

    // Upewnij się, że pozycje nie są ujemne (może się zdarzyć przy bardzo małym QLabel)
    xPos = qMax(0, xPos);
    yPos = qMax(0, yPos);

    // Rysuj przekształcony obrazek z 30% przezroczystością na środku
    QPainter painter(&finalPixmap);
    painter.setOpacity(0.7); // 70% nieprzezroczystości = 30% przezroczystości
    painter.drawPixmap(xPos, yPos, transformedPixmap);
    painter.end();

    // Ustaw finalny obrazek w QLabel
    m_ui->imuArrow->setPixmap(finalPixmap);
}




bool VisualizationManager::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_ui->imuArrow && event->type() == QEvent::Resize) {
        // Odśwież obrazek po zmianie rozmiaru QLabel
        updateImuArrow();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

