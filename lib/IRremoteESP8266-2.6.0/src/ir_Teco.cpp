// Copyright 2019 Fabien Valthier
/*
Node MCU/ESP8266 Sketch to emulate Teco
*/

#include "ir_Teco.h"
#include <algorithm>
#include "IRremoteESP8266.h"
#include "IRutils.h"
#ifndef ARDUINO
#include <string>
#endif

// Constants
// using SPACE modulation.
const uint16_t kTecoHdrMark = 9000;
const uint16_t kTecoHdrSpace = 4440;
const uint16_t kTecoBitMark = 620;
const uint16_t kTecoOneSpace = 1650;
const uint16_t kTecoZeroSpace = 580;
const uint32_t kTecoGap = kDefaultMessageGap;  // Made-up value. Just a guess.

#if SEND_TECO
// Send a Teco A/C message.
//
// Args:
//   data:   Contents of the message to be sent.
//   nbits:  Nr. of bits of data to be sent. Typically kTecoBits.
//   repeat: Nr. of additional times the message is to be sent.
void IRsend::sendTeco(uint64_t data, uint16_t nbits, uint16_t repeat) {
  sendGeneric(kTecoHdrMark, kTecoHdrSpace, kTecoBitMark, kTecoOneSpace,
              kTecoBitMark, kTecoZeroSpace, kTecoBitMark, kTecoGap,
              data, nbits, 38000, false, repeat, kDutyDefault);
}
#endif  // SEND_TECO

// Class for decoding and constructing Teco AC messages.
IRTecoAc::IRTecoAc(const uint16_t pin) : _irsend(pin) { stateReset(); }

void IRTecoAc::begin() { _irsend.begin(); }

#if SEND_TECO
void IRTecoAc::send(const uint16_t repeat) {
  _irsend.sendTeco(remote_state, kTecoBits, repeat);
}
#endif  // SEND_TECO

void IRTecoAc::stateReset(void) {
  // Mode:auto, Power:Off, fan:auto, temp:16, swing:off, sleep:off
  remote_state = kTecoReset;
}

uint64_t IRTecoAc::getRaw(void) { return remote_state; }

void IRTecoAc::setRaw(const uint64_t new_code) { remote_state = new_code; }

void IRTecoAc::on(void) { remote_state |= kTecoPower; }

void IRTecoAc::off(void) { remote_state &= ~kTecoPower; }

void IRTecoAc::setPower(const bool on) {
  if (on)
    this->on();
  else
    this->off();
}

bool IRTecoAc::getPower(void) {
  return (remote_state & kTecoPower) == kTecoPower; }

void IRTecoAc::setTemp(const uint8_t temp) {
  uint8_t newtemp = temp;
  newtemp = std::min(newtemp, kTecoMaxTemp);
  newtemp = std::max(newtemp, kTecoMinTemp);
  newtemp -= kTecoMinTemp;  // 16=0b000

  remote_state &= ~kTecoTempMask;  // reinit temp
  remote_state |= (newtemp << 8);
}

uint8_t IRTecoAc::getTemp(void) {
  return ((remote_state & kTecoTempMask) >> 8) + kTecoMinTemp;
}

// Set the speed of the fan
void IRTecoAc::setFan(const uint8_t speed) {
  uint8_t newspeed = speed;
  switch (speed) {
    case kTecoFanAuto:
    case kTecoFanHigh:
    case kTecoFanMed:
    case kTecoFanLow:
      break;
    default:
      newspeed = kTecoFanAuto;
  }
  remote_state &= ~kTecoFanMask;  // reinit fan
  remote_state |= (newspeed << 4);
}

uint8_t IRTecoAc::getFan(void) { return (remote_state & kTecoFanMask) >> 4; }

void IRTecoAc::setMode(const uint8_t mode) {
  uint8_t newmode = mode;
  switch (mode) {
    case kTecoAuto:
    case kTecoCool:
    case kTecoDry:
    case kTecoFan:
    case kTecoHeat:
      break;
    default:
      newmode = kTecoAuto;
  }
  remote_state &= ~kTecoModeMask;  // reinit mode
  remote_state |= newmode;
}

uint8_t IRTecoAc::getMode(void) { return remote_state & kTecoModeMask; }

void IRTecoAc::setSwing(const bool on) {
  if (on)
    remote_state |= kTecoSwing;
  else
    remote_state &= ~kTecoSwing;
}

bool IRTecoAc::getSwing(void) { return remote_state & kTecoSwing; }

void IRTecoAc::setSleep(const bool on) {
  if (on)
    remote_state |= kTecoSleep;
  else
    remote_state &= ~kTecoSleep;
}

bool IRTecoAc::getSleep(void) { return remote_state & kTecoSleep; }

// Convert a standard A/C mode into its native mode.
uint8_t IRTecoAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool:
      return kTecoCool;
    case stdAc::opmode_t::kHeat:
      return kTecoHeat;
    case stdAc::opmode_t::kDry:
      return kTecoDry;
    case stdAc::opmode_t::kFan:
      return kTecoFan;
    default:
      return kTecoAuto;
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRTecoAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:
      return kTecoFanLow;
    case stdAc::fanspeed_t::kMedium:
      return kTecoFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:
      return kTecoFanHigh;
    default:
      return kTecoFanAuto;
  }
}

// Convert the internal state into a human readable string.
#ifdef ARDUINO
String IRTecoAc::toString(void) {
  String result = "";
#else
std::string IRTecoAc::toString(void) {
  std::string result = "";
#endif  // ARDUINO
  result += F("Power: ");
  result += (this->getPower() ? F("On") : F("Off"));
  result += F(", Mode: ");
  result += uint64ToString(this->getMode());
  switch (this->getMode()) {
    case kTecoAuto:
      result += F(" (AUTO)");
      break;
    case kTecoCool:
      result += F(" (COOL)");
      break;
    case kTecoHeat:
      result += F(" (HEAT)");
      break;
    case kTecoDry:
      result += F(" (DRY)");
      break;
    case kTecoFan:
      result += F(" (FAN)");
      break;
    default:
      result += F(" (UNKNOWN)");
  }
  result += F(", Temp: ");
  result += uint64ToString(getTemp());
  result += F("C, Fan: ");
  result += uint64ToString(getFan());
  switch (this->getFan()) {
    case kTecoFanAuto:
      result += F(" (Auto)");
      break;
    case kTecoFanHigh:
      result += F(" (High)");
      break;
    case kTecoFanLow:
      result += F(" (Low)");
      break;
    case kTecoFanMed:
      result += F(" (Med)");
      break;
    default:
      result += F(" (UNKNOWN)");
  }
  result += F(", Sleep: ");
  result += (this->getSleep() ? F("On") : F("Off"));
  result += F(", Swing: ");
  result += (this->getSwing() ? F("On") : F("Off"));
  return result;
}

#if DECODE_TECO
// Decode the supplied Teco message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect. Typically kTecoBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: STABLE / Tested.
bool IRrecv::decodeTeco(decode_results* results, uint16_t nbits, bool strict) {
  // Check if can possibly be a valid Teco message.
  if (results->rawlen < 2 * nbits + kHeader + kFooter - 1) return false;
  if (strict && nbits != kTecoBits) return false;  // Not what is expected

  uint64_t data = 0;
  uint16_t offset = kStartOffset;
  match_result_t data_result;

  // Header
  if (!matchMark(results->rawbuf[offset++], kTecoHdrMark)) return false;
  if (!matchSpace(results->rawbuf[offset++], kTecoHdrSpace)) return false;
  // Data (35 bits)
  data_result =
      matchData(&(results->rawbuf[offset]), 35, kTecoBitMark, kTecoOneSpace,
                kTecoBitMark, kTecoZeroSpace, kTolerance, kMarkExcess, false);
  if (data_result.success == false) return false;
  data = data_result.data;
  offset += data_result.used;
  uint16_t actualBits = data_result.used / 2;

  // Footer.
  if (!matchMark(results->rawbuf[offset++], kTecoBitMark)) return false;
  if (offset < results->rawlen &&
      !matchAtLeast(results->rawbuf[offset], kTecoGap)) return false;

  // Compliance
  if (actualBits < nbits) return false;
  if (strict && actualBits != nbits) return false;  // Not as we expected.

  // Success
  results->decode_type = TECO;
  results->bits = actualBits;
  results->value = data;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif  // DECODE_TECO
