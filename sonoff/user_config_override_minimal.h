/*****************************************************************************************************\
 * User specific configuration parameters to override user_config.h
 * 
 * ATTENTION: - Changes to most PARAMETER defines will only override flash settings if you change
 *              define CFG_HOLDER. 
 *            - Expect compiler warnings when no ifdef/undef/endif sequence is used.
 *            - You still need to update user_config.h for major defines MODULE and USE_MQTT_TLS.
 *            - Changing MODULE defines are not being tested for validity as they are in user_config.h.
 *            - Most parameters can be changed online using commands via MQTT, WebConsole or serial.
 *            - So I see no use in this but anyway, your on your own.
\*****************************************************************************************************/

// Examples
//#ifdef CFG_HOLDER
//#undef CFG_HOLDER
//#endif
//#define CFG_HOLDER             0x20161210

//#ifdef STA_SSID1
//#undef STA_SSID1
//#endif
//#define STA_SSID1              "yourssid1"

#ifdef BE_MINIMAL
#ifdef USE_MQTT_TLS
#undef USE_MQTT_TLS                         // Disable TLS support won't work as the MQTTHost is not set
#endif
#ifdef USE_DISCOVERY
#undef USE_DISCOVERY                        // Disable Discovery services for both MQTT and web server
#endif
#ifdef USE_DOMOTICZ
#undef USE_DOMOTICZ                         // Disable Domoticz
#endif
//#ifdef USE_WEBSERVER
//#undef USE_WEBSERVER                        // Disable Webserver
//#endif
#ifdef USE_EMULATION
#undef USE_EMULATION                        // Disable Wemo or Hue emulation
#endif
#ifdef USE_DS18x20
#undef USE_DS18x20                          // Disable DS18x20 sensor
#endif
#ifdef USE_I2C
#undef USE_I2C                              // Disable all I2C sensors
#endif
#ifdef USE_WS2812
#undef USE_WS2812                           // Disable WS2812 Led string
#endif
#ifdef USE_DS18B20
#undef USE_DS18B20                          // Disable internal DS18B20 sensor
#endif
#ifdef USE_DHT
#undef USE_DHT                              // Disable internal DHT sensor
#endif
#ifdef USE_IR_REMOTE
#undef USE_IR_REMOTE                        // Disable IR driver
#endif
#ifdef DEBUG_THEO
#undef DEBUG_THEO                           // Disable debug code
#endif
#endif  // BE_MINIMAL

