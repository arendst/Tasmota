/*
  xdrv_52_3_berry_matter.ino - Berry support for Matter UI

  Copyright (C) 2021 Stephan Hadinger, Berry language by Guan Wenliang https://github.com/Skiars/berry

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

// Mappgin from internal light and a generic `light_state` Berry class

#ifdef USE_BERRY

// Convert seconds to a string representing days, hours or minutes present in the n-value.
// The string will contain the most coarse time only, rounded down (61m == 01h, 01h37m == 01h).
// Inputs:
// - seconds: uint32_t representing some number of seconds
// Outputs:
// - char for unit (d for day, h for hour, m for minute)
// - the hex color to be used to display the text
//
extern "C" uint32_t matter_convert_seconds_to_dhm(uint32_t seconds,  char *unit, uint32_t *color, bbool days) {
  *color = WebColor(COL_TEXT);
  static const uint32_t conversions[3] = {24 * 3600, 3600, 60};
  static const char     units[3] = { 'd', 'h', 'm'};   // day, hour, minute
  static const uint32_t color_threshold_hours[2] = {24 * 3600, 3600};               // 0 - 1 hour - 1 day
  static const uint32_t color_threshold_days[2] = {7 * 24 * 3600, 2 * 24 * 3600};    // 0 - 2 days - 7 days

  uint32_t color_text_8 = WebColor(COL_TEXT);    // color of text on 8 bits
  uint8_t color_text_8_r = (color_text_8 & 0xFF0000) >> 16;
  uint8_t color_text_8_g = (color_text_8 & 0x00FF00) >> 8;
  uint8_t color_text_8_b = (color_text_8 & 0x0000FF);

  uint32_t color_back_8 = WebColor(COL_BACKGROUND);    // color of background on 8 bits
  uint8_t color_back_8_r = (color_back_8 & 0xFF0000) >> 16;
  uint8_t color_back_8_g = (color_back_8 & 0x00FF00) >> 8;
  uint8_t color_back_8_b = (color_back_8 & 0x0000FF);

  int32_t colors[3] = {
    ((changeUIntScale( 6, 0, 16, color_back_8_r, color_text_8_r) & 0xFF) << 16U) |   //  6/16 of text
    ((changeUIntScale( 6, 0, 16, color_back_8_g, color_text_8_g) & 0xFF) <<  8U) |   //  6/16 of text
    ( changeUIntScale( 6, 0, 16, color_back_8_b, color_text_8_r) & 0xFF),            //  6/16 of text

    ((changeUIntScale(10, 0, 16, color_back_8_r, color_text_8_r) & 0xFF) << 16U) |   // 10/16 of text
    ((changeUIntScale(10, 0, 16, color_back_8_g, color_text_8_g) & 0xFF) <<  8U) |   // 10/16 of text
    ( changeUIntScale(10, 0, 16, color_back_8_b, color_text_8_r) & 0xFF),            // 10/16 of text

    (color_text_8_r << 16U) |
    (color_text_8_g <<  8U) |
    (color_text_8_b)
  };

  *color = (uint32_t)colors[2];
  for (uint32_t i = 0; i < 2; i++) {
    if (seconds > (days ? color_threshold_days[i] : color_threshold_hours[i])) {
      *color = (uint32_t)colors[i];
      break;
    }
  }
  for(uint32_t i = 0; i < 3; ++i) {
    *unit = units[i];
    if (seconds > conversions[i]) {    // always pass even if 00m
      return seconds / conversions[i];
    }
  }
  return 0;
}


//
// internal function to publish a command received from the Matter controller
//
// `matter.publish_command("MtrReceived", 2, "buld", '"Power":1')`
// matter_publish_command(const char * json_prefix, uint32_t ep, const char * friendly_name, const char * payload) {
extern "C" int matter_publish_command(bvm *vm) {
  int32_t argc = be_top(vm); // Get the number of arguments
  if (argc >= 4 && be_isstring(vm, 1) && be_isint(vm, 2) && be_isstring(vm, 3) && be_isstring(vm, 4)) {
    const char * json_prefix = be_tostring(vm, 1);
    uint32_t ep = be_toint(vm, 2);
    const char * friendly_name = be_tostring(vm, 3);
    const char * payload = be_tostring(vm, 4);
    be_pop(vm, be_top(vm));   // avoid `Error be_top is non zero`

    bool use_fname = (Settings->flag4.zigbee_use_names) && (friendly_name && strlen(friendly_name));    // should we replace shortaddr with friendlyname?

    ResponseClear(); // clear string

    // Do we prefix with `ZbReceived`?
    if (!Settings->flag4.remove_zbreceived && !Settings->flag5.zb_received_as_subtopic) {
      if (Settings->flag5.zigbee_include_time && Rtc.utc_time >= START_VALID_TIME) {
        // Add time if needed (and if time is valide)
        ResponseAppendTimeFormat(Settings->flag2.time_format);
        ResponseAppend_P(PSTR(",\"%s\":"), json_prefix);
      } else {
        ResponseAppend_P(PSTR("{\"%s\":"), json_prefix);
      }
    }

    // What key do we use, shortaddr or name?
    if (!Settings->flag5.zb_omit_json_addr) {
      if (use_fname) {
        ResponseAppend_P(PSTR("{\"%s\":"), friendly_name);
      } else {
        ResponseAppend_P(PSTR("{\"%i\":"), ep);
      }
    }
    ResponseAppend_P(PSTR("{"));

    // Add "Ep":<ep>
    ResponseAppend_P(PSTR("\"" "Ep" "\":%i,"), ep);

    // Add "Name":"xxx" if name is present
    if (friendly_name && strlen(friendly_name)) {
      ResponseAppend_P(PSTR("\"" "Name" "\":\"%s\","), EscapeJSONString(friendly_name).c_str());
    }
    // Add all other attributes
    ResponseAppend_P(PSTR("%s}"), payload);

    if (!Settings->flag5.zb_omit_json_addr) {
      ResponseAppend_P(PSTR("}"));
    }

    if (!Settings->flag4.remove_zbreceived && !Settings->flag5.zb_received_as_subtopic) {
      ResponseAppend_P(PSTR("}"));
    }

  #ifdef USE_INFLUXDB
    InfluxDbProcess(1);        // Use a copy of ResponseData
  #endif

    if (Settings->flag4.zigbee_distinct_topics) {
      char subtopic[TOPSZ];
      if (Settings->flag4.zb_topic_fname && friendly_name && strlen(friendly_name)) {
        // Clean special characters
        char stemp[TOPSZ];
        strlcpy(stemp, friendly_name, sizeof(stemp));
        MakeValidMqtt(0, stemp);
        if (Settings->flag5.zigbee_hide_bridge_topic) {
          snprintf_P(subtopic, sizeof(subtopic), PSTR("%s"), stemp);
        } else {
          snprintf_P(subtopic, sizeof(subtopic), PSTR("%s/%s"), TasmotaGlobal.mqtt_topic, stemp);
        }
      } else {
        if (Settings->flag5.zigbee_hide_bridge_topic) {
          snprintf_P(subtopic, sizeof(subtopic), PSTR("%i"), ep);
        } else {
          snprintf_P(subtopic, sizeof(subtopic), PSTR("%s/%i"), TasmotaGlobal.mqtt_topic, ep);
        }
      }
      char stopic[TOPSZ];
      if (Settings->flag5.zb_received_as_subtopic) {
        GetTopic_P(stopic, STAT, subtopic, json_prefix);
      } else {
        GetTopic_P(stopic, STAT, subtopic, PSTR(D_RSLT_COMMAND));
      }
      MqttPublish(stopic, Settings->flag.mqtt_sensor_retain);
    } else {
      MqttPublishPrefixTopic_P(STAT, PSTR(D_RSLT_COMMAND), Settings->flag.mqtt_sensor_retain);
    }
    XdrvRulesProcess(0);     // apply rules
    be_return_nil(vm);
  }
  be_raise(vm, kTypeError, nullptr);
}

#endif  // USE_BERRY
