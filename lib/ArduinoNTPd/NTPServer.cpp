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
  // millisecs is millis() at the time of the last iTOW reception, where iTOW%1000 == 0
  uint32_t refMillis = millis()-millisecs;
  if (refMillis>999){
    utc++;
    refMillis = refMillis%1000;
  }

  bool processed = false;
  
  int packetDataSize = timeServerPort_.parsePacket();
  if (packetDataSize && packetDataSize >= NtpPacket::PACKET_SIZE)
  {
      // We need the time we've received the packet in our response.
      uint32_t recvSecs =  utc + NTP_TIMESTAMP_DIFF;

      uint64_t recvFract64 = refMillis;
      recvFract64 <<= 32;
      recvFract64 /= 1000;
      uint32_t recvFract = recvFract64 & 0xffffffff;
      // is equal to:
      // uint32_t recvFract = (double)(refMillis)/0.00000023283064365386963;

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
      packet.stratum = 1; // >1 will lead to misinterpretation of refId
      packet.poll = 10; // 6-10 per RFC 5905.
      packet.precision = -21; // ~0.5 microsecond precision.
      packet.rootDelay = 100 * (0xFFFF / 1000); //~100 milliseconds
      packet.rootDispersion = 50 * (0xFFFF / 1000);; //~50 millisecond dispersion
      packet.referenceId[0] = 'G';
      packet.referenceId[1] = 'P';
      packet.referenceId[2] = 'S';
      packet.referenceId[3] = 0;
      packet.referenceTimestampSeconds = recvSecs;
      packet.referenceTimestampFraction = 0;  // the "click" of the GPS
      packet.originTimestampSeconds = packet.transmitTimestampSeconds;
      packet.originTimestampFraction = packet.transmitTimestampFraction;
      packet.receiveTimestampSeconds = recvSecs;
      packet.receiveTimestampFraction = recvFract;
      
      // ...and the transmit time.
      // the latency has been between 135 and 175 microseconds in internal testing, so we harcode 150 
      uint32_t transFract = recvFract+(150*(10^3)/(2^32)); // microsec/((10^3)/(2^32))
      if (recvFract>transFract){
        recvSecs++; //overflow
      }
      packet.transmitTimestampSeconds = recvSecs;
      packet.transmitTimestampFraction = transFract; 
      
      // Now transmit the response to the client.
      packet.swapEndian();

      timeServerPort_.beginPacket(timeServerPort_.remoteIP(), timeServerPort_.remotePort());
      timeServerPort_.write(packet.packet(), NtpPacket::PACKET_SIZE);
      timeServerPort_.endPacket();

      processed = true;
  } 
  
  return processed;
}