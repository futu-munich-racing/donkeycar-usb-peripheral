#pragma once

#include <Wire.h>

#include <HCSR04.h>
#include <VL53L0X.h>
#include <LIS3MDL.h>
#include <LSM6.h>

class Sensors
{
public:
    Sensors(uint8_t sonicTrigPin, uint8_t sonicEchoPin);

    void begin();

    void update();

private:
    LSM6 _imuSensor;
    LIS3MDL _compassSensor;
    VL53L0X _tofDistanceSensor;
    UltraSonicDistanceSensor _sonicDistanceSensor;

    uint16_t _distance[3];
    uint16_t _compass[3];
    uint16_t _acceleration[3];
    uint16_t _gyro[3];
};