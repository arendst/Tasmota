/*
 * Sonoff and Wkaku by Theo Arends
 * 
 * ---------------------------------------------
 * >>>> Tools - Flash size: 1M (64K SPIFFS) <<<<
 * ---------------------------------------------
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

#define VERSION                0x01001000   // 1.0.16

#define LOG_LEVEL_NONE         0
#define LOG_LEVEL_ERROR        1
#define LOG_LEVEL_INFO         2
#define LOG_LEVEL_DEBUG        3
#define LOG_LEVEL_DEBUG_MORE   4

#include "user_config.h"

#define SERIAL_IO                           // Enable serial command line
#define STATES                 10           // loops per second
#define MQTT_RETRY_SECS        10           // Seconds to retry MQTT connection

//#define LED_PIN                2            // GPIO 2 = Blue Led (0 = On, 1 = Off) - ESP-12
#define LED_PIN                13           // GPIO 13 = Green Led (0 = On, 1 = Off) - Sonoff
//#define LED_PIN                16           // NodeMCU
#define REL_PIN                12           // GPIO 12 = Red Led and Relay (0 = Off, 1 = On)
#define KEY_PIN                0            // GPIO 00 = Button
#define PRESSED                0
#define NOT_PRESSED            1

#define WIFI_STATUS            0
#define WIFI_SMARTCONFIG       1

#define TOPSZ                  40           // Max number of characters in topic string
#define MESSZ                  200          // Max number of characters in message string (Syntax string)
#define LOGSZ                  80           // Max number of characters in log string

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <PubSubClient.h>

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

char Version[16];
char Hostname[32];
uint8_t mqttcounter = 0;
unsigned long timerxs = 0, timersec = 0;
int state = 0;
int otaflag = 0;
int restartflag = 0;
int smartconfigflag = 0;
int heartbeatflag = 0;
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

void mqtt_publish(const char* topic, const char* data)
{
  char log[TOPSZ+MESSZ];
  
  mqttClient.publish(topic, data);
  snprintf_P(log, sizeof(log), PSTR("MQTT: %s = %s"), strchr(topic,'/')+1, data);     // Skip topic prefix
  addLog(LOG_LEVEL_INFO, log);
  mqttClient.loop();  // Solve LmacRxBlk:1 messages
  blinks++;
}

void mqtt_connected()
{
  char stopic[TOPSZ], svalue[TOPSZ];

  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/#"), SUB_PREFIX, sysCfg.mqtt_topic);
  mqttClient.subscribe(stopic);
  mqttClient.loop();  // Solve LmacRxBlk:1 messages
  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/#"), SUB_PREFIX, sysCfg.mqtt_grptopic);
  mqttClient.subscribe(stopic);
  mqttClient.loop();  // Solve LmacRxBlk:1 messages
  snprintf_P(stopic, sizeof(stopic), PSTR("%s/"MQTT_CLIENT_ID"/#"), SUB_PREFIX, ESP.getChipId());  // Fall back topic
  mqttClient.subscribe(stopic);
  mqttClient.loop();  // Solve LmacRxBlk:1 messages

  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/NAME"), PUB_PREFIX, sysCfg.mqtt_topic);
  snprintf_P(svalue, sizeof(svalue), PSTR("Sonoff switch"));
  mqtt_publish(stopic, svalue);
  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/VERSION"), PUB_PREFIX, sysCfg.mqtt_topic);
  snprintf_P(svalue, sizeof(svalue), PSTR("%s"), Version);
  mqtt_publish(stopic, svalue);
  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/FALLBACKTOPIC"), PUB_PREFIX, sysCfg.mqtt_topic);
  snprintf_P(svalue, sizeof(svalue), PSTR(MQTT_CLIENT_ID), ESP.getChipId());
  mqtt_publish(stopic, svalue);
}

void mqtt_reconnect()
{
  char stopic[TOPSZ], svalue[TOPSZ], log[LOGSZ];

  mqttcounter = MQTT_RETRY_SECS;
  addLog(LOG_LEVEL_INFO, "MQTT: Attempting connection");
  snprintf(svalue, sizeof(svalue), MQTT_CLIENT_ID, ESP.getChipId());
  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/lwt"), PUB_PREFIX, sysCfg.mqtt_topic);
  if (mqttClient.connect(svalue, MQTT_USER, MQTT_PASS, stopic, 0, 0, "offline")) {
    addLog(LOG_LEVEL_INFO, "MQTT: Connected");
    mqttcounter = 0;
    mqtt_connected();
  } else {
    snprintf_P(log, sizeof(log), PSTR("MQTT: Connect failed, rc %d. Retry in %d seconds"), mqttClient.state(), mqttcounter);
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

    uint16_t payload = atoi(dataBuf);
    if (!strcmp(dataBufUc,"OFF")) payload = 0;
    if (!strcmp(dataBufUc,"ON")) payload = 1;
    if (!strcmp(dataBufUc,"TOGGLE")) payload = 2;

    if (!strcmp(type,"STATUS")) {
      if ((data_len == 0) || (payload < 0) || (payload > 6)) payload = 7;
      if ((payload == 0) || (payload == 7)) {
        snprintf_P(svalue, sizeof(svalue), PSTR("%s, %s, %s, %s, %d, %d"),
          Version, sysCfg.mqtt_topic, sysCfg.mqtt_topic2, sysCfg.mqtt_subtopic, sysCfg.power, sysCfg.timezone);
        if (payload == 0) mqtt_publish(stopic, svalue);
      }
      if ((payload == 0) || (payload == 1)) {
        snprintf_P(svalue, sizeof(svalue), PSTR("PRM: %s, "MQTT_CLIENT_ID", %s, %s, %d, %d"),
          sysCfg.mqtt_grptopic, ESP.getChipId(), sysCfg.otaUrl, sysCfg.mqtt_host, heartbeat, sysCfg.saveFlag);
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
        IPAddress ip = WiFi.localIP();
        IPAddress gw = WiFi.gatewayIP();
        IPAddress nm = WiFi.subnetMask();
        snprintf_P(svalue, sizeof(svalue), PSTR("NET: Hostname %s, IP %u.%u.%u.%u, Gateway %u.%u.%u.%u, Subnetmask %u.%u.%u.%u"),
          Hostname, ip[0], ip[1], ip[2], ip[3], gw[0], gw[1], gw[2], gw[3], nm[0], nm[1], nm[2], nm[3]);
        if (payload == 0) mqtt_publish(stopic, svalue);
      }          
      if ((payload == 0) || (payload == 6)) {
        snprintf_P(svalue, sizeof(svalue), PSTR("MQT: Host %s, MAX_PACKET_SIZE %d, KEEPALIVE %d"),
          sysCfg.mqtt_host, MQTT_MAX_PACKET_SIZE, MQTT_KEEPALIVE); 
      }      
    }
    else if (!grpflg && !strcmp(type,"UPGRADE")) {
      if ((data_len > 0) && (payload == 1)) {
        otaflag = 3;
        snprintf_P(svalue, sizeof(svalue), PSTR("Upgrade %s"), Version);
      }
      else
        snprintf_P(svalue, sizeof(svalue), PSTR("1 to upgrade"));
    }
    else if (!grpflg && !strcmp(type,"OTAURL")) {
      if ((data_len > 0) && (data_len < 80))
        strlcpy(sysCfg.otaUrl, (payload == 1) ? OTA_URL : dataBuf, sizeof(sysCfg.otaUrl));
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.otaUrl);
    }
    else if (!strcmp(type,"SERIALLOG")) {
      if ((data_len > 0) && (payload >= LOG_LEVEL_NONE) && (payload <= LOG_LEVEL_DEBUG_MORE)) {
        sysCfg.seriallog_level = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d"), sysCfg.seriallog_level);
    }
    else if (!strcmp(type,"SYSLOG")) {
      if ((data_len > 0) && (payload >= LOG_LEVEL_NONE) && (payload <= LOG_LEVEL_DEBUG_MORE)) {
        sysCfg.syslog_level = payload;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%d"), sysCfg.syslog_level);
    }
    else if (!strcmp(type,"LOGHOST")) {
      if ((data_len > 0) && (data_len < 32)) {
        strlcpy(sysCfg.syslog_host, (payload == 1) ? SYS_LOG_HOST : dataBuf, sizeof(sysCfg.syslog_host));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.syslog_host);
    }
    else if (!grpflg && !strcmp(type,"SSID")) {
      if ((data_len > 0) && (data_len < 32)) {
        strlcpy(sysCfg.sta_ssid, (payload == 1) ? STA_SSID : dataBuf, sizeof(sysCfg.sta_ssid));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.sta_ssid);
    }
    else if (!grpflg && !strcmp(type,"PASSWORD")) {
      if ((data_len > 0) && (data_len < 64)) {
        strlcpy(sysCfg.sta_pwd, (payload == 1) ? STA_PASS : dataBuf, sizeof(sysCfg.sta_pwd));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.sta_pwd);
    }
    else if (!grpflg && !strcmp(type,"MQTTHOST")) {
      if ((data_len > 0) && (data_len < 32)) {
        strlcpy(sysCfg.mqtt_host, (payload == 1) ? MQTT_HOST : dataBuf, sizeof(sysCfg.mqtt_host));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.mqtt_host);
    }
    else if (!strcmp(type,"GROUPTOPIC")) {
      if ((data_len > 0) && (data_len < 32)) {
        for(i = 0; i <= data_len; i++)
          if ((dataBuf[i] == '/') || (dataBuf[i] == '+') || (dataBuf[i] == '#')) dataBuf[i] = '_';
        snprintf_P(svalue, sizeof(svalue), PSTR(MQTT_CLIENT_ID), ESP.getChipId());
        if (!strcmp(dataBuf, svalue)) payload = 1;
        strlcpy(sysCfg.mqtt_grptopic, (payload == 1) ? MQTT_GRPTOPIC : dataBuf, sizeof(sysCfg.mqtt_grptopic));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.mqtt_grptopic);
    }
    else if (!grpflg && !strcmp(type,"TOPIC")) {
      if ((data_len > 0) && (data_len < 32)) {
        for(i = 0; i <= data_len; i++)
          if ((dataBuf[i] == '/') || (dataBuf[i] == '+') || (dataBuf[i] == '#')) dataBuf[i] = '_';
        snprintf_P(svalue, sizeof(svalue), PSTR(MQTT_CLIENT_ID), ESP.getChipId());
        if (!strcmp(dataBuf, svalue)) payload = 1;
        strlcpy(sysCfg.mqtt_topic, (payload == 1) ? MQTT_TOPIC : dataBuf, sizeof(sysCfg.mqtt_topic));
        restartflag = 2;
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.mqtt_topic);
    }
    else if (!grpflg && !strcmp(type,"BUTTONTOPIC")) {
      if ((data_len > 0) && (data_len < 32)) {
        for(i = 0; i <= data_len; i++)
          if ((dataBuf[i] == '/') || (dataBuf[i] == '+') || (dataBuf[i] == '#')) dataBuf[i] = '_';
        snprintf_P(svalue, sizeof(svalue), PSTR(MQTT_CLIENT_ID), ESP.getChipId());
        if (!strcmp(dataBuf, svalue)) payload = 1;
        strlcpy(sysCfg.mqtt_topic2, (payload == 1) ? MQTT_TOPIC : dataBuf, sizeof(sysCfg.mqtt_topic2));
      }
      snprintf_P(svalue, sizeof(svalue), PSTR("%s"), sysCfg.mqtt_topic2);
    }
    else if (!grpflg && !strcmp(type,"SMARTCONFIG")) {
      if ((data_len > 0) && (payload == 1)) {
        blinks = 1999;
        smartconfigflag = 1;
        snprintf_P(svalue, sizeof(svalue), PSTR("Smartconfig started"));
      } else
        snprintf_P(svalue, sizeof(svalue), PSTR("1 to start smartconfig"));
    }
    else if (!grpflg && !strcmp(type,"RESTART")) {
      if ((data_len > 0) && (payload == 1)) {
        restartflag = 2;
        snprintf_P(svalue, sizeof(svalue), PSTR("Restarting"));
      } else
        snprintf_P(svalue, sizeof(svalue), PSTR("1 to restart"));
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
      if ((data_len > 0) && (payload >= -12) && (payload <= 12)) {
        sysCfg.timezone = payload;
        rtc_timezone(sysCfg.timezone);
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
      if (!grpflg)
//        strlcpy(svalue, "Status, Upgrade, Otaurl, Restart, Reset, Smartconfig, Seriallog, Syslog, LogHost, SSId, Password, MqttHost, GroupTopic, Topic, ButtonTopic, Timezone, Light, Power, Ledstate", sizeof(svalue));  // Fails progmem access
        snprintf_P(svalue, sizeof(svalue), PSTR("Status, Upgrade, Otaurl, Restart, Reset, Smartconfig, Seriallog, Syslog, LogHost, SSId, Password, MqttHost, GroupTopic, Topic, ButtonTopic, Timezone, Light, Power, Ledstate"));
      else
//        strlcpy(svalue, "Status, GroupTopic, Timezone, Light, Power, Ledstate", sizeof(svalue));  // Fails progmem access
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
  strlcpy(svalue, (sysCfg.power == 0) ? "Off" : "On", sizeof(svalue));
  mqtt_publish(stopic, svalue);
}

void send_updateStatus(const char* svalue)
{
  char stopic[TOPSZ];
  
  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/UPGRADE"), PUB_PREFIX, sysCfg.mqtt_topic);
  mqtt_publish(stopic, svalue);
}

void every_second()
{
  char stopic[TOPSZ], svalue[TOPSZ];

  if (heartbeatflag) {
    heartbeatflag = 0;
    heartbeat++;
    snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/HEARTBEAT"), PUB_PREFIX, sysCfg.mqtt_topic);
    snprintf_P(svalue, sizeof(svalue), PSTR("%d"), heartbeat);
    mqtt_publish(stopic, svalue);
  }
}

const char commands[6][14] PROGMEM = {
  {"reset 1"},        // Hold button for more than 4 seconds
  {"light 2"},        // Press button once
  {"light 2"},        // Press button twice
  {"smartconfig 1"},  // Press button three times
  {"upgrade 1"},      // Press button four times
  {"restart 1"}};     // Press button five times

void stateloop()
{
  uint8_t button;
  char scmnd[20], log[LOGSZ];
  
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
    blinks = 1;
    multiwindow = STATES /2;         // 1/2 second multi press window
  }
  lastbutton = button;
  if (button == NOT_PRESSED) {
    holdcount = 0;
  } else {
    holdcount++;
    if (holdcount == (STATES *4)) {  // 4 seconds button hold
//      strlcpy(scmnd, commands[0], sizeof(scmnd));  // Fails progmem access
      snprintf_P(scmnd, sizeof(scmnd), commands[0]);
      multipress = 0;
      do_cmnd(scmnd);
    }
  }
  if (multiwindow) {
    multiwindow--;
  } else {
    if ((!holdcount) && (multipress >= 1) && (multipress <= 5)) {
//      strlcpy(scmnd, commands[multipress], sizeof(scmnd));  // Fails progmem access
      snprintf_P(scmnd, sizeof(scmnd), commands[multipress]);
      if (strcmp(sysCfg.mqtt_topic2,"0") && (multipress == 1) && mqttClient.connected())
        send_button(scmnd);          // Execute command via MQTT using ButtonTopic to sync external clients
      else
        do_cmnd(scmnd);              // Execute command internally 
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
        otaflag = 255;
        ESPhttpUpdate.update(sysCfg.otaUrl);
        send_updateStatus(ESPhttpUpdate.getLastErrorString().c_str());
        restartflag = 2;
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
        restartflag = 1;
      }
      restartflag--;
      if (restartflag <= 0) ESP.restart();
    }
    break;
  case (STATES/10)*6:
    if (smartconfigflag) {
      smartconfigflag = 0;
      WIFI_Check(WIFI_SMARTCONFIG);
    } else {
      WIFI_Check(WIFI_STATUS);
    }
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

#ifdef SERIAL_IO
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
      Serial.println(serialInBuf);
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
    
    sysCfg.version = VERSION;
  }

  snprintf_P(Hostname, sizeof(Hostname), PSTR(WIFI_HOSTNAME), ESP.getChipId(), sysCfg.mqtt_topic);
  WIFI_Connect(Hostname);

  mqttClient.setServer(sysCfg.mqtt_host, MQTT_PORT);
  mqttClient.setCallback(mqttDataCb);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, blinkstate);

  pinMode(REL_PIN, OUTPUT);
  digitalWrite(REL_PIN, sysCfg.power);

  pinMode(KEY_PIN, INPUT_PULLUP);

  rtc_init(sysCfg.timezone);

  snprintf_P(log, sizeof(log), PSTR("App: Project %s (Topic %s, Fallback "MQTT_CLIENT_ID", GroupTopic %s) Version %s"),
    PROJECT, sysCfg.mqtt_topic, ESP.getChipId(), sysCfg.mqtt_grptopic, Version);
  addLog(LOG_LEVEL_INFO, log);
}

void loop()
{
  if (millis() >= timersec) {
    timersec = millis() + 1000;
    rtc_second();
    if ((rtcTime.Minute == 2) && (rtcTime.Second == 30)) heartbeatflag = 1;
  }

  if (millis() >= timerxs) stateloop();

  mqttClient.loop();

#ifdef SERIAL_IO
  if (Serial.available()) serial();
#endif
  
  yield();
}

