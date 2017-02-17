//
// Created by Elijah on 14/02/2017.
//

#include "Logger.h"

Logger::Logger(int sdChipSelect) {
    SD.begin(sdChipSelect);
    logName = getNextName();
    Serial.println("Logging to: " + logName + ".QFL");
    logDir = "logs";
    log("System", "Startup.");
}

Logger::Logger() {};

void Logger::log(String tag, String data) {
    String logLine = "[" + parseMillis(millis()) + "][" + tag + "] " + data;
    Serial.println(logLine);
    File logFile = SD.open("/" + logDir + "/" + logName + ".qfl", FILE_WRITE);
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
    File dir = SD.open("/logs/");
    int maxFlightN = 0;
    while (true) {
        File file = dir.openNextFile();
        if (! file) {
            // No more files
            break;
        }
        String name = file.name();
        int len = name.length();
        if (name.substring(len - 3, len) == "QFL") {
            name.replace("FLIGHT", "");
            name.replace(".QFL", "");
            int flightN = name.toInt();
            if (flightN > maxFlightN) {
                maxFlightN = flightN;
            }
        }
    }
    return "FLIGHT" + String(maxFlightN + 1);
}