#ifndef MOTORCHARTMANAGER_H
#define MOTORCHARTMANAGER_H

#include <QObject>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include "sensordata.h"

class MotorChartManager : public QObject {
    Q_OBJECT
public:
    explicit MotorChartManager(SensorData* sensorData, QWidget* container, QObject *parent = nullptr);
    ~MotorChartManager();

    void setupChart();
    void updateChart(int time);
    void clearChart();

private:
    SensorData* m_sensorData;
    QWidget* m_container;
    QChart* m_chart;
    QLineSeries* m_series1;
    QLineSeries* m_series2;
    QValueAxis* m_axisX;
    QValueAxis* m_axisY;
    QChartView* m_chartView;

    static const int MAX_POINTS = 100;
    int m_currentPoint = 0;
};

#endif // MOTORCHARTMANAGER_H
