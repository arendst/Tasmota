/*
 * Sonoff and ElectroDragon by Theo Arends
 * 
 * ====================================================
 * Prerequisites:
 *   - Change libraries/PubSubClient/src/PubSubClient.h
 *       #define MQTT_MAX_PACKET_SIZE 400
 *       
 *   - Select IDE Tools - Flash size: "1M (64K SPIFFS)"
 * ====================================================
*/

#define VERSION                0x02000F00   // 2.0.15

#define SONOFF                 1            // Sonoff, Sonoff SV, Sonoff Dual, Sonoff TH 10A/16A, S20 Smart Socket, 4 Channel
#define SONOFF_POW             9            // Sonoff Pow
#define ELECTRO_DRAGON         10           // Electro Dragon Wifi IoT Relay Board Based on ESP8266

#define DHT11                  11
#define DHT21                  21
#define DHT22                  22
#define AM2301                 21
#define AM2302                 22
#define AM2321                 22

enum log_t   {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE, LOG_LEVEL_ALL};
enum week_t  {Last, First, Second, Third, Fourth}; 
enum dow_t   {Sun=1, Mon, Tue, Wed, Thu, Fri, Sat};
enum month_t {Jan=1, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec};
enum wifi_t  {WIFI_STATUS, WIFI_SMARTCONFIG, WIFI_MANAGER, WIFI_WPSCONFIG};
enum msgf_t  {LEGACY, JSON, MAX_FORMAT};

#include "user_config.h"

/*********************************************************************************************\
 * Enable feature by removing leading // or disable feature by adding leading //
\*********************************************************************************************/

//#define USE_SPIFFS                          // Switch persistent configuration from flash to spiffs (+24k code, +0.6k mem)

/*********************************************************************************************\
 * Not released yet
\*********************************************************************************************/

#if MODULE == SONOFF_POW
//  #define FEATURE_POWER_LIMIT
#endif
#define MAX_POWER_LIMIT_HOLD         10     // Time in SECONDS to allow max agreed power
#define MAX_POWER_LIMIT_WINDOW       30     // Time in SECONDS to disable allow max agreed power
#define MAX_SAFE_POWER_LIMIT_HOLD    10     // Time in SECONDS to allow max unit safe power
#define MAX_SAFE_POWER_LIMIT_WINDOW  30     // Time in MINUTES to disable allow max unit safe power
#define MAX_POWER_LIMIT_RETRY        5      // Retry count allowing agreed power limit overflow

/*********************************************************************************************\
 * No user configurable items below
\*********************************************************************************************/

#define SONOFF_DUAL            2            // (iTEAD PSB)
#define CHANNEL_3              3            // iTEAD PSB
#define CHANNEL_4              4            // iTEAD PSB
#define CHANNEL_5              5            // Future use
#define CHANNEL_6              6            // Future use
#define CHANNEL_7              7            // Future use
#define CHANNEL_8              8            // Future use

#define DEF_WIFI_HOSTNAME      "%s-%04d"    // Expands to <MQTT_TOPIC>-<last 4 decimal chars of MAC address>
#define DEF_MQTT_CLIENT_ID     "DVES_%06X"  // Also fall back topic using Chip Id = last 6 characters of MAC address

#define HLW_PREF_PULSE         12345        // was 4975us = 201Hz = 1000W
#define HLW_UREF_PULSE         1950         // was 1666us = 600Hz = 220V
#define HLW_IREF_PULSE         3500         // was 1666us = 600Hz = 4.545A

#define MESSAGE_FORMAT         LEGACY       // LEGACY or JSON
#define MQTT_UNITS             0            // Default do not show value units (Hr, Sec, V, A, W etc.)
#define MQTT_SUBTOPIC          "POWER"      // Default MQTT subtopic (POWER or LIGHT)
#define APP_POWER              0            // Default saved power state Off
#define MAX_DEVICE             1            // Max number of devices

#define STATES                 10           // loops per second
#define MQTT_RETRY_SECS        10           // Seconds to retry MQTT connection
#define SYSLOG_TIMER           600          // Seconds to restore syslog_level

#define INPUT_BUFFER_SIZE      128          // Max number of characters in serial buffer
#define TOPSZ                  60           // Max number of characters in topic string
#define MESSZ                  300          // Max number of characters in message string (Domoticz string)
#define LOGSZ                  128          // Max number of characters in log string
#define MAX_LOG_LINES          80           // Max number of lines in weblog

#define APP_BAUDRATE           115200       // Default serial baudrate

#ifdef USE_POWERMONITOR
  #define MAX_STATUS           9
#else
  #define MAX_STATUS           7
#endif

#define DOMOTICZ_RELAY_IDX3    0            // Relay 3 (4 Channel)
#define DOMOTICZ_RELAY_IDX4    0            // Relay 4 (4 Channel)
#define DOMOTICZ_KEY_IDX3      0            // Button 3 (4 Channel)
#define DOMOTICZ_KEY_IDX4      0            // Button 4 (4 Channel)

enum butt_t {PRESSED, NOT_PRESSED};

#include "support.h"                        // Global support
#include <Ticker.h>                         // RTC
#include <ESP8266WiFi.h>                    // MQTT, Ota, WifiManager
#include <ESP8266HTTPClient.h>              // MQTT, Ota
#include <ESP8266httpUpdate.h>              // Ota
#include <PubSubClient.h>                   // MQTT
#ifdef USE_WEBSERVER
  #include <ESP8266WebServer.h>             // WifiManager, Webserver
  #include <DNSServer.h>                    // WifiManager
#endif  // USE_WEBSERVER
#ifdef USE_SPIFFS
  #include <FS.h>                           // Config
#endif

typedef void (*rtcCallback)();

extern "C" uint32_t _SPIFFS_start;
extern "C" uint32_t _SPIFFS_end;

#define MAX_BUTTON_COMMANDS    5            // Max number of button commands supported

const char commands[MAX_BUTTON_COMMANDS][14] PROGMEM = {
  {"wificonfig 1"},   // Press button three times
  {"wificonfig 2"},   // Press button four times
  {"wificonfig 3"},   // Press button five times
  {"restart 1"},      // Press button six times
  {"upgrade 1"}};     // Press button seven times

struct SYSCFG {
  unsigned long cfg_holder;
  unsigned long saveFlag;
  unsigned long version;
  byte          seriallog_level;
  byte          syslog_level;
  char          syslog_host[32];
  char          sta_ssid[32];
  char          sta_pwd[64];
  char          otaUrl[80];
  char          mqtt_host[32];
  char          mqtt_grptopic[32];
  char          mqtt_topic[32];
  char          mqtt_topic2[32];
  char          mqtt_subtopic[32];
  int8_t        timezone;
  uint8_t       power;
  uint8_t       ledstate;
  uint16_t      mqtt_port;
  char          mqtt_client[33];
  char          mqtt_user[33];
  char          mqtt_pwd[33];
  uint8_t       webserver;
  unsigned long bootcount;
  char          hostname[33];
  uint16_t      syslog_port;
  byte          weblog_level;
  uint16_t      tele_period;
  uint8_t       sta_config;
  int16_t       savedata;
  byte          model;
  byte          mqtt_retain;
  byte          savestate;
  unsigned long hlw_pcal;
  unsigned long hlw_ucal;
  unsigned long hlw_ical;
  unsigned long hlw_esave;
  byte          mqtt_units;
  uint16_t      hlw_pmin;
  uint16_t      hlw_pmax;
  uint16_t      hlw_umin;
  uint16_t      hlw_umax;
  uint16_t      hlw_imin;
  uint16_t      hlw_imax;
  uint16_t      hlw_mpl;    // MaxPowerLimit
  uint16_t      hlw_mplh;   // MaxPowerLimitHold
  uint16_t      hlw_mplw;   // MaxPowerLimitWindow
  uint16_t      hlw_mspl;   // MaxSafePowerLimit
  uint16_t      hlw_msplh;  // MaxSafePowerLimitHold
  uint16_t      hlw_msplw;  // MaxSafePowerLimitWindow
  uint16_t      hlw_mdpl;   // MaxDailyPowerLimit
  uint16_t      hlw_mdpls;  // MaxDailyPowerLimitStart
  char          domoticz_in_topic[33];  
  char          domoticz_out_topic[33];  
  uint16_t      domoticz_update_timer;  
  unsigned long domoticz_relay_idx[4];
  unsigned long domoticz_key_idx[4];
  byte          message_format;
} sysCfg;

struct TIME_T {
  uint8_t       Second;
  uint8_t       Minute;
  uint8_t       Hour;
  uint8_t       Wday;      // day of week, sunday is day 1
  uint8_t       Day;
  uint8_t       Month;
  char          MonthName[4];
  uint16_t      Year;
  unsigned long Valid;
} rtcTime;

struct TimeChangeRule
{
  uint8_t       week;      // 1=First, 2=Second, 3=Third, 4=Fourth, or 0=Last week of the month
  uint8_t       dow;       // day of week, 1=Sun, 2=Mon, ... 7=Sat
  uint8_t       month;     // 1=Jan, 2=Feb, ... 12=Dec
  uint8_t       hour;      // 0-23
  int           offset;    // offset from UTC in minutes
};

TimeChangeRule myDST = { TIME_DST };  // Daylight Saving Time
TimeChangeRule mySTD = { TIME_STD };  // Standard Time

int Baudrate = APP_BAUDRATE;          // Serial interface baud rate
byte SerialInByte;                    // Received byte
int SerialInByteCounter = 0;          // Index in receive buffer
char serialInBuf[INPUT_BUFFER_SIZE + 2];  // Receive buffer
byte Hexcode = 0;                     // Sonoff dual input flag
uint16_t ButtonCode = 0;              // Sonoff dual received code
int16_t savedatacounter;              // Counter and flag for config save to Flash or Spiffs
char Version[16];                     // Version string from VERSION define
char Hostname[33];                    // Composed Wifi hostname
char MQTTClient[33];                  // Composed MQTT Clientname
uint8_t mqttcounter = 0;              // MQTT connection retry counter
unsigned long timerxs = 0;            // State loop timer
int state = 0;                        // State per second flag
int mqttflag = 1;                     // MQTT connection messages flag
int otaflag = 0;                      // OTA state flag
int otaok;                            // OTA result
int restartflag = 0;                  // Sonoff restart flag
int wificheckflag = WIFI_STATUS;      // Wifi state flag
int uptime = 0;                       // Current uptime in hours
int tele_period = 0;                  // Tele period timer
String Log[MAX_LOG_LINES];            // Web log buffer
byte logidx = 0;                      // Index in Web log buffer
byte Maxdevice = MAX_DEVICE;          // Max number of devices supported
int status_update_timer = 0;          // Refresh initial status

WiFiClient espClient;                 // Wifi Client
PubSubClient mqttClient(espClient);   // MQTT Client
WiFiUDP portUDP;                      // UDP Syslog

uint8_t power;                        // Current copy of sysCfg.power
byte syslog_level;                    // Current copy of sysCfg.syslog_level
uint16_t syslog_timer = 0;            // Timer to re-enable syslog_level

int blinks = 1;                       // Number of LED blinks
uint8_t blinkstate = 0;               // LED state

uint8_t lastbutton = NOT_PRESSED;     // Last button state
uint8_t holdcount = 0;                // Timer recording button hold
uint8_t multiwindow = 0;              // Max time between button presses to record press count
uint8_t multipress = 0;               // Number of button presses within multiwindow

#ifdef USE_POWERMONITOR
  byte hlw_pminflg = 0;
  byte hlw_pmaxflg = 0;
  byte hlw_uminflg = 0;
  byte hlw_umaxflg = 0;
  byte hlw_iminflg = 0;
  byte hlw_imaxflg = 0;
  byte power_steady_cntr;
#ifdef FEATURE_POWER_LIMIT
  byte hlw_mdpls_flag = 0;
  byte hlw_mplr_counter = 0;
  uint16_t hlw_mplh_counter = 0;
  uint16_t hlw_mplw_counter = 0;
#endif  // FEATURE_POWER_LIMIT
#endif  // USE_POWERMONITOR

#ifdef USE_DOMOTICZ
  int domoticz_update_timer = 0;
  byte domoticz_update_flag;  
#endif  // USE_DOMOTICZ 

/********************************************************************************************/

void CFG_Default()
{
  addLog_P(LOG_LEVEL_INFO, PSTR("Config: Use default configuration"));
  memset(&sysCfg, 0x00, sizeof(SYSCFG));
  
  sysCfg.cfg_holder = CFG_HOLDER;
  sysCfg.saveFlag = 0;
  sysCfg.version = VERSION;
  sysCfg.bootcount = 0;
  sysCfg.savedata = SAVE_DATA;
  sysCfg.savestate = SAVE_STATE;
  sysCfg.message_format = MESSAGE_FORMAT;
  sysCfg.weblog_level = WEB_LOG_LEVEL;
  sysCfg.seriallog_level = SERIAL_LOG_LEVEL;
  sysCfg.syslog_level = SYS_LOG_LEVEL;
  strlcpy(sysCfg.syslog_host, SYS_LOG_HOST, sizeof(sysCfg.syslog_host));
  sysCfg.syslog_port = SYS_LOG_PORT;
  strlcpy(sysCfg.sta_ssid, STA_SSID, sizeof(sysCfg.sta_ssid));
  strlcpy(sysCfg.sta_pwd, STA_PASS, sizeof(sysCfg.sta_pwd));
  sysCfg.sta_config = WIFI_CONFIG_TOOL;
  strlcpy(sysCfg.hostname, WIFI_HOSTNAME, sizeof(sysCfg.hostname));
  strlcpy(sysCfg.otaUrl, OTA_URL, sizeof(sysCfg.otaUrl));
  strlcpy(sysCfg.mqtt_host, MQTT_HOST, sizeof(sysCfg.mqtt_host));
  sysCfg.mqtt_port = MQTT_PORT;
  strlcpy(sysCfg.mqtt_client, MQTT_CLIENT_ID, sizeof(sysCfg.mqtt_client));
  strlcpy(sysCfg.mqtt_user, MQTT_USER, sizeof(sysCfg.mqtt_user));
  strlcpy(sysCfg.mqtt_pwd, MQTT_PASS, sizeof(sysCfg.mqtt_pwd));
  strlcpy(sysCfg.mqtt_grptopic, MQTT_GRPTOPIC, sizeof(sysCfg.mqtt_grptopic));
  strlcpy(sysCfg.mqtt_topic, MQTT_TOPIC, sizeof(sysCfg.mqtt_topic));
  strlcpy(sysCfg.mqtt_topic2, "0", sizeof(sysCfg.mqtt_topic2));
  strlcpy(sysCfg.mqtt_subtopic, MQTT_SUBTOPIC, sizeof(sysCfg.mqtt_subtopic));
  sysCfg.mqtt_retain = MQTT_BUTTON_RETAIN;
  sysCfg.mqtt_units = MQTT_UNITS;
  sysCfg.tele_period = TELE_PERIOD;
  strlcpy(sysCfg.domoticz_in_topic, DOMOTICZ_IN_TOPIC, sizeof(sysCfg.domoticz_in_topic));  
  strlcpy(sysCfg.domoticz_out_topic, DOMOTICZ_OUT_TOPIC, sizeof(sysCfg.domoticz_out_topic));  
  sysCfg.domoticz_update_timer = DOMOTICZ_UPDATE_TIMER;  
  sysCfg.domoticz_relay_idx[0] = DOMOTICZ_RELAY_IDX1;
  sysCfg.domoticz_relay_idx[1] = DOMOTICZ_RELAY_IDX2;
  sysCfg.domoticz_relay_idx[2] = DOMOTICZ_RELAY_IDX3;
  sysCfg.domoticz_relay_idx[3] = DOMOTICZ_RELAY_IDX4;
  sysCfg.domoticz_key_idx[0] = DOMOTICZ_KEY_IDX1;
  sysCfg.domoticz_key_idx[1] = DOMOTICZ_KEY_IDX2;
  sysCfg.domoticz_key_idx[2] = DOMOTICZ_KEY_IDX3;
  sysCfg.domoticz_key_idx[3] = DOMOTICZ_KEY_IDX4;
  sysCfg.timezone = APP_TIMEZONE;
  sysCfg.power = APP_POWER;
  sysCfg.ledstate = APP_LEDSTATE;
  sysCfg.webserver = WEB_SERVER;
  sysCfg.model = 0;
  sysCfg.hlw_pcal = HLW_PREF_PULSE;
  sysCfg.hlw_ucal = HLW_UREF_PULSE;
  sysCfg.hlw_ical = HLW_IREF_PULSE;
  sysCfg.hlw_esave = 0;
  sysCfg.hlw_pmin = 0;
  sysCfg.hlw_pmax = 0;
  sysCfg.hlw_umin = 0;
  sysCfg.hlw_umax = 0;
  sysCfg.hlw_imin = 0;
  sysCfg.hlw_imax = 0;
  sysCfg.hlw_mpl = 0;                              // MaxPowerLimit
  sysCfg.hlw_mplh = MAX_POWER_LIMIT_HOLD;          // MaxPowerLimitHold
  sysCfg.hlw_mplw = MAX_POWER_LIMIT_WINDOW;        // MaxPowerLimitWindow
  sysCfg.hlw_mspl = 0;                             // MaxSafePowerLimit
  sysCfg.hlw_msplh = MAX_SAFE_POWER_LIMIT_HOLD;    // MaxSafePowerLimitHold
  sysCfg.hlw_msplw = MAX_SAFE_POWER_LIMIT_WINDOW;  // MaxSafePowerLimitWindow
  sysCfg.hlw_mdpl = 0;                             // MaxDailyPowerLimit
  sysCfg.hlw_mdpls = 0;                            // MaxDailyPowerLimitStart
  
  CFG_Save();
}

void CFG_Delta()
{
  if (sysCfg.version != VERSION) {      // Fix version dependent changes
    if (sysCfg.version < 0x01000D00) {  // 1.0.13 - Add ledstate
      sysCfg.ledstate = APP_LEDSTATE;
    }
    if (sysCfg.version < 0x01001600) {  // 1.0.22 - Add MQTT parameters
      sysCfg.mqtt_port = MQTT_PORT;
      strlcpy(sysCfg.mqtt_client, MQTT_CLIENT_ID, sizeof(sysCfg.mqtt_client));
      strlcpy(sysCfg.mqtt_user, MQTT_USER, sizeof(sysCfg.mqtt_user));
      strlcpy(sysCfg.mqtt_pwd, MQTT_PASS, sizeof(sysCfg.mqtt_pwd));
    }
    if (sysCfg.version < 0x01001700) {  // 1.0.23 - Add webserver parameters
      sysCfg.webserver = WEB_SERVER;
    }
    if (sysCfg.version < 0x01001A00) {  // 1.0.26 - Add bootcount
      sysCfg.bootcount = 0;
      strlcpy(sysCfg.hostname, WIFI_HOSTNAME, sizeof(sysCfg.hostname));
      sysCfg.syslog_port = SYS_LOG_PORT;
    }
    if (sysCfg.version < 0x01001B00) {  // 1.0.27 - Add web log level
      sysCfg.weblog_level = WEB_LOG_LEVEL;
    }
    if (sysCfg.version < 0x01001C00) {  // 1.0.28 - Add telemetry parameter
      sysCfg.tele_period = TELE_PERIOD;
    }
    if (sysCfg.version < 0x01002000) {  // 1.0.32 - Add default Wifi config
      sysCfg.sta_config = WIFI_CONFIG_TOOL;
    }
    if (sysCfg.version < 0x01002300) {  // 1.0.35 - Add default savedata flag
      sysCfg.savedata = SAVE_DATA;
    }
    if (sysCfg.version < 0x02000000) {  // 2.0.0 - Add default model
      sysCfg.model = 0;
    }
    if (sysCfg.version < 0x02000300) {  // 2.0.3 - Add button retain flag
      sysCfg.mqtt_retain = MQTT_BUTTON_RETAIN;
      sysCfg.savestate = SAVE_STATE;
    }
    if (sysCfg.version < 0x02000500) {  // 2.0.5 - Add pow calibration
      sysCfg.hlw_pcal = HLW_PREF_PULSE;
      sysCfg.hlw_ucal = HLW_UREF_PULSE;
      sysCfg.hlw_ical = HLW_IREF_PULSE;
      sysCfg.hlw_esave = 0;
      sysCfg.mqtt_units = MQTT_UNITS;
    }
    if (sysCfg.version < 0x02000600) {  // 2.0.6 - Add pow thresholds
      sysCfg.hlw_pmin = 0;
      sysCfg.hlw_pmax = 0;
      sysCfg.hlw_umin = 0;
      sysCfg.hlw_umax = 0;
      sysCfg.hlw_imin = 0;
      sysCfg.hlw_imax = 0;
    }
    if (sysCfg.version < 0x02000700) {  // 2.0.7 - Add Domoticz defaults and Pow limits
      sysCfg.message_format = MESSAGE_FORMAT;
      strlcpy(sysCfg.domoticz_in_topic, DOMOTICZ_IN_TOPIC, sizeof(sysCfg.domoticz_in_topic));  
      strlcpy(sysCfg.domoticz_out_topic, DOMOTICZ_OUT_TOPIC, sizeof(sysCfg.domoticz_out_topic));  
      sysCfg.domoticz_update_timer = DOMOTICZ_UPDATE_TIMER;  
      sysCfg.domoticz_relay_idx[0] = DOMOTICZ_RELAY_IDX1;
      sysCfg.domoticz_relay_idx[1] = DOMOTICZ_RELAY_IDX2;
      sysCfg.domoticz_relay_idx[2] = DOMOTICZ_RELAY_IDX3;
      sysCfg.domoticz_relay_idx[3] = DOMOTICZ_RELAY_IDX4;
      sysCfg.domoticz_key_idx[0] = DOMOTICZ_KEY_IDX1;
      sysCfg.domoticz_key_idx[1] = DOMOTICZ_KEY_IDX2;
      sysCfg.domoticz_key_idx[2] = DOMOTICZ_KEY_IDX3;
      sysCfg.domoticz_key_idx[3] = DOMOTICZ_KEY_IDX4;
      sysCfg.hlw_mpl = 0;                              // MaxPowerLimit
      sysCfg.hlw_mplh = MAX_POWER_LIMIT_HOLD;          // MaxPowerLimitHold
      sysCfg.hlw_mplw = MAX_POWER_LIMIT_WINDOW;        // MaxPowerLimitWindow
      sysCfg.hlw_mspl = 0;                             // MaxSafePowerLimit
      sysCfg.hlw_msplh = MAX_SAFE_POWER_LIMIT_HOLD;    // MaxSafePowerLimitHold
      sysCfg.hlw_msplw = MAX_SAFE_POWER_LIMIT_WINDOW;  // MaxSafePowerLimitWindow
      sysCfg.hlw_mdpl = 0;                             // MaxDailyPowerLimit
      sysCfg.hlw_mdpls = 0;                            // MaxDailyPowerLimitStart
    }
    
    sysCfg.version = VERSION;
  }
}

/********************************************************************************************/

void setRelay(uint8_t power)
{
  if ((sysCfg.model >= SONOFF_DUAL) && (sysCfg.model <= CHANNEL_4)) {
    Serial.write(0xA0);
    Serial.write(0x04);
    Serial.write(power);
    Serial.write(0xA1);
    Serial.write('\n');
    Serial.flush();
  } else {
    digitalWrite(REL_PIN, power & 0x1);
  }
#ifdef USE_POWERMONITOR
  power_steady_cntr = 2;
#endif  
}

#ifdef USE_DOMOTICZ
unsigned long getKeyIntValue(const char *json, const char *key)
{
  char *p, *b;
  int i;

  // search key
  p = strstr(json, key);
  if (!p) return 0;
  // search following separator :
  b = strchr(p + strlen(key), ':');
  if (!b) return 0;
  // Only the following chars are allowed between key and separator :
  for(i = b - json + strlen(key); i < p-json; i++) {
    switch (json[i]) {
    case ' ':
    case '\n':
    case '\t':
    case '\r':
      continue;
    default:
      return 0;
    }
  }
  // Convert to integer
  return atoi(b +1);
}
#endif  // USE_DOMOTICZ

/********************************************************************************************/

void mqtt_publish(const char* topic, const char* data, boolean retained)
{
  char log[TOPSZ+MESSZ];

  if (mqttClient.publish(topic, data, retained)) {
    snprintf_P(log, sizeof(log), PSTR("MQTT: %s = %s%s"), topic, data, (retained) ? " (retained)" : "");
//    snprintf_P(log, sizeof(log), PSTR("MQTT: %s = %s%s"), strchr(topic,'/')+1, data, (retained) ? " (retained)" : ""); // Skip topic prefix
//    mqttClient.loop();  // Do not use here! Will block previous publishes
  } else {
    snprintf_P(log, sizeof(log), PSTR("RSLT: %s = %s"), topic, data);
  }
  addLog(LOG_LEVEL_INFO, log);
  blinks++;
}

void mqtt_publish(const char* topic, const char* data)
{
  mqtt_publish(topic, data, false);
}

void mqtt_connected()
{
  char stopic[TOPSZ], svalue[MESSZ];

  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/#"), SUB_PREFIX, sysCfg.mqtt_topic);
  mqttClient.subscribe(stopic);
  mqttClient.loop();  // Solve LmacRxBlk:1 messages
  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/#"), SUB_PREFIX, sysCfg.mqtt_grptopic);
  mqttClient.subscribe(stopic);
  mqttClient.loop();  // Solve LmacRxBlk:1 messages
  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/#"), SUB_PREFIX, MQTTClient); // Fall back topic
  mqttClient.subscribe(stopic);
  mqttClient.loop();  // Solve LmacRxBlk:1 messages
#ifdef USE_DOMOTICZ  
  if (sysCfg.domoticz_relay_idx[0] && (strlen(sysCfg.domoticz_out_topic) != 0)) {  
    snprintf_P(stopic, sizeof(stopic), PSTR("%s/#"), sysCfg.domoticz_out_topic); // domoticz topic  
    mqttClient.subscribe(stopic);  
    mqttClient.loop();  // Solve LmacRxBlk:1 messages  
  }  
#endif  // USE_DOMOTICZ  

  if (mqttflag) {
    snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/INFO"), PUB_PREFIX, sysCfg.mqtt_topic);
    snprintf_P(svalue, sizeof(svalue), PSTR(APP_NAME " version %s, Fallback %s, GroupTopic %s"), Version, MQTTClient, sysCfg.mqtt_grptopic);
    mqtt_publish(stopic, svalue);
#ifdef USE_WEBSERVER
    if (sysCfg.webserver) {
      snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/INFO"), PUB_PREFIX, sysCfg.mqtt_topic);
      snprintf_P(svalue, sizeof(svalue), PSTR("Webserver active for %s on %s with IP address %s"),
        (sysCfg.webserver == 2) ? "Admin" : "User", Hostname, WiFi.localIP().toString().c_str());
      mqtt_publish(stopic, svalue);
    }
#endif  // USE_WEBSERVER
    if (MQTT_MAX_PACKET_SIZE < (TOPSZ+MESSZ)) {
      snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/WARNING"), PUB_PREFIX, sysCfg.mqtt_topic);
      snprintf_P(svalue, sizeof(svalue), PSTR("Change MQTT_MAX_PACKET_SIZE in libraries/PubSubClient.h to at least %d"), TOPSZ+MESSZ);
      mqtt_publish(stopic, svalue);
    }
    if (!spiffsPresent()) {
      snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/WARNING"), PUB_PREFIX, sysCfg.mqtt_topic);
      snprintf_P(svalue, sizeof(svalue), PSTR("No persistent config. Please reflash with at least 16K SPIFFS"));
      mqtt_publish(stopic, svalue);
    }
    status_update_timer = 2;
#ifdef USE_DOMOTICZ
    domoticz_update_timer = 2;
#endif  // USE_DOMOTICZ  
  }
  mqttflag = 0;
}

void mqtt_reconnect()
{
  char stopic[TOPSZ], log[LOGSZ];

  mqttcounter = MQTT_RETRY_SECS;
  addLog_P(LOG_LEVEL_INFO, PSTR("MQTT: Attempting connection..."));
  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/lwt"), PUB_PREFIX, sysCfg.mqtt_topic);
  if (mqttClient.connect(MQTTClient, sysCfg.mqtt_user, sysCfg.mqtt_pwd, stopic, 0, 0, "offline")) {
    addLog_P(LOG_LEVEL_INFO, PSTR("MQTT: Connected"));
    mqttcounter = 0;
    mqtt_connected();
  } else {
    snprintf_P(log, sizeof(log), PSTR("MQTT: CONNECT FAILED, rc %d. Retry in %d seconds"), mqttClient.state(), mqttcounter);
    addLog(LOG_LEVEL_DEBUG, log);
  }
}

void mqttDataCb(char* topic, byte* data, unsigned int data_len)
{
  int i, grpflg = 0, device;
  float ped, pi, pc;
  uint16_t pe, pw, pu;
  char topicBuf[TOPSZ], dataBuf[MESSZ], dataBufUc[MESSZ];
  char *str, *p, *mtopic = NULL, *type = NULL, *devc = NULL;
  char stopic[TOPSZ], svalue[MESSZ], stemp1[TOPSZ], stemp2[10], stemp3[10];
  
  strncpy(topicBuf, topic, sizeof(topicBuf));
  i = (data_len > sizeof(dataBuf)) ? sizeof(dataBuf) : data_len;
  memcpy(dataBuf, data, i);
  dataBuf[i] = 0;

  snprintf_P(svalue, sizeof(svalue), PSTR("MQTT: Receive topic %s, data %s"), topicBuf, dataBuf);
  addLog(LOG_LEVEL_DEBUG, svalue);

#ifdef USE_DOMOTICZ
  domoticz_update_flag = 1;
  if (!strncmp(topicBuf, sysCfg.domoticz_out_topic, strlen(sysCfg.domoticz_out_topic)) != 0) {  
    unsigned long idx = 0;  
    int16_t nvalue = 0;  

    for(i = 0; i <= strlen(dataBuf); i++) dataBufUc[i] = toupper(dataBuf[i]);
    dataBuf[0] = '\0';
    idx = getKeyIntValue(dataBufUc,"\"IDX\"");  
    nvalue = getKeyIntValue(dataBufUc,"\"NVALUE\"");  
    if (nvalue == 0 || nvalue == 1) {  
      for (i = 0; i < Maxdevice; i++) {
        if (idx > 0 && idx == sysCfg.domoticz_relay_idx[i]) {
          snprintf_P(dataBuf, sizeof(dataBuf), PSTR("%d"), nvalue);  
          break;
        }
      }
    }
    if (!strlen(dataBuf)) return;
    snprintf_P(topicBuf, sizeof(topicBuf), PSTR("%s/%s/%d/LIGHT"), SUB_PREFIX, sysCfg.mqtt_topic, i +1);
    domoticz_update_flag = 0;
  }
#endif //USE_DOMOTICZ  

  i = 0;
  for (str = strtok_r(topicBuf, "/", &p); str && i < 4; str = strtok_r(NULL, "/", &p)) {
    switch (i++) {
    case 0:  // cmnd
      break;
    case 1:  // Topic / GroupTopic / DVES_123456
      mtopic = str;
      break;
    case 2:  // TopicIndex / Text
      type = str;
      break;
    case 3:  // Text
      devc = str;
    }
  }
  if (!strcmp(mtopic, sysCfg.mqtt_grptopic)) grpflg = 1;

  device = 1;
  if (devc != NULL) {
    if (Maxdevice) {
      device = atoi(type);
      if ((device < 1) || (device > Maxdevice)) device = 1;
    }
    type = devc;
  }
  if (!device) type = NULL;
  
  if (type != NULL) for(i = 0; i < strlen(type); i++) type[i] = toupper(type[i]);

  for(i = 0; i <= data_len; i++) dataBufUc[i] = toupper(dataBuf[i]);

  snprintf_P(svalue, sizeof(svalue), PSTR("MQTT: DataCb Topic %s, Group %d, Device %d, Type %s, Data %s (%s)"),
    mtopic, grpflg, device, type, dataBuf, dataBufUc);
  addLog(LOG_LEVEL_DEBUG, svalue);

  if (type != NULL) {
    if (devc == NULL) {
      snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/%s"), PUB_PREFIX, sysCfg.mqtt_topic, type);
    } else {
      snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/%d/%s"), PUB_PREFIX, sysCfg.mqtt_topic, device, type);
    }
    strlcpy(svalue, "Error", sizeof(svalue));

    if (!strcmp(dataBufUc,"?")) data_len = 0;
    int16_t payload = atoi(dataBuf);
    if (!strcmp(dataBufUc,"OFF") || !strcmp(dataBufUc,"STOP")) payload = 0;
    if (!strcmp(dataBufUc,"ON") || !strcmp(dataBufUc,"START") || !strcmp(dataBufUc,"USER")) payload = 1;
    if (!strcmp(dataBufUc,"TOGGLE") || !strcmp(dataBufUc,"ADMIN")) payload = 2;
    
    if (!strcmp(type,"STATUS")) {
      if ((data_len == 0) || (payload < 0) || (payload > MAX_STATUS)) payload = 99;
      if ((payload == 0) || (payload == 99)) {
        if (sysCfg.message_format == JSON) {
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"Version\":\"%s\", \"Model\":%d, \"Topic\":\"%s\", \"ButtonTopic\":\"%s\", \"Subtopic\":\"%s\", \"Power\":%d, \"Timezone\":%d, \"Ledstate\":%d, \"SaveData\":%d, \"SaveState\":%d, \"Retain\":%d}"),
            Version, sysCfg.model, sysCfg.mqtt_topic, sysCfg.mqtt_topic2, sysCfg.mqtt_subtopic, power, sysCfg.timezone, sysCfg.ledstate, sysCfg.savedata, sysCfg.savestate, sysCfg.mqtt_retain);
        } else {
          snprintf_P(svalue, sizeof(svalue), PSTR("%s, %d, %s, %s, %s, %d, %d, %d, %d, %d, %d"),
            Version, sysCfg.model, sysCfg.mqtt_topic, sysCfg.mqtt_topic2, sysCfg.mqtt_subtopic, power, sysCfg.timezone, sysCfg.ledstate, sysCfg.savedata, sysCfg.savestate, sysCfg.mqtt_retain);
        }
        if (payload == 0) mqtt_publish(stopic, svalue);
      }
#ifdef USE_POWERMONITOR
      if ((payload == 0) || (payload == 8)) {
        hlw_readEnergy(0, ped, pe, pw, pu, pi, pc);
        dtostrf(pi, 1, 3, stemp1);
        dtostrf(ped, 1, 3, stemp2);
        dtostrf(pc, 1, 2, stemp3);
        if (sysCfg.message_format == JSON) {
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"PWR\":{\"Voltage\":%d, \"Current\":\"%s\", \"Current Power\":%d, \"Total Energy Today\":\"%s\", \"Power Factor\":\"%s\"}}"),
            pu, stemp1, pw, stemp2, stemp3); 
        } else {
          snprintf_P(svalue, sizeof(svalue), PSTR("PWR: Voltage %d V, Current %s A, Current Power %d W, Total Energy Today %s kWh, Power Factor %s"),
            pu, stemp1, pw, stemp2, stemp3); 
        }
        if (payload == 0) mqtt_publish(stopic, svalue);
      }
      if ((payload == 0) || (payload == 9)) {
        if (sysCfg.message_format == JSON) {
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"PWR\":{\"Threshold PowerLow\":%d, \"PowerHigh\":%d, \"VoltageLow\":%d, \"VoltageHigh\":%d, \"CurrentLow\":%d, \"CurrentHigh\":%d}}"),
            sysCfg.hlw_pmin, sysCfg.hlw_pmax, sysCfg.hlw_umin, sysCfg.hlw_umax, sysCfg.hlw_imin, sysCfg.hlw_imax);
        } else {
          snprintf_P(svalue, sizeof(svalue), PSTR("PWR: Threshold PowerLow %d W, PowerHigh %d W, VoltageLow %d V, VoltageHigh %d V, CurrentLow %d mA, CurrentHigh %d mA"),
            sysCfg.hlw_pmin, sysCfg.hlw_pmax, sysCfg.hlw_umin, sysCfg.hlw_umax, sysCfg.hlw_imin, sysCfg.hlw_imax);
        }
        if (payload == 0) mqtt_publish(stopic, svalue);
      }
#endif  // USE_POWERMONITOR      
      if ((payload == 0) || (payload == 1)) {
        if (sysCfg.message_format == JSON) {
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"PRM\":{\"Baudrate\":%d, \"GroupTopic\":\"%s\", \"OtaUrl\":\"%s\", \"Uptime\":%d, \"BootCount\":%d, \"SaveCount\":%d}}"),
            Baudrate, sysCfg.mqtt_grptopic, sysCfg.otaUrl, uptime, sysCfg.bootcount, sysCfg.saveFlag);
        } else {
          snprintf_P(svalue, sizeof(svalue), PSTR("PRM: Baudrate %d, GroupTopic %s, OtaUrl %s, Uptime %d Hr, BootCount %d, SaveCount %d"),
            Baudrate, sysCfg.mqtt_grptopic, sysCfg.otaUrl, uptime, sysCfg.bootcount, sysCfg.saveFlag);
        }
        if (payload == 0) mqtt_publish(stopic, svalue);
      }          
      if ((payload == 0) || (payload == 2)) {
        if (sysCfg.message_format == JSON) {
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"FWR\":{\"Program version\":\"%s\", \"Boot version\":%d, \"SDK version\":\"%s\"}}"),
            Version, ESP.getBootVersion(), ESP.getSdkVersion());
        } else {
          snprintf_P(svalue, sizeof(svalue), PSTR("FWR: Version %s, Boot %d, SDK %s"),
            Version, ESP.getBootVersion(), ESP.getSdkVersion());
        }
        if (payload == 0) mqtt_publish(stopic, svalue);
      }          
      if ((payload == 0) || (payload == 3)) {
        if (sysCfg.message_format == JSON) {
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"LOG\":{\"Seriallog\":%d, \"Weblog\":%d, \"Syslog\":%d, \"LogHost\":\"%s\", \"SSId\":\"%s\", \"Password\":\"%s\", \"TelePeriod\":%d}}"),
            sysCfg.seriallog_level, sysCfg.weblog_level, sysCfg.syslog_level, sysCfg.syslog_host, sysCfg.sta_ssid, sysCfg.sta_pwd, sysCfg.tele_period);
        } else {
          snprintf_P(svalue, sizeof(svalue), PSTR("LOG: Seriallog %d, Weblog %d, Syslog %d, LogHost %s, SSId %s, Password %s, TelePeriod %d"),
            sysCfg.seriallog_level, sysCfg.weblog_level, sysCfg.syslog_level, sysCfg.syslog_host, sysCfg.sta_ssid, sysCfg.sta_pwd, sysCfg.tele_period);
        }
        if (payload == 0) mqtt_publish(stopic, svalue);
      }          
      if ((payload == 0) || (payload == 4)) {
        if (sysCfg.message_format == JSON) {
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"MEM\":{\"Program size\":%d, \"Free\":%d, \"Heap\":%d, \"Spiffs start\":%d, \"Spiffs size\":%d, \"Flash size\":%d, \"Program flash size\":%d}}"),
            ESP.getSketchSize()/1024, ESP.getFreeSketchSpace()/1024, ESP.getFreeHeap()/1024, ((uint32_t)&_SPIFFS_start - 0x40200000)/1024,
            (((uint32_t)&_SPIFFS_end - 0x40200000) - ((uint32_t)&_SPIFFS_start - 0x40200000))/1024, ESP.getFlashChipRealSize()/1024, ESP.getFlashChipSize()/1024);
        } else {
          snprintf_P(svalue, sizeof(svalue), PSTR("MEM: Program size %dkB, Free %dkB (Heap %dkB), Spiffs start %dkB (%dkB), Flash size %dkB (%dkB)"),
            ESP.getSketchSize()/1024, ESP.getFreeSketchSpace()/1024, ESP.getFreeHeap()/1024, ((uint32_t)&_SPIFFS_start - 0x40200000)/1024,
            (((uint32_t)&_SPIFFS_end - 0x40200000) - ((uint32_t)&_SPIFFS_start - 0x40200000))/1024, ESP.getFlashChipRealSize()/1024, ESP.getFlashChipSize()/1024);
        }
        if (payload == 0) mqtt_publish(stopic, svalue);
      }          
      if ((payload == 0) || (payload == 5)) {
        if (sysCfg.message_format == JSON) {
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"NET\":{\"Host\":\"%s\", \"IP\":\"%s\", \"Gateway\":\"%s\", \"Subnetmask\":\"%s\", \"Mac\":\"%s\", \"Webserver\":%d, \"WifiConfig\":%d}}"),
            Hostname, WiFi.localIP().toString().c_str(), WiFi.gatewayIP().toString().c_str(), WiFi.subnetMask().toString().c_str(),
            WiFi.macAddress().c_str(), sysCfg.webserver, sysCfg.sta_config);
        } else {
          snprintf_P(svalue, sizeof(svalue), PSTR("NET: Host %s, IP %s, Gateway %s, Subnetmask %s, Mac %s, Webserver %d, WifiConfig %d"),
            Hostname, WiFi.localIP().toString().c_str(), WiFi.gatewayIP().toString().c_str(), WiFi.subnetMask().toString().c_str(),
            WiFi.macAddress().c_str(), sysCfg.webserver, sysCfg.sta_config);
        }
        if (payload == 0) mqtt_publish(stopic, svalue);
      }          
      if ((payload == 0) || (payload == 6)) {
        if (sysCfg.message_format == JSON) {
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"MQT\":{\"Host\":\"%s\", \"Port\":%d, \"Client mask\":\"%s\", \"Client\":\"%s\", \"User\":\"%s\", \"Password\":\"%s\", \"MAX_PACKET_SIZE\":%d, \"KEEPALIVE\":%d}}"),
            sysCfg.mqtt_host, sysCfg.mqtt_port, sysCfg.mqtt_client, MQTTClient, sysCfg.mqtt_user, sysCfg.mqtt_pwd, MQTT_MAX_PACKET_SIZE, MQTT_KEEPALIVE); 
        } else {
          snprintf_P(svalue, sizeof(svalue), PSTR("MQT: Host %s, Port %d, Client %s (%s), User %s, Password %s, MAX_PACKET_SIZE %d, KEEPALIVE %d"),
            sysCfg.mqtt_host, sysCfg.mqtt_port, sysCfg.mqtt_client, MQTTClient, sysCfg.mqtt_user, sysCfg.mqtt_pwd, MQTT_MAX_PACKET_SIZE, MQTT_KEEPALIVE); 
        }
        if (payload == 0) mqtt_publish(stopic, svalue);
      }      
      if ((payload == 0) || (payload == 7)) {
        if (sysCfg.message_format == JSON) {
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"TIM\":{\"UTC\":\"%s\", \"Local\":\"%s\", \"Start DST\":\"%s\", \"End DST\":\"%s\"}}"),
            rtc_time(0).c_str(), rtc_time(1).c_str(), rtc_time(2).c_str(), rtc_time(3).c_str()); 
        } else {
          snprintf_P(svalue, sizeof(svalue), PSTR("TIM: UTC %s, Local %s, Start DST %s, End DST %s"),
            rtc_time(0).c_str(), rtc_time(1).c_str(), rtc_time(2).c_str(), rtc_time(3).c_str()); 
        }
      }      
    }
    else if (!strcmp(type,"SAVEDATA")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 3600)) {
        sysCfg.savedata = payload;
        savedatacounter = sysCfg.savedata;
      }
      if (sysCfg.savestate) sysCfg.power = power;
      CFG_Save();
      if (sysCfg.savedata > 1) {
        snprintf_P(svalue, sizeof(svalue), PSTR("Every %d seconds"), sysCfg.savedata);
      } else {
        strlcpy(svalue, (sysCfg.savedata) ? "On" : "Manual", sizeof(svalue));
      }
    }
    else if (!strcmp(type,"SAVESTATE")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 1)) {
        sysCfg.savestate = payload;
      }
      strlcpy(svalue, (sysCfg.savestate) ? "On" : "Off", sizeof(svalue));
    }
    else if (!strcmp(type,"MESSAGEFORMAT")) {
      if ((data_len > 0) && (payload >= 0) && (payload < MAX_FORMAT)) {
        sysCfg.message_format = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d"), sysCfg.message_format);
    }
    else if (!strcmp(type,"MODEL")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= CHANNEL_4)) {
        sysCfg.model = payload;
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d"), sysCfg.model);
    }
    else if (!strcmp(type,"UPGRADE") || !strcmp(type,"UPLOAD")) {
      if ((data_len > 0) && (payload == 1)) {
        otaflag = 3;
        snprintf_P(svalue, sizeof(svalue), PSTR("Upgrade %s from %s"), Version, sysCfg.otaUrl);
      } else {
        snprintf_P(svalue, sizeof(svalue), PSTR("1 to upgrade"));
      }
    }
    else if (!strcmp(type,"OTAURL")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.otaUrl)))
        strlcpy(sysCfg.otaUrl, (payload == 1) ? OTA_URL : dataBuf, sizeof(sysCfg.otaUrl));
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.otaUrl);
    }
    else if (!strcmp(type,"SERIALLOG")) {
      if ((data_len > 0) && (payload >= LOG_LEVEL_NONE) && (payload <= LOG_LEVEL_ALL)) {
        sysCfg.seriallog_level = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d"), sysCfg.seriallog_level);
    }
    else if (!strcmp(type,"SYSLOG")) {
      if ((data_len > 0) && (payload >= LOG_LEVEL_NONE) && (payload <= LOG_LEVEL_ALL)) {
        sysCfg.syslog_level = payload;
        syslog_level = payload;
        syslog_timer = 0;
        snprintf_P(svalue, sizeof(svalue), PSTR("%d"), sysCfg.syslog_level);
      } else {
        snprintf_P(svalue, sizeof(svalue), PSTR("%d (%d)"), syslog_level, sysCfg.syslog_level);
      }
    }
    else if (!strcmp(type,"LOGHOST")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.syslog_host))) {
        strlcpy(sysCfg.syslog_host, (payload == 1) ? SYS_LOG_HOST : dataBuf, sizeof(sysCfg.syslog_host));
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.syslog_host);
    }
    else if (!strcmp(type,"LOGPORT")) {
      if ((data_len > 0) && (payload > 0) && (payload < 32766)) {
        sysCfg.syslog_port = (payload == 1) ? SYS_LOG_PORT : payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d"), sysCfg.syslog_port);
    }
    else if (!strcmp(type,"SSID")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.sta_ssid))) {
        strlcpy(sysCfg.sta_ssid, (payload == 1) ? STA_SSID : dataBuf, sizeof(sysCfg.sta_ssid));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.sta_ssid);
    }
    else if (!strcmp(type,"PASSWORD")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.sta_pwd))) {
        strlcpy(sysCfg.sta_pwd, (payload == 1) ? STA_PASS : dataBuf, sizeof(sysCfg.sta_pwd));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.sta_pwd);
    }
    else if (!grpflg && !strcmp(type,"HOSTNAME")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.hostname))) {
        strlcpy(sysCfg.hostname, (payload == 1) ? WIFI_HOSTNAME : dataBuf, sizeof(sysCfg.hostname));
        if (strstr(sysCfg.hostname,"%"))
          strlcpy(sysCfg.hostname, DEF_WIFI_HOSTNAME, sizeof(sysCfg.hostname));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.hostname);
    }
    else if (!strcmp(type,"WIFICONFIG") || !strcmp(type,"SMARTCONFIG")) {
      if (data_len > 0) {
        if (payload == 0) payload = sysCfg.sta_config;
        if ((payload > 0) && (payload <= 3)) {
          wificheckflag = payload;
          sysCfg.sta_config = wificheckflag;
          snprintf_P(svalue, sizeof(svalue), PSTR("%s selected"), (payload == WIFI_SMARTCONFIG) ? "Smartconfig" : (payload == WIFI_MANAGER) ? "Wifimanager" : "WPSconfig");
          if (WIFI_State() != WIFI_STATUS) {
            snprintf_P(svalue, sizeof(svalue), PSTR("%s on restart"), svalue);
            restartflag = 2;
          }
        }
      } else {
        snprintf_P(svalue, sizeof(svalue), PSTR("1 to start smartconfig, 2 to start wifimanager, 3 to start wpsconfig. Default is %d"), sysCfg.sta_config);
      }
    }
#ifdef USE_WEBSERVER
    else if (!strcmp(type,"WEBSERVER")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 2)) {
        sysCfg.webserver = payload;
      }
      if (sysCfg.webserver) {
        snprintf_P(svalue, sizeof(svalue), PSTR("Webserver active for %s on %s with IP address %s"),
          (sysCfg.webserver == 2) ? "ADMIN" : "USER", Hostname, WiFi.localIP().toString().c_str());
      } else {
        snprintf_P(svalue, sizeof(svalue), PSTR("OFF"));
      }
    }
    else if (!strcmp(type,"WEBLOG")) {
      if ((data_len > 0) && (payload >= LOG_LEVEL_NONE) && (payload <= LOG_LEVEL_ALL)) {
        sysCfg.weblog_level = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d"), sysCfg.weblog_level);
    }
#endif  // USE_WEBSERVER
    else if (!strcmp(type,"MQTTHOST")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.mqtt_host))) {
        strlcpy(sysCfg.mqtt_host, (payload == 1) ? MQTT_HOST : dataBuf, sizeof(sysCfg.mqtt_host));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.mqtt_host);
    }
    else if (!strcmp(type,"MQTTPORT")) {
      if ((data_len > 0) && (payload > 0) && (payload < 32766)) {
        sysCfg.mqtt_port = (payload == 1) ? MQTT_PORT : payload;
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d"), sysCfg.mqtt_port);
    }
    else if (!grpflg && !strcmp(type,"MQTTCLIENT")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.mqtt_client))) {
        strlcpy(sysCfg.mqtt_client, (payload == 1) ? MQTT_CLIENT_ID : dataBuf, sizeof(sysCfg.mqtt_client));
        if (strstr(sysCfg.mqtt_client,"%"))
          strlcpy(sysCfg.mqtt_client, DEF_MQTT_CLIENT_ID, sizeof(sysCfg.mqtt_client));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.mqtt_client);
    }
    else if (!strcmp(type,"MQTTUSER")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.mqtt_user))) {
        strlcpy(sysCfg.mqtt_user, (payload == 1) ? MQTT_USER : dataBuf, sizeof(sysCfg.mqtt_user));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.mqtt_user);
    }
    else if (!strcmp(type,"MQTTPASSWORD")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.mqtt_pwd))) {
        strlcpy(sysCfg.mqtt_pwd, (payload == 1) ? MQTT_PASS : dataBuf, sizeof(sysCfg.mqtt_pwd));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.mqtt_pwd);
    }
    else if (!strcmp(type,"MQTTUNITS")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 1)) {
        sysCfg.mqtt_units = payload;
      }
      strlcpy(svalue, (sysCfg.mqtt_units) ? "On" : "Off", sizeof(svalue));
    }
#ifdef USE_DOMOTICZ
    else if (!strcmp(type,"DOMOTICZINTOPIC")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.domoticz_in_topic))) {
        strlcpy(sysCfg.domoticz_in_topic, (payload == 1) ? DOMOTICZ_IN_TOPIC : dataBuf, sizeof(sysCfg.domoticz_in_topic));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.domoticz_in_topic);
    }
    else if (!strcmp(type,"DOMOTICZOUTTOPIC")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.domoticz_out_topic))) {
        strlcpy(sysCfg.domoticz_out_topic, (payload == 1) ? DOMOTICZ_OUT_TOPIC : dataBuf, sizeof(sysCfg.domoticz_out_topic));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.domoticz_out_topic);
    }
    else if (!strcmp(type,"DOMOTICZIDX")) {
      if ((data_len > 0) && (payload >= 0)) {
        sysCfg.domoticz_relay_idx[device -1] = payload;
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d"), sysCfg.domoticz_relay_idx[device -1]);
    }
    else if (!strcmp(type,"DOMOTICZKEYIDX")) {
      if ((data_len > 0) && (payload >= 0)) {
        sysCfg.domoticz_key_idx[device -1] = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d"), sysCfg.domoticz_key_idx[device -1]);
    }
    else if (!strcmp(type,"DOMOTICZUPDATETIMER")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.domoticz_update_timer = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d"), sysCfg.domoticz_update_timer);
    }
#endif  // USE_DOMOTICZ  
    else if (!strcmp(type,"TELEPERIOD")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.tele_period = (payload == 1) ? TELE_PERIOD : payload;
        tele_period = sysCfg.tele_period;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), sysCfg.tele_period, (sysCfg.mqtt_units) ? " Sec" : "");
    }
    else if (!strcmp(type,"GROUPTOPIC")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.mqtt_grptopic))) {
        for(i = 0; i <= data_len; i++)
          if ((dataBuf[i] == '/') || (dataBuf[i] == '+') || (dataBuf[i] == '#')) dataBuf[i] = '_';
        if (!strcmp(dataBuf, MQTTClient)) payload = 1;
        strlcpy(sysCfg.mqtt_grptopic, (payload == 1) ? MQTT_GRPTOPIC : dataBuf, sizeof(sysCfg.mqtt_grptopic));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.mqtt_grptopic);
    }
    else if (!grpflg && !strcmp(type,"TOPIC")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.mqtt_topic))) {
        for(i = 0; i <= data_len; i++)
          if ((dataBuf[i] == '/') || (dataBuf[i] == '+') || (dataBuf[i] == '#')) dataBuf[i] = '_';
        if (!strcmp(dataBuf, MQTTClient)) payload = 1;
        strlcpy(sysCfg.mqtt_topic, (payload == 1) ? MQTT_TOPIC : dataBuf, sizeof(sysCfg.mqtt_topic));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.mqtt_topic);
    }
    else if (!grpflg && !strcmp(type,"BUTTONTOPIC")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.mqtt_topic2))) {
        for(i = 0; i <= data_len; i++)
          if ((dataBuf[i] == '/') || (dataBuf[i] == '+') || (dataBuf[i] == '#')) dataBuf[i] = '_';
        if (!strcmp(dataBuf, MQTTClient)) payload = 1;
        strlcpy(sysCfg.mqtt_topic2, (payload == 1) ? sysCfg.mqtt_topic : dataBuf, sizeof(sysCfg.mqtt_topic2));
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.mqtt_topic2);
    }
    else if (!strcmp(type,"BUTTONRETAIN")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 1)) {
        strlcpy(sysCfg.mqtt_topic2, sysCfg.mqtt_topic, sizeof(sysCfg.mqtt_topic2));
        if (!payload) {
          for(i = 1; i <= Maxdevice; i++) {
            send_button_power(i, 3);  // Clear MQTT retain in broker
          }
        }
        sysCfg.mqtt_retain = payload;
      }
      strlcpy(svalue, (sysCfg.mqtt_retain) ? "On" : "Off", sizeof(svalue));
    }
    else if (!strcmp(type,"RESTART")) {
      switch (payload) {
      case 1: 
        restartflag = 2;
        snprintf_P(svalue, sizeof(svalue), PSTR("Restarting"));
        break;
      case 99:
        addLog_P(LOG_LEVEL_INFO, PSTR("APP: Restarting"));
        ESP.restart();
        break;
      default:
        snprintf_P(svalue, sizeof(svalue), PSTR("1 to restart"));
      }
    }
    else if (!strcmp(type,"RESET")) {
      switch (payload) {
      case 1: 
        restartflag = 211;
        snprintf_P(svalue, sizeof(svalue), PSTR("Reset and Restarting"));
        break;
      case 2:
        restartflag = 212;
        snprintf_P(svalue, sizeof(svalue), PSTR("Erase, Reset and Restarting"));
        break;
      default:
        snprintf_P(svalue, sizeof(svalue), PSTR("1 to reset"));
      }
    }
    else if (!strcmp(type,"TIMEZONE")) {
      if ((data_len > 0) && (((payload >= -12) && (payload <= 12)) || (payload == 99))) {
        sysCfg.timezone = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), sysCfg.timezone, (sysCfg.mqtt_units) ? " Hr" : "");
    }
    else if ((!strcmp(type,"LIGHT")) || (!strcmp(type,"POWER"))) {
      snprintf_P(sysCfg.mqtt_subtopic, sizeof(sysCfg.mqtt_subtopic), PSTR("%s"), type);
      byte mask = 0x01 << (device -1);
      if ((data_len > 0) && (payload >= 0) && (payload <= 2)) {
        switch (payload) {
        case 0: { // Off
          power &= (0xFF ^ mask);
          break; }
        case 1: // On
          power |= mask;
          break;
        case 2: // Toggle
          power ^= mask;
          break;
        }
        setRelay(power);
#ifdef USE_DOMOTICZ
        if (domoticz_update_flag) domoticz_update_timer = 1;
#endif  // USE_DOMOTICZ  
      }
      strlcpy(svalue, (power & mask) ? MQTT_STATUS_ON : MQTT_STATUS_OFF, sizeof(svalue));
    }
    else if (!strcmp(type,"LEDSTATE")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 1)) {
        sysCfg.ledstate = payload;
      }
      strlcpy(svalue, (sysCfg.ledstate) ? "On" : "Off", sizeof(svalue));
    }
#ifdef USE_POWERMONITOR
    else if (!strcmp(type,"POWERLOW")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.hlw_pmin = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), sysCfg.hlw_pmin, (sysCfg.mqtt_units) ? " W" : "");
    }
    else if (!strcmp(type,"POWERHIGH")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.hlw_pmax = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), sysCfg.hlw_pmax, (sysCfg.mqtt_units) ? " W" : "");
    }
    else if (!strcmp(type,"VOLTAGELOW")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 501)) {
        sysCfg.hlw_umin = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), sysCfg.hlw_umin, (sysCfg.mqtt_units) ? " V" : "");
    }
    else if (!strcmp(type,"VOLTAGEHIGH")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 501)) {
        sysCfg.hlw_umax = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), sysCfg.hlw_umax, (sysCfg.mqtt_units) ? " V" : "");
    }
    else if (!strcmp(type,"CURRENTLOW")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 16001)) {
        sysCfg.hlw_imin = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), sysCfg.hlw_imin, (sysCfg.mqtt_units) ? " mA" : "");
    }
    else if (!strcmp(type,"CURRENTHIGH")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 16001)) {
        sysCfg.hlw_imax = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), sysCfg.hlw_imax, (sysCfg.mqtt_units) ? " mA" : "");
    }
#ifdef USE_POWERCALIBRATION
    else if (!strcmp(type,"HLWPCAL")) {
      if ((data_len > 0) && (payload > 0) && (payload < 32001)) {
        sysCfg.hlw_pcal = (payload == 1) ? HLW_PREF_PULSE : payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), sysCfg.hlw_pcal, (sysCfg.mqtt_units) ? " uS" : "");
    }
    else if (!strcmp(type,"HLWUCAL")) {
      if ((data_len > 0) && (payload > 0) && (payload < 32001)) {
        sysCfg.hlw_ucal = (payload == 1) ? HLW_UREF_PULSE : payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), sysCfg.hlw_ucal, (sysCfg.mqtt_units) ? " uS" : "");
    }
    else if (!strcmp(type,"HLWICAL")) {
      if ((data_len > 0) && (payload > 0) && (payload < 32001)) {
        sysCfg.hlw_ical = (payload == 1) ? HLW_IREF_PULSE : payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), sysCfg.hlw_ical, (sysCfg.mqtt_units) ? " uS" : "");
    }
#endif  // USE_POWERCALIBRATION
#ifdef FEATURE_POWER_LIMIT
    else if (!strcmp(type,"MAXPL")) {         // MaxPowerLimit
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.hlw_mpl = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), sysCfg.hlw_mpl, (sysCfg.mqtt_units) ? " W" : "");
    }
    else if (!strcmp(type,"MAXPLHOLD")) {     // MaxPowerLimitHold
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.hlw_mplh = (payload == 1) ? MAX_POWER_LIMIT_HOLD : payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), sysCfg.hlw_mplh, (sysCfg.mqtt_units) ? " Sec" : "");
    }
    else if (!strcmp(type,"MAXPLWINDOW")) {   // MaxPowerLimitWindow
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.hlw_mplw = (payload == 1) ? MAX_POWER_LIMIT_WINDOW : payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), sysCfg.hlw_mplw, (sysCfg.mqtt_units) ? " Sec" : "");
    }
    else if (!strcmp(type,"MAXSPL")) {        // MaxSafePowerLimit
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.hlw_mspl = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), sysCfg.hlw_mspl, (sysCfg.mqtt_units) ? " W" : "");
    }
    else if (!strcmp(type,"MAXSPLHOLD")) {    // MaxSafePowerLimitHold
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.hlw_msplh = (payload == 1) ? MAX_SAFE_POWER_LIMIT_HOLD : payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), sysCfg.hlw_msplh, (sysCfg.mqtt_units) ? " Sec" : "");
    }
    else if (!strcmp(type,"MAXSPLWINDOW")) {  // MaxSafePowerLimitWindow
      if ((data_len > 0) && (payload >= 0) && (payload < 1440)) {
        sysCfg.hlw_msplw = (payload == 1) ? MAX_SAFE_POWER_LIMIT_WINDOW : payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), sysCfg.hlw_msplw, (sysCfg.mqtt_units) ? " Min" : "");
    }
    else if (!strcmp(type,"MAXDPL")) {        // MaxDailyPowerLimit
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.hlw_mdpl = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), sysCfg.hlw_mdpl, (sysCfg.mqtt_units) ? " Wh" : "");
    }
    else if (!strcmp(type,"MAXDPLSTART")) {   // MaxDailyPowerLimitStart
      if ((data_len > 0) && (payload >= 0) && (payload < 24)) {
        sysCfg.hlw_mdpls = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), sysCfg.hlw_mdpls, (sysCfg.mqtt_units) ? " Hr" : "");
    }
#endif  // FEATURE_POWER_LIMIT
#endif  // USE_POWERMONITOR
    else {
      type = NULL;
    }
  }
  if (type == NULL) {
    blinks = 1;
    snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/SYNTAX"), PUB_PREFIX, sysCfg.mqtt_topic);
#ifdef USE_WEBSERVER
    snprintf_P(svalue, sizeof(svalue), PSTR("Status, SaveData, SaveSate, Upgrade, Otaurl, Restart, Reset, WifiConfig, Seriallog, Weblog, Syslog, LogHost, LogPort, SSId, Password%s, Webserver"), (!grpflg) ? ", Hostname" : "");
#else      
    snprintf_P(svalue, sizeof(svalue), PSTR("Status, SaveData, SaveSate, Upgrade, Otaurl, Restart, Reset, WifiConfig, Seriallog, Syslog, LogHost, LogPort, SSId, Password%s"), (!grpflg) ? ", Hostname" : "");
#endif      
    mqtt_publish(stopic, svalue);
    snprintf_P(svalue, sizeof(svalue), PSTR("MqttHost, MqttPort, MqttUser, MqttPassword%s, MqttUnits, MessageFormat, GroupTopic, Timezone, Light, Power, Ledstate, TelePeriod"), (!grpflg) ? ", MqttClient, Topic, ButtonTopic, ButtonRetain" : "");
#ifdef USE_DOMOTICZ 
    mqtt_publish(stopic, svalue);
    snprintf_P(svalue, sizeof(svalue), PSTR("DomoticzInTopic, DomoticzOutTopic, DomoticzIdx, DomoticzKeyIdx, DomoticzUpdateTimer"));
#endif  // USE_DOMOTICZ
#ifdef USE_POWERMONITOR
    mqtt_publish(stopic, svalue);
#ifdef FEATURE_POWER_LIMIT
    snprintf_P(svalue, sizeof(svalue), PSTR("PowerLow, PowerHigh, VoltageLow, VoltageHigh, CurrentLow, CurrentHigh, MaxPL, MaxPLHold, MaxPLWindow, MaxSPL, MaxSPLHold, MaxSPLWindow, MaxDPL, MaxDPLStart"));
#else
    snprintf_P(svalue, sizeof(svalue), PSTR("PowerLow, PowerHigh, VoltageLow, VoltageHigh, CurrentLow, CurrentHigh"));
#endif  // FEATURE_POWER_LIMIT
#endif  // USE_POWERMONITOR
  }
  mqtt_publish(stopic, svalue);
}

/********************************************************************************************/

void send_button_power(byte device, byte state)
{
  char stopic[TOPSZ], svalue[TOPSZ], log[LOGSZ];
  byte mask;

  if (device > Maxdevice) device = 1;
  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/%d/%s"), SUB_PREFIX, sysCfg.mqtt_topic2, device, sysCfg.mqtt_subtopic);
  if (state == 3) {
    snprintf_P(svalue, sizeof(svalue), "");
  } else {
    if (state == 2) {
      mask = 0x01 << (device -1);
      state = power & mask ^ mask;
    }
    snprintf_P(svalue, sizeof(svalue), PSTR("%s"), (state) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
  }
#ifdef USE_DOMOTICZ
  if (sysCfg.domoticz_key_idx[device -1] && strlen(svalue)) {
    strlcpy(stopic, sysCfg.domoticz_in_topic, sizeof(stopic));
    snprintf_P(svalue, sizeof(svalue), PSTR("{\"command\":\"switchlight\", \"idx\":%d, \"switchcmd\":\"%s\"}"),
      sysCfg.domoticz_key_idx[device -1], (state) ? "On" : "Off");
    mqtt_publish(stopic, svalue);
  } else {
    mqtt_publish(stopic, svalue, sysCfg.mqtt_retain);
  }
#else
  mqtt_publish(stopic, svalue, sysCfg.mqtt_retain);
#endif  // USE_DOMOTICZ    
}

void do_cmnd_power(byte device, byte state)
{
  char stopic[TOPSZ], svalue[TOPSZ];

  if (device > Maxdevice) device = 1;
  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/%d/%s"), SUB_PREFIX, sysCfg.mqtt_topic2, device, sysCfg.mqtt_subtopic);
  snprintf_P(svalue, sizeof(svalue), PSTR("%d"), state);
  mqttDataCb(stopic, (byte*)svalue, strlen(svalue));
}

void do_cmnd(char *cmnd)
{
  char stopic[TOPSZ], svalue[128];
  char *token;

  token = strtok(cmnd, " ");
  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/%s"), SUB_PREFIX, sysCfg.mqtt_topic, token);
  token = strtok(NULL, "");
  snprintf_P(svalue, sizeof(svalue), PSTR("%s"), (token == NULL) ? "" : token);
  mqttDataCb(stopic, (byte*)svalue, strlen(svalue));
}

#ifdef USE_POWERMONITOR
boolean hlw_margin(byte type, uint16_t margin, uint16_t value, byte &flag, byte &saveflag)
{
  byte change;

  if (!margin) return false;
  change = saveflag;
  if (type) {
    flag = (value > margin);
  } else {
    flag = (value < margin);
  }
  saveflag = flag;
  return (change != saveflag);
}

void hlw_margin_chk()
{
  char log[LOGSZ], stopic[TOPSZ], svalue[MESSZ];
  float ped, pi, pc;
  uint16_t uped, piv, pe, pw, pu;
  byte flag;

  if (power_steady_cntr) {
    power_steady_cntr--;
    return;
  }

  hlw_readEnergy(0, ped, pe, pw, pu, pi, pc);
  if (power && (sysCfg.hlw_pmin || sysCfg.hlw_pmax || sysCfg.hlw_umin || sysCfg.hlw_umax || sysCfg.hlw_imin || sysCfg.hlw_imax)) {
    piv = (uint16_t)(pi * 1000);

//    snprintf_P(log, sizeof(log), PSTR("HLW: W %d, U %d, I %d"), pw, pu, piv);
//    addLog(LOG_LEVEL_DEBUG, log);
    
    if (hlw_margin(0, sysCfg.hlw_pmin, pw, flag, hlw_pminflg)) {
      snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/POWER_LOW"), PUB_PREFIX2, sysCfg.mqtt_topic);
      strlcpy(svalue, (flag) ? MQTT_STATUS_ON : MQTT_STATUS_OFF, sizeof(svalue));
      mqtt_publish(stopic, svalue);
    }
    if (hlw_margin(1, sysCfg.hlw_pmax, pw, flag, hlw_pmaxflg)) {
      snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/POWER_HIGH"), PUB_PREFIX2, sysCfg.mqtt_topic);
      strlcpy(svalue, (flag) ? MQTT_STATUS_ON : MQTT_STATUS_OFF, sizeof(svalue));
      mqtt_publish(stopic, svalue);
    }
    if (hlw_margin(0, sysCfg.hlw_umin, pu, flag, hlw_uminflg)) {
      snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/VOLTAGE_LOW"), PUB_PREFIX2, sysCfg.mqtt_topic);
      strlcpy(svalue, (flag) ? MQTT_STATUS_ON : MQTT_STATUS_OFF, sizeof(svalue));
      mqtt_publish(stopic, svalue);
    }
    if (hlw_margin(1, sysCfg.hlw_umax, pw, flag, hlw_umaxflg)) {
      snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/VOLTAGE_HIGH"), PUB_PREFIX2, sysCfg.mqtt_topic);
      strlcpy(svalue, (flag) ? MQTT_STATUS_ON : MQTT_STATUS_OFF, sizeof(svalue));
      mqtt_publish(stopic, svalue);
    }
    if (hlw_margin(0, sysCfg.hlw_imin, piv, flag, hlw_iminflg)) {
      snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/CURRENT_LOW"), PUB_PREFIX2, sysCfg.mqtt_topic);
      strlcpy(svalue, (flag) ? MQTT_STATUS_ON : MQTT_STATUS_OFF, sizeof(svalue));
      mqtt_publish(stopic, svalue);
    }
    if (hlw_margin(1, sysCfg.hlw_imax, piv, flag, hlw_imaxflg)) {
      snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/CURRENT_HIGH"), PUB_PREFIX2, sysCfg.mqtt_topic);
      strlcpy(svalue, (flag) ? MQTT_STATUS_ON : MQTT_STATUS_OFF, sizeof(svalue));
      mqtt_publish(stopic, svalue);
    }
  }

#ifdef FEATURE_POWER_LIMIT
  if (sysCfg.hlw_mpl) {
    if (pw > sysCfg.hlw_mpl) {
      if (!hlw_mplh_counter) {
        hlw_mplh_counter = sysCfg.hlw_mplh;
      } else {
        hlw_mplh_counter--;
        if (!hlw_mplh_counter) {
          snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/MAX_POWER_REACHED"), PUB_PREFIX, sysCfg.mqtt_topic);
          snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), pw, (sysCfg.mqtt_units) ? " W" : "");
          mqtt_publish(stopic, svalue);
          do_cmnd_power(1, 0);
          if (!hlw_mplr_counter) hlw_mplr_counter = MAX_POWER_LIMIT_RETRY +1;
          hlw_mplw_counter = sysCfg.hlw_mplw;
        }
      }
    }
    else if (power && (pw <= sysCfg.hlw_mpl)) {
      hlw_mplh_counter = 0;
      hlw_mplr_counter = 0;
      hlw_mplw_counter = 0;
    }
    if (!power) {
      if (hlw_mplw_counter) {
        hlw_mplw_counter--;
      } else {
        if (hlw_mplr_counter) {
          hlw_mplr_counter--;
          if (hlw_mplr_counter) {
            snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/MAX_POWER_MONITOR"), PUB_PREFIX, sysCfg.mqtt_topic);
            mqtt_publish(stopic, MQTT_STATUS_ON);
            do_cmnd_power(1, 1);
          } else {
            snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/MAX_POWER_REACHED_RETRY"), PUB_PREFIX, sysCfg.mqtt_topic);
            mqtt_publish(stopic, MQTT_STATUS_OFF);
          }
        }
      }
    }
  }

  if (sysCfg.hlw_mdpl) {
    uped = (uint16_t)(ped * 1000);
    if (!hlw_mdpls_flag && (rtcTime.Hour == sysCfg.hlw_mdpls)) {
      hlw_mdpls_flag = 1;
      snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/MAX_DAILY_ENERGY_MONITOR"), PUB_PREFIX, sysCfg.mqtt_topic);
      mqtt_publish(stopic, MQTT_STATUS_ON);
      do_cmnd_power(1, 1);
    }
    else if (hlw_mdpls_flag && (uped > sysCfg.hlw_mdpl)) {
      snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/MAX_DAILY_ENERGY_REACHED"), PUB_PREFIX, sysCfg.mqtt_topic);
      dtostrf(ped, 1, 3, svalue);
      snprintf_P(svalue, sizeof(svalue), PSTR("%s%s"), svalue, (sysCfg.mqtt_units) ? " kWh" : "");
      mqtt_publish(stopic, svalue);
      do_cmnd_power(1, 0);
    }
  }
#endif  // FEATURE_POWER_LIMIT
}
#endif  // USE_POWERMONITOR

void every_second_cb()
{
  // 1 second rtc interrupt routine
  // Keep this code small (every_second is to large - it'll trip exception)
  
#ifdef FEATURE_POWER_LIMIT
  if (rtc_loctime() == rtc_midnight()) {
    hlw_mdpls_flag = 0;
    
  }
#endif  // FEATURE_POWER_LIMIT
}

void every_second()
{
  char log[LOGSZ], stopic[TOPSZ], svalue[MESSZ], stemp1[10], stemp2[10], stemp3[10];
  float t, h, ped, pi, pc;
  uint16_t pe, pw, pu;
  byte i;


  if (syslog_timer) {  // Restore syslog level
    syslog_timer--;
    if (!syslog_timer) {
      syslog_level = sysCfg.syslog_level;
      if (sysCfg.syslog_level) {
        addLog(LOG_LEVEL_INFO, "SYSL: Syslog logging re-enabled");  // Might trigger disable again (on purpose)
      }
    }
  }

  if (status_update_timer) {
    status_update_timer--;
    if (!status_update_timer) {
      for (i = 0; i < Maxdevice; i++) {
        snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/%d/%s"), PUB_PREFIX, sysCfg.mqtt_topic, i +1, sysCfg.mqtt_subtopic);
        strlcpy(svalue, (power & (0x01 << i)) ? MQTT_STATUS_ON : MQTT_STATUS_OFF, sizeof(svalue));
        mqtt_publish(stopic, svalue);
      }
    }
  }

#ifdef USE_DOMOTICZ
  if ((sysCfg.domoticz_update_timer || domoticz_update_timer) && sysCfg.domoticz_relay_idx[0] && (strlen(sysCfg.domoticz_in_topic) != 0)) {
    domoticz_update_timer--;
    if (domoticz_update_timer <= 0) {
      domoticz_update_timer = sysCfg.domoticz_update_timer;
      strlcpy(stopic, sysCfg.domoticz_in_topic, sizeof(stopic));
      for (i = 0; i < Maxdevice; i++) {
        if (sysCfg.domoticz_relay_idx[i]) {
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"idx\":%d, \"nvalue\":%d, \"svalue\":\"\"}"),
            sysCfg.domoticz_relay_idx[i], (power & (0x01 << i)) ? 1 : 0);
          mqtt_publish(stopic, svalue);
        }
      }
    }
  }
#endif  // USE_DOMOTICZ

  if (sysCfg.tele_period) {
    tele_period++;
    if (tele_period == sysCfg.tele_period -1) {
      
#ifdef SEND_TELEMETRY_DS18B20
      dsb_readTempPrep();
#endif  // SEND_TELEMETRY_DS18B20

#ifdef SEND_TELEMETRY_DHT
      dht_readPrep();
#endif  // SEND_TELEMETRY_DHT

    }
    if (tele_period >= sysCfg.tele_period) {
      tele_period = 0;

#ifdef SEND_TELEMETRY_UPTIME
      snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/UPTIME"), PUB_PREFIX2, sysCfg.mqtt_topic);
      snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), uptime, (sysCfg.mqtt_units) ? " Hr" : "");
      mqtt_publish(stopic, svalue);
#endif  // SEND_TELEMETRY_UPTIME

#ifdef SEND_TELEMETRY_RSSI
      snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/RSSI"), PUB_PREFIX2, sysCfg.mqtt_topic);
      snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), WIFI_getRSSIasQuality(WiFi.RSSI()), (sysCfg.mqtt_units) ? " %" : "");
      mqtt_publish(stopic, svalue);
#endif  // SEND_TELEMETRY_RSSI

#ifdef SEND_TELEMETRY_DS18B20
      if (dsb_readTemp(t)) {                 // Check if read failed
        snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/TEMPERATURE"), PUB_PREFIX2, sysCfg.mqtt_topic);
        dtostrf(t, 1, 1, stemp1);
        snprintf_P(svalue, sizeof(svalue), PSTR("%s%s"), stemp1, (sysCfg.mqtt_units) ? " C" : "");
        mqtt_publish(stopic, svalue);
      }
#endif  // SEND_TELEMETRY_DS18B20

#ifdef SEND_TELEMETRY_DHT
      if (dht_readTempHum(false, t, h)) {     // Read temperature as Celsius (the default)
        dtostrf(t, 1, 1, stemp1);
        dtostrf(h, 1, 1, stemp2);
        if (sysCfg.message_format == JSON) {
          snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/SENSOR"), PUB_PREFIX2, sysCfg.mqtt_topic);
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"Temperature\":\"%s\", \"Humidity\":%s\"}"), stemp1, stemp2);
        } else {
          snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/TEMPERATURE"), PUB_PREFIX2, sysCfg.mqtt_topic);
          snprintf_P(svalue, sizeof(svalue), PSTR("%s%s"), stemp1, (sysCfg.mqtt_units) ? " C" : "");
          mqtt_publish(stopic, svalue);
          snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/HUMIDITY"), PUB_PREFIX2, sysCfg.mqtt_topic);
          snprintf_P(svalue, sizeof(svalue), PSTR("%s%s"), stemp2, (sysCfg.mqtt_units) ? " %" : "");
        }
        mqtt_publish(stopic, svalue);
      }
#endif  // SEND_TELEMETRY_DHT

#ifdef USE_POWERMONITOR
#ifdef SEND_TELEMETRY_ENERGY
      hlw_readEnergy(1, ped, pe, pw, pu, pi, pc);
      dtostrf(ped, 1, 3, stemp1);
      dtostrf(pc, 1, 2, stemp2);
      dtostrf(pi, 1, 3, stemp3);
      if (sysCfg.message_format == JSON) {
        snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/ENERGY"), PUB_PREFIX2, sysCfg.mqtt_topic);
        snprintf_P(svalue, sizeof(svalue), PSTR("{\"Total Energy Today\":\"%s\", \"Period Energy\":%d, \"Current Power\":%d, \"Power Factor\":\"%s\", \"Voltage\":%d, \"Current\":\"%s\"}"),
          stemp1, pe, pw, stemp2, pu, stemp3);
      } else {
        snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/TODAY_ENERGY"), PUB_PREFIX2, sysCfg.mqtt_topic);
        snprintf_P(svalue, sizeof(svalue), PSTR("%s%s"), stemp1, (sysCfg.mqtt_units) ? " kWh" : "");
        mqtt_publish(stopic, svalue);
        snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/PERIOD_ENERGY"), PUB_PREFIX2, sysCfg.mqtt_topic);
        snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), pe, (sysCfg.mqtt_units) ? " Wh" : "");
        mqtt_publish(stopic, svalue);
        snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/CURRENT_POWER"), PUB_PREFIX2, sysCfg.mqtt_topic);
        snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), pw, (sysCfg.mqtt_units) ? " W" : "");
        mqtt_publish(stopic, svalue);
        snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/POWER_FACTOR"), PUB_PREFIX2, sysCfg.mqtt_topic);
        mqtt_publish(stopic, stemp2);
        snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/VOLTAGE"), PUB_PREFIX2, sysCfg.mqtt_topic);
        snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), pu, (sysCfg.mqtt_units) ? " V" : "");
        mqtt_publish(stopic, svalue);
        snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/CURRENT"), PUB_PREFIX2, sysCfg.mqtt_topic);
        snprintf_P(svalue, sizeof(svalue), PSTR("%s%s"), stemp3, (sysCfg.mqtt_units) ? " A" : "");
      }
      mqtt_publish(stopic, svalue);
#endif  // SEND_TELEMETRY_ENERGY
#endif  // USE_POWERMONITOR

#ifdef SEND_TELEMETRY_POWER
      for (i = 0; i < Maxdevice; i++) {
        if (Maxdevice == 1) {  // Legacy
          snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/%s"), PUB_PREFIX2, sysCfg.mqtt_topic, sysCfg.mqtt_subtopic);
        } else {
          snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/%d/%s"), PUB_PREFIX2, sysCfg.mqtt_topic, i +1, sysCfg.mqtt_subtopic);
        }
        strlcpy(svalue, (power & (0x01 << i)) ? MQTT_STATUS_ON : MQTT_STATUS_OFF, sizeof(svalue));
        mqtt_publish(stopic, svalue);
      }
#endif  // SEND_TELEMETRY_POWER

      snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/TIME"), PUB_PREFIX2, sysCfg.mqtt_topic);
      snprintf_P(svalue, sizeof(svalue), PSTR("%04d-%02d-%02dT%02d:%02d:%02d"),
        rtcTime.Year, rtcTime.Month, rtcTime.Day, rtcTime.Hour, rtcTime.Minute, rtcTime.Second);
      mqtt_publish(stopic, svalue);
    }
  }

#ifdef USE_POWERMONITOR
  hlw_margin_chk();
#endif  // USE_POWERMONITOR
  
  if ((rtcTime.Minute == 2) && (rtcTime.Second == 30)) { 
    uptime++;
    snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/UPTIME"), PUB_PREFIX2, sysCfg.mqtt_topic);
    snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), uptime, (sysCfg.mqtt_units) ? " Hr" : "");
    mqtt_publish(stopic, svalue);
  }
}

void stateloop()
{
  uint8_t button, flag;
  char scmnd[20], log[LOGSZ], stopic[TOPSZ], svalue[TOPSZ];
  
  timerxs = millis() + (1000 / STATES);
  state++;
  if (state == STATES) {             // Every second
    state = 0;
    every_second();
  }

  if ((sysCfg.model >= SONOFF_DUAL) && (sysCfg.model <= CHANNEL_4)) {
    if (ButtonCode) {
      snprintf_P(log, sizeof(log), PSTR("APP: Button code %04X"), ButtonCode);
      addLog(LOG_LEVEL_DEBUG, log);
      button = PRESSED;
/*
      if ((ButtonCode >> 8) == 0x04) {
        if ((ButtonCode & 0x02) != (power & 0x02)) {
          multiwindow = STATES /2;
          multipress = 1;
        }
      } else {
*/
        if (ButtonCode == 0xF500) holdcount = (STATES *4) -1;
/*
      }
*/
      ButtonCode = 0;
    } else {
      button = NOT_PRESSED;
    }
  } else {
    button = digitalRead(KEY_PIN);
  }
  if ((button == PRESSED) && (lastbutton == NOT_PRESSED)) {
    multipress = (multiwindow) ? multipress +1 : 1;
    snprintf_P(log, sizeof(log), PSTR("APP: Multipress %d"), multipress);
    addLog(LOG_LEVEL_DEBUG, log);
    blinks = 1;
    multiwindow = STATES /2;         // 1/2 second multi press window
  }
  lastbutton = button;
  if (button == NOT_PRESSED) {
    holdcount = 0;
  } else {
    holdcount++;
    if (holdcount == (STATES *4)) {  // 4 seconds button hold
      snprintf_P(scmnd, sizeof(scmnd), PSTR("reset 1"));
      multipress = 0;
      do_cmnd(scmnd);
    }
  }
  if (multiwindow) {
    multiwindow--;
  } else {
    if ((!restartflag) && (!holdcount) && (multipress > 0) && (multipress < MAX_BUTTON_COMMANDS +3)) {
      if ((sysCfg.model >= SONOFF_DUAL) && (sysCfg.model <= CHANNEL_4)) {
        flag = ((multipress == 1) || (multipress == 2));
      } else  {
        flag = (multipress == 1);
      }
      if (flag && mqttClient.connected() && strcmp(sysCfg.mqtt_topic2,"0")) {
        send_button_power(multipress, 2);  // Execute command via MQTT using ButtonTopic to sync external clients
      } else {
        if ((multipress == 1) || (multipress == 2)) {
          if (WIFI_State()) {  // WPSconfig, Smartconfig or Wifimanager active
            restartflag = 1;
          } else {
            do_cmnd_power(multipress, 2);    // Execute command internally
          }
        } else {
          snprintf_P(scmnd, sizeof(scmnd), commands[multipress -3]);
          do_cmnd(scmnd);             
        }
      }  
      multipress = 0;
    }
  }

  if (!(state % ((STATES/10)*2))) {
    if (blinks || restartflag || otaflag) {
      if (restartflag || otaflag) {
        blinkstate = 1;   // Stay lit
      } else {
        blinkstate ^= 1;  // Blink
      }
      digitalWrite(LED_PIN, (LED_INVERTED) ? !blinkstate : blinkstate);
      if (!blinkstate) blinks--;
    } else {
      if (sysCfg.ledstate) {
        digitalWrite(LED_PIN, (LED_INVERTED) ? !power : power);
      }
    }
  }
  
  switch (state) {
  case (STATES/10)*2:
    if (otaflag) {
      otaflag--;
      if (otaflag <= 0) {
        otaflag = 12;
        ESPhttpUpdate.rebootOnUpdate(false);
        otaok = (ESPhttpUpdate.update(sysCfg.otaUrl) == HTTP_UPDATE_OK);
      }
      if (otaflag == 10) {  // Allow MQTT to reconnect
        otaflag = 0;
        snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/UPGRADE"), PUB_PREFIX, sysCfg.mqtt_topic);
        if (otaok) {
          snprintf_P(svalue, sizeof(svalue), PSTR("Successful. Restarting"));
          restartflag = 2;
        } else {
          snprintf_P(svalue, sizeof(svalue), PSTR("Failed %s"), ESPhttpUpdate.getLastErrorString().c_str());
        }
        mqtt_publish(stopic, svalue);
      }
    }
    break;
  case (STATES/10)*4:
    if (savedatacounter) {
      savedatacounter--;
      if (savedatacounter <= 0) {
        if (sysCfg.savestate) sysCfg.power = power;
        CFG_Save();
        savedatacounter = sysCfg.savedata;
      }
    }
    if (restartflag) {
      if (restartflag == 211) {
        CFG_Default();
        restartflag = 2;
      }
      if (restartflag == 212) {
        CFG_Erase();
        CFG_Default();
        restartflag = 2;
      }
      if (sysCfg.savestate) sysCfg.power = power;
      CFG_Save();
      restartflag--;
      if (restartflag <= 0) {
        addLog_P(LOG_LEVEL_INFO, PSTR("APP: Restarting"));
        ESP.restart();
      }
    }
    break;
  case (STATES/10)*6:
    WIFI_Check(wificheckflag);
    wificheckflag = WIFI_STATUS;
    break;
  case (STATES/10)*8:
    if ((WiFi.status() == WL_CONNECTED) && (!mqttClient.connected())) {
      if (!mqttcounter) {
        mqtt_reconnect();
      } else {
        mqttcounter--;
      }
    }
    break;
  }
}

void serial()
{
  char log[LOGSZ];

  while (Serial.available()) {
    yield();
    SerialInByte = Serial.read();

    // Sonoff dual 19200 baud serial interface
    if (Hexcode) {
      Hexcode--;
      if (Hexcode) {
        ButtonCode = (ButtonCode << 8) | SerialInByte;
        SerialInByte = 0;
      } else {
        if (SerialInByte != 0xA1) ButtonCode = 0;  // 0xA1 - End of Sonoff dual button code
      }
    }
    if (SerialInByte == 0xA0) {                    // 0xA0 - Start of Sonoff dual button code
      SerialInByte = 0;
      ButtonCode = 0;
      Hexcode = 3;
    }

    if (SerialInByte > 127) { // binary data...
      SerialInByteCounter = 0;
      Serial.flush();
      return;
    }
    if (isprint(SerialInByte)) {
      if (SerialInByteCounter < INPUT_BUFFER_SIZE) {  // add char to string if it still fits
        serialInBuf[SerialInByteCounter++] = SerialInByte;
      } else {
        SerialInByteCounter = 0;
      }
    }
    if (SerialInByte == '\n') {
      serialInBuf[SerialInByteCounter] = 0;  // serial data completed
      if (sysCfg.seriallog_level < LOG_LEVEL_INFO) sysCfg.seriallog_level = LOG_LEVEL_INFO;
      snprintf_P(log, sizeof(log), PSTR("CMND: %s"), serialInBuf);
      addLog(LOG_LEVEL_INFO, log);
      do_cmnd(serialInBuf);
      SerialInByteCounter = 0;
      Serial.flush();
      return;
    }
  }
}

/********************************************************************************************/

void setup()
{
  char log[LOGSZ];
  byte idx;

  Serial.begin(Baudrate);
  delay(10);
  Serial.println();
  sysCfg.seriallog_level = LOG_LEVEL_INFO;  // Allow specific serial messages until config loaded

  snprintf_P(Version, sizeof(Version), PSTR("%d.%d.%d"), VERSION >> 24 & 0xff, VERSION >> 16 & 0xff, VERSION >> 8 & 0xff);
  if (VERSION & 0x1f) {
    idx = strlen(Version);
    Version[idx] = 96 + (VERSION & 0x1f);
    Version[idx +1] = 0;
  }
  if (!spiffsPresent())
    addLog_P(LOG_LEVEL_ERROR, PSTR("SPIFFS: ERROR - No spiffs present. Please reflash with at least 16K SPIFFS"));
#ifdef USE_SPIFFS
  initSpiffs();
#endif
  CFG_Load();
  CFG_Delta();
  
  if (!sysCfg.model) {
#if MODULE == SONOFF
    pinMode(REL_PIN, INPUT_PULLUP);
    sysCfg.model = digitalRead(REL_PIN) +1;  // SONOFF (1) or SONOFF_DUAL (2)
#else
    sysCfg.model = SONOFF;
#endif
  }
  
  if ((sysCfg.model >= SONOFF_DUAL) && (sysCfg.model <= CHANNEL_4)) {
    Baudrate = 19200;
    Maxdevice = sysCfg.model;
  }
  if (Serial.baudRate() != Baudrate) {
    snprintf_P(log, sizeof(log), PSTR("APP: Need to change baudrate to %d"), Baudrate);
    addLog(LOG_LEVEL_INFO, log);
    delay(100);
    Serial.flush();
    Serial.begin(Baudrate);
    delay(10);
    Serial.println();
  }
  
  sysCfg.bootcount++;
  snprintf_P(log, sizeof(log), PSTR("APP: Bootcount %d"), sysCfg.bootcount);
  addLog(LOG_LEVEL_DEBUG, log);
  savedatacounter = sysCfg.savedata;
  power = ((0x00FF << Maxdevice) >> 8) & sysCfg.power;
  syslog_level = sysCfg.syslog_level;

  if (strstr(sysCfg.hostname, "%")) strlcpy(sysCfg.hostname, DEF_WIFI_HOSTNAME, sizeof(sysCfg.hostname));
  if (!strcmp(sysCfg.hostname, DEF_WIFI_HOSTNAME)) {
    snprintf_P(Hostname, sizeof(Hostname)-1, sysCfg.hostname, sysCfg.mqtt_topic, ESP.getChipId() & 0x1FFF);
  } else {
    snprintf_P(Hostname, sizeof(Hostname)-1, sysCfg.hostname);
  }
  WIFI_Connect(Hostname);

  if (strstr(sysCfg.mqtt_client, "%")) strlcpy(sysCfg.mqtt_client, DEF_MQTT_CLIENT_ID, sizeof(sysCfg.mqtt_client));
  if (!strcmp(sysCfg.mqtt_client, DEF_MQTT_CLIENT_ID)) {
    snprintf_P(MQTTClient, sizeof(MQTTClient), sysCfg.mqtt_client, ESP.getChipId());
  } else {
    snprintf_P(MQTTClient, sizeof(MQTTClient), sysCfg.mqtt_client);
  }
  mqttClient.setServer(sysCfg.mqtt_host, sysCfg.mqtt_port);
  mqttClient.setCallback(mqttDataCb);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, (LED_INVERTED) ? !blinkstate : blinkstate);

  if ((sysCfg.model < SONOFF_DUAL) || (sysCfg.model > CHANNEL_8)) {
    pinMode(KEY_PIN, INPUT_PULLUP);
    pinMode(REL_PIN, OUTPUT);
  }
  if (sysCfg.savestate) setRelay(power);

  rtc_init(every_second_cb);

#ifdef SEND_TELEMETRY_DHT
  dht_init();
#endif

#ifdef USE_POWERMONITOR
  hlw_init();
#endif  // USE_POWERMONITOR

  snprintf_P(log, sizeof(log), PSTR("APP: Project %s (Topic %s, Fallback %s, GroupTopic %s) Version %s"),
    PROJECT, sysCfg.mqtt_topic, MQTTClient, sysCfg.mqtt_grptopic, Version);
  addLog(LOG_LEVEL_INFO, log);
}

void loop()
{
#ifdef USE_WEBSERVER
  pollDnsWeb();
#endif  // USE_WEBSERVER

  if (millis() >= timerxs) stateloop();

  mqttClient.loop();

  if (Serial.available()) serial();
 
  yield();
}

