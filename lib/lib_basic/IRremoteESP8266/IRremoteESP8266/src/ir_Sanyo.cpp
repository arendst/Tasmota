// Copyright 2009 Ken Shirriff
// Copyright 2016 marcosamarinho
// Copyright 2017-2021 David Conran

/// @file
/// @brief Support for Sanyo protocols.
/// Sanyo LC7461 support originally by marcosamarinho
/// Sanyo SA 8650B originally added from
///   https://github.com/shirriff/Arduino-IRremote/
/// @see https://github.com/z3t0/Arduino-IRremote/blob/master/ir_Sanyo.cpp
/// @see http://pdf.datasheetcatalog.com/datasheet/sanyo/LC7461.pdf
/// @see https://github.com/marcosamarinho/IRremoteESP8266/blob/master/ir_Sanyo.cpp
/// @see http://slydiman.narod.ru/scr/kb/sanyo.htm
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1211
/// @see https://docs.google.com/spreadsheets/d/1dYfLsnYvpjV-SgO8pdinpfuBIpSzm8Q1R5SabrLeskw/edit?usp=sharing
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1503

#include "ir_Sanyo.h"
#include <algorithm>
#include <cstring>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

using irutils::addBoolToString;
using irutils::addFanToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addSwingVToString;
using irutils::addTempToString;
using irutils::minsToString;
using irutils::sumNibbles;

// Constants
// Sanyo SA 8650B
const uint16_t kSanyoSa8650bHdrMark = 3500;  // seen range 3500
const uint16_t kSanyoSa8650bHdrSpace = 950;  // seen 950
const uint16_t kSanyoSa8650bOneMark = 2400;  // seen 2400
const uint16_t kSanyoSa8650bZeroMark = 700;  // seen 700
// usually see 713 - not using ticks as get number wrapround
const uint16_t kSanyoSa8650bDoubleSpaceUsecs = 800;
const uint16_t kSanyoSa8650bRptLength = 45000;

// Sanyo LC7461
const uint16_t kSanyoLc7461AddressMask = (1 << kSanyoLC7461AddressBits) - 1;
const uint16_t kSanyoLc7461CommandMask = (1 << kSanyoLC7461CommandBits) - 1;
const uint16_t kSanyoLc7461HdrMark = 9000;
const uint16_t kSanyoLc7461HdrSpace = 4500;
const uint16_t kSanyoLc7461BitMark = 560;    // 1T
const uint16_t kSanyoLc7461OneSpace = 1690;  // 3T
const uint16_t kSanyoLc7461ZeroSpace = 560;  // 1T
const uint32_t kSanyoLc7461MinCommandLength = 108000;

const uint16_t kSanyoLc7461MinGap =
    kSanyoLc7461MinCommandLength -
    (kSanyoLc7461HdrMark + kSanyoLc7461HdrSpace +
     kSanyoLC7461Bits * (kSanyoLc7461BitMark +
                         (kSanyoLc7461OneSpace + kSanyoLc7461ZeroSpace) / 2) +
     kSanyoLc7461BitMark);

const uint16_t kSanyoAcHdrMark = 8500;   ///< uSeconds
const uint16_t kSanyoAcHdrSpace = 4200;  ///< uSeconds
const uint16_t kSanyoAcBitMark = 500;    ///< uSeconds
const uint16_t kSanyoAcOneSpace = 1600;  ///< uSeconds
const uint16_t kSanyoAcZeroSpace = 550;  ///< uSeconds
const uint32_t kSanyoAcGap = kDefaultMessageGap;  ///< uSeconds (Guess only)
const uint16_t kSanyoAcFreq = 38000;  ///< Hz. (Guess only)

const uint16_t kSanyoAc88HdrMark = 5400;   ///< uSeconds
const uint16_t kSanyoAc88HdrSpace = 2000;  ///< uSeconds
const uint16_t kSanyoAc88BitMark = 500;    ///< uSeconds
const uint16_t kSanyoAc88OneSpace = 1500;  ///< uSeconds
const uint16_t kSanyoAc88ZeroSpace = 750;  ///< uSeconds
const uint32_t kSanyoAc88Gap = 3675;       ///< uSeconds
const uint16_t kSanyoAc88Freq = 38000;     ///< Hz. (Guess only)
const uint8_t  kSanyoAc88ExtraTolerance = 5;  /// (%) Extra tolerance to use.

const uint16_t kSanyoAc152HdrMark = 3300;   ///< uSeconds
const uint16_t kSanyoAc152BitMark = 440;    ///< uSeconds
const uint16_t kSanyoAc152HdrSpace = 1725;  ///< uSeconds
const uint16_t kSanyoAc152OneSpace = 1290;  ///< uSeconds
const uint16_t kSanyoAc152ZeroSpace = 405;  ///< uSeconds
const uint16_t kSanyoAc152Freq = 38000;     ///< Hz. (Guess only)
const uint8_t  kSanyoAc152ExtraTolerance = 13;  /// (%) Extra tolerance to use.

#if SEND_SANYO
/// Construct a Sanyo LC7461 message.
/// @param[in] address The 13 bit value of the address(Custom) portion of the
///   protocol.
/// @param[in] command The 8 bit value of the command(Key) portion of the
///   protocol.
/// @return An uint64_t with the encoded raw 42 bit Sanyo LC7461 data value.
/// @note This protocol uses the NEC protocol timings. However, data is
///  formatted as : address(13 bits), !address, command(8 bits), !command.
///  According with LIRC, this protocol is used on Sanyo, Aiwa and Chinon
uint64_t IRsend::encodeSanyoLC7461(uint16_t address, uint8_t command) {
  // Mask our input values to ensure the correct bit sizes.
  address &= kSanyoLc7461AddressMask;
  command &= kSanyoLc7461CommandMask;

  uint64_t data = address;
  address ^= kSanyoLc7461AddressMask;  // Invert the 13 LSBs.
  // Append the now inverted address.
  data = (data << kSanyoLC7461AddressBits) | address;
  // Append the command.
  data = (data << kSanyoLC7461CommandBits) | command;
  command ^= kSanyoLc7461CommandMask;  // Invert the command.
  // Append the now inverted command.
  data = (data << kSanyoLC7461CommandBits) | command;

  return data;
}

/// Send a Sanyo LC7461 message.
/// Status: BETA / Probably works.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @note Based on \@marcosamarinho's work.
///   This protocol uses the NEC protocol timings. However, data is
///   formatted as : address(13 bits), !address, command (8 bits), !command.
///   According with LIRC, this protocol is used on Sanyo, Aiwa and Chinon
///   Information for this protocol is available at the Sanyo LC7461 datasheet.
///   Repeats are performed similar to the NEC method of sending a special
///   repeat message, rather than duplicating the entire message.
/// @see https://github.com/marcosamarinho/IRremoteESP8266/blob/master/ir_Sanyo.cpp
/// @see http://pdf.datasheetcatalog.com/datasheet/sanyo/LC7461.pdf
void IRsend::sendSanyoLC7461(const uint64_t data, const uint16_t nbits,
                             const uint16_t repeat) {
  // This protocol appears to be another 42-bit variant of the NEC protocol.
  sendNEC(data, nbits, repeat);
}
#endif  // SEND_SANYO

#if DECODE_SANYO
/// Decode the supplied SANYO LC7461 message.
/// Status: BETA / Probably works.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
/// @note Based on \@marcosamarinho's work.
///   This protocol uses the NEC protocol. However, data is
///   formatted as : address(13 bits), !address, command (8 bits), !command.
///   According with LIRC, this protocol is used on Sanyo, Aiwa and Chinon
///   Information for this protocol is available at the Sanyo LC7461 datasheet.
/// @see http://slydiman.narod.ru/scr/kb/sanyo.htm
/// @see https://github.com/marcosamarinho/IRremoteESP8266/blob/master/ir_Sanyo.cpp
/// @see http://pdf.datasheetcatalog.com/datasheet/sanyo/LC7461.pdf
bool IRrecv::decodeSanyoLC7461(decode_results *results, uint16_t offset,
                               const uint16_t nbits, const bool strict) {
  if (strict && nbits != kSanyoLC7461Bits)
    return false;  // Not strictly in spec.
  // This protocol is basically a 42-bit variant of the NEC protocol.
  if (!decodeNEC(results, offset, nbits, false))
    return false;  // Didn't match a NEC format (without strict)

  // Bits 30 to 42+.
  uint16_t address =
      results->value >> (kSanyoLC7461Bits - kSanyoLC7461AddressBits);
  // Bits 9 to 16.
  uint8_t command =
      (results->value >> kSanyoLC7461CommandBits) & kSanyoLc7461CommandMask;
  // Compliance
  if (strict) {
    if (results->bits != nbits) return false;
    // Bits 17 to 29.
    uint16_t inverted_address =
        (results->value >> (kSanyoLC7461CommandBits * 2)) &
        kSanyoLc7461AddressMask;
    // Bits 1-8.
    uint8_t inverted_command = results->value & kSanyoLc7461CommandMask;
    if ((address ^ kSanyoLc7461AddressMask) != inverted_address)
      return false;  // Address integrity check failed.
    if ((command ^ kSanyoLc7461CommandMask) != inverted_command)
      return false;  // Command integrity check failed.
  }

  // Success
  results->decode_type = SANYO_LC7461;
  results->address = address;
  results->command = command;
  return true;
}

/* NOTE: Disabled due to poor quality.
/// Decode the supplied Sanyo SA 8650B message.
/// Status: Depricated.
/// @depricated Disabled due to poor quality.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
/// @warning This decoder looks like rubbish. Only keeping it for compatibility
///   with the Arduino IRremote library. Seriously, don't trust it.
///   If someone has a device that this is supposed to be for, please log an
///   Issue on github with a rawData dump please. We should probably remove it.
///   We think this is a Sanyo decoder - serial = SA 8650B
/// @see https://github.com/z3t0/Arduino-IRremote/blob/master/ir_Sanyo.cpp
bool IRrecv::decodeSanyo(decode_results *results, uint16_t nbits, bool strict) {
  if (results->rawlen < 2 * nbits + kHeader - 1)
    return false;  // Shorter than shortest possible.
  if (strict && nbits != kSanyoSA8650BBits)
    return false;  // Doesn't match the spec.

  uint16_t offset = 0;

  // TODO(crankyoldgit): This repeat code looks like garbage, it should never
  //   match or if it does, it won't be reliable. We should probably just
  //   remove it.
  if (results->rawbuf[offset++] < kSanyoSa8650bDoubleSpaceUsecs) {
    results->bits = 0;
    results->value = kRepeat;
    results->decode_type = SANYO;
    results->address = 0;
    results->command = 0;
    results->repeat = true;
    return true;
  }

  // Header
  if (!matchMark(results->rawbuf[offset++], kSanyoSa8650bHdrMark))
    return false;
  // NOTE: These next two lines look very wrong. Treat as suspect.
  if (!matchMark(results->rawbuf[offset++], kSanyoSa8650bHdrMark))
    return false;
  // Data
  uint64_t data = 0;
  while (offset + 1 < results->rawlen) {
    if (!matchSpace(results->rawbuf[offset], kSanyoSa8650bHdrSpace))
      break;
    offset++;
    if (matchMark(results->rawbuf[offset], kSanyoSa8650bOneMark))
      data = (data << 1) | 1;  // 1
    else if (matchMark(results->rawbuf[offset], kSanyoSa8650bZeroMark))
      data <<= 1;  // 0
    else
      return false;
    offset++;
  }

  if (strict && kSanyoSA8650BBits > (offset - 1U) / 2U)
    return false;

  // Success
  results->bits = (offset - 1) / 2;
  results->decode_type = SANYO;
  results->value = data;
  results->address = 0;
  results->command = 0;
  return true;
}
*/
#endif  // DECODE_SANYO


#if SEND_SANYO_AC
/// Send a SanyoAc formatted message.
/// Status: STABLE / Reported as working.
/// @param[in] data An array of bytes containing the IR command.
/// @param[in] nbytes Nr. of bytes of data in the array.
/// @param[in] repeat Nr. of times the message is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1211
void IRsend::sendSanyoAc(const uint8_t data[], const uint16_t nbytes,
                         const uint16_t repeat) {
  // Header + Data + Footer
  sendGeneric(kSanyoAcHdrMark, kSanyoAcHdrSpace,
              kSanyoAcBitMark, kSanyoAcOneSpace,
              kSanyoAcBitMark, kSanyoAcZeroSpace,
              kSanyoAcBitMark, kSanyoAcGap,
              data, nbytes, kSanyoAcFreq, false, repeat, kDutyDefault);
}
#endif  // SEND_SANYO_AC

#if DECODE_SANYO_AC
/// Decode the supplied SanyoAc message.
/// Status: STABLE / Reported as working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1211
bool IRrecv::decodeSanyoAc(decode_results *results, uint16_t offset,
                           const uint16_t nbits, const bool strict) {
  if (strict && nbits != kSanyoAcBits)
    return false;

  // Header + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, results->state,
                    results->rawlen - offset, nbits,
                    kSanyoAcHdrMark, kSanyoAcHdrSpace,
                    kSanyoAcBitMark, kSanyoAcOneSpace,
                    kSanyoAcBitMark, kSanyoAcZeroSpace,
                    kSanyoAcBitMark, kSanyoAcGap,
                    true, kUseDefTol, kMarkExcess, false)) return false;
  // Compliance
  if (strict)
    if (!IRSanyoAc::validChecksum(results->state, nbits / 8)) return false;

  // Success
  results->decode_type = decode_type_t::SANYO_AC;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_SANYO_AC

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRSanyoAc::IRSanyoAc(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Reset the state of the remote to a known  state/sequence.
void IRSanyoAc::stateReset(void) {
  static const uint8_t kReset[kSanyoAcStateLength] = {
    0x6A, 0x6D, 0x51, 0x00, 0x10, 0x45, 0x00, 0x00, 0x33};
  std::memcpy(_.raw, kReset, kSanyoAcStateLength);
}

/// Set up hardware to be able to send a message.
void IRSanyoAc::begin(void) { _irsend.begin(); }

#if SEND_SANYO_AC
/// Send the current internal state as IR messages.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRSanyoAc::send(const uint16_t repeat) {
  _irsend.sendSanyoAc(getRaw(), kSanyoAcStateLength, repeat);
}
#endif  // SEND_SANYO_AC

/// Get a PTR to the internal state/code for this protocol with all integrity
///   checks passing.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t* IRSanyoAc::getRaw(void) {
  checksum();
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] newState A valid code for this protocol.
void IRSanyoAc::setRaw(const uint8_t newState[]) {
  std::memcpy(_.raw, newState, kSanyoAcStateLength);
}

/// Calculate the checksum for a given state.
/// @param[in] state The array to calc the checksum of.
/// @param[in] length The length/size of the array.
/// @return The calculated checksum value.
uint8_t IRSanyoAc::calcChecksum(const uint8_t state[],
                                const uint16_t length) {
  return length ? sumNibbles(state, length - 1) : 0;
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length/size of the array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRSanyoAc::validChecksum(const uint8_t state[], const uint16_t length) {
  return length && state[length - 1] == IRSanyoAc::calcChecksum(state, length);
}

/// Calculate & set the checksum for the current internal state of the remote.
void IRSanyoAc::checksum(void) {
  // Stored the checksum value in the last byte.
  _.Sum = calcChecksum(_.raw);
}


/// Set the requested power state of the A/C to on.
void IRSanyoAc::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void IRSanyoAc::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRSanyoAc::setPower(const bool on) {
  _.Power = (on ? kSanyoAcPowerOn : kSanyoAcPowerOff);
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRSanyoAc::getPower(void) const {
  return _.Power == kSanyoAcPowerOn;
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRSanyoAc::getMode(void) const {
  return _.Mode;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
/// @note If we get an unexpected mode, default to AUTO.
void IRSanyoAc::setMode(const uint8_t mode) {
  switch (mode) {
    case kSanyoAcAuto:
    case kSanyoAcCool:
    case kSanyoAcDry:
    case kSanyoAcHeat:
      _.Mode = mode;
      break;
    default: _.Mode = kSanyoAcAuto;
  }
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRSanyoAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kSanyoAcCool;
    case stdAc::opmode_t::kHeat: return kSanyoAcHeat;
    case stdAc::opmode_t::kDry:  return kSanyoAcDry;
    default:                     return kSanyoAcAuto;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRSanyoAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kSanyoAcCool: return stdAc::opmode_t::kCool;
    case kSanyoAcHeat: return stdAc::opmode_t::kHeat;
    case kSanyoAcDry:  return stdAc::opmode_t::kDry;
    default:           return stdAc::opmode_t::kAuto;
  }
}

/// Set the desired temperature.
/// @param[in] degrees The temperature in degrees celsius.
void IRSanyoAc::setTemp(const uint8_t degrees) {
  uint8_t temp = std::max((uint8_t)kSanyoAcTempMin, degrees);
  temp = std::min((uint8_t)kSanyoAcTempMax, temp);
  _.Temp = temp - kSanyoAcTempDelta;
}

/// Get the current desired temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRSanyoAc::getTemp(void) const {
  return _.Temp + kSanyoAcTempDelta;
}

/// Set the sensor temperature.
/// @param[in] degrees The temperature in degrees celsius.
void IRSanyoAc::setSensorTemp(const uint8_t degrees) {
  uint8_t temp = std::max((uint8_t)kSanyoAcTempMin, degrees);
  temp = std::min((uint8_t)kSanyoAcTempMax, temp);
  _.SensorTemp = temp - kSanyoAcTempDelta;
}

/// Get the current sensor temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRSanyoAc::getSensorTemp(void) const {
  return _.SensorTemp + kSanyoAcTempDelta;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRSanyoAc::setFan(const uint8_t speed) {
  _.Fan = speed;
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t IRSanyoAc::getFan(void) const {
  return _.Fan;
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRSanyoAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kSanyoAcFanLow;
    case stdAc::fanspeed_t::kMedium: return kSanyoAcFanMedium;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kSanyoAcFanHigh;
    default:                         return kSanyoAcFanAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] spd The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRSanyoAc::toCommonFanSpeed(const uint8_t spd) {
  switch (spd) {
    case kSanyoAcFanHigh:   return stdAc::fanspeed_t::kHigh;
    case kSanyoAcFanMedium: return stdAc::fanspeed_t::kMedium;
    case kSanyoAcFanLow:    return stdAc::fanspeed_t::kLow;
    default:                return stdAc::fanspeed_t::kAuto;
  }
}

/// Get the vertical swing setting of the A/C.
/// @return The current swing mode setting.
uint8_t IRSanyoAc::getSwingV(void) const {
  return _.SwingV;
}

/// Set the vertical swing setting of the A/C.
/// @param[in] setting The value of the desired setting.
void IRSanyoAc::setSwingV(const uint8_t setting) {
  if (setting == kSanyoAcSwingVAuto ||
      (setting >= kSanyoAcSwingVLowest && setting <= kSanyoAcSwingVHighest))
    _.SwingV = setting;
  else
    _.SwingV = kSanyoAcSwingVAuto;
}

/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRSanyoAc::convertSwingV(const stdAc::swingv_t position) {
  switch (position) {
    case stdAc::swingv_t::kHighest: return kSanyoAcSwingVHighest;
    case stdAc::swingv_t::kHigh:    return kSanyoAcSwingVHigh;
    case stdAc::swingv_t::kMiddle:  return kSanyoAcSwingVUpperMiddle;
    case stdAc::swingv_t::kLow:     return kSanyoAcSwingVLow;
    case stdAc::swingv_t::kLowest:  return kSanyoAcSwingVLowest;
    default:                        return kSanyoAcSwingVAuto;
  }
}

/// Convert a native vertical swing postion to it's common equivalent.
/// @param[in] setting A native position to convert.
/// @return The common vertical swing position.
stdAc::swingv_t IRSanyoAc::toCommonSwingV(const uint8_t setting) {
  switch (setting) {
    case kSanyoAcSwingVHighest:     return stdAc::swingv_t::kHighest;
    case kSanyoAcSwingVHigh:        return stdAc::swingv_t::kHigh;
    case kSanyoAcSwingVUpperMiddle:
    case kSanyoAcSwingVLowerMiddle: return stdAc::swingv_t::kMiddle;
    case kSanyoAcSwingVLow:         return stdAc::swingv_t::kLow;
    case kSanyoAcSwingVLowest:      return stdAc::swingv_t::kLowest;
    default:                        return stdAc::swingv_t::kAuto;
  }
}

/// Set the Sleep (Night Setback) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRSanyoAc::setSleep(const bool on) {
  _.Sleep = on;
}

/// Get the Sleep (Night Setback) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRSanyoAc::getSleep(void) const {
  return _.Sleep;
}

/// Set the Sensor Location setting of the A/C.
/// i.e. Where the ambient temperature is measured.
/// @param[in] location true is Unit/Wall, false is Remote/Room.
void IRSanyoAc::setSensor(const bool location) {
  _.Sensor = location;
}

/// Get the Sensor Location setting of the A/C.
/// i.e. Where the ambient temperature is measured.
/// @return true is Unit/Wall, false is Remote/Room.
bool IRSanyoAc::getSensor(void) const {
  return _.Sensor;
}

/// Set the Beep setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRSanyoAc::setBeep(const bool on) {
  _.Beep = on;
}

/// Get the Beep setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRSanyoAc::getBeep(void) const {
  return _.Beep;
}

/// Get the nr of minutes the Off Timer is set to.
/// @return The timer time expressed as the number of minutes.
///   A value of 0 means the Off Timer is off/disabled.
/// @note The internal precission has a resolution of 1 hour.
uint16_t IRSanyoAc::getOffTimer(void) const {
  if (_.OffTimer)
    return _.OffHour * 60;
  else
    return 0;
}

/// Set the nr of minutes for the Off Timer.
/// @param[in] mins The timer time expressed as nr. of minutes.
///   A value of 0 means the Off Timer is off/disabled.
/// @note The internal precission has a resolution of 1 hour.
void IRSanyoAc::setOffTimer(const uint16_t mins) {
  const uint8_t hours = std::min((uint8_t)(mins / 60), kSanyoAcHourMax);
  _.OffTimer = (hours > 0);
  _.OffHour = hours;
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRSanyoAc::toCommon(void) const {
  stdAc::state_t result{};
  result.protocol = decode_type_t::SANYO_AC;
  result.model = -1;  // Not supported.
  result.power = getPower();
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.sensorTemperature = getSensorTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.sleep = _.Sleep ? 0 : -1;
  result.swingv = toCommonSwingV(_.SwingV);
  result.beep = _.Beep;
  result.iFeel = !getSensor();
  // Not supported.
  result.swingh = stdAc::swingh_t::kOff;
  result.turbo = false;
  result.econo = false;
  result.light = false;
  result.filter = false;
  result.quiet = false;
  result.clean = false;
  result.clock = -1;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRSanyoAc::toString(void) const {
  String result = "";
  result.reserve(140);
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(_.Mode, kSanyoAcAuto, kSanyoAcCool,
                            kSanyoAcHeat, kSanyoAcDry, kSanyoAcAuto);
  result += addTempToString(getTemp());
  result += addFanToString(_.Fan, kSanyoAcFanHigh, kSanyoAcFanLow,
                           kSanyoAcFanAuto, kSanyoAcFanAuto,
                           kSanyoAcFanMedium);
  result += addSwingVToString(_.SwingV, kSanyoAcSwingVAuto,
                              kSanyoAcSwingVHighest, kSanyoAcSwingVHigh,
                              kSanyoAcSwingVUpperMiddle,
                              kSanyoAcSwingVAuto,  // Middle is unused
                              kSanyoAcSwingVLowerMiddle,
                              kSanyoAcSwingVLow, kSanyoAcSwingVLowest,
                              // Below are unused.
                              kSanyoAcSwingVAuto,
                              kSanyoAcSwingVAuto,
                              kSanyoAcSwingVAuto,
                              kSanyoAcSwingVAuto);
  result += addBoolToString(_.Sleep, kSleepStr);
  result += addBoolToString(_.Beep, kBeepStr);
  result += addLabeledString(_.Sensor ? kRoomStr : kWallStr, kSensorStr);
  result += kCommaSpaceStr;
  result += kSensorStr;
  result += ' ';
  result += addTempToString(getSensorTemp(), true, false);
  const uint16_t offtime =  getOffTimer();
  result += addLabeledString(offtime ? minsToString(offtime) : kOffStr,
                             kOffTimerStr);
  return result;
}

#if SEND_SANYO_AC88
/// Send a SanyoAc88 formatted message.
/// Status: ALPHA / Completely untested.
/// @param[in] data An array of bytes containing the IR command.
/// @warning data's bit order may change. It is not yet confirmed.
/// @param[in] nbytes Nr. of bytes of data in the array.
/// @param[in] repeat Nr. of times the message is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1503
void IRsend::sendSanyoAc88(const uint8_t data[], const uint16_t nbytes,
                           const uint16_t repeat) {
  // (Header + Data + Footer) per repeat
  sendGeneric(kSanyoAc88HdrMark, kSanyoAc88HdrSpace,
              kSanyoAc88BitMark, kSanyoAc88OneSpace,
              kSanyoAc88BitMark, kSanyoAc88ZeroSpace,
              kSanyoAc88BitMark, kSanyoAc88Gap,
              data, nbytes, kSanyoAc88Freq, false, repeat, kDutyDefault);
  space(kDefaultMessageGap);  // Make a guess at a post message gap.
}
#endif  // SEND_SANYO_AC88

#if DECODE_SANYO_AC88
/// Decode the supplied SanyoAc88 message.
/// Status: ALPHA / Untested.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
/// @warning data's bit order may change. It is not yet confirmed.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1503
bool IRrecv::decodeSanyoAc88(decode_results *results, uint16_t offset,
                             const uint16_t nbits, const bool strict) {
  if (strict && nbits != kSanyoAc88Bits)
    return false;

  uint16_t used = 0;
  // Compliance
  const uint16_t expected_repeats = strict ? kSanyoAc88MinRepeat : 0;

  // Handle the expected nr of repeats.
  for (uint16_t r = 0; r <= expected_repeats; r++) {
    // Header + Data + Footer
    used = matchGeneric(results->rawbuf + offset, results->state,
                        results->rawlen - offset, nbits,
                        kSanyoAc88HdrMark, kSanyoAc88HdrSpace,
                        kSanyoAc88BitMark, kSanyoAc88OneSpace,
                        kSanyoAc88BitMark, kSanyoAc88ZeroSpace,
                        kSanyoAc88BitMark,
                        // Expect an inter-message gap, or just the end of msg?
                        (r < expected_repeats) ? kSanyoAc88Gap
                                               : kDefaultMessageGap,
                        r == expected_repeats,
                        _tolerance + kSanyoAc88ExtraTolerance,
                        kMarkExcess, false);
    if (!used) return false;  // No match!
    offset += used;
  }

  // Success
  results->decode_type = decode_type_t::SANYO_AC88;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_SANYO_AC88

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRSanyoAc88::IRSanyoAc88(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Reset the state of the remote to a known good state/sequence.
/// @see https://docs.google.com/spreadsheets/d/1dYfLsnYvpjV-SgO8pdinpfuBIpSzm8Q1R5SabrLeskw/edit?ts=5f0190a5#gid=1050142776&range=A2:B2
void IRSanyoAc88::stateReset(void) {
  static const uint8_t kReset[kSanyoAc88StateLength] = {
    0xAA, 0x55, 0xA0, 0x16, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x10};
  std::memcpy(_.raw, kReset, kSanyoAc88StateLength);
}

/// Set up hardware to be able to send a message.
void IRSanyoAc88::begin(void) { _irsend.begin(); }

#if SEND_SANYO_AC88
/// Send the current internal state as IR messages.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRSanyoAc88::send(const uint16_t repeat) {
  _irsend.sendSanyoAc88(getRaw(), kSanyoAc88StateLength, repeat);
}
#endif  // SEND_SANYO_AC88

/// Get a PTR to the internal state/code for this protocol with all integrity
///   checks passing.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t* IRSanyoAc88::getRaw(void) {
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] newState A valid code for this protocol.
void IRSanyoAc88::setRaw(const uint8_t newState[]) {
  std::memcpy(_.raw, newState, kSanyoAc88StateLength);
}

/// Set the requested power state of the A/C to on.
void IRSanyoAc88::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void IRSanyoAc88::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRSanyoAc88::setPower(const bool on) {   _.Power = on; }

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRSanyoAc88::getPower(void) const { return _.Power; }

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRSanyoAc88::getMode(void) const { return _.Mode; }

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
/// @note If we get an unexpected mode, default to AUTO.
void IRSanyoAc88::setMode(const uint8_t mode) {
  switch (mode) {
    case kSanyoAc88Auto:
    case kSanyoAc88FeelCool:
    case kSanyoAc88Cool:
    case kSanyoAc88FeelHeat:
    case kSanyoAc88Heat:
    case kSanyoAc88Fan:
      _.Mode = mode;
      break;
    default: _.Mode = kSanyoAc88Auto;
  }
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRSanyoAc88::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kSanyoAc88Cool;
    case stdAc::opmode_t::kHeat: return kSanyoAc88Heat;
    case stdAc::opmode_t::kFan:  return kSanyoAc88Fan;
    default:                     return kSanyoAc88Auto;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRSanyoAc88::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kSanyoAc88FeelCool:
    case kSanyoAc88Cool:
      return stdAc::opmode_t::kCool;
    case kSanyoAc88FeelHeat:
    case kSanyoAc88Heat:
      return stdAc::opmode_t::kHeat;
    case kSanyoAc88Fan:
      return stdAc::opmode_t::kFan;
    default:
      return stdAc::opmode_t::kAuto;
  }
}

/// Set the desired temperature.
/// @param[in] degrees The temperature in degrees celsius.
void IRSanyoAc88::setTemp(const uint8_t degrees) {
  uint8_t temp = std::max((uint8_t)kSanyoAc88TempMin, degrees);
  _.Temp = std::min((uint8_t)kSanyoAc88TempMax, temp);
}

/// Get the current desired temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRSanyoAc88::getTemp(void) const { return _.Temp; }

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRSanyoAc88::setFan(const uint8_t speed) { _.Fan = speed; }

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t IRSanyoAc88::getFan(void) const { return _.Fan; }

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRSanyoAc88::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kSanyoAc88FanLow;
    case stdAc::fanspeed_t::kMedium: return kSanyoAc88FanMedium;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kSanyoAc88FanHigh;
    default:                         return kSanyoAc88FanAuto;
  }
}

/// Get the current clock time.
/// @return The time as the nr. of minutes past midnight.
uint16_t IRSanyoAc88::getClock(void) const {
  return _.ClockHrs * 60 + _.ClockMins;
}

/// Set the current clock time.
/// @param[in] mins_since_midnight The time as nr. of minutes past midnight.
void IRSanyoAc88::setClock(const uint16_t mins_since_midnight) {
  uint16_t mins = std::min(mins_since_midnight, (uint16_t)(23 * 60 + 59));
  _.ClockMins = mins % 60;
  _.ClockHrs = mins / 60;
  _.ClockSecs = 0;
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] spd The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRSanyoAc88::toCommonFanSpeed(const uint8_t spd) {
  switch (spd) {
    case kSanyoAc88FanHigh:   return stdAc::fanspeed_t::kHigh;
    case kSanyoAc88FanMedium: return stdAc::fanspeed_t::kMedium;
    case kSanyoAc88FanLow:    return stdAc::fanspeed_t::kLow;
    default:                  return stdAc::fanspeed_t::kAuto;
  }
}

/// Change the SwingV setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRSanyoAc88::setSwingV(const bool on) { _.SwingV = on; }

/// Get the value of the current SwingV setting.
/// @return true, the setting is on. false, the setting is off.
bool IRSanyoAc88::getSwingV(void) const { return _.SwingV; }

/// Change the Turbo setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRSanyoAc88::setTurbo(const bool on) { _.Turbo = on; }

/// Get the value of the current Turbo setting.
/// @return true, the setting is on. false, the setting is off.
bool IRSanyoAc88::getTurbo(void) const { return _.Turbo; }

/// Change the Filter setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRSanyoAc88::setFilter(const bool on) { _.Filter = on; }

/// Get the value of the current Filter setting.
/// @return true, the setting is on. false, the setting is off.
bool IRSanyoAc88::getFilter(void) const { return _.Filter; }

/// Change the Sleep setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRSanyoAc88::setSleep(const bool on) { _.Sleep = on; }

/// Get the value of the current Sleep setting.
/// @return true, the setting is on. false, the setting is off.
bool IRSanyoAc88::getSleep(void) const { return _.Sleep; }

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRSanyoAc88::toCommon(void) const {
  stdAc::state_t result{};
  result.protocol = decode_type_t::SANYO_AC88;
  result.model = -1;  // Not supported.
  result.power = getPower();
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.swingv = _.SwingV ? stdAc::swingv_t::kAuto : stdAc::swingv_t::kOff;
  result.filter = _.Filter;
  result.turbo = _.Turbo;
  result.sleep = _.Sleep ? 0 : -1;
  result.clock = getClock();
  // Not supported.
  result.swingh = stdAc::swingh_t::kOff;
  result.econo = false;
  result.light = false;
  result.quiet = false;
  result.beep = false;
  result.clean = false;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRSanyoAc88::toString(void) const {
  String result = "";
  result.reserve(115);
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(_.Mode, kSanyoAc88Auto, kSanyoAc88Cool,
                            kSanyoAc88Heat, kSanyoAc88Auto, kSanyoAc88Fan);
  result += addTempToString(getTemp());
  result += addFanToString(_.Fan, kSanyoAc88FanHigh, kSanyoAc88FanLow,
                           kSanyoAc88FanAuto, kSanyoAc88FanAuto,
                           kSanyoAc88FanMedium);
  result += addBoolToString(_.SwingV, kSwingVStr);
  result += addBoolToString(_.Turbo, kTurboStr);
  result += addBoolToString(_.Sleep, kSleepStr);
  result += addLabeledString(minsToString(getClock()), kClockStr);
  return result;
}

#if SEND_SANYO_AC152
/// Send a SanyoAc152 formatted message.
/// Status: BETA / Probably works.
/// @param[in] data An array of bytes containing the IR command.
/// @warning data's bit order may change. It is not yet confirmed.
/// @param[in] nbytes Nr. of bytes of data in the array.
/// @param[in] repeat Nr. of times the message is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1826
void IRsend::sendSanyoAc152(const uint8_t data[], const uint16_t nbytes,
                           const uint16_t repeat) {
  // (Header + Data + Footer) per repeat
  sendGeneric(kSanyoAc152HdrMark, kSanyoAc152HdrSpace,
              kSanyoAc152BitMark, kSanyoAc152OneSpace,
              kSanyoAc152BitMark, kSanyoAc152ZeroSpace,
              kSanyoAc152BitMark, kDefaultMessageGap,
              data, nbytes, kSanyoAc152Freq, false, repeat, kDutyDefault);
  space(kDefaultMessageGap);  // Make a guess at a post message gap.
}
#endif  // SEND_SANYO_AC152

#if DECODE_SANYO_AC152
/// Decode the supplied SanyoAc152 message.
/// Status: BETA / Probably works.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
/// @warning data's bit order may change. It is not yet confirmed.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1503
bool IRrecv::decodeSanyoAc152(decode_results *results, uint16_t offset,
                              const uint16_t nbits, const bool strict) {
  if (strict && nbits != kSanyoAc152Bits)
    return false;

  // Header + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, results->state,
                    results->rawlen - offset, nbits,
                    kSanyoAc152HdrMark, kSanyoAc152HdrSpace,
                    kSanyoAc152BitMark, kSanyoAc152OneSpace,
                    kSanyoAc152BitMark, kSanyoAc152ZeroSpace,
                    kSanyoAc152BitMark,
                    kDefaultMessageGap,  // Just a guess.
                    false, _tolerance + kSanyoAc152ExtraTolerance,
                    kMarkExcess, false))
    return false;  // No match!

  // Success
  results->decode_type = decode_type_t::SANYO_AC152;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_SANYO_AC152
