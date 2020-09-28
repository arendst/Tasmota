/*
 * IRCommandDispatcher.cpp.h
 *
 * Library to process IR commands by calling functions specified in a mapping array.
 *
 * To run this example need to install the "IRLremote" and "PinChangeInterrupt" libraries under "Tools -> Manage Libraries..." or "Ctrl+Shift+I"
 * Use "IRLremote" and "PinChangeInterrupt" as filter string.
 *
 *  Copyright (C) 2019-2020  Armin Joachimsmeyer
 *  armin.joachimsmeyer@gmail.com
 *
 *  This file is part of ServoEasing https://github.com/ArminJo/ServoEasing.
 *  This file is part of IRMP https://github.com/ukw100/IRMP.
 *  This file is part of Arduino-IRremote https://github.com/z3t0/Arduino-IRremote.
 *
 *  ServoEasing is free software: you can redistribute it and/or modify
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
 */

#include <Arduino.h>

#include "IRCommandDispatcher.h"

//#define INFO // activate this out to see serial info output
//#define DEBUG // activate this out to see serial info output
#ifdef INFO
#  ifndef DEBUG
#define DEBUG
#  endif
#endif

IRCommandDispatcher IRDispatcher;

#if defined(USE_TINY_IR_RECEIVER)
#include "TinyIRReceiver.cpp.h"

void IRCommandDispatcher::init() {
    initPCIInterruptForTinyReceiver();
}

/*
 * This is the function is called if a complete command was received
 */
#if defined(ESP8266)
void ICACHE_RAM_ATTR handleReceivedTinyIRData(uint16_t aAddress, uint8_t aCommand, bool isRepeat)
#elif defined(ESP32)
void IRAM_ATTR handleReceivedTinyIRData(uint16_t aAddress, uint8_t aCommand, bool isRepeat)
#else
void handleReceivedTinyIRData(uint16_t aAddress, uint8_t aCommand, bool isRepeat)
#endif
{
    IRDispatcher.IRReceivedData.address = aAddress;
    IRDispatcher.IRReceivedData.command = aCommand;
    IRDispatcher.IRReceivedData.isRepeat = isRepeat;
    IRDispatcher.IRReceivedData.MillisOfLastCode = millis();
    IRDispatcher.IRReceivedData.isAvailable = true;
#ifdef INFO
    Serial.print(F("A=0x"));
    Serial.print(aAddress, HEX);
    Serial.print(F(" C=0x"));
    Serial.print(aCommand, HEX);
    if (isRepeat) {
        Serial.print(F("R"));
    }
    Serial.println();
#endif
    if (aAddress == IR_ADDRESS) {
        IRDispatcher.loop(false); // cannot use IRDispatcher.loop as parameter for irmp_register_complete_callback_function
#ifdef INFO
        } else {
        Serial.print(F(" Wrong address. Expected 0x"));
        Serial.println(IR_ADDRESS, HEX);
#endif
    }
}

#elif defined(USE_IRMP_LIBRARY)
void IRCommandDispatcher::init() {
    irmp_init();
}

/*
 * This is the function is called if a complete command was received
 */
#if defined(ESP8266)
void ICACHE_RAM_ATTR handleReceivedIRData()
#elif defined(ESP32)
void IRAM_ATTR handleReceivedIRData()
#else
void handleReceivedIRData()
#endif
{
    IRMP_DATA tTeporaryData;
    irmp_get_data(&tTeporaryData);
    IRDispatcher.IRReceivedData.address = tTeporaryData.address;
    IRDispatcher.IRReceivedData.command = tTeporaryData.command;
    IRDispatcher.IRReceivedData.isRepeat = tTeporaryData.flags & IRMP_FLAG_REPETITION;
    IRDispatcher.IRReceivedData.MillisOfLastCode = millis();
    IRDispatcher.IRReceivedData.isAvailable = true;
#ifdef INFO
    Serial.print(F("A=0x"));
    Serial.print(IRDispatcher.IRReceivedData.address, HEX);
    Serial.print(F(" C=0x"));
    Serial.print(IRDispatcher.IRReceivedData.command, HEX);
    if (IRDispatcher.IRReceivedData.isRepeat) {
        Serial.print(F("R"));
    }
    Serial.println();
#endif
    // To enable delay() for commands
#if !defined(ARDUINO_ARCH_MBED)
    interrupts(); // be careful with always executable commands which lasts longer than the IR repeat duration.
#endif

    if (IRDispatcher.IRReceivedData.address == IR_ADDRESS) {
        IRDispatcher.loop(false); // cannot use IRDispatcher.loop as parameter for irmp_register_complete_callback_function
#ifdef INFO
        } else {
        Serial.print(F(" Wrong address. Expected 0x"));
        Serial.println(IR_ADDRESS, HEX);
#endif
    }
}
#endif

/*
 * Resets stop flag, gets new command, checks and runs it.
 *
 * @param aRunRejectedCommand if true run a command formerly rejected because of recursive calling.
 */
void IRCommandDispatcher::loop(bool aRunRejectedCommand) {
    /*
     * search IR code or take last rejected command and call associated function
     */
    if (aRunRejectedCommand && (rejectedRegularCommand != COMMAND_INVALID)) {
#ifdef INFO
        Serial.print(F("Take rejected command = 0x"));
        Serial.println(rejectedRegularCommand, HEX);
#endif
        IRReceivedData.command = rejectedRegularCommand;
        rejectedRegularCommand = COMMAND_INVALID;
        IRReceivedData.isRepeat = false;
        checkAndCallCommand();
    }

    if (IRReceivedData.isAvailable) {
        IRReceivedData.isAvailable = false;
        checkAndCallCommand();
    }
}

/*
 * Sets flags justCalledRegularIRCommand, executingRegularCommand
 */
uint8_t IRCommandDispatcher::checkAndCallCommand() {
    if (IRReceivedData.command == COMMAND_EMPTY) {
        return IR_CODE_EMPTY;
    }

    for (uint8_t i = 0; i < sizeof(IRMapping) / sizeof(struct IRToCommandMappingStruct); ++i) {
        if (IRReceivedData.command == IRMapping[i].IRCode) {

#ifdef INFO
            const __FlashStringHelper *tCommandName = reinterpret_cast<const __FlashStringHelper*>(IRMapping[i].CommandString);
#endif
            /*
             * Check for repeat and if it is allowed for the current command
             */
            if (IRReceivedData.isRepeat && !(IRMapping[i].Flags & IR_COMMAND_FLAG_REPEATABLE)) {
#ifdef DEBUG
                Serial.print(F("Repeats of command \""));
                Serial.print(tCommandName);
                Serial.println("\" not accepted");
#endif
                return FOUND_BUT_REPEAT_NOT_ACCEPTED;
            }

            /*
             * Do not accept recursive call of the same command
             */
            if (currentRegularCommandCalled == IRReceivedData.command) {
#ifdef DEBUG
                Serial.print(F("Recursive command \""));
                Serial.print(tCommandName);
                Serial.println("\" not accepted");
#endif
                return FOUND_BUT_REPEAT_NOT_ACCEPTED;
            }

            /*
             * Handle stop command and requestToStopReceived flag
             */
            if (IRMapping[i].Flags & IR_COMMAND_FLAG_IS_STOP_COMMAND) {
                requestToStopReceived = true;
#ifdef INFO
                Serial.println(F("Stop command received"));
#endif
            } else {
                // lets start a new turn
                requestToStopReceived = false;
            }

#ifdef INFO
            Serial.print(F("Found command: "));
            Serial.println(tCommandName);
#endif

            bool tIsRegularCommand = !(IRMapping[i].Flags & IR_COMMAND_FLAG_EXECUTE_ALWAYS);
            if (tIsRegularCommand) {
                if (executingRegularCommand) {
                    /*
                     * A regular command may not be called as long as another regular command is running.
                     */
                    rejectedRegularCommand = IRReceivedData.command;
#ifdef INFO
                    Serial.println(F("Regular command rejected, since another regular command is already running"));
#endif
                    return FOUND_BUT_RECURSIVE_LOCK;
                }

                justCalledRegularIRCommand = true;
                executingRegularCommand = true; // set lock for recursive calls
                currentRegularCommandCalled = IRReceivedData.command;
                /*
                 * This call may be blocking!!!
                 */
                IRMapping[i].CommandToCall();
                executingRegularCommand = false;
                currentRegularCommandCalled = COMMAND_INVALID;
            } else {
                // short command here, just call
                IRMapping[i].CommandToCall();
            }
            return CALLED;
        }
    }
    return IR_CODE_NOT_FOUND;
}

void IRCommandDispatcher::setRequestToStopReceived() {
    requestToStopReceived = true;
}

/*
 * @return  true (and sets requestToStopReceived) if invalid or recursive regular IR command received
 */
bool IRCommandDispatcher::checkIRInputForAlwaysExecutableCommand() {
    uint8_t tCheckResult;
    if (IRDispatcher.IRReceivedData.isAvailable) {
        IRReceivedData.isAvailable = false;
        tCheckResult = checkAndCallCommand();
        if ((tCheckResult == IR_CODE_NOT_FOUND) || (tCheckResult == FOUND_BUT_RECURSIVE_LOCK)) {
            // IR command not found in mapping or received a recursive (while just running another one) regular command -> request stop
#ifdef INFO
            Serial.println(F("Invalid or recursive regular command received -> set stop"));
#endif
            requestToStopReceived = true; // return to loop
            return true;
        }
    }
    return false;
}

/*
 * Special delay function for the IRCommandDispatcher.
 * @return  true - if stop received
 */
bool IRCommandDispatcher::delayAndCheckForIRCommand(uint16_t aDelayMillis) {
    uint32_t tStartMillis = millis();

    do {
        if (IRDispatcher.checkIRInputForAlwaysExecutableCommand()) {
            return true;
        }

    } while (millis() - tStartMillis < aDelayMillis);
    return false;
}

void IRCommandDispatcher::printIRCommandString() {
#ifdef INFO
    Serial.print(F("IRCommand="));
    for (uint8_t i = 0; i < sizeof(IRMapping) / sizeof(struct IRToCommandMappingStruct); ++i) {
        if (IRReceivedData.command == IRMapping[i].IRCode) {
            Serial.println(reinterpret_cast<const __FlashStringHelper*>(IRMapping[i].CommandString));
            return;
        }
    }
    Serial.println(reinterpret_cast<const __FlashStringHelper*>(unknown));
#endif
}

