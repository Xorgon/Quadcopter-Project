//
// Created by Elijah on 14/02/2017.
//

#include "Logger.h"

Logger::Logger(int sdPin) {
    SD.begin(sdPin);
    logName = "";
    log("System", "Startup.");
}

void Logger::log(char tag[], char data[]) {
    logFile = SD.open(logName, FILE_WRITE);
    logFile.println("[" + parseMillis(millis()) + "][" + tag + "] " + data);
    logFile.close();
}

String Logger::parseMillis(int millis) {
    return String(floor((millis) / 1000 * 60))
           + ":" + String(floor((millis % (1000 * 60)) / 1000))
           + "." + String(floor((millis % 1000) / 10));
}