#ifndef PROPORTIONALWIDGET_H
#define PROPORTIONALWIDGET_H

#include <QWidget>

class QStackedWidget;
class QResizeEvent;

class ProportionalWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProportionalWidget(QWidget *parent = nullptr);

    // Method to set up the tof widget (tofLEFT or tofRIGHT)
    void setupTofWidget(QStackedWidget* tofWidget);

    // Method to set up the arrow widget (M1_arrow or M2_arrow)
    void setupArrowWidget(QStackedWidget* arrowWidget, bool leftSide);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QStackedWidget* m_tofWidget;     // tofLEFT or tofRIGHT
    QWidget* m_arrowContainer;       // Container for the arrow widget
    QStackedWidget* m_arrowWidget;   // M1_arrow or M2_arrow
    bool m_isLeftSide;               // Whether this is the left or right side
};

#endif // PROPORTIONALWIDGET_H
