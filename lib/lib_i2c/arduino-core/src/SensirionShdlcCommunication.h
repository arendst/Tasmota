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
#ifndef SENSIRION_SHDLC_COMMUNICATION_H_
#define SENSIRION_SHDLC_COMMUNICATION_H_

#include <stdint.h>
#include <stdlib.h>

#include "Arduino.h"

#include "SensirionShdlcRxFrame.h"
#include "SensirionShdlcTxFrame.h"

class SensirionShdlcTxFrame;
class SensirionShdlcRxFrame;

/*
 * SensirionShdlcCommunication - Class which is responsible for the
 * communication via a UART (Serial) interface. It provides functionality to
 * send and receive frames from a Sensirion sensor. The data is sent and
 * received in a SensirionShdlcTxFrame or SensirionShdlcRxFrame respectively.
 */
class SensirionShdlcCommunication {

  public:
    /**
     * sendFrame() - Sends frame to sensor
     *
     * @param frame  Tx frame object containing a finished frame to send to the
     *               sensor.
     * @param serial Stream object to communicate with the sensor.
     *
     * @return       NoError on success, an error code otherwise
     */
    static uint16_t sendFrame(SensirionShdlcTxFrame& frame, Stream& serial);

    /**
     * receiveFrame() - Receive Frame from sensor
     *
     * @param frame         Rx frame to store the received data in.
     * @param serial        Stream object to communicate with the sensor.
     * @param timeoutMicros Timeout in micro seconds for the receive operation.
     *
     * @return              NoError on success, an error code otherwise
     */
    static uint16_t receiveFrame(SensirionShdlcRxFrame& frame, Stream& serial,
                                 uint32_t timeoutMicros);

    /**
     * sendAndReceiveFrame() - Send and receive a frame from sensor.
     *
     * @param serial          Stream object to communicate with the sensor.
     * @param txFrame         Tx frame object containing a finished frame to
     *                        send to the sensor.
     * @param rxFrame         Rx frame to store the received data in.
     * @param rxTimeoutMicros Timeout in micro seconds for the receive
     *                        operation.
     *
     * @return                NoError on success, an error code otherwise
     */
    static uint16_t sendAndReceiveFrame(Stream& serial,
                                        SensirionShdlcTxFrame& txFrame,
                                        SensirionShdlcRxFrame& rxFrame,
                                        uint32_t rxTimeoutMicros);
};

#endif /* SENSIRION_SHDLC_COMMUNICATION_H_ */
