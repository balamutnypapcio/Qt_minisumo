#ifndef MOTORCHARTMANAGER_H
#define MOTORCHARTMANAGER_H

#include <QObject>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QTimer>
#include "sensordata.h"
#include "chartviewhelper.h"

class QGraphicsTextItem;

/**
 * @class MotorChartManager
 * @brief Zarządza dynamicznym wykresem prezentującym dane z silników (np. prędkości lub sterowania).
 *
 * Odpowiada za:
 *  - Tworzenie i konfigurację wykresu (QChart) dla dwóch sygnałów z silników (np. lewy i prawy).
 *  - Dodawanie nowych próbek, automatyczne przewijanie oraz czyszczenie danych.
 *  - Dynamiczne dopasowywanie osi do okna i blokowanie przewijania poza zakres danych.
 *  - Obsługę auto-scrollingu oraz wyłączanie go przy ręcznej interakcji użytkownika.
 *
 * Współpracuje z HorizontalOnlyChartView (ograniczenie interakcji do osi X).
 *
 * Przykład użycia:
 * @code
 * MotorChartManager* motorMgr = new MotorChartManager(sensorData, ui->motorChartContainer, this);
 * connect(csvManager, &CSVManager::dataUpdated, motorMgr, &MotorChartManager::updateChart);
 * @endcode
 *
 * @author Jakub Wilczyński
 */
class MotorChartManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor.
     * @param sensorData Wskaźnik do SensorData (źródło danych z silników).
     * @param container QWidget, w którym osadzany jest wykres.
     * @param parent Opcjonalny rodzic QObject.
     */
    explicit MotorChartManager(SensorData* sensorData, QWidget* container, QObject *parent = nullptr);

    /**
     * @brief Destruktor. Usuwa wykres i zwalnia zasoby.
     */
    ~MotorChartManager();

    /**
     * @brief Inicjalizuje (tworzy) wykres i ustawia wszystkie parametry.
     */
    void setupChart();

    /**
     * @brief Aktualizuje wykres o nowe dane z SensorData.
     * @param time Znacznik czasu nowej próbki (ms).
     */
    void updateChart(int time);

    /**
     * @brief Czyści wszystkie próbki z wykresu oraz resetuje oś X.
     */
    void clearChart();

    /**
     * @brief Przywraca widok wykresu do najnowszych danych (włącza auto-scroll).
     */
    void resetToCurrentData();

protected:
    /**
     * @brief Filtr zdarzeń do obsługi resize kontenera wykresu.
     * @param watched Obiekt obserwowany.
     * @param event Wskaźnik do zdarzenia.
     * @return true jeśli filtr przechwycił zdarzenie.
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

    /**
     * @brief Wywoływane przy kliknięciu/przesuwaniu wykresu – wyłącza auto-scroll.
     */
    void handleChartMousePressed();

    /**
     * @brief Wywoływane przy puszczeniu myszy na wykresie – blokuje zakres przewijania.
     */
    void handleChartMouseReleased();

    /**
     * @brief Wymusza przestrzeganie limitów osi X (brak przewijania poza [0, aktualny_czas]).
     * @param min Aktualne minimum zakresu osi X.
     * @param max Aktualne maksimum zakresu osi X.
     */
    void enforceAxisLimits(qreal min, qreal max);

    /**
     * @brief Dostosowuje liczbę etykiet osi X do rozmiaru wykresu.
     */
    void adjustAxisLabels();

private:
    SensorData* m_sensorData;      ///< Źródło danych z silników.
    QWidget* m_container;          ///< Kontener, w którym umieszczany jest wykres.
    QChart* m_chart;               ///< Obiekt wykresu (QChart).
    QLineSeries* m_series1;        ///< Seria danych Motor 1.
    QLineSeries* m_series2;        ///< Seria danych Motor 2.
    QValueAxis* m_axisX;           ///< Oś X (czas).
    QValueAxis* m_axisY;           ///< Oś Y (prędkość silnika lub sterowanie).
    HorizontalOnlyChartView* m_chartView; ///< Widok wykresu, ograniczony do przewijania poziomego.

    QGraphicsTextItem* m_xAxisTitle;  ///< Tytuł osi X (opcjonalnie graficzny, obecnie ukryty).
    QGraphicsTextItem* m_yAxisTitle;  ///< Tytuł osi Y (opcjonalnie graficzny, obecnie ukryty).

    static const int MAX_POINTS = 100;  ///< Liczba próbek widocznych na wykresie (szerokość okna).
    int m_currentPoint = 0;             ///< Licznik wstawionych punktów.
    qint64 m_currentTimeOffset;         ///< Przesunięcie czasu (nieużywane – do rozbudowy).
    bool m_isAutoScrollEnabled;         ///< Czy auto-scroll jest włączony.
    qreal m_latestDataTime = 0;         ///< Znacznik czasu najnowszych danych (ms).
};

#endif // MOTORCHARTMANAGER_H
