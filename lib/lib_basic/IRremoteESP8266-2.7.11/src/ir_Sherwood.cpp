// Copyright 2017 David Conran

/// @file
/// @brief Support for Sherwood protocols.

// Supports:
//   Brand: Sherwood,  Model: RC-138 remote
//   Brand: Sherwood,  Model: RD6505(B) Receiver

#include <algorithm>
#include "IRsend.h"

#if SEND_SHERWOOD
/// Send an IR command to a Sherwood device.
/// Status: STABLE / Known working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @note Sherwood remote codes appear to be NEC codes with a manditory repeat
///   code. i.e. repeat should be >= kSherwoodMinRepeat (1).
void IRsend::sendSherwood(uint64_t data, uint16_t nbits, uint16_t repeat) {
  sendNEC(data, nbits, std::max((uint16_t)kSherwoodMinRepeat, repeat));
}
#endif  // SEND_SHERWOOD
