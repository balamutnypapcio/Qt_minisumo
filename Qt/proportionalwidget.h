#ifndef PROPORTIONALWIDGET_H
#define PROPORTIONALWIDGET_H

#include <QWidget>

class QStackedWidget;
class QResizeEvent;

/**
 * @class ProportionalWidget
 * @brief Widget dzielący przestrzeń na część z czujnikiem TOF i strzałką według proporcji.
 *
 * Klasa ułatwia układanie elementów interfejsu robota, dzieląc dostępną przestrzeń
 * na górną część (czujnik TOF) i dolną (strzałka), zachowując określone proporcje.
 */
class ProportionalWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor domyślny.
     * @param parent Rodzic widgetu.
     */
    explicit ProportionalWidget(QWidget *parent = nullptr);

    /**
     * @brief Ustawia widget czujnika TOF (np. tofLEFT lub tofRIGHT).
     * @param tofWidget Wskaźnik do QStackedWidget z czujnikami TOF.
     */
    void setupTofWidget(QStackedWidget* tofWidget);

    /**
     * @brief Ustawia widget strzałki (np. M1_arrow lub M2_arrow).
     * @param arrowWidget Wskaźnik do QStackedWidget ze strzałkami.
     * @param leftSide Czy to jest lewa strona (true) czy prawa (false).
     */
    void setupArrowWidget(QStackedWidget* arrowWidget, bool leftSide);

protected:
    /**
     * @brief Obsługa zdarzenia zmiany rozmiaru widgetu.
     * @param event Zdarzenie zmiany rozmiaru.
     */
    void resizeEvent(QResizeEvent *event) override;

private:
    QStackedWidget* m_tofWidget;     ///< Widget czujnika TOF (tofLEFT/tofRIGHT).
    QWidget* m_arrowContainer;       ///< Kontener na widget strzałki.
    QStackedWidget* m_arrowWidget;   ///< Widget strzałki (M1_arrow/M2_arrow).
    bool m_isLeftSide;               ///< Czy to lewa strona (true) czy prawa (false).
};

#endif // PROPORTIONALWIDGET_H
