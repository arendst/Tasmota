/*
 * IRremoteESP8266: IRServer - demonstrates sending IR codes controlled from a webserver
 * An IR LED must be connected to ESP8266 pin 0.
 * Version 0.1 June, 2015
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <IRremoteESP8266.h>
 
const char* ssid = ".....";
const char* password = ".....";
MDNSResponder mdns;

ESP8266WebServer server(80);

IRsend irsend(0);

void handleRoot() {
 server.send(200, "text/html", "<html><head> <title>ESP8266 Demo</title></head><body><h1>Hello from ESP8266, you can send NEC encoded IR signals from here!</h1><p><a href=\"ir?code=16769055\">Send 0xFFE01F</a></p><p><a href=\"ir?code=16429347\">Send 0xFAB123</a></p><p><a href=\"ir?code=16771222\">Send 0xFFE896</a></p></body></html>");
}

void handleIr(){
  for (uint8_t i=0; i<server.args(); i++){
    if(server.argName(i) == "code") 
    {
      unsigned long code = server.arg(i).toInt();
      irsend.sendNEC(code, 32);
    }
  }
  handleRoot();
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
 
void setup(void){
  irsend.begin();
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  
  server.on("/", handleRoot);
  server.on("/ir", handleIr); 
 
  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();
} 
