/*
  tasmota.ino - Tasmota firmware for iTead Sonoff, Wemos and NodeMCU hardware

  Copyright (C) 2020  Theo Arends

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
        #define MQTT_MAX_PACKET_SIZE 1200

  Arduino IDE 1.8.12 and up parameters
    - Select IDE Tools - Board: "Generic ESP8266 Module"
    - Select IDE Tools - Flash Mode: "DOUT (compatible)"
    - Select IDE Tools - Flash Size: "1M (FS:none OTA:~502KB)"
    - Select IDE Tools - LwIP Variant: "v2 Higher Bandwidth (no feature)"
    - Select IDE Tools - VTables: "Flash"
    - Select IDE Tools - Espressif FW: "nonos-sdk-2.2.1+100 (190703)"
  ====================================================*/

// Location specific includes
#include <core_version.h>                   // Arduino_Esp8266 version information (ARDUINO_ESP8266_RELEASE and ARDUINO_ESP8266_RELEASE_2_3_0)
#include "tasmota_compat.h"
#include "tasmota_version.h"                // Tasmota version information
#include "tasmota.h"                        // Enumeration used in my_user_config.h
#include "my_user_config.h"                 // Fixed user configurable options
#ifdef USE_MQTT_TLS
  #include <t_bearssl.h>                    // We need to include before "tasmota_globals.h" to take precedence over the BearSSL version in Arduino
#endif  // USE_MQTT_TLS
#include "tasmota_globals.h"                // Function prototypes and global configuration
#include "i18n.h"                           // Language support configured by my_user_config.h
#include "tasmota_template.h"               // Hardware configuration

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

/*********************************************************************************************\
 * Global variables
\*********************************************************************************************/

WiFiUDP PortUdp;                            // UDP Syslog and Alexa

unsigned long feature_drv1;                 // Compiled driver feature map
unsigned long feature_drv2;                 // Compiled driver feature map
unsigned long feature_sns1;                 // Compiled sensor feature map
unsigned long feature_sns2;                 // Compiled sensor feature map
unsigned long feature5;                     // Compiled feature map
unsigned long feature6;                     // Compiled feature map
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
int serial_in_byte_counter = 0;             // Index in receive buffer
int ota_state_flag = 0;                     // OTA state flag
int ota_result = 0;                         // OTA result
int restart_flag = 0;                       // Tasmota restart flag
int wifi_state_flag = WIFI_RESTART;         // Wifi state flag
int blinks = 201;                           // Number of LED blinks
uint32_t uptime = 0;                        // Counting every second until 4294967295 = 130 year
uint32_t loop_load_avg = 0;                 // Indicative loop load average
uint32_t global_update = 0;                 // Timestamp of last global temperature and humidity update
uint32_t web_log_index = 1;                 // Index in Web log buffer (should never be 0)
float global_temperature = 9999;            // Provide a global temperature to be used by some sensors
float global_humidity = 0;                  // Provide a global humidity to be used by some sensors
float global_pressure = 0;                  // Provide a global pressure to be used by some sensors
uint16_t tele_period = 9999;                // Tele period timer
uint16_t blink_counter = 0;                 // Number of blink cycles
uint16_t seriallog_timer = 0;               // Timer to disable Seriallog
uint16_t syslog_timer = 0;                  // Timer to re-enable syslog_level
int16_t save_data_counter;                  // Counter and flag for config save to Flash
RulesBitfield rules_flag;                   // Rule state flags (16 bits)
uint8_t mqtt_cmnd_blocked = 0;              // Ignore flag for publish command
uint8_t mqtt_cmnd_blocked_reset = 0;        // Count down to reset if needed
uint8_t state_250mS = 0;                    // State 250msecond per second flag
uint8_t latching_relay_pulse = 0;           // Latching relay pulse timer
uint8_t ssleep;                             // Current copy of Settings.sleep
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
uint8_t prepped_loglevel = 0;               // Delayed log level message
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
bool is_8285 = false;                       // Hardware device ESP8266EX (0) or ESP8285 (1)
bool skip_light_fade;                       // Temporarily skip light fading
myio my_module;                             // Active copy of Module GPIOs (17 x 8 bits)
gpio_flag my_module_flag;                   // Active copy of Template GPIO flags
StateBitfield global_state;                 // Global states (currently Wifi and Mqtt) (8 bits)
char my_version[33];                        // Composed version string
char my_image[33];                          // Code image and/or commit
char my_hostname[33];                       // Composed Wifi hostname
char mqtt_client[TOPSZ];                    // Composed MQTT Clientname
char mqtt_topic[TOPSZ];                     // Composed MQTT topic
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

/*********************************************************************************************\
 * Main
\*********************************************************************************************/

void setup(void)
{
#ifdef ESP32
#ifdef DISABLE_ESP32_BROWNOUT
  DisableBrownout();      // Workaround possible weak LDO resulting in brownout detection during Wifi connection
#endif
#endif

  global_state.data = 3;  // Init global state (wifi_down, mqtt_down) to solve possible network issues

  RtcRebootLoad();
  if (!RtcRebootValid()) {
    RtcReboot.fast_reboot_count = 0;
  }
  RtcReboot.fast_reboot_count++;
  RtcRebootSave();

  Serial.begin(APP_BAUDRATE);
  seriallog_level = LOG_LEVEL_INFO;  // Allow specific serial messages until config loaded

  snprintf_P(my_version, sizeof(my_version), PSTR("%d.%d.%d"), VERSION >> 24 & 0xff, VERSION >> 16 & 0xff, VERSION >> 8 & 0xff);  // Release version 6.3.0
  if (VERSION & 0xff) {  // Development or patched version 6.3.0.10
    snprintf_P(my_version, sizeof(my_version), PSTR("%s.%d"), my_version, VERSION & 0xff);
  }
  // Thehackbox inserts "release" or "commit number" before compiling using sed -i -e 's/PSTR("(%s)")/PSTR("(85cff52-%s)")/g' tasmota.ino
  snprintf_P(my_image, sizeof(my_image), PSTR("(%s)"), CODE_IMAGE_STR);  // Results in (85cff52-tasmota) or (release-tasmota)

  SettingsLoad();
  SettingsDelta();

  OsWatchInit();

  GetFeatures();

  if (1 == RtcReboot.fast_reboot_count) {  // Allow setting override only when all is well
    UpdateQuickPowerCycle(true);
    XdrvCall(FUNC_SETTINGS_OVERRIDE);
  }

//  mdns_delayed_start = Settings.param[P_MDNS_DELAYED_START];
  seriallog_level = Settings.seriallog_level;
  seriallog_timer = SERIALLOG_TIMER;
  syslog_level = Settings.syslog_level;
  stop_flash_rotate = Settings.flag.stop_flash_rotate;  // SetOption12 - Switch between dynamic or fixed slot flash save location
  save_data_counter = Settings.save_data;
  ssleep = Settings.sleep;
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

  if (Settings.param[P_BOOT_LOOP_OFFSET]) {         // SetOption36
    // Disable functionality as possible cause of fast restart within BOOT_LOOP_TIME seconds (Exception, WDT or restarts)
    if (RtcReboot.fast_reboot_count > Settings.param[P_BOOT_LOOP_OFFSET]) {       // Restart twice
      Settings.flag3.user_esp8285_enable = 0;       // SetOption51 - Enable ESP8285 user GPIO's - Disable ESP8285 Generic GPIOs interfering with flash SPI
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
        for (uint32_t i = 0; i < ARRAY_SIZE(Settings.my_gp.io); i++) {
          Settings.my_gp.io[i] = GPIO_NONE;         // Reset user defined GPIO disabling sensors
        }
        Settings.my_adc0 = ADC0_NONE;               // Reset user defined ADC0 disabling sensors
      }
      if (RtcReboot.fast_reboot_count > Settings.param[P_BOOT_LOOP_OFFSET] +4) {  // Restarted 6 times
#ifdef ESP8266
        Settings.module = SONOFF_BASIC;             // Reset module to Sonoff Basic
  //      Settings.last_module = SONOFF_BASIC;
#endif  // ESP8266
#ifdef ESP32
        Settings.module = WEMOS;                    // Reset module to Wemos
#endif  // ESP32
      }
      AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_LOG_SOME_SETTINGS_RESET " (%d)"), RtcReboot.fast_reboot_count);
    }
  }

  Format(mqtt_client, SettingsText(SET_MQTT_CLIENT), sizeof(mqtt_client));
  Format(mqtt_topic, SettingsText(SET_MQTT_TOPIC), sizeof(mqtt_topic));
  if (strstr(SettingsText(SET_HOSTNAME), "%") != nullptr) {
    SettingsUpdateText(SET_HOSTNAME, WIFI_HOSTNAME);
    snprintf_P(my_hostname, sizeof(my_hostname)-1, SettingsText(SET_HOSTNAME), mqtt_topic, ESP_getChipId() & 0x1FFF);
  } else {
    snprintf_P(my_hostname, sizeof(my_hostname)-1, SettingsText(SET_HOSTNAME));
  }

  GetEspHardwareType();
  GpioInit();

//  SetSerialBaudrate(Settings.baudrate * 300);  // Allow reset of serial interface if current baudrate is different from requested baudrate

  WifiConnect();

  SetPowerOnState();

  AddLog_P2(LOG_LEVEL_INFO, PSTR(D_PROJECT " %s %s " D_VERSION " %s%s-" ARDUINO_CORE_RELEASE), PROJECT, SettingsText(SET_FRIENDLYNAME1), my_version, my_image);
#ifdef FIRMWARE_MINIMAL
  AddLog_P2(LOG_LEVEL_INFO, PSTR(D_WARNING_MINIMAL_VERSION));
#endif  // FIRMWARE_MINIMAL

  memcpy_P(log_data, VERSION_MARKER, 1);  // Dummy for compiler saving VERSION_MARKER

  RtcInit();

#ifdef USE_ARDUINO_OTA
  ArduinoOTAInit();
#endif  // USE_ARDUINO_OTA

  XdrvCall(FUNC_INIT);
  XsnsCall(FUNC_INIT);
}

void BacklogLoop(void)
{
  if (TimeReached(backlog_delay)) {
    if (!BACKLOG_EMPTY && !backlog_mutex) {
#ifdef SUPPORT_IF_STATEMENT
      backlog_mutex = true;
      String cmd = backlog.shift();
      backlog_mutex = false;
      ExecuteCommand((char*)cmd.c_str(), SRC_BACKLOG);
#else
      backlog_mutex = true;
      ExecuteCommand((char*)backlog[backlog_pointer].c_str(), SRC_BACKLOG);
      backlog_pointer++;
      if (backlog_pointer >= MAX_BACKLOG) { backlog_pointer = 0; }
      backlog_mutex = false;
#endif
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
#ifdef USE_DEVICE_GROUPS
  DeviceGroupsLoop();
#endif  // USE_DEVICE_GROUPS
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
  ArduinoOtaLoop();
#endif  // USE_ARDUINO_OTA

  uint32_t my_activity = millis() - my_sleep;

  if (Settings.flag3.sleep_normal) {              // SetOption60 - Enable normal sleep instead of dynamic sleep
    //  yield();                                  // yield == delay(0), delay contains yield, auto yield in loop
    delay(ssleep);                                // https://github.com/esp8266/Arduino/issues/2021
  } else {
    if (my_activity < (uint32_t)ssleep) {
      delay((uint32_t)ssleep - my_activity);      // Provide time for background tasks like wifi
    } else {
      if (global_state.wifi_down) {
        delay(my_activity /2);                    // If wifi down and my_activity > setoption36 then force loop delay to 1/3 of my_activity period
      }
    }
  }

  if (!my_activity) { my_activity++; }            // We cannot divide by 0
  uint32_t loop_delay = ssleep;
  if (!loop_delay) { loop_delay++; }              // We cannot divide by 0
  uint32_t loops_per_second = 1000 / loop_delay;  // We need to keep track of this many loops per second
  uint32_t this_cycle_ratio = 100 * my_activity / loop_delay;
  loop_load_avg = loop_load_avg - (loop_load_avg / loops_per_second) + (this_cycle_ratio / loops_per_second); // Take away one loop average away and add the new one
}
