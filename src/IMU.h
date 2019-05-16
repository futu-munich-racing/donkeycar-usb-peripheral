#pragma once

#include <LIS3MDL.h>
#include <LSM6.h>
#include <MadgwickAHRS.h>

class IMU
{
public:
    static const int DataRate = 40;

    void begin();
    void update();

    uint16_t getPitch();
    uint16_t getRoll();
    uint16_t getYaw();

protected:
    float convertRawAcceleration(uint16_t value);
    float convertRawGyro(uint16_t value);
    float convertRawCompass(uint16_t value);

private:
    LSM6 _accelGyroSensor;
    LIS3MDL _compassSensor;

    Madgwick _filter;

    uint32_t _lastMeasurement{0};
};