// Copyright 2015 Kristian Lauszus
// Copyright 2017, 2018 David Conran

#include "ir_Panasonic.h"
#include <algorithm>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

//       PPPP    AAA   N   N   AAA    SSSS   OOO   N   N  IIIII   CCCC
//       P   P  A   A  NN  N  A   A  S      O   O  NN  N    I    C
//       PPPP   AAAAA  N N N  AAAAA   SSS   O   O  N N N    I    C
//       P      A   A  N  NN  A   A      S  O   O  N  NN    I    C
//       P      A   A  N   N  A   A  SSSS    OOO   N   N  IIIII   CCCC

// Panasonic protocol originally added by Kristian Lauszus from:
//   https://github.com/z3t0/Arduino-IRremote
// (Thanks to zenwheel and other people at the original blog post)
//
// Panasonic A/C support add by crankyoldgit but heavily influenced by:
//   https://github.com/ToniA/ESPEasy/blob/HeatpumpIR/lib/HeatpumpIR/PanasonicHeatpumpIR.cpp
// Panasonic A/C Clock & Timer support:
//   Reverse Engineering by MikkelTb
//   Code by crankyoldgit
// Panasonic A/C models supported:
//   A/C Series/models:
//     JKE, LKE, DKE, CKP, & NKE series. (In theory)
//     CS-YW9MKD (confirmed)
//     CS-ME14CKPG / CS-ME12CKPG / CS-ME10CKPG
//   A/C Remotes:
//     A75C3747 (confirmed)
//     A75C3704
//     A75C2311 (CKP)

// Constants
// Ref:
//   http://www.remotecentral.com/cgi-bin/mboard/rc-pronto/thread.cgi?26152

const uint16_t kPanasonicTick = 432;
const uint16_t kPanasonicHdrMarkTicks = 8;
const uint16_t kPanasonicHdrMark = kPanasonicHdrMarkTicks * kPanasonicTick;
const uint16_t kPanasonicHdrSpaceTicks = 4;
const uint16_t kPanasonicHdrSpace = kPanasonicHdrSpaceTicks * kPanasonicTick;
const uint16_t kPanasonicBitMarkTicks = 1;
const uint16_t kPanasonicBitMark = kPanasonicBitMarkTicks * kPanasonicTick;
const uint16_t kPanasonicOneSpaceTicks = 3;
const uint16_t kPanasonicOneSpace = kPanasonicOneSpaceTicks * kPanasonicTick;
const uint16_t kPanasonicZeroSpaceTicks = 1;
const uint16_t kPanasonicZeroSpace = kPanasonicZeroSpaceTicks * kPanasonicTick;
const uint16_t kPanasonicMinCommandLengthTicks = 378;
const uint32_t kPanasonicMinCommandLength =
    kPanasonicMinCommandLengthTicks * kPanasonicTick;
const uint16_t kPanasonicEndGap = 5000;  // See issue #245
const uint16_t kPanasonicMinGapTicks =
    kPanasonicMinCommandLengthTicks -
    (kPanasonicHdrMarkTicks + kPanasonicHdrSpaceTicks +
     kPanasonicBits * (kPanasonicBitMarkTicks + kPanasonicOneSpaceTicks) +
     kPanasonicBitMarkTicks);
const uint32_t kPanasonicMinGap = kPanasonicMinGapTicks * kPanasonicTick;

const uint16_t kPanasonicAcSectionGap = 10000;
const uint16_t kPanasonicAcSection1Length = 8;
const uint32_t kPanasonicAcMessageGap = 100000;  // A complete guess.

#if (SEND_PANASONIC || SEND_DENON)
// Send a Panasonic formatted message.
//
// Args:
//   data:   The message to be sent.
//   nbits:  The number of bits of the message to be sent. (kPanasonicBits).
//   repeat: The number of times the command is to be repeated.
//
// Status: BETA / Should be working.
//
// Note:
//   This protocol is a modified version of Kaseikyo.
void IRsend::sendPanasonic64(uint64_t data, uint16_t nbits, uint16_t repeat) {
  sendGeneric(kPanasonicHdrMark, kPanasonicHdrSpace, kPanasonicBitMark,
              kPanasonicOneSpace, kPanasonicBitMark, kPanasonicZeroSpace,
              kPanasonicBitMark, kPanasonicMinGap, kPanasonicMinCommandLength,
              data, nbits, kPanasonicFreq, true, repeat, 50);
}

// Send a Panasonic formatted message.
//
// Args:
//   address: The manufacturer code.
//   data:    The data portion to be sent.
//   nbits:   The number of bits of the message to be sent. (kPanasonicBits).
//   repeat:  The number of times the command is to be repeated.
//
// Status: STABLE.
//
// Note:
//   This protocol is a modified version of Kaseikyo.
void IRsend::sendPanasonic(uint16_t address, uint32_t data, uint16_t nbits,
                           uint16_t repeat) {
  sendPanasonic64(((uint64_t)address << 32) | (uint64_t)data, nbits, repeat);
}

// Calculate the raw Panasonic data based on device, subdevice, & function.
//
// Args:
//   manufacturer: A 16-bit manufacturer code. e.g. 0x4004 is Panasonic.
//   device:       An 8-bit code.
//   subdevice:    An 8-bit code.
//   function:     An 8-bit code.
// Returns:
//   A raw uint64_t Panasonic message.
//
// Status: BETA / Should be working..
//
// Note:
//   Panasonic 48-bit protocol is a modified version of Kaseikyo.
// Ref:
//   http://www.remotecentral.com/cgi-bin/mboard/rc-pronto/thread.cgi?2615
uint64_t IRsend::encodePanasonic(uint16_t manufacturer, uint8_t device,
                                 uint8_t subdevice, uint8_t function) {
  uint8_t checksum = device ^ subdevice ^ function;
  return (((uint64_t)manufacturer << 32) | ((uint64_t)device << 24) |
          ((uint64_t)subdevice << 16) | ((uint64_t)function << 8) | checksum);
}
#endif  // (SEND_PANASONIC || SEND_DENON)

#if (DECODE_PANASONIC || DECODE_DENON)
// Decode the supplied Panasonic message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of data bits to expect.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: BETA / Should be working.
// Note:
//   Panasonic 48-bit protocol is a modified version of Kaseikyo.
// Ref:
//   http://www.remotecentral.com/cgi-bin/mboard/rc-pronto/thread.cgi?26152
//   http://www.hifi-remote.com/wiki/index.php?title=Panasonic
bool IRrecv::decodePanasonic(decode_results *results, uint16_t nbits,
                             bool strict, uint32_t manufacturer) {
  if (results->rawlen < 2 * nbits + kHeader + kFooter - 1)
    return false;  // Not enough entries to be a Panasonic message.
  if (strict && nbits != kPanasonicBits)
    return false;  // Request is out of spec.

  uint64_t data = 0;
  uint16_t offset = kStartOffset;

  // Header
  if (!matchMark(results->rawbuf[offset], kPanasonicHdrMark)) return false;
  // Calculate how long the common tick time is based on the header mark.
  uint32_t m_tick =
      results->rawbuf[offset++] * kRawTick / kPanasonicHdrMarkTicks;
  if (!matchSpace(results->rawbuf[offset], kPanasonicHdrSpace)) return false;
  // Calculate how long the common tick time is based on the header space.
  uint32_t s_tick =
      results->rawbuf[offset++] * kRawTick / kPanasonicHdrSpaceTicks;

  // Data
  match_result_t data_result = matchData(
      &(results->rawbuf[offset]), nbits, kPanasonicBitMarkTicks * m_tick,
      kPanasonicOneSpaceTicks * s_tick, kPanasonicBitMarkTicks * m_tick,
      kPanasonicZeroSpaceTicks * s_tick);
  if (data_result.success == false) return false;
  data = data_result.data;
  offset += data_result.used;

  // Footer
  if (!match(results->rawbuf[offset++], kPanasonicBitMarkTicks * m_tick))
    return false;
  if (offset < results->rawlen &&
      !matchAtLeast(results->rawbuf[offset], kPanasonicEndGap))
    return false;

  // Compliance
  uint32_t address = data >> 32;
  uint32_t command = data & 0xFFFFFFFF;
  if (strict) {
    if (address != manufacturer)  // Verify the Manufacturer code.
      return false;
    // Verify the checksum.
    uint8_t checksumOrig = data & 0xFF;
    uint8_t checksumCalc = ((data >> 24) ^ (data >> 16) ^ (data >> 8)) & 0xFF;
    if (checksumOrig != checksumCalc) return false;
  }

  // Success
  results->value = data;
  results->address = address;
  results->command = command;
  results->decode_type = PANASONIC;
  results->bits = nbits;
  return true;
}
#endif  // (DECODE_PANASONIC || DECODE_DENON)

#if SEND_PANASONIC_AC
// Send a Panasonic A/C message.
//
// Args:
//   data:   Contents of the message to be sent. (Guessing MSBF order)
//   nbits:  Nr. of bits of data to be sent. Typically kPanasonicAcBits.
//   repeat: Nr. of additional times the message is to be sent.
//
// Status: Beta / Appears to work with real device(s).
//:
// Panasonic A/C models supported:
//   A/C Series/models:
//     JKE, LKE, DKE, & NKE series.
//     CS-YW9MKD
//   A/C Remotes:
//     A75C3747
//     A75C3704
//
void IRsend::sendPanasonicAC(uint8_t data[], uint16_t nbytes, uint16_t repeat) {
  if (nbytes < kPanasonicAcSection1Length) return;
  for (uint16_t r = 0; r <= repeat; r++) {
    // First section. (8 bytes)
    sendGeneric(kPanasonicHdrMark, kPanasonicHdrSpace, kPanasonicBitMark,
                kPanasonicOneSpace, kPanasonicBitMark, kPanasonicZeroSpace,
                kPanasonicBitMark, kPanasonicAcSectionGap, data,
                kPanasonicAcSection1Length, kPanasonicFreq, false, 0, 50);
    // First section. (The rest of the data bytes)
    sendGeneric(kPanasonicHdrMark, kPanasonicHdrSpace, kPanasonicBitMark,
                kPanasonicOneSpace, kPanasonicBitMark, kPanasonicZeroSpace,
                kPanasonicBitMark, kPanasonicAcMessageGap,
                data + kPanasonicAcSection1Length,
                nbytes - kPanasonicAcSection1Length, kPanasonicFreq, false, 0,
                50);
  }
}
#endif  // SEND_PANASONIC_AC

IRPanasonicAc::IRPanasonicAc(uint16_t pin) : _irsend(pin) { stateReset(); }

void IRPanasonicAc::stateReset() {
  for (uint8_t i = 0; i < kPanasonicAcStateLength; i++)
    remote_state[i] = kPanasonicKnownGoodState[i];
  _temp = 25;  // An initial saved desired temp. Completely made up.
  _swingh = kPanasonicAcSwingHMiddle;  // A similar made up value for H Swing.
}

void IRPanasonicAc::begin() { _irsend.begin(); }

// Verify the checksum is valid for a given state.
// Args:
//   state:  The array to verify the checksum of.
//   length: The size of the state.
// Returns:
//   A boolean.
bool IRPanasonicAc::validChecksum(uint8_t state[], const uint16_t length) {
  if (length < 2) return false;  // 1 byte of data can't have a checksum.
  return (state[length - 1] ==
          sumBytes(state, length - 1, kPanasonicAcChecksumInit));
}

uint8_t IRPanasonicAc::calcChecksum(uint8_t state[], const uint16_t length) {
  return sumBytes(state, length - 1, kPanasonicAcChecksumInit);
}

void IRPanasonicAc::fixChecksum(const uint16_t length) {
  remote_state[length - 1] = calcChecksum(remote_state, length);
}

#if SEND_PANASONIC_AC
void IRPanasonicAc::send() {
  fixChecksum();
  _irsend.sendPanasonicAC(remote_state);
}
#endif  // SEND_PANASONIC_AC

void IRPanasonicAc::setModel(const panasonic_ac_remote_model_t model) {
  switch (model) {
    case kPanasonicDke:
    case kPanasonicJke:
    case kPanasonicLke:
    case kPanasonicNke:
    case kPanasonicCkp:
      break;
    default:  // Only proceed if we know what to do.
      return;
  }
  // clear & set the various bits and bytes.
  remote_state[13] &= 0xF0;
  remote_state[17] = 0x00;
  remote_state[21] &= 0b11101111;
  remote_state[23] = 0x81;
  remote_state[25] = 0x00;

  switch (model) {
    case kPanasonicLke:
      remote_state[13] |= 0x02;
      remote_state[17] = 0x06;
      break;
    case kPanasonicDke:
      remote_state[23] = 0x01;
      remote_state[25] = 0x06;
      // Has to be done last as setSwingHorizontal has model check built-in
      setSwingHorizontal(_swingh);
      break;
    case kPanasonicNke:
      remote_state[17] = 0x06;
      break;
    case kPanasonicJke:
      break;
    case kPanasonicCkp:
      remote_state[21] |= 0x10;
      remote_state[23] = 0x01;
    default:
      break;
  }
}

panasonic_ac_remote_model_t IRPanasonicAc::getModel() {
  if (remote_state[17] == 0x00) {
    if ((remote_state[21] & 0x10) && (remote_state[23] & 0x01))
      return kPanasonicCkp;
    if (remote_state[23] & 0x80) return kPanasonicJke;
  }
  if (remote_state[17] == 0x06 && (remote_state[13] & 0x0F) == 0x02)
    return kPanasonicLke;
  if (remote_state[23] == 0x01) return kPanasonicDke;
  if (remote_state[17] == 0x06) return kPanasonicNke;
  return kPanasonicUnknown;
}

uint8_t *IRPanasonicAc::getRaw() {
  fixChecksum();
  return remote_state;
}

void IRPanasonicAc::setRaw(const uint8_t state[]) {
  for (uint8_t i = 0; i < kPanasonicAcStateLength; i++) {
    remote_state[i] = state[i];
  }
}

// Control the power state of the A/C unit.
//
// For CKP models, the remote has no memory of the power state the A/C unit
// should be in. For those models setting this on/true will toggle the power
// state of the Panasonic A/C unit with the next meessage.
// e.g. If the A/C unit is already on, setPower(true) will turn it off.
//      If the A/C unit is already off, setPower(true) will turn it on.
//      setPower(false) will leave the A/C power state as it was.
//
// For all other models, setPower(true) should set the internal state to
// turn it on, and setPower(false) should turn it off.
void IRPanasonicAc::setPower(const bool state) {
  if (state)
    on();
  else
    off();
}

// Return the A/C power state of the remote.
// Except for CKP models, where it returns if the power state will be toggled
// on the A/C unit when the next message is sent.
bool IRPanasonicAc::getPower() {
  return (remote_state[13] & kPanasonicAcPower) == kPanasonicAcPower;
}

void IRPanasonicAc::on() { remote_state[13] |= kPanasonicAcPower; }

void IRPanasonicAc::off() { remote_state[13] &= ~kPanasonicAcPower; }

uint8_t IRPanasonicAc::getMode() { return remote_state[13] >> 4; }

void IRPanasonicAc::setMode(const uint8_t desired) {
  uint8_t mode = kPanasonicAcAuto;  // Default to Auto mode.
  switch (desired) {
    case kPanasonicAcFan:
      // Allegedly Fan mode has a temperature of 27.
      setTemp(kPanasonicAcFanModeTemp, false);
      mode = desired;
      break;
    case kPanasonicAcAuto:
    case kPanasonicAcCool:
    case kPanasonicAcHeat:
    case kPanasonicAcDry:
      mode = desired;
      // Set the temp to the saved temp, just incase our previous mode was Fan.
      setTemp(_temp);
      break;
  }
  remote_state[13] &= 0x0F;  // Clear the previous mode bits.
  remote_state[13] |= mode << 4;
}

uint8_t IRPanasonicAc::getTemp() { return remote_state[14] >> 1; }

// Set the desitred temperature in Celcius.
// Args:
//   celsius: The temperature to set the A/C unit to.
//   remember: A boolean flag for the class to remember the temperature.
//
// Automatically safely limits the temp to the operating range supported.
void IRPanasonicAc::setTemp(const uint8_t celsius, const bool remember) {
  uint8_t temperature;
  temperature = std::max(celsius, kPanasonicAcMinTemp);
  temperature = std::min(temperature, kPanasonicAcMaxTemp);
  remote_state[14] = temperature << 1;
  if (remember) _temp = temperature;
}

uint8_t IRPanasonicAc::getSwingVertical() { return remote_state[16] & 0x0F; }

void IRPanasonicAc::setSwingVertical(const uint8_t desired_elevation) {
  uint8_t elevation = desired_elevation;
  if (elevation != kPanasonicAcSwingVAuto) {
    elevation = std::max(elevation, kPanasonicAcSwingVUp);
    elevation = std::min(elevation, kPanasonicAcSwingVDown);
  }
  remote_state[16] &= 0xF0;
  remote_state[16] |= elevation;
}

uint8_t IRPanasonicAc::getSwingHorizontal() { return remote_state[17]; }

void IRPanasonicAc::setSwingHorizontal(const uint8_t desired_direction) {
  switch (desired_direction) {
    case kPanasonicAcSwingHAuto:
    case kPanasonicAcSwingHMiddle:
    case kPanasonicAcSwingHFullLeft:
    case kPanasonicAcSwingHLeft:
    case kPanasonicAcSwingHRight:
    case kPanasonicAcSwingHFullRight:
      break;
    default:  // Ignore anything that isn't valid.
      return;
  }
  _swingh = desired_direction;  // Store the direction for later.
  uint8_t direction = desired_direction;
  switch (getModel()) {
    case kPanasonicDke:
      break;
    case kPanasonicNke:
    case kPanasonicLke:
      direction = kPanasonicAcSwingHMiddle;
      break;
    default:  // Ignore everything else.
      return;
  }
  remote_state[17] = direction;
}

void IRPanasonicAc::setFan(const uint8_t speed) {
  if (speed <= kPanasonicAcFanMax || speed == kPanasonicAcFanAuto)
    remote_state[16] =
        (remote_state[16] & 0x0F) | ((speed + kPanasonicAcFanOffset) << 4);
}

uint8_t IRPanasonicAc::getFan() {
  return (remote_state[16] >> 4) - kPanasonicAcFanOffset;
}

bool IRPanasonicAc::getQuiet() {
  if (getModel() == kPanasonicCkp)
    return remote_state[21] & kPanasonicAcQuietCkp;
  else
    return remote_state[21] & kPanasonicAcQuiet;
}

void IRPanasonicAc::setQuiet(const bool state) {
  uint8_t quiet;
  if (getModel() == kPanasonicCkp)
    quiet = kPanasonicAcQuietCkp;
  else
    quiet = kPanasonicAcQuiet;

  if (state) {
    setPowerful(false);  // Powerful is mutually exclusive.
    remote_state[21] |= quiet;
  } else {
    remote_state[21] &= ~quiet;
  }
}

bool IRPanasonicAc::getPowerful() {
  if (getModel() == kPanasonicCkp)
    return remote_state[21] & kPanasonicAcPowerfulCkp;
  else
    return remote_state[21] & kPanasonicAcPowerful;
}

void IRPanasonicAc::setPowerful(const bool state) {
  uint8_t powerful;
  if (getModel() == kPanasonicCkp)
    powerful = kPanasonicAcPowerfulCkp;
  else
    powerful = kPanasonicAcPowerful;

  if (state) {
    setQuiet(false);  // Quiet is mutually exclusive.
    remote_state[21] |= powerful;
  } else {
    remote_state[21] &= ~powerful;
  }
}

uint16_t IRPanasonicAc::encodeTime(const uint8_t hours, const uint8_t mins) {
  return std::min(hours, (uint8_t)23) * 60 + std::min(mins, (uint8_t)59);
}

uint16_t IRPanasonicAc::getClock() {
  uint16_t result = ((remote_state[25] & 0b00000111) << 8) + remote_state[24];
  if (result == kPanasonicAcTimeSpecial) return 0;
  return result;
}

void IRPanasonicAc::setClock(const uint16_t mins_since_midnight) {
  uint16_t corrected = std::min(mins_since_midnight, kPanasonicAcTimeMax);
  if (mins_since_midnight == kPanasonicAcTimeSpecial)
    corrected = kPanasonicAcTimeSpecial;
  remote_state[24] = corrected & 0xFF;
  remote_state[25] &= 0b11111000;
  remote_state[25] |= (corrected >> 8);
}

uint16_t IRPanasonicAc::getOnTimer() {
  uint16_t result = ((remote_state[19] & 0b00000111) << 8) + remote_state[18];
  if (result == kPanasonicAcTimeSpecial) return 0;
  return result;
}

void IRPanasonicAc::setOnTimer(const uint16_t mins_since_midnight,
                               const bool enable) {
  // Ensure it's on a 10 minute boundary and no overflow.
  uint16_t corrected = std::min(mins_since_midnight, kPanasonicAcTimeMax);
  corrected -= corrected % 10;
  if (mins_since_midnight == kPanasonicAcTimeSpecial)
    corrected = kPanasonicAcTimeSpecial;

  if (enable)
    remote_state[13] |= kPanasonicAcOnTimer;  // Set the Ontimer flag.
  else
    remote_state[13] &= ~kPanasonicAcOnTimer;  // Clear the Ontimer flag.
  // Store the time.
  remote_state[18] = corrected & 0xFF;
  remote_state[19] &= 0b11111000;
  remote_state[19] |= (corrected >> 8);
}

void IRPanasonicAc::cancelOnTimer() { setOnTimer(0, false); }

bool IRPanasonicAc::isOnTimerEnabled() {
  return remote_state[13] & kPanasonicAcOnTimer;
}

uint16_t IRPanasonicAc::getOffTimer() {
  uint16_t result =
      ((remote_state[20] & 0b01111111) << 4) + (remote_state[19] >> 4);
  if (result == kPanasonicAcTimeSpecial) return 0;
  return result;
}

void IRPanasonicAc::setOffTimer(const uint16_t mins_since_midnight,
                                const bool enable) {
  // Ensure its on a 10 minute boundary and no overflow.
  uint16_t corrected = std::min(mins_since_midnight, kPanasonicAcTimeMax);
  corrected -= corrected % 10;
  if (mins_since_midnight == kPanasonicAcTimeSpecial)
    corrected = kPanasonicAcTimeSpecial;

  if (enable)
    remote_state[13] |= kPanasonicAcOffTimer;  // Set the OffTimer flag.
  else
    remote_state[13] &= ~kPanasonicAcOffTimer;  // Clear the OffTimer flag.
  // Store the time.
  remote_state[19] &= 0b00001111;
  remote_state[19] |= (corrected & 0b00001111) << 4;
  remote_state[20] &= 0b10000000;
  remote_state[20] |= corrected >> 4;
}

void IRPanasonicAc::cancelOffTimer() { setOffTimer(0, false); }

bool IRPanasonicAc::isOffTimerEnabled() {
  return remote_state[13] & kPanasonicAcOffTimer;
}

#ifdef ARDUINO
String IRPanasonicAc::timeToString(const uint16_t mins_since_midnight) {
  String result = "";
#else
std::string IRPanasonicAc::timeToString(const uint16_t mins_since_midnight) {
  std::string result = "";
#endif  // ARDUINO
  result += uint64ToString(mins_since_midnight / 60) + ":";
  uint8_t mins = mins_since_midnight % 60;
  if (mins < 10) result += "0";  // Zero pad the minutes.
  return result + uint64ToString(mins);
}

// Convert the internal state into a human readable string.
#ifdef ARDUINO
String IRPanasonicAc::toString() {
  String result = "";
#else
std::string IRPanasonicAc::toString() {
  std::string result = "";
#endif  // ARDUINO
  result += "Model: " + uint64ToString(getModel());
  switch (getModel()) {
    case kPanasonicDke:
      result += " (DKE)";
      break;
    case kPanasonicJke:
      result += " (JKE)";
      break;
    case kPanasonicNke:
      result += " (NKE)";
      break;
    case kPanasonicLke:
      result += " (LKE)";
      break;
    case kPanasonicCkp:
      result += " (CKP)";
      break;
    default:
      result += " (UNKNOWN)";
  }
  result += ", Power: ";
  if (getPower())
    result += "On";
  else
    result += "Off";
  result += ", Mode: " + uint64ToString(getMode());
  switch (getMode()) {
    case kPanasonicAcAuto:
      result += " (AUTO)";
      break;
    case kPanasonicAcCool:
      result += " (COOL)";
      break;
    case kPanasonicAcHeat:
      result += " (HEAT)";
      break;
    case kPanasonicAcDry:
      result += " (DRY)";
      break;
    case kPanasonicAcFan:
      result += " (FAN)";
      break;
    default:
      result += " (UNKNOWN)";
  }
  result += ", Temp: " + uint64ToString(getTemp()) + "C";
  result += ", Fan: " + uint64ToString(getFan());
  switch (getFan()) {
    case kPanasonicAcFanAuto:
      result += " (AUTO)";
      break;
    case kPanasonicAcFanMax:
      result += " (MAX)";
      break;
    case kPanasonicAcFanMin:
      result += " (MIN)";
      break;
    default:
      result += " (UNKNOWN)";
      break;
  }
  result += ", Swing (Vertical): " + uint64ToString(getSwingVertical());
  switch (getSwingVertical()) {
    case kPanasonicAcSwingVAuto:
      result += " (AUTO)";
      break;
    case kPanasonicAcSwingVUp:
      result += " (Full Up)";
      break;
    case kPanasonicAcSwingVDown:
      result += " (Full Down)";
      break;
    case 2:
    case 3:
    case 4:
      break;
    default:
      result += " (UNKNOWN)";
      break;
  }
  switch (getModel()) {
    case kPanasonicJke:
    case kPanasonicCkp:
      break;  // No Horizontal Swing support.
    default:
      result += ", Swing (Horizontal): " + uint64ToString(getSwingHorizontal());
      switch (getSwingHorizontal()) {
        case kPanasonicAcSwingHAuto:
          result += " (AUTO)";
          break;
        case kPanasonicAcSwingHFullLeft:
          result += " (Full Left)";
          break;
        case kPanasonicAcSwingHLeft:
          result += " (Left)";
          break;
        case kPanasonicAcSwingHMiddle:
          result += " (Middle)";
          break;
        case kPanasonicAcSwingHFullRight:
          result += " (Full Right)";
          break;
        case kPanasonicAcSwingHRight:
          result += " (Right)";
          break;
        default:
          result += " (UNKNOWN)";
          break;
      }
  }
  result += ", Quiet: ";
  if (getQuiet())
    result += "On";
  else
    result += "Off";
  result += ", Powerful: ";
  if (getPowerful())
    result += "On";
  else
    result += "Off";
  result += ", Clock: " + timeToString(getClock());
  result += ", On Timer: ";
  if (isOnTimerEnabled())
    result += timeToString(getOnTimer());
  else
    result += "Off";
  result += ", Off Timer: ";
  if (isOffTimerEnabled())
    result += timeToString(getOffTimer());
  else
    result += "Off";
  return result;
}

#if DECODE_PANASONIC_AC
// Decode the supplied Panasonic AC message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect. Typically kPanasonicAcBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: Beta / Appears to work with real device(s).
//
// Panasonic A/C models supported:
//   A/C Series/models:
//     JKE, LKE, DKE, & NKE series.
//     CS-YW9MKD
//   A/C Remotes:
//     A75C3747 (Confirmed)
//     A75C3704
bool IRrecv::decodePanasonicAC(decode_results *results, uint16_t nbits,
                               bool strict) {
  if (nbits % 8 != 0)  // nbits has to be a multiple of nr. of bits in a byte.
    return false;

  uint8_t min_nr_of_messages = 1;
  if (strict) {
    if (nbits != kPanasonicAcBits && nbits != kPanasonicAcShortBits)
      return false;  // Not strictly a PANASONIC_AC message.
  }

  if (results->rawlen <
      min_nr_of_messages * (2 * nbits + kHeader + kFooter) - 1)
    return false;  // Can't possibly be a valid PANASONIC_AC message.

  uint16_t dataBitsSoFar = 0;
  uint16_t offset = kStartOffset;
  match_result_t data_result;

  // Header
  if (!matchMark(results->rawbuf[offset], kPanasonicHdrMark,
                 kPanasonicAcTolerance, kPanasonicAcExcess))
    return false;
  // Calculate how long the common tick time is based on the header mark.
  uint32_t m_tick =
      results->rawbuf[offset++] * kRawTick / kPanasonicHdrMarkTicks;
  if (!matchSpace(results->rawbuf[offset], kPanasonicHdrSpace,
                  kPanasonicAcTolerance, kPanasonicAcExcess))
    return false;
  // Calculate how long the common tick time is based on the header space.
  uint32_t s_tick =
      results->rawbuf[offset++] * kRawTick / kPanasonicHdrSpaceTicks;

  uint16_t i = 0;
  // Data (Section #1)
  // Keep reading bytes until we either run out of section or state to fill.
  for (; offset <= results->rawlen - 16 && i < kPanasonicAcSection1Length;
       i++, dataBitsSoFar += 8, offset += data_result.used) {
    data_result = matchData(
        &(results->rawbuf[offset]), 8, kPanasonicBitMarkTicks * m_tick,
        kPanasonicOneSpaceTicks * s_tick, kPanasonicBitMarkTicks * m_tick,
        kPanasonicZeroSpaceTicks * s_tick, kPanasonicAcTolerance,
        kPanasonicAcExcess, false);
    if (data_result.success == false) {
      DPRINT("DEBUG: offset = ");
      DPRINTLN(offset + data_result.used);
      return false;  // Fail
    }
    results->state[i] = data_result.data;
  }
  // Section footer.
  if (!matchMark(results->rawbuf[offset++], kPanasonicBitMarkTicks * m_tick,
                 kPanasonicAcTolerance, kPanasonicAcExcess))
    return false;
  if (!matchSpace(results->rawbuf[offset++], kPanasonicAcSectionGap,
                  kPanasonicAcTolerance, kPanasonicAcExcess))
    return false;
  // Header.
  if (!matchMark(results->rawbuf[offset++], kPanasonicHdrMarkTicks * m_tick,
                 kPanasonicAcTolerance, kPanasonicAcExcess))
    return false;
  if (!matchSpace(results->rawbuf[offset++], kPanasonicHdrSpaceTicks * s_tick,
                  kPanasonicAcTolerance, kPanasonicAcExcess))
    return false;
  // Data (Section #2)
  // Keep reading bytes until we either run out of data.
  for (; offset <= results->rawlen - 16 && i < nbits / 8;
       i++, dataBitsSoFar += 8, offset += data_result.used) {
    data_result = matchData(
        &(results->rawbuf[offset]), 8, kPanasonicBitMarkTicks * m_tick,
        kPanasonicOneSpaceTicks * s_tick, kPanasonicBitMarkTicks * m_tick,
        kPanasonicZeroSpaceTicks * s_tick, kPanasonicAcTolerance,
        kPanasonicAcExcess, false);
    if (data_result.success == false) {
      DPRINT("DEBUG: offset = ");
      DPRINTLN(offset + data_result.used);
      return false;  // Fail
    }
    results->state[i] = data_result.data;
  }
  // Message Footer.
  if (!matchMark(results->rawbuf[offset++], kPanasonicBitMarkTicks * m_tick,
                 kPanasonicAcTolerance, kPanasonicAcExcess))
    return false;
  if (offset <= results->rawlen &&
      !matchAtLeast(results->rawbuf[offset++], kPanasonicAcMessageGap))
    return false;

  // Compliance
  if (strict) {
    // Check the signatures of the section blocks. They start with 0x02& 0x20.
    if (results->state[0] != 0x02 || results->state[1] != 0x20 ||
        results->state[8] != 0x02 || results->state[9] != 0x20)
      return false;
    if (!IRPanasonicAc::validChecksum(results->state, nbits / 8)) return false;
  }

  // Success
  results->decode_type = PANASONIC_AC;
  results->bits = nbits;
  return true;
}
#endif  // DECODE_PANASONIC_AC
