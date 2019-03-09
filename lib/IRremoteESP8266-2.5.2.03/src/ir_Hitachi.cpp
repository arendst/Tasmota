// Copyright 2018 David Conran
//
// Code to emulate Hitachi protocol compatible devices.
// Should be compatible with:
// * Hitachi RAS-35THA6 remote
//

#include "ir_Hitachi.h"
#include <algorithm>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "IRutils.h"

//              HH   HH IIIII TTTTTTT   AAA    CCCCC  HH   HH IIIII
//              HH   HH  III    TTT    AAAAA  CC    C HH   HH  III
//              HHHHHHH  III    TTT   AA   AA CC      HHHHHHH  III
//              HH   HH  III    TTT   AAAAAAA CC    C HH   HH  III
//              HH   HH IIIII   TTT   AA   AA  CCCCC  HH   HH IIIII

// Constants
// Ref: https://github.com/markszabo/IRremoteESP8266/issues/417
const uint16_t kHitachiAcHdrMark = 3300;
const uint16_t kHitachiAcHdrSpace = 1700;
const uint16_t kHitachiAc1HdrMark = 3400;
const uint16_t kHitachiAc1HdrSpace = 3400;
const uint16_t kHitachiAcBitMark = 400;
const uint16_t kHitachiAcOneSpace = 1250;
const uint16_t kHitachiAcZeroSpace = 500;
const uint32_t kHitachiAcMinGap = 100000;  // Completely made up value.

#if (SEND_HITACHI_AC || SEND_HITACHI_AC2)
// Send a Hitachi A/C message.
//
// Args:
//   data: An array of bytes containing the IR command.
//   nbytes: Nr. of bytes of data in the array. (>=kHitachiAcStateLength)
//   repeat: Nr. of times the message is to be repeated. (Default = 0).
//
// Status: ALPHA / Untested.
//
// Ref:
//   https://github.com/markszabo/IRremoteESP8266/issues/417
void IRsend::sendHitachiAC(unsigned char data[], uint16_t nbytes,
                           uint16_t repeat) {
  if (nbytes < kHitachiAcStateLength)
    return;  // Not enough bytes to send a proper message.
  sendGeneric(kHitachiAcHdrMark, kHitachiAcHdrSpace, kHitachiAcBitMark,
              kHitachiAcOneSpace, kHitachiAcBitMark, kHitachiAcZeroSpace,
              kHitachiAcBitMark, kHitachiAcMinGap, data, nbytes, 38, true,
              repeat, 50);
}
#endif  // (SEND_HITACHI_AC || SEND_HITACHI_AC2)

#if SEND_HITACHI_AC1
// Send a Hitachi A/C 13-byte message.
//
// For devices:
//  Hitachi A/C Series VI (Circa 2007) / Remote: LT0541-HTA
//
// Args:
//   data: An array of bytes containing the IR command.
//   nbytes: Nr. of bytes of data in the array. (>=kHitachiAc1StateLength)
//   repeat: Nr. of times the message is to be repeated. (Default = 0).
//
// Status: BETA / Appears to work.
//
// Ref:
//   https://github.com/markszabo/IRremoteESP8266/issues/453
//   Basically the same as sendHitatchiAC() except different size and header.
void IRsend::sendHitachiAC1(unsigned char data[], uint16_t nbytes,
                            uint16_t repeat) {
  if (nbytes < kHitachiAc1StateLength)
    return;  // Not enough bytes to send a proper message.
  sendGeneric(kHitachiAc1HdrMark, kHitachiAc1HdrSpace, kHitachiAcBitMark,
              kHitachiAcOneSpace, kHitachiAcBitMark, kHitachiAcZeroSpace,
              kHitachiAcBitMark, kHitachiAcMinGap, data, nbytes, 38, true,
              repeat, 50);
}
#endif  // SEND_HITACHI_AC1

#if SEND_HITACHI_AC2
// Send a Hitachi A/C 53-byte message.
//
// For devices:
//  Hitachi A/C Series VI (Circa 2007) / Remote: LT0541-HTA
//
// Args:
//   data: An array of bytes containing the IR command.
//   nbytes: Nr. of bytes of data in the array. (>=kHitachiAc2StateLength)
//   repeat: Nr. of times the message is to be repeated. (Default = 0).
//
// Status: BETA / Appears to work.
//
// Ref:
//   https://github.com/markszabo/IRremoteESP8266/issues/417
//   Basically the same as sendHitatchiAC() except different size.
void IRsend::sendHitachiAC2(unsigned char data[], uint16_t nbytes,
                            uint16_t repeat) {
  if (nbytes < kHitachiAc2StateLength)
    return;  // Not enough bytes to send a proper message.
  sendHitachiAC(data, nbytes, repeat);
}
#endif  // SEND_HITACHI_AC2

// Class for handling the remote control oh a Hitachi 28 byte A/C message.
// Inspired by:
// https://github.com/ToniA/arduino-heatpumpir/blob/master/HitachiHeatpumpIR.cpp

IRHitachiAc::IRHitachiAc(uint16_t pin) : _irsend(pin) { stateReset(); }

void IRHitachiAc::stateReset() {
  remote_state[0] = 0x80;
  remote_state[1] = 0x08;
  remote_state[2] = 0x0C;
  remote_state[3] = 0x02;
  remote_state[4] = 0xFD;
  remote_state[5] = 0x80;
  remote_state[6] = 0x7F;
  remote_state[7] = 0x88;
  remote_state[8] = 0x48;
  remote_state[9] = 0x10;
  for (uint8_t i = 10; i < kHitachiAcStateLength; i++) remote_state[i] = 0x00;
  remote_state[14] = 0x60;
  remote_state[15] = 0x60;
  remote_state[24] = 0x80;
  setTemp(23);
}

void IRHitachiAc::begin() { _irsend.begin(); }

uint8_t IRHitachiAc::calcChecksum(const uint8_t state[],
                                  const uint16_t length) {
  int8_t sum = 62;
  for (uint16_t i = 0; i < length - 1; i++) sum -= reverseBits(state[i], 8);
  return reverseBits((uint8_t)sum, 8);
}

void IRHitachiAc::checksum(const uint16_t length) {
  remote_state[length - 1] = calcChecksum(remote_state, length);
}

bool IRHitachiAc::validChecksum(const uint8_t state[], const uint16_t length) {
  if (length < 2) return true;  // Assume true for lengths that are too short.
  return (state[length - 1] == calcChecksum(state, length));
}

uint8_t *IRHitachiAc::getRaw() {
  checksum();
  return remote_state;
}

void IRHitachiAc::setRaw(const uint8_t new_code[], const uint16_t length) {
  for (uint8_t i = 0; i < length && i < kHitachiAcStateLength; i++)
    remote_state[i] = new_code[i];
}

#if SEND_HITACHI_AC
void IRHitachiAc::send() {
  checksum();
  _irsend.sendHitachiAC(remote_state);
}
#endif  // SEND_HITACHI_AC

bool IRHitachiAc::getPower() { return (remote_state[17] & 0x01); }

void IRHitachiAc::setPower(const bool on) {
  if (on)
    remote_state[17] |= 0x01;
  else
    remote_state[17] &= 0xFE;
}

void IRHitachiAc::on() { setPower(true); }

void IRHitachiAc::off() { setPower(false); }

uint8_t IRHitachiAc::getMode() { return reverseBits(remote_state[10], 8); }

void IRHitachiAc::setMode(const uint8_t mode) {
  uint8_t newmode = mode;
  switch (mode) {
    case kHitachiAcFan:
      // Fan mode sets a special temp.
      setTemp(64);
      break;
    case kHitachiAcAuto:
    case kHitachiAcHeat:
    case kHitachiAcCool:
    case kHitachiAcDry:
      break;
    default:
      newmode = kHitachiAcAuto;
  }
  remote_state[10] = reverseBits(newmode, 8);
  if (mode != kHitachiAcFan) setTemp(_previoustemp);
  setFan(getFan());  // Reset the fan speed after the mode change.
}

uint8_t IRHitachiAc::getTemp() { return reverseBits(remote_state[11], 8) >> 1; }

void IRHitachiAc::setTemp(const uint8_t celsius) {
  uint8_t temp;
  if (celsius != 64) _previoustemp = celsius;
  switch (celsius) {
    case 64:
      temp = celsius;
      break;
    default:
      temp = std::min(celsius, kHitachiAcMaxTemp);
      temp = std::max(temp, kHitachiAcMinTemp);
  }
  remote_state[11] = reverseBits(temp << 1, 8);
  if (temp == kHitachiAcMinTemp)
    remote_state[9] = 0x90;
  else
    remote_state[9] = 0x10;
}

uint8_t IRHitachiAc::getFan() { return reverseBits(remote_state[13], 8); }

void IRHitachiAc::setFan(const uint8_t speed) {
  uint8_t fanmin = kHitachiAcFanAuto;
  uint8_t fanmax = kHitachiAcFanHigh;
  switch (getMode()) {
    case kHitachiAcDry:  // Only 2 x low speeds in Dry mode.
      fanmin = kHitachiAcFanLow;
      fanmax = kHitachiAcFanLow + 1;
      break;
    case kHitachiAcFan:
      fanmin = kHitachiAcFanLow;  // No Auto in Fan mode.
      break;
  }
  uint8_t newspeed = std::max(speed, fanmin);
  newspeed = std::min(newspeed, fanmax);
  remote_state[13] = reverseBits(newspeed, 8);
}

bool IRHitachiAc::getSwingVertical() { return remote_state[14] & 0x80; }

void IRHitachiAc::setSwingVertical(const bool on) {
  if (on)
    remote_state[14] |= 0x80;
  else
    remote_state[14] &= 0x7F;
}

bool IRHitachiAc::getSwingHorizontal() { return remote_state[15] & 0x80; }

void IRHitachiAc::setSwingHorizontal(const bool on) {
  if (on)
    remote_state[15] |= 0x80;
  else
    remote_state[15] &= 0x7F;
}

// Convert the internal state into a human readable string.
#ifdef ARDUINO
String IRHitachiAc::toString() {
  String result = "";
#else
std::string IRHitachiAc::toString() {
  std::string result = "";
#endif  // ARDUINO
  result += "Power: ";
  if (getPower())
    result += "On";
  else
    result += "Off";
  result += ", Mode: " + uint64ToString(getMode());
  switch (getMode()) {
    case kHitachiAcAuto:
      result += " (AUTO)";
      break;
    case kHitachiAcCool:
      result += " (COOL)";
      break;
    case kHitachiAcHeat:
      result += " (HEAT)";
      break;
    case kHitachiAcDry:
      result += " (DRY)";
      break;
    case kHitachiAcFan:
      result += " (FAN)";
      break;
    default:
      result += " (UNKNOWN)";
  }
  result += ", Temp: " + uint64ToString(getTemp()) + "C";
  result += ", Fan: " + uint64ToString(getFan());
  switch (getFan()) {
    case kHitachiAcFanAuto:
      result += " (AUTO)";
      break;
    case kHitachiAcFanLow:
      result += " (LOW)";
      break;
    case kHitachiAcFanHigh:
      result += " (HIGH)";
      break;
    default:
      result += " (UNKNOWN)";
      break;
  }
  result += ", Swing (Vertical): ";
  if (getSwingVertical())
    result += "On";
  else
    result += "Off";
  result += ", Swing (Horizontal): ";
  if (getSwingHorizontal())
    result += "On";
  else
    result += "Off";
  return result;
}

#if (DECODE_HITACHI_AC || DECODE_HITACHI_AC1 || DECODE_HITACHI_AC2)
// Decode the supplied Hitachi A/C message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect.
//            Typically kHitachiAcBits, kHitachiAc1Bits, kHitachiAc2Bits
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: ALPHA / Untested.
//
// Supported devices:
//  Hitachi A/C Series VI (Circa 2007) / Remote: LT0541-HTA
//
// Ref:
//   https://github.com/markszabo/IRremoteESP8266/issues/417
//   https://github.com/markszabo/IRremoteESP8266/issues/453
bool IRrecv::decodeHitachiAC(decode_results *results, uint16_t nbits,
                             bool strict) {
  const uint8_t kTolerance = 30;
  if (results->rawlen < 2 * nbits + kHeader + kFooter - 1)
    return false;  // Can't possibly be a valid HitachiAC message.
  if (strict) {
    switch (nbits) {
      case kHitachiAcBits:
      case kHitachiAc1Bits:
      case kHitachiAc2Bits:
        break;  // Okay to continue.
      default:
        return false;  // Not strictly a Hitachi message.
    }
  }
  uint16_t offset = kStartOffset;
  uint16_t dataBitsSoFar = 0;
  match_result_t data_result;

  // Header
  if (nbits == kHitachiAc1Bits) {
    if (!matchMark(results->rawbuf[offset++], kHitachiAc1HdrMark, kTolerance))
      return false;
    if (!matchSpace(results->rawbuf[offset++], kHitachiAc1HdrSpace, kTolerance))
      return false;
  } else {  // Everything else.
    if (!matchMark(results->rawbuf[offset++], kHitachiAcHdrMark, kTolerance))
      return false;
    if (!matchSpace(results->rawbuf[offset++], kHitachiAcHdrSpace, kTolerance))
      return false;
  }
  // Data
  // Keep reading bytes until we either run out of message or state to fill.
  for (uint16_t i = 0; offset <= results->rawlen - 16 && i < nbits / 8;
       i++, dataBitsSoFar += 8, offset += data_result.used) {
    data_result = matchData(&(results->rawbuf[offset]), 8, kHitachiAcBitMark,
                            kHitachiAcOneSpace, kHitachiAcBitMark,
                            kHitachiAcZeroSpace, kTolerance);
    if (data_result.success == false) break;  // Fail
    results->state[i] = (uint8_t)data_result.data;
  }

  // Footer
  if (!matchMark(results->rawbuf[offset++], kHitachiAcBitMark, kTolerance))
    return false;
  if (offset <= results->rawlen &&
      !matchAtLeast(results->rawbuf[offset], kHitachiAcMinGap, kTolerance))
    return false;

  // Compliance
  if (strict) {
    // Re-check we got the correct size/length due to the way we read the data.
    switch (dataBitsSoFar / 8) {
      case kHitachiAcStateLength:
      case kHitachiAc1StateLength:
      case kHitachiAc2StateLength:
        break;  // Continue
      default:
        return false;
    }
    if (dataBitsSoFar / 8 == kHitachiAcStateLength &&
        !IRHitachiAc::validChecksum(results->state, kHitachiAcStateLength))
      return false;
  }

  // Success
  switch (dataBitsSoFar) {
    case kHitachiAc1Bits:
      results->decode_type = HITACHI_AC1;
      break;
    case kHitachiAc2Bits:
      results->decode_type = HITACHI_AC2;
      break;
    case kHitachiAcBits:
    default:
      results->decode_type = HITACHI_AC;
  }
  results->bits = dataBitsSoFar;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // (DECODE_HITACHI_AC || DECODE_HITACHI_AC1 || DECODE_HITACHI_AC2)
