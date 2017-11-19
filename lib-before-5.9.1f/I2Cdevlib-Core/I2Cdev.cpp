// I2Cdev library collection - Main I2C device class
// Abstracts bit and byte I2C R/W functions into a convenient class
// 2013-06-05 by Jeff Rowberg <jeff@rowberg.net>
//
// Changelog:
//      2013-05-06 - add Francesco Ferrara's Fastwire v0.24 implementation with small modifications
//      2013-05-05 - fix issue with writing bit values to words (Sasquatch/Farzanegan)
//      2012-06-09 - fix major issue with reading > 32 bytes at a time with Arduino Wire
//                 - add compiler warnings when using outdated or IDE or limited I2Cdev implementation
//      2011-11-01 - fix write*Bits mask calculation (thanks sasquatch @ Arduino forums)
//      2011-10-03 - added automatic Arduino version detection for ease of use
//      2011-10-02 - added Gene Knight's NBWire TwoWire class implementation with small modifications
//      2011-08-31 - added support for Arduino 1.0 Wire library (methods are different from 0.x)
//      2011-08-03 - added optional timeout parameter to read* methods to easily change from default
//      2011-08-02 - added support for 16-bit registers
//                 - fixed incorrect Doxygen comments on some methods
//                 - added timeout value for read operations (thanks mem @ Arduino forums)
//      2011-07-30 - changed read/write function structures to return success or byte counts
//                 - made all methods static for multi-device memory savings
//      2011-07-28 - initial release

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2013 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#include "I2Cdev.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE

    #ifdef I2CDEV_IMPLEMENTATION_WARNINGS
        #if ARDUINO < 100
            #warning Using outdated Arduino IDE with Wire library is functionally limiting.
            #warning Arduino IDE v1.6.5+ with I2Cdev Fastwire implementation is recommended.
            #warning This I2Cdev implementation does not support:
            #warning - Repeated starts conditions
            #warning - Timeout detection (some Wire requests block forever)
        #elif ARDUINO == 100
            #warning Using outdated Arduino IDE with Wire library is functionally limiting.
            #warning Arduino IDE v1.6.5+ with I2Cdev Fastwire implementation is recommended.
            #warning This I2Cdev implementation does not support:
            #warning - Repeated starts conditions
            #warning - Timeout detection (some Wire requests block forever)
        #elif ARDUINO > 100
            /*#warning Using current Arduino IDE with Wire library is functionally limiting.
            #warning Arduino IDE v1.6.5+ with I2CDEV_BUILTIN_FASTWIRE implementation is recommended.
            #warning This I2Cdev implementation does not support:
            #warning - Timeout detection (some Wire requests block forever)*/
        #endif
    #endif

#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE

    //#error The I2CDEV_BUILTIN_FASTWIRE implementation is known to be broken right now. Patience, Iago!

#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_NBWIRE

    #ifdef I2CDEV_IMPLEMENTATION_WARNINGS
        #warning Using I2CDEV_BUILTIN_NBWIRE implementation may adversely affect interrupt detection.
        #warning This I2Cdev implementation does not support:
        #warning - Repeated starts conditions
    #endif

    // NBWire implementation based heavily on code by Gene Knight <Gene@Telobot.com>
    // Originally posted on the Arduino forum at http://arduino.cc/forum/index.php/topic,70705.0.html
    // Originally offered to the i2cdevlib project at http://arduino.cc/forum/index.php/topic,68210.30.html
    TwoWire Wire;

#endif

/** Default constructor.
 */
I2Cdev::I2Cdev() {
}

/** Read a single bit from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitNum Bit position to read (0-7)
 * @param data Container for single bit value
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
 * @return Status of read operation (true = success)
 */
int8_t I2Cdev::readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data, uint16_t timeout) {
    uint8_t b;
    uint8_t count = readByte(devAddr, regAddr, &b, timeout);
    *data = b & (1 << bitNum);
    return count;
}

/** Read a single bit from a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitNum Bit position to read (0-15)
 * @param data Container for single bit value
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
 * @return Status of read operation (true = success)
 */
int8_t I2Cdev::readBitW(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t *data, uint16_t timeout) {
    uint16_t b;
    uint8_t count = readWord(devAddr, regAddr, &b, timeout);
    *data = b & (1 << bitNum);
    return count;
}

/** Read multiple bits from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitStart First bit position to read (0-7)
 * @param length Number of bits to read (not more than 8)
 * @param data Container for right-aligned value (i.e. '101' read from any bitStart position will equal 0x05)
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
 * @return Status of read operation (true = success)
 */
int8_t I2Cdev::readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data, uint16_t timeout) {
    // 01101001 read byte
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    //    010   masked
    //   -> 010 shifted
    uint8_t count, b;
    if ((count = readByte(devAddr, regAddr, &b, timeout)) != 0) {
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        b &= mask;
        b >>= (bitStart - length + 1);
        *data = b;
    }
    return count;
}

/** Read multiple bits from a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitStart First bit position to read (0-15)
 * @param length Number of bits to read (not more than 16)
 * @param data Container for right-aligned value (i.e. '101' read from any bitStart position will equal 0x05)
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
 * @return Status of read operation (1 = success, 0 = failure, -1 = timeout)
 */
int8_t I2Cdev::readBitsW(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t *data, uint16_t timeout) {
    // 1101011001101001 read byte
    // fedcba9876543210 bit numbers
    //    xxx           args: bitStart=12, length=3
    //    010           masked
    //           -> 010 shifted
    uint8_t count;
    uint16_t w;
    if ((count = readWord(devAddr, regAddr, &w, timeout)) != 0) {
        uint16_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        w &= mask;
        w >>= (bitStart - length + 1);
        *data = w;
    }
    return count;
}

/** Read single byte from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param data Container for byte value read from device
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
 * @return Status of read operation (true = success)
 */
int8_t I2Cdev::readByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t timeout) {
    return readBytes(devAddr, regAddr, 1, data, timeout);
}

/** Read single word from a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param data Container for word value read from device
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
 * @return Status of read operation (true = success)
 */
int8_t I2Cdev::readWord(uint8_t devAddr, uint8_t regAddr, uint16_t *data, uint16_t timeout) {
    return readWords(devAddr, regAddr, 1, data, timeout);
}

/** Read multiple bytes from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr First register regAddr to read from
 * @param length Number of bytes to read
 * @param data Buffer to store read data in
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
 * @return Number of bytes read (-1 indicates failure)
 */
int8_t I2Cdev::readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data, uint16_t timeout) {
    #ifdef I2CDEV_SERIAL_DEBUG
        Serial.print("I2C (0x");
        Serial.print(devAddr, HEX);
        Serial.print(") reading ");
        Serial.print(length, DEC);
        Serial.print(" bytes from 0x");
        Serial.print(regAddr, HEX);
        Serial.print("...");
    #endif

    int8_t count = 0;
    uint32_t t1 = millis();

    #if (I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE)

        #if (ARDUINO < 100)
            // Arduino v00xx (before v1.0), Wire library

            // I2C/TWI subsystem uses internal buffer that breaks with large data requests
            // so if user requests more than BUFFER_LENGTH bytes, we have to do it in
            // smaller chunks instead of all at once
            for (uint8_t k = 0; k < length; k += min(length, BUFFER_LENGTH)) {
                Wire.beginTransmission(devAddr);
                Wire.send(regAddr);
                Wire.endTransmission();
                Wire.beginTransmission(devAddr);
                Wire.requestFrom(devAddr, (uint8_t)min(length - k, BUFFER_LENGTH));

                for (; Wire.available() && (timeout == 0 || millis() - t1 < timeout); count++) {
                    data[count] = Wire.receive();
                    #ifdef I2CDEV_SERIAL_DEBUG
                        Serial.print(data[count], HEX);
                        if (count + 1 < length) Serial.print(" ");
                    #endif
                }

                Wire.endTransmission();
            }
        #elif (ARDUINO == 100)
            // Arduino v1.0.0, Wire library
            // Adds standardized write() and read() stream methods instead of send() and receive()

            // I2C/TWI subsystem uses internal buffer that breaks with large data requests
            // so if user requests more than BUFFER_LENGTH bytes, we have to do it in
            // smaller chunks instead of all at once
            for (uint8_t k = 0; k < length; k += min(length, BUFFER_LENGTH)) {
                Wire.beginTransmission(devAddr);
                Wire.write(regAddr);
                Wire.endTransmission();
                Wire.beginTransmission(devAddr);
                Wire.requestFrom(devAddr, (uint8_t)min(length - k, BUFFER_LENGTH));
        
                for (; Wire.available() && (timeout == 0 || millis() - t1 < timeout); count++) {
                    data[count] = Wire.read();
                    #ifdef I2CDEV_SERIAL_DEBUG
                        Serial.print(data[count], HEX);
                        if (count + 1 < length) Serial.print(" ");
                    #endif
                }
        
                Wire.endTransmission();
            }
        #elif (ARDUINO > 100)
            // Arduino v1.0.1+, Wire library
            // Adds official support for repeated start condition, yay!

            // I2C/TWI subsystem uses internal buffer that breaks with large data requests
            // so if user requests more than BUFFER_LENGTH bytes, we have to do it in
            // smaller chunks instead of all at once
            for (uint8_t k = 0; k < length; k += min(length, BUFFER_LENGTH)) {
                Wire.beginTransmission(devAddr);
                Wire.write(regAddr);
                Wire.endTransmission();
                Wire.beginTransmission(devAddr);
                Wire.requestFrom(devAddr, (uint8_t)min(length - k, BUFFER_LENGTH));
        
                for (; Wire.available() && (timeout == 0 || millis() - t1 < timeout); count++) {
                    data[count] = Wire.read();
                    #ifdef I2CDEV_SERIAL_DEBUG
                        Serial.print(data[count], HEX);
                        if (count + 1 < length) Serial.print(" ");
                    #endif
                }
            }
        #endif

    #elif (I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE)

        // Fastwire library
        // no loop required for fastwire
        uint8_t status = Fastwire::readBuf(devAddr << 1, regAddr, data, length);
        if (status == 0) {
            count = length; // success
        } else {
            count = -1; // error
        }

    #endif

    // check for timeout
    if (timeout > 0 && millis() - t1 >= timeout && count < length) count = -1; // timeout

    #ifdef I2CDEV_SERIAL_DEBUG
        Serial.print(". Done (");
        Serial.print(count, DEC);
        Serial.println(" read).");
    #endif

    return count;
}

/** Read multiple words from a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr First register regAddr to read from
 * @param length Number of words to read
 * @param data Buffer to store read data in
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
 * @return Number of words read (-1 indicates failure)
 */
int8_t I2Cdev::readWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data, uint16_t timeout) {
    #ifdef I2CDEV_SERIAL_DEBUG
        Serial.print("I2C (0x");
        Serial.print(devAddr, HEX);
        Serial.print(") reading ");
        Serial.print(length, DEC);
        Serial.print(" words from 0x");
        Serial.print(regAddr, HEX);
        Serial.print("...");
    #endif

    int8_t count = 0;
    uint32_t t1 = millis();

    #if (I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE)

        #if (ARDUINO < 100)
            // Arduino v00xx (before v1.0), Wire library

            // I2C/TWI subsystem uses internal buffer that breaks with large data requests
            // so if user requests more than BUFFER_LENGTH bytes, we have to do it in
            // smaller chunks instead of all at once
            for (uint8_t k = 0; k < length * 2; k += min(length * 2, BUFFER_LENGTH)) {
                Wire.beginTransmission(devAddr);
                Wire.send(regAddr);
                Wire.endTransmission();
                Wire.beginTransmission(devAddr);
                Wire.requestFrom(devAddr, (uint8_t)(length * 2)); // length=words, this wants bytes
    
                bool msb = true; // starts with MSB, then LSB
                for (; Wire.available() && count < length && (timeout == 0 || millis() - t1 < timeout);) {
                    if (msb) {
                        // first byte is bits 15-8 (MSb=15)
                        data[count] = Wire.receive() << 8;
                    } else {
                        // second byte is bits 7-0 (LSb=0)
                        data[count] |= Wire.receive();
                        #ifdef I2CDEV_SERIAL_DEBUG
                            Serial.print(data[count], HEX);
                            if (count + 1 < length) Serial.print(" ");
                        #endif
                        count++;
                    }
                    msb = !msb;
                }

                Wire.endTransmission();
            }
        #elif (ARDUINO == 100)
            // Arduino v1.0.0, Wire library
            // Adds standardized write() and read() stream methods instead of send() and receive()
    
            // I2C/TWI subsystem uses internal buffer that breaks with large data requests
            // so if user requests more than BUFFER_LENGTH bytes, we have to do it in
            // smaller chunks instead of all at once
            for (uint8_t k = 0; k < length * 2; k += min(length * 2, BUFFER_LENGTH)) {
                Wire.beginTransmission(devAddr);
                Wire.write(regAddr);
                Wire.endTransmission();
                Wire.beginTransmission(devAddr);
                Wire.requestFrom(devAddr, (uint8_t)(length * 2)); // length=words, this wants bytes
    
                bool msb = true; // starts with MSB, then LSB
                for (; Wire.available() && count < length && (timeout == 0 || millis() - t1 < timeout);) {
                    if (msb) {
                        // first byte is bits 15-8 (MSb=15)
                        data[count] = Wire.read() << 8;
                    } else {
                        // second byte is bits 7-0 (LSb=0)
                        data[count] |= Wire.read();
                        #ifdef I2CDEV_SERIAL_DEBUG
                            Serial.print(data[count], HEX);
                            if (count + 1 < length) Serial.print(" ");
                        #endif
                        count++;
                    }
                    msb = !msb;
                }
        
                Wire.endTransmission();
            }
        #elif (ARDUINO > 100)
            // Arduino v1.0.1+, Wire library
            // Adds official support for repeated start condition, yay!

            // I2C/TWI subsystem uses internal buffer that breaks with large data requests
            // so if user requests more than BUFFER_LENGTH bytes, we have to do it in
            // smaller chunks instead of all at once
            for (uint8_t k = 0; k < length * 2; k += min(length * 2, BUFFER_LENGTH)) {
                Wire.beginTransmission(devAddr);
                Wire.write(regAddr);
                Wire.endTransmission();
                Wire.beginTransmission(devAddr);
                Wire.requestFrom(devAddr, (uint8_t)(length * 2)); // length=words, this wants bytes
        
                bool msb = true; // starts with MSB, then LSB
                for (; Wire.available() && count < length && (timeout == 0 || millis() - t1 < timeout);) {
                    if (msb) {
                        // first byte is bits 15-8 (MSb=15)
                        data[count] = Wire.read() << 8;
                    } else {
                        // second byte is bits 7-0 (LSb=0)
                        data[count] |= Wire.read();
                        #ifdef I2CDEV_SERIAL_DEBUG
                            Serial.print(data[count], HEX);
                            if (count + 1 < length) Serial.print(" ");
                        #endif
                        count++;
                    }
                    msb = !msb;
                }
        
                Wire.endTransmission();
            }
        #endif

    #elif (I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE)

        // Fastwire library
        // no loop required for fastwire
        uint16_t intermediate[(uint8_t)length];
        uint8_t status = Fastwire::readBuf(devAddr << 1, regAddr, (uint8_t *)intermediate, (uint8_t)(length * 2));
        if (status == 0) {
            count = length; // success
            for (uint8_t i = 0; i < length; i++) {
                data[i] = (intermediate[2*i] << 8) | intermediate[2*i + 1];
            }
        } else {
            count = -1; // error
        }

    #endif

    if (timeout > 0 && millis() - t1 >= timeout && count < length) count = -1; // timeout

    #ifdef I2CDEV_SERIAL_DEBUG
        Serial.print(". Done (");
        Serial.print(count, DEC);
        Serial.println(" read).");
    #endif
    
    return count;
}

/** write a single bit in an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitNum Bit position to write (0-7)
 * @param value New bit value to write
 * @return Status of operation (true = success)
 */
bool I2Cdev::writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data) {
    uint8_t b;
    readByte(devAddr, regAddr, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return writeByte(devAddr, regAddr, b);
}

/** write a single bit in a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitNum Bit position to write (0-15)
 * @param value New bit value to write
 * @return Status of operation (true = success)
 */
bool I2Cdev::writeBitW(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t data) {
    uint16_t w;
    readWord(devAddr, regAddr, &w);
    w = (data != 0) ? (w | (1 << bitNum)) : (w & ~(1 << bitNum));
    return writeWord(devAddr, regAddr, w);
}

/** Write multiple bits in an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitStart First bit position to write (0-7)
 * @param length Number of bits to write (not more than 8)
 * @param data Right-aligned value to write
 * @return Status of operation (true = success)
 */
bool I2Cdev::writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data) {
    //      010 value to write
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    // 00011100 mask byte
    // 10101111 original value (sample)
    // 10100011 original & ~mask
    // 10101011 masked | value
    uint8_t b;
    if (readByte(devAddr, regAddr, &b) != 0) {
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        data <<= (bitStart - length + 1); // shift data into correct position
        data &= mask; // zero all non-important bits in data
        b &= ~(mask); // zero all important bits in existing byte
        b |= data; // combine data with existing byte
        return writeByte(devAddr, regAddr, b);
    } else {
        return false;
    }
}

/** Write multiple bits in a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitStart First bit position to write (0-15)
 * @param length Number of bits to write (not more than 16)
 * @param data Right-aligned value to write
 * @return Status of operation (true = success)
 */
bool I2Cdev::writeBitsW(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t data) {
    //              010 value to write
    // fedcba9876543210 bit numbers
    //    xxx           args: bitStart=12, length=3
    // 0001110000000000 mask word
    // 1010111110010110 original value (sample)
    // 1010001110010110 original & ~mask
    // 1010101110010110 masked | value
    uint16_t w;
    if (readWord(devAddr, regAddr, &w) != 0) {
        uint16_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        data <<= (bitStart - length + 1); // shift data into correct position
        data &= mask; // zero all non-important bits in data
        w &= ~(mask); // zero all important bits in existing word
        w |= data; // combine data with existing word
        return writeWord(devAddr, regAddr, w);
    } else {
        return false;
    }
}

/** Write single byte to an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register address to write to
 * @param data New byte value to write
 * @return Status of operation (true = success)
 */
bool I2Cdev::writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data) {
    return writeBytes(devAddr, regAddr, 1, &data);
}

/** Write single word to a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register address to write to
 * @param data New word value to write
 * @return Status of operation (true = success)
 */
bool I2Cdev::writeWord(uint8_t devAddr, uint8_t regAddr, uint16_t data) {
    return writeWords(devAddr, regAddr, 1, &data);
}

/** Write multiple bytes to an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr First register address to write to
 * @param length Number of bytes to write
 * @param data Buffer to copy new data from
 * @return Status of operation (true = success)
 */
bool I2Cdev::writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t* data) {
    #ifdef I2CDEV_SERIAL_DEBUG
        Serial.print("I2C (0x");
        Serial.print(devAddr, HEX);
        Serial.print(") writing ");
        Serial.print(length, DEC);
        Serial.print(" bytes to 0x");
        Serial.print(regAddr, HEX);
        Serial.print("...");
    #endif
    uint8_t status = 0;
    #if ((I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE && ARDUINO < 100) || I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_NBWIRE)
        Wire.beginTransmission(devAddr);
        Wire.send((uint8_t) regAddr); // send address
    #elif (I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE && ARDUINO >= 100)
        Wire.beginTransmission(devAddr);
        Wire.write((uint8_t) regAddr); // send address
    #elif (I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE)
        Fastwire::beginTransmission(devAddr);
        Fastwire::write(regAddr);
    #endif
    for (uint8_t i = 0; i < length; i++) {
        #ifdef I2CDEV_SERIAL_DEBUG
            Serial.print(data[i], HEX);
            if (i + 1 < length) Serial.print(" ");
        #endif
        #if ((I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE && ARDUINO < 100) || I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_NBWIRE)
            Wire.send((uint8_t) data[i]);
        #elif (I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE && ARDUINO >= 100)
            Wire.write((uint8_t) data[i]);
        #elif (I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE)
            Fastwire::write((uint8_t) data[i]);
        #endif
    }
    #if ((I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE && ARDUINO < 100) || I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_NBWIRE)
        Wire.endTransmission();
    #elif (I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE && ARDUINO >= 100)
        status = Wire.endTransmission();
    #elif (I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE)
        Fastwire::stop();
        //status = Fastwire::endTransmission();
    #endif
    #ifdef I2CDEV_SERIAL_DEBUG
        Serial.println(". Done.");
    #endif
    return status == 0;
}

/** Write multiple words to a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr First register address to write to
 * @param length Number of words to write
 * @param data Buffer to copy new data from
 * @return Status of operation (true = success)
 */
bool I2Cdev::writeWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t* data) {
    #ifdef I2CDEV_SERIAL_DEBUG
        Serial.print("I2C (0x");
        Serial.print(devAddr, HEX);
        Serial.print(") writing ");
        Serial.print(length, DEC);
        Serial.print(" words to 0x");
        Serial.print(regAddr, HEX);
        Serial.print("...");
    #endif
    uint8_t status = 0;
    #if ((I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE && ARDUINO < 100) || I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_NBWIRE)
        Wire.beginTransmission(devAddr);
        Wire.send(regAddr); // send address
    #elif (I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE && ARDUINO >= 100)
        Wire.beginTransmission(devAddr);
        Wire.write(regAddr); // send address
    #elif (I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE)
        Fastwire::beginTransmission(devAddr);
        Fastwire::write(regAddr);
    #endif
    for (uint8_t i = 0; i < length * 2; i++) {
        #ifdef I2CDEV_SERIAL_DEBUG
            Serial.print(data[i], HEX);
            if (i + 1 < length) Serial.print(" ");
        #endif
        #if ((I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE && ARDUINO < 100) || I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_NBWIRE)
            Wire.send((uint8_t)(data[i] >> 8));     // send MSB
            Wire.send((uint8_t)data[i++]);          // send LSB
        #elif (I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE && ARDUINO >= 100)
            Wire.write((uint8_t)(data[i] >> 8));    // send MSB
            Wire.write((uint8_t)data[i++]);         // send LSB
        #elif (I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE)
            Fastwire::write((uint8_t)(data[i] >> 8));       // send MSB
            status = Fastwire::write((uint8_t)data[i++]);   // send LSB
            if (status != 0) break;
        #endif
    }
    #if ((I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE && ARDUINO < 100) || I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_NBWIRE)
        Wire.endTransmission();
    #elif (I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE && ARDUINO >= 100)
        status = Wire.endTransmission();
    #elif (I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE)
        Fastwire::stop();
        //status = Fastwire::endTransmission();
    #endif
    #ifdef I2CDEV_SERIAL_DEBUG
        Serial.println(". Done.");
    #endif
    return status == 0;
}

/** Default timeout value for read operations.
 * Set this to 0 to disable timeout detection.
 */
uint16_t I2Cdev::readTimeout = I2CDEV_DEFAULT_READ_TIMEOUT;

#if I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    // I2C library
    //////////////////////
    // Copyright(C) 2012
    // Francesco Ferrara
    // ferrara[at]libero[point]it
    //////////////////////

    /*
    FastWire
    - 0.24 added stop
    - 0.23 added reset

     This is a library to help faster programs to read I2C devices.
     Copyright(C) 2012 Francesco Ferrara
     occhiobello at gmail dot com
     [used by Jeff Rowberg for I2Cdevlib with permission]
     */

    boolean Fastwire::waitInt() {
        int l = 250;
        while (!(TWCR & (1 << TWINT)) && l-- > 0);
        return l > 0;
    }

    void Fastwire::setup(int khz, boolean pullup) {
        TWCR = 0;
        #if defined(__AVR_ATmega168__) || defined(__AVR_ATmega8__) || defined(__AVR_ATmega328P__)
            // activate internal pull-ups for twi (PORTC bits 4 & 5)
            // as per note from atmega8 manual pg167
            if (pullup) PORTC |= ((1 << 4) | (1 << 5));
            else        PORTC &= ~((1 << 4) | (1 << 5));
        #elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644__)
            // activate internal pull-ups for twi (PORTC bits 0 & 1)
            if (pullup) PORTC |= ((1 << 0) | (1 << 1));
            else        PORTC &= ~((1 << 0) | (1 << 1));
        #else
            // activate internal pull-ups for twi (PORTD bits 0 & 1)
            // as per note from atmega128 manual pg204
            if (pullup) PORTD |= ((1 << 0) | (1 << 1));
            else        PORTD &= ~((1 << 0) | (1 << 1));
        #endif

        TWSR = 0; // no prescaler => prescaler = 1
        TWBR = ((16000L / khz) - 16) / 2; // change the I2C clock rate
        TWCR = 1 << TWEN; // enable twi module, no interrupt
    }

    // added by Jeff Rowberg 2013-05-07:
    // Arduino Wire-style "beginTransmission" function
    // (takes 7-bit device address like the Wire method, NOT 8-bit: 0x68, not 0xD0/0xD1)
    byte Fastwire::beginTransmission(byte device) {
        byte twst, retry;
        retry = 2;
        do {
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO) | (1 << TWSTA);
            if (!waitInt()) return 1;
            twst = TWSR & 0xF8;
            if (twst != TW_START && twst != TW_REP_START) return 2;

            //Serial.print(device, HEX);
            //Serial.print(" ");
            TWDR = device << 1; // send device address without read bit (1)
            TWCR = (1 << TWINT) | (1 << TWEN);
            if (!waitInt()) return 3;
            twst = TWSR & 0xF8;
        } while (twst == TW_MT_SLA_NACK && retry-- > 0);
        if (twst != TW_MT_SLA_ACK) return 4;
        return 0;
    }

    byte Fastwire::writeBuf(byte device, byte address, byte *data, byte num) {
        byte twst, retry;

        retry = 2;
        do {
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO) | (1 << TWSTA);
            if (!waitInt()) return 1;
            twst = TWSR & 0xF8;
            if (twst != TW_START && twst != TW_REP_START) return 2;

            //Serial.print(device, HEX);
            //Serial.print(" ");
            TWDR = device & 0xFE; // send device address without read bit (1)
            TWCR = (1 << TWINT) | (1 << TWEN);
            if (!waitInt()) return 3;
            twst = TWSR & 0xF8;
        } while (twst == TW_MT_SLA_NACK && retry-- > 0);
        if (twst != TW_MT_SLA_ACK) return 4;

        //Serial.print(address, HEX);
        //Serial.print(" ");
        TWDR = address; // send data to the previously addressed device
        TWCR = (1 << TWINT) | (1 << TWEN);
        if (!waitInt()) return 5;
        twst = TWSR & 0xF8;
        if (twst != TW_MT_DATA_ACK) return 6;

        for (byte i = 0; i < num; i++) {
            //Serial.print(data[i], HEX);
            //Serial.print(" ");
            TWDR = data[i]; // send data to the previously addressed device
            TWCR = (1 << TWINT) | (1 << TWEN);
            if (!waitInt()) return 7;
            twst = TWSR & 0xF8;
            if (twst != TW_MT_DATA_ACK) return 8;
        }
        //Serial.print("\n");

        return 0;
    }

    byte Fastwire::write(byte value) {
        byte twst;
        //Serial.println(value, HEX);
        TWDR = value; // send data
        TWCR = (1 << TWINT) | (1 << TWEN);
        if (!waitInt()) return 1;
        twst = TWSR & 0xF8;
        if (twst != TW_MT_DATA_ACK) return 2;
        return 0;
    }

    byte Fastwire::readBuf(byte device, byte address, byte *data, byte num) {
        byte twst, retry;

        retry = 2;
        do {
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO) | (1 << TWSTA);
            if (!waitInt()) return 16;
            twst = TWSR & 0xF8;
            if (twst != TW_START && twst != TW_REP_START) return 17;

            //Serial.print(device, HEX);
            //Serial.print(" ");
            TWDR = device & 0xfe; // send device address to write
            TWCR = (1 << TWINT) | (1 << TWEN);
            if (!waitInt()) return 18;
            twst = TWSR & 0xF8;
        } while (twst == TW_MT_SLA_NACK && retry-- > 0);
        if (twst != TW_MT_SLA_ACK) return 19;

        //Serial.print(address, HEX);
        //Serial.print(" ");
        TWDR = address; // send data to the previously addressed device
        TWCR = (1 << TWINT) | (1 << TWEN);
        if (!waitInt()) return 20;
        twst = TWSR & 0xF8;
        if (twst != TW_MT_DATA_ACK) return 21;

        /***/

        retry = 2;
        do {
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO) | (1 << TWSTA);
            if (!waitInt()) return 22;
            twst = TWSR & 0xF8;
            if (twst != TW_START && twst != TW_REP_START) return 23;

            //Serial.print(device, HEX);
            //Serial.print(" ");
            TWDR = device | 0x01; // send device address with the read bit (1)
            TWCR = (1 << TWINT) | (1 << TWEN);
            if (!waitInt()) return 24;
            twst = TWSR & 0xF8;
        } while (twst == TW_MR_SLA_NACK && retry-- > 0);
        if (twst != TW_MR_SLA_ACK) return 25;

        for (uint8_t i = 0; i < num; i++) {
            if (i == num - 1)
                TWCR = (1 << TWINT) | (1 << TWEN);
            else
                TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
            if (!waitInt()) return 26;
            twst = TWSR & 0xF8;
            if (twst != TW_MR_DATA_ACK && twst != TW_MR_DATA_NACK) return twst;
            data[i] = TWDR;
            //Serial.print(data[i], HEX);
            //Serial.print(" ");
        }
        //Serial.print("\n");
        stop();

        return 0;
    }

    void Fastwire::reset() {
        TWCR = 0;
    }

    byte Fastwire::stop() {
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
        if (!waitInt()) return 1;
        return 0;
    }
#endif

#if I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_NBWIRE
    // NBWire implementation based heavily on code by Gene Knight <Gene@Telobot.com>
    // Originally posted on the Arduino forum at http://arduino.cc/forum/index.php/topic,70705.0.html
    // Originally offered to the i2cdevlib project at http://arduino.cc/forum/index.php/topic,68210.30.html

    /*
    call this version 1.0
    
    Offhand, the only funky part that I can think of is in nbrequestFrom, where the buffer
    length and index are set *before* the data is actually read. The problem is that these
    are variables local to the TwoWire object, and by the time we actually have read the
    data, and know what the length actually is, we have no simple access to the object's 
    variables. The actual bytes read *is* given to the callback function, though.
    
    The ISR code for a slave receiver is commented out. I don't have that setup, and can't
    verify it at this time. Save it for 2.0!
    
    The handling of the read and write processes here is much like in the demo sketch code: 
    the process is broken down into sequential functions, where each registers the next as a
    callback, essentially.
    
    For example, for the Read process, twi_read00 just returns if TWI is not yet in a 
    ready state. When there's another interrupt, and the interface *is* ready, then it
    sets up the read, starts it, and registers twi_read01 as the function to call after
    the *next* interrupt. twi_read01, then, just returns if the interface is still in a
    "reading" state. When the reading is done, it copies the information to the buffer,
    cleans up, and calls the user-requested callback function with the actual number of 
    bytes read.
    
    The writing is similar.
    
    Questions, comments and problems can go to Gene@Telobot.com.
    
    Thumbs Up!
    Gene Knight
    
    */
    
    uint8_t TwoWire::rxBuffer[NBWIRE_BUFFER_LENGTH];
    uint8_t TwoWire::rxBufferIndex = 0;
    uint8_t TwoWire::rxBufferLength = 0;
    
    uint8_t TwoWire::txAddress = 0;
    uint8_t TwoWire::txBuffer[NBWIRE_BUFFER_LENGTH];
    uint8_t TwoWire::txBufferIndex = 0;
    uint8_t TwoWire::txBufferLength = 0;
    
    //uint8_t TwoWire::transmitting = 0;
    void (*TwoWire::user_onRequest)(void);
    void (*TwoWire::user_onReceive)(int);
    
    static volatile uint8_t twi_transmitting;
    static volatile uint8_t twi_state;
    static uint8_t twi_slarw;
    static volatile uint8_t twi_error;
    static uint8_t twi_masterBuffer[TWI_BUFFER_LENGTH];
    static volatile uint8_t twi_masterBufferIndex;
    static uint8_t twi_masterBufferLength;
    static uint8_t twi_rxBuffer[TWI_BUFFER_LENGTH];
    static volatile uint8_t twi_rxBufferIndex;
    //static volatile uint8_t twi_Interrupt_Continue_Command;
    static volatile uint8_t twi_Return_Value;
    static volatile uint8_t twi_Done;
    void (*twi_cbendTransmissionDone)(int);
    void (*twi_cbreadFromDone)(int);
    
    void twi_init() {
        // initialize state
        twi_state = TWI_READY;

        // activate internal pull-ups for twi
        // as per note from atmega8 manual pg167
        sbi(PORTC, 4);
        sbi(PORTC, 5);

        // initialize twi prescaler and bit rate
        cbi(TWSR, TWPS0); // TWI Status Register - Prescaler bits
        cbi(TWSR, TWPS1);

        /* twi bit rate formula from atmega128 manual pg 204
        SCL Frequency = CPU Clock Frequency / (16 + (2 * TWBR))
        note: TWBR should be 10 or higher for master mode
        It is 72 for a 16mhz Wiring board with 100kHz TWI */

        TWBR = ((CPU_FREQ / TWI_FREQ) - 16) / 2; // bitrate register
        // enable twi module, acks, and twi interrupt

        TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA);

        /* TWEN - TWI Enable Bit
        TWIE - TWI Interrupt Enable
        TWEA - TWI Enable Acknowledge Bit
        TWINT - TWI Interrupt Flag
        TWSTA - TWI Start Condition
        */
    }
    
    typedef struct {
        uint8_t address;
        uint8_t* data;
        uint8_t length;
        uint8_t wait;
        uint8_t i;
    } twi_Write_Vars;

    twi_Write_Vars *ptwv = 0;
    static void (*fNextInterruptFunction)(void) = 0;

    void twi_Finish(byte bRetVal) {
        if (ptwv) {
            free(ptwv);
            ptwv = 0;
        }
        twi_Done = 0xFF;
        twi_Return_Value = bRetVal;
        fNextInterruptFunction = 0;
    }
    
    uint8_t twii_WaitForDone(uint16_t timeout) {
        uint32_t endMillis = millis() + timeout;
        while (!twi_Done && (timeout == 0 || millis() < endMillis)) continue;
        return twi_Return_Value;
    }
    
    void twii_SetState(uint8_t ucState) {
        twi_state = ucState;
    }

    void twii_SetError(uint8_t ucError) {
        twi_error = ucError ;
    }

    void twii_InitBuffer(uint8_t ucPos, uint8_t ucLength) {
        twi_masterBufferIndex = 0;
        twi_masterBufferLength = ucLength;
    }

    void twii_CopyToBuf(uint8_t* pData, uint8_t ucLength) {
        uint8_t i;
        for (i = 0; i < ucLength; ++i) {
            twi_masterBuffer[i] = pData[i];
        }
    }

    void twii_CopyFromBuf(uint8_t *pData, uint8_t ucLength) {
        uint8_t i;
        for (i = 0; i < ucLength; ++i) {
            pData[i] = twi_masterBuffer[i];
        }
    }

    void twii_SetSlaRW(uint8_t ucSlaRW) {
        twi_slarw = ucSlaRW;
    }

    void twii_SetStart() {
        TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTA);
    }

    void twi_write01() {
        if (TWI_MTX == twi_state) return; // blocking test
        twi_transmitting = 0 ;
        if (twi_error == 0xFF)
            twi_Finish (0);    // success
        else if (twi_error == TW_MT_SLA_NACK)
            twi_Finish (2);    // error: address send, nack received
        else if (twi_error == TW_MT_DATA_NACK)
            twi_Finish (3);    // error: data send, nack received
        else
            twi_Finish (4);    // other twi error
        if (twi_cbendTransmissionDone) return twi_cbendTransmissionDone(twi_Return_Value);
        return;
    }
    
    
    void twi_write00() {
        if (TWI_READY != twi_state) return; // blocking test
        if (TWI_BUFFER_LENGTH < ptwv -> length) {
            twi_Finish(1); // end write with error 1
            return;
        }
        twi_Done = 0x00; // show as working
        twii_SetState(TWI_MTX); // to transmitting
        twii_SetError(0xFF); // to No Error
        twii_InitBuffer(0, ptwv -> length); // pointer and length
        twii_CopyToBuf(ptwv -> data, ptwv -> length); // get the data
        twii_SetSlaRW((ptwv -> address << 1) | TW_WRITE); // write command
        twii_SetStart(); // start the cycle
        fNextInterruptFunction = twi_write01; // next routine
        return twi_write01();
    }
    
    void twi_writeTo(uint8_t address, uint8_t* data, uint8_t length, uint8_t wait) {
        uint8_t i;
        ptwv = (twi_Write_Vars *)malloc(sizeof(twi_Write_Vars));
        ptwv -> address = address;
        ptwv -> data = data;
        ptwv -> length = length;
        ptwv -> wait = wait;
        fNextInterruptFunction = twi_write00;
        return twi_write00();
    }

    void twi_read01() {
        if (TWI_MRX == twi_state) return; // blocking test
        if (twi_masterBufferIndex < ptwv -> length) ptwv -> length = twi_masterBufferIndex;
        twii_CopyFromBuf(ptwv -> data, ptwv -> length);
        twi_Finish(ptwv -> length);
        if (twi_cbreadFromDone) return twi_cbreadFromDone(twi_Return_Value);
        return;
    }
    
    void twi_read00() {
        if (TWI_READY != twi_state) return; // blocking test
        if (TWI_BUFFER_LENGTH < ptwv -> length) twi_Finish(0); // error return
        twi_Done = 0x00; // show as working
        twii_SetState(TWI_MRX); // reading
        twii_SetError(0xFF); // reset error
        twii_InitBuffer(0, ptwv -> length - 1); // init to one less than length
        twii_SetSlaRW((ptwv -> address << 1) | TW_READ); // read command
        twii_SetStart(); // start cycle
        fNextInterruptFunction = twi_read01;
        return twi_read01();
    }

    void twi_readFrom(uint8_t address, uint8_t* data, uint8_t length) {
        uint8_t i;

        ptwv = (twi_Write_Vars *)malloc(sizeof(twi_Write_Vars));
        ptwv -> address = address;
        ptwv -> data = data;
        ptwv -> length = length;
        fNextInterruptFunction = twi_read00;
        return twi_read00();
    }

    void twi_reply(uint8_t ack) {
        // transmit master read ready signal, with or without ack
        if (ack){
            TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
        } else {
            TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
        }
    }
    
    void twi_stop(void) {
        // send stop condition
        TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTO);
    
        // wait for stop condition to be exectued on bus
        // TWINT is not set after a stop condition!
        while (TWCR & _BV(TWSTO)) {
            continue;
        }
    
        // update twi state
        twi_state = TWI_READY;
    }

    void twi_releaseBus(void) {
        // release bus
        TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT);
    
        // update twi state
        twi_state = TWI_READY;
    }
    
    SIGNAL(TWI_vect) {
        switch (TW_STATUS) {
            // All Master
            case TW_START:     // sent start condition
            case TW_REP_START: // sent repeated start condition
                // copy device address and r/w bit to output register and ack
                TWDR = twi_slarw;
                twi_reply(1);
                break;
    
            // Master Transmitter
            case TW_MT_SLA_ACK:  // slave receiver acked address
            case TW_MT_DATA_ACK: // slave receiver acked data
                // if there is data to send, send it, otherwise stop
                if (twi_masterBufferIndex < twi_masterBufferLength) {
                    // copy data to output register and ack
                    TWDR = twi_masterBuffer[twi_masterBufferIndex++];
                    twi_reply(1);
                } else {
                    twi_stop();
                }
                break;

            case TW_MT_SLA_NACK:  // address sent, nack received
                twi_error = TW_MT_SLA_NACK;
                twi_stop();
                break;

            case TW_MT_DATA_NACK: // data sent, nack received
                twi_error = TW_MT_DATA_NACK;
                twi_stop();
                break;

            case TW_MT_ARB_LOST: // lost bus arbitration
                twi_error = TW_MT_ARB_LOST;
                twi_releaseBus();
                break;
    
            // Master Receiver
            case TW_MR_DATA_ACK: // data received, ack sent
                // put byte into buffer
                twi_masterBuffer[twi_masterBufferIndex++] = TWDR;

            case TW_MR_SLA_ACK:  // address sent, ack received
                // ack if more bytes are expected, otherwise nack
                if (twi_masterBufferIndex < twi_masterBufferLength) {
                    twi_reply(1);
                } else {
                    twi_reply(0);
                }
                break;

            case TW_MR_DATA_NACK: // data received, nack sent
                // put final byte into buffer
                twi_masterBuffer[twi_masterBufferIndex++] = TWDR;

            case TW_MR_SLA_NACK: // address sent, nack received
                twi_stop();
                break;

        // TW_MR_ARB_LOST handled by TW_MT_ARB_LOST case

        // Slave Receiver (NOT IMPLEMENTED YET)
        /*
            case TW_SR_SLA_ACK:   // addressed, returned ack
            case TW_SR_GCALL_ACK: // addressed generally, returned ack
            case TW_SR_ARB_LOST_SLA_ACK:   // lost arbitration, returned ack
            case TW_SR_ARB_LOST_GCALL_ACK: // lost arbitration, returned ack
                // enter slave receiver mode
                twi_state = TWI_SRX;

                // indicate that rx buffer can be overwritten and ack
                twi_rxBufferIndex = 0;
                twi_reply(1);
                break;

            case TW_SR_DATA_ACK:       // data received, returned ack
            case TW_SR_GCALL_DATA_ACK: // data received generally, returned ack
                // if there is still room in the rx buffer
                if (twi_rxBufferIndex < TWI_BUFFER_LENGTH) {
                    // put byte in buffer and ack
                    twi_rxBuffer[twi_rxBufferIndex++] = TWDR;
                    twi_reply(1);
                } else {
                    // otherwise nack
                    twi_reply(0);
                }
                break;

            case TW_SR_STOP: // stop or repeated start condition received
                // put a null char after data if there's room
                if (twi_rxBufferIndex < TWI_BUFFER_LENGTH) {
                    twi_rxBuffer[twi_rxBufferIndex] = 0;
                }

                // sends ack and stops interface for clock stretching
                twi_stop();

                // callback to user defined callback
                twi_onSlaveReceive(twi_rxBuffer, twi_rxBufferIndex);

                // since we submit rx buffer to "wire" library, we can reset it
                twi_rxBufferIndex = 0;

                // ack future responses and leave slave receiver state
                twi_releaseBus();
                break;

            case TW_SR_DATA_NACK:       // data received, returned nack
            case TW_SR_GCALL_DATA_NACK: // data received generally, returned nack
                // nack back at master
                twi_reply(0);
                break;

            // Slave Transmitter
            case TW_ST_SLA_ACK:          // addressed, returned ack
            case TW_ST_ARB_LOST_SLA_ACK: // arbitration lost, returned ack
                // enter slave transmitter mode
                twi_state = TWI_STX;

                // ready the tx buffer index for iteration
                twi_txBufferIndex = 0;

                // set tx buffer length to be zero, to verify if user changes it
                twi_txBufferLength = 0;

                // request for txBuffer to be filled and length to be set
                // note: user must call twi_transmit(bytes, length) to do this
                twi_onSlaveTransmit();

                // if they didn't change buffer & length, initialize it
                if (0 == twi_txBufferLength) {
                    twi_txBufferLength = 1;
                    twi_txBuffer[0] = 0x00;
                }
                
                // transmit first byte from buffer, fall through

            case TW_ST_DATA_ACK: // byte sent, ack returned
                // copy data to output register
                TWDR = twi_txBuffer[twi_txBufferIndex++];

                // if there is more to send, ack, otherwise nack
                if (twi_txBufferIndex < twi_txBufferLength) {
                    twi_reply(1);
                } else {
                    twi_reply(0);
                }
                break;

            case TW_ST_DATA_NACK: // received nack, we are done
            case TW_ST_LAST_DATA: // received ack, but we are done already!
                // ack future responses
                twi_reply(1);
                // leave slave receiver state
                twi_state = TWI_READY;
                break;
            */

            // all
            case TW_NO_INFO:   // no state information
                break;

            case TW_BUS_ERROR: // bus error, illegal stop/start
                twi_error = TW_BUS_ERROR;
                twi_stop();
                break;
        }

        if (fNextInterruptFunction) return fNextInterruptFunction();
    }

    TwoWire::TwoWire() { }
    
    void TwoWire::begin(void) {
        rxBufferIndex = 0;
        rxBufferLength = 0;
    
        txBufferIndex = 0;
        txBufferLength = 0;

        twi_init();
    }
    
    void TwoWire::beginTransmission(uint8_t address) {
        //beginTransmission((uint8_t)address);

        // indicate that we are transmitting
        twi_transmitting = 1;
        
        // set address of targeted slave
        txAddress = address;
        
        // reset tx buffer iterator vars
        txBufferIndex = 0;
        txBufferLength = 0;
    }

    uint8_t TwoWire::endTransmission(uint16_t timeout) {
        // transmit buffer (blocking)
        //int8_t ret =
        twi_cbendTransmissionDone = NULL;
        twi_writeTo(txAddress, txBuffer, txBufferLength, 1);
        int8_t ret = twii_WaitForDone(timeout);

        // reset tx buffer iterator vars
        txBufferIndex = 0;
        txBufferLength = 0;

        // indicate that we are done transmitting
        // twi_transmitting = 0;
        return ret;
    }

    void TwoWire::nbendTransmission(void (*function)(int)) {
        twi_cbendTransmissionDone = function;
        twi_writeTo(txAddress, txBuffer, txBufferLength, 1);
        return;
    }
    
    void TwoWire::send(uint8_t data) {
        if (twi_transmitting) {
            // in master transmitter mode
            // don't bother if buffer is full
            if (txBufferLength >= NBWIRE_BUFFER_LENGTH) {
                return;
            }

            // put byte in tx buffer
            txBuffer[txBufferIndex] = data;
            ++txBufferIndex;

            // update amount in buffer
            txBufferLength = txBufferIndex;
        } else {
            // in slave send mode
            // reply to master
            //twi_transmit(&data, 1);
        }
    }
    
    uint8_t TwoWire::receive(void) {
        // default to returning null char
        // for people using with char strings
        uint8_t value = 0;
      
        // get each successive byte on each call
        if (rxBufferIndex < rxBufferLength) {
            value = rxBuffer[rxBufferIndex];
            ++rxBufferIndex;
        }
    
        return value;
    }
    
    uint8_t TwoWire::requestFrom(uint8_t address, int quantity, uint16_t timeout) {
        // clamp to buffer length
        if (quantity > NBWIRE_BUFFER_LENGTH) {
            quantity = NBWIRE_BUFFER_LENGTH;
        }

        // perform blocking read into buffer
        twi_cbreadFromDone = NULL;
        twi_readFrom(address, rxBuffer, quantity);
        uint8_t read = twii_WaitForDone(timeout);

        // set rx buffer iterator vars
        rxBufferIndex = 0;
        rxBufferLength = read;
    
        return read;
    }
    
    void TwoWire::nbrequestFrom(uint8_t address, int quantity, void (*function)(int)) {
        // clamp to buffer length
        if (quantity > NBWIRE_BUFFER_LENGTH) {
            quantity = NBWIRE_BUFFER_LENGTH;
        }

        // perform blocking read into buffer
        twi_cbreadFromDone = function;
        twi_readFrom(address, rxBuffer, quantity);
        //uint8_t read = twii_WaitForDone();

        // set rx buffer iterator vars
        //rxBufferIndex = 0;
        //rxBufferLength = read;

        rxBufferIndex = 0;
        rxBufferLength = quantity; // this is a hack

        return; //read;
    }

    uint8_t TwoWire::available(void) {
        return rxBufferLength - rxBufferIndex;
    }

#endif
