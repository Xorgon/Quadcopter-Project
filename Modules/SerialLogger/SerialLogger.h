//
// Created by Elijah on 14/02/2017.
//

#ifndef QUADCOPTER_PROJECT_LOGGER_H
#define QUADCOPTER_PROJECT_LOGGER_H

#include <Arduino.h>

/**
 * Allows for easy logging of information through serial communications using log().
 *
 * \see Logger::log() for logging data.
 *
 * @author Elijah Andrews
 */
class SerialLogger {
public:
    SerialLogger();

    SerialLogger(uint8_t ledPin);

    void log(String tag, String data);

    void log(String logLine);

    String parseMillis(uint32_t millis);

private:
    uint8_t ledPin;
};

#endif //QUADCOPTER_PROJECT_LOGGER_H
