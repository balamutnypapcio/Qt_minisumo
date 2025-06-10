#ifndef ROTATEDLABEL_H
#define ROTATEDLABEL_H

#include <QLabel>
#include <QPainter>

/**
 * @class RotatedLabel
 * @brief Etykieta z obróconym o 90 stopni tekstem.
 *
 * Klasa rozszerzająca standardową QLabel, która wyświetla tekst obrócony
 * o 90 stopni przeciwnie do ruchu wskazówek zegara. Używana głównie do
 * pionowych etykiet osi na wykresach, lecz może być wykorzystana wszędzie,
 * gdzie potrzebny jest pionowy napis.
 *
 * - Rozszerza QLabel.
 * - Obraca tekst o -90 stopni (CCW) wokół środka widgetu.
 * - Dba o czytelność i poprawne czyszczenie tła.
 * - Sugeruje rozmiar zamieniając szerokość z wysokością (względem standardowej etykiety).
 *
 * Przykład użycia:
 * @code
 * auto* label = new RotatedLabel("Oś Y", this);
 * layout->addWidget(label);
 * @endcode
 *
 * @author Jakub Wilczyński
 */
class RotatedLabel : public QLabel
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor domyślny.
     * @param parent Wskaźnik do widgetu rodzica.
     */
    explicit RotatedLabel(QWidget* parent = nullptr) : QLabel(parent) {}

    /**
     * @brief Konstruktor z tekstem inicjalnym.
     * @param text Tekst do wyświetlenia.
     * @param parent Wskaźnik do widgetu rodzica.
     */
    explicit RotatedLabel(const QString& text, QWidget* parent = nullptr) : QLabel(text, parent) {}

protected:
    /**
     * @brief Obsługuje zdarzenie rysowania widgetu.
     *
     * Rysuje tekst obrócony o 90 stopni przeciwnie do ruchu wskazówek zegara.
     * Przed rysowaniem czyści tło, aby nie zostawiać artefaktów.
     *
     * @param event Obiekt zdarzenia rysowania.
     */
    void paintEvent(QPaintEvent* event) override {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Wyczyść tło
        painter.fillRect(rect(), palette().color(QPalette::Window));

        if (text().isEmpty())
            return;

        painter.save();

        // Obrót o -90 stopni wokół środka widgetu
        painter.translate(width() / 2, height() / 2);
        painter.rotate(-90);

        // Oblicz prostokąt pod tekst
        QFontMetrics fm(font());
        int textWidth = fm.horizontalAdvance(text());
        int textHeight = fm.height();
        QRect textRect(-textWidth / 2, -textHeight / 2, textWidth, textHeight);

        painter.setPen(palette().color(QPalette::WindowText));
        painter.drawText(textRect, Qt::AlignCenter, text());

        painter.restore();
    }

    /**
     * @brief Sugerowany rozmiar widgetu.
     *
     * Zamienia szerokość i wysokość standardowej etykiety, aby uwzględnić obrót tekstu.
     *
     * @return QSize sugerowany rozmiar widgetu.
     */
    QSize sizeHint() const override {
        QFontMetrics fm(font());
        return QSize(fm.height(), fm.horizontalAdvance(text()));
    }

    /**
     * @brief Minimalny sugerowany rozmiar widgetu.
     *
     * @return QSize minimalny rozmiar widgetu.
     */
    QSize minimumSizeHint() const override {
        return sizeHint();
    }
};

#endif // ROTATEDLABEL_H
