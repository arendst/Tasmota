// Copyright 2018 crankyoldgit
// The specifics of reverse engineering the protocols details:
// * HSU07-HEA03 by kuzin2006.
// * YR-W02/HSU-09HMC203 by non7top.

#include "ir_Haier.h"
#ifndef UNIT_TEST
#include <Arduino.h>
#else
#include <string>
#endif
#include "IRremoteESP8266.h"
#include "IRutils.h"

//                      HH   HH   AAA   IIIII EEEEEEE RRRRRR
//                      HH   HH  AAAAA   III  EE      RR   RR
//                      HHHHHHH AA   AA  III  EEEEE   RRRRRR
//                      HH   HH AAAAAAA  III  EE      RR  RR
//                      HH   HH AA   AA IIIII EEEEEEE RR   RR

// Supported devices:
//   * Haier HSU07-HEA03 Remote control.
//   * Haier YR-W02 Remote control
//   * Haier HSU-09HMC203 A/C unit.

// Ref:
//   https://github.com/markszabo/IRremoteESP8266/issues/404
//   https://www.dropbox.com/s/mecyib3lhdxc8c6/IR%20data%20reverse%20engineering.xlsx?dl=0
//   https://github.com/markszabo/IRremoteESP8266/issues/485
//   https://www.dropbox.com/sh/w0bt7egp0fjger5/AADRFV6Wg4wZskJVdFvzb8Z0a?dl=0&preview=haer2.ods

// Constants
const uint16_t kHaierAcHdr = 3000;
const uint16_t kHaierAcHdrGap = 4300;
const uint16_t kHaierAcBitMark = 520;
const uint16_t kHaierAcOneSpace = 1650;
const uint16_t kHaierAcZeroSpace = 650;
const uint32_t kHaierAcMinGap = 150000;  // Completely made up value.

#if (SEND_HAIER_AC || SEND_HAIER_AC_YRW02)
// Send a Haier A/C message. (HSU07-HEA03 remote)
//
// Args:
//   data: An array of bytes containing the IR command.
//   nbytes: Nr. of bytes of data in the array. (>=kHaierACStateLength)
//   repeat: Nr. of times the message is to be repeated. (Default = 0).
//
// Status: Beta / Probably working.
//
void IRsend::sendHaierAC(unsigned char data[], uint16_t nbytes,
                         uint16_t repeat) {
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
void IRsend::sendHaierACYRW02(unsigned char data[], uint16_t nbytes,
                              uint16_t repeat) {
  if (nbytes >= kHaierACYRW02StateLength) sendHaierAC(data, nbytes, repeat);
}
#endif  // SEND_HAIER_AC_YRW02

// Class for emulating a Haier HSU07-HEA03 remote
IRHaierAC::IRHaierAC(uint16_t pin) : _irsend(pin) { stateReset(); }

void IRHaierAC::begin() { _irsend.begin(); }

#if SEND_HAIER_AC
void IRHaierAC::send() {
  checksum();
  _irsend.sendHaierAC(remote_state);
}
#endif  // SEND_HAIER_AC

void IRHaierAC::checksum() {
  remote_state[8] = sumBytes(remote_state, kHaierACStateLength - 1);
}

bool IRHaierAC::validChecksum(uint8_t state[], const uint16_t length) {
  if (length < 2) return false;  // 1 byte of data can't have a checksum.
  return (state[length - 1] == sumBytes(state, length - 1));
}

void IRHaierAC::stateReset() {
  for (uint8_t i = 1; i < kHaierACStateLength; i++) remote_state[i] = 0x0;
  remote_state[0] = kHaierAcPrefix;
  remote_state[2] = 0b00100000;

  setTemp(kHaierAcDefTemp);
  setFan(kHaierAcFanAuto);
  setMode(kHaierAcAuto);
  setCommand(kHaierAcCmdOn);
}

uint8_t* IRHaierAC::getRaw() {
  checksum();
  return remote_state;
}

void IRHaierAC::setRaw(uint8_t new_code[]) {
  for (uint8_t i = 0; i < kHaierACStateLength; i++) {
    remote_state[i] = new_code[i];
  }
}

void IRHaierAC::setCommand(uint8_t state) {
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

uint8_t IRHaierAC::getCommand() { return remote_state[1] & (0b00001111); }

void IRHaierAC::setFan(uint8_t speed) {
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

uint8_t IRHaierAC::getFan() {
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
  remote_state[7] &= 0b00011111;
  remote_state[7] |= (new_mode << 5);
}

uint8_t IRHaierAC::getMode() { return (remote_state[7] & 0b11100000) >> 5; }

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

uint8_t IRHaierAC::getTemp() {
  return ((remote_state[1] & 0b11110000) >> 4) + kHaierAcMinTemp;
}

void IRHaierAC::setHealth(bool state) {
  setCommand(kHaierAcCmdHealth);
  remote_state[4] &= 0b11011111;
  remote_state[4] |= (state << 5);
}

bool IRHaierAC::getHealth(void) { return remote_state[4] & (1 << 5); }

void IRHaierAC::setSleep(bool state) {
  setCommand(kHaierAcCmdSleep);
  remote_state[7] &= 0b10111111;
  remote_state[7] |= (state << 6);
}

bool IRHaierAC::getSleep(void) { return remote_state[7] & 0b01000000; }

uint16_t IRHaierAC::getTime(const uint8_t ptr[]) {
  return (ptr[0] & 0b00011111) * 60 + (ptr[1] & 0b00111111);
}

int16_t IRHaierAC::getOnTimer() {
  if (remote_state[3] & 0b10000000)  // Check if the timer is turned on.
    return getTime(remote_state + 6);
  else
    return -1;
}

int16_t IRHaierAC::getOffTimer() {
  if (remote_state[3] & 0b01000000)  // Check if the timer is turned on.
    return getTime(remote_state + 4);
  else
    return -1;
}

uint16_t IRHaierAC::getCurrTime() { return getTime(remote_state + 2); }

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

void IRHaierAC::cancelTimers() {
  setCommand(kHaierAcCmdTimerCancel);
  remote_state[3] &= 0b00111111;
}

void IRHaierAC::setCurrTime(const uint16_t nr_mins) {
  setTime(remote_state + 2, nr_mins);
}

uint8_t IRHaierAC::getSwing() { return (remote_state[2] & 0b11000000) >> 6; }

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

// Convert a Haier time into a human readable string.
#ifdef ARDUINO
String IRHaierAC::timeToString(const uint16_t nr_mins) {
  String result = "";
#else
std::string IRHaierAC::timeToString(const uint16_t nr_mins) {
  std::string result = "";
#endif  // ARDUINO

  if (nr_mins / 24 < 10) result += "0";  // Zero pad.
  result += uint64ToString(nr_mins / 60);
  result += ":";
  if (nr_mins % 60 < 10) result += "0";  // Zero pad.
  result += uint64ToString(nr_mins % 60);
  return result;
}

// Convert the internal state into a human readable string.
#ifdef ARDUINO
String IRHaierAC::toString() {
  String result = "";
#else
std::string IRHaierAC::toString() {
  std::string result = "";
#endif  // ARDUINO
  uint8_t cmd = getCommand();
  result += "Command: " + uint64ToString(cmd) + " (";
  switch (cmd) {
    case kHaierAcCmdOff:
      result += "Off";
      break;
    case kHaierAcCmdOn:
      result += "On";
      break;
    case kHaierAcCmdMode:
      result += "Mode";
      break;
    case kHaierAcCmdFan:
      result += "Fan";
      break;
    case kHaierAcCmdTempUp:
      result += "Temp Up";
      break;
    case kHaierAcCmdTempDown:
      result += "Temp Down";
      break;
    case kHaierAcCmdSleep:
      result += "Sleep";
      break;
    case kHaierAcCmdTimerSet:
      result += "Timer Set";
      break;
    case kHaierAcCmdTimerCancel:
      result += "Timer Cancel";
      break;
    case kHaierAcCmdHealth:
      result += "Health";
      break;
    case kHaierAcCmdSwing:
      result += "Swing";
      break;
    default:
      result += "Unknown";
  }
  result += ")";
  result += ", Mode: " + uint64ToString(getMode());
  switch (getMode()) {
    case kHaierAcAuto:
      result += " (AUTO)";
      break;
    case kHaierAcCool:
      result += " (COOL)";
      break;
    case kHaierAcHeat:
      result += " (HEAT)";
      break;
    case kHaierAcDry:
      result += " (DRY)";
      break;
    case kHaierAcFan:
      result += " (FAN)";
      break;
    default:
      result += " (UNKNOWN)";
  }
  result += ", Temp: " + uint64ToString(getTemp()) + "C";
  result += ", Fan: " + uint64ToString(getFan());
  switch (getFan()) {
    case kHaierAcFanAuto:
      result += " (AUTO)";
      break;
    case kHaierAcFanHigh:
      result += " (MAX)";
      break;
  }
  result += ", Swing: " + uint64ToString(getSwing()) + " (";
  switch (getSwing()) {
    case kHaierAcSwingOff:
      result += "Off";
      break;
    case kHaierAcSwingUp:
      result += "Up";
      break;
    case kHaierAcSwingDown:
      result += "Down";
      break;
    case kHaierAcSwingChg:
      result += "Chg";
      break;
    default:
      result += "Unknown";
  }
  result += ")";
  result += ", Sleep: ";
  if (getSleep())
    result += "On";
  else
    result += "Off";
  result += ", Health: ";
  if (getHealth())
    result += "On";
  else
    result += "Off";
  result += ", Current Time: " + timeToString(getCurrTime());
  result += ", On Timer: ";
  if (getOnTimer() >= 0)
    result += timeToString(getOnTimer());
  else
    result += "Off";
  result += ", Off Timer: ";
  if (getOffTimer() >= 0)
    result += timeToString(getOffTimer());
  else
    result += "Off";

  return result;
}
// End of IRHaierAC class.

// Class for emulating a Haier YRW02 remote
IRHaierACYRW02::IRHaierACYRW02(uint16_t pin) : _irsend(pin) { stateReset(); }

void IRHaierACYRW02::begin() { _irsend.begin(); }

#if SEND_HAIER_AC_YRW02
void IRHaierACYRW02::send() {
  checksum();
  _irsend.sendHaierACYRW02(remote_state);
}
#endif  // SEND_HAIER_AC_YRW02

void IRHaierACYRW02::checksum() {
  remote_state[kHaierACYRW02StateLength - 1] =
      sumBytes(remote_state, kHaierACYRW02StateLength - 1);
}

bool IRHaierACYRW02::validChecksum(uint8_t state[], const uint16_t length) {
  if (length < 2) return false;  // 1 byte of data can't have a checksum.
  return (state[length - 1] == sumBytes(state, length - 1));
}

void IRHaierACYRW02::stateReset() {
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

uint8_t* IRHaierACYRW02::getRaw() {
  checksum();
  return remote_state;
}

void IRHaierACYRW02::setRaw(uint8_t new_code[]) {
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

uint8_t IRHaierACYRW02::getButton() { return remote_state[12] & (0b00001111); }

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

uint8_t IRHaierACYRW02::getMode() { return remote_state[7] >> 4; }

void IRHaierACYRW02::setTemp(const uint8_t celcius) {
  uint8_t temp = celcius;
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

uint8_t IRHaierACYRW02::getTemp() {
  return ((remote_state[1] & 0b11110000) >> 4) + kHaierAcMinTemp;
}

void IRHaierACYRW02::setHealth(bool state) {
  setButton(kHaierAcYrw02ButtonHealth);
  remote_state[3] &= 0b11111101;
  remote_state[3] |= (state << 1);
}

bool IRHaierACYRW02::getHealth(void) { return remote_state[3] & 0b00000010; }

bool IRHaierACYRW02::getPower() { return remote_state[4] & kHaierAcYrw02Power; }

void IRHaierACYRW02::setPower(bool state) {
  setButton(kHaierAcYrw02ButtonPower);
  if (state)
    remote_state[4] |= kHaierAcYrw02Power;
  else
    remote_state[4] &= ~kHaierAcYrw02Power;
}

void IRHaierACYRW02::on() { setPower(true); }

void IRHaierACYRW02::off() { setPower(false); }

bool IRHaierACYRW02::getSleep() { return remote_state[8] & kHaierAcYrw02Sleep; }

void IRHaierACYRW02::setSleep(bool state) {
  setButton(kHaierAcYrw02ButtonSleep);
  if (state)
    remote_state[8] |= kHaierAcYrw02Sleep;
  else
    remote_state[8] &= ~kHaierAcYrw02Sleep;
}

uint8_t IRHaierACYRW02::getTurbo() { return remote_state[6] >> 6; }

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

uint8_t IRHaierACYRW02::getFan() { return remote_state[5] >> 4; }

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

uint8_t IRHaierACYRW02::getSwing() { return remote_state[1] & 0b00001111; }

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

// Convert the internal state into a human readable string.
#ifdef ARDUINO
String IRHaierACYRW02::toString() {
  String result = "";
#else
std::string IRHaierACYRW02::toString() {
  std::string result = "";
#endif  // ARDUINO
  result += "Power: ";
  if (getPower())
    result += "On";
  else
    result += "Off";
  uint8_t cmd = getButton();
  result += ", Button: " + uint64ToString(cmd) + " (";
  switch (cmd) {
    case kHaierAcYrw02ButtonPower:
      result += "Power";
      break;
    case kHaierAcYrw02ButtonMode:
      result += "Mode";
      break;
    case kHaierAcYrw02ButtonFan:
      result += "Fan";
      break;
    case kHaierAcYrw02ButtonTempUp:
      result += "Temp Up";
      break;
    case kHaierAcYrw02ButtonTempDown:
      result += "Temp Down";
      break;
    case kHaierAcYrw02ButtonSleep:
      result += "Sleep";
      break;
    case kHaierAcYrw02ButtonHealth:
      result += "Health";
      break;
    case kHaierAcYrw02ButtonSwing:
      result += "Swing";
      break;
    case kHaierAcYrw02ButtonTurbo:
      result += "Turbo";
      break;
    default:
      result += "Unknown";
  }
  result += ")";
  result += ", Mode: " + uint64ToString(getMode());
  switch (getMode()) {
    case kHaierAcYrw02Auto:
      result += " (Auto)";
      break;
    case kHaierAcYrw02Cool:
      result += " (Cool)";
      break;
    case kHaierAcYrw02Heat:
      result += " (Heat)";
      break;
    case kHaierAcYrw02Dry:
      result += " (Dry)";
      break;
    case kHaierAcYrw02Fan:
      result += " (Fan)";
      break;
    default:
      result += " (UNKNOWN)";
  }
  result += ", Temp: " + uint64ToString(getTemp()) + "C";
  result += ", Fan: " + uint64ToString(getFan());
  switch (getFan()) {
    case kHaierAcYrw02FanAuto:
      result += " (Auto)";
      break;
    case kHaierAcYrw02FanHigh:
      result += " (High)";
      break;
    case kHaierAcYrw02FanLow:
      result += " (Low)";
      break;
    case kHaierAcYrw02FanMed:
      result += " (Med)";
      break;
    default:
      result += " (Unknown)";
  }
  result += ", Turbo: " + uint64ToString(getTurbo()) + " (";
  switch (getTurbo()) {
    case kHaierAcYrw02TurboOff:
      result += "Off";
      break;
    case kHaierAcYrw02TurboLow:
      result += "Low";
      break;
    case kHaierAcYrw02TurboHigh:
      result += "High";
      break;
    default:
      result += "Unknown";
  }
  result += ")";
  result += ", Swing: " + uint64ToString(getSwing()) + " (";
  switch (getSwing()) {
    case kHaierAcYrw02SwingOff:
      result += "Off";
      break;
    case kHaierAcYrw02SwingAuto:
      result += "Auto";
      break;
    case kHaierAcYrw02SwingBottom:
      result += "Bottom";
      break;
    case kHaierAcYrw02SwingDown:
      result += "Down";
      break;
    case kHaierAcYrw02SwingTop:
      result += "Top";
      break;
    case kHaierAcYrw02SwingMiddle:
      result += "Middle";
      break;
    default:
      result += "Unknown";
  }
  result += ")";
  result += ", Sleep: ";
  if (getSleep())
    result += "On";
  else
    result += "Off";
  result += ", Health: ";
  if (getHealth())
    result += "On";
  else
    result += "Off";

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
// Status: BETA / Appears to be working.
//
bool IRrecv::decodeHaierAC(decode_results* results, uint16_t nbits,
                           bool strict) {
  if (nbits % 8 != 0)  // nbits has to be a multiple of nr. of bits in a byte.
    return false;

  if (strict) {
    if (nbits != kHaierACBits)
      return false;  // Not strictly a HAIER_AC message.
  }

  if (results->rawlen < (2 * nbits + kHeader) + kFooter - 1)
    return false;  // Can't possibly be a valid HAIER_AC message.

  uint16_t offset = kStartOffset;

  // Header
  if (!matchMark(results->rawbuf[offset++], kHaierAcHdr)) return false;
  if (!matchSpace(results->rawbuf[offset++], kHaierAcHdr)) return false;
  if (!matchMark(results->rawbuf[offset++], kHaierAcHdr)) return false;
  if (!matchSpace(results->rawbuf[offset++], kHaierAcHdrGap)) return false;

  // Data
  for (uint16_t i = 0; i < nbits / 8; i++) {
    match_result_t data_result =
        matchData(&(results->rawbuf[offset]), 8, kHaierAcBitMark,
                  kHaierAcOneSpace, kHaierAcBitMark, kHaierAcZeroSpace);
    if (data_result.success == false) return false;
    offset += data_result.used;
    results->state[i] = (uint8_t)data_result.data;
  }

  // Footer
  if (!matchMark(results->rawbuf[offset++], kHaierAcBitMark)) return false;
  if (offset < results->rawlen &&
      !matchAtLeast(results->rawbuf[offset++], kHaierAcMinGap))
    return false;

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
