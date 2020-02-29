// Copyright 2019 ribeirodanielf
// Copyright 2019 David Conran
//
// Code to emulate Goodweather protocol compatible HVAC devices.
// Should be compatible with:
// * ZH/JT-03 remote control
//

#include "ir_Goodweather.h"
#include <algorithm>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

using irutils::addBoolToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addFanToString;
using irutils::addTempToString;
using irutils::setBit;
using irutils::setBits;

#if SEND_GOODWEATHER
// Send a Goodweather message.
//
// Args:
//   data: The raw message to be sent.
//   nbits: Nr. of bits of data in the message. (Default is kGoodweatherBits)
//   repeat: Nr. of times the message is to be repeated. (Default = 0).
//
// Status: ALPHA / Untested.
//
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/697
void IRsend::sendGoodweather(const uint64_t data, const uint16_t nbits,
                             const uint16_t repeat) {
  if (nbits != kGoodweatherBits)
    return;  // Wrong nr. of bits to send a proper message.
  // Set IR carrier frequency
  enableIROut(38);

  for (uint16_t r = 0; r <= repeat; r++) {
    // Header
    mark(kGoodweatherHdrMark);
    space(kGoodweatherHdrSpace);

    // Data
    for (int16_t i = 0; i < nbits; i += 8) {
      uint16_t chunk = (data >> i) & 0xFF;  // Grab a byte at a time.
      chunk = (~chunk) << 8 | chunk;  // Prepend a inverted copy of the byte.
      sendData(kGoodweatherBitMark, kGoodweatherOneSpace,
               kGoodweatherBitMark, kGoodweatherZeroSpace,
               chunk, 16, false);
    }
    // Footer
    mark(kGoodweatherBitMark);
    space(kGoodweatherHdrSpace);
    mark(kGoodweatherBitMark);
    space(kDefaultMessageGap);
  }
}
#endif  // SEND_GOODWEATHER

IRGoodweatherAc::IRGoodweatherAc(const uint16_t pin, const bool inverted,
                                 const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

void IRGoodweatherAc::stateReset(void) { remote = kGoodweatherStateInit; }

void IRGoodweatherAc::begin(void) { _irsend.begin(); }

#if SEND_GOODWEATHER
void IRGoodweatherAc::send(const uint16_t repeat) {
  _irsend.sendGoodweather(remote, kGoodweatherBits, repeat);
}
#endif  // SEND_GOODWEATHER

uint64_t IRGoodweatherAc::getRaw(void) { return remote; }

void IRGoodweatherAc::setRaw(const uint64_t state) { remote = state; }

void IRGoodweatherAc::on(void) { this->setPower(true); }

void IRGoodweatherAc::off(void) { this->setPower(false); }

void IRGoodweatherAc::setPower(const bool on) {
  this->setCommand(kGoodweatherCmdPower);
  setBit(&remote, kGoodweatherBitPower, on);
}

bool IRGoodweatherAc::getPower(void) {
  return GETBIT64(remote, kGoodweatherBitPower);
}

// Set the temp. in deg C
void IRGoodweatherAc::setTemp(const uint8_t temp) {
  uint8_t new_temp = std::max(kGoodweatherTempMin, temp);
  new_temp = std::min(kGoodweatherTempMax, new_temp);
  if (new_temp > this->getTemp()) this->setCommand(kGoodweatherCmdUpTemp);
  if (new_temp < this->getTemp()) this->setCommand(kGoodweatherCmdDownTemp);
  setBits(&remote, kGoodweatherBitTemp, kGoodweatherTempSize,
          new_temp - kGoodweatherTempMin);
}

// Return the set temp. in deg C
uint8_t IRGoodweatherAc::getTemp(void) {
  return GETBITS64(remote, kGoodweatherBitTemp, kGoodweatherTempSize) +
      kGoodweatherTempMin;
}

// Set the speed of the fan
void IRGoodweatherAc::setFan(const uint8_t speed) {
  switch (speed) {
    case kGoodweatherFanAuto:
    case kGoodweatherFanLow:
    case kGoodweatherFanMed:
    case kGoodweatherFanHigh:
      this->setCommand(kGoodweatherCmdFan);
      setBits(&remote, kGoodweatherBitFan, kGoodweatherFanSize, speed);
      break;
    default:
      this->setFan(kGoodweatherFanAuto);
  }
}

uint8_t IRGoodweatherAc::getFan() {
  return GETBITS64(remote, kGoodweatherBitFan, kGoodweatherFanSize);
}

void IRGoodweatherAc::setMode(const uint8_t mode) {
  switch (mode) {
    case kGoodweatherAuto:
    case kGoodweatherDry:
    case kGoodweatherCool:
    case kGoodweatherFan:
    case kGoodweatherHeat:
      this->setCommand(kGoodweatherCmdMode);
      setBits(&remote, kGoodweatherBitMode, kModeBitsSize, mode);
      break;
    default:
      // If we get an unexpected mode, default to AUTO.
      this->setMode(kGoodweatherAuto);
  }
}

uint8_t IRGoodweatherAc::getMode() {
  return GETBITS64(remote, kGoodweatherBitMode, kModeBitsSize);
}

void IRGoodweatherAc::setLight(const bool toggle) {
  this->setCommand(kGoodweatherCmdLight);
  setBit(&remote, kGoodweatherBitLight, toggle);
}

bool IRGoodweatherAc::getLight() {
  return GETBIT64(remote, kGoodweatherBitLight);
}

void IRGoodweatherAc::setSleep(const bool toggle) {
  this->setCommand(kGoodweatherCmdSleep);
  setBit(&remote, kGoodweatherBitSleep, toggle);
}

bool IRGoodweatherAc::getSleep() {
  return GETBIT64(remote, kGoodweatherBitSleep);
}

void IRGoodweatherAc::setTurbo(const bool toggle) {
  this->setCommand(kGoodweatherCmdTurbo);
  setBit(&remote, kGoodweatherBitTurbo, toggle);
}

bool IRGoodweatherAc::getTurbo() {
  return GETBIT64(remote, kGoodweatherBitTurbo);
}

void IRGoodweatherAc::setSwing(const uint8_t speed) {
  switch (speed) {
    case kGoodweatherSwingOff:
    case kGoodweatherSwingSlow:
    case kGoodweatherSwingFast:
      this->setCommand(kGoodweatherCmdSwing);
      setBits(&remote, kGoodweatherBitSwing, kGoodweatherSwingSize, speed);
      break;
    default:
      this->setSwing(kGoodweatherSwingOff);
  }
}

uint8_t IRGoodweatherAc::getSwing() {
  return GETBITS64(remote, kGoodweatherBitSwing, kGoodweatherSwingSize);
}

void IRGoodweatherAc::setCommand(const uint8_t cmd) {
  if (cmd <= kGoodweatherCmdLight)
    setBits(&remote, kGoodweatherBitCommand, kGoodweatherCommandSize, cmd);
}

uint8_t IRGoodweatherAc::getCommand() {
  return GETBITS64(remote, kGoodweatherBitCommand, kGoodweatherCommandSize);
}

// Convert a standard A/C mode into its native mode.
uint8_t IRGoodweatherAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kGoodweatherCool;
    case stdAc::opmode_t::kHeat: return kGoodweatherHeat;
    case stdAc::opmode_t::kDry:  return kGoodweatherDry;
    case stdAc::opmode_t::kFan:  return kGoodweatherFan;
    default:                     return kGoodweatherAuto;
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRGoodweatherAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kGoodweatherFanLow;
    case stdAc::fanspeed_t::kMedium: return kGoodweatherFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kGoodweatherFanHigh;
    default:                         return kGoodweatherFanAuto;
  }
}

// Convert a standard A/C Vertical Swing into its native version.
uint8_t IRGoodweatherAc::convertSwingV(const stdAc::swingv_t swingv) {
  switch (swingv) {
    case stdAc::swingv_t::kHighest:
    case stdAc::swingv_t::kHigh:
    case stdAc::swingv_t::kMiddle: return kGoodweatherSwingFast;
    case stdAc::swingv_t::kLow:
    case stdAc::swingv_t::kLowest:
    case stdAc::swingv_t::kAuto:   return kGoodweatherSwingSlow;
    default:                       return kGoodweatherSwingOff;
  }
}

// Convert a native mode to it's common equivalent.
stdAc::opmode_t IRGoodweatherAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kGoodweatherCool: return stdAc::opmode_t::kCool;
    case kGoodweatherHeat: return stdAc::opmode_t::kHeat;
    case kGoodweatherDry:  return stdAc::opmode_t::kDry;
    case kGoodweatherFan:  return stdAc::opmode_t::kFan;
    default:               return stdAc::opmode_t::kAuto;
  }
}

// Convert a native fan speed to it's common equivalent.
stdAc::fanspeed_t IRGoodweatherAc::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kGoodweatherFanHigh: return stdAc::fanspeed_t::kMax;
    case kGoodweatherFanMed:  return stdAc::fanspeed_t::kMedium;
    case kGoodweatherFanLow:  return stdAc::fanspeed_t::kMin;
    default:                  return stdAc::fanspeed_t::kAuto;
  }
}

// Convert the A/C state to it's common equivalent.
stdAc::state_t IRGoodweatherAc::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::GOODWEATHER;
  result.power = this->getPower();
  result.mode = this->toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = this->toCommonFanSpeed(this->getFan());
  result.swingv = this->getSwing() == kGoodweatherSwingOff ?
      stdAc::swingv_t::kOff : stdAc::swingv_t::kAuto;
  result.turbo = this->getTurbo();
  result.light = this->getLight();
  result.sleep = this->getSleep() ? 0: -1;
  // Not supported.
  result.model = -1;
  result.swingh = stdAc::swingh_t::kOff;
  result.quiet = false;
  result.econo = false;
  result.filter = false;
  result.clean = false;
  result.beep = false;
  result.clock = -1;
  return result;
}

// Convert the internal state into a human readable string.
String IRGoodweatherAc::toString(void) {
  String result = "";
  result.reserve(150);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(getMode(), kGoodweatherAuto, kGoodweatherCool,
                            kGoodweatherHeat, kGoodweatherDry, kGoodweatherFan);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kGoodweatherFanHigh, kGoodweatherFanLow,
                           kGoodweatherFanAuto, kGoodweatherFanAuto,
                           kGoodweatherFanMed);
  result += addLabeledString(getTurbo() ? kToggleStr : "-", kTurboStr);
  result += addLabeledString(getLight() ? kToggleStr : "-", kLightStr);
  result += addLabeledString(getSleep() ? kToggleStr : "-", kSleepStr);
  result += addIntToString(getSwing(), kSwingStr);
  result += kSpaceLBraceStr;
  switch (this->getSwing()) {
    case kGoodweatherSwingFast:
      result += kFastStr;
      break;
    case kGoodweatherSwingSlow:
      result += kSlowStr;
      break;
    case kGoodweatherSwingOff:
      result += kOffStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addIntToString(getCommand(), kCommandStr);
  result += kSpaceLBraceStr;
  switch (this->getCommand()) {
    case kGoodweatherCmdPower:
      result += kPowerStr;
      break;
    case kGoodweatherCmdMode:
      result += kModeStr;
      break;
    case kGoodweatherCmdUpTemp:
      result += kTempUpStr;
      break;
    case kGoodweatherCmdDownTemp:
      result += kTempDownStr;
      break;
    case kGoodweatherCmdSwing:
      result += kSwingStr;
      break;
    case kGoodweatherCmdFan:
      result += kFanStr;
      break;
    case kGoodweatherCmdTimer:
      result += kTimerStr;
      break;
    case kGoodweatherCmdAirFlow:
      result += kAirFlowStr;
      break;
    case kGoodweatherCmdHold:
      result += kHoldStr;
      break;
    case kGoodweatherCmdSleep:
      result += kSleepStr;
      break;
    case kGoodweatherCmdTurbo:
      result += kTurboStr;
      break;
    case kGoodweatherCmdLight:
      result += kLightStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  return result;
}

#if DECODE_GOODWEATHER
// Decode the supplied Goodweather message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   offset:  The starting index to use when attempting to decode the raw data.
//            Typically/Defaults to kStartOffset.
//   nbits:   The number of data bits to expect. Typically kGoodweatherBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: ALPHA / Untested.
bool IRrecv::decodeGoodweather(decode_results* results, uint16_t offset,
                               const uint16_t nbits,
                               const bool strict) {
  if (results->rawlen < 2 * (2 * nbits) + kHeader + 2 * kFooter - 1 + offset)
    return false;  // Can't possibly be a valid Goodweather message.
  if (strict && nbits != kGoodweatherBits)
    return false;  // Not strictly a Goodweather message.

  uint64_t dataSoFar = 0;
  uint16_t dataBitsSoFar = 0;
  match_result_t data_result;

  // Header
  if (!matchMark(results->rawbuf[offset++], kGoodweatherHdrMark)) return false;
  if (!matchSpace(results->rawbuf[offset++], kGoodweatherHdrSpace))
    return false;

  // Data
  for (; offset <= results->rawlen - 32 && dataBitsSoFar < nbits;
       dataBitsSoFar += 8) {
    DPRINT("DEBUG: Attempting Byte #");
    DPRINTLN(dataBitsSoFar / 8);
    // Read in a byte at a time.
    // Normal first.
    data_result = matchData(&(results->rawbuf[offset]), 8,
                            kGoodweatherBitMark, kGoodweatherOneSpace,
                            kGoodweatherBitMark, kGoodweatherZeroSpace,
                            _tolerance + kGoodweatherExtraTolerance,
                            kMarkExcess, false);
    if (data_result.success == false) return false;
    DPRINTLN("DEBUG: Normal byte read okay.");
    offset += data_result.used;
    uint8_t data = (uint8_t)data_result.data;
    // Then inverted.
    data_result = matchData(&(results->rawbuf[offset]), 8,
                            kGoodweatherBitMark, kGoodweatherOneSpace,
                            kGoodweatherBitMark, kGoodweatherZeroSpace,
                            _tolerance + kGoodweatherExtraTolerance,
                            kMarkExcess, false);
    if (data_result.success == false) return false;
    DPRINTLN("DEBUG: Inverted byte read okay.");
    offset += data_result.used;
    uint8_t inverted = (uint8_t)data_result.data;
    DPRINT("DEBUG: data = ");
    DPRINTLN((uint16_t)data);
    DPRINT("DEBUG: inverted = ");
    DPRINTLN((uint16_t)inverted);
    if (data != (inverted ^ 0xFF)) return false;  // Data integrity failed.
    dataSoFar |= (uint64_t)data << dataBitsSoFar;
  }

  // Footer.
  if (!matchMark(results->rawbuf[offset++], kGoodweatherBitMark,
                 _tolerance + kGoodweatherExtraTolerance)) return false;
  if (!matchSpace(results->rawbuf[offset++], kGoodweatherHdrSpace))
    return false;
  if (!matchMark(results->rawbuf[offset++], kGoodweatherBitMark,
                 _tolerance + kGoodweatherExtraTolerance)) return false;
  if (offset <= results->rawlen &&
      !matchAtLeast(results->rawbuf[offset], kGoodweatherHdrSpace))
    return false;

  // Compliance
  if (strict && (dataBitsSoFar != kGoodweatherBits)) return false;

  // Success
  results->decode_type = decode_type_t::GOODWEATHER;
  results->bits = dataBitsSoFar;
  results->value = dataSoFar;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif  // DECODE_GOODWEATHER
