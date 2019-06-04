#pragma once

#include <Wire.h>

#include <HCSR04.h>
#include <VL53L0X.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

#include "Protocol.h"

class Sensors
{
public:
    static const int SampleRate = 20;
    static const int SampleInterval = 1000/SampleRate;

    Sensors(uint8_t sonicTrigPin1, uint8_t sonicEchoPin1, uint8_t sonicTrigPin2, uint8_t sonicEchoPin2, Protocol &protocol);

    void begin();

    void update();

private:
    Protocol &_protocol;

    Adafruit_BNO055 _imuSensor;
    VL53L0X _tofDistanceSensor;
    UltraSonicDistanceSensor _sonicDistanceSensor1;
    UltraSonicDistanceSensor _sonicDistanceSensor2;

    uint16_t _distance[3];
    uint16_t _orientation[3];
    uint16_t _rotation[3];
    uint16_t _acceleration[3];
    uint8_t _calibration[4];

    uint32_t _lastSentMsg{0};
    uint32_t _lastDistMeasurement{0};
};