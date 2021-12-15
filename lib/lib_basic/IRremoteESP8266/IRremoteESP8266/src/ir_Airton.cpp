// Copyright 2021 David Conran (crankyoldgit)
/// @file
/// @brief Support for Airton protocol
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1670

// Supports:
//   Brand: Airton,  Model: SMVH09B-2A2A3NH ref. 409730 A/C
//   Brand: Airton,  Model: RD1A1 remote

#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

const uint16_t kAirtonHdrMark = 6630;
const uint16_t kAirtonBitMark = 400;
const uint16_t kAirtonHdrSpace = 3350;
const uint16_t kAirtonOneSpace = 1260;
const uint16_t kAirtonZeroSpace = 430;
const uint16_t kAirtonFreq = 38000;  // Hz. (Just a guess)

#if SEND_AIRTON
// Function should be safe up to 64 bits.
/// Send a Airton formatted message.
/// Status: STABLE / Confirmed working.
/// @param[in] data containing the IR command.
/// @param[in] nbits Nr. of bits to send. usually kAirtonBits
/// @param[in] repeat Nr. of times the message is to be repeated.
void IRsend::sendAirton(const uint64_t data, const uint16_t nbits,
                        const uint16_t repeat) {
  sendGeneric(kAirtonHdrMark, kAirtonHdrSpace,
              kAirtonBitMark, kAirtonOneSpace,
              kAirtonBitMark, kAirtonZeroSpace,
              kAirtonBitMark, kDefaultMessageGap,
              data, nbits, kAirtonFreq, false, repeat, kDutyDefault);
}
#endif  // SEND_AIRTON

#if DECODE_AIRTON
/// Decode the supplied Airton message.
/// Status: STABLE / Confirmed working. LSBF ordering confirmed via temperature.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeAirton(decode_results *results, uint16_t offset,
                          const uint16_t nbits, const bool strict) {
  if (results->rawlen < 2 * nbits + kHeader + kFooter - offset)
    return false;  // Too short a message to match.
  if (strict && nbits != kAirtonBits)
    return false;

  // Header + Data + Footer
  if (!matchGeneric(&(results->rawbuf[offset]), &(results->value),
                    results->rawlen - offset, nbits,
                    kAirtonHdrMark, kAirtonHdrSpace,
                    kAirtonBitMark, kAirtonOneSpace,
                    kAirtonBitMark, kAirtonZeroSpace,
                    kAirtonBitMark, kDefaultMessageGap,
                    true, kUseDefTol, kMarkExcess, false)) return false;

  // Success
  results->decode_type = decode_type_t::AIRTON;
  results->bits = nbits;
  results->command = 0;
  results->address = 0;
  return true;
}
#endif  // DECODE_AIRTON
