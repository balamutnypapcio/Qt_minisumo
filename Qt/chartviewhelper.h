#ifndef CHARTVIEWHELPER_H
#define CHARTVIEWHELPER_H

#include <QtCharts/QChartView>
#include <QWheelEvent>
#include <QMouseEvent>
#include <functional>

/**
 * @class HorizontalOnlyChartView
 * @brief Specjalistyczny widok QChartView umożliwiający przewijanie i zoomowanie wyłącznie w osi poziomej (X).
 *
 * Klasa rozszerza QChartView, ograniczając interakcje użytkownika tylko do osi X (tylko poziome przewijanie i skalowanie).
 * Obsługuje callbacki dla naciśnięcia i puszczenia przycisku myszy, co jest przydatne np. do zarządzania auto-przewijaniem
 * lub synchronizacją z innymi komponentami GUI.
 *
 * Przykład użycia:
 * @code
 * QChart *chart = new QChart();
 * HorizontalOnlyChartView *view = new HorizontalOnlyChartView(chart);
 * view->setMousePressedCallback([this](){ stopAutoScroll(); });
 * view->setMouseReleasedCallback([this](){ startAutoScroll(); });
 * @endcode
 *
 * @note Przesuwanie i zoomowanie pionowe (oś Y) są zablokowane.
 * @note Kursor automatycznie zmienia się na "otwartą" lub "zamkniętą" dłoń w zależności od stanu przeciągania.
 * @author Jakub Wilczyński
 */
class HorizontalOnlyChartView : public QChartView {
public:
    /**
     * @brief Konstruktor widoku wykresu umożliwiającego tylko poziome przewijanie i zoom.
     * @param chart Wskaźnik do obiektu QChart wyświetlanego w widoku. Nie może być nullptr.
     */
    explicit HorizontalOnlyChartView(QChart* chart);

    /**
     * @brief Wirtualny destruktor.
     */
    virtual ~HorizontalOnlyChartView();

    /**
     * @brief Typ funkcji callback wywoływanej przy naciśnięciu lub puszczeniu myszy.
     * Sygnatura: void ()
     */
    typedef std::function<void()> MouseCallback;

    /**
     * @brief Ustawia callback wywoływany przy naciśnięciu lewego przycisku myszy na wykresie.
     * @param callback Funkcja typu void(), wywoływana przy rozpoczęciu przeciągania.
     *
     * Możesz wykorzystać np. do zatrzymania auto-przewijania.
     */
    void setMousePressedCallback(MouseCallback callback);

    /**
     * @brief Ustawia callback wywoływany przy puszczeniu lewego przycisku myszy na wykresie.
     * @param callback Funkcja typu void(), wywoływana przy zakończeniu przeciągania.
     *
     * Możesz wykorzystać np. do wznowienia auto-przewijania.
     */
    void setMouseReleasedCallback(MouseCallback callback);

protected:
    /**
     * @brief Obsługa kółka myszy: umożliwia zoomowanie wyłącznie w poziomie (oś X).
     * @param event Zdarzenie kółka myszy.
     *
     * Ignoruje pionowy komponent scrolla, pozwalając jedynie na zoom w poziomie.
     */
    void wheelEvent(QWheelEvent *event) override;

    /**
     * @brief Obsługa ruchu myszy: przesuwa wykres tylko w poziomie podczas przeciągania.
     * @param event Zdarzenie ruchu myszy.
     *
     * Przeciąganie przesuwa wykres tylko w lewo/prawo.
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /**
     * @brief Obsługa puszczenia przycisku myszy: kończy przeciąganie wykresu.
     * @param event Zdarzenie puszczenia przycisku myszy.
     *
     * Wywołuje callback (jeśli ustawiony), resetuje stan przeciągania oraz zmienia kursor.
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief Obsługa naciśnięcia przycisku myszy: rozpoczyna przeciąganie wykresu.
     * @param event Zdarzenie naciśnięcia przycisku myszy.
     *
     * Wywołuje callback (jeśli ustawiony) i zmienia kursor na "zamkniętą dłoń".
     */
    void mousePressEvent(QMouseEvent *event) override;

private:
    QPoint lastMousePos;                 ///< Ostatnia zapamiętana pozycja kursora podczas przeciągania.
    bool isDragging = false;             ///< Czy aktualnie trwa przeciąganie wykresu.
    MouseCallback mousePressedCallback;  ///< Callback na naciśnięcie myszy (rozpoczęcie przeciągania).
    MouseCallback mouseReleasedCallback; ///< Callback na puszczenie myszy (zakończenie przeciągania).
};

#endif // CHARTVIEWHELPER_H
