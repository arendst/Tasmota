/*
  xdrv_21_wemo.ino - wemo support for Tasmota

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

#if defined(USE_WEBSERVER) && defined(USE_EMULATION) && defined (USE_EMULATION_WEMO)
/*********************************************************************************************\
 * Belkin WeMo emulation
\*********************************************************************************************/

#define XDRV_21           21

const char WEMO_MSEARCH[] PROGMEM =
  "HTTP/1.1 200 OK\r\n"
  "CACHE-CONTROL: max-age=86400\r\n"
  "DATE: Fri, 15 Apr 2016 04:56:29 GMT\r\n"
  "EXT:\r\n"
  "LOCATION: http://%s:80/setup.xml\r\n"
  "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
  "01-NLS: b9200ebb-736d-4b93-bf03-835149d13983\r\n"
  "SERVER: Unspecified, UPnP/1.0, Unspecified\r\n"
  "ST: %s\r\n"                // type1 = urn:Belkin:device:**, type2 = upnp:rootdevice
  "USN: uuid:%s::%s\r\n"      // type1 = urn:Belkin:device:**, type2 = upnp:rootdevice
  "X-User-Agent: redsonic\r\n"
  "\r\n";

String WemoSerialnumber(void)
{
  char serial[16];

  snprintf_P(serial, sizeof(serial), PSTR("201612K%08X"), ESP_getChipId());
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
    char type[24];
    if (1 == echo_type) {              // type1 echo 1g & dot 2g
      strcpy_P(type, URN_BELKIN_DEVICE_CAP);
    } else {                           // type2 echo 2g (echo, plus, show)
      strcpy_P(type, UPNP_ROOTDEVICE);
    }
    char response[400];
    snprintf_P(response, sizeof(response), WEMO_MSEARCH, WiFi.localIP().toString().c_str(), type, WemoUuid().c_str(), type);
    PortUdp.write(response);
    PortUdp.endPacket();
    snprintf_P(message, sizeof(message), PSTR(D_RESPONSE_SENT));
  } else {
    snprintf_P(message, sizeof(message), PSTR(D_FAILED_TO_SEND_RESPONSE));
  }
  // Do not use AddLog_P2 here (interrupt routine) if syslog or mqttlog is enabled. UDP/TCP will force exception 9
  PrepLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPNP D_WEMO " " D_JSON_TYPE " %d, %s " D_TO " %s:%d"),
    echo_type, message, udp_remote_ip.toString().c_str(), udp_remote_port);

  udp_response_mutex = false;
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

/********************************************************************************************/

void HandleUpnpEvent(void)
{
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, PSTR(D_WEMO_BASIC_EVENT));

  char event[500];
  strlcpy(event, Webserver->arg(0).c_str(), sizeof(event));

//  AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("\n%s"), event);

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
      uint8_t device = (light_type) ? devices_present : 1;  // Select either a configured light or relay1
      ExecuteCommandPower(device, power, SRC_WEMO);
    }
  }

  snprintf_P(event, sizeof(event), WEMO_RESPONSE_STATE_SOAP, state, bitRead(power, devices_present -1), state);
  WSSend(200, CT_XML, event);
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
  setup_xml.replace("{x1", SettingsText(SET_FRIENDLYNAME1));
  setup_xml.replace("{x2", WemoUuid());
  setup_xml.replace("{x3", WemoSerialnumber());
  WSSend(200, CT_XML, setup_xml);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv21(uint8_t function)
{
  bool result = false;

  if (devices_present && (EMUL_WEMO == Settings.flag2.emulation)) {
    switch (function) {
      case FUNC_WEB_ADD_HANDLER:
        Webserver->on("/upnp/control/basicevent1", HTTP_POST, HandleUpnpEvent);
        Webserver->on("/eventservice.xml", HandleUpnpService);
        Webserver->on("/metainfoservice.xml", HandleUpnpMetaService);
        Webserver->on("/setup.xml", HandleUpnpSetupWemo);
        break;
    }
  }
  return result;
}

#endif  // USE_WEBSERVER && USE_EMULATION && USE_EMULATION_WEMO
