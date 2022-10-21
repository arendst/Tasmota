/*
  RCSwitch - Arduino libary for remote control outlet switches
  Copyright (c) 2011 Suat Özgür.  All right reserved.

  Contributors:
  - Andre Koehler / info(at)tomate-online(dot)de
  - Gordeev Andrey Vladimirovich / gordeev(at)openpyro(dot)com
  - Skineffect / http://forum.ardumote.com/viewtopic.php?f=2&t=46
  - Dominik Fischer / dom_fischer(at)web(dot)de
  - Frank Oltmanns / <first name>.<last name>(at)gmail(dot)com
  - Andreas Steinel / A.<lastname>(at)gmail(dot)com
  - Max Horn / max(at)quendi(dot)de
  - Robert ter Vehn / <first name>.<last name>(at)gmail(dot)com
  - Johann Richard / <first name>.<last name>(at)gmail(dot)com
  - Vlad Gheorghe / <first name>.<last name>(at)gmail(dot)com https://github.com/vgheo

  Project home: https://github.com/sui77/rc-switch/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "RCSwitch.h"

#ifdef RaspberryPi
    // PROGMEM and _P functions are for AVR based microprocessors,
    // so we must normalize these for the ARM processor:
    #define PROGMEM
    #define memcpy_P(dest, src, num) memcpy((dest), (src), (num))
#endif

#if defined(ESP8266)
    // interrupt handler and related code must be in RAM on ESP8266,
    // according to issue #46.
    #define RECEIVE_ATTR ICACHE_RAM_ATTR
    #define VAR_ISR_ATTR
#elif defined(ESP32)
    #define RECEIVE_ATTR IRAM_ATTR
    #define VAR_ISR_ATTR DRAM_ATTR
#else
    #define RECEIVE_ATTR
    #define VAR_ISR_ATTR
#endif


/* Protocol description format
 *
 * {
 *    Pulse length, 
 * 
 *    PreambleFactor,
 *    Preamble {high,low},
 * 
 *    HeaderFactor,
 *    Header {high,low},
 * 
 *    "0" bit {high,low},
 *    "1" bit {high,low},
 * 
 *    Inverted Signal,
 *    Guard time
 * }
 * 
 * Pulse length: pulse duration (Te) in microseconds,
 *               for example 350
 * PreambleFactor: Number of high and low states to send
 *                 (One pulse = 2 states, in orther words, number of pulses is
 *                 ceil(PreambleFactor/2).)
 * Preamble: Pulse shape which defines a preamble bit.
 *           Sent ceil(PreambleFactor/2) times.
 *           For example, {1, 2} with factor 3 would send
 *      _    _   
 *     | |__| |__         (each horizontal bar has a duration of Te,
 *                         vertical bars are ignored)
 * HeaderFactor: Number of times to send the header pulse.
 * Header: Pulse shape which defines a header (or "sync"/"clock") pulse.
 *           {1, 31} means one pulse of duration 1 Te high and 31 Te low
 *      _
 *     | |_______________________________ (don't count the vertical bars)
 * 
 * "0" bit: pulse shape defining a data bit, which is a logical "0"
 *          {1, 3} means 1 pulse duration Te high level and 3 low
 *      _
 *     | |___
 * 
 * "1" bit: pulse shape that defines the data bit, which is a logical "1"
 *          {3, 1} means 3 pulses with a duration of Te high level and 1 low
 *      ___
 *     |   |_
 *
 * (note: to form the state bit Z (Tri-State bit), two codes are combined)
 * 
 * Inverted Signal: Signal inversion - if true the signal is inverted
 *                  replacing high to low in a transmitted / received packet
 * Guard time: Separation time between two retries. It will be followed by the 
 *             next preamble of the next packet. In number of Te.
 *             e.g. 39 pulses of duration Te low level
 */

#if defined(ESP8266) || defined(ESP32)
static const VAR_ISR_ATTR RCSwitch::Protocol proto[] = {
#else
static const RCSwitch::Protocol PROGMEM proto[] = {
#endif
  { 350,  0, { 0, 0 }, 1, {  1, 31 }, { 1,  3 }, { 3, 1 }, false,  0 },  // 01 (Princeton, PT-2240)
  { 650,  0, { 0, 0 }, 1, {  1, 10 }, { 1,  2 }, { 2, 1 }, false,  0 },  // 02
  { 100,  0, { 0, 0 }, 1, { 30, 71 }, { 4, 11 }, { 9, 6 }, false,  0 },  // 03
  { 380,  0, { 0, 0 }, 1, {  1,  6 }, { 1,  3 }, { 3, 1 }, false,  0 },  // 04
  { 500,  0, { 0, 0 }, 1, {  6, 14 }, { 1,  2 }, { 2, 1 }, false,  0 },  // 05
  { 450,  0, { 0, 0 }, 1, { 23,  1 }, { 1,  2 }, { 2, 1 }, true,   0 },  // 06 (HT6P20B)
  { 150,  0, { 0, 0 }, 1, {  2, 62 }, { 1,  6 }, { 6, 1 }, false,  0 },  // 07 (HS2303-PT, i. e. used in AUKEY Remote)
  { 320,  0, { 0, 0 }, 1, { 36,  1 }, { 1,  2 }, { 2, 1 }, true,   0 },  // 08 (Came 12bit, HT12E)
  { 700,  0, { 0, 0 }, 1, { 32,  1 }, { 1,  2 }, { 2, 1 }, true,   0 },  // 09 (Nice_Flo 12bit)
  { 420,  0, { 0, 0 }, 1, { 60,  6 }, { 1,  2 }, { 2, 1 }, true,   0 },  // 10 (V2 phoenix)
  { 500,  2, { 3, 3 }, 0, {  0,  0 }, { 1,  2 }, { 2, 1 }, false, 37 },  // 11 (Nice_FloR-S 52bit)
  { 400, 23, { 1, 1 }, 1, {  0,  9 }, { 2,  1 }, { 1, 2 }, false, 39 },  // 12 Placeholder not working! (Keeloq 64/66)
  { 300,  6, { 2, 2 }, 3, {  8,  3 }, { 2,  2 }, { 3, 3 }, false,  0 },  // 13 test (CFM)
  { 250, 12, { 4, 4 }, 0, {  0,  0 }, { 1,  1 }, { 2, 2 }, false,  0 },  // 14 test (StarLine)
  { 500,  0, { 0, 0 }, 0, { 100, 1 }, { 1,  2 }, { 2, 1 }, false, 35 },  // 15

  { 361,  0, { 0, 0 }, 1, {  52,  1 }, { 1,  3 }, { 3, 1 }, true,   0 }, // 16 (Einhell)
  { 500,  0, { 0, 0 }, 1, {   1, 23 }, { 1,  2 }, { 2, 1 }, false,  0 }, // 17 (InterTechno PAR-1000)
  { 180,  0, { 0, 0 }, 1, {   1, 15 }, { 1,  1 }, { 1, 8 }, false,  0 }, // 18 (Intertechno ITT-1500)
  { 350,  0, { 0, 0 }, 1, {   1,  2 }, { 0,  2 }, { 3, 2 }, false,  0 }, // 19 (Murcury)
  { 150,  0, { 0, 0 }, 1, {  34,  3 }, { 1,  3 }, { 3, 1 }, false,  0 }, // 20 (AC114)
  { 360,  0, { 0, 0 }, 1, {  13,  4 }, { 1,  2 }, { 2, 1 }, false,  0 }, // 21 (DC250)
  { 650,  0, { 0, 0 }, 1, {   1, 10 }, { 1,  2 }, { 2, 1 }, true,   0 }, // 22 (Mandolyn/Lidl TR-502MSV/RC-402/RC-402DX)
  { 641,  0, { 0, 0 }, 1, { 115,  1 }, { 1,  2 }, { 2, 1 }, true,   0 }, // 23 (Lidl TR-502MSV/RC-402 - Flavien)
  { 620,  0, { 0, 0 }, 1, {   0, 64 }, { 0,  1 }, { 1, 0 }, false,  0 }, // 24 (Lidl TR-502MSV/RC701)
  { 560,  0, { 0, 0 }, 1, {  16,  8 }, { 1,  1 }, { 1, 3 }, false,  0 }, // 25 (NEC)
  { 385,  0, { 0, 0 }, 1, {   1, 17 }, { 1,  2 }, { 2, 1 }, false,  0 }, // 26 (Arlec RC210)
  { 188,  0, { 0, 0 }, 1, {   1, 31 }, { 1,  3 }, { 3, 1 }, false,  0 }, // 27 (Zap, FHT-7901)

  { 700,  1, { 0, 1 }, 1, { 116,  0 }, { 1,  2 }, { 2, 1 }, true,   0 }, // 28 (Quigg GT-7000) from @Tho85 https://github.com/sui77/rc-switch/pull/115
  { 220,  0, { 0, 0 }, 1, {   1, 46 }, { 1,  6 }, { 1, 1 }, false,  2 }, // 29 (NEXA)
  { 260,  0, { 0, 0 }, 1, {   1,  8 }, { 1,  4 }, { 4, 1 }, true,   0 },  // 30 (Anima)

  { 400,  0, { 0, 0 }, 1, {   1,  1 }, { 1,  2 }, { 2, 1 }, false, 43 },  // 31 (Mertik Maxitrol G6R-H4T1)
  { 365,  0, { 0, 0 }, 1, {  18,  1 }, { 3,  1 }, { 1, 3 }, true,   0 },  // 32 (1ByOne Doorbell) from @Fatbeard https://github.com/sui77/rc-switch/pull/277
  { 340,  0, { 0, 0 }, 1, {  14,  4 }, { 1,  2 }, { 2, 1 }, false,  0 },  // 33 (Dooya Control DC2708L)
  { 120,  0, { 0, 0 }, 1, {   1, 28 }, { 1,  3 }, { 3, 1 }, false,  0 },   // 34 DIGOO SD10 - so as to use this protocol RCSWITCH_SEPARATION_LIMIT must be set to 2600
  { 20,   0, { 0, 0 }, 1, { 239, 78 }, {20, 35 }, {35, 20}, false, 10000 } // 35 Dooya 5-Channel blinds remote DC1603
};

enum {
   numProto = sizeof(proto) / sizeof(proto[0])
};

#if not defined( RCSwitchDisableReceiving )
volatile unsigned long long RCSwitch::nReceivedValue = 0;
volatile unsigned long long RCSwitch::nReceiveProtocolMask;
volatile unsigned int RCSwitch::nReceivedBitlength = 0;
volatile unsigned int RCSwitch::nReceivedDelay = 0;
volatile unsigned int RCSwitch::nReceivedProtocol = 0;
int RCSwitch::nReceiveTolerance = 60;
const unsigned int RCSwitch::nSeparationLimit = RCSWITCH_SEPARATION_LIMIT;
unsigned int RCSwitch::timings[RCSWITCH_MAX_CHANGES];
unsigned int RCSwitch::buftimings[4];
#endif

RCSwitch::RCSwitch() {
  this->nTransmitterPin = -1;
  this->setRepeatTransmit(5);
  this->setProtocol(1);
  #if not defined( RCSwitchDisableReceiving )
  this->nReceiverInterrupt = -1;
  this->setReceiveTolerance(60);
  RCSwitch::nReceivedValue = 0;
  RCSwitch::nReceiveProtocolMask = (1ULL << numProto)-1;  //pow(2,numProto)-1;
  #endif
}

uint8_t RCSwitch::getNumProtos() {
  return numProto;
}
/**
  * Sets the protocol to send.
  */
void RCSwitch::setProtocol(Protocol protocol) {
  this->protocol = protocol;
}

/**
  * Sets the protocol to send, from a list of predefined protocols
  */
void RCSwitch::setProtocol(int nProtocol) {
  if (nProtocol < 1 || nProtocol > numProto) {
    nProtocol = 1;  // TODO: trigger an error, e.g. "bad protocol" ???
  }
#if defined(ESP8266) || defined(ESP32)
  this->protocol = proto[nProtocol-1];
#else
  memcpy_P(&this->protocol, &proto[nProtocol-1], sizeof(Protocol));
#endif
}

/**
  * Sets the protocol to send with pulse length in microseconds.
  */
void RCSwitch::setProtocol(int nProtocol, int nPulseLength) {
  setProtocol(nProtocol);
  this->setPulseLength(nPulseLength);
}


/**
  * Sets pulse length in microseconds
  */
void RCSwitch::setPulseLength(int nPulseLength) {
  this->protocol.pulseLength = nPulseLength;
}

/**
 * Sets Repeat Transmits
 */
void RCSwitch::setRepeatTransmit(int nRepeatTransmit) {
  this->nRepeatTransmit = nRepeatTransmit;
}

/**
 * Set Receiving Tolerance
 */
#if not defined( RCSwitchDisableReceiving )
void RCSwitch::setReceiveTolerance(int nPercent) {
  RCSwitch::nReceiveTolerance = nPercent;
}

void RCSwitch::setReceiveProtocolMask(unsigned long long mask) {
  RCSwitch::nReceiveProtocolMask = mask;
}
#endif


/**
 * Enable transmissions
 *
 * @param nTransmitterPin    Arduino Pin to which the sender is connected to
 */
void RCSwitch::enableTransmit(int nTransmitterPin) {
  this->nTransmitterPin = nTransmitterPin;
  pinMode(this->nTransmitterPin, OUTPUT);
}

/**
  * Disable transmissions
  */
void RCSwitch::disableTransmit() {
  this->nTransmitterPin = -1;
}

/**
 * Switch a remote switch on (Type D REV)
 *
 * @param sGroup        Code of the switch group (A,B,C,D)
 * @param nDevice       Number of the switch itself (1..3)
 */
void RCSwitch::switchOn(char sGroup, int nDevice) {
  this->sendTriState( this->getCodeWordD(sGroup, nDevice, true) );
}

/**
 * Switch a remote switch off (Type D REV)
 *
 * @param sGroup        Code of the switch group (A,B,C,D)
 * @param nDevice       Number of the switch itself (1..3)
 */
void RCSwitch::switchOff(char sGroup, int nDevice) {
  this->sendTriState( this->getCodeWordD(sGroup, nDevice, false) );
}

/**
 * Switch a remote switch on (Type C Intertechno)
 *
 * @param sFamily  Familycode (a..f)
 * @param nGroup   Number of group (1..4)
 * @param nDevice  Number of device (1..4)
  */
void RCSwitch::switchOn(char sFamily, int nGroup, int nDevice) {
  this->sendTriState( this->getCodeWordC(sFamily, nGroup, nDevice, true) );
}

/**
 * Switch a remote switch off (Type C Intertechno)
 *
 * @param sFamily  Familycode (a..f)
 * @param nGroup   Number of group (1..4)
 * @param nDevice  Number of device (1..4)
 */
void RCSwitch::switchOff(char sFamily, int nGroup, int nDevice) {
  this->sendTriState( this->getCodeWordC(sFamily, nGroup, nDevice, false) );
}

/**
 * Switch a remote switch on (Type B with two rotary/sliding switches)
 *
 * @param nAddressCode  Number of the switch group (1..4)
 * @param nChannelCode  Number of the switch itself (1..4)
 */
void RCSwitch::switchOn(int nAddressCode, int nChannelCode) {
  this->sendTriState( this->getCodeWordB(nAddressCode, nChannelCode, true) );
}

/**
 * Switch a remote switch off (Type B with two rotary/sliding switches)
 *
 * @param nAddressCode  Number of the switch group (1..4)
 * @param nChannelCode  Number of the switch itself (1..4)
 */
void RCSwitch::switchOff(int nAddressCode, int nChannelCode) {
  this->sendTriState( this->getCodeWordB(nAddressCode, nChannelCode, false) );
}

/**
 * Deprecated, use switchOn(const char* sGroup, const char* sDevice) instead!
 * Switch a remote switch on (Type A with 10 pole DIP switches)
 *
 * @param sGroup        Code of the switch group (refers to DIP switches 1..5 where "1" = on and "0" = off, if all DIP switches are on it's "11111")
 * @param nChannelCode  Number of the switch itself (1..5)
 */
void RCSwitch::switchOn(const char* sGroup, int nChannel) {
  const char* code[6] = { "00000", "10000", "01000", "00100", "00010", "00001" };
  this->switchOn(sGroup, code[nChannel]);
}

/**
 * Deprecated, use switchOff(const char* sGroup, const char* sDevice) instead!
 * Switch a remote switch off (Type A with 10 pole DIP switches)
 *
 * @param sGroup        Code of the switch group (refers to DIP switches 1..5 where "1" = on and "0" = off, if all DIP switches are on it's "11111")
 * @param nChannelCode  Number of the switch itself (1..5)
 */
void RCSwitch::switchOff(const char* sGroup, int nChannel) {
  const char* code[6] = { "00000", "10000", "01000", "00100", "00010", "00001" };
  this->switchOff(sGroup, code[nChannel]);
}

/**
 * Switch a remote switch on (Type A with 10 pole DIP switches)
 *
 * @param sGroup        Code of the switch group (refers to DIP switches 1..5 where "1" = on and "0" = off, if all DIP switches are on it's "11111")
 * @param sDevice       Code of the switch device (refers to DIP switches 6..10 (A..E) where "1" = on and "0" = off, if all DIP switches are on it's "11111")
 */
void RCSwitch::switchOn(const char* sGroup, const char* sDevice) {
  this->sendTriState( this->getCodeWordA(sGroup, sDevice, true) );
}

/**
 * Switch a remote switch off (Type A with 10 pole DIP switches)
 *
 * @param sGroup        Code of the switch group (refers to DIP switches 1..5 where "1" = on and "0" = off, if all DIP switches are on it's "11111")
 * @param sDevice       Code of the switch device (refers to DIP switches 6..10 (A..E) where "1" = on and "0" = off, if all DIP switches are on it's "11111")
 */
void RCSwitch::switchOff(const char* sGroup, const char* sDevice) {
  this->sendTriState( this->getCodeWordA(sGroup, sDevice, false) );
}


/**
 * Returns a char[13], representing the code word to be send.
 *
 */
char* RCSwitch::getCodeWordA(const char* sGroup, const char* sDevice, bool bStatus) {
  static char sReturn[13];
  int nReturnPos = 0;

  for (int i = 0; i < 5; i++) {
    sReturn[nReturnPos++] = (sGroup[i] == '0') ? 'F' : '0';
  }

  for (int i = 0; i < 5; i++) {
    sReturn[nReturnPos++] = (sDevice[i] == '0') ? 'F' : '0';
  }

  sReturn[nReturnPos++] = bStatus ? '0' : 'F';
  sReturn[nReturnPos++] = bStatus ? 'F' : '0';

  sReturn[nReturnPos] = '\0';
  return sReturn;
}

/**
 * Encoding for type B switches with two rotary/sliding switches.
 *
 * The code word is a tristate word and with following bit pattern:
 *
 * +-----------------------------+-----------------------------+----------+------------+
 * | 4 bits address              | 4 bits address              | 3 bits   | 1 bit      |
 * | switch group                | switch number               | not used | on / off   |
 * | 1=0FFF 2=F0FF 3=FF0F 4=FFF0 | 1=0FFF 2=F0FF 3=FF0F 4=FFF0 | FFF      | on=F off=0 |
 * +-----------------------------+-----------------------------+----------+------------+
 *
 * @param nAddressCode  Number of the switch group (1..4)
 * @param nChannelCode  Number of the switch itself (1..4)
 * @param bStatus       Whether to switch on (true) or off (false)
 *
 * @return char[13], representing a tristate code word of length 12
 */
char* RCSwitch::getCodeWordB(int nAddressCode, int nChannelCode, bool bStatus) {
  static char sReturn[13];
  int nReturnPos = 0;

  if (nAddressCode < 1 || nAddressCode > 4 || nChannelCode < 1 || nChannelCode > 4) {
    return 0;
  }

  for (int i = 1; i <= 4; i++) {
    sReturn[nReturnPos++] = (nAddressCode == i) ? '0' : 'F';
  }

  for (int i = 1; i <= 4; i++) {
    sReturn[nReturnPos++] = (nChannelCode == i) ? '0' : 'F';
  }

  sReturn[nReturnPos++] = 'F';
  sReturn[nReturnPos++] = 'F';
  sReturn[nReturnPos++] = 'F';

  sReturn[nReturnPos++] = bStatus ? 'F' : '0';

  sReturn[nReturnPos] = '\0';
  return sReturn;
}

/**
 * Like getCodeWord (Type C = Intertechno)
 */
char* RCSwitch::getCodeWordC(char sFamily, int nGroup, int nDevice, bool bStatus) {
  static char sReturn[13];
  int nReturnPos = 0;

  int nFamily = (int)sFamily - 'a';
  if ( nFamily < 0 || nFamily > 15 || nGroup < 1 || nGroup > 4 || nDevice < 1 || nDevice > 4) {
    return 0;
  }

  // encode the family into four bits
  sReturn[nReturnPos++] = (nFamily & 1) ? 'F' : '0';
  sReturn[nReturnPos++] = (nFamily & 2) ? 'F' : '0';
  sReturn[nReturnPos++] = (nFamily & 4) ? 'F' : '0';
  sReturn[nReturnPos++] = (nFamily & 8) ? 'F' : '0';

  // encode the device and group
  sReturn[nReturnPos++] = ((nDevice-1) & 1) ? 'F' : '0';
  sReturn[nReturnPos++] = ((nDevice-1) & 2) ? 'F' : '0';
  sReturn[nReturnPos++] = ((nGroup-1) & 1) ? 'F' : '0';
  sReturn[nReturnPos++] = ((nGroup-1) & 2) ? 'F' : '0';

  // encode the status code
  sReturn[nReturnPos++] = '0';
  sReturn[nReturnPos++] = 'F';
  sReturn[nReturnPos++] = 'F';
  sReturn[nReturnPos++] = bStatus ? 'F' : '0';

  sReturn[nReturnPos] = '\0';
  return sReturn;
}

/**
 * Encoding for the REV Switch Type
 *
 * The code word is a tristate word and with following bit pattern:
 *
 * +-----------------------------+-------------------+----------+--------------+
 * | 4 bits address              | 3 bits address    | 3 bits   | 2 bits       |
 * | switch group                | device number     | not used | on / off     |
 * | A=1FFF B=F1FF C=FF1F D=FFF1 | 1=0FF 2=F0F 3=FF0 | 000      | on=10 off=01 |
 * +-----------------------------+-------------------+----------+--------------+
 *
 * Source: http://www.the-intruder.net/funksteckdosen-von-rev-uber-arduino-ansteuern/
 *
 * @param sGroup        Name of the switch group (A..D, resp. a..d)
 * @param nDevice       Number of the switch itself (1..3)
 * @param bStatus       Whether to switch on (true) or off (false)
 *
 * @return char[13], representing a tristate code word of length 12
 */
char* RCSwitch::getCodeWordD(char sGroup, int nDevice, bool bStatus) {
  static char sReturn[13];
  int nReturnPos = 0;

  // sGroup must be one of the letters in "abcdABCD"
  int nGroup = (sGroup >= 'a') ? (int)sGroup - 'a' : (int)sGroup - 'A';
  if ( nGroup < 0 || nGroup > 3 || nDevice < 1 || nDevice > 3) {
    return 0;
  }

  for (int i = 0; i < 4; i++) {
    sReturn[nReturnPos++] = (nGroup == i) ? '1' : 'F';
  }

  for (int i = 1; i <= 3; i++) {
    sReturn[nReturnPos++] = (nDevice == i) ? '1' : 'F';
  }

  sReturn[nReturnPos++] = '0';
  sReturn[nReturnPos++] = '0';
  sReturn[nReturnPos++] = '0';

  sReturn[nReturnPos++] = bStatus ? '1' : '0';
  sReturn[nReturnPos++] = bStatus ? '0' : '1';

  sReturn[nReturnPos] = '\0';
  return sReturn;
}

/**
 * @param sCodeWord   a tristate code word consisting of the letter 0, 1, F
 */
void RCSwitch::sendTriState(const char* sCodeWord) {
  // turn the tristate code word into the corresponding bit pattern, then send it
  unsigned long long code = 0;
  unsigned int length = 0;
  for (const char* p = sCodeWord; *p; p++) {
    code <<= 2L;
    switch (*p) {
      case '0':
        // bit pattern 00
        break;
      case 'F':
        // bit pattern 01
        code |= 1ULL;
        break;
      case '1':
        // bit pattern 11
        code |= 3ULL;
        break;
    }
    length += 2;
  }
  this->send(code, length);
}

/**
 * @param duration   no. of microseconds to delay
 */
static inline void safeDelayMicroseconds(unsigned long duration) {
#if defined(ESP8266) || defined(ESP32)
  if (duration > 10000) {
    // if delay > 10 milliseconds, use yield() to avoid wdt reset
    unsigned long start = micros();
    while ((micros() - start) < duration) {
      yield();
    }
  }
  else {
    delayMicroseconds(duration);
  }
#else
  delayMicroseconds(duration);
#endif
}

/**
 * @param sCodeWord   a binary code word consisting of the letter 0, 1
 */
void RCSwitch::send(const char* sCodeWord) {
  // turn the tristate code word into the corresponding bit pattern, then send it
  unsigned long long code = 0;
  unsigned int length = 0;
  for (const char* p = sCodeWord; *p; p++) {
    code <<= 1ULL;
    if (*p != '0')
      code |= 1ULL;
    length++;
  }
  this->send(code, length);
}

/**
 * Transmit the first 'length' bits of the integer 'code'. The
 * bits are sent from MSB to LSB, i.e., first the bit at position length-1,
 * then the bit at position length-2, and so on, till finally the bit at position 0.
 */
void RCSwitch::send(unsigned long long code, unsigned int length) {
  if (this->nTransmitterPin == -1)
    return;

#if not defined( RCSwitchDisableReceiving )
  // make sure the receiver is disabled while we transmit
  int nReceiverInterrupt_backup = nReceiverInterrupt;
  if (nReceiverInterrupt_backup != -1) {
    this->disableReceive();
  }
#endif

  // repeat sending the packet nRepeatTransmit times
  for (int nRepeat = 0; nRepeat < nRepeatTransmit; nRepeat++) {
    // send the preamble
    for (int i = 0; i < ((protocol.PreambleFactor / 2) + (protocol.PreambleFactor %2 )); i++) {
       this->transmit({protocol.Preamble.high, protocol.Preamble.low});
    }
    // send the header
    if (protocol.HeaderFactor > 0) {
      for (int i = 0; i < protocol.HeaderFactor; i++) {
        this->transmit(protocol.Header);
      }
    }
    // send the code
    for (int i = length - 1; i >= 0; i--) {
      if (code & (1ULL << i))
        this->transmit(protocol.one);
      else
        this->transmit(protocol.zero);
    }
    // for kilok, there should be a duration of 66, and 64 significant data codes are stored
    // send two more bits for even count
    if (length == 64) {
      if (nRepeat == 0) {
        this->transmit(protocol.zero);
        this->transmit(protocol.zero);
      } else {
        this->transmit(protocol.one);
        this->transmit(protocol.one);
      }
     }
    // Set the guard Time
    if (protocol.Guard > 0) {
      digitalWrite(this->nTransmitterPin, LOW);
      safeDelayMicroseconds(this->protocol.pulseLength * protocol.Guard);
    }
  }

  // Disable transmit after sending (i.e., for inverted protocols)
  digitalWrite(this->nTransmitterPin, LOW);

#if not defined( RCSwitchDisableReceiving )
  // enable receiver again if we just disabled it
  if (nReceiverInterrupt_backup != -1) {
    this->enableReceive(nReceiverInterrupt_backup);
  }
#endif
}

/**
 * Transmit a single high-low pulse.
 */
void RCSwitch::transmit(HighLow pulses) {
  uint8_t firstLogicLevel = (this->protocol.invertedSignal) ? LOW : HIGH;
  uint8_t secondLogicLevel = (this->protocol.invertedSignal) ? HIGH : LOW;

  if (pulses.high > 0) {
    digitalWrite(this->nTransmitterPin, firstLogicLevel);
    delayMicroseconds( this->protocol.pulseLength * pulses.high);
  }
  if (pulses.low > 0) {
    digitalWrite(this->nTransmitterPin, secondLogicLevel);
    delayMicroseconds( this->protocol.pulseLength * pulses.low);
  }
}


#if not defined( RCSwitchDisableReceiving )
/**
 * Enable receiving data
 */
void RCSwitch::enableReceive(int interrupt) {
  this->nReceiverInterrupt = interrupt;
  this->enableReceive();
}

void RCSwitch::enableReceive() {
  if (this->nReceiverInterrupt != -1) {
    RCSwitch::nReceivedValue = 0;
    RCSwitch::nReceivedBitlength = 0;
#if defined(RaspberryPi) // Raspberry Pi
    wiringPiISR(this->nReceiverInterrupt, INT_EDGE_BOTH, &handleInterrupt);
#else // Arduino
    attachInterrupt(this->nReceiverInterrupt, handleInterrupt, CHANGE);
#endif
  }
}

/**
 * Disable receiving data
 */
void RCSwitch::disableReceive() {
#if not defined(RaspberryPi) // Arduino
  detachInterrupt(this->nReceiverInterrupt);
#endif // For Raspberry Pi (wiringPi) you can't unregister the ISR
  this->nReceiverInterrupt = -1;
}

bool RCSwitch::available() {
  return RCSwitch::nReceivedValue != 0;
}

void RCSwitch::resetAvailable() {
  RCSwitch::nReceivedValue = 0;
}

unsigned long long RCSwitch::getReceivedValue() {
  return RCSwitch::nReceivedValue;
}

unsigned int RCSwitch::getReceivedBitlength() {
  return RCSwitch::nReceivedBitlength;
}

unsigned int RCSwitch::getReceivedDelay() {
  return RCSwitch::nReceivedDelay;
}

unsigned int RCSwitch::getReceivedProtocol() {
  return RCSwitch::nReceivedProtocol;
}

unsigned int* RCSwitch::getReceivedRawdata() {
  return RCSwitch::timings;
}

/* helper function for the receiveProtocol method */
static inline unsigned int diff(int A, int B) {
  return abs(A - B);
}

/**
 *
 */
bool RECEIVE_ATTR RCSwitch::receiveProtocol(const int p, unsigned int changeCount) {
#if defined(ESP8266) || defined(ESP32)
    const Protocol &pro = proto[p-1];
#else
    Protocol pro;
    memcpy_P(&pro, &proto[p-1], sizeof(Protocol));
#endif

    unsigned long long code = 0;
    unsigned int FirstTiming = 0;
    if (pro.PreambleFactor > 0) {
      FirstTiming = pro.PreambleFactor + 1;
    }
    unsigned int BeginData = 0;
    if (pro.HeaderFactor > 0) {
      BeginData = (pro.invertedSignal) ? (2) : (1);
      // Header pulse count correction for more than one
      if (pro.HeaderFactor > 1) {
        BeginData += (pro.HeaderFactor - 1) * 2;
      }
    }
    //Assuming the longer pulse length is the pulse captured in timings[FirstTiming]
    // берем наибольшее значение из Header
    const unsigned int syncLengthInPulses =  ((pro.Header.low) > (pro.Header.high)) ? (pro.Header.low) : (pro.Header.high);
    // определяем длительность Te как длительность первого импульса header деленную на количество импульсов в нем
    // или как длительность импульса preamble деленную на количество Te в нем
    unsigned int sdelay = 0;
    if (syncLengthInPulses > 0) {
      sdelay = RCSwitch::timings[FirstTiming] / syncLengthInPulses;
    } else {
      sdelay = RCSwitch::timings[FirstTiming-2] / pro.PreambleFactor;
    }
    const unsigned int delay = sdelay;
    // nReceiveTolerance = 60
    // допустимое отклонение длительностей импульсов на 60 %
    const unsigned int delayTolerance = delay * RCSwitch::nReceiveTolerance / 100;

    // 0 - sync перед preamble или data
    // BeginData - сдвиг на 1 или 2 от sync к preamble/data
    // FirstTiming - сдвиг на preamble к header
    // firstDataTiming первый импульс data
    // bitChangeCount - количество импульсов в data

    /* For protocols that start low, the sync period looks like
     *               _________
     * _____________|         |XXXXXXXXXXXX|
     *
     * |--1st dur--|-2nd dur-|-Start data-|
     *
     * The 3rd saved duration starts the data.
     *
     * For protocols that start high, the sync period looks like
     *
     *  ______________
     * |              |____________|XXXXXXXXXXXXX|
     *
     * |-filtered out-|--1st dur--|--Start data--|
     *
     * The 2nd saved duration starts the data
     */
    // если invertedSignal=false, то сигнал начинается с 1 элемента массива (высокий уровень)
    // если invertedSignal=true, то сигнал начинается со 2 элемента массива (низкий уровень)
    // добавляем поправку на Преамбулу и Хедер
    const unsigned int firstDataTiming = BeginData + FirstTiming;
    unsigned int bitChangeCount = changeCount - firstDataTiming - 1 + pro.invertedSignal;
    if (bitChangeCount > 128) {
      bitChangeCount = 128;
    }

    for (unsigned int i = firstDataTiming; i < firstDataTiming + bitChangeCount; i += 2) {
        code <<= 1;
        if (diff(RCSwitch::timings[i], delay * pro.zero.high) < delayTolerance &&
            diff(RCSwitch::timings[i + 1], delay * pro.zero.low) < delayTolerance) {
            // zero
        } else if (diff(RCSwitch::timings[i], delay * pro.one.high) < delayTolerance &&
                   diff(RCSwitch::timings[i + 1], delay * pro.one.low) < delayTolerance) {
            // one
            code |= 1;
        } else {
            // Failed
            return false;
        }
    }

    if (bitChangeCount > 14) {    // ignore very short transmissions: no device sends them, so this must be noise
        RCSwitch::nReceivedValue = code;
        RCSwitch::nReceivedBitlength = bitChangeCount / 2;
        RCSwitch::nReceivedDelay = delay;
        RCSwitch::nReceivedProtocol = p;
        return true;
    }

    return false;
}

void RECEIVE_ATTR RCSwitch::handleInterrupt() {

  static unsigned int changeCount = 0;
  static unsigned long lastTime = 0;
  static byte repeatCount = 0;

  const long time = micros();
  const unsigned int duration = time - lastTime;

  RCSwitch::buftimings[3]=RCSwitch::buftimings[2];
  RCSwitch::buftimings[2]=RCSwitch::buftimings[1];
  RCSwitch::buftimings[1]=RCSwitch::buftimings[0];
  RCSwitch::buftimings[0]=duration;

  if (duration > RCSwitch::nSeparationLimit ||
      changeCount == 156 ||
      (diff(RCSwitch::buftimings[3], RCSwitch::buftimings[2]) < 50 &&
        diff(RCSwitch::buftimings[2], RCSwitch::buftimings[1]) < 50 &&
        changeCount > 25)) {
    // принят длинный импульс продолжительностью более nSeparationLimit (4300)
    // A long stretch without signal level change occurred. This could
    // be the gap between two transmission.
    if (diff(duration, RCSwitch::timings[0]) < 400 ||
        changeCount == 156 ||
        (diff(RCSwitch::buftimings[3], RCSwitch::timings[1]) < 50 &&
          diff(RCSwitch::buftimings[2], RCSwitch::timings[2]) < 50 &&
          diff(RCSwitch::buftimings[1], RCSwitch::timings[3]) < 50 &&
          changeCount > 25)) {
      // если его длительность отличается от первого импульса,
      // который приняли раньше, менее чем на +-200 (исходно 200)
      // то считаем это повторным пакетом и игнорируем его
      // This long signal is close in length to the long signal which
      // started the previously recorded timings; this suggests that
      // it may indeed by a a gap between two transmissions (we assume
      // here that a sender will send the signal multiple times,
      // with roughly the same gap between them).

      // количество повторных пакетов
      repeatCount++;
      // при приеме второго повторного начинаем анализ принятого первым
      if (repeatCount == 1) {
        unsigned long long thismask = 1;
        for(unsigned int i = 1; i <= numProto; i++) {
          if (RCSwitch::nReceiveProtocolMask & thismask) {
            if (receiveProtocol(i, changeCount)) {
              // receive succeeded for protocol i
              break;
            }
          }
          thismask <<= 1;
        }
        // очищаем количество повторных пакетов
        repeatCount = 0;
      }
    }
    // дительность отличается более чем на +-200 от первого
    // принятого ранее, очищаем счетчик для приема нового пакета
    changeCount = 0;
    if (diff(RCSwitch::buftimings[3], RCSwitch::buftimings[2]) < 50 &&
        diff(RCSwitch::buftimings[2], RCSwitch::buftimings[1]) < 50) {
      RCSwitch::timings[1]=RCSwitch::buftimings[3];
      RCSwitch::timings[2]=RCSwitch::buftimings[2];
      RCSwitch::timings[3]=RCSwitch::buftimings[1];
      changeCount = 4;
    }
  }

  // detect overflow
  if (changeCount >= RCSWITCH_MAX_CHANGES) {
    changeCount = 0;
    repeatCount = 0;
  }

  // заносим в массив длительность очередного принятого импульса
  if (changeCount > 0 && duration < 100) { // игнорируем шумовые всплески менее 100 мкс
    RCSwitch::timings[changeCount-1] += duration;
  } else {
    RCSwitch::timings[changeCount++] = duration;
  }
  lastTime = time;
}
#endif
