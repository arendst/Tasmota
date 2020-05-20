// Copyright 2020 David Conran

// Send & decode support for Symphony added by David Conran

// Supports:
//   Brand: Symphony,  Model: Air Cooler 3Di
//   Brand: SamHop,    Model: SM3015 Fan Remote Control
//   Brand: SamHop,    Model: SM5021 Encoder chip
//   Brand: SamHop,    Model: SM5032 Decoder chip
//   Brand: Blyss,     Model: Owen-SW-5 3 Fan
//   Brand: Blyss,     Model: WP-YK8 090218 remote
//   Brand: Westinghouse,  Model: Ceiling fan
//   Brand: Westinghouse,  Model: 78095 Remote
//   Brand: Satellite Electronic,  Model: ID6 Remote
//   Brand: Satellite Electronic,  Model: JY199I Fan driver
//   Brand: Satellite Electronic,  Model: JY199I-L Fan driver

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtimer.h"
#include "IRutils.h"

// Constants
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/1057
const uint16_t kSymphonyZeroMark = 400;
const uint16_t kSymphonyZeroSpace = 1250;
const uint16_t kSymphonyOneMark = kSymphonyZeroSpace;
const uint16_t kSymphonyOneSpace = kSymphonyZeroMark;
const uint32_t kSymphonyFooterGap = 4 * (kSymphonyZeroMark +
                                         kSymphonyZeroSpace);

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
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/1105
//   https://www.alldatasheet.com/datasheet-pdf/pdf/124369/ANALOGICTECH/SM5021B.html
void IRsend::sendSymphony(uint64_t data, uint16_t nbits, uint16_t repeat) {
  sendGeneric(0, 0,
              kSymphonyOneMark, kSymphonyOneSpace,
              kSymphonyZeroMark, kSymphonyZeroSpace,
              0, kSymphonyFooterGap,
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
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/1057
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/1105
//   https://www.alldatasheet.com/datasheet-pdf/pdf/124369/ANALOGICTECH/SM5021B.html
bool IRrecv::decodeSymphony(decode_results *results, uint16_t offset,
                            const uint16_t nbits, const bool strict) {
  uint64_t data = 0;

  if (results->rawlen < 2 * nbits + offset - 1)
    return false;  // Not enough entries to ever be SYMPHONY.
  // Compliance
  if (strict && nbits != kSymphonyBits) return false;

  if (!matchGenericConstBitTime(results->rawbuf + offset, &data,
                                results->rawlen - offset,
                                nbits,
                                0, 0,  // No Header
                                kSymphonyOneMark, kSymphonyZeroMark,
                                0, kSymphonyFooterGap, true,
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
