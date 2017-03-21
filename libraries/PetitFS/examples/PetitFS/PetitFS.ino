// Petit FS test.   
// For minimum flash use edit pffconfig.h and only enable
// _USE_READ and either _FS_FAT16 or _FS_FAT32

#include "PetitFS.h"
#include "PetitSerial.h"

uint32_t charsWritten;

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
    while (pf_mount(&fs)) {
        Serial.println("Error: pf_mount");
    }

    if (pf_open("RAW.QFL")) errorHalt("pf_open");

    pf_lseek(2);

    char buf[] = "Write test\n";

    for (int i = 0; i < 1; ++i) {
        UINT bytesWritten;
        if (pf_write(&buf, sizeof(buf), &bytesWritten)) errorHalt("pf_write");
        charsWritten += bytesWritten;
        Serial.println(bytesWritten);
    }
    uint16_t fillChars = 1024 - charsWritten % 1024;
    char fill[fillChars];
    for (int j = 0; j < fillChars; ++j) {
        if (j == fillChars - 1) {
            Serial.println("YUP");
            fill[j] = "v";
        } else {
            fill[j] = "X";
        }
    }
    UINT bytesWritten;
    if (pf_write(&fill, sizeof(fill), &bytesWritten)) errorHalt("fill write");
//    if (pf_lseek(fs.fptr + 1)) errorHalt("pf_lseek"); // TODO: Test this for double write test.
}

void setup() {
    Serial.begin(9600);
    charsWritten = 0;
    testWrite();
//    testRead();
    Serial.println("\nDone!");
}

void loop() {}