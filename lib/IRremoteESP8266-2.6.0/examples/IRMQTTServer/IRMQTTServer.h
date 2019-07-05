/*
 * Send & receive arbitrary IR codes via a web server or MQTT.
 * Copyright David Conran 2016, 2017, 2018, 2019
 */
#ifndef EXAMPLES_IRMQTTSERVER_IRMQTTSERVER_H_
#define EXAMPLES_IRMQTTSERVER_IRMQTTSERVER_H_

#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRtimer.h>
#include <IRutils.h>
#include <IRac.h>

// ---------------- Start of User Configuration Section ------------------------

#ifndef MQTT_ENABLE
#define MQTT_ENABLE true  // Whether or not MQTT is used at all.
#endif  // MQTT_ENABLE

// ---------------------- Board Related Settings -------------------------------
// NOTE: Make sure you set your Serial Monitor to the same speed.
#define BAUD_RATE 115200  // Serial port Baud rate.

// GPIO the IR LED is connected to/controlled by. GPIO 4 = D2.
#define IR_LED 4  // <=- CHANGE_ME (optional)
// define IR_LED 3  // For an ESP-01 we suggest you use RX/GPIO3/Pin 7.

// GPIO the IR RX module is connected to/controlled by. e.g. GPIO 14 = D5.
// Comment this out to disable receiving/decoding IR messages entirely.
#define IR_RX 14  // <=- CHANGE_ME (optional)
#define IR_RX_PULLUP false

// --------------------- Network Related Settings ------------------------------
const uint16_t kHttpPort = 80;  // The TCP port the HTTP server is listening on.
// Change to 'true'/'false' if you do/don't want these features or functions.
#define USE_STATIC_IP false  // Change to 'true' if you don't want to use DHCP.
// We obtain our network config via DHCP by default but allow an easy way to
// use a static IP config.
#if USE_STATIC_IP
const IPAddress kIPAddress = IPAddress(10, 0, 1, 78);
const IPAddress kGateway = IPAddress(10, 0, 1, 1);
const IPAddress kSubnetMask = IPAddress(255, 255, 255, 0);
#endif  // USE_STATIC_IP

// See: https://github.com/tzapu/WiFiManager#filter-networks for these settings.
#define HIDE_DUPLIATE_NETWORKS false  // Should WifiManager hide duplicate SSIDs
// #define MIN_SIGNAL_STRENGTH 20  // Minimum WiFi signal stength (percentage)
                                   // before we will connect.
                                   // The unset default is 8%.
                                   // (Uncomment to enable)

// ----------------------- HTTP Related Settings -------------------------------
#define FIRMWARE_OTA true  // Allow remote update of the firmware via http.
                           // Less secure if enabled.
                           // Note: Firmware OTA is also disabled until
                           //       a password is set.
#define HTML_PASSWORD_ENABLE false  // Protect access to the HTML interface.
                                    // Note: OTA update is always passworded.
// If you do not set a password, Firmware OTA updates will be blocked.

// ----------------------- MQTT Related Settings -------------------------------
#if MQTT_ENABLE
const uint32_t kMqttReconnectTime = 5000;  // Delay(ms) between reconnect tries.

#define MQTT_ACK "sent"  // Sub-topic we send back acknowledgements on.
#define MQTT_SEND "send"  // Sub-topic we get new commands from.
#define MQTT_RECV "received"  // Topic we send received IRs to.
#define MQTT_LOG "log"  // Topic we send log messages to.
#define MQTT_LWT "status"  // Topic for the Last Will & Testament.
#define MQTT_CLIMATE "ac"  // Sub-topic for the climate topics.
#define MQTT_CLIMATE_CMND "cmnd"  // Sub-topic for the climate command topics.
#define MQTT_CLIMATE_STAT "stat"  // Sub-topic for the climate stat topics.
#define MQTTbroadcastInterval 10 * 60  // Seconds between rebroadcasts

#define QOS 1  // MQTT broker should queue up any unreceived messages for us
// #define QOS 0  // MQTT broker WON'T queue up messages for us. Fire & Forget.
#endif  // MQTT_ENABLE

// ------------------------ IR Capture Settings --------------------------------
// Let's use a larger than normal buffer so we can handle AirCon remote codes.
const uint16_t kCaptureBufferSize = 1024;
#if DECODE_AC
// Some A/C units have gaps in their protocols of ~40ms. e.g. Kelvinator
// A value this large may swallow repeats of some protocols
const uint8_t kCaptureTimeout = 50;  // Milliseconds
#else  // DECODE_AC
// Suits most messages, while not swallowing many repeats.
const uint8_t kCaptureTimeout = 15;  // Milliseconds
#endif  // DECODE_AC
// Ignore unknown messages with <10 pulses (see also REPORT_UNKNOWNS)
const uint16_t kMinUnknownSize = 2 * 10;
#define REPORT_UNKNOWNS false  // Report inbound IR messages that we don't know.
#define REPORT_RAW_UNKNOWNS false  // Report the whole buffer, recommended:
                                   // MQTT_MAX_PACKET_SIZE of 1024 or more

// ------------------------ Advanced Usage Only --------------------------------
// Change if you need multiple independent send gpio/topics.
const uint8_t gpioTable[] = {
  IR_LED,  // Default GPIO. e.g. ir_server/send or ir_server/send_0
  // Uncomment the following as needed.
  // NOTE: Remember to disable DEBUG if you are using one of the serial pins.
  // 5,  // GPIO 5 / D1 e.g. ir_server/send_1
  // 14,  // GPIO 14 / D5 e.g. ir_server/send_2
  // 16,  // GPIO 16 / D0 e.g. ir_server/send_3
};

#define KEY_PROTOCOL "protocol"
#define KEY_MODEL "model"
#define KEY_POWER "power"
#define KEY_MODE "mode"
#define KEY_TEMP "temp"
#define KEY_FANSPEED "fanspeed"
#define KEY_SWINGV "swingv"
#define KEY_SWINGH "swingh"
#define KEY_QUIET "quiet"
#define KEY_TURBO "turbo"
#define KEY_LIGHT "light"
#define KEY_BEEP "beep"
#define KEY_ECONO "econo"
#define KEY_SLEEP "sleep"
#define KEY_CLOCK "clock"
#define KEY_FILTER "filter"
#define KEY_CLEAN "clean"
#define KEY_CELSIUS "use_celsius"

// HTML arguments we will parse for IR code information.
#define KEY_TYPE "type"  // KEY_PROTOCOL is also checked too.
#define KEY_CODE "code"
#define KEY_BITS "bits"
#define KEY_REPEAT "repeats"

// Text for Last Will & Testament status messages.
const char* kLwtOnline = "Online";
const char* kLwtOffline = "Offline";

const uint8_t kHostnameLength = 30;
const uint8_t kPortLength = 5;  // Largest value of uint16_t is "65535".
const uint8_t kUsernameLength = 15;
const uint8_t kPasswordLength = 20;

// -------------------------- Debug Settings -----------------------------------
// Disable debug output if any of the IR pins are on the TX (D1) pin.
// Note: This is a crude method to catch the common use cases.
// See `isSerialGpioUsedByIr()` for the better method.
#if (IR_LED != 1 && IR_RX != 1)
#ifndef DEBUG
#define DEBUG true  // Change to 'false' to disable all serial output.
#endif  // DEBUG
#else  // (IR_LED != 1 && IR_RX != 1)
#undef DEBUG
#define DEBUG false
#endif

// ----------------- End of User Configuration Section -------------------------

// Constants
#define _MY_VERSION_ "v1.0.0"

const uint8_t kSendTableSize = sizeof(gpioTable);
// JSON stuff
// Name of the json config file in SPIFFS.
const char* kConfigFile = "/config.json";
const char* kMqttServerKey = "mqtt_server";
const char* kMqttPortKey = "mqtt_port";
const char* kMqttUserKey = "mqtt_user";
const char* kMqttPassKey = "mqtt_pass";
const char* kMqttPrefixKey = "mqtt_prefix";
const char* kHostnameKey = "hostname";
const char* kHttpUserKey = "http_user";
const char* kHttpPassKey = "http_pass";

#if MQTT_ENABLE
const uint32_t kBroadcastPeriodMs = MQTTbroadcastInterval * 1000;  // mSeconds.
const uint32_t kStatListenPeriodMs = 5 * 1000;  // mSeconds

void mqttCallback(char* topic, byte* payload, unsigned int length);
String listOfCommandTopics(void);
void handleSendMqttDiscovery(void);
void subscribing(const String topic_name);
void unsubscribing(const String topic_name);
void mqttLog(const String mesg);
bool reconnect(void);
void receivingMQTT(String const topic_name, String const callback_str);
void callback(char* topic, byte* payload, unsigned int length);
void sendMQTTDiscovery(const char *topic);
void doBroadcast(TimerMs *timer, const uint32_t interval,
                 const commonAcState_t state, const bool retain,
                 const bool force);
#endif  // MQTT_ENABLE
bool isSerialGpioUsedByIr(void);
void debug(const char *str);
void saveWifiConfigCallback(void);
void saveWifiConfig(void);
void loadWifiConfigFile(void);
String msToHumanString(uint32_t const msecs);
String timeElapsed(uint32_t const msec);
String timeSince(uint32_t const start);
String listOfSendGpios(void);
bool hasUnsafeHTMLChars(String input);
String htmlMenu(void);
void handleRoot(void);
String addJsReloadUrl(const String url, const uint16_t timeout_s,
                      const bool notify);
void handleExamples(void);
String boolToString(const bool value);
String opmodeToString(const stdAc::opmode_t mode);
String fanspeedToString(const stdAc::fanspeed_t speed);
String swingvToString(const stdAc::swingv_t swingv);
String swinghToString(const stdAc::swingh_t swingh);
String htmlSelectBool(const String name, const bool def);
String htmlSelectProtocol(const String name, const decode_type_t def);
String htmlSelectModel(const String name, const int16_t def);
String htmlSelectMode(const String name, const stdAc::opmode_t def);
String htmlSelectFanspeed(const String name, const stdAc::fanspeed_t def);
String htmlSelectSwingv(const String name, const stdAc::swingv_t def);
String htmlSelectSwingh(const String name, const stdAc::swingh_t def);
void handleAirCon(void);
void handleAirConSet(void);
void handleAdmin(void);
void handleInfo(void);
void handleReset(void);
void handleReboot(void);
bool parseStringAndSendAirCon(IRsend *irsend, const uint16_t irType,
                              const String str);
uint16_t countValuesInStr(const String str, char sep);
uint16_t * newCodeArray(const uint16_t size);
#if SEND_GLOBALCACHE
bool parseStringAndSendGC(IRsend *irsend, const String str);
#endif  // SEND_GLOBALCACHE
#if SEND_PRONTO
bool parseStringAndSendPronto(IRsend *irsend, const String str,
                              uint16_t repeats);
#endif  // SEND_PRONTO
#if SEND_RAW
bool parseStringAndSendRaw(IRsend *irsend, const String str);
#endif  // SEND_RAW
void handleIr(void);
void handleNotFound(void);
void setup_wifi(void);
void init_vars(void);
void setup(void);
void loop(void);
uint64_t getUInt64fromHex(char const *str);
bool sendIRCode(IRsend *irsend, int const ir_type,
                uint64_t const code, char const * code_str, uint16_t bits,
                uint16_t repeat);
bool sendInt(const String topic, const int32_t num, const bool retain);
bool sendBool(const String topic, const bool on, const bool retain);
bool sendString(const String topic, const String str, const bool retain);
bool sendFloat(const String topic, const float_t temp, const bool retain);
commonAcState_t updateClimate(commonAcState_t current, const String str,
                              const String prefix, const String payload);
bool cmpClimate(const commonAcState_t a, const commonAcState_t b);
bool sendClimate(const commonAcState_t prev, const commonAcState_t next,
                 const String topic_prefix, const bool retain,
                 const bool forceMQTT, const bool forceIR);
#endif  // EXAMPLES_IRMQTTSERVER_IRMQTTSERVER_H_
