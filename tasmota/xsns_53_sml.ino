/*
  xsns_53_sml.ino - SML,OBIS,EBUS,RAW,COUNTER interface for Tasmota

  Created by Gerhard Mutz on 07.10.11.
  adapted for Tasmota

  Copyright (C) 2021  Gerhard Mutz and Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifdef USE_SML_M

#define XSNS_53 53

// default baudrate of D0 output
#define SML_BAUDRATE 9600

// send this every N seconds (for meters that only send data on demand)
// not longer supported, use scripting instead
//#define SML_SEND_SEQ

// debug counter input to led for counter1 and 2
//#define DEBUG_CNT_LED1 2
//#define DEBUG_CNT_LED1 2

// use analog optical counter sensor with AD Converter ADS1115 (not yet functional)
//#define ANALOG_OPTO_SENSOR
// fototransistor with pullup at A0, A1 of ADS1115 A3 and +3.3V
// level and amplification are automatically set


#include <TasmotaSerial.h>

// use special no wait serial driver, should be always on
#ifndef ESP32
#define SPECIAL_SS
#endif

#ifndef TMSBSIZ
#define TMSBSIZ 256
#endif

// addresses a bug in meter DWS74
//#define DWS74_BUG

// JSON Strings do not translate
// max 23 char
#define DJ_TPWRIN "Total_in"
#define DJ_TPWRIN0 "Total_in_0"
#define DJ_TPWRIN1 "Total_in_1"
#define DJ_TPWROUT "Total_out"
#define DJ_TPWRCURR "Power_curr"
#define DJ_TPWRCURR1 "Power_p1"
#define DJ_TPWRCURR2 "Power_p2"
#define DJ_TPWRCURR3 "Power_p3"
#define DJ_CURR1 "Curr_p1"
#define DJ_CURR2 "Curr_p2"
#define DJ_CURR3 "Curr_p3"
#define DJ_VOLT1 "Volt_p1"
#define DJ_VOLT2 "Volt_p2"
#define DJ_VOLT3 "Volt_p3"
#define DJ_METERNR "Meter_number"
#define DJ_METERSID "Meter_id"
#define DJ_CSUM "Curr_summ"
#define DJ_VAVG "Volt_avg"
#define DJ_COUNTER "Count"

struct METER_DESC {
  int8_t srcpin;
  uint8_t type;
  uint16_t flag;
  int32_t params;
  char prefix[8];
  int8_t trxpin;
  uint8_t tsecs;
  char *txmem;
  uint8_t index;
  uint8_t max_index;
  char *script_str;
  uint8_t sopt;
};

// this descriptor method is no longer supported
// but still functional for simple meters
// use scripting method instead
// meter list , enter new meters here
//=====================================================
#define EHZ161_0 1
#define EHZ161_1 2
#define EHZ363 3
#define EHZH 4
#define EDL300 5
#define Q3B 6
#define COMBO3 7
#define COMBO2 8
#define COMBO3a 9
#define Q3B_V1 10
#define EHZ363_2 11
#define COMBO3b 12
#define WGS_COMBO 13
#define EBZD_G 14
#define SML_NO_OP 15
#define Q3C 16

// select this meter
// SML_NO_OP ignores hardcoded interface
#define METER SML_NO_OP
//#define METER EHZ161_1

#if METER==SML_NO_OP
#undef METERS_USED
#define METERS_USED 0
struct METER_DESC const meter_desc[]={};
const uint8_t meter[]="";
#endif


#if METER==EHZ161_0
#undef METERS_USED
#define METERS_USED 1
struct METER_DESC const meter_desc[METERS_USED]={
  [0]={3,'o',0,SML_BAUDRATE,"OBIS",-1,1,0}};
const uint8_t meter[]=
"1,1-0:1.8.0*255(@1," D_TPWRIN ",kWh," DJ_TPWRIN ",4|"
"1,1-0:2.8.0*255(@1," D_TPWROUT ",kWh," DJ_TPWROUT ",4|"
"1,1-0:21.7.0*255(@1," D_TPWRCURR1 ",W," DJ_TPWRCURR1 ",0|"
"1,1-0:41.7.0*255(@1," D_TPWRCURR2 ",W," DJ_TPWRCURR2 ",0|"
"1,1-0:61.7.0*255(@1," D_TPWRCURR3 ",W," DJ_TPWRCURR3 ",0|"
"1,=m 3+4+5 @1," D_TPWRCURR ",W," DJ_TPWRCURR ",0|"
"1,1-0:0.0.0*255(@#)," D_METERNR ",," DJ_METERNR ",0";

#endif

//=====================================================

#if METER==EHZ161_1
#undef METERS_USED
#define METERS_USED 1
struct METER_DESC const meter_desc[METERS_USED]={
  [0]={3,'o',0,SML_BAUDRATE,"OBIS",-1,1,0}};
const uint8_t meter[]=
"1,1-0:1.8.1*255(@1," D_TPWRIN ",kWh," DJ_TPWRIN ",4|"
"1,1-0:2.8.1*255(@1," D_TPWROUT ",kWh," DJ_TPWROUT ",4|"
"1,=d 2 10 @1," D_TPWRCURR ",W," DJ_TPWRCURR ",0|"
"1,1-0:0.0.0*255(@#)," D_METERNR ",," DJ_METERNR ",0";
#endif

//=====================================================

#if METER==EHZ363
#undef METERS_USED
#define METERS_USED 1
struct METER_DESC const meter_desc[METERS_USED]={
  [0]={3,'s',0,SML_BAUDRATE,"SML",-1,1,0}};
// 2 Richtungszähler EHZ SML 8 bit 9600 baud, binär
const uint8_t meter[]=
//0x77,0x07,0x01,0x00,0x01,0x08,0x00,0xff
"1,77070100010800ff@1000," D_TPWRIN ",kWh," DJ_TPWRIN ",4|"
//0x77,0x07,0x01,0x00,0x02,0x08,0x00,0xff
"1,77070100020800ff@1000," D_TPWROUT ",kWh," DJ_TPWROUT ",4|"
//0x77,0x07,0x01,0x00,0x10,0x07,0x00,0xff
"1,77070100100700ff@1," D_TPWRCURR ",W," DJ_TPWRCURR ",0|"
//0x77,0x07,0x01,0x00,0x00,0x00,0x09,0xff
"1,77070100000009ff@#," D_METERNR ",," DJ_METERNR ",0";
#endif

//=====================================================

#if METER==EHZH
#undef METERS_USED
#define METERS_USED 1
struct METER_DESC const meter_desc[METERS_USED]={
  [0]={3,'s',0,SML_BAUDRATE,"SML",-1,1,0}};
// 2 Richtungszähler EHZ SML 8 bit 9600 baud, binär
// verbrauch total
const uint8_t meter[]=
//0x77,0x07,0x01,0x00,0x01,0x08,0x00,0xff
"1,77070100010800ff@1000," D_TPWRIN ",kWh," DJ_TPWRIN ",4|"
//0x77,0x07,0x01,0x00,0x01,0x08,0x01,0xff
"1,77070100020800ff@1000," D_TPWROUT ",kWh," DJ_TPWROUT ",4|"
//0x77,0x07,0x01,0x00,0x0f,0x07,0x00,0xff
"1,770701000f0700ff@1," D_TPWRCURR ",W," DJ_TPWRCURR ",0";
#endif

//=====================================================

#if METER==EDL300
#undef METERS_USED
#define METERS_USED 1
struct METER_DESC const meter_desc[METERS_USED]={
  [0]={3,'s',0,SML_BAUDRATE,"SML",-1,1,0}};
// 2 Richtungszähler EHZ SML 8 bit 9600 baud, binär
// verbrauch total
const uint8_t meter[]=
//0x77,0x07,0x01,0x00,0x01,0x08,0x00,0xff
"1,77070100010800ff@1000," D_TPWRIN ",kWh," DJ_TPWRIN ",4|"
//0x77,0x07,0x01,0x00,0x01,0x08,0x01,0xff
"1,77070100020801ff@1000," D_TPWROUT ",kWh," DJ_TPWROUT ",4|"
//0x77,0x07,0x01,0x00,0x0f,0x07,0x00,0xff
"1,770701000f0700ff@1," D_TPWRCURR ",W," DJ_TPWRCURR ",0";
#endif

#if METER==EBZD_G
#undef METERS_USED
#define METERS_USED 1
struct METER_DESC const meter_desc[METERS_USED]={
  [0]={3,'s',0,SML_BAUDRATE,"strom",-1,1,0}};
const uint8_t meter[]=
//0x77,0x07,0x01,0x00,0x01,0x08,0x00,0xff
"1,77070100010800ff@1000," D_TPWRIN ",kWh," DJ_TPWRIN ",4|"
// ..
"1,77070100020800ff@1000," D_TPWROUT ",kWh," DJ_TPWROUT ",4|"
//0x77,0x07,0x01,0x00,0x01,0x08,0x01,0xff
"1,77070100010801ff@1000," D_TPWRCURR1 ",kWh," DJ_TPWRCURR1 ",4|"
//0x77,0x07,0x01,0x00,0x01,0x08,0x02,0xff
"1,77070100010802ff@1000," D_TPWRCURR2 ",kWh," DJ_TPWRCURR2 ",4|"
// 77 07 01 00 10 07 00 FF
"1,77070100100700ff@1," D_TPWRCURR ",W," DJ_TPWRCURR ",0|"
// ..
"1,77070100600100ff@#," D_METERNR ",," DJ_METERNR ",0";
#endif


//=====================================================

#if METER==Q3B
#undef METERS_USED
#define METERS_USED 1
struct METER_DESC const meter_desc[METERS_USED]={
  [0]={3,'s',0,SML_BAUDRATE,"SML",-1,1,0}};
const uint8_t meter[]=
//0x77,0x07,0x01,0x00,0x01,0x08,0x01,0xff
"1,77070100010800ff@1000," D_TPWRIN ",kWh," DJ_TPWRIN ",4|"
//0x77,0x07,0x01,0x00,0x02,0x08,0x01,0xff
"1,77070100020801ff@1000," D_TPWROUT ",kWh," DJ_TPWROUT ",4|"
//0x77,0x07,0x01,0x00,0x01,0x07,0x00,0xff
"1,77070100010700ff@1," D_TPWRCURR ",W," DJ_TPWRCURR ",0";
#endif

#if METER==COMBO3
// 3 Zähler Beispiel
#undef METERS_USED
#define METERS_USED 3

struct METER_DESC const meter_desc[METERS_USED]={
  [0]={3,'o',0,SML_BAUDRATE,"OBIS",-1,1,0}, // harware serial RX pin
  [1]={14,'s',0,SML_BAUDRATE,"SML",-1,1,0}, // GPIO14 software serial
  [2]={4,'o',0,SML_BAUDRATE,"OBIS2",-1,1,0}}; // GPIO4 software serial

// 3 Zähler definiert
const uint8_t meter[]=
"1,1-0:1.8.0*255(@1," D_TPWRIN ",kWh," DJ_TPWRIN ",4|"
"1,1-0:2.8.0*255(@1," D_TPWROUT ",kWh," DJ_TPWROUT ",4|"
"1,1-0:21.7.0*255(@1," D_TPWRCURR1 ",W," DJ_TPWRCURR1 ",0|"
"1,1-0:41.7.0*255(@1," D_TPWRCURR2 ",W," DJ_TPWRCURR2 ",0|"
"1,1-0:61.7.0*255(@1," D_TPWRCURR3 ",W," DJ_TPWRCURR3 ",0|"
"1,=m 3+4+5 @1," D_TPWRCURR ",W," DJ_TPWRCURR ",0|"
"1,1-0:0.0.0*255(@#)," D_METERNR ",," DJ_METERNR ",0|"
"2,77070100010800ff@1000," D_TPWRIN ",kWh," DJ_TPWRIN ",4|"
"2,77070100020800ff@1000," D_TPWROUT ",kWh," DJ_TPWROUT ",4|"
"2,77070100100700ff@1," D_TPWRCURR ",W," DJ_TPWRCURR ",0|"
"3,1-0:1.8.1*255(@1," D_TPWRIN ",kWh," DJ_TPWRIN ",4|"
"3,1-0:2.8.1*255(@1," D_TPWROUT ",kWh," DJ_TPWROUT ",4|"
"3,=d 2 10 @1," D_TPWRCURR ",W," DJ_TPWRCURR ",0|"
"3,1-0:0.0.0*255(@#)," D_METERNR ",," DJ_METERNR ",0";

#endif

#if METER==COMBO2
// 2 Zähler Beispiel
#undef METERS_USED
#define METERS_USED 2

struct METER_DESC const meter_desc[METERS_USED]={
  [0]={3,'o',0,SML_BAUDRATE,"OBIS1",-1,1,0}, // harware serial RX pin
  [1]={14,'o',0,SML_BAUDRATE,"OBIS2",-1,1,0}}; // GPIO14 software serial

// 2 Zähler definiert
const uint8_t meter[]=
"1,1-0:1.8.1*255(@1," D_TPWRIN ",kWh," DJ_TPWRIN ",4|"
"1,1-0:2.8.1*255(@1," D_TPWROUT ",kWh," DJ_TPWROUT ",4|"
"1,=d 2 10 @1," D_TPWRCURR ",W," DJ_TPWRCURR ",0|"
"1,1-0:0.0.0*255(@#)," D_METERNR ",," DJ_METERNR ",0|"

"2,1-0:1.8.1*255(@1," D_TPWRIN ",kWh," DJ_TPWRIN ",4|"
"2,1-0:2.8.1*255(@1," D_TPWROUT ",kWh," DJ_TPWROUT ",4|"
"2,=d 6 10 @1," D_TPWRCURR ",W," DJ_TPWRCURR ",0|"
"2,1-0:0.0.0*255(@#)," D_METERNR ",," DJ_METERNR ",0";

#endif

#if METER==COMBO3a
#undef METERS_USED
#define METERS_USED 3

struct METER_DESC const meter_desc[METERS_USED]={
  [0]={3,'o',0,SML_BAUDRATE,"OBIS1",-1,1,0}, // harware serial RX pin
  [1]={14,'o',0,SML_BAUDRATE,"OBIS2",-1,1,0},
  [2]={1,'o',0,SML_BAUDRATE,"OBIS3",-1,1,0}};

// 3 Zähler definiert
const uint8_t meter[]=
"1,=h --- Zähler Nr 1 ---|"
"1,1-0:1.8.1*255(@1," D_TPWRIN ",kWh," DJ_TPWRIN ",4|"
"1,1-0:2.8.1*255(@1," D_TPWROUT ",kWh," DJ_TPWROUT ",4|"
"1,=d 2 10 @1," D_TPWRCURR ",W," DJ_TPWRCURR ",0|"
"1,1-0:0.0.0*255(@#)," D_METERNR ",," DJ_METERNR ",0|"
"2,=h --- Zähler Nr 2 ---|"
"2,1-0:1.8.1*255(@1," D_TPWRIN ",kWh," DJ_TPWRIN ",4|"
"2,1-0:2.8.1*255(@1," D_TPWROUT ",kWh," DJ_TPWROUT ",4|"
"2,=d 6 10 @1," D_TPWRCURR ",W," DJ_TPWRCURR ",0|"
"2,1-0:0.0.0*255(@#)," D_METERNR ",," DJ_METERNR ",0|"
"3,=h --- Zähler Nr 3 ---|"
"3,1-0:1.8.1*255(@1," D_TPWRIN ",kWh," DJ_TPWRIN ",4|"
"3,1-0:2.8.1*255(@1," D_TPWROUT ",kWh," DJ_TPWROUT ",4|"
"3,=d 10 10 @1," D_TPWRCURR ",W," DJ_TPWRCURR ",0|"
"3,1-0:0.0.0*255(@#)," D_METERNR ",," DJ_METERNR ",0";

#endif

//=====================================================

#if METER==Q3B_V1
#undef METERS_USED
#define METERS_USED 1
struct METER_DESC const meter_desc[METERS_USED]={
[0]={3,'o',0,SML_BAUDRATE,"OBIS",-1,1,0}};
const uint8_t meter[]=
"1,1-0:1.8.1*255(@1," D_TPWRIN ",kWh," DJ_TPWRIN ",4|"
"1,=d 1 10 @1," D_TPWRCURR ",W," DJ_TPWRCURR ",0|"
"1,1-0:0.0.0*255(@#)," D_METERNR ",," DJ_METERNR ",0";
#endif

//=====================================================

#if METER==EHZ363_2
#undef METERS_USED
#define METERS_USED 1
struct METER_DESC const meter_desc[METERS_USED]={
[0]={3,'s',0,SML_BAUDRATE,"SML",-1,1,0}};
// 2 direction meter EHZ SML 8 bit 9600 baud, binary
const uint8_t meter[]=
//0x77,0x07,0x01,0x00,0x01,0x08,0x00,0xff
"1,77070100010800ff@1000," D_TPWRIN ",kWh," DJ_TPWRIN ",4|"
//0x77,0x07,0x01,0x00,0x02,0x08,0x00,0xff
"1,77070100020800ff@1000," D_TPWROUT ",kWh," DJ_TPWROUT ",4|"
//0x77,0x07,0x01,0x00,0x01,0x08,0x01,0xff
"1,77070100010801ff@1000," D_TPWRCURR1 ",kWh," DJ_TPWRCURR1 ",4|"
//0x77,0x07,0x01,0x00,0x01,0x08,0x02,0xff
"1,77070100010802ff@1000," D_TPWRCURR2 ",kWh," DJ_TPWRCURR2 ",4|"
//0x77,0x07,0x01,0x00,0x10,0x07,0x00,0xff
"1,77070100100700ff@1," D_TPWRCURR ",W," DJ_TPWRCURR ",0|"
//0x77,0x07,0x01,0x00,0x00,0x00,0x09,0xff
"1,77070100000009ff@#," D_METERNR ",," DJ_METERNR ",0";
#endif

// example  OBIS power meter + gas and water counter
#if METER==COMBO3b
#undef METERS_USED
#define METERS_USED 3
struct METER_DESC const meter_desc[METERS_USED]={
  [0]={3,'o',0,SML_BAUDRATE,"OBIS",-1,1,0}, // harware serial RX pin
  [1]={14,'c',0,50,"Gas"}, // GPIO14 gas counter
  [2]={1,'c',0,10,"Wasser"}}; // water counter

// 3 meters defined
const uint8_t meter[]=
"1,1-0:1.8.1*255(@1," D_TPWRIN ",kWh," DJ_TPWRIN ",4|"
"1,1-0:2.8.1*255(@1," D_TPWROUT ",kWh," DJ_TPWROUT ",4|"
"1,=d 2 10 @1," D_TPWRCURR ",W," DJ_TPWRCURR ",0|"
"1,1-0:0.0.0*255(@#)," D_METERNR ",," DJ_METERNR ",0|"

// with counters the comparison string must be exactly this string
"2,1-0:1.8.0*255(@100," D_GasIN ",cbm," DJ_COUNTER ",2|"

"3,1-0:1.8.0*255(@100," D_H2oIN ",cbm," DJ_COUNTER ",2";
#endif


#if METER==WGS_COMBO
#undef METERS_USED
#define METERS_USED 3

struct METER_DESC const meter_desc[METERS_USED]={
  [0]={1,'c',0,10,"H20",-1,1,0}, // GPIO1 water counter
  [1]={4,'c',0,50,"GAS",-1,1,0}, // GPIO4 gas counter
  [2]={3,'s',0,SML_BAUDRATE,"SML",-1,1,0}}; // SML harware serial RX pin

const uint8_t meter[]=
//----------------------------Wasserzähler--sensor53 c1------------------------------------
//"1,=h==================|"
"1,1-0:1.8.0*255(@10000," D_H2oIN ",cbm," DJ_COUNTER ",4|"            // 1
//----------------------------Gaszähler-----sensor53 c2------------------------------------
// bei gaszählern (countern) muss der Vergleichsstring so aussehen wie hier
"2,=h==================|"
"2,1-0:1.8.0*255(@100," D_GasIN ",cbm," DJ_COUNTER ",3|"              // 2
//----------------------------Stromzähler-EHZ363W5--sensor53 d0----------------------------
"3,=h==================|"
//0x77,0x07,0x01,0x00,0x01,0x08,0x00,0xff
"3,77070100010800ff@1000," D_TPWRIN ",kWh," DJ_TPWRIN ",3|"         // 3  Zählerstand Total
"3,=h==================|"
//0x77,0x07,0x01,0x00,0x10,0x07,0x00,0xff
"3,77070100100700ff@1," D_TPWRCURR ",W," DJ_TPWRCURR ",2|"          // 4  Aktuelle Leistung
"3,=h -------------------------------|"
"3,=m 10+11+12 @100," D_StL1L2L3 ",A," DJ_CSUM ",2|"            // 5  Summe Aktuelle Ströme
//"3,=h -------------------------------|"
"3,=m 13+14+15/#3 @100," D_SpL1L2L3 ",V," DJ_VAVG ",2|"      // 6   Mittelwert Spannungen
"3,=h==================|"
//0x77,0x07,0x01,0x00,0x24,0x07,0x00,0xff
"3,77070100240700ff@1," D_TPWRCURR1 ",W," DJ_TPWRCURR1 ",2|"        // 7  Wirkleistung L1
//0x77,0x07,0x01,0x00,0x38,0x07,0x00,0xff
"3,77070100380700ff@1," D_TPWRCURR2 ",W," DJ_TPWRCURR2 ",2|"        // 8  Wirkleistung L2
//0x77,0x07,0x01,0x00,0x4c,0x07,0x00,0xff
"3,770701004c0700ff@1," D_TPWRCURR3 ",W," DJ_TPWRCURR3 ",2|"        // 9  Wirkleistung L3
"3,=h -------------------------------|"
//0x77,0x07,0x01,0x00,0x1f,0x07,0x00,0xff
"3,770701001f0700ff@100," D_Strom_L1 ",A," DJ_CURR1 ",2|"        // 10 Strom L1
//0x77,0x07,0x01,0x00,0x33,0x07,0x00,0xff
"3,77070100330700ff@100," D_Strom_L2 ",A," DJ_CURR2 ",2|"        // 11 Strom L2
//0x77,0x07,0x01,0x00,0x47,0x07,0x00,0xff
"3,77070100470700ff@100," D_Strom_L3 ",A," DJ_CURR3 ",2|"        // 12 Strom L3
"3,=h -------------------------------|"
//0x77,0x07,0x01,0x00,0x20,0x07,0x00,0xff
"3,77070100200700ff@100," D_Spannung_L1 ",V," DJ_VOLT1 ",2|"  // 13 Spannung L1
//0x77,0x07,0x01,0x00,0x34,0x07,0x00,0xff
"3,77070100340700ff@100," D_Spannung_L2 ",V," DJ_VOLT2 ",2|"  // 14 Spannung L2
//0x77,0x07,0x01,0x00,0x48,0x07,0x00,0xff
"3,77070100480700ff@100," D_Spannung_L3 ",V," DJ_VOLT3 ",2|"  // 15 Spannung L3
"3,=h==================|"
//0x77,0x07,0x01,0x00,0x00,0x00,0x09,0xff
"3,77070100000009ff@#," D_METERSID ",," DJ_METERSID ",0|"           // 16 Service ID
"3,=h--------------------------------";                             // letzte Zeile
#endif


#if METER==Q3C
#undef METERS_USED
#define METERS_USED 1
struct METER_DESC const meter_desc[METERS_USED]={
  [0]={3,'s',0,SML_BAUDRATE,"SML",-1,1,0}};
const uint8_t meter[]=
//0x77,0x07,0x01,0x00,0x01,0x08,0x01,0xff
"1,77070101010800ff@1000," D_TPWRIN0 ",kWh," DJ_TPWRIN0 ",2|" // Verbrauch T0
//0x77,0x07,0x01,0x00,0x01,0x08,0x01,0xff
"1,77070101010801ff@1000," D_TPWRIN1 ",kWh," DJ_TPWRIN1 ",2|" // Verbrauch T1
//0x77,0x07,0x01,0x00,0x01,0x07,0x00,0xff
"1,77070100010700ff@1," D_TPWRCURR ",W," DJ_TPWRCURR ",0|" // Strom Gesamt
//0x77,0x07,0x01,0x00,0x01,0x07,0x00,0xff
"1,77070100150700ff@1," D_TPWRCURR1 ",W," DJ_TPWRCURR1 ",0|" // Strom L1
//0x77,0x07,0x01,0x00,0x01,0x07,0x00,0xff
"1,77070100290700ff@1," D_TPWRCURR2 ",W," DJ_TPWRCURR2 ",0|" // Strom L2
//0x77,0x07,0x01,0x00,0x01,0x07,0x00,0xff
"1,770701003D0700ff@1," D_TPWRCURR3 ",W," DJ_TPWRCURR3 ",0"; // Strom L3
#endif

// this driver uses double because meter vars would not fit in float
//=====================================================

// median filter eliminates outliers, but uses much RAM and CPU cycles
// 672 bytes extra RAM with SML_MAX_VARS = 16
// default compile on, but must be enabled by descriptor flag 16
// may be undefined if RAM must be saved
#define USE_SML_MEDIAN_FILTER

// max number of vars , may be adjusted
#ifndef SML_MAX_VARS
#define SML_MAX_VARS 20
#endif

// max number of meters , may be adjusted
#ifndef MAX_METERS
#define MAX_METERS 5
#endif
double meter_vars[SML_MAX_VARS];
// calulate deltas
#define MAX_DVARS MAX_METERS*2
double dvalues[MAX_DVARS];
uint32_t dtimes[MAX_DVARS];
uint8_t meters_used;
uint8_t dvalid[SML_MAX_VARS];

struct METER_DESC const *meter_desc_p;
const uint8_t *meter_p;
uint16_t meter_spos[MAX_METERS];

// software serial pointers
#ifdef ESP8266
TasmotaSerial *meter_ss[MAX_METERS];
#endif  // ESP8266


// serial buffers, may be made larger depending on telegram lenght
#ifndef SML_BSIZ
#define SML_BSIZ 48
#endif
uint8_t smltbuf[MAX_METERS][SML_BSIZ];

// meter nr as string
#define METER_ID_SIZE 24
char meter_id[MAX_METERS][METER_ID_SIZE];

#define VBUS_SYNC		0xaa
#define SML_SYNC		0x77
#define EBUS_SYNC		0xaa
#define EBUS_ESC    0xa9

uint8_t sml_send_blocks;
uint8_t sml_100ms_cnt;
uint8_t sml_desc_cnt;
uint8_t sml_json_enable = 1;

#ifdef USE_SML_MEDIAN_FILTER
// median filter, should be odd size
#define MEDIAN_SIZE 5
struct SML_MEDIAN_FILTER {
double buffer[MEDIAN_SIZE];
int8_t index;
} sml_mf[SML_MAX_VARS];

#ifndef FLT_MAX
#define FLT_MAX 99999999
#endif

double sml_median_array(double *array,uint8_t len) {
      uint8_t ind[len];
      uint8_t mind=0,index=0,flg;
      double min=FLT_MAX;

      for (uint8_t hcnt=0; hcnt<len/2+1; hcnt++) {
          for (uint8_t mcnt=0; mcnt<len; mcnt++) {
              flg=0;
              for (uint8_t icnt=0; icnt<index; icnt++) {
                  if (ind[icnt]==mcnt) {
                      flg=1;
                  }
              }
              if (!flg) {
                  if (array[mcnt]<min) {
                      min=array[mcnt];
                      mind=mcnt;
                  }
              }
          }
          ind[index]=mind;
          index++;
          min=FLT_MAX;
      }
      return array[ind[len/2]];
  }


// calc median
double sml_median(struct SML_MEDIAN_FILTER* mf, double in) {
  //double tbuff[MEDIAN_SIZE],tmp;
  //uint8_t flag;
  mf->buffer[mf->index]=in;
  mf->index++;
  if (mf->index>=MEDIAN_SIZE) mf->index=0;

  return sml_median_array(mf->buffer,MEDIAN_SIZE);
/*
  // sort list and take median
  memmove(tbuff,mf->buffer,sizeof(tbuff));
  for (byte ocnt=0; ocnt<MEDIAN_SIZE; ocnt++) {
    flag=0;
    for (byte count=0; count<MEDIAN_SIZE-1; count++) {
      if (tbuff[count]>tbuff[count+1]) {
        tmp=tbuff[count];
        tbuff[count]=tbuff[count+1];
        tbuff[count+1]=tmp;
        flag=1;
      }
    }
    if (!flag) break;
  }
  return tbuff[MEDIAN_SIZE/2];
  */
}
#endif


// ESP32 software serial read only
#ifdef ESP32
#ifdef USE_ESP32_SW_SERIAL

#ifndef ESP32_SWS_BUFFER_SIZE
#define ESP32_SWS_BUFFER_SIZE 256
#endif


class SML_ESP32_SERIAL : public Stream {
public:
	SML_ESP32_SERIAL(uint32_t uart_index);
  virtual ~SML_ESP32_SERIAL();
  bool begin(uint32_t speed, uint32_t smode, int32_t recpin, int32_t trxpin);
  int32_t peek(void);
  int32_t read(void) override;
  size_t write(uint8_t byte) override;
  int32_t available(void) override;
  void flush(void) override;
  void setRxBufferSize(uint32_t size);
  void updateBaudRate(uint32_t baud);
  void rxRead(void);
  using Print::write;
private:
  // Member variables
  void setbaud(uint32_t speed);
  uint32_t uart_index;
  int8_t m_rx_pin;
  int8_t m_tx_pin;
  uint32_t cfgmode;
  uint32_t ss_byte;
  uint32_t ss_bstart;
  uint32_t ss_index;
  uint32_t m_bit_time;
  uint32_t m_in_pos;
  uint32_t m_out_pos;
  uint16_t serial_buffer_size;
  bool m_valid;
  uint8_t *m_buffer;
  HardwareSerial *hws;
};


void IRAM_ATTR sml_callRxRead(void *self) { ((SML_ESP32_SERIAL*)self)->rxRead(); };

SML_ESP32_SERIAL::SML_ESP32_SERIAL(uint32_t index) {
  uart_index = index;
  m_valid = true;
}

SML_ESP32_SERIAL::~SML_ESP32_SERIAL(void) {
  if (hws) {
    hws->end();
  } else {
    detachInterrupt(m_rx_pin);
    if (m_buffer) {
      free(m_buffer);
    }
  }
}

void SML_ESP32_SERIAL::setbaud(uint32_t speed) {
  m_bit_time = ESP.getCpuFreqMHz() * 1000000 / speed;
}


bool SML_ESP32_SERIAL::begin(uint32_t speed, uint32_t smode, int32_t recpin, int32_t trxpin) {
  if (!m_valid) { return false; }

  m_buffer = 0;
  if (recpin < 0) {
    setbaud(speed);
    m_rx_pin = -recpin;
    serial_buffer_size = ESP32_SWS_BUFFER_SIZE;
    m_buffer = (uint8_t*)malloc(serial_buffer_size);
    if (m_buffer == NULL) return false;
    pinMode(m_rx_pin, INPUT);
    attachInterruptArg(m_rx_pin, sml_callRxRead, this, CHANGE);
    m_in_pos = m_out_pos = 0;
    hws = nullptr;
  } else {
    cfgmode = smode;
    m_rx_pin = recpin;
    m_tx_pin = trxpin;
    hws = new HardwareSerial(uart_index);
    if (hws) {
      hws->begin(speed, cfgmode, m_rx_pin, m_tx_pin);
    }
  }
  return true;
}

void SML_ESP32_SERIAL::flush(void) {
  if (hws) {
    hws->flush();
  } else {
    m_in_pos = m_out_pos = 0;
  }
}

int32_t SML_ESP32_SERIAL::peek(void) {
  if (hws) {
    return  hws->peek();
  } else {
    if (m_in_pos == m_out_pos) return -1;
    return m_buffer[m_out_pos];
  }
}

int32_t SML_ESP32_SERIAL::read(void) {
  if (hws) {
    return hws->read();
  } else {
    if (m_in_pos == m_out_pos) return -1;
    uint32_t ch = m_buffer[m_out_pos];
    m_out_pos = (m_out_pos +1) % serial_buffer_size;
    return ch;
  }
}

int32_t SML_ESP32_SERIAL::available(void) {
  if (hws) {
    return hws->available();
  } else {
    int avail = m_in_pos - m_out_pos;
    if (avail < 0) avail += serial_buffer_size;
    return avail;
  }
}

size_t SML_ESP32_SERIAL::write(uint8_t byte) {
  if (hws) {
    return hws->write(byte);
  }
  return 0;
}

void SML_ESP32_SERIAL::setRxBufferSize(uint32_t size) {
  if (hws) {
    hws->setRxBufferSize(size);
  } else {
    if (m_buffer) {
        free(m_buffer);
    }
    serial_buffer_size = size;
    m_buffer = (uint8_t*)malloc(size);
  }
}
void SML_ESP32_SERIAL::updateBaudRate(uint32_t baud) {
  if (hws) {
    hws->updateBaudRate(baud);
  } else {
    setbaud(baud);
  }
}

// no wait mode only 8N1  (or 7X1, obis only, ignoring parity)
void IRAM_ATTR SML_ESP32_SERIAL::rxRead(void) {
  uint32_t diff;
  uint32_t level;

#define SML_LASTBIT 9

  level = digitalRead(m_rx_pin);

  if (!level && !ss_index) {
    // start condition
    ss_bstart = ESP.getCycleCount() - (m_bit_time / 4);
    ss_byte = 0;
    ss_index++;
  } else {
    // now any bit changes go here
    // calc bit number
    diff = (ESP.getCycleCount() - ss_bstart) / m_bit_time;

    if (!level && diff > SML_LASTBIT) {
      // start bit of next byte, store  and restart
      // leave irq at change
      for (uint32_t i = ss_index; i <= SML_LASTBIT; i++) {
        ss_byte |= (1 << i);
      }
      uint32_t next = (m_in_pos + 1) % serial_buffer_size;
      if (next != (uint32_t)m_out_pos) {
        m_buffer[m_in_pos] = ss_byte >> 1;
        m_in_pos = next;
      }

      ss_bstart = ESP.getCycleCount() - (m_bit_time / 4);
      ss_byte = 0;
      ss_index = 1;
      return;
    }
    if (diff >= SML_LASTBIT) {
      // bit zero was 0,
      uint32_t next = (m_in_pos + 1) % serial_buffer_size;
      if (next != (uint32_t)m_out_pos) {
        m_buffer[m_in_pos] = ss_byte >> 1;
        m_in_pos = next;
      }
      ss_byte = 0;
      ss_index = 0;
    } else {
      // shift in
      for (uint32_t i = ss_index; i < diff; i++) {
        if (!level) ss_byte |= (1 << i);
      }
      ss_index = diff;
    }
  }
}
#endif // USE_ESP32_SW_SERIAL
#endif // ESP32

#ifdef ESP32
#ifndef USE_ESP32_SW_SERIAL
HardwareSerial *meter_ss[MAX_METERS];
#else
SML_ESP32_SERIAL *meter_ss[MAX_METERS];
#endif
#endif  // ESP32


#ifdef ANALOG_OPTO_SENSOR
// sensor over ADS1115 with i2c Bus
uint8_t ads1115_up;

// ads1115 driver
#define SAMPLE_BIT (0x8000)

#define ADS1115_COMP_QUEUE_SHIFT 0
#define ADS1115_COMP_LATCH_SHIFT 2
#define ADS1115_COMP_POLARITY_SHIFT 3
#define ADS1115_COMP_MODE_SHIFT 4
#define ADS1115_DATA_RATE_SHIFT 5
#define ADS1115_MODE_SHIFT 8
#define ADS1115_PGA_SHIFT 9
#define ADS1115_MUX_SHIFT 12

enum ads1115_comp_queue {
	ADS1115_COMP_QUEUE_AFTER_ONE = 0,
	ADS1115_COMP_QUEUE_AFTER_TWO = 0x1 << ADS1115_COMP_QUEUE_SHIFT,
	ADS1115_COMP_QUEUE_AFTER_FOUR = 0x2 << ADS1115_COMP_QUEUE_SHIFT,
	ADS1115_COMP_QUEUE_DISABLE = 0x3 << ADS1115_COMP_QUEUE_SHIFT,
	ADS1115_COMP_QUEUE_MASK = 0x3 << ADS1115_COMP_QUEUE_SHIFT,
};

enum ads1115_comp_latch {
	ADS1115_COMP_LATCH_NO = 0,
	ADS1115_COMP_LATCH_YES = 1 << ADS1115_COMP_LATCH_SHIFT,
	ADS1115_COMP_LATCH_MASK = 1 << ADS1115_COMP_LATCH_SHIFT,
};

enum ads1115_comp_polarity {
	ADS1115_COMP_POLARITY_ACTIVE_LOW = 0,
	ADS1115_COMP_POLARITY_ACTIVE_HIGH = 1 << ADS1115_COMP_POLARITY_SHIFT,
	ADS1115_COMP_POLARITY_MASK = 1 << ADS1115_COMP_POLARITY_SHIFT,
};

enum ads1115_comp_mode {
	ADS1115_COMP_MODE_WINDOW = 0,
	ADS1115_COMP_MODE_HYSTERESIS = 1 << ADS1115_COMP_MODE_SHIFT,
	ADS1115_COMP_MODE_MASK = 1 << ADS1115_COMP_MODE_SHIFT,
};

enum ads1115_data_rate {
	ADS1115_DATA_RATE_8_SPS = 0,
	ADS1115_DATA_RATE_16_SPS = 0x1 << ADS1115_DATA_RATE_SHIFT,
	ADS1115_DATA_RATE_32_SPS = 0x2 << ADS1115_DATA_RATE_SHIFT,
	ADS1115_DATA_RATE_64_SPS = 0x3 << ADS1115_DATA_RATE_SHIFT,
	ADS1115_DATA_RATE_128_SPS = 0x4 << ADS1115_DATA_RATE_SHIFT,
	ADS1115_DATA_RATE_250_SPS = 0x5 << ADS1115_DATA_RATE_SHIFT,
	ADS1115_DATA_RATE_475_SPS = 0x6 << ADS1115_DATA_RATE_SHIFT,
	ADS1115_DATA_RATE_860_SPS = 0x7 << ADS1115_DATA_RATE_SHIFT,
	ADS1115_DATA_RATE_MASK = 0x7 << ADS1115_DATA_RATE_SHIFT,
};

enum ads1115_mode {
	ADS1115_MODE_CONTINUOUS = 0,
	ADS1115_MODE_SINGLE_SHOT = 1 << ADS1115_MODE_SHIFT,
	ADS1115_MODE_MASK = 1 << ADS1115_MODE_SHIFT,
};

enum ads1115_pga {
	ADS1115_PGA_TWO_THIRDS = 0, //±6.144 V
	ADS1115_PGA_ONE = 0x1 << ADS1115_PGA_SHIFT, //±4.096 V
	ADS1115_PGA_TWO = 0x2 << ADS1115_PGA_SHIFT, //±2.048 V
	ADS1115_PGA_FOUR = 0x3 << ADS1115_PGA_SHIFT, //±1.024 V
	ADS1115_PGA_EIGHT = 0x4 << ADS1115_PGA_SHIFT, //±0.512 V
	ADS1115_PGA_SIXTEEN = 0x5 << ADS1115_PGA_SHIFT, //±0.256 V
	ADS1115_PGA_MASK = 0x7 << ADS1115_PGA_SHIFT,
};


enum ads1115_mux {
	ADS1115_MUX_DIFF_AIN0_AIN1 = 0,
	ADS1115_MUX_DIFF_AIN0_AIN3 = 0x1 << ADS1115_MUX_SHIFT,
	ADS1115_MUX_DIFF_AIN1_AIN3 = 0x2 << ADS1115_MUX_SHIFT,
	ADS1115_MUX_DIFF_AIN2_AIN3 = 0x3 << ADS1115_MUX_SHIFT,
	ADS1115_MUX_GND_AIN0 = 0x4 << ADS1115_MUX_SHIFT,
	ADS1115_MUX_GND_AIN1 = 0x5 << ADS1115_MUX_SHIFT,
	ADS1115_MUX_GND_AIN2 = 0x6 << ADS1115_MUX_SHIFT,
	ADS1115_MUX_GND_AIN3 = 0x7 << ADS1115_MUX_SHIFT,
	ADS1115_MUX_MASK = 0x7 << ADS1115_MUX_SHIFT,
};

class ADS1115 {
public:
	ADS1115(uint8_t address = 0x48);

	void begin();
	uint8_t trigger_sample();
	uint8_t reset();
	bool is_sample_in_progress();
	int16_t read_sample();
	float sample_to_float(int16_t val);
	float read_sample_float();

	void set_comp_queue(enum ads1115_comp_queue val) { set_config(val, ADS1115_COMP_QUEUE_MASK); }
	void set_comp_latching(enum ads1115_comp_latch val) { set_config(val, ADS1115_COMP_LATCH_MASK); }
	void set_comp_polarity(enum ads1115_comp_polarity val) { set_config(val, ADS1115_COMP_POLARITY_MASK); }
	void set_comp_mode(enum ads1115_comp_mode val) { set_config(val, ADS1115_COMP_MODE_MASK); }
	void set_data_rate(enum ads1115_data_rate val) { set_config(val, ADS1115_DATA_RATE_MASK); }
	void set_mode(enum ads1115_mode val) { set_config(val, ADS1115_MODE_MASK); }
	void set_pga(enum ads1115_pga val) { set_config(val, ADS1115_PGA_MASK); m_voltage_range = val >> ADS1115_PGA_SHIFT; }
	void set_mux(enum ads1115_mux val) { set_config(val, ADS1115_MUX_MASK); }

private:
	void set_config(uint16_t val, uint16_t mask) {
		m_config = (m_config & ~mask) | val;
	}

	uint8_t write_register(uint8_t reg, uint16_t val);
	uint16_t read_register(uint8_t reg);

	uint8_t m_address;
	uint16_t m_config;
	int m_voltage_range;
};


enum ads1115_register {
	ADS1115_REGISTER_CONVERSION = 0,
	ADS1115_REGISTER_CONFIG = 1,
	ADS1115_REGISTER_LOW_THRESH = 2,
	ADS1115_REGISTER_HIGH_THRESH = 3,
};

#define FACTOR 32768.0
static float ranges[] = { 6.144 / FACTOR, 4.096 / FACTOR, 2.048 / FACTOR, 1.024 / FACTOR, 0.512 / FACTOR, 0.256 / FACTOR};

ADS1115::ADS1115(uint8_t address)
{
        m_address = address;
        m_config = ADS1115_COMP_QUEUE_AFTER_ONE |
                   ADS1115_COMP_LATCH_NO |
                   ADS1115_COMP_POLARITY_ACTIVE_LOW |
                   ADS1115_COMP_MODE_WINDOW |
                   ADS1115_DATA_RATE_128_SPS |
                   ADS1115_MODE_SINGLE_SHOT |
                   ADS1115_MUX_GND_AIN0;
        set_pga(ADS1115_PGA_ONE);
}

uint8_t ADS1115::write_register(uint8_t reg, uint16_t val)
{
        Wire.beginTransmission(m_address);
        Wire.write(reg);
        Wire.write(val>>8);
        Wire.write(val & 0xFF);
        return Wire.endTransmission();
}

uint16_t ADS1115::read_register(uint8_t reg)
{
        Wire.beginTransmission(m_address);
        Wire.write(reg);
        Wire.endTransmission();

        uint8_t result = Wire.requestFrom((int)m_address, 2, 1);
        if (result != 2) {
          return 0;
        }

        uint16_t val;

        val = Wire.read() << 8;
        val |= Wire.read();
        return val;
}

void ADS1115::begin()
{
        Wire.begin();
}

uint8_t ADS1115::trigger_sample()
{
        return write_register(ADS1115_REGISTER_CONFIG, m_config | SAMPLE_BIT);
}

uint8_t ADS1115::reset()
{
	Wire.beginTransmission(0);
	Wire.write(0x6);
	return Wire.endTransmission();
}

bool ADS1115::is_sample_in_progress()
{
	uint16_t val = read_register(ADS1115_REGISTER_CONFIG);
	return (val & SAMPLE_BIT) == 0;
}

int16_t ADS1115::read_sample()
{
        return read_register(ADS1115_REGISTER_CONVERSION);
}

float ADS1115::sample_to_float(int16_t val)
{
	return val * ranges[m_voltage_range];
}

float ADS1115::read_sample_float()
{
	return sample_to_float(read_sample());
}

ADS1115 adc;

void ADS1115_init(void) {

  ads1115_up=0;
  if (!TasmotaGlobal.i2c_enabled) return;

  adc.begin();
  adc.set_data_rate(ADS1115_DATA_RATE_128_SPS);
  adc.set_mode(ADS1115_MODE_CONTINUOUS);
  adc.set_mux(ADS1115_MUX_DIFF_AIN0_AIN3);
  adc.set_pga(ADS1115_PGA_TWO);

  int16_t val = adc.read_sample();
  ads1115_up=1;
}

#endif

char sml_start;
uint8_t dump2log=0;

uint8_t ser_act_LED_pin=255;
uint8_t ser_act_meter_num=0;

#define SML_SAVAILABLE Serial_available()
#define SML_SREAD Serial_read()
#define SML_SPEAK Serial_peek()

bool Serial_available() {
  uint8_t num=dump2log&7;
  if (num<1 || num>meters_used) num=1;
  if (!meter_ss[num-1]) return 0;
  return meter_ss[num-1]->available();
}

uint8_t Serial_read() {
  uint8_t num=dump2log&7;
  if (num<1 || num>meters_used) num=1;
  if (!meter_ss[num-1]) return 0;
  return meter_ss[num-1]->read();
}

uint8_t Serial_peek() {
  uint8_t num=dump2log&7;
  if (num<1 || num>meters_used) num=1;
  if (!meter_ss[num-1]) return 0;
  return meter_ss[num-1]->peek();
}

#ifndef SML_DUMP_SIZE
#define SML_DUMP_SIZE 128
#endif

uint16_t sml_logindex;
char log_data[SML_DUMP_SIZE];

#define SML_EBUS_SKIP_SYNC_DUMPS

void Dump2log(void) {
  int16_t index = 0, hcnt = 0;
  uint32_t d_lastms;
  uint8_t dchars[16];
  uint8_t type = meter_desc_p[(dump2log&7) - 1].type;

  //if (!SML_SAVAILABLE) return;

  if (dump2log&8) {
    // combo mode
    while (SML_SAVAILABLE) {
      log_data[index] = ':';
      index++;
      log_data[index] = ' ';
      index++;
      d_lastms = millis();
      while ((millis() - d_lastms) < 40) {
        if (SML_SAVAILABLE) {
          uint8_t c = SML_SREAD;
          sprintf(&log_data[index], "%02x ", c);
          dchars[hcnt] = c;
          index += 3;
          hcnt++;
          if (hcnt > 15) {
            // line complete, build asci chars
            log_data[index] = '=';
            index++;
            log_data[index] = '>';
            index++;
            log_data[index] = ' ';
            index++;
            for (uint8_t ccnt = 0; ccnt < 16; ccnt++) {
              if (isprint(dchars[ccnt])) {
                log_data[index] = dchars[ccnt];
              } else {
                log_data[index] = ' ';
              }
              index++;
            }
            break;
          }
        }
      }
      if (index > 0) {
        log_data[index] = 0;
        AddLogData(LOG_LEVEL_INFO, log_data);
        index = 0;
        hcnt = 0;
      }
    }
  } else {
    if (type == 'o') {
      // obis
      while (SML_SAVAILABLE) {
        char c = SML_SREAD&0x7f;
        if (c == '\n' || c == '\r') {
          if (sml_logindex > 2) {
            log_data[sml_logindex] = 0;
            AddLogData(LOG_LEVEL_INFO, log_data);
            log_data[0] = ':';
            log_data[1] = ' ';
            sml_logindex = 2;
          }
          continue;
        }
        log_data[sml_logindex] = c;
        if (sml_logindex < sizeof(log_data) - 2) {
          sml_logindex++;
        }
      }
    } else if (type == 'v') {
      // vbus
      uint8_t c;
      while (SML_SAVAILABLE) {
        c = SML_SREAD;
        if (c == VBUS_SYNC) {
          log_data[sml_logindex] = 0;
          AddLogData(LOG_LEVEL_INFO, log_data);
          log_data[0] = ':';
          log_data[1] = ' ';
          sml_logindex = 2;
        }
        sprintf(&log_data[sml_logindex], "%02x ", c);
        if (sml_logindex < sizeof(log_data) - 7) {
          sml_logindex += 3;
        }
      }
    } else if (type == 'e') {
      // ebus
      uint8_t c, p;
      while (SML_SAVAILABLE) {
        c = SML_SREAD;
        if (c == EBUS_SYNC) {
          p = SML_SPEAK;
          if (p != EBUS_SYNC && sml_logindex > 5) {
            // new packet, plot last one
            log_data[sml_logindex] = 0;
            AddLogData(LOG_LEVEL_INFO, log_data);
            strcpy(&log_data[0], ": aa ");
            sml_logindex = 5;
          }
          continue;
        }
        sprintf(&log_data[sml_logindex], "%02x ", c);
        if (sml_logindex < sizeof(log_data) - 7) {
          sml_logindex += 3;
        }
      }
    } else if (type == 's') {
      // sml
      uint8_t c;
      while (SML_SAVAILABLE) {
        c = SML_SREAD;
        if (c == SML_SYNC) {
          log_data[sml_logindex] = 0;
          AddLogData(LOG_LEVEL_INFO, log_data);
          log_data[0] = ':';
          log_data[1] = ' ';
          sml_logindex = 2;
        }
        sprintf(&log_data[sml_logindex], "%02x ", c);
        if (sml_logindex < sizeof(log_data) - 7) {
          sml_logindex += 3;
        }
      }
    } else {
      // raw dump
      d_lastms = millis();
      log_data[0] = ':';
      log_data[1] = ' ';
      sml_logindex = 2;
      while ((millis() - d_lastms) < 40) {
        while (SML_SAVAILABLE) {
          sprintf(&log_data[sml_logindex], "%02x ", SML_SREAD);
          sml_logindex += 3;
        }
      }
      if (sml_logindex > 2) {
        log_data[sml_logindex] = 0;
        AddLogData(LOG_LEVEL_INFO, log_data);
      }
    }
  }
}

void Hexdump(uint8_t *sbuff, uint32_t slen) {
  char cbuff[slen*3+10];
  char *cp = cbuff;
  *cp++ = '>';
  *cp++ = ' ';
  for (uint32_t cnt = 0; cnt < slen; cnt ++) {
    sprintf(cp, "%02x ", sbuff[cnt]);
    cp += 3;
  }
  AddLogData(LOG_LEVEL_INFO, cbuff);
}

#ifdef ED300L
uint8_t sml_status[MAX_METERS];
uint8_t g_mindex;
#endif

// skip sml entries
uint8_t *skip_sml(uint8_t *cp,int16_t *res) {
    uint8_t len,len1,type;
    len=*cp&0xf;
    type=*cp&0x70;
    if (type==0x70) {
        // list, skip entries
        // list
        cp++;
        while (len--) {
            len1=*cp&0x0f;
            cp+=len1;
        }
         *res=0;
    } else {
        // skip len
        *res=(signed char)*(cp+1);
        cp+=len;
    }
    return cp;
}

// get sml binary value
// not defined for unsigned >0x7fff ffff ffff ffff (should never happen)
double sml_getvalue(unsigned char *cp,uint8_t index) {
uint8_t len,unit,type;
int16_t scaler,result;
int64_t value;
double dval;

  // scan for values
  // check status
#ifdef ED300L
  unsigned char *cpx=cp-5;
  // decode OBIS 0180 amd extract direction info
  if (*cp==0x64 && *cpx==0 && *(cpx+1)==0x01 && *(cpx+2)==0x08 && *(cpx+3)==0) {
      sml_status[g_mindex]=*(cp+3);
  }
  if (*cp==0x63 && *cpx==0 && *(cpx+1)==0x01 && *(cpx+2)==0x08 && *(cpx+3)==0) {
      sml_status[g_mindex]=*(cp+2);
  }
#endif

  cp=skip_sml(cp,&result);
  // check time
  cp=skip_sml(cp,&result);
  // check unit
  cp=skip_sml(cp,&result);
  // check scaler
  cp=skip_sml(cp,&result);
  scaler=result;

  // get value
  type=*cp&0x70;
  len=*cp&0x0f;
  cp++;
    if (type==0x50 || type==0x60) {
        // shift into 64 bit
        uint64_t uvalue=0;
        uint8_t nlen=len;
        while (--nlen) {
            uvalue<<=8;
            uvalue|=*cp++;
        }
        if (type==0x50) {
            // signed
            switch (len-1) {
                case 1:
                    // byte
                    value=(signed char)uvalue;
                    break;
                case 2:
                    // signed 16 bit
#ifdef DWS74_BUG
                    if (scaler==-2) {
                      value=(uint32_t)uvalue;
                    } else {
                      value=(int16_t)uvalue;
                    }
#else
                    value=(int16_t)uvalue;
#endif
                    break;
                case 3:
                  // signed 24 bit
                  value=(int32_t)(uvalue<<8);
                  value/=256;
                  break;

                case 4:
                    // signed 32 bit
                    value=(int32_t)uvalue;
                    break;
                case 5:
                case 6:
                case 7:
                case 8:
                    // signed 64 bit
                    value=(int64_t)uvalue;
                    break;
            }
        } else {
            // unsigned
            value=uvalue;
        }

    } else {
        if (!(type&0xf0)) {
            // octet string serial number
            // no coding found on the net
            // up to now 2 types identified on Hager
            if (len==9) {
              // serial number on hager => 24 bit - 24 bit
                cp++;
                uint32_t s1,s2;
                s1=*cp<<16|*(cp+1)<<8|*(cp+2);
                cp+=4;
                s2=*cp<<16|*(cp+1)<<8|*(cp+2);
                sprintf(&meter_id[index][0],"%u-%u",s1,s2);
            } else {
                // server id on hager
                char *str=&meter_id[index][0];
                for (type=0; type<len-1; type++) {
                    sprintf(str,"%02x",*cp++);
                    str+=2;
                }
            }
            value=0;
        } else {
            value=999999;
            scaler=0;
        }
    }
    dval=value;
    if (scaler==-1) {
      dval/=10;
    } else if (scaler==-2) {
      dval/=100;
    } else if (scaler==-3) {
      dval/=1000;
    } else if (scaler==-4) {
      dval/=10000;
    } else if (scaler==1) {
      dval*=10;
    } else if (scaler==2) {
      dval*=100;
    } else if (scaler==3) {
      dval*=1000;
    }
  #ifdef ED300L
    // decode current power OBIS 00 0F 07 00
    if (*cpx==0x00 && *(cpx+1)==0x0f && *(cpx+2)==0x07 && *(cpx+3)==0) {
        if (sml_status[g_mindex]&0x20) {
          // and invert sign on solar feed
          dval*=-1;
        }
    }
  #endif
    return dval;
}

uint8_t hexnibble(char chr) {
  uint8_t rVal = 0;
  if (isdigit(chr)) {
    rVal = chr - '0';
  } else  {
    chr=toupper(chr);
    if (chr >= 'A' && chr <= 'F') rVal = chr + 10 - 'A';
  }
  return rVal;
}

uint8_t sb_counter;

// need double precision in this driver
double CharToDouble(const char *str)
{
  // simple ascii to double, because atof or strtod are too large
  char strbuf[24];

  strlcpy(strbuf, str, sizeof(strbuf));
  char *pt = strbuf;
  while ((*pt != '\0') && isspace(*pt)) { pt++; }  // Trim leading spaces

  signed char sign = 1;
  if (*pt == '-') { sign = -1; }
  if (*pt == '-' || *pt=='+') { pt++; }            // Skip any sign

  double left = 0;
  if (*pt != '.') {
    left = atoi(pt);                               // Get left part
    while (isdigit(*pt)) { pt++; }                 // Skip number
  }

  double right = 0;
  if (*pt == '.') {
    pt++;
    right = atoi(pt);                              // Decimal part
    while (isdigit(*pt)) {
      pt++;
      right /= 10.0;
    }
  }

  double result = left + right;
  if (sign < 0) {
    return -result;                                // Add negative sign
  }
  return result;
}


// remove ebus escapes
void ebus_esc(uint8_t *ebus_buffer, unsigned char len) {
    short count,count1;
    for (count=0; count<len; count++) {
        if (ebus_buffer[count]==EBUS_ESC) {
            //found escape
            ebus_buffer[count]+=ebus_buffer[count+1];
            // remove 2. char
            count++;
            for (count1=count; count1<len; count1++) {
                ebus_buffer[count1]=ebus_buffer[count1+1];
            }
        }
    }

}

uint8_t ebus_crc8(uint8_t data, uint8_t crc_init) {
	uint8_t crc;
	uint8_t polynom;
	int i;

	crc = crc_init;
	for (i = 0; i < 8; i++) {
		if (crc & 0x80) {
			polynom = (uint8_t) 0x9B;
		}
		else {
			polynom = (uint8_t) 0;
		}
		crc = (uint8_t)((crc & ~0x80) << 1);
		if (data & 0x80) {
			crc = (uint8_t)(crc | 1) ;
		}
		crc = (uint8_t)(crc ^ polynom);
		data = (uint8_t)(data << 1);
	}
	return (crc);
}

// ebus crc
uint8_t ebus_CalculateCRC( uint8_t *Data, uint16_t DataLen ) {
	uint16_t i;
	uint8_t Crc = 0;
	for(i = 0 ; i < DataLen ; ++i, ++Data ) {
      Crc = ebus_crc8( *Data, Crc );
   }
   return Crc;
}

void sml_empty_receiver(uint32_t meters) {
  while (meter_ss[meters]->available()) {
    meter_ss[meters]->read();
  }
}


void sml_shift_in(uint32_t meters,uint32_t shard) {
  uint32_t count;
#ifndef SML_OBIS_LINE
  if (meter_desc_p[meters].type!='e' && meter_desc_p[meters].type!='m' && meter_desc_p[meters].type!='M' && meter_desc_p[meters].type!='p' && meter_desc_p[meters].type!='R' && meter_desc_p[meters].type!='v') {
#else
  if (meter_desc_p[meters].type!='o' && meter_desc_p[meters].type!='e' && meter_desc_p[meters].type!='m' && meter_desc_p[meters].type!='M' && meter_desc_p[meters].type!='p' && meter_desc_p[meters].type!='R' && meter_desc_p[meters].type!='v') {
#endif
    // shift in
    for (count=0; count<SML_BSIZ-1; count++) {
      smltbuf[meters][count]=smltbuf[meters][count+1];
    }
  }
  uint8_t iob=(uint8_t)meter_ss[meters]->read();

  if (meter_desc_p[meters].type == 'o') {
#ifndef SML_OBIS_LINE
    smltbuf[meters][SML_BSIZ-1] = iob & 0x7f;
#else
    iob &= 0x7f;
    smltbuf[meters][meter_spos[meters]] = iob;
    meter_spos[meters]++;
    if (meter_spos[meters] >= SML_BSIZ) {
      meter_spos[meters] = 0;
    }
    if (iob == 0x0a) {
      SML_Decode(meters);
      meter_spos[meters] = 0;
    }
#endif
  } else if (meter_desc_p[meters].type=='s') {
    smltbuf[meters][SML_BSIZ-1]=iob;
  } else if (meter_desc_p[meters].type=='r') {
    smltbuf[meters][SML_BSIZ-1]=iob;
  } else if (meter_desc_p[meters].type=='m' || meter_desc_p[meters].type=='M') {
    smltbuf[meters][meter_spos[meters]] = iob;
    meter_spos[meters]++;
    if (meter_spos[meters]>=SML_BSIZ) {
      meter_spos[meters]=0;
    }
    if (meter_spos[meters]>=8) {
      uint32_t mlen=smltbuf[meters][2]+5;
      if (mlen>SML_BSIZ) mlen=SML_BSIZ;
      if (meter_spos[meters]>=mlen) {
        SML_Decode(meters);
        sml_empty_receiver(meters);
        meter_spos[meters]=0;
      }
    }
  } else if (meter_desc_p[meters].type=='p') {
    smltbuf[meters][meter_spos[meters]] = iob;
    meter_spos[meters]++;
    if (meter_spos[meters]>=7) {
      SML_Decode(meters);
      sml_empty_receiver(meters);
      meter_spos[meters]=0;
    }
  } else if (meter_desc_p[meters].type=='R') {
    smltbuf[meters][meter_spos[meters]] = iob;
    meter_spos[meters]++;
    if (meter_spos[meters]>=SML_BSIZ) {
      meter_spos[meters]=0;
    }
  } else if (meter_desc_p[meters].type=='v') {
    if (iob==EBUS_SYNC) {
      sb_counter = 0;
      SML_Decode(meters);
      smltbuf[meters][0] = iob;
      meter_spos[meters] = 1;
    } else {
      if (meter_spos[meters] < SML_BSIZ) {
        smltbuf[meters][meter_spos[meters]] = iob;
        meter_spos[meters]++;
      }
    }
  }
  else {
    if (iob==EBUS_SYNC) {
    	// should be end of telegramm
      // QQ,ZZ,PB,SB,NN ..... CRC, ACK SYNC
      if (meter_spos[meters]>4+5) {
      	// get telegramm lenght
        uint16_t tlen=smltbuf[meters][4]+5;
        // test crc
        if (smltbuf[meters][tlen]=ebus_CalculateCRC(smltbuf[meters],tlen)) {
            ebus_esc(smltbuf[meters],tlen);
            SML_Decode(meters);
        } else {
            // crc error
            //AddLog(LOG_LEVEL_INFO, PSTR("ebus crc error"));
        }
      }
      meter_spos[meters] = 0;
      return;
    }
		smltbuf[meters][meter_spos[meters]] = iob;
		meter_spos[meters]++;
		if (meter_spos[meters]>=SML_BSIZ) {
			meter_spos[meters]=0;
		}
  }
  sb_counter++;
#ifndef SML_OBIS_LINE
  if (meter_desc_p[meters].type!='e' && meter_desc_p[meters].type!='m' && meter_desc_p[meters].type!='M' && meter_desc_p[meters].type!='p' && meter_desc_p[meters].type!='R' && meter_desc_p[meters].type!='v') SML_Decode(meters);
#else
  if (meter_desc_p[meters].type!='o' && meter_desc_p[meters].type!='e' && meter_desc_p[meters].type!='m' && meter_desc_p[meters].type!='M' && meter_desc_p[meters].type!='p' && meter_desc_p[meters].type!='R' && meter_desc_p[meters].type!='v') SML_Decode(meters);
#endif
}


// polled every 50 ms
void SML_Poll(void) {
uint32_t meters;

    for (meters = 0; meters < meters_used; meters++) {
      if (meter_desc_p[meters].type != 'c') {
        // poll for serial input
        if (!meter_ss[meters]) continue;
        if (ser_act_LED_pin!=255 && (ser_act_meter_num==0 || ser_act_meter_num-1==meters)) {
          digitalWrite(ser_act_LED_pin, meter_ss[meters]->available() && !digitalRead(ser_act_LED_pin)); // Invert LED, if queue is continuously full
        }
        while (meter_ss[meters]->available()) {
          sml_shift_in(meters, 0);
        }
      }
    }
}

#define VBUS_BAD_CRC 0
// get vbus septet with 6 bytes
uint32_t vbus_get_septet(uint8_t *cp) {
  uint32_t result = 0;

  //AddLog(LOG_LEVEL_INFO,PSTR("septet: %02x %02x %02x %02x %02x %02x"),cp[0] ,cp[1],cp[2],cp[3],cp[4],cp[5]);

  uint8_t Crc = 0x7F;
  for (uint32_t i = 0; i < 5; i++) {
    Crc = (Crc - cp[i]) & 0x7f;
  }
  if (Crc != cp[5]) {
    result = VBUS_BAD_CRC;
  } else {
    result = (cp[3] | ((cp[4]&8)<<4));
    result <<= 8;
    result |= (cp[2] | ((cp[4]&4)<<5));
    result <<= 8;
    result |= (cp[1] | ((cp[4]&2)<<6));
    result <<= 8;
    result |= (cp[0] | ((cp[4]&1)<<7));
  }

  //AddLog(LOG_LEVEL_INFO,PSTR("septet r: %d"),result);
  return result;
}


void SML_Decode(uint8_t index) {
  const char *mp=(const char*)meter_p;
  int8_t mindex;
  uint8_t *cp;
  uint8_t dindex = 0, vindex = 0;
  delay(0);

  while (mp != NULL) {
    // check list of defines
    if (*mp == 0) break;

    // new section
    mindex = ((*mp) & 7) - 1;

    if (mindex < 0 || mindex >= meters_used) mindex = 0;
    mp += 2;
    if (*mp == '=' && *(mp+1) == 'h') {
      mp = strchr(mp, '|');
      if (mp) mp++;
      continue;
    }

    // =d must handle dindex
    if (*mp == '=' && *(mp + 1) == 'd') {
      if (index != mindex) {
        dindex++;
      }
    }

    if (index!=mindex) goto nextsect;

    // start of serial source buffer
    cp = &smltbuf[mindex][0];

    // compare
    if (*mp == '=') {
      // calculated entry, check syntax
      mp++;
      // do math m 1+2+3
      if (*mp == 'm' && !sb_counter) {
        // only every 256 th byte
        // else it would be calculated every single serial byte
        mp++;
        while (*mp == ' ') mp++;
        // 1. index
        double dvar;
        uint8_t opr;
        uint8_t mind;
        int32_t ind;
        mind = strtol((char*)mp, (char**)&mp, 10);
        if (mind < 1 || mind > SML_MAX_VARS) mind = 1;
        dvar = meter_vars[mind - 1];
        while (*mp==' ') mp++;
        for (uint8_t p = 0; p < 8; p++) {
          if (*mp == '@') {
            // store result
            meter_vars[vindex] = dvar;
            mp++;
            break;
          }
          opr = *mp;
          mp++;
          uint8_t iflg = 0;
          if (*mp == '#') {
            iflg = 1;
            mp++;
          }
          ind = strtol((char*)mp, (char**)&mp, 10);
          mind = ind;
          if (mind < 1 || mind > SML_MAX_VARS) mind = 1;
          switch (opr) {
              case '+':
                if (iflg) dvar += ind;
                else dvar += meter_vars[mind - 1];
                break;
              case '-':
                if (iflg) dvar -= ind;
                else dvar -= meter_vars[mind - 1];
                break;
              case '*':
                if (iflg) dvar *= ind;
                else dvar *= meter_vars[mind - 1];
                break;
              case '/':
                if (iflg) dvar /= ind;
                else dvar /= meter_vars[mind - 1];
                break;
          }
          while (*mp==' ') mp++;
          if (*mp == '@') {
            // store result
            meter_vars[vindex] = dvar;
            mp++;
            break;
          }
        }
        double fac = CharToDouble((char*)mp);
        meter_vars[vindex] /= fac;
        SML_Immediate_MQTT((const char*)mp, vindex, mindex);
        dvalid[vindex] = 1;
        // get sfac
      } else if (*mp=='d') {
        // calc deltas d ind 10 (eg every 10 secs)
        if (dindex < MAX_DVARS) {
          // only n indexes
          mp++;
          while (*mp == ' ') mp++;
          uint8_t ind = atoi(mp);
          while (*mp >= '0' && *mp <= '9') mp++;
          if (ind < 1 || ind > SML_MAX_VARS) ind = 1;
          uint32_t delay = atoi(mp) * 1000;
          uint32_t dtime = millis() - dtimes[dindex];
          if (dtime > delay) {
            // calc difference
            dtimes[dindex] = millis();
            double vdiff = meter_vars[ind - 1] - dvalues[dindex];
            dvalues[dindex] = meter_vars[ind - 1];
            double dres = (double)360000.0 * vdiff / ((double)dtime / 10000.0);

            dvalid[vindex] += 1;

            if (dvalid[vindex] >= 2) {
              // differece is only valid after 2. calculation
              dvalid[vindex] = 2;
#ifdef USE_SML_MEDIAN_FILTER
              if (meter_desc_p[mindex].flag & 16) {
                meter_vars[vindex] = sml_median(&sml_mf[vindex], dres);
              } else {
                meter_vars[vindex] = dres;
              }
#else
              meter_vars[vindex] = dres;
#endif
            }
            mp=strchr(mp,'@');
            if (mp) {
              mp++;
              double fac = CharToDouble((char*)mp);
              meter_vars[vindex] /= fac;
              SML_Immediate_MQTT((const char*)mp, vindex, mindex);
            }
          }
          //dvalid[vindex] = 1;
          dindex++;
        }
      } else if (*mp == 'h') {
        // skip html tag line
        mp = strchr(mp, '|');
        if (mp) mp++;
        continue;
      }
    } else {
      // compare value
      uint8_t found = 1;
      double ebus_dval = 99;
      float mbus_dval = 99;
      while (*mp != '@') {
        if (meter_desc_p[mindex].type == 'o' || meter_desc_p[mindex].type == 'c') {
          if (*mp++ != *cp++) {
            found=0;
          }
        } else {
          if (meter_desc_p[mindex].type == 's') {
            // sml
            uint8_t val = hexnibble(*mp++) << 4;
            val |= hexnibble(*mp++);
            if (val != *cp++) {
              found=0;
            }
          } else {
            // ebus mbus pzem vbus or raw
            // XXHHHHSSUU
            if (*mp == 'x') {
              if (*(mp + 1) == 'x') {
                //ignore one byte
                mp += 2;
                cp++;
              } else {
                mp++;
                if (isdigit(*mp)) {
                  uint8_t skip = strtol((char*)mp, (char**)&mp, 10);
                  cp += skip;
                }
              }
            } else if (!strncmp(mp, "UUuuUUuu", 8)) {
              uint32_t val = (cp[0]<<24) | (cp[1]<<16) | (cp[2]<<8) | (cp[3]<<0);
              mp += 8;
              cp += 4;
              if (*mp == 's') {
                mp++;
                // swap words
                val = (val>>16) | (val<<16);
              }
              ebus_dval = val;
              mbus_dval = val;
            } else if (!strncmp(mp, "uuUUuuUU", 8)) {
              uint32_t val = (cp[1]<<24) | (cp[0]<<16) | (cp[3]<<8) | (cp[2]<<0);
              mp += 8;
              cp += 4;
              if (*mp == 's') {
                mp++;
                // swap words
                val = (val>>16) | (val<<16);
              }
              ebus_dval = val;
              mbus_dval = val;
            } else if (!strncmp(mp, "UUuu", 4)) {
              uint16_t val = cp[1] | (cp[0]<<8);
              mbus_dval = val;
              ebus_dval = val;
              mp += 4;
              cp += 2;
            } else if (!strncmp(mp, "SSssSSss", 8)) {
              int32_t val = (cp[0]<<24) | (cp[1]<<16) | (cp[2]<<8) | (cp[3]<<0);
              mp += 8;
              cp += 4;
              if (*mp == 's') {
                mp++;
                // swap words
                val = ((uint32_t)val>>16) | ((uint32_t)val<<16);
              }
              ebus_dval = val;
              mbus_dval = val;
            } else if (!strncmp(mp, "ssSSssSS", 8)) {
              int32_t val = (cp[1]<<24) | (cp[0]<<16) | (cp[3]<<8) | (cp[2]<<0);
              mp += 8;
              cp += 4;
              if (*mp == 's') {
                mp++;
                // swap words
                val = ((uint32_t)val>>16) | ((uint32_t)val<<16);
              }
              ebus_dval = val;
              mbus_dval = val;
            } else if (!strncmp(mp, "uuUU", 4)) {
              uint16_t val = cp[0] | (cp[1]<<8);
              mbus_dval = val;
              ebus_dval = val;
              mp += 4;
              cp += 2;
            } else if (!strncmp(mp, "uu", 2)) {
              uint8_t val = *cp++;
              mbus_dval = val;
              ebus_dval = val;
              mp += 2;
            } else if (!strncmp(mp, "ssSS", 4)) {
              int16_t val = *cp | (*(cp+1)<<8);
              mbus_dval = val;
              ebus_dval = val;
              mp += 4;
              cp += 2;
            } else if (!strncmp(mp, "SSss", 4)) {
              int16_t val = cp[1] | (cp[0]<<8);
              mbus_dval = val;
              ebus_dval = val;
              mp += 4;
              cp += 2;
            } else if (!strncmp(mp,"ss", 2)) {
              int8_t val = *cp++;
              mbus_dval = val;
              ebus_dval = val;
              mp += 2;
            } else if (!strncmp(mp, "ffffffff", 8)) {
              uint32_t val = (cp[0]<<24) | (cp[1]<<16) | (cp[2]<<8) | (cp[3]<<0);
              float *fp = (float*)&val;
              ebus_dval = *fp;
              mbus_dval = *fp;
              mp += 8;
              cp += 4;
            } else if (!strncmp(mp, "FFffFFff", 8)) {
              // reverse word float
              uint32_t val = (cp[1]<<0) | (cp[0]<<8) | (cp[3]<<16) | (cp[2]<<24);
              float *fp = (float*)&val;
              ebus_dval = *fp;
              mbus_dval = *fp;
              mp += 8;
              cp += 4;
            } else if (!strncmp(mp, "eeeeee", 6)) {
              uint32_t val = (cp[0]<<16) | (cp[1]<<8) | (cp[2]<<0);
              mbus_dval = val;
              mp += 6;
              cp += 3;
            } else if (!strncmp(mp, "vvvvvv", 6)) {
              mbus_dval = (float)((cp[0]<<8) | (cp[1])) + ((float)cp[2]/10.0);
              mp += 6;
              cp += 3;
            } else if (!strncmp(mp, "cccccc", 6)) {
              mbus_dval = (float)((cp[0]<<8) | (cp[1])) + ((float)cp[2]/100.0);
              mp += 6;
              cp += 3;
            } else if (!strncmp(mp, "pppp", 4)) {
              mbus_dval = (float)((cp[0]<<8) | cp[1]);
              mp += 4;
              cp += 2;
            } else if (*mp == 'v') {
              // vbus values vul, vsl, vuwh, vuwl, wswh, vswl, vswh
              // vub3, vsb3 etc
              mp++;
              int16_t offset = -1;
              if (*mp == 'o') {
                mp++;
                offset = strtol((char*)mp, (char**)&mp, 10);
                cp += (offset / 4) * 6;
              }
              uint8_t usign;
              if (*mp == 'u') {
                usign = 1;
              } else if (*mp == 's') {
                usign = 0;
              }
              mp++;
              switch (*mp) {
                case 'l':
                  mp++;
                  // get long value
                  if (usign) {
                    ebus_dval = vbus_get_septet(cp);
                  } else {
                    ebus_dval = (int32_t)vbus_get_septet(cp);
                  }
                  break;
                case 'w':
                  mp++;
                  char wflg;
                  if (offset >= 0) {
                    if (offset % 4) {
                      wflg = 'h';
                    } else {
                      wflg = 'l';
                    }
                  } else {
                    wflg = *mp;
                    mp++;
                  }
                  // get word value
                  if (wflg == 'h') {
                    // high word
                    if (usign) ebus_dval = (vbus_get_septet(cp) >> 16) & 0xffff;
                    else ebus_dval = (int16_t)((vbus_get_septet(cp) >> 16) & 0xffff);
                  } else {
                    // low word
                    if (usign) ebus_dval = vbus_get_septet(cp) & 0xffff;
                    else ebus_dval = (int16_t)(vbus_get_septet(cp) & 0xffff);
                  }
                  break;
                case 'b':
                  mp++;
                  char bflg;
                  if (offset >= 0) {
                    bflg = 0x30 | (offset % 4);
                  } else {
                    bflg = *mp;
                    mp++;
                  }
                  switch (bflg) {
                    case '3':
                      if (usign) ebus_dval = vbus_get_septet(cp) >> 24;
                      else ebus_dval = (int8_t)(vbus_get_septet(cp) >> 24);
                      break;
                    case '2':
                      if (usign) ebus_dval = (vbus_get_septet(cp) >> 16) & 0xff;
                      else ebus_dval = (int8_t)((vbus_get_septet(cp) >> 16) & 0xff);
                      break;
                    case '1':
                      if (usign) ebus_dval = (vbus_get_septet(cp) >> 8) & 0xff;
                      else ebus_dval = (int8_t)((vbus_get_septet(cp) >> 8) & 0xff);
                      break;
                    case '0':
                      if (usign) ebus_dval = vbus_get_septet(cp) & 0xff;
                      else ebus_dval = (int8_t)(vbus_get_septet(cp) & 0xff);
                      break;
                  }
                  break;
                case 't':
                  mp++;
                  { uint16_t time;
                    if (offset % 4) {
                      time = (vbus_get_septet(cp) >> 16) & 0xffff;
                    } else {
                      time = vbus_get_septet(cp) & 0xffff;
                    }
                    sprintf(&meter_id[mindex][0], "%02d:%02d", time / 60, time % 60);
                  }
                  break;
              }
              cp += 6;
            }
            else {
              uint8_t val = hexnibble(*mp++) << 4;
              val |= hexnibble(*mp++);
              if (val!=*cp++) {
                found=0;
              }
            }
          }
        }
      }
      if (found) {
        // matches, get value
        dvalid[vindex] = 1;
        mp++;
#ifdef ED300L
        g_mindex=mindex;
#endif
        if (*mp == '#') {
          // get string value
          getstr:
          mp++;
          if (meter_desc_p[mindex].type != 'v') {
            if (meter_desc_p[mindex].type == 'o') {
              uint32_t p;
              for (p = 0; p < METER_ID_SIZE - 2; p++) {
                if (*cp == *mp) {
                  break;
                }
                meter_id[mindex][p] = *cp++;
              }
              meter_id[mindex][p] = 0;
            } else {
              sml_getvalue(cp,mindex);
            }
          }
        } else {
          double dval;
          if (meter_desc_p[mindex].type!='e' && meter_desc_p[mindex].type!='r' && meter_desc_p[mindex].type!='m' && meter_desc_p[mindex].type!='M' && meter_desc_p[mindex].type!='p' && meter_desc_p[mindex].type!='v') {
            // get numeric values
            if (meter_desc_p[mindex].type=='o' || meter_desc_p[mindex].type=='c') {
              if (*mp == '(') {
                mp++;
                // skip this number of brackets
                uint8_t toskip = strtol((char*)mp,(char**)&mp, 10);
                mp++;
                char *lcp = (char*)cp;
                if (toskip) {
                  char *bp = (char*)cp;
                  for (uint32_t cnt = 0; cnt < toskip; cnt++) {
                    bp = strchr(bp, '(');
                    if (!bp) {
                      break;
                    }
                    bp++;
                    lcp = bp;
                  }
                }
                if (*mp=='#') {
                  cp = (uint8_t*)lcp;
                  goto getstr;
                }
                dval=CharToDouble((char*)lcp);
              } else {
                dval=CharToDouble((char*)cp);
              }
            } else {
              dval = sml_getvalue(cp,mindex);
            }
          } else {
            // ebus pzem vbus or mbus or raw
            if (*mp=='b') {
              mp++;
              uint8_t shift = *mp&7;
              ebus_dval = (uint32_t)ebus_dval>>shift;
              ebus_dval = (uint32_t)ebus_dval&1;
              mp+=2;
            }
            if (*mp=='i') {
              // mbus index
              mp++;
              uint8_t mb_index=strtol((char*)mp,(char**)&mp,10);
              if (mb_index!=meter_desc_p[mindex].index) {
                goto nextsect;
              }
              uint16_t pos = smltbuf[mindex][2]+3;
              if (pos>32) pos=32;
              uint16_t crc = MBUS_calculateCRC(&smltbuf[mindex][0],pos);
              if (lowByte(crc)!=smltbuf[mindex][pos]) goto nextsect;
              if (highByte(crc)!=smltbuf[mindex][pos+1]) goto nextsect;
              dval=mbus_dval;
              //AddLog(LOG_LEVEL_INFO, PSTR(">> %s"),mp);
              mp++;
            } else {
              if (meter_desc_p[mindex].type=='p') {
                uint8_t crc = SML_PzemCrc(&smltbuf[mindex][0],6);
                if (crc!=smltbuf[mindex][6]) goto nextsect;
                dval=mbus_dval;
              } else {
                dval=ebus_dval;
              }
            }

          }
#ifdef USE_SML_MEDIAN_FILTER
          if (meter_desc_p[mindex].flag&16) {
            meter_vars[vindex]=sml_median(&sml_mf[vindex],dval);
          } else {
            meter_vars[vindex]=dval;
          }
#else
          meter_vars[vindex]=dval;
#endif

          //AddLog(LOG_LEVEL_INFO, PSTR(">> %s"),mp);
          // get scaling factor
          double fac = CharToDouble((char*)mp);
          meter_vars[vindex] /= fac;
          SML_Immediate_MQTT((const char*)mp, vindex, mindex);
        }
      }
      //AddLog(LOG_LEVEL_INFO, PSTR("set valid in line %d"), vindex);
    }
nextsect:
    // next section
    if (vindex<SML_MAX_VARS-1) {
      vindex++;
    }
    mp = strchr(mp, '|');
    if (mp) mp++;
  }
}

//"1-0:1.8.0*255(@1," D_TPWRIN ",kWh," DJ_TPWRIN ",4|"
void SML_Immediate_MQTT(const char *mp,uint8_t index,uint8_t mindex) {
  char tpowstr[32];
  char jname[24];

  // we must skip sf,webname,unit
  char *cp=strchr(mp,',');
  if (cp) {
    cp++;
    // wn
    cp=strchr(cp,',');
    if (cp) {
      cp++;
      // unit
      cp=strchr(cp,',');
      if (cp) {
        cp++;
        // json mqtt
        for (uint8_t count=0;count<sizeof(jname);count++) {
          if (*cp==',') {
            jname[count]=0;
            break;
          }
          jname[count]=*cp++;
        }
        cp++;
        uint8_t dp=atoi(cp);
        if (dp&0x10) {
          // immediate mqtt
          dtostrfd(meter_vars[index],dp&0xf,tpowstr);
          ResponseTime_P(PSTR(",\"%s\":{\"%s\":%s}}"),meter_desc_p[mindex].prefix,jname,tpowstr);
          MqttPublishTeleSensor();
        }
      }
    }
  }
}

// web + json interface
void SML_Show(boolean json) {
  int8_t count, mindex, cindex = 0;
  char tpowstr[32];
  char name[24];
  char unit[8];
  char jname[24];
  int8_t index=0,mid=0;
  char *mp=(char*)meter_p;
  char *cp, nojson = 0;
  //char b_mqtt_data[MESSZ];
  //b_mqtt_data[0]=0;

    if (!meters_used) return;

    int8_t lastmind=((*mp)&7)-1;
    if (lastmind<0 || lastmind>=meters_used) lastmind=0;
    while (mp != NULL) {
        if (*mp==0) break;
        // setup sections
        mindex=((*mp)&7)-1;

        if (mindex<0 || mindex>=meters_used) mindex=0;
        if (meter_desc_p[mindex].prefix[0]=='*' && meter_desc_p[mindex].prefix[1]==0) {
          nojson = 1;
        } else {
          nojson = 0;
        }
        mp+=2;
        if (*mp=='=' && *(mp+1)=='h') {
          mp+=2;
          // html tag
          if (json) {
            mp = strchr(mp, '|');
            if (mp) mp++;
            continue;
          }
          // web ui export
          uint8_t i;
          for (i=0;i<sizeof(tpowstr)-2;i++) {
            if (*mp=='|' || *mp==0) break;
            tpowstr[i]=*mp++;
          }
          tpowstr[i]=0;
          // export html
          //snprintf_P(b_mqtt_data, sizeof(b_mqtt_data), "%s{s}%s{e}", b_mqtt_data,tpowstr);
          WSContentSend_PD(PSTR("{s}%s{e}"),tpowstr);
          // rewind, to ensure strchr
          mp--;
          mp = strchr(mp, '|');
          if (mp) mp++;
          continue;
        }
        // skip compare section
        cp=strchr(mp,'@');
        if (cp) {
          cp++;
          tststr:
          if (*cp=='#') {
            // meter id
            sprintf(tpowstr,"\"%s\"",&meter_id[mindex][0]);
            mid=1;
          } else if (*cp=='(') {
            if (meter_desc_p[mindex].type=='o') {
              cp++;
              strtol((char*)cp,(char**)&cp, 10);
              cp++;
              goto tststr;
            } else {
              mid=0;
            }
          } else if (*cp=='b') {
            // bit value
#ifdef SML_BIT_TEXT
            sprintf_P(tpowstr, PSTR("\"%s\""), (uint8_t)meter_vars[index]?D_ON:D_OFF);
            mid = 2;
#endif
          } else {
            mid = 0;
          }
          // skip scaling
          cp=strchr(cp,',');
          if (cp) {
            // this is the name in web UI
            cp++;
            for (count=0;count<sizeof(name);count++) {
              if (*cp==',') {
                name[count]=0;
                break;
              }
              name[count]=*cp++;
            }
            cp++;

            for (count=0;count<sizeof(unit);count++) {
              if (*cp==',') {
                unit[count]=0;
                break;
              }
              unit[count]=*cp++;
            }
            cp++;

            for (count=0;count<sizeof(jname);count++) {
              if (*cp==',') {
                jname[count]=0;
                break;
              }
              jname[count]=*cp++;
            }

            cp++;

            if (!mid) {
              uint8_t dp=atoi(cp)&0xf;
              dtostrfd(meter_vars[index],dp,tpowstr);
            }

            if (json) {
              //if (dvalid[index]) {

                //AddLog(LOG_LEVEL_INFO, PSTR("not yet valid line %d"), index);
              //}
              // json export
              if (index==0) {
                  //snprintf_P(b_mqtt_data, sizeof(b_mqtt_data), "%s,\"%s\":{\"%s\":%s", b_mqtt_data,meter_desc_p[mindex].prefix,jname,tpowstr);
                  if (!nojson) {
                    ResponseAppend_P(PSTR(",\"%s\":{\"%s\":%s"),meter_desc_p[mindex].prefix,jname,tpowstr);
                  }
              }
              else {
                if (lastmind!=mindex) {
                  // meter changed, close mqtt
                  //snprintf_P(b_mqtt_data, sizeof(b_mqtt_data), "%s}", b_mqtt_data);
                  if (!nojson) {
                     ResponseAppend_P(PSTR("}"));
                   }
                    // and open new
                    //snprintf_P(b_mqtt_data, sizeof(b_mqtt_data), "%s,\"%s\":{\"%s\":%s", b_mqtt_data,meter_desc_p[mindex].prefix,jname,tpowstr);
                  if (!nojson) {
                    ResponseAppend_P(PSTR(",\"%s\":{\"%s\":%s"),meter_desc_p[mindex].prefix,jname,tpowstr);
                  }
                  lastmind=mindex;
                } else {
                  //snprintf_P(b_mqtt_data, sizeof(b_mqtt_data), "%s,\"%s\":%s", b_mqtt_data,jname,tpowstr);
                  if (!nojson) {
                    ResponseAppend_P(PSTR(",\"%s\":%s"),jname,tpowstr);
                  }
                }
              }

            } else {
              // web ui export
              //snprintf_P(b_mqtt_data, sizeof(b_mqtt_data), "%s{s}%s %s: {m}%s %s{e}", b_mqtt_data,meter_desc[mindex].prefix,name,tpowstr,unit);
             if (strcmp(name,"*"))  WSContentSend_PD(PSTR("{s}%s %s {m}%s %s{e}"),meter_desc_p[mindex].prefix,name,tpowstr,unit);
            }
          }
        }
        if (index<SML_MAX_VARS-1) {
          index++;
        }
        // next section
        mp = strchr(cp, '|');
        if (mp) mp++;
    }
    if (json) {
     //snprintf_P(b_mqtt_data, sizeof(b_mqtt_data), "%s}", b_mqtt_data);
     //ResponseAppend_P(PSTR("%s"),b_mqtt_data);
     if (!nojson) {
       ResponseAppend_P(PSTR("}"));
     }
   } else {
     //WSContentSend_PD(PSTR("%s"),b_mqtt_data);
   }



#ifdef USE_DOMOTICZ
  if (json && !TasmotaGlobal.tele_period) {
    char str[16];
    dtostrfd(meter_vars[0], 1, str);
    DomoticzSensorPowerEnergy(meter_vars[1], str);  // PowerUsage, EnergyToday
    dtostrfd(meter_vars[2], 1, str);
    DomoticzSensor(DZ_VOLTAGE, str);  // Voltage
    dtostrfd(meter_vars[3], 1, str);
    DomoticzSensor(DZ_CURRENT, str);  // Current
  }
#endif  // USE_DOMOTICZ

}

struct SML_COUNTER {
  uint8_t sml_cnt_debounce;
  uint8_t sml_cnt_old_state;
  uint32_t sml_cnt_last_ts;
  uint32_t sml_counter_ltime;
  uint16_t sml_debounce;
  uint8_t sml_cnt_updated;

#ifdef ANALOG_OPTO_SENSOR
  int16_t ana_curr;
  int16_t ana_max;
  int16_t ana_min;
  int16_t ana_cmpl;
  int16_t ana_cmph;
#endif
} sml_counters[MAX_COUNTERS];

uint8_t sml_counter_pinstate;

uint8_t sml_cnt_index[MAX_COUNTERS] =  { 0, 1, 2, 3 };
void IRAM_ATTR SML_CounterIsr(void *arg) {
uint32_t index = *static_cast<uint8_t*>(arg);

uint32_t time = millis();
uint32_t debounce_time;

  if (digitalRead(meter_desc_p[sml_counters[index].sml_cnt_old_state].srcpin) == bitRead(sml_counter_pinstate, index)) {
    return;
  }

  debounce_time = time - sml_counters[index].sml_counter_ltime;

  if (debounce_time <= sml_counters[index].sml_debounce) return;

  if bitRead(sml_counter_pinstate, index) {
    // falling edge
    RtcSettings.pulse_counter[index]++;
    sml_counters[index].sml_cnt_updated = 1;
  }
  sml_counters[index].sml_counter_ltime = time;
  sml_counter_pinstate ^= (1 << index);
}


#ifdef USE_SCRIPT
struct METER_DESC  script_meter_desc[MAX_METERS];
uint8_t *script_meter;
#endif

#ifndef METER_DEF_SIZE
#define METER_DEF_SIZE 3000
#endif



#ifdef SML_REPLACE_VARS

#ifndef SML_SRCBSIZE
#define SML_SRCBSIZE 256
#endif

uint32_t SML_getlinelen(char *lp) {
uint32_t cnt;
  for (cnt=0; cnt<SML_SRCBSIZE-1; cnt++) {
    if (lp[cnt]==SCRIPT_EOL) {
      break;
    }
  }
  return cnt;
}

uint32_t SML_getscriptsize(char *lp) {
uint32_t mlen=0;
char dstbuf[SML_SRCBSIZE*2];
  while (1) {
    Replace_Cmd_Vars(lp,1,dstbuf,sizeof(dstbuf));
    lp+=SML_getlinelen(lp)+1;
    uint32_t slen=strlen(dstbuf);
    //AddLog(LOG_LEVEL_INFO, PSTR("%d - %s"),slen,dstbuf);
    mlen+=slen+1;
    if (*lp=='#') break;
    if (*lp=='>') break;
    if (*lp==0) break;
  }
  //AddLog(LOG_LEVEL_INFO, PSTR("len=%d"),mlen);
  return mlen+32;
}
#else
uint32_t SML_getscriptsize(char *lp) {
  uint32_t mlen=0;
  for (uint32_t cnt=0;cnt<METER_DEF_SIZE-1;cnt++) {
    if (lp[cnt]=='\n' && lp[cnt+1]=='#') {
      mlen=cnt+3;
      break;
    }
  }
  //AddLog(LOG_LEVEL_INFO, PSTR("len=%d"),mlen);
  return mlen;
}
#endif

bool Gpio_used(uint8_t gpiopin) {
  if ((gpiopin < nitems(TasmotaGlobal.gpio_pin)) && (TasmotaGlobal.gpio_pin[gpiopin] > 0)) {
    return true;
  }
  return false;
}

void SML_Init(void) {
  meters_used=METERS_USED;
  meter_desc_p=meter_desc;
  meter_p=meter;

  sml_desc_cnt=0;

  for (uint32_t cnt=0;cnt<SML_MAX_VARS;cnt++) {
    meter_vars[cnt]=0;
    dvalid[cnt]=0;
  }

  for (uint32_t cnt=0;cnt<MAX_METERS;cnt++) {
    meter_spos[cnt]=0;
  }

#ifdef USE_SCRIPT

  for (uint32_t cnt=0;cnt<MAX_METERS;cnt++) {
    if (script_meter_desc[cnt].txmem) {
     free(script_meter_desc[cnt].txmem);
    }
    script_meter_desc[cnt].txmem=0;
    script_meter_desc[cnt].trxpin=-1;

    if (meter_ss[cnt]) {
        delete meter_ss[cnt];
        meter_ss[cnt]=NULL;
    }

  }

  if (bitRead(Settings->rule_enabled, 0)) {

  uint8_t meter_script=Run_Scripter(">M",-2,0);
  if (meter_script==99) {
    // use script definition
    if (script_meter) free(script_meter);
    script_meter = 0;
    uint8_t *tp = 0;
    uint16_t index = 0;
    uint8_t section = 0;
    int8_t srcpin = 0;
    uint8_t dec_line = 0;
    char *lp = glob_script_mem.scriptptr;
    sml_send_blocks = 0;
    while (lp) {
      if (!section) {
        if (*lp == '>' && *(lp+1) == 'M') {
          lp += 2;
          meters_used = strtol(lp, 0, 10);
          section = 1;
          uint32_t mlen = SML_getscriptsize(lp);
          if (mlen == 0) return; // missing end #
          script_meter = (uint8_t*)calloc(mlen, 1);
          if (!script_meter) {
            goto dddef_exit;
          }
          tp = script_meter;
          goto next_line;
        }
      }
      else {
        if (!*lp || *lp == '#' || *lp == '>') {
          if (*(tp-1) == '|') *(tp-1) = 0;
          break;
        }
        if (*lp == '+') {
          // add descriptor +1,1,c,0,10,H20
          //toLogEOL(">>",lp);
          lp++;
          index = *lp&7;
          lp += 2;
          if (index < 1 || index > meters_used) {
            AddLog(LOG_LEVEL_INFO, PSTR("illegal meter number!"));
            goto next_line;
          }
          index--;
          srcpin  = strtol(lp,&lp,10);
          if (Gpio_used(abs(srcpin))) {
            AddLog(LOG_LEVEL_INFO, PSTR("SML: Error: Duplicate GPIO %d defined. Not usable for RX in meter number %d"),abs(srcpin),index+1);
dddef_exit:
            if (script_meter) free(script_meter);
            script_meter = 0;
            meters_used = METERS_USED;
            goto init10;
          }
          script_meter_desc[index].srcpin = srcpin;
          if (*lp != ',') goto next_line;
          lp++;
          script_meter_desc[index].type = *lp;
          lp++;
          if (*lp != ',') {
            switch (*lp) {
              case 'N':
                lp++;
                script_meter_desc[index].sopt = 0x10 | (*lp & 3);
                lp++;
                break;
              case 'E':
                lp++;
                script_meter_desc[index].sopt = 0x20 | (*lp & 3);
                lp++;
                break;
              case 'O':
                lp++;
                script_meter_desc[index].sopt = 0x30 | (*lp & 3);
                lp++;
                break;
              default:
                script_meter_desc[index].sopt = *lp&7;
                lp++;
            }
          } else {
            script_meter_desc[index].sopt = 0;
          }
          lp++;
          script_meter_desc[index].flag = strtol(lp, &lp, 10);
          if (*lp != ',') goto next_line;
          lp++;
          script_meter_desc[index].params = strtol(lp, &lp, 10);
          if (*lp != ',') goto next_line;
          lp++;
          script_meter_desc[index].prefix[7] = 0;
          for (uint32_t cnt = 0; cnt < 8; cnt++) {
            if (*lp == SCRIPT_EOL || *lp == ',') {
              script_meter_desc[index].prefix[cnt] = 0;
              break;
            }
            script_meter_desc[index].prefix[cnt] = *lp++;
          }
          if (*lp == ',') {
            lp++;
            script_meter_desc[index].trxpin = strtol(lp, &lp, 10);
            if (Gpio_used(script_meter_desc[index].trxpin)) {
              AddLog(LOG_LEVEL_INFO, PSTR("SML: Error: Duplicate GPIO %d defined. Not usable for TX in meter number %d"),script_meter_desc[index].trxpin,index+1);
              goto dddef_exit;
            }
            if (*lp != ',') goto next_line;
            lp++;
            script_meter_desc[index].tsecs = strtol(lp, &lp, 10);
            if (*lp == ',') {
              lp++;
#if 1
              // look ahead
              uint16_t txlen = 0;
              uint16_t tx_entries = 1;
              char *txp = lp;
              while (*txp) {
                if (*txp == ',') tx_entries++;
                if (*txp == SCRIPT_EOL) {
                  if (tx_entries > 1) {
                    if (*(txp - 1) != ',' ) {
                      break;
                    }
                    // line ends with ,
                  } else {
                    // single entry
                    break;
                  }
                }
                txp++;
                txlen++;
              }
              if (txlen) {
                script_meter_desc[index].txmem = (char*)calloc(txlen + 2, 1);
                if (script_meter_desc[index].txmem) {
                  // now copy send blocks
                  char *txp = lp;
                  uint16_t tind = 0;
                  for (uint32_t cnt = 0; cnt < txlen; cnt++) {
                      if (*txp == SCRIPT_EOL) {
                        txp++;
                      } else {
                        script_meter_desc[index].txmem[tind] = *txp++;
                        tind++;
                      }
                  }
                }
                //AddLog(LOG_LEVEL_INFO, PSTR(">>> %s - %d"), script_meter_desc[index].txmem, txlen);
                script_meter_desc[index].index = 0;
                script_meter_desc[index].max_index = tx_entries;
                sml_send_blocks++;
                lp += txlen;
              }
#else
              char txbuff[SML_SRCBSIZE];
              uint32_t txlen = 0, tx_entries = 1;
              for (uint32_t cnt = 0; cnt < sizeof(txbuff); cnt++) {
                if (*lp == SCRIPT_EOL && *(lp - 1) != ',' ) {
                  txbuff[cnt] = 0;
                  txlen = cnt;
                  break;
                }
                if (*lp == ',') tx_entries++;
                txbuff[cnt] = *lp++;
              }
              if (txlen) {
                script_meter_desc[index].txmem = (char*)calloc(txlen + 2, 1);
                if (script_meter_desc[index].txmem) {
                  strcpy(script_meter_desc[index].txmem, txbuff);
                }
                script_meter_desc[index].index = 0;
                script_meter_desc[index].max_index = tx_entries;
                sml_send_blocks++;
              }
#endif
            }
          }
          if (*lp == SCRIPT_EOL) lp--;
          goto next_line;
        }
#ifdef SML_REPLACE_VARS
        char dstbuf[SML_SRCBSIZE*2];
        Replace_Cmd_Vars(lp, 1, dstbuf,sizeof(dstbuf));
        lp += SML_getlinelen(lp);
        //AddLog(LOG_LEVEL_INFO, PSTR("%s"),dstbuf);
        char *lp1 = dstbuf;
        if (*lp1 == '-' || isdigit(*lp1)) {
          //toLogEOL(">>",lp);
          // add meters line -1,1-0:1.8.0*255(@10000,H2OIN,cbm,COUNTER,4|
          if (*lp1 == '-') lp1++;
          uint8_t mnum = strtol(lp1, 0, 10);
          if (mnum < 1 || mnum > meters_used) {
            AddLog(LOG_LEVEL_INFO, PSTR("illegal meter number!"));
            goto next_line;
          }
          // 1,=h—————————————
          if (strncmp(lp1 + 1, ",=h", 3)) {
            dec_line++;
            if (dec_line >= SML_MAX_VARS) {
              AddLog(LOG_LEVEL_INFO, PSTR("too many decode lines: %d !"), dec_line);
              goto next_line;
            }
          }
          while (1) {
            if (*lp1 == 0) {
              *tp++ = '|';
              goto next_line;
            }
            *tp++ = *lp1++;
            index++;
            if (index >= METER_DEF_SIZE) break;
          }
        }
#else

        if (*lp == '-' || isdigit(*lp)) {
          //toLogEOL(">>",lp);
          // add meters line -1,1-0:1.8.0*255(@10000,H2OIN,cbm,COUNTER,4|
          if (*lp == '-') lp++;
          uint8_t mnum = strtol(lp,0,10);
          if (mnum < 1 || mnum > meters_used) {
            AddLog(LOG_LEVEL_INFO, PSTR("illegal meter number!"));
            goto next_line;
          }
          if (strncmp(lp + 1, ",=h", 3)) {
            dec_line++;
            if (dec_line >= SML_MAX_VARS) {
              AddLog(LOG_LEVEL_INFO, PSTR("too many decode lines: %d !"), dec_line);
              goto next_line;
            }
          }

          while (1) {
            if (*lp == SCRIPT_EOL) {
              if (*(tp-1) != '|') *tp++ = '|';
              goto next_line;
            }
            *tp++ = *lp++;
            index++;
            if (index >= METER_DEF_SIZE) break;
          }

        }
#endif

      }

next_line:
      if (*lp==SCRIPT_EOL) {
        lp++;
      } else {
        lp = strchr(lp, SCRIPT_EOL);
        if (!lp) break;
        lp++;
      }
    }
    *tp=0;
    meter_desc_p=script_meter_desc;
    meter_p=script_meter;
  }
  }
#endif

init10:
  typedef void (*function)();
  uint8_t cindex=0;
  // preloud counters
  for (byte i = 0; i < MAX_COUNTERS; i++) {
      RtcSettings.pulse_counter[i] = Settings->pulse_counter[i];
      sml_counters[i].sml_cnt_last_ts = millis();
  }
  uint32_t uart_index = 2;
  sml_counter_pinstate = 0;
  for (uint8_t meters = 0; meters < meters_used; meters++) {
    if (meter_desc_p[meters].type == 'c') {
        if (meter_desc_p[meters].flag & 2) {
          // analog mode
#ifdef ANALOG_OPTO_SENSOR
          ADS1115_init();
          sml_counters[cindex].ana_max=-32768;
          sml_counters[cindex].ana_min=+32767;
#endif
        } else {
          // counters, set to input with pullup
          if (meter_desc_p[meters].flag&1) {
            pinMode(meter_desc_p[meters].srcpin,INPUT_PULLUP);
          } else {
            pinMode(meter_desc_p[meters].srcpin,INPUT);
          }
          // check for irq mode
          if (meter_desc_p[meters].params<=0) {
            // init irq mode
            sml_counters[cindex].sml_cnt_old_state = meters;
            sml_counters[cindex].sml_debounce = -meter_desc_p[meters].params;
            attachInterruptArg(meter_desc_p[meters].srcpin, SML_CounterIsr, &sml_cnt_index[cindex], CHANGE);
            if (digitalRead(meter_desc_p[meters].srcpin) > 0) {
              sml_counter_pinstate |= (1 << cindex);
            }
            sml_counters[cindex].sml_counter_ltime = millis();
          }

          RtcSettings.pulse_counter[cindex] = Settings->pulse_counter[cindex];
          InjektCounterValue(meters, RtcSettings.pulse_counter[cindex]);
          cindex++;
        }
    } else {
      // serial input, init
#ifdef SPECIAL_SS
        if (meter_desc_p[meters].type=='m' || meter_desc_p[meters].type=='M' || meter_desc_p[meters].type=='p' || meter_desc_p[meters].type=='R' || meter_desc_p[meters].type=='v') {
          meter_ss[meters] = new TasmotaSerial(meter_desc_p[meters].srcpin,meter_desc_p[meters].trxpin,1,0,TMSBSIZ);
        } else {
          meter_ss[meters] = new TasmotaSerial(meter_desc_p[meters].srcpin,meter_desc_p[meters].trxpin,1,1,TMSBSIZ);
        }
#else
#ifdef ESP8266
        meter_ss[meters] = new TasmotaSerial(meter_desc_p[meters].srcpin,meter_desc_p[meters].trxpin,1,0,TMSBSIZ);
#endif  // ESP8266
#ifdef ESP32
        // use hardware serial
#ifdef USE_ESP32_SW_SERIAL
        meter_ss[meters] = new SML_ESP32_SERIAL(uart_index);
        if (meter_desc_p[meters].srcpin >= 0) {
          if (uart_index == 0) { ClaimSerial(); }
          uart_index--;
          if (uart_index < 0) uart_index = 0;
        }
#else
        meter_ss[meters] = new HardwareSerial(uart_index);
        if (uart_index == 0) { ClaimSerial(); }
        uart_index--;
        if (uart_index < 0) uart_index = 0;
        meter_ss[meters]->setRxBufferSize(TMSBSIZ);
#endif // USE_ESP32_SW_SERIAL

#endif  // ESP32
#endif // SPECIAL_SS

        SerialConfig smode = SERIAL_8N1;

        if (meter_desc_p[meters].sopt & 0xf0) {
          // new serial config
          switch (meter_desc_p[meters].sopt >> 4) {
            case 1:
              if ((meter_desc_p[meters].sopt & 1) == 1) smode = SERIAL_8N1;
              else smode = SERIAL_8N2;
              break;
            case 2:
              if ((meter_desc_p[meters].sopt & 1) == 1) smode = SERIAL_8E1;
              else smode = SERIAL_8E2;
              break;
            case 3:
              if ((meter_desc_p[meters].sopt & 1) == 1) smode = SERIAL_8O1;
              else smode = SERIAL_8O2;
              break;
          }
        } else {
          // depecated serial config
          if (meter_desc_p[meters].sopt == 2) {
            smode = SERIAL_8N2;
          }
          if (meter_desc_p[meters].type=='M') {
            smode = SERIAL_8E1;
            if (meter_desc_p[meters].sopt == 2) {
              smode = SERIAL_8E2;
            }
          }
        }

#ifdef ESP8266
        if (meter_ss[meters]->begin(meter_desc_p[meters].params)) {
          meter_ss[meters]->flush();
        }
        if (meter_ss[meters]->hardwareSerial()) {
          Serial.begin(meter_desc_p[meters].params, smode);
          ClaimSerial();
          //Serial.setRxBufferSize(512);
        }
#endif  // ESP8266
#ifdef ESP32
        meter_ss[meters]->begin(meter_desc_p[meters].params, smode, meter_desc_p[meters].srcpin, meter_desc_p[meters].trxpin);
        //meter_ss[meters]->setRxBufferSize(TMSBSIZ);
#endif  // ESP32
    }
  }
}


#ifdef USE_SML_SCRIPT_CMD
uint32_t sml_getv(uint32_t sel) {
  if (!sel) {
    for (uint8_t cnt = 0; cnt < SML_MAX_VARS; cnt++) {
      dvalid[cnt] = 0;
    }
    sel = 0;
  } else {
    if (sel < 1) sel = 1;
    sel = dvalid[sel - 1];
  }
  return sel;
}
uint32_t SML_SetBaud(uint32_t meter, uint32_t br) {
  if (meter < 1 || meter > meters_used) return 0;
  meter--;
  if (!meter_ss[meter]) return 0;
#ifdef ESP8266
  if (meter_ss[meter]->begin(br)) {
    meter_ss[meter]->flush();
  }
  if (meter_ss[meter]->hardwareSerial()) {
    if (meter_desc_p[meter].type=='M') {
      Serial.begin(br, SERIAL_8E1);
    }
  }
#endif  // ESP8266
#ifdef ESP32
  meter_ss[meter]->flush();
  meter_ss[meter]->updateBaudRate(br);
  /*
  if (meter_desc_p[meter].type=='M') {
    meter_ss[meter]->begin(br,SERIAL_8E1,meter_desc_p[meter].srcpin,meter_desc_p[meter].trxpin);
  } else {
    meter_ss[meter]->begin(br,SERIAL_8N1,meter_desc_p[meter].srcpin,meter_desc_p[meter].trxpin);
  }*/
#endif  // ESP32
  return 1;
}

uint32_t SML_Status(uint32_t meter) {
  if (meter<1 || meter>meters_used) return 0;
  meter--;
#ifdef ED300L
  return sml_status[meter];
#else
  return 0;
#endif
}


uint32_t SML_Write(uint32_t meter,char *hstr) {
  if (meter<1 || meter>meters_used) return 0;
  meter--;
  if (!meter_ss[meter]) return 0;
  SML_Send_Seq(meter,hstr);
  return 1;
}

uint32_t SML_Read(int32_t meter,char *str, uint32_t slen) {
uint8_t hflg=0;
  if (meter<0) {
    meter=abs(meter);
    hflg=1;
  }
  if (meter<1 || meter>meters_used) return 0;
  meter--;
  if (!meter_ss[meter]) return 0;

  if (!meter_spos[meter]) {
    return 0;
  }

  smltbuf[meter][meter_spos[meter]]=0;

  if (!hflg) {
    strlcpy(str,(char*)&smltbuf[meter][0],slen);
  } else {
    uint32_t index=0;
    for (uint32_t cnt=0; cnt<meter_spos[meter]; cnt++) {
      sprintf(str,"%02x",smltbuf[meter][cnt]);
      str+=2;
      index+=2;
      if (index>=slen-2) break;
    }
  }
  meter_spos[meter]=0;
  return 1;
}

float SML_GetVal(uint32_t index) {
  if (index < 1 || index > SML_MAX_VARS) { index = 1;}
  return meter_vars[index-1];
}

char *SML_GetSVal(uint32_t index) {
  if (index < 1 || index > MAX_METERS) { index = 1;}
  return &meter_id[index - 1][0];
}

int32_t SML_Set_WStr(uint32_t meter, char *hstr) {
  if (meter < 1 || meter > meters_used) return -1;
  meter--;
  if (!meter_ss[meter]) return -2;
  script_meter_desc[meter].script_str = hstr;
  return 0;
}

#endif // USE_SML_SCRIPT_CMD


void SetDBGLed(uint8_t srcpin, uint8_t ledpin) {
    pinMode(ledpin, OUTPUT);
    if (digitalRead(srcpin)) {
      digitalWrite(ledpin,LOW);
    } else {
      digitalWrite(ledpin,HIGH);
    }
}

// fast counter polling
void SML_Counter_Poll(void) {
uint16_t meters,cindex=0;
uint32_t ctime=millis();

  for (meters=0; meters<meters_used; meters++) {
    if (meter_desc_p[meters].type=='c') {
      // poll for counters and debouce
      if (meter_desc_p[meters].params>0) {
        if (ctime-sml_counters[cindex].sml_cnt_last_ts>meter_desc_p[meters].params) {
          sml_counters[cindex].sml_cnt_last_ts=ctime;

          if (meter_desc_p[meters].flag&2) {
            // analog mode, get next value
#ifdef ANALOG_OPTO_SENSOR
            if (ads1115_up) {
              int16_t val = adc.read_sample();
              if (val>sml_counters[cindex].ana_max) sml_counters[cindex].ana_max=val;
              if (val<sml_counters[cindex].ana_min) sml_counters[cindex].ana_min=val;
              sml_counters[cindex].ana_curr=val;
              int16_t range=sml_counters[cindex].ana_max-sml_counters[cindex].ana_min;
            }
#endif
          } else {
            // poll digital input
            uint8_t state;
            sml_counters[cindex].sml_cnt_debounce<<=1;
            sml_counters[cindex].sml_cnt_debounce|=(digitalRead(meter_desc_p[meters].srcpin)&1)|0x80;
            if (sml_counters[cindex].sml_cnt_debounce==0xc0) {
              // is 1
              state=1;
            } else {
              // is 0, means switch down
              state=0;
            }
            if (sml_counters[cindex].sml_cnt_old_state!=state) {
              // state has changed
              sml_counters[cindex].sml_cnt_old_state=state;
              if (state==0) {
                // inc counter
                RtcSettings.pulse_counter[cindex]++;
                InjektCounterValue(meters,RtcSettings.pulse_counter[cindex]);
              }
            }
          }
        }
#ifdef DEBUG_CNT_LED1
        if (cindex==0) SetDBGLed(meter_desc_p[meters].srcpin,DEBUG_CNT_LED1);
#endif
#ifdef DEBUG_CNT_LED2
        if (cindex==1) SetDBGLed(meter_desc_p[meters].srcpin,DEBUG_CNT_LED2);
#endif
      } else {
        if (ctime-sml_counters[cindex].sml_cnt_last_ts>10) {
          sml_counters[cindex].sml_cnt_last_ts=ctime;
#ifdef DEBUG_CNT_LED1
          if (cindex==0) SetDBGLed(meter_desc_p[meters].srcpin,DEBUG_CNT_LED1);
#endif
#ifdef DEBUG_CNT_LED2
          if (cindex==1) SetDBGLed(meter_desc_p[meters].srcpin,DEBUG_CNT_LED2);
#endif
        }

        if (sml_counters[cindex].sml_cnt_updated) {
          InjektCounterValue(sml_counters[cindex].sml_cnt_old_state,RtcSettings.pulse_counter[cindex]);
          sml_counters[cindex].sml_cnt_updated=0;
        }


      }
      cindex++;
    }
  }
}

#ifdef USE_SCRIPT
char *SML_Get_Sequence(char *cp,uint32_t index) {
  if (!index) return cp;
  uint32_t cindex=0;
  while (cp) {
    cp=strchr(cp,',');
    if (cp) {
      cp++;
      cindex++;
      if (cindex==index) {
        return cp;
      }
    }
  }
  return cp;
}

void SML_Check_Send(void) {
  sml_100ms_cnt++;
  char *cp;
  for (uint32_t cnt = sml_desc_cnt; cnt < meters_used; cnt++) {
    if (script_meter_desc[cnt].trxpin >= 0 && script_meter_desc[cnt].txmem) {
      //AddLog(LOG_LEVEL_INFO, PSTR("100 ms>> %d - %s - %d"),sml_desc_cnt,script_meter_desc[cnt].txmem,script_meter_desc[cnt].tsecs);
      if ((sml_100ms_cnt >= script_meter_desc[cnt].tsecs)) {
        sml_100ms_cnt = 0;
        // check for scriptsync extra output
        if (script_meter_desc[cnt].script_str) {
          cp = script_meter_desc[cnt].script_str;
          script_meter_desc[cnt].script_str = 0;
        } else {
          //AddLog(LOG_LEVEL_INFO, PSTR("100 ms>> 2"),cp);
          if (script_meter_desc[cnt].max_index>1) {
            script_meter_desc[cnt].index++;
            if (script_meter_desc[cnt].index >= script_meter_desc[cnt].max_index) {
              script_meter_desc[cnt].index = 0;
              sml_desc_cnt++;
            }
            cp=SML_Get_Sequence(script_meter_desc[cnt].txmem,script_meter_desc[cnt].index);
            //SML_Send_Seq(cnt,cp);
          } else {
            cp = script_meter_desc[cnt].txmem;
            //SML_Send_Seq(cnt,cp);
            sml_desc_cnt++;
          }
        }
        //AddLog(LOG_LEVEL_INFO, PSTR(">> %s"),cp);
        SML_Send_Seq(cnt,cp);
        if (sml_desc_cnt >= meters_used) {
          sml_desc_cnt = 0;
        }
        break;
      }
    } else {
      sml_desc_cnt++;
    }

    if (sml_desc_cnt >= meters_used) {
      sml_desc_cnt = 0;
    }
  }
}

uint8_t sml_hexnibble(char chr) {
  uint8_t rVal = 0;
  if (isdigit(chr)) {
    rVal = chr - '0';
  } else  {
    if (chr >= 'A' && chr <= 'F') rVal = chr + 10 - 'A';
    if (chr >= 'a' && chr <= 'f') rVal = chr + 10 - 'a';
  }
  return rVal;
}

// send sequence every N Seconds
void SML_Send_Seq(uint32_t meter,char *seq) {
  uint8_t sbuff[48];
  uint8_t *ucp = sbuff, slen = 0;
  char *cp = seq;
  uint8_t rflg = 0;
  if (*cp == 'r') {
    rflg = 1;
    cp++;
  }
  while (*cp) {
    if (!*cp || !*(cp+1)) break;
    if (*cp == ',') break;
    uint8_t iob = (sml_hexnibble(*cp) << 4) | sml_hexnibble(*(cp + 1));
    cp += 2;
    *ucp++ = iob;
    slen++;
    if (slen >= sizeof(sbuff)-6) break; // leave space for checksum
  }
  if (script_meter_desc[meter].type == 'm' || script_meter_desc[meter].type == 'M') {
    if (!rflg) {
      *ucp++ = 0;
      *ucp++ = 2;
      slen += 2;
    }
    // append crc
    uint16_t crc = MBUS_calculateCRC(sbuff, slen);
    *ucp++ = lowByte(crc);
    *ucp++ = highByte(crc);
    slen += 2;
  }
  if (script_meter_desc[meter].type == 'o') {
    for (uint32_t cnt = 0; cnt < slen; cnt++) {
      sbuff[cnt] |= (CalcEvenParity(sbuff[cnt]) << 7);
    }
  }
  if (script_meter_desc[meter].type == 'p') {
    *ucp++ = 0xc0;
    *ucp++ = 0xa8;
    *ucp++ = 1;
    *ucp++ = 1;
    *ucp++ = 0;
    *ucp++ = SML_PzemCrc(sbuff, 6);
    slen += 6;
  }
  meter_ss[meter]->write(sbuff, slen);
  if (dump2log) {
#ifdef SML_DUMP_OUT_ALL
    Hexdump(sbuff, slen);
#else
    uint8_t type = meter_desc_p[(dump2log&7) - 1].type;
    if (type == 'm' || type == 'M') {
      Hexdump(sbuff, slen);
    }
#endif
  }

}
#endif // USE_SCRIPT

uint16_t MBUS_calculateCRC(uint8_t *frame, uint8_t num) {
  uint16_t crc, flag;
  crc = 0xFFFF;
  for (uint32_t i = 0; i < num; i++) {
    crc ^= frame[i];
    for (uint32_t j = 8; j; j--) {
      if ((crc & 0x0001) != 0) {        // If the LSB is set
        crc >>= 1;                      // Shift right and XOR 0xA001
        crc ^= 0xA001;
      } else {                          // Else LSB is not set
        crc >>= 1;                      // Just shift right
      }
    }
  }
  return crc;
}

uint8_t SML_PzemCrc(uint8_t *data, uint8_t len) {
  uint16_t crc = 0;
  for (uint32_t i = 0; i < len; i++) crc += *data++;
  return (uint8_t)(crc & 0xFF);
}

// for odd parity init with 1
uint8_t CalcEvenParity(uint8_t data) {
uint8_t parity=0;

  while(data) {
    parity^=(data &1);
    data>>=1;
  }
  return parity;
}



// dump to log shows serial data on console
// has to be off for normal use
// in console sensor53 d1, d2, d3 ... or d0 for normal use
// set counter => sensor53 c1 xxxx
// restart driver => sensor53 r
// meter number for monitoring serial activity => sensor53 m1, m2, m3 ... or m0 for all (default)
// LED-GPIO for monitoring serial activity => sensor53 l2, l13, l15 ... or l255 for turn off (default)

bool XSNS_53_cmd(void) {
  bool serviced = true;
  if (XdrvMailbox.data_len > 0) {
      char *cp=XdrvMailbox.data;
      if (*cp=='d') {
        // set dump mode
        cp++;
        uint8_t index=atoi(cp);
        if ((index&7)>meters_used) index=1;
        if (index>0 && meter_desc_p[(index&7)-1].type=='c') {
          index=0;
        }
        dump2log=index;
        ResponseTime_P(PSTR(",\"SML\":{\"CMD\":\"dump: %d\"}}"),dump2log);
      } else if (*cp=='c') {
        // set counter
          cp++;
          uint8_t index=*cp&7;
          if (index<1 || index>MAX_COUNTERS) index=1;
          cp++;
          while (*cp==' ') cp++;
          if (isdigit(*cp)) {
            uint32_t cval=atoi(cp);
            while (isdigit(*cp)) cp++;
            RtcSettings.pulse_counter[index-1]=cval;
            uint8_t cindex=0;
            for (uint8_t meters=0; meters<meters_used; meters++) {
              if (meter_desc_p[meters].type=='c') {
                InjektCounterValue(meters,RtcSettings.pulse_counter[cindex]);
                cindex++;
              }
            }
          }
          ResponseTime_P(PSTR(",\"SML\":{\"CMD\":\"counter%d: %d\"}}"),index,RtcSettings.pulse_counter[index-1]);
      } else if (*cp=='r') {
        // restart
        ResponseTime_P(PSTR(",\"SML\":{\"CMD\":\"restart\"}}"));
        SML_CounterSaveState();
        SML_Init();
      } else if (*cp=='m') {
        // meter number for serial activity
        cp++;
        if (!isdigit(*cp)) {
          ResponseTime_P(PSTR(",\"SML\":{\"CMD\":\"ser_act_meter_num: %d\"}}"),ser_act_meter_num);
        } else {
          ser_act_meter_num=atoi(cp);
          ResponseTime_P(PSTR(",\"SML\":{\"CMD\":\"ser_act_meter_num: %d\"}}"),ser_act_meter_num);
        }
      } else if (*cp=='l') {
        // serial activity LED-GPIO
        cp++;
        if (!isdigit(*cp)) {
          ResponseTime_P(PSTR(",\"SML\":{\"CMD\":\"ser_act_LED_pin: %d\"}}"),ser_act_LED_pin);
        } else {
          ser_act_LED_pin=atoi(cp);
          if (Gpio_used(ser_act_LED_pin)) {
            AddLog(LOG_LEVEL_INFO, PSTR("SML: Error: Duplicate GPIO %d defined. Not usable for LED."),ser_act_LED_pin);
            ser_act_LED_pin=255;
          }
          if (ser_act_LED_pin!=255) {
            pinMode(ser_act_LED_pin, OUTPUT);
          }
          ResponseTime_P(PSTR(",\"SML\":{\"CMD\":\"ser_act_LED_pin: %d\"}}"),ser_act_LED_pin);
        }
      } else {
        serviced=false;
      }
  }
  return serviced;
}

void InjektCounterValue(uint8_t meter,uint32_t counter) {
  sprintf((char*)&smltbuf[meter][0],"1-0:1.8.0*255(%d)",counter);
  SML_Decode(meter);
}

void SML_CounterSaveState(void) {
  for (byte i = 0; i < MAX_COUNTERS; i++) {
      Settings->pulse_counter[i] = RtcSettings.pulse_counter[i];
  }
}




/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns53(byte function) {
  bool result = false;
    switch (function) {
      case FUNC_INIT:
        SML_Init();
        break;
      case FUNC_LOOP:
        SML_Counter_Poll();
        if (dump2log) Dump2log();
        else {
          SML_Poll();
        }
        break;
    //  case FUNC_EVERY_50_MSECOND:
    //    if (dump2log) Dump2log();
    //    else SML_Poll();
    //    break;
#ifdef USE_SCRIPT
      case FUNC_EVERY_100_MSECOND:
        if (bitRead(Settings->rule_enabled, 0)) {
          SML_Check_Send();
        }
        break;
#endif // USE_SCRIPT
      case FUNC_JSON_APPEND:
        if (sml_json_enable) {
          SML_Show(1);
        }
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        SML_Show(0);
        break;
#endif  // USE_WEBSERVER
      case FUNC_COMMAND_SENSOR:
        if (XSNS_53 == XdrvMailbox.index) {
          result = XSNS_53_cmd();
        }
        break;
      case FUNC_SAVE_BEFORE_RESTART:
      case FUNC_SAVE_AT_MIDNIGHT:
        SML_CounterSaveState();
        break;
    }
  return result;
}

#endif  // USE_SML
