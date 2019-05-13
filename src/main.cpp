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
    //   sensorMsg.distance[0] = (uint16_t)(sonicSensor.measureDistanceCm() * 10.0);
    //   sensorMsg.distance[1] = 0;
    //   sensorMsg.distance[2] = tofSensor.readRangeContinuousMillimeters();

    //   mag.read();
    //   sensorMsg.compass[0] = mag.m.x;
    //   sensorMsg.compass[1] = mag.m.y;
    //   sensorMsg.compass[2] = mag.m.z;

    //   imu.read();
    //   sensorMsg.acceleration[0] = imu.a.x;
    //   sensorMsg.acceleration[1] = imu.a.y;
    //   sensorMsg.acceleration[2] = imu.a.z;

    //   sensorMsg.gyro[0] = imu.g.x;
    //   sensorMsg.gyro[1] = imu.g.y;
    //   sensorMsg.gyro[2] = imu.g.z;

    //   sensorMsg.checksum = CRC32::calculate((const uint8_t *)&sensorMsg, sizeof(SensorMessage) - 4);

    //   comm.send((const uint8_t *)&sensorMsg, sizeof(SensorMessage));

    delay(150);
    digitalToggle(PC13);

    // speedController.speed(counter / 10);
    // servo.write(counter);
    // counter += 10;

    // if (counter >= 200)
    //     counter = 0;

    //   comm.update();

    protocol.update();
}