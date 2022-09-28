/*
 Long message ESP8266 MQTT example

 This sketch demonstrates sending arbitrarily large messages in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "greenBottles/#", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the sub-topic is a number, it publishes a "greenBottles/lyrics" message
    with a payload consisting of the lyrics to "10 green bottles", replacing
    10 with the number given in the sub-topic.

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "........";
const char* password = "........";
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Find out how many bottles we should generate lyrics for
  String topicStr(topic);
  int bottleCount = 0; // assume no bottles unless we correctly parse a value from the topic
  if (topicStr.indexOf('/') >= 0) {
    // The topic includes a '/', we'll try to read the number of bottles from just after that
    topicStr.remove(0, topicStr.indexOf('/')+1);
    // Now see if there's a number of bottles after the '/'
    bottleCount = topicStr.toInt();
  }

  if (bottleCount > 0) {
    // Work out how big our resulting message will be
    int msgLen = 0;
    for (int i = bottleCount; i > 0; i--) {
      String numBottles(i);
      msgLen += 2*numBottles.length();
      if (i == 1) {
        msgLen += 2*String(" green bottle, standing on the wall\n").length();
      } else {
        msgLen += 2*String(" green bottles, standing on the wall\n").length();
      }
      msgLen += String("And if one green bottle should accidentally fall\nThere'll be ").length();
      switch (i) {
      case 1:
        msgLen += String("no green bottles, standing on the wall\n\n").length();
        break;
      case 2:
        msgLen += String("1 green bottle, standing on the wall\n\n").length();
        break;
      default:
        numBottles = i-1;
        msgLen += numBottles.length();
        msgLen += String(" green bottles, standing on the wall\n\n").length();
        break;
      };
    }

    // Now we can start to publish the message
    client.beginPublish("greenBottles/lyrics", msgLen, false);
    for (int i = bottleCount; i > 0; i--) {
      for (int j = 0; j < 2; j++) {
        client.print(i);
        if (i == 1) {
          client.print(" green bottle, standing on the wall\n");
        } else {
          client.print(" green bottles, standing on the wall\n");
        }
      }
      client.print("And if one green bottle should accidentally fall\nThere'll be ");
      switch (i) {
      case 1:
        client.print("no green bottles, standing on the wall\n\n");
        break;
      case 2:
        client.print("1 green bottle, standing on the wall\n\n");
        break;
      default:
        client.print(i-1);
        client.print(" green bottles, standing on the wall\n\n");
        break;
      };
    }
    // Now we're done!
    client.endPublish();
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("greenBottles/#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
