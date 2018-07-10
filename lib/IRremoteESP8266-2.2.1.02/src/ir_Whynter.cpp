// Copyright 2009 Ken Shirriff
// Copyright 2017 David Conran

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtimer.h"
#include "IRutils.h"

//               W   W  H   H  Y   Y N   N TTTTT EEEEE  RRRRR
//               W   W  H   H   Y Y  NN  N   T   E      R   R
//               W W W  HHHHH    Y   N N N   T   EEE    RRRR
//               W W W  H   H    Y   N  NN   T   E      R  R
//                WWW   H   H    Y   N   N   T   EEEEE  R   R

// Whynter A/C ARC-110WD added by Francesco Meschia
// Whynter originally added from https://github.com/shirriff/Arduino-IRremote/

// Constants
#define WHYNTER_TICK                       50U
#define WHYNTER_HDR_MARK_TICKS             57U
#define WHYNTER_HDR_MARK                 (WHYNTER_HDR_MARK_TICKS * WHYNTER_TICK)
#define WHYNTER_HDR_SPACE_TICKS            57U
#define WHYNTER_HDR_SPACE                (WHYNTER_HDR_SPACE_TICKS * \
                                          WHYNTER_TICK)
#define WHYNTER_BIT_MARK_TICKS             15U
#define WHYNTER_BIT_MARK                 (WHYNTER_BIT_MARK_TICKS * WHYNTER_TICK)
#define WHYNTER_ONE_SPACE_TICKS            43U
#define WHYNTER_ONE_SPACE                (WHYNTER_ONE_SPACE_TICKS * \
                                          WHYNTER_TICK)
#define WHYNTER_ZERO_SPACE_TICKS           15U
#define WHYNTER_ZERO_SPACE               (WHYNTER_ZERO_SPACE_TICKS * \
                                          WHYNTER_TICK)
#define WHYNTER_MIN_COMMAND_LENGTH_TICKS 2160U  // Completely made up value.
#define WHYNTER_MIN_COMMAND_LENGTH       (WHYNTER_MIN_COMMAND_LENGTH_TICKS * \
                                          WHYNTER_TICK)
#define WHYNTER_MIN_GAP_TICKS            (WHYNTER_MIN_COMMAND_LENGTH_TICKS - \
    (2 * (WHYNTER_BIT_MARK_TICKS + WHYNTER_ZERO_SPACE_TICKS) + \
     WHYNTER_BITS * (WHYNTER_BIT_MARK_TICKS + WHYNTER_ONE_SPACE_TICKS)))
#define WHYNTER_MIN_GAP                  (WHYNTER_MIN_GAP_TICKS * WHYNTER_TICK)

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
  IRtimer usecTimer = IRtimer();

  for (uint16_t i = 0; i <= repeat; i++) {
    usecTimer.reset();
    // Header
    mark(WHYNTER_BIT_MARK);
    space(WHYNTER_ZERO_SPACE);
    mark(WHYNTER_HDR_MARK);
    space(WHYNTER_HDR_SPACE);
    // Data
    sendData(WHYNTER_BIT_MARK, WHYNTER_ONE_SPACE, WHYNTER_BIT_MARK,
             WHYNTER_ZERO_SPACE, data, nbits, true);
    // Footer
    mark(WHYNTER_BIT_MARK);
    space(std::max(WHYNTER_MIN_COMMAND_LENGTH - usecTimer.elapsed(),
                   WHYNTER_MIN_GAP));
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
  if (results->rawlen < 2 * nbits + 2 * HEADER + FOOTER - 1)
     return false;  // We don't have enough entries to possibly match.

  // Compliance
  if (strict && nbits != WHYNTER_BITS)
    return false;  // Incorrect nr. of bits per spec.

  uint16_t offset = OFFSET_START;

  // Header
  // Sequence begins with a bit mark and a zero space.
  // These are typically small, so we'll prefer to do the calibration
  // on the much larger header mark & space that are next.
  if (!matchMark(results->rawbuf[offset++], WHYNTER_BIT_MARK)) return false;
  if (!matchSpace(results->rawbuf[offset++], WHYNTER_ZERO_SPACE)) return false;
  // Main header mark and space
  if (!matchMark(results->rawbuf[offset], WHYNTER_HDR_MARK)) return false;
  // Calculate how long the common tick time is based on the header mark.
  uint32_t m_tick = results->rawbuf[offset++] * RAWTICK /
      WHYNTER_HDR_MARK_TICKS;
  if (!matchSpace(results->rawbuf[offset], WHYNTER_HDR_SPACE)) return false;
  // Calculate how long the common tick time is based on the header space.
  uint32_t s_tick = results->rawbuf[offset++] * RAWTICK /
      WHYNTER_HDR_SPACE_TICKS;

  // Data
  uint64_t data = 0;
  match_result_t data_result = matchData(&(results->rawbuf[offset]), nbits,
                                         WHYNTER_BIT_MARK_TICKS * m_tick,
                                         WHYNTER_ONE_SPACE_TICKS * s_tick,
                                         WHYNTER_BIT_MARK_TICKS * m_tick,
                                         WHYNTER_ZERO_SPACE_TICKS * s_tick);
  if (data_result.success == false) return false;
  data = data_result.data;
  offset += data_result.used;

  // Footer
  if (!matchMark(results->rawbuf[offset++], WHYNTER_BIT_MARK_TICKS * m_tick))
    return false;
  if (offset < results->rawlen &&
      !matchAtLeast(results->rawbuf[offset], WHYNTER_MIN_GAP_TICKS * s_tick))
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
