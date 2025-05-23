#ifndef LINECHARTMENAGER_H
#define LINECHARTMENAGER_H

#include <QObject>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QTimer>
#include "sensordata.h"
#include "chartviewhelper.h"

class QGraphicsTextItem;

/**
 * @class LineChartMenager
 * @brief Zarządza wykresem danych z czujników linii.
 */
class LineChartMenager : public QObject
{
    Q_OBJECT

public:
    explicit LineChartMenager(SensorData* sensorData, QWidget* container, QObject *parent = nullptr);
    ~LineChartMenager();

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

    /**
     * @brief Przywraca widok do aktualnych danych (włącza auto-scrolling).
     */
    void resetToCurrentData();

protected:
    /**
     * @brief Filtr zdarzeń do obsługi zmiany rozmiaru kontenera.
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

    /**
     * @brief Obsługa kliknięcia i przesunięcia myszy na wykresie.
     */
    void handleChartMousePressed();
    void handleChartMouseReleased();

    /**
     * @brief Wymusza przestrzeganie limitów osi X (min ≥ 0, max ≤ aktualny_czas).
     */
    void enforceAxisLimits(qreal min, qreal max);

    /**
     * @brief Dostosowuje etykiety osi X do bieżącego rozmiaru wykresu.
     */
    void adjustAxisLabels();

private:
    SensorData* m_sensorData;      ///< Wskaźnik do danych sensorów.
    QWidget* m_container;          ///< Kontener QWidget, w którym umieszczany jest wykres.
    QChart* m_chart;               ///< Obiekt wykresu (QChart) zarządzający serią i osiami.
    QLineSeries* m_series1;        ///< Pierwsza seria danych wykresu.
    QLineSeries* m_series2;        ///< Druga seria danych wykresu.
    QLineSeries* m_series3;        ///< Trzecia seria danych wykresu.
    QValueAxis* m_axisX;           ///< Oś X wykresu (zwykle czas).
    QValueAxis* m_axisY;           ///< Oś Y wykresu (np. wartość sensora).
    HorizontalOnlyChartView* m_chartView; ///< Widok wykresu do umieszczenia w GUI.

    // Dodajemy własne etykiety tytułów osi
    QGraphicsTextItem* m_xAxisTitle;  ///< Tytuł osi X jako osobny element graficzny
    QGraphicsTextItem* m_yAxisTitle;  ///< Tytuł osi Y jako osobny element graficzny

    static const int MAX_POINTS = 100;  ///< Maksymalna liczba punktów widocznych na wykresie.
    int m_currentPoint = 0;             ///< Aktualny numer punktu (licznik).
    qint64 m_currentTimeOffset;         ///< Bieżące przesunięcie czasu.
    bool m_isAutoScrollEnabled;         ///< Czy auto-przewijanie jest włączone.
    qreal m_latestDataTime = 0;         ///< Czas najnowszych danych.
};

#endif // LINECHARTMENAGER_H
