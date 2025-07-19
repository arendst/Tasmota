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
#ifndef SENSIRION_I2C_COMMUNICATION_H_
#define SENSIRION_I2C_COMMUNICATION_H_

#include <stdint.h>
#include <stdlib.h>

#include "Arduino.h"
#include "Wire.h"

#include "SensirionI2CRxFrame.h"
#include "SensirionI2CTxFrame.h"

class SensirionI2CTxFrame;
class SensirionI2CRxFrame;

/*
 * SensirionI2CCommunication - Class which is responsible for the communication
 * via a I2C bus. It provides functionality to send and receive frames from a
 * Sensirion sensor. The data is sent and received in a SensirionI2cTxFrame or
 * SensirionI2cRxFrame respectively.
 */
class SensirionI2CCommunication {
  public:
    /**
     * sendFrame() - Sends frame to sensor
     *
     * @param address I2C address of the sensor.
     * @param frame   Tx frame object containing a finished frame to send to
     *                the sensor.
     * @param i2cBus  TwoWire object to communicate with the sensor.
     *
     * @return        NoError on success, an error code otherwise
     */
    static uint16_t sendFrame(uint8_t address, SensirionI2CTxFrame& frame,
                              TwoWire& i2cBus);

    /**
     * receiveFrame() - Receive Frame from sensor
     *
     * @param address  I2C address of the sensor.
     * @param numBytes Number of bytes to receive.
     * @param frame    Rx frame to store the received data in.
     * @param i2cBus   TwoWire object to communicate with the sensor.
     * @param poly     CRC polynomal to use. Defaults to 0x31 with init 0xFF
     *
     * @return        NoError on success, an error code otherwise
     */
    static uint16_t receiveFrame(uint8_t address, size_t numBytes,
                                 SensirionI2CRxFrame& frame, TwoWire& i2cBus,
                                 CrcPolynomial poly = CRC31_ff);
};

#endif /* SENSIRION_I2C_COMMUNICATION_H_ */
