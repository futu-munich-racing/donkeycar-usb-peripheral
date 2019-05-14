#include "Sensors.h"

#include <Arduino.h>

Sensors::Sensors(uint8_t sonicTrigPin, uint8_t sonicEchoPin, Protocol &protocol) : _protocol(protocol), _sonicDistanceSensor(sonicTrigPin, sonicEchoPin)
{
}

void Sensors::begin()
{
    Wire.begin();

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

    if (!_imuSensor.init())
    {
        // Failed to detect and initialize IMU!
        while (1)
        {
            delay(50);
            digitalToggle(PC13);
        }
    }
    _imuSensor.enableDefault();

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
    _tofDistanceSensor.startContinuous(50);
}

void Sensors::update()
{
    _distance[0] = (uint16_t)(_sonicDistanceSensor.measureDistanceCm() * 10.0);
    _distance[1] = 0;
    _distance[2] = _tofDistanceSensor.readRangeContinuousMillimeters();

    _compassSensor.read();
    _compass[0] = _compassSensor.m.x;
    _compass[1] = _compassSensor.m.y;
    _compass[2] = _compassSensor.m.z;

    _imuSensor.read();
    _acceleration[0] = _imuSensor.a.x;
    _acceleration[1] = _imuSensor.a.y;
    _acceleration[2] = _imuSensor.a.z;
    _gyro[0] = _imuSensor.g.x;
    _gyro[1] = _imuSensor.g.y;
    _gyro[2] = _imuSensor.g.z;

    _protocol.send(_distance, _compass, _acceleration, _gyro);
}