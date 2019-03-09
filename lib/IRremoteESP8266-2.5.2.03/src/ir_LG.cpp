// Copyright 2015 Darryl Smith
// Copyright 2015 cheaplin
// Copyright 2017, 2018 David Conran

#include "ir_LG.h"
#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

//                               L       GGGG
//                               L      G
//                               L      G  GG
//                               L      G   G
//                               LLLLL   GGG

// LG decode originally added by Darryl Smith (based on the JVC protocol)
// LG send originally added by https://github.com/chaeplin
//
// Known supported devices:
//   IR Remotes:
//     6711A20083V
//     AKB74395308

// Constants
const uint16_t kLgTick = 50;
const uint16_t kLgHdrMarkTicks = 170;
const uint16_t kLgHdrMark = kLgHdrMarkTicks * kLgTick;  // 8500
const uint16_t kLgHdrSpaceTicks = 85;
const uint16_t kLgHdrSpace = kLgHdrSpaceTicks * kLgTick;  // 4250
const uint16_t kLgBitMarkTicks = 11;
const uint16_t kLgBitMark = kLgBitMarkTicks * kLgTick;  // 550
const uint16_t kLgOneSpaceTicks = 32;
const uint16_t kLgOneSpace = kLgOneSpaceTicks * kLgTick;  // 1600
const uint16_t kLgZeroSpaceTicks = 11;
const uint16_t kLgZeroSpace = kLgZeroSpaceTicks * kLgTick;  // 550
const uint16_t kLgRptSpaceTicks = 45;
const uint16_t kLgRptSpace = kLgRptSpaceTicks * kLgTick;  // 2250
const uint16_t kLgMinGapTicks = 795;
const uint16_t kLgMinGap = kLgMinGapTicks * kLgTick;  // 39750
const uint16_t kLgMinMessageLengthTicks = 2161;
const uint32_t kLgMinMessageLength = kLgMinMessageLengthTicks * kLgTick;

const uint16_t kLg32HdrMarkTicks = 90;
const uint16_t kLg32HdrMark = kLg32HdrMarkTicks * kLgTick;  // 4500
const uint16_t kLg32HdrSpaceTicks = 89;
const uint16_t kLg32HdrSpace = kLg32HdrSpaceTicks * kLgTick;  // 4450
const uint16_t kLg32RptHdrMarkTicks = 179;
const uint16_t kLg32RptHdrMark = kLg32RptHdrMarkTicks * kLgTick;  // 8950

const uint16_t kLg2HdrMarkTicks = 64;
const uint16_t kLg2HdrMark = kLg2HdrMarkTicks * kLgTick;  // 3200
const uint16_t kLg2HdrSpaceTicks = 197;
const uint16_t kLg2HdrSpace = kLg2HdrSpaceTicks * kLgTick;  // 9850
const uint16_t kLg2BitMarkTicks = 10;
const uint16_t kLg2BitMark = kLg2BitMarkTicks * kLgTick;  // 500

#if (SEND_LG || DECODE_LG)
// Calculate the rolling 4-bit wide checksum over all of the data.
//  Args:
//    data: The value to be checksum'ed.
//  Returns:
//    A 4-bit checksum.
uint8_t calcLGChecksum(uint16_t data) {
  return (((data >> 12) + ((data >> 8) & 0xF) + ((data >> 4) & 0xF) +
           (data & 0xF)) &
          0xF);
}
#endif

#if SEND_LG
// Send an LG formatted message.
//
// Args:
//   data:   The contents of the message you want to send.
//   nbits:  The bit size of the message being sent.
//           Typically kLgBits or kLg32Bits.
//   repeat: The number of times you want the message to be repeated.
//
// Status: Beta / Should be working.
//
// Notes:
//   LG has a separate message to indicate a repeat, like NEC does.
// Supports:
//   IR Remote models: 6711A20083V
void IRsend::sendLG(uint64_t data, uint16_t nbits, uint16_t repeat) {
  uint16_t repeatHeaderMark = 0;

  if (nbits >= kLg32Bits) {
    // LG 32bit protocol is near identical to Samsung except for repeats.
    sendSAMSUNG(data, nbits, 0);  // Send it as a single Samsung message.
    repeatHeaderMark = kLg32RptHdrMark;
    repeat++;
  } else {
    // LG (28-bit) protocol.
    repeatHeaderMark = kLgHdrMark;
    sendGeneric(kLgHdrMark, kLgHdrSpace, kLgBitMark, kLgOneSpace, kLgBitMark,
                kLgZeroSpace, kLgBitMark, kLgMinGap, kLgMinMessageLength, data,
                nbits, 38, true, 0,  // Repeats are handled later.
                50);
  }

  // Repeat
  // Protocol has a mandatory repeat-specific code sent after every command.
  if (repeat)
    sendGeneric(repeatHeaderMark, kLgRptSpace, 0, 0, 0, 0,  // No data is sent.
                kLgBitMark, kLgMinGap, kLgMinMessageLength, 0, 0,  // No data.
                38, true, repeat - 1, 50);
}

// Send an LG Variant-2 formatted message.
//
// Args:
//   data:   The contents of the message you want to send.
//   nbits:  The bit size of the message being sent.
//           Typically kLgBits or kLg32Bits.
//   repeat: The number of times you want the message to be repeated.
//
// Status: Beta / Should be working.
//
// Notes:
//   LG has a separate message to indicate a repeat, like NEC does.
// Supports:
//   IR Remote models: AKB74395308
void IRsend::sendLG2(uint64_t data, uint16_t nbits, uint16_t repeat) {
  if (nbits >= kLg32Bits) {
    // Let the original routine handle it.
    sendLG(data, nbits, repeat);  // Send it as a single Samsung message.
    return;
  }

  // LGv2 (28-bit) protocol.
  sendGeneric(kLg2HdrMark, kLg2HdrSpace, kLgBitMark, kLgOneSpace, kLgBitMark,
              kLgZeroSpace, kLgBitMark, kLgMinGap, kLgMinMessageLength, data,
              nbits, 38, true, 0,  // Repeats are handled later.
              50);

  // TODO(crackn): Verify the details of what repeat messages look like.
  // Repeat
  // Protocol has a mandatory repeat-specific code sent after every command.
  if (repeat)
    sendGeneric(kLg2HdrMark, kLgRptSpace, 0, 0, 0, 0,  // No data is sent.
                kLgBitMark, kLgMinGap, kLgMinMessageLength, 0, 0,  // No data.
                38, true, repeat - 1, 50);
}

// Construct a raw 28-bit LG message code from the supplied address & command.
//
// Args:
//   address: The address code.
//   command: The command code.
// Returns:
//   A raw 28-bit LG message code suitable for sendLG() etc.
//
// Status: BETA / Should work.
//
// Notes:
//   e.g. Sequence of bits = address + command + checksum.
uint32_t IRsend::encodeLG(uint16_t address, uint16_t command) {
  return ((address << 20) | (command << 4) | calcLGChecksum(command));
}
#endif

#if DECODE_LG
// Decode the supplied LG message.
// LG protocol has a repeat code which is 4 items long.
// Even though the protocol has 28/32 bits of data, only 24/28 bits are
// distinct.
// In transmission order, the 28/32 bits are constructed as follows:
//   8/12 bits of address + 16 bits of command + 4 bits of checksum.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of bits to expect in the data portion.
//            Typically kLgBits or kLg32Bits.
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: BETA / Should work.
//
// Note:
//   LG 32bit protocol appears near identical to the Samsung protocol.
//   They possibly differ on how they repeat and initial HDR mark.
//
// Supports:
//   IR Remote models: 6711A20083V, AKB74395308

// Ref:
//   https://funembedded.wordpress.com/2014/11/08/ir-remote-control-for-lg-conditioner-using-stm32f302-mcu-on-mbed-platform/
bool IRrecv::decodeLG(decode_results *results, uint16_t nbits, bool strict) {
  if (nbits >= kLg32Bits) {
    if (results->rawlen < 2 * nbits + 2 * (kHeader + kFooter) - 1)
      return false;  // Can't possibly be a valid LG32 message.
  } else {
    if (results->rawlen < 2 * nbits + kHeader + kFooter - 1)
      return false;  // Can't possibly be a valid LG message.
  }
  if (strict && nbits != kLgBits && nbits != kLg32Bits)
    return false;  // Doesn't comply with expected LG protocol.

  uint64_t data = 0;
  uint16_t offset = kStartOffset;
  bool isLg2 = false;

  // Header
  uint32_t m_tick;
  if (matchMark(results->rawbuf[offset], kLgHdrMark)) {
    m_tick = results->rawbuf[offset++] * kRawTick / kLgHdrMarkTicks;
  } else if (matchMark(results->rawbuf[offset], kLg2HdrMark)) {
    m_tick = results->rawbuf[offset++] * kRawTick / kLg2HdrMarkTicks;
    isLg2 = true;
  } else if (matchMark(results->rawbuf[offset], kLg32HdrMark)) {
    m_tick = results->rawbuf[offset++] * kRawTick / kLg32HdrMarkTicks;
  } else {
    return false;
  }
  uint32_t s_tick;
  if (isLg2) {
    if (matchSpace(results->rawbuf[offset], kLg2HdrSpace))
      s_tick = results->rawbuf[offset++] * kRawTick / kLg2HdrSpaceTicks;
    else
      return false;
  } else {
    if (matchSpace(results->rawbuf[offset], kLgHdrSpace))
      s_tick = results->rawbuf[offset++] * kRawTick / kLgHdrSpaceTicks;
    else if (matchSpace(results->rawbuf[offset], kLg2HdrSpace))
      s_tick = results->rawbuf[offset++] * kRawTick / kLg32HdrSpaceTicks;
    else
      return false;
  }

  // Set up the expected tick sizes based on variant.
  uint16_t bitmarkticks;
  if (isLg2) {
    bitmarkticks = kLg2BitMarkTicks;
  } else {
    bitmarkticks = kLgBitMarkTicks;
  }

  // Data
  match_result_t data_result =
      matchData(&(results->rawbuf[offset]), nbits, bitmarkticks * m_tick,
                kLgOneSpaceTicks * s_tick, bitmarkticks * m_tick,
                kLgZeroSpaceTicks * s_tick, kTolerance, 0);
  if (data_result.success == false) return false;
  data = data_result.data;
  offset += data_result.used;

  // Footer
  if (!matchMark(results->rawbuf[offset++], bitmarkticks * m_tick))
    return false;
  if (offset < results->rawlen &&
      !matchAtLeast(results->rawbuf[offset], kLgMinGapTicks * s_tick))
    return false;

  // Repeat
  if (nbits >= kLg32Bits) {
    // If we are expecting the LG 32-bit protocol, there is always
    // a repeat message. So, check for it.
    offset++;
    if (!matchMark(results->rawbuf[offset++], kLg32RptHdrMarkTicks * m_tick))
      return false;
    if (!matchSpace(results->rawbuf[offset++], kLgRptSpaceTicks * s_tick))
      return false;
    if (!matchMark(results->rawbuf[offset++], bitmarkticks * m_tick))
      return false;
    if (offset < results->rawlen &&
        !matchAtLeast(results->rawbuf[offset], kLgMinGapTicks * s_tick))
      return false;
  }

  // Compliance
  uint16_t command = (data >> 4) & 0xFFFF;  // The 16 bits before the checksum.

  if (strict && (data & 0xF) != calcLGChecksum(command))
    return false;  // The last 4 bits sent are the expected checksum.

  // Success
  if (isLg2)
    results->decode_type = LG2;
  else
    results->decode_type = LG;
  results->bits = nbits;
  results->value = data;
  results->command = command;
  results->address = data >> 20;  // The bits before the command.
  return true;
}
#endif
