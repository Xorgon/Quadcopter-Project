//
// Created by Elijah on 14/02/2017.
//

#ifndef QUADCOPTER_PROJECT_LOGGER_H
#define QUADCOPTER_PROJECT_LOGGER_H

#include <Arduino.h>
#include <SD.h>

class Logger {
public:
    Logger(int sdChipSelect);

    Logger();

    void log(String tag, String data);

    String parseMillis(uint32_t millis);

private:

    String logName;
    String logDir;
    String toWrite;

    String getNextName();
};

#endif //QUADCOPTER_PROJECT_LOGGER_H
