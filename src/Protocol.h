#pragma once

#include <PacketSerial.h>

// Struct for sensor data transmission over serial
struct __attribute__((packed)) SensorPacket
{
    uint8_t magicBytes[2] = {0x55, 0xaa};
    uint16_t distance[3];
    uint16_t compass[3];
    uint16_t acceleration[3];
    uint16_t gyro[3];
    uint32_t checksum;
};

// Struct for control data transmission over serial
struct __attribute__((packed)) ControlPacket
{
    uint8_t magicBytes[2] = {0x55, 0xaa};
    uint16_t steering;
    uint16_t acceleration;
    uint32_t checksum;
};

class Protocol
{
public:
    Protocol();

    void begin(int serialBaud);
    void update();

    void send(uint16_t distance[3], uint16_t compass[3], uint16_t acceleration[3], uint16_t gyro[3]);

protected:
    void receive(const uint8_t *buffer, size_t size);

private:
    SLIPPacketSerial _comm;

    SensorPacket _sensorPacketBuffer;
    ControlPacket _controlPacketBuffer;
};