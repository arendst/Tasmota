// Copyright 2017 David Conran

#include <algorithm>
#include "IRsend.h"
#include "IRtimer.h"

//                PPPPPP                        tt
//                PP   PP rr rr   oooo  nn nnn  tt     oooo
//                PPPPPP  rrr  r oo  oo nnn  nn tttt  oo  oo
//                PP      rr     oo  oo nn   nn tt    oo  oo
//                PP      rr      oooo  nn   nn  tttt  oooo

// Constants
#define PRONTO_FREQ_FACTOR      0.241246
#define PRONTO_TYPE_OFFSET             0U
#define PRONTO_FREQ_OFFSET             1U
#define PRONTO_SEQ_1_LEN_OFFSET        2U
#define PRONTO_SEQ_2_LEN_OFFSET        3U
#define PRONTO_DATA_OFFSET             4U

#if SEND_PRONTO
// Send a Pronto Code formatted message.
//
// Args:
//   data: An array of uint16_t containing the pronto codes.
//   len: Nr. of entries in the data[] array.
//   repeat: Nr. of times to repeat the message.
//
// Status: ALPHA / Not tested in the real world.
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
//       sendPronto(prontoCode, 46, SONY_MIN_REPEAT);
//
// Ref:
//   http://www.etcwiki.org/wiki/Pronto_Infrared_Format
//   http://www.remotecentral.com/features/irdisp2.htm
void IRsend::sendPronto(uint16_t data[], uint16_t len, uint16_t repeat) {
  // Check we have enough data to work out what to send.
  if (len < PRONTO_MIN_LENGTH) return;

  // We only know how to deal with 'raw' pronto codes types. Reject all others.
  if (data[PRONTO_TYPE_OFFSET] != 0) return;

  // Pronto frequency is in Hz.
  uint16_t hz = (uint16_t) (1000000U / (data[PRONTO_FREQ_OFFSET] *
                                        PRONTO_FREQ_FACTOR));
  enableIROut(hz);

  // Grab the length of the two sequences.
  uint16_t seq_1_len = data[PRONTO_SEQ_1_LEN_OFFSET] * 2;
  uint16_t seq_2_len = data[PRONTO_SEQ_2_LEN_OFFSET] * 2;
  // Calculate where each sequence starts in the buffer.
  uint16_t seq_1_start = PRONTO_DATA_OFFSET;
  uint16_t seq_2_start = PRONTO_DATA_OFFSET + seq_1_len;

  uint32_t periodic_time = calcUSecPeriod(hz, false);

  // Normal (1st sequence) case.
  // Is there a first (normal) sequence to send?
  if (seq_1_len > 0) {
    // Check we have enough data to send the complete first sequence.
    if (seq_1_len + seq_1_start > len) return;
    // Send the contents of the 1st sequence.
    for (uint16_t i = seq_1_start; i < seq_1_start + seq_1_len; i += 2) {
      mark(data[i] * periodic_time);
      space(data[i + 1] * periodic_time);
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
        mark(data[i] * periodic_time);
        space(data[i + 1] * periodic_time);
      }
  }
}
#endif
