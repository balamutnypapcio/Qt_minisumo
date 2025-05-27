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
#include "rotatedlabel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @class MainWindow
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

    /**
     * @brief Ustawia etykiety silników.
     */
    void labelMotors();

private slots:
    /**
     * @brief Obsługuje kliknięcie przycisku ładowania pliku CSV.
     */
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
     * @brief Obsługuje kliknięcie przycisku zatrzymania odtwarzania CSV.
     *
     * Ta metoda zatrzymuje odtwarzanie danych z pliku CSV poprzez wywołanie
     * odpowiedniej metody w CSVManager. Po zatrzymaniu wyświetla komunikat
     * w pasku statusu.
     */
    void on_buttSTOPcsv_clicked();

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

    /**
     * @brief Wyświetla potwierdzenie wyjścia z aplikacji.
     */
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

    /**
     * @brief Konfiguruje widgety proporcjonalne dla czujników.
     */
    void setupProportionalWidgets();

    ProportionalWidget *leftSensorWidget;        ///< Widget proporcjonalny dla lewego czujnika.
    ProportionalWidget *rightSensorWidget;       ///< Widget proporcjonalny dla prawego czujnika.

    /**
     * @brief Ustawia proporcje w grid layout.
     */
    void adjustGridLayoutProportions();

    bool m_isEnglish = true;                     ///< Flaga określająca aktualny język.
    QMap<QLabel*, QString> m_englishTexts;       ///< Mapa etykiet i ich angielskich tekstów.
    QMap<QLabel*, QString> m_polishTexts;        ///< Mapa etykiet i ich polskich tekstów.
    QMap<QPushButton*, QString> m_englishButtonTexts; ///< Mapa przycisków i ich angielskich tekstów.
    QMap<QPushButton*, QString> m_polishButtonTexts;  ///< Mapa przycisków i ich polskich tekstów.

    /**
     * @brief Inicjalizuje teksty do tłumaczeń.
     */
    void initializeTranslations();

    /**
     * @brief Przełącza język interfejsu.
     */
    void switchLanguage();

    /**
     * @brief Wczytuje preferencję języka z ustawień.
     */
    void loadLanguagePreference();

    /**
     * @brief Zapisuje preferencję języka do ustawień.
     */
    void saveLanguagePreference();

    /**
     * @brief Zastosuj polskie tłumaczenie do interfejsu.
     */
    void applyPolishTranslation();

    /**
     * @brief Tworzy i ustawia etykiety obrotowe.
     */
    void setupRotatedLabels();

    /**
     * @brief Zastępuje wskazaną etykietę nową etykietą.
     * @param oldLabel Stara etykieta.
     * @param newLabel Nowa etykieta.
     */
    void replaceLabel(QLabel* oldLabel, QLabel* newLabel);

    /**
     * @brief Wymusza odświeżenie widgetów rodzica po zmianie języka.
     */
    void refreshParentWidgets();
};

#endif // MAINWINDOW_H
