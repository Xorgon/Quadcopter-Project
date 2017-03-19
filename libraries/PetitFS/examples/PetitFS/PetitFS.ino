// Petit FS test.   
// For minimum flash use edit pffconfig.h and only enable
// _USE_READ and either _FS_FAT16 or _FS_FAT32

#include "PetitFS.h"
#include "PetitSerial.h"

PetitSerial PS;
// Use PetitSerial instead of Serial.
#define Serial PS

// The SD chip select pin is currently defined as 10
// in pffArduino.h.  Edit pffArduino.h to change the CS pin.

FATFS fs;     /* File system object */
//------------------------------------------------------------------------------
void errorHalt(char *msg) {
    Serial.print("Error: ");
    Serial.println(msg);
    while (1);
}

//------------------------------------------------------------------------------
void testRead() {
    uint8_t buf[32];

    // Initialize SD and file system.
    if (pf_mount(&fs)) errorHalt("pf_mount");

    // Open test file.
    if (pf_open("TEST.TXT")) errorHalt("pf_open");

    // Dump test file to Serial.
    while (1) {
        UINT nr;
        if (pf_read(buf, sizeof(buf), &nr)) errorHalt("pf_read");
        if (nr == 0) break;
        Serial.write(buf, nr);
    }
}

//------------------------------------------------------------------------------
void testWrite() {
    if (pf_mount(&fs)) errorHalt("pf_mount");

    if (pf_open("TEST.TXT")) errorHalt("pf_open");

    char buf[33] = "FARTPOOPFARTPOOPFARTPOOPFARTPOOP\n";

//    while (1) {
    for (int i = 0; i < 100; ++i) {
        UINT bytesWritten;
        if (pf_write(&buf, sizeof(buf), &bytesWritten)) errorHalt("pf_write");
        Serial.println(bytesWritten);
    }
//        if (bytesWritten == 0) break;
//    }

}

void setup() {
    Serial.begin(9600);
    testWrite();
//    testRead();
    Serial.println("\nDone!");
}

void loop() {}