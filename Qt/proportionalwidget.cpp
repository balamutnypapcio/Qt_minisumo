#include "proportionalwidget.h"
#include <QResizeEvent>
#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

ProportionalWidget::ProportionalWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(2);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setLayout(mainLayout);

    m_tofWidget = nullptr;
    m_arrowContainer = nullptr;
    m_arrowWidget = nullptr;
}






void ProportionalWidget::setupTofWidget(QStackedWidget* tofWidget)
{
    if (!tofWidget) return;

    m_tofWidget = tofWidget;

    // Remove from its current parent and add to our layout
    if (m_tofWidget->parentWidget() != this) {
        m_tofWidget->setParent(this);
        layout()->addWidget(m_tofWidget);
    }

    // Create arrow container if needed
    if (!m_arrowContainer) {
        m_arrowContainer = new QWidget(this);
        m_arrowContainer->setStyleSheet("background: transparent; border: none;");
        QHBoxLayout* arrowLayout = new QHBoxLayout(m_arrowContainer);
        arrowLayout->setContentsMargins(0, 0, 0, 0);
        arrowLayout->setSpacing(0);

        // Add the arrow container to the main layout
        layout()->addWidget(m_arrowContainer);
    }

    // Set stretch factors on the layout
    QVBoxLayout* vLayout = qobject_cast<QVBoxLayout*>(layout());
    if (vLayout) {
        vLayout->setStretch(0, 40); // 40% for tof widget
        vLayout->setStretch(1, 60); // 60% for arrow container
    }
}

void ProportionalWidget::setupArrowWidget(QStackedWidget* arrowWidget, bool leftSide)
{
    if (!arrowWidget || !m_arrowContainer) return;

    m_arrowWidget = arrowWidget;
    m_isLeftSide = leftSide;

    // Remove from its current parent and add to our arrow container
    if (m_arrowWidget->parentWidget() != m_arrowContainer) {
        m_arrowWidget->setParent(m_arrowContainer);
    }

    // Create or recreate the layout for the arrow container
    QHBoxLayout* arrowLayout = qobject_cast<QHBoxLayout*>(m_arrowContainer->layout());
    if (!arrowLayout) {
        delete m_arrowContainer->layout();
        arrowLayout = new QHBoxLayout(m_arrowContainer);
        arrowLayout->setContentsMargins(0, 0, 0, 0);
        arrowLayout->setSpacing(0);
    }

    // Clear the layout
    while (QLayoutItem* item = arrowLayout->takeAt(0)) {
        if (item->widget()) {
            item->widget()->hide();
        }
        delete item;
    }

    // Add widgets to the layout according to side
    if (leftSide) {
        QWidget* spacer = new QWidget(m_arrowContainer);
        arrowLayout->addWidget(spacer, 60);
        arrowLayout->addWidget(m_arrowWidget, 40);
    } else {
        arrowLayout->addWidget(m_arrowWidget, 40);
        QWidget* spacer = new QWidget(m_arrowContainer);
        arrowLayout->addWidget(spacer, 60);
    }

    m_arrowWidget->show();

    // Reset all arrow icon styles in the stack
    for (int i = 0; i < m_arrowWidget->count(); i++) {
        QWidget* page = m_arrowWidget->widget(i);
        if (i == 0) {
            page->setStyleSheet(
                "border-image: none; "
                "background: transparent; "
                "border: none; "
                "image: url(:/new/image/img/arrowUp.png); "
                "image-position: center;"
                );
        } else if (i == 1) {
            page->setStyleSheet(
                "border-image: none; "
                "background: transparent; "
                "border: none; "
                "image: url(:/new/image/img/arrowDown.png); "
                "image-position: center;"
                );
        }
    }
}

void ProportionalWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    QWidget* robotView = nullptr;
    QWidget* parentWid = parentWidget();
    if (parentWid) {
        robotView = parentWid->findChild<QWidget*>("robotView");
    }

    if (!robotView || !m_tofWidget || !m_arrowContainer || !m_arrowWidget) {
        return;
    }

    // Get dimensions of the robot view
    int robotWidth = robotView->width();
    int robotHeight = robotView->height();
    int minDim = qMin(robotWidth, robotHeight);

    // Size for the tof widget (40% of the minimum dimension)
    int tofSize = minDim * 0.4;

    // Cały ProportionalWidget nie wymusza rozmiaru – tylko dzieli swoją wysokość na dwie części:
    // górna (tof) i dolna (arrow)

    // 1. Ustaw max wysokość tofLEFT, żeby był kwadratem, ale NIE wymuszaj szerokości/całej wysokości
    m_tofWidget->setMaximumHeight(tofSize);
    m_tofWidget->setMinimumHeight(0);
    // Ustaw politykę rozmiaru
    m_tofWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 2. ArrowContainer dostaje resztę wysokości widgetu ProportionalWidget
    int widgetH = height();
    int arrowContainerHeight = widgetH - m_tofWidget->height();
    m_arrowContainer->setMaximumHeight(arrowContainerHeight > 0 ? arrowContainerHeight : 0);
    m_arrowContainer->setMinimumHeight(0);
    m_arrowContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 3. Arrow wewnątrz kontenera – proporcjonalny (np. 60% wysokości kontenera)
    int arrowSize = m_arrowContainer->height() * 0.6;
    m_arrowWidget->setMaximumHeight(arrowSize);
    m_arrowWidget->setMaximumWidth(arrowSize);
    m_arrowWidget->setMinimumSize(0,0);
    m_arrowWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Stretch factors w layoucie (na wszelki wypadek)
    QVBoxLayout* vLayout = qobject_cast<QVBoxLayout*>(layout());
    if (vLayout) {
        vLayout->setStretch(0, 40);  // tof widget (góra)
        vLayout->setStretch(1, 60);  // arrow container (dół)
    }
}
