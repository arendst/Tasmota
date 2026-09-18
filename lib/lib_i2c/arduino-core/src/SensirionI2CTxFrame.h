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
#ifndef SENSIRION_I2C_TX_FRAME_H_
#define SENSIRION_I2C_TX_FRAME_H_

#include <stdint.h>
#include <stdlib.h>

#include "SensirionCrc.h"
#include "SensirionI2CCommunication.h"

/*
 * SensirionI2CTxFrame - Class which helps to build a correct I2C frame for
 * Sensirion Sensors. The different addDatatype() functions add the frame data
 * and the addCommand() function writes the command at the beginning. Using
 * these functions one can easily construct a I2C frame for Sensirion sensors.
 */
class SensirionI2CTxFrame {

    friend class SensirionI2CCommunication;

  public:
    /**
     * Factory to create a SensirionI2CTxFrame using a UInt8 command.
     *
     * @param command    Command to add to the send frame.
     * @param buffer     Buffer in which the send frame will be stored.
     * @param bufferSize Number of bytes in the buffer for the send frame.
     * @param poly       CRC polynomal to use. Defaults to 0x31 with init 0xFF
     *
     * @return the constructed SensirionI2CTxFrame.
     */
    static SensirionI2CTxFrame
    createWithUInt8Command(uint8_t command, uint8_t buffer[], size_t bufferSize,
                           CrcPolynomial poly = CRC31_ff);

    /**
     * Factory to create a SensirionI2CTxFrame using a UInt16 command.
     *
     * @param command    Command to add to the send frame.
     * @param buffer     Buffer in which the send frame will be stored.
     * @param bufferSize Number of bytes in the buffer for the send frame.
     * @param poly       CRC polynomal to use. Defaults to 0x31 with init 0xFF
     *
     * @return the constructed SensirionI2CTxFrame.
     */
    static SensirionI2CTxFrame
    createWithUInt16Command(uint16_t command, uint8_t buffer[],
                            size_t bufferSize, CrcPolynomial poly = CRC31_ff);

    /**
     * Constructor
     *
     * @param buffer     Buffer in which the send frame will be stored.
     * @param bufferSize Number of bytes in the buffer for the send frame.
     * @param poly       CRC polynomal to use. Defaults to 0x31 with init 0xFF
     *
     * @deprecated Use createWithUInt16Command() instead
     */
    SensirionI2CTxFrame(uint8_t buffer[], size_t bufferSize,
                        CrcPolynomial poly = CRC31_ff);

    /**
     * addCommand() - Add command to the send frame.
     *
     * @param command Command to add to the send frame.
     *
     * @return        NoError on success, an error code otherwise
     *
     * @deprecated Use createWithUInt16Command() instead
     */
    uint16_t addCommand(uint16_t command);

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

  private:
    SensirionI2CTxFrame(uint8_t buffer[], size_t bufferSize,
                        size_t numCommandBytes, CrcPolynomial poly = CRC31_ff);

    uint16_t _addByte(uint8_t data);

    uint8_t* _buffer;
    size_t _bufferSize;
    size_t _index;
    size_t _numCommandBytes;
    CrcPolynomial _polynomial_type;
};

#endif /* SENSIRION_I2C_TX_FRAME_H_ */
