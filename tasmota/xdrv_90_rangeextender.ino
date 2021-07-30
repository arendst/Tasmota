/*
  xdrv_90_rangeextender.ino - WiFi Range Extender for Tasmota

To use this, add the following to your user_config_override.h
#define USE_DRV_RANGE_EXTENDER


Additionally PIO_FRAMEWORK_ARDUINO_LWIP2_HIGHER_BANDWIDTH must be set in your build options.
For example, in your platfromio_tasmota_cenv.ini, you will need an entry such as:
[env:tasmota-rangeextender]
build_flags = ${common.build_flags} 
              -D FIRMWARE_RANGE_EXTENDER
              -D PIO_FRAMEWORK_ARDUINO_LWIP2_HIGHER_BANDWIDTH

If you want to support NAPT (removing the need for routes on a core router):
#define USE_RANGE_EXTENDER_NAPT

To have NAPT enabled by default:
#define EXTENDER_NAPT_ENABLE 1

An example full static configuration:
#define USE_DRV_RANGE_EXTENDER
#define USE_RANGE_EXTENDER_NAPT
#define EXTENDER_SSID "rangeextender"
#define EXTENDER_PASSWORD "securepassword"
#define EXTENDER_LOCAL_IP IPAddress(10, 99, 1, 1)
#define EXTENDER_SUBNET IPAddress(255, 255, 255, 0)
#define EXTENDER_NAPT_ENABLE 1


A dynamic configuration is also supported. Please note, this is **NOT SAVED TO FLASH**:
Backlog RgxSSID rangeextender ; RgxPassword securepassword ;  RgxAddress 192.168.99.1 ; RgxSubnet 255.255.255.0 ; RgxNAPT ON

To have this run automatically at start up, you can do the following (this will save to flash):
Rule1 ON Power1#Boot DO Backlog RgxSSID rangeextender ; RgxPassword securepassword ;  RgxAddress 192.168.99.1 ; RgxSubnet 255.255.255.0 ; RgxNAPT ON ENDON
Rule1 ON

*/

#ifdef USE_DRV_RANGE_EXTENDER
#ifdef ESP32
#error arduino-esp32 v2 required for NAPT support - not yet a stable release
#endif // ESP32

#warning **** USE_DRV_RANGE_EXTENDER is enabled ****

#if LWIP_FEATURES
// All good
#else
#error LWIP_FEATURES required, add "-D PIO_FRAMEWORK_ARDUINO_LWIP2_HIGHER_BANDWIDTH" to build_flags
#endif

#define XDRV_90 90

#ifndef EXTENDER_SSID
#define EXTENDER_SSID ""
#endif
#ifndef EXTENDER_PASSWORD
#define EXTENDER_PASSWORD ""
#endif
#ifndef EXTENDER_LOCAL_IP
#define EXTENDER_LOCAL_IP IPAddress(192, 168, 99, 1)
#endif
#ifndef EXTENDER_SUBNET
#define EXTENDER_SUBNET IPAddress(255, 255, 255, 0)
#endif

const char kDrvRgxCommands[] PROGMEM = "Rgx|" // Prefix
                                       "SSID"
                                       "|"
                                       "Password"
#ifdef USE_RANGE_EXTENDER_NAPT
                                       "|"
                                       "NAPT"
#endif // USE_RANGE_EXTENDER_NAPT
                                       "|"
                                       "Address"
                                       "|"
                                       "Subnet";

void (*const DrvRgxCommand[])(void) PROGMEM = {
    &CmndRgxSSID,
    &CmndRgxPassword,
#ifdef USE_RANGE_EXTENDER_NAPT
    &CmndRgxNAPT,
#endif // USE_RANGE_EXTENDER_NAPT
    &CmndRgxAddresses,
    &CmndRgxAddresses,
};

#ifdef USE_RANGE_EXTENDER_NAPT
#ifdef ESP8266
#include <lwip/napt.h>
#endif // ESP8266
#endif // USE_RANGE_EXTENDER_NAPT

#include <ESP8266WiFi.h>
#include <lwip/dns.h>
#include <dhcpserver.h>

#define RGX_NOT_CONFIGURED 0
#define RGX_FORCE_CONFIGURE 1
#define RGX_CONFIGURED 2
#define RGX_CONFIG_INCOMPLETE 3

typedef struct
{
  char ssid[31] = EXTENDER_SSID;
  char password[31] = EXTENDER_PASSWORD;
  uint32_t ipv4_address = EXTENDER_LOCAL_IP;
  uint32_t ipv4_subnet = EXTENDER_SUBNET;
  uint8_t status = RGX_NOT_CONFIGURED;
#ifdef USE_RANGE_EXTENDER_NAPT
  bool napt_enabled = false;
  bool napt_inited = false;
#ifdef EXTENDER_NAPT_ENABLE
  bool use_napt = true;
#else
  bool use_napt = false;
#endif // EXTENDER_NAPT_ENABLE
#endif // USE_RANGE_EXTENDER_NAPT
} TRgxSettings;

TRgxSettings RgxSettings;

// Check the current configuration is complete, updating RgxSettings.status
void RgxCheckConfig(void)
{
  if (
      strlen(RgxSettings.ssid) > 0 &&
      strlen(RgxSettings.password) >= 8 &&
      RgxSettings.ipv4_address &&
      RgxSettings.ipv4_subnet)
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

void CmndRgxAddresses(void)
{
  char network_address[22];
  ext_snprintf_P(network_address, sizeof(network_address), PSTR(" (%_I)"), (uint32_t)NetworkAddress());
  uint32_t ipv4_address;
  if (ParseIPv4(&ipv4_address, XdrvMailbox.data))
  {
    if (XdrvMailbox.command[3] == 'S')
    {
      // Subnet
      RgxSettings.ipv4_subnet = ipv4_address;
    }
    else
    {
      RgxSettings.ipv4_address = ipv4_address;
    }
    RgxSettings.status = RGX_FORCE_CONFIGURE;
  }
  ResponseRgxConfig();
}

void CmndRgxSSID(void)
{
  if (XdrvMailbox.data_len)
  {
    if (XdrvMailbox.data_len < 2 || XdrvMailbox.data_len >= sizeof(RgxSettings.ssid))
    {
      ResponseCmndChar_P(PSTR("SSID too short/long"));
      return;
    }
  }
  strcpy(RgxSettings.ssid, XdrvMailbox.data);
  RgxSettings.status = RGX_FORCE_CONFIGURE;
  ResponseRgxConfig();
}

void CmndRgxPassword(void)
{
  if (XdrvMailbox.data_len)
  {
    if (XdrvMailbox.data_len < 8 || XdrvMailbox.data_len >= sizeof(RgxSettings.password))
    {
      ResponseCmndChar_P(PSTR("Password too short/long"));
      return;
    }
  }
  strcpy(RgxSettings.password, XdrvMailbox.data);
  RgxSettings.status = RGX_FORCE_CONFIGURE;
  ResponseRgxConfig();
}

#ifdef USE_RANGE_EXTENDER_NAPT
void CmndRgxNAPT(void)
{
  if (XdrvMailbox.data_len)
  {
    RgxSettings.use_napt = XdrvMailbox.payload;
    RgxSettings.status = RGX_FORCE_CONFIGURE;
  }
  ResponseCmndStateText(RgxSettings.use_napt);
};
#endif // USE_RANGE_EXTENDER_NAPT

void ResponseRgxConfig(void)
{
  RgxCheckConfig();
  Response_P(PSTR("{CompleteConfiguration: %s, RgxSSID:\"%s\", RgxPassword:\"%s\", RgxAddress:\"%_I\", RgxSubnet:\"%_I\"}"),
             (RgxSettings.status == RGX_CONFIG_INCOMPLETE) ? "false" : "true",
             RgxSettings.ssid,
             RgxSettings.password,
             RgxSettings.ipv4_address,
             RgxSettings.ipv4_subnet);
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
  dhcps_set_dns(0, WiFi.dnsIP(0));
  dhcps_set_dns(1, WiFi.dnsIP(1));
  // WiFi.softAPConfig(EXTENDER_LOCAL_IP, EXTENDER_GATEWAY_IP, EXTENDER_SUBNET);
  WiFi.softAPConfig(RgxSettings.ipv4_address, RgxSettings.ipv4_address, RgxSettings.ipv4_subnet);
  WiFi.softAP(RgxSettings.ssid, RgxSettings.password);
  AddLog(LOG_LEVEL_INFO, PSTR("RGX: WiFi Extender AP Enabled"));

#ifdef USE_RANGE_EXTENDER_NAPT
// Memory usage at 512: Heap from 30136 to 17632: 12504
// Memory usage at 128: Heap from 30136 to 26848: 3288
#define NAPT 128     // IP_NAPT_MAX: 512
#define NAPT_PORT 10 //  IP_PORTMAP_MAX: 32
  if (RgxSettings.use_napt && !RgxSettings.napt_enabled)
  {
    if (!RgxSettings.napt_inited)
    {
      err_t ret = ip_napt_init(NAPT, NAPT_PORT);
      if (ret == ERR_OK)
      {
        AddLog(LOG_LEVEL_INFO, PSTR("RGX: NAPT initialization complete"));
        RgxSettings.napt_inited = true;
      }
      else
      {
        AddLog(LOG_LEVEL_ERROR, PSTR("RGX: NAPT initialization failed! (%d)"), ret);
      }
    }
    if (RgxSettings.napt_inited)
    {
      err_t ret = ip_napt_enable_no(SOFTAP_IF, 1);
      if (ret == ERR_OK)
      {
        AddLog(LOG_LEVEL_INFO, PSTR("RGX: NAPT Enabled"));
        RgxSettings.napt_enabled = true;
      }
    }
  }
  else if (!RgxSettings.use_napt && RgxSettings.napt_enabled)
  {
    err_t ret = ip_napt_enable_no(SOFTAP_IF, 0);
    if (ret == ERR_OK)
    {
      AddLog(LOG_LEVEL_INFO, "RGX: NAPT Disabled");
      RgxSettings.napt_enabled = false;
    }
  }
#endif // USE_RANGE_EXTENDER_NAPT
  RgxSettings.status = RGX_CONFIGURED;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv90(uint8_t function)
{
  bool result = false;

  switch (function)
  {
  case FUNC_COMMAND:
    result = DecodeCommand(kDrvRgxCommands, DrvRgxCommand);
    break;
  case FUNC_PRE_INIT:
    break;
  case FUNC_EVERY_SECOND:
    // AddLog(LOG_LEVEL_INFO, PSTR("RGX: XXX DEBUG INFO: Wifi.status: %d, WiFi.getMode(): %d, RgxSettings.status: %d"), Wifi.status, WiFi.getMode(), RgxSettings.status);
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
    else if (RgxSettings.status == RGX_CONFIGURED)
    {
      if (Wifi.status != WL_CONNECTED)
      {
        // No longer connected, need to setup again
        AddLog(LOG_LEVEL_INFO, PSTR("RGX: No longer connected, prepare to reconnect WiFi AP..."));
        RgxSettings.status = RGX_NOT_CONFIGURED;
      }
    }
    break;
  }
  return result;
}

#endif // USE_DRV_FILE_DEMO