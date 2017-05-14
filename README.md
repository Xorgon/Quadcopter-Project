# Quadcopter-Project
Quadcopter Group Design Project repository.

# Project Structure
The project is split into 4 modules:
- Autopilot
- Grabber
- Instruments
- Logger

These are each implemented in the QuadControl sketch.

## Logger
The Logger module allows for easy logging of information to the SD card and to the Serial monitor. 

There are three different logger modules: Logger, LoggerLite, and SerialLogger.

All modules should take a parameter of a logger and invoke `logger.log(String tag, String data)` to log data.

### Logger

The Logger module is for SD writing. It has to main functions:

Firstly: `logger.log(String tag, String data)`

This takes a logger tag and some data to be logged and writes a log line to the SD card.

e.g. `[00:04.54][Instruments] Location: -1.01, -0.52, 0.92`

Secondly: `logger.log(String logLine)`

This takes a whole log line and writes it to the SD card.

Logger writes "FLIGHTX.QFL" (Quadcopter Flight Log) files to the "logs" folder on the SD card. The flights are numerically ordered, so the first flight would be FLIGHT1.QFL.

The Logger will usually use sdChipSelect pin 10.

### LoggerLite

The LoggerLite module is an incomplete attempt to reduce memory overhead using [Bill Greiman's PetitFS](https://github.com/greiman/PetitFS) (also included in the libraries directory).

LoggerLite would function the same as Logger on the front-end.

### SerialLogger

SerialLogger functions similarly to Logger, except that it simply prints through the hardware Serial.

SerialLogger has two modes, chosen by the boolean `sync`. When `sync` is true the SerialLogger module waits for a response over Serial. When `sync` is false the SerialLogger module does not wait for a response over Serial.

SerialLogger is accompanied by SerialLogger.ino, a sketch that receives Serial input and uses the Logger module to write to SD.

## Grabber
The Grabber module deals with release of the beanbag payload.

The main function of the module is `grabber.run(float pos[], float target[])`.

Logs when the payload is released.

## Autopilot
The Autopilot module handles communications and, when active, controls the aircraft.

To output control PWM values it uses Servo objects for easy PWM output. Each control has its own object which outputs to the following digital pins:

```
pitch: 3
roll: 4
yaw: 5
throttle: 6
```

The Autopilot receives its activation signal on digital pin 2. The active signal is 1900 microseconds +/- 100.

The Autopilot requires three external global variable declarations in sketches that use it:

```
volatile unsigned long lastPWMTime;
volatile uint16_t pwmValue;
volatile bool autopilotActive;
 ```

# Arduino Pin Connections
| Pin | Connection |
| --- | ---------- |
| D0 | SerialLogger RX |
| D1 | SerialLogger TX |
| D2 | ~~Autopilot PWM Activation Input~~ |
| D3 | Pitch Servo Output |
| D4 | Roll Servo Output |
| D5 | ~~Yaw Servo Output~~ |
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
| A5 | Autopilot Digital Activation Input|
