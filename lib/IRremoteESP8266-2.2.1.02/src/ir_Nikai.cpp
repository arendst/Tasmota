// Copyright 2009 Ken Shirriff
// Copyright 2017 David Conran

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtimer.h"
#include "IRutils.h"

//               NN   NN IIIII KK  KK   AAA   IIIII
//               NNN  NN  III  KK KK   AAAAA   III
//               NN N NN  III  KKKK   AA   AA  III
//               NN  NNN  III  KK KK  AAAAAAA  III
//               NN   NN IIIII KK  KK AA   AA IIIII

// Constants
// Ref:
//   https://github.com/markszabo/IRremoteESP8266/issues/309
#define NIKAI_TICK             500U
#define NIKAI_HDR_MARK_TICKS     8U
#define NIKAI_HDR_MARK         (NIKAI_HDR_MARK_TICKS * NIKAI_TICK)
#define NIKAI_HDR_SPACE_TICKS    8U
#define NIKAI_HDR_SPACE        (NIKAI_HDR_SPACE_TICKS * NIKAI_TICK)
#define NIKAI_BIT_MARK_TICKS     1U
#define NIKAI_BIT_MARK         (NIKAI_BIT_MARK_TICKS * NIKAI_TICK)
#define NIKAI_ONE_SPACE_TICKS    2U
#define NIKAI_ONE_SPACE        (NIKAI_ONE_SPACE_TICKS * NIKAI_TICK)
#define NIKAI_ZERO_SPACE_TICKS   4U
#define NIKAI_ZERO_SPACE       (NIKAI_ZERO_SPACE_TICKS * NIKAI_TICK)
#define NIKAI_MIN_GAP_TICKS     17U
#define NIKAI_MIN_GAP          (NIKAI_MIN_GAP_TICKS * NIKAI_TICK)


#if SEND_NIKAI
// Send a Nikai TV formatted message.
//
// Args:
//   data:   The message to be sent.
//   nbits:  The bit size of the message being sent. typically NIKAI_BITS.
//   repeat: The number of times the message is to be repeated.
//
// Status: ALPHA / Untested.
//
// Ref: https://github.com/markszabo/IRremoteESP8266/issues/309
void IRsend::sendNikai(uint64_t data, uint16_t nbits, uint16_t repeat) {
  // Set 38kHz IR carrier frequency & a 1/3 (33%) duty cycle.
  enableIROut(38, 33);
  // We always send a message, even for repeat=0, hence '<= repeat'.
  for (uint16_t i=0; i <= repeat; i++) {
    // Header
    mark(NIKAI_HDR_MARK);
    space(NIKAI_HDR_SPACE);
    // Data
    sendData(NIKAI_BIT_MARK, NIKAI_ONE_SPACE, NIKAI_BIT_MARK,
             NIKAI_ZERO_SPACE, data, nbits, true);
    // Footer
    mark(NIKAI_BIT_MARK);
    space(NIKAI_MIN_GAP);
  }
}
#endif

#if DECODE_NIKAI
// Decode the supplied Nikai message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of bits to expect in the data portion.
//            Typically NIKAI_BITS.
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: ALPHA / Untested.
//
bool IRrecv::decodeNikai(decode_results *results, uint16_t nbits, bool strict) {
  if (results->rawlen < 2 * nbits + HEADER + FOOTER - 1)
    return false;  // Can't possibly be a valid Nikai message.
  if (strict && nbits != NIKAI_BITS)
    return false;  // We expect Nikai to be a certain sized message.

  uint64_t data = 0;
  uint16_t offset = OFFSET_START;

  // Header
  if (!matchMark(results->rawbuf[offset], NIKAI_HDR_MARK)) return false;
  // Calculate how long the common tick time is based on the header mark.
  uint32_t m_tick = results->rawbuf[offset++] * RAWTICK /
      NIKAI_HDR_MARK_TICKS;
  if (!matchSpace(results->rawbuf[offset], NIKAI_HDR_SPACE)) return false;
  // Calculate how long the common tick time is based on the header space.
  uint32_t s_tick = results->rawbuf[offset++] * RAWTICK /
      NIKAI_HDR_SPACE_TICKS;
  // Data
  match_result_t data_result = matchData(&(results->rawbuf[offset]), nbits,
                                         NIKAI_BIT_MARK_TICKS * m_tick,
                                         NIKAI_ONE_SPACE_TICKS * s_tick,
                                         NIKAI_BIT_MARK_TICKS * m_tick,
                                         NIKAI_ZERO_SPACE_TICKS * s_tick);
  if (data_result.success == false) return false;
  data = data_result.data;
  offset += data_result.used;
  // Footer
  if (!matchMark(results->rawbuf[offset++], NIKAI_BIT_MARK_TICKS * m_tick))
    return false;
  if (offset < results->rawlen &&
      !matchAtLeast(results->rawbuf[offset], NIKAI_MIN_GAP_TICKS * s_tick))
    return false;

  // Compliance

  // Success
  results->bits = nbits;
  results->value = data;
  results->decode_type = NIKAI;
  results->command = 0;
  results->address = 0;
  return true;
}
#endif
