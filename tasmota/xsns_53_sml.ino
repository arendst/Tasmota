/*
  xsns_53_sml.ino - SML,OBIS,EBUS,RAW,COUNTER interface for Tasmota

  Created by Gerhard Mutz on 07.10.11.
  adapted for Tasmota

  Copyright (C) 2020  Gerhard Mutz and Theo Arends

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

#undef TMSBSIZ
#define TMSBSIZ 256

// addresses a bug in meter DWS74
//#define DWS74_BUG

// JSON Strings do not translate
// max 23 char
#define DJ_TPWRIN "Total_in"
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
  uint8_t srcpin;
  uint8_t type;
  uint16_t flag;
  int32_t params;
  char prefix[8];
  int8_t trxpin;
  uint8_t tsecs;
  char *txmem;
  uint8_t index;
  uint8_t max_index;
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

// select this meter
#define METER EHZ161_1


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

struct METER_DESC const *meter_desc_p;
const uint8_t *meter_p;
uint8_t meter_spos[MAX_METERS];

// software serial pointers
#ifdef ESP32
HardwareSerial *meter_ss[MAX_METERS];
#else
TasmotaSerial *meter_ss[MAX_METERS];
#endif

// serial buffers, may be made larger depending on telegram lenght
#ifndef SML_BSIZ
#define SML_BSIZ 48
#endif
uint8_t smltbuf[MAX_METERS][SML_BSIZ];

// meter nr as string
#define METER_ID_SIZE 24
char meter_id[MAX_METERS][METER_ID_SIZE];

#define EBUS_SYNC		0xaa
#define EBUS_ESC    0xa9

uint8_t sml_send_blocks;
uint8_t sml_100ms_cnt;
uint8_t sml_desc_cnt;

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
  if (!i2c_flg) return;

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

uint8_t sml_logindex;

void Dump2log(void) {

int16_t index=0,hcnt=0;
uint32_t d_lastms;
uint8_t dchars[16];

  //if (!SML_SAVAILABLE) return;

  if (dump2log&8) {
    // combo mode
    while (SML_SAVAILABLE) {
      log_data[index]=':';
      index++;
      log_data[index]=' ';
      index++;
      d_lastms=millis();
      while ((millis()-d_lastms)<40) {
        if (SML_SAVAILABLE) {
          uint8_t c=SML_SREAD;
          sprintf(&log_data[index],"%02x ",c);
          dchars[hcnt]=c;
          index+=3;
          hcnt++;
          if (hcnt>15) {
            // line complete, build asci chars
            log_data[index]='=';
            index++;
            log_data[index]='>';
            index++;
            log_data[index]=' ';
            index++;
            for (uint8_t ccnt=0; ccnt<16; ccnt++) {
              if (isprint(dchars[ccnt])) {
                log_data[index]=dchars[ccnt];
              } else {
                log_data[index]=' ';
              }
              index++;
            }
            break;
          }
        }
      }
      if (index>0) {
        log_data[index]=0;
        AddLog(LOG_LEVEL_INFO);
        index=0;
        hcnt=0;
      }
    }
  } else {
    if (meter_desc_p[(dump2log&7)-1].type=='o') {
      // obis
      while (SML_SAVAILABLE) {
        char c=SML_SREAD&0x7f;
        if (c=='\n' || c=='\r') {
          log_data[sml_logindex]=0;
          AddLog(LOG_LEVEL_INFO);
          sml_logindex=2;
          log_data[0]=':';
          log_data[1]=' ';
          break;
        }
        log_data[sml_logindex]=c;
        if (sml_logindex<sizeof(log_data)-2) {
          sml_logindex++;
        }
      }
    } else {
      //while (SML_SAVAILABLE) {
      index=0;
      log_data[index]=':';
      index++;
      log_data[index]=' ';
      index++;
      d_lastms=millis();
      while ((millis()-d_lastms)<40) {
        if (SML_SAVAILABLE) {
          unsigned char c;
          if (meter_desc_p[(dump2log&7)-1].type=='e') {
            // ebus
            c=SML_SREAD;
            sprintf(&log_data[index],"%02x ",c);
            index+=3;
            if (c==EBUS_SYNC) break;
          } else {
            // sml
            if (sml_start==0x77) {
              sml_start=0;
            } else {
              c=SML_SPEAK;
              if (c==0x77) {
                sml_start=c;
                break;
              }
            }
            c=SML_SREAD;
            sprintf(&log_data[index],"%02x ",c);
            index+=3;
          }
        }
      }
      if (index>2) {
        log_data[index]=0;
        AddLog(LOG_LEVEL_INFO);
      }
    }
  }
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
  while ((*pt != '\0') && isblank(*pt)) { pt++; }  // Trim leading spaces

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
  if (meter_desc_p[meters].type!='e' && meter_desc_p[meters].type!='m' && meter_desc_p[meters].type!='M' && meter_desc_p[meters].type!='p' && meter_desc_p[meters].type!='R') {
    // shift in
    for (count=0; count<SML_BSIZ-1; count++) {
      smltbuf[meters][count]=smltbuf[meters][count+1];
    }
  }
  uint8_t iob=(uint8_t)meter_ss[meters]->read();

  if (meter_desc_p[meters].type=='o') {
    smltbuf[meters][SML_BSIZ-1]=iob&0x7f;
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
  }
  else {
    if (iob==EBUS_SYNC) {
    	// should be end of telegramm
      // QQ,ZZ,PB,SB,NN ..... CRC, ACK SYNC
      if (meter_spos[meters]>4+5) {
      	// get telegramm lenght
        uint8_t tlen=smltbuf[meters][4]+5;
        // test crc
        if (smltbuf[meters][tlen]=ebus_CalculateCRC(smltbuf[meters],tlen)) {
            ebus_esc(smltbuf[meters],tlen);
            SML_Decode(meters);
        } else {
            // crc error
            //AddLog_P(LOG_LEVEL_INFO, PSTR("ebus crc error"));
        }
      }
      meter_spos[meters]=0;
      return;
    }
		smltbuf[meters][meter_spos[meters]] = iob;
		meter_spos[meters]++;
		if (meter_spos[meters]>=SML_BSIZ) {
			meter_spos[meters]=0;
		}
  }
  sb_counter++;
  if (meter_desc_p[meters].type!='e' && meter_desc_p[meters].type!='m' && meter_desc_p[meters].type!='M' && meter_desc_p[meters].type!='p' && meter_desc_p[meters].type!='R') SML_Decode(meters);
}


// polled every 50 ms
void SML_Poll(void) {
uint32_t meters;

    for (meters=0; meters<meters_used; meters++) {
      if (meter_desc_p[meters].type!='c') {
        // poll for serial input
        if (!meter_ss[meters]) continue;
        while (meter_ss[meters]->available()) {
          sml_shift_in(meters,0);
        }
      }
    }
}


void SML_Decode(uint8_t index) {
  const char *mp=(const char*)meter_p;
  int8_t mindex;
  uint8_t *cp;
  uint8_t dindex=0,vindex=0;
  delay(0);
  while (mp != NULL) {
    // check list of defines
    if (*mp==0) break;

    // new section
    mindex=((*mp)&7)-1;

    if (mindex<0 || mindex>=meters_used) mindex=0;
    mp+=2;
    if (*mp=='=' && *(mp+1)=='h') {
      mp = strchr(mp, '|');
      if (mp) mp++;
      continue;
    }

    if (index!=mindex) goto nextsect;

    // start of serial source buffer
    cp=&smltbuf[mindex][0];

    // compare
    if (*mp=='=') {
      // calculated entry, check syntax
      mp++;
      // do math m 1+2+3
      if (*mp=='m' && !sb_counter) {
        // only every 256 th byte
        // else it would be calculated every single serial byte
        mp++;
        while (*mp==' ') mp++;
        // 1. index
        double dvar;
        uint8_t opr;
        uint32_t ind;
        ind=atoi(mp);
        while (*mp>='0' && *mp<='9') mp++;
        if (ind<1 || ind>SML_MAX_VARS) ind=1;
        dvar=meter_vars[ind-1];
        for (uint8_t p=0;p<5;p++) {
          if (*mp=='@') {
            // store result
            meter_vars[vindex]=dvar;
            mp++;
            SML_Immediate_MQTT((const char*)mp,vindex,mindex);
            break;
          }
          opr=*mp;
          mp++;
          uint8_t iflg=0;
          if (*mp=='#') {
            iflg=1;
            mp++;
          }
          ind=atoi(mp);
          while (*mp>='0' && *mp<='9') mp++;
          if (ind<1 || ind>SML_MAX_VARS) ind=1;
          switch (opr) {
              case '+':
                if (iflg) dvar+=ind;
                else dvar+=meter_vars[ind-1];
                break;
              case '-':
                if (iflg) dvar-=ind;
                else dvar-=meter_vars[ind-1];
                break;
              case '*':
                if (iflg) dvar*=ind;
                else dvar*=meter_vars[ind-1];
                break;
              case '/':
                if (iflg) dvar/=ind;
                else dvar/=meter_vars[ind-1];
                break;
          }
          while (*mp==' ') mp++;
          if (*mp=='@') {
            // store result
            meter_vars[vindex]=dvar;
            mp++;
            SML_Immediate_MQTT((const char*)mp,vindex,mindex);
            break;
          }
        }
      } else if (*mp=='d') {
        // calc deltas d ind 10 (eg every 10 secs)
        if (dindex<MAX_DVARS) {
          // only n indexes
          mp++;
          while (*mp==' ') mp++;
          uint8_t ind=atoi(mp);
          while (*mp>='0' && *mp<='9') mp++;
          if (ind<1 || ind>SML_MAX_VARS) ind=1;
          uint32_t delay=atoi(mp)*1000;
          uint32_t dtime=millis()-dtimes[dindex];
          if (dtime>delay) {
            // calc difference
            dtimes[dindex]=millis();
            double vdiff = meter_vars[ind-1]-dvalues[dindex];
            dvalues[dindex]=meter_vars[ind-1];
            meter_vars[vindex]=(double)360000.0*vdiff/((double)dtime/10000.0);

            mp=strchr(mp,'@');
            if (mp) {
              mp++;
              SML_Immediate_MQTT((const char*)mp,vindex,mindex);
            }
          }
          dindex++;
        }
      } else if (*mp=='h') {
        // skip html tag line
        mp = strchr(mp, '|');
        if (mp) mp++;
        continue;
      }
    } else {
      // compare value
      uint8_t found=1;
      uint32_t ebus_dval=99;
      float mbus_dval=99;
      while (*mp!='@') {
        if (meter_desc_p[mindex].type=='o' || meter_desc_p[mindex].type=='c') {
          if (*mp++!=*cp++) {
            found=0;
          }
        } else {
          if (meter_desc_p[mindex].type=='s') {
            // sml
            uint8_t val = hexnibble(*mp++) << 4;
            val |= hexnibble(*mp++);
            if (val!=*cp++) {
              found=0;
            }
          } else {
            // ebus mbus pzem or raw
            // XXHHHHSSUU
            if (*mp=='x' && *(mp+1)=='x') {
              //ignore
              mp+=2;
              cp++;
            } else if (!strncmp(mp,"UUuuUUuu",8)) {
              uint32_t val= (cp[0]<<24)|(cp[1]<<16)|(cp[2]<<8)|(cp[3]<<0);
              ebus_dval=val;
              mbus_dval=val;
              mp+=8;
              cp+=4;
            } else if (*mp=='U' && *(mp+1)=='U' && *(mp+2)=='u' && *(mp+3)=='u'){
              uint16_t val = cp[1]|(cp[0]<<8);
              mbus_dval=val;
              ebus_dval=val;
              mp+=4;
              cp+=2;
            } else if (!strncmp(mp,"SSssSSss",8)) {
              int32_t val= (cp[0]<<24)|(cp[1]<<16)|(cp[2]<<8)|(cp[3]<<0);
              ebus_dval=val;
              mbus_dval=val;
              mp+=8;
              cp+=4;
            } else if (*mp=='u' && *(mp+1)=='u' && *(mp+2)=='U' && *(mp+3)=='U'){
              uint16_t val = cp[0]|(cp[1]<<8);
              mbus_dval=val;
              ebus_dval=val;
              mp+=4;
              cp+=2;
            } else if (*mp=='u' && *(mp+1)=='u') {
              uint8_t val = *cp++;
              mbus_dval=val;
              ebus_dval=val;
              mp+=2;
            } else if (*mp=='s' && *(mp+1)=='s' && *(mp+2)=='S' && *(mp+3)=='S') {
              int16_t val = *cp|(*(cp+1)<<8);
              mbus_dval=val;
              ebus_dval=val;
              mp+=4;
              cp+=2;
            } else if (*mp=='S' && *(mp+1)=='S' && *(mp+2)=='s' && *(mp+3)=='s') {
              int16_t val = cp[1]|(cp[0]<<8);
              mbus_dval=val;
              ebus_dval=val;
              mp+=4;
              cp+=2;
            }
            else if (*mp=='s' && *(mp+1)=='s') {
              int8_t val = *cp++;
              mbus_dval=val;
              ebus_dval=val;
              mp+=2;
            }
            else if (!strncmp(mp,"ffffffff",8)) {
              uint32_t val= (cp[0]<<24)|(cp[1]<<16)|(cp[2]<<8)|(cp[3]<<0);
              float *fp=(float*)&val;
              ebus_dval=*fp;
              mbus_dval=*fp;
              mp+=8;
              cp+=4;
            }
            else if (!strncmp(mp,"FFffFFff",8)) {
              // reverse word float
              uint32_t val= (cp[1]<<0)|(cp[0]<<8)|(cp[3]<<16)|(cp[2]<<24);
              float *fp=(float*)&val;
              ebus_dval=*fp;
              mbus_dval=*fp;
              mp+=8;
              cp+=4;
            }
            else if (!strncmp(mp,"eeeeee",6)) {
              uint32_t val=(cp[0]<<16)|(cp[1]<<8)|(cp[2]<<0);
              mbus_dval=val;
              mp+=6;
              cp+=3;
            }
            else if (!strncmp(mp,"vvvvvv",6)) {
              mbus_dval=(float)((cp[0]<<8)|(cp[1])) + ((float)cp[2]/10.0);
              mp+=6;
              cp+=3;
            }
            else if (!strncmp(mp,"cccccc",6)) {
              mbus_dval=(float)((cp[0]<<8)|(cp[1])) + ((float)cp[2]/100.0);
              mp+=6;
              cp+=3;
            }
            else if (!strncmp(mp,"pppp",4)) {
              mbus_dval=(float)((cp[0]<<8)|cp[1]);
              mp+=4;
              cp+=2;
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
        mp++;
#ifdef ED300L
        g_mindex=mindex;
#endif
        if (*mp=='#') {
          // get string value
          mp++;
          if (meter_desc_p[mindex].type=='o') {
            for (uint8_t p=0;p<METER_ID_SIZE;p++) {
              if (*cp==*mp) {
                meter_id[mindex][p]=0;
                break;
              }
              meter_id[mindex][p]=*cp++;
            }
          } else {
            sml_getvalue(cp,mindex);
          }
        } else {
          double dval;
          if (meter_desc_p[mindex].type!='e' && meter_desc_p[mindex].type!='r' && meter_desc_p[mindex].type!='m' && meter_desc_p[mindex].type!='M' && meter_desc_p[mindex].type!='p') {
            // get numeric values
            if (meter_desc_p[mindex].type=='o' || meter_desc_p[mindex].type=='c') {
              dval=CharToDouble((char*)cp);
            } else {
              dval=sml_getvalue(cp,mindex);
            }
          } else {
            // ebus pzem or mbus or raw
            if (*mp=='b') {
              mp++;
              uint8_t shift=*mp&7;
              ebus_dval>>=shift;
              ebus_dval&=1;
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
              //AddLog_P2(LOG_LEVEL_INFO, PSTR(">> %s"),mp);
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
//AddLog_P2(LOG_LEVEL_INFO, PSTR(">> %s"),mp);
          // get scaling factor
          double fac=CharToDouble((char*)mp);
          meter_vars[vindex]/=fac;
          SML_Immediate_MQTT((const char*)mp,vindex,mindex);
        }
      }
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
  int8_t count,mindex,cindex=0;
  char tpowstr[32];
  char name[24];
  char unit[8];
  char jname[24];
  int8_t index=0,mid=0;
  char *mp=(char*)meter_p;
  char *cp,nojson=0;
  //char b_mqtt_data[MESSZ];
  //b_mqtt_data[0]=0;


    int8_t lastmind=((*mp)&7)-1;
    if (lastmind<0 || lastmind>=meters_used) lastmind=0;
    while (mp != NULL) {
        if (*mp==0) break;
        // setup sections
        mindex=((*mp)&7)-1;

        if (mindex<0 || mindex>=meters_used) mindex=0;
        if (meter_desc_p[mindex].prefix[0]=='*' && meter_desc_p[mindex].prefix[1]==0) {
          nojson=1;
        } else {
          nojson=0;
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
          if (*cp=='#') {
            // meter id
            sprintf(tpowstr,"\"%s\"",&meter_id[mindex][0]);
            mid=1;
          } else {
            mid=0;
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
              // json export
              if (index==0) {
                  //snprintf_P(b_mqtt_data, sizeof(b_mqtt_data), "%s,\"%s\":{\"%s\":%s", b_mqtt_data,meter_desc_p[mindex].prefix,jname,tpowstr);
                  if (!nojson) ResponseAppend_P(PSTR(",\"%s\":{\"%s\":%s"),meter_desc_p[mindex].prefix,jname,tpowstr);
              }
              else {
                if (lastmind!=mindex) {
                  // meter changed, close mqtt
                  //snprintf_P(b_mqtt_data, sizeof(b_mqtt_data), "%s}", b_mqtt_data);
                  if (!nojson) ResponseAppend_P(PSTR("}"));
                    // and open new
                    //snprintf_P(b_mqtt_data, sizeof(b_mqtt_data), "%s,\"%s\":{\"%s\":%s", b_mqtt_data,meter_desc_p[mindex].prefix,jname,tpowstr);
                  if (!nojson) ResponseAppend_P(PSTR(",\"%s\":{\"%s\":%s"),meter_desc_p[mindex].prefix,jname,tpowstr);
                  lastmind=mindex;
                } else {
                  //snprintf_P(b_mqtt_data, sizeof(b_mqtt_data), "%s,\"%s\":%s", b_mqtt_data,jname,tpowstr);
                  if (!nojson) ResponseAppend_P(PSTR(",\"%s\":%s"),jname,tpowstr);
                }
              }

            } else {
              // web ui export
              //snprintf_P(b_mqtt_data, sizeof(b_mqtt_data), "%s{s}%s %s: {m}%s %s{e}", b_mqtt_data,meter_desc[mindex].prefix,name,tpowstr,unit);
             if (strcmp(name,"*"))  WSContentSend_PD(PSTR("{s}%s %s: {m}%s %s{e}"),meter_desc_p[mindex].prefix,name,tpowstr,unit);
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
     if (!nojson) ResponseAppend_P(PSTR("}"));
   } else {
     //WSContentSend_PD(PSTR("%s"),b_mqtt_data);
   }


/*
#ifdef USE_DOMOTICZ
  if (json && !tele_period) {
    char str[16];
    dtostrfd(meter_vars[0], 1, str);
    DomoticzSensorPowerEnergy(meter_vars[1], str);  // PowerUsage, EnergyToday
    dtostrfd(meter_vars[2], 1, str);
    DomoticzSensor(DZ_VOLTAGE, str);  // Voltage
    dtostrfd(meter_vars[3], 1, str);
    DomoticzSensor(DZ_CURRENT, str);  // Current
  }
#endif  // USE_DOMOTICZ
*/
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

void ICACHE_RAM_ATTR SML_CounterUpd(uint8_t index) {

  uint8_t level=digitalRead(meter_desc_p[sml_counters[index].sml_cnt_old_state].srcpin);
  if (!level) {
    // falling edge
    uint32_t ltime=millis()-sml_counters[index].sml_counter_ltime;
    sml_counters[index].sml_counter_ltime=millis();
    if (ltime>sml_counters[index].sml_debounce) {
      RtcSettings.pulse_counter[index]++;
      sml_counters[index].sml_cnt_updated=1;
      //InjektCounterValue(sml_counters[index].sml_cnt_old_state,RtcSettings.pulse_counter[index]);
    }
  } else {
    // rising edge
    sml_counters[index].sml_counter_ltime=millis();
  }
}

void ICACHE_RAM_ATTR SML_CounterUpd1(void) {
  SML_CounterUpd(0);
}

void ICACHE_RAM_ATTR SML_CounterUpd2(void) {
  SML_CounterUpd(1);
}

void ICACHE_RAM_ATTR SML_CounterUpd3(void) {
  SML_CounterUpd(2);
}

void ICACHE_RAM_ATTR SML_CounterUpd4(void) {
  SML_CounterUpd(3);
}

#ifdef USE_SCRIPT
struct METER_DESC  script_meter_desc[MAX_METERS];
uint8_t *script_meter;
#endif

#ifndef METER_DEF_SIZE
#define METER_DEF_SIZE 3000
#endif



#ifdef SML_REPLACE_VARS

#define SML_SRCBSIZE 256

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
    //AddLog_P2(LOG_LEVEL_INFO, PSTR("%d - %s"),slen,dstbuf);
    mlen+=slen+1;
    if (*lp=='#') break;
    if (*lp=='>') break;
    if (*lp==0) break;
  }
  //AddLog_P2(LOG_LEVEL_INFO, PSTR("len=%d"),mlen);
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
  //AddLog_P2(LOG_LEVEL_INFO, PSTR("len=%d"),mlen);
  return mlen;
}
#endif

bool Gpio_used(uint8_t gpiopin) {
/*
  for (uint16_t i=0;i<GPIO_SENSOR_END;i++) {
//    if (pin_gpio[i]==gpiopin) {
    if (Pin(i)==gpiopin) {
      return true;
    }
  }
*/
  if ((gpiopin < ARRAY_SIZE(gpio_pin)) && (gpio_pin[gpiopin] > 0)) {
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

  if (bitRead(Settings.rule_enabled, 0)) {

  uint8_t meter_script=Run_Scripter(">M",-2,0);
  if (meter_script==99) {
    // use script definition
    if (script_meter) free(script_meter);
    script_meter=0;
    uint8_t *tp=0;
    uint16_t index=0;
    uint8_t section=0;
    uint8_t srcpin=0;
    char *lp=glob_script_mem.scriptptr;
    sml_send_blocks=0;
    while (lp) {
      if (!section) {
        if (*lp=='>' && *(lp+1)=='M') {
          lp+=2;
          meters_used=strtol(lp,0,10);
          section=1;
          uint32_t mlen=SML_getscriptsize(lp);
          if (mlen==0) return; // missing end #
          script_meter=(uint8_t*)calloc(mlen,1);
          if (!script_meter) {
            goto dddef_exit;
          }
          tp=script_meter;
          goto next_line;
        }
      }
      else {
        if (!*lp || *lp=='#' || *lp=='>') {
          if (*(tp-1)=='|') *(tp-1)=0;
          break;
        }
        if (*lp=='+') {
          // add descriptor +1,1,c,0,10,H20
          //toLogEOL(">>",lp);
          lp++;
          index=*lp&7;
          lp+=2;
          if (index<1 || index>meters_used) goto next_line;
          index--;
          srcpin=strtol(lp,&lp,10);
          if (Gpio_used(srcpin)) {
            AddLog_P(LOG_LEVEL_INFO, PSTR("gpio rx double define!"));
dddef_exit:
            if (script_meter) free(script_meter);
            script_meter=0;
            meters_used=METERS_USED;
            goto init10;
          }
          script_meter_desc[index].srcpin=srcpin;
          if (*lp!=',') goto next_line;
          lp++;
          script_meter_desc[index].type=*lp;
          lp+=2;
          script_meter_desc[index].flag=strtol(lp,&lp,10);
          if (*lp!=',') goto next_line;
          lp++;
          script_meter_desc[index].params=strtol(lp,&lp,10);
          if (*lp!=',') goto next_line;
          lp++;
          script_meter_desc[index].prefix[7]=0;
          for (uint32_t cnt=0; cnt<8; cnt++) {
            if (*lp==SCRIPT_EOL || *lp==',') {
              script_meter_desc[index].prefix[cnt]=0;
              break;
            }
            script_meter_desc[index].prefix[cnt]=*lp++;
          }
          if (*lp==',') {
            lp++;
            script_meter_desc[index].trxpin=strtol(lp,&lp,10);
            if (Gpio_used(script_meter_desc[index].trxpin)) {
              AddLog_P(LOG_LEVEL_INFO, PSTR("gpio tx double define!"));
              goto dddef_exit;
            }
            if (*lp!=',') goto next_line;
            lp++;
            script_meter_desc[index].tsecs=strtol(lp,&lp,10);
            if (*lp==',') {
              lp++;
              char txbuff[256];
              uint32_t txlen=0,tx_entries=1;
              for (uint32_t cnt=0; cnt<sizeof(txbuff); cnt++) {
                if (*lp==SCRIPT_EOL) {
                  txbuff[cnt]=0;
                  txlen=cnt;
                  break;
                }
                if (*lp==',') tx_entries++;
                txbuff[cnt]=*lp++;
              }
              if (txlen) {
                script_meter_desc[index].txmem=(char*)calloc(txlen+2,1);
                if (script_meter_desc[index].txmem) {
                  strcpy(script_meter_desc[index].txmem,txbuff);
                }
                script_meter_desc[index].index=0;
                script_meter_desc[index].max_index=tx_entries;
                sml_send_blocks++;
              }
            }
          }
          if (*lp==SCRIPT_EOL) lp--;
          goto next_line;
        }

#ifdef SML_REPLACE_VARS
        char dstbuf[SML_SRCBSIZE*2];
        Replace_Cmd_Vars(lp,1,dstbuf,sizeof(dstbuf));
        lp+=SML_getlinelen(lp);
        //AddLog_P2(LOG_LEVEL_INFO, PSTR("%s"),dstbuf);
        char *lp1=dstbuf;
        if (*lp1=='-' || isdigit(*lp1)) {
          //toLogEOL(">>",lp);
          // add meters line -1,1-0:1.8.0*255(@10000,H2OIN,cbm,COUNTER,4|
          if (*lp1=='-') lp1++;
          uint8_t mnum=strtol(lp1,0,10);
          if (mnum<1 || mnum>meters_used) goto next_line;
          while (1) {
            if (*lp1==0) {
              *tp++='|';
              goto next_line;
            }
            *tp++=*lp1++;
            index++;
            if (index>=METER_DEF_SIZE) break;
          }
        }
#else

        if (*lp=='-' || isdigit(*lp)) {
          //toLogEOL(">>",lp);
          // add meters line -1,1-0:1.8.0*255(@10000,H2OIN,cbm,COUNTER,4|
          if (*lp=='-') lp++;
          uint8_t mnum=strtol(lp,0,10);
          if (mnum<1 || mnum>meters_used) goto next_line;
          while (1) {
            if (*lp==SCRIPT_EOL) {
              if (*(tp-1)!='|') *tp++='|';
              goto next_line;
            }
            *tp++=*lp++;
            index++;
            if (index>=METER_DEF_SIZE) break;
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
  function counter_callbacks[] = {SML_CounterUpd1,SML_CounterUpd2,SML_CounterUpd3,SML_CounterUpd4};
  uint8_t cindex=0;
  // preloud counters
  for (byte i = 0; i < MAX_COUNTERS; i++) {
      RtcSettings.pulse_counter[i]=Settings.pulse_counter[i];
      sml_counters[i].sml_cnt_last_ts=millis();
  }
  uint32_t uart_index=2;
  for (uint8_t meters=0; meters<meters_used; meters++) {
    if (meter_desc_p[meters].type=='c') {
        if (meter_desc_p[meters].flag&2) {
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
            attachInterrupt(meter_desc_p[meters].srcpin, counter_callbacks[cindex], CHANGE);
            sml_counters[cindex].sml_cnt_old_state=meters;
            sml_counters[cindex].sml_debounce=-meter_desc_p[meters].params;
          }
          InjektCounterValue(meters,RtcSettings.pulse_counter[cindex]);
          cindex++;
        }
    } else {
      // serial input, init
#ifdef SPECIAL_SS
        if (meter_desc_p[meters].type=='m' || meter_desc_p[meters].type=='M' || meter_desc_p[meters].type=='p' || meter_desc_p[meters].type=='R') {
          meter_ss[meters] = new TasmotaSerial(meter_desc_p[meters].srcpin,meter_desc_p[meters].trxpin,1,0,TMSBSIZ);
        } else {
          meter_ss[meters] = new TasmotaSerial(meter_desc_p[meters].srcpin,meter_desc_p[meters].trxpin,1,1,TMSBSIZ);
        }
#else
#ifdef ESP32
        meter_ss[meters] = new HardwareSerial(uart_index);
        if (uart_index==0) { ClaimSerial(); }
        uart_index--;
        if (uart_index<0) uart_index=0;
        meter_ss[meters]->setRxBufferSize(TMSBSIZ);
#else
        meter_ss[meters] = new TasmotaSerial(meter_desc_p[meters].srcpin,meter_desc_p[meters].trxpin,1,0,TMSBSIZ);
#endif
#endif

#ifdef ESP32
        if (meter_desc_p[meters].type=='M') {
          meter_ss[meters]->begin(meter_desc_p[meters].params, SERIAL_8E1,meter_desc_p[meters].srcpin,meter_desc_p[meters].trxpin);
        } else {
          meter_ss[meters]->begin(meter_desc_p[meters].params,SERIAL_8N1,meter_desc_p[meters].srcpin,meter_desc_p[meters].trxpin);
        }
#else
        if (meter_ss[meters]->begin(meter_desc_p[meters].params)) {
          meter_ss[meters]->flush();
        }
        if (meter_ss[meters]->hardwareSerial()) {
          if (meter_desc_p[meters].type=='M') {
            Serial.begin(meter_desc_p[meters].params, SERIAL_8E1);
          }
          ClaimSerial();
          //Serial.setRxBufferSize(512);
        }
#endif
    }
  }

}


#ifdef USE_SML_SCRIPT_CMD
uint32_t SML_SetBaud(uint32_t meter, uint32_t br) {
  if (meter<1 || meter>meters_used) return 0;
  meter--;
  if (!meter_ss[meter]) return 0;

#ifdef ESP32
  meter_ss[meter]->flush();
  meter_ss[meter]->updateBaudRate(br);
  /*
  if (meter_desc_p[meter].type=='M') {
    meter_ss[meter]->begin(br,SERIAL_8E1,meter_desc_p[meter].srcpin,meter_desc_p[meter].trxpin);
  } else {
    meter_ss[meter]->begin(br,SERIAL_8N1,meter_desc_p[meter].srcpin,meter_desc_p[meter].trxpin);
  }*/
#else
  if (meter_ss[meter]->begin(br)) {
    meter_ss[meter]->flush();
  }
  if (meter_ss[meter]->hardwareSerial()) {
    if (meter_desc_p[meter].type=='M') {
      Serial.begin(br, SERIAL_8E1);
    }
  }
#endif
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
  if (index<1 && index>SML_MAX_VARS) { index = 1;}
  return meter_vars[index-1];
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
  for (uint32_t cnt=sml_desc_cnt; cnt<meters_used; cnt++) {
    if (script_meter_desc[cnt].trxpin>=0 && script_meter_desc[cnt].txmem) {
      //AddLog_P2(LOG_LEVEL_INFO, PSTR("100 ms>> %d - %s - %d"),sml_desc_cnt,script_meter_desc[cnt].txmem,script_meter_desc[cnt].tsecs);
      if ((sml_100ms_cnt>=script_meter_desc[cnt].tsecs)) {
        sml_100ms_cnt=0;
        //AddLog_P2(LOG_LEVEL_INFO, PSTR("100 ms>> 2"),cp);
        if (script_meter_desc[cnt].max_index>1) {
          script_meter_desc[cnt].index++;
          if (script_meter_desc[cnt].index>=script_meter_desc[cnt].max_index) {
            script_meter_desc[cnt].index=0;
            sml_desc_cnt++;
          }
          cp=SML_Get_Sequence(script_meter_desc[cnt].txmem,script_meter_desc[cnt].index);
          //SML_Send_Seq(cnt,cp);
        } else {
          cp=script_meter_desc[cnt].txmem;
          //SML_Send_Seq(cnt,cp);
          sml_desc_cnt++;
        }
        //AddLog_P2(LOG_LEVEL_INFO, PSTR(">> %s"),cp);
        SML_Send_Seq(cnt,cp);
        if (sml_desc_cnt>=meters_used) {
          sml_desc_cnt=0;
        }
        break;
      }
    } else {
      sml_desc_cnt++;
    }

    if (sml_desc_cnt>=meters_used) {
      sml_desc_cnt=0;
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
  uint8_t sbuff[32];
  uint8_t *ucp=sbuff,slen=0;
  char *cp=seq;
  uint8_t rflg = 0;
  if (*cp=='r') {
    rflg = 1;
    cp++;
  }
  while (*cp) {
    if (!*cp || !*(cp+1)) break;
    if (*cp==',') break;
    uint8_t iob=(sml_hexnibble(*cp) << 4) | sml_hexnibble(*(cp+1));
    cp+=2;
    *ucp++=iob;
    slen++;
    if (slen>=sizeof(sbuff)) break;
  }
  if (script_meter_desc[meter].type=='m' || script_meter_desc[meter].type=='M') {
    if (!rflg) {
      *ucp++=0;
      *ucp++=2;
      slen+=2;
    }
    // append crc
    uint16_t crc = MBUS_calculateCRC(sbuff,slen);
    *ucp++=lowByte(crc);
    *ucp++=highByte(crc);
    slen+=2;
  }
  if (script_meter_desc[meter].type=='o') {
    for (uint32_t cnt=0;cnt<slen;cnt++) {
      sbuff[cnt]|=(CalcEvenParity(sbuff[cnt])<<7);
    }
  }
  if (script_meter_desc[meter].type=='p') {
    *ucp++=0xc0;
    *ucp++=0xa8;
    *ucp++=1;
    *ucp++=1;
    *ucp++=0;
    *ucp++=SML_PzemCrc(sbuff,6);
    slen+=6;
  }
  meter_ss[meter]->write(sbuff,slen);
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
// in console sensor53 d1,d2,d3 .. or. d0 for normal use
// set counter => sensor53 c1 xxxx
// restart driver => sensor53 r

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
          // set ounter
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
      Settings.pulse_counter[i] = RtcSettings.pulse_counter[i];
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
        else SML_Poll();
        break;
    //  case FUNC_EVERY_50_MSECOND:
    //    if (dump2log) Dump2log();
    //    else SML_Poll();
    //    break;
#ifdef USE_SCRIPT
      case FUNC_EVERY_100_MSECOND:
        if (bitRead(Settings.rule_enabled, 0)) {
          SML_Check_Send();
        }
        break;
#endif // USE_SCRIPT
      case FUNC_JSON_APPEND:
        SML_Show(1);
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
