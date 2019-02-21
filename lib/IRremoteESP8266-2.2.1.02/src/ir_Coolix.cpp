// Copyright bakrus
// Copyright 2017 David Conran

#include "IRrecv.h"
#include "IRsend.h"
#include "IRtimer.h"
#include "IRutils.h"

//             CCCCC   OOOOO   OOOOO  LL      IIIII XX    XX
//            CC    C OO   OO OO   OO LL       III   XX  XX
//            CC      OO   OO OO   OO LL       III    XXXX
//            CC    C OO   OO OO   OO LL       III   XX  XX
//             CCCCC   OOOO0   OOOO0  LLLLLLL IIIII XX    XX

// Coolix A/C / heatpump added by (send) bakrus & (decode) crankyoldgit

// Constants
// Pulse parms are *50-100 for the Mark and *50+100 for the space
// First MARK is the one after the long gap
// pulse parameters in usec
#define COOLIX_TICK             560U  // Approximately 21 cycles at 38kHz
#define COOLIX_BIT_MARK_TICKS     1U
#define COOLIX_BIT_MARK         (COOLIX_BIT_MARK_TICKS * COOLIX_TICK)
#define COOLIX_ONE_SPACE_TICKS    3U
#define COOLIX_ONE_SPACE        (COOLIX_ONE_SPACE_TICKS * COOLIX_TICK)
#define COOLIX_ZERO_SPACE_TICKS   1U
#define COOLIX_ZERO_SPACE       (COOLIX_ZERO_SPACE_TICKS * COOLIX_TICK)
#define COOLIX_HDR_MARK_TICKS     8U
#define COOLIX_HDR_MARK         (COOLIX_HDR_MARK_TICKS * COOLIX_TICK)
#define COOLIX_HDR_SPACE_TICKS    8U
#define COOLIX_HDR_SPACE        (COOLIX_HDR_SPACE_TICKS * COOLIX_TICK)
#define COOLIX_MIN_GAP_TICKS    (COOLIX_HDR_MARK_TICKS + \
                                 COOLIX_ZERO_SPACE_TICKS)
#define COOLIX_MIN_GAP          (COOLIX_MIN_GAP_TICKS * COOLIX_TICK)

#if SEND_COOLIX
// Send a Coolix message
//
// Args:
//   data:   Contents of the message to be sent.
//   nbits:  Nr. of bits of data to be sent. Typically COOLIX_BITS.
//   repeat: Nr. of additional times the message is to be sent.
//
// Status: BETA / Probably works.
//
// Ref:
//   https://github.com/z3t0/Arduino-IRremote/blob/master/ir_COOLIX.cpp
// TODO(anyone): Verify repeat functionality against a real unit.
void IRsend::sendCOOLIX(uint64_t data, uint16_t nbits, uint16_t repeat) {
  if (nbits % 8 != 0)
    return;  // nbits is required to be a multiple of 8.

  // Set IR carrier frequency
  enableIROut(38);

  for (uint16_t r = 0; r <= repeat; r++) {
    // Header
    mark(COOLIX_HDR_MARK);
    space(COOLIX_HDR_SPACE);

    // Data
    //   Break data into byte segments, starting at the Most Significant
    //   Byte. Each byte then being sent normal, then followed inverted.
    for (uint16_t i = 8; i <= nbits; i += 8) {
      // Grab a bytes worth of data.
      uint8_t segment = (data >> (nbits - i)) & 0xFF;
      // Normal
      sendData(COOLIX_BIT_MARK, COOLIX_ONE_SPACE,
               COOLIX_BIT_MARK, COOLIX_ZERO_SPACE,
               segment, 8, true);
      // Inverted.
      sendData(COOLIX_BIT_MARK, COOLIX_ONE_SPACE,
               COOLIX_BIT_MARK, COOLIX_ZERO_SPACE,
               segment ^ 0xFF, 8, true);
    }

    // Footer
    mark(COOLIX_BIT_MARK);
    space(COOLIX_MIN_GAP);  // Pause before repeating
  }
}
#endif

#if DECODE_COOLIX
// Decode the supplied Coolix message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect. Typically COOLIX_BITS.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: BETA / Probably working.
bool IRrecv::decodeCOOLIX(decode_results *results, uint16_t nbits,
                          bool strict) {
  // The protocol sends the data normal + inverted, alternating on
  // each byte. Hence twice the number of expected data bits.
  if (results->rawlen < 2 * 2 * nbits + HEADER + FOOTER - 1)
    return false;  // Can't possibly be a valid COOLIX message.
  if (strict && nbits != COOLIX_BITS)
    return false;  // Not strictly an COOLIX message.
  if (nbits % 8 != 0)  // nbits has to be a multiple of nr. of bits in a byte.
    return false;

  uint64_t data = 0;
  uint64_t inverted = 0;
  uint16_t offset = OFFSET_START;

  if (nbits > sizeof(data) * 8)
    return false;  // We can't possibly capture a Coolix packet that big.

  // Header
  if (!matchMark(results->rawbuf[offset], COOLIX_HDR_MARK)) return false;
  // Calculate how long the common tick time is based on the header mark.
  uint32_t m_tick = results->rawbuf[offset++] * RAWTICK / COOLIX_HDR_MARK_TICKS;
  if (!matchSpace(results->rawbuf[offset], COOLIX_HDR_SPACE)) return false;
  // Calculate how long the common tick time is based on the header space.
  uint32_t s_tick = results->rawbuf[offset++] * RAWTICK /
      COOLIX_HDR_SPACE_TICKS;

  // Data
  // Twice as many bits as there are normal plus inverted bits.
  for (uint16_t i = 0; i < nbits * 2; i++, offset++) {
    bool flip = (i / 8) % 2;
    if (!matchMark(results->rawbuf[offset++], COOLIX_BIT_MARK_TICKS * m_tick))
      return false;
    if (matchSpace(results->rawbuf[offset], COOLIX_ONE_SPACE_TICKS * s_tick)) {
      if (flip)
        inverted = (inverted << 1) | 1;
      else
        data = (data << 1) | 1;
    } else if (matchSpace(results->rawbuf[offset],
                          COOLIX_ZERO_SPACE_TICKS * s_tick)) {
      if (flip)
        inverted <<= 1;
      else
        data <<= 1;
    } else {
      return false;
    }
  }

  // Footer
  if (!matchMark(results->rawbuf[offset++], COOLIX_BIT_MARK_TICKS * m_tick))
    return false;
  if (offset < results->rawlen &&
      !matchAtLeast(results->rawbuf[offset], COOLIX_MIN_GAP_TICKS * s_tick))
    return false;

  // Compliance
  uint64_t orig = data;  // Save a copy of the data.
  if (strict) {
    for (uint16_t i = 0; i < nbits; i += 8, data >>= 8, inverted >>= 8)
      if ((data & 0xFF) != ((inverted & 0xFF) ^ 0xFF))
        return false;
  }

  // Success
  results->decode_type = COOLIX;
  results->bits = nbits;
  results->value = orig;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif
