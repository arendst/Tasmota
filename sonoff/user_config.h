/*********************************************************************************************\
 * User specific configuration parameters
\*********************************************************************************************/

// Enable only one out of three MODULE defines below
#define MODULE                 SONOFF            // Sonoff, Sonoff SV, Sonoff Dual, Sonoff TH 10A/16A, S20 Smart Socket, 4 Channel
//#define MODULE                 SONOFF_POW        // Sonoff Pow
//#define MODULE                 ELECTRO_DRAGON    // Electro Dragon Wifi IoT Relay Board Based on ESP8266  

#define PROJECT                "sonoff"     // PROJECT is used as the default topic delimiter and OTA file name
                                            // As an IDE restriction it needs to be the same as the main .ino file
                                            
#define CFG_HOLDER             0x20160520   // Change this value to load default configuration parameters
#define SAVE_DATA              1            // Save changed parameters to Flash (0 = disable, 1 - 3600 seconds)
#define SAVE_STATE             1            // Save changed power state to Flash (0 = disable, 1 = enable)

// Wifi
#define STA_SSID               "indebuurt3"      // Wifi SSID
#define STA_PASS               "VnsqrtnrsddbrN"  // Wifi password
#define WIFI_HOSTNAME          "%s-%04d"         // Expands to <MQTT_TOPIC>-<last 4 decimal chars of MAC address>
#define WIFI_CONFIG_TOOL       WIFI_WPSCONFIG    // Default tool if wifi fails to connect (WIFI_SMARTCONFIG, WIFI_MANAGER or WIFI_WPSCONFIG)

// Syslog
#define SYS_LOG_HOST           "domus1"
#define SYS_LOG_PORT           514
#define SYS_LOG_LEVEL          LOG_LEVEL_NONE
#define SERIAL_LOG_LEVEL       LOG_LEVEL_INFO
#define WEB_LOG_LEVEL          LOG_LEVEL_INFO

// Ota
#if (ARDUINO >= 168)
  #define OTA_URL              "http://domus1:80/api/arduino/" PROJECT ".ino.bin"
#else
  #define OTA_URL              "http://domus1:80/api/arduino/" PROJECT ".cpp.bin"
#endif

// MQTT
#define MQTT_HOST              "domus1"
#define MQTT_PORT              1883

#define MQTT_CLIENT_ID         "DVES_%06X"  // Also fall back topic using Chip Id = last 6 characters of MAC address
#define MQTT_USER              "DVES_USER"
#define MQTT_PASS              "DVES_PASS"

#define SUB_PREFIX             "cmnd"       // Sonoff devices subscribe to:- SUB_PREFIX/MQTT_TOPIC and SUB_PREFIX/MQTT_GRPTOPIC
#define PUB_PREFIX             "stat"       // Sonoff devices publish to:- PUB_PREFIX/MQTT_TOPIC
#define PUB_PREFIX2            "tele"       // Sonoff devices publish telemetry data to:- PUB_PREFIX2/MQTT_TOPIC/UPTIME, POWER/LIGHT and TIME
                                            // May be named the same as PUB_PREFIX
#define MQTT_TOPIC             PROJECT
#define MQTT_BUTTON_RETAIN     0            // Button may send retain flag (0 = off, 1 = on)

#define MESSAGE_FORMAT         LEGACY       // MQTT Message Format (LEGACY or JSON)
#define MQTT_STATUS_ON         "ON"         // Status result when turned on (needs to be a string like "1" or "On")
#define MQTT_STATUS_OFF        "OFF"        // Status result when turned off (needs to be a string like "0" or "Off")

// Domoticz
#define USE_DOMOTICZ                        // Enable Domoticz (+3k code, +0.2k mem) - Disable by //
#define DOMOTICZ_IN_TOPIC      "domoticz/in"
#define DOMOTICZ_OUT_TOPIC     "domoticz/out"
#define DOMOTICZ_RELAY_IDX1    0            // Sonoff Relay 1 (0 = disable domoticz)
#define DOMOTICZ_RELAY_IDX2    0            // Sonoff Dual Relay 2
#define DOMOTICZ_KEY_IDX1      0            // Button single press (0 = disable)
#define DOMOTICZ_KEY_IDX2      0            // Button double press (0 = disable)
#define DOMOTICZ_UPDATE_TIMER  0            // Send relay status (0 = disable, 1 - 3600 seconds) (Optional)

// MQTT - Telemetry
#define TELE_PERIOD            300          // Telemetry (0 = disable, 10 - 3600 seconds)
#define SEND_TELEMETRY_UPTIME               // Enable sending uptime telemetry (if disabled will still send hourly message)
#define SEND_TELEMETRY_RSSI                 // Enable sending wifi RSSI telemetry
#define SEND_TELEMETRY_POWER                // Enable sending power telemetry

// HTTP
#define USE_WEBSERVER                       // Enable web server and wifi manager (+37k code, +2k mem) - Disable by //
#define WEB_SERVER             2            // Web server (0 = Off, 1 = Start as User, 2 = Start as Admin)

// Time - Up to three NTP servers in your region
#define NTP_SERVER1            "pool.ntp.org"
#define NTP_SERVER2            "nl.pool.ntp.org"
#define NTP_SERVER3            "0.nl.pool.ntp.org"

// Time - Start Daylight Saving Time and timezone offset from UTC in minutes
#define TIME_DST               Last, Sun, Mar, 2, +120  // Last sunday in march at 02:00 +120 minutes

// Time - Start Standard Time and timezone offset from UTC in minutes
#define TIME_STD               Last, Sun, Oct, 3, +60   // Last sunday in october 02:00 +60 minutes

// Application
#define APP_TIMEZONE           1            // +1 hour (Amsterdam) (-12 .. 12 = hours from UTC, 99 = use TIME_DST/TIME_STD)
#define APP_LEDSTATE           1            // Do not show power state (1 = Show power state)

#define TEMP_RESOLUTION        1            // Maximum number of decimals (0 - 3) showing sensor Temperature
#define HUMIDITY_RESOLUTION    1            // Maximum number of decimals (0 - 3) showing sensor Humidity
#define PRESSURE_RESOLUTION    1            // Maximum number of decimals (0 - 3) showing sensor Pressure

/*********************************************************************************************\
 * Sonoff specific paremeters
\*********************************************************************************************/

#if MODULE == SONOFF                        // programming header 1:3.3V 2:rx 3:tx 4:gnd
  #define APP_NAME             "Sonoff module"
  #define MQTT_GRPTOPIC        "sonoffs"    // Group topic
/*-------------------------------------------------------------------------------------------*/
  #define LED_PIN              13           // GPIO 13 = Green/Blue Led (0 = On, 1 = Off) - Sonoff
  #define LED_INVERTED         1            // 0 = (1 = On, 0 = Off), 1 = (0 = On, 1 = Off)
  #define REL_PIN              12           // GPIO 12 = Red Led and Relay (0 = Off, 1 = On)
  #define KEY_PIN              0            // GPIO 00 = Button
/*-------------------------------------------------------------------------------------------*\
 * Wall switch and HC-SR501
\*-------------------------------------------------------------------------------------------*/
  #define SWITCH_PIN           14           // GPIO 14 = Standard wall switch to Gnd (Sonoff_TH10A(16A), Sonoff SV)
  #define SWITCH_MODE          TOGGLE       // TOGGLE, FOLLOW or FOLLOW_INV (the wall switch state)
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
 * I2C devices BMP085, BMP180, BMP280, BME280 and HTU21D
\*-------------------------------------------------------------------------------------------*/
  #define I2C_SDA_PIN          4            // GPIO 4 = I2C SDA (Sonoff_TH10A(16A)- Needs extra hardware)
  #define I2C_SCL_PIN          14           // GPIO 14 = I2C SCL (Sonoff_TH10A(16A))
//  #define SEND_TELEMETRY_I2C                // Enable sending I2C sensor telemetry
  
/*********************************************************************************************\
 * Sonoff Pow specific parameters
\*********************************************************************************************/

#elif MODULE == SONOFF_POW                  // programming header 1:3.3V 2:rx 3:tx 4:gnd
  #define APP_NAME             "Sonoff Pow module"
  #define MQTT_GRPTOPIC        "pows"       // Group topic
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
 * Electrodragon specific paremeters
\*********************************************************************************************/

#elif MODULE == ELECTRO_DRAGON              // programming header 5V/3V/gnd/
  #define APP_NAME             "ElectroDragon module"
  #define MQTT_GRPTOPIC        "dragons"    // Group topic
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
  #define SWITCH_MODE          TOGGLE       // TOGGLE, FOLLOW or FOLLOW_INV (the wall switch state)
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
 * I2C devices BMP085, BMP180, BMP280, BME280 and HTU21D
\*-------------------------------------------------------------------------------------------*/
  #define I2C_SDA_PIN          4            // GPIO 4 = I2C SDA (Sonoff_TH10A(16A)- Needs extra hardware)
  #define I2C_SCL_PIN          14           // GPIO 14 = I2C SCL (Sonoff_TH10A(16A))
//  #define SEND_TELEMETRY_I2C                // Enable sending I2C sensor telemetry

/*********************************************************************************************\
 * No user configurable items below
\*********************************************************************************************/
 
#else
  #error "Select either module SONOFF, SONOFF_POW or ELECTRO_DRAGON"
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

