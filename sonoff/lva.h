/*
lva.h - lva configuration overrides  for Sonoff-Tasmota

*/

//#ifdef _USER_CONFIG_OVERRIDE_H_
//#define _USER_CONFIG_OVERRIDE_H_

/*****************************************************************************************************\
 * ATTENTION: - Changes to most PARAMETER defines will only override flash settings if you change
 *              define CFG_HOLDER.
 *            - Expect compiler warnings when no ifdef/undef/endif sequence is used.
 *            - You still need to update user_config.h for major defines MODULE and USE_MQTT_TLS.
 *            - Changing MODULE defines are not being tested for validity as they are in user_config.h.
 *            - Most parameters can be changed online using commands via MQTT, WebConsole or serial.
 *            - So I see no use in this but anyway, your on your own.
\*****************************************************************************************************/

#define LVA_DEBUG
// Examples
//не рабатает
#ifdef  DS18X20_MAX_SENSORS
  //#undef DS18X20_MAX_SENSORS
  #define  DS18X20_MAX_SENSORS  20
#endif
//#define CFG_HOLDER             0x20161210

//#ifdef STA_SSID1
//#undef STA_SSID1
//#endif
//#define STA_SSID1              "yourssid1"

// бесполезно т.к все утсройства выключены
// LVA Added
#ifdef USE_SPI
  #undef USE_SPI                              // Disable all SPI devices
#endif



//#endif  // _USER_CONFIG_OVERRIDE_H_
