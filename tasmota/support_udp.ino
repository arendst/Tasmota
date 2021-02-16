/*
  support_udp.ino - Udp support for Tasmota

  Copyright (C) 2021  Heiko Krupp and Theo Arends

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

#ifdef USE_EMULATION

#ifndef UDP_BUFFER_SIZE
#define UDP_BUFFER_SIZE         120      // Max UDP buffer size needed for M-SEARCH message
#endif
#define UDP_MSEARCH_DEBOUNCE  300        // Don't send new response if same request within 300 ms

uint32_t  udp_last_received = 0;         // timestamp of last udp received packet
                                         // if non-zero we keep silend and don't send response
                                         // there is a very low probability that after 53 days the timestamp is
                                         // genuingly zero. This is not an issue and would result in an extra response sent.
IPAddress udp_remote_ip;                 // M-Search remote IP address
uint16_t udp_remote_port;                // M-Search remote port

bool udp_connected = false;

#ifdef ESP8266
#ifndef UDP_MAX_PACKETS
#define UDP_MAX_PACKETS   3             // we support x more packets than the current one
#endif

#include "UdpListener.h"
UdpListener<UDP_BUFFER_SIZE> UdpCtx(UDP_MAX_PACKETS);
#endif

/*********************************************************************************************\
 * UPNP/SSDP search targets
\*********************************************************************************************/

const char URN_BELKIN_DEVICE[] PROGMEM = "urn:belkin:device:**";
const char URN_BELKIN_DEVICE_CAP[] PROGMEM = "urn:Belkin:device:**";
const char UPNP_ROOTDEVICE[] PROGMEM = "upnp:rootdevice";
const char SSDPSEARCH_ALL[] PROGMEM = "ssdpsearch:all";
const char SSDP_ALL[] PROGMEM = "ssdp:all";

/*********************************************************************************************\
 * UDP support routines
\*********************************************************************************************/

bool UdpDisconnect(void)
{
  if (udp_connected) {
    // flush any outgoing packet
    PortUdp.flush();
#ifdef ESP8266
    UdpCtx.disconnect();
#endif
#ifdef USE_DEVICE_GROUPS
    // stop
    PortUdp.stop();
#else // USE_DEVICE_GROUPS
    // stop all
    WiFiUDP::stopAll();
#endif  // !USE_DEVICE_GROUPS
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPNP D_MULTICAST_DISABLED));
    udp_connected = false;
  }
  return udp_connected;
}

bool UdpConnect(void)
{
  if (!udp_connected && !TasmotaGlobal.restart_flag) {
    // Simple Service Discovery Protocol (SSDP)
#ifdef ESP8266
    UdpCtx.reset();
    if (igmp_joingroup(WiFi.localIP(), IPAddress(239,255,255,250)) == ERR_OK) { // addr 239.255.255.250
      ip_addr_t addr = IPADDR4_INIT(INADDR_ANY);
      if (UdpCtx.listen(&addr, 1900)) {         // port 1900
        // OK
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_UPNP D_MULTICAST_REJOINED));
        udp_connected = true;
      }
#endif  // ESP8266
#ifdef ESP32
    if (PortUdp.beginMulticast(WiFi.localIP(), IPAddress(239,255,255,250), 1900)) {
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_UPNP D_MULTICAST_REJOINED));
      udp_connected = true;
#endif  // ESP32
    }
    if (!udp_connected) {     // if connection failed
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_UPNP D_MULTICAST_JOIN_FAILED));
    }
  }
  return udp_connected;
}

void PollUdp(void)
{
  if (udp_connected) {
    if (TimeReached(udp_last_received + UDP_MSEARCH_DEBOUNCE)) {
      udp_last_received = 0;      // re-init timer
    }
#ifdef ESP8266
    while (UdpCtx.next()) {
      UdpPacket<UDP_BUFFER_SIZE> *packet;
      packet = UdpCtx.read();
      if (packet->len >= UDP_BUFFER_SIZE) {
        packet->len--;    // leave space for NULL terminator
      }
      packet->buf[packet->len] = 0;   // add NULL at the end of the packet
      char * packet_buffer = (char*) &packet->buf;
      int32_t len = packet->len;
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("UDP: Packet (%d)"), len);
#endif  // ESP8266
#ifdef ESP32
    while (uint32_t pack_len = PortUdp.parsePacket()) {
      char packet_buffer[UDP_BUFFER_SIZE];     // buffer to hold incoming UDP/SSDP packet

      int32_t len = PortUdp.read(packet_buffer, UDP_BUFFER_SIZE -1);
      packet_buffer[len] = 0;
      PortUdp.flush();
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("UDP: Packet (%d/%d)"), len, pack_len);
#endif  // ESP32

      // AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("\n%s"), packet_buffer);

      // Simple Service Discovery Protocol (SSDP)
      if (Settings.flag2.emulation) {
#if defined(USE_SCRIPT_HUE) || defined(USE_ZIGBEE)
        if (strstr_P(packet_buffer, PSTR("M-SEARCH")) != nullptr) {
#else
        if (TasmotaGlobal.devices_present && (strstr_P(packet_buffer, PSTR("M-SEARCH")) != nullptr)) {
#endif
          if (0 == udp_last_received) {
            udp_last_received = millis();
#ifdef ESP8266
            udp_remote_ip = packet->srcaddr;
            udp_remote_port = packet->srcport;
#else
            udp_remote_ip = PortUdp.remoteIP();
            udp_remote_port = PortUdp.remotePort();
#endif

            // AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("UDP: M-SEARCH Packet from %_I:%d\n%s"),
            //   (uint32_t)udp_remote_ip, udp_remote_port, packet_buffer);

            LowerCase(packet_buffer, packet_buffer);
            RemoveSpace(packet_buffer);

            bool udp_proccessed = false;      // make sure we process the packet only once
#ifdef USE_EMULATION_WEMO
            if (!udp_proccessed && (EMUL_WEMO == Settings.flag2.emulation)) {
              if (strstr_P(packet_buffer, URN_BELKIN_DEVICE) != nullptr) {     // type1 echo dot 2g, echo 1g's
                WemoRespondToMSearch(1);
                udp_proccessed = true;
              }
              else if ((strstr_P(packet_buffer, UPNP_ROOTDEVICE) != nullptr) ||  // type2 Echo 2g (echo & echo plus)
                      (strstr_P(packet_buffer, SSDPSEARCH_ALL) != nullptr) ||
                      (strstr_P(packet_buffer, SSDP_ALL) != nullptr)) {
                WemoRespondToMSearch(2);
                udp_proccessed = true;
              }
            }
#endif  // USE_EMULATION_WEMO

#ifdef USE_EMULATION_HUE
            if (!udp_proccessed && (EMUL_HUE == Settings.flag2.emulation)) {
              AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("UDP: HUE"));
              if ((strstr_P(packet_buffer, PSTR(":device:basic:1")) != nullptr) ||
                  (strstr_P(packet_buffer, UPNP_ROOTDEVICE) != nullptr) ||
                  (strstr_P(packet_buffer, SSDPSEARCH_ALL) != nullptr) ||
                  (strstr_P(packet_buffer, SSDP_ALL) != nullptr)) {
                HueRespondToMSearch();
                udp_proccessed = true;
              }
            }
#endif  // USE_EMULATION_HUE
          }
        }
      }
    }
    optimistic_yield(100);
  }
}

#endif  // USE_EMULATION
