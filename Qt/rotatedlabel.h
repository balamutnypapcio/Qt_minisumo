#ifndef ROTATEDLABEL_H
#define ROTATEDLABEL_H

#include <QLabel>
#include <QPainter>

/**
 * @class RotatedLabel
 * @brief Etykieta z obróconym o 90 stopni tekstem
 *
 * Klasa rozszerzająca standardową QLabel, która wyświetla tekst obrócony
 * o 90 stopni przeciwnie do ruchu wskazówek zegara. Używana głównie do
 * wyświetlania pionowych etykiet osi na wykresach.
 */
class RotatedLabel : public QLabel
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor domyślny
     * @param parent Wskaźnik do widgetu rodzica
     */
    explicit RotatedLabel(QWidget* parent = nullptr) : QLabel(parent) {}

    /**
     * @brief Konstruktor z tekstem
     * @param text Tekst do wyświetlenia
     * @param parent Wskaźnik do widgetu rodzica
     */
    explicit RotatedLabel(const QString& text, QWidget* parent = nullptr) : QLabel(text, parent) {}

protected:
    /**
     * @brief Metoda obsługująca zdarzenie rysowania widgetu
     *
     * Implementacja rysuje tekst obrócony o 90 stopni przeciwnie do ruchu wskazówek zegara.
     * Przed rysowaniem czyści całe tło, aby zapobiec pozostawaniu artefaktów po poprzednim tekście.
     *
     * @param event Obiekt zdarzenia rysowania
     */
    void paintEvent(QPaintEvent* event) override {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Całkowicie wyczyść tło przed rysowaniem
        painter.fillRect(rect(), palette().color(QPalette::Window));

        // Jeśli tekst jest pusty, nie ma potrzeby dalszego rysowania
        if (text().isEmpty())
            return;

        // Zapisz aktualny stan
        painter.save();

        // Obrót o -90 stopni wokół środka
        painter.translate(width() / 2, height() / 2);
        painter.rotate(-90); // Obrót o -90 stopni

        // Obliczenie prostokąta dla obroconego tekstu
        QFontMetrics fm(font());
        int textWidth = fm.horizontalAdvance(text());
        int textHeight = fm.height();
        QRect textRect(-textWidth / 2, -textHeight / 2, textWidth, textHeight);

        // Narysuj tekst
        painter.setPen(palette().color(QPalette::WindowText));
        painter.drawText(textRect, Qt::AlignCenter, text());

        // Przywróć zapisany stan
        painter.restore();
    }

    /**
     * @brief Sugerowany rozmiar widgetu
     *
     * Zwraca sugerowany rozmiar widgetu, zamieniający szerokość i wysokość
     * w porównaniu do standardowej etykiety, aby uwzględnić obrót tekstu o 90 stopni.
     *
     * @return QSize Sugerowany rozmiar widgetu
     */
    QSize sizeHint() const override {
        QFontMetrics fm(font());
        // Zamiana szerokości i wysokości ze względu na obrót o 90 stopni
        return QSize(fm.height(), fm.horizontalAdvance(text()));
    }

    /**
     * @brief Minimalny sugerowany rozmiar widgetu
     *
     * Zwraca minimalny rozmiar widgetu potrzebny do poprawnego wyświetlenia obroconego tekstu.
     *
     * @return QSize Minimalny sugerowany rozmiar widgetu
     */
    QSize minimumSizeHint() const override {
        return sizeHint();
    }
};

#endif // ROTATEDLABEL_H
