/*
 * Copyright (c) 2021, Sensirion AG
 * Copyright (c) 2023, Andrew Klaus (Removing delay functions)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "SensirionI2CSgp4x.h"
#include "Arduino.h"
#include "SensirionCore.h"
#include <Wire.h>

#define SGP4X_I2C_ADDRESS 0x59

SensirionI2CSgp4x::SensirionI2CSgp4x() {
}

void SensirionI2CSgp4x::begin(TwoWire& i2cBus) {
    _i2cBus = &i2cBus;
}

uint16_t SensirionI2CSgp4x::sendConditioningCmd(uint16_t defaultRh,
                                                uint16_t defaultT) {
    uint16_t error;
    uint8_t buffer[8];
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x2612, buffer, 8);

    error = txFrame.addUInt16(defaultRh);
    error |= txFrame.addUInt16(defaultT);
    if (error) {
        return error;
    }

    error = SensirionI2CCommunication::sendFrame(SGP4X_I2C_ADDRESS, txFrame,
                                                 *_i2cBus);
    return error;
}

uint16_t SensirionI2CSgp4x::readConditioningValue(uint16_t& srawVoc){
    // This must run at least 50ms after initiateConditioning
    uint16_t error;
    uint8_t buffer[8];

    SensirionI2CRxFrame rxFrame(buffer, 8);
    error = SensirionI2CCommunication::receiveFrame(SGP4X_I2C_ADDRESS, 3,
                                                    rxFrame, *_i2cBus);
    if (error) {
        return error;
    }

    error |= rxFrame.getUInt16(srawVoc);
    return error;
}

uint16_t SensirionI2CSgp4x::sendRawSignalsCmd(uint16_t relativeHumidity,
                                              uint16_t temperature) {
    uint16_t error;
    uint8_t buffer[8];
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x2619, buffer, 8);

    error = txFrame.addUInt16(relativeHumidity);
    error |= txFrame.addUInt16(temperature);
    if (error) {
        return error;
    }

    error = SensirionI2CCommunication::sendFrame(SGP4X_I2C_ADDRESS, txFrame,
                                                 *_i2cBus);
    return error;
}

uint16_t SensirionI2CSgp4x::readRawSignalsValue(uint16_t& srawVoc,
                                                uint16_t& srawNox) {
    // This must run 50ms after initiateRawSignals

    uint16_t error;
    uint8_t buffer[6];
    SensirionI2CRxFrame rxFrame(buffer, 6);
    error = SensirionI2CCommunication::receiveFrame(SGP4X_I2C_ADDRESS, 6,
                                                    rxFrame, *_i2cBus);
    if (error) {
        return error;
    }

    error |= rxFrame.getUInt16(srawVoc);
    error |= rxFrame.getUInt16(srawNox);
    return error;
}

uint16_t SensirionI2CSgp4x::sendRawSignalCmd(uint16_t relativeHumidity,
                                             uint16_t temperature) {
    uint16_t error;
    uint8_t buffer[8];
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x260F, buffer, 8);

    error = txFrame.addUInt16(relativeHumidity);
    error |= txFrame.addUInt16(temperature);

    if (error) {
        return error;
    }

    error = SensirionI2CCommunication::sendFrame(SGP4X_I2C_ADDRESS, txFrame,
                                                 *_i2cBus);
    return error;

}


uint16_t SensirionI2CSgp4x::readRawSignalValue(uint16_t& srawVoc) {
    uint16_t error;
    uint8_t buffer[8];

    SensirionI2CRxFrame rxFrame(buffer, 8);
    error = SensirionI2CCommunication::receiveFrame(SGP4X_I2C_ADDRESS, 3,
                                                    rxFrame, *_i2cBus);
    if (error) {
        return error;
    }

    error |= rxFrame.getUInt16(srawVoc);
    return error;

}

uint16_t SensirionI2CSgp4x::sendSelfTestCmd() {
    uint16_t error;
    uint8_t buffer[3];
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x280E, buffer, 3);

    error = SensirionI2CCommunication::sendFrame(SGP4X_I2C_ADDRESS, txFrame,
                                                 *_i2cBus);
    return error;
}

uint16_t SensirionI2CSgp4x::readSelfTestValue(uint16_t& testResult) {
    // Must run 320ms after initiateSelfTest
    uint16_t error;
    uint8_t buffer[3];

    SensirionI2CRxFrame rxFrame(buffer, 3);
    error = SensirionI2CCommunication::receiveFrame(SGP4X_I2C_ADDRESS, 3,
                                                    rxFrame, *_i2cBus);
    if (error) {
        return error;
    }

    error |= rxFrame.getUInt16(testResult);
    return error;
}

uint16_t SensirionI2CSgp4x::getFeaturesValue(uint16_t& featureResult) {
    uint16_t error;
    uint8_t buffer[3];
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x202F, buffer, 3);

    error = SensirionI2CCommunication::sendFrame(SGP4X_I2C_ADDRESS, txFrame,
                                                 *_i2cBus);

    if (error) {
        return error;
    }

    delay(1); // 1ms delay for feature request
    SensirionI2CRxFrame rxFrame(buffer, 3);
    error = SensirionI2CCommunication::receiveFrame(SGP4X_I2C_ADDRESS, 3,
                                                    rxFrame, *_i2cBus);

    error |= rxFrame.getUInt16(featureResult);
    return error;
}

uint16_t SensirionI2CSgp4x::turnHeaterOff() {
    uint16_t error;
    uint8_t buffer[2];
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x3615, buffer, 2);

    error = SensirionI2CCommunication::sendFrame(SGP4X_I2C_ADDRESS, txFrame,
                                                 *_i2cBus);
    delay(1);
    return error;
}

uint16_t SensirionI2CSgp4x::getSerialNumber(uint16_t serialNumber[],
                                            uint8_t serialNumberSize) {
    uint16_t error;
    uint8_t buffer[9];
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x3682, buffer, 9);

    error = SensirionI2CCommunication::sendFrame(SGP4X_I2C_ADDRESS, txFrame,
                                                 *_i2cBus);
    if (error) {
        return error;
    }

    delay(1);

    SensirionI2CRxFrame rxFrame(buffer, 9);
    error = SensirionI2CCommunication::receiveFrame(SGP4X_I2C_ADDRESS, 9,
                                                    rxFrame, *_i2cBus);
    if (error) {
        return error;
    }

    error |= rxFrame.getUInt16(serialNumber[0]);
    error |= rxFrame.getUInt16(serialNumber[1]);
    error |= rxFrame.getUInt16(serialNumber[2]);
    return error;
}
