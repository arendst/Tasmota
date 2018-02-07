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
 * ATTENTION: Changes to most PARAMETER defines will only override flash settings if you change
 *            define CFG_HOLDER.
 *            Most parameters can be changed online using commands via MQTT, WebConsole or serial
 *
 * Corresponding MQTT/Serial/Console commands in [brackets]
\*********************************************************************************************/

// -- Localization --------------------------------
//#define MY_LANGUAGE            de-DE           // German in Germany
//#define MY_LANGUAGE            en-GB           // English in Great Britain. Enabled by Default
//#define MY_LANGUAGE            es-AR           // Spanish in Argentina
//#define MY_LANGUAGE            fr-FR           // French in France
//#define MY_LANGUAGE            it-IT           // Italian in Italy
//#define MY_LANGUAGE            nl-NL           // Dutch in the Netherlands
//#define MY_LANGUAGE            pl-PL           // Polish in Poland
//#define MY_LANGUAGE            zh-CN           // Chinese (Simplified) in China

// -- Project -------------------------------------
#define PROJECT                "sonoff"          // PROJECT is used as the default topic delimiter and OTA file name
                                                 //   As an IDE restriction it needs to be the same as the main .ino file

#define CFG_HOLDER             0x20161209        // [Reset 1] Change this value to load following default configuration parameters
#define SAVE_DATA              1                 // [SaveData] Save changed parameters to Flash (0 = disable, 1 - 3600 seconds)
#define SAVE_STATE             1                 // [SetOption0] Save changed power state to Flash (0 = disable, 1 = enable)

// -- Wifi ----------------------------------------
#define WIFI_IP_ADDRESS        "0.0.0.0"         // [IpAddress1] Set to 0.0.0.0 for using DHCP or IP address
#define WIFI_GATEWAY           "192.168.2.254"   // [IpAddress2] If not using DHCP set Gateway IP address
#define WIFI_SUBNETMASK        "255.255.255.0"   // [IpAddress3] If not using DHCP set Network mask
#define WIFI_DNS               "192.168.2.27"    // [IpAddress4] If not using DHCP set DNS IP address (might be equal to WIFI_GATEWAY)

#define STA_SSID1              "indebuurt1"      // [Ssid1] Wifi SSID
#define STA_PASS1              "VnsqrtnrsddbrN"  // [Password1] Wifi password
#define STA_SSID2              "indebuurt2"      // [Ssid2] Optional alternate AP Wifi SSID
#define STA_PASS2              "VnsqrtnrsddbrN"  // [Password2] Optional alternate AP Wifi password
#define WIFI_CONFIG_TOOL       WIFI_WPSCONFIG    // [WifiConfig] Default tool if wifi fails to connect
                                                 //   (WIFI_RESTART, WIFI_SMARTCONFIG, WIFI_MANAGER, WIFI_WPSCONFIG, WIFI_RETRY, WIFI_WAIT)

// -- Syslog --------------------------------------
#define SYS_LOG_HOST           "domus1"          // [LogHost] (Linux) syslog host
#define SYS_LOG_PORT           514               // [LogPort] default syslog UDP port
#define SYS_LOG_LEVEL          LOG_LEVEL_NONE    // [SysLog]
#define SERIAL_LOG_LEVEL       LOG_LEVEL_INFO    // [SerialLog]
#define WEB_LOG_LEVEL          LOG_LEVEL_INFO    // [WebLog]

// -- Ota -----------------------------------------
#define OTA_URL                "http://domus1:80/api/arduino/" PROJECT ".ino.bin"  // [OtaUrl]

// -- MQTT ----------------------------------------
#define MQTT_USE               1                 // [SetOption3] Select default MQTT use (0 = Off, 1 = On)
// !!! TLS uses a LOT OF MEMORY (20k) so be careful to enable other options at the same time !!!
//#define USE_MQTT_TLS                             // EXPERIMENTAL Use TLS for MQTT connection (+53k code, +20k mem) - Disable by //
                                                 //   Needs Fingerprint, TLS Port, UserId and Password
#ifdef USE_MQTT_TLS
  #define MQTT_HOST            "m20.cloudmqtt.com"  // [MqttHost]
  #define MQTT_FINGERPRINT     "A5 02 FF 13 99 9F 8B 39 8E F1 83 4F 11 23 65 0B 32 36 FC 07"  // [MqttFingerprint]
  #define MQTT_PORT            20123                // [MqttPort] MQTT TLS port
  #define MQTT_USER            "cloudmqttuser"      // [MqttUser] Mandatory user
  #define MQTT_PASS            "cloudmqttpassword"  // [MqttPassword] Mandatory password
#else
  #define MQTT_HOST            "domus1"          // [MqttHost]
  #define MQTT_PORT            1883              // [MqttPort] MQTT port (10123 on CloudMQTT)
  #define MQTT_USER            "DVES_USER"       // [MqttUser] Optional user
  #define MQTT_PASS            "DVES_PASS"       // [MqttPassword] Optional password
#endif

#define MQTT_BUTTON_RETAIN     0                 // [ButtonRetain] Button may send retain flag (0 = off, 1 = on)
#define MQTT_POWER_RETAIN      0                 // [PowerRetain] Power status message may send retain flag (0 = off, 1 = on)
#define MQTT_SWITCH_RETAIN     0                 // [SwitchRetain] Switch may send retain flag (0 = off, 1 = on)

#define MQTT_STATUS_OFF        "OFF"             // [StateText1] Command or Status result when turned off (needs to be a string like "0" or "Off")
#define MQTT_STATUS_ON         "ON"              // [StateText2] Command or Status result when turned on (needs to be a string like "1" or "On")
#define MQTT_CMND_TOGGLE       "TOGGLE"          // [StateText3] Command to send when toggling (needs to be a string like "2" or "Toggle")
#define MQTT_CMND_HOLD         "HOLD"            // [StateText4] Command to send when button is kept down for over KEY_HOLD_TIME * 0.1 seconds (needs to be a string like "HOLD")

// -- MQTT topics ---------------------------------
//#define MQTT_FULLTOPIC         "tasmota/bedroom/%topic%/%prefix%/" // Up to max 80 characers
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
#define USE_DOMOTICZ                             // Enable Domoticz (+6k code, +0.3k mem) - Disable by //
  #define DOMOTICZ_IN_TOPIC      "domoticz/in"   // Domoticz Input Topic
  #define DOMOTICZ_OUT_TOPIC     "domoticz/out"  // Domoticz Output Topic
  #define DOMOTICZ_UPDATE_TIMER  0               // [DomoticzUpdateTimer] Send relay status (0 = disable, 1 - 3600 seconds) (Optional)

// -- MQTT - Home Assistant Discovery -------------
#define USE_HOME_ASSISTANT                       // Enable Home Assistant Discovery Support (+1k4 code)
  #define HOME_ASSISTANT_DISCOVERY_PREFIX "homeassistant"  // Home Assistant discovery prefix
  #define HOME_ASSISTANT_DISCOVERY_ENABLE 0      // [SetOption19] Home Assistant Discovery (0 = Disable, 1 = Enable)

// -- HTTP ----------------------------------------
#define USE_WEBSERVER                            // Enable web server and wifi manager (+66k code, +8k mem) - Disable by //
  #define WEB_SERVER           2                 // [WebServer] Web server (0 = Off, 1 = Start as User, 2 = Start as Admin)
  #define WEB_PORT             80                // Web server Port for User and Admin mode
  #define WEB_USERNAME         "admin"           // Web server Admin mode user name
  #define WEB_PASSWORD         ""                // [WebPassword] Web server Admin mode Password for WEB_USERNAME (empty string = Disable)
  #define FRIENDLY_NAME        "Sonoff"          // [FriendlyName] Friendlyname up to 32 characters used by webpages and Alexa
  #define USE_EMULATION                          // Enable Belkin WeMo and Hue Bridge emulation for Alexa (+16k code, +2k mem)
    #define EMULATION          EMUL_NONE         // [Emulation] Select Belkin WeMo (single relay/light) or Hue Bridge emulation (multi relay/light) (EMUL_NONE, EMUL_WEMO or EMUL_HUE)

// -- mDNS ----------------------------------------
#define USE_DISCOVERY                            // Enable mDNS for the following services (+8k code, +0.3k mem) - Disable by //
  #define WEBSERVER_ADVERTISE                    // Provide access to webserver by name <Hostname>.local/
  #define MQTT_HOST_DISCOVERY                    // Find MQTT host server (overrides MQTT_HOST if found)

// -- Time - Up to three NTP servers in your region
#define NTP_SERVER1            "pool.ntp.org"       // [NtpServer1] Select first NTP server by name or IP address (129.250.35.250)
#define NTP_SERVER2            "nl.pool.ntp.org"    // [NtpServer2] Select second NTP server by name or IP address (5.39.184.5)
#define NTP_SERVER3            "0.nl.pool.ntp.org"  // [NtpServer3] Select third NTP server by name or IP address (93.94.224.67)

// -- Time - Start Daylight Saving Time and timezone offset from UTC in minutes
#define TIME_DST               North, Last, Sun, Mar, 2, +120  // Northern Hemisphere, Last sunday in march at 02:00 +120 minutes

// -- Time - Start Standard Time and timezone offset from UTC in minutes
#define TIME_STD               North, Last, Sun, Oct, 3, +60   // Northern Hemisphere, Last sunday in october 02:00 +60 minutes

// -- Application ---------------------------------
#define APP_TIMEZONE           1                 // [Timezone] +1 hour (Amsterdam) (-12 .. 12 = hours from UTC, 99 = use TIME_DST/TIME_STD)
#define APP_LEDSTATE           LED_POWER         // [LedState] Function of led (LED_OFF, LED_POWER, LED_MQTTSUB, LED_POWER_MQTTSUB, LED_MQTTPUB, LED_POWER_MQTTPUB, LED_MQTT, LED_POWER_MQTT)
#define APP_PULSETIME          0                 // [PulseTime] Time in 0.1 Sec to turn off power for relay 1 (0 = disabled)
#define APP_POWERON_STATE      3                 // [PowerOnState] Power On Relay state (0 = Off, 1 = On, 2 = Toggle Saved state, 3 = Saved state)
#define APP_BLINKTIME          10                // [BlinkTime] Time in 0.1 Sec to blink/toggle power for relay 1
#define APP_BLINKCOUNT         10                // [BlinkCount] Number of blinks (0 = 32000)
#define APP_SLEEP              0                 // [Sleep] Sleep time to lower energy consumption (0 = Off, 1 - 250 mSec)

#define KEY_HOLD_TIME          40                // [SetOption32] Number of 0.1 seconds to hold Button or external Pushbutton before sending HOLD message
#define SWITCH_MODE            TOGGLE            // [SwitchMode] TOGGLE, FOLLOW, FOLLOW_INV, PUSHBUTTON, PUSHBUTTON_INV, PUSHBUTTONHOLD, PUSHBUTTONHOLD_INV or PUSHBUTTON_TOGGLE (the wall switch state)
#define WS2812_LEDS            30                // [Pixels] Number of WS2812 LEDs to start with

#define TEMP_CONVERSION        0                 // [SetOption8] Return temperature in (0 = Celsius or 1 = Fahrenheit)
#define TEMP_RESOLUTION        1                 // [TempRes] Maximum number of decimals (0 - 3) showing sensor Temperature
#define HUMIDITY_RESOLUTION    1                 // [HumRes] Maximum number of decimals (0 - 3) showing sensor Humidity
#define PRESSURE_RESOLUTION    1                 // [PressRes] Maximum number of decimals (0 - 3) showing sensor Pressure
#define ENERGY_RESOLUTION      3                 // [EnergyRes] Maximum number of decimals (0 - 5) showing energy usage in kWh

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
  #define USE_SHT3X                              // Add I2C code for SHT3x sensor (+0k6 code)
  #define USE_HTU                                // Add I2C code for HTU21/SI7013/SI7020/SI7021 sensor (+1k5 code)
  #define USE_BMP                                // Add I2C code for BMP085/BMP180/BMP280/BME280 sensor (+4k code)
//    #define USE_BME680                           // Add additional support for BME680 sensor using Adafruit Sensor and BME680 libraries (+6k code)
  #define USE_BH1750                             // Add I2C code for BH1750 sensor (+0k5 code)
//  #define USE_VEML6070                           // Add I2C code for VEML6070 sensor (+0k5 code)
//  #define USE_TSL2561                            // Add I2C code for TSL2561 sensor using library Adafruit TSL2561 Arduino (+1k2 code)
//  #define USE_ADS1115                            // Add I2C code for ADS1115 16 bit A/D converter based on Adafruit ADS1x15 library (no library needed) (+0k7 code)
//  #define USE_ADS1115_I2CDEV                     // Add I2C code for ADS1115 16 bit A/D converter using library i2cdevlib-Core and i2cdevlib-ADS1115 (+2k code)
//  #define USE_INA219                             // Add I2C code for INA219 Low voltage and current sensor (+1k code)
#endif  // USE_I2C

// -- Serial sensors ------------------------------
#define USE_MHZ19                                // Add support for MH-Z19 CO2 sensor (+2k code)
#define USE_SENSEAIR                             // Add support for SenseAir K30, K70 and S8 CO2 sensor (+2k3 code)
  #define CO2_LOW              800               // Below this CO2 value show green light (needs PWM or WS2812 RG(B) led and enable with SetOption18 1)
  #define CO2_HIGH             1200              // Above this CO2 value show red light (needs PWM or WS2812 RG(B) led and enable with SetOption18 1)
#define USE_PMS5003                              // Add support for PMS5003 and PMS7003 particle concentration sensor (+1k3 code)
#define USE_PZEM004T                             // Add support for PZEM004T Energy monitor (+2k code)

// -- Low level interface devices -----------------
#define USE_IR_REMOTE                            // Send IR remote commands using library IRremoteESP8266 and ArduinoJson (+4k code, 0k3 mem, 48 iram)
//  #define USE_IR_HVAC                            // Support for HVAC system using IR (+2k code)
  #define USE_IR_RECEIVE                         // Support for IR receiver (+5k5 code, 264 iram)

#define USE_WS2812                               // WS2812 Led string using library NeoPixelBus (+5k code, +1k mem, 232 iram) - Disable by //
  #define USE_WS2812_CTYPE     1                 // WS2812 Color type (0 - RGB, 1 - GRB, 2 - RGBW, 3 - GRBW)
//  #define USE_WS2812_DMA                         // DMA supports only GPIO03 (= Serial RXD) (+1k mem). When USE_WS2812_DMA is enabled expect Exceptions on Pow

#define USE_ARILUX_RF                            // Add support for Arilux RF remote controller (+0k8 code, 252 iram (non 2.3.0))

/*********************************************************************************************\
 * Compile a minimal version if upgrade memory gets tight ONLY TO BE USED FOR UPGRADE STEP 1!
 *   To be used as step 1 during upgrade.
 *   Step 2 is re-compile with option BE_MINIMAL commented out.
 *   !!! Needed for next release of Arduino/ESP8266 (+22k code, +2k mem) !!!
\*********************************************************************************************/

//#define BE_MINIMAL                               // Minimal version if upgrade memory gets tight (-45k code, -2k mem)

/*********************************************************************************************\
 * No user configurable items below
\*********************************************************************************************/

#if defined(USE_MQTT_TLS) && defined(USE_WEBSERVER)
  #error "Select either USE_MQTT_TLS or USE_WEBSERVER as there is just not enough memory to play with"
#endif

#endif  // _USER_CONFIG_H_
