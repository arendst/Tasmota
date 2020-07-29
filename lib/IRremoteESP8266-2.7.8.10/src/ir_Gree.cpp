// Copyright 2017 Ville Skyttä (scop)
// Copyright 2017, 2018 David Conran

/// @file
/// @brief Support for Gree A/C protocols.
/// @see https://github.com/ToniA/arduino-heatpumpir/blob/master/GreeHeatpumpIR.h

#include "ir_Gree.h"
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
#include "ir_Kelvinator.h"

// Constants
const uint16_t kGreeHdrMark = 9000;
const uint16_t kGreeHdrSpace = 4500;  ///< See #684 & real example in unit tests
const uint16_t kGreeBitMark = 620;
const uint16_t kGreeOneSpace = 1600;
const uint16_t kGreeZeroSpace = 540;
const uint16_t kGreeMsgSpace = 19000;
const uint8_t kGreeBlockFooter = 0b010;
const uint8_t kGreeBlockFooterBits = 3;

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

#if SEND_GREE
/// Send a Gree Heat Pump formatted message.
/// Status: STABLE / Working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendGree(const uint8_t data[], const uint16_t nbytes,
                      const uint16_t repeat) {
  if (nbytes < kGreeStateLength)
    return;  // Not enough bytes to send a proper message.

  for (uint16_t r = 0; r <= repeat; r++) {
    // Block #1
    sendGeneric(kGreeHdrMark, kGreeHdrSpace, kGreeBitMark, kGreeOneSpace,
                kGreeBitMark, kGreeZeroSpace, 0, 0,  // No Footer.
                data, 4, 38, false, 0, 50);
    // Footer #1
    sendGeneric(0, 0,  // No Header
                kGreeBitMark, kGreeOneSpace, kGreeBitMark, kGreeZeroSpace,
                kGreeBitMark, kGreeMsgSpace, 0b010, 3, 38, false, 0, 50);

    // Block #2
    sendGeneric(0, 0,  // No Header for Block #2
                kGreeBitMark, kGreeOneSpace, kGreeBitMark, kGreeZeroSpace,
                kGreeBitMark, kGreeMsgSpace, data + 4, nbytes - 4, 38, false, 0,
                50);
  }
}

/// Send a Gree Heat Pump formatted message.
/// Status: STABLE / Working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendGree(const uint64_t data, const uint16_t nbits,
                      const uint16_t repeat) {
  if (nbits != kGreeBits)
    return;  // Wrong nr. of bits to send a proper message.
  // Set IR carrier frequency
  enableIROut(38);

  for (uint16_t r = 0; r <= repeat; r++) {
    // Header
    mark(kGreeHdrMark);
    space(kGreeHdrSpace);

    // Data
    for (int16_t i = 8; i <= nbits; i += 8) {
      sendData(kGreeBitMark, kGreeOneSpace, kGreeBitMark, kGreeZeroSpace,
               (data >> (nbits - i)) & 0xFF, 8, false);
      if (i == nbits / 2) {
        // Send the mid-message Footer.
        sendData(kGreeBitMark, kGreeOneSpace, kGreeBitMark, kGreeZeroSpace,
                 0b010, 3);
        mark(kGreeBitMark);
        space(kGreeMsgSpace);
      }
    }
    // Footer
    mark(kGreeBitMark);
    space(kGreeMsgSpace);
  }
}
#endif  // SEND_GREE

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] model The enum of the model to be emulated.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRGreeAC::IRGreeAC(const uint16_t pin, const gree_ac_remote_model_t model,
                   const bool inverted, const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) {
  stateReset();
  setModel(model);
}

/// Reset the internal state to a fixed known good state.
void IRGreeAC::stateReset(void) {
  // This resets to a known-good state to Power Off, Fan Auto, Mode Auto, 25C.
  for (uint8_t i = 0; i < kGreeStateLength; i++) remote_state[i] = 0x0;
  remote_state[1] = 0x09;
  remote_state[2] = 0x20;
  remote_state[3] = 0x50;
  remote_state[5] = 0x20;
  remote_state[7] = 0x50;
}

/// Fix up the internal state so it is correct.
/// @note Internal use only.
void IRGreeAC::fixup(void) {
  setPower(getPower());  // Redo the power bits as they differ between models.
  checksum();  // Calculate the checksums
}

/// Set up hardware to be able to send a message.
void IRGreeAC::begin(void) { _irsend.begin(); }

#if SEND_GREE
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRGreeAC::send(const uint16_t repeat) {
  fixup();  // Ensure correct settings before sending.
  _irsend.sendGree(remote_state, kGreeStateLength, repeat);
}
#endif  // SEND_GREE

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t* IRGreeAC::getRaw(void) {
  fixup();  // Ensure correct settings before sending.
  return remote_state;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
void IRGreeAC::setRaw(const uint8_t new_code[]) {
  memcpy(remote_state, new_code, kGreeStateLength);
  // We can only detect the difference between models when the power is on.
  if (getPower()) {
    if (GETBIT8(remote_state[2], kGreePower2Offset))
      _model = gree_ac_remote_model_t::YAW1F;
    else
      _model = gree_ac_remote_model_t::YBOFB;
  }
}

/// Calculate and set the checksum values for the internal state.
/// @param[in] length The size/length of the state array to fix the checksum of.
void IRGreeAC::checksum(const uint16_t length) {
  // Gree uses the same checksum alg. as Kelvinator's block checksum.
  setBits(&remote_state[length - 1], kHighNibble, kNibbleSize,
          IRKelvinatorAC::calcBlockChecksum(remote_state, length));
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length of the state array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRGreeAC::validChecksum(const uint8_t state[], const uint16_t length) {
  // Top 4 bits of the last byte in the state is the state's checksum.
  return GETBITS8(state[length - 1], kHighNibble, kNibbleSize) ==
      IRKelvinatorAC::calcBlockChecksum(state, length);
}

/// Set the model of the A/C to emulate.
/// @param[in] model The enum of the appropriate model.
void IRGreeAC::setModel(const gree_ac_remote_model_t model) {
  switch (model) {
    case gree_ac_remote_model_t::YAW1F:
    case gree_ac_remote_model_t::YBOFB: _model = model; break;
    default: setModel(gree_ac_remote_model_t::YAW1F);
  }
}

/// Get/Detect the model of the A/C.
/// @return The enum of the compatible model.
gree_ac_remote_model_t IRGreeAC::getModel(void) { return _model; }

/// Change the power setting to On.
void IRGreeAC::on(void) { setPower(true); }

/// Change the power setting to Off.
void IRGreeAC::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/814
void IRGreeAC::setPower(const bool on) {
  setBit(&remote_state[0], kGreePower1Offset, on);
  // May not be needed. See #814
  setBit(&remote_state[2], kGreePower2Offset,
         on && _model != gree_ac_remote_model_t::YBOFB);
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/814
bool IRGreeAC::getPower(void) {
  //  See #814. Not checking/requiring: (remote_state[2] & kGreePower2Mask)
  return GETBIT8(remote_state[0], kGreePower1Offset);
}

/// Set the default temperature units to use.
/// @param[in] on Use Fahrenheit as the units.
///   true is Fahrenheit, false is Celsius.
void IRGreeAC::setUseFahrenheit(const bool on) {
  setBit(&remote_state[3], kGreeUseFahrenheitOffset, on);
}

/// Get the default temperature units in use.
/// @return true is Fahrenheit, false is Celsius.
bool IRGreeAC::getUseFahrenheit(void) {
  return GETBIT8(remote_state[3], kGreeUseFahrenheitOffset);
}

/// Set the temp. in degrees
/// @param[in] temp Desired temperature in Degrees.
/// @param[in] fahrenheit Use units of Fahrenheit and set that as units used.
///   false is Celsius (Default), true is Fahrenheit.
/// @note The unit actually works in Celsius with a special optional
///   "extra degree" when sending Fahrenheit.
void IRGreeAC::setTemp(const uint8_t temp, const bool fahrenheit) {
  float safecelsius = temp;
  if (fahrenheit)
    // Covert to F, and add a fudge factor to round to the expected degree.
    // Why 0.6 you ask?! Because it works. Ya'd thing 0.5 would be good for
    // rounding, but Noooooo!
    safecelsius = fahrenheitToCelsius(temp + 0.6);
  setUseFahrenheit(fahrenheit);  // Set the correct Temp units.

  // Make sure we have desired temp in the correct range.
  safecelsius = std::max(static_cast<float>(kGreeMinTempC), safecelsius);
  safecelsius = std::min(static_cast<float>(kGreeMaxTempC), safecelsius);
  // An operating mode of Auto locks the temp to a specific value. Do so.
  if (getMode() == kGreeAuto) safecelsius = 25;

  // Set the "main" Celsius degrees.
  setBits(&remote_state[1], kGreeTempOffset, kGreeTempSize,
          safecelsius - kGreeMinTempC);
  // Deal with the extra degree fahrenheit difference.
  setBit(&remote_state[3], kGreeTempExtraDegreeFOffset,
         (uint8_t)(safecelsius * 2) & 1);
}

/// Get the set temperature
/// @return The temperature in degrees in the current units (C/F) set.
uint8_t IRGreeAC::getTemp(void) {
  uint8_t deg = kGreeMinTempC + GETBITS8(remote_state[1], kGreeTempOffset,
                                         kGreeTempSize);
  if (getUseFahrenheit()) {
    deg = celsiusToFahrenheit(deg);
    // Retrieve the "extra" fahrenheit from elsewhere in the code.
    if (GETBIT8(remote_state[3], kGreeTempExtraDegreeFOffset)) deg++;
    deg = std::max(deg, kGreeMinTempF);  // Cover the fact that 61F is < 16C
  }
  return deg;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting. 0 is auto, 1-3 is the speed.
void IRGreeAC::setFan(const uint8_t speed) {
  uint8_t fan = std::min((uint8_t)kGreeFanMax, speed);  // Bounds check
  if (getMode() == kGreeDry) fan = 1;  // DRY mode is always locked to fan 1.
  // Set the basic fan values.
  setBits(&remote_state[0], kGreeFanOffset, kGreeFanSize, fan);
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRGreeAC::getFan(void) {
  return GETBITS8(remote_state[0], kGreeFanOffset, kGreeFanSize);
}

/// Set the operating mode of the A/C.
/// @param[in] new_mode The desired operating mode.
void IRGreeAC::setMode(const uint8_t new_mode) {
  uint8_t mode = new_mode;
  switch (mode) {
    // AUTO is locked to 25C
    case kGreeAuto: setTemp(25); break;
    // DRY always sets the fan to 1.
    case kGreeDry: setFan(1); break;
    case kGreeCool:
    case kGreeFan:
    case kGreeHeat: break;
    // If we get an unexpected mode, default to AUTO.
    default: mode = kGreeAuto;
  }
  setBits(&remote_state[0], kLowNibble, kModeBitsSize, mode);
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRGreeAC::getMode(void) {
  return GETBITS8(remote_state[0], kLowNibble, kModeBitsSize);
}

/// Set the Light (LED) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRGreeAC::setLight(const bool on) {
  setBit(&remote_state[2], kGreeLightOffset, on);
}

/// Get the Light (LED) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRGreeAC::getLight(void) {
  return GETBIT8(remote_state[2], kGreeLightOffset);
}

/// Set the IFeel setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRGreeAC::setIFeel(const bool on) {
  setBit(&remote_state[5], kGreeIFeelOffset, on);
}

/// Get the IFeel setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRGreeAC::getIFeel(void) {
  return GETBIT8(remote_state[5], kGreeIFeelOffset);
}

/// Set the Wifi (enabled) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRGreeAC::setWiFi(const bool on) {
  setBit(&remote_state[5], kGreeWiFiOffset, on);
}

/// Get the Wifi (enabled) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRGreeAC::getWiFi(void) {
  return GETBIT8(remote_state[5], kGreeWiFiOffset);
}

/// Set the XFan (Mould) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRGreeAC::setXFan(const bool on) {
  setBit(&remote_state[2], kGreeXfanOffset, on);
}

/// Get the XFan (Mould) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRGreeAC::getXFan(void) {
  return GETBIT8(remote_state[2], kGreeXfanOffset);
}

/// Set the Sleep setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRGreeAC::setSleep(const bool on) {
  setBit(&remote_state[0], kGreeSleepOffset, on);
}

/// Get the Sleep setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRGreeAC::getSleep(void) {
  return GETBIT8(remote_state[0], kGreeSleepOffset);
}

/// Set the Turbo setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRGreeAC::setTurbo(const bool on) {
  setBit(&remote_state[2], kGreeTurboOffset, on);
}

/// Get the Turbo setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRGreeAC::getTurbo(void) {
  return GETBIT8(remote_state[2], kGreeTurboOffset);
}

/// Set the Vertical Swing mode of the A/C.
/// @param[in] automatic Do we use the automatic setting?
/// @param[in] position The position/mode to set the vanes to.
void IRGreeAC::setSwingVertical(const bool automatic, const uint8_t position) {
  setBit(&remote_state[0], kGreeSwingAutoOffset, automatic);
  uint8_t new_position = position;
  if (!automatic) {
    switch (position) {
      case kGreeSwingUp:
      case kGreeSwingMiddleUp:
      case kGreeSwingMiddle:
      case kGreeSwingMiddleDown:
      case kGreeSwingDown:
        break;
      default:
        new_position = kGreeSwingLastPos;
    }
  } else {
    switch (position) {
      case kGreeSwingAuto:
      case kGreeSwingDownAuto:
      case kGreeSwingMiddleAuto:
      case kGreeSwingUpAuto:
        break;
      default:
        new_position = kGreeSwingAuto;
    }
  }
  setBits(&remote_state[4], kLowNibble, kGreeSwingSize, new_position);
}

/// Get the Vertical Swing Automatic mode setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRGreeAC::getSwingVerticalAuto(void) {
  return GETBIT8(remote_state[0], kGreeSwingAutoOffset);
}

/// Get the Vertical Swing position setting of the A/C.
/// @return The native position/mode.
uint8_t IRGreeAC::getSwingVerticalPosition(void) {
  return GETBITS8(remote_state[4], kLowNibble, kGreeSwingSize);
}

/// Set the timer enable setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRGreeAC::setTimerEnabled(const bool on) {
  setBit(&remote_state[1], kGreeTimerEnabledOffset, on);
}

/// Get the timer enabled setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRGreeAC::getTimerEnabled(void) {
  return GETBIT8(remote_state[1], kGreeTimerEnabledOffset);
}

/// Get the timer time value from the A/C.
/// @return The number of minutes the timer is set for.
uint16_t IRGreeAC::getTimer(void) {
  uint16_t hrs = irutils::bcdToUint8(
      (GETBITS8(remote_state[1], kGreeTimerTensHrOffset,
                kGreeTimerTensHrSize) << kNibbleSize) |
      GETBITS8(remote_state[2], kGreeTimerHoursOffset, kGreeTimerHoursSize));
  return hrs * 60 + (GETBIT8(remote_state[1], kGreeTimerHalfHrOffset) ? 30 : 0);
}

/// Set the A/C's timer to turn off in X many minutes.
/// @param[in] minutes The number of minutes the timer should be set for.
/// @note Stores time internally in 30 min units.
///  e.g. 5 mins means 0 (& Off), 95 mins is  90 mins (& On). Max is 24 hours.
void IRGreeAC::setTimer(const uint16_t minutes) {
  uint16_t mins = std::min(kGreeTimerMax, minutes);  // Bounds check.
  setTimerEnabled(mins >= 30);  // Timer is enabled when >= 30 mins.
  uint8_t hours = mins / 60;
  // Set the half hour bit.
  setBit(&remote_state[1], kGreeTimerHalfHrOffset, !((mins % 60) < 30));
  // Set the "tens" digit of hours.
  setBits(&remote_state[1], kGreeTimerTensHrOffset, kGreeTimerTensHrSize,
          hours / 10);
  // Set the "units" digit of hours.
  setBits(&remote_state[2], kGreeTimerHoursOffset, kGreeTimerHoursSize,
          hours % 10);
}

/// Set temperature display mode.
/// i.e. Internal, External temperature sensing.
/// @param[in] mode The desired temp source to display.
/// @note In order for the A/C unit properly accept these settings. You must
///   cycle (send) in the following order:
///   kGreeDisplayTempOff(0) -> kGreeDisplayTempSet(1) ->
///   kGreeDisplayTempInside(2) ->kGreeDisplayTempOutside(3) ->
///   kGreeDisplayTempOff(0).
///   The unit will no behave correctly if the changes of this setting are sent
///   out of order.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1118#issuecomment-628242152
void IRGreeAC::setDisplayTempSource(const uint8_t mode) {
  setBits(&remote_state[5], kGreeDisplayTempOffset, kGreeDisplayTempSize, mode);
}

/// Get the temperature display mode.
/// i.e. Internal, External temperature sensing.
/// @return The current temp source being displayed.
uint8_t IRGreeAC::getDisplayTempSource(void) {
  return GETBITS8(remote_state[5], kGreeDisplayTempOffset,
                  kGreeDisplayTempSize);
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivilant of the enum.
uint8_t IRGreeAC::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kGreeCool;
    case stdAc::opmode_t::kHeat: return kGreeHeat;
    case stdAc::opmode_t::kDry:  return kGreeDry;
    case stdAc::opmode_t::kFan:  return kGreeFan;
    default:                     return kGreeAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivilant of the enum.
uint8_t IRGreeAC::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:    return kGreeFanMin;
    case stdAc::fanspeed_t::kLow:
    case stdAc::fanspeed_t::kMedium: return kGreeFanMax - 1;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kGreeFanMax;
    default:                         return kGreeFanAuto;
  }
}

/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] swingv The enum to be converted.
/// @return The native equivilant of the enum.
uint8_t IRGreeAC::convertSwingV(const stdAc::swingv_t swingv) {
  switch (swingv) {
    case stdAc::swingv_t::kHighest: return kGreeSwingUp;
    case stdAc::swingv_t::kHigh:    return kGreeSwingMiddleUp;
    case stdAc::swingv_t::kMiddle:  return kGreeSwingMiddle;
    case stdAc::swingv_t::kLow:     return kGreeSwingMiddleDown;
    case stdAc::swingv_t::kLowest:  return kGreeSwingDown;
    default:                        return kGreeSwingAuto;
  }
}

/// Convert a native mode into its stdAc equivilant.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivilant of the native setting.
stdAc::opmode_t IRGreeAC::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kGreeCool: return stdAc::opmode_t::kCool;
    case kGreeHeat: return stdAc::opmode_t::kHeat;
    case kGreeDry: return stdAc::opmode_t::kDry;
    case kGreeFan: return stdAc::opmode_t::kFan;
    default: return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivilant.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivilant of the native setting.
stdAc::fanspeed_t IRGreeAC::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kGreeFanMax: return stdAc::fanspeed_t::kMax;
    case kGreeFanMax - 1: return stdAc::fanspeed_t::kMedium;
    case kGreeFanMin: return stdAc::fanspeed_t::kMin;
    default: return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] pos The enum to be converted.
/// @return The native equivilant of the enum.
stdAc::swingv_t IRGreeAC::toCommonSwingV(const uint8_t pos) {
  switch (pos) {
    case kGreeSwingUp: return stdAc::swingv_t::kHighest;
    case kGreeSwingMiddleUp: return stdAc::swingv_t::kHigh;
    case kGreeSwingMiddle: return stdAc::swingv_t::kMiddle;
    case kGreeSwingMiddleDown: return stdAc::swingv_t::kLow;
    case kGreeSwingDown: return stdAc::swingv_t::kLowest;
    default: return stdAc::swingv_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivilant.
/// @return The stdAc equivilant of the native settings.
stdAc::state_t IRGreeAC::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::GREE;
  result.model = this->getModel();
  result.power = this->getPower();
  result.mode = this->toCommonMode(this->getMode());
  result.celsius = !this->getUseFahrenheit();
  result.degrees = this->getTemp();
  result.fanspeed = this->toCommonFanSpeed(this->getFan());
  if (this->getSwingVerticalAuto())
    result.swingv = stdAc::swingv_t::kAuto;
  else
    result.swingv = this->toCommonSwingV(this->getSwingVerticalPosition());
  result.turbo = this->getTurbo();
  result.light = this->getLight();
  result.clean = this->getXFan();
  result.sleep = this->getSleep() ? 0 : -1;
  // Not supported.
  result.swingh = stdAc::swingh_t::kOff;
  result.quiet = false;
  result.econo = false;
  result.filter = false;
  result.beep = false;
  result.clock = -1;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRGreeAC::toString(void) {
  String result = "";
  result.reserve(220);  // Reserve some heap for the string to reduce fragging.
  result += addModelToString(decode_type_t::GREE, getModel(), false);
  result += addBoolToString(getPower(), kPowerStr);
  result += addModeToString(getMode(), kGreeAuto, kGreeCool, kGreeHeat,
                            kGreeDry, kGreeFan);
  result += addTempToString(getTemp(), !getUseFahrenheit());
  result += addFanToString(getFan(), kGreeFanMax, kGreeFanMin, kGreeFanAuto,
                           kGreeFanAuto, kGreeFanMed);
  result += addBoolToString(getTurbo(), kTurboStr);
  result += addBoolToString(getIFeel(), kIFeelStr);
  result += addBoolToString(getWiFi(), kWifiStr);
  result += addBoolToString(getXFan(), kXFanStr);
  result += addBoolToString(getLight(), kLightStr);
  result += addBoolToString(getSleep(), kSleepStr);
  result += addLabeledString(getSwingVerticalAuto() ? kAutoStr : kManualStr,
                             kSwingVModeStr);
  result += addIntToString(getSwingVerticalPosition(), kSwingVStr);
  result += kSpaceLBraceStr;
  switch (getSwingVerticalPosition()) {
    case kGreeSwingLastPos:
      result += kLastStr;
      break;
    case kGreeSwingAuto:
      result += kAutoStr;
      break;
    default: result += kUnknownStr;
  }
  result += ')';
  result += addLabeledString(
      getTimerEnabled() ? minsToString(getTimer()) : kOffStr, kTimerStr);
  uint8_t src = getDisplayTempSource();
  result += addIntToString(src, kDisplayTempStr);
  result += kSpaceLBraceStr;
  switch (src) {
    case kGreeDisplayTempOff:
      result += kOffStr;
      break;
    case kGreeDisplayTempSet:
      result += kSetStr;
      break;
    case kGreeDisplayTempInside:
      result += kInsideStr;
      break;
    case kGreeDisplayTempOutside:
      result += kOutsideStr;
      break;
    default: result += kUnknownStr;
  }
  result += ')';
  return result;
}

#if DECODE_GREE
/// Decode the supplied Gree HVAC message.
/// Status: STABLE / Working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeGree(decode_results* results, uint16_t offset,
                        const uint16_t nbits, bool const strict) {
  if (results->rawlen <=
      2 * (nbits + kGreeBlockFooterBits) + (kHeader + kFooter + 1) - 1 + offset)
    return false;  // Can't possibly be a valid Gree message.
  if (strict && nbits != kGreeBits)
    return false;  // Not strictly a Gree message.

  // There are two blocks back-to-back in a full Gree IR message
  // sequence.

  uint16_t used;
  // Header + Data Block #1 (32 bits)
  used = matchGeneric(results->rawbuf + offset, results->state,
                      results->rawlen - offset, nbits / 2,
                      kGreeHdrMark, kGreeHdrSpace,
                      kGreeBitMark, kGreeOneSpace,
                      kGreeBitMark, kGreeZeroSpace,
                      0, 0, false,
                      _tolerance, kMarkExcess, false);
  if (used == 0) return false;
  offset += used;

  // Block #1 footer (3 bits, B010)
  match_result_t data_result;
  data_result = matchData(&(results->rawbuf[offset]), kGreeBlockFooterBits,
                          kGreeBitMark, kGreeOneSpace, kGreeBitMark,
                          kGreeZeroSpace, _tolerance, kMarkExcess, false);
  if (data_result.success == false) return false;
  if (data_result.data != kGreeBlockFooter) return false;
  offset += data_result.used;

  // Inter-block gap + Data Block #2 (32 bits) + Footer
  if (!matchGeneric(results->rawbuf + offset, results->state + 4,
                    results->rawlen - offset, nbits / 2,
                    kGreeBitMark, kGreeMsgSpace,
                    kGreeBitMark, kGreeOneSpace,
                    kGreeBitMark, kGreeZeroSpace,
                    kGreeBitMark, kGreeMsgSpace, true,
                    _tolerance, kMarkExcess, false)) return false;

  // Compliance
  if (strict) {
    // Verify the message's checksum is correct.
    if (!IRGreeAC::validChecksum(results->state)) return false;
  }

  // Success
  results->decode_type = GREE;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_GREE
