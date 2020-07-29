// Copyright 2018 Erdem U. Altinyurt
// Copyright 2019 David Conran

// Vestel added by Erdem U. Altinyurt

#include "ir_Vestel.h"
#include <algorithm>
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include "IRrecv.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"
#include "ir_Haier.h"

// Equipment it seems compatible with:
//  * Vestel AC Model BIOX CXP-9 (9K BTU)
//  * <Add models (A/C & remotes) you've gotten it working with here>

// Ref:
//   None. Totally reverse engineered.

using irutils::addBoolToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;
using irutils::minsToString;
using irutils::setBit;
using irutils::setBits;

#if SEND_VESTEL_AC
// Send a Vestel message
//
// Args:
//   data:   Contents of the message to be sent.
//   nbits:  Nr. of bits of data to be sent. Typically kVestelBits.
//
// Status: STABLE / Working.
//
void IRsend::sendVestelAc(const uint64_t data, const uint16_t nbits,
                          const uint16_t repeat) {
  if (nbits % 8 != 0) return;  // nbits is required to be a multiple of 8.

  sendGeneric(kVestelAcHdrMark, kVestelAcHdrSpace,   // Header
              kVestelAcBitMark, kVestelAcOneSpace,   // Data
              kVestelAcBitMark, kVestelAcZeroSpace,  // Data
              kVestelAcBitMark, 100000,              // Footer + repeat gap
              data, nbits, 38, false, repeat, 50);
}
#endif

// Code to emulate Vestel A/C IR remote control unit.

// Initialise the object.
IRVestelAc::IRVestelAc(const uint16_t pin, const bool inverted,
                       const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { this->stateReset(); }

// Reset the state of the remote to a known good state/sequence.
void IRVestelAc::stateReset(void) {
  // Power On, Mode Auto, Fan Auto, Temp = 25C/77F
  remote_state = kVestelAcStateDefault;
  remote_time_state = kVestelAcTimeStateDefault;
  use_time_state = false;
}

// Configure the pin for output.
void IRVestelAc::begin(void) {
  _irsend.begin();
}

#if SEND_VESTEL_AC
// Send the current desired state to the IR LED.
void IRVestelAc::send(void) { _irsend.sendVestelAc(getRaw()); }
#endif  // SEND_VESTEL_AC

// Return the internal state date of the remote.
uint64_t IRVestelAc::getRaw(void) {
  this->checksum();
  if (use_time_state) return remote_time_state;
  return remote_state;
}

// Override the internal state with the new state.
void IRVestelAc::setRaw(const uint8_t* newState) {
  uint64_t upState = 0;
  for (int i = 0; i < 7; i++)
    upState |= static_cast<uint64_t>(newState[i]) << (i * 8);
  this->setRaw(upState);
}

void IRVestelAc::setRaw(const uint64_t newState) {
  use_time_state = false;
  remote_state = newState;
  remote_time_state = newState;
  if (this->isTimeCommand()) {
    use_time_state = true;
    remote_state = kVestelAcStateDefault;
  } else {
    remote_time_state = kVestelAcTimeStateDefault;
  }
}

// Set the requested power state of the A/C to on.
void IRVestelAc::on(void) { setPower(true); }

// Set the requested power state of the A/C to off.
void IRVestelAc::off(void) { setPower(false); }

// Set the requested power state of the A/C.
void IRVestelAc::setPower(const bool on) {
  setBits(&remote_state, kVestelAcPowerOffset, kVestelAcPowerSize,
          on ? 0b11 : 0b00);
  use_time_state = false;
}

// Return the requested power state of the A/C.
bool IRVestelAc::getPower(void) {
  return GETBITS64(remote_state, kVestelAcPowerOffset, kVestelAcPowerSize);
}

// Set the temperature in Celsius degrees.
void IRVestelAc::setTemp(const uint8_t temp) {
  uint8_t new_temp = std::max(kVestelAcMinTempC, temp);
  new_temp = std::min(kVestelAcMaxTemp, new_temp);
  setBits(&remote_state, kVestelAcTempOffset, kNibbleSize,
          new_temp - kVestelAcMinTempH);
  use_time_state = false;
}

// Return the set temperature.
uint8_t IRVestelAc::getTemp(void) {
  return GETBITS64(remote_state, kVestelAcTempOffset, kNibbleSize) +
      kVestelAcMinTempH;
}

// Set the speed of the fan,
void IRVestelAc::setFan(const uint8_t fan) {
  switch (fan) {
    case kVestelAcFanLow:
    case kVestelAcFanMed:
    case kVestelAcFanHigh:
    case kVestelAcFanAutoCool:
    case kVestelAcFanAutoHot:
    case kVestelAcFanAuto:
      setBits(&remote_state, kVestelAcFanOffset, kVestelAcFanSize, fan);
      break;
    default:
      setFan(kVestelAcFanAuto);
  }
  use_time_state = false;
}

// Return the requested state of the unit's fan.
uint8_t IRVestelAc::getFan(void) {
  return GETBITS64(remote_state, kVestelAcFanOffset, kVestelAcFanSize);
}

// Get the requested climate operation mode of the a/c unit.
// Returns:
//   A uint8_t containing the A/C mode.
uint8_t IRVestelAc::getMode(void) {
  return GETBITS64(remote_state, kVestelAcModeOffset, kModeBitsSize);
}

// Set the requested climate operation mode of the a/c unit.
void IRVestelAc::setMode(const uint8_t mode) {
  // If we get an unexpected mode, default to AUTO.
  switch (mode) {
    case kVestelAcAuto:
    case kVestelAcCool:
    case kVestelAcHeat:
    case kVestelAcDry:
    case kVestelAcFan:
      setBits(&remote_state, kVestelAcModeOffset, kModeBitsSize, mode);
      break;
    default:
      setMode(kVestelAcAuto);
  }
  use_time_state = false;
}

// Set Auto mode of AC.
void IRVestelAc::setAuto(const int8_t autoLevel) {
  if (autoLevel < -2 || autoLevel > 2) return;
  setMode(kVestelAcAuto);
  setFan((autoLevel < 0 ? kVestelAcFanAutoCool : kVestelAcFanAutoHot));
  if (autoLevel == 2)
    setTemp(30);
  else if (autoLevel == 1)
    setTemp(31);
  else if (autoLevel == 0)
    setTemp(25);
  else if (autoLevel == -1)
    setTemp(16);
  else if (autoLevel == -2)
    setTemp(17);
}

void IRVestelAc::setTimerActive(const bool on) {
  setBit(&remote_time_state, kVestelAcTimerFlagOffset, on);
  use_time_state = true;
}

bool IRVestelAc::isTimerActive(void) {
  return GETBIT64(remote_time_state, kVestelAcTimerFlagOffset);
}

// Set Timer option of AC.
// Valid time arguments are 0, 0.5, 1, 2, 3 and 5 hours (in min). 0 disables the
// timer.
void IRVestelAc::setTimer(const uint16_t minutes) {
  // Clear both On & Off timers.
  remote_time_state &= ~((uint64_t)0xFFFF << kVestelAcOffTimeOffset);
  // Set the "Off" time with the nr of minutes before we turn off.
  remote_time_state |= (uint64_t)(((minutes / 60) << 3) + (minutes % 60) / 10)
                       << kVestelAcOffTimeOffset;
  setOffTimerActive(false);
  // Yes. On Timer instead of Off timer active.
  setOnTimerActive(minutes != 0);
  setTimerActive(minutes != 0);
  use_time_state = true;
}

uint16_t IRVestelAc::getTimer(void) { return getOffTimer(); }

// Set the AC's internal clock
void IRVestelAc::setTime(const uint16_t minutes) {
  setBits(&remote_time_state, kVestelAcHourOffset, kVestelAcHourSize,
          minutes / 60);
  setBits(&remote_time_state, kVestelAcMinuteOffset, kVestelAcMinuteSize,
          minutes % 60);
  use_time_state = true;
}

uint16_t IRVestelAc::getTime(void) {
  return GETBITS64(remote_time_state, kVestelAcHourOffset, kVestelAcHourSize) *
      60 + GETBITS64(remote_time_state, kVestelAcMinuteOffset,
                     kVestelAcMinuteSize);
}

void IRVestelAc::setOnTimerActive(const bool on) {
  setBit(&remote_time_state, kVestelAcOnTimerFlagOffset, on);
  use_time_state = true;
}

bool IRVestelAc::isOnTimerActive(void) {
  return GETBIT64(remote_time_state, kVestelAcOnTimerFlagOffset);
}

// Set a given timer (via offset). Takes time in nr. of minutes.
void IRVestelAc::_setTimer(const uint16_t minutes, const uint8_t offset) {
  setBits(&remote_time_state, offset, kVestelAcTimerSize,
          ((minutes / 60) << 3) + (minutes % 60) / 10);
  setTimerActive(false);
  use_time_state = true;
}

// Get the number of mins a timer is set for.
uint16_t IRVestelAc::_getTimer(const uint8_t offset) {
  return GETBITS64(remote_time_state, offset + kVestelAcTimerMinsSize,
                   kVestelAcTimerHourSize) * 60 +  // Hrs
      GETBITS64(remote_time_state, offset, kVestelAcTimerMinsSize) * 10;  // Min
}
// Set AC's wake up time. Takes time in minute.
void IRVestelAc::setOnTimer(const uint16_t minutes) {
  setOnTimerActive(minutes);
  _setTimer(minutes, kVestelAcOnTimeOffset);
}

uint16_t IRVestelAc::getOnTimer(void) {
  return _getTimer(kVestelAcOnTimeOffset);
}

void IRVestelAc::setOffTimerActive(const bool on) {
  setBit(&remote_time_state, kVestelAcOffTimerFlagOffset, on);
  use_time_state = true;
}

bool IRVestelAc::isOffTimerActive(void) {
  return GETBIT64(remote_time_state, kVestelAcOffTimerFlagOffset);
}

// Set AC's turn off time. Takes time in minute.
void IRVestelAc::setOffTimer(const uint16_t minutes) {
  setOffTimerActive(minutes);
  _setTimer(minutes, kVestelAcOffTimeOffset);
}

uint16_t IRVestelAc::getOffTimer(void) {
  return _getTimer(kVestelAcOffTimeOffset);
}

// Set the Sleep state of the A/C.
void IRVestelAc::setSleep(const bool on) {
  setBits(&remote_state, kVestelAcTurboSleepOffset, kNibbleSize,
          on ? kVestelAcSleep : kVestelAcNormal);
  use_time_state = false;
}

// Return the Sleep state of the A/C.
bool IRVestelAc::getSleep(void) {
  return GETBITS64(remote_state, kVestelAcTurboSleepOffset, kNibbleSize) ==
      kVestelAcSleep;
}

// Set the Turbo state of the A/C.
void IRVestelAc::setTurbo(const bool on) {
  setBits(&remote_state, kVestelAcTurboSleepOffset, kNibbleSize,
          on ? kVestelAcTurbo : kVestelAcNormal);
  use_time_state = false;
}

// Return the Turbo state of the A/C.
bool IRVestelAc::getTurbo(void) {
  return GETBITS64(remote_state, kVestelAcTurboSleepOffset, kNibbleSize) ==
      kVestelAcTurbo;
}

// Set the Ion state of the A/C.
void IRVestelAc::setIon(const bool on) {
  setBit(&remote_state, kVestelAcIonOffset, on);
  use_time_state = false;
}

// Return the Ion state of the A/C.
bool IRVestelAc::getIon(void) {
  return GETBIT64(remote_state, kVestelAcIonOffset);
}

// Set the Swing Roaming state of the A/C.
void IRVestelAc::setSwing(const bool on) {
  setBits(&remote_state, kVestelAcSwingOffset, kNibbleSize,
          on ? kVestelAcSwing : 0xF);
  use_time_state = false;
}

// Return the Swing Roaming state of the A/C.
bool IRVestelAc::getSwing(void) {
  return GETBITS64(remote_state, kVestelAcSwingOffset, kNibbleSize) ==
      kVestelAcSwing;
}

// Calculate the checksum for a given array.
// Args:
//   state:  The state to calculate the checksum over.
// Returns:
//   The 8 bit checksum value.
uint8_t IRVestelAc::calcChecksum(const uint64_t state) {
  // Just counts the set bits +1 on stream and take inverse after mask
  return 0xFF - countBits(GETBITS64(state, 20, 44), 44, true, 2);
}

// Verify the checksum is valid for a given state.
// Args:
//   state:  The state to verify the checksum of.
// Returns:
//   A boolean.
bool IRVestelAc::validChecksum(const uint64_t state) {
  return GETBITS64(state, kVestelAcChecksumOffset, kVestelAcChecksumSize) ==
      IRVestelAc::calcChecksum(state);
}

// Calculate & set the checksum for the current internal state of the remote.
void IRVestelAc::checksum(void) {
  // Stored the checksum value in the last byte.
  setBits(&remote_state, kVestelAcChecksumOffset, kVestelAcChecksumSize,
          this->calcChecksum(remote_state));
  setBits(&remote_time_state, kVestelAcChecksumOffset, kVestelAcChecksumSize,
          this->calcChecksum(remote_time_state));
}

bool IRVestelAc::isTimeCommand(void) {
  return !GETBITS64(remote_state, kVestelAcPowerOffset, kNibbleSize) ||
      use_time_state;
}

// Convert a standard A/C mode into its native mode.
uint8_t IRVestelAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kVestelAcCool;
    case stdAc::opmode_t::kHeat: return kVestelAcHeat;
    case stdAc::opmode_t::kDry:  return kVestelAcDry;
    case stdAc::opmode_t::kFan:  return kVestelAcFan;
    default:                     return kVestelAcAuto;
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRVestelAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kVestelAcFanLow;
    case stdAc::fanspeed_t::kMedium: return kVestelAcFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kVestelAcFanHigh;
    default:                         return kVestelAcFanAuto;
  }
}

// Convert a native mode to it's common equivalent.
stdAc::opmode_t IRVestelAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kVestelAcCool: return stdAc::opmode_t::kCool;
    case kVestelAcHeat: return stdAc::opmode_t::kHeat;
    case kVestelAcDry:  return stdAc::opmode_t::kDry;
    case kVestelAcFan:  return stdAc::opmode_t::kFan;
    default:            return stdAc::opmode_t::kAuto;
  }
}

// Convert a native fan speed to it's common equivalent.
stdAc::fanspeed_t IRVestelAc::toCommonFanSpeed(const uint8_t spd) {
  switch (spd) {
    case kVestelAcFanHigh: return stdAc::fanspeed_t::kMax;
    case kVestelAcFanMed:  return stdAc::fanspeed_t::kMedium;
    case kVestelAcFanLow:  return stdAc::fanspeed_t::kMin;
    default:               return stdAc::fanspeed_t::kAuto;
  }
}

// Convert the A/C state to it's common equivalent.
stdAc::state_t IRVestelAc::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::VESTEL_AC;
  result.model = -1;  // Not supported.
  result.power = this->getPower();
  result.mode = this->toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = this->toCommonFanSpeed(this->getFan());
  result.swingv = this->getSwing() ? stdAc::swingv_t::kAuto :
                                     stdAc::swingv_t::kOff;
  result.turbo = this->getTurbo();
  result.filter = this->getIon();
  result.sleep = this->getSleep() ? 0 : -1;
  // Not supported.
  result.swingh = stdAc::swingh_t::kOff;
  result.light = false;
  result.econo = false;
  result.quiet = false;
  result.clean = false;
  result.beep = false;
  result.clock = -1;
  return result;
}

// Convert the internal state into a human readable string.
String IRVestelAc::toString(void) {
  String result = "";
  result.reserve(100);  // Reserve some heap for the string to reduce fragging.
  if (this->isTimeCommand()) {
    result += addLabeledString(minsToString(getTime()), kClockStr, false);
    result += addLabeledString(
        isTimerActive() ? minsToString(getTimer()) : kOffStr,
        kTimerStr);
    result += addLabeledString(
        (isOnTimerActive() && !isTimerActive()) ?
          minsToString(this->getOnTimer()) : kOffStr,
        kOnTimerStr);
    result += addLabeledString(
        isOffTimerActive() ? minsToString(getOffTimer()) : kOffStr,
        kOffTimerStr);
    return result;
  }
  // Not a time command, it's a normal command.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(getMode(), kVestelAcAuto, kVestelAcCool,
                            kVestelAcHeat, kVestelAcDry, kVestelAcFan);
  result += addTempToString(getTemp());
  result += addIntToString(getFan(), kFanStr);
  result += kSpaceLBraceStr;
  switch (this->getFan()) {
    case kVestelAcFanAuto:
      result += kAutoStr;
      break;
    case kVestelAcFanLow:
      result += kLowStr;
      break;
    case kVestelAcFanMed:
      result += kMedStr;
      break;
    case kVestelAcFanHigh:
      result += kHighStr;
      break;
    case kVestelAcFanAutoCool:
      result += kAutoStr;
      result += ' ';
      result += kCoolStr;
      break;
    case kVestelAcFanAutoHot:
      result += kAutoStr;
      result += ' ';
      result += kHeatStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addBoolToString(getSleep(), kSleepStr);
  result += addBoolToString(getTurbo(), kTurboStr);
  result += addBoolToString(getIon(), kIonStr);
  result += addBoolToString(getSwing(), kSwingStr);
  return result;
}

#if DECODE_VESTEL_AC
// Decode the supplied Vestel message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   offset:  The starting index to use when attempting to decode the raw data.
//            Typically/Defaults to kStartOffset.
//   nbits:   The number of data bits to expect. Typically kVestelBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: Alpha / Needs testing against a real device.
//
bool IRrecv::decodeVestelAc(decode_results* results, uint16_t offset,
                            const uint16_t nbits, const bool strict) {
  if (nbits % 8 != 0)  // nbits has to be a multiple of nr. of bits in a byte.
    return false;

  if (strict)
    if (nbits != kVestelAcBits)
      return false;  // Not strictly a Vestel AC message.

  uint64_t data = 0;

  if (nbits > sizeof(data) * 8)
    return false;  // We can't possibly capture a Vestel packet that big.

  // Match Header + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, &data,
                    results->rawlen - offset, nbits,
                    kVestelAcHdrMark, kVestelAcHdrSpace,
                    kVestelAcBitMark, kVestelAcOneSpace,
                    kVestelAcBitMark, kVestelAcZeroSpace,
                    kVestelAcBitMark, 0, false,
                    kVestelAcTolerance, kMarkExcess, false)) return false;
  // Compliance
  if (strict)
    if (!IRVestelAc::validChecksum(data)) return false;

  // Success
  results->decode_type = VESTEL_AC;
  results->bits = nbits;
  results->value = data;
  results->address = 0;
  results->command = 0;

  return true;
}
#endif  // DECODE_VESTEL_AC
