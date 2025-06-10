#ifndef VISUALIZATIONMANAGER_H
#define VISUALIZATIONMANAGER_H

#include <QObject>
#include <QWidget>
#include <QPainter>

/**
 * Forward declarations to avoid unnecessary includes.
 */
class SensorData;
namespace Ui { class MainWindow; }

/**
 * @class VisualizationManager
 * @brief Zarządza wizualizacją stanu robota oraz aktualizuje widok na podstawie danych z sensorów.
 *
 * Klasa odpowiada za:
 *  - Dynamiczną aktualizację graficznych elementów GUI związanych ze stanem robota (strzałki silników, czujniki, IMU, status WiFi).
 *  - Synchronizację stanów graficznych z wartościami z SensorData.
 *  - Obsługę zdarzeń resize/paint dla wybranych widgetów.
 *
 * Przykład użycia:
 * @code
 * VisualizationManager* visMan = new VisualizationManager(sensorData, ui, this);
 * connect(sensorData, &SensorData::dataChanged, visMan, &VisualizationManager::updateAll);
 * @endcode
 *
 * @author Jakub Wilczyński
 */
class VisualizationManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor menedżera wizualizacji.
     * @param sensorData Wskaźnik do danych z sensorów.
     * @param ui Wskaźnik do interfejsu użytkownika (MainWindow).
     * @param parent Rodzic QObject (opcjonalnie).
     */
    explicit VisualizationManager(SensorData* sensorData, Ui::MainWindow* ui, QObject* parent = nullptr);

    /**
     * @brief Inicjalizuje strzałki kierunku silników na starcie.
     *
     * Ustawia obrazy (np. QPixmap) i przygotowuje elementy wizualne.
     */
    void setupArrows();

    /**
     * @brief Aktualizuje wskaźnik połączenia WiFi na podstawie statusu.
     * @param connected True jeśli ESP jest połączone, false jeśli nie.
     */
    void updateConnectionStatusUI(bool connected);

public slots:
    /**
     * @brief Odświeża wszystkie elementy wizualizacji na podstawie bieżących danych sensorów.
     *
     * Wywoływane przy każdej zmianie danych – aktualizuje silniki, czujniki, IMU, itp.
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
     * @brief Filtr zdarzeń do obsługi (np. zmiany rozmiaru QLabel z IMU).
     * @param obj Wskaźnik do obserwowanego obiektu.
     * @param event Wskaźnik do zdarzenia.
     * @return true jeśli zdarzenie obsłużone, false w przeciwnym razie.
     *
     * Może być wykorzystywane do dynamicznego skalowania grafiki lub obsługi repaint.
     */
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // VISUALIZATIONMANAGER_H
