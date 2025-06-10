#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <QObject>

/**
 * @class SensorData
 * @brief Przechowuje dane z różnych sensorów i udostępnia je aplikacji.
 *
 * Klasa stanowi centralny bufor danych wszystkich podsystemów robota:
 *  - Czujniki TOF (Time-of-Flight)
 *  - Czujniki linii (Line Sensors)
 *  - Silniki (prędkości)
 *  - IMU (akcelerometr, żyroskop)
 *
 * Pozwala na dostęp do wszystkich danych poprzez gettery/settery oraz sygnalizuje zmiany dedykowanymi sygnałami Qt.
 *
 * Przykład użycia:
 * @code
 * SensorData* data = new SensorData();
 * connect(data, &SensorData::dataChanged, this, &MainWindow::refresh);
 * data->setMotor1Speed(50);
 * @endcode
 *
 * @author Jakub Wilczyński
 */
class SensorData : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Konstruktor klasy SensorData.
     * @param parent Rodzic QObject.
     */
    explicit SensorData(QObject *parent = nullptr);

    // --- Gettery i settery dla wszystkich sensorów i aktuatorów ---

    /** @name TOF sensors */
    ///@{
    bool getTof1() const { return m_isTof_1; }
    void setTof1(bool value);

    bool getTof2() const { return m_isTof_2; }
    void setTof2(bool value);

    bool getTof3() const { return m_isTof_3; }
    void setTof3(bool value);

    bool getTof4() const { return m_isTof_4; }
    void setTof4(bool value);
    ///@}

    /** @name Line sensors */
    ///@{
    bool getLineS1Active() const { return m_islineS_1_Active; }
    void setLineS1Active(bool value);

    bool getLineS2Active() const { return m_islineS_2_Active; }
    void setLineS2Active(bool value);

    bool getLineS3Active() const { return m_islineS_3_Active; }
    void setLineS3Active(bool value);
    ///@}

    /** @name Motor speeds */
    ///@{
    int getMotor1Speed() const { return m_motor1_speed; }
    void setMotor1Speed(int value);

    int getMotor2Speed() const { return m_motor2_speed; }
    void setMotor2Speed(int value);
    ///@}

    /** @name IMU values */
    ///@{
    float getImuX() const { return m_imuX; }
    void setImuX(float value);

    float getImuY() const { return m_imuY; }
    void setImuY(float value);
    ///@}

    /**
     * @brief Aktualizuje wszystkie dane naraz, optymalnie emitując tylko niezbędne sygnały.
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
     * @brief Sygnał emitowany przy zmianie dowolnych danych TOF.
     */
    void tofDataChanged();

    /**
     * @brief Sygnał emitowany przy zmianie dowolnych czujników linii.
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
    // --- Pola przechowujące aktualny stan wszystkich sensorów ---
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
