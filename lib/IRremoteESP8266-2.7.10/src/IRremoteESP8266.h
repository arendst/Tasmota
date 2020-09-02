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
#define _IRREMOTEESP8266_VERSION_ "2.7.10"

// Set the language & locale for the library. See the `locale` dir for options.
#ifndef _IR_LOCALE_
#define _IR_LOCALE_ en-AU
#endif  // _IR_LOCALE_

// Do we enable all the protocols by default (true), or disable them (false)?
// This allows users of the library to disable or enable all protocols at
// compile-time with `-D_IR_ENABLE_DEFAULT_=true` or
// `-D_IR_ENABLE_DEFAULT_=false` compiler flags respectively.
// Everything is included by default.
// e.g. If you only want to enable use of he NEC protocol to save program space,
//      you would use something like:
//        `-D_IR_ENABLE_DEFAULT_=false -DDECODE_NEC=true -DSEND_NEC=true`
//
//      or alter your 'platform.ini' file accordingly:
//        ```
//        build_flags = -D_IR_ENABLE_DEFAULT_=false
//                      -DDECODE_NEC=true
//                      -DSEND_NEC=true
//        ```
//      If you want to enable support for every protocol *except* _decoding_ the
//      Kelvinator protocol, you would use:
//        `-DDECODE_KELVINATOR=false`
#ifndef _IR_ENABLE_DEFAULT_
#define _IR_ENABLE_DEFAULT_ true  // Unless set externally, the default is on.
#endif  // _IR_ENABLE_DEFAULT_

// Supported IR protocols
// Each protocol you include costs memory and, during decode, costs time
// Disable (set to false) all the protocols you do not need/want!
// The Air Conditioner protocols are the most expensive memory-wise.
//

// Semi-unique code for unknown messages
#ifndef DECODE_HASH
#define DECODE_HASH            _IR_ENABLE_DEFAULT_
#endif  // DECODE_HASH

#ifndef SEND_RAW
#define SEND_RAW               _IR_ENABLE_DEFAULT_
#endif  // SEND_RAW

#ifndef DECODE_NEC
#define DECODE_NEC             _IR_ENABLE_DEFAULT_
#endif  // DECODE_NEC
#ifndef SEND_NEC
#define SEND_NEC               _IR_ENABLE_DEFAULT_
#endif  // SEND_NEC

#ifndef DECODE_SHERWOOD
#define DECODE_SHERWOOD        false  // Not applicable. Actually is DECODE_NEC
#endif  // DECODE_SHERWOOD
#ifndef SEND_SHERWOOD
#define SEND_SHERWOOD          _IR_ENABLE_DEFAULT_
#endif  // SEND_SHERWOOD

#ifndef DECODE_RC5
#define DECODE_RC5             _IR_ENABLE_DEFAULT_
#endif  // DECODE_RC5
#ifndef SEND_RC5
#define SEND_RC5               _IR_ENABLE_DEFAULT_
#endif  // SEND_RC5

#ifndef DECODE_RC6
#define DECODE_RC6             _IR_ENABLE_DEFAULT_
#endif  // DECODE_RC6
#ifndef SEND_RC6
#define SEND_RC6               _IR_ENABLE_DEFAULT_
#endif  // SEND_RC6

#ifndef DECODE_RCMM
#define DECODE_RCMM            _IR_ENABLE_DEFAULT_
#endif  // DECODE_RCMM
#ifndef SEND_RCMM
#define SEND_RCMM              _IR_ENABLE_DEFAULT_
#endif  // SEND_RCMM

#ifndef DECODE_SONY
#define DECODE_SONY            _IR_ENABLE_DEFAULT_
#endif  // DECODE_SONY
#ifndef SEND_SONY
#define SEND_SONY              _IR_ENABLE_DEFAULT_
#endif  // SEND_SONY

#ifndef DECODE_PANASONIC
#define DECODE_PANASONIC       _IR_ENABLE_DEFAULT_
#endif  // DECODE_PANASONIC
#ifndef SEND_PANASONIC
#define SEND_PANASONIC         _IR_ENABLE_DEFAULT_
#endif  // SEND_PANASONIC

#ifndef DECODE_JVC
#define DECODE_JVC             _IR_ENABLE_DEFAULT_
#endif  // DECODE_JVC
#ifndef SEND_JVC
#define SEND_JVC               _IR_ENABLE_DEFAULT_
#endif  // SEND_JVC

#ifndef DECODE_SAMSUNG
#define DECODE_SAMSUNG         _IR_ENABLE_DEFAULT_
#endif  // DECODE_SAMSUNG
#ifndef SEND_SAMSUNG
#define SEND_SAMSUNG           _IR_ENABLE_DEFAULT_
#endif  // SEND_SAMSUNG

#ifndef DECODE_SAMSUNG36
#define DECODE_SAMSUNG36       _IR_ENABLE_DEFAULT_
#endif  // DECODE_SAMSUNG36
#ifndef SEND_SAMSUNG36
#define SEND_SAMSUNG36         _IR_ENABLE_DEFAULT_
#endif  // SEND_SAMSUNG36

#ifndef DECODE_SAMSUNG_AC
#define DECODE_SAMSUNG_AC      _IR_ENABLE_DEFAULT_
#endif  // DECODE_SAMSUNG_AC
#ifndef SEND_SAMSUNG_AC
#define SEND_SAMSUNG_AC        _IR_ENABLE_DEFAULT_
#endif  // SEND_SAMSUNG_AC

#ifndef DECODE_WHYNTER
#define DECODE_WHYNTER         _IR_ENABLE_DEFAULT_
#endif  // DECODE_WHYNTER
#ifndef SEND_WHYNTER
#define SEND_WHYNTER           _IR_ENABLE_DEFAULT_
#endif  // SEND_WHYNTER

#ifndef DECODE_AIWA_RC_T501
#define DECODE_AIWA_RC_T501    _IR_ENABLE_DEFAULT_
#endif  // DECODE_AIWA_RC_T501
#ifndef SEND_AIWA_RC_T501
#define SEND_AIWA_RC_T501      _IR_ENABLE_DEFAULT_
#endif  // SEND_AIWA_RC_T501

#ifndef DECODE_LG
#define DECODE_LG              _IR_ENABLE_DEFAULT_
#endif  // DECODE_LG
#ifndef SEND_LG
#define SEND_LG                _IR_ENABLE_DEFAULT_
#endif  // SEND_LG

#ifndef DECODE_SANYO
#define DECODE_SANYO           _IR_ENABLE_DEFAULT_
#endif  // DECODE_SANYO
#ifndef SEND_SANYO
#define SEND_SANYO             _IR_ENABLE_DEFAULT_
#endif  // SEND_SANYO

#ifndef DECODE_SANYO_AC
#define DECODE_SANYO_AC        _IR_ENABLE_DEFAULT_
#endif  // DECODE_SANYO_AC
#ifndef SEND_SANYO_AC
#define SEND_SANYO_AC          _IR_ENABLE_DEFAULT_
#endif  // SEND_SANYO_AC

#ifndef DECODE_MITSUBISHI
#define DECODE_MITSUBISHI      _IR_ENABLE_DEFAULT_
#endif  // DECODE_MITSUBISHI
#ifndef SEND_MITSUBISHI
#define SEND_MITSUBISHI        _IR_ENABLE_DEFAULT_
#endif  // SEND_MITSUBISHI

#ifndef DECODE_MITSUBISHI2
#define DECODE_MITSUBISHI2     _IR_ENABLE_DEFAULT_
#endif  // DECODE_MITSUBISHI2
#ifndef SEND_MITSUBISHI2
#define SEND_MITSUBISHI2       _IR_ENABLE_DEFAULT_
#endif  // SEND_MITSUBISHI2

#ifndef DECODE_DISH
#define DECODE_DISH            _IR_ENABLE_DEFAULT_
#endif  // DECODE_DISH
#ifndef SEND_DISH
#define SEND_DISH              _IR_ENABLE_DEFAULT_
#endif  // SEND_DISH

#ifndef DECODE_SHARP
#define DECODE_SHARP           _IR_ENABLE_DEFAULT_
#endif  // DECODE_SHARP
#ifndef SEND_SHARP
#define SEND_SHARP             _IR_ENABLE_DEFAULT_
#endif  // SEND_SHARP

#ifndef DECODE_SHARP_AC
#define DECODE_SHARP_AC        _IR_ENABLE_DEFAULT_
#endif  // DECODE_SHARP_AC
#ifndef SEND_SHARP_AC
#define SEND_SHARP_AC          _IR_ENABLE_DEFAULT_
#endif  // SEND_SHARP_AC

#ifndef DECODE_DENON
#define DECODE_DENON           _IR_ENABLE_DEFAULT_
#endif  // DECODE_DENON
#ifndef SEND_DENON
#define SEND_DENON             _IR_ENABLE_DEFAULT_
#endif  // SEND_DENON

#ifndef DECODE_KELVINATOR
#define DECODE_KELVINATOR      _IR_ENABLE_DEFAULT_
#endif  // DECODE_KELVINATOR
#ifndef SEND_KELVINATOR
#define SEND_KELVINATOR        _IR_ENABLE_DEFAULT_
#endif  // SEND_KELVINATOR

#ifndef DECODE_MITSUBISHI_AC
#define DECODE_MITSUBISHI_AC   _IR_ENABLE_DEFAULT_
#endif  // DECODE_MITSUBISHI_AC
#ifndef SEND_MITSUBISHI_AC
#define SEND_MITSUBISHI_AC     _IR_ENABLE_DEFAULT_
#endif  // SEND_MITSUBISHI_AC

#ifndef DECODE_MITSUBISHI136
#define DECODE_MITSUBISHI136   _IR_ENABLE_DEFAULT_
#endif  // DECODE_MITSUBISHI136
#ifndef SEND_MITSUBISHI136
#define SEND_MITSUBISHI136     _IR_ENABLE_DEFAULT_
#endif  // SEND_MITSUBISHI136

#ifndef DECODE_MITSUBISHI112
#define DECODE_MITSUBISHI112   _IR_ENABLE_DEFAULT_
#endif  // DECODE_MITSUBISHI112
#ifndef SEND_MITSUBISHI112
#define SEND_MITSUBISHI112     _IR_ENABLE_DEFAULT_
#endif  // SEND_MITSUBISHI112

#ifndef DECODE_FUJITSU_AC
#define DECODE_FUJITSU_AC      _IR_ENABLE_DEFAULT_
#endif  // DECODE_FUJITSU_AC
#ifndef SEND_FUJITSU_AC
#define SEND_FUJITSU_AC        _IR_ENABLE_DEFAULT_
#endif  // SEND_FUJITSU_AC

#ifndef DECODE_INAX
#define DECODE_INAX            _IR_ENABLE_DEFAULT_
#endif  // DECODE_INAX
#ifndef SEND_INAX
#define SEND_INAX              _IR_ENABLE_DEFAULT_
#endif  // SEND_INAX

#ifndef DECODE_DAIKIN
#define DECODE_DAIKIN          _IR_ENABLE_DEFAULT_
#endif  // DECODE_DAIKIN
#ifndef SEND_DAIKIN
#define SEND_DAIKIN            _IR_ENABLE_DEFAULT_
#endif  // SEND_DAIKIN

#ifndef DECODE_COOLIX
#define DECODE_COOLIX          _IR_ENABLE_DEFAULT_
#endif  // DECODE_COOLIX
#ifndef SEND_COOLIX
#define SEND_COOLIX            _IR_ENABLE_DEFAULT_
#endif  // SEND_COOLIX

#ifndef DECODE_GLOBALCACHE
#define DECODE_GLOBALCACHE     false  // Not applicable.
#endif  // DECODE_GLOBALCACHE
#ifndef SEND_GLOBALCACHE
#define SEND_GLOBALCACHE       _IR_ENABLE_DEFAULT_
#endif  // SEND_GLOBALCACHE

#ifndef DECODE_GOODWEATHER
#define DECODE_GOODWEATHER     _IR_ENABLE_DEFAULT_
#endif  // DECODE_GOODWEATHER
#ifndef SEND_GOODWEATHER
#define SEND_GOODWEATHER       _IR_ENABLE_DEFAULT_
#endif  // SEND_GOODWEATHER

#ifndef DECODE_GREE
#define DECODE_GREE            _IR_ENABLE_DEFAULT_
#endif  // DECODE_GREE
#ifndef SEND_GREE
#define SEND_GREE              _IR_ENABLE_DEFAULT_
#endif  // SEND_GREE

#ifndef DECODE_PRONTO
#define DECODE_PRONTO          false  // Not applicable.
#endif  // DECODE_PRONTO
#ifndef SEND_PRONTO
#define SEND_PRONTO            _IR_ENABLE_DEFAULT_
#endif  // SEND_PRONTO

#ifndef DECODE_ARGO
#define DECODE_ARGO            _IR_ENABLE_DEFAULT_
#endif  // DECODE_ARGO
#ifndef SEND_ARGO
#define SEND_ARGO              _IR_ENABLE_DEFAULT_
#endif  // SEND_ARGO

#ifndef DECODE_TROTEC
#define DECODE_TROTEC          _IR_ENABLE_DEFAULT_
#endif  // DECODE_TROTEC
#ifndef SEND_TROTEC
#define SEND_TROTEC            _IR_ENABLE_DEFAULT_
#endif  // SEND_TROTEC

#ifndef DECODE_NIKAI
#define DECODE_NIKAI           _IR_ENABLE_DEFAULT_
#endif  // DECODE_NIKAI
#ifndef SEND_NIKAI
#define SEND_NIKAI             _IR_ENABLE_DEFAULT_
#endif  // SEND_NIKAI

#ifndef DECODE_TOSHIBA_AC
#define DECODE_TOSHIBA_AC      _IR_ENABLE_DEFAULT_
#endif  // DECODE_TOSHIBA_AC
#ifndef SEND_TOSHIBA_AC
#define SEND_TOSHIBA_AC        _IR_ENABLE_DEFAULT_
#endif  // SEND_TOSHIBA_AC

#ifndef DECODE_MAGIQUEST
#define DECODE_MAGIQUEST       _IR_ENABLE_DEFAULT_
#endif  // DECODE_MAGIQUEST
#ifndef SEND_MAGIQUEST
#define SEND_MAGIQUEST         _IR_ENABLE_DEFAULT_
#endif  // SEND_MAGIQUEST

#ifndef DECODE_MIDEA
#define DECODE_MIDEA           _IR_ENABLE_DEFAULT_
#endif  // DECODE_MIDEA
#ifndef SEND_MIDEA
#define SEND_MIDEA             _IR_ENABLE_DEFAULT_
#endif  // SEND_MIDEA

#ifndef DECODE_MIDEA24
#define DECODE_MIDEA24         _IR_ENABLE_DEFAULT_
#endif  // DECODE_MIDEA24
#ifndef SEND_MIDEA24
#define SEND_MIDEA24           _IR_ENABLE_DEFAULT_
#endif  // SEND_MIDEA24

#ifndef DECODE_LASERTAG
#define DECODE_LASERTAG        _IR_ENABLE_DEFAULT_
#endif  // DECODE_LASERTAG
#ifndef SEND_LASERTAG
#define SEND_LASERTAG          _IR_ENABLE_DEFAULT_
#endif  // SEND_LASERTAG

#ifndef DECODE_CARRIER_AC
#define DECODE_CARRIER_AC      _IR_ENABLE_DEFAULT_
#endif  // DECODE_CARRIER_AC
#ifndef SEND_CARRIER_AC
#define SEND_CARRIER_AC        _IR_ENABLE_DEFAULT_
#endif  // SEND_CARRIER_AC

#ifndef DECODE_CARRIER_AC40
#define DECODE_CARRIER_AC40    _IR_ENABLE_DEFAULT_
#endif  // DECODE_CARRIER_AC40
#ifndef SEND_CARRIER_AC40
#define SEND_CARRIER_AC40      _IR_ENABLE_DEFAULT_
#endif  // SEND_CARRIER_AC40

#ifndef DECODE_CARRIER_AC64
#define DECODE_CARRIER_AC64    _IR_ENABLE_DEFAULT_
#endif  // DECODE_CARRIER_AC64
#ifndef SEND_CARRIER_AC64
#define SEND_CARRIER_AC64      _IR_ENABLE_DEFAULT_
#endif  // SEND_CARRIER_AC64

#ifndef DECODE_HAIER_AC
#define DECODE_HAIER_AC        _IR_ENABLE_DEFAULT_
#endif  // DECODE_HAIER_AC
#ifndef SEND_HAIER_AC
#define SEND_HAIER_AC          _IR_ENABLE_DEFAULT_
#endif  // SEND_HAIER_AC

#ifndef DECODE_HITACHI_AC
#define DECODE_HITACHI_AC      _IR_ENABLE_DEFAULT_
#endif  // DECODE_HITACHI_AC
#ifndef SEND_HITACHI_AC
#define SEND_HITACHI_AC        _IR_ENABLE_DEFAULT_
#endif  // SEND_HITACHI_AC

#ifndef DECODE_HITACHI_AC1
#define DECODE_HITACHI_AC1     _IR_ENABLE_DEFAULT_
#endif  // DECODE_HITACHI_AC1
#ifndef SEND_HITACHI_AC1
#define SEND_HITACHI_AC1       _IR_ENABLE_DEFAULT_
#endif  // SEND_HITACHI_AC1

#ifndef DECODE_HITACHI_AC2
#define DECODE_HITACHI_AC2     _IR_ENABLE_DEFAULT_
#endif  // DECODE_HITACHI_AC2
#ifndef SEND_HITACHI_AC2
#define SEND_HITACHI_AC2       _IR_ENABLE_DEFAULT_
#endif  // SEND_HITACHI_AC2

#ifndef DECODE_HITACHI_AC3
#define DECODE_HITACHI_AC3     _IR_ENABLE_DEFAULT_
#endif  // DECODE_HITACHI_AC3
#ifndef SEND_HITACHI_AC3
#define SEND_HITACHI_AC3       _IR_ENABLE_DEFAULT_
#endif  // SEND_HITACHI_AC3

#ifndef DECODE_HITACHI_AC344
#define DECODE_HITACHI_AC344   _IR_ENABLE_DEFAULT_
#endif  // DECODE_HITACHI_AC344
#ifndef SEND_HITACHI_AC344
#define SEND_HITACHI_AC344     _IR_ENABLE_DEFAULT_
#endif  // SEND_HITACHI_AC344

#ifndef DECODE_HITACHI_AC424
#define DECODE_HITACHI_AC424   _IR_ENABLE_DEFAULT_
#endif  // DECODE_HITACHI_AC424
#ifndef SEND_HITACHI_AC424
#define SEND_HITACHI_AC424     _IR_ENABLE_DEFAULT_
#endif  // SEND_HITACHI_AC424

#ifndef DECODE_GICABLE
#define DECODE_GICABLE         _IR_ENABLE_DEFAULT_
#endif  // DECODE_GICABLE
#ifndef SEND_GICABLE
#define SEND_GICABLE           _IR_ENABLE_DEFAULT_
#endif  // SEND_GICABLE

#ifndef DECODE_HAIER_AC_YRW02
#define DECODE_HAIER_AC_YRW02  _IR_ENABLE_DEFAULT_
#endif  // DECODE_HAIER_AC_YRW02
#ifndef SEND_HAIER_AC_YRW02
#define SEND_HAIER_AC_YRW02    _IR_ENABLE_DEFAULT_
#endif  // SEND_HAIER_AC_YRW02

#ifndef DECODE_WHIRLPOOL_AC
#define DECODE_WHIRLPOOL_AC    _IR_ENABLE_DEFAULT_
#endif  // DECODE_WHIRLPOOL_AC
#ifndef SEND_WHIRLPOOL_AC
#define SEND_WHIRLPOOL_AC      _IR_ENABLE_DEFAULT_
#endif  // SEND_WHIRLPOOL_AC

#ifndef DECODE_LUTRON
#define DECODE_LUTRON          _IR_ENABLE_DEFAULT_
#endif  // DECODE_LUTRON
#ifndef SEND_LUTRON
#define SEND_LUTRON            _IR_ENABLE_DEFAULT_
#endif  // SEND_LUTRON

#ifndef DECODE_ELECTRA_AC
#define DECODE_ELECTRA_AC      _IR_ENABLE_DEFAULT_
#endif  // DECODE_ELECTRA_AC
#ifndef SEND_ELECTRA_AC
#define SEND_ELECTRA_AC        _IR_ENABLE_DEFAULT_
#endif  // SEND_ELECTRA_AC

#ifndef DECODE_PANASONIC_AC
#define DECODE_PANASONIC_AC    _IR_ENABLE_DEFAULT_
#endif  // DECODE_PANASONIC_AC
#ifndef SEND_PANASONIC_AC
#define SEND_PANASONIC_AC      _IR_ENABLE_DEFAULT_
#endif  // SEND_PANASONIC_AC

#ifndef DECODE_MWM
#define DECODE_MWM             _IR_ENABLE_DEFAULT_
#endif  // DECODE_MWM
#ifndef SEND_MWM
#define SEND_MWM               _IR_ENABLE_DEFAULT_
#endif  // SEND_MWM

#ifndef DECODE_PIONEER
#define DECODE_PIONEER         _IR_ENABLE_DEFAULT_
#endif  // DECODE_PIONEER
#ifndef SEND_PIONEER
#define SEND_PIONEER           _IR_ENABLE_DEFAULT_
#endif  // SEND_PIONEER

#ifndef DECODE_DAIKIN2
#define DECODE_DAIKIN2         _IR_ENABLE_DEFAULT_
#endif  // DECODE_DAIKIN2
#ifndef SEND_DAIKIN2
#define SEND_DAIKIN2           _IR_ENABLE_DEFAULT_
#endif  // SEND_DAIKIN2

#ifndef DECODE_VESTEL_AC
#define DECODE_VESTEL_AC       _IR_ENABLE_DEFAULT_
#endif  // DECODE_VESTEL_AC
#ifndef SEND_VESTEL_AC
#define SEND_VESTEL_AC         _IR_ENABLE_DEFAULT_
#endif  // SEND_VESTEL_AC

#ifndef DECODE_TECO
#define DECODE_TECO            _IR_ENABLE_DEFAULT_
#endif  // DECODE_TECO
#ifndef SEND_TECO
#define SEND_TECO              _IR_ENABLE_DEFAULT_
#endif  // SEND_TECO

#ifndef DECODE_TCL112AC
#define DECODE_TCL112AC        _IR_ENABLE_DEFAULT_
#endif  // DECODE_TCL112AC
#ifndef SEND_TCL112AC
#define SEND_TCL112AC          _IR_ENABLE_DEFAULT_
#endif  // SEND_TCL112AC

#ifndef DECODE_LEGOPF
#define DECODE_LEGOPF          _IR_ENABLE_DEFAULT_
#endif  // DECODE_LEGOPF
#ifndef SEND_LEGOPF
#define SEND_LEGOPF            _IR_ENABLE_DEFAULT_
#endif  // SEND_LEGOPF

#ifndef DECODE_MITSUBISHIHEAVY
#define DECODE_MITSUBISHIHEAVY _IR_ENABLE_DEFAULT_
#endif  // DECODE_MITSUBISHIHEAVY
#ifndef SEND_MITSUBISHIHEAVY
#define SEND_MITSUBISHIHEAVY   _IR_ENABLE_DEFAULT_
#endif  // SEND_MITSUBISHIHEAVY

#ifndef DECODE_DAIKIN216
#define DECODE_DAIKIN216       _IR_ENABLE_DEFAULT_
#endif  // DECODE_DAIKIN216
#ifndef SEND_DAIKIN216
#define SEND_DAIKIN216         _IR_ENABLE_DEFAULT_
#endif  // SEND_DAIKIN216

#ifndef DECODE_DAIKIN160
#define DECODE_DAIKIN160       _IR_ENABLE_DEFAULT_
#endif  // DECODE_DAIKIN160
#ifndef SEND_DAIKIN160
#define SEND_DAIKIN160         _IR_ENABLE_DEFAULT_
#endif  // SEND_DAIKIN160

#ifndef DECODE_NEOCLIMA
#define DECODE_NEOCLIMA        _IR_ENABLE_DEFAULT_
#endif  // DECODE_NEOCLIMA
#ifndef SEND_NEOCLIMA
#define SEND_NEOCLIMA          _IR_ENABLE_DEFAULT_
#endif  // SEND_NEOCLIMA

#ifndef DECODE_DAIKIN176
#define DECODE_DAIKIN176       _IR_ENABLE_DEFAULT_
#endif  // DECODE_DAIKIN176
#ifndef SEND_DAIKIN176
#define SEND_DAIKIN176         _IR_ENABLE_DEFAULT_
#endif  // SEND_DAIKIN176

#ifndef DECODE_DAIKIN128
#define DECODE_DAIKIN128       _IR_ENABLE_DEFAULT_
#endif  // DECODE_DAIKIN128
#ifndef SEND_DAIKIN128
#define SEND_DAIKIN128         _IR_ENABLE_DEFAULT_
#endif  // SEND_DAIKIN128

#ifndef DECODE_AMCOR
#define DECODE_AMCOR           _IR_ENABLE_DEFAULT_
#endif  // DECODE_AMCOR
#ifndef SEND_AMCOR
#define SEND_AMCOR             _IR_ENABLE_DEFAULT_
#endif  // SEND_AMCOR

#ifndef DECODE_DAIKIN152
#define DECODE_DAIKIN152       _IR_ENABLE_DEFAULT_
#endif  // DECODE_DAIKIN152
#ifndef SEND_DAIKIN152
#define SEND_DAIKIN152         _IR_ENABLE_DEFAULT_
#endif  // SEND_DAIKIN152

#ifndef DECODE_EPSON
#define DECODE_EPSON           _IR_ENABLE_DEFAULT_
#endif  // DECODE_EPSON
#ifndef SEND_EPSON
#define SEND_EPSON             _IR_ENABLE_DEFAULT_
#endif  // SEND_EPSON

#ifndef DECODE_SYMPHONY
#define DECODE_SYMPHONY        _IR_ENABLE_DEFAULT_
#endif  // DECODE_SYMPHONY
#ifndef SEND_SYMPHONY
#define SEND_SYMPHONY          _IR_ENABLE_DEFAULT_
#endif  // SEND_SYMPHONY

#ifndef DECODE_DAIKIN64
#define DECODE_DAIKIN64        _IR_ENABLE_DEFAULT_
#endif  // DECODE_DAIKIN64
#ifndef SEND_DAIKIN64
#define SEND_DAIKIN64          _IR_ENABLE_DEFAULT_
#endif  // SEND_DAIKIN64

#ifndef DECODE_AIRWELL
#define DECODE_AIRWELL         _IR_ENABLE_DEFAULT_
#endif  // DECODE_AIRWELL
#ifndef SEND_AIRWELL
#define SEND_AIRWELL           _IR_ENABLE_DEFAULT_
#endif  // SEND_AIRWELL

#ifndef DECODE_DELONGHI_AC
#define DECODE_DELONGHI_AC     _IR_ENABLE_DEFAULT_
#endif  // DECODE_DELONGHI_AC
#ifndef SEND_DELONGHI_AC
#define SEND_DELONGHI_AC       _IR_ENABLE_DEFAULT_
#endif  // SEND_DELONGHI_AC

#ifndef DECODE_DOSHISHA
#define DECODE_DOSHISHA        _IR_ENABLE_DEFAULT_
#endif  // DECODE_DOSHISHA
#ifndef SEND_DOSHISHA
#define SEND_DOSHISHA          _IR_ENABLE_DEFAULT_
#endif  // SEND_DOSHISHA

#ifndef DECODE_MULTIBRACKETS
#define DECODE_MULTIBRACKETS   _IR_ENABLE_DEFAULT_
#endif  // DECODE_MULTIBRACKETS
#ifndef SEND_MULTIBRACKETS
#define SEND_MULTIBRACKETS     _IR_ENABLE_DEFAULT_
#endif  // SEND_MULTIBRACKETS

#ifndef DECODE_CORONA_AC
#define DECODE_CORONA_AC       _IR_ENABLE_DEFAULT_
#endif  // DECODE_CORONA_AC
#ifndef SEND_CORONA_AC
#define SEND_CORONA_AC         _IR_ENABLE_DEFAULT_
#endif  // SEND_CORONA_AC

#ifndef DECODE_ZEPEAL
#define DECODE_ZEPEAL          _IR_ENABLE_DEFAULT_
#endif  // DECODE_ZEPEAL
#ifndef SEND_ZEPEAL
#define SEND_ZEPEAL            _IR_ENABLE_DEFAULT_
#endif  // SEND_ZEPEAL

#ifndef DECODE_VOLTAS
#define DECODE_VOLTAS          _IR_ENABLE_DEFAULT_
#endif  // DECODE_VOLTAS
#ifndef SEND_VOLTAS
#define SEND_VOLTAS            _IR_ENABLE_DEFAULT_
#endif  // SEND_VOLTAS

#ifndef DECODE_METZ
#define DECODE_METZ          _IR_ENABLE_DEFAULT_
#endif  // DECODE_METZ
#ifndef SEND_METZ
#define SEND_METZ            _IR_ENABLE_DEFAULT_
#endif  // SEND_METZ

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
     DECODE_MITSUBISHI112 || DECODE_HITACHI_AC424 || DECODE_HITACHI_AC3 || \
     DECODE_HITACHI_AC344 || DECODE_CORONA_AC || DECODE_SANYO_AC || \
     DECODE_VOLTAS)
  // Add any DECODE to the above if it uses result->state (see kStateSizeMax)
  // you might also want to add the protocol to hasACState function
#define DECODE_AC true  // We need some common infrastructure for decoding A/Cs.
#else
#define DECODE_AC false   // We don't need that infrastructure.
#endif

// Use millisecond 'delay()' calls where we can to avoid tripping the WDT.
// Note: If you plan to send IR messages in the callbacks of the AsyncWebserver
//       library, you need to set ALLOW_DELAY_CALLS to false.
//       Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/430
#ifndef ALLOW_DELAY_CALLS
#define ALLOW_DELAY_CALLS true
#endif  // ALLOW_DELAY_CALLS

// Enable a run-time settable high-pass filter on captured data **before**
// trying any protocol decoding.
// i.e. Try to remove/merge any really short pulses detected in the raw data.
// Note: Even when this option is enabled, it is _off_ by default, and requires
//       a user who knows what they are doing to enable it.
//       The option to disable this feature is here if your project is _really_
//       tight on resources. i.e. Saves a small handful of bytes and cpu time.
// WARNING: If you use this feature at runtime, you can no longer trust the
//          **raw** data captured. It will now have been slightly **cooked**!
// DANGER: If you set the `noise_floor` value too high, it **WILL** break
//         decoding of some protocols. You have been warned. Here Be Dragons!
//
// See: `irrecv::decode()` in IRrecv.cpp for more info.
#ifndef ENABLE_NOISE_FILTER_OPTION
#define ENABLE_NOISE_FILTER_OPTION true
#endif  // ENABLE_NOISE_FILTER_OPTION

/// Enumerator for defining and numbering of supported IR protocol.
/// @note Always add to the end of the list and should never remove entries
///  or change order. Projects may save the type number for later usage
///  so numbering should always stay the same.
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
  SONY_38K,
  EPSON,  // 75
  SYMPHONY,
  HITACHI_AC3,
  DAIKIN64,
  AIRWELL,
  DELONGHI_AC,  // 80
  DOSHISHA,
  MULTIBRACKETS,
  CARRIER_AC40,
  CARRIER_AC64,
  HITACHI_AC344,  // 85
  CORONA_AC,
  MIDEA24,
  ZEPEAL,
  SANYO_AC,
  VOLTAS,  // 90
  METZ,
  // Add new entries before this one, and update it to point to the last entry.
  kLastDecodeType = METZ,
};

// Message lengths & required repeat values
const uint16_t kNoRepeat = 0;
const uint16_t kSingleRepeat = 1;

const uint16_t kAirwellBits = 34;
const uint16_t kAirwellMinRepeats = 2;
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
const uint16_t kCarrierAc40Bits = 40;
const uint16_t kCarrierAc40MinRepeat = 2;
const uint16_t kCarrierAc64Bits = 64;
const uint16_t kCarrierAc64MinRepeat = kNoRepeat;
const uint16_t kCoronaAcStateLengthShort = 7;
const uint16_t kCoronaAcStateLength = kCoronaAcStateLengthShort * 3;
const uint16_t kCoronaAcBitsShort = kCoronaAcStateLengthShort * 8;
const uint16_t kCoronaAcBits = kCoronaAcStateLength * 8;
const uint16_t kDaikinStateLength = 35;
const uint16_t kDaikinBits = kDaikinStateLength * 8;
const uint16_t kDaikinStateLengthShort = kDaikinStateLength - 8;
const uint16_t kDaikinBitsShort = kDaikinStateLengthShort * 8;
const uint16_t kDaikinDefaultRepeat = kNoRepeat;
const uint16_t kDaikin2StateLength = 39;
const uint16_t kDaikin2Bits = kDaikin2StateLength * 8;
const uint16_t kDaikin2DefaultRepeat = kNoRepeat;
const uint16_t kDaikin64Bits = 64;
const uint16_t kDaikin64DefaultRepeat = kNoRepeat;
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
const uint16_t kDelonghiAcBits = 64;
const uint16_t kDelonghiAcDefaultRepeat = kNoRepeat;
const uint16_t kDenonBits = 15;
const uint16_t kDenon48Bits = 48;
const uint16_t kDenonLegacyBits = 14;
const uint16_t kDishBits = 16;
const uint16_t kDishMinRepeat = 3;
const uint16_t kDoshishaBits = 40;
const uint16_t kEpsonBits = 32;
const uint16_t kEpsonMinRepeat = 2;
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
const uint16_t kHitachiAc3StateLength = 27;
const uint16_t kHitachiAc3Bits = kHitachiAc3StateLength * 8;
const uint16_t kHitachiAc3MinStateLength = 15;
const uint16_t kHitachiAc3MinBits = kHitachiAc3MinStateLength * 8;
const uint16_t kHitachiAc344StateLength = 43;
const uint16_t kHitachiAc344Bits = kHitachiAc344StateLength * 8;
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
const uint16_t kMetzBits = 19;
const uint16_t kMetzMinRepeat = kNoRepeat;
const uint16_t kMideaBits = 48;
const uint16_t kMideaMinRepeat = kNoRepeat;
const uint16_t kMidea24Bits = 24;
const uint16_t kMidea24MinRepeat = kSingleRepeat;
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
const uint16_t kMultibracketsBits = 8;
const uint16_t kMultibracketsDefaultRepeat = kSingleRepeat;
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
const uint16_t kSanyoAcStateLength = 9;
const uint16_t kSanyoAcBits = kSanyoAcStateLength * 8;
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
const uint16_t kSymphonyBits = 12;
const uint16_t kSymphonyDefaultRepeat = 3;
const uint16_t kTcl112AcStateLength = 14;
const uint16_t kTcl112AcBits = kTcl112AcStateLength * 8;
const uint16_t kTcl112AcDefaultRepeat = kNoRepeat;
const uint16_t kTecoBits = 35;
const uint16_t kTecoDefaultRepeat = kNoRepeat;
const uint16_t kToshibaACStateLength = 9;
const uint16_t kToshibaACBits = kToshibaACStateLength * 8;
const uint16_t kToshibaACMinRepeat = kSingleRepeat;
const uint16_t kToshibaACStateLengthShort = kToshibaACStateLength - 2;
const uint16_t kToshibaACBitsShort = kToshibaACStateLengthShort * 8;
const uint16_t kToshibaACStateLengthLong = kToshibaACStateLength + 1;
const uint16_t kToshibaACBitsLong = kToshibaACStateLengthLong * 8;
const uint16_t kTrotecStateLength = 9;
const uint16_t kTrotecBits = kTrotecStateLength * 8;
const uint16_t kTrotecDefaultRepeat = kNoRepeat;
const uint16_t kWhirlpoolAcStateLength = 21;
const uint16_t kWhirlpoolAcBits = kWhirlpoolAcStateLength * 8;
const uint16_t kWhirlpoolAcDefaultRepeat = kNoRepeat;
const uint16_t kWhynterBits = 32;
const uint8_t  kVestelAcBits = 56;
const uint16_t kZepealBits = 16;
const uint16_t kZepealMinRepeat = 4;
const uint16_t kVoltasBits = 80;
const uint16_t kVoltasStateLength = 10;


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
