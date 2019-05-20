#pragma once

#include <Wire.h>

#include <HCSR04.h>
#include <VL53L0X.h>
#include <LIS3MDL.h>
#include <LSM6.h>

#include "Protocol.h"

class Sensors
{
public:
    Sensors(uint8_t sonicTrigPin1, uint8_t sonicEchoPin1, uint8_t sonicTrigPin2, uint8_t sonicEchoPin2, Protocol &protocol);

    void begin();

    void update();

private:
    Protocol &_protocol;

    LSM6 _imuSensor;
    LIS3MDL _magnetoSensor;
    VL53L0X _tofDistanceSensor;
    UltraSonicDistanceSensor _sonicDistanceSensor1;
    UltraSonicDistanceSensor _sonicDistanceSensor2;

    uint16_t _distance[3];
    uint16_t _magneto[3];
    uint16_t _acceleration[3];
    uint16_t _gyro[3];
};