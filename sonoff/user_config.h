/*********************************************************************************************\
 * User specific configuration parameters
 *
 * ATTENTION: Changes to most PARAMETER defines will only override flash settings if you change
 *            define CFG_HOLDER.
 *            Most parameters can be changed online using commands via MQTT, WebConsole or serial
 *
 * Corresponding MQTT/Serial/Console commands in [brackets]
\*********************************************************************************************/

// Enable only one out of five MODULE defines below
#define MODULE                 SONOFF            // Sonoff Basic, Sonoff RF, Sonoff SV, Sonoff Dual, Sonoff TH, S20 Smart Socket
//#define MODULE                 SONOFF_2          // Sonoff Touch, Sonoff 4CH
//#define MODULE                 SONOFF_POW        // Sonoff Pow
//#define MODULE                 MOTOR_CAC         // iTead Motor Clockwise/Anticlockwise
//#define MODULE                 ELECTRO_DRAGON    // Electro Dragon Wifi IoT Relay Board Based on ESP8266

// -- Project --------------------------------
#define PROJECT                "sonoff"     // PROJECT is used as the default topic delimiter and OTA file name
                                            //   As an IDE restriction it needs to be the same as the main .ino file

#define CFG_HOLDER             0x20161209   // [Reset 1] Change this value to load following default configuration parameters
#define SAVE_DATA              1            // [SaveData] Save changed parameters to Flash (0 = disable, 1 - 3600 seconds)
#define SAVE_STATE             1            // [SaveState] Save changed power state to Flash (0 = disable, 1 = enable)

// -- Wifi -----------------------------------
#define STA_SSID1              "indebuurt1"      // [Ssid1] Wifi SSID
#define STA_PASS1              "VnsqrtnrsddbrN"  // [Password1] Wifi password
#define STA_SSID2              "indebuurt2"      // [Ssid2] Optional alternate AP Wifi SSID
#define STA_PASS2              "VnsqrtnrsddbrN"  // [Password2] Optional alternate AP Wifi password
#define WIFI_HOSTNAME          "%s-%04d"         // [Hostname] Expands to <MQTT_TOPIC>-<last 4 decimal chars of MAC address>
#define WIFI_CONFIG_TOOL       WIFI_WPSCONFIG    // [WifiConfig] Default tool if wifi fails to connect
                                                 //   (WIFI_RESTART, WIFI_SMARTCONFIG, WIFI_MANAGER, WIFI_WPSCONFIG, WIFI_RETRY)
// -- Syslog ---------------------------------
#define SYS_LOG_HOST           "domus1"        // [LogHost] (Linux) syslog host
#define SYS_LOG_PORT           514             // [LogPort] default syslog UDP port
#define SYS_LOG_LEVEL          LOG_LEVEL_NONE  // [SysLog]
#define SERIAL_LOG_LEVEL       LOG_LEVEL_INFO  // [SerialLog]
#define WEB_LOG_LEVEL          LOG_LEVEL_INFO  // [WebLog]

// -- Ota ------------------------------------
#if (ARDUINO >= 168)
  #define OTA_URL              "http://domus1:80/api/arduino/" PROJECT ".ino.bin"  // [OtaUrl]
#else
  #define OTA_URL              "http://domus1:80/api/arduino/" PROJECT ".cpp.bin"  // [OtaUrl]
#endif

// -- MQTT -----------------------------------
#define USE_MQTT                            // Enable MQTT and Domoticz (+10k code, +1k mem)

// !!! TLS uses a LOT OF MEMORY (20k) so be careful to enable other options at the same time !!!
//#define USE_MQTT_TLS                        // EXPERIMENTAL Use TLS for MQTT connection (+53k code, +20k mem)
                                            //   Needs Fingerprint, TLS Port, UserId and Password
#ifdef USE_MQTT_TLS
  #define MQTT_HOST            "m20.cloudmqtt.com"  // [MqttHost]
  #define MQTT_FINGERPRINT     "A5 02 FF 13 99 9F 8B 39 8E F1 83 4F 11 23 65 0B 32 36 FC 07"  // [MqttFingerprint]
  #define MQTT_PORT            20123                // [MqttPort] MQTT TLS port
  #define MQTT_USER            "cloudmqttuser"      // [MqttUser] Mandatory user
  #define MQTT_PASS            "cloudmqttpassword"  // [MqttPassword] Mandatory password
#else
  #define MQTT_HOST            "domus1"     // [MqttHost]
  #define MQTT_PORT            1883         // [MqttPort] MQTT port (10123 on CloudMQTT)
  #define MQTT_USER            "DVES_USER"  // [MqttUser] Optional user
  #define MQTT_PASS            "DVES_PASS"  // [MqttPassword] Optional password
#endif

#define MQTT_CLIENT_ID         "DVES_%06X"  // [MqttClient] Also fall back topic using Chip Id = last 6 characters of MAC address

#define SUB_PREFIX             "cmnd"       // Sonoff devices subscribe to:- SUB_PREFIX/MQTT_TOPIC and SUB_PREFIX/MQTT_GRPTOPIC
#define PUB_PREFIX             "stat"       // Sonoff devices publish to:- PUB_PREFIX/MQTT_TOPIC
#define PUB_PREFIX2            "tele"       // Sonoff devices publish telemetry data to:- PUB_PREFIX2/MQTT_TOPIC/UPTIME, POWER/LIGHT and TIME
                                            //   May be named the same as PUB_PREFIX
#define MQTT_TOPIC             PROJECT      // [Topic] (unique) MQTT device topic
#define MQTT_BUTTON_RETAIN     0            // [ButtonRetain] Button may send retain flag (0 = off, 1 = on)
#define MQTT_POWER_RETAIN      0            // [PowerRetain] Power status message may send retain flag (0 = off, 1 = on)

#define MESSAGE_FORMAT         LEGACY       // [MessageFormat] MQTT Message Format (LEGACY or JSON)
#define MQTT_STATUS_OFF        "OFF"        // Command or Status result when turned off (needs to be a string like "0" or "Off")
#define MQTT_STATUS_ON         "ON"         // Command or Status result when turned on (needs to be a string like "1" or "On")
#define MQTT_CMND_TOGGLE       "TOGGLE"     // Command to send when toggling (needs to be a string like "2" or "Toggle")

// -- MQTT - Telemetry -----------------------
#define TELE_PERIOD            300          // [TelePeriod] Telemetry (0 = disable, 10 - 3600 seconds)
#define SEND_TELEMETRY_UPTIME               // Enable sending uptime telemetry (if disabled will still send hourly message)
#define SEND_TELEMETRY_WIFI                 // Enable sending wifi telemetry
#define SEND_TELEMETRY_POWER                // Enable sending power telemetry

// -- MQTT - Domoticz ------------------------
#define USE_DOMOTICZ                        // Enable Domoticz (+3k code, +0.2k mem) - Disable by //
  #define DOMOTICZ_IN_TOPIC      "domoticz/in"  // [DomoticzInTopic]
  #define DOMOTICZ_OUT_TOPIC     "domoticz/out" // [DomoticzOutTopic]
  #define DOMOTICZ_RELAY_IDX1    0          // [DomoticzIdx1] Sonoff Relay 1 (0 = disable domoticz)
  #define DOMOTICZ_RELAY_IDX2    0          // [DomoticzIdx2] Sonoff Dual and 4CH Relay 2
  #define DOMOTICZ_RELAY_IDX3    0          // [DomoticzIdx3] Sonoff 4CH Relay 3
  #define DOMOTICZ_RELAY_IDX4    0          // [DomoticzIdx4] Sonoff 4CH Relay 4
  #define DOMOTICZ_KEY_IDX1      0          // [DomoticzKeyIdx1] Button single press (0 = disable)
  #define DOMOTICZ_KEY_IDX2      0          // [DomoticzKeyIdx2] Button double press (0 = disable)
  #define DOMOTICZ_KEY_IDX3      0          // [DomoticzKeyIdx3] Sonoff 4CH Button 3
  #define DOMOTICZ_KEY_IDX4      0          // [DomoticzKeyIdx4] Sonoff 4CH Button 4
  #define DOMOTICZ_UPDATE_TIMER  0          // [DomoticzUpdateTimer] Send relay status (0 = disable, 1 - 3600 seconds) (Optional)

// -- HTTP -----------------------------------
#define USE_WEBSERVER                       // Enable web server and wifi manager (+43k code, +2k mem) - Disable by //
  #define FRIENDLY_NAME1       "Sonoff"     // [FriendlyName1] Friendlyname up to 32 characters used by webpages and Alexa
  #define FRIENDLY_NAME2       "Sonoff2"    // [FriendlyName2] Friendlyname up to 32 characters used by Alexa
  #define FRIENDLY_NAME3       "Sonoff3"    // [FriendlyName3] Friendlyname up to 32 characters used by Alexa
  #define FRIENDLY_NAME4       "Sonoff4"    // [FriendlyName4] Friendlyname up to 32 characters used by Alexa
  #define WEB_SERVER           2            // [WebServer] Web server (0 = Off, 1 = Start as User, 2 = Start as Admin)
//  #define USE_WEMO_EMULATION                // Enable Belkin WeMo PowerSwitch emulation for Alexa (+4k code, +2k mem)
//  #define USE_HUE_EMULATION                 // Enable Hue Bridge emulation for Alexa

// -- mDNS -----------------------------------
#define USE_DISCOVERY                       // Enable mDNS for the following services (+8k code, +0.3k mem)
  #define WEBSERVER_ADVERTISE               // Provide access to webserver by name <Hostname>.local/
  #define MQTT_HOST_DISCOVERY               // Find MQTT host server (overrides MQTT_HOST if found)

// -- Time - Up to three NTP servers in your region
#define NTP_SERVER1            "pool.ntp.org"
#define NTP_SERVER2            "nl.pool.ntp.org"
#define NTP_SERVER3            "0.nl.pool.ntp.org"

// -- Time - Start Daylight Saving Time and timezone offset from UTC in minutes
#define TIME_DST               Last, Sun, Mar, 2, +120  // Last sunday in march at 02:00 +120 minutes

// -- Time - Start Standard Time and timezone offset from UTC in minutes
#define TIME_STD               Last, Sun, Oct, 3, +60   // Last sunday in october 02:00 +60 minutes

// -- Application ----------------------------
#define APP_TIMEZONE           1            // [Timezone] +1 hour (Amsterdam) (-12 .. 12 = hours from UTC, 99 = use TIME_DST/TIME_STD)
#define APP_LEDSTATE           LED_POWER    // [LedState] Function of led (LED_OFF, LED_POWER, LED_MQTTSUB, LED_POWER_MQTTSUB, LED_MQTTPUB, LED_POWER_MQTTPUB, LED_MQTT, LED_POWER_MQTT)
#define APP_PULSETIME          0            // [PulseTime] Time in 0.1 Sec to turn off power for relay 1 (0 = disabled)
#define APP_POWERON_STATE      3            // [PowerOnState] Power On Relay state (0 = Off, 1 = On, 2 = Toggle Saved state, 3 = Saved state)
#define APP_BLINKTIME          10           // [BlinkTime] Time in 0.1 Sec to blink/toggle power for relay 1
#define APP_BLINKCOUNT         10           // [BlinkCount] Number of blinks (0 = 32000)

#define TEMP_CONVERSION        0            // Convert temperature to (0 = Celsius or 1 = Fahrenheit)
#define TEMP_RESOLUTION        1            // Maximum number of decimals (0 - 3) showing sensor Temperature
#define HUMIDITY_RESOLUTION    1            // Maximum number of decimals (0 - 3) showing sensor Humidity
#define PRESSURE_RESOLUTION    1            // Maximum number of decimals (0 - 3) showing sensor Pressure

/*********************************************************************************************\
 * Sonoff Basic, Sonoff RF, Sonoff SV, Sonoff Dual, Sonoff TH, S20 Smart Socket
 *
 * >>> Select Board "Generic ESP8266 Module" and Flash Size "1M (64K SPIFFS)" <<<
\*********************************************************************************************/

#if MODULE == SONOFF                        // programming header 1:3.3V 2:rx 3:tx 4:gnd
  #define APP_NAME             "Sonoff 8266 Module"
  #define MQTT_GRPTOPIC        "sonoffs"    // [GroupTopic] MQTT Group topic
/*-------------------------------------------------------------------------------------------*/
  #define LED_PIN              13           // GPIO 13 = Green/Blue Led (0 = On, 1 = Off) - Sonoff
  #define LED_INVERTED         1            // 0 = (1 = On, 0 = Off), 1 = (0 = On, 1 = Off)
  #define REL_PIN              12           // GPIO 12 = Red Led and Relay (0 = Off, 1 = On)
  #define KEY_PIN              0            // GPIO 00 = Button
/*-------------------------------------------------------------------------------------------*\
 * Wall switch and HC-SR501
\*-------------------------------------------------------------------------------------------*/
  #define SWITCH_PIN           14           // GPIO 14 = Standard wall switch to Gnd (Sonoff_TH10A(16A), Sonoff SV)
  #define SWITCH_MODE          TOGGLE       // [SwitchMode] TOGGLE, FOLLOW, FOLLOW_INV, PUSHBUTTON or PUSHBUTTON_INV (the wall switch state)
//  #define USE_WALL_SWITCH                   // Enable the use of a standard wall switch to control the relay
/*-------------------------------------------------------------------------------------------*\
 * Single wire devices DS18B20 and DS18S20
\*-------------------------------------------------------------------------------------------*/
  #define DSB_PIN              14           // GPIO 14 = DS18x20 (Sonoff_TH10A(16A), Sonoff SV)
  // *** Option 1 - Single DS18B20 - Select either Option 1 OR Option 2
//  #define SEND_TELEMETRY_DS18B20            // Enable sending single temperature telemetry
  // *** Option 2 - Multiple DS18B20 and/or DS18S20 (needs OneWire library!)
//  #define SEND_TELEMETRY_DS18x20            // Enable sending multi temperature telemetry
/*-------------------------------------------------------------------------------------------*\
 * DHT11, DHT21, DHT22, AM2301, AM2302 and AM2321
\*-------------------------------------------------------------------------------------------*/
  #define DHT_PIN              14           // GPIO 14 = AM2301 (Sonoff_TH10A(16A), Sonoff SV)
  #define DHT_TYPE             AM2301       // DHT module type (DHT11, DHT21, DHT22, AM2301, AM2302 or AM2321)
  // *** Option 1 - No external library needed - Select either Option 1 OR Option 2
//  #define SEND_TELEMETRY_DHT                // Enable sending temperature and humidity telemetry
  // *** Option 2 - Use Adafruit DHT library - Select either Option 1 OR Option 2
//  #define SEND_TELEMETRY_DHT2               // Enable sending temperature and humidity telemetry
/*-------------------------------------------------------------------------------------------*\
 * I2C devices BH1750, BMP085, BMP180, BMP280, BME280 and HTU21D
\*-------------------------------------------------------------------------------------------*/
  #define I2C_SDA_PIN          4            // GPIO 04 = I2C SDA (Sonoff_TH10A(16A)- Needs extra hardware)
  #define I2C_SCL_PIN          14           // GPIO 14 = I2C SCL (Sonoff_TH10A(16A))
//  #define SEND_TELEMETRY_I2C                // Enable sending I2C sensor telemetry
/*-------------------------------------------------------------------------------------------*\
 * WS2812 LED Support
\*-------------------------------------------------------------------------------------------*/
//  #define WS2812_PIN           3            // GPIO 03 Serial RX reused due to DMA controlling of LEDs (Fails Serial communication!)
  #define WS2812_PIN           14           // GPIO 14 option
  #define WS2812_LEDS          30           // [Pixels] Number of LEDs
//  #define USE_WS2812

/*********************************************************************************************\
 * Sonoff Touch and Sonoff 4CH
 *
 * >>> Select Board "Generic ESP8285 Module" (=Flash Mode "DOUT") and Flash Size "1M (64K SPIFFS)" <<<
\*********************************************************************************************/

#elif MODULE == SONOFF_2                    // programming header 1:3.3V 2:rx 3:tx 4:gnd
  #define APP_NAME             "Sonoff 8285 Module"
  #define MQTT_GRPTOPIC        "sonoff2s"   // [GroupTopic] MQTT Group topic
/*-------------------------------------------------------------------------------------------*/
  #define LED_PIN              13           // GPIO 13 = Green/Blue Led (0 = On, 1 = Off) - Sonoff
  #define LED_INVERTED         1            // 0 = (1 = On, 0 = Off), 1 = (0 = On, 1 = Off)
  #define REL_PIN              12           // GPIO 12 = Red Led and Relay (0 = Off, 1 = On)
  #define KEY_PIN              0            // GPIO 00 = Button
  // Sonoff 4CH additions
  #define REL2_PIN             5            // GPIO 05 = Red Led and Relay 2 (0 = Off, 1 = On)
  #define KEY2_PIN             9            // GPIO 09 = Button 2
  #define REL3_PIN             4            // GPIO 04 = Red Led and Relay 3 (0 = Off, 1 = On)
  #define KEY3_PIN             10           // GPIO 10 = Button 3
  #define REL4_PIN             15           // GPIO 15 = Red Led and Relay 4 (0 = Off, 1 = On)
  #define KEY4_PIN             14           // GPIO 14 = Button 4
  #define SDA_PIN              2            // GPIO 02 = SDA
  #define SNS1_PIN             7            // GPIO 07 = Sensor 1 (not present)
  #define SNS2_PIN             8            // GPIO 08 = Sensor 2 (not present)
/*-------------------------------------------------------------------------------------------*\
 * Wall switch and HC-SR501
\*-------------------------------------------------------------------------------------------*/
  #define SWITCH_PIN           7            // GPIO 07 = Standard wall switch to Gnd (Sonoff 4CH - Needs extra hardware)
  #define SWITCH_MODE          TOGGLE       // [SwitchMode] TOGGLE, FOLLOW, FOLLOW_INV, PUSHBUTTON or PUSHBUTTON_INV (the wall switch state)
//  #define USE_WALL_SWITCH                   // Enable the use of a standard wall switch to control the relay
/*-------------------------------------------------------------------------------------------*\
 * Single wire devices DS18B20 and DS18S20
\*-------------------------------------------------------------------------------------------*/
  #define DSB_PIN              7            // GPIO 07 = DS18x20 (Sonoff 4CH - Needs extra hardware)
  // *** Option 1 - Single DS18B20 - Select either Option 1 OR Option 2
//  #define SEND_TELEMETRY_DS18B20            // Enable sending single temperature telemetry
  // *** Option 2 - Multiple DS18B20 and/or DS18S20 (needs OneWire library!)
//  #define SEND_TELEMETRY_DS18x20            // Enable sending multi temperature telemetry
/*-------------------------------------------------------------------------------------------*\
 * DHT11, DHT21, DHT22, AM2301, AM2302 and AM2321
\*-------------------------------------------------------------------------------------------*/
  #define DHT_PIN              7            // GPIO 07 = AM2301 (Sonoff 4CH - Needs extra hardware)
  #define DHT_TYPE             AM2301       // DHT module type (DHT11, DHT21, DHT22, AM2301, AM2302 or AM2321)
  // *** Option 1 - No external library needed - Select either Option 1 OR Option 2
//  #define SEND_TELEMETRY_DHT                // Enable sending temperature and humidity telemetry
  // *** Option 2 - Use Adafruit DHT library - Select either Option 1 OR Option 2
//  #define SEND_TELEMETRY_DHT2               // Enable sending temperature and humidity telemetry
/*-------------------------------------------------------------------------------------------*\
 * I2C devices BH1750, BMP085, BMP180, BMP280, BME280 and HTU21D
\*-------------------------------------------------------------------------------------------*/
  #define I2C_SDA_PIN          8            // GPIO 08 = I2C SDA (Sonoff 4CH - Needs extra hardware)
  #define I2C_SCL_PIN          7            // GPIO 07 = I2C SCL (Sonoff 4CH - Needs extra hardware)
//  #define SEND_TELEMETRY_I2C                // Enable sending I2C sensor telemetry
/*-------------------------------------------------------------------------------------------*\
 * WS2812 LED Support
\*-------------------------------------------------------------------------------------------*/
//  #define WS2812_PIN           3            // GPIO 03 Serial RX reused due to DMA controlling of LEDs (Fails Serial communication!)
  #define WS2812_PIN           7           // GPIO 07 option
  #define WS2812_LEDS          30           // [Pixels] Number of LEDs
//  #define USE_WS2812

/*********************************************************************************************\
 * Sonoff Pow
 *
 * >>> Select Board "Generic ESP8266 Module" and Flash Size "1M (64K SPIFFS)" or "4M (1M SPIFFS)" <<<
\*********************************************************************************************/

#elif MODULE == SONOFF_POW                  // programming header 1:3.3V 2:rx 3:tx 4:gnd
  #define APP_NAME             "Sonoff Pow Module"
  #define MQTT_GRPTOPIC        "pows"       // [GroupTopic] MQTT Group topic
  #define USE_POWERMONITOR                  // Enable Power Monitoring
//  #define USE_POWERCALIBRATION              // Enable setting Calibration parameters by user commands
/*-------------------------------------------------------------------------------------------*/
  #define LED_PIN              15           // GPIO 15 = Green Led (0 = On, 1 = Off) - Sonoff
  #define LED_INVERTED         0            // 0 = (1 = On, 0 = Off), 1 = (0 = On, 1 = Off)
  #define REL_PIN              12           // GPIO 12 = Red Led and Relay (0 = Off, 1 = On)
  #define KEY_PIN              0            // GPIO 00 = Button
/*-------------------------------------------------------------------------------------------*/
  #define HLW_SEL              5            // GPIO 05 = HLW8012 Sel output (Sonoff Pow)
  #define HLW_CF1              13           // GPIO 13 = HLW8012 CF1 voltage / current (Sonoff Pow)
  #define HLW_CF               14           // GPIO 14 = HLW8012 CF power (Sonoff Pow)
  #define SEND_TELEMETRY_ENERGY             // Enable sending energy telemetry

/*********************************************************************************************\
 * Motor Clockwise/Anticlockwise
 *
 * >>> Select Board "Generic ESP8266 Module" and Flash Size "1M (64K SPIFFS)" <<<
\*********************************************************************************************/

#elif MODULE == MOTOR_CAC                   // programming pins 6:3.3V 7:rx 8:tx 9:gnd of PSA-B
  #define APP_NAME             "Motor C/AC Module"
  #define MQTT_GRPTOPIC        "motors"     // [GroupTopic] MQTT Group topic
/*-------------------------------------------------------------------------------------------*/
  #define LED_PIN              13           // GPIO 13 = Green/Blue Led (0 = On, 1 = Off) - Sonoff
  #define LED_INVERTED         1            // 0 = (1 = On, 0 = Off), 1 = (0 = On, 1 = Off)
  #define REL_PIN              12           // GPIO 12 = Red Leds and Relays (0 = Off, 1 = On)
  #define KEY_PIN              0            // GPIO 00 = Button

/*********************************************************************************************\
 * Electrodragon
 *
 * >>> Select Board "Generic ESP8266 Module" and Flash Size "1M (64K SPIFFS)" or "4M (1M SPIFFS)" <<<
\*********************************************************************************************/

#elif MODULE == ELECTRO_DRAGON              // programming header 5V/3V/gnd/
  #define APP_NAME             "ElectroDragon Module"
  #define MQTT_GRPTOPIC        "dragons"    // [GroupTopic] MQTT Group topic
/*-------------------------------------------------------------------------------------------*/
  #define LED_PIN              16           // GPIO 16 = Led (0 = Off, 1 = On)
  #define LED_INVERTED         0            // 0 = (1 = On, 0 = Off), 1 = (0 = On, 1 = Off)
  #define REL_PIN              13           // GPIO 13 = Red Led and Relay 1 (0 = Off, 1 = On)
  #define KEY_PIN              2            // GPIO 02 = Button 1
  #define REL2_PIN             12           // GPIO 12 = Red Led and Relay 2 (0 = Off, 1 = On)
  #define KEY2_PIN             0            // GPIO 00 = Button 2
/*-------------------------------------------------------------------------------------------*\
 * Wall switch and HC-SR501
\*-------------------------------------------------------------------------------------------*/
  #define SWITCH_PIN           14           // GPIO 14 = Standard wall switch to Gnd
  #define SWITCH_MODE          TOGGLE       // [SwitchMode] TOGGLE, FOLLOW, FOLLOW_INV, PUSHBUTTON or PUSHBUTTON_INV (the wall switch state)
//  #define USE_WALL_SWITCH                   // Enable the use of a standard wall switch to control the relay
/*-------------------------------------------------------------------------------------------*\
 * Single wire devices DS18B20 and DS18S20
\*-------------------------------------------------------------------------------------------*/
  #define DSB_PIN              4            // GPIO 04 = DS18x20
  // *** Option 1 - Single DS18B20 - Select either Option 1 OR Option 2
//  #define SEND_TELEMETRY_DS18B20            // Enable sending single temperature telemetry
  // *** Option 2 - Multiple DS18B20 and/or DS18S20 (needs OneWire library!)
//  #define SEND_TELEMETRY_DS18x20            // Enable sending multi temperature telemetry
/*-------------------------------------------------------------------------------------------*\
 * DHT11, DHT21, DHT22, AM2301, AM2302 and AM2321
\*-------------------------------------------------------------------------------------------*/
  #define DHT_PIN              14           // GPIO 14 = DHT22
  #define DHT_TYPE             DHT22        // DHT module type (DHT11, DHT21, DHT22, AM2301, AM2302 or AM2321)
  // *** Option 1 - No external library needed - Select either Option 1 OR Option 2
//  #define SEND_TELEMETRY_DHT                // Enable sending temperature and humidity telemetry
  // *** Option 2 - Use Adafruit DHT library - Select either Option 1 OR Option 2
//  #define SEND_TELEMETRY_DHT2               // Enable sending temperature and humidity telemetry
/*-------------------------------------------------------------------------------------------*\
 * I2C devices BH1750, BMP085, BMP180, BMP280, BME280 and HTU21D
\*-------------------------------------------------------------------------------------------*/
  #define I2C_SDA_PIN          4            // GPIO 04 = I2C SDA (Sonoff_TH10A(16A)- Needs extra hardware)
  #define I2C_SCL_PIN          14           // GPIO 14 = I2C SCL (Sonoff_TH10A(16A))
//  #define SEND_TELEMETRY_I2C                // Enable sending I2C sensor telemetry
/*-------------------------------------------------------------------------------------------*\
 * WS2812 LED Support
\*-------------------------------------------------------------------------------------------*/
//  #define WS2812_PIN           3            // GPIO 03 Serial RX reused due to DMA controlling of LEDs (Fails Serial communication!)
  #define WS2812_PIN           14           // GPIO 14 option
  #define WS2812_LEDS          30           // [Pixels] Number of LEDs
//  #define USE_WS2812

/*********************************************************************************************\
 * No user configurable items below
\*********************************************************************************************/

#else
  #error "Select either module SONOFF, SONOFF_85, SONOFF_POW, MOTOR_CAC or ELECTRO_DRAGON"
#endif

#if defined(USE_WEMO_EMULATION) && defined(USE_HUE_EMULATION)
  #error "Select either USE_WEMO_EMULATION or USE_HUE_EMULATION"
#endif

#if defined(SEND_TELEMETRY_DS18B20) && defined(SEND_TELEMETRY_DS18x20)
  #error "Select either SEND_TELEMETRY_DS18B20 or SEND_TELEMETRY_DS18x20"
#endif

#if defined(SEND_TELEMETRY_DHT) && defined(SEND_TELEMETRY_DHT2)
  #error "Select either SEND_TELEMETRY_DHT or SEND_TELEMETRY_DHT2"
#endif

#if defined(SEND_TELEMETRY_DS18B20) || defined(SEND_TELEMETRY_DS18x20)
  #define ST_DSB
#endif
#if defined(SEND_TELEMETRY_DHT) || defined(SEND_TELEMETRY_DHT2)
  #define ST_DHT
#endif

#if ((defined(ST_DSB) && defined(ST_DHT)) || (defined(ST_DSB) && defined(USE_WALL_SWITCH)) || (defined(ST_DHT) && defined(USE_WALL_SWITCH)))
#if ((DSB_PIN == DHT_PIN) || (DSB_PIN == SWITCH_PIN) || (DHT_PIN == SWITCH_PIN))
  #error "Select either SEND_TELEMETRY_DS18B20 or SEND_TELEMETRY_DHT or USE_WALL_SWITCH or use different GPIOs"
#endif
#endif

#if defined(ST_DSB) && defined(SEND_TELEMETRY_I2C)
#if ((DSB_PIN == I2C_SDA_PIN) || (DSB_PIN == I2C_SCL_PIN))
  #error "Select either SEND_TELEMETRY_DS18B20 or SEND_TELEMETRY_I2C or use different GPIOs"
#endif
#endif

#if defined(ST_DHT) && defined(SEND_TELEMETRY_I2C)
#if ((DHT_PIN == I2C_SDA_PIN) || (DHT_PIN == I2C_SCL_PIN))
  #error "Select either SEND_TELEMETRY_DHT or SEND_TELEMETRY_I2C or use different GPIOs"
#endif
#endif

#if defined(USE_WALL_SWITCH) && defined(SEND_TELEMETRY_I2C)
#if ((SWITCH_PIN == I2C_SDA_PIN) || (SWITCH_PIN == I2C_SCL_PIN))
  #error "Select either USE_WALL_SWITCH or SEND_TELEMETRY_I2C or use different GPIOs"
#endif
#endif
