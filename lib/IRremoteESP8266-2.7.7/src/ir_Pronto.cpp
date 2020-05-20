// Copyright 2017 David Conran

// Pronto code message generation

#include <algorithm>
#include "IRsend.h"

// Constants
const float kProntoFreqFactor = 0.241246;
const uint16_t kProntoTypeOffset = 0;
const uint16_t kProntoFreqOffset = 1;
const uint16_t kProntoSeq1LenOffset = 2;
const uint16_t kProntoSeq2LenOffset = 3;
const uint16_t kProntoDataOffset = 4;

#if SEND_PRONTO
// Send a Pronto Code formatted message.
//
// Args:
//   data: An array of uint16_t containing the pronto codes.
//   len: Nr. of entries in the data[] array.
//   repeat: Nr. of times to repeat the message.
//
// Status: STABLE / Known working.
//
// Note:
//   Pronto codes are typically represented in hexadecimal.
//   You will need to convert the code to an array of integers, and calculate
//   it's length.
//   e.g.
//      A Sony 20 bit DVD remote command.
//      "0000 0067 0000 0015 0060 0018 0018 0018 0030 0018 0030 0018 0030 0018
//       0018 0018 0030 0018 0018 0018 0018 0018 0030 0018 0018 0018 0030 0018
//       0030 0018 0030 0018 0018 0018 0018 0018 0030 0018 0018 0018 0018 0018
//       0030 0018 0018 03f6"
//
//   converts to:
//
//       uint16_t prontoCode[46] = {
//           0x0000, 0x0067, 0x0000, 0x0015,
//           0x0060, 0x0018, 0x0018, 0x0018, 0x0030, 0x0018, 0x0030, 0x0018,
//           0x0030, 0x0018, 0x0018, 0x0018, 0x0030, 0x0018, 0x0018, 0x0018,
//           0x0018, 0x0018, 0x0030, 0x0018, 0x0018, 0x0018, 0x0030, 0x0018,
//           0x0030, 0x0018, 0x0030, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018,
//           0x0030, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0030, 0x0018,
//           0x0018, 0x03f6};
//       // Send the Pronto(Sony) code. Repeat twice as Sony's require that.
//       sendPronto(prontoCode, 46, kSonyMinRepeat);
//
// Ref:
//   http://www.etcwiki.org/wiki/Pronto_Infrared_Format
//   http://www.remotecentral.com/features/irdisp2.htm
void IRsend::sendPronto(uint16_t data[], uint16_t len, uint16_t repeat) {
  // Check we have enough data to work out what to send.
  if (len < kProntoMinLength) return;

  // We only know how to deal with 'raw' pronto codes types. Reject all others.
  if (data[kProntoTypeOffset] != 0) return;

  // Pronto frequency is in Hz.
  uint16_t hz =
      (uint16_t)(1000000U / (data[kProntoFreqOffset] * kProntoFreqFactor));
  enableIROut(hz);

  // Grab the length of the two sequences.
  uint16_t seq_1_len = data[kProntoSeq1LenOffset] * 2;
  uint16_t seq_2_len = data[kProntoSeq2LenOffset] * 2;
  // Calculate where each sequence starts in the buffer.
  uint16_t seq_1_start = kProntoDataOffset;
  uint16_t seq_2_start = kProntoDataOffset + seq_1_len;

  uint32_t periodic_time_x10 = calcUSecPeriod(hz / 10, false);

  // Normal (1st sequence) case.
  // Is there a first (normal) sequence to send?
  if (seq_1_len > 0) {
    // Check we have enough data to send the complete first sequence.
    if (seq_1_len + seq_1_start > len) return;
    // Send the contents of the 1st sequence.
    for (uint16_t i = seq_1_start; i < seq_1_start + seq_1_len; i += 2) {
      mark((data[i] * periodic_time_x10) / 10);
      space((data[i + 1] * periodic_time_x10) / 10);
    }
  } else {
    // There was no first sequence to send, it is implied that we have to send
    // the 2nd/repeat sequence an additional time. i.e. At least once.
    repeat++;
  }

  // Repeat (2nd sequence) case.
  // Is there a second (repeat) sequence to be sent?
  if (seq_2_len > 0) {
    // Check we have enough data to send the complete second sequence.
    if (seq_2_len + seq_2_start > len) return;

    // Send the contents of the 2nd sequence.
    for (uint16_t r = 0; r < repeat; r++)
      for (uint16_t i = seq_2_start; i < seq_2_start + seq_2_len; i += 2) {
        mark((data[i] * periodic_time_x10) / 10);
        space((data[i + 1] * periodic_time_x10) / 10);
      }
  }
}
#endif
