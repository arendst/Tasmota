/*
  sonoff.ino - Sonoff-Tasmota firmware for iTead Sonoff, Wemos and NodeMCU hardware

  Copyright (C) 2019  Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/*====================================================
  Prerequisites:
    - Change libraries/PubSubClient/src/PubSubClient.h
        #define MQTT_MAX_PACKET_SIZE 1000

    - Select IDE Tools - Flash Mode: "DOUT"
    - Select IDE Tools - Flash Size: "1M (no SPIFFS)"
  ====================================================*/

// Location specific includes
#include <core_version.h>                   // Arduino_Esp8266 version information (ARDUINO_ESP8266_RELEASE and ARDUINO_ESP8266_RELEASE_2_3_0)
#include "sonoff_version.h"                 // Sonoff-Tasmota version information
#include "sonoff.h"                         // Enumeration used in my_user_config.h
#include "my_user_config.h"                 // Fixed user configurable options
#ifdef USE_MQTT_TLS
  #include <t_bearssl.h>                    // we need to include before "sonoff_post.h" to take precedence over the BearSSL version in Arduino
#endif  // USE_MQTT_TLS
#include "sonoff_post.h"                    // Configuration overrides for all previous includes
#include "i18n.h"                           // Language support configured by my_user_config.h
#include "sonoff_template.h"                // Hardware configuration


#ifdef ARDUINO_ESP8266_RELEASE_2_4_0
#include "lwip/init.h"
#if LWIP_VERSION_MAJOR != 1
  #error Please use stable lwIP v1.4
#endif
#endif

// Libraries
#include <ESP8266HTTPClient.h>              // Ota
#include <ESP8266httpUpdate.h>              // Ota
#include <StreamString.h>                   // Webserver, Updater
#include <ArduinoJson.h>                    // WemoHue, IRremote, Domoticz
#ifdef USE_ARDUINO_OTA
  #include <ArduinoOTA.h>                   // Arduino OTA
  #ifndef USE_DISCOVERY
  #define USE_DISCOVERY
  #endif
#endif  // USE_ARDUINO_OTA
#ifdef USE_DISCOVERY
  #include <ESP8266mDNS.h>                  // MQTT, Webserver, Arduino OTA
#endif  // USE_DISCOVERY
#ifdef USE_I2C
  #include <Wire.h>                         // I2C support library
#endif  // USE_I2C
#ifdef USE_SPI
  #include <SPI.h>                          // SPI support, TFT
#endif  // USE_SPI

// Structs
#include "settings.h"

const char kSleepMode[] PROGMEM = "Dynamic|Normal";
const char kPrefixes[] PROGMEM = D_CMND "|" D_STAT "|" D_TELE;
const char kCodeImage[] PROGMEM = "sonoff|minimal|sensors|knx|basic|display|ir";

// Global variables
SerialConfig serial_config = SERIAL_8N1;    // Serial interface configuration 8 data bits, No parity, 1 stop bit

WiFiUDP PortUdp;                            // UDP Syslog and Alexa

unsigned long feature_drv1;                 // Compiled driver feature map
unsigned long feature_drv2;                 // Compiled driver feature map
unsigned long feature_sns1;                 // Compiled sensor feature map
unsigned long feature_sns2;                 // Compiled sensor feature map
unsigned long feature5;                     // Compiled feature map
unsigned long serial_polling_window = 0;    // Serial polling window
unsigned long state_second = 0;             // State second timer
unsigned long state_50msecond = 0;          // State 50msecond timer
unsigned long state_100msecond = 0;         // State 100msecond timer
unsigned long state_250msecond = 0;         // State 250msecond timer
unsigned long pulse_timer[MAX_PULSETIMERS] = { 0 }; // Power off timer
unsigned long blink_timer = 0;              // Power cycle timer
unsigned long backlog_delay = 0;            // Command backlog delay
power_t power = 0;                          // Current copy of Settings.power
power_t last_power = 0;                     // Last power set state
power_t blink_power;                        // Blink power state
power_t blink_mask = 0;                     // Blink relay active mask
power_t blink_powersave;                    // Blink start power save state
power_t latching_power = 0;                 // Power state at latching start
power_t rel_inverted = 0;                   // Relay inverted flag (1 = (0 = On, 1 = Off))
int baudrate = APP_BAUDRATE;                // Serial interface baud rate
int serial_in_byte_counter = 0;             // Index in receive buffer
int ota_state_flag = 0;                     // OTA state flag
int ota_result = 0;                         // OTA result
int restart_flag = 0;                       // Sonoff restart flag
int wifi_state_flag = WIFI_RESTART;         // Wifi state flag
int tele_period = 1;                        // Tele period timer
int blinks = 201;                           // Number of LED blinks
uint32_t uptime = 0;                        // Counting every second until 4294967295 = 130 year
uint32_t loop_load_avg = 0;                 // Indicative loop load average
uint32_t global_update = 0;                 // Timestamp of last global temperature and humidity update
uint32_t web_log_index = 1;                 // Index in Web log buffer (should never be 0)
float global_temperature = 9999;            // Provide a global temperature to be used by some sensors
float global_humidity = 0;                  // Provide a global humidity to be used by some sensors
float global_pressure = 0;                  // Provide a global pressure to be used by some sensors
char *ota_url;                              // OTA url string pointer
uint16_t mqtt_cmnd_publish = 0;             // ignore flag for publish command
uint16_t blink_counter = 0;                 // Number of blink cycles
uint16_t seriallog_timer = 0;               // Timer to disable Seriallog
uint16_t syslog_timer = 0;                  // Timer to re-enable syslog_level
int16_t save_data_counter;                  // Counter and flag for config save to Flash
RulesBitfield rules_flag;                   // Rule state flags (16 bits)
uint8_t state_250mS = 0;                    // State 250msecond per second flag
uint8_t latching_relay_pulse = 0;           // Latching relay pulse timer
uint8_t sleep;                              // Current copy of Settings.sleep
uint8_t blinkspeed = 1;                     // LED blink rate
uint8_t pin[GPIO_MAX];                      // Possible pin configurations
uint8_t active_device = 1;                  // Active device in ExecuteCommandPower
uint8_t leds_present = 0;                   // Max number of LED supported
uint8_t led_inverted = 0;                   // LED inverted flag (1 = (0 = On, 1 = Off))
uint8_t led_power = 0;                      // LED power state
uint8_t ledlnk_inverted = 0;                // Link LED inverted flag (1 = (0 = On, 1 = Off))
uint8_t pwm_inverted = 0;                   // PWM inverted flag (1 = inverted)
uint8_t energy_flg = 0;                     // Energy monitor configured
uint8_t light_flg = 0;                      // Light module configured
uint8_t light_type = 0;                     // Light types
uint8_t serial_in_byte;                     // Received byte
uint8_t ota_retry_counter = OTA_ATTEMPTS;   // OTA retry counter
uint8_t devices_present = 0;                // Max number of devices supported
uint8_t seriallog_level;                    // Current copy of Settings.seriallog_level
uint8_t syslog_level;                       // Current copy of Settings.syslog_level
uint8_t my_module_type;                     // Current copy of Settings.module or user template type
uint8_t my_adc0;                            // Active copy of Module ADC0
uint8_t last_source = 0;                    // Last command source
uint8_t shutters_present = 0;               // Number of actual define shutters
//uint8_t mdns_delayed_start = 0;             // mDNS delayed start
bool serial_local = false;                  // Handle serial locally;
bool fallback_topic_flag = false;           // Use Topic or FallbackTopic
bool backlog_mutex = false;                 // Command backlog pending
bool interlock_mutex = false;               // Interlock power command pending
bool stop_flash_rotate = false;             // Allow flash configuration rotation
bool blinkstate = false;                    // LED state
//bool latest_uptime_flag = true;             // Signal latest uptime
bool pwm_present = false;                   // Any PWM channel configured with SetOption15 0
bool i2c_flg = false;                       // I2C configured
bool spi_flg = false;                       // SPI configured
bool soft_spi_flg = false;                  // Software SPI configured
bool ntp_force_sync = false;                // Force NTP sync
bool ntp_synced_message = false;            // NTP synced message flag
bool prep_called = false;                   // Deep sleep flag to detect a proper start of initialize sensors
myio my_module;                             // Active copy of Module GPIOs (17 x 8 bits)
gpio_flag my_module_flag;                   // Active copy of Template GPIO flags
StateBitfield global_state;                 // Global states (currently Wifi and Mqtt) (8 bits)
char my_version[33];                        // Composed version string
char my_image[33];                          // Code image and/or commit
char my_hostname[33];                       // Composed Wifi hostname
char mqtt_client[33];                       // Composed MQTT Clientname
char mqtt_topic[33];                        // Composed MQTT topic
char serial_in_buffer[INPUT_BUFFER_SIZE];   // Receive buffer
char mqtt_data[MESSZ];                      // MQTT publish buffer and web page ajax buffer
char log_data[LOGSZ];                       // Logging
char web_log[WEB_LOG_SIZE] = {'\0'};        // Web log buffer
#ifdef SUPPORT_IF_STATEMENT
  #include <LinkedList.h>
  LinkedList<String> backlog;               // Command backlog implemented with LinkedList
  #define BACKLOG_EMPTY (backlog.size() == 0)
#else
  uint8_t backlog_index = 0;                // Command backlog index
  uint8_t backlog_pointer = 0;              // Command backlog pointer
  String backlog[MAX_BACKLOG];              // Command backlog buffer
  #define BACKLOG_EMPTY (backlog_pointer == backlog_index)
#endif

/********************************************************************************************/

char* Format(char* output, const char* input, int size)
{
  char *token;
  uint32_t digits = 0;

  if (strstr(input, "%") != nullptr) {
    strlcpy(output, input, size);
    token = strtok(output, "%");
    if (strstr(input, "%") == input) {
      output[0] = '\0';
    } else {
      token = strtok(nullptr, "");
    }
    if (token != nullptr) {
      digits = atoi(token);
      if (digits) {
        char tmp[size];
        if (strchr(token, 'd')) {
          snprintf_P(tmp, size, PSTR("%s%c0%dd"), output, '%', digits);
          snprintf_P(output, size, tmp, ESP.getChipId() & 0x1fff);            // %04d - short chip ID in dec, like in hostname
        } else {
          snprintf_P(tmp, size, PSTR("%s%c0%dX"), output, '%', digits);
          snprintf_P(output, size, tmp, ESP.getChipId());                   // %06X - full chip ID in hex
        }
      } else {
        if (strchr(token, 'd')) {
          snprintf_P(output, size, PSTR("%s%d"), output, ESP.getChipId());  // %d - full chip ID in dec
          digits = 8;
        }
      }
    }
  }
  if (!digits) {
    strlcpy(output, input, size);
  }
  return output;
}

char* GetOtaUrl(char *otaurl, size_t otaurl_size)
{
  if (strstr(Settings.ota_url, "%04d") != nullptr) {     // OTA url contains placeholder for chip ID
    snprintf(otaurl, otaurl_size, Settings.ota_url, ESP.getChipId() & 0x1fff);
  }
  else if (strstr(Settings.ota_url, "%d") != nullptr) {  // OTA url contains placeholder for chip ID
    snprintf_P(otaurl, otaurl_size, Settings.ota_url, ESP.getChipId());
  }
  else {
    strlcpy(otaurl, Settings.ota_url, otaurl_size);
  }
  return otaurl;
}

char* GetTopic_P(char *stopic, uint32_t prefix, char *topic, const char* subtopic)
{
  /* prefix 0 = Cmnd
     prefix 1 = Stat
     prefix 2 = Tele
     prefix 4 = Cmnd fallback
     prefix 5 = Stat fallback
     prefix 6 = Tele fallback
  */
  char romram[CMDSZ];
  String fulltopic;

  snprintf_P(romram, sizeof(romram), subtopic);
  if (fallback_topic_flag || (prefix > 3)) {
    prefix &= 3;
    char stemp[11];
    fulltopic = GetTextIndexed(stemp, sizeof(stemp), prefix, kPrefixes);
    fulltopic += F("/");
    fulltopic += mqtt_client;
    fulltopic += F("_fb");                    // cmnd/<mqttclient>_fb
  } else {
    fulltopic = Settings.mqtt_fulltopic;
    if ((0 == prefix) && (-1 == fulltopic.indexOf(FPSTR(MQTT_TOKEN_PREFIX)))) {
      fulltopic += F("/");
      fulltopic += FPSTR(MQTT_TOKEN_PREFIX);  // Need prefix for commands to handle mqtt topic loops
    }
    for (uint32_t i = 0; i < 3; i++) {
      if ('\0' == Settings.mqtt_prefix[i][0]) {
        GetTextIndexed(Settings.mqtt_prefix[i], sizeof(Settings.mqtt_prefix[i]), i, kPrefixes);
      }
    }
    fulltopic.replace(FPSTR(MQTT_TOKEN_PREFIX), Settings.mqtt_prefix[prefix]);
    fulltopic.replace(FPSTR(MQTT_TOKEN_TOPIC), topic);
    fulltopic.replace(F("%hostname%"), my_hostname);
    String token_id = WiFi.macAddress();
    token_id.replace(":", "");
    fulltopic.replace(F("%id%"), token_id);
  }
  fulltopic.replace(F("#"), "");
  fulltopic.replace(F("//"), "/");
  if (!fulltopic.endsWith("/")) {
    fulltopic += "/";
  }
  snprintf_P(stopic, TOPSZ, PSTR("%s%s"), fulltopic.c_str(), romram);
  return stopic;
}

char* GetFallbackTopic_P(char *stopic, uint32_t prefix, const char* subtopic)
{
  return GetTopic_P(stopic, prefix +4, nullptr, subtopic);
}

char* GetStateText(uint32_t state)
{
  if (state > 3) {
    state = 1;
  }
  return Settings.state_text[state];
}

/********************************************************************************************/

void SetLatchingRelay(power_t lpower, uint32_t state)
{
  // power xx00 - toggle REL1 (Off) and REL3 (Off) - device 1 Off, device 2 Off
  // power xx01 - toggle REL2 (On)  and REL3 (Off) - device 1 On,  device 2 Off
  // power xx10 - toggle REL1 (Off) and REL4 (On)  - device 1 Off, device 2 On
  // power xx11 - toggle REL2 (On)  and REL4 (On)  - device 1 On,  device 2 On

  if (state && !latching_relay_pulse) {  // Set latching relay to power if previous pulse has finished
    latching_power = lpower;
    latching_relay_pulse = 2;            // max 200mS (initiated by stateloop())
  }

  for (uint32_t i = 0; i < devices_present; i++) {
    uint32_t port = (i << 1) + ((latching_power >> i) &1);
    if (pin[GPIO_REL1 +port] < 99) {
      digitalWrite(pin[GPIO_REL1 +port], bitRead(rel_inverted, port) ? !state : state);
    }
  }
}

void SetDevicePower(power_t rpower, uint32_t source)
{
  ShowSource(source);
  last_source = source;

  if (POWER_ALL_ALWAYS_ON == Settings.poweronstate) {  // All on and stay on
    power = (1 << devices_present) -1;
    rpower = power;
  }

  if (Settings.flag.interlock) {          // Allow only one or no relay set
    for (uint32_t i = 0; i < MAX_INTERLOCKS; i++) {
      power_t mask = 1;
      uint32_t count = 0;
      for (uint32_t j = 0; j < devices_present; j++) {
        if ((Settings.interlock[i] & mask) && (rpower & mask)) {
          count++;
        }
        mask <<= 1;
      }
      if (count > 1) {
        mask = ~Settings.interlock[i];    // Turn interlocked group off as there would be multiple relays on
        power &= mask;
        rpower &= mask;
      }
    }
  }

  if (rpower) {                           // Any power set
    last_power = rpower;
  }

  XdrvMailbox.index = rpower;
  XdrvCall(FUNC_SET_POWER);               // Signal power state

  XdrvMailbox.index = rpower;
  XdrvMailbox.payload = source;
  if (XdrvCall(FUNC_SET_DEVICE_POWER)) {  // Set power state and stop if serviced
    // Serviced
  }
  else if ((SONOFF_DUAL == my_module_type) || (CH4 == my_module_type)) {
    Serial.write(0xA0);
    Serial.write(0x04);
    Serial.write(rpower &0xFF);
    Serial.write(0xA1);
    Serial.write('\n');
    Serial.flush();
  }
  else if (EXS_RELAY == my_module_type) {
    SetLatchingRelay(rpower, 1);
  }
  else {
    for (uint32_t i = 0; i < devices_present; i++) {
      power_t state = rpower &1;
      if ((i < MAX_RELAYS) && (pin[GPIO_REL1 +i] < 99)) {
        digitalWrite(pin[GPIO_REL1 +i], bitRead(rel_inverted, i) ? !state : state);
      }
      rpower >>= 1;
    }
  }
}

void RestorePower(bool publish_power, uint32_t source)
{
  if (power != last_power) {
    SetDevicePower(last_power, source);
    if (publish_power) {
      MqttPublishAllPowerState();
    }
  }
}

void SetAllPower(uint32_t state, uint32_t source)
{
// state 0 = POWER_OFF = Relay Off
// state 1 = POWER_ON = Relay On (turn off after Settings.pulse_timer * 100 mSec if enabled)
// state 2 = POWER_TOGGLE = Toggle relay
// state 8 = POWER_OFF_NO_STATE = Relay Off and no publishPowerState
// state 9 = POWER_ON_NO_STATE = Relay On and no publishPowerState
// state 10 = POWER_TOGGLE_NO_STATE = Toggle relay and no publishPowerState
// state 16 = POWER_SHOW_STATE = Show power state

  bool publish_power = true;
  if ((state >= POWER_OFF_NO_STATE) && (state <= POWER_TOGGLE_NO_STATE)) {
    state &= 3;                           // POWER_OFF, POWER_ON or POWER_TOGGLE
    publish_power = false;
  }
  if ((state >= POWER_OFF) && (state <= POWER_TOGGLE)) {
    power_t all_on = (1 << devices_present) -1;
    switch (state) {
    case POWER_OFF:
      power = 0;
      break;
    case POWER_ON:
      power = all_on;
      break;
    case POWER_TOGGLE:
      power ^= all_on;                    // Complement current state
    }
    SetDevicePower(power, source);
  }
  if (publish_power) {
    MqttPublishAllPowerState();
  }
}

void SetLedPowerIdx(uint32_t led, uint32_t state)
{
  if ((99 == pin[GPIO_LEDLNK]) && (0 == led)) {  // Legacy - LED1 is link led only if LED2 is present
    if (pin[GPIO_LED2] < 99) {
      led = 1;
    }
  }
  if (pin[GPIO_LED1 + led] < 99) {
    uint32_t mask = 1 << led;
    if (state) {
      state = 1;
      led_power |= mask;
    } else {
      led_power &= (0xFF ^ mask);
    }
    digitalWrite(pin[GPIO_LED1 + led], bitRead(led_inverted, led) ? !state : state);
  }
}

void SetLedPower(uint32_t state)
{
  if (99 == pin[GPIO_LEDLNK]) {           // Legacy - Only use LED1 and/or LED2
    SetLedPowerIdx(0, state);
  } else {
    power_t mask = 1;
    for (uint32_t i = 0; i < leds_present; i++) {  // Map leds to power
      bool tstate = (power & mask);
      SetLedPowerIdx(i, tstate);
      mask <<= 1;
    }
  }
}

void SetLedPowerAll(uint32_t state)
{
  for (uint32_t i = 0; i < leds_present; i++) {
    SetLedPowerIdx(i, state);
  }
}

void SetLedLink(uint32_t state)
{
  uint32_t led_pin = pin[GPIO_LEDLNK];
  uint32_t led_inv = ledlnk_inverted;
  if (99 == led_pin) {                    // Legacy - LED1 is status
    led_pin = pin[GPIO_LED1];
    led_inv = bitRead(led_inverted, 0);
  }
  if (led_pin < 99) {
    if (state) { state = 1; }
    digitalWrite(led_pin, (led_inv) ? !state : state);
  }
}

void SetPulseTimer(uint32_t index, uint32_t time)
{
  pulse_timer[index] = (time > 111) ? millis() + (1000 * (time - 100)) : (time > 0) ? millis() + (100 * time) : 0L;
}

uint32_t GetPulseTimer(uint32_t index)
{
  long time = TimePassedSince(pulse_timer[index]);
  if (time < 0) {
    time *= -1;
    return (time > 11100) ? (time / 1000) + 100 : (time > 0) ? time / 100 : 0;
  }
  return 0;
}

/********************************************************************************************/

bool SendKey(uint32_t key, uint32_t device, uint32_t state)
{
// key 0 = KEY_BUTTON = button_topic
// key 1 = KEY_SWITCH = switch_topic
// state 0 = POWER_OFF = off
// state 1 = POWER_ON = on
// state 2 = POWER_TOGGLE = toggle
// state 3 = POWER_HOLD = hold
// state 9 = CLEAR_RETAIN = clear retain flag

  char stopic[TOPSZ];
  char scommand[CMDSZ];
  char key_topic[sizeof(Settings.button_topic)];
  bool result = false;

  char *tmp = (key) ? Settings.switch_topic : Settings.button_topic;
  Format(key_topic, tmp, sizeof(key_topic));
  if (Settings.flag.mqtt_enabled && MqttIsConnected() && (strlen(key_topic) != 0) && strcmp(key_topic, "0")) {
    if (!key && (device > devices_present)) {
      device = 1;                  // Only allow number of buttons up to number of devices
    }
    GetTopic_P(stopic, CMND, key_topic,
               GetPowerDevice(scommand, device, sizeof(scommand), (key + Settings.flag.device_index_enable)));  // cmnd/switchtopic/POWERx
    if (CLEAR_RETAIN == state) {
      mqtt_data[0] = '\0';
    } else {
      if ((Settings.flag3.button_switch_force_local || !strcmp(mqtt_topic, key_topic) || !strcmp(Settings.mqtt_grptopic, key_topic)) && (POWER_TOGGLE == state)) {
        state = ~(power >> (device -1)) &1;  // POWER_OFF or POWER_ON
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), GetStateText(state));
    }
#ifdef USE_DOMOTICZ
    if (!(DomoticzSendKey(key, device, state, strlen(mqtt_data)))) {
      MqttPublishDirect(stopic, ((key) ? Settings.flag.mqtt_switch_retain : Settings.flag.mqtt_button_retain) && (state != POWER_HOLD || !Settings.flag3.no_hold_retain));
    }
#else
    MqttPublishDirect(stopic, ((key) ? Settings.flag.mqtt_switch_retain : Settings.flag.mqtt_button_retain) && (state != POWER_HOLD || !Settings.flag3.no_hold_retain));
#endif  // USE_DOMOTICZ
    result = !Settings.flag3.button_switch_force_local;
  } else {
    Response_P(PSTR("{\"%s%d\":{\"State\":%d}}"), (key) ? "Switch" : "Button", device, state);
    result = XdrvRulesProcess();
  }
  int32_t payload_save = XdrvMailbox.payload;
  XdrvMailbox.payload = key << 16 | state << 8 | device;
  XdrvCall(FUNC_ANY_KEY);
  XdrvMailbox.payload = payload_save;
  return result;
}

void ExecuteCommandPower(uint32_t device, uint32_t state, uint32_t source)
{
// device  = Relay number 1 and up
// state 0 = POWER_OFF = Relay Off
// state 1 = POWER_ON = Relay On (turn off after Settings.pulse_timer * 100 mSec if enabled)
// state 2 = POWER_TOGGLE = Toggle relay
// state 3 = POWER_BLINK = Blink relay
// state 4 = POWER_BLINK_STOP = Stop blinking relay
// state 8 = POWER_OFF_NO_STATE = Relay Off and no publishPowerState
// state 9 = POWER_ON_NO_STATE = Relay On and no publishPowerState
// state 10 = POWER_TOGGLE_NO_STATE = Toggle relay and no publishPowerState
// state 16 = POWER_SHOW_STATE = Show power state

//  ShowSource(source);

#ifdef USE_SONOFF_IFAN
  if (IsModuleIfan()) {
    blink_mask &= 1;                 // No blinking on the fan relays
    Settings.flag.interlock = 0;     // No interlock mode as it is already done by the microcontroller
    Settings.pulse_timer[1] = 0;     // No pulsetimers on the fan relays
    Settings.pulse_timer[2] = 0;
    Settings.pulse_timer[3] = 0;
  }
#endif  // USE_SONOFF_IFAN

  bool publish_power = true;
  if ((state >= POWER_OFF_NO_STATE) && (state <= POWER_TOGGLE_NO_STATE)) {
    state &= 3;                      // POWER_OFF, POWER_ON or POWER_TOGGLE
    publish_power = false;
  }

  if ((device < 1) || (device > devices_present)) {
    device = 1;
  }
  active_device = device;

  if (device <= MAX_PULSETIMERS) {
    SetPulseTimer(device -1, 0);
  }
  power_t mask = 1 << (device -1);        // Device to control
  if (state <= POWER_TOGGLE) {
    if ((blink_mask & mask)) {
      blink_mask &= (POWER_MASK ^ mask);  // Clear device mask
      MqttPublishPowerBlinkState(device);
    }

    if (Settings.flag.interlock &&
        !interlock_mutex &&
        ((POWER_ON == state) || ((POWER_TOGGLE == state) && !(power & mask)))
       ) {
      interlock_mutex = true;                           // Clear all but masked relay in interlock group if new set requested
      for (uint32_t i = 0; i < MAX_INTERLOCKS; i++) {
        if (Settings.interlock[i] & mask) {             // Find interlock group
          for (uint32_t j = 0; j < devices_present; j++) {
            power_t imask = 1 << j;
            if ((Settings.interlock[i] & imask) && (power & imask) && (mask != imask)) {
              ExecuteCommandPower(j +1, POWER_OFF, SRC_IGNORE);
              delay(50);                                // Add some delay to make sure never have more than one relay on
            }
          }
          break;                                        // An interlocked relay is only present in one group so quit
        }
      }
      interlock_mutex = false;
    }

    switch (state) {
    case POWER_OFF: {
      power &= (POWER_MASK ^ mask);
      break; }
    case POWER_ON:
      power |= mask;
      break;
    case POWER_TOGGLE:
      power ^= mask;
    }
    SetDevicePower(power, source);
#ifdef USE_DOMOTICZ
    DomoticzUpdatePowerState(device);
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
    KnxUpdatePowerState(device, power);
#endif  // USE_KNX
    if (publish_power && Settings.flag3.hass_tele_on_power) {
      MqttPublishTeleState();
    }
    if (device <= MAX_PULSETIMERS) {  // Restart PulseTime if powered On
      SetPulseTimer(device -1, (((POWER_ALL_OFF_PULSETIME_ON == Settings.poweronstate) ? ~power : power) & mask) ? Settings.pulse_timer[device -1] : 0);
    }
  }
  else if (POWER_BLINK == state) {
    if (!(blink_mask & mask)) {
      blink_powersave = (blink_powersave & (POWER_MASK ^ mask)) | (power & mask);  // Save state
      blink_power = (power >> (device -1))&1;  // Prep to Toggle
    }
    blink_timer = millis() + 100;
    blink_counter = ((!Settings.blinkcount) ? 64000 : (Settings.blinkcount *2)) +1;
    blink_mask |= mask;  // Set device mask
    MqttPublishPowerBlinkState(device);
    return;
  }
  else if (POWER_BLINK_STOP == state) {
    bool flag = (blink_mask & mask);
    blink_mask &= (POWER_MASK ^ mask);  // Clear device mask
    MqttPublishPowerBlinkState(device);
    if (flag) {
      ExecuteCommandPower(device, (blink_powersave >> (device -1))&1, SRC_IGNORE);  // Restore state
    }
    return;
  }
  if (publish_power) {
    MqttPublishPowerState(device);
  }
}

void StopAllPowerBlink(void)
{
  power_t mask;

  for (uint32_t i = 1; i <= devices_present; i++) {
    mask = 1 << (i -1);
    if (blink_mask & mask) {
      blink_mask &= (POWER_MASK ^ mask);  // Clear device mask
      MqttPublishPowerBlinkState(i);
      ExecuteCommandPower(i, (blink_powersave >> (i -1))&1, SRC_IGNORE);  // Restore state
    }
  }
}

void MqttShowPWMState(void)
{
  ResponseAppend_P(PSTR("\"" D_CMND_PWM "\":{"));
  bool first = true;
  for (uint32_t i = 0; i < MAX_PWMS; i++) {
    if (pin[GPIO_PWM1 + i] < 99) {
      ResponseAppend_P(PSTR("%s\"" D_CMND_PWM "%d\":%d"), first ? "" : ",", i+1, Settings.pwm_value[i]);
      first = false;
    }
  }
  ResponseJsonEnd();
}

void MqttShowState(void)
{
  char stemp1[33];

  ResponseAppendTime();
  ResponseAppend_P(PSTR(",\"" D_JSON_UPTIME "\":\"%s\",\"UptimeSec\":%u"), GetUptime().c_str(), UpTime());

#ifdef USE_ADC_VCC
  dtostrfd((double)ESP.getVcc()/1000, 3, stemp1);
  ResponseAppend_P(PSTR(",\"" D_JSON_VCC "\":%s"), stemp1);
#endif

  ResponseAppend_P(PSTR(",\"" D_JSON_HEAPSIZE "\":%d,\"SleepMode\":\"%s\",\"Sleep\":%u,\"LoadAvg\":%u,\"MqttCount\":%u"),
    ESP.getFreeHeap()/1024, GetTextIndexed(stemp1, sizeof(stemp1), Settings.flag3.sleep_normal, kSleepMode), sleep, loop_load_avg, MqttConnectCount());

  for (uint32_t i = 1; i <= devices_present; i++) {
#ifdef USE_LIGHT
    if ((LightDevice()) && (i >= LightDevice())) {
      if (i == LightDevice())  { LightState(1); }    // call it only once
    } else {
#endif
      ResponseAppend_P(PSTR(",\"%s\":\"%s\""), GetPowerDevice(stemp1, i, sizeof(stemp1), Settings.flag.device_index_enable), GetStateText(bitRead(power, i-1)));
#ifdef USE_SONOFF_IFAN
      if (IsModuleIfan()) {
        ResponseAppend_P(PSTR(",\"" D_CMND_FANSPEED "\":%d"), GetFanspeed());
        break;
      }
#endif  // USE_SONOFF_IFAN
#ifdef USE_LIGHT
    }
#endif
  }

  if (pwm_present) {
    ResponseAppend_P(PSTR(","));
    MqttShowPWMState();
  }

  ResponseAppend_P(PSTR(",\"" D_JSON_WIFI "\":{\"" D_JSON_AP "\":%d,\"" D_JSON_SSID "\":\"%s\",\"" D_JSON_BSSID "\":\"%s\",\"" D_JSON_CHANNEL "\":%d,\"" D_JSON_RSSI "\":%d,\"" D_JSON_LINK_COUNT "\":%d,\"" D_JSON_DOWNTIME "\":\"%s\"}}"),
    Settings.sta_active +1, Settings.sta_ssid[Settings.sta_active], WiFi.BSSIDstr().c_str(), WiFi.channel(), WifiGetRssiAsQuality(WiFi.RSSI()), WifiLinkCount(), WifiDowntime().c_str());
}

void MqttPublishTeleState(void)
{
  mqtt_data[0] = '\0';
  MqttShowState();
  MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_STATE), MQTT_TELE_RETAIN);
#ifdef USE_SCRIPT
  RulesTeleperiod();  // Allow rule based HA messages
#endif  // USE_SCRIPT
}

bool MqttShowSensor(void)
{
  ResponseAppendTime();

  int json_data_start = strlen(mqtt_data);
  for (uint32_t i = 0; i < MAX_SWITCHES; i++) {
#ifdef USE_TM1638
    if ((pin[GPIO_SWT1 +i] < 99) || ((pin[GPIO_TM16CLK] < 99) && (pin[GPIO_TM16DIO] < 99) && (pin[GPIO_TM16STB] < 99))) {
#else
    if (pin[GPIO_SWT1 +i] < 99) {
#endif  // USE_TM1638
      bool swm = ((FOLLOW_INV == Settings.switchmode[i]) || (PUSHBUTTON_INV == Settings.switchmode[i]) || (PUSHBUTTONHOLD_INV == Settings.switchmode[i]));
      ResponseAppend_P(PSTR(",\"" D_JSON_SWITCH "%d\":\"%s\""), i +1, GetStateText(swm ^ SwitchLastState(i)));
    }
  }
  XsnsCall(FUNC_JSON_APPEND);
  XdrvCall(FUNC_JSON_APPEND);

  bool json_data_available = (strlen(mqtt_data) - json_data_start);
  if (strstr_P(mqtt_data, PSTR(D_JSON_PRESSURE)) != nullptr) {
    ResponseAppend_P(PSTR(",\"" D_JSON_PRESSURE_UNIT "\":\"%s\""), PressureUnit().c_str());
  }
  if (strstr_P(mqtt_data, PSTR(D_JSON_TEMPERATURE)) != nullptr) {
    ResponseAppend_P(PSTR(",\"" D_JSON_TEMPERATURE_UNIT "\":\"%c\""), TempUnit());
  }
  ResponseJsonEnd();

  if (json_data_available) { XdrvCall(FUNC_SHOW_SENSOR); }
  return json_data_available;
}

/********************************************************************************************/

void PerformEverySecond(void)
{
  uptime++;

  if (ntp_synced_message) {
    // Moved here to fix syslog UDP exception 9 during RtcSecond
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("NTP: Drift %d, (" D_UTC_TIME ") %s, (" D_DST_TIME ") %s, (" D_STD_TIME ") %s"),
      DriftTime(), GetTime(0).c_str(), GetTime(2).c_str(), GetTime(3).c_str());
    ntp_synced_message = false;
  }

  if (POWER_CYCLE_TIME == uptime) {
    UpdateQuickPowerCycle(false);
  }

  if (BOOT_LOOP_TIME == uptime) {
    RtcReboot.fast_reboot_count = 0;
    RtcRebootSave();

    Settings.bootcount++;              // Moved to here to stop flash writes during start-up
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_BOOT_COUNT " %d"), Settings.bootcount);
  }

  if (seriallog_timer) {
    seriallog_timer--;
    if (!seriallog_timer) {
      if (seriallog_level) {
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_SERIAL_LOGGING_DISABLED));
      }
      seriallog_level = 0;
    }
  }

  if (syslog_timer) {  // Restore syslog level
    syslog_timer--;
    if (!syslog_timer) {
      syslog_level = Settings.syslog_level;
      if (Settings.syslog_level) {
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_SYSLOG_LOGGING_REENABLED));  // Might trigger disable again (on purpose)
      }
    }
  }

  ResetGlobalValues();

  if (Settings.tele_period) {
    tele_period++;
    // increase time for prepare and document state to ensure TELEPERIOD deliver results
    if (tele_period == Settings.tele_period -3 && !prep_called) {
      // sensores must be called later if driver switch on e.g. power on deepsleep
      XdrvCall(FUNC_PREP_BEFORE_TELEPERIOD);
      XsnsCall(FUNC_PREP_BEFORE_TELEPERIOD);
      prep_called = true;
    }
   if (tele_period >= Settings.tele_period && prep_called) {
      tele_period = 0;

      MqttPublishTeleState();

      mqtt_data[0] = '\0';
      if (MqttShowSensor()) {
        MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);
#if defined(USE_RULES) || defined(USE_SCRIPT)
        RulesTeleperiod();  // Allow rule based HA messages
#endif  // USE_RULES
      }
      prep_called = true;
      XdrvCall(FUNC_AFTER_TELEPERIOD);
    }
  }
}

/*********************************************************************************************\
 * State loops
\*********************************************************************************************/
/*-------------------------------------------------------------------------------------------*\
 * Every 0.1 second
\*-------------------------------------------------------------------------------------------*/

void Every100mSeconds(void)
{
  // As the max amount of sleep = 250 mSec this loop will shift in time...
  power_t power_now;

  if (latching_relay_pulse) {
    latching_relay_pulse--;
    if (!latching_relay_pulse) SetLatchingRelay(0, 0);
  }

  for (uint32_t i = 0; i < MAX_PULSETIMERS; i++) {
    if (pulse_timer[i] != 0L) {           // Timer active?
      if (TimeReached(pulse_timer[i])) {  // Timer finished?
        pulse_timer[i] = 0L;              // Turn off this timer
        ExecuteCommandPower(i +1, (POWER_ALL_OFF_PULSETIME_ON == Settings.poweronstate) ? POWER_ON : POWER_OFF, SRC_PULSETIMER);
      }
    }
  }

  if (blink_mask) {
    if (TimeReached(blink_timer)) {
      SetNextTimeInterval(blink_timer, 100 * Settings.blinktime);
      blink_counter--;
      if (!blink_counter) {
        StopAllPowerBlink();
      } else {
        blink_power ^= 1;
        power_now = (power & (POWER_MASK ^ blink_mask)) | ((blink_power) ? blink_mask : 0);
        SetDevicePower(power_now, SRC_IGNORE);
      }
    }
  }
}

/*-------------------------------------------------------------------------------------------*\
 * Every 0.25 second
\*-------------------------------------------------------------------------------------------*/

void Every250mSeconds(void)
{
// As the max amount of sleep = 250 mSec this loop should always be taken...

  uint32_t blinkinterval = 1;

  state_250mS++;
  state_250mS &= 0x3;

  if (mqtt_cmnd_publish) mqtt_cmnd_publish--;             // Clean up

  if (!Settings.flag.global_state) {                      // Problem blinkyblinky enabled
    if (global_state.data) {                              // Any problem
      if (global_state.mqtt_down) { blinkinterval = 7; }  // MQTT problem so blink every 2 seconds (slowest)
      if (global_state.wifi_down) { blinkinterval = 3; }  // Wifi problem so blink every second (slow)
      blinks = 201;                                       // Allow only a single blink in case the problem is solved
    }
  }
  if (blinks || restart_flag || ota_state_flag) {
    if (restart_flag || ota_state_flag) {                 // Overrule blinks and keep led lit
      blinkstate = true;                                  // Stay lit
    } else {
      blinkspeed--;
      if (!blinkspeed) {
        blinkspeed = blinkinterval;                       // Set interval to 0.2 (default), 1 or 2 seconds
        blinkstate ^= 1;                                  // Blink
      }
    }
    if ((!(Settings.ledstate &0x08)) && ((Settings.ledstate &0x06) || (blinks > 200) || (blinkstate))) {
      SetLedLink(blinkstate);                            // Set led on or off
    }
    if (!blinkstate) {
      blinks--;
      if (200 == blinks) blinks = 0;                      // Disable blink
    }
  }
  if (Settings.ledstate &1 && (pin[GPIO_LEDLNK] < 99 || !(blinks || restart_flag || ota_state_flag)) ) {
    bool tstate = power & Settings.ledmask;
    if ((SONOFF_TOUCH == my_module_type) || (SONOFF_T11 == my_module_type) || (SONOFF_T12 == my_module_type) || (SONOFF_T13 == my_module_type)) {
      tstate = (!power) ? 1 : 0;                          // As requested invert signal for Touch devices to find them in the dark
    }
    SetLedPower(tstate);
  }

/*-------------------------------------------------------------------------------------------*\
 * Every second at 0.25 second interval
\*-------------------------------------------------------------------------------------------*/

  switch (state_250mS) {
  case 0:                                                 // Every x.0 second
    if (ota_state_flag && BACKLOG_EMPTY) {
      ota_state_flag--;
      if (2 == ota_state_flag) {
        ota_url = Settings.ota_url;
        RtcSettings.ota_loader = 0;  // Try requested image first
        ota_retry_counter = OTA_ATTEMPTS;
        ESPhttpUpdate.rebootOnUpdate(false);
        SettingsSave(1);  // Free flash for OTA update
      }
      if (ota_state_flag <= 0) {
#ifdef USE_WEBSERVER
        if (Settings.webserver) StopWebserver();
#endif  // USE_WEBSERVER
#ifdef USE_ARILUX_RF
        AriluxRfDisable();  // Prevent restart exception on Arilux Interrupt routine
#endif  // USE_ARILUX_RF
        ota_state_flag = 92;
        ota_result = 0;
        ota_retry_counter--;
        if (ota_retry_counter) {
          strlcpy(mqtt_data, GetOtaUrl(log_data, sizeof(log_data)), sizeof(mqtt_data));
#ifndef FIRMWARE_MINIMAL
          if (RtcSettings.ota_loader) {
            char *bch = strrchr(mqtt_data, '/');                        // Only consider filename after last backslash prevent change of urls having "-" in it
            char *pch = strrchr((bch != nullptr) ? bch : mqtt_data, '-');  // Change from filename-DE.bin into filename-minimal.bin
            char *ech = strrchr((bch != nullptr) ? bch : mqtt_data, '.');  // Change from filename.bin into filename-minimal.bin
            if (!pch) { pch = ech; }
            if (pch) {
              mqtt_data[pch - mqtt_data] = '\0';
              char *ech = strrchr(Settings.ota_url, '.');  // Change from filename.bin into filename-minimal.bin
              snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s-" D_JSON_MINIMAL "%s"), mqtt_data, ech);  // Minimal filename must be filename-minimal
            }
          }
#endif  // FIRMWARE_MINIMAL
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPLOAD "%s"), mqtt_data);
#if defined(ARDUINO_ESP8266_RELEASE_2_3_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_1) || defined(ARDUINO_ESP8266_RELEASE_2_4_2)
          ota_result = (HTTP_UPDATE_FAILED != ESPhttpUpdate.update(mqtt_data));
#else
          // If using core stage or 2.5.0+ the syntax has changed
          WiFiClient OTAclient;
          ota_result = (HTTP_UPDATE_FAILED != ESPhttpUpdate.update(OTAclient, mqtt_data));
#endif
          if (!ota_result) {
#ifndef FIRMWARE_MINIMAL
            int ota_error = ESPhttpUpdate.getLastError();
            DEBUG_CORE_LOG(PSTR("OTA: Error %d"), ota_error);
            if ((HTTP_UE_TOO_LESS_SPACE == ota_error) || (HTTP_UE_BIN_FOR_WRONG_FLASH == ota_error)) {
              RtcSettings.ota_loader = 1;  // Try minimal image next
            }
#endif  // FIRMWARE_MINIMAL
            ota_state_flag = 2;    // Upgrade failed - retry
          }
        }
      }
      if (90 == ota_state_flag) {  // Allow MQTT to reconnect
        ota_state_flag = 0;
        if (ota_result) {
//          SetFlashModeDout();      // Force DOUT for both ESP8266 and ESP8285
          Response_P(PSTR(D_JSON_SUCCESSFUL ". " D_JSON_RESTARTING));
        } else {
          Response_P(PSTR(D_JSON_FAILED " %s"), ESPhttpUpdate.getLastErrorString().c_str());
        }
        restart_flag = 2;          // Restart anyway to keep memory clean webserver
        MqttPublishPrefixTopic_P(STAT, PSTR(D_CMND_UPGRADE));
      }
    }
    break;
  case 1:                                                 // Every x.25 second
    if (MidnightNow()) {
      XsnsCall(FUNC_SAVE_AT_MIDNIGHT);
    }
    if (save_data_counter && BACKLOG_EMPTY) {
      save_data_counter--;
      if (save_data_counter <= 0) {
        if (Settings.flag.save_state) {
          power_t mask = POWER_MASK;
          for (uint32_t i = 0; i < MAX_PULSETIMERS; i++) {
            if ((Settings.pulse_timer[i] > 0) && (Settings.pulse_timer[i] < 30)) {  // 3 seconds
              mask &= ~(1 << i);
            }
          }
          if (!((Settings.power &mask) == (power &mask))) {
            Settings.power = power;
          }
        } else {
          Settings.power = 0;
        }
        SettingsSave(0);
        save_data_counter = Settings.save_data;
      }
    }
    if (restart_flag && BACKLOG_EMPTY) {
      if ((214 == restart_flag) || (215 == restart_flag) || (216 == restart_flag)) {
        char storage_wifi[sizeof(Settings.sta_ssid) +
                          sizeof(Settings.sta_pwd)];
        char storage_mqtt[sizeof(Settings.mqtt_host) +
                          sizeof(Settings.mqtt_port) +
                          sizeof(Settings.mqtt_client) +
                          sizeof(Settings.mqtt_user) +
                          sizeof(Settings.mqtt_pwd) +
                          sizeof(Settings.mqtt_topic)];
        memcpy(storage_wifi, Settings.sta_ssid, sizeof(storage_wifi));     // Backup current SSIDs and Passwords
        if (216 == restart_flag) {
          memcpy(storage_mqtt, Settings.mqtt_host, sizeof(storage_mqtt));  // Backup mqtt host, port, client, username and password
        }
        if ((215 == restart_flag) || (216 == restart_flag)) {
          SettingsErase(0);  // Erase all flash from program end to end of physical flash
        }
        SettingsDefault();
        memcpy(Settings.sta_ssid, storage_wifi, sizeof(storage_wifi));     // Restore current SSIDs and Passwords
        if (216 == restart_flag) {
          memcpy(Settings.mqtt_host, storage_mqtt, sizeof(storage_mqtt));  // Restore the mqtt host, port, client, username and password
          strlcpy(Settings.mqtt_client, MQTT_CLIENT_ID, sizeof(Settings.mqtt_client));  // Set client to default
        }
        restart_flag = 2;
      }
      else if (213 == restart_flag) {
        SettingsSdkErase();  // Erase flash SDK parameters
        restart_flag = 2;
      }
      else if (212 == restart_flag) {
        SettingsErase(0);    // Erase all flash from program end to end of physical flash
        restart_flag = 211;
      }
      if (211 == restart_flag) {
        SettingsDefault();
        restart_flag = 2;
      }
      if (2 == restart_flag) {
        SettingsSaveAll();
      }
      restart_flag--;
      if (restart_flag <= 0) {
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_RESTARTING));
        EspRestart();
      }
    }
    break;
  case 2:                                                 // Every x.5 second
    WifiCheck(wifi_state_flag);
    wifi_state_flag = WIFI_RESTART;
    break;
  case 3:                                                 // Every x.75 second
    if (!global_state.wifi_down) { MqttCheck(); }
    break;
  }
}

#ifdef USE_ARDUINO_OTA
/*********************************************************************************************\
 * Allow updating via the Arduino OTA-protocol.
 *
 * - Once started disables current wifi clients and udp
 * - Perform restart when done to re-init wifi clients
\*********************************************************************************************/

bool arduino_ota_triggered = false;
uint16_t arduino_ota_progress_dot_count = 0;

void ArduinoOTAInit(void)
{
  ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname(my_hostname);
  if (Settings.web_password[0] !=0) { ArduinoOTA.setPassword(Settings.web_password); }

  ArduinoOTA.onStart([]()
  {
    SettingsSave(1);  // Free flash for OTA update
#ifdef USE_WEBSERVER
    if (Settings.webserver) { StopWebserver(); }
#endif  // USE_WEBSERVER
#ifdef USE_ARILUX_RF
    AriluxRfDisable();  // Prevent restart exception on Arilux Interrupt routine
#endif  // USE_ARILUX_RF
    if (Settings.flag.mqtt_enabled) { MqttDisconnect(); }
    AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "Arduino OTA " D_UPLOAD_STARTED));
    arduino_ota_triggered = true;
    arduino_ota_progress_dot_count = 0;
    delay(100);       // Allow time for message xfer
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
  {
    if ((LOG_LEVEL_DEBUG <= seriallog_level)) {
      arduino_ota_progress_dot_count++;
      Serial.printf(".");
      if (!(arduino_ota_progress_dot_count % 80)) { Serial.println(); }
    }
  });

  ArduinoOTA.onError([](ota_error_t error)
  {
    /*
    From ArduinoOTA.h:
    typedef enum { OTA_AUTH_ERROR, OTA_BEGIN_ERROR, OTA_CONNECT_ERROR, OTA_RECEIVE_ERROR, OTA_END_ERROR } ota_error_t;
    */
    char error_str[100];

    if ((LOG_LEVEL_DEBUG <= seriallog_level) && arduino_ota_progress_dot_count) { Serial.println(); }
    switch (error) {
      case OTA_BEGIN_ERROR: strncpy_P(error_str, PSTR(D_UPLOAD_ERR_2), sizeof(error_str)); break;
      case OTA_RECEIVE_ERROR: strncpy_P(error_str, PSTR(D_UPLOAD_ERR_5), sizeof(error_str)); break;
      case OTA_END_ERROR: strncpy_P(error_str, PSTR(D_UPLOAD_ERR_7), sizeof(error_str)); break;
      default:
        snprintf_P(error_str, sizeof(error_str), PSTR(D_UPLOAD_ERROR_CODE " %d"), error);
    }
    AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "Arduino OTA  %s. " D_RESTARTING), error_str);
    EspRestart();
  });

  ArduinoOTA.onEnd([]()
  {
    if ((LOG_LEVEL_DEBUG <= seriallog_level)) { Serial.println(); }
    AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "Arduino OTA " D_SUCCESSFUL ". " D_RESTARTING));
    EspRestart();
	});

  ArduinoOTA.begin();
  AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "Arduino OTA " D_ENABLED " " D_PORT " 8266"));
}
#endif  // USE_ARDUINO_OTA

/********************************************************************************************/

void SerialInput(void)
{
  while (Serial.available()) {
//    yield();
    delay(0);
    serial_in_byte = Serial.read();

/*-------------------------------------------------------------------------------------------*\
 * Sonoff dual and ch4 19200 baud serial interface
\*-------------------------------------------------------------------------------------------*/
    if ((SONOFF_DUAL == my_module_type) || (CH4 == my_module_type)) {
      serial_in_byte = ButtonSerial(serial_in_byte);
    }

/*-------------------------------------------------------------------------------------------*/

    if (XdrvCall(FUNC_SERIAL)) {
      serial_in_byte_counter = 0;
      Serial.flush();
      return;
    }

/*-------------------------------------------------------------------------------------------*/

    if (serial_in_byte > 127 && !Settings.flag.mqtt_serial_raw) {                // Discard binary data above 127 if no raw reception allowed
      serial_in_byte_counter = 0;
      Serial.flush();
      return;
    }
    if (!Settings.flag.mqtt_serial) {                                            // SerialSend active
      if (isprint(serial_in_byte)) {                                             // Any char between 32 and 127
        if (serial_in_byte_counter < INPUT_BUFFER_SIZE -1) {                     // Add char to string if it still fits
          serial_in_buffer[serial_in_byte_counter++] = serial_in_byte;
        } else {
          serial_in_byte_counter = 0;
        }
      }
    } else {
      if (serial_in_byte || Settings.flag.mqtt_serial_raw) {                     // Any char between 1 and 127 or any char (0 - 255)
        if ((serial_in_byte_counter < INPUT_BUFFER_SIZE -1) &&                   // Add char to string if it still fits and ...
            ((isprint(serial_in_byte) && (128 == Settings.serial_delimiter)) ||  // Any char between 32 and 127
            ((serial_in_byte != Settings.serial_delimiter) && (128 != Settings.serial_delimiter)) ||  // Any char between 1 and 127 and not being delimiter
              Settings.flag.mqtt_serial_raw)) {                                  // Any char between 0 and 255
          serial_in_buffer[serial_in_byte_counter++] = serial_in_byte;
          serial_polling_window = millis();
        } else {
          serial_polling_window = 0;                                             // Reception done - send mqtt
          break;
        }
      }
    }

#ifdef USE_SONOFF_SC
/*-------------------------------------------------------------------------------------------*\
 * Sonoff SC 19200 baud serial interface
\*-------------------------------------------------------------------------------------------*/
    if (SONOFF_SC == my_module_type) {
      if (serial_in_byte == '\x1B') {                                            // Sonoff SC status from ATMEGA328P
        serial_in_buffer[serial_in_byte_counter] = 0;                            // Serial data completed
        SonoffScSerialInput(serial_in_buffer);
        serial_in_byte_counter = 0;
        Serial.flush();
        return;
      }
    } else
#endif  // USE_SONOFF_SC
/*-------------------------------------------------------------------------------------------*/

    if (!Settings.flag.mqtt_serial && (serial_in_byte == '\n')) {
      serial_in_buffer[serial_in_byte_counter] = 0;                              // Serial data completed
      seriallog_level = (Settings.seriallog_level < LOG_LEVEL_INFO) ? (uint8_t)LOG_LEVEL_INFO : Settings.seriallog_level;
      AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_COMMAND "%s"), serial_in_buffer);
      ExecuteCommand(serial_in_buffer, SRC_SERIAL);
      serial_in_byte_counter = 0;
      serial_polling_window = 0;
      Serial.flush();
      return;
    }
  }

  if (Settings.flag.mqtt_serial && serial_in_byte_counter && (millis() > (serial_polling_window + SERIAL_POLLING))) {
    serial_in_buffer[serial_in_byte_counter] = 0;                                // Serial data completed
    char hex_char[(serial_in_byte_counter * 2) + 2];
    Response_P(PSTR(",\"" D_JSON_SERIALRECEIVED "\":\"%s\"}"),
      (Settings.flag.mqtt_serial_raw) ? ToHex_P((unsigned char*)serial_in_buffer, serial_in_byte_counter, hex_char, sizeof(hex_char)) : serial_in_buffer);
    MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_SERIALRECEIVED));
    XdrvRulesProcess();
    serial_in_byte_counter = 0;
  }
}

/********************************************************************************************/

void GpioInit(void)
{
  uint32_t mpin;

  if (!ValidModule(Settings.module)) {
    uint32_t module = MODULE;
    if (!ValidModule(MODULE)) { module = SONOFF_BASIC; }
    Settings.module = module;
    Settings.last_module = module;
  }
  SetModuleType();

  if (Settings.module != Settings.last_module) {
    baudrate = APP_BAUDRATE;
  }

  for (uint32_t i = 0; i < sizeof(Settings.user_template.gp); i++) {
    if ((Settings.user_template.gp.io[i] >= GPIO_SENSOR_END) && (Settings.user_template.gp.io[i] < GPIO_USER)) {
      Settings.user_template.gp.io[i] = GPIO_USER;  // Fix not supported sensor ids in template
    }
  }

  myio def_gp;
  ModuleGpios(&def_gp);
  for (uint32_t i = 0; i < sizeof(Settings.my_gp); i++) {
    if ((Settings.my_gp.io[i] >= GPIO_SENSOR_END) && (Settings.my_gp.io[i] < GPIO_USER)) {
      Settings.my_gp.io[i] = GPIO_NONE;             // Fix not supported sensor ids in module
    }
    else if (Settings.my_gp.io[i] > GPIO_NONE) {
      my_module.io[i] = Settings.my_gp.io[i];       // Set User selected Module sensors
    }
    if ((def_gp.io[i] > GPIO_NONE) && (def_gp.io[i] < GPIO_USER)) {
      my_module.io[i] = def_gp.io[i];               // Force Template override
    }
  }
  if ((Settings.my_adc0 >= ADC0_END) && (Settings.my_adc0 < ADC0_USER)) {
    Settings.my_adc0 = ADC0_NONE;                   // Fix not supported sensor ids in module
  }
  else if (Settings.my_adc0 > ADC0_NONE) {
    my_adc0 = Settings.my_adc0;                     // Set User selected Module sensors
  }
  my_module_flag = ModuleFlag();
  uint32_t template_adc0 = my_module_flag.data &15;
  if ((template_adc0 > ADC0_NONE) && (template_adc0 < ADC0_USER)) {
    my_adc0 = template_adc0;                        // Force Template override
  }

  for (uint32_t i = 0; i < GPIO_MAX; i++) {
    pin[i] = 99;
  }
  for (uint32_t i = 0; i < sizeof(my_module.io); i++) {
    mpin = ValidPin(i, my_module.io[i]);

    DEBUG_CORE_LOG(PSTR("INI: gpio pin %d, mpin %d"), i, mpin);

    if (mpin) {
      XdrvMailbox.index = mpin;
      XdrvMailbox.payload = i;

      if ((mpin >= GPIO_SWT1_NP) && (mpin < (GPIO_SWT1_NP + MAX_SWITCHES))) {
        SwitchPullupFlag(mpin - GPIO_SWT1_NP);
        mpin -= (GPIO_SWT1_NP - GPIO_SWT1);
      }
      else if ((mpin >= GPIO_KEY1_NP) && (mpin < (GPIO_KEY1_NP + MAX_KEYS))) {
        ButtonPullupFlag(mpin - GPIO_KEY1_NP);       //  0 .. 3
        mpin -= (GPIO_KEY1_NP - GPIO_KEY1);
      }
      else if ((mpin >= GPIO_KEY1_INV) && (mpin < (GPIO_KEY1_INV + MAX_KEYS))) {
        ButtonInvertFlag(mpin - GPIO_KEY1_INV);      //  0 .. 3
        mpin -= (GPIO_KEY1_INV - GPIO_KEY1);
      }
      else if ((mpin >= GPIO_KEY1_INV_NP) && (mpin < (GPIO_KEY1_INV_NP + MAX_KEYS))) {
        ButtonPullupFlag(mpin - GPIO_KEY1_INV_NP);   //  0 .. 3
        ButtonInvertFlag(mpin - GPIO_KEY1_INV_NP);   //  0 .. 3
        mpin -= (GPIO_KEY1_INV_NP - GPIO_KEY1);
      }
      else if ((mpin >= GPIO_REL1_INV) && (mpin < (GPIO_REL1_INV + MAX_RELAYS))) {
        bitSet(rel_inverted, mpin - GPIO_REL1_INV);
        mpin -= (GPIO_REL1_INV - GPIO_REL1);
      }
      else if ((mpin >= GPIO_LED1_INV) && (mpin < (GPIO_LED1_INV + MAX_LEDS))) {
        bitSet(led_inverted, mpin - GPIO_LED1_INV);
        mpin -= (GPIO_LED1_INV - GPIO_LED1);
      }
      else if (mpin == GPIO_LEDLNK_INV) {
        ledlnk_inverted = 1;
        mpin -= (GPIO_LEDLNK_INV - GPIO_LEDLNK);
      }
      else if ((mpin >= GPIO_PWM1_INV) && (mpin < (GPIO_PWM1_INV + MAX_PWMS))) {
        bitSet(pwm_inverted, mpin - GPIO_PWM1_INV);
        mpin -= (GPIO_PWM1_INV - GPIO_PWM1);
      }
      else if (XdrvCall(FUNC_PIN_STATE)) {
        mpin = XdrvMailbox.index;
      }
      else if (XsnsCall(FUNC_PIN_STATE)) {
        mpin = XdrvMailbox.index;
      };
    }
    if (mpin) pin[mpin] = i;
  }

  if ((2 == pin[GPIO_TXD]) || (H801 == my_module_type)) { Serial.set_tx(2); }

  analogWriteRange(Settings.pwm_range);      // Default is 1023 (Arduino.h)
  analogWriteFreq(Settings.pwm_frequency);   // Default is 1000 (core_esp8266_wiring_pwm.c)

#ifdef USE_SPI
  spi_flg = ((((pin[GPIO_SPI_CS] < 99) && (pin[GPIO_SPI_CS] > 14)) || (pin[GPIO_SPI_CS] < 12)) || (((pin[GPIO_SPI_DC] < 99) && (pin[GPIO_SPI_DC] > 14)) || (pin[GPIO_SPI_DC] < 12)));
  if (spi_flg) {
    for (uint32_t i = 0; i < GPIO_MAX; i++) {
      if ((pin[i] >= 12) && (pin[i] <=14)) pin[i] = 99;
    }
    my_module.io[12] = GPIO_SPI_MISO;
    pin[GPIO_SPI_MISO] = 12;
    my_module.io[13] = GPIO_SPI_MOSI;
    pin[GPIO_SPI_MOSI] = 13;
    my_module.io[14] = GPIO_SPI_CLK;
    pin[GPIO_SPI_CLK] = 14;
  }
  soft_spi_flg = ((pin[GPIO_SSPI_CS] < 99) && (pin[GPIO_SSPI_SCLK] < 99) && ((pin[GPIO_SSPI_MOSI] < 99) || (pin[GPIO_SSPI_MOSI] < 99)));
#endif  // USE_SPI

#ifdef USE_I2C
  i2c_flg = ((pin[GPIO_I2C_SCL] < 99) && (pin[GPIO_I2C_SDA] < 99));
  if (i2c_flg) {
    Wire.begin(pin[GPIO_I2C_SDA], pin[GPIO_I2C_SCL]);
  }
#endif  // USE_I2C

  devices_present = 0;
  light_type = LT_BASIC;                     // Use basic PWM control if SetOption15 = 0
  if (XdrvCall(FUNC_MODULE_INIT)) {
    // Serviced
  }
  else if (YTF_IR_BRIDGE == my_module_type) {
    ClaimSerial();  // Stop serial loopback mode
//    devices_present = 1;
  }
  else if (SONOFF_DUAL == my_module_type) {
    Settings.flag.mqtt_serial = 0;
    devices_present = 2;
    baudrate = 19200;
  }
  else if (CH4 == my_module_type) {
    Settings.flag.mqtt_serial = 0;
    devices_present = 4;
    baudrate = 19200;
  }
#ifdef USE_SONOFF_SC
  else if (SONOFF_SC == my_module_type) {
    Settings.flag.mqtt_serial = 0;
    devices_present = 0;
    baudrate = 19200;
  }
#endif  // USE_SONOFF_SC

  if (!light_type) {
    for (uint32_t i = 0; i < MAX_PWMS; i++) {     // Basic PWM control only
      if (pin[GPIO_PWM1 +i] < 99) {
        pwm_present = true;
        pinMode(pin[GPIO_PWM1 +i], OUTPUT);
        analogWrite(pin[GPIO_PWM1 +i], bitRead(pwm_inverted, i) ? Settings.pwm_range - Settings.pwm_value[i] : Settings.pwm_value[i]);
      }
    }
  }
  for (uint32_t i = 0; i < MAX_RELAYS; i++) {
    if (pin[GPIO_REL1 +i] < 99) {
      pinMode(pin[GPIO_REL1 +i], OUTPUT);
      devices_present++;
      if (EXS_RELAY == my_module_type) {
        digitalWrite(pin[GPIO_REL1 +i], bitRead(rel_inverted, i) ? 1 : 0);
        if (i &1) { devices_present--; }
      }
    }
  }

  for (uint32_t i = 0; i < MAX_LEDS; i++) {
    if (pin[GPIO_LED1 +i] < 99) {
#ifdef USE_ARILUX_RF
      if ((3 == i) && (leds_present < 2) && (99 == pin[GPIO_ARIRFSEL])) {
        pin[GPIO_ARIRFSEL] = pin[GPIO_LED4];  // Legacy support where LED4 was Arilux RF enable
        pin[GPIO_LED4] = 99;
      } else {
#endif
        pinMode(pin[GPIO_LED1 +i], OUTPUT);
        leds_present++;
        digitalWrite(pin[GPIO_LED1 +i], bitRead(led_inverted, i));
#ifdef USE_ARILUX_RF
      }
#endif
    }
  }
  if (pin[GPIO_LEDLNK] < 99) {
    pinMode(pin[GPIO_LEDLNK], OUTPUT);
    digitalWrite(pin[GPIO_LEDLNK], ledlnk_inverted);
  }

  ButtonInit();
  SwitchInit();
#ifdef ROTARY_V1
  RotaryInit();
#endif

  SetLedPower(Settings.ledstate &8);
  SetLedLink(Settings.ledstate &8);

  XdrvCall(FUNC_PRE_INIT);
}

extern "C" {
extern struct rst_info resetInfo;
}

void setup(void)
{
  global_state.data = 3;  // Init global state (wifi_down, mqtt_down) to solve possible network issues

  RtcRebootLoad();
  if (!RtcRebootValid()) {
    RtcReboot.fast_reboot_count = 0;
  }
  RtcReboot.fast_reboot_count++;
  RtcRebootSave();

  Serial.begin(baudrate);
  delay(10);
  Serial.println();
  seriallog_level = LOG_LEVEL_INFO;  // Allow specific serial messages until config loaded

  snprintf_P(my_version, sizeof(my_version), PSTR("%d.%d.%d"), VERSION >> 24 & 0xff, VERSION >> 16 & 0xff, VERSION >> 8 & 0xff);  // Release version 6.3.0
  if (VERSION & 0xff) {  // Development or patched version 6.3.0.10
    snprintf_P(my_version, sizeof(my_version), PSTR("%s.%d"), my_version, VERSION & 0xff);
  }
  char code_image[20];
  snprintf_P(my_image, sizeof(my_image), PSTR("(%s)"), GetTextIndexed(code_image, sizeof(code_image), CODE_IMAGE, kCodeImage));

  SettingsLoad();
  SettingsDelta();

  OsWatchInit();

  GetFeatures();

  if (1 == RtcReboot.fast_reboot_count) {  // Allow setting override only when all is well
    UpdateQuickPowerCycle(true);
    XdrvCall(FUNC_SETTINGS_OVERRIDE);
  }

  baudrate = Settings.baudrate * 300;
//  mdns_delayed_start = Settings.param[P_MDNS_DELAYED_START];
  seriallog_level = Settings.seriallog_level;
  seriallog_timer = SERIALLOG_TIMER;
  syslog_level = Settings.syslog_level;
  stop_flash_rotate = Settings.flag.stop_flash_rotate;
  save_data_counter = Settings.save_data;
  sleep = Settings.sleep;
#ifndef USE_EMULATION
  Settings.flag2.emulation = 0;
#else
#ifndef USE_EMULATION_WEMO
  if (EMUL_WEMO == Settings.flag2.emulation) { Settings.flag2.emulation = 0; }
#endif
#ifndef USE_EMULATION_HUE
  if (EMUL_HUE == Settings.flag2.emulation) { Settings.flag2.emulation = 0; }
#endif
#endif  // USE_EMULATION

  if (Settings.param[P_BOOT_LOOP_OFFSET]) {
    // Disable functionality as possible cause of fast restart within BOOT_LOOP_TIME seconds (Exception, WDT or restarts)
    if (RtcReboot.fast_reboot_count > Settings.param[P_BOOT_LOOP_OFFSET]) {       // Restart twice
      Settings.flag3.user_esp8285_enable = 0;       // Disable ESP8285 Generic GPIOs interfering with flash SPI
      if (RtcReboot.fast_reboot_count > Settings.param[P_BOOT_LOOP_OFFSET] +1) {  // Restart 3 times
        for (uint32_t i = 0; i < MAX_RULE_SETS; i++) {
          if (bitRead(Settings.rule_stop, i)) {
            bitWrite(Settings.rule_enabled, i, 0);  // Disable rules causing boot loop
          }
        }
      }
      if (RtcReboot.fast_reboot_count > Settings.param[P_BOOT_LOOP_OFFSET] +2) {  // Restarted 4 times
        Settings.rule_enabled = 0;                  // Disable all rules
      }
      if (RtcReboot.fast_reboot_count > Settings.param[P_BOOT_LOOP_OFFSET] +3) {  // Restarted 5 times
        for (uint32_t i = 0; i < sizeof(Settings.my_gp); i++) {
          Settings.my_gp.io[i] = GPIO_NONE;         // Reset user defined GPIO disabling sensors
        }
        Settings.my_adc0 = ADC0_NONE;               // Reset user defined ADC0 disabling sensors
      }
      if (RtcReboot.fast_reboot_count > Settings.param[P_BOOT_LOOP_OFFSET] +4) {  // Restarted 6 times
        Settings.module = SONOFF_BASIC;             // Reset module to Sonoff Basic
  //      Settings.last_module = SONOFF_BASIC;
      }
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_LOG_SOME_SETTINGS_RESET " (%d)"), RtcReboot.fast_reboot_count);
    }
  }

  Format(mqtt_client, Settings.mqtt_client, sizeof(mqtt_client));
  Format(mqtt_topic, Settings.mqtt_topic, sizeof(mqtt_topic));
  if (strstr(Settings.hostname, "%") != nullptr) {
    strlcpy(Settings.hostname, WIFI_HOSTNAME, sizeof(Settings.hostname));
    snprintf_P(my_hostname, sizeof(my_hostname)-1, Settings.hostname, mqtt_topic, ESP.getChipId() & 0x1FFF);
  } else {
    snprintf_P(my_hostname, sizeof(my_hostname)-1, Settings.hostname);
  }

  GpioInit();

  SetSerialBaudrate(baudrate);

  WifiConnect();

  if (MOTOR == my_module_type) { Settings.poweronstate = POWER_ALL_ON; }  // Needs always on else in limbo!
  if (POWER_ALL_ALWAYS_ON == Settings.poweronstate) {
    SetDevicePower(1, SRC_RESTART);
  } else {
    if ((resetInfo.reason == REASON_DEFAULT_RST) || (resetInfo.reason == REASON_EXT_SYS_RST)) {
      switch (Settings.poweronstate) {
      case POWER_ALL_OFF:
      case POWER_ALL_OFF_PULSETIME_ON:
        power = 0;
        SetDevicePower(power, SRC_RESTART);
        break;
      case POWER_ALL_ON:  // All on
        power = (1 << devices_present) -1;
        SetDevicePower(power, SRC_RESTART);
        break;
      case POWER_ALL_SAVED_TOGGLE:
        power = (Settings.power & ((1 << devices_present) -1)) ^ POWER_MASK;
        if (Settings.flag.save_state) {
          SetDevicePower(power, SRC_RESTART);
        }
        break;
      case POWER_ALL_SAVED:
        power = Settings.power & ((1 << devices_present) -1);
        if (Settings.flag.save_state) {
          SetDevicePower(power, SRC_RESTART);
        }
        break;
      }
    } else {
      power = Settings.power & ((1 << devices_present) -1);
      if (Settings.flag.save_state) {
        SetDevicePower(power, SRC_RESTART);
      }
    }
  }

  // Issue #526 and #909
  for (uint32_t i = 0; i < devices_present; i++) {
    if (!Settings.flag3.no_power_feedback) {  // #5594 and #5663
      if ((i < MAX_RELAYS) && (pin[GPIO_REL1 +i] < 99)) {
        bitWrite(power, i, digitalRead(pin[GPIO_REL1 +i]) ^ bitRead(rel_inverted, i));
      }
    }
    if ((i < MAX_PULSETIMERS) && (bitRead(power, i) || (POWER_ALL_OFF_PULSETIME_ON == Settings.poweronstate))) {
      SetPulseTimer(i, Settings.pulse_timer[i]);
    }
  }
  blink_powersave = power;

  AddLog_P2(LOG_LEVEL_INFO, PSTR(D_PROJECT " %s %s " D_VERSION " %s%s-" ARDUINO_ESP8266_RELEASE), PROJECT, Settings.friendlyname[0], my_version, my_image);
#ifdef FIRMWARE_MINIMAL
  AddLog_P2(LOG_LEVEL_INFO, PSTR(D_WARNING_MINIMAL_VERSION));
#endif  // FIRMWARE_MINIMAL

  RtcInit();

#ifdef USE_ARDUINO_OTA
  ArduinoOTAInit();
#endif  // USE_ARDUINO_OTA

  XdrvCall(FUNC_INIT);
  XsnsCall(FUNC_INIT);
}

static void BacklogLoop()
{
  if (TimeReached(backlog_delay)) {
    if (!BACKLOG_EMPTY && !backlog_mutex) {
      backlog_mutex = true;
#ifdef SUPPORT_IF_STATEMENT
      ExecuteCommand((char*)backlog.shift().c_str(), SRC_BACKLOG);
#else
      ExecuteCommand((char*)backlog[backlog_pointer].c_str(), SRC_BACKLOG);
      backlog_pointer++;
      if (backlog_pointer >= MAX_BACKLOG) { backlog_pointer = 0; }
#endif
      backlog_mutex = false;
    }
  }
}

void loop(void)
{
  uint32_t my_sleep = millis();

  XdrvCall(FUNC_LOOP);
  XsnsCall(FUNC_LOOP);

  OsWatchLoop();

  ButtonLoop();
  SwitchLoop();
#ifdef ROTARY_V1
  RotaryLoop();
#endif
  BacklogLoop();

  if (TimeReached(state_50msecond)) {
    SetNextTimeInterval(state_50msecond, 50);
    XdrvCall(FUNC_EVERY_50_MSECOND);
    XsnsCall(FUNC_EVERY_50_MSECOND);
  }
  if (TimeReached(state_100msecond)) {
    SetNextTimeInterval(state_100msecond, 100);
    Every100mSeconds();
    XdrvCall(FUNC_EVERY_100_MSECOND);
    XsnsCall(FUNC_EVERY_100_MSECOND);
  }
  if (TimeReached(state_250msecond)) {
    SetNextTimeInterval(state_250msecond, 250);
    Every250mSeconds();
    XdrvCall(FUNC_EVERY_250_MSECOND);
    XsnsCall(FUNC_EVERY_250_MSECOND);
  }
  if (TimeReached(state_second)) {
    SetNextTimeInterval(state_second, 1000);
    PerformEverySecond();
    XdrvCall(FUNC_EVERY_SECOND);
    XsnsCall(FUNC_EVERY_SECOND);
  }

  if (!serial_local) { SerialInput(); }

#ifdef USE_ARDUINO_OTA
  MDNS.update();
  ArduinoOTA.handle();
  // Once OTA is triggered, only handle that and dont do other stuff. (otherwise it fails)
  while (arduino_ota_triggered) ArduinoOTA.handle();
#endif  // USE_ARDUINO_OTA

  uint32_t my_activity = millis() - my_sleep;

  if (Settings.flag3.sleep_normal) {
    //  yield();       // yield == delay(0), delay contains yield, auto yield in loop
    delay(sleep);  // https://github.com/esp8266/Arduino/issues/2021
  } else {
    if (my_activity < (uint32_t)sleep) {
      delay((uint32_t)sleep - my_activity);  // Provide time for background tasks like wifi
    } else {
      if (global_state.wifi_down) {
        delay(my_activity /2); // If wifi down and my_activity > setoption36 then force loop delay to 1/3 of my_activity period
      }
    }
  }

  if (!my_activity) { my_activity++; }            // We cannot divide by 0
  uint32_t loop_delay = sleep;
  if (!loop_delay) { loop_delay++; }              // We cannot divide by 0
  uint32_t loops_per_second = 1000 / loop_delay;  // We need to keep track of this many loops per second
  uint32_t this_cycle_ratio = 100 * my_activity / loop_delay;
  loop_load_avg = loop_load_avg - (loop_load_avg / loops_per_second) + (this_cycle_ratio / loops_per_second); // Take away one loop average away and add the new one
}
