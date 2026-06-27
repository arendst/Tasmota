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
#include "SensirionErrors.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

void errorToString(uint16_t error, char errorMessage[],
                   size_t errorMessageSize) {

    uint16_t highLevelError = error & 0xFF00;
    uint16_t lowLevelError = error & 0x00FF;

    if (error & HighLevelError::SensorSpecificError) {
        snprintf(errorMessage, errorMessageSize, "Sensor specific error: 0x%2x",
                 lowLevelError);
        return;
    }

    switch (highLevelError) {
        case HighLevelError::NoError:
            if (!error) {
                strncpy(errorMessage, "No error", errorMessageSize);
                return;
            }
            break;
        case HighLevelError::WriteError:
            switch (lowLevelError) {
                case LowLevelError::Undefined:
                    strncpy(errorMessage, "Write error",
                            errorMessageSize);
                    return;
                case LowLevelError::SerialWriteError:
                    strncpy(errorMessage, "Error writing to serial",
                            errorMessageSize);
                    return;
                case LowLevelError::InternalBufferSizeError:
                    strncpy(errorMessage,
                            "Data too long to fit in transmit buffer",
                            errorMessageSize);
                    return;
                case LowLevelError::I2cAddressNack:
                    strncpy(errorMessage,
                            "Received NACK on transmit of address",
                            errorMessageSize);
                    return;
                case LowLevelError::I2cDataNack:
                    strncpy(errorMessage, "Received NACK on transmit of data",
                            errorMessageSize);
                    return;
                case LowLevelError::I2cOtherError:
                    strncpy(errorMessage, "Error writing to I2C bus",
                            errorMessageSize);
                    return;
            }
            break;
        case HighLevelError::ReadError:
            switch (lowLevelError) {
                case LowLevelError::Undefined:
                    strncpy(errorMessage, "Read error", errorMessageSize);
                    return;
                case LowLevelError::NonemptyFrameError:
                    strncpy(errorMessage, "Frame already contains data",
                            errorMessageSize);
                    return;
                case LowLevelError::TimeoutError:
                    strncpy(errorMessage, "Timeout while reading data",
                            errorMessageSize);
                    return;
                case LowLevelError::ChecksumError:
                    strncpy(errorMessage, "Checksum is wrong",
                            errorMessageSize);
                    return;
                case LowLevelError::CRCError:
                    strncpy(errorMessage, "Wrong CRC found", errorMessageSize);
                    return;
                case LowLevelError::WrongNumberBytesError:
                    strncpy(errorMessage, "Number of bytes not a multiple of 3",
                            errorMessageSize);
                    return;
                case LowLevelError::NotEnoughDataError:
                    strncpy(errorMessage, "Not enough data received",
                            errorMessageSize);
                    return;
                case LowLevelError::InternalBufferSizeError:
                    strncpy(errorMessage, "Internal I2C buffer too small",
                            errorMessageSize);
                    return;
            }
            break;
        case HighLevelError::ExecutionError: {
            char format[] = "Execution error, status register: 0x%x";
            snprintf(errorMessage, errorMessageSize, format, lowLevelError);
            return;
        }
        case HighLevelError::TxFrameError:
            switch (lowLevelError) {
                case LowLevelError::Undefined:
                    strncpy(errorMessage, "Tx frame error", errorMessageSize);
                    return;
                case LowLevelError::BufferSizeError:
                    strncpy(errorMessage, "Not enough space in buffer",
                            errorMessageSize);
                    return;
            }
            break;
        case HighLevelError::RxFrameError:
            switch (lowLevelError) {
                case LowLevelError::Undefined:
                    strncpy(errorMessage, "Rx frame error", errorMessageSize);
                    return;
                case LowLevelError::BufferSizeError:
                    strncpy(errorMessage, "Not enough space in buffer",
                            errorMessageSize);
                    return;
                case LowLevelError::NoDataError:
                    strncpy(errorMessage, "No more data in frame",
                            errorMessageSize);
                    return;
                case LowLevelError::RxAddressError:
                    strncpy(errorMessage, "Wrong address in return frame",
                            errorMessageSize);
                    return;
                case LowLevelError::RxCommandError:
                    strncpy(errorMessage, "Wrong command in return frame",
                            errorMessageSize);
                    return;
            }
    }
    strncpy(errorMessage, "Error processing error", errorMessageSize);
    return;
}
