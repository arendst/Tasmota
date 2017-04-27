/*
This example will transmit a universe via Art-Net into the Network.
This example may be copied under the terms of the MIT license, see the LICENSE file for details
*/

#if defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <WiFiUdp.h>
#include <ArtnetWifi.h>

//Wifi settings
const char* ssid = "ssid"; // CHANGE FOR YOUR SETUP
const char* password = "pAsSwOrD"; // CHANGE FOR YOUR SETUP

// Artnet settings
ArtnetWifi artnet;
const int startUniverse = 0; // CHANGE FOR YOUR SETUP most software this is 1, some software send out artnet first universe as 0.
const char host[] = "2.1.1.1"; // CHANGE FOR YOUR SETUP your destination

// connect to wifi – returns true if successful or false if not
boolean ConnectWifi(void)
{
  boolean state = true;
  int i = 0;

  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");
  
  // Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20){
      state = false;
      break;
    }
    i++;
  }
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  
  return state;
}

void setup()
{
  Serial.begin(115200);
  ConnectWifi();
  artnet.begin(host);
  artnet.setLength(3);
  artnet.setUniverse(startUniverse);
}

void loop()
{
  uint8_t i;
  uint8_t j;
  
  // set the first 3 byte to all the same value. A RGB lamp will show a ramp-up white.
  for (j = 0; j < 255; j++) {
    for (i = 0; i < 3; i++) {
      artnet.setByte(i, j);
    }
    // send out the Art-Net DMX data
    artnet.write();
    delay(100);
  }
}
