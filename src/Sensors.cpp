#include "Sensors.h"

#include <Arduino.h>

Sensors::Sensors(
    uint8_t sonicTrigPin1,
    uint8_t sonicEchoPin1,
    uint8_t sonicTrigPin2,
    uint8_t sonicEchoPin2,
    Protocol &protocol) : _protocol(protocol),
                          _sonicDistanceSensor1(sonicTrigPin1, sonicEchoPin1),
                          _sonicDistanceSensor2(sonicTrigPin2, sonicEchoPin2)
{
    memset(_distance, 0, sizeof(uint16_t) * 3);
    memset(_magneto, 0, sizeof(uint16_t) * 3);
    memset(_acceleration, 0, sizeof(uint16_t) * 3);
    memset(_gyro, 0, sizeof(uint16_t) * 3);
}

void Sensors::begin()
{
    Wire.begin();

    if (!_magnetoSensor.init())
    {
        // Failed to detect and initialize magnetometer!
        while (1)
        {
            delay(50);
            digitalToggle(PC13);
        }
    }

    _magnetoSensor.enableDefault();

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
    _tofDistanceSensor.startContinuous();
}

void Sensors::update()
{

    if (millis() > _lastDistMeasurement + 100)
    {
        _distance[0] = (uint16_t)(_sonicDistanceSensor1.measureDistanceCm() * 10.0);
        _distance[1] = (uint16_t)(_sonicDistanceSensor2.measureDistanceCm() * 10.0);
        _distance[2] = _tofDistanceSensor.readRangeContinuousMillimeters();
        _lastDistMeasurement = millis();
    }

    if (millis() > _lastSentMsg + SampleInterval)
    {

        _magnetoSensor.read();
        _magneto[0] = _magnetoSensor.m.x;
        _magneto[1] = _magnetoSensor.m.y;
        _magneto[2] = _magnetoSensor.m.z;

        _imuSensor.read();
        _acceleration[0] = _imuSensor.a.x;
        _acceleration[1] = _imuSensor.a.y;
        _acceleration[2] = _imuSensor.a.z;
        _gyro[0] = _imuSensor.g.x;
        _gyro[1] = _imuSensor.g.y;
        _gyro[2] = _imuSensor.g.z;


        _protocol.send(_distance, _magneto, _acceleration, _gyro);
        _lastSentMsg = millis();
    }
}