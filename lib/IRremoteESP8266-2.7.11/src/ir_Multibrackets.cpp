// Copyright 2020 David Conran

/// @file
/// @brief Support for Multibrackets protocols.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1103
/// @see http://info.multibrackets.com/data/common/manuals/4500_code.pdf

// Supports:
//   Brand: Multibrackets,  Model: Motorized Swing mount large - 4500

#include "IRrecv.h"
#include "IRsend.h"

const uint16_t kMultibracketsTick = 5000;  // uSeconds
const uint16_t kMultibracketsHdrMark = 3 * kMultibracketsTick;  // uSeconds
const uint16_t kMultibracketsFooterSpace = 6 * kMultibracketsTick;  // uSeconds
const uint8_t kMultibracketsTolerance = 5;  // Percent
const uint16_t kMultibracketsFreq = 38000;  // Hertz

#if SEND_MULTIBRACKETS
/// Send a Multibrackets formatted message.
/// Status: BETA / Appears to be working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendMultibrackets(uint64_t data, uint16_t nbits, uint16_t repeat) {
  enableIROut(kMultibracketsFreq);
  for (uint16_t r = 0; r <= repeat; r++) {
    uint16_t bits = nbits;
    // Header
    mark(kMultibracketsHdrMark);
    // Data
    // Send 0's until we get down to a bit size we can actually manage.
    while (bits > sizeof(data) * 8) {
      space(kMultibracketsTick);
      bits--;
    }
    // Send the supplied data.
    for (uint64_t mask = 1ULL << (bits - 1); mask; mask >>= 1)
      if (data & mask)  // Send a 1
        mark(kMultibracketsTick);
      else  // Send a 0
        space(kMultibracketsTick);
    // Footer
    space(kMultibracketsFooterSpace);
  }
}
#endif  // SEND_MULTIBRACKETS

#if DECODE_MULTIBRACKETS
/// Decode the Multibrackets message.
/// Status: BETA / Appears to be working.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
bool IRrecv::decodeMultibrackets(decode_results *results, uint16_t offset,
                                 const uint16_t nbits, const bool strict) {
  // Compliance
  if (strict && nbits != kMultibracketsBits)
    return false;  // Doesn't match our protocol defn.

  // Check there is enough unprocessed buffer left.
  if (results->rawlen < offset) return false;

  // Header
  int32_t remaining = *(results->rawbuf + offset);
  if (!matchAtLeast(remaining, kMultibracketsHdrMark, kMultibracketsTolerance))
    return false;
  remaining -= (kMultibracketsHdrMark / kRawTick);  // Remove the header.

  // We are done with the header. Onto the data.
  bool bit = true;
  uint16_t bitsSoFar = 0;
  uint64_t data = 0;
  // Keep going till we run out of message or expected bits.
  while (offset <= results->rawlen && bitsSoFar < nbits) {
    // Have we finished processing this rawbuf value yet?
    if (remaining <= 0) {  // No more possible "bits" left in this value.
      // Invert the bit for next time, and move along the rawbuf.
      bit = !bit;
      offset++;
      // Load the next data point if there is one.
      if (offset <= results->rawlen) remaining = *(results->rawbuf + offset);
    } else {  // Look for more bits in this entry.
      if (matchAtLeast(remaining, kMultibracketsTick,
          kMultibracketsTolerance)) {  // There is!
        data <<= 1;
        data += bit;
        bitsSoFar++;
      }
      remaining -= (kMultibracketsTick / kRawTick);  // Remove the "bit".
    }
  }

  // Compliance
  if (bitsSoFar != nbits) return false;

  // Footer
  if (results->rawlen <= offset && !matchAtLeast(*(results->rawbuf + offset),
                                                 kMultibracketsFooterSpace,
                                                 kMultibracketsTolerance))
    return false;

  // Success
  results->decode_type = decode_type_t::MULTIBRACKETS;
  results->value = data;
  results->bits = nbits;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif  // DECODE_MULTIBRACKETS
