/*
  xdrv_58_range_extender.ino - WiFi Range Extender for Tasmota

  Copyright (C) 2021  sillyfrog and Theo Arends

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

#ifdef USE_WIFI_RANGE_EXTENDER
/*********************************************************************************************
To use this, add the following to your user_config_override.h
#define USE_WIFI_RANGE_EXTENDER

Additionally, for the ESP8266, PIO_FRAMEWORK_ARDUINO_LWIP2_HIGHER_BANDWIDTH must be 
set in your build options.
For example, in your platfromio_tasmota_cenv.ini, you will need an entry such as:
[env:tasmota-rangeextender]
build_flags = ${common.build_flags}
              -D PIO_FRAMEWORK_ARDUINO_LWIP2_HIGHER_BANDWIDTH

For the ESP32, the arduino-esp32 library must be at least version 2, with 
CONFIG_LWIP_IP_FORWARD option set, and optionally CONFIG_LWIP_IPV4_NAPT.

If you want to support NAPT (removing the need for routes on a core router):
#define USE_WIFI_RANGE_EXTENDER_NAPT


An example full static configuration:
#define USE_WIFI_RANGE_EXTENDER
#define USE_WIFI_RANGE_EXTENDER_NAPT
#define WIFI_RGX_STATE 1
#define WIFI_RGX_NAPT 1
#define WIFI_RGX_SSID "rangeextender"
#define WIFI_RGX_PASSWORD "securepassword"
#define WIFI_RGX_IP_ADDRESS "10.99.1.1"
#define WIFI_RGX_SUBNETMASK "255.255.255.0"


A full command to enable the Range Extender, including with NAPT could be:
Backlog RgxSSID rangeextender ; RgxPassword securepassword ; RgxAddress 192.168.123.1 ; RgxSubnet 255.255.255.0; RgxState 1 ; RgxNAPT 1

\*********************************************************************************************/

#define XDRV_58 58

// Memory usage at 512: Heap from 30136 to 17632: 12504
// Memory usage at 128: Heap from 30136 to 26848: 3288
#define NAPT 128     // IP_NAPT_MAX: 512
#define NAPT_PORT 10 // IP_PORTMAP_MAX: 32

#warning **** USE_WIFI_RANGE_EXTENDER is enabled ****

#ifdef ESP8266
#if LWIP_FEATURES
// All good
#else
#error LWIP_FEATURES required, add "-D PIO_FRAMEWORK_ARDUINO_LWIP2_HIGHER_BANDWIDTH" to build_flags
#endif // LWIP_FEATURES
#endif // ESP8266

#ifdef ESP32
#ifdef CONFIG_LWIP_IP_FORWARD
// All good
#else
#error CONFIG_LWIP_IP_FORWARD not set, arduino-esp32 v2 or later required with CONFIG_LWIP_IP_FORWARD support
#endif // CONFIG_LWIP_IP_FORWARD
#ifdef USE_WIFI_RANGE_EXTENDER_NAPT
#ifdef CONFIG_LWIP_IPV4_NAPT
// All good
#else
#error CONFIG_LWIP_IPV4_NAPT not set, arduino-esp32 v2 or later required with CONFIG_LWIP_IPV4_NAPT support
#endif // IP_NAPT
#endif // CONFIG_LWIP_IPV4_NAPT
#endif // ESP32

const char kDrvRgxCommands[] PROGMEM = "Rgx|" // Prefix
                                       "State"
                                       "|" D_CMND_SSID
                                       "|" D_CMND_PASSWORD
#ifdef USE_WIFI_RANGE_EXTENDER_NAPT
                                       "|"
                                       "NAPT"
#endif // USE_WIFI_RANGE_EXTENDER_NAPT
                                       "|"
                                       "Address"
                                       "|"
                                       "Subnet";

void (*const DrvRgxCommand[])(void) PROGMEM = {
    &CmndRgxState,
    &CmndRgxSSID,
    &CmndRgxPassword,
#ifdef USE_WIFI_RANGE_EXTENDER_NAPT
    &CmndRgxNAPT,
#endif // USE_WIFI_RANGE_EXTENDER_NAPT
    &CmndRgxAddresses,
    &CmndRgxAddresses,
};

#ifdef USE_WIFI_RANGE_EXTENDER_NAPT
#ifdef ESP8266
#include <lwip/napt.h>
#endif // ESP8266
#endif // USE_WIFI_RANGE_EXTENDER_NAPT

#include <ESP8266WiFi.h>
#include <lwip/dns.h>
#ifdef ESP8266
#include <dhcpserver.h>
#endif // ESP8266
#ifdef ESP32
#include "lwip/lwip_napt.h"
#include <dhcpserver/dhcpserver.h>
#endif // ESP32

#define RGX_NOT_CONFIGURED 0
#define RGX_FORCE_CONFIGURE 1
#define RGX_CONFIGURED 2
#define RGX_CONFIG_INCOMPLETE 3
#define RGX_SETUP_NAPT 4

typedef struct
{
  uint8_t status = RGX_NOT_CONFIGURED;
  uint16_t lastlinkcount = 0;
#ifdef USE_WIFI_RANGE_EXTENDER_NAPT
  bool napt_enabled = false;
#endif // USE_WIFI_RANGE_EXTENDER_NAPT
} TRgxSettings;

TRgxSettings RgxSettings;

// Check the current configuration is complete, updating RgxSettings.status
void RgxCheckConfig(void)
{
  if (
      strlen(SettingsText(SET_RGX_SSID)) > 0 &&
      strlen(SettingsText(SET_RGX_PASSWORD)) >= 8 &&
      Settings->ipv4_rgx_address &&
      Settings->ipv4_rgx_subnetmask)
  {
    if (RgxSettings.status != RGX_FORCE_CONFIGURE)
    {
      RgxSettings.status = RGX_NOT_CONFIGURED;
    }
  }
  else
  {
    RgxSettings.status = RGX_CONFIG_INCOMPLETE;
  }
}

void CmndRgxState(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1))
  {
    if (Settings->sbflag1.range_extender != XdrvMailbox.payload)
    {
      Settings->sbflag1.range_extender = XdrvMailbox.payload;
      if (0 == XdrvMailbox.payload)
      { // Turn off
        TasmotaGlobal.restart_flag = 2;
      }
    }
  }
  ResponseCmndStateText(Settings->sbflag1.range_extender);
}

void CmndRgxAddresses(void)
{
  char network_address[22];
  ext_snprintf_P(network_address, sizeof(network_address), PSTR(" (%_I)"), (uint32_t)NetworkAddress());
  uint32_t ipv4_address;
  if (ParseIPv4(&ipv4_address, XdrvMailbox.data))
  {
    if (XdrvMailbox.command[3] == 'S') // Subnet
    {
      Settings->ipv4_rgx_subnetmask = ipv4_address;
    }
    else
    {
      Settings->ipv4_rgx_address = ipv4_address;
    }
    RgxSettings.status = RGX_FORCE_CONFIGURE;
  }
  ResponseRgxConfig();
}

void CmndRgxSSID(void)
{
  if (XdrvMailbox.data_len > 0)
  {
    SettingsUpdateText(SET_RGX_SSID, (SC_CLEAR == Shortcut()) ? "" : XdrvMailbox.data);
    RgxSettings.status = RGX_FORCE_CONFIGURE;
  }
  ResponseRgxConfig();
}

void CmndRgxPassword(void)
{
  if (XdrvMailbox.data_len > 0)
  {
    SettingsUpdateText(SET_RGX_PASSWORD, (SC_CLEAR == Shortcut()) ? "" : XdrvMailbox.data);
    RgxSettings.status = RGX_FORCE_CONFIGURE;
  }
  ResponseRgxConfig();
}

#ifdef USE_WIFI_RANGE_EXTENDER_NAPT
void CmndRgxNAPT(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1))
  {
    if (Settings->sbflag1.range_extender_napt != XdrvMailbox.payload)
    {
      Settings->sbflag1.range_extender_napt = XdrvMailbox.payload;
      if (0 == XdrvMailbox.payload)
      { // Turn off
        // ESP32 does not disable reliably, reboot to ensure a complete disable of NAPT
        // rebooting also completely frees up the used RAM
        TasmotaGlobal.restart_flag = 2;
      }
      else
      {
        RgxSettings.status = RGX_FORCE_CONFIGURE;
      }
    }
  }
  ResponseCmndStateText(Settings->sbflag1.range_extender_napt);
};
#endif // USE_WIFI_RANGE_EXTENDER_NAPT

void ResponseRgxConfig(void)
{
  RgxCheckConfig();
  Response_P(PSTR("{\"Rgx\":{\"Valid\":\"%s\",\"" D_CMND_SSID "\":\"%s\",\"" D_CMND_PASSWORD "\":\"%s\",\"" D_CMND_IPADDRESS "\":\"%_I\",\"" D_JSON_SUBNETMASK "\":\"%_I\"}"),
             (RgxSettings.status == RGX_CONFIG_INCOMPLETE) ? "false" : "true",
             EscapeJSONString(SettingsText(SET_RGX_SSID)).c_str(),
             EscapeJSONString(SettingsText(SET_RGX_PASSWORD)).c_str(),
             Settings->ipv4_rgx_address,
             Settings->ipv4_rgx_subnetmask);
}

void rngxSetup()
{
  // Check we have a complete config first
  RgxCheckConfig();
  if (RgxSettings.status == RGX_CONFIG_INCOMPLETE)
  {
    AddLog(LOG_LEVEL_DEBUG, PSTR("RGX: Range Extender config incomplete"));
    return;
  }
#ifdef ESP8266
  dhcps_set_dns(0, WiFi.dnsIP(0));
  dhcps_set_dns(1, WiFi.dnsIP(1));
#endif // ESP8266
#ifdef ESP32
  esp_err_t err;
  tcpip_adapter_dns_info_t ip_dns;

  err = tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP);
  err = tcpip_adapter_get_dns_info(TCPIP_ADAPTER_IF_STA, ESP_NETIF_DNS_MAIN, &ip_dns);
  err = tcpip_adapter_set_dns_info(TCPIP_ADAPTER_IF_AP, ESP_NETIF_DNS_MAIN, &ip_dns);
  dhcps_offer_t opt_val = OFFER_DNS; // supply a dns server via dhcps
  tcpip_adapter_dhcps_option(ESP_NETIF_OP_SET, ESP_NETIF_DOMAIN_NAME_SERVER, &opt_val, 1);
  err = tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP);
#endif // ESP32
  // WiFi.softAPConfig(EXTENDER_LOCAL_IP, EXTENDER_GATEWAY_IP, EXTENDER_SUBNET);
  WiFi.softAPConfig(Settings->ipv4_rgx_address, Settings->ipv4_rgx_address, Settings->ipv4_rgx_subnetmask);
  WiFi.softAP(SettingsText(SET_RGX_SSID), SettingsText(SET_RGX_PASSWORD));
  AddLog(LOG_LEVEL_INFO, PSTR("RGX: WiFi Extender AP Enabled with SSID: %s"), WiFi.softAPSSID().c_str());
  RgxSettings.status = RGX_SETUP_NAPT;
  RgxSettings.lastlinkcount = Wifi.link_count;
}

void rngxSetupNAPT(void)
{
  // A short delay is required for enabling NAPT to work on the ESP32, hence a dedicated
  // function called a second later
#ifdef USE_WIFI_RANGE_EXTENDER_NAPT
  if (Settings->sbflag1.range_extender_napt && !RgxSettings.napt_enabled)
  {
#ifdef ESP8266
    // ip_napt_init can only be called once, however device will reboot when disabled
    // so no need to limit calls to init separately.
    err_t ret = ip_napt_init(NAPT, NAPT_PORT);
    if (ret == ERR_OK)
    {
      AddLog(LOG_LEVEL_INFO, PSTR("RGX: NAPT initialization complete"));
      err_t ret = ip_napt_enable_no(SOFTAP_IF, 1);
      if (ret == ERR_OK)
      {
        AddLog(LOG_LEVEL_INFO, PSTR("RGX: NAPT Enabled"));
        RgxSettings.napt_enabled = true;
      }
    }
    else
    {
      AddLog(LOG_LEVEL_ERROR, PSTR("RGX: NAPT initialization failed! (%d)"), ret);
    }

#endif // ESP8266
#ifdef ESP32
    ip_napt_enable(WiFi.softAPIP(), 1);
    AddLog(LOG_LEVEL_INFO, PSTR("RGX: NAPT Enabled"));
    RgxSettings.napt_enabled = true;
#endif // ESP32
  }
  // This code path is no longer used as device will reboot to disable NAPT, maybe
  // restored when working correctly on ESP32
  /*
  else if (!Settings->sbflag1.range_extender_napt && RgxSettings.napt_enabled)
  {
#ifdef ESP8266
    err_t ret = ip_napt_enable_no(SOFTAP_IF, 0);
    if (ret == ERR_OK)
    {
      AddLog(LOG_LEVEL_INFO, "RGX: NAPT Disabled");
      RgxSettings.napt_enabled = false;
    }
#endif // ESP8266
#ifdef ESP32
    ip_napt_enable(WiFi.softAPIP(), 0);
    AddLog(LOG_LEVEL_INFO, "RGX: NAPT Disabled, reboot maybe required");
#endif // ESP32
  }
  */
#endif // USE_WIFI_RANGE_EXTENDER_NAPT
  RgxSettings.status = RGX_CONFIGURED;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv58(uint8_t function)
{
  bool result = false;

  if (FUNC_COMMAND == function)
  {
    result = DecodeCommand(kDrvRgxCommands, DrvRgxCommand);
  }
  else if (Settings->sbflag1.range_extender && !TasmotaGlobal.restart_flag)
  {
    switch (function)
    {
    case FUNC_PRE_INIT:
      break;
    case FUNC_EVERY_SECOND:
      // AddLog(LOG_LEVEL_INFO, PSTR("RGX: XXX DEBUG: Wifi.status: %d, WiFi.getMode(): %d, RgxSettings.status: %d, link_count: %d"), Wifi.status, WiFi.getMode(), RgxSettings.status, Wifi.link_count);
      if (RgxSettings.status == RGX_NOT_CONFIGURED && Wifi.status == WL_CONNECTED)
      {
        // Setup only if WiFi in STA only mode
        if (WiFi.getMode() == WIFI_STA)
        {
          // Connecting for the first time, setup WiFi
          rngxSetup();
        }
        else
        {
          RgxSettings.status = RGX_CONFIGURED;
        }
      }
      else if (RgxSettings.status == RGX_FORCE_CONFIGURE && Wifi.status == WL_CONNECTED)
      {
        rngxSetup();
      }
      else if (RgxSettings.status == RGX_SETUP_NAPT)
      {
        // Call NAPT a second later as ESP32 requires short delay
        rngxSetupNAPT();
      }
      else if (RgxSettings.status == RGX_CONFIGURED)
      {
        if (Wifi.status != WL_CONNECTED)
        {
          // No longer connected, need to setup again
          AddLog(LOG_LEVEL_INFO, PSTR("RGX: No longer connected, prepare to reconnect WiFi AP..."));
          RgxSettings.status = RGX_NOT_CONFIGURED;
        }
        else if (RgxSettings.lastlinkcount != Wifi.link_count && WiFi.getMode() != WIFI_AP_STA)
        {
          // Assume WiFi has reconnected and been reconfigured, prepare to reconnect
          AddLog(LOG_LEVEL_INFO, PSTR("RGX: Link count now: %d, WiFi.getMode(): %d, unconfigure..."), Wifi.link_count, WiFi.getMode());
          RgxSettings.status = RGX_NOT_CONFIGURED;
        }
      }
      break;
    }
  }
  return result;
}

#endif // USE_WIFI_RANGE_EXTENDER