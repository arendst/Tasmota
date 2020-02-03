/*
Node MCU/ESP8266 Sketch to emulate Argo Ulisse 13 DCI remote
Controls Argo Ulisse 13 DCI A/C
Copyright 2017 Schmolders
Copyright 2019 crankyoldgit
*/

#include "ir_Argo.h"
#include <algorithm>
#include <cstring>
#ifndef UNIT_TEST
#include <Arduino.h>
#endif  // UNIT_TEST
#include "IRremoteESP8266.h"
#include "IRtext.h"
#include "IRutils.h"

// Constants
// using SPACE modulation. MARK is always const 400u
const uint16_t kArgoHdrMark = 6400;
const uint16_t kArgoHdrSpace = 3300;
const uint16_t kArgoBitMark = 400;
const uint16_t kArgoOneSpace = 2200;
const uint16_t kArgoZeroSpace = 900;
const uint32_t kArgoGap = kDefaultMessageGap;  // Made up value. Complete guess.

using irutils::addBoolToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;
using irutils::setBit;
using irutils::setBits;

#if SEND_ARGO
// Send an Argo A/C message.
//
// Args:
//   data: An array of kArgoStateLength bytes containing the IR command.
//
// Status: ALPHA / Untested.

void IRsend::sendArgo(const unsigned char data[], const uint16_t nbytes,
                      const uint16_t repeat) {
  // Check if we have enough bytes to send a proper message.
  if (nbytes < kArgoStateLength) return;
  // TODO(kaschmo): validate
  sendGeneric(kArgoHdrMark, kArgoHdrSpace, kArgoBitMark, kArgoOneSpace,
              kArgoBitMark, kArgoZeroSpace, 0, 0,  // No Footer.
              data, nbytes, 38, false, repeat, kDutyDefault);
}
#endif  // SEND_ARGO

IRArgoAC::IRArgoAC(const uint16_t pin, const bool inverted,
                   const bool use_modulation)
      : _irsend(pin, inverted, use_modulation) { this->stateReset(); }

void IRArgoAC::begin(void) { _irsend.begin(); }

#if SEND_ARGO
void IRArgoAC::send(const uint16_t repeat) {
  _irsend.sendArgo(getRaw(), kArgoStateLength, repeat);
}
#endif  // SEND_ARGO

uint8_t IRArgoAC::calcChecksum(const uint8_t state[], const uint16_t length) {
  // Corresponds to byte 11 being constant 0b01
  // Only add up bytes to 9. byte 10 is 0b01 constant anyway.
  // Assume that argo array is MSB first (left)
  return sumBytes(state, length - 2, 2);
}

bool IRArgoAC::validChecksum(const uint8_t state[], const uint16_t length) {
  return ((state[length - 2] >> 2) + (state[length - 1] << 6)) ==
      IRArgoAC::calcChecksum(state, length);
}

void IRArgoAC::checksum(void) {
  uint8_t sum = IRArgoAC::calcChecksum(argo, kArgoStateLength);
  // Append sum to end of array
  // Set const part of checksum bit 10
  argo[10] = 0b00000010;
  argo[10] += sum << 2;  // Shift up 2 bits and append to byte 10
  argo[11] = sum >> 6;   // Shift down 6 bits and add in two LSBs of bit 11
}

void IRArgoAC::stateReset(void) {
  for (uint8_t i = 0; i < kArgoStateLength; i++) argo[i] = 0x0;

  // Argo Message. Store MSB left.
  // Default message:
  argo[0] = 0b10101100;  // LSB first (as sent) 0b00110101; //const preamble
  argo[1] = 0b11110101;  // LSB first: 0b10101111; //const preamble
  // Keep payload 2-9 at zero
  argo[10] = 0b00000010;  // Const 01, checksum 6bit
  argo[11] = 0b00000000;  // Checksum 2bit

  this->off();
  this->setTemp(20);
  this->setRoomTemp(25);
  this->setMode(kArgoAuto);
  this->setFan(kArgoFanAuto);
}

uint8_t* IRArgoAC::getRaw(void) {
  this->checksum();  // Ensure correct bit array before returning
  return argo;
}

void IRArgoAC::setRaw(const uint8_t state[]) {
  memcpy(argo, state, kArgoStateLength);
}

void IRArgoAC::on(void) { setPower(true); }

void IRArgoAC::off(void) { setPower(false); }

void IRArgoAC::setPower(const bool on) {
  setBit(&argo[9], kArgoPowerBitOffset, on);
}

bool IRArgoAC::getPower(void) { return GETBIT8(argo[9], kArgoPowerBitOffset); }

void IRArgoAC::setMax(const bool on) {
  setBit(&argo[9], kArgoMaxBitOffset, on);
}

bool IRArgoAC::getMax(void) { return GETBIT8(argo[9], kArgoMaxBitOffset); }

// Set the temp in deg C
// Sending 0 equals +4
void IRArgoAC::setTemp(const uint8_t degrees) {
  uint8_t temp = std::max(kArgoMinTemp, degrees);
  // delta 4 degrees. "If I want 12 degrees, I need to send 8"
  temp = std::min(kArgoMaxTemp, temp) - kArgoTempDelta;
  // Settemp = Bit 6,7 of byte 2, and bit 0-2 of byte 3
  // mask out bits
  // argo[13] & 0x00000100;  // mask out ON/OFF Bit
  setBits(&argo[2], kArgoTempLowOffset, kArgoTempLowSize, temp);
  setBits(&argo[3], kArgoTempHighOffset, kArgoTempHighSize,
          temp >> kArgoTempLowSize);
}

uint8_t IRArgoAC::getTemp(void) {
  return ((GETBITS8(argo[3], kArgoTempHighOffset,
                    kArgoTempHighSize) << kArgoTempLowSize) |
           GETBITS8(argo[2], kArgoTempLowOffset, kArgoTempLowSize)) +
      kArgoTempDelta;
}

// Set the speed of the fan
void IRArgoAC::setFan(const uint8_t fan) {
  setBits(&argo[3], kArgoFanOffset, kArgoFanSize, std::min(fan, kArgoFan3));
}

uint8_t IRArgoAC::getFan(void) {
  return GETBITS8(argo[3], kArgoFanOffset, kArgoFanSize);
}

void IRArgoAC::setFlap(const uint8_t flap) {
  flap_mode = flap;
  // TODO(kaschmo): set correct bits for flap mode
}

uint8_t IRArgoAC::getFlap(void) { return flap_mode; }

uint8_t IRArgoAC::getMode(void) {
  return GETBITS8(argo[2], kArgoModeOffset, kArgoModeSize);
}

void IRArgoAC::setMode(const uint8_t mode) {
  switch (mode) {
    case kArgoCool:
    case kArgoDry:
    case kArgoAuto:
    case kArgoOff:
    case kArgoHeat:
    case kArgoHeatAuto:
      setBits(&argo[2], kArgoModeOffset, kArgoModeSize, mode);
      return;
    default:
      this->setMode(kArgoAuto);
  }
}

void IRArgoAC::setNight(const bool on) {
  setBit(&argo[9], kArgoNightBitOffset, on);
}

bool IRArgoAC::getNight(void) { return GETBIT8(argo[9], kArgoNightBitOffset); }

void IRArgoAC::setiFeel(const bool on) {
  setBit(&argo[9], kArgoIFeelBitOffset, on);
}

bool IRArgoAC::getiFeel(void) { return GETBIT8(argo[9], kArgoIFeelBitOffset); }

void IRArgoAC::setTime(void) {
  // TODO(kaschmo): use function call from checksum to set time first
}

void IRArgoAC::setRoomTemp(const uint8_t degrees) {
  uint8_t temp = std::min(degrees, kArgoMaxRoomTemp);
  temp = std::max(temp, kArgoTempDelta) - kArgoTempDelta;
  setBits(&argo[3], kArgoRoomTempLowOffset, kArgoRoomTempLowSize, temp);
  setBits(&argo[4], kArgoRoomTempHighOffset, kArgoRoomTempHighSize,
          temp >> kArgoRoomTempLowSize);
}

uint8_t IRArgoAC::getRoomTemp(void) {
  return ((GETBITS8(argo[4], kArgoRoomTempHighOffset,
                   kArgoRoomTempHighSize) << kArgoRoomTempLowSize) |
           GETBITS8(argo[3], kArgoRoomTempLowOffset, kArgoRoomTempLowSize)) +
      kArgoTempDelta;
}

// Convert a standard A/C mode into its native mode.
uint8_t IRArgoAC::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool:
      return kArgoCool;
    case stdAc::opmode_t::kHeat:
      return kArgoHeat;
    case stdAc::opmode_t::kDry:
      return kArgoDry;
    case stdAc::opmode_t::kOff:
      return kArgoOff;
    // No fan mode.
    default:
      return kArgoAuto;
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRArgoAC::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:
      return kArgoFan1;
    case stdAc::fanspeed_t::kMedium:
      return kArgoFan2;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:
      return kArgoFan3;
    default:
      return kArgoFanAuto;
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRArgoAC::convertSwingV(const stdAc::swingv_t position) {
  switch (position) {
    case stdAc::swingv_t::kHighest:
      return kArgoFlapFull;
    case stdAc::swingv_t::kHigh:
      return kArgoFlap5;
    case stdAc::swingv_t::kMiddle:
      return kArgoFlap4;
    case stdAc::swingv_t::kLow:
      return kArgoFlap3;
    case stdAc::swingv_t::kLowest:
      return kArgoFlap1;
    default:
      return kArgoFlapAuto;
  }
}

// Convert a native mode to it's common equivalent.
stdAc::opmode_t IRArgoAC::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kArgoCool: return stdAc::opmode_t::kCool;
    case kArgoHeat: return stdAc::opmode_t::kHeat;
    case kArgoDry: return stdAc::opmode_t::kDry;
    // No fan mode.
    default: return stdAc::opmode_t::kAuto;
  }
}

// Convert a native fan speed to it's common equivalent.
stdAc::fanspeed_t IRArgoAC::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kArgoFan3: return stdAc::fanspeed_t::kMax;
    case kArgoFan2: return stdAc::fanspeed_t::kMedium;
    case kArgoFan1: return stdAc::fanspeed_t::kMin;
    default: return stdAc::fanspeed_t::kAuto;
  }
}

// Convert the A/C state to it's common equivalent.
stdAc::state_t IRArgoAC::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::ARGO;
  result.power = this->getPower();
  result.mode = this->toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = this->toCommonFanSpeed(this->getFan());
  result.turbo = this->getMax();
  result.sleep = this->getNight() ? 0 : -1;
  // Not supported.
  result.model = -1;  // Not supported.
  result.swingv = stdAc::swingv_t::kOff;
  result.swingh = stdAc::swingh_t::kOff;
  result.light = false;
  result.filter = false;
  result.econo = false;
  result.quiet = false;
  result.clean = false;
  result.beep = false;
  result.clock = -1;
  return result;
}

// Convert the internal state into a human readable string.
String IRArgoAC::toString(void) {
  String result = "";
  result.reserve(100);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addIntToString(getMode(), kModeStr);
  result += kSpaceLBraceStr;
  switch (getMode()) {
    case kArgoAuto:
      result += kAutoStr;
      break;
    case kArgoCool:
      result += kCoolStr;
      break;
    case kArgoHeat:
      result += kHeatStr;
      break;
    case kArgoDry:
      result += kDryStr;
      break;
    case kArgoHeatAuto:
      result += kHeatStr;
      result += ' ';
      result += kAutoStr;
      break;
    case kArgoOff:
      result += kOffStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addIntToString(getFan(), kFanStr);
  result += kSpaceLBraceStr;
  switch (getFan()) {
    case kArgoFanAuto:
      result += kAutoStr;
      break;
    case kArgoFan3:
      result += kMaxStr;
      break;
    case kArgoFan1:
      result += kMinStr;
      break;
    case kArgoFan2:
      result += kMedStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addTempToString(getTemp());
  result += kCommaSpaceStr;
  result += kRoomStr;
  result += ' ';
  result += addTempToString(getRoomTemp(), true, false);
  result += addBoolToString(getMax(), kMaxStr);
  result += addBoolToString(getiFeel(), kIFeelStr);
  result += addBoolToString(getNight(), kNightStr);
  return result;
}

#if DECODE_ARGO
// Decode the supplied Argo message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect. Typically kArgoBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: ALPHA / Probably doesn't work.
//
// Note:
//   This decoder is based soley off sendArgo(). We have no actual captures
//   to test this against. If you have one of these units, please let us know.
bool IRrecv::decodeArgo(decode_results *results, const uint16_t nbits,
                        const bool strict) {
  if (strict && nbits != kArgoBits) return false;

  uint16_t offset = kStartOffset;

  // Match Header + Data
  if (!matchGeneric(results->rawbuf + offset, results->state,
                    results->rawlen - offset, nbits,
                    kArgoHdrMark, kArgoHdrSpace,
                    kArgoBitMark, kArgoOneSpace,
                    kArgoBitMark, kArgoZeroSpace,
                    0, 0,  // Footer (None, allegedly. This seems very wrong.)
                    true, _tolerance, 0, false)) return false;

  // Compliance
  // Verify we got a valid checksum.
  if (strict && !IRArgoAC::validChecksum(results->state)) return false;
  // Success
  results->decode_type = decode_type_t::ARGO;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_ARGO
