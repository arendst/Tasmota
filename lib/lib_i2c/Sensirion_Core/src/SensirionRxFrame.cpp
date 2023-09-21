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

#include "SensirionRxFrame.h"

#include <stdint.h>
#include <stdlib.h>

#include "SensirionErrors.h"

SensirionRxFrame::SensirionRxFrame(uint8_t buffer[], size_t bufferSize)
    : _buffer(buffer), _bufferSize(bufferSize), _index(0), _numBytes(0) {
}

uint16_t SensirionRxFrame::getUInt32(uint32_t& data) {
    if (_numBytes < 4) {
        return static_cast<uint16_t>(RxFrameError) | static_cast<uint16_t>(NoDataError);
    }
    data = static_cast<uint32_t>(_buffer[_index++]) << 24;
    data |= static_cast<uint32_t>(_buffer[_index++]) << 16;
    data |= static_cast<uint32_t>(_buffer[_index++]) << 8;
    data |= static_cast<uint32_t>(_buffer[_index++]);
    _numBytes -= 4;
    return NoError;
}

uint16_t SensirionRxFrame::getInt32(int32_t& data) {
    uint32_t ret;
    uint16_t error = getUInt32(ret);
    data = static_cast<int32_t>(ret);
    return error;
}

uint16_t SensirionRxFrame::getUInt16(uint16_t& data) {
    if (_numBytes < 2) {
        return static_cast<uint16_t>(RxFrameError) | static_cast<uint16_t>(NoDataError);
    }
    data = static_cast<uint16_t>(_buffer[_index++]) << 8;
    data |= static_cast<uint16_t>(_buffer[_index++]);
    _numBytes -= 2;
    return NoError;
}

uint16_t SensirionRxFrame::getInt16(int16_t& data) {
    uint16_t ret;
    uint16_t error = getUInt16(ret);
    data = static_cast<int16_t>(ret);
    return error;
}

uint16_t SensirionRxFrame::getUInt8(uint8_t& data) {
    if (_numBytes < 1) {
        return static_cast<uint16_t>(RxFrameError) | static_cast<uint16_t>(NoDataError);
    }
    data = _buffer[_index++];
    _numBytes -= 1;
    return NoError;
}

uint16_t SensirionRxFrame::getInt8(int8_t& data) {
    if (_numBytes < 1) {
        return static_cast<uint16_t>(RxFrameError) | static_cast<uint16_t>(NoDataError);
    }
    data = static_cast<int8_t>(_buffer[_index++]);
    _numBytes -= 1;
    return NoError;
}

uint16_t SensirionRxFrame::getBool(bool& data) {
    if (_numBytes < 1) {
        return static_cast<uint16_t>(RxFrameError) | static_cast<uint16_t>(NoDataError);
    }
    data = static_cast<bool>(_buffer[_index++]);
    _numBytes -= 1;
    return NoError;
}

uint16_t SensirionRxFrame::getFloat(float& data) {
    union {
        uint32_t uInt32Data;
        float floatData;
    } convert;
    uint16_t error = getUInt32(convert.uInt32Data);
    data = convert.floatData;
    return error;
}

uint16_t SensirionRxFrame::getBytes(uint8_t data[], size_t maxBytes) {
    if (_numBytes < 1) {
        return static_cast<uint16_t>(RxFrameError) | static_cast<uint16_t>(NoDataError);
    }
    size_t readAmount = maxBytes;
    if (_numBytes < maxBytes) {
        readAmount = _numBytes;
    }
    for (size_t i = 0; i < readAmount; i++) {
        data[i] = _buffer[_index++];
    }
    _numBytes -= readAmount;
    return NoError;
}
