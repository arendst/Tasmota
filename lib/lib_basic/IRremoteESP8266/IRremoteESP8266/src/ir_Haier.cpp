// Copyright 2018-2021 crankyoldgit
/// @file
/// @brief Support for Haier A/C protocols.
/// The specifics of reverse engineering the protocols details:
/// * HSU07-HEA03 by kuzin2006.
/// * YR-W02/HSU-09HMC203 by non7top.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/404
/// @see https://www.dropbox.com/s/mecyib3lhdxc8c6/IR%20data%20reverse%20engineering.xlsx?dl=0
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/485
/// @see https://www.dropbox.com/sh/w0bt7egp0fjger5/AADRFV6Wg4wZskJVdFvzb8Z0a?dl=0&preview=haer2.ods
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1480

#include "ir_Haier.h"
#include <algorithm>
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
using irutils::addModelToString;
using irutils::addSwingHToString;
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

#if (SEND_HAIER_AC || SEND_HAIER_AC_YRW02 || SEND_HAIER_AC160 || \
     SEND_HAIER_AC176)
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
#endif  // (SEND_HAIER_AC || SEND_HAIER_AC_YRW02 || SEND_HAIER_AC176)

#if SEND_HAIER_AC_YRW02
/// Send a Haier YR-W02 remote A/C formatted message.
/// Status: STABLE / Known to be working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendHaierACYRW02(const unsigned char data[], const uint16_t nbytes,
                              const uint16_t repeat) {
  if (nbytes >= kHaierACYRW02StateLength) sendHaierAC(data, nbytes, repeat);
}
#endif  // SEND_HAIER_AC_YRW02

#if SEND_HAIER_AC176
/// Send a Haier 176 bit remote A/C formatted message.
/// Status: STABLE / Known to be working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendHaierAC176(const unsigned char data[], const uint16_t nbytes,
                            const uint16_t repeat) {
  if (nbytes >= kHaierAC176StateLength) sendHaierAC(data, nbytes, repeat);
}
#endif  // SEND_HAIER_AC176

#if SEND_HAIER_AC160
/// Send a Haier 160 bit remote A/C formatted message.
/// Status: STABLE / Known to be working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendHaierAC160(const unsigned char data[], const uint16_t nbytes,
                            const uint16_t repeat) {
  if (nbytes >= kHaierAC160StateLength) sendHaierAC(data, nbytes, repeat);
}
#endif  // SEND_HAIER_AC160

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
/// @return The native vertical swing mode.
uint8_t IRHaierAC::getSwingV(void) const {
  return _.SwingV;
}

/// Set the Vertical Swing mode of the A/C.
/// @param[in] state The mode to set the vanes to.
void IRHaierAC::setSwingV(const uint8_t state) {
  if (state == _.SwingV) return;  // Nothing to do.
  switch (state) {
    case kHaierAcSwingVOff:
    case kHaierAcSwingVUp:
    case kHaierAcSwingVDown:
    case kHaierAcSwingVChg:
      _.Command = kHaierAcCmdSwing;
      _.SwingV = state;
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
    case stdAc::swingv_t::kMiddle:  return kHaierAcSwingVUp;
    case stdAc::swingv_t::kLow:
    case stdAc::swingv_t::kLowest:  return kHaierAcSwingVDown;
    case stdAc::swingv_t::kOff:     return kHaierAcSwingVOff;
    default:                        return kHaierAcSwingVChg;
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
    case kHaierAcSwingVUp:   return stdAc::swingv_t::kHighest;
    case kHaierAcSwingVDown: return stdAc::swingv_t::kLowest;
    case kHaierAcSwingVOff:  return stdAc::swingv_t::kOff;
    default:                 return stdAc::swingv_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRHaierAC::toCommon(void) const {
  stdAc::state_t result{};
  result.protocol = decode_type_t::HAIER_AC;
  result.model = -1;  // No models used.
  result.power = true;
  if (_.Command == kHaierAcCmdOff) result.power = false;
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(getFan());
  result.swingv = toCommonSwingV(_.SwingV);
  result.filter = _.Health;
  result.sleep = _.Sleep ? 0 : -1;
  // Not supported.
  result.swingh = stdAc::swingh_t::kOff;
  result.quiet = false;
  result.turbo = false;
  result.econo = false;
  result.light = false;
  result.clean = false;
  result.beep = true;
  result.clock = -1;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRHaierAC::toString(void) const {
  String result = "";
  result.reserve(170);  // Reserve some heap for the string to reduce fragging.
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
      result += kSwingVStr;
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
  result += addIntToString(_.SwingV, kSwingVStr);
  result += kSpaceLBraceStr;
  switch (_.SwingV) {
    case kHaierAcSwingVOff:
      result += kOffStr;
      break;
    case kHaierAcSwingVUp:
      result += kUpStr;
      break;
    case kHaierAcSwingVDown:
      result += kDownStr;
      break;
    case kHaierAcSwingVChg:
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
IRHaierAC176::IRHaierAC176(const uint16_t pin, const bool inverted,
                           const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Set up hardware to be able to send a message.
void IRHaierAC176::begin(void) { _irsend.begin(); }

#if SEND_HAIER_AC176
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRHaierAC176::send(const uint16_t repeat) {
  _irsend.sendHaierAC176(getRaw(), kHaierAC176StateLength, repeat);
}
#endif  // SEND_HAIER_AC176

/// Calculate and set the checksum values for the internal state.
void IRHaierAC176::checksum(void) {
  _.Sum = sumBytes(_.raw, kHaierACYRW02StateLength - 1);
  _.Sum2 = sumBytes(_.raw + kHaierACYRW02StateLength,
                    kHaierAC176StateLength - kHaierACYRW02StateLength - 1);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length of the state array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRHaierAC176::validChecksum(const uint8_t state[], const uint16_t length) {
  if (length < 2) return false;  // 1 byte of data can't have a checksum.
  if (length < kHaierAC160StateLength) {  // Is it too short?
    // Then it is just a checksum of the whole thing.
    return (state[length - 1] == sumBytes(state, length - 1));
  } else {  // It is long enough for two checksums.
    return (state[kHaierACYRW02StateLength - 1] ==
            sumBytes(state, kHaierACYRW02StateLength - 1)) &&
           (state[length - 1] ==
            sumBytes(state + kHaierACYRW02StateLength,
                     length - kHaierACYRW02StateLength - 1));
  }
}

/// Reset the internal state to a fixed known good state.
void IRHaierAC176::stateReset(void) {
  std::memset(_.raw, 0, sizeof _.raw);
  _.Model = kHaierAcYrw02ModelA;
  _.Prefix2 = kHaierAc176Prefix;
  _.Temp = kHaierAcYrw02DefTempC - kHaierAcYrw02MinTempC;
  _.Health = true;
  setFan(kHaierAcYrw02FanAuto);
  _.Power = true;
  _.Button = kHaierAcYrw02ButtonPower;
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t* IRHaierAC176::getRaw(void) {
  checksum();
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
void IRHaierAC176::setRaw(const uint8_t new_code[]) {
  memcpy(_.raw, new_code, kHaierAC176StateLength);
}

/// Set the Button/Command setting of the A/C.
/// @param[in] button The value of the button/command that was pressed.
void IRHaierAC176::setButton(uint8_t button) {
  switch (button) {
    case kHaierAcYrw02ButtonTempUp:
    case kHaierAcYrw02ButtonTempDown:
    case kHaierAcYrw02ButtonSwingV:
    case kHaierAcYrw02ButtonSwingH:
    case kHaierAcYrw02ButtonFan:
    case kHaierAcYrw02ButtonPower:
    case kHaierAcYrw02ButtonMode:
    case kHaierAcYrw02ButtonHealth:
    case kHaierAcYrw02ButtonTurbo:
    case kHaierAcYrw02ButtonSleep:
    case kHaierAcYrw02ButtonLock:
    case kHaierAcYrw02ButtonCFAB:
      _.Button = button;
  }
}

/// Get/Detect the model of the A/C.
/// @return The enum of the compatible model.
haier_ac176_remote_model_t IRHaierAC176::getModel(void) const {
  switch (_.Model) {
    case kHaierAcYrw02ModelB: return haier_ac176_remote_model_t::V9014557_B;
    default:                  return haier_ac176_remote_model_t::V9014557_A;
  }
}

/// Set the model of the A/C to emulate.
/// @param[in] model The enum of the appropriate model.
void IRHaierAC176::setModel(haier_ac176_remote_model_t model) {
  _.Button = kHaierAcYrw02ButtonCFAB;
  switch (model) {
    case haier_ac176_remote_model_t::V9014557_B:
      _.Model = kHaierAcYrw02ModelB;
      break;
    default:
      _.Model = kHaierAcYrw02ModelA;
  }
}

/// Get the Button/Command setting of the A/C.
/// @return The value of the button/command that was pressed.
uint8_t IRHaierAC176::getButton(void) const {
  return _.Button;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRHaierAC176::setMode(uint8_t mode) {
  switch (mode) {
    case kHaierAcYrw02Auto:
    case kHaierAcYrw02Dry:
    case kHaierAcYrw02Fan:
      // Turbo & Quiet is only available in Cool/Heat mode.
      _.Turbo = false;
      _.Quiet = false;
      // FALL-THRU
    case kHaierAcYrw02Cool:
    case kHaierAcYrw02Heat:
      _.Button = kHaierAcYrw02ButtonMode;
      _.Mode = mode;
      break;
    default:
      setMode(kHaierAcYrw02Auto);  // Unexpected, default to auto mode.
  }
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRHaierAC176::getMode(void) const { return _.Mode; }

/// Set the default temperature units to use.
/// @param[in] on Use Fahrenheit as the units.
///   true is Fahrenheit, false is Celsius.
void IRHaierAC176::setUseFahrenheit(const bool on) { _.UseFahrenheit = on; }

/// Get the default temperature units in use.
/// @return true is Fahrenheit, false is Celsius.
bool IRHaierAC176::getUseFahrenheit(void) const { return _.UseFahrenheit; }

/// Set the temperature.
/// @param[in] degree The temperature in degrees.
/// @param[in] fahrenheit Use units of Fahrenheit and set that as units used.
void IRHaierAC176::setTemp(const uint8_t degree, const bool fahrenheit) {
  uint8_t old_temp = getTemp();
  if (old_temp == degree) return;

  if (_.UseFahrenheit == fahrenheit) {
    if (old_temp > degree)
      _.Button = kHaierAcYrw02ButtonTempDown;
    else
      _.Button = kHaierAcYrw02ButtonTempUp;
  } else {
      _.Button = kHaierAcYrw02ButtonCFAB;
  }
  _.UseFahrenheit = fahrenheit;

  uint8_t temp = degree;
  if (fahrenheit) {
    if (temp < kHaierAcYrw02MinTempF)
      temp = kHaierAcYrw02MinTempF;
    else if (temp > kHaierAcYrw02MaxTempF)
      temp = kHaierAcYrw02MaxTempF;
    if (degree >= 77) { temp++; }
    if (degree >= 79) { temp++; }
    // See at IRHaierAC176::getTemp() comments for clarification
    _.ExtraDegreeF = temp % 2;
    _.Temp = (temp - kHaierAcYrw02MinTempF -_.ExtraDegreeF) >> 1;
  } else {
    if (temp < kHaierAcYrw02MinTempC)
      temp = kHaierAcYrw02MinTempC;
    else if (temp > kHaierAcYrw02MaxTempC)
      temp = kHaierAcYrw02MaxTempC;
    _.Temp = temp - kHaierAcYrw02MinTempC;
  }
}

/// Get the current temperature setting.
/// The unit of temperature is specified by UseFahrenheit value.
/// @return The current setting for temperature.
uint8_t IRHaierAC176::getTemp(void) const {
  if (!_.UseFahrenheit) { return _.Temp + kHaierAcYrw02MinTempC; }
  uint8_t degree = _.Temp*2 + kHaierAcYrw02MinTempF + _.ExtraDegreeF;
  // The way of coding the temperature in degree Fahrenheit is
  // kHaierAcYrw02MinTempF + Temp*2 + ExtraDegreeF, for example
  // Temp = 0b0011, ExtraDegreeF = 0b1, temperature is 60 + 3*2 + 1 = 67F
  // But around 78F there is unconsistency, see table below
  //
  // | Fahrenheit | Temp   | ExtraDegreeF |
  // |    60F     | 0b0000 |     0b0      |
  // |    61F     | 0b0000 |     0b1      |
  // |    62F     | 0b0001 |     0b0      |
  // |    63F     | 0b0001 |     0b1      |
  // |    64F     | 0b0010 |     0b0      |
  // |    65F     | 0b0010 |     0b1      |
  // |    66F     | 0b0011 |     0b0      |
  // |    67F     | 0b0011 |     0b1      |
  // |    68F     | 0b0100 |     0b0      |
  // |    69F     | 0b0100 |     0b1      |
  // |    70F     | 0b0101 |     0b0      |
  // |    71F     | 0b0101 |     0b1      |
  // |    72F     | 0b0110 |     0b0      |
  // |    73F     | 0b0110 |     0b1      |
  // |    74F     | 0b0111 |     0b0      |
  // |    75F     | 0b0111 |     0b1      |
  // |    76F     | 0b1000 |     0b0      |
  // |  Not Used  | 0b1000 |     0b1      |
  // |    77F     | 0b1001 |     0b0      |
  // |  Not Used  | 0b1001 |     0b1      |
  // |    78F     | 0b1010 |     0b0      |
  // |    79F     | 0b1010 |     0b1      |
  // |    80F     | 0b1011 |     0b0      |
  // |    81F     | 0b1011 |     0b1      |
  // |    82F     | 0b1100 |     0b0      |
  // |    83F     | 0b1100 |     0b1      |
  // |    84F     | 0b1101 |     0b0      |
  // |    86F     | 0b1110 |     0b0      |
  // |    85F     | 0b1101 |     0b1      |
  if (degree >= 77) { degree--; }
  if (degree >= 79) { degree--; }
  return degree;
}

/// Set the Health (filter) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHaierAC176::setHealth(const bool on) {
  _.Button = kHaierAcYrw02ButtonHealth;
  _.Health = on;
}

/// Get the Health (filter) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHaierAC176::getHealth(void) const { return _.Health; }

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRHaierAC176::getPower(void) const { return _.Power; }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHaierAC176::setPower(const bool on) {
  _.Button = kHaierAcYrw02ButtonPower;
  _.Power = on;
}

/// Change the power setting to On.
void IRHaierAC176::on(void) { setPower(true); }

/// Change the power setting to Off.
void IRHaierAC176::off(void) { setPower(false); }

/// Get the Sleep setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHaierAC176::getSleep(void) const { return _.Sleep; }

/// Set the Sleep setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHaierAC176::setSleep(const bool on) {
  _.Button = kHaierAcYrw02ButtonSleep;
  _.Sleep = on;
}

/// Get the Turbo setting of the A/C.
/// @return The current turbo setting.
bool IRHaierAC176::getTurbo(void) const { return _.Turbo; }

/// Set the Turbo setting of the A/C.
/// @param[in] on The desired turbo setting.
/// @note Turbo & Quiet can't be on at the same time, and only in Heat/Cool mode
void IRHaierAC176::setTurbo(const bool on) {
  switch (getMode()) {
    case kHaierAcYrw02Cool:
    case kHaierAcYrw02Heat:
      _.Turbo = on;
      _.Button = kHaierAcYrw02ButtonTurbo;
      if (on) _.Quiet = false;
  }
}

/// Get the Quiet setting of the A/C.
/// @return The current Quiet setting.
bool IRHaierAC176::getQuiet(void) const { return _.Quiet; }

/// Set the Quiet setting of the A/C.
/// @param[in] on The desired Quiet setting.
/// @note Turbo & Quiet can't be on at the same time, and only in Heat/Cool mode
void IRHaierAC176::setQuiet(const bool on) {
  switch (getMode()) {
    case kHaierAcYrw02Cool:
    case kHaierAcYrw02Heat:
      _.Quiet = on;
      _.Button = kHaierAcYrw02ButtonTurbo;
      if (on) _.Turbo = false;
  }
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRHaierAC176::getFan(void) const { return _.Fan; }

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRHaierAC176::setFan(uint8_t speed) {
  switch (speed) {
    case kHaierAcYrw02FanLow:
    case kHaierAcYrw02FanMed:
    case kHaierAcYrw02FanHigh:
    case kHaierAcYrw02FanAuto:
      _.Fan = speed;
      _.Fan2 = (speed == kHaierAcYrw02FanAuto) ? 0 : speed;
      _.Button = kHaierAcYrw02ButtonFan;
  }
}

/// For backward compatibility. Use getSwingV() instead.
/// Get the Vertical Swing position setting of the A/C.
/// @return The native position/mode.
uint8_t IRHaierAC176::getSwing(void) const {
  return IRHaierAC176::getSwingV();
}

/// For backward compatibility. Use setSwingV() instead.
/// Set the Vertical Swing mode of the A/C.
/// @param[in] pos The position/mode to set the vanes to.
void IRHaierAC176::setSwing(uint8_t pos) { setSwingV(pos); }

/// Get the Vertical Swing position setting of the A/C.
/// @return The native position/mode.
uint8_t IRHaierAC176::getSwingV(void) const { return _.SwingV; }

/// Set the Vertical Swing mode of the A/C.
/// @param[in] pos The position/mode to set the vanes to.
void IRHaierAC176::setSwingV(uint8_t pos) {
  uint8_t newpos = pos;
  switch (pos) {
    case kHaierAcYrw02SwingVOff:
    case kHaierAcYrw02SwingVAuto:
    case kHaierAcYrw02SwingVTop:
    case kHaierAcYrw02SwingVMiddle:
    case kHaierAcYrw02SwingVBottom:
    case kHaierAcYrw02SwingVDown: _.Button = kHaierAcYrw02ButtonSwingV; break;
    default: return;  // Unexpected value so don't do anything.
  }
  // Heat mode has no MIDDLE setting, use BOTTOM instead.
  if (pos == kHaierAcYrw02SwingVMiddle && _.Mode == kHaierAcYrw02Heat)
    newpos = kHaierAcYrw02SwingVBottom;
  // BOTTOM is only allowed if we are in Heat mode, otherwise MIDDLE.
  if (pos == kHaierAcYrw02SwingVBottom && _.Mode != kHaierAcYrw02Heat)
    newpos = kHaierAcYrw02SwingVMiddle;
  _.SwingV = newpos;
}

/// Get the Horizontal Swing position setting of the A/C.
/// @return The native position/mode.
uint8_t IRHaierAC176::getSwingH(void) const { return _.SwingH; }

/// Set the Horizontal Swing mode of the A/C.
/// @param[in] pos The position/mode to set the vanes to.
void IRHaierAC176::setSwingH(uint8_t pos) {
  switch (pos) {
    case kHaierAcYrw02SwingHMiddle:
    case kHaierAcYrw02SwingHLeftMax:
    case kHaierAcYrw02SwingHLeft:
    case kHaierAcYrw02SwingHRight:
    case kHaierAcYrw02SwingHRightMax:
    case kHaierAcYrw02SwingHAuto: _.Button = kHaierAcYrw02ButtonSwingH; break;
    default: return;  // Unexpected value so don't do anything.
  }
  _.SwingH = pos;
}


/// Set the Timer operating mode.
/// @param[in] mode The timer mode to use.
void IRHaierAC176::setTimerMode(const uint8_t mode) {
  _.TimerMode = (mode > kHaierAcYrw02OffThenOnTimer) ? kHaierAcYrw02NoTimers
                                                     : mode;
  switch (_.TimerMode) {
    case kHaierAcYrw02NoTimers:
      setOnTimer(0);  // Disable the On timer.
      setOffTimer(0);  // Disable the Off timer.
      break;
    case kHaierAcYrw02OffTimer:
      setOnTimer(0);  // Disable the On timer.
      break;
    case kHaierAcYrw02OnTimer:
      setOffTimer(0);  // Disable the Off timer.
      break;
  }
}

/// Get the Timer operating mode.
/// @return The mode of the timer is currently configured to.
uint8_t IRHaierAC176::getTimerMode(void) const { return _.TimerMode; }

/// Set the number of minutes of the On Timer setting.
/// @param[in] mins Nr. of Minutes for the Timer. `0` means disable the timer.
void IRHaierAC176::setOnTimer(const uint16_t mins) {
  const uint16_t nr_mins = std::min((uint16_t)(23 * 60 + 59), mins);
  _.OnTimerHrs = nr_mins / 60;
  _.OnTimerMins = nr_mins % 60;

  const bool enabled = (nr_mins > 0);
  uint8_t mode = getTimerMode();
  switch (mode) {
    case kHaierAcYrw02OffTimer:
      mode = enabled ? kHaierAcYrw02OffThenOnTimer : mode;
      break;
    case kHaierAcYrw02OnThenOffTimer:
    case kHaierAcYrw02OffThenOnTimer:
      mode = enabled ? kHaierAcYrw02OffThenOnTimer : kHaierAcYrw02OffTimer;
      break;
    default:
      // Enable/Disable the On timer for the simple case.
      mode = enabled << 1;
  }
  _.TimerMode = mode;
}

/// Get the number of minutes of the On Timer setting.
/// @return Nr of minutes.
uint16_t IRHaierAC176::getOnTimer(void) const {
  return _.OnTimerHrs * 60 + _.OnTimerMins;
}

/// Set the number of minutes of the Off Timer setting.
/// @param[in] mins Nr. of Minutes for the Timer. `0` means disable the timer.
void IRHaierAC176::setOffTimer(const uint16_t mins) {
  const uint16_t nr_mins = std::min((uint16_t)(23 * 60 + 59), mins);
  _.OffTimerHrs = nr_mins / 60;
  _.OffTimerMins = nr_mins % 60;

  const bool enabled = (nr_mins > 0);
  uint8_t mode = getTimerMode();
  switch (mode) {
    case kHaierAcYrw02OnTimer:
      mode = enabled ? kHaierAcYrw02OnThenOffTimer : mode;
      break;
    case kHaierAcYrw02OnThenOffTimer:
    case kHaierAcYrw02OffThenOnTimer:
      mode = enabled ? kHaierAcYrw02OnThenOffTimer : kHaierAcYrw02OnTimer;
      break;
    default:
      // Enable/Disable the Off timer for the simple case.
      mode = enabled;
  }
  _.TimerMode = mode;
}

/// Get the number of minutes of the Off Timer setting.
/// @return Nr of minutes.
uint16_t IRHaierAC176::getOffTimer(void) const {
  return _.OffTimerHrs * 60 + _.OffTimerMins;
}

/// Get the Lock setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHaierAC176::getLock(void) const { return _.Lock; }

/// Set the Lock setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHaierAC176::setLock(const bool on) {
  _.Button = kHaierAcYrw02ButtonLock;
  _.Lock = on;
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRHaierAC176::convertMode(const stdAc::opmode_t mode) {
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
uint8_t IRHaierAC176::convertFan(const stdAc::fanspeed_t speed) {
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
uint8_t IRHaierAC176::convertSwingV(const stdAc::swingv_t position) {
  switch (position) {
    case stdAc::swingv_t::kHighest:
    case stdAc::swingv_t::kHigh:   return kHaierAcYrw02SwingVTop;
    case stdAc::swingv_t::kMiddle: return kHaierAcYrw02SwingVMiddle;
    case stdAc::swingv_t::kLow:    return kHaierAcYrw02SwingVDown;
    case stdAc::swingv_t::kLowest: return kHaierAcYrw02SwingVBottom;
    case stdAc::swingv_t::kOff:    return kHaierAcYrw02SwingVOff;
    default:                       return kHaierAcYrw02SwingVAuto;
  }
}

/// Convert a stdAc::swingh_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRHaierAC176::convertSwingH(const stdAc::swingh_t position) {
  switch (position) {
    case stdAc::swingh_t::kMiddle:   return kHaierAcYrw02SwingHMiddle;
    case stdAc::swingh_t::kLeftMax:  return kHaierAcYrw02SwingHLeftMax;
    case stdAc::swingh_t::kLeft:     return kHaierAcYrw02SwingHLeft;
    case stdAc::swingh_t::kRight:    return kHaierAcYrw02SwingHRight;
    case stdAc::swingh_t::kRightMax: return kHaierAcYrw02SwingHRightMax;
    case stdAc::swingh_t::kAuto:     return kHaierAcYrw02SwingHAuto;
    default:                         return kHaierAcYrw02SwingHMiddle;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRHaierAC176::toCommonMode(const uint8_t mode) {
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
stdAc::fanspeed_t IRHaierAC176::toCommonFanSpeed(const uint8_t speed) {
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
stdAc::swingv_t IRHaierAC176::toCommonSwingV(const uint8_t pos) {
  switch (pos) {
    case kHaierAcYrw02SwingVTop:    return stdAc::swingv_t::kHighest;
    case kHaierAcYrw02SwingVMiddle: return stdAc::swingv_t::kMiddle;
    case kHaierAcYrw02SwingVDown:   return stdAc::swingv_t::kLow;
    case kHaierAcYrw02SwingVBottom: return stdAc::swingv_t::kLowest;
    case kHaierAcYrw02SwingVOff:    return stdAc::swingv_t::kOff;
    default:                        return stdAc::swingv_t::kAuto;
  }
}

/// Convert a stdAc::swingh_t enum into it's native setting.
/// @param[in] pos The enum to be converted.
/// @return The native equivalent of the enum.
stdAc::swingh_t IRHaierAC176::toCommonSwingH(const uint8_t pos) {
  switch (pos) {
    case kHaierAcYrw02SwingHMiddle:   return stdAc::swingh_t::kMiddle;
    case kHaierAcYrw02SwingHLeftMax:  return stdAc::swingh_t::kLeftMax;
    case kHaierAcYrw02SwingHLeft:     return stdAc::swingh_t::kLeft;
    case kHaierAcYrw02SwingHRight:    return stdAc::swingh_t::kRight;
    case kHaierAcYrw02SwingHRightMax: return stdAc::swingh_t::kRightMax;
    case kHaierAcYrw02SwingHAuto:     return stdAc::swingh_t::kAuto;
    default:                          return stdAc::swingh_t::kOff;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRHaierAC176::toCommon(void) const {
  stdAc::state_t result{};
  result.protocol = decode_type_t::HAIER_AC_YRW02;
  result.model = getModel();
  result.power = _.Power;
  result.mode = toCommonMode(_.Mode);
  result.celsius = !_.UseFahrenheit;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.swingv = toCommonSwingV(_.SwingV);
  result.swingh = toCommonSwingH(_.SwingH);
  result.filter = _.Health;
  result.sleep = _.Sleep ? 0 : -1;
  result.turbo = _.Turbo;
  result.quiet = _.Quiet;
  // Not supported.
  result.econo = false;
  result.light = false;
  result.clean = false;
  result.beep = true;
  result.clock = -1;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRHaierAC176::toString(void) const {
  String result = "";
  result.reserve(280);  // Reserve some heap for the string to reduce fragging.
  result += addModelToString(decode_type_t::HAIER_AC176, getModel(), false);
  result += addBoolToString(_.Power, kPowerStr);
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
    case kHaierAcYrw02ButtonSwingV:
      result += kSwingVStr;
      break;
    case kHaierAcYrw02ButtonSwingH:
      result += kSwingHStr;
      break;
    case kHaierAcYrw02ButtonTurbo:
      result += kTurboStr;
      break;
    case kHaierAcYrw02ButtonTimer:
      result += kTimerStr;
      break;
    case kHaierAcYrw02ButtonLock:
      result += kLockStr;
      break;
    case kHaierAcYrw02ButtonCFAB:
      result += kCelsiusFahrenheitStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addModeToString(_.Mode, kHaierAcYrw02Auto, kHaierAcYrw02Cool,
                            kHaierAcYrw02Heat, kHaierAcYrw02Dry,
                            kHaierAcYrw02Fan);
  result += addTempToString(getTemp(), !_.UseFahrenheit);
  result += addFanToString(_.Fan, kHaierAcYrw02FanHigh, kHaierAcYrw02FanLow,
                           kHaierAcYrw02FanAuto, kHaierAcYrw02FanAuto,
                           kHaierAcYrw02FanMed);
  result += addBoolToString(_.Turbo, kTurboStr);
  result += addBoolToString(_.Quiet, kQuietStr);
  result += addIntToString(_.SwingV, kSwingVStr);
  result += kSpaceLBraceStr;
  switch (_.SwingV) {
    case kHaierAcYrw02SwingVOff:
      result += kOffStr;
      break;
    case kHaierAcYrw02SwingVAuto:
      result += kAutoStr;
      break;
    case kHaierAcYrw02SwingVBottom:
      result += kLowestStr;
      break;
    case kHaierAcYrw02SwingVDown:
      result += kLowStr;
      break;
    case kHaierAcYrw02SwingVTop:
      result += kHighestStr;
      break;
    case kHaierAcYrw02SwingVMiddle:
      result += kMiddleStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addSwingHToString(_.SwingH, kHaierAcYrw02SwingHAuto,
                              kHaierAcYrw02SwingHLeftMax,
                              kHaierAcYrw02SwingHLeft,
                              kHaierAcYrw02SwingHMiddle,
                              kHaierAcYrw02SwingHRight,
                              kHaierAcYrw02SwingHRightMax,
                              // Below are unused.
                              kHaierAcYrw02SwingHMiddle,
                              kHaierAcYrw02SwingHMiddle,
                              kHaierAcYrw02SwingHMiddle,
                              kHaierAcYrw02SwingHMiddle,
                              kHaierAcYrw02SwingHMiddle);
  result += addBoolToString(_.Sleep, kSleepStr);
  result += addBoolToString(_.Health, kHealthStr);
  const uint8_t tmode = getTimerMode();
  result += addIntToString(tmode, kTimerModeStr);
  result += kSpaceLBraceStr;
  switch (tmode) {
    case kHaierAcYrw02NoTimers:
      result += kNAStr;
      break;
    case kHaierAcYrw02OnTimer:
      result += kOnStr;
      break;
    case kHaierAcYrw02OffTimer:
      result += kOffStr;
      break;
    case kHaierAcYrw02OnThenOffTimer:
      result += kOnStr;
      result += '-';
      result += kOffStr;
      break;
    case kHaierAcYrw02OffThenOnTimer:
      result += kOffStr;
      result += '-';
      result += kOnStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addLabeledString((tmode != kHaierAcYrw02NoTimers &&
                              tmode != kHaierAcYrw02OffTimer) ?
      minsToString(getOnTimer()) : kOffStr, kOnTimerStr);
  result += addLabeledString((tmode != kHaierAcYrw02NoTimers &&
                              tmode != kHaierAcYrw02OnTimer) ?
      minsToString(getOffTimer()) : kOffStr, kOffTimerStr);
  result += addBoolToString(_.Lock, kLockStr);
  return result;
}
// End of IRHaierAC176 class.


/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRHaierACYRW02::IRHaierACYRW02(const uint16_t pin, const bool inverted,
                               const bool use_modulation)
    : IRHaierAC176(pin, inverted, use_modulation) { stateReset(); }

#if SEND_HAIER_AC_YRW02
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRHaierACYRW02::send(const uint16_t repeat) {
  _irsend.sendHaierACYRW02(getRaw(), kHaierACYRW02StateLength, repeat);
}
#endif  // SEND_HAIER_AC_YRW02

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
void IRHaierACYRW02::setRaw(const uint8_t new_code[]) {
  memcpy(_.raw, new_code, kHaierACYRW02StateLength);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length of the state array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRHaierACYRW02::validChecksum(const uint8_t state[],
                                   const uint16_t length) {
  return IRHaierAC176::validChecksum(state, length);
}
// End of IRHaierACYRW02 class.

#if (DECODE_HAIER_AC || DECODE_HAIER_AC_YRW02 || DECODE_HAIER_AC160 || \
     DECODE_HAIER_AC176)
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
    if (results->state[0] != kHaierAcYrw02ModelA) return false;
    if (!IRHaierACYRW02::validChecksum(results->state, nbits / 8)) return false;
  }

  // Success
  // It looks correct, but we haven't check the checksum etc.
  results->decode_type = HAIER_AC_YRW02;
  return true;
}
#endif  // DECODE_HAIER_AC_YRW02

#if DECODE_HAIER_AC176
/// Decode the supplied Haier 176 bit remote A/C message.
/// Status: STABLE / Known to be working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeHaierAC176(decode_results* results, uint16_t offset,
                              const uint16_t nbits, const bool strict) {
  if (strict) {
    if (nbits != kHaierAC176Bits)
      return false;  // Not strictly a HAIER_AC176 message.
  }

  // The protocol is almost exactly the same as HAIER_AC
  if (!decodeHaierAC(results, offset, nbits, false)) return false;

  // Compliance
  if (strict) {
    if ((results->state[0] != kHaierAcYrw02ModelA) &&
        (results->state[0] != kHaierAcYrw02ModelB)) return false;
    if (!IRHaierAC176::validChecksum(results->state, nbits / 8)) return false;
  }

  // Success
  // It looks correct, but we haven't check the checksum etc.
  results->decode_type = HAIER_AC176;
  return true;
}
#endif  // DECODE_HAIER_AC176

#if DECODE_HAIER_AC160
/// Decode the supplied Haier 160 bit remote A/C message.
/// Status: STABLE / Known to be working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeHaierAC160(decode_results* results, uint16_t offset,
                              const uint16_t nbits, const bool strict) {
  if (strict) {
    if (nbits != kHaierAC160Bits)
      return false;  // Not strictly a HAIER_AC160 message.
  }

  // The protocol is almost exactly the same as HAIER_AC
  if (!decodeHaierAC(results, offset, nbits, false)) return false;

  // Compliance
  if (strict) {
    if (!IRHaierAC176::validChecksum(results->state, nbits / 8)) return false;
  }

  // Success
  // It looks correct, but we haven't check the checksum etc.
  results->decode_type = HAIER_AC160;
  return true;
}
#endif  // DECODE_HAIER_AC160


/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRHaierAC160::IRHaierAC160(const uint16_t pin, const bool inverted,
                           const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Set up hardware to be able to send a message.
void IRHaierAC160::begin(void) { _irsend.begin(); }

#if SEND_HAIER_AC160
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRHaierAC160::send(const uint16_t repeat) {
  _irsend.sendHaierAC160(getRaw(), kHaierAC160StateLength, repeat);
}
#endif  // SEND_HAIER_AC160

/// Calculate and set the checksum values for the internal state.
void IRHaierAC160::checksum(void) {
  _.Sum = sumBytes(_.raw, kHaierACYRW02StateLength - 1);
  _.Sum2 = sumBytes(_.raw + kHaierACYRW02StateLength,
                    kHaierAC160StateLength - kHaierACYRW02StateLength - 1);
}

/// Reset the internal state to a fixed known good state.
void IRHaierAC160::stateReset(void) {
  std::memset(_.raw, 0, sizeof _.raw);
  _.Model = kHaierAcYrw02ModelA;
  _.Prefix = kHaierAc160Prefix;
  _.Temp = kHaierAcYrw02DefTempC - kHaierAcYrw02MinTempC;
  setClean(false);
  setFan(kHaierAcYrw02FanAuto);
  _.Power = true;
  _.Button = kHaierAcYrw02ButtonPower;
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t* IRHaierAC160::getRaw(void) {
  checksum();
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
void IRHaierAC160::setRaw(const uint8_t new_code[]) {
  memcpy(_.raw, new_code, kHaierAC160StateLength);
}

/// Set the Button/Command setting of the A/C.
/// @param[in] button The value of the button/command that was pressed.
void IRHaierAC160::setButton(uint8_t button) {
  switch (button) {
    case kHaierAcYrw02ButtonTempUp:
    case kHaierAcYrw02ButtonTempDown:
    case kHaierAcYrw02ButtonSwingV:
    case kHaierAcYrw02ButtonSwingH:
    case kHaierAcYrw02ButtonFan:
    case kHaierAcYrw02ButtonPower:
    case kHaierAcYrw02ButtonMode:
    case kHaierAcYrw02ButtonHealth:
    case kHaierAcYrw02ButtonTurbo:
    case kHaierAcYrw02ButtonSleep:
    case kHaierAcYrw02ButtonLock:
    case kHaierAc160ButtonClean:
    case kHaierAcYrw02ButtonCFAB:
      _.Button = button;
  }
}

/// Get the Button/Command setting of the A/C.
/// @return The value of the button/command that was pressed.
uint8_t IRHaierAC160::getButton(void) const { return _.Button; }

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRHaierAC160::setMode(uint8_t mode) {
  switch (mode) {
    case kHaierAcYrw02Auto:
    case kHaierAcYrw02Dry:
    case kHaierAcYrw02Fan:
      // Turbo & Quiet is only available in Cool/Heat mode.
      _.Turbo = false;
      _.Quiet = false;
      // FALL-THRU
    case kHaierAcYrw02Cool:
    case kHaierAcYrw02Heat:
      _.Button = kHaierAcYrw02ButtonMode;
      _.Mode = mode;
      break;
    default:
      setMode(kHaierAcYrw02Auto);  // Unexpected, default to auto mode.
  }
  _.AuxHeating = (_.Mode == kHaierAcYrw02Heat);  // Set only if heat mode.
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRHaierAC160::getMode(void) const { return _.Mode; }

/// Set the default temperature units to use.
/// @param[in] on Use Fahrenheit as the units.
///   true is Fahrenheit, false is Celsius.
void IRHaierAC160::setUseFahrenheit(const bool on) { _.UseFahrenheit = on; }

/// Get the default temperature units in use.
/// @return true is Fahrenheit, false is Celsius.
bool IRHaierAC160::getUseFahrenheit(void) const { return _.UseFahrenheit; }

/// Set the temperature.
/// @param[in] degree The temperature in degrees.
/// @param[in] fahrenheit Use units of Fahrenheit and set that as units used.
void IRHaierAC160::setTemp(const uint8_t degree, const bool fahrenheit) {
  uint8_t old_temp = getTemp();
  if (old_temp == degree) return;

  if (_.UseFahrenheit == fahrenheit) {
    if (old_temp > degree)
      _.Button = kHaierAcYrw02ButtonTempDown;
    else
      _.Button = kHaierAcYrw02ButtonTempUp;
  } else {
      _.Button = kHaierAcYrw02ButtonCFAB;
  }
  _.UseFahrenheit = fahrenheit;

  uint8_t temp = degree;
  if (fahrenheit) {
    if (temp < kHaierAcYrw02MinTempF)
      temp = kHaierAcYrw02MinTempF;
    else if (temp > kHaierAcYrw02MaxTempF)
      temp = kHaierAcYrw02MaxTempF;
    if (degree >= 77) { temp++; }
    if (degree >= 79) { temp++; }
    // See at IRHaierAC160::getTemp() comments for clarification
    _.ExtraDegreeF = temp % 2;
    _.Temp = (temp - kHaierAcYrw02MinTempF -_.ExtraDegreeF) >> 1;
  } else {
    if (temp < kHaierAcYrw02MinTempC)
      temp = kHaierAcYrw02MinTempC;
    else if (temp > kHaierAcYrw02MaxTempC)
      temp = kHaierAcYrw02MaxTempC;
    _.Temp = temp - kHaierAcYrw02MinTempC;
  }
}

/// Get the current temperature setting.
/// The unit of temperature is specified by UseFahrenheit value.
/// @return The current setting for temperature.
uint8_t IRHaierAC160::getTemp(void) const {
  if (!_.UseFahrenheit) { return _.Temp + kHaierAcYrw02MinTempC; }
  uint8_t degree = _.Temp*2 + kHaierAcYrw02MinTempF + _.ExtraDegreeF;
  // The way of coding the temperature in degree Fahrenheit is
  // kHaierAcYrw02MinTempF + Temp*2 + ExtraDegreeF, for example
  // Temp = 0b0011, ExtraDegreeF = 0b1, temperature is 60 + 3*2 + 1 = 67F
  // But around 78F there is unconsistency, see table below
  //
  // | Fahrenheit | Temp   | ExtraDegreeF |
  // |    60F     | 0b0000 |     0b0      |
  // |    61F     | 0b0000 |     0b1      |
  // |    62F     | 0b0001 |     0b0      |
  // |    63F     | 0b0001 |     0b1      |
  // |    64F     | 0b0010 |     0b0      |
  // |    65F     | 0b0010 |     0b1      |
  // |    66F     | 0b0011 |     0b0      |
  // |    67F     | 0b0011 |     0b1      |
  // |    68F     | 0b0100 |     0b0      |
  // |    69F     | 0b0100 |     0b1      |
  // |    70F     | 0b0101 |     0b0      |
  // |    71F     | 0b0101 |     0b1      |
  // |    72F     | 0b0110 |     0b0      |
  // |    73F     | 0b0110 |     0b1      |
  // |    74F     | 0b0111 |     0b0      |
  // |    75F     | 0b0111 |     0b1      |
  // |    76F     | 0b1000 |     0b0      |
  // |  Not Used  | 0b1000 |     0b1      |
  // |    77F     | 0b1001 |     0b0      |
  // |  Not Used  | 0b1001 |     0b1      |
  // |    78F     | 0b1010 |     0b0      |
  // |    79F     | 0b1010 |     0b1      |
  // |    80F     | 0b1011 |     0b0      |
  // |    81F     | 0b1011 |     0b1      |
  // |    82F     | 0b1100 |     0b0      |
  // |    83F     | 0b1100 |     0b1      |
  // |    84F     | 0b1101 |     0b0      |
  // |    86F     | 0b1110 |     0b0      |
  // |    85F     | 0b1101 |     0b1      |
  if (degree >= 77) { degree--; }
  if (degree >= 79) { degree--; }
  return degree;
}

/// Set the Clean setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHaierAC160::setClean(const bool on) {
  _.Button = kHaierAc160ButtonClean;
  _.Clean = on;
  _.Clean2 = on;
}

/// Get the Clean setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHaierAC160::getClean(void) const { return _.Clean && _.Clean2; }

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRHaierAC160::getPower(void) const { return _.Power; }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHaierAC160::setPower(const bool on) {
  _.Button = kHaierAcYrw02ButtonPower;
  _.Power = on;
}

/// Change the power setting to On.
void IRHaierAC160::on(void) { setPower(true); }

/// Change the power setting to Off.
void IRHaierAC160::off(void) { setPower(false); }

/// Get the Sleep setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHaierAC160::getSleep(void) const { return _.Sleep; }

/// Set the Sleep setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHaierAC160::setSleep(const bool on) {
  _.Button = kHaierAcYrw02ButtonSleep;
  _.Sleep = on;
}

/// Get the Turbo setting of the A/C.
/// @return The current turbo setting.
bool IRHaierAC160::getTurbo(void) const { return _.Turbo; }

/// Set the Turbo setting of the A/C.
/// @param[in] on The desired turbo setting.
/// @note Turbo & Quiet can't be on at the same time, and only in Heat/Cool mode
void IRHaierAC160::setTurbo(const bool on) {
  switch (getMode()) {
    case kHaierAcYrw02Cool:
    case kHaierAcYrw02Heat:
      _.Turbo = on;
      _.Button = kHaierAcYrw02ButtonTurbo;
      if (on) _.Quiet = false;
  }
}

/// Get the Quiet setting of the A/C.
/// @return The current Quiet setting.
bool IRHaierAC160::getQuiet(void) const { return _.Quiet; }

/// Set the Quiet setting of the A/C.
/// @param[in] on The desired Quiet setting.
/// @note Turbo & Quiet can't be on at the same time, and only in Heat/Cool mode
void IRHaierAC160::setQuiet(const bool on) {
  switch (getMode()) {
    case kHaierAcYrw02Cool:
    case kHaierAcYrw02Heat:
      _.Quiet = on;
      _.Button = kHaierAcYrw02ButtonTurbo;
      if (on) _.Turbo = false;
  }
}

/// Get the value of the Aux Heating setting.
/// @return true, the setting is on. false, the setting is off.
bool IRHaierAC160::getAuxHeating(void) const { return _.AuxHeating; }

/// Change the Aux Heating setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHaierAC160::setAuxHeating(const bool on) {
  _.Button = kHaierAc160ButtonAuxHeating;
  _.AuxHeating = on;
}

/// Get the value of the current Light toggle setting.
/// @return true, the setting is on. false, the setting is off.
/// @note This setting seems to be controlled just by the button setting.
bool IRHaierAC160::getLightToggle(void) const {
  return _.Button == kHaierAc160ButtonLight;
}

/// Set the Light Toggle setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @note This setting seems to be controlled just by the button setting.
void IRHaierAC160::setLightToggle(const bool on) {
  _.Button = on ? kHaierAc160ButtonLight : kHaierAcYrw02ButtonPower;
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRHaierAC160::getFan(void) const { return _.Fan; }

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRHaierAC160::setFan(uint8_t speed) {
  switch (speed) {
    case kHaierAcYrw02FanLow:
    case kHaierAcYrw02FanMed:
    case kHaierAcYrw02FanHigh:
    case kHaierAcYrw02FanAuto:
      _.Fan = speed;
      _.Fan2 = (speed == kHaierAcYrw02FanAuto) ? 0 : speed;
      _.Button = kHaierAcYrw02ButtonFan;
  }
}

/// Set the Health (filter) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHaierAC160::setHealth(const bool on) {
  _.Button = kHaierAcYrw02ButtonHealth;
  _.Health = on;
}

/// Get the Health (filter) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHaierAC160::getHealth(void) const { return _.Health; }

/// Get the Vertical Swing position setting of the A/C.
/// @return The native position/mode.
uint8_t IRHaierAC160::getSwingV(void) const { return _.SwingV; }

/// Set the Vertical Swing mode of the A/C.
/// @param[in] pos The position/mode to set the vanes to.
void IRHaierAC160::setSwingV(const uint8_t pos) {
  switch (pos) {
    case kHaierAc160SwingVOff:
    case kHaierAc160SwingVAuto:
    case kHaierAc160SwingVTop:
    case kHaierAc160SwingVHighest:
    case kHaierAc160SwingVHigh:
    case kHaierAc160SwingVMiddle:
    case kHaierAc160SwingVLow:
    case kHaierAc160SwingVLowest:
      _.Button = kHaierAcYrw02ButtonSwingV;
      _.SwingV = pos;
      break;
    default: return;  // If in doubt, Do nothing.
  }
}

/// Set the Timer operating mode.
/// @param[in] mode The timer mode to use.
void IRHaierAC160::setTimerMode(const uint8_t mode) {
  _.TimerMode = (mode > kHaierAcYrw02OffThenOnTimer) ? kHaierAcYrw02NoTimers
                                                     : mode;
  switch (_.TimerMode) {
    case kHaierAcYrw02NoTimers:
      setOnTimer(0);  // Disable the On timer.
      setOffTimer(0);  // Disable the Off timer.
      break;
    case kHaierAcYrw02OffTimer:
      setOnTimer(0);  // Disable the On timer.
      break;
    case kHaierAcYrw02OnTimer:
      setOffTimer(0);  // Disable the Off timer.
      break;
  }
}

/// Get the Timer operating mode.
/// @return The mode of the timer is currently configured to.
uint8_t IRHaierAC160::getTimerMode(void) const { return _.TimerMode; }

/// Set the number of minutes of the On Timer setting.
/// @param[in] mins Nr. of Minutes for the Timer. `0` means disable the timer.
void IRHaierAC160::setOnTimer(const uint16_t mins) {
  const uint16_t nr_mins = std::min((uint16_t)(23 * 60 + 59), mins);
  _.OnTimerHrs = nr_mins / 60;
  _.OnTimerMins = nr_mins % 60;

  const bool enabled = (nr_mins > 0);
  uint8_t mode = getTimerMode();
  switch (mode) {
    case kHaierAcYrw02OffTimer:
      mode = enabled ? kHaierAcYrw02OffThenOnTimer : mode;
      break;
    case kHaierAcYrw02OnThenOffTimer:
    case kHaierAcYrw02OffThenOnTimer:
      mode = enabled ? kHaierAcYrw02OffThenOnTimer : kHaierAcYrw02OffTimer;
      break;
    default:
      // Enable/Disable the On timer for the simple case.
      mode = enabled << 1;
  }
  _.TimerMode = mode;
}

/// Get the number of minutes of the On Timer setting.
/// @return Nr of minutes.
uint16_t IRHaierAC160::getOnTimer(void) const {
  return _.OnTimerHrs * 60 + _.OnTimerMins;
}

/// Set the number of minutes of the Off Timer setting.
/// @param[in] mins Nr. of Minutes for the Timer. `0` means disable the timer.
void IRHaierAC160::setOffTimer(const uint16_t mins) {
  const uint16_t nr_mins = std::min((uint16_t)(23 * 60 + 59), mins);
  _.OffTimerHrs = nr_mins / 60;
  _.OffTimerMins = nr_mins % 60;

  const bool enabled = (nr_mins > 0);
  uint8_t mode = getTimerMode();
  switch (mode) {
    case kHaierAcYrw02OnTimer:
      mode = enabled ? kHaierAcYrw02OnThenOffTimer : mode;
      break;
    case kHaierAcYrw02OnThenOffTimer:
    case kHaierAcYrw02OffThenOnTimer:
      mode = enabled ? kHaierAcYrw02OnThenOffTimer : kHaierAcYrw02OnTimer;
      break;
    default:
      // Enable/Disable the Off timer for the simple case.
      mode = enabled;
  }
  _.TimerMode = mode;
}

/// Get the number of minutes of the Off Timer setting.
/// @return Nr of minutes.
uint16_t IRHaierAC160::getOffTimer(void) const {
  return _.OffTimerHrs * 60 + _.OffTimerMins;
}

/// Get the Lock setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHaierAC160::getLock(void) const { return _.Lock; }

/// Set the Lock setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHaierAC160::setLock(const bool on) {
  _.Button = kHaierAcYrw02ButtonLock;
  _.Lock = on;
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRHaierAC160::convertMode(const stdAc::opmode_t mode) {
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
uint8_t IRHaierAC160::convertFan(const stdAc::fanspeed_t speed) {
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
uint8_t IRHaierAC160::convertSwingV(const stdAc::swingv_t position) {
  switch (position) {
    case stdAc::swingv_t::kHighest: return kHaierAc160SwingVTop;
    case stdAc::swingv_t::kHigh:    return kHaierAc160SwingVHigh;
    case stdAc::swingv_t::kMiddle:  return kHaierAc160SwingVMiddle;
    case stdAc::swingv_t::kLow:     return kHaierAc160SwingVLow;
    case stdAc::swingv_t::kLowest:  return kHaierAc160SwingVLowest;
    case stdAc::swingv_t::kOff:     return kHaierAc160SwingVOff;
    default:                        return kHaierAc160SwingVAuto;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRHaierAC160::toCommonMode(const uint8_t mode) {
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
stdAc::fanspeed_t IRHaierAC160::toCommonFanSpeed(const uint8_t speed) {
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
stdAc::swingv_t IRHaierAC160::toCommonSwingV(const uint8_t pos) {
  switch (pos) {
    case kHaierAc160SwingVTop:
    case kHaierAc160SwingVHighest: return stdAc::swingv_t::kHighest;
    case kHaierAc160SwingVHigh:    return stdAc::swingv_t::kHigh;
    case kHaierAc160SwingVMiddle:  return stdAc::swingv_t::kMiddle;
    case kHaierAc160SwingVLow:     return stdAc::swingv_t::kLow;
    case kHaierAc160SwingVLowest:  return stdAc::swingv_t::kLowest;
    case kHaierAc160SwingVOff:     return stdAc::swingv_t::kOff;
    default:                       return stdAc::swingv_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @param[in] prev Ptr to the previous state if required.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRHaierAC160::toCommon(const stdAc::state_t *prev) const {
  stdAc::state_t result{};
  // Start with the previous state if given it.
  if (prev != NULL) {
    result = *prev;
  } else {
    // Set defaults for non-zero values that are not implicitly set for when
    // there is no previous state.
    // e.g. Any setting that toggles should probably go here.
    result.light = false;
  }
  result.protocol = decode_type_t::HAIER_AC160;
  result.power = _.Power;
  result.mode = toCommonMode(_.Mode);
  result.celsius = !_.UseFahrenheit;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.swingv = toCommonSwingV(_.SwingV);
  result.swingh = stdAc::swingh_t::kOff;
  result.sleep = _.Sleep ? 0 : -1;
  result.turbo = _.Turbo;
  result.quiet = _.Quiet;
  result.clean = _.Clean && _.Clean2;
  result.light ^= getLightToggle();
  result.filter = _.Health;
  // Not supported.
  result.model = -1;
  result.econo = false;
  result.beep = true;
  result.clock = -1;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRHaierAC160::toString(void) const {
  String result = "";
  result.reserve(280);  // Reserve some heap for the string to reduce fragging.
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
    case kHaierAcYrw02ButtonSwingV:
      result += kSwingVStr;
      break;
    case kHaierAcYrw02ButtonSwingH:
      result += kSwingHStr;
      break;
    case kHaierAcYrw02ButtonTurbo:
      result += kTurboStr;
      break;
    case kHaierAcYrw02ButtonTimer:
      result += kTimerStr;
      break;
    case kHaierAcYrw02ButtonLock:
      result += kLockStr;
      break;
    case kHaierAc160ButtonClean:
      result += kCleanStr;
      break;
    case kHaierAc160ButtonLight:
      result += kLightStr;
      break;
    case kHaierAc160ButtonAuxHeating:
      result += kHeatingStr;
      break;
    case kHaierAcYrw02ButtonCFAB:
      result += kCelsiusFahrenheitStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addModeToString(_.Mode, kHaierAcYrw02Auto, kHaierAcYrw02Cool,
                            kHaierAcYrw02Heat, kHaierAcYrw02Dry,
                            kHaierAcYrw02Fan);
  result += addTempToString(getTemp(), !_.UseFahrenheit);
  result += addFanToString(_.Fan, kHaierAcYrw02FanHigh, kHaierAcYrw02FanLow,
                           kHaierAcYrw02FanAuto, kHaierAcYrw02FanAuto,
                           kHaierAcYrw02FanMed);
  result += addBoolToString(_.Turbo, kTurboStr);
  result += addBoolToString(_.Quiet, kQuietStr);
  result += addBoolToString(_.Health, kHealthStr);
  result += addIntToString(_.SwingV, kSwingVStr);
  result += kSpaceLBraceStr;
  switch (_.SwingV) {
    case kHaierAc160SwingVOff:     result += kOffStr;     break;
    case kHaierAc160SwingVAuto:    result += kAutoStr;    break;
    case kHaierAc160SwingVTop:     result += kTopStr;     break;
    case kHaierAc160SwingVHighest: result += kHighestStr; break;
    case kHaierAc160SwingVHigh:    result += kHighStr;    break;
    case kHaierAc160SwingVMiddle:  result += kMiddleStr;  break;
    case kHaierAc160SwingVLow:     result += kLowStr;     break;
    case kHaierAc160SwingVLowest:  result += kLowestStr;  break;
    default:                       result += kUnknownStr;
  }
  result += ')';
  result += addBoolToString(_.Sleep, kSleepStr);
  result += addBoolToString(getClean(), kCleanStr);
  const uint8_t tmode = getTimerMode();
  result += addIntToString(tmode, kTimerModeStr);
  result += kSpaceLBraceStr;
  switch (tmode) {
    case kHaierAcYrw02NoTimers:
      result += kNAStr;
      break;
    case kHaierAcYrw02OnTimer:
      result += kOnStr;
      break;
    case kHaierAcYrw02OffTimer:
      result += kOffStr;
      break;
    case kHaierAcYrw02OnThenOffTimer:
      result += kOnStr;
      result += '-';
      result += kOffStr;
      break;
    case kHaierAcYrw02OffThenOnTimer:
      result += kOffStr;
      result += '-';
      result += kOnStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addLabeledString((tmode != kHaierAcYrw02NoTimers &&
                              tmode != kHaierAcYrw02OffTimer) ?
      minsToString(getOnTimer()) : kOffStr, kOnTimerStr);
  result += addLabeledString((tmode != kHaierAcYrw02NoTimers &&
                              tmode != kHaierAcYrw02OnTimer) ?
      minsToString(getOffTimer()) : kOffStr, kOffTimerStr);
  result += addBoolToString(_.Lock, kLockStr);
  result += addBoolToString(_.AuxHeating, kHeatingStr);
  return result;
}
// End of IRHaierAC160 class.
