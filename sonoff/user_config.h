/* 
 * user_config.h
 * 
 * User specific configuration parameters
 */

#define PROJECT                "sonoff"     // PROJECT is used as the default topic delimiter and OTA file name
#define CFG_HOLDER             0x20160520   // Change this value to load default configurations

// Wifi
#define STA_SSID               "indebuurt3"      // Wifi SSID
#define STA_PASS               "VnsqrtnrsddbrN"  // Wifi password
#define WIFI_HOSTNAME          "esp-%06x-%s"

// Syslog
#define SYS_LOG_HOST           "domus1"
#define SYS_LOG_PORT           514
#define SYS_LOG_LEVEL          LOG_LEVEL_NONE
#define SERIAL_LOG_LEVEL       LOG_LEVEL_NONE

// Ota
#if (ARDUINO >= 168)
  #define OTA_URL              "http://domus1:80/api/arduino/"PROJECT".ino.bin"
#else
  #define OTA_URL              "http://domus1:80/api/arduino/"PROJECT".cpp.bin"
#endif

// MQTT
#define MQTT_HOST              "domus1"
#define MQTT_PORT              1883

#define MQTT_CLIENT_ID         "DVES_%06X"  // Also fall back topic using Chip Id = last 6 characters of MAC address
#define MQTT_USER              "DVES_USER"
#define MQTT_PASS              "DVES_PASS"

#define SUB_PREFIX             "cmnd"       // Sonoff devices subscribe to:- cmnd/MQTT_TOPIC and cmnd/MQTT_GRPTOPIC
#define PUB_PREFIX             "stat"       // Sonoff devices publish to:- stat/MQTT_TOPIC
#define MQTT_TOPIC             PROJECT
#define MQTT_GRPTOPIC          PROJECT"s"   // Group topic

// Application
#define MQTT_SUBTOPIC          "POWER"
#define APP_TIMEZONE           1            // +1 hour (Amsterdam)
#define APP_POWER              0            // Saved power state Off
#define APP_LEDSTATE           0            // Do not show power state (1 = Show power state)

