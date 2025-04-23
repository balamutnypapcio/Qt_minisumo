#include "tcpmanager.h"
#include <QDebug>

TCPManager::TCPManager(SensorData* sensorData, QObject *parent)
    : QObject(parent), m_sensorData(sensorData), m_socket(nullptr), m_timestamp(0)
{
    // Inicjalizacja gniazda TCP
    m_socket = new QTcpSocket(this);

    // Podłączenie sygnałów gniazda
    connect(m_socket, &QTcpSocket::readyRead, this, &TCPManager::onReadyRead);
    connect(m_socket, &QTcpSocket::stateChanged, this, &TCPManager::onSocketStateChanged);
    connect(m_socket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::errorOccurred),
            this, &TCPManager::onSocketError);

}

TCPManager::~TCPManager()
{
    // Zamknięcie połączenia przy niszczeniu obiektu
    if (m_socket) {
        m_socket->close();
        delete m_socket;
    }
}

bool TCPManager::connectToDevice(const QString &hostAddress, quint16 port)
{
    // Rozłączenie, jeśli istnieje aktywne połączenie
    if (m_socket->state() != QAbstractSocket::UnconnectedState) {
        m_socket->close();
    }

    // Próba połączenia z urządzeniem ESP
    m_socket->connectToHost(hostAddress, port);

    // Zwraca true jeśli udało się zainicjować połączenie
    // (samo połączenie jest asynchroniczne)
    qDebug() << "Próba połączenia z ESP na" << hostAddress << ":" << port;
    return true;
}

void TCPManager::disconnectFromDevice()
{
    // Rozłączenie z urządzeniem
    m_socket->close();
    qDebug() << "Rozłączono z ESP";
}

bool TCPManager::isConnected() const
{
    // Sprawdzenie stanu połączenia
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

void TCPManager::onReadyRead()
{
    // Odczyt dostępnych danych
    QByteArray data = m_socket->readAll();
    m_buffer += QString::fromUtf8(data);

    // Przetworzenie kompletnych linii w buforze
    int newlinePos;
    while ((newlinePos = m_buffer.indexOf('\n')) != -1) {
        QString line = m_buffer.left(newlinePos).trimmed();
        m_buffer = m_buffer.mid(newlinePos + 1);

        // Przetwarzanie niepustych linii
        if (!line.isEmpty()) {
            processDataLine(line);
        }
    }
}

void TCPManager::processDataLine(const QString &line)
{
    // Parsowanie linii (oczekiwany format: czas,tof1,tof2,tof3,tof4,lineS1,lineS2,lineS3,motor1,motor2,imuX,imuY)
    QStringList data = line.split(',');

    // Walidacja formatu danych
    if (data.size() < 12) {
        qWarning() << "Nieprawidłowy format danych:" << line;
        return;
    }

    try {
        // Parsowanie danych
        m_timestamp = data[0].toInt();
        bool tof1 = (data[1].toInt() == 1);
        bool tof2 = (data[2].toInt() == 1);
        bool tof3 = (data[3].toInt() == 1);
        bool tof4 = (data[4].toInt() == 1);
        bool lineS1 = (data[5].toInt() == 1);
        bool lineS2 = (data[6].toInt() == 1);
        bool lineS3 = (data[7].toInt() == 1);
        int motor1 = data[8].toInt();
        int motor2 = data[9].toInt();
        float imuX = data[10].toFloat();
        float imuY = data[11].toFloat();

        // Aktualizacja danych sensorów
        m_sensorData->updateAllData(tof1, tof2, tof3, tof4, lineS1, lineS2, lineS3, motor1, motor2, imuX, imuY);

        // Emitowanie sygnału o aktualizacji danych
        emit dataUpdated(m_timestamp);
    } catch (...) {
        qWarning() << "Błąd przetwarzania linii danych:" << line;
    }
}

void TCPManager::onSocketStateChanged(QAbstractSocket::SocketState state)
{
    // Obsługa zmiany stanu gniazda
    switch (state) {
    case QAbstractSocket::ConnectedState:
        qDebug() << "Połączono z ESP";
        emit connectionStatusChanged(true);
        break;
    case QAbstractSocket::UnconnectedState:
        qDebug() << "Rozłączono z ESP";
        emit connectionStatusChanged(false);
        break;
    default:
        // Inne stany jak connecting, bound, itp.
        break;
    }
}

void TCPManager::onSocketError(QAbstractSocket::SocketError error)
{
    // Obsługa błędów gniazda
    QString errorMsg = "Błąd gniazda: " + m_socket->errorString();
    qWarning() << errorMsg;
    emit errorOccurred(errorMsg);
}
