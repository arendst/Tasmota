// Copyright 2019 David Conran

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

// LEGO
// (LEGO is a Registrated Trademark of the Lego Group.)
//
// Supports:
//   Brand: LEGO Power Functions,  Model: IR Receiver
//
// Ref:
// - https://github.com/crankyoldgit/IRremoteESP8266/issues/641
// - https://github.com/crankyoldgit/IRremoteESP8266/files/2974525/LEGO_Power_Functions_RC_v120.pdf

// Constants
const uint16_t kLegoPfBitMark = 158;
const uint16_t kLegoPfHdrSpace = 1026;
const uint16_t kLegoPfZeroSpace = 263;
const uint16_t kLegoPfOneSpace = 553;
const uint32_t kLegoPfMinCommandLength = 16000;  // 16ms


#if SEND_LEGOPF
// Send a LEGO Power Functions message.
//
// Args:
//   data:   Contents of the message to be sent.
//   nbits:  Nr. of bits of data to be sent. Typically kLegoPfBits.
//   repeat: Nr. of additional times the message is to be sent.
//           Note: Non-zero repeats results in at least 5 messages per spec.
//
// Status: Beta / Should work.
void IRsend::sendLegoPf(const uint64_t data, const uint16_t nbits,
                        const uint16_t repeat) {
  uint8_t channelid = ((data >> (nbits - 4)) & 0b11) + 1;
  if (repeat) {
    // We are in repeat mode.
    // Spec says a pause before transmittion.
    if (channelid < 4) space((4 - channelid) * kLegoPfMinCommandLength);
    // Spec says there are a minimum of 5 message repeats.
    for (uint16_t r = 0; r < std::max(repeat, (uint16_t)5); r++) {
      // Lego has a special repeat mode which repeats a message with varying
      // start to start times.
      sendGeneric(kLegoPfBitMark, kLegoPfHdrSpace,
                  kLegoPfBitMark, kLegoPfOneSpace,
                  kLegoPfBitMark, kLegoPfZeroSpace,
                  kLegoPfBitMark, kLegoPfHdrSpace,
                  ((r < 2) ? 5 : (6 + 2 * channelid)) * kLegoPfMinCommandLength,
                  data, nbits, 38000, true, 0, kDutyDefault);
    }
  } else {  // No repeat, just a simple message.
    sendGeneric(kLegoPfBitMark, kLegoPfHdrSpace,
                kLegoPfBitMark, kLegoPfOneSpace,
                kLegoPfBitMark, kLegoPfZeroSpace,
                kLegoPfBitMark, kLegoPfHdrSpace,
                kLegoPfMinCommandLength * 5,
                data, nbits, 38000, true, 0, kDutyDefault);
  }
}
#endif  // SEND_LEGO

#if DECODE_LEGOPF
// Decode the supplied LEGO Power Functions message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   offset:  The starting index to use when attempting to decode the raw data.
//            Typically/Defaults to kStartOffset.
//   nbits:   The number of data bits to expect. Typically kLegoPfBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: STABLE / Appears to work.
bool IRrecv::decodeLegoPf(decode_results* results, uint16_t offset,
                          const uint16_t nbits, const bool strict) {
  // Check if can possibly be a valid LEGO message.
  if (strict && nbits != kLegoPfBits) return false;  // Not what is expected

  uint64_t data = 0;

  // Match Header + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, &data,
                    results->rawlen - offset, nbits,
                    kLegoPfBitMark, kLegoPfHdrSpace,
                    kLegoPfBitMark, kLegoPfOneSpace,
                    kLegoPfBitMark, kLegoPfZeroSpace,
                    kLegoPfBitMark, kLegoPfMinCommandLength,
                    true)) return false;
  // Compliance
  if (strict) {
    // Verify the Longitudinal Redundancy Check (LRC)
    uint16_t lrc_data = data;
    uint8_t lrc = 0xF;
    for (uint8_t i = 0; i < 4; i++) {
      lrc ^= (lrc_data & 0xF);
      lrc_data >>= 4;
    }
    if (lrc) return false;
  }

  // Success
  results->decode_type = LEGOPF;
  results->bits = nbits;
  results->value = data;
  results->address = ((data >> (nbits - 4)) & 0b11) + 1;  // Channel Id
  results->command = (data >> 4) & 0xFF;  // Stuff between Channel Id and LRC.
  return true;
}
#endif  // DECODE_LEGOPF
