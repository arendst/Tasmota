/*
  xdrv_90_rangeextender.ino - WiFi Range Extender for Tasmota
*/

#define USE_DRV_RANGE_EXTENDER  // XXX
#define USE_RANGE_EXTENDER_NAPT // XXX
#ifdef USE_DRV_RANGE_EXTENDER
#warning **** USE_DRV_RANGE_EXTENDER is enabled ****

#define XDRV_90 90

const char kDrvRgxCommands[] PROGMEM = "Rgx|" // Prefix
                                       "Address"
                                       "|"
                                       "Subnet"
                                       "|"
                                       "SSID"
                                       "|"
                                       "Password"
                                       "|"
                                       "NAPT";
void (*const DrvRgxCommand[])(void) PROGMEM = {
    &CmndRgxAddresses, &CmndRgxAddresses, &CmndRgxSSID, &CmndRgxPassword, &CmndRgxNAPT};

#ifdef USE_RANGE_EXTENDER_NAPT
#include <lwip/napt.h>
#endif // USE_RANGE_EXTENDER_NAPT
#include <ESP8266WiFi.h>
#include <lwip/dns.h>
#include <dhcpserver.h>

#define RGX_NOT_CONFIGURED 0
#define RGX_FORCE_CONFIGURE 1
#define RGX_CONFIGURED 2

#define RGX_BIT_NAPT_ENABLED 1 << 0
#define RGX_BIT_USE_NAPT 1 << 1
typedef struct
{
  char ssid[33] = EXTENDER_SSID;
  char password[65] = EXTENDER_PASSWORD;
  uint32_t ipv4_address = EXTENDER_LOCAL_IP;
  uint32_t ipv4_subnet = EXTENDER_SUBNET;
  uint8_t status = RGX_NOT_CONFIGURED;
#if defined(USE_RANGE_EXTENDER_NAPT) && defined(EXTENDER_NAPT_ENABLE)
  uint8_t statebits = RGX_BIT_USE_NAPT;
#else
  uint8_t statebits = 0;
#endif // USE_RANGE_EXTENDER_NAPT
} TRgxSettings;

TRgxSettings RgxSettings;

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
  if (XdrvMailbox.data_len >= sizeof(RgxSettings.ssid))
  {
    ResponseCmndChar_P(PSTR("SSID too long"));
    return;
  }
  strcpy(RgxSettings.ssid, XdrvMailbox.data);
  RgxSettings.status = RGX_FORCE_CONFIGURE;
  // Response_P(PSTR("{SSID:\"%s\"}"), RgxSettings.ssid);
  ResponseRgxConfig();
}

#ifdef USE_RANGE_EXTENDER_NAPT
void CmndRgxNAPT(void)
{
  Serial.printf("XXX before Data len: %u Bits: %u\n", XdrvMailbox.data_len, RgxSettings.statebits);
  if (XdrvMailbox.data_len)
  {
    if (XdrvMailbox.payload)
    {
      RgxSettings.statebits |= RGX_BIT_USE_NAPT;
    }
    else
    {
      RgxSettings.statebits &= ~RGX_BIT_USE_NAPT;
    }
    RgxSettings.status = RGX_FORCE_CONFIGURE;
  }
  Serial.printf("XXX after Data len: %u Bits: %u\n", XdrvMailbox.data_len, RgxSettings.statebits);
  ResponseCmndStateText((bool)(RgxSettings.statebits & RGX_BIT_USE_NAPT));
};
#endif // USE_RANGE_EXTENDER_NAPT

void ResponseRgxConfig(void)
{
  Response_P(PSTR("{RgxSSID:\"%s\", RgxPassword:\"%s\", RgxAddress:\"%_I\", RgxSubnet:\"%_I\"}"), RgxSettings.ssid, RgxSettings.password, RgxSettings.ipv4_address, RgxSettings.ipv4_subnet);
}

// void CmndRgxAddress(void)
// {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 31))
//   {
//     // Settings->eth_address = XdrvMailbox.payload;
//     AddLog(LOG_LEVEL_INFO, "XXXX Payload: %s", XdrvMailbox.data);
//     // TasmotaGlobal.restart_flag = 2;
//   }
//   // ResponseCmndNumber(Settings->eth_address);
//   ResponseCmndDone();
// }

// void CmndRgxSSID(void){};
void CmndRgxPassword(void){};

void rngxSetup()
{
  dhcps_set_dns(0, WiFi.dnsIP(0));
  dhcps_set_dns(1, WiFi.dnsIP(1));
  // WiFi.softAPConfig(EXTENDER_LOCAL_IP, EXTENDER_GATEWAY_IP, EXTENDER_SUBNET);
  WiFi.softAPConfig(RgxSettings.ipv4_address, RgxSettings.ipv4_address, RgxSettings.ipv4_subnet);
  WiFi.softAP(RgxSettings.ssid, RgxSettings.password);
  AddLog(LOG_LEVEL_INFO, "XXX, WiFi Extender Enabled");

#ifdef USE_RANGE_EXTENDER_NAPT
// Memory usage at 512: Heap from 30136 to 17632: 12504
// Memory usage at 128: Heap from 30136 to 26848: 3288
#define NAPT 128     // IP_NAPT_MAX: 512
#define NAPT_PORT 10 //  IP_PORTMAP_MAX: 32
  Serial.printf("XXX Bits: %u: and use: %u  and enabled: %u, value: %u, %u\n", RgxSettings.statebits, (RgxSettings.statebits & RGX_BIT_USE_NAPT), (RgxSettings.statebits & RGX_BIT_NAPT_ENABLED), RGX_BIT_NAPT_ENABLED, RGX_BIT_USE_NAPT);
  if ((RgxSettings.statebits & RGX_BIT_USE_NAPT) && !(RgxSettings.statebits & RGX_BIT_NAPT_ENABLED))
  {
    err_t ret = ip_napt_init(NAPT, NAPT_PORT);
    AddLog(LOG_LEVEL_DEBUG, "XXX ip_napt_init(%d,%d): ret=%d (OK=%d)", NAPT, NAPT_PORT, (int)ret, (int)ERR_OK);
    if (ret == ERR_OK)
    {
      ret = ip_napt_enable_no(SOFTAP_IF, 1);
      if (ret == ERR_OK)
      {
        AddLog(LOG_LEVEL_INFO, "XXX Extended WiFi Network '%s'", EXTENDER_SSID);
        RgxSettings.statebits |= RGX_BIT_NAPT_ENABLED;
      }
    }
    if (ret != ERR_OK)
    {
      AddLog(LOG_LEVEL_ERROR, PSTR("XXX NAPT initialization failed, already configured?"));
    }
  }
  else if (!(RgxSettings.statebits & RGX_BIT_USE_NAPT) && (RgxSettings.statebits & RGX_BIT_NAPT_ENABLED))
  {
    err_t ret = ip_napt_enable_no(SOFTAP_IF, 1);
    if (ret == ERR_OK)
    {
      AddLog(LOG_LEVEL_INFO, "XXX NAPT Disabled");
      RgxSettings.statebits &= ~RGX_BIT_NAPT_ENABLED;
    }
  }
#endif // USE_RANGE_EXTENDER_NAPT
  Serial.printf("XXX after Bits: %u: and use: %u  and enabled: %u, value: %ui\n", RgxSettings.statebits, (RgxSettings.statebits & RGX_BIT_USE_NAPT), (RgxSettings.statebits & RGX_BIT_NAPT_ENABLED), RGX_BIT_NAPT_ENABLED);
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
    AddLog(LOG_LEVEL_INFO, PSTR("TEST Command"));
    result = DecodeCommand(kDrvRgxCommands, DrvRgxCommand);
    break;
  case FUNC_PRE_INIT:
    AddLog(LOG_LEVEL_INFO, PSTR("TEST PreInit"));
    break;
  case FUNC_EVERY_SECOND:
    uint8_t xx = 7;
    Serial.printf("================== XX: %u\n", xx);
    xx |= RGX_BIT_USE_NAPT;
    Serial.printf("================== XX: |= %u\n", xx);

    Serial.printf("================== XX: ~%u\n", ~(uint8_t)RGX_BIT_USE_NAPT);

    xx &= ~(uint8_t)RGX_BIT_USE_NAPT;
    Serial.printf("================== XX: &= ~%u\n", xx);

    if (RgxSettings.status == RGX_NOT_CONFIGURED && Wifi.status == WL_CONNECTED)
    {
      // Setup only if WiFi in STA only mode
      if (WiFi.getMode() == WIFI_STA)
      {
        // Connecting for the first time, setup WiFi
        AddLog(LOG_LEVEL_INFO, PSTR("RGX: Setup WIFI AP..."));
        rngxSetup();
      }
      else
      {
        AddLog(LOG_LEVEL_INFO, PSTR("RGX: NOT doing Setup WIFI AP..."));
        RgxSettings.status = RGX_CONFIGURED;
      }
    }
    else if (RgxSettings.status == RGX_FORCE_CONFIGURE)
    {
      rngxSetup();
    }
    else if (RgxSettings.status == RGX_CONFIGURED)
    {
      if (Wifi.status != WL_CONNECTED)
      {
        // No longer connected, need to setup again
        AddLog(LOG_LEVEL_INFO, PSTR("RGX: No longer connected, prepare to reconnect WIFI AP..."));
        RgxSettings.status = RGX_NOT_CONFIGURED;
      }
    }
    // XXX
    AddLog(LOG_LEVEL_INFO, PSTR("RGX: DEBUG INFO: Wifi.status: %d, WiFi.getMode(): %d"), Wifi.status, WiFi.getMode());
    break;
  }
  return result;
}

#endif // USE_DRV_FILE_DEMO