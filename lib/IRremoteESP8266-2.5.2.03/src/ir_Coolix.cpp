// Copyright bakrus
// Copyright 2017 David Conran

#include "ir_Coolix.h"
#include <algorithm>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

//             CCCCC   OOOOO   OOOOO  LL      IIIII XX    XX
//            CC    C OO   OO OO   OO LL       III   XX  XX
//            CC      OO   OO OO   OO LL       III    XXXX
//            CC    C OO   OO OO   OO LL       III   XX  XX
//             CCCCC   OOOO0   OOOO0  LLLLLLL IIIII XX    XX

// Coolix A/C / heatpump added by (send) bakrus & (decode) crankyoldgit
//
// Supports:
//   RG57K7(B)/BGEF remote control for Beko BINR 070/071 split-type aircon.
// Ref:
//   https://github.com/markszabo/IRremoteESP8266/issues/484

// Constants
// Pulse parms are *50-100 for the Mark and *50+100 for the space
// First MARK is the one after the long gap
// pulse parameters in usec
const uint16_t kCoolixTick = 560;  // Approximately 21 cycles at 38kHz
const uint16_t kCoolixBitMarkTicks = 1;
const uint16_t kCoolixBitMark = kCoolixBitMarkTicks * kCoolixTick;
const uint16_t kCoolixOneSpaceTicks = 3;
const uint16_t kCoolixOneSpace = kCoolixOneSpaceTicks * kCoolixTick;
const uint16_t kCoolixZeroSpaceTicks = 1;
const uint16_t kCoolixZeroSpace = kCoolixZeroSpaceTicks * kCoolixTick;
const uint16_t kCoolixHdrMarkTicks = 8;
const uint16_t kCoolixHdrMark = kCoolixHdrMarkTicks * kCoolixTick;
const uint16_t kCoolixHdrSpaceTicks = 8;
const uint16_t kCoolixHdrSpace = kCoolixHdrSpaceTicks * kCoolixTick;
const uint16_t kCoolixMinGapTicks = kCoolixHdrMarkTicks + kCoolixZeroSpaceTicks;
const uint16_t kCoolixMinGap = kCoolixMinGapTicks * kCoolixTick;

#if SEND_COOLIX
// Send a Coolix message
//
// Args:
//   data:   Contents of the message to be sent.
//   nbits:  Nr. of bits of data to be sent. Typically kCoolixBits.
//   repeat: Nr. of additional times the message is to be sent.
//
// Status: BETA / Probably works.
//
// Ref:
//   https://github.com/z3t0/Arduino-IRremote/blob/master/ir_COOLIX.cpp
// TODO(anyone): Verify repeat functionality against a real unit.
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
}
#endif

// IRCoolixAC class
// Supports:
//   RG57K7(B)/BGEF remote control for Beko BINR 070/071 split-type aircon.
// Ref:
//   https://github.com/markszabo/IRremoteESP8266/issues/484
IRCoolixAC::IRCoolixAC(uint16_t pin) : _irsend(pin) { stateReset(); }

void IRCoolixAC::stateReset() { remote_state = kCoolixDefaultState; }

void IRCoolixAC::begin() { _irsend.begin(); }

#if SEND_COOLIX
void IRCoolixAC::send() { _irsend.sendCOOLIX(remote_state); }
#endif  // SEND_COOLIX

uint32_t IRCoolixAC::getRaw() { return remote_state; }

void IRCoolixAC::setRaw(const uint32_t new_code) { remote_state = new_code; }

void IRCoolixAC::setTempRaw(const uint8_t code) {
  remote_state &= ~kCoolixTempMask;  // Clear the old temp.
  remote_state |= (code << 4);
}

uint8_t IRCoolixAC::getTempRaw() {
  return (remote_state & kCoolixTempMask) >> 4;
}

void IRCoolixAC::setTemp(const uint8_t desired) {
  // Range check.
  uint8_t temp = std::min(desired, kCoolixTempMax);
  temp = std::max(temp, kCoolixTempMin);
  setTempRaw(kCoolixTempMap[temp - kCoolixTempMin]);
}

uint8_t IRCoolixAC::getTemp() {
  uint8_t code = getTempRaw();
  uint8_t i;
  for (i = 0; i < kCoolixTempRange; i++)
    if (kCoolixTempMap[i] == code) return kCoolixTempMin + i;
  return kCoolixUnknown;  // Not a temp we expected.
}

void IRCoolixAC::setSensorTempRaw(const uint8_t code) {
  remote_state &= ~kCoolixSensorTempMask;  // Clear previous sensor temp.
  remote_state |= ((code & 0xF) << 8);
}

void IRCoolixAC::setSensorTemp(const uint8_t desired) {
  uint8_t temp = desired;
  temp = std::min(temp, kCoolixSensorTempMax);
  temp = std::max(temp, kCoolixSensorTempMin);
  setSensorTempRaw(temp - kCoolixSensorTempMin);
  setZoneFollow(true);  // Setting a Sensor temp means you want to Zone Follow.
}

uint8_t IRCoolixAC::getSensorTemp() {
  return ((remote_state & kCoolixSensorTempMask) >> 8) + kCoolixSensorTempMin;
}

bool IRCoolixAC::getPower() {
  // There is only an off state. Everything else is "on".
  return remote_state != kCoolixOff;
}

void IRCoolixAC::setPower(const bool power) {
  if (!power) remote_state = kCoolixOff;
  // There really is no distinct "on" setting, so do nothing.
}

bool IRCoolixAC::getSwing() { return remote_state == kCoolixSwing; }

void IRCoolixAC::setSwing() {
  // Assumes that repeated sending "swing" toggles the action on the device.
  remote_state = kCoolixSwing;
}

bool IRCoolixAC::getSleep() { return remote_state == kCoolixSleep; }

void IRCoolixAC::setSleep() { remote_state = kCoolixSleep; }

bool IRCoolixAC::getTurbo() { return remote_state == kCoolixTurbo; }

void IRCoolixAC::setTurbo() {
  // Assumes that repeated sending "turbo" toggles the action on the device.
  remote_state = kCoolixTurbo;
}

bool IRCoolixAC::getLed() { return remote_state == kCoolixLed; }

void IRCoolixAC::setLed() {
  // Assumes that repeated sending "Led" toggles the action on the device.
  remote_state = kCoolixLed;
}

bool IRCoolixAC::getClean() { return remote_state == kCoolixClean; }

void IRCoolixAC::setClean() { remote_state = kCoolixClean; }

bool IRCoolixAC::getZoneFollow() {
  return remote_state & kCoolixZoneFollowMask;
}

// Internal use only.
void IRCoolixAC::setZoneFollow(bool state) {
  if (state) {
    remote_state |= kCoolixZoneFollowMask;
  } else {
    remote_state &= ~kCoolixZoneFollowMask;
  }
}

void IRCoolixAC::clearSensorTemp() {
  setZoneFollow(false);
  setSensorTempRaw(kCoolixSensorTempIgnoreCode);
}

void IRCoolixAC::setMode(const uint8_t mode) {
  uint32_t actualmode = mode;
  // Fan mode is a special case of Dry.
  if (mode == kCoolixFan) actualmode = kCoolixDry;
  switch (actualmode) {
    case kCoolixCool:
    case kCoolixAuto:
    case kCoolixHeat:
    case kCoolixDry:
      remote_state = (remote_state & ~kCoolixModeMask) | (actualmode << 2);
      // Force the temp into a known-good state.
      setTemp(getTemp());
  }
  if (mode == kCoolixFan) setTempRaw(kCoolixFanTempCode);
}

uint8_t IRCoolixAC::getMode() {
  uint8_t mode = (remote_state & kCoolixModeMask) >> 2;
  if (mode == kCoolixDry)
    if (getTempRaw() == kCoolixFanTempCode) return kCoolixFan;
  return mode;
}

uint8_t IRCoolixAC::getFan() { return (remote_state & kCoolixFanMask) >> 13; }

void IRCoolixAC::setFan(const uint8_t speed) {
  uint8_t newspeed = speed;
  switch (speed) {
    case kCoolixFanMin:
    case kCoolixFanMed:
    case kCoolixFanMax:
    case kCoolixFanAuto:
    case kCoolixFanZoneFollow:
    case kCoolixFanFixed:
      break;
    default:  // Unknown speed requested.
      newspeed = kCoolixFanAuto;
  }
  remote_state &= ~kCoolixFanMask;
  remote_state |= ((newspeed << 13) & kCoolixFanMask);
}

// Convert the internal state into a human readable string.
#ifdef ARDUINO
String IRCoolixAC::toString() {
  String result = "";
#else
std::string IRCoolixAC::toString() {
  std::string result = "";
#endif  // ARDUINO
  result += "Power: ";
  if (getPower()) {
    result += "On";
  } else {
    result += "Off";
    return result;  // If it's off, there is no other info.
  }
  result += ", Fan: " + uint64ToString(getFan());
  switch (getFan()) {
    case kCoolixFanAuto:
      result += " (AUTO)";
      break;
    case kCoolixFanMax:
      result += " (MAX)";
      break;
    case kCoolixFanMin:
      result += " (MIN)";
      break;
    case kCoolixFanMed:
      result += " (MED)";
      break;
    case kCoolixFanZoneFollow:
      result += " (ZONEFOLLOW)";
      break;
    case kCoolixFanFixed:
      result += " (FIXED)";
      break;
    default:
      result += " (UNKNOWN)";
  }
  // Special modes.
  if (getSwing()) {
    result += ", Swing: Toggle";
    return result;
  }
  if (getSleep()) {
    result += ", Sleep: Toggle";
    return result;
  }
  if (getTurbo()) {
    result += ", Turbo: Toggle";
    return result;
  }
  if (getLed()) {
    result += ", Led: Toggle";
    return result;
  }
  if (getClean()) {
    result += ", Mode: Self clean";
    return result;
  }
  result += ", Mode: " + uint64ToString(getMode());
  switch (getMode()) {
    case kCoolixAuto:
      result += " (AUTO)";
      break;
    case kCoolixCool:
      result += " (COOL)";
      break;
    case kCoolixHeat:
      result += " (HEAT)";
      break;
    case kCoolixDry:
      result += " (DRY)";
      break;
    case kCoolixFan:
      result += " (FAN)";
      break;
    default:
      result += " (UNKNOWN)";
  }
  if (getMode() != kCoolixFan)  // Fan mode doesn't have a temperature.
    result += ", Temp: " + uint64ToString(getTemp()) + "C";
  result += ", Zone Follow: ";
  if (getZoneFollow())
    result += "On";
  else
    result += "Off";
  result += ", Sensor Temp: ";
  if (getSensorTemp() > kCoolixSensorTempMax)
    result += "Ignored";
  else
    result += uint64ToString(getSensorTemp()) + "C";
  return result;
}

#if DECODE_COOLIX
// Decode the supplied Coolix message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect. Typically kCoolixBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: BETA / Probably working.
bool IRrecv::decodeCOOLIX(decode_results *results, uint16_t nbits,
                          bool strict) {
  // The protocol sends the data normal + inverted, alternating on
  // each byte. Hence twice the number of expected data bits.
  if (results->rawlen < 2 * 2 * nbits + kHeader + kFooter - 1)
    return false;  // Can't possibly be a valid COOLIX message.
  if (strict && nbits != kCoolixBits)
    return false;      // Not strictly a COOLIX message.
  if (nbits % 8 != 0)  // nbits has to be a multiple of nr. of bits in a byte.
    return false;

  uint64_t data = 0;
  uint64_t inverted = 0;
  uint16_t offset = kStartOffset;

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
