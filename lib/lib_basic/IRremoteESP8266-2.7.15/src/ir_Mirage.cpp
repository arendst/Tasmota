// Copyright 2020 David Conran (crankyoldgit)
/// @file
/// @brief Support for Mirage protocol
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1289

// Supports:
//   Brand: Mirage,  Model: VLU series A/C

#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"


// Constants
const uint16_t kMirageHdrMark = 8360;            ///< uSeconds
const uint16_t kMirageBitMark = 554;             ///< uSeconds
const uint16_t kMirageHdrSpace = 4248;           ///< uSeconds
const uint16_t kMirageOneSpace = 1592;           ///< uSeconds
const uint16_t kMirageZeroSpace = 545;           ///< uSeconds
const uint32_t kMirageGap = kDefaultMessageGap;  ///< uSeconds (just a guess)
const uint16_t kMirageFreq = 38000;              ///< Hz. (Just a guess)


#if SEND_MIRAGE
/// Send a Mirage formatted message.
/// Status: STABLE / Reported as working.
/// @param[in] data An array of bytes containing the IR command.
/// @param[in] nbytes Nr. of bytes of data in the array. (>=kMirageStateLength)
/// @param[in] repeat Nr. of times the message is to be repeated.
void IRsend::sendMirage(const uint8_t data[], const uint16_t nbytes,
                        const uint16_t repeat) {
  sendGeneric(kMirageHdrMark, kMirageHdrSpace,
              kMirageBitMark, kMirageOneSpace,
              kMirageBitMark, kMirageZeroSpace,
              kMirageBitMark, kMirageGap,
              data, nbytes, kMirageFreq, false,  // LSB
              repeat, kDutyDefault);
}
#endif  // SEND_MIRAGE

#if DECODE_MIRAGE
/// Decode the supplied Mirage message.
/// Status: STABLE / Reported as working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeMirage(decode_results *results, uint16_t offset,
                          const uint16_t nbits, const bool strict) {
  if (strict && nbits != kMirageBits) return false;  // Compliance.

  if (!matchGeneric(results->rawbuf + offset, results->state,
                    results->rawlen - offset, nbits,
                    kMirageHdrMark, kMirageHdrSpace,
                    kMirageBitMark, kMirageOneSpace,
                    kMirageBitMark, kMirageZeroSpace,
                    kMirageBitMark, kMirageGap, true,
                    kUseDefTol, kMarkExcess, false)) return false;

  // Success
  results->decode_type = decode_type_t::MIRAGE;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_MIRAGE
