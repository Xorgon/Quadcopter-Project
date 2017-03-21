#include <Arduino.h>
#include "LoggerLite.h"
#include "PetitFS.h"

#define QFL_FILE_NAME "RAW.QFL"

FATFS fs;
uint32_t charsWritten;
//LoggerLite logger;

void setup() {
    Serial.begin(9600);
//    logger = LoggerLite(0); // Arbitrary int.
//    logger.log("TEST", "TEST TEST TEST");
//    logger.writeSecure();

    Serial.println(F("Waiting for file system to mount..."));
    while (pf_mount(&fs));
    Serial.println(F("File system mounted."));

    if (pf_open(QFL_FILE_NAME)) {
        Serial.println(F("Failed to open file."));
    } else {
        Serial.println(F("File opened."));
    }

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

    if (kilobytesWritten == 0) {
        charsWritten = 2;
        pf_lseek(0);
        char buf2[2];
        buf2[0] = char(10);
        buf2[1] = char(10);
        UINT bytesWritten;
        FRESULT res = pf_write(buf2, sizeof(*buf2), &bytesWritten);
        if (res) {
            Serial.println(F("Failed to write."));
            Serial.println(res);
        };
        charsWritten += bytesWritten;
    } else {
        charsWritten = 1024 * kilobytesWritten;
        if (pf_lseek(charsWritten)) {
            Serial.println(F("Failed to move past previous writes."));
        };
    }

    String logLine = "[00:00:55][FARTS] FARTS FARTS\n";
    char buf3[logLine.length()];
    logLine.toCharArray(buf3, logLine.length());
    UINT bytesWritten;
    FRESULT res = pf_write(buf3, sizeof(buf3), &bytesWritten);
    if (res) {
        Serial.println(F("Failed to write."));
        Serial.println(res);
    };
    charsWritten += bytesWritten;
    Serial.print(logLine);

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
    bytesWritten;
    res = pf_write(fill, sizeof(fill), &bytesWritten);
    if (res) {
        Serial.println(F("Failed to write."));
        Serial.println(res);
    };
    charsWritten += bytesWritten;

    Serial.println(F("Storing chars written."));
    // Store in the first two bytes of the file.
    kilobytesWritten = (charsWritten + fillChars) / 1024;
    char chars[2];
    chars[0] = kilobytesWritten >> 8; // Get first byte.
    chars[1] = kilobytesWritten & 0xFF; // Get second byte.
    pf_lseek(0);
    bytesWritten;
    res = pf_write(chars, sizeof(chars), &bytesWritten);
    if (res) {
        Serial.println(F("Failed to write."));
        Serial.println(res);
    };
    charsWritten += bytesWritten;

}

void loop() {

}