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
#ifndef SENSIRION_I2C_RX_FRAME_H_
#define SENSIRION_I2C_RX_FRAME_H_

#include <stdint.h>
#include <stdlib.h>

#include "SensirionI2CCommunication.h"
#include "SensirionRxFrame.h"

/**
 * SenirionI2CRxFrame - Class which decodes the through I2C received data into
 * common data types. It contains a buffer which is filled by the
 * SensirionI2CCommunication class. By calling the different decode function
 * inherited from the SensirionRxFrame base class the raw data can be decoded
 * into different data types.
 */
class SensirionI2CRxFrame : public SensirionRxFrame {

    friend class SensirionI2CCommunication;

  public:
    /**
     * Constructor
     *
     * @param buffer     Buffer in which the receive frame will be
     *                   stored.
     * @param bufferSize Number of bytes in the buffer for the receive frame.
     *
     */
    SensirionI2CRxFrame(uint8_t buffer[], size_t bufferSize)
        : SensirionRxFrame(buffer, bufferSize){};
};

#endif /* SENSIRION_I2C_RX_FRAME_H_ */
