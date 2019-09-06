// Copyright 2017 Ville Skyttä (scop)
// Copyright 2017, 2018 David Conran
//
// Code to emulate Gree protocol compatible HVAC devices.
// Should be compatible with:
// * Heat pumps carrying the "Ultimate" brand name.
// * EKOKAI air conditioners.
//

#include "ir_Gree.h"
#include <algorithm>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "IRutils.h"
#include "ir_Kelvinator.h"

// Constants
// Ref: https://github.com/ToniA/arduino-heatpumpir/blob/master/GreeHeatpumpIR.h
const uint16_t kGreeHdrMark = 9000;
const uint16_t kGreeHdrSpace = 4500;  // See #684 and real example in unit tests
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
using irutils::addFanToString;
using irutils::addTempToString;
using irutils::minsToString;

#if SEND_GREE
// Send a Gree Heat Pump message.
//
// Args:
//   data: An array of bytes containing the IR command.
//   nbytes: Nr. of bytes of data in the array. (>=kGreeStateLength)
//   repeat: Nr. of times the message is to be repeated. (Default = 0).
//
// Status: ALPHA / Untested.
//
// Ref:
//   https://github.com/ToniA/arduino-heatpumpir/blob/master/GreeHeatpumpIR.cpp
void IRsend::sendGree(const unsigned char data[], const uint16_t nbytes,
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

// Send a Gree Heat Pump message.
//
// Args:
//   data: The raw message to be sent.
//   nbits: Nr. of bits of data in the message. (Default is kGreeBits)
//   repeat: Nr. of times the message is to be repeated. (Default = 0).
//
// Status: ALPHA / Untested.
//
// Ref:
//   https://github.com/ToniA/arduino-heatpumpir/blob/master/GreeHeatpumpIR.cpp
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

IRGreeAC::IRGreeAC(const uint16_t pin, const gree_ac_remote_model_t model,
                   const bool inverted, const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) {
  stateReset();
  setModel(model);
}

void IRGreeAC::stateReset(void) {
  // This resets to a known-good state to Power Off, Fan Auto, Mode Auto, 25C.
  for (uint8_t i = 0; i < kGreeStateLength; i++) remote_state[i] = 0x0;
  remote_state[1] = 0x09;
  remote_state[2] = 0x20;
  remote_state[3] = 0x50;
  remote_state[5] = 0x20;
  remote_state[7] = 0x50;
}

void IRGreeAC::fixup(void) {
  setPower(getPower());  // Redo the power bits as they differ between models.
  checksum();  // Calculate the checksums
}

void IRGreeAC::begin(void) { _irsend.begin(); }

#if SEND_GREE
void IRGreeAC::send(const uint16_t repeat) {
  fixup();  // Ensure correct settings before sending.
  _irsend.sendGree(remote_state, kGreeStateLength, repeat);
}
#endif  // SEND_GREE

uint8_t* IRGreeAC::getRaw(void) {
  fixup();  // Ensure correct settings before sending.
  return remote_state;
}

void IRGreeAC::setRaw(const uint8_t new_code[]) {
  for (uint8_t i = 0; i < kGreeStateLength; i++) {
    remote_state[i] = new_code[i];
  }
  // We can only detect the difference between models when the power is on.
  if (getPower()) {
    if (remote_state[2] & kGreePower2Mask)
      _model = gree_ac_remote_model_t::YAW1F;
    else
      _model = gree_ac_remote_model_t::YBOFB;
  }
}

void IRGreeAC::checksum(const uint16_t length) {
  // Gree uses the same checksum alg. as Kelvinator's block checksum.
  uint8_t sum = IRKelvinatorAC::calcBlockChecksum(remote_state, length);
  remote_state[length - 1] = (sum << 4) | (remote_state[length - 1] & 0xFU);
}

// Verify the checksum is valid for a given state.
// Args:
//   state:  The array to verify the checksum of.
//   length: The size of the state.
// Returns:
//   A boolean.
bool IRGreeAC::validChecksum(const uint8_t state[], const uint16_t length) {
  // Top 4 bits of the last byte in the state is the state's checksum.
  return (state[length - 1] >> 4 == IRKelvinatorAC::calcBlockChecksum(state,
                                                                      length));
}

void IRGreeAC::setModel(const gree_ac_remote_model_t model) {
  switch (model) {
    case gree_ac_remote_model_t::YAW1F:
    case gree_ac_remote_model_t::YBOFB:
      _model = model; break;
    default:
      setModel(gree_ac_remote_model_t::YAW1F);
  }
}

gree_ac_remote_model_t IRGreeAC::getModel(void) {
  return _model;
}

void IRGreeAC::on(void) { setPower(true); }

void IRGreeAC::off(void) { setPower(false); }

void IRGreeAC::setPower(const bool on) {
  if (on) {
    remote_state[0] |= kGreePower1Mask;
    switch (_model) {
      case gree_ac_remote_model_t::YBOFB: break;
      default:
        remote_state[2] |= kGreePower2Mask;
    }
  } else {
    remote_state[0] &= ~kGreePower1Mask;
    remote_state[2] &= ~kGreePower2Mask;  // May not be needed. See #814
  }
}

bool IRGreeAC::getPower(void) {
  //  See #814. Not checking/requiring: (remote_state[2] & kGreePower2Mask)
  return remote_state[0] & kGreePower1Mask;
}

// Set the temp. in deg C
void IRGreeAC::setTemp(const uint8_t temp) {
  uint8_t new_temp = std::max((uint8_t)kGreeMinTemp, temp);
  new_temp = std::min((uint8_t)kGreeMaxTemp, new_temp);
  if (getMode() == kGreeAuto) new_temp = 25;
  remote_state[1] = (remote_state[1] & ~kGreeTempMask) |
                    (new_temp - kGreeMinTemp);
}

// Return the set temp. in deg C
uint8_t IRGreeAC::getTemp(void) {
  return ((remote_state[1] & kGreeTempMask) + kGreeMinTemp);
}

// Set the speed of the fan, 0-3, 0 is auto, 1-3 is the speed
void IRGreeAC::setFan(const uint8_t speed) {
  uint8_t fan = std::min((uint8_t)kGreeFanMax, speed);  // Bounds check

  if (getMode() == kGreeDry) fan = 1;  // DRY mode is always locked to fan 1.
  // Set the basic fan values.
  remote_state[0] &= ~kGreeFanMask;
  remote_state[0] |= (fan << 4);
}

uint8_t IRGreeAC::getFan(void) { return (remote_state[0] & kGreeFanMask) >> 4; }

void IRGreeAC::setMode(const uint8_t new_mode) {
  uint8_t mode = new_mode;
  switch (mode) {
    case kGreeAuto:
      // AUTO is locked to 25C
      setTemp(25);
      break;
    case kGreeDry:
      // DRY always sets the fan to 1.
      setFan(1);
      break;
    case kGreeCool:
    case kGreeFan:
    case kGreeHeat:
      break;
    default:
      // If we get an unexpected mode, default to AUTO.
      mode = kGreeAuto;
  }
  remote_state[0] &= ~kGreeModeMask;
  remote_state[0] |= mode;
}

uint8_t IRGreeAC::getMode(void) { return (remote_state[0] & kGreeModeMask); }

void IRGreeAC::setLight(const bool on) {
  if (on)
    remote_state[2] |= kGreeLightMask;
  else
    remote_state[2] &= ~kGreeLightMask;
}

bool IRGreeAC::getLight(void) { return remote_state[2] & kGreeLightMask; }

void IRGreeAC::setIFeel(const bool on) {
  if (on)
    remote_state[5] |= kGreeIFeelMask;
  else
    remote_state[5] &= ~kGreeIFeelMask;
}

bool IRGreeAC::getIFeel(void) { return remote_state[5] & kGreeIFeelMask; }

void IRGreeAC::setWiFi(const bool on) {
  if (on)
    remote_state[5] |= kGreeWiFiMask;
  else
    remote_state[5] &= ~kGreeWiFiMask;
}

bool IRGreeAC::getWiFi(void) { return remote_state[5] & kGreeWiFiMask; }

void IRGreeAC::setXFan(const bool on) {
  if (on)
    remote_state[2] |= kGreeXfanMask;
  else
    remote_state[2] &= ~kGreeXfanMask;
}

bool IRGreeAC::getXFan(void) { return remote_state[2] & kGreeXfanMask; }

void IRGreeAC::setSleep(const bool on) {
  if (on)
    remote_state[0] |= kGreeSleepMask;
  else
    remote_state[0] &= ~kGreeSleepMask;
}

bool IRGreeAC::getSleep(void) { return remote_state[0] & kGreeSleepMask; }

void IRGreeAC::setTurbo(const bool on) {
  if (on)
    remote_state[2] |= kGreeTurboMask;
  else
    remote_state[2] &= ~kGreeTurboMask;
}

bool IRGreeAC::getTurbo(void) { return remote_state[2] & kGreeTurboMask; }

void IRGreeAC::setSwingVertical(const bool automatic, const uint8_t position) {
  remote_state[0] &= ~kGreeSwingAutoMask;
  remote_state[0] |= (automatic << 6);
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
  remote_state[4] &= ~kGreeSwingPosMask;
  remote_state[4] |= new_position;
}

bool IRGreeAC::getSwingVerticalAuto(void) {
  return remote_state[0] & kGreeSwingAutoMask;
}

uint8_t IRGreeAC::getSwingVerticalPosition(void) {
  return remote_state[4] & kGreeSwingPosMask;
}

void IRGreeAC::setTimerEnabled(const bool on) {
  if (on)
    remote_state[1] |= kGreeTimerEnabledBit;
  else
    remote_state[1] &= ~kGreeTimerEnabledBit;
}

bool IRGreeAC::getTimerEnabled(void) {
  return remote_state[1] & kGreeTimerEnabledBit;
}

// Returns the number of minutes the timer is set for.
uint16_t IRGreeAC::getTimer(void) {
  uint16_t hrs = irutils::bcdToUint8(
      (remote_state[2] & kGreeTimerHoursMask) |
      ((remote_state[1] & kGreeTimerTensHrMask) >> 1));
  return hrs * 60 + ((remote_state[1] & kGreeTimerHalfHrBit) ? 30 : 0);
}

// Set the A/C's timer to turn off in X many minutes.
// Stores time internally in 30 min units.
//   e.g. 5 mins means 0 (& Off), 95 mins is  90 mins (& On). Max is 24 hours.
//
// Args:
//   minutes: The number of minutes the timer should be set for.
void IRGreeAC::setTimer(const uint16_t minutes) {
  // Clear the previous settings.
  remote_state[1] &= ~kGreeTimer1Mask;
  remote_state[2] &= ~kGreeTimerHoursMask;
  uint16_t mins = std::min(kGreeTimerMax, minutes);  // Bounds check.
  setTimerEnabled(mins >= 30);  // Timer is enabled when >= 30 mins.
  uint8_t hours = mins / 60;
  uint8_t halfhour = (mins % 60) < 30 ? 0 : 1;
  // Set the "tens" digit of hours & the half hour.
  remote_state[1] |= (((hours / 10) << 1) | halfhour) << 4;
  // Set the "units" digit of hours.
  remote_state[2] |= (hours % 10);
}

// Convert a standard A/C mode into its native mode.
uint8_t IRGreeAC::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool:
      return kGreeCool;
    case stdAc::opmode_t::kHeat:
      return kGreeHeat;
    case stdAc::opmode_t::kDry:
      return kGreeDry;
    case stdAc::opmode_t::kFan:
      return kGreeFan;
    default:
      return kGreeAuto;
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRGreeAC::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
      return kGreeFanMin;
    case stdAc::fanspeed_t::kLow:
    case stdAc::fanspeed_t::kMedium:
      return kGreeFanMax - 1;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:
      return kGreeFanMax;
    default:
      return kGreeFanAuto;
  }
}

// Convert a standard A/C Vertical Swing into its native version.
uint8_t IRGreeAC::convertSwingV(const stdAc::swingv_t swingv) {
  switch (swingv) {
    case stdAc::swingv_t::kHighest:
      return kGreeSwingUp;
    case stdAc::swingv_t::kHigh:
      return kGreeSwingMiddleUp;
    case stdAc::swingv_t::kMiddle:
      return kGreeSwingMiddle;
    case stdAc::swingv_t::kLow:
      return kGreeSwingMiddleDown;
    case stdAc::swingv_t::kLowest:
      return kGreeSwingDown;
    default:
      return kGreeSwingAuto;
  }
}

// Convert a native mode to it's common equivalent.
stdAc::opmode_t IRGreeAC::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kGreeCool: return stdAc::opmode_t::kCool;
    case kGreeHeat: return stdAc::opmode_t::kHeat;
    case kGreeDry: return stdAc::opmode_t::kDry;
    case kGreeFan: return stdAc::opmode_t::kFan;
    default: return stdAc::opmode_t::kAuto;
  }
}

// Convert a native fan speed to it's common equivalent.
stdAc::fanspeed_t IRGreeAC::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kGreeFanMax: return stdAc::fanspeed_t::kMax;
    case kGreeFanMax - 1: return stdAc::fanspeed_t::kMedium;
    case kGreeFanMin: return stdAc::fanspeed_t::kMin;
    default: return stdAc::fanspeed_t::kAuto;
  }
}

// Convert a native vertical swing to it's common equivalent.
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

// Convert the A/C state to it's common equivalent.
stdAc::state_t IRGreeAC::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::GREE;
  result.model = this->getModel();
  result.power = this->getPower();
  result.mode = this->toCommonMode(this->getMode());
  result.celsius = true;
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

// Convert the internal state into a human readable string.
String IRGreeAC::toString(void) {
  String result = "";
  result.reserve(150);  // Reserve some heap for the string to reduce fragging.
  result += addIntToString(getModel(), F("Model"), false);
  switch (getModel()) {
    case gree_ac_remote_model_t::YAW1F: result += F(" (YAW1F)"); break;
    case gree_ac_remote_model_t::YBOFB: result += F(" (YBOFB)"); break;
    default: result += F(" (UNKNOWN)");
  }
  result += addBoolToString(getPower(), F("Power"));
  result += addModeToString(getMode(), kGreeAuto, kGreeCool, kGreeHeat,
                            kGreeDry, kGreeFan);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kGreeFanMax, kGreeFanMin, kGreeFanAuto,
                           kGreeFanAuto, kGreeFanMed);
  result += addBoolToString(getTurbo(), F("Turbo"));
  result += addBoolToString(getIFeel(), F("IFeel"));
  result += addBoolToString(getWiFi(), F("WiFi"));
  result += addBoolToString(getXFan(), F("XFan"));
  result += addBoolToString(getLight(), F("Light"));
  result += addBoolToString(getSleep(), F("Sleep"));
  result += addLabeledString(getSwingVerticalAuto() ? F("Auto") : F("Manual"),
                             F("Swing Vertical Mode"));
  result += addIntToString(getSwingVerticalPosition(), F("Swing Vertical Pos"));
  switch (getSwingVerticalPosition()) {
    case kGreeSwingLastPos:
      result += F(" (Last Pos)");
      break;
    case kGreeSwingAuto:
      result += F(" (Auto)");
      break;
  }
  result += F(", Timer: ");
  if (getTimerEnabled())
    result += minsToString(getTimer());
  else
    result += F("Off");
  return result;
}

#if DECODE_GREE
// Decode the supplied Gree message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect. Typically kGreeBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: ALPHA / Untested.
bool IRrecv::decodeGree(decode_results* results, uint16_t nbits, bool strict) {
  if (results->rawlen <
      2 * (nbits + kGreeBlockFooterBits) + (kHeader + kFooter + 1))
    return false;  // Can't possibly be a valid Gree message.
  if (strict && nbits != kGreeBits)
    return false;  // Not strictly a Gree message.

  uint16_t offset = kStartOffset;

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
