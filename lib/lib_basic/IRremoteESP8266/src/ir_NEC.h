// Copyright 2009 Ken Shirriff
// Copyright 2017, 2018 David Conran

/// @file
/// @brief Support for NEC (Renesas) protocols.
/// NEC originally added from https://github.com/shirriff/Arduino-IRremote/
/// @see http://www.sbprojects.net/knowledge/ir/nec.php

// Supports:
//   Brand: Yamaha,   Model: RAV561 remote
//   Brand: Yamaha,   Model: RXV585B A/V Receiver
//   Brand: Aloka,    Model: SleepyLights LED Lamp
//   Brand: Toshiba,  Model: 42TL838 LCD TV
//   Brand: Duux,  Model: Blizzard Smart 10K / DXMA04 A/C
//   Brand: Duux,  Model: YJ-A081 TR Remote
//   Brand: Silan Microelectronics,  Model: SC6121-001 IC

#ifndef IR_NEC_H_
#define IR_NEC_H_

#include <stdint.h>
#include "IRremoteESP8266.h"

// Constants
const uint16_t kNecTick = 560;
const uint16_t kNecHdrMarkTicks = 16;
const uint16_t kNecHdrMark = kNecHdrMarkTicks * kNecTick;
const uint16_t kNecHdrSpaceTicks = 8;
const uint16_t kNecHdrSpace = kNecHdrSpaceTicks * kNecTick;
const uint16_t kNecBitMarkTicks = 1;
const uint16_t kNecBitMark = kNecBitMarkTicks * kNecTick;
const uint16_t kNecOneSpaceTicks = 3;
const uint16_t kNecOneSpace = kNecOneSpaceTicks * kNecTick;
const uint16_t kNecZeroSpaceTicks = 1;
const uint16_t kNecZeroSpace = kNecZeroSpaceTicks * kNecTick;
const uint16_t kNecRptSpaceTicks = 4;
const uint16_t kNecRptSpace = kNecRptSpaceTicks * kNecTick;
const uint16_t kNecRptLength = 4;
const uint16_t kNecMinCommandLengthTicks = 193;
const uint32_t kNecMinCommandLength = kNecMinCommandLengthTicks * kNecTick;
const uint32_t kNecMinGap =
    kNecMinCommandLength -
    (kNecHdrMark + kNecHdrSpace + kNECBits * (kNecBitMark + kNecOneSpace) +
     kNecBitMark);
const uint16_t kNecMinGapTicks =
    kNecMinCommandLengthTicks -
    (kNecHdrMarkTicks + kNecHdrSpaceTicks +
     kNECBits * (kNecBitMarkTicks + kNecOneSpaceTicks) + kNecBitMarkTicks);

// IR codes and structure for kids ALOKA SleepyLights LED Lamp.
// https://aloka-designs.com/
// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1004
//
// May be useful for someone wanting to control the lamp.
//
// The lamp is toggled On and Off with the same power button.
// The colour, when selected, is the brightest and there are 4 levels of
// brightness that decrease on each send of the colour. A fifth send of the
// colour resets to brightest again.
//
// Remote buttons defined left to right, top line to bottom line on the remote.
const uint32_t kAlokaPower =         0xFF609F;
const uint32_t kAlokaLedWhite =      0xFF906F;
const uint32_t kAlokaLedGreen =      0xFF9867;
const uint32_t kAlokaLedBlue =       0xFFD827;
const uint32_t kAlokaLedPinkRed =    0xFF8877;
const uint32_t kAlokaLedRed =        0xFFA857;
const uint32_t kAlokaLedLightGreen = 0xFFE817;
const uint32_t kAlokaLedMidBlue =    0xFF48B7;
const uint32_t kAlokaLedPink =       0xFF6897;
const uint32_t kAlokaLedOrange =     0xFFB24D;
const uint32_t kAlokaLedYellow =     0xFF00FF;
const uint32_t kAlokaNightFade =     0xFF50AF;
const uint32_t kAlokaNightTimer =    0xFF7887;
const uint32_t kAlokaLedRainbow =    0xFF708F;
// Didn't have a better description for it...
const uint32_t kAlokaLedTreeGrow =   0xFF58A7;
#endif  // IR_NEC_H_
