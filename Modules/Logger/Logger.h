//
// Created by Elijah on 14/02/2017.
//

#ifndef QUADCOPTER_PROJECT_LOGGER_H
#define QUADCOPTER_PROJECT_LOGGER_H

#include <Arduino.h>
#include <SD.h>
#include <math.h>

class Logger {
public:
    Logger(int sdPin);

    log(char tag[], char data[]);

    String parseMillis(int millis);

private:

    String logName;
};

#endif //QUADCOPTER_PROJECT_LOGGER_H
