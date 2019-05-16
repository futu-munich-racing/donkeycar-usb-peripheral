#include "Sensors.h"

#include <Arduino.h>

Sensors::Sensors(uint8_t sonicTrigPin1, uint8_t sonicEchoPin1, uint8_t sonicTrigPin2, uint8_t sonicEchoPin2, Protocol &protocol) : _protocol(protocol), _sonicDistanceSensor1(sonicTrigPin1, sonicEchoPin1), _sonicDistanceSensor2(sonicTrigPin2, sonicEchoPin2)
{
}

void Sensors::begin()
{
    _imu.begin();

    if (!_tofDistanceSensor.init())
    {
        // Failed to detect and initialize ToF sensor!
        while (1)
        {
            delay(50);
            digitalToggle(PC13);
        }
    }
    _tofDistanceSensor.setTimeout(500);
    _tofDistanceSensor.startContinuous();
}

void Sensors::update()
{
    _distance[0] = (uint16_t)(_sonicDistanceSensor1.measureDistanceCm() * 10.0);
    _distance[1] = (uint16_t)(_sonicDistanceSensor2.measureDistanceCm() * 10.0);
    _distance[2] = _tofDistanceSensor.readRangeContinuousMillimeters();

    _imu.update();


    // _compassSensor.read();
    // _compass[0] = _compassSensor.m.x;
    // _compass[1] = _compassSensor.m.y;
    // _compass[2] = _compassSensor.m.z;

    // _imuSensor.read();
    // _acceleration[0] = convertRawAcceleration(_imuSensor.a.x);
    // _acceleration[1] = convertRawAcceleration(_imuSensor.a.y);
    // _acceleration[2] = convertRawAcceleration(_imuSensor.a.z);
    // _gyro[0] = convertRawGyro(_imuSensor.g.x);
    // _gyro[1] = convertRawGyro(_imuSensor.g.y);
    // _gyro[2] = convertRawGyro(_imuSensor.g.z);

    // _filter.updateIMU(_gyro[0], _gyro[1], _gyro[2], _acceleration[0], _acceleration[1], _acceleration[2]);

    uint16_t data[3] = {
        _imu.getPitch(),
        _imu.getRoll(),
        _imu.getYaw()};

    _protocol.send(_distance, data, data, data);
}