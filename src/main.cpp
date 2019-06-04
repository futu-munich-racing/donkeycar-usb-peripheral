#include <Arduino.h>

#include "Protocol.h"
#include "Sensors.h"

Protocol protocol;
Sensors sensors(PB12, PB13, PB14, PB15, protocol);

uint32_t lastLedToggle{0};

void setup()
{
  pinMode(PC13, OUTPUT);
  digitalWrite(PC13, LOW);

  protocol.begin(115200);
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
