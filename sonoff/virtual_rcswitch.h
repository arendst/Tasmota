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
#ifndef _VirtualRCSwitch_h
#define _VirtualRCSwitch_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define LOW 0
#define HIGH 1
#endif

#include <stdint.h>

class VirtualRCSwitch
{
  public:
    VirtualRCSwitch();

    void switchOn(int nGroupNumber, int nSwitchNumber);
    void switchOff(int nGroupNumber, int nSwitchNumber);
    void switchOn(const char *sGroup, int nSwitchNumber);
    void switchOff(const char *sGroup, int nSwitchNumber);
    void switchOn(char sFamily, int nGroup, int nDevice);
    void switchOff(char sFamily, int nGroup, int nDevice);
    void switchOn(const char *sGroup, const char *sDevice);
    void switchOff(const char *sGroup, const char *sDevice);
    void switchOn(char sGroup, int nDevice);
    void switchOff(char sGroup, int nDevice);

    void sendTriState(const char *sCodeWord);
    void send(unsigned long code, unsigned int length);
    void send(const char *sCodeWord);

    void setPulseLength(int nPulseLength);
    void setRepeatTransmit(int nRepeatTransmit);

    /**
	* Description of a single pule, which consists of a high signal
	* whose duration is "high" times the base pulse length, followed
	* by a low signal lasting "low" times the base pulse length.
	* Thus, the pulse overall lasts (high+low)*pulseLength
	*/
    struct HighLow
    {
        uint8_t high;
        uint8_t low;
    };

    /**
	* A "protocol" describes how zero and one bits are encoded into high/low
	* pulses.
	*/
    struct Protocol
    {
        /** base pulse length in microseconds, e.g. 350 */
        uint16_t pulseLength;

        HighLow syncFactor;
        HighLow zero;
        HighLow one;

        /**
		* If true, interchange high and low logic levels in all transmissions.
		*
		* By default, RCSwitch assumes that any signals it sends or receives
		* can be broken down into pulses which start with a high signal level,
		* followed by a a low signal level. This is e.g. the case for the
		* popular PT 2260 encoder chip, and thus many switches out there.
		*
		* But some devices do it the other way around, and start with a low
		* signal level, followed by a high signal level, e.g. the HT6P20B. To
		* accommodate this, one can set invertedSignal to true, which causes
		* RCSwitch to change how it interprets any HighLow struct FOO: It will
		* then assume transmissions start with a low signal lasting
		* FOO.high*pulseLength microseconds, followed by a high signal lasting
		* FOO.low*pulseLength microseconds.
		*/
        bool invertedSignal;
    };

    void setProtocol(Protocol protocol);
    void setProtocol(int nProtocol);
    void setProtocol(int nProtocol, int nPulseLength);

    String toString();
    void resetRecording();

  private:
    char *getCodeWordA(const char *sGroup, const char *sDevice, bool bStatus);
    char *getCodeWordB(int nGroupNumber, int nSwitchNumber, bool bStatus);
    char *getCodeWordC(char sFamily, int nGroup, int nDevice, bool bStatus);
    char *getCodeWordD(char group, int nDevice, bool bStatus);
    void transmit(HighLow pulses);
    void recordBucket(unsigned int duration);

    Protocol protocol;
    int nRepeatTransmit;

    std::vector<unsigned int> buckets = std::vector<unsigned int>();
    String rfdata = "";
};

#endif