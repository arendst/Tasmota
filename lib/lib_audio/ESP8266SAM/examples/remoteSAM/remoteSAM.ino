// In a webbrowser go to http://sam.local/say/{message} to make it speak
// ex: http://sam.local/say/hello world

#include <Arduino.h>

#if !defined(ESP8266)
#error This example is only for the ESP8266
#endif

#include <ESP8266SAM.h>
#include "AudioOutputI2SNoDAC.h"

#include <ESP8266mDNS.h>
#include <ESP8266NetBIOS.h>
#include <ESP8266SSDP.h> //Library for SSDP (Show ESP in Network on Windows)
#include <ESP8266WebServer.h> //Library for WebServer
#include <WiFiManager.h>
#include <uri/UriBraces.h>

AudioOutputI2SNoDAC *out = NULL;

ESP8266WebServer server(80); //Web Server on port 80
WiFiManager wifiManager;
const char* NAME = "SAM";

void setup()
{
  Serial.begin(115200);
  out = new AudioOutputI2SNoDAC();
  out->begin();

  wifiManager.autoConnect(NAME);

  MDNS.begin(NAME);
  MDNS.addService("http", "tcp", 80);
  NBNS.begin(NAME);
  
  server.on(UriBraces("/say/{}"), []() {
    String message_encoded = server.pathArg(0);
    String message_decoded = urldecode(message_encoded);
    const char* message = message_decoded.c_str();

    Serial.println(message_encoded);
    Serial.println(message_decoded);
    Serial.println(message);
    
    ESP8266SAM *sam = new ESP8266SAM;
    sam->Say(out, message);
    delete sam;    
    server.send(200, "text/plain", "OK");
  });

  server.on("/description.xml", HTTP_GET, []() {
    SSDP.schema(server.client());
  });
  server.begin();
  ssdp();
}

void ssdp() {
  //Simple Service Discovery Protocol : Display ESP in Windows Network Tab
  SSDP.setSchemaURL("description.xml");
  SSDP.setHTTPPort(80);
  SSDP.setName(NAME);
  SSDP.setDeviceType("upnp: rootdevice");
  SSDP.setSerialNumber("000000000001");
  SSDP.setURL("/say/connected");
  SSDP.setModelName("ESP8266SAM");
  SSDP.setModelNumber("0000000000001");
  SSDP.setModelURL("https://github.com/earlephilhower/ESP8266SAM");
  SSDP.setManufacturer("earlephilhower");
  SSDP.setManufacturerURL("https://github.com/earlephilhower/");
  SSDP.begin();
}

void loop() {
  server.handleClient();
}

char* string2char(String command) {
  if (command.length() != 0) {
    char *p = const_cast<char*>(command.c_str());
    return p;
  } else {
    return "";
  }
}

unsigned char h2int(char c) {
  if (c >= '0' && c <= '9') {
    return ((unsigned char)c - '0');
  }
  if (c >= 'a' && c <= 'f') {
    return ((unsigned char)c - 'a' + 10);
  }
  if (c >= 'A' && c <= 'F') {
    return ((unsigned char)c - 'A' + 10);
  }
  return (0);
}

String urldecode(String str)
{

  String encodedString = "";
  char c;
  char code0;
  char code1;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (c == '+') {
      encodedString += ' ';
    } else if (c == '%') {
      i++;
      code0 = str.charAt(i);
      i++;
      code1 = str.charAt(i);
      c = (h2int(code0) << 4) | h2int(code1);
      encodedString += c;
    } else {

      encodedString += c;
    }

    yield();
  }

  return encodedString;
}
