# Quadcopter-Project
Quadcopter Group Design Project repository.

# Project Structure
The project will be split up into 5 key modules:
- Autopilot
- Grabber
- Instruments
- Logger
- Receiver

# Logger
The Logger module allows for easy logging of information to the SD card and to the Serial monitor.

All modules should take a parameter of type Logger and invoke `logger.log(String tag, String data).`

The Logger writes "FLIGHTX.QFL" (Quadcopter Flight Log) files to the "logs" folder on the SD card. The flights are numerically ordered, so the first flight would be FLIGHT1.QFL.
