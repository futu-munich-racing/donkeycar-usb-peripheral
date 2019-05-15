from protocol import Protcol
from threading import Timer


class RepeatedTimer(object):
    def __init__(self, interval, function, *args, **kwargs):
        self._timer = None
        self.interval = interval
        self.function = function
        self.args = args
        self.kwargs = kwargs
        self.is_running = False
        self.start()

    def _run(self):
        self.is_running = False
        self.start()
        self.function(*self.args, **self.kwargs)

    def start(self):
        if not self.is_running:
            self._timer = Timer(self.interval, self._run)
            self._timer.start()
            self.is_running = True

    def stop(self):
        self._timer.cancel()
        self.is_running = False



def packetHandler(packet):
    print(packet)

proto = Protcol(packetHandler=packetHandler)

counter = 0
def send():
    global counter
    proto.sendControlPacket(counter, counter)
    counter = counter + 30
    if counter >= 180:
        counter = 0


rt = RepeatedTimer(1, send)

proto.start()
