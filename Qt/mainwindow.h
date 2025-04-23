#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "sensordata.h"
#include "csvmanager.h"
#include "tofchartmanager.h"
#include "linechartmenager.h"
#include "motorchartmanager.h"
#include "imuchartmanager.h"
#include "visualizationmanager.h"
#include "tcpmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @brief Klasa reprezentująca główne okno aplikacji.
 *
 * Zarządza interfejsem użytkownika, obsługuje dane z czujników oraz ich wizualizację.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Konstruktor klasy MainWindow.
     * @param parent Wskaźnik na obiekt nadrzędny (domyślnie nullptr).
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destruktor klasy MainWindow.
     */
    ~MainWindow();

private slots:
    void on_line1_button_clicked();
    void on_tof1_button_clicked();
    void on_dial_valueChanged(int value);
    void on_dial_2_valueChanged(int value);
    void on_x_dial_valueChanged(int value);
    void on_y_dial_valueChanged(int value);
    void on_load_fButton_clicked();

    /**
     * @brief Slot aktualizujący dane przy odtwarzaniu z pliku CSV.
     * @param time Czas w milisekundach od początku odtwarzania.
     */
    void handleDataUpdated(int time);

    /**
     * @brief Slot obsługujący połączenie TCP z ESP.
     */
    void on_connectTcpButton_clicked();

    /**
     * @brief Slot obsługujący rozłączenie TCP z ESP.
     */
    void on_disconnectTcpButton_clicked();

    /**
     * @brief Slot obsługujący zmianę stanu połączenia TCP.
     * @param connected Status połączenia.
     */
    void handleConnectionStatusChanged(bool connected);

    /**
     * @brief Slot obsługujący błędy TCP.
     * @param errorMessage Komunikat o błędzie.
     */
    void handleTcpError(const QString &errorMessage);


private:
    Ui::MainWindow *ui;                          ///< Wskaźnik do interfejsu użytkownika.

    SensorData* m_sensorData;                    ///< Wskaźnik na dane z czujników.
    CSVManager* m_csvManager;                    ///< Wskaźnik na menedżera pliku CSV.
    TCPManager* m_tcpManager;                    ///< Wskaźnik na menedżera połączenia TCP.
    TofChartManager* m_tofChartManager;          ///< Wskaźnik na menedżera wykresu ToF.
    LineChartMenager* m_lineChartManager;        ///< Wskaźnik na menedżera wykresu czujników linii.
    MotorChartManager* m_motorChartManager;      ///< Wskaźnik na menedżera wykresu silników.
    ImuChartManager* m_imuChartManager;          ///< Wskaźnik na menedżera wykresu IMU.
    VisualizationManager* m_visualManager;       ///< Wskaźnik na menedżera wizualizacji (strzałki, ikony itp.).
};

#endif // MAINWINDOW_H
