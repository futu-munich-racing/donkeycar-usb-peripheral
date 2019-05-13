#include "Sensors.h"

Sensors::Sensors(uint8_t sonicTrigPin, uint8_t sonicEchoPin) : _sonicDistance(sonicTrigPin, sonicEchoPin)
{
}

void Sensors::begin()
{

    Wire.begin();

    if (!_compass.init())
    {
        Serial.println("Failed to detect and initialize magnetometer!");
        while (1)
            ;
    }

    _compass.enableDefault();

    if (!_imu.init())
    {
        Serial.println("Failed to detect and initialize IMU!");
        while (1)
            ;
    }
    _imu.enableDefault();

    _tofDistance.init();
    _tofDistance.setTimeout(500);
    _tofDistance.startContinuous(50);
}