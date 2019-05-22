#include "Protocol.h"

#include <functional>

#include <CRC32.h>

// Self reference for packet handler
extern Protocol protocol;

Protocol::Protocol(Control &control) : _control(control)
{
}

void Protocol::begin(int serialBaud)
{
    _comm.begin(serialBaud);
    _comm.setPacketHandler([](const uint8_t *buffer, size_t size) {
        protocol.receive(buffer, size);
    });
}

void Protocol::send(uint16_t distance[3], uint16_t magneto[3], uint16_t acceleration[3], uint16_t gyro[3])
{
    memcpy(_sensorPacketBuffer.distance, distance, 3 * sizeof(uint16_t));
    memcpy(_sensorPacketBuffer.magneto, magneto, 3 * sizeof(uint16_t));
    memcpy(_sensorPacketBuffer.acceleration, acceleration, 3 * sizeof(uint16_t));
    memcpy(_sensorPacketBuffer.gyro, gyro, 3 * sizeof(uint16_t));
    _sensorPacketBuffer.checksum = CRC32::calculate((const uint8_t *)&_sensorPacketBuffer, sizeof(SensorPacket) - 4);
    _comm.send((const uint8_t *)&_sensorPacketBuffer, sizeof(SensorPacket));
}

void Protocol::receive(const uint8_t *buffer, size_t size)
{
    ControlPacket *packet = (ControlPacket *)buffer;
    uint32_t checksum = CRC32::calculate((const uint8_t *)&buffer, sizeof(ControlPacket) - 4);
    if (packet->checksum == checksum)
    {
        _control.setSpeed(packet->speed);
        _control.setSteering(packet->steering);
    }
}

void Protocol::update()
{
    _comm.update();
}