// Copyright 2009 Ken Shirriff
// Copyright 2017, 2018, 2019 David Conran

// Samsung remote emulation

#include "ir_Samsung.h"
#include <algorithm>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRsend.h"
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
    space(100000 - kSamsungAcSectionGap);
  }
}
#endif  // SEND_SAMSUNG_AC

IRSamsungAc::IRSamsungAc(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) {
  this->stateReset();
}

void IRSamsungAc::stateReset(void) {
  for (uint8_t i = 0; i < kSamsungAcExtendedStateLength; i++)
    remote_state[i] = 0x0;
  remote_state[0] = 0x02;
  remote_state[1] = 0x92;
  remote_state[2] = 0x0F;
  remote_state[6] = 0xF0;
  remote_state[7] = 0x01;
  remote_state[8] = 0x02;
  remote_state[9] = 0xAE;
  remote_state[10] = 0x71;
  remote_state[12] = 0x15;
  remote_state[13] = 0xF0;
  _sendpower = false;
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
  sum -= countBits(state[length - 6] & 0xF, 8);
  sum += countBits(state[length - 5] & 0b11111110, 8);
  sum += countBits(state + length - 4, 3);
  return (28 - sum) & 0xF;
}

bool IRSamsungAc::validChecksum(const uint8_t state[], const uint16_t length) {
  if (length < kSamsungAcStateLength)
    return true;  // No checksum to compare with. Assume okay.
  uint8_t offset = 0;
  if (length >= kSamsungAcExtendedStateLength) offset = 7;
  return ((state[length - 6] >> 4) == IRSamsungAc::calcChecksum(state, length)
          && (state[length - (13 + offset)] >> 4) == IRSamsungAc::calcChecksum(
                 state, length - (7 + offset)));
}

// Update the checksum for the internal state.
void IRSamsungAc::checksum(uint16_t length) {
  if (length < 13) return;
  remote_state[length - 6] &= 0x0F;
  remote_state[length - 6] |= (this->calcChecksum(remote_state, length) << 4);
  remote_state[length - 13] &= 0x0F;
  remote_state[length - 13] |= (this->calcChecksum(remote_state,
                                                   length - 7) << 4);
}

#if SEND_SAMSUNG_AC
// Use for most function/mode/settings changes to the unit.
// i.e. When the device is already running.
void IRSamsungAc::send(const uint16_t repeat, const bool calcchecksum) {
  if (calcchecksum) this->checksum();
  if (_sendpower) {  // Do we need to send a the special power on/off message?
    _sendpower = false;  // It will now been sent.
    if (this->getPower()) {
      this->sendOn();
    } else {
      this->sendOff();
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
}
#endif  // SEND_SAMSUNG_AC

uint8_t *IRSamsungAc::getRaw(void) {
  this->checksum();
  return remote_state;
}

void IRSamsungAc::setRaw(const uint8_t new_code[], const uint16_t length) {
  for (uint8_t i = 0; i < length && i < kSamsungAcExtendedStateLength; i++) {
    remote_state[i] = new_code[i];
  }
  // Shrink the extended state into a normal state.
  if (length > kSamsungAcStateLength) {
    for (uint8_t i = kSamsungAcStateLength; i < length; i++)
      remote_state[i - kSamsungACSectionLength] = remote_state[i];
  }
}

void IRSamsungAc::on(void) {
  remote_state[1] &= ~kSamsungAcPowerMask1;  // Bit needs to be cleared.
  remote_state[6] |= kSamsungAcPowerMask6;  // Bit needs to be set.
  _sendpower = true;  // Flag that we need to send the special power message(s).
}

void IRSamsungAc::off(void) {
  remote_state[1] |= kSamsungAcPowerMask1;  // Bit needs to be set.
  remote_state[6] &= ~kSamsungAcPowerMask6;  // Bit needs to be cleared.
  _sendpower = true;  // Flag that we need to send the special power message(s).
}

void IRSamsungAc::setPower(const bool on) {
  if (on)
    this->on();
  else
    this->off();
}

bool IRSamsungAc::getPower(void) {
  return (remote_state[6] & kSamsungAcPowerMask6) &&
         !(remote_state[1] & kSamsungAcPowerMask1);
}

// Set the temp. in deg C
void IRSamsungAc::setTemp(const uint8_t temp) {
  uint8_t newtemp = std::max(kSamsungAcMinTemp, temp);
  newtemp = std::min(kSamsungAcMaxTemp, newtemp);
  remote_state[11] = (remote_state[11] & ~kSamsungAcTempMask) |
                     ((newtemp - kSamsungAcMinTemp) << 4);
}

// Return the set temp. in deg C
uint8_t IRSamsungAc::getTemp(void) {
  return ((remote_state[11] & kSamsungAcTempMask) >> 4) + kSamsungAcMinTemp;
}

void IRSamsungAc::setMode(const uint8_t mode) {
  // If we get an unexpected mode, default to AUTO.
  uint8_t newmode = mode;
  if (newmode > kSamsungAcHeat) newmode = kSamsungAcAuto;
  remote_state[12] = (remote_state[12] & ~kSamsungAcModeMask) | (newmode << 4);

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
  return (remote_state[12] & kSamsungAcModeMask) >> 4;
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
  remote_state[12] = (remote_state[12] & ~kSamsungAcFanMask) | (speed << 1);
}

uint8_t IRSamsungAc::getFan(void) {
  return ((remote_state[12] & kSamsungAcFanMask) >> 1);
}

bool IRSamsungAc::getSwing(void) {
  // TODO(Hollako): Explain why sometimes the LSB of remote_state[9] is a 1.
  // e.g. 0xAE or 0XAF for swing move.
  return ((remote_state[9] & kSamsungAcSwingMask) >> 4) == kSamsungAcSwingMove;
}

void IRSamsungAc::setSwing(const bool on) {
  // TODO(Hollako): Explain why sometimes the LSB of remote_state[9] is a 1.
  // e.g. 0xAE or 0XAF for swing move.
  remote_state[9] &= ~kSamsungAcSwingMask;  // Clear the previous swing state.
  if (on)
    remote_state[9] |= (kSamsungAcSwingMove << 4);
  else
    remote_state[9] |= (kSamsungAcSwingStop << 4);
}

bool IRSamsungAc::getBeep(void) {
  return remote_state[13] & kSamsungAcBeepMask;
}

void IRSamsungAc::setBeep(const bool on) {
  if (on)
    remote_state[13] |= kSamsungAcBeepMask;
  else
    remote_state[13] &= ~kSamsungAcBeepMask;
}

bool IRSamsungAc::getClean(void) {
  return (remote_state[10] & kSamsungAcCleanMask10) &&
         (remote_state[11] & kSamsungAcCleanMask11);
}

void IRSamsungAc::setClean(const bool on) {
  if (on) {
    remote_state[10] |= kSamsungAcCleanMask10;
    remote_state[11] |= kSamsungAcCleanMask11;
  } else {
    remote_state[10] &= ~kSamsungAcCleanMask10;
    remote_state[11] &= ~kSamsungAcCleanMask11;
  }
}

bool IRSamsungAc::getQuiet(void) {
  return !(remote_state[1] & kSamsungAcQuietMask1) &&
         (remote_state[5] & kSamsungAcQuietMask5);
}

void IRSamsungAc::setQuiet(const bool on) {
  if (on) {
    remote_state[1] &= ~kSamsungAcQuietMask1;  // Bit needs to be cleared.
    remote_state[5] |= kSamsungAcQuietMask5;  // Bit needs to be set.
    // Quiet mode seems to set fan speed to auto.
    this->setFan(kSamsungAcFanAuto);
    this->setPowerful(false);  // Quiet 'on' is mutually exclusive to Powerful.
  } else {
    remote_state[1] |= kSamsungAcQuietMask1;  // Bit needs to be set.
    remote_state[5] &= ~kSamsungAcQuietMask5;  // Bit needs to be cleared.
  }
}

bool IRSamsungAc::getPowerful(void) {
  return !(remote_state[8] & kSamsungAcPowerfulMask8) &&
         (remote_state[10] & kSamsungAcPowerfulMask10) &&
         this->getFan() == kSamsungAcFanTurbo;
}

void IRSamsungAc::setPowerful(const bool on) {
  if (on) {
    remote_state[8] &= ~kSamsungAcPowerfulMask8;  // Bit needs to be cleared.
    remote_state[10] |= kSamsungAcPowerfulMask10;  // Bit needs to be set.
    // Powerful mode sets fan speed to Turbo.
    this->setFan(kSamsungAcFanTurbo);
    this->setQuiet(false);  // Powerful 'on' is mutually exclusive to Quiet.
  } else {
    remote_state[8] |= kSamsungAcPowerfulMask8;  // Bit needs to be set.
    remote_state[10] &= ~kSamsungAcPowerfulMask10;  // Bit needs to be cleared.
    // Turning off Powerful mode sets fan speed to Auto if we were in Turbo mode
    if (this->getFan() == kSamsungAcFanTurbo) this->setFan(kSamsungAcFanAuto);
  }
}

// Convert a standard A/C mode into its native mode.
uint8_t IRSamsungAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool:
      return kSamsungAcCool;
    case stdAc::opmode_t::kHeat:
      return kSamsungAcHeat;
    case stdAc::opmode_t::kDry:
      return kSamsungAcDry;
    case stdAc::opmode_t::kFan:
      return kSamsungAcFan;
    default:
      return kSamsungAcAuto;
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRSamsungAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:
      return kSamsungAcFanLow;
    case stdAc::fanspeed_t::kMedium:
      return kSamsungAcFanMed;
    case stdAc::fanspeed_t::kHigh:
      return kSamsungAcFanHigh;
    case stdAc::fanspeed_t::kMax:
      return kSamsungAcFanTurbo;
    default:
      return kSamsungAcFanAuto;
  }
}

// Convert a native mode to it's common equivalent.
stdAc::opmode_t IRSamsungAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kSamsungAcCool: return stdAc::opmode_t::kCool;
    case kSamsungAcHeat: return stdAc::opmode_t::kHeat;
    case kSamsungAcDry: return stdAc::opmode_t::kDry;
    case kSamsungAcFan: return stdAc::opmode_t::kFan;
    default: return stdAc::opmode_t::kAuto;
  }
}

// Convert a native fan speed to it's common equivalent.
stdAc::fanspeed_t IRSamsungAc::toCommonFanSpeed(const uint8_t spd) {
  switch (spd) {
    case kSamsungAcFanTurbo: return stdAc::fanspeed_t::kMax;
    case kSamsungAcFanHigh: return stdAc::fanspeed_t::kHigh;
    case kSamsungAcFanMed: return stdAc::fanspeed_t::kMedium;
    case kSamsungAcFanLow: return stdAc::fanspeed_t::kMin;
    default: return stdAc::fanspeed_t::kAuto;
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
  result += addBoolToString(getPower(), F("Power"), false);
  result += addModeToString(getMode(), kSamsungAcAuto, kSamsungAcCool,
                            kSamsungAcHeat, kSamsungAcDry,
                            kSamsungAcFan);
  result += addTempToString(getTemp());
  result += addIntToString(getFan(), F("Fan"));
  switch (getFan()) {
    case kSamsungAcFanAuto:
    case kSamsungAcFanAuto2:
      result += F(" (Auto)");
      break;
    case kSamsungAcFanLow:
      result += F(" (Low)");
      break;
    case kSamsungAcFanMed:
      result += F(" (Medium)");
      break;
    case kSamsungAcFanHigh:
      result += F(" (High)");
      break;
    case kSamsungAcFanTurbo:
      result += F(" (Turbo)");
      break;
    default:
      result += F(" (UNKNOWN)");
      break;
  }
  result += addBoolToString(getSwing(), F("Swing"));
  result += addBoolToString(getBeep(), F("Beep"));
  result += addBoolToString(getClean(), F("Clean"));
  result += addBoolToString(getQuiet(), F("Quiet"));
  result += addBoolToString(getPowerful(), F("Powerful"));
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
