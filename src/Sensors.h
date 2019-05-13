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

private:
    LSM6 _imu;
    LIS3MDL _compass;
    VL53L0X _tofDistance;
    UltraSonicDistanceSensor _sonicDistance;
};