#ifndef CSVMANAGER_H
#define CSVMANAGER_H

#include <QObject>
#include <QElapsedTimer>
#include <QVector>
#include <QTimer>
#include <QStringList>
#include "sensordata.h"

/**
 * @class CSVManager
 * @brief Zarządza wczytywaniem, odtwarzaniem i symulacją danych z pliku CSV dla obiektów SensorData.
 *
 * Klasa umożliwia:
 *  - Wczytywanie danych eksperymentalnych z pliku CSV (z pominięciem nagłówka).
 *  - Odtwarzanie tych danych w czasie rzeczywistym (symulacja).
 *  - Emitowanie sygnałów aktualizacji danych (np. do wykresów lub innych komponentów GUI).
 *  - Synchronizację odtwarzania z czasem rzeczywistym przy użyciu QElapsedTimer i QTimer.
 *
 * Przykładowe użycie:
 * @code
 * CSVManager* manager = new CSVManager(sensorData, this);
 * connect(manager, &CSVManager::dataUpdated, this, &MainWindow::onDataUpdated);
 * if (manager->loadFromFile("dane.csv")) {
 *     manager->startPlayback();
 * }
 * @endcode
 *
 * Plik CSV powinien mieć następujący format (jeden wiersz = jeden pomiar):
 * time, tof1, tof2, tof3, tof4, lineS1, lineS2, lineS3, motor1, motor2, imuX, imuY
 *
 * @author Jakub Wilczyński
 */
class CSVManager : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Konstruktor klasy CSVManager.
     * @param sensorData Wskaźnik do obiektu SensorData, który będzie aktualizowany na podstawie CSV.
     * @param parent Opcjonalny rodzic QObject.
     */
    explicit CSVManager(SensorData* sensorData, QObject *parent = nullptr);

    /**
     * @brief Wirtualny destruktor. Zatrzymuje timery i sprząta zasoby.
     */
    ~CSVManager();

    /**
     * @brief Wczytuje dane z pliku CSV i przygotowuje je do odtwarzania.
     * @param filePath Ścieżka do pliku CSV.
     * @return true jeśli wczytywanie się powiodło, false w przypadku błędu (np. uszkodzony plik).
     *
     * Wczytuje wszystkie wiersze (z pominięciem nagłówka) i waliduje liczbę kolumn (minimum 12).
     */
    bool loadFromFile(const QString &filePath);

    /**
     * @brief Rozpoczyna odtwarzanie/symulację danych z CSV.
     *
     * Inicjuje timer odtwarzania i rozpoczyna cykliczne aktualizacje poprzez QTimer.
     */
    void startPlayback();

    /**
     * @brief Zatrzymuje odtwarzanie danych CSV.
     *
     * Zatrzymuje timery i emituje sygnał @ref playbackFinished.
     */
    void stopPlayback();

signals:
    /**
     * @brief Sygnał informujący o aktualizacji danych.
     * @param time Znacznik czasu (w ms) aktualizacji.
     */
    void dataUpdated(int time);

    /**
     * @brief Sygnał emitowany po zakończeniu odtwarzania (np. koniec pliku).
     */
    void playbackFinished();

private slots:
    /**
     * @brief Slot cyklicznie aktualizujący dane na podstawie upływu czasu.
     *
     * Automatycznie wywoływany przez QTimer.
     */
    void updateData();

private:
    SensorData* m_sensorData;             ///< Wskaźnik do obiektu SensorData, do którego trafiają dane z CSV.
    QVector<QStringList> m_csvData;       ///< Bufor danych wczytanych z pliku CSV (każdy wiersz = QStringList).
    QElapsedTimer m_playbackTimer;        ///< Timer mierzący czas od rozpoczęcia odtwarzania (symulacji).
    QTimer* m_dataUpdateTimer = nullptr;  ///< Timer cyklicznej aktualizacji (driven by Qt event loop).
    int m_currentRow = 0;                 ///< Indeks bieżącego wiersza danych CSV.

    /**
     * @brief Aktualizuje SensorData na podstawie danych z wybranego wiersza CSV.
     * @param row Numer (indeks) wiersza do przetworzenia.
     *
     * Sprawdza poprawność indeksu i liczby kolumn. W razie błędu loguje ostrzeżenie.
     * Po aktualizacji wywołuje sygnał dataUpdated(time).
     */
    void updateVariablesFromCSV(int row);
};

#endif // CSVMANAGER_H
