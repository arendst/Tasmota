/*
 * Unions.h
 *
 *  Copyright (C) 2020  Armin Joachimsmeyer
 *  Email: armin.joachimsmeyer@gmail.com
 *
 *  This file is part of Arduino-Utils https://github.com/ArminJo/Arduino-Utils.
 *
 *  Arduino-Utils is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/gpl.html>.
 *
 */

#ifndef LONG_UNION_H
#define LONG_UNION_H

#include <Arduino.h>
#include <stdint.h>

/*
 * Sometimes it helps the compiler if you use this unions
 */
union WordUnion {
    struct {
        uint8_t LowByte;
        uint8_t HighByte;
    } UByte;
    struct {
        int8_t LowByte;
        int8_t HighByte;
    } Byte;
    uint8_t UBytes[2];
    int8_t Bytes[2];
    uint16_t UWord;
    int16_t Word;
    uint8_t * BytePointer;
};

union LongUnion {
    struct {
        uint8_t LowByte;
        uint8_t MidLowByte;
        uint8_t MidHighByte;
        uint8_t HighByte;
    } UByte;
    struct {
        int8_t LowByte;
        int8_t MidLowByte;
        int8_t MidHighByte;
        int8_t HighByte;
    } Byte;
    struct {
        uint8_t LowByte;
        WordUnion MidWord;
        uint8_t HighByte;
    } ByteWord;
    struct {
        int16_t LowWord;
        int16_t HighWord;
    } Word;
    struct {
        WordUnion LowWord;
        WordUnion HighWord;
    } WordUnion;
    struct {
        uint16_t LowWord;
        uint16_t HighWord;
    } UWord;
    uint8_t UBytes[4];
    int8_t Bytes[4];
    uint16_t UWords[2];
    int16_t Words[2];
    uint32_t ULong;
    int32_t Long;
};

#endif // LONG_UNION_H

#pragma once
