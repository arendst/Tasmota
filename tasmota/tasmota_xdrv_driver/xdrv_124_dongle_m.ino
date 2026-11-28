/*
  xdrv_124_dongle_m.ino - SONOFF Dongle Max support for Tasmota

  Copyright (C) 2026  SONOFF

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

#if defined(ESP32) && defined(USE_DONGLE_M)

#include <ETH.h>

#ifndef USE_TCP_BRIDGE
#error USE_DONGLE_M requires USE_TCP_BRIDGE
#endif
#ifndef USE_ETHERNET
#error USE_DONGLE_M requires USE_ETHERNET
#endif

#define XDRV_124                  124
#define DONGLEM_SETTINGS_VERSION  0x01020000
#define DONGLEM_BOOT_PIN          12
#define DONGLEM_RESET_PIN         15
#define DONGLEM_DEFAULT_PORT      6638
#define DONGLEM_DEFAULT_BAUDRATE  115200
#define DONGLEM_RESET_TIME        100
#define DONGLEM_BOOT_SETTLE_TIME  500
#define DONGLEM_PWM_RED           5
#define DONGLEM_PWM_GREEN         6
#define DONGLEM_PWM_BLUE          7
#define DONGLEM_BLINK_TIME        500
#define DONGLEM_STATE_PUBLISH_MS  200   // Coalesce adjacent command/web changes
#define DONGLEM_SETTINGS_SAVE_MS  2000  // Debounce writes when sliding LED controls

#ifndef HOME_ASSISTANT_DISCOVERY_PREFIX
#define HOME_ASSISTANT_DISCOVERY_PREFIX "homeassistant"
#endif

enum DongleMMode : uint8_t {
  DONGLEM_MODE_APP,
  DONGLEM_MODE_BOOTLOADER
};

// Arduino's .ino preprocessor otherwise emits this prototype before DongleMMode.
// An explicit declaration here prevents that misplaced auto-generated prototype.
bool DongleMSetMode(DongleMMode mode);

enum DongleMTransition : uint8_t {
  DONGLEM_TRANSITION_IDLE,
  DONGLEM_TRANSITION_RESET_LOW,
  DONGLEM_TRANSITION_SETTLE
};

// 0 preserves the original behaviour: solid when connected, blinking otherwise.
enum DongleMLightEffect : uint8_t {
  DONGLEM_LIGHT_NETWORK = 0,
  DONGLEM_LIGHT_SOLID = 1,
  DONGLEM_LIGHT_BLINK = 2
};


struct DONGLEM_SETTINGS {
  uint32_t crc32;
  uint32_t version;
  uint32_t app_baudrate;
  uint16_t tcp_port;
  uint8_t light_power;
  uint8_t light_brightness;
  uint8_t light_color[3];
  uint8_t light_effect;  // Reuses the former spare byte; 0 = Network.
} DongleMSettings;

const char* DongleMLightEffectName(void) {
  switch (DongleMSettings.light_effect) {
    case DONGLEM_LIGHT_SOLID: return "Solid";
    case DONGLEM_LIGHT_BLINK: return "Blink";
    default: return "Network";
  }
}

struct DONGLEM_RUNTIME {
  uint32_t deadline;
  uint32_t button_pressed_at;
  uint32_t light_deadline;
  uint32_t publish_deadline;
  uint32_t save_deadline;
  DongleMMode requested_mode;
  DongleMMode active_mode;
  DongleMTransition transition;
  bool busy;
  bool button_pressed;
  bool button_long_reported;
  bool light_phase_on;
  bool network_connected;
  bool ethernet_connected;
  bool wifi_connected;
  bool state_dirty;
  bool settings_dirty;
  char last_button_event[20];
  char last_button_time[24];
} DongleM;

void DongleMPublishState(void);
void DongleMPublishModeState(void);

// Schedules state publication without flooding MQTT on frequent slider updates.
void DongleMMarkStateDirty(void) {
  if (!DongleM.state_dirty) {
    DongleM.publish_deadline = millis() + DONGLEM_STATE_PUBLISH_MS;
    DongleM.state_dirty = true;
  }
}

void DongleMMarkSettingsDirty(void) {
  DongleM.settings_dirty = true;
  // Reset the debounce window with each successive edit.
  DongleM.save_deadline = millis() + DONGLEM_SETTINGS_SAVE_MS;
  DongleMMarkStateDirty();
}

/*********************************************************************************************\
 * Settings
\*********************************************************************************************/

void DongleMSettingsDefault(void) {
  memset(&DongleMSettings, 0, sizeof(DongleMSettings));
  DongleMSettings.version = DONGLEM_SETTINGS_VERSION;
  DongleMSettings.app_baudrate = DONGLEM_DEFAULT_BAUDRATE;
  DongleMSettings.tcp_port = DONGLEM_DEFAULT_PORT;
  DongleMSettings.light_power = 1;
  DongleMSettings.light_brightness = 75;
  DongleMSettings.light_color[0] = 0x00;
  DongleMSettings.light_color[1] = 0x00;
  DongleMSettings.light_color[2] = 0xFF;
}

// Return false on a filesystem failure so the scheduler can retry.
// Preserve the old CRC on failure; otherwise subsequent saves would be skipped.
bool DongleMSettingsSave(void) {
#ifdef USE_UFILESYS
  uint32_t crc32 = GetCfgCrc32((uint8_t*)&DongleMSettings + 4,
                              sizeof(DongleMSettings) - 4);
  if (crc32 != DongleMSettings.crc32) {
    uint32_t previous_crc32 = DongleMSettings.crc32;
    DongleMSettings.crc32 = crc32;
    char filename[20];
    snprintf_P(filename, sizeof(filename), PSTR(TASM_FILE_DRIVER), XDRV_124);
    if (!TfsSaveFile(filename, (const uint8_t*)&DongleMSettings, sizeof(DongleMSettings))) {
      DongleMSettings.crc32 = previous_crc32;
      AddLog(LOG_LEVEL_ERROR, PSTR("DML: Unable to save settings"));
      return false;
    }
  }
#endif  // USE_UFILESYS
  return true;
}

void DongleMFlushSettings(void) {
  if (!DongleM.settings_dirty) { return; }
  if (DongleMSettingsSave()) {
    DongleM.settings_dirty = false;
  } else {
    DongleM.save_deadline = millis() + DONGLEM_SETTINGS_SAVE_MS;
  }
}

void DongleMProcessPendingEvery250ms(void) {
  uint32_t now = millis();
  if (DongleM.state_dirty && (int32_t)(now - DongleM.publish_deadline) >= 0) {
    DongleMPublishState();
  }
  if (DongleM.settings_dirty && (int32_t)(now - DongleM.save_deadline) >= 0) {
    DongleMFlushSettings();
  }
}

void DongleMSettingsLoad(bool erase) {
  DongleMSettingsDefault();
#ifdef USE_UFILESYS
  char filename[20];
  snprintf_P(filename, sizeof(filename), PSTR(TASM_FILE_DRIVER), XDRV_124);
  if (erase) {
    TfsDeleteFile(filename);
  } else if (TfsLoadFile(filename, (uint8_t*)&DongleMSettings, sizeof(DongleMSettings))) {
    if ((DongleMSettings.version != DONGLEM_SETTINGS_VERSION) ||
        !DongleMSettings.tcp_port ||
        ((DONGLEM_DEFAULT_BAUDRATE != DongleMSettings.app_baudrate) &&
         (460800 != DongleMSettings.app_baudrate) &&
         (921600 != DongleMSettings.app_baudrate)) ||
        !DongleMSettings.light_brightness || (DongleMSettings.light_brightness > 100) ||
         (DongleMSettings.light_effect > DONGLEM_LIGHT_BLINK)) {
      DongleMSettingsDefault();
    }
  }
#endif  // USE_UFILESYS
}

/*********************************************************************************************\
 * RGB indicator (independent PWM6, PWM7 and PWM8)
\*********************************************************************************************/

bool DongleMLightAvailable(void) {
  return PinUsed(GPIO_PWM1, DONGLEM_PWM_RED) &&
         PinUsed(GPIO_PWM1, DONGLEM_PWM_GREEN) &&
         PinUsed(GPIO_PWM1, DONGLEM_PWM_BLUE);
}

bool DongleMEthernetConnected(void) {
  // A configured/static address can remain on the Ethernet netif after the
  // cable is removed.  An address alone therefore does not mean that the
  // device is reachable.
  return !TasmotaGlobal.global_state.eth_down && EthernetHasIPv4();
}

bool DongleMWifiConnected(void) {
  return Settings->flag4.network_wifi && WifiHasIPv4();
}

void DongleMSelectNetworkInterface(bool ethernet_connected, bool wifi_connected) {
  // ESP-IDF gives Wi-Fi a higher default route priority than Ethernet.  If
  // Wi-Fi obtains an address while a TCP client is connected through
  // Ethernet, changing the default netif can break that established stream.
  // Pin the default route to Ethernet while its link is usable, and restore
  // Wi-Fi explicitly when Ethernet goes away.
  if (ethernet_connected) {
    ETH.setDefault();
  } else if (wifi_connected) {
    WiFi.STA.setDefault();
  }
}

void DongleMLightApply(void) {
  if (!DongleMLightAvailable()) { return; }

  bool blinking = (DONGLEM_LIGHT_BLINK == DongleMSettings.light_effect) ||
                  ((DONGLEM_LIGHT_NETWORK == DongleMSettings.light_effect) &&
                   !DongleM.network_connected);
  bool output_on = DongleMSettings.light_power &&
                   (!blinking || DongleM.light_phase_on);
  const uint8_t pwm_index[] = { DONGLEM_PWM_RED, DONGLEM_PWM_GREEN, DONGLEM_PWM_BLUE };
  for (uint32_t i = 0; i < nitems(pwm_index); i++) {
    uint32_t value = 0;
    if (output_on) {
      value = ((uint32_t)DongleMSettings.light_color[i] *
               DongleMSettings.light_brightness * Settings->pwm_range + 12750) / 25500;
    }
    TasmotaGlobal.pwm_value[pwm_index[i]] = value;
  }
  PwmApplyGPIO(false);
}

void DongleMLightEvery250ms(bool force = false) {
  bool ethernet_connected = DongleMEthernetConnected();
  bool wifi_connected = DongleMWifiConnected();
  bool connected = ethernet_connected || wifi_connected;
  bool changed = force;

  if (force || ethernet_connected != DongleM.ethernet_connected ||
      wifi_connected != DongleM.wifi_connected) {
    DongleM.ethernet_connected = ethernet_connected;
    DongleM.wifi_connected = wifi_connected;
    DongleMSelectNetworkInterface(ethernet_connected, wifi_connected);
  }

  if (connected != DongleM.network_connected) {
    DongleM.network_connected = connected;
    DongleM.light_phase_on = true;
    DongleM.light_deadline = millis() + DONGLEM_BLINK_TIME;
    changed = true;
  } else if ((int32_t)(millis() - DongleM.light_deadline) >= 0) {
    DongleM.light_phase_on = !DongleM.light_phase_on;
    DongleM.light_deadline = millis() + DONGLEM_BLINK_TIME;
    changed = (DONGLEM_LIGHT_BLINK == DongleMSettings.light_effect) ||
              ((DONGLEM_LIGHT_NETWORK == DongleMSettings.light_effect) && !connected);
  }

  if (changed) { DongleMLightApply(); }
}

bool DongleMSettingsRestore(void) {
  XdrvMailbox.data = (char*)&DongleMSettings;
  XdrvMailbox.index = sizeof(DongleMSettings);
  return true;
}

/*********************************************************************************************\
 * MG24 state machine
\*********************************************************************************************/

const char* DongleMActiveModeName(void) {
  return (DONGLEM_MODE_BOOTLOADER == DongleM.active_mode) ? PSTR("BOOT") : PSTR("APP");
}

const char* DongleMRequestedModeName(void) {
  return (DONGLEM_MODE_BOOTLOADER == DongleM.requested_mode) ? PSTR("BOOT") : PSTR("APP");
}

bool DongleMBaudrateValid(uint32_t baudrate) {
  return ((115200 == baudrate) || (460800 == baudrate) || (921600 == baudrate));
}

void DongleMStartBridge(void) {
  TCPBridgeStart(DongleMSettings.tcp_port);
}

bool DongleMStartTransition(uint8_t mode) {
  if (DongleM.busy) { return false; }

  DongleM.busy = true;
  DongleM.requested_mode = (DongleMMode)mode;
  TCPBridgeStop();

  if (DONGLEM_MODE_BOOTLOADER == mode) {
    TCPBridgeSetBaudrate(115200);
    digitalWrite(DONGLEM_BOOT_PIN, HIGH);
  } else {
    TCPBridgeSetBaudrate(DongleMSettings.app_baudrate);
    digitalWrite(DONGLEM_BOOT_PIN, LOW);
  }

  digitalWrite(DONGLEM_RESET_PIN, LOW);
  DongleM.deadline = millis() + DONGLEM_RESET_TIME;
  DongleM.transition = DONGLEM_TRANSITION_RESET_LOW;
  DongleMMarkStateDirty();
  AddLog(LOG_LEVEL_INFO, PSTR("DML: MG24 transition to %s"), DongleMRequestedModeName());
  return true;
}

void DongleMTransitionEvery50ms(void) {
  if (!DongleM.busy || (int32_t)(millis() - DongleM.deadline) < 0) { return; }

  switch (DongleM.transition) {
    case DONGLEM_TRANSITION_RESET_LOW:
      digitalWrite(DONGLEM_RESET_PIN, HIGH);
      DongleM.deadline = millis() + DONGLEM_BOOT_SETTLE_TIME;
      DongleM.transition = DONGLEM_TRANSITION_SETTLE;
      break;
    case DONGLEM_TRANSITION_SETTLE:
      // BOOT is sampled during reset and can be released after the bootloader starts.
      digitalWrite(DONGLEM_BOOT_PIN, LOW);
      DongleMStartBridge();
      DongleM.active_mode = DongleM.requested_mode;
      DongleM.transition = DONGLEM_TRANSITION_IDLE;
      DongleM.busy = false;
      DongleMPublishModeState();
      DongleMMarkStateDirty();
      AddLog(LOG_LEVEL_INFO, PSTR("DML: MG24 %s applied, TCP bridge restored"), DongleMActiveModeName());
      break;
    default:
      DongleM.busy = false;
      DongleM.transition = DONGLEM_TRANSITION_IDLE;
      break;
  }
}


/*********************************************************************************************\
 * Shared business operations (Web, console and MQTT all call these)
\*********************************************************************************************/

bool DongleMSetMode(DongleMMode mode) {
  if (mode != DONGLEM_MODE_APP && mode != DONGLEM_MODE_BOOTLOADER) { return false; }
  return DongleMStartTransition(mode);
}

bool DongleMRestart(void) {
  return DongleMSetMode(DongleM.active_mode);
}

bool DongleMSetBaudrate(uint32_t baudrate) {
  if (!DongleMBaudrateValid(baudrate) || DongleM.busy) { return false; }
  if (DongleMSettings.app_baudrate == baudrate) { return true; }

  DongleMSettings.app_baudrate = baudrate;
  if (DONGLEM_MODE_APP == DongleM.active_mode) {
    TCPBridgeStop();
    TCPBridgeSetBaudrate(baudrate);
    DongleMStartBridge();
  }
  DongleMMarkSettingsDirty();
  return true;
}

bool DongleMSetPort(uint32_t port) {
  if (!port || port > UINT16_MAX || DongleM.busy) { return false; }
  if (DongleMSettings.tcp_port == port) { return true; }

  DongleMSettings.tcp_port = (uint16_t)port;
  DongleMStartBridge();
  DongleMMarkSettingsDirty();
  return true;
}

struct DONGLEM_LIGHT_STATE {
  uint8_t power;
  uint8_t brightness;
  uint8_t color[3];
  uint8_t effect;
};

// Keep these explicit prototypes after the type declaration: Tasmota preprocesses
// all .ino files into one translation unit and could otherwise place them too early.
DONGLEM_LIGHT_STATE DongleMGetLightState(void);
bool DongleMSetLight(const DONGLEM_LIGHT_STATE &next);

DONGLEM_LIGHT_STATE DongleMGetLightState(void) {
  DONGLEM_LIGHT_STATE state;
  state.power = DongleMSettings.light_power;
  state.brightness = DongleMSettings.light_brightness;
  memcpy(state.color, DongleMSettings.light_color, sizeof(state.color));
  state.effect = DongleMSettings.light_effect;
  return state;
}

bool DongleMSetLight(const DONGLEM_LIGHT_STATE &next) {
  if (!DongleMLightAvailable() || next.power > 1 ||
      next.brightness < 1 || next.brightness > 100 ||
      next.effect > DONGLEM_LIGHT_BLINK) {
    return false;
  }
  DONGLEM_LIGHT_STATE previous = DongleMGetLightState();
  if (previous.power == next.power && previous.brightness == next.brightness &&
      previous.effect == next.effect &&
      !memcmp(previous.color, next.color, sizeof(previous.color))) {
    return true;
  }

  // All fields have been validated: commit together, then apply hardware once.
  DongleMSettings.light_power = next.power;
  DongleMSettings.light_brightness = next.brightness;
  memcpy(DongleMSettings.light_color, next.color, sizeof(next.color));
  DongleMSettings.light_effect = next.effect;
  DongleM.light_phase_on = true;
  DongleM.light_deadline = millis() + DONGLEM_BLINK_TIME;
  DongleMLightApply();
  DongleMMarkSettingsDirty();
  return true;
}

bool DongleMParseLightEffect(const char *text, uint8_t *effect) {
  if (!strcasecmp(text, "Network")) { *effect = DONGLEM_LIGHT_NETWORK; }
  else if (!strcasecmp(text, "Solid")) { *effect = DONGLEM_LIGHT_SOLID; }
  else if (!strcasecmp(text, "Blink")) { *effect = DONGLEM_LIGHT_BLINK; }
  else { return false; }
  return true;
}

// Strict non-negative integer parsing, avoiding String::toInt accepting trailing junk.
bool DongleMParseUInt(const String &input, uint32_t *out) {
  if (!input.length()) { return false; }
  uint32_t value = 0;
  for (uint32_t i = 0; i < input.length(); i++) {
    char c = input[i];
    if (c < '0' || c > '9') { return false; }
    uint8_t digit = c - '0';
    if (value > (UINT32_MAX - digit) / 10) { return false; }
    value = value * 10 + digit;
  }
  *out = value;
  return true;
}

/*********************************************************************************************\
 * Button event
\*********************************************************************************************/

void DongleMButtonTimestamp(char *buffer, size_t size) {
  String timestamp = GetDateAndTime(DT_LOCALNOTZ);
  timestamp.replace('T', ' ');
  strlcpy(buffer, timestamp.c_str(), size);
}

void DongleMPublishState(void);

void DongleMRecordButtonEvent(const char *event) {
  strlcpy(DongleM.last_button_event, event, sizeof(DongleM.last_button_event));
  DongleMButtonTimestamp(DongleM.last_button_time, sizeof(DongleM.last_button_time));

  Response_P(PSTR("{\"event_type\":\"%s\",\"time\":\"%s\"}"),
             DongleM.last_button_event, DongleM.last_button_time);
  MqttPublishPrefixTopicRulesProcess_P(STAT, PSTR("DONGLEM_EVENT"), false);
  DongleMMarkStateDirty();
}

bool DongleMButtonPressed(void) {
  if (XdrvMailbox.index) { return false; }  // Dongle-M has one physical button.

  bool pressed = (PRESSED == XdrvMailbox.payload);
  if (pressed && !DongleM.button_pressed) {
    DongleM.button_pressed = true;
    DongleM.button_long_reported = false;
    DongleM.button_pressed_at = millis();
  } else if (!pressed && DongleM.button_pressed) {
    DongleM.button_pressed = false;
    if (DongleM.button_long_reported) {
      DongleMRecordButtonEvent(PSTR("long_release"));
    }
  } else if (pressed && !DongleM.button_long_reported) {
    uint32_t hold_ms = Settings->param[P_HOLD_TIME] * 100;
    if ((millis() - DongleM.button_pressed_at) >= hold_ms) {
      DongleM.button_long_reported = true;
      DongleMRecordButtonEvent(PSTR("long_press"));
    }
  }
  return false;  // Keep the standard Tasmota button processing active.
}

bool DongleMButtonMultiPressed(void) {
  if (XdrvMailbox.index || DongleM.button_long_reported) { return false; }
  if (1 == XdrvMailbox.payload) {
    DongleMRecordButtonEvent(PSTR("single_click"));
  } else if (2 == XdrvMailbox.payload) {
    DongleMRecordButtonEvent(PSTR("double_click"));
  }
  return false;
}

/*********************************************************************************************\
 * Status JSON
\*********************************************************************************************/

void DongleMUptime(char *buffer, size_t size) {
  uint32_t uptime = UpTime();
  uint32_t days = uptime / 86400;
  uptime %= 86400;
  uint32_t hours = uptime / 3600;
  uptime %= 3600;
  uint32_t minutes = uptime / 60;
  uint32_t seconds = uptime % 60;
  snprintf_P(buffer, size, PSTR("%02u d %02u h %02u m %02u s"),
             days, hours, minutes, seconds);
}

void DongleMAppendStatusJson(bool root, bool include_diagnostics = true) {
  bool wifi_connected = DongleMWifiConnected();
  bool ethernet_connected = DongleMEthernetConnected();
  uint32_t clients = TCPBridgeClientCount();
  String wifi_ip = wifi_connected ? WifiGetIPv4Str() : String();
  String ethernet_ip = ethernet_connected ? EthernetGetIPv4Str() : String();
  String ssid = wifi_connected ? EscapeJSONString(WiFi.SSID().c_str()) : String();
  String hostname = EscapeJSONString(SettingsText(SET_HOSTNAME));
  hostname += F(".local");
  char color[7];
  snprintf_P(color, sizeof(color), PSTR("%02X%02X%02X"),
             DongleMSettings.light_color[0], DongleMSettings.light_color[1],
             DongleMSettings.light_color[2]);
  uint32_t background = WebColor(COL_BACKGROUND);
  uint32_t luminance = (((background >> 16) & 0xFF) * 299) +
                       (((background >> 8) & 0xFF) * 587) + ((background & 0xFF) * 114);

  if (root) {
    Response_P(PSTR("{\"DongleM\":{"));
  } else {
    ResponseAppend_P(PSTR(",\"DongleM\":{"));
  }
  ResponseAppend_P(PSTR(
    "\"Ethernet\":{\"Connected\":%s,\"IP\":\"%s\"},"
    "\"Wifi\":{\"Connected\":%s,\"IP\":\"%s\",\"RSSI\":%d,\"SSID\":\"%s\"},"
    "\"TCP\":{\"Listening\":%s,\"Connected\":%s,\"Clients\":%u,\"Port\":%u},"
    "\"MG24\":{\"Mode\":\"%s\",\"RequestedMode\":\"%s\",\"Confirmed\":false,\"Busy\":%s,\"Baudrate\":%u,\"AppBaudrate\":%u},"
    "\"Button\":{\"Time\":\"%s\",\"Event\":\"%s\"},"
    "\"Light\":{\"Available\":%s,\"Power\":%s,\"Brightness\":%u,\"Color\":\"%s\","
      "\"RGB\":[%u,%u,%u],\"Effect\":\"%s\"},"
    "\"Network\":{\"Hostname\":\"%s\"},"
    "\"Theme\":\"%s\""),
    ethernet_connected ? PSTR("true") : PSTR("false"), ethernet_ip.c_str(),
    wifi_connected ? PSTR("true") : PSTR("false"), wifi_ip.c_str(),
    wifi_connected ? WiFi.RSSI() : 0, ssid.c_str(),
    TCPBridgeListening() ? PSTR("true") : PSTR("false"), clients ? PSTR("true") : PSTR("false"),
    clients, DongleMSettings.tcp_port,
    DongleMActiveModeName(), DongleMRequestedModeName(),
    DongleM.busy ? PSTR("true") : PSTR("false"),
    TCPBridgeBaudrate(), DongleMSettings.app_baudrate,
    DongleM.last_button_time, DongleM.last_button_event,
    DongleMLightAvailable() ? PSTR("true") : PSTR("false"),
    DongleMSettings.light_power ? PSTR("true") : PSTR("false"),
    DongleMSettings.light_brightness, color,
    DongleMSettings.light_color[0], DongleMSettings.light_color[1],
    DongleMSettings.light_color[2], DongleMLightEffectName(),
    hostname.c_str(), (luminance >= 128000) ? PSTR("light") : PSTR("dark"));

  // The web page does not render these fields. Keep the two diagnostics in the
  // MQTT/command payload because Home Assistant discovery consumes them.
  if (include_diagnostics) {
    char uptime[36];
    DongleMUptime(uptime, sizeof(uptime));
    ResponseAppend_P(PSTR(",\"Uptime\":\"%s\",\"FirmwareVersion\":\"%s\""),
                     uptime, TasmotaGlobal.version);
  }

  ResponseAppend_P(PSTR("}"));

  if (root) {
    ResponseAppend_P(PSTR("}"));
  }
}

void DongleMPublishState(void) {
  DongleM.state_dirty = false;
  DongleMAppendStatusJson(true);
  MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR("DONGLEM"), false);
}

// Home Assistant needs a compact retained state so it can restore the switch
// immediately after an HA restart without retaining the full diagnostic JSON.
void DongleMPublishModeState(void) {
  Response_P(PSTR("%s"), DongleMActiveModeName());
  MqttPublishPrefixTopic_P(STAT, PSTR("DONGLEM_MODE"), true);
}

void DongleMConnectionEverySecond(void) {
  static bool initialized = false;
  static uint32_t last_clients = 0;
  static bool last_eth = false;
  static bool last_wifi = false;
  static String last_eth_ip;
  static String last_wifi_ip;
  static String last_ssid;

  uint32_t clients = TCPBridgeClientCount();
  bool eth = DongleMEthernetConnected();
  bool wifi = DongleMWifiConnected();
  String eth_ip = eth ? EthernetGetIPv4Str() : String();
  String wifi_ip = wifi ? WifiGetIPv4Str() : String();
  String ssid = wifi ? WiFi.SSID() : String();

  if (!initialized || clients != last_clients || eth != last_eth ||
      wifi != last_wifi || eth_ip != last_eth_ip ||
      wifi_ip != last_wifi_ip || ssid != last_ssid) {
    initialized = true;
    last_clients = clients;
    last_eth = eth;
    last_wifi = wifi;
    last_eth_ip = eth_ip;
    last_wifi_ip = wifi_ip;
    last_ssid = ssid;
    DongleMMarkStateDirty();
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndDongleMStatus(void) {
  DongleMAppendStatusJson(true);
}

void CmndDongleMMode(void) {
  if (XdrvMailbox.data_len) {
    DongleMMode mode;
    if (!strcasecmp(XdrvMailbox.data, PSTR("APP"))) {
      mode = DONGLEM_MODE_APP;
    } else if (!strcasecmp(XdrvMailbox.data, PSTR("BOOT")) ||
               !strcasecmp(XdrvMailbox.data, PSTR("BOOTLOADER"))) {
      mode = DONGLEM_MODE_BOOTLOADER;
    } else {
      ResponseCmndFailed();
      return;
    }
    if (!DongleMSetMode(mode)) { ResponseCmndFailed(); return; }
  }
  ResponseCmndChar(XdrvMailbox.data_len ? DongleMRequestedModeName() : DongleMActiveModeName());
}

void CmndDongleMRestart(void) {
  if (DongleMRestart()) { ResponseCmndDone(); }
  else { ResponseCmndFailed(); }
}

void CmndDongleMBaudrate(void) {
  if (XdrvMailbox.data_len && !DongleMSetBaudrate(XdrvMailbox.payload)) {
    ResponseCmndFailed();
    return;
  }
  ResponseCmndNumber(DongleMSettings.app_baudrate);
}

void CmndDongleMPort(void) {
  if (XdrvMailbox.data_len && !DongleMSetPort(XdrvMailbox.payload)) {
    ResponseCmndFailed();
    return;
  }
  ResponseCmndNumber(DongleMSettings.tcp_port);
}

void CmndDongleMLightPower(void) {
  if (XdrvMailbox.data_len) {
    DONGLEM_LIGHT_STATE next = DongleMGetLightState();
    if (!strcasecmp(XdrvMailbox.data, "ON") || !strcmp(XdrvMailbox.data, "1")) {
      next.power = 1;
    } else if (!strcasecmp(XdrvMailbox.data, "OFF") || !strcmp(XdrvMailbox.data, "0")) {
      next.power = 0;
    } else { ResponseCmndFailed(); return; }
    if (!DongleMSetLight(next)) { ResponseCmndFailed(); return; }
  }
  ResponseCmndChar(DongleMSettings.light_power ? "ON" : "OFF");
}

void CmndDongleMLightBrightness(void) {
  if (XdrvMailbox.data_len) {
    int32_t level = XdrvMailbox.payload;
    if (level < 0 || level > 100) { ResponseCmndFailed(); return; }
    DONGLEM_LIGHT_STATE next = DongleMGetLightState();
    if (level == 0) { next.power = 0; }
    else { next.brightness = (uint8_t)level; next.power = 1; }
    if (!DongleMSetLight(next)) { ResponseCmndFailed(); return; }
  }
  ResponseCmndNumber(DongleMSettings.light_brightness);
}

void CmndDongleMLightColor(void) {
  if (XdrvMailbox.data_len) {
    int r = -1, g = -1, b = -1;
    char extra;
    if (sscanf(XdrvMailbox.data, "%d,%d,%d%c", &r, &g, &b, &extra) != 3 ||
        r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
      ResponseCmndFailed();
      return;
    }
    DONGLEM_LIGHT_STATE next = DongleMGetLightState();
    next.color[0] = (uint8_t)r;
    next.color[1] = (uint8_t)g;
    next.color[2] = (uint8_t)b;
    next.power = 1;
    if (!DongleMSetLight(next)) { ResponseCmndFailed(); return; }
  }
  char rgb[13];
  snprintf_P(rgb, sizeof(rgb), PSTR("%u,%u,%u"),
             DongleMSettings.light_color[0],
             DongleMSettings.light_color[1],
             DongleMSettings.light_color[2]);
  ResponseCmndChar(rgb);
}

void CmndDongleMLightEffect(void) {
  if (XdrvMailbox.data_len) {
    DONGLEM_LIGHT_STATE next = DongleMGetLightState();
    if (!DongleMParseLightEffect(XdrvMailbox.data, &next.effect) ||
        !DongleMSetLight(next)) {
      ResponseCmndFailed();
      return;
    }
  }
  ResponseCmndChar(DongleMLightEffectName());
}

const char kDongleMCommands[] PROGMEM = "DongleM|"
  "Status|Mode|Restart|Baudrate|Port|LightPower|LightBrightness|LightColor|LightEffect";

void (* const DongleMCommand[])(void) PROGMEM = {
  &CmndDongleMStatus, &CmndDongleMMode, &CmndDongleMRestart,
  &CmndDongleMBaudrate, &CmndDongleMPort,
  &CmndDongleMLightPower, &CmndDongleMLightBrightness,
  &CmndDongleMLightColor, &CmndDongleMLightEffect
};

/*********************************************************************************************\
 * Web UI
\*********************************************************************************************/

#ifdef USE_WEBSERVER

#include "tasmota_xdrv_driver/xdrv_124_dongle_m_web.h"

void DongleMWebJsonReply(int code, bool ok, const char *message) {
  Response_P(PSTR("{\"ok\":%s,\"message\":\"%s\"}"),
             ok ? PSTR("true") : PSTR("false"), EscapeJSONString(message).c_str());
  WSSend(code, CT_APP_JSON, ResponseData());
}

bool DongleMParseIPv4(const String &value, uint32_t *address) {
  unsigned int part[4];
  char tail;
  if (4 != sscanf(value.c_str(), "%u.%u.%u.%u%c", &part[0], &part[1], &part[2], &part[3], &tail)) {
    return false;
  }
  for (uint32_t i = 0; i < 4; i++) {
    if (part[i] > 255) { return false; }
  }
  IPAddress ip(part[0], part[1], part[2], part[3]);
  *address = (uint32_t)ip;
  return true;
}

bool DongleMNetmaskValid(uint32_t address) {
  IPAddress mask(address);
  bool zero_seen = false;
  uint32_t ones = 0;
  for (uint32_t octet = 0; octet < 4; octet++) {
    for (int32_t bit = 7; bit >= 0; bit--) {
      bool one = bitRead(mask[octet], bit);
      if (one && zero_seen) { return false; }
      zero_seen |= !one;
      ones += one;
    }
  }
  return ((ones >= 8) && (ones <= 30));
}

bool DongleMHostNameValid(const String &hostname) {
  if (!hostname.length() || (hostname.length() > 32) ||
      ('-' == hostname[0]) || ('-' == hostname[hostname.length() -1])) {
    return false;
  }
  for (uint32_t i = 0; i < hostname.length(); i++) {
    char c = hostname[i];
    if (!isalnum((unsigned char)c) && ('-' != c)) { return false; }
  }
  return true;
}

bool DongleMNormalizeHostName(String &hostname) {
  hostname.trim();
  String lower = hostname;
  lower.toLowerCase();
  if (lower.endsWith(F(".local"))) {
    hostname.remove(hostname.length() -6);
  }
  return DongleMHostNameValid(hostname);
}

void DongleMWebRootContent(void) {
  WSContentSendRaw_P(DONGLEM_WEB_STYLE);
  WSContentSendRaw_P(DONGLEM_WEB_BODY);
  WSContentSendRaw_P(DONGLEM_WEB_SCRIPT);
}

bool DongleMWebAuthenticate(void) {
  if (HTTP_USER == Web.state) {
    HandleRoot();
    return false;
  }
  if (!WebAuthenticate()) {
    Webserver->requestAuthentication();
    return false;
  }
  return true;
}

// Validate API requests against the browser-visible origin. Tasmota's generic
// Referer check does not accept direct page loads or hostnames ending in .local.
bool DongleMWebCheckRequest(void) {
  if (!DongleMWebAuthenticate()) { return false; }
  if (Settings->flag5.disable_referer_chk || WifiIsInManagerMode()) { return true; }

  String host = Webserver->header(F("Host"));
  String referer = Webserver->header(F("Referer"));
  if (host.length() && referer.length()) {
    String origin = F("http://");
    origin += host;
    origin += '/';
    origin.toUpperCase();
    referer.toUpperCase();
    if (referer.startsWith(origin)) { return true; }
  }

  AddLog(LOG_LEVEL_INFO, PSTR("DML: Referer '%s' denied for host '%s'"),
         referer.c_str(), host.c_str());
  return false;
}

void DongleMWebStatus(void) {
  if (!DongleMWebCheckRequest()) { return; }
  DongleMAppendStatusJson(true, false);
  WSSend(200, CT_APP_JSON, ResponseData());
}

void DongleMWebAction(void) {
  if (!DongleMWebCheckRequest()) { return; }
  String action = Webserver->arg(F("action"));

  if (action == F("mode")) {
    String mode = Webserver->arg(F("mode"));
    DongleMMode target;
    if (mode == F("APP")) { target = DONGLEM_MODE_APP; }
    else if (mode == F("BOOT")) { target = DONGLEM_MODE_BOOTLOADER; }
    else { DongleMWebJsonReply(400, false, PSTR("Invalid MG24 mode")); return; }
    if (!DongleMSetMode(target)) {
      DongleMWebJsonReply(409, false, PSTR("MG24 is busy")); return;
    }
  } else if (action == F("restart")) {
    if (!DongleMRestart()) {
      DongleMWebJsonReply(409, false, PSTR("MG24 is busy")); return;
    }
  } else if (action == F("baudrate")) {
    uint32_t baudrate;
    if (!DongleMParseUInt(Webserver->arg(F("value")), &baudrate) ||
        !DongleMBaudrateValid(baudrate)) {
      DongleMWebJsonReply(400, false, PSTR("Invalid baudrate")); return;
    }
    if (!DongleMSetBaudrate(baudrate)) {
      DongleMWebJsonReply(409, false, PSTR("MG24 is busy")); return;
    }
  } else if (action == F("port")) {
    uint32_t port;
    if (!DongleMParseUInt(Webserver->arg(F("value")), &port) ||
        !port || port > UINT16_MAX) {
      DongleMWebJsonReply(400, false, PSTR("Invalid TCP port")); return;
    }
    if (!DongleMSetPort(port)) {
      DongleMWebJsonReply(409, false, PSTR("MG24 is busy")); return;
    }
  } else if (action == F("light")) {
    if (!DongleMLightAvailable()) {
      DongleMWebJsonReply(409, false, PSTR("PWM6, PWM7 and PWM8 are not configured"));
      return;
    }

    // Phase 1: read and validate *all* submitted fields into a temporary copy.
    DONGLEM_LIGHT_STATE next = DongleMGetLightState();
    if (Webserver->hasArg(F("power"))) {
      String power = Webserver->arg(F("power"));
      if (power != F("0") && power != F("1")) {
        DongleMWebJsonReply(400, false, PSTR("Invalid LED power")); return;
      }
      next.power = (power == F("1"));
    }
    if (Webserver->hasArg(F("brightness"))) {
      uint32_t brightness;
      if (!DongleMParseUInt(Webserver->arg(F("brightness")), &brightness) ||
          brightness < 1 || brightness > 100) {
        DongleMWebJsonReply(400, false, PSTR("Invalid brightness")); return;
      }
      next.brightness = (uint8_t)brightness;
    }
    if (Webserver->hasArg(F("color"))) {
      String color = Webserver->arg(F("color"));
      if (color.length() != 6) {
        DongleMWebJsonReply(400, false, PSTR("Invalid color")); return;
      }
      for (uint32_t i = 0; i < color.length(); i++) {
        if (!isxdigit((unsigned char)color[i])) {
          DongleMWebJsonReply(400, false, PSTR("Invalid color")); return;
        }
      }
      for (uint32_t i = 0; i < 3; i++) {
        next.color[i] = (uint8_t)strtoul(color.substring(2 * i, 2 * i + 2).c_str(), nullptr, 16);
      }
    }
    if (Webserver->hasArg(F("effect"))) {
      String effect = Webserver->arg(F("effect"));
      if (!DongleMParseLightEffect(effect.c_str(), &next.effect)) {
        DongleMWebJsonReply(400, false, PSTR("Invalid LED effect")); return;
      }
    }

    // Phase 2: commit exactly once. Failed validations never partially mutate state.
    if (!DongleMSetLight(next)) {
      DongleMWebJsonReply(409, false, PSTR("LED unavailable")); return;
    }
  } else if (action == F("theme")) {
    String command;
    if (Webserver->arg(F("value")) == F("light")) {
      command = F("WebColor {\"WebColor\":[\"#000\",\"#fff\",\"#f2f2f2\",\"#000\",\"#fff\",\"#000\",\"#fff\",\"#d21f3c\",\"#087f23\",\"#fff\",\"#147cc1\",\"#0e5f91\",\"#c62828\",\"#8e1d1d\",\"#2e9d52\",\"#237f41\",\"#fff\",\"#777\",\"#111827\",\"#d8ecf8\"]}");
    } else if (Webserver->arg(F("value")) == F("dark")) {
      command = F("WebColor {\"WebColor\":[\"#e8edf5\",\"#111722\",\"#1b2432\",\"#e8edf5\",\"#101722\",\"#72d572\",\"#0b1018\",\"#ff6577\",\"#52c77a\",\"#fff\",\"#268fd0\",\"#1674ad\",\"#d94a4a\",\"#a42f2f\",\"#36a866\",\"#288451\",\"#fff\",\"#657184\",\"#f4f7fb\",\"#20334a\"]}");
    } else {
      DongleMWebJsonReply(400, false, PSTR("Invalid theme")); return;
    }
    ExecuteWebCommand((char*)command.c_str());
    SettingsSaveAll();
    DongleMMarkStateDirty();
  } else {
    DongleMWebJsonReply(400, false, PSTR("Unknown action"));
    return;
  }

  DongleMWebJsonReply(200, true, PSTR("OK"));
}

void DongleMWebNetwork(void) {
  if (!DongleMWebCheckRequest()) { return; }
  String hostname = Webserver->arg(F("hostname"));
  String mode = Webserver->arg(F("mode"));
  if (!DongleMNormalizeHostName(hostname)) {
    DongleMWebJsonReply(400, false, PSTR("mDNS hostname must be one label containing only letters, numbers and hyphens"));
    return;
  }

  uint32_t ip = 0, gateway = 0, subnet = 0, dns = 0;
  if (mode == F("static")) {
    if (!DongleMParseIPv4(Webserver->arg(F("ip")), &ip) ||
        !DongleMParseIPv4(Webserver->arg(F("gateway")), &gateway) ||
        !DongleMParseIPv4(Webserver->arg(F("subnet")), &subnet) ||
        !DongleMParseIPv4(Webserver->arg(F("dns")), &dns) ||
        !ip || !gateway || !dns || !DongleMNetmaskValid(subnet) ||
        ((ip & subnet) != (gateway & subnet)) ||
        !(ip & ~subnet) || ((ip & ~subnet) == ~subnet)) {
      DongleMWebJsonReply(400, false, PSTR("Invalid static IPv4 configuration"));
      return;
    }
    Settings->eth_ipv4_address[0] = ip;
    Settings->eth_ipv4_address[1] = gateway;
    Settings->eth_ipv4_address[2] = subnet;
    Settings->eth_ipv4_address[3] = dns;
    Settings->eth_ipv4_address[4] = 0;
  } else if (mode == F("dhcp")) {
    memset(Settings->eth_ipv4_address, 0, sizeof(Settings->eth_ipv4_address));
  } else if (mode.length()) {
    DongleMWebJsonReply(400, false, PSTR("Invalid IP mode"));
    return;
  }

  SettingsUpdateText(SET_HOSTNAME, hostname.c_str());
  DongleMWebJsonReply(200, true, PSTR("Saved; restarting"));
  TasmotaGlobal.restart_flag = 2;
}

void DongleMWebAddHandlers(void) {
  WebServer_on(PSTR("/donglem/status"), DongleMWebStatus, HTTP_GET);
  WebServer_on(PSTR("/donglem/action"), DongleMWebAction, HTTP_POST);
  WebServer_on(PSTR("/donglem/network"), DongleMWebNetwork, HTTP_POST);
}

#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Home Assistant MQTT discovery
\*********************************************************************************************/

void DongleMHassTopic(char *topic, size_t size, const char *component, const char *suffix) {
  snprintf_P(topic, size, PSTR(HOME_ASSISTANT_DISCOVERY_PREFIX "/%s/%s_%s/config"),
             component, NetworkUniqueId().c_str(), suffix);
}

void DongleMHassPublishConfig(const char *component, const char *suffix, const String &payload) {
  char topic[TOPSZ];
  DongleMHassTopic(topic, sizeof(topic), component, suffix);
  MqttPublishPayload(topic, payload.c_str(), 0, true);
}

String DongleMHassCommandTopic(const char *command) {
  char topic[TOPSZ];
  GetTopic_P(topic, CMND, TasmotaGlobal.mqtt_topic, command);
  return String(topic);
}

void DongleMHassDevice(String &device) {
  String ip = DongleMEthernetConnected() ? EthernetGetIPv4Str() :
              (DongleMWifiConnected() ? WifiGetIPv4Str() : String());
  String address = ip.length() ? ip : (String(SettingsText(SET_HOSTNAME)) + F(".local"));
  device = F("\"device\":{\"identifiers\":[\"donglem_");
  device += NetworkUniqueId();
  device += F("\"],\"name\":\"");
  device += EscapeJSONString(SettingsText(SET_DEVICENAME));
  device += F("\",\"manufacturer\":\"SONOFF\",\"model\":\"Dongle-M\",\"sw_version\":\"");
  device += TasmotaGlobal.version;
  device += F("\",\"configuration_url\":\"http://");
  device += address;
  device += F("/\"}");
}

struct DONGLEM_HASS_CONTEXT {
  String uid;
  String state_topic;
  String mode_state_topic;
  String event_topic;
  String common;
};

// Explicit prototypes suppress Arduino's misplaced .ino auto-prototypes for
// functions whose signatures mention DONGLEM_HASS_CONTEXT.
DONGLEM_HASS_CONTEXT DongleMHassMakeContext(void);
String DongleMHassBegin(const DONGLEM_HASS_CONTEXT &ctx, const char *name,
                        const char *suffix);
void DongleMHassDiscoverLight(const DONGLEM_HASS_CONTEXT &ctx);
void DongleMHassDiscoverSwitch(const DONGLEM_HASS_CONTEXT &ctx);
void DongleMHassDiscoverButton(const DONGLEM_HASS_CONTEXT &ctx);
void DongleMHassDiscoverTCP(const DONGLEM_HASS_CONTEXT &ctx);
void DongleMHassDiscoverBaudrate(const DONGLEM_HASS_CONTEXT &ctx);
void DongleMHassDiscoverEvent(const DONGLEM_HASS_CONTEXT &ctx);
void DongleMHassDiscoverSensor(const DONGLEM_HASS_CONTEXT &ctx, const char *name,
                               const char *suffix, const char *json_path,
                               bool signal_strength);
void DongleMHassDiscoverDiagnostics(const DONGLEM_HASS_CONTEXT &ctx);

DONGLEM_HASS_CONTEXT DongleMHassMakeContext(void) {
  DONGLEM_HASS_CONTEXT ctx;
  ctx.uid = NetworkUniqueId();
  char topic[TOPSZ];
  GetTopic_P(topic, TELE, TasmotaGlobal.mqtt_topic, PSTR("DONGLEM"));
  ctx.state_topic = topic;
  GetTopic_P(topic, STAT, TasmotaGlobal.mqtt_topic, PSTR("DONGLEM_MODE"));
  ctx.mode_state_topic = topic;
  GetTopic_P(topic, STAT, TasmotaGlobal.mqtt_topic, PSTR("DONGLEM_EVENT"));
  ctx.event_topic = topic;
  GetTopic_P(topic, TELE, TasmotaGlobal.mqtt_topic, PSTR("LWT"));
  ctx.common = F(",\"availability_topic\":\"");
  ctx.common += topic;
  ctx.common += F("\",\"payload_available\":\"Online\",\"payload_not_available\":\"Offline\",");
  String device;
  DongleMHassDevice(device);
  ctx.common += device;
  ctx.common += '}';
  return ctx;
}

String DongleMHassBegin(const DONGLEM_HASS_CONTEXT &ctx, const char *name,
                        const char *suffix) {
  String payload = F("{\"name\":\"");
  payload += name;
  payload += F("\",\"unique_id\":\"");
  payload += ctx.uid;
  payload += '_';
  payload += suffix;
  payload += '"';
  return payload;
}

void DongleMHassDiscoverLight(const DONGLEM_HASS_CONTEXT &ctx) {
  if (!DongleMLightAvailable()) {
    DongleMHassPublishConfig("light", "led", String());
    return;
  }
  String payload = DongleMHassBegin(ctx, "LED", "led");
  payload += F(",\"schema\":\"basic\",\"brightness_scale\":100,");
  payload += F("\"effect_list\":[\"Network\",\"Solid\",\"Blink\"],");
  payload += F("\"state_topic\":\""); payload += ctx.state_topic;
  payload += F("\",\"state_value_template\":\"{{ 'ON' if value_json.DongleM.Light.Power else 'OFF' }}\",");
  payload += F("\"brightness_state_topic\":\""); payload += ctx.state_topic;
  payload += F("\",\"brightness_value_template\":\"{{ value_json.DongleM.Light.Brightness }}\",");
  payload += F("\"rgb_state_topic\":\""); payload += ctx.state_topic;
  payload += F("\",\"rgb_value_template\":\"{{ value_json.DongleM.Light.RGB | join(',') }}\",");
  payload += F("\"effect_state_topic\":\""); payload += ctx.state_topic;
  payload += F("\",\"effect_value_template\":\"{{ value_json.DongleM.Light.Effect }}\",");
  payload += F("\"command_topic\":\""); payload += DongleMHassCommandTopic(PSTR("DongleMLightPower"));
  payload += F("\",\"brightness_command_topic\":\""); payload += DongleMHassCommandTopic(PSTR("DongleMLightBrightness"));
  payload += F("\",\"rgb_command_topic\":\""); payload += DongleMHassCommandTopic(PSTR("DongleMLightColor"));
  payload += F("\",\"effect_command_topic\":\""); payload += DongleMHassCommandTopic(PSTR("DongleMLightEffect"));
  payload += '"'; payload += ctx.common;
  DongleMHassPublishConfig("light", "led", payload);
}

void DongleMHassDiscoverSwitch(const DONGLEM_HASS_CONTEXT &ctx) {
  String payload = DongleMHassBegin(ctx, "MG24 Bootloader Mode", "mg24_boot");
  payload += F(",\"state_topic\":\""); payload += ctx.mode_state_topic;
  payload += F("\",\"state_on\":\"BOOT\",\"state_off\":\"APP\",");
  payload += F("\"command_topic\":\""); payload += DongleMHassCommandTopic(PSTR("DongleMMode"));
  payload += F("\",\"payload_on\":\"BOOT\",\"payload_off\":\"APP\"");
  payload += ctx.common;
  DongleMHassPublishConfig("switch", "mg24_boot", payload);
}

void DongleMHassDiscoverButton(const DONGLEM_HASS_CONTEXT &ctx) {
  String payload = DongleMHassBegin(ctx, "MG24 Restart", "mg24_restart");
  payload += F(",\"command_topic\":\""); payload += DongleMHassCommandTopic(PSTR("DongleMRestart"));
  payload += F("\",\"payload_press\":\"1\"");
  payload += ctx.common;
  DongleMHassPublishConfig("button", "mg24_restart", payload);
}

void DongleMHassDiscoverTCP(const DONGLEM_HASS_CONTEXT &ctx) {
  String payload = DongleMHassBegin(ctx, "UART over TCP", "uart_tcp");
  payload += F(",\"device_class\":\"connectivity\",\"state_topic\":\"");
  payload += ctx.state_topic;
  payload += F("\",\"value_template\":\"{{ 'ON' if value_json.DongleM.TCP.Connected else 'OFF' }}\",");
  payload += F("\"payload_on\":\"ON\",\"payload_off\":\"OFF\"");
  payload += ctx.common;
  DongleMHassPublishConfig("binary_sensor", "uart_tcp", payload);
}

void DongleMHassDiscoverBaudrate(const DONGLEM_HASS_CONTEXT &ctx) {
  String payload = DongleMHassBegin(ctx, "MG24 Baud Rate", "mg24_baud");
  payload += F(",\"entity_category\":\"config\",\"options\":[\"115200\",\"460800\",\"921600\"],");
  payload += F("\"state_topic\":\""); payload += ctx.state_topic;
  payload += F("\",\"value_template\":\"{{ value_json.DongleM.MG24.AppBaudrate | string }}\",");
  payload += F("\"command_topic\":\""); payload += DongleMHassCommandTopic(PSTR("DongleMBaudrate"));
  payload += '"'; payload += ctx.common;
  DongleMHassPublishConfig("select", "mg24_baud", payload);
}

void DongleMHassDiscoverEvent(const DONGLEM_HASS_CONTEXT &ctx) {
  String payload = DongleMHassBegin(ctx, "Button", "button");
  payload += F(",\"device_class\":\"button\",\"state_topic\":\"");
  payload += ctx.event_topic;
  // The event payload already contains {"event_type": ..., "time": ...}.
  // HA exposes additional event keys (including time) as event attributes.
  payload += F("\",\"event_types\":[\"single_click\",\"double_click\",\"long_press\",\"long_release\"]");
  payload += ctx.common;
  DongleMHassPublishConfig("event", "button", payload);
}

void DongleMHassDiscoverSensor(const DONGLEM_HASS_CONTEXT &ctx, const char *name,
                               const char *suffix, const char *json_path, bool signal_strength = false) {
  String payload = DongleMHassBegin(ctx, name, suffix);
  payload += F(",\"entity_category\":\"diagnostic\",\"state_topic\":\"");
  payload += ctx.state_topic;
  payload += F("\",\"value_template\":\"{{ value_json.DongleM.");
  payload += json_path;
  payload += F(" }}\"");
  if (signal_strength) {
    payload += F(",\"device_class\":\"signal_strength\",\"state_class\":\"measurement\",\"unit_of_measurement\":\"dBm\"");
  }
  payload += ctx.common;
  DongleMHassPublishConfig("sensor", suffix, payload);
}

void DongleMHassDiscoverDiagnostics(const DONGLEM_HASS_CONTEXT &ctx) {
  DongleMHassDiscoverSensor(ctx, "Firmware Version", "firmware_version", "FirmwareVersion");
  DongleMHassDiscoverSensor(ctx, "Ethernet IP", "ethernet_ip", "Ethernet.IP");
  DongleMHassDiscoverSensor(ctx, "Wi-Fi IP", "wifi_ip", "Wifi.IP");
  DongleMHassDiscoverSensor(ctx, "Wi-Fi RSSI", "wifi_rssi", "Wifi.RSSI", true);
  DongleMHassDiscoverSensor(ctx, "Uptime", "uptime", "Uptime");
  DongleMHassDiscoverSensor(ctx, "Wi-Fi SSID", "wifi_ssid", "Wifi.SSID");
  DongleMHassDiscoverSensor(ctx, "MG24 Mode", "mg24_mode", "MG24.Mode");
}

void DongleMHassDiscovery(void) {
  DONGLEM_HASS_CONTEXT ctx = DongleMHassMakeContext();
  DongleMHassDiscoverLight(ctx);
  DongleMHassDiscoverSwitch(ctx);
  DongleMHassDiscoverButton(ctx);
  DongleMHassDiscoverTCP(ctx);
  DongleMHassDiscoverBaudrate(ctx);
  DongleMHassDiscoverEvent(ctx);
  DongleMHassDiscoverDiagnostics(ctx);
}

/*********************************************************************************************\
 * Driver lifecycle
\*********************************************************************************************/

void DongleMPreInit(void) {
  DongleMSettingsLoad(false);
  memset(&DongleM, 0, sizeof(DongleM));
  DongleM.requested_mode = DONGLEM_MODE_APP;
  DongleM.active_mode = DONGLEM_MODE_APP;
  DongleM.transition = DONGLEM_TRANSITION_IDLE;
  DongleM.light_phase_on = true;
  DongleM.light_deadline = millis() + DONGLEM_BLINK_TIME;
  strlcpy(DongleM.last_button_event, PSTR(""), sizeof(DongleM.last_button_event));
  strlcpy(DongleM.last_button_time, PSTR(""), sizeof(DongleM.last_button_time));

  pinMode(DONGLEM_BOOT_PIN, OUTPUT);
  digitalWrite(DONGLEM_BOOT_PIN, LOW);
  pinMode(DONGLEM_RESET_PIN, OUTPUT);
  digitalWrite(DONGLEM_RESET_PIN, HIGH);

  // GPIO/PWM setup has already completed before FUNC_PRE_INIT. Apply the
  // persisted light state now so the startup effect does not wait for
  // FUNC_INIT and network initialization.
  DongleMLightEvery250ms(true);
}

void DongleMInit(void) {
  DongleMSetMode(DONGLEM_MODE_APP);
}

bool Xdrv124(uint32_t function) {
  switch (function) {
    case FUNC_PRE_INIT:
      DongleMPreInit();
      break;
    case FUNC_INIT:
      DongleMInit();
      break;
    case FUNC_EVERY_50_MSECOND:
      DongleMTransitionEvery50ms();
      break;
    case FUNC_EVERY_250_MSECOND:
      DongleMLightEvery250ms();
      DongleMProcessPendingEvery250ms();
      break;
    case FUNC_EVERY_SECOND:
      DongleMConnectionEverySecond();
      break;
    case FUNC_RESET_SETTINGS:
      DongleM.settings_dirty = false;
      DongleMSettingsLoad(true);
      break;
    case FUNC_RESTORE_SETTINGS:
      return DongleMSettingsRestore();
    case FUNC_SAVE_SETTINGS:
      DongleMFlushSettings();
      if (!DongleM.settings_dirty) { DongleMSettingsSave(); }
      break;
    // Do not append the entire DongleM JSON to standard Tasmota telemetry.
    // That may create duplicate auto-discovered entities for each nested field.
    // tele/.../DONGLEM is still published explicitly by DongleMPublishState().
    case FUNC_AFTER_TELEPERIOD:
      DongleMPublishState();
      break;
    case FUNC_BUTTON_PRESSED:
      return DongleMButtonPressed();
    case FUNC_BUTTON_MULTI_PRESSED:
      return DongleMButtonMultiPressed();
    case FUNC_MQTT_SUBSCRIBE:
      DongleMHassDiscovery();
      DongleMPublishModeState();
      DongleMPublishState();
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_ADD_MAIN_BUTTON:
      DongleMWebRootContent();
      break;
    case FUNC_WEB_ADD_HANDLER:
      DongleMWebAddHandlers();
      break;
#endif  // USE_WEBSERVER
    case FUNC_COMMAND:
      return DecodeCommand(kDongleMCommands, DongleMCommand);
    case FUNC_ACTIVE:
      return true;
  }
  return false;
}

#endif  // ESP32 && USE_DONGLE_M
