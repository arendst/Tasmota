/*
 * Copyright (c) 2020, Sensirion AG
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
#include "SensirionI2CCommunication.h"

#include <stdint.h>
#include <stdlib.h>

#include "Arduino.h"
#include "SensirionCrc.h"
#include "SensirionErrors.h"
#include "SensirionI2CRxFrame.h"
#include "SensirionI2CTxFrame.h"

static void clearRxBuffer(TwoWire& i2cBus) {
    while (i2cBus.available()) {
        (void)i2cBus.read();
    }
}

uint16_t SensirionI2CCommunication::sendFrame(uint8_t address,
                                              SensirionI2CTxFrame& frame,
                                              TwoWire& i2cBus) {
    i2cBus.beginTransmission(address);
    size_t writtenBytes = i2cBus.write(frame._buffer, frame._index);
    uint8_t i2c_error = i2cBus.endTransmission();
    if (writtenBytes != frame._index) {
        return static_cast<uint16_t>(WriteError) | static_cast<uint16_t>(I2cOtherError);
    }
    // translate Arduino errors, see
    // https://www.arduino.cc/en/Reference/WireEndTransmission
    switch (i2c_error) {
        case 0:
            return NoError;
        case 1:
            return static_cast<uint16_t>(WriteError) | static_cast<uint16_t>(InternalBufferSizeError);
        case 2:
            return static_cast<uint16_t>(WriteError) | static_cast<uint16_t>(I2cAddressNack);
        case 3:
            return static_cast<uint16_t>(WriteError) | static_cast<uint16_t>(I2cDataNack);
        default:
            return static_cast<uint16_t>(WriteError) | static_cast<uint16_t>(I2cOtherError);
    }
}

uint16_t SensirionI2CCommunication::receiveFrame(uint8_t address,
                                                 size_t numBytes,
                                                 SensirionI2CRxFrame& frame,
                                                 TwoWire& i2cBus,
                                                 CrcPolynomial poly) {
    size_t readAmount;
    size_t i = 0;

#ifdef I2C_BUFFER_LENGTH
    const uint8_t sizeBuffer =
        (static_cast<uint8_t>(I2C_BUFFER_LENGTH) / static_cast<uint8_t>(3)) * 3;
#elif defined(BUFFER_LENGTH)
    const uint8_t sizeBuffer =
        (static_cast<uint8_t>(BUFFER_LENGTH) / static_cast<uint8_t>(3)) * 3;
#else
    const uint8_t sizeBuffer = 30;
#endif

    if (numBytes % 3) {
        return static_cast<uint16_t>(ReadError) | static_cast<uint16_t>(WrongNumberBytesError);
    }
    if ((numBytes / 3) * 2 > frame._bufferSize) {
        return static_cast<uint16_t>(ReadError) | static_cast<uint16_t>(BufferSizeError);
    }
    if (numBytes > sizeBuffer) {
        return static_cast<uint16_t>(ReadError) | static_cast<uint16_t>(InternalBufferSizeError);
    }

    readAmount = i2cBus.requestFrom(address, static_cast<uint8_t>(numBytes),
                                    static_cast<uint8_t>(true));
    if (numBytes != readAmount) {
        return static_cast<uint16_t>(ReadError) | static_cast<uint16_t>(NotEnoughDataError);
    }
    do {
        frame._buffer[i++] = i2cBus.read();
        frame._buffer[i++] = i2cBus.read();
        uint8_t actualCRC = i2cBus.read();
        uint8_t expectedCRC = generateCRC(&frame._buffer[i - 2], 2, poly);
        if (actualCRC != expectedCRC) {
            clearRxBuffer(i2cBus);
            return static_cast<uint16_t>(ReadError) | static_cast<uint16_t>(CRCError);
        }
        readAmount -= 3;
    } while (readAmount > 0);
    frame._numBytes = i;
    return NoError;
}
