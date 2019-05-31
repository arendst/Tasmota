// Copyright 2018 David Conran

#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

//             CCCCC    AAA   RRRRRR  RRRRRR  IIIII EEEEEEE RRRRRR
//            CC    C  AAAAA  RR   RR RR   RR  III  EE      RR   RR
//            CC      AA   AA RRRRRR  RRRRRR   III  EEEEE   RRRRRR
//            CC    C AAAAAAA RR  RR  RR  RR   III  EE      RR  RR
//             CCCCC  AA   AA RR   RR RR   RR IIIII EEEEEEE RR   RR

// Suits Carrier/Surrey HVAC models:
//   42QG5A55970 (remote)
//   619EGX0090E0 / 619EGX0120E0 / 619EGX0180E0 / 619EGX0220E0 (indoor units)
//   53NGK009/012 (inverter)

// Constants
// Ref:
//   https://github.com/markszabo/IRremoteESP8266/issues/385
const uint16_t kCarrierAcHdrMark = 8532;
const uint16_t kCarrierAcHdrSpace = 4228;
const uint16_t kCarrierAcBitMark = 628;
const uint16_t kCarrierAcOneSpace = 1320;
const uint16_t kCarrierAcZeroSpace = 532;
const uint16_t kCarrierAcGap = 20000;

#if SEND_CARRIER_AC
// Send a Carrier HVAC formatted message.
//
// Args:
//   data:   The message to be sent.
//   nbits:  The bit size of the message being sent. typically kCarrierAcBits.
//   repeat: The number of times the message is to be repeated.
//
// Status: BETA / Appears to work on real devices.
//
void IRsend::sendCarrierAC(uint64_t data, uint16_t nbits, uint16_t repeat) {
  for (uint16_t r = 0; r <= repeat; r++) {
    uint64_t temp_data = data;
    // Carrier sends the data block three times. normal + inverted + normal.
    for (uint16_t i = 0; i < 3; i++) {
      sendGeneric(kCarrierAcHdrMark, kCarrierAcHdrSpace, kCarrierAcBitMark,
                  kCarrierAcOneSpace, kCarrierAcBitMark, kCarrierAcZeroSpace,
                  kCarrierAcBitMark, kCarrierAcGap, temp_data, nbits, 38, true,
                  0, kDutyDefault);
      temp_data = invertBits(temp_data, nbits);
    }
  }
}
#endif

#if DECODE_CARRIER_AC
// Decode the supplied Carrier HVAC message.
// Carrier HVAC messages contain only 32 bits, but it is sent three(3) times.
// i.e. normal + inverted + normal
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of bits to expect in the data portion.
//            Typically kCarrierAcBits.
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: ALPHA / Untested.
//
bool IRrecv::decodeCarrierAC(decode_results *results, uint16_t nbits,
                             bool strict) {
  if (results->rawlen < ((2 * nbits + kHeader + kFooter) * 3) - 1)
    return false;  // Can't possibly be a valid Carrier message.
  if (strict && nbits != kCarrierAcBits)
    return false;  // We expect Carrier to be 32 bits of message.

  uint64_t data = 0;
  uint64_t prev_data = 0;
  uint16_t offset = kStartOffset;

  for (uint8_t i = 0; i < 3; i++) {
    prev_data = data;
    // Header
    if (!matchMark(results->rawbuf[offset++], kCarrierAcHdrMark)) return false;
    if (!matchSpace(results->rawbuf[offset++], kCarrierAcHdrSpace))
      return false;
    // Data
    match_result_t data_result =
        matchData(&(results->rawbuf[offset]), nbits, kCarrierAcBitMark,
                  kCarrierAcOneSpace, kCarrierAcBitMark, kCarrierAcZeroSpace);
    if (data_result.success == false) return false;
    data = data_result.data;
    offset += data_result.used;
    // Footer
    if (!matchMark(results->rawbuf[offset++], kCarrierAcBitMark)) return false;
    if (offset < results->rawlen &&
        !matchAtLeast(results->rawbuf[offset++], kCarrierAcGap))
      return false;
    // Compliance.
    if (strict) {
      // Check if the data is an inverted copy of the previous data.
      if (i > 0 && prev_data != invertBits(data, nbits)) return false;
    }
  }

  // Success
  results->bits = nbits;
  results->value = data;
  results->decode_type = CARRIER_AC;
  results->address = data >> 16;
  results->command = data & 0xFFFF;
  return true;
}
#endif
