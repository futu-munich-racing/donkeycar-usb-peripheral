#include "IMU.h"

void IMU::begin()
{
    if (!_compassSensor.init())
    {
        // Failed to detect and initialize magnetometer!
        while (1)
        {
            delay(50);
            digitalToggle(PC13);
        }
    }
    _compassSensor.enableDefault();
    // Set output data rate to 40 Hz
    _compassSensor.writeReg(_compassSensor.CTRL_REG1, 0x78);

    if (!_accelGyroSensor.init())
    {
        // Failed to detect and initialize IMU!
        while (1)
        {
            delay(50);
            digitalToggle(PC13);
        }
    }
    _accelGyroSensor.enableDefault();

    _filter.begin(DataRate);
}

void IMU::update()
{
    if (millis() > _lastMeasurement + (1000 / DataRate))
    {

        _compassSensor.read();
        // _compass[0] = _compassSensor.m.x;
        // _compass[1] = _compassSensor.m.y;
        // _compass[2] = _compassSensor.m.z;

        _accelGyroSensor.read();
        // _acceleration[0] = convertRawAcceleration(_accelGyroSensor.a.x);
        // _acceleration[1] = convertRawAcceleration(_accelGyroSensor.a.y);
        // _acceleration[2] = convertRawAcceleration(_accelGyroSensor.a.z);
        // _gyro[0] = convertRawGyro(_accelGyroSensor.g.x);
        // _gyro[1] = convertRawGyro(_accelGyroSensor.g.y);
        // _gyro[2] = convertRawGyro(_accelGyroSensor.g.z);

        _filter.updateIMU(
            convertRawGyro(_accelGyroSensor.g.x),
            convertRawGyro(_accelGyroSensor.g.y),
            convertRawGyro(_accelGyroSensor.g.z),
            convertRawAcceleration(_accelGyroSensor.a.x),
            convertRawAcceleration(_accelGyroSensor.a.y),
            convertRawAcceleration(_accelGyroSensor.a.z));

        _lastMeasurement = millis();
    }
    // uint16_t data[3] = {
    //     (uint16_t)(_filter.getPitch() * 10.0),
    //     (uint16_t)(_filter.getRoll() * 10.0),
    //     (uint16_t)(_filter.getYaw() * 10.0)};
}

float IMU::convertRawAcceleration(uint16_t value)
{
    return ((float)value * 2.0) / 32768.0;
}

float IMU::convertRawGyro(uint16_t value)
{
    return ((float)value * 245.0) / 32768.0;
}

float IMU::convertRawCompass(uint16_t value)
{
    return ((float)value * 4.0) / 32768.0;
}

uint16_t IMU::getPitch()
{
    return (uint16_t)_filter.getPitch();
}

uint16_t IMU::getRoll()
{
    return (uint16_t)_filter.getRoll();
}

uint16_t IMU::getYaw()
{
    return (uint16_t)_filter.getYaw();
}