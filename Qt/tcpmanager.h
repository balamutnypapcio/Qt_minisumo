#ifndef TCPMANAGER_H
#define TCPMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QString>
#include <QOverload>
#include "sensordata.h"

/**
 * @class TCPManager
 * @brief Zarządza połączeniem TCP z urządzeniem ESP i odbiorem danych sensorów.
 *
 * Klasa odpowiada za:
 *  - Nawiązanie i rozłączanie połączenia TCP z urządzeniem (np. ESP32).
 *  - Odbiór, buforowanie i parsowanie danych przesyłanych przez ESP w formacie CSV.
 *  - Aktualizację obiektu SensorData na podstawie odebranych danych.
 *  - Emisję sygnałów Qt o aktualizacji danych, zmianie statusu połączenia oraz błędach sieciowych.
 *
 * Przykład użycia:
 * @code
 * TCPManager* tcpMgr = new TCPManager(sensorData, this);
 * connect(tcpMgr, &TCPManager::dataUpdated, this, &MainWindow::onDataUpdated);
 * tcpMgr->connectToDevice("192.168.4.1", 8888);
 * @endcode
 *
 * @author Jakub Wilczyński
 */
class TCPManager : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Konstruktor klasy TCPManager.
     * @param sensorData Wskaźnik do obiektu SensorData.
     * @param parent Opcjonalny rodzic QObject.
     */
    explicit TCPManager(SensorData* sensorData, QObject *parent = nullptr);

    /**
     * @brief Destruktor. Zamyka gniazdo TCP.
     */
    ~TCPManager();

    /**
     * @brief Nawiązuje połączenie z urządzeniem ESP.
     * @param hostAddress Adres IP urządzenia ESP.
     * @param port Port, na którym nasłuchuje urządzenie ESP.
     * @return true jeśli próba połączenia została zainicjowana.
     *
     * Połączenie inicjowane jest asynchronicznie.
     */
    bool connectToDevice(const QString &hostAddress, quint16 port);

    /**
     * @brief Rozłącza połączenie z urządzeniem ESP.
     */
    void disconnectFromDevice();

    /**
     * @brief Sprawdza stan połączenia.
     * @return true jeśli połączenie jest aktywne.
     */
    bool isConnected() const;

signals:
    /**
     * @brief Sygnał informujący o aktualizacji danych.
     * @param time Znacznik czasu aktualizacji.
     */
    void dataUpdated(int time);

    /**
     * @brief Sygnał emitowany po zmianie stanu połączenia.
     * @param connected Stan połączenia (true - połączony, false - rozłączony).
     */
    void connectionStatusChanged(bool connected);

    /**
     * @brief Sygnał emitowany w przypadku wystąpienia błędu.
     * @param errorMessage Komunikat o błędzie.
     */
    void errorOccurred(const QString &errorMessage);

private slots:
    /**
     * @brief Slot wywoływany, gdy dostępne są nowe dane do odczytu.
     */
    void onReadyRead();

    /**
     * @brief Slot wywoływany przy zmianie stanu gniazda.
     * @param state Nowy stan gniazda.
     */
    void onSocketStateChanged(QAbstractSocket::SocketState state);

    /**
     * @brief Slot wywoływany przy wystąpieniu błędu gniazda.
     * @param error Kod błędu.
     */
    void onSocketError(QAbstractSocket::SocketError error);

private:
    /**
     * @brief Przetwarza linię danych otrzymaną z ESP.
     * @param line Linia danych do przetworzenia.
     *
     * Oczekiwany format: czas,tof1,tof2,tof3,tof4,lineS1,lineS2,lineS3,motor1,motor2,imuX,imuY
     */
    void processDataLine(const QString &line);

    SensorData* m_sensorData;     ///< Wskaźnik do danych sensorów.
    QTcpSocket* m_socket;         ///< Gniazdo TCP do komunikacji z ESP.
    QString m_buffer;             ///< Bufor danych przychodzących.
    int m_timestamp;              ///< Ostatni znacznik czasu.
};

#endif // TCPMANAGER_H
