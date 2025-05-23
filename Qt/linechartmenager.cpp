#include "linechartmenager.h"
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QDateTime>
#include <QFont>
#include <QTimer>

// Definicja stałej MAX_POINTS, jeśli nie jest zdefiniowana w nagłówku
const int LineChartMenager::MAX_POINTS;

LineChartMenager::LineChartMenager(SensorData* sensorData, QWidget* container, QObject *parent)
    : QObject(parent), m_sensorData(sensorData), m_container(container),
    m_chart(nullptr), m_chartView(nullptr), m_currentPoint(0),
    m_isAutoScrollEnabled(true), m_currentTimeOffset(0)
{
    // Sprawdzamy czy przekazane wskaźniki są poprawne
    if (!m_sensorData || !m_container) {
        qWarning() << "LineChartMenager: Nieprawidłowe wskaźniki do sensorData lub container!";
        return;
    }

    // Instaluj filtr zdarzeń, aby reagować na zmianę rozmiaru
    m_container->installEventFilter(this);

    setupChart();

    // Podobnie jak w MotorChartManager - nie aktualizujemy wykresu bezpośrednio w slotach
    connect(m_sensorData, &SensorData::lineSDataChanged, this, [this]() {
        // Slot jest pusty - aktualizacja będzie wywoływana z zewnątrz
    });

    // Usuwamy błędną linię łączącą z sygnałem resized, którego nie ma w QWidget
}

LineChartMenager::~LineChartMenager()
{
    // Usuń filtr zdarzeń
    if (m_container) {
        m_container->removeEventFilter(this);
    }

    // Nie usuwamy m_container i m_sensorData, bo zostały przekazane z zewnątrz
    delete m_chart;  // m_chartView jest własnością m_container i zostanie usunięty wraz z nim
}

bool LineChartMenager::eventFilter(QObject *watched, QEvent *event)
{
    // Jeśli to zdarzenie zmiany rozmiaru dla kontenera
    if (watched == m_container && event->type() == QEvent::Resize) {
        if (m_chartView) {
            // Dostosuj rozmiar widoku wykresu do kontenera
            m_chartView->resize(m_container->size());

            // Dostosuj etykiety osi po zmianie rozmiaru - wywołujemy bezpośrednio tutaj
            adjustAxisLabels();
        }
        return false; // Pozwól na standardową obsługę zdarzenia
    }
    return QObject::eventFilter(watched, event);
}


void LineChartMenager::setupChart()
{
    // Tworzenie wykresu
    m_chart = new QChart();
    m_chart->setMargins(QMargins(0, 0, 0, 0));  // Przywracamy marginesy 0 jak w oryginalnym kodzie
    m_chart->setBackgroundRoundness(0);
    m_chart->setBackgroundVisible(false);
    m_chart->setAnimationOptions(QChart::NoAnimation);

    // Tworzenie serii danych
    m_series1 = new QLineSeries();
    m_series2 = new QLineSeries();
    m_series3 = new QLineSeries();

    m_series1->setName("LS 1");
    m_series2->setName("LS 2");
    m_series3->setName("LS 3");

    // Ustawienie różnych kolorów dla serii
    m_series1->setColor(QColor(255, 0, 0));     // Czerwony
    m_series2->setColor(QColor(0, 0, 255));     // Niebieski
    m_series3->setColor(QColor(0, 128, 0));     // Zielony

    // Ustawienie grubszych linii
    QPen pen1 = m_series1->pen();
    QPen pen2 = m_series2->pen();
    QPen pen3 = m_series3->pen();
    pen1.setWidth(2);
    pen2.setWidth(2);
    pen3.setWidth(2);
    m_series1->setPen(pen1);
    m_series2->setPen(pen2);
    m_series3->setPen(pen3);

    // Dodawanie serii do wykresu
    m_chart->addSeries(m_series1);
    m_chart->addSeries(m_series2);
    m_chart->addSeries(m_series3);

    // Tworzenie osi
    m_axisX = new QValueAxis();
    m_axisY = new QValueAxis();

    // Ukrywamy podpisy osi (zgodnie z wymaganiem)
    m_axisX->setTitleVisible(false);
    m_axisY->setTitleVisible(false);

    // Ustawiamy teksty osi (mimo że są ukryte)
    m_axisX->setTitleText("Czas (ms)");
    m_axisY->setTitleText("Stan");

    // Czcionka dla osi - ustawiamy na 8 punktów jak w MotorChartManager
    QFont axisFont;
    axisFont.setPointSize(8);  // Stały rozmiar, identyczny jak w MotorChartManager

    m_axisX->setLabelsFont(axisFont);
    m_axisY->setLabelsFont(axisFont);

    // Ustawienie formatu etykiet osi X - pełne wartości
    m_axisX->setLabelFormat("%.0f");

    // Dynamiczna liczba etykiet na osi X
    m_axisX->setTickCount(5);

    // Ustawienie osi Y dla wartości 0/1
    m_axisY->setRange(0, 1.0);
    m_axisY->setTickCount(2);
    m_axisY->setLabelFormat("%d");

    // Ustawienie początkowego zakresu osi X
    m_axisX->setRange(0, MAX_POINTS * 100);

    // Dodawanie osi do wykresu
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);

    // Dołączanie osi do serii
    m_series1->attachAxis(m_axisX);
    m_series1->attachAxis(m_axisY);
    m_series2->attachAxis(m_axisX);
    m_series2->attachAxis(m_axisY);
    m_series3->attachAxis(m_axisX);
    m_series3->attachAxis(m_axisY);

    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignRight);  // Ustawia legendę po prawej stronie
    m_chart->legend()->setBackgroundVisible(false);
    m_chart->legend()->setBorderColor(Qt::transparent);

    // Dostosowanie położenia legendy
    QRectF legendRect = m_chart->legend()->geometry();
    legendRect.moveTopRight(m_chart->plotArea().topRight() - QPointF(10, 10));
    m_chart->legend()->setGeometry(legendRect);

    // Ustawiamy czcionkę dla legendy
    QFont legendFont;
    legendFont.setPointSize(8);  // Stały rozmiar czcionki dla legendy
    m_chart->legend()->setFont(legendFont);

    // Tworzenie niestandardowego widoku wykresu
    m_chartView = new HorizontalOnlyChartView(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    // Wyłączamy paski przewijania, ale włączamy możliwość przeciągania
    m_chartView->setRubberBand(QChartView::HorizontalRubberBand);
    m_chartView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_chartView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Podłączamy callbacki dla obsługi myszy
    m_chartView->setMousePressedCallback([this]() {
        handleChartMousePressed();
    });
    m_chartView->setMouseReleasedCallback([this]() {
        handleChartMouseReleased();
    });

    // Nasłuchujemy zmiany zakresu osi X by ograniczyć przesuwanie
    connect(m_axisX, &QValueAxis::rangeChanged, this, &LineChartMenager::enforceAxisLimits);

    // Ustawienie layoutu - identycznie jak w motorchartmanager
    QLayout* existingLayout = m_container->layout();

    if (existingLayout) {
        // Czyszczenie istniejącego layoutu
        while (QLayoutItem* item = existingLayout->takeAt(0)) {
            if (QWidget* widget = item->widget()) {
                widget->setParent(nullptr);
                delete widget;
            }
            delete item;
        }
        existingLayout->addWidget(m_chartView);
    } else {
        QVBoxLayout *layout = new QVBoxLayout(m_container);
        layout->addWidget(m_chartView);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        m_container->setLayout(layout);
    }

    // Początkowe dostosowanie etykiet osi X do rozmiaru
    QTimer::singleShot(0, this, &LineChartMenager::adjustAxisLabels);
}

void LineChartMenager::updateChart(int time)
{
    // Dodanie nowych punktów do wykresów na podstawie bieżących danych z SensorData
    m_series1->append(time, m_sensorData->getLineS1Active() ? 1 : 0);
    m_series2->append(time, m_sensorData->getLineS2Active() ? 1 : 0);
    m_series3->append(time, m_sensorData->getLineS3Active() ? 1 : 0);

    m_currentPoint++;
    m_latestDataTime = time;

    // Automatyczne przesuwanie wykresu gdy nowe dane wykraczają poza widoczny obszar
    qreal currentMax = m_axisX->max();

    if (time > currentMax || m_isAutoScrollEnabled) {
        // Przesuwamy zakres osi X, aby pokazać najnowsze dane
        m_axisX->setRange(time - MAX_POINTS * 100, time);
    }

    // Usuwamy stare punkty, tylko gdy jest ich za dużo (dla wydajności)
    const int MAX_HISTORY = MAX_POINTS * 10; // Przechowujemy więcej danych do przewijania

    if (m_series1->count() > MAX_HISTORY) {
        int pointsToRemove = m_series1->count() - MAX_HISTORY;
        m_series1->removePoints(0, pointsToRemove);
        m_series2->removePoints(0, pointsToRemove);
        m_series3->removePoints(0, pointsToRemove);
    }
}

void LineChartMenager::clearChart()
{
    m_series1->clear();
    m_series2->clear();
    m_series3->clear();

    m_currentPoint = 0;
    m_currentTimeOffset = 0;
    m_latestDataTime = 0;
    m_isAutoScrollEnabled = true;

    // Resetowanie zakresu osi X
    m_axisX->setRange(0, MAX_POINTS * 100);
}

void LineChartMenager::handleChartMousePressed()
{
    // Gdy użytkownik kliknie i zacznie przesuwać, wyłączamy auto-scrolling
    m_isAutoScrollEnabled = false;
}

void LineChartMenager::handleChartMouseReleased()
{
    // Wymuszamy przestrzeganie limitów osi
    enforceAxisLimits(m_axisX->min(), m_axisX->max());
}

void LineChartMenager::resetToCurrentData()
{
    // Przywracamy auto-scrolling i pokazujemy najnowsze dane
    m_isAutoScrollEnabled = true;

    if (m_latestDataTime > 0) {
        m_axisX->setRange(m_latestDataTime - MAX_POINTS * 100, m_latestDataTime);
    } else {
        m_axisX->setRange(0, MAX_POINTS * 100);
    }
}

void LineChartMenager::enforceAxisLimits(qreal min, qreal max)
{
    // Blokujemy przesuwanie poza zakres danych
    bool needAdjustment = false;
    qreal newMin = min;
    qreal newMax = max;
    qreal rangeSize = max - min;

    // Nie pozwalamy przesunąć w lewo poniżej zera
    if (min < 0) {
        newMin = 0;
        newMax = rangeSize;  // Zachowujemy szerokość zakresu
        needAdjustment = true;
    }

    // Nie pozwalamy przesunąć w prawo poza aktualny czas danych
    if (max > m_latestDataTime && m_latestDataTime > 0) {
        newMax = m_latestDataTime;
        newMin = qMax(0.0, m_latestDataTime - rangeSize);  // Zachowujemy szerokość zakresu, ale nie poniżej zera
        needAdjustment = true;
    }

    // Jeśli potrzebna korekta, to blokujemy sygnał zmiany zakresu i ustawiamy nowy zakres
    if (needAdjustment) {
        // Blokujemy sygnały, aby zapobiec pętli sygnałów
        QSignalBlocker blocker(m_axisX);
        m_axisX->setRange(newMin, newMax);
    }
}

void LineChartMenager::adjustAxisLabels()
{
    if (m_chartView && m_axisX) {
        // Obliczamy szerokość dostępną dla wykresu
        int chartWidth = m_chartView->width();

        if (chartWidth <= 0)
            return;  // Wykres jeszcze nie ma poprawnych wymiarów

        // Dostosowujemy liczbę etykiet w zależności od szerokości
        int labelCount;
        if (chartWidth < 100) {
            // Bardzo mały wykres - tylko 2 etykiety (początek i koniec)
            labelCount = 2;
        } else if (chartWidth < 200) {
            // Mały wykres - 3 etykiety
            labelCount = 3;
        } else if (chartWidth < 300) {
            // Średni wykres - 4 etykiety
            labelCount = 4;
        } else {
            // Duży wykres - 5 etykiet
            labelCount = 5;
        }

        // Ustawiamy nową liczbę etykiet
        if (m_axisX->tickCount() != labelCount) {
            m_axisX->setTickCount(labelCount);
        }

        // Upewniamy się, że tytuł osi X jest zawsze widoczny
        m_axisX->setTitleVisible(false);

        // Dodatkowo modyfikujemy sposób wyświetlania etykiet
        if (chartWidth < 150) {
            // Dla bardzo małych wykresów skracamy format etykiet
            m_axisX->setLabelFormat("%.0f");
        } else {
            // Dla większych wykresów pokazujemy pełny format
            m_axisX->setLabelFormat("%.0f");
        }
    }
}
