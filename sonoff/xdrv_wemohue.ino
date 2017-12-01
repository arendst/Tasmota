/*
  xdrv_wemohue.ino - wemo and hue support for Sonoff-Tasmota

  Copyright (C) 2017  Heiko Krupp and Theo Arends

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

/*********************************************************************************************\
 * Belkin WeMo and Philips Hue bridge emulation
\*********************************************************************************************/

#ifdef USE_EMULATION

#define UDP_BUFFER_SIZE 200         // Max UDP buffer size needed for M-SEARCH message

boolean udp_connected = false;

char packet_buffer[UDP_BUFFER_SIZE]; // buffer to hold incoming UDP packet
IPAddress ipMulticast(239,255,255,250); // Simple Service Discovery Protocol (SSDP)
uint32_t port_multicast = 1900;      // Multicast address and port

/*********************************************************************************************\
 * WeMo UPNP support routines
\*********************************************************************************************/

const char WEMO_MSEARCH[] PROGMEM =
  "HTTP/1.1 200 OK\r\n"
  "CACHE-CONTROL: max-age=86400\r\n"
  "DATE: Fri, 15 Apr 2016 04:56:29 GMT\r\n"
  "EXT:\r\n"
  "LOCATION: http://{r1:80/setup.xml\r\n"
  "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
  "01-NLS: b9200ebb-736d-4b93-bf03-835149d13983\r\n"
  "SERVER: Unspecified, UPnP/1.0, Unspecified\r\n"
  "ST: urn:Belkin:device:**\r\n"
  "USN: uuid:{r2::urn:Belkin:device:**\r\n"
  "X-User-Agent: redsonic\r\n"
  "\r\n";

String WemoSerialnumber()
{
  char serial[16];

  snprintf_P(serial, sizeof(serial), PSTR("201612K%08X"), ESP.getChipId());
  return String(serial);
}

String WemoUuid()
{
  char uuid[27];

  snprintf_P(uuid, sizeof(uuid), PSTR("Socket-1_0-%s"), WemoSerialnumber().c_str());
  return String(uuid);
}

void WemoRespondToMSearch()
{
  char message[TOPSZ];

  if (PortUdp.beginPacket(PortUdp.remoteIP(), PortUdp.remotePort())) {
    String response = FPSTR(WEMO_MSEARCH);
    response.replace("{r1", WiFi.localIP().toString());
    response.replace("{r2", WemoUuid());
    PortUdp.write(response.c_str());
    PortUdp.endPacket();
    snprintf_P(message, sizeof(message), PSTR(D_RESPONSE_SENT));
  } else {
    snprintf_P(message, sizeof(message), PSTR(D_FAILED_TO_SEND_RESPONSE));
  }
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_UPNP D_WEMO " %s " D_TO " %s:%d"),
    message, PortUdp.remoteIP().toString().c_str(), PortUdp.remotePort());
  AddLog(LOG_LEVEL_DEBUG);
}

/*********************************************************************************************\
 * Hue Bridge UPNP support routines
 * Need to send 3 response packets with varying ST and USN
 *
 * Using Espressif Inc Mac Address of 5C:CF:7F:00:00:00
 * Philips Lighting is 00:17:88:00:00:00
\*********************************************************************************************/

const char HUE_RESPONSE[] PROGMEM =
  "HTTP/1.1 200 OK\r\n"
  "HOST: 239.255.255.250:1900\r\n"
  "CACHE-CONTROL: max-age=100\r\n"
  "EXT:\r\n"
  "LOCATION: http://{r1:80/description.xml\r\n"
  "SERVER: Linux/3.14.0 UPnP/1.0 IpBridge/1.17.0\r\n"
  "hue-bridgeid: {r2\r\n";
const char HUE_ST1[] PROGMEM =
  "ST: upnp:rootdevice\r\n"
  "USN: uuid:{r3::upnp:rootdevice\r\n"
  "\r\n";
const char HUE_ST2[] PROGMEM =
  "ST: uuid:{r3\r\n"
  "USN: uuid:{r3\r\n"
  "\r\n";
const char HUE_ST3[] PROGMEM =
  "ST: urn:schemas-upnp-org:device:basic:1\r\n"
  "USN: uuid:{r3\r\n"
  "\r\n";

String HueBridgeId()
{
  String temp = WiFi.macAddress();
  temp.replace(":", "");
  String bridgeid = temp.substring(0, 6) + "FFFE" + temp.substring(6);
  return bridgeid;  // 5CCF7FFFFE139F3D
}

String HueSerialnumber()
{
  String serial = WiFi.macAddress();
  serial.replace(":", "");
  serial.toLowerCase();
  return serial;  // 5ccf7f139f3d
}

String HueUuid()
{
  String uuid = F("f6543a06-da50-11ba-8d8f-");
  uuid += HueSerialnumber();
  return uuid;  // f6543a06-da50-11ba-8d8f-5ccf7f139f3d
}

void HueRespondToMSearch()
{
  char message[TOPSZ];

  if (PortUdp.beginPacket(PortUdp.remoteIP(), PortUdp.remotePort())) {
    String response1 = FPSTR(HUE_RESPONSE);
    response1.replace("{r1", WiFi.localIP().toString());
    response1.replace("{r2", HueBridgeId());

    String response = response1;
    response += FPSTR(HUE_ST1);
    response.replace("{r3", HueUuid());
    PortUdp.write(response.c_str());
    PortUdp.endPacket();

    response = response1;
    response += FPSTR(HUE_ST2);
    response.replace("{r3", HueUuid());
    PortUdp.write(response.c_str());
    PortUdp.endPacket();

    response = response1;
    response += FPSTR(HUE_ST3);
    response.replace("{r3", HueUuid());
    PortUdp.write(response.c_str());
    PortUdp.endPacket();

    snprintf_P(message, sizeof(message), PSTR(D_3_RESPONSE_PACKETS_SENT));
  } else {
    snprintf_P(message, sizeof(message), PSTR(D_FAILED_TO_SEND_RESPONSE));
  }
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_UPNP D_HUE " %s " D_TO " %s:%d"),
    message, PortUdp.remoteIP().toString().c_str(), PortUdp.remotePort());
  AddLog(LOG_LEVEL_DEBUG);
}

/*********************************************************************************************\
 * Belkin WeMo and Philips Hue bridge UDP multicast support
\*********************************************************************************************/

boolean UdpDisconnect()
{
  if (udp_connected) {
    WiFiUDP::stopAll();
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPNP D_MULTICAST_DISABLED));
    udp_connected = false;
  }
  return udp_connected;
}

boolean UdpConnect()
{
  if (!udp_connected) {
    if (PortUdp.beginMulticast(WiFi.localIP(), ipMulticast, port_multicast)) {
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_UPNP D_MULTICAST_REJOINED));
      udp_connected = true;
    } else {
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_UPNP D_MULTICAST_JOIN_FAILED));
      udp_connected = false;
    }
  }
  return udp_connected;
}

void PollUdp()
{
  if (udp_connected) {
    if (PortUdp.parsePacket()) {
      int len = PortUdp.read(packet_buffer, UDP_BUFFER_SIZE -1);
      if (len > 0) {
        packet_buffer[len] = 0;
      }
      String request = packet_buffer;

//      AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("UDP: Packet received"));
//      AddLog_P(LOG_LEVEL_DEBUG_MORE, packet_buffer);

      if (request.indexOf("M-SEARCH") >= 0) {
        request.toLowerCase();
        request.replace(" ", "");

//        AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("UDP: M-SEARCH Packet received"));
//        AddLog_P(LOG_LEVEL_DEBUG_MORE, request.c_str());

        if ((EMUL_WEMO == Settings.flag2.emulation) && (request.indexOf(F("urn:belkin:device:**")) > 0)) {
          WemoRespondToMSearch();
        }
        else if ((EMUL_HUE == Settings.flag2.emulation) &&
                ((request.indexOf(F("st:urn:schemas-upnp-org:device:basic:1")) > 0) ||
                 (request.indexOf(F("st:upnp:rootdevice")) > 0) ||
                 (request.indexOf(F("st:ssdpsearch:all")) > 0) ||
                 (request.indexOf(F("st:ssdp:all")) > 0))) {
          HueRespondToMSearch();
        }
      }
    }
  }
}

#ifdef USE_WEBSERVER
/*********************************************************************************************\
 * Wemo web server additions
\*********************************************************************************************/

const char WEMO_EVENTSERVICE_XML[] PROGMEM =
  "<?scpd xmlns=\"urn:Belkin:service-1-0\"?>"
  "<actionList>"
    "<action>"
      "<name>SetBinaryState</name>"
      "<argumentList>"
        "<argument>"
          "<retval/>"
          "<name>BinaryState</name>"
          "<relatedStateVariable>BinaryState</relatedStateVariable>"
          "<direction>in</direction>"
        "</argument>"
      "</argumentList>"
      "<serviceStateTable>"
        "<stateVariable sendEvents=\"yes\">"
          "<name>BinaryState</name>"
          "<dataType>Boolean</dataType>"
          "<defaultValue>0</defaultValue>"
        "</stateVariable>"
        "<stateVariable sendEvents=\"yes\">"
          "<name>level</name>"
          "<dataType>string</dataType>"
          "<defaultValue>0</defaultValue>"
        "</stateVariable>"
      "</serviceStateTable>"
    "</action>"
  "</scpd>\r\n"
  "\r\n";
const char WEMO_SETUP_XML[] PROGMEM =
  "<?xml version=\"1.0\"?>"
  "<root>"
    "<device>"
      "<deviceType>urn:Belkin:device:controllee:1</deviceType>"
      "<friendlyName>{x1</friendlyName>"
      "<manufacturer>Belkin International Inc.</manufacturer>"
      "<modelName>Sonoff Socket</modelName>"
      "<modelNumber>3.1415</modelNumber>"
      "<UDN>uuid:{x2</UDN>"
      "<serialNumber>{x3</serialNumber>"
      "<binaryState>0</binaryState>"
      "<serviceList>"
        "<service>"
          "<serviceType>urn:Belkin:service:basicevent:1</serviceType>"
          "<serviceId>urn:Belkin:serviceId:basicevent1</serviceId>"
          "<controlURL>/upnp/control/basicevent1</controlURL>"
          "<eventSubURL>/upnp/event/basicevent1</eventSubURL>"
          "<SCPDURL>/eventservice.xml</SCPDURL>"
        "</service>"
      "</serviceList>"
    "</device>"
  "</root>\r\n"
  "\r\n";

/********************************************************************************************/

void HandleUpnpEvent()
{
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, PSTR(D_WEMO_BASIC_EVENT));
  String request = WebServer->arg(0);
  if (request.indexOf(F("State>1</Binary")) > 0) {
//    ExecuteCommandPower(1, 1);
    ExecuteCommandPower(devices_present, 1);
  }
  if (request.indexOf(F("State>0</Binary")) > 0) {
//    ExecuteCommandPower(1, 0);
    ExecuteCommandPower(devices_present, 0);
  }
  WebServer->send(200, FPSTR(HDR_CTYPE_PLAIN), "");
}

void HandleUpnpService()
{
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, PSTR(D_WEMO_EVENT_SERVICE));
  WebServer->send(200, FPSTR(HDR_CTYPE_PLAIN), FPSTR(WEMO_EVENTSERVICE_XML));
}

void HandleUpnpSetupWemo()
{
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, PSTR(D_WEMO_SETUP));

  String setup_xml = FPSTR(WEMO_SETUP_XML);
  setup_xml.replace("{x1", Settings.friendlyname[0]);
  setup_xml.replace("{x2", WemoUuid());
  setup_xml.replace("{x3", WemoSerialnumber());
  WebServer->send(200, FPSTR(HDR_CTYPE_XML), setup_xml);
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
    "<modelDescription>Philips hue Personal Wireless Lighting</modelDescription>"
    "<modelName>Philips hue bridge 2012</modelName>"
    "<modelNumber>929000226503</modelNumber>"
    "<serialNumber>{x3</serialNumber>"
    "<UDN>uuid:{x2</UDN>"
  "</device>"
  "</root>\r\n"
  "\r\n";
const char HueLightStatus_JSON[] PROGMEM =
  "\"on\":{state},"
  "\"bri\":{b},"
  "\"hue\":{h},"
  "\"sat\":{s},"
  "\"xy\":[0.5, 0.5],"
  "\"ct\":500,"
  "\"alert\":\"none\","
  "\"effect\":\"none\","
  "\"colormode\":\"hs\","
  "\"reachable\":true";
const char HUE_LIGHTS_STATUS_JSON[] PROGMEM =
  "\"type\":\"Extended color light\","
  "\"name\":\"{j1\","
  "\"modelid\":\"LCT007\","
  "\"uniqueid\":\"{j2\","
  "\"swversion\":\"5.50.1.19085\""
  "}";
const char HUE_GROUP0_STATUS_JSON[] PROGMEM =
  "{\"name\":\"Group 0\","
   "\"lights\":[{l1],"
   "\"type\":\"LightGroup\","
   "\"action\":{";
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
   "\"swversion\":\"01039019\","
   "\"apiversion\":\"1.17.0\","
   "\"swupdate\":{\"updatestate\":0,\"url\":\"\",\"text\":\"\",\"notify\": false},"
   "\"linkbutton\":false,"
   "\"portalservices\":false"
  "}";
const char HUE_LIGHT_RESPONSE_JSON[] PROGMEM =
  "{\"success\":{\"/lights/{id/state/{cm\":{re}}";
const char HUE_ERROR_JSON[] PROGMEM =
  "[{\"error\":{\"type\":901,\"address\":\"/\",\"description\":\"Internal Error\"}}]";

/********************************************************************************************/

String GetHueDeviceId(uint8_t id)
{
  String deviceid = WiFi.macAddress() + F(":00:11-") + String(id);
  deviceid.toLowerCase();
  return deviceid;  // 5c:cf:7f:13:9f:3d:00:11-1
}

String GetHueUserId()
{
  char userid[7];

  snprintf_P(userid, sizeof(userid), PSTR("%03x"), ESP.getChipId());
  return String(userid);
}

void HandleUpnpSetupHue()
{
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, PSTR(D_HUE_BRIDGE_SETUP));
  String description_xml = FPSTR(HUE_DESCRIPTION_XML);
  description_xml.replace("{x1", WiFi.localIP().toString());
  description_xml.replace("{x2", HueUuid());
  description_xml.replace("{x3", HueSerialnumber());
  WebServer->send(200, FPSTR(HDR_CTYPE_XML), description_xml);
}

void HueNotImplemented(String *path)
{
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_HTTP D_HUE_API_NOT_IMPLEMENTED " (%s)"), path->c_str());
  AddLog(LOG_LEVEL_DEBUG_MORE);

  WebServer->send(200, FPSTR(HDR_CTYPE_JSON), "{}");
}

void HueConfigResponse(String *response)
{
  *response += FPSTR(HueConfigResponse_JSON);
  response->replace("{ma", WiFi.macAddress());
  response->replace("{ip", WiFi.localIP().toString());
  response->replace("{ms", WiFi.subnetMask().toString());
  response->replace("{gw", WiFi.gatewayIP().toString());
  response->replace("{br", HueBridgeId());
  response->replace("{dt", GetUtcDateAndTime());
  response->replace("{id", GetHueUserId());
}

void HueConfig(String *path)
{
  String response = "";
  HueConfigResponse(&response);
  WebServer->send(200, FPSTR(HDR_CTYPE_JSON), response);
}

void HueLightStatus(byte device, String *response)
{
  *response += FPSTR(HueLightStatus_JSON);
  response->replace("{state}", (power & (1 << (device-1))) ? "true" : "false");

  if (light_type) {
    LightReplaceHsb(response);
  } else {
    response->replace("{h}", "0");
    response->replace("{s}", "0");
    response->replace("{b}", "0");
  }
}

void HueGlobalConfig(String *path)
{
  String response;
  uint8_t maxhue = (devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : devices_present;

  path->remove(0,1);                                 // cut leading / to get <id>
  response = F("{\"lights\":{\"");
  for (uint8_t i = 1; i <= maxhue; i++) {
    response += i;
    response += F("\":{\"state\":{");
    HueLightStatus(i, &response);
    response += "},";
    response += FPSTR(HUE_LIGHTS_STATUS_JSON);
    response.replace("{j1", Settings.friendlyname[i-1]);
    response.replace("{j2", GetHueDeviceId(i));
    if (i < maxhue) {
      response += ",\"";
    }
  }
  response += F("},\"groups\":{},\"schedules\":{},\"config\":");
  HueConfigResponse(&response);
  response += "}";
  WebServer->send(200, FPSTR(HDR_CTYPE_JSON), response);
}

void HueAuthentication(String *path)
{
  char response[38];

  snprintf_P(response, sizeof(response), PSTR("[{\"success\":{\"username\":\"%s\"}}]"), GetHueUserId().c_str());
  WebServer->send(200, FPSTR(HDR_CTYPE_JSON), response);
}

void HueLights(String *path)
{
/*
 * http://sonoff/api/username/lights/1/state?1={"on":true,"hue":56100,"sat":254,"bri":254,"alert":"none","transitiontime":40}
 */
  String response;
  uint8_t device = 1;
  uint16_t tmp = 0;
  int16_t pos = 0;
  float bri = 0;
  float hue = 0;
  float sat = 0;
  uint16_t ct = 0;
  bool resp = false;
  bool on = false;
  bool change = false;
  char id[4];
  uint8_t maxhue = (devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : devices_present;

  path->remove(0,path->indexOf("/lights"));          // Remove until /lights
  if (path->endsWith("/lights")) {                   // Got /lights
    response = "{\"";
    for (uint8_t i = 1; i <= maxhue; i++) {
      response += i;
      response += F("\":{\"state\":{");
      HueLightStatus(i, &response);
      response += "},";
      response += FPSTR(HUE_LIGHTS_STATUS_JSON);
      response.replace("{j1", Settings.friendlyname[i-1]);
      response.replace("{j2", GetHueDeviceId(i));
      if (i < maxhue) {
        response += ",\"";
      }
    }
    response += "}";
    WebServer->send(200, FPSTR(HDR_CTYPE_JSON), response);
  }
  else if (path->endsWith("/state")) {               // Got ID/state
    path->remove(0,8);                               // Remove /lights/
    path->remove(path->indexOf("/state"));           // Remove /state
    device = atoi(path->c_str());
    if ((device < 1) || (device > maxhue)) {
      device = 1;
    }
    if (1 == WebServer->args()) {
      response = "[";

      StaticJsonBuffer<400> jsonBuffer;
      JsonObject &hue_json = jsonBuffer.parseObject(WebServer->arg(0));
      if (hue_json.containsKey("on")) {

        response += FPSTR(HUE_LIGHT_RESPONSE_JSON);
        response.replace("{id", String(device));
        response.replace("{cm", "on");

        on = hue_json["on"];
        switch(on)
        {
          case false : ExecuteCommandPower(device, 0);
                       response.replace("{re", "false");
                       break;
          case true  : ExecuteCommandPower(device, 1);
                       response.replace("{re", "true");
                       break;
          default    : response.replace("{re", (power & (1 << (device-1))) ? "true" : "false");
                       break;
        }
        resp = true;
      }

      if (light_type) {
        LightGetHsb(&hue,&sat,&bri);
      }

      if (hue_json.containsKey("bri")) {
        tmp = hue_json["bri"];
        bri = (float)tmp / 254.0f;
        if (resp) {
          response += ",";
        }
        response += FPSTR(HUE_LIGHT_RESPONSE_JSON);
        response.replace("{id", String(device));
        response.replace("{cm", "bri");
        response.replace("{re", String(tmp));
        resp = true;
        change = true;
      }
      if (hue_json.containsKey("hue")) {
        tmp = hue_json["hue"];
        hue = (float)tmp / 65535.0f;
        if (resp) {
          response += ",";
        }
        response += FPSTR(HUE_LIGHT_RESPONSE_JSON);
        response.replace("{id", String(device));
        response.replace("{cm", "hue");
        response.replace("{re", String(tmp));
        resp = true;
        change = true;
      }
      if (hue_json.containsKey("sat")) {
        tmp = hue_json["sat"];
        sat = (float)tmp / 254.0f;
        if (resp) {
          response += ",";
        }
        response += FPSTR(HUE_LIGHT_RESPONSE_JSON);
        response.replace("{id", String(device));
        response.replace("{cm", "sat");
        response.replace("{re", String(tmp));
        change = true;
      }
      if (hue_json.containsKey("ct")) {              // Color temperature 153 (Cold) to 500 (Warm)
        ct = hue_json["ct"];
        if (resp) {
          response += ",";
        }
        response += FPSTR(HUE_LIGHT_RESPONSE_JSON);
        response.replace("{id", String(device));
        response.replace("{cm", "ct");
        response.replace("{re", String(ct));
        change = true;
      }
      if (change) {
        if (light_type) {
          LightSetHsb(hue, sat, bri, ct);
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

    WebServer->send(200, FPSTR(HDR_CTYPE_JSON), response);
  }
  else if(path->indexOf("/lights/") >= 0) {          // Got /lights/ID
    path->remove(0,8);                               // Remove /lights/
    device = atoi(path->c_str());
    if ((device < 1) || (device > maxhue)) {
      device = 1;
    }
    response += F("{\"state\":{");
    HueLightStatus(device, &response);
    response += "},";
    response += FPSTR(HUE_LIGHTS_STATUS_JSON);
    response.replace("{j1", Settings.friendlyname[device-1]);
    response.replace("{j2", GetHueDeviceId(device));
    WebServer->send(200, FPSTR(HDR_CTYPE_JSON), response);
  }
  else {
    WebServer->send(406, FPSTR(HDR_CTYPE_JSON), "{}");
  }
}

void HueGroups(String *path)
{
/*
 * http://sonoff/api/username/groups?1={"name":"Woonkamer","lights":[],"type":"Room","class":"Living room"})
 */
  String response = "{}";
  uint8_t maxhue = (devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : devices_present;

  if (path->endsWith("/0")) {
    response = FPSTR(HUE_GROUP0_STATUS_JSON);
    String lights = F("\"1\"");
    for (uint8_t i = 2; i <= maxhue; i++) {
      lights += ",\"" + String(i) + "\"";
    }
    response.replace("{l1", lights);
    HueLightStatus(1, &response);
    response += F("}}");
  }

  WebServer->send(200, FPSTR(HDR_CTYPE_JSON), response);
}

void HandleHueApi(String *path)
{
  /* HUE API uses /api/<userid>/<command> syntax. The userid is created by the echo device and
   * on original HUE the pressed button allows for creation of this user. We simply ignore the
   * user part and allow every caller as with Web or WeMo.
   *
   * (c) Heiko Krupp, 2017
   */

  uint8_t args = 0;

  path->remove(0, 4);                                // remove /api
  uint16_t apilen = path->length();
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_HTTP D_HUE_API " (%s)"), path->c_str());
  AddLog(LOG_LEVEL_DEBUG_MORE);                      // HTP: Hue API (//lights/1/state)
  for (args = 0; args < WebServer->args(); args++) {
    String json = WebServer->arg(args);
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_HTTP D_HUE_POST_ARGS " (%s)"), json.c_str());
    AddLog(LOG_LEVEL_DEBUG_MORE);                    // HTP: Hue POST args ({"on":false})
  }

  if (path->endsWith("/invalid/")) {}                // Just ignore
  else if (!apilen) HueAuthentication(path);                  // New HUE App setup
  else if (path->endsWith("/")) HueAuthentication(path);      // New HUE App setup
  else if (path->endsWith("/config")) HueConfig(path);
  else if (path->indexOf("/lights") >= 0) HueLights(path);
  else if (path->indexOf("/groups") >= 0) HueGroups(path);
  else if (path->endsWith("/schedules")) HueNotImplemented(path);
  else if (path->endsWith("/sensors")) HueNotImplemented(path);
  else if (path->endsWith("/scenes")) HueNotImplemented(path);
  else if (path->endsWith("/rules")) HueNotImplemented(path);
  else HueGlobalConfig(path);
}
#endif  // USE_WEBSERVER
#endif  // USE_EMULATION

