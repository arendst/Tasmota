// Copyright 2018 David Conran

#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

//          EEEEEEE LL      EEEEEEE  CCCCC  TTTTTTT RRRRRR    AAA
//          EE      LL      EE      CC    C   TTT   RR   RR  AAAAA
//          EEEEE   LL      EEEEE   CC        TTT   RRRRRR  AA   AA
//          EE      LL      EE      CC    C   TTT   RR  RR  AAAAAAA
//          EEEEEEE LLLLLLL EEEEEEE  CCCCC    TTT   RR   RR AA   AA

// Electra A/C added by crankyoldgit
//
// Equipment it seems compatible with:
//  * <Add models (A/C & remotes) you've gotten it working with here>

// Ref:
//   https://github.com/markszabo/IRremoteESP8266/issues/527

// Constants
const uint16_t kElectraAcHdrMark = 9166;
const uint16_t kElectraAcBitMark = 646;
const uint16_t kElectraAcHdrSpace = 4470;
const uint16_t kElectraAcOneSpace = 1647;
const uint16_t kElectraAcZeroSpace = 547;
const uint32_t kElectraAcMessageGap = 100000;  // Completely made-up guess.

#if SEND_ELECTRA_AC
// Send a Electra message
//
// Args:
//   data:   Contents of the message to be sent. (Guessing MSBF order)
//   nbits:  Nr. of bits of data to be sent. Typically kElectraAcBits.
//   repeat: Nr. of additional times the message is to be sent.
//
// Status: Alpha / Needs testing against a real device.
//
void IRsend::sendElectraAC(uint8_t data[], uint16_t nbytes, uint16_t repeat) {
  for (uint16_t r = 0; r <= repeat; r++)
    sendGeneric(kElectraAcHdrMark, kElectraAcHdrSpace, kElectraAcBitMark,
                kElectraAcOneSpace, kElectraAcBitMark, kElectraAcZeroSpace,
                kElectraAcBitMark, kElectraAcMessageGap, data, nbytes,
                38000,  // Complete guess of the modulation frequency.
                true, 0, 50);
}
#endif

#if DECODE_ELECTRA_AC
// Decode the supplied Electra A/C message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect. Typically kElectraAcBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: Alpha / Needs testing against a real device.
//
bool IRrecv::decodeElectraAC(decode_results *results, uint16_t nbits,
                             bool strict) {
  if (nbits % 8 != 0)  // nbits has to be a multiple of nr. of bits in a byte.
    return false;

  if (strict) {
    if (nbits != kElectraAcBits)
      return false;  // Not strictly a ELECTRA_AC message.
  }

  // The protocol sends the data normal + inverted, alternating on
  // each byte. Hence twice the number of expected data bits.
  if (results->rawlen < 2 * nbits + kHeader + kFooter - 1)
    return false;  // Can't possibly be a valid ELECTRA_AC message.

  uint16_t offset = kStartOffset;

  // Message Header
  if (!matchMark(results->rawbuf[offset++], kElectraAcHdrMark)) return false;
  if (!matchSpace(results->rawbuf[offset++], kElectraAcHdrSpace)) return false;

  // Data Section
  match_result_t data_result;
  uint16_t dataBitsSoFar = 0;
  // Keep reading bytes until we either run out of section or state to fill.
  for (uint16_t i = 0; offset <= results->rawlen - 16 && i < nbits / 8;
       i++, dataBitsSoFar += 8, offset += data_result.used) {
    data_result = matchData(&(results->rawbuf[offset]), 8, kElectraAcBitMark,
                            kElectraAcOneSpace, kElectraAcBitMark,
                            kElectraAcZeroSpace, kTolerance, 0, true);
    if (data_result.success == false) return false;  // Fail
    results->state[i] = data_result.data;
  }

  // Message Footer
  if (!matchMark(results->rawbuf[offset++], kElectraAcBitMark)) return false;
  if (offset <= results->rawlen &&
      !matchAtLeast(results->rawbuf[offset++], kElectraAcMessageGap))
    return false;

  // Compliance
  if (strict && dataBitsSoFar != nbits) return false;

  // Success
  results->decode_type = ELECTRA_AC;
  results->bits = dataBitsSoFar;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_ELECTRA_AC
