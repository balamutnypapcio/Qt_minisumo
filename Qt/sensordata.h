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
    explicit SensorData(QObject *parent = nullptr);

    // Gettery i settery dla danych sensorów
    bool getTof1() const { return m_isTof_1; }
    void setTof1(bool value);

    bool getTof2() const { return m_isTof_2; }
    void setTof2(bool value);

    bool getTof3() const { return m_isTof_3; }
    void setTof3(bool value);

    bool getTof4() const { return m_isTof_4; }
    void setTof4(bool value);

    bool getLineS1Active() const { return m_islineS_1_Active; }
    void setLineS1Active(bool value);

    bool getLineS2Active() const { return m_islineS_2_Active; }
    void setLineS2Active(bool value);

    bool getLineS3Active() const { return m_islineS_3_Active; }
    void setLineS3Active(bool value);

    int getMotor1Speed() const { return m_motor1_speed; }
    void setMotor1Speed(int value);

    int getMotor2Speed() const { return m_motor2_speed; }
    void setMotor2Speed(int value);

    float getImuX() const { return m_imuX; }
    void setImuX(float value);

    float getImuY() const { return m_imuY; }
    void setImuY(float value);

    /**
     * @brief Aktualizuje wszystkie dane naraz.
     */
    void updateAllData(bool tof1, bool tof2, bool tof3, bool tof4,
                       bool lineS1, bool lineS2, bool lineS3,
                       int motor1, int motor2,
                       float imuX, float imuY);

signals:
    // Sygnały emitowane przy zmianie danych
    void tofDataChanged();
    void lineSDataChanged();
    void motorDataChanged();
    void imuDataChanged();
    void dataChanged();

private:
    bool m_isTof_1 = false;
    bool m_isTof_2 = false;
    bool m_isTof_3 = false;
    bool m_isTof_4 = false;

    bool m_islineS_1_Active = false;
    bool m_islineS_2_Active = false;
    bool m_islineS_3_Active = false;

    int m_motor1_speed = 0;
    int m_motor2_speed = 0;

    float m_imuX = 0;
    float m_imuY = 0;
};

#endif // SENSORDATA_H
