#ifndef VISUALIZATIONMANAGER_H
#define VISUALIZATIONMANAGER_H

#include <QObject>
#include <QWidget>
#include <QPainter>

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
    void updateImuArrow();

    SensorData* m_sensorData;
    Ui::MainWindow* m_ui;
    QPixmap m_originalImuPixmap;
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // VISUALIZATIONMANAGER_H
