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
#include "SensirionShdlcCommunication.h"

#include <stdint.h>
#include <stdlib.h>

#include "Arduino.h"
#include "SensirionErrors.h"
#include "SensirionShdlcRxFrame.h"
#include "SensirionShdlcTxFrame.h"

static uint16_t readByte(uint8_t& data, Stream& serial, unsigned long startTime,
                         unsigned long timeoutMicros) {
    do {
        if (micros() - startTime > timeoutMicros) {
            return static_cast<uint16_t>(ReadError) | static_cast<uint16_t>(TimeoutError);
        }
    } while (!serial.available());
    data = serial.read();
    return NoError;
}

static uint16_t unstuffByte(uint8_t& data, Stream& serial,
                            unsigned long startTime,
                            unsigned long timeoutMicros) {
    uint16_t error = readByte(data, serial, startTime, timeoutMicros);
    if (error) {
        return error;
    }
    if (data == 0x7d) {
        error = readByte(data, serial, startTime, timeoutMicros);
        if (error) {
            return error;
        }
        data = data ^ (1 << 5);
    }
    return NoError;
}

uint16_t SensirionShdlcCommunication::sendFrame(SensirionShdlcTxFrame& frame,
                                                Stream& serial) {
    size_t writtenBytes = serial.write(&frame._buffer[0], frame._index);
    if (writtenBytes != frame._index) {
        return static_cast<uint16_t>(WriteError) | static_cast<uint16_t>(SerialWriteError);
    }
    return NoError;
}

uint16_t SensirionShdlcCommunication::receiveFrame(
    SensirionShdlcRxFrame& frame, Stream& serial, unsigned long timeoutMicros) {
    unsigned long startTime = micros();
    uint16_t error;
    uint8_t dataLength;
    uint8_t current = 0;

    if (frame._numBytes) {
        return static_cast<uint16_t>(ReadError) | static_cast<uint16_t>(NonemptyFrameError);
    }

    // Wait for start byte and ignore all other bytes in case a partial frame
    // is still in the receive buffer due to a previous error.
    do {
        error = readByte(current, serial, startTime, timeoutMicros);
        if (error) {
            return error;
        }
    } while (current != 0x7e);

    // Handle a repeated start byte which may happen
    do {
        error = unstuffByte(current, serial, startTime, timeoutMicros);
        if (error) {
            return error;
        }
    } while (current == 0x7e);

    frame._address = current;
    error = unstuffByte(frame._command, serial, startTime, timeoutMicros);
    if (error) {
        return error;
    }
    error = unstuffByte(frame._state, serial, startTime, timeoutMicros);
    if (error) {
        return error;
    }
    error = unstuffByte(dataLength, serial, startTime, timeoutMicros);
    if (error) {
        return error;
    }

    uint8_t checksum =
        frame._address + frame._command + frame._state + dataLength;

    if (dataLength > frame._bufferSize) {
        return static_cast<uint16_t>(RxFrameError) | static_cast<uint16_t>(BufferSizeError);
    }

    size_t i = 0;
    while (i < dataLength) {
        error = unstuffByte(current, serial, startTime, timeoutMicros);
        if (error) {
            return error;
        }
        frame._buffer[i] = current;
        checksum += current;
        i++;
    }

    uint8_t expectedChecksum = ~checksum;
    uint8_t actualChecksum;
    error = unstuffByte(actualChecksum, serial, startTime, timeoutMicros);
    if (error) {
        return error;
    }
    if (expectedChecksum != actualChecksum) {
        return static_cast<uint16_t>(ReadError) | static_cast<uint16_t>(ChecksumError);
    }

    uint8_t stop;
    error = readByte(stop, serial, startTime, timeoutMicros);
    if (error) {
        return error;
    }
    if (stop != 0x7e) {
        return static_cast<uint16_t>(ReadError) | static_cast<uint16_t>(StopByteError);
    }
    if (frame._state & 0x7F) {
        return static_cast<uint16_t>(ExecutionError) | frame._state;
    }
    frame._dataLength = dataLength;
    frame._numBytes = dataLength;
    return NoError;
}

uint16_t SensirionShdlcCommunication::sendAndReceiveFrame(
    Stream& serial, SensirionShdlcTxFrame& txFrame,
    SensirionShdlcRxFrame& rxFrame, unsigned long rxTimeoutMicros) {
    uint16_t error;
    error = SensirionShdlcCommunication::sendFrame(txFrame, serial);
    if (error) {
        return error;
    }
    error = SensirionShdlcCommunication::receiveFrame(rxFrame, serial,
                                                      rxTimeoutMicros);
    if (error) {
        return error;
    }
    if (rxFrame.getCommand() != txFrame.getCommand()) {
        return static_cast<uint16_t>(RxFrameError) | static_cast<uint16_t>(RxCommandError);
    }
    if (rxFrame.getAddress() != txFrame.getAddress()) {
        return static_cast<uint16_t>(RxFrameError) | static_cast<uint16_t>(RxAddressError);
    }
    return NoError;
}
