// Copyright 2017 David Conran

/// @file
/// @brief Support for Lasertag protocols.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/366

// Supports:
//   Brand: Lasertag,  Model: Phaser emitters

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

// Constants
const uint16_t kLasertagMinSamples = 13;
const uint16_t kLasertagTick = 333;
const uint32_t kLasertagMinGap = kDefaultMessageGap;  // Just a guess.
const uint8_t kLasertagTolerance = 0;     // Percentage error margin.
const uint16_t kLasertagExcess = 0;       // See kMarkExcess.
const uint16_t kLasertagDelta = 150;  // Use instead of Excess and Tolerance.
const int16_t kSpace = 1;
const int16_t kMark = 0;

#if SEND_LASERTAG
/// Send a Lasertag packet/message.
/// Status: STABLE / Working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @note This protocol is pretty much just raw Manchester encoding.
/// @todo Convert this to use `sendManchester()` if we can.`
void IRsend::sendLasertag(uint64_t data, uint16_t nbits, uint16_t repeat) {
  if (nbits > sizeof(data) * 8) return;  // We can't send something that big.

  // Set 36kHz IR carrier frequency & a 1/4 (25%) duty cycle.
  // NOTE: duty cycle is not confirmed. Just guessing based on RC5/6 protocols.
  enableIROut(36, 25);

  for (uint16_t i = 0; i <= repeat; i++) {
    // Data
    for (uint64_t mask = 1ULL << (nbits - 1); mask; mask >>= 1)
      if (data & mask) {       // 1
        space(kLasertagTick);  // 1 is space, then mark.
        mark(kLasertagTick);
      } else {                // 0
        mark(kLasertagTick);  // 0 is mark, then space.
        space(kLasertagTick);
      }
    // Footer
    space(kLasertagMinGap);
  }
}
#endif  // SEND_LASERTAG

#if DECODE_LASERTAG
/// Decode the supplied Lasertag message.
/// Status: BETA / Appears to be working 90% of the time.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
/// @note This protocol is pretty much just raw Manchester encoding.
/// @see http://www.sbprojects.com/knowledge/ir/rc5.php
/// @see https://en.wikipedia.org/wiki/RC-5
/// @see https://en.wikipedia.org/wiki/Manchester_code
/// @todo Convert to using `matchManchester()` if we can.
bool IRrecv::decodeLasertag(decode_results *results, uint16_t offset,
                            const uint16_t nbits, const bool strict) {
  if (results->rawlen <= kLasertagMinSamples + offset) return false;

  // Compliance
  if (strict && nbits != kLasertagBits) return false;

  uint16_t used = 0;
  uint64_t data = 0;
  uint16_t actual_bits = 0;

  // No Header

  // Data
  for (; offset <= results->rawlen; actual_bits++) {
    int16_t levelA =
        getRClevel(results, &offset, &used, kLasertagTick, kLasertagTolerance,
                   kLasertagExcess, kLasertagDelta);
    int16_t levelB =
        getRClevel(results, &offset, &used, kLasertagTick, kLasertagTolerance,
                   kLasertagExcess, kLasertagDelta);
    if (levelA == kSpace && levelB == kMark) {
      data = (data << 1) | 1;  // 1
    } else {
      if (levelA == kMark && levelB == kSpace) {
        data <<= 1;  // 0
      } else {
        break;
      }
    }
  }
  // Footer (None)

  // Compliance
  if (actual_bits < nbits) return false;  // Less data than we expected.
  if (strict && actual_bits != kLasertagBits) return false;

  // Success
  results->decode_type = LASERTAG;
  results->value = data;
  results->address = data & 0xF;  // Unit
  results->command = data >> 4;   // Team
  results->repeat = false;
  results->bits = actual_bits;
  return true;
}
#endif  // DECODE_LASERTAG
