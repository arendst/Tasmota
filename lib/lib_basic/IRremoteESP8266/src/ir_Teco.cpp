// Copyright 2019 Fabien Valthier

/// @file
/// @brief Support for Teco protocols.

#include "ir_Teco.h"
#include <algorithm>
#include "IRremoteESP8266.h"
#include "IRtext.h"
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

using irutils::addBoolToString;
using irutils::addFanToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;

#if SEND_TECO
/// Send a Teco A/C message.
/// Status: Beta / Probably working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendTeco(const uint64_t data, const uint16_t nbits,
                      const uint16_t repeat) {
  sendGeneric(kTecoHdrMark, kTecoHdrSpace, kTecoBitMark, kTecoOneSpace,
              kTecoBitMark, kTecoZeroSpace, kTecoBitMark, kTecoGap,
              data, nbits, 38000, false, repeat, kDutyDefault);
}
#endif  // SEND_TECO

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRTecoAc::IRTecoAc(const uint16_t pin, const bool inverted,
                   const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Set up hardware to be able to send a message.
void IRTecoAc::begin(void) { _irsend.begin(); }

#if SEND_TECO
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRTecoAc::send(const uint16_t repeat) {
  _irsend.sendTeco(_.raw, kTecoBits, repeat);
}
#endif  // SEND_TECO

/// Reset the internal state of the emulation.
/// @note Mode:auto, Power:Off, fan:auto, temp:16, swing:off, sleep:off
void IRTecoAc::stateReset(void) {
  _.raw = kTecoReset;
}

/// Get a copy of the internal state/code for this protocol.
/// @return A code for this protocol based on the current internal state.
uint64_t IRTecoAc::getRaw(void) const { return _.raw; }

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
void IRTecoAc::setRaw(const uint64_t new_code) { _.raw = new_code; }

/// Set the requested power state of the A/C to on.
void IRTecoAc::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void IRTecoAc::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRTecoAc::setPower(const bool on) {
  _.Power = on;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRTecoAc::getPower(void) const {
  return _.Power;
}

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
void IRTecoAc::setTemp(const uint8_t temp) {
  uint8_t newtemp = temp;
  newtemp = std::min(newtemp, kTecoMaxTemp);
  newtemp = std::max(newtemp, kTecoMinTemp);
  _.Temp = newtemp - kTecoMinTemp;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRTecoAc::getTemp(void) const {
  return _.Temp + kTecoMinTemp;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRTecoAc::setFan(const uint8_t speed) {
  uint8_t newspeed = speed;
  switch (speed) {
    case kTecoFanAuto:
    case kTecoFanHigh:
    case kTecoFanMed:
    case kTecoFanLow: break;
    default: newspeed = kTecoFanAuto;
  }
  _.Fan = newspeed;
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t IRTecoAc::getFan(void) const {
  return _.Fan;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRTecoAc::setMode(const uint8_t mode) {
  uint8_t newmode = mode;
  switch (mode) {
    case kTecoAuto:
    case kTecoCool:
    case kTecoDry:
    case kTecoFan:
    case kTecoHeat: break;
    default: newmode = kTecoAuto;
  }
  _.Mode = newmode;
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRTecoAc::getMode(void) const {
  return _.Mode;
}

/// Set the (vertical) swing setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRTecoAc::setSwing(const bool on) {
  _.Swing = on;
}

/// Get the (vertical) swing setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRTecoAc::getSwing(void) const {
  return _.Swing;
}

/// Set the Sleep setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRTecoAc::setSleep(const bool on) {
  _.Sleep = on;
}

/// Get the Sleep setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRTecoAc::getSleep(void) const {
  return _.Sleep;
}

/// Set the Light (LED/Display) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRTecoAc::setLight(const bool on) {
  _.Light = on;
}

/// Get the Light (LED/Display) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRTecoAc::getLight(void) const {
  return _.Light;
}

/// Set the Humid setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRTecoAc::setHumid(const bool on) {
  _.Humid = on;
}

/// Get the Humid setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRTecoAc::getHumid(void) const {
  return _.Humid;
}

/// Set the Save setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRTecoAc::setSave(const bool on) {
  _.Save = on;
}

/// Get the Save setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRTecoAc::getSave(void) const {
  return _.Save;
}

/// Is the timer function enabled?
/// @return true, the setting is on. false, the setting is off.
inline bool IRTecoAc::getTimerEnabled(void) const {
  return _.TimerOn;
}

/// Get the timer time for when the A/C unit will switch power state.
/// @return The number of minutes left on the timer. `0` means off.
uint16_t IRTecoAc::getTimer(void) const {
  uint16_t mins = 0;
  if (getTimerEnabled()) {
    mins = (_.TensHours * 10 + _.UnitHours) * 60;
    if (_.HalfHour) mins += 30;
  }
  return mins;
}

/// Set the timer for when the A/C unit will switch power state.
/// @param[in] nr_mins Number of minutes before power state change.
///   `0` will clear the timer. Max is 24 hrs.
/// @note Time is stored internally in increments of 30 mins.
void IRTecoAc::setTimer(const uint16_t nr_mins) {
  uint16_t mins = std::min(nr_mins, (uint16_t)(24 * 60));  // Limit to 24 hrs.
  uint8_t hours = mins / 60;
  _.TimerOn = mins > 0;  // Set the timer flag.
  _.HalfHour = (mins % 60) >= 30;
  _.UnitHours = hours % 10;
  _.TensHours = hours / 10;
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRTecoAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kTecoCool;
    case stdAc::opmode_t::kHeat: return kTecoHeat;
    case stdAc::opmode_t::kDry:  return kTecoDry;
    case stdAc::opmode_t::kFan:  return kTecoFan;
    default:                     return kTecoAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRTecoAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kTecoFanLow;
    case stdAc::fanspeed_t::kMedium: return kTecoFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kTecoFanHigh;
    default:                         return kTecoFanAuto;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRTecoAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kTecoCool: return stdAc::opmode_t::kCool;
    case kTecoHeat: return stdAc::opmode_t::kHeat;
    case kTecoDry:  return stdAc::opmode_t::kDry;
    case kTecoFan:  return stdAc::opmode_t::kFan;
    default:        return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRTecoAc::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kTecoFanHigh: return stdAc::fanspeed_t::kMax;
    case kTecoFanMed:  return stdAc::fanspeed_t::kMedium;
    case kTecoFanLow:  return stdAc::fanspeed_t::kMin;
    default:           return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRTecoAc::toCommon(void) const {
  stdAc::state_t result{};
  result.protocol = decode_type_t::TECO;
  result.model = -1;  // Not supported.
  result.power = _.Power;
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.swingv = _.Swing ? stdAc::swingv_t::kAuto : stdAc::swingv_t::kOff;
  result.sleep = _.Sleep ? 0 : -1;
  result.light = _.Light;
  // Not supported.
  result.swingh = stdAc::swingh_t::kOff;
  result.turbo = false;
  result.filter = false;
  result.econo = false;
  result.quiet = false;
  result.clean = false;
  result.beep = false;
  result.clock = -1;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRTecoAc::toString(void) const {
  String result = "";
  result.reserve(100);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(_.Power, kPowerStr, false);
  result += addModeToString(_.Mode, kTecoAuto, kTecoCool, kTecoHeat,
                            kTecoDry, kTecoFan);
  result += addTempToString(getTemp());
  result += addFanToString(_.Fan, kTecoFanHigh, kTecoFanLow,
                           kTecoFanAuto, kTecoFanAuto, kTecoFanMed);
  result += addBoolToString(_.Sleep, kSleepStr);
  result += addBoolToString(_.Swing, kSwingStr);
  result += addBoolToString(_.Light, kLightStr);
  result += addBoolToString(_.Humid, kHumidStr);
  result += addBoolToString(_.Save, kSaveStr);
  if (getTimerEnabled())
    result += addLabeledString(irutils::minsToString(getTimer()),
                               kTimerStr);
  else
    result += addBoolToString(false, kTimerStr);
  return result;
}

#if DECODE_TECO
/// Decode the supplied Teco message.
/// Status: STABLE / Tested.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
bool IRrecv::decodeTeco(decode_results* results, uint16_t offset,
                        const uint16_t nbits, const bool strict) {
  if (strict && nbits != kTecoBits) return false;  // Not what is expected

  uint64_t data = 0;
  // Match Header + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, &data,
                    results->rawlen - offset, nbits,
                    kTecoHdrMark, kTecoHdrSpace,
                    kTecoBitMark, kTecoOneSpace,
                    kTecoBitMark, kTecoZeroSpace,
                    kTecoBitMark, kTecoGap, true,
                    _tolerance, kMarkExcess, false)) return false;

  // Success
  results->decode_type = TECO;
  results->bits = nbits;
  results->value = data;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif  // DECODE_TECO
