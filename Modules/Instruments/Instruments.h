//
// Created by Elijah on 25/04/2017.
//

#ifndef QUADCOPTER_TESTING_INSTRUMENTS_H
#define QUADCOPTER_TESTING_INSTRUMENTS_H

#endif //QUADCOPTER_TESTING_INSTRUMENTS_H

#include <Arduino.h>
#include "SerialLogger.h"
#include "infrared.h"
#include <SoftwareSerial.h>

#define MSP_ATTITUDE 108

class Instruments {
public:
    Instruments();

    Instruments(SerialLogger *logger, uint8_t softSerialRX, uint8_t softSerialTX);

    float setPos(float pos[]);

private:
    SerialLogger *logger;
    infrared sensors;
    SoftwareSerial *mspSerial;

    void sendMSPRequest(uint8_t cmd, uint8_t *data, uint8_t n_bytes);

    void getAttitude(float attitude[]);
};