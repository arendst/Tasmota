// Copyright 2009 Ken Shirriff
// Copyright 2017 David Conran

/// @file
/// @brief Nikai
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/309

// Supports:
//   Brand: Nikai,  Model: Unknown LCD TV

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

// Constants
const uint16_t kNikaiTick = 500;
const uint16_t kNikaiHdrMarkTicks = 8;
const uint16_t kNikaiHdrMark = kNikaiHdrMarkTicks * kNikaiTick;
const uint16_t kNikaiHdrSpaceTicks = 8;
const uint16_t kNikaiHdrSpace = kNikaiHdrSpaceTicks * kNikaiTick;
const uint16_t kNikaiBitMarkTicks = 1;
const uint16_t kNikaiBitMark = kNikaiBitMarkTicks * kNikaiTick;
const uint16_t kNikaiOneSpaceTicks = 2;
const uint16_t kNikaiOneSpace = kNikaiOneSpaceTicks * kNikaiTick;
const uint16_t kNikaiZeroSpaceTicks = 4;
const uint16_t kNikaiZeroSpace = kNikaiZeroSpaceTicks * kNikaiTick;
const uint16_t kNikaiMinGapTicks = 17;
const uint16_t kNikaiMinGap = kNikaiMinGapTicks * kNikaiTick;

#if SEND_NIKAI
/// Send a Nikai formatted message.
/// Status: STABLE / Working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendNikai(uint64_t data, uint16_t nbits, uint16_t repeat) {
  sendGeneric(kNikaiHdrMark, kNikaiHdrSpace, kNikaiBitMark, kNikaiOneSpace,
              kNikaiBitMark, kNikaiZeroSpace, kNikaiBitMark, kNikaiMinGap, data,
              nbits, 38, true, repeat, 33);
}
#endif  // SEND_NIKAI

#if DECODE_NIKAI
/// Decode the supplied Nikai message.
/// Status: STABLE / Working.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
bool IRrecv::decodeNikai(decode_results *results, uint16_t offset,
                         const uint16_t nbits, const bool strict) {
  if (strict && nbits != kNikaiBits)
    return false;  // We expect Nikai to be a certain sized message.

  uint64_t data = 0;

  // Match Header + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, &data,
                    results->rawlen - offset, nbits,
                    kNikaiHdrMark, kNikaiHdrSpace,
                    kNikaiBitMark, kNikaiOneSpace,
                    kNikaiBitMark, kNikaiZeroSpace,
                    kNikaiBitMark, kNikaiMinGap, true)) return false;
  // Success
  results->bits = nbits;
  results->value = data;
  results->decode_type = NIKAI;
  results->command = 0;
  results->address = 0;
  return true;
}
#endif  // DECODE_NIKAI
