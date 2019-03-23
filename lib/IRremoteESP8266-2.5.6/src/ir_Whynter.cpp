// Copyright 2009 Ken Shirriff
// Copyright 2017 David Conran

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

//               W   W  H   H  Y   Y N   N TTTTT EEEEE  RRRRR
//               W   W  H   H   Y Y  NN  N   T   E      R   R
//               W W W  HHHHH    Y   N N N   T   EEE    RRRR
//               W W W  H   H    Y   N  NN   T   E      R  R
//                WWW   H   H    Y   N   N   T   EEEEE  R   R

// Whynter A/C ARC-110WD added by Francesco Meschia
// Whynter originally added from https://github.com/shirriff/Arduino-IRremote/

// Constants

const uint16_t kWhynterTick = 50;
const uint16_t kWhynterHdrMarkTicks = 57;
const uint16_t kWhynterHdrMark = kWhynterHdrMarkTicks * kWhynterTick;
const uint16_t kWhynterHdrSpaceTicks = 57;
const uint16_t kWhynterHdrSpace = kWhynterHdrSpaceTicks * kWhynterTick;
const uint16_t kWhynterBitMarkTicks = 15;
const uint16_t kWhynterBitMark = kWhynterBitMarkTicks * kWhynterTick;
const uint16_t kWhynterOneSpaceTicks = 43;
const uint16_t kWhynterOneSpace = kWhynterOneSpaceTicks * kWhynterTick;
const uint16_t kWhynterZeroSpaceTicks = 15;
const uint16_t kWhynterZeroSpace = kWhynterZeroSpaceTicks * kWhynterTick;
const uint16_t kWhynterMinCommandLengthTicks = 2160;  // Totally made up value.
const uint32_t kWhynterMinCommandLength =
    kWhynterMinCommandLengthTicks * kWhynterTick;
const uint16_t kWhynterMinGapTicks =
    kWhynterMinCommandLengthTicks -
    (2 * (kWhynterBitMarkTicks + kWhynterZeroSpaceTicks) +
     kWhynterBits * (kWhynterBitMarkTicks + kWhynterOneSpaceTicks));
const uint16_t kWhynterMinGap = kWhynterMinGapTicks * kWhynterTick;

#if SEND_WHYNTER
// Send a Whynter message.
//
// Args:
//   data: message to be sent.
//   nbits: Nr. of bits of the message to be sent.
//   repeat: Nr. of additional times the message is to be sent.
//
// Status: STABLE
//
// Ref:
//   https://github.com/z3t0/Arduino-IRremote/blob/master/ir_Whynter.cpp
void IRsend::sendWhynter(uint64_t data, uint16_t nbits, uint16_t repeat) {
  // Set IR carrier frequency
  enableIROut(38);

  for (uint16_t i = 0; i <= repeat; i++) {
    // (Pre-)Header
    mark(kWhynterBitMark);
    space(kWhynterZeroSpace);
    sendGeneric(
        kWhynterHdrMark, kWhynterHdrSpace, kWhynterBitMark, kWhynterOneSpace,
        kWhynterBitMark, kWhynterZeroSpace, kWhynterBitMark, kWhynterMinGap,
        kWhynterMinCommandLength - (kWhynterBitMark + kWhynterZeroSpace), data,
        nbits, 38, true, 0,  // Repeats are already handled.
        50);
  }
}
#endif

#if DECODE_WHYNTER
// Decode the supplied Whynter message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of data bits to expect.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: BETA  Strict mode is ALPHA.
//
// Ref:
//   https://github.com/z3t0/Arduino-IRremote/blob/master/ir_Whynter.cpp
bool IRrecv::decodeWhynter(decode_results *results, uint16_t nbits,
                           bool strict) {
  if (results->rawlen < 2 * nbits + 2 * kHeader + kFooter - 1)
    return false;  // We don't have enough entries to possibly match.

  // Compliance
  if (strict && nbits != kWhynterBits)
    return false;  // Incorrect nr. of bits per spec.

  uint16_t offset = kStartOffset;

  // Header
  // Sequence begins with a bit mark and a zero space.
  // These are typically small, so we'll prefer to do the calibration
  // on the much larger header mark & space that are next.
  if (!matchMark(results->rawbuf[offset++], kWhynterBitMark)) return false;
  if (!matchSpace(results->rawbuf[offset++], kWhynterZeroSpace)) return false;
  // Main header mark and space
  if (!matchMark(results->rawbuf[offset], kWhynterHdrMark)) return false;
  // Calculate how long the common tick time is based on the header mark.
  uint32_t m_tick = results->rawbuf[offset++] * kRawTick / kWhynterHdrMarkTicks;
  if (!matchSpace(results->rawbuf[offset], kWhynterHdrSpace)) return false;
  // Calculate how long the common tick time is based on the header space.
  uint32_t s_tick =
      results->rawbuf[offset++] * kRawTick / kWhynterHdrSpaceTicks;

  // Data
  uint64_t data = 0;
  match_result_t data_result =
      matchData(&(results->rawbuf[offset]), nbits,
                kWhynterBitMarkTicks * m_tick, kWhynterOneSpaceTicks * s_tick,
                kWhynterBitMarkTicks * m_tick, kWhynterZeroSpaceTicks * s_tick);
  if (data_result.success == false) return false;
  data = data_result.data;
  offset += data_result.used;

  // Footer
  if (!matchMark(results->rawbuf[offset++], kWhynterBitMarkTicks * m_tick))
    return false;
  if (offset < results->rawlen &&
      !matchAtLeast(results->rawbuf[offset], kWhynterMinGapTicks * s_tick))
    return false;

  // Success
  results->decode_type = WHYNTER;
  results->bits = nbits;
  results->value = data;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif
