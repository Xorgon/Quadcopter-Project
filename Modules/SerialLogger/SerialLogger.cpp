//
// Created by Elijah on 14/02/2017.
//

#include "SerialLogger.h"

SerialLogger::SerialLogger() {
    sync = false;
    log(F("Logger"), F("Started."));
    sync = true;
}

SerialLogger::SerialLogger(uint8_t ledPin) {
    sync = false;
    log(F("Logger"), F("Started."));
    this->ledPin = ledPin;
    pinMode(ledPin, OUTPUT);
    sync = true;
}

/**
 * Prepares data for logging and sends it through serial communications.
 * @param tag The name of the module calling the function.
 * @param data The data to be logged.
 */
void SerialLogger::log(String tag, String data) {
    String logLine = "[" + parseMillis(millis()) + "][" + tag + "] " + data + "\n";
    Serial.print(logLine);

    digitalWrite(ledPin, HIGH);

    if (sync) {
        while (!Serial.available());
        while (Serial.available()) {
            Serial.read();
        }
    }

    digitalWrite(ledPin, LOW);
}

/**
 * Sends data through serial communications.
 * @param logLine The string to be written.
 */
void SerialLogger::log(String logLine) {
    Serial.print(logLine);

    digitalWrite(ledPin, HIGH);

    if (sync) {
        while (!Serial.available());
        while (Serial.available()) {
            Serial.read();
        }
    }

    digitalWrite(ledPin, LOW);
}

/**
 * Parses the given millis into a human-readable format.
 * @param millis The millis to be parsed. e.g. millis()
 * @return String with the format [mm:ss.ss]. e.g. [01:32.11]
 */
String SerialLogger::parseMillis(uint32_t millis) {
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