#pragma once

#include <Wire.h>

#include <HCSR04.h>
#include <VL53L0X.h>
#include "IMU.h"

#include "Protocol.h"

class Sensors
{
public:
    Sensors(uint8_t sonicTrigPin1, uint8_t sonicEchoPin1, uint8_t sonicTrigPin2, uint8_t sonicEchoPin2, Protocol &protocol);

    void begin();

    void update();

private:
    Protocol &_protocol;
    
    VL53L0X _tofDistanceSensor;
    UltraSonicDistanceSensor _sonicDistanceSensor1;
    UltraSonicDistanceSensor _sonicDistanceSensor2;

    IMU _imu;

    uint16_t _distance[3];
};