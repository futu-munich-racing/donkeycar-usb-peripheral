# Sources from: https://tinkering.xyz/async-serial/
# Thanks for the great article!

import asyncio
import serial_asyncio
from sliplib import Driver as SLIPDriver
from hexdump import hexdump
from zlib import crc32
import struct


def calcChecksum(data):
    return (crc32(data) & 0xffffffff).to_bytes(4, 'little')


slipDriver = SLIPDriver()


class SensorPacket:
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


class Writer(asyncio.Protocol):
    def connection_made(self, transport):
        """Store the serial transport and schedule the task to send data.
        """
        self._transport = transport
        print('Writer connection created')
        # asyncio.ensure_future(self.send())
        # print('Writer.send() scheduled')

    def connection_lost(self, exc):
        print('Writer closed')

    async def send(self, packet):
        """Send four newline-terminated messages, one byte at a time.
        """
        # message = b'foo\nbar\nbaz\nqux\n'
        # for b in message:
        #     await asyncio.sleep(0.5)
        #     self.transport.serial.write(bytes([b]))
        #     print(f'Writer sent: {bytes([b])}')
        # self.transport.close()


class Reader(asyncio.Protocol):
    def __init__(self, packetHandler=lambda: None):
        super().__init__()
        self._packetHandler = packetHandler

    def connection_made(self, transport):
        """Store the serial transport and prepare to receive data.
        """
        self._transport = transport
        self._buffer = bytearray()
        print('Reader connection created')

    def data_received(self, data):
        """Store characters until a newline is received.
        """
        print('Data received')
        self._buffer += data
        if len(self._buffer) >= 28:
            data = slipDriver.receive(self._buffer)
            if len(data) > 0:
                for rawPacket in data:
                    hexdump(rawPacket)
                    packet = SensorPacket(rawPacket)
                    self._packetHandler(packet)
        # if b'\n' in self.buf:
        #     lines = self.buf.split(b'\n')
        #     self.buf = lines[-1]  # whatever was left over
        #     for line in lines[:-1]:
        #         print(f'Reader received: {line.decode()}')
        #         self.msgs_recvd += 1
        # if self.msgs_recvd == 4:
        #     self.transport.close()

    def connection_lost(self, exc):
        print('Reader closed')


class Protcol:
    def __init__(self, device='/dev/ttyACM0', baud=115200):
        self._loop = asyncio.get_event_loop()
        self._reader = serial_asyncio.create_serial_connection(
            self._loop, lambda: Reader(self.packetHandler), device, baudrate=baud)
        # self._writer = serial_asyncio.create_serial_connection(self._loop, Writer, device, baudrate=baud)
        asyncio.ensure_future(self._reader)
        print('Reader scheduled')
        # asyncio.ensure_future(self._writer)
        # print('Writer scheduled')

    def start(self):
        # self._loop.call_later(10, self._loop.stop)
        self._loop.run_forever()
        print('Done')

    def packetHandler(self, packet):
        print(packet)

