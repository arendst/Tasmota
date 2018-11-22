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

const char HASS_DISCOVER_BUTTON[] PROGMEM =
  "{\"name\":\"%s\","                              // dualr2 1 BTN
  "\"state_topic\":\"%s\","                        // cmnd/dualr2/POWER  (implies "\"optimistic\":\"false\",")
//  "\"value_template\":\"{{value_json.%s}}\","      // POWER2
  "\"payload_on\":\"%s\","                         // TOGGLE
//  "\"optimistic\":\"false\","                    // false is Hass default when state_topic is set
  "\"availability_topic\":\"%s\","                 // tele/dualr2/LWT
  "\"payload_available\":\"" D_ONLINE "\","        // Online
  "\"payload_not_available\":\"" D_OFFLINE "\","   // Offline
  "\"force_update\":true";

const char HASS_DISCOVER_LIGHT_DIMMER[] PROGMEM =
  "%s,\"brightness_command_topic\":\"%s\","        // cmnd/led2/Dimmer
  "\"brightness_state_topic\":\"%s\","             // stat/led2/RESULT
  "\"brightness_scale\":100,"                      // 100%
  "\"on_command_type\":\"brightness\","            // power on (first), power on (last), no power on (brightness)
  "\"brightness_value_template\":\"{{value_json." D_CMND_DIMMER "}}\"";

const char HASS_DISCOVER_LIGHT_COLOR[] PROGMEM =
  "%s,\"rgb_command_topic\":\"%s2\","              // cmnd/led2/Color2
  "\"rgb_state_topic\":\"%s\","                    // stat/led2/RESULT
  "\"rgb_value_template\":\"{{value_json." D_CMND_COLOR "}}\"";
//  "\"rgb_value_template\":\"{{value_json." D_CMND_COLOR " | join(',')}}\"";

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

const char HASS_DISCOVER_BUTTON_SHORT[] PROGMEM =
  "{\"name\":\"%s\","                              // dualr2 1 BTN
  "\"stat_t\":\"%s\","                             // cmnd/dualr2/POWER  (implies "\"optimistic\":\"false\",")
//  "\"value_template\":\"{{value_json.%s}}\","      // POWER2
  "\"pl_on\":\"%s\","                              // TOGGLE
//  "\"optimistic\":\"false\","                    // false is Hass default when state_topic is set
  "\"avty_t\":\"%s\","                             // tele/dualr2/LWT
  "\"pl_avail\":\"" D_ONLINE "\","                 // Online
  "\"pl_not_avail\":\"" D_OFFLINE "\","            // Offline
  "\"frc_upd\":true";

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
//  "\"rgb_val_tpl\":\"{{value_json." D_CMND_COLOR " | join(',')}}\"";

const char HASS_DISCOVER_LIGHT_CT_SHORT[] PROGMEM =
  "%s,\"clr_temp_cmd_t\":\"%s\","                  // cmnd/led2/CT
  "\"clr_temp_stat_t\":\"%s\","                    // stat/led2/RESULT
  "\"clr_temp_val_tpl\":\"{{value_json." D_CMND_COLORTEMPERATURE "}}\"";
/*
const char HASS_DISCOVER_LIGHT_SCHEME[] PROGMEM =
  "%s,\"effect_command_topic\":\"%s\","            // cmnd/led2/Scheme
  "\"effect_state_topic\":\"%s\","                 // stat/led2/RESULT
  "\"effect_value_template\":\"{{value_json." D_CMND_SCHEME "}}\","
  "\"effect_list\":[\"0\",\"1\",\"2\",\"3\",\"4\"]";  // string list with reference to scheme parameter. Currently only supports numbers 0 to 11 as it make the mqtt string too long
*/

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
  char sidx[8];
  char stopic[TOPSZ];
  bool is_light = false;
  bool is_topic_light = false;

  for (int i = 1; i <= MAX_RELAYS; i++) {
    is_light = ((i == devices_present) && (light_type));
    is_topic_light = Settings.flag.hass_light || is_light;

    mqtt_data[0] = '\0';  // Clear retained message

    snprintf_P(sidx, sizeof(sidx), PSTR("_%d"), i);
    // Clear "other" topic first in case the device has been reconfigured
    snprintf_P(stopic, sizeof(stopic), PSTR(HOME_ASSISTANT_DISCOVERY_PREFIX "/%s/%s%s/config"), (is_topic_light) ? "switch" : "light", mqtt_topic, sidx);
    MqttPublish(stopic, true);
    // Clear or Set topic
    snprintf_P(stopic, sizeof(stopic), PSTR(HOME_ASSISTANT_DISCOVERY_PREFIX "/%s/%s%s/config"), (is_topic_light) ? "light" : "switch", mqtt_topic, sidx);

    if (Settings.flag.hass_discovery && (i <= devices_present)) {
      char name[33];
      char value_template[33];
      char _command_topic[TOPSZ];
      char _state_topic[TOPSZ];
      char _availability_topic[TOPSZ];
      char prefix[TOPSZ];
      char *command_topic = _command_topic;
      char *state_topic = _state_topic;
      char *availability_topic = _availability_topic;

      if (i > MAX_FRIENDLYNAMES) {
        snprintf_P(name, sizeof(name), PSTR("%s %d"), Settings.friendlyname[0], i);
      } else {
        snprintf_P(name, sizeof(name), Settings.friendlyname[i -1]);
      }
      GetPowerDevice(value_template, i, sizeof(value_template), Settings.flag.device_index_enable);
      GetTopic_P(command_topic, CMND, mqtt_topic, value_template);
      GetTopic_P(state_topic, STAT, mqtt_topic, S_RSLT_RESULT);
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
        char _brightness_command_topic[TOPSZ];
        char *brightness_command_topic = _brightness_command_topic;

        GetTopic_P(brightness_command_topic, CMND, mqtt_topic, D_CMND_DIMMER);
        if (Settings.flag3.hass_short_discovery_msg)
          Shorten(&brightness_command_topic, prefix);
        snprintf_P(mqtt_data, sizeof(mqtt_data), Settings.flag3.hass_short_discovery_msg?HASS_DISCOVER_LIGHT_DIMMER_SHORT:HASS_DISCOVER_LIGHT_DIMMER,
                   mqtt_data, brightness_command_topic, state_topic);

        if (light_subtype >= LST_RGB) {
          char _rgb_command_topic[TOPSZ];
          char *rgb_command_topic = _rgb_command_topic;

          GetTopic_P(rgb_command_topic, CMND, mqtt_topic, D_CMND_COLOR);
          if (Settings.flag3.hass_short_discovery_msg)
            Shorten(&rgb_command_topic, prefix);
          snprintf_P(mqtt_data, sizeof(mqtt_data), Settings.flag3.hass_short_discovery_msg?HASS_DISCOVER_LIGHT_COLOR_SHORT:HASS_DISCOVER_LIGHT_COLOR,
                     mqtt_data, rgb_command_topic, state_topic);
/*
          char effect_command_topic[TOPSZ];

          GetTopic_P(effect_command_topic, CMND, mqtt_topic, D_CMND_SCHEME);
          snprintf_P(mqtt_data, sizeof(mqtt_data), HASS_DISCOVER_LIGHT_SCHEME, mqtt_data, effect_command_topic, state_topic);
*/
        }
        if ((LST_COLDWARM == light_subtype) || (LST_RGBWC == light_subtype)) {
          char _color_temp_command_topic[TOPSZ];
          char *color_temp_command_topic = _color_temp_command_topic;

          GetTopic_P(color_temp_command_topic, CMND, mqtt_topic, D_CMND_COLORTEMPERATURE);
          if (Settings.flag3.hass_short_discovery_msg)
            Shorten(&color_temp_command_topic, prefix);
          snprintf_P(mqtt_data, sizeof(mqtt_data), Settings.flag3.hass_short_discovery_msg?HASS_DISCOVER_LIGHT_CT_SHORT:HASS_DISCOVER_LIGHT_CT,
                     mqtt_data, color_temp_command_topic, state_topic);
        }
      }
      if (Settings.flag3.hass_short_discovery_msg)
        snprintf_P(mqtt_data, sizeof(mqtt_data), HASS_DISCOVER_TOPIC_PREFIX, mqtt_data, prefix);
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
    }
    MqttPublish(stopic, true);
  }
}

void HAssAnnounceButton(void)
{
  char sidx[8];
  char stopic[TOPSZ];
  char key_topic[sizeof(Settings.button_topic)];

  // Send info about buttons
  char *tmp = Settings.button_topic;
  Format(key_topic, tmp, sizeof(key_topic));
  if ((strlen(key_topic) != 0) && strcmp(key_topic, "0")) {
    for (byte button_index = 0; button_index < MAX_KEYS; button_index++) {
      uint8_t button_present = 0;

      if (!button_index && ((SONOFF_DUAL == Settings.module) || (CH4 == Settings.module))) {
        button_present = 1;
      } else {
        if (pin[GPIO_KEY1 + button_index] < 99) {
          button_present = 1;
        }
      }

      mqtt_data[0] = '\0';  // Clear retained message

      // Clear or Set topic
      snprintf_P(sidx, sizeof(sidx), PSTR("_%d"), button_index+1);
      snprintf_P(stopic, sizeof(stopic), PSTR(HOME_ASSISTANT_DISCOVERY_PREFIX "/%s/%s%s/config"), "binary_sensor", key_topic, sidx);

      if (Settings.flag.hass_discovery && button_present) {
        char name[33];
        char value_template[33];
        char _state_topic[TOPSZ];
        char _availability_topic[TOPSZ];
        char prefix[TOPSZ];
        char *state_topic = _state_topic;
        char *availability_topic = _availability_topic;

        if (button_index+1 > MAX_FRIENDLYNAMES) {
          snprintf_P(name, sizeof(name), PSTR("%s %d BTN"), Settings.friendlyname[0], button_index+1);
        } else {
          snprintf_P(name, sizeof(name), PSTR("%s BTN"), Settings.friendlyname[button_index]);
        }
        GetPowerDevice(value_template, button_index+1, sizeof(value_template), Settings.flag.device_index_enable);
        GetTopic_P(state_topic, CMND, key_topic, value_template); // State of button is sent as CMND TOGGLE
        GetTopic_P(availability_topic, TELE, mqtt_topic, S_LWT);
        FindPrefix(state_topic, availability_topic, prefix);
        if (Settings.flag3.hass_short_discovery_msg) {
          Shorten(&state_topic, prefix);
          Shorten(&availability_topic, prefix);
        }
        snprintf_P(mqtt_data, sizeof(mqtt_data), Settings.flag3.hass_short_discovery_msg?HASS_DISCOVER_BUTTON_SHORT:HASS_DISCOVER_BUTTON,
                   name, state_topic, Settings.state_text[2], availability_topic);

        if (Settings.flag3.hass_short_discovery_msg)
          snprintf_P(mqtt_data, sizeof(mqtt_data), HASS_DISCOVER_TOPIC_PREFIX, mqtt_data, prefix);
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
      }
      MqttPublish(stopic, true);
    }
  }
}

void HAssAnnounceSensor(const char* sensorname, const char* subsensortype)
{
  char stopic[TOPSZ];

  // Announce sensor, special handling of temperature and humidity sensors
  mqtt_data[0] = '\0';  // Clear retained message

  // Clear or Set topic
  snprintf_P(stopic, sizeof(stopic), PSTR(HOME_ASSISTANT_DISCOVERY_PREFIX "/sensor/%s_%s_%s/config"),
             mqtt_topic, sensorname, subsensortype);

  if (Settings.flag.hass_discovery) {
    char name[33];
    char _state_topic[TOPSZ];
    char _availability_topic[TOPSZ];
    char prefix[TOPSZ];
    char *state_topic = _state_topic;
    char *availability_topic = _availability_topic;

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
    if (!strcmp(subsensortype, "Temperature")) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), Settings.flag3.hass_short_discovery_msg?HASS_DISCOVER_SENSOR_TEMP_SHORT:HASS_DISCOVER_SENSOR_TEMP,
                 mqtt_data, TempUnit(), sensorname);
    } else if (!strcmp(subsensortype, "Humidity")) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), Settings.flag3.hass_short_discovery_msg?HASS_DISCOVER_SENSOR_HUM_SHORT:HASS_DISCOVER_SENSOR_HUM,
                 mqtt_data, sensorname);
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), Settings.flag3.hass_short_discovery_msg?HASS_DISCOVER_SENSOR_ANY_SHORT:HASS_DISCOVER_SENSOR_ANY,
                 mqtt_data, sensorname, subsensortype);
    }
    if (Settings.flag3.hass_short_discovery_msg)
      snprintf_P(mqtt_data, sizeof(mqtt_data), HASS_DISCOVER_TOPIC_PREFIX, mqtt_data, prefix);
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
    Settings.flag.mqtt_response = 0;     // Response always as RESULT and not as uppercase command
    Settings.flag.decimal_text = 1;      // Respond with decimal color values
//    Settings.light_scheme = 0;           // To just control color it needs to be Scheme 0
    if (!string_ends_with(Settings.mqtt_fulltopic, "%prefix%/"))
      strncpy_P(Settings.mqtt_fulltopic, PSTR("%topic%/%prefix%/"), sizeof(Settings.mqtt_fulltopic));
  }

  if (Settings.flag.hass_discovery || (1 == mode)) {
    // Send info about relays and lights
    HAssAnnounceRelayLight();

    // Send info about buttons
    HAssAnnounceButton();

    // TODO: Send info about switches

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
