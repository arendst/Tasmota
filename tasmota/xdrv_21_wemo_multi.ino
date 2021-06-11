/*
  xdrv_21_wemo_multi.ino - multiple wemo support for Tasmota

  Copyright (C) 2021  Magic73, Heiko Krupp and Theo Arends

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

#if defined(USE_WEBSERVER) && defined(USE_EMULATION) && defined(USE_EMULATION_WEMO)
/*********************************************************************************************\
 * Belkin WeMo emulation for multiple devices
\*********************************************************************************************/

#define XDRV_21           21

//#define USE_EMULATION_WEMO_DEBUG

const char WEMO_MSEARCH[] PROGMEM =
    "HTTP/1.1 200 OK\r\n"
    "CACHE-CONTROL: max-age=86400\r\n"
    "DATE: Fri, 15 Apr 2016 04:56:29 GMT\r\n"
    "EXT:\r\n"
    "LOCATION: http://%s:%d/setup.xml\r\n"
    "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
    "01-NLS: b9200ebb-736d-4b93-bf03-835149d13983\r\n"
    "SERVER: Unspecified, UPnP/1.0, Unspecified\r\n"
    "ST: %s\r\n"                // type1 = urn:Belkin:device:**, type2 = upnp:rootdevice
    "USN: uuid:%s::%s\r\n"      // type1 = urn:Belkin:device:**, type2 = upnp:rootdevice
    "X-User-Agent: redsonic\r\n"
    "\r\n";

/*********************************************************************************************\
 * Wemo web server additions
\*********************************************************************************************/

#ifdef USE_UNISHOX_COMPRESSION

//<scpd xmlns="urn:Belkin:service-1-0"><actionList><action><name>SetBinaryState</name><argumentList><argument><retval/><name>BinaryState</name><relatedStateVariable>BinaryState</relatedStateVariable><direction>in</direction></argument></argumentList></action><action><name>GetBinaryState</name><argumentList><argument><retval/><name>BinaryState</name><relatedStateVariable>BinaryState</relatedStateVariable><direction>out</direction></argument></argumentList></action></actionList><serviceStateTable><stateVariable sendEvents="yes"><name>BinaryState</name><dataType>bool</dataType><defaultValue>0</defaultValue></stateVariable><stateVariable sendEvents="yes"><name>level</name><dataType>string</dataType><defaultValue>0</defaultValue></stateVariable></serviceStateTable></scpd>\r\n\r\n
//Successfully compressed from 779 to 249 bytes (-68%)
const size_t WEMO_EVENTSERVICE_XML_SIZE = 779;
const char WEMO_EVENTSERVICE_XML[] PROGMEM = "\x3D\x3C\x18\xC1\x11\xB0\x68\x5D\xE3\xE1\xEC\x17\xFE\x3C\xC8\x73\x08\xD3\x78\xF3"
                            "\xF3\xF9\x9E\x86\xCE\xB3\x90\xEB\x67\xB0\xFC\x3D\x0A\xC3\xAD\xCE\x20\xB7\xD4\x08"
                            "\x72\x0F\xC3\xD3\xAC\x6B\x3F\x0B\xCE\x88\x76\xF5\xFC\xC8\xBD\x57\x4C\xF4\x3B\x3A"
                            "\xC6\xB3\xF0\xF4\xBF\x8F\x0B\x1A\xFA\x81\x0B\x0D\x04\x29\x47\xE1\xE9\xF7\x46\x76"
                            "\x11\xD8\x08\x58\xC0\x27\x62\xBF\x61\x5D\x31\x0B\xD5\x74\xC8\xCE\xFF\xB6\x38\x20"
                            "\x4A\xC1\x01\x42\xF1\xE8\x26\xFD\x82\x0E\xE7\xBC\x7A\x1D\x80\x8B\x28\xF4\x3B\x01"
                            "\x17\x59\x04\x48\xE0\x83\xB9\x1D\x80\x87\xC1\x20\x24\x70\x58\x43\xC0\xDA\xF8\x2C"
                            "\xC1\x74\x0C\x2F\x82\xD0\x42\x8A\x08\x34\x81\x0B\x92\x42\xF5\x5D\x32\xA0\x41\xCE"
                            "\x7C\x08\xFA\x42\xF3\xE1\x09\x99\xBE\xAF\x1F\x0F\x61\x93\xF1\xEC\x05\x5E\x0A\x44"
                            "\xBA\xB2\xA3\x21\x8C\xFC\x1D\x98\x11\xE8\x76\x02\x24\xB3\xD0\x46\x62\xC5\x85\x44"
                            "\x67\x61\x0B\x67\xE1\xC6\x7A\x1D\x84\x09\x13\x0F\x43\xB0\x12\x34\xC0\x60\x5A\xD8"
                            "\x4C\xCD\x84\x09\x9A\xAF\xAB\xFB\xC3\xC0\xC5\x75\x73\xB0\x13\xB8\x6A\x3B\x3C\x18"
                            "\xC1\x0F\xC9\xC2\x91\xBA\x70\xA4\x6E";

//<scpd xmlns="urn:Belkin:service-1-0"><specVersion><major>1</major><minor>0</minor></specVersion><actionList><action><name>GetMetaInfo</name><argumentList><retval /><name>GetMetaInfo</name><relatedStateVariable>MetaInfo</relatedStateVariable><direction>in</direction></argumentList></action></actionList><serviceStateTable><stateVariable sendEvents="yes"><name>MetaInfo</name><dataType>string</dataType><defaultValue>0</defaultValue></stateVariable></serviceStateTable></scpd>\r\n\r\n
//Successfully compressed from 479 to 253 bytes (-47.2%)
const size_t WEMO_METASERVICE_XML_SIZE = 479;
const char WEMO_METASERVICE_XML[] PROGMEM = "\x3D\x3C\x18\xC1\x11\xB0\x68\x5D\xE3\xE1\xEC\x17\xFE\x3C\xC8\x73\x08\xD3\x78\xF3"
                            "\xF3\xF9\x9E\x86\xCE\xB3\x90\xEB\x67\xB0\xFC\x3D\x0B\xC3\x18\x64\x66\xFF\xED\xCE"
                            "\x3F\x0F\x41\xB6\x6B\xCF\x9F\x87\x21\xE8\x76\x10\x20\xC5\x3D\x06\xEF\x67\xCF\xC3"
                            "\x8C\xF4\x3B\x08\x10\x62\x9E\x87\x60\x24\x61\x56\x1D\x6E\x71\x05\xBE\xA0\x43\x90"
                            "\x7E\x1E\x9D\x63\x59\xF8\x43\xCE\x88\x6B\xAB\x2D\xE3\x18\x7A\x1D\x9D\x63\x59\xF8"
                            "\x7A\x5F\xC7\x85\x8D\x7D\x40\x83\x85\x7D\xD1\x9D\x84\x8E\xC0\x55\xC3\x3E\xC2\xBA"
                            "\x62\x17\xAA\xE9\x91\x9D\xFF\x6C\x70\x4C\xFC\x04\x5C\x04\x14\x2D\x9E\x82\x6F\xD8"
                            "\x20\xEC\x9B\xC7\xA1\xD8\x08\xB2\x8F\x43\xB0\x12\x75\xB3\xB0\x10\xF8\x0A\x04\x28"
                            "\xA0\x83\x48\x10\xB8\x74\x2F\x55\xD3\x2A\x2B\x04\x1C\xB7\xC0\x8F\x9E\x2F\x3E\x10"
                            "\x99\x9B\xEA\xF1\xF0\xF6\x19\x3F\x1E\xC0\x42\xE0\x68\x12\xF8\x17\x12\xEA\xCA\x8C"
                            "\x86\x33\xF3\xD5\xFD\xE1\xE3\xD0\xEC\x04\x49\xA7\xA0\x8C\xC5\x8B\x0A\x88\xCE\xC2"
                            "\x16\xCF\xC3\x8C\xF4\x3B\x08\x12\x26\x1E\x87\x60\x24\x69\x67\xE1\xE8\x76\x02\x76"
                            "\xDC\x76\x78\x31\x82\x1F\x93\x85\x23\x74\xE1\x48\xDC";

//<s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/" s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/"><s:Body><u:%cetBinaryStateResponse xmlns:u="urn:Belkin:service:basicevent:1"><BinaryState>%d</BinaryState></u:%cetBinaryStateResponse></s:Body></s:Envelope>\r\n
//Successfully compressed from 282 to 161 bytes (-42.9%)
const size_t WEMO_RESPONSE_STATE_SOAP_SIZE = 282;
const char WEMO_RESPONSE_STATE_SOAP[] PROGMEM = "\x3D\x3C\x79\x93\xE3\x36\x16\x0C\x68\xD8\x34\x2E\xF1\xE7\xE3\xE1\xEC\x15\x54\x30"
                            "\xF3\x3B\x0E\xCF\x06\x29\x8D\xBF\x1D\x0D\x83\x42\xF6\x58\xC3\xA6\x7C\x78\xEC\xF6"
                            "\x58\xC3\xB1\x82\x16\x1C\x76\x1E\xC5\xE3\xCD\xF0\x78\x26\xF0\xF1\x7A\x8C\x82\x60"
                            "\xBF\x8C\x02\x0E\x16\x76\x1E\xC3\xF0\xF4\xF1\xE6\x43\xB0\x43\x23\xF0\xF4\x16\x79"
                            "\x9F\x41\xBA\x21\xDB\xD7\xF3\x22\xF5\x5D\x32\xFB\xF0\xCC\xEF\x02\x1E\xDE\x2C\xF8"
                            "\x7B\x05\xFF\x8F\x32\x1C\xC2\x34\xDE\x3C\xFC\xFE\x67\xA1\xB3\xCC\x75\xFB\x43\x66"
                            "\x6F\xA8\xF3\x39\x0F\x61\xF8\x7A\x10\x23\x63\x67\xE1\xF4\x21\xE8\x76\x02\x3C\xC3"
                            "\xD0\xEC\x05\x4C\xFC\xFC\x3D\x0E\xC0\x43\xD8\xCE\xC0\x45\xE1\xA0\xFC\x9C\x29\x1B"
                            "\x8D";

//<?xml version="1.0"?><root xmlns="urn:Belkin:device-1-0"><device><deviceType>urn:Belkin:device:controllee:1</deviceType><friendlyName>{x1</friendlyName><manufacturer>Belkin International Inc.</manufacturer><modelName>Socket</modelName><modelNumber>3.1415</modelNumber><UDN>uuid:{x2</UDN><serialNumber>{x3</serialNumber><binaryState>0</binaryState><serviceList><service><serviceType>urn:Belkin:service:basicevent:1</serviceType><serviceId>urn:Belkin:serviceId:basicevent1</serviceId><controlURL>/upnp/control/basicevent1</controlURL><eventSubURL>/upnp/event/basicevent1</eventSubURL><SCPDURL>/eventservice.xml</SCPDURL></service><service><serviceType>urn:Belkin:service:metainfo:1</serviceType><serviceId>urn:Belkin:serviceId:metainfo1</serviceId><controlURL>/upnp/control/metainfo1</controlURL><eventSubURL>/upnp/event/metainfo1</eventSubURL><SCPDURL>/metainfoservice.xml</SCPDURL></service></serviceList></device></root>\r\n
//Successfully compressed from 923 to 392 bytes (-57.5%)
const size_t WEMO_SETUP_XML_SIZE = 923;
const char WEMO_SETUP_XML[] PROGMEM = "\x3D\x0E\xD1\xB0\x68\x48\xCD\xFF\xDB\x9C\x7C\x3D\x87\x21\xD1\x9E\xC3\xB4\x7E\x1E"
                            "\x85\xFC\xCA\x46\xC1\xA1\x77\x8F\x87\xB0\x5F\xF8\xF3\x21\xCC\x23\x4D\xE3\xCC\x46"
                            "\x67\xA1\xB3\xAC\xE4\x3A\xD9\xEC\x3F\x0F\x42\x04\x19\x20\x87\x10\xA8\xC8\x63\x3F"
                            "\x01\x33\x07\x3C\xC3\xCE\xAF\xE0\x41\x36\x79\x9C\x87\xA1\xD8\x40\x8D\x83\x9E\x86"
                            "\x3F\xAF\x84\x08\xC8\xBA\xC6\xB3\xF0\xF6\x9B\x0E\x43\xD0\xEC\x20\x48\x9C\x7A\x0D"
                            "\xBE\x16\x62\xC3\xA1\x7F\x7F\x3F\x01\x07\x31\x45\xBD\x4F\xFD\x75\xB9\xD6\x12\x2D"
                            "\xE0\xCE\x87\xA1\xD8\x09\x18\x21\xE8\x37\x04\x61\x17\x58\xD6\x7E\x17\xB0\x33\x47"
                            "\x47\xA1\xD8\x08\xB3\x81\x0A\xC8\xB1\xA3\x9F\xCF\xC3\x96\x74\x99\x34\x81\x0E\xD8"
                            "\x20\xD0\x3D\x08\x59\x08\x5C\x7E\x0B\x17\xA2\x1E\x67\xB4\xD8\x72\x8F\x43\xB0\x88"
                            "\x59\x08\x5C\x7E\x1E\x9E\x7F\xDB\x04\x3B\xA7\xB4\xD8\x72\xCF\x43\xB0\x81\x22\x71"
                            "\xE8\x3B\x7A\xFE\x64\x5E\xAB\xA6\x7E\x1C\x67\xA1\xD8\x40\x8F\x2C\xF4\xF3\xF9\x9E"
                            "\x86\xC8\x2D\xF5\x02\x24\x90\x44\x8A\x09\x7C\x46\x82\x15\x33\xCC\x75\xFB\x43\x66"
                            "\x6F\xA8\xF3\x39\x0F\x43\xB0\x81\x1F\x09\x04\x3C\x58\xB4\x40\x4E\xC5\x0B\x44\x04"
                            "\x6C\x58\x11\x71\x52\xD1\x0F\xC3\xD0\x10\xB8\xE0\x21\x65\xF2\x08\xFC\x3B\x05\x8C"
                            "\xE1\x87\x60\x21\x4D\x3B\x01\x23\x0D\x04\x6C\x08\xF4\x66\x6F\xA8\xBC\x2C\x70\x22"
                            "\xE1\xEC\xCD\xF5\x02\x4E\x1A\x08\xF8\x09\xE8\x45\xE0\xC6\x08\x2F\xE1\x11\xF8\x08"
                            "\x34\x81\x0B\x59\x3A\x1B\x06\x84\x7A\x1D\x80\x87\x5C\x11\x37\x2A\x01\x60\xBC\x34"
                            "\x0D\x75\x7B\xC6\x30\x18\x5F\x0C\xC0\x87\x8A\x03\x02\xE1\x90\x11\xB0\xB0\x5F\xE1"
                            "\x88\x11\xB0\xB0\x51\xE1\x80\x10\xEE\x82\xDF\x0C\x60\x87\x18\x10\x79\x7D\x04\x2E"
                            "\x83\xD1\xF8\x7A\x1D\x9F\xCC\xA3\xF2\x70\xA4\x6E";
#else
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
      "<u:%cetBinaryStateResponse xmlns:u=\"urn:Belkin:service:basicevent:1\">"
        "<BinaryState>%d</BinaryState>"
      "</u:%cetBinaryStateResponse>"
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
#endif

/********************************************************************************************/

class WemoSwitch {

private:
  ESP8266WebServer *_webServer = NULL;
  int _localPort = 80;
  int _deviceId;

  String WemoSerialnumber(void) {
    char serial[20];

    char index[8] = { 0 };
    if (_deviceId > 1) {  // Keep backward compatibility
      snprintf_P(index, sizeof(index), PSTR("%02X"), _deviceId);
    }
    snprintf_P(serial, sizeof(serial), PSTR("201612K%08X%s"), ESP_getChipId(), index);
    return String(serial);
  }

  String WemoUuid(void) {
    char uuid[32];

    snprintf_P(uuid, sizeof(uuid), PSTR("Socket-1_0-%s"), WemoSerialnumber().c_str());
    return String(uuid);
  }

public:
  WemoSwitch(int deviceId, ESP8266WebServer *webServer) {
    _deviceId = deviceId;
    _webServer = webServer;
#ifdef USE_EMULATION_WEMO_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR("WMO: Device #%d listening on port %d"), _deviceId, _localPort);
#endif
  }

  WemoSwitch(int deviceId, int localPort) {
    _deviceId = deviceId;
    _localPort = localPort;
#ifdef USE_EMULATION_WEMO_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR("WMO: Device #%d listening on port %d"), _deviceId, _localPort);
#endif
    _webServer = new ESP8266WebServer(_localPort);

    _webServer->begin();
  }

  ~WemoSwitch() {
    if (_deviceId > 1) {
      _webServer->close();
    }
  }

  void WemoRespondToMSearch(int echo_type) {
//    AddLog(LOG_LEVEL_DEBUG, PSTR("WMO: WemoRespondToMSearch device #%d: %d"), _deviceId, echo_type);

    char message[TOPSZ];

    if (PortUdp.beginPacket(udp_remote_ip, udp_remote_port)) {
      char type[24];
      if (1 == echo_type) {              // type1 echo 1g & dot 2g
        strcpy_P(type, URN_BELKIN_DEVICE_CAP);
      } else {                           // type2 echo 2g (echo, plus, show)
        strcpy_P(type, UPNP_ROOTDEVICE);
      }
      char response[400];
      snprintf_P(response, sizeof(response), WEMO_MSEARCH, WiFi.localIP().toString().c_str(), _localPort, type, WemoUuid().c_str(), type);
      PortUdp.write(response);
      PortUdp.endPacket();

//      AddLog(LOG_LEVEL_DEBUG, PSTR("WMO: Sending packet device %d: %s"), _deviceId, response);

      snprintf_P(message, sizeof(message), PSTR(D_RESPONSE_SENT));
    } else {
      snprintf_P(message, sizeof(message), PSTR(D_FAILED_TO_SEND_RESPONSE));
    }
    AddLog(LOG_LEVEL_DEBUG, PSTR("WMO: WeMo Type %d, %s to %s:%d"),
      echo_type, message, udp_remote_ip.toString().c_str(), udp_remote_port);
  }

  void HandleServerLoop() {
    if (_webServer != NULL &&  _deviceId > 1) {
      _webServer->handleClient();
      delay(1);
    }
  }

private:
  void InternalWSSend(int code, int ctype, const String &content) {
    char ct[25]; // strlen("application/octet-stream") +1 = Longest Content type string
    _webServer->send(code, GetTextIndexed(ct, sizeof(ct), ctype, kContentTypes), content);
  }

  void LogUpnpWithClient(const char *msg) {
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP "%s from %s"), msg, _webServer->client().remoteIP().toString().c_str());
  }

  void HandleUpnpEvent() {
    LogUpnpWithClient(PSTR(D_WEMO_BASIC_EVENT));

    char event[500];
    strlcpy(event, _webServer->arg(0).c_str(), sizeof(event));

#ifdef USE_EMULATION_WEMO_DEBUG
    //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("\n%s"), event);
    AddLog(LOG_LEVEL_DEBUG, PSTR("WMO: HandleUpnpEvent for deviceId %d: %s"), _deviceId, event);
#endif

    //differentiate get and set state
    char state = 'G';
    if (strstr_P(event, PSTR("SetBinaryState")) != nullptr) {
      state = 'S';
      uint8_t power = POWER_TOGGLE;
      if (strstr_P(event, PSTR("State>1</Binary")) != nullptr) {
        power = POWER_ON;
      }
      else if (strstr_P(event, PSTR("State>0</Binary")) != nullptr) {
        power = POWER_OFF;
      }
      if (power != POWER_TOGGLE) {
        //uint8_t device = (TasmotaGlobal.light_type) ? TasmotaGlobal.devices_present : 1;  // Select either a configured light or relay1
        ExecuteCommandPower(_deviceId, power, SRC_WEMO);
      }
    }

#ifdef USE_UNISHOX_COMPRESSION
    String setup_xml = Decompress(WEMO_RESPONSE_STATE_SOAP, WEMO_RESPONSE_STATE_SOAP_SIZE);
#else
    String setup_xml = FPSTR(WEMO_RESPONSE_STATE_SOAP);
#endif

    snprintf_P(event, sizeof(event), setup_xml.c_str(), state, bitRead(TasmotaGlobal.power, _deviceId - 1), state);
    InternalWSSend(200, CT_XML, event);
  }

  void HandleUpnpService(void) {
#ifdef USE_EMULATION_WEMO_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR("WMO: HandleUpnpService"));
#endif
    LogUpnpWithClient(PSTR(D_WEMO_EVENT_SERVICE));

#ifdef USE_UNISHOX_COMPRESSION
    InternalWSSend(200, CT_PLAIN, Decompress(WEMO_EVENTSERVICE_XML, WEMO_EVENTSERVICE_XML_SIZE));
#else
    InternalWSSend(200, CT_PLAIN, FPSTR(WEMO_EVENTSERVICE_XML));
#endif
  }

  void HandleUpnpMetaService(void) {
#ifdef USE_EMULATION_WEMO_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR("WMO: HandleUpnpMetaService"));
#endif
    LogUpnpWithClient(PSTR(D_WEMO_META_SERVICE));

#ifdef USE_UNISHOX_COMPRESSION
    InternalWSSend(200, CT_PLAIN, Decompress(WEMO_METASERVICE_XML, WEMO_METASERVICE_XML_SIZE));
#else
    InternalWSSend(200, CT_PLAIN, FPSTR(WEMO_METASERVICE_XML));
#endif
  }

  void HandleUpnpSetupWemo(void) {
    LogUpnpWithClient(PSTR(D_WEMO_SETUP));

#ifdef USE_UNISHOX_COMPRESSION
    String setup_xml = Decompress(WEMO_SETUP_XML, WEMO_SETUP_XML_SIZE);
#else
    String setup_xml = FPSTR(WEMO_SETUP_XML);
#endif
    setup_xml.replace("{x1", SettingsText(SET_FRIENDLYNAME1 + (_deviceId - 1)));
    setup_xml.replace("{x2", WemoUuid());
    setup_xml.replace("{x3", WemoSerialnumber());
    InternalWSSend(200, CT_XML, setup_xml);
#ifdef USE_EMULATION_WEMO_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR("WMO: Sending device #%d: %s"), _deviceId, setup_xml.c_str());
#endif
  }

public:
  void RegisterHandlers(void) {
#ifdef USE_EMULATION_WEMO_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR("WMO: Register device #%d"), _deviceId);
#endif
    _webServer->on(F("/upnp/control/basicevent1"), [&]() { HandleUpnpEvent(); });
    _webServer->on(F("/eventservice.xml"), [&]() { HandleUpnpService(); });
    _webServer->on(F("/metainfoservice.xml"), [&]() { HandleUpnpMetaService(); });
    _webServer->on(F("/setup.xml"), [&]() { HandleUpnpSetupWemo(); });
  }
};

WemoSwitch *wemoDevice[MAX_FRIENDLYNAMES] = {};
int numOfWemoSwitch = 0;

void WemoRespondToMSearch(int echo_type) {
  for (uint32_t i = 0; i < numOfWemoSwitch; i++) {
    wemoDevice[i]->WemoRespondToMSearch(echo_type);
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv21(uint8_t function)
{
  bool result = false;

  if (TasmotaGlobal.devices_present && (EMUL_WEMO == Settings->flag2.emulation)) {
    switch (function) {
      case FUNC_LOOP:
        for (uint32_t i = 1; i < numOfWemoSwitch; i++) { // Handle devices web server
          wemoDevice[i]->HandleServerLoop();
        }
        break;
      case FUNC_WEB_ADD_HANDLER:
#ifdef USE_EMULATION_WEMO_DEBUG
        AddLog(LOG_LEVEL_DEBUG, PSTR("WMO: Adding handlers for %d devices"), TasmotaGlobal.devices_present);
#endif
        // For the first device use the current webserver, for the others.. create a new one listening on a different PortUdp
        wemoDevice[numOfWemoSwitch] = new WemoSwitch(1, Webserver);
        wemoDevice[numOfWemoSwitch]->RegisterHandlers();
        numOfWemoSwitch++;

        for (uint32_t i = 1; i < TasmotaGlobal.devices_present; i++) {
          wemoDevice[numOfWemoSwitch] = new WemoSwitch(i + 1, 8080 + i);
          wemoDevice[numOfWemoSwitch]->RegisterHandlers();
          numOfWemoSwitch++;
        }
        break;
    }
  }
  return result;
}

#endif // USE_WEBSERVER && USE_EMULATION && USE_EMULATION_WEMO