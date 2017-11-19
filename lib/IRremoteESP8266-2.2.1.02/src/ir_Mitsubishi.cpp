// Copyright 2009 Ken Shirriff
// Copyright 2017 David Conran

#include "ir_Mitsubishi.h"
#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtimer.h"
#include "IRutils.h"

//    MMMMM  IIIII TTTTT   SSSS  U   U  BBBB   IIIII   SSSS  H   H  IIIII
//    M M M    I     T    S      U   U  B   B    I    S      H   H    I
//    M M M    I     T     SSS   U   U  BBBB     I     SSS   HHHHH    I
//    M   M    I     T        S  U   U  B   B    I        S  H   H    I
//    M   M  IIIII   T    SSSS    UUU   BBBBB  IIIII  SSSS   H   H  IIIII

// Mitsubishi (TV) decoding added from https://github.com/z3t0/Arduino-IRremote
// Mitsubishi (TV) sending & Mitsubishi A/C support added by David Conran

// Constants
// Mitsubishi TV
// period time is 1/33000Hz = 30.303 uSeconds (T)
// Ref:
//   GlobalCache's Control Tower's Mitsubishi TV data.
//   https://github.com/marcosamarinho/IRremoteESP8266/blob/master/ir_Mitsubishi.cpp
#define MITSUBISHI_TICK                       30U
#define MITSUBISHI_BIT_MARK_TICKS             10U
#define MITSUBISHI_BIT_MARK           (MITSUBISHI_BIT_MARK_TICKS * \
                                       MITSUBISHI_TICK)
#define MITSUBISHI_ONE_SPACE_TICKS            70U
#define MITSUBISHI_ONE_SPACE          (MITSUBISHI_ONE_SPACE_TICKS * \
                                       MITSUBISHI_TICK)
#define MITSUBISHI_ZERO_SPACE_TICKS           30U
#define MITSUBISHI_ZERO_SPACE         (MITSUBISHI_ZERO_SPACE_TICKS * \
                                       MITSUBISHI_TICK)
#define MITSUBISHI_MIN_COMMAND_LENGTH_TICKS 1786U
#define MITSUBISHI_MIN_COMMAND_LENGTH (MITSUBISHI_MIN_COMMAND_LENGTH_TICKS * \
                                       MITSUBISHI_TICK)
#define MITSUBISHI_MIN_GAP_TICKS             936U
#define MITSUBISHI_MIN_GAP            (MITSUBISHI_MIN_GAP_TICKS * \
                                       MITSUBISHI_TICK)

// Mitsubishi A/C
// Ref:
//   https://github.com/r45635/HVAC-IR-Control/blob/master/HVAC_ESP8266/HVAC_ESP8266.ino#L84
#define MITSUBISHI_AC_HDR_MARK    3400U
#define MITSUBISHI_AC_HDR_SPACE   1750U
#define MITSUBISHI_AC_BIT_MARK     450U
#define MITSUBISHI_AC_ONE_SPACE   1300U
#define MITSUBISHI_AC_ZERO_SPACE   420U
#define MITSUBISHI_AC_RPT_MARK     440U
#define MITSUBISHI_AC_RPT_SPACE  17100UL

#if SEND_MITSUBISHI
// Send a Mitsubishi message
//
// Args:
//   data:   Contents of the message to be sent.
//   nbits:  Nr. of bits of data to be sent. Typically MITSUBISHI_BITS.
//   repeat: Nr. of additional times the message is to be sent.
//
// Status: ALPHA / untested.
//
// Notes:
//   This protocol appears to have no header.
// Ref:
//   https://github.com/marcosamarinho/IRremoteESP8266/blob/master/ir_Mitsubishi.cpp
//   GlobalCache's Control Tower's Mitsubishi TV data.
void IRsend::sendMitsubishi(uint64_t data, uint16_t nbits, uint16_t repeat) {
  enableIROut(33);  // Set IR carrier frequency
  IRtimer usecTimer = IRtimer();

  for (uint16_t i = 0; i <= repeat; i++) {
    usecTimer.reset();
    // No header

    // Data
    sendData(MITSUBISHI_BIT_MARK, MITSUBISHI_ONE_SPACE,
             MITSUBISHI_BIT_MARK, MITSUBISHI_ZERO_SPACE,
             data, nbits, true);
    // Footer
    mark(MITSUBISHI_BIT_MARK);
    space(std::max(MITSUBISHI_MIN_COMMAND_LENGTH - usecTimer.elapsed(),
                   MITSUBISHI_MIN_GAP));
  }
}
#endif

#if DECODE_MITSUBISHI
// Decode the supplied Mitsubishi message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of data bits to expect.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: BETA / previously working.
//
// Notes:
//   This protocol appears to have no header.
//
// Ref:
//   GlobalCache's Control Tower's Mitsubishi TV data.
bool IRrecv::decodeMitsubishi(decode_results *results, uint16_t nbits,
                              bool strict) {
  if (results->rawlen < 2 * nbits + FOOTER - 1)
    return false;  // Shorter than shortest possibly expected.
  if (strict && nbits != MITSUBISHI_BITS)
    return false;  // Request is out of spec.

  uint16_t offset = OFFSET_START;
  uint64_t data = 0;

  // No Header
  // But try to auto-calibrate off the initial mark signal.
  if (!matchMark(results->rawbuf[offset], MITSUBISHI_BIT_MARK, 30))
    return false;
  // Calculate how long the common tick time is based on the initial mark.
  uint32_t tick = results->rawbuf[offset] * RAWTICK / MITSUBISHI_BIT_MARK_TICKS;

  // Data
  match_result_t data_result = matchData(&(results->rawbuf[offset]), nbits,
                                         MITSUBISHI_BIT_MARK_TICKS * tick,
                                         MITSUBISHI_ONE_SPACE_TICKS * tick,
                                         MITSUBISHI_BIT_MARK_TICKS * tick,
                                         MITSUBISHI_ZERO_SPACE_TICKS * tick);
  if (data_result.success == false) return false;
  data = data_result.data;
  offset += data_result.used;
  uint16_t actualBits = data_result.used / 2;

  // Footer
  if (!matchMark(results->rawbuf[offset++], MITSUBISHI_BIT_MARK_TICKS * tick,
                 30)) return false;
  if (offset < results->rawlen &&
      !matchAtLeast(results->rawbuf[offset], MITSUBISHI_MIN_GAP_TICKS * tick))
    return false;

  // Compliance
  if (actualBits < nbits)
    return false;
  if (strict && actualBits != nbits)
    return false;  // Not as we expected.

  // Success
  results->decode_type = MITSUBISHI;
  results->bits = actualBits;
  results->value = data;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif

#if SEND_MITSUBISHI_AC
// Send a Mitsubishi A/C message.
//
// Args:
//   data: An array of bytes containing the IR command.
//   nbytes: Nr. of bytes of data in the array. (>=MITSUBISHI_AC_STATE_LENGTH)
//   repeat: Nr. of times the message is to be repeated.
//          (Default = MITSUBISHI_AC_MIN_REPEAT).
//
// Status: BETA / Appears to be working.
//
void IRsend::sendMitsubishiAC(unsigned char data[], uint16_t nbytes,
                              uint16_t repeat) {
  if (nbytes < MITSUBISHI_AC_STATE_LENGTH)
    return;  // Not enough bytes to send a proper message.

  // Set IR carrier frequency
  enableIROut(38);
  // Mitsubishi AC remote sends the packet twice.
  for (uint16_t r = 0; r <= repeat; r++) {
    // Header
    mark(MITSUBISHI_AC_HDR_MARK);
    space(MITSUBISHI_AC_HDR_SPACE);
    // Data
    for (uint16_t i = 0; i < nbytes; i++)
      sendData(MITSUBISHI_AC_BIT_MARK, MITSUBISHI_AC_ONE_SPACE,
               MITSUBISHI_AC_BIT_MARK, MITSUBISHI_AC_ZERO_SPACE,
               data[i], 8, false);
    // Footer
    mark(MITSUBISHI_AC_RPT_MARK);
    space(MITSUBISHI_AC_RPT_SPACE);
  }
}

// Code to emulate Mitsubishi A/C IR remote control unit.
// Inspired and derived from the work done at:
//   https://github.com/r45635/HVAC-IR-Control
//
// Warning: Consider this very alpha code. Seems to work, but not validated.
//
// Equipment it seems compatible with:
//  * <Add models (A/C & remotes) you've gotten it working with here>
// Initialise the object.
IRMitsubishiAC::IRMitsubishiAC(uint16_t pin) : _irsend(pin) {
  stateReset();
}

// Reset the state of the remote to a known good state/sequence.
void IRMitsubishiAC::stateReset() {
  // The state of the IR remote in IR code form.
  // Known good state obtained from:
  //   https://github.com/r45635/HVAC-IR-Control/blob/master/HVAC_ESP8266/HVAC_ESP8266.ino#L108
  // Note: Can't use the following because it requires -std=c++11
  // uint8_t known_good_state[MITSUBISHI_AC_STATE_LENGTH] = {
  //    0x23, 0xCB, 0x26, 0x01, 0x00, 0x20, 0x08, 0x06, 0x30, 0x45, 0x67, 0x00,
  //    0x00, 0x00, 0x00, 0x00, 0x00, 0x1F};
  remote_state[0] = 0x23;
  remote_state[1] = 0xCB;
  remote_state[2] = 0x26;
  remote_state[3] = 0x01;
  remote_state[4] = 0x00;
  remote_state[5] = 0x20;
  remote_state[6] = 0x08;
  remote_state[7] = 0x06;
  remote_state[8] = 0x30;
  remote_state[9] = 0x45;
  remote_state[10] = 0x67;
  for (uint8_t i = 11; i < MITSUBISHI_AC_STATE_LENGTH - 1; i++)
    remote_state[i] = 0;
  remote_state[MITSUBISHI_AC_STATE_LENGTH - 1] = 0x1F;
  checksum();  // Calculate the checksum
}

// Configure the pin for output.
void IRMitsubishiAC::begin() {
    _irsend.begin();
}

// Send the current desired state to the IR LED.
void IRMitsubishiAC::send() {
  checksum();   // Ensure correct checksum before sending.
  _irsend.sendMitsubishiAC(remote_state);
}

// Return a pointer to the internal state date of the remote.
uint8_t* IRMitsubishiAC::getRaw() {
  checksum();
  return remote_state;
}

// Calculate the checksum for the current internal state of the remote.
void IRMitsubishiAC::checksum() {
  uint8_t sum = 0;
  // Checksum is simple addition of all previous bytes stored
  // as a 8 bit value.
  for (uint8_t i = 0; i < 17; i++)
    sum += remote_state[i];
  remote_state[17] = sum & 0xFFU;
}

// Set the requested power state of the A/C to off.
void IRMitsubishiAC::on() {
  // state = ON;
  remote_state[5] |= MITSUBISHI_AC_POWER;
}

// Set the requested power state of the A/C to off.
void IRMitsubishiAC::off() {
  // state = OFF;
  remote_state[5] &= ~MITSUBISHI_AC_POWER;
}

// Set the requested power state of the A/C.
void IRMitsubishiAC::setPower(bool state) {
  if (state)
    on();
  else
    off();
}

// Return the requested power state of the A/C.
bool IRMitsubishiAC::getPower() {
  return((remote_state[5] & MITSUBISHI_AC_POWER) != 0);
}

// Set the temp. in deg C
void IRMitsubishiAC::setTemp(uint8_t temp) {
  temp = std::max((uint8_t) MITSUBISHI_AC_MIN_TEMP, temp);
  temp = std::min((uint8_t) MITSUBISHI_AC_MAX_TEMP, temp);
  remote_state[7] = temp - MITSUBISHI_AC_MIN_TEMP;
}

// Return the set temp. in deg C
uint8_t IRMitsubishiAC::getTemp() {
  return(remote_state[7] + MITSUBISHI_AC_MIN_TEMP);
}

// Set the speed of the fan, 0-6.
// 0 is auto, 1-5 is the speed, 6 is silent.
void IRMitsubishiAC::setFan(uint8_t fan) {
  // Bounds check
  if (fan > MITSUBISHI_AC_FAN_SILENT)
    fan = MITSUBISHI_AC_FAN_MAX;  // Set the fan to maximum if out of range.
  if (fan == MITSUBISHI_AC_FAN_AUTO) {   // Automatic is a special case.
    remote_state[9] = 0b10000000 | (remote_state[9] & 0b01111000);
    return;
  } else if (fan >= MITSUBISHI_AC_FAN_MAX) {
    fan--;  // There is no spoon^H^H^Heed 5 (max), pretend it doesn't exist.
  }
  remote_state[9] &= 0b01111000;  // Clear the previous state
  remote_state[9] |= fan;
}

// Return the requested state of the unit's fan.
uint8_t IRMitsubishiAC::getFan() {
  uint8_t fan = remote_state[9] & 0b111;
  if (fan == MITSUBISHI_AC_FAN_MAX)
    return MITSUBISHI_AC_FAN_SILENT;
  return fan;
}

// Return the requested climate operation mode of the a/c unit.
uint8_t IRMitsubishiAC::getMode() {
  return(remote_state[6]);
}

// Set the requested climate operation mode of the a/c unit.
void IRMitsubishiAC::setMode(uint8_t mode) {
  // If we get an unexpected mode, default to AUTO.
  switch (mode) {
    case MITSUBISHI_AC_AUTO: break;
    case MITSUBISHI_AC_COOL: break;
    case MITSUBISHI_AC_DRY: break;
    case MITSUBISHI_AC_HEAT: break;
    default: mode = MITSUBISHI_AC_AUTO;
  }
  remote_state[6] = mode;
}

// Set the requested vane operation mode of the a/c unit.
void IRMitsubishiAC::setVane(uint8_t mode) {
  mode = std::min(mode, (uint8_t) 0b111);  // bounds check
  mode |= 0b1000;
  mode <<= 3;
  remote_state[9] &= 0b11000111;  // Clear the previous setting.
  remote_state[9] |= mode;
}

// Return the requested vane operation mode of the a/c unit.
uint8_t IRMitsubishiAC::getVane() {
  return ((remote_state[9] & 0b00111000) >> 3);
}
#endif
