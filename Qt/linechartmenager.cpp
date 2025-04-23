#include "linechartmenager.h".h"
#include <QVBoxLayout>
#include <QFont>

LineChartMenager::LineChartMenager(SensorData* sensorData, QWidget* container, QObject *parent)
    : QObject(parent), m_sensorData(sensorData), m_container(container),
    m_chart(nullptr), m_chartView(nullptr), m_currentPoint(0)
{
    // Sprawdzamy czy przekazane wskaźniki są poprawne
    if (!m_sensorData || !m_container) {
        qWarning() << "TofChartManager: Nieprawidłowe wskaźniki do sensorData lub container!";
        return;
    }

    setupChart();

    // Połączenie sygnału zmiany danych z metodą aktualizującą wykres
    connect(m_sensorData, &SensorData::tofDataChanged, this, [this]() {
        // Możemy tutaj dodać aktualizację wykresu, jeśli potrzebujemy
        // Zwykle będziemy korzystać z updateChart(time) wywoływanej z zewnątrz
    });
}

LineChartMenager::~LineChartMenager()
{
    // Nie usuwamy m_container i m_sensorData, bo zostały przekazane z zewnątrz
    delete m_chart;  // m_chartView jest własnością m_container i zostanie usunięty wraz z nim
}

void LineChartMenager::setupChart()
{
    // Tworzenie wykresu
    m_chart = new QChart();
    m_chart->setMargins(QMargins(0, 0, 0, 0));
    m_chart->setBackgroundRoundness(0);  // bez zaokrągleń tła

    // Tworzenie serii danych
    m_series1 = new QLineSeries();
    m_series2 = new QLineSeries();
    m_series3 = new QLineSeries();

    m_series1->setName("LS 1");
    m_series2->setName("LS 2");
    m_series3->setName("LS 3");

    // Dodawanie serii do wykresu
    m_chart->addSeries(m_series1);
    m_chart->addSeries(m_series2);
    m_chart->addSeries(m_series3);

    // Tworzenie osi
    m_axisX = new QValueAxis();
    m_axisY = new QValueAxis();

    m_axisX->setTitleText("Czas (ms)");
    m_axisY->setTitleText("Stan");

    m_axisY->setRange(0, 1.1);  // Dla wartości binarnych (0/1)
    m_axisX->setRange(0, MAX_POINTS * 100);  // Zakładamy dane co 100ms

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

    // Opcje wizualne
    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignTop | Qt::AlignRight);
    m_chart->legend()->setBackgroundVisible(false);
    m_chart->legend()->setBorderColor(Qt::transparent);

    // Dostosowanie położenia legendy
    QRectF legendRect = m_chart->legend()->geometry();
    legendRect.moveTopRight(m_chart->plotArea().topRight() - QPointF(10, 10));
    m_chart->legend()->setGeometry(legendRect);

    // Zmniejsz czcionkę legendy
    QFont legendFont = m_chart->legend()->font();
    legendFont.setPointSize(8); // Zmniejsz rozmiar czcionki
    m_chart->legend()->setFont(legendFont);

    // Wartości osi
    QFont axisFont;
    axisFont.setPointSize(8);

    m_axisX->setLabelsFont(axisFont);
    m_axisY->setLabelsFont(axisFont);
    m_axisX->setTitleVisible(false);
    m_axisY->setTitleVisible(false);

    // Tworzenie widoku wykresu
    m_chartView = new QChartView(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    // Zamiast tworzyć nowy layout, sprawdź czy container już ma layout
    QLayout* existingLayout = m_container->layout();

    if (existingLayout) {
        // Jeśli już istnieje layout, wyczyść go (usuń obecne widgety)
        while (QLayoutItem* item = existingLayout->takeAt(0)) {
            if (QWidget* widget = item->widget()) {
                widget->setParent(nullptr);  // Odłącz widget od layoutu
                delete widget;
            }
            delete item;
        }

        // Dodaj widok wykresu do istniejącego layoutu
        existingLayout->addWidget(m_chartView);
    } else {
        // Jeśli nie ma layoutu, utwórz nowy
        QVBoxLayout *layout = new QVBoxLayout(m_container);
        layout->addWidget(m_chartView);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        m_container->setLayout(layout);
    }
}

void LineChartMenager::updateChart(int time)
{
    // Dodanie nowych punktów do wykresów na podstawie bieżących danych z SensorData
    m_series1->append(time, m_sensorData->getLineS1Active() ? 1 : 0);
    m_series2->append(time, m_sensorData->getLineS2Active() ? 1 : 0);
    m_series3->append(time, m_sensorData->getLineS3Active() ? 1 : 0);

    m_currentPoint++;

    // Przesuwanie okna wykresu, gdy zbyt dużo punktów
    if (m_currentPoint >= MAX_POINTS) {
        m_axisX->setRange(time - MAX_POINTS * 100, time);
    }
}

void LineChartMenager::clearChart()
{
    m_series1->clear();
    m_series2->clear();
    m_series3->clear();

    m_currentPoint = 0;

    // Resetowanie zakresu osi X
    m_axisX->setRange(0, MAX_POINTS * 100);
}
