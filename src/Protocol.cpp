#include "Protocol.h"

#include <functional>

#include <CRC32.h>

// Self reference for packet handler
extern Protocol protocol;

Protocol::Protocol()
{
}

void Protocol::begin(int serialBaud)
{
    _comm.begin(serialBaud);
}

void Protocol::send(uint16_t distance[3], uint16_t orientation[3], uint16_t acceleration[3], uint16_t rotation[3], uint8_t calibration[4])
{
    memcpy(_sensorPacketBuffer.distance, distance, 3 * sizeof(uint16_t));
    memcpy(_sensorPacketBuffer.orientation, orientation, 3 * sizeof(uint16_t));
    memcpy(_sensorPacketBuffer.acceleration, acceleration, 3 * sizeof(uint16_t));
    memcpy(_sensorPacketBuffer.rotation, rotation, 3 * sizeof(uint16_t));
    memcpy(_sensorPacketBuffer.calibration, calibration, 4 * sizeof(uint8_t));
    _sensorPacketBuffer.checksum = CRC32::calculate((const uint8_t *)&_sensorPacketBuffer, sizeof(SensorPacket) - 4);
    _comm.send((const uint8_t *)&_sensorPacketBuffer, sizeof(SensorPacket));
}

void Protocol::update()
{
    _comm.update();
}