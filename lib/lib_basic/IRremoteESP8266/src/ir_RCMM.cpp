// Copyright 2017 David Conran

/// @file
/// @brief Support for the Phillips RC-MM protocol.
/// @see http://www.sbprojects.net/knowledge/ir/rcmm.php

// Supports:
//   Brand: Microsoft,  Model: XBOX 360

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtimer.h"
#include "IRutils.h"

// Constants
const uint16_t kRcmmTick = 28;  // Technically it would be 27.777*
const uint16_t kRcmmHdrMarkTicks = 15;
const uint16_t kRcmmHdrMark = 416;
const uint16_t kRcmmHdrSpaceTicks = 10;
const uint16_t kRcmmHdrSpace = 277;
const uint16_t kRcmmBitMarkTicks = 6;
const uint16_t kRcmmBitMark = 166;
const uint16_t kRcmmBitSpace0Ticks = 10;
const uint16_t kRcmmBitSpace0 = 277;
const uint16_t kRcmmBitSpace1Ticks = 16;
const uint16_t kRcmmBitSpace1 = 444;
const uint16_t kRcmmBitSpace2Ticks = 22;
const uint16_t kRcmmBitSpace2 = 611;
const uint16_t kRcmmBitSpace3Ticks = 28;
const uint16_t kRcmmBitSpace3 = 777;
const uint16_t kRcmmRptLengthTicks = 992;
const uint32_t kRcmmRptLength = 27778;
const uint16_t kRcmmMinGapTicks = 120;
const uint32_t kRcmmMinGap = 3360;
// Use a tolerance of +/-10% when matching some data spaces.
const uint8_t kRcmmTolerance = 10;
const uint16_t kRcmmExcess = 50;

#if SEND_RCMM
/// Send a Philips RC-MM packet.
/// Status: STABLE / Should be working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendRCMM(uint64_t data, uint16_t nbits, uint16_t repeat) {
  // Set 36kHz IR carrier frequency & a 1/3 (33%) duty cycle.
  enableIROut(36, 33);
  IRtimer usecs = IRtimer();

  for (uint16_t r = 0; r <= repeat; r++) {
    usecs.reset();
    // Header
    mark(kRcmmHdrMark);
    space(kRcmmHdrSpace);
    // Data
    uint64_t mask = 0b11ULL << (nbits - 2);
    // RC-MM sends data 2 bits at a time.
    for (int32_t i = nbits; i > 0; i -= 2) {
      mark(kRcmmBitMark);
      // Grab the next Most Significant Bits to send.
      switch ((data & mask) >> (i - 2)) {
        case 0b00:
          space(kRcmmBitSpace0);
          break;
        case 0b01:
          space(kRcmmBitSpace1);
          break;
        case 0b10:
          space(kRcmmBitSpace2);
          break;
        case 0b11:
          space(kRcmmBitSpace3);
          break;
      }
      mask >>= 2;
    }
    // Footer
    mark(kRcmmBitMark);
    // Protocol requires us to wait at least kRcmmRptLength usecs from the
    // start or kRcmmMinGap usecs.
    space(std::max(kRcmmRptLength - usecs.elapsed(), kRcmmMinGap));
  }
}
#endif  // SEND_RCMM

#if DECODE_RCMM
/// Decode a Philips RC-MM packet (between 12 & 32 bits) if possible.
/// Status:  STABLE / Should be working.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
bool IRrecv::decodeRCMM(decode_results *results, uint16_t offset,
                        const uint16_t nbits, const bool strict) {
  uint64_t data = 0;

  if (results->rawlen <= 4 + offset - 1)
    return false;  // Not enough entries to ever be RCMM.

  // Calc the maximum size in bits, the message can be, or that we can accept.
  int16_t maxBitSize =
      std::min((uint16_t)results->rawlen - 5, (uint16_t)sizeof(data) * 8);
  // Compliance
  if (strict) {
    // Technically the spec says bit sizes should be 12 xor 24. however
    // 32 bits has been seen from a device. We are going to assume
    // 12 <= bits <= 32 is the 'required' bit length for the spec.
    if (maxBitSize < 12 || maxBitSize > 32) return false;
    if (maxBitSize < nbits)
      return false;  // Short cut, we can never reach the expected nr. of bits.
  }
  // Header decode
  if (!matchMark(results->rawbuf[offset], kRcmmHdrMark)) return false;
  // Calculate how long the common tick time is based on the header mark.
  uint32_t m_tick = results->rawbuf[offset++] * kRawTick / kRcmmHdrMarkTicks;
  if (!matchSpace(results->rawbuf[offset], kRcmmHdrSpace)) return false;
  // Calculate how long the common tick time is based on the header space.
  uint32_t s_tick = results->rawbuf[offset++] * kRawTick / kRcmmHdrSpaceTicks;

  // Data decode
  // RC-MM has two bits of data per mark/space pair.
  uint16_t actualBits;
  for (actualBits = 0; actualBits < maxBitSize; actualBits += 2, offset++) {
    if (!match(results->rawbuf[offset++], kRcmmBitMarkTicks * m_tick))
      return false;

    data <<= 2;
    // Use non-default tolerance & excess for matching some of the spaces as the
    // defaults are too generous and causes mis-matches in some cases.
    if (match(results->rawbuf[offset], kRcmmBitSpace0Ticks * s_tick))
      data += 0;
    else if (match(results->rawbuf[offset], kRcmmBitSpace1Ticks * s_tick))
      data += 1;
    else if (match(results->rawbuf[offset], kRcmmBitSpace2Ticks * s_tick,
                   kRcmmTolerance))
      data += 2;
    else if (match(results->rawbuf[offset], kRcmmBitSpace3Ticks * s_tick,
                   kRcmmTolerance))
      data += 3;
    else
      return false;
  }
  // Footer decode
  if (!match(results->rawbuf[offset++], kRcmmBitMarkTicks * m_tick))
    return false;
  if (offset < results->rawlen &&
      !matchAtLeast(results->rawbuf[offset], kRcmmMinGapTicks * s_tick))
    return false;

  // Compliance
  if (strict && actualBits != nbits) return false;

  // Success
  results->value = data;
  results->decode_type = RCMM;
  results->bits = actualBits;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif  // DECODE_RCMM
