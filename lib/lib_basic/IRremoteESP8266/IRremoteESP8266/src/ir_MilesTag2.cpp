// Copyright 2021 Victor Mukayev (vitos1k)
// Copyright 2021 David Conran (crankyoldgit)

/// @file
/// @brief Support for the MilesTag2 IR protocol for LaserTag gaming
/// @see http://hosting.cmalton.me.uk/chrism/lasertag/MT2Proto.pdf
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1360

// Supports:
//   Brand: Milestag2,  Model: Various

// TODO(vitos1k): This implementation would support only
// short SHOT packets(14bits) and MSGs = 24bits. Support
// for long MSGs > 24bits is TODO

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

// Constants
// Shot packets have this bit as `0`
const uint16_t kMilesTag2ShotMask = 1 << (kMilesTag2ShotBits - 1);
// Msg packets have this bit as `1`
const uint32_t kMilesTag2MsgMask = 1 << (kMilesTag2MsgBits - 1);
const uint8_t  kMilesTag2MsgTerminator = 0xE8;
const uint16_t kMilesTag2HdrMark = 2400;     /// uSeconds.
const uint16_t kMilesTag2Space = 600;        /// uSeconds.
const uint16_t kMilesTag2OneMark = 1200;     /// uSeconds.
const uint16_t kMilesTag2ZeroMark = 600;     /// uSeconds.
const uint16_t kMilesTag2RptLength = 32000;  /// uSeconds.
const uint16_t kMilesTag2StdFreq = 38000;    /// Hz.
const uint16_t kMilesTag2StdDuty = 25;       /// Percentage.

#if SEND_MILESTAG2
/// Send a MilesTag2 formatted Shot/Msg packet.
/// Status: ALPHA / Probably works but needs testing with a real device.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendMilestag2(const uint64_t data, const uint16_t nbits,
                      const uint16_t repeat) {
  sendGeneric(
    kMilesTag2HdrMark, kMilesTag2Space,  // Header
    kMilesTag2OneMark, kMilesTag2Space,  // 1 bit
    kMilesTag2ZeroMark, kMilesTag2Space,  // 0 bit
    0,  // No footer mark
    kMilesTag2RptLength, data, nbits, kMilesTag2StdFreq, true,  // MSB First
    repeat, kMilesTag2StdDuty);
}
#endif  // SEND_MILESTAG2

#if DECODE_MILESTAG2
/// Decode the supplied MilesTag2 message.
/// Status: ALPHA / Probably works but needs testing with a real device.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1360
bool IRrecv::decodeMilestag2(decode_results *results, uint16_t offset,
                        const uint16_t nbits, const bool strict) {
  uint64_t data = 0;
  // Header + Data + Optional Footer
  if (!matchGeneric(results->rawbuf + offset, &data,
                  results->rawlen - offset, nbits,
                  kMilesTag2HdrMark, kMilesTag2Space,
                  kMilesTag2OneMark, kMilesTag2Space,
                  kMilesTag2ZeroMark, kMilesTag2Space,
                  0, kMilesTag2RptLength, true)) return false;

  // Compliance
  if (strict) {
    switch (nbits) {
      case kMilesTag2ShotBits:
        // Is it a valid shot packet?
        if (data & kMilesTag2ShotMask) return false;
        break;
      case kMilesTag2MsgBits:
        // Is it a valid msg packet? i.e. Msg bit set & Terminator present.
        if (!(data & kMilesTag2MsgMask) ||
            ((data & 0xFF) != kMilesTag2MsgTerminator))
          return false;
        break;
      default:
        DPRINT("incorrect nbits:");
        DPRINTLN(nbits);
        return false;  // The request doesn't strictly match the protocol defn.
    }
  }

  // Success
  results->bits = nbits;
  results->value = data;
  results->decode_type = decode_type_t::MILESTAG2;
  switch (nbits) {
    case kMilesTag2ShotBits:
      results->command = data & 0x3F;  // Team & Damage
      results->address = data >> 6;  // Player ID.
      break;
    case kMilesTag2MsgBits:
      results->command = (data >> 8) & 0xFF;  // Message data
      results->address = (data >> 16) & 0x7F;  // Message ID
      break;
    default:
      results->command = 0;
      results->address = 0;
  }
  return true;
}
#endif  // DECODE_MILESTAG2
