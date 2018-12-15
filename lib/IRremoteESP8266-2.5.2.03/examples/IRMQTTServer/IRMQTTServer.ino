/*
 * Send & receive arbitrary IR codes via a web server or MQTT.
 * Copyright David Conran 2016, 2017, 2018
 *
 * NOTE: An IR LED circuit *MUST* be connected to ESP8266 GPIO4 (D2) if
 *       you want to send IR messages. See IR_LED below.
 *       A compatible IR RX modules *MUST* be connected to ESP8266 GPIO14 (D5)
 *       if you want to capture & decode IR nessages. See IR_RX below.
 *
 * WARN: This is very advanced & complicated example code. Not for beginners.
 *       You are strongly suggested to try & look at other example code first.
 *
 * # Instructions
 *
 * ## Before First Boot (i.e. Compile time)
 * - Either:
 *   o Set the MQTT_SERVER define below to the address of your MQTT server.
 *   or
 *   o Disable MQTT by commenting out the line "#define MQTT_ENABLE" down below.
 *
 * - Arduino IDE:
 *   o Install the following libraries via Library Manager
 *     - WiFiManager (https://github.com/tzapu/WiFiManager) (Version >= 0.14)
 *     - PubSubClient (https://pubsubclient.knolleary.net/)
 *   o You MUST change <PubSubClient.h> to have the following (or larger) value:
 *     #define MQTT_MAX_PACKET_SIZE 512
 * - PlatformIO IDE:
 *     If you are using PlatformIO, this should already been done for you in
 *     the accompanying platformio.ini file.
 *
 * ## First Boot (Initial setup)
 * The ESP8266 board will boot into the WiFiManager's AP mode.
 * i.e. It will create a WiFi Access Point with a SSID like: "ESP123456" etc.
 * Connect to that SSID. Then point your browser to http://192.168.4.1/ and
 * configure the ESP8266 to connect to your desired WiFi network.
 * It will remember the new WiFi connection details on next boot.
 * More information can be found here:
 *   https://github.com/tzapu/WiFiManager#how-it-works
 *
 * If you need to reset the WiFi settings, visit:
 *   http://<your_esp8266's_ip_address>/reset
 *
 * ## Normal Use (After setup)
 * Enter 'http://<your_esp8266's_ip_address/' in your browser & follow the
 * instructions there to send IR codes via HTTP/HTML.
 * You can send URLs like the following, with similar data type limitations as
 * the MQTT formating in the next section. e.g:
 *   http://<your_esp8266's_ip_address>/ir?type=7&code=E0E09966
 *   http://<your_esp8266's_ip_address>/ir?type=4&code=0xf50&bits=12
 *   http://<your_esp8266's_ip_address>/ir?code=C1A2E21D&repeats=8&type=19
 *   http://<your_esp8266's_ip_address>/ir?type=31&code=40000,1,1,96,24,24,24,48,24,24,24,24,24,48,24,24,24,24,24,48,24,24,24,24,24,24,24,24,1058
 *   http://<your_esp8266's_ip_address>/ir?type=18&code=190B8050000000E0190B8070000010f0
 *   http://<your_esp8266's_ip_address>/ir?repeats=1&type=25&code=0000,006E,0022,0002,0155,00AA,0015,0040,0015,0040,0015,0015,0015,0015,0015,0015,0015,0015,0015,0015,0015,0040,0015,0040,0015,0015,0015,0040,0015,0015,0015,0015,0015,0015,0015,0040,0015,0015,0015,0015,0015,0040,0015,0040,0015,0015,0015,0015,0015,0015,0015,0015,0015,0015,0015,0040,0015,0015,0015,0015,0015,0040,0015,0040,0015,0040,0015,0040,0015,0040,0015,0640,0155,0055,0015,0E40
 *
 * or
 *
 * Send a MQTT message to the topic 'ir_server/send' using the following
 * format (Order is important):
 *   protocol_num,hexcode  e.g. 7,E0E09966 which is Samsung(7), Power On code,
 *                              default bit size, default nr. of repeats.
 *   protocol_num,hexcode,bits  e.g. 4,f50,12 which is Sony(4), Power Off code,
 *                               12 bits & default nr. of repeats.
 *   protocol_num,hexcode,bits,repeats  e.g. 19,C1A2E21D,0,8 which is
 *                                      Sherwood(19), Vol Up, default bit size &
 *                                      repeated 8 times.
 *   30,frequency,raw_string  e.g. 30,38000,9000,4500,500,1500,500,750,500,750
 *                             Raw (30) @ 38kHz with a raw code of "9000,4500,500,1500,500,750,500,750"
 *   31,code_string  e.g. 31,40000,1,1,96,24,24,24,48,24,24,24,24,24,48,24,24,24,24,24,48,24,24,24,24,24,24,24,24,1058
 *                        GlobalCache (31) & "40000,1,1,96,..." (Sony Vol Up)
 *   25,Rrepeats,hex_code_string  e.g. 25,R1,0000,006E,0022,0002,0155,00AA,0015,0040,0015,0040,0015,0015,0015,0015,0015,0015,0015,0015,0015,0015,0015,0040,0015,0040,0015,0015,0015,0040,0015,0015,0015,0015,0015,0015,0015,0040,0015,0015,0015,0015,0015,0040,0015,0040,0015,0015,0015,0015,0015,0015,0015,0015,0015,0015,0015,0040,0015,0015,0015,0015,0015,0040,0015,0040,0015,0040,0015,0040,0015,0040,0015,0640,0155,0055,0015,0E40
 *                               Pronto (25), 1 repeat, & "0000 006E 0022 0002 ..." (Sherwood Amp Tape Input)
 *   ac_protocol_num,really_long_hexcode  e.g. 18,190B8050000000E0190B8070000010F0
 *                           Kelvinator (18) Air Con on, Low Fan, 25 deg etc.
 *                           NOTE: Ensure you zero-pad to the correct number of
 *                                 digits for the bit/byte size you want to send
 *                                 as some A/C units have units have different
 *                                 sized messages. e.g. Fujitsu A/C units.
 *   In short:
 *     No spaces after/before commas.
 *     Values are comma separated.
 *     The first value is always in Decimal.
 *     For simple protocols, the next value (hexcode) is always hexadecimal.
 *     The optional bit size is in decimal.
 *
 *   Unix command line usage example:
 *     # Install a MQTT client
 *     $ sudo apt install mosquitto-clients
 *     # Send a 32-bit NEC code of 0x1234abcd via MQTT.
 *     $ mosquitto_pub -h 10.20.0.253 -t ir_server/send -m '3,1234abcd,32'
 *
 * This server will send (back) what ever IR message it just transmitted to
 * the MQTT topic 'ir_server/sent' to confirm it has been performed. This works
 * for messages requested via MQTT or via HTTP.
 * Note: Other status messages are also sent to 'ir_server/sent' from time to
 * time.
 *   Unix command line usage example:
 *     # Listen to MQTT acknowledgements.
 *     $ mosquitto_sub -h 10.20.0.253 -t ir_server/sent
 *
 * Incoming IR messages (from an IR remote control) will be transmitted to
 * the MQTT topic 'ir_server/received'. The MQTT message will be formatted
 * similar to what is required to for the 'sent' topic.
 * e.g. "3,C1A2F00F,32" (Protocol,Value,Bits) for simple codes
 *   or "18,110B805000000060110B807000001070" (Protocol,Value) for complex codes
 * Note: If the protocol is listed as -1, then that is an UNKNOWN IR protocol.
 *       You can't use that to recreate/resend an IR message. It's only for
 *       matching purposes and shouldn't be trusted.
 *
 *   Unix command line usage example:
 *     # Listen via MQTT for IR messages captured by this server.
 *     $ mosquitto_sub -h 10.20.0.253 -t ir_server/received
 *
 * If DEBUG is turned on, there is additional information printed on the Serial
 * Port.
 *
 * ## Updates
 * You can upload new firmware over the air (OTA) via the form on the device's
 * main page. No need to connect to the device again via USB. \o/
 * Your WiFi settings should be remembered between updates. \o/ \o/
 *
 * Copyright Notice:
 *   Code for this has been borrowed from lots of other OpenSource projects &
 *   resources. I'm *NOT* claiming complete Copyright ownership of all the code.
 *   Likewise, feel free to borrow from this as much as you want.
 */

#define MQTT_ENABLE  // Comment this out if you don't want to use MQTT at all.

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ESP8266mDNS.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRutils.h>
#ifdef MQTT_ENABLE
// --------------------------------------------------------------------
// * * * IMPORTANT * * *
// You must change <PubSubClient.h> to have the following value.
// #define MQTT_MAX_PACKET_SIZE 512
// --------------------------------------------------------------------
#include <PubSubClient.h>
#endif  // MQTT_ENABLE
#include <algorithm>
#include <string>

// Configuration parameters
// GPIO the IR LED is connected to/controlled by. GPIO 4 = D2.
#define IR_LED 4
// define IR_LED 3  // For an ESP-01 we suggest you use RX/GPIO3/Pin 7.
//
// GPIO the IR RX module is connected to/controlled by. GPIO 14 = D5.
// Comment this out to disable receiving/decoding IR messages entirely.
#define IR_RX 14
const uint16_t kHttpPort = 80;  // The TCP port the HTTP server is listening on.
// Name of the device you want in mDNS.
// NOTE: Changing this will change the MQTT path too unless you override it
//       via MQTTprefix below.
#define HOSTNAME "ir_server"

// We obtain our network config via DHCP by default but allow an easy way to
// use a static IP config.
#define USE_STATIC_IP false  // Change to 'true' if you don't want to use DHCP.
#if USE_STATIC_IP
const IPAddress kIPAddress = IPAddress(10, 0, 1, 78);
const IPAddress kGateway = IPAddress(10, 0, 1, 1);
const IPAddress kSubnetMask = IPAddress(255, 255, 255, 0);
#endif  // USE_STATIC_IP

#ifdef MQTT_ENABLE
// Address of your MQTT server.
#define MQTT_SERVER "10.20.0.253"  // <=- CHANGE ME
const uint16_t kMqttPort = 1883;  // Default port used by MQTT servers.
// Set if your MQTT server requires a Username & Password to connect.
const char* mqtt_user = "";
const char* mqtt_password = "";
const uint32_t kMqttReconnectTime = 5000;  // Delay(ms) between reconnect tries.

#define MQTTprefix HOSTNAME  // Change this if you want the MQTT topic to be
                             // independent of the hostname.
#define MQTTack MQTTprefix "/sent"  // Topic we send back acknowledgements on
#define MQTTcommand MQTTprefix "/send"  // Topic we get new commands from.
#define MQTTrecv MQTTprefix "/received"  // Topic we send received IRs to.
#endif  // MQTT_ENABLE

// HTML arguments we will parse for IR code information.
#define argType "type"
#define argData "code"
#define argBits "bits"
#define argRepeat "repeats"
// Let's use a larger than normal buffer so we can handle AirCon remote codes.
const uint16_t kCaptureBufferSize = 1024;
#if DECODE_AC
// Some A/C units have gaps in their protocols of ~40ms. e.g. Kelvinator
// A value this large may swallow repeats of some protocols
const uint8_t kCaptureTimeout = 50;
#else  // DECODE_AC
// Suits most messages, while not swallowing many repeats.
const uint8_t kCaptureTimeout = 15;
#endif  // DECODE_AC
// Ignore unknown messages with <10 pulses
const uint16_t kMinUnknownSize = 20;

#define _MY_VERSION_ "v0.7.0"

// Disable debug output if any of the IR pins are on the TX (D1) pin.
#if (IR_LED != 1 && IR_RX != 1)
#undef DEBUG
#define DEBUG true  // Change to 'false' to disable all serial output.
#else
#undef DEBUG
#define DEBUG false
#endif
// NOTE: Make sure you set your Serial Monitor to the same speed.
#define BAUD_RATE 115200  // Serial port Baud rate.

// Globals
ESP8266WebServer server(kHttpPort);
IRsend irsend = IRsend(IR_LED);
#ifdef IR_RX
IRrecv irrecv(IR_RX, kCaptureBufferSize, kCaptureTimeout, true);
decode_results capture;  // Somewhere to store inbound IR messages.
#endif  // IR_RX
MDNSResponder mdns;
WiFiClient espClient;
WiFiManager wifiManager;

uint16_t *codeArray;
uint32_t lastReconnectAttempt = 0;  // MQTT last attempt reconnection number
bool boot = true;
bool ir_lock = false;  // Primitive locking for gating the IR LED.
uint32_t sendReqCounter = 0;
bool lastSendSucceeded = false;  // Store the success status of the last send.
uint32_t lastSendTime = 0;
int8_t offset;  // The calculated period offset for this chip and library.

#ifdef MQTT_ENABLE
String lastMqttCmd = "None";
uint32_t lastMqttCmdTime = 0;
uint32_t lastConnectedTime = 0;
uint32_t lastDisconnectedTime = 0;
uint32_t mqttDisconnectCounter = 0;
bool wasConnected = true;
#ifdef IR_RX
String lastIrReceived = "None";
uint32_t lastIrReceivedTime = 0;
uint32_t irRecvCounter = 0;
#endif  // IR_RX


// MQTT client parameters
void callback(char* topic, byte* payload, unsigned int length);
PubSubClient mqtt_client(MQTT_SERVER, kMqttPort, callback, espClient);
// Create a unique MQTT client id.
String mqtt_clientid = MQTTprefix + String(ESP.getChipId(), HEX);
#endif  // MQTT_ENABLE

// Debug messages get sent to the serial port.
void debug(String str) {
#ifdef DEBUG
  uint32_t now = millis();
  Serial.printf("%07u.%03u: %s\n", now / 1000, now % 1000, str.c_str());
#endif  // DEBUG
}

String timeSince(uint32_t const start) {
  if (start == 0)
    return "Never";
  uint32_t diff = 0;
  uint32_t now = millis();
  if (start < now)
    diff = now - start;
  else
    diff = UINT32_MAX - start + now;
  diff /= 1000;  // Convert to seconds.
  if (diff == 0)  return "Now";

  // Note: millis() can only count up to 45 days, so uint8_t is safe.
  uint8_t days = diff / (60 * 60 * 24);
  uint8_t hours = (diff / (60 * 60)) % 24;
  uint8_t minutes = (diff / 60) % 60;
  uint8_t seconds = diff % 60;

  String result = "";
  if (days)
    result += String(days) + " day";
  if (days > 1)  result += "s";
  if (hours)
    result += " " + String(hours) + " hour";
  if (hours > 1)  result += "s";
  if (minutes)
    result += " " + String(minutes) + " minute";
  if (minutes > 1)  result += "s";
  if (seconds)
    result += " " + String(seconds) + " second";
  if (seconds > 1)  result += "s";
  result.trim();
  return result + " ago";
}

// Quick and dirty check for any unsafe chars in a string
// that may cause HTML shenanigans. e.g. An XSS.
bool hasUnsafeHTMLChars(String input) {
  static char unsafe[] = "';!-\"<>=&{}()";
  for (uint8_t i = 0; unsafe[i]; i++)
    if (input.indexOf(unsafe[i]) != -1) return true;
  return false;
}

// Root web page with example usage etc.
void handleRoot() {
  server.send(200, "text/html",
    "<html><head><title>IR MQTT server</title></head>"
    "<body>"
    "<center><h1>ESP8266 IR MQTT Server</h1></center>"
    "<br><hr>"
    "<h3>Information</h3>"
    "<p>IP address: " + WiFi.localIP().toString() + "<br>"
    "Booted: " + timeSince(1) + "<br>" +
    "Version: " _MY_VERSION_ "<br>"
    "Period Offset: " + String(offset) + "us<br>"
    "IR Lib Version: " _IRREMOTEESP8266_VERSION_ "<br>"
    "ESP8266 Core Version: " + ESP.getCoreVersion() + "<br>"
    "IR Send GPIO: " + String(IR_LED) + "<br>"
    "Total send requests: " + String(sendReqCounter) + "<br>"
    "Last message sent: " + String(lastSendSucceeded ? "Ok" : "FAILED") +
    " <i>(" + timeSince(lastSendTime) + ")</i><br>"
#ifdef IR_RX
    "IR Recv GPIO: " + String(IR_RX) + "<br>"
    "Total IR Received: " + String(irRecvCounter) + "<br>"
    "Last IR Received: " + lastIrReceived +
    " <i>(" + timeSince(lastIrReceivedTime) + ")</i><br>"
#endif  // IR_RX
    "</p>"
#ifdef MQTT_ENABLE
    "<h4>MQTT Information</h4>"
    "<p>Server: " MQTT_SERVER ":" + String(kMqttPort) + " <i>(" +
    (mqtt_client.connected() ? "Connected " + timeSince(lastDisconnectedTime)
                             : "Disconnected " + timeSince(lastConnectedTime)) +
    ")</i><br>"
    "Disconnections: " + String(mqttDisconnectCounter - 1) + "<br>"
    "Client id: " + mqtt_clientid + "<br>"
    "Command topic: " MQTTcommand "<br>"
    "Acknowledgements topic: " MQTTack "<br>"
#ifdef IR_RX
    "IR Received topic: " MQTTrecv "<br>"
#endif  // IR_RX
    "Last MQTT command seen: " +
    // lastMqttCmd is unescaped untrusted input.
    // Avoid any possible HTML/XSS when displaying it.
    (hasUnsafeHTMLChars(lastMqttCmd) ?
        "<i>Contains unsafe HTML characters</i>" : lastMqttCmd) +
    " <i>(" + timeSince(lastMqttCmdTime) + ")</i></p>"
#endif  // MQTT_ENABLE
    "<br><hr>"
    "<h3>Hardcoded examples</h3>"
    "<p><a href=\"ir?code=38000,1,69,341,171,21,64,21,64,21,21,21,21,21,21,21,"
        "21,21,21,21,64,21,64,21,21,21,64,21,21,21,21,21,21,21,64,21,21,21,64,"
        "21,21,21,21,21,21,21,64,21,21,21,21,21,21,21,21,21,64,21,64,21,64,21,"
        "21,21,64,21,64,21,64,21,1600,341,85,21,3647&type=31\">"
        "Sherwood Amp On (GlobalCache)</a></p>"
    "<p><a href=\"ir?code=38000,8840,4446,546,1664,546,1664,546,546,546,546,"
        "546,546,546,546,546,546,546,1664,546,1664,546,546,546,1664,546,546,"
        "546,546,546,546,546,1664,546,546,546,1664,546,546,546,1664,546,1664,"
        "546,1664,546,546,546,546,546,546,546,546,546,1664,546,546,546,546,546,"
        "546,546,1664,546,1664,546,1664,546,41600,8840,2210,546&type=30\">"
        "Sherwood Amp Off (Raw)</a></p>"
    "<p><a href=\"ir?code=0000,006E,0022,0002,0155,00AA,0015,0040,0015,0040"
        ",0015,0015,0015,0015,0015,0015,0015,0015,0015,0015,0015,0040,0015,0040"
        ",0015,0015,0015,0040,0015,0015,0015,0015,0015,0015,0015,0040,0015,0015"
        ",0015,0015,0015,0040,0015,0040,0015,0015,0015,0015,0015,0015,0015,0015"
        ",0015,0015,0015,0040,0015,0015,0015,0015,0015,0040,0015,0040,0015,0040"
        ",0015,0040,0015,0040,0015,0640,0155,0055,0015,0E40"
        "&type=25&repeats=1\">"
        "Sherwood Amp Input TAPE (Pronto)</a></p>"
    "<p><a href=\"ir?type=7&code=E0E09966\">TV on (Samsung)</a></p>"
    "<p><a href=\"ir?type=4&code=0xf50&bits=12\">Power Off (Sony 12bit)</a></p>"
    "<br><hr>"
    "<h3>Send a simple IR message</h3><p>"
    "<form method='POST' action='/ir' enctype='multipart/form-data'>"
      "Type: "
      "<select name='type'>"
        "<option value='9'>Aiwa RC T501</option>"
        "<option value='37'>Carrier AC</option>"
        "<option value='15'>Coolix</option>"
        "<option value='17'>Denon</option>"
        "<option value='13'>Dish</option>"
        "<option value='43'>GICable</option>"
        "<option value='6'>JVC</option>"
        "<option value='36'>Lasertag</option>"
        "<option value='10'>LG</option>"
        "<option value='51'>LG2</option>"
        "<option value='47'>Lutron</option>"
        "<option value='35'>MagiQuest</option>"
        "<option value='34'>Midea</option>"
        "<option value='12'>Mitsubishi</option>"
        "<option value='39'>Mitsubishi2</option>"
        "<option selected='selected' value='3'>NEC</option>"  // Default
        "<option value='29'>Nikai</option>"
        "<option value='5'>Panasonic</option>"
        "<option value='50'>Pioneer</option>"
        "<option value='1'>RC-5</option>"
        "<option value='23'>RC-5X</option>"
        "<option value='2'>RC-6</option>"
        "<option value='21'>RC-MM</option>"
        "<option value='7'>Samsung</option>"
        "<option value='11'>Sanyo</option>"
        "<option value='22'>Sanyo LC7461</option>"
        "<option value='14'>Sharp</option>"
        "<option value='19'>Sherwood</option>"
        "<option value='4'>Sony</option>"
        "<option value='8'>Whynter</option>"
      "</select>"
      " Code: 0x<input type='text' name='code' min='0' value='0' size='16'"
        " maxlength='16'>"
      " Bit size: "
      "<select name='bits'>"
        "<option selected='selected' value='0'>Default</option>"  // Default
        // Common bit length options for most protocols.
        "<option value='12'>12</option>"
        "<option value='13'>13</option>"
        "<option value='14'>14</option>"
        "<option value='15'>15</option>"
        "<option value='16'>16</option>"
        "<option value='20'>20</option>"
        "<option value='21'>21</option>"
        "<option value='24'>24</option>"
        "<option value='28'>28</option>"
        "<option value='32'>32</option>"
        "<option value='35'>35</option>"
        "<option value='36'>36</option>"
        "<option value='48'>48</option>"
        "<option value='56'>56</option>"
      "</select>"
      " Repeats: <input type='number' name='repeats' min='0' max='99' value='0'"
        "size='2' maxlength='2'>"
      " <input type='submit' value='Send IR'>"
    "</form>"
    "<br><hr>"
    "<h3>Send an IRremote Raw IR message</h3><p>"
    "<form method='POST' action='/ir' enctype='multipart/form-data'>"
      "<input type='hidden' name='type' value='30'>"
      "String: (freq,array data) <input type='text' name='code' size='132'"
      " value='38000,4420,4420,520,1638,520,1638,520,1638,520,520,520,520,520,"
          "520,520,520,520,520,520,1638,520,1638,520,1638,520,520,520,"
          "520,520,520,520,520,520,520,520,520,520,1638,520,520,520,520,520,"
          "520,520,520,520,520,520,520,520,1638,520,520,520,1638,520,1638,520,"
          "1638,520,1638,520,1638,520,1638,520'>"
      " <input type='submit' value='Send Raw'>"
    "</form>"
    "<br><hr>"
    "<h3>Send a <a href='https://irdb.globalcache.com/'>GlobalCache</a>"
        " IR message</h3><p>"
    "<form method='POST' action='/ir' enctype='multipart/form-data'>"
      "<input type='hidden' name='type' value='31'>"
      "String: 1:1,1,<input type='text' name='code' size='132'"
      " value='38000,1,1,170,170,20,63,20,63,20,63,20,20,20,20,20,20,20,20,20,"
          "20,20,63,20,63,20,63,20,20,20,20,20,20,20,20,20,20,20,20,20,63,20,"
          "20,20,20,20,20,20,20,20,20,20,20,20,63,20,20,20,63,20,63,20,63,20,"
          "63,20,63,20,63,20,1798'>"
      " <input type='submit' value='Send GlobalCache'>"
    "</form>"
    "<br><hr>"
    "<h3>Send a <a href='http://www.remotecentral.com/cgi-bin/files/rcfiles.cgi"
      "?area=pronto&db=discrete'>Pronto code</a> IR message</h3><p>"
    "<form method='POST' action='/ir' enctype='multipart/form-data'>"
      "<input type='hidden' name='type' value='25'>"
      "String (comma separated): <input type='text' name='code' size='132'"
      " value='0000,0067,0000,0015,0060,0018,0018,0018,0030,0018,0030,0018,"
          "0030,0018,0018,0018,0030,0018,0018,0018,0018,0018,0030,0018,0018,"
          "0018,0030,0018,0030,0018,0030,0018,0018,0018,0018,0018,0030,0018,"
          "0018,0018,0018,0018,0030,0018,0018,03f6'>"
      " Repeats: <input type='number' name='repeats' min='0' max='99' value='0'"
          "size='2' maxlength='2'>"
      " <input type='submit' value='Send Pronto'>"
    "</form>"
    "<br><hr>"
    "<h3>Send an Air Conditioner IR message</h3><p>"
    "<form method='POST' action='/ir' enctype='multipart/form-data'>"
      "Type: "
      "<select name='type'>"
        "<option value='27'>Argo</option>"
        "<option value='16'>Daikin</option>"
        "<option value='48'>Electra</option>"
        "<option value='33'>Fujitsu</option>"
        "<option value='24'>Gree</option>"
        "<option value='38'>Haier (9 bytes)</option>"
        "<option value='44'>Haier (14 bytes/YR-W02)</option>"
        "<option value='40'>Hitachi (28 bytes)</option>"
        "<option value='41'>Hitachi1 (13 bytes)</option>"
        "<option value='42'>Hitachi2 (53 bytes)</option>"
        "<option selected='selected' value='18'>Kelvinator</option>"  // Default
        "<option value='20'>Mitsubishi</option>"
        "<option value='52'>MWM</option>"
        "<option value='46'>Samsung</option>"
        "<option value='32'>Toshiba</option>"
        "<option value='28'>Trotec</option>"
        "<option value='45'>Whirlpool</option>"
      "</select>"
      " State code: 0x"
      "<input type='text' name='code' size='" + String(kStateSizeMax * 2) +
          "' maxlength='" + String(kStateSizeMax * 2) + "'"
          " value='190B8050000000E0190B8070000010F0'>"
      " <input type='submit' value='Send A/C State'>"
    "</form>"
    "<br><hr>"
    "<h3>Update IR Server firmware</h3><p>"
    "<b><mark>Warning:</mark></b><br> "
    "<i>Updating your firmware may screw up your access to the device. "
    "If you are going to use this, know what you are doing first "
    "(and you probably do).</i><br>"
    "<form method='POST' action='/update' enctype='multipart/form-data'>"
      "Firmware to upload: <input type='file' name='update'>"
      "<input type='submit' value='Update'>"
    "</form>"
    "</body></html>");
}

// Reset web page
void handleReset() {
  server.send(200, "text/html",
    "<html><head><title>Reset Config</title></head>"
    "<body>"
    "<h1>Resetting the WiFiManager config back to defaults.</h1>"
    "<p>Device restarting. Try connecting in a few seconds.</p>"
    "</body></html>");
    // Do the reset.
  wifiManager.resetSettings();
  delay(10);
  ESP.restart();
  delay(1000);
}

// Parse an Air Conditioner A/C Hex String/code and send it.
// Args:
//   irType: Nr. of the protocol we need to send.
//   str: A hexadecimal string containing the state to be sent.
// Returns:
//   bool: Successfully sent or not.
bool parseStringAndSendAirCon(const uint16_t irType, const String str) {
  uint8_t strOffset = 0;
  uint8_t state[kStateSizeMax] = {0};  // All array elements are set to 0.
  uint16_t stateSize = 0;

  if (str.startsWith("0x") || str.startsWith("0X"))
    strOffset = 2;
  // Calculate how many hexadecimal characters there are.
  uint16_t inputLength = str.length() - strOffset;
  if (inputLength == 0) {
    debug("Zero length AirCon code encountered. Ignored.");
    return false;  // No input. Abort.
  }

  switch (irType) {  // Get the correct state size for the protocol.
    case KELVINATOR:
      stateSize = kKelvinatorStateLength;
      break;
    case TOSHIBA_AC:
      stateSize = kToshibaACStateLength;
      break;
    case DAIKIN:
      stateSize = kDaikinStateLength;
      break;
    case ELECTRA_AC:
      stateSize = kElectraAcStateLength;
      break;
    case MITSUBISHI_AC:
      stateSize = kMitsubishiACStateLength;
      break;
    case PANASONIC_AC:
      stateSize = kPanasonicAcStateLength;
      break;
    case TROTEC:
      stateSize = kTrotecStateLength;
      break;
    case ARGO:
      stateSize = kArgoStateLength;
      break;
    case GREE:
      stateSize = kGreeStateLength;
      break;
    case FUJITSU_AC:
      // Fujitsu has four distinct & different size states, so make a best guess
      // which one we are being presented with based on the number of
      // hexadecimal digits provided. i.e. Zero-pad if you need to to get
      // the correct length/byte size.
      stateSize = inputLength / 2;  // Every two hex chars is a byte.
      // Use at least the minimum size.
      stateSize = std::max(stateSize,
                           (uint16_t) (kFujitsuAcStateLengthShort - 1));
      // If we think it isn't a "short" message.
      if (stateSize > kFujitsuAcStateLengthShort)
        // Then it has to be at least the smaller version of the "normal" size.
        stateSize = std::max(stateSize, (uint16_t) (kFujitsuAcStateLength - 1));
      // Lastly, it should never exceed the maximum "normal" size.
      stateSize = std::min(stateSize, kFujitsuAcStateLength);
      break;
    case HAIER_AC:
      stateSize = kHaierACStateLength;
      break;
    case HAIER_AC_YRW02:
      stateSize = kHaierACYRW02StateLength;
      break;
    case HITACHI_AC:
      stateSize = kHitachiAcStateLength;
      break;
    case HITACHI_AC1:
      stateSize = kHitachiAc1StateLength;
      break;
    case HITACHI_AC2:
      stateSize = kHitachiAc2StateLength;
      break;
    case WHIRLPOOL_AC:
      stateSize = kWhirlpoolAcStateLength;
      break;
    case SAMSUNG_AC:
      // Samsung has two distinct & different size states, so make a best guess
      // which one we are being presented with based on the number of
      // hexadecimal digits provided. i.e. Zero-pad if you need to to get
      // the correct length/byte size.
      stateSize = inputLength / 2;  // Every two hex chars is a byte.
      // Use at least the minimum size.
      stateSize = std::max(stateSize, (uint16_t) (kSamsungAcStateLength));
      // If we think it isn't a "normal" message.
      if (stateSize > kSamsungAcStateLength)
        // Then it probably the extended size.
        stateSize = std::max(stateSize,
                             (uint16_t) (kSamsungAcExtendedStateLength));
      // Lastly, it should never exceed the maximum "extended" size.
      stateSize = std::min(stateSize, kSamsungAcExtendedStateLength);
      break;
    case MWM:
      // MWM has variable size states, so make a best guess
      // which one we are being presented with based on the number of
      // hexadecimal digits provided. i.e. Zero-pad if you need to to get
      // the correct length/byte size.
      stateSize = inputLength / 2;  // Every two hex chars is a byte.
      // Use at least the minimum size.
      stateSize = std::max(stateSize, (uint16_t) 3);
      // Cap the maximum size.
      stateSize = std::min(stateSize, kStateSizeMax);
      break;
    default:  // Not a protocol we expected. Abort.
      debug("Unexpected AirCon protocol detected. Ignoring.");
      return false;
  }
  if (inputLength > stateSize * 2) {
    debug("AirCon code to large for the given protocol.");
    return false;
  }

  // Ptr to the least significant byte of the resulting state for this protocol.
  uint8_t *statePtr = &state[stateSize - 1];

  // Convert the string into a state array of the correct length.
  for (uint16_t i = 0; i < inputLength; i++) {
    // Grab the next least sigificant hexadecimal digit from the string.
    uint8_t c = tolower(str[inputLength + strOffset - i - 1]);
    if (isxdigit(c)) {
      if (isdigit(c))
        c -= '0';
      else
        c = c - 'a' + 10;
    } else {
      debug("Aborting! Non-hexadecimal char found in AirCon state: " + str);
      return false;
    }
    if (i % 2 == 1) {  // Odd: Upper half of the byte.
      *statePtr += (c << 4);
      statePtr--;  // Advance up to the next least significant byte of state.
    } else {  // Even: Lower half of the byte.
      *statePtr = c;
    }
  }

  // Make the appropriate call for the protocol type.
  switch (irType) {
#if SEND_KELVINATOR
    case KELVINATOR:
      irsend.sendKelvinator(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_TOSHIBA_AC
    case TOSHIBA_AC:
      irsend.sendToshibaAC(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_DAIKIN
    case DAIKIN:
      irsend.sendDaikin(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if MITSUBISHI_AC
    case MITSUBISHI_AC:
      irsend.sendMitsubishiAC(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_TROTEC
    case TROTEC:
      irsend.sendTrotec(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_ARGO
    case ARGO:
      irsend.sendArgo(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_GREE
    case GREE:
      irsend.sendGree(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_FUJITSU_AC
    case FUJITSU_AC:
      irsend.sendFujitsuAC(reinterpret_cast<uint8_t *>(state), stateSize);
      break;
#endif
#if SEND_HAIER_AC
    case HAIER_AC:
      irsend.sendHaierAC(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_HAIER_AC_YRW02
    case HAIER_AC_YRW02:
      irsend.sendHaierACYRW02(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_HITACHI_AC
    case HITACHI_AC:
      irsend.sendHitachiAC(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_HITACHI_AC1
    case HITACHI_AC1:
      irsend.sendHitachiAC1(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_HITACHI_AC2
    case HITACHI_AC2:
      irsend.sendHitachiAC2(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_WHIRLPOOL_AC
    case WHIRLPOOL_AC:
      irsend.sendWhirlpoolAC(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_SAMSUNG_AC
    case SAMSUNG_AC:
      irsend.sendSamsungAC(reinterpret_cast<uint8_t *>(state), stateSize);
      break;
#endif
#if SEND_ELECTRA_AC
    case ELECTRA_AC:
      irsend.sendElectraAC(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_PANASONIC_AC
    case PANASONIC_AC:
      irsend.sendPanasonicAC(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_MWM_
    case MWM:
      irsend.sendMWM(reinterpret_cast<uint8_t *>(state), stateSize);
      break;
#endif
    default:
      debug("Unexpected AirCon type in send request. Not sent.");
      return false;
  }
  return true;  // We were successful as far as we can tell.
}

// Count how many values are in the String.
// Args:
//   str:  String containing the values.
//   sep:  Character that separates the values.
// Returns:
//   The number of values found in the String.
uint16_t countValuesInStr(const String str, char sep) {
  int16_t index = -1;
  uint16_t count = 1;
  do {
    index = str.indexOf(sep, index + 1);
    count++;
  } while (index != -1);
  return count;
}

// Dynamically allocate an array of uint16_t's.
// Args:
//   size:  Nr. of uint16_t's need to be in the new array.
// Returns:
//   A Ptr to the new array. Restarts the ESP8266 if it fails.
uint16_t * newCodeArray(const uint16_t size) {
  uint16_t *result;

  result = reinterpret_cast<uint16_t*>(malloc(size * sizeof(uint16_t)));
  // Check we malloc'ed successfully.
  if (result == NULL) {  // malloc failed, so give up.
    Serial.printf("\nCan't allocate %d bytes. (%d bytes free)\n",
                  size * sizeof(uint16_t), ESP.getFreeHeap());
    Serial.println("Giving up & forcing a reboot.");
    ESP.restart();  // Reboot.
    delay(500);  // Wait for the restart to happen.
    return result;  // Should never get here, but just in case.
  }
  return result;
}

#if SEND_GLOBALCACHE
// Parse a GlobalCache String/code and send it.
// Args:
//   str: A GlobalCache formatted String of comma separated numbers.
//        e.g. "38000,1,1,170,170,20,63,20,63,20,63,20,20,20,20,20,20,20,20,20,
//              20,20,63,20,63,20,63,20,20,20,20,20,20,20,20,20,20,20,20,20,63,
//              20,20,20,20,20,20,20,20,20,20,20,20,20,63,20,20,20,63,20,63,20,
//              63,20,63,20,63,20,63,20,1798"
//        Note: The leading "1:1,1," of normal GC codes should be removed.
// Returns:
//   bool: Successfully sent or not.
bool parseStringAndSendGC(const String str) {
  uint16_t count;
  uint16_t *code_array;
  String tmp_str;

  // Remove the leading "1:1,1," if present.
  if (str.startsWith("1:1,1,"))
    tmp_str = str.substring(6);
  else
    tmp_str = str;

  // Find out how many items there are in the string.
  count = countValuesInStr(tmp_str, ',');

  // Now we know how many there are, allocate the memory to store them all.
  code_array = newCodeArray(count);

  // Now convert the strings to integers and place them in code_array.
  count = 0;
  uint16_t start_from = 0;
  int16_t index = -1;
  do {
    index = tmp_str.indexOf(',', start_from);
    code_array[count] = tmp_str.substring(start_from, index).toInt();
    start_from = index + 1;
    count++;
  } while (index != -1);

  irsend.sendGC(code_array, count);  // All done. Send it.
  free(code_array);  // Free up the memory allocated.
  if (count > 0)
    return true;  // We sent something.
  return false;  // We probably didn't.
}
#endif  // SEND_GLOBALCACHE

#if SEND_PRONTO
// Parse a Pronto Hex String/code and send it.
// Args:
//   str: A comma-separated String of nr. of repeats, then hexadecimal numbers.
//        e.g. "R1,0000,0067,0000,0015,0060,0018,0018,0018,0030,0018,0030,0018,
//              0030,0018,0018,0018,0030,0018,0018,0018,0018,0018,0030,0018,
//              0018,0018,0030,0018,0030,0018,0030,0018,0018,0018,0018,0018,
//              0030,0018,0018,0018,0018,0018,0030,0018,0018,03f6"
//              or
//              "0000,0067,0000,0015,0060,0018". i.e. without the Repeat value
//        Requires at least kProntoMinLength comma-separated values.
//        sendPronto() only supports raw pronto code types, thus so does this.
//   repeats:  Nr. of times the message is to be repeated.
//             This value is ignored if an embeddd repeat is found in str.
// Returns:
//   bool: Successfully sent or not.
bool parseStringAndSendPronto(const String str, uint16_t repeats) {
  uint16_t count;
  uint16_t *code_array;
  int16_t index = -1;
  uint16_t start_from = 0;

  // Find out how many items there are in the string.
  count = countValuesInStr(str, ',');

  // Check if we have the optional embedded repeats value in the code string.
  if (str.startsWith("R") || str.startsWith("r")) {
    // Grab the first value from the string, as it is the nr. of repeats.
    index = str.indexOf(',', start_from);
    repeats = str.substring(start_from + 1, index).toInt();  // Skip the 'R'.
    start_from = index + 1;
    count--;  // We don't count the repeats value as part of the code array.
  }

  // We need at least kProntoMinLength values for the code part.
  if (count < kProntoMinLength) return false;

  // Now we know how many there are, allocate the memory to store them all.
  code_array = newCodeArray(count);

  // Rest of the string are values for the code array.
  // Now convert the hex strings to integers and place them in code_array.
  count = 0;
  do {
    index = str.indexOf(',', start_from);
    // Convert the hexadecimal value string to an unsigned integer.
    code_array[count] = strtoul(str.substring(start_from, index).c_str(),
                                NULL, 16);
    start_from = index + 1;
    count++;
  } while (index != -1);

  irsend.sendPronto(code_array, count, repeats);  // All done. Send it.
  free(code_array);  // Free up the memory allocated.
  if (count > 0)
    return true;  // We sent something.
  return false;  // We probably didn't.
}
#endif  // SEND_PRONTO

#if SEND_RAW
// Parse an IRremote Raw Hex String/code and send it.
// Args:
//   str: A comma-separated String containing the freq and raw IR data.
//        e.g. "38000,9000,4500,600,1450,600,900,650,1500,..."
//        Requires at least two comma-separated values.
//        First value is the transmission frequency in Hz or kHz.
// Returns:
//   bool: Successfully sent or not.
bool parseStringAndSendRaw(const String str) {
  uint16_t count;
  uint16_t freq = 38000;  // Default to 38kHz.
  uint16_t *raw_array;

  // Find out how many items there are in the string.
  count = countValuesInStr(str, ',');

  // We expect the frequency as the first comma separated value, so we need at
  // least two values. If not, bail out.
  if (count < 2)  return false;
  count--;  // We don't count the frequency value as part of the raw array.

  // Now we know how many there are, allocate the memory to store them all.
  raw_array = newCodeArray(count);

  // Grab the first value from the string, as it is the frequency.
  int16_t index = str.indexOf(',', 0);
  freq = str.substring(0, index).toInt();
  uint16_t start_from = index + 1;
  // Rest of the string are values for the raw array.
  // Now convert the strings to integers and place them in raw_array.
  count = 0;
  do {
    index = str.indexOf(',', start_from);
    raw_array[count] = str.substring(start_from, index).toInt();
    start_from = index + 1;
    count++;
  } while (index != -1);

  irsend.sendRaw(raw_array, count, freq);  // All done. Send it.
  free(raw_array);  // Free up the memory allocated.
  if (count > 0)
    return true;  // We sent something.
  return false;  // We probably didn't.
}
#endif  // SEND_RAW

// Parse the URL args to find the IR code.
void handleIr() {
  uint64_t data = 0;
  String data_str = "";
  int ir_type = 3;  // Default to NEC codes.
  uint16_t nbits = 0;
  uint16_t repeat = 0;

  for (uint16_t i = 0; i < server.args(); i++) {
    if (server.argName(i) == argType)
      ir_type = atoi(server.arg(i).c_str());
    if (server.argName(i) == argData) {
      data = getUInt64fromHex(server.arg(i).c_str());
      data_str = server.arg(i);
    }
    if (server.argName(i) == argBits)
      nbits = atoi(server.arg(i).c_str());
    if (server.argName(i) == argRepeat)
      repeat = atoi(server.arg(i).c_str());
  }
  debug("New code received via HTTP");
  lastSendSucceeded = sendIRCode(ir_type, data, data_str.c_str(), nbits,
                                 repeat);
  handleRoot();
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i < server.args(); i++)
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  server.send(404, "text/plain", message);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network

  wifiManager.setTimeout(300);  // Time out after 5 mins.
#if USE_STATIC_IP
  // Use a static IP config rather than the one supplied via DHCP.
  wifiManager.setSTAStaticIPConfig(kIPAddress, kGateway, kSubnetMask);
#endif  // USE_STATIC_IP
  if (!wifiManager.autoConnect()) {
    debug("Wifi failed to connect and hit timeout.");
    delay(3000);
    // Reboot. A.k.a. "Have you tried turning it Off and On again?"
    ESP.reset();
    delay(5000);
  }

  debug("WiFi connected. IP address: " + WiFi.localIP().toString());
}

void setup(void) {
  irsend.begin();
  offset = irsend.calibrate();
#if IR_RX
#if DECODE_HASH
  // Ignore messages with less than minimum on or off pulses.
  irrecv.setUnknownThreshold(kMinUnknownSize);
#endif  // DECODE_HASH
  irrecv.enableIRIn();  // Start the receiver
#endif  // IR_RX

  #ifdef DEBUG
  // Use SERIAL_TX_ONLY so that the RX pin can be freed up for GPIO/IR use.
  Serial.begin(BAUD_RATE, SERIAL_8N1, SERIAL_TX_ONLY);
  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);
  Serial.println();
  debug("IRMQTTServer " _MY_VERSION_" has booted.");
  #endif  // DEBUG

  setup_wifi();

  // Wait a bit for things to settle.
  delay(1500);

  lastReconnectAttempt = 0;

  if (mdns.begin(HOSTNAME, WiFi.localIP())) {
    debug("MDNS responder started");
  }

  // Setup the root web page.
  server.on("/", handleRoot);
  // Setup the page to handle web-based IR codes.
  server.on("/ir", handleIr);
  // Setup a reset page to cause WiFiManager information to be reset.
  server.on("/reset", handleReset);

  // Setup the URL to allow Over-The-Air (OTA) firmware updates.
  server.on("/update", HTTP_POST, [](){
      server.sendHeader("Connection", "close");
      server.send(200, "text/plain", (Update.hasError())?"FAIL":"OK");
      ESP.restart();
    }, [](){
      HTTPUpload& upload = server.upload();
      if (upload.status == UPLOAD_FILE_START) {
        WiFiUDP::stopAll();
        debug("Update: " + upload.filename);
        uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) &
            0xFFFFF000;
        if (!Update.begin(maxSketchSpace)) {  // start with max available size
#ifdef DEBUG
          Update.printError(Serial);
#endif  // DEBUG
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) !=
            upload.currentSize) {
#ifdef DEBUG
          Update.printError(Serial);
#endif  // DEBUG
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) {  // true to set the size to the current progress
          debug("Update Success: " + (String) upload.totalSize +
                "\nRebooting...");
        }
      }
      yield();
    });

  // Set up an error page.
  server.onNotFound(handleNotFound);

  server.begin();
  debug("HTTP server started");
}

#ifdef MQTT_ENABLE
// MQTT subscribing to topic
void subscribing(const String topic_name) {
  // subscription to topic for receiving data
  if (mqtt_client.subscribe(topic_name.c_str())) {
    debug("Subscription OK to " + topic_name);
  }
}

bool reconnect() {
  // Loop a few times or until we're reconnected
  uint16_t tries = 1;
  while (!mqtt_client.connected() && tries <= 3) {
    int connected = false;
    // Attempt to connect
    debug("Attempting MQTT connection to " MQTT_SERVER ":" + String(kMqttPort) +
          "... ");
    if (mqtt_user && mqtt_password)
      connected = mqtt_client.connect(mqtt_clientid.c_str(), mqtt_user,
                                      mqtt_password);
    else
      connected = mqtt_client.connect(mqtt_clientid.c_str());
    if (connected) {
    // Once connected, publish an announcement...
      mqtt_client.publish(MQTTack, "Connected");
      debug("connected.");
      // Subscribing to topic(s)
      subscribing(MQTTcommand);
    } else {
      debug("failed, rc=" + String(mqtt_client.state()) +
            " Try again in a bit.");
      // Wait for a bit before retrying
      delay(tries << 7);  // Linear increasing back-off (x128)
    }
    tries++;
  }
  return mqtt_client.connected();
}
#endif  // MQTT_ENABLE

void loop(void) {
  server.handleClient();  // Handle any web activity

#ifdef MQTT_ENABLE
  uint32_t now = millis();
  // MQTT client connection management
  if (!mqtt_client.connected()) {
    if (wasConnected) {
      lastDisconnectedTime = now;
      wasConnected = false;
      mqttDisconnectCounter++;
    }
    // Reconnect if it's longer than kMqttReconnectTime since we last tried.
    if (now - lastReconnectAttempt > kMqttReconnectTime) {
      lastReconnectAttempt = now;
      debug("client mqtt not connected, trying to connect");
      // Attempt to reconnect
      if (reconnect()) {
        lastReconnectAttempt = 0;
        wasConnected = true;
        if (boot) {
          mqtt_client.publish(MQTTack, "IR Server just booted");
          boot = false;
        } else {
          String text = "IR Server just (re)connected to MQTT. "
              "Lost connection about " + timeSince(lastConnectedTime);
          mqtt_client.publish(MQTTack, text.c_str());
        }
        lastConnectedTime = now;
        debug("successful client mqtt connection");
      }
    }
  } else {
    lastConnectedTime = now;
    // MQTT loop
    mqtt_client.loop();
  }
#endif  // MQTT_ENABLE
#ifdef IR_RX
  // Check if an IR code has been received via the IR RX module.
  if (irrecv.decode(&capture)) {
    lastIrReceivedTime = millis();
    lastIrReceived = String(capture.decode_type) + "," +
        resultToHexidecimal(&capture);
    // If it isn't an AC code, add the bits.
    if (!hasACState(capture.decode_type))
      lastIrReceived += "," + String(capture.bits);
    mqtt_client.publish(MQTTrecv, lastIrReceived.c_str());
    irRecvCounter++;
    debug("Incoming IR message sent to MQTT: " + lastIrReceived);
  }
#endif  // IR_RX
  delay(100);
}

// Arduino framework doesn't support strtoull(), so make our own one.
uint64_t getUInt64fromHex(char const *str) {
  uint64_t result = 0;
  uint16_t offset = 0;
  // Skip any leading '0x' or '0X' prefix.
  if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
    offset = 2;
  for (; isxdigit((unsigned char)str[offset]); offset++) {
    char c = str[offset];
    result *= 16;
    if (isdigit(c)) /* '0' .. '9' */
      result += c - '0';
    else if (isupper(c)) /* 'A' .. 'F' */
      result += c - 'A' + 10;
    else /* 'a' .. 'f'*/
      result += c - 'a' + 10;
  }
  return result;
}

// Transmit the given IR message.
//
// Args:
//   ir_type:  enum of the protocol to be sent.
//   code:     Numeric payload of the IR message. Most protocols use this.
//   code_str: The unparsed code to be sent. Used by complex protocol encodings.
//   bits:     Nr. of bits in the protocol. 0 means use the protocol's default.
//   repeat:   Nr. of times the message is to be repeated. (Not all protcols.)
// Returns:
//   bool: Successfully sent or not.
bool sendIRCode(int const ir_type, uint64_t const code, char const * code_str,
                uint16_t bits, uint16_t repeat) {
  // Create a pseudo-lock so we don't try to send two codes at the same time.
  while (ir_lock)
    delay(20);
  ir_lock = true;

  bool success = true;  // Assume success.

  // send the IR message.
  switch (ir_type) {
#if SEND_RC5
    case RC5:  // 1
      if (bits == 0)
        bits = kRC5Bits;
      irsend.sendRC5(code, bits, repeat);
      break;
#endif
#if SEND_RC6
    case RC6:  // 2
      if (bits == 0)
        bits = kRC6Mode0Bits;
      irsend.sendRC6(code, bits, repeat);
      break;
#endif
#if SEND_NEC
    case NEC:  // 3
      if (bits == 0)
        bits = kNECBits;
      irsend.sendNEC(code, bits, repeat);
      break;
#endif
#if SEND_SONY
    case SONY:  // 4
      if (bits == 0)
        bits = kSony12Bits;
      repeat = std::max(repeat, kSonyMinRepeat);
      irsend.sendSony(code, bits, repeat);
      break;
#endif
#if SEND_PANASONIC
    case PANASONIC:  // 5
      if (bits == 0)
        bits = kPanasonicBits;
      irsend.sendPanasonic64(code, bits, repeat);
      break;
#endif
#if SEND_JVC
    case JVC:  // 6
      if (bits == 0)
        bits = kJvcBits;
      irsend.sendJVC(code, bits, repeat);
      break;
#endif
#if SEND_SAMSUNG
    case SAMSUNG:  // 7
      if (bits == 0)
        bits = kSamsungBits;
      irsend.sendSAMSUNG(code, bits, repeat);
      break;
#endif
#if SEND_WHYNTER
    case WHYNTER:  // 8
      if (bits == 0)
        bits = kWhynterBits;
      irsend.sendWhynter(code, bits, repeat);
      break;
#endif
#if SEND_AIWA_RC_T501
    case AIWA_RC_T501:  // 9
      if (bits == 0)
        bits = kAiwaRcT501Bits;
      repeat = std::max(repeat, kAiwaRcT501MinRepeats);
      irsend.sendAiwaRCT501(code, bits, repeat);
      break;
#endif
#if SEND_LG
    case LG:  // 10
      if (bits == 0)
        bits = kLgBits;
      irsend.sendLG(code, bits, repeat);
      break;
#endif
#if SEND_MITSUBISHI
    case MITSUBISHI:  // 12
      if (bits == 0)
        bits = kMitsubishiBits;
      repeat = std::max(repeat, kMitsubishiMinRepeat);
      irsend.sendMitsubishi(code, bits, repeat);
      break;
#endif
#if SEND_DISH
    case DISH:  // 13
      if (bits == 0)
        bits = kDishBits;
      repeat = std::max(repeat, kDishMinRepeat);
      irsend.sendDISH(code, bits, repeat);
      break;
#endif
#if SEND_SHARP
    case SHARP:  // 14
      if (bits == 0)
        bits = kSharpBits;
      irsend.sendSharpRaw(code, bits, repeat);
      break;
#endif
#if SEND_COOLIX
    case COOLIX:  // 15
      if (bits == 0)
        bits = kCoolixBits;
      irsend.sendCOOLIX(code, bits, repeat);
      break;
#endif
    case DAIKIN:  // 16
    case KELVINATOR:  // 18
    case MITSUBISHI_AC:  // 20
    case GREE:  // 24
    case ARGO:  // 27
    case TROTEC:  // 28
    case TOSHIBA_AC:  // 32
    case FUJITSU_AC:  // 33
    case HAIER_AC:  // 38
    case HAIER_AC_YRW02:  // 44
    case HITACHI_AC:  // 40
    case HITACHI_AC1:  // 41
    case HITACHI_AC2:  // 42
    case WHIRLPOOL_AC:  // 45
    case SAMSUNG_AC:  // 46
    case ELECTRA_AC:  // 48
    case PANASONIC_AC:  // 49
    case MWM:  // 52
      success = parseStringAndSendAirCon(ir_type, code_str);
      break;
#if SEND_DENON
    case DENON:  // 17
      if (bits == 0)
        bits = DENON_BITS;
      irsend.sendDenon(code, bits, repeat);
      break;
#endif
#if SEND_SHERWOOD
    case SHERWOOD:  // 19
      if (bits == 0)
        bits = kSherwoodBits;
      repeat = std::max(repeat, kSherwoodMinRepeat);
      irsend.sendSherwood(code, bits, repeat);
      break;
#endif
#if SEND_RCMM
    case RCMM:  // 21
      if (bits == 0)
        bits = kRCMMBits;
      irsend.sendRCMM(code, bits, repeat);
      break;
#endif
#if SEND_SANYO
    case SANYO_LC7461:  // 22
      if (bits == 0)
        bits = kSanyoLC7461Bits;
      irsend.sendSanyoLC7461(code, bits, repeat);
      break;
#endif
#if SEND_RC5
    case RC5X:  // 23
      if (bits == 0)
        bits = kRC5XBits;
      irsend.sendRC5(code, bits, repeat);
      break;
#endif
#if SEND_PRONTO
    case PRONTO:  // 25
      success = parseStringAndSendPronto(code_str, repeat);
      break;
#endif
#if SEND_NIKAI
    case NIKAI:  // 29
      if (bits == 0)
        bits = kNikaiBits;
      irsend.sendNikai(code, bits, repeat);
      break;
#endif
#if SEND_RAW
    case RAW:  // 30
      success = parseStringAndSendRaw(code_str);
      break;
#endif
#if SEND_GLOBALCACHE
    case GLOBALCACHE:  // 31
      success = parseStringAndSendGC(code_str);
      break;
#endif
#if SEND_MIDEA
    case MIDEA:  // 34
      if (bits == 0)
        bits = kMideaBits;
      irsend.sendMidea(code, bits, repeat);
      break;
#endif
#if SEND_MAGIQUEST
    case MAGIQUEST:  // 35
      if (bits == 0)
        bits = kMagiquestBits;
      irsend.sendMagiQuest(code, bits, repeat);
      break;
#endif
#if SEND_LASERTAG
    case LASERTAG:  // 36
      if (bits == 0)
        bits = kLasertagBits;
      irsend.sendLasertag(code, bits, repeat);
      break;
#endif
#if SEND_CARRIER_AC
    case CARRIER_AC:  // 37
      if (bits == 0)
        bits = kCarrierAcBits;
      irsend.sendCarrierAC(code, bits, repeat);
      break;
#endif
#if SEND_MITSUBISHI2
    case MITSUBISHI2:  // 39
      if (bits == 0)
        bits = kMitsubishiBits;
      repeat = std::max(repeat, kMitsubishiMinRepeat);
      irsend.sendMitsubishi2(code, bits, repeat);
      break;
#endif
#if SEND_GICABLE
    case GICABLE:  // 43
      if (bits == 0)
        bits = kGicableBits;
      repeat = std::max(repeat, kGicableMinRepeat);
      irsend.sendGICable(code, bits, repeat);
      break;
#endif
#if SEND_LUTRON
    case LUTRON:  // 47
      if (bits == 0)
        bits = kLutronBits;
      irsend.sendLutron(code, bits, repeat);
      break;
#endif
#if SEND_PIONEER
    case PIONEER:  // 50
      if (bits == 0)
        bits = kPioneerBits;
      irsend.sendPioneer(code, bits, repeat);
      break;
#endif

#if SEND_LG
    case LG2:  // 51
      if (bits == 0)
        bits = kLgBits;
      irsend.sendLG2(code, bits, repeat);
      break;
#endif
    default:
      // If we got here, we didn't know how to send it.
      success = false;
  }
  lastSendTime = millis();
  // Release the lock.
  ir_lock = false;

  // Indicate that we sent the message or not.
  if (success) {
    sendReqCounter++;
    debug("Sent the IR message:");
  } else {
    debug("Failed to send IR Message:");
  }
  debug("Type: " + String(ir_type));
  // For "long" codes we basically repeat what we got.
  if (hasACState((decode_type_t) ir_type) ||
      ir_type == PRONTO ||
      ir_type == RAW ||
      ir_type == GLOBALCACHE) {
    debug("Code: ");
    debug(code_str);
    // Confirm what we were asked to send was sent.
#ifdef MQTT_ENABLE
    if (success) {
      if (ir_type == PRONTO && repeat > 0)
        mqtt_client.publish(MQTTack, (String(ir_type) + ",R" +
                                      String(repeat) + "," +
                                      String(code_str)).c_str());
      else
        mqtt_client.publish(MQTTack, (String(ir_type) + "," +
                                      String(code_str)).c_str());
    }
#endif  // MQTT_ENABLE
  } else {  // For "short" codes, we break it down a bit more before we report.
    debug("Code: 0x" + uint64ToString(code, 16));
    debug("Bits: " + String(bits));
    debug("Repeats: " + String(repeat));
#ifdef MQTT_ENABLE
    if (success)
      mqtt_client.publish(MQTTack, (String(ir_type) + "," +
                                    uint64ToString(code, 16)
                                    + "," + String(bits) + "," +
                                    String(repeat)).c_str());
#endif  // MQTT_ENABLE
  }
  return success;
}

#ifdef MQTT_ENABLE
void receivingMQTT(String const topic_name, String const callback_str) {
  char* tok_ptr;
  uint64_t code = 0;
  uint16_t nbits = 0;
  uint16_t repeat = 0;

  debug("Receiving data by MQTT topic " + topic_name);

  // Make a copy of the callback string as strtok destroys it.
  char* callback_c_str = strdup(callback_str.c_str());
  debug("MQTT Payload (raw): " + callback_str);
  // Save the message as the last command seen (global).
  lastMqttCmd = callback_str;
  lastMqttCmdTime = millis();

  // Get the numeric protocol type.
  int ir_type = strtoul(strtok_r(callback_c_str, ",", &tok_ptr), NULL, 10);
  char* next = strtok_r(NULL, ",", &tok_ptr);
  // If there is unparsed string left, try to convert it assuming it's hex.
  if (next != NULL) {
    code = getUInt64fromHex(next);
    next = strtok_r(NULL, ",", &tok_ptr);
  } else {
    // We require at least two value in the string. Give up.
    return;
  }
  // If there is still string left, assume it is the bit size.
  if (next != NULL) {
    nbits = atoi(next);
    next = strtok_r(NULL, ",", &tok_ptr);
  }
  // If there is still string left, assume it is the repeat count.
  if (next != NULL)
    repeat = atoi(next);

  free(callback_c_str);


  // send received MQTT value by IR signal
  lastSendSucceeded = sendIRCode(
      ir_type, code,
      callback_str.substring(callback_str.indexOf(",") + 1).c_str(),
      nbits, repeat);
}

// Callback function, when the gateway receive an MQTT value on the topics
// subscribed this function is called
void callback(char* topic, byte* payload, unsigned int length) {
  // In order to republish this payload, a copy must be made
  // as the orignal payload buffer will be overwritten whilst
  // constructing the PUBLISH packet.
  // Allocate the correct amount of memory for the payload copy
  byte* payload_copy = reinterpret_cast<byte*>(malloc(length + 1));
  // Copy the payload to the new buffer
  memcpy(payload_copy, payload, length);

  // Conversion to a printable string
  payload_copy[length] = '\0';
  String callback_string = String(reinterpret_cast<char*>(payload_copy));
  String topic_name = String(reinterpret_cast<char*>(topic));

  // launch the function to treat received data
  receivingMQTT(topic_name, callback_string);

  // Free the memory
  free(payload_copy);
}
#endif  // MQTT_ENABLE
