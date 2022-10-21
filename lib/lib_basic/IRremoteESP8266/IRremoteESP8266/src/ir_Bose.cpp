// Copyright 2021 parsnip42
// Copyright 2021 David Conran

/// @file
/// @brief Support for Bose protocols.
/// @note Currently only tested against Bose TV Speaker.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/pull/1579

// Supports:
//   Brand: Bose,  Model: Bose TV Speaker

#include "IRrecv.h"
#include "IRsend.h"

const uint16_t kBoseHdrMark = 1100;
const uint16_t kBoseHdrSpace = 1350;
const uint16_t kBoseBitMark = 555;
const uint16_t kBoseOneSpace = 1435;
const uint16_t kBoseZeroSpace = 500;
const uint32_t kBoseGap = kDefaultMessageGap;
const uint16_t kBoseFreq = 38;

#if SEND_BOSE
/// Send a Bose formatted message.
/// Status: STABLE / Known working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendBose(const uint64_t data, const uint16_t nbits,
                      const uint16_t repeat) {
  sendGeneric(kBoseHdrMark, kBoseHdrSpace,
              kBoseBitMark, kBoseOneSpace,
              kBoseBitMark, kBoseZeroSpace,
              kBoseBitMark, kBoseGap,
              data, nbits, kBoseFreq, false,
              repeat, kDutyDefault);
}
#endif  // SEND_BOSE

#if DECODE_BOSE
/// Decode the supplied Bose formatted message.
/// Status: STABLE / Known working.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
bool IRrecv::decodeBose(decode_results *results, uint16_t offset,
                        const uint16_t nbits, const bool strict) {
  if (strict && nbits != kBoseBits) return false;

  if (!matchGeneric(results->rawbuf + offset, &(results->value),
                    results->rawlen - offset, nbits,
                    kBoseHdrMark, kBoseHdrSpace,
                    kBoseBitMark, kBoseOneSpace,
                    kBoseBitMark, kBoseZeroSpace,
                    kBoseBitMark, kBoseGap, true,
                    kUseDefTol, 0, false)) {
    return false;
  }

  //
  results->decode_type = decode_type_t::BOSE;
  results->bits = nbits;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif  // DECODE_BOSE
