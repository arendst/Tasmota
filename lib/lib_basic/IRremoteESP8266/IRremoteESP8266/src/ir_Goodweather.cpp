// Copyright 2019 ribeirodanielf
// Copyright 2019 David Conran
/// @file
/// @brief Support for Goodweather compatible HVAC protocols.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/697

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
using irutils::addToggleToString;

#if SEND_GOODWEATHER
/// Send a Goodweather HVAC formatted message.
/// Status: BETA / Needs testing on real device.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
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

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRGoodweatherAc::IRGoodweatherAc(const uint16_t pin, const bool inverted,
                                 const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Reset the internal state to a fixed known good state.
void IRGoodweatherAc::stateReset(void) { _.raw = kGoodweatherStateInit; }

/// Set up hardware to be able to send a message.
void IRGoodweatherAc::begin(void) { _irsend.begin(); }

#if SEND_GOODWEATHER
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRGoodweatherAc::send(const uint16_t repeat) {
  _irsend.sendGoodweather(getRaw(), kGoodweatherBits, repeat);
}
#endif  // SEND_GOODWEATHER

/// Get a copy of the internal state as a valid code for this protocol.
/// @return A valid code for this protocol based on the current internal state.
uint64_t IRGoodweatherAc::getRaw(void) { return _.raw; }

/// Set the internal state from a valid code for this protocol.
/// @param[in] state A valid code for this protocol.
void IRGoodweatherAc::setRaw(const uint64_t state) { _.raw  = state; }

/// Change the power setting to On.
void IRGoodweatherAc::on(void) { setPower(true); }

/// Change the power setting to Off.
void IRGoodweatherAc::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRGoodweatherAc::setPower(const bool on) {
  _.Command = kGoodweatherCmdPower;
  _.Power = on;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRGoodweatherAc::getPower(void) const {
  return _.Power;
}

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
void IRGoodweatherAc::setTemp(const uint8_t temp) {
  uint8_t new_temp = std::max(kGoodweatherTempMin, temp);
  new_temp = std::min(kGoodweatherTempMax, new_temp);
  if (new_temp > getTemp()) _.Command = kGoodweatherCmdUpTemp;
  if (new_temp < getTemp()) _.Command = kGoodweatherCmdDownTemp;
  _.Temp = new_temp - kGoodweatherTempMin;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRGoodweatherAc::getTemp(void) const {
  return _.Temp + kGoodweatherTempMin;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRGoodweatherAc::setFan(const uint8_t speed) {
  _.Command = kGoodweatherCmdFan;
  switch (speed) {
    case kGoodweatherFanAuto:
    case kGoodweatherFanLow:
    case kGoodweatherFanMed:
    case kGoodweatherFanHigh:
      _.Fan = speed;
      break;
    default:
      _.Fan = kGoodweatherFanAuto;
  }
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRGoodweatherAc::getFan(void) const {
  return _.Fan;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRGoodweatherAc::setMode(const uint8_t mode) {
  _.Command = kGoodweatherCmdMode;
  switch (mode) {
    case kGoodweatherAuto:
    case kGoodweatherDry:
    case kGoodweatherCool:
    case kGoodweatherFan:
    case kGoodweatherHeat:
      _.Mode = mode;
      break;
    default:
      _.Mode = kGoodweatherAuto;
  }
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRGoodweatherAc::getMode(void) const {
  return _.Mode;
}

/// Set the Light (LED) Toggle setting of the A/C.
/// @param[in] toggle true, the setting is on. false, the setting is off.
void IRGoodweatherAc::setLight(const bool toggle) {
  _.Command = kGoodweatherCmdLight;
  _.Light = toggle;
}

/// Get the Light (LED) Toggle setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRGoodweatherAc::getLight(void) const {
  return _.Light;
}

/// Set the Sleep Toggle setting of the A/C.
/// @param[in] toggle true, the setting is on. false, the setting is off.
void IRGoodweatherAc::setSleep(const bool toggle) {
  _.Command = kGoodweatherCmdSleep;
  _.Sleep = toggle;
}

/// Get the Sleep Toggle setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRGoodweatherAc::getSleep(void) const {
  return _.Sleep;
}

/// Set the Turbo Toggle setting of the A/C.
/// @param[in] toggle true, the setting is on. false, the setting is off.
void IRGoodweatherAc::setTurbo(const bool toggle) {
  _.Command = kGoodweatherCmdTurbo;
  _.Turbo = toggle;
}

/// Get the Turbo Toggle setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRGoodweatherAc::getTurbo(void) const {
  return _.Turbo;
}

/// Set the Vertical Swing speed of the A/C.
/// @param[in] speed The speed to set the swing to.
void IRGoodweatherAc::setSwing(const uint8_t speed) {
  _.Command = kGoodweatherCmdSwing;
  switch (speed) {
    case kGoodweatherSwingOff:
    case kGoodweatherSwingSlow:
    case kGoodweatherSwingFast:
      _.Swing = speed;
      break;
    default:
      _.Swing = kGoodweatherSwingOff;
  }
}

/// Get the Vertical Swing speed of the A/C.
/// @return The native swing speed setting.
uint8_t IRGoodweatherAc::getSwing(void) const {
  return _.Swing;
}

/// Set the remote Command type/button pressed.
/// @param[in] cmd The command/button that was issued/pressed.
void IRGoodweatherAc::setCommand(const uint8_t cmd) {
  if (cmd <= kGoodweatherCmdLight)
    _.Command = cmd;
}

/// Get the Command type/button pressed from the current settings
/// @return The command/button that was issued/pressed.
uint8_t IRGoodweatherAc::getCommand(void) const {
  return _.Command;
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRGoodweatherAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kGoodweatherCool;
    case stdAc::opmode_t::kHeat: return kGoodweatherHeat;
    case stdAc::opmode_t::kDry:  return kGoodweatherDry;
    case stdAc::opmode_t::kFan:  return kGoodweatherFan;
    default:                     return kGoodweatherAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
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

/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] swingv The enum to be converted.
/// @return The native equivalent of the enum.
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

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRGoodweatherAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kGoodweatherCool: return stdAc::opmode_t::kCool;
    case kGoodweatherHeat: return stdAc::opmode_t::kHeat;
    case kGoodweatherDry:  return stdAc::opmode_t::kDry;
    case kGoodweatherFan:  return stdAc::opmode_t::kFan;
    default:               return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRGoodweatherAc::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kGoodweatherFanHigh: return stdAc::fanspeed_t::kMax;
    case kGoodweatherFanMed:  return stdAc::fanspeed_t::kMedium;
    case kGoodweatherFanLow:  return stdAc::fanspeed_t::kMin;
    default:                  return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRGoodweatherAc::toCommon(void) const {
  stdAc::state_t result{};
  result.protocol = decode_type_t::GOODWEATHER;
  result.power = _.Power;
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.swingv = (_.Swing == kGoodweatherSwingOff ?
      stdAc::swingv_t::kOff : stdAc::swingv_t::kAuto);
  result.turbo = _.Turbo;
  result.light = _.Light;
  result.sleep = _.Sleep ? 0: -1;
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

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRGoodweatherAc::toString(void) const {
  String result = "";
  result.reserve(150);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(_.Power, kPowerStr, false);
  result += addModeToString(_.Mode, kGoodweatherAuto, kGoodweatherCool,
                            kGoodweatherHeat, kGoodweatherDry, kGoodweatherFan);
  result += addTempToString(getTemp());
  result += addFanToString(_.Fan, kGoodweatherFanHigh, kGoodweatherFanLow,
                           kGoodweatherFanAuto, kGoodweatherFanAuto,
                           kGoodweatherFanMed);

  result += addToggleToString(_.Turbo, kTurboStr);
  result += addToggleToString(_.Light, kLightStr);
  result += addToggleToString(_.Sleep, kSleepStr);
  result += addIntToString(_.Swing, kSwingStr);
  result += kSpaceLBraceStr;
  switch (_.Swing) {
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
  result += addIntToString(_.Command, kCommandStr);
  result += kSpaceLBraceStr;
  switch (_.Command) {
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
/// Decode the supplied Goodweather message.
/// Status: BETA / Probably works.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
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
