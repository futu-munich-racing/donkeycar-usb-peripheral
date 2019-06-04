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
    memset(_orientation, 0, sizeof(uint16_t) * 3);
    memset(_rotation, 0, sizeof(uint16_t) * 3);
    memset(_acceleration, 0, sizeof(uint16_t) * 3);
    memset(_calibration, 0, sizeof(uint8_t) * 4);
}

void Sensors::begin()
{
    Wire.begin();

    if (!_imuSensor.begin())
    {
        // Failed to detect and initialize IMU!
        while (1)
        {
            delay(100);
            digitalToggle(PC13);
        }
    }

    if (!_tofDistanceSensor.init())
    {
        // Failed to detect and initialize ToF sensor!
        while (1)
        {
            delay(100);
            digitalToggle(PC13);
        }
    }
    _tofDistanceSensor.startContinuous();
}

void Sensors::update()
{

    if (millis() > _lastDistMeasurement + 100)
    {
        // _distance[0] = (uint16_t)(_sonicDistanceSensor1.measureDistanceCm() * 10.0);
        // _distance[1] = (uint16_t)(_sonicDistanceSensor2.measureDistanceCm() * 10.0);
        _distance[2] = _tofDistanceSensor.readRangeContinuousMillimeters();
        _lastDistMeasurement = millis();
    }

    if (millis() > _lastSentMsg + SampleInterval)
    {
        sensors_event_t orientationData, angVelocityData, linearAccelData;

        _imuSensor.getCalibration(&_calibration[0], &_calibration[1], &_calibration[2], &_calibration[3]);
        
        _imuSensor.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
        _imuSensor.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
        _imuSensor.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);

        _orientation[0] = (uint16_t)(orientationData.orientation.roll * 100.0);
        _orientation[1] = (uint16_t)(orientationData.orientation.pitch * 100.0);
        _orientation[2] = (uint16_t)(orientationData.orientation.heading * 100.0);

        _acceleration[0] = (uint16_t)(linearAccelData.acceleration.x * 100.0);
        _acceleration[1] = (uint16_t)(linearAccelData.acceleration.y * 100.0);
        _acceleration[2] = (uint16_t)(linearAccelData.acceleration.z * 100.0);

        _rotation[0] = (uint16_t)(angVelocityData.gyro.x * 100.0);
        _rotation[1] = (uint16_t)(angVelocityData.gyro.y * 100.0);
        _rotation[2] = (uint16_t)(angVelocityData.gyro.z * 100.0);

        _protocol.send(_distance, _orientation, _acceleration, _rotation, _calibration);
        _lastSentMsg = millis();
    }
}