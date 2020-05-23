// Copyright 2018, 2020 David Conran

// Supports:
//   Brand: Carrier/Surrey,  Model: 42QG5A55970 remote
//   Brand: Carrier/Surrey,  Model: 619EGX0090E0 A/C
//   Brand: Carrier/Surrey,  Model: 619EGX0120E0 A/C
//   Brand: Carrier/Surrey,  Model: 619EGX0180E0 A/C
//   Brand: Carrier/Surrey,  Model: 619EGX0220E0 A/C
//   Brand: Carrier/Surrey,  Model: 53NGK009/012 Inverter

#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

// Constants
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/385
const uint16_t kCarrierAcHdrMark = 8532;
const uint16_t kCarrierAcHdrSpace = 4228;
const uint16_t kCarrierAcBitMark = 628;
const uint16_t kCarrierAcOneSpace = 1320;
const uint16_t kCarrierAcZeroSpace = 532;
const uint16_t kCarrierAcGap = 20000;
const uint16_t kCarrierAcFreq = 38;  // kHz. (An educated guess)

const uint16_t kCarrierAc40HdrMark = 8402;
const uint16_t kCarrierAc40HdrSpace = 4166;
const uint16_t kCarrierAc40BitMark = 547;
const uint16_t kCarrierAc40OneSpace = 1540;
const uint16_t kCarrierAc40ZeroSpace = 497;

// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1127
const uint16_t kCarrierAc64HdrMark = 8940;
const uint16_t kCarrierAc64HdrSpace = 4556;
const uint16_t kCarrierAc64BitMark = 503;
const uint16_t kCarrierAc64OneSpace = 1736;
const uint16_t kCarrierAc64ZeroSpace = 615;
const uint32_t kCarrierAc64Gap = kDefaultMessageGap;  // A guess.


#if SEND_CARRIER_AC
// Send a Carrier HVAC formatted message.
//
// Args:
//   data:   The message to be sent.
//   nbits:  The bit size of the message being sent. typically kCarrierAcBits.
//   repeat: The number of times the message is to be repeated.
//
// Status: STABLE / Work on real devices.
//
void IRsend::sendCarrierAC(uint64_t data, uint16_t nbits, uint16_t repeat) {
  for (uint16_t r = 0; r <= repeat; r++) {
    uint64_t temp_data = data;
    // Carrier sends the data block three times. normal + inverted + normal.
    for (uint16_t i = 0; i < 3; i++) {
      sendGeneric(kCarrierAcHdrMark, kCarrierAcHdrSpace, kCarrierAcBitMark,
                  kCarrierAcOneSpace, kCarrierAcBitMark, kCarrierAcZeroSpace,
                  kCarrierAcBitMark, kCarrierAcGap, temp_data, nbits, 38, true,
                  0, kDutyDefault);
      temp_data = invertBits(temp_data, nbits);
    }
  }
}
#endif

#if DECODE_CARRIER_AC
// Decode the supplied Carrier HVAC message.
// Carrier HVAC messages contain only 32 bits, but it is sent three(3) times.
// i.e. normal + inverted + normal
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   offset:  The starting index to use when attempting to decode the raw data.
//            Typically/Defaults to kStartOffset.
//   nbits:   Nr. of bits to expect in the data portion.
//            Typically kCarrierAcBits.
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: BETA / Probably works.
//
bool IRrecv::decodeCarrierAC(decode_results *results, uint16_t offset,
                             const uint16_t nbits, const bool strict) {
  if (results->rawlen < ((2 * nbits + kHeader + kFooter) * 3) - 1 + offset)
    return false;  // Can't possibly be a valid Carrier message.
  if (strict && nbits != kCarrierAcBits)
    return false;  // We expect Carrier to be 32 bits of message.

  uint64_t data = 0;
  uint64_t prev_data = 0;

  for (uint8_t i = 0; i < 3; i++) {
    prev_data = data;
    // Match Header + Data + Footer
    uint16_t used;
    used = matchGeneric(results->rawbuf + offset, &data,
                        results->rawlen - offset, nbits,
                        kCarrierAcHdrMark, kCarrierAcHdrSpace,
                        kCarrierAcBitMark, kCarrierAcOneSpace,
                        kCarrierAcBitMark, kCarrierAcZeroSpace,
                        kCarrierAcBitMark, kCarrierAcGap, true);
    if (!used) return false;
    offset += used;
    // Compliance.
    if (strict) {
      // Check if the data is an inverted copy of the previous data.
      if (i > 0 && prev_data != invertBits(data, nbits)) return false;
    }
  }

  // Success
  results->bits = nbits;
  results->value = data;
  results->decode_type = CARRIER_AC;
  results->address = data >> 16;
  results->command = data & 0xFFFF;
  return true;
}
#endif  // DECODE_CARRIER_AC

#if SEND_CARRIER_AC40
/// Send a Carrier 40bit HVAC formatted message.
/// Status: Alpha / Yet to be tested against a real device.
/// @param[in] data The message to be sent.
/// @param[in] nbits The bit size of the message being sent.
/// @param[in] repeat The number of times the message is to be repeated.
void IRsend::sendCarrierAC40(const uint64_t data, const uint16_t nbits,
                             const uint16_t repeat) {
  sendGeneric(kCarrierAc40HdrMark, kCarrierAc40HdrSpace, kCarrierAc40BitMark,
              kCarrierAc40OneSpace, kCarrierAc40BitMark, kCarrierAc40ZeroSpace,
              kCarrierAc40BitMark, kCarrierAcGap,
              data, nbits, kCarrierAcFreq, true, repeat, kDutyDefault);
}
#endif  // SEND_CARRIER_AC40

#if DECODE_CARRIER_AC40
/// Decode the supplied Carrier 40-bit HVAC message.
/// Carrier HVAC messages contain only 40 bits, but it is sent three(3) times.
/// Status: BETA / Probably works.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeCarrierAC40(decode_results *results, uint16_t offset,
                               const uint16_t nbits, const bool strict) {
  if (results->rawlen < 2 * nbits + kHeader + kFooter - 1 + offset)
    return false;  // Can't possibly be a valid Carrier message.
  if (strict && nbits != kCarrierAc40Bits)
    return false;  // We expect Carrier to be 40 bits of message.

  if (!matchGeneric(results->rawbuf + offset, &(results->value),
                    results->rawlen - offset, nbits,
                    kCarrierAc40HdrMark, kCarrierAc40HdrSpace,
                    kCarrierAc40BitMark, kCarrierAc40OneSpace,
                    kCarrierAc40BitMark, kCarrierAc40ZeroSpace,
                    kCarrierAc40BitMark, kCarrierAcGap, true)) return false;

  // Success
  results->bits = nbits;
  results->decode_type = CARRIER_AC40;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif  // DECODE_CARRIER_AC40

#if SEND_CARRIER_AC64
/// Send a Carrier 64bit HVAC formatted message.
/// Status: Alpha / Yet to be tested against a real device.
/// @param[in] data The message to be sent.
/// @param[in] nbits The bit size of the message being sent.
/// @param[in] repeat The number of times the message is to be repeated.
void IRsend::sendCarrierAC64(const uint64_t data, const uint16_t nbits,
                             const uint16_t repeat) {
  sendGeneric(kCarrierAc64HdrMark, kCarrierAc64HdrSpace, kCarrierAc64BitMark,
              kCarrierAc64OneSpace, kCarrierAc64BitMark, kCarrierAc64ZeroSpace,
              kCarrierAc64BitMark, kCarrierAc64Gap,
              data, nbits, kCarrierAcFreq, false, repeat, kDutyDefault);
}
#endif  // SEND_CARRIER_AC64

#if DECODE_CARRIER_AC64
/// Decode the supplied Carrier 64-bit HVAC message.
/// Status: BETA / Probably works.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeCarrierAC64(decode_results *results, uint16_t offset,
                               const uint16_t nbits, const bool strict) {
  if (results->rawlen < 2 * nbits + kHeader + kFooter - 1 + offset)
    return false;  // Can't possibly be a valid Carrier message.
  if (strict && nbits != kCarrierAc64Bits)
    return false;  // We expect Carrier to be 64 bits of message.

  if (!matchGeneric(results->rawbuf + offset, &(results->value),
                    results->rawlen - offset, nbits,
                    kCarrierAc64HdrMark, kCarrierAc64HdrSpace,
                    kCarrierAc64BitMark, kCarrierAc64OneSpace,
                    kCarrierAc64BitMark, kCarrierAc64ZeroSpace,
                    kCarrierAc64BitMark, kCarrierAc64Gap, true,
                    kUseDefTol, kMarkExcess, false)) return false;

  // Success
  results->bits = nbits;
  results->decode_type = CARRIER_AC64;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif  // DECODE_CARRIER_AC64
