// Copyright 2017 David Conran

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

//   LL        AAA    SSSSS  EEEEEEE RRRRRR  TTTTTTT   AAA     GGGG
//   LL       AAAAA  SS      EE      RR   RR   TTT    AAAAA   GG  GG
//   LL      AA   AA  SSSSS  EEEEE   RRRRRR    TTT   AA   AA GG
//   LL      AAAAAAA      SS EE      RR  RR    TTT   AAAAAAA GG   GG
//   LLLLLLL AA   AA  SSSSS  EEEEEEE RR   RR   TTT   AA   AA  GGGGGG

// Constants
const uint16_t kLasertagMinSamples = 13;
const uint16_t kLasertagTick = 333;
const uint32_t kLasertagMinGap = 100000;  // Completely made up amount.
const uint8_t kLasertagTolerance = 0;     // Percentage error margin.
const uint16_t kLasertagExcess = 0;       // See kMarkExcess.
const uint16_t kLasertagDelta = 150;  // Use instead of Excess and Tolerance.
const int16_t kSpace = 1;
const int16_t kMark = 0;

#if SEND_LASERTAG
// Send a Lasertag packet.
// This protocol is pretty much just raw Manchester encoding.
//
// Args:
//   data:    The message you wish to send.
//   nbits:   Bit size of the protocol you want to send.
//   repeat:  Nr. of extra times the data will be sent.
//
// Status: STABLE / Working.
//
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
// Decode the supplied Lasertag message.
// This protocol is pretty much just raw Manchester encoding.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: BETA / Appears to be working 90% of the time.
//
// Ref:
//   http://www.sbprojects.com/knowledge/ir/rc5.php
//   https://en.wikipedia.org/wiki/RC-5
//   https://en.wikipedia.org/wiki/Manchester_code
bool IRrecv::decodeLasertag(decode_results *results, uint16_t nbits,
                            bool strict) {
  if (results->rawlen < kLasertagMinSamples) return false;

  // Compliance
  if (strict && nbits != kLasertagBits) return false;

  uint16_t offset = kStartOffset;
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
