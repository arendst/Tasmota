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

#define VERSION                0x03000400   // 3.0.3

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
enum wifi_t  {WIFI_RESTART, WIFI_SMARTCONFIG, WIFI_MANAGER, WIFI_WPSCONFIG};
enum msgf_t  {LEGACY, JSON, MAX_FORMAT};
enum swtch_t {TOGGLE, FOLLOW, FOLLOW_INV, PUSHBUTTON, PUSHBUTTON_INV, MAX_SWITCH_OPTION};

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
#define MAX_POWER_HOLD         10           // Time in SECONDS to allow max agreed power
#define MAX_POWER_WINDOW       30           // Time in SECONDS to disable allow max agreed power
#define SAFE_POWER_HOLD        10           // Time in SECONDS to allow max unit safe power
#define SAFE_POWER_WINDOW      30           // Time in MINUTES to disable allow max unit safe power
#define MAX_POWER_RETRY        5            // Retry count allowing agreed power limit overflow

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

#ifndef SWITCH_MODE
#define SWITCH_MODE            TOGGLE       // TOGGLE, FOLLOW or FOLLOW_INV (the wall switch state)
#endif

#ifndef MQTT_FINGERPRINT
#define MQTT_FINGERPRINT       "A5 02 FF 13 99 9F 8B 39 8E F1 83 4F 11 23 65 0B 32 36 FC 07"
#endif

#define DEF_WIFI_HOSTNAME      "%s-%04d"    // Expands to <MQTT_TOPIC>-<last 4 decimal chars of MAC address>

#define HLW_PREF_PULSE         12530        // was 4975us = 201Hz = 1000W
#define HLW_UREF_PULSE         1950         // was 1666us = 600Hz = 220V
#define HLW_IREF_PULSE         3500         // was 1666us = 600Hz = 4.545A

#define MQTT_UNITS             0            // Default do not show value units (Hr, Sec, V, A, W etc.)
#define MQTT_SUBTOPIC          "POWER"      // Default MQTT subtopic (POWER or LIGHT)
#define MQTT_RETRY_SECS        10           // Seconds to retry MQTT connection
#define APP_POWER              0            // Default saved power state Off
#define MAX_DEVICE             1            // Max number of devices

#define STATES                 10           // loops per second
#define SYSLOG_TIMER           600          // Seconds to restore syslog_level

#define INPUT_BUFFER_SIZE      100          // Max number of characters in serial buffer
#define TOPSZ                  60           // Max number of characters in topic string
#define MESSZ                  240          // Max number of characters in JSON message string
#define LOGSZ                  128          // Max number of characters in log string
#ifdef USE_MQTT_TLS
  #define MAX_LOG_LINES        10           // Max number of lines in weblog
#else
  #define MAX_LOG_LINES        70           // Max number of lines in weblog
#endif

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
#ifdef SEND_TELEMETRY_I2C
  #include <Wire.h>                         // I2C support library
#endif // SEND_TELEMETRY_I2C

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

const char wificfg[4][12] PROGMEM = { "Restart", "Smartconfig", "Wifimanager", "WPSconfig" };

struct SYSCFG2 {
  unsigned long cfg_holder;
  unsigned long saveFlag;
  unsigned long version;
  byte          seriallog_level;
  byte          syslog_level;
  char          syslog_host[32];
  char          sta_ssid1[32];
  char          sta_pwd1[64];
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
  unsigned long hlw_kWhyesterday;
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
  uint16_t      hlw_mkwh;   // MaxEnergy
  uint16_t      hlw_mkwhs;  // MaxEnergyStart
  char          domoticz_in_topic[33];
  char          domoticz_out_topic[33];
  uint16_t      domoticz_update_timer;
  unsigned long domoticz_relay_idx[4];
  unsigned long domoticz_key_idx[4];
  byte          message_format;
  unsigned long hlw_kWhtoday;
  uint16_t      hlw_kWhdoy;
  uint8_t       switchmode;
  char          mqtt_fingerprint[60];
  byte          sta_active;
  char          sta_ssid2[33];
  char          sta_pwd2[65];

} sysCfg2;

struct SYSCFG {
  unsigned long cfg_holder;
  unsigned long saveFlag;
  unsigned long version;
  unsigned long bootcount;
  byte          migflag;
  int16_t       savedata;
  byte          savestate;
  byte          model;
  int8_t        timezone;
  char          otaUrl[101];
  char          friendlyname[33];

  byte          serial_enable;
  byte          seriallog_level;
  uint8_t       sta_config;
  byte          sta_active;
  char          sta_ssid[2][33];
  char          sta_pwd[2][65];
  char          hostname[33];
  char          syslog_host[33];
  uint16_t      syslog_port;
  byte          syslog_level;
  uint8_t       webserver;
  byte          weblog_level;

  char          mqtt_fingerprint[60];
  char          mqtt_host[33];
  uint16_t      mqtt_port;
  char          mqtt_client[33];
  char          mqtt_user[33];
  char          mqtt_pwd[33];
  char          mqtt_topic[33];
  char          mqtt_topic2[33];
  char          mqtt_grptopic[33];
  char          mqtt_subtopic[33];
  byte          mqtt_button_retain;
  byte          mqtt_power_retain;
  byte          mqtt_units;
  byte          message_format;
  uint16_t      tele_period;

  uint8_t       power;
  uint8_t       ledstate;
  uint8_t       switchmode;

  char          domoticz_in_topic[33];
  char          domoticz_out_topic[33];
  uint16_t      domoticz_update_timer;
  unsigned long domoticz_relay_idx[4];
  unsigned long domoticz_key_idx[4];

  unsigned long hlw_pcal;
  unsigned long hlw_ucal;
  unsigned long hlw_ical;
  unsigned long hlw_kWhtoday;
  unsigned long hlw_kWhyesterday;
  uint16_t      hlw_kWhdoy;
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
  uint16_t      hlw_mkwh;   // MaxEnergy
  uint16_t      hlw_mkwhs;  // MaxEnergyStart

} sysCfg;

struct TIME_T {
  uint8_t       Second;
  uint8_t       Minute;
  uint8_t       Hour;
  uint8_t       Wday;      // day of week, sunday is day 1
  uint8_t       Day;
  uint8_t       Month;
  char          MonthName[4];
  uint16_t      DayOfYear;
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
int mqttflag = 2;                     // MQTT connection messages flag
int otaflag = 0;                      // OTA state flag
int otaok;                            // OTA result
int restartflag = 0;                  // Sonoff restart flag
int wificheckflag = WIFI_RESTART;     // Wifi state flag
int uptime = 0;                       // Current uptime in hours
int tele_period = 0;                  // Tele period timer
String Log[MAX_LOG_LINES];            // Web log buffer
byte logidx = 0;                      // Index in Web log buffer
byte Maxdevice = MAX_DEVICE;          // Max number of devices supported
int status_update_timer = 0;          // Refresh initial status

#ifdef USE_MQTT_TLS
  WiFiClientSecure espClient;         // Wifi Secure Client
#else
  WiFiClient espClient;               // Wifi Client
#endif
PubSubClient mqttClient(espClient);   // MQTT Client
WiFiUDP portUDP;                      // UDP Syslog and Alexa

uint8_t power;                        // Current copy of sysCfg.power
byte syslog_level;                    // Current copy of sysCfg.syslog_level
uint16_t syslog_timer = 0;            // Timer to re-enable syslog_level

int blinks = 1;                       // Number of LED blinks
uint8_t blinkstate = 0;               // LED state

uint8_t lastbutton = NOT_PRESSED;     // Last button state
uint8_t holdcount = 0;                // Timer recording button hold
uint8_t multiwindow = 0;              // Max time between button presses to record press count
uint8_t multipress = 0;               // Number of button presses within multiwindow
uint8_t lastbutton2 = NOT_PRESSED;    // Last button 2 state

boolean udpConnected = false;
#ifdef USE_WEMO_EMULATION
  #define WEMO_BUFFER_SIZE 200        // Max UDP buffer size needed for M-SEARCH message

  char packetBuffer[WEMO_BUFFER_SIZE]; // buffer to hold incoming UDP packet
  IPAddress ipMulticast(239, 255, 255, 250); // Simple Service Discovery Protocol (SSDP)
  uint32_t portMulticast = 1900;      // Multicast address and port
#endif  // USE_WEMO_EMULATION

#ifdef USE_WALL_SWITCH
  uint8_t lastwallswitch;             // Last wall switch state
#endif  // USE_WALL_SWITCH

#ifdef USE_POWERMONITOR
  byte hlw_pminflg = 0;
  byte hlw_pmaxflg = 0;
  byte hlw_uminflg = 0;
  byte hlw_umaxflg = 0;
  byte hlw_iminflg = 0;
  byte hlw_imaxflg = 0;
  byte power_steady_cntr;
#ifdef FEATURE_POWER_LIMIT
  byte hlw_mkwh_state = 0;
  byte hlw_mplr_counter = 0;
  uint16_t hlw_mplh_counter = 0;
  uint16_t hlw_mplw_counter = 0;
#endif  // FEATURE_POWER_LIMIT
#endif  // USE_POWERMONITOR

#ifdef USE_DOMOTICZ
  int domoticz_update_timer = 0;
  byte domoticz_update_flag = 1;
#endif  // USE_DOMOTICZ

/********************************************************************************************/

void CFG_Default()
{
  addLog_P(LOG_LEVEL_NONE, PSTR("Config: Use default configuration"));
  memset(&sysCfg, 0x00, sizeof(SYSCFG));

  sysCfg.cfg_holder = CFG_HOLDER;
  sysCfg.saveFlag = 0;
  sysCfg.version = VERSION;
  sysCfg.bootcount = 0;
  sysCfg.migflag = 0;
  sysCfg.savedata = SAVE_DATA;
  sysCfg.savestate = SAVE_STATE;
  sysCfg.model = 0;
  sysCfg.timezone = APP_TIMEZONE;
  strlcpy(sysCfg.otaUrl, OTA_URL, sizeof(sysCfg.otaUrl));
  strlcpy(sysCfg.friendlyname, MQTT_TOPIC, sizeof(sysCfg.friendlyname));

  sysCfg.seriallog_level = SERIAL_LOG_LEVEL;
  sysCfg.sta_active = 0;
  strlcpy(sysCfg.sta_ssid[0], STA_SSID1, sizeof(sysCfg.sta_ssid[0]));
  strlcpy(sysCfg.sta_pwd[0], STA_PASS1, sizeof(sysCfg.sta_pwd[0]));
  strlcpy(sysCfg.sta_ssid[1], STA_SSID2, sizeof(sysCfg.sta_ssid[1]));
  strlcpy(sysCfg.sta_pwd[1], STA_PASS2, sizeof(sysCfg.sta_pwd[1]));
  strlcpy(sysCfg.hostname, WIFI_HOSTNAME, sizeof(sysCfg.hostname));
  sysCfg.sta_config = WIFI_CONFIG_TOOL;
  strlcpy(sysCfg.syslog_host, SYS_LOG_HOST, sizeof(sysCfg.syslog_host));
  sysCfg.syslog_port = SYS_LOG_PORT;
  sysCfg.syslog_level = SYS_LOG_LEVEL;
  sysCfg.webserver = WEB_SERVER;
  sysCfg.weblog_level = WEB_LOG_LEVEL;

  strlcpy(sysCfg.mqtt_fingerprint, MQTT_FINGERPRINT, sizeof(sysCfg.mqtt_fingerprint));
  strlcpy(sysCfg.mqtt_host, MQTT_HOST, sizeof(sysCfg.mqtt_host));
  sysCfg.mqtt_port = MQTT_PORT;
  strlcpy(sysCfg.mqtt_client, MQTT_CLIENT_ID, sizeof(sysCfg.mqtt_client));
  strlcpy(sysCfg.mqtt_user, MQTT_USER, sizeof(sysCfg.mqtt_user));
  strlcpy(sysCfg.mqtt_pwd, MQTT_PASS, sizeof(sysCfg.mqtt_pwd));
  strlcpy(sysCfg.mqtt_topic, MQTT_TOPIC, sizeof(sysCfg.mqtt_topic));
  strlcpy(sysCfg.mqtt_topic2, "0", sizeof(sysCfg.mqtt_topic2));
  strlcpy(sysCfg.mqtt_grptopic, MQTT_GRPTOPIC, sizeof(sysCfg.mqtt_grptopic));
  strlcpy(sysCfg.mqtt_subtopic, MQTT_SUBTOPIC, sizeof(sysCfg.mqtt_subtopic));
  sysCfg.mqtt_button_retain = MQTT_BUTTON_RETAIN;
  sysCfg.mqtt_power_retain = MQTT_POWER_RETAIN;
  sysCfg.mqtt_units = MQTT_UNITS;
  sysCfg.message_format = MESSAGE_FORMAT;
  sysCfg.tele_period = TELE_PERIOD;

  sysCfg.power = APP_POWER;
  sysCfg.ledstate = APP_LEDSTATE;
  sysCfg.switchmode = SWITCH_MODE;

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

  sysCfg.hlw_pcal = HLW_PREF_PULSE;
  sysCfg.hlw_ucal = HLW_UREF_PULSE;
  sysCfg.hlw_ical = HLW_IREF_PULSE;
  sysCfg.hlw_kWhtoday = 0;
  sysCfg.hlw_kWhyesterday = 0;
  sysCfg.hlw_kWhdoy = 0;
  sysCfg.hlw_pmin = 0;
  sysCfg.hlw_pmax = 0;
  sysCfg.hlw_umin = 0;
  sysCfg.hlw_umax = 0;
  sysCfg.hlw_imin = 0;
  sysCfg.hlw_imax = 0;
  sysCfg.hlw_mpl = 0;                              // MaxPowerLimit
  sysCfg.hlw_mplh = MAX_POWER_HOLD;
  sysCfg.hlw_mplw = MAX_POWER_WINDOW;
  sysCfg.hlw_mspl = 0;                             // MaxSafePowerLimit
  sysCfg.hlw_msplh = SAFE_POWER_HOLD;
  sysCfg.hlw_msplw = SAFE_POWER_WINDOW;
  sysCfg.hlw_mkwh = 0;                             // MaxEnergy
  sysCfg.hlw_mkwhs = 0;                            // MaxEnergyStart

  CFG_Save();
}

void CFG_Migrate_Part2()
{
  addLog_P(LOG_LEVEL_NONE, PSTR("Config: Migrating configuration"));
  memset(&sysCfg, 0x00, sizeof(SYSCFG));

  sysCfg.cfg_holder = CFG_HOLDER;
  sysCfg.saveFlag = 0;
  sysCfg.version = VERSION;
  sysCfg.bootcount = sysCfg2.bootcount;
  sysCfg.migflag = 0;
  sysCfg.savedata = sysCfg2.savedata;
  sysCfg.savestate = sysCfg2.savestate;
  sysCfg.model = sysCfg2.model;
  sysCfg.timezone = sysCfg2.timezone;
  strlcpy(sysCfg.otaUrl, sysCfg2.otaUrl, sizeof(sysCfg.otaUrl));
  strlcpy(sysCfg.friendlyname, sysCfg2.mqtt_client, sizeof(sysCfg.friendlyname));

  sysCfg.seriallog_level = sysCfg2.seriallog_level;
  sysCfg.sta_active = sysCfg2.sta_active;
  strlcpy(sysCfg.sta_ssid[0], sysCfg2.sta_ssid1, sizeof(sysCfg.sta_ssid[0]));
  strlcpy(sysCfg.sta_pwd[0], sysCfg2.sta_pwd1, sizeof(sysCfg.sta_pwd[0]));
  strlcpy(sysCfg.sta_ssid[1], sysCfg2.sta_ssid2, sizeof(sysCfg.sta_ssid[1]));
  strlcpy(sysCfg.sta_pwd[1], sysCfg2.sta_pwd2, sizeof(sysCfg.sta_pwd[1]));
  strlcpy(sysCfg.hostname, sysCfg2.hostname, sizeof(sysCfg.hostname));
  sysCfg.sta_config = sysCfg2.sta_config;
  strlcpy(sysCfg.syslog_host, sysCfg2.syslog_host, sizeof(sysCfg.syslog_host));
  sysCfg.syslog_port = sysCfg2.syslog_port;
  sysCfg.syslog_level = sysCfg2.syslog_level;
  sysCfg.webserver = sysCfg2.webserver;
  sysCfg.weblog_level = sysCfg2.weblog_level;

  strlcpy(sysCfg.mqtt_fingerprint, sysCfg2.mqtt_fingerprint, sizeof(sysCfg.mqtt_fingerprint));
  strlcpy(sysCfg.mqtt_host, sysCfg2.mqtt_host, sizeof(sysCfg.mqtt_host));
  sysCfg.mqtt_port = sysCfg2.mqtt_port;
  strlcpy(sysCfg.mqtt_client, sysCfg2.mqtt_client, sizeof(sysCfg.mqtt_client));
  strlcpy(sysCfg.mqtt_user, sysCfg2.mqtt_user, sizeof(sysCfg.mqtt_user));
  strlcpy(sysCfg.mqtt_pwd, sysCfg2.mqtt_pwd, sizeof(sysCfg.mqtt_pwd));
  strlcpy(sysCfg.mqtt_topic, sysCfg2.mqtt_topic, sizeof(sysCfg.mqtt_topic));
  strlcpy(sysCfg.mqtt_topic2, sysCfg2.mqtt_topic2, sizeof(sysCfg.mqtt_topic2));
  strlcpy(sysCfg.mqtt_grptopic, sysCfg2.mqtt_grptopic, sizeof(sysCfg.mqtt_grptopic));
  strlcpy(sysCfg.mqtt_subtopic, sysCfg2.mqtt_subtopic, sizeof(sysCfg.mqtt_subtopic));
  sysCfg.mqtt_button_retain = sysCfg2.mqtt_retain;
  sysCfg.mqtt_power_retain = MQTT_POWER_RETAIN;
  sysCfg.mqtt_units = sysCfg2.mqtt_units;
  sysCfg.message_format = sysCfg2.message_format;
  sysCfg.tele_period = sysCfg2.tele_period;
  if ((sysCfg.tele_period > 0) && (sysCfg.tele_period < 10)) sysCfg.tele_period = 10;   // Do not allow periods < 10 seconds

  sysCfg.power = sysCfg2.power;
  sysCfg.ledstate = sysCfg2.ledstate;
  sysCfg.switchmode = sysCfg2.switchmode;

  strlcpy(sysCfg.domoticz_in_topic, sysCfg2.domoticz_in_topic, sizeof(sysCfg.domoticz_in_topic));
  strlcpy(sysCfg.domoticz_out_topic, sysCfg2.domoticz_out_topic, sizeof(sysCfg.domoticz_out_topic));
  sysCfg.domoticz_update_timer = sysCfg2.domoticz_update_timer;
  sysCfg.domoticz_relay_idx[0] = sysCfg2.domoticz_relay_idx[0];
  sysCfg.domoticz_relay_idx[1] = sysCfg2.domoticz_relay_idx[1];
  sysCfg.domoticz_relay_idx[2] = sysCfg2.domoticz_relay_idx[2];
  sysCfg.domoticz_relay_idx[3] = sysCfg2.domoticz_relay_idx[3];
  sysCfg.domoticz_key_idx[0] = sysCfg2.domoticz_key_idx[0];
  sysCfg.domoticz_key_idx[1] = sysCfg2.domoticz_key_idx[1];
  sysCfg.domoticz_key_idx[2] = sysCfg2.domoticz_key_idx[2];
  sysCfg.domoticz_key_idx[3] = sysCfg2.domoticz_key_idx[3];

  sysCfg.hlw_pcal = sysCfg2.hlw_pcal;
  sysCfg.hlw_ucal = sysCfg2.hlw_ucal;
  sysCfg.hlw_ical = sysCfg2.hlw_ical;
  sysCfg.hlw_kWhtoday = sysCfg2.hlw_kWhtoday;
  sysCfg.hlw_kWhyesterday = sysCfg2.hlw_kWhyesterday;
  sysCfg.hlw_kWhdoy = sysCfg2.hlw_kWhdoy;
  sysCfg.hlw_pmin = sysCfg2.hlw_pmin;
  sysCfg.hlw_pmax = sysCfg2.hlw_pmax;
  sysCfg.hlw_umin = sysCfg2.hlw_umin;
  sysCfg.hlw_umax = sysCfg2.hlw_umax;
  sysCfg.hlw_imin = sysCfg2.hlw_imin;
  sysCfg.hlw_imax = sysCfg2.hlw_imax;
  sysCfg.hlw_mpl = sysCfg2.hlw_mpl;                              // MaxPowerLimit
  sysCfg.hlw_mplh = sysCfg2.hlw_mplh;
  sysCfg.hlw_mplw = sysCfg2.hlw_mplw;
  sysCfg.hlw_mspl = sysCfg2.hlw_mspl;                             // MaxSafePowerLimit
  sysCfg.hlw_msplh = SAFE_POWER_HOLD;
  sysCfg.hlw_msplw = sysCfg2.hlw_msplw;
  sysCfg.hlw_mkwh = sysCfg.hlw_mkwh;                             // MaxEnergy
  sysCfg.hlw_mkwhs = sysCfg.hlw_mkwhs;                            // MaxEnergyStart

  CFG_Save();
}

void CFG_Delta()
{
  if (sysCfg.version != VERSION) {      // Fix version dependent changes

    sysCfg.version = VERSION;
  }
}

/********************************************************************************************/

void getClient(char* output, const char* input, byte size)
{
  char *token;
  uint8_t digits = 0;

  if (strstr(input, "%")) {
    strlcpy(output, input, size);
    token = strtok(output, "%");
    token = strtok(NULL, "");
    if (token != NULL) {
      digits = atoi(token);
      if (digits) {
        snprintf_P(output, size, PSTR("%s%c0%dX"), output, '%', digits);
        snprintf_P(output, size, output, ESP.getChipId());
      }
    }
  }
  if (!digits) strlcpy(output, input, size);
}

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
#ifdef REL2_PIN
    digitalWrite(REL2_PIN, (power & 0x2));
#endif
  }
#ifdef USE_POWERMONITOR
  power_steady_cntr = 2;
#endif
}

void json2legacy(char* stopic, char* svalue)
{
  char *p, *token;
  uint16_t i, j;

  if (!strstr(svalue, "{\"")) return;  // No JSON

// stopic = stat/sonoff/RESULT
// svalue = {"POWER2":"ON"}
// --> stopic = "stat/sonoff/POWER2", svalue = "ON"
// svalue = {"Upgrade":{"Version":"2.1.2", "OtaUrl":"%s"}}
// --> stopic = "stat/sonoff/UPGRADE", svalue = "2.1.2"
// svalue = {"SerialLog":2}
// --> stopic = "stat/sonoff/SERIALLOG", svalue = "2"
// svalue = {"POWER":""}
// --> stopic = "stat/sonoff/POWER", svalue = ""

  token = strtok(svalue, "{\"");      // Topic
  p = strrchr(stopic, '/') +1;
  i = p - stopic;
  for (j = 0; j < strlen(token)+1; j++) stopic[i+j] = toupper(token[j]);
  token = strtok(NULL, "\"");         // : or :3} or :3, or :{
  if (strstr(token, ":{")) {
    token = strtok(NULL, "\"");       // Subtopic
    token = strtok(NULL, "\"");       // : or :3} or :3,
  }
  if (strlen(token) > 1) {
    token++;
    p = strchr(token, ',');
    if (!p) p = strchr(token, '}');
    i = p - token;
    token[i] = '\0';                  // Value
  } else {
    token = strtok(NULL, "\"");       // Value or , or }
    if ((token[0] == ',') || (token[0] == '}')) {  // Empty parameter
      token = NULL;
    }
  }
  if (token == NULL) {
    svalue[0] = '\0';
  } else {
    memcpy(svalue, token, strlen(token)+1);
  }
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

void mqtt_publishPowerState(byte device)
{
  char stopic[TOPSZ], svalue[MESSZ], sdevice[10];

  if ((device < 1) || (device > Maxdevice)) device = 1;
  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/RESULT"), PUB_PREFIX, sysCfg.mqtt_topic);
  snprintf_P(sdevice, sizeof(sdevice), PSTR("%d"), device);
  snprintf_P(svalue, sizeof(svalue), PSTR("{\"%s%s\":\"%s\"}"),
    sysCfg.mqtt_subtopic, (Maxdevice > 1) ? sdevice : "", (power & (0x01 << (device -1))) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
  if (sysCfg.message_format == JSON) mqtt_publish(stopic, svalue);
  json2legacy(stopic, svalue);
  mqtt_publish(stopic, svalue, sysCfg.mqtt_power_retain);
}

#ifdef USE_DOMOTICZ
void mqtt_publishDomoticzPowerState(byte device)
{
  char svalue[MESSZ];

  if (sysCfg.domoticz_relay_idx[device -1] && (strlen(sysCfg.domoticz_in_topic) != 0)) {
    if ((device < 1) || (device > Maxdevice)) device = 1;
    snprintf_P(svalue, sizeof(svalue), PSTR("{\"idx\":%d, \"nvalue\":%d, \"svalue\":\"\"}"),
      sysCfg.domoticz_relay_idx[device -1], (power & (0x01 << (device -1))) ? 1 : 0);
    mqtt_publish(sysCfg.domoticz_in_topic, svalue);
  }
}
#endif  // USE_DOMOTICZ

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
    if (sysCfg.message_format == JSON) {
      snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/RESULT"), PUB_PREFIX, sysCfg.mqtt_topic);
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Info1\":{\"AppName\":\"" APP_NAME "\", \"Version\":\"%s\", \"FallbackTopic\":\"%s\", \"GroupTopic\":\"%s\"}}"),
        Version, MQTTClient, sysCfg.mqtt_grptopic);
    } else {
      snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/INFO"), PUB_PREFIX, sysCfg.mqtt_topic);
      snprintf_P(svalue, sizeof(svalue), PSTR(APP_NAME " version %s, FallbackTopic %s, GroupTopic %s"), Version, MQTTClient, sysCfg.mqtt_grptopic);
    }
    mqtt_publish(stopic, svalue);
#ifdef USE_WEBSERVER
    if (sysCfg.webserver) {
      if (sysCfg.message_format == JSON) {
        snprintf_P(svalue, sizeof(svalue), PSTR("{\"Info2\":{\"WebserverMode\":\"%s\", \"Hostname\":\"%s\", \"IPaddress\":\"%s\"}}"),
          (sysCfg.webserver == 2) ? "Admin" : "User", Hostname, WiFi.localIP().toString().c_str());
      } else {
        snprintf_P(svalue, sizeof(svalue), PSTR("Webserver active for %s on %s with IP address %s"),
          (sysCfg.webserver == 2) ? "Admin" : "User", Hostname, WiFi.localIP().toString().c_str());
      }
      mqtt_publish(stopic, svalue);
    }
#endif  // USE_WEBSERVER
    if (MQTT_MAX_PACKET_SIZE < (TOPSZ+MESSZ)) {
      if (sysCfg.message_format == JSON) {
        snprintf_P(svalue, sizeof(svalue), PSTR("{\"Warning1\":\"Change MQTT_MAX_PACKET_SIZE in libraries/PubSubClient.h to at least %d\"}"), TOPSZ+MESSZ);
      } else {
        snprintf_P(svalue, sizeof(svalue), PSTR("WARNING: Change MQTT_MAX_PACKET_SIZE in libraries/PubSubClient.h to at least %d"), TOPSZ+MESSZ);
      }
      mqtt_publish(stopic, svalue);
    }
    if (!spiffsPresent()) {
      if (sysCfg.message_format == JSON) {
        snprintf_P(svalue, sizeof(svalue), PSTR("{\"Warning2\":\"No persistent config. Please reflash with at least 16K SPIFFS\"}"));
      } else {
        snprintf_P(svalue, sizeof(svalue), PSTR("WARNING: No persistent config. Please reflash with at least 16K SPIFFS"));
      }
      mqtt_publish(stopic, svalue);
    }
    if (sysCfg.tele_period) tele_period = sysCfg.tele_period -9;
    status_update_timer = 2;
#ifdef USE_DOMOTICZ
    domoticz_update_timer = 2;
#endif  // USE_DOMOTICZ
  }
  mqttflag = 0;
}

void mqtt_reconnect()
{
  char stopic[TOPSZ], svalue[TOPSZ], log[LOGSZ];

  mqttcounter = MQTT_RETRY_SECS;

  if (udpConnected) WiFiUDP::stopAll();
  if (mqttflag > 1) {
#ifdef USE_MQTT_TLS
    addLog_P(LOG_LEVEL_INFO, PSTR("MQTT: Verify TLS fingerprint..."));
    if (!espClient.connect(sysCfg.mqtt_host, sysCfg.mqtt_port)) {
      snprintf_P(log, sizeof(log), PSTR("MQTT: TLS CONNECT FAILED USING WRONG MQTTHost (%s) or MQTTPort (%d). Retry in %d seconds"),
        sysCfg.mqtt_host, sysCfg.mqtt_port, mqttcounter);
      addLog(LOG_LEVEL_DEBUG, log);
      return;
    }
    if (espClient.verify(sysCfg.mqtt_fingerprint, sysCfg.mqtt_host)) {
      addLog_P(LOG_LEVEL_INFO, PSTR("MQTT: Verified"));
    } else {
      addLog_P(LOG_LEVEL_DEBUG, PSTR("MQTT: WARNING - Insecure connection due to invalid Fingerprint"));
    }
#endif  // USE_MQTT_TLS
    mqttClient.setServer(sysCfg.mqtt_host, sysCfg.mqtt_port);
    mqttClient.setCallback(mqttDataCb);
    mqttflag = 1;
    mqttcounter = 1;
    return;
  }

  addLog_P(LOG_LEVEL_INFO, PSTR("MQTT: Attempting connection..."));
  if (sysCfg.message_format == JSON) {
    snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/TELEMETRY"), PUB_PREFIX2, sysCfg.mqtt_topic);
    snprintf_P(svalue, sizeof(svalue), PSTR("{\"LWT\":\"Offline\"}"));
  } else {
    snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/LWT"), PUB_PREFIX2, sysCfg.mqtt_topic);
    snprintf_P(svalue, sizeof(svalue), PSTR("Offline"));
  }
  if (mqttClient.connect(MQTTClient, sysCfg.mqtt_user, sysCfg.mqtt_pwd, stopic, 0, 0, svalue)) {
    addLog_P(LOG_LEVEL_INFO, PSTR("MQTT: Connected"));
    mqttcounter = 0;
    udpConnected = false;
    mqtt_connected();
  } else {
    snprintf_P(log, sizeof(log), PSTR("MQTT: CONNECT FAILED, rc %d. Retry in %d seconds"), mqttClient.state(), mqttcounter);
    addLog(LOG_LEVEL_DEBUG, log);
  }
}

void mqttDataCb(char* topic, byte* data, unsigned int data_len)
{
  uint16_t i = 0, grpflg = 0, index;
  char topicBuf[TOPSZ], dataBuf[data_len+1], dataBufUc[MESSZ];
  char *str, *p, *mtopic = NULL, *type = NULL, *devc = NULL;
  char stopic[TOPSZ], svalue[MESSZ], stemp1[TOPSZ], stemp2[10], stemp3[10];
  float ped, pi, pc;
  uint16_t pe, pw, pu;

  strncpy(topicBuf, topic, sizeof(topicBuf));
  memcpy(dataBuf, data, sizeof(dataBuf));
  dataBuf[sizeof(dataBuf)-1] = 0;

  snprintf_P(svalue, sizeof(svalue), PSTR("MQTT: Receive topic %s, data size %d, data %s"), topicBuf, data_len, dataBuf);
  addLog(LOG_LEVEL_DEBUG_MORE, svalue);

#ifdef USE_DOMOTICZ
  domoticz_update_flag = 1;
  if (!strncmp(topicBuf, sysCfg.domoticz_out_topic, strlen(sysCfg.domoticz_out_topic)) != 0) {
    unsigned long idx = 0;
    int16_t nvalue;

    if (strlen(dataBuf) < 20) return;
    idx = getKeyIntValue(dataBuf,"\"idx\"");
    nvalue = getKeyIntValue(dataBuf,"\"nvalue\"");
    dataBuf[0] = '\0';
    if (nvalue == 0 || nvalue == 1) {
      for (i = 0; i < Maxdevice; i++) {
        if (idx > 0 && idx == sysCfg.domoticz_relay_idx[i]) {
          snprintf_P(dataBuf, sizeof(dataBuf), PSTR("%d"), nvalue);
          break;
        }
      }
    }
    if (!strlen(dataBuf)) return;
    if (((power >> i) &1) == nvalue) return;
    snprintf_P(stemp1, sizeof(stemp1), PSTR("%d"), i +1);
    snprintf_P(topicBuf, sizeof(topicBuf), PSTR("%s/%s/%s%s"),
      SUB_PREFIX, sysCfg.mqtt_topic, sysCfg.mqtt_subtopic, (Maxdevice > 1) ? stemp1 : "");
    domoticz_update_flag = 0;
  }
#endif //USE_DOMOTICZ

  memmove(topicBuf, topicBuf+sizeof(SUB_PREFIX), sizeof(topicBuf)-sizeof(SUB_PREFIX));  // Remove SUB_PREFIX
/*
  // Use following code after release 3.0.0
  i = 0;
  for (str = strtok_r(topicBuf, "/", &p); str && i < 2; str = strtok_r(NULL, "/", &p)) {
    switch (i++) {
    case 0:  // Topic / GroupTopic / DVES_123456
      mtopic = str;
      break;
    case 1:  // TopicIndex / Text
      type = str;
    }
  }
  if (!strcmp(mtopic, sysCfg.mqtt_grptopic)) grpflg = 1;
  index = 1;
*/
  i = 0;
  for (str = strtok_r(topicBuf, "/", &p); str && i < 3; str = strtok_r(NULL, "/", &p)) {
    switch (i++) {
    case 0:  // Topic / GroupTopic / DVES_123456
      mtopic = str;
      break;
    case 1:  // TopicIndex / Text
      type = str;
      break;
    case 2:  // Text
      devc = str;
    }
  }
  if (!strcmp(mtopic, sysCfg.mqtt_grptopic)) grpflg = 1;

  index = 1;
  if (devc != NULL) {
    index = atoi(type);
    if ((index < 1) || (index > Maxdevice)) index = 0;
    type = devc;
  }
  if (!index) type = NULL;

  if (type != NULL) {
    for(i = 0; i < strlen(type); i++) type[i] = toupper(type[i]);
    i--;
    if ((type[i] > '0') && (type[i] < '9')) {
      index = (int)type[i] - '0';
      type[i] = '\0';
    }
  }

  for(i = 0; i <= sizeof(dataBufUc); i++) dataBufUc[i] = toupper(dataBuf[i]);

  snprintf_P(svalue, sizeof(svalue), PSTR("MQTT: DataCb Topic %s, Group %d, Index %d, Type %s, Data %s (%s)"),
    mtopic, grpflg, index, type, dataBuf, dataBufUc);
  addLog(LOG_LEVEL_DEBUG, svalue);

  if (type != NULL) {
    snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/RESULT"), PUB_PREFIX, sysCfg.mqtt_topic);
    snprintf_P(svalue, sizeof(svalue), PSTR("{\"Command\":\"Error\"}"));

    if (!strcmp(dataBufUc,"?")) data_len = 0;
    int16_t payload = atoi(dataBuf);
    if (!strcmp(dataBufUc,"OFF") || !strcmp(dataBufUc,"STOP")) payload = 0;
    if (!strcmp(dataBufUc,"ON") || !strcmp(dataBufUc,"START") || !strcmp(dataBufUc,"USER")) payload = 1;
    if (!strcmp(dataBufUc,"TOGGLE") || !strcmp(dataBufUc,"ADMIN")) payload = 2;

    if ((!strcmp(type,"POWER") || !strcmp(type,"LIGHT")) && (index <= Maxdevice)) {
      snprintf_P(sysCfg.mqtt_subtopic, sizeof(sysCfg.mqtt_subtopic), PSTR("%s"), type);
      if ((data_len == 0) || (payload > 2)) payload = 3;
      do_cmnd_power(index, payload);
      return;
    }
    else if (!strcmp(type,"STATUS")) {
      if ((data_len == 0) || (payload < 0) || (payload > MAX_STATUS)) payload = 99;
      if ((payload == 0) || (payload == 99)) {
        if (sysCfg.message_format == JSON) {
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"Status\":{Version\":\"%s\", \"Model\":%d, \"Topic\":\"%s\", \"ButtonTopic\":\"%s\", \"Subtopic\":\"%s\", \"Power\":%d, \"Timezone\":%d, \"Ledstate\":%d, \"SaveData\":%d, \"SaveState\":%d, \"ButtonRetain\":%d, \"PowerRetain\":%d}}"),
            Version, sysCfg.model, sysCfg.mqtt_topic, sysCfg.mqtt_topic2, sysCfg.mqtt_subtopic, power, sysCfg.timezone, sysCfg.ledstate, sysCfg.savedata, sysCfg.savestate, sysCfg.mqtt_button_retain, sysCfg.mqtt_power_retain);
        } else {
          snprintf_P(svalue, sizeof(svalue), PSTR("%s, %d, %s, %s, %s, %d, %d, %d, %d, %d, %d, %d"),
            Version, sysCfg.model, sysCfg.mqtt_topic, sysCfg.mqtt_topic2, sysCfg.mqtt_subtopic, power, sysCfg.timezone, sysCfg.ledstate, sysCfg.savedata, sysCfg.savestate, sysCfg.mqtt_button_retain, sysCfg.mqtt_power_retain);
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
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"StatusPWR\":{\"Voltage\":%d, \"Current\":\"%s\", \"Power\":%d, \"Today\":\"%s\", \"Factor\":\"%s\"}}"),
            pu, stemp1, pw, stemp2, stemp3);
        } else {
          snprintf_P(svalue, sizeof(svalue), PSTR("PWR: Voltage %d V, Current %s A, Power %d W, Today %s kWh, Factor %s"),
            pu, stemp1, pw, stemp2, stemp3);
        }
        if (payload == 0) mqtt_publish(stopic, svalue);
      }
      if ((payload == 0) || (payload == 9)) {
        if (sysCfg.message_format == JSON) {
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"StatusPWRThreshold\":{\"PowerLow\":%d, \"PowerHigh\":%d, \"VoltageLow\":%d, \"VoltageHigh\":%d, \"CurrentLow\":%d, \"CurrentHigh\":%d}}"),
            sysCfg.hlw_pmin, sysCfg.hlw_pmax, sysCfg.hlw_umin, sysCfg.hlw_umax, sysCfg.hlw_imin, sysCfg.hlw_imax);
        } else {
          snprintf_P(svalue, sizeof(svalue), PSTR("PWR Threshold: PowerLow %d W, PowerHigh %d W, VoltageLow %d V, VoltageHigh %d V, CurrentLow %d mA, CurrentHigh %d mA"),
            sysCfg.hlw_pmin, sysCfg.hlw_pmax, sysCfg.hlw_umin, sysCfg.hlw_umax, sysCfg.hlw_imin, sysCfg.hlw_imax);
        }
        if (payload == 0) mqtt_publish(stopic, svalue);
      }
#endif  // USE_POWERMONITOR
      if ((payload == 0) || (payload == 1)) {
        if (sysCfg.message_format == JSON) {
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"StatusPRM\":{\"Baudrate\":%d, \"GroupTopic\":\"%s\", \"OtaUrl\":\"%s\", \"Uptime\":%d, \"BootCount\":%d, \"SaveCount\":%d}}"),
            Baudrate, sysCfg.mqtt_grptopic, sysCfg.otaUrl, uptime, sysCfg.bootcount, sysCfg.saveFlag);
        } else {
          snprintf_P(svalue, sizeof(svalue), PSTR("PRM: Baudrate %d, GroupTopic %s, OtaUrl %s, Uptime %d Hr, BootCount %d, SaveCount %d"),
            Baudrate, sysCfg.mqtt_grptopic, sysCfg.otaUrl, uptime, sysCfg.bootcount, sysCfg.saveFlag);
        }
        if (payload == 0) mqtt_publish(stopic, svalue);
      }
      if ((payload == 0) || (payload == 2)) {
        if (sysCfg.message_format == JSON) {
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"StatusFWR\":{\"Program\":\"%s\", \"Boot\":%d, \"SDK\":\"%s\"}}"),
            Version, ESP.getBootVersion(), ESP.getSdkVersion());
        } else {
          snprintf_P(svalue, sizeof(svalue), PSTR("FWR: Program %s, Boot %d, SDK %s"),
            Version, ESP.getBootVersion(), ESP.getSdkVersion());
        }
        if (payload == 0) mqtt_publish(stopic, svalue);
      }
      if ((payload == 0) || (payload == 3)) {
        if (sysCfg.message_format == JSON) {
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"StatusLOG\":{\"Seriallog\":%d, \"Weblog\":%d, \"Syslog\":%d, \"LogHost\":\"%s\", \"SSId1\":\"%s\", \"Password1\":\"%s\", \"SSId2\":\"%s\", \"Password2\":\"%s\", \"TelePeriod\":%d}}"),
            sysCfg.seriallog_level, sysCfg.weblog_level, sysCfg.syslog_level, sysCfg.syslog_host, sysCfg.sta_ssid[0], sysCfg.sta_pwd[0], sysCfg.sta_ssid[1], sysCfg.sta_pwd[1], sysCfg.tele_period);
        } else {
          snprintf_P(svalue, sizeof(svalue), PSTR("LOG: Seriallog %d, Weblog %d, Syslog %d, LogHost %s, SSId1 %s, Password1 %s, SSId2 %s, Password2 %s, TelePeriod %d"),
            sysCfg.seriallog_level, sysCfg.weblog_level, sysCfg.syslog_level, sysCfg.syslog_host, sysCfg.sta_ssid[0], sysCfg.sta_pwd[0], sysCfg.sta_ssid[1], sysCfg.sta_pwd[1], sysCfg.tele_period);
        }
        if (payload == 0) mqtt_publish(stopic, svalue);
      }
      if ((payload == 0) || (payload == 4)) {
        if (sysCfg.message_format == JSON) {
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"StatusMEM\":{\"ProgramSize\":%d, \"Free\":%d, \"Heap\":%d, \"SpiffsStart\":%d, \"SpiffsSize\":%d, \"FlashSize\":%d, \"ProgramFlashSize\":%d}}"),
            ESP.getSketchSize()/1024, ESP.getFreeSketchSpace()/1024, ESP.getFreeHeap()/1024, ((uint32_t)&_SPIFFS_start - 0x40200000)/1024,
            (((uint32_t)&_SPIFFS_end - 0x40200000) - ((uint32_t)&_SPIFFS_start - 0x40200000))/1024, ESP.getFlashChipRealSize()/1024, ESP.getFlashChipSize()/1024);
        } else {
          snprintf_P(svalue, sizeof(svalue), PSTR("MEM: ProgramSize %dkB, Free %dkB (Heap %dkB), SpiffsStart %dkB (%dkB), FlashSize %dkB (%dkB)"),
            ESP.getSketchSize()/1024, ESP.getFreeSketchSpace()/1024, ESP.getFreeHeap()/1024, ((uint32_t)&_SPIFFS_start - 0x40200000)/1024,
            (((uint32_t)&_SPIFFS_end - 0x40200000) - ((uint32_t)&_SPIFFS_start - 0x40200000))/1024, ESP.getFlashChipRealSize()/1024, ESP.getFlashChipSize()/1024);
        }
        if (payload == 0) mqtt_publish(stopic, svalue);
      }
      if ((payload == 0) || (payload == 5)) {
        if (sysCfg.message_format == JSON) {
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"StatusNET\":{\"Host\":\"%s\", \"IP\":\"%s\", \"Gateway\":\"%s\", \"Subnetmask\":\"%s\", \"Mac\":\"%s\", \"Webserver\":%d, \"WifiConfig\":%d}}"),
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
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"StatusMQT\":{\"Host\":\"%s\", \"Port\":%d, \"ClientMask\":\"%s\", \"Client\":\"%s\", \"User\":\"%s\", \"Password\":\"%s\", \"MAX_PACKET_SIZE\":%d, \"KEEPALIVE\":%d}}"),
            sysCfg.mqtt_host, sysCfg.mqtt_port, sysCfg.mqtt_client, MQTTClient, sysCfg.mqtt_user, sysCfg.mqtt_pwd, MQTT_MAX_PACKET_SIZE, MQTT_KEEPALIVE);
        } else {
          snprintf_P(svalue, sizeof(svalue), PSTR("MQT: Host %s, Port %d, Client %s (%s), User %s, Password %s, MAX_PACKET_SIZE %d, KEEPALIVE %d"),
            sysCfg.mqtt_host, sysCfg.mqtt_port, sysCfg.mqtt_client, MQTTClient, sysCfg.mqtt_user, sysCfg.mqtt_pwd, MQTT_MAX_PACKET_SIZE, MQTT_KEEPALIVE);
        }
        if (payload == 0) mqtt_publish(stopic, svalue);
      }
      if ((payload == 0) || (payload == 7)) {
        if (sysCfg.message_format == JSON) {
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"StatusTIM\":{\"UTC\":\"%s\", \"Local\":\"%s\", \"StartDST\":\"%s\", \"EndDST\":\"%s\"}}"),
            rtc_time(0).c_str(), rtc_time(1).c_str(), rtc_time(2).c_str(), rtc_time(3).c_str());
        } else {
          snprintf_P(svalue, sizeof(svalue), PSTR("TIM: UTC %s, Local %s, StartDST %s, EndDST %s"),
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
      if (sysCfg.savedata > 1) snprintf_P(stemp1, sizeof(stemp1), PSTR("Every %d seconds"), sysCfg.savedata);
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"SaveData\":\"%s\"}"), (sysCfg.savedata) ? (sysCfg.savedata > 1) ? stemp1 : MQTT_STATUS_ON : MQTT_STATUS_OFF);
    }
    else if (!strcmp(type,"SAVESTATE")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 1)) {
        sysCfg.savestate = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"SaveState\":\"%s\"}"), (sysCfg.savestate) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
    }
    else if (!strcmp(type,"MESSAGEFORMAT")) {
      if ((data_len > 0) && (payload >= 0) && (payload < MAX_FORMAT)) {
        sysCfg.message_format = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"MessageFormat\":\"%s\"}"), (sysCfg.message_format) ? "JSON" : "Legacy");
    }
    else if (!strcmp(type,"MODEL")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= CHANNEL_4)) {
        sysCfg.model = payload;
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Model\":%d}"), sysCfg.model);
    }
    else if (!strcmp(type,"UPGRADE") || !strcmp(type,"UPLOAD")) {
      if ((data_len > 0) && (payload == 1)) {
        otaflag = 3;
        snprintf_P(svalue, sizeof(svalue), PSTR("{\"Upgrade\":\"Version %s from %s\"}"), Version, sysCfg.otaUrl);
      } else {
        snprintf_P(svalue, sizeof(svalue), PSTR("{\"Upgrade\":\"Option 1 to upgrade\"}"));
      }
    }
    else if (!strcmp(type,"OTAURL")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.otaUrl)))
        strlcpy(sysCfg.otaUrl, (payload == 1) ? OTA_URL : dataBuf, sizeof(sysCfg.otaUrl));
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"OtaUrl\":\"%s\"}"), sysCfg.otaUrl);
    }
    else if (!strcmp(type,"SERIALLOG")) {
      if ((data_len > 0) && (payload >= LOG_LEVEL_NONE) && (payload <= LOG_LEVEL_ALL)) {
        sysCfg.seriallog_level = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"SerialLog\":%d}"), sysCfg.seriallog_level);
    }
    else if (!strcmp(type,"SYSLOG")) {
      if ((data_len > 0) && (payload >= LOG_LEVEL_NONE) && (payload <= LOG_LEVEL_ALL)) {
        sysCfg.syslog_level = payload;
        syslog_level = payload;
        syslog_timer = 0;
        snprintf_P(svalue, sizeof(svalue), PSTR("{\"SysLog\":%d}"), sysCfg.syslog_level);
      } else {
       snprintf_P(svalue, sizeof(svalue), PSTR("{\"SysLog\":\"%d (Setting %d)\"}"), syslog_level, sysCfg.syslog_level);
      }
    }
    else if (!strcmp(type,"LOGHOST")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.syslog_host))) {
        strlcpy(sysCfg.syslog_host, (payload == 1) ? SYS_LOG_HOST : dataBuf, sizeof(sysCfg.syslog_host));
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"LogHost\":\"%s\"}"), sysCfg.syslog_host);
    }
    else if (!strcmp(type,"LOGPORT")) {
      if ((data_len > 0) && (payload > 0) && (payload < 32766)) {
        sysCfg.syslog_port = (payload == 1) ? SYS_LOG_PORT : payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"LogPort\":%d}"), sysCfg.syslog_port);
    }
    else if (!strcmp(type,"AP")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 2)) {
        switch (payload) {
        case 0:  // Toggle
          sysCfg.sta_active ^= 1;
          break;
        case 1:  // AP1
        case 2:  // AP2
          sysCfg.sta_active = payload -1;
        }
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Ap\":\"%d (%s)\"}"), sysCfg.sta_active +1, sysCfg.sta_ssid[sysCfg.sta_active]);
    }
    else if (!strcmp(type,"SSID") && (index <= 2)) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.sta_ssid[0]))) {
        strlcpy(sysCfg.sta_ssid[index -1], (payload == 1) ? (index == 1) ? STA_SSID1 : STA_SSID2 : dataBuf, sizeof(sysCfg.sta_ssid[0]));
        sysCfg.sta_active = 0;
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"SSid%d\":\"%s\"}"), index, sysCfg.sta_ssid[index -1]);
    }
    else if (!strcmp(type,"PASSWORD") && (index <= 2)) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.sta_pwd[0]))) {
        strlcpy(sysCfg.sta_pwd[index -1], (payload == 1) ? (index == 1) ? STA_PASS1 : STA_PASS2 : dataBuf, sizeof(sysCfg.sta_pwd[0]));
        sysCfg.sta_active = 0;
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Password%d\":\"%s\"}"), index, sysCfg.sta_pwd[index -1]);
    }
    else if (!grpflg && !strcmp(type,"HOSTNAME")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.hostname))) {
        strlcpy(sysCfg.hostname, (payload == 1) ? WIFI_HOSTNAME : dataBuf, sizeof(sysCfg.hostname));
        if (strstr(sysCfg.hostname,"%"))
          strlcpy(sysCfg.hostname, DEF_WIFI_HOSTNAME, sizeof(sysCfg.hostname));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Hostname\":\"%s\"}"), sysCfg.hostname);
    }
    else if (!strcmp(type,"WIFICONFIG") || !strcmp(type,"SMARTCONFIG")) {
      if ((data_len > 0) && (payload >= WIFI_RESTART) && (payload <= WIFI_WPSCONFIG)) {
        sysCfg.sta_config = payload;
        wificheckflag = sysCfg.sta_config;
        snprintf_P(stemp1, sizeof(stemp1), wificfg[sysCfg.sta_config]);
        snprintf_P(svalue, sizeof(svalue), PSTR("{\"WifiConfig\":\"%s selected\"}"), stemp1);
        if (WIFI_State() != WIFI_RESTART) {
//          snprintf_P(svalue, sizeof(svalue), PSTR("%s after restart"), svalue);
          restartflag = 2;
        }
      } else {
        snprintf_P(stemp1, sizeof(stemp1), wificfg[sysCfg.sta_config]);
        snprintf_P(svalue, sizeof(svalue), PSTR("{\"WifiConfig\":\"%d (%s)\"}"), sysCfg.sta_config, stemp1);
      }
    }
#ifdef USE_WALL_SWITCH
    else if (!strcmp(type,"SWITCHMODE")) {
      if ((data_len > 0) && (payload >= 0) && (payload < MAX_SWITCH_OPTION)) {
        sysCfg.switchmode = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"SwitchMode\":%d}"), sysCfg.switchmode);
    }
#endif  // USE_WALL_SWITCH
#ifdef USE_WEBSERVER
    else if (!strcmp(type,"WEBSERVER")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 2)) {
        sysCfg.webserver = payload;
      }
      if (sysCfg.webserver) {
        snprintf_P(svalue, sizeof(svalue), PSTR("{\"Webserver\":\"Active for %s on %s with IP address %s\"}"),
          (sysCfg.webserver == 2) ? "ADMIN" : "USER", Hostname, WiFi.localIP().toString().c_str());
      } else {
        snprintf_P(svalue, sizeof(svalue), PSTR("{\"Webserver\":\"%s\"}"), MQTT_STATUS_OFF);
      }
    }
    else if (!strcmp(type,"WEBLOG")) {
      if ((data_len > 0) && (payload >= LOG_LEVEL_NONE) && (payload <= LOG_LEVEL_ALL)) {
        sysCfg.weblog_level = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"WebLog\":%d}"), sysCfg.weblog_level);
    }
#endif  // USE_WEBSERVER
    else if (!strcmp(type,"MQTTHOST")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.mqtt_host))) {
        strlcpy(sysCfg.mqtt_host, (payload == 1) ? MQTT_HOST : dataBuf, sizeof(sysCfg.mqtt_host));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"MqttHost\",\"%s\"}"), sysCfg.mqtt_host);
    }
    else if (!strcmp(type,"MQTTPORT")) {
      if ((data_len > 0) && (payload > 0) && (payload < 32766)) {
        sysCfg.mqtt_port = (payload == 1) ? MQTT_PORT : payload;
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"MqttPort\":%d}"), sysCfg.mqtt_port);
    }
#ifdef USE_MQTT_TLS
    else if (!strcmp(type,"MQTTFINGERPRINT")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.mqtt_fingerprint))) {
        strlcpy(sysCfg.mqtt_fingerprint, (payload == 1) ? MQTT_FINGERPRINT : dataBuf, sizeof(sysCfg.mqtt_fingerprint));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"MqttFingerprint\":\"%s\"}"), sysCfg.mqtt_fingerprint);
    }
#endif
    else if (!grpflg && !strcmp(type,"MQTTCLIENT")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.mqtt_client))) {
        strlcpy(sysCfg.mqtt_client, (payload == 1) ? MQTT_CLIENT_ID : dataBuf, sizeof(sysCfg.mqtt_client));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"MqttClient\":\"%s\"}"), sysCfg.mqtt_client);
    }
    else if (!strcmp(type,"MQTTUSER")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.mqtt_user))) {
        strlcpy(sysCfg.mqtt_user, (payload == 1) ? MQTT_USER : dataBuf, sizeof(sysCfg.mqtt_user));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("[\"MqttUser\":\"%s\"}"), sysCfg.mqtt_user);
    }
    else if (!strcmp(type,"MQTTPASSWORD")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.mqtt_pwd))) {
        strlcpy(sysCfg.mqtt_pwd, (payload == 1) ? MQTT_PASS : dataBuf, sizeof(sysCfg.mqtt_pwd));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"MqttPassword\":\"%s\"}"), sysCfg.mqtt_pwd);
    }
    else if (!strcmp(type,"MQTTUNITS")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 1)) {
        sysCfg.mqtt_units = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"MqttUnits\":\"%s\"}"), (sysCfg.mqtt_units) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
    }
#ifdef USE_DOMOTICZ
    else if (!strcmp(type,"DOMOTICZINTOPIC")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.domoticz_in_topic))) {
        strlcpy(sysCfg.domoticz_in_topic, (payload == 1) ? DOMOTICZ_IN_TOPIC : dataBuf, sizeof(sysCfg.domoticz_in_topic));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"DomoticzInTopic\":\"%s\"}"), sysCfg.domoticz_in_topic);
    }
    else if (!strcmp(type,"DOMOTICZOUTTOPIC")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.domoticz_out_topic))) {
        strlcpy(sysCfg.domoticz_out_topic, (payload == 1) ? DOMOTICZ_OUT_TOPIC : dataBuf, sizeof(sysCfg.domoticz_out_topic));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"DomoticzOutTopic\":\"%s\"}"), sysCfg.domoticz_out_topic);
    }
    else if (!strcmp(type,"DOMOTICZIDX") && (index <= Maxdevice)) {
      if ((data_len > 0) && (payload >= 0)) {
        sysCfg.domoticz_relay_idx[index -1] = payload;
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"DomoticzIdx\":%d}"), sysCfg.domoticz_relay_idx[index -1]);
    }
    else if (!strcmp(type,"DOMOTICZKEYIDX") && (index <= Maxdevice)) {
      if ((data_len > 0) && (payload >= 0)) {
        sysCfg.domoticz_key_idx[index -1] = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"DomoticzKeyIdx\":%d}"), sysCfg.domoticz_key_idx[index -1]);
    }
    else if (!strcmp(type,"DOMOTICZUPDATETIMER")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.domoticz_update_timer = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"DomoticzUpdateTimer\":%d}"), sysCfg.domoticz_update_timer);
    }
#endif  // USE_DOMOTICZ
    else if (!strcmp(type,"TELEPERIOD")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.tele_period = (payload == 1) ? TELE_PERIOD : payload;
        if ((sysCfg.tele_period > 0) && (sysCfg.tele_period < 10)) sysCfg.tele_period = 10;   // Do not allow periods < 10 seconds
        tele_period = sysCfg.tele_period;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"TelePeriod\":\"%d%s\"}"), sysCfg.tele_period, (sysCfg.mqtt_units) ? " Sec" : "");
    }
    else if (!strcmp(type,"GROUPTOPIC")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.mqtt_grptopic))) {
        for(i = 0; i <= data_len; i++)
          if ((dataBuf[i] == '/') || (dataBuf[i] == '+') || (dataBuf[i] == '#')) dataBuf[i] = '_';
        if (!strcmp(dataBuf, MQTTClient)) payload = 1;
        strlcpy(sysCfg.mqtt_grptopic, (payload == 1) ? MQTT_GRPTOPIC : dataBuf, sizeof(sysCfg.mqtt_grptopic));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"GroupTopic\":\"%s\"}"), sysCfg.mqtt_grptopic);
    }
    else if (!grpflg && !strcmp(type,"TOPIC")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.mqtt_topic))) {
        for(i = 0; i <= data_len; i++)
          if ((dataBuf[i] == '/') || (dataBuf[i] == '+') || (dataBuf[i] == '#')) dataBuf[i] = '_';
        if (!strcmp(dataBuf, MQTTClient)) payload = 1;
        strlcpy(sysCfg.mqtt_topic, (payload == 1) ? MQTT_TOPIC : dataBuf, sizeof(sysCfg.mqtt_topic));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Topic\":\"%s\"}"), sysCfg.mqtt_topic);
    }
    else if (!grpflg && !strcmp(type,"BUTTONTOPIC")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.mqtt_topic2))) {
        for(i = 0; i <= data_len; i++)
          if ((dataBuf[i] == '/') || (dataBuf[i] == '+') || (dataBuf[i] == '#')) dataBuf[i] = '_';
        if (!strcmp(dataBuf, MQTTClient)) payload = 1;
        strlcpy(sysCfg.mqtt_topic2, (payload == 1) ? sysCfg.mqtt_topic : dataBuf, sizeof(sysCfg.mqtt_topic2));
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"ButtonTopic\":\"%s\"}"), sysCfg.mqtt_topic2);
    }
    else if (!strcmp(type,"BUTTONRETAIN")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 1)) {
        strlcpy(sysCfg.mqtt_topic2, sysCfg.mqtt_topic, sizeof(sysCfg.mqtt_topic2));
        if (!payload) {
          for(i = 1; i <= Maxdevice; i++) {
            send_button_power(i, 3);  // Clear MQTT retain in broker
          }
        }
        sysCfg.mqtt_button_retain = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"ButtonRetain\":\"%s\"}"), (sysCfg.mqtt_button_retain) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
    }
    else if (!strcmp(type,"POWERRETAIN") || !strcmp(type,"LIGHTRETAIN")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 1)) {
        if (!payload) {
          for(i = 1; i <= Maxdevice; i++) {  // Clear MQTT retain in broker
            snprintf_P(stemp2, sizeof(stemp2), PSTR("%d"), i);
            snprintf_P(stemp1, sizeof(stemp1), PSTR("%s/%s/POWER%s"), PUB_PREFIX, sysCfg.mqtt_topic, (Maxdevice > 1) ? stemp2 : "");
            mqtt_publish(stemp1, "", sysCfg.mqtt_power_retain);
            snprintf_P(stemp1, sizeof(stemp1), PSTR("%s/%s/LIGHT%s"), PUB_PREFIX, sysCfg.mqtt_topic, (Maxdevice > 1) ? stemp2 : "");
            mqtt_publish(stemp1, "", sysCfg.mqtt_power_retain);
          }
        }
        sysCfg.mqtt_power_retain = payload;
      }
      snprintf_P(stemp1, sizeof(stemp1), PSTR("%s"), (!strcmp(sysCfg.mqtt_subtopic,"POWER")) ? "Power" : "Light");
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"%sRetain\":\"%s\"}"), stemp1, (sysCfg.mqtt_power_retain) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
    }
    else if (!strcmp(type,"RESTART")) {
      switch (payload) {
      case 1:
        restartflag = 2;
        snprintf_P(svalue, sizeof(svalue), PSTR("{\"Restart\":\"Restarting\"}"));
        break;
      case 99:
        addLog_P(LOG_LEVEL_INFO, PSTR("APP: Restarting"));
        ESP.restart();
        break;
      default:
        snprintf_P(svalue, sizeof(svalue), PSTR("{\"Restart\":\"1 to restart\"}"));
      }
    }
    else if (!strcmp(type,"RESET")) {
      switch (payload) {
      case 1:
        restartflag = 211;
        snprintf_P(svalue, sizeof(svalue), PSTR("{\"Reset\":\"Reset and Restarting\"}"));
        break;
      case 2:
        restartflag = 212;
        snprintf_P(svalue, sizeof(svalue), PSTR("{\"Reset\":\"Erase, Reset and Restarting\"}"));
        break;
      default:
        snprintf_P(svalue, sizeof(svalue), PSTR("{\"Reset\":\"1 to reset\"}"));
      }
    }
    else if (!strcmp(type,"TIMEZONE")) {
      if ((data_len > 0) && (((payload >= -12) && (payload <= 12)) || (payload == 99))) {
        sysCfg.timezone = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Timezone\":\"%d%s\"}"), sysCfg.timezone, (sysCfg.mqtt_units) ? " Hr" : "");
    }
    else if (!strcmp(type,"LEDSTATE")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 1)) {
        sysCfg.ledstate = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"LedState\":\"%s\"}"), (sysCfg.ledstate) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
    }
    else if (!strcmp(type,"CFGDUMP")) {
      CFG_Dump();
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"CfgDump\":\"Done\"}"));
    }
#ifdef SEND_TELEMETRY_I2C
    else if (!strcmp(type,"I2CSCAN")) {
      i2c_scan(svalue, sizeof(svalue));
    }
#endif //SEND_TELEMETRY_I2C
#ifdef USE_POWERMONITOR
    else if (!strcmp(type,"POWERLOW")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.hlw_pmin = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"PowerLow\":\"%d%s\"}"), sysCfg.hlw_pmin, (sysCfg.mqtt_units) ? " W" : "");
    }
    else if (!strcmp(type,"POWERHIGH")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.hlw_pmax = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"PowerHigh\":\"%d%s\"}"), sysCfg.hlw_pmax, (sysCfg.mqtt_units) ? " W" : "");
    }
    else if (!strcmp(type,"VOLTAGELOW")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 501)) {
        sysCfg.hlw_umin = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"VoltageLow\":\"%d%s\"}"), sysCfg.hlw_umin, (sysCfg.mqtt_units) ? " V" : "");
    }
    else if (!strcmp(type,"VOLTAGEHIGH")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 501)) {
        sysCfg.hlw_umax = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("[\"VoltageHigh\":\"%d%s\"}"), sysCfg.hlw_umax, (sysCfg.mqtt_units) ? " V" : "");
    }
    else if (!strcmp(type,"CURRENTLOW")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 16001)) {
        sysCfg.hlw_imin = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"CurrentLow\":\"%d%s\"}"), sysCfg.hlw_imin, (sysCfg.mqtt_units) ? " mA" : "");
    }
    else if (!strcmp(type,"CURRENTHIGH")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 16001)) {
        sysCfg.hlw_imax = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"CurrentHigh\":\"%d%s\"}"), sysCfg.hlw_imax, (sysCfg.mqtt_units) ? " mA" : "");
    }
#ifdef USE_POWERCALIBRATION
    else if (!strcmp(type,"HLWPCAL")) {
      if ((data_len > 0) && (payload > 0) && (payload < 32001)) {
        sysCfg.hlw_pcal = (payload == 1) ? HLW_PREF_PULSE : payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("(\"HlwPcal\":\"%d%s\"}"), sysCfg.hlw_pcal, (sysCfg.mqtt_units) ? " uS" : "");
    }
    else if (!strcmp(type,"HLWUCAL")) {
      if ((data_len > 0) && (payload > 0) && (payload < 32001)) {
        sysCfg.hlw_ucal = (payload == 1) ? HLW_UREF_PULSE : payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"HlwUcal\":\"%d%s\"}"), sysCfg.hlw_ucal, (sysCfg.mqtt_units) ? " uS" : "");
    }
    else if (!strcmp(type,"HLWICAL")) {
      if ((data_len > 0) && (payload > 0) && (payload < 32001)) {
        sysCfg.hlw_ical = (payload == 1) ? HLW_IREF_PULSE : payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"HlwIcal\":\"%d%s\"}"), sysCfg.hlw_ical, (sysCfg.mqtt_units) ? " uS" : "");
    }
#endif  // USE_POWERCALIBRATION
#ifdef FEATURE_POWER_LIMIT
    else if (!strcmp(type,"MAXPOWER")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.hlw_mpl = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"MaxPower\":\"%d%s\"}"), sysCfg.hlw_mpl, (sysCfg.mqtt_units) ? " W" : "");
    }
    else if (!strcmp(type,"MAXPOWERHOLD")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.hlw_mplh = (payload == 1) ? MAX_POWER_HOLD : payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"MaxPowerHold\":\"%d%s\"}"), sysCfg.hlw_mplh, (sysCfg.mqtt_units) ? " Sec" : "");
    }
    else if (!strcmp(type,"MAXPOWERWINDOW")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.hlw_mplw = (payload == 1) ? MAX_POWER_WINDOW : payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"MaxPowerWindow\":\"%d%s\"}"), sysCfg.hlw_mplw, (sysCfg.mqtt_units) ? " Sec" : "");
    }
    else if (!strcmp(type,"SAFEPOWER")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.hlw_mspl = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"SafePower\":\"%d%s\"}"), sysCfg.hlw_mspl, (sysCfg.mqtt_units) ? " W" : "");
    }
    else if (!strcmp(type,"SAFEPOWERHOLD")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.hlw_msplh = (payload == 1) ? SAFE_POWER_HOLD : payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"SafePowerHold\":\"%d%s\"}"), sysCfg.hlw_msplh, (sysCfg.mqtt_units) ? " Sec" : "");
    }
    else if (!strcmp(type,"SAFEPOWERWINDOW")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 1440)) {
        sysCfg.hlw_msplw = (payload == 1) ? SAFE_POWER_WINDOW : payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"SafePowerWindow\":\"%d%s\"}"), sysCfg.hlw_msplw, (sysCfg.mqtt_units) ? " Min" : "");
    }
    else if (!strcmp(type,"MAXENERGY")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.hlw_mkwh = payload;
        hlw_mkwh_state = 3;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"MaxEnergy\":\"%d%s\"}"), sysCfg.hlw_mkwh, (sysCfg.mqtt_units) ? " Wh" : "");
    }
    else if (!strcmp(type,"MAXENERGYSTART")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 24)) {
        sysCfg.hlw_mkwhs = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"MaxEnergyStart\":\"%d%s\"}"), sysCfg.hlw_mkwhs, (sysCfg.mqtt_units) ? " Hr" : "");
    }
#endif  // FEATURE_POWER_LIMIT
#endif  // USE_POWERMONITOR
    else {
      type = NULL;
    }
  }
  if (type == NULL) {
    blinks = 1;
    snprintf_P(svalue, sizeof(svalue), PSTR("{\"Commands1\":\"Status, SaveData, SaveSate, Upgrade, Otaurl, Restart, Reset, WifiConfig, Seriallog, Syslog, LogHost, LogPort, SSId1, SSId2, Password1, Password2, AP%s\"}"), (!grpflg) ? ", Hostname" : "");
    if (sysCfg.message_format != JSON) json2legacy(stopic, svalue);
    mqtt_publish(stopic, svalue);

    snprintf_P(svalue, sizeof(svalue), PSTR("{\"Commands2\":\"MqttHost, MqttPort, MqttUser, MqttPassword%s, MqttUnits, MessageFormat, GroupTopic, Timezone, Ledstate, TelePeriod\"}"), (!grpflg) ? ", MqttClient, Topic, ButtonTopic, ButtonRetain, PowerRetain" : "");
    if (sysCfg.message_format != JSON) json2legacy(stopic, svalue);
    mqtt_publish(stopic, svalue);

    snprintf_P(svalue, sizeof(svalue), PSTR("{\"Commands3\":\"%s"), (sysCfg.model == SONOFF) ? "Power, Light" : "Power1, Power2, Light1 Light2");
#ifdef USE_WEBSERVER
    snprintf_P(svalue, sizeof(svalue), PSTR("%s, Weblog, Webserver"), svalue);
#endif
#ifdef USE_DOMOTICZ
    snprintf_P(svalue, sizeof(svalue), PSTR("%s, DomoticzInTopic, DomoticzOutTopic, DomoticzIdx, DomoticzKeyIdx, DomoticzUpdateTimer"), svalue);
#endif  // USE_DOMOTICZ
#ifdef USE_WALL_SWITCH
    snprintf_P(svalue, sizeof(svalue), PSTR("%s, SwitchMode"), svalue);
#endif
#ifdef SEND_TELEMETRY_I2C
    snprintf_P(svalue, sizeof(svalue), PSTR("%s, I2CScan"), svalue);
#endif
    snprintf_P(svalue, sizeof(svalue), PSTR("%s\"}"), svalue);

#ifdef USE_POWERMONITOR
    if (sysCfg.message_format != JSON) json2legacy(stopic, svalue);
    mqtt_publish(stopic, svalue);
    snprintf_P(svalue, sizeof(svalue), PSTR("{\"Commands4\":\"PowerLow, PowerHigh, VoltageLow, VoltageHigh, CurrentLow, CurrentHigh"));
#ifdef FEATURE_POWER_LIMIT
    snprintf_P(svalue, sizeof(svalue), PSTR("%s, SafePower, SafePowerHold, SafePowerWindow, MaxPower, MaxPowerHold, MaxPowerWindow, MaxEnergy, MaxEnergyStart"), svalue);
#endif  // FEATURE_POWER_LIMIT
    snprintf_P(svalue, sizeof(svalue), PSTR("%s\"}"), svalue);
#endif  // USE_POWERMONITOR
  }
  if (sysCfg.message_format != JSON) json2legacy(stopic, svalue);
  mqtt_publish(stopic, svalue);
}

/********************************************************************************************/

void send_button_power(byte device, byte state)
{
  char stopic[TOPSZ], svalue[TOPSZ], stemp1[10];

  if (device > Maxdevice) device = 1;
  snprintf_P(stemp1, sizeof(stemp1), PSTR("%d"), device);
  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/%s%s"),
    SUB_PREFIX, sysCfg.mqtt_topic2, sysCfg.mqtt_subtopic, (Maxdevice > 1) ? stemp1 : "");
  if (state == 3) {
    svalue[0] = '\0';
  } else {
    if (state == 2) {
      state = ~(power >> (device -1)) & 0x01;
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
    mqtt_publish(stopic, svalue, sysCfg.mqtt_button_retain);
  }
#else
  mqtt_publish(stopic, svalue, sysCfg.mqtt_button_retain);
#endif  // USE_DOMOTICZ
}

void do_cmnd_power(byte device, byte state)
{
// device  = Relay number 1 and up
// state 0 = Relay Off
// state 1 = Relay on
// state 2 = Toggle relay
// state 3 = Show power state

  if ((device < 1) || (device > Maxdevice)) device = 1;
  byte mask = 0x01 << (device -1);
  if (state <= 2) {
    switch (state) {
    case 0: { // Off
      power &= (0xFF ^ mask);
      break; }
    case 1: // On
      power |= mask;
      break;
    case 2: // Toggle
      power ^= mask;
    }
    setRelay(power);
#ifdef USE_DOMOTICZ
    if (domoticz_update_flag) mqtt_publishDomoticzPowerState(device);
    domoticz_update_flag = 1;
#endif  // USE_DOMOTICZ
  }
  mqtt_publishPowerState(device);
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
  byte flag, jsonflg;

  if (power_steady_cntr) {
    power_steady_cntr--;
    return;
  }

  hlw_readEnergy(0, ped, pe, pw, pu, pi, pc);
  if (power && (sysCfg.hlw_pmin || sysCfg.hlw_pmax || sysCfg.hlw_umin || sysCfg.hlw_umax || sysCfg.hlw_imin || sysCfg.hlw_imax)) {
    piv = (uint16_t)(pi * 1000);

//    snprintf_P(log, sizeof(log), PSTR("HLW: W %d, U %d, I %d"), pw, pu, piv);
//    addLog(LOG_LEVEL_DEBUG, log);

    if (sysCfg.message_format == JSON) {
      snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/TELEMETRY"), PUB_PREFIX2, sysCfg.mqtt_topic);
      snprintf_P(svalue, sizeof(svalue), PSTR("{"));
      jsonflg = 0;
    }
    if (hlw_margin(0, sysCfg.hlw_pmin, pw, flag, hlw_pminflg)) {
      if (sysCfg.message_format == JSON) {
        snprintf_P(svalue, sizeof(svalue), PSTR("%s%s\"PowerLow\":\"%s\""), svalue, (jsonflg)?", ":"", (flag) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
        jsonflg = 1;
      } else {
        snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/POWER_LOW"), PUB_PREFIX2, sysCfg.mqtt_topic);
        strlcpy(svalue, (flag) ? MQTT_STATUS_ON : MQTT_STATUS_OFF, sizeof(svalue));
        mqtt_publish(stopic, svalue);
      }
    }
    if (hlw_margin(1, sysCfg.hlw_pmax, pw, flag, hlw_pmaxflg)) {
      if (sysCfg.message_format == JSON) {
        snprintf_P(svalue, sizeof(svalue), PSTR("%s%s\"PowerHigh\":\"%s\""), svalue, (jsonflg)?", ":"", (flag) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
        jsonflg = 1;
      } else {
        snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/POWER_HIGH"), PUB_PREFIX2, sysCfg.mqtt_topic);
        strlcpy(svalue, (flag) ? MQTT_STATUS_ON : MQTT_STATUS_OFF, sizeof(svalue));
        mqtt_publish(stopic, svalue);
      }
    }
    if (hlw_margin(0, sysCfg.hlw_umin, pu, flag, hlw_uminflg)) {
      if (sysCfg.message_format == JSON) {
        snprintf_P(svalue, sizeof(svalue), PSTR("%s%s\"VoltageLow\":\"%s\""), svalue, (jsonflg)?", ":"", (flag) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
        jsonflg = 1;
      } else {
        snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/VOLTAGE_LOW"), PUB_PREFIX2, sysCfg.mqtt_topic);
        strlcpy(svalue, (flag) ? MQTT_STATUS_ON : MQTT_STATUS_OFF, sizeof(svalue));
        mqtt_publish(stopic, svalue);
      }
    }
    if (hlw_margin(1, sysCfg.hlw_umax, pw, flag, hlw_umaxflg)) {
      if (sysCfg.message_format == JSON) {
        snprintf_P(svalue, sizeof(svalue), PSTR("%s%s\"VoltageHigh\":\"%s\""), svalue, (jsonflg)?", ":"", (flag) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
        jsonflg = 1;
      } else {
        snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/VOLTAGE_HIGH"), PUB_PREFIX2, sysCfg.mqtt_topic);
        strlcpy(svalue, (flag) ? MQTT_STATUS_ON : MQTT_STATUS_OFF, sizeof(svalue));
        mqtt_publish(stopic, svalue);
      }
    }
    if (hlw_margin(0, sysCfg.hlw_imin, piv, flag, hlw_iminflg)) {
      if (sysCfg.message_format == JSON) {
        snprintf_P(svalue, sizeof(svalue), PSTR("%s%s\"CurrentLow\":\"%s\""), svalue, (jsonflg)?", ":"", (flag) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
        jsonflg = 1;
      } else {
        snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/CURRENT_LOW"), PUB_PREFIX2, sysCfg.mqtt_topic);
        strlcpy(svalue, (flag) ? MQTT_STATUS_ON : MQTT_STATUS_OFF, sizeof(svalue));
        mqtt_publish(stopic, svalue);
      }
    }
    if (hlw_margin(1, sysCfg.hlw_imax, piv, flag, hlw_imaxflg)) {
      if (sysCfg.message_format == JSON) {
        snprintf_P(svalue, sizeof(svalue), PSTR("%s%s\"CurrentHigh\":\"%s\""), svalue, (jsonflg)?", ":"", (flag) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
        jsonflg = 1;
      } else {
        snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/CURRENT_HIGH"), PUB_PREFIX2, sysCfg.mqtt_topic);
        strlcpy(svalue, (flag) ? MQTT_STATUS_ON : MQTT_STATUS_OFF, sizeof(svalue));
        mqtt_publish(stopic, svalue);
      }
    }
    if (jsonflg) {
      snprintf_P(svalue, sizeof(svalue), PSTR("%s}"), svalue);
      mqtt_publish(stopic, svalue);
    }
  }

#ifdef FEATURE_POWER_LIMIT
  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/RESULT"), PUB_PREFIX, sysCfg.mqtt_topic);
  // Max Power
  if (sysCfg.hlw_mpl) {
    if (pw > sysCfg.hlw_mpl) {
      if (!hlw_mplh_counter) {
        hlw_mplh_counter = sysCfg.hlw_mplh;
      } else {
        hlw_mplh_counter--;
        if (!hlw_mplh_counter) {
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"MaxPowerReached\":\"%d%s\"}"), pw, (sysCfg.mqtt_units) ? " W" : "");
          if (sysCfg.message_format != JSON) json2legacy(stopic, svalue);
          mqtt_publish(stopic, svalue);
          do_cmnd_power(1, 0);
          if (!hlw_mplr_counter) hlw_mplr_counter = MAX_POWER_RETRY +1;
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
            snprintf_P(svalue, sizeof(stopic), PSTR("{\"PowerMonitor\":\"%s\"}"), MQTT_STATUS_ON);
            if (sysCfg.message_format != JSON) json2legacy(stopic, svalue);
            mqtt_publish(stopic, svalue);
            do_cmnd_power(1, 1);
          } else {
            snprintf_P(svalue, sizeof(stopic), PSTR("{\"MaxPowerReachedRetry\":\"%s\"}"), MQTT_STATUS_OFF);
            if (sysCfg.message_format != JSON) json2legacy(stopic, svalue);
            mqtt_publish(stopic, svalue);
          }
        }
      }
    }
  }

  // Max Energy
  if (sysCfg.hlw_mkwh) {
    uped = (uint16_t)(ped * 1000);
    if (!hlw_mkwh_state && (rtcTime.Hour == sysCfg.hlw_mkwhs)) {
      hlw_mkwh_state = 1;
      snprintf_P(svalue, sizeof(stopic), PSTR("{\"EnergyMonitor\":\"%s\"}"), MQTT_STATUS_ON);
      if (sysCfg.message_format != JSON) json2legacy(stopic, svalue);
      mqtt_publish(stopic, svalue);
      do_cmnd_power(1, 1);
    }
    else if ((hlw_mkwh_state == 1) && (uped >= sysCfg.hlw_mkwh)) {
      hlw_mkwh_state = 2;
      dtostrf(ped, 1, 3, svalue);
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"MaxEnergyReached\":\"%s%s\"}"), svalue, (sysCfg.mqtt_units) ? " kWh" : "");
      if (sysCfg.message_format != JSON) json2legacy(stopic, svalue);
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
  if (rtcTime.Valid) {
    if (rtc_loctime() == rtc_midnight()) {
      hlw_mkwh_state = 3;
    }
    if ((rtcTime.Hour == sysCfg.hlw_mkwhs) && (hlw_mkwh_state == 3)) {
      hlw_mkwh_state = 0;
    }
  }
#endif  // FEATURE_POWER_LIMIT
}

void every_second()
{
  char log[LOGSZ], stopic[TOPSZ], svalue[MESSZ], stime[21], stemp0[10], stemp1[10], stemp2[10], stemp3[10];
  uint8_t i, djson;
  float t, h, ped, pi, pc;
  uint16_t pe, pw, pu;

  snprintf_P(stime, sizeof(stime), PSTR("%04d-%02d-%02dT%02d:%02d:%02d"),
    rtcTime.Year, rtcTime.Month, rtcTime.Day, rtcTime.Hour, rtcTime.Minute, rtcTime.Second);

  if (syslog_timer) {  // Restore syslog level
    syslog_timer--;
    if (!syslog_timer) {
      syslog_level = sysCfg.syslog_level;
      if (sysCfg.syslog_level) {
        addLog_P(LOG_LEVEL_INFO, PSTR("SYSL: Syslog logging re-enabled"));  // Might trigger disable again (on purpose)
      }
    }
  }

  if (status_update_timer) {
    status_update_timer--;
    if (!status_update_timer) {
      for (i = 1; i <= Maxdevice; i++) mqtt_publishPowerState(i);
    }
  }

#ifdef USE_DOMOTICZ
  if ((sysCfg.domoticz_update_timer || domoticz_update_timer) && sysCfg.domoticz_relay_idx[0]) {
    domoticz_update_timer--;
    if (domoticz_update_timer <= 0) {
      domoticz_update_timer = sysCfg.domoticz_update_timer;
      for (i = 1; i <= Maxdevice; i++) mqtt_publishDomoticzPowerState(i);
    }
  }
#endif  // USE_DOMOTICZ

  if (sysCfg.tele_period) {
    tele_period++;
    if (tele_period == sysCfg.tele_period -1) {

#ifdef SEND_TELEMETRY_DS18B20
      dsb_readTempPrep();
#endif  // SEND_TELEMETRY_DS18B20

#ifdef SEND_TELEMETRY_DS18x20
      ds18x20_search();      // Check for changes in sensors number
      ds18x20_convert();     // Start Conversion, takes up to one second
#endif  // SEND_TELEMETRY_DS18x20

#ifdef SEND_TELEMETRY_DHT
      dht_readPrep();
#endif  // SEND_TELEMETRY_DHT

#ifdef SEND_TELEMETRY_I2C
      if (htu_detect()) {
        snprintf_P(log, sizeof(log), PSTR("I2C: %s found at address 0x%x"), htu_type(), htu_address());
        addLog(LOG_LEVEL_DEBUG, log);
      }
      if (bmp_detect()) {
        snprintf_P(log, sizeof(log), PSTR("I2C: %s found at address 0x%x"), bmp_type(), bmp_address());
        addLog(LOG_LEVEL_DEBUG, log);
      }
#endif // SEND_TELEMETRY_I2C

    }
    if (tele_period >= sysCfg.tele_period) {
      tele_period = 0;

      if (sysCfg.message_format == JSON) {
        snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/TELEMETRY"), PUB_PREFIX2, sysCfg.mqtt_topic);
        snprintf_P(svalue, sizeof(svalue), PSTR("{\"Time\":\"%s\", \"Uptime\":%d"), stime, uptime);
        for (i = 0; i < Maxdevice; i++) {
          if (Maxdevice == 1) {  // Legacy
            snprintf_P(svalue, sizeof(svalue), PSTR("%s, \"%s\":"), svalue, sysCfg.mqtt_subtopic);
          } else {
            snprintf_P(svalue, sizeof(svalue), PSTR("%s, \"%s%d\":"), svalue, sysCfg.mqtt_subtopic, i +1);
          }
          snprintf_P(svalue, sizeof(svalue), PSTR("%s\"%s\""), svalue, (power & (0x01 << i)) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
        }
        snprintf_P(svalue, sizeof(svalue), PSTR("%s, \"Wifi\":{\"AP\":%d, \"SSID\":\"%s\", \"RSSI\":%d}}"),
          svalue, sysCfg.sta_active +1, sysCfg.sta_ssid[sysCfg.sta_active], WIFI_getRSSIasQuality(WiFi.RSSI()));
        mqtt_publish(stopic, svalue);
      } else {
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
#ifdef SEND_TELEMETRY_UPTIME
        snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/UPTIME"), PUB_PREFIX2, sysCfg.mqtt_topic);
        snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), uptime, (sysCfg.mqtt_units) ? " Hr" : "");
        mqtt_publish(stopic, svalue);
#endif  // SEND_TELEMETRY_UPTIME
#ifdef SEND_TELEMETRY_WIFI
        snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/AP"), PUB_PREFIX2, sysCfg.mqtt_topic);
        snprintf_P(svalue, sizeof(svalue), PSTR("%d"), sysCfg.sta_active +1);
        mqtt_publish(stopic, svalue);
        snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/SSID"), PUB_PREFIX2, sysCfg.mqtt_topic);
        snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.sta_ssid[sysCfg.sta_active]);
        mqtt_publish(stopic, svalue);
        snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/RSSI"), PUB_PREFIX2, sysCfg.mqtt_topic);
        snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), WIFI_getRSSIasQuality(WiFi.RSSI()), (sysCfg.mqtt_units) ? " %" : "");
        mqtt_publish(stopic, svalue);
#endif  // SEND_TELEMETRY_WIFI
      }

      djson = 0;
      if (sysCfg.message_format == JSON) {
        snprintf_P(svalue, sizeof(svalue), PSTR("{\"Time\":\"%s\""), stime);
      }

#ifdef SEND_TELEMETRY_DS18B20
      if (dsb_readTemp(t)) {                 // Check if read failed
        dtostrf(t, 1, TEMP_RESOLUTION &3, stemp1);
        if (sysCfg.message_format == JSON) {
          snprintf_P(svalue, sizeof(svalue), PSTR("%s, \"DS18B20\":{\"Temperature\":\"%s\"}"), svalue, stemp1);
          djson = 1;
        } else {
          snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/DS18B20/TEMPERATURE"), PUB_PREFIX2, sysCfg.mqtt_topic);
          snprintf_P(svalue, sizeof(svalue), PSTR("%s%s"), stemp1, (sysCfg.mqtt_units) ? " C" : "");
          mqtt_publish(stopic, svalue);
        }
      }
#endif  // SEND_TELEMETRY_DS18B20

#ifdef SEND_TELEMETRY_DS18x20
      byte dsxflg = 0;
      for (i = 0; i < ds18x20_sensors(); i++) {
        if (ds18x20_read(i,t)) {           // Check if read failed
          if (!dsxflg) {
            snprintf_P(svalue, sizeof(svalue), PSTR("%s, \"DS18x20\":{"), svalue);
            djson = 1;
            strcpy(stemp1, "");
            dsxflg = 1;
          }
          dtostrf(t, 1, TEMP_RESOLUTION &3, stemp2);
          if (sysCfg.message_format == JSON) {
            snprintf_P(svalue, sizeof(svalue), PSTR("%s%s\"DS%d\":{\"Type\":\"%s\", \"Address\":\"%s\", \"Temperature\":\"%s\"}"),
              svalue, stemp1, i +1, ds18x20_type(i).c_str(), ds18x20_address(i).c_str(), stemp2);
            strcpy(stemp1, ", ");
          } else {
            snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/%s/%d/ADDRESS"), PUB_PREFIX2, sysCfg.mqtt_topic, ds18x20_type(i).c_str(), i +1);
            snprintf_P(svalue, sizeof(svalue), PSTR("%s"), ds18x20_address(i).c_str());
            mqtt_publish(stopic, svalue);
            snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/%s/%d/TEMPERATURE"), PUB_PREFIX2, sysCfg.mqtt_topic, ds18x20_type(i).c_str(), i +1);
            snprintf_P(svalue, sizeof(svalue), PSTR("%s%s"), stemp2, (sysCfg.mqtt_units) ? " C" : "");
            mqtt_publish(stopic, svalue);
          }
        }
      }
      if (dsxflg) snprintf_P(svalue, sizeof(svalue), PSTR("%s}"), svalue);
#endif  // SEND_TELEMETRY_DS18x20

#if defined(SEND_TELEMETRY_DHT) || defined(SEND_TELEMETRY_DHT2)
      if (dht_readTempHum(false, t, h)) {     // Read temperature as Celsius (the default)
        dtostrf(t, 1, TEMP_RESOLUTION &3, stemp1);
        dtostrf(h, 1, HUMIDITY_RESOLUTION &3, stemp2);
        if (sysCfg.message_format == JSON) {
          snprintf_P(svalue, sizeof(svalue), PSTR("%s, \"DHT\":{\"Temperature\":\"%s\", \"Humidity\":\"%s\"}"), svalue, stemp1, stemp2);
          djson = 1;
        } else {
          snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/DHT/TEMPERATURE"), PUB_PREFIX2, sysCfg.mqtt_topic);
          snprintf_P(svalue, sizeof(svalue), PSTR("%s%s"), stemp1, (sysCfg.mqtt_units) ? " C" : "");
          mqtt_publish(stopic, svalue);
          snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/DHT/HUMIDITY"), PUB_PREFIX2, sysCfg.mqtt_topic);
          snprintf_P(svalue, sizeof(svalue), PSTR("%s%s"), stemp2, (sysCfg.mqtt_units) ? " %" : "");
          mqtt_publish(stopic, svalue);
        }
      }
#endif  // SEND_TELEMETRY_DHT/2

#ifdef SEND_TELEMETRY_I2C
      if(htu_found())
      {
        t = htu21_readTemperature();
        h = htu21_readHumidity();
        h = htu21_compensatedHumidity(h, t);
        dtostrf(t, 1, TEMP_RESOLUTION &3, stemp1);
        dtostrf(h, 1, HUMIDITY_RESOLUTION &3, stemp2);
        if (sysCfg.message_format == JSON) {
          snprintf_P(svalue, sizeof(svalue), PSTR("%s, \"%s\":{\"Temperature\":\"%s\", \"Humidity\":\"%s\"}"), svalue, htu_type(), stemp1, stemp2);
          djson = 1;
        } else
        {
          snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/%s/TEMPERATURE"), PUB_PREFIX2, sysCfg.mqtt_topic, htu_type());
          snprintf_P(svalue, sizeof(svalue), PSTR("%s%s"), stemp1, (sysCfg.mqtt_units) ? " C" : "");
          mqtt_publish(stopic, svalue);
          snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/%s/HUMIDITY"), PUB_PREFIX2, sysCfg.mqtt_topic, htu_type());
          snprintf_P(svalue, sizeof(svalue), PSTR("%s%s"), stemp2, (sysCfg.mqtt_units) ? " %" : "");
          mqtt_publish(stopic, svalue);
        }
      }
      if(bmp_found())
      {
        double t_bmp = bmp_readTemperature();
        double p_bmp = bmp_readPressure();
        double h_bmp = bmp_readHumidity();
        dtostrf(t_bmp, 1, TEMP_RESOLUTION &3, stemp1);
        dtostrf(p_bmp, 1, PRESSURE_RESOLUTION &3, stemp2);
        dtostrf(h_bmp, 1, HUMIDITY_RESOLUTION &3, stemp3);
        if (sysCfg.message_format == JSON) {
          if (!strcmp(bmp_type(),"BME280")) {
            snprintf_P(svalue, sizeof(svalue), PSTR("%s, \"%s\":{\"Temperature\":\"%s\", \"Humidity\":\"%s\", \"Pressure\":\"%s\"}"),
              svalue, bmp_type(), stemp1, stemp3, stemp2);
          } else {
            snprintf_P(svalue, sizeof(svalue), PSTR("%s, \"%s\":{\"Temperature\":\"%s\", \"Pressure\":\"%s\"}"),
              svalue, bmp_type(), stemp1, stemp2);
          }
          djson = 1;
        } else {
          snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/%s/TEMPERATURE"), PUB_PREFIX2, sysCfg.mqtt_topic, bmp_type());
          snprintf_P(svalue, sizeof(svalue), PSTR("%s%s"), stemp1, (sysCfg.mqtt_units) ? " C" : "");
          mqtt_publish(stopic, svalue);
          if (!strcmp(bmp_type(),"BME280")) {
            snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/%s/HUMIDITY"), PUB_PREFIX2, sysCfg.mqtt_topic, bmp_type());
            snprintf_P(svalue, sizeof(svalue), PSTR("%s%s"), stemp3, (sysCfg.mqtt_units) ? " %" : "");
            mqtt_publish(stopic, svalue);
          }
          snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/%s/PRESSURE"), PUB_PREFIX2, sysCfg.mqtt_topic, bmp_type());
          snprintf_P(svalue, sizeof(svalue), PSTR("%s%s"), stemp2, (sysCfg.mqtt_units) ? " mbar" : "");
          mqtt_publish(stopic, svalue);
        }
      }
#endif // SEND_TELEMETRY_I2C

      if (djson) {
        snprintf_P(svalue, sizeof(svalue), PSTR("%s}"), svalue);
        mqtt_publish(stopic, svalue);
      }

#ifdef USE_POWERMONITOR
#ifdef SEND_TELEMETRY_ENERGY
      hlw_readEnergy(1, ped, pe, pw, pu, pi, pc);
      dtostrf((float)sysCfg.hlw_kWhyesterday / 100000000, 1, 3, stemp0);
      dtostrf(ped, 1, 3, stemp1);
      dtostrf(pc, 1, 2, stemp2);
      dtostrf(pi, 1, 3, stemp3);
      if (sysCfg.message_format == JSON) {
        snprintf_P(svalue, sizeof(svalue), PSTR("{\"Time\":\"%s\", \"Energy\":{\"Yesterday\":\"%s\", \"Today\":\"%s\", \"Period\":%d, \"Power\":%d, \"Factor\":\"%s\", \"Voltage\":%d, \"Current\":\"%s\"}}"),
          stime, stemp0, stemp1, pe, pw, stemp2, pu, stemp3);
        mqtt_publish(stopic, svalue);
      } else {
        snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/YESTERDAY_ENERGY"), PUB_PREFIX2, sysCfg.mqtt_topic);
        snprintf_P(svalue, sizeof(svalue), PSTR("%s%s"), stemp0, (sysCfg.mqtt_units) ? " kWh" : "");
        mqtt_publish(stopic, svalue);
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
        mqtt_publish(stopic, svalue);
      }
#endif  // SEND_TELEMETRY_ENERGY
#endif  // USE_POWERMONITOR

      if (sysCfg.message_format != JSON) {
        snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/TIME"), PUB_PREFIX2, sysCfg.mqtt_topic);
        snprintf_P(svalue, sizeof(svalue), PSTR("%s"), stime);
        mqtt_publish(stopic, svalue);
      }
    }
  }

#ifdef USE_POWERMONITOR
  hlw_margin_chk();
#endif  // USE_POWERMONITOR

  if ((rtcTime.Minute == 2) && (rtcTime.Second == 30)) {
    uptime++;
    if (sysCfg.message_format == JSON) {
      snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/TELEMETRY"), PUB_PREFIX2, sysCfg.mqtt_topic);
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Time\":\"%s\", \"Uptime\":%d}"), stime, uptime);
    } else {
      snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/UPTIME"), PUB_PREFIX2, sysCfg.mqtt_topic);
      snprintf_P(svalue, sizeof(svalue), PSTR("%d%s"), uptime, (sysCfg.mqtt_units) ? " Hr" : "");
    }
    mqtt_publish(stopic, svalue);
  }
}

void stateloop()
{
  uint8_t button, flag, switchflag;
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
      if (ButtonCode == 0xF500) holdcount = (STATES *4) -1;
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

#ifdef KEY2_PIN
  button = digitalRead(KEY2_PIN);
  if ((button == PRESSED) && (lastbutton2 == NOT_PRESSED)) {
    if (mqttClient.connected() && strcmp(sysCfg.mqtt_topic2,"0")) {
      send_button_power(2, 2);   // Execute commend via MQTT
    } else {
      do_cmnd_power(2, 2);       // Execute command internally
    }
  }
  lastbutton2 = button;
#endif

#ifdef USE_WALL_SWITCH
  button = digitalRead(SWITCH_PIN);
  if (button != lastwallswitch) {
    switchflag = 3;
    switch (sysCfg.switchmode) {
    case TOGGLE:
      switchflag = 2;                // Toggle
      break;
    case FOLLOW:
      switchflag = button & 0x01;    // Follow wall switch state
      break;
    case FOLLOW_INV:
      switchflag = ~button & 0x01;   // Follow inverted wall switch state
      break;
    case PUSHBUTTON:
      if ((button == PRESSED) && (lastwallswitch == NOT_PRESSED)) switchflag = 2;  // Toggle with pushbutton to Gnd
      break;
    case PUSHBUTTON_INV:
      if ((button == NOT_PRESSED) && (lastwallswitch == PRESSED)) switchflag = 2;  // Toggle with releasing pushbutton from Gnd
    }
    if (switchflag < 3) {
      if (mqttClient.connected() && strcmp(sysCfg.mqtt_topic2,"0")) {
        send_button_power(1, switchflag);   // Execute commend via MQTT
      } else {
        do_cmnd_power(1, switchflag);       // Execute command internally
      }
    }
    lastwallswitch = button;
  }
#endif // USE_WALL_SWITCH

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

#ifdef USE_POWERMONITOR
      hlw_savestate();
#endif  // USE_POWERMONITOR

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
    wificheckflag = WIFI_RESTART;
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
  if (MODULE == ELECTRO_DRAGON) Maxdevice = 2;
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

  getClient(MQTTClient, sysCfg.mqtt_client, sizeof(MQTTClient));

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, (LED_INVERTED) ? !blinkstate : blinkstate);

  if ((sysCfg.model < SONOFF_DUAL) || (sysCfg.model > CHANNEL_8)) {
    pinMode(KEY_PIN, INPUT_PULLUP);
    pinMode(REL_PIN, OUTPUT);
#ifdef KEY2_PIN
    pinMode(KEY2_PIN, INPUT_PULLUP);
#endif
#ifdef REL2_PIN
    pinMode(REL2_PIN, OUTPUT);
#endif
  }
  if (sysCfg.savestate) setRelay(power);

#ifdef USE_WALL_SWITCH
  pinMode(SWITCH_PIN, INPUT_PULLUP);            // set pin to input, fitted with external pull up on Sonoff TH10/16 board
  lastwallswitch = digitalRead(SWITCH_PIN);     // set global now so doesn't change the saved power state on first switch check
#endif  // USE_WALL_SWITCH

#if defined(SEND_TELEMETRY_DHT) || defined(SEND_TELEMETRY_DHT2)
  dht_init();
#endif

#ifdef SEND_TELEMETRY_I2C
  Wire.begin(I2C_SDA_PIN,I2C_SCL_PIN);
#endif // SEND_TELEMETRY_I2C

#ifdef USE_POWERMONITOR
  hlw_init();
#endif  // USE_POWERMONITOR

  rtc_init(every_second_cb);

  snprintf_P(log, sizeof(log), PSTR("APP: Project %s (Topic %s, Fallback %s, GroupTopic %s) Version %s"),
    PROJECT, sysCfg.mqtt_topic, MQTTClient, sysCfg.mqtt_grptopic, Version);
  addLog(LOG_LEVEL_INFO, log);
}

void loop()
{
#ifdef USE_WEBSERVER
  pollDnsWeb();
#endif  // USE_WEBSERVER
#ifdef USE_WEMO_EMULATION
  pollUDP();
#endif  // USE_WEMO_EMULATION

  if (millis() >= timerxs) stateloop();

  mqttClient.loop();

  if (Serial.available()) serial();

  yield();
}
