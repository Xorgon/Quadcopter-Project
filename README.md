# Quadcopter-Project
Quadcopter Group Design Project repository.

# Project Structure
The project will be split up into 4 key modules:
- Autopilot
- Grabber
- Instruments
- Logger

# Logger
The Logger module allows for easy logging of information to the SD card and to the Serial monitor. The Logger will usually use sdChipSelect pin 10.

All modules should take a parameter of type Logger and invoke `logger.log(String tag, String data).`

The Logger writes "FLIGHTX.QFL" (Quadcopter Flight Log) files to the "logs" folder on the SD card. The flights are numerically ordered, so the first flight would be FLIGHT1.QFL.

# Grabber
The Grabber module deals with release of the beanbag payload.

The main function of the module is `grabber.run(float pos[], float target[])`.

Logs when the payload is released.

# Autopilot
The Autopilot module handles communications and, when active, controls the aircraft.

To output control PWM values it uses Servo objects for easy PWM output. Each control has its own object which outputs to the following digital pins:

`pitch: 4`
`roll: 5`
`yaw: 6`
`throttle: 7`

The Autopilot receives its activation signal on digital pin 2. The active signal is 1900 microseconds +/- 100.

# Arduino Pin Connections
| Pin | Connection |
| --- | ---------- |
| D0 | SerialLogger RX |
| D1 | SerialLogger TX |
| D2 | Autopilot Activation Input |
| D3 | Pitch Servo Output |
| D4 | Roll Servo Output |
| D5 | Yaw Servo Output |
| D6 | Throttle Servo Output |
| D7 | Grabber Servo Output |
| D8 | MSP SoftSerial RX |
| D9 | MSP SoftSerial TX |
| D10 | Autopilot Activation Relay Output |
| D11 | SerialLogger Writing LED |
| D12 | Autopilot Active LED |
| D13 | General Loop Running LED |
| A0 | X-Axis IR Input |
| A1 | Y-Axis IR Input |
| A2 | Z-Axis IR Input |
| A3 | |
| A4 | |
| A5 | |
