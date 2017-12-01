// Copyright 2016 David Conran
//
// Code to emulate IR Kelvinator YALIF remote control unit, which should control
// at least the following Kelvinator A/C units:
// KSV26CRC, KSV26HRC, KSV35CRC, KSV35HRC, KSV53HRC, KSV62HRC, KSV70CRC,
// KSV70HRC, KSV80HRC.
//
// Note:
// * Unsupported:
//    - All Sleep modes.
//    - All Timer modes.
//    - "I Feel" button & mode.
//    - Energy Saving mode.
//    - Low Heat mode.
//    - Fahrenheit.

#include "ir_Kelvinator.h"
#include <algorithm>

// KK  KK EEEEEEE LL     VV     VV IIIII NN   NN   AAA   TTTTTTT  OOOOO  RRRRRR
// KK KK  EE      LL     VV     VV  III  NNN  NN  AAAAA    TTT   OO   OO RR   RR
// KKKK   EEEEE   LL      VV   VV   III  NN N NN AA   AA   TTT   OO   OO RRRRRR
// KK KK  EE      LL       VV VV    III  NN  NNN AAAAAAA   TTT   OO   OO RR  RR
// KK  KK EEEEEEE LLLLLLL   VVV    IIIII NN   NN AA   AA   TTT    OOOO0  RR   RR

// Constants
#define KELVINATOR_HDR_MARK                 8990U
#define KELVINATOR_HDR_SPACE                4490U
#define KELVINATOR_BIT_MARK                  675U
#define KELVINATOR_ONE_SPACE                1560U
#define KELVINATOR_ZERO_SPACE                520U
#define KELVINATOR_GAP_SPACE               19950U
#define KELVINATOR_CMD_FOOTER                  2U

#define KELVINATOR_POWER                       8U
#define KELVINATOR_MODE_MASK                0xF8U
#define KELVINATOR_FAN_OFFSET                  4U
#define KELVINATOR_BASIC_FAN_MASK uint8_t(0xFFU ^ (3U << KELVINATOR_FAN_OFFSET))
#define KELVINATOR_FAN_MASK uint8_t(0xFFU ^ (7U << KELVINATOR_FAN_OFFSET))
#define KELVINATOR_CHECKSUM_START             10U
#define KELVINATOR_VENT_SWING_OFFSET           6U
#define KELVINATOR_VENT_SWING uint8_t(1U << KELVINATOR_VENT_SWING_OFFSET)
#define KELVINATOR_VENT_SWING_V       uint8_t(1U)
#define KELVINATOR_VENT_SWING_H  uint8_t(1U << 4)
#define KELVINATOR_SLEEP_1_AND_3 uint8_t(1U << 7)
#define KELVINATOR_QUIET_OFFSET                7U
#define KELVINATOR_QUIET uint8_t(1U << KELVINATOR_QUIET_OFFSET)
#define KELVINATOR_ION_FILTER_OFFSET           6U
#define KELVINATOR_ION_FILTER uint8_t(1U << KELVINATOR_ION_FILTER_OFFSET)
#define KELVINATOR_LIGHT_OFFSET                5U
#define KELVINATOR_LIGHT uint8_t(1U << KELVINATOR_LIGHT_OFFSET)
#define KELVINATOR_XFAN_OFFSET                 7U
#define KELVINATOR_XFAN uint8_t(1U << KELVINATOR_XFAN_OFFSET)
#define KELVINATOR_TURBO_OFFSET                4U
#define KELVINATOR_TURBO uint8_t(1U << KELVINATOR_TURBO_OFFSET)

#if SEND_KELVINATOR
// Send a Kelvinator A/C message.
//
// Args:
//   data: An array of bytes containing the IR command.
//   nbytes: Nr. of bytes of data in the array. (>=KELVINATOR_STATE_LENGTH)
//   repeat: Nr. of times the message is to be repeated. (Default = 0).
//
// Status: STABLE / Known working.
//
void IRsend::sendKelvinator(unsigned char data[], uint16_t nbytes,
                            uint16_t repeat) {
  if (nbytes < KELVINATOR_STATE_LENGTH)
    return;  // Not enough bytes to send a proper message.

  // Set IR carrier frequency
  enableIROut(38);

  for (uint16_t r = 0; r <= repeat; r++) {
    // Header #1
    mark(KELVINATOR_HDR_MARK);
    space(KELVINATOR_HDR_SPACE);
    // Data (command)
    // Send the first command data (4 bytes)
    uint8_t i;
    for (i = 0; i < 4; i++)
      sendData(KELVINATOR_BIT_MARK, KELVINATOR_ONE_SPACE, KELVINATOR_BIT_MARK,
               KELVINATOR_ZERO_SPACE, data[i], 8, false);
    // Send Footer for the command data (3 bits (0b010))
    sendData(KELVINATOR_BIT_MARK, KELVINATOR_ONE_SPACE, KELVINATOR_BIT_MARK,
             KELVINATOR_ZERO_SPACE, KELVINATOR_CMD_FOOTER, 3, false);
    // Send an interdata gap.
    mark(KELVINATOR_BIT_MARK);
    space(KELVINATOR_GAP_SPACE);
    // Data (options)
    // Send the 1st option chunk of data (4 bytes).
    for (; i < 8; i++)
      sendData(KELVINATOR_BIT_MARK, KELVINATOR_ONE_SPACE, KELVINATOR_BIT_MARK,
               KELVINATOR_ZERO_SPACE, data[i], 8, false);
    // Send a double data gap to signify we are starting a new command sequence.
    mark(KELVINATOR_BIT_MARK);
    space(KELVINATOR_GAP_SPACE * 2);
    // Header #2
    mark(KELVINATOR_HDR_MARK);
    space(KELVINATOR_HDR_SPACE);
    // Data (command)
    // Send the 2nd command data (4 bytes).
    // Basically an almost identical repeat of the earlier command data.
    for (; i < 12; i++)
      sendData(KELVINATOR_BIT_MARK, KELVINATOR_ONE_SPACE, KELVINATOR_BIT_MARK,
               KELVINATOR_ZERO_SPACE, data[i], 8, false);
    // Send Footer for the command data (3 bits (B010))
    sendData(KELVINATOR_BIT_MARK, KELVINATOR_ONE_SPACE, KELVINATOR_BIT_MARK,
             KELVINATOR_ZERO_SPACE, KELVINATOR_CMD_FOOTER, 3, false);
    // Send an interdata gap.
    mark(KELVINATOR_BIT_MARK);
    space(KELVINATOR_GAP_SPACE);
    // Data (options)
    // Send the 2nd option chunk of data (4 bytes).
    // Unlike the commands, definitely not a repeat of the earlier option data.
    for (; i < KELVINATOR_STATE_LENGTH; i++)
      sendData(KELVINATOR_BIT_MARK, KELVINATOR_ONE_SPACE, KELVINATOR_BIT_MARK,
               KELVINATOR_ZERO_SPACE, data[i], 8, false);
    // Footer
    mark(KELVINATOR_BIT_MARK);
    space(KELVINATOR_GAP_SPACE * 2);
  }
}

IRKelvinatorAC::IRKelvinatorAC(uint16_t pin) : _irsend(pin) {
  stateReset();
}

void IRKelvinatorAC::stateReset() {
  for (uint8_t i = 0; i < KELVINATOR_STATE_LENGTH; i++)
    remote_state[i] = 0x0;
  remote_state[3] = 0x50;
  remote_state[11] = 0x70;
}

void IRKelvinatorAC::begin() {
  _irsend.begin();
}

void IRKelvinatorAC::fixup() {
  // X-Fan mode is only valid in COOL or DRY modes.
  if (getMode() != KELVINATOR_COOL && getMode() != KELVINATOR_DRY)
    setXFan(false);
  checksum();  // Calculate the checksums
}

void IRKelvinatorAC::send() {
  fixup();   // Ensure correct settings before sending.
  _irsend.sendKelvinator(remote_state);
}

uint8_t* IRKelvinatorAC::getRaw() {
  fixup();   // Ensure correct settings before sending.
  return remote_state;
}

// Many Bothans died to bring us this information.
void IRKelvinatorAC::checksum() {
  // For each command + options block.
  for (uint8_t offset = 0; offset < KELVINATOR_STATE_LENGTH; offset += 8) {
    uint8_t sum = KELVINATOR_CHECKSUM_START;
    // Sum the lower half of the first 4 bytes of this block.
    for (uint8_t i = 0; i < 4; i++)
      sum += (remote_state[i + offset] & 0xFU);
    // then sum the upper half of the next 3 bytes.
    for (uint8_t i = 4; i < 7; i++)
      sum += (remote_state[i + offset] >> 4);
    // Trim it down to fit into the 4 bits allowed. i.e. Mod 16.
    sum &= 0xFU;
    // Place it into the IR code in the top half of the 8th & 16th byte.
    remote_state[7 + offset] = (sum << 4) | (remote_state[7 + offset] & 0xFU);
  }
}

void IRKelvinatorAC::on() {
  remote_state[0] |= KELVINATOR_POWER;
  remote_state[8] = remote_state[0];  // Duplicate to the 2nd command chunk.
}

void IRKelvinatorAC::off() {
  remote_state[0] &= ~KELVINATOR_POWER;
  remote_state[8] = remote_state[0];  // Duplicate to the 2nd command chunk.
}

void IRKelvinatorAC::setPower(bool state) {
  if (state)
    on();
  else
    off();
}

bool IRKelvinatorAC::getPower() {
  return ((remote_state[0] & KELVINATOR_POWER) != 0);
}

// Set the temp. in deg C
void IRKelvinatorAC::setTemp(uint8_t temp) {
  temp = std::max((uint8_t) KELVINATOR_MIN_TEMP, temp);
  temp = std::min((uint8_t) KELVINATOR_MAX_TEMP, temp);
  remote_state[1] = (remote_state[1] & 0xF0U) | (temp - KELVINATOR_MIN_TEMP);
  remote_state[9] = remote_state[1];  // Duplicate to the 2nd command chunk.
}

// Return the set temp. in deg C
uint8_t IRKelvinatorAC::getTemp() {
  return ((remote_state[1] & 0xFU) + KELVINATOR_MIN_TEMP);
}

// Set the speed of the fan, 0-5, 0 is auto, 1-5 is the speed
void IRKelvinatorAC::setFan(uint8_t fan) {
  fan = std::min((uint8_t) KELVINATOR_FAN_MAX, fan);  // Bounds check

  // Only change things if we need to.
  if (fan != getFan()) {
    // Set the basic fan values.
    uint8_t fan_basic = std::min((uint8_t) KELVINATOR_BASIC_FAN_MAX, fan);
    remote_state[0] = (remote_state[0] & KELVINATOR_BASIC_FAN_MASK) |
        (fan_basic << KELVINATOR_FAN_OFFSET);
    remote_state[8] = remote_state[0];  // Duplicate to the 2nd command chunk.
    // Set the advanced(?) fan value.
    remote_state[14] = (remote_state[14] & KELVINATOR_FAN_MASK) |
        (fan << KELVINATOR_FAN_OFFSET);
    setTurbo(false);  // Turbo mode is turned off if we change the fan settings.
  }
}

uint8_t IRKelvinatorAC::getFan() {
  return ((remote_state[14] & ~KELVINATOR_FAN_MASK) >> KELVINATOR_FAN_OFFSET);
}

uint8_t IRKelvinatorAC::getMode() {
  return (remote_state[0] & ~KELVINATOR_MODE_MASK);
}

void IRKelvinatorAC::setMode(uint8_t mode) {
  // If we get an unexpected mode, default to AUTO.
  if (mode > KELVINATOR_HEAT) mode = KELVINATOR_AUTO;
  remote_state[0] = (remote_state[0] & KELVINATOR_MODE_MASK) | mode;
  remote_state[8] = remote_state[0];  // Duplicate to the 2nd command chunk.
  if (mode == KELVINATOR_AUTO || KELVINATOR_DRY)
    // When the remote is set to Auto or Dry, it defaults to 25C and doesn't
    // show it.
    setTemp(KELVINATOR_AUTO_TEMP);
}

void IRKelvinatorAC::setSwingVertical(bool state) {
  if (state) {
    remote_state[0] |= KELVINATOR_VENT_SWING;
    remote_state[4] |= KELVINATOR_VENT_SWING_V;
  } else {
    remote_state[4] &= ~KELVINATOR_VENT_SWING_V;
    if (!getSwingHorizontal())
      remote_state[0] &= ~KELVINATOR_VENT_SWING;
  }
  remote_state[8] = remote_state[0];  // Duplicate to the 2nd command chunk.
}

bool IRKelvinatorAC::getSwingVertical() {
  return ((remote_state[4] & KELVINATOR_VENT_SWING_V) != 0);
}

void IRKelvinatorAC::setSwingHorizontal(bool state) {
  if (state) {
    remote_state[0] |= KELVINATOR_VENT_SWING;
    remote_state[4] |= KELVINATOR_VENT_SWING_H;
  } else {
    remote_state[4] &= ~KELVINATOR_VENT_SWING_H;
    if (!getSwingVertical())
      remote_state[0] &= ~KELVINATOR_VENT_SWING;
  }
  remote_state[8] = remote_state[0];  // Duplicate to the 2nd command chunk.
}

bool IRKelvinatorAC::getSwingHorizontal() {
  return ((remote_state[4] & KELVINATOR_VENT_SWING_H) != 0);
}

void IRKelvinatorAC::setQuiet(bool state) {
  remote_state[12] &= ~KELVINATOR_QUIET;
  remote_state[12] |= (state << KELVINATOR_QUIET_OFFSET);
}

bool IRKelvinatorAC::getQuiet() {
  return ((remote_state[12] & KELVINATOR_QUIET) != 0);
}

void IRKelvinatorAC::setIonFilter(bool state) {
  remote_state[2] &= ~KELVINATOR_ION_FILTER;
  remote_state[2] |= (state << KELVINATOR_ION_FILTER_OFFSET);
  remote_state[10] = remote_state[2];  // Duplicate to the 2nd command chunk.
}

bool IRKelvinatorAC::getIonFilter() {
  return ((remote_state[2] & KELVINATOR_ION_FILTER) != 0);
}

void IRKelvinatorAC::setLight(bool state) {
  remote_state[2] &= ~KELVINATOR_LIGHT;
  remote_state[2] |= (state << KELVINATOR_LIGHT_OFFSET);
  remote_state[10] = remote_state[2];  // Duplicate to the 2nd command chunk.
}

bool IRKelvinatorAC::getLight() {
  return ((remote_state[2] & KELVINATOR_LIGHT) != 0);
}

// Note: XFan mode is only valid in Cool or Dry mode.
void IRKelvinatorAC::setXFan(bool state) {
  remote_state[2] &= ~KELVINATOR_XFAN;
  remote_state[2] |= (state << KELVINATOR_XFAN_OFFSET);
  remote_state[10] = remote_state[2];  // Duplicate to the 2nd command chunk.
}

bool IRKelvinatorAC::getXFan() {
  return ((remote_state[2] & KELVINATOR_XFAN) != 0);
}

// Note: Turbo mode is turned off if the fan speed is changed.
void IRKelvinatorAC::setTurbo(bool state) {
  remote_state[2] &= ~KELVINATOR_TURBO;
  remote_state[2] |= (state << KELVINATOR_TURBO_OFFSET);
  remote_state[10] = remote_state[2];  // Duplicate to the 2nd command chunk.
}

bool IRKelvinatorAC::getTurbo() {
  return ((remote_state[2] & KELVINATOR_TURBO) != 0);
}
#endif
