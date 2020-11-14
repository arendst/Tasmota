// Copyright 2018 crankyoldgit
/// @file
/// @brief Support for Haier A/C protocols.
/// The specifics of reverse engineering the protocols details:
/// * HSU07-HEA03 by kuzin2006.
/// * YR-W02/HSU-09HMC203 by non7top.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/404
/// @see https://www.dropbox.com/s/mecyib3lhdxc8c6/IR%20data%20reverse%20engineering.xlsx?dl=0
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/485
/// @see https://www.dropbox.com/sh/w0bt7egp0fjger5/AADRFV6Wg4wZskJVdFvzb8Z0a?dl=0&preview=haer2.ods

#include "ir_Haier.h"
#include <cstring>
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include "IRremoteESP8266.h"
#include "IRtext.h"
#include "IRutils.h"

// Constants
const uint16_t kHaierAcHdr = 3000;
const uint16_t kHaierAcHdrGap = 4300;
const uint16_t kHaierAcBitMark = 520;
const uint16_t kHaierAcOneSpace = 1650;
const uint16_t kHaierAcZeroSpace = 650;
const uint32_t kHaierAcMinGap = 150000;  // Completely made up value.

using irutils::addBoolToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addFanToString;
using irutils::addTempToString;
using irutils::minsToString;

#define GETTIME(x) _.x##Hours * 60 + _.x##Mins
#define SETTIME(x, n) do { \
  uint16_t mins = n;\
  if (n > kHaierAcMaxTime) mins = kHaierAcMaxTime;\
  _.x##Hours = mins / 60;\
  _.x##Mins = mins % 60;\
} while (0)

#if (SEND_HAIER_AC || SEND_HAIER_AC_YRW02)
/// Send a Haier A/C formatted message. (HSU07-HEA03 remote)
/// Status: STABLE / Known to be working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendHaierAC(const unsigned char data[], const uint16_t nbytes,
                         const uint16_t repeat) {
  if (nbytes < kHaierACStateLength) return;

  for (uint16_t r = 0; r <= repeat; r++) {
    enableIROut(38000);
    mark(kHaierAcHdr);
    space(kHaierAcHdr);
    sendGeneric(kHaierAcHdr, kHaierAcHdrGap, kHaierAcBitMark, kHaierAcOneSpace,
                kHaierAcBitMark, kHaierAcZeroSpace, kHaierAcBitMark,
                kHaierAcMinGap, data, nbytes, 38, true,
                0,  // Repeats handled elsewhere
                50);
  }
}
#endif  // (SEND_HAIER_AC || SEND_HAIER_AC_YRW02)

#if SEND_HAIER_AC_YRW02
/// Send a Haier YR-W02 remote A/C formatted message.
/// Status: Alpha / Untested on a real device.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendHaierACYRW02(const unsigned char data[], const uint16_t nbytes,
                              const uint16_t repeat) {
  if (nbytes >= kHaierACYRW02StateLength) sendHaierAC(data, nbytes, repeat);
}
#endif  // SEND_HAIER_AC_YRW02

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRHaierAC::IRHaierAC(const uint16_t pin, const bool inverted,
                     const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Set up hardware to be able to send a message.
void IRHaierAC::begin(void) { _irsend.begin(); }

#if SEND_HAIER_AC
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRHaierAC::send(const uint16_t repeat) {
  _irsend.sendHaierAC(getRaw(), kHaierACStateLength, repeat);
}
#endif  // SEND_HAIER_AC

/// Calculate and set the checksum values for the internal state.
void IRHaierAC::checksum(void) {
  _.Sum = sumBytes(_.remote_state, kHaierACStateLength - 1);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length of the state array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRHaierAC::validChecksum(uint8_t state[], const uint16_t length) {
  if (length < 2) return false;  // 1 byte of data can't have a checksum.
  return (state[length - 1] == sumBytes(state, length - 1));
}

/// Reset the internal state to a fixed known good state.
void IRHaierAC::stateReset(void) {
  std::memset(_.remote_state, 0, sizeof _.remote_state);
  _.Prefix = kHaierAcPrefix;
  _.unknown = 1;  // const value
  _.OffHours = 12;  // default initial state
  _.Temp = kHaierAcDefTemp - kHaierAcMinTemp;
  _.Fan = 3;  // kHaierAcFanLow;
  _.Command = kHaierAcCmdOn;
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t* IRHaierAC::getRaw(void) {
  checksum();
  return _.remote_state;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
void IRHaierAC::setRaw(const uint8_t new_code[]) {
  std::memcpy(_.remote_state, new_code, kHaierACStateLength);
}

/// Set the Command/Button setting of the A/C.
/// @param[in] command The value of the command/button that was pressed.
void IRHaierAC::setCommand(const uint8_t command) {
  switch (command) {
    case kHaierAcCmdOff:
    case kHaierAcCmdOn:
    case kHaierAcCmdMode:
    case kHaierAcCmdFan:
    case kHaierAcCmdTempUp:
    case kHaierAcCmdTempDown:
    case kHaierAcCmdSleep:
    case kHaierAcCmdTimerSet:
    case kHaierAcCmdTimerCancel:
    case kHaierAcCmdHealth:
    case kHaierAcCmdSwing:
      _.Command = command;
  }
}

/// Get the Command/Button setting of the A/C.
/// @return The value of the command/button that was pressed.
uint8_t IRHaierAC::getCommand(void) const {
  return _.Command;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRHaierAC::setFan(const uint8_t speed) {
  uint8_t new_speed = kHaierAcFanAuto;
  switch (speed) {
    case kHaierAcFanLow:  new_speed = 3; break;
    case kHaierAcFanMed:  new_speed = 2; break;
    case kHaierAcFanHigh: new_speed = 1; break;
    // Default to auto for anything else.
    default:              new_speed = kHaierAcFanAuto;
  }

  if (speed != getFan()) _.Command = kHaierAcCmdFan;
  _.Fan = new_speed;
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRHaierAC::getFan(void) const {
  switch (_.Fan) {
    case 1:  return kHaierAcFanHigh;
    case 2:  return kHaierAcFanMed;
    case 3:  return kHaierAcFanLow;
    default: return kHaierAcFanAuto;
  }
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRHaierAC::setMode(const uint8_t mode) {
  uint8_t new_mode = mode;
  _.Command = kHaierAcCmdMode;
  // If out of range, default to auto mode.
  if (mode > kHaierAcFan) new_mode = kHaierAcAuto;
  _.Mode = new_mode;
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRHaierAC::getMode(void) const {
  return _.Mode;
}

/// Set the temperature.
/// @param[in] degrees The temperature in degrees celsius.
void IRHaierAC::setTemp(const uint8_t degrees) {
  uint8_t temp = degrees;
  if (temp < kHaierAcMinTemp)
    temp = kHaierAcMinTemp;
  else if (temp > kHaierAcMaxTemp)
    temp = kHaierAcMaxTemp;

  uint8_t old_temp = getTemp();
  if (old_temp == temp) return;
  if (old_temp > temp)
    _.Command = kHaierAcCmdTempDown;
  else
    _.Command = kHaierAcCmdTempUp;
  _.Temp = temp - kHaierAcMinTemp;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRHaierAC::getTemp(void) const {
  return _.Temp + kHaierAcMinTemp;
}

/// Set the Health (filter) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHaierAC::setHealth(const bool on) {
  _.Command = kHaierAcCmdHealth;
  _.Health = on;
}

/// Get the Health (filter) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHaierAC::getHealth(void) const {
  return _.Health;
}

/// Set the Sleep setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHaierAC::setSleep(const bool on) {
  _.Command = kHaierAcCmdSleep;
  _.Sleep = on;
}

/// Get the Sleep setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHaierAC::getSleep(void) const {
  return _.Sleep;
}

/// Get the On Timer value/setting of the A/C.
/// @return Nr of minutes the timer is set to. -1 is Off/not set etc.
int16_t IRHaierAC::getOnTimer(void) const {
  // Check if the timer is turned on.
  if (_.OnTimer)
    return GETTIME(On);
  else
    return -1;
}

/// Get the Off Timer value/setting of the A/C.
/// @return Nr of minutes the timer is set to. -1 is Off/not set etc.
int16_t IRHaierAC::getOffTimer(void) const {
  // Check if the timer is turned on.
  if (_.OffTimer)
    return GETTIME(Off);
  else
    return -1;
}

/// Get the clock value of the A/C.
/// @return The clock time, in Nr of minutes past midnight.
uint16_t IRHaierAC::getCurrTime(void) const { return GETTIME(Curr); }

/// Set & enable the On Timer.
/// @param[in] nr_mins The time expressed in total number of minutes.
void IRHaierAC::setOnTimer(const uint16_t nr_mins) {
  _.Command = kHaierAcCmdTimerSet;
  _.OnTimer = 1;

  SETTIME(On, nr_mins);
}

/// Set & enable the Off Timer.
/// @param[in] nr_mins The time expressed in total number of minutes.
void IRHaierAC::setOffTimer(const uint16_t nr_mins) {
  _.Command = kHaierAcCmdTimerSet;
  _.OffTimer = 1;

  SETTIME(Off, nr_mins);
}

/// Cancel/disable the On & Off timers.
void IRHaierAC::cancelTimers(void) {
  _.Command = kHaierAcCmdTimerCancel;
  _.OffTimer = 0;
  _.OnTimer = 0;
}

/// Set the clock value for the A/C.
/// @param[in] nr_mins The clock time, in Nr of minutes past midnight.
void IRHaierAC::setCurrTime(const uint16_t nr_mins) {
  SETTIME(Curr, nr_mins);
}

/// Get the Vertical Swing position setting of the A/C.
/// @return The native swing mode.
uint8_t IRHaierAC::getSwing(void) const {
  return _.Swing;
}

/// Set the Vertical Swing mode of the A/C.
/// @param[in] state The mode to set the vanes to.
void IRHaierAC::setSwing(const uint8_t state) {
  if (state == _.Swing) return;  // Nothing to do.
  switch (state) {
    case kHaierAcSwingOff:
    case kHaierAcSwingUp:
    case kHaierAcSwingDown:
    case kHaierAcSwingChg:
      _.Command = kHaierAcCmdSwing;
      _.Swing = state;
      break;
  }
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRHaierAC::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kHaierAcCool;
    case stdAc::opmode_t::kHeat: return kHaierAcHeat;
    case stdAc::opmode_t::kDry:  return kHaierAcDry;
    case stdAc::opmode_t::kFan:  return kHaierAcFan;
    default:                     return kHaierAcAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRHaierAC::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kHaierAcFanLow;
    case stdAc::fanspeed_t::kMedium: return kHaierAcFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kHaierAcFanHigh;
    default:                         return kHaierAcFanAuto;
  }
}

/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRHaierAC::convertSwingV(const stdAc::swingv_t position) {
  switch (position) {
    case stdAc::swingv_t::kHighest:
    case stdAc::swingv_t::kHigh:
    case stdAc::swingv_t::kMiddle:  return kHaierAcSwingUp;
    case stdAc::swingv_t::kLow:
    case stdAc::swingv_t::kLowest:  return kHaierAcSwingDown;
    case stdAc::swingv_t::kOff:     return kHaierAcSwingOff;
    default:                        return kHaierAcSwingChg;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRHaierAC::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kHaierAcCool: return stdAc::opmode_t::kCool;
    case kHaierAcHeat: return stdAc::opmode_t::kHeat;
    case kHaierAcDry:  return stdAc::opmode_t::kDry;
    case kHaierAcFan:  return stdAc::opmode_t::kFan;
    default:           return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRHaierAC::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kHaierAcFanHigh: return stdAc::fanspeed_t::kMax;
    case kHaierAcFanMed:  return stdAc::fanspeed_t::kMedium;
    case kHaierAcFanLow:  return stdAc::fanspeed_t::kMin;
    default:              return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] pos The enum to be converted.
/// @return The native equivalent of the enum.
stdAc::swingv_t IRHaierAC::toCommonSwingV(const uint8_t pos) {
  switch (pos) {
    case kHaierAcSwingUp:   return stdAc::swingv_t::kHighest;
    case kHaierAcSwingDown: return stdAc::swingv_t::kLowest;
    case kHaierAcSwingOff:  return stdAc::swingv_t::kOff;
    default:                return stdAc::swingv_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRHaierAC::toCommon(void) const {
  stdAc::state_t result;
  result.protocol = decode_type_t::HAIER_AC;
  result.model = -1;  // No models used.
  result.power = true;
  if (_.Command == kHaierAcCmdOff) result.power = false;
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(getFan());
  result.swingv = toCommonSwingV(_.Swing);
  result.filter = _.Health;
  result.sleep = _.Sleep ? 0 : -1;
  // Not supported.
  result.swingh = stdAc::swingh_t::kOff;
  result.quiet = false;
  result.turbo = false;
  result.econo = false;
  result.light = false;
  result.clean = false;
  result.beep = false;
  result.clock = -1;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRHaierAC::toString(void) const {
  String result = "";
  result.reserve(150);  // Reserve some heap for the string to reduce fragging.
  uint8_t cmd = _.Command;
  result += addIntToString(cmd, kCommandStr, false);
  result += kSpaceLBraceStr;
  switch (cmd) {
    case kHaierAcCmdOff:
      result += kOffStr;
      break;
    case kHaierAcCmdOn:
      result += kOnStr;
      break;
    case kHaierAcCmdMode:
      result += kModeStr;
      break;
    case kHaierAcCmdFan:
      result += kFanStr;
      break;
    case kHaierAcCmdTempUp:
      result += kTempUpStr;
      break;
    case kHaierAcCmdTempDown:
      result += kTempDownStr;
      break;
    case kHaierAcCmdSleep:
      result += kSleepStr;
      break;
    case kHaierAcCmdTimerSet:
      result += kTimerStr;
      result += ' ';
      result += kSetStr;
      break;
    case kHaierAcCmdTimerCancel:
      result += kTimerStr;
      result += ' ';
      result += kCancelStr;
      break;
    case kHaierAcCmdHealth:
      result += kHealthStr;
      break;
    case kHaierAcCmdSwing:
      result += kSwingStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addModeToString(_.Mode, kHaierAcAuto, kHaierAcCool, kHaierAcHeat,
                            kHaierAcDry, kHaierAcFan);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kHaierAcFanHigh, kHaierAcFanLow,
                           kHaierAcFanAuto, kHaierAcFanAuto, kHaierAcFanMed);
  result += addIntToString(_.Swing, kSwingStr);
  result += kSpaceLBraceStr;
  switch (_.Swing) {
    case kHaierAcSwingOff:
      result += kOffStr;
      break;
    case kHaierAcSwingUp:
      result += kUpStr;
      break;
    case kHaierAcSwingDown:
      result += kDownStr;
      break;
    case kHaierAcSwingChg:
      result += kChangeStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addBoolToString(_.Sleep, kSleepStr);
  result += addBoolToString(_.Health, kHealthStr);
  result += addLabeledString(minsToString(getCurrTime()), kClockStr);
  result += addLabeledString(
      getOnTimer() >= 0 ? minsToString(getOnTimer()) : kOffStr, kOnTimerStr);
  result += addLabeledString(
      getOffTimer() >= 0 ? minsToString(getOffTimer()) : kOffStr,
      kOffTimerStr);
  return result;
}
// End of IRHaierAC class.

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRHaierACYRW02::IRHaierACYRW02(const uint16_t pin, const bool inverted,
                               const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Set up hardware to be able to send a message.
void IRHaierACYRW02::begin(void) { _irsend.begin(); }

#if SEND_HAIER_AC_YRW02
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRHaierACYRW02::send(const uint16_t repeat) {
  _irsend.sendHaierACYRW02(getRaw(), kHaierACYRW02StateLength, repeat);
}
#endif  // SEND_HAIER_AC_YRW02

/// Calculate and set the checksum values for the internal state.
void IRHaierACYRW02::checksum(void) {
  _.Sum = sumBytes(_.raw, kHaierACYRW02StateLength - 1);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length of the state array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRHaierACYRW02::validChecksum(uint8_t state[], const uint16_t length) {
  if (length < 2) return false;  // 1 byte of data can't have a checksum.
  return (state[length - 1] == sumBytes(state, length - 1));
}

/// Reset the internal state to a fixed known good state.
void IRHaierACYRW02::stateReset(void) {
  std::memset(_.raw, 0, sizeof _.raw);

  _.Prefix = kHaierAcYrw02Prefix;
  _.Temp = kHaierAcDefTemp - kHaierAcMinTemp;
  _.Health = true;
  _.Fan = kHaierAcYrw02FanAuto;
  _.Power = true;
  _.Button = kHaierAcYrw02ButtonPower;
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t* IRHaierACYRW02::getRaw(void) {
  checksum();
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
void IRHaierACYRW02::setRaw(const uint8_t new_code[]) {
  memcpy(_.raw, new_code, kHaierACYRW02StateLength);
}

/// Set the Button/Command setting of the A/C.
/// @param[in] button The value of the button/command that was pressed.
void IRHaierACYRW02::setButton(uint8_t button) {
  switch (button) {
    case kHaierAcYrw02ButtonTempUp:
    case kHaierAcYrw02ButtonTempDown:
    case kHaierAcYrw02ButtonSwing:
    case kHaierAcYrw02ButtonFan:
    case kHaierAcYrw02ButtonPower:
    case kHaierAcYrw02ButtonMode:
    case kHaierAcYrw02ButtonHealth:
    case kHaierAcYrw02ButtonTurbo:
    case kHaierAcYrw02ButtonSleep:
      _.Button = button;
  }
}

/// Get the Button/Command setting of the A/C.
/// @return The value of the button/command that was pressed.
uint8_t IRHaierACYRW02::getButton(void) const {
  return _.Button;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRHaierACYRW02::setMode(uint8_t mode) {
  uint8_t new_mode = mode;
  _.Button = kHaierAcYrw02ButtonMode;
  switch (mode) {
    case kHaierAcYrw02Auto:
    case kHaierAcYrw02Cool:
    case kHaierAcYrw02Dry:
    case kHaierAcYrw02Heat:
    case kHaierAcYrw02Fan: break;
    default: new_mode = kHaierAcYrw02Auto;  // Unexpected, default to auto mode.
  }
  _.Mode = new_mode;
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRHaierACYRW02::getMode(void) const {
  return _.Mode;
}

/// Set the temperature.
/// @param[in] celsius The temperature in degrees celsius.
void IRHaierACYRW02::setTemp(const uint8_t celsius) {
  uint8_t temp = celsius;
  if (temp < kHaierAcMinTemp)
    temp = kHaierAcMinTemp;
  else if (temp > kHaierAcMaxTemp)
    temp = kHaierAcMaxTemp;

  uint8_t old_temp = getTemp();
  if (old_temp == temp) return;
  if (old_temp > temp)
    _.Button = kHaierAcYrw02ButtonTempDown;
  else
    _.Button = kHaierAcYrw02ButtonTempUp;
  _.Temp = temp - kHaierAcMinTemp;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRHaierACYRW02::getTemp(void) const {
  return _.Temp + kHaierAcMinTemp;
}

/// Set the Health (filter) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHaierACYRW02::setHealth(const bool on) {
  _.Button = kHaierAcYrw02ButtonHealth;
  _.Health = on;
}

/// Get the Health (filter) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHaierACYRW02::getHealth(void) const {
  return _.Health;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRHaierACYRW02::getPower(void) const {
  return _.Power;
}

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHaierACYRW02::setPower(const bool on) {
  _.Button = kHaierAcYrw02ButtonPower;
  _.Power = on;
}

/// Change the power setting to On.
void IRHaierACYRW02::on(void) { setPower(true); }

/// Change the power setting to Off.
void IRHaierACYRW02::off(void) { setPower(false); }

/// Get the Sleep setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHaierACYRW02::getSleep(void) const {
  return _.Sleep;
}

/// Set the Sleep setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHaierACYRW02::setSleep(const bool on) {
  _.Button = kHaierAcYrw02ButtonSleep;
  _.Sleep = on;
}

/// Get the Turbo setting of the A/C.
/// @return The current turbo speed setting.
uint8_t IRHaierACYRW02::getTurbo(void) const {
  return _.Turbo;
}

/// Set the Turbo setting of the A/C.
/// @param[in] speed The desired turbo speed setting.
/// @note Valid speeds are kHaierAcYrw02TurboOff, kHaierAcYrw02TurboLow, &
///   kHaierAcYrw02TurboHigh.
void IRHaierACYRW02::setTurbo(uint8_t speed) {
  switch (speed) {
    case kHaierAcYrw02TurboOff:
    case kHaierAcYrw02TurboLow:
    case kHaierAcYrw02TurboHigh:
      _.Turbo = speed;
      _.Button = kHaierAcYrw02ButtonTurbo;
  }
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRHaierACYRW02::getFan(void) const {
  return _.Fan;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRHaierACYRW02::setFan(uint8_t speed) {
  switch (speed) {
    case kHaierAcYrw02FanLow:
    case kHaierAcYrw02FanMed:
    case kHaierAcYrw02FanHigh:
    case kHaierAcYrw02FanAuto:
      _.Fan = speed;
      _.Button = kHaierAcYrw02ButtonFan;
  }
}

/// Get the Vertical Swing position setting of the A/C.
/// @return The native position/mode.
uint8_t IRHaierACYRW02::getSwing(void) const {
  return _.Swing;
}

/// Set the Vertical Swing mode of the A/C.
/// @param[in] pos The position/mode to set the vanes to.
void IRHaierACYRW02::setSwing(uint8_t pos) {
  uint8_t newpos = pos;
  switch (pos) {
    case kHaierAcYrw02SwingOff:
    case kHaierAcYrw02SwingAuto:
    case kHaierAcYrw02SwingTop:
    case kHaierAcYrw02SwingMiddle:
    case kHaierAcYrw02SwingBottom:
    case kHaierAcYrw02SwingDown: _.Button = kHaierAcYrw02ButtonSwing; break;
    default: return;  // Unexpected value so don't do anything.
  }
  // Heat mode has no MIDDLE setting, use BOTTOM instead.
  if (pos == kHaierAcYrw02SwingMiddle && _.Mode == kHaierAcYrw02Heat)
    newpos = kHaierAcYrw02SwingBottom;
  // BOTTOM is only allowed if we are in Heat mode, otherwise MIDDLE.
  if (pos == kHaierAcYrw02SwingBottom && _.Mode != kHaierAcYrw02Heat)
    newpos = kHaierAcYrw02SwingMiddle;
  _.Swing = newpos;
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRHaierACYRW02::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kHaierAcYrw02Cool;
    case stdAc::opmode_t::kHeat: return kHaierAcYrw02Heat;
    case stdAc::opmode_t::kDry:  return kHaierAcYrw02Dry;
    case stdAc::opmode_t::kFan:  return kHaierAcYrw02Fan;
    default:                     return kHaierAcYrw02Auto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRHaierACYRW02::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kHaierAcYrw02FanLow;
    case stdAc::fanspeed_t::kMedium: return kHaierAcYrw02FanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kHaierAcYrw02FanHigh;
    default:                         return kHaierAcYrw02FanAuto;
  }
}

/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRHaierACYRW02::convertSwingV(const stdAc::swingv_t position) {
  switch (position) {
    case stdAc::swingv_t::kHighest:
    case stdAc::swingv_t::kHigh:   return kHaierAcYrw02SwingTop;
    case stdAc::swingv_t::kMiddle: return kHaierAcYrw02SwingMiddle;
    case stdAc::swingv_t::kLow:    return kHaierAcYrw02SwingDown;
    case stdAc::swingv_t::kLowest: return kHaierAcYrw02SwingBottom;
    case stdAc::swingv_t::kOff:    return kHaierAcYrw02SwingOff;
    default:                       return kHaierAcYrw02SwingAuto;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRHaierACYRW02::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kHaierAcYrw02Cool: return stdAc::opmode_t::kCool;
    case kHaierAcYrw02Heat: return stdAc::opmode_t::kHeat;
    case kHaierAcYrw02Dry:  return stdAc::opmode_t::kDry;
    case kHaierAcYrw02Fan:  return stdAc::opmode_t::kFan;
    default:                return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRHaierACYRW02::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kHaierAcYrw02FanHigh: return stdAc::fanspeed_t::kMax;
    case kHaierAcYrw02FanMed:  return stdAc::fanspeed_t::kMedium;
    case kHaierAcYrw02FanLow:  return stdAc::fanspeed_t::kMin;
    default:                   return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] pos The enum to be converted.
/// @return The native equivalent of the enum.
stdAc::swingv_t IRHaierACYRW02::toCommonSwingV(const uint8_t pos) {
  switch (pos) {
    case kHaierAcYrw02SwingTop:    return stdAc::swingv_t::kHighest;
    case kHaierAcYrw02SwingMiddle: return stdAc::swingv_t::kMiddle;
    case kHaierAcYrw02SwingDown:   return stdAc::swingv_t::kLow;
    case kHaierAcYrw02SwingBottom: return stdAc::swingv_t::kLowest;
    case kHaierAcYrw02SwingOff:    return stdAc::swingv_t::kOff;
    default:                       return stdAc::swingv_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRHaierACYRW02::toCommon(void) const {
  stdAc::state_t result;
  result.protocol = decode_type_t::HAIER_AC_YRW02;
  result.model = -1;  // No models used.
  result.power = _.Power;
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.swingv = toCommonSwingV(_.Swing);
  result.filter = _.Health;
  result.sleep = _.Sleep ? 0 : -1;
  // Not supported.
  result.swingh = stdAc::swingh_t::kOff;
  result.quiet = false;
  result.turbo = false;
  result.econo = false;
  result.light = false;
  result.clean = false;
  result.beep = false;
  result.clock = -1;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRHaierACYRW02::toString(void) const {
  String result = "";
  result.reserve(130);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(_.Power, kPowerStr, false);
  uint8_t cmd = _.Button;
  result += addIntToString(cmd, kButtonStr);
  result += kSpaceLBraceStr;
  switch (cmd) {
    case kHaierAcYrw02ButtonPower:
      result += kPowerStr;
      break;
    case kHaierAcYrw02ButtonMode:
      result += kModeStr;
      break;
    case kHaierAcYrw02ButtonFan:
      result += kFanStr;
      break;
    case kHaierAcYrw02ButtonTempUp:
      result += kTempUpStr;
      break;
    case kHaierAcYrw02ButtonTempDown:
      result += kTempDownStr;
      break;
    case kHaierAcYrw02ButtonSleep:
      result += kSleepStr;
      break;
    case kHaierAcYrw02ButtonHealth:
      result += kHealthStr;
      break;
    case kHaierAcYrw02ButtonSwing:
      result += kSwingStr;
      break;
    case kHaierAcYrw02ButtonTurbo:
      result += kTurboStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addModeToString(_.Mode, kHaierAcYrw02Auto, kHaierAcYrw02Cool,
                            kHaierAcYrw02Heat, kHaierAcYrw02Dry,
                            kHaierAcYrw02Fan);
  result += addTempToString(getTemp());
  result += addFanToString(_.Fan, kHaierAcYrw02FanHigh, kHaierAcYrw02FanLow,
                           kHaierAcYrw02FanAuto, kHaierAcYrw02FanAuto,
                           kHaierAcYrw02FanMed);
  result += addIntToString(_.Turbo, kTurboStr);
  result += kSpaceLBraceStr;
  switch (_.Turbo) {
    case kHaierAcYrw02TurboOff:
      result += kOffStr;
      break;
    case kHaierAcYrw02TurboLow:
      result += kLowStr;
      break;
    case kHaierAcYrw02TurboHigh:
      result += kHighStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addIntToString(_.Swing, kSwingStr);
  result += kSpaceLBraceStr;
  switch (_.Swing) {
    case kHaierAcYrw02SwingOff:
      result += kOffStr;
      break;
    case kHaierAcYrw02SwingAuto:
      result += kAutoStr;
      break;
    case kHaierAcYrw02SwingBottom:
      result += kLowestStr;
      break;
    case kHaierAcYrw02SwingDown:
      result += kLowStr;
      break;
    case kHaierAcYrw02SwingTop:
      result += kHighestStr;
      break;
    case kHaierAcYrw02SwingMiddle:
      result += kMiddleStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addBoolToString(_.Sleep, kSleepStr);
  result += addBoolToString(_.Health, kHealthStr);
  return result;
}
// End of IRHaierACYRW02 class.

#if (DECODE_HAIER_AC || DECODE_HAIER_AC_YRW02)
/// Decode the supplied Haier HSU07-HEA03 remote message.
/// Status: STABLE / Known to be working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeHaierAC(decode_results* results, uint16_t offset,
                           const uint16_t nbits, const bool strict) {
  if (strict) {
    if (nbits != kHaierACBits)
      return false;  // Not strictly a HAIER_AC message.
  }

  if (results->rawlen <= (2 * nbits + kHeader) + kFooter - 1 + offset)
    return false;  // Can't possibly be a valid HAIER_AC message.

  // Pre-Header
  if (!matchMark(results->rawbuf[offset++], kHaierAcHdr)) return false;
  if (!matchSpace(results->rawbuf[offset++], kHaierAcHdr)) return false;

  // Match Header + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, results->state,
                    results->rawlen - offset, nbits,
                    kHaierAcHdr, kHaierAcHdrGap,
                    kHaierAcBitMark, kHaierAcOneSpace,
                    kHaierAcBitMark, kHaierAcZeroSpace,
                    kHaierAcBitMark, kHaierAcMinGap, true,
                    _tolerance, kMarkExcess)) return false;

  // Compliance
  if (strict) {
    if (results->state[0] != kHaierAcPrefix) return false;
    if (!IRHaierAC::validChecksum(results->state, nbits / 8)) return false;
  }

  // Success
  results->decode_type = HAIER_AC;
  results->bits = nbits;
  return true;
}
#endif  // (DECODE_HAIER_AC || DECODE_HAIER_AC_YRW02)

#if DECODE_HAIER_AC_YRW02
/// Decode the supplied Haier YR-W02 remote A/C message.
/// Status: BETA / Appears to be working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeHaierACYRW02(decode_results* results, uint16_t offset,
                                const uint16_t nbits, const bool strict) {
  if (strict) {
    if (nbits != kHaierACYRW02Bits)
      return false;  // Not strictly a HAIER_AC_YRW02 message.
  }

  // The protocol is almost exactly the same as HAIER_AC
  if (!decodeHaierAC(results, offset, nbits, false)) return false;

  // Compliance
  if (strict) {
    if (results->state[0] != kHaierAcYrw02Prefix) return false;
    if (!IRHaierACYRW02::validChecksum(results->state, nbits / 8)) return false;
  }

  // Success
  // It looks correct, but we haven't check the checksum etc.
  results->decode_type = HAIER_AC_YRW02;
  return true;
}
#endif  // DECODE_HAIER_AC_YRW02
