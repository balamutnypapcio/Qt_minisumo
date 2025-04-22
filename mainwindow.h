#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QFileDialog>
#include <QDateTime>
#include <QElapsedTimer>
#include <QtMath>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief Główna klasa aplikacji, zarządzająca interfejsem użytkownika.
 *
 * Ta klasa odpowiada za tworzenie interfejsu aplikacji, zarządzanie wykresami,
 * oraz obsługę różnych funkcji związanych z wizualizowaniem danych.
 * Obejmuje funkcje związane z aktualizowaniem zmiennych, generowaniem wykresów,
 * oraz obsługą timerów dla odtwarzania danych z pliku CSV.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor klasy MainWindow.
     *
     * Tworzy główną aplikację, inicjalizuje interfejs użytkownika i ustawia początkowe wartości.
     * @param parent Wskaźnik do rodzica (domyślnie nullptr).
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destruktor klasy MainWindow.
     *
     * Uwalnia zasoby używane przez obiekt MainWindow.
     */
    ~MainWindow();

private slots:
    /**
     * @brief Funkcja do aktualizacji widoczności elementów interfejsu.
     *
     * Przełącza widoczność odpowiednich komponentów na interfejsie użytkownika.
     */
    void updateVisibility();

    /**
     * @brief Funkcja do aktualizacji prędkości silników.
     *
     * Zmienia wartości prędkości silników na podstawie danych wejściowych.
     */
    void updateMotorSpeed();

    /**
     * @brief Funkcja do aktualizacji rozmiaru strzałki (strzałka 1).
     *
     * Zmienia rozmiar strzałki, zależnie od danych.
     */
    void updateArrowSize1();

    /**
     * @brief Funkcja do aktualizacji rozmiaru strzałki (strzałka 2).
     *
     * Zmienia rozmiar drugiej strzałki w zależności od danych.
     */
    void updateArrowSize2();

    /**
     * @brief Funkcja do aktualizacji rozmiaru strzałki IMU.
     *
     * Zmienia rozmiar strzałki IMU na podstawie wartości.
     */
    void updateArrowIMU();

    /**
     * @brief Funkcja do aktualizacji wszystkich elementów interfejsu.
     *
     * Odświeża wszystkie dane wyświetlane na interfejsie użytkownika.
     */
    void updateAll();

    /**
     * @brief Funkcja ustawiająca przezroczystość strzałki.
     *
     * Ustala przezroczystość elementu QLabel reprezentującego strzałkę.
     * @param arrowLabel Wskaźnik do obiektu QLabel reprezentującego strzałkę.
     */
    void setArrowOpacity(QLabel* arrowLabel);

    /**
     * @brief Funkcja do ustawiania strzałek na interfejsie.
     *
     * Inicjalizuje i ustawia strzałki na odpowiednich miejscach interfejsu.
     */
    void setUpArrows();

    /**
     * @brief Funkcja obsługująca kliknięcie przycisku dla linii 1.
     */
    void on_line1_button_clicked();

    /**
     * @brief Funkcja obsługująca kliknięcie przycisku dla ToF 1.
     */
    void on_tof1_button_clicked();

    /**
     * @brief Funkcja do obsługi zmiany wartości pokrętła (dial).
     *
     * Zmienia wartość parametru powiązanego z dialem (pokrętłem).
     * @param value Nowa wartość ustawiona na dialu.
     */
    void on_dial_valueChanged(int value);

    /**
     * @brief Funkcja do obsługi zmiany wartości pokrętła (dial 2).
     *
     * Zmienia wartość parametru powiązanego z drugim dialem.
     * @param value Nowa wartość ustawiona na dialu.
     */
    void on_dial_2_valueChanged(int value);

    /**
     * @brief Funkcja do obsługi zmiany wartości pokrętła dla osi X.
     *
     * Zmienia wartość parametru na osi X.
     * @param value Nowa wartość osi X.
     */
    void on_x_dial_valueChanged(int value);

    /**
     * @brief Funkcja do obsługi zmiany wartości pokrętła dla osi Y.
     *
     * Zmienia wartość parametru na osi Y.
     * @param value Nowa wartość osi Y.
     */
    void on_y_dial_valueChanged(int value);

    /**
     * @brief Funkcja do ładowania ustawień z pliku CSV.
     *
     * Odczytuje dane z pliku CSV i zapisuje je w odpowiednich zmiennych.
     * @param filePath Ścieżka do pliku CSV.
     */
    void on_load_fButton_clicked();

    /**
     * @brief Funkcja do ustawiania wykresu ToF (Time of Flight).
     */
    void setupTofChart();

    /**
     * @brief Funkcja do ustawiania wykresu dla linii.
     */
    void setupLineSChart();

    /**
     * @brief Funkcja do ustawiania wykresu prędkości silników.
     */
    void setupMotorsChart();

    /**
     * @brief Funkcja do ustawiania wykresu dla IMU (Inertial Measurement Unit).
     */
    void setupImuChart();

    /**
     * @brief Funkcja do konfiguracji wszystkich wykresów w aplikacji.
     */
    void setupCharts();

    /**
     * @brief Funkcja do aktualizacji wykresów na podstawie danych czasowych.
     *
     * Aktualizuje dane na wykresach w zależności od upływającego czasu.
     * @param time Czas, na podstawie którego odbywa się aktualizacja.
     */
    void updateCharts(int time);

    /**
     * @brief Funkcja do czyszczenia danych wykresów.
     *
     * Usuwa wszystkie dane z wykresów.
     */
    void clearCharts();

private:
    Ui::MainWindow *ui; ///< Wskaźnik do interfejsu użytkownika

    QElapsedTimer playbackTimer; ///< Timer do pomiaru czasu odtwarzania

    QTimer* dataUpdateTimer = nullptr; ///< Timer do aktualizacji danych
    int currentRow = 0; ///< Numer aktualnego wiersza w danych CSV
    QVector<QStringList> csvData; ///< Dane odczytane z pliku CSV

    /**
     * @brief Funkcja do ładowania ustawień z pliku CSV.
     * @param filePath Ścieżka do pliku CSV.
     */
    void loadSettingsFromCSV(const QString &filePath);

    /**
     * @brief Funkcja do aktualizacji zmiennych na podstawie danych z pliku CSV.
     * @param row Numer wiersza w pliku CSV.
     */
    void updateVariablesFromCSV(int row);

    /**
     * @brief Funkcja do uruchamiania timera aktualizacji danych.
     */
    void startDataUpdateTimer();

    /**
     * @brief Funkcja do aktualizacji danych w czasie rzeczywistym.
     */
    void updateData();

    // Zmienne do przechowywania stanów różnych elementów interfejsu i urządzeń
    bool isTof_1 = true; ///< Stan czujnika ToF 1
    bool isTof_2 = false; ///< Stan czujnika ToF 2
    bool isTof_3 = false; ///< Stan czujnika ToF 3
    bool isTof_4 = false; ///< Stan czujnika ToF 4

    bool islineS_1_Active = true; ///< Stan linii S 1
    bool islineS_2_Active = false; ///< Stan linii S 2
    bool islineS_3_Active = false; ///< Stan linii S 3

    int motor1_speed = 0; ///< Prędkość pierwszego silnika
    int motor2_speed = 0; ///< Prędkość drugiego silnika

    float imuX = 0; ///< Wartość odczytu z IMU (osi X)
    float imuY = 0; ///< Wartość odczytu z IMU (osi Y)

    QPixmap originalArrowPixmap; ///< Oryginalny obrazek strzałki
    QPixmap originalArrowIMU; ///< Oryginalny obrazek strzałki IMU

    // Wskaźniki do wykresów
    QChart *tofChart;
    QChart *lineSChart;
    QChart *motorsChart;
    QChart *imuChart;

    // Serie danych
    QLineSeries *tofSeries1;
    QLineSeries *tofSeries2;
    QLineSeries *tofSeries3;
    QLineSeries *tofSeries4;

    QLineSeries *lineSeries1;
    QLineSeries *lineSeries2;
    QLineSeries *lineSeries3;

    QLineSeries *motorSeries1;
    QLineSeries *motorSeries2;

    QLineSeries *imuSeriesX;
    QLineSeries *imuSeriesY;

    // Osie wykresów
    QValueAxis *tofAxisX;
    QValueAxis *tofAxisY;

    QValueAxis *lineAxisX;
    QValueAxis *lineAxisY;

    QValueAxis *motorAxisX;
    QValueAxis *motorAxisY;

    QValueAxis *imuAxisX;
    QValueAxis *imuAxisY;

    // Widoki wykresów
    QChartView *tofChartView;
    QChartView *lineChartView;
    QChartView *motorChartView;
    QChartView *imuChartView;

    // Bufor danych do wykresów (ograniczenie punktów)
    static const int MAX_POINTS = 100; ///< Maksymalna liczba punktów na wykresie
    int currentPoint = 0; ///< Aktualny punkt na wykresie
};

#endif // MAINWINDOW_H
