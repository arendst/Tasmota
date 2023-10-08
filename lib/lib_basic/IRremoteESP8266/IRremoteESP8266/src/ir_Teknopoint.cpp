// Copyright 2021 David Conran (crankyoldgit)
/// @file
/// @brief Support for the Teknopoint protocol
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1486

// Supports:
//   Brand: Teknopoint,  Model: Allegro SSA-09H A/C
//   Brand: Teknopoint,  Model: GZ-055B-E1 remote
//   Brand: Teknopoint,  Model: GZ01-BEJ0-000 remote

#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

// Protocol timings
const uint16_t kTeknopointHdrMark = 3600;
const uint16_t kTeknopointBitMark = 477;
const uint16_t kTeknopointHdrSpace = 1600;
const uint16_t kTeknopointOneSpace = 1200;
const uint16_t kTeknopointZeroSpace = 530;
const uint16_t kTeknopointFreq = 38000;  // Hz. (Guess Only)
const uint8_t  kTeknopointExtraTol = 10;  // Extra tolerance percentage.

#if SEND_TEKNOPOINT
/// Send a Teknopoint formatted message.
/// Status: BETA / Probably works.
/// @param[in] data An array of bytes containing the IR command.
/// @param[in] nbytes Nr. of bytes of data in the array.
/// @param[in] repeat Nr. of times the message is to be repeated.
void IRsend::sendTeknopoint(const uint8_t data[], const uint16_t nbytes,
                            const uint16_t repeat) {
  sendGeneric(kTeknopointHdrMark, kTeknopointHdrSpace,
              kTeknopointBitMark, kTeknopointOneSpace,
              kTeknopointBitMark, kTeknopointZeroSpace,
              kTeknopointBitMark, kDefaultMessageGap,
              data, nbytes,  // Bytes
              kTeknopointFreq, false, repeat, kDutyDefault);
}
#endif  // SEND_TEKNOPOINT

#if DECODE_TEKNOPOINT
/// Decode the supplied Teknopoint message.
/// Status: Alpha / Probably works.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeTeknopoint(decode_results *results, uint16_t offset,
                              const uint16_t nbits, const bool strict) {
  if (results->rawlen < 2 * nbits + kHeader + kFooter - 1 - offset)
    return false;  // Too short a message to match.
  if (strict && nbits != kTeknopointBits)
    return false;

  if (!matchGeneric(results->rawbuf + offset, results->state,
                    results->rawlen - offset, nbits,
                    kTeknopointHdrMark, kTeknopointHdrSpace,
                    kTeknopointBitMark, kTeknopointOneSpace,
                    kTeknopointBitMark, kTeknopointZeroSpace,
                    kTeknopointBitMark, kDefaultMessageGap,
                    true, _tolerance + kTeknopointExtraTol,
                    kMarkExcess, false)) return false;
  // Compliance
  if (strict) {
    // Is the checksum valid?
    if (sumBytes(results->state, kTeknopointStateLength - 1) !=
        results->state[kTeknopointStateLength - 1]) return false;
  }
  // Success
  results->decode_type = decode_type_t::TEKNOPOINT;
  results->bits = nbits;
  return true;
}
#endif  // DECODE_TEKNOPOINT

// Looking for the IRTeknopoint/IRTeknopointAc class?
// It doesn't exist, it is instead part of the `IRTcl112Ac` class.
// i.e. use `IRTcl112Ac::setModel(tcl_ac_remote_model_t::GZ055BE1);` for
// Teknopoint A/Cs.
