/*
  xplg_wemohue.ino - wemo and hue support for Sonoff-Tasmota

  Copyright (C) 2019  Heiko Krupp and Theo Arends

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

//#define min(a,b) ((a)<(b)?(a):(b))
//#define max(a,b) ((a)>(b)?(a):(b))

#if defined(USE_WEBSERVER) && defined(USE_EMULATION)
/*********************************************************************************************\
 * Belkin WeMo and Philips Hue bridge emulation
\*********************************************************************************************/

#define UDP_BUFFER_SIZE         200      // Max UDP buffer size needed for M-SEARCH message
#define UDP_MSEARCH_SEND_DELAY  1500     // Delay in ms before M-Search response is send

#include <Ticker.h>
Ticker TickerMSearch;

bool udp_connected = false;

char packet_buffer[UDP_BUFFER_SIZE];     // buffer to hold incoming UDP packet
IPAddress ipMulticast(239,255,255,250);  // Simple Service Discovery Protocol (SSDP)
uint32_t port_multicast = 1900;          // Multicast address and port

bool udp_response_mutex = false;         // M-Search response mutex to control re-entry
IPAddress udp_remote_ip;                 // M-Search remote IP address
uint16_t udp_remote_port;                // M-Search remote port

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
  "ST: {r3\r\n"                 // type1 = urn:Belkin:device:**, type2 = upnp:rootdevice
  "USN: uuid:{r2::{r3\r\n"      // type1 = urn:Belkin:device:**, type2 = upnp:rootdevice
  "X-User-Agent: redsonic\r\n"
  "\r\n";

String WemoSerialnumber(void)
{
  char serial[16];

  snprintf_P(serial, sizeof(serial), PSTR("201612K%08X"), ESP.getChipId());
  return String(serial);
}

String WemoUuid(void)
{
  char uuid[27];

  snprintf_P(uuid, sizeof(uuid), PSTR("Socket-1_0-%s"), WemoSerialnumber().c_str());
  return String(uuid);
}

void WemoRespondToMSearch(int echo_type)
{
  char message[TOPSZ];

  TickerMSearch.detach();
  if (PortUdp.beginPacket(udp_remote_ip, udp_remote_port)) {
    String response = FPSTR(WEMO_MSEARCH);
    response.replace("{r1", WiFi.localIP().toString());
    response.replace("{r2", WemoUuid());
    if (1 == echo_type) {              // type1 echo 1g & dot 2g
      response.replace("{r3", F("urn:Belkin:device:**"));
    } else {                           // type2 echo 2g (echo, plus, show)
      response.replace("{r3", F("upnp:rootdevice"));
    }
    PortUdp.write(response.c_str());
    PortUdp.endPacket();
    snprintf_P(message, sizeof(message), PSTR(D_RESPONSE_SENT));
  } else {
    snprintf_P(message, sizeof(message), PSTR(D_FAILED_TO_SEND_RESPONSE));
  }
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPNP D_WEMO " " D_JSON_TYPE " %d, %s " D_TO " %s:%d"),
    echo_type, message, udp_remote_ip.toString().c_str(), udp_remote_port);

  udp_response_mutex = false;
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

String HueBridgeId(void)
{
  String temp = WiFi.macAddress();
  temp.replace(":", "");
  String bridgeid = temp.substring(0, 6) + "FFFE" + temp.substring(6);
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
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPNP D_HUE " %s " D_TO " %s:%d"),
    message, udp_remote_ip.toString().c_str(), udp_remote_port);

  udp_response_mutex = false;
}

/*********************************************************************************************\
 * Belkin WeMo and Philips Hue bridge UDP multicast support
\*********************************************************************************************/

bool UdpDisconnect(void)
{
  if (udp_connected) {
    WiFiUDP::stopAll();
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPNP D_MULTICAST_DISABLED));
    udp_connected = false;
  }
  return udp_connected;
}

bool UdpConnect(void)
{
  if (!udp_connected) {
    if (PortUdp.beginMulticast(WiFi.localIP(), ipMulticast, port_multicast)) {
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_UPNP D_MULTICAST_REJOINED));
      udp_response_mutex = false;
      udp_connected = true;
    } else {
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_UPNP D_MULTICAST_JOIN_FAILED));
      udp_connected = false;
    }
  }
  return udp_connected;
}

void PollUdp(void)
{
  if (udp_connected && !udp_response_mutex) {
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

        udp_remote_ip = PortUdp.remoteIP();
        udp_remote_port = PortUdp.remotePort();
        if (EMUL_WEMO == Settings.flag2.emulation) {
          if (request.indexOf(F("urn:belkin:device:**")) > 0) {    // type1 echo dot 2g, echo 1g's
            udp_response_mutex = true;
            TickerMSearch.attach_ms(UDP_MSEARCH_SEND_DELAY, WemoRespondToMSearch, 1);
          }
          else if ((request.indexOf(F("upnp:rootdevice")) > 0) ||  // type2 Echo 2g (echo & echo plus)
                   (request.indexOf(F("ssdpsearch:all")) > 0) ||
                   (request.indexOf(F("ssdp:all")) > 0)) {
            udp_response_mutex = true;
            TickerMSearch.attach_ms(UDP_MSEARCH_SEND_DELAY, WemoRespondToMSearch, 2);
          }
        }
        else if ((EMUL_HUE == Settings.flag2.emulation) &&
                ((request.indexOf(F("urn:schemas-upnp-org:device:basic:1")) > 0) ||
                 (request.indexOf(F("upnp:rootdevice")) > 0) ||
                 (request.indexOf(F("ssdpsearch:all")) > 0) ||
                 (request.indexOf(F("ssdp:all")) > 0))) {
            udp_response_mutex = true;
            TickerMSearch.attach_ms(UDP_MSEARCH_SEND_DELAY, HueRespondToMSearch);
        }
      }
    }
  }
}

/*********************************************************************************************\
 * Wemo web server additions
\*********************************************************************************************/

const char WEMO_EVENTSERVICE_XML[] PROGMEM =
  "<scpd xmlns=\"urn:Belkin:service-1-0\">"
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
      "</action>"
      "<action>"
        "<name>GetBinaryState</name>"
        "<argumentList>"
          "<argument>"
            "<retval/>"
            "<name>BinaryState</name>"
            "<relatedStateVariable>BinaryState</relatedStateVariable>"
            "<direction>out</direction>"
          "</argument>"
        "</argumentList>"
      "</action>"
    "</actionList>"
    "<serviceStateTable>"
      "<stateVariable sendEvents=\"yes\">"
        "<name>BinaryState</name>"
        "<dataType>bool</dataType>"
        "<defaultValue>0</defaultValue>"
      "</stateVariable>"
      "<stateVariable sendEvents=\"yes\">"
        "<name>level</name>"
        "<dataType>string</dataType>"
        "<defaultValue>0</defaultValue>"
      "</stateVariable>"
    "</serviceStateTable>"
  "</scpd>\r\n\r\n";

const char WEMO_METASERVICE_XML[] PROGMEM =
  "<scpd xmlns=\"urn:Belkin:service-1-0\">"
    "<specVersion>"
      "<major>1</major>"
      "<minor>0</minor>"
    "</specVersion>"
    "<actionList>"
      "<action>"
        "<name>GetMetaInfo</name>"
        "<argumentList>"
          "<retval />"
          "<name>GetMetaInfo</name>"
          "<relatedStateVariable>MetaInfo</relatedStateVariable>"
          "<direction>in</direction>"
        "</argumentList>"
      "</action>"
    "</actionList>"
    "<serviceStateTable>"
      "<stateVariable sendEvents=\"yes\">"
        "<name>MetaInfo</name>"
        "<dataType>string</dataType>"
        "<defaultValue>0</defaultValue>"
      "</stateVariable>"
    "</serviceStateTable>"
  "</scpd>\r\n\r\n";

const char WEMO_RESPONSE_STATE_SOAP[] PROGMEM =
  "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
    "<s:Body>"
      "<u:SetBinaryStateResponse xmlns:u=\"urn:Belkin:service:basicevent:1\">"
        "<BinaryState>{x1</BinaryState>"
      "</u:SetBinaryStateResponse>"
    "</s:Body>"
  "</s:Envelope>\r\n";

const char WEMO_SETUP_XML[] PROGMEM =
  "<?xml version=\"1.0\"?>"
  "<root xmlns=\"urn:Belkin:device-1-0\">"
    "<device>"
      "<deviceType>urn:Belkin:device:controllee:1</deviceType>"
      "<friendlyName>{x1</friendlyName>"
      "<manufacturer>Belkin International Inc.</manufacturer>"
      "<modelName>Socket</modelName>"
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
        "<service>"
          "<serviceType>urn:Belkin:service:metainfo:1</serviceType>"
          "<serviceId>urn:Belkin:serviceId:metainfo1</serviceId>"
          "<controlURL>/upnp/control/metainfo1</controlURL>"
          "<eventSubURL>/upnp/event/metainfo1</eventSubURL>"
          "<SCPDURL>/metainfoservice.xml</SCPDURL>"
        "</service>"
      "</serviceList>"
    "</device>"
  "</root>\r\n";

/********************************************************************************************/

void HandleUpnpEvent(void)
{
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, PSTR(D_WEMO_BASIC_EVENT));

  String request = WebServer->arg(0);
  String state_xml = FPSTR(WEMO_RESPONSE_STATE_SOAP);
  //differentiate get and set state
  if (request.indexOf(F("SetBinaryState")) > 0) {
    uint8_t power = POWER_TOGGLE;
    if (request.indexOf(F("State>1</Binary")) > 0) {
      power = POWER_ON;
    }
    else if (request.indexOf(F("State>0</Binary")) > 0) {
      power = POWER_OFF;
    }
    if (power != POWER_TOGGLE) {
      uint8_t device = (light_type) ? devices_present : 1;  // Select either a configured light or relay1
      ExecuteCommandPower(device, power, SRC_WEMO);
    }
  }
  else if(request.indexOf(F("GetBinaryState")) > 0){
    state_xml.replace(F("Set"), F("Get"));
  }
  state_xml.replace("{x1", String(bitRead(power, devices_present -1)));
  WSSend(200, CT_XML, state_xml);
}

void HandleUpnpService(void)
{
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, PSTR(D_WEMO_EVENT_SERVICE));

  WSSend(200, CT_PLAIN, FPSTR(WEMO_EVENTSERVICE_XML));
}

void HandleUpnpMetaService(void)
{
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, PSTR(D_WEMO_META_SERVICE));

  WSSend(200, CT_PLAIN, FPSTR(WEMO_METASERVICE_XML));
}

void HandleUpnpSetupWemo(void)
{
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, PSTR(D_WEMO_SETUP));

  String setup_xml = FPSTR(WEMO_SETUP_XML);
  setup_xml.replace("{x1", Settings.friendlyname[0]);
  setup_xml.replace("{x2", WemoUuid());
  setup_xml.replace("{x3", WemoSerialnumber());
  WSSend(200, CT_XML, setup_xml);
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
const char HUE_LIGHTS_STATUS_JSON[] PROGMEM =
  "{\"on\":{state},"
  "\"bri\":{b},"
  "\"hue\":{h},"
  "\"sat\":{s},"
  "\"xy\":[0.5, 0.5],"
  "\"ct\":{t},"
  "\"alert\":\"none\","
  "\"effect\":\"none\","
  "\"colormode\":\"{m}\","
  "\"reachable\":true}";
const char HUE_LIGHTS_STATUS_JSON2[] PROGMEM =
  ",\"type\":\"Extended color light\","
  "\"name\":\"{j1\","
  "\"modelid\":\"LCT007\","
  "\"uniqueid\":\"{j2\","
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

bool g_gotct = false;

void HueLightStatus1(uint8_t device, String *response)
{
  float hue = 0;
  float sat = 0;
  float bri = 254;
  uint16_t ct = 500;

  if (light_type) {
    LightGetHsb(&hue, &sat, &bri, g_gotct);
    ct = LightGetColorTemp();
  }
  *response += FPSTR(HUE_LIGHTS_STATUS_JSON);
  response->replace("{state}", (power & (1 << (device-1))) ? "true" : "false");
  response->replace("{h}", String((uint16_t)(65535.0f * hue)));
  response->replace("{s}", String((uint8_t)(254.0f * sat)));
  response->replace("{b}", String((uint8_t)(254.0f * bri)));
  response->replace("{t}", String(ct));
  response->replace("{m}", g_gotct?"ct":"hs");
}

void HueLightStatus2(uint8_t device, String *response)
{
  *response += FPSTR(HUE_LIGHTS_STATUS_JSON2);
  response->replace("{j1", Settings.friendlyname[device-1]);
  response->replace("{j2", GetHueDeviceId(device));
}

void HueGlobalConfig(String *path)
{
  String response;
  uint8_t maxhue = (devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : devices_present;

  path->remove(0,1);                                 // cut leading / to get <id>
  response = F("{\"lights\":{\"");
  for (uint8_t i = 1; i <= maxhue; i++) {
    response += i;
    response += F("\":{\"state\":");
    HueLightStatus1(i, &response);
    HueLightStatus2(i, &response);
    if (i < maxhue) {
      response += ",\"";
    }
  }
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
}

void HueLights(String *path)
{
/*
 * http://sonoff/api/username/lights/1/state?1={"on":true,"hue":56100,"sat":254,"bri":254,"alert":"none","transitiontime":40}
 */
  String response;
  int code = 200;
  float bri = 0;
  float hue = 0;
  float sat = 0;
  uint16_t tmp = 0;
  uint16_t ct = 0;
  bool resp = false;
  bool on = false;
  bool change = false;
  uint8_t device = 1;
  uint8_t maxhue = (devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : devices_present;

  path->remove(0,path->indexOf("/lights"));          // Remove until /lights
  if (path->endsWith("/lights")) {                   // Got /lights
    response = "{\"";
    for (uint8_t i = 1; i <= maxhue; i++) {
      response += i;
      response += F("\":{\"state\":");
      HueLightStatus1(i, &response);
      HueLightStatus2(i, &response);
      if (i < maxhue) {
        response += ",\"";
      }
    }
    response += "}";
  }
  else if (path->endsWith("/state")) {               // Got ID/state
    path->remove(0,8);                               // Remove /lights/
    path->remove(path->indexOf("/state"));           // Remove /state
    device = atoi(path->c_str());
    if ((device < 1) || (device > maxhue)) {
      device = 1;
    }
    if (WebServer->args()) {
      response = "[";

      StaticJsonBuffer<400> jsonBuffer;
      JsonObject &hue_json = jsonBuffer.parseObject(WebServer->arg((WebServer->args())-1));
      if (hue_json.containsKey("on")) {

        response += FPSTR(HUE_LIGHT_RESPONSE_JSON);
        response.replace("{id", String(device));
        response.replace("{cm", "on");

        on = hue_json["on"];
        switch(on)
        {
          case false : ExecuteCommandPower(device, POWER_OFF, SRC_HUE);
                       response.replace("{re", "false");
                       break;
          case true  : ExecuteCommandPower(device, POWER_ON, SRC_HUE);
                       response.replace("{re", "true");
                       break;
          default    : response.replace("{re", (power & (1 << (device-1))) ? "true" : "false");
                       break;
        }
        resp = true;
      }

      if (light_type) {
        LightGetHsb(&hue, &sat, &bri, g_gotct);
      }

      if (hue_json.containsKey("bri")) {             // Brightness is a scale from 1 (the minimum the light is capable of) to 254 (the maximum). Note: a brightness of 1 is not off.
        tmp = hue_json["bri"];
        tmp = tmax(tmp, 1);
        tmp = tmin(tmp, 254);
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
      if (hue_json.containsKey("hue")) {             // The hue value is a wrapping value between 0 and 65535. Both 0 and 65535 are red, 25500 is green and 46920 is blue.
        tmp = hue_json["hue"];
        hue = (float)tmp / 65535.0f;
        if (resp) {
          response += ",";
        }
        response += FPSTR(HUE_LIGHT_RESPONSE_JSON);
        response.replace("{id", String(device));
        response.replace("{cm", "hue");
        response.replace("{re", String(tmp));
        g_gotct = false;
        resp = true;
        change = true;
      }
      if (hue_json.containsKey("sat")) {             // Saturation of the light. 254 is the most saturated (colored) and 0 is the least saturated (white).
        tmp = hue_json["sat"];
        tmp = tmax(tmp, 0);
        tmp = tmin(tmp, 254);
        sat = (float)tmp / 254.0f;
        if (resp) {
          response += ",";
        }
        response += FPSTR(HUE_LIGHT_RESPONSE_JSON);
        response.replace("{id", String(device));
        response.replace("{cm", "sat");
        response.replace("{re", String(tmp));
        g_gotct = false;
        resp = true;
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
        g_gotct = true;
        change = true;
      }
      if (change) {
        if (light_type) {
          LightSetHsb(hue, sat, bri, ct, g_gotct);
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
  }
  else if(path->indexOf("/lights/") >= 0) {          // Got /lights/ID
    path->remove(0,8);                               // Remove /lights/
    device = atoi(path->c_str());
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
  WSSend(code, CT_JSON, response);
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
    HueLightStatus1(1, &response);
    response += F("}");
  }

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
   * http://sonoff/api/username/lights/1/state with post data {"on":true,"hue":56100,"sat":254,"bri":254,"alert":"none","transitiontime":40}
   * is converted by webserver to
   * http://sonoff/api/username/lights/1/state with arg plain={"on":true,"hue":56100,"sat":254,"bri":254,"alert":"none","transitiontime":40}
   */

  uint8_t args = 0;

  path->remove(0, 4);                                // remove /api
  uint16_t apilen = path->length();
  AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE_API " (%s)"), path->c_str());         // HTP: Hue API (//lights/1/state
  for (args = 0; args < WebServer->args(); args++) {
    String json = WebServer->arg(args);
    AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE_POST_ARGS " (%s)"), json.c_str());  // HTP: Hue POST args ({"on":false})
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

void HueWemoAddHandlers(void)
{
  if (EMUL_WEMO == Settings.flag2.emulation) {
    WebServer->on("/upnp/control/basicevent1", HTTP_POST, HandleUpnpEvent);
    WebServer->on("/eventservice.xml", HandleUpnpService);
    WebServer->on("/metainfoservice.xml", HandleUpnpMetaService);
    WebServer->on("/setup.xml", HandleUpnpSetupWemo);
  }
  if (EMUL_HUE == Settings.flag2.emulation) {
    WebServer->on("/description.xml", HandleUpnpSetupHue);
  }
}

#endif  // USE_WEBSERVER && USE_EMULATION
