// Copyright 2017 David Conran

// Sherwood IR remote emulation

// Supports:
//   Brand: Sherwood,  Model: RC-138 remote
//   Brand: Sherwood,  Model: RD6505(B) Receiver

#include <algorithm>
#include "IRsend.h"

#if SEND_SHERWOOD
// Send an IR command to a Sherwood device.
//
// Args:
//   data:   The contents of the command you want to send.
//   nbits:  The bit size of the command being sent. (kSherwoodBits)
//   repeat: The nr. of times you want the command to be repeated. (Default: 1)
//
// Status: STABLE / Known working.
//
// Note:
//   Sherwood remote codes appear to be NEC codes with a manditory repeat code.
//   i.e. repeat should be >= kSherwoodMinRepeat (1).
void IRsend::sendSherwood(uint64_t data, uint16_t nbits, uint16_t repeat) {
  sendNEC(data, nbits, std::max((uint16_t)kSherwoodMinRepeat, repeat));
}
#endif
