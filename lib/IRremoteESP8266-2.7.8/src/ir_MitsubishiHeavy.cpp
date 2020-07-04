// Copyright 2019 David Conran

/// @file
/// @brief Support for Mitsubishi Heavy Industry protocols.
/// Code to emulate Mitsubishi Heavy Industries A/C IR remote control units.
/// @note This code was *heavily* influenced by ToniA's great work & code,
///   but it has been written from scratch.
///   Nothing was copied other than constants and message analysis.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/660
/// @see https://github.com/ToniA/Raw-IR-decoder-for-Arduino/blob/master/MitsubishiHeavy.cpp
/// @see https://github.com/ToniA/arduino-heatpumpir/blob/master/MitsubishiHeavyHeatpumpIR.cpp

#include "ir_MitsubishiHeavy.h"
#include <algorithm>
#include <cstring>
#include "IRremoteESP8266.h"
#include "IRtext.h"
#include "IRutils.h"
#ifndef ARDUINO
#include <string>
#endif

// Constants
const uint16_t kMitsubishiHeavyHdrMark = 3140;
const uint16_t kMitsubishiHeavyHdrSpace = 1630;
const uint16_t kMitsubishiHeavyBitMark = 370;
const uint16_t kMitsubishiHeavyOneSpace = 420;
const uint16_t kMitsubishiHeavyZeroSpace = 1220;
const uint32_t kMitsubishiHeavyGap = kDefaultMessageGap;  // Just a guess.

using irutils::addBoolToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;
using irutils::setBit;
using irutils::setBits;

#if SEND_MITSUBISHIHEAVY
/// Send a MitsubishiHeavy 88-bit A/C message.
/// Status: BETA / Appears to be working. Needs testing against a real device.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendMitsubishiHeavy88(const unsigned char data[],
                                   const uint16_t nbytes,
                                   const uint16_t repeat) {
  if (nbytes < kMitsubishiHeavy88StateLength)
    return;  // Not enough bytes to send a proper message.
  sendGeneric(kMitsubishiHeavyHdrMark, kMitsubishiHeavyHdrSpace,
              kMitsubishiHeavyBitMark, kMitsubishiHeavyOneSpace,
              kMitsubishiHeavyBitMark, kMitsubishiHeavyZeroSpace,
              kMitsubishiHeavyBitMark, kMitsubishiHeavyGap,
              data, nbytes, 38000, false, repeat, kDutyDefault);
}

/// Send a MitsubishiHeavy 152-bit A/C message.
/// Status: BETA / Appears to be working. Needs testing against a real device.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendMitsubishiHeavy152(const unsigned char data[],
                                    const uint16_t nbytes,
                                    const uint16_t repeat) {
  if (nbytes < kMitsubishiHeavy152StateLength)
    return;  // Not enough bytes to send a proper message.
  sendMitsubishiHeavy88(data, nbytes, repeat);
}
#endif  // SEND_MITSUBISHIHEAVY

// Class for decoding and constructing MitsubishiHeavy152 AC messages.

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRMitsubishiHeavy152Ac::IRMitsubishiHeavy152Ac(const uint16_t pin,
                                               const bool inverted,
                                               const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Set up hardware to be able to send a message.
void IRMitsubishiHeavy152Ac::begin(void) { _irsend.begin(); }

#if SEND_MITSUBISHIHEAVY
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRMitsubishiHeavy152Ac::send(const uint16_t repeat) {
  _irsend.sendMitsubishiHeavy152(this->getRaw(), kMitsubishiHeavy152StateLength,
                                 repeat);
}
#endif  // SEND_MITSUBISHIHEAVY

/// Reset the state of the remote to a known good state/sequence.
void IRMitsubishiHeavy152Ac::stateReset(void) {
  memcpy(remote_state, kMitsubishiHeavyZmsSig, kMitsubishiHeavySigLength);
  for (uint8_t i = kMitsubishiHeavySigLength;
       i < kMitsubishiHeavy152StateLength - 3; i += 2) remote_state[i] = 0;
  remote_state[17] = 0x80;
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRMitsubishiHeavy152Ac::getRaw(void) {
  checksum();
  return remote_state;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] data A valid code for this protocol.
void IRMitsubishiHeavy152Ac::setRaw(const uint8_t *data) {
  memcpy(remote_state, data, kMitsubishiHeavy152StateLength);
}

/// Set the requested power state of the A/C to on.
void IRMitsubishiHeavy152Ac::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void IRMitsubishiHeavy152Ac::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiHeavy152Ac::setPower(const bool on) {
  setBit(&remote_state[5], kMitsubishiHeavyPowerOffset, on);
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiHeavy152Ac::getPower(void) {
  return GETBIT8(remote_state[5], kMitsubishiHeavyPowerOffset);
}

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
void IRMitsubishiHeavy152Ac::setTemp(const uint8_t temp) {
  uint8_t newtemp = temp;
  newtemp = std::min(newtemp, kMitsubishiHeavyMaxTemp);
  newtemp = std::max(newtemp, kMitsubishiHeavyMinTemp);
  setBits(&remote_state[7], kLowNibble, kNibbleSize,
          newtemp - kMitsubishiHeavyMinTemp);
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRMitsubishiHeavy152Ac::getTemp(void) {
  return GETBITS8(remote_state[7], kLowNibble, kNibbleSize) +
      kMitsubishiHeavyMinTemp;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRMitsubishiHeavy152Ac::setFan(const uint8_t speed) {
  uint8_t newspeed = speed;
  switch (speed) {
    case kMitsubishiHeavy152FanLow:
    case kMitsubishiHeavy152FanMed:
    case kMitsubishiHeavy152FanHigh:
    case kMitsubishiHeavy152FanMax:
    case kMitsubishiHeavy152FanEcono:
    case kMitsubishiHeavy152FanTurbo: break;
    default: newspeed = kMitsubishiHeavy152FanAuto;
  }
  setBits(&remote_state[9], kLowNibble, kNibbleSize, newspeed);
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t IRMitsubishiHeavy152Ac::getFan(void) {
  return GETBITS8(remote_state[9], kLowNibble, kNibbleSize);
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRMitsubishiHeavy152Ac::setMode(const uint8_t mode) {
  uint8_t newmode = mode;
  switch (mode) {
    case kMitsubishiHeavyCool:
    case kMitsubishiHeavyDry:
    case kMitsubishiHeavyFan:
    case kMitsubishiHeavyHeat:
      break;
    default:
      newmode = kMitsubishiHeavyAuto;
  }
  setBits(&remote_state[5], kMitsubishiHeavyModeOffset, kModeBitsSize, newmode);
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRMitsubishiHeavy152Ac::getMode(void) {
  return GETBITS8(remote_state[5], kMitsubishiHeavyModeOffset, kModeBitsSize);
}

/// Set the Vertical Swing mode of the A/C.
/// @param[in] pos The position/mode to set the swing to.
void IRMitsubishiHeavy152Ac::setSwingVertical(const uint8_t pos) {
  setBits(&remote_state[11], kMitsubishiHeavy152SwingVOffset,
          kMitsubishiHeavy152SwingVSize,
          std::min(pos, kMitsubishiHeavy152SwingVOff));
}

/// Get the Vertical Swing mode of the A/C.
/// @return The native position/mode setting.
uint8_t IRMitsubishiHeavy152Ac::getSwingVertical(void) {
  return GETBITS8(remote_state[11], kMitsubishiHeavy152SwingVOffset,
                  kMitsubishiHeavy152SwingVSize);
}

/// Set the Horizontal Swing mode of the A/C.
/// @param[in] pos The position/mode to set the swing to.
void IRMitsubishiHeavy152Ac::setSwingHorizontal(const uint8_t pos) {
  setBits(&remote_state[13], kLowNibble, kNibbleSize,
          std::min(pos, kMitsubishiHeavy152SwingHOff));
}

/// Get the Horizontal Swing mode of the A/C.
/// @return The native position/mode setting.
uint8_t IRMitsubishiHeavy152Ac::getSwingHorizontal(void) {
  return GETBITS8(remote_state[13], kLowNibble, kNibbleSize);
}

/// Set the Night (Sleep) mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiHeavy152Ac::setNight(const bool on) {
  setBit(&remote_state[15], kMitsubishiHeavyNightOffset, on);
}

/// Get the Night (Sleep) mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiHeavy152Ac::getNight(void) {
  return GETBIT8(remote_state[15], kMitsubishiHeavyNightOffset);
}

/// Set the 3D mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiHeavy152Ac::set3D(const bool on) {
  if (on)
    remote_state[11] |= kMitsubishiHeavy3DMask;
  else
    remote_state[11] &= ~kMitsubishiHeavy3DMask;
}

/// Get the 3D mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiHeavy152Ac::get3D(void) {
  return (remote_state[11] & kMitsubishiHeavy3DMask) == kMitsubishiHeavy3DMask;
}

/// Set the Silent (Quiet) mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiHeavy152Ac::setSilent(const bool on) {
  setBit(&remote_state[15], kMitsubishiHeavySilentOffset, on);
}

/// Get the Silent (Quiet) mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiHeavy152Ac::getSilent(void) {
  return GETBIT8(remote_state[15], kMitsubishiHeavySilentOffset);
}

/// Set the Filter mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiHeavy152Ac::setFilter(const bool on) {
  setBit(&remote_state[5], kMitsubishiHeavyFilterOffset, on);
}

/// Get the Filter mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiHeavy152Ac::getFilter(void) {
  return GETBIT8(remote_state[5], kMitsubishiHeavyFilterOffset);
}

/// Set the Clean mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiHeavy152Ac::setClean(const bool on) {
  this->setFilter(on);
  setBit(&remote_state[5], kMitsubishiHeavyCleanOffset, on);
}

/// Get the Clean mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiHeavy152Ac::getClean(void) {
  return GETBIT8(remote_state[5], kMitsubishiHeavyCleanOffset) && getFilter();
}

/// Set the Turbo mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiHeavy152Ac::setTurbo(const bool on) {
  if (on)
    this->setFan(kMitsubishiHeavy152FanTurbo);
  else if (this->getTurbo()) this->setFan(kMitsubishiHeavy152FanAuto);
}

/// Get the Turbo mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiHeavy152Ac::getTurbo(void) {
  return this->getFan() == kMitsubishiHeavy152FanTurbo;
}

/// Set the Economical mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiHeavy152Ac::setEcono(const bool on) {
  if (on)
    this->setFan(kMitsubishiHeavy152FanEcono);
  else if (this->getEcono()) this->setFan(kMitsubishiHeavy152FanAuto);
}

/// Get the Economical mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiHeavy152Ac::getEcono(void) {
  return this->getFan() == kMitsubishiHeavy152FanEcono;
}

/// Verify the given state has a ZM-S signature.
/// @param[in] state A ptr to a state to be checked.
/// @return true, the check passed. Otherwise, false.
bool IRMitsubishiHeavy152Ac::checkZmsSig(const uint8_t *state) {
  for (uint8_t i = 0; i < kMitsubishiHeavySigLength; i++)
    if (state[i] != kMitsubishiHeavyZmsSig[i]) return false;
  return true;
}

/// Calculate the checksum for the current internal state of the remote.
/// Note: Technically it has no checksum, but does has inverted byte pairs.
void IRMitsubishiHeavy152Ac::checksum(void) {
  for (uint8_t i = kMitsubishiHeavySigLength - 2;
       i < kMitsubishiHeavy152StateLength;
       i += 2) {
    remote_state[i + 1] = ~remote_state[i];
  }
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length/size of the state array.
/// @return true, if the state has a valid checksum. Otherwise, false.
/// Note: Technically it has no checksum, but does has inverted byte pairs.
bool IRMitsubishiHeavy152Ac::validChecksum(const uint8_t *state,
                                           const uint16_t length) {
  // Assume anything too short is fine.
  if (length < kMitsubishiHeavySigLength) return true;
  // Check all the byte pairs.
  for (uint16_t i = kMitsubishiHeavySigLength - 2;
       i < length;
       i += 2) {
    // XOR of a byte and it's self inverted should be 0xFF;
    if ((state[i] ^ state[i + 1]) != 0xFF) return false;
  }
  return true;
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivilant of the enum.
uint8_t IRMitsubishiHeavy152Ac::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kMitsubishiHeavyCool;
    case stdAc::opmode_t::kHeat: return kMitsubishiHeavyHeat;
    case stdAc::opmode_t::kDry:  return kMitsubishiHeavyDry;
    case stdAc::opmode_t::kFan:  return kMitsubishiHeavyFan;
    default:                     return kMitsubishiHeavyAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivilant of the enum.
uint8_t IRMitsubishiHeavy152Ac::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    // Assumes Econo is slower than Low.
    case stdAc::fanspeed_t::kMin:    return kMitsubishiHeavy152FanEcono;
    case stdAc::fanspeed_t::kLow:    return kMitsubishiHeavy152FanLow;
    case stdAc::fanspeed_t::kMedium: return kMitsubishiHeavy152FanMed;
    case stdAc::fanspeed_t::kHigh:   return kMitsubishiHeavy152FanHigh;
    case stdAc::fanspeed_t::kMax:    return kMitsubishiHeavy152FanMax;
    default:                         return kMitsubishiHeavy152FanAuto;
  }
}

/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivilant of the enum.
uint8_t IRMitsubishiHeavy152Ac::convertSwingV(const stdAc::swingv_t position) {
  switch (position) {
    case stdAc::swingv_t::kAuto:    return kMitsubishiHeavy152SwingVAuto;
    case stdAc::swingv_t::kHighest: return kMitsubishiHeavy152SwingVHighest;
    case stdAc::swingv_t::kHigh:    return kMitsubishiHeavy152SwingVHigh;
    case stdAc::swingv_t::kMiddle:  return kMitsubishiHeavy152SwingVMiddle;
    case stdAc::swingv_t::kLow:     return kMitsubishiHeavy152SwingVLow;
    case stdAc::swingv_t::kLowest:  return kMitsubishiHeavy152SwingVLowest;
    default:                        return kMitsubishiHeavy152SwingVOff;
  }
}

/// Convert a stdAc::swingh_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivilant of the enum.
uint8_t IRMitsubishiHeavy152Ac::convertSwingH(const stdAc::swingh_t position) {
  switch (position) {
    case stdAc::swingh_t::kAuto:     return kMitsubishiHeavy152SwingHAuto;
    case stdAc::swingh_t::kLeftMax:  return kMitsubishiHeavy152SwingHLeftMax;
    case stdAc::swingh_t::kLeft:     return kMitsubishiHeavy152SwingHLeft;
    case stdAc::swingh_t::kMiddle:   return kMitsubishiHeavy152SwingHMiddle;
    case stdAc::swingh_t::kRight:    return kMitsubishiHeavy152SwingHRight;
    case stdAc::swingh_t::kRightMax: return kMitsubishiHeavy152SwingHRightMax;
    default:                         return kMitsubishiHeavy152SwingHOff;
  }
}

/// Convert a native mode into its stdAc equivilant.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivilant of the native setting.
stdAc::opmode_t IRMitsubishiHeavy152Ac::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kMitsubishiHeavyCool: return stdAc::opmode_t::kCool;
    case kMitsubishiHeavyHeat: return stdAc::opmode_t::kHeat;
    case kMitsubishiHeavyDry:  return stdAc::opmode_t::kDry;
    case kMitsubishiHeavyFan:  return stdAc::opmode_t::kFan;
    default:                   return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivilant.
/// @param[in] spd The native setting to be converted.
/// @return The stdAc equivilant of the native setting.
stdAc::fanspeed_t IRMitsubishiHeavy152Ac::toCommonFanSpeed(const uint8_t spd) {
  switch (spd) {
    case kMitsubishiHeavy152FanMax:   return stdAc::fanspeed_t::kMax;
    case kMitsubishiHeavy152FanHigh:  return stdAc::fanspeed_t::kHigh;
    case kMitsubishiHeavy152FanMed:   return stdAc::fanspeed_t::kMedium;
    case kMitsubishiHeavy152FanLow:   return stdAc::fanspeed_t::kLow;
    case kMitsubishiHeavy152FanEcono: return stdAc::fanspeed_t::kMin;
    default:                          return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert a native horizontal swing postion to it's common equivalent.
/// @param[in] pos A native position to convert.
/// @return The common horizontal swing position.
stdAc::swingh_t IRMitsubishiHeavy152Ac::toCommonSwingH(const uint8_t pos) {
  switch (pos) {
    case kMitsubishiHeavy152SwingHLeftMax:  return stdAc::swingh_t::kLeftMax;
    case kMitsubishiHeavy152SwingHLeft:     return stdAc::swingh_t::kLeft;
    case kMitsubishiHeavy152SwingHMiddle:   return stdAc::swingh_t::kMiddle;
    case kMitsubishiHeavy152SwingHRight:    return stdAc::swingh_t::kRight;
    case kMitsubishiHeavy152SwingHRightMax: return stdAc::swingh_t::kRightMax;
    case kMitsubishiHeavy152SwingHOff:      return stdAc::swingh_t::kOff;
    default:                                return stdAc::swingh_t::kAuto;
  }
}

/// Convert a native vertical swing postion to it's common equivalent.
/// @param[in] pos A native position to convert.
/// @return The common vertical swing position.
stdAc::swingv_t IRMitsubishiHeavy152Ac::toCommonSwingV(const uint8_t pos) {
  switch (pos) {
    case kMitsubishiHeavy152SwingVHighest: return stdAc::swingv_t::kHighest;
    case kMitsubishiHeavy152SwingVHigh:    return stdAc::swingv_t::kHigh;
    case kMitsubishiHeavy152SwingVMiddle:  return stdAc::swingv_t::kMiddle;
    case kMitsubishiHeavy152SwingVLow:     return stdAc::swingv_t::kLow;
    case kMitsubishiHeavy152SwingVLowest:  return stdAc::swingv_t::kLowest;
    case kMitsubishiHeavy152SwingVOff:     return stdAc::swingv_t::kOff;
    default:                               return stdAc::swingv_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivilant.
/// @return The stdAc equivilant of the native settings.
stdAc::state_t IRMitsubishiHeavy152Ac::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::MITSUBISHI_HEAVY_152;
  result.model = -1;  // No models used.
  result.power = this->getPower();
  result.mode = this->toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = this->toCommonFanSpeed(this->getFan());
  result.swingv = this->toCommonSwingV(this->getSwingVertical());
  result.swingh = this->toCommonSwingH(this->getSwingHorizontal());
  result.turbo = this->getTurbo();
  result.econo = this->getEcono();
  result.clean = this->getClean();
  result.quiet = this->getSilent();
  result.filter = this->getFilter();
  result.sleep = this->getNight() ? 0 : -1;
  // Not supported.
  result.light = false;
  result.beep = false;
  result.clock = -1;
  return result;
}

/// Convert the internal state into a human readable string.
/// @return A string containing the settings in human-readable form.
String IRMitsubishiHeavy152Ac::toString(void) {
  String result = "";
  result.reserve(180);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(getMode(), kMitsubishiHeavyAuto,
                            kMitsubishiHeavyCool, kMitsubishiHeavyHeat,
                            kMitsubishiHeavyDry, kMitsubishiHeavyFan);
  result += addTempToString(getTemp());
  result += addIntToString(getFan(), kFanStr);
  result += kSpaceLBraceStr;
  switch (this->getFan()) {
    case kMitsubishiHeavy152FanAuto:
      result += kAutoStr;
      break;
    case kMitsubishiHeavy152FanHigh:
      result += kHighStr;
      break;
    case kMitsubishiHeavy152FanLow:
      result += kLowStr;
      break;
    case kMitsubishiHeavy152FanMed:
      result += kMedStr;
      break;
    case kMitsubishiHeavy152FanMax:
      result += kMaxStr;
      break;
    case kMitsubishiHeavy152FanEcono:
      result += kEconoStr;
      break;
    case kMitsubishiHeavy152FanTurbo:
      result += kTurboStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addIntToString(getSwingVertical(), kSwingVStr);
  result += kSpaceLBraceStr;
  switch (this->getSwingVertical()) {
    case kMitsubishiHeavy152SwingVAuto:
      result += kAutoStr;
      break;
    case kMitsubishiHeavy152SwingVHighest:
      result += kHighestStr;
      break;
    case kMitsubishiHeavy152SwingVHigh:
      result += kHighStr;
      break;
    case kMitsubishiHeavy152SwingVMiddle:
      result += kMiddleStr;
      break;
    case kMitsubishiHeavy152SwingVLow:
      result += kLowStr;
      break;
    case kMitsubishiHeavy152SwingVLowest:
      result += kLowestStr;
      break;
    case kMitsubishiHeavy152SwingVOff:
      result += kOffStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addIntToString(getSwingHorizontal(), kSwingHStr);
  result += kSpaceLBraceStr;
  switch (this->getSwingHorizontal()) {
    case kMitsubishiHeavy152SwingHAuto:
      result += kAutoStr;
      break;
    case kMitsubishiHeavy152SwingHLeftMax:
      result += kMaxLeftStr;
      break;
    case kMitsubishiHeavy152SwingHLeft:
      result += kLeftStr;
      break;
    case kMitsubishiHeavy152SwingHMiddle:
      result += kMiddleStr;
      break;
    case kMitsubishiHeavy152SwingHRight:
      result += kRightStr;
      break;
    case kMitsubishiHeavy152SwingHRightMax:
      result += kMaxRightStr;
      break;
    case kMitsubishiHeavy152SwingHLeftRight:
      result += kLeftStr;
      result += ' ';
      result += kRightStr;
      break;
    case kMitsubishiHeavy152SwingHRightLeft:
      result += kRightStr;
      result += ' ';
      result += kLeftStr;
      break;
    case kMitsubishiHeavy152SwingHOff:
      result += kOffStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addBoolToString(getSilent(), kSilentStr);
  result += addBoolToString(getTurbo(), kTurboStr);
  result += addBoolToString(getEcono(), kEconoStr);
  result += addBoolToString(getNight(), kNightStr);
  result += addBoolToString(getFilter(), kFilterStr);
  result += addBoolToString(get3D(), k3DStr);
  result += addBoolToString(getClean(), kCleanStr);
  return result;
}


// Class for decoding and constructing MitsubishiHeavy88 AC messages.

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRMitsubishiHeavy88Ac::IRMitsubishiHeavy88Ac(const uint16_t pin,
                                             const bool inverted,
                                             const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Set up hardware to be able to send a message.
void IRMitsubishiHeavy88Ac::begin(void) { _irsend.begin(); }

#if SEND_MITSUBISHIHEAVY
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRMitsubishiHeavy88Ac::send(const uint16_t repeat) {
  _irsend.sendMitsubishiHeavy88(this->getRaw(), kMitsubishiHeavy88StateLength,
                                repeat);
}
#endif  // SEND_MITSUBISHIHEAVY

/// Reset the state of the remote to a known good state/sequence.
void IRMitsubishiHeavy88Ac::stateReset(void) {
  memcpy(remote_state, kMitsubishiHeavyZjsSig, kMitsubishiHeavySigLength);
  for (uint8_t i = kMitsubishiHeavySigLength; i < kMitsubishiHeavy88StateLength;
       i++) remote_state[i] = 0;
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRMitsubishiHeavy88Ac::getRaw(void) {
  checksum();
  return remote_state;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] data A valid code for this protocol.
void IRMitsubishiHeavy88Ac::setRaw(const uint8_t *data) {
  memcpy(remote_state, data, kMitsubishiHeavy88StateLength);
}

/// Set the requested power state of the A/C to on.
void IRMitsubishiHeavy88Ac::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void IRMitsubishiHeavy88Ac::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiHeavy88Ac::setPower(const bool on) {
  setBit(&remote_state[9], kMitsubishiHeavyPowerOffset, on);
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiHeavy88Ac::getPower(void) {
  return GETBIT8(remote_state[9], kMitsubishiHeavyPowerOffset);
}

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
void IRMitsubishiHeavy88Ac::setTemp(const uint8_t temp) {
  uint8_t newtemp = temp;
  newtemp = std::min(newtemp, kMitsubishiHeavyMaxTemp);
  newtemp = std::max(newtemp, kMitsubishiHeavyMinTemp);
  setBits(&remote_state[9], kHighNibble, kNibbleSize,
          newtemp - kMitsubishiHeavyMinTemp);
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRMitsubishiHeavy88Ac::getTemp(void) {
  return GETBITS8(remote_state[9], kHighNibble, kNibbleSize) +
      kMitsubishiHeavyMinTemp;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRMitsubishiHeavy88Ac::setFan(const uint8_t speed) {
  uint8_t newspeed = speed;
  switch (speed) {
    case kMitsubishiHeavy88FanLow:
    case kMitsubishiHeavy88FanMed:
    case kMitsubishiHeavy88FanHigh:
    case kMitsubishiHeavy88FanTurbo:
    case kMitsubishiHeavy88FanEcono: break;
    default: newspeed = kMitsubishiHeavy88FanAuto;
  }
  setBits(&remote_state[7], kMitsubishiHeavy88FanOffset,
          kMitsubishiHeavy88FanSize, newspeed);
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t IRMitsubishiHeavy88Ac::getFan(void) {
  return GETBITS8(remote_state[7], kMitsubishiHeavy88FanOffset,
                  kMitsubishiHeavy88FanSize);
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRMitsubishiHeavy88Ac::setMode(const uint8_t mode) {
  uint8_t newmode = mode;
  switch (mode) {
    case kMitsubishiHeavyCool:
    case kMitsubishiHeavyDry:
    case kMitsubishiHeavyFan:
    case kMitsubishiHeavyHeat:
      break;
    default:
      newmode = kMitsubishiHeavyAuto;
  }
  setBits(&remote_state[9], kMitsubishiHeavyModeOffset, kModeBitsSize, newmode);
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRMitsubishiHeavy88Ac::getMode(void) {
  return GETBITS8(remote_state[9], kMitsubishiHeavyModeOffset, kModeBitsSize);
}

/// Set the Vertical Swing mode of the A/C.
/// @param[in] pos The position/mode to set the swing to.
void IRMitsubishiHeavy88Ac::setSwingVertical(const uint8_t pos) {
  uint8_t newpos;
  switch (pos) {
    case kMitsubishiHeavy88SwingVAuto:
    case kMitsubishiHeavy88SwingVHighest:
    case kMitsubishiHeavy88SwingVHigh:
    case kMitsubishiHeavy88SwingVMiddle:
    case kMitsubishiHeavy88SwingVLow:
    case kMitsubishiHeavy88SwingVLowest: newpos = pos; break;
    default: newpos = kMitsubishiHeavy88SwingVOff;
  }
  setBit(&remote_state[5], kMitsubishiHeavy88SwingVByte5Offset,
         newpos & 1);
  setBits(&remote_state[7], kMitsubishiHeavy88SwingVByte7Offset,
          kMitsubishiHeavy88SwingVByte7Size,
          newpos >> kMitsubishiHeavy88SwingVByte5Size);
}

/// Get the Vertical Swing mode of the A/C.
/// @return The native position/mode setting.
uint8_t IRMitsubishiHeavy88Ac::getSwingVertical(void) {
  return GETBITS8(remote_state[5], kMitsubishiHeavy88SwingVByte5Offset,
                  kMitsubishiHeavy88SwingVByte5Size) |
         (GETBITS8(remote_state[7], kMitsubishiHeavy88SwingVByte7Offset,
                   kMitsubishiHeavy88SwingVByte7Size) <<
          kMitsubishiHeavy88SwingVByte5Size);
}

/// Set the Horizontal Swing mode of the A/C.
/// @param[in] pos The position/mode to set the swing to.
void IRMitsubishiHeavy88Ac::setSwingHorizontal(const uint8_t pos) {
  uint8_t newpos;
  switch (pos) {
    case kMitsubishiHeavy88SwingHAuto:
    case kMitsubishiHeavy88SwingHLeftMax:
    case kMitsubishiHeavy88SwingHLeft:
    case kMitsubishiHeavy88SwingHMiddle:
    case kMitsubishiHeavy88SwingHRight:
    case kMitsubishiHeavy88SwingHRightMax:
    case kMitsubishiHeavy88SwingHLeftRight:
    case kMitsubishiHeavy88SwingHRightLeft:
    case kMitsubishiHeavy88SwingH3D: newpos = pos; break;
    default:                         newpos = kMitsubishiHeavy88SwingHOff;
  }
  setBits(&remote_state[5], kMitsubishiHeavy88SwingHOffset1,
                  kMitsubishiHeavy88SwingHSize, newpos);
  setBits(&remote_state[5], kMitsubishiHeavy88SwingHOffset2,
                  kMitsubishiHeavy88SwingHSize,
                  newpos >> kMitsubishiHeavy88SwingHSize);
}

/// Get the Horizontal Swing mode of the A/C.
/// @return The native position/mode setting.
uint8_t IRMitsubishiHeavy88Ac::getSwingHorizontal(void) {
  return GETBITS8(remote_state[5], kMitsubishiHeavy88SwingHOffset1,
                  kMitsubishiHeavy88SwingHSize) |
         (GETBITS8(remote_state[5], kMitsubishiHeavy88SwingHOffset2,
                   kMitsubishiHeavy88SwingHSize) <<
          kMitsubishiHeavy88SwingHSize);
}

/// Set the Turbo mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiHeavy88Ac::setTurbo(const bool on) {
  if (on)
    this->setFan(kMitsubishiHeavy88FanTurbo);
  else if (this->getTurbo()) this->setFan(kMitsubishiHeavy88FanAuto);
}

/// Get the Turbo mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiHeavy88Ac::getTurbo(void) {
  return this->getFan() == kMitsubishiHeavy88FanTurbo;
}

/// Set the Economical mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiHeavy88Ac::setEcono(const bool on) {
  if (on)
    this->setFan(kMitsubishiHeavy88FanEcono);
  else if (this->getEcono()) this->setFan(kMitsubishiHeavy88FanAuto);
}

/// Get the Economical mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiHeavy88Ac::getEcono(void) {
  return this->getFan() == kMitsubishiHeavy88FanEcono;
}

/// Set the 3D mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiHeavy88Ac::set3D(const bool on) {
  if (on)
    this->setSwingHorizontal(kMitsubishiHeavy88SwingH3D);
  else if (this->get3D())
    this->setSwingHorizontal(kMitsubishiHeavy88SwingHOff);
}

/// Get the 3D mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiHeavy88Ac::get3D(void) {
  return this->getSwingHorizontal() == kMitsubishiHeavy88SwingH3D;
}

/// Set the Clean mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiHeavy88Ac::setClean(const bool on) {
  setBit(&remote_state[5], kMitsubishiHeavy88CleanOffset, on);
}

/// Get the Clean mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiHeavy88Ac::getClean(void) {
  return GETBIT8(remote_state[5], kMitsubishiHeavy88CleanOffset);
}

/// Verify the given state has a ZJ-S signature.
/// @param[in] state A ptr to a state to be checked.
/// @return true, the check passed. Otherwise, false.
bool IRMitsubishiHeavy88Ac::checkZjsSig(const uint8_t *state) {
  for (uint8_t i = 0; i < kMitsubishiHeavySigLength; i++)
    if (state[i] != kMitsubishiHeavyZjsSig[i]) return false;
  return true;
}

/// Calculate the checksum for the current internal state of the remote.
/// Note: Technically it has no checksum, but does has inverted byte pairs.
void IRMitsubishiHeavy88Ac::checksum(void) {
  for (uint8_t i = kMitsubishiHeavySigLength - 2;
       i < kMitsubishiHeavy88StateLength;
       i += 2) {
    remote_state[i + 1] = ~remote_state[i];
  }
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length/size of the state array.
/// @return true, if the state has a valid checksum. Otherwise, false.
/// Note: Technically it has no checksum, but does has inverted byte pairs.
bool IRMitsubishiHeavy88Ac::validChecksum(const uint8_t *state,
                                           const uint16_t length) {
  return IRMitsubishiHeavy152Ac::validChecksum(state, length);
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivilant of the enum.
uint8_t IRMitsubishiHeavy88Ac::convertMode(const stdAc::opmode_t mode) {
  return IRMitsubishiHeavy152Ac::convertMode(mode);
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivilant of the enum.
uint8_t IRMitsubishiHeavy88Ac::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    // Assumes Econo is slower than Low.
    case stdAc::fanspeed_t::kMin:    return kMitsubishiHeavy88FanEcono;
    case stdAc::fanspeed_t::kLow:    return kMitsubishiHeavy88FanLow;
    case stdAc::fanspeed_t::kMedium: return kMitsubishiHeavy88FanMed;
    case stdAc::fanspeed_t::kHigh:   return kMitsubishiHeavy88FanHigh;
    case stdAc::fanspeed_t::kMax:    return kMitsubishiHeavy88FanTurbo;
    default:                         return kMitsubishiHeavy88FanAuto;
  }
}

/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivilant of the enum.
uint8_t IRMitsubishiHeavy88Ac::convertSwingV(const stdAc::swingv_t position) {
  switch (position) {
    case stdAc::swingv_t::kAuto:    return kMitsubishiHeavy88SwingVAuto;
    case stdAc::swingv_t::kHighest: return kMitsubishiHeavy88SwingVHighest;
    case stdAc::swingv_t::kHigh:    return kMitsubishiHeavy88SwingVHigh;
    case stdAc::swingv_t::kMiddle:  return kMitsubishiHeavy88SwingVMiddle;
    case stdAc::swingv_t::kLow:     return kMitsubishiHeavy88SwingVLow;
    case stdAc::swingv_t::kLowest:  return kMitsubishiHeavy88SwingVLowest;
    default:                        return kMitsubishiHeavy88SwingVOff;
  }
}

/// Convert a stdAc::swingh_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivilant of the enum.
uint8_t IRMitsubishiHeavy88Ac::convertSwingH(const stdAc::swingh_t position) {
  switch (position) {
    case stdAc::swingh_t::kAuto:     return kMitsubishiHeavy88SwingHAuto;
    case stdAc::swingh_t::kLeftMax:  return kMitsubishiHeavy88SwingHLeftMax;
    case stdAc::swingh_t::kLeft:     return kMitsubishiHeavy88SwingHLeft;
    case stdAc::swingh_t::kMiddle:   return kMitsubishiHeavy88SwingHMiddle;
    case stdAc::swingh_t::kRight:    return kMitsubishiHeavy88SwingHRight;
    case stdAc::swingh_t::kRightMax: return kMitsubishiHeavy88SwingHRightMax;
    default:                         return kMitsubishiHeavy88SwingHOff;
  }
}

/// Convert a native fan speed into its stdAc equivilant.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivilant of the native setting.
stdAc::fanspeed_t IRMitsubishiHeavy88Ac::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kMitsubishiHeavy88FanTurbo: return stdAc::fanspeed_t::kMax;
    case kMitsubishiHeavy88FanHigh:  return stdAc::fanspeed_t::kHigh;
    case kMitsubishiHeavy88FanMed:   return stdAc::fanspeed_t::kMedium;
    case kMitsubishiHeavy88FanLow:   return stdAc::fanspeed_t::kLow;
    case kMitsubishiHeavy88FanEcono: return stdAc::fanspeed_t::kMin;
    default:                         return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert a native horizontal swing postion to it's common equivalent.
/// @param[in] pos A native position to convert.
/// @return The common horizontal swing position.
stdAc::swingh_t IRMitsubishiHeavy88Ac::toCommonSwingH(const uint8_t pos) {
  switch (pos) {
    case kMitsubishiHeavy88SwingHLeftMax:  return stdAc::swingh_t::kLeftMax;
    case kMitsubishiHeavy88SwingHLeft:     return stdAc::swingh_t::kLeft;
    case kMitsubishiHeavy88SwingHMiddle:   return stdAc::swingh_t::kMiddle;
    case kMitsubishiHeavy88SwingHRight:    return stdAc::swingh_t::kRight;
    case kMitsubishiHeavy88SwingHRightMax: return stdAc::swingh_t::kRightMax;
    case kMitsubishiHeavy88SwingHOff:      return stdAc::swingh_t::kOff;
    default:                               return stdAc::swingh_t::kAuto;
  }
}

/// Convert a native vertical swing postion to it's common equivalent.
/// @param[in] pos A native position to convert.
/// @return The common vertical swing position.
stdAc::swingv_t IRMitsubishiHeavy88Ac::toCommonSwingV(const uint8_t pos) {
  switch (pos) {
    case kMitsubishiHeavy88SwingVHighest: return stdAc::swingv_t::kHighest;
    case kMitsubishiHeavy88SwingVHigh:    return stdAc::swingv_t::kHigh;
    case kMitsubishiHeavy88SwingVMiddle:  return stdAc::swingv_t::kMiddle;
    case kMitsubishiHeavy88SwingVLow:     return stdAc::swingv_t::kLow;
    case kMitsubishiHeavy88SwingVLowest:  return stdAc::swingv_t::kLowest;
    case kMitsubishiHeavy88SwingVOff:     return stdAc::swingv_t::kOff;
    default:                              return stdAc::swingv_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivilant.
/// @return The stdAc equivilant of the native settings.
stdAc::state_t IRMitsubishiHeavy88Ac::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::MITSUBISHI_HEAVY_88;
  result.model = -1;  // No models used.
  result.power = this->getPower();
  result.mode = IRMitsubishiHeavy152Ac::toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = this->toCommonFanSpeed(this->getFan());
  result.swingv = this->toCommonSwingV(this->getSwingVertical());
  result.swingh = this->toCommonSwingH(this->getSwingHorizontal());
  result.turbo = this->getTurbo();
  result.econo = this->getEcono();
  result.clean = this->getClean();
  // Not supported.
  result.quiet = false;
  result.filter = false;
  result.light = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

/// Convert the internal state into a human readable string.
/// @return A string containing the settings in human-readable form.
String IRMitsubishiHeavy88Ac::toString(void) {
  String result = "";
  result.reserve(140);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(getMode(), kMitsubishiHeavyAuto,
                            kMitsubishiHeavyCool, kMitsubishiHeavyHeat,
                            kMitsubishiHeavyDry, kMitsubishiHeavyFan);
  result += addTempToString(getTemp());
  result += addIntToString(getFan(), kFanStr);
  result += kSpaceLBraceStr;
  switch (this->getFan()) {
    case kMitsubishiHeavy88FanAuto:
      result += kAutoStr;
      break;
    case kMitsubishiHeavy88FanHigh:
      result += kHighStr;
      break;
    case kMitsubishiHeavy88FanLow:
      result += kLowStr;
      break;
    case kMitsubishiHeavy88FanMed:
      result += kMedStr;
      break;
    case kMitsubishiHeavy88FanEcono:
      result += kEconoStr;
      break;
    case kMitsubishiHeavy88FanTurbo:
      result += kTurboStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addIntToString(getSwingVertical(), kSwingVStr);
  result += kSpaceLBraceStr;
  switch (this->getSwingVertical()) {
    case kMitsubishiHeavy88SwingVAuto:
      result += kAutoStr;
      break;
    case kMitsubishiHeavy88SwingVHighest:
      result += kHighestStr;
      break;
    case kMitsubishiHeavy88SwingVHigh:
      result += kHighStr;
      break;
    case kMitsubishiHeavy88SwingVMiddle:
      result += kMiddleStr;
      break;
    case kMitsubishiHeavy88SwingVLow:
      result += kLowStr;
      break;
    case kMitsubishiHeavy88SwingVLowest:
      result += kLowestStr;
      break;
    case kMitsubishiHeavy88SwingVOff:
      result += kOffStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addIntToString(getSwingHorizontal(), kSwingHStr);
  result += kSpaceLBraceStr;
  switch (this->getSwingHorizontal()) {
    case kMitsubishiHeavy88SwingHAuto:
      result += kAutoStr;
      break;
    case kMitsubishiHeavy88SwingHLeftMax:
      result += kMaxLeftStr;
      break;
    case kMitsubishiHeavy88SwingHLeft:
      result += kLeftStr;
      break;
    case kMitsubishiHeavy88SwingHMiddle:
      result += kMiddleStr;
      break;
    case kMitsubishiHeavy88SwingHRight:
      result += kRightStr;
      break;
    case kMitsubishiHeavy88SwingHRightMax:
      result += kMaxRightStr;
      break;
    case kMitsubishiHeavy88SwingHLeftRight:
      result += kLeftStr;
      result += ' ';
      result += kRightStr;
      break;
    case kMitsubishiHeavy88SwingHRightLeft:
      result += kRightStr;
      result += ' ';
      result += kLeftStr;
      break;
    case kMitsubishiHeavy88SwingH3D:
      result += k3DStr;
      break;
    case kMitsubishiHeavy88SwingHOff:
      result += kOffStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addBoolToString(getTurbo(), kTurboStr);
  result += addBoolToString(getEcono(), kEconoStr);
  result += addBoolToString(get3D(), k3DStr);
  result += addBoolToString(getClean(), kCleanStr);
  return result;
}

#if DECODE_MITSUBISHIHEAVY
/// Decode the supplied Mitsubishi Heavy Industries A/C message.
/// Status: BETA / Appears to be working. Needs testing against a real device.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
///   Typically kMitsubishiHeavy88Bits or kMitsubishiHeavy152Bits (def).
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
bool IRrecv::decodeMitsubishiHeavy(decode_results* results, uint16_t offset,
                                   const uint16_t nbits, const bool strict) {
  if (strict) {
    switch (nbits) {
      case kMitsubishiHeavy88Bits:
      case kMitsubishiHeavy152Bits:
        break;
      default:
        return false;  // Not what is expected
    }
  }

  uint16_t used;
  used = matchGeneric(results->rawbuf + offset, results->state,
                      results->rawlen - offset, nbits,
                      kMitsubishiHeavyHdrMark, kMitsubishiHeavyHdrSpace,
                      kMitsubishiHeavyBitMark, kMitsubishiHeavyOneSpace,
                      kMitsubishiHeavyBitMark, kMitsubishiHeavyZeroSpace,
                      kMitsubishiHeavyBitMark, kMitsubishiHeavyGap, true,
                      _tolerance, 0, false);
  if (used == 0) return false;
  offset += used;
  // Compliance
  switch (nbits) {
    case kMitsubishiHeavy88Bits:
      if (strict && !(IRMitsubishiHeavy88Ac::checkZjsSig(results->state) &&
                      IRMitsubishiHeavy88Ac::validChecksum(results->state)))
        return false;
      results->decode_type = MITSUBISHI_HEAVY_88;
      break;
    case kMitsubishiHeavy152Bits:
      if (strict && !(IRMitsubishiHeavy152Ac::checkZmsSig(results->state) &&
                      IRMitsubishiHeavy152Ac::validChecksum(results->state)))
        return false;
      results->decode_type = MITSUBISHI_HEAVY_152;
      break;
    default:
      return false;
  }

  // Success
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_MITSUBISHIHEAVY
