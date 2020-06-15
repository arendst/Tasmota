/*
  xdrv_82_ethernet.ino - ESP32 (PoE) ethernet support for Tasmota

  Copyright (C) 2020  Theo Arends

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

#ifdef ESP32
#ifdef USE_ETHERNET
/*********************************************************************************************\
 * Ethernet support for ESP32
\*********************************************************************************************/

#define XDRV_82           82

// Olimex ESP32-PoE
#define ETH_CLK_MODE      ETH_CLOCK_GPIO17_OUT
#define ETH_PHY_POWER     12

#include <ETH.h>

struct {
  char hostname[33];
} Eth;

void EthernetEvent(WiFiEvent_t event) {
  switch (event) {
    case SYSTEM_EVENT_ETH_START:
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ETH: " D_ATTEMPTING_CONNECTION));
      ETH.setHostname(Eth.hostname);
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      AddLog_P2(LOG_LEVEL_INFO, PSTR("ETH: " D_CONNECTED));
      break;
    case SYSTEM_EVENT_ETH_GOT_IP:
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ETH: Mac %s, IPAddress %s, Hostname %s"),
        ETH.macAddress().c_str(), ETH.localIP().toString().c_str(), Eth.hostname);
/*
      if (ETH.fullDuplex()) {
        Serial.print(", FULL_DUPLEX");
      }
      Serial.print(", ");
      Serial.print(ETH.linkSpeed());
      Serial.println("Mbps");
*/
      global_state.eth_down = 0;
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      AddLog_P2(LOG_LEVEL_INFO, PSTR("ETH: Disconnected"));
      global_state.eth_down = 1;
      break;
    case SYSTEM_EVENT_ETH_STOP:
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ETH: Stopped"));
      global_state.eth_down = 1;
      break;
    default:
      break;
  }
}

void EthernetInit(void) {
  if (!Settings.flag4.network_ethernet) { return; }

  snprintf_P(Eth.hostname, sizeof(Eth.hostname), PSTR("%s_eth"), my_hostname);
  WiFi.onEvent(EthernetEvent);
  ETH.begin();
}

IPAddress EthernetLocalIP(void) {
  return ETH.localIP();
}

char* EthernetHostname(void) {
  return Eth.hostname;
}

String EthernetMacAddress(void) {
  return ETH.macAddress();
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kEthernetCommands[] PROGMEM = "|"  // No prefix
  D_CMND_ETHERNET;

void (* const EthernetCommand[])(void) PROGMEM = {
  &CmndEthernet };

void CmndEthernet(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings.flag4.network_ethernet = XdrvMailbox.payload;
    restart_flag = 2;
  }
  ResponseCmndStateText(Settings.flag4.network_ethernet);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv82(uint8_t function) {
  bool result = false;

  switch (function) {
    case FUNC_COMMAND:
      result = DecodeCommand(kEthernetCommands, EthernetCommand);
      break;
    case FUNC_INIT:
      EthernetInit();
      break;
  }
  return result;
}

#endif  // USE_ETHERNET
#endif  // ESP32
