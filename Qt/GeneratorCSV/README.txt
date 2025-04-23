GENERATOR DANYCH CSV DLA SYMULATORA ROBOTA
=========================================

Program służy do generowania plików CSV z danymi symulującymi odczyty sensorów i sterowanie silnikami robota.

KOMPILACJA
---------
Aby skompilować program, użyj następującej komendy:

g++ -std=c++11 csv_generator.cpp -o csv_generator

SPOSÓB UŻYCIA
------------
Program uruchamiamy z linii komend z trzema parametrami:

./csv_generator <przedział_czasu_ms> <liczba_wierszy> <nazwa_pliku.csv>

gdzie:
- <przedział_czasu_ms> - interwał czasu w milisekundach między kolejnymi wierszami danych
- <liczba_wierszy> - liczba wierszy danych do wygenerowania
- <nazwa_pliku.csv> - nazwa pliku wyjściowego CSV

PRZYKŁADY UŻYCIA
--------------
1. Generowanie 100 wierszy danych z interwałem 100ms:
   ./csv_generator 100 100 dane_100ms.csv

2. Generowanie 1000 wierszy danych z interwałem 50ms:
   ./csv_generator 50 1000 dane_50ms.csv

3. Generowanie 500 wierszy danych z interwałem 200ms:
   ./csv_generator 200 500 dane_200ms.csv

FORMAT WYJŚCIOWY
--------------
Wygenerowany plik CSV zawiera następujące kolumny:
1. czas - czas w milisekundach
2. isTof_1 - stan czujnika ToF 1 (0 lub 1)
3. isTof_2 - stan czujnika ToF 2 (0 lub 1)
4. isTof_3 - stan czujnika ToF 3 (0 lub 1)
5. isTof_4 - stan czujnika ToF 4 (0 lub 1)
6. islineS_1_Active - stan czujnika linii 1 (0 lub 1)
7. islineS_2_Active - stan czujnika linii 2 (0 lub 1)
8. islineS_3_Active - stan czujnika linii 3 (0 lub 1)
9. motor1_speed - prędkość silnika 1 (od -100 do 100)
10. motor2_speed - prędkość silnika 2 (od -100 do 100)
11. imuX - odczyt IMU w osi X (float od -100.0 do 100.0)
12. imuY - odczyt IMU w osi Y (float od -100.0 do 100.0)

UWAGI
-----
- Wszystkie wartości są generowane losowo w określonych zakresach
- Wartości IMU są zapisywane z dokładnością do dwóch miejsc po przecinku
- Pierwszy wiersz zawiera nagłówki kolumn
