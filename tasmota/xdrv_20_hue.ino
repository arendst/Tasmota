/*
  xdrv_20_hue.ino - Philips Hue support for Tasmota

  Copyright (C) 2020  Heiko Krupp and Theo Arends

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

#if defined(USE_WEBSERVER) && defined(USE_EMULATION) && defined(USE_EMULATION_HUE) && defined(USE_LIGHT)
/*********************************************************************************************\
 * Philips Hue bridge emulation
 *
 * Hue Bridge UPNP support routines
 * Need to send 3 response packets with varying ST and USN
 *
 * Using Espressif Inc Mac Address of 5C:CF:7F:00:00:00
 * Philips Lighting is 00:17:88:00:00:00
\*********************************************************************************************/

#define XDRV_20           20

const char HUE_RESPONSE[] PROGMEM =
  "HTTP/1.1 200 OK\r\n"
  "HOST: 239.255.255.250:1900\r\n"
  "CACHE-CONTROL: max-age=100\r\n"
  "EXT:\r\n"
  "LOCATION: http://%s:80/description.xml\r\n"
  "SERVER: Linux/3.14.0 UPnP/1.0 IpBridge/1.24.0\r\n"  // was 1.17
  "hue-bridgeid: %s\r\n";
const char HUE_ST1[] PROGMEM =
  "ST: upnp:rootdevice\r\n"
  "USN: uuid:%s::upnp:rootdevice\r\n"
  "\r\n";
const char HUE_ST2[] PROGMEM =
  "ST: uuid:%s\r\n"
  "USN: uuid:%s\r\n"
  "\r\n";
const char HUE_ST3[] PROGMEM =
  "ST: urn:schemas-upnp-org:device:basic:1\r\n"
  "USN: uuid:%s\r\n"
  "\r\n";

String HueBridgeId(void)
{
  String temp = WiFi.macAddress();
  temp.replace(":", "");
  String bridgeid = temp.substring(0, 6);
  bridgeid += "FFFE";
  bridgeid += temp.substring(6);
  return bridgeid;  // 5CCF7FFFFE139F3D
}

String HueSerialnumber(void)
{
  String serial = WiFi.macAddress();
  serial.replace(":", "");
  serial.toLowerCase();
  return serial;  // 5ccf7f139f3d
}

String HueUuid(void)
{
  String uuid = F("f6543a06-da50-11ba-8d8f-");
  uuid += HueSerialnumber();
  return uuid;  // f6543a06-da50-11ba-8d8f-5ccf7f139f3d
}

void HueRespondToMSearch(void)
{
  char message[TOPSZ];

  TickerMSearch.detach();
  if (PortUdp.beginPacket(udp_remote_ip, udp_remote_port)) {
    char response[320];
    snprintf_P(response, sizeof(response), HUE_RESPONSE, WiFi.localIP().toString().c_str(), HueBridgeId().c_str());
    int len = strlen(response);
    String uuid = HueUuid();

    snprintf_P(response + len, sizeof(response) - len, HUE_ST1, uuid.c_str());
    PortUdp.write(response);
    PortUdp.endPacket();

    snprintf_P(response + len, sizeof(response) - len, HUE_ST2, uuid.c_str(), uuid.c_str());
    PortUdp.write(response);
    PortUdp.endPacket();

    snprintf_P(response + len, sizeof(response) - len, HUE_ST3, uuid.c_str());
    PortUdp.write(response);
    PortUdp.endPacket();

    snprintf_P(message, sizeof(message), PSTR(D_3_RESPONSE_PACKETS_SENT));
  } else {
    snprintf_P(message, sizeof(message), PSTR(D_FAILED_TO_SEND_RESPONSE));
  }
  // Do not use AddLog_P2 here (interrupt routine) if syslog or mqttlog is enabled. UDP/TCP will force exception 9
  PrepLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPNP D_HUE " %s " D_TO " %s:%d"),
    message, udp_remote_ip.toString().c_str(), udp_remote_port);

  udp_response_mutex = false;
}

/*********************************************************************************************\
 * Hue web server additions
\*********************************************************************************************/

const char HUE_DESCRIPTION_XML[] PROGMEM =
  "<?xml version=\"1.0\"?>"
  "<root xmlns=\"urn:schemas-upnp-org:device-1-0\">"
  "<specVersion>"
    "<major>1</major>"
    "<minor>0</minor>"
  "</specVersion>"
//  "<URLBase>http://{x1/</URLBase>"
  "<URLBase>http://{x1:80/</URLBase>"
  "<device>"
    "<deviceType>urn:schemas-upnp-org:device:Basic:1</deviceType>"
    "<friendlyName>Amazon-Echo-HA-Bridge ({x1)</friendlyName>"
//    "<friendlyName>Philips hue ({x1)</friendlyName>"
    "<manufacturer>Royal Philips Electronics</manufacturer>"
    "<manufacturerURL>http://www.philips.com</manufacturerURL>"
    "<modelDescription>Philips hue Personal Wireless Lighting</modelDescription>"
    "<modelName>Philips hue bridge 2012</modelName>"
    "<modelNumber>929000226503</modelNumber>"
    "<serialNumber>{x3</serialNumber>"
    "<UDN>uuid:{x2</UDN>"
  "</device>"
  "</root>\r\n"
  "\r\n";
const char HUE_LIGHTS_STATUS_JSON1_SUFFIX[] PROGMEM =
  "%s\"alert\":\"none\","
  "\"effect\":\"none\","
  "\"reachable\":true}";
const char HUE_LIGHTS_STATUS_JSON2[] PROGMEM =
  ",\"type\":\"Extended color light\","
  "\"name\":\"%s\","
  "\"modelid\":\"LCT007\","
  "\"uniqueid\":\"%s\","
  "\"swversion\":\"5.50.1.19085\"}";
const char HUE_GROUP0_STATUS_JSON[] PROGMEM =
  "{\"name\":\"Group 0\","
   "\"lights\":[{l1],"
   "\"type\":\"LightGroup\","
   "\"action\":";
//     "\"scene\":\"none\",";
const char HueConfigResponse_JSON[] PROGMEM =
  "{\"name\":\"Philips hue\","
   "\"mac\":\"{ma\","
   "\"dhcp\":true,"
   "\"ipaddress\":\"{ip\","
   "\"netmask\":\"{ms\","
   "\"gateway\":\"{gw\","
   "\"proxyaddress\":\"none\","
   "\"proxyport\":0,"
   "\"bridgeid\":\"{br\","
   "\"UTC\":\"{dt\","
   "\"whitelist\":{\"{id\":{"
     "\"last use date\":\"{dt\","
     "\"create date\":\"{dt\","
     "\"name\":\"Remote\"}},"
   "\"swversion\":\"01041302\","
   "\"apiversion\":\"1.17.0\","
   "\"swupdate\":{\"updatestate\":0,\"url\":\"\",\"text\":\"\",\"notify\": false},"
   "\"linkbutton\":false,"
   "\"portalservices\":false"
  "}";
const char HUE_ERROR_JSON[] PROGMEM =
  "[{\"error\":{\"type\":901,\"address\":\"/\",\"description\":\"Internal Error\"}}]";

/********************************************************************************************/

String GetHueDeviceId(uint16_t id)
{
  String deviceid = WiFi.macAddress();
  deviceid += F(":00:11-");
  deviceid += String(id);
  deviceid.toLowerCase();
  return deviceid;  // 5c:cf:7f:13:9f:3d:00:11-1
}

String GetHueUserId(void)
{
  char userid[7];

  snprintf_P(userid, sizeof(userid), PSTR("%03x"), ESP.getChipId());
  return String(userid);
}

void HandleUpnpSetupHue(void)
{
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, PSTR(D_HUE_BRIDGE_SETUP));
  String description_xml = FPSTR(HUE_DESCRIPTION_XML);
  description_xml.replace("{x1", WiFi.localIP().toString());
  description_xml.replace("{x2", HueUuid());
  description_xml.replace("{x3", HueSerialnumber());
  WSSend(200, CT_XML, description_xml);
}

void HueNotImplemented(String *path)
{
  AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE_API_NOT_IMPLEMENTED " (%s)"), path->c_str());

  WSSend(200, CT_JSON, "{}");
}

void HueConfigResponse(String *response)
{
  *response += FPSTR(HueConfigResponse_JSON);
  response->replace("{ma", WiFi.macAddress());
  response->replace("{ip", WiFi.localIP().toString());
  response->replace("{ms", WiFi.subnetMask().toString());
  response->replace("{gw", WiFi.gatewayIP().toString());
  response->replace("{br", HueBridgeId());
  response->replace("{dt", GetDateAndTime(DT_UTC));
  response->replace("{id", GetHueUserId());
}

void HueConfig(String *path)
{
  String response = "";
  HueConfigResponse(&response);
  WSSend(200, CT_JSON, response);
}

// device is forced to CT mode instead of HSB
// only makes sense for LST_COLDWARM, LST_RGBW and LST_RGBCW
bool g_gotct = false;

// store previously set values from the Alexa app
// it allows to correct slight deviations from value set by the app
// The Alexa app is very sensitive to exact values
uint16_t prev_hue = 0;
uint8_t  prev_sat = 0;
uint8_t  prev_bri = 254;
uint16_t prev_ct  = 254;
char     prev_x_str[24] = "\0"; // store previously set xy by Alexa app
char     prev_y_str[24] = "\0";

uint8_t getLocalLightSubtype(uint8_t device) {
  if (light_type) {
    if (device >= Light.device) {
      if (Settings.flag3.pwm_multi_channels) {  // SetOption68 - Enable multi-channels PWM instead of Color PWM
        return LST_SINGLE;     // If SetOption68, each channel acts like a dimmer
      } else {
        return Light.subtype;  // the actual light
      }
    } else {
      return LST_NONE;       // relays
    }
  } else {
    return LST_NONE;
  }
}

void HueLightStatus1(uint8_t device, String *response)
{
  uint16_t ct = 0;
  uint8_t  color_mode;
  String light_status = "";
  uint16_t hue = 0;
  uint8_t  sat = 0;
  uint8_t  bri = 254;
  uint32_t echo_gen = findEchoGeneration();   // 1 for 1st gen =+ Echo Dot 2nd gen, 2 for 2nd gen and above
  // local_light_subtype simulates the Light.subtype for 'device'
  // For relays LST_NONE, for dimmers LST_SINGLE
  uint8_t local_light_subtype = getLocalLightSubtype(device);

  bri = LightGetBri(device);   // get Dimmer corrected with SetOption68
  if (bri > 254)  bri = 254;    // Philips Hue bri is between 1 and 254
  if (bri < 1)    bri = 1;

#ifdef USE_SHUTTER
  if (ShutterState(device)) {
    bri = (float)((Settings.shutter_options[device-1] & 1) ? 100 - Settings.shutter_position[device-1] : Settings.shutter_position[device-1]) / 100;
  }
#endif

  if (light_type) {
    light_state.getHSB(&hue, &sat, nullptr);

    if ((bri > prev_bri ? bri - prev_bri : prev_bri - bri) < 1)
      bri = prev_bri;

    if (sat > 254)  sat = 254;  // Philips Hue only accepts 254 as max hue
    if ((sat > prev_sat ? sat - prev_sat : prev_sat - sat) < 1) {
      sat = prev_sat;
    } else {  // if sat was changed outside of Alexa, reset xy
      prev_x_str[0] = prev_y_str[0] = 0;
    }

    hue = changeUIntScale(hue, 0, 360, 0, 65535);
    if ((hue > prev_hue ? hue - prev_hue : prev_hue - hue) < 400) {
      hue = prev_hue;
    } else {  // if hue was changed outside of Alexa, reset xy
      prev_x_str[0] = prev_y_str[0] = 0;
    }

    color_mode = light_state.getColorMode();
    ct = light_state.getCT();
    if (LCM_RGB == color_mode) { g_gotct = false; }
    if (LCM_CT  == color_mode) { g_gotct = true; }
    // If LCM_BOTH == color_mode, leave g_gotct unchanged

    // re-adjust ct if close to command value
    if ((ct > prev_ct ? ct - prev_ct : prev_ct - ct) < 1)
      ct = prev_ct;

    //AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("HueLightStatus1 HSB (%d, %d, %d) Prev_HSB (%d, %d, %d)"),
    //  hue, sat, bri, prev_hue, prev_sat, prev_bri);
  }

  const size_t buf_size = 256;
  char * buf = (char*) malloc(buf_size);     // temp buffer for strings, avoid stack

  snprintf_P(buf, buf_size, PSTR("{\"on\":%s,"), (power & (1 << (device-1))) ? "true" : "false");
  // Brightness for all devices with PWM
  if ((1 == echo_gen) || (LST_SINGLE <= local_light_subtype)) { // force dimmer for 1st gen Echo
    snprintf_P(buf, buf_size, PSTR("%s\"bri\":%d,"), buf, bri);
  }
  if (LST_COLDWARM <= local_light_subtype) {
    snprintf_P(buf, buf_size, PSTR("%s\"colormode\":\"%s\","), buf, g_gotct ? "ct" : "hs");
  }
  if (LST_RGB <= local_light_subtype) {  // colors
    if (prev_x_str[0] && prev_y_str[0]) {
      snprintf_P(buf, buf_size, PSTR("%s\"xy\":[%s,%s],"), buf, prev_x_str, prev_y_str);
    } else {
      float x, y;
      light_state.getXY(&x, &y);
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

// Check whether this device should be reported to Alexa or considered hidden.
// Any device whose friendly name start with "$" is considered hidden
bool HueActive(uint8_t device) {
  if (device > MAX_FRIENDLYNAMES) { device = MAX_FRIENDLYNAMES; }
  return '$' != *SettingsText(SET_FRIENDLYNAME1 +device -1);
}

void HueLightStatus2(uint8_t device, String *response)
{
  const size_t buf_size = 192;
  char * buf = (char*) malloc(buf_size);
  const size_t max_name_len = 32;
  char fname[max_name_len + 1];

  strlcpy(fname, SettingsText(device <= MAX_FRIENDLYNAMES ? SET_FRIENDLYNAME1 + device -1 : SET_FRIENDLYNAME1 + MAX_FRIENDLYNAMES -1), max_name_len + 1);

  if (device > MAX_FRIENDLYNAMES) {
    uint32_t fname_len = strlen(fname);
    if (fname_len > max_name_len - 2) { fname_len = max_name_len - 2; }
    fname[fname_len++] = '-';
    if (device - MAX_FRIENDLYNAMES < 10) {
      fname[fname_len++] = '0' + device - MAX_FRIENDLYNAMES;
    } else {
      fname[fname_len++] = 'A' + device - MAX_FRIENDLYNAMES - 10;
    }
    fname[fname_len] = 0x00;
  }
  snprintf_P(buf, buf_size, HUE_LIGHTS_STATUS_JSON2, fname, GetHueDeviceId(device).c_str());
  *response += buf;
  free(buf);
}

// generate a unique lightId mixing local IP address and device number
// it is limited to 32 devices.
// last 24 bits of Mac address + 4 bits of local light + high bit for relays 16-31, relay 32 is mapped to 0
// Zigbee extension: bit 29 = 1, and last 16 bits = short address of Zigbee device
#ifndef USE_ZIGBEE
uint32_t EncodeLightId(uint8_t relay_id)
#else
uint32_t EncodeLightId(uint8_t relay_id, uint16_t z_shortaddr = 0)
#endif
{
  uint8_t mac[6];
  WiFi.macAddress(mac);
  uint32_t id = (mac[3] << 20) | (mac[4] << 12) | (mac[5] << 4);

  if (relay_id >= 32) {   // for Relay #32, we encode as 0
    relay_id = 0;
  }
  if (relay_id > 15) {
    id |= (1 << 28);
  }
  id |= (relay_id & 0xF);
#ifdef USE_ZIGBEE
  if ((z_shortaddr) && (!relay_id)) {
    // fror Zigbee devices, we have relay_id == 0 and shortaddr != 0
    id = (1 << 29) | z_shortaddr;
  }
#endif

  return id;
}


// get hue_id and decode the relay_id
// 4 LSB decode to 1-15, if bit 28 is set, it encodes 16-31, if 0 then 32
// Zigbee:
// If the Id encodes a Zigbee device (meaning bit 29 is set)
// it returns 0 and sets the 'shortaddr' to the device short address
#ifndef USE_ZIGBEE
uint32_t DecodeLightId(uint32_t hue_id)
#else
uint32_t DecodeLightId(uint32_t hue_id, uint16_t * shortaddr = nullptr)
#endif
{
  uint8_t relay_id = hue_id & 0xF;
  if (hue_id & (1 << 28)) {   // check if bit 25 is set, if so we have
    relay_id += 16;
  }
  if (0 == relay_id) {        // special value 0 is actually relay #32
    relay_id = 32;
  }
#ifdef USE_ZIGBEE
  if (hue_id & (1 << 29)) {
    // this is actually a Zigbee ID
    if (shortaddr) { *shortaddr = hue_id & 0xFFFF; }
    relay_id = 0;
  }
#endif // USE_ZIGBEE
  return relay_id;
}

static const char * FIRST_GEN_UA[] = {  // list of User-Agents signature
  "AEOBC",                              // Echo Dot 2ng Generation
};

// Check if the Echo device is of 1st generation, which triggers different results
uint32_t findEchoGeneration(void) {
  // result is 1 for 1st gen, 2 for 2nd gen and further
  String user_agent = WebServer->header("User-Agent");
  uint32_t gen = 2;

  for (uint32_t i = 0; i < sizeof(FIRST_GEN_UA)/sizeof(char*); i++) {
    if (user_agent.indexOf(FIRST_GEN_UA[i]) >= 0) {  // found
      gen = 1;
      break;
    }
  }
  if (0 == user_agent.length()) {
    gen = 1;        // if no user-agent, also revert to gen v1
  }

  AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE " User-Agent: %s, gen=%d"), user_agent.c_str(), gen);  // Header collection is set in xdrv_01_webserver.ino, in StartWebserver()

  return gen;
}

void HueGlobalConfig(String *path) {
  String response;

  path->remove(0,1);                                 // cut leading / to get <id>
  response = F("{\"lights\":{");
  bool appending = false;                             // do we need to add a comma to append
  CheckHue(&response, appending);
#ifdef USE_ZIGBEE
  ZigbeeCheckHue(&response, appending);
#endif // USE_ZIGBEE
  response += F("},\"groups\":{},\"schedules\":{},\"config\":");
  HueConfigResponse(&response);
  response += "}";
  WSSend(200, CT_JSON, response);
}

void HueAuthentication(String *path)
{
  char response[38];

  snprintf_P(response, sizeof(response), PSTR("[{\"success\":{\"username\":\"%s\"}}]"), GetHueUserId().c_str());
  WSSend(200, CT_JSON, response);
  AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE " Authentication Result (%s)"), response);
}

// refactored to remove code duplicates
void CheckHue(String * response, bool &appending) {
  uint8_t maxhue = (devices_present > MAX_HUE_DEVICES) ? MAX_HUE_DEVICES : devices_present;
  for (uint32_t i = 1; i <= maxhue; i++) {
    if (HueActive(i)) {
      if (appending) { *response += ","; }
      *response += "\"";
      *response += EncodeLightId(i);
      *response += F("\":{\"state\":");
      HueLightStatus1(i, response);
      HueLightStatus2(i, response);
      appending = true;
    }
  }
}

void HueLightsCommand(uint8_t device, uint32_t device_id, String &response) {
  uint16_t tmp = 0;
  uint16_t hue = 0;
  uint8_t  sat = 0;
  uint8_t  bri = 254;
  uint16_t ct = 0;
  bool on = false;
  bool resp = false;  // is the response non null (add comma between parameters)
  bool change = false;  // need to change a parameter to the light
  uint8_t local_light_subtype = getLocalLightSubtype(device); // get the subtype for this device

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

#ifdef USE_SHUTTER
      if (ShutterState(device)) {
        if (!change) {
          bri = on ? 1.0f : 0.0f; // when bri is not part of this request then calculate it
          change = true;
          resp = true;
          response += buf;        // actually publish the state
        }
      } else {
#endif
        switch(on)
        {
          case false : ExecuteCommandPower(device, POWER_OFF, SRC_HUE);
                      break;
          case true  : ExecuteCommandPower(device, POWER_ON, SRC_HUE);
                      break;
        }
        response += buf;
        resp = true;
#ifdef USE_SHUTTER
      }
#endif  // USE_SHUTTER
    }

    if (light_type && (local_light_subtype >= LST_SINGLE)) {
      if (!Settings.flag3.pwm_multi_channels) {  // SetOption68 - Enable multi-channels PWM instead of Color PWM
        light_state.getHSB(&hue, &sat, nullptr);
        bri = light_state.getBri();   // get the combined bri for CT and RGB, not only the RGB one
        ct = light_state.getCT();
        uint8_t color_mode = light_state.getColorMode();
        if (LCM_RGB == color_mode) { g_gotct = false; }
        if (LCM_CT  == color_mode) { g_gotct = true; }
        // If LCM_BOTH == color_mode, leave g_gotct unchanged
      } else {    // treat each channel as simple dimmer
        bri = LightGetBri(device);
      }
    }
    prev_x_str[0] = prev_y_str[0] = 0;  // reset xy string

    if (hue_json.containsKey("bri")) {             // Brightness is a scale from 1 (the minimum the light is capable of) to 254 (the maximum). Note: a brightness of 1 is not off.
      bri = hue_json["bri"];
      prev_bri = bri;   // store command value
      if (resp) { response += ","; }
      snprintf_P(buf, buf_size,
                 PSTR("{\"success\":{\"/lights/%d/state/%s\":%d}}"),
                 device_id, "bri", bri);
      response += buf;
      if (LST_SINGLE <= Light.subtype) {
        // extend bri value if set to max
        if (254 <= bri) { bri = 255; }
        change = true;
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
      uint8_t rr,gg,bb;
      LightStateClass::XyToRgb(x, y, &rr, &gg, &bb);
      LightStateClass::RgbToHsb(rr, gg, bb, &hue, &sat, nullptr);
      prev_hue = changeUIntScale(hue, 0, 360, 0, 65535);  // calculate back prev_hue
      prev_sat = (sat > 254 ? 254 : sat);
      //AddLog_P2(LOG_LEVEL_DEBUG_MORE, "XY RGB (%d %d %d) HS (%d %d)", rr,gg,bb,hue,sat);
      if (resp) { response += ","; }
      snprintf_P(buf, buf_size,
                 PSTR("{\"success\":{\"/lights/%d/state/xy\":[%s,%s]}}"),
                 device_id, prev_x_str, prev_y_str);
      response += buf;
      g_gotct = false;
      resp = true;
      change = true;
    }
    if (hue_json.containsKey("hue")) {             // The hue value is a wrapping value between 0 and 65535. Both 0 and 65535 are red, 25500 is green and 46920 is blue.
      hue = hue_json["hue"];
      prev_hue = hue;
      if (resp) { response += ","; }
      snprintf_P(buf, buf_size,
                 PSTR("{\"success\":{\"/lights/%d/state/%s\":%d}}"),
                 device_id, "hue", hue);
      response += buf;
      if (LST_RGB <= Light.subtype) {
        // change range from 0..65535 to 0..360
        hue = changeUIntScale(hue, 0, 65535, 0, 360);
        g_gotct = false;
        change = true;
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
      if (LST_RGB <= Light.subtype) {
        // extend sat value if set to max
        if (254 <= sat) { sat = 255; }
        g_gotct = false;
        change = true;
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
      if ((LST_COLDWARM == Light.subtype) || (LST_RGBW <= Light.subtype)) {
        g_gotct = true;
        change = true;
      }
      resp = true;
    }
    if (change) {
#ifdef USE_SHUTTER
      if (ShutterState(device)) {
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Settings.shutter_invert: %d"), Settings.shutter_options[device-1] & 1);
        ShutterSetPosition(device, bri * 100.0f );
      } else
#endif
      if (light_type && (local_light_subtype > LST_NONE)) {   // not relay
        if (!Settings.flag3.pwm_multi_channels) {  // SetOption68 - Enable multi-channels PWM instead of Color PWM
          if (g_gotct) {
            light_controller.changeCTB(ct, bri);
          } else {
            light_controller.changeHSB(hue, sat, bri);
          }
          LightPreparePower();
        } else {  // SetOption68 On, each channel is a dimmer
          LightSetBri(device, bri);
        }
        if (LST_COLDWARM <= local_light_subtype) {
          MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_CMND_COLOR));
        } else {
          MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_CMND_DIMMER));
        }
        XdrvRulesProcess();
      }
      change = false;
    }
    response += "]";
    if (2 == response.length()) {
      response = FPSTR(HUE_ERROR_JSON);
    }
  }
  else {
    response = FPSTR(HUE_ERROR_JSON);
  }
  free(buf);
}

void HueLights(String *path)
{
/*
 * http://tasmota/api/username/lights/1/state?1={"on":true,"hue":56100,"sat":254,"bri":254,"alert":"none","transitiontime":40}
 */
  String response;
  int code = 200;
  uint8_t device = 1;
  uint32_t device_id;   // the raw device_id used by Hue emulation
  uint8_t maxhue = (devices_present > MAX_HUE_DEVICES) ? MAX_HUE_DEVICES : devices_present;

  path->remove(0,path->indexOf(F("/lights")));          // Remove until /lights
  if (path->endsWith(F("/lights"))) {                   // Got /lights
    response = "{";
    bool appending = false;
    CheckHue(&response, appending);
#ifdef USE_ZIGBEE
    ZigbeeCheckHue(&response, appending);
#endif // USE_ZIGBEE
#ifdef USE_SCRIPT_HUE
    Script_Check_Hue(&response);
#endif
    response += "}";
  }
  else if (path->endsWith(F("/state"))) {               // Got ID/state
    path->remove(0,8);                               // Remove /lights/
    path->remove(path->indexOf(F("/state")));           // Remove /state
    device_id = atoi(path->c_str());
    device = DecodeLightId(device_id);
#ifdef USE_ZIGBEE
    uint16_t shortaddr;
    device = DecodeLightId(device_id, &shortaddr);
    if (shortaddr) {
      return ZigbeeHandleHue(shortaddr, device_id, response);
    }
#endif // USE_ZIGBEE

#ifdef USE_SCRIPT_HUE
    if (device > devices_present) {
      return Script_Handle_Hue(path);
    }
#endif
    if ((device >= 1) || (device <= maxhue)) {
      HueLightsCommand(device, device_id, response);
    }

  }
  else if(path->indexOf(F("/lights/")) >= 0) {          // Got /lights/ID
    AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("/lights path=%s"), path->c_str());
    path->remove(0,8);                               // Remove /lights/
    device_id = atoi(path->c_str());
    device = DecodeLightId(device_id);
#ifdef USE_ZIGBEE
    uint16_t shortaddr;
    device = DecodeLightId(device_id, &shortaddr);
    if (shortaddr) {
      ZigbeeHueStatus(&response, shortaddr);
      goto exit;
    }
#endif // USE_ZIGBEE

#ifdef USE_SCRIPT_HUE
    if (device > devices_present) {
      Script_HueStatus(&response, device-devices_present - 1);
      goto exit;
    }
#endif

    if ((device < 1) || (device > maxhue)) {
      device = 1;
    }
    response += F("{\"state\":");
    HueLightStatus1(device, &response);
    HueLightStatus2(device, &response);
  }
  else {
    response = "{}";
    code = 406;
  }
  exit:
  AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE " Result (%s)"), response.c_str());
  WSSend(code, CT_JSON, response);
}

void HueGroups(String *path)
{
/*
 * http://tasmota/api/username/groups?1={"name":"Woonkamer","lights":[],"type":"Room","class":"Living room"})
 */
  String response = "{}";
  uint8_t maxhue = (devices_present > MAX_HUE_DEVICES) ? MAX_HUE_DEVICES : devices_present;
  //AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE " HueGroups (%s)"), path->c_str());
  
  if (path->endsWith("/0")) {
    response = FPSTR(HUE_GROUP0_STATUS_JSON);
    String lights = F("\"1\"");
    for (uint32_t i = 2; i <= maxhue; i++) {
      lights += ",\"";
      lights += EncodeLightId(i);
      lights += "\"";
    }
    
#ifdef USE_ZIGBEE
    ZigbeeHueGroups(&response);
#endif // USE_ZIGBEE
    response.replace("{l1", lights);
    HueLightStatus1(1, &response);
    response += F("}");
  }

  AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE " HueGroups Result (%s)"), path->c_str());
  WSSend(200, CT_JSON, response);
}

void HandleHueApi(String *path)
{
  /* HUE API uses /api/<userid>/<command> syntax. The userid is created by the echo device and
   * on original HUE the pressed button allows for creation of this user. We simply ignore the
   * user part and allow every caller as with Web or WeMo.
   *
   * (c) Heiko Krupp, 2017
   *
   * Hue URL
   * http://tasmota/api/username/lights/1/state with post data {"on":true,"hue":56100,"sat":254,"bri":254,"alert":"none","transitiontime":40}
   * is converted by webserver to
   * http://tasmota/api/username/lights/1/state with arg plain={"on":true,"hue":56100,"sat":254,"bri":254,"alert":"none","transitiontime":40}
   */

  uint8_t args = 0;

  path->remove(0, 4);                                // remove /api
  uint16_t apilen = path->length();
  AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE_API " (%s)"), path->c_str());         // HTP: Hue API (//lights/1/state
  for (args = 0; args < WebServer->args(); args++) {
    String json = WebServer->arg(args);
    AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE_POST_ARGS " (%s)"), json.c_str());  // HTP: Hue POST args ({"on":false})
  }

  if (path->endsWith(F("/invalid/"))) {}                // Just ignore
  else if (!apilen) HueAuthentication(path);                  // New HUE App setup
  else if (path->endsWith(F("/"))) HueAuthentication(path);      // New HUE App setup
  else if (path->endsWith(F("/config"))) HueConfig(path);
  else if (path->indexOf(F("/lights")) >= 0) HueLights(path);
  else if (path->indexOf(F("/groups")) >= 0) HueGroups(path);
  else if (path->endsWith(F("/schedules"))) HueNotImplemented(path);
  else if (path->endsWith(F("/sensors"))) HueNotImplemented(path);
  else if (path->endsWith(F("/scenes"))) HueNotImplemented(path);
  else if (path->endsWith(F("/rules"))) HueNotImplemented(path);
  else if (path->endsWith(F("/resourcelinks"))) HueNotImplemented(path);
  else HueGlobalConfig(path);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv20(uint8_t function)
{
  bool result = false;

#if defined(USE_SCRIPT_HUE) || defined(USE_ZIGBEE)
  if ((EMUL_HUE == Settings.flag2.emulation)) {
#else
  if (devices_present && (EMUL_HUE == Settings.flag2.emulation)) {
#endif
    switch (function) {
      case FUNC_WEB_ADD_HANDLER:
        WebServer->on(F("/description.xml"), HandleUpnpSetupHue);
        break;
    }
  }
  return result;
}

#endif  // USE_WEBSERVER && USE_EMULATION && USE_EMULATION_HUE
