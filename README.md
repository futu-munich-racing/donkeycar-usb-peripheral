# donkeycar-usb-peripheral

Here you can find the firmware for the device ([Blue Pill](https://wiki.stm32duino.com/index.php?title=Blue_Pill)) and the host library to communicate with the peripheral.

## Directories

* **src/**: Firmware sources
* **lib/**: 3rd party libraries, which are not in the registry
* **client/**: Python client library sources

## Firmware components

* **main.cpp**: Configures the system and runs the main loop
* **Control.cpp/h**: Class to control the actuators (servo and ESC)
* **Sensors.cpp/h**: Class to bundle the sensors and make measurents with a defined sample rate
* **Protocol.cpp/h**: Class bundling a very simple communication protocol to communicate over serial (SLIP)