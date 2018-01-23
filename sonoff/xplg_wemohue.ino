/*
  xplg_wemohue.ino - wemo and hue support for Sonoff-Tasmota

  Copyright (C) 2018  Heiko Krupp and Theo Arends

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

#ifdef USE_EMULATION
/*********************************************************************************************\
 * Belkin WeMo and Philips Hue bridge emulation
\*********************************************************************************************/

#define UDP_BUFFER_SIZE         200      // Max UDP buffer size needed for M-SEARCH message
#define UDP_MSEARCH_SEND_DELAY  1500     // Delay in ms before M-Search response is send

#include <Ticker.h>
Ticker TickerMSearch;

boolean udp_connected = false;

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
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_UPNP D_WEMO " " D_JSON_TYPE " %d, %s " D_TO " %s:%d"),
    echo_type, message, udp_remote_ip.toString().c_str(), udp_remote_port);
  AddLog(LOG_LEVEL_DEBUG);

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
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_UPNP D_HUE " %s " D_TO " %s:%d"),
    message, udp_remote_ip.toString().c_str(), udp_remote_port);
  AddLog(LOG_LEVEL_DEBUG);

  udp_response_mutex = false;
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
      udp_response_mutex = false;
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

#ifdef USE_WEBSERVER
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
        "<dataType>Boolean</dataType>"
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

void HandleUpnpEvent()
{
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, PSTR(D_WEMO_BASIC_EVENT));

  String request = WebServer->arg(0);
  String state_xml = FPSTR(WEMO_RESPONSE_STATE_SOAP);
  //differentiate get and set state
  if (request.indexOf(F("SetBinaryState")) > 0) {
    if (request.indexOf(F("State>1</Binary")) > 0) {
      ExecuteCommandPower(devices_present, 1);
    }
    else if (request.indexOf(F("State>0</Binary")) > 0) {
      ExecuteCommandPower(devices_present, 0);
    }
  }
  else if(request.indexOf(F("GetBinaryState")) > 0){
    state_xml.replace(F("Set"), F("Get"));
  }
  state_xml.replace("{x1", String(bitRead(power, devices_present -1)));
  WebServer->send(200, FPSTR(HDR_CTYPE_XML), state_xml);
}

void HandleUpnpService()
{
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, PSTR(D_WEMO_EVENT_SERVICE));

  WebServer->send(200, FPSTR(HDR_CTYPE_PLAIN), FPSTR(WEMO_EVENTSERVICE_XML));
}

void HandleUpnpMetaService()
{
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, PSTR(D_WEMO_META_SERVICE));

  WebServer->send(200, FPSTR(HDR_CTYPE_PLAIN), FPSTR(WEMO_METASERVICE_XML));
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
  "{\"on\":%s,"
  "\"bri\":%u,"
  "\"hue\":%u,"
  "\"sat\":%u,"
  "\"xy\":[0.5, 0.5],"
  "\"ct\":500,"
  "\"alert\":\"none\","
  "\"effect\":\"none\","
  "\"colormode\":\"hs\","
  "\"reachable\":true}";
const char HUE_LIGHTS_STATUS_JSON[] PROGMEM =
  ",\"type\":\"Extended color light\","
  "\"name\":\"%s\","
  "\"modelid\":\"LCT007\","
  "\"uniqueid\":\"%s\","
  "\"swversion\":\"5.50.1.19085\""
  "}";
const char HUE_GROUP0_STATUS_JSON[] PROGMEM =
  "{\"name\":\"Group 0\","
   "\"lights\":[%s],"
   "\"type\":\"LightGroup\","
   "\"action\":";

const char HueConfigResponse_JSON[] PROGMEM =
  "{\"name\":\"Philips hue\","
   "\"mac\":\"%s\","
   "\"dhcp\":true,"
   "\"ipaddress\":\"%s\","
   "\"netmask\":\"%s\","
   "\"gateway\":\"%s\","
   "\"proxyaddress\":\"none\","
   "\"proxyport\":0,"
   "\"bridgeid\":\"%s\","
   "\"UTC\":\"%s\","
   "\"whitelist\":{\"%s\":{"
     "\"last use date\":\"%s\","
     "\"create date\":\"%s\","
     "\"name\":\"Remote\"}},"
   "\"swversion\":\"01039019\","
   "\"apiversion\":\"1.17.0\","
   "\"swupdate\":{\"updatestate\":0,\"url\":\"\",\"text\":\"\",\"notify\": false},"
   "\"linkbutton\":false,"
   "\"portalservices\":false"
  "}";
const char HUE_LIGHT_RESPONSE_JSON[] PROGMEM =
  "{\"success\":{\"/lights/%u/state/%s\":%s}}";
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

int HueConfigResponse(char *output, int len)
{

  return snprintf_P(output, len, HueConfigResponse_JSON, WiFi.macAddress().c_str(), WiFi.localIP().toString().c_str(),
                    WiFi.subnetMask().toString().c_str(), WiFi.gatewayIP().toString().c_str(), HueBridgeId().c_str(),
                    GetUtcDateAndTime().c_str(), GetHueUserId().c_str(), GetUtcDateAndTime().c_str(), GetUtcDateAndTime().c_str());
}

void HueConfig(String *path)
{
  char* cresponse = mqtt_data;
  HueConfigResponse(cresponse, sizeof(mqtt_data));
  WebServer->send(200, FPSTR(HDR_CTYPE_JSON), cresponse);
}

void HueGlobalConfig(String *path)
{
  //String response;
  char* cresponse = mqtt_data;
  uint8_t maxhue = (devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : devices_present;

  char *cpath = (char*)path->c_str();

  cpath++;                                           // skip leading / to get <id>
  int tmpiter = 0;
  tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, PSTR("{\"lights\":{"));
  for (uint8_t i = 1; i <= maxhue; i++) {
    tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, PSTR("\"%d\":{\"state\":"), i);
    uint16_t hue=0; uint8_t sat=0; uint8_t bri=0;
    if (light_type) {
      LightGetHsbFixedPoint(&hue, &sat, &bri);
    }
    tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, HueLightStatus_JSON, (power & (1 << (i-1))) ? "true" : "false", hue, sat, bri);
    tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, HUE_LIGHTS_STATUS_JSON, Settings.friendlyname[i-1], GetHueDeviceId(i).c_str());
    if (i < maxhue) {
      tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, ",");
    }
  }
  tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, PSTR("},\"groups\":{},\"schedules\":{},\"config\":"));
  tmpiter += HueConfigResponse(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter);

  tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, "}");
  WebServer->send(200, FPSTR(HDR_CTYPE_JSON), cresponse);
}

void HueAuthentication(String *path)
{
  char response[38];

  snprintf_P(response, sizeof(response), PSTR("[{\"success\":{\"username\":\"%s\"}}]"), GetHueUserId().c_str());
  WebServer->send(200, FPSTR(HDR_CTYPE_JSON), response);
}


static int endsWith(const char *str, const char *suffix)
{
    if (!str || !suffix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix >  lenstr)
        return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

void HueLights(String *path)
{
/*
 * http://sonoff/api/username/lights/1/state?1={"on":true,"hue":56100,"sat":254,"bri":254,"alert":"none","transitiontime":40}
 */
  //String response;
  char* cresponse = mqtt_data;
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
  bool changepower = false;
  char id[4];
  uint8_t maxhue = (devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : devices_present;

  char *cpath = (char*)path->c_str();

  cpath = strstr(cpath, "/lights");
  if (endsWith(cpath, "/lights")) {                   // Got /lights
    int tmpiter = 0;
    tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, "{\"");
    for (uint8_t i = 1; i <= maxhue; i++) {
      tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, PSTR("%d\":{\"state\":"), i);
      uint16_t hue=0; uint8_t sat=0; uint8_t bri=0;
      if (light_type) {
        LightGetHsbFixedPoint(&hue, &sat, &bri);
      }
      tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, HueLightStatus_JSON, (power & (1 << (i-1))) ? "true" : "false", hue, sat, bri);
      tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, HUE_LIGHTS_STATUS_JSON, Settings.friendlyname[i-1], GetHueDeviceId(i).c_str());
      if (i < maxhue) {
        tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, ",\"");
      }

    }
    strncat(cresponse, "}", sizeof(mqtt_data));
    WebServer->send(200, FPSTR(HDR_CTYPE_JSON), cresponse);
  }
  else if (endsWith(cpath, "/state")) {               // Got ID/state
    int tmpiter = 0;
    cpath += 8;
    device = atoi(cpath); //Not necessary to trime "/state", atoi will ignore trailing garbage
    if ((device < 1) || (device > maxhue)) {
      device = 1;
    }
    if (1 == WebServer->args()) {
      //response = "[";
      tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, "[");

      StaticJsonBuffer<400> jsonBuffer;
      JsonObject &hue_json = jsonBuffer.parseObject(WebServer->arg(0));
      if (hue_json.containsKey("on")) {
        const char* re;

        on = hue_json["on"];
        switch(on)
        {
          case false : changepower = true;
                       re = "false";
                       break;
          case true  : changepower = true;
                       re = "true";
                       break;
          default    : re = (power & (1 << (device-1))) ? "true" : "false";
                       break;
        }
        tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, HUE_LIGHT_RESPONSE_JSON, device, "on", re);
        resp = true;
      }

      if (light_type) {
        LightGetHsb(&hue,&sat,&bri);
      }

      if (hue_json.containsKey("bri")) {
        tmp = hue_json["bri"];
        bri = (float)tmp / 254.0f;
        if (resp) {
          tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, ",");
        }
        tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, HUE_LIGHT_RESPONSE_JSON, device, "bri", String(tmp).c_str());
        resp = true;
        change = true;
      }
      if (hue_json.containsKey("hue")) {
        tmp = hue_json["hue"];
        hue = (float)tmp / 65535.0f;
        if (resp) {
          tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, ",");
        }
        tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, HUE_LIGHT_RESPONSE_JSON, device, "bri", String(tmp).c_str());
        resp = true;
        change = true;
      }
      if (hue_json.containsKey("sat")) {
        tmp = hue_json["sat"];
        sat = (float)tmp / 254.0f;
        if (resp) {
          tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, ",");
        }
        tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, HUE_LIGHT_RESPONSE_JSON, device, "sat", String(tmp).c_str());
        change = true;
      }
      if (hue_json.containsKey("ct")) {              // Color temperature 153 (Cold) to 500 (Warm)
        ct = hue_json["ct"];
        if (resp) {
          tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, ",");
        }
        tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, HUE_LIGHT_RESPONSE_JSON, device, "sat", String(tmp).c_str());
        change = true;
      }
      tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, "]");
      if (2 == strlen(cresponse)) {
        snprintf_P(cresponse, sizeof(mqtt_data), HUE_ERROR_JSON);
      }
    }
    else {
      snprintf_P(cresponse, sizeof(mqtt_data), HUE_ERROR_JSON);
    }

    // Do light commands
    WebServer->send(200, FPSTR(HDR_CTYPE_JSON), cresponse);
    if (change) {
      if (light_type) {
        LightSetHsb(hue, sat, bri, ct);
      }
    }
    if (changepower) ExecuteCommandPower(device, on);
  }
  else if(strstr(cpath, "/lights/")) {          // Got /lights/ID
    cpath+=8;                               // Remove /lights/
    device = atoi(cpath);
    if ((device < 1) || (device > maxhue)) {
      device = 1;
    }
    int tmpiter = 0;
    tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, PSTR("{\"state\":"));
    uint16_t hue=0; uint8_t sat=0; uint8_t bri=0;
    if (light_type) {
      LightGetHsbFixedPoint(&hue, &sat, &bri);
    }
    tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, HueLightStatus_JSON, (power & (1 << (device-1))) ? "true" : "false", hue,sat,bri);
    tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, HUE_LIGHTS_STATUS_JSON, Settings.friendlyname[device-1], GetHueDeviceId(device).c_str());
    WebServer->send(200, FPSTR(HDR_CTYPE_JSON), cresponse);
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
  char* cresponse = (char*) "{}";
  uint8_t maxhue = (devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : devices_present;
  char *cpath = (char*)path->c_str();

  if (endsWith(cpath, "/0")) {
    int tmpiter = 0;
    cresponse = mqtt_data;
    String lights = F("\"1\"");
    for (uint8_t i = 2; i <= maxhue; i++) {
      lights += ",\"" + String(i) + "\"";
    }
    tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, HUE_GROUP0_STATUS_JSON, lights.c_str());
    uint16_t hue=0; uint8_t sat=0; uint8_t bri=0;
    if (light_type) {
      LightGetHsbFixedPoint(&hue, &sat, &bri);
    }
    tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, HueLightStatus_JSON, (power & (1 << (1-1))) ? "true" : "false", hue,sat,bri);
    tmpiter += snprintf_P(cresponse+tmpiter, sizeof(mqtt_data)-tmpiter, PSTR("}"));
  }

  WebServer->send(200, FPSTR(HDR_CTYPE_JSON), cresponse);
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

