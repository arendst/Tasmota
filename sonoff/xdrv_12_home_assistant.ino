/*
  xdrv_12_home_assistant.ino - home assistant support for Sonoff-Tasmota

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

#ifdef USE_HOME_ASSISTANT

#define XDRV_12             12

const char HASS_DISCOVER_RELAY[] PROGMEM =
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

const char HASS_DISCOVER_BUTTON_SWITCH[] PROGMEM =
  "{\"name\":\"%s\","                              // dualr2 1 BTN
  "\"stat_t\":\"%s\","                             // cmnd/dualr2/POWER  (implies "\"optimistic\":\"false\",")
//  "\"value_template\":\"{{value_json.%s}}\","      // POWER2
  "\"pl_on\":\"%s\","                              // TOGGLE
//  "\"optimistic\":\"false\","                    // false is Hass default when state_topic is set
  "\"avty_t\":\"%s\","                             // tele/dualr2/LWT
  "\"pl_avail\":\"" D_ONLINE "\","                 // Online
  "\"pl_not_avail\":\"" D_OFFLINE "\"";            // Offline

const char HASS_DISCOVER_BUTTON_SWITCH_TOGGLE[] PROGMEM =
  "%s,\"off_delay\":1";                            // Hass has no support for TOGGLE, fake it by resetting to OFF after 1s

const char HASS_DISCOVER_BUTTON_SWITCH_ONOFF[] PROGMEM =
  "%s,\"frc_upd\":true,"                           // In ON/OFF case, enable force_update to make automations work
  "\"pl_off\":\"%s\"";                             // OFF


const char HASS_DISCOVER_LIGHT_DIMMER[] PROGMEM =
  "%s,\"bri_cmd_t\":\"%s\","                       // cmnd/led2/Dimmer
  "\"bri_stat_t\":\"%s\","                         // stat/led2/RESULT
  "\"bri_scl\":100,"                               // 100%
  "\"on_cmd_type\":\"brightness\","                // power on (first), power on (last), no power on (brightness)
  "\"bri_val_tpl\":\"{{value_json." D_CMND_DIMMER "}}\"";

const char HASS_DISCOVER_LIGHT_COLOR[] PROGMEM =
  "%s,\"rgb_cmd_t\":\"%s2\","                      // cmnd/led2/Color2
  "\"rgb_stat_t\":\"%s\","                         // stat/led2/RESULT
  "\"rgb_val_tpl\":\"{{value_json." D_CMND_COLOR ".split(',')[0:3]|join(',')}}\"";

const char HASS_DISCOVER_LIGHT_WHITE[] PROGMEM =
  "%s,\"whit_val_cmd_t\":\"%s\","                  // cmnd/led2/White
  "\"whit_val_stat_t\":\"%s\","                    // stat/led2/RESULT
  "\"white_value_scale\":100,"                     // (No abbreviation defined)
  "\"whit_val_tpl\":\"{{value_json.Channel[3]}}\"";

const char HASS_DISCOVER_LIGHT_CT[] PROGMEM =
  "%s,\"clr_temp_cmd_t\":\"%s\","                  // cmnd/led2/CT
  "\"clr_temp_stat_t\":\"%s\","                    // stat/led2/RESULT
  "\"clr_temp_val_tpl\":\"{{value_json." D_CMND_COLORTEMPERATURE "}}\"";

const char HASS_DISCOVER_LIGHT_SCHEME[] PROGMEM =
  "%s,\"fx_cmd_t\":\"%s\","                        // cmnd/led2/Scheme
  "\"fx_stat_t\":\"%s\","                          // stat/led2/RESULT
  "\"fx_val_tpl\":\"{{value_json." D_CMND_SCHEME "}}\","
  "\"fx_list\":[\"0\",\"1\",\"2\",\"3\",\"4\"]";   // string list with reference to scheme parameter.

const char HASS_DISCOVER_SENSOR[] PROGMEM =
  "{\"name\":\"%s\","                                 // dualr2 1 BTN
  "\"stat_t\":\"%s\","                                // cmnd/dualr2/POWER  (implies "\"optimistic\":\"false\",")
  "\"avty_t\":\"%s\","                                // tele/dualr2/LWT
  "\"pl_avail\":\"" D_ONLINE "\","                    // Online
  "\"pl_not_avail\":\"" D_OFFLINE "\"";               // Offline

const char HASS_DISCOVER_SENSOR_TEMP[] PROGMEM =
  "%s,\"unit_of_meas\":\"°%c\","                      // °C / °F
  "\"val_tpl\":\"{{value_json['%s'].Temperature}}\""; // "SI7021-14":{"Temperature":null,"Humidity":null} -> {{ value_json['SI7021-14'].Temperature }}

const char HASS_DISCOVER_SENSOR_HUM[] PROGMEM =
  "%s,\"unit_of_meas\":\"%%\","                       // %
  "\"val_tpl\":\"{{value_json['%s'].Humidity}}\","    // "SI7021-14":{"Temperature":null,"Humidity":null} -> {{ value_json['SI7021-14'].Humidity }}
  "\"dev_cla\":\"humidity\"";                         // humidity

const char HASS_DISCOVER_SENSOR_PRESS[] PROGMEM =
  "%s,\"unit_of_meas\":\"%s\","                      // PressureUnit() setting
  "\"val_tpl\":\"{{value_json['%s'].Pressure}}\","    // "BME280":{"Temperature":19.7,"Humidity":27.8,"Pressure":990.1} -> {{ value_json['BME280'].Pressure }}
  "\"dev_cla\":\"pressure\"";                         // pressure

//ENERGY
const char HASS_DISCOVER_SENSOR_KWH[] PROGMEM =
  "%s,\"unit_of_meas\":\"kWh\","                      // kWh
  "\"val_tpl\":\"{{value_json['%s'].%s}}\""; // "ENERGY":{"TotalStartTime":null,"Total":null,"Yesterday":null,"Today":null,"Power":null,"ApparentPower":null,"ReactivePower":null,"Factor":null,"Voltage":null,"Current":null} -> {{ value_json['ENERGY'].Total/Yesterday/Today }}

const char HASS_DISCOVER_SENSOR_WATT[] PROGMEM =
  "%s,\"unit_of_meas\":\"W\","                      // W
  "\"val_tpl\":\"{{value_json['%s'].%s}}\""; // "ENERGY":{"TotalStartTime":null,"Total":null,"Yesterday":null,"Today":null,"Power":null,"ApparentPower":null,"ReactivePower":null,"Factor":null,"Voltage":null,"Current":null} -> {{ value_json['ENERGY'].POWER }}

const char HASS_DISCOVER_SENSOR_VOLTAGE[] PROGMEM =
  "%s,\"unit_of_meas\":\"V\","                      // V
  "\"val_tpl\":\"{{value_json['%s'].%s}}\""; // "ENERGY":{"TotalStartTime":null,"Total":null,"Yesterday":null,"Today":null,"Power":null,"ApparentPower":null,"ReactivePower":null,"Factor":null,"Voltage":null,"Current":null} -> {{ value_json['ENERGY'].Voltage }}

const char HASS_DISCOVER_SENSOR_AMPERE[] PROGMEM =
  "%s,\"unit_of_meas\":\"A\","                      // A
  "\"val_tpl\":\"{{value_json['%s'].%s}}\""; // "ENERGY":{"TotalStartTime":null,"Total":null,"Yesterday":null,"Today":null,"Power":null,"ApparentPower":null,"ReactivePower":null,"Factor":null,"Voltage":null,"Current":null} -> {{ value_json['ENERGY'].Current }}

const char HASS_DISCOVER_SENSOR_ANY[] PROGMEM =
  "%s,\"unit_of_meas\":\" \","                        // " " As unit of measurement to get a value graph in Hass
  "\"val_tpl\":\"{{value_json['%s'].%s}}\"";          // "COUNTER":{"C1":0} -> {{ value_json['COUNTER'].C1 }}

const char HASS_DISCOVER_SENSOR_HASS_STATUS[] PROGMEM =
  "%s,\"json_attributes_topic\":\"%s\","
  "\"unit_of_meas\":\" \","                          // " " As unit of measurement to get a value graph in Hass
  "\"val_tpl\":\"{{value_json['" D_JSON_RSSI "']}}\"";// "COUNTER":{"C1":0} -> {{ value_json['COUNTER'].C1 }}

const char HASS_DISCOVER_DEVICE_INFO[] PROGMEM =
  "%s,\"uniq_id\":\"%s\","
  "\"device\":{\"identifiers\":[\"%06X\"],"
  "\"name\":\"%s\","
  "\"model\":\"%s\","
  "\"sw_version\":\"%s%s\","
  "\"manufacturer\":\"Tasmota\"}";

const char HASS_DISCOVER_DEVICE_INFO_SHORT[] PROGMEM =
  "%s,\"uniq_id\":\"%s\","
  "\"device\":{\"identifiers\":[\"%06X\"]}";

const char HASS_DISCOVER_TOPIC_PREFIX[] PROGMEM =
  "%s, \"~\":\"%s\"";

uint8_t hass_init_step = 0;
uint8_t hass_mode = 0;
int hass_tele_period = 0;

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
  if (len > prefixlen && prefixlen != 0 && !strncmp(*s, prefix, prefixlen)) {
    *s += prefixlen-1;
    *s[0] = '~';
  }
}

void try_snprintf_P(char *s, int n, const char *format, ... )
{
  va_list args;
  va_start(args, format);
  char dummy[2];
  int len = vsnprintf_P(dummy, 1, format, args);
  if (len >= n) {
    AddLog_P2(LOG_LEVEL_ERROR, PSTR("ERROR: MQTT discovery failed due to too long topic or friendly name. "
                                    "Please shorten topic and friendly name. Failed to format(%u/%u):"), len, n);
    va_start(args, format);
    vsnprintf_P(log_data, sizeof(log_data), format, args);
    AddLog(LOG_LEVEL_ERROR);
  } else {
    va_start(args, format);
    vsnprintf_P(s, n, format, args);
  }
  va_end(args);
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

    // Clear "other" topic first in case the device has been reconfigured from light to switch or vice versa
    snprintf_P(unique_id, sizeof(unique_id), PSTR("%06X_%s_%d"), ESP.getChipId(), (is_topic_light) ? "RL" : "LI", i);
    snprintf_P(stopic, sizeof(stopic), PSTR(HOME_ASSISTANT_DISCOVERY_PREFIX "/%s/%s/config"),
               (is_topic_light) ? "switch" : "light", unique_id);
    MqttPublish(stopic, true);
    // Clear or Set topic
    snprintf_P(unique_id, sizeof(unique_id), PSTR("%06X_%s_%d"), ESP.getChipId(), (is_topic_light) ? "LI" : "RL", i);
    snprintf_P(stopic, sizeof(stopic), PSTR(HOME_ASSISTANT_DISCOVERY_PREFIX "/%s/%s/config"),
               (is_topic_light) ? "light" : "switch", unique_id);

    if (Settings.flag.hass_discovery && (i <= devices_present)) {
      char name[33+2]; // friendlyname(33) + " " + index
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
      Shorten(&command_topic, prefix);
      Shorten(&state_topic, prefix);
      Shorten(&availability_topic, prefix);

      try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_RELAY,
                 name, command_topic, state_topic, value_template, Settings.state_text[0], Settings.state_text[1], availability_topic);
      try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_DEVICE_INFO_SHORT, mqtt_data,
                 unique_id, ESP.getChipId());
      try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_TOPIC_PREFIX, mqtt_data, prefix);

      if (is_light) {
        char *brightness_command_topic = stemp1;

        GetTopic_P(brightness_command_topic, CMND, mqtt_topic, D_CMND_DIMMER);
        Shorten(&brightness_command_topic, prefix);
        try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_LIGHT_DIMMER, mqtt_data, brightness_command_topic, state_topic);

        if (light_subtype >= LST_RGB) {
          char *rgb_command_topic = stemp1;

          GetTopic_P(rgb_command_topic, CMND, mqtt_topic, D_CMND_COLOR);
          Shorten(&rgb_command_topic, prefix);
          try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_LIGHT_COLOR, mqtt_data, rgb_command_topic, state_topic);

          char *effect_command_topic = stemp1;
          GetTopic_P(effect_command_topic, CMND, mqtt_topic, D_CMND_SCHEME);
          Shorten(&effect_command_topic, prefix);
          try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_LIGHT_SCHEME, mqtt_data, effect_command_topic, state_topic);

        }
        if (LST_RGBW == light_subtype) {
          char *white_temp_command_topic = stemp1;

          GetTopic_P(white_temp_command_topic, CMND, mqtt_topic, D_CMND_WHITE);
          Shorten(&white_temp_command_topic, prefix);
          try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_LIGHT_WHITE, mqtt_data, white_temp_command_topic, state_topic);
        }
        if ((LST_COLDWARM == light_subtype) || (LST_RGBWC == light_subtype)) {
          char *color_temp_command_topic = stemp1;

          GetTopic_P(color_temp_command_topic, CMND, mqtt_topic, D_CMND_COLORTEMPERATURE);
          Shorten(&color_temp_command_topic, prefix);
          try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_LIGHT_CT, mqtt_data, color_temp_command_topic, state_topic);
        }
      }
      try_snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
    }
    MqttPublish(stopic, true);
  }
}

void HAssAnnounceButtonSwitch(uint8_t device, char* topic, uint8_t present, uint8_t key, uint8_t toggle)
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
    char name[33+6]; // friendlyname(33) + " " + "BTN" + " " + index
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
    Shorten(&state_topic, prefix);
    Shorten(&availability_topic, prefix);
    try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_BUTTON_SWITCH,
               name, state_topic, Settings.state_text[toggle?2:1], availability_topic);
    try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_DEVICE_INFO_SHORT, mqtt_data,
               unique_id, ESP.getChipId());
    if (strlen(prefix) > 0 ) try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_TOPIC_PREFIX, mqtt_data, prefix);
    if (toggle) try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_BUTTON_SWITCH_TOGGLE, mqtt_data);
    else try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_BUTTON_SWITCH_ONOFF, mqtt_data, Settings.state_text[0]);

    try_snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
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
    for (uint8_t switch_index = 0; switch_index < MAX_SWITCHES; switch_index++) {
      uint8_t switch_present = 0;
      uint8_t toggle = 1;

      if (pin[GPIO_SWT1 + switch_index] < 99) {
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
    for (uint8_t button_index = 0; button_index < MAX_KEYS; button_index++) {
      uint8_t button_present = 0;
      uint8_t toggle = 1;

      if (!button_index && ((SONOFF_DUAL == my_module_type) || (CH4 == my_module_type))) {
        button_present = 1;
      } else {
        if (pin[GPIO_KEY1 + button_index] < 99) {
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
    char name[33+42]; // friendlyname(33) + " " + sensorname(20?) + " " + sensortype(20?)
    char prefix[TOPSZ];
    char *state_topic = stemp1;
    char *availability_topic = stemp2;

    snprintf_P(name, sizeof(name), PSTR("%s %s %s"), Settings.friendlyname[0], sensorname, subsensortype);
    GetTopic_P(state_topic, TELE, mqtt_topic, PSTR(D_RSLT_SENSOR));
    GetTopic_P(availability_topic, TELE, mqtt_topic, S_LWT);
    FindPrefix(state_topic, availability_topic, prefix);
    Shorten(&state_topic, prefix);
    Shorten(&availability_topic, prefix);

    try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_SENSOR,
               name, state_topic, availability_topic);
    try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_DEVICE_INFO_SHORT, mqtt_data,
               unique_id, ESP.getChipId());
    try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_TOPIC_PREFIX, mqtt_data, prefix);
    if (!strcmp_P(subsensortype, PSTR(D_JSON_TEMPERATURE))) {
      try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_SENSOR_TEMP,
                 mqtt_data, TempUnit(), sensorname);
    } else if (!strcmp_P(subsensortype, PSTR(D_JSON_HUMIDITY))) {
      try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_SENSOR_HUM,
                 mqtt_data, sensorname);
    } else if (!strcmp_P(subsensortype, PSTR(D_JSON_PRESSURE))) {
      try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_SENSOR_PRESS,
                 mqtt_data, PressureUnit().c_str(), sensorname);
    } else if (!strcmp_P(subsensortype, PSTR(D_JSON_TOTAL))
               || !strcmp_P(subsensortype, PSTR(D_JSON_TODAY))
               || !strcmp_P(subsensortype, PSTR(D_JSON_YESTERDAY))){
      try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_SENSOR_KWH,
                 mqtt_data, sensorname, subsensortype);
    } else if (!strcmp_P(subsensortype, PSTR(D_JSON_POWERUSAGE))){
      try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_SENSOR_WATT,
                 mqtt_data, sensorname, subsensortype);
    } else if (!strcmp_P(subsensortype, PSTR(D_JSON_VOLTAGE))){
      try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_SENSOR_VOLTAGE,
                 mqtt_data, sensorname, subsensortype);
    } else if (!strcmp_P(subsensortype, PSTR(D_JSON_CURRENT))){
      try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_SENSOR_AMPERE,
                 mqtt_data, sensorname, subsensortype);
    }
    else {
      try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_SENSOR_ANY,
                 mqtt_data, sensorname, subsensortype);
    }
    try_snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
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

    char sensordata[256];                             // Copy because we need to write to mqtt_data
    strlcpy(sensordata, mqtt_data, sizeof(sensordata));

    if (strlen(sensordata)) {
      sensordata[0] = '{';                             // {"INA219":{"Voltage":4.494,"Current":0.020,"Power":0.089}
      snprintf_P(sensordata, sizeof(sensordata), PSTR("%s}"), sensordata);  // {"INA219":{"Voltage":4.494,"Current":0.020,"Power":0.089}}

      // JsonBuffer size calculation (https://arduinojson.org/v5/assistant/)
      // 383 = {"MCP230XX":{"D0":0,"D1":0,"D2":0,"D3":0,"D4":0,"D5":0,"D6":0,"D7":0,"D8":0,"D9":0,"D10":0,"D11":0,"D12":0,"D13":0,"D14":0,"D15":0}}
      // 381 = {"MPR121A":{"Button0":0,"Button1":0,"Button2":0,"Button3":0,"Button4":0,"Button5":0,"Button6":0,"Button7":0,"Button8":0,"Button9":0,"Button10":0,"Button11":0,"Button12":0}}
      // 420 = {"ENERGY":{"TotalStartTime":"2018-10-30T17:09:47","Total":2.684,"Yesterday":0.020,"Today":0.006,"Period":0.04,"Power":0.49,"ApparentPower":4.71,"ReactivePower":4.70,"Factor":0.10,"Frequency":50.04,"Voltage":226.3,"Current":0.021}}
      StaticJsonBuffer<500> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(sensordata);
      if (!root.success()) {
        AddLog_P2(LOG_LEVEL_ERROR, PSTR("HASS: failed to parse '%s'"), sensordata);
        continue;
      }
      for (auto sensor : root) {
        const char* sensorname = sensor.key;
        JsonObject& sensors = sensor.value.as<JsonObject>();
        if (!sensors.success()) {
          AddLog_P2(LOG_LEVEL_ERROR, PSTR("HASS: failed to parse '%s'"), sensordata);
          continue;
        }
        for (auto subsensor : sensors) {
          HAssAnnounceSensor(sensorname, subsensor.key);
        }
      }
    }
    yield();
  } while (hass_xsns_index != 0);
}

void HAssAnnounceStatusSensor(void)
{
  char stopic[TOPSZ];
  char stemp1[TOPSZ];
  char stemp2[TOPSZ];
  char unique_id[30];

  // Announce sensor
  mqtt_data[0] = '\0';  // Clear retained message

  // Clear or Set topic
  snprintf_P(unique_id, sizeof(unique_id), PSTR("%06X_%s"), ESP.getChipId(), "status");
  snprintf_P(stopic, sizeof(stopic), PSTR(HOME_ASSISTANT_DISCOVERY_PREFIX "/sensor/%s/config"), unique_id);

  if (Settings.flag.hass_discovery) {
    char name[33+7]; // friendlyname(33) + " " + "status"
    char prefix[TOPSZ];
    char *state_topic = stemp1;
    char *availability_topic = stemp2;

    snprintf_P(name, sizeof(name), PSTR("%s %s"), Settings.friendlyname[0], "status");
    GetTopic_P(state_topic, TELE, mqtt_topic, PSTR(D_RSLT_HASS_STATE));
    GetTopic_P(availability_topic, TELE, mqtt_topic, S_LWT);
    FindPrefix(state_topic, availability_topic, prefix);
    Shorten(&state_topic, prefix);
    Shorten(&availability_topic, prefix);

    try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_SENSOR,
               name, state_topic, availability_topic);
    try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_SENSOR_HASS_STATUS,
                 mqtt_data, state_topic);
    try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_DEVICE_INFO, mqtt_data,
               unique_id, ESP.getChipId(),
               Settings.friendlyname[0], ModuleName().c_str(), my_version, my_image);
    try_snprintf_P(mqtt_data, sizeof(mqtt_data)-1, HASS_DISCOVER_TOPIC_PREFIX, mqtt_data, prefix);
    try_snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
  }
  MqttPublish(stopic, true);
}

void HAssPublishStatus(void)
{
  snprintf_P(mqtt_data, sizeof(mqtt_data),
             PSTR("{\"" D_JSON_VERSION "\":\"%s%s\",\"" D_JSON_BUILDDATETIME "\":\"%s\","
             "\"" D_JSON_COREVERSION "\":\"" ARDUINO_ESP8266_RELEASE "\",\"" D_JSON_SDKVERSION "\":\"%s\","
             "\"" D_CMND_MODULE "\":\"%s\",\"" D_JSON_RESTARTREASON "\":\"%s\",\"" D_JSON_UPTIME "\":\"%s\","
             "\"WiFi " D_JSON_LINK_COUNT "\":%d,\"WiFi " D_JSON_DOWNTIME "\":\"%s\",\"" D_JSON_MQTT_COUNT "\":%d,"
             "\"" D_JSON_BOOTCOUNT "\":%d,\"" D_JSON_SAVECOUNT "\":%d,\"" D_CMND_IPADDRESS "\":\"%s\","
             "\"" D_JSON_RSSI "\":\"%d\",\"LoadAvg\":%lu}"),
             my_version, my_image, GetBuildDateAndTime().c_str(), ESP.getSdkVersion(), ModuleName().c_str(),
             GetResetReason().c_str(), GetUptime().c_str(), WifiLinkCount(), WifiDowntime().c_str(), MqttConnectCount(),
             Settings.bootcount, Settings.save_flag, WiFi.localIP().toString().c_str(),
             WifiGetRssiAsQuality(WiFi.RSSI()), loop_load_avg);
  MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_HASS_STATE));
}

void HAssDiscovery(void)
{
  // Configure Tasmota for default Home Assistant parameters to keep discovery message as short as possible
  if (Settings.flag.hass_discovery) {
    Settings.flag.mqtt_response = 0;         // Response always as RESULT and not as uppercase command
    Settings.flag.decimal_text = 1;          // Respond with decimal color values
    Settings.flag3.hass_tele_on_power = 1;   // send tele/STATE message as stat/RESULT
//    Settings.light_scheme = 0;             // To just control color it needs to be Scheme 0
    if (strcmp_P(Settings.mqtt_fulltopic, PSTR("%topic%/%prefix%/"))) {
      strncpy_P(Settings.mqtt_fulltopic, PSTR("%topic%/%prefix%/"), sizeof(Settings.mqtt_fulltopic));
      restart_flag = 2;
      return;                                // As full topic has changed do restart first before sending discovery data
    }
  }

  if (Settings.flag.hass_discovery || (1 == hass_mode)) {
    // Send info about relays and lights
    HAssAnnounceRelayLight();

    // Send info about buttons
    HAssAnnounceButtons();

    // Send info about switches
    HAssAnnounceSwitches();

    // Send info about sensors
    HAssAnnounceSensors();

    // Send info about status sensor
    HAssAnnounceStatusSensor();
  }
}

void HAssDiscover(void)
{
  hass_mode = 1;                             // Force discovery
  hass_init_step = 1;                        // Delayed discovery
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv12(uint8_t function)
{
  bool result = false;

  if (Settings.flag.mqtt_enabled) {
    switch (function) {
      case FUNC_MQTT_INIT:
        hass_mode = 0;                       // Discovery only if Settings.flag.hass_discovery is set
        hass_init_step = 2;                  // Delayed discovery
        break;
      case FUNC_EVERY_SECOND:
        if (hass_init_step) {
          hass_init_step--;
          if (!hass_init_step) {
            HAssDiscovery();                 // Scheduled discovery using available resources
          }
        } else if (Settings.flag.hass_discovery && Settings.tele_period) {
          hass_tele_period++;
          if (hass_tele_period >= Settings.tele_period) {
            hass_tele_period = 0;

            mqtt_data[0] = '\0';
            HAssPublishStatus();
          }
        }
        break;
    }
  }
  return result;
}

#endif  // USE_HOME_ASSISTANT
