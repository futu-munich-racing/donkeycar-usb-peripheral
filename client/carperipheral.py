from protocol import Protocol


def packetHandler(packet):
    print(packet)


proto = Protocol(device='/dev/ttyACM0', packetHandler=packetHandler)


proto.start()
