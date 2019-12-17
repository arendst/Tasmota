/*
 * File: NTPServer.cpp
 * Description:
 *   NTP server implementation.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */


#include <WiFiUdp.h>

#include "NTPPacket.h"
#include "NTPServer.h"

#define NTP_PORT 123
#define NTP_TIMESTAMP_DIFF     (2208988800)    // 1900 to 1970 in seconds

bool NtpServer::beginListening()
{
    if (timeServerPort_.begin(NTP_PORT)){
      return true;
    }
    return false;
}

bool NtpServer::processOneRequest(uint32_t utc, uint32_t millisecs) 
{
    // We need the time we've received the packet in our response.
    uint32_t recvSecs =  utc + NTP_TIMESTAMP_DIFF;
    double recvFractDouble = (double)millisecs/0.00023283064365386963; // millisec/((10^6)/(2^32))
    uint32_t recvFract = (double)recvFractDouble; //TODO: really handle this!!!
    bool processed = false;
    
    int packetDataSize = timeServerPort_.parsePacket();
    if (packetDataSize && packetDataSize >= NtpPacket::PACKET_SIZE)
    {
        // Received what is probably an NTP packet. Read it in and verify
        // that it's legit.
        NtpPacket packet;
        timeServerPort_.read((char*)&packet, NtpPacket::PACKET_SIZE);
        // TODO: verify packet.

        // Populate response.
        packet.swapEndian();        
        packet.leapIndicator(0);
        packet.versionNumber(4);
        packet.mode(4);
        packet.stratum = 2; // I guess stratum 1 is too optimistic
        packet.poll = 10; // 6-10 per RFC 5905.
        packet.precision = -21; // ~0.5 microsecond precision.
        packet.rootDelay = 0; //60 * (0xFFFF / 1000); // ~60 milliseconds, TBD
        packet.rootDispersion = 0; //10 * (0xFFFF / 1000); // ~10 millisecond dispersion, TBD
        packet.referenceId[0] = 'G';
        packet.referenceId[1] = 'P';
        packet.referenceId[2] = 'S';
        packet.referenceId[3] = 0;
        packet.referenceTimestampSeconds = utc;
        packet.referenceTimestampFraction = recvFract;
        packet.originTimestampSeconds = packet.transmitTimestampSeconds;
        packet.originTimestampFraction = packet.transmitTimestampFraction;
        packet.receiveTimestampSeconds = recvSecs;
        packet.receiveTimestampFraction = recvFract;
        
        // ...and the transmit time.
        // timeSource_.now(&packet.transmitTimestampSeconds, &packet.transmitTimestampFraction);
        
        // Now transmit the response to the client.
        packet.swapEndian();
        timeServerPort_.beginPacket(timeServerPort_.remoteIP(), timeServerPort_.remotePort());
        for (int count = 0; count < NtpPacket::PACKET_SIZE; count++)
        {
            timeServerPort_.write(packet.packet()[count]);
        }
        timeServerPort_.endPacket();
        processed = true;
    } 
    
    return processed;
}