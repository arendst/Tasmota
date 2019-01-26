/*
  VirtualRCSwitch Arduino libary for remote control outlet switches on snfbridge
  based on RCSwitch - Arduino libary for remote control outlet switches
  Copyright (c) 2011 Suat Özgür.  All right reserved.

  Contributors:
  - Andre Koehler / info(at)tomate-online(dot)de
  - Gordeev Andrey Vladimirovich / gordeev(at)openpyro(dot)com
  - Skineffect / http://forum.ardumote.com/viewtopic.php?f=2&t=46
  - Dominik Fischer / dom_fischer(at)web(dot)de
  - Frank Oltmanns / <first name>.<last name>(at)gmail(dot)com
  - Max Horn / max(at)quendi(dot)de
  - Robert ter Vehn / <first name>.<last name>(at)gmail(dot)com
  
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

#include "virtual_rcswitch.h"

/* Format for protocol definitions:
* {pulselength, Sync bit, "0" bit, "1" bit}
*
* pulselength: pulse length in microseconds, e.g. 350
* Sync bit: {1, 31} means 1 high pulse and 31 low pulses
*     (perceived as a 31*pulselength long pulse, total length of sync bit is
*     32*pulselength microseconds), i.e:
*      _
*     | |_______________________________ (don't count the vertical bars)
* "0" bit: waveform for a data bit of value "0", {1, 3} means 1 high pulse
*     and 3 low pulses, total length (1+3)*pulselength, i.e:
*      _
*     | |___
* "1" bit: waveform for a data bit of value "1", e.g. {3,1}:
*      ___
*     |   |_
*
* These are combined to form Tri-State bits when sending or receiving codes.
*/
#if defined(ESP8266) || defined(ESP32)
static const VirtualRCSwitch::Protocol proto[] = {
#else
static const VirtualRCSwitch::Protocol PROGMEM proto[] = {
//static const VirtualRCSwitch::Protocol proto[] = {
#endif
    {350, {1, 31}, {1, 3}, {3, 1}, false},   // protocol 1
    {650, {1, 10}, {1, 2}, {2, 1}, false},   // protocol 2
    {100, {30, 71}, {4, 11}, {9, 6}, false}, // protocol 3
    {380, {1, 6}, {1, 3}, {3, 1}, false},    // protocol 4
    {500, {6, 14}, {1, 2}, {2, 1}, false},   // protocol 5
    {450, {23, 1}, {1, 2}, {2, 1}, true},    // protocol 6 (HT6P20B)
    {150, {2, 62}, {1, 6}, {6, 1}, false}    // protocol 7 (HS2303-PT, i. e. used in AUKEY Remote)
};

enum
{
    numProto = sizeof(proto) / sizeof(proto[0])
};

VirtualRCSwitch::VirtualRCSwitch()
{
    this->setRepeatTransmit(10);
    this->setProtocol(1);
    this->resetRecording();
}

/**
* Switch a remote switch on (Type D REV)
*
* @param sGroup        Code of the switch group (A,B,C,D)
* @param nDevice       Number of the switch itself (1..3)
*/
void VirtualRCSwitch::switchOn(char sGroup, int nDevice)
{
    this->sendTriState(this->getCodeWordD(sGroup, nDevice, true));
}

/**
* Switch a remote switch off (Type D REV)
*
* @param sGroup        Code of the switch group (A,B,C,D)
* @param nDevice       Number of the switch itself (1..3)
*/
void VirtualRCSwitch::switchOff(char sGroup, int nDevice)
{
    this->sendTriState(this->getCodeWordD(sGroup, nDevice, false));
}

/**
* Switch a remote switch on (Type C Intertechno)
*
* @param sFamily  Familycode (a..f)
* @param nGroup   Number of group (1..4)
* @param nDevice  Number of device (1..4)
*/
void VirtualRCSwitch::switchOn(char sFamily, int nGroup, int nDevice)
{
    this->sendTriState(this->getCodeWordC(sFamily, nGroup, nDevice, true));
}

/**
* Switch a remote switch off (Type C Intertechno)
*
* @param sFamily  Familycode (a..f)
* @param nGroup   Number of group (1..4)
* @param nDevice  Number of device (1..4)
*/
void VirtualRCSwitch::switchOff(char sFamily, int nGroup, int nDevice)
{
    this->sendTriState(this->getCodeWordC(sFamily, nGroup, nDevice, false));
}

/**
* Switch a remote switch on (Type B with two rotary/sliding switches)
*
* @param nAddressCode  Number of the switch group (1..4)
* @param nChannelCode  Number of the switch itself (1..4)
*/
void VirtualRCSwitch::switchOn(int nAddressCode, int nChannelCode)
{
    this->sendTriState(this->getCodeWordB(nAddressCode, nChannelCode, true));
}

/**
* Switch a remote switch off (Type B with two rotary/sliding switches)
*
* @param nAddressCode  Number of the switch group (1..4)
* @param nChannelCode  Number of the switch itself (1..4)
*/
void VirtualRCSwitch::switchOff(int nAddressCode, int nChannelCode)
{
    this->sendTriState(this->getCodeWordB(nAddressCode, nChannelCode, false));
}

/**
* Deprecated, use switchOn(const char* sGroup, const char* sDevice) instead!
* Switch a remote switch on (Type A with 10 pole DIP switches)
*
* @param sGroup        Code of the switch group (refers to DIP switches 1..5 where "1" = on and "0" = off, if all DIP switches are on it's "11111")
* @param nChannelCode  Number of the switch itself (1..5)
*/
void VirtualRCSwitch::switchOn(const char *sGroup, int nChannel)
{
    const char *code[6] = {"00000", "10000", "01000", "00100", "00010", "00001"};
    this->switchOn(sGroup, code[nChannel]);
}

/**
* Deprecated, use switchOff(const char* sGroup, const char* sDevice) instead!
* Switch a remote switch off (Type A with 10 pole DIP switches)
*
* @param sGroup        Code of the switch group (refers to DIP switches 1..5 where "1" = on and "0" = off, if all DIP switches are on it's "11111")
* @param nChannelCode  Number of the switch itself (1..5)
*/
void VirtualRCSwitch::switchOff(const char *sGroup, int nChannel)
{
    const char *code[6] = {"00000", "10000", "01000", "00100", "00010", "00001"};
    this->switchOff(sGroup, code[nChannel]);
}

/**
* Switch a remote switch on (Type A with 10 pole DIP switches)
*
* @param sGroup        Code of the switch group (refers to DIP switches 1..5 where "1" = on and "0" = off, if all DIP switches are on it's "11111")
* @param sDevice       Code of the switch device (refers to DIP switches 6..10 (A..E) where "1" = on and "0" = off, if all DIP switches are on it's "11111")
*/
void VirtualRCSwitch::switchOn(const char *sGroup, const char *sDevice)
{
    this->sendTriState(this->getCodeWordA(sGroup, sDevice, true));
}

/**
* Switch a remote switch off (Type A with 10 pole DIP switches)
*
* @param sGroup        Code of the switch group (refers to DIP switches 1..5 where "1" = on and "0" = off, if all DIP switches are on it's "11111")
* @param sDevice       Code of the switch device (refers to DIP switches 6..10 (A..E) where "1" = on and "0" = off, if all DIP switches are on it's "11111")
*/
void VirtualRCSwitch::switchOff(const char *sGroup, const char *sDevice)
{
    this->sendTriState(this->getCodeWordA(sGroup, sDevice, false));
}

/**
* Returns a char[13], representing the code word to be send.
*
*/
char *VirtualRCSwitch::getCodeWordA(const char *sGroup, const char *sDevice, bool bStatus)
{
    static char sReturn[13];
    int nReturnPos = 0;

    for (int i = 0; i < 5; i++)
    {
        sReturn[nReturnPos++] = (sGroup[i] == '0') ? 'F' : '0';
    }

    for (int i = 0; i < 5; i++)
    {
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
char *VirtualRCSwitch::getCodeWordB(int nAddressCode, int nChannelCode, bool bStatus)
{
    static char sReturn[13];
    int nReturnPos = 0;

    if (nAddressCode < 1 || nAddressCode > 4 || nChannelCode < 1 || nChannelCode > 4)
    {
        return 0;
    }

    for (int i = 1; i <= 4; i++)
    {
        sReturn[nReturnPos++] = (nAddressCode == i) ? '0' : 'F';
    }

    for (int i = 1; i <= 4; i++)
    {
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
char *VirtualRCSwitch::getCodeWordC(char sFamily, int nGroup, int nDevice, bool bStatus)
{
    static char sReturn[13];
    int nReturnPos = 0;

    int nFamily = (int)sFamily - 'a';
    if (nFamily < 0 || nFamily > 15 || nGroup < 1 || nGroup > 4 || nDevice < 1 || nDevice > 4)
    {
        return 0;
    }

    // encode the family into four bits
    sReturn[nReturnPos++] = (nFamily & 1) ? 'F' : '0';
    sReturn[nReturnPos++] = (nFamily & 2) ? 'F' : '0';
    sReturn[nReturnPos++] = (nFamily & 4) ? 'F' : '0';
    sReturn[nReturnPos++] = (nFamily & 8) ? 'F' : '0';

    // encode the device and group
    sReturn[nReturnPos++] = ((nDevice - 1) & 1) ? 'F' : '0';
    sReturn[nReturnPos++] = ((nDevice - 1) & 2) ? 'F' : '0';
    sReturn[nReturnPos++] = ((nGroup - 1) & 1) ? 'F' : '0';
    sReturn[nReturnPos++] = ((nGroup - 1) & 2) ? 'F' : '0';

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
char *VirtualRCSwitch::getCodeWordD(char sGroup, int nDevice, bool bStatus)
{
    static char sReturn[13];
    int nReturnPos = 0;

    // sGroup must be one of the letters in "abcdABCD"
    int nGroup = (sGroup >= 'a') ? (int)sGroup - 'a' : (int)sGroup - 'A';
    if (nGroup < 0 || nGroup > 3 || nDevice < 1 || nDevice > 3)
    {
        return 0;
    }

    for (int i = 0; i < 4; i++)
    {
        sReturn[nReturnPos++] = (nGroup == i) ? '1' : 'F';
    }

    for (int i = 1; i <= 3; i++)
    {
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
void VirtualRCSwitch::sendTriState(const char *sCodeWord)
{
    // turn the tristate code word into the corresponding bit pattern, then send it
    unsigned long code = 0;
    unsigned int length = 0;
    for (const char *p = sCodeWord; *p; p++)
    {
        code <<= 2L;
        switch (*p)
        {
        case '0':
            // bit pattern 00
            break;
        case 'F':
            // bit pattern 01
            code |= 1L;
            break;
        case '1':
            // bit pattern 11
            code |= 3L;
            break;
        }
        length += 2;
    }
    this->send(code, length);
}

/**
* @param sCodeWord   a binary code word consisting of the letter 0, 1
*/
void VirtualRCSwitch::send(const char *sCodeWord)
{
    // turn the tristate code word into the corresponding bit pattern, then send it
    unsigned long code = 0;
    unsigned int length = 0;
    for (const char *p = sCodeWord; *p; p++)
    {
        code <<= 1L;
        if (*p != '0')
            code |= 1L;
        length++;
    }
    this->send(code, length);
}

/**
* Transmit the first 'length' bits of the integer 'code'. The
* bits are sent from MSB to LSB, i.e., first the bit at position length-1,
* then the bit at position length-2, and so on, till finally the bit at position 0.
*/
void VirtualRCSwitch::send(unsigned long code, unsigned int length)
{

    for (int i = length - 1; i >= 0; i--)
    {
        if (code & (1L << i))
            this->transmit(protocol.one);
        else
            this->transmit(protocol.zero);
    }
    this->transmit(protocol.syncFactor);
}

/**
* Sets the protocol to send.
*/
void VirtualRCSwitch::setProtocol(Protocol protocol)
{
    this->protocol = protocol;
}

/**
* Sets the protocol to send, from a list of predefined protocols
*/
void VirtualRCSwitch::setProtocol(int nProtocol)
{
    if (nProtocol < 1 || nProtocol > numProto)
    {
        nProtocol = 1; // TODO: trigger an error, e.g. "bad protocol" ???
    }
#if defined(ESP8266) || defined(ESP32)
    this->protocol = proto[nProtocol - 1];
#else
    memcpy_P(&this->protocol, &proto[nProtocol - 1], sizeof(Protocol));
#endif
}

/**
* Sets the protocol to send with pulse length in microseconds.
*/
void VirtualRCSwitch::setProtocol(int nProtocol, int nPulseLength)
{
    this->setProtocol(nProtocol);
    this->setPulseLength(nPulseLength);
}

/**
* Sets pulse length in microseconds
*/
void VirtualRCSwitch::setPulseLength(int nPulseLength)
{
    this->protocol.pulseLength = nPulseLength;
}

/**
* Sets Repeat Transmits
*/
void VirtualRCSwitch::setRepeatTransmit(int nRepeatTransmit)
{
    this->nRepeatTransmit = nRepeatTransmit;
}

/**
* Transmit a single high-low pulse.
*/
void VirtualRCSwitch::transmit(HighLow pulses)
{

    //uint8_t firstLogicLevel = (this->protocol.invertedSignal) ? LOW : HIGH;
    //uint8_t secondLogicLevel = (this->protocol.invertedSignal) ? HIGH : LOW;

    //digitalWrite(this->nTransmitterPin, firstLogicLevel);
    this->recordBucket(this->protocol.pulseLength * pulses.high);
    //digitalWrite(this->nTransmitterPin, secondLogicLevel);
    this->recordBucket(this->protocol.pulseLength * pulses.low);
}

void VirtualRCSwitch::recordBucket(unsigned int duration)
{
    int bucketssize = this->buckets.size();    
    for (size_t i = 0; i < bucketssize; i++)
    {
        if (this->buckets[i] == duration)
        {            
            rfdata+=String(i,16);
            return;
        }
    }

    this->rfdata+=String(bucketssize,16);
    this->buckets.push_back(duration);
}

void VirtualRCSwitch::resetRecording()
{
    this->buckets.clear();
    this->rfdata = "";  
}

String VirtualRCSwitch::toString()
{
    int frdatalen = this->rfdata.length();
    int buketsize = this->buckets.size(); 
    if (buketsize == 0 || frdatalen < 2)
    {
        return "";
    }

    // see BitBucketConverter.py from gerardovf
    // fixed handling, findSyncPattern with bucketAtEnd = True
    int dlen = 2 + buketsize * 2 + frdatalen / 2;

    char tmp[10];

    sprintf(tmp, "AA B0 %02X %02X %02X ", dlen, buketsize, this->nRepeatTransmit);

    String result = String(tmp);

    for (size_t i = 0; i < buketsize; i++)
    {
        sprintf(tmp, "%04X ", buckets[i]);
        result+=tmp;
    }
    result+=rfdata.substring(frdatalen - 2);
    result+=rfdata.substring(0,frdatalen - 2);
    return result;
}
