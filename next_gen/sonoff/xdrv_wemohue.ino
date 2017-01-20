/*
Copyright (c) 2017 Heiko Krupp and Theo Arends.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#if defined(USE_WEMO_EMULATION) || defined(USE_HUE_EMULATION)

#define UDP_BUFFER_SIZE 200         // Max UDP buffer size needed for M-SEARCH message

boolean udpConnected = false;

char packetBuffer[UDP_BUFFER_SIZE]; // buffer to hold incoming UDP packet
IPAddress ipMulticast(239, 255, 255, 250); // Simple Service Discovery Protocol (SSDP)
uint32_t portMulticast = 1900;      // Multicast address and port

#ifdef USE_WEMO_EMULATION
/*********************************************************************************************\
 * WeMo UPNP support routines
\*********************************************************************************************/
const char WEMO_MSEARCH[] PROGMEM =
  "HTTP/1.1 200 OK\r\n"
  "CACHE-CONTROL: max-age=86400\r\n"
  "DATE: Fri, 15 Apr 2016 04:56:29 GMT\r\n"
  "EXT:\r\n"
  "LOCATION: http://{r1}:80/setup.xml\r\n"
  "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
  "01-NLS: b9200ebb-736d-4b93-bf03-835149d13983\r\n"
  "SERVER: Unspecified, UPnP/1.0, Unspecified\r\n"
  "ST: urn:Belkin:device:**\r\n"
  "USN: uuid:{r2}::urn:Belkin:device:**\r\n"
  "X-User-Agent: redsonic\r\n"
  "\r\n";

String wemo_serial()
{
  char serial[15];
  snprintf_P(serial, sizeof(serial), PSTR("201612K%07d"), ESP.getChipId());
  return String(serial);
}

String wemo_UUID()
{
  char uuid[26];
  snprintf_P(uuid, sizeof(uuid), PSTR("Socket-1_0-%s"), wemo_serial().c_str());
  return String(uuid);
}

void wemo_respondToMSearch()
{
  char message[TOPSZ], log[LOGSZ];

  if (portUDP.beginPacket(portUDP.remoteIP(), portUDP.remotePort())) {
    String response = FPSTR(WEMO_MSEARCH);
    response.replace("{r1}", WiFi.localIP().toString());
    response.replace("{r2}", wemo_UUID());
    portUDP.write(response.c_str());
    portUDP.endPacket();
    snprintf_P(message, sizeof(message), PSTR("Response sent"));
  } else {
    snprintf_P(message, sizeof(message), PSTR("Failed to send response"));
  }
  snprintf_P(log, sizeof(log), PSTR("UPnP: Wemo %s to %s:%d"),
    message, portUDP.remoteIP().toString().c_str(), portUDP.remotePort());
  addLog(LOG_LEVEL_DEBUG, log);
}
#endif  // USE_WEMO_EMULATION

#ifdef USE_HUE_EMULATION
/*********************************************************************************************\
 * Hue Bridge UPNP support routines
 * Need to send 3 response packets with varying ST and USN
\*********************************************************************************************/
const char HUE_RESPONSE[] PROGMEM =
  "HTTP/1.0 200 OK\r\n"
  "HOST: 239.255.255.250:1900\r\n"
  "CACHE-CONTROL: max-age=100\r\n"
  "EXT:\r\n"
  "LOCATION: http://{r1}:80/description.xml\r\n"
  "SERVER: FreeRTOS/7.4.2 UPnP/1.0 IpBridge/1.15.0\r\n"
  "hue-bridgeid: {r2}\r\n";
const char HUE_ST1[] PROGMEM = 
  "ST: upnp:rootdevice\r\n";
const char HUE_USN1[] PROGMEM =
  "USN: uuid:{r3}::upnp:rootdevice\r\n"
  "\r\n";

const char HUE_ST2[] PROGMEM =
  "ST: uuid:{r3}\r\n";
const char HUE_USN2[] PROGMEM =
  "USN: uuid:{r3}\r\n"
  "\r\n";

const char HUE_ST3[] PROGMEM =
  "ST: urn:schemas-upnp-org:device:basic:1\r\n";
  
String hue_bridgeid()
{
  char bridgeid[16];
  snprintf_P(bridgeid, sizeof(bridgeid), PSTR("5CCF7FFFFE%03X"), ESP.getChipId());
  return String(bridgeid);
}

String hue_UUID()
{
  char serial[36];
  snprintf_P(serial, sizeof(serial), PSTR("f6543a06-da50-11ba-8d8f-5ccf7f%03x"), ESP.getChipId());
  return String(serial);
}

void hue_respondToMSearch()
{
  char message[TOPSZ], log[LOGSZ];

  if (portUDP.beginPacket(portUDP.remoteIP(), portUDP.remotePort())) {
    String response = FPSTR(HUE_RESPONSE);
    String response_st=FPSTR(HUE_ST1);
    String response_usn=FPSTR(HUE_USN1);
    response += response_st + response_usn;
    response.replace("{r1}", WiFi.localIP().toString());
    response.replace("{r2}", hue_bridgeid());
    response.replace("{r3}", hue_UUID());
    portUDP.write(response.c_str());
    portUDP.endPacket();
    snprintf_P(message, sizeof(message), PSTR("Response1 sent"));
    addLog(LOG_LEVEL_DEBUG_MORE, response.c_str());
    
    response = FPSTR(HUE_RESPONSE);
    response_st=FPSTR(HUE_ST2);
    response_usn=FPSTR(HUE_USN2);
    response += response_st + response_usn;
    response.replace("{r1}", WiFi.localIP().toString());
    response.replace("{r2}", hue_bridgeid());
    response.replace("{r3}", hue_UUID());
    portUDP.write(response.c_str());
    portUDP.endPacket();
    snprintf_P(message, sizeof(message), PSTR("Response2 sent"));
    addLog(LOG_LEVEL_DEBUG_MORE, response.c_str());
    
    response = FPSTR(HUE_RESPONSE);
    response_st=FPSTR(HUE_ST3);
    response += response_st + response_usn;
    response.replace("{r1}", WiFi.localIP().toString());
    response.replace("{r2}", hue_bridgeid());
    response.replace("{r3}", hue_UUID());
    portUDP.write(response.c_str());
    portUDP.endPacket();
    snprintf_P(message, sizeof(message), PSTR("Response3 sent"));
    addLog(LOG_LEVEL_DEBUG_MORE, response.c_str());

  } else {
    snprintf_P(message, sizeof(message), PSTR("Failed to send response"));
  }
  snprintf_P(log, sizeof(log), PSTR("UPnP: HUE %s to %s:%d"),
    message, portUDP.remoteIP().toString().c_str(), portUDP.remotePort());
  addLog(LOG_LEVEL_DEBUG, log);
}
#endif  // USE_HUE_EMULATION

/********************************************************************************************/

#if defined(USE_WEMO_EMULATION) || defined(USE_HUE_EMULATION)
boolean UDP_Disconnect()
{
  if (udpConnected) {
    WiFiUDP::stopAll();
    addLog_P(LOG_LEVEL_DEBUG, PSTR("UPnP: Multicast disabled"));
    udpConnected = false;
  }
  return udpConnected;
}

boolean UDP_Connect()
{
  if (!udpConnected) {
    if (portUDP.beginMulticast(WiFi.localIP(), ipMulticast, portMulticast)) {
      addLog_P(LOG_LEVEL_INFO, PSTR("UPnP: Multicast (re)joined"));
      udpConnected = true;
    } else {
      addLog_P(LOG_LEVEL_INFO, PSTR("UPnP: Multicast join failed"));
      udpConnected = false;
    }
  }
  return udpConnected;
}

void pollUDP()
{
  if (udpConnected) {
    if (portUDP.parsePacket()) {
      int len = portUDP.read(packetBuffer, UDP_BUFFER_SIZE -1);
      if (len > 0) packetBuffer[len] = 0;
      String request = packetBuffer;
      addLog_P(LOG_LEVEL_DEBUG_MORE, packetBuffer);
      if (request.indexOf("M-SEARCH") >= 0) {
#ifdef USE_WEMO_EMULATION
        if (request.indexOf("urn:Belkin:device:**") > 0) {
          wemo_respondToMSearch();
        }
#endif // USE_WEMO_EMULATION
#ifdef USE_HUE_EMULATION
        if (request.indexOf("ST: urn:schemas-upnp-org:device:basic:1") > 0 ||
            request.indexOf("ST: upnp:rootdevice") > 0 ||
            request.indexOf("ST: ssdp:all") > 0) {
          hue_respondToMSearch();
        }
#endif // USE_HUE_EMULATION
      }
    }
  }
}
#endif  // USE_WEMO_EMULATION || USE_HUE_EMULATION
#endif  // USE_WEMO_EMULATION || USE_HUE_EMULATION

