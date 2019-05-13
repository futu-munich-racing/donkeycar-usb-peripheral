#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>

// External libraries
#include <HCSR04.h>
#include <VL53L0X.h>
#include <LIS3MDL.h>
#include <LSM6.h>
#include <CRC32.h>
#include <RingBuf.h>

// Local libraries
#include <ESC.h>

// Struct for sensor data transmission over serial
struct __attribute__((packed)) SensorMessage
{
  uint8_t magicBytes[2] = {0x55, 0xaa};
  uint16_t distance[3];
  uint16_t compass[3];
  uint16_t acceleration[3];
  uint16_t gyro[3];
  uint32_t checksum;
};

// Struct for control data transmission over serial
struct __attribute__((packed)) ControlMessage
{
  uint8_t magicBytes[2] = {0x55, 0xaa};
  uint16_t steering;
  uint16_t acceleration;
  uint32_t checksum;
};

SensorMessage sensorMsg;
ControlMessage controlMessage;

LSM6 imu;
LIS3MDL mag;
Servo servo;
VL53L0X tofSensor;
UltraSonicDistanceSensor sonicSensor(PB12, PB13);
ESC speedController(PB8);

int counter = 0;

RingBuf *inBuffer = RingBuf_new(sizeof(uint8_t), 64);

enum ReceiverStates
{
  ReceiverSyncA,
  ReceiverSyncB,
  ReceiverPayload,
  ReceiverChecksum
};

ReceiverStates state = ReceiverSyncA;
uint8_t payloadCounter = 0;
uint8_t checksumCounter = 0;

void transmitSensorData()
{
  Serial.write((const uint8_t *)&sensorMsg, sizeof(SensorMessage));
}

void receiveControlData()
{
  int tmp;
  while (1)
  {
    tmp = Serial.read();
    if (tmp == -1)
      break;

    switch (state)
    {
    case ReceiverSyncA:
      payloadCounter = 0;
      checksumCounter = 0;
      if (tmp == 0x55)
      {
        state = ReceiverSyncB;
      }
      break;
    case ReceiverSyncB:
      if (tmp == 0xaa)
      {
        state = ReceiverPayload;
      }
      else
      {
        state = ReceiverSyncA;
      }
      break;
    case ReceiverPayload:
      break;
    case ReceiverChecksum:
      break;
    }
  }
}

void setup()
{
  pinMode(PC13, OUTPUT);

  digitalWrite(PC13, LOW);

  Serial.begin(115200);
  Wire.begin();

  speedController.arm();
  servo.attach(PB9);

  if (!mag.init())
  {
    Serial.println("Failed to detect and initialize magnetometer!");
    while (1)
      ;
  }

  mag.enableDefault();

  if (!imu.init())
  {
    Serial.println("Failed to detect and initialize IMU!");
    while (1)
      ;
  }
  imu.enableDefault();

  tofSensor.init();
  tofSensor.setTimeout(500);
  tofSensor.startContinuous();
}

void loop()
{
  sensorMsg.distance[0] = (uint16_t)(sonicSensor.measureDistanceCm() * 10.0);
  sensorMsg.distance[1] = 0;
  sensorMsg.distance[2] = tofSensor.readRangeContinuousMillimeters();

  mag.read();
  sensorMsg.compass[0] = mag.m.x;
  sensorMsg.compass[1] = mag.m.y;
  sensorMsg.compass[2] = mag.m.z;

  imu.read();
  sensorMsg.acceleration[0] = imu.a.x;
  sensorMsg.acceleration[1] = imu.a.y;
  sensorMsg.acceleration[2] = imu.a.z;
  sensorMsg.gyro[0] = imu.g.x;
  sensorMsg.gyro[1] = imu.g.y;
  sensorMsg.gyro[2] = imu.g.z;

  sensorMsg.checksum = CRC32::calculate((const uint8_t *)&sensorMsg, sizeof(SensorMessage) - 4);

  transmitSensorData();

  delay(150);
  digitalToggle(PC13);

  speedController.speed(counter / 10);
  servo.write(counter);
  counter += 10;
  if (counter >= 200)
    counter = 0;
}