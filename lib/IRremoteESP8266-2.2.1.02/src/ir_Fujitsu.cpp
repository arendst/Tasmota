// Copyright 2017 Jonny Graham
#include "ir_Fujitsu.h"
#include <algorithm>
#include "IRsend.h"


// Fujitsu A/C support added by Jonny Graham


// Fujitsu A/C
// Ref:
// These values are based on averages of measurements
#define FUJITSU_AC_HDR_MARK    3224U
#define FUJITSU_AC_HDR_SPACE   1574U
#define FUJITSU_AC_BIT_MARK    448U
#define FUJITSU_AC_ONE_SPACE   1182U
#define FUJITSU_AC_ZERO_SPACE  367U
#define FUJITSU_AC_TRL_MARK    448U
#define FUJITSU_AC_TRL_SPACE   8100U

#if SEND_FUJITSU_AC
// Send a Fujitsu A/C message.
//
// Args:
//   data: An array of bytes containing the IR command.
//   nbytes: Nr. of bytes of data in the array. (typically either
//           FUJITSU_AC_STATE_LENGTH or FUJITSU_AC_STATE_LENGTH_SHORT)
//   repeat: Nr. of times the message is to be repeated.
//          (Default = FUJITSU_AC_MIN_REPEAT).
//
// Status: BETA / Appears to be working.
//
void IRsend::sendFujitsuAC(unsigned char data[], uint16_t nbytes,
                           uint16_t repeat) {
  // Set IR carrier frequency
  enableIROut(38);
  for (uint16_t r = 0; r <= repeat; ++r) {
    // Header
    mark(FUJITSU_AC_HDR_MARK);
    space(FUJITSU_AC_HDR_SPACE);
    // Data
    for (uint16_t i = 0; i < nbytes; i++)
      sendData(FUJITSU_AC_BIT_MARK, FUJITSU_AC_ONE_SPACE,
               FUJITSU_AC_BIT_MARK, FUJITSU_AC_ZERO_SPACE,
               data[i], 8, false);
    // Footer
    mark(FUJITSU_AC_TRL_MARK);
    space(FUJITSU_AC_TRL_SPACE);
  }
}

// Code to emulate Fujitsu A/C IR remote control unit.

// Warning: Consider this very alpha code. Seems to work, but not validated.
//
// Equipment it seems compatible with:
//  * Fujitsu ASYG30LFCA with remote AR-RAH2E
//  * <Add models (A/C & remotes) you've gotten it working with here>
// Initialise the object.
IRFujitsuAC::IRFujitsuAC(uint16_t pin) : _irsend(pin) {
  stateReset();
}

// Reset the state of the remote to a known good state/sequence.
void IRFujitsuAC::stateReset() {
  _temp = 24;
  _fanSpeed = FUJITSU_AC_FAN_HIGH;
  _mode = FUJITSU_AC_MODE_COOL;
  _swingMode = FUJITSU_AC_SWING_BOTH;
  _cmd = FUJITSU_AC_CMD_TURN_ON;
}

// Configure the pin for output.
void IRFujitsuAC::begin() {
    _irsend.begin();
}

// Send the current desired state to the IR LED.
void IRFujitsuAC::send() {
  getRaw();
  uint8_t len = getCommandLength();
  _irsend.sendFujitsuAC(remote_state, len);
}

uint8_t IRFujitsuAC::getCommandLength() {
  if (remote_state[5] != 0xFE)
    return FUJITSU_AC_STATE_LENGTH_SHORT;
  else
    return FUJITSU_AC_STATE_LENGTH;
}

// Return a pointer to the internal state date of the remote.
uint8_t* IRFujitsuAC::getRaw() {
  remote_state[0] = 0x14;
  remote_state[1] = 0x63;
  remote_state[2] = 0x00;
  remote_state[3] = 0x10;
  remote_state[4] = 0x10;
  bool fullCmd = false;
  switch (_cmd) {
    case FUJITSU_AC_CMD_TURN_OFF:
      remote_state[5] = 0x02;
      break;
    case FUJITSU_AC_CMD_STEP_HORIZ:
      remote_state[5] = 0x79;
      break;
    case FUJITSU_AC_CMD_STEP_VERT:
      remote_state[5] = 0x6C;
      break;
    default:
      remote_state[5] = 0xFE;
      fullCmd = true;
      break;
  }
  if (fullCmd) {
    remote_state[6] = 0x09;
    remote_state[7] = 0x30;
    uint8_t tempByte = _temp - FUJITSU_AC_MIN_TEMP;
    remote_state[8] = (_cmd == FUJITSU_AC_CMD_TURN_ON) | (tempByte << 4);
    remote_state[9] = _mode | 0 << 4;  // timer off
    remote_state[10] = _fanSpeed | _swingMode << 4;
    remote_state[11] = 0;  // timerOff values
    remote_state[12] = 0;  // timerOff/on values
    remote_state[13] = 0;  // timerOn values
    remote_state[14] = 0x20;
    // Checksum is the sum of the 8th to 16th bytes (ie remote_state[7]
    // thru remote_state[15]).
    // The checksum itself is stored in the 16th byte (ie remote_state[15]).
    // So we sum bytes 8th-15th...
    uint8_t checksum = 0;
    for (uint8_t i = 7 ; i < 15; ++i) {
      checksum += remote_state[i];
    }
    // and then do 0 - sum and store it in 16th.
    remote_state[15] = 0 - checksum;
  } else {
    // For the short codes, byte 7 is the inverse of byte 6
    remote_state[6] = ~remote_state[5];
    for (uint8_t i = 7; i < FUJITSU_AC_STATE_LENGTH; ++i) {
      remote_state[i] = 0;
    }
  }
  return remote_state;
}

// Set the requested power state of the A/C to off.
void IRFujitsuAC::off() {
  _cmd = FUJITSU_AC_CMD_TURN_OFF;
}

void IRFujitsuAC::stepHoriz() {
  _cmd = FUJITSU_AC_CMD_STEP_HORIZ;
}

void IRFujitsuAC::stepVert() {
  _cmd = FUJITSU_AC_CMD_STEP_VERT;
}

// Set the requested command of the A/C.
void IRFujitsuAC::setCmd(uint8_t cmd) {
  switch (cmd) {
    case FUJITSU_AC_CMD_TURN_OFF:
    case FUJITSU_AC_CMD_TURN_ON:
    case FUJITSU_AC_CMD_STAY_ON:
    case FUJITSU_AC_CMD_STEP_HORIZ:
    case FUJITSU_AC_CMD_STEP_VERT:
      break;
    default:
      cmd = FUJITSU_AC_CMD_STAY_ON;
      break;
  }
  _cmd = cmd;
}

uint8_t IRFujitsuAC::getCmd() {
  return _cmd;
}

// Set the temp. in deg C
void IRFujitsuAC::setTemp(uint8_t temp) {
  temp = std::max((uint8_t) FUJITSU_AC_MIN_TEMP, temp);
  temp = std::min((uint8_t) FUJITSU_AC_MAX_TEMP, temp);
  _temp = temp;
}

uint8_t IRFujitsuAC::getTemp() {
  return _temp;
}

// Set the speed of the fan
void IRFujitsuAC::setFanSpeed(uint8_t fanSpeed) {
  if (fanSpeed > FUJITSU_AC_FAN_QUIET)
    fanSpeed = FUJITSU_AC_FAN_HIGH;  // Set the fan to maximum if out of range.
  _fanSpeed = fanSpeed;
}
uint8_t IRFujitsuAC::getFanSpeed() {
  return _fanSpeed;
}

// Set the requested climate operation mode of the a/c unit.
void IRFujitsuAC::setMode(uint8_t mode) {
  if (mode > FUJITSU_AC_MODE_HEAT)
    mode = FUJITSU_AC_MODE_HEAT;  // Set the mode to maximum if out of range.
  _mode = mode;
}

uint8_t IRFujitsuAC::getMode() {
  return _mode;
}
// Set the requested swing operation mode of the a/c unit.
void IRFujitsuAC::setSwing(uint8_t swingMode) {
  if (swingMode > FUJITSU_AC_SWING_BOTH)
    swingMode = FUJITSU_AC_SWING_BOTH;  // Set the mode to max if out of range
  _swingMode = swingMode;
}

uint8_t IRFujitsuAC::getSwing() {
  return _swingMode;
}

#endif
