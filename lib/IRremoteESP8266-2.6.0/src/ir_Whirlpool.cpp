// Copyright 2018 David Conran
//
// Code to emulate Whirlpool protocol compatible devices.
// Should be compatible with:
// * SPIS409L, SPIS412L, SPIW409L, SPIW412L, SPIW418L
// Remotes:
// * DG11J1-3A / DG11J1-04
// * DG11J1-91
//
// Note: Smart, iFeel, AroundU, PowerSave, & Silent modes are unsupported.
//       Advanced 6thSense, Dehumidify, & Sleep modes are not supported.
//       FYI:
//         Dim == !Light
//         Jet == Super == Turbo
//

#include "ir_Whirlpool.h"
#include <algorithm>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "IRutils.h"

//    WW      WW HH   HH IIIII RRRRRR  LL      PPPPPP   OOOOO   OOOOO  LL
//    WW      WW HH   HH  III  RR   RR LL      PP   PP OO   OO OO   OO LL
//    WW   W  WW HHHHHHH  III  RRRRRR  LL      PPPPPP  OO   OO OO   OO LL
//     WW WWW WW HH   HH  III  RR  RR  LL      PP      OO   OO OO   OO LL
//      WW   WW  HH   HH IIIII RR   RR LLLLLLL PP       OOOO0   OOOO0  LLLLLLL

// Constants
// Ref: https://github.com/markszabo/IRremoteESP8266/issues/509
const uint16_t kWhirlpoolAcHdrMark = 8950;
const uint16_t kWhirlpoolAcHdrSpace = 4484;
const uint16_t kWhirlpoolAcBitMark = 597;
const uint16_t kWhirlpoolAcOneSpace = 1649;
const uint16_t kWhirlpoolAcZeroSpace = 533;
const uint16_t kWhirlpoolAcGap = 7920;
const uint32_t kWhirlpoolAcMinGap = kDefaultMessageGap;  // Just a guess.
const uint8_t kWhirlpoolAcSections = 3;

#if SEND_WHIRLPOOL_AC
// Send a Whirlpool A/C message.
//
// Args:
//   data: An array of bytes containing the IR command.
//   nbytes: Nr. of bytes of data in the array. (>=kWhirlpoolAcStateLength)
//   repeat: Nr. of times the message is to be repeated. (Default = 0).
//
// Status: ALPHA / Untested.
//
// Ref:
//   https://github.com/markszabo/IRremoteESP8266/issues/509
void IRsend::sendWhirlpoolAC(unsigned char data[], uint16_t nbytes,
                             uint16_t repeat) {
  if (nbytes < kWhirlpoolAcStateLength)
    return;  // Not enough bytes to send a proper message.
  for (uint16_t r = 0; r <= repeat; r++) {
    // Section 1
    sendGeneric(kWhirlpoolAcHdrMark, kWhirlpoolAcHdrSpace, kWhirlpoolAcBitMark,
                kWhirlpoolAcOneSpace, kWhirlpoolAcBitMark,
                kWhirlpoolAcZeroSpace, kWhirlpoolAcBitMark, kWhirlpoolAcGap,
                data, 6,  // 6 bytes == 48 bits
                38000,    // Complete guess of the modulation frequency.
                false, 0, 50);
    // Section 2
    sendGeneric(0, 0, kWhirlpoolAcBitMark, kWhirlpoolAcOneSpace,
                kWhirlpoolAcBitMark, kWhirlpoolAcZeroSpace, kWhirlpoolAcBitMark,
                kWhirlpoolAcGap, data + 6, 8,  // 8 bytes == 64 bits
                38000,  // Complete guess of the modulation frequency.
                false, 0, 50);
    // Section 3
    sendGeneric(0, 0, kWhirlpoolAcBitMark, kWhirlpoolAcOneSpace,
                kWhirlpoolAcBitMark, kWhirlpoolAcZeroSpace, kWhirlpoolAcBitMark,
                kWhirlpoolAcMinGap, data + 14, 7,  // 7 bytes == 56 bits
                38000,  // Complete guess of the modulation frequency.
                false, 0, 50);
  }
}
#endif  // SEND_WHIRLPOOL_AC

// Class for emulating a Whirlpool A/C remote.
// Decoding help from:
//   @redmusicxd, @josh929800, @raducostea

IRWhirlpoolAc::IRWhirlpoolAc(uint16_t pin) : _irsend(pin) { stateReset(); }

void IRWhirlpoolAc::stateReset() {
  for (uint8_t i = 2; i < kWhirlpoolAcStateLength; i++) remote_state[i] = 0x0;
  remote_state[0] = 0x83;
  remote_state[1] = 0x06;
  remote_state[6] = 0x80;
  _setTemp(kWhirlpoolAcAutoTemp);  // Default to a sane value.
}

void IRWhirlpoolAc::begin() { _irsend.begin(); }

bool IRWhirlpoolAc::validChecksum(uint8_t state[], const uint16_t length) {
  if (length > kWhirlpoolAcChecksumByte1 &&
      state[kWhirlpoolAcChecksumByte1] !=
          xorBytes(state + 2, kWhirlpoolAcChecksumByte1 - 1 - 2)) {
    DPRINTLN("DEBUG: First Whirlpool AC checksum failed.");
    return false;
  }
  if (length > kWhirlpoolAcChecksumByte2 &&
      state[kWhirlpoolAcChecksumByte2] !=
          xorBytes(state + kWhirlpoolAcChecksumByte1 + 1,
                   kWhirlpoolAcChecksumByte2 - kWhirlpoolAcChecksumByte1 - 1)) {
    DPRINTLN("DEBUG: Second Whirlpool AC checksum failed.");
    return false;
  }
  // State is too short to have a checksum or everything checked out.
  return true;
}

// Update the checksum for the internal state.
void IRWhirlpoolAc::checksum(uint16_t length) {
  if (length >= kWhirlpoolAcChecksumByte1)
    remote_state[kWhirlpoolAcChecksumByte1] =
        xorBytes(remote_state + 2, kWhirlpoolAcChecksumByte1 - 1 - 2);
  if (length >= kWhirlpoolAcChecksumByte2)
    remote_state[kWhirlpoolAcChecksumByte2] =
        xorBytes(remote_state + kWhirlpoolAcChecksumByte1 + 1,
                 kWhirlpoolAcChecksumByte2 - kWhirlpoolAcChecksumByte1 - 1);
}

#if SEND_WHIRLPOOL_AC
void IRWhirlpoolAc::send(const uint16_t repeat, const bool calcchecksum) {
  if (calcchecksum) checksum();
  _irsend.sendWhirlpoolAC(remote_state, kWhirlpoolAcStateLength, repeat);
}
#endif  // SEND_WHIRLPOOL_AC

uint8_t *IRWhirlpoolAc::getRaw(const bool calcchecksum) {
  if (calcchecksum) checksum();
  return remote_state;
}

void IRWhirlpoolAc::setRaw(const uint8_t new_code[], const uint16_t length) {
  for (uint8_t i = 0; i < length && i < kWhirlpoolAcStateLength; i++)
    remote_state[i] = new_code[i];
}

whirlpool_ac_remote_model_t IRWhirlpoolAc::getModel() {
  if (remote_state[kWhirlpoolAcAltTempPos] & kWhirlpoolAcAltTempMask)
    return DG11J191;
  else
    return DG11J13A;
}

void IRWhirlpoolAc::setModel(const whirlpool_ac_remote_model_t model) {
  switch (model) {
    case DG11J191:
      remote_state[kWhirlpoolAcAltTempPos] |= kWhirlpoolAcAltTempMask;
      break;
    case DG11J13A:
      // FALL THRU
    default:
      remote_state[kWhirlpoolAcAltTempPos] &= ~kWhirlpoolAcAltTempMask;
  }
  _setTemp(_desiredtemp);  // Different models have different temp values.
}

// Return the temp. offset in deg C for the current model.
int8_t IRWhirlpoolAc::getTempOffset() {
  switch (getModel()) {
    case DG11J191:
      return -2;
      break;
    default:
      return 0;
  }
}

// Set the temp. in deg C
void IRWhirlpoolAc::_setTemp(const uint8_t temp, const bool remember) {
  if (remember) _desiredtemp = temp;
  int8_t offset = getTempOffset();  // Cache the min temp for the model.
  uint8_t newtemp = std::max((uint8_t)(kWhirlpoolAcMinTemp + offset), temp);
  newtemp = std::min((uint8_t)(kWhirlpoolAcMaxTemp + offset), newtemp);
  remote_state[kWhirlpoolAcTempPos] =
      (remote_state[kWhirlpoolAcTempPos] & ~kWhirlpoolAcTempMask) |
      ((newtemp - (kWhirlpoolAcMinTemp + offset)) << 4);
}

// Set the temp. in deg C
void IRWhirlpoolAc::setTemp(const uint8_t temp) {
  _setTemp(temp);
  setSuper(false);  // Changing temp cancels Super/Jet mode.
  setCommand(kWhirlpoolAcCommandTemp);
}

// Return the set temp. in deg C
uint8_t IRWhirlpoolAc::getTemp() {
  return ((remote_state[kWhirlpoolAcTempPos] & kWhirlpoolAcTempMask) >> 4) +
         + kWhirlpoolAcMinTemp + getTempOffset();
}

void IRWhirlpoolAc::_setMode(const uint8_t mode) {
  switch (mode) {
    case kWhirlpoolAcAuto:
      setFan(kWhirlpoolAcFanAuto);
      _setTemp(kWhirlpoolAcAutoTemp, false);
      setSleep(false);  // Cancel sleep mode when in auto/6thsense mode.
      // FALL THRU
    case kWhirlpoolAcHeat:
    case kWhirlpoolAcCool:
    case kWhirlpoolAcDry:
    case kWhirlpoolAcFan:
      remote_state[kWhirlpoolAcModePos] &= ~kWhirlpoolAcModeMask;
      remote_state[kWhirlpoolAcModePos] |= mode;
      setCommand(kWhirlpoolAcCommandMode);
      break;
    default:
      return;
  }
  if (mode == kWhirlpoolAcAuto) setCommand(kWhirlpoolAcCommand6thSense);
}

void IRWhirlpoolAc::setMode(const uint8_t mode) {
    setSuper(false);  // Changing mode cancels Super/Jet mode.
    _setMode(mode);
}

uint8_t IRWhirlpoolAc::getMode() {
  return remote_state[kWhirlpoolAcModePos] & kWhirlpoolAcModeMask;
}

void IRWhirlpoolAc::setFan(const uint8_t speed) {
  switch (speed) {
    case kWhirlpoolAcFanAuto:
    case kWhirlpoolAcFanLow:
    case kWhirlpoolAcFanMedium:
    case kWhirlpoolAcFanHigh:
      remote_state[kWhirlpoolAcFanPos] =
          (remote_state[kWhirlpoolAcFanPos] & ~kWhirlpoolAcFanMask) | speed;
      setSuper(false);  // Changing fan speed cancels Super/Jet mode.
      setCommand(kWhirlpoolAcCommandFanSpeed);
      break;
  }
}

uint8_t IRWhirlpoolAc::getFan() {
  return remote_state[kWhirlpoolAcFanPos] & kWhirlpoolAcFanMask;
}

void IRWhirlpoolAc::setSwing(const bool on) {
  if (on) {
    remote_state[kWhirlpoolAcFanPos] |= kWhirlpoolAcSwing1Mask;
    remote_state[kWhirlpoolAcOffTimerPos] |= kWhirlpoolAcSwing2Mask;
  } else {
    remote_state[kWhirlpoolAcFanPos] &= ~kWhirlpoolAcSwing1Mask;
    remote_state[kWhirlpoolAcOffTimerPos] &= ~kWhirlpoolAcSwing2Mask;
  }
  setCommand(kWhirlpoolAcCommandSwing);
}

bool IRWhirlpoolAc::getSwing() {
  return (remote_state[kWhirlpoolAcFanPos] & kWhirlpoolAcSwing1Mask) &&
         (remote_state[kWhirlpoolAcOffTimerPos] & kWhirlpoolAcSwing2Mask);
}

void IRWhirlpoolAc::setLight(const bool on) {
  if (on)
    remote_state[kWhirlpoolAcClockPos] &= ~kWhirlpoolAcLightMask;
  else
    remote_state[kWhirlpoolAcClockPos] |= kWhirlpoolAcLightMask;
}

bool IRWhirlpoolAc::getLight() {
  return !(remote_state[kWhirlpoolAcClockPos] & kWhirlpoolAcLightMask);
}

void IRWhirlpoolAc::setTime(const uint16_t pos,
                            const uint16_t minspastmidnight) {
  // Hours
  remote_state[pos] &= ~kWhirlpoolAcHourMask;
  remote_state[pos] |= (minspastmidnight / 60) % 24;
  // Minutes
  remote_state[pos + 1] &= ~kWhirlpoolAcMinuteMask;
  remote_state[pos + 1] |= minspastmidnight % 60;
}

uint16_t IRWhirlpoolAc::getTime(const uint16_t pos) {
  return (remote_state[pos] & kWhirlpoolAcHourMask) * 60 +
         (remote_state[pos + 1] & kWhirlpoolAcMinuteMask);
}

bool IRWhirlpoolAc::isTimerEnabled(const uint16_t pos) {
  return remote_state[pos - 1] & kWhirlpoolAcTimerEnableMask;
}

void IRWhirlpoolAc::enableTimer(const uint16_t pos, const bool state) {
  if (state)
    remote_state[pos - 1] |= kWhirlpoolAcTimerEnableMask;
  else
    remote_state[pos - 1] &= ~kWhirlpoolAcTimerEnableMask;
}

void IRWhirlpoolAc::setClock(const uint16_t minspastmidnight) {
  setTime(kWhirlpoolAcClockPos, minspastmidnight);
}

uint16_t IRWhirlpoolAc::getClock() { return getTime(kWhirlpoolAcClockPos); }

void IRWhirlpoolAc::setOffTimer(const uint16_t minspastmidnight) {
  setTime(kWhirlpoolAcOffTimerPos, minspastmidnight);
}

uint16_t IRWhirlpoolAc::getOffTimer() {
  return getTime(kWhirlpoolAcOffTimerPos);
}

bool IRWhirlpoolAc::isOffTimerEnabled() {
  return isTimerEnabled(kWhirlpoolAcOffTimerPos);
}

void IRWhirlpoolAc::enableOffTimer(const bool state) {
  enableTimer(kWhirlpoolAcOffTimerPos, state);
  setCommand(kWhirlpoolAcCommandOffTimer);
}

void IRWhirlpoolAc::setOnTimer(const uint16_t minspastmidnight) {
  setTime(kWhirlpoolAcOnTimerPos, minspastmidnight);
}

uint16_t IRWhirlpoolAc::getOnTimer() { return getTime(kWhirlpoolAcOnTimerPos); }

bool IRWhirlpoolAc::isOnTimerEnabled() {
  return isTimerEnabled(kWhirlpoolAcOnTimerPos);
}

void IRWhirlpoolAc::enableOnTimer(const bool state) {
  enableTimer(kWhirlpoolAcOnTimerPos, state);
  setCommand(kWhirlpoolAcCommandOnTimer);
}

void IRWhirlpoolAc::setPowerToggle(const bool on) {
  if (on)
    remote_state[kWhirlpoolAcPowerTogglePos] |= kWhirlpoolAcPowerToggleMask;
  else
    remote_state[kWhirlpoolAcPowerTogglePos] &= ~kWhirlpoolAcPowerToggleMask;
  setSuper(false);  // Changing power cancels Super/Jet mode.
  setCommand(kWhirlpoolAcCommandPower);
}

bool IRWhirlpoolAc::getPowerToggle() {
  return remote_state[kWhirlpoolAcPowerTogglePos] & kWhirlpoolAcPowerToggleMask;
}

uint8_t IRWhirlpoolAc::getCommand() {
  return remote_state[kWhirlpoolAcCommandPos];
}

void IRWhirlpoolAc::setSleep(const bool on) {
  if (on) {
    remote_state[kWhirlpoolAcSleepPos] |= kWhirlpoolAcSleepMask;
    setFan(kWhirlpoolAcFanLow);
  } else {
    remote_state[kWhirlpoolAcSleepPos] &= ~kWhirlpoolAcSleepMask;
  }
  setCommand(kWhirlpoolAcCommandSleep);
}

bool IRWhirlpoolAc::getSleep() {
  return remote_state[kWhirlpoolAcSleepPos] & kWhirlpoolAcSleepMask;
}

// AKA Jet/Turbo mode.
void IRWhirlpoolAc::setSuper(const bool on) {
  if (on) {
    setFan(kWhirlpoolAcFanHigh);
    switch (getMode()) {
      case kWhirlpoolAcHeat:
        setTemp(kWhirlpoolAcMaxTemp + getTempOffset());
        break;
      case kWhirlpoolAcCool:
      default:
        setTemp(kWhirlpoolAcMinTemp + getTempOffset());
        setMode(kWhirlpoolAcCool);
        break;
    }
    remote_state[kWhirlpoolAcSuperPos] |= kWhirlpoolAcSuperMask;
  } else {
    remote_state[kWhirlpoolAcSuperPos] &= ~kWhirlpoolAcSuperMask;
  }
  setCommand(kWhirlpoolAcCommandSuper);
}

bool IRWhirlpoolAc::getSuper() {
  return remote_state[kWhirlpoolAcSuperPos] & kWhirlpoolAcSuperMask;
}

void IRWhirlpoolAc::setCommand(const uint8_t code) {
  remote_state[kWhirlpoolAcCommandPos] = code;
}

// Convert a standard A/C mode into its native mode.
uint8_t IRWhirlpoolAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool:
      return kWhirlpoolAcCool;
    case stdAc::opmode_t::kHeat:
      return kWhirlpoolAcHeat;
    case stdAc::opmode_t::kDry:
      return kWhirlpoolAcDry;
    case stdAc::opmode_t::kFan:
      return kWhirlpoolAcFan;
    default:
      return kWhirlpoolAcAuto;
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRWhirlpoolAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:
      return kWhirlpoolAcFanLow;
    case stdAc::fanspeed_t::kMedium:
      return kWhirlpoolAcFanMedium;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:
      return kWhirlpoolAcFanHigh;
    default:
      return kWhirlpoolAcFanAuto;
  }
}

#ifdef ARDUINO
String IRWhirlpoolAc::timeToString(const uint16_t minspastmidnight) {
  String result = "";
#else
std::string IRWhirlpoolAc::timeToString(const uint16_t minspastmidnight) {
  std::string result = "";
#endif  // ARDUINO
  uint8_t hours = minspastmidnight / 60;
  if (hours < 10) result += '0';
  result += uint64ToString(hours);
  result += ':';
  uint8_t mins = minspastmidnight % 60;
  if (mins < 10) result += '0';
  result += uint64ToString(mins);
  return result;
}

// Convert the internal state into a human readable string.
#ifdef ARDUINO
String IRWhirlpoolAc::toString() {
  String result = "";
#else
std::string IRWhirlpoolAc::toString() {
  std::string result = "";
#endif  // ARDUINO
  result += F("Model: ");
  result += uint64ToString(getModel());
  switch (getModel()) {
    case DG11J191:
      result += F(" (DG11J191)");
      break;
    case DG11J13A:
      result += F(" (DG11J13A)");
      break;
    default:
      result += F(" (UNKNOWN)");
  }
  result += F(", Power toggle: ");
  if (getPowerToggle())
    result += F("On");
  else
    result += F("Off");
  result += F(", Mode: ");
  result += uint64ToString(getMode());
  switch (getMode()) {
    case kWhirlpoolAcHeat:
      result += F(" (HEAT)");
      break;
    case kWhirlpoolAcAuto:
      result += F(" (AUTO)");
      break;
    case kWhirlpoolAcCool:
      result += F(" (COOL)");
      break;
    case kWhirlpoolAcDry:
      result += F(" (DRY)");
      break;
    case kWhirlpoolAcFan:
      result += F(" (FAN)");
      break;
    default:
      result += F(" (UNKNOWN)");
  }
  result += F(", Temp: ");
  result += uint64ToString(getTemp());
  result += F("C, Fan: ");
  result += uint64ToString(getFan());
  switch (getFan()) {
    case kWhirlpoolAcFanAuto:
      result += F(" (AUTO)");
      break;
    case kWhirlpoolAcFanHigh:
      result += F(" (HIGH)");
      break;
    case kWhirlpoolAcFanMedium:
      result += F(" (MEDIUM)");
      break;
    case kWhirlpoolAcFanLow:
      result += F(" (LOW)");
      break;
    default:
      result += F(" (UNKNOWN)");
      break;
  }
  result += F(", Swing: ");
  if (getSwing())
    result += F("On");
  else
    result += F("Off");
  result += F(", Light: ");
  if (getLight())
    result += F("On");
  else
    result += F("Off");
  result += F(", Clock: ");
  result += timeToString(getClock());
  result += F(", On Timer: ");
  if (isOnTimerEnabled())
    result += timeToString(getOnTimer());
  else
    result += F("Off");
  result += F(", Off Timer: ");
  if (isOffTimerEnabled())
    result += timeToString(getOffTimer());
  else
    result += F("Off");
  result += F(", Sleep: ");
  if (getSleep())
    result += F("On");
  else
    result += F("Off");
  result += F(", Super: ");
  if (getSuper())
    result += F("On");
  else
    result += F("Off");
  result += F(", Command: ");
  result += uint64ToString(getCommand());
  switch (getCommand()) {
    case kWhirlpoolAcCommandLight:
      result += F(" (LIGHT)");
      break;
    case kWhirlpoolAcCommandPower:
      result += F(" (POWER)");
      break;
    case kWhirlpoolAcCommandTemp:
      result += F(" (TEMP)");
      break;
    case kWhirlpoolAcCommandSleep:
      result += F(" (SLEEP)");
      break;
    case kWhirlpoolAcCommandSuper:
      result += F(" (SUPER)");
      break;
    case kWhirlpoolAcCommandOnTimer:
      result += F(" (ONTIMER)");
      break;
    case kWhirlpoolAcCommandMode:
      result += F(" (MODE)");
      break;
    case kWhirlpoolAcCommandSwing:
      result += F(" (SWING)");
      break;
    case kWhirlpoolAcCommandIFeel:
      result += F(" (IFEEL)");
      break;
    case kWhirlpoolAcCommandFanSpeed:
      result += F(" (FANSPEED)");
      break;
    case kWhirlpoolAcCommand6thSense:
      result += F(" (6THSENSE)");
      break;
    case kWhirlpoolAcCommandOffTimer:
      result += F(" (OFFTIMER)");
      break;
    default:
      result += F(" (UNKNOWN)");
      break;
  }
  return result;
}

#if DECODE_WHIRLPOOL_AC
// Decode the supplied Whirlpool A/C message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect. Typically kWhirlpoolAcBits
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: STABLE / Working as intended.
//
//
// Ref:
//   https://github.com/markszabo/IRremoteESP8266/issues/509
bool IRrecv::decodeWhirlpoolAC(decode_results *results, uint16_t nbits,
                               bool strict) {
  if (results->rawlen < 2 * nbits + 4 + kHeader + kFooter - 1)
    return false;  // Can't possibly be a valid Whirlpool A/C message.
  if (strict) {
    if (nbits != kWhirlpoolAcBits) return false;
  }

  uint16_t offset = kStartOffset;
  uint16_t dataBitsSoFar = 0;
  uint16_t i = 0;
  match_result_t data_result;
  uint8_t sectionSize[kWhirlpoolAcSections] = {6, 8, 7};

  // Header
  if (!matchMark(results->rawbuf[offset++], kWhirlpoolAcHdrMark)) return false;
  if (!matchSpace(results->rawbuf[offset++], kWhirlpoolAcHdrSpace))
    return false;

  // Data Section
  // Keep reading bytes until we either run out of section or state to fill.
  for (uint8_t section = 0, pos = 0; section < kWhirlpoolAcSections;
       section++) {
    pos += sectionSize[section];
    for (; offset <= results->rawlen - 16 && i < pos;
         i++, dataBitsSoFar += 8, offset += data_result.used) {
      data_result =
          matchData(&(results->rawbuf[offset]), 8, kWhirlpoolAcBitMark,
                    kWhirlpoolAcOneSpace, kWhirlpoolAcBitMark,
                    kWhirlpoolAcZeroSpace, kTolerance, kMarkExcess, false);
      if (data_result.success == false) break;  // Fail
      // Data is in LSB order. We need to reverse it.
      results->state[i] = (uint8_t)data_result.data;
    }
    // Section Footer
    if (!matchMark(results->rawbuf[offset++], kWhirlpoolAcBitMark))
      return false;
    if (section < kWhirlpoolAcSections - 1) {  // Inter-section gaps.
      if (!matchSpace(results->rawbuf[offset++], kWhirlpoolAcGap)) return false;
    } else {  // Last section / End of message gap.
      if (offset <= results->rawlen &&
          !matchAtLeast(results->rawbuf[offset++], kWhirlpoolAcGap))
        return false;
    }
  }

  // Compliance
  if (strict) {
    // Re-check we got the correct size/length due to the way we read the data.
    if (dataBitsSoFar != kWhirlpoolAcBits) return false;
    if (!IRWhirlpoolAc::validChecksum(results->state, dataBitsSoFar / 8))
      return false;
  }

  // Success
  results->decode_type = WHIRLPOOL_AC;
  results->bits = dataBitsSoFar;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // WHIRLPOOL_AC
