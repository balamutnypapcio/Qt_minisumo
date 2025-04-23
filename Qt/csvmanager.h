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
 * @brief Zarządza odczytem i odtwarzaniem danych z pliku CSV.
 */
class CSVManager : public QObject {
    Q_OBJECT

public:

    /**
     * @brief Konstruktor klasy CSVManager.
     * @param sensorData Wskaźnik do obiektu SensorData.
     * @param parent Opcjonalny rodzic QObject.
     */
    explicit CSVManager(SensorData* sensorData, QObject *parent = nullptr);

    /**
     * @brief Destruktor.
     */
    ~CSVManager();

    /**
     * @brief Wczytuje dane z pliku CSV.
     * @param filePath Ścieżka do pliku CSV.
     * @return true jeśli wczytywanie się powiodło.
     */
    bool loadFromFile(const QString &filePath);

    /**
     * @brief Rozpoczyna odtwarzanie danych z CSV.
     */
    void startPlayback();

    /**
     * @brief Zatrzymuje odtwarzanie danych.
     */
    void stopPlayback();

signals:

    /**
     * @brief Sygnał informujący o aktualizacji danych.
     * @param time Znacznik czasu aktualizacji.
     */
    void dataUpdated(int time);

    /**
     * @brief Sygnał emitowany po zakończeniu odtwarzania.
     */
    void playbackFinished();

private slots:

    /**
     * @brief Slot do aktualizacji danych w czasie rzeczywistym.
     */
    void updateData();

private:
    SensorData* m_sensorData;             ///< Wskaźnik do danych sensorów.
    QVector<QStringList> m_csvData;       ///< Dane wczytane z pliku CSV.
    QElapsedTimer m_playbackTimer;        ///< Timer do pomiaru czasu odtwarzania.
    QTimer* m_dataUpdateTimer = nullptr;  ///< Timer do aktualizacji danych.
    int m_currentRow = 0;                 ///< Bieżący wiersz CSV.

    /**
     * @brief Aktualizuje dane na podstawie wiersza CSV.
     * @param row Numer wiersza.
     */
    void updateVariablesFromCSV(int row);
};
#endif // CSVMANAGER_H
