//
// Created by Elijah on 25/04/2017.
//

#include "Instruments.h"

/**
 * Allows for declaration of an Instruments variable without initialization.
 */
Instruments::Instruments() {}

/**
 * Initializes the Instruments object.
 * @param logger Pointer to a SerialLogger.
 * @param softSerialRX MSP Software Serial RX pin.
 * @param softSerialTX MSP Software Serial TX pin.
 */
Instruments::Instruments(SerialLogger *logger, uint8_t softSerialRX, uint8_t softSerialTX) {
    this->logger = logger;
    mspSerial = new AltSoftSerial(softSerialRX, softSerialTX);
    mspSerial->begin(9600);
    pinMode(13, OUTPUT);
}

/**
 * Sets the correct values for the position in the provided array.
 * @param pos Position array.
 * @return yaw float.
 */
float Instruments::setPos(float *pos) {

    // Create attitude array to be set by getAttitude.
    float attitude[3];
    bool valid = false;

    // Turn MSP communications status LED on.
    digitalWrite(13, HIGH);

    while (!valid) {
        // Send MSP Request:
        uint8_t data = 0;
        sendMSPRequest(MSP_ATTITUDE, &data, 0);

        // Wait for a response from the flight controller.
        while (!mspSerial->available()) {}

        // Read the response from the flight controller.
        getAttitude(attitude);

        // Verify that the values are within the correct ranges.
        if (attitude[0] < 180 && attitude[0] > -180
            && attitude[1] < 180 && attitude[1] > -180
            && attitude[2] <= 360 && attitude[2] >= 0) {
            if (attitude[0] != 0 && attitude[1] != 0 && attitude[2] != 0) {
                valid = true;
            }
        } else {
            // Log an error if the values are invalid.
            logger->log(F("ERROR"), "R=" + String(attitude[0])
                                    + ", P=" + String(attitude[1])
                                    + ", Y=" + String(attitude[2]));
        }
    }

    // Turn MSP communications LED off.
    digitalWrite(13, LOW);

    // Log attitude data (Roll, Pitch, Yaw).
    String logData = "R=" + String(attitude[0])
                     + ", P=" + String(attitude[1])
                     + ", Y=" + String(attitude[2]);
    logger->log(INSTRUMENTS_LOGGER_TAG, logData);

    // Create an infrared object.
    infrared sensor = infrared(logger);

    // Pass attitude data to the infrared object.
    sensor.setAngle(attitude[1], attitude[0], attitude[2]);

    // Calculate and retrieve distances.
    sensor.Calculate();
    pos[0] = sensor.GetDistanceX() / 100.0;
    pos[1] = sensor.GetDistanceY() / 100.0;
    pos[2] = sensor.GetDistanceZ() / 100.0;

    // Log calculated distances.
    logData = "x=" + String(pos[0]) + ", y=" + String(pos[1]) + ", z=" + String(pos[2]);
    logger->log(INSTRUMENTS_LOGGER_TAG, logData);

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

    mspSerial->write((byte *) "$M<", 3); // Send start of preamble.
    mspSerial->write(n_bytes); // Send the number of bytes to be sent.
    checksum ^= n_bytes; // Start generating checksum.

    mspSerial->write(cmd); // Send command.
    checksum ^= cmd; // Finish checksum.

    mspSerial->write(checksum);   // Send checksum.
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
        // Counter for bytes.
        count += 1;
        byte c = mspSerial->read();
        switch (count) {
            // Byte 6 is the second byte of the 16 bit roll integer.
            case 6:
                roll = c;
                break;

                // Byte 7 is the first byte of the 16 bit roll integer.
            case 7:
                roll <<= 8; // Move the existing byte into the first byte.
                roll += c; // Add the new byte.
                roll = (roll & 0xFF00) >> 8 | (roll & 0x00FF) << 8; // Reverse the order of bytes.
                break;

                // Byte 8 is the second byte of the 16 bit pitch integer.
            case 8:
                pitch += c;
                break;

                // Byte 9 is the first byte of the 16 bit pitch integer.
            case 9:
                pitch <<= 8; // Move the existing byte into the first byte.
                pitch += c; // Add the new byte.
                pitch = (pitch & 0xFF00) >> 8 | (pitch & 0x00FF) << 8; // Reverse the order of bytes
                break;

                // Byte 10 is the second byte of the 16 bit yaw integer.
            case 10:
                yaw += c;
                break;

                // Byte 11 is the is the first byte of the 16 bit yaw integer.
            case 11:
                yaw <<= 8; // Move the existing byte into the first byte.
                yaw += c; // Add the new byte.
                yaw = (yaw & 0xFF00) >> 8 | (yaw & 0x00FF) << 8; // Reverse the order of bytes
                break;
        }
    }

    // Correct units to degrees and set attitude array members.
    attitude[0] = roll / 10.0;
    attitude[1] = pitch / 10.0;
    attitude[2] = yaw;
}