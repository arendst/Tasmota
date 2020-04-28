/*
 * Send & receive arbitrary IR codes via a web server or MQTT.
 * Copyright David Conran 2016, 2017, 2018, 2019
 */
#ifndef EXAMPLES_IRMQTTSERVER_IRMQTTSERVER_H_
#define EXAMPLES_IRMQTTSERVER_IRMQTTSERVER_H_

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#endif  // ESP8266
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRtext.h>
#include <IRtimer.h>
#include <IRutils.h>
#include <IRac.h>

// ---------------- Start of User Configuration Section ------------------------

#ifndef MQTT_ENABLE
#define MQTT_ENABLE true  // Whether or not MQTT is used at all.
#endif  // MQTT_ENABLE

#ifndef EXAMPLES_ENABLE
// Whether or not examples are included. `false` saves ~2.5K of program space.
#define EXAMPLES_ENABLE true
#endif  // EXAMPLES_ENABLE

// ---------------------- Board Related Settings -------------------------------
// NOTE: Make sure you set your Serial Monitor to the same speed.
#define BAUD_RATE 115200  // Serial port Baud rate.

// Change if you need multiple independent send gpios & topics. (MQTT only)
const uint8_t kNrOfIrTxGpios = 1;
// Default GPIO the IR LED is connected to/controlled by. GPIO 4 = D2.
// For an ESP-01 we suggest you use RX/GPIO3/Pin 7. i.e. kDefaultIrLed = 3
// Note: A value of -1 means unused.
const int8_t kDefaultIrLed = 4;  // <=- CHANGE_ME (optional)

// **DANGER** Optional flag to invert the output. (default = false)
//            `false`: The LED is illuminated when the GPIO is HIGH.
//            `true`: The LED is illuminated when GPIO is LOW rather than HIGH.
//            Setting this to something other than the default could
//            easily destroy your IR LED if you are overdriving it.
//            Unless you *REALLY* know what you are doing, don't change this.
const bool kInvertTxOutput = false;

// Default GPIO the IR demodulator is connected to/controlled by. GPIO 14 = D5.
// Note: GPIO 16 won't work on the ESP8266 as it does not have interrupts.
const int8_t kDefaultIrRx = 14;  // <=- CHANGE_ME (optional)

// Enable/disable receiving/decoding IR messages entirely.
// Note: IR_RX costs about 40k+ of program memory.
#define IR_RX true

// Should we use PULLUP on the IR Rx gpio?
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
#define HIDE_DUPLICATE_NETWORKS false  // Make WifiManager hide duplicate SSIDs
// #define MIN_SIGNAL_STRENGTH 20  // Minimum WiFi signal stength (percentage)
                                   // before we will connect.
                                   // The unset default is 8%.
                                   // (Uncomment to enable)
// Do you want/need mdns enabled? (https://en.wikipedia.org/wiki/Multicast_DNS)
#define MDNS_ENABLE true  // `false` to disable and save ~21k of program space.

// ----------------------- HTTP Related Settings -------------------------------
#define FIRMWARE_OTA true  // Allow remote update of the firmware via http.
                           // Less secure if enabled.
                           // Note: Firmware OTA is also disabled until
                           //       a password is set.
#define HTML_PASSWORD_ENABLE false  // Protect access to the HTML interface.
                                    // Note: OTA & GPIO updates are always
                                    //       passworded.
// If you do not set a password, Firmware OTA & GPIO updates will be blocked.

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
// Enable sending/receiving climate via JSON. `true` cost ~5k of program space.
#define MQTT_CLIMATE_JSON false
// Use Home Assistant-style operation modes.
// i.e. Change the climate mode to "off" when turning the power "off".
// See: https://www.home-assistant.io/components/climate.mqtt/#modes
// Change to false, if your home automation system doesn't like this.
#define MQTT_CLIMATE_HA_MODE true
// Do we send an IR message when we reboot and recover the existing A/C state?
// If set to `false` you may miss requested state changes while the ESP was
// down. If set to `true`, it will resend the previous desired state sent to the
// A/C. Depending on your circumstances, you may need to change this.
#define MQTT_CLIMATE_IR_SEND_ON_RESTART false
#define MQTTbroadcastInterval 10 * 60  // Seconds between rebroadcasts.

#define QOS 1  // MQTT broker should queue up any unreceived messages for us
// #define QOS 0  // MQTT broker WON'T queue up messages for us. Fire & Forget.

// Enable(true)/Disable(false) the option to send a MQTT Discovery message for
// the AirCon/Climate system to Home Assistant. Note: `false` saves ~1.5k.
#define MQTT_DISCOVERY_ENABLE true
// Enable(true)/Disable(false) the option to clear any settings stored in MQTT
// for this device's current config. e.g. Climate states using MQTT retain.
// In theory, you shouldn't need this as you can always clean up by hand, hence
// it is disabled by default. Note: `false` saves ~1.2k.
#define MQTT_CLEAR_ENABLE false
#endif  // MQTT_ENABLE

// ------------------------ IR Capture Settings --------------------------------
// Should we stop listening for IR messages when we send a message via IR?
// Set this to `true` if your IR demodulator is picking up self transmissions.
// Use `false` if it isn't or can't see the self-sent transmissions
// Using `true` may mean some incoming IR messages are lost or garbled.
// i.e. `false` is better if you can get away with it.
#define DISABLE_CAPTURE_WHILE_TRANSMITTING true
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

// Should we use and report individual A/C settings we capture via IR if we
// can understand the individual settings of the remote.
// e.g. Aquire the A/C settings from an actual A/C IR remote and override
//      any local settings set via MQTT/HTTP etc.
#define USE_DECODED_AC_SETTINGS true  // `false` to disable. `true` to enable.
// Should we allow or ignore an A/C IR remote to override the A/C protocol/model
// as set via MQTT or HTTP?
// e.g. If `true`, you can use any fully supported A/C remote to control
//      another brand's or model's A/C unit. `false` means change to the new
//      protocol/model if we support it via `USE_DECODED_AC_SETTINGS`.
#define IGNORE_DECODED_AC_PROTOCOL true
// Do we (re-)send the captured & decoded A/C message via the IR_LED?
// `false` if you don't want to repeat the captured message.
// e.g. Useful if the IR demodulator is located in the path between the remote
//      and the A/C unit so the command isn't sent twice.
// `true` if you want it sent anyway.
// e.g. The IR demodulator is in a completely different location than than the
//      actual a/c unit.
#define REPLAY_DECODED_AC_MESSAGE false

// ------------------------ Advanced Usage Only --------------------------------

// Reports the input voltage to the ESP chip. **NOT** the input voltage
// to the development board (e.g. NodeMCU, D1 Mini etc) which are typically
// powered by USB (5V) which is then lowered to 3V via a Low Drop Out (LDO)
// Voltage regulator. Hence, this feature is turned off by default as it
// make little sense for most users as it really isn't the actual input voltage.
// E.g. For purposes of monitoring a battery etc.
// Note: Turning on the feature costs ~250 bytes of prog space.
#define REPORT_VCC false  // Do we report Vcc via html info page & MQTT?

// Keywords for MQTT topics, html arguments, or config file.
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
#define KEY_FILTER "filter"
#define KEY_CLEAN "clean"
#define KEY_CELSIUS "use_celsius"
#define KEY_JSON "json"
#define KEY_RESEND "resend"
#define KEY_VCC "vcc"

// HTML arguments we will parse for IR code information.
#define KEY_TYPE "type"  // KEY_PROTOCOL is also checked too.
#define KEY_CODE "code"
#define KEY_BITS "bits"
#define KEY_REPEAT "repeats"
#define KEY_CHANNEL "channel"  // Which IR TX channel to send on.

// GPIO html/config keys
#define KEY_TX_GPIO "tx"
#define KEY_RX_GPIO "rx"

// Text for Last Will & Testament status messages.
const char* kLwtOnline = "Online";
const char* kLwtOffline = "Offline";

const uint8_t kHostnameLength = 30;
const uint8_t kPortLength = 5;  // Largest value of uint16_t is "65535".
const uint8_t kUsernameLength = 15;
const uint8_t kPasswordLength = 20;

// -------------------------- Json Settings ------------------------------------

const uint16_t kJsonConfigMaxSize = 512;    // Bytes
const uint16_t kJsonAcStateMaxSize = 1024;  // Bytes

// -------------------------- Debug Settings -----------------------------------
// Debug output is disabled if any of the IR pins are on the TX (D1) pin.
// See `isSerialGpioUsedByIr()`.
// Note: Debug costs ~6k of program space.
#ifndef DEBUG
#define DEBUG false  // Change to 'true' for serial debug output.
#endif  // DEBUG

// ----------------- End of User Configuration Section -------------------------

// Constants
#define _MY_VERSION_ "v1.4.9"

const uint8_t kRebootTime = 15;  // Seconds
const uint8_t kQuickDisplayTime = 2;  // Seconds

// Common bit sizes for the simple protocols.
const uint8_t kCommonBitSizes[] = {
    12, 13, 15, 16, 20, 24, 28, 32, 35, 36, 42, 48, 56, 64};
// Gpio related
#if defined(ESP8266)
const int8_t kTxGpios[] = {-1, 0, 1, 2, 3, 4, 5, 12, 13, 14, 15, 16};
const int8_t kRxGpios[] = {-1, 0, 1, 2, 3, 4, 5, 12, 13, 14, 15};
#endif  // ESP8266
#if defined(ESP32)
// Ref: https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
const int8_t kTxGpios[] = {
    -1, 0, 1, 2, 3, 4, 5, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23,
    25, 26, 27, 32, 33};
const int8_t kRxGpios[] = {
    -1, 1, 2, 3, 4, 5, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23,
    25, 26, 27, 32, 33, 34, 35, 36, 39};
#endif  // ESP32

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
const char* kCommandDelimiter = ",";

// URLs
const char* kUrlRoot = "/";
const char* kUrlAdmin = "/admin";
const char* kUrlAircon = "/aircon";
const char* kUrlSendDiscovery = "/send_discovery";
const char* kUrlExamples = "/examples";
const char* kUrlGpio = "/gpio";
const char* kUrlGpioSet = "/gpio/set";
const char* kUrlInfo = "/info";
const char* kUrlReboot = "/quitquitquit";
const char* kUrlWipe = "/reset";
const char* kUrlClearMqtt = "/clear_retained";

#if MQTT_ENABLE
const uint32_t kBroadcastPeriodMs = MQTTbroadcastInterval * 1000;  // mSeconds.
// How long should we listen to recover for previous states?
// Default is 5 seconds per IR TX GPIOs (channels) used.
const uint32_t kStatListenPeriodMs = 5 * 1000 * kNrOfIrTxGpios;  // mSeconds
const int32_t kMaxPauseMs = 10000;  // 10 Seconds.
const char* kSequenceDelimiter = ";";
const char kPauseChar = 'P';
#if defined(ESP8266)
const uint32_t kChipId = ESP.getChipId();
#endif  // ESP8266
#if defined(ESP32)
const uint32_t kChipId = ESP.getEfuseMac();  // Discard the top 16 bits.
#endif  // ESP32

const char* kClimateTopics =
    "(" KEY_PROTOCOL "|" KEY_MODEL "|" KEY_POWER "|" KEY_MODE "|" KEY_TEMP "|"
    KEY_FANSPEED "|" KEY_SWINGV "|" KEY_SWINGH "|" KEY_QUIET "|"
    KEY_TURBO "|" KEY_LIGHT "|" KEY_BEEP "|" KEY_ECONO "|" KEY_SLEEP "|"
    KEY_FILTER "|" KEY_CLEAN "|" KEY_CELSIUS "|" KEY_RESEND
#if MQTT_CLIMATE_JSON
    "|" KEY_JSON
#endif  // MQTT_CLIMATE_JSON
    ")<br>";
const char* kMqttTopics[] = {
    KEY_PROTOCOL, KEY_MODEL, KEY_POWER, KEY_MODE, KEY_TEMP, KEY_FANSPEED,
    KEY_SWINGV, KEY_SWINGH, KEY_QUIET, KEY_TURBO, KEY_LIGHT, KEY_BEEP,
    KEY_ECONO, KEY_SLEEP, KEY_FILTER, KEY_CLEAN, KEY_CELSIUS, KEY_RESEND,
    KEY_JSON};  // KEY_JSON needs to be the last one.


void mqttCallback(char* topic, byte* payload, unsigned int length);
String listOfCommandTopics(void);
void handleSendMqttDiscovery(void);
void subscribing(const String topic_name);
void unsubscribing(const String topic_name);
void mqttLog(const char* str);
bool mountSpiffs(void);
bool reconnect(void);
void receivingMQTT(String const topic_name, String const callback_str);
void callback(char* topic, byte* payload, unsigned int length);
void sendMQTTDiscovery(const char *topic);
void doBroadcast(TimerMs *timer, const uint32_t interval,
                 IRac *climates[], const bool retain,
                 const bool force);
#if MQTT_CLIMATE_JSON
stdAc::state_t jsonToState(const stdAc::state_t current, const char *str);
void sendJsonState(const stdAc::state_t state, const String topic,
                   const bool retain = false,
                   const bool ha_mode = MQTT_CLIMATE_HA_MODE);
#endif  // MQTT_CLIMATE_JSON
#endif  // MQTT_ENABLE
#if REPORT_VCC
String vccToString(void);
#endif  // REPORT_VCC
bool isSerialGpioUsedByIr(void);
void debug(const char *str);
void saveWifiConfigCallback(void);
void saveWifiConfig(void);
void loadWifiConfigFile(void);
void doRestart(const char* str, const bool serial_only = false);
String msToHumanString(uint32_t const msecs);
String timeElapsed(uint32_t const msec);
String timeSince(uint32_t const start);
String gpioToString(const int16_t gpio);
uint8_t getDefaultIrSendIdx(void);
IRsend* getDefaultIrSendPtr(void);
int8_t getDefaultTxGpio(void);
String genStatTopic(const uint16_t channel = 0);
String listOfTxGpios(void);
bool hasUnsafeHTMLChars(String input);
String htmlHeader(const String title, const String h1_text = "");
String htmlEnd(void);
String htmlButton(const String url, const String button,
                  const String text = "");
String htmlMenu(void);
void handleRoot(void);
String addJsReloadUrl(const String url, const uint16_t timeout_s,
                      const bool notify);
void handleExamples(void);
String htmlSelectBool(const String name, const bool def);
String htmlSelectClimateProtocol(const String name, const decode_type_t def);
String htmlSelectAcStateProtocol(const String name, const decode_type_t def,
                                 const bool simple);
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
bool parseStringAndSendAirCon(IRsend *irsend, const decode_type_t irType,
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
uint32_t maxSketchSpace(void);
uint64_t getUInt64fromHex(char const *str);
bool sendIRCode(IRsend *irsend, decode_type_t const ir_type,
                uint64_t const code, char const * code_str, uint16_t bits,
                uint16_t repeat);
bool sendInt(const String topic, const int32_t num, const bool retain);
bool sendBool(const String topic, const bool on, const bool retain);
bool sendString(const String topic, const String str, const bool retain);
bool sendFloat(const String topic, const float_t temp, const bool retain);
void updateClimate(stdAc::state_t *current, const String str,
                   const String prefix, const String payload);
bool cmpClimate(const stdAc::state_t a, const stdAc::state_t b);
bool sendClimate(const String topic_prefix, const bool retain,
                 const bool forceMQTT, const bool forceIR,
                 const bool enableIR = true, IRac *ac = NULL);
bool decodeCommonAc(const decode_results *decode);
#endif  // EXAMPLES_IRMQTTSERVER_IRMQTTSERVER_H_
