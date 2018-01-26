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
//#ifdef  DS18X20_MAX_SENSORS
//#undef DS18X20_MAX_SENSORS
//  #define  DS18X20_MAX_SENSORS  20
//#endif


#ifdef USE_SPI
  #undef USE_SPI                              // Disable all SPI devices
#endif

#ifdef PROJECT
  #undef PROJECT
  #define PROJECT                "sonoff-lva"
#endif


#ifdef CFG_HOLDER
  #undef CFG_HOLDER
  #define CFG_HOLDER                0x20161210
#endif




#ifdef STA_SSID1
  #undef STA_SSID1
  #define STA_SSID1              "TP5-G-FL"      // [Ssid1] Wifi SSID
#endif

#ifdef STA_PASS1
  #undef STA_PASS1
  #define STA_PASS1              "bcc2813db03419d61cc968bd6b"  // [Password1] Wifi password
#endif

#ifdef STA_SSID2
  #undef STA_SSID2
  #define STA_SSID2              "Point_00"      // [Ssid2] Optional alternate AP Wifi SSID
#endif

#ifdef STA_PASS2
  #undef STA_PASS2
  #define STA_PASS2              "27061970"  // [Password2] Optional alternate AP Wifi password
#endif

#ifdef MQTT_HOST
  #undef MQTT_HOST
  #define MQTT_HOST            "192.168.1.134"          // [MqttHost]
#endif

#ifdef MQTT_PORT
  #undef MQTT_PORT
  #define MQTT_PORT            1883              // [MqttPort] MQTT port (10123 on CloudMQTT)
#endif

#ifdef MQTT_USER
  #undef MQTT_USER
  #define MQTT_USER            "esp"       // [MqttUser] Optional user
#endif

#ifdef MQTT_PASS
  #undef MQTT_PASS
  #define MQTT_PASS            "esp"       // [MqttPassword] Optional password
#endif

// на период отладки по умолчанию 300
#ifdef TELE_PERIOD
  #undef TELE_PERIOD
  #define TELE_PERIOD            60               // [TelePeriod] Telemetry (0 = disable, 10 - 3600 seconds)
#endif

#ifdef NTP_SERVER1
    #undef NTP_SERVER1
    #define NTP_SERVER1            MQTT_HOST       // [NtpServer1] Select first NTP server by name or IP address (129.250.35.250)
#endif

#ifdef NTP_SERVER2
  #undef NTP_SERVER2
  #define NTP_SERVER2            "ntp5.stratum1.ru"    // [NtpServer2] Select second NTP server by name or IP address (5.39.184.5)
#endif

#ifdef NTP_SERVER3
  #undef NTP_SERVER3
  #define NTP_SERVER3            "ntp4.stratum1.ru"  // [NtpServer3] Select third NTP server by name or IP address (93.94.224.67)
#endif

#ifdef USE_DS18x20_LEGACY
    #undef USE_DS18x20_LEGACY

#endif

  #undef USE_SHT                                // Add I2C emulating code for SHT1X sensor
  #undef USE_SHT3X                              // Add I2C code for SHT3x sensor
  #undef USE_HTU                                // Add I2C code for HTU21/SI7013/SI7020/SI7021 sensor
  #undef USE_BMP                                // Add I2C code for BMP/BME280 sensor
  #undef USE_BH1750                             // Add I2C code for BH1750 sensor
  #undef USE_VEML6070
  #undef USE_ADS1115_I2CDEV                     // Add I2C code for ADS1115 16 bit A/D converter using library i2cdevlib-Core and i2cdevlib-ADS1115 (+2k code)
  #undef USE_INA219                             // Add I2C code for INA219 Low voltage and current sensor (+1k code)
  #undef USE_IR_REMOTE                            // Send IR remote commands using library IRremoteESP8266 and ArduinoJson (+4k code, 0.3k mem)
  #undef USE_IR_HVAC                            // Support for HVAC system using IR (+2k code)
  #undef USE_IR_RECEIVE                         // Support for IR receiver (+5.5k code)
  #undef USE_WS2812                               // WS2812 Led string using library NeoPixelBus (+5k code, +1k mem) - Disable by //
  #undef USE_WS2812_CTYPE                      // WS2812 Color type (0 - RGB, 1 - GRB, 2 - RGBW, 3 - GRBW)
  #undef USE_WS2812_DMA                         // DMA supports only GPIO03 (= Serial RXD) (+1k mem). When USE_WS2812_DMA is enabled expect Exceptions on Pow
  #undef USE_MHZ19                                // Add support for MH-Z19 CO2 sensor using hardware serial interface at 9600 bps
  #undef USE_ARILUX_RF                            // Add support for Arilux RF remote controller (+0.8k code)

#define LVA  // включить изменения LVA
#define USE_DS18x20_LEGACY  // включили
#define USE_ADS1115
//#define USE_PCA9685

//#define CFG_HOLDER             0x20161210

//#ifdef STA_SSID1
//#undef STA_SSID1
//#endif
//#define STA_SSID1              "yourssid1"

// бесполезно т.к все утсройства выключены
// LVA Added



//#endif  // _USER_CONFIG_OVERRIDE_H_
