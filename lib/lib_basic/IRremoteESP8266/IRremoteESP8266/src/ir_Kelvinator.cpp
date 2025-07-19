// Copyright 2016 David Conran
/// @file
/// @brief Support for Kelvinator A/C protocols.
/// Code to emulate IR Kelvinator YALIF remote control unit, which should
/// control at least the following Kelvinator A/C units:
/// KSV26CRC, KSV26HRC, KSV35CRC, KSV35HRC, KSV53HRC, KSV62HRC, KSV70CRC,
/// KSV70HRC, KSV80HRC.
///
/// @note Unsupported:
///    - All Sleep modes.
///    - All Timer modes.
///    - "I Feel" button & mode.
///    - Energy Saving mode.
///    - Low Heat mode.
///    - Fahrenheit.

#include "ir_Kelvinator.h"
#include <algorithm>
#include <cstring>
#ifndef ARDUINO
#include <string>
#endif
#include "IRac.h"
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

// Constants
const uint16_t kKelvinatorTick = 85;
const uint16_t kKelvinatorHdrMarkTicks = 106;
const uint16_t kKelvinatorHdrMark = kKelvinatorHdrMarkTicks * kKelvinatorTick;
const uint16_t kKelvinatorHdrSpaceTicks = 53;
const uint16_t kKelvinatorHdrSpace = kKelvinatorHdrSpaceTicks * kKelvinatorTick;
const uint16_t kKelvinatorBitMarkTicks = 8;
const uint16_t kKelvinatorBitMark = kKelvinatorBitMarkTicks * kKelvinatorTick;
const uint16_t kKelvinatorOneSpaceTicks = 18;
const uint16_t kKelvinatorOneSpace = kKelvinatorOneSpaceTicks * kKelvinatorTick;
const uint16_t kKelvinatorZeroSpaceTicks = 6;
const uint16_t kKelvinatorZeroSpace =
    kKelvinatorZeroSpaceTicks * kKelvinatorTick;
const uint16_t kKelvinatorGapSpaceTicks = 235;
const uint16_t kKelvinatorGapSpace = kKelvinatorGapSpaceTicks * kKelvinatorTick;

const uint8_t kKelvinatorCmdFooter = 2;
const uint8_t kKelvinatorCmdFooterBits = 3;

const uint8_t kKelvinatorChecksumStart = 10;

using irutils::addBoolToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addFanToString;
using irutils::addTempToString;
using irutils::addSwingVToString;

#if SEND_KELVINATOR
/// Send a Kelvinator A/C message.
/// Status: STABLE / Known working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendKelvinator(const unsigned char data[], const uint16_t nbytes,
                            const uint16_t repeat) {
  if (nbytes < kKelvinatorStateLength)
    return;  // Not enough bytes to send a proper message.

  for (uint16_t r = 0; r <= repeat; r++) {
    // Command Block #1 (4 bytes)
    sendGeneric(kKelvinatorHdrMark, kKelvinatorHdrSpace, kKelvinatorBitMark,
                kKelvinatorOneSpace, kKelvinatorBitMark, kKelvinatorZeroSpace,
                0, 0,  // No Footer yet.
                data, 4, 38, false, 0, 50);
    // Send Footer for the command block (3 bits (b010))
    sendGeneric(0, 0,  // No Header
                kKelvinatorBitMark, kKelvinatorOneSpace, kKelvinatorBitMark,
                kKelvinatorZeroSpace, kKelvinatorBitMark, kKelvinatorGapSpace,
                kKelvinatorCmdFooter, kKelvinatorCmdFooterBits, 38, false, 0,
                50);
    // Data Block #1 (4 bytes)
    sendGeneric(0, 0,  // No header
                kKelvinatorBitMark, kKelvinatorOneSpace, kKelvinatorBitMark,
                kKelvinatorZeroSpace, kKelvinatorBitMark,
                kKelvinatorGapSpace * 2, data + 4, 4, 38, false, 0, 50);
    // Command Block #2 (4 bytes)
    sendGeneric(kKelvinatorHdrMark, kKelvinatorHdrSpace, kKelvinatorBitMark,
                kKelvinatorOneSpace, kKelvinatorBitMark, kKelvinatorZeroSpace,
                0, 0,  // No Footer yet.
                data + 8, 4, 38, false, 0, 50);
    // Send Footer for the command block (3 bits (B010))
    sendGeneric(0, 0,  // No Header
                kKelvinatorBitMark, kKelvinatorOneSpace, kKelvinatorBitMark,
                kKelvinatorZeroSpace, kKelvinatorBitMark, kKelvinatorGapSpace,
                kKelvinatorCmdFooter, kKelvinatorCmdFooterBits, 38, false, 0,
                50);
    // Data Block #2 (4 bytes)
    sendGeneric(0, 0,  // No header
                kKelvinatorBitMark, kKelvinatorOneSpace, kKelvinatorBitMark,
                kKelvinatorZeroSpace, kKelvinatorBitMark,
                kKelvinatorGapSpace * 2, data + 12, 4, 38, false, 0, 50);
  }
}
#endif  // SEND_KELVINATOR

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRKelvinatorAC::IRKelvinatorAC(const uint16_t pin, const bool inverted,
                               const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Reset the internals of the object to a known good state.
void IRKelvinatorAC::stateReset(void) {
  for (uint8_t i = 0; i < kKelvinatorStateLength; i++) _.raw[i] = 0x0;
  _.raw[3] = 0x50;
  _.raw[11] = 0x70;
}

/// Set up hardware to be able to send a message.
void IRKelvinatorAC::begin(void) { _irsend.begin(); }

/// Fix up any odd conditions for the current state.
void IRKelvinatorAC::fixup(void) {
  // X-Fan mode is only valid in COOL or DRY modes.
  if (_.Mode != kKelvinatorCool && _.Mode != kKelvinatorDry)
    setXFan(false);
  // Duplicate to the 2nd command chunk.
  _.raw[8] = _.raw[0];
  _.raw[9] = _.raw[1];
  _.raw[10] = _.raw[2];
  checksum();  // Calculate the checksums
}

#if SEND_KELVINATOR
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRKelvinatorAC::send(const uint16_t repeat) {
  _irsend.sendKelvinator(getRaw(), kKelvinatorStateLength, repeat);
}
#endif  // SEND_KELVINATOR

/// Get the raw state of the object, suitable to be sent with the appropriate
/// IRsend object method.
/// @return A PTR to the internal state.
uint8_t *IRKelvinatorAC::getRaw(void) {
  fixup();  // Ensure correct settings before sending.
  return _.raw;
}

/// Set the raw state of the object.
/// @param[in] new_code The raw state from the native IR message.
void IRKelvinatorAC::setRaw(const uint8_t new_code[]) {
  std::memcpy(_.raw, new_code, kKelvinatorStateLength);
}

/// Calculate the checksum for a given block of state.
/// @param[in] block A pointer to a block to calc the checksum of.
/// @param[in] length Length of the block array to checksum.
/// @return The calculated checksum value.
/// @note Many Bothans died to bring us this information.
uint8_t IRKelvinatorAC::calcBlockChecksum(const uint8_t *block,
                                          const uint16_t length) {
  uint8_t sum = kKelvinatorChecksumStart;
  // Sum the lower half of the first 4 bytes of this block.
  for (uint8_t i = 0; i < 4 && i < length - 1; i++, block++)
    sum += (*block & 0b1111);
  // then sum the upper half of the next 3 bytes.
  for (uint8_t i = 4; i < length - 1; i++, block++) sum += (*block >> 4);
  // Trim it down to fit into the 4 bits allowed. i.e. Mod 16.
  return sum & 0b1111;
}

/// Calculate the checksum for the internal state.
void IRKelvinatorAC::checksum(void) {
  _.Sum1 = calcBlockChecksum(_.raw);
  _.Sum2 = calcBlockChecksum(_.raw + 8);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The size of the state.
/// @return A boolean indicating if it is valid.
bool IRKelvinatorAC::validChecksum(const uint8_t state[],
                                   const uint16_t length) {
  for (uint16_t offset = 0; offset + 7 < length; offset += 8) {
    // Top 4 bits of the last byte in the block is the block's checksum.
    if (GETBITS8(state[offset + 7], kHighNibble, kNibbleSize) !=
        calcBlockChecksum(state + offset))
      return false;
  }
  return true;
}

/// Set the internal state to have the power on.
void IRKelvinatorAC::on(void) { setPower(true); }

/// Set the internal state to have the power off.
void IRKelvinatorAC::off(void) {setPower(false); }

/// Set the internal state to have the desired power.
/// @param[in] on The desired power state.
void IRKelvinatorAC::setPower(const bool on) {
  _.Power = on;
}

/// Get the power setting from the internal state.
/// @return A boolean indicating if the power setting.
bool IRKelvinatorAC::getPower(void) const {
  return _.Power;
}

/// Set the temperature setting.
/// @param[in] degrees The temperature in degrees celsius.
void IRKelvinatorAC::setTemp(const uint8_t degrees) {
  uint8_t temp = std::max(kKelvinatorMinTemp, degrees);
  temp = std::min(kKelvinatorMaxTemp, temp);
  _.Temp = temp - kKelvinatorMinTemp;
}

/// Get the current temperature setting.
/// @return Get current setting for temp. in degrees celsius.
uint8_t IRKelvinatorAC::getTemp(void) const {
  return _.Temp + kKelvinatorMinTemp;
}

/// Set the speed of the fan.
/// @param[in] speed 0 is auto, 1-5 is the speed
void IRKelvinatorAC::setFan(const uint8_t speed) {
  uint8_t fan = std::min(kKelvinatorFanMax, speed);  // Bounds check

  // Only change things if we need to.
  if (fan != _.Fan) {
    // Set the basic fan values.
    _.BasicFan = std::min(kKelvinatorBasicFanMax, fan);
    // Set the advanced(?) fan value.
    _.Fan = fan;
    // Turbo mode is turned off if we change the fan settings.
    setTurbo(false);
  }
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRKelvinatorAC::getFan(void) const {
  return _.Fan;
}

/// Get the current operation mode setting.
/// @return The current operation mode.
uint8_t IRKelvinatorAC::getMode(void) const {
  return _.Mode;
}

/// Set the desired operation mode.
/// @param[in] mode The desired operation mode.
void IRKelvinatorAC::setMode(const uint8_t mode) {
  switch (mode) {
    case kKelvinatorAuto:
    case kKelvinatorDry:
      // When the remote is set to Auto or Dry, it defaults to 25C and doesn't
      // show it.
      setTemp(kKelvinatorAutoTemp);
      // FALL-THRU
    case kKelvinatorHeat:
    case kKelvinatorCool:
    case kKelvinatorFan:
      _.Mode = mode;
      break;
    default:
      setTemp(kKelvinatorAutoTemp);
      _.Mode = kKelvinatorAuto;
      break;
  }
}

/// Set the Vertical Swing mode of the A/C.
/// @param[in] automatic Do we use the automatic setting?
/// @param[in] position The position/mode to set the vanes to.
void IRKelvinatorAC::setSwingVertical(const bool automatic,
                                      const uint8_t position) {
  _.SwingAuto = (automatic || _.SwingH);
  uint8_t new_position = position;
  if (!automatic) {
    switch (position) {
      case kKelvinatorSwingVHighest:
      case kKelvinatorSwingVUpperMiddle:
      case kKelvinatorSwingVMiddle:
      case kKelvinatorSwingVLowerMiddle:
      case kKelvinatorSwingVLowest:
        break;
      default:
        new_position = kKelvinatorSwingVOff;
    }
  } else {
    switch (position) {
      case kKelvinatorSwingVAuto:
      case kKelvinatorSwingVLowAuto:
      case kKelvinatorSwingVMiddleAuto:
      case kKelvinatorSwingVHighAuto:
        break;
      default:
        new_position = kKelvinatorSwingVAuto;
    }
  }
  _.SwingV = new_position;
}

/// Get the Vertical Swing Automatic mode setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRKelvinatorAC::getSwingVerticalAuto(void) const {
  return _.SwingV & 0b0001;
}

/// Get the Vertical Swing position setting of the A/C.
/// @return The native position/mode.
uint8_t IRKelvinatorAC::getSwingVerticalPosition(void) const {
  return _.SwingV;
}

/// Control the current horizontal swing setting.
/// @param[in] on The desired setting.
void IRKelvinatorAC::setSwingHorizontal(const bool on) {
  _.SwingH = on;
  _.SwingAuto = (on || (_.SwingV & 0b0001));
}

/// Is the horizontal swing setting on?
/// @return The current value.
bool IRKelvinatorAC::getSwingHorizontal(void) const {
  return _.SwingH;
}

/// Control the current Quiet setting.
/// @param[in] on The desired setting.
void IRKelvinatorAC::setQuiet(const bool on) {
  _.Quiet = on;
}

/// Is the Quiet setting on?
/// @return The current value.
bool IRKelvinatorAC::getQuiet(void) const {
  return _.Quiet;
}

/// Control the current Ion Filter setting.
/// @param[in] on The desired setting.
void IRKelvinatorAC::setIonFilter(const bool on) {
  _.IonFilter = on;
}

/// Is the Ion Filter setting on?
/// @return The current value.
bool IRKelvinatorAC::getIonFilter(void) const {
  return _.IonFilter;
}

/// Control the current Light setting.
/// i.e. The LED display on the A/C unit that shows the basic settings.
/// @param[in] on The desired setting.
void IRKelvinatorAC::setLight(const bool on) {
  _.Light = on;
}

/// Is the Light (Display) setting on?
/// @return The current value.
bool IRKelvinatorAC::getLight(void) const {
  return _.Light;
}

/// Control the current XFan setting.
/// This setting will cause the unit blow air after power off to dry out the
/// A/C device.
/// @note XFan mode is only valid in Cool or Dry mode.
/// @param[in] on The desired setting.
void IRKelvinatorAC::setXFan(const bool on) {
  _.XFan = on;
}

/// Is the XFan setting on?
/// @return The current value.
bool IRKelvinatorAC::getXFan(void) const {
  return _.XFan;
}

/// Control the current Turbo setting.
/// @note Turbo mode is turned off if the fan speed is changed.
/// @param[in] on The desired setting.
void IRKelvinatorAC::setTurbo(const bool on) {
  _.Turbo = on;
}

/// Is the Turbo setting on?
/// @return The current value.
bool IRKelvinatorAC::getTurbo(void) const {
  return _.Turbo;
}

/// Convert a standard A/C mode (stdAc::opmode_t) into it a native mode.
/// @param[in] mode A stdAc::opmode_t operation mode.
/// @return The native mode equivalent.
uint8_t IRKelvinatorAC::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kKelvinatorCool;
    case stdAc::opmode_t::kHeat: return kKelvinatorHeat;
    case stdAc::opmode_t::kDry:  return kKelvinatorDry;
    case stdAc::opmode_t::kFan:  return kKelvinatorFan;
    default:                     return kKelvinatorAuto;
  }
}

/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] swingv The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRKelvinatorAC::convertSwingV(const stdAc::swingv_t swingv) {
  switch (swingv) {
    case stdAc::swingv_t::kHighest: return kKelvinatorSwingVHighest;
    case stdAc::swingv_t::kHigh:    return kKelvinatorSwingVHighAuto;
    case stdAc::swingv_t::kMiddle:  return kKelvinatorSwingVMiddle;
    case stdAc::swingv_t::kLow:     return kKelvinatorSwingVLowAuto;
    case stdAc::swingv_t::kLowest:  return kKelvinatorSwingVLowest;
    default:                        return kKelvinatorSwingVAuto;
  }
}

/// Convert a native mode to it's stdAc::opmode_t equivalent.
/// @param[in] mode A native operating mode value.
/// @return The stdAc::opmode_t equivalent.
stdAc::opmode_t IRKelvinatorAC::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kKelvinatorCool: return stdAc::opmode_t::kCool;
    case kKelvinatorHeat: return stdAc::opmode_t::kHeat;
    case kKelvinatorDry:  return stdAc::opmode_t::kDry;
    case kKelvinatorFan:  return stdAc::opmode_t::kFan;
    default:              return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed to it's stdAc::fanspeed_t equivalent.
/// @param[in] speed A native fan speed value.
/// @return The stdAc::fanspeed_t equivalent.
stdAc::fanspeed_t IRKelvinatorAC::toCommonFanSpeed(const uint8_t speed) {
  return (stdAc::fanspeed_t)speed;
}

/// Convert the internal A/C object state to it's stdAc::state_t equivalent.
/// @return A stdAc::state_t containing the current settings.
stdAc::state_t IRKelvinatorAC::toCommon(void) const {
  stdAc::state_t result{};
  result.protocol = decode_type_t::KELVINATOR;
  result.model = -1;  // Unused.
  result.power = _.Power;
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.swingv = _.SwingV ? stdAc::swingv_t::kAuto : stdAc::swingv_t::kOff;
  result.swingh = _.SwingH ? stdAc::swingh_t::kAuto : stdAc::swingh_t::kOff;
  result.quiet = _.Quiet;
  result.turbo = _.Turbo;
  result.light = _.Light;
  result.filter = _.IonFilter;
  result.clean = _.XFan;
  // Not supported.
  result.econo = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

/// Convert the internal settings into a human readable string.
/// @return A String.
String IRKelvinatorAC::toString(void) const {
  String result = "";
  result.reserve(160);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(_.Power, kPowerStr, false);
  result += addModeToString(_.Mode, kKelvinatorAuto, kKelvinatorCool,
                            kKelvinatorHeat, kKelvinatorDry, kKelvinatorFan);
  result += addTempToString(getTemp());
  result += addFanToString(_.Fan, kKelvinatorFanMax, kKelvinatorFanMin,
                           kKelvinatorFanAuto, kKelvinatorFanAuto,
                           kKelvinatorBasicFanMax);
  result += addBoolToString(_.Turbo, kTurboStr);
  result += addBoolToString(_.Quiet, kQuietStr);
  result += addBoolToString(_.XFan, kXFanStr);
  result += addBoolToString(_.IonFilter, kIonStr);
  result += addBoolToString(_.Light, kLightStr);
  result += addBoolToString(_.SwingH, kSwingHStr);
  result += addSwingVToString(_.SwingV, kKelvinatorSwingVAuto,
                              kKelvinatorSwingVHighest,
                              kKelvinatorSwingVHighAuto,
                              kKelvinatorSwingVUpperMiddle,
                              kKelvinatorSwingVMiddle,
                              kKelvinatorSwingVLowerMiddle,
                              kKelvinatorSwingVLowAuto,
                              kKelvinatorSwingVLowest,
                              kKelvinatorSwingVOff,
                              kKelvinatorSwingVAuto, kKelvinatorSwingVAuto,
                              kKelvinatorSwingVAuto);
  return result;
}

#if DECODE_KELVINATOR
/// Decode the supplied Kelvinator message.
/// Status: STABLE / Known working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeKelvinator(decode_results *results, uint16_t offset,
                              const uint16_t nbits, const bool strict) {
  if (results->rawlen <=
      2 * (nbits + kKelvinatorCmdFooterBits) + (kHeader + kFooter + 1) * 2 - 1 +
      offset)
    return false;  // Can't possibly be a valid Kelvinator message.
  if (strict && nbits != kKelvinatorBits)
    return false;  // Not strictly a Kelvinator message.

  // There are two messages back-to-back in a full Kelvinator IR message
  // sequence.
  int8_t pos = 0;
  for (uint8_t s = 0; s < 2; s++) {
    match_result_t data_result;

    uint16_t used;
    // Header + Data Block #1 (32 bits)
    used = matchGeneric(results->rawbuf + offset, results->state + pos,
                        results->rawlen - offset, 32,
                        kKelvinatorHdrMark, kKelvinatorHdrSpace,
                        kKelvinatorBitMark, kKelvinatorOneSpace,
                        kKelvinatorBitMark, kKelvinatorZeroSpace,
                        0, 0, false,
                        _tolerance, kMarkExcess, false);
    if (used == 0) return false;
    offset += used;
    pos += 4;

    // Command data footer (3 bits, B010)
    data_result = matchData(
        &(results->rawbuf[offset]), kKelvinatorCmdFooterBits,
        kKelvinatorBitMark, kKelvinatorOneSpace,
        kKelvinatorBitMark, kKelvinatorZeroSpace,
        _tolerance, kMarkExcess, false);
    if (data_result.success == false) return false;
    if (data_result.data != kKelvinatorCmdFooter) return false;
    offset += data_result.used;

    // Gap + Data (Options) (32 bits)
    used = matchGeneric(results->rawbuf + offset, results->state + pos,
                        results->rawlen - offset, 32,
                        kKelvinatorBitMark, kKelvinatorGapSpace,
                        kKelvinatorBitMark, kKelvinatorOneSpace,
                        kKelvinatorBitMark, kKelvinatorZeroSpace,
                        kKelvinatorBitMark, kKelvinatorGapSpace * 2,
                        s > 0,
                        _tolerance, kMarkExcess, false);
    if (used == 0) return false;
    offset += used;
    pos += 4;
  }

  // Compliance
  if (strict) {
    // Verify the message's checksum is correct.
    if (!IRKelvinatorAC::validChecksum(results->state)) return false;
  }

  // Success
  results->decode_type = decode_type_t::KELVINATOR;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_KELVINATOR
