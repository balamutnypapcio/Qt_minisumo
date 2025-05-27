#include "chartviewhelper.h"
#include <QApplication>

/**
 * @brief Konstruktor widoku wykresu umożliwiającego jedynie poziome przewijanie i zoom.
 * @param chart Wskaźnik do obiektu QChart do wyświetlania.
 */
HorizontalOnlyChartView::HorizontalOnlyChartView(QChart* chart)
    : QChartView(chart), lastMousePos(), isDragging(false)
{
    setMouseTracking(true);

    // Ustawiamy politykę rozmiaru - wykres ma się rozciągać, ale z minimalną wielkością
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

/**
 * @brief Destruktor widoku wykresu.
 */
HorizontalOnlyChartView::~HorizontalOnlyChartView()
{}

/**
 * @brief Ustawia callback wywoływany przy naciśnięciu przycisku myszy.
 * @param callback Funkcja typu void().
 */
void HorizontalOnlyChartView::setMousePressedCallback(MouseCallback callback)
{
    mousePressedCallback = callback;
}

/**
 * @brief Ustawia callback wywoływany przy puszczeniu przycisku myszy.
 * @param callback Funkcja typu void().
 */
void HorizontalOnlyChartView::setMouseReleasedCallback(MouseCallback callback)
{
    mouseReleasedCallback = callback;
}

/**
 * @brief Obsługuje zdarzenie przewijania kółkiem myszy – pozwala tylko na przewijanie w poziomie.
 * @param event Zdarzenie kółka myszy.
 */
void HorizontalOnlyChartView::wheelEvent(QWheelEvent *event)
{
    // Przechwycenie zdarzenia scroll i zamiana na tylko poziome
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

/**
 * @brief Obsługuje naciśnięcie przycisku myszy – rozpoczyna przeciąganie wykresu w poziomie.
 * @param event Zdarzenie naciśnięcia przycisku myszy.
 */
void HorizontalOnlyChartView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastMousePos = event->pos();
        isDragging = true;
        setCursor(Qt::ClosedHandCursor); // Zmiana kursora na rękę zamkniętą
        if (mousePressedCallback) {
            mousePressedCallback();
        }
    }
    QChartView::mousePressEvent(event);
}

/**
 * @brief Obsługuje ruch myszy – przesuwa wykres tylko w poziomie podczas przeciągania.
 * @param event Zdarzenie ruchu myszy.
 */
void HorizontalOnlyChartView::mouseMoveEvent(QMouseEvent *event)
{
    if (isDragging && !lastMousePos.isNull()) {
        // Obliczamy przesunięcie tylko w poziomie
        QPoint delta(event->pos().x() - lastMousePos.x(), 0);
        if (!delta.isNull()) {
            // Przesuwamy wykres
            chart()->scroll(-delta.x(), 0);
        }
        lastMousePos = event->pos();
        event->accept();
        return;
    } else if (rubberBand() != QChartView::NoRubberBand) {
        // Gdy kursor jest nad wykresem, zmieniamy kursor na rękę otwartą
        setCursor(Qt::OpenHandCursor);
    }
    QChartView::mouseMoveEvent(event);
}

/**
 * @brief Obsługuje puszczenie przycisku myszy – kończy przeciąganie wykresu.
 * @param event Zdarzenie puszczenia przycisku myszy.
 */
void HorizontalOnlyChartView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isDragging) {
        isDragging = false;
        setCursor(Qt::OpenHandCursor); // Zmiana kursora na rękę otwartą
        if (mouseReleasedCallback) {
            mouseReleasedCallback();
        }
    }
    lastMousePos = QPoint();
    QChartView::mouseReleaseEvent(event);
}
