// Copyright 2020 David Conran

#include "IRrecv.h"
#include "IRsend.h"

// Airwell "Manchester code" based protocol.
// Some other Airwell products use the COOLIX protocol.
//
// Supports:
//   Brand: Airwell,  Model: RC08W remote

const uint8_t kAirwellOverhead = 4;
const uint16_t kAirwellHalfClockPeriod = 950;  // uSeconds
const uint16_t kAirwellHdrMark = 3 * kAirwellHalfClockPeriod;  // uSeconds
const uint16_t kAirwellHdrSpace = 4 * kAirwellHalfClockPeriod;  // uSeconds
const uint16_t kAirwellFooterMark = 5 * kAirwellHalfClockPeriod;  // uSeconds

#if SEND_AIRWELL
// Send an Airwell Manchester Code formatted message.
//
// Args:
//   data:   The message to be sent.
//   nbits:  The number of bits of the message to be sent.
//           Typically kAirwellBits.
//   repeat: The number of times the command is to be repeated.
//
// Status: BETA / Appears to be working.
//
// Ref:
//  https://github.com/crankyoldgit/IRremoteESP8266/issues/1069
void IRsend::sendAirwell(uint64_t data, uint16_t nbits, uint16_t repeat) {
  // Header + Data
  sendManchester(kAirwellHdrMark, kAirwellHdrMark, kAirwellHalfClockPeriod,
                 0, 0, data, nbits, 38000, true, repeat);
  // Footer
  mark(kAirwellHdrMark + kAirwellHalfClockPeriod);
  space(kDefaultMessageGap);  // A guess.
}
#endif

#if DECODE_AIRWELL
// Decode the supplied Airwell "Manchester code" message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   offset:  The starting index to use when attempting to decode the raw data.
//            Typically/Defaults to kStartOffset.
//   nbits:   The number of data bits to expect. Typically kAirwellBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: BETA / Appears to be working.
//
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/1069
bool IRrecv::decodeAirwell(decode_results *results, uint16_t offset,
                           const uint16_t nbits, const bool strict) {
  if (results->rawlen < nbits + kAirwellOverhead - offset)
    return false;  // Too short a message to match.

  // Compliance
  if (strict && nbits != kAirwellBits)
    return false;  // Doesn't match our protocol defn.

  // Header #1 + Data #1 + Footer #1 (There are total of 3 sections)
  uint16_t used = matchManchester(results->rawbuf + offset, &results->value,
                                  results->rawlen - offset, nbits,
                                  kAirwellHdrMark, kAirwellHdrMark,
                                  kAirwellHalfClockPeriod,
                                  kAirwellHdrMark, kAirwellHdrSpace,
                                  true);
  if (used == 0) return false;
  offset += used;

  // Success
  results->decode_type = decode_type_t::AIRWELL;
  results->bits = nbits;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif
