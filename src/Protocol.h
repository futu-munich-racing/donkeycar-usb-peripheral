#pragma once

#include <PacketSerial.h>

// Struct for sensor data transmission over serial
struct __attribute__((packed)) SensorPacket
{
    uint16_t distance[3];
    uint16_t orientation[3];
    uint16_t acceleration[3];
    uint16_t rotation[3];
    uint8_t calibration[4];
    uint32_t checksum;
};

class Protocol
{
public:
    Protocol();

    void begin(int serialBaud);
    void update();

    void send(uint16_t distance[3], uint16_t orientation[3], uint16_t acceleration[3], uint16_t rotation[3], uint8_t calibration[4]);

private:
    SLIPPacketSerial _comm;

    SensorPacket _sensorPacketBuffer;
};