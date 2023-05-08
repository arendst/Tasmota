// Copyright 2022 Mateusz Bronk (mbronk)
/// @file
/// @brief Support for the Gorenje cooker hood IR protocols.
/// @see https://techfresh.pl/wp-content/uploads/2017/08/Gorenje-DKF-2600-MWT.pdf
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1887

// Supports:
//   Brand: Gorenje,  Model: DKF 2600 MWT Cooker Hood

#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

const uint32_t kGorenjeMinGap = 100000U;  // 0.1s
const uint16_t kGorenjeHdrMark = 0;
const uint32_t kGorenjeHdrSpace = 0;
const uint16_t kGorenjeBitMark = 1300;
const uint32_t kGorenjeOneSpace = 5700;
const uint32_t kGorenjeZeroSpace = 1700;
const uint16_t kGorenjeFreq = 38000;  // Hz
const uint16_t kGorenjeTolerance = 7;  // %

#if SEND_GORENJE
/// Send a Gorenje Cooker Hood formatted message.
/// Status: STABLE / Known working.
/// @param[in] data containing the IR command to be sent.
/// @param[in] nbits Nr. of bits of the message to send. usually kGorenjeBits
/// @param[in] repeat Nr. of times the message is to be repeated.
void IRsend::sendGorenje(const uint64_t data, const uint16_t nbits,
                         const uint16_t repeat) {
  sendGeneric(kGorenjeHdrMark, kGorenjeHdrSpace,
              kGorenjeBitMark, kGorenjeOneSpace,
              kGorenjeBitMark, kGorenjeZeroSpace,
              kGorenjeBitMark, kGorenjeMinGap,
              data, nbits, kGorenjeFreq, true, repeat, kDutyDefault);
}
#endif  // SEND_GORENJE

#if DECODE_GORENJE
/// Decode the supplied Gorenje Cooker Hood message.
/// Status: STABLE / Known working.
/// @param[in,out] results Ptr to the data to decode & where to store the
///   decoded result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeGorenje(decode_results *results, uint16_t offset,
                           const uint16_t nbits, const bool strict) {
  if (strict && nbits != kGorenjeBits)
    return false;  // We expect Gorenje to be a certain sized message.

  uint64_t data = 0;
  if (!matchGeneric(results->rawbuf + offset, &data,
                  results->rawlen - offset, nbits,
                  kGorenjeHdrMark, kGorenjeHdrSpace,
                  kGorenjeBitMark, kGorenjeOneSpace,
                  kGorenjeBitMark, kGorenjeZeroSpace,
                  kGorenjeBitMark, kGorenjeMinGap,
                  true, kGorenjeTolerance)) return false;

  // Matched!
  results->bits = nbits;
  results->value = data;
  results->decode_type = decode_type_t::GORENJE;
  results->command = 0;
  results->address = 0;
  return true;
}
#endif  // DECODE_GORENJE
