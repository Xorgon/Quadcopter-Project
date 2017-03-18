#include <SoftwareSerial.h>

#define MSP_ATTITUDE 108

SoftwareSerial mspSerial(11, 12); // RX TX

void setup() {
    mspSerial.begin(9600);
    Serial.begin(9600);
}

void loop() {
    uint8_t datad = 0;
    uint8_t *data = &datad;

    sendMSP(MSP_ATTITUDE, data, 0);
    readData();
    delay(1000);
}

void sendMSP(uint8_t cmd, uint8_t *data, uint8_t n_bytes) {

    uint8_t checksum = 0;

    mspSerial.write((byte *) "$M<", 3);
    mspSerial.write(n_bytes);
    checksum ^= n_bytes;

    mspSerial.write(cmd);
    checksum ^= cmd;

    mspSerial.write(checksum);
}

void readData() {
    delay(100);

    byte count = 0;

    int16_t roll;
    int16_t pitch;
    int16_t yaw;

    while (mspSerial.available()) {
        count += 1;
        byte c = mspSerial.read();
        switch (count) {
            case 6:
                roll = c;
                break;
            case 7:
                roll <<= 8;
                roll += c;
                roll = (roll & 0xFF00) >> 8 | (roll & 0x00FF) << 8; // Reverse the order of bytes
                break;
            case 8:
                pitch += c;
                break;
            case 9:
                pitch <<= 8;
                pitch += c;
                pitch = (pitch & 0xFF00) >> 8 | (pitch & 0x00FF) << 8; // Reverse the order of bytes
                break;
            case 10:
                yaw += c;
                break;
            case 11:
                yaw <<= 8;
                yaw += c;
                yaw = (yaw & 0xFF00) >> 8 | (yaw & 0x00FF) << 8; // Reverse the order of bytes
                break;
        }
    }

    Serial.print("Roll: " + String(roll/10.0));
    Serial.print(" Pitch: " + String(pitch/10.0));
    Serial.println(" Yaw: " + String(yaw));
}