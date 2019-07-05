/*
 * Send & receive arbitrary IR codes via a web server or MQTT.
 * Copyright David Conran 2016, 2017, 2018, 2019
 *
 * Copyright:
 *   Code for this has been borrowed from lots of other OpenSource projects &
 *   resources. I'm *NOT* claiming complete Copyright ownership of all the code.
 *   Likewise, feel free to borrow from this as much as you want.
 *
 * NOTE: An IR LED circuit SHOULD be connected to ESP8266 GPIO4 (D2) if
 *       you want to send IR messages.
 *       A compatible IR RX modules SHOULD be connected to ESP8266 GPIO14 (D5)
 *       if you want to capture & decode IR nessages.
 *       See 'IR_LED' & 'IR_RX' in IRMQTTServer.h.
 *
 * WARN: This is *very* advanced & complicated example code. Not for beginners.
 *       You are strongly suggested to try & look at other example code first
 *       to understand how this library works.
 *
 * # Instructions
 *
 * ## Before First Boot (i.e. Compile time)
 * - Disable MQTT if desired. (see '#define MQTT_ENABLE' in IRMQTTServer.h).
 *
 * - Site specific settings:
 *   o Search for 'CHANGE_ME' in IRMQTTServer.h for the things you probably
 *     need to change for your particular situation.
 *   o All user changable settings are in the file IRMQTTServer.h.
 *
 * - Arduino IDE:
 *   o Install the following libraries via Library Manager
 *     - ArduinoJson (https://arduinojson.org/) (Version >= 5.x and < 6)
 *     - PubSubClient (https://pubsubclient.knolleary.net/)
 *     - WiFiManager (https://github.com/tzapu/WiFiManager) (Version >= 0.14)
 *   o You MUST change <PubSubClient.h> to have the following (or larger) value:
 *     (with REPORT_RAW_UNKNOWNS 1024 or more is recommended)
 *     #define MQTT_MAX_PACKET_SIZE 768
 * - PlatformIO IDE:
 *     If you are using PlatformIO, this should already been done for you in
 *     the accompanying platformio.ini file.
 *
 * ## First Boot (Initial setup)
 * The ESP8266 board will boot into the WiFiManager's AP mode.
 * i.e. It will create a WiFi Access Point with a SSID like: "ESP123456" etc.
 * Connect to that SSID. Then point your browser to http://192.168.4.1/ and
 * configure the ESP8266 to connect to your desired WiFi network and associated
 * required settings. It will remember these details on next boot if the device
 * connects successfully.
 * More information can be found here:
 *   https://github.com/tzapu/WiFiManager#how-it-works
 *
 * If you need to reset the WiFi and saved settings to go back to "First Boot",
 * visit:  http://<your_esp8266's_ip_address>/reset
 *
 * ## Normal Use (After initial setup)
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
 * Send a MQTT message to the topic 'ir_server/send' (or 'ir_server/send_0' etc)
 * using the following format (Order is important):
 *   protocol_num,hexcode
 *     e.g. 7,E0E09966
 *          which is: Samsung(7), Power On code, default bit size,
 *                    default nr. of repeats.
 *
 *   protocol_num,hexcode,bits
 *     e.g. 4,f50,12
 *          which is: Sony(4), Power Off code, 12 bits & default nr. of repeats.
 *
 *   protocol_num,hexcode,bits,repeats
 *     e.g. 19,C1A2E21D,0,8
 *          which is: Sherwood(19), Vol Up, default bit size & repeated 8 times.
 *
 *   30,frequency,raw_string
 *     e.g. 30,38000,9000,4500,500,1500,500,750,500,750
 *          which is: Raw (30) @ 38kHz with a raw code of
 *            "9000,4500,500,1500,500,750,500,750"
 *
 *   31,code_string
 *     e.g. 31,40000,1,1,96,24,24,24,48,24,24,24,24,24,48,24,24,24,24,24,48,24,24,24,24,24,24,24,24,1058
 *          which is: GlobalCache (31) & "40000,1,1,96,..." (Sony Vol Up)
 *
 *   25,Rrepeats,hex_code_string
 *     e.g. 25,R1,0000,006E,0022,0002,0155,00AA,0015,0040,0015,0040,0015,0015,0015,0015,0015,0015,0015,0015,0015,0015,0015,0040,0015,0040,0015,0015,0015,0040,0015,0015,0015,0015,0015,0015,0015,0040,0015,0015,0015,0015,0015,0040,0015,0040,0015,0015,0015,0015,0015,0015,0015,0015,0015,0015,0015,0040,0015,0015,0015,0015,0015,0040,0015,0040,0015,0040,0015,0040,0015,0040,0015,0640,0155,0055,0015,0E40
 *          which is: Pronto (25), 1 repeat, & "0000 006E 0022 0002 ..."
 *             aka a "Sherwood Amp Tape Input" message.
 *
 *   ac_protocol_num,really_long_hexcode
 *     e.g. 18,190B8050000000E0190B8070000010F0
 *          which is: Kelvinator (18) Air Con on, Low Fan, 25 deg etc.
 *          NOTE: Ensure you zero-pad to the correct number of digits for the
 *                bit/byte size you want to send as some A/C units have units
 *                have different sized messages. e.g. Fujitsu A/C units.
 *
 *   In short:
 *     No spaces after/before commas.
 *     Values are comma separated.
 *     The first value is always in Decimal.
 *     For simple protocols, the next value (hexcode) is always hexadecimal.
 *     The optional bit size is in decimal.
 *     CAUTION: Some AC protocols DO NOT use the really_long_hexcode method.
 *              e.g. < 64bit AC protocols.
 *
 *   Unix command line usage example:
 *     # Install a MQTT client
 *     $ sudo apt install mosquitto-clients
 *     # Send a 32-bit NEC code of 0x1234abcd via MQTT.
 *     $ mosquitto_pub -h 10.0.0.4 -t ir_server/send -m '3,1234abcd,32'
 *
 * This server will send (back) what ever IR message it just transmitted to
 * the MQTT topic 'ir_server/sent' to confirm it has been performed. This works
 * for messages requested via MQTT or via HTTP.
 *
 *   Unix command line usage example:
 *     # Listen to MQTT acknowledgements.
 *     $ mosquitto_sub -h 10.0.0.4 -t ir_server/sent
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
 *     $ mosquitto_sub -h 10.0.0.4 -t ir_server/received
 *
 * Note: General logging messages are also sent to 'ir_server/log' from
 *       time to time.
 *
 * ## Climate (AirCon) interface. (Advanced use)
 * You can now control Air Conditioner devices that have full/detailed support
 * from the IRremoteESP8266 library. See the "Aircon" page for list of supported
 * devices. You can do this via HTTP/HTML or via MQTT.
 *
 * NOTE: It will only change the attributes you change/set. It's up to you to
 *       maintain a consistent set of attributes for your particular aircon.
 *
 * TIP: Use "-1" for 'model' if your A/C doesn't have a specific `setModel()`
 *      or IR class attribute. Most don't. Some do.
 *      e.g. PANASONIC_AC, FUJITSU_AC, WHIRLPOOL_AC
 *
 * ### via MQTT:
 * The code listen for commands (via wildcard) on the MQTT topics at the
 * `ir_server/ac/cmnd/+` level, such as:
 * i.e. protocol, model, power, mode, temp, fanspeed, swingv, swingh, quiet,
 *      turbo, light, beep, econo, sleep, filter, clean, use_celsius
 * e.g. ir_server/ac/cmnd/power, ir_server/ac/cmnd/temp, etc.
 * It will process them, and if successful and it caused a change, it will
 * acknowledge this via the relevant state topic for that command.
 * e.g. If the aircon/climate changes from power off to power on, it will
 *      send an "on" payload to "ir_server/ac/stat/power"
 * NOTE: These "stat" messages have the MQTT retain flag set to on. Thus the
 *       MQTT broker will remember them until reset/restarted etc.
 *
 * The code will also periodically broadcast all possible aircon/climate state
 * attributes to their corresponding "ir_server/ac/stat" topics. This ensures
 * any updates to the ESP's knowledge that may have been lost in transmission
 * are re-communicated. e.g. The MQTT broker being offline.
 * This also helps with Home Assistant MQTT discovery.
 *
 * The program on boot & first successful connection to the MQTT broker, will
 * try to re-acquire any previous aircon/climate state information and act
 * accordingly. This will typically result in A/C IR message being sent as and
 * saved state will probably be different from the defaults.
 *
 * NOTE: Command attributes are processed sequentially.
 *       e.g. Going from "25C, cool, fan low" to "27C, heat, fan high" may go
 *       via "27C, cool, fan low" & "27C, heat, fan low" depending on the order
 *       of arrival & processing of the MQTT commands.
 *
 * ### Home Assistant (HA) MQTT climate integration
 * After you have set the Protocol (required) & Model (if needed) and any of
 * the other misc aircon settings you desire, you can then add the following to
 * your Home Assistant configuration, and it should allow you to
 * control most of the important settings. Google Home/Assistant (via HA)
 * can also control the device, but you will need to configure Home Assistant
 * via it's documentation for that. It has even more limited control.
 * It's far beyond the scope of these instructions to guide you through setting
 * up HA and Google Home integration. See https://www.home-assistant.io/
 *
 * In HA's configuration.yaml, add:
 *
 * climate:
 *   platform: mqtt
 *   name: Living Room Aircon
 *   modes:
 *     - "off"
 *     - "auto"
 *     - "cool"
 *     - "heat"
 *     - "dry"
 *     - "fan_only"
 *   fan_modes:
 *     - "auto"
 *     - "min"
 *     - "low"
 *     - "medium"
 *     - "high"
 *     - "max"
 *   swing_modes:
 *     - "off"
 *     - "auto"
 *     - "highest"
 *     - "high"
 *     - "middle"
 *     - "low"
 *     - "lowest"
 *   power_command_topic: "ir_server/ac/cmnd/power"
 *   mode_command_topic: "ir_server/ac/cmnd/mode"
 *   mode_state_topic: "ir_server/ac/stat/mode"
 *   temperature_command_topic: "ir_server/ac/cmnd/temp"
 *   temperature_state_topic: "ir_server/ac/stat/temp"
 *   fan_mode_command_topic: "ir_server/ac/cmnd/fanspeed"
 *   fan_mode_state_topic: "ir_server/ac/stat/fanspeed"
 *   swing_mode_command_topic: "ir_server/ac/cmnd/swingv"
 *   swing_mode_state_topic: "ir_server/ac/stat/swingv"
 *   min_temp: 16
 *   max_temp: 32
 *   temp_step: 1
 *   retain: false
 *
 * ### via HTTP:
 *   Use the "http://<your_esp8266's_ip_address>/aircon/set" URL and pass on
 *   the arguments as needed to control your device. See the `KEY_*` #defines
 *   in the code for all the parameters.
 *   i.e. protocol, model, power, mode, temp, fanspeed, swingv, swingh, quiet,
 *        turbo, light, beep, econo, sleep, filter, clean, use_celsius
 *   Example:
 *     http://<your_esp8266's_ip_address>/aircon/set?protocol=PANASONIC_AC&model=LKE&power=on&mode=auto&fanspeed=min&temp=23
 *
 * ## Debugging & Logging
 * If DEBUG is turned on, there is additional information printed on the Serial
 * Port. Serial Port output may be disabled if the GPIO is used for IR.
 *
 * If MQTT is enabled, some information/logging is sent to the MQTT topic:
 *   `ir_server/log`
 *
 * ## Updates
 * You can upload new firmware over the air (OTA) via the form on the device's
 * main page. No need to connect to the device again via USB. \o/
 * Your WiFi settings should be remembered between updates. \o/ \o/
 *
 * ## Security
 * <security-hat="on">
 * There is NO authentication set on the HTTP/HTML interface by default (see
 * `HTML_PASSWORD_ENABLE` to change that), and there is NO SSL/TLS (encryption)
 * used by this example code.
 *   i.e. All usernames & passwords are sent in clear text.
 *        All communication to the MQTT server is in clear text.
 *   e.g. This on/using the public Internet is a 'Really Bad Idea<tm>'!
 * You should NOT have or use this code or device exposed on an untrusted and/or
 * unprotected network.
 * If you allow access to OTA firmware updates, then a 'Bad Guy<tm>' could
 * potentially compromise your network. OTA updates are password protected by
 * default. If you are sufficiently paranoid, you SHOULD disable uploading
 * firmware via OTA. (see 'FIRMWARE_OTA')
 * You SHOULD also set/change all usernames & passwords.
 * For extra bonus points: Use a separate untrusted SSID/vlan/network/ segment
 * for your IoT stuff, including this device.
 *             Caveat Emptor. You have now been suitably warned.
 * </security-hat>
 */

#include "IRMQTTServer.h"
#include <Arduino.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ESP8266mDNS.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRtimer.h>
#include <IRutils.h>
#include <IRac.h>
#if MQTT_ENABLE
// --------------------------------------------------------------------
// * * * IMPORTANT * * *
// You must change <PubSubClient.h> to have the following value.
// #define MQTT_MAX_PACKET_SIZE 768
// --------------------------------------------------------------------
#include <PubSubClient.h>
#endif  // MQTT_ENABLE
#include <algorithm>  // NOLINT(build/include)
#include <memory>
#include <string>

// Globals
ESP8266WebServer server(kHttpPort);
#ifdef IR_RX
IRrecv irrecv(IR_RX, kCaptureBufferSize, kCaptureTimeout, true);
decode_results capture;  // Somewhere to store inbound IR messages.
#endif  // IR_RX
MDNSResponder mdns;
WiFiClient espClient;
WiFiManager wifiManager;
bool flagSaveWifiConfig = false;
char HttpUsername[kUsernameLength + 1] = "admin";  // Default HTT username.
char HttpPassword[kPasswordLength + 1] = "";  // No HTTP password by default.
char Hostname[kHostnameLength + 1] = "ir_server";  // Default hostname.
uint16_t *codeArray;
uint32_t lastReconnectAttempt = 0;  // MQTT last attempt reconnection number
bool boot = true;
bool lockIr = false;  // Primitive locking for gating the IR LED.
uint32_t sendReqCounter = 0;
bool lastSendSucceeded = false;  // Store the success status of the last send.
uint32_t lastSendTime = 0;
int8_t offset;  // The calculated period offset for this chip and library.
IRsend *IrSendTable[kSendTableSize];

#ifdef IR_RX
String lastIrReceived = "None";
uint32_t lastIrReceivedTime = 0;
uint32_t irRecvCounter = 0;
#endif  // IR_RX

// Climate stuff
commonAcState_t climate;
commonAcState_t climate_prev;
IRac commonAc(gpioTable[0]);
TimerMs lastClimateIr = TimerMs();  // When we last sent the IR Climate mesg.
uint32_t irClimateCounter = 0;  // How many have we sent?
// Store the success status of the last climate send.
bool lastClimateSucceeded = false;
bool hasClimateBeenSent = false;  // Has the Climate ever been sent?

#if MQTT_ENABLE
PubSubClient mqtt_client(espClient);
String lastMqttCmd = "None";
String lastMqttCmdTopic = "None";
uint32_t lastMqttCmdTime = 0;
uint32_t lastConnectedTime = 0;
uint32_t lastDisconnectedTime = 0;
uint32_t mqttDisconnectCounter = 0;
uint32_t mqttSentCounter = 0;
uint32_t mqttRecvCounter = 0;
bool wasConnected = true;

char MqttServer[kHostnameLength + 1] = "10.0.0.4";
char MqttPort[kPortLength + 1] = "1883";
char MqttUsername[kUsernameLength + 1] = "";
char MqttPassword[kPasswordLength + 1] = "";
char MqttPrefix[kHostnameLength + 1] = "";

String MqttAck;  // Sub-topic we send back acknowledgements on.
String MqttSend;  // Sub-topic we get new commands from.
String MqttRecv;  // Topic we send received IRs to.
String MqttLog;  // Topic we send log messages to.
String MqttLwt;  // Topic for the Last Will & Testament.
String MqttClimate;  // Sub-topic for the climate topics.
String MqttClimateCmnd;  // Sub-topic for the climate command topics.
String MqttClimateStat;  // Sub-topic for the climate stat topics.
String MqttDiscovery;
String MqttHAName;
String MqttClientId;

// Primative lock file for gating MQTT state broadcasts.
bool lockMqttBroadcast = true;
TimerMs lastBroadcast = TimerMs();  // When we last sent a broadcast.
bool hasBroadcastBeenSent = false;
TimerMs lastDiscovery = TimerMs();  // When we last sent a Discovery.
bool hasDiscoveryBeenSent = false;
TimerMs statListenTime = TimerMs();  // How long we've been listening for.
#endif  // MQTT_ENABLE

bool isSerialGpioUsedByIr(void) {
  const uint8_t kSerialTxGpio = 1;  // The GPIO serial output is sent too.
                                    // Note: *DOES NOT* control Serial output.
  // Ensure we are not trodding on anything IR related.
#ifdef IR_RX
  if (IR_RX == kSerialTxGpio)
    return true;  // Serial port is in use by IR capture. Abort.
#endif  // IR_RX
  for (uint8_t i = 0; i < kSendTableSize; i++)
    if (gpioTable[i] == kSerialTxGpio)
      return true;  // Serial port is in use for IR sending. Abort.
  return false;  // Not in use as far as we can tell.
}

// Debug messages get sent to the serial port.
void debug(const char *str) {
#if DEBUG
  if (isSerialGpioUsedByIr()) return;  // Abort.
  uint32_t now = millis();
  Serial.printf("%07u.%03u: %s\n", now / 1000, now % 1000, str);
#endif  // DEBUG
}

// callback notifying us of the need to save the wifi config
void saveWifiConfigCallback(void) {
  debug("saveWifiConfigCallback called.");
  flagSaveWifiConfig = true;
}

void saveWifiConfig(void) {
  debug("Saving the wifi config.");
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
#if MQTT_ENABLE
  json[kMqttServerKey] = MqttServer;
  json[kMqttPortKey] = MqttPort;
  json[kMqttUserKey] = MqttUsername;
  json[kMqttPassKey] = MqttPassword;
  json[kMqttPrefixKey] = MqttPrefix;
#endif  // MQTT_ENABLE
  json[kHostnameKey] = Hostname;
  json[kHttpUserKey] = HttpUsername;
  json[kHttpPassKey] = HttpPassword;

  if (SPIFFS.begin()) {
    File configFile = SPIFFS.open(kConfigFile, "w");
    if (!configFile) {
      debug("Failed to open config file for writing.");
    } else {
      debug("Writing out the config file.");
      json.printTo(configFile);
      configFile.close();
      debug("Finished writing config file.");
    }
    SPIFFS.end();
  }
}

void loadWifiConfigFile(void) {
  debug("Trying to mount SPIFFS");
  if (SPIFFS.begin()) {
    debug("mounted file system");
    if (SPIFFS.exists(kConfigFile)) {
      debug("config file exists");

      File configFile = SPIFFS.open(kConfigFile, "r");
      if (configFile) {
        debug("Opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        if (json.success()) {
          debug("Json config file parsed ok.");
#if MQTT_ENABLE
          strncpy(MqttServer, json[kMqttServerKey] | "", kHostnameLength);
          strncpy(MqttPort, json[kMqttPortKey] | "1883", kPortLength);
          strncpy(MqttUsername, json[kMqttUserKey] | "", kUsernameLength);
          strncpy(MqttPassword, json[kMqttPassKey] | "", kPasswordLength);
          strncpy(MqttPrefix, json[kMqttPrefixKey] | "", kHostnameLength);
#endif  // MQTT_ENABLE
          strncpy(Hostname, json[kHostnameKey] | "", kHostnameLength);
          strncpy(HttpUsername, json[kHttpUserKey] | "", kUsernameLength);
          strncpy(HttpPassword, json[kHttpPassKey] | "", kPasswordLength);
          debug("Recovered Json fields.");
        } else {
          debug("Failed to load json config");
        }
        debug("Closing the config file.");
        configFile.close();
      }
    } else {
      debug("Config file doesn't exist!");
    }
    debug("Unmounting SPIFFS.");
    SPIFFS.end();
  } else {
    debug("Failed to mount SPIFFS");
  }
}

String msToHumanString(uint32_t const msecs) {
  uint32_t totalseconds = msecs / 1000;
  if (totalseconds == 0) return "Now";

  // Note: millis() can only count up to 45 days, so uint8_t is safe.
  uint8_t days = totalseconds / (60 * 60 * 24);
  uint8_t hours = (totalseconds / (60 * 60)) % 24;
  uint8_t minutes = (totalseconds / 60) % 60;
  uint8_t seconds = totalseconds % 60;

  String result = "";
  if (days) result += String(days) + " day";
  if (days > 1) result += 's';
  if (hours) result += ' ' + String(hours) + " hour";
  if (hours > 1) result += 's';
  if (minutes) result += ' ' + String(minutes) + " minute";
  if (minutes > 1) result += 's';
  if (seconds) result += ' ' + String(seconds) + " second";
  if (seconds > 1) result += 's';
  result.trim();
  return result;
}

String timeElapsed(uint32_t const msec) {
  String result = msToHumanString(msec);
  if (result.equalsIgnoreCase("Now"))
    return result;
  else
    return result + " ago";
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
  return msToHumanString(diff) + " ago";
}

// Return a string containing the comma separated list of sending gpios.
String listOfSendGpios(void) {
  String result = String(gpioTable[0]);
  if (kSendTableSize > 1) result += " (default)";
  for (uint8_t i = 1; i < kSendTableSize; i++) {
    result += ", " + String(gpioTable[i]);
  }
  return result;
}

String htmlMenu(void) {
  return F(
      "<center>"
        "<button type='button' "
          "onclick='window.location=\"/\"'>"
          "Home"
        "</button>"
        "<button type='button' "
          "onclick='window.location=\"/aircon\"'>"
          "Aircon"
        "</button>"
        "<button type='button' "
          "onclick='window.location=\"/examples\"'>"
          "Examples"
        "</button>"
        "<button type='button' "
          "onclick='window.location=\"/info\"'>"
          "System Info"
        "</button>"
        "<button type='button' "
          "onclick='window.location=\"/admin\"'>"
          "Admin"
        "</button>"
      "</center>"
      "<hr>");
}

// Root web page with example usage etc.
void handleRoot(void) {
#if HTML_PASSWORD_ENABLE
  if (!server.authenticate(HttpUsername, HttpPassword)) {
    debug("Basic HTTP authentication failure for /.");
    return server.requestAuthentication();
  }
#endif
  String html = F(
    "<html><head><title>IR MQTT server</title></head>"
    "<body>"
    "<center><h1>ESP8266 IR MQTT Server</h1></center>"
    "<center><small><i>" _MY_VERSION_ "</i></small></center>");
  html += htmlMenu();
  html += F(
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
        "<option value='58'>LEGOPF</option>"
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
        "<option value='56'>Samsung36</option>"
        "<option value='11'>Sanyo</option>"
        "<option value='22'>Sanyo LC7461</option>"
        "<option value='14'>Sharp</option>"
        "<option value='19'>Sherwood</option>"
        "<option value='4'>Sony</option>"
        "<option value='54'>Vestel AC</option>"
        "<option value='55'>Teco AC</option>"
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
    "<h3>Send a complex (Air Conditioner) IR message</h3><p>"
    "<form method='POST' action='/ir' enctype='multipart/form-data'>"
      "Type: "
      "<select name='type'>"
        "<option value='27'>Argo</option>"
        "<option value='16'>Daikin</option>"
        "<option value='53'>Daikin2</option>"
        "<option value='61'>Daikin216 (27 bytes)</option>"
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
        "<option value='59'>Mitsubishi Heavy (11 bytes)</option>"
        "<option value='60'>Mitsubishi Heavy (19 bytes)</option>"
        "<option value='52'>MWM</option>"
        "<option value='46'>Samsung</option>"
        "<option value='57'>TCL112</option>"
        "<option value='32'>Toshiba</option>"
        "<option value='28'>Trotec</option>"
        "<option value='45'>Whirlpool</option>"
      "</select>"
      " State code: 0x"
      "<input type='text' name='code' size='");
  html += String(kStateSizeMax * 2);
  html += F("' maxlength='");
  html += String(kStateSizeMax * 2);
  html += F("'"
          " value='190B8050000000E0190B8070000010F0'>"
      " <input type='submit' value='Send A/C State'>"
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
    "<br></body></html>");
  server.send(200, "text/html", html);
}

String addJsReloadUrl(const String url, const uint16_t timeout_s,
                      const bool notify) {
  String html = F(
      "<script type=\"text/javascript\">\n"
      "<!--\n"
      "  function Redirect() {\n"
      "    window.location=\"");
  html += url;
  html += F("\";\n"
      "  }\n"
      "\n");
  if (notify && timeout_s) {
    html += F("  document.write(\"You will be redirected to the main page in ");
    html += String(timeout_s);
    html += F(" seconds.\");\n");
  }
  html += F("  setTimeout('Redirect()', ");
  html += String(timeout_s * 1000);  // Convert to mSecs
  html += F(");\n"
      "//-->\n"
      "</script>\n");
  return html;
}

// Web page with hardcoded example usage etc.
void handleExamples(void) {
#if HTML_PASSWORD_ENABLE
  if (!server.authenticate(HttpUsername, HttpPassword)) {
    debug("Basic HTTP authentication failure for /examples.");
    return server.requestAuthentication();
  }
#endif
  String html = F(
    "<html><head><title>IR MQTT examples</title></head>"
    "<body>"
    "<center><h1>ESP8266 IR MQTT Server</h1></center>"
    "<center><small><i>" _MY_VERSION_ "</i></small></center>");
  html += htmlMenu();
  html += F(
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
    "<p><a href=\"aircon/set?protocol=PANASONIC_AC&model=LKE&power=on&"
      "mode=auto&fanspeed=min&temp=23\">"
      "Panasonic A/C LKE model, On, Auto mode, Min fan, 23C"
      " <i>(via HTTP aircon interface)</i></a></p>"
    "<p><a href=\"aircon/set?temp=27\">"
      "Change just the temp to 27C <i>(via HTTP aircon interface)</i></a></p>"
    "<p><a href=\"aircon/set?power=off&mode=off\">"
      "Turn OFF the current A/C <i>(via HTTP aircon interface)</i></a></p>"
    "<br><hr></body></html>");
  server.send(200, "text/html", html);
}

String boolToString(const bool value) {
  return value ? F("on") : F("off");
}


String opmodeToString(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kOff:
      return F("off");
    case stdAc::opmode_t::kAuto:
      return F("auto");
    case stdAc::opmode_t::kCool:
      return F("cool");
    case stdAc::opmode_t::kHeat:
      return F("heat");
    case stdAc::opmode_t::kDry:
      return F("dry");
    case stdAc::opmode_t::kFan:
      return F("fan_only");
    default:
      return F("unknown");
  }
}

String fanspeedToString(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kAuto:
      return F("auto");
    case stdAc::fanspeed_t::kMax:
      return F("max");
    case stdAc::fanspeed_t::kHigh:
      return F("high");
    case stdAc::fanspeed_t::kMedium:
      return F("medium");
    case stdAc::fanspeed_t::kLow:
      return F("low");
    case stdAc::fanspeed_t::kMin:
      return F("min");
    default:
      return F("unknown");
  }
}

String swingvToString(const stdAc::swingv_t swingv) {
  switch (swingv) {
    case stdAc::swingv_t::kOff:
      return F("off");
    case stdAc::swingv_t::kAuto:
      return F("auto");
    case stdAc::swingv_t::kHighest:
      return F("highest");
    case stdAc::swingv_t::kHigh:
      return F("high");
    case stdAc::swingv_t::kMiddle:
      return F("middle");
    case stdAc::swingv_t::kLow:
      return F("low");
    case stdAc::swingv_t::kLowest:
      return F("lowest");
    default:
      return F("unknown");
  }
}

String swinghToString(const stdAc::swingh_t swingh) {
  switch (swingh) {
    case stdAc::swingh_t::kOff:
      return F("off");
    case stdAc::swingh_t::kAuto:
      return F("auto");
    case stdAc::swingh_t::kLeftMax:
      return F("leftmax");
    case stdAc::swingh_t::kLeft:
      return F("left");
    case stdAc::swingh_t::kMiddle:
      return F("middle");
    case stdAc::swingh_t::kRight:
      return F("right");
    case stdAc::swingh_t::kRightMax:
      return F("rightmax");
    default:
      return F("unknown");
  }
}

String htmlSelectBool(const String name, const bool def) {
  String html = "<select name='" + name + "'>";
  for (uint16_t i = 0; i < 2; i++) {
    html += F("<option value='");
    html += boolToString(i);
    html += '\'';
    if (i == def) html += F(" selected='selected'");
    html += '>';
    html += boolToString(i);
    html += F("</option>");
  }
  html += F("</select>");
  return html;
}

String htmlSelectProtocol(const String name, const decode_type_t def) {
  String html = "<select name='" + name + "'>";
  for (uint8_t i = 1; i <= decode_type_t::kLastDecodeType; i++) {
    if (IRac::isProtocolSupported((decode_type_t)i)) {
      html += F("<option value='");
      html += String(i);
      html += '\'';
      if (i == def) html += F(" selected='selected'");
      html += '>';
      html += typeToString((decode_type_t)i);
      html += F("</option>");
    }
  }
  html += F("</select>");
  return html;
}

String htmlSelectModel(const String name, const int16_t def) {
  String html = "<select name='" + name + "'>";
  for (int16_t i = -1; i <= 6; i++) {
    String num = String(i);
    html += F("<option value='");
    html += num;
    html += '\'';
    if (i == def) html += F(" selected='selected'");
    html += '>';
    if (i == -1)
      html += F("Default");
    else if (i == 0)
      html += F("Unknown");
    else
      html += num;
    html += F("</option>");
  }
  html += F("</select>");
  return html;
}

String htmlSelectMode(const String name, const stdAc::opmode_t def) {
  String html = "<select name='" + name + "'>";
  for (int8_t i = -1; i <= 4; i++) {
    String mode = opmodeToString((stdAc::opmode_t)i);
    html += F("<option value='");
    html += mode;
    html += '\'';
    if ((stdAc::opmode_t)i == def) html += F(" selected='selected'");
    html += '>';
    html += mode;
    html += F("</option>");
  }
  html += F("</select>");
  return html;
}

String htmlSelectFanspeed(const String name, const stdAc::fanspeed_t def) {
  String html = "<select name='" + name + "'>";
  for (int8_t i = 0; i <= 5; i++) {
    String speed = fanspeedToString((stdAc::fanspeed_t)i);
    html += F("<option value='");
    html += speed;
    html += '\'';
    if ((stdAc::fanspeed_t)i == def) html += F(" selected='selected'");
    html += '>';
    html += speed;
    html += F("</option>");
  }
  html += F("</select>");
  return html;
}

String htmlSelectSwingv(const String name, const stdAc::swingv_t def) {
  String html = "<select name='" + name + "'>";
  for (int8_t i = -1; i <= 5; i++) {
    String swing = swingvToString((stdAc::swingv_t)i);
    html += F("<option value='");
    html += swing;
    html += '\'';
    if ((stdAc::swingv_t)i == def) html += F(" selected='selected'");
    html += '>';
    html += swing;
    html += F("</option>");
  }
  html += F("</select>");
  return html;
}

String htmlSelectSwingh(const String name, const stdAc::swingh_t def) {
  String html = "<select name='" + name + "'>";
  for (int8_t i = -1; i <= 5; i++) {
    String swing = swinghToString((stdAc::swingh_t)i);
    html += F("<option value='");
    html += swing;
    html += '\'';
    if ((stdAc::swingh_t)i == def) html += F(" selected='selected'");
    html += '>';
    html += swing;
    html += F("</option>");
  }
  html += F("</select>");
  return html;
}

// Admin web page
void handleAirCon(void) {
  String html = F(
    "<html><head><title>AirCon control</title></head>"
    "<body>"
    "<center><h1>Air Conditioner Control</h1></center>");
  html += htmlMenu();
  html += "<h3>Current Settings</h3>"
      "<form method='POST' action='/aircon/set' enctype='multipart/form-data'>"
      "<table style='width:33%'>"
      "<tr><td>Protocol</td><td>" +
          htmlSelectProtocol(KEY_PROTOCOL, climate.protocol) + "</td></tr>"
      "<tr><td>Model</td><td>" + htmlSelectModel(KEY_MODEL, climate.model) +
          "</td></tr>"
      "<tr><td>Power</td><td>" + htmlSelectBool(KEY_POWER, climate.power) +
          "</td></tr>"
      "<tr><td>Mode</td><td>" + htmlSelectMode(KEY_MODE, climate.mode) +
          "</td></tr>"
      "<tr><td>Temp</td><td>"
          "<input type='number' name='" KEY_TEMP "' min='16' max='90' "
          "step='0.5' value='" + String(climate.degrees, 1) + "'>"
          "<select name='" KEY_CELSIUS "'>"
              "<option value='on'" +
              (climate.celsius ? " selected='selected'" : "") + ">C</option>"
              "<option value='off'" +
              (!climate.celsius ? " selected='selected'" : "") + ">F</option>"
          "</select></td></tr>"
      "<tr><td>Fan Speed</td><td>" +
          htmlSelectFanspeed(KEY_FANSPEED, climate.fanspeed) + "</td></tr>"
      "<tr><td>Swing (V)</td><td>" +
          htmlSelectSwingv(KEY_SWINGV, climate.swingv) + "</td></tr>"
      "<tr><td>Swing (H)</td><td>" +
          htmlSelectSwingh(KEY_SWINGH, climate.swingh) + "</td></tr>"
      "<tr><td>Quiet</td><td>" + htmlSelectBool(KEY_QUIET, climate.quiet) +
          "</td></tr>"
      "<tr><td>Turbo</td><td>" + htmlSelectBool(KEY_TURBO, climate.turbo) +
          "</td></tr>"
      "<tr><td>Econo</td><td>" + htmlSelectBool(KEY_ECONO, climate.econo) +
          "</td></tr>"
      "<tr><td>Light</td><td>" + htmlSelectBool(KEY_LIGHT, climate.light) +
          "</td></tr>"
      "<tr><td>Filter</td><td>" + htmlSelectBool(KEY_FILTER, climate.filter) +
          "</td></tr>"
      "<tr><td>Clean</td><td>" + htmlSelectBool(KEY_CLEAN, climate.clean) +
          "</td></tr>"
      "<tr><td>Beep</td><td>" + htmlSelectBool(KEY_BEEP, climate.beep) +
          "</td></tr>"
      "</table>"
      "<input type='submit' value='Update & Send'>"
      "</form>";
  // Display the current settings.
  html += F("</body></html>");
  server.send(200, "text/html", html);
}

// Parse the URL args to find the Common A/C arguments.
void handleAirConSet(void) {
#if HTML_PASSWORD_ENABLE
  if (!server.authenticate(HttpUsername, HttpPassword)) {
    debug("Basic HTTP authentication failure for /aircon/set.");
    return server.requestAuthentication();
  }
#endif
  commonAcState_t result = climate;
  debug("New common a/c received via HTTP");
  for (uint16_t i = 0; i < server.args(); i++)
    result = updateClimate(result, server.argName(i), "", server.arg(i));

#if MQTT_ENABLE
  sendClimate(climate, result, MqttClimateStat,
              true, false, false);
#else  // MQTT_ENABLE
  sendClimate(climate, result, "", false, false, false);
#endif  // MQTT_ENABLE
  // Update the old climate state with the new one.
  climate = result;
  // Redirect back to the aircon page.
  String html = F(
      "<html><head><title>Update Aircon</title></head>"
      "<body>"
      "<center><h1>Aircon updated!</h1></center>");
  html += addJsReloadUrl("/aircon", 2, false);
  html += F("</body></html>");
  server.send(200, "text/html", html);
}

// Admin web page
void handleAdmin(void) {
  String html = F(
    "<html><head><title>IR MQTT server admin</title></head>"
    "<body>"
    "<center><h1>Administration</h1></center>");
  html += htmlMenu();
  html += F(
    "<h3>Special commands</h3>"
#if MQTT_ENABLE
      "<button type='button' "
        "onclick='window.location=\"/send_discovery\"'>"
        "Send MQTT Discovery"
      "</button> "
      "Send a Climate MQTT discovery message to Home Assistant.<br><br>"
#endif  // MQTT_ENABLE
      "<button type='button' "
        "onclick='window.location=\"/quitquitquit\"'>"
        "Reboot"
      "</button> A simple reboot of the ESP8266. "
          "<small>ie. No changes</small><br><br>"
      "<button type='button' "
        "onclick='window.location=\"/reset\"'>"
        "Wipe Settings"
      "</button> <mark>Warning:</mark> "
      "Resets the device back to original settings. "
          "<small>ie. Goes back to AP/Setup mode.</small><br>");
#if FIRMWARE_OTA
  html += F("<hr><h3>Update firmware</h3><p>"
          "<b><mark>Warning:</mark></b><br> ");
  if (!strlen(HttpPassword))  // Deny if password not set
    html += F("<i>OTA firmware is disabled until you set a password. "
              "You will need to <a href='/reset'>wipe & reset</a> to set one."
              "</i><br><br>");
  else  // default password has been changed, so allow it.
    html += F(
        "<i>Updating your firmware may screw up your access to the device. "
        "If you are going to use this, know what you are doing first "
        "(and you probably do).</i><br>"
        "<form method='POST' action='/update' enctype='multipart/form-data'>"
          "Firmware to upload: <input type='file' name='update'>"
          "<input type='submit' value='Update'>"
        "</form>");
#endif  // FIRMWARE_OTA
  html += F("</body></html>");
  server.send(200, "text/html", html);
}

// Info web page
void handleInfo(void) {
  String html =
    "<html><head><title>IR MQTT server info</title></head>"
    "<body>"
    "<center><h1>Information</h1></center>";
  html += htmlMenu();
  html +=
    "<h3>General</h3>"
    "<p>Hostname: " + String(Hostname) + "<br>"
    "IP address: " + WiFi.localIP().toString() + "<br>"
    "Booted: " + timeSince(1) + "<br>" +
    "Version: " _MY_VERSION_ "<br>"
    "Built: " __DATE__
      " " __TIME__ "<br>"
    "Period Offset: " + String(offset) + "us<br>"
    "IR Lib Version: " _IRREMOTEESP8266_VERSION_ "<br>"
    "ESP8266 Core Version: " + ESP.getCoreVersion() + "<br>"
    "IR Send GPIO(s): " + listOfSendGpios() + "<br>"
    "Total send requests: " + String(sendReqCounter) + "<br>"
    "Last message sent: " + String(lastSendSucceeded ? "Ok" : "FAILED") +
    " <i>(" + timeSince(lastSendTime) + ")</i><br>"
#ifdef IR_RX
    "IR Recv GPIO: " + String(IR_RX) +
#if IR_RX_PULLUP
    " (pullup)"
#endif  // IR_RX_PULLUP
    "<br>"
    "Total IR Received: " + String(irRecvCounter) + "<br>"
    "Last IR Received: " + lastIrReceived +
    " <i>(" + timeSince(lastIrReceivedTime) + ")</i><br>"
#endif  // IR_RX
    "Duplicate Wifi networks: " +
        String(HIDE_DUPLIATE_NETWORKS ? "Hide" : "Show") + "<br>"
    "Min Wifi signal required: "
#ifdef MIN_SIGNAL_STRENGTH
        + String(static_cast<int>(MIN_SIGNAL_STRENGTH)) +
#else  // MIN_SIGNAL_STRENGTH
        "8"
#endif  // MIN_SIGNAL_STRENGTH
        "%<br>"
    "Serial debugging: "
#if DEBUG
        + String(isSerialGpioUsedByIr() ? "Off" : "On") +
#else  // DEBUG
        "Off"
#endif  // DEBUG
        "<br>"
    "</p>"
#if MQTT_ENABLE
    "<h4>MQTT Information</h4>"
    "<p>Server: " + String(MqttServer) + ":" + String(MqttPort) + " <i>(" +
    (mqtt_client.connected() ? "Connected " + timeSince(lastDisconnectedTime)
                             : "Disconnected " + timeSince(lastConnectedTime)) +
    ")</i><br>"
    "Disconnections: " + String(mqttDisconnectCounter - 1) + "<br>"
    "Client id: " + MqttClientId + "<br>"
    "Command topic(s): " + listOfCommandTopics() + "<br>"
    "Acknowledgements topic: " + MqttAck + "<br>"
#ifdef IR_RX
    "IR Received topic: " + MqttRecv + "<br>"
#endif  // IR_RX
    "Log topic: " + MqttLog + "<br>"
    "LWT topic: " + MqttLwt + "<br>"
    "QoS: " + String(QOS) + "<br>"
    // lastMqttCmd* is unescaped untrusted input.
    // Avoid any possible HTML/XSS when displaying it.
    "Last MQTT command seen: (topic) '" + htmlEscape(lastMqttCmdTopic) +
         "' (payload) '" + htmlEscape(lastMqttCmd) + "' <i>(" +
         timeSince(lastMqttCmdTime) + ")</i><br>"
    "Total published: " + String(mqttSentCounter) + "<br>"
    "Total received: " + String(mqttRecvCounter) + "<br>"
    "</p>"
#endif  // MQTT_ENABLE
    "<h4>Climate Information</h4>"
    "<p>"
    "IR Send GPIO: " + String(gpioTable[0]) + "<br>"
    "Total sent: " + String(irClimateCounter) + "<br>"
    "Last send: " + String(hasClimateBeenSent ?
        (String(lastClimateSucceeded ? "Ok" : "FAILED") +
         " <i>(" + timeElapsed(lastClimateIr.elapsed()) + ")</i>") :
        "<i>Never</i>") + "<br>"
#if MQTT_ENABLE
    "State listen period: " + msToHumanString(kStatListenPeriodMs) + "<br>"
    "State broadcast period: " + msToHumanString(kBroadcastPeriodMs) + "<br>"
    "Last state broadcast: " + (hasBroadcastBeenSent ?
        timeElapsed(lastBroadcast.elapsed()) :
        String("<i>Never</i>")) + "<br>"
    "Last discovery sent: " + (lockMqttBroadcast ?
        String("<b>Locked</b>") :
        (hasDiscoveryBeenSent ?
            timeElapsed(lastDiscovery.elapsed()) :
            String("<i>Never</i>"))) +
        "<br>"
    "Command topics: " + MqttClimateCmnd +
      "(" KEY_PROTOCOL "|" KEY_MODEL "|" KEY_POWER "|" KEY_MODE "|" KEY_TEMP "|"
          KEY_FANSPEED "|" KEY_SWINGV "|" KEY_SWINGH "|" KEY_QUIET "|"
          KEY_TURBO "|" KEY_LIGHT "|" KEY_BEEP "|" KEY_ECONO "|" KEY_SLEEP "|"
          KEY_CLOCK "|" KEY_FILTER "|" KEY_CLEAN "|" KEY_CELSIUS ")<br>"
    "State topics: " + MqttClimateStat +
      "(" KEY_PROTOCOL "|" KEY_MODEL "|" KEY_POWER "|" KEY_MODE "|" KEY_TEMP "|"
          KEY_FANSPEED "|" KEY_SWINGV "|" KEY_SWINGH "|" KEY_QUIET "|"
          KEY_TURBO "|" KEY_LIGHT "|" KEY_BEEP "|" KEY_ECONO "|" KEY_SLEEP "|"
          KEY_CLOCK "|" KEY_FILTER "|" KEY_CLEAN "|" KEY_CELSIUS ")<br>"
#endif  // MQTT_ENABLE
    "</p>"
    // Page footer
    "<hr><p><small><center>"
      "<i>(Note: Page will refresh every 60 seconds.)</i>"
    "<centre></small></p>";
  html += addJsReloadUrl("/info", 60, false);
  html += "</body></html>";
  server.send(200, "text/html", html);
}
// Reset web page
void handleReset(void) {
#if HTML_PASSWORD_ENABLE
  if (!server.authenticate(HttpUsername, HttpPassword)) {
    debug("Basic HTTP authentication failure for /reset.");
    return server.requestAuthentication();
  }
#endif
  server.send(200, "text/html",
    "<html><head><title>Reset WiFi Config</title></head>"
    "<body>"
    "<h1>Resetting the WiFiManager config back to defaults.</h1>"
    "<p>Device restarting. Try connecting in a few seconds.</p>" +
    addJsReloadUrl("/", 10, true) +
    "</body></html>");
  // Do the reset.
#if MQTT_ENABLE
  mqttLog("Wiping all saved config settings.");
#endif  // MQTT_ENABLE
  debug("Trying to mount SPIFFS");
  if (SPIFFS.begin()) {
    debug("Removing JSON config file");
    SPIFFS.remove(kConfigFile);
    SPIFFS.end();
  }
  delay(1000);
  debug("Reseting wifiManager's settings.");
  wifiManager.resetSettings();
  delay(1000);
  debug("rebooting...");
  ESP.restart();
  delay(1000);
}

// Reboot web page
void handleReboot() {
#if HTML_PASSWORD_ENABLE
  if (!server.authenticate(HttpUsername, HttpPassword)) {
    debug("Basic HTTP authentication failure for /quitquitquit.");
    return server.requestAuthentication();
  }
#endif
  server.send(200, "text/html",
    "<html><head><title>Rebooting</title></head>"
    "<body>"
    "<h1>Device restarting.</h1>"
    "<p>Try connecting in a few seconds.</p>" +
    addJsReloadUrl("/", 15, true) +
    "</body></html>");
#if MQTT_ENABLE
  mqttLog("Reboot requested");
#endif  // MQTT_ENABLE
  // Do the reset.
  delay(1000);
  ESP.restart();
  delay(1000);
}

// Parse an Air Conditioner A/C Hex String/code and send it.
// Args:
//   irsend: A Ptr to the IRsend object to transmit via.
//   irType: Nr. of the protocol we need to send.
//   str: A hexadecimal string containing the state to be sent.
// Returns:
//   bool: Successfully sent or not.
bool parseStringAndSendAirCon(IRsend *irsend, const uint16_t irType,
                              const String str) {
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
      // Daikin has 2 different possible size states.
      // (The correct size, and a legacy shorter size.)
      // Guess which one we are being presented with based on the number of
      // hexadecimal digits provided. i.e. Zero-pad if you need to to get
      // the correct length/byte size.
      // This should provide backward compatiblity with legacy messages.
      stateSize = inputLength / 2;  // Every two hex chars is a byte.
      // Use at least the minimum size.
      stateSize = std::max(stateSize, kDaikinStateLengthShort);
      // If we think it isn't a "short" message.
      if (stateSize > kDaikinStateLengthShort)
        // Then it has to be at least the version of the "normal" size.
        stateSize = std::max(stateSize, kDaikinStateLength);
      // Lastly, it should never exceed the "normal" size.
      stateSize = std::min(stateSize, kDaikinStateLength);
      break;
    case DAIKIN2:
      stateSize = kDaikin2StateLength;
      break;
    case DAIKIN216:
      stateSize = kDaikin216StateLength;
      break;
    case ELECTRA_AC:
      stateSize = kElectraAcStateLength;
      break;
    case MITSUBISHI_AC:
      stateSize = kMitsubishiACStateLength;
      break;
    case MITSUBISHI_HEAVY_88:
      stateSize = kMitsubishiHeavy88StateLength;
      break;
    case MITSUBISHI_HEAVY_152:
      stateSize = kMitsubishiHeavy152StateLength;
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
    case TCL112AC:
      stateSize = kTcl112AcStateLength;
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
      debug("Aborting! Non-hexadecimal char found in AirCon state:");
      debug(str.c_str());
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
      irsend->sendKelvinator(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_TOSHIBA_AC
    case TOSHIBA_AC:
      irsend->sendToshibaAC(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_DAIKIN
    case DAIKIN:
      irsend->sendDaikin(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_DAIKIN2
    case DAIKIN2:
      irsend->sendDaikin2(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_DAIKIN216
    case DAIKIN216:
      irsend->sendDaikin216(reinterpret_cast<uint8_t *>(state));
      break;
#endif  // SEND_DAIKIN216
#if SEND_MITSUBISHI_AC
    case MITSUBISHI_AC:
      irsend->sendMitsubishiAC(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_MITSUBISHIHEAVY
    case MITSUBISHI_HEAVY_88:  // 59
      irsend->sendMitsubishiHeavy88(reinterpret_cast<uint8_t *>(state));
      break;
    case MITSUBISHI_HEAVY_152:  // 60
      irsend->sendMitsubishiHeavy152(reinterpret_cast<uint8_t *>(state));
      break;
#endif  // SEND_MITSUBISHIHEAVY
#if SEND_TROTEC
    case TROTEC:
      irsend->sendTrotec(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_ARGO
    case ARGO:
      irsend->sendArgo(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_GREE
    case GREE:
      irsend->sendGree(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_FUJITSU_AC
    case FUJITSU_AC:
      irsend->sendFujitsuAC(reinterpret_cast<uint8_t *>(state), stateSize);
      break;
#endif
#if SEND_HAIER_AC
    case HAIER_AC:
      irsend->sendHaierAC(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_HAIER_AC_YRW02
    case HAIER_AC_YRW02:
      irsend->sendHaierACYRW02(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_HITACHI_AC
    case HITACHI_AC:
      irsend->sendHitachiAC(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_HITACHI_AC1
    case HITACHI_AC1:
      irsend->sendHitachiAC1(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_HITACHI_AC2
    case HITACHI_AC2:
      irsend->sendHitachiAC2(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_WHIRLPOOL_AC
    case WHIRLPOOL_AC:
      irsend->sendWhirlpoolAC(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_SAMSUNG_AC
    case SAMSUNG_AC:
      irsend->sendSamsungAC(reinterpret_cast<uint8_t *>(state), stateSize);
      break;
#endif
#if SEND_ELECTRA_AC
    case ELECTRA_AC:
      irsend->sendElectraAC(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_PANASONIC_AC
    case PANASONIC_AC:
      irsend->sendPanasonicAC(reinterpret_cast<uint8_t *>(state));
      break;
#endif
#if SEND_MWM
    case MWM:
      irsend->sendMWM(reinterpret_cast<uint8_t *>(state), stateSize);
      break;
#endif
#if SEND_TCL112AC
    case TCL112AC:
      irsend->sendTcl112Ac(reinterpret_cast<uint8_t *>(state));
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
//   irsend: A ptr to the IRsend object to transmit via.
//   str: A GlobalCache formatted String of comma separated numbers.
//        e.g. "38000,1,1,170,170,20,63,20,63,20,63,20,20,20,20,20,20,20,20,20,
//              20,20,63,20,63,20,63,20,20,20,20,20,20,20,20,20,20,20,20,20,63,
//              20,20,20,20,20,20,20,20,20,20,20,20,20,63,20,20,20,63,20,63,20,
//              63,20,63,20,63,20,63,20,1798"
//        Note: The leading "1:1,1," of normal GC codes should be removed.
// Returns:
//   bool: Successfully sent or not.
bool parseStringAndSendGC(IRsend *irsend, const String str) {
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

  irsend->sendGC(code_array, count);  // All done. Send it.
  free(code_array);  // Free up the memory allocated.
  if (count > 0)
    return true;  // We sent something.
  return false;  // We probably didn't.
}
#endif  // SEND_GLOBALCACHE

#if SEND_PRONTO
// Parse a Pronto Hex String/code and send it.
// Args:
//   irsend: A ptr to the IRsend object to transmit via.
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
bool parseStringAndSendPronto(IRsend *irsend, const String str,
                              uint16_t repeats) {
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

  irsend->sendPronto(code_array, count, repeats);  // All done. Send it.
  free(code_array);  // Free up the memory allocated.
  if (count > 0)
    return true;  // We sent something.
  return false;  // We probably didn't.
}
#endif  // SEND_PRONTO

#if SEND_RAW
// Parse an IRremote Raw Hex String/code and send it.
// Args:
//   irsend: A ptr to the IRsend object to transmit via.
//   str: A comma-separated String containing the freq and raw IR data.
//        e.g. "38000,9000,4500,600,1450,600,900,650,1500,..."
//        Requires at least two comma-separated values.
//        First value is the transmission frequency in Hz or kHz.
// Returns:
//   bool: Successfully sent or not.
bool parseStringAndSendRaw(IRsend *irsend, const String str) {
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

  irsend->sendRaw(raw_array, count, freq);  // All done. Send it.
  free(raw_array);  // Free up the memory allocated.
  if (count > 0)
    return true;  // We sent something.
  return false;  // We probably didn't.
}
#endif  // SEND_RAW

// Parse the URL args to find the IR code.
void handleIr(void) {
#if HTML_PASSWORD_ENABLE
  if (!server.authenticate(HttpUsername, HttpPassword)) {
    debug("Basic HTTP authentication failure for /ir.");
    return server.requestAuthentication();
  }
#endif
  uint64_t data = 0;
  String data_str = "";
  int16_t ir_type = decode_type_t::NEC;  // Default to NEC codes.
  uint16_t nbits = 0;
  uint16_t repeat = 0;

  for (uint16_t i = 0; i < server.args(); i++) {
    if (server.argName(i).equals(KEY_TYPE) ||
        server.argName(i).equals(KEY_PROTOCOL)) {
      ir_type = strToDecodeType(server.arg(i).c_str());
    } else if (server.argName(i).equals(KEY_CODE)) {
      data = getUInt64fromHex(server.arg(i).c_str());
      data_str = server.arg(i);
    } else if (server.argName(i).equals(KEY_BITS)) {
      nbits = server.arg(i).toInt();
    } else if (server.argName(i).equals(KEY_REPEAT)) {
      repeat = server.arg(i).toInt();
    }
  }
  debug("New code received via HTTP");
  lastSendSucceeded = sendIRCode(IrSendTable[0], ir_type, data,
                                 data_str.c_str(), nbits, repeat);
  String html = F(
     "<html><head><title>Send IR command</title></head>"
     "<body>"
     "<center><h1>IR command sent!</h1></center>");
  html += addJsReloadUrl("/", 2, true);
  html += F("</body></html>");
  server.send(200, "text/html", html);
}

void handleNotFound(void) {
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

void setup_wifi(void) {
  delay(10);
  loadWifiConfigFile();
  // We start by connecting to a WiFi network
  wifiManager.setTimeout(300);  // Time out after 5 mins.
  // Set up additional parameters for WiFiManager config menu page.
  wifiManager.setSaveConfigCallback(saveWifiConfigCallback);
  WiFiManagerParameter custom_hostname_text(
      "<br><center>Hostname</center>");
  wifiManager.addParameter(&custom_hostname_text);
  WiFiManagerParameter custom_hostname(
      kHostnameKey, kHostnameKey, Hostname, kHostnameLength);
  wifiManager.addParameter(&custom_hostname);
  WiFiManagerParameter custom_authentication_text(
      "<br><br><center>Web/OTA authentication</center>");
  wifiManager.addParameter(&custom_authentication_text);
  WiFiManagerParameter custom_http_username(
      kHttpUserKey, "username", HttpUsername, kUsernameLength);
  wifiManager.addParameter(&custom_http_username);
  WiFiManagerParameter custom_http_password(
      kHttpPassKey, "password (No OTA if blank)", HttpPassword, kPasswordLength,
      " type='password'");
  wifiManager.addParameter(&custom_http_password);
#if MQTT_ENABLE
  WiFiManagerParameter custom_mqtt_text(
      "<br><br><center>MQTT Broker details</center>");
  wifiManager.addParameter(&custom_mqtt_text);
  WiFiManagerParameter custom_mqtt_server(
      kMqttServerKey, "mqtt server", MqttServer, kHostnameLength);
  wifiManager.addParameter(&custom_mqtt_server);
  WiFiManagerParameter custom_mqtt_port(
      kMqttPortKey, "mqtt port", MqttPort, kPortLength,
      " input type='number' min='1' max='65535'");
  wifiManager.addParameter(&custom_mqtt_port);
  WiFiManagerParameter custom_mqtt_user(
      kMqttUserKey, "mqtt username", MqttUsername, kUsernameLength);
  wifiManager.addParameter(&custom_mqtt_user);
  WiFiManagerParameter custom_mqtt_pass(
      kMqttPassKey, "mqtt password", MqttPassword, kPasswordLength,
      " type='password'");
  wifiManager.addParameter(&custom_mqtt_pass);
  WiFiManagerParameter custom_prefix_text(
      "<br><br><center>MQTT Prefix</center>");
  wifiManager.addParameter(&custom_prefix_text);
  WiFiManagerParameter custom_mqtt_prefix(
      kMqttPrefixKey, "Leave empty to use Hostname", MqttPrefix,
      kHostnameLength);
  wifiManager.addParameter(&custom_mqtt_prefix);
  #endif  // MQTT_ENABLE
#if USE_STATIC_IP
  // Use a static IP config rather than the one supplied via DHCP.
  wifiManager.setSTAStaticIPConfig(kIPAddress, kGateway, kSubnetMask);
#endif  // USE_STATIC_IP
#if MIN_SIGNAL_STRENGTH
  wifiManager.setMinimumSignalQuality(MIN_SIGNAL_STRENGTH);
#endif  // MIN_SIGNAL_STRENGTH
  wifiManager.setRemoveDuplicateAPs(HIDE_DUPLIATE_NETWORKS);

  if (!wifiManager.autoConnect()) {
    debug("Wifi failed to connect and hit timeout. Rebooting...");
    delay(3000);
    // Reboot. A.k.a. "Have you tried turning it Off and On again?"
    ESP.reset();
    delay(5000);
  }

#if MQTT_ENABLE
  strncpy(MqttServer, custom_mqtt_server.getValue(), kHostnameLength);
  strncpy(MqttPort, custom_mqtt_port.getValue(), kPortLength);
  strncpy(MqttUsername, custom_mqtt_user.getValue(), kUsernameLength);
  strncpy(MqttPassword, custom_mqtt_pass.getValue(), kPasswordLength);
  strncpy(MqttPrefix, custom_mqtt_prefix.getValue(), kHostnameLength);
#endif  // MQTT_ENABLE
  strncpy(Hostname, custom_hostname.getValue(), kHostnameLength);
  strncpy(HttpUsername, custom_http_username.getValue(), kUsernameLength);
  strncpy(HttpPassword, custom_http_password.getValue(), kPasswordLength);
  if (flagSaveWifiConfig) {
    saveWifiConfig();
  }
  debug("WiFi connected. IP address:");
  debug(WiFi.localIP().toString().c_str());
}

void init_vars(void) {
#if MQTT_ENABLE
  // If we have a prefix already, use it. Otherwise use the hostname.
  if (!strlen(MqttPrefix)) strncpy(MqttPrefix, Hostname, kHostnameLength);
  // Topic we send back acknowledgements on.
  MqttAck = String(MqttPrefix) + '/' + MQTT_ACK;
  // Sub-topic we get new commands from.
  MqttSend = String(MqttPrefix) + '/' + MQTT_SEND;
  // Topic we send received IRs to.
  MqttRecv = String(MqttPrefix) + '/' + MQTT_RECV;
  // Topic we send log messages to.
  MqttLog = String(MqttPrefix) + '/' + MQTT_LOG;
  // Topic for the Last Will & Testament.
  MqttLwt = String(MqttPrefix) + '/' + MQTT_LWT;
  // Sub-topic for the climate topics.
  MqttClimate = String(MqttPrefix) + '/' + MQTT_CLIMATE;
  // Sub-topic for the climate command topics.
  MqttClimateCmnd = MqttClimate + '/' + MQTT_CLIMATE_CMND + '/';
  // Sub-topic for the climate stat topics.
  MqttClimateStat = MqttClimate + '/' + MQTT_CLIMATE_STAT + '/';
  MqttDiscovery = "homeassistant/climate/" + String(Hostname) + "/config";
  MqttHAName = String(Hostname) + "_aircon";
  // Create a unique MQTT client id.
  MqttClientId = String(Hostname) + String(ESP.getChipId(), HEX);
#endif  // MQTT_ENABLE
}

void setup(void) {
  // Set the default climate settings.
  climate.protocol = decode_type_t::UNKNOWN;
  climate.model = -1;  // Unknown.
  climate.power = false;
  climate.mode = stdAc::opmode_t::kAuto;
  climate.celsius = true;
  climate.degrees = 25;  // 25C
  climate.fanspeed = stdAc::fanspeed_t::kAuto;
  climate.swingv = stdAc::swingv_t::kAuto;
  climate.swingh = stdAc::swingh_t::kAuto;
  climate.quiet = false;
  climate.turbo = false;
  climate.econo = false;
  climate.light = false;
  climate.filter = false;
  climate.clean = false;
  climate.beep = false;
  climate.sleep = -1;  // Off
  climate.clock = -1;  // Don't set.
  climate_prev = climate;

  // Initialise all the IR transmitters.
  for (uint8_t i = 0; i < kSendTableSize; i++) {
    IrSendTable[i] = new IRsend(gpioTable[i]);
    IrSendTable[i]->begin();
    offset = IrSendTable[i]->calibrate();
  }
#ifdef IR_RX
#if IR_RX_PULLUP
  pinMode(IR_RX, INPUT_PULLUP);
#endif  // IR_RX_PULLUP
#if DECODE_HASH
  // Ignore messages with less than minimum on or off pulses.
  irrecv.setUnknownThreshold(kMinUnknownSize);
#endif  // DECODE_HASH
  irrecv.enableIRIn();  // Start the receiver
#endif  // IR_RX

#if DEBUG
  if (!isSerialGpioUsedByIr()) {
    // Use SERIAL_TX_ONLY so that the RX pin can be freed up for GPIO/IR use.
    Serial.begin(BAUD_RATE, SERIAL_8N1, SERIAL_TX_ONLY);
    while (!Serial)  // Wait for the serial connection to be establised.
      delay(50);
    Serial.println();
    debug("IRMQTTServer " _MY_VERSION_" has booted.");
  }
#endif  // DEBUG

  setup_wifi();

  // Wait a bit for things to settle.
  delay(500);

  lastReconnectAttempt = 0;

  if (mdns.begin(Hostname, WiFi.localIP())) {
    debug("MDNS responder started");
  }

  // Setup the root web page.
  server.on("/", handleRoot);
  // Setup the examples web page.
  server.on("/examples", handleExamples);
  // Setup the page to handle web-based IR codes.
  server.on("/ir", handleIr);
  // Setup the aircon page.
  server.on("/aircon", handleAirCon);
  // Setup the aircon update page.
  server.on("/aircon/set", handleAirConSet);
  // Setup the info page.
  server.on("/info", handleInfo);
  // Setup the admin page.
  server.on("/admin", handleAdmin);
  // Setup a reset page to cause WiFiManager information to be reset.
  server.on("/reset", handleReset);
  // Reboot url
  server.on("/quitquitquit", handleReboot);
#if MQTT_ENABLE
  // MQTT Discovery url
  server.on("/send_discovery", handleSendMqttDiscovery);
  // Finish setup of the mqtt clent object.
  mqtt_client.setServer(MqttServer, atoi(MqttPort));
  mqtt_client.setCallback(mqttCallback);
  // Set various variables
  init_vars();
#endif  // MQTT_ENABLE

#if FIRMWARE_OTA
  // Setup the URL to allow Over-The-Air (OTA) firmware updates.
  if (strlen(HttpPassword)) {  // Allow if password is set.
    server.on("/update", HTTP_POST, [](){
#if MQTT_ENABLE
        mqttLog("Attempting firmware update & reboot");
        delay(1000);
#endif  // MQTT_ENABLE
        server.send(200, "text/html",
            "<html><head><title>Updating firmware.</title></head>"
            "<body>"
            "<h1>Updating firmware</h1>"
            "<hr>"
            "<h3>Warning! Don't power off the device for 60 seconds!</h3>"
            "<p>The firmware is uploading and will try to flash itself. "
            "It is important to not interrupt the process.</p>"
            "<p>The firmware upload seems to have " +
            String(Update.hasError() ? "FAILED!" : "SUCCEEDED!") +
            " Rebooting! </p>" +
            addJsReloadUrl("/", 20, true) +
            "</body></html>");
        delay(1000);
        ESP.restart();
        delay(1000);
      }, [](){
        if (!server.authenticate(HttpUsername, HttpPassword)) {
          debug("Basic HTTP authentication failure for /update.");
          return server.requestAuthentication();
        }
        HTTPUpload& upload = server.upload();
        if (upload.status == UPLOAD_FILE_START) {
          WiFiUDP::stopAll();
          debug("Update:");
          debug(upload.filename.c_str());
          uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) &
              0xFFFFF000;
          if (!Update.begin(maxSketchSpace)) {  // start with max available size
#if DEBUG
            if (!isSerialGpioUsedByIr())
              Update.printError(Serial);
#endif  // DEBUG
          }
        } else if (upload.status == UPLOAD_FILE_WRITE) {
          if (Update.write(upload.buf, upload.currentSize) !=
              upload.currentSize) {
#if DEBUG
            if (!isSerialGpioUsedByIr())
              Update.printError(Serial);
#endif  // DEBUG
          }
        } else if (upload.status == UPLOAD_FILE_END) {
          // true to set the size to the current progress
          if (Update.end(true)) {
            debug("Update Success:");
            debug(String(upload.totalSize).c_str());
            debug("Rebooting...");
          }
        }
        yield();
      });
    }
#endif  // FIRMWARE_OTA

  // Set up an error page.
  server.onNotFound(handleNotFound);

  server.begin();
  debug("HTTP server started");
}

#if MQTT_ENABLE
// MQTT subscribing to topic
void subscribing(const String topic_name) {
  // subscription to topic for receiving data with QoS.
  if (mqtt_client.subscribe(topic_name.c_str(), QOS))
    debug("Subscription OK to:");
  else
    debug("Subscription FAILED to:");
  debug(topic_name.c_str());
}

// Un-subscribe from a MQTT topic
void unsubscribing(const String topic_name) {
  // subscription to topic for receiving data with QoS.
  if (mqtt_client.unsubscribe(topic_name.c_str()))
    debug("Unsubscribed OK from:");
  else
    debug("FAILED to unsubscribe from:");
  debug(topic_name.c_str());
}

void mqttLog(const String mesg) {
  debug(mesg.c_str());
  mqtt_client.publish(MqttLog.c_str(), mesg.c_str());
  mqttSentCounter++;
}

bool reconnect(void) {
  // Loop a few times or until we're reconnected
  uint16_t tries = 1;
  while (!mqtt_client.connected() && tries <= 3) {
    int connected = false;
    // Attempt to connect
    debug(("Attempting MQTT connection to " + String(MqttServer) + ":" +
           String(MqttPort) + "... ").c_str());
    if (strcmp(MqttUsername, "") && strcmp(MqttPassword, "")) {
      debug("Using mqtt username/password to connect.");
      connected = mqtt_client.connect(MqttClientId.c_str(),
                                      MqttUsername, MqttPassword,
                                      MqttLwt.c_str(),
                                      QOS, true, kLwtOffline);

    } else {
      debug("Using password-less mqtt connection.");
      connected = mqtt_client.connect(MqttClientId.c_str(), MqttLwt.c_str(),
                                      QOS, true, kLwtOffline);
    }
    if (connected) {
    // Once connected, publish an announcement...
      mqttLog("(Re)Connected.");

      // Update Last Will & Testament to say we are back online.
      mqtt_client.publish(MqttLwt.c_str(), kLwtOnline, true);
      mqttSentCounter++;

      // Subscribing to topic(s)
      subscribing(MqttSend);
      for (uint8_t i = 0; i < kSendTableSize; i++) {
        subscribing(MqttSend + '_' + String(static_cast<int>(i)));
      }
      // Climate command topics.
      subscribing(MqttClimateCmnd + '+');
    } else {
      debug(("failed, rc=" + String(mqtt_client.state()) +
            " Try again in a bit.").c_str());
      // Wait for a bit before retrying
      delay(tries << 7);  // Linear increasing back-off (x128)
    }
    tries++;
  }
  return mqtt_client.connected();
}

// Return a string containing the comma separated list of MQTT command topics.
String listOfCommandTopics(void) {
  String result = MqttSend;
  for (uint16_t i = 0; i < kSendTableSize; i++) {
    result += ", " + MqttSend + '_' + String(i);
  }
  return result;
}

// MQTT Discovery web page
void handleSendMqttDiscovery(void) {
#if HTML_PASSWORD_ENABLE
  if (!server.authenticate(HttpUsername, HttpPassword)) {
    debug("Basic HTTP authentication failure for /send_discovery.");
    return server.requestAuthentication();
  }
#endif  // HTML_PASSWORD_ENABLE
  server.send(200, "text/html",
      "<html><head><title>Sending MQTT Discovery message</title></head>"
      "<body>"
      "<h1>Sending MQTT Discovery message.</h1>" +
      htmlMenu() +
      "<p>The Home Assistant MQTT Discovery message is being sent to topic: " +
      MqttDiscovery + ". It will show up in Home Assistant in a few seconds."
      "</p>"
      "<h3>Warning!</h3>"
      "<p>Home Assistant's config for this device is reset each time this is "
      " is sent.</p>" +
      addJsReloadUrl("/", 15, true) +
      "</body></html>");
  sendMQTTDiscovery(MqttDiscovery.c_str());
}

void doBroadcast(TimerMs *timer, const uint32_t interval,
                 const commonAcState_t state, const bool retain,
                 const bool force) {
  if (force || (!lockMqttBroadcast && timer->elapsed() > interval)) {
    debug("Sending MQTT stat update broadcast.");
    sendClimate(state, state, MqttClimateStat,
                retain, true, false);
    timer->reset();  // It's been sent, so reset the timer.
    hasBroadcastBeenSent = true;
  }
}

void receivingMQTT(String const topic_name, String const callback_str) {
  char* tok_ptr;
  uint64_t code = 0;
  uint16_t nbits = 0;
  uint16_t repeat = 0;
  uint8_t channel = 0;  // Default to the first channel. e.g. "*_0"

  debug("Receiving data by MQTT topic:");
  debug(topic_name.c_str());
  debug("with payload:");
  debug(callback_str.c_str());
  // Save the message as the last command seen (global).
  lastMqttCmdTopic = topic_name;
  lastMqttCmd = callback_str;
  lastMqttCmdTime = millis();
  mqttRecvCounter++;

  if (topic_name.startsWith(MqttClimate)) {
    if (topic_name.startsWith(MqttClimateCmnd)) {
      debug("It's a climate command topic");
      commonAcState_t updated = updateClimate(
          climate, topic_name, MqttClimateCmnd, callback_str);
      sendClimate(climate, updated, MqttClimateStat,
                  true, false, false);
      climate = updated;
    } else if (topic_name.startsWith(MqttClimateStat)) {
      debug("It's a climate state topic. Update internal state and DON'T send");
      climate = updateClimate(
          climate, topic_name, MqttClimateStat, callback_str);
    }
    return;  // We are done for now.
  }
  // Check if a specific channel was requested by looking for a "*_[0-9]" suffix
  for (uint8_t i = 0; i < kSendTableSize; i++) {
    debug(("Checking if " + topic_name + " ends with _" + String(i)).c_str());
    if (topic_name.endsWith("_" + String(i))) {
      channel = i;
      debug("It does!");
      break;
    }
  }

  debug(("Using transmit channel " + String(static_cast<int>(channel)) +
         " / GPIO " + String(static_cast<int>(gpioTable[channel]))).c_str());
  // Make a copy of the callback string as strtok destroys it.
  char* callback_c_str = strdup(callback_str.c_str());
  debug("MQTT Payload (raw):");
  debug(callback_c_str);

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
      IrSendTable[channel], ir_type, code,
      callback_str.substring(callback_str.indexOf(",") + 1).c_str(),
      nbits, repeat);
}

// Callback function, when we receive an MQTT value on the topics
// subscribed this function is called
void mqttCallback(char* topic, byte* payload, unsigned int length) {
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

void sendMQTTDiscovery(const char *topic) {
  if (mqtt_client.publish(
      topic, String(
      "{"
      "\"~\":\"" + MqttClimate + "\","
      "\"name\":\"" + MqttHAName + "\","
      "\"pow_cmd_t\":\"~" MQTT_CLIMATE "/" MQTT_CLIMATE_CMND "/" KEY_POWER "\","
      "\"mode_cmd_t\":\"~" MQTT_CLIMATE "/" MQTT_CLIMATE_CMND "/" KEY_MODE "\","
      "\"mode_stat_t\":\"~" MQTT_CLIMATE "/" MQTT_CLIMATE_STAT "/" KEY_MODE
          "\","
      "\"modes\":[\"off\",\"auto\",\"cool\",\"heat\",\"dry\",\"fan_only\"],"
      "\"temp_cmd_t\":\"~" MQTT_CLIMATE "/" MQTT_CLIMATE_CMND "/" KEY_TEMP "\","
      "\"temp_stat_t\":\"~" MQTT_CLIMATE "/" MQTT_CLIMATE_STAT "/" KEY_TEMP
          "\","
      "\"min_temp\":\"16\","
      "\"max_temp\":\"30\","
      "\"temp_step\":\"1\","
      "\"fan_mode_cmd_t\":\"~" MQTT_CLIMATE "/" MQTT_CLIMATE_CMND "/"
          KEY_FANSPEED "\","
      "\"fan_mode_stat_t\":\"~" MQTT_CLIMATE "/" MQTT_CLIMATE_STAT "/"
          KEY_FANSPEED "\","
      "\"fan_modes\":[\"auto\",\"min\",\"low\",\"medium\",\"high\",\"max\"],"
      "\"swing_mode_cmd_t\":\"~" MQTT_CLIMATE "/" MQTT_CLIMATE_CMND "/"
          KEY_SWINGV "\","
      "\"swing_mode_stat_t\":\"~" MQTT_CLIMATE "/" MQTT_CLIMATE_STAT "/"
          KEY_SWINGV "\","
      "\"swing_modes\":["
        "\"off\",\"auto\",\"highest\",\"high\",\"middle\",\"low\",\"lowest\""
      "]"
      "}").c_str())) {
    mqttLog("MQTT climate discovery successful sent.");
    hasDiscoveryBeenSent = true;
    lastDiscovery.reset();
    mqttSentCounter++;
  } else {
    mqttLog("MQTT climate discovery FAILED to send.");
  }
}
#endif  // MQTT_ENABLE

void loop(void) {
  server.handleClient();  // Handle any web activity

#if MQTT_ENABLE
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
          mqttLog("IR Server just booted");
          boot = false;
        } else {
          mqttLog("IR Server just (re)connected to MQTT. "
                  "Lost connection about " + timeSince(lastConnectedTime));
        }
        lastConnectedTime = now;
        debug("successful client mqtt connection");
        if (lockMqttBroadcast) {
          // Attempt to fetch back any Climate state stored in MQTT retained
          // messages on the MQTT broker.
          mqttLog("Started listening for previous state.");
          climate_prev = climate;  // Make a copy so we can compare afterwards.
          subscribing(MqttClimateStat + '+');
          statListenTime.reset();
        }
      }
    }
  } else {
    // MQTT loop
    lastConnectedTime = now;
    mqtt_client.loop();
    if (lockMqttBroadcast && statListenTime.elapsed() > kStatListenPeriodMs) {
      unsubscribing(MqttClimateStat + '+');
      mqttLog("Finished listening for previous state.");
      if (cmpClimate(climate, climate_prev)) {  // Something changed.
        mqttLog("The state was recovered from MQTT broker. Updating.");
        sendClimate(climate_prev, climate, MqttClimateStat,
                    true, false, false);
      }
      lockMqttBroadcast = false;  // Release the lock so we can broadcast again.
    }
    // Periodically send all of the climate state via MQTT.
    doBroadcast(&lastBroadcast, kBroadcastPeriodMs, climate, false, false);
  }
#endif  // MQTT_ENABLE
#ifdef IR_RX
  // Check if an IR code has been received via the IR RX module.
#if REPORT_UNKNOWNS
  if (irrecv.decode(&capture)) {
#else  // REPORT_UNKNOWNS
  if (irrecv.decode(&capture) && capture.decode_type != UNKNOWN) {
#endif  // REPORT_UNKNOWNS
    lastIrReceivedTime = millis();
    lastIrReceived = String(capture.decode_type) + "," +
        resultToHexidecimal(&capture);
#if REPORT_RAW_UNKNOWNS
    if (capture.decode_type == UNKNOWN) {
      lastIrReceived += ";";
      for (uint16_t i = 1; i < capture.rawlen; i++) {
        uint32_t usecs;
        for (usecs = capture.rawbuf[i] * kRawTick; usecs > UINT16_MAX;
             usecs -= UINT16_MAX) {
          lastIrReceived += uint64ToString(UINT16_MAX);
          lastIrReceived += ",0,";
        }
        lastIrReceived += uint64ToString(usecs, 10);
        if (i < capture.rawlen - 1)
          lastIrReceived += ",";
      }
    }
#endif  // REPORT_RAW_UNKNOWNS
    // If it isn't an AC code, add the bits.
    if (!hasACState(capture.decode_type))
      lastIrReceived += "," + String(capture.bits);
#if MQTT_ENABLE
    mqtt_client.publish(MqttRecv.c_str(), lastIrReceived.c_str());
    mqttSentCounter++;
#endif  // MQTT_ENABLE
    irRecvCounter++;
    debug("Incoming IR message sent to MQTT:");
    debug(lastIrReceived.c_str());
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
//   irsend:   A pointer to a IRsend object to transmit via.
//   ir_type:  enum of the protocol to be sent.
//   code:     Numeric payload of the IR message. Most protocols use this.
//   code_str: The unparsed code to be sent. Used by complex protocol encodings.
//   bits:     Nr. of bits in the protocol. 0 means use the protocol's default.
//   repeat:   Nr. of times the message is to be repeated. (Not all protcols.)
// Returns:
//   bool: Successfully sent or not.
bool sendIRCode(IRsend *irsend, int const ir_type,
                uint64_t const code, char const * code_str, uint16_t bits,
                uint16_t repeat) {
  // Create a pseudo-lock so we don't try to send two codes at the same time.
  while (lockIr)
    delay(20);
  lockIr = true;

  bool success = true;  // Assume success.

  // send the IR message.
  switch (ir_type) {
#if SEND_RC5
    case RC5:  // 1
      if (bits == 0)
        bits = kRC5Bits;
      irsend->sendRC5(code, bits, repeat);
      break;
#endif
#if SEND_RC6
    case RC6:  // 2
      if (bits == 0)
        bits = kRC6Mode0Bits;
      irsend->sendRC6(code, bits, repeat);
      break;
#endif
#if SEND_NEC
    case NEC:  // 3
      if (bits == 0)
        bits = kNECBits;
      irsend->sendNEC(code, bits, repeat);
      break;
#endif
#if SEND_SONY
    case SONY:  // 4
      if (bits == 0)
        bits = kSony12Bits;
      repeat = std::max(repeat, kSonyMinRepeat);
      irsend->sendSony(code, bits, repeat);
      break;
#endif
#if SEND_PANASONIC
    case PANASONIC:  // 5
      if (bits == 0)
        bits = kPanasonicBits;
      irsend->sendPanasonic64(code, bits, repeat);
      break;
#endif
#if SEND_JVC
    case JVC:  // 6
      if (bits == 0)
        bits = kJvcBits;
      irsend->sendJVC(code, bits, repeat);
      break;
#endif
#if SEND_SAMSUNG
    case SAMSUNG:  // 7
      if (bits == 0)
        bits = kSamsungBits;
      irsend->sendSAMSUNG(code, bits, repeat);
      break;
#endif
#if SEND_SAMSUNG36
    case SAMSUNG36:  // 56
      if (bits == 0)
        bits = kSamsung36Bits;
      irsend->sendSamsung36(code, bits, repeat);
      break;
#endif
#if SEND_WHYNTER
    case WHYNTER:  // 8
      if (bits == 0)
        bits = kWhynterBits;
      irsend->sendWhynter(code, bits, repeat);
      break;
#endif
#if SEND_AIWA_RC_T501
    case AIWA_RC_T501:  // 9
      if (bits == 0)
        bits = kAiwaRcT501Bits;
      repeat = std::max(repeat, kAiwaRcT501MinRepeats);
      irsend->sendAiwaRCT501(code, bits, repeat);
      break;
#endif
#if SEND_LG
    case LG:  // 10
      if (bits == 0)
        bits = kLgBits;
      irsend->sendLG(code, bits, repeat);
      break;
#endif
#if SEND_MITSUBISHI
    case MITSUBISHI:  // 12
      if (bits == 0)
        bits = kMitsubishiBits;
      repeat = std::max(repeat, kMitsubishiMinRepeat);
      irsend->sendMitsubishi(code, bits, repeat);
      break;
#endif
#if SEND_DISH
    case DISH:  // 13
      if (bits == 0)
        bits = kDishBits;
      repeat = std::max(repeat, kDishMinRepeat);
      irsend->sendDISH(code, bits, repeat);
      break;
#endif
#if SEND_SHARP
    case SHARP:  // 14
      if (bits == 0)
        bits = kSharpBits;
      irsend->sendSharpRaw(code, bits, repeat);
      break;
#endif
#if SEND_COOLIX
    case COOLIX:  // 15
      if (bits == 0)
        bits = kCoolixBits;
      irsend->sendCOOLIX(code, bits, repeat);
      break;
#endif
    case DAIKIN:  // 16
    case DAIKIN2:  // 53
    case DAIKIN216:  // 61
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
      success = parseStringAndSendAirCon(irsend, ir_type, code_str);
      break;
#if SEND_DENON
    case DENON:  // 17
      if (bits == 0)
        bits = DENON_BITS;
      irsend->sendDenon(code, bits, repeat);
      break;
#endif
#if SEND_SHERWOOD
    case SHERWOOD:  // 19
      if (bits == 0)
        bits = kSherwoodBits;
      repeat = std::max(repeat, kSherwoodMinRepeat);
      irsend->sendSherwood(code, bits, repeat);
      break;
#endif
#if SEND_RCMM
    case RCMM:  // 21
      if (bits == 0)
        bits = kRCMMBits;
      irsend->sendRCMM(code, bits, repeat);
      break;
#endif
#if SEND_SANYO
    case SANYO_LC7461:  // 22
      if (bits == 0)
        bits = kSanyoLC7461Bits;
      irsend->sendSanyoLC7461(code, bits, repeat);
      break;
#endif
#if SEND_RC5
    case RC5X:  // 23
      if (bits == 0)
        bits = kRC5XBits;
      irsend->sendRC5(code, bits, repeat);
      break;
#endif
#if SEND_PRONTO
    case PRONTO:  // 25
      success = parseStringAndSendPronto(irsend, code_str, repeat);
      break;
#endif
#if SEND_NIKAI
    case NIKAI:  // 29
      if (bits == 0)
        bits = kNikaiBits;
      irsend->sendNikai(code, bits, repeat);
      break;
#endif
#if SEND_RAW
    case RAW:  // 30
      success = parseStringAndSendRaw(irsend, code_str);
      break;
#endif
#if SEND_GLOBALCACHE
    case GLOBALCACHE:  // 31
      success = parseStringAndSendGC(irsend, code_str);
      break;
#endif
#if SEND_MIDEA
    case MIDEA:  // 34
      if (bits == 0)
        bits = kMideaBits;
      irsend->sendMidea(code, bits, repeat);
      break;
#endif
#if SEND_MAGIQUEST
    case MAGIQUEST:  // 35
      if (bits == 0)
        bits = kMagiquestBits;
      irsend->sendMagiQuest(code, bits, repeat);
      break;
#endif
#if SEND_LASERTAG
    case LASERTAG:  // 36
      if (bits == 0)
        bits = kLasertagBits;
      irsend->sendLasertag(code, bits, repeat);
      break;
#endif
#if SEND_CARRIER_AC
    case CARRIER_AC:  // 37
      if (bits == 0)
        bits = kCarrierAcBits;
      irsend->sendCarrierAC(code, bits, repeat);
      break;
#endif
#if SEND_MITSUBISHI2
    case MITSUBISHI2:  // 39
      if (bits == 0)
        bits = kMitsubishiBits;
      repeat = std::max(repeat, kMitsubishiMinRepeat);
      irsend->sendMitsubishi2(code, bits, repeat);
      break;
#endif
#if SEND_GICABLE
    case GICABLE:  // 43
      if (bits == 0)
        bits = kGicableBits;
      repeat = std::max(repeat, kGicableMinRepeat);
      irsend->sendGICable(code, bits, repeat);
      break;
#endif
#if SEND_LUTRON
    case LUTRON:  // 47
      if (bits == 0)
        bits = kLutronBits;
      irsend->sendLutron(code, bits, repeat);
      break;
#endif
#if SEND_PIONEER
    case PIONEER:  // 50
      if (bits == 0)
        bits = kPioneerBits;
      irsend->sendPioneer(code, bits, repeat);
      break;
#endif
#if SEND_LG
    case LG2:  // 51
      if (bits == 0)
        bits = kLgBits;
      irsend->sendLG2(code, bits, repeat);
      break;
#endif
#if SEND_VESTEL_AC
    case VESTEL_AC:  // 54
      if (bits == 0)
        bits = kVestelAcBits;
      irsend->sendVestelAc(code, bits, repeat);
      break;
#endif
#if SEND_TECO
    case TECO:  // 55
      if (bits == 0)
        bits = kTecoBits;
      irsend->sendTeco(code, bits, repeat);
      break;
#endif
#if SEND_LEGOPF
    case LEGOPF:  // 58
      if (bits == 0)
        bits = kLegoPfBits;
      irsend->sendLegoPf(code, bits, repeat);
      break;
#endif
    default:
      // If we got here, we didn't know how to send it.
      success = false;
  }
  lastSendTime = millis();
  // Release the lock.
  lockIr = false;

  // Indicate that we sent the message or not.
  if (success) {
    sendReqCounter++;
    debug("Sent the IR message:");
  } else {
    debug("Failed to send IR Message:");
  }
  debug("Type:");
  debug(String(ir_type).c_str());
  // For "long" codes we basically repeat what we got.
  if (hasACState((decode_type_t) ir_type) ||
      ir_type == PRONTO ||
      ir_type == RAW ||
      ir_type == GLOBALCACHE) {
    debug("Code: ");
    debug(code_str);
    // Confirm what we were asked to send was sent.
#if MQTT_ENABLE
    if (success) {
      if (ir_type == PRONTO && repeat > 0)
        mqtt_client.publish(MqttAck.c_str(), (String(ir_type) + ",R" +
                                              String(repeat) + "," +
                                              String(code_str)).c_str());
      else
        mqtt_client.publish(MqttAck.c_str(), (String(ir_type) + "," +
                                              String(code_str)).c_str());
      mqttSentCounter++;
    }
#endif  // MQTT_ENABLE
  } else {  // For "short" codes, we break it down a bit more before we report.
    debug(("Code: 0x" + uint64ToString(code, 16)).c_str());
    debug(("Bits: " + String(bits)).c_str());
    debug(("Repeats: " + String(repeat)).c_str());
#if MQTT_ENABLE
    if (success) {
      mqtt_client.publish(MqttAck.c_str(), (String(ir_type) + "," +
                                            uint64ToString(code, 16)
                                            + "," + String(bits) + "," +
                                            String(repeat)).c_str());
      mqttSentCounter++;
    }
#endif  // MQTT_ENABLE
  }
  return success;
}

bool sendInt(const String topic, const int32_t num, const bool retain) {
#if MQTT_ENABLE
  mqttSentCounter++;
  return mqtt_client.publish(topic.c_str(), String(num).c_str(), retain);
#else  // MQTT_ENABLE
  return true;
#endif  // MQTT_ENABLE
}

bool sendBool(const String topic, const bool on, const bool retain) {
#if MQTT_ENABLE
  mqttSentCounter++;
  return mqtt_client.publish(topic.c_str(), (on ? "on" : "off"), retain);
#else  // MQTT_ENABLE
  return true;
#endif  // MQTT_ENABLE
}

bool sendString(const String topic, const String str, const bool retain) {
#if MQTT_ENABLE
  mqttSentCounter++;
  return mqtt_client.publish(topic.c_str(), str.c_str(), retain);
#else  // MQTT_ENABLE
  return true;
#endif  // MQTT_ENABLE
}

bool sendFloat(const String topic, const float_t temp, const bool retain) {
#if MQTT_ENABLE
  mqttSentCounter++;
  return mqtt_client.publish(topic.c_str(), String(temp, 1).c_str(), retain);
#else  // MQTT_ENABLE
  return true;
#endif  // MQTT_ENABLE
}

commonAcState_t updateClimate(commonAcState_t current, const String str,
                              const String prefix, const String payload) {
  commonAcState_t result = current;
  String value = payload;
  value.toUpperCase();
  if (str.equals(prefix + KEY_PROTOCOL))
    result.protocol = strToDecodeType(value.c_str());
  else if (str.equals(prefix + KEY_MODEL))
    result.model = IRac::strToModel(value.c_str());
  else if (str.equals(prefix + KEY_POWER))
    result.power = IRac::strToBool(value.c_str());
  else if (str.equals(prefix + KEY_MODE))
    result.mode = IRac::strToOpmode(value.c_str());
  else if (str.equals(prefix + KEY_TEMP))
    result.degrees = value.toFloat();
  else if (str.equals(prefix + KEY_FANSPEED))
    result.fanspeed = IRac::strToFanspeed(value.c_str());
  else if (str.equals(prefix + KEY_SWINGV))
    result.swingv = IRac::strToSwingV(value.c_str());
  else if (str.equals(prefix + KEY_SWINGH))
    result.swingh = IRac::strToSwingH(value.c_str());
  else if (str.equals(prefix + KEY_QUIET))
    result.quiet = IRac::strToBool(value.c_str());
  else if (str.equals(prefix + KEY_TURBO))
    result.turbo = IRac::strToBool(value.c_str());
  else if (str.equals(prefix + KEY_ECONO))
    result.econo = IRac::strToBool(value.c_str());
  else if (str.equals(prefix + KEY_LIGHT))
    result.light = IRac::strToBool(value.c_str());
  else if (str.equals(prefix + KEY_BEEP))
    result.beep = IRac::strToBool(value.c_str());
  else if (str.equals(prefix + KEY_FILTER))
    result.filter = IRac::strToBool(value.c_str());
  else if (str.equals(prefix + KEY_CLEAN))
    result.clean = IRac::strToBool(value.c_str());
  else if (str.equals(prefix + KEY_SLEEP))
    result.sleep = value.toInt();
  else if (str.equals(prefix + KEY_CLOCK))
    result.clock = value.toInt();
  return result;
}

// Compare two AirCon states (climates).
// Returns: True if they differ, False if they don't.
bool cmpClimate(const commonAcState_t a, const commonAcState_t b) {
  return a.protocol != b.protocol || a.model != b.model || a.power != b.power ||
      a.mode != b.mode || a.degrees != b.degrees || a.celsius != b.celsius ||
      a.fanspeed != b.fanspeed || a.swingv != b.swingv ||
      a.swingh != b.swingh || a.quiet != b.quiet || a.turbo != b.turbo ||
      a.econo != b.econo || a.light != b.light || a.filter != b.filter ||
      a.clean != b.clean || a.beep != b.beep || a.sleep != b.sleep;
}

bool sendClimate(const commonAcState_t prev, const commonAcState_t next,
                 const String topic_prefix, const bool retain,
                 const bool forceMQTT, const bool forceIR) {
  bool diff = false;
  bool success = true;

  if (prev.protocol != next.protocol || forceMQTT) {
    diff = true;
    success &= sendString(topic_prefix + KEY_PROTOCOL,
                          typeToString(next.protocol), retain);
  }
  if (prev.model != next.model || forceMQTT) {
    diff = true;
    success &= sendInt(topic_prefix + KEY_MODEL, next.model, retain);
  }
  if (prev.power != next.power || prev.mode != next.mode || forceMQTT) {
    diff = true;
    success &= sendBool(topic_prefix + KEY_POWER, next.power, retain);
    success &= sendString(topic_prefix + KEY_MODE,
                          (next.power ? opmodeToString(next.mode) : F("off")),
                          retain);
  }
  if (prev.degrees != next.degrees || forceMQTT) {
    diff = true;
    success &= sendFloat(topic_prefix + KEY_TEMP, next.degrees, retain);
  }
  if (prev.celsius != next.celsius || forceMQTT) {
    diff = true;
    success &= sendBool(topic_prefix + KEY_CELSIUS, next.celsius, retain);
  }
  if (prev.fanspeed != next.fanspeed || forceMQTT) {
    diff = true;
    success &= sendString(topic_prefix + KEY_FANSPEED,
                          fanspeedToString(next.fanspeed), retain);
  }
  if (prev.swingv != next.swingv || forceMQTT) {
    diff = true;
    success &= sendString(topic_prefix + KEY_SWINGV,
                          swingvToString(next.swingv), retain);
  }
  if (prev.swingh != next.swingh || forceMQTT) {
    diff = true;
    success &= sendString(topic_prefix + KEY_SWINGH,
                          swinghToString(next.swingh), retain);
  }
  if (prev.quiet != next.quiet || forceMQTT) {
    diff = true;
    success &= sendBool(topic_prefix + KEY_QUIET, next.quiet, retain);
  }
  if (prev.turbo != next.turbo || forceMQTT) {
    diff = true;
    success &= sendBool(topic_prefix + KEY_TURBO, next.turbo, retain);
  }
  if (prev.econo != next.econo || forceMQTT) {
    diff = true;
    success &= sendBool(topic_prefix + KEY_ECONO, next.econo, retain);
  }
  if (prev.light != next.light || forceMQTT) {
    diff = true;
    success &= sendBool(topic_prefix + KEY_LIGHT, next.light, retain);
  }
  if (prev.filter != next.filter || forceMQTT) {
    diff = true;
    success &= sendBool(topic_prefix + KEY_FILTER, next.filter, retain);
  }
  if (prev.clean != next.clean || forceMQTT) {
    diff = true;
    success &= sendBool(topic_prefix + KEY_CLEAN, next.clean, retain);
  }
  if (prev.beep != next.beep || forceMQTT) {
    diff = true;
    success &= sendBool(topic_prefix + KEY_BEEP, next.beep, retain);
  }
  if (prev.sleep != next.sleep || forceMQTT) {
    diff = true;
    success &= sendInt(topic_prefix + KEY_SLEEP, next.sleep, retain);
  }
  if (diff && !forceMQTT)
    debug("Difference in common A/C state detected.");
  else
    debug("NO difference in common A/C state detected.");
  // Only send an IR message if we need to.
  if ((diff && !forceMQTT) || forceIR) {
    debug("Sending common A/C state via IR.");
    lastClimateSucceeded = commonAc.sendAc(
        next.protocol, next.model, next.power, next.mode,
        next.degrees, next.celsius, next.fanspeed, next.swingv, next.swingh,
        next.quiet, next.turbo, next.econo, next.light, next.filter, next.clean,
        next.beep, next.sleep, -1);
    if (lastClimateSucceeded) hasClimateBeenSent = true;
    success &= lastClimateSucceeded;
    lastClimateIr.reset();
    irClimateCounter++;
    sendReqCounter++;
  }
  return success;
}
