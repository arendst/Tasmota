/*
 * ATtinySerialOut.cpp
 *
 * For transmitting debug data over bit bang serial with 115200 baud for 1/8/16 MHz ATtiny clock.
 * For 1 MHz you can choose also 38400 baud (120 bytes smaller code size).
 * For 8/16 MHz you can choose also 230400 baud (just faster).
 * 1 Start, 8 Data, 1 Stop, No Parity
 *
 * Using PB2 // (Pin7 on Tiny85) as default TX pin to be compatible with digispark board
 * To change the output pin, modify the line "#define TX_PIN ..." in TinySerialOut.h or or set it as compiler symbol like "-DTX_PIN PB1".
 *
 * Using the Serial.print commands needs 4 bytes extra for each call.
 *
 *
 *  Copyright (C) 2015-2020  Armin Joachimsmeyer
 *  Email: armin.joachimsmeyer@gmail.com
 *
 *  This file is part of TinySerialOut https://github.com/ArminJo/ATtinySerialOut.
 *
 *  TinySerialOut is free software: you can redistribute it and/or modify
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

#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) \
    || defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__) \
    || defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__)
#include "ATtinySerialOut.h"
#include <avr/eeprom.h>     // for eeprom_read_byte() in writeString_E()

#ifndef _NOP
#define _NOP()  __asm__ volatile ("nop")
#endif

#ifndef PORTB
#define PORTB (*(volatile uint8_t *)((0x18) + 0x20))
#endif

#if defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__)
#  ifndef TX_PORT
#define TX_PORT PORTA
#define TX_PORT_ADDR 0x02 // PORTA
#define TX_DDR DDRA

//#define TX_PORT PORTB
//#define TX_PORT_ADDR 0x05
//#define TX_DDR DDRB
#  endif

#else
//  ATtinyX5 here
#define TX_PORT PORTB
#define TX_PORT_ADDR 0x18 // PORTB
#define TX_DDR DDRB
#endif // defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__)

void write1Start8Data1StopNoParity(uint8_t aValue);

bool sUseCliSeiForWrite = true;

void initTXPin() {
    // TX_PIN is active LOW, so set it to HIGH initially
    TX_PORT |= (1 << TX_PIN);
    // set pin direction to output
    TX_DDR |= (1 << TX_PIN);
}

void write1Start8Data1StopNoParityWithCliSei(uint8_t aValue) {
    uint8_t oldSREG = SREG;
    cli();
    write1Start8Data1StopNoParity(aValue);
    SREG = oldSREG;
}

void writeValue(uint8_t aValue) {
    write1Start8Data1StopNoParity(aValue);
}

/*
 * Used for writeString() and therefore all write<type>() and print<type>
 */
void useCliSeiForStrings(bool aUseCliSeiForWrite) {
    sUseCliSeiForWrite = aUseCliSeiForWrite;
}

/*
 * Write String residing in RAM
 */
void writeString(const char *aStringPtr) {
#ifndef USE_ALWAYS_CLI_SEI_GUARD_FOR_OUTPUT
    if (sUseCliSeiForWrite) {
#endif
        while (*aStringPtr != 0) {
            write1Start8Data1StopNoParityWithCliSei(*aStringPtr++);
        }
#ifndef USE_ALWAYS_CLI_SEI_GUARD_FOR_OUTPUT
    } else {
        while (*aStringPtr != 0) {
            write1Start8Data1StopNoParity(*aStringPtr++);
        }
    }
#endif
}

/*
 * Write string residing in program space (FLASH)
 */
void writeString_P(const char *aStringPtr) {
    uint8_t tChar = pgm_read_byte((const uint8_t * ) aStringPtr);
// Comparing with 0xFF is safety net for wrong string pointer
    while (tChar != 0 && tChar != 0xFF) {
#ifdef USE_ALWAYS_CLI_SEI_GUARD_FOR_OUTPUT
        write1Start8Data1StopNoParityWithCliSei(tChar);
#else
        if (sUseCliSeiForWrite) {
            write1Start8Data1StopNoParityWithCliSei(tChar);
        } else {
            write1Start8Data1StopNoParity(tChar);
        }
#endif
        tChar = pgm_read_byte((const uint8_t * ) ++aStringPtr);
    }
}

/*
 * Write string residing in program space (FLASH)
 */
void writeString(const __FlashStringHelper *aStringPtr) {
    PGM_P tPGMStringPtr = reinterpret_cast<PGM_P>(aStringPtr);
    uint8_t tChar = pgm_read_byte((const uint8_t * ) aStringPtr);
// Comparing with 0xFF is safety net for wrong string pointer
    while (tChar != 0 && tChar != 0xFF) {
#ifdef USE_ALWAYS_CLI_SEI_GUARD_FOR_OUTPUT
        write1Start8Data1StopNoParityWithCliSei(tChar);
#else
        if (sUseCliSeiForWrite) {
            write1Start8Data1StopNoParityWithCliSei(tChar);
        } else {
            write1Start8Data1StopNoParity(tChar);
        }
#endif
        tChar = pgm_read_byte((const uint8_t * ) ++tPGMStringPtr);
    }
}

/*
 * Write string residing in EEPROM space
 */
void writeString_E(const char *aStringPtr) {
    uint8_t tChar = eeprom_read_byte((const uint8_t *) aStringPtr);
    // Comparing with 0xFF is safety net for wrong string pointer
    while (tChar != 0 && tChar != 0xFF) {
#ifdef USE_ALWAYS_CLI_SEI_GUARD_FOR_OUTPUT
        write1Start8Data1StopNoParityWithCliSei(tChar);
#else
        if (sUseCliSeiForWrite) {
            write1Start8Data1StopNoParityWithCliSei(tChar);
        } else {
            write1Start8Data1StopNoParity(tChar);
        }
#endif
        tChar = eeprom_read_byte((const uint8_t *) ++aStringPtr);
    }
}

void writeStringWithoutCliSei(const char *aStringPtr) {
    while (*aStringPtr != 0) {
        write1Start8Data1StopNoParity(*aStringPtr++);
    }
}

void writeStringWithCliSei(const char *aStringPtr) {
    while (*aStringPtr != 0) {
        write1Start8Data1StopNoParityWithCliSei(*aStringPtr++);
    }
}

void writeStringSkipLeadingSpaces(const char *aStringPtr) {
    // skip leading spaces
    while (*aStringPtr == ' ' && *aStringPtr != 0) {
        aStringPtr++;
    }
#ifndef USE_ALWAYS_CLI_SEI_GUARD_FOR_OUTPUT
    if (sUseCliSeiForWrite) {
#endif
        while (*aStringPtr != 0) {
            write1Start8Data1StopNoParityWithCliSei(*aStringPtr++);
        }
#ifndef USE_ALWAYS_CLI_SEI_GUARD_FOR_OUTPUT
    } else {
        while (*aStringPtr != 0) {
            write1Start8Data1StopNoParity(*aStringPtr++);
        }
    }
#endif
}

void writeBinary(uint8_t aByte) {
#ifdef USE_ALWAYS_CLI_SEI_GUARD_FOR_OUTPUT
    write1Start8Data1StopNoParityWithCliSei(aByte);
#else
    if (sUseCliSeiForWrite) {
        write1Start8Data1StopNoParityWithCliSei(aByte);
    } else {
        write1Start8Data1StopNoParity(aByte);
    }
#endif
}

void writeChar(uint8_t aChar) {
    writeBinary(aChar);
}

void writeCRLF() {
    writeBinary('\r');
    writeBinary('\n');
}

void writeUnsignedByte(uint8_t aByte) {
    char tStringBuffer[4];
    utoa(aByte, tStringBuffer, 10);
    writeStringSkipLeadingSpaces(tStringBuffer);
}

/*
 * 2 Byte Hex output
 */
void writeUnsignedByteHex(uint8_t aByte) {
    char tStringBuffer[3];
    //    tStringBuffer[0] = nibbleToHex(aByte >> 4);
    //    tStringBuffer[1] = nibbleToHex(aByte);
    //    tStringBuffer[2] = '\0';
    // the utoa() version is 8 bytes smaller than the version with nibbleToHex(), if utoa() is allocated by another function.
    utoa(aByte, &tStringBuffer[0], 16);
    if (tStringBuffer[1] == '\0') {
        tStringBuffer[2] = '\0';
        tStringBuffer[1] = tStringBuffer[0];
        tStringBuffer[0] = '0';
    }
    writeString(tStringBuffer);
}

/*
 * 2 Byte Hex output with 2 Byte prefix "0x"
 */
void writeUnsignedByteHexWithPrefix(uint8_t aByte) {
    writeBinary('0');
    writeBinary('x');
    writeUnsignedByteHex(aByte);
}

char nibbleToHex(uint8_t aByte) {
    aByte = aByte & 0x0F;
    if (aByte < 10) {
        return aByte + '0';
    }
    return aByte + 'A' - 10;
}

void writeByte(int8_t aByte) {
    char tStringBuffer[5];
    itoa(aByte, tStringBuffer, 10);
    writeStringSkipLeadingSpaces(tStringBuffer);
}

void writeInt(int16_t aInteger) {
    char tStringBuffer[7];
    itoa(aInteger, tStringBuffer, 10);
    writeStringSkipLeadingSpaces(tStringBuffer);
}

void writeUnsignedInt(uint16_t aInteger) {
    char tStringBuffer[6];
    utoa(aInteger, tStringBuffer, 10);
    writeStringSkipLeadingSpaces(tStringBuffer);
}

void writeLong(int32_t aLong) {
    char tStringBuffer[12];
    ltoa(aLong, tStringBuffer, 10);
    writeStringSkipLeadingSpaces(tStringBuffer);
}

void writeUnsignedLong(uint32_t aLong) {
    char tStringBuffer[11];
    ultoa(aLong, tStringBuffer, 10);
    writeStringSkipLeadingSpaces(tStringBuffer);
}

void writeFloat(double aFloat) {
    char tStringBuffer[11];
    dtostrf(aFloat, 10, 3, tStringBuffer);
    writeStringSkipLeadingSpaces(tStringBuffer);
}

void writeFloat(double aFloat, uint8_t aDigits) {
    char tStringBuffer[11];
    dtostrf(aFloat, 10, aDigits, tStringBuffer);
    writeStringSkipLeadingSpaces(tStringBuffer);
}

/******************************************************
 * The TinySerialOut class functions which implements
 * the Serial + printHex() and printlnHex() functions
 ******************************************************/
/*
 * An alternative way to call the init function :-)
 */
void TinySerialOut::begin(long aBaudrate) {
    initTXPin();
#if defined(USE_115200BAUD) // else smaller code, but only 38400 baud at 1 MHz
    if (aBaudrate != 115200) {
        println(F("Only 115200 supported!"));
    }
#else
#if (F_CPU == 1000000)
    if (aBaudrate != 38400) {
        println(F("Only 38400 supported!"));
    }
#else
    if (aBaudrate != 230400) {
        println(F("Only 230400 supported!"));
    }
#endif
#endif
}

void TinySerialOut::end() {
    // no action needed
}

void TinySerialOut::flush() {
    // no action needed, since we do not use a buffer
}

/*
 * 2 Byte Hex output with 2 Byte prefix "0x"
 */
void TinySerialOut::printHex(uint8_t aByte) {
    writeUnsignedByteHexWithPrefix(aByte);
}

void TinySerialOut::printHex(uint16_t aWord) {
    writeUnsignedByteHexWithPrefix(aWord >> 8);
    writeUnsignedByteHex(aWord);
}

void TinySerialOut::printlnHex(uint8_t aByte) {
    printHex(aByte);
    println();
}

void TinySerialOut::printlnHex(uint16_t aWord) {
    printHex(aWord);
    println();
}

// virtual functions of Print class
size_t TinySerialOut::write(uint8_t aByte) {
    writeBinary(aByte);
    return 1;
}

void TinySerialOut::print(const char *aStringPtr) {
    writeString(aStringPtr);
}

void TinySerialOut::print(const __FlashStringHelper *aStringPtr) {
    writeString(aStringPtr);
}

void TinySerialOut::print(char aChar) {
    writeBinary(aChar);
}

void TinySerialOut::print(uint8_t aByte, uint8_t aBase) {
    if (aBase == 16) {
        /*
         * Print Hex always with two characters
         */
        writeUnsignedByteHex(aByte);
    } else {
        char tStringBuffer[4];
        utoa(aByte, tStringBuffer, aBase);
        writeStringSkipLeadingSpaces(tStringBuffer);
    }
}

void TinySerialOut::print(int16_t aInteger, uint8_t aBase) {
    char tStringBuffer[7];
    itoa(aInteger, tStringBuffer, aBase);
    writeStringSkipLeadingSpaces(tStringBuffer);
}

void TinySerialOut::print(uint16_t aInteger, uint8_t aBase) {
    char tStringBuffer[6];
    utoa(aInteger, tStringBuffer, aBase);
    writeStringSkipLeadingSpaces(tStringBuffer);
}

void TinySerialOut::print(int32_t aLong, uint8_t aBase) {
    char tStringBuffer[12];
    ltoa(aLong, tStringBuffer, aBase);
    writeStringSkipLeadingSpaces(tStringBuffer);
}

void TinySerialOut::print(uint32_t aLong, uint8_t aBase) {
    char tStringBuffer[11];
    ultoa(aLong, tStringBuffer, aBase);
    writeStringSkipLeadingSpaces(tStringBuffer);
}

void TinySerialOut::print(double aFloat, uint8_t aDigits) {
    char tStringBuffer[11];
    dtostrf(aFloat, 10, aDigits, tStringBuffer);
    writeStringSkipLeadingSpaces(tStringBuffer);
}

void TinySerialOut::println(char aChar) {
    print(aChar);
    println();
}

void TinySerialOut::println(const char *aStringPtr) {
    print(aStringPtr);
    println();
}

void TinySerialOut::println(const __FlashStringHelper *aStringPtr) {
    print(aStringPtr);
    println();
}

void TinySerialOut::println(uint8_t aByte, uint8_t aBase) {
    print(aByte, aBase);
    println();
}

void TinySerialOut::println(int16_t aInteger, uint8_t aBase) {
    print(aInteger, aBase);
    println();
}

void TinySerialOut::println(uint16_t aInteger, uint8_t aBase) {
    print(aInteger, aBase);
    println();
}

void TinySerialOut::println(int32_t aLong, uint8_t aBase) {
    print(aLong, aBase);
    println();
}

void TinySerialOut::println(uint32_t aLong, uint8_t aBase) {
    print(aLong, aBase);
    println();
}

void TinySerialOut::println(double aFloat, uint8_t aDigits) {
    print(aFloat, aDigits);
    println();
}

void TinySerialOut::println() {
    print('\r');
    print('\n');
}

/*
 * The Serial Instance!!!
 */
// #if ... to be compatible with ATTinyCores and AttinyDigisparkCores
#if (!defined(UBRRH) && !defined(UBRR0H)) /*AttinyDigisparkCore and AttinyDigisparkCore condition*/ \
    || USE_SOFTWARE_SERIAL /*AttinyDigisparkCore condition*/\
    || ((defined(UBRRH) || defined(UBRR0H) || defined(UBRR1H) || defined(LINBRRH)) && !USE_SOFTWARE_SERIAL)/*AttinyDigisparkCore condition for HardwareSerial*/
// Switch to SerialOut since Serial is already defined
// or activate line 745 in TinyDebugSerial.h included in AttinyDigisparkCores/src/tiny/WProgram.h at line 24 for AttinyDigisparkCores
TinySerialOut SerialOut;
#else
TinySerialOut Serial;
#endif

/********************************
 * Basic serial output function
 *******************************/

inline void delay4CyclesInlineExact(uint16_t a4Microseconds) {
    /*
     * The loop takes 4 cycles (4 microseconds  at 1 MHz). Last loop is only 3 cycles. Setting of loop counter a4Microseconds needs 2 cycles
     * 3 -> 13 cycles (3*4 -1 + 2) = 3*4 + 1
     * 4 -> 17 cycles
     * 5 -> 21 cycles
     */
    asm volatile (
            "1: sbiw %0,1" "\n\t" // 2 cycles
            "brne .-4" : "=w" (a4Microseconds) : "0" (a4Microseconds)// 2 cycles
    );
}

#if (F_CPU == 1000000) && defined(USE_115200BAUD) //else smaller code, but only 38400 baud at 1 MHz
/*
 * 115200 baud - 8,680 cycles per bit, 86,8 per byte at 1 MHz
 *
 *  Assembler code for 115200 baud extracted from Digispark core files:
 *  Code size is 196 Byte (including first call)
 *
 *   TinySerialOut.h - Tiny write-only software serial.
 *   Copyright 2010 Rowdy Dog Software. This code is part of Arduino-Tiny.
 *
 *   Arduino-Tiny is free software: you can redistribute it and/or modify it
 *   under the terms of the GNU Lesser General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or (at your
 *   option) any later version.
 */
void write1Start8Data1StopNoParity(uint8_t aValue) {
    asm volatile
    (
            "cbi   %[txport], %[txpin]" "\n\t" /* 2  <--- 0 */
            "ror   %[value]" "\n\t" /* 1 */
            "nop" "\n\t" /* 1 */
            "nop" "\n\t" /* 1 */
            "nop" "\n\t" /* 1 */
            "nop" "\n\t" /* 1 */

            "brcs  L%=b0h" "\n\t" /* 1  (not taken) */
            "nop" "\n\t" /* 1 */
            "cbi   %[txport], %[txpin]" "\n\t" /* 2  <--- st is 9 cycles */
            "rjmp  L%=b0z" "\n\t" /* 2 */
            "L%=b0h: " /* 2  (taken) */
            "sbi   %[txport], %[txpin]" "\n\t" /* 2  <--- st is 9 cycles */
            "nop" "\n\t" /* 1 */
            "nop" "\n\t" /* 1 */
            "L%=b0z: "
            "ror   %[value]" "\n\t" /* 1 */

            "nop" "\n\t" /* 1 */

            "brcs  L%=b1h" "\n\t" /* 1  (not taken) */
            "nop" "\n\t" /* 1 */
            "cbi   %[txport], %[txpin]" "\n\t" /* 2  <--- b0 is 8 cycles */
            "rjmp  L%=b1z" "\n\t" /* 2 */
            "L%=b1h: " /* 2  (taken) */
            "sbi   %[txport], %[txpin]" "\n\t" /* 2  <--- b0 is 8 cycles */
            "nop" "\n\t" /* 1 */
            "nop" "\n\t" /* 1 */
            "L%=b1z: "
            "ror   %[value]" "\n\t" /* 1 */

            "nop" "\n\t" /* 1 */
            "nop" "\n\t" /* 1 */

            "brcs  L%=b2h" "\n\t" /* 1  (not taken) */
            "nop" "\n\t" /* 1 */
            "cbi   %[txport], %[txpin]" "\n\t" /* 2  <--- b1 is 9 cycles */
            "rjmp  L%=b2z" "\n\t" /* 2 */
            "L%=b2h: " /* 2  (taken) */
            "sbi   %[txport], %[txpin]" "\n\t" /* 2  <--- b1 is 9 cycles */
            "nop" "\n\t" /* 1 */
            "nop" "\n\t" /* 1 */
            "L%=b2z: "
            "ror   %[value]" "\n\t" /* 1 */

            "nop" "\n\t" /* 1 */
            "nop" "\n\t" /* 1 */

            "brcs  L%=b3h" "\n\t" /* 1  (not taken) */
            "nop" "\n\t" /* 1 */
            "cbi   %[txport], %[txpin]" "\n\t" /* 2  <--- b2 is 9 cycles */
            "rjmp  L%=b3z" "\n\t" /* 2 */
            "L%=b3h: " /* 2  (taken) */
            "sbi   %[txport], %[txpin]" "\n\t" /* 2  <--- b2 is 9 cycles */
            "nop" "\n\t" /* 1 */
            "nop" "\n\t" /* 1 */
            "L%=b3z: "
            "ror   %[value]" "\n\t" /* 1 */

            "nop" "\n\t" /* 1 */

            "brcs  L%=b4h" "\n\t" /* 1  (not taken) */
            "nop" "\n\t" /* 1 */
            "cbi   %[txport], %[txpin]" "\n\t" /* 2  <--- b3 is 8 cycles */
            "rjmp  L%=b4z" "\n\t" /* 2 */
            "L%=b4h: " /* 2  (taken) */
            "sbi   %[txport], %[txpin]" "\n\t" /* 2  <--- b3 is 8 cycles */
            "nop" "\n\t" /* 1 */
            "nop" "\n\t" /* 1 */
            "L%=b4z: "
            "ror   %[value]" "\n\t" /* 1 */

            "nop" "\n\t" /* 1 */
            "nop" "\n\t" /* 1 */

            "brcs  L%=b5h" "\n\t" /* 1  (not taken) */
            "nop" "\n\t" /* 1 */
            "cbi   %[txport], %[txpin]" "\n\t" /* 2  <--- b4 is 9 cycles */
            "rjmp  L%=b5z" "\n\t" /* 2 */
            "L%=b5h: " /* 2  (taken) */
            "sbi   %[txport], %[txpin]" "\n\t" /* 2  <--- b4 is 9 cycles */
            "nop" "\n\t" /* 1 */
            "nop" "\n\t" /* 1 */
            "L%=b5z: "
            "ror   %[value]" "\n\t" /* 1 */

            "nop" "\n\t" /* 1 */
            "nop" "\n\t" /* 1 */

            "brcs  L%=b6h" "\n\t" /* 1  (not taken) */
            "nop" "\n\t" /* 1 */
            "cbi   %[txport], %[txpin]" "\n\t" /* 2  <--- b5 is 9 cycles */
            "rjmp  L%=b6z" "\n\t" /* 2 */
            "L%=b6h: " /* 2  (taken) */
            "sbi   %[txport], %[txpin]" "\n\t" /* 2  <--- b5 is 9 cycles */
            "nop" "\n\t" /* 1 */
            "nop" "\n\t" /* 1 */
            "L%=b6z: "
            "ror   %[value]" "\n\t" /* 1 */

            "nop" "\n\t" /* 1 */

            "brcs  L%=b7h" "\n\t" /* 1  (not taken) */
            "nop" "\n\t" /* 1 */
            "cbi   %[txport], %[txpin]" "\n\t" /* 2  <--- b6 is 8 cycles */
            "rjmp  L%=b7z" "\n\t" /* 2 */
            "L%=b7h: " /* 2  (taken) */
            "sbi   %[txport], %[txpin]" "\n\t" /* 2  <--- b6 is 8 cycles */
            "nop" "\n\t" /* 1 */
            "nop" "\n\t" /* 1 */
            "L%=b7z: "
            "nop" "\n\t" /* 1 */

            "nop" "\n\t" /* 1 */
            "nop" "\n\t" /* 1 */

            "nop" "\n\t" /* 1 */
            "nop" "\n\t" /* 1 */
            "sbi   %[txport], %[txpin]" "\n\t" /* 2  <--- b7 is 9 cycles */
            "nop" "\n\t" /* 1 */
            "nop" "\n\t" /* 1 */
            "nop" "\n\t" /* 1 */
            "nop" "\n\t" /* 1 */
            "nop" "\n\t" /* 1 */
            "nop" "\n\t" /* 1 */
            "nop" "\n\t" /* 1 */
            /*    <---sp is 9 cycles */

            :
            :
            [value] "r" ( aValue ),
            [txport] "I" ( TX_PORT_ADDR ),
            [txpin] "I" ( TX_PIN )
    );
}
#else
/*
 * Small code using loop. Code size is 76 Byte (including first call)
 *
 * 1 MHz CPU Clock
 *  26,04 cycles per bit, 260,4 per byte for 38400 baud at 1 MHz Clock
 *  17,36 cycles per bit, 173,6 per byte for 57600 baud at 1 MHz Clock -> therefore use 38400 baud
 *  24 cycles between each cbi/sbi (Clear/Set Bit in IO-register) command.
 *
 * 8 MHz CPU Clock
 *  69,44 cycles per bit, 694,4 per byte for 115200 baud at 8 MHz Clock
 *  34,72 cycles per bit, 347,2 per byte for 230400 baud at 8 MHz Clock.
 *  68 / 33 cycles between each cbi (Clear Bit in IO-register) or sbi command.
 *
 * 16 MHz CPU Clock
 *  138,88 cycles per bit, 1388,8 per byte for 115200 baud at 16 MHz Clock
 *  69,44 cycles per bit, 694,4 per byte for 230400 baud at 16 MHz Clock
 *  137 / 68 cycles between each cbi (Clear Bit in IO-register) or sbi command.
 *
 * 2 cycles for each cbi/sbi instruction.
 */
void write1Start8Data1StopNoParity(uint8_t aValue) {
    asm volatile
    (
            "cbi  %[txport] , %[txpin]" "\n\t" // 2    PORTB &= ~(1 << TX_PIN);
#if (F_CPU == 1000000) && !defined(USE_115200BAUD) // 1 MHz 38400 baud
            // 0 cycles padding to get additional 4 cycles
            //delay4CyclesInlineExact(5); -> 20 cycles
            "ldi  r30 , 0x05" "\n\t"// 1
#elif ((F_CPU == 8000000) && defined(USE_115200BAUD)) || ((F_CPU == 16000000) && !defined(USE_115200BAUD)) // 8 MHz 115200 baud OR 16 MHz 230400 baud
            // 3 cycles padding to get additional 7 cycles
            "nop" "\n\t"// 1    _nop"();
            "nop" "\n\t"// 1    _nop"();
            "nop" "\n\t"// 1    _nop"();
            //delay4CyclesInlineExact(15); -> 61 cycles
            "ldi  r30 , 0x0F" "\n\t"// 1
#elif (F_CPU == 8000000) && !defined(USE_115200BAUD) // 8 MHz 230400 baud
            // 0 cycles padding to get additional 4 cycles
            //delay4CyclesInlineExact(7); -> 29 cycles
            "ldi  r30 , 0x07" "\n\t"// 1
#elif (F_CPU == 16000000) && defined(USE_115200BAUD) // 16 MHz 115200 baud
            // 0 cycles padding to get additional 4 cycles
            //delay4CyclesInlineExact(33); -> 133 cycles
            "ldi  r30 , 0x21" "\n\t"// 1
#endif
            "ldi  r31 , 0x00" "\n\t"            // 1
            "delay1:"
            "sbiw r30 , 0x01" "\n\t"// 2
            "brne delay1" "\n\t"// 1-2

            "ldi r25 , 0x08" "\n\t"// 1

            // Start of loop
            // if (aValue & 0x01) {
            "txloop:"
            "sbrs %[value] , 0" "\n\t"// 1
            "rjmp .+6" "\n\t"// 2

            "nop" "\n\t"// 1
            "sbi %[txport] , %[txpin]" "\n\t"// 2    PORTB |= 1 << TX_PIN;
            "rjmp .+6" "\n\t"// 2

            "cbi %[txport] , %[txpin]" "\n\t"// 2    PORTB &= ~(1 << TX_PIN);
            "nop" "\n\t"// 1
            "nop" "\n\t"// 1
            "lsr %[value]" "\n\t"// 1    aValue = aValue >> 1;

#if (F_CPU == 1000000) && !defined(USE_115200BAUD) // 1 MHz 38400 baud
            // 3 cycles padding to get additional 11 cycles
            "nop" "\n\t"// 1
            "nop" "\n\t"// 1
            "nop" "\n\t"// 1
            // delay4CyclesInlineExact(3); -> 13 cycles
            "ldi  r30 , 0x03" "\n\t"// 1
#elif ((F_CPU == 8000000) && defined(USE_115200BAUD)) || ((F_CPU == 16000000) && !defined(USE_115200BAUD)) // 8 MHz 115200 baud OR 16 MHz 230400 baud
            // 3 cycles padding to get additional 11 cycles
            "nop" "\n\t"// 1
            "nop" "\n\t"// 1
            "nop" "\n\t"// 1
            // delay4CyclesInlineExact(14); -> 57 cycles
            "ldi r30 , 0x0E" "\n\t"// 1
#elif (F_CPU == 8000000) && !defined(USE_115200BAUD) // 8 MHz 230400 baud
            // 0 cycles padding to get additional 8 cycles
            // delay4CyclesInlineExact(6); -> 25 cycles
            "ldi r30 , 0x05" "\n\t"// 1
#elif (F_CPU == 16000000) && defined(USE_115200BAUD) // 16 MHz 115200 baud
            // 0 cycles padding to get additional 8 cycles
            //delay4CyclesInlineExact(32); -> 129 cycles
            "ldi  r30 , 0x20" "\n\t"// 1
#endif
            "ldi r31 , 0x00" "\n\t"            // 1
            "delay2:"
            "sbiw r30 , 0x01" "\n\t"// 2
            "brne delay2" "\n\t"// 1-2

            // }while (i > 0);
            "subi r25 , 0x01" "\n\t"// 1
            "brne txloop" "\n\t"// 1-2
            // To compensate for missing loop cycles at last bit
            "nop" "\n\t"// 1
            "nop" "\n\t"// 1
            "nop" "\n\t"// 1
            "nop" "\n\t"// 1

            // Stop bit
            "sbi %[txport] , %[txpin]" "\n\t"// 2    PORTB |= 1 << TX_PIN;

#if (F_CPU == 1000000) && !defined(USE_115200BAUD) // 1 MHz 38400 baud
            // delay4CyclesInlineExact(4); -> 17 cycles - gives minimum 25 cycles for stop bit
            "ldi  r30 , 0x04" "\n\t"// 1
#elif ((F_CPU == 8000000) && defined(USE_115200BAUD)) || ((F_CPU == 16000000) && !defined(USE_115200BAUD)) // 8 MHz 115200 baud OR 16 MHz 230400 baud
            // delay4CyclesInlineExact(15) -> 61 cycles - gives minimum 69 cycles for stop bit
            "ldi r30 , 0x0F" "\n\t"// 1
#elif (F_CPU == 8000000) && !defined(USE_115200BAUD) // 8 MHz 230400 baud
            // delay4CyclesInlineExact(5) -> 27 cycles - gives minimum 35 cycles for stop bit
            "ldi r30 , 0x05" "\n\t"// 1
#elif (F_CPU == 16000000) && defined(USE_115200BAUD) // 16 MHz 115200 baud
            // delay4CyclesInlineExact(32) -> 129 cycles - gives minimum 137 cycles for stop bit
            "ldi r30 , 0x20" "\n\t"// 1
#endif
            "ldi r31 , 0x00" "\n\t"            // 1
            "delay3:"
            "sbiw r30 , 0x01" "\n\t"//
            "brne delay3" "\n\t"// 1-2
            // return needs 4 cycles, load of next value needs 1 cycle, next rcall needs 3 cycles -> gives additional 8 cycles minimum for stop bit

            :
            :
            [value] "r" ( aValue ),
            [txport] "I" ( TX_PORT_ADDR ) , /* 0x18 is PORTB on Attiny 85 */
            [txpin] "I" ( TX_PIN )
            :
            "r25",
            "r30",
            "r31"
    );

}
#endif

/*
 * C Version which generates the assembler code above.
 *      In order to guarantee the correct timing, compile with Arduino standard settings or:
 *      avr-g++ -I"C:\arduino\hardware\arduino\avr\cores\arduino" -I"C:\arduino\hardware\arduino\avr\variants\standard" -c -g -w -Os -ffunction-sections -fdata-sections -mmcu=attiny85 -DF_CPU=1000000UL -MMD -o "TinySerialOut.o" "TinySerialOut.cpp"
 *      Tested with Arduino 1.6.8 and 1.8.5/gcc4.9.2
 *      C Version does not work with AVR gcc7.3.0, since optimization is too bad
 */
void write1Start8Data1StopNoParity_C_Version(uint8_t aValue) {
    /*
     * C Version here for 38400 baud at 1 MHz Clock. You see, it is simple :-)
     */
// start bit
    TX_PORT &= ~(1 << TX_PIN);
    _NOP();
    delay4CyclesInlineExact(4);

// 8 data bits
    uint8_t i = 8;
    do {
        if (aValue & 0x01) {
            // bit=1
            // to compensate for jump at data=0
            _NOP();
            TX_PORT |= 1 << TX_PIN;
        } else {
            // bit=0
            TX_PORT &= ~(1 << TX_PIN);
            // compensate for different cycles of sbrs
            _NOP();
            _NOP();
        }
        aValue = aValue >> 1;
        // 3 cycles padding
        _NOP();
        _NOP();
        _NOP();
        delay4CyclesInlineExact(3);
        --i;
    } while (i > 0);

// to compensate for missing loop cycles at last bit
    _NOP();
    _NOP();
    _NOP();
    _NOP();

// Stop bit
    TX_PORT |= 1 << TX_PIN;
// -8 cycles to compensate for fastest repeated call (1 ret + 1 load + 1 call)
    delay4CyclesInlineExact(4); // gives minimum 25 cycles for stop bit :-)
}
#elif defined(ARDUINO_ARCH_APOLLO3)
    void AttinySerialOutDummyToAvoidBFDAssertions(){
        ;
    }
#endif // defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__)
