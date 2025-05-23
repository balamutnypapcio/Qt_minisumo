#include "csvmanager.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

CSVManager::CSVManager(SensorData* sensorData, QObject *parent)
    : QObject(parent), m_sensorData(sensorData) {
}

CSVManager::~CSVManager() {
    if (m_dataUpdateTimer) {
        m_dataUpdateTimer->stop();
        delete m_dataUpdateTimer;
    }
}

bool CSVManager::loadFromFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Nie udało się otworzyć pliku CSV";
        return false;
    }

    // Wyczyść poprzednie dane
    m_csvData.clear();
    m_currentRow = 0;

    QTextStream in(&file);
    QString line;
    bool firstLine = true;

    // Wczytywanie danych do csvData
    while (!in.atEnd()) {
        line = in.readLine();
        if (firstLine) {
            firstLine = false;
            continue;  // Pomijamy pierwszą linię z nagłówkami
        }
        QStringList data = line.split(",");
        // Walidacja danych
        if (data.size() >= 12) { // Upewnij się, że mamy wszystkie wymagane kolumny
            m_csvData.append(data);
        }
    }

    file.close();
    return true;
}

void CSVManager::startPlayback() {
    m_currentRow = 0;

    // Rozpocznij pomiar czasu dla odtwarzania
    m_playbackTimer.start();

    // Zatrzymaj istniejący timer, jeśli istnieje
    if (m_dataUpdateTimer) {
        m_dataUpdateTimer->stop();
        delete m_dataUpdateTimer;
    }

    m_dataUpdateTimer = new QTimer(this);
    connect(m_dataUpdateTimer, &QTimer::timeout, this, &CSVManager::updateData);
    m_dataUpdateTimer->start(20);
}


void CSVManager::stopPlayback()
{
    // Zatrzymaj timer aktualizujący dane
    if (m_dataUpdateTimer) {
        m_dataUpdateTimer->stop();
    }

    // Zresetuj timer pomiarowy
    m_playbackTimer.invalidate();

    // Emisja sygnału o zakończeniu odtwarzania
    emit playbackFinished();
}



void CSVManager::updateData() {
    // Jeśli nie mamy danych lub timer nie jest aktywny, wyjdź
    if (m_csvData.isEmpty() || !m_playbackTimer.isValid()) {
        return;
    }

    // Oblicz aktualny czas od początku odtwarzania
    qint64 elapsedMs = m_playbackTimer.elapsed();

    while (m_currentRow < m_csvData.size()) {
        int timeInCSV = m_csvData[m_currentRow][0].toInt();

        if (elapsedMs >= timeInCSV) {
            updateVariablesFromCSV(m_currentRow);
            m_currentRow++;
        } else {
            break; // Czekaj na następne wywołanie timera
        }
    }

    // Sprawdź, czy odtwarzanie zakończone
    if (m_currentRow >= m_csvData.size()) {
        m_dataUpdateTimer->stop();
        emit playbackFinished();
        qDebug() << "Odtwarzanie zakończone";
    }
}

void CSVManager::updateVariablesFromCSV(int row) {
    if (row < 0 || row >= m_csvData.size() || m_csvData[row].size() < 12) {
        qWarning() << "Nieprawidłowe dane w wierszu o indeksie" << row;
        return;
    }

    QStringList data = m_csvData[row];
    int time = data[0].toInt();

    try {
        bool tof1 = (data[1].toInt() == 1);
        bool tof2 = (data[2].toInt() == 1);
        bool tof3 = (data[3].toInt() == 1);
        bool tof4 = (data[4].toInt() == 1);
        bool lineS1 = (data[5].toInt() == 1);
        bool lineS2 = (data[6].toInt() == 1);
        bool lineS3 = (data[7].toInt() == 1);
        int motor1 = data[8].toInt();
        int motor2 = data[9].toInt();
        float imuX = data[10].toFloat();
        float imuY = data[11].toFloat();

        m_sensorData->updateAllData(tof1, tof2, tof3, tof4, lineS1, lineS2, lineS3, motor1, motor2, imuX, imuY);
        emit dataUpdated(time);
    } catch (...) {
        qWarning() << "Błąd przetwarzania wiersza" << row;
    }
}
