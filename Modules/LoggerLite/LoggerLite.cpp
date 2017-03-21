//
// Created by Elijah on 14/02/2017.
//

#include "LoggerLite.h"

/**
 * Initializes LoggerLite object.
 * @param sdChipSelect The SD chipSelect value, 10 for an UNO.
 */
LoggerLite::LoggerLite(int sdChipSelect) {
    safeMount();

    if (pf_open(QFL_FILE_NAME)) {
        Serial.println(F("Failed to open file."));
    } else {
        Serial.println(F("File opened."));
    }

    uint8_t kilobytesWritten = getKilobytesWritten();

    if (kilobytesWritten == 0) {
        charsWritten = 2;
        pf_lseek(0);
        char buf[2];
        buf[0] = char(10);
        buf[1] = char(10);
        writeData(buf);
    } else {
        charsWritten = 1024 * kilobytesWritten;
        if (pf_lseek(charsWritten)) {
            Serial.println(F("Failed to move past previous writes."));
        };
    }

    for (int i = 0; i < 50; ++i) {
        log("TEST", "TEST TEST TEST TEST");
    }
    writeSecure();
}

/**
 * Allows for creation of a LoggerLite variable without initialization.
 */
LoggerLite::LoggerLite() {};

/**
 * Prepares data for logging and writes to an SD card and Serial monitor when buffer is full.
 * @param tag The name of the module calling the function.
 * @param data The data to be logged.
 */
void LoggerLite::log(String tag, String data) {
    String logLine = "[" + parseMillis(millis()) + "][" + tag + "] " + data + "\n";
    char buf[logLine.length()];
    logLine.toCharArray(buf, logLine.length());
    writeData(buf);
    Serial.print(logLine);
}

/**
 * Mounts the file system in a way that guarantees it will mount before continuing.
 */
void LoggerLite::safeMount() {
    Serial.println(F("Waiting for file system to mount..."));
    while (pf_mount(&fs));
    Serial.println(F("File system mounted."));
    // TODO: Mounting indicator.
}

uint16_t LoggerLite::getKilobytesWritten() {
    Serial.println("Getting Kilobytes written.");
    byte buf[2];
    UINT read;
    do {
        pf_read(&buf, sizeof(buf), &read);
    } while (read < 2);
    uint16_t kilobytesWritten;
    kilobytesWritten = buf[0];
    kilobytesWritten <<= 8;
    kilobytesWritten += buf[1];
    Serial.println(String(kilobytesWritten) + " KB");
    return kilobytesWritten;
}

void LoggerLite::writeData(const char *buf) {
    UINT bytesWritten;
    FRESULT res = pf_write(buf, sizeof(*buf), &bytesWritten);
    if (res) {
        Serial.println(F("Failed to write."));
        Serial.println(res);
    };
    charsWritten += bytesWritten;
}

/**
 * Ensures all data is written by filling up the file to the next sector boundary (next kilobyte).
 */
void LoggerLite::writeSecure() {
    Serial.println(F("Securing write."));
    // Fill to the next sector boundary (next kilobyte).
    uint16_t fillChars = 1024 - charsWritten % 1024;
    Serial.println("Filling " + String(fillChars) + " characters.");
    char fill[fillChars];
    for (int j = 0; j < fillChars; ++j) {
        if (j == fillChars - 1) {
            fill[j] = "v"; // TODO: Figure out why these don't write right (lol).
        } else {
            fill[j] = "X";
        }
    }
    writeData(fill);

    Serial.println(F("Storing chars written."));
    // Store in the first two bytes of the file.
    uint16_t kilobytesWritten = (charsWritten + fillChars) / 1024;
    char chars[2];
    chars[0] = kilobytesWritten >> 8; // Get first byte.
    chars[1] = kilobytesWritten & 0xFF; // Get second byte.
    pf_lseek(0);
    writeData(chars);
}

/**
 * Parses the given millis into a human-readable format.
 * @param millis The millis to be parsed. e.g. millis()
 * @return String with the format [mm:ss.ss]. e.g. [01:32.11]
 */
String LoggerLite::parseMillis(uint32_t millis) {
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