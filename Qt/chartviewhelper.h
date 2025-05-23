#ifndef CHARTVIEWHELPER_H
#define CHARTVIEWHELPER_H

#include <QtCharts/QChartView>
#include <QWheelEvent>
#include <QMouseEvent>
#include <functional>


class HorizontalOnlyChartView : public QChartView {
public:
    explicit HorizontalOnlyChartView(QChart* chart);
    virtual ~HorizontalOnlyChartView();

    // Funkcje callback zamiast sygnałów
    typedef std::function<void()> MouseCallback;
    void setMousePressedCallback(MouseCallback callback);
    void setMouseReleasedCallback(MouseCallback callback);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QPoint lastMousePos;
    bool isDragging = false;
    MouseCallback mousePressedCallback;
    MouseCallback mouseReleasedCallback;
};

#endif // CHARTVIEWHELPER_H
