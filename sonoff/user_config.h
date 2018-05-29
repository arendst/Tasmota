/*
  user_config.h - user specific configuration for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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

#ifndef _USER_CONFIG_H_
#define _USER_CONFIG_H_

/*********************************************************************************************\
 * This file consists of TWO sections.
 *
 * SECTION 1:
 * The first section contains PARAMETERS overriding flash settings if define CFG_HOLDER is CHANGED.
 * All parameters can be persistent changed online using commands via MQTT, WebConsole or Serial.
 *   Corresponding MQTT/Serial/Console commands are shown in [brackets]
 *
 * SECTION 2:
 * The second section contains Enabled and Disabled FEATURES allowing different program size.
 *   Changes in this section do NOT need a change of define CFG_HOLDER.
 *
 * ATTENTION:
 * Users are advised to use the user_config_override.h file for most changes.
\*********************************************************************************************/

//#define USE_CONFIG_OVERRIDE                      // Uncomment to use user_config_override.h file. See README.md

/*********************************************************************************************\
 * SECTION 1
 * - After initial load any change here only take effect if CFG_HOLDER is changed too
\*********************************************************************************************/

// -- Master parameter control --------------------
#define CFG_HOLDER             0x20161209        // [Reset 1] Change this value to load SECTION1 configuration parameters to flash

// -- Project -------------------------------------
#define PROJECT                "sonoff"          // PROJECT is used as the default topic delimiter
//#define MODULE                 SONOFF_BASIC      // [Module] Select default model from sonoff_template.h (Should not be changed)

#define SAVE_DATA              1                 // [SaveData] Save changed parameters to Flash (0 = disable, 1 - 3600 seconds)
#define SAVE_STATE             1                 // [SetOption0] Save changed power state to Flash (0 = disable, 1 = enable)

// -- Wifi ----------------------------------------
#define WIFI_IP_ADDRESS        "0.0.0.0"         // [IpAddress1] Set to 0.0.0.0 for using DHCP or IP address
#define WIFI_GATEWAY           "192.168.1.1"   // [IpAddress2] If not using DHCP set Gateway IP address
#define WIFI_SUBNETMASK        "255.255.255.0"   // [IpAddress3] If not using DHCP set Network mask
#define WIFI_DNS               "192.168.1.1"    // [IpAddress4] If not using DHCP set DNS IP address (might be equal to WIFI_GATEWAY)

#define STA_SSID1              ""                // [Ssid1] Wifi SSID
#define STA_PASS1              ""                // [Password1] Wifi password
#define STA_SSID2              ""                // [Ssid2] Optional alternate AP Wifi SSID
#define STA_PASS2              ""                // [Password2] Optional alternate AP Wifi password
#define WIFI_CONFIG_TOOL       WIFI_MANAGER      // [WifiConfig] Default tool if wifi fails to connect
                                                 //   (WIFI_RESTART, WIFI_SMARTCONFIG, WIFI_MANAGER, WIFI_WPSCONFIG, WIFI_RETRY, WIFI_WAIT)

// -- Syslog --------------------------------------
#define SYS_LOG_HOST           ""                // [LogHost] (Linux) syslog host
#define SYS_LOG_PORT           514               // [LogPort] default syslog UDP port
#define SYS_LOG_LEVEL          LOG_LEVEL_NONE    // [SysLog] (LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE)
#define SERIAL_LOG_LEVEL       LOG_LEVEL_INFO    // [SerialLog] (LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE)
#define WEB_LOG_LEVEL          LOG_LEVEL_INFO    // [WebLog] (LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE)

// -- Ota -----------------------------------------
#define OTA_URL                "http://sonoff.maddox.co.uk/tasmota/sonoff.bin"  // [OtaUrl]

// -- MQTT ----------------------------------------
#define MQTT_USE               1                 // [SetOption3] Select default MQTT use (0 = Off, 1 = On)

#define MQTT_HOST              ""                // [MqttHost]
#define MQTT_FINGERPRINT1      "A5 02 FF 13 99 9F 8B 39 8E F1 83 4F 11 23 65 0B 32 36 FC 07"  // [MqttFingerprint1]
#define MQTT_FINGERPRINT2      "A5 02 FF 13 99 9F 8B 39 8E F1 83 4F 11 23 65 0B 32 36 FC 07"  // [MqttFingerprint2]
#define MQTT_PORT              1883              // [MqttPort] MQTT port (10123 on CloudMQTT)
#define MQTT_USER              "DVES_USER"       // [MqttUser] MQTT user
#define MQTT_PASS              "DVES_PASS"       // [MqttPassword] MQTT password

#define MQTT_BUTTON_RETAIN     0                 // [ButtonRetain] Button may send retain flag (0 = off, 1 = on)
#define MQTT_POWER_RETAIN      0                 // [PowerRetain] Power status message may send retain flag (0 = off, 1 = on)
#define MQTT_SWITCH_RETAIN     0                 // [SwitchRetain] Switch may send retain flag (0 = off, 1 = on)

#define MQTT_STATUS_OFF        "OFF"             // [StateText1] Command or Status result when turned off (needs to be a string like "0" or "Off")
#define MQTT_STATUS_ON         "ON"              // [StateText2] Command or Status result when turned on (needs to be a string like "1" or "On")
#define MQTT_CMND_TOGGLE       "TOGGLE"          // [StateText3] Command to send when toggling (needs to be a string like "2" or "Toggle")
#define MQTT_CMND_HOLD         "HOLD"            // [StateText4] Command to send when button is kept down for over KEY_HOLD_TIME * 0.1 seconds (needs to be a string like "HOLD")

// -- MQTT topics ---------------------------------
  // Example "tasmota/bedroom/%topic%/%prefix%/" up to 80 characers
#define MQTT_FULLTOPIC         "%prefix%/%topic%/" // [FullTopic] Subscribe and Publish full topic name - Legacy topic

// %prefix% token options
#define SUB_PREFIX             "cmnd"            // [Prefix1] Sonoff devices subscribe to %prefix%/%topic% being SUB_PREFIX/MQTT_TOPIC and SUB_PREFIX/MQTT_GRPTOPIC
#define PUB_PREFIX             "stat"            // [Prefix2] Sonoff devices publish to %prefix%/%topic% being PUB_PREFIX/MQTT_TOPIC
#define PUB_PREFIX2            "tele"            // [Prefix3] Sonoff devices publish telemetry data to %prefix%/%topic% being PUB_PREFIX2/MQTT_TOPIC/UPTIME, POWER and TIME
                                                 //   May be named the same as PUB_PREFIX
// %topic% token options (also ButtonTopic and SwitchTopic)
#define MQTT_TOPIC             PROJECT           // [Topic] (unique) MQTT device topic
#define MQTT_GRPTOPIC          "sonoffs"         // [GroupTopic] MQTT Group topic
#define MQTT_CLIENT_ID         "DVES_%06X"       // [MqttClient] Also fall back topic using Chip Id = last 6 characters of MAC address

// -- MQTT - Telemetry ----------------------------
#define TELE_PERIOD            300               // [TelePeriod] Telemetry (0 = disable, 10 - 3600 seconds)

// -- MQTT - Domoticz -----------------------------
#define DOMOTICZ_UPDATE_TIMER  0                 // [DomoticzUpdateTimer] Send relay status (0 = disable, 1 - 3600 seconds)

// -- MQTT - Home Assistant Discovery -------------
#define HOME_ASSISTANT_DISCOVERY_ENABLE   0      // [SetOption19] Home Assistant Discovery (0 = Disable, 1 = Enable)

// -- HTTP ----------------------------------------
#define WEB_SERVER             2                 // [WebServer] Web server (0 = Off, 1 = Start as User, 2 = Start as Admin)
#define WEB_PASSWORD           ""                // [WebPassword] Web server Admin mode Password for WEB_USERNAME (empty string = Disable)
#define FRIENDLY_NAME          "Sonoff"          // [FriendlyName] Friendlyname up to 32 characters used by webpages and Alexa
#define EMULATION              EMUL_NONE         // [Emulation] Select Belkin WeMo (single relay/light) or Hue Bridge emulation (multi relay/light) (EMUL_NONE, EMUL_WEMO or EMUL_HUE)

// -- Time - Up to three NTP servers in your region
#define NTP_SERVER1            "pool.ntp.org"       // [NtpServer1] Select first NTP server by name or IP address (129.250.35.250)
#define NTP_SERVER2            "nl.pool.ntp.org"    // [NtpServer2] Select second NTP server by name or IP address (5.39.184.5)
#define NTP_SERVER3            "0.nl.pool.ntp.org"  // [NtpServer3] Select third NTP server by name or IP address (93.94.224.67)

// -- Time - Start Daylight Saving Time and timezone offset from UTC in minutes
#define TIME_DST_HEMISPHERE    North              // [TimeDst] Hemisphere (0 or North, 1 or South)
#define TIME_DST_WEEK          Last               // Week of month (0 or Last, 1 or First, 2 or Second, 3 or Third, 4 or Fourth)
#define TIME_DST_DAY           Sun                // Day of week (1 or Sun, 2 or Mon, 3 or Tue, 4 or Wed, 5 or Thu, 6 or Fri, 7 or Sat)
#define TIME_DST_MONTH         Mar                // Month (1 or Jan, 2 or Feb, 3 or Mar, 4 or Apr, 5 or May, 6 or Jun, 7 or Jul, 8 or Aug, 9 or Sep, 10 or Oct, 11 or Nov, 12 or Dec)
#define TIME_DST_HOUR          2                  // Hour (0 to 23)
#define TIME_DST_OFFSET        +120               // Offset from UTC in minutes (-780 to +780)

// -- Time - Start Standard Time and timezone offset from UTC in minutes
#define TIME_STD_HEMISPHERE    North              // [TimeStd] Hemisphere (0 or North, 1 or South)
#define TIME_STD_WEEK          Last               // Week of month (0 or Last, 1 or First, 2 or Second, 3 or Third, 4 or Fourth)
#define TIME_STD_DAY           Sun                // Day of week (1 or Sun, 2 or Mon, 3 or Tue, 4 or Wed, 5 or Thu, 6 or Fri, 7 or Sat)
#define TIME_STD_MONTH         Oct                // Month (1 or Jan, 2 or Feb, 3 or Mar, 4 or Apr, 5 or May, 6 or Jun, 7 or Jul, 8 or Aug, 9 or Sep, 10 or Oct, 11 or Nov, 12 or Dec)
#define TIME_STD_HOUR          3                  // Hour (0 to 23)
#define TIME_STD_OFFSET        +60                // Offset from UTC in minutes (-780 to +780)

// -- Location ------------------------------------
#define LATITUDE               48.858360         // [Latitude] Your location to be used with sunrise and sunset
#define LONGITUDE              2.294442          // [Longitude] Your location to be used with sunrise and sunset

// -- Application ---------------------------------
#define APP_TIMEZONE           1                 // [Timezone] +1 hour (Amsterdam) (-13 .. 14 = hours from UTC, 99 = use TIME_DST/TIME_STD)
#define APP_LEDSTATE           LED_POWER         // [LedState] Function of led
                                                 //   (LED_OFF, LED_POWER, LED_MQTTSUB, LED_POWER_MQTTSUB, LED_MQTTPUB, LED_POWER_MQTTPUB, LED_MQTT, LED_POWER_MQTT)
#define APP_PULSETIME          0                 // [PulseTime] Time in 0.1 Sec to turn off power for relay 1 (0 = disabled)
#define APP_POWERON_STATE      POWER_ALL_SAVED   // [PowerOnState] Power On Relay state
                                                 //   (POWER_ALL_OFF, POWER_ALL_ON, POWER_ALL_SAVED_TOGGLE, POWER_ALL_SAVED, POWER_ALL_ALWAYS_ON, POWER_ALL_OFF_PULSETIME_ON)
#define APP_BLINKTIME          10                // [BlinkTime] Time in 0.1 Sec to blink/toggle power for relay 1
#define APP_BLINKCOUNT         10                // [BlinkCount] Number of blinks (0 = 32000)
#define APP_SLEEP              0                 // [Sleep] Sleep time to lower energy consumption (0 = Off, 1 - 250 mSec)

#define KEY_HOLD_TIME          40                // [SetOption32] Number of 0.1 seconds to hold Button or external Pushbutton before sending HOLD message
#define SWITCH_MODE            TOGGLE            // [SwitchMode] TOGGLE, FOLLOW, FOLLOW_INV, PUSHBUTTON, PUSHBUTTON_INV, PUSHBUTTONHOLD, PUSHBUTTONHOLD_INV, PUSHBUTTON_TOGGLE (the wall switch state)
#define WS2812_LEDS            30                // [Pixels] Number of WS2812 LEDs to start with (max is 512)

#define TEMP_CONVERSION        0                 // [SetOption8] Return temperature in (0 = Celsius or 1 = Fahrenheit)
#define TEMP_RESOLUTION        1                 // [TempRes] Maximum number of decimals (0 - 3) showing sensor Temperature
#define HUMIDITY_RESOLUTION    1                 // [HumRes] Maximum number of decimals (0 - 3) showing sensor Humidity
#define PRESSURE_RESOLUTION    1                 // [PressRes] Maximum number of decimals (0 - 3) showing sensor Pressure
#define ENERGY_RESOLUTION      3                 // [EnergyRes] Maximum number of decimals (0 - 5) showing energy usage in kWh

/*********************************************************************************************\
 * END OF SECTION 1
 *
 * SECTION 2
 * - Enable a feature by removing both // in front of it
 * - Disable a feature by preceding it with //
\*********************************************************************************************/

//#define USE_ARDUINO_OTA                          // Add optional support for Arduino OTA (+4k5 code)

// -- Localization --------------------------------
  // If non selected the default en-GB will be used
//#define MY_LANGUAGE            bg-BG           // Bulgarian in Bulgaria
//#define MY_LANGUAGE            cs-CZ           // Czech in Czech
//#define MY_LANGUAGE            de-DE           // German in Germany
//#define MY_LANGUAGE            el-GR           // Greek in Greece
//#define MY_LANGUAGE            en-GB           // English in Great Britain. Enabled by Default
//#define MY_LANGUAGE            es-AR           // Spanish in Argentina
//#define MY_LANGUAGE            fr-FR           // French in France
//#define MY_LANGUAGE            hu-HU           // Hungarian in Hungary
//#define MY_LANGUAGE            it-IT           // Italian in Italy
//#define MY_LANGUAGE            nl-NL           // Dutch in the Netherlands
//#define MY_LANGUAGE            pl-PL           // Polish in Poland
//#define MY_LANGUAGE            pt-BR           // Portuguese in Brazil
//#define MY_LANGUAGE            pt-PT           // Portuguese in Portugal
//#define MY_LANGUAGE            ru-RU           // Russian in Russia
//#define MY_LANGUAGE            uk-UK           // Ukrainian in Ukrain
//#define MY_LANGUAGE            zh-CN           // Chinese (Simplified) in China
//#define MY_LANGUAGE            zh-TW           // Chinese (Traditional) in Taiwan

/*-------------------------------------------------------------------------------------------*\
 * Select ONE of possible three MQTT library types below
\*-------------------------------------------------------------------------------------------*/
  // Default MQTT driver for both non-TLS and TLS connections. Blocks network if MQTT server is unavailable.
//#define MQTT_LIBRARY_TYPE      MQTT_PUBSUBCLIENT   // Use PubSubClient library
  // Alternative MQTT driver does not block network when MQTT server is unavailable. No TLS support
#define MQTT_LIBRARY_TYPE      MQTT_TASMOTAMQTT    // Use TasmotaMqtt library (+4k4 code, +4k mem) - non-TLS only
  // Alternative MQTT driver does not block network when MQTT server is unavailable. No TLS support
//#define MQTT_LIBRARY_TYPE      MQTT_ESPMQTTARDUINO // Use (patched) esp-mqtt-arduino library (+4k8 code, +4k mem) - non-TLS only

// -- MQTT ----------------------------------------
#define MQTT_TELE_RETAIN     0                   // Tele messages may send retain flag (0 = off, 1 = on)

// -- MQTT - Domoticz -----------------------------
#define USE_DOMOTICZ                             // Enable Domoticz (+6k code, +0.3k mem)
  #define DOMOTICZ_IN_TOPIC    "domoticz/in"     // Domoticz Input Topic
  #define DOMOTICZ_OUT_TOPIC   "domoticz/out"    // Domoticz Output Topic

// -- MQTT - Home Assistant Discovery -------------
//#define USE_HOME_ASSISTANT                       // Enable Home Assistant Discovery Support (+2k code)
  #define HOME_ASSISTANT_DISCOVERY_PREFIX "homeassistant"  // Home Assistant discovery prefix

// -- MQTT - TLS ----------------------------------
  // !!! TLS uses a LOT OF MEMORY so be careful to enable other options at the same time !!!
//#define USE_MQTT_TLS                             // Use TLS for MQTT connection (+53k code, +15k mem)

// -- KNX IP Protocol -----------------------------
//#define USE_KNX                                  // Enable KNX IP Protocol Support (+23k code, +3k3 mem)

// -- HTTP ----------------------------------------
#define USE_WEBSERVER                            // Enable web server and wifi manager (+66k code, +8k mem)
  #define WEB_PORT             80                // Web server Port for User and Admin mode
  #define WEB_USERNAME         "admin"           // Web server Admin mode user name
  #define USE_EMULATION                          // Enable Belkin WeMo and Hue Bridge emulation for Alexa (+16k code, +2k mem)

// -- mDNS ----------------------------------------
//#define USE_DISCOVERY                            // Enable mDNS for the following services (+8k code, +0.3k mem)
  #define WEBSERVER_ADVERTISE                    // Provide access to webserver by name <Hostname>.local/
  #define MQTT_HOST_DISCOVERY                    // Find MQTT host server (overrides MQTT_HOST if found)

// -- Time ----------------------------------------
#define USE_TIMERS                               // Add support for up to 16 timers (+2k2 code)
  #define USE_TIMERS_WEB                         // Add timer webpage support (+4k5 code)
  #define USE_SUNRISE                            // Add support for Sunrise and sunset tools (+16k)
    #define SUNRISE_DAWN_ANGLE DAWN_NORMAL       // Select desired Dawn Angle from (DAWN_NORMAL, DAWN_CIVIL, DAWN_NAUTIC, DAWN_ASTRONOMIC)

// -- Rules ---------------------------------------
#define USE_RULES                                // Add support for rules (+4k4 code)

// -- Internal Analog input -----------------------
#define USE_ADC_VCC                              // Display Vcc in Power status. Disable for use as Analog input on selected devices

// -- One wire sensors ----------------------------
                                                 // WARNING: Select none for default one DS18B20 sensor or enable one of the following two options for multiple sensors
//#define USE_DS18x20                              // Optional for more than one DS18x20 sensors with id sort, single scan and read retry (+1k3 code)
//#define USE_DS18x20_LEGACY                       // Optional for more than one DS18x20 sensors with dynamic scan using library OneWire (+1k5 code)

// -- I2C sensors ---------------------------------
#define USE_I2C                                  // I2C using library wire (+10k code, 0k2 mem, 124 iram)
#ifdef USE_I2C
  #define USE_SHT                                // Add I2C emulating code for SHT1X sensor (+1k4 code)
  #define USE_SHT3X                              // Add I2C code for SHT3x or SHTC3 sensor (+0k7 code)
  #define USE_HTU                                // Add I2C code for HTU21/SI7013/SI7020/SI7021 sensor (+1k5 code)
  #define USE_BMP                                // Add I2C code for BMP085/BMP180/BMP280/BME280 sensor (+4k code)
//    #define USE_BME680                           // Add additional support for BME680 sensor using Adafruit Sensor and BME680 libraries (+6k code)
  #define USE_SGP30                              // Add I2C code for SGP30 sensor (+1k1 code)
  #define USE_BH1750                             // Add I2C code for BH1750 sensor (+0k5 code)
//  #define USE_VEML6070                           // Add I2C code for VEML6070 sensor (+0k5 code)
//  #define USE_TSL2561                            // Add I2C code for TSL2561 sensor using library Joba_Tsl2561 (+2k3 code)
//  #define USE_SI1145                             // Add I2C code for SI1145/46/47 sensor (+1k code)
//  #define USE_ADS1115                            // Add I2C code for ADS1115 16 bit A/D converter based on Adafruit ADS1x15 library (no library needed) (+0k7 code)
//  #define USE_ADS1115_I2CDEV                     // Add I2C code for ADS1115 16 bit A/D converter using library i2cdevlib-Core and i2cdevlib-ADS1115 (+2k code)
//  #define USE_INA219                             // Add I2C code for INA219 Low voltage and current sensor (+1k code)
//  #define USE_MGS                                // Add I2C code for Xadow and Grove Mutichannel Gas sensor using library Multichannel_Gas_Sensor (+10k code)
    #define MGS_SENSOR_ADDR    0x04              // Default Mutichannel Gas sensor i2c address
#endif  // USE_I2C

// -- SPI sensors ---------------------------------
//#define USE_SPI                                  // SPI using default library
#ifdef USE_SPI

#endif  // USE_SPI

// -- Serial sensors ------------------------------
//#define USE_MHZ19                                // Add support for MH-Z19 CO2 sensor (+2k code)
//#define USE_SENSEAIR                             // Add support for SenseAir K30, K70 and S8 CO2 sensor (+2k3 code)
  #define CO2_LOW              800               // Below this CO2 value show green light (needs PWM or WS2812 RG(B) led and enable with SetOption18 1)
  #define CO2_HIGH             1200              // Above this CO2 value show red light (needs PWM or WS2812 RG(B) led and enable with SetOption18 1)
//#define USE_PMS5003                              // Add support for PMS5003 and PMS7003 particle concentration sensor (+1k3 code)
//#define USE_NOVA_SDS                             // Add support for SDS011 and SDS021 particle concentration sensor (+0k7 code)
//#define USE_PZEM004T                             // Add support for PZEM004T Energy monitor (+2k code)
#define USE_SERIAL_BRIDGE                        // Add support for software Serial Bridge (+0k8 code)
//#define USE_SDM120                               // Add support for Eastron SDM120-Modbus energy meter (+1k7 code)
  #define SDM120_SPEED         9600              // SDM120-Modbus RS485 serial speed (default: 2400 baud)
//#define USE_SDM630                               // Add support for Eastron SDM630-Modbus energy meter (+2k code)
  #define SDM630_SPEED         9600              // SDM630-Modbus RS485 serial speed (default: 9600 baud)

// -- Low level interface devices -----------------
//#define USE_IR_REMOTE                            // Send IR remote commands using library IRremoteESP8266 and ArduinoJson (+4k code, 0k3 mem, 48 iram)
//  #define USE_IR_HVAC                            // Support for HVAC system using IR (+2k code)
//  #define USE_IR_RECEIVE                         // Support for IR receiver (+5k5 code, 264 iram)

#define USE_WS2812                               // WS2812 Led string using library NeoPixelBus (+5k code, +1k mem, 232 iram) - Disable by //
  #define USE_WS2812_CTYPE     NEO_GRB           // WS2812 Color type (NEO_RGB, NEO_GRB, NEO_BRG, NEO_RBG, NEO_RGBW, NEO_GRBW)
//  #define USE_WS2812_DMA                         // DMA supports only GPIO03 (= Serial RXD) (+1k mem). When USE_WS2812_DMA is enabled expect Exceptions on Pow

//#define USE_ARILUX_RF                            // Add support for Arilux RF remote controller (+0k8 code, 252 iram (non 2.3.0))

//#define USE_SR04                                 // Add support for HC-SR04 ultrasonic devices (+1k code)

/*********************************************************************************************\
 * DISPLAY
\*********************************************************************************************/

//#define USE_DISPLAY                            // Add I2C Display Support for LCD, Oled and up to eigth Matrices (+19k code)

//#define DISPLAY_CONFIG                         // Display Support for LCD, Oled
#ifdef DISPLAY_CONFIG

#ifdef USE_I2C
  #define USE_DISPLAY                            // Add I2C Display Support for LCD, Oled and up to eigth Matrices (+19k code)
    #define MTX_ADDRESS1       0x71              // [DisplayAddress[1]] I2C address of first 8x8 matrix module
    #define MTX_ADDRESS2       0x74              // [DisplayAddress[2]] I2C address of second 8x8 matrix module
    #define MTX_ADDRESS3       0x75              // [DisplayAddress[3]] I2C address of third 8x8 matrix module
    #define MTX_ADDRESS4       0x72              // [DisplayAddress[4]] I2C address of fourth 8x8 matrix module
    #define MTX_ADDRESS5       0x73              // [DisplayAddress[5]] I2C address of fifth 8x8 matrix module
    #define MTX_ADDRESS6       0x76              // [DisplayAddress[6]] I2C address of sixth 8x8 matrix module
    #define MTX_ADDRESS7       0x00              // [DisplayAddress[7]] I2C address of seventh 8x8 matrix module
    #define MTX_ADDRESS8       0x00              // [DisplayAddress[8]] I2C address of eigth 8x8 matrix module
#endif  // USE_I2C

#ifdef USE_SPI                                   // SPI using library TasmotaTFT
  #ifndef USE_DISPLAY
  #define USE_DISPLAY                            // Add SPI Display support for 320x240 and 480x320 TFT, using library TasmotaTFT
  #endif
#endif  // USE_SPI

#endif  // DISPLAY_CONFIG

/*********************************************************************************************\
 * TIME PROPORTIONAL CONTROLLER
\*********************************************************************************************/

/**
 * Code to drive one or more relays in a time proportioned manner give a
 * required power value.
 *
 * Given required power values in the range 0.0 to 1.0 the relays will be
 * driven on/off in such that the average power suppled will represent
 * the required power.
 * The cycle time is configurable.  If, for example, the
 * period is set to 10 minutes and the power input is 0.2 then the output will
 * be on for two minutes in every ten minutes.
 *
 * A value for actuator dead time may be provided. If you have a device that
 * takes a significant time to open/close then set this to the average of the
 * open and close times.  The algorithim will then adjust the output timing
 * accordingly to ensure that the output is not switched more rapidly than
 * the actuator can cope with.
 *
 * A facility to invert the output is provided which can be useful when used in
 * refrigeration processes and similar.
 *
 * In the case where only one relay is being driven the power value is set by
 * writing the value to the mqtt topic cmnd/timeprop_setpower_0.  If more than
 * one relay is being driven (as might be the case for a heat/cool application
 * where one relay drives the heater and the other the cooler) then the power
 * for the second relay is written to topic cmnd/timeprop_setpower_1 and so on.
 *
 * To cope with the problem of temporary wifi failure etc a
 * TIMEPROP_MAX_UPDATE_INTERVALS value is available. This can be set to the max
 * expected time between power updates and if this time is exceeded then the
 * power will fallback to a given safe value until a new value is provided. Set
 * the interval to 0 to disable this feature.
 *
 **/

//#define USE_TIMEPROP    //  include the timeprop feature (+1.2k)

// Configuration for single output
/*
 #define TIMEPROP_NUM_OUTPUTS          1       // how many outputs to control (with separate alogorithm for each)
 #define TIMEPROP_CYCLETIMES           60      // cycle time seconds
 #define TIMEPROP_DEADTIMES            0       // actuator action time seconds
 #define TIMEPROP_OPINVERTS            false   // whether to invert the output
 #define TIMEPROP_FALLBACK_POWERS      0       // falls back to this if too long betwen power updates
 #define TIMEPROP_MAX_UPDATE_INTERVALS 120     // max no secs that are allowed between power updates (0 to disable)
 #define TIMEPROP_RELAYS               1       // which relay to control 1:8
/*

// Configuration for two outputs:
/*
 #define TIMEPROP_NUM_OUTPUTS          2               // how many outputs to control (with separate alogorithm for each)
 #define TIMEPROP_CYCLETIMES           60,     10      // cycle time seconds
 #define TIMEPROP_DEADTIMES            0,      0       // actuator action time seconds
 #define TIMEPROP_OPINVERTS            false,  false   // whether to invert the output
 #define TIMEPROP_FALLBACK_POWERS      0,      0       // falls back to this if too long betwen power updates
 #define TIMEPROP_MAX_UPDATE_INTERVALS 120,    120     // max no secs that are allowed between power updates (0 to disable)
 #define TIMEPROP_RELAYS               1,      2       // which relay to control 1:8
*/

/*********************************************************************************************\
 * PID CONTROLLER
\*********************************************************************************************/

// Help with using the PID algorithm and with loop tuning can be found at
// http://blog.clanlaw.org.uk/2018/01/09/PID-tuning-with-node-red-contrib-pid.html
// This is directed towards using the algorithm in the node-red node node-red-contrib-pid but the algorithm here is based on
// the code there and the tuning techique described there should work just the same.

//#define USE_PID         // include the pid feature (+4.3k)
  #define PID_SETPOINT                  19.5    // Setpoint value. This is the process value that the process is
                                                // aiming for.
                                                // May be adjusted via MQTT using cmnd pid_sp

  #define PID_PROPBAND                  5       // Proportional band in process units (eg degrees). This controls
                                                // the gain of the loop and is the range of process value over which
                                                // the power output will go from 0 to full power. The units are that
                                                // of the process and setpoint, so for example in a heating
                                                // application it might be set to 1.5 degrees.
                                                // May be adjusted via MQTT using cmnd pid_pb

  #define PID_INTEGRAL_TIME             1800    // Integral time seconds. This is a setting for the integral time,
                                                // in seconds. It represents the time constant of the integration
                                                // effect. The larger the value the slower the integral effect will be.
                                                // Obviously the slower the process is the larger this should be. For
                                                // example for a domestic room heated by convection radiators a setting
                                                // of one hour might be appropriate (in seconds). To disable the
                                                // integral effect set this to a large number.
                                                // May be adjusted via MQTT using cmnd pid_ti

  #define PID_DERIVATIVE_TIME           15      // Derivative time seconds. This is a setting for the derivative time,
                                                // in seconds. It represents the time constant of the derivative effect.
                                                // The larger the value the greater will be the derivative effect.
                                                // Typically this will be set to somewhat less than 25% of the integral
                                                // setting, once the integral has been adjusted to the optimum value. To
                                                // disable the derivative effect set this to 0. When initially tuning a
                                                // loop it is often sensible to start with derivative zero and wind it in
                                                // once other parameters have been setup.
                                                // May be adjusted via MQTT using cmnd pid_td

  #define PID_INITIAL_INT               0.5     // Initial integral value (0:1). This is an initial value which is used
                                                // to preset the integrated error value when the flow is deployed in
                                                // order to assist in homing in on the setpoint the first time. It should
                                                // be set to an estimate of what the power requirement might be in order
                                                // to maintain the process at the setpoint. For example for a domestic
                                                // room heating application it might be set to 0.2 indicating that 20% of
                                                // the available power might be required to maintain the setpoint. The
                                                // value is of no consequence apart from device restart.

  #define PID_MAX_INTERVAL              300     // This is the maximum time in seconds that is expected between samples.
                                                // It is provided to cope with unusual situations such as a faulty sensor
                                                // that might prevent the node from being supplied with a process value.
                                                // If no new process value is received for this time then the power is set
                                                // to the value defined for PID_MANUAL_POWER.
                                                // May be adjusted via MQTT using cmnd pid_max_interval

  #define PID_DERIV_SMOOTH_FACTOR       3       // In situations where the process sensor has limited resolution (such as
                                                // the DS18B20), the use of deriviative can be problematic as when the
                                                // process is changing only slowly the steps in the value cause spikes in
                                                // the derivative. To reduce the effect of these this parameter can be
                                                // set to apply a filter to the derivative term. I have found that with
                                                // the DS18B20 that a value of 3 here can be beneficial, providing
                                                // effectively a low pass filter on the derivative at 1/3 of the derivative
                                                // time. This feature may also be useful if the process value is particularly
                                                // noisy. The smaller the value the greater the filtering effect but the
                                                // more it will reduce the effectiveness of the derivative. A value of zero
                                                // disables this feature.
                                                // May be adjusted via MQTT using cmnd pid_d_smooth

  #define PID_AUTO                      1       // Auto mode 1 or 0 (for manual). This can be used to enable or disable
                                                // the control (1=enable, auto mode, 0=disabled, manual mode). When in
                                                // manual mode the output is set the value definded for PID_MANUAL_POWER
                                                // May be adjusted via MQTT using cmnd pid_auto

  #define PID_MANUAL_POWER              0       // Power output when in manual mode or fallback mode if too long elapses
                                                // between process values
                                                // May be adjusted via MQTT using cmnd pid_manual_power

  #define PID_UPDATE_SECS               0       // How often to run the pid algorithm (integer secs) or 0 to run the algorithm
                                                // each time a new pv value is received, for most applictions specify 0.
                                                // Otherwise set this to a time
                                                // that is short compared to the response of the process.  For example,
                                                // something like 15 seconds may well be appropriate for a domestic room
                                                // heating application.
                                                // May be adjusted via MQTT using cmnd pid_update_secs

  #define PID_USE_TIMPROP               1       // To use an internal relay for a time proportioned output to drive the
                                                // process, set this to indicate which timeprop output to use. For a device
                                                // with just one relay then this will be 1.
                                                // It is then also necessary to define USE_TIMEPROP and set the output up as
                                                // explained in xdrv_91_timeprop.ino
                                                // To disable this feature leave this undefined (undefined, not defined to nothing).

  #define PID_USE_LOCAL_SENSOR                  // if defined then the local sensor will be used for pv. Leave undefined if
                                                // this is not required.  The rate that the sensor is read is defined by TELE_PERIOD
                                                // If not using the sensor then you can supply process values via MQTT using
                                                // cmnd pid_pv


/*********************************************************************************************\
 * Select features and sensors enabled in previous version saving space
\*********************************************************************************************/

//#define USE_CLASSIC                             // Create sonoff-classic (See sonoff_post.h for selected features)

/*********************************************************************************************\
 * Select all sensors - overrides above undefines!!
\*********************************************************************************************/

//#define USE_ALL_SENSORS                          // Create sonoff-allsensors with all sensors enabled (See sonoff_post.h for selected sensors)

/*********************************************************************************************\
 * Select KNX without Emulation to save space
\*********************************************************************************************/

//#define USE_KNX_NO_EMULATION                     // Create sonoff-knx with KNX but without Emulation (See sonoff_post.h)

/*********************************************************************************************\
 * Compile a minimal version if upgrade memory gets tight ONLY TO BE USED FOR UPGRADE STEP 1!
 *   To be used as step 1 during upgrade.
 *   Step 2 is re-compile with option BE_MINIMAL commented out.
\*********************************************************************************************/

//#define BE_MINIMAL                               // Minimal version if upgrade memory gets tight (-45k code, -2k mem)

/*********************************************************************************************\
 * No user configurable items below
\*********************************************************************************************/

#if defined(USE_MQTT_TLS) && defined(USE_WEBSERVER)
  #error "Select either USE_MQTT_TLS or USE_WEBSERVER as there is just not enough memory to play with"
#endif

#endif  // _USER_CONFIG_H_
