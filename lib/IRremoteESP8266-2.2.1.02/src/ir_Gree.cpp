// Copyright 2017 Ville Skyttä (scop)
// Copyright 2017 David Conran
//
//  Gree protocol compatible heat pump carrying the "Ultimate" brand name.
//

#include "IRremoteESP8266.h"
#include "IRsend.h"

//                      GGGG  RRRRRR  EEEEEEE EEEEEEE
//                     GG  GG RR   RR EE      EE
//                    GG      RRRRRR  EEEEE   EEEEE
//                    GG   GG RR  RR  EE      EE
//                     GGGGGG RR   RR EEEEEEE EEEEEEE

// Constants
// Ref: https://github.com/ToniA/arduino-heatpumpir/blob/master/GreeHeatpumpIR.h
#define GREE_HDR_MARK      9000U
#define GREE_HDR_SPACE     4000U
#define GREE_BIT_MARK       620U
#define GREE_ONE_SPACE     1600U
#define GREE_ZERO_SPACE     540U
#define GREE_MSG_SPACE    19000U

#if SEND_GREE
// Send a Gree Heat Pump message.
//
// Args:
//   data: An array of bytes containing the IR command.
//   nbytes: Nr. of bytes of data in the array. (>=GREE_STATE_LENGTH)
//   repeat: Nr. of times the message is to be repeated. (Default = 0).
//
// Status: ALPHA / Untested.
//
// Ref:
//   https://github.com/ToniA/arduino-heatpumpir/blob/master/GreeHeatpumpIR.cpp
void IRsend::sendGree(unsigned char data[], uint16_t nbytes, uint16_t repeat) {
  if (nbytes < GREE_STATE_LENGTH)
    return;  // Not enough bytes to send a proper message.

  // Set IR carrier frequency
  enableIROut(38);

  for (uint16_t r = 0; r <= repeat; r++) {
    // Header #1
    mark(GREE_HDR_MARK);
    space(GREE_HDR_SPACE);

    // Data #1
    uint16_t i;
    for (i = 0; i < 4 && i < nbytes; i++)
      sendData(GREE_BIT_MARK, GREE_ONE_SPACE, GREE_BIT_MARK, GREE_ZERO_SPACE,
               data[i], 8, false);

    // Footer #1 (010)
    sendData(GREE_BIT_MARK, GREE_ONE_SPACE, GREE_BIT_MARK, GREE_ZERO_SPACE,
             0b010, 3);

    // Header #2
    mark(GREE_BIT_MARK);
    space(GREE_MSG_SPACE);

    // Data #2
    for (; i < nbytes; i++)
      sendData(GREE_BIT_MARK, GREE_ONE_SPACE, GREE_BIT_MARK, GREE_ZERO_SPACE,
               data[i], 8, false);

    // Footer #2
    mark(GREE_BIT_MARK);
    space(GREE_MSG_SPACE);
  }
}

// Send a Gree Heat Pump message.
//
// Args:
//   data: The raw message to be sent.
//   nbits: Nr. of bits of data in the message. (Default is GREE_BITS)
//   repeat: Nr. of times the message is to be repeated. (Default = 0).
//
// Status: ALPHA / Untested.
//
// Ref:
//   https://github.com/ToniA/arduino-heatpumpir/blob/master/GreeHeatpumpIR.cpp
void IRsend::sendGree(uint64_t data, uint16_t nbits, uint16_t repeat) {
  if (nbits != GREE_BITS)
    return;  // Wrong nr. of bits to send a proper message.
  // Set IR carrier frequency
  enableIROut(38);

  for (uint16_t r = 0; r <= repeat; r++) {
    // Header
    mark(GREE_HDR_MARK);
    space(GREE_HDR_SPACE);

    // Data
    for (int16_t i = 8; i <= nbits; i += 8) {
      sendData(GREE_BIT_MARK, GREE_ONE_SPACE, GREE_BIT_MARK, GREE_ZERO_SPACE,
               (data >> (nbits - i)) & 0xFF, 8, false);
      if (i == nbits / 2) {
        // Send the mid-message Footer.
        sendData(GREE_BIT_MARK, GREE_ONE_SPACE, GREE_BIT_MARK, GREE_ZERO_SPACE,
                 0b010, 3);
        mark(GREE_BIT_MARK);
        space(GREE_MSG_SPACE);
      }
    }
    // Footer
    mark(GREE_BIT_MARK);
    space(GREE_MSG_SPACE);
  }
}
#endif  // SEND_GREE
