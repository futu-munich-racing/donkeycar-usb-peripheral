#include <Arduino.h>

#include "Protocol.h"
#include "Control.h"
#include "Sensors.h"

Control control(PB9, PB8);
Protocol protocol(control);
Sensors sensors(PB12, PB13, PB14, PB15, protocol);

uint32_t lastLedToggle{0};

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

  if (millis() > lastLedToggle + 700)
  {
    digitalToggle(PC13);
    lastLedToggle = millis();
  }
}