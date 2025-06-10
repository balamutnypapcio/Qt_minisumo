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

    // Przenieś widget TOF do tego widgetu, jeśli nie jest już przypisany.
    if (m_tofWidget->parentWidget() != this) {
        m_tofWidget->setParent(this);
        layout()->addWidget(m_tofWidget);
    }

    // Utwórz kontener na strzałkę, jeśli go jeszcze nie ma.
    if (!m_arrowContainer) {
        m_arrowContainer = new QWidget(this);
        m_arrowContainer->setStyleSheet("background: transparent; border: none;");
        QHBoxLayout* arrowLayout = new QHBoxLayout(m_arrowContainer);
        arrowLayout->setContentsMargins(0, 0, 0, 0);
        arrowLayout->setSpacing(0);

        // Dodaj kontener na strzałkę do głównego layoutu
        layout()->addWidget(m_arrowContainer);
    }

    // Ustaw proporcje rozciągania layoutu: TOF (40%), strzałka (60%)
    QVBoxLayout* vLayout = qobject_cast<QVBoxLayout*>(layout());
    if (vLayout) {
        vLayout->setStretch(0, 40);
        vLayout->setStretch(1, 60);
    }
}

void ProportionalWidget::setupArrowWidget(QStackedWidget* arrowWidget, bool leftSide)
{
    if (!arrowWidget || !m_arrowContainer) return;

    m_arrowWidget = arrowWidget;
    m_isLeftSide = leftSide;

    // Przenieś widget strzałki do kontenera, jeśli nie jest już przypisany.
    if (m_arrowWidget->parentWidget() != m_arrowContainer) {
        m_arrowWidget->setParent(m_arrowContainer);
    }

    // Utwórz lub wyczyść layout dla kontenera strzałki.
    QHBoxLayout* arrowLayout = qobject_cast<QHBoxLayout*>(m_arrowContainer->layout());
    if (!arrowLayout) {
        delete m_arrowContainer->layout();
        arrowLayout = new QHBoxLayout(m_arrowContainer);
        arrowLayout->setContentsMargins(0, 0, 0, 0);
        arrowLayout->setSpacing(0);
    }

    // Wyczyść layout z poprzednich widgetów.
    while (QLayoutItem* item = arrowLayout->takeAt(0)) {
        if (item->widget()) {
            item->widget()->hide();
        }
        delete item;
    }

    // Dodaj widgety do layoutu zgodnie z pozycją (lewa/prawa strona).
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

    // Ustaw style ikon strzałek dla wszystkich stron stosu.
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

    // Pobierz wymiary robotView
    int robotWidth = robotView->width();
    int robotHeight = robotView->height();
    int minDim = qMin(robotWidth, robotHeight);

    // Rozmiar TOF: 40% z minimalnego wymiaru robotView
    int tofSize = minDim * 0.4;

    // Ustaw maksymalną wysokość widgetu TOF tak, żeby był kwadratem
    m_tofWidget->setMaximumHeight(tofSize);
    m_tofWidget->setMinimumHeight(0);
    m_tofWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Kontener na strzałkę dostaje resztę wysokości widgetu ProportionalWidget
    int widgetH = height();
    int arrowContainerHeight = widgetH - m_tofWidget->height();
    m_arrowContainer->setMaximumHeight(arrowContainerHeight > 0 ? arrowContainerHeight : 0);
    m_arrowContainer->setMinimumHeight(0);
    m_arrowContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Strzałka wewnątrz kontenera - 60% wysokości kontenera
    int arrowSize = m_arrowContainer->height() * 0.6;
    m_arrowWidget->setMaximumHeight(arrowSize);
    m_arrowWidget->setMaximumWidth(arrowSize);
    m_arrowWidget->setMinimumSize(0,0);
    m_arrowWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Ustaw współczynniki rozciągania (na wszelki wypadek)
    QVBoxLayout* vLayout = qobject_cast<QVBoxLayout*>(layout());
    if (vLayout) {
        vLayout->setStretch(0, 40);
        vLayout->setStretch(1, 60);
    }
}
