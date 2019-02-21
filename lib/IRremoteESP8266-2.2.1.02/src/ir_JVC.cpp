// Copyright 2015 Kristian Lauszus
// Copyright 2017 David Conran

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtimer.h"
#include "IRutils.h"

//                             JJJJJ  V   V   CCCC
//                               J    V   V  C
//                               J     V V   C
//                             J J     V V   C
//                              J       V     CCCC

// JVC originally added by Kristian Lauszus
// (Thanks to zenwheel and other people at the original blog post)

// Constants
// Ref:
//   http://www.sbprojects.com/knowledge/ir/jvc.php
#define JVC_TICK              75U
#define JVC_HDR_MARK_TICKS   112U
#define JVC_HDR_MARK         (JVC_HDR_MARK_TICKS * JVC_TICK)
#define JVC_HDR_SPACE_TICKS   56U
#define JVC_HDR_SPACE        (JVC_HDR_SPACE_TICKS * JVC_TICK)
#define JVC_BIT_MARK_TICKS     7U
#define JVC_BIT_MARK         (JVC_BIT_MARK_TICKS * JVC_TICK)
#define JVC_ONE_SPACE_TICKS   23U
#define JVC_ONE_SPACE        (JVC_ONE_SPACE_TICKS * JVC_TICK)
#define JVC_ZERO_SPACE_TICKS  7U
#define JVC_ZERO_SPACE       (JVC_ZERO_SPACE_TICKS * JVC_TICK)
#define JVC_RPT_LENGTH_TICKS 800U
#define JVC_RPT_LENGTH       (JVC_RPT_LENGTH_TICKS * JVC_TICK)
#define JVC_MIN_GAP_TICKS    (JVC_RPT_LENGTH_TICKS - \
    (JVC_HDR_MARK_TICKS + JVC_HDR_SPACE_TICKS + \
     JVC_BITS * (JVC_BIT_MARK_TICKS + JVC_ONE_SPACE_TICKS) + \
     JVC_BIT_MARK_TICKS))
#define JVC_MIN_GAP          (JVC_MIN_GAP_TICKS * JVC_TICK)

#if SEND_JVC
// Send a JVC message.
//
// Args:
//   data:   The contents of the command you want to send.
//   nbits:  The bit size of the command being sent. (JVC_BITS)
//   repeat: The number of times you want the command to be repeated.
//
// Status: STABLE.
//
// Ref:
//   http://www.sbprojects.com/knowledge/ir/jvc.php
void IRsend::sendJVC(uint64_t data, uint16_t nbits, uint16_t repeat) {
  // Set 38kHz IR carrier frequency & a 1/3 (33%) duty cycle.
  enableIROut(38, 33);

  IRtimer usecs = IRtimer();
  // Header
  // Only sent for the first message.
  mark(JVC_HDR_MARK);
  space(JVC_HDR_SPACE);

  // We always send the data & footer at least once, hence '<= repeat'.
  for (uint16_t i = 0; i <= repeat; i++) {
    // Data
    sendData(JVC_BIT_MARK, JVC_ONE_SPACE, JVC_BIT_MARK, JVC_ZERO_SPACE,
             data, nbits, true);
    // Footer
    mark(JVC_BIT_MARK);
    // Wait till the end of the repeat time window before we send another code.
    space(std::max(JVC_MIN_GAP, JVC_RPT_LENGTH - usecs.elapsed()));
    usecs.reset();
  }
}

// Calculate the raw JVC data based on address and command.
//
// Args:
//   address: An 8-bit address value.
//   command: An 8-bit command value.
// Returns:
//   A raw JVC message.
//
// Status: BETA / Should work fine.
//
// Ref:
//   http://www.sbprojects.com/knowledge/ir/jvc.php
uint16_t IRsend::encodeJVC(uint8_t address, uint8_t command) {
  return reverseBits((command << 8) | address, 16);
}
#endif

#if DECODE_JVC
// Decode the supplied JVC message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of bits of data to expect. Typically JVC_BITS.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: STABLE
//
// Note:
//   JVC repeat codes don't have a header.
// Ref:
//   http://www.sbprojects.com/knowledge/ir/jvc.php
bool IRrecv::decodeJVC(decode_results *results, uint16_t nbits,  bool strict) {
  if (strict && nbits != JVC_BITS)
    return false;  // Must be called with the correct nr. of bits.
  if (results->rawlen < 2 * nbits + FOOTER - 1)
    return false;  // Can't possibly be a valid JVC message.

  uint64_t data = 0;
  uint16_t offset = OFFSET_START;
  bool isRepeat = true;

  uint32_t m_tick;
  uint32_t s_tick;
  // Header
  // (Optional as repeat codes don't have the header)
  if (matchMark(results->rawbuf[offset], JVC_HDR_MARK)) {
    isRepeat = false;
    m_tick = results->rawbuf[offset++] * RAWTICK / JVC_HDR_MARK_TICKS;
    if (results->rawlen < 2 * nbits + 4)
      return false;  // Can't possibly be a valid JVC message with a header.
    if (!matchSpace(results->rawbuf[offset], JVC_HDR_SPACE))
      return false;
    s_tick = results->rawbuf[offset++] * RAWTICK / JVC_HDR_SPACE_TICKS;
  } else {
    // We can't easily auto-calibrate as there is no header, so assume
    // the default tick time.
    m_tick = JVC_TICK;
    s_tick = JVC_TICK;
  }

  // Data
  match_result_t data_result = matchData(&(results->rawbuf[offset]), nbits,
                                         JVC_BIT_MARK_TICKS * m_tick,
                                         JVC_ONE_SPACE_TICKS * s_tick,
                                         JVC_BIT_MARK_TICKS * m_tick,
                                         JVC_ZERO_SPACE_TICKS * s_tick);
  if (data_result.success == false) return false;
  data = data_result.data;
  offset += data_result.used;

  // Footer
  if (!matchMark(results->rawbuf[offset++], JVC_BIT_MARK_TICKS * m_tick))
    return false;
  if (offset < results->rawlen &&
      !matchAtLeast(results->rawbuf[offset], JVC_MIN_GAP_TICKS * s_tick))
    return false;

  // Success
  results->decode_type = JVC;
  results->bits = nbits;
  results->value = data;
  // command & address are transmitted LSB first, so we need to reverse them.
  results->address = reverseBits(data >> 8, 8);  // The first 8 bits sent.
  results->command = reverseBits(data & 0xFF, 8);  // The last 8 bits sent.
  results->repeat = isRepeat;
  return true;
}
#endif
