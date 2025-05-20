#ifndef VISUALIZATIONMANAGER_H
#define VISUALIZATIONMANAGER_H

#include <QObject>
#include <QWidget>

// Forward declarations
class SensorData;
namespace Ui {
class MainWindow;
}

class VisualizationManager : public QObject
{
    Q_OBJECT

public:
    explicit VisualizationManager(SensorData* sensorData, Ui::MainWindow* ui, QObject* parent = nullptr);
    void setupArrows();
    void updateConnectionStatusUI(bool connected);

public slots:
    void updateAll();

private:
    void updateMotorArrows();
    void updateTofSensors();
    void updateMotorLabels();
    void updateLsSensors();

    SensorData* m_sensorData;
    Ui::MainWindow* m_ui;
};

#endif // VISUALIZATIONMANAGER_H
