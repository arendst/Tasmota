// Copyright 2009 Ken Shirriff
// Copyright 2017, 2018, 2019 David Conran
/// @file
/// @brief Support for Samsung protocols.
/// Samsung originally added from https://github.com/shirriff/Arduino-IRremote/
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/505
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/621
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1062
/// @see http://elektrolab.wz.cz/katalog/samsung_protocol.pdf

#include "ir_Samsung.h"
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
const uint16_t kSamsungTick = 560;
const uint16_t kSamsungHdrMarkTicks = 8;
const uint16_t kSamsungHdrMark = kSamsungHdrMarkTicks * kSamsungTick;
const uint16_t kSamsungHdrSpaceTicks = 8;
const uint16_t kSamsungHdrSpace = kSamsungHdrSpaceTicks * kSamsungTick;
const uint16_t kSamsungBitMarkTicks = 1;
const uint16_t kSamsungBitMark = kSamsungBitMarkTicks * kSamsungTick;
const uint16_t kSamsungOneSpaceTicks = 3;
const uint16_t kSamsungOneSpace = kSamsungOneSpaceTicks * kSamsungTick;
const uint16_t kSamsungZeroSpaceTicks = 1;
const uint16_t kSamsungZeroSpace = kSamsungZeroSpaceTicks * kSamsungTick;
const uint16_t kSamsungRptSpaceTicks = 4;
const uint16_t kSamsungRptSpace = kSamsungRptSpaceTicks * kSamsungTick;
const uint16_t kSamsungMinMessageLengthTicks = 193;
const uint32_t kSamsungMinMessageLength =
    kSamsungMinMessageLengthTicks * kSamsungTick;
const uint16_t kSamsungMinGapTicks =
    kSamsungMinMessageLengthTicks -
    (kSamsungHdrMarkTicks + kSamsungHdrSpaceTicks +
     kSamsungBits * (kSamsungBitMarkTicks + kSamsungOneSpaceTicks) +
     kSamsungBitMarkTicks);
const uint32_t kSamsungMinGap = kSamsungMinGapTicks * kSamsungTick;

const uint16_t kSamsungAcHdrMark = 690;
const uint16_t kSamsungAcHdrSpace = 17844;
const uint8_t kSamsungAcSections = 2;
const uint16_t kSamsungAcSectionMark = 3086;
const uint16_t kSamsungAcSectionSpace = 8864;
const uint16_t kSamsungAcSectionGap = 2886;
const uint16_t kSamsungAcBitMark = 586;
const uint16_t kSamsungAcOneSpace = 1432;
const uint16_t kSamsungAcZeroSpace = 436;

// Data from https://github.com/crankyoldgit/IRremoteESP8266/issues/1220
// Values calculated based on the average of ten messages.
const uint16_t kSamsung36HdrMark = 4515;  /// < uSeconds
const uint16_t kSamsung36HdrSpace = 4438;  /// < uSeconds
const uint16_t kSamsung36BitMark = 512;  /// < uSeconds
const uint16_t kSamsung36OneSpace = 1468;  /// < uSeconds
const uint16_t kSamsung36ZeroSpace = 490;  /// < uSeconds

using irutils::addBoolToString;
using irutils::addFanToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;

#if SEND_SAMSUNG
/// Send a 32-bit Samsung formatted message.
/// Status: STABLE / Should be working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see http://elektrolab.wz.cz/katalog/samsung_protocol.pdf
/// @note Samsung has a separate message to indicate a repeat, like NEC does.
/// @todo Confirm that is actually how Samsung sends a repeat.
///   The refdoc doesn't indicate it is true.
void IRsend::sendSAMSUNG(const uint64_t data, const uint16_t nbits,
                         const uint16_t repeat) {
  sendGeneric(kSamsungHdrMark, kSamsungHdrSpace, kSamsungBitMark,
              kSamsungOneSpace, kSamsungBitMark, kSamsungZeroSpace,
              kSamsungBitMark, kSamsungMinGap, kSamsungMinMessageLength, data,
              nbits, 38, true, repeat, 33);
}

/// Construct a raw Samsung message from the supplied customer(address) &
/// command.
/// Status: STABLE / Should be working.
/// @param[in] customer The customer code. (aka. Address)
/// @param[in] command The command code.
/// @return A raw 32-bit Samsung message suitable for `sendSAMSUNG()`.
uint32_t IRsend::encodeSAMSUNG(const uint8_t customer, const uint8_t command) {
  uint8_t revcustomer = reverseBits(customer, sizeof(customer) * 8);
  uint8_t revcommand = reverseBits(command, sizeof(command) * 8);
  return ((revcommand ^ 0xFF) | (revcommand << 8) | (revcustomer << 16) |
          (revcustomer << 24));
}
#endif

#if DECODE_SAMSUNG
/// Decode the supplied Samsung 32-bit message.
/// Status: STABLE
/// @note Samsung messages whilst 32 bits in size, only contain 16 bits of
///   distinct data. e.g. In transmition order:
///   customer_byte + customer_byte(same) + address_byte + invert(address_byte)
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
/// @note LG 32bit protocol appears near identical to the Samsung protocol.
///   They differ on their compliance criteria and how they repeat.
/// @see http://elektrolab.wz.cz/katalog/samsung_protocol.pdf
bool IRrecv::decodeSAMSUNG(decode_results *results, uint16_t offset,
                           const uint16_t nbits, const bool strict) {
  if (strict && nbits != kSamsungBits)
    return false;  // We expect Samsung to be 32 bits of message.

  uint64_t data = 0;

  // Match Header + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, &data,
                    results->rawlen - offset, nbits,
                    kSamsungHdrMark, kSamsungHdrSpace,
                    kSamsungBitMark, kSamsungOneSpace,
                    kSamsungBitMark, kSamsungZeroSpace,
                    kSamsungBitMark, kSamsungMinGap, true)) return false;
  // Compliance
  // According to the spec, the customer (address) code is the first 8
  // transmitted bits. It's then repeated. Check for that.
  uint8_t address = data >> 24;
  if (strict && address != ((data >> 16) & 0xFF)) return false;
  // Spec says the command code is the 3rd block of transmitted 8-bits,
  // followed by the inverted command code.
  uint8_t command = (data & 0xFF00) >> 8;
  if (strict && command != ((data & 0xFF) ^ 0xFF)) return false;

  // Success
  results->bits = nbits;
  results->value = data;
  results->decode_type = SAMSUNG;
  // command & address need to be reversed as they are transmitted LSB first,
  results->command = reverseBits(command, sizeof(command) * 8);
  results->address = reverseBits(address, sizeof(address) * 8);
  return true;
}
#endif

#if SEND_SAMSUNG36
/// Send a Samsung 36-bit formatted message.
/// Status: STABLE / Works on real devices.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/621
void IRsend::sendSamsung36(const uint64_t data, const uint16_t nbits,
                           const uint16_t repeat) {
  if (nbits < 16) return;  // To small to send.
  for (uint16_t r = 0; r <= repeat; r++) {
    // Block #1 (16 bits)
    sendGeneric(kSamsung36HdrMark, kSamsung36HdrSpace,
                kSamsung36BitMark, kSamsung36OneSpace,
                kSamsung36BitMark, kSamsung36ZeroSpace,
                kSamsung36BitMark, kSamsung36HdrSpace,
                data >> (nbits - 16), 16, 38, true, 0, kDutyDefault);
    // Block #2 (The rest, typically 20 bits)
    sendGeneric(0, 0,  // No header
                kSamsung36BitMark, kSamsung36OneSpace,
                kSamsung36BitMark, kSamsung36ZeroSpace,
                kSamsung36BitMark, kSamsungMinGap,  // Gap is just a guess.
                // Mask off the rest of the bits.
                data & ((1ULL << (nbits - 16)) - 1),
                nbits - 16, 38, true, 0, kDutyDefault);
  }
}
#endif  // SEND_SAMSUNG36

#if DECODE_SAMSUNG36
/// Decode the supplied Samsung36 message.
/// Status: STABLE / Expected to work.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/621
bool IRrecv::decodeSamsung36(decode_results *results, uint16_t offset,
                             const uint16_t nbits, const bool strict) {
  if (results->rawlen < 2 * nbits + kHeader + kFooter * 2 - 1 + offset)
    return false;  // Can't possibly be a valid Samsung message.
  // We need to be looking for > 16 bits to make sense.
  if (nbits <= 16) return false;
  if (strict && nbits != kSamsung36Bits)
    return false;  // We expect nbits to be 36 bits of message.

  uint64_t data = 0;

  // Match Header + Data + Footer
  uint16_t used;
  used = matchGeneric(results->rawbuf + offset, &data,
                      results->rawlen - offset, 16,
                      kSamsung36HdrMark, kSamsung36HdrSpace,
                      kSamsung36BitMark, kSamsung36OneSpace,
                      kSamsung36BitMark, kSamsung36ZeroSpace,
                      kSamsung36BitMark, kSamsung36HdrSpace, false);
  if (!used) return false;
  offset += used;
  // Data (Block #2)
  uint64_t data2 = 0;
  if (!matchGeneric(results->rawbuf + offset, &data2,
                    results->rawlen - offset, nbits - 16,
                    0, 0,
                    kSamsung36BitMark, kSamsung36OneSpace,
                    kSamsung36BitMark, kSamsung36ZeroSpace,
                    kSamsung36BitMark, kSamsungMinGap, true)) return false;
  data <<= (nbits - 16);
  data += data2;

  // Success
  results->bits = nbits;
  results->value = data;
  results->decode_type = SAMSUNG36;
  results->command = data & ((1ULL << (nbits - 16)) - 1);
  results->address = data >> (nbits - 16);
  return true;
}
#endif  // DECODE_SAMSUNG36

#if SEND_SAMSUNG_AC
/// Send a Samsung A/C message.
/// Status: Stable / Known working.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/505
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendSamsungAC(const uint8_t data[], const uint16_t nbytes,
                           const uint16_t repeat) {
  if (nbytes < kSamsungAcStateLength && nbytes % kSamsungAcSectionLength)
    return;  // Not an appropriate number of bytes to send a proper message.

  enableIROut(38);
  for (uint16_t r = 0; r <= repeat; r++) {
    // Header
    mark(kSamsungAcHdrMark);
    space(kSamsungAcHdrSpace);
    // Send in 7 byte sections.
    for (uint16_t offset = 0; offset < nbytes;
         offset += kSamsungAcSectionLength) {
      sendGeneric(kSamsungAcSectionMark, kSamsungAcSectionSpace,
                  kSamsungAcBitMark, kSamsungAcOneSpace, kSamsungAcBitMark,
                  kSamsungAcZeroSpace, kSamsungAcBitMark, kSamsungAcSectionGap,
                  data + offset, kSamsungAcSectionLength,  // 7 bytes == 56 bits
                  38000, false, 0, 50);                    // Send in LSBF order
    }
    // Complete made up guess at inter-message gap.
    space(kDefaultMessageGap - kSamsungAcSectionGap);
  }
}
#endif  // SEND_SAMSUNG_AC

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRSamsungAc::IRSamsungAc(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) {
  stateReset();
}

/// Reset the internal state of the emulation.
/// @param[in] forcepower A flag indicating if force sending a special power
///   message with the first `send()` call.
/// @param[in] initialPower Set the initial power state. True, on. False, off.
void IRSamsungAc::stateReset(const bool forcepower, const bool initialPower) {
  static const uint8_t kReset[kSamsungAcExtendedStateLength] = {
      0x02, 0x92, 0x0F, 0x00, 0x00, 0x00, 0xF0, 0x01, 0x02, 0xAE, 0x71, 0x00,
      0x15, 0xF0};
  std::memcpy(_.raw, kReset, kSamsungAcExtendedStateLength);
  _forcepower = forcepower;
  _lastsentpowerstate = initialPower;
  setPower(initialPower);
}

/// Set up hardware to be able to send a message.
void IRSamsungAc::begin(void) { _irsend.begin(); }

/// Calculate the checksum for a given state.
/// @param[in] state The array to calc the checksum of.
/// @param[in] length The length/size of the array.
/// @return The calculated checksum value.
uint8_t IRSamsungAc::calcChecksum(const uint8_t state[],
                                  const uint16_t length) {
  uint8_t sum = 0;
  // Safety check so we don't go outside the array.
  if (length < 7) return 255;
  // Shamelessly inspired by:
  //   https://github.com/adafruit/Raw-IR-decoder-for-Arduino/pull/3/files
  // Count most of the '1' bits after the checksum location.
  sum += countBits(state[length - 7], 8);
  sum -= countBits(GETBITS8(state[length - 6], kLowNibble, kNibbleSize), 8);
  sum += countBits(GETBITS8(state[length - 5], 1, 7), 8);
  sum += countBits(state + length - 4, 3);
  return GETBITS8(28 - sum, kLowNibble, kNibbleSize);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length/size of the array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRSamsungAc::validChecksum(const uint8_t state[], const uint16_t length) {
  if (length < kSamsungAcStateLength)
    return true;  // No checksum to compare with. Assume okay.
  uint8_t offset = 0;
  if (length >= kSamsungAcExtendedStateLength) offset = 7;
  return (GETBITS8(state[length - 6], kHighNibble, kNibbleSize) ==
          IRSamsungAc::calcChecksum(state, length)) &&
         (GETBITS8(state[length - (13 + offset)], kHighNibble, kNibbleSize) ==
          IRSamsungAc::calcChecksum(state, length - (7 + offset)));
}

/// Update the checksum for the internal state.
/// @param[in] length The length/size of the internal array to checksum.
void IRSamsungAc::checksum(uint16_t length) {
  if (length < 13) return;
  _.Sum2 = calcChecksum(_.raw, length);
  _.Sum1 = calcChecksum(_.raw, length - 7);
}

#if SEND_SAMSUNG_AC
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
/// @param[in] calcchecksum Do we update the checksum before sending?
/// @note Use for most function/mode/settings changes to the unit.
///   i.e. When the device is already running.
void IRSamsungAc::send(const uint16_t repeat, const bool calcchecksum) {
  if (calcchecksum) checksum();
  // Do we need to send a the special power on/off message?
  if (getPower() != _lastsentpowerstate || _forcepower) {
    _forcepower = false;  // It will now been sent, so clear the flag if set.
    if (getPower()) {
      sendOn(repeat);
    } else {
      sendOff(repeat);
      return;  // No point sending anything else if we are turning the unit off.
    }
  }
  _irsend.sendSamsungAC(_.raw, kSamsungAcStateLength, repeat);
}

/// Send the extended current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
/// @param[in] calcchecksum Do we update the checksum before sending?
/// @note Use this for when you need to power on/off the device.
/// Samsung A/C requires an extended length message when you want to
/// change the power operating mode of the A/C unit.
void IRSamsungAc::sendExtended(const uint16_t repeat, const bool calcchecksum) {
  if (calcchecksum) checksum();
  uint8_t extended_state[kSamsungAcExtendedStateLength] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x01, 0xD2, 0x0F, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  // Copy/convert the internal state to an extended state.
  for (uint16_t i = 0; i < kSamsungAcSectionLength; i++)
    extended_state[i] = _.raw[i];
  for (uint16_t i = kSamsungAcSectionLength; i < kSamsungAcStateLength; i++)
    extended_state[i + kSamsungAcSectionLength] = _.raw[i];
  // extended_state[8] seems special. This is a guess on how to calculate it.
  extended_state[8] = (extended_state[1] & 0x9F) | 0x40;
  // Send it.
  _irsend.sendSamsungAC(extended_state, kSamsungAcExtendedStateLength, repeat);
}

/// Send the special extended "On" message as the library can't seem to
/// reproduce this message automatically.
/// @param[in] repeat Nr. of times the message will be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/604#issuecomment-475020036
void IRSamsungAc::sendOn(const uint16_t repeat) {
  const uint8_t extended_state[21] = {
      0x02, 0x92, 0x0F, 0x00, 0x00, 0x00, 0xF0,
      0x01, 0xD2, 0x0F, 0x00, 0x00, 0x00, 0x00,
      0x01, 0xE2, 0xFE, 0x71, 0x80, 0x11, 0xF0};
  _irsend.sendSamsungAC(extended_state, kSamsungAcExtendedStateLength, repeat);
  _lastsentpowerstate = true;  // On
}

/// Send the special extended "Off" message as the library can't seem to
/// reproduce this message automatically.
/// @param[in] repeat Nr. of times the message will be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/604#issuecomment-475020036
void IRSamsungAc::sendOff(const uint16_t repeat) {
  const uint8_t extended_state[21] = {
      0x02, 0xB2, 0x0F, 0x00, 0x00, 0x00, 0xC0,
      0x01, 0xD2, 0x0F, 0x00, 0x00, 0x00, 0x00,
      0x01, 0x02, 0xFF, 0x71, 0x80, 0x11, 0xC0};
  _irsend.sendSamsungAC(extended_state, kSamsungAcExtendedStateLength, repeat);
  _lastsentpowerstate = false;  // Off
}
#endif  // SEND_SAMSUNG_AC

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRSamsungAc::getRaw(void) {
  checksum();
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
/// @param[in] length The length/size of the new_code array.
void IRSamsungAc::setRaw(const uint8_t new_code[], const uint16_t length) {
  std::memcpy(_.raw, new_code, std::min(length,
                                          kSamsungAcExtendedStateLength));
  // Shrink the extended state into a normal state.
  if (length > kSamsungAcStateLength) {
    for (uint8_t i = kSamsungAcStateLength; i < length; i++)
      _.raw[i - kSamsungAcSectionLength] = _.raw[i];
  }
}

/// Set the requested power state of the A/C to on.
void IRSamsungAc::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void IRSamsungAc::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRSamsungAc::setPower(const bool on) {
  _.Power1 = !on;  // Cleared when on.
  _.Power6 = (on ? 0b11 : 0b00);
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRSamsungAc::getPower(void) const {
  return (_.Power6 == 0b11) && !_.Power1;
}

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
void IRSamsungAc::setTemp(const uint8_t temp) {
  uint8_t newtemp = std::max(kSamsungAcMinTemp, temp);
  newtemp = std::min(kSamsungAcMaxTemp, newtemp);
  _.Temp = newtemp - kSamsungAcMinTemp;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRSamsungAc::getTemp(void) const {
  return _.Temp + kSamsungAcMinTemp;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRSamsungAc::setMode(const uint8_t mode) {
  // If we get an unexpected mode, default to AUTO.
  uint8_t newmode = mode;
  if (newmode > kSamsungAcHeat) newmode = kSamsungAcAuto;
  _.Mode = newmode;

  // Auto mode has a special fan setting valid only in auto mode.
  if (newmode == kSamsungAcAuto) {
    _.Fan = kSamsungAcFanAuto2;
  } else {
    // Non-Auto can't have this fan setting
    if (_.Fan == kSamsungAcFanAuto2)
      _.Fan = kSamsungAcFanAuto;  // Default to something safe.
  }
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRSamsungAc::getMode(void) const {
  return _.Mode;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRSamsungAc::setFan(const uint8_t speed) {
  switch (speed) {
    case kSamsungAcFanAuto:
    case kSamsungAcFanLow:
    case kSamsungAcFanMed:
    case kSamsungAcFanHigh:
    case kSamsungAcFanTurbo:
      if (_.Mode == kSamsungAcAuto) return;  // Not valid in Auto mode.
      break;
    case kSamsungAcFanAuto2:  // Special fan setting for when in Auto mode.
      if (_.Mode != kSamsungAcAuto) return;
      break;
    default:
      return;
  }
  _.Fan = speed;
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t IRSamsungAc::getFan(void) const {
  return _.Fan;
}

/// Get the vertical swing setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
/// @todo (Hollako) Explain why sometimes the LSB of remote_state[9] is a 1.
/// e.g. 0xAE or 0XAF for swing move.
bool IRSamsungAc::getSwing(void) const {
  return _.Swing == kSamsungAcSwingMove;
}

/// Set the vertical swing setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @todo (Hollako) Explain why sometimes the LSB of remote_state[9] is a 1.
///   e.g. 0xAE or 0XAF for swing move.
void IRSamsungAc::setSwing(const bool on) {
  _.Swing = (on ? kSamsungAcSwingMove : kSamsungAcSwingStop);
}

/// Get the Beep setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRSamsungAc::getBeep(void) const {
  return _.Beep;
}

/// Set the Beep setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRSamsungAc::setBeep(const bool on) {
  _.Beep = on;
}

/// Get the Clean setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRSamsungAc::getClean(void) const {
  return _.Clean10 && _.Clean11;
}

/// Set the Clean setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRSamsungAc::setClean(const bool on) {
  _.Clean10 = on;
  _.Clean11 = on;
}

/// Get the Quiet setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRSamsungAc::getQuiet(void) const {
  return !_.Quiet1 && _.Quiet5;
}

/// Set the Quiet setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRSamsungAc::setQuiet(const bool on) {
  _.Quiet1 = !on;  // Cleared when on.
  _.Quiet5 = on;
  if (on) {
    // Quiet mode seems to set fan speed to auto.
    setFan(kSamsungAcFanAuto);
    setPowerful(false);  // Quiet 'on' is mutually exclusive to Powerful.
  }
}

/// Get the Powerful (Turbo) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRSamsungAc::getPowerful(void) const {
  return !(_.Powerful8 & kSamsungAcPowerfulMask8) &&
         (_.Powerful10 == kSamsungAcPowerful10On) &&
         (_.Fan == kSamsungAcFanTurbo);
}

/// Set the Powerful (Turbo) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRSamsungAc::setPowerful(const bool on) {
  uint8_t off_value = getBreeze() ? kSamsungAcBreezeOn : 0b000;
  _.Powerful10 = (on ? kSamsungAcPowerful10On : off_value);
  if (on) {
    _.Powerful8 &= ~kSamsungAcPowerfulMask8;  // Bit needs to be cleared.
    // Powerful mode sets fan speed to Turbo.
    setFan(kSamsungAcFanTurbo);
    setQuiet(false);  // Powerful 'on' is mutually exclusive to Quiet.
  } else {
    _.Powerful8 |= kSamsungAcPowerfulMask8;  // Bit needs to be set.
    // Turning off Powerful mode sets fan speed to Auto if we were in Turbo mode
    if (_.Fan == kSamsungAcFanTurbo) setFan(kSamsungAcFanAuto);
  }
}

/// Are the vanes closed over the fan outlet, to stop direct wind? Aka. WindFree
/// @return true, the setting is on. false, the setting is off.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1062
bool IRSamsungAc::getBreeze(void) const {
  return (_.Breeze == kSamsungAcBreezeOn) &&
         (_.Fan == kSamsungAcFanAuto && !getSwing());
}

/// Closes the vanes over the fan outlet, to stop direct wind. Aka. WindFree
/// @param[in] on true, the setting is on. false, the setting is off.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1062
void IRSamsungAc::setBreeze(const bool on) {
  uint8_t off_value = getPowerful() ? kSamsungAcPowerful10On : 0b000;
  _.Breeze = (on ? kSamsungAcBreezeOn : off_value);
  if (on) {
    setFan(kSamsungAcFanAuto);
    setSwing(false);
  }
}

/// Get the Display (Light/LED) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRSamsungAc::getDisplay(void) const {
  return _.Display;
}

/// Set the Display (Light/LED) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRSamsungAc::setDisplay(const bool on) {
  _.Display = on;
}

/// Get the Ion (Filter) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRSamsungAc::getIon(void) const {
  return _.Ion;
}

/// Set the Ion (Filter) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRSamsungAc::setIon(const bool on) {
  _.Ion = on;
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRSamsungAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kSamsungAcCool;
    case stdAc::opmode_t::kHeat: return kSamsungAcHeat;
    case stdAc::opmode_t::kDry:  return kSamsungAcDry;
    case stdAc::opmode_t::kFan:  return kSamsungAcFan;
    default:                     return kSamsungAcAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRSamsungAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kSamsungAcFanLow;
    case stdAc::fanspeed_t::kMedium: return kSamsungAcFanMed;
    case stdAc::fanspeed_t::kHigh:   return kSamsungAcFanHigh;
    case stdAc::fanspeed_t::kMax:    return kSamsungAcFanTurbo;
    default:                         return kSamsungAcFanAuto;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRSamsungAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kSamsungAcCool: return stdAc::opmode_t::kCool;
    case kSamsungAcHeat: return stdAc::opmode_t::kHeat;
    case kSamsungAcDry:  return stdAc::opmode_t::kDry;
    case kSamsungAcFan:  return stdAc::opmode_t::kFan;
    default:             return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] spd The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRSamsungAc::toCommonFanSpeed(const uint8_t spd) {
  switch (spd) {
    case kSamsungAcFanTurbo: return stdAc::fanspeed_t::kMax;
    case kSamsungAcFanHigh:  return stdAc::fanspeed_t::kHigh;
    case kSamsungAcFanMed:   return stdAc::fanspeed_t::kMedium;
    case kSamsungAcFanLow:   return stdAc::fanspeed_t::kMin;
    default:                 return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRSamsungAc::toCommon(void) const {
  stdAc::state_t result;
  result.protocol = decode_type_t::SAMSUNG_AC;
  result.model = -1;  // Not supported.
  result.power = getPower();
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.swingv = getSwing() ? stdAc::swingv_t::kAuto :
                                     stdAc::swingv_t::kOff;
  result.quiet = getQuiet();
  result.turbo = getPowerful();
  result.clean = getClean();
  result.beep = _.Beep;
  result.light = _.Display;
  result.filter = _.Ion;
  // Not supported.
  result.swingh = stdAc::swingh_t::kOff;
  result.econo = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRSamsungAc::toString(void) const {
  String result = "";
  result.reserve(115);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(_.Mode, kSamsungAcAuto, kSamsungAcCool,
                            kSamsungAcHeat, kSamsungAcDry,
                            kSamsungAcFan);
  result += addTempToString(getTemp());
  result += addIntToString(_.Fan, kFanStr);
  result += kSpaceLBraceStr;
  switch (_.Fan) {
    case kSamsungAcFanAuto:
    case kSamsungAcFanAuto2:
      result += kAutoStr;
      break;
    case kSamsungAcFanLow:
      result += kLowStr;
      break;
    case kSamsungAcFanMed:
      result += kMedStr;
      break;
    case kSamsungAcFanHigh:
      result += kHighStr;
      break;
    case kSamsungAcFanTurbo:
      result += kTurboStr;
      break;
    default:
      result += kUnknownStr;
      break;
  }
  result += ')';
  result += addBoolToString(getSwing(), kSwingStr);
  result += addBoolToString(_.Beep, kBeepStr);
  result += addBoolToString(getClean(), kCleanStr);
  result += addBoolToString(getQuiet(), kQuietStr);
  result += addBoolToString(getPowerful(), kPowerfulStr);
  result += addBoolToString(getBreeze(), kBreezeStr);
  result += addBoolToString(_.Display, kLightStr);
  result += addBoolToString(_.Ion, kIonStr);
  return result;
}

#if DECODE_SAMSUNG_AC
/// Decode the supplied Samsung A/C message.
/// Status: Stable / Known to be working.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/505
bool IRrecv::decodeSamsungAC(decode_results *results, uint16_t offset,
                             const uint16_t nbits, const bool strict) {
  if (results->rawlen < 2 * nbits + kHeader * 3 + kFooter * 2 - 1 + offset)
    return false;  // Can't possibly be a valid Samsung A/C message.
  if (nbits != kSamsungAcBits && nbits != kSamsungAcExtendedBits) return false;

  // Message Header
  if (!matchMark(results->rawbuf[offset++], kSamsungAcBitMark)) return false;
  if (!matchSpace(results->rawbuf[offset++], kSamsungAcHdrSpace)) return false;
  // Section(s)
  for (uint16_t pos = 0; pos <= (nbits / 8) - kSamsungAcSectionLength;
       pos += kSamsungAcSectionLength) {
    uint16_t used;
    // Section Header + Section Data (7 bytes) + Section Footer
    used = matchGeneric(results->rawbuf + offset, results->state + pos,
                        results->rawlen - offset, kSamsungAcSectionLength * 8,
                        kSamsungAcSectionMark, kSamsungAcSectionSpace,
                        kSamsungAcBitMark, kSamsungAcOneSpace,
                        kSamsungAcBitMark, kSamsungAcZeroSpace,
                        kSamsungAcBitMark, kSamsungAcSectionGap,
                        pos + kSamsungAcSectionLength >= nbits / 8,
                        _tolerance, 0, false);
    if (used == 0) return false;
    offset += used;
  }
  // Compliance
  // Is the signature correct?
  DPRINTLN("DEBUG: Checking signature.");
  if (results->state[0] != 0x02 || results->state[2] != 0x0F) return false;
  if (strict) {
    // Is the checksum valid?
    if (!IRSamsungAc::validChecksum(results->state, nbits / 8)) {
      DPRINTLN("DEBUG: Checksum failed!");
      return false;
    }
  }
  // Success
  results->decode_type = SAMSUNG_AC;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_SAMSUNG_AC
