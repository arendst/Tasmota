/*********************************************************************************************\
 * User specific configuration parameters
 *
 * ATTENTION: Changes to most PARAMETER defines will only override flash settings if you change
 *            define CFG_HOLDER.
 *            Most parameters can be changed online using commands via MQTT, WebConsole or serial
 *
 * Corresponding MQTT/Serial/Console commands in [brackets]
\*********************************************************************************************/

// -- Project -------------------------------------
#define PROJECT                "sonoff"          // PROJECT is used as the default topic delimiter and OTA file name
                                                 //   As an IDE restriction it needs to be the same as the main .ino file

#define CFG_HOLDER             0x20161209        // [Reset 1] Change this value to load following default configuration parameters
#define SAVE_DATA              1                 // [SaveData] Save changed parameters to Flash (0 = disable, 1 - 3600 seconds)
#define SAVE_STATE             1                 // [SaveState] Save changed power state to Flash (0 = disable, 1 = enable)

// -- Wifi ----------------------------------------
#define STA_SSID1              "indebuurt1"      // [Ssid1] Wifi SSID
#define STA_PASS1              "VnsqrtnrsddbrN"  // [Password1] Wifi password
#define STA_SSID2              "indebuurt2"      // [Ssid2] Optional alternate AP Wifi SSID
#define STA_PASS2              "VnsqrtnrsddbrN"  // [Password2] Optional alternate AP Wifi password
#define WIFI_HOSTNAME          "%s-%04d"         // [Hostname] Expands to <MQTT_TOPIC>-<last 4 decimal chars of MAC address>
#define WIFI_CONFIG_TOOL       WIFI_WPSCONFIG    // [WifiConfig] Default tool if wifi fails to connect
                                                 //   (WIFI_RESTART, WIFI_SMARTCONFIG, WIFI_MANAGER, WIFI_WPSCONFIG, WIFI_RETRY)
// -- Syslog --------------------------------------
#define SYS_LOG_HOST           "domus1"          // [LogHost] (Linux) syslog host
#define SYS_LOG_PORT           514               // [LogPort] default syslog UDP port
#define SYS_LOG_LEVEL          LOG_LEVEL_NONE    // [SysLog]
#define SERIAL_LOG_LEVEL       LOG_LEVEL_INFO    // [SerialLog]
#define WEB_LOG_LEVEL          LOG_LEVEL_INFO    // [WebLog]

// -- Ota -----------------------------------------
#define OTA_URL                "http://domus1:80/api/arduino/" PROJECT ".ino.bin"  // [OtaUrl]

// -- MQTT ----------------------------------------
#define MQTT_USE               1                 // [Mqtt] Select default MQTT use (0 = Off, 1 = On)
// !!! TLS uses a LOT OF MEMORY (20k) so be careful to enable other options at the same time !!!
//#define USE_MQTT_TLS                             // EXPERIMENTAL Use TLS for MQTT connection (+53k code, +20k mem)
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

#define MQTT_CLIENT_ID         "DVES_%06X"       // [MqttClient] Also fall back topic using Chip Id = last 6 characters of MAC address

#define SUB_PREFIX             "cmnd"            // Sonoff devices subscribe to:- SUB_PREFIX/MQTT_TOPIC and SUB_PREFIX/MQTT_GRPTOPIC
#define PUB_PREFIX             "stat"            // Sonoff devices publish to:- PUB_PREFIX/MQTT_TOPIC
#define PUB_PREFIX2            "tele"            // Sonoff devices publish telemetry data to:- PUB_PREFIX2/MQTT_TOPIC/UPTIME, POWER/LIGHT and TIME
                                                 //   May be named the same as PUB_PREFIX
#define MQTT_TOPIC             PROJECT           // [Topic] (unique) MQTT device topic
#define MQTT_GRPTOPIC          "sonoffs"         // [GroupTopic] MQTT Group topic
#define MQTT_BUTTON_RETAIN     0                 // [ButtonRetain] Button may send retain flag (0 = off, 1 = on)
#define MQTT_POWER_RETAIN      0                 // [PowerRetain] Power status message may send retain flag (0 = off, 1 = on)
#define MQTT_SWITCH_RETAIN     0                 // [SwitchRetain] Switch may send retain flag (0 = off, 1 = on)

#define MQTT_STATUS_OFF        "OFF"             // Command or Status result when turned off (needs to be a string like "0" or "Off")
#define MQTT_STATUS_ON         "ON"              // Command or Status result when turned on (needs to be a string like "1" or "On")
#define MQTT_CMND_TOGGLE       "TOGGLE"          // Command to send when toggling (needs to be a string like "2" or "Toggle")

// -- MQTT - Telemetry ----------------------------
#define TELE_PERIOD            300               // [TelePeriod] Telemetry (0 = disable, 10 - 3600 seconds)

// -- MQTT - Domoticz -----------------------------
#define USE_DOMOTICZ                             // Enable Domoticz (+4k code, +0.2k mem) - Disable by //
  #define DOMOTICZ_IN_TOPIC      "domoticz/in"   // [DomoticzInTopic]
  #define DOMOTICZ_OUT_TOPIC     "domoticz/out"  // [DomoticzOutTopic]
  #define DOMOTICZ_RELAY_IDX1    0               // [DomoticzIdx1] Sonoff Relay 1 (0 = disable domoticz)
  #define DOMOTICZ_RELAY_IDX2    0               // [DomoticzIdx2] Sonoff Dual and 4CH Relay 2
  #define DOMOTICZ_RELAY_IDX3    0               // [DomoticzIdx3] Sonoff 4CH Relay 3
  #define DOMOTICZ_RELAY_IDX4    0               // [DomoticzIdx4] Sonoff 4CH Relay 4
  #define DOMOTICZ_KEY_IDX1      0               // [DomoticzKeyIdx1] Button single press (0 = disable)
  #define DOMOTICZ_KEY_IDX2      0               // [DomoticzKeyIdx2] Button double press (0 = disable)
  #define DOMOTICZ_KEY_IDX3      0               // [DomoticzKeyIdx3] Sonoff 4CH Button 3
  #define DOMOTICZ_KEY_IDX4      0               // [DomoticzKeyIdx4] Sonoff 4CH Button 4
  #define DOMOTICZ_UPDATE_TIMER  0               // [DomoticzUpdateTimer] Send relay status (0 = disable, 1 - 3600 seconds) (Optional)

// -- HTTP ----------------------------------------
#define USE_WEBSERVER                            // Enable web server and wifi manager (+43k code, +2k mem) - Disable by //
  #define FRIENDLY_NAME1         "Sonoff"        // [FriendlyName1] Friendlyname up to 32 characters used by webpages and Alexa
  #define FRIENDLY_NAME2         "Sonoff2"       // [FriendlyName2] Friendlyname up to 32 characters used by Alexa
  #define FRIENDLY_NAME3         "Sonoff3"       // [FriendlyName3] Friendlyname up to 32 characters used by Alexa
  #define FRIENDLY_NAME4         "Sonoff4"       // [FriendlyName4] Friendlyname up to 32 characters used by Alexa
  #define WEB_SERVER             2               // [WebServer] Web server (0 = Off, 1 = Start as User, 2 = Start as Admin)
//  #define USE_WEMO_EMULATION                     // Enable Belkin WeMo PowerSwitch emulation for Alexa (+4k code, +2k mem)
//  #define USE_HUE_EMULATION                      // Enable Hue Bridge emulation for Alexa (+5k code, +2k mem)

// -- mDNS ----------------------------------------
#define USE_DISCOVERY                            // Enable mDNS for the following services (+8k code, +0.3k mem)
  #define WEBSERVER_ADVERTISE                    // Provide access to webserver by name <Hostname>.local/
  #define MQTT_HOST_DISCOVERY                    // Find MQTT host server (overrides MQTT_HOST if found)

// -- Time - Up to three NTP servers in your region
#define NTP_SERVER1            "pool.ntp.org"
#define NTP_SERVER2            "nl.pool.ntp.org"
#define NTP_SERVER3            "0.nl.pool.ntp.org"

// -- Time - Start Daylight Saving Time and timezone offset from UTC in minutes
#define TIME_DST               Last, Sun, Mar, 2, +120  // Last sunday in march at 02:00 +120 minutes

// -- Time - Start Standard Time and timezone offset from UTC in minutes
#define TIME_STD               Last, Sun, Oct, 3, +60   // Last sunday in october 02:00 +60 minutes

// -- Application ---------------------------------
#define APP_TIMEZONE           1                 // [Timezone] +1 hour (Amsterdam) (-12 .. 12 = hours from UTC, 99 = use TIME_DST/TIME_STD)
#define APP_LEDSTATE           LED_POWER         // [LedState] Function of led (LED_OFF, LED_POWER, LED_MQTTSUB, LED_POWER_MQTTSUB, LED_MQTTPUB, LED_POWER_MQTTPUB, LED_MQTT, LED_POWER_MQTT)
#define APP_PULSETIME          0                 // [PulseTime] Time in 0.1 Sec to turn off power for relay 1 (0 = disabled)
#define APP_POWERON_STATE      3                 // [PowerOnState] Power On Relay state (0 = Off, 1 = On, 2 = Toggle Saved state, 3 = Saved state)
#define APP_BLINKTIME          10                // [BlinkTime] Time in 0.1 Sec to blink/toggle power for relay 1
#define APP_BLINKCOUNT         10                // [BlinkCount] Number of blinks (0 = 32000)

#define SWITCH_MODE            TOGGLE            // [SwitchMode] TOGGLE, FOLLOW, FOLLOW_INV, PUSHBUTTON or PUSHBUTTON_INV (the wall switch state)
#define WS2812_LEDS            30                // [Pixels] Number of WS2812 LEDs to start with

#define TEMP_CONVERSION        0                 // Convert temperature to (0 = Celsius or 1 = Fahrenheit)
#define TEMP_RESOLUTION        1                 // Maximum number of decimals (0 - 3) showing sensor Temperature
#define HUMIDITY_RESOLUTION    1                 // Maximum number of decimals (0 - 3) showing sensor Humidity
#define PRESSURE_RESOLUTION    1                 // Maximum number of decimals (0 - 3) showing sensor Pressure

// -- Sensor code selection -----------------------
#define USE_DHT1                                 // *** Option 1 - No external library needed
//#define USE_DHT2                                 // *** Option 2 - Use Adafruit DHT library

#define USE_DS18B20                              // *** Option 1 - Single DS18B20 without library
//#define USE_DS18x20                              // *** Option 2 - Use OneWire library for multiple DS18B20 and/or DS18S20

#define USE_I2C                                  // I2C Support (+10k code, 0.2k mem)
  #define USE_BH1750                             // Add I2C code for BH1750 sensor
  #define USE_BMP                                // Add I2C code for BMP/BME280 sensor
  #define USE_HTU                                // Add I2C code for HTU21 sensor

#define USE_WS2812                               // WS2812 Led string support (+8k code, +1k mem)
//  #define USE_WS2812_DMA                         // Using DMA only GPIO03 (= Serial TXD) is supported (+1k mem)
                                                 //   When USE_WS2812_DMA is enabled expect Exceptions on Pow

/*********************************************************************************************\
 * No user configurable items below
\*********************************************************************************************/

#if defined(USE_WEMO_EMULATION) && defined(USE_HUE_EMULATION)
  #error "Select either USE_WEMO_EMULATION or USE_HUE_EMULATION"
#endif

#if (ARDUINO < 10610)
  #error "This software is supported with Arduino IDE starting from 1.6.10 and ESP8266 Release 2.3.0"
#endif

