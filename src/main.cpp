#include <Arduino.h>

#include "Protocol.h"
#include "Control.h"
#include "Sensors.h"

int counter = 0;

Protocol protocol;
Control control(PB9, PB8);
Sensors sensors(PB12, PB13);

void setup()
{
    pinMode(PC13, OUTPUT);
    digitalWrite(PC13, LOW);

    protocol.begin(115200);
    control.begin();
    sensors.begin();
}

void loop()
{
    sensors.update();
    protocol.update();

    delay(150);
    digitalToggle(PC13);
    

}