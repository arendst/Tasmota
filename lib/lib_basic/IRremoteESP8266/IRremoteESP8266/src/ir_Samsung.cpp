// Copyright 2009 Ken Shirriff
// Copyright 2017-2021 David Conran
/// @file
/// @brief Support for Samsung protocols.
/// Samsung originally added from https://github.com/shirriff/Arduino-IRremote/
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/505
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/621
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1062
/// @see http://elektrolab.wz.cz/katalog/samsung_protocol.pdf
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1538 (Checksum)

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

// _.Swing
const uint8_t kSamsungAcSwingV =        0b010;
const uint8_t kSamsungAcSwingH =        0b011;
const uint8_t kSamsungAcSwingBoth =     0b100;
const uint8_t kSamsungAcSwingOff =      0b111;
// _.FanSpecial
const uint8_t kSamsungAcFanSpecialOff = 0b000;
const uint8_t kSamsungAcPowerfulOn =    0b011;
const uint8_t kSamsungAcBreezeOn =      0b101;
const uint8_t kSamsungAcEconoOn =       0b111;

using irutils::addBoolToString;
using irutils::addFanToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;
using irutils::addToggleToString;
using irutils::minsToString;

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
/// @param[in] extended A flag indicating if force sending a special extended
///   message with the first `send()` call.
/// @param[in] initialPower Set the initial power state. True, on. False, off.
void IRSamsungAc::stateReset(const bool extended, const bool initialPower) {
  static const uint8_t kReset[kSamsungAcExtendedStateLength] = {
      0x02, 0x92, 0x0F, 0x00, 0x00, 0x00, 0xF0,
      0x01, 0x02, 0xAE, 0x71, 0x00, 0x15, 0xF0};
  std::memcpy(_.raw, kReset, kSamsungAcExtendedStateLength);
  _forceextended = extended;
  _lastsentpowerstate = initialPower;
  setPower(initialPower);
  _OnTimerEnable = false;
  _OffTimerEnable = false;
  _Sleep = false;
  _lastSleep = false;
  _OnTimer = _OffTimer = _lastOnTimer = _lastOffTimer = 0;
}

/// Set up hardware to be able to send a message.
void IRSamsungAc::begin(void) { _irsend.begin(); }

/// Get the existing checksum for a given state section.
/// @param[in] section The array to extract the checksum from.
/// @return The existing checksum value.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1538#issuecomment-894645947
uint8_t IRSamsungAc::getSectionChecksum(const uint8_t *section) {
  return ((GETBITS8(*(section + 2), kLowNibble, kNibbleSize) << kNibbleSize) +
          GETBITS8(*(section + 1), kHighNibble, kNibbleSize));
}

/// Calculate the checksum for a given state section.
/// @param[in] section The array to calc the checksum of.
/// @return The calculated checksum value.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1538#issuecomment-894645947
uint8_t IRSamsungAc::calcSectionChecksum(const uint8_t *section) {
  uint8_t sum = 0;

  sum += countBits(*section, 8);  // Include the entire first byte
  // The lower half of the second byte.
  sum += countBits(GETBITS8(*(section + 1), kLowNibble, kNibbleSize), 8);
  // The upper half of the third byte.
  sum += countBits(GETBITS8(*(section + 2), kHighNibble, kNibbleSize), 8);
  // The next 4 bytes.
  sum += countBits(section + 3, 4);
  // Bitwise invert the result.
  return sum ^ UINT8_MAX;
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length/size of the array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRSamsungAc::validChecksum(const uint8_t state[], const uint16_t length) {
  bool result = true;
  const uint16_t maxlength =
      (length > kSamsungAcExtendedStateLength) ? kSamsungAcExtendedStateLength
                                               : length;
  for (uint16_t offset = 0;
       offset + kSamsungAcSectionLength <= maxlength;
       offset += kSamsungAcSectionLength)
    result &= (getSectionChecksum(state + offset) ==
               calcSectionChecksum(state + offset));
  return result;
}

/// Update the checksum for the internal state.
void IRSamsungAc::checksum(void) {
  uint8_t sectionsum = calcSectionChecksum(_.raw);
  _.Sum1Upper = GETBITS8(sectionsum, kHighNibble, kNibbleSize);
  _.Sum1Lower = GETBITS8(sectionsum, kLowNibble, kNibbleSize);
  sectionsum = calcSectionChecksum(_.raw + kSamsungAcSectionLength);
  _.Sum2Upper = GETBITS8(sectionsum, kHighNibble, kNibbleSize);
  _.Sum2Lower = GETBITS8(sectionsum, kLowNibble, kNibbleSize);
  sectionsum = calcSectionChecksum(_.raw + kSamsungAcSectionLength * 2);
  _.Sum3Upper = GETBITS8(sectionsum, kHighNibble, kNibbleSize);
  _.Sum3Lower = GETBITS8(sectionsum, kLowNibble, kNibbleSize);
}

#if SEND_SAMSUNG_AC
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
/// @note Use for most function/mode/settings changes to the unit.
///   i.e. When the device is already running.
void IRSamsungAc::send(const uint16_t repeat) {
  // Do we need to send a special (extended) message?
  if (getPower() != _lastsentpowerstate || _forceextended ||
      (_lastOnTimer != _OnTimer) || (_lastOffTimer != _OffTimer) ||
      (_Sleep != _lastSleep))  // We do.
    sendExtended(repeat);
  else  // No, it's just a normal message.
    _irsend.sendSamsungAC(getRaw(), kSamsungAcStateLength, repeat);
}

/// Send the extended current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
/// @note Samsung A/C requires an extended length message when you want to
/// change the power operating mode, Timers, or Sleep setting of the A/C unit.
void IRSamsungAc::sendExtended(const uint16_t repeat) {
  _lastsentpowerstate = getPower();  // Remember the last power state sent.
  _lastOnTimer = _OnTimer;
  _lastOffTimer = _OffTimer;
  static const uint8_t extended_middle_section[kSamsungAcSectionLength] = {
      0x01, 0xD2, 0x0F, 0x00, 0x00, 0x00, 0x00};
  // Copy/convert the internal state to an extended state by
  // copying the second section to the third section, and inserting the extended
  // middle (second) section.
  std::memcpy(_.raw + 2 * kSamsungAcSectionLength,
              _.raw + kSamsungAcSectionLength,
              kSamsungAcSectionLength);
  std::memcpy(_.raw + kSamsungAcSectionLength, extended_middle_section,
              kSamsungAcSectionLength);
  _setOnTimer();
  _setSleepTimer();  // This also sets any Off Timer if needed too.
  // Send it.
  _irsend.sendSamsungAC(getRaw(), kSamsungAcExtendedStateLength, repeat);
  // Now revert it by copying the third section over the second section.
  std::memcpy(_.raw + kSamsungAcSectionLength,
              _.raw + 2 * kSamsungAcSectionLength,
              kSamsungAcSectionLength);

  _forceextended = false;  // It has now been sent, so clear the flag if set.
}

/// Send the special extended "On" message as the library can't seem to
/// reproduce this message automatically.
/// @param[in] repeat Nr. of times the message will be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/604#issuecomment-475020036
void IRSamsungAc::sendOn(const uint16_t repeat) {
  const uint8_t extended_state[kSamsungAcExtendedStateLength] = {
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
  const uint8_t extended_state[kSamsungAcExtendedStateLength] = {
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
    _OnTimerEnable = _.OnTimerEnable;
    _OffTimerEnable = _.OffTimerEnable;
    _Sleep = _.Sleep5 && _.Sleep12;
    _OnTimer = _getOnTimer();
    _OffTimer = _getOffTimer();
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
  _.Power1 = _.Power2 = (on ? 0b11 : 0b00);
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRSamsungAc::getPower(void) const {
  return _.Power1 == 0b11 && _.Power2 == 0b11;
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
bool IRSamsungAc::getSwing(void) const {
  switch (_.Swing) {
    case kSamsungAcSwingV:
    case kSamsungAcSwingBoth: return true;
    default:                  return false;
  }
}

/// Set the vertical swing setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRSamsungAc::setSwing(const bool on) {
  switch (_.Swing) {
    case kSamsungAcSwingBoth:
    case kSamsungAcSwingH:
      _.Swing = on ? kSamsungAcSwingBoth : kSamsungAcSwingH;
      break;
    default:
      _.Swing = on ? kSamsungAcSwingV : kSamsungAcSwingOff;
  }
}

/// Get the horizontal swing setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRSamsungAc::getSwingH(void) const {
  switch (_.Swing) {
    case kSamsungAcSwingH:
    case kSamsungAcSwingBoth: return true;
    default:                  return false;
  }
}

/// Set the horizontal swing setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRSamsungAc::setSwingH(const bool on) {
  switch (_.Swing) {
    case kSamsungAcSwingV:
    case kSamsungAcSwingBoth:
      _.Swing = on ? kSamsungAcSwingBoth : kSamsungAcSwingV;
      break;
    default:
      _.Swing = on ? kSamsungAcSwingH : kSamsungAcSwingOff;
  }
}

/// Get the Beep toggle setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRSamsungAc::getBeep(void) const { return _.BeepToggle; }

/// Set the Beep toggle setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRSamsungAc::setBeep(const bool on) { _.BeepToggle = on; }

/// Get the Clean toggle setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRSamsungAc::getClean(void) const {
  return _.CleanToggle10 && _.CleanToggle11;
}

/// Set the Clean toggle setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRSamsungAc::setClean(const bool on) {
  _.CleanToggle10 = on;
  _.CleanToggle11 = on;
}

/// Get the Quiet setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRSamsungAc::getQuiet(void) const { return _.Quiet; }

/// Set the Quiet setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRSamsungAc::setQuiet(const bool on) {
  _.Quiet = on;
  if (on) {
    // Quiet mode seems to set fan speed to auto.
    setFan(kSamsungAcFanAuto);
    setPowerful(false);  // Quiet 'on' is mutually exclusive to Powerful.
  }
}

/// Get the Powerful (Turbo) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRSamsungAc::getPowerful(void) const {
  return (_.FanSpecial == kSamsungAcPowerfulOn) &&
         (_.Fan == kSamsungAcFanTurbo);
}

/// Set the Powerful (Turbo) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRSamsungAc::setPowerful(const bool on) {
  uint8_t off_value = (getBreeze() || getEcono()) ? _.FanSpecial
                                                  : kSamsungAcFanSpecialOff;
  _.FanSpecial = (on ? kSamsungAcPowerfulOn : off_value);
  if (on) {
    // Powerful mode sets fan speed to Turbo.
    setFan(kSamsungAcFanTurbo);
    setQuiet(false);  // Powerful 'on' is mutually exclusive to Quiet.
  }
}

/// Are the vanes closed over the fan outlet, to stop direct wind? Aka. WindFree
/// @return true, the setting is on. false, the setting is off.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1062
bool IRSamsungAc::getBreeze(void) const {
  return (_.FanSpecial == kSamsungAcBreezeOn) &&
         (_.Fan == kSamsungAcFanAuto && !getSwing());
}

/// Closes the vanes over the fan outlet, to stop direct wind. Aka. WindFree
/// @param[in] on true, the setting is on. false, the setting is off.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1062
void IRSamsungAc::setBreeze(const bool on) {
  const uint8_t off_value = (getPowerful() ||
                             getEcono()) ? _.FanSpecial
                                         : kSamsungAcFanSpecialOff;
  _.FanSpecial = (on ? kSamsungAcBreezeOn : off_value);
  if (on) {
    setFan(kSamsungAcFanAuto);
    setSwing(false);
  }
}

/// Get the current Economy (Eco) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRSamsungAc::getEcono(void) const {
  return (_.FanSpecial == kSamsungAcEconoOn) &&
         (_.Fan == kSamsungAcFanAuto && getSwing());
}

/// Set the current Economy (Eco) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRSamsungAc::setEcono(const bool on) {
  const uint8_t off_value = (getBreeze() ||
                             getPowerful()) ? _.FanSpecial
                                            : kSamsungAcFanSpecialOff;
  _.FanSpecial = (on ? kSamsungAcEconoOn : off_value);
  if (on) {
    setFan(kSamsungAcFanAuto);
    setSwing(true);
  }
}

/// Get the Display (Light/LED) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRSamsungAc::getDisplay(void) const { return _.Display; }

/// Set the Display (Light/LED) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRSamsungAc::setDisplay(const bool on) { _.Display = on; }

/// Get the Ion (Filter) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRSamsungAc::getIon(void) const { return _.Ion; }

/// Set the Ion (Filter) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRSamsungAc::setIon(const bool on) { _.Ion = on; }

/// Get the On Timer setting of the A/C from a raw extended state.
/// @return The Nr. of minutes the On Timer is set for.
uint16_t IRSamsungAc::_getOnTimer(void) const {
  if (_.OnTimeDay) return 24 * 60;
  return (_.OnTimeHrs2 * 2 + _.OnTimeHrs1) * 60 + _.OnTimeMins * 10;
}

/// Set the current On Timer value of the A/C into the raw extended state.
void IRSamsungAc::_setOnTimer(void) {
  _.OnTimerEnable = _OnTimerEnable = (_OnTimer > 0);
  _.OnTimeDay = (_OnTimer >= 24 * 60);
  if (_.OnTimeDay) {
    _.OnTimeHrs2 = _.OnTimeHrs1 = _.OnTimeMins = 0;
    return;
  }
  _.OnTimeMins = (_OnTimer % 60) / 10;
  const uint8_t hours = _OnTimer / 60;
  _.OnTimeHrs1 = hours & 0b1;
  _.OnTimeHrs2 = hours >> 1;
}

/// Get the Off Timer setting of the A/C from a raw extended state.
/// @return The Nr. of minutes the Off Timer is set for.
uint16_t IRSamsungAc::_getOffTimer(void) const {
  if (_.OffTimeDay) return 24 * 60;
  return (_.OffTimeHrs2 * 2 + _.OffTimeHrs1) * 60 + _.OffTimeMins * 10;
}

/// Set the current Off Timer value of the A/C into the raw extended state.
void IRSamsungAc::_setOffTimer(void) {
  _.OffTimerEnable = _OffTimerEnable = (_OffTimer > 0);
  _.OffTimeDay = (_OffTimer >= 24 * 60);
  if (_.OffTimeDay) {
    _.OffTimeHrs2 = _.OffTimeHrs1 = _.OffTimeMins = 0;
    return;
  }
  _.OffTimeMins = (_OffTimer % 60) / 10;
  const uint8_t hours = _OffTimer / 60;
  _.OffTimeHrs1 = hours & 0b1;
  _.OffTimeHrs2 = hours >> 1;
}

// Set the current Sleep Timer value of the A/C into the raw extended state.
void IRSamsungAc::_setSleepTimer(void) {
  _setOffTimer();
  // The Sleep mode/timer should only be engaged if an off time has been set.
  _.Sleep5 = _Sleep && _OffTimerEnable;
  _.Sleep12 = _.Sleep5;
}

/// Get the On Timer setting of the A/C.
/// @return The Nr. of minutes the On Timer is set for.
uint16_t IRSamsungAc::getOnTimer(void) const { return _OnTimer; }

/// Get the Off Timer setting of the A/C.
/// @return The Nr. of minutes the Off Timer is set for.
/// @note Sleep & Off Timer share the same timer.
uint16_t IRSamsungAc::getOffTimer(void) const {
  return _Sleep ? 0 : _OffTimer;
}

/// Get the Sleep Timer setting of the A/C.
/// @return The Nr. of minutes the Off Timer is set for.
/// @note Sleep & Off Timer share the same timer.
uint16_t IRSamsungAc::getSleepTimer(void) const {
  return _Sleep ? _OffTimer : 0;
}

#define TIMER_RESOLUTION(mins) \
    (((std::min((mins), (uint16_t)(24 * 60))) / 10) * 10)

/// Set the On Timer value of the A/C.
/// @param[in] nr_of_mins The number of minutes the timer should be.
/// @note The timer time only has a resolution of 10 mins.
/// @note Setting the On Timer active will cancel the Sleep timer/setting.
void IRSamsungAc::setOnTimer(const uint16_t nr_of_mins) {
  // Limit to one day, and round down to nearest 10 min increment.
  _OnTimer = TIMER_RESOLUTION(nr_of_mins);
  _OnTimerEnable = _OnTimer > 0;
  if (_OnTimer) _Sleep = false;
}

/// Set the Off Timer value of the A/C.
/// @param[in] nr_of_mins The number of minutes the timer should be.
/// @note The timer time only has a resolution of 10 mins.
/// @note Setting the Off Timer active will cancel the Sleep timer/setting.
void IRSamsungAc::setOffTimer(const uint16_t nr_of_mins) {
  // Limit to one day, and round down to nearest 10 min increment.
  _OffTimer = TIMER_RESOLUTION(nr_of_mins);
  _OffTimerEnable = _OffTimer > 0;
  if (_OffTimer) _Sleep = false;
}

/// Set the Sleep Timer value of the A/C.
/// @param[in] nr_of_mins The number of minutes the timer should be.
/// @note The timer time only has a resolution of 10 mins.
/// @note Sleep timer acts as an Off timer, and cancels any On Timer.
void IRSamsungAc::setSleepTimer(const uint16_t nr_of_mins) {
  // Limit to one day, and round down to nearest 10 min increment.
  _OffTimer = TIMER_RESOLUTION(nr_of_mins);
  if (_OffTimer) setOnTimer(0);  // Clear the on timer if set.
  _Sleep = _OffTimer > 0;
  _OffTimerEnable = _Sleep;
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
  result.swingv = getSwing() ? stdAc::swingv_t::kAuto : stdAc::swingv_t::kOff;
  result.swingh = getSwingH() ? stdAc::swingh_t::kAuto : stdAc::swingh_t::kOff;
  result.quiet = getQuiet();
  result.turbo = getPowerful();
  result.econo = getEcono();
  result.clean = getClean();
  result.beep = _.BeepToggle;
  result.light = _.Display;
  result.filter = _.Ion;
  result.sleep = _Sleep ? getSleepTimer() : -1;
  // Not supported.
  result.clock = -1;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRSamsungAc::toString(void) const {
  String result = "";
  result.reserve(230);  // Reserve some heap for the string to reduce fragging.
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
  result += addBoolToString(getSwing(), kSwingVStr);
  result += addBoolToString(getSwingH(), kSwingHStr);
  result += addToggleToString(_.BeepToggle, kBeepStr);
  result += addToggleToString(getClean(), kCleanStr);
  result += addBoolToString(getQuiet(), kQuietStr);
  result += addBoolToString(getPowerful(), kPowerfulStr);
  result += addBoolToString(getEcono(), kEconoStr);
  result += addBoolToString(getBreeze(), kBreezeStr);
  result += addBoolToString(_.Display, kLightStr);
  result += addBoolToString(_.Ion, kIonStr);
  if (_OnTimerEnable)
    result += addLabeledString(minsToString(_OnTimer), kOnTimerStr);
  if (_OffTimerEnable)
    result += addLabeledString(minsToString(_OffTimer),
                               _Sleep ? kSleepTimerStr : kOffTimerStr);
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
