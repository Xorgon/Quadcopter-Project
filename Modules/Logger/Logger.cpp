//
// Created by Elijah on 14/02/2017.
//

#include "Logger.h"

Logger::Logger(int sdChipSelect) {
    SD.begin(sdChipSelect);
    logName = "test";
    logDir = "logs";
    log("System", "Startup.");
}

Logger::Logger() {};

void Logger::log(String tag, String data) {
    String logLine = "[" + parseMillis(millis()) + "][" + tag + "] " + data;
    Serial.println(logLine);
    File logFile = SD.open(logDir + "/" + getNextName() + ".qfl", FILE_WRITE);
    logFile.println(logLine);
    logFile.close();
}


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

String Logger::getNextName() {
    File dir = SD.open("/" + dir);
    int maxFlightN = 0;
    while (dir.available() > 0){
        File file = dir.openNextFile();
        String name = file.name();
        if (name.substring(0,6) == "flight") {
            flightN = name.substring(6,9).toInt();
            if (flightN > maxFlightN) {
                maxFlightN = flightN;
            }
        }
    }
    return "flight" + String(maxFlightN + 1);
}