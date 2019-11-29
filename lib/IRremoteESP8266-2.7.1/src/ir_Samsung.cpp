// Copyright 2009 Ken Shirriff
// Copyright 2017, 2018, 2019 David Conran

// Samsung remote emulation

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

// Samsung originally added from https://github.com/shirriff/Arduino-IRremote/

// Constants
// Ref:
//   http://elektrolab.wz.cz/katalog/samsung_protocol.pdf
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

using irutils::addBoolToString;
using irutils::addFanToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;
using irutils::setBit;
using irutils::setBits;

#if SEND_SAMSUNG
// Send a Samsung formatted message.
// Samsung has a separate message to indicate a repeat, like NEC does.
// TODO(crankyoldgit): Confirm that is actually how Samsung sends a repeat.
//                     The refdoc doesn't indicate it is true.
//
// Args:
//   data:   The message to be sent.
//   nbits:  The bit size of the message being sent. typically kSamsungBits.
//   repeat: The number of times the message is to be repeated.
//
// Status: BETA / Should be working.
//
// Ref: http://elektrolab.wz.cz/katalog/samsung_protocol.pdf
void IRsend::sendSAMSUNG(const uint64_t data, const uint16_t nbits,
                         const uint16_t repeat) {
  sendGeneric(kSamsungHdrMark, kSamsungHdrSpace, kSamsungBitMark,
              kSamsungOneSpace, kSamsungBitMark, kSamsungZeroSpace,
              kSamsungBitMark, kSamsungMinGap, kSamsungMinMessageLength, data,
              nbits, 38, true, repeat, 33);
}

// Construct a raw Samsung message from the supplied customer(address) &
// command.
//
// Args:
//   customer: The customer code. (aka. Address)
//   command:  The command code.
// Returns:
//   A raw 32-bit Samsung message suitable for sendSAMSUNG().
//
// Status: BETA / Should be working.
uint32_t IRsend::encodeSAMSUNG(const uint8_t customer, const uint8_t command) {
  uint8_t revcustomer = reverseBits(customer, sizeof(customer) * 8);
  uint8_t revcommand = reverseBits(command, sizeof(command) * 8);
  return ((revcommand ^ 0xFF) | (revcommand << 8) | (revcustomer << 16) |
          (revcustomer << 24));
}
#endif

#if DECODE_SAMSUNG
// Decode the supplied Samsung message.
// Samsung messages whilst 32 bits in size, only contain 16 bits of distinct
// data. e.g. In transmition order:
//   customer_byte + customer_byte(same) + address_byte + invert(address_byte)
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of bits to expect in the data portion. Typically kSamsungBits.
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: STABLE
//
// Note:
//   LG 32bit protocol appears near identical to the Samsung protocol.
//   They differ on their compliance criteria and how they repeat.
// Ref:
//  http://elektrolab.wz.cz/katalog/samsung_protocol.pdf
bool IRrecv::decodeSAMSUNG(decode_results *results, const uint16_t nbits,
                           const bool strict) {
  if (results->rawlen < 2 * nbits + kHeader + kFooter - 1)
    return false;  // Can't possibly be a valid Samsung message.
  if (strict && nbits != kSamsungBits)
    return false;  // We expect Samsung to be 32 bits of message.

  uint64_t data = 0;
  uint16_t offset = kStartOffset;

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
// Send a Samsung 36-bit formatted message.
//
// Args:
//   data:   The message to be sent.
//   nbits:  The bit size of the message being sent. typically kSamsung36Bits.
//   repeat: The number of times the message is to be repeated.
//
// Status: Alpha / Experimental.
//
// Note:
//   Protocol is used by Samsung Bluray Remote: ak59-00167a
//
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/621
void IRsend::sendSamsung36(const uint64_t data, const uint16_t nbits,
                           const uint16_t repeat) {
  if (nbits < 16) return;  // To small to send.
  for (uint16_t r = 0; r <= repeat; r++) {
    // Block #1 (16 bits)
    sendGeneric(kSamsungHdrMark, kSamsungHdrSpace,
                kSamsungBitMark, kSamsungOneSpace,
                kSamsungBitMark, kSamsungZeroSpace,
                kSamsungBitMark, kSamsungHdrSpace,
                data >> (nbits - 16), 16, 38, true, 0, kDutyDefault);
    // Block #2 (The rest, typically 20 bits)
    sendGeneric(0, 0,  // No header
                kSamsungBitMark, kSamsungOneSpace,
                kSamsungBitMark, kSamsungZeroSpace,
                kSamsungBitMark, kSamsungMinGap,  // Gap is just a guess.
                // Mask off the rest of the bits.
                data & ((1ULL << (nbits - 16)) - 1),
                nbits - 16, 38, true, 0, kDutyDefault);
  }
}
#endif  // SEND_SAMSUNG36

#if DECODE_SAMSUNG36
// Decode the supplied Samsung36 message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of bits to expect in the data portion.
//            Typically kSamsung36Bits.
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: Alpha / Experimental
//
// Note:
//   Protocol is used by Samsung Bluray Remote: ak59-00167a
//
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/621
bool IRrecv::decodeSamsung36(decode_results *results, const uint16_t nbits,
                             const bool strict) {
  if (results->rawlen < 2 * nbits + kHeader + kFooter * 2 - 1)
    return false;  // Can't possibly be a valid Samsung message.
  // We need to be looking for > 16 bits to make sense.
  if (nbits <= 16) return false;
  if (strict && nbits != kSamsung36Bits)
    return false;  // We expect nbits to be 36 bits of message.

  uint64_t data = 0;
  uint16_t offset = kStartOffset;

  // Match Header + Data + Footer
  uint16_t used;
  used = matchGeneric(results->rawbuf + offset, &data,
                      results->rawlen - offset, 16,
                      kSamsungHdrMark, kSamsungHdrSpace,
                      kSamsungBitMark, kSamsungOneSpace,
                      kSamsungBitMark, kSamsungZeroSpace,
                      kSamsungBitMark, kSamsungHdrSpace, false);
  if (!used) return false;
  offset += used;
  // Data (Block #2)
  uint64_t data2 = 0;
  if (!matchGeneric(results->rawbuf + offset, &data2,
                    results->rawlen - offset, nbits - 16,
                    0, 0,
                    kSamsungBitMark, kSamsungOneSpace,
                    kSamsungBitMark, kSamsungZeroSpace,
                    kSamsungBitMark, kSamsungMinGap, true)) return false;
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
// Send a Samsung A/C message.
//
// Args:
//   data: An array of bytes containing the IR command.
//   nbytes: Nr. of bytes of data in the array. (>=kSamsungAcStateLength)
//   repeat: Nr. of times the message is to be repeated. (Default = 0).
//
// Status: Stable / Known working.
//
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/505
void IRsend::sendSamsungAC(const uint8_t data[], const uint16_t nbytes,
                           const uint16_t repeat) {
  if (nbytes < kSamsungAcStateLength && nbytes % kSamsungACSectionLength)
    return;  // Not an appropriate number of bytes to send a proper message.

  enableIROut(38);
  for (uint16_t r = 0; r <= repeat; r++) {
    // Header
    mark(kSamsungAcHdrMark);
    space(kSamsungAcHdrSpace);
    // Send in 7 byte sections.
    for (uint16_t offset = 0; offset < nbytes;
         offset += kSamsungACSectionLength) {
      sendGeneric(kSamsungAcSectionMark, kSamsungAcSectionSpace,
                  kSamsungAcBitMark, kSamsungAcOneSpace, kSamsungAcBitMark,
                  kSamsungAcZeroSpace, kSamsungAcBitMark, kSamsungAcSectionGap,
                  data + offset, kSamsungACSectionLength,  // 7 bytes == 56 bits
                  38000, false, 0, 50);                    // Send in LSBF order
    }
    // Complete made up guess at inter-message gap.
    space(kDefaultMessageGap - kSamsungAcSectionGap);
  }
}
#endif  // SEND_SAMSUNG_AC

IRSamsungAc::IRSamsungAc(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) {
  this->stateReset();
}

// Reset the internal state of the emulation.
// Args:
//   forcepower: A flag indicating if force sending a special power message
//              with the first `send()` call. Default: true
void IRSamsungAc::stateReset(const bool forcepower, const bool initialPower) {
  static const uint8_t kReset[kSamsungAcExtendedStateLength] = {
      0x02, 0x92, 0x0F, 0x00, 0x00, 0x00, 0xF0, 0x01, 0x02, 0xAE, 0x71, 0x00,
      0x15, 0xF0};
  memcpy(remote_state, kReset, kSamsungAcExtendedStateLength);
  _forcepower = forcepower;
  _lastsentpowerstate = initialPower;
  setPower(initialPower);
}

void IRSamsungAc::begin(void) { _irsend.begin(); }

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

// Update the checksum for the internal state.
void IRSamsungAc::checksum(uint16_t length) {
  if (length < 13) return;
  setBits(&remote_state[length - 6], kHighNibble, kNibbleSize,
          this->calcChecksum(remote_state, length));
  setBits(&remote_state[length - 13], kHighNibble, kNibbleSize,
          this->calcChecksum(remote_state, length - 7));
}

#if SEND_SAMSUNG_AC
// Use for most function/mode/settings changes to the unit.
// i.e. When the device is already running.
void IRSamsungAc::send(const uint16_t repeat, const bool calcchecksum) {
  if (calcchecksum) this->checksum();
  // Do we need to send a the special power on/off message?
  if (this->getPower() != _lastsentpowerstate || _forcepower) {
    _forcepower = false;  // It will now been sent, so clear the flag if set.
    if (this->getPower()) {
      this->sendOn(repeat);
    } else {
      this->sendOff(repeat);
      return;  // No point sending anything else if we are turning the unit off.
    }
  }
  _irsend.sendSamsungAC(remote_state, kSamsungAcStateLength, repeat);
}

// Use this for when you need to power on/off the device.
// Samsung A/C requires an extended length message when you want to
// change the power operating mode of the A/C unit.
void IRSamsungAc::sendExtended(const uint16_t repeat, const bool calcchecksum) {
  if (calcchecksum) this->checksum();
  uint8_t extended_state[kSamsungAcExtendedStateLength] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x01, 0xD2, 0x0F, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  // Copy/convert the internal state to an extended state.
  for (uint16_t i = 0; i < kSamsungACSectionLength; i++)
    extended_state[i] = remote_state[i];
  for (uint16_t i = kSamsungACSectionLength; i < kSamsungAcStateLength; i++)
    extended_state[i + kSamsungACSectionLength] = remote_state[i];
  // extended_state[8] seems special. This is a guess on how to calculate it.
  extended_state[8] = (extended_state[1] & 0x9F) | 0x40;
  // Send it.
  _irsend.sendSamsungAC(extended_state, kSamsungAcExtendedStateLength, repeat);
}

// Send the special extended "On" message as the library can't seem to reproduce
// this message automatically.
// See: https://github.com/crankyoldgit/IRremoteESP8266/issues/604#issuecomment-475020036
void IRSamsungAc::sendOn(const uint16_t repeat) {
  const uint8_t extended_state[21] = {
      0x02, 0x92, 0x0F, 0x00, 0x00, 0x00, 0xF0,
      0x01, 0xD2, 0x0F, 0x00, 0x00, 0x00, 0x00,
      0x01, 0xE2, 0xFE, 0x71, 0x80, 0x11, 0xF0};
  _irsend.sendSamsungAC(extended_state, kSamsungAcExtendedStateLength, repeat);
  _lastsentpowerstate = true;  // On
}

// Send the special extended "Off" message as the library can't seem to
// reproduce this message automatically.
// See: https://github.com/crankyoldgit/IRremoteESP8266/issues/604#issuecomment-475020036
void IRSamsungAc::sendOff(const uint16_t repeat) {
  const uint8_t extended_state[21] = {
      0x02, 0xB2, 0x0F, 0x00, 0x00, 0x00, 0xC0,
      0x01, 0xD2, 0x0F, 0x00, 0x00, 0x00, 0x00,
      0x01, 0x02, 0xFF, 0x71, 0x80, 0x11, 0xC0};
  _irsend.sendSamsungAC(extended_state, kSamsungAcExtendedStateLength, repeat);
  _lastsentpowerstate = false;  // Off
}
#endif  // SEND_SAMSUNG_AC

uint8_t *IRSamsungAc::getRaw(void) {
  this->checksum();
  return remote_state;
}

void IRSamsungAc::setRaw(const uint8_t new_code[], const uint16_t length) {
  memcpy(remote_state, new_code, std::min(length,
                                          kSamsungAcExtendedStateLength));
  // Shrink the extended state into a normal state.
  if (length > kSamsungAcStateLength) {
    for (uint8_t i = kSamsungAcStateLength; i < length; i++)
      remote_state[i - kSamsungACSectionLength] = remote_state[i];
  }
}

void IRSamsungAc::on(void) { setPower(true); }

void IRSamsungAc::off(void) { setPower(false); }

void IRSamsungAc::setPower(const bool on) {
  setBit(&remote_state[1], kSamsungAcPower1Offset, !on);  // Cleared when on.
  setBits(&remote_state[6], kSamsungAcPower6Offset, kSamsungAcPower6Size,
          on ? 0b11 : 0b00);
}

bool IRSamsungAc::getPower(void) {
  return (GETBITS8(remote_state[6], kSamsungAcPower6Offset,
                   kSamsungAcPower6Size) == 0b11) &&
      !GETBIT8(remote_state[1], kSamsungAcPower1Offset);
}

// Set the temp. in deg C
void IRSamsungAc::setTemp(const uint8_t temp) {
  uint8_t newtemp = std::max(kSamsungAcMinTemp, temp);
  newtemp = std::min(kSamsungAcMaxTemp, newtemp);
  setBits(&remote_state[11], kHighNibble, kNibbleSize,
          newtemp - kSamsungAcMinTemp);
}

// Return the set temp. in deg C
uint8_t IRSamsungAc::getTemp(void) {
  return GETBITS8(remote_state[11], kHighNibble, kNibbleSize) +
      kSamsungAcMinTemp;
}

void IRSamsungAc::setMode(const uint8_t mode) {
  // If we get an unexpected mode, default to AUTO.
  uint8_t newmode = mode;
  if (newmode > kSamsungAcHeat) newmode = kSamsungAcAuto;
  setBits(&remote_state[12], kSamsungAcModeOffset, kModeBitsSize, newmode);

  // Auto mode has a special fan setting valid only in auto mode.
  if (newmode == kSamsungAcAuto) {
    this->setFan(kSamsungAcFanAuto2);
  } else {
    // Non-Auto can't have this fan setting
    if (this->getFan() == kSamsungAcFanAuto2)
      this->setFan(kSamsungAcFanAuto);  // Default to something safe.
  }
}

uint8_t IRSamsungAc::getMode(void) {
  return GETBITS8(remote_state[12], kSamsungAcModeOffset, kModeBitsSize);
}

void IRSamsungAc::setFan(const uint8_t speed) {
  switch (speed) {
    case kSamsungAcFanAuto:
    case kSamsungAcFanLow:
    case kSamsungAcFanMed:
    case kSamsungAcFanHigh:
    case kSamsungAcFanTurbo:
      if (this->getMode() == kSamsungAcAuto) return;  // Not valid in Auto mode.
      break;
    case kSamsungAcFanAuto2:  // Special fan setting for when in Auto mode.
      if (this->getMode() != kSamsungAcAuto) return;
      break;
    default:
      return;
  }
  setBits(&remote_state[12], kSamsungAcFanOffest, kSamsungAcFanSize, speed);
}

uint8_t IRSamsungAc::getFan(void) {
  return GETBITS8(remote_state[12], kSamsungAcFanOffest, kSamsungAcFanSize);
}

bool IRSamsungAc::getSwing(void) {
  // TODO(Hollako): Explain why sometimes the LSB of remote_state[9] is a 1.
  // e.g. 0xAE or 0XAF for swing move.
  return GETBITS8(remote_state[9], kSamsungAcSwingOffset,
                  kSamsungAcSwingSize) == kSamsungAcSwingMove;
}

void IRSamsungAc::setSwing(const bool on) {
  // TODO(Hollako): Explain why sometimes the LSB of remote_state[9] is a 1.
  // e.g. 0xAE or 0XAF for swing move.
  setBits(&remote_state[9], kSamsungAcSwingOffset, kSamsungAcSwingSize,
          on ? kSamsungAcSwingMove : kSamsungAcSwingStop);
}

bool IRSamsungAc::getBeep(void) {
  return GETBIT8(remote_state[13], kSamsungAcBeepOffset);
}

void IRSamsungAc::setBeep(const bool on) {
  setBit(&remote_state[13], kSamsungAcBeepOffset, on);
}

bool IRSamsungAc::getClean(void) {
  return GETBIT8(remote_state[10], kSamsungAcClean10Offset) &&
         GETBIT8(remote_state[11], kSamsungAcClean11Offset);
}

void IRSamsungAc::setClean(const bool on) {
  setBit(&remote_state[10], kSamsungAcClean10Offset, on);
  setBit(&remote_state[11], kSamsungAcClean11Offset, on);
}

bool IRSamsungAc::getQuiet(void) {
  return !GETBIT8(remote_state[1], kSamsungAcQuiet1Offset) &&
         GETBIT8(remote_state[5], kSamsungAcQuiet5Offset);
}

void IRSamsungAc::setQuiet(const bool on) {
  setBit(&remote_state[1], kSamsungAcQuiet1Offset, !on);  // Cleared when on.
  setBit(&remote_state[5], kSamsungAcQuiet5Offset, on);
  if (on) {
    // Quiet mode seems to set fan speed to auto.
    this->setFan(kSamsungAcFanAuto);
    this->setPowerful(false);  // Quiet 'on' is mutually exclusive to Powerful.
  }
}

bool IRSamsungAc::getPowerful(void) {
  return !(remote_state[8] & kSamsungAcPowerfulMask8) &&
         GETBITS8(remote_state[10], kSamsungAcPowerful10Offset,
                  kSamsungAcPowerful10Offset) &&
         (this->getFan() == kSamsungAcFanTurbo);
}

void IRSamsungAc::setPowerful(const bool on) {
  setBits(&remote_state[10], kSamsungAcPowerful10Offset,
          kSamsungAcPowerful10Offset, on ? 0b11: 0b00);
  if (on) {
    remote_state[8] &= ~kSamsungAcPowerfulMask8;  // Bit needs to be cleared.
    // Powerful mode sets fan speed to Turbo.
    this->setFan(kSamsungAcFanTurbo);
    this->setQuiet(false);  // Powerful 'on' is mutually exclusive to Quiet.
  } else {
    remote_state[8] |= kSamsungAcPowerfulMask8;  // Bit needs to be set.
    // Turning off Powerful mode sets fan speed to Auto if we were in Turbo mode
    if (this->getFan() == kSamsungAcFanTurbo) this->setFan(kSamsungAcFanAuto);
  }
}

// Convert a standard A/C mode into its native mode.
uint8_t IRSamsungAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kSamsungAcCool;
    case stdAc::opmode_t::kHeat: return kSamsungAcHeat;
    case stdAc::opmode_t::kDry:  return kSamsungAcDry;
    case stdAc::opmode_t::kFan:  return kSamsungAcFan;
    default:                     return kSamsungAcAuto;
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
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

// Convert a native mode to it's common equivalent.
stdAc::opmode_t IRSamsungAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kSamsungAcCool: return stdAc::opmode_t::kCool;
    case kSamsungAcHeat: return stdAc::opmode_t::kHeat;
    case kSamsungAcDry:  return stdAc::opmode_t::kDry;
    case kSamsungAcFan:  return stdAc::opmode_t::kFan;
    default:             return stdAc::opmode_t::kAuto;
  }
}

// Convert a native fan speed to it's common equivalent.
stdAc::fanspeed_t IRSamsungAc::toCommonFanSpeed(const uint8_t spd) {
  switch (spd) {
    case kSamsungAcFanTurbo: return stdAc::fanspeed_t::kMax;
    case kSamsungAcFanHigh:  return stdAc::fanspeed_t::kHigh;
    case kSamsungAcFanMed:   return stdAc::fanspeed_t::kMedium;
    case kSamsungAcFanLow:   return stdAc::fanspeed_t::kMin;
    default:                 return stdAc::fanspeed_t::kAuto;
  }
}

// Convert the A/C state to it's common equivalent.
stdAc::state_t IRSamsungAc::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::SAMSUNG_AC;
  result.model = -1;  // Not supported.
  result.power = this->getPower();
  result.mode = this->toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = this->toCommonFanSpeed(this->getFan());
  result.swingv = this->getSwing() ? stdAc::swingv_t::kAuto :
                                     stdAc::swingv_t::kOff;
  result.quiet = this->getQuiet();
  result.turbo = this->getPowerful();
  result.clean = this->getClean();
  result.beep = this->getBeep();
  // Not supported.
  result.swingh = stdAc::swingh_t::kOff;
  result.econo = false;
  result.filter = false;
  result.light = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

// Convert the internal state into a human readable string.
String IRSamsungAc::toString(void) {
  String result = "";
  result.reserve(100);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(getMode(), kSamsungAcAuto, kSamsungAcCool,
                            kSamsungAcHeat, kSamsungAcDry,
                            kSamsungAcFan);
  result += addTempToString(getTemp());
  result += addIntToString(getFan(), kFanStr);
  result += kSpaceLBraceStr;
  switch (getFan()) {
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
  result += addBoolToString(getBeep(), kBeepStr);
  result += addBoolToString(getClean(), kCleanStr);
  result += addBoolToString(getQuiet(), kQuietStr);
  result += addBoolToString(getPowerful(), kPowerfulStr);
  return result;
}

#if DECODE_SAMSUNG_AC
// Decode the supplied Samsung A/C message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect. Typically kSamsungAcBits
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: Stable / Known to be working.
//
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/505
bool IRrecv::decodeSamsungAC(decode_results *results, const uint16_t nbits,
                             const bool strict) {
  if (results->rawlen < 2 * nbits + kHeader * 3 + kFooter * 2 - 1)
    return false;  // Can't possibly be a valid Samsung A/C message.
  if (nbits != kSamsungAcBits && nbits != kSamsungAcExtendedBits) return false;

  uint16_t offset = kStartOffset;

  // Message Header
  if (!matchMark(results->rawbuf[offset++], kSamsungAcBitMark)) return false;
  if (!matchSpace(results->rawbuf[offset++], kSamsungAcHdrSpace)) return false;
  // Section(s)
  for (uint16_t pos = 0; pos <= (nbits / 8) - kSamsungACSectionLength;
       pos += kSamsungACSectionLength) {
    uint16_t used;
    // Section Header + Section Data (7 bytes) + Section Footer
    used = matchGeneric(results->rawbuf + offset, results->state + pos,
                        results->rawlen - offset, kSamsungACSectionLength * 8,
                        kSamsungAcSectionMark, kSamsungAcSectionSpace,
                        kSamsungAcBitMark, kSamsungAcOneSpace,
                        kSamsungAcBitMark, kSamsungAcZeroSpace,
                        kSamsungAcBitMark, kSamsungAcSectionGap,
                        pos + kSamsungACSectionLength >= nbits / 8,
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
