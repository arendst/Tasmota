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
#ifndef SENSIRION_SHDLC_TX_FRAME_H_
#define SENSIRION_SHDLC_TX_FRAME_H_

#include <stdint.h>
#include <stdlib.h>

#include "SensirionShdlcCommunication.h"

/*
 * SensirionShdlcTxFrame - Class which helps to build a correct SHDLC frame.
 * The begin() functions writes the header. The different addDatatype()
 * functions add the frame data and the finish() function writes the tail.
 * Using these functions one can easily construct a SHDLC frame.
 */
class SensirionShdlcTxFrame {

    friend class SensirionShdlcCommunication;

  public:
    /**
     * Constructor
     *
     * @param buffer     Buffer in which the send frame will be stored.
     * @param bufferSize Number of bytes in the buffer for the send frame.
     */
    SensirionShdlcTxFrame(uint8_t buffer[], size_t bufferSize)
        : _buffer(buffer), _bufferSize(bufferSize) {
    }

    /**
     * begin() - Begin frame and write header.
     *
     * @note This function needs to be called before calling other
     *       data add functions to write the header at the beginning.
     *
     * @param command    Command byte to add to the send frame.
     * @param address    Address byte to add to the send frame.
     * @param dataLength Data length byte to add to the send frame.
     *
     * @return           NoError on success, an error code otherwise
     */
    uint16_t begin(uint8_t command, uint8_t address, uint8_t dataLength);

    /**
     * finish() - Finish frame and write tail.
     *
     * @note This function needs to be called last, after adding all
     *       data to frame and before sending the frame to the sensor.
     *
     * @return NoError on success, an error code otherwise
     */
    uint16_t finish(void);

    /**
     * addUInt32() - Add unsigned 32bit integer to the send frame.
     *
     * @param data Unsigned 32bit integer to add to the send frame.
     *
     * @return     NoError on success, an error code otherwise
     */
    uint16_t addUInt32(uint32_t data);

    /**
     * addInt32() - Add signed 32bit integer to the send frame.
     *
     * @param data Signed 32bit integer to add to the send frame.
     *
     * @return     NoError on success, an error code otherwise
     */
    uint16_t addInt32(int32_t data);

    /**
     * addUInt16() - Add unsigned 16bit integer to the send frame.
     *
     * @param data Unsigned 16bit integer to add to the send frame.
     *
     * @return     NoError on success, an error code otherwise
     */
    uint16_t addUInt16(uint16_t data);

    /**
     * addInt16() - Add signed 16bit integer to the send frame.
     *
     * @param data Signed 16bit integer to add to the send frame.
     *
     * @return     NoError on success, an error code otherwise
     */
    uint16_t addInt16(int16_t data);

    /**
     * addUInt8() - Add unsigned 8bit integer to the send frame.
     *
     * @param data Unsigned 8bit integer to add to the send frame.
     *
     * @return     NoError on success, an error code otherwise
     */
    uint16_t addUInt8(uint8_t data);

    /**
     * addInt8() - Add signed 8bit integer to the send frame.
     *
     * @param data Signed 8bit integer to add to the send frame.
     *
     * @return     NoError on success, an error code otherwise
     */
    uint16_t addInt8(int8_t data);

    /**
     * addBool() - Add boolean to the send frame.
     *
     * @param data Boolean to add to the send frame.
     *
     * @return     NoError on success, an error code otherwise
     */
    uint16_t addBool(bool data);

    /**
     * addFloat() - Add float to the send frame.
     *
     * @param data Float to add to the send frame.
     *
     * @return     NoError on success, an error code otherwise
     */
    uint16_t addFloat(float data);

    /**
     * addBytes() - Add byte array to the send frame.
     *
     * @param data       Byte array to add to the send frame.
     * @param dataLength Number of bytes to add to the send frame.
     *
     * @return           NoError on success, an error code otherwise
     */
    uint16_t addBytes(const uint8_t data[], size_t dataLength);

    uint8_t getCommand(void) const {
        return _command;
    };

    uint8_t getAddress(void) const {
        return _address;
    }

  private:
    uint8_t* _buffer;
    size_t _bufferSize;
    size_t _index = 0;
    uint8_t _checksum = 0;
    bool _isFinished = false;
    uint8_t _command = 0;
    uint8_t _address = 0;
};

#endif /* SENSIRION_SHDLC_TX_FRAME_H_ */
