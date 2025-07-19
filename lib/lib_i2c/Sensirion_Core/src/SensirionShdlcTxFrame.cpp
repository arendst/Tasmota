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
 *   list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   software without specific prior written permission.
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

#include "SensirionShdlcTxFrame.h"

#include <stdint.h>
#include <stdlib.h>

#include "SensirionErrors.h"

uint16_t SensirionShdlcTxFrame::begin(uint8_t command, uint8_t address,
                                      uint8_t dataLength) {
    _buffer[_index++] = 0x7e;
    uint16_t error = addUInt8(address);
    error |= addUInt8(command);
    error |= addUInt8(dataLength);
    _command = command;
    _address = address;
    return error;
}

uint16_t SensirionShdlcTxFrame::finish(void) {
    uint16_t error = addUInt8(~_checksum);
    if (error) {
        return error;
    }
    if (_index + 1 > _bufferSize) {
        return  static_cast<uint16_t>(TxFrameError) | BufferSizeError;
    }
    _buffer[_index++] = 0x7e;
    _isFinished = true;
    return NoError;
}

uint16_t SensirionShdlcTxFrame::addUInt32(uint32_t data) {
    uint16_t error = addUInt8(static_cast<uint8_t>((data & 0xFF000000) >> 24));
    error |= addUInt8(static_cast<uint8_t>((data & 0x00FF0000) >> 16));
    error |= addUInt8(static_cast<uint8_t>((data & 0x0000FF00) >> 8));
    error |= addUInt8(static_cast<uint8_t>((data & 0x000000FF) >> 0));
    return error;
}

uint16_t SensirionShdlcTxFrame::addInt32(int32_t data) {
    return addUInt32(static_cast<uint32_t>(data));
}

uint16_t SensirionShdlcTxFrame::addUInt16(uint16_t data) {
    uint16_t error = addUInt8(static_cast<uint8_t>((data & 0xFF00) >> 8));
    error |= addUInt8(static_cast<uint8_t>((data & 0x00FF) >> 0));
    return error;
}

uint16_t SensirionShdlcTxFrame::addInt16(int16_t data) {
    return addUInt16(static_cast<uint16_t>(data));
}

uint16_t SensirionShdlcTxFrame::addUInt8(uint8_t data) {
    if (_index + 2 > _bufferSize) {
        return static_cast<uint16_t>(TxFrameError) | BufferSizeError;
    }
    switch (data) {
        case 0x11:
        case 0x13:
        case 0x7d:
        case 0x7e:
            // byte stuffing is done by inserting 0x7d and inverting bit 5
            _buffer[_index++] = 0x7d;
            _buffer[_index++] = data ^ (1 << 5);
            break;
        default:
            _buffer[_index++] = data;
    }
    _checksum += data;
    return NoError;
}

uint16_t SensirionShdlcTxFrame::addInt8(int8_t data) {
    return addUInt8(static_cast<uint8_t>(data));
}

uint16_t SensirionShdlcTxFrame::addBool(bool data) {
    return addUInt8(static_cast<uint8_t>(data));
}

uint16_t SensirionShdlcTxFrame::addFloat(float data) {
    union {
        uint32_t uInt32Data;
        float floatData;
    } convert;

    convert.floatData = data;
    return addUInt32(convert.uInt32Data);
}

uint16_t SensirionShdlcTxFrame::addBytes(const uint8_t data[],
                                         size_t dataLength) {
    uint16_t error = 0;
    for (size_t i = 0; i < dataLength; i++) {
        error |= addUInt8(data[i]);
    }
    return error;
}
