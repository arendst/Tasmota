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
 *
 * Dedicated fixed Phy pins
 * GPIO17 - EMAC_CLK_OUT_180
 * GPIO19 - EMAC_TXD0(RMII)
 * GPIO21 - EMAC_TX_EN(RMII)
 * GPIO22 - EMAC_TXD1(RMII)
 * GPIO25 - EMAC_RXD0(RMII)
 * GPIO26 - EMAC_RXD1(RMII)
 * GPIO27 - EMAC_RX_CRS_DV
 *
 * {"NAME":"Olimex ESP32-PoE","GPIO":[65504,65504,65504,65504,65504,65504,0,0,5536,65504,65504,65504,65504,0,5600,0,0,0,0,5568,0,0,0,0,0,0,0,0,65504,65504,65504,65504,65504,0,0,65504],"FLAG":0,"BASE":1}
 *
\*********************************************************************************************/

#define XDRV_82           82

/*
// Olimex ESP32-PoE
#define ETH_CLKMODE       ETH_CLOCK_GPIO17_OUT
#define ETH_POWER_PIN     12

//********************************************************************************************

#ifndef ETH_ADDR
#define ETH_ADDR          0                      // esp_eth.h eth_phy_base_t:   0 = PHY0 .. 31 = PHY31
#endif

#ifndef ETH_TYPE
#define ETH_TYPE          ETH_PHY_LAN8720        // ETH.h eth_phy_type_t:       0 = ETH_PHY_LAN8720, 1 = ETH_PHY_TLK110
#endif

#ifndef ETH_CLKMODE
#define ETH_CLKMODE       ETH_CLOCK_GPIO0_IN     // esp_eth.h eth_clock_mode_t: 0 = ETH_CLOCK_GPIO0_IN, 1 = ETH_CLOCK_GPIO0_OUT, 2 = ETH_CLOCK_GPIO16_OUT, 3 = ETH_CLOCK_GPIO17_OUT
#endif
*/

#ifndef ETH_POWER_PIN
#define ETH_POWER_PIN     -1
#endif

#ifndef ETH_MDC_PIN
#define ETH_MDC_PIN       23
#endif

#ifndef ETH_MDIO_PIN
#define ETH_MDIO_PIN      18
#endif

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
      Settings.ip_address[1] = (uint32_t)ETH.gatewayIP();
      Settings.ip_address[2] = (uint32_t)ETH.subnetMask();
      Settings.ip_address[3] = (uint32_t)ETH.dnsIP();
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

  int eth_power = (PinUsed(GPIO_ETH_PHY_POWER)) ? Pin(GPIO_ETH_PHY_POWER) : ETH_POWER_PIN;
  int eth_mdc = (PinUsed(GPIO_ETH_PHY_MDC)) ? Pin(GPIO_ETH_PHY_MDC) : ETH_MDC_PIN;
  int eth_mdio = (PinUsed(GPIO_ETH_PHY_MDIO)) ? Pin(GPIO_ETH_PHY_MDIO) : ETH_MDIO_PIN;
  if (!ETH.begin(Settings.eth_address, eth_power, eth_mdc, eth_mdio, (eth_phy_type_t)Settings.eth_type, (eth_clock_mode_t)Settings.eth_clk_mode)) {
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ETH: Bad PHY type or init error"));
  };
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

#define D_CMND_ETHADDRESS "EthAddress"
#define D_CMND_ETHTYPE "EthType"
#define D_CMND_ETHCLOCKMODE "EthClockMode"

const char kEthernetCommands[] PROGMEM = "|"  // No prefix
  D_CMND_ETHERNET "|" D_CMND_ETHADDRESS "|" D_CMND_ETHTYPE "|" D_CMND_ETHCLOCKMODE;

void (* const EthernetCommand[])(void) PROGMEM = {
  &CmndEthernet, &CmndEthAddress, &CmndEthType, &CmndEthClockMode };

void CmndEthernet(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings.flag4.network_ethernet = XdrvMailbox.payload;
    restart_flag = 2;
  }
  ResponseCmndStateText(Settings.flag4.network_ethernet);
}

void CmndEthAddress(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 31)) {
    Settings.eth_address = XdrvMailbox.payload;
    restart_flag = 2;
  }
  ResponseCmndNumber(Settings.eth_address);
}

void CmndEthType(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings.eth_type = XdrvMailbox.payload;
    restart_flag = 2;
  }
  ResponseCmndNumber(Settings.eth_type);
}

void CmndEthClockMode(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
    Settings.eth_clk_mode = XdrvMailbox.payload;
    restart_flag = 2;
  }
  ResponseCmndNumber(Settings.eth_clk_mode);
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
