/*
  xdrv_12_home_assistant.ino - home assistant support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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

#ifdef USE_HOME_ASSISTANT

#define XDRV_12             12

const char HASS_DISCOVER_RELAY[] PROGMEM =
  "{\"name\":\"%s\","                              // dualr2 1
  "\"command_topic\":\"%s\","                      // cmnd/dualr2/POWER2
  "\"state_topic\":\"%s\","                        // stat/dualr2/RESULT  (implies "\"optimistic\":\"false\",")
  "\"value_template\":\"{{value_json.%s}}\","      // POWER2
  "\"payload_off\":\"%s\","                        // OFF
  "\"payload_on\":\"%s\","                         // ON
//  "\"optimistic\":\"false\","                    // false is Hass default when state_topic is set
  "\"availability_topic\":\"%s\","                 // tele/dualr2/LWT
  "\"payload_available\":\"" D_ONLINE "\","        // Online
  "\"payload_not_available\":\"" D_OFFLINE "\"";   // Offline

const char HASS_DISCOVER_BUTTON_SWITCH[] PROGMEM =
  "{\"name\":\"%s\","                              // dualr2 1 BTN
  "\"state_topic\":\"%s\","                        // cmnd/dualr2/POWER  (implies "\"optimistic\":\"false\",")
//  "\"value_template\":\"{{value_json.%s}}\","      // POWER2
  "\"payload_on\":\"%s\","                         // TOGGLE / ON
//  "\"optimistic\":\"false\","                    // false is Hass default when state_topic is set
  "\"availability_topic\":\"%s\","                 // tele/dualr2/LWT
  "\"payload_available\":\"" D_ONLINE "\","        // Online
  "\"payload_not_available\":\"" D_OFFLINE "\"";   // Offline

const char HASS_DISCOVER_BUTTON_SWITCH_TOGGLE[] PROGMEM =
  "%s,\"off_delay\":1";                            // Hass has no support for TOGGLE, fake it by resetting to OFF after 1s

const char HASS_DISCOVER_BUTTON_SWITCH_ONOFF[] PROGMEM =
  "%s,\"force_update\":true,"                      // In ON/OFF case, enable force_update to make automations work
  "\"payload_off\":\"%s\"";                        // OFF

const char HASS_DISCOVER_LIGHT_DIMMER[] PROGMEM =
  "%s,\"brightness_command_topic\":\"%s\","        // cmnd/led2/Dimmer
  "\"brightness_state_topic\":\"%s\","             // stat/led2/RESULT
  "\"brightness_scale\":100,"                      // 100%
  "\"on_command_type\":\"brightness\","            // power on (first), power on (last), no power on (brightness)
  "\"brightness_value_template\":\"{{value_json." D_CMND_DIMMER "}}\"";

const char HASS_DISCOVER_LIGHT_COLOR[] PROGMEM =
  "%s,\"rgb_command_topic\":\"%s2\","              // cmnd/led2/Color2
  "\"rgb_state_topic\":\"%s\","                    // stat/led2/RESULT
  "\"rgb_value_template\":\"{{value_json." D_CMND_COLOR ".split(',')[0:3]|join(',')}}\"";

const char HASS_DISCOVER_LIGHT_WHITE[] PROGMEM =
  "%s,\"white_value_command_topic\":\"%s\","       // cmnd/led2/White
  "\"white_value_state_topic\":\"%s\","            // stat/led2/RESULT
  "\"white_value_scale\":100,"
  "\"white_value_template\":\"{{ value_json.Channel[3] }}\"";

const char HASS_DISCOVER_LIGHT_CT[] PROGMEM =
  "%s,\"color_temp_command_topic\":\"%s\","        // cmnd/led2/CT
  "\"color_temp_state_topic\":\"%s\","             // stat/led2/RESULT
  "\"color_temp_value_template\":\"{{value_json." D_CMND_COLORTEMPERATURE "}}\"";

const char HASS_DISCOVER_SENSOR[] PROGMEM =
  "{\"name\":\"%s\","                                        // dualr2 1 BTN
  "\"state_topic\":\"%s\","                                  // cmnd/dualr2/POWER  (implies "\"optimistic\":\"false\",")
  "\"availability_topic\":\"%s\","                           // tele/dualr2/LWT
  "\"payload_available\":\"" D_ONLINE "\","                  // Online
  "\"payload_not_available\":\"" D_OFFLINE "\"";             // Offline

const char HASS_DISCOVER_SENSOR_TEMP[] PROGMEM =
  "%s,\"unit_of_measurement\":\"°%c\","                      // °C / °F
  "\"value_template\":\"{{value_json['%s'].Temperature}}\""; // "SI7021-14":{"Temperature":null,"Humidity":null} -> {{ value_json['SI7021-14'].Temperature }}

const char HASS_DISCOVER_SENSOR_HUM[] PROGMEM =
  "%s,\"unit_of_measurement\":\"%%\","                       // %
  "\"value_template\":\"{{value_json['%s'].Humidity}}\","    // "SI7021-14":{"Temperature":null,"Humidity":null} -> {{ value_json['SI7021-14'].Humidity }}
  "\"device_class\":\"humidity\"";                           // temperature / humidity

const char HASS_DISCOVER_SENSOR_ANY[] PROGMEM =
  "%s,\"value_template\":\"{{value_json['%s'].%s}}\"";       // "COUNTER":{"C1":0} -> {{ value_json['COUNTER'].C1 }}

const char HASS_DISCOVER_RELAY_SHORT[] PROGMEM =
  "{\"name\":\"%s\","                              // dualr2 1
  "\"cmd_t\":\"%s\","                              // cmnd/dualr2/POWER2
  "\"stat_t\":\"%s\","                             // stat/dualr2/RESULT  (implies "\"optimistic\":\"false\",")
  "\"val_tpl\":\"{{value_json.%s}}\","             // POWER2
  "\"pl_off\":\"%s\","                             // OFF
  "\"pl_on\":\"%s\","                              // ON
//  "\"optimistic\":\"false\","                    // false is Hass default when state_topic is set
  "\"avty_t\":\"%s\","                             // tele/dualr2/LWT
  "\"pl_avail\":\"" D_ONLINE "\","                 // Online
  "\"pl_not_avail\":\"" D_OFFLINE "\"";            // Offline

const char HASS_DISCOVER_BUTTON_SWITCH_SHORT[] PROGMEM =
  "{\"name\":\"%s\","                              // dualr2 1 BTN
  "\"stat_t\":\"%s\","                             // cmnd/dualr2/POWER  (implies "\"optimistic\":\"false\",")
//  "\"value_template\":\"{{value_json.%s}}\","      // POWER2
  "\"pl_on\":\"%s\","                              // TOGGLE
//  "\"optimistic\":\"false\","                    // false is Hass default when state_topic is set
  "\"avty_t\":\"%s\","                             // tele/dualr2/LWT
  "\"pl_avail\":\"" D_ONLINE "\","                 // Online
  "\"pl_not_avail\":\"" D_OFFLINE "\"";            // Offline

const char HASS_DISCOVER_BUTTON_SWITCH_TOGGLE_SHORT[] PROGMEM =
  "%s,\"off_delay\":1";                            // Hass has no support for TOGGLE, fake it by resetting to OFF after 1s

const char HASS_DISCOVER_BUTTON_SWITCH_ONOFF_SHORT[] PROGMEM =
  "%s,\"frc_upd\":true,"                           // In ON/OFF case, enable force_update to make automations work
  "\"pl_off\":\"%s\"";                             // OFF


const char HASS_DISCOVER_LIGHT_DIMMER_SHORT[] PROGMEM =
  "%s,\"bri_cmd_t\":\"%s\","                       // cmnd/led2/Dimmer
  "\"bri_stat_t\":\"%s\","                         // stat/led2/RESULT
  "\"bri_scl\":100,"                               // 100%
  "\"on_cmd_type\":\"brightness\","                // power on (first), power on (last), no power on (brightness)
  "\"bri_val_tpl\":\"{{value_json." D_CMND_DIMMER "}}\"";

const char HASS_DISCOVER_LIGHT_COLOR_SHORT[] PROGMEM =
  "%s,\"rgb_cmd_t\":\"%s2\","                      // cmnd/led2/Color2
  "\"rgb_stat_t\":\"%s\","                         // stat/led2/RESULT
  "\"rgb_val_tpl\":\"{{value_json." D_CMND_COLOR ".split(',')[0:3]|join(',')}}\"";

const char HASS_DISCOVER_LIGHT_WHITE_SHORT[] PROGMEM =
  "%s,\"whit_val_cmd_t\":\"%s\","                  // cmnd/led2/White
  "\"whit_val_stat_t\":\"%s\","                    // stat/led2/RESULT
  "\"white_value_scale\":100,"                     // (No abbreviation defined)
  "\"whit_val_tpl\":\"{{ value_json.Channel[3] }}\"";

const char HASS_DISCOVER_LIGHT_CT_SHORT[] PROGMEM =
  "%s,\"clr_temp_cmd_t\":\"%s\","                  // cmnd/led2/CT
  "\"clr_temp_stat_t\":\"%s\","                    // stat/led2/RESULT
  "\"clr_temp_val_tpl\":\"{{value_json." D_CMND_COLORTEMPERATURE "}}\"";

const char HASS_DISCOVER_LIGHT_SCHEME_SHORT[] PROGMEM =
  "%s,\"fx_cmd_t\":\"%s\","                        // cmnd/led2/Scheme
  "\"fx_stat_t\":\"%s\","                          // stat/led2/RESULT
  "\"fx_val_tpl\":\"{{value_json." D_CMND_SCHEME "}}\","
  "\"fx_list\":[\"0\",\"1\",\"2\",\"3\",\"4\"]";   // string list with reference to scheme parameter.

const char HASS_DISCOVER_SENSOR_SHORT[] PROGMEM =
  "{\"name\":\"%s\","                                 // dualr2 1 BTN
  "\"stat_t\":\"%s\","                                // cmnd/dualr2/POWER  (implies "\"optimistic\":\"false\",")
  "\"avty_t\":\"%s\","                                // tele/dualr2/LWT
  "\"pl_avail\":\"" D_ONLINE "\","                    // Online
  "\"pl_not_avail\":\"" D_OFFLINE "\"";               // Offline

const char HASS_DISCOVER_SENSOR_TEMP_SHORT[] PROGMEM =
  "%s,\"unit_of_meas\":\"°%c\","                      // °C / °F
  "\"val_tpl\":\"{{value_json['%s'].Temperature}}\""; // "SI7021-14":{"Temperature":null,"Humidity":null} -> {{ value_json['SI7021-14'].Temperature }}

const char HASS_DISCOVER_SENSOR_HUM_SHORT[] PROGMEM =
  "%s,\"unit_of_meas\":\"%%\","                       // %
  "\"val_tpl\":\"{{value_json['%s'].Humidity}}\","    // "SI7021-14":{"Temperature":null,"Humidity":null} -> {{ value_json['SI7021-14'].Humidity }}
  "\"dev_cla\":\"humidity\"";                         // humidity

const char HASS_DISCOVER_SENSOR_ANY_SHORT[] PROGMEM =
  "%s,\"val_tpl\":\"{{value_json['%s'].%s}}\"";       // "COUNTER":{"C1":0} -> {{ value_json['COUNTER'].C1 }}

const char HASS_DISCOVER_DEVICE_INFO_SHORT[] PROGMEM =
  "%s,\"uniq_id\":\"%s\","
  "\"device\":{\"identifiers\":[\"%06X\"],"
  "\"name\":\"%s\","
  "\"model\":\"%s\","
  "\"sw_version\":\"%s%s\","
  "\"manufacturer\":\"%s\"}";

const char HASS_DISCOVER_TOPIC_PREFIX[] PROGMEM =
  "%s, \"~\":\"%s\"";

static void FindPrefix(char* s1, char* s2, char* out)
{
  int prefixlen = 0;

  while (s1[prefixlen] != '\0' && s2[prefixlen] != '\0' && s1[prefixlen] == s2[prefixlen]) {
    prefixlen++;
  }
  strlcpy(out, s1, prefixlen+1);
}

static void Shorten(char** s, char *prefix)
{
  size_t len = strlen(*s);
  size_t prefixlen = strlen(prefix);
  if (len > prefixlen && !strncmp(*s, prefix, prefixlen)) {
    *s += prefixlen-1;
    *s[0] = '~';
  }
}

void HAssAnnounceRelayLight(void)
{
  char stopic[TOPSZ];
  char stemp1[TOPSZ];
  char stemp2[TOPSZ];
  char stemp3[TOPSZ];
  char unique_id[30];
  bool is_light = false;
  bool is_topic_light = false;

  for (int i = 1; i <= MAX_RELAYS; i++) {
    is_light = ((i == devices_present) && (light_type));
    is_topic_light = Settings.flag.hass_light || is_light;

    mqtt_data[0] = '\0';  // Clear retained message

    // Clear "other" topic first in case the device has been reconfigured from ligth to switch or vice versa
    snprintf_P(unique_id, sizeof(unique_id), PSTR("%06X_%s_%d"), ESP.getChipId(), (is_topic_light) ? "RL" : "LI", i);
    snprintf_P(stopic, sizeof(stopic), PSTR(HOME_ASSISTANT_DISCOVERY_PREFIX "/%s/%s/config"), (is_topic_light) ? "switch" : "light", unique_id);
    MqttPublish(stopic, true);
    // Clear or Set topic
    snprintf_P(unique_id, sizeof(unique_id), PSTR("%06X_%s_%d"), ESP.getChipId(), (is_topic_light) ? "LI" : "RL", i);
    snprintf_P(stopic, sizeof(stopic), PSTR(HOME_ASSISTANT_DISCOVERY_PREFIX "/%s/%s/config"), (is_topic_light) ? "light" : "switch", unique_id);

    if (Settings.flag.hass_discovery && (i <= devices_present)) {
      char name[33];
      char value_template[33];
      char prefix[TOPSZ];
      char *command_topic = stemp1;
      char *state_topic = stemp2;
      char *availability_topic = stemp3;

      if (i > MAX_FRIENDLYNAMES) {
        snprintf_P(name, sizeof(name), PSTR("%s %d"), Settings.friendlyname[0], i);
      } else {
        snprintf_P(name, sizeof(name), Settings.friendlyname[i -1]);
      }
      GetPowerDevice(value_template, i, sizeof(value_template), Settings.flag.device_index_enable);
      GetTopic_P(command_topic, CMND, mqtt_topic, value_template);
      //GetTopic_P(state_topic, STAT, mqtt_topic, S_RSLT_RESULT);
      GetTopic_P(state_topic, TELE, mqtt_topic, D_RSLT_STATE);
      GetTopic_P(availability_topic, TELE, mqtt_topic, S_LWT);
      FindPrefix(command_topic, state_topic, prefix);
      if (Settings.flag3.hass_short_discovery_msg) {
        Shorten(&command_topic, prefix);
        Shorten(&state_topic, prefix);
        Shorten(&availability_topic, prefix);
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), Settings.flag3.hass_short_discovery_msg?HASS_DISCOVER_RELAY_SHORT:HASS_DISCOVER_RELAY,
                 name, command_topic, state_topic, value_template, Settings.state_text[0], Settings.state_text[1], availability_topic);

      if (is_light) {
        char *brightness_command_topic = stemp1;

        GetTopic_P(brightness_command_topic, CMND, mqtt_topic, D_CMND_DIMMER);
        if (Settings.flag3.hass_short_discovery_msg)
          Shorten(&brightness_command_topic, prefix);
        snprintf_P(mqtt_data, sizeof(mqtt_data), Settings.flag3.hass_short_discovery_msg?HASS_DISCOVER_LIGHT_DIMMER_SHORT:HASS_DISCOVER_LIGHT_DIMMER,
                   mqtt_data, brightness_command_topic, state_topic);

        if (light_subtype >= LST_RGB) {
          char *rgb_command_topic = stemp1;

          GetTopic_P(rgb_command_topic, CMND, mqtt_topic, D_CMND_COLOR);
          if (Settings.flag3.hass_short_discovery_msg)
            Shorten(&rgb_command_topic, prefix);
          snprintf_P(mqtt_data, sizeof(mqtt_data), Settings.flag3.hass_short_discovery_msg?HASS_DISCOVER_LIGHT_COLOR_SHORT:HASS_DISCOVER_LIGHT_COLOR,
                     mqtt_data, rgb_command_topic, state_topic);

          char *effect_command_topic = stemp1;
          GetTopic_P(effect_command_topic, CMND, mqtt_topic, D_CMND_SCHEME);
          if (Settings.flag3.hass_short_discovery_msg) {
            Shorten(&effect_command_topic, prefix);
            snprintf_P(mqtt_data, sizeof(mqtt_data), HASS_DISCOVER_LIGHT_SCHEME_SHORT, mqtt_data, effect_command_topic, state_topic);
          }

        }
        if (LST_RGBW == light_subtype) {
          char *white_temp_command_topic = stemp1;

          GetTopic_P(white_temp_command_topic, CMND, mqtt_topic, D_CMND_WHITE);
          if (Settings.flag3.hass_short_discovery_msg)
            Shorten(&white_temp_command_topic, prefix);
          snprintf_P(mqtt_data, sizeof(mqtt_data), Settings.flag3.hass_short_discovery_msg?HASS_DISCOVER_LIGHT_WHITE_SHORT:HASS_DISCOVER_LIGHT_WHITE,
                     mqtt_data, white_temp_command_topic, state_topic);
        }
        if ((LST_COLDWARM == light_subtype) || (LST_RGBWC == light_subtype)) {
          char *color_temp_command_topic = stemp1;

          GetTopic_P(color_temp_command_topic, CMND, mqtt_topic, D_CMND_COLORTEMPERATURE);
          if (Settings.flag3.hass_short_discovery_msg)
            Shorten(&color_temp_command_topic, prefix);
          snprintf_P(mqtt_data, sizeof(mqtt_data), Settings.flag3.hass_short_discovery_msg?HASS_DISCOVER_LIGHT_CT_SHORT:HASS_DISCOVER_LIGHT_CT,
                     mqtt_data, color_temp_command_topic, state_topic);
        }
      }
      if (Settings.flag3.hass_short_discovery_msg) {
        snprintf_P(stemp1, sizeof(stemp1), kModules[Settings.module].name);
        snprintf_P(mqtt_data, sizeof(mqtt_data), HASS_DISCOVER_DEVICE_INFO_SHORT, mqtt_data,
                   unique_id, ESP.getChipId(),
                   Settings.friendlyname[0], stemp1, my_version, my_image, "Tasmota");
        snprintf_P(mqtt_data, sizeof(mqtt_data), HASS_DISCOVER_TOPIC_PREFIX, mqtt_data, prefix);
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
    }
    MqttPublish(stopic, true);
  }
}

void HAssAnnounceButtonSwitch(byte device, char* topic, byte present, byte key, byte toggle)
{
// key 0 = button
// key 1 = switch
  char stopic[TOPSZ];
  char stemp1[TOPSZ];
  char stemp2[TOPSZ];
  char unique_id[30];

  mqtt_data[0] = '\0';  // Clear retained message

  // Clear or Set topic
  snprintf_P(unique_id, sizeof(unique_id), PSTR("%06X_%s_%d"), ESP.getChipId(), key?"SW":"BTN", device+1);
  snprintf_P(stopic, sizeof(stopic), PSTR(HOME_ASSISTANT_DISCOVERY_PREFIX "/binary_sensor/%s/config"), unique_id);

  if (Settings.flag.hass_discovery && present) {
    char name[33];
    char value_template[33];
    char prefix[TOPSZ];
    char *state_topic = stemp1;
    char *availability_topic = stemp2;

    if (device+1 > MAX_FRIENDLYNAMES) {
      snprintf_P(name, sizeof(name), PSTR("%s %s %d"), Settings.friendlyname[0], key?"SW":"BTN", device+1);
    } else {
      snprintf_P(name, sizeof(name), PSTR("%s %s"), Settings.friendlyname[device], key?"SW":"BTN");
    }
    GetPowerDevice(value_template, device+1, sizeof(value_template),
                   key + Settings.flag.device_index_enable); // Force index for Switch 1, Index on Button1 is controlled by Settings.flag.device_index_enable
    GetTopic_P(state_topic, CMND, topic, value_template); // State of button is sent as CMND TOGGLE, state of switch is sent as ON/OFF
    GetTopic_P(availability_topic, TELE, mqtt_topic, S_LWT);
    FindPrefix(state_topic, availability_topic, prefix);
    if (Settings.flag3.hass_short_discovery_msg) {
      Shorten(&state_topic, prefix);
      Shorten(&availability_topic, prefix);
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), Settings.flag3.hass_short_discovery_msg?HASS_DISCOVER_BUTTON_SWITCH_SHORT:HASS_DISCOVER_BUTTON_SWITCH,
               name, state_topic, Settings.state_text[toggle?2:1], availability_topic);
    if (toggle) snprintf_P(mqtt_data, sizeof(mqtt_data),
               Settings.flag3.hass_short_discovery_msg?HASS_DISCOVER_BUTTON_SWITCH_TOGGLE_SHORT:HASS_DISCOVER_BUTTON_SWITCH_TOGGLE,
               mqtt_data);
    if (!toggle) snprintf_P(mqtt_data, sizeof(mqtt_data),
                            Settings.flag3.hass_short_discovery_msg?HASS_DISCOVER_BUTTON_SWITCH_ONOFF_SHORT:HASS_DISCOVER_BUTTON_SWITCH_ONOFF,
                            mqtt_data, Settings.state_text[0]);

    if (Settings.flag3.hass_short_discovery_msg) {
      snprintf_P(stemp1, sizeof(stemp1), kModules[Settings.module].name);
      snprintf_P(mqtt_data, sizeof(mqtt_data), HASS_DISCOVER_DEVICE_INFO_SHORT, mqtt_data,
                   unique_id, ESP.getChipId(),
                   Settings.friendlyname[0], stemp1, my_version, my_image, "Tasmota");
      snprintf_P(mqtt_data, sizeof(mqtt_data), HASS_DISCOVER_TOPIC_PREFIX, mqtt_data, prefix);
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
  }
  MqttPublish(stopic, true);
}

void HAssAnnounceSwitches(void)
{
  char sw_topic[sizeof(Settings.switch_topic)];

  // Send info about buttons
  char *tmp = Settings.switch_topic;
  Format(sw_topic, tmp, sizeof(sw_topic));
  if ((strlen(sw_topic) != 0) && strcmp(sw_topic, "0")) {
    for (byte switch_index = 0; switch_index < MAX_SWITCHES; switch_index++) {
      byte switch_present = 0;
      byte toggle = 1;

      if ((pin[GPIO_SWT1 + switch_index] < 99) || (pin[GPIO_SWT1_NP + switch_index] < 99)) {
        switch_present = 1;
      }

      // Check if MQTT message will be ON/OFF or TOGGLE
      if (Settings.switchmode[switch_index] == FOLLOW || Settings.switchmode[switch_index] == FOLLOW_INV ||
          Settings.flag3.button_switch_force_local ||
          !strcmp(mqtt_topic, sw_topic) || !strcmp(Settings.mqtt_grptopic, sw_topic))
      {
        toggle = 0; // MQTT message will be ON/OFF
      }

      HAssAnnounceButtonSwitch(switch_index, sw_topic, switch_present, 1, toggle);
    }
  }
}

void HAssAnnounceButtons(void)
{
  char key_topic[sizeof(Settings.button_topic)];

  // Send info about buttons
  char *tmp = Settings.button_topic;
  Format(key_topic, tmp, sizeof(key_topic));
  if ((strlen(key_topic) != 0) && strcmp(key_topic, "0")) {
    for (byte button_index = 0; button_index < MAX_KEYS; button_index++) {
      byte button_present = 0;
      byte toggle = 1;

      if (!button_index && ((SONOFF_DUAL == Settings.module) || (CH4 == Settings.module))) {
        button_present = 1;
      } else {
        if ((pin[GPIO_KEY1 + button_index] < 99) || (pin[GPIO_KEY1_NP + button_index] < 99)) {
          button_present = 1;
        }
      }

      // Check if MQTT message will be ON/OFF or TOGGLE
      if (Settings.flag3.button_switch_force_local ||
          !strcmp(mqtt_topic, key_topic) || !strcmp(Settings.mqtt_grptopic, key_topic))
      {
        toggle = 0; // MQTT message will be ON/OFF
      }

      HAssAnnounceButtonSwitch(button_index, key_topic, button_present, 0, toggle);
    }
  }
}

void HAssAnnounceSensor(const char* sensorname, const char* subsensortype)
{
  char stopic[TOPSZ];
  char stemp1[TOPSZ];
  char stemp2[TOPSZ];
  char unique_id[30];

  // Announce sensor, special handling of temperature and humidity sensors
  mqtt_data[0] = '\0';  // Clear retained message

  // Clear or Set topic
  snprintf_P(unique_id, sizeof(unique_id), PSTR("%06X_%s_%s"), ESP.getChipId(), sensorname, subsensortype);
  snprintf_P(stopic, sizeof(stopic), PSTR(HOME_ASSISTANT_DISCOVERY_PREFIX "/sensor/%s/config"), unique_id);

  if (Settings.flag.hass_discovery) {
    char name[33];
    char prefix[TOPSZ];
    char *state_topic = stemp1;
    char *availability_topic = stemp2;

    snprintf_P(name, sizeof(name), PSTR("%s %s %s"), Settings.friendlyname[0], sensorname, subsensortype);
    GetTopic_P(state_topic, TELE, mqtt_topic, PSTR(D_RSLT_SENSOR));
    GetTopic_P(availability_topic, TELE, mqtt_topic, S_LWT);
    FindPrefix(state_topic, availability_topic, prefix);
    if (Settings.flag3.hass_short_discovery_msg) {
      Shorten(&state_topic, prefix);
      Shorten(&availability_topic, prefix);
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), Settings.flag3.hass_short_discovery_msg?HASS_DISCOVER_SENSOR_SHORT:HASS_DISCOVER_SENSOR,
               name, state_topic, availability_topic);
    if (!strcmp_P(subsensortype, PSTR(D_JSON_TEMPERATURE))) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), Settings.flag3.hass_short_discovery_msg?HASS_DISCOVER_SENSOR_TEMP_SHORT:HASS_DISCOVER_SENSOR_TEMP,
                 mqtt_data, TempUnit(), sensorname);
    } else if (!strcmp_P(subsensortype, PSTR(D_JSON_HUMIDITY))) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), Settings.flag3.hass_short_discovery_msg?HASS_DISCOVER_SENSOR_HUM_SHORT:HASS_DISCOVER_SENSOR_HUM,
                 mqtt_data, sensorname);
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), Settings.flag3.hass_short_discovery_msg?HASS_DISCOVER_SENSOR_ANY_SHORT:HASS_DISCOVER_SENSOR_ANY,
                 mqtt_data, sensorname, subsensortype);
    }
    if (Settings.flag3.hass_short_discovery_msg) {
      snprintf_P(stemp1, sizeof(stemp1), kModules[Settings.module].name);
      snprintf_P(mqtt_data, sizeof(mqtt_data), HASS_DISCOVER_DEVICE_INFO_SHORT, mqtt_data,
                 unique_id, ESP.getChipId(),
                 Settings.friendlyname[0], stemp1, my_version, my_image, "Tasmota");
      snprintf_P(mqtt_data, sizeof(mqtt_data), HASS_DISCOVER_TOPIC_PREFIX, mqtt_data, prefix);
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
  }
  MqttPublish(stopic, true);
}

void HAssAnnounceSensors(void)
{
  uint8_t hass_xsns_index = 0;
  do {
    mqtt_data[0] = '\0';
    int tele_period_save = tele_period;
    tele_period = 2;                                  // Do not allow HA updates during next function call
    XsnsNextCall(FUNC_JSON_APPEND, hass_xsns_index);  // ,"INA219":{"Voltage":4.494,"Current":0.020,"Power":0.089}
    tele_period = tele_period_save;

    char sensordata[256]; // Copy because we need to write to mqtt_data
    strlcpy(sensordata, mqtt_data, sizeof(sensordata));

    if (strlen(sensordata)) {
      sensordata[0] = '{';                             // {"INA219":{"Voltage":4.494,"Current":0.020,"Power":0.089}
      snprintf_P(sensordata, sizeof(sensordata), PSTR("%s}"), sensordata);

      StaticJsonBuffer<256> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(sensordata);
      if (!root.success()) {
        snprintf_P(log_data, sizeof(log_data), PSTR("HASS: failed to parse '%s'"), sensordata);
        AddLog(LOG_LEVEL_ERROR);
        continue;
      }
      for (auto sensor : root) {
        const char* sensorname = sensor.key;
        JsonObject& sensors = sensor.value.as<JsonObject>();
        if (!sensors.success()) {
          snprintf_P(log_data, sizeof(log_data), PSTR("HASS: failed to parse '%s'"), sensordata);
          AddLog(LOG_LEVEL_ERROR);
          continue;
        }
        for (auto subsensor : sensors) {
          HAssAnnounceSensor(sensorname, subsensor.key);
        }
      }
    }
  } while (hass_xsns_index != 0);
}

static int string_ends_with(const char * str, const char * suffix)
{
  int str_len = strlen(str);
  int suffix_len = strlen(suffix);

  return (str_len >= suffix_len) && (0 == strcmp(str + (str_len-suffix_len), suffix));
}

void HAssDiscovery(uint8_t mode)
{
  // Configure Tasmota for default Home Assistant parameters to keep discovery message as short as possible
  if (Settings.flag.hass_discovery) {
    Settings.flag.mqtt_response = 0;        // Response always as RESULT and not as uppercase command
    Settings.flag.decimal_text = 1;         // Respond with decimal color values
    Settings.flag3.hass_tele_on_power = 1; // send tele/STATE message as stat/RESULT
//    Settings.light_scheme = 0;           // To just control color it needs to be Scheme 0
    if (!string_ends_with(Settings.mqtt_fulltopic, "%prefix%/")) {
      strncpy_P(Settings.mqtt_fulltopic, PSTR("%topic%/%prefix%/"), sizeof(Settings.mqtt_fulltopic));
      restart_flag = 2;
    }
  }

  if (Settings.flag.hass_discovery || (1 == mode)) {
    // Send info about relays and lights
    HAssAnnounceRelayLight();

    // Send info about buttons
    HAssAnnounceButtons();

    // Send info about switches
    HAssAnnounceSwitches();

    // Send info about sensors
    HAssAnnounceSensors();
  }
}

/*
#define D_CMND_HASSDISCOVER "HassDiscover"

enum HassCommands { CMND_HASSDISCOVER };
const char kHassCommands[] PROGMEM = D_CMND_HASSDISCOVER ;

boolean HassCommand(void)
{
  char command[CMDSZ];
  boolean serviced = true;

  int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, kHassCommands);
  if (-1 == command_code) {
    serviced = false;  // Unknown command
  }
  else if (CMND_HASSDISCOVER == command_code) {
    if (XdrvMailbox.data_len > 0) {
      switch (XdrvMailbox.payload) {
      case 0: // Off
      case 1: // On
        Settings.flag.hass_discovery = XdrvMailbox.payload;
        break;
      case 2: // Toggle
        Settings.flag.hass_discovery ^= 1;
        break;
      case 4: // Off
      case 5: // On
        Settings.flag.hass_light = XdrvMailbox.payload &1;
        break;
      case 6: // Toggle
        Settings.flag.hass_light ^= 1;
        break;
      }
      HAssDiscovery(1);
    }
    snprintf_P (mqtt_data, sizeof(mqtt_data), PSTR("{\"%s\":\"%s\",\"Force light\":\"%s\"}"),
      command, GetStateText(Settings.flag.hass_discovery), GetStateText(Settings.flag.hass_light));
  }
  else serviced = false;  // Unknown command

  return serviced;
}
*/

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xdrv12(byte function)
{
  boolean result = false;

  if (Settings.flag.mqtt_enabled) {
    switch (function) {
      case FUNC_MQTT_INIT:
        HAssDiscovery(0);
        break;
/*
      case FUNC_COMMAND:
        result = HassCommand();
        break;
*/
    }
  }
  return result;
}

#endif  // USE_HOME_ASSISTANT
