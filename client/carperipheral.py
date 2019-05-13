import serial
from hexdump import hexdump
from zlib import crc32


class SensorMessage:
    def __init__(self, data):
        payload = data[0:26]
        checksum = data[26:30]
        calculatedChecksum = (
            crc32(payload) & 0xffffffff).to_bytes(4, 'little')

        self.valid = checksum == calculatedChecksum
        if not self.valid:
            return

        self.distance = self._convert(payload, 2)
        self.compass = self._convert(payload, 8)
        self.acceleration = self._convert(payload, 14)
        self.gyro = self._convert(payload, 20)
        
        hexdump(payload[2:])

    def _convert(self, data, offset):
        out=[]
        for idx in range(3):
            out.append(int.from_bytes(data[offset+(idx*2):offset+((idx+1)*2)], 'little'))
        return out

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

with serial.Serial('/dev/ttyACM0', 115200) as comm:
    while True:
        inBuff.extend(comm.read_all())
        if len(inBuff) >= 30:
            while inBuff[0] != 0x55 and inBuff[1] != 0xaa:
                del inBuff[0]

            if len(inBuff) >= 30:
                msg = SensorMessage(inBuff[0:30])
                # msg = inBuff[0:30]
                print(msg)
                # hexdump(msg)
                del inBuff[0:30]
