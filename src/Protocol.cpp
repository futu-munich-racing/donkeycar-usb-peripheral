#include "Protocol.h"

#include <functional>

#include <CRC32.h>


// void onPacketReceived(const uint8_t *buffer, size_t size)
// {
//     Protocol.receive(buffer, size);
//     //   if (size == sizeof(ControlMessage))
//     //   {
//     //     ControlMessage *controlMessage = (ControlMessage *)buffer;
//     //   }
// }

Protocol::Protocol()
{
}

void Protocol::begin(int serialBaud)
{
    _comm.begin(115200);
    _comm.setPacketHandler([](const uint8_t *buffer, size_t size) {

    });
}

void Protocol::send(uint16_t distance[3], uint16_t compass[3], uint16_t acceleration[3], uint16_t gyro[3])
{
    memccpy(_sensorPacketBuffer.distance, distance, 3, sizeof(uint16_t));
    memccpy(_sensorPacketBuffer.compass, compass, 3, sizeof(uint16_t));
    memccpy(_sensorPacketBuffer.acceleration, acceleration, 3, sizeof(uint16_t));
    memccpy(_sensorPacketBuffer.gyro, gyro, 3, sizeof(uint16_t));
    _sensorPacketBuffer.checksum = CRC32::calculate((const uint8_t *)&_sensorPacketBuffer, sizeof(SensorPacket) - 4);
}

void Protocol::receive(const uint8_t *buffer, size_t size)
{
}

void Protocol::update()
{
    _comm.update();
}