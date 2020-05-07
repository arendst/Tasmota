// Copyright 2020 David Conran

// Send & decode support for Symphony added by David Conran

// Supports:
//   Brand: Symphony,  Model: Air Cooler 3Di

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtimer.h"
#include "IRutils.h"

// Constants
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/1057
const uint16_t kSymphonyZeroMark = 1250;
const uint16_t kSymphonyZeroSpace = 400;
const uint16_t kSymphonyOneMark = kSymphonyZeroSpace;
const uint16_t kSymphonyOneSpace = kSymphonyZeroMark;
const uint16_t kSymphonyFooterMark = kSymphonyOneMark;
const uint32_t kSymphonyFooterGap = 8000;

#if SEND_SYMPHONY
// Send a Symphony packet.
//
// Args:
//   data: The data we want to send. MSB first.
//   nbits: The number of bits of data to send. (Typically 12, 24, or 32[Nokia])
//   repeat: The nr. of times the message should be sent.
//
// Status:  STABLE / Should be working.
//
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/1057
void IRsend::sendSymphony(uint64_t data, uint16_t nbits, uint16_t repeat) {
  sendGeneric(0, 0,
              kSymphonyOneMark, kSymphonyOneSpace,
              kSymphonyZeroMark, kSymphonyZeroSpace,
              kSymphonyFooterMark, kSymphonyFooterGap,
              data, nbits, 38000, true, repeat, kDutyDefault);
}
#endif  // SEND_SYMPHONY

#if DECODE_SYMPHONY
// Decode a Symphony packet if possible.
// Places successful decode information in the results pointer.
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   offset:  The starting index to use when attempting to decode the raw data.
//            Typically/Defaults to kStartOffset.
//   nbits:   Nr. of bits to expect in the data portion. Typically kSymphonyBits
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status:  STABLE / Should be working.
//
// Ref:
//
bool IRrecv::decodeSymphony(decode_results *results, uint16_t offset,
                            const uint16_t nbits, const bool strict) {
  uint64_t data = 0;

  if (results->rawlen < 2 * nbits + kFooter + offset - 1)
    return false;  // Not enough entries to ever be SYMPHONY.
  // Compliance
  if (strict && nbits != kSymphonyBits) return false;

  if (!matchGeneric(results->rawbuf + offset, &data, results->rawlen - offset,
                    nbits,
                    0, 0,  // No Header
                    kSymphonyOneMark, kSymphonyOneSpace,
                    kSymphonyZeroMark, kSymphonyZeroSpace,
                    kSymphonyFooterMark, kSymphonyFooterGap, true,
                    _tolerance, 0))
    return false;

  // Success
  results->value = data;
  results->decode_type = decode_type_t::SYMPHONY;
  results->bits = nbits;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif  // DECODE_SYMPHONY
