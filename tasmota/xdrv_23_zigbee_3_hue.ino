/*
  xdrv_23_zigbee.ino - zigbee support for Tasmota

  Copyright (C) 2020  Theo Arends and Stephan Hadinger

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

#ifdef USE_ZIGBEE
#if defined(USE_WEBSERVER) && defined(USE_EMULATION) && defined(USE_EMULATION_HUE) && defined(USE_LIGHT)

// Add global functions for Hue Emulation

// idx: index in the list of zigbee_devices
void HueLightStatus1Zigbee(uint16_t shortaddr, uint8_t local_light_subtype, String *response) {
  uint8_t  power, colormode, bri, sat;
  uint16_t ct, hue;
  uint16_t x, y;
  String light_status = "";
  uint32_t echo_gen = findEchoGeneration();   // 1 for 1st gen =+ Echo Dot 2nd gen, 2 for 2nd gen and above

  zigbee_devices.getHueState(shortaddr, &power, &colormode, &bri, &sat, &ct, &hue, &x, &y);

  if (bri > 254)  bri = 254;    // Philips Hue bri is between 1 and 254
  if (bri < 1)    bri = 1;
  if (sat > 254)  sat = 254;  // Philips Hue only accepts 254 as max hue
  uint8_t hue8 = changeUIntScale(hue, 0, 360, 0, 254);    // default hue is 0..254, we don't use extended hue

  const size_t buf_size = 256;
  char * buf = (char*) malloc(buf_size);     // temp buffer for strings, avoid stack

  snprintf_P(buf, buf_size, PSTR("{\"on\":%s,"), (power & 1) ? "true" : "false");
  // Brightness for all devices with PWM
  if ((1 == echo_gen) || (LST_SINGLE <= local_light_subtype)) { // force dimmer for 1st gen Echo
    snprintf_P(buf, buf_size, PSTR("%s\"bri\":%d,"), buf, bri);
  }
  if (LST_COLDWARM <= local_light_subtype) {
    snprintf_P(buf, buf_size, PSTR("%s\"colormode\":\"%s\","), buf, (0 == colormode) ? "hs" : (1 == colormode) ? "xy" : "ct");
  }
  if (LST_RGB <= local_light_subtype) {  // colors
    if (prev_x_str[0] && prev_y_str[0]) {
      snprintf_P(buf, buf_size, PSTR("%s\"xy\":[%s,%s],"), buf, prev_x_str, prev_y_str);
    } else {
      float x_f = x / 65536.0f;
      float y_f = y / 65536.0f;
      snprintf_P(buf, buf_size, PSTR("%s\"xy\":[%s,%s],"), buf, String(x, 5).c_str(), String(y, 5).c_str());
    }
    snprintf_P(buf, buf_size, PSTR("%s\"hue\":%d,\"sat\":%d,"), buf, hue, sat);
  }
  if (LST_COLDWARM == local_light_subtype || LST_RGBW <= local_light_subtype) {  // white temp
    snprintf_P(buf, buf_size, PSTR("%s\"ct\":%d,"), buf, ct > 0 ? ct : 284);
  }
  snprintf_P(buf, buf_size, HUE_LIGHTS_STATUS_JSON1_SUFFIX, buf);

  *response += buf;
  free(buf);
}

void HueLightStatus2Zigbee(uint16_t shortaddr, String *response)
{
  const size_t buf_size = 192;
  char * buf = (char*) malloc(buf_size);

  const char * friendlyName = zigbee_devices.getFriendlyName(shortaddr);
  char shortaddrname[8];
  snprintf_P(shortaddrname, sizeof(shortaddrname), PSTR("0x%04X"), shortaddr);

  snprintf_P(buf, buf_size, HUE_LIGHTS_STATUS_JSON2,
              (friendlyName) ? friendlyName : shortaddrname,
              GetHueDeviceId(shortaddr).c_str());
  *response += buf;
  free(buf);
}

void ZigbeeHueStatus(String * response, uint16_t shortaddr) {
  *response += F("{\"state\":");
  HueLightStatus1Zigbee(shortaddr, zigbee_devices.getHueBulbtype(shortaddr), response);
  HueLightStatus2Zigbee(shortaddr, response);
}

void ZigbeeCheckHue(String * response, bool &appending) {
  uint32_t zigbee_num = zigbee_devices.devicesSize();
  for (uint32_t i = 0; i < zigbee_num; i++) {
    int8_t bulbtype = zigbee_devices.devicesAt(i).bulbtype;

    if (bulbtype >= 0) {
      uint16_t shortaddr = zigbee_devices.devicesAt(i).shortaddr;
      // this bulb is advertized
      if (appending) { *response += ","; }
      *response += "\"";
      *response += EncodeLightId(0, shortaddr);
      *response += F("\":{\"state\":");
      HueLightStatus1Zigbee(shortaddr, bulbtype, response);    // TODO
      HueLightStatus2Zigbee(shortaddr, response);
      appending = true;
    }
  }
}

void ZigbeeHueGroups(String * lights) {
  uint32_t zigbee_num = zigbee_devices.devicesSize();
  for (uint32_t i = 0; i < zigbee_num; i++) {
    int8_t bulbtype = zigbee_devices.devicesAt(i).bulbtype;

    if (bulbtype >= 0) {
      *lights += ",\"";
      *lights += EncodeLightId(i);
      *lights += "\"";
    }
  }
}

// Send commands
// Power On/Off
void ZigbeeHuePower(uint16_t shortaddr, uint8_t power) {
  zigbeeZCLSendStr(shortaddr, 0, 0, true, 0x0006, power, "");
  zigbee_devices.updateHueState(shortaddr, &power, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
}

// Dimmer
void ZigbeeHueDimmer(uint16_t shortaddr, uint8_t dimmer) {
  if (dimmer > 0xFE) { dimmer = 0xFE; }
  char param[8];
  snprintf_P(param, sizeof(param), PSTR("%02X0A00"), dimmer);
  zigbeeZCLSendStr(shortaddr, 0, 0, true, 0x0008, 0x04, param);
  zigbee_devices.updateHueState(shortaddr, nullptr, nullptr, &dimmer, nullptr, nullptr, nullptr, nullptr, nullptr);
}

// CT
void ZigbeeHueCT(uint16_t shortaddr, uint16_t ct) {
  if (ct > 0xFEFF) { ct = 0xFEFF; }
  AddLog_P2(LOG_LEVEL_INFO, PSTR("ZigbeeHueCT 0x%04X - %d"), shortaddr, ct);
  char param[12];
  snprintf_P(param, sizeof(param), PSTR("%02X%02X0A00"), ct & 0xFF, ct >> 8);
  uint8_t colormode = 2;      // "ct"
  zigbeeZCLSendStr(shortaddr, 0, 0, true, 0x0300, 0x0A, param);
  zigbee_devices.updateHueState(shortaddr, nullptr, &colormode, nullptr, nullptr, &ct, nullptr, nullptr, nullptr);
}

// XY
void ZigbeeHueXY(uint16_t shortaddr, uint16_t x, uint16_t y) {
  char param[16];
  if (x > 0xFEFF) { x = 0xFEFF; }
  if (y > 0xFEFF) { y = 0xFEFF; }
  snprintf_P(param, sizeof(param), PSTR("%02X%02X%02X%02X0A00"), x & 0xFF, x >> 8, y & 0xFF, y >> 8);
  uint8_t colormode = 1;      // "xy"
  zigbeeZCLSendStr(shortaddr, 0, 0, true, 0x0300, 0x07, param);
  zigbee_devices.updateHueState(shortaddr, nullptr, &colormode, nullptr, nullptr, nullptr, nullptr, &x, &y);
}

// HueSat
void ZigbeeHueHS(uint16_t shortaddr, uint16_t hue, uint8_t sat) {
  char param[16];
  uint8_t hue8 = changeUIntScale(hue, 0, 360, 0, 254);
  if (sat > 0xFE) { sat = 0xFE; }
  snprintf_P(param, sizeof(param), PSTR("%02X%02X0A00"), hue8, sat);
  uint8_t colormode = 0;      // "hs"
  zigbeeZCLSendStr(shortaddr, 0, 0, true, 0x0300, 0x06, param);
  zigbee_devices.updateHueState(shortaddr, nullptr, &colormode, nullptr, &sat, nullptr, &hue, nullptr, nullptr);
}

void ZigbeeHandleHue(uint16_t shortaddr, uint32_t device_id, String &response) {
  uint8_t  power, colormode, bri, sat;
  uint16_t ct, hue;
  float    x, y;
  int code = 200;

  bool resp = false;  // is the response non null (add comma between parameters)
  bool on = false;

  uint8_t bulbtype = zigbee_devices.getHueBulbtype(shortaddr);

  const size_t buf_size = 100;
  char * buf = (char*) malloc(buf_size);

  if (WebServer->args()) {
    response = "[";

    StaticJsonBuffer<300> jsonBuffer;
    JsonObject &hue_json = jsonBuffer.parseObject(WebServer->arg((WebServer->args())-1));
    if (hue_json.containsKey("on")) {
      on = hue_json["on"];
      snprintf_P(buf, buf_size,
                 PSTR("{\"success\":{\"/lights/%d/state/on\":%s}}"),
                 device_id, on ? "true" : "false");

      switch(on)
      {
        case false : ZigbeeHuePower(shortaddr, 0x00);
                    break;
        case true  : ZigbeeHuePower(shortaddr, 0x01);
                    break;
      }
      response += buf;
      resp = true;
    }

    if (hue_json.containsKey("bri")) {             // Brightness is a scale from 1 (the minimum the light is capable of) to 254 (the maximum). Note: a brightness of 1 is not off.
      bri = hue_json["bri"];
      prev_bri = bri;   // store command value
      if (resp) { response += ","; }
      snprintf_P(buf, buf_size,
                 PSTR("{\"success\":{\"/lights/%d/state/%s\":%d}}"),
                 device_id, "bri", bri);
      response += buf;
      if (LST_SINGLE <= bulbtype) {
        // extend bri value if set to max
        if (254 <= bri) { bri = 255; }
        ZigbeeHueDimmer(shortaddr, bri);
      }
      resp = true;
    }
    // handle xy before Hue/Sat
    // If the request contains both XY and HS, we wan't to give priority to HS
    if (hue_json.containsKey("xy")) {
      float x = hue_json["xy"][0];
      float y = hue_json["xy"][1];
      const String &x_str = hue_json["xy"][0];
      const String &y_str = hue_json["xy"][1];
      x_str.toCharArray(prev_x_str, sizeof(prev_x_str));
      y_str.toCharArray(prev_y_str, sizeof(prev_y_str));
      if (resp) { response += ","; }
      snprintf_P(buf, buf_size,
                 PSTR("{\"success\":{\"/lights/%d/state/xy\":[%s,%s]}}"),
                 device_id, prev_x_str, prev_y_str);
      response += buf;
      resp = true;
      uint16_t xi = x * 65536.0f;
      uint16_t yi = y * 65536.0f;
      ZigbeeHueXY(shortaddr, xi, yi);
    }
    bool huesat_changed = false;
    if (hue_json.containsKey("hue")) {             // The hue value is a wrapping value between 0 and 65535. Both 0 and 65535 are red, 25500 is green and 46920 is blue.
      hue = hue_json["hue"];
      prev_hue = hue;
      if (resp) { response += ","; }
      snprintf_P(buf, buf_size,
                 PSTR("{\"success\":{\"/lights/%d/state/%s\":%d}}"),
                 device_id, "hue", hue);
      response += buf;
      if (LST_RGB <= bulbtype) {
        // change range from 0..65535 to 0..359
        hue = changeUIntScale(hue, 0, 65535, 0, 359);
        huesat_changed = true;
      }
      resp = true;
    }
    if (hue_json.containsKey("sat")) {             // Saturation of the light. 254 is the most saturated (colored) and 0 is the least saturated (white).
      sat = hue_json["sat"];
      prev_sat = sat;   // store command value
      if (resp) { response += ","; }
      snprintf_P(buf, buf_size,
                 PSTR("{\"success\":{\"/lights/%d/state/%s\":%d}}"),
                 device_id, "sat", sat);
      response += buf;
      if (LST_RGB <= bulbtype) {
        // extend sat value if set to max
        if (254 <= sat) { sat = 255; }
        huesat_changed = true;
      }
      if (huesat_changed) {
        ZigbeeHueHS(shortaddr, hue, sat);
      }
      resp = true;
    }
    if (hue_json.containsKey("ct")) {  // Color temperature 153 (Cold) to 500 (Warm)
      ct = hue_json["ct"];
      prev_ct = ct;   // store commande value
      if (resp) { response += ","; }
      snprintf_P(buf, buf_size,
                 PSTR("{\"success\":{\"/lights/%d/state/%s\":%d}}"),
                 device_id, "ct", ct);
      response += buf;
      if ((LST_COLDWARM == bulbtype) || (LST_RGBW <= bulbtype)) {
        ZigbeeHueCT(shortaddr, ct);
      }
      resp = true;
    }

    response += "]";
    if (2 == response.length()) {
      response = FPSTR(HUE_ERROR_JSON);
    }
  }
  else {
    response = FPSTR(HUE_ERROR_JSON);
  }
  AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE " Result (%s)"), response.c_str());
  WSSend(code, CT_JSON, response);

  free(buf);
}

#endif  // USE_WEBSERVER && USE_EMULATION && USE_EMULATION_HUE
#endif  // USE_ZIGBEE
