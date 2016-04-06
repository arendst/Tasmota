/*
 * Sonoff Arduino implementation by Theo Arends
*/

#define PROJECT             "sonoff"
#define VERSION             "1.0.6"
#define CFG_HOLDER          0x20160309   // Change this value to load default configurations

// Wifi
#define STA_SSID            "indebuurt3"
#define STA_PASS            "VnsqrtnrsddbrN"
#define WIFI_HOSTNAME       "ESP-%06X-%s"

// Ota
#if (ARDUINO >= 168)
  #define OTA_URL           "http://sidnas2:80/api/arduino/"PROJECT".ino.bin"
#else
  #define OTA_URL           "http://sidnas2:80/api/arduino/"PROJECT".cpp.bin"
#endif

// MQTT
#define MQTT_HOST           "sidnas2"
#define MQTT_PORT           1883

#define MQTT_CLIENT_ID      "DVES_%06X"  // Also fall back topic using Chip Id = last 6 characters of MAC address
#define MQTT_USER           "DVES_USER"
#define MQTT_PASS           "DVES_PASS"

#define SUB_PREFIX          "cmnd"
#define PUB_PREFIX          "stat"
#define MQTT_GRPTOPIC       PROJECT"s"   // Group topic
#define MQTT_TOPIC          PROJECT

// Application
#define MQTT_SUBTOPIC       "POWER"
#define APP_TIMEZONE        1            // +1 hour (Amsterdam)
#define APP_POWER           0            // Saved power state Off

// End of user defines **************************************************************************

#define REL_PIN             12           // GPIO 12 = Red Led and Relay (0 = Off, 1 = On)

//#define LED_PIN             BUILTIN_LED  // NodeMCU
//#define LED_PIN             2            // GPIO 2 = Blue Led (0 = On, 1 = Off) - ESP-12
#define LED_PIN             13           // GPIO 13 = Green Led (0 = On, 1 = Off) - Sonoff

#define KEY_PIN             0            // GPIO 00 = Button
#define PRESSED             0
#define NOT_PRESSED         1

#define SERIAL_IO                        // Enable serial command line
#define STATES              10           // loops per second

#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ )
#else
#define DEBUG_MSG(...) 
#endif

#define WIFI_STATUS         0
#define WIFI_SMARTCONFIG    1

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <PubSubClient.h>

struct SYSCFG {
  unsigned long cfg_holder;
  unsigned long saveFlag;
  char          sta_ssid[32];
  char          sta_pwd[64];
  char          otaUrl[80];
  char          mqtt_host[32];
  char          mqtt_grptopic[32];
  char          mqtt_topic[32];
  char          mqtt_subtopic[32];
  int8_t        timezone;
  int8_t        power;
} sysCfg;

struct TIME_T {
  uint8_t Second;
  uint8_t Minute;
  uint8_t Hour;
  uint8_t Wday;   // day of week, sunday is day 1
  uint8_t Day;
  uint8_t Month;
  uint16_t Year;
  unsigned long Valid;
} rtcTime;

unsigned long timerxs;
int state = 0;
int otaflag = 0;
int restartflag = 0;
int smartconfigflag = 0;
int heartbeat = 0;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

uint8_t lastbutton = NOT_PRESSED;
uint8_t holdcount = 0;
uint8_t multiwindow = 0;
uint8_t multipress = 0;
int blinks = 3;
uint8_t blinkstate = 1;

void mqtt_publish(const char* topic, const char* data)
{
  mqttClient.publish(topic, data);
#ifdef SERIAL_IO
  Serial.printf("%s = %s\n", strrchr(topic,'/')+1, data);
#endif
}

void mqtt_reconnect()
{
  char stopic[40], svalue[40];

  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    DEBUG_MSG("APP: Attempting MQTT connection...\n");
    sprintf(svalue, MQTT_CLIENT_ID, ESP.getChipId());
    sprintf_P(stopic, PSTR("%s/%s/lwt"), PUB_PREFIX, sysCfg.mqtt_topic);
    if (mqttClient.connect(svalue, MQTT_USER, MQTT_PASS, stopic, 0, 0, "offline")) {
      DEBUG_MSG("APP: MQTT connected\n");

      sprintf_P(stopic, PSTR("%s/%s/#"), SUB_PREFIX, sysCfg.mqtt_topic);
      mqttClient.subscribe(stopic);
      sprintf_P(stopic, PSTR("%s/%s/#"), SUB_PREFIX, sysCfg.mqtt_grptopic);
      mqttClient.subscribe(stopic);
      sprintf_P(stopic, PSTR("%s/"MQTT_CLIENT_ID"/#"), SUB_PREFIX, ESP.getChipId());  // Fall back topic
      mqttClient.subscribe(stopic);

      sprintf_P(stopic, PSTR("%s/%s/NAME"), PUB_PREFIX, sysCfg.mqtt_topic);
      sprintf_P(svalue, PSTR("Sonoff switch"));
      mqtt_publish(stopic, svalue);
      sprintf_P(stopic, PSTR("%s/%s/VERSION"), PUB_PREFIX, sysCfg.mqtt_topic);
      sprintf_P(svalue, PSTR("%s"), VERSION);
      mqtt_publish(stopic, svalue);
      sprintf_P(stopic, PSTR("%s/%s/FALLBACKTOPIC"), PUB_PREFIX, sysCfg.mqtt_topic);
      sprintf_P(svalue, PSTR(MQTT_CLIENT_ID), ESP.getChipId());
      mqtt_publish(stopic, svalue);
    } else {
      DEBUG_MSG("APP MQTT connect failed, rc = %d retry in 5 seconds\n", mqttClient.state());
      delay(5000);
    }
  }
}

void mqttDataCb(char* topic, byte* data, unsigned int data_len)
{
  int i, grpflg = 0;
  char *str, *p, *mtopic = NULL, *type = NULL;
  char stopic[40], svalue[200];

  int topic_len = strlen(topic);
  char topicBuf[topic_len +1]; 
  char dataBuf[data_len +1]; 
  char dataBufUc[data_len +1]; 
  
  memcpy(topicBuf, topic, topic_len);
  topicBuf[topic_len] = 0;
  i = 0;
  for (str = strtok_r(topicBuf, "/", &p); str && i < 3; str = strtok_r(NULL, "/", &p))
  {
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

  memcpy(dataBuf, data, data_len);
  dataBuf[data_len] = 0;
  for(i = 0; i <= data_len; i++) dataBufUc[i] = toupper(dataBuf[i]);

  DEBUG_MSG("APP: MQTT DataCb Topic = %s, Group = %d, Type = %s, data = %s (%s)\n", mtopic, grpflg, type, dataBuf, dataBufUc);

  if (type != NULL) {
    blinks = 2;
    sprintf_P(stopic, PSTR("%s/%s/%s"), PUB_PREFIX, sysCfg.mqtt_topic, type);
    strcpy(svalue, "Error");

    uint16_t payload = atoi(dataBuf);
    if (!strcmp(dataBufUc,"OFF")) payload = 0;
    if (!strcmp(dataBufUc,"ON")) payload = 1;
    if (!strcmp(dataBufUc,"TOGGLE")) payload = 2;

    if (!strcmp(type,"STATUS")) {
      sprintf_P(svalue, PSTR("%s, %s, %s, %d, %d"),
        VERSION, sysCfg.mqtt_topic, sysCfg.mqtt_subtopic, sysCfg.power, sysCfg.timezone);
      if ((data_len > 0) && (payload == 1)) {
        sprintf_P(svalue, PSTR("%s, "MQTT_CLIENT_ID", %s, %s, %s, %s, %d, %d"),
          sysCfg.mqtt_grptopic, ESP.getChipId(), sysCfg.otaUrl, sysCfg.sta_ssid, sysCfg.sta_pwd, sysCfg.mqtt_host, heartbeat, sysCfg.saveFlag);
      }
      if ((data_len > 0) && (payload == 2)) {
        sprintf_P(svalue, PSTR("Version %s, Boot %d, SDK %s"), VERSION, ESP.getBootVersion(), ESP.getSdkVersion());
      }
    }
    else if (!grpflg && !strcmp(type,"UPGRADE")) {
      if ((data_len > 0) && (payload == 1)) {
        blinks = 4;
        otaflag = 3;
        sprintf_P(svalue, PSTR("Upgrade %s"), VERSION);
      }
      else
        sprintf_P(svalue, PSTR("1 to upgrade"));
    }
    else if (!grpflg && !strcmp(type,"OTAURL")) {
      if ((data_len > 0) && (data_len < 80))
        strcpy(sysCfg.otaUrl, (payload == 1) ? OTA_URL : dataBuf);
      sprintf_P(svalue, PSTR("%s"), sysCfg.otaUrl);
    }
    else if (!grpflg && !strcmp(type,"SSID")) {
      if ((data_len > 0) && (data_len < 32)) {
        strcpy(sysCfg.sta_ssid, (payload == 1) ? STA_SSID : dataBuf);
        restartflag = 2;
      }
      sprintf_P(svalue, PSTR("%s"), sysCfg.sta_ssid);
    }
    else if (!grpflg && !strcmp(type,"PASSWORD")) {
      if ((data_len > 0) && (data_len < 64)) {
        strcpy(sysCfg.sta_pwd, (payload == 1) ? STA_PASS : dataBuf);
        restartflag = 2;
      }
      sprintf_P(svalue, PSTR("%s"), sysCfg.sta_pwd);
    }
    else if (!grpflg && !strcmp(type,"HOST")) {
      if ((data_len > 0) && (data_len < 32)) {
        strcpy(sysCfg.mqtt_host, (payload == 1) ? MQTT_HOST : dataBuf);
        restartflag = 2;
      }
      sprintf_P(svalue, PSTR("%s"), sysCfg.mqtt_host);
    }
    else if (!strcmp(type,"GROUPTOPIC")) {
      if ((data_len > 0) && (data_len < 32)) {
        for(i = 0; i <= data_len; i++)
          if ((dataBuf[i] == '/') || (dataBuf[i] == '+') || (dataBuf[i] == '#')) dataBuf[i] = '_';
        sprintf_P(svalue, PSTR(MQTT_CLIENT_ID), ESP.getChipId());
        if (!strcmp(dataBuf, svalue)) payload = 1;
        strcpy(sysCfg.mqtt_grptopic, (payload == 1) ? MQTT_GRPTOPIC : dataBuf);
        restartflag = 2;
      }
      sprintf_P(svalue, PSTR("%s"), sysCfg.mqtt_grptopic);
    }
    else if (!grpflg && !strcmp(type,"TOPIC")) {
      if ((data_len > 0) && (data_len < 32)) {
        for(i = 0; i <= data_len; i++)
          if ((dataBuf[i] == '/') || (dataBuf[i] == '+') || (dataBuf[i] == '#')) dataBuf[i] = '_';
        sprintf_P(svalue, PSTR(MQTT_CLIENT_ID), ESP.getChipId());
        if (!strcmp(dataBuf, svalue)) payload = 1;
        strcpy(sysCfg.mqtt_topic, (payload == 1) ? MQTT_TOPIC : dataBuf);
        restartflag = 2;
      }
      sprintf_P(svalue, PSTR("%s"), sysCfg.mqtt_topic);
    }
    else if (!grpflg && !strcmp(type,"SMARTCONFIG")) {
      if ((data_len > 0) && (payload == 1)) {
        smartconfigflag = 1;
        sprintf_P(svalue, PSTR("Smartconfig started"));
      } else
        sprintf_P(svalue, PSTR("1 to start smartconfig"));
    }
    else if (!grpflg && !strcmp(type,"RESTART")) {
      if ((data_len > 0) && (payload == 1)) {
        restartflag = 2;
        sprintf_P(svalue, PSTR("Restarting"));
      } else
        sprintf_P(svalue, PSTR("1 to restart"));
    }
    else if (!grpflg && !strcmp(type,"RESET")) {
      if ((data_len > 0) && (payload == 1)) {
        CFG_Default();
        restartflag = 2;
        sprintf_P(svalue, PSTR("Reset and Restarting"));
      } else
        sprintf_P(svalue, PSTR("1 to reset"));
    }
    else if (!strcmp(type,"TIMEZONE")) {
      if ((data_len > 0) && (payload >= -12) && (payload <= 12)) {
        sysCfg.timezone = payload;
        rtc_timezone(sysCfg.timezone);
      }
      sprintf_P(svalue, PSTR("%d"), sysCfg.timezone);
    }
    else if ((!strcmp(type,"LIGHT")) || (!strcmp(type,"POWER"))) {
      sprintf(sysCfg.mqtt_subtopic, "%s", type);
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
      strcpy(svalue, (sysCfg.power == 0) ? "Off" : "On");
    }
    else {
      type = NULL;
    }
    if (type == NULL) {
      blinks = 1;
      sprintf_P(stopic, PSTR("%s/%s/SYNTAX"), PUB_PREFIX, sysCfg.mqtt_topic);
      if (!grpflg)
        strcpy_P(svalue, PSTR("Status, Upgrade, Otaurl, Restart, Reset, Smartconfig, SSId, Password, Host, GroupTopic, Topic, Timezone, Light, Power"));
      else
        strcpy_P(svalue, PSTR("Status, GroupTopic, Timezone, Light, Power"));
    }
    mqtt_publish(stopic, svalue);
  }
}

void send_power()
{
  char stopic[40], svalue[20];

  sprintf_P(stopic, PSTR("%s/%s/%s"), PUB_PREFIX, sysCfg.mqtt_topic, sysCfg.mqtt_subtopic);
  strcpy(svalue, (sysCfg.power == 0) ? "Off" : "On");
  mqtt_publish(stopic, svalue);
}

void send_heartbeat()
{
  char stopic[40], svalue[20];

  heartbeat++;
  sprintf_P(stopic, PSTR("%s/%s/HEARTBEAT"), PUB_PREFIX, sysCfg.mqtt_topic);
  sprintf_P(svalue, PSTR("%d"), heartbeat);
  mqtt_publish(stopic, svalue);
}

void send_updateStatus(const char* svalue)
{
  char stopic[40];
  
  sprintf_P(stopic, PSTR("%s/%s/UPGRADE"), PUB_PREFIX, sysCfg.mqtt_topic);
  mqtt_publish(stopic, svalue);
}

void stateloop()
{
  uint8_t button;
  
  timerxs = millis() + (1000 / STATES);
  state++;
  if (state == STATES) {             // Every second
    state = 0;
    rtc_second();
    if ((rtcTime.Minute == 2) && (rtcTime.Second == 30)) send_heartbeat();
  }

  button = digitalRead(KEY_PIN);
  if ((button == PRESSED) && (lastbutton == NOT_PRESSED)) {
    if (multiwindow == 0) {
      multipress = 1;
      sysCfg.power ^= 1;
      digitalWrite(REL_PIN, sysCfg.power);
      send_power();
    } else {
      multipress++;
      DEBUG_MSG("APP: Multipress %d\n", multipress);
    }
    blinks = 1;
    multiwindow = STATES;            // 1 second multi press window
  }
  lastbutton = button;
  if (button == NOT_PRESSED) {
    holdcount = 0;
  } else {
    holdcount++;
    if (holdcount == (STATES *4)) {  // 4 seconds button hold
      CFG_Default();
      multipress = 0;
      restartflag = 4;               // Allow 4 second delay to release button
      blinks = 2;
    }
  }
  if (multiwindow) {
    multiwindow--;
  } else {
    switch (multipress) {
    case 3:
      smartconfigflag = 1;
      blinks = 1000;
      break;
    case 4:
      otaflag = 1;
      digitalWrite(LED_PIN, 0);
      break;
    }
    multipress = 0;
  }

  if ((blinks) && (state & 1)) {
    blinkstate ^= 1;
    digitalWrite(LED_PIN, blinkstate);
    if (blinkstate) blinks--;
  }

  switch (state) {
  case (STATES/10)*2:
    if (otaflag) {
      otaflag--;
      if (otaflag <= 0) {
        ESPhttpUpdate.update(sysCfg.otaUrl);
        send_updateStatus(ESPhttpUpdate.getLastErrorString().c_str());
      }
    }
    break;
  case (STATES/10)*4:
    CFG_Save();
    if (restartflag) {
      restartflag--;
//      if (restartflag <= 0) ESP.reset();
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
  }
}

#ifdef SERIAL_IO
#define INPUT_BUFFER_SIZE          128

byte SerialInByte;
int SerialInByteCounter = 0;
char serialInBuf[INPUT_BUFFER_SIZE + 2];

void serial()
{
  char stopic[128], svalue[128];
  char *token;

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
      token = strtok(serialInBuf, " ");
      sprintf_P(stopic, PSTR("%s/%s/%s"), SUB_PREFIX, sysCfg.mqtt_topic, token);
      token = strtok(NULL, "");
      sprintf_P(svalue, PSTR("%s"), (token == NULL) ? "" : token);
      SerialInByteCounter = 0;
      mqttDataCb(stopic, (byte*)svalue, strlen(svalue));
    }
  }
}
#endif

void setup()
{
  Serial.begin(115200);
  delay(10);
  Serial.println();

  CFG_Load();

  Serial.printf("Project %s (Topic %s, Fallback "MQTT_CLIENT_ID", GroupTopic %s) Version %s (Boot %d, SDK %s)\n",
    PROJECT, sysCfg.mqtt_topic, ESP.getChipId(), sysCfg.mqtt_grptopic, VERSION, ESP.getBootVersion(), ESP.getSdkVersion());

  WIFI_Connect();

  mqttClient.setServer(sysCfg.mqtt_host, MQTT_PORT);
  mqttClient.setCallback(mqttDataCb);

  timerxs = millis() + (1000 / STATES);

  pinMode(REL_PIN, OUTPUT);
  digitalWrite(REL_PIN, sysCfg.power);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, blinkstate);

  pinMode(KEY_PIN, INPUT_PULLUP);

  rtc_init(sysCfg.timezone);

  DEBUG_MSG("APP: Started\n");
}

void loop()
{
  if (millis() >= timerxs)
    stateloop();

  if ((WiFi.status() == WL_CONNECTED) && (!mqttClient.connected())) {
    mqtt_reconnect();
  }
  mqttClient.loop();

#ifdef SERIAL_IO
  if (Serial.available())
    serial();
#endif
  
  yield();
}

