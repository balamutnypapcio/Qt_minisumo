#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <ctime>

int main(int argc, char* argv[]) {
    // Sprawdzenie liczby argumentów
    if (argc != 4) {
        std::cerr << "Użycie: " << argv[0] << " <przedział_czasu_ms> <liczba_wierszy> <nazwa_pliku.csv>" << std::endl;
        return 1;
    }

    // Parsowanie argumentów
    int timeInterval = std::stoi(argv[1]); // przedział czasu w ms
    int rowCount = std::stoi(argv[2]);     // liczba wierszy do wygenerowania
    std::string fileName = argv[3];        // nazwa pliku wyjściowego

    // Inicjalizacja generatora liczb losowych
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> binary(0, 1);           // dla wartości 0 lub 1
    std::uniform_int_distribution<> motor(-100, 100);       // dla wartości silników -100 do 100
    std::uniform_real_distribution<float> imu(-100.0, 100.0); // dla wartości IMU -100.0 do 100.0

    // Otwarcie pliku
    std::ofstream outFile(fileName);
    if (!outFile) {
        std::cerr << "Błąd: Nie można otworzyć pliku " << fileName << std::endl;
        return 1;
    }

    // Nagłówek CSV
    outFile << "czas,isTof_1,isTof_2,isTof_3,isTof_4,islineS_1_Active,islineS_2_Active,islineS_3_Active,motor1_speed,motor2_speed,imuX,imuY" << std::endl;

    // Generowanie danych
    for (int i = 0; i < rowCount; i++) {
        int time = i * timeInterval;
        outFile << time << ",";                    // czas
        outFile << binary(gen) << ",";             // isTof_1
        outFile << binary(gen) << ",";             // isTof_2
        outFile << binary(gen) << ",";             // isTof_3
        outFile << binary(gen) << ",";             // isTof_4
        outFile << binary(gen) << ",";             // islineS_1_Active
        outFile << binary(gen) << ",";             // islineS_2_Active
        outFile << binary(gen) << ",";             // islineS_3_Active
        outFile << motor(gen) << ",";              // motor1_speed
        outFile << motor(gen) << ",";              // motor2_speed
        
        // Wartości IMU z dwoma miejscami po przecinku
        outFile.precision(2);
        outFile << std::fixed << imu(gen) << ",";  // imuX
        outFile << std::fixed << imu(gen);         // imuY
        
        outFile << std::endl;
    }

    outFile.close();
    std::cout << "Wygenerowano " << rowCount << " wierszy danych do pliku " << fileName << std::endl;
    std::cout << "Przedział czasu: " << timeInterval << " ms" << std::endl;

    return 0;
}
