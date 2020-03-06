// Copyright bakrus
// Copyright 2017,2019 David Conran

#include "ir_Coolix.h"
#include <algorithm>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

// Coolix A/C / heatpump added by (send) bakrus & (decode) crankyoldgit
//
// Supports:
//   Brand: Beko, Model: RG57K7(B)/BGEF Remote
//   Brand: Beko, Model: BINR 070/071 split-type A/C
//   Brand: Midea, Model: RG52D/BGE Remote
//   Brand: Midea, Model: MS12FU-10HRDN1-QRD0GW(B) A/C
//   Brand: Midea, Model: MSABAU-07HRFN1-QRD0GW A/C (circa 2016)
//   Brand: Tokio, Model: AATOEMF17-12CHR1SW split-type RG51|50/BGE Remote
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/484

// Constants
// Pulse parms are *50-100 for the Mark and *50+100 for the space
// First MARK is the one after the long gap
// pulse parameters in usec
const uint16_t kCoolixTick = 276;  // Approximately 10.5 cycles at 38kHz
const uint16_t kCoolixBitMarkTicks = 2;
const uint16_t kCoolixBitMark = kCoolixBitMarkTicks * kCoolixTick;
const uint16_t kCoolixOneSpaceTicks = 6;
const uint16_t kCoolixOneSpace = kCoolixOneSpaceTicks * kCoolixTick;
const uint16_t kCoolixZeroSpaceTicks = 2;
const uint16_t kCoolixZeroSpace = kCoolixZeroSpaceTicks * kCoolixTick;
const uint16_t kCoolixHdrMarkTicks = 17;
const uint16_t kCoolixHdrMark = kCoolixHdrMarkTicks * kCoolixTick;
const uint16_t kCoolixHdrSpaceTicks = 16;
const uint16_t kCoolixHdrSpace = kCoolixHdrSpaceTicks * kCoolixTick;
const uint16_t kCoolixMinGapTicks = kCoolixHdrMarkTicks + kCoolixZeroSpaceTicks;
const uint16_t kCoolixMinGap = kCoolixMinGapTicks * kCoolixTick;

using irutils::addBoolToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;
using irutils::setBit;
using irutils::setBits;

#if SEND_COOLIX
// Send a Coolix message
//
// Args:
//   data:   Contents of the message to be sent.
//   nbits:  Nr. of bits of data to be sent. Typically kCoolixBits.
//   repeat: Nr. of additional times the message is to be sent.
//
// Status: STABLE / Confirmed Working.
//
// Ref:
//   https://github.com/z3t0/Arduino-IRremote/blob/master/ir_COOLIX.cpp
void IRsend::sendCOOLIX(uint64_t data, uint16_t nbits, uint16_t repeat) {
  if (nbits % 8 != 0) return;  // nbits is required to be a multiple of 8.

  // Set IR carrier frequency
  enableIROut(38);

  for (uint16_t r = 0; r <= repeat; r++) {
    // Header
    mark(kCoolixHdrMark);
    space(kCoolixHdrSpace);

    // Data
    //   Break data into byte segments, starting at the Most Significant
    //   Byte. Each byte then being sent normal, then followed inverted.
    for (uint16_t i = 8; i <= nbits; i += 8) {
      // Grab a bytes worth of data.
      uint8_t segment = (data >> (nbits - i)) & 0xFF;
      // Normal
      sendData(kCoolixBitMark, kCoolixOneSpace, kCoolixBitMark,
               kCoolixZeroSpace, segment, 8, true);
      // Inverted.
      sendData(kCoolixBitMark, kCoolixOneSpace, kCoolixBitMark,
               kCoolixZeroSpace, segment ^ 0xFF, 8, true);
    }

    // Footer
    mark(kCoolixBitMark);
    space(kCoolixMinGap);  // Pause before repeating
  }
  space(kDefaultMessageGap);
}
#endif

// IRCoolixAC class
// Supports:
//   RG57K7(B)/BGEF remote control for Beko BINR 070/071 split-type aircon.
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/484
IRCoolixAC::IRCoolixAC(const uint16_t pin, const bool inverted,
                       const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

void IRCoolixAC::stateReset() {
  setRaw(kCoolixDefaultState);
  clearSensorTemp();
  powerFlag = false;
  turboFlag = false;
  ledFlag = false;
  cleanFlag = false;
  sleepFlag = false;
  swingFlag = false;
  swingHFlag = false;
  swingVFlag = false;
}

void IRCoolixAC::begin() { _irsend.begin(); }

#if SEND_COOLIX
void IRCoolixAC::send(const uint16_t repeat) {
  _irsend.sendCOOLIX(remote_state, kCoolixBits, repeat);
  // make sure to remove special state from remote_state
  // after command has being transmitted.
  recoverSavedState();
}
#endif  // SEND_COOLIX

uint32_t IRCoolixAC::getRaw() { return remote_state; }

void IRCoolixAC::setRaw(const uint32_t new_code) {
  powerFlag = true;  // Everything that is not the special power off mesg is On.
  if (!handleSpecialState(new_code)) {
    // it isn`t special so might affect Temp|mode|Fan
    if (new_code == kCoolixCmdFan) {
      setMode(kCoolixFan);
      return;
    }
  }
  // must be a command changing Temp|Mode|Fan
  // it is safe to just copy to remote var
  remote_state = new_code;
}

// Return true if the current state is a special state.
bool IRCoolixAC::isSpecialState(void) {
  switch (remote_state) {
    case kCoolixClean:
    case kCoolixLed:
    case kCoolixOff:
    case kCoolixSwing:
    case kCoolixSleep:
    case kCoolixTurbo: return true;
    default: return false;
  }
}

// Special state means commands that are not
// affecting Temperature/Mode/Fan
bool IRCoolixAC::handleSpecialState(const uint32_t data) {
  switch (data) {
    case kCoolixClean:
      cleanFlag = !cleanFlag;
      break;
    case kCoolixLed:
      ledFlag = !ledFlag;
      break;
    case kCoolixOff:
      powerFlag = false;
      break;
    case kCoolixSwing:
      swingFlag = !swingFlag;
      break;
    case kCoolixSleep:
      sleepFlag = !sleepFlag;
      break;
    case kCoolixTurbo:
      turboFlag = !turboFlag;
      break;
    default:
      return false;
  }
  return true;
}

// must be called before every special state
// to make sure the remote_state is safe
void IRCoolixAC::updateSavedState(void) {
  if (!isSpecialState()) saved_state = remote_state;
}

void IRCoolixAC::recoverSavedState(void) {
  // If the current state is a special one, last known normal one.
  if (isSpecialState()) remote_state = saved_state;
  // If the saved_state was also a special state, reset as we expect a normal
  // state out of all this.
  if (isSpecialState()) stateReset();
}

void IRCoolixAC::setTempRaw(const uint8_t code) {
  setBits(&remote_state, kCoolixTempOffset, kCoolixTempSize, code);
}

uint8_t IRCoolixAC::getTempRaw() {
  return GETBITS32(remote_state, kCoolixTempOffset, kCoolixTempSize);
}

void IRCoolixAC::setTemp(const uint8_t desired) {
  // Range check.
  uint8_t temp = std::min(desired, kCoolixTempMax);
  temp = std::max(temp, kCoolixTempMin);
  setTempRaw(kCoolixTempMap[temp - kCoolixTempMin]);
}

uint8_t IRCoolixAC::getTemp() {
  const uint8_t code = getTempRaw();
  for (uint8_t i = 0; i < kCoolixTempRange; i++)
    if (kCoolixTempMap[i] == code) return kCoolixTempMin + i;
  return kCoolixTempMax;  // Not a temp we expected.
}

void IRCoolixAC::setSensorTempRaw(const uint8_t code) {
  setBits(&remote_state, kCoolixSensorTempOffset, kCoolixSensorTempSize, code);
}

void IRCoolixAC::setSensorTemp(const uint8_t desired) {
  uint8_t temp = desired;
  temp = std::min(temp, kCoolixSensorTempMax);
  temp = std::max(temp, kCoolixSensorTempMin);
  setSensorTempRaw(temp - kCoolixSensorTempMin);
  setZoneFollow(true);  // Setting a Sensor temp means you want to Zone Follow.
}

uint8_t IRCoolixAC::getSensorTemp() {
  return GETBITS32(remote_state, kCoolixSensorTempOffset,
                   kCoolixSensorTempSize) + kCoolixSensorTempMin;
}

bool IRCoolixAC::getPower() {
  // There is only an off state. Everything else is "on".
  return powerFlag;
}

void IRCoolixAC::setPower(const bool on) {
  if (!on) {
    updateSavedState();
    remote_state = kCoolixOff;
  } else if (!powerFlag) {
    // at this point remote_state must be ready
    // to be transmitted
    recoverSavedState();
  }
  powerFlag = on;
}

void IRCoolixAC::on(void) { this->setPower(true); }

void IRCoolixAC::off(void) { this->setPower(false); }

bool IRCoolixAC::getSwing() { return swingFlag; }

void IRCoolixAC::setSwing() {
  // Assumes that repeated sending "swing" toggles the action on the device.
  updateSavedState();
  remote_state = kCoolixSwing;
  swingFlag = !swingFlag;
}

bool IRCoolixAC::getSleep() { return sleepFlag; }

void IRCoolixAC::setSleep() {
  updateSavedState();
  remote_state = kCoolixSleep;
  sleepFlag = !sleepFlag;
}

bool IRCoolixAC::getTurbo() { return turboFlag; }

void IRCoolixAC::setTurbo() {
  // Assumes that repeated sending "turbo" toggles the action on the device.
  updateSavedState();
  remote_state = kCoolixTurbo;
  turboFlag = !turboFlag;
}

bool IRCoolixAC::getLed() { return ledFlag; }

void IRCoolixAC::setLed() {
  // Assumes that repeated sending "Led" toggles the action on the device.
  updateSavedState();
  remote_state = kCoolixLed;
  ledFlag = !ledFlag;
}

bool IRCoolixAC::getClean() { return cleanFlag; }

void IRCoolixAC::setClean() {
  updateSavedState();
  remote_state = kCoolixClean;
  cleanFlag = !cleanFlag;
}

bool IRCoolixAC::getZoneFollow() {
  return zoneFollowFlag;
}

// Internal use only.
void IRCoolixAC::setZoneFollow(bool on) {
  zoneFollowFlag = on;
  setBit(&remote_state, kCoolixZoneFollowMaskOffset, on);
}

void IRCoolixAC::clearSensorTemp() {
  setZoneFollow(false);
  setSensorTempRaw(kCoolixSensorTempIgnoreCode);
}

void IRCoolixAC::setMode(const uint8_t mode) {
  uint32_t actualmode = mode;
  switch (actualmode) {
    case kCoolixAuto:
    case kCoolixDry:
      setFan(kCoolixFanAuto0, false);
      break;
    case kCoolixCool:
    case kCoolixHeat:
    case kCoolixFan:
      setFan(kCoolixFanAuto, false);
      break;
    default:  // Anything else, go with Auto mode.
      setMode(kCoolixAuto);
      setFan(kCoolixFanAuto0, false);
      return;
  }
  setTemp(getTemp());
  // Fan mode is a special case of Dry.
  if (mode == kCoolixFan) {
    actualmode = kCoolixDry;
    setTempRaw(kCoolixFanTempCode);
  }
  setBits(&remote_state, kCoolixModeOffset, kCoolixModeSize, actualmode);
}

uint8_t IRCoolixAC::getMode() {
  uint8_t mode = GETBITS32(remote_state, kCoolixModeOffset,
                           kCoolixModeSize);
  if (mode == kCoolixDry)
    if (getTempRaw() == kCoolixFanTempCode) return kCoolixFan;
  return mode;
}

uint8_t IRCoolixAC::getFan() {
  return GETBITS32(remote_state, kCoolixFanOffset, kCoolixFanSize);
}

void IRCoolixAC::setFan(const uint8_t speed, const bool modecheck) {
  uint8_t newspeed = speed;
  switch (speed) {
    case kCoolixFanAuto:  // Dry & Auto mode can't have this speed.
      if (modecheck) {
        switch (this->getMode()) {
          case kCoolixAuto:
          case kCoolixDry:
            newspeed = kCoolixFanAuto0;
          break;
        }
      }
      break;
    case kCoolixFanAuto0:  // Only Dry & Auto mode can have this speed.
      if (modecheck) {
        switch (this->getMode()) {
          case kCoolixAuto:
          case kCoolixDry: break;
          default: newspeed = kCoolixFanAuto;
        }
      }
      break;
    case kCoolixFanMin:
    case kCoolixFanMed:
    case kCoolixFanMax:
    case kCoolixFanZoneFollow:
    case kCoolixFanFixed:
      break;
    default:  // Unknown speed requested.
      newspeed = kCoolixFanAuto;
      break;
  }
  setBits(&remote_state, kCoolixFanOffset, kCoolixFanSize, newspeed);
}

// Convert a standard A/C mode into its native mode.
uint8_t IRCoolixAC::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kCoolixCool;
    case stdAc::opmode_t::kHeat: return kCoolixHeat;
    case stdAc::opmode_t::kDry: return kCoolixDry;
    case stdAc::opmode_t::kFan: return kCoolixFan;
    default: return kCoolixAuto;
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRCoolixAC::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow: return kCoolixFanMin;
    case stdAc::fanspeed_t::kMedium: return kCoolixFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax: return kCoolixFanMax;
    default: return kCoolixFanAuto;
  }
}

// Convert a native mode to it's common equivalent.
stdAc::opmode_t IRCoolixAC::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kCoolixCool: return stdAc::opmode_t::kCool;
    case kCoolixHeat: return stdAc::opmode_t::kHeat;
    case kCoolixDry: return stdAc::opmode_t::kDry;
    case kCoolixFan: return stdAc::opmode_t::kFan;
    default: return stdAc::opmode_t::kAuto;
  }
}

// Convert a native fan speed to it's common equivalent.
stdAc::fanspeed_t IRCoolixAC::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kCoolixFanMax: return stdAc::fanspeed_t::kMax;
    case kCoolixFanMed: return stdAc::fanspeed_t::kMedium;
    case kCoolixFanMin: return stdAc::fanspeed_t::kMin;
    default: return stdAc::fanspeed_t::kAuto;
  }
}

// Convert the A/C state to it's common equivalent. Utilise the previous
// state if supplied.
stdAc::state_t IRCoolixAC::toCommon(const stdAc::state_t *prev) {
  stdAc::state_t result;
  // Start with the previous state if given it.
  if (prev != NULL) {
    result = *prev;
  } else {
    // Set defaults for non-zero values that are not implicitly set for when
    // there is no previous state.
    // e.g. Any setting that toggles should probably go here.
    result.swingv = stdAc::swingv_t::kOff;
    result.turbo = false;
    result.clean = false;
    result.light = false;
    result.sleep = -1;
  }
  // Not supported.
  result.model = -1;  // No models used.
  result.swingh = stdAc::swingh_t::kOff;
  result.quiet = false;
  result.econo = false;
  result.filter = false;
  result.beep = false;
  result.clock = -1;

  // Supported.
  result.protocol = decode_type_t::COOLIX;
  result.celsius = true;
  result.power = this->getPower();
  // Power off state no other state info. Use the previous state if we have it.
  if (!result.power) return result;
  // Handle the special single command (Swing/Turbo/Light/Clean/Sleep) toggle
  // messages. These have no other state info so use the rest of the previous
  // state if we have it for them.
  if (this->getSwing()) {
    result.swingv = result.swingv != stdAc::swingv_t::kOff ?
        stdAc::swingv_t::kOff : stdAc::swingv_t::kAuto;  // Invert swing.
    return result;
  } else if (this->getTurbo()) {
    result.turbo = !result.turbo;
    return result;
  } else if (this->getLed()) {
    result.light = !result.light;
    return result;
  } else if (this->getClean()) {
    result.clean = !result.clean;
    return result;
  } else if (this->getSleep()) {
    result.sleep = result.sleep >= 0 ? -1 : 0;  // Invert sleep.
    return result;
  }
  // Back to "normal" stateful messages.
  result.mode = this->toCommonMode(this->getMode());
  result.degrees = this->getTemp();
  result.fanspeed = this->toCommonFanSpeed(this->getFan());
  return result;
}

// Convert the internal state into a human readable string.
String IRCoolixAC::toString(void) {
  String result = "";
  result.reserve(100);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  if (!getPower()) return result;  // If it's off, there is no other info.
  // Special modes.
  if (getSwing()) {
    result += kCommaSpaceStr;
    result += kSwingStr;
    result += kColonSpaceStr;
    result += kToggleStr;
    return result;
  }
  if (getSleep()) {
    result += kCommaSpaceStr;
    result += kSleepStr;
    result += kColonSpaceStr;
    result += kToggleStr;
    return result;
  }
  if (getTurbo()) {
    result += kCommaSpaceStr;
    result += kTurboStr;
    result += kColonSpaceStr;
    result += kToggleStr;
    return result;
  }
  if (getLed()) {
    result += kCommaSpaceStr;
    result += kLightStr;
    result += kColonSpaceStr;
    result += kToggleStr;
    return result;
  }
  if (getClean()) {
    result += kCommaSpaceStr;
    result += kCleanStr;
    result += kColonSpaceStr;
    result += kToggleStr;
    return result;
  }
  result += addModeToString(getMode(), kCoolixAuto, kCoolixCool, kCoolixHeat,
                            kCoolixDry, kCoolixFan);
  result += addIntToString(getFan(), kFanStr);
  result += kSpaceLBraceStr;
  switch (getFan()) {
    case kCoolixFanAuto:
      result += kAutoStr;
      break;
    case kCoolixFanAuto0:
      result += kAutoStr;
      result += '0';
      break;
    case kCoolixFanMax:
      result += kMaxStr;
      break;
    case kCoolixFanMin:
      result += kMinStr;
      break;
    case kCoolixFanMed:
      result += kMedStr;
      break;
    case kCoolixFanZoneFollow:
      result += kZoneFollowStr;
      break;
    case kCoolixFanFixed:
      result += kFixedStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  // Fan mode doesn't have a temperature.
  if (getMode() != kCoolixFan) result += addTempToString(getTemp());
  result += addBoolToString(getZoneFollow(), kZoneFollowStr);
  result += addLabeledString(
      (getSensorTemp() > kCoolixSensorTempMax)
          ? kOffStr : uint64ToString(getSensorTemp()) + 'C', kSensorTempStr);
  return result;
}

#if DECODE_COOLIX
// Decode the supplied Coolix message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   offset:  The starting index to use when attempting to decode the raw data.
//            Typically/Defaults to kStartOffset.
//   nbits:   The number of data bits to expect. Typically kCoolixBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: BETA / Probably working.
bool IRrecv::decodeCOOLIX(decode_results *results, uint16_t offset,
                          const uint16_t nbits, const bool strict) {
  // The protocol sends the data normal + inverted, alternating on
  // each byte. Hence twice the number of expected data bits.
  if (results->rawlen <= 2 * 2 * nbits + kHeader + kFooter - 1 + offset)
    return false;  // Can't possibly be a valid COOLIX message.
  if (strict && nbits != kCoolixBits)
    return false;      // Not strictly a COOLIX message.
  if (nbits % 8 != 0)  // nbits has to be a multiple of nr. of bits in a byte.
    return false;

  uint64_t data = 0;
  uint64_t inverted = 0;

  if (nbits > sizeof(data) * 8)
    return false;  // We can't possibly capture a Coolix packet that big.

  // Header
  if (!matchMark(results->rawbuf[offset], kCoolixHdrMark)) return false;
  // Calculate how long the common tick time is based on the header mark.
  uint32_t m_tick = results->rawbuf[offset++] * kRawTick / kCoolixHdrMarkTicks;
  if (!matchSpace(results->rawbuf[offset], kCoolixHdrSpace)) return false;
  // Calculate how long the common tick time is based on the header space.
  uint32_t s_tick = results->rawbuf[offset++] * kRawTick / kCoolixHdrSpaceTicks;

  // Data
  // Twice as many bits as there are normal plus inverted bits.
  for (uint16_t i = 0; i < nbits * 2; i++, offset++) {
    bool flip = (i / 8) % 2;
    if (!matchMark(results->rawbuf[offset++], kCoolixBitMarkTicks * m_tick))
      return false;
    if (matchSpace(results->rawbuf[offset], kCoolixOneSpaceTicks * s_tick)) {
      if (flip)
        inverted = (inverted << 1) | 1;
      else
        data = (data << 1) | 1;
    } else if (matchSpace(results->rawbuf[offset],
                          kCoolixZeroSpaceTicks * s_tick)) {
      if (flip)
        inverted <<= 1;
      else
        data <<= 1;
    } else {
      return false;
    }
  }

  // Footer
  if (!matchMark(results->rawbuf[offset++], kCoolixBitMarkTicks * m_tick))
    return false;
  if (offset < results->rawlen &&
      !matchAtLeast(results->rawbuf[offset], kCoolixMinGapTicks * s_tick))
    return false;

  // Compliance
  uint64_t orig = data;  // Save a copy of the data.
  if (strict) {
    for (uint16_t i = 0; i < nbits; i += 8, data >>= 8, inverted >>= 8)
      if ((data & 0xFF) != ((inverted & 0xFF) ^ 0xFF)) return false;
  }

  // Success
  results->decode_type = COOLIX;
  results->bits = nbits;
  results->value = orig;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif
