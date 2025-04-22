#ifndef VISUALIZATIONMANAGER_H
#define VISUALIZATIONMANAGER_H

#include <QObject>
#include <QLabel>
#include <QPixmap>
#include "sensordata.h"
#include "ui_mainwindow.h"
#include <QGraphicsOpacityEffect>
#include <QtMath>


/**
 * @class VisualizationManager
 * @brief Zarządza wizualizacją danych sensorów w GUI (strzałki, opacities, itp.).
 */
class VisualizationManager : public QObject {
    Q_OBJECT

public:
    explicit VisualizationManager(SensorData* sensorData, Ui::MainWindow* ui, QObject *parent = nullptr);

    /**
     * @brief Inicjalizuje obrazki strzałek.
     */
    void setupArrows();

    /**
     * @brief Aktualizuje widoczność elementów graficznych.
     */
    void updateVisibility();

    /**
     * @brief Aktualizuje strzałki silników.
     */
    void updateMotorArrows();

    /**
     * @brief Aktualizuje strzałkę IMU.
     */
    void updateImuArrow();

    /**
     * @brief Aktualizuje wartości prędkości silników.
     */
    void updateMotorSpeed();

    /**
     * @brief Aktualizuje wszystkie elementy wizualizacji.
     */
    void updateAll();

private:
    void updateArrowSize1();
    void updateArrowSize2();

    /**
     * @brief Ustawia przezroczystość strzałki.
     * @param arrowLabel QLabel z obrazkiem strzałki.
     */
    void setArrowOpacity(QLabel* arrowLabel);

    SensorData* m_sensorData;        ///< Wskaźnik do danych sensorów.
    Ui::MainWindow* m_ui;            ///< Wskaźnik do UI głównego okna.
    QPixmap m_originalArrowPixmap;   ///< Oryginalny obrazek strzałki silnika.
    QPixmap m_originalArrowIMU;      ///< Oryginalny obrazek strzałki IMU.
};

#endif // VISUALIZATIONMANAGER_H
