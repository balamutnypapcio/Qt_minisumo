#include "tofchartmanager.h"
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QDateTime>
#include <QFont>
#include <QTimer>

/* Stała MAX_POINTS – deklaracja */
const int TofChartManager::MAX_POINTS;

TofChartManager::TofChartManager(SensorData* sensorData, QWidget* container, QObject *parent)
    : QObject(parent), m_sensorData(sensorData), m_container(container),
    m_chart(nullptr), m_chartView(nullptr), m_currentPoint(0),
    m_isAutoScrollEnabled(true), m_currentTimeOffset(0)
{
    // Sprawdź poprawność wskaźników
    if (!m_sensorData || !m_container) {
        qWarning() << "TofChartManager: Nieprawidłowe wskaźniki do sensorData lub container!";
        return;
    }

    // Instaluj filtr zdarzeń dla kontenera
    m_container->installEventFilter(this);

    setupChart();

    // Slot SensorData::tofDataChanged zostaje pusty – aktualizacja wywoływana z zewnątrz
    connect(m_sensorData, &SensorData::tofDataChanged, this, [this]() {
        // Slot pusty – aktualizacja wykresu przez updateChart(time)
    });
}

TofChartManager::~TofChartManager()
{
    // Usuń filtr zdarzeń
    if (m_container) {
        m_container->removeEventFilter(this);
    }

    delete m_chart;  // m_chartView usuwany przez m_container
}

bool TofChartManager::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_container && event->type() == QEvent::Resize) {
        if (m_chartView) {
            m_chartView->resize(m_container->size());
            adjustAxisLabels();
        }
        return false; // Pozwól na domyślną obsługę zdarzenia
    }
    return QObject::eventFilter(watched, event);
}

void TofChartManager::setupChart()
{
    m_chart = new QChart();
    m_chart->setMargins(QMargins(0, 0, 0, 0));
    m_chart->setBackgroundRoundness(0);
    m_chart->setBackgroundVisible(false);
    m_chart->setAnimationOptions(QChart::NoAnimation);

    m_series1 = new QLineSeries();
    m_series2 = new QLineSeries();
    m_series3 = new QLineSeries();
    m_series4 = new QLineSeries();

    m_series1->setName("ToF L");
    m_series2->setName("ToF FL");
    m_series3->setName("ToF FR");
    m_series4->setName("ToF R");

    m_series1->setColor(QColor(255, 0, 0));
    m_series2->setColor(QColor(0, 0, 255));
    m_series3->setColor(QColor(0, 128, 0));
    m_series4->setColor(QColor(128, 0, 128));

    QPen pen1 = m_series1->pen();
    QPen pen2 = m_series2->pen();
    QPen pen3 = m_series3->pen();
    QPen pen4 = m_series4->pen();
    pen1.setWidth(2);
    pen2.setWidth(2);
    pen3.setWidth(2);
    pen4.setWidth(2);
    m_series1->setPen(pen1);
    m_series2->setPen(pen2);
    m_series3->setPen(pen3);
    m_series4->setPen(pen4);

    m_chart->addSeries(m_series1);
    m_chart->addSeries(m_series2);
    m_chart->addSeries(m_series3);
    m_chart->addSeries(m_series4);

    m_axisX = new QValueAxis();
    m_axisY = new QValueAxis();

    m_axisX->setTitleVisible(false);
    m_axisY->setTitleVisible(false);

    m_axisX->setTitleText("Czas (ms)");
    m_axisY->setTitleText("Stan");

    QFont axisFont;
    axisFont.setPointSize(8);

    m_axisX->setLabelsFont(axisFont);
    m_axisY->setLabelsFont(axisFont);

    m_axisX->setLabelFormat("%.0f");
    m_axisX->setTickCount(5);

    m_axisY->setRange(0, 1.0);
    m_axisY->setTickCount(2);
    m_axisY->setLabelFormat("%d");

    m_axisX->setRange(0, MAX_POINTS * 100);

    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);

    m_series1->attachAxis(m_axisX);
    m_series1->attachAxis(m_axisY);
    m_series2->attachAxis(m_axisX);
    m_series2->attachAxis(m_axisY);
    m_series3->attachAxis(m_axisX);
    m_series3->attachAxis(m_axisY);
    m_series4->attachAxis(m_axisX);
    m_series4->attachAxis(m_axisY);

    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignRight);
    m_chart->legend()->setBackgroundVisible(false);
    m_chart->legend()->setBorderColor(Qt::transparent);

    QRectF legendRect = m_chart->legend()->geometry();
    legendRect.moveTopRight(m_chart->plotArea().topRight() - QPointF(10, 10));
    m_chart->legend()->setGeometry(legendRect);

    QFont legendFont;
    legendFont.setPointSize(7);
    m_chart->legend()->setFont(legendFont);

    m_chartView = new HorizontalOnlyChartView(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_chartView->setRubberBand(QChartView::HorizontalRubberBand);
    m_chartView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_chartView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_chartView->setMousePressedCallback([this]() {
        handleChartMousePressed();
    });
    m_chartView->setMouseReleasedCallback([this]() {
        handleChartMouseReleased();
    });

    connect(m_axisX, &QValueAxis::rangeChanged, this, &TofChartManager::enforceAxisLimits);

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

    QTimer::singleShot(0, this, &TofChartManager::adjustAxisLabels);
}

void TofChartManager::updateChart(int time)
{
    m_series1->append(time, m_sensorData->getTof1() ? 1 : 0);
    m_series2->append(time, m_sensorData->getTof2() ? 1 : 0);
    m_series3->append(time, m_sensorData->getTof3() ? 1 : 0);
    m_series4->append(time, m_sensorData->getTof4() ? 1 : 0);

    m_currentPoint++;
    m_latestDataTime = time;

    qreal currentMax = m_axisX->max();

    if (time > currentMax || m_isAutoScrollEnabled) {
        m_axisX->setRange(time - MAX_POINTS * 100, time);
    }

    const int MAX_HISTORY = MAX_POINTS * 10;
    if (m_series1->count() > MAX_HISTORY) {
        int pointsToRemove = m_series1->count() - MAX_HISTORY;
        m_series1->removePoints(0, pointsToRemove);
        m_series2->removePoints(0, pointsToRemove);
        m_series3->removePoints(0, pointsToRemove);
        m_series4->removePoints(0, pointsToRemove);
    }
}

void TofChartManager::clearChart()
{
    m_series1->clear();
    m_series2->clear();
    m_series3->clear();
    m_series4->clear();

    m_currentPoint = 0;
    m_currentTimeOffset = 0;
    m_latestDataTime = 0;
    m_isAutoScrollEnabled = true;

    m_axisX->setRange(0, MAX_POINTS * 100);
}

void TofChartManager::handleChartMousePressed()
{
    m_isAutoScrollEnabled = false;
}

void TofChartManager::handleChartMouseReleased()
{
    enforceAxisLimits(m_axisX->min(), m_axisX->max());
}

void TofChartManager::resetToCurrentData()
{
    m_isAutoScrollEnabled = true;

    if (m_latestDataTime > 0) {
        m_axisX->setRange(m_latestDataTime - MAX_POINTS * 100, m_latestDataTime);
    } else {
        m_axisX->setRange(0, MAX_POINTS * 100);
    }
}

void TofChartManager::enforceAxisLimits(qreal min, qreal max)
{
    bool needAdjustment = false;
    qreal newMin = min;
    qreal newMax = max;
    qreal rangeSize = max - min;

    if (min < 0) {
        newMin = 0;
        newMax = rangeSize;
        needAdjustment = true;
    }

    if (max > m_latestDataTime && m_latestDataTime > 0) {
        newMax = m_latestDataTime;
        newMin = qMax(0.0, m_latestDataTime - rangeSize);
        needAdjustment = true;
    }

    if (needAdjustment) {
        QSignalBlocker blocker(m_axisX);
        m_axisX->setRange(newMin, newMax);
    }
}

void TofChartManager::adjustAxisLabels()
{
    if (m_chartView && m_axisX) {
        int chartWidth = m_chartView->width();

        if (chartWidth <= 0)
            return;

        int labelCount;
        if (chartWidth < 100) {
            labelCount = 2;
        } else if (chartWidth < 200) {
            labelCount = 3;
        } else if (chartWidth < 300) {
            labelCount = 4;
        } else {
            labelCount = 5;
        }

        if (m_axisX->tickCount() != labelCount) {
            m_axisX->setTickCount(labelCount);
        }

        m_axisX->setTitleVisible(false);
        m_axisY->setTitleVisible(false);

        if (chartWidth < 150) {
            m_axisX->setLabelFormat("%.0f");
        } else {
            m_axisX->setLabelFormat("%.0f");
        }
    }
}
