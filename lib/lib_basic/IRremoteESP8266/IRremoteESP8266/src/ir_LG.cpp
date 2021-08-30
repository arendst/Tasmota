// Copyright 2015 Darryl Smith
// Copyright 2015 cheaplin
// Copyright 2017-2021 David Conran

/// @file
/// @brief Support for LG protocols.
/// LG decode originally added by Darryl Smith (based on the JVC protocol)
/// LG send originally added by https://github.com/chaeplin
/// @see https://github.com/arendst/Tasmota/blob/54c2eb283a02e4287640a4595e506bc6eadbd7f2/sonoff/xdrv_05_irremote.ino#L327-438
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1513

#include "ir_LG.h"
#include <algorithm>
#include "IRac.h"
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

using irutils::addBoolToString;
using irutils::addModeToString;
using irutils::addModelToString;
using irutils::addFanToString;
using irutils::addTempToString;
using irutils::addSwingVToString;
using irutils::addIntToString;


// Constants
// Common timings
const uint16_t kLgBitMark = 550;              ///< uSeconds.
const uint16_t kLgOneSpace = 1600;            ///< uSeconds.
const uint16_t kLgZeroSpace = 550;            ///< uSeconds.
const uint16_t kLgRptSpace = 2250;            ///< uSeconds.
const uint16_t kLgMinGap = 39750;             ///< uSeconds.
const uint32_t kLgMinMessageLength = 108050;  ///< uSeconds.
// LG (28 Bit)
const uint16_t kLgHdrMark = 8500;             ///< uSeconds.
const uint16_t kLgHdrSpace = 4250;            ///< uSeconds.
// LG (32 Bit)
const uint16_t kLg32HdrMark = 4500;           ///< uSeconds.
const uint16_t kLg32HdrSpace = 4450;          ///< uSeconds.
const uint16_t kLg32RptHdrMark = 8950;        ///< uSeconds.
// LG2 (28 Bit)
const uint16_t kLg2HdrMark = 3200;            ///< uSeconds.
const uint16_t kLg2HdrSpace = 9900;           ///< uSeconds.
const uint16_t kLg2BitMark = 480;             ///< uSeconds.

const uint32_t kLgAcAKB74955603DetectionMask = 0x0000080;
const uint8_t  kLgAcChecksumSize = 4;  ///< Size in bits.
// Signature has the checksum removed, and another bit to match both Auto & Off.
const uint8_t  kLgAcSwingHOffsetSize = kLgAcChecksumSize + 1;
const uint32_t kLgAcSwingHSignature  = kLgAcSwingHOff >> kLgAcSwingHOffsetSize;
const uint32_t kLgAcVaneSwingVBase = 0x8813200;

#ifdef VANESWINGVPOS
#undef VANESWINGVPOS
#endif
#define VANESWINGVPOS(code) (code % kLgAcVaneSwingVSize)

#if SEND_LG
/// Send an LG formatted message. (LG)
/// Status: Beta / Should be working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
///   Typically kLgBits or kLg32Bits.
/// @param[in] repeat The number of times the command is to be repeated.
/// @note LG has a separate message to indicate a repeat, like NEC does.
void IRsend::sendLG(uint64_t data, uint16_t nbits, uint16_t repeat) {
  uint16_t repeatHeaderMark = 0;
  uint8_t duty = kDutyDefault;

  if (nbits >= kLg32Bits) {
    // LG 32bit protocol is near identical to Samsung except for repeats.
    sendSAMSUNG(data, nbits, 0);  // Send it as a single Samsung message.
    repeatHeaderMark = kLg32RptHdrMark;
    duty = 33;
    repeat++;
  } else {
    // LG (28-bit) protocol.
    repeatHeaderMark = kLgHdrMark;
    sendGeneric(kLgHdrMark, kLgHdrSpace, kLgBitMark, kLgOneSpace, kLgBitMark,
                kLgZeroSpace, kLgBitMark, kLgMinGap, kLgMinMessageLength, data,
                nbits, 38, true, 0,  // Repeats are handled later.
                duty);
  }

  // Repeat
  // Protocol has a mandatory repeat-specific code sent after every command.
  if (repeat)
    sendGeneric(repeatHeaderMark, kLgRptSpace, 0, 0, 0, 0,  // No data is sent.
                kLgBitMark, kLgMinGap, kLgMinMessageLength, 0, 0,  // No data.
                38, true, repeat - 1, duty);
}

/// Send an LG Variant-2 formatted message. (LG2)
/// Status: Beta / Should be working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
///   Typically kLgBits or kLg32Bits.
/// @param[in] repeat The number of times the command is to be repeated.
/// @note LG has a separate message to indicate a repeat, like NEC does.
void IRsend::sendLG2(uint64_t data, uint16_t nbits, uint16_t repeat) {
  if (nbits >= kLg32Bits) {
    // Let the original routine handle it.
    sendLG(data, nbits, repeat);  // Send it as a single Samsung message.
    return;
  }

  // LGv2 (28-bit) protocol.
  sendGeneric(kLg2HdrMark, kLg2HdrSpace, kLg2BitMark, kLgOneSpace, kLg2BitMark,
              kLgZeroSpace, kLg2BitMark, kLgMinGap, kLgMinMessageLength, data,
              nbits, 38, true, 0,  // Repeats are handled later.
              33);  // Use a duty cycle of 33% (Testing)

  // TODO(crackn): Verify the details of what repeat messages look like.
  // Repeat
  // Protocol has a mandatory repeat-specific code sent after every command.
  if (repeat)
    sendGeneric(kLg2HdrMark, kLgRptSpace, 0, 0, 0, 0,  // No data is sent.
                kLgBitMark, kLgMinGap, kLgMinMessageLength, 0, 0,  // No data.
                38, true, repeat - 1, 50);
}

/// Construct a raw 28-bit LG message code from the supplied address & command.
/// Status: STABLE / Works.
/// @param[in] address The address code.
/// @param[in] command The command code.
/// @return A raw 28-bit LG message code suitable for sendLG() etc.
/// @note Sequence of bits = address + command + checksum.
uint32_t IRsend::encodeLG(uint16_t address, uint16_t command) {
  return ((address << 20) | (command << kLgAcChecksumSize) |
          irutils::sumNibbles(command, 4));
}
#endif  // SEND_LG

#if DECODE_LG
/// Decode the supplied LG message.
/// Status: STABLE / Working.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
///   Typically kLgBits or kLg32Bits.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
/// @note LG protocol has a repeat code which is 4 items long.
///   Even though the protocol has 28/32 bits of data, only 24/28 bits are
///   distinct.
///   In transmission order, the 28/32 bits are constructed as follows:
///     8/12 bits of address + 16 bits of command + 4 bits of checksum.
/// @note LG 32bit protocol appears near identical to the Samsung protocol.
///   They possibly differ on how they repeat and initial HDR mark.
/// @see https://funembedded.wordpress.com/2014/11/08/ir-remote-control-for-lg-conditioner-using-stm32f302-mcu-on-mbed-platform/
bool IRrecv::decodeLG(decode_results *results, uint16_t offset,
                      const uint16_t nbits, const bool strict) {
  if (nbits >= kLg32Bits) {
    if (results->rawlen <= 2 * nbits + 2 * (kHeader + kFooter) - 1 + offset)
      return false;  // Can't possibly be a valid LG32 message.
  } else {
    if (results->rawlen <= 2 * nbits + kHeader - 1 + offset)
      return false;  // Can't possibly be a valid LG message.
  }
  // Compliance
  if (strict && nbits != kLgBits && nbits != kLg32Bits)
    return false;  // Doesn't comply with expected LG protocol.

  // Header (Mark)
  uint32_t kHdrSpace;
  if (matchMark(results->rawbuf[offset], kLgHdrMark))
    kHdrSpace = kLgHdrSpace;
  else if (matchMark(results->rawbuf[offset], kLg2HdrMark))
    kHdrSpace = kLg2HdrSpace;
  else if (matchMark(results->rawbuf[offset], kLg32HdrMark))
    kHdrSpace = kLg32HdrSpace;
  else
    return false;
  offset++;

  // Set up the expected data section values.
  const uint16_t kBitmark = (kHdrSpace == kLg2HdrSpace) ? kLg2BitMark
                                                        : kLgBitMark;
  // Header Space + Data + Footer
  uint64_t data = 0;
  uint16_t used = matchGeneric(results->rawbuf + offset, &data,
                               results->rawlen - offset, nbits,
                               0,  // Already matched the Header mark.
                               kHdrSpace,
                               kBitmark, kLgOneSpace, kBitmark, kLgZeroSpace,
                               kBitmark, kLgMinGap, true, kUseDefTol, 0, true);
  if (!used) return false;
  offset += used;

  // Repeat
  if (nbits >= kLg32Bits) {
    // If we are expecting the LG 32-bit protocol, there is always
    // a repeat message. So, check for it.
    uint64_t unused;
    if (!matchGeneric(results->rawbuf + offset, &unused,
                      results->rawlen - offset, 0,  // No Data bits to match.
                      kLg32RptHdrMark, kLgRptSpace,
                      kBitmark, kLgOneSpace, kBitmark, kLgZeroSpace,
                      kBitmark, kLgMinGap, true, kUseDefTol)) return false;
  }

  // The 16 bits before the checksum.
  uint16_t command = (data >> kLgAcChecksumSize);

  // Compliance
  if (strict && (data & 0xF) != irutils::sumNibbles(command, 4))
    return false;  // The last 4 bits sent are the expected checksum.
  // Success
  if (kHdrSpace == kLg2HdrSpace)  // Was it an LG2 message?
    results->decode_type = LG2;
  else
    results->decode_type = LG;
  results->bits = nbits;
  results->value = data;
  results->command = command;
  results->address = data >> 20;  // The bits before the command.
  return true;
}
#endif  // DECODE_LG

// LG A/C Class

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRLgAc::IRLgAc(const uint16_t pin, const bool inverted,
               const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Reset the internals of the object to a known good state.
void IRLgAc::stateReset(void) {
  setRaw(kLgAcOffCommand);
  setModel(lg_ac_remote_model_t::GE6711AR2853M);
  _light = true;
  _swingv = kLgAcSwingVOff;
  _swingh = false;
  for (uint8_t i = 0; i < kLgAcSwingVMaxVanes; i++)
    _vaneswingv[i] = 0;  // Reset to an unused value.
  updateSwingPrev();
}

/// Set up hardware to be able to send a message.
void IRLgAc::begin(void) { _irsend.begin(); }

#if SEND_LG
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRLgAc::send(const uint16_t repeat) {
  if (getPower()) {
    _irsend.send(_protocol, getRaw(), kLgBits, repeat);
    // Some models have extra/special settings & controls
    switch (getModel()) {
      case lg_ac_remote_model_t::AKB74955603:
        // Only send the swing setting if we need to.
        if (_swingv != _swingv_prev)
          _irsend.send(_protocol, _swingv, kLgBits, repeat);
        // Any "normal" command sent will always turn the light on, thus we only
        // send it when we want it off. Must be sent last!
        // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1513#issuecomment-877283080
        if (!_light) _irsend.send(_protocol, kLgAcLightToggle, kLgBits, repeat);
        break;
      case lg_ac_remote_model_t::AKB73757604:
        // Check if we need to send any vane specific swingv's.
        for (uint8_t i = 0; i < kLgAcSwingVMaxVanes; i++)  // For all vanes
          if (_vaneswingv[i] != _vaneswingv_prev[i])  // Only send if we must.
            _irsend.send(_protocol, calcVaneSwingV(i, _vaneswingv[i]), kLgBits,
                         repeat);
        // and if we need to send a swingh message.
        if (_swingh != _swingh_prev)
          _irsend.send(_protocol, _swingh ? kLgAcSwingHAuto : kLgAcSwingHOff,
                       kLgBits, repeat);
        break;
      default:
        break;
    }
    updateSwingPrev();  // Swing changes will have been sent, so make them prev.
  } else {
    // Always send the special Off command if the power is set to off.
    // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1008#issuecomment-570763580
    _irsend.send(_protocol, kLgAcOffCommand, kLgBits, repeat);
  }
}
#endif  // SEND_LG

/// Is the current message a normal (non-special) message?
/// @return True, if it is a normal message, False, if it is special.
bool IRLgAc::_isNormal(void) const {
  switch (_.raw) {
    case kLgAcOffCommand:
    case kLgAcLightToggle:
      return false;
  }
  if (isSwing()) return false;
  return true;
}

/// Set the model of the A/C to emulate.
/// @param[in] model The enum of the appropriate model.
void IRLgAc::setModel(const lg_ac_remote_model_t model) {
  switch (model) {
    case lg_ac_remote_model_t::AKB75215403:
    case lg_ac_remote_model_t::AKB74955603:
    case lg_ac_remote_model_t::AKB73757604:
      _protocol = decode_type_t::LG2;
      break;
    case lg_ac_remote_model_t::GE6711AR2853M:
      _protocol = decode_type_t::LG;
      break;
    default:
      return;
  }
  _model = model;
}

/// Get the model of the A/C.
/// @return The enum of the compatible model.
lg_ac_remote_model_t IRLgAc::getModel(void) const {
  return _model;
}

/// Check if the stored code must belong to a AKB74955603 model.
/// @return true, if it is AKB74955603 message. Otherwise, false.
/// @note Internal use only.
bool IRLgAc::_isAKB74955603(void) const {
  return ((_.raw & kLgAcAKB74955603DetectionMask) && _isNormal()) ||
      isSwingV() || isLightToggle();
}

/// Check if the stored code must belong to a AKB73757604 model.
/// @return true, if it is AKB73757604 message. Otherwise, false.
/// @note Internal use only.
bool IRLgAc::_isAKB73757604(void) const {
  return isSwingH() || isVaneSwingV();
}

/// Get a copy of the internal state/code for this protocol.
/// @return The code for this protocol based on the current internal state.
uint32_t IRLgAc::getRaw(void) {
  checksum();
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
/// @param[in] protocol A valid decode protocol type to use.
void IRLgAc::setRaw(const uint32_t new_code, const decode_type_t protocol) {
  _.raw = new_code;
  // Set the default model for this protocol, if the protocol is supplied.
  switch (protocol) {
    case decode_type_t::LG:
      setModel(lg_ac_remote_model_t::GE6711AR2853M);
      break;
    case decode_type_t::LG2:
      setModel(lg_ac_remote_model_t::AKB75215403);
      break;
    default:
      // Don't change anything if it isn't an expected protocol.
      break;
  }
  // Look for model specific settings/features to improve model detection.
  if (_isAKB74955603()) {
    setModel(lg_ac_remote_model_t::AKB74955603);
    if (isSwingV()) _swingv = new_code;
  }
  if (_isAKB73757604()) {
    setModel(lg_ac_remote_model_t::AKB73757604);
    if (isVaneSwingV()) {
      // Extract just the vane nr and position part of the message.
      const uint32_t vanecode = getVaneCode(_.raw);
      _vaneswingv[vanecode / kLgAcVaneSwingVSize] = VANESWINGVPOS(vanecode);
    } else if (isSwingH()) {
      _swingh = (_.raw == kLgAcSwingHAuto);
    }
  }
  _temp = 15;  // Ensure there is a "sane" previous temp.
  _temp = getTemp();
}

/// Calculate the checksum for a given state.
/// @param[in] state The value to calc the checksum of.
/// @return The calculated checksum value.
uint8_t IRLgAc::calcChecksum(const uint32_t state) {
  return irutils::sumNibbles(state >> kLgAcChecksumSize, 4);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The value to verify the checksum of.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRLgAc::validChecksum(const uint32_t state) {
  LGProtocol LGp;
  LGp.raw = state;
  return calcChecksum(state) == LGp.Sum;
}

/// Calculate and set the checksum values for the internal state.
void IRLgAc::checksum(void) {
  _.Sum = calcChecksum(_.raw);
}

/// Change the power setting to On.
void IRLgAc::on(void) { setPower(true); }

/// Change the power setting to Off.
void IRLgAc::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRLgAc::setPower(const bool on) {
  _.Power = (on ? kLgAcPowerOn : kLgAcPowerOff);
  if (on)
    setTemp(_temp);  // Reset the temp if we are on.
  else
    _setTemp(0);  // Off clears the temp.
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRLgAc::getPower(void) const {
  return _.Power == kLgAcPowerOn;
}

/// Is the message a Power Off message?
/// @return true, if it is. false, if not.
bool IRLgAc::isOffCommand(void) const { return _.raw == kLgAcOffCommand; }

/// Change the light/led/display setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRLgAc::setLight(const bool on) { _light = on; }

/// Get the value of the current light setting.
/// @return true, the setting is on. false, the setting is off.
bool IRLgAc::getLight(void) const { return _light; }

/// Is the message a Light Toggle message?
/// @return true, if it is. false, if not.
bool IRLgAc::isLightToggle(void) const { return _.raw == kLgAcLightToggle; }

/// Set the temperature.
/// @param[in] value The native temperature.
/// @note Internal use only.
inline void IRLgAc::_setTemp(const uint8_t value) { _.Temp = value; }

/// Set the temperature.
/// @param[in] degrees The temperature in degrees celsius.
void IRLgAc::setTemp(const uint8_t degrees) {
  uint8_t temp = std::max(kLgAcMinTemp, degrees);
  temp = std::min(kLgAcMaxTemp, temp);
  _temp = temp;
  _setTemp(temp - kLgAcTempAdjust);
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRLgAc::getTemp(void) const {
  return _isNormal() ? _.Temp + kLgAcTempAdjust : _temp;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRLgAc::setFan(const uint8_t speed) {
  uint8_t _speed = speed;
  // Only model AKB74955603 has these speeds, so convert if we have to.
  if (getModel() != lg_ac_remote_model_t::AKB74955603) {
    switch (speed) {
      case kLgAcFanLowAlt:
        _.Fan = kLgAcFanLow;
        return;
      case kLgAcFanHigh:
        _.Fan = kLgAcFanMax;
        return;
    }
  }
  switch (speed) {
    case kLgAcFanLow:
    case kLgAcFanLowAlt:
      _speed = (getModel() != lg_ac_remote_model_t::AKB74955603)
          ? kLgAcFanLow : kLgAcFanLowAlt;
      break;
    case kLgAcFanHigh:
      _speed = (getModel() != lg_ac_remote_model_t::AKB74955603)
          ? kLgAcFanMax : speed;
      break;
    case kLgAcFanAuto:
    case kLgAcFanLowest:
    case kLgAcFanMedium:
    case kLgAcFanMax:
      _speed = speed;
      break;
    default:
      _speed = kLgAcFanAuto;
  }
  _.Fan = _speed;
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRLgAc::getFan(void) const { return _.Fan; }

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRLgAc::getMode(void) const { return _.Mode; }

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRLgAc::setMode(const uint8_t mode) {
  switch (mode) {
    case kLgAcAuto:
    case kLgAcDry:
    case kLgAcHeat:
    case kLgAcCool:
    case kLgAcFan:
      _.Mode = mode;
      break;
    default:
      _.Mode = kLgAcAuto;
  }
}

/// Check if the stored code is a Swing message.
/// @return true, if it is. Otherwise, false.
bool IRLgAc::isSwing(void) const {
  return (_.raw >> 12) == kLgAcSwingSignature;
}

/// Check if the stored code is a non-vane SwingV message.
/// @return true, if it is. Otherwise, false.
bool IRLgAc::isSwingV(void) const {
  const uint32_t code = _.raw >> kLgAcChecksumSize;
  return code >= (kLgAcSwingVLowest >> kLgAcChecksumSize) &&
      code < (kLgAcSwingHAuto >> kLgAcChecksumSize);
}

/// Check if the stored code is a SwingH message.
/// @return true, if it is. Otherwise, false.
bool IRLgAc::isSwingH(void) const {
  return (_.raw >> kLgAcSwingHOffsetSize) == kLgAcSwingHSignature;
}

/// Get the Horizontal Swing position setting of the A/C.
/// @return true, if it is. Otherwise, false.
bool IRLgAc::getSwingH(void) const { return _swingh; }

/// Set the Horizontal Swing mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRLgAc::setSwingH(const bool on) { _swingh = on; }

/// Check if the stored code is a vane specific SwingV message.
/// @return true, if it is. Otherwise, false.
bool IRLgAc::isVaneSwingV(void) const {
  return _.raw > kLgAcVaneSwingVBase &&
      _.raw < (kLgAcVaneSwingVBase +
               ((kLgAcSwingVMaxVanes *
                 kLgAcVaneSwingVSize) << kLgAcChecksumSize));
}

/// Set the Vertical Swing mode of the A/C.
/// @param[in] position The position/mode to set the vanes to.
void IRLgAc::setSwingV(const uint32_t position) {
  // Is it a valid position code?
  if (position == kLgAcSwingVOff ||
      toCommonSwingV(position) != stdAc::swingv_t::kOff) {
    if (position <= 0xFF) {  // It's a short code, convert it.
      _swingv = (kLgAcSwingSignature << 8 | position) << kLgAcChecksumSize;
      _swingv |= calcChecksum(_swingv);
    } else {
      _swingv = position;
    }
  }
}

// Copy the previous swing settings from the current ones.
void IRLgAc::updateSwingPrev(void) {
  _swingv_prev = _swingv;
  for (uint8_t i = 0; i < kLgAcSwingVMaxVanes; i++)
    _vaneswingv_prev[i] = _vaneswingv[i];
}

/// Get the Vertical Swing position setting of the A/C.
/// @return The native position/mode.
uint32_t IRLgAc::getSwingV(void) const { return _swingv; }

/// Set the per Vane Vertical Swing mode of the A/C.
/// @param[in] vane The nr. of the vane to control.
/// @param[in] position The position/mode to set the vanes to.
void IRLgAc::setVaneSwingV(const uint8_t vane, const uint8_t position) {
  if (vane < kLgAcSwingVMaxVanes)  // It's a valid vane nr.
    if (position && position <= kLgAcVaneSwingVLowest)  // Valid position
      _vaneswingv[vane] = position;
}

/// Get the Vertical Swing position for the given vane of the A/C.
/// @return The native position/mode.
uint8_t IRLgAc::getVaneSwingV(const uint8_t vane) const {
  return (vane < kLgAcSwingVMaxVanes) ? _vaneswingv[vane] : 0;
}

/// Get the vane code of a Vane Vertical Swing message.
/// @param[in] raw A raw number representing a native LG message.
/// @return A number containing just the vane nr, and the position.
uint8_t IRLgAc::getVaneCode(const uint32_t raw) {
  return (raw - kLgAcVaneSwingVBase) >> kLgAcChecksumSize;
}

/// Calculate the Vane specific Vertical Swing code for the A/C.
/// @return The native raw code.
uint32_t IRLgAc::calcVaneSwingV(const uint8_t vane, const uint8_t position) {
  uint32_t result = kLgAcVaneSwingVBase;
  if (vane < kLgAcSwingVMaxVanes)  // It's a valid vane nr.
    if (position && position <= kLgAcVaneSwingVLowest)  // Valid position
      result += ((vane * kLgAcVaneSwingVSize + position) << kLgAcChecksumSize);
  return result | calcChecksum(result);
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRLgAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kLgAcCool;
    case stdAc::opmode_t::kHeat: return kLgAcHeat;
    case stdAc::opmode_t::kFan:  return kLgAcFan;
    case stdAc::opmode_t::kDry:  return kLgAcDry;
    default:                     return kLgAcAuto;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRLgAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kLgAcCool: return stdAc::opmode_t::kCool;
    case kLgAcHeat: return stdAc::opmode_t::kHeat;
    case kLgAcDry:  return stdAc::opmode_t::kDry;
    case kLgAcFan:  return stdAc::opmode_t::kFan;
    default:        return stdAc::opmode_t::kAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRLgAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:    return kLgAcFanLowest;
    case stdAc::fanspeed_t::kLow:    return kLgAcFanLow;
    case stdAc::fanspeed_t::kMedium: return kLgAcFanMedium;
    case stdAc::fanspeed_t::kHigh:   return kLgAcFanHigh;
    case stdAc::fanspeed_t::kMax:    return kLgAcFanMax;
    default:                         return kLgAcFanAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRLgAc::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kLgAcFanMax:     return stdAc::fanspeed_t::kMax;
    case kLgAcFanHigh:    return stdAc::fanspeed_t::kHigh;
    case kLgAcFanMedium:  return stdAc::fanspeed_t::kMedium;
    case kLgAcFanLow:
    case kLgAcFanLowAlt:  return stdAc::fanspeed_t::kLow;
    case kLgAcFanLowest:  return stdAc::fanspeed_t::kMin;
    default:              return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] swingv The enum to be converted.
/// @return The native equivalent of the enum.
uint32_t IRLgAc::convertSwingV(const stdAc::swingv_t swingv) {
  switch (swingv) {
    case stdAc::swingv_t::kHighest: return kLgAcSwingVHighest;
    case stdAc::swingv_t::kHigh:    return kLgAcSwingVHigh;
    case stdAc::swingv_t::kMiddle:  return kLgAcSwingVMiddle;
    case stdAc::swingv_t::kLow:     return kLgAcSwingVLow;
    case stdAc::swingv_t::kLowest:  return kLgAcSwingVLowest;
    case stdAc::swingv_t::kAuto:    return kLgAcSwingVSwing;
    default:                        return kLgAcSwingVOff;
  }
}

/// Convert a native Vertical Swing into its stdAc equivalent.
/// @param[in] code The native code to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::swingv_t IRLgAc::toCommonSwingV(const uint32_t code) {
  switch (code) {
    case kLgAcSwingVHighest_Short:
    case kLgAcSwingVHighest: return stdAc::swingv_t::kHighest;
    case kLgAcSwingVHigh_Short:
    case kLgAcSwingVHigh:    return stdAc::swingv_t::kHigh;
    case kLgAcSwingVUpperMiddle_Short:
    case kLgAcSwingVUpperMiddle:
    case kLgAcSwingVMiddle_Short:
    case kLgAcSwingVMiddle:  return stdAc::swingv_t::kMiddle;
    case kLgAcSwingVLow_Short:
    case kLgAcSwingVLow:     return stdAc::swingv_t::kLow;
    case kLgAcSwingVLowest_Short:
    case kLgAcSwingVLowest:  return stdAc::swingv_t::kLowest;
    case kLgAcSwingVSwing_Short:
    case kLgAcSwingVSwing:   return stdAc::swingv_t::kAuto;
    default:                 return stdAc::swingv_t::kOff;
  }
}

/// Convert a native Vane specific Vertical Swing into its stdAc equivalent.
/// @param[in] pos The native position to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::swingv_t IRLgAc::toCommonVaneSwingV(const uint8_t pos) {
  switch (pos) {
    case kLgAcVaneSwingVHigh:    return stdAc::swingv_t::kHigh;
    case kLgAcVaneSwingVUpperMiddle:
    case kLgAcVaneSwingVMiddle:  return stdAc::swingv_t::kMiddle;
    case kLgAcVaneSwingVLow:     return stdAc::swingv_t::kLow;
    case kLgAcVaneSwingVLowest:  return stdAc::swingv_t::kLowest;
    default:                     return stdAc::swingv_t::kHighest;
  }
}

/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] swingv The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRLgAc::convertVaneSwingV(const stdAc::swingv_t swingv) {
  switch (swingv) {
    case stdAc::swingv_t::kHigh:    return kLgAcVaneSwingVHigh;
    case stdAc::swingv_t::kMiddle:  return kLgAcVaneSwingVMiddle;
    case stdAc::swingv_t::kLow:     return kLgAcVaneSwingVLow;
    case stdAc::swingv_t::kLowest:  return kLgAcVaneSwingVLowest;
    default:                        return kLgAcVaneSwingVHighest;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @param[in] prev Ptr to the previous state if required.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRLgAc::toCommon(const stdAc::state_t *prev) const {
  stdAc::state_t result;
  // Start with the previous state if given it.
  if (prev != NULL) {
    result = *prev;
  } else {
    // Set defaults for non-zero values that are not implicitly set for when
    // there is no previous state.
    // e.g. Any setting that toggles should probably go here.
    result.light = true;
    result.swingv = toCommonSwingV(getSwingV());
  }
  result.protocol = _protocol;
  result.model = getModel();
  result.power = getPower();
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.light = isLightToggle() ? !result.light : _light;
  if (isSwingV()) result.swingv = toCommonSwingV(getSwingV());
  if (isVaneSwingV())
    result.swingv = toCommonVaneSwingV(VANESWINGVPOS(getVaneCode(_.raw)));
  result.swingh = isSwingH() ? stdAc::swingh_t::kAuto : stdAc::swingh_t::kOff;
  // Not supported.
  result.quiet = false;
  result.turbo = false;
  result.filter = false;
  result.clean = false;
  result.econo = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRLgAc::toString(void) const {
  String result = "";
  result.reserve(80);  // Reserve some heap for the string to reduce fragging.
  result += addModelToString(_protocol, getModel(), false);
  if (_isNormal()) {  // A "Normal" generic settings message.
    result += addBoolToString(getPower(), kPowerStr);
    if (getPower()) {  // Only display the rest if is in power on state.
      result += addModeToString(_.Mode, kLgAcAuto, kLgAcCool,
                                kLgAcHeat, kLgAcDry, kLgAcFan);
      result += addTempToString(getTemp());
      result += addFanToString(_.Fan, kLgAcFanHigh,
                               _isAKB74955603() ? kLgAcFanLowAlt : kLgAcFanLow,
                               kLgAcFanAuto, kLgAcFanLowest, kLgAcFanMedium,
                               kLgAcFanMax);
    }
  } else {  // It must be a special single purpose code.
    if (isOffCommand()) {
      result += addBoolToString(false, kPowerStr);
    } else if (isLightToggle()) {
      result += addBoolToString(true, kLightToggleStr);
    } else if (isSwingH()) {
      result += addBoolToString(_swingh, kSwingHStr);
    } else if (isSwingV()) {
      result += addSwingVToString((uint8_t)(_swingv >> kLgAcChecksumSize),
                                  0,  // No Auto, See "swing". Unused
                                  kLgAcSwingVHighest_Short,
                                  kLgAcSwingVHigh_Short,
                                  kLgAcSwingVUpperMiddle_Short,
                                  kLgAcSwingVMiddle_Short,
                                  0,  // Unused
                                  kLgAcSwingVLow_Short,
                                  kLgAcSwingVLowest_Short,
                                  kLgAcSwingVOff_Short,
                                  kLgAcSwingVSwing_Short,
                                  0, 0);
    } else if (isVaneSwingV()) {
      const uint8_t vane = getVaneCode(_.raw) / kLgAcVaneSwingVSize;
      result += addIntToString(vane, kVaneStr);
      result += addSwingVToString(_vaneswingv[vane],
                                  0,  // No Auto, See "swing". Unused
                                  kLgAcVaneSwingVHighest,
                                  kLgAcVaneSwingVHigh,
                                  kLgAcVaneSwingVUpperMiddle,
                                  kLgAcVaneSwingVMiddle,
                                  0,  // Unused
                                  kLgAcVaneSwingVLow,
                                  kLgAcVaneSwingVLowest,
                                  // Rest unused
                                  0, 0, 0, 0);
    }
  }
  return result;
}

/// Check if the internal state looks like a valid LG A/C message.
/// @return true, the internal state is a valid LG A/C mesg. Otherwise, false.
bool IRLgAc::isValidLgAc(void) const {
  return validChecksum(_.raw) && (_.Sign == kLgAcSignature);
}
