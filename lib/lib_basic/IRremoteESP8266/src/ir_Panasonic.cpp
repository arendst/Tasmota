// Copyright 2015 Kristian Lauszus
// Copyright 2017, 2018 David Conran

/// @file
/// @brief Support for Panasonic protocols.
/// Panasonic protocol originally added by Kristian Lauszus
/// (Thanks to zenwheel and other people at the original blog post)
/// @see Panasonic https://github.com/z3t0/Arduino-IRremote
/// @see http://www.remotecentral.com/cgi-bin/mboard/rc-pronto/thread.cgi?2615
/// @see Panasonic A/C support heavily influenced by https://github.com/ToniA/ESPEasy/blob/HeatpumpIR/lib/HeatpumpIR/PanasonicHeatpumpIR.cpp
/// Panasonic A/C Clock & Timer support:
///   Reverse Engineering by MikkelTb
///   Code by crankyoldgit

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

// Constants
/// @see http://www.remotecentral.com/cgi-bin/mboard/rc-pronto/thread.cgi?26152
const uint16_t kPanasonicHdrMark = 3456;             ///< uSeconds.
const uint16_t kPanasonicHdrSpace = 1728;            ///< uSeconds.
const uint16_t kPanasonicBitMark = 432;              ///< uSeconds.
const uint16_t kPanasonicOneSpace = 1296;            ///< uSeconds.
const uint16_t kPanasonicZeroSpace = 432;            ///< uSeconds.
const uint32_t kPanasonicMinCommandLength = 163296;  ///< uSeconds.
const uint16_t kPanasonicEndGap = 5000;              ///< uSeconds. See #245
const uint32_t kPanasonicMinGap = 74736;             ///< uSeconds.

const uint16_t kPanasonicAcSectionGap = 10000;       ///< uSeconds.
const uint16_t kPanasonicAcSection1Length = 8;
const uint32_t kPanasonicAcMessageGap = kDefaultMessageGap;  // Just a guess.

const uint16_t kPanasonicAc32HdrMark = 3543;         ///< uSeconds.
const uint16_t kPanasonicAc32BitMark = 920;          ///< uSeconds.
const uint16_t kPanasonicAc32HdrSpace = 3450;        ///< uSeconds.
const uint16_t kPanasonicAc32OneSpace = 2575;        ///< uSeconds.
const uint16_t kPanasonicAc32ZeroSpace = 828;        ///< uSeconds.
const uint16_t kPanasonicAc32SectionGap = 13946;     ///< uSeconds.
const uint8_t  kPanasonicAc32Sections = 2;
const uint8_t  kPanasonicAc32BlocksPerSection = 2;

using irutils::addBoolToString;
using irutils::addFanToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addModelToString;
using irutils::addSwingHToString;
using irutils::addSwingVToString;
using irutils::addTempToString;
using irutils::minsToString;
using irutils::setBit;
using irutils::setBits;

// Used by Denon as well.
#if (SEND_PANASONIC || SEND_DENON)
/// Send a Panasonic formatted message.
/// Status: STABLE / Should be working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @note This protocol is a modified version of Kaseikyo.
/// @note Use this method if you want to send the results of `decodePanasonic`.
void IRsend::sendPanasonic64(const uint64_t data, const uint16_t nbits,
                             const uint16_t repeat) {
  sendGeneric(kPanasonicHdrMark, kPanasonicHdrSpace, kPanasonicBitMark,
              kPanasonicOneSpace, kPanasonicBitMark, kPanasonicZeroSpace,
              kPanasonicBitMark, kPanasonicMinGap, kPanasonicMinCommandLength,
              data, nbits, kPanasonicFreq, true, repeat, 50);
}

/// Send a Panasonic formatted message.
/// Status: STABLE, but DEPRECATED
/// @deprecated This is only for legacy use only, please use `sendPanasonic64()`
///   instead.
/// @param[in] address The 16-bit manufacturer code.
/// @param[in] data The 32-bit data portion of the message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @note This protocol is a modified version of Kaseikyo.
void IRsend::sendPanasonic(const uint16_t address, const uint32_t data,
                           const uint16_t nbits, const uint16_t repeat) {
  sendPanasonic64(((uint64_t)address << 32) | (uint64_t)data, nbits, repeat);
}

/// Calculate the raw Panasonic data based on device, subdevice, & function.
/// Status: STABLE / Should be working.
/// @param[in] manufacturer A 16-bit manufacturer code. e.g. 0x4004 is Panasonic
/// @param[in] device An 8-bit code.
/// @param[in] subdevice An 8-bit code.
/// @param[in] function An 8-bit code.
/// @return A value suitable for use with `sendPanasonic64()`.
/// @note Panasonic 48-bit protocol is a modified version of Kaseikyo.
/// @see http://www.remotecentral.com/cgi-bin/mboard/rc-pronto/thread.cgi?2615
uint64_t IRsend::encodePanasonic(const uint16_t manufacturer,
                                 const uint8_t device,
                                 const uint8_t subdevice,
                                 const uint8_t function) {
  uint8_t checksum = device ^ subdevice ^ function;
  return (((uint64_t)manufacturer << 32) | ((uint64_t)device << 24) |
          ((uint64_t)subdevice << 16) | ((uint64_t)function << 8) | checksum);
}
#endif  // (SEND_PANASONIC || SEND_DENON)

// Used by Denon as well.
#if (DECODE_PANASONIC || DECODE_DENON)
/// Decode the supplied Panasonic message.
/// Status: STABLE / Should be working.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] manufacturer A 16-bit manufacturer code. e.g. 0x4004 is Panasonic
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
/// @warning Results to be used with `sendPanasonic64()`, not `sendPanasonic()`.
/// @note Panasonic 48-bit protocol is a modified version of Kaseikyo.
/// @see http://www.remotecentral.com/cgi-bin/mboard/rc-pronto/thread.cgi?2615
/// @see http://www.hifi-remote.com/wiki/index.php?title=Panasonic
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
/// Send a Panasonic A/C message.
/// Status: STABLE / Work with real device(s).
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
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

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRPanasonicAc::IRPanasonicAc(const uint16_t pin, const bool inverted,
                             const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Reset the state of the remote to a known good state/sequence.
void IRPanasonicAc::stateReset(void) {
  memcpy(remote_state, kPanasonicKnownGoodState, kPanasonicAcStateLength);
  _temp = 25;  // An initial saved desired temp. Completely made up.
  _swingh = kPanasonicAcSwingHMiddle;  // A similar made up value for H Swing.
}

/// Set up hardware to be able to send a message.
void IRPanasonicAc::begin(void) { _irsend.begin(); }

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length of the state array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRPanasonicAc::validChecksum(const uint8_t *state, const uint16_t length) {
  if (length < 2) return false;  // 1 byte of data can't have a checksum.
  return (state[length - 1] ==
          sumBytes(state, length - 1, kPanasonicAcChecksumInit));
}

/// Calculate the checksum for a given state.
/// @param[in] state The value to calc the checksum of.
/// @param[in] length The size/length of the state.
/// @return The calculated checksum value.
uint8_t IRPanasonicAc::calcChecksum(const uint8_t *state,
                                    const uint16_t length) {
  return sumBytes(state, length - 1, kPanasonicAcChecksumInit);
}

/// Calculate and set the checksum values for the internal state.
/// @param[in] length The size/length of the state.
void IRPanasonicAc::fixChecksum(const uint16_t length) {
  remote_state[length - 1] = calcChecksum(remote_state, length);
}

#if SEND_PANASONIC_AC
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRPanasonicAc::send(const uint16_t repeat) {
  _irsend.sendPanasonicAC(getRaw(), kPanasonicAcStateLength, repeat);
}
#endif  // SEND_PANASONIC_AC

/// Set the model of the A/C to emulate.
/// @param[in] model The enum of the appropriate model.
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

/// Get/Detect the model of the A/C.
/// @return The enum of the compatible model.
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

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRPanasonicAc::getRaw(void) {
  fixChecksum();
  return remote_state;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] state A valid code for this protocol.
void IRPanasonicAc::setRaw(const uint8_t state[]) {
  memcpy(remote_state, state, kPanasonicAcStateLength);
}

/// Control the power state of the A/C unit.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @warning For CKP models, the remote has no memory of the power state the A/C
///   unit should be in. For those models setting this on/true will toggle the
///   power state of the Panasonic A/C unit with the next message.
///     e.g. If the A/C unit is already on, setPower(true) will turn it off.
///       If the A/C unit is already off, setPower(true) will turn it on.
///       `setPower(false)` will leave the A/C power state as it was.
///   For all other models, setPower(true) should set the internal state to
///   turn it on, and setPower(false) should turn it off.
void IRPanasonicAc::setPower(const bool on) {
  setBit(&remote_state[13], kPanasonicAcPowerOffset, on);
}

/// Get the A/C power state of the remote.
/// @return true, the setting is on. false, the setting is off.
/// @warning Except for CKP models, where it returns if the power state will be
///   toggled on the A/C unit when the next message is sent.
bool IRPanasonicAc::getPower(void) {
  return GETBIT8(remote_state[13], kPanasonicAcPowerOffset);
}

/// Change the power setting to On.
void IRPanasonicAc::on(void) { setPower(true); }

/// Change the power setting to Off.
void IRPanasonicAc::off(void) { setPower(false); }

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRPanasonicAc::getMode(void) {
  return GETBITS8(remote_state[13], kHighNibble, kModeBitsSize);
}

/// Set the operating mode of the A/C.
/// @param[in] desired The desired operating mode.
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
  setBits(&remote_state[13], kHighNibble, kModeBitsSize, mode);
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRPanasonicAc::getTemp(void) {
  return GETBITS8(remote_state[14], kPanasonicAcTempOffset,
                  kPanasonicAcTempSize);
}

/// Set the temperature.
/// @param[in] celsius The temperature in degrees celsius.
/// @param[in] remember: A flag for the class to remember the temperature.
/// @note Automatically safely limits the temp to the operating range supported.
void IRPanasonicAc::setTemp(const uint8_t celsius, const bool remember) {
  uint8_t temperature;
  temperature = std::max(celsius, kPanasonicAcMinTemp);
  temperature = std::min(temperature, kPanasonicAcMaxTemp);
  if (remember) _temp = temperature;
  setBits(&remote_state[14], kPanasonicAcTempOffset, kPanasonicAcTempSize,
          temperature);
}

/// Get the current vertical swing setting.
/// @return The current position it is set to.
uint8_t IRPanasonicAc::getSwingVertical(void) {
  return GETBITS8(remote_state[16], kLowNibble, kNibbleSize);
}

/// Control the vertical swing setting.
/// @param[in] desired_elevation The position to set the vertical swing to.
void IRPanasonicAc::setSwingVertical(const uint8_t desired_elevation) {
  uint8_t elevation = desired_elevation;
  if (elevation != kPanasonicAcSwingVAuto) {
    elevation = std::max(elevation, kPanasonicAcSwingVHighest);
    elevation = std::min(elevation, kPanasonicAcSwingVLowest);
  }
  setBits(&remote_state[16], kLowNibble, kNibbleSize, elevation);
}

/// Get the current horizontal swing setting.
/// @return The current position it is set to.
uint8_t IRPanasonicAc::getSwingHorizontal(void) {
  return GETBITS8(remote_state[17], kLowNibble, kNibbleSize);
}

/// Control the horizontal swing setting.
/// @param[in] desired_direction The position to set the horizontal swing to.
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
  switch (getModel()) {
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

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
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

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRPanasonicAc::getFan(void) {
  return GETBITS8(remote_state[16], kHighNibble, kNibbleSize) -
      kPanasonicAcFanDelta;
}

/// Get the Quiet setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRPanasonicAc::getQuiet(void) {
  switch (getModel()) {
    case kPanasonicRkr:
    case kPanasonicCkp:
      return GETBIT8(remote_state[21], kPanasonicAcQuietCkpOffset);
    default:
      return GETBIT8(remote_state[21], kPanasonicAcQuietOffset);
  }
}

/// Set the Quiet setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRPanasonicAc::setQuiet(const bool on) {
  uint8_t offset;
  switch (getModel()) {
    case kPanasonicRkr:
    case kPanasonicCkp: offset = kPanasonicAcQuietCkpOffset; break;
    default:            offset = kPanasonicAcQuietOffset;
  }
  if (on) setPowerful(false);  // Powerful is mutually exclusive.
  setBit(&remote_state[21], offset, on);
}

/// Get the Powerful (Turbo) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRPanasonicAc::getPowerful(void) {
  switch (getModel()) {
    case kPanasonicRkr:
    case kPanasonicCkp:
      return GETBIT8(remote_state[21], kPanasonicAcPowerfulCkpOffset);
    default:
      return GETBIT8(remote_state[21], kPanasonicAcPowerfulOffset);
  }
}

/// Set the Powerful (Turbo) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRPanasonicAc::setPowerful(const bool on) {
  uint8_t offset;
  switch (getModel()) {
    case kPanasonicRkr:
    case kPanasonicCkp: offset = kPanasonicAcPowerfulCkpOffset; break;
    default:            offset = kPanasonicAcPowerfulOffset;
  }

  if (on) setQuiet(false);  // Quiet is mutually exclusive.
  setBit(&remote_state[21], offset, on);
}

/// Convert standard (military/24hr) time to nr. of minutes since midnight.
/// @param[in] hours The hours component of the time.
/// @param[in] mins The minutes component of the time.
/// @return The nr of minutes since midnight.
uint16_t IRPanasonicAc::encodeTime(const uint8_t hours, const uint8_t mins) {
  return std::min(hours, (uint8_t)23) * 60 + std::min(mins, (uint8_t)59);
}

/// Get the time from a given pointer location.
/// @param[in] ptr A pointer to a time location in a state.
/// @return The time expressed as nr. of minutes past midnight.
/// @note Internal use only.
uint16_t IRPanasonicAc::_getTime(const uint8_t ptr[]) {
  uint16_t result = (GETBITS8(
      ptr[1], kLowNibble, kPanasonicAcTimeOverflowSize) <<
      (kPanasonicAcTimeSize - kPanasonicAcTimeOverflowSize)) + ptr[0];
  if (result == kPanasonicAcTimeSpecial) return 0;
  return result;
}

/// Get the current clock time value.
/// @return The time expressed as nr. of minutes past midnight.
uint16_t IRPanasonicAc::getClock(void) { return _getTime(&remote_state[24]); }

/// Set the time at a given pointer location.
/// @param[in, out] ptr A pointer to a time location in a state.
/// @param[in] mins_since_midnight The time as nr. of minutes past midnight.
/// @param[in] round_down Do we round to the nearest 10 minute mark?
/// @note Internal use only.
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

/// Set the current clock time value.
/// @param[in] mins_since_midnight The time as nr. of minutes past midnight.
void IRPanasonicAc::setClock(const uint16_t mins_since_midnight) {
  _setTime(&remote_state[24], mins_since_midnight, false);
}

/// Get the On Timer time value.
/// @return The time expressed as nr. of minutes past midnight.
uint16_t IRPanasonicAc::getOnTimer(void) { return _getTime(&remote_state[18]); }

/// Set/Enable the On Timer.
/// @param[in] mins_since_midnight The time as nr. of minutes past midnight.
/// @param[in] enable Do we enable the timer or not?
void IRPanasonicAc::setOnTimer(const uint16_t mins_since_midnight,
                               const bool enable) {
  // Set the timer flag.
  setBit(&remote_state[13], kPanasonicAcOnTimerOffset, enable);
  // Store the time.
  _setTime(&remote_state[18], mins_since_midnight, true);
}

/// Cancel the On Timer.
void IRPanasonicAc::cancelOnTimer(void) { setOnTimer(0, false); }

/// Check if the On Timer is Enabled.
/// @return true, the setting is on. false, the setting is off.
bool IRPanasonicAc::isOnTimerEnabled(void) {
  return GETBIT8(remote_state[13], kPanasonicAcOnTimerOffset);
}

/// Get the Off Timer time value.
/// @return The time expressed as nr. of minutes past midnight.
uint16_t IRPanasonicAc::getOffTimer(void) {
  uint16_t result = (GETBITS8(remote_state[20], 0, 7) << kNibbleSize) |
      GETBITS8(remote_state[19], kHighNibble, kNibbleSize);
  if (result == kPanasonicAcTimeSpecial) return 0;
  return result;
}

/// Set/Enable the Off Timer.
/// @param[in] mins_since_midnight The time as nr. of minutes past midnight.
/// @param[in] enable Do we enable the timer or not?
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

/// Cancel the Off Timer.
void IRPanasonicAc::cancelOffTimer(void) { setOffTimer(0, false); }

/// Check if the Off Timer is Enabled.
/// @return true, the setting is on. false, the setting is off.
bool IRPanasonicAc::isOffTimerEnabled(void) {
  return GETBIT8(remote_state[13], kPanasonicAcOffTimerOffset);
}

/// Get the Ion (filter) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRPanasonicAc::getIon(void) {
  switch (getModel()) {
    case kPanasonicDke:
      return GETBIT8(remote_state[kPanasonicAcIonFilterByte],
                     kPanasonicAcIonFilterOffset);
    default:
      return false;
  }
}

/// Set the Ion (filter) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRPanasonicAc::setIon(const bool on) {
  if (getModel() == kPanasonicDke)
    setBit(&remote_state[kPanasonicAcIonFilterByte],
           kPanasonicAcIonFilterOffset, on);
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRPanasonicAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kPanasonicAcCool;
    case stdAc::opmode_t::kHeat: return kPanasonicAcHeat;
    case stdAc::opmode_t::kDry:  return kPanasonicAcDry;
    case stdAc::opmode_t::kFan:  return kPanasonicAcFan;
    default:                     return kPanasonicAcAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
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

/// Convert a standard A/C vertical swing into its native setting.
/// @param[in] position A stdAc::swingv_t position to convert.
/// @return The equivalent native horizontal swing position.
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

/// Convert a standard A/C horizontal swing into its native setting.
/// @param[in] position A stdAc::swingh_t position to convert.
/// @return The equivalent native horizontal swing position.
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

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRPanasonicAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kPanasonicAcCool: return stdAc::opmode_t::kCool;
    case kPanasonicAcHeat: return stdAc::opmode_t::kHeat;
    case kPanasonicAcDry:  return stdAc::opmode_t::kDry;
    case kPanasonicAcFan:  return stdAc::opmode_t::kFan;
    default:               return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] spd The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
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

/// Convert a native horizontal swing postion to it's common equivalent.
/// @param[in] pos A native position to convert.
/// @return The common horizontal swing position.
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

/// Convert a native vertical swing postion to it's common equivalent.
/// @param[in] pos A native position to convert.
/// @return The common vertical swing position.
stdAc::swingv_t IRPanasonicAc::toCommonSwingV(const uint8_t pos) {
  if (pos >= kPanasonicAcSwingVHighest && pos <= kPanasonicAcSwingVLowest)
    return (stdAc::swingv_t)pos;
  else
    return stdAc::swingv_t::kAuto;
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRPanasonicAc::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::PANASONIC_AC;
  result.model = getModel();
  result.power = getPower();
  result.mode = toCommonMode(getMode());
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(getFan());
  result.swingv = toCommonSwingV(getSwingVertical());
  result.swingh = toCommonSwingH(getSwingHorizontal());
  result.quiet = getQuiet();
  result.turbo = getPowerful();
  result.filter = getIon();
  // Not supported.
  result.econo = false;
  result.clean = false;
  result.light = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

/// Convert the internal state into a human readable string.
/// @return A string containing the settings in human-readable form.
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
  result += addSwingVToString(getSwingVertical(), kPanasonicAcSwingVAuto,
                              kPanasonicAcSwingVHighest,
                              kPanasonicAcSwingVHigh,
                              kPanasonicAcSwingVAuto,  // Upper Middle is unused
                              kPanasonicAcSwingVMiddle,
                              kPanasonicAcSwingVAuto,  // Lower Middle is unused
                              kPanasonicAcSwingVLow,
                              kPanasonicAcSwingVLowest,
                              // Below are unused.
                              kPanasonicAcSwingVAuto,
                              kPanasonicAcSwingVAuto,
                              kPanasonicAcSwingVAuto,
                              kPanasonicAcSwingVAuto);
  switch (getModel()) {
    case kPanasonicJke:
    case kPanasonicCkp:
      break;  // No Horizontal Swing support.
    default:
      result += addSwingHToString(getSwingHorizontal(), kPanasonicAcSwingHAuto,
                                  kPanasonicAcSwingHFullLeft,
                                  kPanasonicAcSwingHLeft,
                                  kPanasonicAcSwingHMiddle,
                                  kPanasonicAcSwingHRight,
                                  kPanasonicAcSwingHFullRight,
                                  // Below are unused.
                                  kPanasonicAcSwingHAuto,
                                  kPanasonicAcSwingHAuto,
                                  kPanasonicAcSwingHAuto,
                                  kPanasonicAcSwingHAuto,
                                  kPanasonicAcSwingHAuto);
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
/// Decode the supplied Panasonic AC message.
/// Status: STABLE / Works with real device(s).
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
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

#if SEND_PANASONIC_AC32
/// Send a Panasonic AC 32/16bit formatted message.
/// Status: STABLE / Confirmed working.
/// @param[in] data containing the IR command.
/// @param[in] nbits Nr. of bits to send. Usually kPanasonicAc32Bits
/// @param[in] repeat Nr. of times the message is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1307
void IRsend::sendPanasonicAC32(const uint64_t data, const uint16_t nbits,
                               const uint16_t repeat) {
  uint16_t section_bits;
  uint16_t sections;
  uint16_t blocks;
  // Calculate the section, block, and bit sizes based on the requested bit size
  if (nbits > kPanasonicAc32Bits / 2) {  // A long message
    section_bits = nbits / kPanasonicAc32Sections;
    sections = kPanasonicAc32Sections;
    blocks = kPanasonicAc32BlocksPerSection;
  } else {  // A short message
    section_bits = nbits;
    sections = kPanasonicAc32Sections - 1;
    blocks = kPanasonicAc32BlocksPerSection + 1;
  }
  for (uint16_t r = 0; r <= repeat; r++) {
    for (uint8_t section = 0; section < sections;  section++) {
      uint64_t section_data;
      section_data = GETBITS64(data, section_bits * (sections - section - 1),
                               section_bits);

      // Duplicate bytes in the data.
      uint64_t expanded_data = 0;
      for (uint8_t i = 0; i < sizeof(expanded_data); i++) {
       const uint8_t first_byte = section_data >> 56;
       for (uint8_t i = 0; i < 2; i++)
         expanded_data = (expanded_data << 8) | first_byte;
       section_data <<= 8;
      }
      // Two data blocks per section (i.e. 1 + a repeat)
      sendGeneric(kPanasonicAc32HdrMark, kPanasonicAc32HdrSpace,  // Header
                  kPanasonicAc32BitMark, kPanasonicAc32OneSpace,  // Data
                  kPanasonicAc32BitMark, kPanasonicAc32ZeroSpace,
                  0, 0,  // No Footer
                  expanded_data, section_bits * 2, kPanasonicFreq, false,
                  blocks - 1,  // Repeat
                  50);
      // Section Footer
      sendGeneric(kPanasonicAc32HdrMark, kPanasonicAc32HdrSpace,  // Header
                  0, 0, 0, 0,  // No Data
                  kPanasonicAc32BitMark, kPanasonicAc32SectionGap,  // Footer
                  data, 0,  // No data (bits)
                  kPanasonicFreq, true, 0, 50);
    }
  }
}
#endif  // SEND_PANASONIC_AC32

#if DECODE_PANASONIC_AC32
/// Decode the supplied Panasonic AC 32/16bit message.
/// Status: STABLE / Confirmed working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
///   Typically: kPanasonicAc32Bits or kPanasonicAc32Bits/2
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1307
/// @note Protocol has two known configurations:
///   (long)
///   Two sections of identical 32 bit data block pairs. ie. (32+32)+(32+32)=128
///   or
///   (short)
///   A single section of 3 x identical 32 bit data blocks i.e. (32+32+32)=96
/// Each data block also has a pair of 8 bits repeated identical bits.
/// e.g. (8+8)+(8+8)=32
///
/// So each long version really only has 32 unique bits, and the short version
/// really only has 16 unique bits.
bool IRrecv::decodePanasonicAC32(decode_results *results, uint16_t offset,
                                 const uint16_t nbits, const bool strict) {
  if (strict && (nbits != kPanasonicAc32Bits &&
                 nbits != kPanasonicAc32Bits / 2))
    return false;  // Not strictly a valid bit size.

  // Determine if this is a long or a short message we are looking for.
  const bool is_long = (nbits > kPanasonicAc32Bits / 2);
  const uint16_t min_length = is_long ?
      kPanasonicAc32Sections * kPanasonicAc32BlocksPerSection *
      ((2 * nbits) + kHeader + kFooter) - 1 + offset :
      (kPanasonicAc32BlocksPerSection + 1) * ((4 * nbits) + kHeader) +
      kFooter - 1 + offset;

  if (results->rawlen < min_length)
    return false;  // Can't possibly be a valid message.

  // Calculate the parameters for the decode based on it's length.
  uint16_t sections;
  uint16_t blocks_per_section;
  if (is_long) {
    sections = kPanasonicAc32Sections;
    blocks_per_section = kPanasonicAc32BlocksPerSection;
  } else {
    sections = kPanasonicAc32Sections - 1;
    blocks_per_section = kPanasonicAc32BlocksPerSection + 1;
  }
  const uint16_t bits_per_block = nbits / sections;

  uint64_t data = 0;
  uint64_t section_data = 0;
  uint32_t prev_section_data;

  // Match all the expected data blocks.
  for (uint16_t block = 0;
       block < sections * blocks_per_section;
       block++) {
    prev_section_data = section_data;
    uint16_t used = matchGeneric(results->rawbuf + offset, &section_data,
                                 results->rawlen - offset, bits_per_block * 2,
                                 kPanasonicAc32HdrMark, kPanasonicAc32HdrSpace,
                                 kPanasonicAc32BitMark, kPanasonicAc32OneSpace,
                                 kPanasonicAc32BitMark, kPanasonicAc32ZeroSpace,
                                 0, 0,  // No Footer
                                 false, kUseDefTol, kMarkExcess, false);
    if (!used) return false;
    offset += used;
    // Is it the first block of the section?
    if (block % blocks_per_section == 0) {
      // The protocol repeats each byte twice, so to shrink the code we
      // remove the duplicate bytes in the collected data. We only need to do
      // this for the first block in a section.
      uint64_t shrunk_data = 0;
      uint64_t data_copy = section_data;
      for (uint8_t i = 0; i < sizeof(data_copy); i += 2) {
        const uint8_t first_byte = GETBITS64(data_copy,
                                             (sizeof(data_copy) - 1) * 8, 8);
        shrunk_data = (shrunk_data << 8) | first_byte;
        // Compliance
        if (strict) {
          // Every second byte must be a duplicate of the previous.
          const uint8_t next_byte = GETBITS64(data_copy,
                                              (sizeof(data_copy) - 2) * 8, 8);
          if (first_byte != next_byte) return false;
        }
        data_copy <<= 16;
      }
      // Keep the data from the first of the block in the section.
      data = (data << bits_per_block) | shrunk_data;
    } else {  // Not the first block in a section.
      // Compliance
      if (strict)
        // Compare the data from the blocks in pairs.
        if (section_data != prev_section_data) return false;
      // Look for the section footer at the end of the blocks.
      if ((block + 1) % blocks_per_section == 0) {
        uint64_t junk;
        used = matchGeneric(results->rawbuf + offset, &junk,
                            results->rawlen - offset, 0,
                            // Header
                            kPanasonicAc32HdrMark, kPanasonicAc32HdrSpace,
                            // No Data
                            0, 0,
                            0, 0,
                            // Footer
                            kPanasonicAc32BitMark, kPanasonicAc32SectionGap,
                            true);
        if (!used) return false;
        offset += used;
      }
    }
  }

  // Success
  results->value = data;
  results->decode_type = decode_type_t::PANASONIC_AC32;
  results->bits = nbits;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif  // DECODE_PANASONIC_AC32

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRPanasonicAc32::IRPanasonicAc32(const uint16_t pin, const bool inverted,
                                 const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

#if SEND_PANASONIC_AC32
/// Send the current internal state as IR messages.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRPanasonicAc32::send(const uint16_t repeat) {
  _irsend.sendPanasonicAC32(getRaw(), kPanasonicAc32Bits, repeat);
}
#endif  // SEND_PANASONIC_AC32

/// Set up hardware to be able to send a message.
void IRPanasonicAc32::begin(void) { _irsend.begin(); }

/// Get a copy of the internal state/code for this protocol.
/// @return The code for this protocol based on the current internal state.
uint32_t IRPanasonicAc32::getRaw(void) const { return _.raw; }

/// Set the internal state from a valid code for this protocol.
/// @param[in] state A valid code for this protocol.
void IRPanasonicAc32::setRaw(const uint32_t state) {  _.raw = state; }

/// Reset the state of the remote to a known good state/sequence.
void IRPanasonicAc32::stateReset(void) { setRaw(kPanasonicAc32KnownGood); }

/// Set the Power Toggle setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRPanasonicAc32::setPowerToggle(const bool on) { _.PowerToggle = !on; }

/// Get the Power Toggle setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRPanasonicAc32::getPowerToggle(void) const { return !_.PowerToggle; }

/// Set the desired temperature.
/// @param[in] degrees The temperature in degrees celsius.
void IRPanasonicAc32::setTemp(const uint8_t degrees) {
  uint8_t temp = std::max((uint8_t)kPanasonicAcMinTemp, degrees);
  temp = std::min((uint8_t)kPanasonicAcMaxTemp, temp);
  _.Temp = temp - (kPanasonicAcMinTemp - 1);
}

/// Get the current desired temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRPanasonicAc32::getTemp(void) const {
  return _.Temp + (kPanasonicAcMinTemp - 1);
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRPanasonicAc32::getMode(void) const { return _.Mode; }

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
/// @note If we get an unexpected mode, default to AUTO.
void IRPanasonicAc32::setMode(const uint8_t mode) {
  switch (mode) {
    case kPanasonicAc32Auto:
    case kPanasonicAc32Cool:
    case kPanasonicAc32Dry:
    case kPanasonicAc32Heat:
    case kPanasonicAc32Fan:
      _.Mode = mode;
      break;
    default: _.Mode = kPanasonicAc32Auto;
  }
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRPanasonicAc32::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kPanasonicAc32Cool;
    case stdAc::opmode_t::kHeat: return kPanasonicAc32Heat;
    case stdAc::opmode_t::kDry:  return kPanasonicAc32Dry;
    case stdAc::opmode_t::kFan:  return kPanasonicAc32Fan;
    default:                     return kPanasonicAc32Auto;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRPanasonicAc32::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kPanasonicAc32Cool: return stdAc::opmode_t::kCool;
    case kPanasonicAc32Heat: return stdAc::opmode_t::kHeat;
    case kPanasonicAc32Dry:  return stdAc::opmode_t::kDry;
    case kPanasonicAc32Fan:  return stdAc::opmode_t::kFan;
    default:                 return stdAc::opmode_t::kAuto;
  }
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRPanasonicAc32::setFan(const uint8_t speed) {
  switch (speed) {
    case kPanasonicAc32FanMin:
    case kPanasonicAc32FanLow:
    case kPanasonicAc32FanMed:
    case kPanasonicAc32FanHigh:
    case kPanasonicAc32FanMax:
    case kPanasonicAc32FanAuto:
      _.Fan = speed;
      break;
    default: _.Fan = kPanasonicAc32FanAuto;
  }
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRPanasonicAc32::getFan(void) const { return _.Fan; }

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] spd The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRPanasonicAc32::toCommonFanSpeed(const uint8_t spd) {
  switch (spd) {
    case kPanasonicAc32FanMax:     return stdAc::fanspeed_t::kMax;
    case kPanasonicAc32FanHigh:    return stdAc::fanspeed_t::kHigh;
    case kPanasonicAc32FanMed:     return stdAc::fanspeed_t::kMedium;
    case kPanasonicAc32FanLow:     return stdAc::fanspeed_t::kLow;
    case kPanasonicAc32FanMin:     return stdAc::fanspeed_t::kMin;
    default:                       return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRPanasonicAc32::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:    return kPanasonicAc32FanMin;
    case stdAc::fanspeed_t::kLow:    return kPanasonicAc32FanLow;
    case stdAc::fanspeed_t::kMedium: return kPanasonicAc32FanMed;
    case stdAc::fanspeed_t::kHigh:   return kPanasonicAc32FanHigh;
    case stdAc::fanspeed_t::kMax:    return kPanasonicAc32FanMax;
    default:                         return kPanasonicAc32FanAuto;
  }
}

/// Get the current horizontal swing setting.
/// @return The current position it is set to.
bool IRPanasonicAc32::getSwingHorizontal(void) const { return _.SwingH; }

/// Control the horizontal swing setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRPanasonicAc32::setSwingHorizontal(const bool on) { _.SwingH = on; }

/// Get the current vertical swing setting.
/// @return The current position it is set to.
uint8_t IRPanasonicAc32::getSwingVertical(void) const { return _.SwingV; }

/// Control the vertical swing setting.
/// @param[in] pos The position to set the vertical swing to.
void IRPanasonicAc32::setSwingVertical(const uint8_t pos) {
  uint8_t elevation = pos;
  if (elevation != kPanasonicAc32SwingVAuto) {
    elevation = std::max(elevation, kPanasonicAcSwingVHighest);
    elevation = std::min(elevation, kPanasonicAcSwingVLowest);
  }
  _.SwingV = elevation;
}

/// Convert a native vertical swing postion to it's common equivalent.
/// @param[in] pos A native position to convert.
/// @return The common vertical swing position.
stdAc::swingv_t IRPanasonicAc32::toCommonSwingV(const uint8_t pos) {
  return IRPanasonicAc::toCommonSwingV(pos);
}

/// Convert a standard A/C vertical swing into its native setting.
/// @param[in] position A stdAc::swingv_t position to convert.
/// @return The equivalent native horizontal swing position.
uint8_t IRPanasonicAc32::convertSwingV(const stdAc::swingv_t position) {
  switch (position) {
    case stdAc::swingv_t::kHighest:
    case stdAc::swingv_t::kHigh:
    case stdAc::swingv_t::kMiddle:
    case stdAc::swingv_t::kLow:
    case stdAc::swingv_t::kLowest: return (uint8_t)position;
    default:                       return kPanasonicAc32SwingVAuto;
  }
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRPanasonicAc32::toString(void) const {
  String result = "";
  result.reserve(110);
  result += addBoolToString(getPowerToggle(), kPowerToggleStr, false);
  result += addModeToString(_.Mode, kPanasonicAc32Auto, kPanasonicAc32Cool,
                            kPanasonicAc32Heat, kPanasonicAc32Dry,
                            kPanasonicAc32Fan);
  result += addTempToString(getTemp());
  result += addFanToString(_.Fan, kPanasonicAc32FanHigh, kPanasonicAc32FanLow,
                           kPanasonicAc32FanAuto, kPanasonicAc32FanMin,
                           kPanasonicAc32FanMed, kPanasonicAc32FanMax);
  result += addBoolToString(_.SwingH, kSwingHStr);
  result += addSwingVToString(getSwingVertical(),
                              kPanasonicAc32SwingVAuto,
                              kPanasonicAcSwingVHighest,
                              kPanasonicAcSwingVHigh,
                              kPanasonicAc32SwingVAuto,  // Upper Middle unused
                              kPanasonicAcSwingVMiddle,
                              kPanasonicAc32SwingVAuto,  // Lower Middle unused
                              kPanasonicAcSwingVLow,
                              kPanasonicAcSwingVLowest,
                              // Below are unused.
                              kPanasonicAc32SwingVAuto,
                              kPanasonicAc32SwingVAuto,
                              kPanasonicAc32SwingVAuto,
                              kPanasonicAc32SwingVAuto);
  return result;
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @param[in] prev Ptr to the previous state if required.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRPanasonicAc32::toCommon(const stdAc::state_t *prev) const {
  stdAc::state_t result;
  // Start with the previous state if given it.
  if (prev != NULL) {
    result = *prev;
  } else {
    // Set defaults for non-zero values that are not implicitly set for when
    // there is no previous state.
    // e.g. Any setting that toggles should probably go here.
    result.power = false;
  }
  result.protocol = decode_type_t::PANASONIC_AC32;
  result.model = -1;
  if (getPowerToggle()) result.power = !result.power;
  result.mode = toCommonMode(getMode());
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(getFan());
  result.swingv = toCommonSwingV(getSwingVertical());
  result.swingh = getSwingHorizontal() ? stdAc::swingh_t::kAuto
                                       : stdAc::swingh_t::kOff;
  // Not supported.
  result.quiet = false;
  result.turbo = false;
  result.filter = false;
  result.econo = false;
  result.clean = false;
  result.light = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}
