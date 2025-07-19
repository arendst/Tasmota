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
#ifndef SENSIRION_SHDLC_RX_FRAME_H_
#define SENSIRION_SHDLC_RX_FRAME_H_

#include <stdint.h>
#include <stdlib.h>

#include "SensirionRxFrame.h"
#include "SensirionShdlcCommunication.h"

/**
 * SenirionShdlcRxFrame - Class which decodes the through UART received data
 * into common data types. It contains a buffer which is filled by the
 * SensirionShdlcCommunication class. By calling the different decode function
 * inherited from the SensirionRxFrame base class the raw data can be decoded
 * into different data types. In addition to that it also stores the four
 * header bytes defined by the SHDLC protocol state, command, address,
 * datalength. These bytes can be read out by the corresponding getter method.
 */
class SensirionShdlcRxFrame : public SensirionRxFrame {

    friend class SensirionShdlcCommunication;

  public:
    /**
     * Constructor
     *
     * @param buffer     Buffer in which the receive frame will be stored.
     * @param bufferSize Number of bytes in the buffer for the receive frame.
     */
    SensirionShdlcRxFrame(uint8_t buffer[], size_t bufferSize)
        : SensirionRxFrame(buffer, bufferSize){};

    uint8_t getAddress(void) const {
        return _address;
    };

    uint8_t getCommand(void) const {
        return _command;
    };

    uint8_t getState(void) const {
        return _state;
    };

    uint8_t getDataLength(void) const {
        return _dataLength;
    };

  private:
    uint8_t _address = 0;
    uint8_t _command = 0;
    uint8_t _state = 0;
    uint8_t _dataLength = 0;
};

#endif /* SENSIRION_SHDLC_RX_FRAME_H_ */
