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

#include "SensirionI2CTxFrame.h"

#include <stdint.h>
#include <stdlib.h>

#include "SensirionCrc.h"
#include "SensirionErrors.h"

SensirionI2CTxFrame::SensirionI2CTxFrame(uint8_t buffer[], size_t bufferSize,
                                         size_t numCommandBytes,
                                         CrcPolynomial poly)
    : _buffer(buffer), _bufferSize(bufferSize), _index(numCommandBytes),
      _numCommandBytes(numCommandBytes), _polynomial_type(poly) {
}

SensirionI2CTxFrame::SensirionI2CTxFrame(uint8_t buffer[], size_t bufferSize,
                                         CrcPolynomial poly)
    : SensirionI2CTxFrame(buffer, bufferSize, 2, poly) {
}

SensirionI2CTxFrame SensirionI2CTxFrame::createWithUInt8Command(
    uint8_t command, uint8_t buffer[], size_t bufferSize, CrcPolynomial poly) {
    SensirionI2CTxFrame instance =
        SensirionI2CTxFrame(buffer, bufferSize, 1, poly);
    instance._buffer[0] = command;
    return instance;
}

SensirionI2CTxFrame SensirionI2CTxFrame::createWithUInt16Command(
    uint16_t command, uint8_t buffer[], size_t bufferSize, CrcPolynomial poly) {
    SensirionI2CTxFrame instance =
        SensirionI2CTxFrame(buffer, bufferSize, 2, poly);
    instance._buffer[0] = static_cast<uint8_t>((command & 0xFF00) >> 8);
    instance._buffer[1] = static_cast<uint8_t>((command & 0x00FF) >> 0);
    return instance;
}

uint16_t SensirionI2CTxFrame::addCommand(uint16_t command) {
    if (_bufferSize < 2) {
        return static_cast<uint16_t>(TxFrameError) | static_cast<uint16_t>(BufferSizeError);
    }
    _buffer[0] = static_cast<uint8_t>((command & 0xFF00) >> 8);
    _buffer[1] = static_cast<uint8_t>((command & 0x00FF) >> 0);
    return NoError;
}

uint16_t SensirionI2CTxFrame::addUInt32(uint32_t data) {
    uint16_t error = _addByte(static_cast<uint8_t>((data & 0xFF000000) >> 24));
    error |= _addByte(static_cast<uint8_t>((data & 0x00FF0000) >> 16));
    error |= _addByte(static_cast<uint8_t>((data & 0x0000FF00) >> 8));
    error |= _addByte(static_cast<uint8_t>((data & 0x000000FF) >> 0));
    return error;
}

uint16_t SensirionI2CTxFrame::addInt32(int32_t data) {
    return addUInt32(static_cast<uint32_t>(data));
}

uint16_t SensirionI2CTxFrame::addUInt16(uint16_t data) {
    uint16_t error = _addByte(static_cast<uint8_t>((data & 0xFF00) >> 8));
    error |= _addByte(static_cast<uint8_t>((data & 0x00FF) >> 0));
    return error;
}

uint16_t SensirionI2CTxFrame::addInt16(int16_t data) {
    return addUInt16(static_cast<uint16_t>(data));
}

uint16_t SensirionI2CTxFrame::addUInt8(uint8_t data) {
    return _addByte(data);
}

uint16_t SensirionI2CTxFrame::addInt8(int8_t data) {
    return _addByte(static_cast<uint8_t>(data));
}

uint16_t SensirionI2CTxFrame::addBool(bool data) {
    return _addByte(static_cast<uint8_t>(data));
}

uint16_t SensirionI2CTxFrame::addFloat(float data) {
    union {
        uint32_t uInt32Data;
        float floatData;
    } convert;

    convert.floatData = data;
    return addUInt32(convert.uInt32Data);
}

uint16_t SensirionI2CTxFrame::addBytes(const uint8_t data[],
                                       size_t dataLength) {
    uint16_t error = 0;
    for (size_t i = 0; i < dataLength; i++) {
        error |= _addByte(data[i]);
    }
    return error;
}

uint16_t SensirionI2CTxFrame::_addByte(uint8_t data) {
    if (_bufferSize <= _index) {
        return static_cast<uint16_t>(TxFrameError) | static_cast<uint16_t>(BufferSizeError);
    }
    _buffer[_index++] = data;
    if ((_index - _numCommandBytes) % 3 == 2) {
        if (_bufferSize <= _index) {
            return static_cast<uint16_t>(TxFrameError) | static_cast<uint16_t>(BufferSizeError);
        }
        uint8_t crc = generateCRC(&_buffer[_index - 2], 2, _polynomial_type);
        _buffer[_index++] = crc;
    }
    return NoError;
}
