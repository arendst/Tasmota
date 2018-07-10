#include <ESP8266WiFi.h>
#include <MQTT.h>

void myDataCb(String& topic, String& data);
void myPublishedCb();
void myDisconnectedCb();
void myConnectedCb();

#define CLIENT_ID "client3"
#define TOPIC "/client1/value"


// create MQTT
MQTT myMqtt(CLIENT_ID, "192.168.0.1", 1883);


const char* ssid     = "ssid";
const char* password = "ssid_password";


//
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  Serial.println("Connecting to MQTT server");  

  // setup callbacks
  myMqtt.onConnected(myConnectedCb);
  myMqtt.onDisconnected(myDisconnectedCb);
  myMqtt.onPublished(myPublishedCb);
  myMqtt.onData(myDataCb);
  
  Serial.println("connect mqtt...");
  myMqtt.connect();

  Serial.println("subscribe to topic...");
  myMqtt.subscribe(TOPIC);

  delay(10);
}

//
void loop() {
}


/*
 * 
 */ 
void myConnectedCb()
{
  Serial.println("connected to MQTT server");
}

void myDisconnectedCb()
{
  Serial.println("disconnected. try to reconnect...");
  delay(500);
  myMqtt.connect();
}

void myPublishedCb()
{
  //Serial.println("published.");
}

void myDataCb(String& topic, String& data)
{

  Serial.print(topic);
  Serial.print(": ");
  Serial.println(data);
}



