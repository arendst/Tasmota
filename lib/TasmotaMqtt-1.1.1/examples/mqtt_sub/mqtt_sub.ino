#include <ESP8266WiFi.h>
#include <TasmotaMqtt.h>

// This needs testing

void myDataCb(char* topic, uint8_t* data, unsigned int data_len);
void myPublishedCb();
void myDisconnectedCb();
void myConnectedCb();

#define CLIENT_ID "client3"
#define TOPIC "/client1/value"

// create MQTT
TasmotaMqtt myMqtt();

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

  myMqtt.InitConnection("192.168.0.1", 1883);
  myMqtt.InitClient(CLIENT_ID, "", "");
  myMqtt.InitLWT("/lwt", "offline");

  // setup callbacks
  myMqtt.OnConnected(myConnectedCb);
  myMqtt.OnDisconnected(myDisconnectedCb);
  myMqtt.OnPublished(myPublishedCb);
  myMqtt.OnData(myDataCb);

  Serial.println("connect mqtt...");
  myMqtt.Connect();

  Serial.println("subscribe to topic...");
  myMqtt.Subscribe(TOPIC);

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
  myMqtt.Connect();
}

void myPublishedCb()
{
  //Serial.println("published.");
}

void myDataCb(char* topic, uint8_t* data, unsigned int data_len)
{
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(data);
}



