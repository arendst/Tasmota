// Copyright 2020 David Conran

/// @file
/// @brief Support for Symphony protocols.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1057
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1105
/// @see https://www.alldatasheet.com/datasheet-pdf/pdf/124369/ANALOGICTECH/SM5021B.html

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
const uint16_t kSymphonyZeroMark = 400;
const uint16_t kSymphonyZeroSpace = 1250;
const uint16_t kSymphonyOneMark = kSymphonyZeroSpace;
const uint16_t kSymphonyOneSpace = kSymphonyZeroMark;
const uint32_t kSymphonyFooterGap = 4 * (kSymphonyZeroMark +
                                         kSymphonyZeroSpace);

#if SEND_SYMPHONY
/// Send a Symphony packet.
/// Status:  STABLE / Should be working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendSymphony(uint64_t data, uint16_t nbits, uint16_t repeat) {
  sendGeneric(0, 0,
              kSymphonyOneMark, kSymphonyOneSpace,
              kSymphonyZeroMark, kSymphonyZeroSpace,
              0, kSymphonyFooterGap,
              data, nbits, 38000, true, repeat, kDutyDefault);
}
#endif  // SEND_SYMPHONY

#if DECODE_SYMPHONY
/// Decode the supplied Symphony packet/message.
/// Status: STABLE / Should be working.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
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
