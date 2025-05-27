#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <QObject>

/**
 * @class SensorData
 * @brief Przechowuje dane z różnych sensorów i udostępnia je aplikacji.
 */
class SensorData : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Konstruktor klasy SensorData.
     * @param parent Rodzic QObject.
     */
    explicit SensorData(QObject *parent = nullptr);

    // Gettery i settery dla danych sensorów

    /**
     * @brief Zwraca wartość czujnika TOF1.
     * @return Stan czujnika TOF1.
     */
    bool getTof1() const { return m_isTof_1; }
    /**
     * @brief Ustawia stan czujnika TOF1.
     * @param value Nowa wartość.
     */
    void setTof1(bool value);

    /**
     * @brief Zwraca wartość czujnika TOF2.
     * @return Stan czujnika TOF2.
     */
    bool getTof2() const { return m_isTof_2; }
    /**
     * @brief Ustawia stan czujnika TOF2.
     * @param value Nowa wartość.
     */
    void setTof2(bool value);

    /**
     * @brief Zwraca wartość czujnika TOF3.
     * @return Stan czujnika TOF3.
     */
    bool getTof3() const { return m_isTof_3; }
    /**
     * @brief Ustawia stan czujnika TOF3.
     * @param value Nowa wartość.
     */
    void setTof3(bool value);

    /**
     * @brief Zwraca wartość czujnika TOF4.
     * @return Stan czujnika TOF4.
     */
    bool getTof4() const { return m_isTof_4; }
    /**
     * @brief Ustawia stan czujnika TOF4.
     * @param value Nowa wartość.
     */
    void setTof4(bool value);

    /**
     * @brief Zwraca stan czujnika linii S1.
     * @return Stan czujnika linii S1.
     */
    bool getLineS1Active() const { return m_islineS_1_Active; }
    /**
     * @brief Ustawia stan czujnika linii S1.
     * @param value Nowa wartość.
     */
    void setLineS1Active(bool value);

    /**
     * @brief Zwraca stan czujnika linii S2.
     * @return Stan czujnika linii S2.
     */
    bool getLineS2Active() const { return m_islineS_2_Active; }
    /**
     * @brief Ustawia stan czujnika linii S2.
     * @param value Nowa wartość.
     */
    void setLineS2Active(bool value);

    /**
     * @brief Zwraca stan czujnika linii S3.
     * @return Stan czujnika linii S3.
     */
    bool getLineS3Active() const { return m_islineS_3_Active; }
    /**
     * @brief Ustawia stan czujnika linii S3.
     * @param value Nowa wartość.
     */
    void setLineS3Active(bool value);

    /**
     * @brief Zwraca prędkość silnika 1.
     * @return Prędkość silnika 1.
     */
    int getMotor1Speed() const { return m_motor1_speed; }
    /**
     * @brief Ustawia prędkość silnika 1.
     * @param value Nowa prędkość.
     */
    void setMotor1Speed(int value);

    /**
     * @brief Zwraca prędkość silnika 2.
     * @return Prędkość silnika 2.
     */
    int getMotor2Speed() const { return m_motor2_speed; }
    /**
     * @brief Ustawia prędkość silnika 2.
     * @param value Nowa prędkość.
     */
    void setMotor2Speed(int value);

    /**
     * @brief Zwraca wartość osi X z IMU.
     * @return Wartość osi X IMU.
     */
    float getImuX() const { return m_imuX; }
    /**
     * @brief Ustawia wartość osi X z IMU.
     * @param value Nowa wartość.
     */
    void setImuX(float value);

    /**
     * @brief Zwraca wartość osi Y z IMU.
     * @return Wartość osi Y IMU.
     */
    float getImuY() const { return m_imuY; }
    /**
     * @brief Ustawia wartość osi Y z IMU.
     * @param value Nowa wartość.
     */
    void setImuY(float value);

    /**
     * @brief Aktualizuje wszystkie dane naraz.
     * @param tof1 Stan TOF1
     * @param tof2 Stan TOF2
     * @param tof3 Stan TOF3
     * @param tof4 Stan TOF4
     * @param lineS1 Stan czujnika linii S1
     * @param lineS2 Stan czujnika linii S2
     * @param lineS3 Stan czujnika linii S3
     * @param motor1 Prędkość silnika 1
     * @param motor2 Prędkość silnika 2
     * @param imuX Wartość osi X IMU
     * @param imuY Wartość osi Y IMU
     */
    void updateAllData(bool tof1, bool tof2, bool tof3, bool tof4,
                       bool lineS1, bool lineS2, bool lineS3,
                       int motor1, int motor2,
                       float imuX, float imuY);

signals:
    /**
     * @brief Sygnał emitowany przy zmianie danych TOF.
     */
    void tofDataChanged();
    /**
     * @brief Sygnał emitowany przy zmianie danych czujników linii.
     */
    void lineSDataChanged();
    /**
     * @brief Sygnał emitowany przy zmianie danych silników.
     */
    void motorDataChanged();
    /**
     * @brief Sygnał emitowany przy zmianie danych IMU.
     */
    void imuDataChanged();
    /**
     * @brief Sygnał emitowany przy jakiejkolwiek zmianie danych.
     */
    void dataChanged();

private:
    bool m_isTof_1 = false;              ///< Stan czujnika TOF1
    bool m_isTof_2 = false;              ///< Stan czujnika TOF2
    bool m_isTof_3 = false;              ///< Stan czujnika TOF3
    bool m_isTof_4 = false;              ///< Stan czujnika TOF4

    bool m_islineS_1_Active = false;     ///< Stan czujnika linii S1
    bool m_islineS_2_Active = false;     ///< Stan czujnika linii S2
    bool m_islineS_3_Active = false;     ///< Stan czujnika linii S3

    int m_motor1_speed = 0;              ///< Prędkość silnika 1
    int m_motor2_speed = 0;              ///< Prędkość silnika 2

    float m_imuX = 0;                    ///< Wartość osi X IMU
    float m_imuY = 0;                    ///< Wartość osi Y IMU
};

#endif // SENSORDATA_H
