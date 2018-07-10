// Copyright 2017 David Conran

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtimer.h"
#include "IRutils.h"

//                RRRRRR   CCCCC          MM    MM MM    MM
//                RR   RR CC    C         MMM  MMM MMM  MMM
//                RRRRRR  CC       _____  MM MM MM MM MM MM
//                RR  RR  CC    C         MM    MM MM    MM
//                RR   RR  CCCCC          MM    MM MM    MM

// Send & decode support for RC-MM added by David Conran

// Constants
// Ref:
//   http://www.sbprojects.com/knowledge/ir/rcmm.php
#define RCMM_TICK                 28U  // Technically it would be 27.777*
#define RCMM_HDR_MARK_TICKS       15U
#define RCMM_HDR_MARK            416U
#define RCMM_HDR_SPACE_TICKS      10U
#define RCMM_HDR_SPACE           277U
#define RCMM_BIT_MARK_TICKS        6U
#define RCMM_BIT_MARK            166U
#define RCMM_BIT_SPACE_0_TICKS    10U
#define RCMM_BIT_SPACE_0         277U
#define RCMM_BIT_SPACE_1_TICKS    16U
#define RCMM_BIT_SPACE_1         444U
#define RCMM_BIT_SPACE_2_TICKS    22U
#define RCMM_BIT_SPACE_2         611U
#define RCMM_BIT_SPACE_3_TICKS    28U
#define RCMM_BIT_SPACE_3         777U
#define RCMM_RPT_LENGTH_TICKS    992U
#define RCMM_RPT_LENGTH        27778U
#define RCMM_MIN_GAP_TICKS       120U
#define RCMM_MIN_GAP            3360U
// Use a tolerance of +/-10% when matching some data spaces.
#define RCMM_TOLERANCE      10U
#define RCMM_EXCESS         50U

#if SEND_RCMM
// Send a Philips RC-MM packet.
//
// Args:
//   data: The data we want to send. MSB first.
//   nbits: The number of bits of data to send. (Typically 12, 24, or 32[Nokia])
//   repeat: The nr. of times the message should be sent.
//
// Status:  BETA / Should be working.
//
// Ref:
//   http://www.sbprojects.com/knowledge/ir/rcmm.php
void IRsend::sendRCMM(uint64_t data, uint16_t nbits, uint16_t repeat) {
  // Set 36kHz IR carrier frequency & a 1/3 (33%) duty cycle.
  enableIROut(36, 33);
  IRtimer usecs = IRtimer();

  for (uint16_t r = 0; r <= repeat; r++) {
    usecs.reset();
    // Header
    mark(RCMM_HDR_MARK);
    space(RCMM_HDR_SPACE);
    // Data
    uint64_t mask = 0b11ULL << (nbits - 2);
    // RC-MM sends data 2 bits at a time.
    for (int32_t i = nbits; i > 0; i -= 2) {
      mark(RCMM_BIT_MARK);
      // Grab the next Most Significant Bits to send.
      switch ((data & mask) >> (i - 2)) {
        case 0b00: space(RCMM_BIT_SPACE_0); break;
        case 0b01: space(RCMM_BIT_SPACE_1); break;
        case 0b10: space(RCMM_BIT_SPACE_2); break;
        case 0b11: space(RCMM_BIT_SPACE_3); break;
      }
      mask >>= 2;
    }
    // Footer
    mark(RCMM_BIT_MARK);
    // Protocol requires us to wait at least RCMM_RPT_LENGTH usecs from the
    // start or RCMM_MIN_GAP usecs.
    space(std::max(RCMM_RPT_LENGTH - usecs.elapsed(), RCMM_MIN_GAP));
  }
}
#endif

#if DECODE_RCMM
// Decode a Philips RC-MM packet (between 12 & 32 bits) if possible.
// Places successful decode information in the results pointer.
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of bits to expect in the data portion. Typically RCMM_BITS.
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status:  BETA / Should be working.
//
// Ref:
//   http://www.sbprojects.com/knowledge/ir/rcmm.php
bool IRrecv::decodeRCMM(decode_results *results, uint16_t nbits, bool strict) {
  uint64_t data = 0;
  uint16_t offset = OFFSET_START;

  if (results->rawlen <= 4)
    return false;  // Not enough entries to ever be RCMM.

  // Calc the maximum size in bits, the message can be, or that we can accept.
  int16_t maxBitSize = std::min((uint16_t) results->rawlen - 4,
                                (uint16_t) sizeof(data) * 8);
  // Compliance
  if (strict) {
    // Technically the spec says bit sizes should be 12 xor 24. however
    // 32 bits has been seen from a device. We are going to assume
    // 12 <= bits <= 32 is the 'required' bit length for the spec.
    if (maxBitSize < 12 || maxBitSize > 32)
      return false;
    if (maxBitSize < nbits)
      return false;  // Short cut, we can never reach the expected nr. of bits.
  }
  // Header decode
  if (!matchMark(results->rawbuf[offset], RCMM_HDR_MARK)) return false;
  // Calculate how long the common tick time is based on the header mark.
  uint32_t m_tick = results->rawbuf[offset++] * RAWTICK / RCMM_HDR_MARK_TICKS;
  if (!matchSpace(results->rawbuf[offset], RCMM_HDR_SPACE)) return false;
  // Calculate how long the common tick time is based on the header space.
  uint32_t s_tick = results->rawbuf[offset++] * RAWTICK / RCMM_HDR_SPACE_TICKS;

  // Data decode
  // RC-MM has two bits of data per mark/space pair.
  uint16_t actualBits;
  for (actualBits = 0; actualBits < maxBitSize; actualBits += 2, offset++) {
    if (!match(results->rawbuf[offset++], RCMM_BIT_MARK_TICKS * m_tick))
      return false;

    data <<= 2;
    // Use non-default tolerance & excess for matching some of the spaces as the
    // defaults are too generous and causes mis-matches in some cases.
    if (match(results->rawbuf[offset], RCMM_BIT_SPACE_0_TICKS * s_tick,
              TOLERANCE))
      data += 0;
    else if (match(results->rawbuf[offset], RCMM_BIT_SPACE_1_TICKS * s_tick,
                   TOLERANCE))
      data += 1;
    else if (match(results->rawbuf[offset], RCMM_BIT_SPACE_2_TICKS * s_tick,
                   RCMM_TOLERANCE))
      data += 2;
    else if (match(results->rawbuf[offset], RCMM_BIT_SPACE_3_TICKS * s_tick,
                   RCMM_TOLERANCE))
      data += 3;
    else
      return false;
  }
  // Footer decode
  if (!match(results->rawbuf[offset++], RCMM_BIT_MARK_TICKS * m_tick))
    return false;
  if (offset < results->rawlen &&
      !matchAtLeast(results->rawbuf[offset], RCMM_MIN_GAP_TICKS * s_tick))
    return false;

  // Compliance
  if (strict && actualBits != nbits)
    return false;

  // Success
  results->value = data;
  results->decode_type = RCMM;
  results->bits = actualBits;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif
