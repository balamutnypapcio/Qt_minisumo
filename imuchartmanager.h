#ifndef IMUCHARTMANAGER_H
#define IMUCHARTMANAGER_H

#include <QObject>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include "sensordata.h"


/**
 * @class ImuChartManager
 * @brief Zarządza wykresem danych z IMU.
 */
class ImuChartManager : public QObject {
    Q_OBJECT
public:
    explicit ImuChartManager(SensorData* sensorData, QWidget* container, QObject *parent = nullptr);
    ~ImuChartManager();

    /**
     * @brief Inicjalizuje wykres.
     */
    void setupChart();
    /**
     * @brief Aktualizuje dane na wykresie.
     * @param time Znacznik czasu.
     */
    void updateChart(int time);

    /**
     * @brief Czyści dane z wykresu.
     */
    void clearChart();

private:
    SensorData* m_sensorData;      ///< Wskaźnik do danych sensorów.
    QWidget* m_container;          ///< Kontener QWidget, w którym umieszczany jest wykres.
    QChart* m_chart;               ///< Obiekt wykresu (QChart) zarządzający serią i osiami.
    QLineSeries* m_series1;        ///< Pierwsza seria danych wykresu.
    QLineSeries* m_series2;        ///< Druga seria danych wykresu.
    QValueAxis* m_axisX;           ///< Oś X wykresu (zwykle czas).
    QValueAxis* m_axisY;           ///< Oś Y wykresu (np. wartość sensora).
    QChartView* m_chartView;       ///< Widok wykresu do umieszczenia w GUI.

    static const int MAX_POINTS = 100;
    int m_currentPoint = 0;
};

#endif // IMUCHARTMANAGER_H
