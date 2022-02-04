/*
  xdrv_23_zigbee.ino - zigbee support for Tasmota

  Copyright (C) 2021  Theo Arends and Stephan Hadinger

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
#if defined(USE_WEBSERVER) && defined(USE_EMULATION) && defined(USE_EMULATION_HUE)

// Add global functions for Hue Emulation

// idx: index in the list of zigbee_devices
void HueLightStatus1Zigbee(uint16_t shortaddr, uint8_t local_light_subtype, String *response) {
  static const char HUE_LIGHTS_STATUS_JSON1_SUFFIX_ZIGBEE[] PROGMEM =
    "%s\"alert\":\"none\","
    "\"effect\":\"none\","
    "\"reachable\":%s}";

  bool      power = false;
  bool      reachable = false;
  uint8_t   colormode = 0xFF;
  uint8_t   bri = 0xFF;
  uint8_t   sat = 0xFF;
  uint16_t  ct = 0xFFFF;
  uint16_t  hue = 0xFFFF;
  uint16_t  x = 0xFFFF, y = 0xFFFF;
  String light_status = "";
  uint32_t echo_gen = findEchoGeneration();   // 1 for 1st gen =+ Echo Dot 2nd gen, 2 for 2nd gen and above

  const Z_Device & device = zigbee_devices.findShortAddr(shortaddr);
  const Z_Data_Light & light = device.data.find<Z_Data_Light>();
  if (&light != &z_data_unk) {
    bri = light.getDimmer();
    colormode = light.getColorMode();
    sat = light.getSat();
    ct = light.getCT();
    hue = light.getHue();
    x = light.getX();
    y = light.getY();
  }
  power = device.getPower();
  reachable = device.getReachable();

  if (bri > 254)   bri = 254;    // Philips Hue bri is between 1 and 254
  if (bri < 1)     bri = 1;
  if (sat > 254)   sat = 254;   // Philips Hue only accepts 254 as max hue
  uint16_t hue16 = changeUIntScale(hue, 0, 360, 0, 65535);

  const size_t buf_size = 256;
  char * buf = (char*) malloc(buf_size);     // temp buffer for strings, avoid stack

  snprintf_P(buf, buf_size, PSTR("{\"on\":%s,"), power ? PSTR("true") : PSTR("false"));
  // Brightness for all devices with PWM
  if ((1 == echo_gen) || (LST_SINGLE <= local_light_subtype)) { // force dimmer for 1st gen Echo
    snprintf_P(buf, buf_size, PSTR("%s\"bri\":%d,"), buf, bri);
  }
  if (LST_COLDWARM <= local_light_subtype) {
    snprintf_P(buf, buf_size, PSTR("%s\"colormode\":\"%s\","), buf, (0 == colormode) ? PSTR("hs") : (1 == colormode) ? PSTR("xy") : PSTR("ct"));
  }
  if (LST_RGB <= local_light_subtype) {  // colors
    if (prev_x_str[0] && prev_y_str[0]) {
      snprintf_P(buf, buf_size, PSTR("%s\"xy\":[%s,%s],"), buf, prev_x_str, prev_y_str);
    } else {
      float x_f = x / 65536.0f;
      float y_f = y / 65536.0f;
      snprintf_P(buf, buf_size, PSTR("%s\"xy\":[%s,%s],"), buf, String(x_f, 5).c_str(), String(y_f, 5).c_str());
    }
    snprintf_P(buf, buf_size, PSTR("%s\"hue\":%d,\"sat\":%d,"), buf, hue16, sat);
  }
  if (LST_COLDWARM == local_light_subtype || LST_RGBW <= local_light_subtype) {  // white temp
    snprintf_P(buf, buf_size, PSTR("%s\"ct\":%d,"), buf, ct > 0 ? ct : 284);
  }
  snprintf_P(buf, buf_size, HUE_LIGHTS_STATUS_JSON1_SUFFIX_ZIGBEE, buf, reachable ? PSTR("true") : PSTR("false"));

  *response += buf;
  free(buf);
}

void HueLightStatus2Zigbee(uint16_t shortaddr, String *response)
{
  const Z_Device & device = zigbee_devices.findShortAddr(shortaddr);
  const char * friendlyName = device.friendlyName;
  const char * modelId = device.modelId;
  const char * manufacturerId = device.manufacturerId;
  char shortaddrname[8];
  snprintf_P(shortaddrname, sizeof(shortaddrname), PSTR("0x%04X"), shortaddr);

  HueLightStatus2Generic(response, (friendlyName) ? friendlyName : shortaddrname,
              (modelId) ? modelId : PSTR("Unknown"),
              (manufacturerId) ? manufacturerId : PSTR("Tasmota"),
              GetHueDeviceId(shortaddr).c_str());
}

int32_t ZigbeeHueStatus(String * response, uint16_t shortaddr) {
  int8_t bulbtype = zigbee_devices.getHueBulbtype(shortaddr);
  if (bulbtype >= 0) {  // respond only if eligible
    *response += F("{\"state\":");
    HueLightStatus1Zigbee(shortaddr, zigbee_devices.getHueBulbtype(shortaddr), response);
    HueLightStatus2Zigbee(shortaddr, response);
    return 200;
  } else {
    return -3;
  }
}

void ZigbeeCheckHue(String & response, bool * appending) {
  uint32_t zigbee_num = zigbee_devices.devicesSize();
  for (uint32_t i = 0; i < zigbee_num; i++) {
    uint16_t shortaddr = zigbee_devices.devicesAt(i).shortaddr;
    int8_t bulbtype = zigbee_devices.getHueBulbtype(shortaddr);

    if (bulbtype >= 0) {
      // this bulb is advertized
      if (*appending) { response += ","; }
      response += "\"";
      response += EncodeLightId(0, shortaddr);
      response += F("\":{\"state\":");
      HueLightStatus1Zigbee(shortaddr, bulbtype, &response);    // TODO
      HueLightStatus2Zigbee(shortaddr, &response);
      *appending = true;
    }
  }
}

void ZigbeeHueGroups(String * lights) {
  uint32_t zigbee_num = zigbee_devices.devicesSize();
  for (uint32_t i = 0; i < zigbee_num; i++) {
    uint16_t shortaddr = zigbee_devices.devicesAt(i).shortaddr;
    int8_t bulbtype = zigbee_devices.getHueBulbtype(shortaddr);

    if (bulbtype >= 0) {
      *lights += ",\"";
      *lights += EncodeLightId(i);
      *lights += "\"";
    }
  }
}

void ZigbeeSendHue(uint16_t shortaddr, uint16_t cluster, uint8_t cmd, const SBuffer & s) {
  ZCLMessage zcl(s.len());
  zcl.shortaddr = shortaddr;
  zcl.cluster = cluster;
  zcl.cmd = cmd;
  zcl.clusterSpecific = true;
  zcl.needResponse = true;
  zcl.direct = false;   // discover route
  zcl.buf.replace(s);
  zigbeeZCLSendCmd(zcl);
}

// Send commands
// Power On/Off
void ZigbeeHuePower(uint16_t shortaddr, bool power) {
  SBuffer s(0);
  ZigbeeSendHue(shortaddr, 0x0006, power ? 1 : 0, s);
  zigbee_devices.getShortAddr(shortaddr).setPower(power, 0);
}

// Dimmer
void ZigbeeHueDimmer(uint16_t shortaddr, uint8_t dimmer) {
  if (dimmer > 0xFE) { dimmer = 0xFE; }
  SBuffer s(4);
  s.add8(dimmer);
  s.add16(0x000A);    // transition time = 1s
  ZigbeeSendHue(shortaddr, 0x0008, 0x04, s);
  zigbee_devices.getLight(shortaddr).setDimmer(dimmer);
}

// CT
void ZigbeeHueCT(uint16_t shortaddr, uint16_t ct) {
  if (ct > 0xFEFF) { ct = 0xFEFF; }
  SBuffer s(4);
  s.add16(ct);
  s.add16(0x000A);    // transition time = 1s
  ZigbeeSendHue(shortaddr, 0x0300, 0x0A, s);
  Z_Data_Light & light = zigbee_devices.getLight(shortaddr);
  light.setColorMode(2);      // "ct"
  light.setCT(ct);
}

// XY
void ZigbeeHueXY(uint16_t shortaddr, uint16_t x, uint16_t y) {
  if (x > 0xFEFF) { x = 0xFEFF; }
  if (y > 0xFEFF) { y = 0xFEFF; }
  SBuffer s(8);
  s.add16(x);
  s.add16(y);
  s.add16(0x000A);    // transition time = 1s
  ZigbeeSendHue(shortaddr, 0x0300, 0x07, s);
  Z_Data_Light & light = zigbee_devices.getLight(shortaddr);
  light.setColorMode(1);      // "xy"
  light.setX(x);
  light.setY(y);
}

// HueSat
void ZigbeeHueHS(uint16_t shortaddr, uint16_t hue, uint8_t sat) {
  uint8_t hue8 = changeUIntScale(hue, 0, 360, 0, 254);
  if (sat > 0xFE) { sat = 0xFE; }
  SBuffer s(4);
  s.add8(hue8);
  s.add8(sat);
  s.add16(0);
  ZigbeeSendHue(shortaddr, 0x0300, 0x06, s);
  Z_Data_Light & light = zigbee_devices.getLight(shortaddr);
  light.setColorMode(0);      // "hs"
  light.setSat(sat);
  light.setHue(hue);
}

int32_t ZigbeeHandleHue(uint16_t shortaddr, uint32_t device_id, String &response) {
  uint8_t  bri, sat;
  uint16_t ct, hue;
  int code = 200;

  int8_t bulbtype = zigbee_devices.getHueBulbtype(shortaddr);
  if (bulbtype < 0) {  // respond only if eligible
    response = F("{}");
    return 200;
  }

  bool resp = false;  // is the response non null (add comma between parameters)
  bool on = false;

  const size_t buf_size = 100;
  char * buf = (char*) malloc(buf_size);
  UnishoxStrings msg(HUE_LIGHTS);

  if (Webserver->args()) {
    response = "[";

#ifdef ESP82666   // ESP8266 memory is limited, avoid copying and modify in place
    JsonParser parser((char*) Webserver->arg((Webserver->args())-1).c_str());
#else             // does not work on ESP32, we need to get a fresh copy of the string
    String request_arg = Webserver->arg((Webserver->args())-1);
    JsonParser parser((char*) request_arg.c_str());
#endif
    JsonParserObject root = parser.getRootObject();

    JsonParserToken hue_on = root[PSTR("on")];
    if (hue_on) {
      on = hue_on.getBool();
      snprintf_P(buf, buf_size,
                 msg[HUE_RESP_ON],
                 device_id, on ? PSTR("true") : PSTR("false"));

      if (on) {
        ZigbeeHuePower(shortaddr, 0x01);
      } else {
        ZigbeeHuePower(shortaddr, 0x00);
      }
      response += buf;
      resp = true;
    }

    parser.setCurrent();
    JsonParserToken hue_bri = root[PSTR("bri")];
    if (hue_bri) {             // Brightness is a scale from 1 (the minimum the light is capable of) to 254 (the maximum). Note: a brightness of 1 is not off.
      bri = hue_bri.getUInt();
      prev_bri = bri;   // store command value
      if (resp) { response += ","; }
      snprintf_P(buf, buf_size,
                 msg[HUE_RESP_NUM],
                 device_id, PSTR("bri"), bri);
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
    parser.setCurrent();
    JsonParserToken hue_xy = root[PSTR("xy")];
    if (hue_xy) {
      JsonParserArray arr_xy = JsonParserArray(hue_xy);
      JsonParserToken tok_x = arr_xy[0];
      JsonParserToken tok_y = arr_xy[1];
      float x = tok_x.getFloat();
      float y = tok_y.getFloat();
      strlcpy(prev_x_str, tok_x.getStr(), sizeof(prev_x_str));
      strlcpy(prev_y_str, tok_y.getStr(), sizeof(prev_y_str));
      if (resp) { response += ","; }
      snprintf_P(buf, buf_size,
                 msg[HUE_RESP_XY],
                 device_id, prev_x_str, prev_y_str);
      response += buf;
      resp = true;
      uint16_t xi = x * 65536.0f;
      uint16_t yi = y * 65536.0f;
      ZigbeeHueXY(shortaddr, xi, yi);
    }
    bool huesat_changed = false;

    parser.setCurrent();
    JsonParserToken hue_hue = root[PSTR("hue")];
    if (hue_hue) {             // The hue value is a wrapping value between 0 and 65535. Both 0 and 65535 are red, 25500 is green and 46920 is blue.
      hue = hue_hue.getUInt();
      prev_hue = hue;
      if (resp) { response += ","; }
      snprintf_P(buf, buf_size,
                 msg[HUE_RESP_NUM],
                 device_id, PSTR("hue"), hue);
      response += buf;
      if (LST_RGB <= bulbtype) {
        // change range from 0..65535 to 0..360
        hue = changeUIntScale(hue, 0, 65535, 0, 360);
        huesat_changed = true;
      }
      resp = true;
    }

    parser.setCurrent();
    JsonParserToken hue_sat = root[PSTR("sat")];
    if (hue_sat) {             // Saturation of the light. 254 is the most saturated (colored) and 0 is the least saturated (white).
      sat = hue_sat.getUInt();
      prev_sat = sat;   // store command value
      if (resp) { response += ","; }
      snprintf_P(buf, buf_size,
                 msg[HUE_RESP_NUM],
                 device_id, PSTR("sat"), sat);
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

    parser.setCurrent();
    JsonParserToken hue_ct = root[PSTR("ct")];
    if (hue_ct) {  // Color temperature 153 (Cold) to 500 (Warm)
      ct = hue_ct.getUInt();
      prev_ct = ct;   // store commande value
      if (resp) { response += ","; }
      snprintf_P(buf, buf_size,
                 msg[HUE_RESP_NUM],
                 device_id, PSTR("ct"), ct);
      response += buf;
      if ((LST_COLDWARM == bulbtype) || (LST_RGBW <= bulbtype)) {
        ZigbeeHueCT(shortaddr, ct);
      }
      resp = true;
    }

    response += "]";
    if (2 == response.length()) {
      response = msg[HUE_ERROR_JSON];
    }
  }
  else {
    response = msg[HUE_ERROR_JSON];
  }
  free(buf);
  return 200;
}

#endif  // USE_WEBSERVER && USE_EMULATION && USE_EMULATION_HUE
#endif  // USE_ZIGBEE
