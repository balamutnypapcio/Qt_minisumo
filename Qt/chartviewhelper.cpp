#include "chartviewhelper.h"
#include <QApplication>

HorizontalOnlyChartView::HorizontalOnlyChartView(QChart* chart)
    : QChartView(chart), lastMousePos(), isDragging(false)
{
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

HorizontalOnlyChartView::~HorizontalOnlyChartView()
{}

void HorizontalOnlyChartView::setMousePressedCallback(MouseCallback callback)
{
    mousePressedCallback = callback;
}

void HorizontalOnlyChartView::setMouseReleasedCallback(MouseCallback callback)
{
    mouseReleasedCallback = callback;
}

void HorizontalOnlyChartView::wheelEvent(QWheelEvent *event)
{
    // Tworzy nowe zdarzenie z wyzerowaną składową pionową, aby wymusić zoom tylko w poziomie.
    QWheelEvent horizontalEvent(
        event->position(),
        event->globalPosition(),
        QPoint(event->pixelDelta().x(), 0),
        QPoint(event->angleDelta().x(), 0),
        event->buttons(),
        event->modifiers(),
        event->phase(),
        event->inverted(),
        event->source()
        );
    QChartView::wheelEvent(&horizontalEvent);
}

void HorizontalOnlyChartView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastMousePos = event->pos();
        isDragging = true;
        setCursor(Qt::ClosedHandCursor);
        if (mousePressedCallback) {
            mousePressedCallback();
        }
    }
    QChartView::mousePressEvent(event);
}

void HorizontalOnlyChartView::mouseMoveEvent(QMouseEvent *event)
{
    if (isDragging && !lastMousePos.isNull()) {
        // Oblicza przesunięcie wyłącznie w osi X
        QPoint delta(event->pos().x() - lastMousePos.x(), 0);
        if (!delta.isNull()) {
            chart()->scroll(-delta.x(), 0);
        }
        lastMousePos = event->pos();
        event->accept();
        return;
    } else if (rubberBand() != QChartView::NoRubberBand) {
        // Gdy nie przeciągamy, zmieniamy kursor na "otwartą dłoń"
        setCursor(Qt::OpenHandCursor);
    }
    QChartView::mouseMoveEvent(event);
}

void HorizontalOnlyChartView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isDragging) {
        isDragging = false;
        setCursor(Qt::OpenHandCursor);
        if (mouseReleasedCallback) {
            mouseReleasedCallback();
        }
    }
    lastMousePos = QPoint();
    QChartView::mouseReleaseEvent(event);
}
