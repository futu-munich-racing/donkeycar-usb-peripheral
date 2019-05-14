#include "Control.h"

Control::Control(uint8_t steeringPin, uint8_t speedPin) : _steeringPin(steeringPin), _speedCtrl(speedPin)
{
}

void Control::begin()
{
    _speedCtrl.arm();
    _steeringCtrl.attach(_steeringPin);
}

void Control::setSpeed(uint16_t speed)
{
    _speedCtrl.speed(speed);
}

void Control::setSteering(uint16_t angle)
{
    _steeringCtrl.write(angle);
}