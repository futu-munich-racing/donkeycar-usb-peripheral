#pragma once

#include <Servo.h>
#include <ESC.h>

class Control
{
public:
    Control(uint8_t steeringPin, uint8_t speedPin);

    void begin();

    void setSpeed(uint16_t speed);
    void setSteering(uint16_t angle);

private:
    Servo _steeringCtrl;
    uint8_t _steeringPin;

    ESC _speedCtrl;
    // bool _speedArmed{false};
};