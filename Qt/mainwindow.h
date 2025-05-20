#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QApplication>
#include <QSettings>
#include "sensordata.h"
#include "csvmanager.h"
#include "tofchartmanager.h"
#include "linechartmenager.h"
#include "motorchartmanager.h"
#include "imuchartmanager.h"
#include "visualizationmanager.h"
#include "tcpmanager.h"
#include "proportionalwidget.h"

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

    void on_buttLOADcsv_clicked();
    //void on_buttSTOPcsv_clicked();

    /**
     * @brief Slot aktualizujący dane przy odtwarzaniu z pliku CSV.
     * @param time Czas w milisekundach od początku odtwarzania.
     */
    void handleDataUpdated(int time);

    /**
     * @brief Slot obsługujący połączenie TCP z ESP.
     */
    void on_buttCONN_clicked();

    /**
     * @brief Slot obsługujący rozłączenie TCP z ESP.
     */
    void on_buttDISS_clicked();

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


    void showExitConfirmation();


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
    void setupProportionalWidgets();
    ProportionalWidget *leftSensorWidget;
    ProportionalWidget *rightSensorWidget;
    void adjustGridLayoutProportions();
    bool m_isEnglish = true; // Domyślnie angielski
    QMap<QLabel*, QString> m_englishTexts; // Teksty angielskie
    QMap<QLabel*, QString> m_polishTexts;  // Teksty polskie
    QMap<QPushButton*, QString> m_englishButtonTexts;
    QMap<QPushButton*, QString> m_polishButtonTexts;
    void initializeTranslations(); // Inicjalizacja tekstów
    void switchLanguage();         // Przełączanie języka
    void loadLanguagePreference();
    void saveLanguagePreference();
    void applyPolishTranslation();

};

#endif // MAINWINDOW_H
