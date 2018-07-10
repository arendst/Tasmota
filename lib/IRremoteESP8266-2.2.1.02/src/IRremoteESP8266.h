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
 * Mitsubishi A/C added by crankyoldgit
 *     (derived from https://github.com/r45635/HVAC-IR-Control)
 * DISH decode by marcosamarinho
 * Gree Heatpump sending added by Ville Skyttä (scop)
 *     (derived from https://github.com/ToniA/arduino-heatpumpir/blob/master/GreeHeatpumpIR.cpp)
 * Updated by markszabo (https://github.com/markszabo/IRremoteESP8266) for sending IR code on ESP8266
 * Updated by Sebastien Warin (http://sebastien.warin.fr) for receiving IR code on ESP8266
 *
 *  Updated by sillyfrog for Daikin, adopted from
 * (https://github.com/mharizanov/Daikin-AC-remote-control-over-the-Internet/)
 * Fujitsu A/C code added by jonnygraham
 * Trotec AC code by stufisher
 *  GPL license, all text above must be included in any redistribution
 ****************************************************/

#ifndef IRREMOTEESP8266_H_
#define IRREMOTEESP8266_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#ifdef UNIT_TEST
#include <iostream>
#endif

// Library Version
#define _IRREMOTEESP8266_VERSION_ "2.2.1"
// Supported IR protocols
// Each protocol you include costs memory and, during decode, costs time
// Disable (set to false) all the protocols you do not need/want!
//
/*
#define DECODE_NEC           true
#define SEND_NEC             true

#define DECODE_SHERWOOD      true  // Doesn't exist. Actually is DECODE_NEC
#define SEND_SHERWOOD        true

#define DECODE_RC5           true
#define SEND_RC5             true

#define DECODE_RC6           true
#define SEND_RC6             true

#define DECODE_RCMM          true
#define SEND_RCMM            true

#define DECODE_SONY          true
#define SEND_SONY            true

#define DECODE_PANASONIC     true
#define SEND_PANASONIC       true

#define DECODE_JVC           true
#define SEND_JVC             true

#define DECODE_SAMSUNG       true
#define SEND_SAMSUNG         true

#define DECODE_WHYNTER       true
#define SEND_WHYNTER         true

#define DECODE_AIWA_RC_T501  true
#define SEND_AIWA_RC_T501    true

#define DECODE_LG            true
#define SEND_LG              true

#define DECODE_SANYO         true
#define SEND_SANYO           true

#define DECODE_MITSUBISHI    true
#define SEND_MITSUBISHI      true

#define DECODE_DISH          true
#define SEND_DISH            true

#define DECODE_SHARP         true
#define SEND_SHARP           true

#define DECODE_DENON         true
#define SEND_DENON           true

#define DECODE_KELVINATOR    false  // Not written.
#define SEND_KELVINATOR      true

#define DECODE_MITSUBISHI_AC false  // Not written.
#define SEND_MITSUBISHI_AC   true

#define DECODE_FUJITSU_AC false  // Not written.
#define SEND_FUJITSU_AC   true

#define DECODE_DAIKIN        false  // Not finished.
#define SEND_DAIKIN          true

#define DECODE_COOLIX        true
#define SEND_COOLIX          true

#define DECODE_GLOBALCACHE   false  // Not written.
#define SEND_GLOBALCACHE     true

#define DECODE_GREE          false  // Not written.
#define SEND_GREE            true

#define DECODE_PRONTO        false  // Not written.
#define SEND_PRONTO          true

#define DECODE_ARGO          false  // Not written.
#define SEND_ARGO            true

#define DECODE_TROTEC        false  // Not implemented.
#define SEND_TROTEC          true

#define DECODE_NIKAI     true
#define SEND_NIKAI       true
*/

// Tasmota supported protocols (less protocols is less code size)
#define DECODE_NEC           true
#define SEND_NEC             true

#define DECODE_SHERWOOD      false  // Doesn't exist. Actually is DECODE_NEC
#define SEND_SHERWOOD        false

#define DECODE_RC5           true
#define SEND_RC5             true

#define DECODE_RC6           true
#define SEND_RC6             true

#define DECODE_RCMM          false
#define SEND_RCMM            false

#define DECODE_SONY          true
#define SEND_SONY            true

#define DECODE_PANASONIC     true
#define SEND_PANASONIC       true

#define DECODE_JVC           true
#define SEND_JVC             true

#define DECODE_SAMSUNG       true
#define SEND_SAMSUNG         true

#define DECODE_WHYNTER       false
#define SEND_WHYNTER         false

#define DECODE_AIWA_RC_T501  false
#define SEND_AIWA_RC_T501    false

#define DECODE_LG            false
#define SEND_LG              false

#define DECODE_SANYO         false
#define SEND_SANYO           false

#define DECODE_MITSUBISHI    false
#define SEND_MITSUBISHI      false

#define DECODE_DISH          false
#define SEND_DISH            true

#define DECODE_SHARP         false
#define SEND_SHARP           false

#define DECODE_DENON         false
#define SEND_DENON           false

#define DECODE_KELVINATOR    false  // Not written.
#define SEND_KELVINATOR      false

#define DECODE_MITSUBISHI_AC false  // Not written.
#define SEND_MITSUBISHI_AC   true

#define DECODE_FUJITSU_AC    false  // Not written.
#define SEND_FUJITSU_AC      false

#define DECODE_DAIKIN        false  // Not finished.
#define SEND_DAIKIN          false

#define DECODE_COOLIX        false
#define SEND_COOLIX          false

#define DECODE_GLOBALCACHE   false  // Not written.
#define SEND_GLOBALCACHE     false

#define DECODE_GREE          false  // Not written.
#define SEND_GREE            false

#define DECODE_PRONTO        false  // Not written.
#define SEND_PRONTO          false

#define DECODE_ARGO          false  // Not written.
#define SEND_ARGO            false

#define DECODE_TROTEC        false  // Not implemented.
#define SEND_TROTEC          false

#define DECODE_NIKAI         false
#define SEND_NIKAI           false

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
  PANASONIC,
  JVC,
  SAMSUNG,
  WHYNTER,
  AIWA_RC_T501,
  LG,
  SANYO,
  MITSUBISHI,
  DISH,
  SHARP,
  COOLIX,
  DAIKIN,
  DENON,
  KELVINATOR,
  SHERWOOD,
  MITSUBISHI_AC,
  RCMM,
  SANYO_LC7461,
  RC5X,
  GREE,
  PRONTO,  // Technically not a protocol, but an encoding.
  NEC_LIKE,
  ARGO,
  TROTEC,
  NIKAI,
  RAW,  // Technically not a protocol, but an encoding.
  GLOBALCACHE  // Technically not a protocol, but an encoding.
};

// Message lengths & required repeat values
#define AIWA_RC_T501_BITS           15U
#define AIWA_RC_T501_MIN_REPEAT      1U
#define COOLIX_BITS                 24U
#define DAIKIN_BITS                 99U
#define DAIKIN_COMMAND_LENGTH       27U
#define DENON_BITS                  SHARP_BITS
#define DENON_48_BITS               PANASONIC_BITS
#define DENON_LEGACY_BITS           14U
#define DISH_BITS                   16U
#define DISH_MIN_REPEAT              3U
#define GREE_STATE_LENGTH            8U
#define GREE_BITS                   (GREE_STATE_LENGTH * 8)
#define JVC_BITS                    16U
#define KELVINATOR_STATE_LENGTH     16U
#define LG_BITS                     28U
#define LG32_BITS                   32U
#define MITSUBISHI_BITS             16U
// TODO(anyone): Verify that the Mitsubishi repeat is really needed.
#define MITSUBISHI_MIN_REPEAT        1U  // Based on marcosamarinho's code.
#define MITSUBISHI_AC_STATE_LENGTH  18U
#define MITSUBISHI_AC_MIN_REPEAT     1U
#define FUJITSU_AC_MIN_REPEAT        0U
#define NEC_BITS                    32U
#define PANASONIC_BITS              48U
#define PANASONIC_MANUFACTURER   0x4004ULL
#define PRONTO_MIN_LENGTH            6U
#define RC5_RAW_BITS                14U
#define RC5_BITS      RC5_RAW_BITS - 2U
#define RC5X_BITS     RC5_RAW_BITS - 1U
#define RC6_MODE0_BITS              20U  // Excludes the 'start' bit.
#define RC6_36_BITS                 36U  // Excludes the 'start' bit.
#define RCMM_BITS                   24U
#define SAMSUNG_BITS                32U
#define SANYO_SA8650B_BITS          12U
#define SANYO_LC7461_ADDRESS_BITS   13U
#define SANYO_LC7461_COMMAND_BITS    8U
#define SANYO_LC7461_BITS           ((SANYO_LC7461_ADDRESS_BITS + \
                                     SANYO_LC7461_COMMAND_BITS) * 2)
#define SHARP_ADDRESS_BITS           5U
#define SHARP_COMMAND_BITS           8U
#define SHARP_BITS (SHARP_ADDRESS_BITS + SHARP_COMMAND_BITS + 2)  // 15U
#define SHERWOOD_BITS          NEC_BITS
#define SHERWOOD_MIN_REPEAT          1U
#define SONY_12_BITS                12U
#define SONY_15_BITS                15U
#define SONY_20_BITS                20U
#define SONY_MIN_BITS      SONY_12_BITS
#define SONY_MIN_REPEAT              2U
#define TROTEC_COMMAND_LENGTH        9U
#define WHYNTER_BITS                32U
#define ARGO_COMMAND_LENGTH         12U
#define NIKAI_BITS                  24U

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

#endif  // IRREMOTEESP8266_H_
