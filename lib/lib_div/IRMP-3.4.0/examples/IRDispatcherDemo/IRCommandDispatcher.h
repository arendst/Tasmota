/*
 * IRCommandDispatcher.h
 *
 *  Created on: 21.05.2019
 *      Author: Armin
 */

#ifndef SRC_IRCOMMANDDISPATCHER_H_
#define SRC_IRCOMMANDDISPATCHER_H_

#include <stdint.h>

#if ! defined(IR_RECEIVER_PIN)
#define IR_RECEIVER_PIN  A0
#endif

/*
 * For command mapping file
 */
#define IR_COMMAND_FLAG_REGULAR             0x00 // default - repeat not accepted, only one command at a time
#define IR_COMMAND_FLAG_REPEATABLE          0x01 // repeat accepted
#define IR_COMMAND_FLAG_EXECUTE_ALWAYS      0x02 // (Non blocking) Command that can be processed any time and may interrupt other IR commands - used for stop etc.
#define IR_COMMAND_FLAG_REPEATABLE_EXECUTE_ALWAYS (IR_COMMAND_FLAG_REPEATABLE | IR_COMMAND_FLAG_EXECUTE_ALWAYS)
/*
 * if this command is received, requestToStopReceived is set until call of next loop.
 * This stops ongoing commands which use:  RDispatcher.delayAndCheckForIRCommand(100);  RETURN_IF_STOP;
 */
#define IR_COMMAND_FLAG_IS_STOP_COMMAND     0x04 // implies IR_COMMAND_FLAG_EXECUTE_ALWAYS

// Basic mapping structure
struct IRToCommandMappingStruct {
    uint8_t IRCode;
    uint8_t Flags;
    void (*CommandToCall)();
    const char * CommandString;
};

struct IRDataForCommandDispatcherStruct {
    uint16_t address;           // to distinguish between multiple senders
    uint16_t command;
    bool isRepeat;
    bool isAvailable;           // Flag set by ISR for new data and reset by consumer
    uint32_t MillisOfLastCode;  // millis() of last IR command received - for timeouts etc.
};

/*
 * Special codes (hopefully) not sent by the remote - otherwise please redefine it here
 */
#define COMMAND_EMPTY       0xFE // code no command received
#define COMMAND_INVALID     0xFF // code for command received, but not in mapping

#define RETURN_IF_STOP if (IRDispatcher.requestToStopReceived) return
/*
 * Return values of loopIRDispatcher and checkAndCallCommand
 */
#define CALLED 0
#define IR_CODE_EMPTY 1
#define NOT_CALLED_MASK 0x02
#define FOUND_BUT_RECURSIVE_LOCK 2
#define FOUND_BUT_REPEAT_NOT_ACCEPTED 3
#define NOT_FOUND_MASK 0x04
#define IR_CODE_NOT_FOUND 4

class IRCommandDispatcher {
public:
    void init();
    void loop(bool aRunRejectedCommand = true);
    void printIRCommandString();
    void setRequestToStopReceived();

    bool checkIRInputForAlwaysExecutableCommand(); // Used by delayAndCheckForIRCommand()
    bool delayAndCheckForIRCommand(uint16_t aDelayMillis);

    uint8_t currentRegularCommandCalled = COMMAND_INVALID; // The code for the current called command
    bool executingRegularCommand = false;               // Lock for recursive calls of regular commands
    bool justCalledRegularIRCommand = false;            // Flag that a regular command was received and called - is set before call of command
    uint8_t rejectedRegularCommand = COMMAND_INVALID;   // Storage for rejected command to allow the current command to end, before it is called by main loop
    /*
     * Flag for main loop, set by checkIRInputForAlwaysExecutableCommand().
     * It works like an exception so we do not need to propagate the return value from the delay up to the movements.
     * Instead we can use "if (requestToStopReceived) return;" (available as macro RETURN_IF_STOP).
     */
    bool requestToStopReceived;

    struct IRDataForCommandDispatcherStruct IRReceivedData;

    /*
     * Functions used internally
     */
    uint8_t checkAndCallCommand();
};

extern IRCommandDispatcher IRDispatcher;

#endif /* SRC_IRCOMMANDDISPATCHER_H_ */

#pragma once
