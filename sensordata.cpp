#include "sensordata.h"

SensorData::SensorData(QObject *parent) : QObject(parent) {
}

void SensorData::setTof1(bool value) {
    if (m_isTof_1 != value) {
        m_isTof_1 = value;
        emit tofDataChanged();
        emit dataChanged();
    }
}

void SensorData::setTof2(bool value) {
    if (m_isTof_2 != value) {
        m_isTof_2 = value;
        emit tofDataChanged();
        emit dataChanged();
    }
}

void SensorData::setTof3(bool value) {
    if (m_isTof_3 != value) {
        m_isTof_3 = value;
        emit tofDataChanged();
        emit dataChanged();
    }
}


void SensorData::setTof4(bool value) {
    if (m_isTof_4 != value) {
        m_isTof_4 = value;
        emit tofDataChanged();
        emit dataChanged();
    }
}

void SensorData::setLineS1Active(bool value) {
    if (m_islineS_1_Active != value) {
        m_islineS_1_Active = value;
        emit lineSDataChanged();
        emit dataChanged();
    }
}


void SensorData::setLineS2Active(bool value) {
    if (m_islineS_2_Active != value) {
        m_islineS_2_Active = value;
        emit lineSDataChanged();
        emit dataChanged();
    }
}

void SensorData::setLineS3Active(bool value) {
    if (m_islineS_3_Active != value) {
        m_islineS_3_Active = value;
        emit lineSDataChanged();
        emit dataChanged();
    }
}

void SensorData::setMotor1Speed(int value) {
    if (m_motor1_speed != value) {
        m_motor1_speed = value;
        emit motorDataChanged();
        emit dataChanged();
    }
}

void SensorData::setMotor2Speed(int value) {
    if (m_motor2_speed != value) {
        m_motor2_speed = value;
        emit motorDataChanged();
        emit dataChanged();
    }
}

void SensorData::setImuX(float value) {
    if (m_imuX != value) {
        m_imuX = value;
        emit imuDataChanged();
        emit dataChanged();
    }
}

void SensorData::setImuY(float value) {
    if (m_imuY != value) {
        m_imuY = value;
        emit imuDataChanged();
        emit dataChanged();
    }
}

void SensorData::updateAllData(bool tof1, bool tof2, bool tof3, bool tof4,
                               bool lineS1, bool lineS2, bool lineS3,
                               int motor1, int motor2,
                               float imuX, float imuY) {
    bool tofChanged = (m_isTof_1 != tof1 || m_isTof_2 != tof2 || m_isTof_3 != tof3 || m_isTof_4 != tof4);
    bool lineChanged = (m_islineS_1_Active != lineS1 || m_islineS_2_Active != lineS2 || m_islineS_3_Active != lineS3);
    bool motorChanged = (m_motor1_speed != motor1 || m_motor2_speed != motor2);
    bool imuChanged = (m_imuX != imuX || m_imuY != imuY);

    m_isTof_1 = tof1;
    m_isTof_2 = tof2;
    m_isTof_3 = tof3;
    m_isTof_4 = tof4;

    m_islineS_1_Active = lineS1;
    m_islineS_2_Active = lineS2;
    m_islineS_3_Active = lineS3;

    m_motor1_speed = motor1;
    m_motor2_speed = motor2;

    m_imuX = imuX;
    m_imuY = imuY;

    if (tofChanged) emit tofDataChanged();
    if (lineChanged) emit lineSDataChanged();
    if (motorChanged) emit motorDataChanged();
    if (imuChanged) emit imuDataChanged();

    emit dataChanged();
}
