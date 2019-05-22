#include "Control.h"

Control::Control(uint8_t steeringPin, uint8_t speedPin) : _steeringPin(steeringPin), _speedCtrl(speedPin)
{
}

void Control::begin()
{
    _steeringCtrl.attach(_steeringPin);
    _speedCtrl.arm();
    _speedCtrl.speed(1500);
}

void Control::setSpeed(uint16_t speed)
{
    // if (!_speedArmed)
    // {
    //     _speedArmed = true;
    // }
    _speedCtrl.speed(speed);
}

void Control::setSteering(uint16_t angle)
{
    _steeringCtrl.write(angle);
}