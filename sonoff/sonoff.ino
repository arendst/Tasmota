/*
 * Sonoff and Wkaku by Theo Arends
 * 
 * ==================================================
 * Prerequisites:
 *   Change libraries/PubSubClient/src/PubSubClient.h
 *     #define MQTT_MAX_PACKET_SIZE 256
 *     #define MQTT_KEEPALIVE 120
 *   Select IDE Tools - Flash size: "1M (64K SPIFFS)"
 * ==================================================
 *
 * ESP-12F connections (Wkaku)
 * 3V3                                                     5V
 *                   |-------------------|       |---------|
 *  |                |   -------------   |    |1N4001|  |Relay|
 *  |                | -|          Tx |- |       |---------|
 *  |                | -|          Rx |- |                /
 *  |-------------------| En          |- |---| 1k|------|<  BC547B
 *  |                | -|             |-                  \
 *  |                | -|        IO00 |------|Switch|------|
 *  |                ---| IO12   IO02 |--- LED (ESP-12E/F) |
 *  |---| 1k|---|LED|---| IO13   IO15 |------|10k|---------|
 *  |-------------------| Vcc     Gnd |--------------------|
 *                       -------------                     |
 *                        | | | | | |                     Gnd
*/

#define APP_NAME               "Sonoff switch"
#define VERSION                0x01001800   // 1.0.24

enum log_t   {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE, LOG_LEVEL_ALL};
enum week_t  {Last, First, Second, Third, Fourth}; 
enum dow_t   {Sun=1, Mon, Tue, Wed, Thu, Fri, Sat};
enum month_t {Jan=1, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec};

#include "user_config.h"

/*********************************************************************************************\
 * Enable feature by removing leading // or disable feature by adding leading //
\*********************************************************************************************/

#define USE_SERIAL                          // Enable serial command line (+0.2k code and memory)
#define USE_TICKER                          // Enable interrupts to keep RTC synced during subscription flooding
//#define USE_SPIFFS                          // Switch persistent configuration from flash to spiffs (+24k code, +0.6k mem)

/*********************************************************************************************/

#define STATES                 10           // loops per second
#define MQTT_RETRY_SECS        10           // Seconds to retry MQTT connection

//#define LED_PIN                2            // GPIO 2 = Blue Led (0 = On, 1 = Off) - ESP-12
#define LED_PIN                13           // GPIO 13 = Green Led (0 = On, 1 = Off) - Sonoff
//#define LED_PIN                16           // NodeMCU
#define REL_PIN                12           // GPIO 12 = Red Led and Relay (0 = Off, 1 = On)
#define KEY_PIN                0            // GPIO 00 = Button

#define TOPSZ                  40           // Max number of characters in topic string
#define MESSZ                  200          // Max number of characters in message string (Syntax string)
#define LOGSZ                  128          // Max number of characters in log string

enum butt_t {PRESSED, NOT_PRESSED};
enum wifi_t {WIFI_STATUS, WIFI_SMARTCONFIG, WIFI_MANAGER};

#include <ESP8266WiFi.h>        // MQTT, Ota, WifiManager
#include <ESP8266HTTPClient.h>  // MQTT, Ota
#include <ESP8266httpUpdate.h>  // Ota
#include <ESP8266WebServer.h>   // WifiManager, Webserver
#include <DNSServer.h>          // WifiManager
#include <PubSubClient.h>       // MQTT
#ifdef USE_TICKER
  #include <Ticker.h>           // RTC
#endif
#ifdef USE_SPIFFS
  #include <FS.h>               // Config
#endif

extern "C" uint32_t _SPIFFS_start;

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
} sysCfg;

struct TIME_T {
  uint8_t       Second;
  uint8_t       Minute;
  uint8_t       Hour;
  uint8_t       Wday;   // day of week, sunday is day 1
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

char Version[16];
char Hostname[32];
char MQTTClient[32];
uint8_t mqttcounter = 0;
unsigned long timerxs = 0;
#ifndef USE_TICKER
  unsigned long timersec = 0;
#endif
int state = 0;
int mqttflag = 1;
int otaflag = 0, otaok;
int restartflag = 0;
int wificheckflag = WIFI_STATUS;
int heartbeat = 0;

WiFiClient espClient;
PubSubClient mqttClient(espClient);
WiFiUDP portUDP;   // syslog

int blinks = 1;
uint8_t blinkstate = 1;

uint8_t lastbutton = NOT_PRESSED;
uint8_t holdcount = 0;
uint8_t multiwindow = 0;
uint8_t multipress = 0;

/********************************************************************************************/

void CFG_Default()
{
  addLog_P(LOG_LEVEL_INFO, PSTR("Config: Use default configuration"));
  memset(&sysCfg, 0x00, sizeof(SYSCFG));
  sysCfg.cfg_holder = CFG_HOLDER;
  sysCfg.saveFlag = 0;
  sysCfg.version = VERSION;
  sysCfg.seriallog_level = SERIAL_LOG_LEVEL;
  sysCfg.syslog_level = SYS_LOG_LEVEL;
  strlcpy(sysCfg.syslog_host, SYS_LOG_HOST, sizeof(sysCfg.syslog_host));
  strlcpy(sysCfg.sta_ssid, STA_SSID, sizeof(sysCfg.sta_ssid));
  strlcpy(sysCfg.sta_pwd, STA_PASS, sizeof(sysCfg.sta_pwd));
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
  sysCfg.timezone = APP_TIMEZONE;
  sysCfg.power = APP_POWER;
  sysCfg.ledstate = APP_LEDSTATE;
  sysCfg.webserver = WEB_SERVER;
  CFG_Save();
}

/********************************************************************************************/

void mqtt_publish(const char* topic, const char* data)
{
  char log[TOPSZ+MESSZ];
  
  mqttClient.publish(topic, data);
  snprintf_P(log, sizeof(log), PSTR("MQTT: %s = %s"), strchr(topic,'/')+1, data); // Skip topic prefix
  addLog(LOG_LEVEL_INFO, log);
//  mqttClient.loop();  // Do not use here! Will block previous publishes
  blinks++;
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

  if (mqttflag) {
    snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/INFO"), PUB_PREFIX, sysCfg.mqtt_topic);
    snprintf_P(svalue, sizeof(svalue), PSTR(APP_NAME " version %s, Fallback %s, GroupTopic %s"), Version, MQTTClient, sysCfg.mqtt_grptopic);
    mqtt_publish(stopic, svalue);
    if (MQTT_MAX_PACKET_SIZE < (TOPSZ+MESSZ)) {
      snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/WARNING"), PUB_PREFIX, sysCfg.mqtt_topic);
      snprintf_P(svalue, sizeof(svalue), PSTR("Change MQTT_MAX_PACKET_SIZE in libraries/PubSubClient.h to at least %d"), TOPSZ+MESSZ);
      mqtt_publish(stopic, svalue);
    }
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
  int i, grpflg = 0;
  char *str, *p, *mtopic = NULL, *type = NULL;
  char stopic[TOPSZ], svalue[MESSZ];

  int topic_len = strlen(topic);
  char topicBuf[topic_len +1]; 
  char dataBuf[data_len +1]; 
  char dataBufUc[data_len +1]; 
  
  memcpy(topicBuf, topic, topic_len);
  topicBuf[topic_len] = 0;

  memcpy(dataBuf, data, data_len);
  dataBuf[data_len] = 0;

  snprintf_P(svalue, sizeof(svalue), PSTR("MQTT: Receive topic %s, data %s"), topicBuf, dataBuf);
  addLog(LOG_LEVEL_DEBUG, svalue);

  i = 0;
  for (str = strtok_r(topicBuf, "/", &p); str && i < 3; str = strtok_r(NULL, "/", &p)) {
    switch (i++) {
    case 0:  // cmnd
      break;
    case 1:  // Topic / GroupTopic / DVES_123456
      mtopic = str;
      break;
    case 2:  // Text
      type = str;
    }
  }
  if (!strcmp(mtopic, sysCfg.mqtt_grptopic)) grpflg = 1;
  if (type != NULL) for(i = 0; i < strlen(type); i++) type[i] = toupper(type[i]);

  for(i = 0; i <= data_len; i++) dataBufUc[i] = toupper(dataBuf[i]);

  snprintf_P(svalue, sizeof(svalue), PSTR("MQTT: DataCb Topic %s, Group %d, Type %s, data %s (%s)"),
    mtopic, grpflg, type, dataBuf, dataBufUc);
  addLog(LOG_LEVEL_DEBUG, svalue);

  if (type != NULL) {
    snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/%s"), PUB_PREFIX, sysCfg.mqtt_topic, type);
    strlcpy(svalue, "Error", sizeof(svalue));

    int16_t payload = atoi(dataBuf);
    if (!strcmp(dataBufUc,"OFF") || !strcmp(dataBufUc,"STOP")) payload = 0;
    if (!strcmp(dataBufUc,"ON") || !strcmp(dataBufUc,"START") || !strcmp(dataBufUc,"USER")) payload = 1;
    if (!strcmp(dataBufUc,"TOGGLE") || !strcmp(dataBufUc,"ADMIN")) payload = 2;

    if (!strcmp(type,"STATUS")) {
      if ((data_len == 0) || (payload < 0) || (payload > 7)) payload = 8;
      if ((payload == 0) || (payload == 8)) {
        snprintf_P(svalue, sizeof(svalue), PSTR("%s, %s, %s, %s, %d, %d, %d"),
          Version, sysCfg.mqtt_topic, sysCfg.mqtt_topic2, sysCfg.mqtt_subtopic, sysCfg.power, sysCfg.timezone, sysCfg.ledstate);
        if (payload == 0) mqtt_publish(stopic, svalue);
      }
      if ((payload == 0) || (payload == 1)) {
        snprintf_P(svalue, sizeof(svalue), PSTR("PRM: GroupTopic %s, OtaUrl %s, Heartbeats %d, SaveCount %d"),
          sysCfg.mqtt_grptopic, sysCfg.otaUrl, heartbeat, sysCfg.saveFlag);
        if (payload == 0) mqtt_publish(stopic, svalue);
      }          
      if ((payload == 0) || (payload == 2)) {
        snprintf_P(svalue, sizeof(svalue), PSTR("FWR: Version %s, Boot %d, SDK %s"),
          Version, ESP.getBootVersion(), ESP.getSdkVersion());
        if (payload == 0) mqtt_publish(stopic, svalue);
      }          
      if ((payload == 0) || (payload == 3)) {
        snprintf_P(svalue, sizeof(svalue), PSTR("LOG: Seriallog %d, Syslog %d, LogHost %s, SSId %s, Password %s"),
          sysCfg.seriallog_level, sysCfg.syslog_level, sysCfg.syslog_host, sysCfg.sta_ssid, sysCfg.sta_pwd);
        if (payload == 0) mqtt_publish(stopic, svalue);
      }          
      if ((payload == 0) || (payload == 4)) {
        snprintf_P(svalue, sizeof(svalue), PSTR("MEM: Sketch size %d, Free %d (Heap %d), Spiffs start %d, Flash size %d (%d)"),
          ESP.getSketchSize(), ESP.getFreeSketchSpace(), ESP.getFreeHeap(), (uint32_t)&_SPIFFS_start - 0x40200000,
          ESP.getFlashChipRealSize(), ESP.getFlashChipSize());
        if (payload == 0) mqtt_publish(stopic, svalue);
      }          
      if ((payload == 0) || (payload == 5)) {
        snprintf_P(svalue, sizeof(svalue), PSTR("NET: Host %s, IP %s, Gateway %s, Subnetmask %s, Mac %s"),
          Hostname, WiFi.localIP().toString().c_str(), WiFi.gatewayIP().toString().c_str(), WiFi.subnetMask().toString().c_str(), WiFi.macAddress().c_str());
        if (payload == 0) mqtt_publish(stopic, svalue);
      }          
      if ((payload == 0) || (payload == 6)) {
        snprintf_P(svalue, sizeof(svalue), PSTR("MQT: Host %s, Port %d, Client %s (%s), User %s, Password %s, MAX_PACKET_SIZE %d, KEEPALIVE %d"),
          sysCfg.mqtt_host, sysCfg.mqtt_port, sysCfg.mqtt_client, MQTTClient, sysCfg.mqtt_user, sysCfg.mqtt_pwd, MQTT_MAX_PACKET_SIZE, MQTT_KEEPALIVE); 
        if (payload == 0) mqtt_publish(stopic, svalue);
      }      
      if ((payload == 0) || (payload == 7)) {
        snprintf_P(svalue, sizeof(svalue), PSTR("TIM: UTC %s, Local %s, Start DST %s, End DST %s"),
          rtc_time(0).c_str(), rtc_time(1).c_str(), rtc_time(2).c_str(), rtc_time(3).c_str()); 
      }      
    }
    else if (!grpflg && (!strcmp(type,"UPGRADE") || !strcmp(type,"UPLOAD"))) {
      if ((data_len > 0) && (payload == 1)) {
        otaflag = 3;
        snprintf_P(svalue, sizeof(svalue), PSTR("Upgrade %s from %s"), Version, sysCfg.otaUrl);
      }
      else
        snprintf_P(svalue, sizeof(svalue), PSTR("1 to upgrade"));
    }
    else if (!grpflg && !strcmp(type,"OTAURL")) {
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
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d"), sysCfg.syslog_level);
    }
    else if (!strcmp(type,"LOGHOST")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.syslog_host))) {
        strlcpy(sysCfg.syslog_host, (payload == 1) ? SYS_LOG_HOST : dataBuf, sizeof(sysCfg.syslog_host));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.syslog_host);
    }
    else if (!grpflg && !strcmp(type,"SSID")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.sta_ssid))) {
        strlcpy(sysCfg.sta_ssid, (payload == 1) ? STA_SSID : dataBuf, sizeof(sysCfg.sta_ssid));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.sta_ssid);
    }
    else if (!grpflg && !strcmp(type,"PASSWORD")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.sta_pwd))) {
        strlcpy(sysCfg.sta_pwd, (payload == 1) ? STA_PASS : dataBuf, sizeof(sysCfg.sta_pwd));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.sta_pwd);
    }
    else if (!grpflg && !strcmp(type,"WEBSERVER")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 2)) {
        sysCfg.webserver = payload;
      }
      strlcpy(svalue, (sysCfg.webserver) ? ((sysCfg.webserver == 2) ? "Admin" : "User") : "Off", sizeof(svalue));
      if (sysCfg.webserver)
        snprintf_P(svalue, sizeof(svalue), PSTR("%s on http://%s/ (http://%s/)"), svalue, Hostname, WiFi.localIP().toString().c_str());
    }
    else if (!grpflg && !strcmp(type,"MQTTHOST")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.mqtt_host))) {
        strlcpy(sysCfg.mqtt_host, (payload == 1) ? MQTT_HOST : dataBuf, sizeof(sysCfg.mqtt_host));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.mqtt_host);
    }
    else if (!grpflg && !strcmp(type,"MQTTPORT")) {
      if ((data_len > 0) && (payload > 0) && (payload < 32766)) {
        sysCfg.mqtt_port = (payload == 1) ? MQTT_PORT : payload;
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d"), sysCfg.mqtt_port);
    }
    else if (!grpflg && !strcmp(type,"MQTTCLIENT")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.mqtt_client))) {
        strlcpy(sysCfg.mqtt_client, (payload == 1) ? MQTT_CLIENT_ID : dataBuf, sizeof(sysCfg.mqtt_client));
        snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.mqtt_client);
        snprintf_P(MQTTClient, sizeof(MQTTClient), svalue, ESP.getChipId());
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s (%s)"), sysCfg.mqtt_client, MQTTClient);
    }
    else if (!grpflg && !strcmp(type,"MQTTUSER")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.mqtt_user))) {
        strlcpy(sysCfg.mqtt_user, (payload == 1) ? MQTT_USER : dataBuf, sizeof(sysCfg.mqtt_user));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.mqtt_user);
    }
    else if (!grpflg && !strcmp(type,"MQTTPASSWORD")) {
      if ((data_len > 0) && (data_len < sizeof(sysCfg.mqtt_pwd))) {
        strlcpy(sysCfg.mqtt_pwd, (payload == 1) ? MQTT_PASS : dataBuf, sizeof(sysCfg.mqtt_pwd));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.mqtt_pwd);
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
        strlcpy(sysCfg.mqtt_topic2, (payload == 1) ? MQTT_TOPIC : dataBuf, sizeof(sysCfg.mqtt_topic2));
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.mqtt_topic2);
    }
    else if (!grpflg && !strcmp(type,"SMARTCONFIG")) {
      switch (payload) {
      case 1:
        if (WIFI_State() == WIFI_STATUS) {
          wificheckflag = WIFI_SMARTCONFIG;
          snprintf_P(svalue, sizeof(svalue), PSTR("Smartconfig started"));
        } else
          snprintf_P(svalue, sizeof(svalue), PSTR("Smartconfig or Wifimanager running"));
        break;
      case 2:
        if (WIFI_State() == WIFI_STATUS) {
          wificheckflag = WIFI_MANAGER;
          snprintf_P(svalue, sizeof(svalue), PSTR("Wifimanager started"));
        } else
          snprintf_P(svalue, sizeof(svalue), PSTR("Smartconfig or Wifimanager running"));
        break;
      default:
        snprintf_P(svalue, sizeof(svalue), PSTR("1 to start smartconfig, 2 to start wifimanager"));
      }
    }
    else if (!grpflg && !strcmp(type,"RESTART")) {
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
    else if (!grpflg && !strcmp(type,"RESET")) {
      switch (payload) {
      case 1: 
        restartflag = 11;
        snprintf_P(svalue, sizeof(svalue), PSTR("Reset and Restarting"));
        break;
      case 2:
        restartflag = 12;
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
      snprintf_P(svalue, sizeof(svalue), PSTR("%d"), sysCfg.timezone);
    }
    else if ((!strcmp(type,"LIGHT")) || (!strcmp(type,"POWER"))) {
      snprintf_P(sysCfg.mqtt_subtopic, sizeof(sysCfg.mqtt_subtopic), PSTR("%s"), type);
      if ((data_len > 0) && (payload >= 0) && (payload <= 2)) {
        switch (payload) {
        case 0: // Off
        case 1: // On
          sysCfg.power = payload;
          break;
        case 2: // Toggle
          sysCfg.power ^= 1;
          break;
        }
        digitalWrite(REL_PIN, sysCfg.power);
      }
      strlcpy(svalue, (sysCfg.power) ? "On" : "Off", sizeof(svalue));
    }
    else if (!strcmp(type,"LEDSTATE")) {
      if ((data_len > 0) && (payload >= 0) && (payload <= 1)) {
        sysCfg.ledstate = payload;
      }
      strlcpy(svalue, (sysCfg.ledstate) ? "On" : "Off", sizeof(svalue));
    }
    else {
      type = NULL;
    }
    if (type == NULL) {
      blinks = 1;
      snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/SYNTAX"), PUB_PREFIX, sysCfg.mqtt_topic);
      if (!grpflg) {
        snprintf_P(svalue, sizeof(svalue), PSTR("Status, Upgrade, Otaurl, Restart, Reset, Smartconfig, Seriallog, Syslog, LogHost, SSId, Password, Webserver"));
        mqtt_publish(stopic, svalue);
        snprintf_P(svalue, sizeof(svalue), PSTR("MqttHost, MqttPort, MqttClient, MqttUser, MqttPassword, GroupTopic, Topic, ButtonTopic, Timezone, Light, Power, Ledstate"));
      } else
        snprintf_P(svalue, sizeof(svalue), PSTR("Status, GroupTopic, Timezone, Light, Power, Ledstate"));
    }
    mqtt_publish(stopic, svalue);
  }
}

/********************************************************************************************/

void send_button(char *cmnd)
{
  char stopic[128], svalue[128];
  char *token;

  token = strtok(cmnd, " ");
  if ((!strcmp(token,"light")) || (!strcmp(token,"power"))) strcpy(token, sysCfg.mqtt_subtopic);
  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/%s"), SUB_PREFIX, sysCfg.mqtt_topic2, token);
  token = strtok(NULL, "");
  snprintf_P(svalue, sizeof(svalue), PSTR("%s"), (token == NULL) ? "" : token);
  mqtt_publish(stopic, svalue);
}

void do_cmnd(char *cmnd)
{
  char stopic[128], svalue[128];
  char *token;

  token = strtok(cmnd, " ");
  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/%s"), SUB_PREFIX, sysCfg.mqtt_topic, token);
  token = strtok(NULL, "");
  snprintf_P(svalue, sizeof(svalue), PSTR("%s"), (token == NULL) ? "" : token);
  mqttDataCb(stopic, (byte*)svalue, strlen(svalue));
}

void send_power()
{
  char stopic[TOPSZ], svalue[TOPSZ];

  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/%s"), PUB_PREFIX, sysCfg.mqtt_topic, sysCfg.mqtt_subtopic);
  strlcpy(svalue, (sysCfg.power) ? "On" : "Off", sizeof(svalue));
  mqtt_publish(stopic, svalue);
}

void every_second()
{
  char stopic[TOPSZ], svalue[TOPSZ];

  if ((rtcTime.Minute == 2) && (rtcTime.Second == 30)) { 
    heartbeat++;
    snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/HEARTBEAT"), PUB_PREFIX, sysCfg.mqtt_topic);
    snprintf_P(svalue, sizeof(svalue), PSTR("%d"), heartbeat);
    mqtt_publish(stopic, svalue);
  }
}

const char commands[6][12] PROGMEM = {
  {"reset 1"},        // Hold button for more than 4 seconds
  {"light 2"},        // Press button once
  {"light 2"},        // Press button twice
  {"smartconfig"},    // Press button three times
  {"upgrade 1"},      // Press button four times
  {"restart 1"}};     // Press button five times

void stateloop()
{
  uint8_t button;
  char scmnd[20], log[LOGSZ], stopic[TOPSZ], svalue[TOPSZ];
  
  timerxs = millis() + (1000 / STATES);
  state++;
  if (state == STATES) {             // Every second
    state = 0;
    every_second();
  }

  button = digitalRead(KEY_PIN);
  if ((button == PRESSED) && (lastbutton == NOT_PRESSED)) {
    multipress = (multiwindow) ? multipress +1 : 1;
    snprintf_P(log, sizeof(log), PSTR("APP: Multipress %d"), multipress);
    addLog(LOG_LEVEL_DEBUG, log);
    if (WIFI_State()) restartflag = 1;
    blinks = 1;
    multiwindow = STATES /2;         // 1/2 second multi press window
  }
  lastbutton = button;
  if (button == NOT_PRESSED) {
    holdcount = 0;
  } else {
    holdcount++;
    if (holdcount == (STATES *4)) {  // 4 seconds button hold
      snprintf_P(scmnd, sizeof(scmnd), commands[0]);
      multipress = 0;
      do_cmnd(scmnd);
    }
  }
  if (multiwindow) {
    multiwindow--;
  } else {
    if ((!holdcount) && (multipress >= 1) && (multipress <= 5)) {
      snprintf_P(scmnd, sizeof(scmnd), commands[multipress]);
      if (strcmp(sysCfg.mqtt_topic2,"0") && (multipress == 1) && mqttClient.connected()) {
        send_button(scmnd);          // Execute command via MQTT using ButtonTopic to sync external clients
      } else {
        if (multipress == 3) {       // Execute command smartconfig 1 or 2
          snprintf_P(scmnd, sizeof(scmnd), PSTR("%s %s"), scmnd, (sysCfg.power) ? "2" : "1");
        }
        do_cmnd(scmnd);              // Execute command internally 
      }  
      multipress = 0;
    }
  }

  if (!(state % ((STATES/10)*2))) {
    if (blinks || restartflag || otaflag) {
      if (restartflag || otaflag)
        blinkstate = 0;   // Stay lit
      else
        blinkstate ^= 1;  // Blink
      digitalWrite(LED_PIN, blinkstate);
      if (blinkstate) blinks--;
    } else {
      if (sysCfg.ledstate) digitalWrite(LED_PIN, !sysCfg.power);
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
    CFG_Save();
    if (restartflag) {
      if (restartflag == 11) {
        CFG_Default();
        restartflag = 2;
      }
      if (restartflag == 12) {
        CFG_Erase();
        CFG_Default();
        restartflag = 2;
      }
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
      if (!mqttcounter)
        mqtt_reconnect();
      else
        mqttcounter--;
    }
    break;
  }
}

#ifdef USE_SERIAL
#define INPUT_BUFFER_SIZE          128

byte SerialInByte;
int SerialInByteCounter = 0;
char serialInBuf[INPUT_BUFFER_SIZE + 2];

void serial()
{
  while (Serial.available())
  {
    yield();
    SerialInByte = Serial.read();
    if (SerialInByte > 127) // binary data...
    {
      Serial.flush();
      SerialInByteCounter = 0;
      return;
    }
    if (isprint(SerialInByte))
    {
      if (SerialInByteCounter < INPUT_BUFFER_SIZE) // add char to string if it still fits
        serialInBuf[SerialInByteCounter++] = SerialInByte;
      else
        SerialInByteCounter = 0;
    }
    if (SerialInByte == '\n')
    {
      serialInBuf[SerialInByteCounter] = 0; // serial data completed
      addLog(LOG_LEVEL_NONE, serialInBuf);
      SerialInByteCounter = 0;
      do_cmnd(serialInBuf);
    }
  }
}
#endif

/********************************************************************************************/

void setup()
{
  char log[128];

  Serial.begin(115200);
  delay(10);
  Serial.println();
  sysCfg.seriallog_level = LOG_LEVEL_INFO;  // Allow specific serial messages until config loaded

  if (spiffsNotPresent()) {
    addLog_P(LOG_LEVEL_ERROR, PSTR("APP: ERROR - System Halted"));
    while (true) delay(1);  // Halt system
  }
#ifdef USE_SPIFFS
  if (spiffsCheck()) {
    addLog_P(LOG_LEVEL_ERROR, PSTR("APP: ERROR - System Halted"));
    while (true) delay(1);  // Halt system
  }
#endif

  snprintf_P(Version, sizeof(Version), PSTR("%d.%d.%d"), VERSION >> 24 & 0xff, VERSION >> 16 & 0xff, VERSION >> 8 & 0xff);
  if (VERSION & 0x1f) {
    byte idx = strlen(Version);
    Version[idx] = 96 + (VERSION & 0x1f);
    Version[idx +1] = 0;
  }
  CFG_Load();
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
    
    sysCfg.version = VERSION;
  }

  if (!strcmp(WIFI_HOSTNAME,"%s-%04d"))
    snprintf_P(Hostname, sizeof(Hostname)-1, PSTR(WIFI_HOSTNAME), sysCfg.mqtt_topic, ESP.getChipId() & 0x1FFF);
  else
    snprintf_P(Hostname, sizeof(Hostname)-1, PSTR(WIFI_HOSTNAME));
  WIFI_Connect(Hostname);

  snprintf_P(log, sizeof(log), PSTR("%s"), sysCfg.mqtt_client);
  snprintf_P(MQTTClient, sizeof(MQTTClient), log, ESP.getChipId());
  mqttClient.setServer(sysCfg.mqtt_host, sysCfg.mqtt_port);
  mqttClient.setCallback(mqttDataCb);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, blinkstate);

  pinMode(REL_PIN, OUTPUT);
  digitalWrite(REL_PIN, sysCfg.power);

  pinMode(KEY_PIN, INPUT_PULLUP);

  rtc_init();

  snprintf_P(log, sizeof(log), PSTR("APP: Project %s (Topic %s, Fallback %s, GroupTopic %s) Version %s"),
    PROJECT, sysCfg.mqtt_topic, MQTTClient, sysCfg.mqtt_grptopic, Version);
  addLog(LOG_LEVEL_INFO, log);
}

void loop()
{
#ifndef USE_TICKER
  if (millis() >= timersec) {
    timersec = millis() + 1000;
    rtc_second();
  }
#endif

  pollDnsWeb();

  if (millis() >= timerxs) stateloop();

  mqttClient.loop();

#ifdef USE_SERIAL
  if (Serial.available()) serial();
#endif
  
  yield();
}

