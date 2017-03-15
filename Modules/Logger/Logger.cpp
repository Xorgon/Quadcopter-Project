//
// Created by Elijah on 14/02/2017.
//

#include "Logger.h"

/**
 * Initializes Logger object.
 * @param sdChipSelect The SD chipSelect value, 10 for an UNO.
 */
Logger::Logger(int sdChipSelect) {
    SD.begin(sdChipSelect);
    String logName = getNextName();
    Serial.println("Logging to: " + logName + ".QFL");
    logFile = SD.open("/logs/" + logName + ".qfl", O_CREAT | O_WRITE);
    log(F("Logger"), F("Started."));
}

/**
 * Allows for creation of a Logger variable without initialization.
 */
Logger::Logger() {};

/**
 * Prepares data for logging and writes to an SD card and Serial monitor when buffer is full.
 * @param tag The name of the module calling the function.
 * @param data The data to be logged.
 */
 // TODO: Make log() automatically flush when the buffer is full.
void Logger::log(String tag, String data) {
    String logLine = "[" + parseMillis(millis()) + "][" + tag + "] " + data + "\n";
    Serial.print(logLine);
    logFile.print(logLine);
}

/**
 * Parses the given millis into a human-readable format.
 * @param millis The millis to be parsed. e.g. millis()
 * @return String with the format [mm:ss.ss]. e.g. [01:32.11]
 */
String Logger::parseMillis(uint32_t millis) {
    uint8_t mins = floor(millis / 60000);
    String sMins;
    if (mins < 10) {
        sMins = "0" + String(mins);
    } else {
        sMins = String(mins);
    }
    uint8_t secs = floor(millis % 60000) / 1000;
    String sSecs;
    if (secs < 10) {
        sSecs = "0" + String(secs);
    } else {
        sSecs = String(secs);
    }
    uint8_t decimalSecs = floor(millis % 1000) / 10;
    String sDecimalSecs;
    if (decimalSecs < 10) {
        sDecimalSecs = "0" + String(decimalSecs);
    } else {
        sDecimalSecs = String(decimalSecs);
    }
    return sMins + ":" + sSecs + "." + sDecimalSecs;
}

/**
 * Gets the next log file name in directory logDir.
 * @return Log file name. e.g. "FLIGHT9"
 */
String Logger::getNextName() {
    File dir = SD.open(F("/logs/"));
    int maxFlightN = 0;
    while (true) {
        File file = dir.openNextFile();
        if (!file) {
            // No more files
            break;
        }
        String name = file.name();
        int len = name.length();
        if (name.substring(len - 3, len) == F("QFL")) {
            name.replace(F("FLIGHT"), F(""));
            name.replace(F(".QFL"), F(""));
            int flightN = name.toInt();
            if (flightN > maxFlightN) {
                maxFlightN = flightN;
            }
        }
    }
    if (maxFlightN >= 15) {
        Serial.println(F("WARNING: EMPTY FLIGHT LOGS FOR NEW FLIGHT LOGS TO BE CREATED."));
        maxFlightN = 15;
    }
    return "FLIGHT" + String(maxFlightN + 1);
}