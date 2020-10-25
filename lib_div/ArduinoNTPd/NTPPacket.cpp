/*
 * File: NTPPacket.cpp
 * Description:
 *   NTP packet representation.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */
 
#include "NTPPacket.h"

void NtpPacket::swapEndian()
{
    reverseBytes_(&rootDelay);
    reverseBytes_(&rootDispersion);
    reverseBytes_(&referenceTimestampSeconds);
    reverseBytes_(&referenceTimestampFraction);
    reverseBytes_(&originTimestampSeconds);
    reverseBytes_(&originTimestampFraction);
    reverseBytes_(&receiveTimestampSeconds);
    reverseBytes_(&receiveTimestampFraction);
    reverseBytes_(&transmitTimestampSeconds);
    reverseBytes_(&transmitTimestampFraction);
}

void NtpPacket::reverseBytes_(uint32_t *number)
{
    char buf[4];
    char *numberAsChar = (char*)number;
    
    buf[0] = numberAsChar[3];
    buf[1] = numberAsChar[2];
    buf[2] = numberAsChar[1];
    buf[3] = numberAsChar[0];
    
    numberAsChar[0] = buf[0];
    numberAsChar[1] = buf[1];
    numberAsChar[2] = buf[2];
    numberAsChar[3] = buf[3];
}
