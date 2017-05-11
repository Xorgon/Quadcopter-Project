//
// Created by Elijah on 25/04/2017.
//

#include "Instruments.h"

Instruments::Instruments() {}

Instruments::Instruments(SerialLogger *logger, uint8_t softSerialRX, uint8_t softSerialTX) {
    this->logger = logger;
    mspSerial = new SoftwareSerial(softSerialRX, softSerialTX);
    mspSerial->begin(57600);
    pinMode(13, OUTPUT);
}

/**
 * Sets the correct values for the position in the provided array.
 * @param pos Position array.
 * @return yaw float.
 */
float Instruments::setPos(float *pos) {

    // Send MSP Request:
    uint8_t data = 0;
    sendMSPRequest(MSP_ATTITUDE, &data, 0);
    digitalWrite(13, HIGH);
    while (!mspSerial->available()) {}
    digitalWrite(13, LOW);
    float attitude[3];

    bool valid = false;

    // Avoid any huge attitude values.
    while (!valid) {
        getAttitude(attitude);

        if (attitude[0] < 180 && attitude[0] > -180
                && attitude[1] < 180 && attitude[1] > -180
                && attitude[3] <= 360 && attitude[3] >= 0) {
            valid = true;
        }
    }
    String logData = "Roll= " + String(attitude[0])
                     + ", Pitch= " + String(attitude[1])
                     + ", Yaw= " + String(attitude[2]);
    logger->log("Instruments", logData);

    infrared sensor = infrared(logger);
    sensor.setAngle(attitude[1], attitude[0], attitude[2]);
    sensor.Calculate();

    pos[0] = sensor.GetDistanceX()/100.0;
    pos[1] = sensor.GetDistanceY()/100.0;
    pos[2] = sensor.GetDistanceZ()/100.0;

    logData = "x= " + String(pos[0]) + ", y= " + String(pos[1]) + ", z= " + String(pos[2]);
    logger->log("Instruments", logData);

    return attitude[2];
}

/**
 * Sends a MultiWii Serial Protocol Request.
 *
 * @param cmd Request code.
 * @param data Request data.
 * @param n_bytes Number of bytes in data.
 */
void Instruments::sendMSPRequest(uint8_t cmd, uint8_t *data, uint8_t n_bytes) {

    uint8_t checksum = 0;

    mspSerial->write((byte *) "$M<", 3);
    mspSerial->write(n_bytes);
    checksum ^= n_bytes;

    mspSerial->write(cmd);
    checksum ^= cmd;

    mspSerial->write(checksum);
}

/**
 * sendMSPRequest() must be invoked before getAttitude().
 *
 * @return Arrray of roll, pitch, yaw
 */
void Instruments::getAttitude(float *attitude) {
    byte count = 0;

    int16_t roll;
    int16_t pitch;
    int16_t yaw;

    while (mspSerial->available()) {
        count += 1;
        byte c = mspSerial->read();
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

    attitude[0] = roll/10.0;
    attitude[1] = pitch/10.0;
    attitude[2] = yaw;
}