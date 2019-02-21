/*
 * IRremoteESP8266: IRGCTCPServer - send Global Cache-formatted codes via TCP.
 * An IR emitter must be connected to GPIO pin 4.
 * Version 0.2  May, 2017
 * Copyright 2016 Hisham Khalifa, http://www.hishamkhalifa.com
 * Copyright 2017 David Conran
 *
 * Example command - Samsung TV power toggle: 38000,1,1,170,170,20,63,20,63,20,63,20,20,20,20,20,20,20,20,20,20,20,63,20,63,20,63,20,20,20,20,20,20,20,20,20,20,20,20,20,63,20,20,20,20,20,20,20,20,20,20,20,20,20,63,20,20,20,63,20,63,20,63,20,63,20,63,20,63,20,1798\r\n
 * For more codes, visit: https://irdb.globalcache.com/
 *
 * How to use this program:
 *   1) Update "kSsid" and "kPassword" below for your WIFI network.
 *   2) Compile and upload the sketch to your ESP8266 module.
 *   3) (Optional) Use the serial connection to confirm it started and get the
 *      IP address.
 *   4) From a client machine, connect to port 4998 on the ESP8266, using
 *      'telnet', 'nc' (netcat), 'putty' or similar command, etc.
 *      You may need to install one.
 *   Unix/OSX:
 *     Start a shell. Then type:
 *       telnet <esp8266deviceIPaddress> 4998
 *   Windows:
 *     Start a new CMD window, then type:
 *       telnet <esp8266deviceIPaddress> 4998
 *
 *   5) Enter a Global Cache-formatted code, starting at the frequency,
 *      and then a return/enter at the end. No spaces. e.g.:
 *
 *   38000,1,1,170,170,20,63,20,63,20,63,20,20,20,20,20,20,20,20,20,20,20,63,20,63,20,63,20,20,20,20,20,20,20,20,20,20,20,20,20,63,20,20,20,20,20,20,20,20,20,20,20,20,20,63,20,20,20,63,20,63,20,63,20,63,20,63,20,63,20,1798
 *
 *   To exit the 'telnet' command:
 *     press <control> + <]> at the same time, then press 'q', and then <return>.
 *   or:
 *     <control> + <d> might work.
 *
 * This program will display the ESP's IP address on the serial console, or you
 * can check your wifi router for it's address.
 */

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <ESP8266WiFi.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

const char* kSsid = "...";  // Put your WIFI SSID here.
const char* kPassword = "...";  // Put your WIFI Password here.

WiFiServer server(4998);  // Uses port 4998.
WiFiClient client;

uint16_t *code_array;
#define IR_LED 4  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(IR_LED);  // Set the GPIO to be used to sending the message.

void sendGCString(String str) {
  int16_t index;
  uint16_t count;

  // Find out how many items there are in the string.
  index = -1;
  count = 1;
  do {
    index = str.indexOf(',', index + 1);
    count++;
  } while (index != -1);

  // Now we know how many there are, allocate the memory to store them all.
  code_array = reinterpret_cast<uint16_t*>(malloc(count * sizeof(uint16_t)));
  // Check we malloc'ed successfully.
  if (code_array == NULL) {  // malloc failed, so give up.
    Serial.printf("\nCan't allocate %d bytes. (%d bytes free)\n",
                  count * sizeof(uint16_t), ESP.getFreeHeap());
    Serial.println("Giving up & forcing a reboot.");
    ESP.restart();  // Reboot.
    delay(500);  // Wait for the restart to happen.
    return;  // Should never get here, but just in case.
  }

  // Now convert the strings to integers and place them in code_array.
  count = 0;
  uint16_t start_from = 0;
  do {
    index = str.indexOf(',', start_from);
    code_array[count] = str.substring(start_from, index).toInt();
    start_from = index + 1;
    count++;
  } while (index != -1);

#if SEND_GLOBALCACHE
  irsend.sendGC(code_array, count);  // All done. Send it.
#endif  // SEND_GLOBALCACHE
  free(code_array);  // Free up the memory allocated.
}

void setup() {
  // initialize serial:
  Serial.begin(115200);
  delay(100);
  Serial.println(" ");
  Serial.println("IR TCP Server");
  WiFi.begin(kSsid, kPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(900);
    Serial.print(".");
  }

  server.begin();
  IPAddress myAddress = WiFi.localIP();
  Serial.println(myAddress.toString());
  irsend.begin();
}

void loop() {
  while (!client)
    client = server.available();

  while (!client.connected()) {
    delay(900);
    client = server.available();
  }

  if (client.available()) {
    String ir_code_str = client.readStringUntil('\r');  // Exclusive of \r
    client.readStringUntil('\n');  // Skip new line as well
    client.flush();
    sendGCString(ir_code_str);
  }
}
