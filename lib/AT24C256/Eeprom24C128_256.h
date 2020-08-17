/**************************************************************************//**
 * \brief EEPROM 24C128 / 24C256 library for Arduino
 * \author Copyright (C) 2012  Julien Le Sech - www.idreammicro.com
 * \version 1.0
 * \date 20120203
 *
 * This file is part of the EEPROM 24C128 / 24C256 library for Arduino.
 *
 * This library is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 * 
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/
 ******************************************************************************/

/**************************************************************************//**
 * \headerfile Eeprom24C128_256.h
 ******************************************************************************/

#ifndef Eeprom24C128_256_h
#define Eeprom24C128_256_h

/******************************************************************************
 * Header file inclusion.
 ******************************************************************************/

#include <Arduino.h>

/**************************************************************************//**
 * \class Eeprom24C128_256
 *
 * \brief EEPROM 24C128 / 24C256 memory driver.
 *
 * This driver is mainly designed for 24C128 and 24C256 EEPROM memories. It's
 * also suitable for 24C512 memories.
 ******************************************************************************/
class Eeprom24C128_256
{
    public:
    
        /******************************************************************//**
         * \fn Eeprom24C128_256(byte deviceAddress)
         *
         * \brief Constructor.
         *
         * \param   deviceAddress   EEPROM address on TWI bus.
         **********************************************************************/
        Eeprom24C128_256
        (
            byte deviceAddress
        );

        /******************************************************************//**
         * \fn void initialize()
         *
         * \brief Initialize library abnd TWI bus.
         * 
         * If several devices are connected to TWI bus, this method mustn't be
         * called. TWI bus must be initialized out of this library using
         * Wire.begin() method.
         **********************************************************************/        
        void
        initialize();

        /******************************************************************//**
         * \fn void writeByte(
         * word address,
         * byte data)
         *
         * \brief Write a byte in EEPROM memory.
         *
         * \remarks A delay of 10 ms is required after write cycle.
         *
         * \param   address Address.
         * \param   data    Byte to write.
         **********************************************************************/
        void
        writeByte
        (
            word    address,
            byte    data
        );
        
        /******************************************************************//**
         * \fn void writeBytes(
         * word     address,
         * word     length,
         * byte*    p_data)
         * 
         * \brief Write bytes in EEPROM memory.
         *
         * \param       address Start address.
         * \param       length  Number of bytes to write.
         * \param[in]   p_data  Bytes to write.
         **********************************************************************/
        void
        writeBytes
        (
            word    address,
            word    length,
            byte*   p_data
        );
        
        /******************************************************************//**
         * \fn byte readByte(word address)
         * 
         * \brief Read a byte in EEPROM memory.
         *
         * \param   address Address.
         *
         * \return Read byte.
         **********************************************************************/
        byte
        readByte
        (
            word    address
        );

        /******************************************************************//**
         * \fn void readBytes(
         * word     address,
         * word     length,
         * byte*    p_data)
         *
         * \brief Read bytes in EEPROM memory.
         *
         * \param       address Start address.
         * \param       length  Number of bytes to read.
         * \patam[in]   p_data  Byte array to fill with read bytes.
         **********************************************************************/
        void
        readBytes
        (
            word    address,
            word    length,
            byte*   p_buffer
        );
        
    private:

        byte m_deviceAddress;

        /******************************************************************//**
         * \fn void writePage(
         * word     address,
         * byte     length,
         * byte*    p_data)
         *
         * \brief Write page in EEPROM memory.
         *
         * \param       address Start address.
         * \param       length  Number of bytes (64 bytes max).
         * \param[in]   p_data  Data.
         **********************************************************************/
        void
        writePage
        (
            word    address,
            byte    length,
            byte*   p_data
        );

        /******************************************************************//**
         * \fn void writeBuffer(
         * word     address,
         * byte     length,
         * byte*    p_data)
         *
         * \brief Write bytes into memory.
         *
         * \param       address Start address.
         * \param       length  Number of bytes (30 bytes max).
         * \param[in]   p_date  Data.
         **********************************************************************/
        void
        writeBuffer
        (
            word    address,
            byte    length,
            byte*   p_data
       );

        /******************************************************************//**
         * \fn void readBuffer(
         * word     address,
         * byte     length,
         * byte*    p_data)
         *
         * \brief Read bytes in memory.
         *
         * \param       address Start address.
         * \param       length  Number of bytes to read (32 bytes max).
         * \param[in]   p_data  Buffer to fill with read bytes.
         **********************************************************************/
        void
        readBuffer
        (
            word    address,
            byte    length,
            byte*   p_data
        );
};

#endif // Eeprom24C128_256_h

