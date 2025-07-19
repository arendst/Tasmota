// Copyright 2009 Ken Shirriff
// Copyright 2017, 2019 David Conran

/// @file
/// @brief Support for Sharp protocols.
/// @see http://www.sbprojects.net/knowledge/ir/sharp.htm
/// @see http://lirc.sourceforge.net/remotes/sharp/GA538WJSA
/// @see http://www.mwftr.com/ucF08/LEC14%20PIC%20IR.pdf
/// @see http://www.hifi-remote.com/johnsfine/DecodeIR.html#Sharp
/// @see GlobalCache's IR Control Tower data.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/638
/// @see https://github.com/ToniA/arduino-heatpumpir/blob/master/SharpHeatpumpIR.cpp

#include "ir_Sharp.h"
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
// period time = 1/38000Hz = 26.316 microseconds.
const uint16_t kSharpTick = 26;
const uint16_t kSharpBitMarkTicks = 10;
const uint16_t kSharpBitMark = kSharpBitMarkTicks * kSharpTick;
const uint16_t kSharpOneSpaceTicks = 70;
const uint16_t kSharpOneSpace = kSharpOneSpaceTicks * kSharpTick;
const uint16_t kSharpZeroSpaceTicks = 30;
const uint16_t kSharpZeroSpace = kSharpZeroSpaceTicks * kSharpTick;
const uint16_t kSharpGapTicks = 1677;
const uint16_t kSharpGap = kSharpGapTicks * kSharpTick;
// Address(5) + Command(8) + Expansion(1) + Check(1)
const uint64_t kSharpToggleMask =
    ((uint64_t)1 << (kSharpBits - kSharpAddressBits)) - 1;
const uint64_t kSharpAddressMask = ((uint64_t)1 << kSharpAddressBits) - 1;
const uint64_t kSharpCommandMask = ((uint64_t)1 << kSharpCommandBits) - 1;

using irutils::addBoolToString;
using irutils::addFanToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addModelToString;
using irutils::addSwingVToString;
using irutils::addTempToString;
using irutils::addToggleToString;
using irutils::minsToString;

// Also used by Denon protocol
#if (SEND_SHARP || SEND_DENON)
/// Send a (raw) Sharp message
/// @note Status: STABLE / Working fine.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @note his procedure handles the inversion of bits required per protocol.
///   The protocol spec says to send the LSB first, but legacy code & usage
///   has us sending the MSB first. Grrrr. Normal invocation of encodeSharp()
///   handles this for you, assuming you are using the correct/standard values.
///   e.g. sendSharpRaw(encodeSharp(address, command));
void IRsend::sendSharpRaw(const uint64_t data, const uint16_t nbits,
                          const uint16_t repeat) {
  uint64_t tempdata = data;
  for (uint16_t i = 0; i <= repeat; i++) {
    // Protocol demands that the data be sent twice; once normally,
    // then with all but the address bits inverted.
    // Note: Previously this used to be performed 3 times (normal, inverted,
    //       normal), however all data points to that being incorrect.
    for (uint8_t n = 0; n < 2; n++) {
      sendGeneric(0, 0,  // No Header
                  kSharpBitMark, kSharpOneSpace, kSharpBitMark, kSharpZeroSpace,
                  kSharpBitMark, kSharpGap, tempdata, nbits, 38, true,
                  0,  // Repeats are handled already.
                  33);
      // Invert the data per protocol. This is always called twice, so it's
      // returned to original upon exiting the inner loop.
      tempdata ^= kSharpToggleMask;
    }
  }
}

/// Encode a (raw) Sharp message from it's components.
/// Status: STABLE / Works okay.
/// @param[in] address The value of the address to be sent.
/// @param[in] command The value of the address to be sent. (8 bits)
/// @param[in] expansion The value of the expansion bit to use.
///   (0 or 1, typically 1)
/// @param[in] check The value of the check bit to use. (0 or 1, typically 0)
/// @param[in] MSBfirst Flag indicating MSB first or LSB first order.
/// @return A uint32_t containing the raw Sharp message for `sendSharpRaw()`.
/// @note Assumes the standard Sharp bit sizes.
///   Historically sendSharp() sends address & command in
///   MSB first order. This is actually incorrect. It should be sent in LSB
///   order. The behaviour of sendSharp() hasn't been changed to maintain
///   backward compatibility.
uint32_t IRsend::encodeSharp(const uint16_t address, const uint16_t command,
                             const uint16_t expansion, const uint16_t check,
                             const bool MSBfirst) {
  // Mask any unexpected bits.
  uint16_t tempaddress = GETBITS16(address, 0, kSharpAddressBits);
  uint16_t tempcommand = GETBITS16(command, 0, kSharpCommandBits);
  uint16_t tempexpansion = GETBITS16(expansion, 0, 1);
  uint16_t tempcheck = GETBITS16(check, 0, 1);

  if (!MSBfirst) {  // Correct bit order if needed.
    tempaddress = reverseBits(tempaddress, kSharpAddressBits);
    tempcommand = reverseBits(tempcommand, kSharpCommandBits);
  }
  // Concatenate all the bits.
  return (tempaddress << (kSharpCommandBits + 2)) | (tempcommand << 2) |
         (tempexpansion << 1) | tempcheck;
}

/// Send a Sharp message
/// Status:  DEPRECATED / Previously working fine.
/// @deprecated Only use this if you are using legacy from the original
///   Arduino-IRremote library. 99% of the time, you will want to use
///   `sendSharpRaw()` instead
/// @param[in] address Address value to be sent.
/// @param[in] command  Command value to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @note This procedure has a non-standard invocation style compared to similar
///   sendProtocol() routines. This is due to legacy, compatibility, & historic
///   reasons. Normally the calling syntax version is like sendSharpRaw().
///  This procedure transmits the address & command in MSB first order, which is
///  incorrect. This behaviour is left as-is to maintain backward
///  compatibility with legacy code.
///  In short, you should use sendSharpRaw(), encodeSharp(), and the correct
///  values of address & command instead of using this, & the wrong values.
void IRsend::sendSharp(const uint16_t address, uint16_t const command,
                       const uint16_t nbits, const uint16_t repeat) {
  sendSharpRaw(encodeSharp(address, command, 1, 0, true), nbits, repeat);
}
#endif  // (SEND_SHARP || SEND_DENON)

// Used by decodeDenon too.
#if (DECODE_SHARP || DECODE_DENON)
/// Decode the supplied Sharp message.
/// Status: STABLE / Working fine.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @param[in] expansion Should we expect the expansion bit to be set.
///   Default is true.
/// @return True if it can decode it, false if it can't.
/// @note This procedure returns a value suitable for use in `sendSharpRaw()`.
/// @todo Need to ensure capture of the inverted message as it can
///   be missed due to the interrupt timeout used to detect an end of message.
///   Several compliance checks are disabled until that is resolved.
bool IRrecv::decodeSharp(decode_results *results, uint16_t offset,
                         const uint16_t nbits, const bool strict,
                         const bool expansion) {
  if (results->rawlen <= 2 * nbits + kFooter - 1 + offset)
    return false;  // Not enough entries to be a Sharp message.
  // Compliance
  if (strict) {
    if (nbits != kSharpBits) return false;  // Request is out of spec.
    // DISABLED - See TODO
#ifdef UNIT_TEST
    // An in spec message has the data sent normally, then inverted. So we
    // expect twice as many entries than to just get the results.
    if (results->rawlen <= (2 * (2 * nbits + kFooter)) - 1 + offset)
      return false;
#endif
  }

  uint64_t data = 0;

  // Match Data + Footer
  uint16_t used;
  used = matchGeneric(results->rawbuf + offset, &data,
                      results->rawlen - offset, nbits,
                      0, 0,  // No Header
                      kSharpBitMark, kSharpOneSpace,
                      kSharpBitMark, kSharpZeroSpace,
                      kSharpBitMark, kSharpGap, true, 35);
  if (!used) return false;
  offset += used;
  // Compliance
  if (strict) {
    // Check the state of the expansion bit is what we expect.
    if ((data & 0b10) >> 1 != expansion) return false;
    // The check bit should be cleared in a normal message.
    if (data & 0b1) return false;
      // DISABLED - See TODO
#ifdef UNIT_TEST
    // Grab the second copy of the data (i.e. inverted)
    uint64_t second_data = 0;
    // Match Data + Footer
    if (!matchGeneric(results->rawbuf + offset, &second_data,
                      results->rawlen - offset, nbits,
                      0, 0,
                      kSharpBitMark, kSharpOneSpace,
                      kSharpBitMark, kSharpZeroSpace,
                      kSharpBitMark, kSharpGap, true, 35)) return false;
    // Check that second_data has been inverted correctly.
    if (data != (second_data ^ kSharpToggleMask)) return false;
#endif  // UNIT_TEST
  }

  // Success
  results->decode_type = SHARP;
  results->bits = nbits;
  results->value = data;
  // Address & command are actually transmitted in LSB first order.
  results->address = reverseBits(data, nbits) & kSharpAddressMask;
  results->command =
      reverseBits((data >> 2) & kSharpCommandMask, kSharpCommandBits);
  return true;
}
#endif  // (DECODE_SHARP || DECODE_DENON)

#if SEND_SHARP_AC
/// Send a Sharp A/C message.
/// Status: Alpha / Untested.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/638
/// @see https://github.com/ToniA/arduino-heatpumpir/blob/master/SharpHeatpumpIR.cpp
void IRsend::sendSharpAc(const unsigned char data[], const uint16_t nbytes,
                         const uint16_t repeat) {
  if (nbytes < kSharpAcStateLength)
    return;  // Not enough bytes to send a proper message.

  sendGeneric(kSharpAcHdrMark, kSharpAcHdrSpace,
              kSharpAcBitMark, kSharpAcOneSpace,
              kSharpAcBitMark, kSharpAcZeroSpace,
              kSharpAcBitMark, kSharpAcGap,
              data, nbytes, 38000, false, repeat, 50);
}
#endif  // SEND_SHARP_AC

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRSharpAc::IRSharpAc(const uint16_t pin, const bool inverted,
                     const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Set up hardware to be able to send a message.
void IRSharpAc::begin(void) { _irsend.begin(); }

#if SEND_SHARP_AC
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRSharpAc::send(const uint16_t repeat) {
  _irsend.sendSharpAc(getRaw(), kSharpAcStateLength, repeat);
}
#endif  // SEND_SHARP_AC

/// Calculate the checksum for a given state.
/// @param[in] state The array to calc the checksum of.
/// @param[in] length The length/size of the array.
/// @return The calculated 4-bit checksum value.
uint8_t IRSharpAc::calcChecksum(uint8_t state[], const uint16_t length) {
  uint8_t xorsum = xorBytes(state, length - 1);
  xorsum ^= GETBITS8(state[length - 1], kLowNibble, kNibbleSize);
  xorsum ^= GETBITS8(xorsum, kHighNibble, kNibbleSize);
  return GETBITS8(xorsum, kLowNibble, kNibbleSize);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length/size of the array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRSharpAc::validChecksum(uint8_t state[], const uint16_t length) {
  return GETBITS8(state[length - 1], kHighNibble, kNibbleSize) ==
      IRSharpAc::calcChecksum(state, length);
}

/// Calculate and set the checksum values for the internal state.
void IRSharpAc::checksum(void) {
  _.Sum = calcChecksum(_.raw);
}

/// Reset the state of the remote to a known good state/sequence.
void IRSharpAc::stateReset(void) {
  static const uint8_t reset[kSharpAcStateLength] = {
      0xAA, 0x5A, 0xCF, 0x10, 0x00, 0x01, 0x00, 0x00, 0x08, 0x80, 0x00, 0xE0,
      0x01};
  std::memcpy(_.raw, reset, kSharpAcStateLength);
  _temp = getTemp();
  _mode = _.Mode;
  _fan = _.Fan;
  _model = getModel(true);
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRSharpAc::getRaw(void) {
  checksum();  // Ensure correct settings before sending.
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
/// @param[in] length The length/size of the new_code array.
void IRSharpAc::setRaw(const uint8_t new_code[], const uint16_t length) {
  std::memcpy(_.raw, new_code, std::min(length, kSharpAcStateLength));
  _model = getModel(true);
}

/// Set the model of the A/C to emulate.
/// @param[in] model The enum of the appropriate model.
void IRSharpAc::setModel(const sharp_ac_remote_model_t model) {
  switch (model) {
    case sharp_ac_remote_model_t::A705:
    case sharp_ac_remote_model_t::A903:
      _model = model;
      _.Model = true;
      break;
    default:
      _model = sharp_ac_remote_model_t::A907;
      _.Model = false;
  }
  _.Model2 = (_model != sharp_ac_remote_model_t::A907);
  // Redo the operating mode as some models don't support all modes.
  setMode(_.Mode);
}

/// Get/Detect the model of the A/C.
/// @param[in] raw Try to determine the model from the raw code only.
/// @return The enum of the compatible model.
sharp_ac_remote_model_t IRSharpAc::getModel(const bool raw) const {
  if (raw) {
    if (_.Model2) {
      if (_.Model)
        return sharp_ac_remote_model_t::A705;
      else
        return sharp_ac_remote_model_t::A903;
    } else {
      return sharp_ac_remote_model_t::A907;
    }
  }
  return _model;
}

/// Set the value of the Power Special setting without any checks.
/// @param[in] value The value to set Power Special to.
inline void IRSharpAc::setPowerSpecial(const uint8_t value) {
  _.PowerSpecial = value;
}

/// Get the value of the Power Special setting.
/// @return The setting's value.
uint8_t IRSharpAc::getPowerSpecial(void) const {
  return _.PowerSpecial;
}

/// Clear the "special"/non-normal bits in the power section.
/// e.g. for normal/common command modes.
void IRSharpAc::clearPowerSpecial(void) {
  setPowerSpecial(_.PowerSpecial & kSharpAcPowerOn);
}

/// Is one of the special power states in use?
/// @return true, it is. false, it isn't.
bool IRSharpAc::isPowerSpecial(void) const {
  switch (_.PowerSpecial) {
    case kSharpAcPowerSetSpecialOff:
    case kSharpAcPowerSetSpecialOn:
    case kSharpAcPowerTimerSetting: return true;
    default: return false;
  }
}

/// Set the requested power state of the A/C to on.
void IRSharpAc::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void IRSharpAc::off(void) { setPower(false); }

/// Change the power setting, including the previous power state.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @param[in] prev_on true, the setting is on. false, the setting is off.
void IRSharpAc::setPower(const bool on, const bool prev_on) {
  setPowerSpecial(on ? (prev_on ? kSharpAcPowerOn : kSharpAcPowerOnFromOff)
                     : kSharpAcPowerOff);
  // Power operations are incompatible with clean mode.
  if (_.Clean) setClean(false);
  _.Special = kSharpAcSpecialPower;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRSharpAc::getPower(void) const {
  switch (_.PowerSpecial) {
    case kSharpAcPowerUnknown:
    case kSharpAcPowerOff: return false;
    default: return true;  // Everything else is "probably" on.
  }
}

/// Set the value of the Special (button/command?) setting.
/// @param[in] mode The value to set Special to.
void IRSharpAc::setSpecial(const uint8_t mode) {
  switch (mode) {
    case kSharpAcSpecialPower:
    case kSharpAcSpecialTurbo:
    case kSharpAcSpecialTempEcono:
    case kSharpAcSpecialFan:
    case kSharpAcSpecialSwing:
    case kSharpAcSpecialTimer:
    case kSharpAcSpecialTimerHalfHour:
      _.Special = mode;
      break;
    default:
      _.Special = kSharpAcSpecialPower;
  }
}

/// Get the value of the Special (button/command?) setting.
/// @return The setting's value.
uint8_t IRSharpAc::getSpecial(void) const { return _.Special; }

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
/// @param[in] save Do we save this setting as a user set one?
void IRSharpAc::setTemp(const uint8_t temp, const bool save) {
  switch (_.Mode) {
    // Auto & Dry don't allow temp changes and have a special temp.
    case kSharpAcAuto:
    case kSharpAcDry:
      _.raw[kSharpAcByteTemp] = 0;
      return;
    default:
      switch (getModel()) {
        case sharp_ac_remote_model_t::A705:
          _.raw[kSharpAcByteTemp] = 0xD0;
          break;
        default:
          _.raw[kSharpAcByteTemp] = 0xC0;
      }
  }
  uint8_t degrees = std::max(temp, kSharpAcMinTemp);
  degrees = std::min(degrees, kSharpAcMaxTemp);
  if (save) _temp = degrees;
  _.Temp = degrees - kSharpAcMinTemp;
  _.Special = kSharpAcSpecialTempEcono;
  clearPowerSpecial();
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRSharpAc::getTemp(void) const {
  return _.Temp + kSharpAcMinTemp;
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRSharpAc::getMode(void) const {
  return _.Mode;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
/// @param[in] save Do we save this setting as a user set one?
void IRSharpAc::setMode(const uint8_t mode, const bool save) {
  uint8_t realMode = mode;
  if (mode == kSharpAcHeat) {
    switch (getModel()) {
      case sharp_ac_remote_model_t::A705:
      case sharp_ac_remote_model_t::A903:
        // These models have no heat mode, use Fan mode instead.
        realMode = kSharpAcFan;
        break;
      default:
        break;
    }
  }

  switch (realMode) {
    case kSharpAcAuto:  // Also kSharpAcFan
    case kSharpAcDry:
      // When Dry or Auto, Fan always 2(Auto)
      setFan(kSharpAcFanAuto, false);
      // FALLTHRU
    case kSharpAcCool:
    case kSharpAcHeat:
      _.Mode = realMode;
      break;
    default:
      setFan(kSharpAcFanAuto, false);
      _.Mode = kSharpAcAuto;
  }
  // Dry/Auto have no temp setting. This step will enforce it.
  setTemp(_temp, false);
  // Save the mode in case we need to revert to it. eg. Clean
  if (save) _mode = _.Mode;

  _.Special = kSharpAcSpecialPower;
  clearPowerSpecial();
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
/// @param[in] save Do we save this setting as a user set one?
void IRSharpAc::setFan(const uint8_t speed, const bool save) {
  switch (speed) {
    case kSharpAcFanAuto:
    case kSharpAcFanMin:
    case kSharpAcFanMed:
    case kSharpAcFanHigh:
    case kSharpAcFanMax:
      _.Fan = speed;
      if (save) _fan = speed;
      break;
    default:
      _.Fan = kSharpAcFanAuto;
      _fan = kSharpAcFanAuto;
  }
  _.Special = kSharpAcSpecialFan;
  clearPowerSpecial();
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t IRSharpAc::getFan(void) const {
  return _.Fan;
}

/// Get the Turbo setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRSharpAc::getTurbo(void) const {
  return (_.PowerSpecial == kSharpAcPowerSetSpecialOn) &&
         (_.Special == kSharpAcSpecialTurbo);
}

/// Set the Turbo setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @note If you use this method, you will need to send it before making
///   other changes to the settings, as they may overwrite some of the bits
///   used by this setting.
void IRSharpAc::setTurbo(const bool on) {
  if (on) setFan(kSharpAcFanMax);
  setPowerSpecial(on ? kSharpAcPowerSetSpecialOn : kSharpAcPowerSetSpecialOff);
  _.Special = kSharpAcSpecialTurbo;
}

/// Get the Vertical Swing setting of the A/C.
/// @return The position of the Vertical Swing setting.
uint8_t IRSharpAc::getSwingV(void) const { return _.Swing; }

/// Set the Vertical Swing setting of the A/C.
/// @note Some positions may not work on all models.
/// @param[in] position The desired position/setting.
/// @note `setSwingV(kSharpAcSwingVLowest)` will only allow the Lowest setting
/// in Heat mode, it will default to `kSharpAcSwingVLow` otherwise.
/// If you want to set this value in other modes e.g. Cool, you must
/// use `setSwingV`s optional `force` parameter.
/// @param[in] force Do we override the safety checks and just do it?
void IRSharpAc::setSwingV(const uint8_t position, const bool force) {
  switch (position) {
    case kSharpAcSwingVCoanda:
      // Only allowed in Heat mode.
      if (!force && getMode() != kSharpAcHeat) {
        setSwingV(kSharpAcSwingVLow);  // Use the next lowest setting.
        return;
      }
      // FALLTHRU
    case kSharpAcSwingVHigh:
    case kSharpAcSwingVMid:
    case kSharpAcSwingVLow:
    case kSharpAcSwingVToggle:
    case kSharpAcSwingVOff:
    case kSharpAcSwingVLast:  // Technically valid, but we don't use it.
      // All expected non-positions set the special bits.
      _.Special = kSharpAcSpecialSwing;
      // FALLTHRU
    case kSharpAcSwingVIgnore:
      _.Swing = position;
  }
}

/// Convert a standard A/C vertical swing into its native setting.
/// @param[in] position A stdAc::swingv_t position to convert.
/// @return The equivalent native horizontal swing position.
uint8_t IRSharpAc::convertSwingV(const stdAc::swingv_t position) {
  switch (position) {
    case stdAc::swingv_t::kHighest:
    case stdAc::swingv_t::kHigh:    return kSharpAcSwingVHigh;
    case stdAc::swingv_t::kMiddle:  return kSharpAcSwingVMid;
    case stdAc::swingv_t::kLow:     return kSharpAcSwingVLow;
    case stdAc::swingv_t::kLowest:  return kSharpAcSwingVCoanda;
    case stdAc::swingv_t::kAuto:    return kSharpAcSwingVToggle;
    case stdAc::swingv_t::kOff:     return kSharpAcSwingVOff;
    default:                        return kSharpAcSwingVIgnore;
  }
}

/// Get the (vertical) Swing Toggle setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRSharpAc::getSwingToggle(void) const {
  return getSwingV() == kSharpAcSwingVToggle;
}

/// Set the (vertical) Swing Toggle setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRSharpAc::setSwingToggle(const bool on) {
  setSwingV(on ? kSharpAcSwingVToggle : kSharpAcSwingVIgnore);
  if (on) _.Special = kSharpAcSpecialSwing;
}

/// Get the Ion (Filter) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRSharpAc::getIon(void) const { return _.Ion; }

/// Set the Ion (Filter) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRSharpAc::setIon(const bool on) {
  _.Ion = on;
  clearPowerSpecial();
  if (on) _.Special = kSharpAcSpecialSwing;
}

/// Get the Economical mode toggle setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
/// @note Shares the same location as the Light setting on A705.
bool IRSharpAc::_getEconoToggle(void) const {
  return (_.PowerSpecial == kSharpAcPowerSetSpecialOn) &&
         (_.Special == kSharpAcSpecialTempEcono);
}

/// Set the Economical mode toggle setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @warning Probably incompatible with `setTurbo()`
/// @note Shares the same location as the Light setting on A705.
void IRSharpAc::_setEconoToggle(const bool on) {
  if (on) _.Special = kSharpAcSpecialTempEcono;
  setPowerSpecial(on ? kSharpAcPowerSetSpecialOn : kSharpAcPowerSetSpecialOff);
}

/// Set the Economical mode toggle setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @warning Probably incompatible with `setTurbo()`
/// @note Available on the A907 models.
void IRSharpAc::setEconoToggle(const bool on) {
  if (_model == sharp_ac_remote_model_t::A907) _setEconoToggle(on);
}

/// Get the Economical mode toggle setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
/// @note Available on the A907 models.
bool IRSharpAc::getEconoToggle(void) const {
  return _model == sharp_ac_remote_model_t::A907 && _getEconoToggle();
}

/// Set the Light mode toggle setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @warning Probably incompatible with `setTurbo()`
/// @note Not available on the A907 model.
void IRSharpAc::setLightToggle(const bool on) {
  if (_model != sharp_ac_remote_model_t::A907) _setEconoToggle(on);
}

/// Get the Light toggle setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
/// @note Not available on the A907 model.
bool IRSharpAc::getLightToggle(void) const {
  return _model != sharp_ac_remote_model_t::A907 && _getEconoToggle();
}

/// Get how long the timer is set for, in minutes.
/// @return The time in nr of minutes.
uint16_t IRSharpAc::getTimerTime(void) const {
  return _.TimerHours * kSharpAcTimerIncrement * 2 +
      ((_.Special == kSharpAcSpecialTimerHalfHour) ? kSharpAcTimerIncrement
                                                   : 0);
}

/// Is the Timer enabled?
/// @return true, the setting is on. false, the setting is off.
bool IRSharpAc::getTimerEnabled(void) const { return _.TimerEnabled; }

/// Get the current timer type.
/// @return true, It's an "On" timer. false, It's an "Off" timer.
bool IRSharpAc::getTimerType(void) const { return _.TimerType; }

/// Set or cancel the timer function.
/// @param[in] enable Is the timer to be enabled (true) or canceled(false)?
/// @param[in] timer_type An On (true) or an Off (false). Ignored if canceled.
/// @param[in] mins Nr. of minutes the timer is to be set to.
/// @note Rounds down to 30 min increments. (max: 720 mins (12h), 0 is Off)
void IRSharpAc::setTimer(bool enable, bool timer_type, uint16_t mins) {
  uint8_t half_hours = std::min(mins / kSharpAcTimerIncrement,
                                kSharpAcTimerHoursMax * 2);
  if (half_hours == 0) enable = false;
  if (!enable) {
    half_hours = 0;
    timer_type = kSharpAcOffTimerType;
  }
  _.TimerEnabled = enable;
  _.TimerType = timer_type;
  _.TimerHours = half_hours / 2;
  // Handle non-round hours.
  _.Special = (half_hours % 2) ? kSharpAcSpecialTimerHalfHour
                               : kSharpAcSpecialTimer;
  setPowerSpecial(kSharpAcPowerTimerSetting);
}

/// Get the Clean setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRSharpAc::getClean(void) const {
  return _.Clean;
}

/// Set the Economical mode toggle setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @note Officially A/C unit needs to be "Off" before clean mode can be entered
void IRSharpAc::setClean(const bool on) {
  // Clean mode appears to be just default dry mode, with an extra bit set.
  if (on) {
    setMode(kSharpAcDry, false);
    setPower(true, false);
  } else {
    // Restore the previous operation mode & fan speed.
    setMode(_mode, false);
    setFan(_fan, false);
  }
  _.Clean = on;
  clearPowerSpecial();
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRSharpAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kSharpAcCool;
    case stdAc::opmode_t::kHeat: return kSharpAcHeat;
    case stdAc::opmode_t::kDry:  return kSharpAcDry;
    // No Fan mode.
    default:                     return kSharpAcAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @param[in] model The enum of the appropriate model.
/// @return The native equivalent of the enum.
uint8_t IRSharpAc::convertFan(const stdAc::fanspeed_t speed,
                              const sharp_ac_remote_model_t model) {
  switch (model) {
    case sharp_ac_remote_model_t::A705:
    case sharp_ac_remote_model_t::A903:
      switch (speed) {
        case stdAc::fanspeed_t::kLow:    return kSharpAcFanA705Low;
        case stdAc::fanspeed_t::kMedium: return kSharpAcFanA705Med;
        default: {};  // Fall thru to the next/default clause if not the above
                      // special cases.
      }
    // FALL THRU
    default:
      switch (speed) {
        case stdAc::fanspeed_t::kMin:
        case stdAc::fanspeed_t::kLow:    return kSharpAcFanMin;
        case stdAc::fanspeed_t::kMedium: return kSharpAcFanMed;
        case stdAc::fanspeed_t::kHigh:   return kSharpAcFanHigh;
        case stdAc::fanspeed_t::kMax:    return kSharpAcFanMax;
        default:                         return kSharpAcFanAuto;
      }
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRSharpAc::toCommonMode(const uint8_t mode) const {
  switch (mode) {
    case kSharpAcCool: return stdAc::opmode_t::kCool;
    case kSharpAcHeat: return stdAc::opmode_t::kHeat;
    case kSharpAcDry:  return stdAc::opmode_t::kDry;
    case kSharpAcAuto:  // Also kSharpAcFan
      switch (getModel()) {
        case sharp_ac_remote_model_t::A705: return stdAc::opmode_t::kFan;
        default:                            return stdAc::opmode_t::kAuto;
      }
      break;
    default:           return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRSharpAc::toCommonFanSpeed(const uint8_t speed) const {
  switch (getModel()) {
    case sharp_ac_remote_model_t::A705:
    case sharp_ac_remote_model_t::A903:
      switch (speed) {
        case kSharpAcFanA705Low:  return stdAc::fanspeed_t::kLow;
        case kSharpAcFanA705Med:  return stdAc::fanspeed_t::kMedium;
      }
      // FALL-THRU
    default:
      switch (speed) {
        case kSharpAcFanMax:  return stdAc::fanspeed_t::kMax;
        case kSharpAcFanHigh: return stdAc::fanspeed_t::kHigh;
        case kSharpAcFanMed:  return stdAc::fanspeed_t::kMedium;
        case kSharpAcFanMin:  return stdAc::fanspeed_t::kMin;
        default:              return stdAc::fanspeed_t::kAuto;
      }
  }
}

/// Convert a native vertical swing postion to it's common equivalent.
/// @param[in] pos A native position to convert.
/// @param[in] mode What operating mode are we in?
/// @return The common vertical swing position.
stdAc::swingv_t IRSharpAc::toCommonSwingV(const uint8_t pos,
                                          const stdAc::opmode_t mode) const {
  switch (pos) {
    case kSharpAcSwingVHigh:   return stdAc::swingv_t::kHighest;
    case kSharpAcSwingVMid:    return stdAc::swingv_t::kMiddle;
    case kSharpAcSwingVLow:    return stdAc::swingv_t::kLow;
    case kSharpAcSwingVCoanda:  // Coanda has mode dependent positionss
      switch (mode) {
        case stdAc::opmode_t::kCool: return stdAc::swingv_t::kHighest;
        case stdAc::opmode_t::kHeat: return stdAc::swingv_t::kLowest;
        default:                     return stdAc::swingv_t::kOff;
      }
    case kSharpAcSwingVToggle: return stdAc::swingv_t::kAuto;
    default:                   return stdAc::swingv_t::kOff;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @param[in] prev Ptr to the previous state if required.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRSharpAc::toCommon(const stdAc::state_t *prev) const {
  stdAc::state_t result{};
  // Start with the previous state if given it.
  if (prev != NULL) result = *prev;
  result.protocol = decode_type_t::SHARP_AC;
  result.model = getModel();
  result.power = getPower();
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.turbo = getTurbo();
  if (getSwingV() != kSharpAcSwingVIgnore)
    result.swingv = toCommonSwingV(getSwingV(), result.mode);
  result.filter = _.Ion;
  result.econo = getEconoToggle();
  result.light = getLightToggle();
  result.clean = _.Clean;
  // Not supported.
  result.swingh = stdAc::swingh_t::kOff;
  result.quiet = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRSharpAc::toString(void) const {
  String result = "";
  const sharp_ac_remote_model_t model = getModel();
  result.reserve(170);  // Reserve some heap for the string to reduce fragging.
  result += addModelToString(decode_type_t::SHARP_AC, getModel(), false);

  result += addLabeledString(isPowerSpecial() ? String("-")
                                              : String(getPower() ? kOnStr
                                                                  : kOffStr),
                             kPowerStr);
  const uint8_t mode = _.Mode;
  result += addModeToString(
      mode,
      // Make the value invalid if the model doesn't support an Auto mode.
      (model == sharp_ac_remote_model_t::A907) ? kSharpAcAuto : 255,
      kSharpAcCool, kSharpAcHeat, kSharpAcDry, kSharpAcFan);
  result += addTempToString(getTemp());
  switch (model) {
    case sharp_ac_remote_model_t::A705:
    case sharp_ac_remote_model_t::A903:
      result += addFanToString(_.Fan, kSharpAcFanMax, kSharpAcFanA705Low,
                               kSharpAcFanAuto, kSharpAcFanAuto,
                               kSharpAcFanA705Med);
      break;
    default:
      result += addFanToString(_.Fan, kSharpAcFanMax, kSharpAcFanMin,
                               kSharpAcFanAuto, kSharpAcFanAuto,
                               kSharpAcFanMed);
  }
  if (getSwingV() == kSharpAcSwingVIgnore) {
    result += addIntToString(kSharpAcSwingVIgnore, kSwingVStr);
    result += kSpaceLBraceStr;
    result += kNAStr;
    result += ')';
  } else {
    result += addSwingVToString(
        getSwingV(), 0xFF,
        // Coanda means Highest when in Cool mode.
        (mode == kSharpAcCool) ? kSharpAcSwingVCoanda : kSharpAcSwingVToggle,
        kSharpAcSwingVHigh,
        0xFF,  // Upper Middle is unused
        kSharpAcSwingVMid,
        0xFF,  // Lower Middle is unused
        kSharpAcSwingVLow,
        kSharpAcSwingVCoanda,
        kSharpAcSwingVOff,
        // Below are unused.
        kSharpAcSwingVToggle,
        0xFF,
        0xFF);
  }
  result += addBoolToString(getTurbo(), kTurboStr);
  result += addBoolToString(_.Ion, kIonStr);
  switch (model) {
    case sharp_ac_remote_model_t::A705:
    case sharp_ac_remote_model_t::A903:
      result += addToggleToString(getLightToggle(), kLightStr);
      break;
    default:
      result += addToggleToString(getEconoToggle(), kEconoStr);
  }
  result += addBoolToString(_.Clean, kCleanStr);
  if (_.TimerEnabled)
    result += addLabeledString(minsToString(getTimerTime()),
                               _.TimerType ? kOnTimerStr : kOffTimerStr);
  return result;
}

#if DECODE_SHARP_AC
/// Decode the supplied Sharp A/C message.
/// Status: STABLE / Known working.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/638
/// @see https://github.com/ToniA/arduino-heatpumpir/blob/master/SharpHeatpumpIR.cpp
bool IRrecv::decodeSharpAc(decode_results *results, uint16_t offset,
                           const uint16_t nbits, const bool strict) {
  // Compliance
  if (strict && nbits != kSharpAcBits) return false;

  // Match Header + Data + Footer
  uint16_t used;
  used = matchGeneric(results->rawbuf + offset, results->state,
                      results->rawlen - offset, nbits,
                      kSharpAcHdrMark, kSharpAcHdrSpace,
                      kSharpAcBitMark, kSharpAcOneSpace,
                      kSharpAcBitMark, kSharpAcZeroSpace,
                      kSharpAcBitMark, kSharpAcGap, true,
                      _tolerance, kMarkExcess, false);
  if (used == 0) return false;
  offset += used;
  // Compliance
  if (strict) {
    if (!IRSharpAc::validChecksum(results->state)) return false;
  }

  // Success
  results->decode_type = SHARP_AC;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_SHARP_AC
