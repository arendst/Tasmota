// Copyright 2009 Ken Shirriff
// Copyright 2017-2019 David Conran
// Copyright 2019 Mark Kuchel
// Copyright 2018 Denes Varga

/// @file
/// @brief Support for Mitsubishi protocols.
/// Mitsubishi (TV) decoding added from https://github.com/z3t0/Arduino-IRremote
/// Mitsubishi (TV) sending & Mitsubishi A/C support added by David Conran
/// @see GlobalCache's Control Tower's Mitsubishi TV data.
/// @see https://github.com/marcosamarinho/IRremoteESP8266/blob/master/ir_Mitsubishi.cpp
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/441
/// @see https://github.com/r45635/HVAC-IR-Control/blob/master/HVAC_ESP8266/HVAC_ESP8266.ino#L84
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/619
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/888
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/947
/// @see https://github.com/kuchel77

#include "ir_Mitsubishi.h"
#include <algorithm>
#include <cstring>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"
#include "ir_Tcl.h"

// Constants
// Mitsubishi TV
// period time is 1/33000Hz = 30.303 uSeconds (T)
const uint16_t kMitsubishiTick = 30;
const uint16_t kMitsubishiBitMarkTicks = 10;
const uint16_t kMitsubishiBitMark = kMitsubishiBitMarkTicks * kMitsubishiTick;
const uint16_t kMitsubishiOneSpaceTicks = 70;
const uint16_t kMitsubishiOneSpace = kMitsubishiOneSpaceTicks * kMitsubishiTick;
const uint16_t kMitsubishiZeroSpaceTicks = 30;
const uint16_t kMitsubishiZeroSpace =
    kMitsubishiZeroSpaceTicks * kMitsubishiTick;
const uint16_t kMitsubishiMinCommandLengthTicks = 1786;
const uint16_t kMitsubishiMinCommandLength =
    kMitsubishiMinCommandLengthTicks * kMitsubishiTick;
const uint16_t kMitsubishiMinGapTicks = 936;
const uint16_t kMitsubishiMinGap = kMitsubishiMinGapTicks * kMitsubishiTick;

// Mitsubishi Projector (HC3000)
const uint16_t kMitsubishi2HdrMark = 8400;
const uint16_t kMitsubishi2HdrSpace = kMitsubishi2HdrMark / 2;
const uint16_t kMitsubishi2BitMark = 560;
const uint16_t kMitsubishi2ZeroSpace = 520;
const uint16_t kMitsubishi2OneSpace = kMitsubishi2ZeroSpace * 3;
const uint16_t kMitsubishi2MinGap = 28500;

// Mitsubishi A/C
const uint16_t kMitsubishiAcHdrMark = 3400;
const uint16_t kMitsubishiAcHdrSpace = 1750;
const uint16_t kMitsubishiAcBitMark = 450;
const uint16_t kMitsubishiAcOneSpace = 1300;
const uint16_t kMitsubishiAcZeroSpace = 420;
const uint16_t kMitsubishiAcRptMark = 440;
const uint16_t kMitsubishiAcRptSpace = 17100;
const uint8_t  kMitsubishiAcExtraTolerance = 5;

// Mitsubishi 136 bit A/C
const uint16_t kMitsubishi136HdrMark = 3324;
const uint16_t kMitsubishi136HdrSpace = 1474;
const uint16_t kMitsubishi136BitMark = 467;
const uint16_t kMitsubishi136OneSpace = 1137;
const uint16_t kMitsubishi136ZeroSpace = 351;
const uint32_t kMitsubishi136Gap = kDefaultMessageGap;

// Mitsubishi 112 bit A/C
const uint16_t kMitsubishi112HdrMark = 3450;
const uint16_t kMitsubishi112HdrSpace = 1696;
const uint16_t kMitsubishi112BitMark = 450;
const uint16_t kMitsubishi112OneSpace = 1250;
const uint16_t kMitsubishi112ZeroSpace = 385;
const uint32_t kMitsubishi112Gap = kDefaultMessageGap;
// Total tolerance percentage to use for matching the header mark.
const uint8_t  kMitsubishi112HdrMarkTolerance = 5;


using irutils::addBoolToString;
using irutils::addFanToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;
using irutils::minsToString;

#if SEND_MITSUBISHI
/// Send the supplied Mitsubishi 16-bit message.
/// Status: STABLE / Working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @note This protocol appears to have no header.
/// @see https://github.com/marcosamarinho/IRremoteESP8266/blob/master/ir_Mitsubishi.cpp
/// @see GlobalCache's Control Tower's Mitsubishi TV data.
void IRsend::sendMitsubishi(uint64_t data, uint16_t nbits, uint16_t repeat) {
  sendGeneric(0, 0,  // No Header
              kMitsubishiBitMark, kMitsubishiOneSpace, kMitsubishiBitMark,
              kMitsubishiZeroSpace, kMitsubishiBitMark, kMitsubishiMinGap,
              kMitsubishiMinCommandLength, data, nbits, 33, true, repeat, 50);
}
#endif  // SEND_MITSUBISHI

#if DECODE_MITSUBISHI
/// Decode the supplied Mitsubishi 16-bit message.
/// Status: STABLE / Working.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
/// @note This protocol appears to have no header.
/// @see GlobalCache's Control Tower's Mitsubishi TV data.
bool IRrecv::decodeMitsubishi(decode_results *results, uint16_t offset,
                              const uint16_t nbits, const bool strict) {
  if (strict && nbits != kMitsubishiBits)
    return false;  // Request is out of spec.

  uint64_t data = 0;

  // Match Data + Footer
  if (!matchGeneric(results->rawbuf + offset, &data,
                    results->rawlen - offset, nbits,
                    0, 0,  // No header
                    kMitsubishiBitMark, kMitsubishiOneSpace,
                    kMitsubishiBitMark, kMitsubishiZeroSpace,
                    kMitsubishiBitMark, kMitsubishiMinGap,
                    true, 30)) return false;
  // Success
  results->decode_type = MITSUBISHI;
  results->bits = nbits;
  results->value = data;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif  // DECODE_MITSUBISHI

#if SEND_MITSUBISHI2
/// Send a supplied second variant Mitsubishi 16-bit message.
/// Status: BETA / Probably works.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @note Based on a Mitsubishi HC3000 projector's remote.
///   This protocol appears to have a mandatory in-protocol repeat.
///   That is in *addition* to the entire message needing to be sent twice
///   for the device to accept the command. That is separate from the repeat.
///   i.e. Allegedly, the real remote requires the "Off" button pressed twice.
///        You will need to add a suitable gap yourself.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/441
void IRsend::sendMitsubishi2(uint64_t data, uint16_t nbits, uint16_t repeat) {
  for (uint16_t i = 0; i <= repeat; i++) {
    // First half of the data.
    sendGeneric(kMitsubishi2HdrMark, kMitsubishi2HdrSpace, kMitsubishi2BitMark,
                kMitsubishi2OneSpace, kMitsubishi2BitMark,
                kMitsubishi2ZeroSpace, kMitsubishi2BitMark,
                kMitsubishi2HdrSpace, data >> (nbits / 2), nbits / 2, 33, true,
                0, 50);
    // Second half of the data.
    sendGeneric(0, 0,  // No header for the second data block
                kMitsubishi2BitMark, kMitsubishi2OneSpace, kMitsubishi2BitMark,
                kMitsubishi2ZeroSpace, kMitsubishi2BitMark, kMitsubishi2MinGap,
                data & ((1 << (nbits / 2)) - 1), nbits / 2, 33, true, 0, 50);
  }
}
#endif  // SEND_MITSUBISHI2

#if DECODE_MITSUBISHI2
/// Decode the supplied second variation of a Mitsubishi 16-bit message.
/// Status: STABLE / Working.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/441
bool IRrecv::decodeMitsubishi2(decode_results *results, uint16_t offset,
                               const uint16_t nbits, const bool strict) {
  if (results->rawlen <= 2 * nbits + kHeader + (kFooter * 2) - 1 + offset)
    return false;  // Shorter than shortest possibly expected.
  if (strict && nbits != kMitsubishiBits)
    return false;  // Request is out of spec.

  results->value = 0;

  // Header
  if (!matchMark(results->rawbuf[offset++], kMitsubishi2HdrMark)) return false;
  if (!matchSpace(results->rawbuf[offset++], kMitsubishi2HdrSpace))
    return false;
  for (uint8_t i = 0; i < 2; i++) {
    // Match Data + Footer
    uint16_t used;
    uint64_t data = 0;
    used = matchGeneric(results->rawbuf + offset, &data,
                        results->rawlen - offset, nbits / 2,
                        0, 0,  // No header
                        kMitsubishi2BitMark, kMitsubishi2OneSpace,
                        kMitsubishi2BitMark, kMitsubishi2ZeroSpace,
                        kMitsubishi2BitMark, kMitsubishi2HdrSpace,
                        i % 2);
    if (!used) return false;
    offset += used;
    results->value <<= (nbits / 2);
    results->value |= data;
  }

  // Success
  results->decode_type = MITSUBISHI2;
  results->bits = nbits;
  results->address = GETBITS64(results->value, nbits / 2, nbits / 2);
  results->command = GETBITS64(results->value, 0, nbits / 2);
  return true;
}
#endif  // DECODE_MITSUBISHI2

#if SEND_MITSUBISHI_AC
/// Send a Mitsubishi 144-bit A/C formatted message. (MITSUBISHI_AC)
/// Status: STABLE / Working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendMitsubishiAC(const unsigned char data[], const uint16_t nbytes,
                              const uint16_t repeat) {
  if (nbytes < kMitsubishiACStateLength)
    return;  // Not enough bytes to send a proper message.

  sendGeneric(kMitsubishiAcHdrMark, kMitsubishiAcHdrSpace, kMitsubishiAcBitMark,
              kMitsubishiAcOneSpace, kMitsubishiAcBitMark,
              kMitsubishiAcZeroSpace, kMitsubishiAcRptMark,
              kMitsubishiAcRptSpace, data, nbytes, 38, false, repeat, 50);
}
#endif  // SEND_MITSUBISHI_AC

#if DECODE_MITSUBISHI_AC
/// Decode the supplied Mitsubish 144-bit A/C message.
/// Status: BETA / Probably works
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @see https://www.analysir.com/blog/2015/01/06/reverse-engineering-mitsubishi-ac-infrared-protocol/
bool IRrecv::decodeMitsubishiAC(decode_results *results, uint16_t offset,
                                const uint16_t nbits,
                                const bool strict) {
  if (results->rawlen <= ((kMitsubishiACBits * 2) + 2) + offset) {
    DPRINTLN("Shorter than shortest possibly expected.");
    return false;  // Shorter than shortest possibly expected.
  }
  if (strict && nbits != kMitsubishiACBits) {
    DPRINTLN("Request is out of spec.");
    return false;  // Request is out of spec.
  }
  for (uint8_t i = 0; i < kMitsubishiACStateLength; i++) results->state[i] = 0;
  bool failure = false;
  uint8_t rep = 0;
  do {
    failure = false;
    // Header:
    //  Sometime happens that junk signals arrives before the real message
    bool headerFound = false;
    while (!headerFound &&
           offset < (results->rawlen - (kMitsubishiACBits * 2 + 2))) {
      headerFound =
          matchMark(results->rawbuf[offset], kMitsubishiAcHdrMark) &&
          matchSpace(results->rawbuf[offset + 1], kMitsubishiAcHdrSpace);
      offset += 2;
    }
    if (!headerFound) {
      DPRINTLN("Header mark not found.");
      return false;
    }
    DPRINT("Header mark found at #");
    DPRINTLN(offset - 2);
    // Decode byte-by-byte:
    match_result_t data_result;
    for (uint8_t i = 0; i < kMitsubishiACStateLength && !failure; i++) {
      results->state[i] = 0;
      data_result =
          matchData(&(results->rawbuf[offset]), 8, kMitsubishiAcBitMark,
                    kMitsubishiAcOneSpace, kMitsubishiAcBitMark,
                    kMitsubishiAcZeroSpace,
                    _tolerance + kMitsubishiAcExtraTolerance, 0, false);
      if (data_result.success == false) {
        failure = true;
        DPRINT("Byte decode failed at #");
        DPRINTLN((uint16_t)i);
      } else {
        results->state[i] = data_result.data;
        offset += data_result.used;
        DPRINT((uint16_t)results->state[i]);
        DPRINT(",");
      }
      DPRINTLN("");
    }
    // HEADER validation:
    if (failure || results->state[0] != 0x23 || results->state[1] != 0xCB ||
        results->state[2] != 0x26 || results->state[3] != 0x01 ||
        results->state[4] != 0x00) {
      DPRINTLN("Header mismatch.");
      failure = true;
    } else {
      // DATA part:

      // FOOTER checksum:
      if (!IRMitsubishiAC::validChecksum(results->state)) {
        DPRINTLN("Checksum error.");
        failure = true;
      }
    }
    if (rep != kMitsubishiACMinRepeat && failure) {
      bool repeatMarkFound = false;
      while (!repeatMarkFound &&
             offset < (results->rawlen - (kMitsubishiACBits * 2 + 4))) {
        repeatMarkFound =
            matchMark(results->rawbuf[offset], kMitsubishiAcRptMark) &&
            matchSpace(results->rawbuf[offset + 1], kMitsubishiAcRptSpace);
            offset += 2;
      }
      if (!repeatMarkFound) {
        DPRINTLN("First attempt failure and repeat mark not found.");
        return false;
      }
    }
    rep++;
    // Check if the repeat is correct if we need strict decode:
    if (strict && !failure) {
      DPRINTLN("Strict repeat check enabled.");
      // Repeat mark and space:
      if (!matchMark(results->rawbuf[offset++], kMitsubishiAcRptMark) ||
          !matchSpace(results->rawbuf[offset++], kMitsubishiAcRptSpace)) {
        DPRINTLN("Repeat mark error.");
        return false;
      }
      // Header mark and space:
      if (!matchMark(results->rawbuf[offset++], kMitsubishiAcHdrMark) ||
          !matchSpace(results->rawbuf[offset++], kMitsubishiAcHdrSpace)) {
        DPRINTLN("Repeat header error.");
        return false;
      }
      // Payload:
      for (uint8_t i = 0; i < kMitsubishiACStateLength; i++) {
        data_result =
            matchData(&(results->rawbuf[offset]), 8, kMitsubishiAcBitMark,
                      kMitsubishiAcOneSpace, kMitsubishiAcBitMark,
                      kMitsubishiAcZeroSpace,
                      _tolerance + kMitsubishiAcExtraTolerance, 0, false);
        if (data_result.success == false ||
            data_result.data != results->state[i]) {
          DPRINTLN("Repeat payload error.");
          return false;
        }
        offset += data_result.used;
      }
    }  // strict repeat check
  } while (failure && rep <= kMitsubishiACMinRepeat);
  results->decode_type = MITSUBISHI_AC;
  results->bits = nbits;
  return !failure;
}
#endif  // DECODE_MITSUBISHI_AC

// Code to emulate Mitsubishi A/C IR remote control unit.
// Inspired and derived from the work done at:
//   https://github.com/r45635/HVAC-IR-Control

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
/// @warning Consider this very alpha code. Seems to work, but not validated.
IRMitsubishiAC::IRMitsubishiAC(const uint16_t pin, const bool inverted,
                               const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Reset the state of the remote to a known good state/sequence.
void IRMitsubishiAC::stateReset(void) {
  // The state of the IR remote in IR code form.
  // Known good state obtained from:
  //   https://github.com/r45635/HVAC-IR-Control/blob/master/HVAC_ESP8266/HVAC_ESP8266.ino#L108
  static const uint8_t kReset[kMitsubishiACStateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x20, 0x08, 0x06, 0x30, 0x45, 0x67};
  setRaw(kReset);
}

/// Set up hardware to be able to send a message.
void IRMitsubishiAC::begin(void) { _irsend.begin(); }

#if SEND_MITSUBISHI_AC
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRMitsubishiAC::send(const uint16_t repeat) {
  _irsend.sendMitsubishiAC(getRaw(), kMitsubishiACStateLength, repeat);
}
#endif  // SEND_MITSUBISHI_AC

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRMitsubishiAC::getRaw(void) {
  checksum();
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] data A valid code for this protocol.
void IRMitsubishiAC::setRaw(const uint8_t *data) {
  std::memcpy(_.raw, data, kMitsubishiACStateLength);
}

/// Calculate and set the checksum values for the internal state.
void IRMitsubishiAC::checksum(void) {
  _.Sum = calculateChecksum(_.raw);
}

/// Verify the checksum is valid for a given state.
/// @param[in] data The array to verify the checksum of.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRMitsubishiAC::validChecksum(const uint8_t *data) {
  return calculateChecksum(data) == data[kMitsubishiACStateLength - 1];
}

/// Calculate the checksum for a given state.
/// @param[in] data The value to calc the checksum of.
/// @return The calculated checksum value.
uint8_t IRMitsubishiAC::calculateChecksum(const uint8_t *data) {
  return sumBytes(data, kMitsubishiACStateLength - 1);
}

/// Set the requested power state of the A/C to on.
void IRMitsubishiAC::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void IRMitsubishiAC::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiAC::setPower(bool on) {
  _.Power = on;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiAC::getPower(void) const {
  return _.Power;
}

/// Set the temperature.
/// @param[in] degrees The temperature in degrees celsius.
void IRMitsubishiAC::setTemp(const uint8_t degrees) {
  uint8_t temp = std::max(kMitsubishiAcMinTemp, degrees);
  temp = std::min(kMitsubishiAcMaxTemp, temp);
  _.Temp = temp - kMitsubishiAcMinTemp;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRMitsubishiAC::getTemp(void) const {
  return (_.Temp + kMitsubishiAcMinTemp);
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting. 0 is auto, 1-5 is speed, 6 is silent.
void IRMitsubishiAC::setFan(const uint8_t speed) {
  uint8_t fan = speed;
  // Bounds check
  if (fan > kMitsubishiAcFanSilent)
    fan = kMitsubishiAcFanMax;        // Set the fan to maximum if out of range.
  // Auto has a special bit.
  _.FanAuto = (fan == kMitsubishiAcFanAuto);
  if (fan >= kMitsubishiAcFanMax)
    fan--;  // There is no spoon^H^H^Heed 5 (max), pretend it doesn't exist.
  _.Fan = fan;
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t IRMitsubishiAC::getFan(void) const {
  uint8_t fan = _.Fan;
  if (fan == kMitsubishiAcFanMax) return kMitsubishiAcFanSilent;
  return fan;
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRMitsubishiAC::getMode(void) const {
  return _.Mode;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRMitsubishiAC::setMode(const uint8_t mode) {
  // If we get an unexpected mode, default to AUTO.
  switch (mode) {
    case kMitsubishiAcAuto: _.raw[8] = 0b00110000; break;
    case kMitsubishiAcCool: _.raw[8] = 0b00110110; break;
    case kMitsubishiAcDry:  _.raw[8] = 0b00110010; break;
    case kMitsubishiAcHeat: _.raw[8] = 0b00110000; break;
    default:
      _.raw[8] = 0b00110000;
      _.Mode = kMitsubishiAcAuto;
      return;
  }
  _.Mode = mode;
}

/// Set the requested vane (Vertical Swing) operation mode of the a/c unit.
/// @param[in] position The position/mode to set the vane to.
void IRMitsubishiAC::setVane(const uint8_t position) {
  uint8_t pos = std::min(position, kMitsubishiAcVaneAutoMove);  // bounds check
  _.VaneBit = 1;
  _.Vane = pos;
}

/// Set the requested wide-vane (Horizontal Swing) operation mode of the a/c.
/// @param[in] position The position/mode to set the wide vane to.
void IRMitsubishiAC::setWideVane(const uint8_t position) {
  _.WideVane = std::min(position, kMitsubishiAcWideVaneAuto);
}

/// Get the Vane (Vertical Swing) mode of the A/C.
/// @return The native position/mode setting.
uint8_t IRMitsubishiAC::getVane(void) const {
  return _.Vane;
}

/// Get the Wide Vane (Horizontal Swing) mode of the A/C.
/// @return The native position/mode setting.
uint8_t IRMitsubishiAC::getWideVane(void) const {
  return _.WideVane;
}

/// Get the clock time of the A/C unit.
/// @return Nr. of 10 minute increments past midnight.
/// @note 1 = 1/6 hour (10 minutes). e.g. 4pm = 48.
uint8_t IRMitsubishiAC::getClock(void) const { return _.Clock; }

/// Set the clock time on the A/C unit.
/// @param[in] clock Nr. of 10 minute increments past midnight.
/// @note 1 = 1/6 hour (10 minutes). e.g. 6am = 36.
void IRMitsubishiAC::setClock(const uint8_t clock) {
  _.Clock = clock;
}

/// Get the desired start time of the A/C unit.
/// @return Nr. of 10 minute increments past midnight.
/// @note 1 = 1/6 hour (10 minutes). e.g. 4pm = 48.
uint8_t IRMitsubishiAC::getStartClock(void) const { return _.StartClock; }

/// Set the desired start time of the A/C unit.
/// @param[in] clock Nr. of 10 minute increments past midnight.
/// @note 1 = 1/6 hour (10 minutes). e.g. 8pm = 120.
void IRMitsubishiAC::setStartClock(const uint8_t clock) {
  _.StartClock = clock;
}

/// Get the desired stop time of the A/C unit.
/// @return Nr. of 10 minute increments past midnight.
/// @note 1 = 1/6 hour (10 minutes). e.g. 10pm = 132.
uint8_t IRMitsubishiAC::getStopClock(void) const { return _.StopClock; }

/// Set the desired stop time of the A/C unit.
/// @param[in] clock Nr. of 10 minute increments past midnight.
/// @note 1 = 1/6 hour (10 minutes). e.g. 10pm = 132.
void IRMitsubishiAC::setStopClock(const uint8_t clock) {
  _.StopClock = clock;
}

/// Get the timers active setting of the A/C.
/// @return The current timers enabled.
/// @note Possible values: kMitsubishiAcNoTimer,
///   kMitsubishiAcStartTimer, kMitsubishiAcStopTimer,
///   kMitsubishiAcStartStopTimer
uint8_t IRMitsubishiAC::getTimer(void) const {
  return _.Timer;
}

/// Set the timers active setting of the A/C.
/// @param[in] timer The timer code indicating which ones are active.
/// @note Possible values: kMitsubishiAcNoTimer,
///   kMitsubishiAcStartTimer, kMitsubishiAcStopTimer,
///   kMitsubishiAcStartStopTimer
void IRMitsubishiAC::setTimer(const uint8_t timer) {
  _.Timer = timer;
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishiAC::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kMitsubishiAcCool;
    case stdAc::opmode_t::kHeat: return kMitsubishiAcHeat;
    case stdAc::opmode_t::kDry:  return kMitsubishiAcDry;
    default:                     return kMitsubishiAcAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishiAC::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:    return kMitsubishiAcFanSilent;
    case stdAc::fanspeed_t::kLow:    return kMitsubishiAcFanRealMax - 3;
    case stdAc::fanspeed_t::kMedium: return kMitsubishiAcFanRealMax - 2;
    case stdAc::fanspeed_t::kHigh:   return kMitsubishiAcFanRealMax - 1;
    case stdAc::fanspeed_t::kMax:    return kMitsubishiAcFanRealMax;
    default:                         return kMitsubishiAcFanAuto;
  }
}


/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishiAC::convertSwingV(const stdAc::swingv_t position) {
  switch (position) {
    case stdAc::swingv_t::kHighest: return kMitsubishiAcVaneAutoMove - 6;
    case stdAc::swingv_t::kHigh:    return kMitsubishiAcVaneAutoMove - 5;
    case stdAc::swingv_t::kMiddle:  return kMitsubishiAcVaneAutoMove - 4;
    case stdAc::swingv_t::kLow:     return kMitsubishiAcVaneAutoMove - 3;
    case stdAc::swingv_t::kLowest:  return kMitsubishiAcVaneAutoMove - 2;
    case stdAc::swingv_t::kAuto:    return kMitsubishiAcVaneAutoMove;
    default:                        return kMitsubishiAcVaneAuto;
  }
}

/// Convert a stdAc::swingh_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishiAC::convertSwingH(const stdAc::swingh_t position) {
  switch (position) {
    case stdAc::swingh_t::kLeftMax:  return kMitsubishiAcWideVaneAuto - 7;
    case stdAc::swingh_t::kLeft:     return kMitsubishiAcWideVaneAuto - 6;
    case stdAc::swingh_t::kMiddle:   return kMitsubishiAcWideVaneAuto - 5;
    case stdAc::swingh_t::kRight:    return kMitsubishiAcWideVaneAuto - 4;
    case stdAc::swingh_t::kRightMax: return kMitsubishiAcWideVaneAuto - 3;
    case stdAc::swingh_t::kWide:     return kMitsubishiAcWideVaneAuto - 2;
    case stdAc::swingh_t::kAuto:     return kMitsubishiAcWideVaneAuto;
    default:                         return kMitsubishiAcWideVaneAuto - 5;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRMitsubishiAC::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kMitsubishiAcCool: return stdAc::opmode_t::kCool;
    case kMitsubishiAcHeat: return stdAc::opmode_t::kHeat;
    case kMitsubishiAcDry:  return stdAc::opmode_t::kDry;
    default:                return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRMitsubishiAC::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kMitsubishiAcFanRealMax:     return stdAc::fanspeed_t::kMax;
    case kMitsubishiAcFanRealMax - 1: return stdAc::fanspeed_t::kHigh;
    case kMitsubishiAcFanRealMax - 2: return stdAc::fanspeed_t::kMedium;
    case kMitsubishiAcFanRealMax - 3: return stdAc::fanspeed_t::kLow;
    case kMitsubishiAcFanSilent:      return stdAc::fanspeed_t::kMin;
    default:                          return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert a native vertical swing postion to it's common equivalent.
/// @param[in] pos A native position to convert.
/// @return The common vertical swing position.
stdAc::swingv_t IRMitsubishiAC::toCommonSwingV(const uint8_t pos) {
  switch (pos) {
    case 1:  return stdAc::swingv_t::kHighest;
    case 2:  return stdAc::swingv_t::kHigh;
    case 3:  return stdAc::swingv_t::kMiddle;
    case 4:  return stdAc::swingv_t::kLow;
    case 5:  return stdAc::swingv_t::kLowest;
    default: return stdAc::swingv_t::kAuto;
  }
}

/// Convert a native horizontal swing postion to it's common equivalent.
/// @param[in] pos A native position to convert.
/// @return The common horizontal swing position.
stdAc::swingh_t IRMitsubishiAC::toCommonSwingH(const uint8_t pos) {
  switch (pos) {
    case 1:  return stdAc::swingh_t::kLeftMax;
    case 2:  return stdAc::swingh_t::kLeft;
    case 3:  return stdAc::swingh_t::kMiddle;
    case 4:  return stdAc::swingh_t::kRight;
    case 5:  return stdAc::swingh_t::kRightMax;
    case 6:  return stdAc::swingh_t::kWide;
    default: return stdAc::swingh_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRMitsubishiAC::toCommon(void) const {
  stdAc::state_t result;
  result.protocol = decode_type_t::MITSUBISHI_AC;
  result.model = -1;  // No models used.
  result.power = _.Power;
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(getFan());
  result.swingv = toCommonSwingV(_.Vane);
  result.swingh = toCommonSwingH(_.WideVane);
  result.quiet = getFan() == kMitsubishiAcFanSilent;
  // Not supported.
  result.turbo = false;
  result.clean = false;
  result.econo = false;
  result.filter = false;
  result.light = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

/// Convert the internal state into a human readable string.
/// @return A string containing the settings in human-readable form.
String IRMitsubishiAC::toString(void) const {
  String result = "";
  result.reserve(110);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(_.Power, kPowerStr, false);
  result += addModeToString(_.Mode, kMitsubishiAcAuto, kMitsubishiAcCool,
                            kMitsubishiAcHeat, kMitsubishiAcDry,
                            kMitsubishiAcAuto);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kMitsubishiAcFanRealMax,
                           kMitsubishiAcFanRealMax - 3,
                           kMitsubishiAcFanAuto, kMitsubishiAcFanQuiet,
                           kMitsubishiAcFanRealMax - 2);
  result += addIntToString(_.Vane, kSwingVStr);
  result += kSpaceLBraceStr;
  switch (_.Vane) {
    case kMitsubishiAcVaneAuto:
      result += kAutoStr;
      break;
    case kMitsubishiAcVaneAutoMove:
      result += kAutoStr;
      result += ' ';
      result += kMoveStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addIntToString(_.WideVane, kSwingHStr);
  result += kSpaceLBraceStr;
  switch (_.WideVane) {
    case kMitsubishiAcWideVaneAuto: result += kAutoStr; break;
    default:                        result += kUnknownStr;
  }
  result += ')';
  result += addLabeledString(minsToString(_.Clock * 10), kClockStr);
  result += addLabeledString(minsToString(_.StartClock * 10), kOnTimerStr);
  result += addLabeledString(minsToString(_.StopClock * 10), kOffTimerStr);
  result += kCommaSpaceStr;
  result += kTimerStr;
  result += kColonSpaceStr;
  switch (_.Timer) {
    case kMitsubishiAcNoTimer:
      result += '-';
      break;
    case kMitsubishiAcStartTimer:
      result += kStartStr;
      break;
    case kMitsubishiAcStopTimer:
      result += kStopStr;
      break;
    case kMitsubishiAcStartStopTimer:
      result += kStartStr;
      result += '+';
      result += kStopStr;
      break;
    default:
      result += F("? (");
      result += _.Timer;
      result += ')';
  }
  return result;
}

#if SEND_MITSUBISHI136
/// Send a Mitsubishi 136-bit A/C message. (MITSUBISHI136)
/// Status: BETA / Probably working. Needs to be tested against a real device.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/888
void IRsend::sendMitsubishi136(const unsigned char data[],
                               const uint16_t nbytes,
                               const uint16_t repeat) {
  if (nbytes < kMitsubishi136StateLength)
    return;  // Not enough bytes to send a proper message.

  sendGeneric(kMitsubishi136HdrMark, kMitsubishi136HdrSpace,
              kMitsubishi136BitMark, kMitsubishi136OneSpace,
              kMitsubishi136BitMark, kMitsubishi136ZeroSpace,
              kMitsubishi136BitMark, kMitsubishi136Gap,
              data, nbytes, 38, false, repeat, 50);
}
#endif  // SEND_MITSUBISHI136

#if DECODE_MITSUBISHI136
/// Decode the supplied Mitsubishi 136-bit A/C message. (MITSUBISHI136)
/// Status: STABLE / Reported as working.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/888
bool IRrecv::decodeMitsubishi136(decode_results *results, uint16_t offset,
                                 const uint16_t nbits,
                                 const bool strict) {
  if (nbits % 8 != 0) return false;  // Not a multiple of an 8 bit byte.
  if (strict) {  // Do checks to see if it matches the spec.
    if (nbits != kMitsubishi136Bits) return false;
  }
  uint16_t used = matchGeneric(results->rawbuf + offset, results->state,
                               results->rawlen - offset, nbits,
                               kMitsubishi136HdrMark, kMitsubishi136HdrSpace,
                               kMitsubishi136BitMark, kMitsubishi136OneSpace,
                               kMitsubishi136BitMark, kMitsubishi136ZeroSpace,
                               kMitsubishi136BitMark, kMitsubishi136Gap,
                               true, _tolerance, 0, false);
  if (!used) return false;
  if (strict) {
    // Header validation: Codes start with 0x23CB26
    if (results->state[0] != 0x23 || results->state[1] != 0xCB ||
        results->state[2] != 0x26) return false;
    if (!IRMitsubishi136::validChecksum(results->state, nbits / 8))
      return false;
  }
  results->decode_type = MITSUBISHI136;
  results->bits = nbits;
  return true;
}
#endif  // DECODE_MITSUBISHI136

// Code to emulate Mitsubishi 136bit A/C IR remote control unit.

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRMitsubishi136::IRMitsubishi136(const uint16_t pin, const bool inverted,
                                 const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Reset the state of the remote to a known good state/sequence.
void IRMitsubishi136::stateReset(void) {
  // The state of the IR remote in IR code form.
  // Known good state obtained from:
  //   https://docs.google.com/spreadsheets/d/1f8EGfIbBUo2B-CzUFdrgKQprWakoYNKM80IKZN4KXQE/edit#gid=312397579&range=A10
  static const uint8_t kReset[kMitsubishi136StateLength] = {
      0x23, 0xCB, 0x26, 0x21, 0x00, 0x40, 0xC2, 0xC7, 0x04};
  std::memcpy(_.raw, kReset, kMitsubishi136StateLength);
}

/// Calculate the checksum for the current internal state of the remote.
void IRMitsubishi136::checksum(void) {
  for (uint8_t i = 0; i < 6; i++)
    _.raw[kMitsubishi136PowerByte + 6 + i] =
        ~_.raw[kMitsubishi136PowerByte + i];
}

/// Verify the checksum is valid for a given state.
/// @param[in] data The array to verify the checksum of.
/// @param[in] len The length of the data array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRMitsubishi136::validChecksum(const uint8_t *data, const uint16_t len) {
  if (len < kMitsubishi136StateLength) return false;
  const uint16_t half = (len - kMitsubishi136PowerByte) / 2;
  for (uint8_t i = 0; i < half; i++) {
    // This variable is needed to avoid the warning: (known compiler issue)
    // warning: comparison of promoted ~unsigned with unsigned [-Wsign-compare]
    const uint8_t inverted = ~data[kMitsubishi136PowerByte + half + i];
    if (data[kMitsubishi136PowerByte + i] != inverted) return false;
  }
  return true;
}

/// Set up hardware to be able to send a message.
void IRMitsubishi136::begin(void) { _irsend.begin(); }

#if SEND_MITSUBISHI136
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRMitsubishi136::send(const uint16_t repeat) {
  _irsend.sendMitsubishi136(getRaw(), kMitsubishi136StateLength, repeat);
}
#endif  // SEND_MITSUBISHI136

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRMitsubishi136::getRaw(void) {
  checksum();
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] data A valid code for this protocol.
void IRMitsubishi136::setRaw(const uint8_t *data) {
  std::memcpy(_.raw, data, kMitsubishi136StateLength);
}

/// Set the requested power state of the A/C to on.
void IRMitsubishi136::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void IRMitsubishi136::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishi136::setPower(bool on) {
  _.Power = on;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishi136::getPower(void) const {
  return _.Power;
}

/// Set the temperature.
/// @param[in] degrees The temperature in degrees celsius.
void IRMitsubishi136::setTemp(const uint8_t degrees) {
  uint8_t temp = std::max((uint8_t)kMitsubishi136MinTemp, degrees);
  temp = std::min((uint8_t)kMitsubishi136MaxTemp, temp);
  _.Temp = temp - kMitsubishiAcMinTemp;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRMitsubishi136::getTemp(void) const {
  return _.Temp + kMitsubishiAcMinTemp;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRMitsubishi136::setFan(const uint8_t speed) {
  _.Fan = std::min(speed, kMitsubishi136FanMax);
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t IRMitsubishi136::getFan(void) const {
  return _.Fan;
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRMitsubishi136::getMode(void) const {
  return _.Mode;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRMitsubishi136::setMode(const uint8_t mode) {
  // If we get an unexpected mode, default to AUTO.
  switch (mode) {
    case kMitsubishi136Fan:
    case kMitsubishi136Cool:
    case kMitsubishi136Heat:
    case kMitsubishi136Auto:
    case kMitsubishi136Dry:
      _.Mode = mode;
      break;
    default:
      _.Mode = kMitsubishi136Auto;
  }
}

/// Set the Vertical Swing mode of the A/C.
/// @param[in] position The position/mode to set the swing to.
void IRMitsubishi136::setSwingV(const uint8_t position) {
  // If we get an unexpected mode, default to auto.
  switch (position) {
    case kMitsubishi136SwingVLowest:
    case kMitsubishi136SwingVLow:
    case kMitsubishi136SwingVHigh:
    case kMitsubishi136SwingVHighest:
    case kMitsubishi136SwingVAuto:
      _.SwingV = position;
      break;
    default:
      _.SwingV = kMitsubishi136SwingVAuto;
  }
}

/// Get the Vertical Swing mode of the A/C.
/// @return The native position/mode setting.
uint8_t IRMitsubishi136::getSwingV(void) const {
  return _.SwingV;
}

/// Set the Quiet mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishi136::setQuiet(bool on) {
  if (on) setFan(kMitsubishi136FanQuiet);
  else if (getQuiet()) setFan(kMitsubishi136FanLow);
}


/// Get the Quiet mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishi136::getQuiet(void) const {
  return _.Fan == kMitsubishi136FanQuiet;
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishi136::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kMitsubishi136Cool;
    case stdAc::opmode_t::kHeat: return kMitsubishi136Heat;
    case stdAc::opmode_t::kDry:  return kMitsubishi136Dry;
    case stdAc::opmode_t::kFan:  return kMitsubishi136Fan;
    default:                     return kMitsubishi136Auto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishi136::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin: return kMitsubishi136FanMin;
    case stdAc::fanspeed_t::kLow: return kMitsubishi136FanLow;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax: return kMitsubishi136FanMax;
    default:                      return kMitsubishi136FanMed;
  }
}

/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishi136::convertSwingV(const stdAc::swingv_t position) {
  switch (position) {
    case stdAc::swingv_t::kHighest: return kMitsubishi136SwingVHighest;
    case stdAc::swingv_t::kHigh:
    case stdAc::swingv_t::kMiddle:  return kMitsubishi136SwingVHigh;
    case stdAc::swingv_t::kLow:     return kMitsubishi136SwingVLow;
    case stdAc::swingv_t::kLowest:  return kMitsubishi136SwingVLowest;
    default:                        return kMitsubishi136SwingVAuto;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRMitsubishi136::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kMitsubishi136Cool: return stdAc::opmode_t::kCool;
    case kMitsubishi136Heat: return stdAc::opmode_t::kHeat;
    case kMitsubishi136Dry:  return stdAc::opmode_t::kDry;
    case kMitsubishi136Fan:  return stdAc::opmode_t::kFan;
    default:                 return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRMitsubishi136::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kMitsubishi136FanMax: return stdAc::fanspeed_t::kMax;
    case kMitsubishi136FanMed: return stdAc::fanspeed_t::kMedium;
    case kMitsubishi136FanLow: return stdAc::fanspeed_t::kLow;
    case kMitsubishi136FanMin: return stdAc::fanspeed_t::kMin;
    default:                   return stdAc::fanspeed_t::kMedium;
  }
}

/// Convert a native vertical swing postion to it's common equivalent.
/// @param[in] pos A native position to convert.
/// @return The common vertical swing position.
stdAc::swingv_t IRMitsubishi136::toCommonSwingV(const uint8_t pos) {
  switch (pos) {
    case kMitsubishi136SwingVHighest: return stdAc::swingv_t::kHighest;
    case kMitsubishi136SwingVHigh:    return stdAc::swingv_t::kHigh;
    case kMitsubishi136SwingVLow:     return stdAc::swingv_t::kLow;
    case kMitsubishi136SwingVLowest:  return stdAc::swingv_t::kLowest;
    default:                          return stdAc::swingv_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRMitsubishi136::toCommon(void) const {
  stdAc::state_t result;
  result.protocol = decode_type_t::MITSUBISHI136;
  result.model = -1;  // No models used.
  result.power = _.Power;
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.swingv = toCommonSwingV(_.SwingV);
  result.quiet = getQuiet();
  // Not supported.
  result.swingh = stdAc::swingh_t::kOff;
  result.turbo = false;
  result.clean = false;
  result.econo = false;
  result.filter = false;
  result.light = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

/// Convert the internal state into a human readable string.
/// @return A string containing the settings in human-readable form.
String IRMitsubishi136::toString(void) const {
  String result = "";
  result.reserve(80);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(_.Power, kPowerStr, false);
  result += addModeToString(_.Mode, kMitsubishi136Auto, kMitsubishi136Cool,
                            kMitsubishi136Heat, kMitsubishi136Dry,
                            kMitsubishi136Fan);
  result += addTempToString(getTemp());
  result += addFanToString(_.Fan, kMitsubishi136FanMax,
                           kMitsubishi136FanLow,  kMitsubishi136FanMax,
                           kMitsubishi136FanQuiet, kMitsubishi136FanMed);
  result += addIntToString(_.SwingV, kSwingVStr);
  result += kSpaceLBraceStr;
  switch (_.SwingV) {
    case kMitsubishi136SwingVHighest: result += kHighestStr; break;
    case kMitsubishi136SwingVHigh: result += kHighStr; break;
    case kMitsubishi136SwingVLow: result += kLowStr; break;
    case kMitsubishi136SwingVLowest: result += kLowestStr; break;
    case kMitsubishi136SwingVAuto: result += kAutoStr; break;
    default: result += kUnknownStr;
  }
  result += ')';
  result += addBoolToString(getQuiet(), kQuietStr);
  return result;
}


#if SEND_MITSUBISHI112
/// Send a Mitsubishi 112-bit A/C formatted message. (MITSUBISHI112)
/// Status: Stable / Reported as working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/947
void IRsend::sendMitsubishi112(const unsigned char data[],
                               const uint16_t nbytes,
                               const uint16_t repeat) {
  if (nbytes < kMitsubishi112StateLength)
    return;  // Not enough bytes to send a proper message.

  sendGeneric(kMitsubishi112HdrMark, kMitsubishi112HdrSpace,
              kMitsubishi112BitMark, kMitsubishi112OneSpace,
              kMitsubishi112BitMark, kMitsubishi112ZeroSpace,
              kMitsubishi112BitMark, kMitsubishi112Gap,
              data, nbytes, 38, false, repeat, 50);
}
#endif  // SEND_MITSUBISHI112

#if DECODE_MITSUBISHI112 || DECODE_TCL112AC
/// Decode the supplied Mitsubishi/TCL 112-bit A/C message.
///   (MITSUBISHI112, TCL112AC)
/// Status: STABLE / Reported as working.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @note Note Mitsubishi112 & Tcl112Ac are basically the same protocol.
///   The only significant difference I can see is Mitsubishi112 has a
///   slightly longer header mark. We will use that to determine which
///   variant it should be. The other differences require full decoding and
///   only only with certain settings.
///   There are some other timing differences too, but the tolerances will
///   overlap.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/619
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/947
bool IRrecv::decodeMitsubishi112(decode_results *results, uint16_t offset,
                                 const uint16_t nbits, const bool strict) {
  if (results->rawlen < (2 * nbits) + kHeader + kFooter - 1 + offset)
    return false;
  if (nbits % 8 != 0) return false;  // Not a multiple of an 8 bit byte.
  if (strict) {  // Do checks to see if it matches the spec.
    if (nbits != kMitsubishi112Bits && nbits != kTcl112AcBits) return false;
  }
  decode_type_t typeguess = decode_type_t::UNKNOWN;
  uint16_t hdrspace;
  uint16_t bitmark;
  uint16_t onespace;
  uint16_t zerospace;
  uint32_t gap;
  uint8_t tolerance = _tolerance;

  // Header
#if DECODE_MITSUBISHI112
  if (matchMark(results->rawbuf[offset], kMitsubishi112HdrMark,
                kMitsubishi112HdrMarkTolerance, 0)) {
    typeguess = decode_type_t::MITSUBISHI112;
    hdrspace = kMitsubishi112HdrSpace;
    bitmark = kMitsubishi112BitMark;
    onespace = kMitsubishi112OneSpace;
    zerospace = kMitsubishi112ZeroSpace;
    gap = kMitsubishi112Gap;
  }
#endif  // DECODE_MITSUBISHI112
#if DECODE_TCL112AC
  if (typeguess == decode_type_t::UNKNOWN &&  // We didn't match Mitsubishi112
      matchMark(results->rawbuf[offset], kTcl112AcHdrMark,
                kTcl112AcHdrMarkTolerance, 0)) {
    typeguess = decode_type_t::TCL112AC;
    hdrspace = kTcl112AcHdrSpace;
    bitmark = kTcl112AcBitMark;
    onespace = kTcl112AcOneSpace;
    zerospace = kTcl112AcZeroSpace;
    gap = kTcl112AcGap;
    tolerance += kTcl112AcTolerance;
  }
#endif  // DECODE_TCL112AC
  if (typeguess == decode_type_t::UNKNOWN) return false;  // No header matched.
  offset++;

  uint16_t used = matchGeneric(results->rawbuf + offset, results->state,
                               results->rawlen - offset, nbits,
                               0,  // Skip the header as we matched it earlier.
                               hdrspace, bitmark, onespace, bitmark, zerospace,
                               bitmark, gap,
                               true, tolerance, 0, false);
  if (!used) return false;
  if (strict) {
    // Header validation: Codes start with 0x23CB26
    if (results->state[0] != 0x23 || results->state[1] != 0xCB ||
        results->state[2] != 0x26) return false;
    // TCL112 and MITSUBISHI112 share the exact same checksum.
    if (!IRTcl112Ac::validChecksum(results->state, nbits / 8)) return false;
  }
  // Success
  results->decode_type = typeguess;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_MITSUBISHI112 || DECODE_TCL112AC

// Code to emulate Mitsubishi 112bit A/C IR remote control unit.

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRMitsubishi112::IRMitsubishi112(const uint16_t pin, const bool inverted,
                                 const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Reset the state of the remote to a known good state/sequence.
void IRMitsubishi112::stateReset(void) {
  const uint8_t kReset[kMitsubishi112StateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x24, 0x03, 0x0B, 0x10,
      0x00, 0x00, 0x00, 0x30};
  setRaw(kReset);
}

/// Calculate the checksum for the current internal state of the remote.
void IRMitsubishi112::checksum(void) {
  _.Sum = IRTcl112Ac::calcChecksum(_.raw, kMitsubishi112StateLength);
}

/// Set up hardware to be able to send a message.
void IRMitsubishi112::begin(void) { _irsend.begin(); }

#if SEND_MITSUBISHI112
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRMitsubishi112::send(const uint16_t repeat) {
  _irsend.sendMitsubishi112(getRaw(), kMitsubishi112StateLength, repeat);
}
#endif  // SEND_MITSUBISHI112

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRMitsubishi112::getRaw(void) {
  checksum();
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] data A valid code for this protocol.
void IRMitsubishi112::setRaw(const uint8_t *data) {
  std::memcpy(_.raw, data, kMitsubishi112StateLength);
}

/// Set the requested power state of the A/C to off.
void IRMitsubishi112::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void IRMitsubishi112::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishi112::setPower(bool on) {
  _.Power = on;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishi112::getPower(void) const {
  return _.Power;
}

/// Set the temperature.
/// @param[in] degrees The temperature in degrees celsius.
void IRMitsubishi112::setTemp(const uint8_t degrees) {
  uint8_t temp = std::max((uint8_t)kMitsubishi112MinTemp, degrees);
  temp = std::min((uint8_t)kMitsubishi112MaxTemp, temp);
  _.Temp = kMitsubishiAcMaxTemp - temp;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRMitsubishi112::getTemp(void) const {
  return kMitsubishiAcMaxTemp - _.Temp;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRMitsubishi112::setFan(const uint8_t speed) {
  switch (speed) {
    case kMitsubishi112FanMin:
    case kMitsubishi112FanLow:
    case kMitsubishi112FanMed:
    case kMitsubishi112FanMax:
      _.Fan = speed;
      break;
    default:
      _.Fan = kMitsubishi112FanMax;
  }
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t IRMitsubishi112::getFan(void) const {
  return _.Fan;
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRMitsubishi112::getMode(void) const {
  return _.Mode;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRMitsubishi112::setMode(const uint8_t mode) {
  // If we get an unexpected mode, default to AUTO.
  switch (mode) {
    // Note: No Fan Only mode.
    case kMitsubishi112Cool:
    case kMitsubishi112Heat:
    case kMitsubishi112Auto:
    case kMitsubishi112Dry:
      _.Mode = mode;
      break;
    default:
      _.Mode = kMitsubishi112Auto;
  }
}

/// Set the Vertical Swing mode of the A/C.
/// @param[in] position The position/mode to set the swing to.
void IRMitsubishi112::setSwingV(const uint8_t position) {
  // If we get an unexpected mode, default to auto.
  switch (position) {
    case kMitsubishi112SwingVLowest:
    case kMitsubishi112SwingVLow:
    case kMitsubishi112SwingVMiddle:
    case kMitsubishi112SwingVHigh:
    case kMitsubishi112SwingVHighest:
    case kMitsubishi112SwingVAuto:
      _.SwingV = position;
      break;
    default:
      _.SwingV = kMitsubishi112SwingVAuto;
  }
}

/// Get the Vertical Swing mode of the A/C.
/// @return The native position/mode setting.
uint8_t IRMitsubishi112::getSwingV(void) const {
  return _.SwingV;
}

/// Set the Horizontal Swing mode of the A/C.
/// @param[in] position The position/mode to set the swing to.
void IRMitsubishi112::setSwingH(const uint8_t position) {
  // If we get an unexpected mode, default to auto.
  switch (position) {
    case kMitsubishi112SwingHLeftMax:
    case kMitsubishi112SwingHLeft:
    case kMitsubishi112SwingHMiddle:
    case kMitsubishi112SwingHRight:
    case kMitsubishi112SwingHRightMax:
    case kMitsubishi112SwingHWide:
    case kMitsubishi112SwingHAuto:
      _.SwingH = position;
      break;
    default:
      _.SwingH = kMitsubishi112SwingHAuto;
  }
}


/// Get the Horizontal Swing mode of the A/C.
/// @return The native position/mode setting.
uint8_t IRMitsubishi112::getSwingH(void) const {
  return _.SwingH;
}

/// Set the Quiet mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @note There is no true quiet setting on this A/C.
void IRMitsubishi112::setQuiet(bool on) {
  if (on)
    setFan(kMitsubishi112FanQuiet);
  else if (getQuiet()) setFan(kMitsubishi112FanLow);
}


/// Get the Quiet mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
/// @note There is no true quiet setting on this A/C.
bool IRMitsubishi112::getQuiet(void) const {
  return _.Fan == kMitsubishi112FanQuiet;
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishi112::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kMitsubishi112Cool;
    case stdAc::opmode_t::kHeat: return kMitsubishi112Heat;
    case stdAc::opmode_t::kDry: return kMitsubishi112Dry;
    // Note: No Fan Only mode.
    default: return kMitsubishi112Auto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishi112::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin: return kMitsubishi112FanMin;
    case stdAc::fanspeed_t::kLow: return kMitsubishi112FanLow;
    case stdAc::fanspeed_t::kMedium: return kMitsubishi112FanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax: return kMitsubishi112FanMax;
    default: return kMitsubishi112FanMed;
  }
}

/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishi112::convertSwingV(const stdAc::swingv_t position) {
  switch (position) {
    case stdAc::swingv_t::kHighest: return kMitsubishi112SwingVHighest;
    case stdAc::swingv_t::kHigh: return kMitsubishi112SwingVHigh;
    case stdAc::swingv_t::kMiddle: return kMitsubishi112SwingVMiddle;
    case stdAc::swingv_t::kLow: return kMitsubishi112SwingVLow;
    case stdAc::swingv_t::kLowest: return kMitsubishi112SwingVLowest;
    default: return kMitsubishi112SwingVAuto;
  }
}

/// Convert a stdAc::swingh_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishi112::convertSwingH(const stdAc::swingh_t position) {
  switch (position) {
    case stdAc::swingh_t::kLeftMax: return kMitsubishi112SwingHLeftMax;
    case stdAc::swingh_t::kLeft: return kMitsubishi112SwingHLeft;
    case stdAc::swingh_t::kMiddle: return kMitsubishi112SwingHMiddle;
    case stdAc::swingh_t::kRight: return kMitsubishi112SwingHRight;
    case stdAc::swingh_t::kRightMax: return kMitsubishi112SwingHRightMax;
    case stdAc::swingh_t::kWide: return kMitsubishi112SwingHWide;
    case stdAc::swingh_t::kAuto: return kMitsubishi112SwingHAuto;
    default: return kMitsubishi112SwingHAuto;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRMitsubishi112::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kMitsubishi112Cool: return stdAc::opmode_t::kCool;
    case kMitsubishi112Heat: return stdAc::opmode_t::kHeat;
    case kMitsubishi112Dry: return stdAc::opmode_t::kDry;
    default: return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRMitsubishi112::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kMitsubishi112FanMax: return stdAc::fanspeed_t::kMax;
    case kMitsubishi112FanMed: return stdAc::fanspeed_t::kMedium;
    case kMitsubishi112FanLow: return stdAc::fanspeed_t::kLow;
    case kMitsubishi112FanMin: return stdAc::fanspeed_t::kMin;
    default: return stdAc::fanspeed_t::kMedium;
  }
}

/// Convert a native vertical swing postion to it's common equivalent.
/// @param[in] pos A native position to convert.
/// @return The common vertical swing position.
stdAc::swingv_t IRMitsubishi112::toCommonSwingV(const uint8_t pos) {
  switch (pos) {
    case kMitsubishi112SwingVHighest: return stdAc::swingv_t::kHighest;
    case kMitsubishi112SwingVHigh:    return stdAc::swingv_t::kHigh;
    case kMitsubishi112SwingVMiddle:  return stdAc::swingv_t::kMiddle;
    case kMitsubishi112SwingVLow:     return stdAc::swingv_t::kLow;
    case kMitsubishi112SwingVLowest:  return stdAc::swingv_t::kLowest;
    default:                          return stdAc::swingv_t::kAuto;
  }
}

/// Convert a native horizontal swing postion to it's common equivalent.
/// @param[in] pos A native position to convert.
/// @return The common horizontal swing position.
stdAc::swingh_t IRMitsubishi112::toCommonSwingH(const uint8_t pos) {
  switch (pos) {
    case kMitsubishi112SwingHLeftMax:  return stdAc::swingh_t::kLeftMax;
    case kMitsubishi112SwingHLeft:     return stdAc::swingh_t::kLeft;
    case kMitsubishi112SwingHMiddle:   return stdAc::swingh_t::kMiddle;
    case kMitsubishi112SwingHRight:    return stdAc::swingh_t::kRight;
    case kMitsubishi112SwingHRightMax: return stdAc::swingh_t::kRightMax;
    case kMitsubishi112SwingHWide:     return stdAc::swingh_t::kWide;
    default:                           return stdAc::swingh_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRMitsubishi112::toCommon(void) const {
  stdAc::state_t result;
  result.protocol = decode_type_t::MITSUBISHI112;
  result.model = -1;  // No models used.
  result.power = _.Power;
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.swingv = toCommonSwingV(_.SwingV);
  result.swingh = toCommonSwingH(_.SwingH);;
  result.quiet = getQuiet();
  // Not supported.
  result.econo = false;  // Need to figure this part from stdAc
  result.clock = -1;
  result.sleep = -1;
  result.turbo = false;
  result.clean = false;
  result.filter = false;
  result.light = false;
  result.beep = false;


  return result;
}

/// Convert the internal state into a human readable string.
/// @return A string containing the settings in human-readable form.
String IRMitsubishi112::toString(void) const {
  String result = "";
  result.reserve(80);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(_.Power, kPowerStr, false);
  result += addModeToString(_.Mode, kMitsubishi112Auto, kMitsubishi112Cool,
                            kMitsubishi112Heat, kMitsubishi112Dry,
                            kMitsubishi112Auto);
  result += addTempToString(getTemp());
  result += addFanToString(_.Fan, kMitsubishi112FanMax,
                           kMitsubishi112FanLow,  kMitsubishi112FanMax,
                           kMitsubishi112FanQuiet, kMitsubishi112FanMed);
  result += addIntToString(_.SwingV, kSwingVStr);
  result += kSpaceLBraceStr;
  switch (_.SwingV) {
    case kMitsubishi112SwingVHighest: result += kHighestStr; break;
    case kMitsubishi112SwingVHigh:    result += kHighStr; break;
    case kMitsubishi112SwingVMiddle:  result += kMiddleStr; break;
    case kMitsubishi112SwingVLow:     result += kLowStr; break;
    case kMitsubishi112SwingVLowest:  result += kLowestStr; break;
    case kMitsubishi112SwingVAuto:    result += kAutoStr; break;
    default:                          result += kUnknownStr;
  }
  result += ')';
  result += addIntToString(_.SwingH, kSwingHStr);
  result += kSpaceLBraceStr;
  switch (_.SwingH) {
    case kMitsubishi112SwingHLeftMax:  result += kLeftMaxStr; break;
    case kMitsubishi112SwingHLeft:     result += kLeftStr; break;
    case kMitsubishi112SwingHMiddle:   result += kMiddleStr; break;
    case kMitsubishi112SwingHRight:    result += kRightStr; break;
    case kMitsubishi112SwingHRightMax: result += kRightMaxStr; break;
    case kMitsubishi112SwingHWide:     result += kWideStr; break;
    case kMitsubishi112SwingHAuto:     result += kAutoStr; break;
    default:                           result += kUnknownStr;
  }
  result += ')';
  result += addBoolToString(getQuiet(), kQuietStr);
  return result;
}
