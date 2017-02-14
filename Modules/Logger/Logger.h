//
// Created by Elijah on 14/02/2017.
//

#ifndef QUADCOPTER_PROJECT_LOGGER_H
#define QUADCOPTER_PROJECT_LOGGER_H

#include <Arduino.h>
#include <SD.h>

class Logger {
public:
    Logger();

    log(char tag[], char data[]);

private:
    File logFile;
};

#endif //QUADCOPTER_PROJECT_LOGGER_H
