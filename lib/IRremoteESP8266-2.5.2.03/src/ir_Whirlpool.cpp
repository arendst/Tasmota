// Copyright 2018 David Conran
//
// Code to emulate Whirlpool protocol compatible devices.
// Should be compatible with:
// * SPIS409L, SPIS412L, SPIW409L, SPIW412L, SPIW418L
//

#include <algorithm>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "IRutils.h"

//    WW      WW HH   HH IIIII RRRRRR  LL      PPPPPP   OOOOO   OOOOO  LL
//    WW      WW HH   HH  III  RR   RR LL      PP   PP OO   OO OO   OO LL
//    WW   W  WW HHHHHHH  III  RRRRRR  LL      PPPPPP  OO   OO OO   OO LL
//     WW WWW WW HH   HH  III  RR  RR  LL      PP      OO   OO OO   OO LL
//      WW   WW  HH   HH IIIII RR   RR LLLLLLL PP       OOOO0   OOOO0  LLLLLLL

// Constants
// Ref: https://github.com/markszabo/IRremoteESP8266/issues/509
const uint16_t kWhirlpoolAcHdrMark = 8950;
const uint16_t kWhirlpoolAcHdrSpace = 4484;
const uint16_t kWhirlpoolAcBitMark = 597;
const uint16_t kWhirlpoolAcOneSpace = 1649;
const uint16_t kWhirlpoolAcZeroSpace = 533;
const uint16_t kWhirlpoolAcGap = 7920;
const uint32_t kWhirlpoolAcMinGap = 100000;  // Completely made up value.
const uint8_t kWhirlpoolAcSections = 3;

#if SEND_WHIRLPOOL_AC
// Send a Whirlpool A/C message.
//
// Args:
//   data: An array of bytes containing the IR command.
//   nbytes: Nr. of bytes of data in the array. (>=kWhirlpoolAcStateLength)
//   repeat: Nr. of times the message is to be repeated. (Default = 0).
//
// Status: ALPHA / Untested.
//
// Ref:
//   https://github.com/markszabo/IRremoteESP8266/issues/509
void IRsend::sendWhirlpoolAC(unsigned char data[], uint16_t nbytes,
                             uint16_t repeat) {
  if (nbytes < kWhirlpoolAcStateLength)
    return;  // Not enough bytes to send a proper message.
  for (uint16_t r = 0; r <= repeat; r++) {
    // Section 1
    sendGeneric(kWhirlpoolAcHdrMark, kWhirlpoolAcHdrSpace, kWhirlpoolAcBitMark,
                kWhirlpoolAcOneSpace, kWhirlpoolAcBitMark,
                kWhirlpoolAcZeroSpace, kWhirlpoolAcBitMark, kWhirlpoolAcGap,
                data, 6,  // 6 bytes == 48 bits
                38000,    // Complete guess of the modulation frequency.
                false, 0, 50);
    // Section 2
    sendGeneric(0, 0, kWhirlpoolAcBitMark, kWhirlpoolAcOneSpace,
                kWhirlpoolAcBitMark, kWhirlpoolAcZeroSpace, kWhirlpoolAcBitMark,
                kWhirlpoolAcGap, data + 6, 8,  // 8 bytes == 64 bits
                38000,  // Complete guess of the modulation frequency.
                false, 0, 50);
    // Section 3
    sendGeneric(0, 0, kWhirlpoolAcBitMark, kWhirlpoolAcOneSpace,
                kWhirlpoolAcBitMark, kWhirlpoolAcZeroSpace, kWhirlpoolAcBitMark,
                kWhirlpoolAcMinGap, data + 14, 7,  // 7 bytes == 56 bits
                38000,  // Complete guess of the modulation frequency.
                false, 0, 50);
  }
}
#endif  // SEND_WHIRLPOOL_AC

#if DECODE_WHIRLPOOL_AC
// Decode the supplied Whirlpool A/C message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect. Typically kWhirlpoolAcBits
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: ALPHA / Untested.
//
//
// Ref:
//   https://github.com/markszabo/IRremoteESP8266/issues/509
bool IRrecv::decodeWhirlpoolAC(decode_results *results, uint16_t nbits,
                               bool strict) {
  if (results->rawlen < 2 * nbits + 4 + kHeader + kFooter - 1)
    return false;  // Can't possibly be a valid Whirlpool A/C message.
  if (strict) {
    if (nbits != kWhirlpoolAcBits) return false;
  }

  uint16_t offset = kStartOffset;
  uint16_t dataBitsSoFar = 0;
  uint16_t i = 0;
  match_result_t data_result;
  uint8_t sectionSize[kWhirlpoolAcSections] = {6, 8, 7};

  // Header
  if (!matchMark(results->rawbuf[offset++], kWhirlpoolAcHdrMark)) return false;
  if (!matchSpace(results->rawbuf[offset++], kWhirlpoolAcHdrSpace))
    return false;

  // Data Section
  // Keep reading bytes until we either run out of section or state to fill.
  for (uint8_t section = 0, pos = 0; section < kWhirlpoolAcSections;
       section++) {
    pos += sectionSize[section];
    for (; offset <= results->rawlen - 16 && i < pos;
         i++, dataBitsSoFar += 8, offset += data_result.used) {
      data_result =
          matchData(&(results->rawbuf[offset]), 8, kWhirlpoolAcBitMark,
                    kWhirlpoolAcOneSpace, kWhirlpoolAcBitMark,
                    kWhirlpoolAcZeroSpace, kTolerance, kMarkExcess, false);
      if (data_result.success == false) break;  // Fail
      // Data is in LSB order. We need to reverse it.
      results->state[i] = (uint8_t)data_result.data;
    }
    // Section Footer
    if (!matchMark(results->rawbuf[offset++], kWhirlpoolAcBitMark))
      return false;
    if (section < kWhirlpoolAcSections - 1) {  // Inter-section gaps.
      if (!matchSpace(results->rawbuf[offset++], kWhirlpoolAcGap)) return false;
    } else {  // Last section / End of message gap.
      if (offset <= results->rawlen &&
          !matchAtLeast(results->rawbuf[offset++], kWhirlpoolAcGap))
        return false;
    }
  }

  // Compliance
  if (strict) {
    // Re-check we got the correct size/length due to the way we read the data.
    if (dataBitsSoFar != kWhirlpoolAcBits) return false;
  }

  // Success
  results->decode_type = WHIRLPOOL_AC;
  results->bits = dataBitsSoFar;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // WHIRLPOOL_AC
