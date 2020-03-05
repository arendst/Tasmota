// Copyright 2015 Kristian Lauszus
// Copyright 2017, 2018 David Conran

// Panasonic devices

#include "ir_Panasonic.h"
#include <algorithm>
#include <cstring>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

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
//     JKE, LKE, DKE, CKP, PKR, RKR, & NKE series. (In theory)
//     CS-YW9MKD, CS-Z9RKR, CS-E7PKR (confirmed)
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
const uint32_t kPanasonicAcMessageGap = kDefaultMessageGap;  // Just a guess.

using irutils::addBoolToString;
using irutils::addFanToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addModelToString;
using irutils::addTempToString;
using irutils::minsToString;
using irutils::setBit;
using irutils::setBits;

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
void IRsend::sendPanasonic64(const uint64_t data, const uint16_t nbits,
                             const uint16_t repeat) {
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
void IRsend::sendPanasonic(const uint16_t address, const uint32_t data,
                           const uint16_t nbits, const uint16_t repeat) {
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
uint64_t IRsend::encodePanasonic(const uint16_t manufacturer,
                                 const uint8_t device,
                                 const uint8_t subdevice,
                                 const uint8_t function) {
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
//   offset:  The starting index to use when attempting to decode the raw data.
//            Typically/Defaults to kStartOffset.
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
bool IRrecv::decodePanasonic(decode_results *results, uint16_t offset,
                             const uint16_t nbits, const bool strict,
                             const uint32_t manufacturer) {
  if (strict && nbits != kPanasonicBits)
    return false;  // Request is out of spec.

  uint64_t data = 0;

  // Match Header + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, &data,
                    results->rawlen - offset, nbits,
                    kPanasonicHdrMark, kPanasonicHdrSpace,
                    kPanasonicBitMark, kPanasonicOneSpace,
                    kPanasonicBitMark, kPanasonicZeroSpace,
                    kPanasonicBitMark, kPanasonicEndGap, true)) return false;
  // Compliance
  uint32_t address = data >> 32;
  uint32_t command = data;
  if (strict) {
    if (address != manufacturer)  // Verify the Manufacturer code.
      return false;
    // Verify the checksum.
    uint8_t checksumOrig = data;
    uint8_t checksumCalc = (data >> 24) ^ (data >> 16) ^ (data >> 8);
    if (checksumOrig != checksumCalc) return false;
  }

  // Success
  results->value = data;
  results->address = address;
  results->command = command;
  results->decode_type = decode_type_t::PANASONIC;
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
//     JKE, LKE, DKE, CKP, PKR, RKR, & NKE series.
//     CS-YW9MKD
//     CS-E7PKR
//   A/C Remotes:
//     A75C3747
//     A75C3704
//
void IRsend::sendPanasonicAC(const uint8_t data[], const uint16_t nbytes,
                             const uint16_t repeat) {
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

IRPanasonicAc::IRPanasonicAc(const uint16_t pin, const bool inverted,
                             const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { this->stateReset(); }

void IRPanasonicAc::stateReset(void) {
  memcpy(remote_state, kPanasonicKnownGoodState, kPanasonicAcStateLength);
  _temp = 25;  // An initial saved desired temp. Completely made up.
  _swingh = kPanasonicAcSwingHMiddle;  // A similar made up value for H Swing.
}

void IRPanasonicAc::begin(void) { _irsend.begin(); }

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
  remote_state[length - 1] = this->calcChecksum(remote_state, length);
}

#if SEND_PANASONIC_AC
void IRPanasonicAc::send(const uint16_t repeat) {
  _irsend.sendPanasonicAC(getRaw(), kPanasonicAcStateLength, repeat);
}
#endif  // SEND_PANASONIC_AC

void IRPanasonicAc::setModel(const panasonic_ac_remote_model_t model) {
  switch (model) {
    case panasonic_ac_remote_model_t::kPanasonicDke:
    case panasonic_ac_remote_model_t::kPanasonicJke:
    case panasonic_ac_remote_model_t::kPanasonicLke:
    case panasonic_ac_remote_model_t::kPanasonicNke:
    case panasonic_ac_remote_model_t::kPanasonicCkp:
    case panasonic_ac_remote_model_t::kPanasonicRkr: break;
    // Only proceed if we know what to do.
    default: return;
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
      this->setSwingHorizontal(_swingh);
      break;
    case kPanasonicNke:
      remote_state[17] = 0x06;
      break;
    case kPanasonicJke:
      break;
    case kPanasonicCkp:
      remote_state[21] |= 0x10;
      remote_state[23] = 0x01;
      break;
    case kPanasonicRkr:
      remote_state[13] |= 0x08;
      remote_state[23] = 0x89;
    default:
      break;
  }
  // Reset the Ion filter.
  setIon(getIon());
}

panasonic_ac_remote_model_t IRPanasonicAc::getModel(void) {
  if (remote_state[23] == 0x89) return kPanasonicRkr;
  if (remote_state[17] == 0x00) {
    if ((remote_state[21] & 0x10) && (remote_state[23] & 0x01))
      return panasonic_ac_remote_model_t::kPanasonicCkp;
    if (remote_state[23] & 0x80)
      return panasonic_ac_remote_model_t::kPanasonicJke;
  }
  if (remote_state[17] == 0x06 && (remote_state[13] & 0x0F) == 0x02)
    return panasonic_ac_remote_model_t::kPanasonicLke;
  if (remote_state[23] == 0x01)
    return panasonic_ac_remote_model_t::kPanasonicDke;
  if (remote_state[17] == 0x06)
    return panasonic_ac_remote_model_t::kPanasonicNke;
  return panasonic_ac_remote_model_t::kPanasonicUnknown;  // Default
}

uint8_t *IRPanasonicAc::getRaw(void) {
  this->fixChecksum();
  return remote_state;
}

void IRPanasonicAc::setRaw(const uint8_t state[]) {
  memcpy(remote_state, state, kPanasonicAcStateLength);
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
void IRPanasonicAc::setPower(const bool on) {
  setBit(&remote_state[13], kPanasonicAcPowerOffset, on);
}

// Return the A/C power state of the remote.
// Except for CKP models, where it returns if the power state will be toggled
// on the A/C unit when the next message is sent.
bool IRPanasonicAc::getPower(void) {
  return GETBIT8(remote_state[13], kPanasonicAcPowerOffset);
}

void IRPanasonicAc::on(void) { setPower(true); }

void IRPanasonicAc::off(void) { setPower(false); }

uint8_t IRPanasonicAc::getMode(void) {
  return GETBITS8(remote_state[13], kHighNibble, kModeBitsSize);
}

void IRPanasonicAc::setMode(const uint8_t desired) {
  uint8_t mode = kPanasonicAcAuto;  // Default to Auto mode.
  switch (desired) {
    case kPanasonicAcFan:
      // Allegedly Fan mode has a temperature of 27.
      this->setTemp(kPanasonicAcFanModeTemp, false);
      mode = desired;
      break;
    case kPanasonicAcAuto:
    case kPanasonicAcCool:
    case kPanasonicAcHeat:
    case kPanasonicAcDry:
      mode = desired;
      // Set the temp to the saved temp, just incase our previous mode was Fan.
      this->setTemp(_temp);
      break;
  }
  remote_state[13] &= 0x0F;  // Clear the previous mode bits.
  setBits(&remote_state[13], kHighNibble, kModeBitsSize, mode);
}

uint8_t IRPanasonicAc::getTemp(void) {
  return GETBITS8(remote_state[14], kPanasonicAcTempOffset,
                  kPanasonicAcTempSize);
}

// Set the desitred temperature in Celsius.
// Args:
//   celsius: The temperature to set the A/C unit to.
//   remember: A boolean flag for the class to remember the temperature.
//
// Automatically safely limits the temp to the operating range supported.
void IRPanasonicAc::setTemp(const uint8_t celsius, const bool remember) {
  uint8_t temperature;
  temperature = std::max(celsius, kPanasonicAcMinTemp);
  temperature = std::min(temperature, kPanasonicAcMaxTemp);
  if (remember) _temp = temperature;
  setBits(&remote_state[14], kPanasonicAcTempOffset, kPanasonicAcTempSize,
          temperature);
}

uint8_t IRPanasonicAc::getSwingVertical(void) {
  return GETBITS8(remote_state[16], kLowNibble, kNibbleSize);
}

void IRPanasonicAc::setSwingVertical(const uint8_t desired_elevation) {
  uint8_t elevation = desired_elevation;
  if (elevation != kPanasonicAcSwingVAuto) {
    elevation = std::max(elevation, kPanasonicAcSwingVHighest);
    elevation = std::min(elevation, kPanasonicAcSwingVLowest);
  }
  setBits(&remote_state[16], kLowNibble, kNibbleSize, elevation);
}

uint8_t IRPanasonicAc::getSwingHorizontal(void) {
  return GETBITS8(remote_state[17], kLowNibble, kNibbleSize);
}

void IRPanasonicAc::setSwingHorizontal(const uint8_t desired_direction) {
  switch (desired_direction) {
    case kPanasonicAcSwingHAuto:
    case kPanasonicAcSwingHMiddle:
    case kPanasonicAcSwingHFullLeft:
    case kPanasonicAcSwingHLeft:
    case kPanasonicAcSwingHRight:
    case kPanasonicAcSwingHFullRight: break;
    // Ignore anything that isn't valid.
    default: return;
  }
  _swingh = desired_direction;  // Store the direction for later.
  uint8_t direction = desired_direction;
  switch (this->getModel()) {
    case kPanasonicDke:
    case kPanasonicRkr:
      break;
    case kPanasonicNke:
    case kPanasonicLke:
      direction = kPanasonicAcSwingHMiddle;
      break;
    default:  // Ignore everything else.
      return;
  }
  setBits(&remote_state[17], kLowNibble, kNibbleSize, direction);
}

void IRPanasonicAc::setFan(const uint8_t speed) {
  switch (speed) {
    case kPanasonicAcFanMin:
    case kPanasonicAcFanMed:
    case kPanasonicAcFanMax:
    case kPanasonicAcFanAuto:
      setBits(&remote_state[16], kHighNibble, kNibbleSize,
              speed + kPanasonicAcFanDelta);
      break;
    default: setFan(kPanasonicAcFanAuto);
  }
}

uint8_t IRPanasonicAc::getFan(void) {
  return GETBITS8(remote_state[16], kHighNibble, kNibbleSize) -
      kPanasonicAcFanDelta;
}

bool IRPanasonicAc::getQuiet(void) {
  switch (this->getModel()) {
    case kPanasonicRkr:
    case kPanasonicCkp:
      return GETBIT8(remote_state[21], kPanasonicAcQuietCkpOffset);
    default:
      return GETBIT8(remote_state[21], kPanasonicAcQuietOffset);
  }
}

void IRPanasonicAc::setQuiet(const bool on) {
  uint8_t offset;
  switch (this->getModel()) {
    case kPanasonicRkr:
    case kPanasonicCkp: offset = kPanasonicAcQuietCkpOffset; break;
    default:            offset = kPanasonicAcQuietOffset;
  }
  if (on) this->setPowerful(false);  // Powerful is mutually exclusive.
  setBit(&remote_state[21], offset, on);
}

bool IRPanasonicAc::getPowerful(void) {
  switch (this->getModel()) {
    case kPanasonicRkr:
    case kPanasonicCkp:
      return GETBIT8(remote_state[21], kPanasonicAcPowerfulCkpOffset);
    default:
      return GETBIT8(remote_state[21], kPanasonicAcPowerfulOffset);
  }
}

void IRPanasonicAc::setPowerful(const bool on) {
  uint8_t offset;
  switch (this->getModel()) {
    case kPanasonicRkr:
    case kPanasonicCkp: offset = kPanasonicAcPowerfulCkpOffset; break;
    default:            offset = kPanasonicAcPowerfulOffset;
  }

  if (on) this->setQuiet(false);  // Quiet is mutually exclusive.
  setBit(&remote_state[21], offset, on);
}

// Convert standard (military/24hr) time to nr. of minutes since midnight.
uint16_t IRPanasonicAc::encodeTime(const uint8_t hours, const uint8_t mins) {
  return std::min(hours, (uint8_t)23) * 60 + std::min(mins, (uint8_t)59);
}

uint16_t IRPanasonicAc::_getTime(const uint8_t ptr[]) {
  uint16_t result = (GETBITS8(
      ptr[1], kLowNibble, kPanasonicAcTimeOverflowSize) <<
      (kPanasonicAcTimeSize - kPanasonicAcTimeOverflowSize)) + ptr[0];
  if (result == kPanasonicAcTimeSpecial) return 0;
  return result;
}

uint16_t IRPanasonicAc::getClock(void) { return _getTime(&remote_state[24]); }

void IRPanasonicAc::_setTime(uint8_t * const ptr,
                             const uint16_t mins_since_midnight,
                             const bool round_down) {
  uint16_t corrected = std::min(mins_since_midnight, kPanasonicAcTimeMax);
  if (round_down) corrected -= corrected % 10;
  if (mins_since_midnight == kPanasonicAcTimeSpecial)
    corrected = kPanasonicAcTimeSpecial;
  ptr[0] = corrected;
  setBits(&ptr[1], kLowNibble, kPanasonicAcTimeOverflowSize,
          corrected >> (kPanasonicAcTimeSize - kPanasonicAcTimeOverflowSize));
}

void IRPanasonicAc::setClock(const uint16_t mins_since_midnight) {
  _setTime(&remote_state[24], mins_since_midnight, false);
}

uint16_t IRPanasonicAc::getOnTimer(void) { return _getTime(&remote_state[18]); }

void IRPanasonicAc::setOnTimer(const uint16_t mins_since_midnight,
                               const bool enable) {
  // Set the timer flag.
  setBit(&remote_state[13], kPanasonicAcOnTimerOffset, enable);
  // Store the time.
  _setTime(&remote_state[18], mins_since_midnight, true);
}

void IRPanasonicAc::cancelOnTimer(void) { this->setOnTimer(0, false); }

bool IRPanasonicAc::isOnTimerEnabled(void) {
  return GETBIT8(remote_state[13], kPanasonicAcOnTimerOffset);
}

uint16_t IRPanasonicAc::getOffTimer(void) {
  uint16_t result = (GETBITS8(remote_state[20], 0, 7) << kNibbleSize) |
      GETBITS8(remote_state[19], kHighNibble, kNibbleSize);
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
  // Set the timer flag.
  setBit(&remote_state[13], kPanasonicAcOffTimerOffset, enable);
  // Store the time.
  setBits(&remote_state[19], kHighNibble, kNibbleSize, corrected);
  setBits(&remote_state[20], 0, 7, corrected >> kNibbleSize);
}

void IRPanasonicAc::cancelOffTimer(void) { this->setOffTimer(0, false); }

bool IRPanasonicAc::isOffTimerEnabled(void) {
  return GETBIT8(remote_state[13], kPanasonicAcOffTimerOffset);
}

bool IRPanasonicAc::getIon(void) {
  switch (this->getModel()) {
    case kPanasonicDke:
      return GETBIT8(remote_state[kPanasonicAcIonFilterByte],
                     kPanasonicAcIonFilterOffset);
    default:
      return false;
  }
}

void IRPanasonicAc::setIon(const bool on) {
  if (this->getModel() == kPanasonicDke)
    setBit(&remote_state[kPanasonicAcIonFilterByte],
           kPanasonicAcIonFilterOffset, on);
}

// Convert a standard A/C mode into its native mode.
uint8_t IRPanasonicAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kPanasonicAcCool;
    case stdAc::opmode_t::kHeat: return kPanasonicAcHeat;
    case stdAc::opmode_t::kDry:  return kPanasonicAcDry;
    case stdAc::opmode_t::kFan:  return kPanasonicAcFan;
    default:                     return kPanasonicAcAuto;
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRPanasonicAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:    return kPanasonicAcFanMin;
    case stdAc::fanspeed_t::kLow:    return kPanasonicAcFanMin + 1;
    case stdAc::fanspeed_t::kMedium: return kPanasonicAcFanMin + 2;
    case stdAc::fanspeed_t::kHigh:   return kPanasonicAcFanMin + 3;
    case stdAc::fanspeed_t::kMax:    return kPanasonicAcFanMax;
    default:                         return kPanasonicAcFanAuto;
  }
}

// Convert a standard A/C vertical swing into its native setting.
uint8_t IRPanasonicAc::convertSwingV(const stdAc::swingv_t position) {
  switch (position) {
    case stdAc::swingv_t::kHighest:
    case stdAc::swingv_t::kHigh:
    case stdAc::swingv_t::kMiddle:
    case stdAc::swingv_t::kLow:
    case stdAc::swingv_t::kLowest: return (uint8_t)position;
    default:                       return kPanasonicAcSwingVAuto;
  }
}

// Convert a standard A/C horizontal swing into its native setting.
uint8_t IRPanasonicAc::convertSwingH(const stdAc::swingh_t position) {
  switch (position) {
    case stdAc::swingh_t::kLeftMax:  return kPanasonicAcSwingHFullLeft;
    case stdAc::swingh_t::kLeft:     return kPanasonicAcSwingHLeft;
    case stdAc::swingh_t::kMiddle:   return kPanasonicAcSwingHMiddle;
    case stdAc::swingh_t::kRight:    return kPanasonicAcSwingHRight;
    case stdAc::swingh_t::kRightMax: return kPanasonicAcSwingHFullRight;
    default:                         return kPanasonicAcSwingHAuto;
  }
}

// Convert a native mode to it's common equivalent.
stdAc::opmode_t IRPanasonicAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kPanasonicAcCool: return stdAc::opmode_t::kCool;
    case kPanasonicAcHeat: return stdAc::opmode_t::kHeat;
    case kPanasonicAcDry:  return stdAc::opmode_t::kDry;
    case kPanasonicAcFan:  return stdAc::opmode_t::kFan;
    default:               return stdAc::opmode_t::kAuto;
  }
}

// Convert a native fan speed to it's common equivalent.
stdAc::fanspeed_t IRPanasonicAc::toCommonFanSpeed(const uint8_t spd) {
  switch (spd) {
    case kPanasonicAcFanMax:     return stdAc::fanspeed_t::kMax;
    case kPanasonicAcFanMin + 3: return stdAc::fanspeed_t::kHigh;
    case kPanasonicAcFanMin + 2: return stdAc::fanspeed_t::kMedium;
    case kPanasonicAcFanMin + 1: return stdAc::fanspeed_t::kLow;
    case kPanasonicAcFanMin:     return stdAc::fanspeed_t::kMin;
    default:                     return stdAc::fanspeed_t::kAuto;
  }
}

// Convert a native vertical swing to it's common equivalent.
stdAc::swingh_t IRPanasonicAc::toCommonSwingH(const uint8_t pos) {
  switch (pos) {
    case kPanasonicAcSwingHFullLeft:  return stdAc::swingh_t::kLeftMax;
    case kPanasonicAcSwingHLeft:      return stdAc::swingh_t::kLeft;
    case kPanasonicAcSwingHMiddle:    return stdAc::swingh_t::kMiddle;
    case kPanasonicAcSwingHRight:     return stdAc::swingh_t::kRight;
    case kPanasonicAcSwingHFullRight: return stdAc::swingh_t::kRightMax;
    default:                          return stdAc::swingh_t::kAuto;
  }
}

// Convert a native vertical swing to it's common equivalent.
stdAc::swingv_t IRPanasonicAc::toCommonSwingV(const uint8_t pos) {
  if (pos >= kPanasonicAcSwingVHighest && pos <= kPanasonicAcSwingVLowest)
    return (stdAc::swingv_t)pos;
  else
    return stdAc::swingv_t::kAuto;
}

// Convert the A/C state to it's common equivalent.
stdAc::state_t IRPanasonicAc::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::PANASONIC_AC;
  result.model = this->getModel();
  result.power = this->getPower();
  result.mode = this->toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = this->toCommonFanSpeed(this->getFan());
  result.swingv = this->toCommonSwingV(this->getSwingVertical());
  result.swingh = this->toCommonSwingH(this->getSwingHorizontal());
  result.quiet = this->getQuiet();
  result.turbo = this->getPowerful();
  result.filter = this->getIon();
  // Not supported.
  result.econo = false;
  result.clean = false;
  result.light = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

// Convert the internal state into a human readable string.
String IRPanasonicAc::toString(void) {
  String result = "";
  result.reserve(180);  // Reserve some heap for the string to reduce fragging.
  result += addModelToString(decode_type_t::PANASONIC_AC, getModel(), false);
  result += addBoolToString(getPower(), kPowerStr);
  result += addModeToString(getMode(), kPanasonicAcAuto, kPanasonicAcCool,
                            kPanasonicAcHeat, kPanasonicAcDry, kPanasonicAcFan);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kPanasonicAcFanMax, kPanasonicAcFanMin,
                           kPanasonicAcFanAuto, kPanasonicAcFanAuto,
                           kPanasonicAcFanMed);
  result += addIntToString(getSwingVertical(), kSwingVStr);
  result += kSpaceLBraceStr;
  switch (getSwingVertical()) {
    case kPanasonicAcSwingVAuto:
      result += kAutoStr;
      break;
    case kPanasonicAcSwingVHighest:
      result += kHighestStr;
      break;
    case kPanasonicAcSwingVHigh:
      result += kHighStr;
      break;
    case kPanasonicAcSwingVMiddle:
      result += kMiddleStr;
      break;
    case kPanasonicAcSwingVLow:
      result += kLowStr;
      break;
    case kPanasonicAcSwingVLowest:
      result += kLowestStr;
      break;
    default:
      result += kUnknownStr;
      break;
  }
  result += ')';
  switch (getModel()) {
    case kPanasonicJke:
    case kPanasonicCkp:
      break;  // No Horizontal Swing support.
    default:
      result += addIntToString(getSwingHorizontal(), kSwingHStr);
      result += kSpaceLBraceStr;
      switch (getSwingHorizontal()) {
        case kPanasonicAcSwingHAuto:
          result += kAutoStr;
          break;
        case kPanasonicAcSwingHFullLeft:
          result += kMaxLeftStr;
          break;
        case kPanasonicAcSwingHLeft:
          result += kLeftStr;
          break;
        case kPanasonicAcSwingHMiddle:
          result += kMiddleStr;
          break;
        case kPanasonicAcSwingHFullRight:
          result += kMaxRightStr;
          break;
        case kPanasonicAcSwingHRight:
          result += kRightStr;
          break;
        default:
          result += kUnknownStr;
      }
      result += ')';
  }
  result += addBoolToString(getQuiet(), kQuietStr);
  result += addBoolToString(getPowerful(), kPowerfulStr);
  if (getModel() == kPanasonicDke)
    result += addBoolToString(getIon(), kIonStr);
  result += addLabeledString(minsToString(getClock()), kClockStr);
  result += addLabeledString(
      isOnTimerEnabled() ? minsToString(getOnTimer()) : kOffStr,
      kOnTimerStr);
  result += addLabeledString(
      isOffTimerEnabled() ? minsToString(getOffTimer()) : kOffStr,
      kOffTimerStr);
  return result;
}

#if DECODE_PANASONIC_AC
// Decode the supplied Panasonic AC message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   offset:  The starting index to use when attempting to decode the raw data.
//            Typically/Defaults to kStartOffset.
//   nbits:   The number of data bits to expect. Typically kPanasonicAcBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: Beta / Appears to work with real device(s).
//
// Panasonic A/C models supported:
//   A/C Series/models:
//     JKE, LKE, DKE, PKR, & NKE series.
//     CS-YW9MKD
//     CS-E7PKR
//   A/C Remotes:
//     A75C3747 (Confirmed)
//     A75C3704
bool IRrecv::decodePanasonicAC(decode_results *results, uint16_t offset,
                               const uint16_t nbits, const bool strict) {
  uint8_t min_nr_of_messages = 1;
  if (strict) {
    if (nbits != kPanasonicAcBits && nbits != kPanasonicAcShortBits)
      return false;  // Not strictly a PANASONIC_AC message.
  }

  if (results->rawlen <=
      min_nr_of_messages * (2 * nbits + kHeader + kFooter) - 1 + offset)
    return false;  // Can't possibly be a valid PANASONIC_AC message.

  // Match Header + Data #1 + Footer
  uint16_t used;
  used = matchGeneric(results->rawbuf + offset, results->state,
                      results->rawlen - offset, kPanasonicAcSection1Length * 8,
                      kPanasonicHdrMark, kPanasonicHdrSpace,
                      kPanasonicBitMark, kPanasonicOneSpace,
                      kPanasonicBitMark, kPanasonicZeroSpace,
                      kPanasonicBitMark, kPanasonicAcSectionGap, false,
                      kPanasonicAcTolerance, kPanasonicAcExcess, false);
  if (!used) return false;
  offset += used;

  // Match Header + Data #2 + Footer
  if (!matchGeneric(results->rawbuf + offset,
                    results->state + kPanasonicAcSection1Length,
                    results->rawlen - offset,
                    nbits - kPanasonicAcSection1Length * 8,
                    kPanasonicHdrMark, kPanasonicHdrSpace,
                    kPanasonicBitMark, kPanasonicOneSpace,
                    kPanasonicBitMark, kPanasonicZeroSpace,
                    kPanasonicBitMark, kPanasonicAcMessageGap, true,
                    kPanasonicAcTolerance, kPanasonicAcExcess, false))
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
  results->decode_type = decode_type_t::PANASONIC_AC;
  results->bits = nbits;
  return true;
}
#endif  // DECODE_PANASONIC_AC
