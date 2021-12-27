/*
  xdrv_12_discovery.ino - MQTT Discovery support for Tasmota

  Copyright (C) 2021  Erik Montnemery, Federico Leoni and Theo Arends

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

#ifdef USE_TASMOTA_DISCOVERY
#undef USE_HOME_ASSISTANT
/*********************************************************************************************\
 * Tasmota discovery
 *
 * A version of xdrv_12_home_assistant supporting the new Tasmota Discovery used by
 * latest versions of Home Assistant or TasmoManager.
 *
 * SetOption19 0   - [DiscoverOff 0] [Discover 1] Enables discovery (default)
 * SetOption19 1   - [DiscoverOff 1] [Discover 0] Disables discovery and removes retained message from MQTT server
 * SetOption73 1   - [DiscoverButton] Enable discovery for buttons
 * SetOption114 1  - [DiscoverSwitch] Enable discovery for switches
\*********************************************************************************************/

#define XDRV_12     12

void TasDiscoverMessage(void) {
  Response_P(PSTR("{\"ip\":\"%_I\","                           // IP Address
                   "\"dn\":\"%s\","                            // Device Name
                   "\"fn\":["),                                // Friendly Names (start)
                   (uint32_t)WiFi.localIP(),
                   SettingsText(SET_DEVICENAME));

  uint32_t maxfn = (TasmotaGlobal.devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : (!TasmotaGlobal.devices_present) ? 1 : TasmotaGlobal.devices_present;
  for (uint32_t i = 0; i < MAX_FRIENDLYNAMES; i++) {
    char fname[TOPSZ];
    snprintf_P(fname, sizeof(fname), PSTR("\"%s\""), EscapeJSONString(SettingsText(SET_FRIENDLYNAME1 +i)).c_str());
    ResponseAppend_P(PSTR("%s%s"), (i > 0 ? "," : ""), (i < maxfn) ? fname : PSTR("null"));
  }

  bool TuyaMod = false;
#ifdef USE_TUYA_MCU
  TuyaMod = IsModuleTuya();
#endif
  bool iFanMod = false;
#ifdef ESP8266
  iFanMod = ((SONOFF_IFAN02 == TasmotaGlobal.module_type) || (SONOFF_IFAN03 == TasmotaGlobal.module_type));
#endif  // ESP8266

  ResponseAppend_P(PSTR("],"                                   // Friendly Names (end)
                   "\"hn\":\"%s\","                            // Host Name
                   "\"mac\":\"%s\","                           // Full MAC as Device id
                   "\"md\":\"%s\","                            // Module or Template Name
                   "\"ty\":%d,\"if\":%d,"                      // Flag for TuyaMCU and Ifan devices
                   "\"ofln\":\"" MQTT_LWT_OFFLINE "\","        // Payload Offline
                   "\"onln\":\"" MQTT_LWT_ONLINE "\","         // Payload Online
                   "\"state\":[\"%s\",\"%s\",\"%s\",\"%s\"],"  // State text for "OFF","ON","TOGGLE","HOLD"
                   "\"sw\":\"%s\","                            // Software Version
                   "\"t\":\"%s\","                             // Topic
                   "\"ft\":\"%s\","                            // Full Topic
                   "\"tp\":[\"%s\",\"%s\",\"%s\"],"            // Topics for command, stat and tele
                   "\"rl\":["),                                // Relays (start)
                   TasmotaGlobal.hostname,
                   NetworkUniqueId().c_str(),
                   ModuleName().c_str(),
                   TuyaMod, iFanMod,
                   GetStateText(0), GetStateText(1), GetStateText(2), GetStateText(3),
                   TasmotaGlobal.version,
                   TasmotaGlobal.mqtt_topic,
                   SettingsText(SET_MQTT_FULLTOPIC),
                   SettingsText(SET_MQTTPREFIX1),
                   SettingsText(SET_MQTTPREFIX2),
                   SettingsText(SET_MQTTPREFIX3));

  uint8_t light_idx = MAX_RELAYS + 1;                          // Will store the starting position of the lights
  uint8_t light_subtype = 0;
  bool light_controller_isCTRGBLinked = false;
#ifdef USE_LIGHT
  light_subtype = Light.subtype;
  if (light_subtype > LST_NONE) {
    light_controller_isCTRGBLinked = light_controller.isCTRGBLinked();
    if (!light_controller_isCTRGBLinked) {                     // One or two lights present
      light_idx = TasmotaGlobal.devices_present - 2;
    } else {
      light_idx = TasmotaGlobal.devices_present - 1;
    }
  }

  if ((Light.device > 0) && Settings->flag3.pwm_multi_channels) {  // How many relays are light devices?
    light_idx = TasmotaGlobal.devices_present - light_subtype;
  }
#endif  // USE_LIGHT

  uint16_t Relay[MAX_RELAYS] = { 0 };                          // Base array to store the relay type
  uint16_t Shutter[MAX_RELAYS] = { 0 };                        // Array to store a temp list for shutters
  for (uint32_t i = 0; i < MAX_RELAYS; i++) {
    if (i < TasmotaGlobal.devices_present) {

#ifdef USE_SHUTTER
      if (Settings->flag3.shutter_mode) {
        for (uint32_t k = 0; k < MAX_SHUTTERS; k++) {
          if (Settings->shutter_startrelay[k] > 0) {
            Shutter[Settings->shutter_startrelay[k]-1] = Shutter[Settings->shutter_startrelay[k]] = 1;
          } else {
            // terminate loop at first INVALID Settings->shutter_startrelay[i].
            break;
          }
        }
      }
#endif  // USE_SHUTTER

      if (Shutter[i] != 0) {                                   // Check if there are shutters present
        Relay[i] = 3;                                          // Relay is a shutter
      } else {
        if (i >= light_idx || (iFanMod && (0 == i))) {          // First relay on Ifan controls the light
          Relay[i] = 2;                                        // Relay is a light
        } else {
          if (!iFanMod) {                                      // Relays 2-4 for ifan are controlled by FANSPEED and don't need to be present if TasmotaGlobal.module_type = SONOFF_IFAN02 or SONOFF_IFAN03
            Relay[i] = 1;                                      // Simple Relay
          }
        }
      }
    }
    ResponseAppend_P(PSTR("%s%d"), (i > 0 ? "," : ""), Relay[i]); // Vector for the Official Integration
  }

  ResponseAppend_P(PSTR("],"                                   // Relays (end)
                        "\"swc\":["));                         // Switch modes (start)

  // Enable Discovery for Switches only if SetOption114 is enabled
  for (uint32_t i = 0; i < MAX_SWITCHES; i++) {
    ResponseAppend_P(PSTR("%s%d"), (i > 0 ? "," : ""), (PinUsed(GPIO_SWT1, i) && Settings->flag5.mqtt_switches) ? Settings->switchmode[i] : -1);
  }

  ResponseAppend_P(PSTR("],"                                   // Switch modes (end)
                        "\"swn\":["));                         // Switch names (start)

  // Enable Discovery for Switches only if SetOption114 is enabled
  for (uint32_t i = 0; i < MAX_SWITCHES; i++) {
    char sname[TOPSZ];
    snprintf_P(sname, sizeof(sname), PSTR("\"%s\""), GetSwitchText(i).c_str());
    ResponseAppend_P(PSTR("%s%s"), (i > 0 ? "," : ""), (PinUsed(GPIO_SWT1, i) && Settings->flag5.mqtt_switches) ? sname : PSTR("null"));
  }

  ResponseAppend_P(PSTR("],"                                   // Switch names (end)
                        "\"btn\":["));                         // Button flag (start)

  bool SerialButton = false;
  // Enable Discovery for Buttons only if SetOption73 is enabled
  for (uint32_t i = 0; i < MAX_KEYS; i++) {
#ifdef ESP8266
    SerialButton = ((0 == i) && (SONOFF_DUAL == TasmotaGlobal.module_type ));
#endif  // ESP8266
    ResponseAppend_P(PSTR("%s%d"), (i > 0 ? "," : ""), (SerialButton ? 1 : (PinUsed(GPIO_KEY1, i)) && Settings->flag3.mqtt_buttons));
  }

  ResponseAppend_P(PSTR("],"                                   // Button flag (end)
                        "\"so\":{\"4\":%d,"                    // SetOptions
                                "\"11\":%d,"
                                "\"13\":%d,"
                                "\"17\":%d,"
                                "\"20\":%d,"
                                "\"30\":%d,"
                                "\"68\":%d,"
                                "\"73\":%d,"
                                "\"82\":%d,"
                                "\"114\":%d,"
                                "\"117\":%d},"
                        "\"lk\":%d,"                           // Light CTRGB linked
                        "\"lt_st\":%d,"                        // Light SubType
                        "\"sho\":["),                          // Shutter Options (start)
                        Settings->flag.mqtt_response,
                        Settings->flag.button_swap,
                        Settings->flag.button_single,
                        Settings->flag.decimal_text,
                        Settings->flag.not_power_linked,
                        Settings->flag.hass_light,
                        Settings->flag3.pwm_multi_channels,
                        Settings->flag3.mqtt_buttons,
                        Settings->flag4.alexa_ct_range,
                        Settings->flag5.mqtt_switches,
                        Settings->flag5.fade_fixed_duration,
                        light_controller_isCTRGBLinked,
                        light_subtype);

  for (uint32_t i = 0; i < MAX_SHUTTERS; i++) {
#ifdef USE_SHUTTER
    ResponseAppend_P(PSTR("%s%d"), (i > 0 ? "," : ""), Settings->shutter_options[i]);
#else
    ResponseAppend_P(PSTR("%s0"), (i > 0 ? "," : ""));
#endif  // USE_SHUTTER
  }

  ResponseAppend_P(PSTR("],"                                   // Shutter Options (end)
                        "\"ver\":1}"));                        // Discovery version
}

void TasDiscovery(void) {
  TasmotaGlobal.masterlog_level = LOG_LEVEL_DEBUG_MORE;        // Hide topic on clean and remove use weblog 4 to show it

  ResponseClear();                                             // Clear retained message
  if (!Settings->flag.hass_discovery) {                         // SetOption19 - Clear retained message
    TasDiscoverMessage();                                      // Build discovery message
  }
  char stopic[TOPSZ];
  snprintf_P(stopic, sizeof(stopic), PSTR("tasmota/discovery/%s/config"), NetworkUniqueId().c_str());
  MqttPublish(stopic, true);

  if (!Settings->flag.hass_discovery) {                         // SetOption19 - Clear retained message
    Response_P(PSTR("{\"sn\":"));
    MqttShowSensor(true);
    ResponseAppend_P(PSTR(",\"ver\":1}"));
  }
  snprintf_P(stopic, sizeof(stopic), PSTR("tasmota/discovery/%s/sensors"), NetworkUniqueId().c_str());
  MqttPublish(stopic, true);

  TasmotaGlobal.masterlog_level = LOG_LEVEL_NONE;              // Restore WebLog state
}

void TasRediscover(void) {
  TasmotaGlobal.discovery_counter = 1;                         // Delayed discovery or clear retained messages
}

void TasDiscoverInit(void) {
  if (ResetReason() != REASON_DEEP_SLEEP_AWAKE) {
    Settings->flag.hass_discovery = 0;                          // SetOption19 - Enable Tasmota discovery and Disable legacy Hass discovery
    TasmotaGlobal.discovery_counter = 10;                      // Delayed discovery
  }
}

/*********************************************************************************************\
 * Commands
 *
 * Discover 0        - Disables discovery and removes retained message from MQTT server
 * Discover 1        - Enables discovery (default)
 * DiscoverOff 0     - Enables discovery (default)
 * DiscoverOff 1     - Disables discovery and removes retained message from MQTT server
 * DiscoverButton 1  - Enable discovery for buttons
 * DiscoverSwitch 1  - Enable discovery for switches
\*********************************************************************************************/

const char kTasDiscoverCommands[] PROGMEM = "Discover|"        // Prefix
  // SetOption synonyms
  "Off|Button|Switch|"
  // Commands
  "|";

SO_SYNONYMS(kTasDiscoverSynonyms,
  19, 73, 114 );

void (* const TasDiscoverCommand[])(void) PROGMEM = {
  &CmndTasDiscover };

void CmndTasDiscover(void) {
  if (XdrvMailbox.payload >= 0) {
    Settings->flag.hass_discovery = !(XdrvMailbox.payload & 1);
    TasRediscover();
  }
  ResponseCmndChar(GetStateText(!Settings->flag.hass_discovery));
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv12(uint8_t function) {
  bool result = false;

  if (Settings->flag.mqtt_enabled) {                            // SetOption3 - Enable MQTT
    switch (function) {
    case FUNC_EVERY_SECOND:
      if (TasmotaGlobal.discovery_counter) {
        TasmotaGlobal.discovery_counter--;
        if (!TasmotaGlobal.discovery_counter) {
          TasDiscovery();                                      // Send the topics for discovery
        }
      }
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kTasDiscoverCommands, TasDiscoverCommand, kTasDiscoverSynonyms);
      break;
    case FUNC_MQTT_SUBSCRIBE:
      if (0 == Mqtt.initial_connection_state) {
        TasRediscover();
      }
      break;
    case FUNC_MQTT_INIT:
      TasDiscoverInit();
      break;
    }
  }
  return result;
}

#endif  // USE_TASMOTA_DISCOVERY
