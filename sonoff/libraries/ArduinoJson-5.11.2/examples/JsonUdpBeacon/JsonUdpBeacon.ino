// Send a JSON object on UDP at regular interval
//
// You can easily test this program with netcat:
// $ nc -ulp 8888
//
// by Benoit Blanchon, MIT License 2015-2017

#include <ArduinoJson.h>
#include <Ethernet.h>
#include <SPI.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress localIp(192, 168, 0, 177);
IPAddress remoteIp(192, 168, 0, 109);
unsigned int remotePort = 8888;
unsigned localPort = 8888;
EthernetUDP udp;

JsonObject& buildJson(JsonBuffer& jsonBuffer) {
  JsonObject& root = jsonBuffer.createObject();

  JsonArray& analogValues = root.createNestedArray("analog");
  for (int pin = 0; pin < 6; pin++) {
    int value = analogRead(pin);
    analogValues.add(value);
  }

  JsonArray& digitalValues = root.createNestedArray("digital");
  for (int pin = 0; pin < 14; pin++) {
    int value = digitalRead(pin);
    digitalValues.add(value);
  }

  return root;
}

void sendJson(JsonObject& json) {
  udp.beginPacket(remoteIp, remotePort);
  json.printTo(udp);
  udp.println();
  udp.endPacket();
}

void setup() {
  Ethernet.begin(mac, localIp);
  udp.begin(localPort);
}

void loop() {
  delay(1000);

  // Use https://bblanchon.github.io/ArduinoJson/assistant/ to
  // compute the right size for the buffer
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject& json = buildJson(jsonBuffer);
  sendJson(json);
}
