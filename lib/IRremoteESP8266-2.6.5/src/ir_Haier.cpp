// Copyright 2018 crankyoldgit
// Code to emulate Haier protocol compatible devices.
// The specifics of reverse engineering the protocols details:
// * HSU07-HEA03 by kuzin2006.
// * YR-W02/HSU-09HMC203 by non7top.

#include "ir_Haier.h"
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include "IRremoteESP8266.h"
#include "IRutils.h"

// Supported devices:
//   * Haier HSU07-HEA03 Remote control.
//   * Haier YR-W02 Remote control
//   * Haier HSU-09HMC203 A/C unit.

// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/404
//   https://www.dropbox.com/s/mecyib3lhdxc8c6/IR%20data%20reverse%20engineering.xlsx?dl=0
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/485
//   https://www.dropbox.com/sh/w0bt7egp0fjger5/AADRFV6Wg4wZskJVdFvzb8Z0a?dl=0&preview=haer2.ods

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

#if (SEND_HAIER_AC || SEND_HAIER_AC_YRW02)
// Send a Haier A/C message. (HSU07-HEA03 remote)
//
// Args:
//   data: An array of bytes containing the IR command.
//   nbytes: Nr. of bytes of data in the array. (>=kHaierACStateLength)
//   repeat: Nr. of times the message is to be repeated. (Default = 0).
//
// Status: STABLE / Known to be working.
//
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
// Send a Haier YR-W02 remote A/C message.
//
// Args:
//   data: An array of bytes containing the IR command.
//   nbytes: Nr. of bytes of data in the array. (>=kHaierACYRW02StateLength)
//   repeat: Nr. of times the message is to be repeated. (Default = 0).
//
// Status: Alpha / Untested on a real device.
//
void IRsend::sendHaierACYRW02(const unsigned char data[], const uint16_t nbytes,
                              const uint16_t repeat) {
  if (nbytes >= kHaierACYRW02StateLength) sendHaierAC(data, nbytes, repeat);
}
#endif  // SEND_HAIER_AC_YRW02

// Class for emulating a Haier HSU07-HEA03 remote
IRHaierAC::IRHaierAC(const uint16_t pin, const bool inverted,
                     const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

void IRHaierAC::begin(void) { _irsend.begin(); }

#if SEND_HAIER_AC
void IRHaierAC::send(const uint16_t repeat) {
  checksum();
  _irsend.sendHaierAC(remote_state, kHaierACStateLength, repeat);
}
#endif  // SEND_HAIER_AC

void IRHaierAC::checksum(void) {
  remote_state[8] = sumBytes(remote_state, kHaierACStateLength - 1);
}

bool IRHaierAC::validChecksum(uint8_t state[], const uint16_t length) {
  if (length < 2) return false;  // 1 byte of data can't have a checksum.
  return (state[length - 1] == sumBytes(state, length - 1));
}

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

uint8_t* IRHaierAC::getRaw(void) {
  checksum();
  return remote_state;
}

void IRHaierAC::setRaw(const uint8_t new_code[]) {
  for (uint8_t i = 0; i < kHaierACStateLength; i++) {
    remote_state[i] = new_code[i];
  }
}

void IRHaierAC::setCommand(const uint8_t state) {
  remote_state[1] &= 0b11110000;
  switch (state) {
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
      remote_state[1] |= (state & 0b00001111);
  }
}

uint8_t IRHaierAC::getCommand(void) { return remote_state[1] & (0b00001111); }

void IRHaierAC::setFan(const uint8_t speed) {
  uint8_t new_speed = kHaierAcFanAuto;
  switch (speed) {
    case kHaierAcFanLow:
      new_speed = 3;
      break;
    case kHaierAcFanMed:
      new_speed = 1;
      break;
    case kHaierAcFanHigh:
      new_speed = 2;
      break;
    default:
      new_speed = kHaierAcFanAuto;  // Default to auto for anything else.
  }

  if (speed != getFan()) setCommand(kHaierAcCmdFan);
  remote_state[5] &= 0b11111100;
  remote_state[5] |= new_speed;
}

uint8_t IRHaierAC::getFan(void) {
  switch (remote_state[5] & 0b00000011) {
    case 1:
      return kHaierAcFanMed;
    case 2:
      return kHaierAcFanHigh;
    case 3:
      return kHaierAcFanLow;
    default:
      return kHaierAcFanAuto;
  }
}

void IRHaierAC::setMode(uint8_t mode) {
  uint8_t new_mode = mode;
  setCommand(kHaierAcCmdMode);
  if (mode > kHaierAcFan)  // If out of range, default to auto mode.
    new_mode = kHaierAcAuto;
  remote_state[6] &= ~kHaierAcModeMask;
  remote_state[6] |= (new_mode << 5);
}

uint8_t IRHaierAC::getMode(void) {
  return (remote_state[6] & kHaierAcModeMask) >> 5;
}

void IRHaierAC::setTemp(const uint8_t celsius) {
  uint8_t temp = celsius;
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

  remote_state[1] &= 0b00001111;  // Clear the previous temp.
  remote_state[1] |= ((temp - kHaierAcMinTemp) << 4);
}

uint8_t IRHaierAC::getTemp(void) {
  return ((remote_state[1] & 0b11110000) >> 4) + kHaierAcMinTemp;
}

void IRHaierAC::setHealth(const bool on) {
  setCommand(kHaierAcCmdHealth);
  remote_state[4] &= 0b11011111;
  remote_state[4] |= (on << 5);
}

bool IRHaierAC::getHealth(void) { return remote_state[4] & (1 << 5); }

void IRHaierAC::setSleep(const bool on) {
  setCommand(kHaierAcCmdSleep);
  if (on)
    remote_state[7] |= kHaierAcSleepBit;
  else
    remote_state[7] &= ~kHaierAcSleepBit;
}

bool IRHaierAC::getSleep(void) { return remote_state[7] & kHaierAcSleepBit; }

uint16_t IRHaierAC::getTime(const uint8_t ptr[]) {
  return (ptr[0] & 0b00011111) * 60 + (ptr[1] & 0b00111111);
}

int16_t IRHaierAC::getOnTimer(void) {
  if (remote_state[3] & 0b10000000)  // Check if the timer is turned on.
    return getTime(remote_state + 6);
  else
    return -1;
}

int16_t IRHaierAC::getOffTimer(void) {
  if (remote_state[3] & 0b01000000)  // Check if the timer is turned on.
    return getTime(remote_state + 4);
  else
    return -1;
}

uint16_t IRHaierAC::getCurrTime(void) { return getTime(remote_state + 2); }

void IRHaierAC::setTime(uint8_t ptr[], const uint16_t nr_mins) {
  uint16_t mins = nr_mins;
  if (nr_mins > kHaierAcMaxTime) mins = kHaierAcMaxTime;

  // Hours
  ptr[0] &= 0b11100000;
  ptr[0] |= (mins / 60);
  // Minutes
  ptr[1] &= 0b11000000;
  ptr[1] |= (mins % 60);
}

void IRHaierAC::setOnTimer(const uint16_t nr_mins) {
  setCommand(kHaierAcCmdTimerSet);
  remote_state[3] |= 0b10000000;
  setTime(remote_state + 6, nr_mins);
}

void IRHaierAC::setOffTimer(const uint16_t nr_mins) {
  setCommand(kHaierAcCmdTimerSet);
  remote_state[3] |= 0b01000000;
  setTime(remote_state + 4, nr_mins);
}

void IRHaierAC::cancelTimers(void) {
  setCommand(kHaierAcCmdTimerCancel);
  remote_state[3] &= 0b00111111;
}

void IRHaierAC::setCurrTime(const uint16_t nr_mins) {
  setTime(remote_state + 2, nr_mins);
}

uint8_t IRHaierAC::getSwing(void) {
  return (remote_state[2] & 0b11000000) >> 6;
}

void IRHaierAC::setSwing(const uint8_t state) {
  if (state == getSwing()) return;  // Nothing to do.
  setCommand(kHaierAcCmdSwing);
  switch (state) {
    case kHaierAcSwingOff:
    case kHaierAcSwingUp:
    case kHaierAcSwingDown:
    case kHaierAcSwingChg:
      remote_state[2] &= 0b00111111;
      remote_state[2] |= (state << 6);
      break;
  }
}

// Convert a standard A/C mode into its native mode.
uint8_t IRHaierAC::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool:
      return kHaierAcCool;
    case stdAc::opmode_t::kHeat:
      return kHaierAcHeat;
    case stdAc::opmode_t::kDry:
      return kHaierAcDry;
    case stdAc::opmode_t::kFan:
      return kHaierAcFan;
    default:
      return kHaierAcAuto;
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRHaierAC::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:
      return kHaierAcFanLow;
    case stdAc::fanspeed_t::kMedium:
      return kHaierAcFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:
      return kHaierAcFanHigh;
    default:
      return kHaierAcFanAuto;
  }
}

// Convert a standard A/C vertical swing into its native setting.
uint8_t IRHaierAC::convertSwingV(const stdAc::swingv_t position) {
  switch (position) {
    case stdAc::swingv_t::kHighest:
    case stdAc::swingv_t::kHigh:
    case stdAc::swingv_t::kMiddle:
      return kHaierAcSwingUp;
    case stdAc::swingv_t::kLow:
    case stdAc::swingv_t::kLowest:
      return kHaierAcSwingDown;
    case stdAc::swingv_t::kOff:
      return kHaierAcSwingOff;
    default:
      return kHaierAcSwingChg;
  }
}

// Convert a native mode to it's common equivalent.
stdAc::opmode_t IRHaierAC::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kHaierAcCool: return stdAc::opmode_t::kCool;
    case kHaierAcHeat: return stdAc::opmode_t::kHeat;
    case kHaierAcDry: return stdAc::opmode_t::kDry;
    case kHaierAcFan: return stdAc::opmode_t::kFan;
    default: return stdAc::opmode_t::kAuto;
  }
}

// Convert a native fan speed to it's common equivalent.
stdAc::fanspeed_t IRHaierAC::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kHaierAcFanHigh: return stdAc::fanspeed_t::kMax;
    case kHaierAcFanMed: return stdAc::fanspeed_t::kMedium;
    case kHaierAcFanLow: return stdAc::fanspeed_t::kMin;
    default: return stdAc::fanspeed_t::kAuto;
  }
}

// Convert a native vertical swing to it's common equivalent.
stdAc::swingv_t IRHaierAC::toCommonSwingV(const uint8_t pos) {
  switch (pos) {
    case kHaierAcSwingUp: return stdAc::swingv_t::kHighest;
    case kHaierAcSwingDown: return stdAc::swingv_t::kLowest;
    case kHaierAcSwingOff: return stdAc::swingv_t::kOff;
    default: return stdAc::swingv_t::kAuto;
  }
}

// Convert the A/C state to it's common equivalent.
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

// Convert the internal state into a human readable string.
String IRHaierAC::toString(void) {
  String result = "";
  result.reserve(150);  // Reserve some heap for the string to reduce fragging.
  uint8_t cmd = getCommand();
  result += addIntToString(cmd, F("Command"), false);
  result += F(" (");
  switch (cmd) {
    case kHaierAcCmdOff:
      result += F("Off");
      break;
    case kHaierAcCmdOn:
      result += F("On");
      break;
    case kHaierAcCmdMode:
      result += F("Mode");
      break;
    case kHaierAcCmdFan:
      result += F("Fan");
      break;
    case kHaierAcCmdTempUp:
      result += F("Temp Up");
      break;
    case kHaierAcCmdTempDown:
      result += F("Temp Down");
      break;
    case kHaierAcCmdSleep:
      result += F("Sleep");
      break;
    case kHaierAcCmdTimerSet:
      result += F("Timer Set");
      break;
    case kHaierAcCmdTimerCancel:
      result += F("Timer Cancel");
      break;
    case kHaierAcCmdHealth:
      result += F("Health");
      break;
    case kHaierAcCmdSwing:
      result += F("Swing");
      break;
    default:
      result += F("Unknown");
  }
  result += ')';
  result += addModeToString(getMode(), kHaierAcAuto, kHaierAcCool, kHaierAcHeat,
                            kHaierAcDry, kHaierAcFan);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kHaierAcFanHigh, kHaierAcFanLow,
                           kHaierAcFanAuto, kHaierAcFanAuto, kHaierAcFanMed);
  result += addIntToString(getSwing(), F("Swing"));
  result += F(" (");
  switch (getSwing()) {
    case kHaierAcSwingOff:
      result += F("Off");
      break;
    case kHaierAcSwingUp:
      result += F("Up");
      break;
    case kHaierAcSwingDown:
      result += F("Down");
      break;
    case kHaierAcSwingChg:
      result += F("Chg");
      break;
    default:
      result += F("Unknown");
  }
  result += ')';
  result += addBoolToString(getSleep(), F("Sleep"));
  result += addBoolToString(getHealth(), F("Health"));
  result += addLabeledString(minsToString(getCurrTime()), F("Current Time"));
  result += addLabeledString(
      getOnTimer() >= 0 ? minsToString(getOnTimer()) : F("Off"), F("On Timer"));
  result += addLabeledString(
      getOffTimer() >= 0 ? minsToString(getOffTimer()) : F("Off"),
      F("Off Timer"));
  return result;
}
// End of IRHaierAC class.

// Class for emulating a Haier YRW02 remote
IRHaierACYRW02::IRHaierACYRW02(const uint16_t pin, const bool inverted,
                               const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

void IRHaierACYRW02::begin(void) { _irsend.begin(); }

#if SEND_HAIER_AC_YRW02
void IRHaierACYRW02::send(const uint16_t repeat) {
  checksum();
  _irsend.sendHaierACYRW02(remote_state, kHaierACYRW02StateLength, repeat);
}
#endif  // SEND_HAIER_AC_YRW02

void IRHaierACYRW02::checksum(void) {
  remote_state[kHaierACYRW02StateLength - 1] =
      sumBytes(remote_state, kHaierACYRW02StateLength - 1);
}

bool IRHaierACYRW02::validChecksum(uint8_t state[], const uint16_t length) {
  if (length < 2) return false;  // 1 byte of data can't have a checksum.
  return (state[length - 1] == sumBytes(state, length - 1));
}

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

uint8_t* IRHaierACYRW02::getRaw(void) {
  checksum();
  return remote_state;
}

void IRHaierACYRW02::setRaw(const uint8_t new_code[]) {
  for (uint8_t i = 0; i < kHaierACYRW02StateLength; i++) {
    remote_state[i] = new_code[i];
  }
}

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
      remote_state[12] &= 0b11110000;
      remote_state[12] |= (button & 0b00001111);
  }
}

uint8_t IRHaierACYRW02::getButton(void) {
  return remote_state[12] & 0b00001111;
}

void IRHaierACYRW02::setMode(uint8_t mode) {
  uint8_t new_mode = mode;
  setButton(kHaierAcYrw02ButtonMode);
  switch (mode) {
    case kHaierAcYrw02Auto:
    case kHaierAcYrw02Cool:
    case kHaierAcYrw02Dry:
    case kHaierAcYrw02Heat:
    case kHaierAcYrw02Fan:
      break;
    default:  // If unexpected, default to auto mode.
      new_mode = kHaierAcYrw02Auto;
  }
  remote_state[7] &= 0b0001111;
  remote_state[7] |= (new_mode << 4);
}

uint8_t IRHaierACYRW02::getMode(void) { return remote_state[7] >> 4; }

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

  remote_state[1] &= 0b00001111;  // Clear the previous temp.
  remote_state[1] |= ((temp - kHaierAcMinTemp) << 4);
}

uint8_t IRHaierACYRW02::getTemp(void) {
  return ((remote_state[1] & 0b11110000) >> 4) + kHaierAcMinTemp;
}

void IRHaierACYRW02::setHealth(const bool on) {
  setButton(kHaierAcYrw02ButtonHealth);
  remote_state[3] &= 0b11111101;
  remote_state[3] |= (on << 1);
}

bool IRHaierACYRW02::getHealth(void) { return remote_state[3] & 0b00000010; }

bool IRHaierACYRW02::getPower(void) {
  return remote_state[4] & kHaierAcYrw02Power;
}

void IRHaierACYRW02::setPower(const bool on) {
  setButton(kHaierAcYrw02ButtonPower);
  if (on)
    remote_state[4] |= kHaierAcYrw02Power;
  else
    remote_state[4] &= ~kHaierAcYrw02Power;
}

void IRHaierACYRW02::on(void) { setPower(true); }

void IRHaierACYRW02::off(void) { setPower(false); }

bool IRHaierACYRW02::getSleep(void) {
  return remote_state[8] & kHaierAcYrw02Sleep;
}

void IRHaierACYRW02::setSleep(const bool on) {
  setButton(kHaierAcYrw02ButtonSleep);
  if (on)
    remote_state[8] |= kHaierAcYrw02Sleep;
  else
    remote_state[8] &= ~kHaierAcYrw02Sleep;
}

uint8_t IRHaierACYRW02::getTurbo(void) { return remote_state[6] >> 6; }

void IRHaierACYRW02::setTurbo(uint8_t speed) {
  switch (speed) {
    case kHaierAcYrw02TurboOff:
    case kHaierAcYrw02TurboLow:
    case kHaierAcYrw02TurboHigh:
      remote_state[6] &= 0b00111111;
      remote_state[6] |= (speed << 6);
      setButton(kHaierAcYrw02ButtonTurbo);
  }
}

uint8_t IRHaierACYRW02::getFan(void) { return remote_state[5] >> 4; }

void IRHaierACYRW02::setFan(uint8_t speed) {
  switch (speed) {
    case kHaierAcYrw02FanLow:
    case kHaierAcYrw02FanMed:
    case kHaierAcYrw02FanHigh:
    case kHaierAcYrw02FanAuto:
      remote_state[5] &= 0b00001111;
      remote_state[5] |= (speed << 4);
      setButton(kHaierAcYrw02ButtonFan);
  }
}

uint8_t IRHaierACYRW02::getSwing(void) { return remote_state[1] & 0b00001111; }

void IRHaierACYRW02::setSwing(uint8_t state) {
  uint8_t newstate = state;
  switch (state) {
    case kHaierAcYrw02SwingOff:
    case kHaierAcYrw02SwingAuto:
    case kHaierAcYrw02SwingTop:
    case kHaierAcYrw02SwingMiddle:
    case kHaierAcYrw02SwingBottom:
    case kHaierAcYrw02SwingDown:
      setButton(kHaierAcYrw02ButtonSwing);
      break;
    default:
      return;  // Unexpected value so don't do anything.
  }

  // Heat mode has no MIDDLE setting, use BOTTOM instead.
  if (state == kHaierAcYrw02SwingMiddle && getMode() == kHaierAcYrw02Heat)
    newstate = kHaierAcYrw02SwingBottom;

  // BOTTOM is only allowed if we are in Heat mode, otherwise MIDDLE.
  if (state == kHaierAcYrw02SwingBottom && getMode() != kHaierAcYrw02Heat)
    newstate = kHaierAcYrw02SwingMiddle;

  remote_state[1] &= 0b11110000;
  remote_state[1] |= newstate;
}

// Convert a standard A/C mode into its native mode.
uint8_t IRHaierACYRW02::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool:
      return kHaierAcYrw02Cool;
    case stdAc::opmode_t::kHeat:
      return kHaierAcYrw02Heat;
    case stdAc::opmode_t::kDry:
      return kHaierAcYrw02Dry;
    case stdAc::opmode_t::kFan:
      return kHaierAcYrw02Fan;
    default:
      return kHaierAcYrw02Auto;
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRHaierACYRW02::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:
      return kHaierAcYrw02FanLow;
    case stdAc::fanspeed_t::kMedium:
      return kHaierAcYrw02FanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:
      return kHaierAcYrw02FanHigh;
    default:
      return kHaierAcYrw02FanAuto;
  }
}

// Convert a standard A/C vertical swing into its native setting.
uint8_t IRHaierACYRW02::convertSwingV(const stdAc::swingv_t position) {
  switch (position) {
    case stdAc::swingv_t::kHighest:
    case stdAc::swingv_t::kHigh:
      return kHaierAcYrw02SwingTop;
    case stdAc::swingv_t::kMiddle:
      return kHaierAcYrw02SwingMiddle;
    case stdAc::swingv_t::kLow:
      return kHaierAcYrw02SwingDown;
    case stdAc::swingv_t::kLowest:
      return kHaierAcYrw02SwingBottom;
    case stdAc::swingv_t::kOff:
      return kHaierAcYrw02SwingOff;
    default:
      return kHaierAcYrw02SwingAuto;
  }
}

// Convert a native mode to it's common equivalent.
stdAc::opmode_t IRHaierACYRW02::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kHaierAcYrw02Cool: return stdAc::opmode_t::kCool;
    case kHaierAcYrw02Heat: return stdAc::opmode_t::kHeat;
    case kHaierAcYrw02Dry: return stdAc::opmode_t::kDry;
    case kHaierAcYrw02Fan: return stdAc::opmode_t::kFan;
    default: return stdAc::opmode_t::kAuto;
  }
}

// Convert a native fan speed to it's common equivalent.
stdAc::fanspeed_t IRHaierACYRW02::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kHaierAcYrw02FanHigh: return stdAc::fanspeed_t::kMax;
    case kHaierAcYrw02FanMed: return stdAc::fanspeed_t::kMedium;
    case kHaierAcYrw02FanLow: return stdAc::fanspeed_t::kMin;
    default: return stdAc::fanspeed_t::kAuto;
  }
}

// Convert a native vertical swing to it's common equivalent.
stdAc::swingv_t IRHaierACYRW02::toCommonSwingV(const uint8_t pos) {
  switch (pos) {
    case kHaierAcYrw02SwingTop: return stdAc::swingv_t::kHighest;
    case kHaierAcYrw02SwingMiddle: return stdAc::swingv_t::kMiddle;
    case kHaierAcYrw02SwingDown: return stdAc::swingv_t::kLow;
    case kHaierAcYrw02SwingBottom: return stdAc::swingv_t::kLowest;
    case kHaierAcYrw02SwingOff: return stdAc::swingv_t::kOff;
    default: return stdAc::swingv_t::kAuto;
  }
}

// Convert the A/C state to it's common equivalent.
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

// Convert the internal state into a human readable string.
String IRHaierACYRW02::toString(void) {
  String result = "";
  result.reserve(130);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), F("Power"), false);
  uint8_t cmd = getButton();
  result += addIntToString(cmd, F("Button"));
  result += F(" (");
  switch (cmd) {
    case kHaierAcYrw02ButtonPower:
      result += F("Power");
      break;
    case kHaierAcYrw02ButtonMode:
      result += F("Mode");
      break;
    case kHaierAcYrw02ButtonFan:
      result += F("Fan");
      break;
    case kHaierAcYrw02ButtonTempUp:
      result += F("Temp Up");
      break;
    case kHaierAcYrw02ButtonTempDown:
      result += F("Temp Down");
      break;
    case kHaierAcYrw02ButtonSleep:
      result += F("Sleep");
      break;
    case kHaierAcYrw02ButtonHealth:
      result += "Health";
      break;
    case kHaierAcYrw02ButtonSwing:
      result += F("Swing");
      break;
    case kHaierAcYrw02ButtonTurbo:
      result += F("Turbo");
      break;
    default:
      result += F("Unknown");
  }
  result += ')';
  result += addModeToString(getMode(), kHaierAcYrw02Auto, kHaierAcYrw02Cool,
                            kHaierAcYrw02Heat, kHaierAcYrw02Dry,
                            kHaierAcYrw02Fan);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kHaierAcYrw02FanHigh, kHaierAcYrw02FanLow,
                           kHaierAcYrw02FanAuto, kHaierAcYrw02FanAuto,
                           kHaierAcYrw02FanMed);
  result += addIntToString(getTurbo(), F("Turbo"));
  result += F(" (");
  switch (getTurbo()) {
    case kHaierAcYrw02TurboOff:
      result += F("Off");
      break;
    case kHaierAcYrw02TurboLow:
      result += F("Low");
      break;
    case kHaierAcYrw02TurboHigh:
      result += F("High");
      break;
    default:
      result += F("Unknown");
  }
  result += ')';
  result += addIntToString(getSwing(), F("Swing"));
  result += F(" (");
  switch (getSwing()) {
    case kHaierAcYrw02SwingOff:
      result += F("Off");
      break;
    case kHaierAcYrw02SwingAuto:
      result += F("Auto");
      break;
    case kHaierAcYrw02SwingBottom:
      result += F("Bottom");
      break;
    case kHaierAcYrw02SwingDown:
      result += F("Down");
      break;
    case kHaierAcYrw02SwingTop:
      result += F("Top");
      break;
    case kHaierAcYrw02SwingMiddle:
      result += F("Middle");
      break;
    default:
      result += F("Unknown");
  }
  result += ')';
  result += addBoolToString(getSleep(), F("Sleep"));
  result += addBoolToString(getHealth(), F("Health"));
  return result;
}
// End of IRHaierACYRW02 class.

#if (DECODE_HAIER_AC || DECODE_HAIER_AC_YRW02)
// Decode the supplied Haier HSU07-HEA03 remote message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect. Typically kHaierACBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: STABLE / Known to be working.
//
bool IRrecv::decodeHaierAC(decode_results* results, uint16_t nbits,
                           bool strict) {
  if (strict) {
    if (nbits != kHaierACBits)
      return false;  // Not strictly a HAIER_AC message.
  }

  if (results->rawlen < (2 * nbits + kHeader) + kFooter - 1)
    return false;  // Can't possibly be a valid HAIER_AC message.

  uint16_t offset = kStartOffset;

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
// Decode the supplied Haier YR-W02 remote A/C message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect. Typically kHaierACYRW02Bits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: BETA / Appears to be working.
//
bool IRrecv::decodeHaierACYRW02(decode_results* results, uint16_t nbits,
                                bool strict) {
  if (strict) {
    if (nbits != kHaierACYRW02Bits)
      return false;  // Not strictly a HAIER_AC_YRW02 message.
  }

  // The protocol is almost exactly the same as HAIER_AC
  if (!decodeHaierAC(results, nbits, false)) return false;

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
