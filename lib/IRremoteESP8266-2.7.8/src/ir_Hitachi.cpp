// Copyright 2018-2019 David Conran
/// @file
/// @brief Support for Hitachi A/C protocols.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/417
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/453
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/973
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1056
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1060
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1134

#include "ir_Hitachi.h"
#include <algorithm>
#include <cstring>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

// Constants
const uint16_t kHitachiAcHdrMark = 3300;
const uint16_t kHitachiAcHdrSpace = 1700;
const uint16_t kHitachiAc1HdrMark = 3400;
const uint16_t kHitachiAc1HdrSpace = 3400;
const uint16_t kHitachiAcBitMark = 400;
const uint16_t kHitachiAcOneSpace = 1250;
const uint16_t kHitachiAcZeroSpace = 500;
const uint32_t kHitachiAcMinGap = kDefaultMessageGap;  // Just a guess.
// Support for HitachiAc424 protocol
const uint16_t kHitachiAc424LdrMark = 29784;   // Leader
const uint16_t kHitachiAc424LdrSpace = 49290;  // Leader
const uint16_t kHitachiAc424HdrMark = 3416;    // Header
const uint16_t kHitachiAc424HdrSpace = 1604;   // Header
const uint16_t kHitachiAc424BitMark = 463;
const uint16_t kHitachiAc424OneSpace = 1208;
const uint16_t kHitachiAc424ZeroSpace = 372;

// Support for HitachiAc3 protocol
const uint16_t kHitachiAc3HdrMark = 3400;    // Header
const uint16_t kHitachiAc3HdrSpace = 1660;   // Header
const uint16_t kHitachiAc3BitMark = 460;
const uint16_t kHitachiAc3OneSpace = 1250;
const uint16_t kHitachiAc3ZeroSpace = 410;

using irutils::addBoolToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addModelToString;
using irutils::addFanToString;
using irutils::addTempToString;
using irutils::minsToString;
using irutils::setBit;
using irutils::setBits;

#if (SEND_HITACHI_AC || SEND_HITACHI_AC2 || SEND_HITACHI_AC344)
/// Send a Hitachi 28-byte/224-bit A/C formatted message. (HITACHI_AC)
/// Status: STABLE / Working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/417
void IRsend::sendHitachiAC(const unsigned char data[], const uint16_t nbytes,
                           const uint16_t repeat) {
  if (nbytes < kHitachiAcStateLength)
    return;  // Not enough bytes to send a proper message.

  const bool MSBfirst = (nbytes == kHitachiAc344StateLength) ? false : true;
  sendGeneric(kHitachiAcHdrMark, kHitachiAcHdrSpace, kHitachiAcBitMark,
              kHitachiAcOneSpace, kHitachiAcBitMark, kHitachiAcZeroSpace,
              kHitachiAcBitMark, kHitachiAcMinGap, data, nbytes, 38, MSBfirst,
              repeat, 50);
}
#endif  // (SEND_HITACHI_AC || SEND_HITACHI_AC2 || SEND_HITACHI_AC344)

#if SEND_HITACHI_AC1
/// Send a Hitachi 13 byte/224-bit A/C formatted message. (HITACHI_AC1)
/// Status: STABLE / Confirmed Working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @note Basically the same as sendHitatchiAC() except different size & header.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/453
void IRsend::sendHitachiAC1(const unsigned char data[], const uint16_t nbytes,
                            const uint16_t repeat) {
  if (nbytes < kHitachiAc1StateLength)
    return;  // Not enough bytes to send a proper message.
  sendGeneric(kHitachiAc1HdrMark, kHitachiAc1HdrSpace, kHitachiAcBitMark,
              kHitachiAcOneSpace, kHitachiAcBitMark, kHitachiAcZeroSpace,
              kHitachiAcBitMark, kHitachiAcMinGap, data, nbytes, kHitachiAcFreq,
              true, repeat, kDutyDefault);
}
#endif  // SEND_HITACHI_AC1

#if SEND_HITACHI_AC2
/// Send a Hitachi 53 byte/424-bit A/C formatted message. (HITACHI_AC2)
///   Basically the same as sendHitatchiAC() except different size.
/// Status: STABLE / Expected to work.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendHitachiAC2(const unsigned char data[], const uint16_t nbytes,
                            const uint16_t repeat) {
  if (nbytes < kHitachiAc2StateLength)
    return;  // Not enough bytes to send a proper message.
  sendHitachiAC(data, nbytes, repeat);
}
#endif  // SEND_HITACHI_AC2

#if SEND_HITACHI_AC344
/// Send a Hitachi A/C 43-byte/344-bit message. (HITACHI_AC344)
///  Basically the same as sendHitatchiAC() except different size.
/// Status: Beta / Probably works.
/// @param[in] data An array of bytes containing the IR command.
/// @param[in] nbytes Nr. of bytes of data in the array.
/// @param[in] repeat Nr. of times the message is to be repeated. (Default = 0).
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1134
void IRsend::sendHitachiAc344(const unsigned char data[], const uint16_t nbytes,
                              const uint16_t repeat) {
  if (nbytes < kHitachiAc344StateLength)
    return;  // Not enough bytes to send a proper message.
  sendHitachiAC(data, nbytes, repeat);
}
#endif  // SEND_HITACHI_AC344

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRHitachiAc::IRHitachiAc(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Reset the internal state to a fixed known good state.
void IRHitachiAc::stateReset(void) {
  remote_state[0] = 0x80;
  remote_state[1] = 0x08;
  remote_state[2] = 0x0C;
  remote_state[3] = 0x02;
  remote_state[4] = 0xFD;
  remote_state[5] = 0x80;
  remote_state[6] = 0x7F;
  remote_state[7] = 0x88;
  remote_state[8] = 0x48;
  remote_state[9] = 0x10;
  for (uint8_t i = 10; i < kHitachiAcStateLength; i++) remote_state[i] = 0x00;
  remote_state[14] = 0x60;
  remote_state[15] = 0x60;
  remote_state[24] = 0x80;
  setTemp(23);
}

/// Set up hardware to be able to send a message.
void IRHitachiAc::begin(void) { _irsend.begin(); }

/// Calculate the checksum for a given state.
/// @param[in] state The value to calc the checksum of.
/// @param[in] length The size/length of the state.
/// @return The calculated checksum value.
uint8_t IRHitachiAc::calcChecksum(const uint8_t state[],
                                  const uint16_t length) {
  uint8_t sum = 62;
  for (uint16_t i = 0; i < length - 1; i++) sum -= reverseBits(state[i], 8);
  return reverseBits(sum, 8);
}

/// Calculate and set the checksum values for the internal state.
/// @param[in] length The size/length of the state.
void IRHitachiAc::checksum(const uint16_t length) {
  remote_state[length - 1] = calcChecksum(remote_state, length);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length of the state array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRHitachiAc::validChecksum(const uint8_t state[], const uint16_t length) {
  if (length < 2) return true;  // Assume true for lengths that are too short.
  return (state[length - 1] == calcChecksum(state, length));
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRHitachiAc::getRaw(void) {
  checksum();
  return remote_state;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
/// @param[in] length The length of the new_code array.
void IRHitachiAc::setRaw(const uint8_t new_code[], const uint16_t length) {
  memcpy(remote_state, new_code, std::min(length, kHitachiAcStateLength));
}

#if SEND_HITACHI_AC
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRHitachiAc::send(const uint16_t repeat) {
  _irsend.sendHitachiAC(getRaw(), kHitachiAcStateLength, repeat);
}
#endif  // SEND_HITACHI_AC

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRHitachiAc::getPower(void) {
  return GETBIT8(remote_state[17], kHitachiAcPowerOffset);
}

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHitachiAc::setPower(const bool on) {
  setBit(&remote_state[17], kHitachiAcPowerOffset, on);
}

/// Change the power setting to On.
void IRHitachiAc::on(void) { setPower(true); }

/// Change the power setting to Off.
void IRHitachiAc::off(void) { setPower(false); }

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRHitachiAc::getMode(void) { return reverseBits(remote_state[10], 8); }

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRHitachiAc::setMode(const uint8_t mode) {
  uint8_t newmode = mode;
  switch (mode) {
    // Fan mode sets a special temp.
    case kHitachiAcFan: setTemp(64); break;
    case kHitachiAcAuto:
    case kHitachiAcHeat:
    case kHitachiAcCool:
    case kHitachiAcDry: break;
    default: newmode = kHitachiAcAuto;
  }
  remote_state[10] = reverseBits(newmode, 8);
  if (mode != kHitachiAcFan) setTemp(_previoustemp);
  setFan(getFan());  // Reset the fan speed after the mode change.
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRHitachiAc::getTemp(void) {
  return reverseBits(remote_state[11], 8) >> 1;
}

/// Set the temperature.
/// @param[in] celsius The temperature in degrees celsius.
void IRHitachiAc::setTemp(const uint8_t celsius) {
  uint8_t temp;
  if (celsius != 64) _previoustemp = celsius;
  switch (celsius) {
    case 64:
      temp = celsius;
      break;
    default:
      temp = std::min(celsius, kHitachiAcMaxTemp);
      temp = std::max(temp, kHitachiAcMinTemp);
  }
  remote_state[11] = reverseBits(temp << 1, 8);
  if (temp == kHitachiAcMinTemp)
    remote_state[9] = 0x90;
  else
    remote_state[9] = 0x10;
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRHitachiAc::getFan(void) { return reverseBits(remote_state[13], 8); }

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRHitachiAc::setFan(const uint8_t speed) {
  uint8_t fanmin = kHitachiAcFanAuto;
  uint8_t fanmax = kHitachiAcFanHigh;
  switch (getMode()) {
    case kHitachiAcDry:  // Only 2 x low speeds in Dry mode.
      fanmin = kHitachiAcFanLow;
      fanmax = kHitachiAcFanLow + 1;
      break;
    case kHitachiAcFan:
      fanmin = kHitachiAcFanLow;  // No Auto in Fan mode.
      break;
  }
  uint8_t newspeed = std::max(speed, fanmin);
  newspeed = std::min(newspeed, fanmax);
  remote_state[13] = reverseBits(newspeed, 8);
}

/// Get the Vertical Swing setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHitachiAc::getSwingVertical(void) {
  return GETBIT8(remote_state[14], kHitachiAcSwingOffset);
}

/// Set the Vertical Swing setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHitachiAc::setSwingVertical(const bool on) {
  setBit(&remote_state[14], kHitachiAcSwingOffset, on);
}

/// Get the Horizontal Swing setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHitachiAc::getSwingHorizontal(void) {
  return GETBIT8(remote_state[15], kHitachiAcSwingOffset);
}

/// Set the Horizontal Swing setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHitachiAc::setSwingHorizontal(const bool on) {
  setBit(&remote_state[15], kHitachiAcSwingOffset, on);
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivilant of the enum.
uint8_t IRHitachiAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kHitachiAcCool;
    case stdAc::opmode_t::kHeat: return kHitachiAcHeat;
    case stdAc::opmode_t::kDry:  return kHitachiAcDry;
    case stdAc::opmode_t::kFan:  return kHitachiAcFan;
    default:                     return kHitachiAcAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivilant of the enum.
uint8_t IRHitachiAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kHitachiAcFanLow;
    case stdAc::fanspeed_t::kMedium: return kHitachiAcFanLow + 1;
    case stdAc::fanspeed_t::kHigh:   return kHitachiAcFanHigh - 1;
    case stdAc::fanspeed_t::kMax:    return kHitachiAcFanHigh;
    default:                         return kHitachiAcFanAuto;
  }
}

/// Convert a native mode into its stdAc equivilant.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivilant of the native setting.
stdAc::opmode_t IRHitachiAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kHitachiAcCool: return stdAc::opmode_t::kCool;
    case kHitachiAcHeat: return stdAc::opmode_t::kHeat;
    case kHitachiAcDry:  return stdAc::opmode_t::kDry;
    case kHitachiAcFan:  return stdAc::opmode_t::kFan;
    default:             return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivilant.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivilant of the native setting.
stdAc::fanspeed_t IRHitachiAc::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kHitachiAcFanHigh:     return stdAc::fanspeed_t::kMax;
    case kHitachiAcFanHigh - 1: return stdAc::fanspeed_t::kHigh;
    case kHitachiAcFanLow + 1:  return stdAc::fanspeed_t::kMedium;
    case kHitachiAcFanLow:      return stdAc::fanspeed_t::kLow;
    default:                    return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivilant.
/// @return The stdAc equivilant of the native settings.
stdAc::state_t IRHitachiAc::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::HITACHI_AC;
  result.model = -1;  // No models used.
  result.power = this->getPower();
  result.mode = this->toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = this->toCommonFanSpeed(this->getFan());
  result.swingv = this->getSwingVertical() ? stdAc::swingv_t::kAuto :
                                             stdAc::swingv_t::kOff;
  result.swingh = this->getSwingHorizontal() ? stdAc::swingh_t::kAuto :
                                               stdAc::swingh_t::kOff;
  // Not supported.
  result.quiet = false;
  result.turbo = false;
  result.clean = false;
  result.econo = false;
  result.filter = false;
  result.light = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRHitachiAc::toString(void) {
  String result = "";
  result.reserve(110);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(getMode(), kHitachiAcAuto, kHitachiAcCool,
                            kHitachiAcHeat, kHitachiAcDry, kHitachiAcFan);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kHitachiAcFanHigh, kHitachiAcFanLow,
                           kHitachiAcFanAuto, kHitachiAcFanAuto,
                           kHitachiAcFanMed);
  result += addBoolToString(getSwingVertical(), kSwingVStr);
  result += addBoolToString(getSwingHorizontal(), kSwingHStr);
  return result;
}

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRHitachiAc1::IRHitachiAc1(const uint16_t pin, const bool inverted,
                           const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Reset the internal state to a fixed known good state.
void IRHitachiAc1::stateReset(void) {
  for (uint8_t i = 0; i < kHitachiAc1StateLength; i++) remote_state[i] = 0x00;
  // Copy in a known good state.
  remote_state[0] = 0xB2;
  remote_state[1] = 0xAE;
  remote_state[2] = 0x4D;
  remote_state[3] = 0x91;
  remote_state[4] = 0xF0;
  remote_state[5] = 0xE1;
  remote_state[6] = 0xA4;
  remote_state[11] = 0x61;
  remote_state[12] = 0x24;
}

/// Set up hardware to be able to send a message.
void IRHitachiAc1::begin(void) { _irsend.begin(); }

/// Calculate the checksum for a given state.
/// @param[in] state The value to calc the checksum of.
/// @param[in] length The size/length of the state.
/// @return The calculated checksum value.
uint8_t IRHitachiAc1::calcChecksum(const uint8_t state[],
                                   const uint16_t length) {
  uint8_t sum = 0;
  for (uint16_t i = kHitachiAc1ChecksumStartByte; i < length - 1; i++) {
    sum += reverseBits(GETBITS8(state[i], kLowNibble, kNibbleSize),
                       kNibbleSize);
    sum += reverseBits(GETBITS8(state[i], kHighNibble, kNibbleSize),
                       kNibbleSize);
  }
  return reverseBits(sum, 8);
}

/// Calculate and set the checksum values for the internal state.
/// @param[in] length The size/length of the state.
void IRHitachiAc1::checksum(const uint16_t length) {
  remote_state[length - 1] = calcChecksum(remote_state, length);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length of the state array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRHitachiAc1::validChecksum(const uint8_t state[], const uint16_t length) {
  if (length < 2) return true;  // Assume true for lengths that are too short.
  return (state[length - 1] == calcChecksum(state, length));
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRHitachiAc1::getRaw(void) {
  checksum();
  return remote_state;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
/// @param[in] length The length of the new_code array.
void IRHitachiAc1::setRaw(const uint8_t new_code[], const uint16_t length) {
  memcpy(remote_state, new_code, std::min(length, kHitachiAc1StateLength));
}

#if SEND_HITACHI_AC
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRHitachiAc1::send(const uint16_t repeat) {
  _irsend.sendHitachiAC1(getRaw(), kHitachiAc1StateLength, repeat);
  // Clear the toggle bits as we have actioned them by sending them.
  setPowerToggle(false);
  setSwingToggle(false);
}
#endif  // SEND_HITACHI_AC

/// Get/Detect the model of the A/C.
/// @return The enum of the compatible model.
hitachi_ac1_remote_model_t IRHitachiAc1::getModel(void) {
  switch (GETBITS8(remote_state[kHitachiAc1ModelByte], kHitachiAc1ModelOffset,
                   kHitachiAc1ModelSize)) {
    case kHitachiAc1Model_B: return hitachi_ac1_remote_model_t::R_LT0541_HTA_B;
    default:                 return hitachi_ac1_remote_model_t::R_LT0541_HTA_A;
  }
}

/// Set the model of the A/C to emulate.
/// @param[in] model The enum of the appropriate model.
void IRHitachiAc1::setModel(const hitachi_ac1_remote_model_t model) {
  uint8_t value = 0;
  switch (model) {
    case hitachi_ac1_remote_model_t::R_LT0541_HTA_B:
      value = kHitachiAc1Model_B;
      break;
    default:
      value = kHitachiAc1Model_A;  // i.e. 'A' mode.
  }
  setBits(&remote_state[kHitachiAc1ModelByte], kHitachiAc1ModelOffset,
          kHitachiAc1ModelSize, value);
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRHitachiAc1::getPower(void) {
  return GETBIT8(remote_state[kHitachiAc1PowerByte], kHitachiAc1PowerOffset);
}

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHitachiAc1::setPower(const bool on) {
  // If the power changes, set the power toggle bit.
  if (on != getPower()) setPowerToggle(true);
  setBit(&remote_state[kHitachiAc1PowerByte], kHitachiAc1PowerOffset, on);
}

/// Get the value of the current power toggle setting.
/// @return true, the setting is on. false, the setting is off.
bool IRHitachiAc1::getPowerToggle(void) {
  return GETBIT8(remote_state[kHitachiAc1PowerByte],
                 kHitachiAc1PowerToggleOffset);
}

/// Change the power toggle setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHitachiAc1::setPowerToggle(const bool on) {
  setBit(&remote_state[kHitachiAc1PowerByte], kHitachiAc1PowerToggleOffset, on);
}

/// Change the power setting to On.
void IRHitachiAc1::on(void) { setPower(true); }

/// Change the power setting to Off.
void IRHitachiAc1::off(void) { setPower(false); }

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRHitachiAc1::getMode(void) {
  return GETBITS8(remote_state[kHitachiAc1ModeByte], kHitachiAc1ModeOffset,
                  kHitachiAc1ModeSize);
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRHitachiAc1::setMode(const uint8_t mode) {
  switch (mode) {
    case kHitachiAc1Auto:
      setTemp(kHitachiAc1TempAuto);
      // FALL THRU
    case kHitachiAc1Fan:
    case kHitachiAc1Heat:
    case kHitachiAc1Cool:
    case kHitachiAc1Dry:
      setBits(&remote_state[kHitachiAc1ModeByte], kHitachiAc1ModeOffset,
              kHitachiAc1ModeSize, mode);
      setSleep(getSleep());  // Correct the sleep mode if required.
      setFan(getFan());  // Correct the fan speed if required.
      break;
    default: setMode(kHitachiAc1Auto);
  }
  switch (mode) {
    case kHitachiAc1Fan:
    case kHitachiAc1Heat:
      // Auto fan speed not available in these modes, change if needed.
      if (getFan() == kHitachiAc1FanAuto) setFan(kHitachiAc1FanLow);
  }
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRHitachiAc1::getTemp(void) {
  return reverseBits(GETBITS8(remote_state[kHitachiAc1TempByte],
                              kHitachiAc1TempOffset, kHitachiAc1TempSize),
                     kHitachiAc1TempSize) + kHitachiAc1TempDelta;
}

/// Set the temperature.
/// @param[in] celsius The temperature in degrees celsius.
void IRHitachiAc1::setTemp(const uint8_t celsius) {
  if (getMode() == kHitachiAc1Auto) return;  // Can't change temp in Auto mode.
  uint8_t temp = std::min(celsius, kHitachiAcMaxTemp);
  temp = std::max(temp, kHitachiAcMinTemp);
  temp -= kHitachiAc1TempDelta;
  temp = reverseBits(temp, kHitachiAc1TempSize);
  setBits(&remote_state[kHitachiAc1TempByte], kHitachiAc1TempOffset,
          kHitachiAc1TempSize, temp);
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRHitachiAc1::getFan(void) {
  return GETBITS8(remote_state[kHitachiAc1FanByte], kHitachiAc1FanOffset,
                  kHitachiAc1FanSize);
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
/// @param[in] force Do we allow setting the speed regardless of restrictions?
void IRHitachiAc1::setFan(const uint8_t speed, const bool force) {
  if (!force) {
    switch (getMode()) {
      case kHitachiAc1Dry:
        setFan(kHitachiAc1FanLow, true);  // Dry is locked to Low speed.
        return;
      case kHitachiAc1Auto:
        setFan(kHitachiAc1FanAuto, true);  // Auto is locked to Auto speed.
        return;
    }
  }
  switch (speed) {
    case kHitachiAc1FanAuto:
      switch (getMode()) {
        case kHitachiAc1Heat:
        case kHitachiAc1Fan: return;  // Auto speed not allowed in these modes.
      }
      // FALL THRU
    case kHitachiAc1FanHigh:
    case kHitachiAc1FanMed:
    case kHitachiAc1FanLow:
      setBits(&remote_state[kHitachiAc1FanByte], kHitachiAc1FanOffset,
              kHitachiAc1FanSize, speed);
      break;
    default: setFan(kHitachiAc1FanAuto);
  }
}

/// Get the Swing Toggle setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHitachiAc1::getSwingToggle(void) {
  return GETBIT8(remote_state[kHitachiAc1SwingByte],
                 kHitachiAc1SwingToggleOffset);
}

/// Set the Swing toggle setting of the A/C.
/// @param[in] toggle true, the setting is on. false, the setting is off.
void IRHitachiAc1::setSwingToggle(const bool toggle) {
  setBit(&remote_state[kHitachiAc1SwingByte], kHitachiAc1SwingToggleOffset,
         toggle);
}

/// Get the Vertical Swing setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHitachiAc1::getSwingV(void) {
  return GETBIT8(remote_state[kHitachiAc1SwingByte], kHitachiAc1SwingVOffset);
}

/// Set the Vertical Swing setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHitachiAc1::setSwingV(const bool on) {
  setBit(&remote_state[kHitachiAc1SwingByte], kHitachiAc1SwingVOffset, on);
}

/// Get the Horizontal Swing setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHitachiAc1::getSwingH(void) {
  return GETBIT8(remote_state[kHitachiAc1SwingByte], kHitachiAc1SwingHOffset);
}

/// Set the Horizontal Swing setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHitachiAc1::setSwingH(const bool on) {
  setBit(&remote_state[kHitachiAc1SwingByte], kHitachiAc1SwingHOffset, on);
}

/// Get the Sleep setting of the A/C.
/// @return The currently configured sleep mode.
/// @note Sleep modes only available in Auto & Cool modes, otherwise it's off.
uint8_t IRHitachiAc1::getSleep(void) {
  return GETBITS8(remote_state[kHitachiAc1SleepByte], kHitachiAc1SleepOffset,
                  kHitachiAc1SleepSize);
}

/// Set the Sleep setting of the A/C.
/// @param[in] mode The mode of sleep to set the A/C to.
/// @note Sleep modes only available in Auto & Cool modes, otherwise it's off.
void IRHitachiAc1::setSleep(const uint8_t mode) {
  switch (getMode()) {
    case kHitachiAc1Auto:
    case kHitachiAc1Cool:
      setBits(&remote_state[kHitachiAc1SleepByte], kHitachiAc1SleepOffset,
              kHitachiAc1SleepSize, std::min(mode, kHitachiAc1Sleep4));
      break;
    default:
      setBits(&remote_state[kHitachiAc1SleepByte], kHitachiAc1SleepOffset,
              kHitachiAc1SleepSize, kHitachiAc1SleepOff);
  }
}

/// Set the On Timer time.
/// @param[in] mins The time expressed in total number of minutes.
void IRHitachiAc1::setOnTimer(const uint16_t mins) {
  const uint16_t mins_lsb = reverseBits(mins, kHitachiAc1TimerSize);
  remote_state[kHitachiAc1OnTimerLowByte] = GETBITS16(mins_lsb, 8, 8);
  remote_state[kHitachiAc1OnTimerHighByte] = GETBITS16(mins_lsb, 0, 8);
}

/// Get the On Timer vtime of the A/C.
/// @return Nr of minutes the timer is set to.
uint16_t IRHitachiAc1::getOnTimer(void) {
  return reverseBits(
      (remote_state[kHitachiAc1OnTimerLowByte] << 8) |
      remote_state[kHitachiAc1OnTimerHighByte], kHitachiAc1TimerSize);
}

/// Set the Off Timer time.
/// @param[in] mins The time expressed in total number of minutes.
void IRHitachiAc1::setOffTimer(const uint16_t mins) {
  const uint16_t mins_lsb = reverseBits(mins, kHitachiAc1TimerSize);
  remote_state[kHitachiAc1OffTimerLowByte] = GETBITS16(mins_lsb, 8, 8);
  remote_state[kHitachiAc1OffTimerHighByte] = GETBITS16(mins_lsb, 0, 8);
}

/// Get the Off Timer vtime of the A/C.
/// @return Nr of minutes the timer is set to.
uint16_t IRHitachiAc1::getOffTimer(void) {
  return reverseBits(
      (remote_state[kHitachiAc1OffTimerLowByte] << 8) |
      remote_state[kHitachiAc1OffTimerHighByte], kHitachiAc1TimerSize);
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivilant of the enum.
uint8_t IRHitachiAc1::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kHitachiAc1Cool;
    case stdAc::opmode_t::kHeat: return kHitachiAc1Heat;
    case stdAc::opmode_t::kDry:  return kHitachiAc1Dry;
    case stdAc::opmode_t::kFan:  return kHitachiAc1Fan;
    default:                     return kHitachiAc1Auto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivilant of the enum.
uint8_t IRHitachiAc1::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kHitachiAc1FanLow;
    case stdAc::fanspeed_t::kMedium: return kHitachiAc1FanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kHitachiAc1FanHigh;
    default:                         return kHitachiAc1FanAuto;
  }
}

/// Convert a native mode into its stdAc equivilant.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivilant of the native setting.
stdAc::opmode_t IRHitachiAc1::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kHitachiAc1Cool: return stdAc::opmode_t::kCool;
    case kHitachiAc1Heat: return stdAc::opmode_t::kHeat;
    case kHitachiAc1Dry:  return stdAc::opmode_t::kDry;
    case kHitachiAc1Fan:  return stdAc::opmode_t::kFan;
    default:              return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivilant.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivilant of the native setting.
stdAc::fanspeed_t IRHitachiAc1::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kHitachiAc1FanHigh:     return stdAc::fanspeed_t::kMax;
    case kHitachiAc1FanMed:      return stdAc::fanspeed_t::kMedium;
    case kHitachiAc1FanLow:      return stdAc::fanspeed_t::kLow;
    default:                     return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivilant.
/// @return The stdAc equivilant of the native settings.
stdAc::state_t IRHitachiAc1::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::HITACHI_AC1;
  result.model = this->getModel();
  result.power = this->getPower();
  result.mode = this->toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = this->toCommonFanSpeed(this->getFan());
  result.swingv = this->getSwingV() ? stdAc::swingv_t::kAuto :
                                      stdAc::swingv_t::kOff;
  result.swingh = this->getSwingH() ? stdAc::swingh_t::kAuto :
                                      stdAc::swingh_t::kOff;
  result.sleep = this->getSleep() ? 0 : -1;
  // Not supported.
  result.quiet = false;
  result.turbo = false;
  result.clean = false;
  result.econo = false;
  result.filter = false;
  result.light = false;
  result.beep = false;
  result.clock = -1;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRHitachiAc1::toString(void) {
  String result = "";
  result.reserve(170);  // Reserve some heap for the string to reduce fragging.
  result += addModelToString(decode_type_t::HITACHI_AC1, getModel(), false);
  result += addBoolToString(getPower(), kPowerStr);
  result += addBoolToString(getPowerToggle(), kPowerToggleStr);
  result += addModeToString(getMode(), kHitachiAc1Auto, kHitachiAc1Cool,
                            kHitachiAc1Heat, kHitachiAc1Dry, kHitachiAc1Fan);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kHitachiAc1FanHigh, kHitachiAc1FanLow,
                           kHitachiAc1FanAuto, kHitachiAc1FanAuto,
                           kHitachiAc1FanMed);
  result += addBoolToString(getSwingToggle(), kSwingVToggleStr);
  result += addBoolToString(getSwingV(), kSwingVStr);
  result += addBoolToString(getSwingH(), kSwingHStr);
  result += addLabeledString(getSleep() ? uint64ToString(getSleep()) : kOffStr,
                             kSleepStr);
  result += addLabeledString(getOnTimer() ? minsToString(getOnTimer())
                                          : kOffStr,
                            kOnTimerStr);
  result += addLabeledString(getOffTimer() ? minsToString(getOffTimer())
                                           : kOffStr,
                            kOffTimerStr);
  return result;
}

#if (DECODE_HITACHI_AC || DECODE_HITACHI_AC1 || DECODE_HITACHI_AC2 || \
     DECODE_HITACHI_AC344)
/// Decode the supplied Hitachi A/C message.
/// Status: STABLE / Expected to work.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
///   Typically kHitachiAcBits, kHitachiAc1Bits, kHitachiAc2Bits,
///   kHitachiAc344Bits
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @param[in] MSBfirst Is the data per byte stored in MSB First (true) or
///   LSB First order(false)?
/// @return True if it can decode it, false if it can't.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/417
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/453
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1134
bool IRrecv::decodeHitachiAC(decode_results *results, uint16_t offset,
                             const uint16_t nbits, const bool strict,
                             const bool MSBfirst) {
  const uint8_t k_tolerance = _tolerance + 5;

  if (strict) {
    switch (nbits) {
      case kHitachiAcBits:
      case kHitachiAc1Bits:
      case kHitachiAc2Bits:
      case kHitachiAc344Bits:
        break;  // Okay to continue.
      default:
        return false;  // Not strictly a Hitachi message.
    }
  }
  uint16_t hmark;
  uint32_t hspace;
  if (nbits == kHitachiAc1Bits) {
    hmark = kHitachiAc1HdrMark;
    hspace = kHitachiAc1HdrSpace;
  } else {
    hmark = kHitachiAcHdrMark;
    hspace = kHitachiAcHdrSpace;
  }
  // Match Header + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, results->state,
                    results->rawlen - offset, nbits,
                    hmark, hspace,
                    kHitachiAcBitMark, kHitachiAcOneSpace,
                    kHitachiAcBitMark, kHitachiAcZeroSpace,
                    kHitachiAcBitMark, kHitachiAcMinGap, true,
                    k_tolerance, kMarkExcess, MSBfirst)) return false;

  // Compliance
  if (strict) {
    if (nbits / 8 == kHitachiAcStateLength &&
        !IRHitachiAc::validChecksum(results->state, kHitachiAcStateLength))
      return false;
    if (nbits / 8 == kHitachiAc1StateLength &&
       !IRHitachiAc1::validChecksum(results->state, kHitachiAc1StateLength))
      return false;
    if (nbits / 8 == kHitachiAc344StateLength &&
        !IRHitachiAc3::hasInvertedStates(results->state,
                                         kHitachiAc344StateLength))
      return false;
  }

  // Success
  switch (nbits) {
    case kHitachiAc1Bits:
      results->decode_type = decode_type_t::HITACHI_AC1;
      break;
    case kHitachiAc2Bits:
      results->decode_type = decode_type_t::HITACHI_AC2;
      break;
    case kHitachiAc344Bits:
      results->decode_type = decode_type_t::HITACHI_AC344;
      break;
    case kHitachiAcBits:
    default:
      results->decode_type = decode_type_t::HITACHI_AC;
  }
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // (DECODE_HITACHI_AC || DECODE_HITACHI_AC1 || DECODE_HITACHI_AC2 ||
        //  DECODE_HITACHI_AC344)

#if SEND_HITACHI_AC424
/// Send a Hitachi 53-byte/424-bit A/C formatted message. (HITACHI_AC424)
/// Status: STABLE / Reported as working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @note This protocol is almost exactly the same as HitachiAC2 except this
///   variant has a leader section as well, and subtle timing differences.
///   It is also in LSBF order (per byte), rather than MSBF order.
void IRsend::sendHitachiAc424(const uint8_t data[], const uint16_t nbytes,
                              const uint16_t repeat) {
  enableIROut(kHitachiAcFreq);
  for (uint16_t r = 0; r <= repeat; r++) {
    // Leader
    mark(kHitachiAc424LdrMark);
    space(kHitachiAc424LdrSpace);
    // Header + Data + Footer
    sendGeneric(kHitachiAc424HdrMark, kHitachiAc424HdrSpace,
                kHitachiAc424BitMark, kHitachiAc424OneSpace,
                kHitachiAc424BitMark, kHitachiAc424ZeroSpace,
                kHitachiAc424BitMark, kHitachiAcMinGap,
                data, nbytes,  // Bytes
                kHitachiAcFreq, false, kNoRepeat, kDutyDefault);
  }
}
#endif  // SEND_HITACHI_AC424

#if DECODE_HITACHI_AC424
/// Decode the supplied Hitachi 53-byte/424-bit A/C message.
/// Status: STABLE / Reported as working.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
/// @note This protocol is almost exactly the same as HitachiAC2 except this
///   variant has a leader section as well, and subtle timing differences.
///   It is also in LSBF order (per byte), rather than MSBF order.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/973
/// @see (Japanese Manual) https://kadenfan.hitachi.co.jp/support/raj/item/docs/ras_aj22h_a_tori.pdf
bool IRrecv::decodeHitachiAc424(decode_results *results, uint16_t offset,
                                const uint16_t nbits,
                                const bool strict) {
  if (results->rawlen < 2 * nbits + kHeader + kHeader + kFooter - 1 + offset)
    return false;  // Too short a message to match.
  if (strict && nbits != kHitachiAc424Bits)
    return false;

  uint16_t used;

  // Leader
  if (!matchMark(results->rawbuf[offset++], kHitachiAc424LdrMark))
    return false;
  if (!matchSpace(results->rawbuf[offset++], kHitachiAc424LdrSpace))
    return false;

  // Header + Data + Footer
  used = matchGeneric(results->rawbuf + offset, results->state,
                      results->rawlen - offset, nbits,
                      kHitachiAc424HdrMark, kHitachiAc424HdrSpace,
                      kHitachiAc424BitMark, kHitachiAc424OneSpace,
                      kHitachiAc424BitMark, kHitachiAc424ZeroSpace,
                      kHitachiAc424BitMark, kHitachiAcMinGap, true,
                      kUseDefTol, 0, false);
  if (used == 0) return false;  // We failed to find any data.

  // Success
  results->decode_type = decode_type_t::HITACHI_AC424;
  results->bits = nbits;
  return true;
}
#endif  // DECODE_HITACHI_AC424

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRHitachiAc424::IRHitachiAc424(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Reset the internal state to a fixed known good state.
/// @note Reset to auto fan, cooling, 23° Celsius
void IRHitachiAc424::stateReset(void) {
  for (uint8_t i = 0; i < kHitachiAc424StateLength; i++)
    remote_state[i] = 0x00;

  remote_state[0]  = 0x01;
  remote_state[1]  = 0x10;
  remote_state[3]  = 0x40;
  remote_state[5]  = 0xFF;
  remote_state[7]  = 0xCC;
  remote_state[33] = 0x80;
  remote_state[35] = 0x03;
  remote_state[37] = 0x01;
  remote_state[39] = 0x88;
  remote_state[45] = 0xFF;
  remote_state[47] = 0xFF;
  remote_state[49] = 0xFF;
  remote_state[51] = 0xFF;

  setTemp(23);
  setPower(true);
  setMode(kHitachiAc424Cool);
  setFan(kHitachiAc424FanAuto);
}

/// Update the internal consistency check for the protocol.
void IRHitachiAc424::setInvertedStates(void) {
  for (uint8_t i = 3; i < kHitachiAc424StateLength - 1; i += 2)
    remote_state[i + 1] = ~remote_state[i];
}

/// Set up hardware to be able to send a message.
void IRHitachiAc424::begin(void) { _irsend.begin(); }

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRHitachiAc424::getRaw(void) {
  setInvertedStates();
  return remote_state;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
/// @param[in] length The length of the new_code array.
void IRHitachiAc424::setRaw(const uint8_t new_code[], const uint16_t length) {
  memcpy(remote_state, new_code, std::min(length, kHitachiAc424StateLength));
}

#if SEND_HITACHI_AC424
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRHitachiAc424::send(const uint16_t repeat) {
  _irsend.sendHitachiAc424(getRaw(), kHitachiAc424StateLength, repeat);
}
#endif  // SEND_HITACHI_AC424

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRHitachiAc424::getPower(void) {
  return remote_state[kHitachiAc424PowerByte] == kHitachiAc424PowerOn;
}

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHitachiAc424::setPower(const bool on) {
  setButton(kHitachiAc424ButtonPowerMode);
  remote_state[kHitachiAc424PowerByte] = on ? kHitachiAc424PowerOn
    : kHitachiAc424PowerOff;
}

/// Change the power setting to On.
void IRHitachiAc424::on(void) { setPower(true); }

/// Change the power setting to Off.
void IRHitachiAc424::off(void) { setPower(false); }

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRHitachiAc424::getMode(void) {
  return GETBITS8(remote_state[kHitachiAc424ModeByte], kLowNibble, kNibbleSize);
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRHitachiAc424::setMode(const uint8_t mode) {
  uint8_t newMode = mode;
  switch (mode) {
    // Fan mode sets a special temp.
    case kHitachiAc424Fan: setTemp(kHitachiAc424FanTemp, false); break;
    case kHitachiAc424Heat:
    case kHitachiAc424Cool:
    case kHitachiAc424Dry: break;
    default: newMode = kHitachiAc424Cool;
  }
  setBits(&remote_state[kHitachiAc424ModeByte], kLowNibble, kNibbleSize,
          newMode);
  if (newMode != kHitachiAc424Fan) setTemp(_previoustemp);
  setFan(getFan());  // Reset the fan speed after the mode change.
  setButton(kHitachiAc424ButtonPowerMode);
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRHitachiAc424::getTemp(void) {
  return GETBITS8(remote_state[kHitachiAc424TempByte], kHitachiAc424TempOffset,
                  kHitachiAc424TempSize);
}

/// Set the temperature.
/// @param[in] celsius The temperature in degrees celsius.
/// @param[in] setPrevious true, remember this if we change mode. false, don't.
void IRHitachiAc424::setTemp(const uint8_t celsius, bool setPrevious) {
  uint8_t temp;
  temp = std::min(celsius, kHitachiAc424MaxTemp);
  temp = std::max(temp, kHitachiAc424MinTemp);
  setBits(&remote_state[kHitachiAc424TempByte], kHitachiAc424TempOffset,
          kHitachiAc424TempSize, temp);
  if (_previoustemp > temp)
    setButton(kHitachiAc424ButtonTempDown);
  else if (_previoustemp < temp)
    setButton(kHitachiAc424ButtonTempUp);
  if (setPrevious) _previoustemp = temp;
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRHitachiAc424::getFan(void) {
  return GETBITS8(remote_state[kHitachiAc424FanByte], kHighNibble, kNibbleSize);
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRHitachiAc424::setFan(const uint8_t speed) {
  uint8_t newSpeed = std::max(speed, kHitachiAc424FanMin);
  uint8_t fanMax = kHitachiAc424FanMax;

  // Only 2 x low speeds in Dry mode or Auto
  if (getMode() == kHitachiAc424Dry && speed == kHitachiAc424FanAuto) {
    fanMax = kHitachiAc424FanAuto;
  } else if (getMode() == kHitachiAc424Dry) {
    fanMax = kHitachiAc424FanMaxDry;
  } else if (getMode() == kHitachiAc424Fan && speed == kHitachiAc424FanAuto) {
    // Fan Mode does not have auto. Set to safe low
    newSpeed = kHitachiAc424FanMin;
  }

  newSpeed = std::min(newSpeed, fanMax);
  // Handle the setting the button value if we are going to change the value.
  if (newSpeed != getFan()) setButton(kHitachiAc424ButtonFan);
  // Set the values
  setBits(&remote_state[kHitachiAc424FanByte], kHighNibble, kNibbleSize,
          newSpeed);
  remote_state[9] = 0x92;
  remote_state[29] = 0x00;

  // When fan is at min/max, additional bytes seem to be set
  if (newSpeed == kHitachiAc424FanMin) remote_state[9] = 0x98;
  if (newSpeed == kHitachiAc424FanMax) {
    remote_state[9] = 0xA9;
    remote_state[29] = 0x30;
  }
}

/// Get the Button/Command setting of the A/C.
/// @return The value of the button/command that was pressed.
uint8_t IRHitachiAc424::getButton(void) {
  return remote_state[kHitachiAc424ButtonByte];
}

/// Set the Button/Command pressed setting of the A/C.
/// @param[in] button The value of the button/command that was pressed.
void IRHitachiAc424::setButton(const uint8_t button) {
  remote_state[kHitachiAc424ButtonByte] = button;
}

/// Set the Vertical Swing toggle setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @note The remote does not keep state of the vertical swing.
///   A byte is sent indicating the swing button is pressed on the remote
void IRHitachiAc424::setSwingVToggle(const bool on) {
  uint8_t button = getButton();  // Get the current button value.
  if (on)
    button = kHitachiAc424ButtonSwingV;  // Set the button to SwingV.
  else if (button == kHitachiAc424ButtonSwingV)  // Asked to unset it
    // It was set previous, so use Power as a default
    button = kHitachiAc424ButtonPowerMode;
  setButton(button);
}

/// Get the Vertical Swing toggle setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHitachiAc424::getSwingVToggle(void) {
  return getButton() == kHitachiAc424ButtonSwingV;
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivilant of the enum.
uint8_t IRHitachiAc424::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kHitachiAc424Cool;
    case stdAc::opmode_t::kHeat: return kHitachiAc424Heat;
    case stdAc::opmode_t::kDry:  return kHitachiAc424Dry;
    case stdAc::opmode_t::kFan:  return kHitachiAc424Fan;
    default:                     return kHitachiAc424Cool;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivilant of the enum.
uint8_t IRHitachiAc424::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:    return kHitachiAc424FanMin;
    case stdAc::fanspeed_t::kLow:    return kHitachiAc424FanLow;
    case stdAc::fanspeed_t::kMedium: return kHitachiAc424FanMedium;
    case stdAc::fanspeed_t::kHigh:   return kHitachiAc424FanHigh;
    case stdAc::fanspeed_t::kMax:    return kHitachiAc424FanMax;
    default:                         return kHitachiAc424FanAuto;
  }
}

/// Convert a native mode into its stdAc equivilant.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivilant of the native setting.
stdAc::opmode_t IRHitachiAc424::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kHitachiAc424Cool: return stdAc::opmode_t::kCool;
    case kHitachiAc424Heat: return stdAc::opmode_t::kHeat;
    case kHitachiAc424Dry:  return stdAc::opmode_t::kDry;
    case kHitachiAc424Fan:  return stdAc::opmode_t::kFan;
    default:                return stdAc::opmode_t::kCool;
  }
}

/// Convert a native fan speed into its stdAc equivilant.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivilant of the native setting.
stdAc::fanspeed_t IRHitachiAc424::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kHitachiAc424FanMax:    return stdAc::fanspeed_t::kMax;
    case kHitachiAc424FanHigh:   return stdAc::fanspeed_t::kHigh;
    case kHitachiAc424FanMedium: return stdAc::fanspeed_t::kMedium;
    case kHitachiAc424FanLow:    return stdAc::fanspeed_t::kLow;
    case kHitachiAc424FanMin:    return stdAc::fanspeed_t::kMin;
    default:                     return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivilant.
/// @return The stdAc equivilant of the native settings.
stdAc::state_t IRHitachiAc424::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::HITACHI_AC424;
  result.model = -1;  // No models used.
  result.power = this->getPower();
  result.mode = this->toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = this->toCommonFanSpeed(this->getFan());
  result.swingv = this->getSwingVToggle() ? stdAc::swingv_t::kAuto
                                                 : stdAc::swingv_t::kOff;
  // Not supported.
  result.swingh = stdAc::swingh_t::kOff;
  result.quiet = false;
  result.turbo = false;
  result.clean = false;
  result.econo = false;
  result.filter = false;
  result.light = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

/// Convert the internal state into a human readable string for the settings
/// that are common to protocols of this nature.
/// @return A string containing the common settings in human-readable form.
String IRHitachiAc424::_toString(void) {
  String result = "";
  result.reserve(100);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(getMode(), 0, kHitachiAc424Cool,
                            kHitachiAc424Heat, kHitachiAc424Dry,
                            kHitachiAc424Fan);
  result += addTempToString(getTemp());
  result += addIntToString(getFan(), kFanStr);
  result += kSpaceLBraceStr;
  switch (getFan()) {
    case kHitachiAc424FanAuto:   result += kAutoStr; break;
    case kHitachiAc424FanMax:    result += kMaxStr; break;
    case kHitachiAc424FanHigh:   result += kHighStr; break;
    case kHitachiAc424FanMedium: result += kMedStr; break;
    case kHitachiAc424FanLow:    result += kLowStr; break;
    case kHitachiAc424FanMin:    result += kMinStr; break;
    default:                     result += kUnknownStr;
  }
  result += ')';
  result += addIntToString(getButton(), kButtonStr);
  result += kSpaceLBraceStr;
  switch (getButton()) {
    case kHitachiAc424ButtonPowerMode:
      result += kPowerStr;
      result += '/';
      result += kModeStr;
      break;
    case kHitachiAc424ButtonFan:      result += kFanStr; break;
    case kHitachiAc424ButtonSwingV:   result += kSwingVStr; break;
    case kHitachiAc344ButtonSwingH:   result += kSwingHStr; break;
    case kHitachiAc424ButtonTempDown: result += kTempDownStr; break;
    case kHitachiAc424ButtonTempUp:   result += kTempUpStr; break;
    default: result += kUnknownStr;
  }
  result += ')';
  return result;
}

/// Convert the internal state into a human readable string.
/// @return A string containing the settings in human-readable form.
String IRHitachiAc424::toString(void) {
  return _toString() + addBoolToString(getSwingVToggle(), kSwingVToggleStr);
}


#if SEND_HITACHI_AC3
/// Send a Hitachi(3) A/C formatted message. (HITACHI_AC3)
/// Status: STABLE / Working fine.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @note This protocol is almost exactly the same as HitachiAC424 except this
///   variant has subtle timing differences. There are five(5) typical sizes:
///     kHitachiAc3MinStateLength (Cancel Timer),
///     kHitachiAc3MinStateLength + 2 (Change Temp),
///     kHitachiAc3StateLength - 6 (Change Mode),
///     kHitachiAc3StateLength - 4 (Normal), &
///     kHitachiAc3StateLength (Set Timer)
void IRsend::sendHitachiAc3(const uint8_t data[], const uint16_t nbytes,
                              const uint16_t repeat) {
  // Header + Data + Footer
  sendGeneric(kHitachiAc3HdrMark, kHitachiAc3HdrSpace,
              kHitachiAc3BitMark, kHitachiAc3OneSpace,
              kHitachiAc3BitMark, kHitachiAc3ZeroSpace,
              kHitachiAc3BitMark, kHitachiAcMinGap,
              data, nbytes,  // Bytes
              kHitachiAcFreq, false, repeat, kDutyDefault);
}
#endif  // SEND_HITACHI_AC3


/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRHitachiAc3::IRHitachiAc3(const uint16_t pin, const bool inverted,
                           const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Reset the internal state to a fixed known good state.
/// @note Reset to auto fan, cooling, 23° Celsius
void IRHitachiAc3::stateReset(void) {
  for (uint8_t i = 0; i < kHitachiAc3StateLength; i++)
    remote_state[i] = 0x00;
  remote_state[0]  = 0x01;
  remote_state[1]  = 0x10;
  remote_state[3]  = 0x40;
  remote_state[5]  = 0xFF;
  remote_state[7]  = 0xE8;
  remote_state[9]  = 0x89;
  remote_state[11] = 0x0B;
  remote_state[13] = 0x3F;
  remote_state[15] = 0x15;
  remote_state[21] = 0x4B;
  remote_state[23] = 0x18;
  setInvertedStates();
}

/// Invert every second byte of the internal state, after the fixed header.
/// @param[in] length The size of the state array.
/// @note This is this protocols integrity check.
void IRHitachiAc3::setInvertedStates(const uint16_t length) {
  for (uint8_t i = 3; i < length - 1; i += 2)
    remote_state[i + 1] = ~remote_state[i];
}

/// Check if every second byte of the state, after the fixed header
///   is inverted to the previous byte.
/// @param[in] state The state array to be checked.
/// @param[in] length The size of the state array.
/// @note This is this protocols integrity check.
bool IRHitachiAc3::hasInvertedStates(const uint8_t state[],
                                     const uint16_t length) {
  for (uint8_t i = 3; i < length - 1; i += 2)
    if ((state[i + 1] ^ state[i]) != 0xFF) return false;
  return true;
}

/// Set up hardware to be able to send a message.
void IRHitachiAc3::begin(void) { _irsend.begin(); }

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRHitachiAc3::getRaw(void) {
  setInvertedStates();
  return remote_state;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
/// @param[in] length The length of the new_code array.
void IRHitachiAc3::setRaw(const uint8_t new_code[], const uint16_t length) {
  memcpy(remote_state, new_code, std::min(length, kHitachiAc3StateLength));
}

#if DECODE_HITACHI_AC3
/// Decode the supplied Hitachi 15to27-byte/120to216-bit A/C message.
/// Status: STABLE / Works fine.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
/// @note This protocol is almost exactly the same as HitachiAC424 except this
///   variant has subtle timing differences and multiple lengths.
///   There are five(5) typical lengths:
///     kHitachiAc3MinStateLength (Cancel Timer),
///     kHitachiAc3MinStateLength + 2 (Change Temp),
///     kHitachiAc3StateLength - 6 (Change Mode),
///     kHitachiAc3StateLength - 4 (Normal), &
///     kHitachiAc3StateLength (Set Timer)
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1060
bool IRrecv::decodeHitachiAc3(decode_results *results, uint16_t offset,
                                const uint16_t nbits,
                                const bool strict) {
  if (results->rawlen < 2 * nbits + kHeader + kFooter - 1 + offset)
    return false;  // Too short a message to match.
  if (strict) {
    // Check the requested bit length.
    switch (nbits) {
      case kHitachiAc3MinBits:  // Cancel Timer (Min Size)
      case kHitachiAc3MinBits + 2 * 8:  // Change Temp
      case kHitachiAc3Bits - 6 * 8:  // Change Mode
      case kHitachiAc3Bits - 4 * 8:  // Normal
      case kHitachiAc3Bits:  // Set Temp (Max Size)
        break;
      default: return false;
    }
  }

  // Header + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, results->state,
                    results->rawlen - offset, nbits,
                    kHitachiAc3HdrMark, kHitachiAc3HdrSpace,
                    kHitachiAc3BitMark, kHitachiAc3OneSpace,
                    kHitachiAc3BitMark, kHitachiAc3ZeroSpace,
                    kHitachiAc3BitMark, kHitachiAcMinGap, true,
                    kUseDefTol, 0, false))
    return false;  // We failed to find any data.

  // Compliance
  if (strict && !IRHitachiAc3::hasInvertedStates(results->state, nbits / 8))
    return false;
  // Success
  results->decode_type = decode_type_t::HITACHI_AC3;
  results->bits = nbits;
  return true;
}
#endif  // DECODE_HITACHI_AC3

/// Class constructor for handling detailed Hitachi_AC344 43 byte A/C messages.
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRHitachiAc344::IRHitachiAc344(const uint16_t pin, const bool inverted,
                               const bool use_modulation)
    : IRHitachiAc424(pin, inverted, use_modulation) { stateReset(); }

/// Reset the internal state to auto fan, cooling, 23° Celsius
void IRHitachiAc344::stateReset(void) {
  IRHitachiAc424::stateReset();
  remote_state[37] = 0x00;
  remote_state[39] = 0x00;
}

#if SEND_HITACHI_AC344
/// Create and send the IR message to the A/C.
/// @param[in] repeat Nr. of times to repeat the message.
void IRHitachiAc344::send(const uint16_t repeat) {
  _irsend.sendHitachiAc344(getRaw(), kHitachiAc344StateLength, repeat);
}
#endif  // SEND_HITACHI_AC344

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
/// @param[in] length Size (in bytes) of the code for this protocol.
void IRHitachiAc344::setRaw(const uint8_t new_code[], const uint16_t length) {
  memcpy(remote_state, new_code, std::min(length, kHitachiAc344StateLength));
}

/// Control the vertical swing setting.
/// @param[in] on True, turns on the feature. False, turns off the feature.
void IRHitachiAc344::setSwingV(const bool on) {
  setSwingVToggle(on);  // Set the button value.
  setBit(&remote_state[kHitachiAc344SwingVByte], kHitachiAc344SwingVOffset, on);
}

/// Get the current vertical swing setting.
/// @return True, if the setting is on. False, it is off.
bool IRHitachiAc344::getSwingV(void) {
  return GETBIT8(remote_state[kHitachiAc344SwingVByte],
                 kHitachiAc344SwingVOffset);
}

/// Control the horizontal swing setting.
/// @param[in] position The position to set the horizontal swing to.
void IRHitachiAc344::setSwingH(const uint8_t position) {
  if (position > kHitachiAc344SwingHLeftMax)
    return setSwingH(kHitachiAc344SwingHMiddle);
  setBits(&remote_state[kHitachiAc344SwingHByte], kHitachiAc344SwingHOffset,
          kHitachiAc344SwingHSize, position);
  setButton(kHitachiAc344ButtonSwingH);
}

/// Get the current horizontal swing setting.
/// @return The current position horizontal swing is set to.
uint8_t IRHitachiAc344::getSwingH(void) {
  return GETBITS8(remote_state[kHitachiAc344SwingHByte],
                  kHitachiAc344SwingHOffset, kHitachiAc344SwingHSize);
}

/// Convert a standard A/C horizontal swing into its native setting.
/// @param[in] position A stdAc::swingh_t position to convert.
/// @return The equivilent native horizontal swing position.
uint8_t IRHitachiAc344::convertSwingH(const stdAc::swingh_t position) {
  switch (position) {
    case stdAc::swingh_t::kAuto:     return kHitachiAc344SwingHAuto;
    case stdAc::swingh_t::kLeftMax:  return kHitachiAc344SwingHLeftMax;
    case stdAc::swingh_t::kLeft:     return kHitachiAc344SwingHLeft;
    case stdAc::swingh_t::kRight:    return kHitachiAc344SwingHRight;
    case stdAc::swingh_t::kRightMax: return kHitachiAc344SwingHRightMax;
    default:                         return kHitachiAc344SwingHMiddle;
  }
}

/// Convert a native horizontal swing postion to it's common equivalent.
/// @param[in] pos A native position to convert.
/// @return The common horizontal swing position.
stdAc::swingh_t IRHitachiAc344::toCommonSwingH(const uint8_t pos) {
  switch (pos) {
    case kHitachiAc344SwingHLeftMax:  return stdAc::swingh_t::kLeftMax;
    case kHitachiAc344SwingHLeft:     return stdAc::swingh_t::kLeft;
    case kHitachiAc344SwingHRight:    return stdAc::swingh_t::kRight;
    case kHitachiAc344SwingHRightMax: return stdAc::swingh_t::kRightMax;
    case kHitachiAc344SwingHAuto:     return stdAc::swingh_t::kAuto;
    default:                          return stdAc::swingh_t::kOff;
  }
}

/// Convert the current internal state into its stdAc::state_t equivilant.
/// @return The stdAc equivilant of the native settings.
stdAc::state_t IRHitachiAc344::toCommon(void) {
  stdAc::state_t result = IRHitachiAc424::toCommon();
  result.protocol = decode_type_t::HITACHI_AC344;
  result.swingv = getSwingV() ? stdAc::swingv_t::kAuto : stdAc::swingv_t::kOff;
  result.swingh = toCommonSwingH(getSwingH());
  return result;
}

/// Convert the internal state into a human readable string.
/// @return A string containing the settings in human-readable form.
String IRHitachiAc344::toString(void) {
  String result;
  result.reserve(120);  // Reserve some heap for the string to reduce fragging.
  result += _toString();
  result += addBoolToString(getSwingV(), kSwingVStr);
  result += addIntToString(getSwingH(), kSwingHStr);
  result += kSpaceLBraceStr;
  switch (getSwingH()) {
    case kHitachiAc344SwingHLeftMax:  result += kLeftMaxStr; break;
    case kHitachiAc344SwingHLeft:     result += kLeftStr; break;
    case kHitachiAc344SwingHMiddle:   result += kMiddleStr; break;
    case kHitachiAc344SwingHRight:    result += kRightStr; break;
    case kHitachiAc344SwingHRightMax: result += kRightMaxStr; break;
    case kHitachiAc344SwingHAuto:     result += kAutoStr; break;
    default:                          result += kUnknownStr;
  }
  result += ')';
  return result;
}
