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

#define VERSION                0x03020602   // 3.2.6b

enum log_t   {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE, LOG_LEVEL_ALL};
enum week_t  {Last, First, Second, Third, Fourth};
enum dow_t   {Sun=1, Mon, Tue, Wed, Thu, Fri, Sat};
enum month_t {Jan=1, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec};
enum wifi_t  {WIFI_RESTART, WIFI_SMARTCONFIG, WIFI_MANAGER, WIFI_WPSCONFIG, WIFI_RETRY, MAX_WIFI_OPTION};
enum swtch_t {TOGGLE, FOLLOW, FOLLOW_INV, PUSHBUTTON, PUSHBUTTON_INV, MAX_SWITCH_OPTION};
enum led_t   {LED_OFF, LED_POWER, LED_MQTTSUB, LED_POWER_MQTTSUB, LED_MQTTPUB, LED_POWER_MQTTPUB, LED_MQTT, LED_POWER_MQTT, MAX_LED_OPTION};

#include "sonoff_template.h"

#include "user_config.h"
#include "user_config_override.h"

/*********************************************************************************************\
 * Enable feature by removing leading // or disable feature by adding leading //
\*********************************************************************************************/

//#define USE_SPIFFS                          // Switch persistent configuration from flash to spiffs (+24k code, +0.6k mem)

/*********************************************************************************************\
 * Not released yet
\*********************************************************************************************/

//#define FEATURE_POWER_LIMIT

/*********************************************************************************************\
 * No user configurable items below
\*********************************************************************************************/

// -- Initial module selection --------------------
// Select from (SONOFF_BASIC, SONOFF_RF, SONOFF_SV, SONOFF_TH, SONOFF_DUAL, SONOFF_POW, SONOFF_4CH, S20, SLAMPHER, SONOFF_TOUCH, SONOFF_LED, CH1, CH4, MOTOR, USER_TEST, ELECTRODRAGON)
#define MODULE                 SONOFF_BASIC      // [Module] Select default model

#ifndef SWITCH_MODE
#define SWITCH_MODE            TOGGLE       // TOGGLE, FOLLOW or FOLLOW_INV (the wall switch state)
#endif

#ifndef MQTT_FINGERPRINT
#define MQTT_FINGERPRINT       "A5 02 FF 13 99 9F 8B 39 8E F1 83 4F 11 23 65 0B 32 36 FC 07"
#endif

#ifndef WS2812_LEDS
#define WS2812_LEDS            30           // [Pixels] Number of LEDs
#endif

#define DEF_WIFI_HOSTNAME      "%s-%04d"    // Expands to <MQTT_TOPIC>-<last 4 decimal chars of MAC address>

#define HLW_PREF_PULSE         12530        // was 4975us = 201Hz = 1000W
#define HLW_UREF_PULSE         1950         // was 1666us = 600Hz = 220V
#define HLW_IREF_PULSE         3500         // was 1666us = 600Hz = 4.545A

#define VALUE_UNITS            0            // Default do not show value units (Hr, Sec, V, A, W etc.)
#define MQTT_SUBTOPIC          "POWER"      // Default MQTT subtopic (POWER or LIGHT)
#define MQTT_RETRY_SECS        10           // Seconds to retry MQTT connection
#define APP_POWER              0            // Default saved power state Off
#define MAX_DEVICE             1            // Max number of devices
#define WS2812_MAX_LEDS        256          // Max number of LEDs

#define MAX_POWER_HOLD         10           // Time in SECONDS to allow max agreed power (Pow)
#define MAX_POWER_WINDOW       30           // Time in SECONDS to disable allow max agreed power (Pow)
#define SAFE_POWER_HOLD        10           // Time in SECONDS to allow max unit safe power (Pow)
#define SAFE_POWER_WINDOW      30           // Time in MINUTES to disable allow max unit safe power (Pow)
#define MAX_POWER_RETRY        5            // Retry count allowing agreed power limit overflow (Pow)

#define STATES                 10           // loops per second
#define SYSLOG_TIMER           600          // Seconds to restore syslog_level
#define SERIALLOG_TIMER        600          // Seconds to disable SerialLog
#define OTA_ATTEMPTS           5            // Number of times to try fetching the new firmware

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
#define MAX_STATUS             9

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
#ifdef USE_DISCOVERY
  #include <ESP8266mDNS.h>                  // MQTT, Webserver
#endif  // USE_DISCOVERY
#ifdef USE_SPIFFS
  #include <FS.h>                           // Config
#endif  // USE_SPIFFS
#ifdef USE_I2C
  #include <Wire.h>                         // I2C support library
#endif  // USE_I2C

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

const char wificfg[5][12] PROGMEM = { "Restart", "Smartconfig", "Wifimanager", "WPSconfig", "Retry" };

struct SYSCFG2 {      // Version 2.x (old)
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
  byte          value_units;
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
  byte          message_format;  // Not used since 3.2.6a
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
  byte          migflg;
  int16_t       savedata;
  byte          savestate;
  byte          model;
  int8_t        timezone;
  char          otaUrl[101];
  char          ex_friendlyname[33];  // Not used since 3.2.5 - see below

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
  char          button_topic[33];
  char          mqtt_grptopic[33];
  char          mqtt_subtopic[33];
  byte          mqtt_button_retain;
  byte          mqtt_power_retain;
  byte          value_units;
  byte          message_format;  // Not used since 3.2.6a
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

  uint16_t      pulsetime;
  uint8_t       poweronstate;
  uint16_t      blinktime;
  uint16_t      blinkcount;

  uint16_t      ws_pixels;
  uint8_t       ws_red;
  uint8_t       ws_green;
  uint8_t       ws_blue;
  uint8_t       ws_ledtable;
  uint8_t       ws_dimmer;
  uint8_t       ws_fade;
  uint8_t       ws_speed;
  uint8_t       ws_scheme;
  uint8_t       ws_width;
  uint16_t      ws_wakeup;  

  char          friendlyname[4][33];

  uint8_t       module;
  mytmplt       my_module;

  uint16_t      led_pixels;
  uint8_t       led_color[5];
  uint8_t       led_table;
  uint8_t       led_dimmer[3];
  uint8_t       led_fade;
  uint8_t       led_speed;
  uint8_t       led_scheme;
  uint8_t       led_width;
  uint16_t      led_wakeup;

  char          switch_topic[33];
  byte          mqtt_switch_retain;
  uint8_t       mqtt_enabled;
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
int otaok = 0;                        // OTA result
int restartflag = 0;                  // Sonoff restart flag
int wificheckflag = WIFI_RESTART;     // Wifi state flag
int uptime = 0;                       // Current uptime in hours
int tele_period = 0;                  // Tele period timer
String Log[MAX_LOG_LINES];            // Web log buffer
byte logidx = 0;                      // Index in Web log buffer
byte Maxdevice = MAX_DEVICE;          // Max number of devices supported
int status_update_timer = 0;          // Refresh initial status
uint16_t pulse_timer = 0;             // Power off timer
uint16_t blink_timer = 0;             // Power cycle timer
uint16_t blink_counter = 0;           // Number of blink cycles
uint8_t blink_power;                  // Blink power state
uint8_t blink_mask = 0;               // Blink relay active mask
uint8_t blink_powersave;              // Blink start power save state
uint16_t mqtt_cmnd_publish = 0;       // ignore flag for publish command

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
byte seriallog_level;                 // Current copy of sysCfg.seriallog_level
uint16_t seriallog_timer = 0;         // Timer to disable Seriallog

int blinks = 201;                     // Number of LED blinks
uint8_t blinkstate = 0;               // LED state

uint8_t lastbutton[4] = { NOT_PRESSED, NOT_PRESSED, NOT_PRESSED, NOT_PRESSED };     // Last button states
uint8_t holdcount = 0;                // Timer recording button hold
uint8_t multiwindow = 0;              // Max time between button presses to record press count
uint8_t multipress = 0;               // Number of button presses within multiwindow
uint8_t lastwallswitch[4];            // Last wall switch states

mytmplt my_module;                    // Active copy of GPIOs
uint8_t pin[GPIO_MAX];                // Possible pin configurations
uint8_t rel_inverted[4] = { 0 };      // Relay inverted flag (1 = (0 = On, 1 = Off))
uint8_t led_inverted[4] = { 0 };      // LED inverted flag (1 = (0 = On, 1 = Off))
uint8_t swt_flg = 0;                  // Any external switch configured
uint8_t dht_type = 0;                 // DHT type (DHT11, DHT21 or DHT22)
uint8_t hlw_flg = 0;                  // Power monitor configured
uint8_t i2c_flg = 0;                  // I2C configured

boolean mDNSbegun = false;

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

#ifdef USE_DOMOTICZ
  int domoticz_update_timer = 0;
  byte domoticz_update_flag = 1;
#endif  // USE_DOMOTICZ

/********************************************************************************************/

void CFG_DefaultSet()
{
  memset(&sysCfg, 0x00, sizeof(SYSCFG));

  sysCfg.cfg_holder = CFG_HOLDER;
  sysCfg.saveFlag = 0;
  sysCfg.version = VERSION;
  sysCfg.bootcount = 0;
  sysCfg.migflg = 0;
  sysCfg.savedata = SAVE_DATA;
  sysCfg.savestate = SAVE_STATE;
  sysCfg.module = MODULE;
  sysCfg.model = 0;
  sysCfg.timezone = APP_TIMEZONE;
  strlcpy(sysCfg.otaUrl, OTA_URL, sizeof(sysCfg.otaUrl));
  strlcpy(sysCfg.ex_friendlyname, FRIENDLY_NAME1, sizeof(sysCfg.ex_friendlyname));

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
  strlcpy(sysCfg.button_topic, "0", sizeof(sysCfg.button_topic));
  strlcpy(sysCfg.mqtt_grptopic, MQTT_GRPTOPIC, sizeof(sysCfg.mqtt_grptopic));
  strlcpy(sysCfg.mqtt_subtopic, MQTT_SUBTOPIC, sizeof(sysCfg.mqtt_subtopic));
  sysCfg.mqtt_button_retain = MQTT_BUTTON_RETAIN;
  sysCfg.mqtt_power_retain = MQTT_POWER_RETAIN;
  sysCfg.value_units = VALUE_UNITS;
  sysCfg.message_format = 0;
  sysCfg.tele_period = TELE_PERIOD;

  sysCfg.power = APP_POWER;
  sysCfg.poweronstate = APP_POWERON_STATE;
  sysCfg.pulsetime = APP_PULSETIME;
  sysCfg.ledstate = APP_LEDSTATE;
  sysCfg.switchmode = SWITCH_MODE;
  sysCfg.blinktime = APP_BLINKTIME;
  sysCfg.blinkcount = APP_BLINKCOUNT;

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

  sysCfg.ws_pixels = WS2812_LEDS;
  sysCfg.ws_red = 255;
  sysCfg.ws_green = 0;
  sysCfg.ws_blue = 0;
  sysCfg.ws_ledtable = 0;
  sysCfg.ws_dimmer = 8;
  sysCfg.ws_fade = 0;
  sysCfg.ws_speed = 1;
  sysCfg.ws_scheme = 1;
  sysCfg.ws_width = 1;
  sysCfg.ws_wakeup = 0;

  strlcpy(sysCfg.friendlyname[0], FRIENDLY_NAME1, sizeof(sysCfg.friendlyname[0]));
  strlcpy(sysCfg.friendlyname[1], FRIENDLY_NAME2, sizeof(sysCfg.friendlyname[1]));
  strlcpy(sysCfg.friendlyname[2], FRIENDLY_NAME3, sizeof(sysCfg.friendlyname[2]));
  strlcpy(sysCfg.friendlyname[3], FRIENDLY_NAME4, sizeof(sysCfg.friendlyname[3]));

  for (byte i = 0; i < MAX_GPIO_PIN; i++) sysCfg.my_module.gp.io[i] = 0;
  
  sysCfg.led_pixels = 0;
  for (byte i = 0; i < 5; i++) sysCfg.led_color[i] = 255;
  sysCfg.led_table = 0;
  for (byte i = 0; i < 3; i++) sysCfg.led_dimmer[i] = 10;
  sysCfg.led_fade = 0;
  sysCfg.led_speed = 0;
  sysCfg.led_scheme = 0;
  sysCfg.led_width = 0;
  sysCfg.led_wakeup = 0;
  
  strlcpy(sysCfg.switch_topic, "0", sizeof(sysCfg.switch_topic));
  sysCfg.mqtt_switch_retain = MQTT_SWITCH_RETAIN;
  sysCfg.mqtt_enabled = MQTT_USE;
  
}

void CFG_Default()
{
  addLog_P(LOG_LEVEL_NONE, PSTR("Config: Use default configuration"));
  CFG_DefaultSet();
  CFG_Save();
}

void CFG_Migrate_Part2()
{
  addLog_P(LOG_LEVEL_NONE, PSTR("Config: Migrating configuration"));
  CFG_DefaultSet();

  sysCfg.seriallog_level = sysCfg2.seriallog_level;
  sysCfg.syslog_level = sysCfg2.syslog_level;
  strlcpy(sysCfg.syslog_host, sysCfg2.syslog_host, sizeof(sysCfg.syslog_host));
  strlcpy(sysCfg.sta_ssid[0], sysCfg2.sta_ssid1, sizeof(sysCfg.sta_ssid[0]));
  strlcpy(sysCfg.sta_pwd[0], sysCfg2.sta_pwd1, sizeof(sysCfg.sta_pwd[0]));
  strlcpy(sysCfg.otaUrl, sysCfg2.otaUrl, sizeof(sysCfg.otaUrl));
  strlcpy(sysCfg.mqtt_host, sysCfg2.mqtt_host, sizeof(sysCfg.mqtt_host));
  strlcpy(sysCfg.mqtt_grptopic, sysCfg2.mqtt_grptopic, sizeof(sysCfg.mqtt_grptopic));
  strlcpy(sysCfg.mqtt_topic, sysCfg2.mqtt_topic, sizeof(sysCfg.mqtt_topic));
  strlcpy(sysCfg.button_topic, sysCfg2.mqtt_topic2, sizeof(sysCfg.button_topic));
  strlcpy(sysCfg.mqtt_subtopic, sysCfg2.mqtt_subtopic, sizeof(sysCfg.mqtt_subtopic));
  sysCfg.timezone = sysCfg2.timezone;
  sysCfg.power = sysCfg2.power;
  if (sysCfg2.version >= 0x01000D00) {  // 1.0.13
    sysCfg.ledstate = sysCfg2.ledstate;
  }
  if (sysCfg2.version >= 0x01001600) {  // 1.0.22
    sysCfg.mqtt_port = sysCfg2.mqtt_port;
    strlcpy(sysCfg.mqtt_client, sysCfg2.mqtt_client, sizeof(sysCfg.mqtt_client));
    strlcpy(sysCfg.mqtt_user, sysCfg2.mqtt_user, sizeof(sysCfg.mqtt_user));
    strlcpy(sysCfg.mqtt_pwd, sysCfg2.mqtt_pwd, sizeof(sysCfg.mqtt_pwd));
    strlcpy(sysCfg.ex_friendlyname, sysCfg2.mqtt_client, sizeof(sysCfg.ex_friendlyname));
  }
  if (sysCfg2.version >= 0x01001700) {  // 1.0.23
    sysCfg.webserver = sysCfg2.webserver;
  }
  if (sysCfg2.version >= 0x01001A00) {  // 1.0.26
    sysCfg.bootcount = sysCfg2.bootcount;
    strlcpy(sysCfg.hostname, sysCfg2.hostname, sizeof(sysCfg.hostname));
    sysCfg.syslog_port = sysCfg2.syslog_port;
  }
  if (sysCfg2.version >= 0x01001B00) {  // 1.0.27
    sysCfg.weblog_level = sysCfg2.weblog_level;
  }
  if (sysCfg2.version >= 0x01001C00) {  // 1.0.28
    sysCfg.tele_period = sysCfg2.tele_period;
    if ((sysCfg.tele_period > 0) && (sysCfg.tele_period < 10)) sysCfg.tele_period = 10;   // Do not allow periods < 10 seconds
  }
  if (sysCfg2.version >= 0x01002000) {  // 1.0.32
    sysCfg.sta_config = sysCfg2.sta_config;
  }
  if (sysCfg2.version >= 0x01002300) {  // 1.0.35
    sysCfg.savedata = sysCfg2.savedata;
  }
  if (sysCfg2.version >= 0x02000000) {  // 2.0.0
    sysCfg.model = sysCfg2.model;
  }
  if (sysCfg2.version >= 0x02000300) {  // 2.0.3
    sysCfg.mqtt_button_retain = sysCfg2.mqtt_retain;
    sysCfg.savestate = sysCfg2.savestate;
  }
  if (sysCfg2.version >= 0x02000500) {  // 2.0.5
    sysCfg.hlw_pcal = sysCfg2.hlw_pcal;
    sysCfg.hlw_ucal = sysCfg2.hlw_ucal;
    sysCfg.hlw_ical = sysCfg2.hlw_ical;
    sysCfg.hlw_kWhyesterday = sysCfg2.hlw_kWhyesterday;
    sysCfg.value_units = sysCfg2.value_units;
  }
  if (sysCfg2.version >= 0x02000600) {  // 2.0.6
    sysCfg.hlw_pmin = sysCfg2.hlw_pmin;
    sysCfg.hlw_pmax = sysCfg2.hlw_pmax;
    sysCfg.hlw_umin = sysCfg2.hlw_umin;
    sysCfg.hlw_umax = sysCfg2.hlw_umax;
    sysCfg.hlw_imin = sysCfg2.hlw_imin;
    sysCfg.hlw_imax = sysCfg2.hlw_imax;
  }
  if (sysCfg2.version >= 0x02000700) {  // 2.0.7
    sysCfg.message_format = 0;
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
    sysCfg.hlw_mpl = sysCfg2.hlw_mpl;              // MaxPowerLimit
    sysCfg.hlw_mplh = sysCfg2.hlw_mplh;
    sysCfg.hlw_mplw = sysCfg2.hlw_mplw;
    sysCfg.hlw_mspl = sysCfg2.hlw_mspl;            // MaxSafePowerLimit
    sysCfg.hlw_msplh = sysCfg2.hlw_msplh;
    sysCfg.hlw_msplw = sysCfg2.hlw_msplw;
    sysCfg.hlw_mkwh = sysCfg2.hlw_mkwh;            // MaxEnergy
    sysCfg.hlw_mkwhs = sysCfg2.hlw_mkwhs;          // MaxEnergyStart
  }
  if (sysCfg2.version >= 0x02001000) {  // 2.0.16
    sysCfg.hlw_kWhtoday = sysCfg2.hlw_kWhtoday;
    sysCfg.hlw_kWhdoy = sysCfg2.hlw_kWhdoy;
  }
  if (sysCfg2.version >= 0x02001200) {  // 2.0.18
    sysCfg.switchmode = sysCfg2.switchmode;
  }
  if (sysCfg2.version >= 0x02010000) {  // 2.1.0
    strlcpy(sysCfg.mqtt_fingerprint, sysCfg2.mqtt_fingerprint, sizeof(sysCfg.mqtt_fingerprint));
  }
  if (sysCfg2.version >= 0x02010200) {  // 2.1.2
    sysCfg.sta_active = sysCfg2.sta_active;
    strlcpy(sysCfg.sta_ssid[1], sysCfg2.sta_ssid2, sizeof(sysCfg.sta_ssid[1]));
    strlcpy(sysCfg.sta_pwd[1], sysCfg2.sta_pwd2, sizeof(sysCfg.sta_pwd[1]));
  }
  CFG_Save();
}

void CFG_Delta()
{
  if (sysCfg.version != VERSION) {      // Fix version dependent changes
    if (sysCfg.version < 0x03000600) {  // 3.0.6 - Add parameter
      sysCfg.pulsetime = APP_PULSETIME;
    }
    if (sysCfg.version < 0x03010100) {  // 3.1.1 - Add parameter
      sysCfg.poweronstate = APP_POWERON_STATE;
    }
    if (sysCfg.version < 0x03010200) {  // 3.1.2 - Add parameter
      if (sysCfg.poweronstate == 2) sysCfg.poweronstate = 3;
    }
    if (sysCfg.version < 0x03010600) {  // 3.1.6 - Add parameter
      sysCfg.blinktime = APP_BLINKTIME;
      sysCfg.blinkcount = APP_BLINKCOUNT;
    }
    if (sysCfg.version < 0x03011000) {  // 3.1.16 - Add parameter
      getClient(sysCfg.ex_friendlyname, sysCfg.mqtt_client, sizeof(sysCfg.ex_friendlyname));
    }
    if (sysCfg.version < 0x03020400) {  // 3.2.4 - Add parameter
      sysCfg.ws_pixels = WS2812_LEDS;
      sysCfg.ws_red = 255;
      sysCfg.ws_green = 0;
      sysCfg.ws_blue = 0;
      sysCfg.ws_ledtable = 0;
      sysCfg.ws_dimmer = 8;
      sysCfg.ws_fade = 0;
      sysCfg.ws_speed = 1;
      sysCfg.ws_scheme = 1;
      sysCfg.ws_width = 1;
      sysCfg.ws_wakeup = 0;
    }
    if (sysCfg.version < 0x03020500) {  // 3.2.5 - Add parameter
      strlcpy(sysCfg.friendlyname[0], sysCfg.ex_friendlyname, sizeof(sysCfg.friendlyname[0]));
      strlcpy(sysCfg.friendlyname[1], FRIENDLY_NAME2, sizeof(sysCfg.friendlyname[1]));
      strlcpy(sysCfg.friendlyname[2], FRIENDLY_NAME3, sizeof(sysCfg.friendlyname[2]));
      strlcpy(sysCfg.friendlyname[3], FRIENDLY_NAME4, sizeof(sysCfg.friendlyname[3]));
    }      
    if (sysCfg.version < 0x03020501) {  // 3.2.5a - Add parameter
      sysCfg.module = MODULE;
      for (byte i = 0; i < MAX_GPIO_PIN; i++) sysCfg.my_module.gp.io[i] = 0;

      sysCfg.led_pixels = 0;
      for (byte i = 0; i < 5; i++) sysCfg.led_color[i] = 255;
      sysCfg.led_table = 0;
      for (byte i = 0; i < 3; i++) sysCfg.led_dimmer[i] = 10;
      sysCfg.led_fade = 0;
      sysCfg.led_speed = 0;
      sysCfg.led_scheme = 0;
      sysCfg.led_width = 0;
      sysCfg.led_wakeup = 0;

      strlcpy(sysCfg.switch_topic, sysCfg.button_topic, sizeof(sysCfg.switch_topic));
      sysCfg.mqtt_switch_retain = MQTT_SWITCH_RETAIN;
      sysCfg.mqtt_enabled = MQTT_USE;
    }

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
    if (strstr(input, "%") == input) {
      output[0] = '\0';
    } else {
      token = strtok(NULL, "");
    }
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
  if ((sysCfg.module == SONOFF_DUAL) || (sysCfg.module == CH4)) {
    Serial.write(0xA0);
    Serial.write(0x04);
    Serial.write(power);
    Serial.write(0xA1);
    Serial.write('\n');
    Serial.flush();
  } else {
    if (sysCfg.module == SONOFF_LED) {
      sl_setColor(power &1);
    } else {
      for (byte i = 0; i < Maxdevice; i++) {
        if (pin[GPIO_REL1 +i] < 99) digitalWrite(pin[GPIO_REL1 +i], power & 0x1);
        power >>= 1;
      }
    }
  }
  power_steady_cntr = 2;
}

void sl_setDim(uint8_t *my_color)
{
  float newDim, fmyCld, fmyWrm, fmyRed, fmyGrn, fmyBlu;
  
  newDim = 100 / (float)sysCfg.led_dimmer[0];
  fmyCld = (float)sysCfg.led_color[0] / newDim;
  newDim = 100 / (float)sysCfg.led_dimmer[1];
  fmyWrm = (float)sysCfg.led_color[1] / newDim;
  newDim = 100 / (float)sysCfg.led_dimmer[2];
  fmyRed = (float)sysCfg.led_color[2] / newDim;
  fmyGrn = (float)sysCfg.led_color[3] / newDim;
  fmyBlu = (float)sysCfg.led_color[4] / newDim;
  my_color[0] = (uint8_t)fmyCld;
  my_color[1] = (uint8_t)fmyWrm;
  my_color[2] = (uint8_t)fmyRed;
  my_color[3] = (uint8_t)fmyGrn;
  my_color[4] = (uint8_t)fmyBlu;
}

void sl_setColor(byte type)
{
// 0 = Off
// 1 = On
// 2 = Dim cold
// 3 = Dim Warm
// 4 = Dim color
  
  uint8_t my_color[5];
  
  sl_setDim(my_color);
  if (type == 0) {
    for (byte i = 0; i < 5; i++) {
      if (pin[GPIO_PWM0 +i] < 99) analogWrite(pin[GPIO_PWM0 +i], 0);
    }
  }
  else if (type == 1) {
    for (byte i = 0; i < 5; i++) {
      if (pin[GPIO_PWM0 +i] < 99) analogWrite(pin[GPIO_PWM0 +i], my_color[i]);
    }
  }
  else if (type == 2) {  // Cold
    if (pin[GPIO_PWM0] < 99) analogWrite(pin[GPIO_PWM0], my_color[0]);
  }
  else if (type == 3) {  // Warm
    if (pin[GPIO_PWM1] < 99) analogWrite(pin[GPIO_PWM1], my_color[1]);
  }
  else if (type == 4) {  // Color
    for (byte i = 2; i < 5; i++) {
      if (pin[GPIO_PWM0 +i] < 99) analogWrite(pin[GPIO_PWM0 +i], my_color[i]);
    }
  }
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

uint32_t Atoh(char *s)
{
  uint32_t value = 0, digit;
  int8_t c;
  
  while((c = *s++)){
    if('0' <= c && c <= '9')
      digit = c - '0';
    else if('A' <= c && c <= 'F')
      digit = c - 'A' + 10;
    else if('a' <= c && c<= 'f')
      digit = c - 'a' + 10;
    else break;
    value = (value << 4) | digit;
  }
  return value;
}

/********************************************************************************************/

void mqtt_publish_sec(const char* topic, const char* data, boolean retained)
{
  char log[TOPSZ+MESSZ];

  if (sysCfg.mqtt_enabled) {
    if (mqttClient.publish(topic, data, retained)) {
      snprintf_P(log, sizeof(log), PSTR("MQTT: %s = %s%s"), topic, data, (retained) ? " (retained)" : "");
//      mqttClient.loop();  // Do not use here! Will block previous publishes
    } else  {
      snprintf_P(log, sizeof(log), PSTR("RSLT: %s = %s"), topic, data);
    }
  } else {
    snprintf_P(log, sizeof(log), PSTR("RSLT: %s = %s"), strrchr(topic,'/')+1, data);
  }

  addLog(LOG_LEVEL_INFO, log);
  if (sysCfg.ledstate &0x04) blinks++;
}

void mqtt_publish(const char* topic, const char* data, boolean retained)
{
  char *me;

  if (!strcmp(SUB_PREFIX,PUB_PREFIX)) {
    me = strstr(topic,SUB_PREFIX);
    if (me == topic) mqtt_cmnd_publish += 8;
  }
  mqtt_publish_sec(topic, data, retained);
}

void mqtt_publish(const char* topic, const char* data)
{
  mqtt_publish(topic, data, false);
}

void mqtt_publishPowerState(byte device)
{
  char stopic[TOPSZ], svalue[MESSZ], sdevice[10];

  if ((device < 1) || (device > Maxdevice)) device = 1;
  snprintf_P(sdevice, sizeof(sdevice), PSTR("%d"), device);
  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/RESULT"), PUB_PREFIX, sysCfg.mqtt_topic);
  snprintf_P(svalue, sizeof(svalue), PSTR("{\"%s%s\":\"%s\"}"),
    sysCfg.mqtt_subtopic, (Maxdevice > 1) ? sdevice : "", (power & (0x01 << (device -1))) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
  mqtt_publish(stopic, svalue);
  json2legacy(stopic, svalue);
  mqtt_publish(stopic, svalue, sysCfg.mqtt_power_retain);
}

void mqtt_publishPowerBlinkState(byte device)
{
  char stopic[TOPSZ], svalue[MESSZ], sdevice[10];

  if ((device < 1) || (device > Maxdevice)) device = 1;
  snprintf_P(sdevice, sizeof(sdevice), PSTR("%d"), device);
  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/RESULT"), PUB_PREFIX, sysCfg.mqtt_topic);
  snprintf_P(svalue, sizeof(svalue), PSTR("{\"%s%s\":\"BLINK %s\"}"),
    sysCfg.mqtt_subtopic, (Maxdevice > 1) ? sdevice : "", (blink_mask & (0x01 << (device -1))) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
  mqtt_publish(stopic, svalue);
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

  if (sysCfg.mqtt_enabled) {
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
  }

  if (mqttflag) {
    snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/RESULT"), PUB_PREFIX2, sysCfg.mqtt_topic);
    snprintf_P(svalue, sizeof(svalue), PSTR("{\"Info1\":{\"Module\":\"%s\", \"Version\":\"%s\", \"FallbackTopic\":\"%s\", \"GroupTopic\":\"%s\"}}"),
      my_module.name, Version, MQTTClient, sysCfg.mqtt_grptopic);
    mqtt_publish(stopic, svalue);
#ifdef USE_WEBSERVER
    if (sysCfg.webserver) {
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Info2\":{\"WebserverMode\":\"%s\", \"Hostname\":\"%s\", \"IPaddress\":\"%s\"}}"),
        (sysCfg.webserver == 2) ? "Admin" : "User", Hostname, WiFi.localIP().toString().c_str());
      mqtt_publish(stopic, svalue);
    }
#endif  // USE_WEBSERVER
    if (sysCfg.mqtt_enabled && (MQTT_MAX_PACKET_SIZE < (TOPSZ+MESSZ))) {
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Warning1\":\"Change MQTT_MAX_PACKET_SIZE in libraries/PubSubClient.h to at least %d\"}"), TOPSZ+MESSZ);
      mqtt_publish(stopic, svalue);
    }
    if (!spiffsPresent()) {
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Warning2\":\"No persistent config. Please reflash with at least 16K SPIFFS\"}"));
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

  if (!sysCfg.mqtt_enabled) {
    mqtt_connected();
    return;
  }
#if defined(USE_WEMO_EMULATION) || defined(USE_HUE_EMULATION)
  UDP_Disconnect();
#endif  // USE_WEMO_EMULATION || USE_HUE_EMULATION
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
    mqttClient.setCallback(mqttDataCb);
    mqttflag = 1;
    mqttcounter = 1;
    return;
  }

  addLog_P(LOG_LEVEL_INFO, PSTR("MQTT: Attempting connection..."));
#ifndef USE_MQTT_TLS
#ifdef USE_DISCOVERY
#ifdef MQTT_HOST_DISCOVERY
  mdns_discoverMQTTServer();
#endif  // MQTT_HOST_DISCOVERY
#endif  // USE_DISCOVERY
#endif  // USE_MQTT_TLS
  mqttClient.setServer(sysCfg.mqtt_host, sysCfg.mqtt_port);
  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/LWT"), PUB_PREFIX2, sysCfg.mqtt_topic);
  snprintf_P(svalue, sizeof(svalue), PSTR("Offline"));
  if (mqttClient.connect(MQTTClient, sysCfg.mqtt_user, sysCfg.mqtt_pwd, stopic, 1, true, svalue)) {
    addLog_P(LOG_LEVEL_INFO, PSTR("MQTT: Connected"));
    mqttcounter = 0;
    snprintf_P(svalue, sizeof(svalue), PSTR("Online"));
    mqtt_publish(stopic, svalue, true);
    mqtt_connected();
  } else {
    snprintf_P(log, sizeof(log), PSTR("MQTT: CONNECT FAILED, rc %d. Retry in %d seconds"), mqttClient.state(), mqttcounter);
    addLog(LOG_LEVEL_DEBUG, log);
  }
}

void mqttDataCb(char* topic, byte* data, unsigned int data_len)
{
  char *str;
  char svalue[MESSZ];

  if (!strcmp(SUB_PREFIX,PUB_PREFIX)) {
    str = strstr(topic,SUB_PREFIX);
    if ((str == topic) && mqtt_cmnd_publish) {
      if (mqtt_cmnd_publish > 8) mqtt_cmnd_publish -= 8; else mqtt_cmnd_publish = 0;
      return;
    }
  }

  uint16_t i = 0, grpflg = 0, index;
  char topicBuf[TOPSZ], dataBuf[data_len+1], dataBufUc[MESSZ];
  char *p, *mtopic = NULL, *type = NULL;
  char stopic[TOPSZ], stemp1[TOPSZ], stemp2[10];

  strncpy(topicBuf, topic, sizeof(topicBuf));
  memcpy(dataBuf, data, sizeof(dataBuf));
  dataBuf[sizeof(dataBuf)-1] = 0;

  snprintf_P(svalue, sizeof(svalue), PSTR("RSLT: Receive topic %s, data size %d, data %s"), topicBuf, data_len, dataBuf);
  addLog(LOG_LEVEL_DEBUG_MORE, svalue);

#ifdef USE_DOMOTICZ
  if (sysCfg.mqtt_enabled) {
    domoticz_update_flag = 1;
    if (!strncmp(topicBuf, sysCfg.domoticz_out_topic, strlen(sysCfg.domoticz_out_topic)) != 0) {
      unsigned long idx = 0;
      int16_t nvalue;

      if (data_len < 20) return;
      idx = getKeyIntValue(dataBuf,"\"idx\"");
      nvalue = getKeyIntValue(dataBuf,"\"nvalue\"");

      snprintf_P(svalue, sizeof(svalue), PSTR("DMTZ: idx %d, nvalue %d"), idx, nvalue);
      addLog(LOG_LEVEL_DEBUG_MORE, svalue);

      data_len = 0;
      if (nvalue == 0 || nvalue == 1) {
        for (i = 0; i < Maxdevice; i++) {
          if ((idx > 0) && (idx == sysCfg.domoticz_relay_idx[i])) {
            snprintf_P(dataBuf, sizeof(dataBuf), PSTR("%d"), nvalue);
            data_len = strlen(dataBuf);
            break;
          }
        }
      }
      if (!data_len) return;
      if (((power >> i) &1) == nvalue) return;
      snprintf_P(stemp1, sizeof(stemp1), PSTR("%d"), i +1);
      snprintf_P(topicBuf, sizeof(topicBuf), PSTR("%s/%s/%s%s"),
        SUB_PREFIX, sysCfg.mqtt_topic, sysCfg.mqtt_subtopic, (Maxdevice > 1) ? stemp1 : "");

      snprintf_P(svalue, sizeof(svalue), PSTR("DMTZ: Receive topic %s, data size %d, data %s"), topicBuf, data_len, dataBuf);
      addLog(LOG_LEVEL_DEBUG_MORE, svalue);

      domoticz_update_flag = 0;
    }
  }
#endif  // USE_DOMOTICZ

  memmove(topicBuf, topicBuf+sizeof(SUB_PREFIX), sizeof(topicBuf)-sizeof(SUB_PREFIX));  // Remove SUB_PREFIX

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
  if (type != NULL) {
    for (i = 0; i < strlen(type); i++) {
      type[i] = toupper(type[i]);
      if (isdigit(type[i])) {
        index = atoi(type +i);
        break;
      }
    }
    type[i] = '\0';
  }

  for (i = 0; i <= sizeof(dataBufUc); i++) dataBufUc[i] = toupper(dataBuf[i]);

  snprintf_P(svalue, sizeof(svalue), PSTR("RSLT: DataCb Topic %s, Group %d, Index %d, Type %s, Data %s (%s)"),
    mtopic, grpflg, index, type, dataBuf, dataBufUc);
  addLog(LOG_LEVEL_DEBUG, svalue);

  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/RESULT"), PUB_PREFIX, sysCfg.mqtt_topic);
  if (type != NULL) {
    snprintf_P(svalue, sizeof(svalue), PSTR("{\"Command\":\"Error\"}"));
    if (sysCfg.ledstate &0x02) blinks++;

    if (!strcmp(dataBufUc,"?")) data_len = 0;
    int16_t payload = atoi(dataBuf);     // -32766 - 32767
    uint16_t payload16 = atoi(dataBuf);  // 0 - 65535
    if (!strcmp(dataBufUc,"OFF") || !strcmp(dataBufUc,"STOP")) payload = 0;
    if (!strcmp(dataBufUc,"ON") || !strcmp(dataBufUc,"START") || !strcmp(dataBufUc,"USER")) payload = 1;
    if (!strcmp(dataBufUc,"TOGGLE") || !strcmp(dataBufUc,"ADMIN")) payload = 2;
    if (!strcmp(dataBufUc,"BLINK")) payload = 3;
    if (!strcmp(dataBufUc,"BLINKOFF")) payload = 4;

    if ((!strcmp(type,"POWER") || !strcmp(type,"LIGHT")) && (index > 0) && (index <= Maxdevice)) {
      snprintf_P(sysCfg.mqtt_subtopic, sizeof(sysCfg.mqtt_subtopic), PSTR("%s"), type);
      if ((data_len == 0) || (payload > 4)) payload = 9;
      do_cmnd_power(index, payload);
      return;
    }
    else if (!strcmp(type,"STATUS")) {
      if ((data_len == 0) || (payload < 0) || (payload > MAX_STATUS)) payload = 99;
      publish_status(payload);
      return;
    }
    else if ((sysCfg.module != MOTOR) && !strcmp(type,"POWERONSTATE")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 3)) {
        sysCfg.poweronstate = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"PowerOnState\":%d}"), sysCfg.poweronstate);
    }
    else if (!strcmp(type,"PULSETIME")) {
      if (data_len > 0) {
        sysCfg.pulsetime = payload16;  // 0 - 65535
        pulse_timer = 0;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"PulseTime\":%d}"), sysCfg.pulsetime);
    }
    else if (!strcmp(type,"BLINKTIME")) {
      if ((data_len > 0) && (payload > 2) && (payload <= 3600)) {
        sysCfg.blinktime = payload;
        if (blink_timer) blink_timer = sysCfg.blinktime;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"BlinkTime\":%d}"), sysCfg.blinktime);
    }
    else if (!strcmp(type,"BLINKCOUNT")) {
      if (data_len > 0) {
        sysCfg.blinkcount = payload16;  // 0 - 65535
        if (blink_counter) blink_counter = sysCfg.blinkcount *2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"BlinkCount\":%d}"), sysCfg.blinkcount);
    }

/*** Sonoff Led Commands *********************************************************************/
    
/*    
    else if ((sysCfg.module == SONOFF_LED) && !strcmp(type,"COLOR"))) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 255)) {
        sysCfg.led_color[index -1] = payload;
        sl_setColor(4);
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Color\":\"%s\"}"), sysCfg.led_color[index -1]);
    }
    else if ((sysCfg.module == SONOFF_LED) && !strcmp(type,"CWRGB") && (index > 0) && (index <= 5)) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 255)) {
        sysCfg.led_color[index -1] = payload;
        sl_setColor(1);
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"CWRGB%d\":%d}"), index, sysCfg.led_color[index -1]);
    }
*/    
    else if ((sysCfg.module == SONOFF_LED) && !strcmp(type,"DIMMER") && (index > 0) && (index <= 3)) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 100)) {
        sysCfg.led_dimmer[index -1] = payload;
        power = 1;
        sl_setColor(index +1);
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Dimmer%d\":%d}"), index, sysCfg.led_dimmer[index -1]);
    }

/*********************************************************************************************/

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
    else if (!strcmp(type,"MODULE")) {
      if ((data_len > 0) && (payload > 0) && (payload <= MAXMODULE)) {
        sysCfg.module = payload -1;
        restartflag = 2;
      }
      snprintf_P(stemp1, sizeof(stemp1), modules[sysCfg.module].name);
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Module\":\"%s (%d)\"}"), stemp1, sysCfg.module +1);
    }
    else if (!strcmp(type,"MODULES")) {
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Modules1\":\""), svalue);
      byte jsflg = 0;
      for (byte i = 0; i < 11; i++) {
        if (jsflg) snprintf_P(svalue, sizeof(svalue), PSTR("%s, "), svalue);
        jsflg = 1;
        snprintf_P(stemp1, sizeof(stemp1), modules[i].name);
        snprintf_P(svalue, sizeof(svalue), PSTR("%s%s (%d)"), svalue, stemp1, i +1);
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s\"}"), svalue);
      mqtt_publish(stopic, svalue);
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Modules2\":\""), svalue);
      jsflg = 0;
      for (byte i = 11; i < MAXMODULE; i++) {
        if (jsflg) snprintf_P(svalue, sizeof(svalue), PSTR("%s, "), svalue);
        jsflg = 1;
        snprintf_P(stemp1, sizeof(stemp1), modules[i].name);
        snprintf_P(svalue, sizeof(svalue), PSTR("%s%s (%d)"), svalue, stemp1, i +1);
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s\"}"), svalue);
    }
    else if (!strcmp(type,"GPIO") && (index < MAX_GPIO_PIN)) {
      mytmplt cmodule;
      memcpy_P(&cmodule, &modules[sysCfg.module], sizeof(cmodule));
      if ((data_len > 0) && (cmodule.gp.io[index] == GPIO_USER) && (payload >= GPIO_SENSOR_START) && (payload < GPIO_SENSOR_END)) {
        for (byte i = 0; i < MAX_GPIO_PIN; i++) {
          if ((cmodule.gp.io[i] == GPIO_USER) && (sysCfg.my_module.gp.io[i] == payload)) sysCfg.my_module.gp.io[i] = 0;
        }
        sysCfg.my_module.gp.io[index] = payload;
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{"), svalue);
      byte jsflg = 0;
      for (byte i = 0; i < MAX_GPIO_PIN; i++) {
        if (cmodule.gp.io[i] == GPIO_USER) {
          if (jsflg) snprintf_P(svalue, sizeof(svalue), PSTR("%s, "), svalue);
          jsflg = 1;
          snprintf_P(stemp1, sizeof(stemp1), sensors[sysCfg.my_module.gp.io[i]]);
          snprintf_P(svalue, sizeof(svalue), PSTR("%s\"GPIO%d\":%d (%s)"), svalue, i, sysCfg.my_module.gp.io[i], stemp1);
        }
      }
      if (jsflg) {
        snprintf_P(svalue, sizeof(svalue), PSTR("%s}"), svalue);
      } else {
        snprintf_P(svalue, sizeof(svalue), PSTR("{\"GPIO\":\"Not supported\"}"));
      }
    }
    else if (!strcmp(type,"GPIOS")) {
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"GPIOs\":\""), svalue);
      byte jsflg = 0;
      for (byte i = 0; i < GPIO_SENSOR_END; i++) {
        if (jsflg) snprintf_P(svalue, sizeof(svalue), PSTR("%s, "), svalue);
        jsflg = 1;
        snprintf_P(stemp1, sizeof(stemp1), sensors[i]);
        snprintf_P(svalue, sizeof(svalue), PSTR("%s%s (%d)"), svalue, stemp1, i);
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s\"}"), svalue);
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
        seriallog_level = payload;
        seriallog_timer = 0;
        snprintf_P(svalue, sizeof(svalue), PSTR("{\"SerialLog\":%d}"), sysCfg.syslog_level);
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"SerialLog\":\"%d (Setting %d)\"}"), seriallog_level, sysCfg.seriallog_level);
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
    else if (!strcmp(type,"SSID") && (index > 0) && (index <= 2)) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.sta_ssid[0]))) {
        strlcpy(sysCfg.sta_ssid[index -1], (payload == 1) ? (index == 1) ? STA_SSID1 : STA_SSID2 : dataBuf, sizeof(sysCfg.sta_ssid[0]));
        sysCfg.sta_active = 0;
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"SSid%d\":\"%s\"}"), index, sysCfg.sta_ssid[index -1]);
    }
    else if (!strcmp(type,"PASSWORD") && (index > 0) && (index <= 2)) {
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
      if ((data_len > 0) && (payload >= WIFI_RESTART) && (payload < MAX_WIFI_OPTION)) {
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
    else if (!strcmp(type,"FRIENDLYNAME") && (index > 0) && (index <= 4)) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.friendlyname[0]))) {
        if (index == 1) 
          strlcpy(sysCfg.friendlyname[0], (payload == 1) ? FRIENDLY_NAME1 : dataBuf, sizeof(sysCfg.friendlyname[0]));
        else if (index == 2) 
          strlcpy(sysCfg.friendlyname[1], (payload == 1) ? FRIENDLY_NAME2 : dataBuf, sizeof(sysCfg.friendlyname[1]));
        else if (index == 3) 
          strlcpy(sysCfg.friendlyname[2], (payload == 1) ? FRIENDLY_NAME3 : dataBuf, sizeof(sysCfg.friendlyname[2]));
        else if (index == 4) 
          strlcpy(sysCfg.friendlyname[3], (payload == 1) ? FRIENDLY_NAME4 : dataBuf, sizeof(sysCfg.friendlyname[3]));
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"FriendlyName%d\":\"%s\"}"), index, sysCfg.friendlyname[index -1]);
    }
    else if (swt_flg && !strcmp(type,"SWITCHMODE")) {
      if ((data_len > 0) && (payload >= 0) && (payload < MAX_SWITCH_OPTION)) {
        sysCfg.switchmode = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"SwitchMode\":%d}"), sysCfg.switchmode);
    }
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
    else if (!strcmp(type,"UNITS")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 1)) {
        sysCfg.value_units = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Units\":\"%s\"}"), (sysCfg.value_units) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
    }
    else if (!strcmp(type,"MQTT")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 1)) {
        sysCfg.mqtt_enabled = payload;
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Mqtt\":\"%s\"}"), (sysCfg.mqtt_enabled) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
    }
    else if (!strcmp(type,"TELEPERIOD")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.tele_period = (payload == 1) ? TELE_PERIOD : payload;
        if ((sysCfg.tele_period > 0) && (sysCfg.tele_period < 10)) sysCfg.tele_period = 10;   // Do not allow periods < 10 seconds
        tele_period = sysCfg.tele_period;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"TelePeriod\":\"%d%s\"}"), sysCfg.tele_period, (sysCfg.value_units) ? " Sec" : "");
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
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Timezone\":\"%d%s\"}"), sysCfg.timezone, (sysCfg.value_units) ? " Hr" : "");
    }
    else if (!strcmp(type,"LEDPOWER")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 2)) {
        sysCfg.ledstate &= 8;
        switch (payload) {
        case 0: // Off
        case 1: // On
          sysCfg.ledstate = payload << 3;
          break;
        case 2: // Toggle
          sysCfg.ledstate ^= 8;
          break;
        }
        uint8_t led = (sysCfg.ledstate >> 3) &1;
        digitalWrite(pin[GPIO_LED1], (led_inverted[0]) ? !led : led);
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"LedPower\":\"%s\"}"), (sysCfg.ledstate &8) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
    }
    else if (!strcmp(type,"LEDSTATE")) {
      if ((data_len > 0) && (payload >= 0) && (payload < MAX_LED_OPTION)) {
        sysCfg.ledstate = payload;
        if (!sysCfg.ledstate) digitalWrite(pin[GPIO_LED1], led_inverted[0]);
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"LedState\":%d}"), sysCfg.ledstate);
    }
    else if (!strcmp(type,"CFGDUMP")) {
      CFG_Dump();
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"CfgDump\":\"Done\"}"));
    }
#ifdef USE_I2C
    else if (i2c_flg && !strcmp(type,"I2CSCAN")) {
      i2c_scan(svalue, sizeof(svalue));
    }
#endif  // USE_I2C

/*** MQTT Commands ***************************************************************************/

    else if (sysCfg.mqtt_enabled && !strcmp(type,"MQTTHOST")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.mqtt_host))) {
        strlcpy(sysCfg.mqtt_host, (payload == 1) ? MQTT_HOST : dataBuf, sizeof(sysCfg.mqtt_host));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"MqttHost\",\"%s\"}"), sysCfg.mqtt_host);
    }
    else if (sysCfg.mqtt_enabled && !strcmp(type,"MQTTPORT")) {
      if ((data_len > 0) && (payload > 0) && (payload < 32766)) {
        sysCfg.mqtt_port = (payload == 1) ? MQTT_PORT : payload;
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"MqttPort\":%d}"), sysCfg.mqtt_port);
    }
#ifdef USE_MQTT_TLS
    else if (sysCfg.mqtt_enabled && !strcmp(type,"MQTTFINGERPRINT")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.mqtt_fingerprint))) {
        strlcpy(sysCfg.mqtt_fingerprint, (payload == 1) ? MQTT_FINGERPRINT : dataBuf, sizeof(sysCfg.mqtt_fingerprint));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"MqttFingerprint\":\"%s\"}"), sysCfg.mqtt_fingerprint);
    }
#endif
    else if (sysCfg.mqtt_enabled && !grpflg && !strcmp(type,"MQTTCLIENT")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.mqtt_client))) {
        strlcpy(sysCfg.mqtt_client, (payload == 1) ? MQTT_CLIENT_ID : dataBuf, sizeof(sysCfg.mqtt_client));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"MqttClient\":\"%s\"}"), sysCfg.mqtt_client);
    }
    else if (sysCfg.mqtt_enabled && !strcmp(type,"MQTTUSER")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.mqtt_user))) {
        strlcpy(sysCfg.mqtt_user, (payload == 1) ? MQTT_USER : dataBuf, sizeof(sysCfg.mqtt_user));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("[\"MqttUser\":\"%s\"}"), sysCfg.mqtt_user);
    }
    else if (sysCfg.mqtt_enabled && !strcmp(type,"MQTTPASSWORD")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.mqtt_pwd))) {
        strlcpy(sysCfg.mqtt_pwd, (payload == 1) ? MQTT_PASS : dataBuf, sizeof(sysCfg.mqtt_pwd));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"MqttPassword\":\"%s\"}"), sysCfg.mqtt_pwd);
    }
#ifdef USE_DOMOTICZ
    else if (sysCfg.mqtt_enabled && !strcmp(type,"DOMOTICZINTOPIC")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.domoticz_in_topic))) {
        strlcpy(sysCfg.domoticz_in_topic, (payload == 1) ? DOMOTICZ_IN_TOPIC : dataBuf, sizeof(sysCfg.domoticz_in_topic));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"DomoticzInTopic\":\"%s\"}"), sysCfg.domoticz_in_topic);
    }
    else if (sysCfg.mqtt_enabled && !strcmp(type,"DOMOTICZOUTTOPIC")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.domoticz_out_topic))) {
        strlcpy(sysCfg.domoticz_out_topic, (payload == 1) ? DOMOTICZ_OUT_TOPIC : dataBuf, sizeof(sysCfg.domoticz_out_topic));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"DomoticzOutTopic\":\"%s\"}"), sysCfg.domoticz_out_topic);
    }
    else if (sysCfg.mqtt_enabled && !strcmp(type,"DOMOTICZIDX") && (index > 0) && (index <= Maxdevice)) {
      if ((data_len > 0) && (payload >= 0)) {
        sysCfg.domoticz_relay_idx[index -1] = payload;
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"DomoticzIdx%d\":%d}"), index, sysCfg.domoticz_relay_idx[index -1]);
    }
    else if (sysCfg.mqtt_enabled && !strcmp(type,"DOMOTICZKEYIDX") && (index > 0) && (index <= Maxdevice)) {
      if ((data_len > 0) && (payload >= 0)) {
        sysCfg.domoticz_key_idx[index -1] = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"DomoticzKeyIdx%d\":%d}"), index, sysCfg.domoticz_key_idx[index -1]);
    }
    else if (sysCfg.mqtt_enabled && !strcmp(type,"DOMOTICZUPDATETIMER")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.domoticz_update_timer = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"DomoticzUpdateTimer\":%d}"), sysCfg.domoticz_update_timer);
    }
#endif  // USE_DOMOTICZ
    else if (sysCfg.mqtt_enabled && !strcmp(type,"GROUPTOPIC")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.mqtt_grptopic))) {
        for(i = 0; i <= data_len; i++)
          if ((dataBuf[i] == '/') || (dataBuf[i] == '+') || (dataBuf[i] == '#')) dataBuf[i] = '_';
        if (!strcmp(dataBuf, MQTTClient)) payload = 1;
        strlcpy(sysCfg.mqtt_grptopic, (payload == 1) ? MQTT_GRPTOPIC : dataBuf, sizeof(sysCfg.mqtt_grptopic));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"GroupTopic\":\"%s\"}"), sysCfg.mqtt_grptopic);
    }
    else if (sysCfg.mqtt_enabled && !grpflg && !strcmp(type,"TOPIC")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.mqtt_topic))) {
        for(i = 0; i <= data_len; i++)
          if ((dataBuf[i] == '/') || (dataBuf[i] == '+') || (dataBuf[i] == '#')) dataBuf[i] = '_';
        if (!strcmp(dataBuf, MQTTClient)) payload = 1;
        strlcpy(sysCfg.mqtt_topic, (payload == 1) ? MQTT_TOPIC : dataBuf, sizeof(sysCfg.mqtt_topic));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Topic\":\"%s\"}"), sysCfg.mqtt_topic);
    }
    else if (sysCfg.mqtt_enabled && !grpflg && !strcmp(type,"BUTTONTOPIC")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.button_topic))) {
        for(i = 0; i <= data_len; i++)
          if ((dataBuf[i] == '/') || (dataBuf[i] == '+') || (dataBuf[i] == '#')) dataBuf[i] = '_';
        if (!strcmp(dataBuf, MQTTClient)) payload = 1;
        strlcpy(sysCfg.button_topic, (payload == 1) ? sysCfg.mqtt_topic : dataBuf, sizeof(sysCfg.button_topic));
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"ButtonTopic\":\"%s\"}"), sysCfg.button_topic);
    }
    else if (sysCfg.mqtt_enabled && !grpflg && !strcmp(type,"SWITCHTOPIC")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.switch_topic[index -1]))) {
        for(i = 0; i <= data_len; i++)
          if ((dataBuf[i] == '/') || (dataBuf[i] == '+') || (dataBuf[i] == '#')) dataBuf[i] = '_';
        if (!strcmp(dataBuf, MQTTClient)) payload = 1;
        strlcpy(sysCfg.switch_topic, (payload == 1) ? sysCfg.mqtt_topic : dataBuf, sizeof(sysCfg.switch_topic));
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"SwitchTopic\":\"%s\"}"), sysCfg.switch_topic);
    }
    else if (sysCfg.mqtt_enabled && !strcmp(type,"BUTTONRETAIN")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 1)) {
        strlcpy(sysCfg.button_topic, sysCfg.mqtt_topic, sizeof(sysCfg.button_topic));
        if (!payload) {
          for(i = 1; i <= Maxdevice; i++) {
            send_button_power(0, i, 3);  // Clear MQTT retain in broker
          }
        }
        sysCfg.mqtt_button_retain = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"ButtonRetain\":\"%s\"}"), (sysCfg.mqtt_button_retain) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
    }
    else if (sysCfg.mqtt_enabled && !strcmp(type,"SWITCHRETAIN")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 1)) {
//        strlcpy(sysCfg.button_topic, sysCfg.mqtt_topic, sizeof(sysCfg.button_topic));
        if (!payload) {
          for(i = 1; i <= 4; i++) {
            send_button_power(1, i, 3);  // Clear MQTT retain in broker
          }
        }
        sysCfg.mqtt_switch_retain = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"SwitchRetain\":\"%s\"}"), (sysCfg.mqtt_switch_retain) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
    }
    else if (sysCfg.mqtt_enabled && (!strcmp(type,"POWERRETAIN") || !strcmp(type,"LIGHTRETAIN"))) {
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

/*** HLW Power management Commands ***********************************************************/

    else if (hlw_flg && !strcmp(type,"POWERLOW")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.hlw_pmin = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"PowerLow\":\"%d%s\"}"), sysCfg.hlw_pmin, (sysCfg.value_units) ? " W" : "");
    }
    else if (hlw_flg && !strcmp(type,"POWERHIGH")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.hlw_pmax = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"PowerHigh\":\"%d%s\"}"), sysCfg.hlw_pmax, (sysCfg.value_units) ? " W" : "");
    }
    else if (hlw_flg && !strcmp(type,"VOLTAGELOW")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 501)) {
        sysCfg.hlw_umin = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"VoltageLow\":\"%d%s\"}"), sysCfg.hlw_umin, (sysCfg.value_units) ? " V" : "");
    }
    else if (hlw_flg && !strcmp(type,"VOLTAGEHIGH")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 501)) {
        sysCfg.hlw_umax = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("[\"VoltageHigh\":\"%d%s\"}"), sysCfg.hlw_umax, (sysCfg.value_units) ? " V" : "");
    }
    else if (hlw_flg && !strcmp(type,"CURRENTLOW")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 16001)) {
        sysCfg.hlw_imin = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"CurrentLow\":\"%d%s\"}"), sysCfg.hlw_imin, (sysCfg.value_units) ? " mA" : "");
    }
    else if (hlw_flg && !strcmp(type,"CURRENTHIGH")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 16001)) {
        sysCfg.hlw_imax = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"CurrentHigh\":\"%d%s\"}"), sysCfg.hlw_imax, (sysCfg.value_units) ? " mA" : "");
    }
    else if (hlw_flg && !strcmp(type,"HLWPCAL")) {
      if ((data_len > 0) && (payload > 0) && (payload < 32001)) {
        sysCfg.hlw_pcal = (payload == 1) ? HLW_PREF_PULSE : payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("(\"HlwPcal\":\"%d%s\"}"), sysCfg.hlw_pcal, (sysCfg.value_units) ? " uS" : "");
    }
    else if (hlw_flg && !strcmp(type,"HLWUCAL")) {
      if ((data_len > 0) && (payload > 0) && (payload < 32001)) {
        sysCfg.hlw_ucal = (payload == 1) ? HLW_UREF_PULSE : payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"HlwUcal\":\"%d%s\"}"), sysCfg.hlw_ucal, (sysCfg.value_units) ? " uS" : "");
    }
    else if (hlw_flg && !strcmp(type,"HLWICAL")) {
      if ((data_len > 0) && (payload > 0) && (payload < 32001)) {
        sysCfg.hlw_ical = (payload == 1) ? HLW_IREF_PULSE : payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"HlwIcal\":\"%d%s\"}"), sysCfg.hlw_ical, (sysCfg.value_units) ? " uS" : "");
    }
#ifdef FEATURE_POWER_LIMIT
    else if (hlw_flg && !strcmp(type,"MAXPOWER")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.hlw_mpl = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"MaxPower\":\"%d%s\"}"), sysCfg.hlw_mpl, (sysCfg.value_units) ? " W" : "");
    }
    else if (hlw_flg && !strcmp(type,"MAXPOWERHOLD")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.hlw_mplh = (payload == 1) ? MAX_POWER_HOLD : payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"MaxPowerHold\":\"%d%s\"}"), sysCfg.hlw_mplh, (sysCfg.value_units) ? " Sec" : "");
    }
    else if (hlw_flg && !strcmp(type,"MAXPOWERWINDOW")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.hlw_mplw = (payload == 1) ? MAX_POWER_WINDOW : payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"MaxPowerWindow\":\"%d%s\"}"), sysCfg.hlw_mplw, (sysCfg.value_units) ? " Sec" : "");
    }
    else if (hlw_flg && !strcmp(type,"SAFEPOWER")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.hlw_mspl = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"SafePower\":\"%d%s\"}"), sysCfg.hlw_mspl, (sysCfg.value_units) ? " W" : "");
    }
    else if (hlw_flg && !strcmp(type,"SAFEPOWERHOLD")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.hlw_msplh = (payload == 1) ? SAFE_POWER_HOLD : payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"SafePowerHold\":\"%d%s\"}"), sysCfg.hlw_msplh, (sysCfg.value_units) ? " Sec" : "");
    }
    else if (hlw_flg && !strcmp(type,"SAFEPOWERWINDOW"}) {
      if ((data_len > 0) && (payload >= 0) && (payload < 1440)) {
        sysCfg.hlw_msplw = (payload == 1) ? SAFE_POWER_WINDOW : payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"SafePowerWindow\":\"%d%s\"}"), sysCfg.hlw_msplw, (sysCfg.value_units) ? " Min" : "");
    }
    else if (hlw_flg && !strcmp(type,"MAXENERGY")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
        sysCfg.hlw_mkwh = payload;
        hlw_mkwh_state = 3;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"MaxEnergy\":\"%d%s\"}"), sysCfg.hlw_mkwh, (sysCfg.value_units) ? " Wh" : "");
    }
    else if (hlw_flg && !strcmp(type,"MAXENERGYSTART")) {
      if ((data_len > 0) && (payload >= 0) && (payload < 24)) {
        sysCfg.hlw_mkwhs = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"MaxEnergyStart\":\"%d%s\"}"), sysCfg.hlw_mkwhs, (sysCfg.value_units) ? " Hr" : "");
    }
#endif  // FEATURE_POWER_LIMIT

/*** WS2812 Commands *************************************************************************/

#ifdef USE_WS2812
    else if ((pin[GPIO_WS2812] < 99) && !strcmp(type,"PIXELS")) {
      if ((data_len > 0) && (payload > 0) && (payload <= WS2812_MAX_LEDS)) {
        sysCfg.ws_pixels = payload;
        ws2812_pixels();
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Pixels\":%d}"), sysCfg.ws_pixels);
    }

    else if ((pin[GPIO_WS2812] < 99) && !strcmp(type,"COLOR")) {
      if (data_len == 6) {
        ws2812_setColor(dataBufUc);
      }
      ws2812_getColor(svalue, sizeof(svalue));
    }
    else if ((pin[GPIO_WS2812] < 99) && !strcmp(type,"DIMMER")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 100)) {
        sysCfg.ws_dimmer = payload;
        power = 1;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Dimmer\":%d}"), sysCfg.ws_dimmer);
    }
    else if ((pin[GPIO_WS2812] < 99) && !strcmp(type,"LEDTABLE")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 2)) {
        switch (payload) {
        case 0: // Off
        case 1: // On
          sysCfg.ws_ledtable = payload;
          break;
        case 2: // Toggle
          sysCfg.ws_ledtable ^= 1;
          break;
        }
        ws2812_update();
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"LedTable\":\"%s\"}"), (sysCfg.ws_ledtable) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
    }
    else if ((pin[GPIO_WS2812] < 99) && !strcmp(type,"FADE")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 2)) {
        switch (payload) {
        case 0: // Off
        case 1: // On
          sysCfg.ws_fade = payload;
          break;
        case 2: // Toggle
          sysCfg.ws_fade ^= 1;
          break;
        }
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Fade\":\"%s\"}"), (sysCfg.ws_fade) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
    }
    else if ((pin[GPIO_WS2812] < 99) && !strcmp(type,"SPEED")) {  // 0 - none, 1 - slow, 4 - fast
      if ((data_len > 0) && (payload >= 0) && (payload <= 4)) {
        sysCfg.ws_speed = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Speed\":%d}"), sysCfg.ws_speed);
    }
    else if ((pin[GPIO_WS2812] < 99) && !strcmp(type,"SCHEME")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 1)) {
        sysCfg.ws_scheme = payload;
/*
        if (sysCfg.scheme == 1) {
          wakeupDimmer = 0;
          wakeupCntr = 0;
        }
*/        
        power = 1;
//        stripTimerCntr = 0;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Scheme\":%d}"), sysCfg.ws_scheme);
    }
    
    else if ((pin[GPIO_WS2812] < 99) && !strcmp(type,"LED")) {
      do_cmnd_led(index, data);
      return;
    }
#endif  // USE_WS2812

/*********************************************************************************************/

    else {
      type = NULL;
    }
  }
  if (type == NULL) {
    blinks = 201;
    snprintf_P(svalue, sizeof(svalue), PSTR("{\"Commands1\":\"Status, SaveData, SaveSate, Upgrade, Otaurl, Restart, Reset, WifiConfig, Seriallog, Syslog, LogHost, LogPort, SSId1, SSId2, Password1, Password2, AP%s\"}"), (!grpflg) ? ", Hostname, Module, Modules, GPIO, GPIOs" : "");
    mqtt_publish(stopic, svalue);

    if (sysCfg.mqtt_enabled) {
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Commands2\":\"Mqtt, MqttHost, MqttPort, MqttUser, MqttPassword%s, GroupTopic, Units, Timezone, LedState, LedPower, TelePeriod\"}"), (!grpflg) ? ", MqttClient, Topic, ButtonTopic, ButtonRetain, SwitchTopic, SwitchRetain, PowerRetain" : "");
    } else {
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Commands2\":\"Mqtt, Units, Timezone, LedState, LedPower, TelePeriod\"}"), (!grpflg) ? ", MqttClient" : "");
    }
    mqtt_publish(stopic, svalue);

    snprintf_P(svalue, sizeof(svalue), PSTR("{\"Commands3\":\"%s%s, PulseTime, BlinkTime, BlinkCount"), (Maxdevice == 1) ? "Power, Light" : "Power1, Power2, Light1 Light2", (sysCfg.module != MOTOR) ? ", PowerOnState" : "");
#ifdef USE_WEBSERVER
    snprintf_P(svalue, sizeof(svalue), PSTR("%s, Weblog, Webserver"), svalue);
#endif
#ifdef USE_DOMOTICZ
    snprintf_P(svalue, sizeof(svalue), PSTR("%s, DomoticzInTopic, DomoticzOutTopic, DomoticzIdx, DomoticzKeyIdx, DomoticzUpdateTimer"), svalue);
#endif  // USE_DOMOTICZ
    if (swt_flg) snprintf_P(svalue, sizeof(svalue), PSTR("%s, SwitchMode"), svalue);
#ifdef USE_I2C
    if (i2c_flg) snprintf_P(svalue, sizeof(svalue), PSTR("%s, I2CScan"), svalue);
#endif  // USE_I2C
#ifdef USE_WS2812
    if (pin[GPIO_WS2812] < 99) snprintf_P(svalue, sizeof(svalue), PSTR("%s, Pixels, Color, Dimmer, Scheme, Fade, Speed, LedTable"), svalue);
#endif
    snprintf_P(svalue, sizeof(svalue), PSTR("%s\"}"), svalue);

    if (hlw_flg) {
      mqtt_publish(stopic, svalue);
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Commands4\":\"PowerLow, PowerHigh, VoltageLow, VoltageHigh, CurrentLow, CurrentHigh"));
#ifdef FEATURE_POWER_LIMIT
      snprintf_P(svalue, sizeof(svalue), PSTR("%s, SafePower, SafePowerHold, SafePowerWindow, MaxPower, MaxPowerHold, MaxPowerWindow, MaxEnergy, MaxEnergyStart"), svalue);
#endif  // FEATURE_POWER_LIMIT
      snprintf_P(svalue, sizeof(svalue), PSTR("%s\"}"), svalue);
    }
  }
  mqtt_publish(stopic, svalue);
}

/********************************************************************************************/

void send_button_power(byte key, byte device, byte state)
{
// key 0 = button_topic
// key 1 = switch_topic

  char stopic[TOPSZ], svalue[TOPSZ], stemp1[10];

  if (device > Maxdevice) device = 1;
  snprintf_P(stemp1, sizeof(stemp1), PSTR("%d"), device);
  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/%s%s"),
    SUB_PREFIX, (key) ? sysCfg.switch_topic : sysCfg.button_topic, sysCfg.mqtt_subtopic, (Maxdevice > 1) ? stemp1 : "");
  
  if (state == 3) {
    svalue[0] = '\0';
  } else {
    if (!strcmp(sysCfg.mqtt_topic,(key) ? sysCfg.switch_topic : sysCfg.button_topic) && (state == 2)) {
      state = ~(power >> (device -1)) & 0x01;
    }
    snprintf_P(svalue, sizeof(svalue), PSTR("%s"), (state) ? (state == 2) ? MQTT_CMND_TOGGLE : MQTT_STATUS_ON : MQTT_STATUS_OFF);
  }
#ifdef USE_DOMOTICZ
  if (sysCfg.domoticz_key_idx[device -1] && strlen(svalue)) {
    strlcpy(stopic, sysCfg.domoticz_in_topic, sizeof(stopic));
    snprintf_P(svalue, sizeof(svalue), PSTR("{\"command\":\"switchlight\", \"idx\":%d, \"switchcmd\":\"%s\"}"),
      sysCfg.domoticz_key_idx[device -1], (state) ? (state == 2) ? "Toggle" : "On" : "Off");
    mqtt_publish(stopic, svalue);
  } else {
    mqtt_publish_sec(stopic, svalue, (key) ? sysCfg.mqtt_switch_retain : sysCfg.mqtt_button_retain);
  }
#else
  mqtt_publish_sec(stopic, svalue, (key) ? sysCfg.mqtt_switch_retain : sysCfg.mqtt_button_retain);
#endif  // USE_DOMOTICZ
}

void do_cmnd_power(byte device, byte state)
{
// device  = Relay number 1 and up
// state 0 = Relay Off
// state 1 = Relay on (turn off after sysCfg.pulsetime * 100 mSec if enabled)
// state 2 = Toggle relay
// state 3 = Blink relay
// state 4 = Stop blinking relay
// state 9 = Show power state

  if ((device < 1) || (device > Maxdevice)) device = 1;
  byte mask = 0x01 << (device -1);
  pulse_timer = 0;
  if (state <= 2) {
    if ((blink_mask & mask)) {
      blink_mask &= (0xFF ^ mask);  // Clear device mask
      mqtt_publishPowerBlinkState(device);
    }
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
    if (device == 1) pulse_timer = (power & mask) ? sysCfg.pulsetime : 0;
  }
  else if (state == 3) { // Blink
    if (!(blink_mask & mask)) {
      blink_powersave = (blink_powersave & (0xFF ^ mask)) | (power & mask);  // Save state
      blink_power = (power >> (device -1))&1;  // Prep to Toggle
    }
    blink_timer = 1;
    blink_counter = ((!sysCfg.blinkcount) ? 64000 : (sysCfg.blinkcount *2)) +1;
    blink_mask |= mask;  // Set device mask
    mqtt_publishPowerBlinkState(device);
    return;
  }
  else if (state == 4) { // No Blink
    byte flag = (blink_mask & mask);
    blink_mask &= (0xFF ^ mask);  // Clear device mask
    mqtt_publishPowerBlinkState(device);
    if (flag) do_cmnd_power(device, (blink_powersave >> (device -1))&1);  // Restore state
    return;
  }
  mqtt_publishPowerState(device);
}

void stop_all_power_blink()
{
  byte i, mask;

  for (i = 1; i <= Maxdevice; i++) {
    mask = 0x01 << (i -1);
    if (blink_mask & mask) {
      blink_mask &= (0xFF ^ mask);  // Clear device mask
      mqtt_publishPowerBlinkState(i);
      do_cmnd_power(i, (blink_powersave >> (i -1))&1);  // Restore state
    }
  }
}

void do_cmnd(char *cmnd)
{
  char stopic[TOPSZ], svalue[128];
  char *start;
  char *token;

  token = strtok(cmnd, " ");
  if (token != NULL) {
    start = strrchr(token, '/');   // Skip possible cmnd/sonoff/ preamble
    if (start) token = start;
  }
  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/%s"), SUB_PREFIX, sysCfg.mqtt_topic, token);
  token = strtok(NULL, "");
  snprintf_P(svalue, sizeof(svalue), PSTR("%s"), (token == NULL) ? "" : token);
  mqttDataCb(stopic, (byte*)svalue, strlen(svalue));
}

void publish_status(uint8_t payload)
{
  char stopic[TOPSZ], svalue[MESSZ], stemp1[TOPSZ], stemp2[10], stemp3[10];
  float ped, pi, pc;
  uint16_t pe, pw, pu;

  // Workaround MQTT - TCP/IP stack queueing when SUB_PREFIX = PUB_PREFIX
  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/RESULT"),
    (!strcmp(SUB_PREFIX,PUB_PREFIX) && (!payload)) ? PUB_PREFIX2 : PUB_PREFIX, sysCfg.mqtt_topic);

  if ((!sysCfg.mqtt_enabled) && (payload == 6)) payload = 99;

  if ((payload == 0) || (payload == 99)) {
    snprintf_P(svalue, sizeof(svalue), PSTR("{\"Status\":{\"Module\":%d, \"FriendlyName\":\"%s\", \"Topic\":\"%s\", \"ButtonTopic\":\"%s\", \"Subtopic\":\"%s\", \"Power\":%d, \"PowerOnState\":%d, \"LedState\":%d, \"SaveData\":%d, \"SaveState\":%d, \"ButtonRetain\":%d, \"PowerRetain\":%d}}"),
      sysCfg.module +1, sysCfg.friendlyname[0], sysCfg.mqtt_topic, sysCfg.button_topic, sysCfg.mqtt_subtopic, power, sysCfg.poweronstate, sysCfg.ledstate, sysCfg.savedata, sysCfg.savestate, sysCfg.mqtt_button_retain, sysCfg.mqtt_power_retain);
    if (payload == 0) mqtt_publish(stopic, svalue);
  }

  if (hlw_flg) {
    if ((payload == 0) || (payload == 8)) {
      hlw_readEnergy(0, ped, pe, pw, pu, pi, pc);
      dtostrf(pi, 1, 3, stemp1);
      dtostrf(ped, 1, 3, stemp2);
      dtostrf(pc, 1, 2, stemp3);
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"StatusPWR\":{\"Voltage\":%d, \"Current\":\"%s\", \"Power\":%d, \"Today\":\"%s\", \"Factor\":\"%s\"}}"),
        pu, stemp1, pw, stemp2, stemp3);
      if (payload == 0) mqtt_publish(stopic, svalue);
    }

    if ((payload == 0) || (payload == 9)) {
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"StatusPWRThreshold\":{\"PowerLow\":%d, \"PowerHigh\":%d, \"VoltageLow\":%d, \"VoltageHigh\":%d, \"CurrentLow\":%d, \"CurrentHigh\":%d}}"),
        sysCfg.hlw_pmin, sysCfg.hlw_pmax, sysCfg.hlw_umin, sysCfg.hlw_umax, sysCfg.hlw_imin, sysCfg.hlw_imax);
      if (payload == 0) mqtt_publish(stopic, svalue);
    }
  }

  if ((payload == 0) || (payload == 1)) {
    snprintf_P(svalue, sizeof(svalue), PSTR("{\"StatusPRM\":{\"Baudrate\":%d, \"GroupTopic\":\"%s\", \"OtaUrl\":\"%s\", \"Uptime\":%d, \"BootCount\":%d, \"SaveCount\":%d}}"),
      Baudrate, sysCfg.mqtt_grptopic, sysCfg.otaUrl, uptime, sysCfg.bootcount, sysCfg.saveFlag);
    if (payload == 0) mqtt_publish(stopic, svalue);
  }

  if ((payload == 0) || (payload == 2)) {
    snprintf_P(svalue, sizeof(svalue), PSTR("{\"StatusFWR\":{\"Program\":\"%s\", \"Boot\":%d, \"SDK\":\"%s\"}}"),
      Version, ESP.getBootVersion(), ESP.getSdkVersion());
    if (payload == 0) mqtt_publish(stopic, svalue);
  }

  if ((payload == 0) || (payload == 3)) {
    snprintf_P(svalue, sizeof(svalue), PSTR("{\"StatusLOG\":{\"Seriallog\":%d, \"Weblog\":%d, \"Syslog\":%d, \"LogHost\":\"%s\", \"SSId1\":\"%s\", \"SSId2\":\"%s\", \"TelePeriod\":%d}}"),
      sysCfg.seriallog_level, sysCfg.weblog_level, sysCfg.syslog_level, sysCfg.syslog_host, sysCfg.sta_ssid[0], sysCfg.sta_ssid[1], sysCfg.tele_period);
    if (payload == 0) mqtt_publish(stopic, svalue);
  }

  if ((payload == 0) || (payload == 4)) {
    snprintf_P(svalue, sizeof(svalue), PSTR("{\"StatusMEM\":{\"ProgramSize\":%d, \"Free\":%d, \"Heap\":%d, \"SpiffsStart\":%d, \"SpiffsSize\":%d, \"FlashSize\":%d, \"ProgramFlashSize\":%d}}"),
      ESP.getSketchSize()/1024, ESP.getFreeSketchSpace()/1024, ESP.getFreeHeap()/1024, ((uint32_t)&_SPIFFS_start - 0x40200000)/1024,
      (((uint32_t)&_SPIFFS_end - 0x40200000) - ((uint32_t)&_SPIFFS_start - 0x40200000))/1024, ESP.getFlashChipRealSize()/1024, ESP.getFlashChipSize()/1024);
    if (payload == 0) mqtt_publish(stopic, svalue);
  }

  if ((payload == 0) || (payload == 5)) {
    snprintf_P(svalue, sizeof(svalue), PSTR("{\"StatusNET\":{\"Host\":\"%s\", \"IP\":\"%s\", \"Gateway\":\"%s\", \"Subnetmask\":\"%s\", \"Mac\":\"%s\", \"Webserver\":%d, \"WifiConfig\":%d}}"),
      Hostname, WiFi.localIP().toString().c_str(), WiFi.gatewayIP().toString().c_str(), WiFi.subnetMask().toString().c_str(),
      WiFi.macAddress().c_str(), sysCfg.webserver, sysCfg.sta_config);
    if (payload == 0) mqtt_publish(stopic, svalue);
  }

  if (((payload == 0) || (payload == 6)) && sysCfg.mqtt_enabled) {
    snprintf_P(svalue, sizeof(svalue), PSTR("{\"StatusMQT\":{\"Host\":\"%s\", \"Port\":%d, \"ClientMask\":\"%s\", \"Client\":\"%s\", \"User\":\"%s\", \"MAX_PACKET_SIZE\":%d, \"KEEPALIVE\":%d}}"),
      sysCfg.mqtt_host, sysCfg.mqtt_port, sysCfg.mqtt_client, MQTTClient, sysCfg.mqtt_user, MQTT_MAX_PACKET_SIZE, MQTT_KEEPALIVE);
    if (payload == 0) mqtt_publish(stopic, svalue);
  }

  if ((payload == 0) || (payload == 7)) {
    snprintf_P(svalue, sizeof(svalue), PSTR("{\"StatusTIM\":{\"UTC\":\"%s\", \"Local\":\"%s\", \"StartDST\":\"%s\", \"EndDST\":\"%s\", \"Timezone\":%d}}"),
      rtc_time(0).c_str(), rtc_time(1).c_str(), rtc_time(2).c_str(), rtc_time(3).c_str(), sysCfg.timezone);
  }
  
  mqtt_publish(stopic, svalue);
}

/********************************************************************************************/

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
  char log[LOGSZ], stopic[TOPSZ], svalue[MESSZ], stime[21];

  snprintf_P(stime, sizeof(stime), PSTR("%04d-%02d-%02dT%02d:%02d:%02d"),
    rtcTime.Year, rtcTime.Month, rtcTime.Day, rtcTime.Hour, rtcTime.Minute, rtcTime.Second);

  if (pulse_timer > 111) pulse_timer--;

  if (seriallog_timer) {
    seriallog_timer--;
    if (!seriallog_timer) {
      if (seriallog_level) {
        addLog_P(LOG_LEVEL_INFO, PSTR("APP: Serial logging disabled"));
      }
      seriallog_level = 0;
    }
  }

  if (syslog_timer) {  // Restore syslog level
    syslog_timer--;
    if (!syslog_timer) {
      syslog_level = sysCfg.syslog_level;
      if (sysCfg.syslog_level) {
        addLog_P(LOG_LEVEL_INFO, PSTR("SYSL: Syslog logging re-enabled"));  // Might trigger disable again (on purpose)
      }
    }
  }

#ifdef USE_DOMOTICZ
  if ((sysCfg.domoticz_update_timer || domoticz_update_timer) && sysCfg.domoticz_relay_idx[0]) {
    domoticz_update_timer--;
    if (domoticz_update_timer <= 0) {
      domoticz_update_timer = sysCfg.domoticz_update_timer;
      for (byte i = 1; i <= Maxdevice; i++) mqtt_publishDomoticzPowerState(i);
    }
  }
#endif  // USE_DOMOTICZ

  if (status_update_timer) {
    status_update_timer--;
    if (!status_update_timer) {
      for (byte i = 1; i <= Maxdevice; i++) mqtt_publishPowerState(i);
    }
  }

  if (sysCfg.tele_period) {
    tele_period++;
    if (tele_period == sysCfg.tele_period -1) {
      if (pin[GPIO_DSB]) {
#ifdef USE_DS18B20
        dsb_readTempPrep();
#endif  // USE_DS18B20
#ifdef USE_DS18x20
        ds18x20_search();      // Check for changes in sensors number
        ds18x20_convert();     // Start Conversion, takes up to one second
#endif  // USE_DS18x20
      }
#ifdef USE_DHT
      if (dht_type) dht_readPrep();
#endif  // USE_DHT
#ifdef USE_I2C
      if (i2c_flg) {
#ifdef USE_HTU
        htu_detect();
#endif  // USE_HTU
#ifdef USE_BMP
        bmp_detect();
#endif  // USE_BMP
#ifdef USE_BH1750
        bh1750_detect();
#endif  // USE_BH1750
      }
#endif  // USE_I2C
    }
    if (tele_period >= sysCfg.tele_period) {
      tele_period = 0;

      snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/TELEMETRY"), PUB_PREFIX2, sysCfg.mqtt_topic);
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Time\":\"%s\", \"Uptime\":%d"), stime, uptime);
      for (byte i = 0; i < Maxdevice; i++) {
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

      uint8_t djson = 0;
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"Time\":\"%s\""), stime);
      if (pin[GPIO_DSB] < 99) {
#ifdef USE_DS18B20
        dsb_mqttPresent(svalue, sizeof(svalue), &djson);
#endif  // USE_DS18B20
#ifdef USE_DS18x20
        ds18x20_mqttPresent(svalue, sizeof(svalue), &djson);
#endif  // USE_DS18x20
      }
#if defined(USE_DHT) || defined(USE_DHT2)
      if (dht_type) dht_mqttPresent(svalue, sizeof(svalue), &djson);
#endif  // USE_DHT/2
#ifdef USE_I2C
      if (i2c_flg) {
#ifdef USE_HTU
        htu_mqttPresent(svalue, sizeof(svalue), &djson);
#endif  // USE_HTU
#ifdef USE_BMP
        bmp_mqttPresent(svalue, sizeof(svalue), &djson);
#endif  // USE_BMP
#ifdef USE_BH1750
        bh1750_mqttPresent(svalue, sizeof(svalue), &djson);
#endif  // USE_BH1750
      }
#endif  // USE_I2C      
      if (djson) {
        snprintf_P(svalue, sizeof(svalue), PSTR("%s}"), svalue);
        mqtt_publish(stopic, svalue);
      }

      if (hlw_flg) hlw_mqttPresent();
    }
  }

  if (hlw_flg) hlw_margin_chk();

  if ((rtcTime.Minute == 2) && (rtcTime.Second == 30)) {
    uptime++;
    snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/TELEMETRY"), PUB_PREFIX2, sysCfg.mqtt_topic);
    snprintf_P(svalue, sizeof(svalue), PSTR("{\"Time\":\"%s\", \"Uptime\":%d}"), stime, uptime);
    mqtt_publish(stopic, svalue);
  }
}

void stateloop()
{
  uint8_t button, flag, switchflag, power_now;
  char scmnd[20], log[LOGSZ], stopic[TOPSZ], svalue[MESSZ];

  timerxs = millis() + (1000 / STATES);
  state++;
  if (state == STATES) {             // Every second
    state = 0;
    every_second();
  }

  if (mqtt_cmnd_publish) mqtt_cmnd_publish--;  // Clean up

  if ((pulse_timer > 0) && (pulse_timer < 112)) {
    pulse_timer--;
    if (!pulse_timer) do_cmnd_power(1, 0);
  }

  if (blink_mask) {
    blink_timer--;
    if (!blink_timer) {
      blink_timer = sysCfg.blinktime;
      blink_counter--;
      if (!blink_counter) {
        stop_all_power_blink();
      } else {
        blink_power ^= 1;
        power_now = (power & (0xFF ^ blink_mask)) | ((blink_power) ? blink_mask : 0);
        setRelay(power_now);
      }
    }
  }

#ifdef USE_WS2812
  if (pin[GPIO_WS2812] < 99) ws2812_animate();
#endif  // USE_WS2812

  if ((sysCfg.module == SONOFF_DUAL) || (sysCfg.module == CH4)) {
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
    button = digitalRead(pin[GPIO_KEY1]);
  }
  if ((button == PRESSED) && (lastbutton[0] == NOT_PRESSED)) {
    multipress = (multiwindow) ? multipress +1 : 1;
    snprintf_P(log, sizeof(log), PSTR("APP: Multipress %d"), multipress);
    addLog(LOG_LEVEL_DEBUG, log);
    blinks = 201;
    multiwindow = STATES /2;         // 1/2 second multi press window
  }
  lastbutton[0] = button;
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
      if ((sysCfg.module == SONOFF_DUAL) || (sysCfg.module == CH4)) {
        flag = ((multipress == 1) || (multipress == 2));
      } else  {
        flag = (multipress == 1);
      }
      if (flag && sysCfg.mqtt_enabled && mqttClient.connected() && strcmp(sysCfg.button_topic, "0")) {
        send_button_power(0, multipress, 2);  // Execute command via MQTT using ButtonTopic to sync external clients
      } else
      {
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

  for (byte i = 1; i < Maxdevice; i++) if (pin[GPIO_KEY1 +i] < 99) {
    button = digitalRead(pin[GPIO_KEY1 +i]);
    if ((button == PRESSED) && (lastbutton[i] == NOT_PRESSED)) {
      if (sysCfg.mqtt_enabled && mqttClient.connected() && strcmp(sysCfg.button_topic, "0")) {
        send_button_power(0, i +1, 2);   // Execute commend via MQTT
      } else {
        do_cmnd_power(i +1, 2);       // Execute command internally
      }
    }
    lastbutton[i] = button;
  }

  for (byte i = 0; i < Maxdevice; i++) if (pin[GPIO_SWT1 +i] < 99) {
    button = digitalRead(pin[GPIO_SWT1 +i]);
    if (button != lastwallswitch[i]) {
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
        if ((button == PRESSED) && (lastwallswitch[i] == NOT_PRESSED)) switchflag = 2;  // Toggle with pushbutton to Gnd
        break;
      case PUSHBUTTON_INV:
        if ((button == NOT_PRESSED) && (lastwallswitch[i] == PRESSED)) switchflag = 2;  // Toggle with releasing pushbutton from Gnd
      }
      if (switchflag < 3) {
        if (sysCfg.mqtt_enabled && mqttClient.connected() && strcmp(sysCfg.switch_topic,"0")) {
          send_button_power(1, i +1, switchflag);   // Execute commend via MQTT
        } else {
          do_cmnd_power(i +1, switchflag);       // Execute command internally
        }
      }
      lastwallswitch[i] = button;
    }
  }

  if (!(state % ((STATES/10)*2))) {
    if (blinks || restartflag || otaflag) {
      if (restartflag || otaflag) {
        blinkstate = 1;   // Stay lit
      } else {
        blinkstate ^= 1;  // Blink
      }
      if ((sysCfg.ledstate &0x06) || (blinks > 200) || (blinkstate)) {
        digitalWrite(pin[GPIO_LED1], (led_inverted[0]) ? !blinkstate : blinkstate);
      }
      if (!blinkstate) {
        blinks--;
        if (blinks == 200) blinks = 0;
      }
    } else {
      if (sysCfg.ledstate &0x01) {
        digitalWrite(pin[GPIO_LED1], (led_inverted[0]) ? !power : power);
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
        // Try multiple times to get the update, in case we have a transient issue.
        // e.g. Someone issued "cmnd/sonoffs/update 1" and all the devices
        //      are hammering the OTAURL.
        for (byte i = 0; i < OTA_ATTEMPTS && !otaok; i++) {
          // Delay an increasing pseudo-random time for each interation.
          // Starting at 0 (no delay) up to a maximum of OTA_ATTEMPTS-1 seconds.
          delay((ESP.getChipId() % 1000) * i);
          otaok = (ESPhttpUpdate.update(sysCfg.otaUrl) == HTTP_UPDATE_OK);
        }
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
        if (sysCfg.savestate) {
          if (!((sysCfg.pulsetime > 0) && (sysCfg.pulsetime < 30) && ((sysCfg.power &0xFE) == (power &0xFE)))) sysCfg.power = power;
        }
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
      if (hlw_flg) hlw_savestate();
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
    if (WiFi.status() == WL_CONNECTED) {
      if (sysCfg.mqtt_enabled) {
        if (!mqttClient.connected()) {
          if (!mqttcounter) {
            mqtt_reconnect();
          } else {
            mqttcounter--;
          }
        }
      } else {
        if (!mqttcounter) {
          mqtt_reconnect();
        }
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
      if (seriallog_level < LOG_LEVEL_INFO) seriallog_level = LOG_LEVEL_INFO;
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

void GPIO_init()
{
  char log[LOGSZ];
  uint8_t mpin;
  mytmplt def_module;

  if (!sysCfg.module) sysCfg.module = SONOFF_BASIC;  // Sonoff Basic

  memcpy_P(&def_module, &modules[sysCfg.module], sizeof(def_module));
  strlcpy(my_module.name, def_module.name, sizeof(my_module.name));
  for (byte i = 0; i < MAX_GPIO_PIN; i++) {
    if (sysCfg.my_module.gp.io[i] > GPIO_NONE) my_module.gp.io[i] = sysCfg.my_module.gp.io[i];
    if ((def_module.gp.io[i] > GPIO_NONE) && (def_module.gp.io[i] < GPIO_USER)) my_module.gp.io[i] = def_module.gp.io[i];
  }
  for (byte i = 0; i < GPIO_MAX; i++) pin[i] = 99;
  for (byte i = 0; i < MAX_GPIO_PIN; i++) {
    mpin = my_module.gp.io[i];

//  snprintf_P(log, sizeof(log), PSTR("DBG: gpio pin %d, mpin %d"), i, mpin);
//  addLog(LOG_LEVEL_DEBUG, log);
    
    if (mpin) {
      if ((mpin >= GPIO_REL1_INV) && (mpin <= GPIO_REL4_INV)) {
        rel_inverted[mpin - GPIO_REL1_INV] = 1;
        mpin -= 4;
      }
      else if ((mpin >= GPIO_LED1_INV) && (mpin <= GPIO_LED4_INV)) {
        led_inverted[mpin - GPIO_LED1_INV] = 1;
        mpin -= 4;
      }
      else if (mpin == GPIO_DHT11) dht_type = DHT11;
      else if (mpin == GPIO_DHT21) {
        dht_type = DHT21;
        mpin--;
      }
      else if (mpin == GPIO_DHT22) {
        dht_type = DHT22;
        mpin -= 2;
      }
      pin[mpin] = i;
    }
  }

  Maxdevice = 1;
  switch (sysCfg.module) {
    case SONOFF_DUAL:
    case ELECTRODRAGON:
      Maxdevice = 2;
      break;
    case SONOFF_4CH:
    case CH4:
      Maxdevice = 4;
      break;
  }

  swt_flg = ((pin[GPIO_SWT1] < 99) || (pin[GPIO_SWT2] < 99) || (pin[GPIO_SWT3] < 99) || (pin[GPIO_SWT4] < 99));

  if ((sysCfg.module == SONOFF_DUAL) && (sysCfg.module == CH4)) {
    Baudrate = 19200;
  } else {  
    if (sysCfg.module == SONOFF_LED) {
      for (byte i = 0; i < 5; i++) {
        if (pin[GPIO_PWM0 +i] < 99) pinMode(pin[GPIO_PWM0 +i], OUTPUT);
      }
    } else {
      for (byte i = 0; i < 4; i++) {
        if (pin[GPIO_KEY1 +i] < 99) pinMode(pin[GPIO_KEY1 +i], INPUT_PULLUP);
        if (pin[GPIO_REL1 +i] < 99) pinMode(pin[GPIO_REL1 +i], OUTPUT);
      }
    }
  }
  for (byte i = 0; i < 4; i++) {
    if (pin[GPIO_LED1 +i] < 99) {
      pinMode(pin[GPIO_LED1 +i], OUTPUT);
      digitalWrite(pin[GPIO_LED1 +i], (led_inverted[i]) ? !blinkstate : blinkstate);
    }
    if (pin[GPIO_SWT1 +i] < 99) {
      pinMode(pin[GPIO_SWT1 +i], INPUT_PULLUP);
      lastwallswitch[i] = digitalRead(pin[GPIO_SWT1 +i]);  // set global now so doesn't change the saved power state on first switch check
    }
  }

  hlw_flg = ((pin[GPIO_HLW_SEL] < 99) && (pin[GPIO_HLW_CF1] < 99) && (pin[GPIO_HLW_CF] < 99));
  if (hlw_flg) hlw_init();

#if defined(USE_DHT) || defined(USE_DHT2)
  if (dht_type) dht_init();
#endif  // USE_DHT/2

#ifdef USE_I2C
  i2c_flg = ((pin[GPIO_I2C_SCL] < 99) && (pin[GPIO_I2C_SDA] < 99));
  if (i2c_flg) Wire.begin(pin[GPIO_I2C_SDA],pin[GPIO_I2C_SCL]);
#endif  // USE_I2C

#ifdef USE_WS2812
  if (pin[GPIO_WS2812] < 99) ws2812_init();
#endif  // USE_WS2812
}

void setup()
{
  char log[LOGSZ];
  byte idx;

  Serial.begin(Baudrate);
  delay(10);
  Serial.println();
  seriallog_level = LOG_LEVEL_INFO;  // Allow specific serial messages until config loaded

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

  sysCfg.bootcount++;
  snprintf_P(log, sizeof(log), PSTR("APP: Bootcount %d"), sysCfg.bootcount);
  addLog(LOG_LEVEL_DEBUG, log);
  savedatacounter = sysCfg.savedata;
  seriallog_timer = SERIALLOG_TIMER;
  seriallog_level = sysCfg.seriallog_level;
  syslog_level = sysCfg.syslog_level;

  GPIO_init();

  if (Serial.baudRate() != Baudrate) {
    if (seriallog_level) {
      snprintf_P(log, sizeof(log), PSTR("APP: Change baudrate to %d and Serial logging will be disabled in %d seconds"), Baudrate, seriallog_timer);
      addLog(LOG_LEVEL_INFO, log);
    }
    delay(100);
    Serial.flush();
    Serial.begin(Baudrate);
    delay(10);
    Serial.println();
  }

  if (strstr(sysCfg.hostname, "%")) strlcpy(sysCfg.hostname, DEF_WIFI_HOSTNAME, sizeof(sysCfg.hostname));
  if (!strcmp(sysCfg.hostname, DEF_WIFI_HOSTNAME)) {
    snprintf_P(Hostname, sizeof(Hostname)-1, sysCfg.hostname, sysCfg.mqtt_topic, ESP.getChipId() & 0x1FFF);
  } else {
    snprintf_P(Hostname, sizeof(Hostname)-1, sysCfg.hostname);
  }
  WIFI_Connect(Hostname);

  getClient(MQTTClient, sysCfg.mqtt_client, sizeof(MQTTClient));

  if (sysCfg.module == MOTOR) sysCfg.poweronstate = 1;  // Needs always on else in limbo!
  if (ESP.getResetReason() == "Power on") {
    if (sysCfg.poweronstate == 0) {       // All off
      power = 0;
      setRelay(power);
    }
    else if (sysCfg.poweronstate == 1) {  // All on
      power = ((0x00FF << Maxdevice) >> 8);
      setRelay(power);
    }
    else if (sysCfg.poweronstate == 2) {  // All saved state toggle
      power = (sysCfg.power & ((0x00FF << Maxdevice) >> 8)) ^ 0xFF;
      if (sysCfg.savestate) setRelay(power);
    }
    else if (sysCfg.poweronstate == 3) {  // All saved state
      power = sysCfg.power & ((0x00FF << Maxdevice) >> 8);
      if (sysCfg.savestate) setRelay(power);
    }
  } else {
    power = sysCfg.power & ((0x00FF << Maxdevice) >> 8);
    if (sysCfg.savestate) setRelay(power);
  }
  blink_powersave = power;

  rtc_init(every_second_cb);

  snprintf_P(log, sizeof(log), PSTR("APP: Project %s %s (Topic %s, Fallback %s, GroupTopic %s) Version %s"),
    PROJECT, sysCfg.friendlyname[0], sysCfg.mqtt_topic, MQTTClient, sysCfg.mqtt_grptopic, Version);
  addLog(LOG_LEVEL_INFO, log);
}

void loop()
{
#ifdef USE_WEBSERVER
  pollDnsWeb();
#endif  // USE_WEBSERVER

#if defined(USE_WEMO_EMULATION) || defined(USE_HUE_EMULATION)
  pollUDP();
#endif  // USE_WEMO_EMULATION || USE_HUE_EMULATION

  if (millis() >= timerxs) stateloop();
  if (sysCfg.mqtt_enabled) mqttClient.loop();
  if (Serial.available()) serial();
  yield();
}
