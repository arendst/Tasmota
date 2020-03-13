/*
  xdrv_12_home_assistant.ino - home assistant support for Tasmota

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

#ifdef USE_HOME_ASSISTANT

#define XDRV_12 12

// List of sensors ready for discovery
const char kHAssJsonSensorTypes[] PROGMEM =
  D_JSON_TEMPERATURE "|" D_JSON_PRESSURE "|" D_JSON_PRESSUREATSEALEVEL "|"
  D_JSON_APPARENT_POWERUSAGE "|Battery|" D_JSON_CURRENT "|" D_JSON_DISTANCE "|" D_JSON_FREQUENCY "|" D_JSON_HUMIDITY "|" D_JSON_ILLUMINANCE "|"
  D_JSON_MOISTURE "|PB0.3|PB0.5|PB1|PB2.5|PB5|PB10|PM1|PM2.5|PM10|" D_JSON_POWERFACTOR "|" D_JSON_POWERUSAGE "|"
  D_JSON_REACTIVE_POWERUSAGE "|" D_JSON_TODAY "|" D_JSON_TOTAL "|" D_JSON_VOLTAGE "|" D_JSON_WEIGHT "|" D_JSON_YESTERDAY
  D_JSON_CO2 "|" D_JSON_ECO2 "|" D_JSON_TVOC;
const char kHAssJsonSensorUnits[] PROGMEM =
  "|||"
  "VA|%|A|Cm|Hz|%|LX|"
  "%|ppd|ppd|ppd|ppd|ppd|ppd|µg/m³|µg/m³|µg/m³|Cos φ|W|"
  "VAr|kWh|kWh|V|Kg|kWh|"
  "ppm|ppm|ppb|";
const char kHAssJsonSensorDevCla[] PROGMEM =
  "dev_cla\":\"temperature|dev_cla\":\"pressure|dev_cla\":\"pressure|"
  "dev_cla\":\"power|dev_cla\":\"battery|ic\":\"mdi:alpha-a-circle-outline|ic\":\"mdi:leak|ic\":\"mdi:current-ac|dev_cla\":\"humidity|dev_cla\":\"illuminance|"
  "ic\":\"mdi:cup-water|ic\":\"mdi:flask|ic\":\"mdi:flask|ic\":\"mdi:flask|ic\":\"mdi:flask|ic\":\"mdi:flask|ic\":\"mdi:flask|"
  "ic\":\"mdi:air-filter|ic\":\"mdi:air-filter|ic\":\"mdi:air-filter|ic\":\"mdi:alpha-f-circle-outline|dev_cla\":\"power|"
  "dev_cla\":\"power|dev_cla\":\"power|dev_cla\":\"power|ic\":\"mdi:alpha-v-circle-outline|ic\":\"mdi:scale|dev_cla\":\"power"
  "ic\":\"mdi:periodic-table-co2|ic\":\"mdi:air-filter|ic\":\"mdi:periodic-table-co2";
// List of sensors ready for discovery

const char HASS_DISCOVER_SENSOR[] PROGMEM =
  ",\"unit_of_meas\":\"%s\",\"%s\","           // unit of measure and class (or icon)
  "\"frc_upd\":true,"                          // force update for better graph representation
  "\"val_tpl\":\"{{value_json['%s']['%s']";    // "COUNTER":{"C1":0} -> {{ value_json['COUNTER'].['C1']

const char HASS_DISCOVER_BASE[] PROGMEM =
  "{\"name\":\"%s\","                          // dualr2 1
  "\"stat_t\":\"%s\","                         // stat/dualr2/RESULT  (implies "\"optimistic\":\"false\",")
  "\"avty_t\":\"%s\","                         // tele/dualr2/LWT
  "\"pl_avail\":\"" D_ONLINE "\","             // Online
  "\"pl_not_avail\":\"" D_OFFLINE "\"";        // Offline

const char HASS_DISCOVER_RELAY[] PROGMEM =
  ",\"cmd_t\":\"%s\","                         // cmnd/dualr2/POWER2
  "\"val_tpl\":\"{{value_json.%s}}\","         // POWER2
  "\"pl_off\":\"%s\","                         // OFF
  "\"pl_on\":\"%s\"";                          // ON

const char HASS_DISCOVER_BIN_SWITCH[] PROGMEM =
  ",\"val_tpl\":\"{{value_json.%s}}\","        // STATE
  "\"frc_upd\":true,"                          // In ON/OFF case, enable force_update to make automations work
  "\"pl_on\":\"%s\","                          // ON
  "\"pl_off\":\"%s\"";                         // OFF

const char HASS_DISCOVER_LIGHT_DIMMER[] PROGMEM =
  ",\"bri_cmd_t\":\"%s\","                     // cmnd/led2/Dimmer
  "\"bri_stat_t\":\"%s\","                     // stat/led2/RESULT
  "\"bri_scl\":100,"                           // 100%
  "\"on_cmd_type\":\"%s\","                    // power on (first), power on (last), no power on (brightness)
  "\"bri_val_tpl\":\"{{value_json." D_CMND_DIMMER "}}\"";

const char HASS_DISCOVER_LIGHT_COLOR[] PROGMEM =
  ",\"rgb_cmd_t\":\"%s2\","                    // cmnd/led2/Color2
  "\"rgb_stat_t\":\"%s\","                     // stat/led2/RESULT
  "\"rgb_val_tpl\":\"{{value_json." D_CMND_COLOR ".split(',')[0:3]|join(',')}}\"";

const char HASS_DISCOVER_LIGHT_WHITE[] PROGMEM =
  ",\"whit_val_cmd_t\":\"%s\","               // cmnd/led2/White
  "\"whit_val_stat_t\":\"%s\","               // stat/led2/RESULT
  "\"whit_val_scl\":100,"
  "\"whit_val_tpl\":\"{{value_json.Channel[3]}}\"";

const char HASS_DISCOVER_LIGHT_CT[] PROGMEM =
  ",\"clr_temp_cmd_t\":\"%s\","               // cmnd/led2/CT
  "\"clr_temp_stat_t\":\"%s\","               // stat/led2/RESULT
  "\"clr_temp_val_tpl\":\"{{value_json." D_CMND_COLORTEMPERATURE "}}\"";

const char HASS_DISCOVER_LIGHT_SCHEME[] PROGMEM =
  ",\"fx_cmd_t\":\"%s\","                         // cmnd/led2/Scheme
  "\"fx_stat_t\":\"%s\","                         // stat/led2/RESULT
  "\"fx_val_tpl\":\"{{value_json." D_CMND_SCHEME "}}\","
  "\"fx_list\":[\"0\",\"1\",\"2\",\"3\",\"4\"]";  // string list with reference to scheme parameter.

const char HASS_DISCOVER_SENSOR_HASS_STATUS[] PROGMEM =
  ",\"json_attr_t\":\"%s\","
  "\"unit_of_meas\":\"%%\","
  "\"val_tpl\":\"{{value_json['" D_JSON_RSSI "']}}\","
  "\"ic\":\"mdi:information-outline\"";

const char HASS_DISCOVER_DEVICE_INFO[] PROGMEM =
  ",\"uniq_id\":\"%s\","
  "\"dev\":{\"ids\":[\"%06X\"],"
  "\"name\":\"%s\","
  "\"mdl\":\"%s\","
  "\"sw\":\"%s%s\","
  "\"mf\":\"Tasmota\"}";

const char HASS_DISCOVER_DEVICE_INFO_SHORT[] PROGMEM =
  ",\"uniq_id\":\"%s\","
  "\"dev\":{\"ids\":[\"%06X\"]}";

const char HASS_TRIGGER_TYPE[] PROGMEM =
  "{\"atype\":\"trigger\","
  "\"t\":\"%sT\","
  "\"pl\":\"{\\\"TRIG\\\":\\\"%s\\\"}\","
  "\"type\":\"%s\","
  "\"stype\":\"%s\","
  "\"dev\":{\"ids\":[\"%06X\"]}}";

const char kHAssTriggerType[] PROGMEM =
  "none|button_short_press|button_long_press|button_double_press";

uint8_t hass_init_step = 0;
uint8_t hass_mode = 0;
int hass_tele_period = 0;

void TryResponseAppend_P(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  char dummy[2];
  int dlen = vsnprintf_P(dummy, 1, format, args);

  int mlen = strlen(mqtt_data);
  int slen = sizeof(mqtt_data) - 1 - mlen;
  if (dlen >= slen)
  {
    AddLog_P2(LOG_LEVEL_ERROR, PSTR("HASS: MQTT discovery failed due to too long topic or friendly name. "
                                    "Please shorten topic and friendly name. Failed to format(%u/%u):"),
              dlen, slen);
    va_start(args, format);
    vsnprintf_P(log_data, sizeof(log_data), format, args);
    AddLog(LOG_LEVEL_ERROR);
  }
  else
  {
    va_start(args, format);
    vsnprintf_P(mqtt_data + mlen, slen, format, args);
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

  for (uint32_t i = 1; i <= MAX_RELAYS; i++)
  {
    is_light = ((i == devices_present) && (light_type));
    is_topic_light = Settings.flag.hass_light || is_light; // SetOption30 - Enforce HAss autodiscovery as light

    mqtt_data[0] = '\0'; // Clear retained message

    // Clear "other" topic first in case the device has been reconfigured from light to switch or vice versa
    snprintf_P(unique_id, sizeof(unique_id), PSTR("%06X_%s_%d"), ESP.getChipId(), (is_topic_light) ? "RL" : "LI", i);
    snprintf_P(stopic, sizeof(stopic), PSTR(HOME_ASSISTANT_DISCOVERY_PREFIX "/%s/%s/config"),
               (is_topic_light) ? "switch" : "light", unique_id);
    MqttPublish(stopic, true);
    // Clear or Set topic
    snprintf_P(unique_id, sizeof(unique_id), PSTR("%06X_%s_%d"), ESP.getChipId(), (is_topic_light) ? "LI" : "RL", i);
    snprintf_P(stopic, sizeof(stopic), PSTR(HOME_ASSISTANT_DISCOVERY_PREFIX "/%s/%s/config"),
               (is_topic_light) ? "light" : "switch", unique_id);

    if (Settings.flag.hass_discovery && (i <= devices_present))
    {                    // SetOption19 - Control Home Assistantautomatic discovery (See SetOption59)
      char name[33 + 2]; // friendlyname(33) + " " + index
      char value_template[33];
      char prefix[TOPSZ];
      char *command_topic = stemp1;
      char *state_topic = stemp2;
      char *availability_topic = stemp3;

      if (i > MAX_FRIENDLYNAMES) {
        snprintf_P(name, sizeof(name), PSTR("%s %d"), SettingsText(SET_FRIENDLYNAME1), i);
      } else {
        snprintf_P(name, sizeof(name), SettingsText(SET_FRIENDLYNAME1 + i - 1));
      }
      GetPowerDevice(value_template, i, sizeof(value_template), Settings.flag.device_index_enable); // SetOption26 - Switch between POWER or POWER1
      GetTopic_P(command_topic, CMND, mqtt_topic, value_template);
      GetTopic_P(state_topic, TELE, mqtt_topic, D_RSLT_STATE);
      GetTopic_P(availability_topic, TELE, mqtt_topic, S_LWT);

      Response_P(HASS_DISCOVER_BASE, name, state_topic, availability_topic);
      TryResponseAppend_P(HASS_DISCOVER_RELAY, command_topic, value_template, SettingsText(SET_STATE_TXT1), SettingsText(SET_STATE_TXT2));
      TryResponseAppend_P(HASS_DISCOVER_DEVICE_INFO_SHORT, unique_id, ESP.getChipId());

#if defined(USE_LIGHT) || defined(USE_PWM_DIMMER)
      if (is_light || PWM_DIMMER == my_module_type)
      {
        char *brightness_command_topic = stemp1;

        GetTopic_P(brightness_command_topic, CMND, mqtt_topic, D_CMND_DIMMER);
        strncpy_P(stemp3, Settings.flag.not_power_linked ? PSTR("last") : PSTR("brightness"), sizeof(stemp3)); // SetOption20 - Control power in relation to Dimmer/Color/Ct changes
        TryResponseAppend_P(HASS_DISCOVER_LIGHT_DIMMER, brightness_command_topic, state_topic, stemp3);

#ifdef USE_LIGHT
        if (Light.subtype >= LST_RGB)
        {
          char *rgb_command_topic = stemp1;

          GetTopic_P(rgb_command_topic, CMND, mqtt_topic, D_CMND_COLOR);
          TryResponseAppend_P(HASS_DISCOVER_LIGHT_COLOR, rgb_command_topic, state_topic);

          char *effect_command_topic = stemp1;
          GetTopic_P(effect_command_topic, CMND, mqtt_topic, D_CMND_SCHEME);
          TryResponseAppend_P(HASS_DISCOVER_LIGHT_SCHEME, effect_command_topic, state_topic);
        }
        if (LST_RGBW == Light.subtype)
        {
          char *white_temp_command_topic = stemp1;

          GetTopic_P(white_temp_command_topic, CMND, mqtt_topic, D_CMND_WHITE);
          TryResponseAppend_P(HASS_DISCOVER_LIGHT_WHITE, white_temp_command_topic, state_topic);
        }
        if ((LST_COLDWARM == Light.subtype) || (LST_RGBCW == Light.subtype))
        {
          char *color_temp_command_topic = stemp1;

          GetTopic_P(color_temp_command_topic, CMND, mqtt_topic, D_CMND_COLORTEMPERATURE);
          TryResponseAppend_P(HASS_DISCOVER_LIGHT_CT, color_temp_command_topic, state_topic);
        }
#endif  // USE_LIGHT
      }
#endif  // defined(USE_LIGHT) || defined(USE_PWM_DIMMER)
      TryResponseAppend_P(PSTR("}"));
    }
    MqttPublish(stopic, true);
  }
}

void HAssAnnouncerTriggers(uint8_t device, uint8_t present, uint8_t key, uint8_t toggle, uint8_t hold)
{
  // key 0 = button
  // key 1 = switch
  char stopic[TOPSZ];
  char stemp1[TOPSZ];
  char stemp2[TOPSZ];
  char unique_id[30];

  mqtt_data[0] = '\0'; // Clear retained message

  for (uint8_t i = 2; i <= 3; i++) {
    snprintf_P(unique_id, sizeof(unique_id), PSTR("%06X_%s_%d_%s"), ESP.getChipId(), key ? "SW" : "BTN", device + 1, GetStateText(i));
    snprintf_P(stopic, sizeof(stopic), PSTR(HOME_ASSISTANT_DISCOVERY_PREFIX "/device_automation/%s/config"), unique_id);

    if (Settings.flag.hass_discovery && present) {                // SetOption19 - Control Home Assistantautomatic discovery (See SetOption59)
      char name[33 + 6];        // friendlyname(33) + " " + "BTN" + " " + index
      char value_template[33];
      char prefix[TOPSZ];
      char *state_topic = stemp1;
      char *availability_topic = stemp2;
      char jsoname[8];

      GetPowerDevice(value_template, device + 1, sizeof(value_template), key + Settings.flag.device_index_enable);     // Force index for Switch 1, Index on Button1 is controlled by SetOption26 - Switch between POWER or POWER1
      snprintf_P(jsoname, sizeof(jsoname), PSTR("%s%d"), key ? "SWITCH" : "BUTTON", device + 1);
      GetTopic_P(state_topic, STAT, mqtt_topic, jsoname);
      GetTopic_P(availability_topic, TELE, mqtt_topic, S_LWT);

      char param[21];
      char subtype[9];
      uint8_t pload = toggle;

      if ((i == 2 && toggle != 0) || (i == 3 && hold != 0)) {
        if (i == 3) { pload = hold; }
        GetTextIndexed(param, sizeof(param), pload, kHAssTriggerType);
        snprintf_P(subtype, sizeof(subtype), PSTR("%s_%d"), key ? "switch" : "button", device + 1);
        Response_P(HASS_TRIGGER_TYPE, state_topic, GetStateText(i), param, subtype, ESP.getChipId());
      } else { mqtt_data[0] = '\0'; } // Need to be cleaned again to avoid duplicate.
    }
    MqttPublish(stopic, true);
  }
}

void HAssAnnouncerBinSensors(uint8_t device, uint8_t present, uint8_t dual, uint8_t toggle)
{
  char stopic[TOPSZ];
  char stemp1[TOPSZ];
  char stemp2[TOPSZ];
  char unique_id[30];

  mqtt_data[0] = '\0'; // Clear retained message

  snprintf_P(unique_id, sizeof(unique_id), PSTR("%06X_SW_%d"), ESP.getChipId(), device + 1);
  snprintf_P(stopic, sizeof(stopic), PSTR(HOME_ASSISTANT_DISCOVERY_PREFIX "/binary_sensor/%s/config"), unique_id);


  if (Settings.flag.hass_discovery && present ) {    // SetOption19 - Control Home Assistantautomatic discovery (See SetOption59)
    if (!toggle || dual) {
      char name[33 + 6];        // friendlyname(33) + " " + "BTN" + " " + index
      char value_template[33];
      char prefix[TOPSZ];
      char *state_topic = stemp1;
      char *availability_topic = stemp2;
      char jsoname[8];

      GetPowerDevice(value_template, device + 1, sizeof(value_template), 1 + Settings.flag.device_index_enable); // Force index for Switch 1, Index on Button1 is controlled by SetOption26 - Switch between POWER or POWER1
      snprintf_P(jsoname, sizeof(jsoname), PSTR("SWITCH%d"), device + 1);
      GetTopic_P(state_topic, STAT, mqtt_topic, jsoname);
      GetTopic_P(availability_topic, TELE, mqtt_topic, S_LWT);

      snprintf_P(name, sizeof(name), PSTR("%s Switch%d"), SettingsText(SET_FRIENDLYNAME1), device + 1);
      Response_P(HASS_DISCOVER_BASE, name, state_topic, availability_topic);
      TryResponseAppend_P(HASS_DISCOVER_BIN_SWITCH, PSTR(D_RSLT_STATE), SettingsText(SET_STATE_TXT2), SettingsText(SET_STATE_TXT1));
      TryResponseAppend_P(HASS_DISCOVER_DEVICE_INFO_SHORT, unique_id, ESP.getChipId());
      TryResponseAppend_P(PSTR("}"));
    }
  }
  MqttPublish(stopic, true);
}

void HAssAnnounceSwitches(void)
{
  for (uint32_t switch_index = 0; switch_index < MAX_SWITCHES; switch_index++)
  {
    uint8_t switch_present = 0;
    uint8_t dual = 0;
    uint8_t toggle = 1;
    uint8_t hold = 0;

    if (pin[GPIO_SWT1 + switch_index] < 99) { switch_present = 1; }
    
    if (KeyTopicActive(1) && strcmp(SettingsText(SET_MQTT_SWITCH_TOPIC), mqtt_topic))   // Enable Discovery for Switches only if Switchtopic is set to a custom name
    {

    // switch matrix for triggers and binary sensor generation when switchtopic is set as custom (default index is 0,0 - TOGGLE, TOGGLE):
    //  SWITCHMODE    INTERNAL              BINARY        PRESS                         DOUBLE PRESS                HOLD                        T,H
    //  0             TOGGLE                NO            TOGGLE (button_short_press)   NONE                        NONE                        1,0
    //  1             FOLLOW                YES           NONE                          NONE                        NONE                        0,0
    //  2             FOLLOW_INV            YES           NONE                          NONE                        NONE                        0,0
    //  3             PUSHBUTTON            YES           TOGGLE (button_short_press)   NONE                        NONE                        1,0
    //  4             PUSHBUTTON_INV        YES           TOGGLE (button_short_press)   NONE                        NONE                        1,0
    //  5             PUSHBUTTONHOLD        YES           TOGGLE (button_short_press)   NONE                        HOLD (button_long_press)    1,2
    //  6             PUSHBUTTONHOLD_INV    YES           TOGGLE (button_short_press)   NONE                        HOLD (button_long_press)    1,2
    //  7             PUSHBUTTON_TOGGLE     NO            TOGGLE (button_short_press)   NONE                        NONE                        1,0
    //  8             TOGGLEMULTI           NO            TOGGLE (button_short_press)   HOLD (button_double_press)  NONE                        1,3
    //  9             FOLLOWMULTI           YES           NONE                          HOLD (button_double_press)  NONE                        0,3
    // 10             FOLLOWMULTI_INV       YES           NONE                          HOLD (button_double_press)  NONE                        0,3
    // 11             PUSHHOLDMULTI         NO            TOGGLE (button_short_press)   NONE                        INC_DEC (button_long_press) 1,0
    //                                                    INV (not available)                                       CLEAR (not available)
    // 12             PUSHHOLDMULTI_INV     NO            TOGGLE (button_short_press)   NONE                        CLEAR (button_long_press)   1,0
    //                                                    INV (not available)                                       INC_DEC (not available)
    // Please note: SwitchMode11 and 12 will register just TOGGLE (button_short_press)
    
    // Trigger types: "0 = none | 1 = button_short_press | 2 = button_long_press | 3 = button_double_press";

      uint8_t swmode = Settings.switchmode[switch_index];

      switch (swmode) {
        case 1:
        case 2:
          toggle = 0;     // Binary sensor and no triggers
          break;
        case 3:
        case 4:
          dual = 1;       // Binary sensor and TOGGLE (button_short_press) trigger
          break;
        case 5:
        case 6:
          dual = 1;       // Binary sensor, TOGGLE (button_short_press) and HOLD (button_long_press) triggers
          hold = 2;
          break;
        case 8:
          hold = 3;       // TOGGLE (button_short_press) and HOLD (button_double_press) triggers
          break;
        case 9:
        case 10:
          dual = 1;       // Binary sensor and HOLD (button_long_press) trigger
          toggle = 0;
          hold = 3;
          break;
      }
    
    } else { switch_present = 0;}

    HAssAnnouncerTriggers(switch_index, switch_present, 1, toggle, hold);
    HAssAnnouncerBinSensors(switch_index, switch_present, dual, toggle);
  }
}


void HAssAnnounceButtons(void)
{
  for (uint32_t button_index = 0; button_index < MAX_KEYS; button_index++)
  {
    uint8_t button_present = 0;
    uint8_t toggle = 1;
    uint8_t hold = 0;

    if (!button_index && ((SONOFF_DUAL == my_module_type) || (CH4 == my_module_type)))
    {
      button_present = 1;
    } else {
      if (pin[GPIO_KEY1 + button_index] < 99) {
        button_present = 1;
      }
    }

    // button matrix for triggers generation when buttontopic is set as custom (default TOGGLE = 1 HOLD = 0):
    // N  SetOption1  SetOption11 SetOption13 PRESS                         DOUBLE PRESS                  HOLD                        T,H
    // 1  0           0           0           TOGGLE (button_short_press)   NONE (toggle real relay)      NONE (reset device)         1,0
    // 2  1           0           0           TOGGLE (button_short_press)   NONE (toggle real relay)      HOLD (button_long_press)    1,2
    // 3  0           1           0           NONE (toggle real relay)      TOGGLE (button_double_press)  NONE (reset device)         3,0
    // 4  1           1           0           NONE (toggle real relay)      TOGGLE (button_double_press)  HOLD (button_long_press)    3,2
    // 5  0           0           1           TOGGLE (button_short_press)   NONE (toggle real relay)      NONE (reset device)         1,0
    // 6  1           0           1           TOGGLE (button_short_press)   NONE (toggle real relay)      NONE (MQTT HOLD)            1,0
    // 7  0           1           1           NONE (toggle real relay)      NONE (toggle real relay)      NONE (reset device)         0,0
    // 8  1           1           1           NONE (toggle real relay)      NONE (toggle real relay)      NONE (MQTT HOLD)            0.0

    // Trigger types: "0 = none | 1 = button_short_press | 2 = button_long_press | 3 = button_double_press";

    if (Settings.flag.button_restrict) {                  // [SetOption1]  Enable/Disable button multipress
      if (!Settings.flag.button_single) {
        hold = 2;                                         // Default TOGGLE (button_short_press) + HOLD (button_long_press) trigger if [SetOption13] is OFF                                               
      }
    }

    if (Settings.flag.button_swap) {                      // [SetOption11] Swap button single and double press functionality
        if (!Settings.flag.button_single) {
          if (!Settings.flag.button_restrict) {
            hold = 0;                                     // TOGGLE (button_double_press) and remove HOLD (button_long_press) trigger if [SetOption1] is OFF
          }
          toggle = 3;                                     // TOGGLE (button_double_press)            
        } else {toggle = 0; hold = 0;}                    // [SetOption13] Immediate action on button press, no TOGGLE or HOLD triggers
    }
   
    if (KeyTopicActive(0)) {                              // Enable Discovery for Buttons only if Buttontopic is set to 1 or a custom name

      if (!strcmp(SettingsText(SET_MQTT_BUTTON_TOPIC), mqtt_topic)) {
        toggle = 0;                                       // When ButtonTopic is set to 1, TOGGLE is not allowed but an HOLD trigger can be generated. 
      } 
    
    } else { button_present = 0; }
    
    HAssAnnouncerTriggers(button_index, button_present, 0, toggle, hold);    
  }
}

void HAssAnnounceSensor(const char *sensorname, const char *subsensortype, const char *MultiSubName, uint8_t subqty, uint8_t subidx)
{
  char stopic[TOPSZ];
  char stemp1[TOPSZ];
  char stemp2[TOPSZ];
  char unique_id[30];

  mqtt_data[0] = '\0'; // Clear retained message
  // Clear or Set topic
  snprintf_P(unique_id, sizeof(unique_id), PSTR("%06X_%s_%s"), ESP.getChipId(), sensorname, MultiSubName);
  snprintf_P(stopic, sizeof(stopic), PSTR(HOME_ASSISTANT_DISCOVERY_PREFIX "/sensor/%s/config"), unique_id);;

  if (Settings.flag.hass_discovery)
  {                     // SetOption19 - Control Home Assistantautomatic discovery (See SetOption59)
    char name[33 + 42]; // friendlyname(33) + " " + sensorname(20?) + " " + sensortype(20?)
    char prefix[TOPSZ];
    char *state_topic = stemp1;
    char *availability_topic = stemp2;

    GetTopic_P(state_topic, TELE, mqtt_topic, PSTR(D_RSLT_SENSOR));
    snprintf_P(name, sizeof(name), PSTR("%s %s %s"), SettingsText(SET_FRIENDLYNAME1), sensorname, MultiSubName);
    GetTopic_P(availability_topic, TELE, mqtt_topic, S_LWT);

    Response_P(HASS_DISCOVER_BASE, name, state_topic, availability_topic);
    TryResponseAppend_P(HASS_DISCOVER_DEVICE_INFO_SHORT, unique_id, ESP.getChipId());


    char jname[32];
    int sensor_index = GetCommandCode(jname, sizeof(jname), subsensortype, kHAssJsonSensorTypes);
    if (sensor_index > -1) {
      char param1[20];
      GetTextIndexed(param1, sizeof(param1), sensor_index, kHAssJsonSensorUnits);
      switch (sensor_index) {
        case 0:   // Temperature
          snprintf_P(param1, sizeof(param1), PSTR("°%c"),TempUnit()); // C or F
          break;
        case 1:   // Pressure
        case 2:
          snprintf_P(param1, sizeof(param1), PSTR("%s"), PressureUnit().c_str());
          break;
      }
      char param2[50];
      GetTextIndexed(param2, sizeof(param2), sensor_index, kHAssJsonSensorDevCla);
      TryResponseAppend_P(HASS_DISCOVER_SENSOR, param1, param2, sensorname, subsensortype);
      if (subidx) {
        TryResponseAppend_P(PSTR("[%d]"), subqty -1);
      }
    } else {
      TryResponseAppend_P(HASS_DISCOVER_SENSOR, " ", "ic\":\"mdi:eye", sensorname, subsensortype);
    }
    TryResponseAppend_P(PSTR("}}\"}"));
  }
  MqttPublish(stopic, true);
}

void HAssAnnounceSensors(void)
{
  uint8_t hass_xsns_index = 0;
  bool is_sensor = true;
  uint8_t subqty = 0;
  do
  {
    mqtt_data[0] = '\0';
    int tele_period_save = tele_period;
    tele_period = 2;                                 // Do not allow HA updates during next function call
    XsnsNextCall(FUNC_JSON_APPEND, hass_xsns_index); // ,"INA219":{"Voltage":4.494,"Current":0.020,"Power":0.089}
    tele_period = tele_period_save;

    char sensordata[512]; // Copy because we need to write to mqtt_data
    strlcpy(sensordata, mqtt_data, sizeof(sensordata));

    if (strlen(sensordata))
    {
      sensordata[0] = '{';
      snprintf_P(sensordata, sizeof(sensordata), PSTR("%s}"), sensordata); // {"INA219":{"Voltage":4.494,"Current":0.020,"Power":0.089}}
      // USE THE FOLLOWING LINE TO TEST JSON
      //snprintf_P(sensordata, sizeof(sensordata), PSTR("{\"HX711\":{\"Weight\":[22,34,1023.4], \"Battery\":25}}"));
      StaticJsonBuffer<500> jsonBuffer;
      JsonObject &root = jsonBuffer.parseObject(sensordata);
      if (!root.success())
      {
        AddLog_P2(LOG_LEVEL_ERROR, PSTR("HASS: jsonBuffer failed to parse '%s'"), sensordata);
        continue;
      }
      for (auto sensor : root)
      {
        const char *sensorname = sensor.key;
        JsonObject &sensors = sensor.value.as<JsonObject>();
        if (!sensors.success())
        {
          AddLog_P2(LOG_LEVEL_ERROR, PSTR("HASS: JsonObject failed to parse '%s'"), sensordata);
          continue;
        }
        for (auto subsensor : sensors)
        {
          // If there is more than a value on sensor data, 'n' entitites will be created
          if (subsensor.value.is<JsonArray&>()) {
            JsonArray& subsensors = subsensor.value.as<JsonArray&>();
            subqty = subsensors.size();
            char MultiSubName[20];
            for (int i = 1; i <= subqty; i++) {
              snprintf_P(MultiSubName, sizeof(MultiSubName), PSTR("%s_%d"), subsensor.key, i);
              HAssAnnounceSensor(sensorname, subsensor.key, MultiSubName, i, 1);

            }
          } else { HAssAnnounceSensor(sensorname, subsensor.key, subsensor.key, 0, 0);}
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
  mqtt_data[0] = '\0'; // Clear retained message

  // Clear or Set topic
  snprintf_P(unique_id, sizeof(unique_id), PSTR("%06X_status"), ESP.getChipId());
  snprintf_P(stopic, sizeof(stopic), PSTR(HOME_ASSISTANT_DISCOVERY_PREFIX "/sensor/%s/config"), unique_id);

  if (Settings.flag.hass_discovery)
  {                    // SetOption19 - Control Home Assistantautomatic discovery (See SetOption59)
    char name[33 + 7]; // friendlyname(33) + " " + "status"
    char prefix[TOPSZ];
    char *state_topic = stemp1;
    char *availability_topic = stemp2;

    snprintf_P(name, sizeof(name), PSTR("%s status"), SettingsText(SET_FRIENDLYNAME1));
    GetTopic_P(state_topic, TELE, mqtt_topic, PSTR(D_RSLT_HASS_STATE));
    GetTopic_P(availability_topic, TELE, mqtt_topic, S_LWT);

    Response_P(HASS_DISCOVER_BASE, name, state_topic, availability_topic);
    TryResponseAppend_P(HASS_DISCOVER_SENSOR_HASS_STATUS, state_topic);
    TryResponseAppend_P(HASS_DISCOVER_DEVICE_INFO, unique_id, ESP.getChipId(), SettingsText(SET_FRIENDLYNAME1),
                        ModuleName().c_str(), my_version, my_image);
    TryResponseAppend_P(PSTR("}"));
  }
  MqttPublish(stopic, true);
}

void HAssPublishStatus(void)
{
  Response_P(PSTR("{\"" D_JSON_VERSION "\":\"%s%s\",\"" D_JSON_BUILDDATETIME "\":\"%s\","
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
  if (Settings.flag.hass_discovery)
  {                                         // SetOption19 - Control Home Assistant automatic discovery (See SetOption59)
    Settings.flag.mqtt_response = 0;        // SetOption4  - Switch between MQTT RESULT or COMMAND - Response always as RESULT and not as uppercase command
    Settings.flag.decimal_text = 1;         // SetOption17 - Switch between decimal or hexadecimal output - Respond with decimal color values
    Settings.flag3.hass_tele_on_power = 1;  // SetOption59 - Send tele/%topic%/STATE in addition to stat/%topic%/RESULT - send tele/STATE message as stat/RESULT
                                            // the purpose of that is so that if HA is restarted, state in HA will be correct within one teleperiod otherwise state
                                            // will not be correct until the device state is changed this is why in the patterns for switch and light, we tell HA to trigger on STATE, not RESULT.
    Settings.light_scheme = 0;              // To just control color it needs to be Scheme 0
  }

  if (Settings.flag.hass_discovery || (1 == hass_mode))
  { // SetOption19 - Control Home Assistantautomatic discovery (See SetOption59)
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
  hass_mode = 1;      // Force discovery
  hass_init_step = 1; // Delayed discovery
}

void HAssAnyKey(void)
{
  if (!Settings.flag.hass_discovery)
  {
    return;
  } // SetOption19 - Control Home Assistantautomatic discovery (See SetOption59)

  uint32_t key = (XdrvMailbox.payload >> 16) & 0xFF;   // 0 = Button, 1 = Switch
  uint32_t device = XdrvMailbox.payload & 0xFF;        // Device number or 1 if more Buttons than Devices
  uint32_t state = (XdrvMailbox.payload >> 8) & 0xFF;  // 0 = Off, 1 = On, 2 = Toggle, 3 = Hold

  if (!key && KeyTopicActive(0)) {                     // Button and ButtonTopic is active
    device = (XdrvMailbox.payload >> 24) & 0xFF;       // Button number
  }

  char scommand[CMDSZ];
  char sw_topic[TOPSZ];
  char key_topic[TOPSZ];
  char *tmpbtn = SettingsText(SET_MQTT_BUTTON_TOPIC);
  char *tmpsw = SettingsText(SET_MQTT_SWITCH_TOPIC);
  uint8_t evkey = 0; // Flag to select the correct topic for a trigger or a binary_sensor
  Format(sw_topic, tmpsw, sizeof(sw_topic));
  Format(key_topic, tmpbtn, sizeof(key_topic));

  if (state == 2 || state == 3 ) { evkey = 1;}
  snprintf_P(scommand, sizeof(scommand), PSTR("%s%d%s"), (key) ? "SWITCH" : "BUTTON", device, (evkey) ? "T" : "");

  char stopic[TOPSZ];

  GetTopic_P(stopic, STAT, mqtt_topic, scommand);
  Response_P(S_JSON_COMMAND_SVALUE, (evkey) ? "TRIG" : PSTR(D_RSLT_STATE), GetStateText(state));
  MqttPublish(stopic);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv12(uint8_t function)
{
  bool result = false;

  if (Settings.flag.mqtt_enabled)
  { // SetOption3 - Enable MQTT
    switch (function)
    {
    case FUNC_EVERY_SECOND:
      if (hass_init_step)
      {
        hass_init_step--;
        if (!hass_init_step)
        {
          HAssDiscovery(); // Scheduled discovery using available resources
        }
      }
      else if (Settings.flag.hass_discovery && Settings.tele_period)
      { // SetOption19 - Control Home Assistantautomatic discovery (See SetOption59)
        hass_tele_period++;
        if (hass_tele_period >= Settings.tele_period)
        {
          hass_tele_period = 0;

          mqtt_data[0] = '\0';
          HAssPublishStatus();
        }
      }
      break;
    case FUNC_ANY_KEY:
      HAssAnyKey();
      break;
    case FUNC_MQTT_INIT:
      hass_mode = 0;      // Discovery only if Settings.flag.hass_discovery is set
      hass_init_step = 2; // Delayed discovery
      break;
    }
  }
  return result;
}

#endif // USE_HOME_ASSISTANT