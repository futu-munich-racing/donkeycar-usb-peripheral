#pragma once

#include <PacketSerial.h>
#include "Control.h"

// Struct for sensor data transmission over serial
struct __attribute__((packed)) SensorPacket
{
    uint16_t distance[3];
    uint16_t magneto[3];
    uint16_t acceleration[3];
    uint16_t gyro[3];
    uint32_t checksum;
};

// Struct for control data transmission over serial
struct __attribute__((packed)) ControlPacket
{
    uint16_t steering;
    uint16_t speed;
    uint32_t checksum;
};

class Protocol
{
public:
    Protocol(Control &control);

    void begin(int serialBaud);
    void update();

    void send(uint16_t distance[3], uint16_t magneto[3], uint16_t acceleration[3], uint16_t gyro[3]);

protected:
    void receive(const uint8_t *buffer, size_t size);

private:
    SLIPPacketSerial _comm;

    SensorPacket _sensorPacketBuffer;
    ControlPacket _controlPacketBuffer;
    Control &_control;
};