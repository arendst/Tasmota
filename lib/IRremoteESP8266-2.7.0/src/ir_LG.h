// Copyright 2017 David Conran

// Supports:
//   Brand: LG,  Model: 6711A20083V remote
//   Brand: LG,  Model: AKB74395308 remote

#ifndef IR_LG_H_
#define IR_LG_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>

uint8_t calcLGChecksum(uint16_t data);

#endif  // IR_LG_H_
