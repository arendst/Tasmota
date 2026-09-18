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
#ifndef _SENSIRION_ERRORS_H_
#define _SENSIRION_ERRORS_H_

#include <stdint.h>
#include <stdlib.h>

enum HighLevelError : uint16_t {
    // general errors
    NoError = 0,
    WriteError = 0x0100,
    ReadError = 0x0200,
    TxFrameError = 0x0300,
    RxFrameError = 0x0400,
    // shdlc errors
    ExecutionError = 0x0500,
    // i2c errors

    // Sensor specific errors. All errors higher than that are depending on the
    // sensor used.
    SensorSpecificError = 0x8000,
};

enum LowLevelError : uint8_t {
    // general errors
    Undefined = 0,
    NonemptyFrameError,
    NoDataError,
    BufferSizeError,
    // shdlc errors
    StopByteError,
    ChecksumError,
    TimeoutError,
    RxCommandError,
    RxAddressError,
    SerialWriteError,
    // i2c errors
    WrongNumberBytesError,
    CRCError,
    I2cAddressNack,
    I2cDataNack,
    I2cOtherError,
    NotEnoughDataError,
    InternalBufferSizeError,
};

/**
 * errorToString() - Convert error code to a human readable error message
 *
 * @param error            Error code to be converted.
 * @param errorMessage     String where the error text can be
 *                         stored.
 * @param errorMessageSize Size in bytes of the string buffer for the error
 *                         message.
 */
void errorToString(uint16_t error, char errorMessage[],
                   size_t errorMessageSize);

#endif /* _SENSIRION_ERRORS_H_ */
