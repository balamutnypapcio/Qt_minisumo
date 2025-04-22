#include "mainwindow.h"
#include "ui_mainwindow.h"




/**
 * @file mainwindow.cpp
 * @brief Plik źródłowy implementujący logikę okna głównego aplikacji.
 *
 * Zawiera funkcje inicjalizacyjne, aktualizacyjne oraz obsługujące interakcje
 * użytkownika z różnymi widgetami w oknie głównym.
 */



/**
 * @brief Konstruktor klasy MainWindow.
 *
 * Inicjalizuje okno główne, ustawia interfejs użytkownika oraz inicjuje
 * wykresy i aktualizuje dane.
 *
 * @param parent Rodzic dla tego okna głównego.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupCharts(); //ustawienie wykresów
    updateAll(); ///< zaktualizaowanie wszystkich elementów widocznych w aplikacji

}


/**
 * @brief Destruktor klasy MainWindow.
 *
 * Usuwa dynamicznie alokowane zasoby, takie jak wskaźniki do wykresów.
 */
MainWindow::~MainWindow(){
    delete ui;
    delete tofChart;
    delete lineSChart;
    delete motorsChart;
    delete imuChart;
}


/**
 * @brief Ustawia strzałki w interfejsie.
 *
 * Ładuje odpowiednie obrazy strzałek dla silników oraz IMU i ustawia je
 * w widżetach.
 */
void MainWindow::setUpArrows(){
    originalArrowPixmap.load(":/new/image/img/arrowUp.png");
    ui->motor1_arrow->setPixmap(originalArrowPixmap);
    updateArrowSize1(); ///< aktualizuje rozmiar strzałki silnika1
    ui->motor2_arrow->setPixmap(originalArrowPixmap);
    updateArrowSize2(); ///< aktualizuje rozmiar strzałki silnika2

    originalArrowIMU.load(":/new/image/img/arrowIMU.png");
    ui->imu_arrow->setPixmap(originalArrowIMU);
    updateArrowIMU();///< Aktualizuje strzałkę IMU
    setArrowOpacity(ui->imu_arrow);///< Ustawia przezroczystość strzałki IMU

}
/**
 * @brief Aktualizuje wszystkie widoczności i parametry na interfejsie użytkownika.
 *
 * Ta funkcja jest wywoływana, aby zaktualizować widoczność elementów UI oraz parametry prędkości silników i strzałek.
 */
void MainWindow::updateAll(){
    updateVisibility();
    setUpArrows();
    updateMotorSpeed();
    updateArrowIMU();
}
/**
 * @brief Ustawia widoczność dla różnych elementów interfejsu użytkownika.
 *
 * Ustala widoczność elementów interfejsu w zależności od zmiennych warunkowych.
 *
 * @note Widoczność jest kontrolowana dla sensorów tof (1-4) oraz aktywności sensorów linii (1-3).
 */
void MainWindow::updateVisibility(){
    // Ustawienie widoczności dla tof_Red_1 do tof_Red_4
    ui->tof_Red_1->setVisible(isTof_1);
    ui->tof_Red_2->setVisible(isTof_2);
    ui->tof_Red_3->setVisible(isTof_3);
    ui->tof_Red_4->setVisible(isTof_4);

    // Ustawienie widoczności dla lineS_Active_1 do lineS_Active_3
    ui->lineS_Active_1->setVisible(islineS_1_Active);
    ui->lineS_Active_2->setVisible(islineS_2_Active);
    ui->lineS_Active_3->setVisible(islineS_3_Active);
}


/**
 * @brief Aktualizuje etykiety prędkości silników.
 *
 * Ta funkcja ustawia tekst etykiet wyświetlających prędkość silników 1 i 2.
 */
void MainWindow::updateMotorSpeed(){
    ui->motor_speed_1_Label->setText(QString::number(motor1_speed) +"%");
    ui->motor_speed_2_Label->setText(QString::number(motor2_speed) +"%");
}


/**
 * @brief Aktualizuje rozmiar i widoczność strzałki silnika 1.
 *
 * Zmienia rozmiar strzałki w zależności od prędkości silnika 1. Strzałka jest również
 * obracana, jeśli prędkość jest ujemna.
 *
 * @note Strzałka jest widoczna tylko, gdy prędkość silnika jest różna od zera.
 */
void MainWindow::updateArrowSize1(){
    if (motor1_speed == 0) {
        ui->motor1_arrow->setVisible(false);
        return;
    }

    QSize maxSize = ui->motor1_arrow->size();

    qreal scaleFactor = 0.3 + (qAbs(motor1_speed) / 100.0) * 0.7;

    QSize scaledSize(maxSize.width() * scaleFactor, maxSize.height() * scaleFactor);

    QPixmap scaledPixmap = originalArrowPixmap.scaled(scaledSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    if (motor1_speed < 0) {
        QTransform transform;
        transform.rotate(180);
        scaledPixmap = scaledPixmap.transformed(transform, Qt::SmoothTransformation);
    }

    // Umieść w QLabel
    ui->motor1_arrow->setPixmap(scaledPixmap);
    ui->motor1_arrow->setAlignment(Qt::AlignCenter);  // wypośrodkuj
    ui->motor1_arrow->setVisible(true);
}


/**
 * @brief Aktualizuje rozmiar i widoczność strzałki silnika 2.
 *
 * Zmienia rozmiar strzałki w zależności od prędkości silnika 2. Strzałka jest również
 * obracana, jeśli prędkość jest ujemna.
 *
 * @note Strzałka jest widoczna tylko, gdy prędkość silnika jest różna od zera.
 */
void MainWindow::updateArrowSize2(){
    if (motor2_speed == 0) {
        ui->motor2_arrow->setVisible(false);
        return;
    }
    QSize maxSize = ui->motor2_arrow->size();

    qreal scaleFactor = 0.3 + (qAbs(motor2_speed) / 100.0) * 0.7;

    QSize scaledSize(maxSize.width() * scaleFactor, maxSize.height() * scaleFactor);

    QPixmap scaledPixmap = originalArrowPixmap.scaled(scaledSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    if (motor2_speed < 0) {
        QTransform transform;
        transform.rotate(180);
        scaledPixmap = scaledPixmap.transformed(transform, Qt::SmoothTransformation);
    }

    // Umieść w QLabel
    ui->motor2_arrow->setPixmap(scaledPixmap);
    ui->motor2_arrow->setAlignment(Qt::AlignCenter);  // wypośrodkuj
    ui->motor2_arrow->setVisible(true);
}

/**
 * @brief Aktualizuje strzałkę IMU na podstawie danych z akcelerometru.
 *
 * Strzałka jest rotowana zgodnie z kątem obliczonym na podstawie danych IMU oraz
 * przeskalowywana na podstawie długości wektora z akcelerometru.
 *
 * @note Strzałka jest widoczna tylko, gdy wartości imuX lub imuY są różne od zera.
 */
void MainWindow::updateArrowIMU(){

    if (imuX == 0 && imuY == 0) {
        ui->imu_arrow->setVisible(false);
        return;
    }
    qreal length = sqrt(imuX * imuX + imuY * imuY);

    qreal angle = atan2(imuY, imuX) * 180 / M_PI;  // Przekształcenie z radianów na stopnie

    QSize maxSize = ui->imu_arrow->size();

    qreal scaleFactor = 0.10 + (length / 100.0) * 0.70;

    QSize scaledSize(maxSize.width() * scaleFactor, maxSize.height() * scaleFactor);

    QPixmap scaledPixmap = originalArrowIMU.scaled(scaledSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    /**
 * @brief Aktualizuje zmienne na podstawie danych z pliku CSV.
 *
 * Funkcja ta odczytuje dane z wiersza o zadanym indeksie `row` z pliku CSV, przetwarza je i przypisuje do odpowiednich zmiennych.
 * Zakłada się, że wiersz zawiera dane dotyczące stanu ToF, stanu linii, prędkości silników oraz wartości IMU (akcelerometr i żyroskop).
 * Następnie, wywołuje funkcję do aktualizacji wykresów i interfejsu użytkownika.
 *
 * @param row Indeks wiersza w pliku CSV, z którego będą pobierane dane.
 *
 * @note Funkcja wykorzystuje struktury danych `csvData` i zakłada, że wiersze mają co najmniej 12 kolumn.
 */
    QTransform transform;
    transform.rotate(angle);

    scaledPixmap = scaledPixmap.transformed(transform, Qt::SmoothTransformation);

    // Umieść w QLabel
    ui->imu_arrow->setPixmap(scaledPixmap);
    ui->imu_arrow->setAlignment(Qt::AlignCenter);  // wypośrodkuj

    ui->imu_arrow->setVisible(true);
}


void MainWindow::on_line1_button_clicked()
{
    islineS_1_Active = !islineS_1_Active;  // Zmieniamy stan
    updateVisibility();
}


void MainWindow::on_tof1_button_clicked()
{
    isTof_1 = !isTof_1;  // Zmieniamy stan
    updateVisibility();   // Aktualizujemy widoczność
}


void MainWindow::on_dial_valueChanged(int value)
{
    motor1_speed = value;
    updateMotorSpeed();
    updateArrowSize1();
}


void MainWindow::on_dial_2_valueChanged(int value)
{
    motor2_speed = value;
    updateMotorSpeed();
    updateArrowSize2();
}

void MainWindow::on_x_dial_valueChanged(int value)
{
    imuX = value;
    updateArrowIMU();
}


void MainWindow::on_y_dial_valueChanged(int value)
{
    imuY = value;
    updateArrowIMU();
}


/**
 * @brief Funkcja wywoływana po załadowaniu pliku CSV.
 *
 * Otwiera plik CSV, wczytuje dane i aktualizuje stan aplikacji na podstawie
 * danych zawartych w pliku.
 *
 * @param filePath Ścieżka do pliku CSV.
 */
void MainWindow::loadSettingsFromCSV(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Nie udało się otworzyć pliku CSV";
        return;
    }

    // Wyczyść poprzednie dane
    csvData.clear();
    currentRow = 0;

    clearCharts();///< Wyczyść wykresy przed załadowaniem nowych danych

    QTextStream in(&file);
    QString line;
    bool firstLine = true;

    // Wczytywanie danych do csvData
    while (!in.atEnd()) {
        line = in.readLine();
        if (firstLine) {
            firstLine = false;
            continue;  // Pomijamy pierwszą linię z nagłówkami
        }
        QStringList data = line.split(",");
        // Walidacja danych
        if (data.size() >= 12) { // Upewnij się, że mamy wszystkie wymagane kolumny
            csvData.append(data); ///< Dodajemy dane do csvData
        }
    }

    file.close();

    // Resetuj zmienne czasowe
    playbackTimer.invalidate();
}


/**
 * @brief Aktualizuje zmienne na podstawie danych z pliku CSV.
 *
 * Funkcja ta odczytuje dane z wiersza o zadanym indeksie `row` z pliku CSV, przetwarza je i przypisuje do odpowiednich zmiennych.
 * Zakłada się, że wiersz zawiera dane dotyczące stanu ToF, stanu linii, prędkości silników oraz wartości IMU (akcelerometr i żyroskop).
 * Następnie, wywołuje funkcję do aktualizacji wykresów i interfejsu użytkownika.
 *
 * @param row Indeks wiersza w pliku CSV, z którego będą pobierane dane.
 *
 * @note Funkcja wykorzystuje struktury danych `csvData` i zakłada, że wiersze mają co najmniej 12 kolumn.
 */
void MainWindow::updateVariablesFromCSV(int row)
{
    if (row < 0 || row >= csvData.size() || csvData[row].size() < 12) {
        qWarning() << "Nieprawidłowe dane w wierszu o indeksie" << row;
        return;
    }

    QStringList data = csvData[row];
    int time = data[0].toInt();

    try {
        isTof_1 = (data[1].toInt() == 1);  // 1 -> true, 0 -> false
        isTof_2 = (data[2].toInt() == 1);  // 1 -> true, 0 -> false
        isTof_3 = (data[3].toInt() == 1);  // 1 -> true, 0 -> false
        isTof_4 = (data[4].toInt() == 1);  // 1 -> true, 0 -> false
        islineS_1_Active = (data[5].toInt() == 1);  // 1 -> true, 0 -> false
        islineS_2_Active = (data[6].toInt() == 1);  // 1 -> true, 0 -> false
        islineS_3_Active = (data[7].toInt() == 1);  // 1 -> true, 0 -> false
        motor1_speed = data[8].toInt();  // Przypisanie prędkości silnika
        motor2_speed = data[9].toInt();  // Przypisanie prędkości silnika
        imuX = data[10].toFloat();
        imuY = data[11].toFloat();


        updateCharts(time);
        updateAll();
    } catch (...) {
        qWarning() << "Błąd przetwarzania wiersza" << row;
    }
}


/**
 * @brief Rozpoczyna aktualizację danych z pliku CSV przy użyciu timera.
 *
 * Funkcja inicjuje odtwarzanie danych z pliku CSV, ustawia odpowiedni timer (`dataUpdateTimer`),
 * który będzie co 20 ms wywoływał funkcję `updateData` do odczytu danych z CSV.
 * Timer odtwarzania (`playbackTimer`) jest również uruchamiany.
 */
void MainWindow::startDataUpdateTimer()
{
    currentRow = 0;

    // Rozpocznij pomiar czasu dla odtwarzania
    playbackTimer.start();

    // Zatrzymaj istniejący timer, jeśli istnieje
    if (dataUpdateTimer) {
        dataUpdateTimer->stop();
        delete dataUpdateTimer;
    }


    dataUpdateTimer = new QTimer(this);
    connect(dataUpdateTimer, &QTimer::timeout, this, &MainWindow::updateData);
    dataUpdateTimer->start(20);
}


/**
 * @brief Aktualizuje dane w interfejsie użytkownika na podstawie czasu od rozpoczęcia odtwarzania.
 *
 * Funkcja ta oblicza, ile czasu minęło od rozpoczęcia odtwarzania danych, porównuje go z czasem zapisanym w wierszach CSV
 * i na tej podstawie aktualizuje zmienne i interfejs użytkownika. Jeśli odtwarzanie danych osiągnie koniec, timer jest zatrzymywany.
 */
void MainWindow::updateData()
{
    // Jeśli nie mamy danych lub timer nie jest aktywny, wyjdź
    if (csvData.isEmpty() || !playbackTimer.isValid()) {
        return;
    }

    // Oblicz aktualny czas od początku odtwarzania
    qint64 elapsedMs = playbackTimer.elapsed();

    while (currentRow < csvData.size()) {
        int timeInCSV = csvData[currentRow][0].toInt();

        if (elapsedMs >= timeInCSV) {
            updateVariablesFromCSV(currentRow);
            currentRow++;
        } else {
            break; // Czekaj na następne wywołanie timera
        }
    }

    // Sprawdź, czy odtwarzanie zakończone
    if (currentRow >= csvData.size()) {
        dataUpdateTimer->stop();
        qDebug() << "Odtwarzanie zakończone";
    }
}

/**
 * @brief Funkcja ustawiająca przezroczystość dla strzałki.
 *
 * Ustawia przezroczystość dla wskazanego QLabel, aby uzyskać efekt przezroczystości.
 *
 * @param arrowLabel Wskaźnik do QLabel, którego przezroczystość ma być zmieniona.
 */
void MainWindow::setArrowOpacity(QLabel* arrowLabel) {

    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(this);
    effect->setOpacity(0.7);  // przezroczystość na 70%

    arrowLabel->setGraphicsEffect(effect);
}



/**
 * @brief Obsługuje kliknięcie przycisku "Załaduj" w celu załadowania pliku CSV.
 *
 * Funkcja otwiera dialog wyboru pliku, w którym użytkownik może wybrać plik CSV.
 * Po wybraniu pliku, wczytuje ustawienia z tego pliku i uruchamia timer do aktualizacji danych.
 *
 * @note Funkcja obsługuje tylko pliki CSV, ale użytkownik może również wybrać inne pliki.
 */
void MainWindow::on_load_fButton_clicked()
{
    // Otwórz dialog pliku
    QString filePath = QFileDialog::getOpenFileName(this, "Wybierz plik CSV", "", "Pliki CSV (*.csv);;Wszystkie pliki (*)");

    // Jeśli użytkownik wybrał plik (nie anulował)
    if (!filePath.isEmpty()) {
        // Wczytaj ustawienia z pliku CSV
        loadSettingsFromCSV(filePath);
        startDataUpdateTimer();
    }
}

/**
 * @brief Funkcja konfigurująca wykresy dla różnych sensorów.
 *
 * Ustawia różne wykresy dla czujników ToF, silników oraz IMU.
 */
void MainWindow::setupCharts() {
    setupTofChart();  ///< Inicjalizuje wykres ToF.
    setupLineSChart();///< Inicjalizuje wykres czujników linii.
    setupMotorsChart();///< Inicjalizuje wykres silników.
    setupImuChart();   ///< Inicjalizuje wykres IMU.
}

/**
 * @brief Konfiguruje wykres ToF dla czterech serii danych.
 *
 * Funkcja ta tworzy wykres dla czterech serii ToF, dodaje odpowiednie serie danych, ustawia zakresy osi czasu i stanu (binarny),
 * konfiguruje opcje wizualne wykresu, takie jak legenda, czcionki i marginesy. Następnie umieszcza wykres w widoku i dodaje do layoutu.
 *
 * @note Funkcja ta jest wywoływana w celu inicjalizacji wykresu, który będzie prezentować dane o czterech różnych liniach ToF,
 * które mogą reprezentować odczyty od różnych czujników w czasie.
 */
void MainWindow::setupTofChart() {
    // Tworzenie wykresu
    tofChart = new QChart();
    tofChart->setMargins(QMargins(0, 0, 0, 0));
    tofChart->setBackgroundRoundness(0);  // opcjonalnie: bez zaokrągleń tła

    // Tworzenie serii danych
    tofSeries1 = new QLineSeries();
    tofSeries2 = new QLineSeries();
    tofSeries3 = new QLineSeries();
    tofSeries4 = new QLineSeries();

    tofSeries1->setName("ToF 1");
    tofSeries2->setName("ToF 2");
    tofSeries3->setName("ToF 3");
    tofSeries4->setName("ToF 4");

    // Dodawanie serii do wykresu
    tofChart->addSeries(tofSeries1);
    tofChart->addSeries(tofSeries2);
    tofChart->addSeries(tofSeries3);
    tofChart->addSeries(tofSeries4);

    // Tworzenie osi
    tofAxisX = new QValueAxis();
    tofAxisY = new QValueAxis();

    tofAxisX->setTitleText("Czas (ms)");
    tofAxisY->setTitleText("Stan");

    tofAxisY->setRange(0, 1.1); // Dla wartości binarnych (0/1)
    tofAxisX->setRange(0, MAX_POINTS * 100); // Zakładamy dane co 100ms

    // Dodawanie osi do wykresu
    tofChart->addAxis(tofAxisX, Qt::AlignBottom);
    tofChart->addAxis(tofAxisY, Qt::AlignLeft);

    // Dołączanie osi do serii
    tofSeries1->attachAxis(tofAxisX);
    tofSeries1->attachAxis(tofAxisY);
    tofSeries2->attachAxis(tofAxisX);
    tofSeries2->attachAxis(tofAxisY);
    tofSeries3->attachAxis(tofAxisX);
    tofSeries3->attachAxis(tofAxisY);
    tofSeries4->attachAxis(tofAxisX);
    tofSeries4->attachAxis(tofAxisY);

    // Opcje
        tofChart->setMargins(QMargins(0, 0, 0, 0));
        tofChart->setBackgroundRoundness(0);  // opcjonalnie: bez zaokrągleń tła

        // Legenda: ustawienie widoczności i pozycji
        tofChart->legend()->setVisible(true);
        tofChart->legend()->setAlignment(Qt::AlignTop | Qt::AlignRight);
        tofChart->legend()->setBackgroundVisible(false);
        tofChart->legend()->setBorderColor(Qt::transparent);

        // Dostosowanie położenia legendy
        QRectF legendRect = tofChart->legend()->geometry();
        legendRect.moveTopRight(tofChart->plotArea().topRight() - QPointF(10, 10));
        tofChart->legend()->setGeometry(legendRect);

        // Zmniejsz czcionkę legendy
        QFont legendFont = tofChart->legend()->font();
        legendFont.setPointSize(8); // Zmniejsz rozmiar czcionki
        tofChart->legend()->setFont(legendFont);

        // Wartości osi
        QFont axisFont;
        axisFont.setPointSize(8);

        tofAxisX->setLabelsFont(axisFont);
        tofAxisY->setLabelsFont(axisFont);
        tofAxisX->setTitleVisible(false);
        tofAxisY->setTitleVisible(false);

    // Tworzenie widoku wykresu
    tofChartView = new QChartView(tofChart);
    tofChartView->setRenderHint(QPainter::Antialiasing);

    // Dodawanie do layoutu widgetu
    QVBoxLayout *layout = new QVBoxLayout(ui->tof_widget);
    layout->addWidget(tofChartView);
    layout->setContentsMargins(0, 0, 0, 0); // usuwa odstępy wokół
    layout->setSpacing(0);
    ui->tof_widget->setLayout(layout);
}

/**
 * @brief Konfiguruje wykres dla trzech linii stanów z osiami i seriami danych.
 *
 * Funkcja ta tworzy wykres dla trzech linii stanów, dodaje serie danych, ustawia zakresy osi czasu oraz stanu (binarny),
 * konfiguruje opcje wizualne wykresu, takie jak legenda, czcionki i marginesy. Następnie umieszcza wykres w widoku i dodaje do layoutu.
 *
 * @note Funkcja ta jest wywoływana w celu inicjalizacji wykresu, który będzie prezentować dane o stanach trzech linii
 * na przestrzeni czasu. Oś Y jest ustawiona na wartości binarne (0/1), co może wskazywać na aktywność lub inny stan.
 */
void MainWindow::setupLineSChart() {
    // Tworzenie wykresu
    lineSChart = new QChart();
    lineSChart->setMargins(QMargins(0, 0, 0, 0));
    lineSChart->setBackgroundRoundness(0);  // opcjonalnie: bez zaokrągleń tła

    // Tworzenie serii danych
    lineSeries1 = new QLineSeries();
    lineSeries2 = new QLineSeries();
    lineSeries3 = new QLineSeries();

    lineSeries1->setName("Linia 1");
    lineSeries2->setName("Linia 2");
    lineSeries3->setName("Linia 3");

    // Dodawanie serii do wykresu
    lineSChart->addSeries(lineSeries1);
    lineSChart->addSeries(lineSeries2);
    lineSChart->addSeries(lineSeries3);

    // Tworzenie osi
    lineAxisX = new QValueAxis();
    lineAxisY = new QValueAxis();

    lineAxisX->setTitleText("Czas (ms)");
    lineAxisY->setTitleText("Stan");

    lineAxisY->setRange(0, 1.1); // Dla wartości binarnych (0/1)
    lineAxisX->setRange(0, MAX_POINTS * 100); // Zakładamy dane co 100ms

    // Dodawanie osi do wykresu
    lineSChart->addAxis(lineAxisX, Qt::AlignBottom);
    lineSChart->addAxis(lineAxisY, Qt::AlignLeft);

    // Dołączanie osi do serii
    lineSeries1->attachAxis(lineAxisX);
    lineSeries1->attachAxis(lineAxisY);
    lineSeries2->attachAxis(lineAxisX);
    lineSeries2->attachAxis(lineAxisY);
    lineSeries3->attachAxis(lineAxisX);
    lineSeries3->attachAxis(lineAxisY);

    // Opcje
        lineSChart->setMargins(QMargins(0, 0, 0, 0));
        lineSChart->setBackgroundRoundness(0);

        // Legenda: ustawienie widoczności i pozycji
        lineSChart->legend()->setVisible(true);
        lineSChart->legend()->setAlignment(Qt::AlignTop | Qt::AlignRight);
        lineSChart->legend()->setBackgroundVisible(false);
        lineSChart->legend()->setBorderColor(Qt::transparent);

        // Dostosowanie położenia legendy
        QRectF legendRect = lineSChart->legend()->geometry();
        legendRect.moveTopRight(lineSChart->plotArea().topRight() - QPointF(10, 10));
        lineSChart->legend()->setGeometry(legendRect);

        // Zmniejsz czcionkę legendy
        QFont legendFont = lineSChart->legend()->font();
        legendFont.setPointSize(8);
        lineSChart->legend()->setFont(legendFont);

        // Wartości osi
        QFont axisFont;
        axisFont.setPointSize(5);

        lineAxisX->setLabelsFont(axisFont);
        lineAxisY->setLabelsFont(axisFont);
        lineAxisX->setTitleVisible(false);
        lineAxisY->setTitleVisible(false);

    // Tworzenie widoku wykresu
    lineChartView = new QChartView(lineSChart);
    lineChartView->setRenderHint(QPainter::Antialiasing);

    // Dodawanie do layoutu widgetu
    QVBoxLayout *layout = new QVBoxLayout(ui->lineS_widget);
    layout->addWidget(lineChartView);
    layout->setContentsMargins(0, 0, 0, 0); // usuwa odstępy wokół
    layout->setSpacing(0);
    ui->lineS_widget->setLayout(layout);
}


/**
 * @brief Konfiguruje wykres dla prędkości silników z osiami i seriami danych.
 *
 * Funkcja ta tworzy wykres dla dwóch silników, dodaje serie danych dla prędkości obu silników,
 * ustawia zakresy osi czasu i prędkości oraz konfiguruje opcje wizualne wykresu, takie jak legenda, czcionki
 * i marginesy. Następnie umieszcza wykres w widoku i dodaje do layoutu.
 *
 * @note Funkcja ta jest wywoływana w celu inicjalizacji wykresu prędkości silników, który będzie prezentować dane
 * o prędkości obu silników na przestrzeni czasu.
 */
void MainWindow::setupMotorsChart() {
    // Tworzenie wykresu
    motorsChart = new QChart();

    // Tworzenie serii danych
    motorSeries1 = new QLineSeries();
    motorSeries2 = new QLineSeries();

    motorSeries1->setName("Silnik 1");
    motorSeries2->setName("Silnik 2");

    // Dodawanie serii do wykresu
    motorsChart->addSeries(motorSeries1);
    motorsChart->addSeries(motorSeries2);
    motorsChart->legend()->setVisible(false);
    motorsChart->legend()->setAlignment(Qt::AlignTop);
    // Tworzenie osi
    motorAxisX = new QValueAxis();
    motorAxisY = new QValueAxis();

    motorAxisX->setTitleText("Czas (ms)");
    motorAxisY->setTitleText("Prędkość (%)");

    motorAxisY->setRange(-100, 100); // Zakres prędkości od -100 do 100
    motorAxisX->setRange(0, MAX_POINTS * 100); // Zakładamy dane co 100ms

    // Dodawanie osi do wykresu
    motorsChart->addAxis(motorAxisX, Qt::AlignBottom);
    motorsChart->addAxis(motorAxisY, Qt::AlignLeft);

    // Dołączanie osi do serii
    motorSeries1->attachAxis(motorAxisX);
    motorSeries1->attachAxis(motorAxisY);
    motorSeries2->attachAxis(motorAxisX);
    motorSeries2->attachAxis(motorAxisY);

    // Tworzenie widoku wykresu
    motorChartView = new QChartView(motorsChart);
    motorChartView->setRenderHint(QPainter::Antialiasing);

    //OPCJE

        //chart
        motorsChart->setMargins(QMargins(0, 0, 0, 0));
        motorsChart->setBackgroundRoundness(0);

        //Legenda
        // Legenda: ustawienie widoczności i pozycji
        motorsChart->legend()->setVisible(true);
        motorsChart->legend()->setAlignment(Qt::AlignTop | Qt::AlignRight);
        motorsChart->legend()->setBackgroundVisible(false); // opcjonalnie: bez tła
        motorsChart->legend()->setBorderColor(Qt::transparent); // bez ramki

        // Dostosowanie położenia legendy
        QRectF legendRect = motorsChart->legend()->geometry();
        legendRect.moveTopRight(motorsChart->plotArea().topRight() - QPointF(10, 10));
        motorsChart->legend()->setGeometry(legendRect);

        // Zmniejsz czcionkę legendy
        QFont legendFont = motorsChart->legend()->font();
        legendFont.setPointSize(8);
        motorsChart->legend()->setFont(legendFont);

        //wartosi osi
        QFont axisFont;
        axisFont.setPointSize(8);

        motorAxisX->setLabelsFont(axisFont);
        motorAxisY->setLabelsFont(axisFont);
        motorAxisX->setTitleVisible(false);
        motorAxisY->setTitleVisible(false);



    // Dodawanie do layoutu widgetu
    QVBoxLayout *layout = new QVBoxLayout(ui->motors_widget);
    layout->addWidget(motorChartView);
    layout->setContentsMargins(0, 0, 0, 0); // usuwa odstępy wokół
    layout->setSpacing(0);
    ui->motors_widget->setLayout(layout);
}

/**
 * @brief Konfiguruje wykres IMU z osiami i seriami danych.
 *
 * Funkcja ta tworzy wykres IMU, dodaje serie danych dla osi X i Y, ustawia zakresy osi oraz konfiguruje
 * opcje wizualne wykresu, takie jak legenda, czcionki i marginesy. Następnie umieszcza wykres w widoku
 * i dodaje do layoutu.
 *
 * @note Funkcja ta jest wywoływana w celu inicjalizacji wykresu IMU, który będzie prezentować dane
 * z czujników IMU na dwóch osiach (X i Y).
 */
void MainWindow::setupImuChart() {
    // Tworzenie wykresu
    imuChart = new QChart();

    // Tworzenie serii danych
    imuSeriesX = new QLineSeries();
    imuSeriesY = new QLineSeries();

    imuSeriesX->setName("IMU X");
    imuSeriesY->setName("IMU Y");

    // Dodawanie serii do wykresu
    imuChart->addSeries(imuSeriesX);
    imuChart->addSeries(imuSeriesY);

    // Tworzenie osi
    imuAxisX = new QValueAxis();
    imuAxisY = new QValueAxis();


    imuAxisY->setRange(-100, 100); // Zakres wartości IMU
    imuAxisX->setRange(0, MAX_POINTS * 100); // Zakładamy dane co 100ms

    // Dodawanie osi do wykresu
    imuChart->addAxis(imuAxisX, Qt::AlignBottom);
    imuChart->addAxis(imuAxisY, Qt::AlignLeft);

    // Dołączanie osi do serii
    imuSeriesX->attachAxis(imuAxisX);
    imuSeriesX->attachAxis(imuAxisY);
    imuSeriesY->attachAxis(imuAxisX);
    imuSeriesY->attachAxis(imuAxisY);

    //OPCJE

        //chart
        imuChart->setMargins(QMargins(0, 0, 0, 0));
        imuChart->setBackgroundRoundness(0);

        //Legenda
        // Legenda: ustawienie widoczności i pozycji
        imuChart->legend()->setVisible(true);
        imuChart->legend()->setAlignment(Qt::AlignTop | Qt::AlignRight);
        imuChart->legend()->setBackgroundVisible(false);
        imuChart->legend()->setBorderColor(Qt::transparent);

        // Dostosowanie położenia legendy
        QRectF legendRect = imuChart->legend()->geometry();
        legendRect.moveTopRight(imuChart->plotArea().topRight() - QPointF(10, 10));
        imuChart->legend()->setGeometry(legendRect);

        // Zmniejsz czcionkę legendy
        QFont legendFont = imuChart->legend()->font();
        legendFont.setPointSize(8);
        imuChart->legend()->setFont(legendFont);

        //wartosi osi
        QFont axisFont;
        axisFont.setPointSize(8);

        imuAxisX->setLabelsFont(axisFont);
        imuAxisY->setLabelsFont(axisFont);
        imuAxisX->setTitleVisible(false);
        imuAxisY->setTitleVisible(false);




    // Tworzenie widoku wykresu
    imuChartView = new QChartView(imuChart);
    imuChartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //imuChartView->setRenderHint(QPainter::Antialiasing);
    //Dodawanie do layoutu widgetu
    QVBoxLayout *layout = new QVBoxLayout(ui->imu_widget);
    layout->addWidget(imuChartView);
    layout->setContentsMargins(0, 0, 0, 0); // usuwa odstępy wokół
    layout->setSpacing(0);
    ui->imu_widget->setLayout(layout);
}

/**
 * @brief Aktualizuje wykresy nowymi danymi.
 *
 * Funkcja ta dodaje nowe punkty do odpowiednich serii danych wykresów na podstawie
 * bieżącego czasu oraz wartości odpowiednich zmiennych. Dodatkowo, po dodaniu punktów,
 * sprawdza, czy liczba punktów przekroczyła dozwolony limit i odpowiednio przesuwa zakres osi X.
 *
 * @param time Czas w milisekundach, który jest używany jako wartość osi X dla nowych punktów.
 *
 * @note Funkcja ta jest wywoływana cyklicznie w celu aktualizacji wykresów z nowymi danymi.
 */
void MainWindow::updateCharts(int time) {
    // Dodanie nowych punktów do wykresów
    tofSeries1->append(time, isTof_1 ? 1 : 0);
    tofSeries2->append(time, isTof_2 ? 1 : 0);
    tofSeries3->append(time, isTof_3 ? 1 : 0);
    tofSeries4->append(time, isTof_4 ? 1 : 0);

    lineSeries1->append(time, islineS_1_Active ? 1 : 0);
    lineSeries2->append(time, islineS_2_Active ? 1 : 0);
    lineSeries3->append(time, islineS_3_Active ? 1 : 0);

    motorSeries1->append(time, motor1_speed);
    motorSeries2->append(time, motor2_speed);

    imuSeriesX->append(time, imuX);
    imuSeriesY->append(time, imuY);

    currentPoint++;

    // Przesuwanie okna wykresu, gdy zbyt dużo punktów
    if (currentPoint >= MAX_POINTS) {
        tofAxisX->setRange(time - MAX_POINTS * 100, time);
        lineAxisX->setRange(time - MAX_POINTS * 100, time);
        motorAxisX->setRange(time - MAX_POINTS * 100, time);
        imuAxisX->setRange(time - MAX_POINTS * 100, time);
    }
}

/**
 * @brief Czyści wszystkie serie danych wykresów i resetuje osie.
 *
 * Funkcja ta usuwa wszystkie dane z serii wykresów oraz resetuje zakresy osi wykresów
 * związanych z danymi z sensorów Tof, linii, silników i IMU. Dodatkowo, resetuje
 * wskaźnik `currentPoint` do początkowej wartości.
 *
 * @note Funkcja ta jest przydatna w przypadku potrzeby zresetowania wykresów przed
 * ponownym załadowaniem nowych danych.
 */
void MainWindow::clearCharts() {
    tofSeries1->clear();
    tofSeries2->clear();
    tofSeries3->clear();
    tofSeries4->clear();

    lineSeries1->clear();
    lineSeries2->clear();
    lineSeries3->clear();

    motorSeries1->clear();
    motorSeries2->clear();

    imuSeriesX->clear();
    imuSeriesY->clear();

    currentPoint = 0;

    // Resetowanie zakresów osi
    tofAxisX->setRange(0, MAX_POINTS * 100);
    lineAxisX->setRange(0, MAX_POINTS * 100);
    motorAxisX->setRange(0, MAX_POINTS * 100);
    imuAxisX->setRange(0, MAX_POINTS * 100);
}
