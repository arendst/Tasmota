/*
  xdrv_20_hue.ino - Philips Hue support for Tasmota

  Copyright (C) 2021  Heiko Krupp and Theo Arends

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

#if defined(USE_WEBSERVER) && defined(USE_EMULATION) && defined(USE_EMULATION_HUE) && (defined(USE_ZIGBEE) || defined(USE_LIGHT))
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
  // Do not use AddLog_P( here (interrupt routine) if syslog or mqttlog is enabled. UDP/TCP will force exception 9
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPNP D_HUE " %s " D_TO " %s:%d"),
    message, udp_remote_ip.toString().c_str(), udp_remote_port);

  udp_response_mutex = false;
}

/*********************************************************************************************\
 * Hue web server additions
\*********************************************************************************************/

//<?xml version="1.0"?><root xmlns="urn:schemas-upnp-org:device-1-0"><specVersion><major>1</major><minor>0</minor></specVersion>"<URLBase>http://{x1:80/</URLBase><device><deviceType>urn:schemas-upnp-org:device:Basic:1</deviceType><friendlyName>Amazon-Echo-HA-Bridge ({x1)</friendlyName><manufacturer>Royal Philips Electronics</manufacturer><manufacturerURL>http://www.philips.com</manufacturerURL><modelDescription>Philips hue Personal Wireless Lighting</modelDescription><modelName>Philips hue bridge 2012</modelName><modelNumber>929000226503</modelNumber><serialNumber>{x3</serialNumber><UDN>uuid:{x2</UDN></device></root>\r\n\r\n
//Successfully compressed from 626 to 394 bytes (-37.1%)
const size_t HUE_DESCRIPTION_XML_SIZE = 626;
const char HUE_DESCRIPTION_XML_COMPRESSED[] PROGMEM = "\x3D\x0E\xD1\xB0\x68\x48\xCD\xFF\xDB\x9C\x7C\x3D\x87\x21\xD1\x9E\xC3\xB4\x7E\x1E"
                             "\x85\xFC\xCA\x46\xC1\xA1\x77\x8F\x87\xB0\x5F\xF8\xF3\xF0\x62\x98\xDB\xF1\xD6\x2C"
                             "\x67\x0C\x3A\xF3\xE3\xC7\x98\x8C\xCF\x43\x67\x59\xC8\x75\xB3\xD8\x7E\x1E\x85\xE1"
                             "\x8C\x32\x33\x04\x1C\x78\xFC\x3D\x06\xD9\xAF\x3E\x7E\x1C\x87\xA1\xD8\x40\x83\x14"
                             "\xF4\x1B\xBD\x9F\x3F\x0E\x33\xD0\xEC\x20\x41\x8A\x7A\x1D\x80\x8F\x85\x1E\xC3\xD0"
                             "\x85\x97\xC8\x22\x1D\x7E\x67\xE0\xAA\xA1\x87\x99\xD8\x76\x1E\xD3\x61\xC8\x79\xFD"
                             "\x9D\x87\xA1\xD8\x40\x87\x50\xF4\x04\x1D\x18\x10\xE2\x15\x19\x0C\x67\xE0\x2B\x6D"
                             "\xC7\x99\x0E\xBF\x68\x67\x99\xC8\x7A\x1D\x84\x08\xD8\x61\xE8\x63\xFA\xF8\x40\x8C"
                             "\x8B\xAC\x6B\x3F\x0A\xC6\xD9\xB7\x38\xEB\x26\x18\xAC\x3A\xC8\x51\x59\xD6\x43\xBF"
                             "\xA2\x0F\x34\x77\x4F\x69\xB0\xE4\x3B\xC7\xA1\xD8\x40\x91\x83\x1E\x83\x6F\x85\x98"
                             "\xB0\xE8\x5F\xDF\xCF\xC2\xFE\x19\x58\x48\x86\x0A\xD0\xB4\x67\x91\x30\x98\x75\xFC"
                             "\xED\x0F\xC7\xA1\xD8\x09\x18\x20\x24\x62\x44\x2C\xBE\x41\x02\x1F\x06\xE3\xE3\xE3"
                             "\xE7\x41\x80\x83\x8D\x1D\x03\xC1\xA0\x93\x89\x10\xB2\xF9\x04\x7E\x1E\x83\x70\x46"
                             "\x11\x08\xFC\x1F\xF4\x65\x6E\x71\xF8\x08\x7A\x48\xA1\x6D\x10\xC7\xFF\x67\x58\x48"
                             "\x87\xF7\xEC\x27\xEF\x22\x0B\x47\x85\x56\xF0\xF1\xE8\x76\x02\x66\x2A\x2B\x08\x39"
                             "\x39\x75\x8D\x60\x91\x90\x0E\x04\x1E\x12\x0E\x53\x94\x40\x85\x88\x82\x17\x08\x98"
                             "\x23\x08\xB8\x58\xD1\xCF\xE7\xE1\xC5\x49\xB7\x55\xDA\xEC\x81\x0F\x01\x04\x1A\xC7"
                             "\xA7\x9F\xF6\xC1\x0E\x51\xED\x36\x1C\xB3\xD0\xEC\x20\x48\x9C\x7A\x10\xB2\x10\xB8"
                             "\xFC\x16\x2F\x44\x3C\xCF\x69\xB0\xE5\x1E\x87\x61\x10\xB2\x10\xB8\xFC\x04\x3E\x38"
                             "\xCF\xC3\xD0\xEC\xFE\x65\x1F\x93\x85\x23\x74\xE1\x48\xDC";
// const char HUE_DESCRIPTION_XML[] PROGMEM =
//   "<?xml version=\"1.0\"?>"
//   "<root xmlns=\"urn:schemas-upnp-org:device-1-0\">"
//   "<specVersion>"
//     "<major>1</major>"
//     "<minor>0</minor>"
//   "</specVersion>"
//   "<URLBase>http://{x1:80/</URLBase>"
//   "<device>"
//     "<deviceType>urn:schemas-upnp-org:device:Basic:1</deviceType>"
//     "<friendlyName>Amazon-Echo-HA-Bridge ({x1)</friendlyName>"
//     "<manufacturer>Royal Philips Electronics</manufacturer>"
//     "<manufacturerURL>http://www.philips.com</manufacturerURL>"
//     "<modelDescription>Philips hue Personal Wireless Lighting</modelDescription>"
//     "<modelName>Philips hue bridge 2012</modelName>"
//     "<modelNumber>929000226503</modelNumber>"
//     "<serialNumber>{x3</serialNumber>"
//     "<UDN>uuid:{x2</UDN>"
//   "</device>"
//   "</root>\r\n"
//   "\r\n";

//%s"alert":"none","effect":"none","reachable":true}
//Successfully compressed from 50 to 34 bytes (-32%)
// const size_t HUE_LIGHTS_STATUS_JSON1_SUFFIX_size = 50;
// const char HUE_LIGHTS_STATUS_JSON1_SUFFIX[] PROGMEM = "\x3E\xBC\x7B\x2C\x27\xFA\x3D\x87\x99\xEC\xEC\xE6\x7B\x0E\xA3\xD8\xCC\x18\x61\x82"
//                              "\x34\xCF\xBB\x0C\x55\x8E\x09\x9E\xC3\xCE\xBE\x2D\x9E\xE3";
const char HUE_LIGHTS_STATUS_JSON1_SUFFIX[] PROGMEM =
  "%s\"alert\":\"none\","
  "\"effect\":\"none\","
  "\"reachable\":true}";

//,"type":"Extended color light","name":"%s","modelid":"%s","manufacturername":"%s","uniqueid":"%s"}
//Successfully compressed from 98 to 64 bytes (-34.7%)
// const size_t HUE_LIGHTS_STATUS_JSON2_size = 98;
// const char HUE_LIGHTS_STATUS_JSON2[] PROGMEM = "\x3A\x8F\x65\x19\x0C\x67\xB0\xF3\x3D\x84\xCD\x94\xF8\x46\x22\x0F\x02\xCF\xA0\xB4"
//                              "\x78\x55\x1E\xC3\xA8\xF6\x75\x8D\x67\xB0\xF3\x3D\x87\xD7\x8F\x61\xD4\x7B\x06\xE0"
//                              "\x8C\x2D\x10\x11\x25\xDF\x0B\x31\x61\xD0\xBF\xBF\x82\x3E\x06\x2F\xB4\xD4\x2D\x82"
//                              "\x1E\x08\x7B\x8D";
const char HUE_LIGHTS_STATUS_JSON2[] PROGMEM =
  ",\"type\":\"Extended color light\","
  "\"name\":\"%s\","
  "\"modelid\":\"%s\","
  "\"manufacturername\":\"%s\","
  "\"uniqueid\":\"%s\"}";

//{"name":"Group 0","lights":[{l1],"type":"LightGroup","action":
//Successfully compressed from 62 to 61 bytes (-1.6%)
const char HUE_GROUP0_STATUS_JSON[] PROGMEM =
  "{\"name\":\"Group 0\","
   "\"lights\":[{l1],"
   "\"type\":\"LightGroup\","
   "\"action\":";
//     "\"scene\":\"none\",";

//{"name":"Philips hue","mac":"{ma","dhcp":true,"ipaddress":"{ip","netmask":"{ms","gateway":"{gw","proxyaddress":"none","proxyport":0,"bridgeid":"{br","UTC":"{dt","whitelist":{"{id":{"last use date":"{dt","create date":"{dt","name":"Remote"}},"swversion":"01041302","apiversion":"1.17.0","swupdate":{"updatestate":0,"url":"","text":"","notify": false},"linkbutton":false,"portalservices":false}
//Successfully compressed from 392 to 302 bytes (-23%)
// const size_t HueConfigResponse_JSON_size = 392;
// const char HueConfigResponse_JSON[] PROGMEM = "\x3D\xA7\xB3\xAC\x6B\x3D\x87\x99\xEC\x21\x82\xB4\x2D\x19\xE4\x28\x5B\x3D\x87\x51"
//                              "\xEC\x1B\x61\x9E\xC3\xCC\xF6\x1E\xD1\xB6\x7B\x0E\xA3\xD8\x20\xA0\xC6\x1E\xC3\xCE"
//                              "\xBE\x2D\x9D\x47\xB3\x46\x58\x82\x7D\xFB\xC7\xB0\xF3\x3D\x87\xB7\x46\x1E\xC3\xA8"
//                              "\xF6\x73\xA1\xB7\xE3\x43\xD8\x79\x9E\xC3\xDA\x37\xC7\xB0\xEA\x3D\x83\xD7\x4C\x7E"
//                              "\xCC\x8F\x61\xE6\x7B\x0F\x68\xF0\xF9\xEC\x3A\x8F\x60\xCF\xE1\xB0\xC8\x11\x71\x1E"
//                              "\xCE\x60\x87\x48\x66\x7E\x8F\x61\xE6\x71\x9D\x47\xB0\x87\x7F\x44\x1E\x7A\x21\xEC"
//                              "\x3C\xCF\x61\xED\x1D\xF3\xD8\x75\x1E\xC2\x16\x54\x41\x9E\xC3\xCC\xF6\x1E\xD1\x28"
//                              "\xF6\x1D\x47\xB0\x7C\x56\xD3\x0B\x7D\x47\xB0\xF3\x3D\xA7\xB0\xF6\xE8\x87\xB0\xF3"
//                              "\x3D\xA7\xB0\x2B\xF5\x21\x7E\x68\x4B\xA6\x08\x98\x30\x7F\x77\x40\x95\x40\x10\xB8"
//                              "\x3A\x2F\xB1\xB9\x4C\xF6\x1E\xE3\xDC\x75\x1E\xCF\x0F\x99\xBF\xFB\x73\x8F\x61\xE6"
//                              "\x7B\x0E\x38\xF2\x5B\xA3\xD8\x75\x1E\xC2\xB1\x9A\x08\xB5\x0E\x43\xA4\xF1\xD1\x9E"
//                              "\xC3\xA8\xF6\x17\x87\xC5\x8C\x04\x1C\xB0\xF6\x9E\xC0\x41\x8D\xEA\xBA\x67\xB0\xF3"
//                              "\x38\xCE\xA3\xD8\x42\xFE\x11\xEC\x3C\xCF\x61\xEC\x3A\x8F\x65\x33\x65\x02\x0C\x6E"
//                              "\xCA\xD3\x06\x47\xB0\xF3\x46\x2C\x2F\x33\xDC\x75\x1E\xC0\xB7\x8D\x07\x0B\xAA\xCE"
//                              "\x3D\x87\x99\x8B\x0B\xCC\xEA\x3D\x83\x33\xF5\x61\x79\xFC\xCF\x43\x7E\x04\x2A\x2B"
//                              "\x67\xB8";
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

  snprintf_P(userid, sizeof(userid), PSTR("%03x"), ESP_getChipId());
  return String(userid);
}

void HandleUpnpSetupHue(void)
{
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_HUE_BRIDGE_SETUP));
  String description_xml = Decompress(HUE_DESCRIPTION_XML_COMPRESSED,HUE_DESCRIPTION_XML_SIZE);
  description_xml.replace("{x1", WiFi.localIP().toString());
  description_xml.replace("{x2", HueUuid());
  description_xml.replace("{x3", HueSerialnumber());
  WSSend(200, CT_XML, description_xml);
}

void HueNotImplemented(String *path)
{
  AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE_API_NOT_IMPLEMENTED " (%s)"), path->c_str());

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

#ifdef USE_LIGHT
uint8_t getLocalLightSubtype(uint8_t device) {
  if (TasmotaGlobal.light_type) {
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

  if (TasmotaGlobal.light_type) {
    light_state.getHSB(&hue, &sat, nullptr);
    if (sat > 254)  sat = 254;  // Philips Hue only accepts 254 as max hue
    hue = changeUIntScale(hue, 0, 360, 0, 65535);

    if ((sat != prev_sat) || (hue != prev_hue)) { // if sat or hue was changed outside of Alexa, reset xy
      prev_x_str[0] = prev_y_str[0] = 0;
    }

    color_mode = light_state.getColorMode();
    ct = light_state.getCT();
    if (LCM_RGB == color_mode) { g_gotct = false; }
    if (LCM_CT  == color_mode) { g_gotct = true; }
  }

  const size_t buf_size = 256;
  char * buf = (char*) malloc(buf_size);     // temp buffer for strings, avoid stack

  snprintf_P(buf, buf_size, PSTR("{\"on\":%s,"), (TasmotaGlobal.power & (1 << (device-1))) ? "true" : "false");
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
  const size_t buf_size = 300;
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
  snprintf_P(buf, buf_size, HUE_LIGHTS_STATUS_JSON2,
            EscapeJSONString(fname).c_str(),
            EscapeJSONString(Settings.user_template_name).c_str(),
            PSTR("Tasmota"),
            GetHueDeviceId(device).c_str());
  *response += buf;
  free(buf);
}
#endif // USE_LIGHT

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
  if (shortaddr) { *shortaddr = 0x0000; }
  if (hue_id & (1 << 29)) {
    // this is actually a Zigbee ID
    if (shortaddr) { *shortaddr = hue_id & 0xFFFF; }
    relay_id = 0;
  }
#endif // USE_ZIGBEE
  return relay_id;
}

// Check if the Echo device is of 1st generation, which triggers different results
inline uint32_t findEchoGeneration(void) {
  // don't try to guess from User-Agent anymore but use SetOption109
  return Settings.flag4.alexa_gen_1 ? 1 : 2;
}

void HueGlobalConfig(String *path) {
  String response;

  path->remove(0,1);                                 // cut leading / to get <id>
  response = F("{\"lights\":{");
  bool appending = false;                             // do we need to add a comma to append
#ifdef USE_LIGHT
  CheckHue(&response, appending);
#endif // USE_LIGHT
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
  AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE " Authentication Result (%s)"), response);
}

#ifdef USE_LIGHT
// refactored to remove code duplicates
void CheckHue(String * response, bool &appending) {
  uint8_t maxhue = (TasmotaGlobal.devices_present > MAX_HUE_DEVICES) ? MAX_HUE_DEVICES : TasmotaGlobal.devices_present;
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

  if (Webserver->args()) {
    response = "[";

    JsonParser parser((char*) Webserver->arg((Webserver->args())-1).c_str());
    JsonParserObject root = parser.getRootObject();

    JsonParserToken hue_on = root[PSTR("on")];
    if (hue_on) {
      on = hue_on.getBool();
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
        ExecuteCommandPower(device, (on) ? POWER_ON : POWER_OFF, SRC_HUE);
        response += buf;
        resp = true;
#ifdef USE_SHUTTER
      }
#endif  // USE_SHUTTER
    }

    if (TasmotaGlobal.light_type && (local_light_subtype >= LST_SINGLE)) {
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

    parser.setCurrent();
    JsonParserToken hue_bri = root[PSTR("bri")];
    if (hue_bri) {             // Brightness is a scale from 1 (the minimum the light is capable of) to 254 (the maximum). Note: a brightness of 1 is not off.
      bri = hue_bri.getUInt();
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
      uint8_t rr,gg,bb;
      XyToRgb(x, y, &rr, &gg, &bb);
      RgbToHsb(rr, gg, bb, &hue, &sat, nullptr);
      prev_hue = changeUIntScale(hue, 0, 360, 0, 65535);  // calculate back prev_hue
      prev_sat = (sat > 254 ? 254 : sat);
      //AddLog_P(LOG_LEVEL_DEBUG_MORE, "XY RGB (%d %d %d) HS (%d %d)", rr,gg,bb,hue,sat);
      if (resp) { response += ","; }
      snprintf_P(buf, buf_size,
                 PSTR("{\"success\":{\"/lights/%d/state/xy\":[%s,%s]}}"),
                 device_id, prev_x_str, prev_y_str);
      response += buf;
      g_gotct = false;
      resp = true;
      change = true;
    }

    parser.setCurrent();
    JsonParserToken hue_hue = root[PSTR("hue")];
    if (hue_hue) {             // The hue value is a wrapping value between 0 and 65535. Both 0 and 65535 are red, 25500 is green and 46920 is blue.
      hue = hue_hue.getUInt();
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

    parser.setCurrent();
    JsonParserToken hue_sat = root[PSTR("sat")];
    if (hue_sat) {             // Saturation of the light. 254 is the most saturated (colored) and 0 is the least saturated (white).
      sat = hue_sat.getUInt();
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

    parser.setCurrent();
    JsonParserToken hue_ct = root[PSTR("ct")];
    if (hue_ct) {  // Color temperature 153 (Cold) to 500 (Warm)
      ct = hue_ct.getUInt();
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
        AddLog_P(LOG_LEVEL_DEBUG, PSTR("Settings.shutter_invert: %d"), Settings.shutter_options[device-1] & 1);
        ShutterSetPosition(device, bri * 100.0f );
      } else
#endif
      if (TasmotaGlobal.light_type && (local_light_subtype > LST_NONE)) {   // not relay
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
#endif // USE_LIGHT

void HueLights(String *path)
{
/*
 * http://tasmota/api/username/lights/1/state?1={"on":true,"hue":56100,"sat":254,"bri":254,"alert":"none","transitiontime":40}
 */
  String response;
  int code = 200;
  uint8_t device = 1;
  uint32_t device_id;   // the raw device_id used by Hue emulation
  uint8_t maxhue = (TasmotaGlobal.devices_present > MAX_HUE_DEVICES) ? MAX_HUE_DEVICES : TasmotaGlobal.devices_present;

  path->remove(0,path->indexOf(F("/lights")));          // Remove until /lights
  if (path->endsWith(F("/lights"))) {                   // Got /lights
    response = "{";
    bool appending = false;
#ifdef USE_LIGHT
    CheckHue(&response, appending);
#endif // USE_LIGHT
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
    if (device > TasmotaGlobal.devices_present) {
      return Script_Handle_Hue(path);
    }
#endif
#ifdef USE_LIGHT
    if ((device >= 1) || (device <= maxhue)) {
      HueLightsCommand(device, device_id, response);
    }
#endif // USE_LIGHT

  }
  else if(path->indexOf(F("/lights/")) >= 0) {          // Got /lights/ID
    AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("/lights path=%s"), path->c_str());
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
    if (device > TasmotaGlobal.devices_present) {
      Script_HueStatus(&response, device-TasmotaGlobal.devices_present - 1);
      goto exit;
    }
#endif

#ifdef USE_LIGHT
    if ((device < 1) || (device > maxhue)) {
      device = 1;
    }
    response += F("{\"state\":");
    HueLightStatus1(device, &response);
    HueLightStatus2(device, &response);
#endif // USE_LIGHT
  }
  else {
    response = "{}";
    code = 406;
  }
  exit:
  AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE " Result (%s)"), response.c_str());
  WSSend(code, CT_JSON, response);
}

void HueGroups(String *path)
{
/*
 * http://tasmota/api/username/groups?1={"name":"Woonkamer","lights":[],"type":"Room","class":"Living room"})
 */
  String response = "{}";
  uint8_t maxhue = (TasmotaGlobal.devices_present > MAX_HUE_DEVICES) ? MAX_HUE_DEVICES : TasmotaGlobal.devices_present;
  //AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE " HueGroups (%s)"), path->c_str());

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
#ifdef USE_LIGHT
    HueLightStatus1(1, &response);
#endif // USE_LIGHT
    response += F("}");
  }

  AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE " HueGroups Result (%s)"), path->c_str());
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
  AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE_API " (%s) from %s"), path->c_str(), Webserver->client().remoteIP().toString().c_str());         // HTP: Hue API (//lights/1/state) from 192.168.1.20
  for (args = 0; args < Webserver->args(); args++) {
    String json = Webserver->arg(args);
    AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE_POST_ARGS " (%s)"), json.c_str());  // HTP: Hue POST args ({"on":false})
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
  if (TasmotaGlobal.devices_present && (EMUL_HUE == Settings.flag2.emulation)) {
#endif
    switch (function) {
      case FUNC_WEB_ADD_HANDLER:
        WebServer_on(PSTR("/description.xml"), HandleUpnpSetupHue);
        break;
    }
  }
  return result;
}

#endif  // USE_WEBSERVER && USE_EMULATION && USE_EMULATION_HUE
