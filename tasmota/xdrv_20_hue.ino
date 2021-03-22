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

#include "UnishoxStrings.h"

const char HUE_RESP_MSG_U[] PROGMEM =
  //=HUE_RESP_RESPONSE
  "HTTP/1.1 200 OK\r\n"
  "HOST: 239.255.255.250:1900\r\n"
  "CACHE-CONTROL: max-age=100\r\n"
  "EXT:\r\n"
  "LOCATION: http://%s:80/description.xml\r\n"
  "SERVER: Linux/3.14.0 UPnP/1.0 IpBridge/1.24.0\r\n"  // was 1.17
  "hue-bridgeid: %s\r\n"
  "\0"
  //=HUE_RESP_ST1
  "ST: upnp:rootdevice\r\n"
  "USN: uuid:%s::upnp:rootdevice\r\n"
  "\r\n"
  "\0"
  //=HUE_RESP_ST2
  "ST: uuid:%s\r\n"
  "USN: uuid:%s\r\n"
  "\r\n"
  "\0"
  //=HUE_RESP_ST3
  "ST: urn:schemas-upnp-org:device:basic:1\r\n"
  "USN: uuid:%s\r\n"
  "\r\n"
  "\0";


// Use the tool at https://tasmota.hadinger.fr/util and choose "Compress Strings template with Unishox"
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++ DO NOT EDIT BELOW ++++++++++++++++++++
// ++++++++++++++++++++vvvvvvvvvvvvvvvvvvv++++++++++++++++++++
enum {
  HUE_RESP_RESPONSE=0,
  HUE_RESP_ST1=185,
  HUE_RESP_ST2=240,
  HUE_RESP_ST3=270,
};

// Compressed from 328 to 247, -24.7%
const char HUE_RESP_MSG[] PROGMEM = "\x00\x15\x21\x45\x45\x44\x30\xEC\x39\x0E\x90\xEE\x53\x67\x70\x8B\x08\xD2\x70\xA4"
                             "\x6E\x21\x45\x85\xE2\xA3\xCD\x1C\xAB\x47\x4A\xDD\x3A\x56\xE9\xD2\xB5\x9E\x71\x36"
                             "\x53\x85\x23\x71\x06\x56\x41\x90\xA2\x67\x59\x10\x79\xD5\xFC\x08\x8F\x34\x36\xCD"
                             "\x87\x5D\x8F\x33\xE1\xC8\xD9\x4E\x14\x8D\xC4\xC8\xD8\x54\x79\xCE\x14\x8D\xC4\x41"
                             "\x60\x77\x5B\x9C\x47\x9A\x15\x54\x30\xF3\x3B\x0E\xC3\xEB\xC7\x99\xCF\xB3\xB0\x84"
                             "\x7E\x0F\xFA\x32\xB7\x38\xE8\x6C\x1A\x14\xE1\x48\xDC\x45\xE7\xF3\x37\xF2\x3C\xD1"
                             "\x05\xBC\x2C\xD8\x76\x1C\xB3\xA4\xC3\xA3\x3B\x84\x42\xC8\x67\x10\xC3\xB0\xE4\x3A"
                             "\x33\xB8\x45\xA3\x08\x77\xF4\x41\xE6\x76\x1C\x87\x4A\xC3\xA3\x29\xC2\x91\xB8\x50"
                             "\xB6\x75\x8E\xFE\x88\x3C\xF4\x43\xCD\x1F\x5E\x9C\x29\x1B\xA7\x0B\xE5\xE2\xA3\xCD"
                             "\x0B\x19\xC3\x0F\x3F\xE6\x50\x8C\xCF\x43\x73\x85\x23\x71\x0B\x2F\x17\x1E\x68\x58"
                             "\xBD\x10\xF3\x3E\xBC\x79\x9E\x60\x99\x6C\x10\xF1\x30\x41\xBA\x09\x38\x58\x22\xDA"
                             "\xFF\x1E\x7E\x0C\x53\x1B\x7E\x3A\xC5\x8C\xE1\x87\x5E\x7C\x78\xF3\x04\x1C\x78\xF3"
                             "\x1D\x7E\xD0\xCF\x33\x90\x81\x3B\x16";

// ++++++++++++++++++++^^^^^^^^^^^^^^^^^^^++++++++++++++++++++
// ++++++++++++++++++++ DO NOT EDIT ABOVE ++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// const char HUE_RESPONSE[] PROGMEM =
//   "HTTP/1.1 200 OK\r\n"
//   "HOST: 239.255.255.250:1900\r\n"
//   "CACHE-CONTROL: max-age=100\r\n"
//   "EXT:\r\n"
//   "LOCATION: http://%s:80/description.xml\r\n"
//   "SERVER: Linux/3.14.0 UPnP/1.0 IpBridge/1.24.0\r\n"  // was 1.17
//   "hue-bridgeid: %s\r\n";
// const char HUE_ST1[] PROGMEM =
//   "ST: upnp:rootdevice\r\n"
//   "USN: uuid:%s::upnp:rootdevice\r\n"
//   "\r\n";
// const char HUE_ST2[] PROGMEM =
//   "ST: uuid:%s\r\n"
//   "USN: uuid:%s\r\n"
//   "\r\n";
// const char HUE_ST3[] PROGMEM =
//   "ST: urn:schemas-upnp-org:device:basic:1\r\n"
//   "USN: uuid:%s\r\n"
//   "\r\n";

const char HUE_API_U[] PROGMEM =
  //=HUE_INVALID
  "/invalid/"
  "\0"
  //=HUE_ROOT
  "/"
  "\0"
  //=HUE_CONFIG
  "/config"
  "\0"
  //=HUE_LIGHTS_API
  "/lights"
  "\0"
  //=HUE_GROUPS
  "/groups"
  "\0"
  //=HUE_SCHEDULES
  "/schedules"
  "\0"
  //=HUE_SENSORS
  "/sensors"
  "\0"
  //=HUE_SCENES
  "/scenes"
  "\0"
  //=HUE_RULES
  "/rules"
  "\0"
  //=HUE_RESOURCELINKS
  "/resourcelinks"
  "\0"
  ;

// Use the tool at https://tasmota.hadinger.fr/util and choose "Compress Strings template with Unishox"
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++ DO NOT EDIT BELOW ++++++++++++++++++++
// ++++++++++++++++++++vvvvvvvvvvvvvvvvvvv++++++++++++++++++++
enum {
  HUE_INVALID=0,
  HUE_ROOT=10,
  HUE_CONFIG=12,
  HUE_LIGHTS_API=20,
  HUE_GROUPS=28,
  HUE_SCHEDULES=36,
  HUE_SENSORS=47,
  HUE_SCENES=56,
  HUE_RULES=64,
  HUE_RESOURCELINKS=71,
};

// Compressed from 86 to 74, -14.0%
const char HUE_API[] PROGMEM = "\x00\x06\x3B\x37\x8C\xEC\x2D\x10\xEC\x9C\x2F\x9D\x93\x85\xF3\xB0\x3C\xE3\x1A\x3D"
                             "\x38\x5F\x3B\x02\xD1\xE1\x55\xE9\xC2\xF9\xD8\x3D\xFC\x16\x33\xD3\x85\xF3\xB3\xC1"
                             "\x8A\x62\x0B\x09\xFA\x70\xBE\x76\x79\xF7\xB3\xFE\x9C\x2F\x9D\x9E\x0D\xF3\xF4\xE1"
                             "\x7C\xEC\xF8\x20\xD4\xFB\xF6\x0B\xF8\x6C\x2D\xE3\x4F\x4E\x17\xCD";
// ++++++++++++++++++++^^^^^^^^^^^^^^^^^^^++++++++++++++++++++
// ++++++++++++++++++++ DO NOT EDIT ABOVE ++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

String HueBridgeId(void)
{
  String temp = WiFi.macAddress();
  temp.replace(":", "");
  String bridgeid = temp.substring(0, 6);
  bridgeid += F("FFFE");
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
    UnishoxStrings msg(HUE_RESP_MSG);
    char response[320];
    snprintf_P(response, sizeof(response), msg[HUE_RESP_RESPONSE], WiFi.localIP().toString().c_str(), HueBridgeId().c_str());
    int len = strlen(response);
    String uuid = HueUuid();

    snprintf_P(response + len, sizeof(response) - len, msg[HUE_RESP_ST1], uuid.c_str());
    PortUdp.write(response);
    PortUdp.endPacket();

    snprintf_P(response + len, sizeof(response) - len, msg[HUE_RESP_ST2], uuid.c_str(), uuid.c_str());
    PortUdp.write(response);
    PortUdp.endPacket();

    snprintf_P(response + len, sizeof(response) - len, msg[HUE_RESP_ST3], uuid.c_str());
    PortUdp.write(response);
    PortUdp.endPacket();

    snprintf_P(message, sizeof(message), PSTR(D_3_RESPONSE_PACKETS_SENT));
  } else {
    snprintf_P(message, sizeof(message), PSTR(D_FAILED_TO_SEND_RESPONSE));
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPNP D_HUE " %s " D_TO " %s:%d"),
    message, udp_remote_ip.toString().c_str(), udp_remote_port);
}

/*********************************************************************************************\
 * Hue web server additions
\*********************************************************************************************/

//<?xml version="1.0"?><root xmlns="urn:schemas-upnp-org:device-1-0"><specVersion><major>1</major><minor>0</minor></specVersion><URLBase>http://{x1:80/</URLBase><device><deviceType>urn:schemas-upnp-org:device:Basic:1</deviceType><friendlyName>Amazon-Echo-HA-Bridge ({x1)</friendlyName><manufacturer>Royal Philips Electronics</manufacturer><manufacturerURL>http://www.philips.com</manufacturerURL><modelDescription>Philips hue Personal Wireless Lighting</modelDescription><modelName>Philips hue bridge 2012</modelName><modelNumber>929000226503</modelNumber><serialNumber>{x3</serialNumber><UDN>uuid:{x2</UDN></device></root>\r\n\r\n
//Successfully compressed from 625 to 391 bytes (-37.4%)
const size_t HUE_DESCRIPTION_XML_SIZE = 625;
const char HUE_DESCRIPTION_XML_COMPRESSED[] PROGMEM = "\x3D\x0E\xD1\xB0\x68\x48\xCD\xFF\xDB\x9C\x7C\x3D\x87\x21\xD1\x9E\xC3\xB4\x7E\x1E"
                             "\x85\xFC\xCA\x46\xC1\xA1\x77\x8F\x87\xB0\x5F\xF8\xF3\xF0\x62\x98\xDB\xF1\xD6\x2C"
                             "\x67\x0C\x3A\xF3\xE3\xC7\x98\x8C\xCF\x43\x67\x59\xC8\x75\xB3\xD8\x7E\x1E\x85\xE1"
                             "\x8C\x32\x33\x04\x1C\x78\xFC\x3D\x06\xD9\xAF\x3E\x7E\x1C\x87\xA1\xD8\x40\x83\x14"
                             "\xF4\x1B\xBD\x9F\x3F\x0E\x33\xD0\xEC\x20\x41\x8A\x7A\x1D\x80\x91\x85\x10\xB2\xF9"
                             "\x04\x43\xAF\xCC\xFC\x15\x54\x30\xF3\x3B\x0E\xC3\xDA\x6C\x39\x0F\x3F\xB3\xB0\xF4"
                             "\x3B\x08\x10\xEA\x1E\x80\x83\xA2\x82\x1C\x42\xA3\x21\x8C\xFC\x05\x6D\xB4\xF3\x21"
                             "\xD7\xED\x0C\xF3\x39\x0F\x43\xB0\x81\x1B\x0C\x3D\x0C\x7F\x5F\x08\x11\x91\x75\x8D"
                             "\x67\xE1\x58\xDB\x36\xE7\x1D\x64\xC3\x15\x87\x59\x0A\x2B\x3A\xC8\x77\xF4\x41\xE6"
                             "\x8E\xE9\xED\x36\x1C\x87\x78\xF4\x3B\x08\x12\x30\x63\xD0\x6D\xF0\xB3\x16\x1D\x0B"
                             "\xFB\xF9\xF8\x5F\xC3\x2B\x09\x10\xC1\x5A\x16\x8C\xF2\x26\x13\x0E\xBF\x9D\xA1\xF8"
                             "\xF4\x3B\x01\x23\x04\x04\x8C\x48\x85\x97\xC8\x20\x43\xE0\xDC\x7C\x7C\x7C\xE8\x30"
                             "\x10\x71\xA3\xA0\x78\x34\x12\x71\x22\x16\x5F\x20\x8F\xC3\xD0\x6E\x08\xC2\x21\x1F"
                             "\x83\xFE\x8C\xAD\xCE\x3F\x01\x0F\x49\x14\x2D\xA2\x18\xFF\xEC\xEB\x09\x10\xFE\xFD"
                             "\x84\xFD\xE4\x41\x68\xF0\xAA\xDE\x1E\x3D\x0E\xC0\x4C\xC5\x41\x07\x27\x2E\xB1\xAC"
                             "\x12\x32\x01\xC0\x83\xC2\x41\xCA\x72\x88\x10\xB1\x10\x42\xE1\x13\x04\x61\x17\x0B"
                             "\x1A\x39\xFC\xFC\x38\xA9\x36\xEA\xBB\x5D\x90\x21\xE0\x20\x83\x58\xF4\xF3\xFE\xD8"
                             "\x21\xCA\x3D\xA6\xC3\x96\x7A\x1D\x84\x09\x13\x8F\x42\x16\x42\x17\x1F\x82\xC5\xE8"
                             "\x87\x99\xED\x36\x1C\xA3\xD0\xEC\x22\x16\x42\x17\x1F\x80\x87\xC7\x19\xF8\x7A\x1D"
                             "\x9F\xCC\xA3\xF2\x70\xA4\x6E\x9C\x29\x1B\x8D";
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

const char HUE_LIGHTS_U[] PROGMEM =
  //=HUE_LIGHTS_STATUS_JSON1_SUFFIX
  "%s\"alert\":\"none\","
  "\"effect\":\"none\","
  "\"reachable\":true}"
  "\0"
  //=HUE_LIGHTS_STATUS_JSON2
  ",\"type\":\"Extended color light\","
  "\"name\":\"%s\","
  "\"modelid\":\"%s\","
  "\"manufacturername\":\"%s\","
  "\"uniqueid\":\"%s\"}"
  "\0"
  //=HUE_GROUP0_STATUS_JSON
  "{\"name\":\"Group 0\","
   "\"lights\":[{l1],"
   "\"type\":\"LightGroup\","
   "\"action\":"
   "\0"
  //=HUE_ERROR_JSON
  "[{\"error\":{\"type\":901,\"address\":\"/\",\"description\":\"Internal Error\"}}]"
  "\0"
  //=HUE_RESP_ON
  "{\"success\":{\"/lights/%d/state/on\":%s}}"
  "\0"
  //=HUE_RESP_NUM
  "{\"success\":{\"/lights/%d/state/%s\":%d}}"
  "\0"
  //=HUE_RESP_XY
  "{\"success\":{\"/lights/%d/state/xy\":[%s,%s]}}"
  "\0"
  ;

// Use the tool at https://tasmota.hadinger.fr/util and choose "Compress Strings template with Unishox"
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++ DO NOT EDIT BELOW ++++++++++++++++++++
// ++++++++++++++++++++vvvvvvvvvvvvvvvvvvv++++++++++++++++++++
enum {
  HUE_LIGHTS_STATUS_JSON1_SUFFIX=0,
  HUE_LIGHTS_STATUS_JSON2=51,
  HUE_GROUP0_STATUS_JSON=150,
  HUE_ERROR_JSON=213,
  HUE_RESP_ON=283,
  HUE_RESP_NUM=322,
  HUE_RESP_XY=361,
};

// Compressed from 405 to 275, -32.1%
const char HUE_LIGHTS[] PROGMEM = "\x00\x1A\x3E\xBC\x7B\x2C\x27\xFA\x3D\x87\x99\xEC\xEC\xE6\x7B\x0E\xA3\xD8\xCC\x18"
                             "\x61\x82\x34\xCF\xBB\x0C\x55\x8E\x09\x9E\xC3\xCE\xBE\x2D\x9E\xE9\xC2\xF9\xD4\x7B"
                             "\x28\xC8\x63\x3D\x87\x99\xEC\x26\x6C\xA7\xC2\x31\x10\x78\x16\x7D\x05\xA3\xC2\xA8"
                             "\xF6\x1D\x47\xB3\xAC\x6B\x3D\x87\x99\xEC\x3E\xBC\x7B\x0E\xA3\xD8\x37\x04\x61\x68"
                             "\x80\x89\x2E\xF8\x59\x8B\x0E\x85\xFD\xFC\x11\xF0\x31\x7D\xA6\xA1\x6C\x10\xF0\x43"
                             "\xDD\x38\x5F\x3D\xA0\x87\x90\x90\xF7\xF0\x58\xC4\x71\x9E\xC3\xA8\xF6\x10\x5A\x3C"
                             "\x2A\x2B\xBC\x7B\x0F\x33\xDE\x3D\xA1\x1C\x87\xBE\x40\x89\xAF\x90\x5A\x3C\x2A\x2B"
                             "\x88\x7B\xF8\x2C\x61\xEC\x3A\x8F\x65\x87\x5B\x9C\x7B\x0F\x39\xC2\xF9\xEF\x1E\xD3"
                             "\xD8\xFF\xFC\xF9\xEC\x3C\xCF\x68\x21\x60\xA7\x13\x87\x51\xEC\x2B\x10\x4F\xBF\x78"
                             "\xF6\x1E\x67\xB0\xEC\x3D\x87\x51\xEC\x11\xF8\x3F\xE8\xC0\x41\xC3\xCF\x61\x6F\x53"
                             "\xFF\x58\x48\x9F\xFF\x9F\x3D\x87\xB8\xF7\x1E\xFC\xE1\x7C\xF6\x9E\xCF\x0B\x0C\x37"
                             "\xEF\x1E\xC3\xCC\xF6\x9E\xC3\xB0\x10\x75\xC3\xB0\xFA\x10\xEC\xF5\x5D\x33\xB3\x38"
                             "\xF6\x1E\x67\xD7\x8F\x71\xEE\x05\xAC\x0C\xFA\xF1\xEC\x3C\xCF\xA1\x01\x73\x03\x36"
                             "\x19\x1E\xC3\xCC\xF7\x8F\xAF\x1D\x47\xD7\x8F\x7C\xF7\x1E\xE9\xC2\xF9";
// ++++++++++++++++++++^^^^^^^^^^^^^^^^^^^++++++++++++++++++++
// ++++++++++++++++++++ DO NOT EDIT ABOVE ++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++




// const char HUE_LIGHTS_STATUS_JSON1_SUFFIX[] PROGMEM =
//   "%s\"alert\":\"none\","
//   "\"effect\":\"none\","
//   "\"reachable\":true}";

// const char HUE_LIGHTS_STATUS_JSON2[] PROGMEM =
//   ",\"type\":\"Extended color light\","
//   "\"name\":\"%s\","
//   "\"modelid\":\"%s\","
//   "\"manufacturername\":\"%s\","
//   "\"uniqueid\":\"%s\"}";

// const char HUE_GROUP0_STATUS_JSON[] PROGMEM =
//   "{\"name\":\"Group 0\","
//    "\"lights\":[{l1],"
//    "\"type\":\"LightGroup\","
//    "\"action\":";

// const char HueConfigResponse_JSON[] PROGMEM =
//   "{\"name\":\"Philips hue\","
//    "\"mac\":\"{ma\","
//    "\"dhcp\":true,"
//    "\"ipaddress\":\"{ip\","
//    "\"netmask\":\"{ms\","
//    "\"gateway\":\"{gw\","
//    "\"proxyaddress\":\"none\","
//    "\"proxyport\":0,"
//    "\"bridgeid\":\"{br\","
//    "\"UTC\":\"{dt\","
//    "\"whitelist\":{\"{id\":{"
//      "\"last use date\":\"{dt\","
//      "\"create date\":\"{dt\","
//      "\"name\":\"Remote\"}},"
//    "\"swversion\":\"01041302\","
//    "\"apiversion\":\"1.17.0\","
//    "\"swupdate\":{\"updatestate\":0,\"url\":\"\",\"text\":\"\",\"notify\": false},"
//    "\"linkbutton\":false,"
//    "\"portalservices\":false"
//   "}";
//{"name":"Philips hue","mac":"{ma","dhcp":true,"ipaddress":"{ip","netmask":"{ms","gateway":"{gw","proxyaddress":"none","proxyport":0,"bridgeid":"{br","UTC":"{dt","whitelist":{"{id":{"last use date":"{dt","create date":"{dt","name":"Remote"}},"swversion":"01041302","apiversion":"1.17.0","swupdate":{"updatestate":0,"url":"","text":"","notify": false},"linkbutton":false,"portalservices":false}
const size_t HueConfigResponse_JSON_SIZE = 392;
const char HueConfigResponse_JSON[] PROGMEM = "\x3D\xA7\xB3\xAC\x6B\x3D\x87\x99\xEC\x21\x82\xB4\x2D\x19\xE4\x28\x5B\x3D\x87\x51"
                             "\xEC\x1B\x61\x9E\xC3\xCC\xF6\x1E\xD1\xB6\x7B\x0E\xA3\xD8\x20\xA0\xC6\x1E\xC3\xCE"
                             "\xBE\x2D\x9D\x47\xB3\x46\x58\x82\x7D\xFB\xC7\xB0\xF3\x3D\x87\xB7\x46\x1E\xC3\xA8"
                             "\xF6\x73\xA1\xB7\xE3\x43\xD8\x79\x9E\xC3\xDA\x37\xC7\xB0\xEA\x3D\x83\xD7\x4C\x7E"
                             "\xCC\x8F\x61\xE6\x7B\x0F\x68\xF0\xF9\xEC\x3A\x8F\x60\xCF\xE1\xB0\xC8\x11\x71\x1E"
                             "\xCE\x60\x87\x48\x66\x7E\x8F\x61\xE6\x71\x9D\x47\xB0\x87\x7F\x44\x1E\x7A\x21\xEC"
                             "\x3C\xCF\x61\xED\x1D\xF3\xD8\x75\x1E\xC2\x16\x54\x41\x9E\xC3\xCC\xF6\x1E\xD1\x28"
                             "\xF6\x1D\x47\xB0\x7C\x56\xD3\x0B\x7D\x47\xB0\xF3\x3D\xA7\xB0\xF6\xE8\x87\xB0\xF3"
                             "\x3D\xA7\xB0\x2B\xF5\x21\x7E\x68\x4B\xA6\x08\x98\x30\x7F\x77\x40\x95\x40\x10\xB8"
                             "\x3A\x2F\xB1\xB9\x4C\xF6\x1E\xE3\xDC\x75\x1E\xCF\x0F\x99\xBF\xFB\x73\x8F\x61\xE6"
                             "\x7B\x0E\x38\xF2\x5B\xA3\xD8\x75\x1E\xC2\xB1\x9A\x08\xB5\x0E\x43\xA4\xF1\xD1\x9E"
                             "\xC3\xA8\xF6\x17\x87\xC5\x8C\x04\x1C\xB0\xF6\x9E\xC0\x41\x8D\xEA\xBA\x67\xB0\xF3"
                             "\x38\xCE\xA3\xD8\x42\xFE\x11\xEC\x3C\xCF\x61\xEC\x3A\x8F\x65\x33\x65\x02\x0C\x6E"
                             "\xCA\xD3\x06\x47\xB0\xF3\x46\x2C\x2F\x33\xDC\x75\x1E\xC0\xB7\x8D\x07\x0B\xAA\xCE"
                             "\x3D\x87\x99\x8B\x0B\xCC\xEA\x3D\x83\x33\xF5\x61\x79\xFC\xCF\x43\x7E\x04\x2A\x2B"
                             "\x67\xB8";

// const char HUE_ERROR_JSON[] PROGMEM =
//   "[{\"error\":{\"type\":901,\"address\":\"/\",\"description\":\"Internal Error\"}}]";

/********************************************************************************************/

String GetHueDeviceId(uint16_t id)
{
  String deviceid = WiFi.macAddress();
  deviceid += F(":00:11-");
  if(id<0x10) deviceid += F("0");
  deviceid += String(id,HEX);
  deviceid.toLowerCase();
  return deviceid;  // 5c:cf:7f:13:9f:3d:00:11-01
}

String GetHueUserId(void)
{
  char userid[7];

  snprintf_P(userid, sizeof(userid), PSTR("%03x"), ESP_getChipId());
  return String(userid);
}

void HandleUpnpSetupHue(void)
{
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_HUE_BRIDGE_SETUP));
  String description_xml = Decompress(HUE_DESCRIPTION_XML_COMPRESSED,HUE_DESCRIPTION_XML_SIZE);
  description_xml.replace(F("{x1"), WiFi.localIP().toString());
  description_xml.replace(F("{x2"), HueUuid());
  description_xml.replace(F("{x3"), HueSerialnumber());
  WSSend(200, CT_XML, description_xml);
}

void HueNotImplemented(String *path)
{
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE_API_NOT_IMPLEMENTED " (%s)"), path->c_str());

  WSSend(200, CT_APP_JSON, PSTR("{}"));
}

void HueConfigResponse(String *response)
{
  *response += Decompress(HueConfigResponse_JSON, HueConfigResponse_JSON_SIZE);
  response->replace(F("{ma"), WiFi.macAddress());
  response->replace(F("{ip"), WiFi.localIP().toString());
  response->replace(F("{ms"), WiFi.subnetMask().toString());
  response->replace(F("{gw"), WiFi.gatewayIP().toString());
  response->replace(F("{br"), HueBridgeId());
  response->replace(F("{dt"), GetDateAndTime(DT_UTC));
  response->replace(F("{id"), GetHueUserId());
}

void HueConfig(String *path)
{
  String response = "";
  HueConfigResponse(&response);
  WSSend(200, CT_APP_JSON, response);
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
  UnishoxStrings msg(HUE_LIGHTS);

  snprintf_P(buf, buf_size, PSTR("{\"on\":%s,"), (TasmotaGlobal.power & (1 << (device-1))) ? PSTR("true") : PSTR("false"));
  // Brightness for all devices with PWM
  if ((1 == echo_gen) || (LST_SINGLE <= local_light_subtype)) { // force dimmer for 1st gen Echo
    snprintf_P(buf, buf_size, PSTR("%s\"bri\":%d,"), buf, bri);
  }
  if (LST_COLDWARM <= local_light_subtype) {
    snprintf_P(buf, buf_size, PSTR("%s\"colormode\":\"%s\","), buf, g_gotct ? PSTR("ct") : PSTR("hs"));
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
  snprintf_P(buf, buf_size, msg[HUE_LIGHTS_STATUS_JSON1_SUFFIX], buf);

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
  UnishoxStrings msg(HUE_LIGHTS);
  snprintf_P(buf, buf_size, msg[HUE_LIGHTS_STATUS_JSON2],
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
  response += F("}");
  WSSend(200, CT_APP_JSON, response);
}

void HueAuthentication(String *path)
{
  char response[38];

  snprintf_P(response, sizeof(response), PSTR("[{\"success\":{\"username\":\"%s\"}}]"), GetHueUserId().c_str());
  WSSend(200, CT_APP_JSON, response);
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE " Authentication Result (%s)"), response);
}

#ifdef USE_LIGHT
// refactored to remove code duplicates
void CheckHue(String * response, bool &appending) {
  uint8_t maxhue = (TasmotaGlobal.devices_present > MAX_HUE_DEVICES) ? MAX_HUE_DEVICES : TasmotaGlobal.devices_present;
  for (uint32_t i = 1; i <= maxhue; i++) {
    if (HueActive(i)) {
      if (appending) { *response += ","; }
      *response += F("\"");
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
  UnishoxStrings msg(HUE_LIGHTS);

  if (Webserver->args()) {
    response = "[";

    JsonParser parser((char*) Webserver->arg((Webserver->args())-1).c_str());
    JsonParserObject root = parser.getRootObject();

    JsonParserToken hue_on = root[PSTR("on")];
    if (hue_on) {
      on = hue_on.getBool();
      snprintf_P(buf, buf_size,
                 msg[HUE_RESP_ON],
                 device_id, on ? PSTR("true") : PSTR("false"));

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
                 msg[HUE_RESP_NUM],
                 device_id, PSTR("bri"), bri);
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
      //AddLog(LOG_LEVEL_DEBUG_MORE, "XY RGB (%d %d %d) HS (%d %d)", rr,gg,bb,hue,sat);
      if (resp) { response += ","; }
      snprintf_P(buf, buf_size,
                 msg[HUE_RESP_XY],
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
                 msg[HUE_RESP_NUM],
                 device_id, PSTR("hue"), hue);
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
                 msg[HUE_RESP_NUM],
                 device_id, PSTR("sat"), sat);
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
                 msg[HUE_RESP_NUM],
                 device_id, PSTR("ct"), ct);
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
        AddLog(LOG_LEVEL_DEBUG, PSTR("Settings.shutter_invert: %d"), Settings.shutter_options[device-1] & 1);
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
      response = msg[HUE_ERROR_JSON];
    }
  }
  else {
    response = msg[HUE_ERROR_JSON];
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
    response = F("{");
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
    response += F("}");
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
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("/lights path=%s"), path->c_str());
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
    response = F("{}");
    code = 406;
  }
  exit:
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE " Result (%s)"), response.c_str());
  WSSend(code, CT_APP_JSON, response);
}

void HueGroups(String *path)
{
/*
 * http://tasmota/api/username/groups?1={"name":"Woonkamer","lights":[],"type":"Room","class":"Living room"})
 */
  String response(F("{}"));
  uint8_t maxhue = (TasmotaGlobal.devices_present > MAX_HUE_DEVICES) ? MAX_HUE_DEVICES : TasmotaGlobal.devices_present;
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE " HueGroups (%s)"), path->c_str());

  if (path->endsWith(F("/0"))) {
    UnishoxStrings msg(HUE_LIGHTS);
    response = msg[HUE_GROUP0_STATUS_JSON];
    String lights = F("\"1\"");
    for (uint32_t i = 2; i <= maxhue; i++) {
      lights += F(",\"");
      lights += EncodeLightId(i);
      lights += F("\"");
    }

#ifdef USE_ZIGBEE
    ZigbeeHueGroups(&response);
#endif // USE_ZIGBEE
    response.replace(F("{l1"), lights);
#ifdef USE_LIGHT
    HueLightStatus1(1, &response);
#endif // USE_LIGHT
    response += F("}");
  }

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE " HueGroups Result (%s)"), path->c_str());
  WSSend(200, CT_APP_JSON, response);
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
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE_API " (%s) from %s"), path->c_str(), Webserver->client().remoteIP().toString().c_str());         // HTP: Hue API (//lights/1/state) from 192.168.1.20
  for (args = 0; args < Webserver->args(); args++) {
    String json = Webserver->arg(args);
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP D_HUE_POST_ARGS " (%s)"), json.c_str());  // HTP: Hue POST args ({"on":false})
  }

  UnishoxStrings msg(HUE_API);
  if (path->endsWith(msg[HUE_INVALID])) {}                // Just ignore
  else if (!apilen) HueAuthentication(path);                  // New HUE App setup
  else if (path->endsWith(msg[HUE_ROOT])) HueAuthentication(path);      // New HUE App setup
  else if (path->endsWith(msg[HUE_CONFIG])) HueConfig(path);
  else if (path->indexOf(msg[HUE_LIGHTS_API]) >= 0) HueLights(path);
  else if (path->indexOf(msg[HUE_GROUPS]) >= 0) HueGroups(path);
  else if (path->endsWith(msg[HUE_SCHEDULES])) HueNotImplemented(path);
  else if (path->endsWith(msg[HUE_SENSORS])) HueNotImplemented(path);
  else if (path->endsWith(msg[HUE_SCENES])) HueNotImplemented(path);
  else if (path->endsWith(msg[HUE_RULES])) HueNotImplemented(path);
  else if (path->endsWith(msg[HUE_RESOURCELINKS])) HueNotImplemented(path);
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
