/*********************************************************************************************\
 * User specific configuration parameters
 * 
 * Select hardware MODULE:
 *  SONOFF          = Sonoff, Sonoff TH 10A and Sonoff TH 16A
 *  ELECTRO_DRAGON  = Electro Dragon (Relay 2 only)
 * 
\*********************************************************************************************/

#define MODULE                 SONOFF       // Hardware module type (SONOFF or ELECTRO_DRAGON)

#define PROJECT                "sonoff"     // PROJECT is used as the default topic delimiter and OTA file name
                                            // As an IDE restriction it needs to be the same as the main .ino file
                                            
#define CFG_HOLDER             0x20160520   // Change this value to load default configurations

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

#define SUB_PREFIX             "cmnd"       // Sonoff devices subscribe to:- cmnd/MQTT_TOPIC and cmnd/MQTT_GRPTOPIC
#define PUB_PREFIX             "stat"       // Sonoff devices publish to:- stat/MQTT_TOPIC
#define PUB_PREFIX2            "tele"       // Sonoff devices publish telemetry data to:- tele/MQTT_TOPIC/UPTIME, POWER/LIGHT and TIME
                                            // May be named the same as PUB_PREFIX
#define MQTT_TOPIC             PROJECT
#define MQTT_GRPTOPIC          PROJECT"s"   // Group topic

// MQTT - Telemetry
#define TELE_PERIOD            300          // Telemetry (0 = disable, 2 - 3600 seconds)
//#define SEND_TELEMETRY_UPTIME               // Enable sending uptime telemetry (if disabled will still send hourly message)
//#define SEND_TELEMETRY_DS18B20              // Enable sending DS18B20 temperature telemetry
//#define SEND_TELEMETRY_DHT                  // Enable sending DHT11, DHT21, DHT22, AM2301, AM2302 or AM2321 temperature and humidity telemetry
#define SEND_TELEMETRY_POWER                // Enable sending power telemetry

// HTTP
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

#if MODULE == SONOFF                        // programming header 1:3.3V 2:rx 3:tx 4:gnd
  #define APP_NAME             "Sonoff module"
  #define LED_PIN              13           // GPIO 13 = Green Led (0 = On, 1 = Off) - Sonoff
  #define LED_INVERTED         1            // 0 = (1 = On, 0 = Off), 1 = (0 = On, 1 = Off)
  #define REL_PIN              12           // GPIO 12 = Red Led and Relay (0 = Off, 1 = On)
  #define KEY_PIN              0            // GPIO 00 = Button
  #define DHT_PIN              14           // GPIO 14 = TEM1 - DHT22 (Sonoff_TH10A(16A))
  #define DHT_TYPE             DHT11        // DHT module type (DHT11, DHT21, DHT22, AM2301, AM2302 or AM2321)
  #define DSB_PIN              4            // GPIO 04 = TEM2 - DS18B20 (Sonoff_TH10A(16A))
  
#elif MODULE == ELECTRO_DRAGON              // programming header 5V/3V/gnd/
  #define APP_NAME             "ElectroDragon module"
  #define LED_PIN              16           // GPIO 16 = Led (0 = Off, 1 = On)
  #define LED_INVERTED         0            // 0 = (1 = On, 0 = Off), 1 = (0 = On, 1 = Off)
  #define REL_PIN              13           // GPIO 13 = Red Led and Relay 1 (0 = Off, 1 = On)
  #define KEY_PIN              2            // GPIO 02 = Button 1
  #define REL2_PIN             12           // GPIO 12 = Red Led and Relay 2 (0 = Off, 1 = On)
  #define KEY2_PIN             0            // GPIO 00 = Button 2
  #define DHT_PIN              14           // GPIO 14 = DHT22
  #define DHT_TYPE             DHT22        // DHT module type (DHT11, DHT21, DHT22, AM2301, AM2302 or AM2321)
  #define DSB_PIN              4            // GPIO 04 = DS18B20
  
#else
  #error "Select either module SONOFF or ELECTRO_DRAGON"
#endif

