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
using irutils::setBit;
using irutils::setBits;

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
  remote_state[8] = sumBytes(remote_state, kHaierACStateLength - 1);
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
  for (uint8_t i = 1; i < kHaierACStateLength; i++) remote_state[i] = 0x0;
  remote_state[0] = kHaierAcPrefix;
  remote_state[2] = 0x20;
  remote_state[4] = 0x0C;
  remote_state[5] = 0xC0;

  setTemp(kHaierAcDefTemp);
  setFan(kHaierAcFanAuto);
  setMode(kHaierAcAuto);
  setCommand(kHaierAcCmdOn);
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t* IRHaierAC::getRaw(void) {
  checksum();
  return remote_state;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
void IRHaierAC::setRaw(const uint8_t new_code[]) {
  memcpy(remote_state, new_code, kHaierACStateLength);
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
      setBits(&remote_state[1], kLowNibble, kNibbleSize, command);
  }
}

/// Get the Command/Button setting of the A/C.
/// @return The value of the command/button that was pressed.
uint8_t IRHaierAC::getCommand(void) {
  return GETBITS8(remote_state[1], kLowNibble, kNibbleSize);
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRHaierAC::setFan(const uint8_t speed) {
  uint8_t new_speed = kHaierAcFanAuto;
  switch (speed) {
    case kHaierAcFanLow:  new_speed = 3; break;
    case kHaierAcFanMed:  new_speed = 1; break;
    case kHaierAcFanHigh: new_speed = 2; break;
    // Default to auto for anything else.
    default:              new_speed = kHaierAcFanAuto;
  }

  if (speed != getFan()) setCommand(kHaierAcCmdFan);
  setBits(&remote_state[5], kLowNibble, kHaierAcSwingSize, new_speed);
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRHaierAC::getFan(void) {
  switch (GETBITS8(remote_state[5], kLowNibble, kHaierAcSwingSize)) {
    case 1:  return kHaierAcFanMed;
    case 2:  return kHaierAcFanHigh;
    case 3:  return kHaierAcFanLow;
    default: return kHaierAcFanAuto;
  }
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRHaierAC::setMode(uint8_t mode) {
  uint8_t new_mode = mode;
  setCommand(kHaierAcCmdMode);
  // If out of range, default to auto mode.
  if (mode > kHaierAcFan) new_mode = kHaierAcAuto;
  setBits(&remote_state[6], kHaierAcModeOffset, kModeBitsSize, new_mode);
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRHaierAC::getMode(void) {
  return GETBITS8(remote_state[6], kHaierAcModeOffset, kModeBitsSize);
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
    setCommand(kHaierAcCmdTempDown);
  else
    setCommand(kHaierAcCmdTempUp);
  setBits(&remote_state[1], kHighNibble, kNibbleSize, temp - kHaierAcMinTemp);
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRHaierAC::getTemp(void) {
  return GETBITS8(remote_state[1], kHighNibble, kNibbleSize) + kHaierAcMinTemp;
}

/// Set the Health (filter) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHaierAC::setHealth(const bool on) {
  setCommand(kHaierAcCmdHealth);
  setBit(&remote_state[4], kHaierAcHealthBitOffset, on);
}

/// Get the Health (filter) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHaierAC::getHealth(void) {
  return GETBIT8(remote_state[4], kHaierAcHealthBitOffset);
}

/// Set the Sleep setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHaierAC::setSleep(const bool on) {
  setCommand(kHaierAcCmdSleep);
  setBit(&remote_state[7], kHaierAcSleepBitOffset, on);
}

/// Get the Sleep setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHaierAC::getSleep(void) {
  return GETBIT8(remote_state[7], kHaierAcSleepBitOffset);
}

/// Get the Time value at the given pointer.
/// @param[in] ptr A Ptr to a location in the internal state to get the time.
uint16_t IRHaierAC::getTime(const uint8_t ptr[]) {
  return GETBITS8(ptr[0], kHaierAcTimeOffset, kHaierAcHoursSize) * 60 +
         GETBITS8(ptr[1], kHaierAcTimeOffset, kHaierAcMinsSize);
}

/// Get the On Timer value/setting of the A/C.
/// @return Nr of minutes the timer is set to. -1 is Off/not set etc.
int16_t IRHaierAC::getOnTimer(void) {
  // Check if the timer is turned on.
  if (GETBIT8(remote_state[3], kHaierAcOnTimerOffset))
    return getTime(remote_state + 6);
  else
    return -1;
}

/// Get the Off Timer value/setting of the A/C.
/// @return Nr of minutes the timer is set to. -1 is Off/not set etc.
int16_t IRHaierAC::getOffTimer(void) {
  // Check if the timer is turned on.
  if (GETBIT8(remote_state[3], kHaierAcOffTimerOffset))
    return getTime(remote_state + 4);
  else
    return -1;
}

/// Get the clock value of the A/C.
/// @return The clock time, in Nr of minutes past midnight.
uint16_t IRHaierAC::getCurrTime(void) { return getTime(remote_state + 2); }

/// Set the Time value at the given pointer.
/// @param[out] ptr A Ptr to a location in the internal state to set the time.
/// @param[in] nr_mins The time expressed in total number of minutes.
void IRHaierAC::setTime(uint8_t ptr[], const uint16_t nr_mins) {
  uint16_t mins = nr_mins;
  if (nr_mins > kHaierAcMaxTime) mins = kHaierAcMaxTime;
  setBits(ptr, kHaierAcTimeOffset, kHaierAcHoursSize, mins / 60);  // Hours
  setBits(ptr + 1, kHaierAcTimeOffset, kHaierAcMinsSize, mins % 60);  // Minutes
}

/// Set & enable the On Timer.
/// @param[in] nr_mins The time expressed in total number of minutes.
void IRHaierAC::setOnTimer(const uint16_t nr_mins) {
  setCommand(kHaierAcCmdTimerSet);
  setBit(&remote_state[3], kHaierAcOnTimerOffset);
  setTime(remote_state + 6, nr_mins);
}

/// Set & enable the Off Timer.
/// @param[in] nr_mins The time expressed in total number of minutes.
void IRHaierAC::setOffTimer(const uint16_t nr_mins) {
  setCommand(kHaierAcCmdTimerSet);
  setBit(&remote_state[3], kHaierAcOffTimerOffset);
  setTime(remote_state + 4, nr_mins);
}

/// Cancel/disable the On & Off timers.
void IRHaierAC::cancelTimers(void) {
  setCommand(kHaierAcCmdTimerCancel);
  setBits(&remote_state[3], kHaierAcOffTimerOffset, 2, 0);
}

/// Set the clock value for the A/C.
/// @param[in] nr_mins The clock time, in Nr of minutes past midnight.
void IRHaierAC::setCurrTime(const uint16_t nr_mins) {
  setTime(remote_state + 2, nr_mins);
}

/// Get the Vertical Swing position setting of the A/C.
/// @return The native swing mode.
uint8_t IRHaierAC::getSwing(void) {
  return GETBITS8(remote_state[2], kHaierAcSwingOffset, kHaierAcSwingSize);
}

/// Set the Vertical Swing mode of the A/C.
/// @param[in] cmd The mode to set the vanes to.
void IRHaierAC::setSwing(const uint8_t cmd) {
  if (cmd == getSwing()) return;  // Nothing to do.
  switch (cmd) {
    case kHaierAcSwingOff:
    case kHaierAcSwingUp:
    case kHaierAcSwingDown:
    case kHaierAcSwingChg:
      setCommand(kHaierAcCmdSwing);
      setBits(&remote_state[2], kHaierAcSwingOffset, kHaierAcSwingSize, cmd);
      break;
  }
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivilant of the enum.
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
/// @return The native equivilant of the enum.
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
/// @return The native equivilant of the enum.
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

/// Convert a native mode into its stdAc equivilant.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivilant of the native setting.
stdAc::opmode_t IRHaierAC::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kHaierAcCool: return stdAc::opmode_t::kCool;
    case kHaierAcHeat: return stdAc::opmode_t::kHeat;
    case kHaierAcDry:  return stdAc::opmode_t::kDry;
    case kHaierAcFan:  return stdAc::opmode_t::kFan;
    default:           return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivilant.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivilant of the native setting.
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
/// @return The native equivilant of the enum.
stdAc::swingv_t IRHaierAC::toCommonSwingV(const uint8_t pos) {
  switch (pos) {
    case kHaierAcSwingUp:   return stdAc::swingv_t::kHighest;
    case kHaierAcSwingDown: return stdAc::swingv_t::kLowest;
    case kHaierAcSwingOff:  return stdAc::swingv_t::kOff;
    default:                return stdAc::swingv_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivilant.
/// @return The stdAc equivilant of the native settings.
stdAc::state_t IRHaierAC::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::HAIER_AC;
  result.model = -1;  // No models used.
  result.power = true;
  if (this->getCommand() == kHaierAcCmdOff) result.power = false;
  result.mode = this->toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = this->toCommonFanSpeed(this->getFan());
  result.swingv = this->toCommonSwingV(this->getSwing());
  result.filter = this->getHealth();
  result.sleep = this->getSleep() ? 0 : -1;
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
String IRHaierAC::toString(void) {
  String result = "";
  result.reserve(150);  // Reserve some heap for the string to reduce fragging.
  uint8_t cmd = getCommand();
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
  result += addModeToString(getMode(), kHaierAcAuto, kHaierAcCool, kHaierAcHeat,
                            kHaierAcDry, kHaierAcFan);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kHaierAcFanHigh, kHaierAcFanLow,
                           kHaierAcFanAuto, kHaierAcFanAuto, kHaierAcFanMed);
  result += addIntToString(getSwing(), kSwingStr);
  result += kSpaceLBraceStr;
  switch (getSwing()) {
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
  result += addBoolToString(getSleep(), kSleepStr);
  result += addBoolToString(getHealth(), kHealthStr);
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
  remote_state[kHaierACYRW02StateLength - 1] =
      sumBytes(remote_state, kHaierACYRW02StateLength - 1);
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
  for (uint8_t i = 1; i < kHaierACYRW02StateLength; i++) remote_state[i] = 0x0;
  remote_state[0] = kHaierAcYrw02Prefix;

  setTemp(kHaierAcDefTemp);
  setHealth(true);
  setTurbo(kHaierAcYrw02TurboOff);
  setSleep(false);
  setFan(kHaierAcYrw02FanAuto);
  setSwing(kHaierAcYrw02SwingOff);
  setMode(kHaierAcYrw02Auto);
  setPower(true);
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t* IRHaierACYRW02::getRaw(void) {
  checksum();
  return remote_state;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
void IRHaierACYRW02::setRaw(const uint8_t new_code[]) {
  memcpy(remote_state, new_code, kHaierACYRW02StateLength);
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
      setBits(&remote_state[12], kLowNibble, kNibbleSize, button);
  }
}

/// Get the Button/Command setting of the A/C.
/// @return The value of the button/command that was pressed.
uint8_t IRHaierACYRW02::getButton(void) {
  return GETBITS8(remote_state[12], kLowNibble, kNibbleSize);
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRHaierACYRW02::setMode(uint8_t mode) {
  uint8_t new_mode = mode;
  setButton(kHaierAcYrw02ButtonMode);
  switch (mode) {
    case kHaierAcYrw02Auto:
    case kHaierAcYrw02Cool:
    case kHaierAcYrw02Dry:
    case kHaierAcYrw02Heat:
    case kHaierAcYrw02Fan: break;
    default: new_mode = kHaierAcYrw02Auto;  // Unexpected, default to auto mode.
  }
  setBits(&remote_state[7], kHaierAcYrw02ModeOffset, kModeBitsSize, new_mode);
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRHaierACYRW02::getMode(void) {
  return GETBITS8(remote_state[7], kHaierAcYrw02ModeOffset, kModeBitsSize);
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
    setButton(kHaierAcYrw02ButtonTempDown);
  else
    setButton(kHaierAcYrw02ButtonTempUp);
  setBits(&remote_state[1], kHighNibble, kNibbleSize, temp - kHaierAcMinTemp);
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRHaierACYRW02::getTemp(void) {
  return GETBITS8(remote_state[1], kHighNibble, kNibbleSize) + kHaierAcMinTemp;
}

/// Set the Health (filter) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHaierACYRW02::setHealth(const bool on) {
  setButton(kHaierAcYrw02ButtonHealth);
  setBit(&remote_state[3], kHaierAcYrw02HealthOffset, on);
}

/// Get the Health (filter) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHaierACYRW02::getHealth(void) {
  return GETBIT8(remote_state[3], kHaierAcYrw02HealthOffset);
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRHaierACYRW02::getPower(void) {
  return GETBIT8(remote_state[4], kHaierAcYrw02PowerOffset);
}

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHaierACYRW02::setPower(const bool on) {
  setButton(kHaierAcYrw02ButtonPower);
  setBit(&remote_state[4], kHaierAcYrw02PowerOffset, on);
}

/// Change the power setting to On.
void IRHaierACYRW02::on(void) { setPower(true); }

/// Change the power setting to Off.
void IRHaierACYRW02::off(void) { setPower(false); }

/// Get the Sleep setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHaierACYRW02::getSleep(void) {
  return GETBIT8(remote_state[8], kHaierAcYrw02SleepOffset);
}

/// Set the Sleep setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHaierACYRW02::setSleep(const bool on) {
  setButton(kHaierAcYrw02ButtonSleep);
  setBit(&remote_state[8], kHaierAcYrw02SleepOffset, on);
}

/// Get the Turbo setting of the A/C.
/// @return The current turbo speed setting.
uint8_t IRHaierACYRW02::getTurbo(void) {
  return GETBITS8(remote_state[6], kHaierAcYrw02TurboOffset,
                  kHaierAcYrw02TurboSize);
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
      setBits(&remote_state[6], kHaierAcYrw02TurboOffset,
              kHaierAcYrw02TurboSize, speed);
      setButton(kHaierAcYrw02ButtonTurbo);
  }
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRHaierACYRW02::getFan(void) {
  return GETBITS8(remote_state[5], kHaierAcYrw02FanOffset,
                  kHaierAcYrw02FanSize);
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRHaierACYRW02::setFan(uint8_t speed) {
  switch (speed) {
    case kHaierAcYrw02FanLow:
    case kHaierAcYrw02FanMed:
    case kHaierAcYrw02FanHigh:
    case kHaierAcYrw02FanAuto:
      setBits(&remote_state[5], kHaierAcYrw02FanOffset, kHaierAcYrw02FanSize,
              speed);
      setButton(kHaierAcYrw02ButtonFan);
  }
}

/// Get the Vertical Swing position setting of the A/C.
/// @return The native position/mode.
uint8_t IRHaierACYRW02::getSwing(void) {
  return GETBITS8(remote_state[1], kLowNibble, kNibbleSize);
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
    case kHaierAcYrw02SwingDown: setButton(kHaierAcYrw02ButtonSwing); break;
    default: return;  // Unexpected value so don't do anything.
  }
  // Heat mode has no MIDDLE setting, use BOTTOM instead.
  if (pos == kHaierAcYrw02SwingMiddle && getMode() == kHaierAcYrw02Heat)
    newpos = kHaierAcYrw02SwingBottom;
  // BOTTOM is only allowed if we are in Heat mode, otherwise MIDDLE.
  if (pos == kHaierAcYrw02SwingBottom && getMode() != kHaierAcYrw02Heat)
    newpos = kHaierAcYrw02SwingMiddle;
  setBits(&remote_state[1], kLowNibble, kNibbleSize, newpos);
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivilant of the enum.
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
/// @return The native equivilant of the enum.
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
/// @return The native equivilant of the enum.
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

/// Convert a native mode into its stdAc equivilant.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivilant of the native setting.
stdAc::opmode_t IRHaierACYRW02::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kHaierAcYrw02Cool: return stdAc::opmode_t::kCool;
    case kHaierAcYrw02Heat: return stdAc::opmode_t::kHeat;
    case kHaierAcYrw02Dry:  return stdAc::opmode_t::kDry;
    case kHaierAcYrw02Fan:  return stdAc::opmode_t::kFan;
    default:                return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivilant.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivilant of the native setting.
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
/// @return The native equivilant of the enum.
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

/// Convert the current internal state into its stdAc::state_t equivilant.
/// @return The stdAc equivilant of the native settings.
stdAc::state_t IRHaierACYRW02::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::HAIER_AC_YRW02;
  result.model = -1;  // No models used.
  result.power = this->getPower();
  result.mode = this->toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = this->toCommonFanSpeed(this->getFan());
  result.swingv = this->toCommonSwingV(this->getSwing());
  result.filter = this->getHealth();
  result.sleep = this->getSleep() ? 0 : -1;
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
String IRHaierACYRW02::toString(void) {
  String result = "";
  result.reserve(130);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  uint8_t cmd = getButton();
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
  result += addModeToString(getMode(), kHaierAcYrw02Auto, kHaierAcYrw02Cool,
                            kHaierAcYrw02Heat, kHaierAcYrw02Dry,
                            kHaierAcYrw02Fan);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kHaierAcYrw02FanHigh, kHaierAcYrw02FanLow,
                           kHaierAcYrw02FanAuto, kHaierAcYrw02FanAuto,
                           kHaierAcYrw02FanMed);
  result += addIntToString(getTurbo(), kTurboStr);
  result += kSpaceLBraceStr;
  switch (getTurbo()) {
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
  result += addIntToString(getSwing(), kSwingStr);
  result += kSpaceLBraceStr;
  switch (getSwing()) {
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
  result += addBoolToString(getSleep(), kSleepStr);
  result += addBoolToString(getHealth(), kHealthStr);
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
