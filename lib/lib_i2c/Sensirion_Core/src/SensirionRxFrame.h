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
#ifndef SENSIRION_RX_FRAME_H_
#define SENSIRION_RX_FRAME_H_

#include <stdint.h>
#include <stdlib.h>

/**
 * @brief
 *
 */
typedef enum : uint16_t {
    Byte = 1,
    Short = 2,
    Integer = 4,
    LongInteger = 8
} IntegerType;

/**
 * SenirionRxFrame - Base class for SensirionShdlcRxFrame and
 * SensirionI2cRxFrame. It decodes received data into common data types. The
 * data is contained in a buffer which is filled by the one of the two
 * communication classes. By calling the different decode function the raw data
 * can be decoded into different data types.
 */
class SensirionRxFrame {

    friend class SensirionI2CCommunication;
    friend class SensirionShdlcCommunication;

  public:
    /**
     * Constructor
     *
     * @param buffer     Buffer in which the receive frame will be stored.
     * @param bufferSize Number of bytes in the buffer for the receive frame.
     */
    SensirionRxFrame(uint8_t buffer[], size_t bufferSize);

    /**
     * getInteger() - Read an integer value of specified type.
     *
     * If number of bytes is less than the size of the specified integer, the
     * value is padded with 0.
     *
     * @param destination Pointer to the integer number that shall be read
     * @param type  specified integer type
     * @param nrOfBytes Number of bytes read from the buffer. If nrOfBytes is
     *                  less than the size of the integer value, the value is
     *                  padded with 0.
     * @return NoError if the integer can be read from the receive buffer;
     *         Otherwise a RxFrameError is returned.
     */
    uint16_t getInteger(uint8_t* destination, IntegerType type,
                        uint8_t nrOfBytes);

    /**
     * getUInt32() - Get unsigned 32bit integer from the received data.
     *
     * @param data Memory to store unsigned 32bit integer in.
     *
     * @return     NoError on success, an error code otherwise
     */
    uint16_t getUInt32(uint32_t& data);

    /**
     * getInt32() - Get signed 32bit integer from the received data.
     *
     * @param data Memory to store signed 32bit integer in.
     *
     * @return     NoError on success, an error code otherwise
     */
    uint16_t getInt32(int32_t& data);

    /**
     * getUInt16() - Get unsigned 16bit integer from the received data.
     *
     * @param data Memory to store unsigned 16bit integer in.
     *
     * @return     NoError on success, an error code otherwise
     */
    uint16_t getUInt16(uint16_t& data);

    /**
     * getInt16() - Get signed 16bit integer from the received data.
     *
     * @param data Memory to store signed 16bit integer in.
     *
     * @return     NoError on success, an error code otherwise
     */
    uint16_t getInt16(int16_t& data);

    /**
     * getUInt8() - Get unsigned 8bit integer from the received data.
     *
     * @param data Memory to store unsigned 8bit integer in.
     *
     * @return     NoError on success, an error code otherwise
     */
    uint16_t getUInt8(uint8_t& data);

    /**
     * getInt8() - Get signed 8bit integer from the received data.
     *
     * @param data Memory to store signed 8bit integer in.
     *
     * @return     NoError on success, an error code otherwise
     */
    uint16_t getInt8(int8_t& data);

    /**
     * getBool() - Get Boolean from the received data.
     *
     * @param data Memory to store boolean in.
     *
     * @return     NoError on success, an error code otherwise
     */
    uint16_t getBool(bool& data);

    /**
     * getFloat() - Get float from the received data.
     *
     * @param data Memory to store float in.
     *
     * @return     NoError on success, an error code otherwise
     */
    uint16_t getFloat(float& data);

    /**
     * getBytes() - Get an array of bytes from the received data.
     *
     * @param data     Buffer to store the bytes in.
     * @param maxBytes Maximal amount of bytes to read from the received data.
     *
     * @return          NoError on success, an error code otherwise
     */
    uint16_t getBytes(uint8_t data[], size_t maxBytes);

  private:
    uint8_t* _buffer = 0;
    size_t _bufferSize = 0;
    size_t _index = 0;
    size_t _numBytes = 0;
};

#endif /* SENSIRION_RX_FRAME_H_ */
