/*
 * Send & receive arbitrary IR codes via a web server or MQTT.
 * Copyright David Conran 2016, 2017, 2018, 2019
 *
 * Copyright:
 *   Code for this has been borrowed from lots of other OpenSource projects &
 *   resources. I'm *NOT* claiming complete Copyright ownership of all the code.
 *   Likewise, feel free to borrow from this as much as you want.
 *
 * NOTE: An IR LED circuit SHOULD be connected to the ESP if
 *       you want to send IR messages. e.g. GPIO4 (D2)
 *       A compatible IR RX modules SHOULD be connected to ESP
 *       if you want to capture & decode IR nessages. e.g. GPIO14 (D5)
 *       See 'IR_RX' in IRMQTTServer.h.
 *       GPIOs are configurable from the http://<your_esp's_ip_address>/gpio
 *       page.
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
 *     - ArduinoJson (https://arduinojson.org/) (Version >= 6.0)
 *     - PubSubClient (https://pubsubclient.knolleary.net/)
 *     - WiFiManager (https://github.com/tzapu/WiFiManager)
 *                   (ESP8266: Version >= 0.14, ESP32: 'development' branch.)
 *   o You MUST change <PubSubClient.h> to have the following (or larger) value:
 *     (with REPORT_RAW_UNKNOWNS 1024 or more is recommended)
 *     #define MQTT_MAX_PACKET_SIZE 768
 *   o Use the smallest non-zero SPIFFS size you can for your board.
 *     (See the Tools -> Flash Size menu)
 *
 * - PlatformIO IDE:
 *     If you are using PlatformIO, this should already been done for you in
 *     the accompanying platformio.ini file.
 *
 * ## First Boot (Initial setup)
 * The ESP board will boot into the WiFiManager's AP mode.
 * i.e. It will create a WiFi Access Point with a SSID like: "ESP123456" etc.
 * Connect to that SSID. Then point your browser to http://192.168.4.1/ and
 * configure the ESP to connect to your desired WiFi network and associated
 * required settings. It will remember these details on next boot if the device
 * connects successfully.
 * More information can be found here:
 *   https://github.com/tzapu/WiFiManager#how-it-works
 *
 * If you need to reset the WiFi and saved settings to go back to "First Boot",
 * visit:  http://<your_esp's_ip_address>/reset
 *
 * ## Normal Use (After initial setup)
 * Enter 'http://<your_esp's_ip_address/' in your browser & follow the
 * instructions there to send IR codes via HTTP/HTML.
 * Visit the http://<your_esp's_ip_address>/gpio page to configure the GPIOs
 * for the IR LED(s) and/or IR RX demodulator.
 *
 * You can send URLs like the following, with similar data type limitations as
 * the MQTT formating in the next section. e.g:
 *   http://<your_esp's_ip_address>/ir?type=7&code=E0E09966
 *   http://<your_esp's_ip_address>/ir?type=4&code=0xf50&bits=12
 *   http://<your_esp's_ip_address>/ir?code=C1A2E21D&repeats=8&type=19
 *   http://<your_esp's_ip_address>/ir?type=31&code=40000,1,1,96,24,24,24,48,24,24,24,24,24,48,24,24,24,24,24,48,24,24,24,24,24,24,24,24,1058
 *   http://<your_esp's_ip_address>/ir?type=18&code=190B8050000000E0190B8070000010f0
 *   http://<your_esp's_ip_address>/ir?repeats=1&type=25&code=0000,006E,0022,0002,0155,00AA,0015,0040,0015,0040,0015,0015,0015,0015,0015,0015,0015,0015,0015,0015,0015,0040,0015,0040,0015,0015,0015,0040,0015,0015,0015,0015,0015,0015,0015,0040,0015,0015,0015,0015,0015,0040,0015,0040,0015,0015,0015,0015,0015,0015,0015,0015,0015,0015,0015,0040,0015,0015,0015,0015,0015,0040,0015,0040,0015,0040,0015,0040,0015,0040,0015,0640,0155,0055,0015,0E40
 * If you have enabled more than 1 TX GPIO, you can use the "channel" argument:
 *   http://<your_esp's_ip_address>/ir?channel=0&type=7&code=E0E09966
 *   http://<your_esp's_ip_address>/ir?channel=1&type=7&code=E0E09966
 *
 * or
 *
 * Send a MQTT message to the topic 'ir_server/send'
 * (or 'ir_server/send_1' etc if you have enabled more than 1 TX GPIO)
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
 *   Sequences.
 *     You can send a sequence of IR messages via MQTT using the above methods
 *     if you separate them with a ';' character. In addition you can add a
 *     pause/gap between sequenced messages by using 'P' followed immediately by
 *     the number of milliseconds you wish to wait (up to a max of kMaxPauseMs).
 *       e.g. 7,E0E09966;4,f50,12
 *         Send a Samsung(7) TV Power on code, followed immediately by a Sony(4)
 *         TV power off message.
 *       or:  19,C1A28877;P500;19,C1A25AA5;P500;19,C1A2E21D,0,30
 *         Turn on a Sherwood(19) Amplifier, Wait 1/2 a second, Switch the
 *         Amplifier to Video input 2, wait 1/2 a second, then send the Sherwood
 *         Amp the "Volume Up" message 30 times.
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
 * `ir_server/ac/cmnd/+` level (or ir_server/ac_1/cmnd/+` if multiple TX GPIOs)
 * such as:
 * i.e. protocol, model, power, mode, temp, fanspeed, swingv, swingh, quiet,
 *      turbo, light, beep, econo, sleep, filter, clean, use_celsius
 * e.g. ir_server/ac/cmnd/power, ir_server/ac/cmnd/temp,
 *      ir_server/ac_0/cmnd/mode, ir_server/ac_2/cmnd/fanspeed, etc.

 * It will process them, and if successful and it caused a change, it will
 * acknowledge this via the relevant state topic for that command.
 * e.g. If the aircon/climate changes from power off to power on, it will
 *      send an "on" payload to "ir_server/ac/stat/power"
 *
 * There is a special command available to force the ESP to resend the current
 * A/C state in an IR message. To do so use the `resend` command MQTT topic,
 * e.g. `ir_server/ac/cmnd/resend` with a payload message of `resend`.
 * There is no corresponding "stat" message update for this particular topic,
 * but a log message is produced indicating it was received.
 *
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
 *   - platform: mqtt
 *     name: Living Room Aircon
 *     modes:
 *       - "off"
 *       - "auto"
 *       - "cool"
 *       - "heat"
 *       - "dry"
 *       - "fan_only"
 *     fan_modes:
 *       - "Auto"
 *       - "Min"
 *       - "Low"
 *       - "Medium"
 *       - "High"
 *       - "Max"
 *     swing_modes:
 *       - "Off"
 *       - "Auto"
 *       - "Highest"
 *       - "High"
 *       - "Middle"
 *       - "Low"
 *       - "Lowest"
 *     power_command_topic: "ir_server/ac/cmnd/power"
 *     mode_command_topic: "ir_server/ac/cmnd/mode"
 *     mode_state_topic: "ir_server/ac/stat/mode"
 *     temperature_command_topic: "ir_server/ac/cmnd/temp"
 *     temperature_state_topic: "ir_server/ac/stat/temp"
 *     fan_mode_command_topic: "ir_server/ac/cmnd/fanspeed"
 *     fan_mode_state_topic: "ir_server/ac/stat/fanspeed"
 *     swing_mode_command_topic: "ir_server/ac/cmnd/swingv"
 *     swing_mode_state_topic: "ir_server/ac/stat/swingv"
 *     min_temp: 16
 *     max_temp: 32
 *     temp_step: 1
 *     retain: false
 *
 * #### Home Assistant MQTT Discovery
 *   There is an option for this: 'Send MQTT Discovery' under the 'Admin' menu.
 *   It will produce a single MQTT Cliamte Discovery message for Home Assistant
 *   provided you have everything configured correctly here and in HA.
 *   This message has MQTT RETAIN set on it, so it only ever needs to be sent
 *   once or if the config details change etc.
 *
 *   If you no longer want it, manually remove it from your MQTT broker.
 *   e.g.
 *     `mosquitto_pub -t homeassistant/climate/ir_server/config -n -r -d`
 *
 *   NOTE: If you have multiple TX GPIOs configured, it *ONLY* works for the
 *   first TX GPIO climate. You will need to manually configure the others.
 *
 * ### via HTTP:
 *   Use the "http://<your_esp's_ip_address>/aircon/set" URL and pass on
 *   the arguments as needed to control your device. See the `KEY_*` #defines
 *   in the code for all the parameters.
 *   i.e. protocol, model, power, mode, temp, fanspeed, swingv, swingh, quiet,
 *        turbo, light, beep, econo, sleep, filter, clean, use_celsius, channel
 *   Example:
 *     http://<your_esp's_ip_address>/aircon/set?channel=0&protocol=PANASONIC_AC&model=LKE&power=on&mode=auto&fanspeed=min&temp=23
 *
 *   NOTE: If you don't set the channel, the first GPIO (Channel 0) is used.
 *
 * ## Debugging & Logging
 * If DEBUG is turned on, there is additional information printed on the Serial
 * Port. Serial Port output may be disabled if the GPIO is used for IR.
 *
 * If MQTT is enabled, some information/logging is sent to the MQTT topic:
 *   `ir_server/log`
 *
 * ## Updates
 * You can upload new firmware Over The Air (OTA) via the form on the device's
 * "Admin" page. No need to connect to the device again via USB. \o/
 * Your settings should be remembered between updates. \o/ \o/
 *
 * On boards with 1 Meg of flash should use an SPIFFS size of 64k if you want a
 * hope of being able to load a firmware via OTA.
 * Boards with only 512k flash have no chance of OTA with this firmware.
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
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#endif  // ESP8266
#if defined(ESP32)
#include <ESPmDNS.h>
#include <WebServer.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <Update.h>
#endif  // ESP32
#include <WiFiClient.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRtext.h>
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

using irutils::msToString;

#if REPORT_VCC
  ADC_MODE(ADC_VCC);
#endif  // REPORT_VCC

// Globals
#if defined(ESP8266)
ESP8266WebServer server(kHttpPort);
#endif  // ESP8266
#if defined(ESP32)
WebServer server(kHttpPort);
#endif  // ESP32
#if MDNS_ENABLE
MDNSResponder mdns;
#endif  // MDNS_ENABLE
WiFiClient espClient;
WiFiManager wifiManager;
bool flagSaveWifiConfig = false;
char HttpUsername[kUsernameLength + 1] = "admin";  // Default HTT username.
char HttpPassword[kPasswordLength + 1] = "";  // No HTTP password by default.
char Hostname[kHostnameLength + 1] = "ir_server";  // Default hostname.
uint16_t *codeArray;
uint32_t lastReconnectAttempt = 0;  // MQTT last attempt reconnection number
bool boot = true;
volatile bool lockIr = false;  // Primitive locking for gating the IR LED.
uint32_t sendReqCounter = 0;
bool lastSendSucceeded = false;  // Store the success status of the last send.
uint32_t lastSendTime = 0;
int8_t offset;  // The calculated period offset for this chip and library.
IRsend *IrSendTable[kNrOfIrTxGpios];
int8_t txGpioTable[kNrOfIrTxGpios] = {kDefaultIrLed};
String lastClimateSource;
#if IR_RX
IRrecv *irrecv = NULL;
decode_results capture;  // Somewhere to store inbound IR messages.
int8_t rx_gpio = kDefaultIrRx;
String lastIrReceived = "None";
uint32_t lastIrReceivedTime = 0;
uint32_t irRecvCounter = 0;
#endif  // IR_RX

// Climate stuff
IRac *climate[kNrOfIrTxGpios];
String channel_re = "(";  // Will be built later.
uint16_t chan = 0;  // The channel to use for the aircon HTML page.

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
#if MQTT_DISCOVERY_ENABLE
String MqttDiscovery;
#endif  // MQTT_DISCOVERY_ENABLE
String MqttHAName;
String MqttClientId;

// Primative lock file for gating MQTT state broadcasts.
bool lockMqttBroadcast = true;
TimerMs lastBroadcast = TimerMs();  // When we last sent a broadcast.
bool hasBroadcastBeenSent = false;
#if MQTT_DISCOVERY_ENABLE
TimerMs lastDiscovery = TimerMs();  // When we last sent a Discovery.
bool hasDiscoveryBeenSent = false;
#endif  // MQTT_DISCOVERY_ENABLE
TimerMs statListenTime = TimerMs();  // How long we've been listening for.
#endif  // MQTT_ENABLE

bool isSerialGpioUsedByIr(void) {
  const int8_t kSerialTxGpio = 1;  // The GPIO serial output is sent to.
                                   // Note: *DOES NOT* control Serial output.
#if defined(ESP32)
  const int8_t kSerialRxGpio = 3;  // The GPIO serial input is received on.
#endif  // ESP32
  // Ensure we are not trodding on anything IR related.
#if IR_RX
  switch (rx_gpio) {
#if defined(ESP32)
    case kSerialRxGpio:
#endif  // ESP32
    case kSerialTxGpio:
      return true;  // Serial port is in use by IR capture. Abort.
  }
#endif  // IR_RX
  for (uint16_t i = 0; i < kNrOfIrTxGpios; i++)
    switch (txGpioTable[i]) {
#if defined(ESP32)
      case kSerialRxGpio:
#endif  // ESP32
      case kSerialTxGpio:
        return true;  // Serial port is in use for IR sending. Abort.
    }
  return false;  // Not in use as far as we can tell.
}

// Debug messages get sent to the serial port.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void debug(const char *str) {
#if DEBUG
  if (isSerialGpioUsedByIr()) return;  // Abort.
  uint32_t now = millis();
  Serial.printf("%07u.%03u: %s\n", now / 1000, now % 1000, str);
#endif  // DEBUG
}
#pragma GCC diagnostic pop

// callback notifying us of the need to save the wifi config
void saveWifiConfigCallback(void) {
  debug("saveWifiConfigCallback called.");
  flagSaveWifiConfig = true;
}

// Forcibly mount the SPIFFS. Formatting the SPIFFS if needed.
//
// Returns:
//   A boolean indicating success or failure.
bool mountSpiffs(void) {
  debug("Mounting SPIFFS...");
  if (SPIFFS.begin()) return true;  // We mounted it okay.
  // We failed the first time.
  debug("Failed to mount SPIFFS!\nFormatting SPIFFS and trying again...");
  SPIFFS.format();
  if (!SPIFFS.begin()) {  // Did we fail?
    debug("DANGER: Failed to mount SPIFFS even after formatting!");
    delay(10000);  // Make sure the debug message doesn't just float by.
    return false;
  }
  return true;  // Success!
}

bool saveConfig(void) {
  debug("Saving the config.");
  bool success = false;
  DynamicJsonDocument json(kJsonConfigMaxSize);
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
#if IR_RX
  json[KEY_RX_GPIO] = static_cast<int>(rx_gpio);
#endif  // IR_RX
  for (uint16_t i = 0; i < kNrOfIrTxGpios; i++) {
    const String key = KEY_TX_GPIO + String(i);
    json[key] = static_cast<int>(txGpioTable[i]);
  }

  if (mountSpiffs()) {
    File configFile = SPIFFS.open(kConfigFile, "w");
    if (!configFile) {
      debug("Failed to open config file for writing.");
    } else {
      debug("Writing out the config file.");
      serializeJson(json, configFile);
      configFile.close();
      debug("Finished writing config file.");
      success = true;
    }
    SPIFFS.end();
  }
  return success;
}

bool loadConfigFile(void) {
  bool success = false;
  if (mountSpiffs()) {
    debug("mounted the file system");
    if (SPIFFS.exists(kConfigFile)) {
      debug("config file exists");

      File configFile = SPIFFS.open(kConfigFile, "r");
      if (configFile) {
        debug("Opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonDocument json(kJsonConfigMaxSize);
        if (!deserializeJson(json, buf.get(), kJsonConfigMaxSize)) {
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
          // Read in the GPIO settings.
#if IR_RX
          // Single RX gpio
          rx_gpio = json[KEY_RX_GPIO] | kDefaultIrRx;
#endif  // IR_RX
          // Potentially multiple TX gpios
          for (uint16_t i = 0; i < kNrOfIrTxGpios; i++)
            txGpioTable[i] = json[String(KEY_TX_GPIO + String(i)).c_str()] |
                           kDefaultIrLed;
          debug("Recovered Json fields.");
          success = true;
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
  }
  return success;
}

String timeElapsed(uint32_t const msec) {
  String result = msToString(msec);
  if (result.equalsIgnoreCase("Now"))
    return result;
  else
    return result + F(" ago");
}

String timeSince(uint32_t const start) {
  if (start == 0)
    return F("Never");
  uint32_t diff = 0;
  uint32_t now = millis();
  if (start < now)
    diff = now - start;
  else
    diff = UINT32_MAX - start + now;
  return msToString(diff) + F(" ago");
}

String gpioToString(const int16_t gpio) {
  if (gpio == kGpioUnused)
    return F("Unused");
  else
    return String(gpio);
}

int8_t getDefaultTxGpio(void) {
  for (int16_t i = 0; i < kNrOfIrTxGpios; i++)
    if (txGpioTable[i] != kGpioUnused) return txGpioTable[i];
  return kGpioUnused;
}

// Return a string containing the comma separated list of sending gpios.
String listOfTxGpios(void) {
  bool found = false;
  String result = "";
  for (uint16_t i = 0; i < kNrOfIrTxGpios; i++) {
    if (i) result += ", ";
    result += gpioToString(txGpioTable[i]);
    if (!found && txGpioTable[i] == getDefaultTxGpio()) {
      result += " (default)";
      found = true;
    }
  }
  return result;
}

String htmlMenu(void) {
  String html = F("<center>");
  html += htmlButton(kUrlRoot, F("Home"));
  html += htmlButton(kUrlAircon, F("Aircon"));
#if EXAMPLES_ENABLE
  html += htmlButton(kUrlExamples, F("Examples"));
#endif  // EXAMPLES_ENABLE
  html += htmlButton(kUrlInfo, F("System Info"));
  html += htmlButton(kUrlAdmin, F("Admin"));
  html += F("</center><hr>");
  return html;
}

String htmlSelectAcStateProtocol(const String name, const decode_type_t def,
                                 const bool simple) {
  String html = "<select name='" + name + "'>";
  for (uint8_t i = 1; i <= decode_type_t::kLastDecodeType; i++) {
    if (simple ^ hasACState((decode_type_t)i)) {
      switch (i) {
        case decode_type_t::RAW:
        case decode_type_t::PRONTO:
        case decode_type_t::GLOBALCACHE:
          break;
        default:
          html += htmlOptionItem(String(i), typeToString((decode_type_t)i),
                                i == def);
      }
    }
  }
  html += F("</select>");
  return html;
}

// Root web page with example usage etc.
void handleRoot(void) {
#if HTML_PASSWORD_ENABLE
  if (!server.authenticate(HttpUsername, HttpPassword)) {
    debug("Basic HTTP authentication failure for /.");
    return server.requestAuthentication();
  }
#endif
  String html = htmlHeader(F("ESP IR MQTT Server"));
  html += F("<center><small><i>" _MY_VERSION_ "</i></small></center>");
  html += htmlMenu();
  html += F(
    "<h3>Send a simple IR message</h3><p>"
    "<form method='POST' action='/ir' enctype='multipart/form-data'>"
      "Type: ");
  html += htmlSelectAcStateProtocol(KEY_TYPE, decode_type_t::NEC, true);
  html += F(
      " Code: 0x<input type='text' name='code' min='0' value='0' size='16'"
        " maxlength='16'>"
      " Bit size: "
      "<select name='bits'>"
        "<option selected='selected' value='0'>Default</option>");  // Default
  for (uint8_t i = 0; i < sizeof(kCommonBitSizes); i++) {
    String num = String(kCommonBitSizes[i]);
    html += F("<option value='");
    html += num;
    html += F("'>");
    html += num;
    html += F("</option>");
  }
  html += F(
      "</select>"
      " Repeats: <input type='number' name='repeats' min='0' max='99' value='0'"
        "size='2' maxlength='2'>"
      " <input type='submit' value='Send IR'>"
    "</form>"
    "<br><hr>"
    "<h3>Send a complex (Air Conditioner) IR message</h3><p>"
    "<form method='POST' action='/ir' enctype='multipart/form-data'>"
      "Type: ");
  html += htmlSelectAcStateProtocol(KEY_TYPE, decode_type_t::KELVINATOR, false);
  html += F(
      " State code: 0x"
      "<input type='text' name='code' size='");
  html += String(kStateSizeMax * 2);
  html += F("' maxlength='");
  html += String(kStateSizeMax * 2);
  html += F("'"
          " value='"
#if EXAMPLES_ENABLE
                "190B8050000000E0190B8070000010F0"
#endif   // EXAMPLES_ENABLE
                "'>"
      " <input type='submit' value='Send A/C State'>"
    "</form>"
    "<br><hr>"
    "<h3>Send an IRremote Raw IR message</h3><p>"
    "<form method='POST' action='/ir' enctype='multipart/form-data'>"
      "<input type='hidden' name='type' value='30'>"
      "String: (freq,array data) <input type='text' name='code' size='132'"
      " value='"
#if EXAMPLES_ENABLE
          "38000,4420,4420,520,1638,520,1638,520,1638,520,520,520,520,520,"
          "520,520,520,520,520,520,1638,520,1638,520,1638,520,520,520,"
          "520,520,520,520,520,520,520,520,520,520,1638,520,520,520,520,520,"
          "520,520,520,520,520,520,520,520,1638,520,520,520,1638,520,1638,520,"
          "1638,520,1638,520,1638,520,1638,520"
#endif   // EXAMPLES_ENABLE
          "'>"
      " <input type='submit' value='Send Raw'>"
    "</form>"
    "<br><hr>"
    "<h3>Send a <a href='https://irdb.globalcache.com/'>GlobalCache</a>"
        " IR message</h3><p>"
    "<form method='POST' action='/ir' enctype='multipart/form-data'>"
      "<input type='hidden' name='type' value='31'>"
      "String: 1:1,1,<input type='text' name='code' size='132'"
      " value='"
#if EXAMPLES_ENABLE
          "38000,1,1,170,170,20,63,20,63,20,63,20,20,20,20,20,20,20,20,20,"
          "20,20,63,20,63,20,63,20,20,20,20,20,20,20,20,20,20,20,20,20,63,20,"
          "20,20,20,20,20,20,20,20,20,20,20,20,63,20,20,20,63,20,63,20,63,20,"
          "63,20,63,20,63,20,1798"
#endif   // EXAMPLES_ENABLE
          "'>"
      " <input type='submit' value='Send GlobalCache'>"
    "</form>"
    "<br><hr>"
    "<h3>Send a <a href='http://www.remotecentral.com/cgi-bin/files/rcfiles.cgi"
      "?area=pronto&db=discrete'>Pronto code</a> IR message</h3><p>"
    "<form method='POST' action='/ir' enctype='multipart/form-data'>"
      "<input type='hidden' name='type' value='25'>"
      "String (comma separated): <input type='text' name='code' size='132'"
      " value='"
#if EXAMPLES_ENABLE
          "0000,0067,0000,0015,0060,0018,0018,0018,0030,0018,0030,0018,"
          "0030,0018,0018,0018,0030,0018,0018,0018,0018,0018,0030,0018,0018,"
          "0018,0030,0018,0030,0018,0030,0018,0018,0018,0018,0018,0030,0018,"
          "0018,0018,0018,0018,0030,0018,0018,03f6"
#endif   // EXAMPLES_ENABLE
          "'>"
      " Repeats: <input type='number' name='repeats' min='0' max='99' value='0'"
          "size='2' maxlength='2'>"
      " <input type='submit' value='Send Pronto'>"
    "</form>"
    "<br>");
  html += htmlEnd();
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

#if EXAMPLES_ENABLE
// Web page with hardcoded example usage etc.
void handleExamples(void) {
#if HTML_PASSWORD_ENABLE
  if (!server.authenticate(HttpUsername, HttpPassword)) {
    debug("Basic HTTP authentication failure for /examples.");
    return server.requestAuthentication();
  }
#endif
  String html = htmlHeader(F("IR MQTT examples"));
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
    "<br><hr>");
  html += htmlEnd();
  server.send(200, "text/html", html);
}
#endif  // EXAMPLES_ENABLE

String htmlOptionItem(const String value, const String text, bool selected) {
  String html = F("<option value='");
  html += value + '\'';
  if (selected) html += F(" selected='selected'");
  html += '>' + text + F("</option>");
  return html;
}

String htmlSelectBool(const String name, const bool def) {
  String html = "<select name='" + name + "'>";
  for (uint16_t i = 0; i < 2; i++)
    html += htmlOptionItem(IRac::boolToString(i), IRac::boolToString(i),
                           i == def);
  html += F("</select>");
  return html;
}

String htmlSelectClimateProtocol(const String name, const decode_type_t def) {
  String html = "<select name='" + name + "'>";
  for (uint8_t i = 1; i <= decode_type_t::kLastDecodeType; i++) {
    if (IRac::isProtocolSupported((decode_type_t)i)) {
      html += htmlOptionItem(String(i), typeToString((decode_type_t)i),
                             i == def);
    }
  }
  html += F("</select>");
  return html;
}

String htmlSelectModel(const String name, const int16_t def) {
  String html = "<select name='" + name + "'>";
  for (int16_t i = -1; i <= 6; i++) {
    String num = String(i);
    String text;
    if (i == -1)
      text = F("Default");
    else if (i == 0)
      text = F("Unknown");
    else
      text = num;
    html += htmlOptionItem(num, text, i == def);
  }
  html += F("</select>");
  return html;
}

String htmlSelectUint(const String name, const uint16_t max,
                      const uint16_t def) {
  String html = "<select name='" + name + "'>";
  for (uint16_t i = 0; i < max; i++) {
    String num = String(i);
    html += htmlOptionItem(num, num, i == def);
  }
  html += F("</select>");
  return html;
}

String htmlSelectGpio(const String name, const int16_t def,
                      const int8_t list[], const int16_t length) {
  String html = ": <select name='" + name + "'>";
  for (int16_t i = 0; i < length; i++) {
    String num = String(list[i]);
    html += htmlOptionItem(num, list[i] == kGpioUnused ? F("Unused") : num,
                           list[i] == def);
    html += F("</option>");
  }
  html += F("</select>");
  return html;
}

String htmlSelectMode(const String name, const stdAc::opmode_t def) {
  String html = "<select name='" + name + "'>";
  for (int8_t i = -1; i <= (int8_t)stdAc::opmode_t::kLastOpmodeEnum; i++) {
    String mode = IRac::opmodeToString((stdAc::opmode_t)i);
    html += htmlOptionItem(mode, mode, (stdAc::opmode_t)i == def);
  }
  html += F("</select>");
  return html;
}

String htmlSelectFanspeed(const String name, const stdAc::fanspeed_t def) {
  String html = "<select name='" + name + "'>";
  for (int8_t i = 0; i <= (int8_t)stdAc::fanspeed_t::kLastFanspeedEnum; i++) {
    String speed = IRac::fanspeedToString((stdAc::fanspeed_t)i);
    html += htmlOptionItem(speed, speed, (stdAc::fanspeed_t)i == def);
  }
  html += F("</select>");
  return html;
}

String htmlSelectSwingv(const String name, const stdAc::swingv_t def) {
  String html = "<select name='" + name + "'>";
  for (int8_t i = -1; i <= (int8_t)stdAc::swingv_t::kLastSwingvEnum; i++) {
    String swing = IRac::swingvToString((stdAc::swingv_t)i);
    html += htmlOptionItem(swing, swing, (stdAc::swingv_t)i == def);
  }
  html += F("</select>");
  return html;
}

String htmlSelectSwingh(const String name, const stdAc::swingh_t def) {
  String html = "<select name='" + name + "'>";
  for (int8_t i = -1; i <= (int8_t)stdAc::swingh_t::kLastSwinghEnum; i++) {
    String swing = IRac::swinghToString((stdAc::swingh_t)i);
    html += htmlOptionItem(swing, swing, (stdAc::swingh_t)i == def);
  }
  html += F("</select>");
  return html;
}

String htmlHeader(const String title, const String h1_text) {
  String html = F("<html><head><title>");
  html += title;
  html += F("</title></head><body><center><h1>");
  if (h1_text.length())
    html += h1_text;
  else
    html += title;
  html += F("</h1></center>");
  return html;
}

String htmlEnd(void) {
  return F("</body></html>");
}

String htmlButton(const String url, const String button, const String text) {
  String html = F("<button type='button' onclick='window.location=\"");
  html += url;
  html += F("\"'>");
  html += button;
  html += F("</button> ");
  html += text;
  return html;
}

// Admin web page
void handleAirCon(void) {
  String html = htmlHeader(F("Air Conditioner Control"));
  html += htmlMenu();
  if (kNrOfIrTxGpios > 1) {
    html += "<form method='POST' action='/aircon/set'"
        " enctype='multipart/form-data'>"
        "<table>"
        "<tr><td><b>Climate #</b></td><td>" +
        htmlSelectUint(KEY_CHANNEL, kNrOfIrTxGpios, chan) +
        "<input type='submit' value='Change'>"
        "</td></tr>"
        "</table>"
        "</form>"
        "<hr>";
  }
  if (climate[chan] != NULL) {
    html += "<h3>Current Settings</h3>"
        "<form method='POST' action='/aircon/set'"
        " enctype='multipart/form-data'>"
        "<input type='hidden' name='" KEY_CHANNEL "' value='" + String(chan) +
            "'>" +
        "<table style='width:33%'>"
        "<tr><td>Protocol</td><td>" +
            htmlSelectClimateProtocol(KEY_PROTOCOL,
                                      climate[chan]->next.protocol) +
            "</td></tr>"
        "<tr><td>Model</td><td>" +
            htmlSelectModel(KEY_MODEL, climate[chan]->next.model) +
            "</td></tr>"
        "<tr><td>Power</td><td>" +
            htmlSelectBool(KEY_POWER, climate[chan]->next.power) +
            "</td></tr>"
        "<tr><td>Mode</td><td>" +
            htmlSelectMode(KEY_MODE, climate[chan]->next.mode) +
            "</td></tr>"
        "<tr><td>Temp</td><td>"
            "<input type='number' name='" KEY_TEMP "' min='16' max='90' "
            "step='0.5' value='" + String(climate[chan]->next.degrees, 1) + "'>"
            "<select name='" KEY_CELSIUS "'>"
                "<option value='on'" +
                (climate[chan]->next.celsius ? " selected='selected'" : "") +
                ">C</option>"
                "<option value='off'" +
                (!climate[chan]->next.celsius ? " selected='selected'" : "") +
                ">F</option>"
            "</select></td></tr>"
        "<tr><td>Fan Speed</td><td>" +
            htmlSelectFanspeed(KEY_FANSPEED, climate[chan]->next.fanspeed) +
            "</td></tr>"
        "<tr><td>Swing (V)</td><td>" +
            htmlSelectSwingv(KEY_SWINGV, climate[chan]->next.swingv) +
            "</td></tr>"
        "<tr><td>Swing (H)</td><td>" +
            htmlSelectSwingh(KEY_SWINGH, climate[chan]->next.swingh) +
            "</td></tr>"
        "<tr><td>Quiet</td><td>" +
            htmlSelectBool(KEY_QUIET, climate[chan]->next.quiet) +
            "</td></tr>"
        "<tr><td>Turbo</td><td>" +
            htmlSelectBool(KEY_TURBO, climate[chan]->next.turbo) +
            "</td></tr>"
        "<tr><td>Econo</td><td>" +
            htmlSelectBool(KEY_ECONO, climate[chan]->next.econo) +
            "</td></tr>"
        "<tr><td>Light</td><td>" +
            htmlSelectBool(KEY_LIGHT, climate[chan]->next.light) +
            "</td></tr>"
        "<tr><td>Filter</td><td>" +
            htmlSelectBool(KEY_FILTER, climate[chan]->next.filter) +
            "</td></tr>"
        "<tr><td>Clean</td><td>" +
            htmlSelectBool(KEY_CLEAN, climate[chan]->next.clean) +
            "</td></tr>"
        "<tr><td>Beep</td><td>" +
            htmlSelectBool(KEY_BEEP, climate[chan]->next.beep) +
            "</td></tr>"
        "<tr><td>Force resend</td><td>" +
            htmlSelectBool(KEY_RESEND, false) +
            "</td></tr>"
        "</table>"
        "<input type='submit' value='Update & Send'>"
        "</form>";
  }
  html += htmlEnd();
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
  debug("New common a/c received via HTTP");
  uint16_t channel = chan;
  if (kNrOfIrTxGpios > 1) {
    // Scan for the channel number if needed.
    for (uint16_t i = 0; i < server.args(); i++) {
      if (server.argName(i).equals(KEY_CHANNEL)) {
        channel = server.arg(i).toInt();
      }
    }
  }
  // Change the HTML channel for the climate if it is within the correct range.
  if (channel < kNrOfIrTxGpios) chan = channel;

  IRac *ac_ptr = climate[chan];
  String html = htmlHeader(F("Aircon updated!"));
  if (ac_ptr != NULL) {
    bool force_resend = false;
    for (uint16_t i = 0; i < server.args(); i++) {
      if (server.argName(i).equals(KEY_RESEND))
        force_resend = IRac::strToBool(server.arg(i).c_str());
      else
        updateClimate(&(ac_ptr->next), server.argName(i), "", server.arg(i));
    }
#if MQTT_ENABLE
    sendClimate(genStatTopic(chan), true, false, force_resend, true, ac_ptr);
#else  // MQTT_ENABLE
    sendClimate("", false, false, force_resend, true, ac_ptr);
#endif  // MQTT_ENABLE
    lastClimateSource = F("HTTP");
  } else {  // ac_ptr == NULL
    debug("No climate setup for the given channel. Aborting!");
    html = htmlHeader(F("Aircon update FAILED!"));
  }
  // Redirect back to the aircon page.
  html += addJsReloadUrl(kUrlAircon, kQuickDisplayTime, false);
  html += htmlEnd();
  server.send(200, "text/html", html);
}

String htmlDisabled(void) {
  String html = F(
      "<i>Updates disabled until you set a password. "
      "You will need to <a href='");
  html += kUrlWipe;
  html += F("'>wipe & reset</a> to set one.</i><br><br>");
  return html;
}

// Admin web page
void handleAdmin(void) {
  String html = htmlHeader(F("Administration"));
  html += htmlMenu();
  html += F("<h3>Special commands</h3>");
#if MQTT_ENABLE
#if MQTT_DISCOVERY_ENABLE
  html += htmlButton(
      kUrlSendDiscovery, F("Send MQTT Discovery"),
      F("Send a Climate MQTT discovery message to Home Assistant.<br><br>"));
#endif  // MQTT_DISCOVERY_ENABLE
#if MQTT_CLEAR_ENABLE
  html += htmlButton(
      kUrlClearMqtt, F("Clear data saved to MQTT"),
      F("Clear all saved climate & discovery messages for this device & "
        "reboot.<br><br>"));
#endif  // MQTT_CLEAR_ENABLE
#endif  // MQTT_ENABLE
  html += htmlButton(
      kUrlReboot, F("Reboot"),
      F("A simple reboot of the ESP. <small>ie. No changes</small><br>"
        "<br>"));
  html += htmlButton(
      kUrlWipe, F("Wipe Settings"),
      F("<mark>Warning:</mark> Resets the device back to original settings. "
        "<small>ie. Goes back to AP/Setup mode.</small><br><br>"));
  html += htmlButton(kUrlGpio, F("GPIOs"), F("Change the IR GPIOs.<br>"));
#if FIRMWARE_OTA
  html += F("<hr><h3>Update firmware</h3><p>"
            "<b><mark>Warning:</mark></b><br> ");
  if (!strlen(HttpPassword))  // Deny if password not set
    html += htmlDisabled();
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
  html += htmlEnd();
  server.send(200, "text/html", html);
}

uint32_t maxSketchSpace(void) {
#if defined(ESP8266)
  return (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
#else  // defined(ESP8266)
  return UPDATE_SIZE_UNKNOWN;
#endif  // defined(ESP8266)
}

#if REPORT_VCC
String vccToString(void) { return String(ESP.getVcc() / 1000.0); }
#endif  // REPORT_VCC

// Info web page
void handleInfo(void) {
  String html = htmlHeader(F("IR MQTT server info"));
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
#if defined(ESP8266)
    "ESP8266 Core Version: " + ESP.getCoreVersion() + "<br>"
    "Free Sketch Space: " + String(maxSketchSpace() >> 10) + "k<br>"
#endif  // ESP8266
#if defined(ESP32)
    "ESP32 SDK Version: " + ESP.getSdkVersion() + "<br>"
#endif  // ESP32
    "Cpu Freq: " + String(ESP.getCpuFreqMHz()) + "MHz<br>"
    "IR Send GPIO(s): " + listOfTxGpios() + "<br>"
    + irutils::addBoolToString(kInvertTxOutput,
                               "Inverting GPIO output", false) + "<br>"
    "Total send requests: " + String(sendReqCounter) + "<br>"
    "Last message sent: " + String(lastSendSucceeded ? "Ok" : "FAILED") +
    " <i>(" + timeSince(lastSendTime) + ")</i><br>"
#if IR_RX
    "IR Recv GPIO: " + gpioToString(rx_gpio) +
#if IR_RX_PULLUP
    " (pullup)"
#endif  // IR_RX_PULLUP
    "<br>"
    "Total IR Received: " + String(irRecvCounter) + "<br>"
    "Last IR Received: " + lastIrReceived +
    " <i>(" + timeSince(lastIrReceivedTime) + ")</i><br>"
#endif  // IR_RX
    "Duplicate Wifi networks: " +
        String(HIDE_DUPLICATE_NETWORKS ? "Hide" : "Show") + "<br>"
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
#if REPORT_VCC
    "Vcc: ";
    html += vccToString();
    html += "V<br>"
#endif  // REPORT_VCC
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
#if IR_RX
    "IR Received topic: " + MqttRecv + "<br>"
#endif  // IR_RX
    "Log topic: " + MqttLog + "<br>"
    "LWT topic: " + MqttLwt + "<br>"
    "QoS: " + String(QOS) + "<br>"
    // lastMqttCmd* is unescaped untrusted input.
    // Avoid any possible HTML/XSS when displaying it.
    "Last MQTT command seen: (topic) '" +
        irutils::htmlEscape(lastMqttCmdTopic) +
         "' (payload) '" + irutils::htmlEscape(lastMqttCmd) + "' <i>(" +
         timeSince(lastMqttCmdTime) + ")</i><br>"
    "Total published: " + String(mqttSentCounter) + "<br>"
    "Total received: " + String(mqttRecvCounter) + "<br>"
    "</p>"
#endif  // MQTT_ENABLE
    "<h4>Climate Information</h4>"
    "<p>"
    "IR Send GPIO: " + String(txGpioTable[0]) + "<br>"
    "Last update source: " + lastClimateSource + "<br>"
    "Total sent: " + String(irClimateCounter) + "<br>"
    "Last send: " + String(hasClimateBeenSent ?
        (String(lastClimateSucceeded ? "Ok" : "FAILED") +
         " <i>(" + timeElapsed(lastClimateIr.elapsed()) + ")</i>") :
        "<i>Never</i>") + "<br>"
#if MQTT_ENABLE
    "State listen period: " + msToString(kStatListenPeriodMs) + "<br>"
    "State broadcast period: " + msToString(kBroadcastPeriodMs) + "<br>"
    "Last state broadcast: " + (hasBroadcastBeenSent ?
        timeElapsed(lastBroadcast.elapsed()) :
        String("<i>Never</i>")) + "<br>"
#if MQTT_DISCOVERY_ENABLE
    "Last discovery sent: " + (lockMqttBroadcast ?
        String("<b>Locked</b>") :
        (hasDiscoveryBeenSent ?
            timeElapsed(lastDiscovery.elapsed()) :
            String("<i>Never</i>"))) +
        "<br>"
#endif  // MQTT_DISCOVERY_ENABLE
    "Command topics: " + MqttClimate + channel_re + '/' + MQTT_CLIMATE_CMND +
        '/' + kClimateTopics +
    "State topics: " + MqttClimate + channel_re + '/' + MQTT_CLIMATE_STAT +
        '/' + kClimateTopics +
#endif  // MQTT_ENABLE
    "</p>"
    // Page footer
    "<hr><p><small><center>"
      "<i>(Note: Page will refresh every 60 seconds.)</i>"
    "<centre></small></p>";
  html += addJsReloadUrl(kUrlInfo, 60, false);
  html += htmlEnd();
  server.send(200, "text/html", html);
}

void doRestart(const char* str, const bool serial_only) {
#if MQTT_ENABLE
  if (!serial_only)
    mqttLog(str);
  else
#endif  // MQTT_ENABLE
    debug(str);
  delay(2000);  // Enough time for messages to be sent.
  ESP.restart();
  delay(5000);  // Enough time to ensure we don't return.
}

#if MQTT_ENABLE && MQTT_CLEAR_ENABLE
// Clear any MQTT message that we might have set retain on.
bool clearMqttSavedStates(const String topic_base) {
  String channelStr = "";
  bool success = true;
  // Clear the Last Will & Testament.
  success &= mqtt_client.publish(MqttLwt.c_str(), "", true);
#if MQTT_DISCOVERY_ENABLE
  // Clear the HA climate discovery message.
  success &= mqtt_client.publish(MqttDiscovery.c_str(), "", true);
#endif  // MQTT_DISCOVERY_ENABLE
  for (size_t channel = 0;
       channel <= kNrOfIrTxGpios;
       channelStr = '_' + String(channel++)) {
    for (size_t i = 0; i < sizeof(kMqttTopics) / sizeof(char*); i++) {
      // Sending a retained "" message to the topic should clear previous values
      // in theory.
      String topic = topic_base + channelStr + '/' + F(MQTT_CLIMATE_STAT) +
          '/' + String(kMqttTopics[i]);
      success &= mqtt_client.publish(topic.c_str(), "", true);
    }
    channelStr = '_' + String(channel);
  }
  String logmesg = "Removing all possible settings saved in MQTT for '" +
      topic_base + "' ";
  logmesg += success ? F("succeeded") : F("failed");
  mqttLog(logmesg.c_str());
  return success;
}

// Clear settings from MQTT web page
void handleClearMqtt(void) {
#if HTML_PASSWORD_ENABLE
  if (!server.authenticate(HttpUsername, HttpPassword)) {
    debug(("Basic HTTP authentication failure for " +
           String(kUrlClearMqtt)).c_str());
    return server.requestAuthentication();
  }
#endif
  server.send(200, "text/html",
    htmlHeader(F("Clearing saved info from MQTT"),
               F("Removing all saved settings for this device from "
                 "MQTT.")) +
    "<p>Device restarting. Try connecting in a few seconds.</p>" +
    addJsReloadUrl(kUrlRoot, 10, true) +
    htmlEnd());
  // Do the clearing.
  mqttLog("Clearing all saved settings from MQTT.");
  clearMqttSavedStates(MqttClimate);
  doRestart("Rebooting...");
}
#endif  // MQTT_ENABLE && MQTT_CLEAR_ENABLE

// Reset web page
void handleReset(void) {
#if HTML_PASSWORD_ENABLE
  if (!server.authenticate(HttpUsername, HttpPassword)) {
    debug(("Basic HTTP authentication failure for " +
           String(kUrlWipe)).c_str());
    return server.requestAuthentication();
  }
#endif
  server.send(200, "text/html",
    htmlHeader(F("Reset WiFi Config"),
               F("Resetting the WiFiManager config back to defaults.")) +
    "<p>Device restarting. Try connecting in a few seconds.</p>" +
    addJsReloadUrl(kUrlRoot, 10, true) +
    htmlEnd());
  // Do the reset.
#if MQTT_ENABLE
#if MQTT_CLEAR_ENABLE
  mqttLog("Clearing all saved climate settings from MQTT.");
  clearMqttSavedStates(MqttClimate);
#endif  // MQTT_CLEAR_ENABLE
  mqttLog("Wiping all saved config settings.");
#endif  // MQTT_ENABLE
  if (mountSpiffs()) {
    debug("Removing JSON config file");
    SPIFFS.remove(kConfigFile);
    SPIFFS.end();
  }
  delay(1000);
  debug("Reseting wifiManager's settings.");
  wifiManager.resetSettings();
  doRestart("Rebooting...");
}

// Reboot web page
void handleReboot() {
#if HTML_PASSWORD_ENABLE
  if (!server.authenticate(HttpUsername, HttpPassword)) {
    debug(("Basic HTTP authentication failure for " +
           String(kUrlReboot)).c_str());
    return server.requestAuthentication();
  }
#endif
  server.send(200, "text/html",
    htmlHeader(F("Device restarting.")) +
    "<p>Try connecting in a few seconds.</p>" +
    addJsReloadUrl(kUrlRoot, kRebootTime, true) +
    htmlEnd());
  doRestart("Reboot requested");
}

// Parse an Air Conditioner A/C Hex String/code and send it.
// Args:
//   irsend: A Ptr to the IRsend object to transmit via.
//   irType: Nr. of the protocol we need to send.
//   str: A hexadecimal string containing the state to be sent.
// Returns:
//   bool: Successfully sent or not.
bool parseStringAndSendAirCon(IRsend *irsend, const decode_type_t irType,
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
    default:  // Everything else.
      stateSize = IRsend::defaultBits(irType) / 8;
      if (!stateSize || !hasACState(irType)) {
        // Not a protocol we expected. Abort.
        debug("Unexpected AirCon protocol detected. Ignoring.");
        return false;
      }
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
  if (!irsend->send(irType, state, stateSize)) {
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
//   A Ptr to the new array. Restarts the ESP if it fails.
uint16_t * newCodeArray(const uint16_t size) {
  uint16_t *result;

  result = reinterpret_cast<uint16_t*>(malloc(size * sizeof(uint16_t)));
  // Check we malloc'ed successfully.
  if (result == NULL)  // malloc failed, so give up.
    doRestart(
        "FATAL: Can't allocate memory for an array for a new message! "
        "Forcing a reboot!", true);  // Send to serial only as we are in low mem
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

uint8_t getDefaultIrSendIdx(void) {
  for (uint16_t i = 0; i < kNrOfIrTxGpios; i++)
    if (IrSendTable[i] != NULL) return i;
  return 0;
}

IRsend* getDefaultIrSendPtr(void) {
  return IrSendTable[getDefaultIrSendIdx()];
}

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
  decode_type_t ir_type = decode_type_t::NEC;  // Default to NEC codes.
  uint16_t nbits = 0;
  uint16_t repeat = 0;
  int16_t channel = -1;

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
    } else if (server.argName(i).equals(KEY_CHANNEL)) {
      channel = server.arg(i).toInt();
    }
  }
  debug("New code received via HTTP");
  IRsend *tx_ptr = getDefaultIrSendPtr();
  if (channel >= 0 && channel < kNrOfIrTxGpios && IrSendTable[channel] != NULL)
    tx_ptr = IrSendTable[channel];
  lastSendSucceeded = sendIRCode(tx_ptr, ir_type, data, data_str.c_str(), nbits,
                                 repeat);
  String html = htmlHeader(F("IR command sent!"));
  html += addJsReloadUrl(kUrlRoot, kQuickDisplayTime, true);
  html += htmlEnd();
  server.send(200, "text/html", html);
}

// GPIO menu page
void handleGpio(void) {
#if HTML_PASSWORD_ENABLE
  if (!server.authenticate(HttpUsername, HttpPassword)) {
    debug("Basic HTTP authentication failure for /gpios.");
    return server.requestAuthentication();
  }
#endif
  String html = htmlHeader(F("GPIO config"));
  html += F(
      "<form method='POST' action='/gpio/set' enctype='multipart/form-data'>");
  html += htmlMenu();
  html += F("<h2><mark>WARNING: Choose carefully! You can cause damage to your "
            "hardware or make the device unresponsive.</mark></h2>");
  html += F("<h3>Send</h3>IR LED");
  for (uint16_t i = 0; i < kNrOfIrTxGpios; i++) {
    if (kNrOfIrTxGpios > 1) {
      html += F(" #");
      html += String(i);
    }
    html += htmlSelectGpio(KEY_TX_GPIO + String(i), txGpioTable[i], kTxGpios,
                           sizeof(kTxGpios));
  }
#if IR_RX
  html += F("<h3>Receive</h3>IR RX Module");
  html += htmlSelectGpio(KEY_RX_GPIO, rx_gpio, kRxGpios,
                         sizeof(kRxGpios));
#endif  // IR_RX
  html += F("<br><br><hr>");
  if (strlen(HttpPassword))  // Allow if password set
    html += F("<input type='submit' value='Save & Reboot'>");
  else
    html += htmlDisabled();
  html += F("</form>");
  html += htmlEnd();
  server.send(200, "text/html", html);
}

// GPIO setting page
void handleGpioSetting(void) {
  bool changed = false;
  if (!server.authenticate(HttpUsername, HttpPassword)) {
    debug("Basic HTTP authentication failure for /gpios.");
    return server.requestAuthentication();
  }
  String html = htmlHeader(F("Update GPIOs"));
  if (!strlen(HttpPassword)) {  // Don't allow if password not set
    html += htmlDisabled();
  } else {
    debug("Attempt to change GPIOs");
    for (uint16_t arg = 0; arg < server.args(); arg++) {
      int8_t num = std::max(static_cast<int8_t>(server.arg(arg).toInt()),
                            kGpioUnused);
#if IR_RX
      if (server.argName(arg).equals(KEY_RX_GPIO)) {
        if (rx_gpio != num) {
          rx_gpio = num;
          changed = true;
        }
      } else {
#endif  // IR_RX
        for (uint16_t i = 0; i < kNrOfIrTxGpios; i++) {
          if (server.argName(arg).equals(KEY_TX_GPIO + String(i))) {
            if (txGpioTable[i] != num) {
              txGpioTable[i] = num;
              changed = true;
            }
          }
        }
#if IR_RX
      }
#endif  // IR_RX
    }
    if (!changed) {
      html += F("<h2>No changes detected!</h2>");
    } else if (saveConfig()) {
      html += F("<h2>Saved changes & rebooting.</h2>");
    } else {
      html += F("<h2><mark>ERROR: Changes didn't save correctly! "
                "Rebooting.</h2>");
    }
  }
  html += addJsReloadUrl(changed ? kUrlRoot : kUrlGpio,
                         changed ? kRebootTime : kQuickDisplayTime,
                         true);
  html += htmlEnd();
  server.send(200, "text/html", html);
  if (changed) doRestart("GPIOs were changed. Rebooting!");
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
  loadConfigFile();
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
  wifiManager.setRemoveDuplicateAPs(HIDE_DUPLICATE_NETWORKS);

  if (!wifiManager.autoConnect())
    // Reboot. A.k.a. "Have you tried turning it Off and On again?"
    doRestart("Wifi failed to connect and hit timeout. Rebooting...", true);

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
    saveConfig();
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
#if MQTT_DISCOVERY_ENABLE
  MqttDiscovery = "homeassistant/climate/" + String(Hostname) + "/config";
#endif  // MQTT_DISCOVERY_ENABLE
  MqttHAName = String(Hostname) + "_aircon";
  // Create a unique MQTT client id.
  MqttClientId = String(Hostname) + String(kChipId, HEX);
#endif  // MQTT_ENABLE
}

void setup(void) {
#if DEBUG
  if (!isSerialGpioUsedByIr()) {
#if defined(ESP8266)
    // Use SERIAL_TX_ONLY so that the RX pin can be freed up for GPIO/IR use.
    Serial.begin(BAUD_RATE, SERIAL_8N1, SERIAL_TX_ONLY);
#else  // ESP8266
    Serial.begin(BAUD_RATE, SERIAL_8N1);
#endif  // ESP8266
    while (!Serial)  // Wait for the serial connection to be establised.
      delay(50);
    Serial.println();
    debug("IRMQTTServer " _MY_VERSION_ " has booted.");
  }
#endif  // DEBUG

  setup_wifi();

#if DEBUG
  // After the config has been loaded, check again if we are using a Serial GPIO
  if (isSerialGpioUsedByIr()) Serial.end();
#endif  // DEBUG

  channel_re.reserve(kNrOfIrTxGpios * 3);
  // Initialise all the IR transmitters.
  for (uint8_t i = 0; i < kNrOfIrTxGpios; i++) {
    if (txGpioTable[i] == kGpioUnused) {
      IrSendTable[i] = NULL;
      climate[i] = NULL;
    } else {
      IrSendTable[i] = new IRsend(txGpioTable[i], kInvertTxOutput);
      if (IrSendTable[i] != NULL) {
        IrSendTable[i]->begin();
        offset = IrSendTable[i]->calibrate();
      }
      climate[i] = new IRac(txGpioTable[i], kInvertTxOutput);
      if (climate[i] != NULL && i > 0) channel_re += '_' + String(i) + '|';
    }
  }
  lastClimateSource = F("None");
  if (channel_re.length() == 1) {
    channel_re = "";
  } else {
    channel_re.remove(channel_re.length() - 1, 1);  // Remove the last char.
    channel_re += F(")?");
  }
#if IR_RX
  if (rx_gpio != kGpioUnused)
    irrecv = new IRrecv(rx_gpio, kCaptureBufferSize, kCaptureTimeout, true);
  if (irrecv != NULL) {
#if DECODE_HASH
    // Ignore messages with less than minimum on or off pulses.
    irrecv->setUnknownThreshold(kMinUnknownSize);
#endif  // DECODE_HASH
    irrecv->enableIRIn(IR_RX_PULLUP);  // Start the receiver
  }
#endif  // IR_RX
  // Wait a bit for things to settle.
  delay(500);

  lastReconnectAttempt = 0;

#if MDNS_ENABLE
#if defined(ESP8266)
  if (mdns.begin(Hostname, WiFi.localIP())) {
#else  // ESP8266
  if (mdns.begin(Hostname)) {
#endif  // ESP8266
    debug("MDNS responder started");
  }
#endif  // MDNS_ENABLE

  // Setup the root web page.
  server.on(kUrlRoot, handleRoot);
#if EXAMPLES_ENABLE
  // Setup the examples web page.
  server.on(kUrlExamples, handleExamples);
#endif  // EXAMPLES_ENABLE
  // Setup the page to handle web-based IR codes.
  server.on("/ir", handleIr);
  // Setup the aircon page.
  server.on(kUrlAircon, handleAirCon);
  // Setup the aircon update page.
  server.on("/aircon/set", handleAirConSet);
  // Setup the info page.
  server.on(kUrlInfo, handleInfo);
  // Setup the admin page.
  server.on(kUrlAdmin, handleAdmin);
  // Setup a reset page to cause WiFiManager information to be reset.
  server.on(kUrlWipe, handleReset);
  // Reboot url
  server.on(kUrlReboot, handleReboot);
  // Show & pick which gpios are used for what etc.
  server.on(kUrlGpio, handleGpio);
  // Parse and update the new gpios.
  server.on(kUrlGpioSet, handleGpioSetting);
#if MQTT_ENABLE
#if MQTT_CLEAR_ENABLE
  // Clear settings saved to MQTT as retained messages.
  server.on(kUrlClearMqtt, handleClearMqtt);
#endif  // MQTT_CLEAR_ENABLE
#if MQTT_DISCOVERY_ENABLE
  // MQTT Discovery url
  server.on(kUrlSendDiscovery, handleSendMqttDiscovery);
#endif  // MQTT_DISCOVERY_ENABLE
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
            htmlHeader(F("Updating firmware")) +
            "<hr>"
            "<h3>Warning! Don't power off the device for 60 seconds!</h3>"
            "<p>The firmware is uploading and will try to flash itself. "
            "It is important to not interrupt the process.</p>"
            "<p>The firmware upload seems to have " +
            String(Update.hasError() ? "FAILED!" : "SUCCEEDED!") +
            " Rebooting! </p>" +
            addJsReloadUrl(kUrlRoot, 20, true) +
            htmlEnd());
        doRestart("Post firmware reboot.");
      }, [](){
        if (!server.authenticate(HttpUsername, HttpPassword)) {
          debug("Basic HTTP authentication failure for /update.");
          return server.requestAuthentication();
        }
        HTTPUpload& upload = server.upload();
        if (upload.status == UPLOAD_FILE_START) {
          debug("Update:");
          debug(upload.filename.c_str());
#if defined(ESP8266)
          WiFiUDP::stopAll();
#endif  // defined(ESP8266)
          if (!Update.begin(maxSketchSpace())) {  // start with max available
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
String genStatTopic(const uint16_t channel) {
  if (channel)  // Never use the '*_0' state channel.
    return MqttClimate + "_" + String(channel) + '/' + MQTT_CLIMATE_STAT + '/';
  else
    return MqttClimate + '/' + MQTT_CLIMATE_STAT + '/';
}

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

void mqttLog(const char* str) {
  debug(str);
  mqtt_client.publish(MqttLog.c_str(), str);
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
      subscribing(MqttSend);  // General base topic.
      subscribing(MqttClimateCmnd + '+');  // Base climate command topics
      // Per channel topics
      for (uint16_t i = 0; i < kNrOfIrTxGpios; i++) {
        // General
        if (IrSendTable[i] != NULL)
          subscribing(MqttSend + '_' + String(i));
        // Climate
        if (climate[i] != NULL)
          subscribing(MqttClimate + '_' + String(i) + '/' + MQTT_CLIMATE_CMND +
                      '/' + '+');
      }
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
  for (uint16_t i = 0; i < kNrOfIrTxGpios; i++) {
    result += ", " + MqttSend + '_' + String(i);
  }
  return result;
}

#if MQTT_DISCOVERY_ENABLE
// MQTT Discovery web page
void handleSendMqttDiscovery(void) {
#if HTML_PASSWORD_ENABLE
  if (!server.authenticate(HttpUsername, HttpPassword)) {
    debug("Basic HTTP authentication failure for /send_discovery.");
    return server.requestAuthentication();
  }
#endif  // HTML_PASSWORD_ENABLE
  server.send(200, "text/html",
      htmlHeader(F("Sending MQTT Discovery message")) +
      htmlMenu() +
      "<p>The Home Assistant MQTT Discovery message is being sent to topic: " +
      MqttDiscovery + ". It will show up in Home Assistant in a few seconds."
      "</p>"
      "<h3>Warning!</h3>"
      "<p>Home Assistant's config for this device is reset each time this is "
      " is sent.</p>" +
      addJsReloadUrl(kUrlRoot, kRebootTime, true) +
      htmlEnd());
  sendMQTTDiscovery(MqttDiscovery.c_str());
}
#endif  // MQTT_DISCOVERY_ENABLE

void doBroadcast(TimerMs *timer, const uint32_t interval,
                 IRac *climate[], const bool retain,
                 const bool force) {
  if (force || (!lockMqttBroadcast && timer->elapsed() > interval)) {
    debug("Sending MQTT stat update broadcast.");
    for (uint16_t i = 0; i < kNrOfIrTxGpios; i++) {
      String stat_topic = genStatTopic(i);
      sendClimate(stat_topic, retain, true, false, true, climate[i]);
#if REPORT_VCC
      sendString(stat_topic + KEY_VCC, vccToString(), false);
#endif  // REPORT_VCC
#if MQTT_CLIMATE_JSON
      sendJsonState(climate[i]->next, stat_topic + KEY_JSON);
#endif  // MQTT_CLIMATE_JSON
    }
    timer->reset();  // It's been sent, so reset the timer.
    hasBroadcastBeenSent = true;
  }
}

void receivingMQTT(String const topic_name, String const callback_str) {
  uint64_t code = 0;
  uint16_t nbits = 0;
  uint16_t repeat = 0;
  uint8_t channel = getDefaultIrSendIdx();  // Default to first usable channel.

  debug("Receiving data by MQTT topic:");
  debug(topic_name.c_str());
  debug("with payload:");
  debug(callback_str.c_str());
  // Save the message as the last command seen (global).
  lastMqttCmdTopic = topic_name;
  lastMqttCmd = callback_str;
  lastMqttCmdTime = millis();
  mqttRecvCounter++;

  // Check if a specific channel was requested by looking for a "*_[0-9]" suffix
  // Or is for a specific ac/climate channel. e.g. "*/ac_[1-9]"
  debug(("Checking for channel number in " + topic_name).c_str());
  for (uint16_t i = 0; i < kNrOfIrTxGpios; i++) {
    if (topic_name.endsWith("_" + String(i)) ||
        (i > 0 && topic_name.startsWith(MqttClimate + "_" + String(i)))) {
      channel = i;
      break;
    }
  }
  debug(("Channel = " + String(channel)).c_str());
  // Is it a climate topic?
  if (topic_name.startsWith(MqttClimate)) {
    String alt_cmnd_topic = MqttClimate + "_" + String(channel) + '/' +
        MQTT_CLIMATE_CMND + '/';
    // Also accept climate commands on the '*_0' channel.
    String cmnd_topic = topic_name.startsWith(alt_cmnd_topic) ? alt_cmnd_topic
                                                              : MqttClimateCmnd;
    String stat_topic = genStatTopic(channel);
    if (topic_name.startsWith(cmnd_topic)) {
      debug("It's a climate command topic");
      updateClimate(&(climate[channel]->next), topic_name, cmnd_topic,
                    callback_str);
      // Handle the special command for forcing a resend of the state via IR.
      bool force_resend = false;
      if (topic_name.equals(cmnd_topic + KEY_RESEND) &&
          callback_str.equalsIgnoreCase(KEY_RESEND)) {
        force_resend = true;
        mqttLog("Climate resend requested.");
      }
      if (sendClimate(stat_topic, true, false, force_resend, true,
                      climate[channel]) && !force_resend)
        lastClimateSource = F("MQTT");
    } else if (topic_name.startsWith(stat_topic)) {
      debug("It's a climate state topic. Update internal state and DON'T send");
      updateClimate(&(climate[channel]->next), topic_name, stat_topic,
                    callback_str);
    }
    return;  // We are done for now.
  }

  debug(("Using transmit channel " + String(static_cast<int>(channel)) +
         " / GPIO " + String(static_cast<int>(txGpioTable[channel]))).c_str());
  // Make a copy of the callback string as strtok destroys it.
  char* callback_c_str = strdup(callback_str.c_str());
  debug("MQTT Payload (raw):");
  debug(callback_c_str);

  // Chop up the str into command chunks.
  // i.e. commands in a sequence are delimitered by ';'.
  char* sequence_tok_ptr;
  for (char* sequence_item = strtok_r(callback_c_str, kSequenceDelimiter,
                                      &sequence_tok_ptr);
       sequence_item != NULL;
       sequence_item = strtok_r(NULL, kSequenceDelimiter, &sequence_tok_ptr)) {
    // Now, process each command individually.
    char* tok_ptr;
    // Make a copy of the sequence_item str as strtok_r stomps on it.
    char* ircommand = strdup(sequence_item);
    // Check if it is a pause command.
    switch (ircommand[0]) {
      case kPauseChar:
        {  // It's a pause. Everything after the 'P' should be a number.
          int32_t msecs = std::min((int32_t) strtoul(ircommand + 1, NULL, 10),
                                   kMaxPauseMs);
          delay(msecs);
          mqtt_client.publish(MqttAck.c_str(),
                              String(kPauseChar + String(msecs)).c_str());
          mqttSentCounter++;
          break;
        }
      default:  // It's an IR command.
        {
          // Get the numeric protocol type.
          decode_type_t ir_type = (decode_type_t)atoi(strtok_r(
              ircommand, kCommandDelimiter, &tok_ptr));
          char* next = strtok_r(NULL, kCommandDelimiter, &tok_ptr);
          // If there is unparsed string left, try to convert it assuming it's
          // hex.
          if (next != NULL) {
            code = getUInt64fromHex(next);
            next = strtok_r(NULL, kCommandDelimiter, &tok_ptr);
          } else {
            // We require at least two value in the string. Give up.
            break;
          }
          // If there is still string left, assume it is the bit size.
          if (next != NULL) {
            nbits = atoi(next);
            next = strtok_r(NULL, kCommandDelimiter, &tok_ptr);
          }
          // If there is still string left, assume it is the repeat count.
          if (next != NULL)
            repeat = atoi(next);
          // send received MQTT value by IR signal
          lastSendSucceeded = sendIRCode(
              IrSendTable[channel], ir_type, code,
              strchr(sequence_item, kCommandDelimiter[0]) + 1, nbits, repeat);
        }
    }
    free(ircommand);
  }
  free(callback_c_str);
}

// Callback function, when we receive an MQTT value on the topics
// subscribed this function is called
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  // In order to republish this payload, a copy must be made
  // as the orignal payload buffer will be overwritten whilst
  // constructing the PUBLISH packet.
  // Allocate the correct amount of memory for the payload copy
  byte* payload_copy = reinterpret_cast<byte*>(malloc(length + 1));
  if (payload_copy == NULL) {
    debug("Can't allocate memory for `payload_copy`. Skipping callback!");
    return;
  }
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

#if MQTT_DISCOVERY_ENABLE
void sendMQTTDiscovery(const char *topic) {
  if (mqtt_client.publish(
      topic, String(
      "{"
      "\"~\":\"" + MqttClimate + "\","
      "\"name\":\"" + MqttHAName + "\","
      "\"pow_cmd_t\":\"~/" MQTT_CLIMATE_CMND "/" KEY_POWER "\","
      "\"mode_cmd_t\":\"~/" MQTT_CLIMATE_CMND "/" KEY_MODE "\","
      "\"mode_stat_t\":\"~/" MQTT_CLIMATE_STAT "/" KEY_MODE "\","
      // I don't know why, but the modes need to be lower case to work with
      // Home Assistant & Google Home.
      "\"modes\":[\"off\",\"auto\",\"cool\",\"heat\",\"dry\",\"fan_only\"],"
      "\"temp_cmd_t\":\"~/" MQTT_CLIMATE_CMND "/" KEY_TEMP "\","
      "\"temp_stat_t\":\"~/" MQTT_CLIMATE_STAT "/" KEY_TEMP "\","
      "\"min_temp\":\"16\","
      "\"max_temp\":\"30\","
      "\"temp_step\":\"1\","
      "\"fan_mode_cmd_t\":\"~/" MQTT_CLIMATE_CMND "/" KEY_FANSPEED "\","
      "\"fan_mode_stat_t\":\"~/" MQTT_CLIMATE_STAT "/" KEY_FANSPEED "\","
      "\"fan_modes\":[\"" D_STR_AUTO "\",\"" D_STR_MIN "\",\"" D_STR_LOW "\",\""
                      D_STR_MEDIUM "\",\"" D_STR_HIGH "\",\"" D_STR_MAX "\"],"
      "\"swing_mode_cmd_t\":\"~/" MQTT_CLIMATE_CMND "/" KEY_SWINGV "\","
      "\"swing_mode_stat_t\":\"~/" MQTT_CLIMATE_STAT "/" KEY_SWINGV "\","
      "\"swing_modes\":[\"" D_STR_OFF "\",\"" D_STR_AUTO "\",\"" D_STR_HIGHEST
                        "\",\"" D_STR_HIGH "\",\"" D_STR_MIDDLE "\",\""
                        D_STR_LOW "\",\"" D_STR_LOWEST "\"]"
      "}").c_str(), true)) {
    mqttLog("MQTT climate discovery successful sent.");
    hasDiscoveryBeenSent = true;
    lastDiscovery.reset();
    mqttSentCounter++;
  } else {
    mqttLog("MQTT climate discovery FAILED to send.");
  }
}
#endif  // MQTT_DISCOVERY_ENABLE
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
          mqttLog("IRMQTTServer " _MY_VERSION_ " just booted");
          boot = false;
        } else {
          mqttLog(String(
              "IRMQTTServer just (re)connected to MQTT. Lost connection about "
              + timeSince(lastConnectedTime)).c_str());
        }
        lastConnectedTime = now;
        debug("successful client mqtt connection");
        if (lockMqttBroadcast) {
          // Attempt to fetch back any Climate state stored in MQTT retained
          // messages on the MQTT broker.
          mqttLog("Started listening for previous state.");
          for (uint16_t i = 0; i < kNrOfIrTxGpios; i++)
            subscribing(genStatTopic(i) + '+');
          statListenTime.reset();
        }
      }
    }
  } else {
    // MQTT loop
    lastConnectedTime = now;
    mqtt_client.loop();
    if (lockMqttBroadcast && statListenTime.elapsed() > kStatListenPeriodMs) {
      for (uint16_t i = 0; i < kNrOfIrTxGpios; i++) {
        String stat_topic = genStatTopic(i);
        unsubscribing(stat_topic + '+');
        // Did something change?
        if (climate[i] != NULL && climate[i]->hasStateChanged()) {
          sendClimate(stat_topic, true, false, false,
                      MQTT_CLIMATE_IR_SEND_ON_RESTART, climate[i]);
          lastClimateSource = F("MQTT (via retain)");
          mqttLog("The state was recovered from MQTT broker.");
        }
      }
      mqttLog("Finished listening for previous state.");
      lockMqttBroadcast = false;  // Release the lock so we can broadcast again.
    }
    // Periodically send all of the climate state via MQTT.
    doBroadcast(&lastBroadcast, kBroadcastPeriodMs, climate, false, false);
  }
#endif  // MQTT_ENABLE
#if IR_RX
  // Check if an IR code has been received via the IR RX module.
#if REPORT_UNKNOWNS
  if (irrecv != NULL && irrecv->decode(&capture)) {
#else  // REPORT_UNKNOWNS
  if (irrecv != NULL && irrecv->decode(&capture) &&
      capture.decode_type != UNKNOWN) {
#endif  // REPORT_UNKNOWNS
    lastIrReceivedTime = millis();
    lastIrReceived = String(capture.decode_type) + kCommandDelimiter[0] +
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
      lastIrReceived += kCommandDelimiter[0] + String(capture.bits);
#if MQTT_ENABLE
    mqtt_client.publish(MqttRecv.c_str(), lastIrReceived.c_str());
    mqttSentCounter++;
    debug("Incoming IR message sent to MQTT:");
    debug(lastIrReceived.c_str());
#endif  // MQTT_ENABLE
    irRecvCounter++;
#if USE_DECODED_AC_SETTINGS
    if (decodeCommonAc(&capture)) lastClimateSource = F("IR");
#endif  // USE_DECODED_AC_SETTINGS
  }
#endif  // IR_RX
  delay(100);
}

// Arduino framework doesn't support strtoull(), so make our own one.
uint64_t getUInt64fromHex(char const *str) {
  uint64_t result = 0;
  uint16_t offset = 0;
  // Skip any leading '0x' or '0X' prefix.
  if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) offset = 2;
  for (; isxdigit((unsigned char)str[offset]); offset++) {
    char c = str[offset];
    result *= 16;
    if (isdigit(c))
      result += c - '0';  // '0' .. '9'
    else if (isupper(c))
      result += c - 'A' + 10;  // 'A' .. 'F'
    else
      result += c - 'a' + 10;  // 'a' .. 'f'
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
bool sendIRCode(IRsend *irsend, decode_type_t const ir_type,
                uint64_t const code, char const * code_str, uint16_t bits,
                uint16_t repeat) {
  if (irsend == NULL) return false;
  bool success = true;  // Assume success.
  // Ensure we have enough repeats.
  repeat = std::max(IRsend::minRepeats(ir_type), repeat);
  if (bits == 0) bits = IRsend::defaultBits(ir_type);
  // Create a pseudo-lock so we don't try to send two codes at the same time.
  while (lockIr)
    delay(20);
  lockIr = true;


  // Turn off IR capture if we need to.
#if IR_RX && DISABLE_CAPTURE_WHILE_TRANSMITTING
  if (irrecv != NULL) irrecv->disableIRIn();  // Stop the IR receiver
#endif  // IR_RX && DISABLE_CAPTURE_WHILE_TRANSMITTING
  // send the IR message.
  switch (ir_type) {
#if SEND_PRONTO
    case decode_type_t::PRONTO:  // 25
      success = parseStringAndSendPronto(irsend, code_str, repeat);
      break;
#endif  // SEND_PRONTO
    case decode_type_t::RAW:  // 30
#if SEND_RAW
      success = parseStringAndSendRaw(irsend, code_str);
      break;
#endif
#if SEND_GLOBALCACHE
    case decode_type_t::GLOBALCACHE:  // 31
      success = parseStringAndSendGC(irsend, code_str);
      break;
#endif
    default:  // Everything else.
      if (hasACState(ir_type))  // protocols with > 64 bits
        success = parseStringAndSendAirCon(irsend, ir_type, code_str);
      else  // protocols with <= 64 bits
        success = irsend->send(ir_type, code, bits, repeat);
  }
#if IR_RX && DISABLE_CAPTURE_WHILE_TRANSMITTING
  // Turn IR capture back on if we need to.
  if (irrecv != NULL) irrecv->enableIRIn();  // Restart the receiver
#endif  // IR_RX && DISABLE_CAPTURE_WHILE_TRANSMITTING
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
  if (hasACState(ir_type) || ir_type == PRONTO || ir_type == RAW ||
      ir_type == GLOBALCACHE) {
    debug("Code: ");
    debug(code_str);
    // Confirm what we were asked to send was sent.
#if MQTT_ENABLE
    if (success) {
      if (ir_type == PRONTO && repeat > 0)
        mqtt_client.publish(MqttAck.c_str(), (String(ir_type) +
                                              kCommandDelimiter[0] + 'R' +
                                              String(repeat) +
                                              kCommandDelimiter[0] +
                                              String(code_str)).c_str());
      else
        mqtt_client.publish(MqttAck.c_str(), (String(ir_type) +
                                              kCommandDelimiter[0] +
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
      mqtt_client.publish(MqttAck.c_str(), (String(ir_type) +
                                            kCommandDelimiter[0] +
                                            uint64ToString(code, 16) +
                                            kCommandDelimiter[0] +
                                            String(bits) +
                                            kCommandDelimiter[0] +
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

#if MQTT_CLIMATE_JSON
void sendJsonState(const stdAc::state_t state, const String topic,
                   const bool retain, const bool ha_mode) {
  DynamicJsonDocument json(kJsonAcStateMaxSize);
  json[KEY_PROTOCOL] = typeToString(state.protocol);
  json[KEY_MODEL] = state.model;
  json[KEY_POWER] = IRac::boolToString(state.power);
  json[KEY_MODE] = IRac::opmodeToString(state.mode);
  // Home Assistant wants mode to be off if power is also off & vice-versa.
  if (ha_mode && (state.mode == stdAc::opmode_t::kOff || !state.power)) {
    json[KEY_MODE] = IRac::opmodeToString(stdAc::opmode_t::kOff);
    json[KEY_POWER] = IRac::boolToString(false);
  }
  json[KEY_CELSIUS] = IRac::boolToString(state.celsius);
  json[KEY_TEMP] = state.degrees;
  json[KEY_FANSPEED] = IRac::fanspeedToString(state.fanspeed);
  json[KEY_SWINGV] = IRac::swingvToString(state.swingv);
  json[KEY_SWINGH] = IRac::swinghToString(state.swingh);
  json[KEY_QUIET] = IRac::boolToString(state.quiet);
  json[KEY_TURBO] = IRac::boolToString(state.turbo);
  json[KEY_ECONO] = IRac::boolToString(state.econo);
  json[KEY_LIGHT] = IRac::boolToString(state.light);
  json[KEY_FILTER] = IRac::boolToString(state.filter);
  json[KEY_CLEAN] = IRac::boolToString(state.clean);
  json[KEY_BEEP] = IRac::boolToString(state.beep);
  json[KEY_SLEEP] = state.sleep;

  String payload = "";
  payload.reserve(200);
  serializeJson(json, payload);
  sendString(topic, payload, retain);
}

bool validJsonStr(DynamicJsonDocument doc, const char* key) {
  return doc.containsKey(key) && doc[key].is<char*>();
}

bool validJsonInt(DynamicJsonDocument doc, const char* key) {
  return doc.containsKey(key) && doc[key].is<signed int>();
}

stdAc::state_t jsonToState(const stdAc::state_t current, const char *str) {
  DynamicJsonDocument json(kJsonAcStateMaxSize);
  if (deserializeJson(json, str, kJsonAcStateMaxSize)) {
    debug("json MQTT message did not parse. Skipping!");
    return current;
  }
  stdAc::state_t result = current;
  if (validJsonStr(json, KEY_PROTOCOL))
    result.protocol = strToDecodeType(json[KEY_PROTOCOL]);
  else if (validJsonInt(json, KEY_PROTOCOL))
    result.protocol = (decode_type_t)json[KEY_PROTOCOL].as<signed int>();
  if (validJsonStr(json, KEY_MODEL))
    result.model = IRac::strToModel(json[KEY_MODEL].as<char*>());
  else if (validJsonInt(json, KEY_MODEL))
    result.model = json[KEY_MODEL];
  if (validJsonStr(json, KEY_MODE))
    result.mode = IRac::strToOpmode(json[KEY_MODE]);
  if (validJsonStr(json, KEY_FANSPEED))
    result.fanspeed = IRac::strToFanspeed(json[KEY_FANSPEED]);
  if (validJsonStr(json, KEY_SWINGV))
    result.swingv = IRac::strToSwingV(json[KEY_SWINGV]);
  if (validJsonStr(json, KEY_SWINGH))
    result.swingh = IRac::strToSwingH(json[KEY_SWINGH]);
  if (json.containsKey(KEY_TEMP))
    result.degrees = json[KEY_TEMP];
  if (validJsonInt(json, KEY_SLEEP))
    result.sleep = json[KEY_SLEEP];
  if (validJsonStr(json, KEY_POWER))
    result.power = IRac::strToBool(json[KEY_POWER]);
  if (validJsonStr(json, KEY_QUIET))
    result.quiet = IRac::strToBool(json[KEY_QUIET]);
  if (validJsonStr(json, KEY_TURBO))
    result.turbo = IRac::strToBool(json[KEY_TURBO]);
  if (validJsonStr(json, KEY_ECONO))
    result.econo = IRac::strToBool(json[KEY_ECONO]);
  if (validJsonStr(json, KEY_LIGHT))
    result.light = IRac::strToBool(json[KEY_LIGHT]);
  if (validJsonStr(json, KEY_CLEAN))
    result.clean = IRac::strToBool(json[KEY_CLEAN]);
  if (validJsonStr(json, KEY_FILTER))
    result.filter = IRac::strToBool(json[KEY_FILTER]);
  if (validJsonStr(json, KEY_BEEP))
    result.beep = IRac::strToBool(json[KEY_BEEP]);
  if (validJsonStr(json, KEY_CELSIUS))
    result.celsius = IRac::strToBool(json[KEY_CELSIUS]);
  return result;
}
#endif  // MQTT_CLIMATE_JSON

void updateClimate(stdAc::state_t *state, const String str,
                   const String prefix, const String payload) {
#if MQTT_CLIMATE_JSON
  if (str.equals(prefix + KEY_JSON))
    *state = jsonToState(*state, payload.c_str());
  else
#endif  // MQTT_CLIMATE_JSON
  if (str.equals(prefix + KEY_PROTOCOL))
    state->protocol = strToDecodeType(payload.c_str());
  else if (str.equals(prefix + KEY_MODEL))
    state->model = IRac::strToModel(payload.c_str());
  else if (str.equals(prefix + KEY_POWER))
    state->power = IRac::strToBool(payload.c_str());
  else if (str.equals(prefix + KEY_MODE))
    state->mode = IRac::strToOpmode(payload.c_str());
  else if (str.equals(prefix + KEY_TEMP))
    state->degrees = payload.toFloat();
  else if (str.equals(prefix + KEY_FANSPEED))
    state->fanspeed = IRac::strToFanspeed(payload.c_str());
  else if (str.equals(prefix + KEY_SWINGV))
    state->swingv = IRac::strToSwingV(payload.c_str());
  else if (str.equals(prefix + KEY_SWINGH))
    state->swingh = IRac::strToSwingH(payload.c_str());
  else if (str.equals(prefix + KEY_QUIET))
    state->quiet = IRac::strToBool(payload.c_str());
  else if (str.equals(prefix + KEY_TURBO))
    state->turbo = IRac::strToBool(payload.c_str());
  else if (str.equals(prefix + KEY_ECONO))
    state->econo = IRac::strToBool(payload.c_str());
  else if (str.equals(prefix + KEY_LIGHT))
    state->light = IRac::strToBool(payload.c_str());
  else if (str.equals(prefix + KEY_BEEP))
    state->beep = IRac::strToBool(payload.c_str());
  else if (str.equals(prefix + KEY_FILTER))
    state->filter = IRac::strToBool(payload.c_str());
  else if (str.equals(prefix + KEY_CLEAN))
    state->clean = IRac::strToBool(payload.c_str());
  else if (str.equals(prefix + KEY_CELSIUS))
    state->celsius = IRac::strToBool(payload.c_str());
  else if (str.equals(prefix + KEY_SLEEP))
    state->sleep = payload.toInt();
}

bool sendClimate(const String topic_prefix, const bool retain,
                 const bool forceMQTT, const bool forceIR,
                 const bool enableIR, IRac *ac) {
  bool diff = false;
  bool success = true;
  const stdAc::state_t next = ac->getState();
  const stdAc::state_t prev = ac->getStatePrev();
  if (prev.protocol != next.protocol || forceMQTT) {
    diff = true;
    success &= sendString(topic_prefix + KEY_PROTOCOL,
                          typeToString(next.protocol), retain);
  }
  if (prev.model != next.model || forceMQTT) {
    diff = true;
    success &= sendInt(topic_prefix + KEY_MODEL, next.model, retain);
  }
  String mode_str = IRac::opmodeToString(next.mode);
  // I don't know why, but the modes need to be lower case to work with
  // Home Assistant & Google Home.
  mode_str.toLowerCase();
#if MQTT_CLIMATE_HA_MODE
  // Home Assistant want's these two bound together.
  if (prev.power != next.power || prev.mode != next.mode || forceMQTT) {
    success &= sendBool(topic_prefix + KEY_POWER, next.power, retain);
    if (!next.power) mode_str = F("off");
#else  // MQTT_CLIMATE_HA_MODE
  // In non-Home Assistant mode, power and mode are not bound together.
  if (prev.power != next.power || forceMQTT) {
    diff = true;
    success &= sendBool(topic_prefix + KEY_POWER, next.power, retain);
  }
  if (prev.mode != next.mode || forceMQTT) {
#endif  // MQTT_CLIMATE_HA_MODE
    success &= sendString(topic_prefix + KEY_MODE, mode_str, retain);
    diff = true;
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
                          IRac::fanspeedToString(next.fanspeed), retain);
  }
  if (prev.swingv != next.swingv || forceMQTT) {
    diff = true;
    success &= sendString(topic_prefix + KEY_SWINGV,
                          IRac::swingvToString(next.swingv), retain);
  }
  if (prev.swingh != next.swingh || forceMQTT) {
    diff = true;
    success &= sendString(topic_prefix + KEY_SWINGH,
                          IRac::swinghToString(next.swingh), retain);
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
  if (diff && !forceMQTT) {
    debug("Difference in common A/C state detected.");
#if MQTT_CLIMATE_JSON
    sendJsonState(next, topic_prefix + KEY_JSON);
#endif  // MQTT_CLIMATE_JSON
  } else {
    debug("NO difference in common A/C state detected.");
  }
  // Only send an IR message if we need to.
  if (enableIR && ((diff && !forceMQTT) || forceIR)) {
    sendReqCounter++;
    if (ac == NULL) {  // No climate object is available.
      debug("Can't send climate state as common A/C object doesn't exist!");
      return false;
    }
    debug("Sending common A/C state via IR.");
#if IR_RX && DISABLE_CAPTURE_WHILE_TRANSMITTING
    // Turn IR capture off if we need to.
    if (irrecv != NULL) irrecv->disableIRIn();  // Stop the IR receiver
#endif  // IR_RX && DISABLE_CAPTURE_WHILE_TRANSMITTING
    lastClimateSucceeded = ac->sendAc();
#if IR_RX && DISABLE_CAPTURE_WHILE_TRANSMITTING
    // Turn IR capture back on if we need to.
    if (irrecv != NULL) irrecv->enableIRIn();  // Restart the receiver
#endif  // IR_RX && DISABLE_CAPTURE_WHILE_TRANSMITTING
    if (lastClimateSucceeded) hasClimateBeenSent = true;
    success &= lastClimateSucceeded;
    lastClimateIr.reset();
    irClimateCounter++;
  }
  return success;
}

#if USE_DECODED_AC_SETTINGS && IR_RX
// Decode and use a valid IR A/C remote that we understand enough to convert
// to a Common A/C format.
// Args:
//   decode: A successful raw IR decode object.
// Returns:
//   A boolean indicating success or failure.
bool decodeCommonAc(const decode_results *decode) {
  if (!IRac::isProtocolSupported(decode->decode_type)) {
    debug("Inbound IR messages isn't a supported common A/C protocol");
    return false;
  }
  if (climate[0] == NULL) {
    debug("No common A/C object allocated for channel 0. Skipping.");
    return false;
  }
  stdAc::state_t state = climate[0]->next;
  debug("Converting inbound IR A/C message to common A/C");
  if (!IRAcUtils::decodeToState(decode, &state, &(climate[0]->next))) {
      debug("Failed to convert to common A/C.");  // This shouldn't happen!
      return false;
  }
#if IGNORE_DECODED_AC_PROTOCOL
  if (climate[0]->next.protocol != decode_type_t::UNKNOWN) {
    // Use the previous protcol/model if set.
    state.protocol = climate[0]->next.protocol;
    state.model = climate[0]->next.model;
  }
#endif  // IGNORE_DECODED_AC_PROTOCOL
// Continue to use the previously prefered temperature units.
// i.e. Keep using Celsius or Fahrenheit.
if (climate[0]->next.celsius != state.celsius) {
  // We've got a mismatch, so we need to convert.
  state.degrees = climate[0]->next.celsius ? fahrenheitToCelsius(state.degrees)
                                           : celsiusToFahrenheit(state.degrees);
  state.celsius = climate[0]->next.celsius;
}
climate[0]->next = state;  // Copy over the new climate state.
#if MQTT_ENABLE
  sendClimate(genStatTopic(0), true, false, REPLAY_DECODED_AC_MESSAGE,
              REPLAY_DECODED_AC_MESSAGE, climate[0]);
#else  // MQTT_ENABLE
  sendClimate("", false, false, REPLAY_DECODED_AC_MESSAGE,
              REPLAY_DECODED_AC_MESSAGE, climate[0]);
#endif  // MQTT_ENABLE
  return true;
}
#endif  // USE_DECODED_AC_SETTINGS && IR_RX
