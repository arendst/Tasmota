// Copyright 2019, 2021 David Conran

/// @file
/// @brief Support for TCL protocols.

#include "ir_Tcl.h"
#include <algorithm>
#include <cstring>
#ifndef ARDUINO
#include <string>
#endif
#include "IRremoteESP8266.h"
#include "IRtext.h"
#include "IRutils.h"

// Constants

const uint8_t kTcl112AcTimerResolution = 20;  // Minutes
const uint16_t kTcl112AcTimerMax = 720;  // Minutes (12 hrs)

using irutils::addBoolToString;
using irutils::addFanToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addModelToString;
using irutils::addSwingVToString;
using irutils::addTempFloatToString;
using irutils::minsToString;

#if SEND_TCL112AC
/// Send a TCL 112-bit A/C message.
/// Status: Beta / Probably working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendTcl112Ac(const unsigned char data[], const uint16_t nbytes,
                          const uint16_t repeat) {
  sendGeneric(kTcl112AcHdrMark, kTcl112AcHdrSpace,
              kTcl112AcBitMark, kTcl112AcOneSpace,
              kTcl112AcBitMark, kTcl112AcZeroSpace,
              kTcl112AcBitMark, kTcl112AcGap,
              data, nbytes, 38000, false, repeat, 50);
}
#endif  // SEND_TCL112AC

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRTcl112Ac::IRTcl112Ac(const uint16_t pin, const bool inverted,
                       const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Set up hardware to be able to send a message.
void IRTcl112Ac::begin(void) { _irsend.begin(); }

#if SEND_TCL112AC
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRTcl112Ac::send(const uint16_t repeat) {
  uint8_t save[kTcl112AcStateLength];
  // Do we need to send the special "quiet" message?
  if (_quiet != _quiet_prev) {
    // Backup the current state.
    std::memcpy(save, _.raw, kTcl112AcStateLength);
    const uint8_t quiet_off[kTcl112AcStateLength] = {
        0x23, 0xCB, 0x26, 0x02, 0x00, 0x40, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65};
    // Use a known good quiet/mute off/type 2 state for the time being.
    // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1528#issuecomment-876989044
    setRaw(quiet_off);
    setQuiet(_quiet);
    // Send it.
    _irsend.sendTcl112Ac(getRaw(), kTcl112AcStateLength, repeat);
    // Now it's been sent, update the quiet previous state.
    _quiet_prev = _quiet;
    // Restore the old state.
    setRaw(save);
    // Make sure it looks like a normal TCL mesg if needed.
    if (_.MsgType == kTcl112AcNormal) _.isTcl = true;
  }
  // Send the normal (type 1) state.
  _irsend.sendTcl112Ac(getRaw(), kTcl112AcStateLength, repeat);
}
#endif  // SEND_TCL112AC

/// Calculate the checksum for a given state.
/// @param[in] state The array to calc the checksum of.
/// @param[in] length The length/size of the array.
/// @return The calculated checksum value.
uint8_t IRTcl112Ac::calcChecksum(uint8_t state[], const uint16_t length) {
  if (length) {
    if (length > 4 && state[3] == 0x02) {  // Special nessage?
      return sumBytes(state, length - 1, 0xF);  // Checksum needs an offset.
    } else {
      return sumBytes(state, length - 1);
    }
  } else {
    return 0;
  }
}

/// Calculate & set the checksum for the current internal state of the remote.
/// @param[in] length The length/size of the internal array to checksum.
void IRTcl112Ac::checksum(const uint16_t length) {
  // Stored the checksum value in the last byte.
  if (length > 1)
    _.Sum = calcChecksum(_.raw, length);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length/size of the array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRTcl112Ac::validChecksum(uint8_t state[], const uint16_t length) {
  return (length > 1 && state[length - 1] == calcChecksum(state, length));
}

/// Check the supplied state looks like a TCL112AC message.
/// @param[in] state The array to verify the checksum of.
/// @note Assumes the state is the correct size.
/// @return true, if the state looks like a TCL112AC message. Otherwise, false.
/// @warning This is just a guess.
bool IRTcl112Ac::isTcl(const uint8_t state[]) {
  Tcl112Protocol mesg;
  std::memcpy(mesg.raw, state, kTcl112AcStateLength);
  return (mesg.MsgType != kTcl112AcNormal) || mesg.isTcl;
}

/// Reset the internal state of the emulation. (On, Cool, 24C)
void IRTcl112Ac::stateReset(void) {
  // A known good state. (On, Cool, 24C)
  static const uint8_t reset[kTcl112AcStateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x24, 0x03, 0x07, 0x40, 0x00, 0x00, 0x00,
      0x00, 0x03};
  std::memcpy(_.raw, reset, kTcl112AcStateLength);
  _quiet = false;
  _quiet_prev = false;
  _quiet_explictly_set = false;
}

/// Get/Detect the model of the A/C.
/// @return The enum of the compatible model.
tcl_ac_remote_model_t IRTcl112Ac::getModel(void) const {
  return isTcl(_.raw) ? tcl_ac_remote_model_t::TAC09CHSD
                      : tcl_ac_remote_model_t::GZ055BE1;
}

/// Set the model of the A/C to emulate.
/// @param[in] model The enum of the appropriate model.
void IRTcl112Ac::setModel(const tcl_ac_remote_model_t model) {
  _.isTcl = (model != tcl_ac_remote_model_t::GZ055BE1);
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t* IRTcl112Ac::getRaw(void) {
  checksum();
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
/// @param[in] length The length/size of the new_code array.
void IRTcl112Ac::setRaw(const uint8_t new_code[], const uint16_t length) {
  std::memcpy(_.raw, new_code, std::min(length, kTcl112AcStateLength));
}

/// Set the requested power state of the A/C to on.
void IRTcl112Ac::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void IRTcl112Ac::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRTcl112Ac::setPower(const bool on) { _.Power = on; }

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRTcl112Ac::getPower(void) const { return _.Power; }

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRTcl112Ac::getMode(void) const { return _.Mode; }

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
/// @note Fan/Ventilation mode sets the fan speed to high.
///   Unknown values default to Auto.
void IRTcl112Ac::setMode(const uint8_t mode) {
  // If we get an unexpected mode, default to AUTO.
  switch (mode) {
    case kTcl112AcFan:
      setFan(kTcl112AcFanHigh);
      // FALLTHRU
    case kTcl112AcAuto:
    case kTcl112AcCool:
    case kTcl112AcHeat:
    case kTcl112AcDry:
      _.Mode = mode;
      break;
    default:
      _.Mode = kTcl112AcAuto;
  }
}

/// Set the temperature.
/// @param[in] celsius The temperature in degrees celsius.
/// @note The temperature resolution is 0.5 of a degree.
void IRTcl112Ac::setTemp(const float celsius) {
  // Make sure we have desired temp in the correct range.
  float safecelsius = std::max(celsius, kTcl112AcTempMin);
  safecelsius = std::min(safecelsius, kTcl112AcTempMax);
  // Convert to integer nr. of half degrees.
  uint8_t nrHalfDegrees = safecelsius * 2;
  // Do we have a half degree celsius?
  _.HalfDegree = nrHalfDegrees & 1;
  _.Temp = static_cast<uint8_t>(kTcl112AcTempMax - nrHalfDegrees / 2);
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
/// @note The temperature resolution is 0.5 of a degree.
float IRTcl112Ac::getTemp(void) const {
  float result = kTcl112AcTempMax - _.Temp;
  if (_.HalfDegree) result += 0.5;
  return result;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
/// @note Unknown speeds will default to Auto.
void IRTcl112Ac::setFan(const uint8_t speed) {
  switch (speed) {
    case kTcl112AcFanAuto:
    case kTcl112AcFanMin:
    case kTcl112AcFanLow:
    case kTcl112AcFanMed:
    case kTcl112AcFanHigh:
      _.Fan = speed;
      break;
    default:
      _.Fan = kTcl112AcFanAuto;
  }
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t IRTcl112Ac::getFan(void) const { return _.Fan; }

/// Set the economy setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRTcl112Ac::setEcono(const bool on) { _.Econo = on; }

/// Get the economy setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRTcl112Ac::getEcono(void) const { return  _.Econo; }

/// Set the Health (Filter) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRTcl112Ac::setHealth(const bool on) { _.Health = on; }

/// Get the Health (Filter) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRTcl112Ac::getHealth(void) const { return _.Health; }

/// Set the Light (LED/Display) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRTcl112Ac::setLight(const bool on) { _.Light = !on; }  // Cleared when on.

/// Get the Light (LED/Display) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRTcl112Ac::getLight(void) const { return !_.Light; }

/// Set the horizontal swing setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRTcl112Ac::setSwingHorizontal(const bool on) { _.SwingH = on; }

/// Get the horizontal swing setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRTcl112Ac::getSwingHorizontal(void) const { return _.SwingH; }

/// Set the vertical swing setting of the A/C.
/// @param[in] setting The value of the desired setting.
void IRTcl112Ac::setSwingVertical(const uint8_t setting) {
  switch (setting) {
    case kTcl112AcSwingVOff:
    case kTcl112AcSwingVHighest:
    case kTcl112AcSwingVHigh:
    case kTcl112AcSwingVMiddle:
    case kTcl112AcSwingVLow:
    case kTcl112AcSwingVLowest:
    case kTcl112AcSwingVOn:
     _.SwingV = setting;
  }
}

/// Get the vertical swing setting of the A/C.
/// @return The current setting.
uint8_t IRTcl112Ac::getSwingVertical(void) const { return _.SwingV; }

/// Set the Turbo setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRTcl112Ac::setTurbo(const bool on) {
  _.Turbo = on;
  if (on) {
    _.Fan = kTcl112AcFanHigh;
    _.SwingV = kTcl112AcSwingVOn;
  }
}

/// Get the Turbo setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRTcl112Ac::getTurbo(void) const { return _.Turbo; }

/// Set the Quiet setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRTcl112Ac::setQuiet(const bool on) {
  _quiet_explictly_set = true;
  _quiet = on;
  if (_.MsgType == kTcl112AcSpecial) _.Quiet = on;
}

/// Get the Quiet setting of the A/C.
/// @param[in] def The default value to use if we are not sure.
/// @return true, the setting is on. false, the setting is off.
bool IRTcl112Ac::getQuiet(const bool def) const {
  if (_.MsgType == kTcl112AcSpecial)
    return _.Quiet;
  else
    return _quiet_explictly_set ? _quiet : def;
}

/// Get how long the On Timer is set for, in minutes.
/// @return The time in nr of minutes.
uint16_t IRTcl112Ac::getOnTimer(void) const {
  return _.OnTimer * kTcl112AcTimerResolution;
}

/// Set or cancel the On Timer function.
/// @param[in] mins Nr. of minutes the timer is to be set to.
/// @note Rounds down to 20 min increments. (max: 720 mins (12h), 0 is Off)
void IRTcl112Ac::setOnTimer(const uint16_t mins) {
  _.OnTimer = std::min(mins, kTcl112AcTimerMax) / kTcl112AcTimerResolution;
  _.OnTimerEnabled = _.OnTimer > 0;
  _.TimerIndicator = _.OnTimerEnabled || _.OffTimerEnabled;
}

/// Get how long the Off Timer is set for, in minutes.
/// @return The time in nr of minutes.
uint16_t IRTcl112Ac::getOffTimer(void) const {
  return _.OffTimer * kTcl112AcTimerResolution;
}

/// Set or cancel the Off Timer function.
/// @param[in] mins Nr. of minutes the timer is to be set to.
/// @note Rounds down to 20 min increments. (max: 720 mins (12h), 0 is Off)
void IRTcl112Ac::setOffTimer(const uint16_t mins) {
  _.OffTimer = std::min(mins, kTcl112AcTimerMax) / kTcl112AcTimerResolution;
  _.OffTimerEnabled = _.OffTimer > 0;
  _.TimerIndicator = _.OnTimerEnabled || _.OffTimerEnabled;
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRTcl112Ac::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kTcl112AcCool;
    case stdAc::opmode_t::kHeat: return kTcl112AcHeat;
    case stdAc::opmode_t::kDry:  return kTcl112AcDry;
    case stdAc::opmode_t::kFan:  return kTcl112AcFan;
    default:                     return kTcl112AcAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRTcl112Ac::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:    return kTcl112AcFanMin;
    case stdAc::fanspeed_t::kLow:    return kTcl112AcFanLow;
    case stdAc::fanspeed_t::kMedium: return kTcl112AcFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kTcl112AcFanHigh;
    default:                         return kTcl112AcFanAuto;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRTcl112Ac::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kTcl112AcCool: return stdAc::opmode_t::kCool;
    case kTcl112AcHeat: return stdAc::opmode_t::kHeat;
    case kTcl112AcDry:  return stdAc::opmode_t::kDry;
    case kTcl112AcFan:  return stdAc::opmode_t::kFan;
    default:            return stdAc::opmode_t::kAuto;
  }
}

/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRTcl112Ac::convertSwingV(const stdAc::swingv_t position) {
  switch (position) {
    case stdAc::swingv_t::kOff:     return kTcl112AcSwingVOff;
    case stdAc::swingv_t::kHighest: return kTcl112AcSwingVHighest;
    case stdAc::swingv_t::kHigh:    return kTcl112AcSwingVHigh;
    case stdAc::swingv_t::kMiddle:  return kTcl112AcSwingVMiddle;
    case stdAc::swingv_t::kLow:     return kTcl112AcSwingVLow;
    case stdAc::swingv_t::kLowest:  return kTcl112AcSwingVLowest;
    default:                        return kTcl112AcSwingVOn;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] spd The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRTcl112Ac::toCommonFanSpeed(const uint8_t spd) {
  switch (spd) {
    case kTcl112AcFanHigh: return stdAc::fanspeed_t::kMax;
    case kTcl112AcFanMed:  return stdAc::fanspeed_t::kMedium;
    case kTcl112AcFanLow:  return stdAc::fanspeed_t::kLow;
    case kTcl112AcFanMin:  return stdAc::fanspeed_t::kMin;
    default:               return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert a native vertical swing postion to it's common equivalent.
/// @param[in] setting A native position to convert.
/// @return The common vertical swing position.
stdAc::swingv_t IRTcl112Ac::toCommonSwingV(const uint8_t setting) {
  switch (setting) {
    case kTcl112AcSwingVOff:       return stdAc::swingv_t::kOff;
    default:                       return stdAc::swingv_t::kAuto;
  }
}
/// Convert the current internal state into its stdAc::state_t equivalent.
/// @param[in] prev Ptr to the previous state if required.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRTcl112Ac::toCommon(const stdAc::state_t *prev) const {
  stdAc::state_t result;
  // Start with the previous state if given it.
  if (prev != NULL) result = *prev;
  result.protocol = decode_type_t::TCL112AC;
  result.model = getModel();
  result.quiet = getQuiet(result.quiet);
  // The rest only get updated if it is a "normal" message.
  if (_.MsgType == kTcl112AcNormal) {
    result.power = _.Power;
    result.mode = toCommonMode(_.Mode);
    result.celsius = true;
    result.degrees = getTemp();
    result.fanspeed = toCommonFanSpeed(_.Fan);
    result.swingv = toCommonSwingV(_.SwingV);
    result.swingh = _.SwingH ? stdAc::swingh_t::kAuto : stdAc::swingh_t::kOff;
    result.turbo = _.Turbo;
    result.filter = _.Health;
    result.econo = _.Econo;
    result.light = getLight();
  }
  // Not supported.
  result.clean = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRTcl112Ac::toString(void) const {
  String result = "";
  result.reserve(220);  // Reserve some heap for the string to reduce fragging.
  tcl_ac_remote_model_t model = getModel();
  result += addModelToString(decode_type_t::TCL112AC, model, false);
  result += addIntToString(_.MsgType, D_STR_TYPE);
  switch (_.MsgType) {
    case kTcl112AcNormal:
      result += addBoolToString(_.Power, kPowerStr);
      result += addModeToString(_.Mode, kTcl112AcAuto, kTcl112AcCool,
                                kTcl112AcHeat, kTcl112AcDry, kTcl112AcFan);
      result += addTempFloatToString(getTemp());
      result += addFanToString(_.Fan, kTcl112AcFanHigh, kTcl112AcFanLow,
                               kTcl112AcFanAuto, kTcl112AcFanMin,
                               kTcl112AcFanMed);
      result += addSwingVToString(_.SwingV, kTcl112AcSwingVOff,
                                            kTcl112AcSwingVHighest,
                                            kTcl112AcSwingVHigh,
                                            0xFF,  // unused
                                            kTcl112AcSwingVMiddle,
                                            0xFF,  // unused
                                            kTcl112AcSwingVLow,
                                            kTcl112AcSwingVLowest,
                                            kTcl112AcSwingVOff,
                                            kTcl112AcSwingVOn,  // Swing
                                            0xFF, 0xFF);  // Both Unused
      if (model != tcl_ac_remote_model_t::GZ055BE1) {
        result += addBoolToString(_.SwingH, kSwingHStr);
        result += addBoolToString(_.Econo, kEconoStr);
        result += addBoolToString(_.Health, kHealthStr);
        result += addBoolToString(_.Turbo, kTurboStr);
        result += addBoolToString(getLight(), kLightStr);
      }
      result += addLabeledString(
          _.OnTimerEnabled ? minsToString(getOnTimer()) : kOffStr,
          kOnTimerStr);
      result += addLabeledString(
          _.OffTimerEnabled ? minsToString(getOffTimer()) : kOffStr,
          kOffTimerStr);
      break;
    case kTcl112AcSpecial:
      result += addBoolToString(_.Quiet, kQuietStr);
      break;
  }
  return result;
}

#if DECODE_TCL112AC
/// @file
/// @note There is no `decodedecodeTcl112Ac()`.
///   It's the same as `decodeMitsubishi112()`. A shared routine is used.
///   You can find it in: ir_Mitsubishi.cpp
#endif  // DECODE_TCL112AC
