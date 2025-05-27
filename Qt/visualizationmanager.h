#ifndef VISUALIZATIONMANAGER_H
#define VISUALIZATIONMANAGER_H

#include <QObject>
#include <QWidget>
#include <QPainter>

// Forward declaration klas pomocniczych
class SensorData;
namespace Ui { class MainWindow; }

/**
 * @class VisualizationManager
 * @brief Zarządza wizualizacją stanu robota oraz aktualizuje widok na podstawie danych z sensorów.
 */
class VisualizationManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor menedżera wizualizacji.
     * @param sensorData Wskaźnik do danych z sensorów.
     * @param ui Wskaźnik do interfejsu użytkownika (MainWindow).
     * @param parent Rodzic QObject.
     */
    explicit VisualizationManager(SensorData* sensorData, Ui::MainWindow* ui, QObject* parent = nullptr);

    /**
     * @brief Inicjalizuje strzałki kierunku silników na starcie.
     */
    void setupArrows();

    /**
     * @brief Aktualizuje wskaźnik połączenia WiFi na podstawie statusu.
     * @param connected Czy połączono z ESP.
     */
    void updateConnectionStatusUI(bool connected);

public slots:
    /**
     * @brief Odświeża wszystkie elementy wizualizacji na podstawie bieżących danych sensorów.
     */
    void updateAll();

private:
    /**
     * @brief Odświeża strzałki silników na podstawie kierunku obrotów.
     */
    void updateMotorArrows();
    /**
     * @brief Odświeża wizualizację czujników TOF.
     */
    void updateTofSensors();
    /**
     * @brief Aktualizuje etykiety PWM silników.
     */
    void updateMotorLabels();
    /**
     * @brief Aktualizuje grafikę czujników linii.
     */
    void updateLsSensors();
    /**
     * @brief Odświeża i obraca strzałkę IMU.
     */
    void updateImuArrow();

    SensorData* m_sensorData;         ///< Wskaźnik do danych sensorów.
    Ui::MainWindow* m_ui;             ///< Wskaźnik do interfejsu użytkownika.
    QPixmap m_originalImuPixmap;      ///< Oryginalny obrazek strzałki IMU do obracania.

    /**
     * @brief Filtr zdarzeń do obsługi np. zmiany rozmiaru QLabel z IMU.
     * @param obj Wskaźnik do obserwowanego obiektu.
     * @param event Wskaźnik do zdarzenia.
     * @return true jeśli zdarzenie obsłużone, false w przeciwnym razie.
     */
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // VISUALIZATIONMANAGER_H
