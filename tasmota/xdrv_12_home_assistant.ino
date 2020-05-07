/*
  xdrv_12_home_assistant.ino - home assistant support for Tasmota

  Copyright (C) 2020  Erik Montnemery, Federico Leoni and Theo Arends

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
  D_JSON_TEMPERATURE "|" D_JSON_DEWPOINT "|" D_JSON_PRESSURE "|" D_JSON_PRESSUREATSEALEVEL "|"
  D_JSON_APPARENT_POWERUSAGE "|Battery|" D_JSON_CURRENT "|" D_JSON_DISTANCE "|" D_JSON_FREQUENCY "|" D_JSON_HUMIDITY "|" D_JSON_ILLUMINANCE "|"
  D_JSON_MOISTURE "|PB0.3|PB0.5|PB1|PB2.5|PB5|PB10|PM1|PM2.5|PM10|" D_JSON_POWERFACTOR "|" D_JSON_POWERUSAGE "|"
  D_JSON_REACTIVE_POWERUSAGE "|" D_JSON_TODAY "|" D_JSON_TOTAL "|" D_JSON_VOLTAGE "|" D_JSON_WEIGHT "|" D_JSON_YESTERDAY "|"
  D_JSON_CO2 "|" D_JSON_ECO2 "|" D_JSON_TVOC "|";

const char kHAssJsonSensorUnits[] PROGMEM =
  "||||"
  "VA|%|A|Cm|Hz|%|LX|"
  "%|ppd|ppd|ppd|ppd|ppd|ppd|µg/m³|µg/m³|µg/m³|Cos φ|W|"
  "VAr|kWh|kWh|V|Kg|kWh|"
  "ppm|ppm|ppb|";

const char kHAssJsonSensorDevCla[] PROGMEM =
  "dev_cla\":\"temperature|ic\":\"mdi:weather-rainy|dev_cla\":\"pressure|dev_cla\":\"pressure|"
  "dev_cla\":\"power|dev_cla\":\"battery|ic\":\"mdi:alpha-a-circle-outline|ic\":\"mdi:leak|ic\":\"mdi:current-ac|dev_cla\":\"humidity|dev_cla\":\"illuminance|"
  "ic\":\"mdi:cup-water|ic\":\"mdi:flask|ic\":\"mdi:flask|ic\":\"mdi:flask|ic\":\"mdi:flask|ic\":\"mdi:flask|ic\":\"mdi:flask|"
  "ic\":\"mdi:air-filter|ic\":\"mdi:air-filter|ic\":\"mdi:air-filter|ic\":\"mdi:alpha-f-circle-outline|dev_cla\":\"power|"
  "dev_cla\":\"power|dev_cla\":\"power|dev_cla\":\"power|ic\":\"mdi:alpha-v-circle-outline|ic\":\"mdi:scale|dev_cla\":\"power|"
  "ic\":\"mdi:periodic-table-co2|ic\":\"mdi:air-filter|ic\":\"mdi:periodic-table-co2|";
   //"ic\":\"mdi:weather-windy|ic\":\"mdi:weather-windy|ic\":\"mdi:weather-windy|ic\":\"mdi:weather-windy|"
// List of sensors ready for discovery

const char HASS_DISCOVER_SENSOR[] PROGMEM =
  ",\"unit_of_meas\":\"%s\",\"%s\","           // unit of measure and class (or icon)
  "\"frc_upd\":true,"                          // force update for better graph representation
  "\"val_tpl\":\"{{value_json['%s']['%s']";    // "COUNTER":{"C1":0} -> {{ value_json['COUNTER']['C1']

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

const char HASS_DISCOVER_BIN_PIR[] PROGMEM =
  ",\"val_tpl\":\"{{value_json.%s}}\","        // STATE
  "\"frc_upd\":true,"                          // In ON/OFF case, enable force_update to make automations work
  "\"pl_on\":\"%s\","                          // ON
  "\"off_dly\":1";                             // Switchmode13 and Switchmode14 doesn't transmit an OFF state.

const char HASS_DISCOVER_BASE_LIGHT[] PROGMEM =
  ",\"bri_cmd_t\":\"%s\","                     // cmnd/led2/Dimmer
  "\"bri_stat_t\":\"%s\","                     // stat/led2/RESULT
  "\"bri_scl\":100,"                           // 100%
  "\"on_cmd_type\":\"%s\","                    // power on (first), power on (last), no power on (brightness)
  "\"bri_val_tpl\":\"{{value_json.%s}}\"";

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

const char kHAssTriggerTypeButtons[] PROGMEM =
  "|button_short_press|button_double_press|button_triple_press|button_quadruple_press|button_quintuple_press|button_long_press|";

const char kHAssTriggerStringButtons[] PROGMEM =
  "|SINGLE|DOUBLE|TRIPLE|QUAD|PENTA|HOLD|";

const char kHAssError1[] PROGMEM =
  "HASS: MQTT discovery failed due to too long topic or friendly name."
  "Please shorten topic and friendly name. Failed to format";

const char kHAssError2[] PROGMEM =
  "HASS: The configuration of the Relays is wrong, there is a Light that is using an index lower than the number of the selected relay.\n               "
  "The Relays have priority over the Lights, an incorrect order could lead to an erroneous Light control.\n               "
  "Please update your configuration to avoid inconsistent results.\n               "
  "Entities for Relays and Lights will not be available in Home Assistant until the configuration will be updated.";

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
    AddLog_P2(LOG_LEVEL_ERROR, PSTR("%s (%u/%u):"), kHAssError1, dlen, slen);
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

  bool LightControl = light_controller.isCTRGBLinked(); // SetOption37 - Color remapping for led channels, also provides an option for allowing independent handling of RGB and white channels
  bool PwmMulti = Settings.flag3.pwm_multi_channels;    // SetOption68 - Multi-channel PWM instead of a single light
  bool is_topic_light = false;                          // Switch HAss domain between Lights and Relays
  bool ind_light = false;                               // Controls Separated Lights when SetOption37 is >= 128
  bool ct_light = false;                                // Controls a CT Light when SetOption37 is >= 128
  bool wt_light = false;                                // Controls a White Light when SetOption37 is >= 128
  bool err_flag = false;                                // When true it blocks the creation of entities if the order of the Relays is not correct to avoid issue with Lights

  uint8_t dimmer = 1;
  uint8_t max_lights = 1;

  // If there is a special Light to be enabled and managed with SetOption68 or SetOption37 >= 128, Discovery calculates the maximum number of entities to be generated in advance

  if (PwmMulti) { max_lights = Light.subtype; }

  if (!LightControl) {
    ind_light = 1;
    if (!PwmMulti) { max_lights = 2;}
  }

  // Lights types:  0 = LST_NONE / 1 = LST_SINGLE / 2 = LST_COLDWARM / 3 = LST_RGB / 4 = LST_RGBW / 5 = LST_RGBCW

  for (uint32_t i = 1; i <= MAX_RELAYS; i++)
  {
    bool RelayX = PinUsed(GPIO_REL1 +i-1);
    is_topic_light = Settings.flag.hass_light && RelayX || light_type && !RelayX; // SetOption30 - Enforce HAss autodiscovery as light

    mqtt_data[0] = '\0'; // Clear retained message

    // Clear "other" topic first in case the device has been reconfigured from light to switch or vice versa
    snprintf_P(unique_id, sizeof(unique_id), PSTR("%06X_%s_%d"), ESP_getChipId(), (is_topic_light) ? "RL" : "LI", i);
    snprintf_P(stopic, sizeof(stopic), PSTR(HOME_ASSISTANT_DISCOVERY_PREFIX "/%s/%s/config"),
               (is_topic_light) ? "switch" : "light", unique_id);
    MqttPublish(stopic, true);
    // Clear or Set topic
    snprintf_P(unique_id, sizeof(unique_id), PSTR("%06X_%s_%d"), ESP_getChipId(), (is_topic_light) ? "LI" : "RL", i);
    snprintf_P(stopic, sizeof(stopic), PSTR(HOME_ASSISTANT_DISCOVERY_PREFIX "/%s/%s/config"),
               (is_topic_light) ? "light" : "switch", unique_id);

    if ((i < Light.device) && !RelayX) {
      err_flag = true;
      AddLog_P2(LOG_LEVEL_ERROR, PSTR("%s"), kHAssError2);
    } else {
      if (Settings.flag.hass_discovery && (RelayX || (Light.device > 0) && (max_lights > 0)) && !err_flag )
      {                    // SetOption19 - Control Home Assistantautomatic discovery (See SetOption59)
          char name[33 + 2]; // friendlyname(33) + " " + index
          char value_template[33];
          char prefix[TOPSZ];
          char *command_topic = stemp1;
          char *state_topic = stemp2;
          char *availability_topic = stemp3;

          if (i > MAX_FRIENDLYNAMES) {
            snprintf_P(name, sizeof(name), PSTR("%s %d"), SettingsText(SET_FRIENDLYNAME1), i-1);
          } else {
            snprintf_P(name, sizeof(name), SettingsText(SET_FRIENDLYNAME1 + i-1));
          }

          GetPowerDevice(value_template, i, sizeof(value_template), Settings.flag.device_index_enable); // SetOption26 - Switch between POWER or POWER1
          GetTopic_P(command_topic, CMND, mqtt_topic, value_template);
          GetTopic_P(state_topic, TELE, mqtt_topic, D_RSLT_STATE);
          GetTopic_P(availability_topic, TELE, mqtt_topic, S_LWT);
          Response_P(HASS_DISCOVER_BASE, name, state_topic, availability_topic);
          TryResponseAppend_P(HASS_DISCOVER_RELAY, command_topic, value_template, SettingsText(SET_STATE_TXT1), SettingsText(SET_STATE_TXT2));
          TryResponseAppend_P(HASS_DISCOVER_DEVICE_INFO_SHORT, unique_id, ESP_getChipId());

  #ifdef USE_LIGHT
        if ((i >= Light.device)
  #ifdef ESP8266
          || PWM_DIMMER == my_module_type
  #endif
        )
        {
          if (!RelayX) {
            char *brightness_command_topic = stemp1;
            strncpy_P(stemp3, Settings.flag.not_power_linked ? PSTR("last") : PSTR("brightness"), sizeof(stemp3)); // SetOption20 - Control power in relation to Dimmer/Color/Ct changes
            char channel_num[9];
            if (PwmMulti) { // SetOption68 - Multi-channel PWM instead of a single light
              snprintf_P(channel_num, sizeof(channel_num), PSTR("Channel%d"), i);
            } else {
              if (!LightControl) { // SetOption37 >= 128 - Color remapping for led channels, also provides an option for allowing independent handling of RGB and white channels
                snprintf_P(channel_num, sizeof(channel_num), PSTR("" D_CMND_DIMMER "%d"), dimmer);
                dimmer ++;
              } else {
                snprintf_P(channel_num, sizeof(channel_num), PSTR("" D_CMND_DIMMER ""));
              }
            }
            GetTopic_P(brightness_command_topic, CMND, mqtt_topic, channel_num);
            TryResponseAppend_P(HASS_DISCOVER_BASE_LIGHT, brightness_command_topic, state_topic, stemp3, channel_num);
          }
          if ((ind_light && !PwmMulti) || LightControl) {

            if (Light.subtype >= LST_RGB) {
              char *rgb_command_topic = stemp1;

              GetTopic_P(rgb_command_topic, CMND, mqtt_topic, D_CMND_COLOR);
              TryResponseAppend_P(HASS_DISCOVER_LIGHT_COLOR, rgb_command_topic, state_topic);

              char *effect_command_topic = stemp1;
              GetTopic_P(effect_command_topic, CMND, mqtt_topic, D_CMND_SCHEME);
              TryResponseAppend_P(HASS_DISCOVER_LIGHT_SCHEME, effect_command_topic, state_topic);
            }
            if (LST_RGBW == Light.subtype) { wt_light = true; }
            if (LST_RGBCW == Light.subtype) { ct_light = true; }
          }

          if ((!ind_light && ct_light) || (LST_COLDWARM == Light.subtype &&
              !PwmMulti && LightControl)) {
              char *color_temp_command_topic = stemp1;

              GetTopic_P(color_temp_command_topic, CMND, mqtt_topic, D_CMND_COLORTEMPERATURE);
              TryResponseAppend_P(HASS_DISCOVER_LIGHT_CT, color_temp_command_topic, state_topic);
              ct_light = false;
          }
          if ((!ind_light && wt_light) || (LST_RGBW == Light.subtype &&
              !PwmMulti && LightControl)) {
              char *white_temp_command_topic = stemp1;

              GetTopic_P(white_temp_command_topic, CMND, mqtt_topic, D_CMND_WHITE);
              TryResponseAppend_P(HASS_DISCOVER_LIGHT_WHITE, white_temp_command_topic, state_topic);
              wt_light = false;
          }
          ind_light = false;
          max_lights--;
        }
  #endif  // USE_LIGHT
        TryResponseAppend_P(PSTR("}"));
      }
    }
    MqttPublish(stopic, true);
  }
}

void HAssAnnouncerTriggers(uint8_t device, uint8_t present, uint8_t key, uint8_t toggle, uint8_t hold, uint8_t single, uint8_t trg_start, uint8_t trg_end)
{
  // key 0 = button
  // key 1 = switch
  char stopic[TOPSZ];
  char stemp1[TOPSZ];
  char stemp2[TOPSZ];
  char unique_id[30];
  char trigger2[8];

  mqtt_data[0] = '\0'; // Clear retained message

  for (uint8_t i = trg_start; i <= trg_end; i++) {
    GetTextIndexed(trigger2, sizeof(trigger2), i, kHAssTriggerStringButtons);
    snprintf_P(unique_id, sizeof(unique_id), PSTR("%06X_%s_%d_%s"), ESP_getChipId(), key ? "SW" : "BTN", device + 1, key ? GetStateText(i) : trigger2);
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
      if (key) {
        if ((i == 2 && toggle != 0) || (i == 3 && hold != 0)) {
          if (i == 3) { pload = hold; }
          GetTextIndexed(param, sizeof(param), pload, kHAssTriggerType);
          snprintf_P(subtype, sizeof(subtype), PSTR("switch_%d"), device + 1);
          Response_P(HASS_TRIGGER_TYPE, state_topic, GetStateText(i), param, subtype, ESP_getChipId());
        } else { mqtt_data[0] = '\0'; } // Need to be cleaned again to avoid duplicate
      } else {
        char trigger1[24];
        GetTextIndexed(trigger1, sizeof(trigger1), i, kHAssTriggerTypeButtons);
        snprintf_P(subtype, sizeof(subtype), PSTR("button_%d"), device + 1);
        if (i > 1 && single) {
          mqtt_data[0] = '\0';  // Need to be cleaned again to avoid duplicate
        } else {
          Response_P(HASS_TRIGGER_TYPE, state_topic, trigger2, trigger1, subtype, ESP_getChipId());
        }
      }
    }
    MqttPublish(stopic, true);
  }
}

void HAssAnnouncerBinSensors(uint8_t device, uint8_t present, uint8_t dual, uint8_t toggle, uint8_t pir)
{
  char stopic[TOPSZ];
  char stemp1[TOPSZ];
  char stemp2[TOPSZ];
  char unique_id[30];

  mqtt_data[0] = '\0'; // Clear retained message

  snprintf_P(unique_id, sizeof(unique_id), PSTR("%06X_SW_%d"), ESP_getChipId(), device + 1);
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

      snprintf_P(name, sizeof(name), PSTR("%s Switch%d"), ModuleName().c_str(), device + 1);
      Response_P(HASS_DISCOVER_BASE, name, state_topic, availability_topic);
      if (!pir) {
        TryResponseAppend_P(HASS_DISCOVER_BIN_SWITCH, PSTR(D_RSLT_STATE), SettingsText(SET_STATE_TXT2), SettingsText(SET_STATE_TXT1));
      } else {
        TryResponseAppend_P(HASS_DISCOVER_BIN_PIR, PSTR(D_RSLT_STATE), SettingsText(SET_STATE_TXT2));
      }
      TryResponseAppend_P(HASS_DISCOVER_DEVICE_INFO_SHORT, unique_id, ESP_getChipId());
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
    uint8_t pir = 0;

    if (PinUsed(GPIO_SWT1, switch_index)) { switch_present = 1; }

    if (KeyTopicActive(1) && strcmp(SettingsText(SET_MQTT_SWITCH_TOPIC), mqtt_topic))   // Enable Discovery for Switches only if SwitchTopic is set to a custom name
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
    // 13             PUSHON                YES           NONE                          NONE                        NONE                        0,0
    // 14             PUSHON_INV            YES           NONE                          NONE                        NONE                        0,0
    // Please note: SwitchMode11 and 12 will register just TOGGLE (button_short_press)

    // Trigger types: "0 = none | 1 = button_short_press | 2 = button_long_press | 3 = button_double_press";

      uint8_t swmode = Settings.switchmode[switch_index];

      switch (swmode) {
        case FOLLOW:
        case FOLLOW_INV:
          toggle = 0;     // Binary sensor and no triggers
          break;
        case PUSHBUTTON:
        case PUSHBUTTON_INV:
          dual = 1;       // Binary sensor and TOGGLE (button_short_press) trigger
          break;
        case PUSHBUTTONHOLD:
        case PUSHBUTTONHOLD_INV:
          dual = 1;       // Binary sensor, TOGGLE (button_short_press) and HOLD (button_long_press) triggers
          hold = 2;
          break;
        case TOGGLEMULTI:
          hold = 3;       // TOGGLE (button_short_press) and HOLD (button_double_press) triggers
          break;
        case FOLLOWMULTI:
        case FOLLOWMULTI_INV:
          dual = 1;       // Binary sensor and HOLD (button_long_press) trigger
          toggle = 0;
          hold = 3;
          break;
        case PUSHON:
        case PUSHON_INV:
          toggle = 0;
          pir = 1;        // Binary sensor with only ON state and automatic OFF after 1 second
      }

    } else { switch_present = 0;}

    HAssAnnouncerTriggers(switch_index, switch_present, 1, toggle, hold, 0, 2, 3);
    HAssAnnouncerBinSensors(switch_index, switch_present, dual, toggle, pir);
  }
}

void HAssAnnounceButtons(void)
{
  for (uint32_t button_index = 0; button_index < MAX_KEYS; button_index++)
  {
    uint8_t button_present = 0;
    uint8_t single = 0;

#ifdef ESP8266
    if (!button_index && ((SONOFF_DUAL == my_module_type) || (CH4 == my_module_type)))
    {
      button_present = 1;
    } else
#endif
    {
      if (PinUsed(GPIO_KEY1, button_index)) {
        button_present = 1;
      }
    }

    // Button matrix for triggers generation when SetOption73 is enabled:
    // N  SetOption1  SetOption11 SetOption13 PRESS                               MULTI PRESS                     HOLD
    // 1  0           0           0           SINGLE (10 - button_short_press)    DOUBLE to PENTA                 YES (button_long_press)
    // 2  1           0           0           SINGLE (10 - button_short_press)    DOUBLE to PENTA                 YES (button_long_press)
    // 3  0           1           0           DOUBLE (11 - button_short_press)    SINGLE then TRIPLE TO PENTA     YES (button_long_press)
    // 4  1           1           0           DOUBLE (11 - button_short_press)    SINGLE then TRIPLE TO PENTA     YES (button_long_press)
    // 5  0           0           1           SINGLE (10 - button_short_press)    NONE                            NONE
    // 6  1           0           1           SINGLE (10 - button_short_press)    NONE                            NONE
    // 7  0           1           1           SINGLE (10 - button_short_press)    NONE                            NONE
    // 8  1           1           1           SINGLE (10 - button_short_press)    NONE                            NONE

    // Trigger types:  10 = button_short_press | 11 = button_double_press | 12 = button_triple_press | 13 = button_quadruple_press | 14 = button_quintuple_press | 3 = button_long_press

    if (!Settings.flag3.mqtt_buttons) {        // Enable Buttons for discovery [SetOption73] - Decouple button from relay and send just mqtt topic
      button_present = 0;
    } else {
      if (Settings.flag.button_single) {       // [SetOption13] Immediate action on button press, just SINGLE trigger
        single = 1;
        }
    }

    HAssAnnouncerTriggers(button_index, button_present, 0, 0, 0, single, 1, 6);
  }
}

void HAssAnnounceSensor(const char *sensorname, const char *subsensortype, const char *MultiSubName, uint8_t subqty, uint8_t subidx, uint8_t nested, const char* SubKey)
{
  char stopic[TOPSZ];
  char stemp1[TOPSZ];
  char stemp2[TOPSZ];
  char unique_id[30];
  char subname[20];

  mqtt_data[0] = '\0'; // Clear retained message

  // Clear or Set topic
  NoAlNumToUnderscore(subname, MultiSubName); //Replace all non alphaumeric characters to '_' to avoid topic name issues
  snprintf_P(unique_id, sizeof(unique_id), PSTR("%06X_%s_%s"), ESP_getChipId(), sensorname, subname);
  snprintf_P(stopic, sizeof(stopic), PSTR(HOME_ASSISTANT_DISCOVERY_PREFIX "/sensor/%s/config"), unique_id);

  if (Settings.flag.hass_discovery)
  {                     // SetOption19 - Control Home Assistantautomatic discovery (See SetOption59)
    char name[33 + 42]; // friendlyname(33) + " " + sensorname(20?) + " " + sensortype(20?)
    char prefix[TOPSZ];
    char *state_topic = stemp1;
    char *availability_topic = stemp2;

    GetTopic_P(state_topic, TELE, mqtt_topic, PSTR(D_RSLT_SENSOR));
    snprintf_P(name, sizeof(name), PSTR("%s %s %s"), ModuleName().c_str(), sensorname, MultiSubName);
    GetTopic_P(availability_topic, TELE, mqtt_topic, S_LWT);

    Response_P(HASS_DISCOVER_BASE, name, state_topic, availability_topic);
    TryResponseAppend_P(HASS_DISCOVER_DEVICE_INFO_SHORT, unique_id, ESP_getChipId());


    char jname[32];
    int sensor_index = GetCommandCode(jname, sizeof(jname), SubKey, kHAssJsonSensorTypes);
    if (sensor_index > -1) {

      char param1[20];
      GetTextIndexed(param1, sizeof(param1), sensor_index, kHAssJsonSensorUnits);
      switch (sensor_index) {
        case 0:   // Temperature and DewPoint
        case 1:
          snprintf_P(param1, sizeof(param1), PSTR("°%c"),TempUnit()); // C or F
          break;
        case 2:   // Pressure and Sea Level Pressure
        case 3:
          snprintf_P(param1, sizeof(param1), PSTR("%s"), PressureUnit().c_str());
          break;
        // case 4:   // Speed. Default to km/h if not set to have a graph representation under HAss
        // case 5:
        // case 6:
        // case 7:
        //   if (Settings.flag2.speed_conversion == 0) {
        //     snprintf_P(param1, sizeof(param1), PSTR("km/h"));
        //   } else {
        //     snprintf_P(param1, sizeof(param1), PSTR("%s"), SpeedUnit().c_str());
        //   }
        //   break;
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
    if (nested) {
      TryResponseAppend_P(PSTR("['%s']"), SubKey);
    }
    TryResponseAppend_P(PSTR("}}\"}"));
  }
  MqttPublish(stopic, true);
}

void HAssAnnounceSensors(void)
{
  uint8_t hass_xsns_index = 0;
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
      //snprintf_P(sensordata, sizeof(sensordata), PSTR("{\"HX711\":{\"Weight\":[22,34,1023.4]}}"));

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
          if (subsensor.value.is<JsonObject&>()) {
            // If there is a nested json on sensor data, second level entitites will be created
            char NestedName[20];
            char NewSensorName[20];
            snprintf_P(NestedName, sizeof(NestedName), PSTR("%s"), subsensor.key);
            JsonObject& subsensors = subsensor.value.as<JsonObject>();
            for (auto subsensor : subsensors) {
              snprintf_P(NewSensorName, sizeof(NewSensorName), PSTR("%s %s"), NestedName, subsensor.key);
              HAssAnnounceSensor(sensorname, NestedName, NewSensorName, 0, 0, 1, subsensor.key);
            }
          } else if (subsensor.value.is<JsonArray&>()) {
            // If there is more than a value on sensor data, 'n' entitites will be created
            JsonArray& subsensors = subsensor.value.as<JsonArray&>();
            uint8_t subqty = subsensors.size();
            char MultiSubName[20];
            for (int i = 1; i <= subqty; i++) {
              snprintf_P(MultiSubName, sizeof(MultiSubName), PSTR("%s %d"), subsensor.key, i);
              HAssAnnounceSensor(sensorname, subsensor.key, MultiSubName, i, 1, 0, subsensor.key);
            }
          } else { HAssAnnounceSensor(sensorname, subsensor.key, subsensor.key, 0, 0, 0, subsensor.key);}
        }
      }
    }
    yield();
  } while (hass_xsns_index != 0);
}

void HAssAnnounceDeviceInfoAndStatusSensor(void)
{
  char stopic[TOPSZ];
  char stemp1[TOPSZ];
  char stemp2[TOPSZ];
  char unique_id[30];

  // Announce sensor
  mqtt_data[0] = '\0'; // Clear retained message

  // Clear or Set topic
  snprintf_P(unique_id, sizeof(unique_id), PSTR("%06X_status"), ESP_getChipId());
  snprintf_P(stopic, sizeof(stopic), PSTR(HOME_ASSISTANT_DISCOVERY_PREFIX "/sensor/%s/config"), unique_id);

  if (Settings.flag.hass_discovery)
  {                    // SetOption19 - Control Home Assistantautomatic discovery (See SetOption59)
    char name[33 + 7]; // friendlyname(33) + " " + "status"
    char prefix[TOPSZ];
    char *state_topic = stemp1;
    char *availability_topic = stemp2;

    snprintf_P(name, sizeof(name), PSTR("%s status"), ModuleName().c_str());
    GetTopic_P(state_topic, TELE, mqtt_topic, PSTR(D_RSLT_HASS_STATE));
    GetTopic_P(availability_topic, TELE, mqtt_topic, S_LWT);

    Response_P(HASS_DISCOVER_BASE, name, state_topic, availability_topic);
    TryResponseAppend_P(HASS_DISCOVER_SENSOR_HASS_STATUS, state_topic);
    TryResponseAppend_P(HASS_DISCOVER_DEVICE_INFO, unique_id, ESP_getChipId(), ModuleName().c_str(),
                        ModuleName().c_str(), my_version, my_image);
    TryResponseAppend_P(PSTR("}"));
  }
  MqttPublish(stopic, true);
}

void HAssPublishStatus(void)
{
  Response_P(PSTR("{\"" D_JSON_VERSION "\":\"%s%s\",\"" D_JSON_BUILDDATETIME "\":\"%s\",\"" D_CMND_MODULE " or " D_CMND_TEMPLATE"\":\"%s\","
                  "\"" D_JSON_RESTARTREASON "\":\"%s\",\"" D_JSON_UPTIME "\":\"%s\",\"" D_CMND_HOSTNAME "\":\"%s\","
                  "\"" D_CMND_IPADDRESS "\":\"%s\",\"" D_JSON_RSSI "\":\"%d\",\"" D_JSON_SIGNAL " (dBm)""\":\"%d\","
                  "\"WiFi " D_JSON_LINK_COUNT "\":%d,\"WiFi " D_JSON_DOWNTIME "\":\"%s\",\"" D_JSON_MQTT_COUNT "\":%d,\"LoadAvg\":%lu}"),
             my_version, my_image, GetBuildDateAndTime().c_str(), ModuleName().c_str(), GetResetReason().c_str(),
             GetUptime().c_str(), my_hostname, WiFi.localIP().toString().c_str(), WifiGetRssiAsQuality(WiFi.RSSI()),
             WiFi.RSSI(), WifiLinkCount(), WifiDowntime().c_str(), MqttConnectCount(), loop_load_avg);
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

    // Send info about buttons
    HAssAnnounceButtons();

    // Send info about switches
    HAssAnnounceSwitches();

    // Send info about sensors
    HAssAnnounceSensors();

    // Send info about relays and lights
    HAssAnnounceRelayLight();

    // Send info about status sensor
    HAssAnnounceDeviceInfoAndStatusSensor();
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
  uint32_t state = (XdrvMailbox.payload >> 8) & 0xFF;  // 0 = Off, 1 = On, 2 = Toggle, 3 = Hold, 10,11,12,13 and 14 for Button Multipress

  if (!key && KeyTopicActive(0)) {                     // Button and ButtonTopic is active
    device = (XdrvMailbox.payload >> 24) & 0xFF;       // Button number
  }

  char scommand[CMDSZ];
  char sw_topic[TOPSZ];
  char key_topic[TOPSZ];
  char trg_state[8];
  char *tmpbtn = SettingsText(SET_MQTT_BUTTON_TOPIC);
  char *tmpsw = SettingsText(SET_MQTT_SWITCH_TOPIC);
  uint8_t evkey = 0; // Flag to select the correct topic for a trigger or a binary_sensor
  Format(sw_topic, tmpsw, sizeof(sw_topic));
  Format(key_topic, tmpbtn, sizeof(key_topic));

  if (state >= 2) { evkey = 1;}
  snprintf_P(scommand, sizeof(scommand), PSTR("%s%d%s"), (key) ? "SWITCH" : "BUTTON", device, (evkey) ? "T" : "");

  char stopic[TOPSZ];

  if (state == 2) {
    snprintf_P(trg_state, sizeof(trg_state), PSTR("SINGLE"));
  } else if (state == 3) {
    snprintf_P(trg_state, sizeof(trg_state), GetStateText(3));
  } else {
    GetTextIndexed(trg_state, sizeof(trg_state), state -9, kHAssTriggerStringButtons);
  }

  GetTopic_P(stopic, STAT, mqtt_topic, scommand);
  Response_P(S_JSON_COMMAND_SVALUE, (evkey) ? "TRIG" : PSTR(D_RSLT_STATE), (key) ? GetStateText(state) : trg_state);
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
