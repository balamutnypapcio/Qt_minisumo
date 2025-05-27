#ifndef CHARTVIEWHELPER_H
#define CHARTVIEWHELPER_H

#include <QtCharts/QChartView>
#include <QWheelEvent>
#include <QMouseEvent>
#include <functional>

/**
 * @class HorizontalOnlyChartView
 * @brief Specjalny widok wykresu QChartView, który umożliwia przewijanie i zoomowanie wyłącznie w poziomie.
 *
 * Klasa rozszerza QChartView, aby ograniczyć interakcje użytkownika tylko do osi X.
 * Dodatkowo obsługuje callbacki dla naciśnięcia i puszczenia my // Zmiana kursora na rękę zamkniętąszy, co umożliwia np. zarządzanie auto-przewijaniem.
 */
class HorizontalOnlyChartView : public QChartView {
public:
    /**
     * @brief Konstruktor.
     * @param chart Wskaźnik do obiektu QChart wyświetlanego w widoku.
     */
    explicit HorizontalOnlyChartView(QChart* chart);

    /**
     * @brief Destruktor.
     */
    virtual ~HorizontalOnlyChartView();

    /**
     * @brief Typ funkcji callback wywoływanej przy naciśnięciu/przesunięciu myszy.
     */
    typedef std::function<void()> MouseCallback;

    /**
     * @brief Ustawia callback wywoływany przy naciśnięciu przycisku myszy.
     * @param callback Funkcja typu void().
     */
    void setMousePressedCallback(MouseCallback callback);

    /**
     * @brief Ustawia callback wywoływany przy puszczeniu przycisku myszy.
     * @param callback Funkcja typu void().
     */
    void setMouseReleasedCallback(MouseCallback callback);

protected:
    /**
     * @brief Obsługa kółka myszy: umożliwia zoomowanie wyłącznie w poziomie.
     * @param event Zdarzenie kółka myszy.
     */
    void wheelEvent(QWheelEvent *event) override;

    /**
     * @brief Obsługa ruchu myszy: przesuwa wykres tylko w poziomie podczas przeciągania.
     * @param event Zdarzenie ruchu myszy.
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /**
     * @brief Obsługa puszczenia przycisku myszy: kończy przesuwanie wykresu.
     * @param event Zdarzenie puszczenia przycisku myszy.
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief Obsługa naciśnięcia przycisku myszy: rozpoczyna przesuwanie wykresu.
     * @param event Zdarzenie naciśnięcia przycisku myszy.
     */
    void mousePressEvent(QMouseEvent *event) override;

private:
    QPoint lastMousePos;               ///< Ostatnia zapamiętana pozycja kursora.
    bool isDragging = false;           ///< Czy aktualnie trwa przeciąganie wykresu.
    MouseCallback mousePressedCallback;   ///< Callback na naciśnięcie myszy.
    MouseCallback mouseReleasedCallback;  ///< Callback na puszczenie myszy.
};

#endif // CHARTVIEWHELPER_H
