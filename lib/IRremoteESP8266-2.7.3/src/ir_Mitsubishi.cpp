// Copyright 2009 Ken Shirriff
// Copyright 2017-2019 David Conran
// Copyright 2019 Mark Kuchel
// Copyright 2018 Denes Varga

// Mitsubishi

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

// Mitsubishi (TV) decoding added from https://github.com/z3t0/Arduino-IRremote
// Mitsubishi (TV) sending & Mitsubishi A/C support added by David Conran

// Constants
// Mitsubishi TV
// period time is 1/33000Hz = 30.303 uSeconds (T)
// Ref:
//   GlobalCache's Control Tower's Mitsubishi TV data.
//   https://github.com/marcosamarinho/IRremoteESP8266/blob/master/ir_Mitsubishi.cpp
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
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/441

const uint16_t kMitsubishi2HdrMark = 8400;
const uint16_t kMitsubishi2HdrSpace = kMitsubishi2HdrMark / 2;
const uint16_t kMitsubishi2BitMark = 560;
const uint16_t kMitsubishi2ZeroSpace = 520;
const uint16_t kMitsubishi2OneSpace = kMitsubishi2ZeroSpace * 3;
const uint16_t kMitsubishi2MinGap = 28500;

// Mitsubishi A/C
// Ref:
//   https://github.com/r45635/HVAC-IR-Control/blob/master/HVAC_ESP8266/HVAC_ESP8266.ino#L84

const uint16_t kMitsubishiAcHdrMark = 3400;
const uint16_t kMitsubishiAcHdrSpace = 1750;
const uint16_t kMitsubishiAcBitMark = 450;
const uint16_t kMitsubishiAcOneSpace = 1300;
const uint16_t kMitsubishiAcZeroSpace = 420;
const uint16_t kMitsubishiAcRptMark = 440;
const uint16_t kMitsubishiAcRptSpace = 17100;
const uint8_t  kMitsubishiAcExtraTolerance = 5;

// Mitsubishi 136 bit A/C
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/888

const uint16_t kMitsubishi136HdrMark = 3324;
const uint16_t kMitsubishi136HdrSpace = 1474;
const uint16_t kMitsubishi136BitMark = 467;
const uint16_t kMitsubishi136OneSpace = 1137;
const uint16_t kMitsubishi136ZeroSpace = 351;
const uint32_t kMitsubishi136Gap = kDefaultMessageGap;

// Mitsubishi 112 bit A/C
// Ref:
//   https://github.com/kuchel77

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
using irutils::setBit;
using irutils::setBits;

#if SEND_MITSUBISHI
// Send a Mitsubishi message
//
// Args:
//   data:   Contents of the message to be sent.
//   nbits:  Nr. of bits of data to be sent. Typically kMitsubishiBits.
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
  sendGeneric(0, 0,  // No Header
              kMitsubishiBitMark, kMitsubishiOneSpace, kMitsubishiBitMark,
              kMitsubishiZeroSpace, kMitsubishiBitMark, kMitsubishiMinGap,
              kMitsubishiMinCommandLength, data, nbits, 33, true, repeat, 50);
}
#endif  // SEND_MITSUBISHI

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
  if (strict && nbits != kMitsubishiBits)
    return false;  // Request is out of spec.

  uint16_t offset = kStartOffset;
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
// Send a Mitsubishi2 message
//
// Args:
//   data:   Contents of the message to be sent.
//   nbits:  Nr. of bits of data to be sent. Typically kMitsubishiBits.
//   repeat: Nr. of additional times the message is to be sent.
//
// Status: ALPHA / untested.
//
// Notes:
//   Based on a Mitsubishi HC3000 projector's remote.
//   This protocol appears to have a manditory in-protocol repeat.
//   That is in *addition* to the entire message needing to be sent twice
//   for the device to accept the command. That is separate from the repeat.
//   i.e. Allegedly, the real remote requires the "Off" button pressed twice.
//        You will need to add a suitable gap yourself.
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/441
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
// Decode the supplied Mitsubishi2 message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of data bits to expect.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: BETA / Works with simulated data.
//
// Notes:
//   Hardware supported:
//     * Mitsubishi HC3000 projector's remote.
//
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/441
bool IRrecv::decodeMitsubishi2(decode_results *results, uint16_t nbits,
                               bool strict) {
  if (results->rawlen < 2 * nbits + kHeader + (kFooter * 2) - 1)
    return false;  // Shorter than shortest possibly expected.
  if (strict && nbits != kMitsubishiBits)
    return false;  // Request is out of spec.

  uint16_t offset = kStartOffset;
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
// Send a Mitsubishi A/C message.
//
// Args:
//   data: An array of bytes containing the IR command.
//   nbytes: Nr. of bytes of data in the array. (>=kMitsubishiACStateLength)
//   repeat: Nr. of times the message is to be repeated.
//          (Default = kMitsubishiACMinRepeat).
//
// Status: BETA / Appears to be working.
//
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
// Decode the supplied Mitsubishi message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of data bits to expect.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: ALPHA / Under development
//
// Ref:
// https://www.analysir.com/blog/2015/01/06/reverse-engineering-mitsubishi-ac-infrared-protocol/
bool IRrecv::decodeMitsubishiAC(decode_results *results, uint16_t nbits,
                                bool strict) {
  if (results->rawlen < ((kMitsubishiACBits * 2) + 2)) {
    DPRINTLN("Shorter than shortest possibly expected.");
    return false;  // Shorter than shortest possibly expected.
  }
  if (strict && nbits != kMitsubishiACBits) {
    DPRINTLN("Request is out of spec.");
    return false;  // Request is out of spec.
  }
  uint16_t offset = kStartOffset;
  for (uint8_t i = 0; i < kMitsubishiACStateLength; i++) {
    results->state[i] = 0;
  }
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
//
// Warning: Consider this very alpha code. Seems to work, but not validated.
//
// Equipment it seems compatible with:
//  * <Add models (A/C & remotes) you've gotten it working with here>
// Initialise the object.
IRMitsubishiAC::IRMitsubishiAC(const uint16_t pin, const bool inverted,
                               const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { this->stateReset(); }

// Reset the state of the remote to a known good state/sequence.
void IRMitsubishiAC::stateReset(void) {
  // The state of the IR remote in IR code form.
  // Known good state obtained from:
  //   https://github.com/r45635/HVAC-IR-Control/blob/master/HVAC_ESP8266/HVAC_ESP8266.ino#L108
  static const uint8_t kReset[kMitsubishiACStateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x20, 0x08, 0x06, 0x30, 0x45, 0x67};
  setRaw(kReset);
}

// Configure the pin for output.
void IRMitsubishiAC::begin(void) { _irsend.begin(); }

#if SEND_MITSUBISHI_AC
// Send the current desired state to the IR LED.
void IRMitsubishiAC::send(const uint16_t repeat) {
  _irsend.sendMitsubishiAC(getRaw(), kMitsubishiACStateLength, repeat);
}
#endif  // SEND_MITSUBISHI_AC

// Return a pointer to the internal state date of the remote.
uint8_t *IRMitsubishiAC::getRaw(void) {
  this->checksum();
  return remote_state;
}

void IRMitsubishiAC::setRaw(const uint8_t *data) {
  memcpy(remote_state, data, kMitsubishiACStateLength);
}

// Calculate the checksum for the current internal state of the remote.
void IRMitsubishiAC::checksum(void) {
  remote_state[kMitsubishiACStateLength - 1] = calculateChecksum(remote_state);
}

bool IRMitsubishiAC::validChecksum(const uint8_t *data) {
  return calculateChecksum(data) == data[kMitsubishiACStateLength - 1];
}

uint8_t IRMitsubishiAC::calculateChecksum(const uint8_t *data) {
  return sumBytes(data, kMitsubishiACStateLength - 1);
}

// Set the requested power state of the A/C to on.
void IRMitsubishiAC::on(void) { setPower(true); }

// Set the requested power state of the A/C to off.
void IRMitsubishiAC::off(void) { setPower(false); }

// Set the requested power state of the A/C.
void IRMitsubishiAC::setPower(bool on) {
  setBit(&remote_state[5], kMitsubishiAcPowerOffset, on);
}

// Return the requested power state of the A/C.
bool IRMitsubishiAC::getPower(void) {
  return GETBIT8(remote_state[5], kMitsubishiAcPowerOffset);
}

// Set the temp. in deg C
void IRMitsubishiAC::setTemp(const uint8_t degrees) {
  uint8_t temp = std::max((uint8_t)kMitsubishiAcMinTemp, degrees);
  temp = std::min((uint8_t)kMitsubishiAcMaxTemp, temp);
  remote_state[7] = temp - kMitsubishiAcMinTemp;
}

// Return the set temp. in deg C
uint8_t IRMitsubishiAC::getTemp(void) {
  return (remote_state[7] + kMitsubishiAcMinTemp);
}

// Set the speed of the fan, 0-6.
// 0 is auto, 1-5 is the speed, 6 is silent.
void IRMitsubishiAC::setFan(const uint8_t speed) {
  uint8_t fan = speed;
  // Bounds check
  if (fan > kMitsubishiAcFanSilent)
    fan = kMitsubishiAcFanMax;        // Set the fan to maximum if out of range.
  // Auto has a special bit.
  setBit(&remote_state[9], kMitsubishiAcFanAutoOffset,
         fan == kMitsubishiAcFanAuto);
  if (fan >= kMitsubishiAcFanMax)
    fan--;  // There is no spoon^H^H^Heed 5 (max), pretend it doesn't exist.
  setBits(&remote_state[9], kMitsubishiAcFanOffset, kMitsubishiAcFanSize, fan);
}

// Return the requested state of the unit's fan.
uint8_t IRMitsubishiAC::getFan(void) {
  uint8_t fan = GETBITS8(remote_state[9], kMitsubishiAcFanOffset,
                         kMitsubishiAcFanSize);
  if (fan == kMitsubishiAcFanMax) return kMitsubishiAcFanSilent;
  return fan;
}

// Return the requested climate operation mode of the a/c unit.
uint8_t IRMitsubishiAC::getMode(void) {
  return GETBITS8(remote_state[6], kMitsubishiAcModeOffset, kModeBitsSize);
}

// Set the requested climate operation mode of the a/c unit.
void IRMitsubishiAC::setMode(const uint8_t mode) {
  // If we get an unexpected mode, default to AUTO.
  switch (mode) {
    case kMitsubishiAcAuto: remote_state[8] = 0b00110000; break;
    case kMitsubishiAcCool: remote_state[8] = 0b00110110; break;
    case kMitsubishiAcDry:  remote_state[8] = 0b00110010; break;
    case kMitsubishiAcHeat: remote_state[8] = 0b00110000; break;
    default:
      this->setMode(kMitsubishiAcAuto);
      return;
  }
  setBits(&remote_state[6], kMitsubishiAcModeOffset, kModeBitsSize, mode);
}

// Set the requested vane operation mode of the a/c unit.
void IRMitsubishiAC::setVane(const uint8_t position) {
  uint8_t pos = std::min(position, kMitsubishiAcVaneAutoMove);  // bounds check
  setBit(&remote_state[9], kMitsubishiAcVaneBitOffset);
  setBits(&remote_state[9], kMitsubishiAcVaneOffset, kMitsubishiAcVaneSize,
          pos);
}

// Set the requested wide-vane operation mode of the a/c unit.
void IRMitsubishiAC::setWideVane(const uint8_t position) {
  setBits(&remote_state[8], kHighNibble, kNibbleSize,
          std::min(position, kMitsubishiAcWideVaneAuto));
}

// Return the requested vane operation mode of the a/c unit.
uint8_t IRMitsubishiAC::getVane(void) {
  return GETBITS8(remote_state[9], kMitsubishiAcVaneOffset,
                  kMitsubishiAcVaneSize);
}

// Return the requested wide vane operation mode of the a/c unit.
uint8_t IRMitsubishiAC::getWideVane(void) {
  return GETBITS8(remote_state[8], kHighNibble, kNibbleSize);
}

// Return the clock setting of the message. 1=1/6 hour. e.g. 4pm = 48
uint8_t IRMitsubishiAC::getClock(void) { return remote_state[10]; }

// Set the current time. 1 = 1/6 hour. e.g. 6am = 36.
void IRMitsubishiAC::setClock(const uint8_t clock) {
  remote_state[10] = clock;
}

// Return the desired start time. 1 = 1/6 hour. e.g. 1am = 6
uint8_t IRMitsubishiAC::getStartClock(void) { return remote_state[12]; }

// Set the desired start time of the AC.  1 = 1/6 hour. e.g. 8pm = 120
void IRMitsubishiAC::setStartClock(const uint8_t clock) {
  remote_state[12] = clock;
}

// Return the desired stop time of the AC. 1 = 1/6 hour. e.g 10pm = 132
uint8_t IRMitsubishiAC::getStopClock(void) { return remote_state[11]; }

// Set the desired stop time of the AC. 1 = 1/6 hour. e.g 10pm = 132
void IRMitsubishiAC::setStopClock(const uint8_t clock) {
  remote_state[11] = clock;
}

// Return the timer setting. Possible values: kMitsubishiAcNoTimer,
//  kMitsubishiAcStartTimer, kMitsubishiAcStopTimer,
//  kMitsubishiAcStartStopTimer
uint8_t IRMitsubishiAC::getTimer(void) {
  return GETBITS8(remote_state[13], 0, 3);
}

// Set the timer setting. Possible values: kMitsubishiAcNoTimer,
//  kMitsubishiAcStartTimer, kMitsubishiAcStopTimer,
//  kMitsubishiAcStartStopTimer
void IRMitsubishiAC::setTimer(uint8_t timer) {
  setBits(&remote_state[13], 0, 3, timer);
}

// Convert a standard A/C mode into its native mode.
uint8_t IRMitsubishiAC::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kMitsubishiAcCool;
    case stdAc::opmode_t::kHeat: return kMitsubishiAcHeat;
    case stdAc::opmode_t::kDry:  return kMitsubishiAcDry;
    default:                     return kMitsubishiAcAuto;
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
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

// Convert a standard A/C vertical swing into its native setting.
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

// Convert a standard A/C wide wane swing into its native setting.
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

// Convert a native mode to it's common equivalent.
stdAc::opmode_t IRMitsubishiAC::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kMitsubishiAcCool: return stdAc::opmode_t::kCool;
    case kMitsubishiAcHeat: return stdAc::opmode_t::kHeat;
    case kMitsubishiAcDry:  return stdAc::opmode_t::kDry;
    default:                return stdAc::opmode_t::kAuto;
  }
}

// Convert a native fan speed to it's common equivalent.
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

// Convert a native vertical swing to it's common equivalent.
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

// Convert a native horizontal swing to it's common equivalent.
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

// Convert the A/C state to it's common equivalent.
stdAc::state_t IRMitsubishiAC::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::MITSUBISHI_AC;
  result.model = -1;  // No models used.
  result.power = this->getPower();
  result.mode = this->toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = this->toCommonFanSpeed(this->getFan());
  result.swingv = this->toCommonSwingV(this->getVane());
  result.swingh = this->toCommonSwingH(this->getWideVane());
  result.quiet = this->getFan() == kMitsubishiAcFanSilent;
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

// Convert the internal state into a human readable string.
String IRMitsubishiAC::toString(void) {
  String result = "";
  result.reserve(110);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(getMode(), kMitsubishiAcAuto, kMitsubishiAcCool,
                            kMitsubishiAcHeat, kMitsubishiAcDry,
                            kMitsubishiAcAuto);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kMitsubishiAcFanRealMax,
                           kMitsubishiAcFanRealMax - 3,
                           kMitsubishiAcFanAuto, kMitsubishiAcFanQuiet,
                           kMitsubishiAcFanRealMax - 2);
  result += addIntToString(this->getVane(), kSwingVStr);
  result += kSpaceLBraceStr;
  switch (this->getVane()) {
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
  result += addIntToString(this->getWideVane(), kSwingHStr);
  result += kSpaceLBraceStr;
  switch (this->getWideVane()) {
    case kMitsubishiAcWideVaneAuto: result += kAutoStr; break;
    default:                        result += kUnknownStr;
  }
  result += ')';
  result += addLabeledString(minsToString(getClock() * 10), kClockStr);
  result += addLabeledString(minsToString(getStartClock() * 10), kOnTimerStr);
  result += addLabeledString(minsToString(getStopClock() * 10), kOffTimerStr);
  result += kCommaSpaceStr;
  result += kTimerStr;
  result += kColonSpaceStr;
  switch (this->getTimer()) {
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
      result += this->getTimer();
      result += ')';
  }
  return result;
}

#if SEND_MITSUBISHI136
// Send a Mitsubishi136 A/C message.
//
// Args:
//   data: An array of bytes containing the IR command.
//   nbytes: Nr. of bytes of data in the array. (>=kMitsubishi136StateLength)
//   repeat: Nr. of times the message is to be repeated.
//          (Default = kMitsubishi136MinRepeat).
//
// Status: ALPHA / Probably working. Needs to be tested against a real device.
//
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/888
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
// Decode the supplied Mitsubishi136 message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of data bits to expect.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: STABLE / Reported as working.
//
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/888
bool IRrecv::decodeMitsubishi136(decode_results *results, const uint16_t nbits,
                                 const bool strict) {
  // Too short to match?
  if (results->rawlen < (2 * nbits) + kHeader + kFooter - 1) return false;
  if (nbits % 8 != 0) return false;  // Not a multiple of an 8 bit byte.
  if (strict) {  // Do checks to see if it matches the spec.
    if (nbits != kMitsubishi136Bits) return false;
  }
  uint16_t used = matchGeneric(results->rawbuf + kStartOffset, results->state,
                               results->rawlen - kStartOffset, nbits,
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
//
// Equipment it seems compatible with:
//   Brand: Mitsubishi Electric,  Model: PEAD-RP71JAA Ducted A/C
//   Brand: Mitsubishi Electric,  Model: 001CP T7WE10714 remote

// Initialise the object.
IRMitsubishi136::IRMitsubishi136(const uint16_t pin, const bool inverted,
                                 const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { this->stateReset(); }

// Reset the state of the remote to a known good state/sequence.
void IRMitsubishi136::stateReset(void) {
  // The state of the IR remote in IR code form.
  // Known good state obtained from:
  //   https://docs.google.com/spreadsheets/d/1f8EGfIbBUo2B-CzUFdrgKQprWakoYNKM80IKZN4KXQE/edit#gid=312397579&range=A10
  static const uint8_t kReset[kMitsubishi136StateLength] = {
      0x23, 0xCB, 0x26, 0x21, 0x00, 0x40, 0xC2, 0xC7, 0x04};
  memcpy(remote_state, kReset, kMitsubishi136StateLength);
}

// Calculate the checksum for the current internal state of the remote.
void IRMitsubishi136::checksum(void) {
  for (uint8_t i = 0; i < 6; i++)
    remote_state[kMitsubishi136PowerByte + 6 + i] =
        ~remote_state[kMitsubishi136PowerByte + i];
}

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

// Configure the pin for output.
void IRMitsubishi136::begin(void) { _irsend.begin(); }

#if SEND_MITSUBISHI136
// Send the current desired state to the IR LED.
void IRMitsubishi136::send(const uint16_t repeat) {
  _irsend.sendMitsubishi136(getRaw(), kMitsubishi136StateLength, repeat);
}
#endif  // SEND_MITSUBISHI136

// Return a pointer to the internal state date of the remote.
uint8_t *IRMitsubishi136::getRaw(void) {
  checksum();
  return remote_state;
}

void IRMitsubishi136::setRaw(const uint8_t *data) {
  memcpy(remote_state, data, kMitsubishi136StateLength);
}

// Set the requested power state of the A/C to off.
void IRMitsubishi136::on(void) { setPower(true); }

// Set the requested power state of the A/C to off.
void IRMitsubishi136::off(void) { setPower(false); }

// Set the requested power state of the A/C.
void IRMitsubishi136::setPower(bool on) {
  setBit(&remote_state[kMitsubishi136PowerByte], kMitsubishi136PowerOffset, on);
}

// Return the requested power state of the A/C.
bool IRMitsubishi136::getPower(void) {
  return GETBIT8(remote_state[kMitsubishi136PowerByte],
                 kMitsubishi136PowerOffset);
}

// Set the temp. in deg C
void IRMitsubishi136::setTemp(const uint8_t degrees) {
  uint8_t temp = std::max((uint8_t)kMitsubishi136MinTemp, degrees);
  temp = std::min((uint8_t)kMitsubishi136MaxTemp, temp);
  setBits(&remote_state[kMitsubishi136TempByte], kHighNibble, kNibbleSize,
          temp - kMitsubishiAcMinTemp);
}

// Return the set temp. in deg C
uint8_t IRMitsubishi136::getTemp(void) {
  return GETBITS8(remote_state[kMitsubishi136TempByte], kHighNibble,
                  kNibbleSize) + kMitsubishiAcMinTemp;
}

void IRMitsubishi136::setFan(const uint8_t speed) {
  setBits(&remote_state[kMitsubishi136FanByte], kMitsubishi136FanOffset,
          kMitsubishi136FanSize, std::min(speed, kMitsubishi136FanMax));
}

// Return the requested state of the unit's fan.
uint8_t IRMitsubishi136::getFan(void) {
  return GETBITS8(remote_state[kMitsubishi136FanByte], kMitsubishi136FanOffset,
                  kMitsubishi136FanSize);
}

// Return the requested climate operation mode of the a/c unit.
uint8_t IRMitsubishi136::getMode(void) {
  return GETBITS8(remote_state[kMitsubishi136ModeByte],
                  kMitsubishi136ModeOffset, kModeBitsSize);
}

// Set the requested climate operation mode of the a/c unit.
void IRMitsubishi136::setMode(const uint8_t mode) {
  // If we get an unexpected mode, default to AUTO.
  switch (mode) {
    case kMitsubishi136Fan:
    case kMitsubishi136Cool:
    case kMitsubishi136Heat:
    case kMitsubishi136Auto:
    case kMitsubishi136Dry:
      setBits(&remote_state[kMitsubishi136ModeByte], kMitsubishi136ModeOffset,
              kModeBitsSize, mode);
      break;
    default:
      setMode(kMitsubishi136Auto);
  }
}

// Set the requested vane operation mode of the a/c unit.
void IRMitsubishi136::setSwingV(const uint8_t position) {
  // If we get an unexpected mode, default to auto.
  switch (position) {
    case kMitsubishi136SwingVLowest:
    case kMitsubishi136SwingVLow:
    case kMitsubishi136SwingVHigh:
    case kMitsubishi136SwingVHighest:
    case kMitsubishi136SwingVAuto:
      setBits(&remote_state[kMitsubishi136SwingVByte], kHighNibble, kNibbleSize,
              position);
      break;
    default:
      setMode(kMitsubishi136SwingVAuto);
  }
}

// Return the requested vane operation mode of the a/c unit.
uint8_t IRMitsubishi136::getSwingV(void) {
  return GETBITS8(remote_state[kMitsubishi136SwingVByte], kHighNibble,
                  kNibbleSize);
}

// Emulate a quiet setting. There is no true quiet setting on this a/c
void IRMitsubishi136::setQuiet(bool on) {
  if (on) setFan(kMitsubishi136FanQuiet);
  else if (getQuiet()) setFan(kMitsubishi136FanLow);
}

// Return the requested power state of the A/C.
bool IRMitsubishi136::getQuiet(void) {
  return getFan() == kMitsubishi136FanQuiet;
}

// Convert a standard A/C mode into its native mode.
uint8_t IRMitsubishi136::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kMitsubishi136Cool;
    case stdAc::opmode_t::kHeat: return kMitsubishi136Heat;
    case stdAc::opmode_t::kDry:  return kMitsubishi136Dry;
    case stdAc::opmode_t::kFan:  return kMitsubishi136Fan;
    default:                     return kMitsubishi136Auto;
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRMitsubishi136::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin: return kMitsubishi136FanMin;
    case stdAc::fanspeed_t::kLow: return kMitsubishi136FanLow;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax: return kMitsubishi136FanMax;
    default:                      return kMitsubishi136FanMed;
  }
}

// Convert a standard A/C vertical swing into its native setting.
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

// Convert a native mode to it's common equivalent.
stdAc::opmode_t IRMitsubishi136::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kMitsubishi136Cool: return stdAc::opmode_t::kCool;
    case kMitsubishi136Heat: return stdAc::opmode_t::kHeat;
    case kMitsubishi136Dry:  return stdAc::opmode_t::kDry;
    case kMitsubishi136Fan:  return stdAc::opmode_t::kFan;
    default:                 return stdAc::opmode_t::kAuto;
  }
}

// Convert a native fan speed to it's common equivalent.
stdAc::fanspeed_t IRMitsubishi136::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kMitsubishi136FanMax: return stdAc::fanspeed_t::kMax;
    case kMitsubishi136FanMed: return stdAc::fanspeed_t::kMedium;
    case kMitsubishi136FanLow: return stdAc::fanspeed_t::kLow;
    case kMitsubishi136FanMin: return stdAc::fanspeed_t::kMin;
    default:                   return stdAc::fanspeed_t::kMedium;
  }
}

// Convert a native vertical swing to it's common equivalent.
stdAc::swingv_t IRMitsubishi136::toCommonSwingV(const uint8_t pos) {
  switch (pos) {
    case kMitsubishi136SwingVHighest: return stdAc::swingv_t::kHighest;
    case kMitsubishi136SwingVHigh:    return stdAc::swingv_t::kHigh;
    case kMitsubishi136SwingVLow:     return stdAc::swingv_t::kLow;
    case kMitsubishi136SwingVLowest:  return stdAc::swingv_t::kLowest;
    default:                          return stdAc::swingv_t::kAuto;
  }
}

// Convert the A/C state to it's common equivalent.
stdAc::state_t IRMitsubishi136::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::MITSUBISHI136;
  result.model = -1;  // No models used.
  result.power = this->getPower();
  result.mode = this->toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = this->toCommonFanSpeed(this->getFan());
  result.swingv = this->toCommonSwingV(this->getSwingV());
  result.quiet = this->getQuiet();
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

// Convert the internal state into a human readable string.
String IRMitsubishi136::toString(void) {
  String result = "";
  result.reserve(80);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(getMode(), kMitsubishi136Auto, kMitsubishi136Cool,
                            kMitsubishi136Heat, kMitsubishi136Dry,
                            kMitsubishi136Fan);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kMitsubishi136FanMax,
                           kMitsubishi136FanLow,  kMitsubishi136FanMax,
                           kMitsubishi136FanQuiet, kMitsubishi136FanMed);
  result += addIntToString(getSwingV(), kSwingVStr);
  result += kSpaceLBraceStr;
  switch (getSwingV()) {
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
// Send a Mitsubishi112 A/C message.
//
// Args:
//   data: An array of bytes containing the IR command.
//   nbytes: Nr. of bytes of data in the array. (>=kMitsubishi112StateLength)
//   repeat: Nr. of times the message is to be repeated.
//          (Default = kMitsubishi112MinRepeat).
//
// Status: Stable / Reported as working.
//
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/947
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
// Decode the supplied Mitsubishi112 / Tcl112Ac message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of data bits to expect.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: STABLE / Reported as working.
//
// Note: Mitsubishi112 & Tcl112Ac are basically the same protocol.
//       The only significant difference I can see is Mitsubishi112 has a
//       slightly longer header mark. We will use that to determine which
//       varient it should be. The other differences require full decoding and
//       only only with certain settings.
//       There are some other timing differences too, but the tolerances will
//       overlap.
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/619
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/947
bool IRrecv::decodeMitsubishi112(decode_results *results, const uint16_t nbits,
                                 const bool strict) {
  if (results->rawlen < ((2 * nbits) + kHeader + kFooter - 1)) return false;
  if (nbits % 8 != 0) return false;  // Not a multiple of an 8 bit byte.
  if (strict) {  // Do checks to see if it matches the spec.
    if (nbits != kMitsubishi112Bits && nbits != kTcl112AcBits) return false;
  }
  uint16_t offset = kStartOffset;
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
//
// Equipment it seems compatible with:
//   Brand: Mitsubishi Electric,  Model: MSH-A24WV / MUH-A24WV A/C
//   Brand: Mitsubishi Electric,  Model: KPOA remote

// Initialise the object.
IRMitsubishi112::IRMitsubishi112(const uint16_t pin, const bool inverted,
                                 const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { this->stateReset(); }

// Reset the state of the remote to a known good state/sequence.
void IRMitsubishi112::stateReset(void) {
  const uint8_t kReset[kMitsubishi112StateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x24, 0x03, 0x0B, 0x10,
      0x00, 0x00, 0x00, 0x30};
  setRaw(kReset);
}

// Calculate the checksum for the current internal state of the remote.
void IRMitsubishi112::checksum(void) {
  remote_state[kMitsubishi112StateLength - 1] = IRTcl112Ac::calcChecksum(
      remote_state, kMitsubishi112StateLength);
}

// Configure the pin for output.
void IRMitsubishi112::begin(void) { _irsend.begin(); }

#if SEND_MITSUBISHI112
// Send the current desired state to the IR LED.
void IRMitsubishi112::send(const uint16_t repeat) {
  _irsend.sendMitsubishi112(getRaw(), kMitsubishi112StateLength, repeat);
}
#endif  // SEND_MITSUBISHI112

// Return a pointer to the internal state date of the remote.
uint8_t *IRMitsubishi112::getRaw(void) {
  checksum();
  return remote_state;
}

void IRMitsubishi112::setRaw(const uint8_t *data) {
  memcpy(remote_state, data, kMitsubishi112StateLength);
}

// Set the requested power state of the A/C to off.
void IRMitsubishi112::on(void) { setPower(true); }

// Set the requested power state of the A/C to off.
void IRMitsubishi112::off(void) { setPower(false); }

// Set the requested power state of the A/C.
void IRMitsubishi112::setPower(bool on) {
  setBit(&remote_state[kMitsubishi112PowerByte], kMitsubishi112PowerOffset, on);
}

// Return the requested power state of the A/C.
bool IRMitsubishi112::getPower(void) {
  return GETBIT8(remote_state[kMitsubishi112PowerByte],
                 kMitsubishi112PowerOffset);
}

// Set the temp. in deg C
void IRMitsubishi112::setTemp(const uint8_t degrees) {
  uint8_t temp = std::max((uint8_t)kMitsubishi112MinTemp, degrees);
  temp = std::min((uint8_t)kMitsubishi112MaxTemp, temp);
  setBits(&remote_state[kMitsubishi112TempByte], kLowNibble,
          kMitsubishi112TempSize, kMitsubishiAcMaxTemp - temp);
}

// Return the set temp. in deg C
uint8_t IRMitsubishi112::getTemp(void) {
  return kMitsubishiAcMaxTemp - GETBITS8(remote_state[kMitsubishi112TempByte],
                                         kLowNibble, kMitsubishi112TempSize);
}

void IRMitsubishi112::setFan(const uint8_t speed) {
  switch (speed) {
    case kMitsubishi112FanMin:
    case kMitsubishi112FanLow:
    case kMitsubishi112FanMed:
    case kMitsubishi112FanMax:
      setBits(&remote_state[kMitsubishi112FanByte], kMitsubishi112FanOffset,
              kMitsubishi112FanSize, speed);
      break;
    default:
      setFan(kMitsubishi112FanMax);
  }
}

// Return the requested state of the unit's fan.
uint8_t IRMitsubishi112::getFan(void) {
  return GETBITS8(remote_state[kMitsubishi112FanByte], kMitsubishi112FanOffset,
                  kMitsubishi112FanSize);
}

// Return the requested climate operation mode of the a/c unit.
uint8_t IRMitsubishi112::getMode(void) {
  return GETBITS8(remote_state[kMitsubishi112ModeByte],
                  kMitsubishi112ModeOffset, kModeBitsSize);
}

// Set the requested climate operation mode of the a/c unit.
void IRMitsubishi112::setMode(const uint8_t mode) {
  // If we get an unexpected mode, default to AUTO.
  switch (mode) {
    // Note: No Fan Only mode.
    case kMitsubishi112Cool:
    case kMitsubishi112Heat:
    case kMitsubishi112Auto:
    case kMitsubishi112Dry:
      setBits(&remote_state[kMitsubishi112ModeByte], kMitsubishi112ModeOffset,
              kModeBitsSize, mode);
      break;
    default:
      setMode(kMitsubishi112Auto);
  }
}

// Set the requested vane operation mode of the a/c unit.
void IRMitsubishi112::setSwingV(const uint8_t position) {
  // If we get an unexpected mode, default to auto.
  switch (position) {
    case kMitsubishi112SwingVLowest:
    case kMitsubishi112SwingVLow:
    case kMitsubishi112SwingVMiddle:
    case kMitsubishi112SwingVHigh:
    case kMitsubishi112SwingVHighest:
    case kMitsubishi112SwingVAuto:
      setBits(&remote_state[kMitsubishi112SwingVByte],
              kMitsubishi112SwingVOffset, kMitsubishi112SwingVSize, position);
      break;
    default:
      setMode(kMitsubishi112SwingVAuto);
  }
}

// Return the requested vane operation mode of the a/c unit.
uint8_t IRMitsubishi112::getSwingV(void) {
  return GETBITS8(remote_state[kMitsubishi112SwingVByte],
                  kMitsubishi112SwingVOffset, kMitsubishi112SwingVSize);
}

// Set the requested vane operation mode of the a/c unit.
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
      setBits(&remote_state[kMitsubishi112SwingHByte],
              kMitsubishi112SwingHOffset, kMitsubishi112SwingHSize, position);
      break;
    default:
      setSwingH(kMitsubishi112SwingHAuto);
  }
}

// Return the requested vane operation mode of the a/c unit.
uint8_t IRMitsubishi112::getSwingH(void) {
  return GETBITS8(remote_state[kMitsubishi112SwingHByte],
                  kMitsubishi112SwingHOffset, kMitsubishi112SwingHSize);
}

// Emulate a quiet setting. There is no true quiet setting on this a/c
void IRMitsubishi112::setQuiet(bool on) {
  if (on)
    setFan(kMitsubishi112FanQuiet);
  else if (getQuiet()) setFan(kMitsubishi112FanLow);
}

// Return the requested power state of the A/C.
bool IRMitsubishi112::getQuiet(void) {
  return getFan() == kMitsubishi112FanQuiet;
}


// Convert a standard A/C mode into its native mode.
uint8_t IRMitsubishi112::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kMitsubishi112Cool;
    case stdAc::opmode_t::kHeat: return kMitsubishi112Heat;
    case stdAc::opmode_t::kDry: return kMitsubishi112Dry;
    // Note: No Fan Only mode.
    default: return kMitsubishi112Auto;
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
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

// Convert a standard A/C vertical swing into its native setting.
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

// Convert a standard A/C vertical swing into its native setting.
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

// Convert a native mode to it's common equivalent.
stdAc::opmode_t IRMitsubishi112::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kMitsubishi112Cool: return stdAc::opmode_t::kCool;
    case kMitsubishi112Heat: return stdAc::opmode_t::kHeat;
    case kMitsubishi112Dry: return stdAc::opmode_t::kDry;
    default: return stdAc::opmode_t::kAuto;
  }
}

// Convert a native fan speed to it's common equivalent.
stdAc::fanspeed_t IRMitsubishi112::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kMitsubishi112FanMax: return stdAc::fanspeed_t::kMax;
    case kMitsubishi112FanMed: return stdAc::fanspeed_t::kMedium;
    case kMitsubishi112FanLow: return stdAc::fanspeed_t::kLow;
    case kMitsubishi112FanMin: return stdAc::fanspeed_t::kMin;
    default: return stdAc::fanspeed_t::kMedium;
  }
}

// Convert a native vertical swing to it's common equivalent.
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

// Convert a native vertical swing to it's common equivalent.
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


// Convert the A/C state to it's common equivalent.
stdAc::state_t IRMitsubishi112::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::MITSUBISHI112;
  result.model = -1;  // No models used.
  result.power = this->getPower();
  result.mode = this->toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = this->toCommonFanSpeed(this->getFan());
  result.swingv = this->toCommonSwingV(this->getSwingV());
  result.swingh = this->toCommonSwingH(this->getSwingH());;
  result.quiet = this->getQuiet();
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

// Convert the internal state into a human readable string.
String IRMitsubishi112::toString(void) {
  String result = "";
  result.reserve(80);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(getMode(), kMitsubishi112Auto, kMitsubishi112Cool,
                            kMitsubishi112Heat, kMitsubishi112Dry,
                            kMitsubishi112Auto);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kMitsubishi112FanMax,
                           kMitsubishi112FanLow,  kMitsubishi112FanMax,
                           kMitsubishi112FanQuiet, kMitsubishi112FanMed);
  result += addIntToString(getSwingV(), kSwingVStr);
  result += kSpaceLBraceStr;
  switch (getSwingV()) {
    case kMitsubishi112SwingVHighest: result += kHighestStr; break;
    case kMitsubishi112SwingVHigh:    result += kHighStr; break;
    case kMitsubishi112SwingVMiddle:  result += kMiddleStr; break;
    case kMitsubishi112SwingVLow:     result += kLowStr; break;
    case kMitsubishi112SwingVLowest:  result += kLowestStr; break;
    case kMitsubishi112SwingVAuto:    result += kAutoStr; break;
    default:                          result += kUnknownStr;
  }
  result += ')';
  result += addIntToString(getSwingH(), kSwingHStr);
  result += kSpaceLBraceStr;
  switch (getSwingH()) {
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
