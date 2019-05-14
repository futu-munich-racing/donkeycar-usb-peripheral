import serial
from sliplib import Driver as SLIPDriver
from hexdump import hexdump
from zlib import crc32
import struct
import time

driver = SLIPDriver()


def calcChecksum(data):
    return (crc32(data) & 0xffffffff).to_bytes(4, 'little')


def sendControlPacket(comm, steering, speed):
    print('Steering: {0}, Speed: {1}'.format(steering, speed))
    payload = bytearray(struct.pack('<HH', steering, speed))
    checksum = calcChecksum(payload)
    payload.extend(checksum)
    framed = driver.send(payload)
    hexdump(payload)
    comm.write(framed)


class SensorMessage:
    def __init__(self, data):
        payload = data[0:24]
        checksum = data[24:28]
        calculatedChecksum = calcChecksum(payload)

        self.valid = checksum == calculatedChecksum
        if not self.valid:
            return

        tmp = list(struct.unpack('<HHHHHHHHHHHH', payload))

        self.distance = tmp[0:3]
        self.compass = tmp[3:6]
        self.acceleration = tmp[6:9]
        self.gyro = tmp[9:12]

    def __str__(self):
        if not self.valid:
            return 'SensorMessage: Invalid!'
        return 'SensorMessage:\nDist: {0}\nCompass: {1}\nAccel: {2}\nGyro: {3}'.format(
            self.distance,
            self.compass,
            self.acceleration,
            self.gyro
        )


inBuff = bytearray()

counter = 0

with serial.Serial('/dev/ttyACM0', 115200) as comm:
    while True:
        inBuff.extend(comm.read_all())

        if len(inBuff) >= 28:
            data = driver.receive(inBuff)
            if len(data) > 0:
                for packet in data:
                    print(len(packet))
                    hexdump(packet)
                    msg = SensorMessage(packet)
                    print(msg)

        time.sleep(1)
        sendControlPacket(comm, counter, counter)
        counter = counter + 60
        if counter > 180:
            counter = 0
