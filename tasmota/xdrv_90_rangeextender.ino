/*
  xdrv_90_rangeextender.ino - WiFi Range Extender for Tasmota
*/

#define USE_DRV_RANGE_EXTENDER // XXX
#ifdef USE_DRV_RANGE_EXTENDER
#warning **** USE_DRV_RANGE_EXTENDER is enabled ****

#define XDRV_90 90

#ifdef USE_RANGE_EXTENDER_NAPT
#include <lwip/napt.h>
#endif // USE_RANGE_EXTENDER_NAPT
#include <ESP8266WiFi.h>
#include <lwip/dns.h>
#include <dhcpserver.h>

uint8_t drv90_status = 0;

void rngxSetup()
{
  dhcps_set_dns(0, WiFi.dnsIP(0));
  dhcps_set_dns(1, WiFi.dnsIP(1));
  WiFi.softAPConfig(EXTENDER_LOCAL_IP, EXTENDER_GATEWAY_IP, EXTENDER_SUBNET);
  WiFi.softAP(EXTENDER_SSID, EXTENDER_PASSWORD);
  AddLog(LOG_LEVEL_INFO, "XXX, WiFi Extender Enabled");

#ifdef USE_RANGE_EXTENDER_NAPT
  if (EXTENDER_NAPT_ENABLE)
  {
// Memory usage at 512: Heap from 30136 to 17632: 12504
// Memory usage at 128: Heap from 30136 to 26848: 3288
#define NAPT 128     // IP_NAPT_MAX: 512
#define NAPT_PORT 10 //  IP_PORTMAP_MAX: 32
    err_t ret = ip_napt_init(NAPT, NAPT_PORT);
    AddLog(LOG_LEVEL_DEBUG, "XXX ip_napt_init(%d,%d): ret=%d (OK=%d)", NAPT, NAPT_PORT, (int)ret, (int)ERR_OK);
    if (ret == ERR_OK)
    {
      ret = ip_napt_enable_no(SOFTAP_IF, 1);
      if (ret == ERR_OK)
      {
        AddLog(LOG_LEVEL_INFO, "XXX Extended WiFi Network '%s'", EXTENDER_SSID);
      }
    }
    if (ret != ERR_OK)
    {
      AddLog(LOG_LEVEL_ERROR, PSTR("XXX NAPT initialization failed, already configured?"));
    }
  }
#endif // USE_RANGE_EXTENDER_NAPT
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
    // result = DecodeCommand(kDrvDemoCommands, DrvDemoCommand);
    break;
  case FUNC_PRE_INIT:
    AddLog(LOG_LEVEL_INFO, PSTR("TEST PreInit"));
    break;
  case FUNC_EVERY_SECOND:
    if (drv90_status == 0 && Wifi.status == WL_CONNECTED)
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
      }
      drv90_status = 1;
    }
    else if (drv90_status > 0)
    {
      if (Wifi.status != WL_CONNECTED)
      {
        // No longer connected, need to setup again
        AddLog(LOG_LEVEL_INFO, PSTR("RGX: No longer connected, prepare to reconnect WIFI AP..."));
        drv90_status = 0;
      }
    }
    // XXX
    AddLog(LOG_LEVEL_INFO, PSTR("RGX: DEBUG INFO: Wifi.status: %d, WiFi.getMode(): %d"), Wifi.status, WiFi.getMode());
    break;
  }
  return result;
}

#endif // USE_DRV_FILE_DEMO