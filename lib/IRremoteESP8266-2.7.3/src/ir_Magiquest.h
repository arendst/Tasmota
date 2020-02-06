// Copyright 2013 mpflaga
// Copyright 2015 kitlaan
// Copyright 2017 Jason kendall, David Conran

#ifndef IR_MAGIQUEST_H_
#define IR_MAGIQUEST_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include "IRremoteESP8266.h"
#include "IRsend.h"

// MagiQuest packet is both Wand ID and magnitude of swish and flick
union magiquest {
  uint64_t llword;
  uint8_t byte[8];
  //  uint16_t   word[4];
  uint32_t lword[2];
  struct {
    uint16_t magnitude;
    uint32_t wand_id;
    uint8_t padding;
    uint8_t scrap;
  } cmd;
};

const uint16_t kMagiQuestTotalUsec = 1150;
const uint8_t kMagiQuestZeroRatio = 30;  // usually <= ~25%
const uint8_t kMagiQuestOneRatio = 38;   // usually >= ~50%
const uint16_t kMagiQuestMarkZero = 280;
const uint16_t kMagiQuestSpaceZero = 850;
const uint16_t kMagiQuestMarkOne = 580;
const uint16_t kMagiQuestSpaceOne = 600;
const uint32_t kMagiQuestGap = kDefaultMessageGap;  // Just a guess.
#endif  // IR_MAGIQUEST_H_
