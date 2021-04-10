/*
  xdrv_12_discovery.ino - Discovery support for Tasmota

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
 * A stripped down version of xdrv_12_home_assistant to be used as alternative for TasmoManager
 *
 * SetOption19 0  - Enables discovery
 * SetOption19 1  - Disables discovery and removes retained message from MQTT server
 * SetOption73 1  - Enable discovery for buttons
 * SetOption114 1 - Enable discovery for switches
\*********************************************************************************************/

#define XDRV_12     12

const char TASMOTA_DISCOVER_DEVICE[] PROGMEM =                      // Basic parameters for Discovery
  "{\"ip\":\"%_I\","                                                // IP Address
  "\"dn\":\"%s\","                                                  // Device Name
  "\"fn\":[%s],"                                                    // Friendly Names
  "\"hn\":\"%s\","                                                  // Host Name
  "\"mac\":\"%s\","                                                 // Full MAC as Device id
  "\"md\":\"%s\","                                                  // Module or Template Name
  "\"ty\":%d,\"if\":%d,"                                            // Flag for TuyaMCU and Ifan devices
  "\"ofln\":\"" MQTT_LWT_OFFLINE "\","                              // Payload Offline
  "\"onln\":\"" MQTT_LWT_ONLINE "\","                               // Payload Online
  "\"state\":[\"%s\",\"%s\",\"%s\",\"%s\"],"                        // State text for "OFF","ON","TOGGLE","HOLD"
  "\"sw\":\"%s\","                                                  // Software Version
  "\"t\":\"%s\","                                                   // Topic
  "\"ft\":\"%s\","                                                  // Full Topic
  "\"tp\":[\"%s\",\"%s\",\"%s\"],"                                  // Topics for command, stat and tele
  "\"rl\":[%s],\"swc\":[%s],\"swn\":[%s],\"btn\":[%s],"             // Inputs / Outputs
  "\"so\":{\"4\":%d,\"11\":%d,\"13\":%d,\"17\":%d,\"20\":%d,"       // SetOptions
  "\"30\":%d,\"68\":%d,\"73\":%d,\"82\":%d,\"114\":%d,\"117\":%d},"
  "\"lk\":%d,\"lt_st\":%d,\"sho\":[%s],\"ver\":1}";                 // Light SubType, Shutter Options and Discovery version

struct {
  uint8_t init_step;
} TasDiscoverData;

void TasDiscovery(void) {
  bool iFan = false;
#ifdef ESP8266
  if ((SONOFF_IFAN02 == TasmotaGlobal.module_type) || (SONOFF_IFAN03 == TasmotaGlobal.module_type)) { iFan = true; }
#endif  // ESP8266

  uint8_t lightidx = MAX_RELAYS + 1;     // Will store the starting position of the lights
  if (Light.subtype > LST_NONE) {
    if (!light_controller.isCTRGBLinked()) { // One or two lights present
      lightidx = TasmotaGlobal.devices_present - 2;
    } else {
      lightidx = TasmotaGlobal.devices_present - 1;
    }
  }

  if ((Light.device > 0) && Settings.flag3.pwm_multi_channels) { // How many relays are light devices?
    lightidx = TasmotaGlobal.devices_present - Light.subtype;
  }

  uint16_t Relay[MAX_RELAYS] = { 0 };    // Base array to store the relay type
  uint16_t Shutter[MAX_RELAYS] = { 0 };  // Array to store a temp list for shutters
  char RelLst[MAX_RELAYS*2] = { 0 };     // Relay as a char list, "0,0,0,0,0,0,0,0"
  for (uint32_t i = 0; i < MAX_RELAYS; i++) {
    if (i < TasmotaGlobal.devices_present) {

#ifdef USE_SHUTTER
      if (Settings.flag3.shutter_mode) {
        for (uint32_t k = 0; k < MAX_SHUTTERS; k++) {
          if (0 == Settings.shutter_startrelay[k]) {
            break;
          } else {
            if (Settings.shutter_startrelay[k] > 0 && Settings.shutter_startrelay[k] <= MAX_SHUTTER_RELAYS) {
              Shutter[Settings.shutter_startrelay[k]-1] = Shutter[Settings.shutter_startrelay[k]] = 1;
            }
          }
        }
      }
#endif  // USE_SHUTTER

      if (Shutter[i] != 0) {    // Check if there are shutters present
        Relay[i] = 3;           // Relay is a shutter
      } else {
        if (i >= lightidx || (iFan && i == 0)) { // First relay on Ifan controls the light
          Relay[i] = 2;         // Relay is a light
        } else {
          if (!iFan) {          // Relays 2-4 for ifan are controlled by FANSPEED and don't need to be present if TasmotaGlobal.module_type = SONOFF_IFAN02 or SONOFF_IFAN03
            Relay[i] = 1;       // Simple Relay
          }
        }
      }
    }
    snprintf_P(RelLst, sizeof(RelLst), PSTR("%s%s%d"), RelLst, (i > 0 ? "," : ""), Relay[i]); // Vector for the Official Integration
  }

  bool TuyaMod = false;
  bool iFanMod = false;
#ifdef ESP8266
  if ((TUYA_DIMMER == TasmotaGlobal.module_type) || (SK03_TUYA == TasmotaGlobal.module_type)) { TuyaMod = true; };
  if ((SONOFF_IFAN02 == TasmotaGlobal.module_type) || (SONOFF_IFAN03 == TasmotaGlobal.module_type)) { iFanMod = true; };
#endif // ESP8266

  char friendly_name[200];
  friendly_name[0] = '\0';
  uint32_t maxfn = (TasmotaGlobal.devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : (!TasmotaGlobal.devices_present) ? 1 : TasmotaGlobal.devices_present;
  for (uint32_t i = 0; i < MAX_FRIENDLYNAMES; i++) {
    char fname[TOPSZ];
    snprintf_P(fname, sizeof(fname), PSTR("\"%s\""), EscapeJSONString(SettingsText(SET_FRIENDLYNAME1 +i)).c_str());
    snprintf_P(friendly_name, sizeof(friendly_name), PSTR("%s%s%s"), friendly_name, (i > 0 ? "," : ""), (i < maxfn) ? fname : PSTR("null"));
  }

  char switch_mode[90];
  switch_mode[0] = '\0';
  char switch_name[300];
  switch_name[0] = '\0';
  // Enable Discovery for Switches only if SetOption114 is enabled
  for (uint32_t i = 0; i < MAX_SWITCHES; i++) {
    char sname[TOPSZ];
    snprintf_P(sname, sizeof(sname), PSTR("\"%s\""), GetSwitchText(i).c_str());
    snprintf_P(switch_mode, sizeof(switch_mode), PSTR("%s%s%d"), switch_mode, (i > 0 ? "," : ""), (PinUsed(GPIO_SWT1, i) & Settings.flag5.mqtt_switches) ? Settings.switchmode[i] : -1);
    snprintf_P(switch_name, sizeof(switch_name), PSTR("%s%s%s"), switch_name, (i > 0 ? "," : ""), (PinUsed(GPIO_SWT1, i) & Settings.flag5.mqtt_switches) ? sname : PSTR("null"));
  }

  bool SerialButton = false;
  char button_flag[90];
  button_flag[0] = '\0';
  // Enable Discovery for Buttons only if SetOption73 is enabled
  for (uint32_t i = 0; i < MAX_KEYS; i++) {
#ifdef ESP8266
    SerialButton = ((0 == i) && (SONOFF_DUAL == TasmotaGlobal.module_type ));
#endif // ESP8266
    snprintf_P(button_flag, sizeof(button_flag), PSTR("%s%s%d"), button_flag, (i > 0 ? "," : ""), (SerialButton ? 1 : (PinUsed(GPIO_KEY1, i)) & Settings.flag3.mqtt_buttons));
  }

  char shutter_option[90];
  shutter_option[0] = '\0';
  for (uint32_t i = 0; i < MAX_SHUTTERS; i++) {
#ifdef USE_SHUTTER
    snprintf_P(shutter_option, sizeof(shutter_option), PSTR("%s%s%d"), shutter_option, (i > 0 ? "," : ""), Settings.shutter_options[i]);
#else
    snprintf_P(shutter_option, sizeof(shutter_option), PSTR("%s%s0"), shutter_option, (i > 0 ? "," : ""));
#endif  // USE_SHUTTER
  }

  // Full 12 chars MAC address as ID
  String mac_address = WiFi.macAddress();
  mac_address.replace(":", "");
  char unique_id[30];
  snprintf_P(unique_id, sizeof(unique_id), PSTR("%s"), mac_address.c_str());

  TasmotaGlobal.masterlog_level = LOG_LEVEL_DEBUG_MORE;  // Hide topic on clean and remove use weblog 4 to show it

  ResponseClear();                       // Clear retained message
  if (!Settings.flag.hass_discovery) {   // SetOption19 - Clear retained message
    Response_P(TASMOTA_DISCOVER_DEVICE, (uint32_t)WiFi.localIP(), SettingsText(SET_DEVICENAME),
              friendly_name, TasmotaGlobal.hostname, unique_id, ModuleName().c_str(), TuyaMod, iFanMod, GetStateText(0), GetStateText(1), GetStateText(2), GetStateText(3),
              TasmotaGlobal.version, TasmotaGlobal.mqtt_topic, SettingsText(SET_MQTT_FULLTOPIC), PSTR(SUB_PREFIX), PSTR(PUB_PREFIX), PSTR(PUB_PREFIX2), RelLst, switch_mode, switch_name,
              button_flag, Settings.flag.mqtt_response, Settings.flag.button_swap, Settings.flag.button_single, Settings.flag.decimal_text, Settings.flag.not_power_linked,
              Settings.flag.hass_light, Settings.flag3.pwm_multi_channels, Settings.flag3.mqtt_buttons, Settings.flag4.alexa_ct_range, Settings.flag5.mqtt_switches,
              Settings.flag5.fade_fixed_duration, light_controller.isCTRGBLinked(), Light.subtype, shutter_option);
  }
  char stopic[TOPSZ];
  snprintf_P(stopic, sizeof(stopic), PSTR("tasmota/discovery/%s/config"), unique_id);
  MqttPublish(stopic, true);

  if (!Settings.flag.hass_discovery) {   // SetOption19 - Clear retained message
    Response_P(PSTR("{\"sn\":"));
    MqttShowSensor();
    ResponseAppend_P(PSTR(",\"ver\":1}"));
  }
  snprintf_P(stopic, sizeof(stopic), PSTR("tasmota/discovery/%s/sensors"), unique_id);
  MqttPublish(stopic, true);

  TasmotaGlobal.masterlog_level = LOG_LEVEL_NONE;  // Restore WebLog state
}

void TasRediscover(void) {
  TasDiscoverData.init_step = 1;         // Delayed discovery or clear retained messages
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv12(uint8_t function) {
  bool result = false;

  if (Settings.flag.mqtt_enabled) {      // SetOption3 - Enable MQTT
    switch (function) {
    case FUNC_EVERY_SECOND:
      if (TasDiscoverData.init_step) {
        TasDiscoverData.init_step--;
        if (!TasDiscoverData.init_step) {
          TasDiscovery();                // Send the topics for discovery
        }
      }
      break;
    case FUNC_MQTT_INIT:
      TasDiscoverData.init_step = 10;    // Delayed discovery
      break;
    }
  }
  return result;
}

#endif  // USE_TASMOTA_DISCOVERY