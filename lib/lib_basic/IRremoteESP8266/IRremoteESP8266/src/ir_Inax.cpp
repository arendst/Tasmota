// Copyright 2019 David Conran (crankyoldgit)
/// @file
/// @brief Support for the Inax Robot Toilet IR protocols.
/// @see https://www.lixil-manual.com/GCW-1365-16050/GCW-1365-16050.pdf
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/706

// Supports:
//   Brand: Lixil,  Model: Inax DT-BA283 Toilet

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

// Constants
const uint16_t kInaxTick = 500;
const uint16_t kInaxHdrMark = 9000;
const uint16_t kInaxHdrSpace = 4500;
const uint16_t kInaxBitMark = 560;
const uint16_t kInaxOneSpace = 1675;
const uint16_t kInaxZeroSpace = kInaxBitMark;
const uint16_t kInaxMinGap = 40000;

#if SEND_INAX
/// Send a Inax Toilet formatted message.
/// Status: STABLE / Working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/706
void IRsend::sendInax(const uint64_t data, const uint16_t nbits,
                      const uint16_t repeat) {
  sendGeneric(kInaxHdrMark, kInaxHdrSpace,
              kInaxBitMark, kInaxOneSpace,
              kInaxBitMark, kInaxZeroSpace,
              kInaxBitMark, kInaxMinGap,
              data, nbits, 38, true, repeat, kDutyDefault);
}
#endif  // SEND_INAX

#if DECODE_INAX
/// Decode the supplied Inax Toilet message.
/// Status: Stable / Known working.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/706
bool IRrecv::decodeInax(decode_results *results, uint16_t offset,
                        const uint16_t nbits, const bool strict) {
  if (strict && nbits != kInaxBits)
    return false;  // We expect Inax to be a certain sized message.

  uint64_t data = 0;

  // Match Header + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, &data,
                    results->rawlen - offset, nbits,
                    kInaxHdrMark, kInaxHdrSpace,
                    kInaxBitMark, kInaxOneSpace,
                    kInaxBitMark, kInaxZeroSpace,
                    kInaxBitMark, kInaxMinGap, true)) return false;
  // Success
  results->bits = nbits;
  results->value = data;
  results->decode_type = decode_type_t::INAX;
  results->command = 0;
  results->address = 0;
  return true;
}
#endif  // DECODE_INAX
