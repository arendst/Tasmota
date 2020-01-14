 /***************************************************
 * IRremote for ESP8266
 *
 * Based on the IRremote library for Arduino by Ken Shirriff
 * Version 0.11 August, 2009
 * Copyright 2009 Ken Shirriff
 * For details, see http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.html
 *
 * Edited by Mitra to add new controller SANYO
 *
 * Interrupt code based on NECIRrcv by Joe Knapp
 * http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1210243556
 * Also influenced by http://zovirl.com/2008/11/12/building-a-universal-remote-with-an-arduino/
 *
 * JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
 * LG added by Darryl Smith (based on the JVC protocol)
 * Whynter A/C ARC-110WD added by Francesco Meschia
 * Coolix A/C / heatpump added by (send) bakrus & (decode) crankyoldgit
 * Denon: sendDenon, decodeDenon added by Massimiliano Pinto
          (from https://github.com/z3t0/Arduino-IRremote/blob/master/ir_Denon.cpp)
 * Kelvinator A/C and Sherwood added by crankyoldgit
 * Mitsubishi (TV) sending added by crankyoldgit
 * Pronto code sending added by crankyoldgit
 * Mitsubishi & Toshiba A/C added by crankyoldgit
 *     (derived from https://github.com/r45635/HVAC-IR-Control)
 * DISH decode by marcosamarinho
 * Gree Heatpump sending added by Ville Skyttä (scop)
 *     (derived from https://github.com/ToniA/arduino-heatpumpir/blob/master/GreeHeatpumpIR.cpp)
 * Updated by markszabo (https://github.com/crankyoldgit/IRremoteESP8266) for sending IR code on ESP8266
 * Updated by Sebastien Warin (http://sebastien.warin.fr) for receiving IR code on ESP8266
 *
 * Updated by sillyfrog for Daikin, adopted from
 * (https://github.com/mharizanov/Daikin-AC-remote-control-over-the-Internet/)
 * Fujitsu A/C code added by jonnygraham
 * Trotec AC code by stufisher
 * Carrier & Haier AC code by crankyoldgit
 * Vestel AC code by Erdem U. Altınyurt
 * Teco AC code by Fabien Valthier (hcoohb)
 * Mitsubishi 112 AC Code by kuchel77
 *
 *  GPL license, all text above must be included in any redistribution
 ****************************************************/

#ifndef IRREMOTEESP8266_H_
#define IRREMOTEESP8266_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#ifdef UNIT_TEST
#include <iostream>
#include <string>
#endif  // UNIT_TEST

// Library Version
#define _IRREMOTEESP8266_VERSION_ "2.7.2"

// Set the language & locale for the library. See the `locale` dir for options.
#ifndef _IR_LOCALE_
#define _IR_LOCALE_ en-AU
#endif  // _IR_LOCALE_

// Supported IR protocols
// Each protocol you include costs memory and, during decode, costs time
// Disable (set to false) all the protocols you do not need/want!
// The Air Conditioner protocols are the most expensive memory-wise.
//

#ifdef USE_IR_REMOTE_FULL   // full IR protocols
#define DECODE_HASH            true  // Semi-unique code for unknown messages

#define SEND_RAW               true

#define DECODE_NEC             true
#define SEND_NEC               true

#define DECODE_SHERWOOD        true  // Doesn't exist. Actually is DECODE_NEC
#define SEND_SHERWOOD          true

#define DECODE_RC5             true
#define SEND_RC5               true

#define DECODE_RC6             true
#define SEND_RC6               true

#define DECODE_RCMM            true
#define SEND_RCMM              true

#define DECODE_SONY            true
#define SEND_SONY              true

#define DECODE_PANASONIC       true
#define SEND_PANASONIC         true

#define DECODE_JVC             true
#define SEND_JVC               true

#define DECODE_SAMSUNG         true
#define SEND_SAMSUNG           true

#define DECODE_SAMSUNG36       true
#define SEND_SAMSUNG36         true

#define DECODE_SAMSUNG_AC      true
#define SEND_SAMSUNG_AC        true

#define DECODE_WHYNTER         true
#define SEND_WHYNTER           true

#define DECODE_AIWA_RC_T501    true
#define SEND_AIWA_RC_T501      true

#define DECODE_LG              true
#define SEND_LG                true

#define DECODE_SANYO           true
#define SEND_SANYO             true

#define DECODE_MITSUBISHI      true
#define SEND_MITSUBISHI        true

#define DECODE_MITSUBISHI2     true
#define SEND_MITSUBISHI2       true

#define DECODE_DISH            true
#define SEND_DISH              true

#define DECODE_SHARP           true
#define SEND_SHARP             true

#define DECODE_SHARP_AC        true
#define SEND_SHARP_AC          true

#define DECODE_DENON           true
#define SEND_DENON             true

#define DECODE_KELVINATOR      true
#define SEND_KELVINATOR        true

#define DECODE_MITSUBISHI_AC   true  // Beta.
#define SEND_MITSUBISHI_AC     true

#define DECODE_MITSUBISHI136   true
#define SEND_MITSUBISHI136     true

#define DECODE_MITSUBISHI112   true
#define SEND_MITSUBISHI112     true

#define DECODE_FUJITSU_AC      true
#define SEND_FUJITSU_AC        true

#define DECODE_INAX            true
#define SEND_INAX              true

#define DECODE_DAIKIN          true
#define SEND_DAIKIN            true

#define DECODE_COOLIX          true
#define SEND_COOLIX            true

#define DECODE_GLOBALCACHE     false  // Not written.
#define SEND_GLOBALCACHE       true

#define DECODE_GOODWEATHER     true
#define SEND_GOODWEATHER       true

#define DECODE_GREE            true
#define SEND_GREE              true

#define DECODE_PRONTO          false  // Not written.
#define SEND_PRONTO            true

#define DECODE_ARGO            true  // Experimental
#define SEND_ARGO              true

#define DECODE_TROTEC          true
#define SEND_TROTEC            true

#define DECODE_NIKAI           true
#define SEND_NIKAI             true

#define DECODE_TOSHIBA_AC      true
#define SEND_TOSHIBA_AC        true

#define DECODE_MAGIQUEST       true
#define SEND_MAGIQUEST         true

#define DECODE_MIDEA           true
#define SEND_MIDEA             true

#define DECODE_LASERTAG        true
#define SEND_LASERTAG          true

#define DECODE_CARRIER_AC      true
#define SEND_CARRIER_AC        true

#define DECODE_HAIER_AC        true
#define SEND_HAIER_AC          true

#define DECODE_HITACHI_AC      true
#define SEND_HITACHI_AC        true

#define DECODE_HITACHI_AC1     true
#define SEND_HITACHI_AC1       true

#define DECODE_HITACHI_AC2     true
#define SEND_HITACHI_AC2       true

#define DECODE_GICABLE         true
#define SEND_GICABLE           true

#define DECODE_HAIER_AC_YRW02  true
#define SEND_HAIER_AC_YRW02    true

#define DECODE_WHIRLPOOL_AC    true
#define SEND_WHIRLPOOL_AC      true

#define DECODE_LUTRON          true
#define SEND_LUTRON            true

#define DECODE_ELECTRA_AC      true
#define SEND_ELECTRA_AC        true

#define DECODE_PANASONIC_AC    true
#define SEND_PANASONIC_AC      true

#define DECODE_MWM             true
#define SEND_MWM               true

#define DECODE_PIONEER         true
#define SEND_PIONEER           true

#define DECODE_DAIKIN2         true
#define SEND_DAIKIN2           true

#define DECODE_VESTEL_AC       true
#define SEND_VESTEL_AC         true

#define DECODE_TECO            true
#define SEND_TECO              true

#define DECODE_TCL112AC        true
#define SEND_TCL112AC          true

#define DECODE_LEGOPF          true
#define SEND_LEGOPF            true

#define DECODE_MITSUBISHIHEAVY true
#define SEND_MITSUBISHIHEAVY   true

#define DECODE_DAIKIN216       true
#define SEND_DAIKIN216         true

#define DECODE_DAIKIN160       true
#define SEND_DAIKIN160         true

#define DECODE_NEOCLIMA        true
#define SEND_NEOCLIMA          true

#define DECODE_DAIKIN176       true
#define SEND_DAIKIN176         true

#define DECODE_DAIKIN128       true
#define SEND_DAIKIN128         true

#define DECODE_AMCOR           true
#define SEND_AMCOR             true

#define DECODE_DAIKIN152       true
#define SEND_DAIKIN152         true

#define DECODE_HITACHI_AC424   true
#define SEND_HITACHI_AC424     true

#else // defined(FIRMWARE_IR) || defined(FIRMWARE_IR_CUSTOM)   // full IR protocols

// Tasmota supported protocols (less protocols is less code size)
#define DECODE_HASH            true  // Semi-unique code for unknown messages

#define SEND_RAW               false

#define DECODE_NEC             true
#define SEND_NEC               true

#define DECODE_SHERWOOD        false  // Doesn't exist. Actually is DECODE_NEC
#define SEND_SHERWOOD          false

#define DECODE_RC5             true
#define SEND_RC5               true

#define DECODE_RC6             true
#define SEND_RC6               true

#define DECODE_RCMM            false
#define SEND_RCMM              false

#define DECODE_SONY            false
#define SEND_SONY              false

#define DECODE_PANASONIC       false
#define SEND_PANASONIC         false

#define DECODE_JVC             false
#define SEND_JVC               false

#define DECODE_SAMSUNG         false
#define SEND_SAMSUNG           false

#define DECODE_SAMSUNG36       false
#define SEND_SAMSUNG36         false

#define DECODE_SAMSUNG_AC      false
#define SEND_SAMSUNG_AC        false

#define DECODE_WHYNTER         false
#define SEND_WHYNTER           false

#define DECODE_AIWA_RC_T501    false
#define SEND_AIWA_RC_T501      false

#define DECODE_LG              false
#define SEND_LG                false

#define DECODE_SANYO           false
#define SEND_SANYO             false

#define DECODE_MITSUBISHI      false
#define SEND_MITSUBISHI        false

#define DECODE_MITSUBISHI2     false
#define SEND_MITSUBISHI2       false

#define DECODE_DISH            false
#define SEND_DISH              false

#define DECODE_SHARP           false
#define SEND_SHARP             false

#define DECODE_SHARP_AC        false
#define SEND_SHARP_AC          false

#define DECODE_DENON           false
#define SEND_DENON             false

#define DECODE_KELVINATOR      false
#define SEND_KELVINATOR        false

#define DECODE_MITSUBISHI_AC   false  // Beta.
#define SEND_MITSUBISHI_AC     false

#define DECODE_FUJITSU_AC      false
#define SEND_FUJITSU_AC        false

#define DECODE_INAX            false
#define SEND_INAX              false

#define DECODE_DAIKIN          false
#define SEND_DAIKIN            false

#define DECODE_COOLIX          false
#define SEND_COOLIX            false

#define DECODE_GLOBALCACHE     false  // Not written.
#define SEND_GLOBALCACHE       false

#define DECODE_GOODWEATHER     false
#define SEND_GOODWEATHER       false

#define DECODE_GREE            false
#define SEND_GREE              false

#define DECODE_PRONTO          false  // Not written.
#define SEND_PRONTO            false

#define DECODE_ARGO            false  // Experimental
#define SEND_ARGO              false

#define DECODE_TROTEC          false
#define SEND_TROTEC            false

#define DECODE_NIKAI           false
#define SEND_NIKAI             false

#define DECODE_TOSHIBA_AC      false
#define SEND_TOSHIBA_AC        false

#define DECODE_MAGIQUEST       false
#define SEND_MAGIQUEST         false

#define DECODE_MIDEA           false
#define SEND_MIDEA             false

#define DECODE_LASERTAG        false
#define SEND_LASERTAG          false

#define DECODE_CARRIER_AC      false
#define SEND_CARRIER_AC        false

#define DECODE_HAIER_AC        false
#define SEND_HAIER_AC          false

#define DECODE_HITACHI_AC      false
#define SEND_HITACHI_AC        false

#define DECODE_HITACHI_AC1     false
#define SEND_HITACHI_AC1       false

#define DECODE_HITACHI_AC2     false
#define SEND_HITACHI_AC2       false

#define DECODE_GICABLE         false
#define SEND_GICABLE           false

#define DECODE_HAIER_AC_YRW02  false
#define SEND_HAIER_AC_YRW02    false

#define DECODE_WHIRLPOOL_AC    false
#define SEND_WHIRLPOOL_AC      false

#define DECODE_LUTRON          false
#define SEND_LUTRON            false

#define DECODE_ELECTRA_AC      false
#define SEND_ELECTRA_AC        false

#define DECODE_PANASONIC_AC    false
#define SEND_PANASONIC_AC      false

#define DECODE_MWM             false
#define SEND_MWM               false

#define DECODE_PIONEER         false
#define SEND_PIONEER           false

#define DECODE_DAIKIN2         false
#define SEND_DAIKIN2           false

#define DECODE_VESTEL_AC       false
#define SEND_VESTEL_AC         false

#define DECODE_TECO            false
#define SEND_TECO              false

#define DECODE_TCL112AC        false
#define SEND_TCL112AC          false

#define DECODE_LEGOPF          false
#define SEND_LEGOPF            false

#define DECODE_MITSUBISHIHEAVY false
#define SEND_MITSUBISHIHEAVY   false

#define DECODE_DAIKIN216       false
#define SEND_DAIKIN216         false

#define DECODE_DAIKIN160       false
#define SEND_DAIKIN160         false

#define DECODE_NEOCLIMA        false
#define SEND_NEOCLIMA          false

#define DECODE_DAIKIN176       false
#define SEND_DAIKIN176         false

#define DECODE_DAIKIN128       false
#define SEND_DAIKIN128         false

#define DECODE_AMCOR           false
#define SEND_AMCOR             false

#define DECODE_DAIKIN152       false
#define SEND_DAIKIN152         false

#define DECODE_HITACHI_AC424   false
#define SEND_HITACHI_AC424     false

#endif  // defined(FIRMWARE_IR) || defined(FIRMWARE_IR_CUSTOM)   // full IR protocols

#if (DECODE_ARGO || DECODE_DAIKIN || DECODE_FUJITSU_AC || DECODE_GREE || \
     DECODE_KELVINATOR || DECODE_MITSUBISHI_AC || DECODE_TOSHIBA_AC || \
     DECODE_TROTEC || DECODE_HAIER_AC || DECODE_HITACHI_AC || \
     DECODE_HITACHI_AC1 || DECODE_HITACHI_AC2 || DECODE_HAIER_AC_YRW02 || \
     DECODE_WHIRLPOOL_AC || DECODE_SAMSUNG_AC || DECODE_ELECTRA_AC || \
     DECODE_PANASONIC_AC || DECODE_MWM || DECODE_DAIKIN2 || \
     DECODE_VESTEL_AC || DECODE_TCL112AC || DECODE_MITSUBISHIHEAVY || \
     DECODE_DAIKIN216 || DECODE_SHARP_AC || DECODE_DAIKIN160 || \
     DECODE_NEOCLIMA || DECODE_DAIKIN176 || DECODE_DAIKIN128 || \
     DECODE_AMCOR || DECODE_DAIKIN152 || DECODE_MITSUBISHI136 || \
     DECODE_MITSUBISHI112 || DECODE_HITACHI_AC424)
#define DECODE_AC true  // We need some common infrastructure for decoding A/Cs.
#else
#define DECODE_AC false   // We don't need that infrastructure.
#endif

// Use millisecond 'delay()' calls where we can to avoid tripping the WDT.
// Note: If you plan to send IR messages in the callbacks of the AsyncWebserver
//       library, you need to set ALLOW_DELAY_CALLS to false.
//       Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/430
#define ALLOW_DELAY_CALLS true

/*
 * Always add to the end of the list and should never remove entries
 * or change order. Projects may save the type number for later usage
 * so numbering should always stay the same.
 */
enum decode_type_t {
  UNKNOWN = -1,
  UNUSED = 0,
  RC5,
  RC6,
  NEC,
  SONY,
  PANASONIC,  // (5)
  JVC,
  SAMSUNG,
  WHYNTER,
  AIWA_RC_T501,
  LG,  // (10)
  SANYO,
  MITSUBISHI,
  DISH,
  SHARP,
  COOLIX,  // (15)
  DAIKIN,
  DENON,
  KELVINATOR,
  SHERWOOD,
  MITSUBISHI_AC,  // (20)
  RCMM,
  SANYO_LC7461,
  RC5X,
  GREE,
  PRONTO,  // Technically not a protocol, but an encoding. (25)
  NEC_LIKE,
  ARGO,
  TROTEC,
  NIKAI,
  RAW,  // Technically not a protocol, but an encoding. (30)
  GLOBALCACHE,  // Technically not a protocol, but an encoding.
  TOSHIBA_AC,
  FUJITSU_AC,
  MIDEA,
  MAGIQUEST,  // (35)
  LASERTAG,
  CARRIER_AC,
  HAIER_AC,
  MITSUBISHI2,
  HITACHI_AC,  // (40)
  HITACHI_AC1,
  HITACHI_AC2,
  GICABLE,
  HAIER_AC_YRW02,
  WHIRLPOOL_AC,  // (45)
  SAMSUNG_AC,
  LUTRON,
  ELECTRA_AC,
  PANASONIC_AC,
  PIONEER,  // (50)
  LG2,
  MWM,
  DAIKIN2,
  VESTEL_AC,
  TECO,  // (55)
  SAMSUNG36,
  TCL112AC,
  LEGOPF,
  MITSUBISHI_HEAVY_88,
  MITSUBISHI_HEAVY_152,  // 60
  DAIKIN216,
  SHARP_AC,
  GOODWEATHER,
  INAX,
  DAIKIN160,  // 65
  NEOCLIMA,
  DAIKIN176,
  DAIKIN128,
  AMCOR,
  DAIKIN152,  // 70
  MITSUBISHI136,
  MITSUBISHI112,
  HITACHI_AC424,
  // Add new entries before this one, and update it to point to the last entry.
  kLastDecodeType = HITACHI_AC424,
};

// Message lengths & required repeat values
const uint16_t kNoRepeat = 0;
const uint16_t kSingleRepeat = 1;

const uint16_t kAiwaRcT501Bits = 15;
const uint16_t kAiwaRcT501MinRepeats = kSingleRepeat;
const uint16_t kAlokaBits = 32;
const uint16_t kAmcorStateLength = 8;
const uint16_t kAmcorBits = kAmcorStateLength * 8;
const uint16_t kAmcorDefaultRepeat = kSingleRepeat;
const uint16_t kArgoStateLength = 12;
const uint16_t kArgoBits = kArgoStateLength * 8;
const uint16_t kArgoDefaultRepeat = kNoRepeat;
const uint16_t kCoolixBits = 24;
const uint16_t kCoolixDefaultRepeat = kSingleRepeat;
const uint16_t kCarrierAcBits = 32;
const uint16_t kCarrierAcMinRepeat = kNoRepeat;
const uint16_t kDaikinStateLength = 35;
const uint16_t kDaikinBits = kDaikinStateLength * 8;
const uint16_t kDaikinStateLengthShort = kDaikinStateLength - 8;
const uint16_t kDaikinBitsShort = kDaikinStateLengthShort * 8;
const uint16_t kDaikinDefaultRepeat = kNoRepeat;
const uint16_t kDaikin2StateLength = 39;
const uint16_t kDaikin2Bits = kDaikin2StateLength * 8;
const uint16_t kDaikin2DefaultRepeat = kNoRepeat;
const uint16_t kDaikin160StateLength = 20;
const uint16_t kDaikin160Bits = kDaikin160StateLength * 8;
const uint16_t kDaikin160DefaultRepeat = kNoRepeat;
const uint16_t kDaikin128StateLength = 16;
const uint16_t kDaikin128Bits = kDaikin128StateLength * 8;
const uint16_t kDaikin128DefaultRepeat = kNoRepeat;
const uint16_t kDaikin152StateLength = 19;
const uint16_t kDaikin152Bits = kDaikin152StateLength * 8;
const uint16_t kDaikin152DefaultRepeat = kNoRepeat;
const uint16_t kDaikin176StateLength = 22;
const uint16_t kDaikin176Bits = kDaikin176StateLength * 8;
const uint16_t kDaikin176DefaultRepeat = kNoRepeat;
const uint16_t kDaikin216StateLength = 27;
const uint16_t kDaikin216Bits = kDaikin216StateLength * 8;
const uint16_t kDaikin216DefaultRepeat = kNoRepeat;
const uint16_t kDenonBits = 15;
const uint16_t kDenon48Bits = 48;
const uint16_t kDenonLegacyBits = 14;
const uint16_t kDishBits = 16;
const uint16_t kDishMinRepeat = 3;
const uint16_t kElectraAcStateLength = 13;
const uint16_t kElectraAcBits = kElectraAcStateLength * 8;
const uint16_t kElectraAcMinRepeat = kNoRepeat;
const uint16_t kFujitsuAcMinRepeat = kNoRepeat;
const uint16_t kFujitsuAcStateLength = 16;
const uint16_t kFujitsuAcStateLengthShort = 7;
const uint16_t kFujitsuAcBits = kFujitsuAcStateLength * 8;
const uint16_t kFujitsuAcMinBits = (kFujitsuAcStateLengthShort - 1) * 8;
const uint16_t kGicableBits = 16;
const uint16_t kGicableMinRepeat = kSingleRepeat;
const uint16_t kGoodweatherBits = 48;
const uint16_t kGoodweatherMinRepeat = kNoRepeat;
const uint16_t kGreeStateLength = 8;
const uint16_t kGreeBits = kGreeStateLength * 8;
const uint16_t kGreeDefaultRepeat = kNoRepeat;
const uint16_t kHaierACStateLength = 9;
const uint16_t kHaierACBits = kHaierACStateLength * 8;
const uint16_t kHaierAcDefaultRepeat = kNoRepeat;
const uint16_t kHaierACYRW02StateLength = 14;
const uint16_t kHaierACYRW02Bits = kHaierACYRW02StateLength * 8;
const uint16_t kHaierAcYrw02DefaultRepeat = kNoRepeat;
const uint16_t kHitachiAcStateLength = 28;
const uint16_t kHitachiAcBits = kHitachiAcStateLength * 8;
const uint16_t kHitachiAcDefaultRepeat = kNoRepeat;
const uint16_t kHitachiAc1StateLength = 13;
const uint16_t kHitachiAc1Bits = kHitachiAc1StateLength * 8;
const uint16_t kHitachiAc2StateLength = 53;
const uint16_t kHitachiAc2Bits = kHitachiAc2StateLength * 8;
const uint16_t kHitachiAc424StateLength = 53;
const uint16_t kHitachiAc424Bits = kHitachiAc424StateLength * 8;
const uint16_t kInaxBits = 24;
const uint16_t kInaxMinRepeat = kSingleRepeat;
const uint16_t kJvcBits = 16;
const uint16_t kKelvinatorStateLength = 16;
const uint16_t kKelvinatorBits = kKelvinatorStateLength * 8;
const uint16_t kKelvinatorDefaultRepeat = kNoRepeat;
const uint16_t kLasertagBits = 13;
const uint16_t kLasertagMinRepeat = kNoRepeat;
const uint16_t kLegoPfBits = 16;
const uint16_t kLegoPfMinRepeat = kNoRepeat;
const uint16_t kLgBits = 28;
const uint16_t kLg32Bits = 32;
const uint16_t kLgDefaultRepeat = kNoRepeat;
const uint16_t kLutronBits = 35;
const uint16_t kMagiquestBits = 56;
const uint16_t kMideaBits = 48;
const uint16_t kMideaMinRepeat = kNoRepeat;
const uint16_t kMitsubishiBits = 16;
// TODO(anyone): Verify that the Mitsubishi repeat is really needed.
//               Based on marcosamarinho's code.
const uint16_t kMitsubishiMinRepeat = kSingleRepeat;
const uint16_t kMitsubishiACStateLength = 18;
const uint16_t kMitsubishiACBits = kMitsubishiACStateLength * 8;
const uint16_t kMitsubishiACMinRepeat = kSingleRepeat;
const uint16_t kMitsubishi136StateLength = 17;
const uint16_t kMitsubishi136Bits = kMitsubishi136StateLength * 8;
const uint16_t kMitsubishi136MinRepeat = kNoRepeat;
const uint16_t kMitsubishi112StateLength = 14;
const uint16_t kMitsubishi112Bits = kMitsubishi112StateLength * 8;
const uint16_t kMitsubishi112MinRepeat = kNoRepeat;
const uint16_t kMitsubishiHeavy88StateLength = 11;
const uint16_t kMitsubishiHeavy88Bits = kMitsubishiHeavy88StateLength * 8;
const uint16_t kMitsubishiHeavy88MinRepeat = kNoRepeat;
const uint16_t kMitsubishiHeavy152StateLength = 19;
const uint16_t kMitsubishiHeavy152Bits = kMitsubishiHeavy152StateLength * 8;
const uint16_t kMitsubishiHeavy152MinRepeat = kNoRepeat;
const uint16_t kNikaiBits = 24;
const uint16_t kNECBits = 32;
const uint16_t kNeoclimaStateLength = 12;
const uint16_t kNeoclimaBits = kNeoclimaStateLength * 8;
const uint16_t kNeoclimaMinRepeat = kNoRepeat;
const uint16_t kPanasonicBits = 48;
const uint32_t kPanasonicManufacturer = 0x4004;
const uint16_t kPanasonicAcStateLength = 27;
const uint16_t kPanasonicAcStateShortLength = 16;
const uint16_t kPanasonicAcBits = kPanasonicAcStateLength * 8;
const uint16_t kPanasonicAcShortBits = kPanasonicAcStateShortLength * 8;
const uint16_t kPanasonicAcDefaultRepeat = kNoRepeat;
const uint16_t kPioneerBits = 64;
const uint16_t kProntoMinLength = 6;
const uint16_t kRC5RawBits = 14;
const uint16_t kRC5Bits = kRC5RawBits - 2;
const uint16_t kRC5XBits = kRC5RawBits - 1;
const uint16_t kRC6Mode0Bits = 20;  // Excludes the 'start' bit.
const uint16_t kRC6_36Bits = 36;  // Excludes the 'start' bit.
const uint16_t kRCMMBits = 24;
const uint16_t kSamsungBits = 32;
const uint16_t kSamsung36Bits = 36;
const uint16_t kSamsungAcStateLength = 14;
const uint16_t kSamsungAcBits = kSamsungAcStateLength * 8;
const uint16_t kSamsungAcExtendedStateLength = 21;
const uint16_t kSamsungAcExtendedBits = kSamsungAcExtendedStateLength * 8;
const uint16_t kSamsungAcDefaultRepeat = kNoRepeat;
const uint16_t kSanyoSA8650BBits = 12;
const uint16_t kSanyoLC7461AddressBits = 13;
const uint16_t kSanyoLC7461CommandBits = 8;
const uint16_t kSanyoLC7461Bits = (kSanyoLC7461AddressBits +
                                   kSanyoLC7461CommandBits) * 2;
const uint8_t  kSharpAddressBits = 5;
const uint8_t  kSharpCommandBits = 8;
const uint16_t kSharpBits = kSharpAddressBits + kSharpCommandBits + 2;  // 15
const uint16_t kSharpAcStateLength = 13;
const uint16_t kSharpAcBits = kSharpAcStateLength * 8;  // 104
const uint16_t kSharpAcDefaultRepeat = kNoRepeat;
const uint8_t  kSherwoodBits = kNECBits;
const uint16_t kSherwoodMinRepeat = kSingleRepeat;
const uint16_t kSony12Bits = 12;
const uint16_t kSony15Bits = 15;
const uint16_t kSony20Bits = 20;
const uint16_t kSonyMinBits = 12;
const uint16_t kSonyMinRepeat = 2;
const uint16_t kTcl112AcStateLength = 14;
const uint16_t kTcl112AcBits = kTcl112AcStateLength * 8;
const uint16_t kTcl112AcDefaultRepeat = kNoRepeat;
const uint16_t kTecoBits = 35;
const uint16_t kTecoDefaultRepeat = kNoRepeat;
const uint16_t kToshibaACStateLength = 9;
const uint16_t kToshibaACBits = kToshibaACStateLength * 8;
const uint16_t kToshibaACMinRepeat = kSingleRepeat;
const uint16_t kTrotecStateLength = 9;
const uint16_t kTrotecBits = kTrotecStateLength * 8;
const uint16_t kTrotecDefaultRepeat = kNoRepeat;
const uint16_t kWhirlpoolAcStateLength = 21;
const uint16_t kWhirlpoolAcBits = kWhirlpoolAcStateLength * 8;
const uint16_t kWhirlpoolAcDefaultRepeat = kNoRepeat;
const uint16_t kWhynterBits = 32;
const uint8_t  kVestelAcBits = 56;


// Legacy defines. (Deprecated)
#define AIWA_RC_T501_BITS             kAiwaRcT501Bits
#define ARGO_COMMAND_LENGTH           kArgoStateLength
#define COOLIX_BITS                   kCoolixBits
#define CARRIER_AC_BITS               kCarrierAcBits
#define DAIKIN_COMMAND_LENGTH         kDaikinStateLength
#define DENON_BITS                    kDenonBits
#define DENON_48_BITS                 kDenon48Bits
#define DENON_LEGACY_BITS             kDenonLegacyBits
#define DISH_BITS                     kDishBits
#define FUJITSU_AC_MIN_REPEAT         kFujitsuAcMinRepeat
#define FUJITSU_AC_STATE_LENGTH       kFujitsuAcStateLength
#define FUJITSU_AC_STATE_LENGTH_SHORT kFujitsuAcStateLengthShort
#define FUJITSU_AC_BITS               kFujitsuAcBits
#define FUJITSU_AC_MIN_BITS           kFujitsuAcMinBits
#define GICABLE_BITS                  kGicableBits
#define GREE_STATE_LENGTH             kGreeStateLength
#define HAIER_AC_STATE_LENGTH         kHaierACStateLength
#define HAIER_AC_YRW02_STATE_LENGTH   kHaierACYRW02StateLength
#define HITACHI_AC_STATE_LENGTH       kHitachiAcStateLength
#define HITACHI_AC_BITS               kHitachiAcBits
#define HITACHI_AC1_STATE_LENGTH      kHitachiAc1StateLength
#define HITACHI_AC1_BITS              kHitachiAc1Bits
#define HITACHI_AC2_STATE_LENGTH      kHitachiAc2StateLength
#define HITACHI_AC2_BITS              kHitachiAc2Bits
#define JVC_BITS                      kJvcBits
#define KELVINATOR_STATE_LENGTH       kKelvinatorStateLength
#define LASERTAG_BITS                 kLasertagBits
#define LG_BITS                       kLgBits
#define LG32_BITS                     kLg32Bits
#define MAGIQUEST_BITS                kMagiquestBits
#define MIDEA_BITS                    kMideaBits
#define MITSUBISHI_BITS               kMitsubishiBits
#define MITSUBISHI_AC_STATE_LENGTH    kMitsubishiACStateLength
#define NEC_BITS                      kNECBits
#define NIKAI_BITS                    kNikaiBits
#define PANASONIC_BITS                kPanasonicBits
#define RC5_BITS                      kRC5Bits
#define RC5X_BITS                     kRC5XBits
#define RC6_MODE0_BITS                kRC6Mode0Bits
#define RC6_36_BITS                   kRC6_36Bits
#define RCMM_BITS                     kRCMMBits
#define SANYO_LC7461_BITS             kSanyoLC7461Bits
#define SAMSUNG_BITS                  kSamsungBits
#define SANYO_SA8650B_BITS            kSanyoSA8650BBits
#define SHARP_BITS                    kSharpBits
#define SHERWOOD_BITS                 kSherwoodBits
#define SONY_12_BITS                  kSony12Bits
#define SONY_15_BITS                  kSony15Bits
#define SONY_20_BITS                  kSony20Bits
#define TOSHIBA_AC_STATE_LENGTH       kToshibaACStateLength
#define TROTEC_COMMAND_LENGTH         kTrotecStateLength
#define WHYNTER_BITS                  kWhynterBits

// Turn on Debugging information by uncommenting the following line.
// #define DEBUG 1

#ifdef DEBUG
#ifdef UNIT_TEST
#define DPRINT(x) do { std::cout << x; } while (0)
#define DPRINTLN(x) do { std::cout << x << std::endl; } while (0)
#endif  // UNIT_TEST
#ifdef ARDUINO
#define DPRINT(x) do { Serial.print(x); } while (0)
#define DPRINTLN(x) do { Serial.println(x); } while (0)
#endif  // ARDUINO
#else  // DEBUG
#define DPRINT(x)
#define DPRINTLN(x)
#endif  // DEBUG

#ifdef UNIT_TEST
#ifndef F
// Create a no-op F() macro so the code base still compiles outside of the
// Arduino framework. Thus we can safely use the Arduino 'F()' macro through-out
// the code base. That macro stores constants in Flash (PROGMEM) memory.
// See: https://github.com/crankyoldgit/IRremoteESP8266/issues/667
#define F(x) x
#endif  // F
typedef std::string String;
#endif  // UNIT_TEST

#endif  // IRREMOTEESP8266_H_
