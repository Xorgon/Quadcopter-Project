//
// Created by Elijah on 14/02/2017.
//

#ifndef QUADCOPTER_PROJECT_LOGGER_H
#define QUADCOPTER_PROJECT_LOGGER_H

#include <Arduino.h>
#include <PetitFS.h>

/**
 * Allows for easy logging of information to the SD card and to the Serial monitor using log().
 *
 * The LoggerLite writes "FLIGHTX.QFL" (Quadcopter Flight Log) files to the logDir folder on the SD card.
 *
 * The flights are numerically ordered, so the first flight would be FLIGHT1.QFL.
 *
 * \see LoggerLite::log() for logging data.
 *
 * @author Elijah Andrews
 */
class LoggerLite {
public:
    LoggerLite(int sdChipSelect); // TODO: Change signature.

    LoggerLite();

    void log(String tag, String data);

    String parseMillis(uint32_t millis);

    void writeSecure();

private:

    FATFS fs;

#define QFL_FILE_NAME "RAW.QFL"

    void safeMount();

    void openFile();

    uint16_t getKilobytesWritten();

    void writeData(const char *buf);

    uint32_t charsWritten;
};

#endif //QUADCOPTER_PROJECT_LOGGER_H
