// Copyright 2016 sillyfrog
// Copyright 2017 sillyfrog, crankyoldgit
// Copyright 2018-2020 crankyoldgit
// Copyright 2019 pasna (IRDaikin160 class / Daikin176 class)

/// @file
/// @brief Support for Daikin A/C protocols.
/// @see Daikin http://harizanov.com/2012/02/control-daikin-air-conditioner-over-the-internet/
/// @see Daikin https://github.com/mharizanov/Daikin-AC-remote-control-over-the-Internet/tree/master/IRremote
/// @see Daikin http://rdlab.cdmt.vn/project-2013/daikin-ir-protocol
/// @see Daikin https://github.com/blafois/Daikin-IR-Reverse
/// @see Daikin128 https://github.com/crankyoldgit/IRremoteESP8266/issues/827
/// @see Daikin152 https://github.com/crankyoldgit/IRremoteESP8266/issues/873
/// @see Daikin152 https://github.com/ToniA/arduino-heatpumpir/blob/master/DaikinHeatpumpARC480A14IR.cpp
/// @see Daikin152 https://github.com/ToniA/arduino-heatpumpir/blob/master/DaikinHeatpumpARC480A14IR.h
/// @see Daikin160 https://github.com/crankyoldgit/IRremoteESP8266/issues/731
/// @see Daikin2 https://docs.google.com/spreadsheets/d/1f8EGfIbBUo2B-CzUFdrgKQprWakoYNKM80IKZN4KXQE/edit#gid=236366525&range=B25:D32
/// @see Daikin2 https://github.com/crankyoldgit/IRremoteESP8266/issues/582
/// @see Daikin2 https://www.daikin.co.nz/sites/default/files/daikin-split-system-US7-FTXZ25-50NV1B.pdf
/// @see Daikin216 https://github.com/crankyoldgit/IRremoteESP8266/issues/689
/// @see Daikin216 https://github.com/danny-source/Arduino_DY_IRDaikin
/// @see Daikin64 https://github.com/crankyoldgit/IRremoteESP8266/issues/1064

// Supports:
//   Brand: Daikin,  Model: ARC433** remote (DAIKIN)
//   Brand: Daikin,  Model: ARC477A1 remote (DAIKIN2)
//   Brand: Daikin,  Model: FTXZ25NV1B A/C (DAIKIN2)
//   Brand: Daikin,  Model: FTXZ35NV1B A/C (DAIKIN2)
//   Brand: Daikin,  Model: FTXZ50NV1B A/C (DAIKIN2)
//   Brand: Daikin,  Model: ARC433B69 remote (DAIKIN216)
//   Brand: Daikin,  Model: ARC423A5 remote (DAIKIN160)
//   Brand: Daikin,  Model: FTE12HV2S A/C
//   Brand: Daikin,  Model: BRC4C153 remote (DAIKIN176)
//   Brand: Daikin,  Model: 17 Series A/C (DAIKIN128)
//   Brand: Daikin,  Model: FTXB12AXVJU A/C (DAIKIN128)
//   Brand: Daikin,  Model: FTXB09AXVJU A/C (DAIKIN128)
//   Brand: Daikin,  Model: BRC52B63 remote (DAIKIN128)
//   Brand: Daikin,  Model: ARC480A5 remote (DAIKIN152)
//   Brand: Daikin,  Model: FFN-C/FCN-F Series A/C (DAIKIN64)
//   Brand: Daikin,  Model: DGS01 remote (DAIKIN64)
//   Brand: Daikin,  Model: M Series A/C (DAIKIN)
//   Brand: Daikin,  Model: FTXM-M A/C (DAIKIN)
//   Brand: Daikin,  Model: ARC466A33 remote (DAIKIN)

#ifndef IR_DAIKIN_H_
#define IR_DAIKIN_H_

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include "IRrecv.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#ifdef UNIT_TEST
#include "IRsend_test.h"
#endif

/*
        Daikin AC map (i.e. DAIKIN, not the other variants)
        byte 6=
          b4:Comfort
        byte 7= checksum of the first part (and last byte before a 29ms pause)
        byte 13=Current time, mins past midnight, low bits
        byte 14
                b5-b3=Day of the week (SUN=1, MON=2, ..., SAT=7)
                b2-b0=Current time, mins past midnight, high bits
        byte 15= checksum of the second part (and last byte before a 29ms pause)
        byte 21=mode
                b7 = 0
                b6+b5+b4 = Mode
                        Modes: b6+b5+b4
                        011 = Cool
                        100 = Heat (temp 23)
                        110 = FAN (temp not shown, but 25)
                        000 = Fully Automatic (temp 25)
                        010 = DRY (temp 0xc0 = 96 degrees c)
                b3 = 1
                b2 = OFF timer set
                b1 = ON timer set
                b0 = Air Conditioner ON
        byte 22=temp*2   (Temp should be between 10 - 32)
        byte 24=Fan
                FAN control
                b7+b6+b5+b4 = Fan speed
                        Fan: b7+b6+b5+b4
                        0×3 = 1 bar
                        0×4 = 2 bar
                        0×5 = 3 bar
                        0×6 = 4 bar
                        0×7 = 5 bar
                        0xa = Auto
                        0xb = Quite
                b3+b2+b1+b0 = Swing control up/down
                        Swing control up/down:
                        0000 = Swing up/down off
                        1111 = Swing up/down on
        byte 25
                        Swing control left/right:
                        0000 = Swing left/right off
                        1111 = Swing left/right on
        byte 26=On timer mins past midnight, low bits
        byte 27
        b0-b3=On timer mins past midnight, high bits
        b4-b7=Off timer mins past midnight, low bits
        byte 28=Off timer mins past midnight, high bits
        byte 29=Aux  -> Powerful (bit 1), Silent (bit 5)
        byte 32=Aux2
        b1: Sensor
        b2: Econo mode
        b7: Intelligent eye on
        byte 33=Aux3
        b1: Mold Proof
        byte 34= checksum of the third part
*/

// Constants
const uint8_t kDaikinAuto = 0b000;
const uint8_t kDaikinDry =  0b010;
const uint8_t kDaikinCool = 0b011;
const uint8_t kDaikinHeat = 0b100;
const uint8_t kDaikinFan =  0b110;
const uint8_t kDaikinModeOffset = 4;
const uint8_t kDaikinModeSize = 3;
const uint8_t kDaikinMinTemp = 10;  // Celsius
const uint8_t kDaikinMaxTemp = 32;  // Celsius
const uint8_t kDaikinFanMin = 1;
const uint8_t kDaikinFanMed = 3;
const uint8_t kDaikinFanMax = 5;
const uint8_t kDaikinFanAuto = 0b1010;  // 10 / 0xA
const uint8_t kDaikinFanQuiet = 0b1011;  // 11 / 0xB
const uint8_t kDaikinFanOffset = 4;
const uint8_t kDaikinFanSize = 4;
const uint8_t kDaikinSwingOffset = 0;
const uint8_t kDaikinSwingSize = 4;
const uint8_t kDaikinSwingOn =  0b1111;
const uint8_t kDaikinSwingOff = 0b0000;
const uint16_t kDaikinHeaderLength = 5;
const uint8_t kDaikinSections = 3;
const uint8_t kDaikinSection1Length = 8;
const uint8_t kDaikinSection2Length = 8;
const uint8_t kDaikinSection3Length =
    kDaikinStateLength - kDaikinSection1Length - kDaikinSection2Length;
const uint8_t kDaikinByteComfort = 6;
const uint8_t kDaikinByteChecksum1 = 7;
const uint8_t kDaikinBitComfortOffset = 4;
const uint8_t kDaikinBitComfort = 1 << kDaikinBitComfortOffset;
const uint8_t kDaikinByteClockMinsLow = 13;
const uint8_t kDaikinByteClockMinsHigh = 14;
const uint8_t kDaikinClockMinsHighOffset = 0;
const uint8_t kDaikinClockMinsHighSize = 3;
const uint8_t kDaikinDoWOffset = 3;
const uint8_t kDaikinDoWSize = 3;
const uint8_t kDaikinByteChecksum2 = 15;
const uint8_t kDaikinBytePower = 21;
const uint8_t kDaikinBitPowerOffset = 0;
const uint8_t kDaikinBitPower = 1 << kDaikinBitPowerOffset;
const uint8_t kDaikinTempOffset = 1;
const uint8_t kDaikinTempSize = 6;
const uint8_t kDaikinByteTemp = 22;
const uint8_t kDaikinByteFan = 24;
const uint8_t kDaikinByteSwingH = 25;
const uint8_t kDaikinByteOnTimerMinsLow = 26;
const uint8_t kDaikinByteOnTimerMinsHigh = 27;
const uint8_t kDaikinOnTimerMinsHighOffset = 0;
const uint8_t kDaikinOnTimerMinsHighSize = 4;
const uint8_t kDaikinByteOffTimerMinsLow = kDaikinByteOnTimerMinsHigh;
const uint8_t kDaikinByteOffTimerMinsHigh = 28;
const uint8_t kDaikinBytePowerful = 29;
const uint8_t kDaikinBitPowerfulOffset = 0;
const uint8_t kDaikinBitPowerful = 1 << kDaikinBitPowerfulOffset;
const uint8_t kDaikinByteSilent = kDaikinBytePowerful;
const uint8_t kDaikinBitSilentOffset = 5;
const uint8_t kDaikinBitSilent = 1 << kDaikinBitSilentOffset;
const uint8_t kDaikinByteSensor = 32;
const uint8_t kDaikinBitSensorOffset = 1;
const uint8_t kDaikinBitSensor = 1 << kDaikinBitSensorOffset;
const uint8_t kDaikinByteEcono = kDaikinByteSensor;
const uint8_t kDaikinBitEconoOffset = 2;
const uint8_t kDaikinBitEcono = 1 << kDaikinBitEconoOffset;
const uint8_t kDaikinByteEye = kDaikinByteSensor;
const uint8_t kDaikinBitEye = 0b10000000;
const uint8_t kDaikinByteWeeklyTimer = kDaikinByteSensor;
const uint8_t kDaikinBitWeeklyTimerOffset = 7;
const uint8_t kDaikinBitWeeklyTimer = 1 << kDaikinBitWeeklyTimerOffset;
const uint8_t kDaikinByteMold = 33;
const uint8_t kDaikinBitMoldOffset = 1;
const uint8_t kDaikinBitMold = 1 << kDaikinBitMoldOffset;
const uint8_t kDaikinByteOffTimer = kDaikinBytePower;
const uint8_t kDaikinBitOffTimerOffset = 2;
const uint8_t kDaikinBitOffTimer = 1 << kDaikinBitOffTimerOffset;
const uint8_t kDaikinByteOnTimer = kDaikinByteOffTimer;
const uint8_t kDaikinBitOnTimerOffset = 1;
const uint8_t kDaikinBitOnTimer = 1 << kDaikinBitOnTimerOffset;
const uint8_t kDaikinByteChecksum3 = kDaikinStateLength - 1;
const uint16_t kDaikinUnusedTime = 0x600;
const uint8_t kDaikinBeepQuiet = 1;
const uint8_t kDaikinBeepLoud = 2;
const uint8_t kDaikinBeepOff = 3;
const uint8_t kDaikinLightBright = 1;
const uint8_t kDaikinLightDim = 2;
const uint8_t kDaikinLightOff = 3;
const uint8_t kDaikinCurBit = kDaikinStateLength;
const uint8_t kDaikinCurIndex = kDaikinStateLength + 1;
const uint8_t kDaikinTolerance = 35;
const uint16_t kDaikinMarkExcess = kMarkExcess;
const uint16_t kDaikinHdrMark = 3650;   // kDaikinBitMark * 8
const uint16_t kDaikinHdrSpace = 1623;  // kDaikinBitMark * 4
const uint16_t kDaikinBitMark = 428;
const uint16_t kDaikinZeroSpace = 428;
const uint16_t kDaikinOneSpace = 1280;
const uint16_t kDaikinGap = 29000;
// Note bits in each octet swapped so can be sent as a single value
const uint64_t kDaikinFirstHeader64 =
    0b1101011100000000000000001100010100000000001001111101101000010001;


const uint16_t kDaikin2Freq = 36700;  // Modulation Frequency in Hz.
const uint16_t kDaikin2LeaderMark = 10024;
const uint16_t kDaikin2LeaderSpace = 25180;
const uint16_t kDaikin2Gap = kDaikin2LeaderMark + kDaikin2LeaderSpace;
const uint16_t kDaikin2HdrMark = 3500;
const uint16_t kDaikin2HdrSpace = 1728;
const uint16_t kDaikin2BitMark = 460;
const uint16_t kDaikin2OneSpace = 1270;
const uint16_t kDaikin2ZeroSpace = 420;
const uint16_t kDaikin2Sections = 2;
const uint16_t kDaikin2Section1Length = 20;
const uint16_t kDaikin2Section2Length = 19;
const uint8_t kDaikin2Tolerance = 5;  // Extra percentage tolerance
const uint8_t kDaikin2BitSleepTimerOffset = 5;
const uint8_t kDaikin2BitSleepTimer = 1 << kDaikin2BitSleepTimerOffset;
const uint8_t kDaikin2BitPurifyOffset = 4;
const uint8_t kDaikin2BitPurify = 1 << kDaikin2BitPurifyOffset;  // 0b00010000
const uint8_t kDaikin2BitEyeOffset = 1;
const uint8_t kDaikin2BitEye = 1 << kDaikin2BitEyeOffset;  // 0b00000010
const uint8_t kDaikin2BitEyeAutoOffset = 7;
const uint8_t kDaikin2BitEyeAuto = 1 << kDaikin2BitEyeAutoOffset;  // 0b10000000
const uint8_t kDaikin2BitMoldOffset = 3;
const uint8_t kDaikin2BitMold = 1 << kDaikin2BitMoldOffset;    // 0b00001000
const uint8_t kDaikin2BitCleanOffset = 5;  // Byte[8]
const uint8_t kDaikin2BitClean = 1 << kDaikin2BitCleanOffset;  // 0b00100000
const uint8_t kDaikin2BitFreshAirOffset = 0;
const uint8_t kDaikin2BitFreshAir = 1 << kDaikin2BitFreshAirOffset;
const uint8_t kDaikin2BitFreshAirHighOffset = 7;
const uint8_t kDaikin2BitFreshAirHigh = 1 << kDaikin2BitFreshAirHighOffset;
const uint8_t kDaikin2BitPowerOffset = 7;
const uint8_t kDaikin2BitPower = 1 << kDaikin2BitPowerOffset;  // 0b10000000
// const uint8_t kDaikin2LightMask =    0b00110000;  // Byte[7]
const uint8_t kDaikin2LightOffset = 4;  // Byte[7]
const uint8_t kDaikin2LightSize = 2;
// const uint8_t kDaikin2BeepMask =     0b11000000;  // Byte[7]
const uint8_t kDaikin2BeepOffset = 6;  // Byte[7]
const uint8_t kDaikin2BeepSize = 2;
const uint8_t kDaikin2SwingVHigh = 0x1;
const uint8_t kDaikin2SwingVLow = 0x6;
const uint8_t kDaikin2SwingVSwing = 0xF;
const uint8_t kDaikin2SwingVAuto = 0xE;
const uint8_t kDaikin2SwingVBreeze = 0xC;
const uint8_t kDaikin2SwingVCirculate = 0xD;
const uint8_t kDaikin2FanByte = 28;

const uint8_t kDaikin2SwingHWide =     0xA3;
const uint8_t kDaikin2SwingHLeftMax =  0xA8;
const uint8_t kDaikin2SwingHLeft =     0xA9;
const uint8_t kDaikin2SwingHMiddle =   0xAA;
const uint8_t kDaikin2SwingHRight =    0xAB;
const uint8_t kDaikin2SwingHRightMax = 0xAC;
const uint8_t kDaikin2SwingHAuto =     0xBE;
const uint8_t kDaikin2SwingHSwing =    0xBF;

const uint8_t kDaikin2MinCoolTemp = 18;  // Min temp (in C) when in Cool mode.


const uint16_t kDaikin216Freq = 38000;  // Modulation Frequency in Hz.
const uint16_t kDaikin216HdrMark = 3440;
const uint16_t kDaikin216HdrSpace = 1750;
const uint16_t kDaikin216BitMark = 420;
const uint16_t kDaikin216OneSpace = 1300;
const uint16_t kDaikin216ZeroSpace = 450;
const uint16_t kDaikin216Gap = 29650;
const uint16_t kDaikin216Sections = 2;
const uint16_t kDaikin216Section1Length = 8;
const uint16_t kDaikin216Section2Length = kDaikin216StateLength -
                                          kDaikin216Section1Length;
const uint8_t kDaikin216BytePower = 13;
const uint8_t kDaikin216ByteMode = kDaikin216BytePower;
// const uint8_t kDaikin216MaskMode = 0b01110000;
const uint8_t kDaikin216ByteTemp = 14;
// const uint8_t kDaikin216MaskTemp = 0b01111110;
const uint8_t kDaikin216TempOffset = 1;
const uint8_t kDaikin216TempSize = 6;

const uint8_t kDaikin216ByteFan = 16;
const uint8_t kDaikin216MaskFan = 0b11110000;
const uint8_t kDaikin216ByteSwingV = 16;
// const uint8_t kDaikin216MaskSwingV = 0b00001111;
const uint8_t kDaikin216SwingSize = 4;
const uint8_t kDaikin216SwingOn = 0b1111;
const uint8_t kDaikin216SwingOff = 0b0000;
const uint8_t kDaikin216ByteSwingH = 17;
const uint8_t kDaikin216BytePowerful = 21;


const uint16_t kDaikin160Freq = 38000;  // Modulation Frequency in Hz.
const uint16_t kDaikin160HdrMark = 5000;
const uint16_t kDaikin160HdrSpace = 2145;
const uint16_t kDaikin160BitMark = 342;
const uint16_t kDaikin160OneSpace = 1786;
const uint16_t kDaikin160ZeroSpace = 700;
const uint16_t kDaikin160Gap = 29650;
const uint16_t kDaikin160Sections = 2;
const uint16_t kDaikin160Section1Length = 7;
const uint16_t kDaikin160Section2Length = kDaikin160StateLength -
                                          kDaikin160Section1Length;
const uint8_t kDaikin160BytePower = 12;
const uint8_t kDaikin160ByteMode = kDaikin160BytePower;
// const uint8_t kDaikin160MaskMode = 0b01110000;
const uint8_t kDaikin160ByteTemp = 16;
// const uint8_t kDaikin160MaskTemp = 0b01111110;
const uint8_t kDaikin160TempOffset = 1;
const uint8_t kDaikin160TempSize = 6;
const uint8_t kDaikin160ByteFan = 17;
const uint8_t kDaikin160MaskFan = 0b00001111;
const uint8_t kDaikin160ByteSwingV = 13;
const uint8_t kDaikin160MaskSwingV = 0b11110000;
const uint8_t kDaikin160SwingVLowest =  0x1;
const uint8_t kDaikin160SwingVLow =     0x2;
const uint8_t kDaikin160SwingVMiddle =  0x3;
const uint8_t kDaikin160SwingVHigh =    0x4;
const uint8_t kDaikin160SwingVHighest = 0x5;
const uint8_t kDaikin160SwingVAuto =    0xF;


const uint16_t kDaikin176Freq = 38000;  // Modulation Frequency in Hz.
const uint16_t kDaikin176HdrMark = 5070;
const uint16_t kDaikin176HdrSpace = 2140;
const uint16_t kDaikin176BitMark = 370;
const uint16_t kDaikin176OneSpace = 1780;
const uint16_t kDaikin176ZeroSpace = 710;
const uint16_t kDaikin176Gap = 29410;
const uint16_t kDaikin176Sections = 2;
const uint16_t kDaikin176Section1Length = 7;
const uint16_t kDaikin176Section2Length = kDaikin176StateLength -
                                          kDaikin176Section1Length;
const uint8_t kDaikin176Cool = 0b111;  // 7
const uint8_t kDaikin176BytePower = 14;
const uint8_t kDaikin176ByteMode = 12;
const uint8_t kDaikin176MaskMode = 0b01110000;
const uint8_t kDaikin176ByteModeButton = 13;
const uint8_t kDaikin176ModeButton = 0b00000100;
const uint8_t kDaikin176ByteTemp = 17;
// const uint8_t kDaikin176MaskTemp = 0b01111110;
const uint8_t kDaikin176TempOffset = 1;
const uint8_t kDaikin176TempSize = 6;
const uint8_t kDaikin176DryFanTemp = 17;  // Dry/Fan mode is always 17 Celsius.
const uint8_t kDaikin176ByteFan = 18;
const uint8_t kDaikin176MaskFan = 0b11110000;
const uint8_t kDaikin176FanMax = 3;
const uint8_t kDaikin176ByteSwingH = 18;
// const uint8_t kDaikin176MaskSwingH = 0b00001111;
const uint8_t kDaikin176SwingHAuto =  0x5;
const uint8_t kDaikin176SwingHOff = 0x6;


const uint16_t kDaikin128Freq = 38000;  // Modulation Frequency in Hz.
const uint16_t kDaikin128LeaderMark = 9800;
const uint16_t kDaikin128LeaderSpace = 9800;
const uint16_t kDaikin128HdrMark = 4600;
const uint16_t kDaikin128HdrSpace = 2500;
const uint16_t kDaikin128BitMark = 350;
const uint16_t kDaikin128OneSpace = 954;
const uint16_t kDaikin128ZeroSpace = 382;
const uint16_t kDaikin128Gap = 20300;
const uint16_t kDaikin128FooterMark = kDaikin128HdrMark;
const uint16_t kDaikin128Sections = 2;
const uint16_t kDaikin128SectionLength = 8;
const uint8_t kDaikin128ByteModeFan = 1;
// const uint8_t kDaikin128MaskMode =     0b00001111;
const uint8_t kDaikin128ModeSize = 4;
const uint8_t kDaikin128Dry =             0b00000001;
const uint8_t kDaikin128Cool =            0b00000010;
const uint8_t kDaikin128Fan =             0b00000100;
const uint8_t kDaikin128Heat =            0b00001000;
const uint8_t kDaikin128Auto =            0b00001010;
const uint8_t kDaikin128MaskFan =         0b11110000;
const uint8_t kDaikin128FanAuto =         0b0001;
const uint8_t kDaikin128FanHigh =         0b0010;
const uint8_t kDaikin128FanMed =          0b0100;
const uint8_t kDaikin128FanLow =          0b1000;
const uint8_t kDaikin128FanPowerful =     0b0011;
const uint8_t kDaikin128FanQuiet =        0b1001;
const uint8_t kDaikin128ByteClockMins = 2;
const uint8_t kDaikin128ByteClockHours = 3;
const uint8_t kDaikin128ByteOnTimer = 4;
const uint8_t kDaikin128ByteOffTimer = 5;
const uint8_t kDaikin128BitTimerEnabledOffset = 7;
const uint8_t kDaikin128BitTimerEnabled = 1 << kDaikin128BitTimerEnabledOffset;
const uint8_t kDaikin128TimerOffset = 0;
const uint8_t kDaikin128TimerSize = 7;
const uint8_t kDaikin128HalfHourOffset = 6;
const uint8_t kDaikin128BitHalfHour = 1 << kDaikin128HalfHourOffset;
// const uint8_t kDaikin128MaskHours =       0b00111111;
const uint8_t kDaikin128HoursOffset = 0;
const uint8_t kDaikin128HoursSize = 6;
const uint8_t kDaikin128ByteTemp = 6;
const uint8_t kDaikin128MinTemp = 16;  // C
const uint8_t kDaikin128MaxTemp = 30;  // C
const uint8_t kDaikin128BytePowerSwingSleep = 7;
const uint8_t kDaikin128BitSwingOffset = 0;
const uint8_t kDaikin128BitSwing = 1 << kDaikin128BitSwingOffset;  // 0b00000001
const uint8_t kDaikin128BitSleepOffset = 1;
const uint8_t kDaikin128BitSleep = 1 << kDaikin128BitSleepOffset;  // 0b00000010
const uint8_t kDaikin128BitPowerToggleOffset = 3;
const uint8_t kDaikin128BitPowerToggle = 1 << kDaikin128BitPowerToggleOffset;
const uint8_t kDaikin128ByteEconoLight = 9;
const uint8_t kDaikin128BitEconoOffset = 2;
const uint8_t kDaikin128BitEcono = 1 << kDaikin128BitEconoOffset;  // 0b00000100
const uint8_t kDaikin128BitWall =         0b00001000;
const uint8_t kDaikin128BitCeiling =      0b00000001;
const uint8_t kDaikin128MaskLight = kDaikin128BitWall | kDaikin128BitCeiling;


const uint16_t kDaikin152Freq = 38000;  // Modulation Frequency in Hz.
const uint8_t  kDaikin152LeaderBits = 5;
const uint16_t kDaikin152HdrMark = 3492;
const uint16_t kDaikin152HdrSpace = 1718;
const uint16_t kDaikin152BitMark = 433;
const uint16_t kDaikin152OneSpace = 1529;
const uint16_t kDaikin152ZeroSpace = kDaikin152BitMark;
const uint16_t kDaikin152Gap = 25182;

// Byte[5]
const uint8_t kDaikin152ModeByte = 5;                        // Mask 0b01110000
const uint8_t kDaikin152PowerByte = kDaikin152ModeByte;      // Mask 0b00000001
// Byte[6]
const uint8_t kDaikin152TempByte = 6;                        // Mask 0b11111110
const uint8_t kDaikin152TempSize = 7;
const uint8_t kDaikin152DryTemp = kDaikin2MinCoolTemp;  // Celsius
const uint8_t kDaikin152FanTemp = 0x60;  // 96 Celsius
// Byte[8]
const uint8_t kDaikin152FanByte = 8;
const uint8_t kDaikin152SwingVByte = kDaikin152FanByte;
// Byte[13]
const uint8_t kDaikin152QuietByte = 13;                      // Mask 0b00100000
const uint8_t kDaikin152PowerfulByte = kDaikin152QuietByte;  // Mask 0b00000001
// Byte[16]
const uint8_t kDaikin152EconoByte = 16;                      // Mask 0b00000100
const uint8_t kDaikin152ComfortByte = kDaikin152EconoByte;   // Mask 0b00000010
const uint8_t kDaikin152ComfortOffset = 1;                   // Mask 0b00000010
const uint8_t kDaikin152SensorByte = kDaikin152EconoByte;    // Mask 0b00001000
const uint8_t kDaikin152SensorOffset = 3;                    // Mask 0b00001000


const uint16_t kDaikin64HdrMark = kDaikin128HdrMark;
const uint16_t kDaikin64BitMark = kDaikin128BitMark;
const uint16_t kDaikin64HdrSpace = kDaikin128HdrSpace;
const uint16_t kDaikin64OneSpace = kDaikin128OneSpace;
const uint16_t kDaikin64ZeroSpace = kDaikin128ZeroSpace;
const uint16_t kDaikin64LdrMark = kDaikin128LeaderMark;
const uint16_t kDaikin64Gap = kDaikin128Gap;
const uint16_t kDaikin64LdrSpace = kDaikin128LeaderSpace;
const uint16_t kDaikin64Freq = kDaikin128Freq;  // Hz.
const uint8_t kDaikin64Overhead = 9;
const int8_t  kDaikin64ToleranceDelta = 5;  // +5%

const uint64_t kDaikin64KnownGoodState = 0x7C16161607204216;
const uint8_t kDaikin64ModeOffset = 8;
const uint8_t kDaikin64ModeSize = 4;  // Mask 0b111100000000
const uint8_t kDaikin64Dry =  0b001;
const uint8_t kDaikin64Cool = 0b010;
const uint8_t kDaikin64Fan =  0b100;
const uint8_t kDaikin64FanOffset = kDaikin64ModeOffset + kDaikin64ModeSize;
const uint8_t kDaikin64FanSize = 4;  // Mask 0b1111000000000000
const uint8_t kDaikin64FanAuto =  0b0001;
const uint8_t kDaikin64FanLow =   0b1000;
const uint8_t kDaikin64FanMed =   0b0100;
const uint8_t kDaikin64FanHigh =  0b0010;
const uint8_t kDaikin64FanQuiet = 0b1001;
const uint8_t kDaikin64FanTurbo = 0b0011;
const uint8_t kDaikin64ClockOffset = kDaikin64FanOffset + kDaikin64FanSize;
const uint8_t kDaikin64ClockMinsSize = 8;
const uint8_t kDaikin64ClockHoursSize = 8;
const uint8_t kDaikin64ClockSize = kDaikin64ClockMinsSize +
    kDaikin64ClockHoursSize;  // Mask 0b1111111111111111 << 15
const uint8_t kDaikin64OnTimeOffset = kDaikin64ClockOffset +
                                      kDaikin64ClockSize;
const uint8_t kDaikin64OnTimeSize = 6;
const uint8_t kDaikin64OnTimeHalfHourBit = kDaikin64OnTimeOffset +
                                           kDaikin64OnTimeSize;
const uint8_t kDaikin64OnTimeEnableBit = kDaikin64OnTimeHalfHourBit + 1;
const uint8_t kDaikin64OffTimeOffset = kDaikin64OnTimeEnableBit + 1;
const uint8_t kDaikin64OffTimeSize = 6;
const uint8_t kDaikin64OffTimeHalfHourBit = kDaikin64OffTimeOffset +
                                            kDaikin64OffTimeSize;
const uint8_t kDaikin64OffTimeEnableBit = kDaikin64OffTimeHalfHourBit + 1;
const uint8_t kDaikin64TempOffset = 48;
const uint8_t kDaikin64TempSize = 8;  // Mask 0b11111111 << 47
const uint8_t kDaikin64MinTemp = 16;  // Celsius
const uint8_t kDaikin64MaxTemp = 30;  // Celsius
const uint8_t kDaikin64SwingVBit = 56;
const uint8_t kDaikin64SleepBit = kDaikin64SwingVBit + 1;
const uint8_t kDaikin64PowerToggleBit = 59;
const uint8_t kDaikin64ChecksumOffset = 60;
const uint8_t kDaikin64ChecksumSize = 4;  // Mask 0b1111 << 59

// Legacy defines.
#define DAIKIN_COOL kDaikinCool
#define DAIKIN_HEAT kDaikinHeat
#define DAIKIN_FAN kDaikinFan
#define DAIKIN_AUTO kDaikinAuto
#define DAIKIN_DRY kDaikinDry
#define DAIKIN_MIN_TEMP kDaikinMinTemp
#define DAIKIN_MAX_TEMP kDaikinMaxTemp
#define DAIKIN_FAN_MIN kDaikinFanMin
#define DAIKIN_FAN_MAX kDaikinFanMax
#define DAIKIN_FAN_AUTO kDaikinFanAuto
#define DAIKIN_FAN_QUIET kDaikinFanQuiet

/// Class for handling detailed Daikin 280-bit A/C messages.
class IRDaikinESP {
 public:
  explicit IRDaikinESP(const uint16_t pin, const bool inverted = false,
                       const bool use_modulation = true);

#if SEND_DAIKIN
  void send(const uint16_t repeat = kDaikinDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif
  void begin(void);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void);
  void setTemp(const uint8_t temp);
  uint8_t getTemp();
  void setFan(const uint8_t fan);
  uint8_t getFan(void);
  void setMode(const uint8_t mode);
  uint8_t getMode(void);
  void setSwingVertical(const bool on);
  bool getSwingVertical(void);
  void setSwingHorizontal(const bool on);
  bool getSwingHorizontal(void);
  bool getQuiet(void);
  void setQuiet(const bool on);
  bool getPowerful(void);
  void setPowerful(const bool on);
  void setSensor(const bool on);
  bool getSensor(void);
  void setEcono(const bool on);
  bool getEcono(void);
  void setMold(const bool on);
  bool getMold(void);
  void setComfort(const bool on);
  bool getComfort(void);
  void enableOnTimer(const uint16_t starttime);
  void disableOnTimer(void);
  uint16_t getOnTime(void);
  bool getOnTimerEnabled();
  void enableOffTimer(const uint16_t endtime);
  void disableOffTimer(void);
  uint16_t getOffTime(void);
  bool getOffTimerEnabled(void);
  void setCurrentTime(const uint16_t mins_since_midnight);
  uint16_t getCurrentTime(void);
  void setCurrentDay(const uint8_t day_of_week);
  uint8_t getCurrentDay(void);
  void setWeeklyTimerEnable(const bool on);
  bool getWeeklyTimerEnable(void);
  uint8_t* getRaw(void);
  void setRaw(const uint8_t new_code[],
              const uint16_t length = kDaikinStateLength);
  static bool validChecksum(uint8_t state[],
                            const uint16_t length = kDaikinStateLength);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(void);
  String toString(void);
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< instance of the IR send class
#else
  /// @cond IGNORE
  IRsendTest _irsend;  ///< instance of the testing IR send class
  /// @endcond
#endif
  // # of bytes per command
  uint8_t remote[kDaikinStateLength];  ///< The state of the IR remote.
  void stateReset(void);
  void checksum(void);
};

/// Class for handling detailed Daikin 312-bit A/C messages.
/// @note Code by crankyoldgit, Reverse engineering analysis by sheppy99
class IRDaikin2 {
 public:
  explicit IRDaikin2(const uint16_t pin, const bool inverted = false,
                     const bool use_modulation = true);

#if SEND_DAIKIN2
  void send(const uint16_t repeat = kDaikin2DefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif
  void begin();
  void on();
  void off();
  void setPower(const bool state);
  bool getPower();
  void setTemp(const uint8_t temp);
  uint8_t getTemp();
  void setFan(const uint8_t fan);
  uint8_t getFan();
  uint8_t getMode();
  void setMode(const uint8_t mode);
  void setSwingVertical(const uint8_t position);
  uint8_t getSwingVertical();
  void setSwingHorizontal(const uint8_t position);
  uint8_t getSwingHorizontal();
  bool getQuiet();
  void setQuiet(const bool on);
  bool getPowerful();
  void setPowerful(const bool on);
  void setEcono(const bool on);
  bool getEcono();
  void setEye(const bool on);
  bool getEye();
  void setEyeAuto(const bool on);
  bool getEyeAuto();
  void setPurify(const bool on);
  bool getPurify();
  void setMold(const bool on);
  bool getMold();
  void enableOnTimer(const uint16_t starttime);
  void disableOnTimer();
  uint16_t getOnTime();
  bool getOnTimerEnabled();
  void enableSleepTimer(const uint16_t sleeptime);
  void disableSleepTimer();
  uint16_t getSleepTime();
  bool getSleepTimerEnabled();
  void enableOffTimer(const uint16_t endtime);
  void disableOffTimer();
  uint16_t getOffTime();
  bool getOffTimerEnabled();
  void setCurrentTime(const uint16_t time);
  uint16_t getCurrentTime();
  void setBeep(const uint8_t beep);
  uint8_t getBeep();
  void setLight(const uint8_t light);
  uint8_t getLight();
  void setClean(const bool on);
  bool getClean();
  void setFreshAir(const bool on);
  bool getFreshAir();
  void setFreshAirHigh(const bool on);
  bool getFreshAirHigh();
  uint8_t* getRaw();
  void setRaw(const uint8_t new_code[]);
  static bool validChecksum(uint8_t state[],
                            const uint16_t length = kDaikin2StateLength);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static uint8_t convertSwingV(const stdAc::swingv_t position);
  static uint8_t convertSwingH(const stdAc::swingh_t position);
  static stdAc::swingv_t toCommonSwingV(const uint8_t setting);
  static stdAc::swingh_t toCommonSwingH(const uint8_t setting);
  stdAc::state_t toCommon(void);
  String toString();
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< instance of the IR send class
#else
  /// @cond IGNORE
  IRsendTest _irsend;  ///< instance of the testing IR send class
  /// @endcond
#endif
  // # of bytes per command
  uint8_t remote_state[kDaikin2StateLength];  ///< The state of the IR remote.
  void stateReset();
  void checksum();
  void clearOnTimerFlag();
  void clearSleepTimerFlag();
};

/// Class for handling detailed Daikin 216-bit A/C messages.
class IRDaikin216 {
 public:
  explicit IRDaikin216(const uint16_t pin, const bool inverted = false,
                       const bool use_modulation = true);

#if SEND_DAIKIN216
  void send(const uint16_t repeat = kDaikin216DefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif
  void begin();
  uint8_t* getRaw();
  void setRaw(const uint8_t new_code[]);
  static bool validChecksum(uint8_t state[],
                            const uint16_t length = kDaikin216StateLength);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void);
  void setTemp(const uint8_t temp);
  uint8_t getTemp();
  void setMode(const uint8_t mode);
  uint8_t getMode(void);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  void setFan(const uint8_t fan);
  uint8_t getFan(void);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  void setSwingVertical(const bool on);
  bool getSwingVertical(void);
  void setSwingHorizontal(const bool on);
  bool getSwingHorizontal(void);
  void setQuiet(const bool on);
  bool getQuiet(void);
  void setPowerful(const bool on);
  bool getPowerful(void);
  stdAc::state_t toCommon(void);
  String toString(void);
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< instance of the IR send class
#else
  /// @cond IGNORE
  IRsendTest _irsend;  ///< instance of the testing IR send class
  /// @endcond
#endif
  // # of bytes per command
  uint8_t remote_state[kDaikin216StateLength];  ///< The state of the IR remote.
  void stateReset();
  void checksum();
};

/// Class for handling detailed Daikin 160-bit A/C messages.
class IRDaikin160 {
 public:
  explicit IRDaikin160(const uint16_t pin, const bool inverted = false,
                       const bool use_modulation = true);

#if SEND_DAIKIN160
  void send(const uint16_t repeat = kDaikin160DefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif
  void begin();
  uint8_t* getRaw();
  void setRaw(const uint8_t new_code[]);
  static bool validChecksum(uint8_t state[],
                            const uint16_t length = kDaikin160StateLength);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void);
  void setTemp(const uint8_t temp);
  uint8_t getTemp();
  void setMode(const uint8_t mode);
  uint8_t getMode(void);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  void setFan(const uint8_t fan);
  uint8_t getFan(void);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  void setSwingVertical(const uint8_t position);
  uint8_t getSwingVertical(void);
  static uint8_t convertSwingV(const stdAc::swingv_t position);
  static stdAc::swingv_t toCommonSwingV(const uint8_t setting);
  stdAc::state_t toCommon(void);
  String toString(void);
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< instance of the IR send class
#else
  /// @cond IGNORE
  IRsendTest _irsend;  ///< instance of the testing IR send class
  /// @endcond
#endif
  // # of bytes per command
  uint8_t remote_state[kDaikin160StateLength];  ///< The state of the IR remote.
  void stateReset();
  void checksum();
};

/// Class for handling detailed Daikin 176-bit A/C messages.
class IRDaikin176 {
 public:
  explicit IRDaikin176(const uint16_t pin, const bool inverted = false,
                       const bool use_modulation = true);

#if SEND_DAIKIN176
  void send(const uint16_t repeat = kDaikin176DefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif
  void begin();
  uint8_t* getRaw();
  void setRaw(const uint8_t new_code[]);
  static bool validChecksum(uint8_t state[],
                            const uint16_t length = kDaikin176StateLength);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void);
  void setTemp(const uint8_t temp);
  uint8_t getTemp();
  void setMode(const uint8_t mode);
  uint8_t getMode(void);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  void setFan(const uint8_t fan);
  uint8_t getFan(void);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  void setSwingHorizontal(const uint8_t position);
  uint8_t getSwingHorizontal(void);
  static uint8_t convertSwingH(const stdAc::swingh_t position);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::swingh_t toCommonSwingH(const uint8_t setting);
  stdAc::state_t toCommon(void);
  String toString(void);

#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< instance of the IR send class
#else
  /// @cond IGNORE
  IRsendTest _irsend;  ///< instance of the testing IR send class
  /// @endcond
#endif
  // # of bytes per command
  uint8_t remote_state[kDaikin176StateLength];  ///< The state of the IR remote.
  uint8_t _saved_temp;
  void stateReset();
  void checksum();
};

/// Class for handling detailed Daikin 128-bit A/C messages.
/// @note Code by crankyoldgit. Analysis by Daniel Vena
class IRDaikin128 {
 public:
  explicit IRDaikin128(const uint16_t pin, const bool inverted = false,
                       const bool use_modulation = true);
#if SEND_DAIKIN128
  void send(const uint16_t repeat = kDaikin128DefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_DAIKIN128
  void begin();
  void setPowerToggle(const bool toggle);
  bool getPowerToggle(void);
  void setTemp(const uint8_t temp);
  uint8_t getTemp(void);
  void setFan(const uint8_t fan);
  uint8_t getFan(void);
  uint8_t getMode(void);
  void setMode(const uint8_t mode);
  void setSwingVertical(const bool on);
  bool getSwingVertical();
  bool getSleep(void);
  void setSleep(const bool on);
  bool getQuiet(void);
  void setQuiet(const bool on);
  bool getPowerful(void);
  void setPowerful(const bool on);
  void setEcono(const bool on);
  bool getEcono(void);
  void setOnTimer(const uint16_t mins_since_midnight);
  uint16_t getOnTimer(void);
  bool getOnTimerEnabled(void);
  void setOnTimerEnabled(const bool on);
  void setOffTimer(const uint16_t mins_since_midnight);
  uint16_t getOffTimer(void);
  bool getOffTimerEnabled(void);
  void setOffTimerEnabled(const bool on);
  void setClock(const uint16_t mins_since_midnight);
  uint16_t getClock(void);
  void setLightToggle(const uint8_t unit_type);
  uint8_t getLightToggle(void);
  uint8_t* getRaw(void);
  void setRaw(const uint8_t new_code[]);
  static bool validChecksum(uint8_t state[]);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(const stdAc::state_t *prev = NULL);
  String toString(void);
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< instance of the IR send class
#else
  /// @cond IGNORE
  IRsendTest _irsend;  ///< instance of the testing IR send class
  /// @endcond
#endif
  // # of bytes per command
  uint8_t remote_state[kDaikin128StateLength];  ///< The state of the IR remote.
  void stateReset(void);
  static uint8_t calcFirstChecksum(const uint8_t state[]);
  static uint8_t calcSecondChecksum(const uint8_t state[]);
  static void setTimer(uint8_t *ptr, const uint16_t mins_since_midnight);
  static uint16_t getTimer(const uint8_t *ptr);
  void checksum(void);
  void clearOnTimerFlag(void);
  void clearSleepTimerFlag(void);
};

/// Class for handling detailed Daikin 152-bit A/C messages.
class IRDaikin152 {
 public:
  explicit IRDaikin152(const uint16_t pin, const bool inverted = false,
                       const bool use_modulation = true);

#if SEND_DAIKIN152
  void send(const uint16_t repeat = kDaikin152DefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif
  void begin();
  uint8_t* getRaw();
  void setRaw(const uint8_t new_code[]);
  static bool validChecksum(uint8_t state[],
                            const uint16_t length = kDaikin152StateLength);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void);
  void setTemp(const uint8_t temp);
  uint8_t getTemp();
  void setFan(const uint8_t fan);
  uint8_t getFan(void);
  void setMode(const uint8_t mode);
  uint8_t getMode(void);
  void setSwingV(const bool on);
  bool getSwingV(void);
  bool getQuiet(void);
  void setQuiet(const bool on);
  bool getPowerful(void);
  void setPowerful(const bool on);
  void setSensor(const bool on);
  bool getSensor(void);
  void setEcono(const bool on);
  bool getEcono(void);
  void setComfort(const bool on);
  bool getComfort(void);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  stdAc::state_t toCommon(void);
  String toString(void);
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< instance of the IR send class
#else
  /// @cond IGNORE
  IRsendTest _irsend;  ///< instance of the testing IR send class
  /// @endcond
#endif
  // # of bytes per command
  uint8_t remote_state[kDaikin152StateLength];  ///< The state of the IR remote.
  void stateReset();
  void checksum();
};

/// Class for handling detailed Daikin 64-bit A/C messages.
class IRDaikin64 {
 public:
  explicit IRDaikin64(const uint16_t pin, const bool inverted = false,
                       const bool use_modulation = true);

#if SEND_DAIKIN64
  void send(const uint16_t repeat = kDaikin64DefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_DAIKIN64
  void begin();
  uint64_t getRaw();
  void setRaw(const uint64_t new_state);
  static uint8_t calcChecksum(const uint64_t state);
  static bool validChecksum(const uint64_t state);
  void setPowerToggle(const bool on);
  bool getPowerToggle(void);
  void setTemp(const uint8_t temp);
  uint8_t getTemp();
  void setFan(const uint8_t fan);
  uint8_t getFan(void);
  void setMode(const uint8_t mode);
  uint8_t getMode(void);
  void setSwingVertical(const bool on);
  bool getSwingVertical(void);
  void setSleep(const bool on);
  bool getSleep(void);
  bool getQuiet(void);
  void setQuiet(const bool on);
  bool getTurbo(void);
  void setTurbo(const bool on);
  void setClock(const uint16_t mins_since_midnight);
  uint16_t getClock(void);
  void setOnTimeEnabled(const bool on);
  bool getOnTimeEnabled(void);
  void setOnTime(const uint16_t mins_since_midnight);
  uint16_t getOnTime(void);
  void setOffTimeEnabled(const bool on);
  bool getOffTimeEnabled(void);
  void setOffTime(const uint16_t mins_since_midnight);
  uint16_t getOffTime(void);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(const stdAc::state_t *prev = NULL);
  String toString(void);
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< instance of the IR send class
#else
  /// @cond IGNORE
  IRsendTest _irsend;  ///< instance of the testing IR send class
  /// @endcond
#endif
  uint64_t remote_state;  ///< The state of the IR remote.
  void stateReset();
  void checksum();
};
#endif  // IR_DAIKIN_H_
