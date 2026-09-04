/*
  xsns_62_esp32_mi_ble.ino - MI-BLE-sensors via ESP32 support for Tasmota

  Copyright (C) 2020  Christian Baars, Simon Hailes, Robert Klauco and Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
//#define VSCODE_DEV

/*
#ifdef VSCODE_DEV
#define ESP32
#define USE_BLE_ESP32
#define USE_MI_ESP32
#endif
*/
//#undef USE_MI_ESP32

// for testing of BLE_ESP32, we remove xsns_62_MI_ESP32.ino completely, and instead add this modified xsns_52_ibeacon_BLE_ESP32.ino
#ifdef USE_BLE_ESP32

#ifdef ESP32                       // ESP32 family only. Use define USE_HM10 for ESP8266 support
#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C5 || CONFIG_IDF_TARGET_ESP32C6 || CONFIG_IDF_TARGET_ESP32S3

#ifdef USE_MI_ESP32

#define MI32_VERSION "v26.9.4"

/*********************************************************************************************\
  BLE Xiaomi/Mijia (MI) sensor decoding
  BLE BTHome V2 sensor decoding

  enabled by #define ESP32 && #define USE_BLE_ESP32
  if (#define ESP32 && no #define USE_BLE_ESP32) then xsns_62_esp32_mi.ino is used - the older driver

  --------------------------------------------------------------------------------------------
  Version yyyymmdd  Action    Description
  --------------------------------------------------------------------------------------------
  26.9.4            changed - BTHome reset button if datagram received (#25002)
  -------
  26.8.31           changed - display icons instead of data lines. disable by removing #define USE_SENSOR_ICON
  -------
  0.9.3.4 20260823  changed - redesign BTHome events and buffer JSON message for easier rule/script/berry support
  -------
  0.9.3.3 20260819  changed - fix BTHome decryption of multiple sensors
  -------
  0.9.3.2 20260816  changed - BTHome decoding additions
  -------
  0.9.3.1 20260804  changed - BTHome v2 device id table refresh based on https://github.com/Bluetooth-Devices/bthome-ble/blob/V2/src/bthome_ble/const.py
                              Add BTHome button decoding
                              Add BTHome binary catch all decoding
  -------
  0.9.3.0 20260803  changed - added BTHome v2 decryption
  -------
  0.9.2.8 20260323  changed - added BTHome v2 protocol support (UUID 0xFCD2)
  -------
  0.9.2.7 20251204  changed - display RSSI in general format "xx% (-yy dBm)"
                              view on UI only when BLE enabled
  -------
  0.9.2.6 20250503  changed - display alias instead of type, when present
  -------
  0.9.2.5 20250319  changed - added support for MI LYWSD02MMC with different device ID
  -------
  0.9.2.4 20240111  changed - Enhancement of debug log output                              
  -------
  0.9.2.3 20240101  changed - added initial support for MI LYWSD02MMC                              
  -------
  0.9.2.2 20231123  changed - added support for Avago Tech Bluetooth buttons                              
  -------
  0.9.2.1 20210217  changed - make features alos depend on received data - i.e. 'unknown' devices will show what they send.
                              Add MI32Option6 1 to switch to tele/tasmota_ble/<somename> style MQTT independent of HASS discovery.
  -------
  0.9.2.0 20210127  changed - Officially includes as the mi driver when using USE_BLE_ESP32.
  -------
  0.9.1.9 20201226  changed - All change now.
  -------
  0.9.1.7 20201116  changed - small bugfixes, add BLOCK and OPTION command, send BLE scan via MQTT
  -------
  0.9.1.6 20201022  changed - Beacon support, RSSI at TELEPERIOD, refactoring
  -------
  0.9.1.5 20201021  changed - HASS related ('null', hold back discovery), number of found sensors for RULES
  -------
  0.9.1.4 20201020  changed - use BearSSL for decryption, revert to old TELEPERIOD-cycle as default
  -------
  0.9.1.3 20200926  changed - Improve HA discovery, make key+MAC case insensitive
  -------
  0.9.1.3 20200916  changed - add ATC (custom FW for LYWSD03MMC), API adaption for NimBLE-Arduino 1.0.2
  -------
  0.9.1.2 20200802  changed - add MHO-C303
  -------
  0.9.1.1 20200715  changed - add MHO-C401, refactoring
  -------
  0.9.1.0 20200712  changed - add lights and yeerc, add pure passive mode with decryption,
                              lots of refactoring
  -------
  0.9.0.1 20200706  changed - adapt to new NimBLE-API, tweak scan process
  -------
  0.9.0.0 20200413  started - initial development by Christian Baars
                    forked  - from arendst/tasmota            - https://github.com/arendst/Tasmota
\*********************************************************************************************/

#define XSNS_62                62

//#define USE_SENSOR_ICON           // Display GUI icons instead of line with data

#define USE_MI_DECRYPTION         // Enable also for BTHome V2
//#define USE_MI_DEBUG              // Enable debug messages at the cost of more code size / flash usage

#include <vector>
#ifdef USE_MI_DECRYPTION
#include <t_bearssl.h>
#endif //USE_MI_DECRYPTION

void MI32scanEndedCB(NimBLEScanResults results);
void MI32notifyCB(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);

///////////////////////////////////////////////////////////
// OVERRIDBLES
//    Default values can be overrided in user_config_override.h

#ifndef MI32_OPTION0_ALWAYS_AGGREGATE
#define MI32_OPTION0_ALWAYS_AGGREGATE   true
#endif

#ifndef MI32_OPTION1_NO_SUMMARY
#define MI32_OPTION1_NO_SUMMARY false
#endif

#ifndef MI32_OPTION2_DIRECT_BRIDGED_MODE
#define MI32_OPTION2_DIRECT_BRIDGED_MODE false
#endif

#ifndef MI32_OPTION4_IGNORE_BOGUS_BATTERY
#define MI32_OPTION4_IGNORE_BOGUS_BATTERY true
#endif

#ifndef MI32_OPTION5_ONLY_ALIASED
#define MI32_OPTION5_ONLY_ALIASED false
#endif

#ifndef MI32_OPTION6_MQTT_TYPE
#define MI32_OPTION6_MQTT_TYPE 0
#endif

#ifndef MI32_PAGE
#define MI32_PAGE       4
#endif

#ifndef MI32_BLE_TOPIC
#define MI32_BLE_TOPIC "tasmota_ble"
#endif

///////////////////////////////////////////////////////////


struct {
  uint16_t perPage = MI32_PAGE;
  uint8_t mqttCurrentSlot = 0;
  uint8_t mqttCurrentSingleSlot = 0;
  String bleTopic = MI32_BLE_TOPIC;
  uint32_t period;             // set manually in addition to TELE-period, is set to TELE-period after start
  int secondsCounter = 0;   // counts up in MI32EverySecond to period
  int secondsCounter2 = 0;  // counts up in MI32EverySecond to period
  union {
    struct {
      uint32_t init:1;
      uint32_t shallClearResults:1;   // BLE scan results
      uint32_t shallShowStatusInfo:1; // react to amount of found sensors via RULES
      uint32_t firstAutodiscoveryDone:1;
      uint32_t shallTriggerTele:1;
      uint32_t triggeredTele:1;
    };
    uint32_t all = 0;
  } mode;

  struct {
    // the slot currently having it's battery read
    // set to 0 to start a battery read cycle
    uint8_t slot = 255;
    uint8_t active = 0;
  } batteryreader;

  struct {
    // the slot currently having it's battery read
    // set to 0 to start a battery read cycle
    uint8_t slot = 255;
    uint8_t active = 0;
  } sensorreader;

  struct {
    uint32_t allwaysAggregate:1;            // Mi32Option0: always show all known values of one sensor in bridge mode
    uint32_t noSummary:1;                   // Mi32Option1: no sensor values at TELE-period
    uint32_t directBridgeMode:1;            // Mi32Option2: send every received BLE-packet as a MQTT-message in real-time
    uint32_t holdBackFirstAutodiscovery:1;  //
    uint32_t showRSSI:1;                    //
    uint32_t ignoreBogusBattery:1;          // Mi32Option4:
    uint32_t minimalSummary:1;              // DEPRECATED!!
    uint32_t onlyAliased:1;                 // Mi32Option5: only include sensors that are aliased
    uint32_t MQTTType:2;                    // Mi32Option6: publish sensor on MI32.bleTopic with 1 topic per sensor
                                            //    Mi32Option6 0 : standard SENSOR message
                                            //    Mi32Option6 1 : MI32.bleTopic + per sensor topic (legacy syntax)
                                            //    MI32Option6 2 : same but add sensor name as key in JSON (same as standard SENSOR)
  } option;
} MI32;

#pragma pack(1)  // byte-aligned structures to read the sensor data

  struct {
    int16_t temp;
    uint8_t hum;
    uint16_t volt; // LYWSD03 only
  } LYWSD0x_HT;
  struct {
    uint8_t spare;
    int16_t temp;
    uint16_t hum;
  } CGD1_HT;
  struct {
    int16_t temp;
    uint8_t spare;
    uint32_t lux;
    uint8_t moist;
    uint16_t fert;
  } Flora_TLMF; // temperature, lux, moisture, fertility


////////////////////////////////////////////////////////////
// from https://github.com/Magalex2x14/LYWSD03MMC-info
struct mi_beacon_frame_data_t{
  // data from byte 0 - e.g. 30
  uint8_t meshflag;             //Byte 0: x.......
  uint8_t dataflag;             //Byte 0: .x......
  uint8_t compatibilityflag;    //Byte 0: ..x..... - indicates compatibility data present
  uint8_t MACFlag;              //Byte 0: ...x....
  uint8_t isencrypted;          //Byte 0: ....x...
  uint8_t reserved;             //Byte 0: .....xxx

  // data from byte 1 - e.g. 58
  uint8_t version;              //Byte 0: xxxx....
  uint8_t authMode;             //Byte 0: ....xx.. // e.g. 2
  uint8_t bindingvalidreq;      //Byte 0: ......x.
  uint8_t registeredflag;       //Byte 0: .......x
};

struct mi_beacon_compatibility_data_t{ // e.g. 28/08
  uint8_t reserved;             //Byte 0: xx......
  uint8_t IOcap;                //Byte 0: ..x.....
  uint8_t bondability;          //Byte 0: ...xx...
  uint8_t unused;               //Byte 0: .....xxx
  uint16_t IOCapability;        // bytes 1-2, e.g. 01 00 -> 0001
};
struct mi_beacon_mac_data_t{ // e.g. 28/08
  uint8_t mac[6];
};
struct mi_beacon_payload_data_t{ //
  uint16_t type;
  uint8_t size;
  uint8_t data[16];
};

struct mi_beacon_data_t { //
  mi_beacon_frame_data_t framedata;
  uint16_t devicetype;
  uint8_t framecnt;
  mi_beacon_mac_data_t macdata;
  mi_beacon_compatibility_data_t compatibility;
  uint8_t payloadpresent;
  uint8_t needkey; // we need a (new) encryption key?

  mi_beacon_payload_data_t payload;
};

struct mi_beacon_data_payload_data_t { //
  union {
    struct{ //01
      uint16_t num;
      uint8_t longPress;
    } Btn;

    int16_t temp; //04
    uint16_t hum; //06
    uint32_t lux; //07
    uint8_t moist; //08
    uint16_t fert; //09
    uint8_t bat; //0a
    struct{ //0d
      int16_t temp;
      uint16_t hum;
    } HT;
    uint32_t NMT; //17
  };
};

///////////////////////////////////////////////////////////

union mi_bindKey_t{
  struct{
    uint8_t key[16];
    uint8_t MAC[6];
    };
  uint8_t buf[22];
};

struct ATCPacket_t{
	//uint8_t		size;	// = 16?
	//uint8_t		uid;	// = 0x16, 16-bit UUID
	//uint16_t	UUID;	// = 0x181A, GATT Service 0x181A Environmental Sensing
  uint8_t MAC[6]; // [0] - hi, .. [6] - lo digits
  uint16_t temp; //sadly this is in wrong endianess
  uint8_t hum;
  uint8_t batPer;
  uint16_t batMV;
  uint8_t frameCnt;
};

// GATT Service 0x181A Environmental Sensing
// All data little-endian
struct PVVXPacket_t {
	//uint8_t		size;	// = 19
	//uint8_t		uid;	// = 0x16, 16-bit UUID
	//uint16_t	UUID;	// = 0x181A, GATT Service 0x181A Environmental Sensing
	uint8_t		MAC[6]; // [0] - lo, .. [6] - hi digits
	int16_t		temperature; // x 0.1 degree
	uint16_t	humidity; // x 0.01 %
	uint16_t	battery_mv; // mV
	uint8_t		battery_level; // 0..100 %
	uint8_t		counter; // measurement count
	uint8_t		flags;
};

struct CGDK2Packet_t {
	//uint8_t	size;	// = 17
	uint16_t 	framedata;
	uint8_t		MAC[6]; // [0] - lo, .. [6] - hi digits
	uint16_t 	devicetype;
	int16_t		temperature; // x 0.1 degree
	uint16_t	humidity; // x 0.01 %
	uint16_t	battery_mv; // mV
	uint8_t		battery_level; // 0..100 %
	uint8_t		counter; // measurement count
	uint8_t		flags;
};

struct MiScaleV1Packet_t {
	//uint8_t		size;	// = 14
	//uint8_t		uid;	// = 0x16, 16-bit UUID
	//uint16_t	UUID;	// = 0x181D
	uint8_t		status; // bit 0 lbs, 4 jin, 5, stabilized, 7, weight removed
	uint16_t	weight;
	uint16_t	year;
	uint8_t		month;
	uint8_t		day;
	uint8_t		hour;
	uint8_t		minute;
	uint8_t		second;
};

struct MiScaleV2Packet_t {
	//uint8_t		size;	// = 17
	//uint8_t		uid;	// = 0x16, 16-bit UUID
	//uint16_t	UUID;	// = 0x181B
	uint8_t		weight_unit;
	uint8_t		status; // bit 14 impedance stabilized
	uint16_t	year;
	uint8_t		month;
	uint8_t		day;
	uint8_t		hour;
	uint8_t		minute;
	uint8_t		second;
	uint16_t	impedance;
	uint16_t	weight;
};

#pragma pack(0)

struct mi_sensor_t{
  uint8_t type; //MI_Flora = 1; MI_MI-HT_V1=2; MI_LYWSD02=3; MI_LYWSD03=4; MI_CGG1=5; MI_CGD1=6
  uint8_t needkey; // tells http to display needkey message with link
  uint8_t lastCnt; //device generated counter of the packet
  uint8_t nextDiscoveryData; // used to lkimit discovery to one MQTT per sec

  uint8_t shallSendMQTT;
  uint8_t MAC[6];
  union {
    struct {
      uint32_t temp:1;
      uint32_t hum:1;
      uint32_t tempHum:1; //every hum sensor has temp too, easier to use Tasmota dew point functions
      uint32_t lux:1;
      uint32_t moist:1;
      uint32_t fert:1;
      uint32_t bat:1;
      uint32_t NMT:1;
      uint32_t PIR:1;
      uint32_t Btn:1;
      uint32_t events:1;
      uint32_t pairing:1;
      uint32_t light:1; // binary light sensor
      uint32_t scale:1;
      uint32_t impedance:1;
      uint32_t flooding:1;
      uint32_t volt:1;
      uint32_t pres:1;
      uint32_t accel:1;
      uint32_t count:1;
      uint32_t unparsed:1;
    };
    uint32_t raw;
  } feature;
  union {
    struct {
      uint32_t temp:1;
      uint32_t hum:1;
      uint32_t tempHum:1; //can be combined from the sensor
      uint32_t lux:1;
      uint32_t moist:1;
      uint32_t fert:1;
      uint32_t bat:1;
      uint32_t NMT:1;
      uint32_t motion:1;
      uint32_t noMotion:1;
      uint32_t Btn:1;
      uint32_t PairBtn:1;
      uint32_t light:1; // binary light sensor
      uint32_t scale:1;
      uint32_t flooding:1;
      uint32_t volt:1;
      uint32_t pres:1;
      uint32_t accel:1;
      uint32_t count:1;
      uint32_t event:1;
    };
    uint32_t raw;
  } eventType;

  uint32_t encr_cnt; // BTHome last encryption counter
  int RSSI;
  uint8_t pairing;
  int8_t light; // binary light sensor - initialise to -1
  int16_t Btn; // moved so we can initialise to -1
  int16_t flooding;

  uint32_t lastTime;
  uint32_t lux;
  int count;
  float temp; //Flora, MJ_HT_V1, LYWSD0x, CGx
  float volt; // BTHome voltage
  float pres; // BTHome pressure
  union {
    struct {
      uint8_t moisture;
      uint16_t fertility;
      char firmware[6]; // actually only for FLORA but hopefully we can add for more devices
    }; // Flora
    struct {
      float hum;
    }; // MJ_HT_V1, LYWSD0x
    struct {
      uint16_t events; //"alarms" since boot
      uint32_t NMT;    // no motion time in seconds for the MJYD2S
    };
  };
  union {
      uint8_t bat; // many values seem to be hard-coded garbage (LYWSD0x, GCD1)
  };
  union {
    struct {
      uint8_t weight_stabilized;
      uint8_t weight_removed;
      char weight_unit[4]; // kg, lbs, jin or empty when unknown
      float weight;
      uint8_t impedance_stabilized;
      uint16_t impedance;
    };
    struct {
      uint8_t accel_used;
      float acceleration[4];
    };
  };
  union {
    uint8_t button[8]; // BTHome button support
    struct {
      uint8_t first_buttons_overlay[4];
      uint8_t bthome_event[4]; // BTHome event support
    };
  };
  uint8_t unparsed[4]; // BTHome unparsed object id
  float unparsed_float[4];
};

struct MAC_t {
  uint8_t buf[7];
};

std::vector<mi_sensor_t> MIBLEsensors;
std::vector<mi_bindKey_t> MIBLEbindKeys;
std::vector<MAC_t> MIBLEBlockList;

SemaphoreHandle_t slotmutex = (SemaphoreHandle_t) nullptr;

/*********************************************************************************************\
 * constants
\*********************************************************************************************/

#define D_CMND_MI32 "MI32"

const char kMI32_Commands[] PROGMEM = D_CMND_MI32 "|"
#ifdef USE_MI_DECRYPTION
  "Key|"
  "Keys|"
#endif  // USE_MI_DECRYPTION
  "Period|Time|Page|Battery|Unit|Block|Option|Topic";

void (*const MI32_Commands[])(void) PROGMEM = {
#ifdef USE_MI_DECRYPTION
  &CmndMi32Key,
  &CmndMi32Keys,
#endif  // USE_MI_DECRYPTION
  &CmndMi32Period, &CmndMi32Time, &CmndMi32Page, &CmndMi32Battery, &CmndMi32Unit, &CmndMi32Block,
  &CmndMi32Option, &CmndMi32Topic };


#define MI_UNKOWN      1
#define MI_FLORA       2
#define MI_MJ_HT_V1    3
#define MI_LYWSD02     4
#define MI_LYWSD03MMC  5
#define MI_CGG1        6
#define MI_CGD1        7
#define MI_NLIGHT      8
#define MI_MJYD2S      9
#define MI_YEERC       10
#define MI_MHOC401     11
#define MI_MHOC303     12
#define MI_ATC         13
#define MI_DOOR        14
#define MI_SCALE_V1    15
#define MI_SCALE_V2    16
#define MI_CGDK2       17
#define AT_BTN         18
#define MI_SJWS01LM    19
#define MI_LYWSD02MMC  20
#define MI_LYWSD02MMC2 21
#define MI_BTHOME      22

#define MI_MI32_TYPES  22 //count this manually

const uint16_t kMI32DeviceID[MI_MI32_TYPES]={
  0x0000, // Unkown
  0x0098, // Flora
  0x01aa, // MJ_HT_V1
  0x045b, // LYWSD02
  0x055b, // LYWSD03
  0x0347, // CGG1
  0x0576, // CGD1
  0x03dd, // NLIGHT
  0x07f6, // MJYD2S
  0x0153, // yee-rc
  0x0387, // MHO-C401
  0x06d3, // MHO-C303
  0x0a1c, // ATC -> this is a fake ID
  0x098b, // door/window sensor
  0x181d, // Mi Scale V1
  0x181b, // Mi Scale V2
  0x066f, // CGDK2
  0x004e, // Avago Tech Bluetooth Buttons (Company Id)
  0x0863, // SJWS01LM
  0x2542, // LYWSD02MMC
  0x16e4, // LYWSD02MMC F3_A1 hardware revision, 2.0.1_0060 firmware revision
  0xFCD2  // BTHome v2
};

const char kMI32DeviceType0[] PROGMEM = "Unknown";
const char kMI32DeviceType1[] PROGMEM = "Flora";
const char kMI32DeviceType2[] PROGMEM = "MJ_HT_V1";
const char kMI32DeviceType3[] PROGMEM = "LYWSD02";
const char kMI32DeviceType4[] PROGMEM = "LYWSD03";
const char kMI32DeviceType5[] PROGMEM = "CGG1";
const char kMI32DeviceType6[] PROGMEM = "CGD1";
const char kMI32DeviceType7[] PROGMEM = "NLIGHT";
const char kMI32DeviceType8[] PROGMEM = "MJYD2S";
const char kMI32DeviceType9[] PROGMEM = "YEERC";
const char kMI32DeviceType10[] PROGMEM ="MHOC401";
const char kMI32DeviceType11[] PROGMEM ="MHOC303";
const char kMI32DeviceType12[] PROGMEM ="ATC";
const char kMI32DeviceType13[] PROGMEM ="DOOR";
const char kMI32DeviceType14[] PROGMEM ="MISCALEV1";
const char kMI32DeviceType15[] PROGMEM ="MISCALEV2";
const char kMI32DeviceType16[] PROGMEM ="CGDK2";
const char kMI32DeviceType17[] PROGMEM ="ATBTN";
const char kMI32DeviceType18[] PROGMEM = "SJWS01LM";
const char kMI32DeviceType19[] PROGMEM = "LYWSD02MMC";
const char kMI32DeviceType20[] PROGMEM = "LYWSD02MMC";
const char kMI32DeviceType21[] PROGMEM = "BTHome";
const char * kMI32DeviceType[] PROGMEM = {kMI32DeviceType0, kMI32DeviceType1, kMI32DeviceType2, kMI32DeviceType3,
  kMI32DeviceType4, kMI32DeviceType5, kMI32DeviceType6, kMI32DeviceType7, kMI32DeviceType8, kMI32DeviceType9,
  kMI32DeviceType10, kMI32DeviceType11, kMI32DeviceType12, kMI32DeviceType13, kMI32DeviceType14, kMI32DeviceType15,
  kMI32DeviceType16, kMI32DeviceType17, kMI32DeviceType18, kMI32DeviceType19, kMI32DeviceType20, kMI32DeviceType21};

typedef int BATREAD_FUNCTION(int slot);
typedef int UNITWRITE_FUNCTION(int slot, int unit);
typedef int TIMEWRITE_FUNCTION(int slot);

int genericOpCompleteFn(BLE_ESP32::generic_sensor_t *pStruct);
int genericBatReadFn(int slot);
int genericUnitWriteFn(int slot, int unit);
int genericTimeWriteFn(int slot);
int MI32scanCompleteCallback(NimBLEScanResults results);

const char LYWSD02_Svc[] PROGMEM =              "EBE0CCB0-7A0A-4B0C-8A1A-6FF2997DA3A6";
const char LYWSD02_BattChar[] PROGMEM =         "EBE0CCC4-7A0A-4B0C-8A1A-6FF2997DA3A6";
const char LYWSD02_UnitChar[] PROGMEM =         "EBE0CCBE-7A0A-4B0C-8A1A-6FF2997DA3A6";
const char LYWSD02_TimeChar[] PROGMEM =         "EBE0CCB7-7A0A-4B0C-8A1A-6FF2997DA3A6";
const char LYWSD02_BattNotifyChar[] PROGMEM =   "EBE0CCC1-7A0A-4B0C-8A1A-6FF2997DA3A6";

const char *LYWSD03_Svc = LYWSD02_Svc;
const char *LYWSD03_BattNotifyChar = LYWSD02_BattNotifyChar;

const char *MHOC303_Svc = LYWSD02_Svc;
const char *MHOC303_UnitChar = LYWSD02_UnitChar;
const char *MHOC303_TimeChar = LYWSD02_TimeChar;

const char *MHOC401_Svc = LYWSD02_Svc;
const char *MHOC401_BattNotifyChar = LYWSD02_BattNotifyChar;

const char CGD1_Svc[] PROGMEM = "180F";
const char CGD1_BattChar[] PROGMEM = "2A19";

const char FLORA_Svc[] PROGMEM =                "00001204-0000-1000-8000-00805F9B34FB";
const char FLORA_BattChar[] PROGMEM =           "00001A02-0000-1000-8000-00805F9B34FB";


const uint8_t ATBTN_Addr[] = { 0xc1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6 }; // All Avago Buttons seem to use same source address

/*********************************************************************************************\
 * enumerations
\*********************************************************************************************/

// types of operation performed, included in context
enum MI32_MI_OP_TYPES {
  OP_TIME_WRITE = 0,
  OP_BATT_READ = 1,
  OP_UNIT_WRITE = 2,
  OP_UNIT_READ = 3,
  OP_UNIT_TOGGLE = 4,
  OP_READ_HT_LY = 5,
};

enum MI32_MI_KEY_REQ {
  KEY_REQUIREMENT_UNKNOWN = 0, // we don't know if a key is needed
  KEY_NOT_REQUIRED = 1, // we got an unencrypted payload
  KEY_REQUIRED_BUT_NOT_FOUND = 2, // we got an encrypted packet, but had not key
  KEY_REQUIRED_AND_FOUND = 3, // we got an encrypted packet, and could decrypt
  KEY_REQUIRED_AND_INVALID = 4, // we got an encrypted packet, and could not decrypt
};

const char kKeyNeeded[] PROGMEM = "Wait|NotKey|NoKey|KeyOk|KeyInv";

/*********************************************************************************************\
 * Classes
\*********************************************************************************************/


// fn type READ_CALLBACK
// NOTE!!!: this callback is called DIRECTLY from the operation task, so be careful about cross-thread access of data
// if is called after read, so that you can do a read/modify/write operation on a characteristic.
int toggleUnit(BLE_ESP32::generic_sensor_t *op){
  uint32_t context = (uint32_t) op->context;
  int opType = context >> 24;
  // we only need to op type
  int devType = (context >> 16) & 0xff;
  int slot = (context) & 0xff;
  switch (opType){
    case OP_UNIT_TOGGLE:{
      uint8_t curUnit = 0;
      if( op->dataRead[0] != 0 && op->dataRead[0] < 101 ){
          curUnit = op->dataRead[0];
      }

      curUnit = curUnit == 0x01?0xFF:0x01;  // C/F
      // copy in ALL of the data, because we don't know how long this is from the existing src code.
      memcpy(op->dataToWrite, op->dataRead, op->readlen);
      op->writelen = op->readlen;
      op->dataToWrite[0] = curUnit;
    } break;
    case OP_UNIT_WRITE:{
      uint8_t curUnit = op->dataToWrite[0];
      // copy in ALL of the data, because we don't know how long this is from the existing src code.
      memcpy(op->dataToWrite, op->dataRead, op->readlen);
      op->writelen = op->readlen;
      op->dataToWrite[0] = curUnit;
    } break;
  }
  return 0;
}

const char *MIaddrStr(const uint8_t *addr, int useAlias = 0) {
  static char addrstr[32];
  const char *id = nullptr;
  if (useAlias){
    id = BLE_ESP32::getAlias(addr);
  }
  if (!id || !(*id)){
    id = addrstr;
    BLE_ESP32::dump(addrstr, 13, addr, 6);
  }
  return id;
}

bool MI32Operation(int slot, int optype, const char *svc, const char *charactistic, const char *notifychar = nullptr, const uint8_t *data = nullptr, int datalen = 0, uint8_t *addr = nullptr ) {
  if (!svc || !svc[0]){
    AddLog(LOG_LEVEL_ERROR, PSTR("M32: Op inv svc"));
    return 0;
  }

  BLE_ESP32::generic_sensor_t *op = nullptr;

  // ALWAYS use this function to create a new one.
  int res = BLE_ESP32::newOperation(&op);
  if (!res){
    AddLog(LOG_LEVEL_ERROR,PSTR("M32: Can't get a newOperation"));
    return 0;
  } else {
    AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("M32: Got a newOperation"));
  }

  if (slot >= 0){
    op->addr = NimBLEAddress(MIBLEsensors[slot].MAC,0);
  } else {
    if (!addr){
      AddLog(LOG_LEVEL_ERROR, PSTR("M32: No addr"));
      BLE_ESP32::freeOperation(&op);
      return 0;
    }
    op->addr = NimBLEAddress(addr, 0);
  }

  bool havechar = false;
  op->serviceUUID = NimBLEUUID(svc);

  if (!op->serviceUUID.bitSize()){
    BLE_ESP32::freeOperation(&op);
    AddLog(LOG_LEVEL_ERROR, PSTR("M32: MI Bad service string %s"), svc);
    return 0;
  }

  if (charactistic && charactistic[0]){
    havechar = true;
    op->characteristicUUID = NimBLEUUID(charactistic);
    if (!op->characteristicUUID.bitSize()){
      BLE_ESP32::freeOperation(&op);
      AddLog(LOG_LEVEL_ERROR, PSTR("M32: MI Bad characteristic string %s"), charactistic);
      return 0;
    }
  }
  if (notifychar && notifychar[0]){
    op->notificationCharacteristicUUID = NimBLEUUID(notifychar);
    if (!op->notificationCharacteristicUUID.bitSize()){
      BLE_ESP32::freeOperation(&op);
      AddLog(LOG_LEVEL_ERROR, PSTR("M32: MI Bad notifycharacteristic string %s"), notifychar);
      return 0;
    }
  }

  if (data && datalen) {
    op->writelen = datalen;
    memcpy(op->dataToWrite, data, datalen);
  } else {
    if (!datalen && havechar){
      op->readlen = 1; // if we don't set readlen, then it won't read
    }
  }

  // the only times we intercept between read abnd write
  if ((optype == OP_UNIT_WRITE) || (optype == OP_UNIT_TOGGLE)){
    op->readlen = 1; // if we don't set readlen, then it won't read
    op->readmodifywritecallback = (void *)toggleUnit;
  }

  // this op will call us back on complete or failure.
  op->completecallback = (void *)genericOpCompleteFn;
  uint32_t context = (optype << 24) | (MIBLEsensors[slot].type << 16) | slot;
  op->context = (void *)context;

  AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("M32: MI s:%d op:%s"), slot, BLE_ESP32::BLETriggerResponse(op).c_str());

  res = BLE_ESP32::extQueueOperation(&op);
  if (!res){
    // if it fails to add to the queue, do please delete it
    BLE_ESP32::freeOperation(&op);
    AddLog(LOG_LEVEL_ERROR, PSTR("M32: Failed to queue new operation - deleted"));
  }

  return res;
}

int genericBatReadFn(int slot){
  int res = 0;

  switch(MIBLEsensors[slot].type) {
    // these use notify for battery read, and it comes in the temp packet
    case MI_LYWSD03MMC:
      res = MI32Operation(slot, OP_BATT_READ, LYWSD03_Svc, nullptr, LYWSD03_BattNotifyChar);
      break;
    case MI_MHOC401:
      res = MI32Operation(slot, OP_BATT_READ, MHOC401_Svc, nullptr, MHOC401_BattNotifyChar);
      break;

    // these read a characteristic
    case MI_FLORA:
      res = MI32Operation(slot, OP_BATT_READ, FLORA_Svc, FLORA_BattChar);
      break;
    case MI_LYWSD02:
      res = MI32Operation(slot, OP_BATT_READ, LYWSD02_Svc, LYWSD02_BattChar);
      break;
    case MI_CGD1:
      res = MI32Operation(slot, OP_BATT_READ, CGD1_Svc, CGD1_BattChar);
      break;

// this was for testing only - it does work, but no need to read as we get good bat in advert
//    case MI_MJ_HT_V1:
//      res = MI32Operation(slot, OP_BATT_READ, CGD1_Svc, CGD1_BattChar);
//      break;

    default:
      res = -10; // no need to read
      break;
  }
  if (res > 0){
    AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_INFO], PSTR("M32: Req batt read slot %d type %d queued"), slot, MIBLEsensors[slot].type);
  } else {
    AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_INFO], PSTR("M32: Req batt read slot %d type %d non-queued res %d"), slot, MIBLEsensors[slot].type, res);
  }
  return res;
}

int genericSensorReadFn(int slot, int force){
  int res = 0;
  switch(MIBLEsensors[slot].type) {
/*  seen notify timeout consistently with MI_LYWSD02,
    so although the characteristic seems to exist, it does not work?
    further dev required with sensor to hand.
    case MI_LYWSD02:
      // don't read if key present and we've decoded at least one advert
      if (MIBLEsensors[slot].needkey == KEY_REQUIRED_AND_FOUND) return -2;
      res = MI32Operation(slot, OP_READ_HT_LY, LYWSD02_Svc, nullptr, LYWSD02_BattNotifyChar);
      break;*/
    case MI_LYWSD03MMC:
      // don't read if key present and we've decoded at least one advert
      if ((MIBLEsensors[slot].needkey == KEY_NOT_REQUIRED || MIBLEsensors[slot].needkey == KEY_REQUIRED_AND_FOUND) && !force) return -2;
      res = MI32Operation(slot, OP_READ_HT_LY, LYWSD03_Svc, nullptr, LYWSD03_BattNotifyChar);
      break;
    case MI_LYWSD02MMC:
    case MI_LYWSD02MMC2:
      res = MI32Operation(slot, OP_READ_HT_LY, LYWSD03_Svc, nullptr, LYWSD03_BattNotifyChar);
      break;
    case MI_MHOC401:
      // don't read if key present and we've decoded at least one advert
      if ((MIBLEsensors[slot].needkey == KEY_NOT_REQUIRED || MIBLEsensors[slot].needkey == KEY_REQUIRED_AND_FOUND) && !force) return -2;
      res = MI32Operation(slot, OP_READ_HT_LY, MHOC401_Svc, nullptr, MHOC401_BattNotifyChar);
      break;

    default:
      res = -1;
      break;
  }
  return res;
}

// called once per second
int readOneSensor(){
  if (MI32.sensorreader.active){
    AddLog(LOG_LEVEL_DEBUG, PSTR("M32: readOneSensor - already active reading %d"), MI32.sensorreader.slot-1);
    return 0;
  }

  // loop if the sensor at the slot does not need to be read
  // i.e. drop out of loop when we start a read, or hit the end
  int res = -1;
  do {
    // MI32.sensorreader.slot is reset to zero to trigger a read sequence
    if (MI32.sensorreader.slot >= MIBLEsensors.size()){
      //AddLog(LOG_LEVEL_DEBUG, PSTR("BLE: readOneSensor past end of slots - %d > %d"), MI32.sensorreader.slot, MIBLEsensors.size());
      return 0;
    }

    res = genericSensorReadFn(MI32.sensorreader.slot, 0);
    AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("M32: genericSensorReadFn slot %d res %d"), MI32.sensorreader.slot, res);

    // if this sensor in this slot does not need to be read via notify, just move on top the next one
    if (res < 0){
      MI32.sensorreader.slot++;
    } else {
      break;
    }
  } while (1);

  if (res == 0){
    // can't read at the moment (no operations available?)
    AddLog(LOG_LEVEL_DEBUG, PSTR("M32: readOneSensor no ops available slot %d res %d"), MI32.sensorreader.slot, res);
    return 0;
  }

  // setup next slot to read
  MI32.sensorreader.slot++;
  // and make it wait until the read/notify is complete
  // this is cleared in the response callback.
  MI32.sensorreader.active = 1;
  AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("M32: readOneSensor reading for slot %d res %d"), MI32.sensorreader.slot-1, res);

  // started one
  return 1;
}

// called once per second
int readOneBat(){
  if (MI32.batteryreader.active){
    return 0;
  }

  //MI32.batteryreader.slot is rest to zero to trigger a read...
  if (MI32.batteryreader.slot >= MIBLEsensors.size()){
    return 0;
  }

  int res = genericBatReadFn(MI32.batteryreader.slot);

  // if this sensor in this slot does not support battery read, just move on top the next one
  if (res < 0){
    MI32.batteryreader.slot++;
    if (MI32.batteryreader.slot >= MIBLEsensors.size()){
      AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_INFO], PSTR("M32: Batt loop complete at %d"), MI32.batteryreader.slot);
    }
    return 0;
  }

  if (res == 0){
    // can't read at the moment (no operations available?)
    return 0;
  }

  // setup next slot to read
  MI32.batteryreader.slot++;
  // and make it wait until the read/notify is complete
  // this is cleared in the response callback.
  MI32.batteryreader.active = 1;
  if (MI32.batteryreader.slot >= MIBLEsensors.size()){
    AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_INFO], PSTR("M32: Batt loop will complete at %d"), MI32.batteryreader.slot);
  }
  // started one
  return 1;
}

/////////////////////////////////////////////////////
// change the unit of measurement?
// call with unit == -1 to cause the unit to be toggled.
int genericUnitWriteFn(int slot, int unit){
  int res = 0;
  int op = OP_UNIT_WRITE;
  if (unit == -1){
    op = OP_UNIT_TOGGLE;
  }
  uint8_t writeData[1];
  writeData[0] = unit;
  switch (MIBLEsensors[slot].type){
    case MI_LYWSD02:
      res = MI32Operation(slot, op, LYWSD02_Svc, LYWSD02_UnitChar, nullptr, writeData, 1);
      break;
    case MI_MHOC303: // actually, EXACTLY the same as above, including the sevice and characteristic...
      res = MI32Operation(slot, op, MHOC303_Svc, MHOC303_UnitChar, nullptr, writeData, 1);
      break;
    default:
      res = -1;
      break;
  }
  return res;
}

/////////////////////////////////////////////////////
// read the unit of measurement.  genericOpCompleteFn
int genericUnitReadFn(int slot){
  int res = 0;
  switch (MIBLEsensors[slot].type){
    case MI_LYWSD02:
      res = MI32Operation(slot, OP_UNIT_READ, LYWSD02_Svc, LYWSD02_UnitChar);
      break;
    case MI_MHOC303: // actually, EXACTLY the same as above, including the sevice and characteristic...
      res = MI32Operation(slot, OP_UNIT_READ, MHOC303_Svc, MHOC303_UnitChar);
      break;
    default:
      res = -1;
      break;
  }
  return res;
}

/////////////////////////////////////////////////////
// write time to a device. genericOpCompleteFn
int genericTimeWriteFn(int slot){
  int res = 0;
  switch (MIBLEsensors[slot].type){
    case MI_LYWSD02MMC:
    case MI_LYWSD02MMC2:
    case MI_LYWSD02: {
      union {
        uint8_t buf[5];
        uint32_t time;
      } _utc;
      _utc.time = Rtc.utc_time;
      _utc.buf[4] = Rtc.time_timezone / 60;
      res = MI32Operation(slot, OP_TIME_WRITE, LYWSD02_Svc, LYWSD02_TimeChar, nullptr, _utc.buf, sizeof(_utc.buf));
    } break;
    case MI_MHOC303: // actually, EXACTLY the same as above, including the sevice and characteristic...
      union {
        uint8_t buf[5];
        uint32_t time;
      } _utc;
      _utc.time = Rtc.utc_time;
      _utc.buf[4] = Rtc.time_timezone / 60;
      res = MI32Operation(slot, OP_TIME_WRITE, MHOC303_Svc, MHOC303_TimeChar, nullptr, _utc.buf, sizeof(_utc.buf));
      break;
    default:
      res = -1;
      break;
  }
  return res;
}

int genericOpCompleteFn(BLE_ESP32::generic_sensor_t *op){
  uint32_t context = (uint32_t) op->context;

  AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("M32: MI op complete context %x"), context);

  int opType = context >> 24;
  int devType = (context >> 16) & 0xff;
  int slot = (context) & 0xff;

  char slotMAC[13];
  BLE_ESP32::dump(slotMAC, sizeof(slotMAC), MIBLEsensors[slot].MAC, 6) ;
  uint8_t addrrev[6];
  memcpy(addrrev, MIBLEsensors[slot].MAC, 6);
  //BLE_ESP32::ReverseMAC(addrrev);
  NimBLEAddress addr(addrrev, 0);

  bool fail = false;
  if (op->addr != addr){
    // slot changed during operation?
    AddLog(LOG_LEVEL_ERROR, PSTR("M32: Slot mac changed during an operation"));
    fail = true;
  }

  if (op->state <= GEN_STATE_FAILED){
    AddLog(LOG_LEVEL_ERROR, PSTR("M32: %s: Operation failed %d"), slotMAC, op->state);
    fail = true;
  }

  if (fail){
    switch(opType){
      case OP_BATT_READ:{
        // allow another...
        MI32.batteryreader.active = 0;
      } break;
      case OP_READ_HT_LY: {
        // allow another...
        MI32.sensorreader.active = 0;
      } break;
    }
    return 0;
  }

  switch(opType){
    case OP_TIME_WRITE:
      AddLog(LOG_LEVEL_DEBUG, PSTR("M32: %s: Time write complete"), slotMAC);
      return 0; // nothing to do
    case OP_BATT_READ:{
      uint8_t *data = nullptr;
      int len = 0;
      if (op->notifylen){
        data = op->dataNotify;
        len = op->notifylen;
        // note: the only thingas that have battery in notify FOR THE MOMENT read it like this.
        MI32notifyHT_LY(slot, (char*)op->dataNotify, op->notifylen);
      }
      if (op->readlen){
        data = op->dataRead;
        len = op->readlen;
        MIParseBatt(slot, data, len);
      }

      // allow another...
      MI32.batteryreader.active = 0;
      AddLog(LOG_LEVEL_INFO, PSTR("M32: %s: Batt read slot %d done state %x"), slotMAC, slot, op->state);

    } return 0;

    case OP_UNIT_WRITE: // nothing more to do?
      AddLog(LOG_LEVEL_DEBUG, PSTR("M32: %s: Unit write complete"), slotMAC);
      return 0;

    case OP_UNIT_READ: {
      uint8_t currUnit = op->dataRead[0];
      AddLog(LOG_LEVEL_DEBUG, PSTR("M32: %s: Unit read complete %d"), slotMAC, currUnit);
    } return 0;

    case OP_UNIT_TOGGLE: {
      uint8_t currUnit = op->dataToWrite[0];
      AddLog(LOG_LEVEL_DEBUG, PSTR("M32: %s: Unit toggle complete %d->%d; datasize was %d"), slotMAC, op->dataRead[0], op->dataToWrite[0], op->readlen);
    } return 0;

    case OP_READ_HT_LY: {
      // allow another...
      MI32.sensorreader.active = 0;
      MI32notifyHT_LY(slot, (char*)op->dataNotify, op->notifylen);
      AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("M32: %s: HT_LY notify complete"), slotMAC);
    } return 0;

    default:
      AddLog(LOG_LEVEL_ERROR, PSTR("M32: %s: OpType %d not recognised?"), slotMAC, opType);
      return 0;
  }

  return 0;
}

int MI32advertismentCallback(BLE_ESP32::ble_advertisment_t *pStruct)
{
  // we will try not to use this...
  const BLEAdvertisedDevice *advertisedDevice = pStruct->advertisedDevice;

  // AddLog(LOG_LEVEL_DEBUG, PSTR("M32: Advertised Device: %s Buffer: %u"),advertisedDevice->getAddress().toString().c_str(),advertisedDevice->getServiceData(0).length());
  int RSSI = pStruct->RSSI;
  const uint8_t *addr = pStruct->addr;
  if(MI32isInBlockList(addr) == true) return 0;
  if (MI32.option.onlyAliased){
    const char *alias = BLE_ESP32::getAlias(addr);
    if (!alias || !(*alias)){
      return 0;
    }
  }

  // ATBTN uses manufacturer data and not a service - bit more like an IBeacon
  if (!memcmp(ATBTN_Addr, addr, 6))
  {
    //AddLog(LOG_LEVEL_INFO, PSTR("M32: AvagoBtn: %s"), advertisedDevice->toString().c_str());
    MI32ParseATBtn((uint8_t*)advertisedDevice->getManufacturerData().data(), advertisedDevice->getManufacturerData().length(), addr, RSSI);
    return 0;
  }

  int svcdataCount = advertisedDevice->getServiceDataCount();

  if (svcdataCount == 0) {
    return 0;
  }

  NimBLEUUID UUIDBig = advertisedDevice->getServiceDataUUID(0);//.getNative()->u16.value;

  const ble_uuid_t* native = UUIDBig.getBase();
  if (native->type != 16){
    //not interested in 128 bit;
    return 0;
  }
  uint16_t UUID = *(uint16_t*)UUIDBig.getValue();

  AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG_MORE], PSTR("M32: %s: svc[0] UUID (%x)"), MIaddrStr(addr), UUID);
  std::string ServiceDataStr = advertisedDevice->getServiceData(0);

  uint32_t  ServiceDataLength = ServiceDataStr.length();
  const uint8_t *ServiceData = (const uint8_t *)ServiceDataStr.data();
  char temp[60];
  BLE_ESP32::dump(temp, 60, ServiceData, ServiceDataLength);
  AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG_MORE], PSTR("M32: %s: SrvData %s"), MIaddrStr(addr), temp);

  if (UUID){
    // this will take and keep the mutex until the function is over
    TasAutoMutex localmutex(&slotmutex, "Mi32AdCB2");
    switch(UUID){
      case 0xfe95: // std MI?
      {
          MI32ParseResponse(ServiceData, ServiceDataLength, addr, RSSI);
      } break;
      case 0xfdcd: // CGD1 & CGDK2
      {
        if (ServiceDataLength == 17){ // CGDK2
          MI32ParseCGDK2Packet(ServiceData, ServiceDataLength, addr, RSSI);
        } else {
          MI32ParseResponse(ServiceData, ServiceDataLength, addr, RSSI);
        }
      } break;
      case 0x181a: { //ATC
        MI32ParseATCPacket(ServiceData, ServiceDataLength, addr, RSSI);
      } break;
      case 0x181d: // Mi Scale V1
      case 0x181b: // Mi Scale V2
      {
        MI32ParseMiScalePacket(ServiceData, ServiceDataLength, addr, RSSI, UUID);
      } break;
      case 0xFCD2: // BTHome v2
      {
        MI32ParseBTHomePacket(ServiceData, ServiceDataLength, addr, RSSI);
      } break;

      default:{
      } break;
    }
  }
  return 0;
}


/*********************************************************************************************\
 * Helper functions
\*********************************************************************************************/

/**
 * @brief Remove all colons from null terminated char array
 *
 * @param _string Typically representing a MAC-address like AA:BB:CC:DD:EE:FF
 */
void MI32stripColon(char* _string){
  uint32_t _length = strlen(_string);
  uint32_t _index = 0;
  while (_index < _length) {
    char c = _string[_index];
    if(c==':'){
      memmove(_string+_index,_string+_index+1,_length-_index);
    }
    _index++;
  }
  _string[_index] = 0;
}

/**
 * @brief Convert string that repesents a hexadecimal number to a byte array
 *
 * @param _string input string in format: AABBCCDDEEFF or AA:BB:CC:DD:EE:FF, caseinsensitive
 * @param _mac  target byte array must match the correct size (i.e. AA:BB -> uint8_t bytes[2])
 */

void MI32HexStringToBytes(char* _string, uint8_t* _byteArray) {
  MI32stripColon(_string);
  UpperCase(_string,_string);
  uint32_t index = 0;
  uint32_t _end = strlen(_string);
  memset(_byteArray,0,_end/2);
  while (index < _end) {
      char c = _string[index];
      uint8_t value = 0;
      if(c >= '0' && c <= '9')
        value = (c - '0');
      else if (c >= 'A' && c <= 'F')
        value = (10 + (c - 'A'));
      _byteArray[(index/2)] += value << (((index + 1) % 2) * 4);
      index++;
  }
}

/**
 * @brief Reverse an array of 6 bytes
 *
 * @param _mac a byte array of size 6 (typicalliy representing a MAC address)
 */
void MI32_ReverseMAC(uint8_t _mac[]){
  uint8_t _reversedMAC[6];
  for (uint8_t i=0; i<6; i++){
    _reversedMAC[5-i] = _mac[i];
  }
  memcpy(_mac,_reversedMAC, sizeof(_reversedMAC));
}

#ifdef USE_MI_DECRYPTION
int MI32AddKey(char* payload, char* key = nullptr){
  mi_bindKey_t keyMAC;

  if (!key){
    MI32HexStringToBytes(payload,keyMAC.buf);
  } else {
    MI32HexStringToBytes(payload,keyMAC.MAC);
    MI32HexStringToBytes(key,keyMAC.key);
  }

  bool unknownKey = true;
  for(uint32_t i = 0; i < MIBLEbindKeys.size(); i++){
    if(!memcmp(keyMAC.MAC, MIBLEbindKeys[i].MAC, sizeof(keyMAC.MAC))){
      AddLog(LOG_LEVEL_DEBUG, PSTR("M32: Known key"));
      memcpy(MIBLEbindKeys[i].key, keyMAC.key, 16);
      unknownKey=false;
      return 1;
    }
  }
  if(unknownKey){
    AddLog(LOG_LEVEL_DEBUG, PSTR("M32: New key"));
    MIBLEbindKeys.push_back(keyMAC);
    return 1;
  }
  return 0;
}

int MIDecryptPayload(const uint8_t *macin, const uint8_t *nonce, uint32_t tag, uint8_t *data, int len){
  uint8_t payload[len +2];
  uint8_t mac[6];
  memcpy(mac, macin, 6);
  MI32_ReverseMAC(mac);
  uint8_t _bindkey[32] = {0x0};
  const unsigned char authData[16] = {0x11};
  bool foundNoKey = true;
  AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("M32: %s: Searching key"), MIaddrStr(mac));
  for(uint32_t i = 0; i < MIBLEbindKeys.size(); i++){
    if(!memcmp(mac, MIBLEbindKeys[i].MAC, 6)){
      memcpy(_bindkey, MIBLEbindKeys[i].key, 16);
      AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("M32: %s: Decryption key found"), MIaddrStr(mac));
      foundNoKey = false;
      break;
    }
  }
  if(foundNoKey){
    AddLog(LOG_LEVEL_DEBUG, PSTR("M32: %s: No key found"), MIaddrStr(mac));
    return -2; // indicates needs key
  }

  br_aes_small_ctrcbc_keys keyCtx;
  br_aes_small_ctrcbc_init(&keyCtx, _bindkey, 16);

  br_ccm_context ctx;
  br_ccm_init(&ctx, &keyCtx.vtable);
  uint16_t devicetype = *(uint16_t*)(nonce +6);
  if (0xFCD2 == devicetype) {  // BTHome
    br_ccm_reset(&ctx, nonce, 13, 0, len, 4);
  } else {  
    br_ccm_reset(&ctx, nonce, 12, 1, len, 4);
    br_ccm_aad_inject(&ctx, authData, 1);
  }
  br_ccm_flip(&ctx);

  memcpy(payload, data, len); //we want to be sure about 4-byte alignement
  br_ccm_run(&ctx, 0, payload, len);
  memcpy(data, payload, len); //back to the packet

  // crashed in here - why?, so give it more space to work with?
  // returns 1 if matched, else 0
  int ret = br_ccm_check_tag(&ctx, &tag) - 1;

  AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("M32: %s: %sDecrypted %*_H"), MIaddrStr(mac), ret ? PSTR("ERROR ") : PSTR(""), len, payload);
  return ret; // -> -1=fail, 0=success
}

#endif // USE_MI_DECRYPTION

// packet examples:
// MJ_HT_V1
// 5020 AA01 41 3AF4DAA8654C 0A100109
// 5020 AA01 43 3AF4DAA8654C 061002E901
// 5020 AA01 48 3AF4DAA8654C 041002BF00
// 5020 AA01 4A 3AF4DAA8654C 0D1004BF00E901
// 7122 AA01 15 3AF4DAA8654C 0D 0200020D10

// LYWSD03 encrypted data:
// 5858 5B05 2F B3E30838C1A4 [69A9FBDF67] ,060000 0791C39A - 23bytes
// 23-9 = 14
// -> nonce B3E30838C1A4|5B02|2F|060000
// 23-6 = 17
// -> tag 0791C39A
// datalen = 23 - 9 - 4 - 3 - 1 - 1 = 5

// CGD1 reconstructed from src: (svcdata on fdcd)
// xxyy FFEEDDCCBBAA MMMM TTTTHHHH|BB
// xxyy FFEEDDCCBBAA 0104 TTTTHHHH
// xxyy FFEEDDCCBBAA 0201 BB

int MIParsePacket(const uint8_t* slotmac, struct mi_beacon_data_t *parsed, const uint8_t *datain, int len){
  uint8_t data[32];
  memcpy(data, datain, len);
  if (!parsed){
    return 0;
  }
  if (len < 5){
    return 0;
  }

  int byteindex = 0;

  // 58 58 = 0x5858 = data|comp|mac|enc, v5|auth2
  // 30 58 = 0x5830 = comp|mac, v5|auth2
  // 30 50 = 0x5030 = comp|mac, v5|auth0
  // 48 59 = 0x5948 = data|enc, v5|auth2|registered
  // 10 59 = 0x5910 = mac, v5|auth2|registered
  // 71 22 = 0x2271 = data|comp|mac v2|bind
  // 50 20 = 0x2050 = data|mac v2 - MJ_HT_V1 data
  // 71 22 = 0x2271 = data|comp|mac|reserved1 v2|bind - MJ_HT_V1 pair

  // data from byte 0 - e.g. 30
  parsed->framedata.meshflag  = (data[byteindex] & 0x80)>>7; //Byte 0: x.......
  parsed->framedata.dataflag  = (data[byteindex] & 0x40)>>6; //Byte 0: .x......
  parsed->framedata.compatibilityflag   = (data[byteindex] & 0x20)>>5; //Byte 0: ..x..... - indicates compatibility data present
  parsed->framedata.MACFlag             = (data[byteindex] & 0x10)>>4; //Byte 0: ...x....
  parsed->framedata.isencrypted         = (data[byteindex] & 0x08)>>3; //Byte 0: ....x...
  parsed->framedata.reserved            = (data[byteindex] & 0x03)>>6; //Byte 0: .....xxx

  // data from byte 1 - e.g. 58
  byteindex++;
  parsed->framedata.version             = (data[byteindex] & 0xf0)>>4; //Byte 0: xxxx....
  parsed->framedata.authMode            = (data[byteindex] & 0x0C)>>6; //Byte 0: ....xx.. // e.g. 2
  parsed->framedata.bindingvalidreq     = (data[byteindex] & 0x02)>>1; //Byte 0: ......x.
  parsed->framedata.registeredflag      = (data[byteindex] & 0x01);    //Byte 0: .......x

  // note:
  // if bindingvalidreq, we should connect and establish a key.
  // However, how do we determine WHICH TAS should do this?

  byteindex++;

  parsed->devicetype = *((uint16_t *)(data + byteindex));
  byteindex += 2;
  parsed->framecnt = data[byteindex];
  //AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("M32: MI frame %d"), parsed->framecnt);
  byteindex++;


  if (parsed->framedata.version <= 3){
    // e.g. MJ_HT_V1
  }

  if (parsed->framedata.MACFlag){
    if (len < byteindex + 6){
      return 0;
    }
    memcpy(parsed->macdata.mac, &data[byteindex], 6);
    byteindex += 6;
  } else {
    // Use slotmac if no MAC in frame
    memcpy(parsed->macdata.mac, slotmac, 6);
    MI32_ReverseMAC(parsed->macdata.mac);
  }

  int decres = 1;
  // everything after MAC is encrypted if specified?
  if (parsed->framedata.isencrypted){
    if (len < byteindex + 3+4+1){
      return 0;
    }
    const uint8_t* mac = parsed->macdata.mac;
    uint8_t nonce[12];
    uint8_t *p = nonce;
    memcpy(p, mac, 6);
    p += 6;
    memcpy(p, &parsed->devicetype, 2);
    p += 2;
    *(p++) = parsed->framecnt;
    uint8_t *extCnt = data +(len-7);
    memcpy(p, extCnt, 3);
    p += 3;
    uint32_t tag = *(uint32_t *)(data + (len-4));

    // decrypt the data in place
    decres = MIDecryptPayload(mac, nonce, tag, data + byteindex, len - byteindex - 7);
    // no longer need the nonce data.
    len -= 7;
  }

  switch(decres){
    case 1: // decrypt not requested
      break;
    case 0: // suceeded
      parsed->needkey = KEY_REQUIRED_AND_FOUND;
      AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG],PSTR("M32: %s: Payload decrypted"), MIaddrStr(slotmac));
      break;
    case -1: // key failed to work
      parsed->needkey = KEY_REQUIRED_AND_INVALID;
      AddLog(LOG_LEVEL_ERROR,PSTR("M32: %s: Payload decrypt failed"), MIaddrStr(slotmac));
      parsed->payloadpresent = 0;
      return 0;
      break;
    case -2: // key not present
      parsed->needkey = KEY_REQUIRED_BUT_NOT_FOUND;
      AddLog(LOG_LEVEL_ERROR,PSTR("M32: %s: Payload encrypted but no key"), MIaddrStr(slotmac));
      parsed->payloadpresent = 0;
      return 0;
      break;
  }

  // if set, there could be 1 or 3 bytes here.
  if (parsed->framedata.compatibilityflag) {
    if (len < byteindex + 1){
      return 0;
    }
    // e.g. in pair: 7122 AA01 15 3AF4DAA8654C [0D] 0200020D10 -> bond|unused2
    parsed->compatibility.reserved      = (data[byteindex] & 0xc0) >> 6;             //Byte 0: xx......
    parsed->compatibility.IOcap         = (data[byteindex] & 0x20) >> 5;             //Byte 0: ..x.....
    parsed->compatibility.bondability   = (data[byteindex] & 0x18) >> 3;             //Byte 0: ...xx...
    parsed->compatibility.unused        = (data[byteindex] & 0x07) >> 0;             //Byte 0: .....xxx
    byteindex ++;

    if (parsed->compatibility.IOcap) {
      if (len < byteindex + 2){
        return 0;
      }
      parsed->compatibility.IOCapability = *((uint16_t *)(data + byteindex));           // bytes 1-2, e.g. 01 00 -> 0001
      byteindex += 2;
    }
  }

  // rest is payload
  int rem = (len - byteindex);
  if (rem > sizeof(parsed->payload)){
    rem = sizeof(parsed->payload);
    return 0;
  }

  if ((len - byteindex) == 0){
    AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG],PSTR("M32: %s: No payload"), MIaddrStr(slotmac));
    parsed->payload.size = 0;
    parsed->payloadpresent = 0;
    return 0;
  }

  // we have payload which did not need decrypt.
  if (decres == 1){
    parsed->needkey = KEY_NOT_REQUIRED;
    AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG],PSTR("M32: %s: Payload unencrypted"), MIaddrStr(slotmac));
  }

  // already decrypted if required
  parsed->payloadpresent = 1;
  memcpy(&parsed->payload, (data + byteindex), (len - byteindex));
  if (parsed->payload.size != (len - byteindex) - 3){
    AddLog(LOG_LEVEL_DEBUG,PSTR("M32: %s: Payload length mismatch"), MIaddrStr(slotmac));
  }

  return 1;
}

/*********************************************************************************************\
 * common functions
\*********************************************************************************************/

/**
 * @brief Return the slot number of a known sensor or return create new sensor slot
 *
 * @param _MAC     BLE address of the sensor
 * @param _type       Type number of the sensor
 * @param counter     sequence number of broadcast - same for duplicates
 * @param ignoreDulicates  (1) ignore if counter matches lastCnt and previous broardcasts, (2) ignore counter
 * @return uint32_t   Known or new slot in the sensors-vector
 */
uint32_t MIBLEgetSensorSlot(const uint8_t *mac, uint16_t _type, uint8_t counter, uint8_t ignoreDuplicate = 0){

  //AddLog(LOG_LEVEL_DEBUG, PSTR("M32: Will test ID-type: %x"), _type);
  bool _success = false;
  for (uint32_t i=0; i < MI_MI32_TYPES; i++){ // i < sizeof(kMI32DeviceID) gives compiler warning
    if(_type == kMI32DeviceID[i]){
      _type = i+1;
      _success = true;
      break;
    }
/*
    else {
      AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: ID-type is not: %x"), kMI32DeviceID[i]);
    }
*/
  }
  if(!_success) {
    _type = 1; // unknown
  }

  //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: Vector size %u"), MIBLEsensors.size());
  for(uint32_t i=0; i<MIBLEsensors.size(); i++){
    if(!memcmp(mac, MIBLEsensors[i].MAC, 6)){
      // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Counters: %x %x"),MIBLEsensors[i].lastCnt, counter);
      if (2 == ignoreDuplicate) return i; // registered before so return slot
      if(MIBLEsensors[i].lastCnt==counter) {
        // AddLog(LOG_LEVEL_DEBUG,PSTR("Old packet"));
        AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG_MORE], PSTR("M32: %s: Slot %u/[0-%u] - ign repeat"), MIaddrStr(mac), i, MIBLEsensors.size() - 1);
        if (1 == ignoreDuplicate) return 0xff; // packet received before, stop here
      }
      AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("M32: %s: Frame %d, last %d"), MIaddrStr(mac), counter, MIBLEsensors[i].lastCnt);
      MIBLEsensors[i].lastCnt = counter;
      AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG_MORE], PSTR("M32: %s: Slot %u/[0-%u]"), MIaddrStr(mac), i, MIBLEsensors.size() - 1);

      if (MIBLEsensors[i].type != _type){
        // this happens on incorrectly configured pvvx ATC firmware
        AddLog(LOG_LEVEL_ERROR, PSTR("M32: %s: Slot %u - device type 0x%04x(%s) -> 0x%04x(%s) - check device is only sending one type of advert."), MIaddrStr(mac), i,
          kMI32DeviceID[MIBLEsensors[i].type-1], kMI32DeviceType[MIBLEsensors[i].type-1], kMI32DeviceID[_type-1], kMI32DeviceType[_type-1]);
        MIBLEsensors[i].type = _type;
      }

      return i;
    }
    //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: i: %x %x %x %x %x %x"), MIBLEsensors[i].MAC[5], MIBLEsensors[i].MAC[4], MIBLEsensors[i].MAC[3], MIBLEsensors[i].MAC[2], MIBLEsensors[i].MAC[1], MIBLEsensors[i].MAC[0]);
    //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: n: %x %x %x %x %x %x"), mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);
  }
  //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: new sensor -> slot %u"), MIBLEsensors.size());
  //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: found new sensor"));
  mi_sensor_t _newSensor;
  memset(&_newSensor, 0 , sizeof(_newSensor));
  memcpy(_newSensor.MAC, mac, 6);
  _newSensor.type = _type;
  _newSensor.eventType.raw = 0;
  _newSensor.feature.raw = 0;
  _newSensor.temp = NAN;
  _newSensor.needkey = KEY_REQUIREMENT_UNKNOWN;
  _newSensor.bat = 0x00;
  _newSensor.RSSI = 0xffff;
  _newSensor.lux = 0x00ffffff;
  _newSensor.light = -1;
  _newSensor.Btn = -1;
  _newSensor.flooding = -1;
  _newSensor.lastCnt = counter;
  switch (_type)
    {
    case MI_FLORA:
      _newSensor.moisture =0xff;
      _newSensor.fertility =0xffff;
      _newSensor.firmware[0]='\0';
      _newSensor.feature.temp=1;
      _newSensor.feature.moist=1;
      _newSensor.feature.fert=1;
      _newSensor.feature.lux=1;
      _newSensor.feature.bat=1;
      break;
    case MI_NLIGHT:
      _newSensor.events=0x00;
      _newSensor.feature.PIR=1;
      _newSensor.feature.NMT=1;
      _newSensor.feature.events=1;
      break;
    case MI_MJYD2S:
      _newSensor.NMT=0;
      _newSensor.events=0x00;
      _newSensor.feature.PIR=1;
      _newSensor.feature.NMT=1;
      _newSensor.feature.lux=1;
      _newSensor.feature.bat=1;
      _newSensor.feature.events=1;
      break;
    case MI_YEERC:
      _newSensor.feature.Btn=1;
      break;
    case MI_DOOR: // MCCGQ02HL
      _newSensor.feature.Btn=1;
      _newSensor.feature.light=1;
      _newSensor.feature.bat=1;
      break;
    case MI_SCALE_V1:
      _newSensor.feature.scale=1;
      break;
    case MI_SCALE_V2:
      _newSensor.feature.scale=1;
      _newSensor.feature.impedance=1;
      break;
    case AT_BTN:
      _newSensor.feature.Btn=1;
      _newSensor.needkey = KEY_NOT_REQUIRED;
      break;
    case MI_SJWS01LM:
      _newSensor.feature.flooding = 1;
      _newSensor.feature.Btn = 1;
      _newSensor.feature.bat = 1;
      break;
    case MI_BTHOME:
      // BTHome sensors announce their own features dynamically via object IDs.
      // Initialise with no features — they are set when the first packet is parsed.
      _newSensor.hum = NAN;
      _newSensor.needkey = KEY_NOT_REQUIRED;
      _newSensor.Btn = 0;
      break;

    default:
      _newSensor.hum=NAN;
      _newSensor.feature.temp=1;
      _newSensor.feature.hum=1;
      _newSensor.feature.tempHum=1;
      _newSensor.feature.bat=1;
      break;
    }
  MIBLEsensors.push_back(_newSensor);
  AddLog(LOG_LEVEL_DEBUG, PSTR("M32: %s: New %s at slot %u"), MIaddrStr(mac), kMI32DeviceType[_type-1], MIBLEsensors.size() - 1);
  MI32.mode.shallShowStatusInfo = 1;
  return MIBLEsensors.size()-1;
};

/**
 * @brief trigger real-time message for PIR or RC
 *
 */
void MI32triggerTele(void){
  MI32.mode.triggeredTele = 1;
  MI32ShowTriggeredSensors();
  MI32.mode.triggeredTele = 0;
}

/**
 * @brief Is called after every finding of new BLE sensor
 *
 */
void MI32StatusInfo() {
  MI32.mode.shallShowStatusInfo = 0;
  Response_P(PSTR("{\"%s\":{\"found\":%u}}"), D_CMND_MI32, MIBLEsensors.size());
  XdrvRulesProcess(0);
}

/*********************************************************************************************\
 * BLE callbacks section
 * These are called from main thread only.
\*********************************************************************************************/

int MI32scanCompleteCallback(NimBLEScanResults results){
  // we actually don't need to do anything here....
  AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG],PSTR("M32: Scan complete"));
  return 0;
}

/*********************************************************************************************\
 * init BLE_32
\*********************************************************************************************/

void MI32Init(void) {
  MIBLEsensors.reserve(10);
  MIBLEbindKeys.reserve(10);
  MI32.mode.init = false;

  //test section for options
  MI32.option.allwaysAggregate = MI32_OPTION0_ALWAYS_AGGREGATE;       // Mi32Option0: always show all known values of one sensor in bridge mode
  MI32.option.noSummary = MI32_OPTION1_NO_SUMMARY;                    // Mi32Option1: no sensor values at TELE-period
  MI32.option.directBridgeMode = MI32_OPTION2_DIRECT_BRIDGED_MODE;    // Mi32Option2: send every received BLE-packet as a MQTT-message in real-time
  MI32.option.holdBackFirstAutodiscovery = 1;
  MI32.option.showRSSI = 1;
  MI32.option.ignoreBogusBattery = MI32_OPTION4_IGNORE_BOGUS_BATTERY; // Mi32Option4: ignore battery from ads
  MI32.option.minimalSummary = 0;
  MI32.option.onlyAliased = MI32_OPTION5_ONLY_ALIASED;                // Mi32Option5: only include sensors that are aliased
  MI32.option.MQTTType = MI32_OPTION6_MQTT_TYPE;                      // Mi32Option6: publish sensor on MI32.bleTopic with 1 topic per sensor

  AddLog(LOG_LEVEL_DEBUG, PSTR("M32: MI32Option0:%d, 1:%d, 2:%d, 4:%d, 5:%d, 6:%d"),
    MI32.option.allwaysAggregate, MI32.option.noSummary, MI32.option.directBridgeMode,
    MI32.option.ignoreBogusBattery, MI32.option.onlyAliased, MI32.option.MQTTType);

  BLE_ESP32::registerForAdvertismentCallbacks((const char *)"MI32", MI32advertismentCallback);
  BLE_ESP32::registerForScanCallbacks((const char *)"MI32", MI32scanCompleteCallback);
  // note: for operations, we will set individual callbacks in the operations we request
  //void registerForOpCallbacks(const char *tag, BLE_ESP32::OPCOMPLETE_CALLBACK* pFn);

  AddLog(LOG_LEVEL_INFO,PSTR("M32: init: request callbacks"));
  MI32.period = Settings->tele_period;
  MI32.mode.init = 1;
  return;
}

/*********************************************************************************************\
 * Task section
\*********************************************************************************************/

int MIParseBatt(int slot, uint8_t *data, int len){
  int value = data[0];
  char slotMAC[13];
  BLE_ESP32::dump(slotMAC, sizeof(slotMAC), MIBLEsensors[slot].MAC, 6) ;

  if ((value != 0) && (value < 101)){
    MIBLEsensors[slot].bat = value;
    if(MIBLEsensors[slot].type==MI_FLORA){
      if (len < 7){
        AddLog(LOG_LEVEL_ERROR,PSTR("M32: FLORA: not enough bytes read for firmware?"));
      } else {
        memcpy(MIBLEsensors[slot].firmware, data+2, 5);
        MIBLEsensors[slot].firmware[5] = '\0';
        AddLog(LOG_LEVEL_DEBUG,PSTR("M32: FLORA Firmware: %s"), MIBLEsensors[slot].firmware);
      }
    }
    MIBLEsensors[slot].eventType.bat  = 1;
    MIBLEsensors[slot].shallSendMQTT = 1;
    MI32.mode.shallTriggerTele = 1;
    AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG],PSTR("M32: Batt read for %s complete %d"), slotMAC, value);
  } else {
    AddLog(LOG_LEVEL_ERROR,PSTR("M32: Batt read for %s complete but out of range 1-101 (%d)"), slotMAC, value);
  }

  return 0;
}

/*********************************************************************************************\
 * parse the response from advertisements
\*********************************************************************************************/

void MI32ParseATCPacket(const uint8_t * _buf, uint32_t length, const uint8_t *addr, int RSSI){
  ATCPacket_t *_packet = (ATCPacket_t*)_buf;
  PVVXPacket_t *ppv_packet = (PVVXPacket_t*)_buf;

  if (length == 15){ // 19-1-1-2
    uint8_t addrrev[6];
    memcpy(addrrev, addr, 6);
    MI32_ReverseMAC(addrrev);
    if (!memcmp(addrrev, ppv_packet->MAC, 6)){
      //int16_t		temperature; // x 0.1 degree
      //uint16_t	humidity; // x 0.01 %
      //uint16_t	battery_mv; // mV
      //uint8_t		battery_level; // 0..100 %
      //uint8_t		counter; // measurement count
      //uint8_t		flags;

      uint32_t _slot = MIBLEgetSensorSlot(addr, 0x0a1c, ppv_packet->counter); // This must be a hard-coded fake ID
      if(_slot == 0xff) return;

      if ((_slot >= 0) && (_slot < MIBLEsensors.size())){
        AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG],PSTR("M32: %s: %s:pvvx at slot %u"), MIaddrStr(addr), kMI32DeviceType[MIBLEsensors[_slot].type-1], _slot);
        MIBLEsensors[_slot].RSSI=RSSI;
        MIBLEsensors[_slot].needkey=KEY_NOT_REQUIRED;
        MIBLEsensors[_slot].lastTime = Rtc.local_time;

        MIBLEsensors[_slot].temp = (float)(ppv_packet->temperature)/100.0f;
        MIBLEsensors[_slot].hum = (float)(ppv_packet->humidity)/100.0f;
        MIBLEsensors[_slot].eventType.tempHum  = 1;
        MIBLEsensors[_slot].bat = ppv_packet->battery_level;
        MIBLEsensors[_slot].eventType.bat  = 1;

        MIBLEsensors[_slot].Btn = (ppv_packet->flags) & 0x1; // First bit is reed switch status
        MIBLEsensors[_slot].eventType.Btn = 1;
        MIBLEsensors[_slot].feature.Btn = 1;

        if(MI32.option.directBridgeMode) {
          MIBLEsensors[_slot].shallSendMQTT = 1;
          MI32.mode.shallTriggerTele = 1;
        }
      }
      return;
    } else {
      AddLog(LOG_LEVEL_ERROR, PSTR("M32: PVVX packet mac mismatch - ignored?"));
      return;
    }
  }

  uint8_t addrrev[6];
  memcpy(addrrev, addr, 6);
  //MI32_ReverseMAC(addrrev);

  // if packet tell a different address to origin, use the different address
  if (memcmp(addrrev, _packet->MAC, 6)){
    MI32_ReverseMAC(_packet->MAC);
    if (!memcmp(addrrev, _packet->MAC, 6)){
      AddLog(LOG_LEVEL_ERROR, PSTR("M32: ATC packet with reversed MAC addr?"));
    } else {
      AddLog(LOG_LEVEL_ERROR, PSTR("M32: ATC packet with MAC addr mismatch - is this mesh?"));
      memcpy(addrrev, _packet->MAC, 6);
    }
    addr = addrrev;
  }

  uint32_t _slot = MIBLEgetSensorSlot(addr, 0x0a1c, _packet->frameCnt); // This must be a hard-coded fake ID

  if(_slot == 0xff) return;

  if ((_slot >= 0) && (_slot < MIBLEsensors.size())){
    AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG],PSTR("M32: %s at slot %u"), kMI32DeviceType[MIBLEsensors[_slot].type-1], _slot);
    MIBLEsensors[_slot].RSSI=RSSI;
    MIBLEsensors[_slot].needkey=KEY_NOT_REQUIRED;
    MIBLEsensors[_slot].lastTime = Rtc.local_time;

    MIBLEsensors[_slot].temp = (float)(int16_t(__builtin_bswap16(_packet->temp)))/10.0f;
    MIBLEsensors[_slot].hum = (float)_packet->hum;
    MIBLEsensors[_slot].eventType.tempHum  = 1;
    MIBLEsensors[_slot].bat = _packet->batPer;
    MIBLEsensors[_slot].eventType.bat  = 1;

    if(MI32.option.directBridgeMode) {
      MIBLEsensors[_slot].shallSendMQTT = 1;
      MI32.mode.shallTriggerTele = 1;
    }
  }
}

void MI32ParseCGDK2Packet(const uint8_t * _buf, uint32_t length, const uint8_t *addr, int RSSI){
  CGDK2Packet_t *cgdk_packet = (CGDK2Packet_t*)_buf;

  if (length == 17){ // 
    uint8_t addrrev[6];
    memcpy(addrrev, addr, 6);
    MI32_ReverseMAC(addrrev);

    if (!memcmp(addrrev, cgdk_packet->MAC, 6)){
        uint32_t _slot = MIBLEgetSensorSlot(addr, 0x066f, cgdk_packet->counter); // This must be a hard-coded fake ID
        if(_slot == 0xff) return;

        if ((_slot >= 0) && (_slot < MIBLEsensors.size())){
          AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("M32: %s:pvvx at slot %u"), kMI32DeviceType[MIBLEsensors[_slot].type-1], _slot);
          MIBLEsensors[_slot].RSSI=RSSI;
          MIBLEsensors[_slot].needkey=KEY_NOT_REQUIRED;
          MIBLEsensors[_slot].lastTime = Rtc.local_time;
          MIBLEsensors[_slot].temp = (float)(cgdk_packet->temperature)/10.0f;
          MIBLEsensors[_slot].hum = (float)(cgdk_packet->humidity)/10.0f;
          MIBLEsensors[_slot].eventType.tempHum  = 1;
          MIBLEsensors[_slot].bat = cgdk_packet->battery_level;
          MIBLEsensors[_slot].eventType.bat  = 1;

          if(MI32.option.directBridgeMode) {
            MIBLEsensors[_slot].shallSendMQTT = 1;
            MI32.mode.shallTriggerTele = 1;
          }
        }
        return;
      } else {
        AddLog(LOG_LEVEL_ERROR, PSTR("M32: CGDK2 packet mac mismatch - ignored?"));
        return;
      }
  }

}

void MI32ParseMiScalePacket(const uint8_t * _buf, uint32_t length, const uint8_t *addr, int RSSI, int UUID) {
  MiScaleV1Packet_t *_packetV1 = (MiScaleV1Packet_t*)_buf;
  MiScaleV2Packet_t *_packetV2 = (MiScaleV2Packet_t*)_buf;
  uint8_t weight_stabilized = 0;
  uint8_t weight_removed = 0;
  uint8_t impedance_stabilized = 0;

  // Mi Scale V1
  if (length == 10 && UUID == 0x181d) { // 14-1-1-2
    weight_stabilized = (_packetV1->status & (1 << 5)) ? 1 : 0;
    weight_removed = (_packetV1->status & (1 << 7)) ? 1 : 0;
    if (!MI32.option.directBridgeMode && (!weight_stabilized || weight_removed))
      return;

    uint32_t _slot = MIBLEgetSensorSlot(addr, UUID, 0);
    if (_slot == 0xff) return;

    if ((_slot >= 0) && (_slot < MIBLEsensors.size())) {
      AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("M32: %s: at slot %u"), kMI32DeviceType[MIBLEsensors[_slot].type-1], _slot);
      MIBLEsensors[_slot].RSSI = RSSI;
      MIBLEsensors[_slot].needkey = KEY_NOT_REQUIRED;
      MIBLEsensors[_slot].lastTime = Rtc.local_time;
      MIBLEsensors[_slot].eventType.scale = 1;

      MIBLEsensors[_slot].weight_stabilized = weight_stabilized;
      MIBLEsensors[_slot].weight_removed = weight_removed;

      if (_packetV1->status & (1 << 0)) {
        strlcpy(MIBLEsensors[_slot].weight_unit, PSTR("lbs"), sizeof(MIBLEsensors[_slot].weight_unit));
        MIBLEsensors[_slot].weight = (float)_packetV1->weight / 100.0f;
      } else if(_packetV1->status & (1 << 4)) {
        strlcpy(MIBLEsensors[_slot].weight_unit, PSTR("jin"), sizeof(MIBLEsensors[_slot].weight_unit));
        MIBLEsensors[_slot].weight = (float)_packetV1->weight / 100.0f;
      } else {
        strlcpy(MIBLEsensors[_slot].weight_unit, PSTR("kg"), sizeof(MIBLEsensors[_slot].weight_unit));
        MIBLEsensors[_slot].weight = (float)_packetV1->weight / 200.0f;
      }

      if (MIBLEsensors[_slot].weight_removed) {
        MIBLEsensors[_slot].weight = 0.0f;
      }

      MIBLEsensors[_slot].shallSendMQTT = 1;
      MI32.mode.shallTriggerTele = 1;
    }
  }

  // Mi Scale V2
  else if (length == 13 && UUID == 0x181b) { // 17-1-1-2
    weight_stabilized = (_packetV2->status & (1 << 5)) ? 1 : 0;
    weight_removed = (_packetV2->status & (1 << 7)) ? 1 : 0;
    impedance_stabilized = (_packetV2->status & (1 << 1)) ? 1 : 0;
    if (!MI32.option.directBridgeMode && (!weight_stabilized || weight_removed))
      return;

    uint32_t _slot = MIBLEgetSensorSlot(addr, UUID, 0);
    if (_slot == 0xff) return;

    if ((_slot >= 0) && (_slot < MIBLEsensors.size())) {
      AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG],PSTR("M32: %s: at slot %u"), kMI32DeviceType[MIBLEsensors[_slot].type-1], _slot);
      MIBLEsensors[_slot].RSSI = RSSI;
      MIBLEsensors[_slot].needkey = KEY_NOT_REQUIRED;
      MIBLEsensors[_slot].lastTime = Rtc.local_time;
      MIBLEsensors[_slot].eventType.scale = 1;

      MIBLEsensors[_slot].weight_stabilized = weight_stabilized;
      MIBLEsensors[_slot].weight_removed = weight_removed;

      if (_packetV2->weight_unit & (1 << 4)) {
        strlcpy(MIBLEsensors[_slot].weight_unit, PSTR("jin"), sizeof(MIBLEsensors[_slot].weight_unit));
        MIBLEsensors[_slot].weight = (float)_packetV2->weight / 100.0f;
      } else if(_packetV2->weight_unit == 3) {
        strlcpy(MIBLEsensors[_slot].weight_unit, PSTR("lbs"), sizeof(MIBLEsensors[_slot].weight_unit));
        MIBLEsensors[_slot].weight = (float)_packetV2->weight / 100.0f;
      } else if(_packetV2->weight_unit == 2) {
        strlcpy(MIBLEsensors[_slot].weight_unit, PSTR("kg"), sizeof(MIBLEsensors[_slot].weight_unit));
        MIBLEsensors[_slot].weight = (float)_packetV2->weight / 200.0f;
      } else {
        strlcpy(MIBLEsensors[_slot].weight_unit, PSTR(""), sizeof(MIBLEsensors[_slot].weight_unit));
        MIBLEsensors[_slot].weight = (float)_packetV2->weight / 100.0f;
      }

      if (weight_removed) {
        MIBLEsensors[_slot].weight = 0.0f;
      }

      MIBLEsensors[_slot].impedance = 0;
      if (MI32.option.directBridgeMode || impedance_stabilized)
      {
        MIBLEsensors[_slot].impedance_stabilized = impedance_stabilized;
        MIBLEsensors[_slot].impedance = _packetV2->impedance;

        if (weight_removed) {
          MIBLEsensors[_slot].impedance = 0;
        }
      }

      MIBLEsensors[_slot].shallSendMQTT = 1;
      MI32.mode.shallTriggerTele = 1;
    }
  }
}

/*********************************************************************************************\
 * BTHome v2 protocol support
 * https://bthome.io/
 * BLE Service UUID: 0xFCD2
\*********************************************************************************************/

// BTHome v2 object ID to data-length mapping table (0xFF = sentinel)
struct bthome_obj_def_t { 
  uint8_t obj_id;
  union {
    struct {
      uint8_t length:2;  // 0 = 1, 1 = 2, 2 = 3, 3 = 4
      uint8_t format:2;  // 0 = uint, 1 = int, 2 = command, text or raw
      uint8_t factor:4;  // 0 = 1, 1 = 0.1, 2 = 0.01, 3 = 0.001, 4 = 0.0001, 5 = 0.00001, 6 = 0.000001, 7 = 0.35
    };
    uint8_t raw;
  } data;
};
// BTHome v2 table from https://bthome.io/format/
// and https://github.com/Bluetooth-Devices/bthome-ble/blob/V2/src/bthome_ble/const.py
static const bthome_obj_def_t BTHOME_OBJECTS[] = {
  // Sensors
  {0x00, 0, 0, 0},  // Packet ID uint8
  {0x01, 0, 0, 0},  // Battery (%) uint8
  {0x02, 1, 1, 2},  // Temperature (0.01 °C) int16
  {0x03, 1, 0, 2},  // Humidity (0.01 %) uint16
  {0x04, 2, 0, 2},  // Pressure (0.01 hPa) uint24
  {0x05, 2, 0, 2},  // Illuminance (0.01 lx) uint24
  {0x06, 1, 0, 2},  // Mass kg (0.01 kg) uint16
  {0x07, 1, 0, 2},  // Mass lbs (0.01 lbs) uint16
  {0x08, 1, 1, 2},  // Dewpoint (0.01 °C) int16
  {0x09, 0, 0, 0},  // Count (1) uint8
  {0x0A, 2, 0, 3},  // Energy (0.001 kWh) uint24
  {0x0B, 2, 0, 2},  // Power (0.01 W) uint24
  {0x0C, 1, 0, 3},  // Voltage (0.001 V) uint16
  {0x0D, 1, 0, 0},  // PM2.5 (ug/m3) uint16
  {0x0E, 1, 0, 0},  // PM10 (ug/m3) uint16
  // Binary sensors uint8
  {0x0F, 0, 0, 0},  // Generic boolean (0 = Off, 1 = On)
  {0x10, 0, 0, 0},  // Power (0 = Off, 1 = On)
  {0x11, 0, 0, 0},  // Opening (0 = Closed, 1 = Open)
  // Sensors
  {0x12, 1, 0, 0},  // CO2 (ppm) uint16
  {0x13, 1, 0, 0},  // TVOC (ug/m3) uint16
  {0x14, 1, 0, 2},  // Moisture (0.01 %) uint16
  // Binary sensors uint8
  {0x15, 0, 0, 0},  // Battery (0 = Normal, 1 = Low)
  {0x16, 0, 0, 0},  // Battery charging (0 = Not charging, 1 = Charging)
  {0x17, 0, 0, 0},  // CO (0 = Not detected, 1 = Detected)
  {0x18, 0, 0, 0},  // Cold (0 = Normal, 1 = Cold)
  {0x19, 0, 0, 0},  // Connectivity (0 = Disconnected, 1 = Connected)
  {0x1A, 0, 0, 0},  // Door (0 = Closed, 1 = Open)
  {0x1B, 0, 0, 0},  // Garage door (0 = Closed, 1 = Open)
  {0x1C, 0, 0, 0},  // Gas (0 = Clear, 1 = Detected)
  {0x1D, 0, 0, 0},  // Heat (0 = Normal, 1 = Hot)
  {0x1E, 0, 0, 0},  // Light (0 = No light, 1 = Light detected)
  {0x1F, 0, 0, 0},  // Lock (0 = Locked, 1 = Unlocked)
  {0x20, 0, 0, 0},  // Moisture (0 = Dry, 1 = Wet)
  {0x21, 0, 0, 0},  // Motion (0 = Clear, 1 = Detected)
  {0x22, 0, 0, 0},  // Moving (0 = Not moving, 1 = Moving)
  {0x23, 0, 0, 0},  // Occupancy (0 = Clear, 1 = Detected)
  {0x24, 0, 0, 0},  // Plug (0 = Unplugged, 1 = Plugged)
  {0x25, 0, 0, 0},  // Presence (0 = Away, 1 = Home)
  {0x26, 0, 0, 0},  // Problem (0 = OK, 1 = Problem)
  {0x27, 0, 0, 0},  // Running (0 = Not running, 1 = Running)
  {0x28, 0, 0, 0},  // Safety (0 = Unsafe, 1 = Safe)
  {0x29, 0, 0, 0},  // Smoke (0 = Clear, 1 = Detected)
  {0x2A, 0, 0, 0},  // Sound (0 = Clear, 1 = Detected)
  {0x2B, 0, 0, 0},  // Tamper (0 = Off, 1 = On)
  {0x2C, 0, 0, 0},  // Vibration (0 = Clear, 1 = Detected)
  {0x2D, 0, 0, 0},  // Window (0 = Closed, 1 = Open)
  // Sensors
  {0x2E, 0, 0, 0},  // Humidity (1 deg) uint8
  {0x2F, 0, 0, 0},  // Moisture (1 %) uint8
  {0x3A, 0, 0, 0},  // Event button uint8
  {0x3B, 1, 2, 0},  // Event command 0..2 uint16, 3..4 uint24
  {0x3C, 1, 0, 0},  // Event dimmer uint16
  {0x3D, 1, 0, 0},  // Count (1) uint16
  {0x3E, 3, 0, 0},  // Count (1) uint32
  {0x3F, 1, 1, 1},  // Rotation (0.1 deg) int16
  {0x40, 1, 0, 0},  // Distance (1 mm) uint16
  {0x41, 1, 0, 1},  // Distance (0.1 m) uint16
  {0x42, 2, 0, 3},  // Duration (0.001 s) uint24
  {0x43, 1, 1, 3},  // Current (0.001 A) int16
  {0x44, 1, 0, 2},  // Speed (0.01 m/s) uint16
  {0x45, 1, 1, 1},  // Temperature (0.1 °C) int16
  {0x46, 0, 0, 1},  // UV index (0.1) uint8
  {0x47, 1, 0, 1},  // Volume (0.1 L) uint16
  {0x48, 1, 0, 0},  // Volume (mL) uint16
  {0x49, 1, 0, 3},  // Volume flow rate (0.001 m3/hr) uint16
  {0x4A, 1, 0, 1},  // Voltage (0.1 V) uint16
  {0x4B, 2, 0, 3},  // Gas (0.001 m3) uint24
  {0x4C, 3, 0, 3},  // Gas (0.001 m3) uint32
  {0x4D, 3, 0, 3},  // Energy (0.001 kWh) uint32
  {0x4E, 3, 0, 3},  // Volume (0.001 L) uint32
  {0x4F, 3, 0, 3},  // Volume water (0.001 L) uint32
  {0x50, 3, 0, 0},  // Timestamp uint32
  {0x51, 1, 0, 3},  // Acceleration (0.001 m/s2) uint16
  {0x52, 1, 0, 3},  // Gyroscope (0.001 deg/s) uint16
  {0x53, 0, 2, 0},  // Text
  {0x54, 0, 2, 0},  // Raw
  {0x55, 3, 0, 3},  // Volume storage (0.001 L) uint32
  {0x56, 1, 0, 0},  // Conductivity (1 uS/cm) uint16
  {0x57, 0, 1, 0},  // Temperature (1 °C) int8
  {0x58, 0, 1, 7},  // Temperature (0.35 °C) int8
  {0x59, 0, 1, 0},  // Count (1) int8
  {0x5A, 1, 1, 0},  // Count (1) int16
  {0x5B, 3, 1, 0},  // Count (1) int32
  {0x5C, 3, 1, 2},  // Power (0.01 W) int32
  {0x5D, 1, 1, 3},  // Current (0.001 A) int16
  {0x5E, 1, 0, 3},  // Direction (0.001 deg) uint16
  {0x5F, 1, 0, 1},  // Precipitation (0.1 mm) uint16
  {0x60, 0, 0, 0},  // Channel uint8
  {0x61, 1, 0, 0},  // Rotational speed (1 rpm) uint16
  {0x62, 3, 1, 6},  // Speed (0.000001 m/s) int32
  {0x63, 3, 1, 6},  // Acceleration (0.000001 m/s2) int32
  {0x64, 0, 0, 0},  // Light level (0 = Dark, 1 = Twilight, 2 = Bright) uint8
  {0x65, 0, 0, 0},  // Settings revision uint8
  {0xF0, 1, 0, 0},  // Device type id uint16
  {0xF1, 3, 0, 0},  // Firmware version (F100010204 = 4.2.1.0} uint32
  {0xF2, 2, 0, 0},  // Firmware version (F2000106 = 6.1.0} uint24
  {0xFF, 0, 0, 0}   // sentinel
};

/*-------------------------------------------------------------------------------------------*/

bool BTHomeDecrypt(uint8_t* buf, uint32_t &length, const uint8_t *mac) {
  uint32_t new_encryption_counter = *(uint32_t *)(buf + (length-8));
#ifdef USE_MI_DEBUG 
  AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("BTH: %s: counter %d, Encrypted packet '%*_H'"),
    MIaddrStr(mac), new_encryption_counter, length, buf);
#endif
  uint32_t slot = MIBLEgetSensorSlot(mac, 0xFCD2, 0, 2); // If registered get last encryption counter
  uint32_t last_encryption_counter = MIBLEsensors[slot].encr_cnt;
  // Raises false on decreasing encryption counter to avoid replay attacks
  // Filter advertisements with a decreasing encryption counter.
  // Allow cases where the counter has restarted from 0
  // (after reaching the highest number or due to a battery change).
  // In all other cases, assume the data has been compromised and skip the advertisement.
  // prepare the data for decryption
  if ((new_encryption_counter < last_encryption_counter) &&
      (new_encryption_counter >= 100)) {
#ifdef USE_MI_DEBUG 
    AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("BTH: %s: new encryption counter (%d) is smaller than previous value (%d)"), 
      MIaddrStr(mac), new_encryption_counter, last_encryption_counter);
#endif
    return false;
  }
  MIBLEsensors[slot].encr_cnt = new_encryption_counter;

  uint8_t nonce[13];
  uint8_t *p = nonce;
  memcpy(p, mac, 6);
  p += 6;
  *(p++) = 0xD2;       // BTHome UUID
  *(p++) = 0xFC;       // BTHome UUID
  *(p++) = 0x41;       // BTHome device data byte
  const uint8_t *encryption_counter = buf +(length-8);
  memcpy(p, encryption_counter, 4);

  uint32_t mic = *(uint32_t *)(buf + (length-4));  // Message Integrity Check

  // decrypt the data in place
  int decres = MIDecryptPayload(mac, nonce, mic, buf + 1, length - 9);
  // no longer need the nonce data.
  length -= 8;

//  AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("BTH: %s: decrypted packet %*_H"), MIaddrStr(mac), length, buf);

  switch(decres){
    case 0:  // suceeded
      break;
    case -1: // key failed to work
      AddLog(LOG_LEVEL_ERROR,PSTR("BTH: %s: payload decrypt failed"), MIaddrStr(mac));
      return false;
    case -2: // key not present
      AddLog(LOG_LEVEL_ERROR,PSTR("BTH: %s: payload encrypted but no key"), MIaddrStr(mac));
      return false;
  }

  buf[0] &= 0xFE;  // Reset encrypted flag
  return true;
}

// Returns True, data length, format and factor for a BTHome v2 object ID, or False if unknown
bool BTHomeGetObjectData(uint8_t obj_id, uint32_t &length, uint32_t &format, uint32_t &factor) {
  for (int i = 0; BTHOME_OBJECTS[i].obj_id != 0xFF; i++) {
    if (BTHOME_OBJECTS[i].obj_id == obj_id) {
      length = BTHOME_OBJECTS[i].data.length + 1;
      format = BTHOME_OBJECTS[i].data.format;
      factor = BTHOME_OBJECTS[i].data.factor;
      return true;
    }
  }
  return false;
}

// Returns True, object ID, data length, format and factor, or False if invalid or end-of-packet
bool BTHomeGetObject(const uint8_t * _buf, uint32_t length, uint32_t &idx, uint32_t &obj_id, uint32_t &dlength, uint32_t &dformat, uint32_t &dfactor) {
  if (idx >= length) {
    return false; // End of packet reached
  }

  uint32_t last_obj_id = obj_id;
  obj_id = _buf[idx++];
  if (obj_id < last_obj_id) {
#ifdef USE_MI_DEBUG 
    AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("BTH: Invalid obj order"));
#endif
    return false;
  }

  if (!BTHomeGetObjectData(obj_id, dlength, dformat, dfactor)) {
#ifdef USE_MI_DEBUG 
    AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("BTH: Invalid obj id 0x%02x (%d)"), obj_id, obj_id);
#endif
    return false;
  }

  if ((0x3B == obj_id) || (0x53 == obj_id) || (0x54 == obj_id)) {  // Command, Text or Raw add length
    dlength = _buf[idx++];
    if (0x3B == obj_id) {
      dlength &= 0x1F;  // length byte: high 3 bits reserved, low 5 bits args length
      dlength++;
    }
  }
  if ((idx + (uint32_t)dlength) > length) {
#ifdef USE_MI_DEBUG 
    AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("BTH: Invalid obj size"));
#endif
    return false;
  }
  return true;
}

////////////////////////////////////////////////////////////
// this SHOULD parse any BTHome packet, including encrypted.
void MI32ParseBTHomePacket(const uint8_t * _buf, uint32_t length, const uint8_t *mac, int RSSI) {
  if (length < 1) {
    return;
  }

  char log_name[32];
  ext_snprintf_P(log_name, sizeof(log_name),PSTR("BTH: %s:"), MIaddrStr(mac));

  uint32_t dev_info = _buf[0];
  uint32_t version = (dev_info >> 5) & 0x07;
  // Only BTHome v2 is supported (version field == 2)
  if (version != 2) {
#ifdef USE_MI_DEBUG 
    AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("%s unsupported version %d"), log_name, version);
#endif
    return;
  }

  if (dev_info & 0x01) {  // Encrypted
    if (!BTHomeDecrypt((uint8_t*)_buf, length, mac)) {
      return;
    }
  }

  uint32_t dlength;
  uint32_t dformat;
  uint32_t dfactor;

  // First pass: find optional packet_id (object 0x00) for duplicate detection
  int packet_id = -1;
  uint32_t obj_id = 0;
  uint32_t idx = 1;
  while (BTHomeGetObject(_buf, length, idx, obj_id, dlength, dformat, dfactor)) {
    if (0x00 == obj_id) { 
      packet_id = _buf[idx];  // Incremental or same packet_id
      break;
    }
    idx += dlength;
  }
/*
  if (-1 == packet_id) {
    packet_id = 0;  // packet_id needs to be an 8-bit value from 0 to 255
  }
  uint32_t slot = MIBLEgetSensorSlot(mac, 0xFCD2, packet_id, (packet_id != 0)?1:0); // Ignore duplicates if packet_id is used
*/
  if (-1 == packet_id) {  // No packet_id in BTHome packet so calculate unique 8-bit hash
    uint8_t hash = 0;
    for (idx = 0; idx < length; idx++) {
      uint8_t inbyte = _buf[idx];
      for (uint32_t i = 8; i; i--) {
        uint8_t mix = (hash ^ inbyte) & 0x01;
        hash >>= 1;
        if (mix) {
          hash ^= 0x8C;
        }
        inbyte >>= 1;
      }
    }
    packet_id = hash;  // Non incremental or same packet_id (can still be used to chk for duplicates)
  }
  uint32_t _slot = MIBLEgetSensorSlot(mac, 0xFCD2, packet_id, 1); // Ignore duplicates

#ifdef USE_MI_DEBUG 
  AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("%s packet %*_H%s"), log_name, length, _buf, (_slot == 0xff)?" (duplicate)":"");
#endif

  if ((_slot == 0xff) ||                 // Skip duplicates
      (_slot >= MIBLEsensors.size())) {
    return;
  }

  MIBLEsensors[_slot].RSSI     = RSSI;
  MIBLEsensors[_slot].needkey  = KEY_NOT_REQUIRED;
  MIBLEsensors[_slot].lastTime = Rtc.local_time;

  bool hasTemp = false;
  bool hasHum  = false;
  uint32_t multi = 0;
  uint32_t multi_event = 0;
  uint32_t multi_unparsed = 0;
  uint32_t last_obj_id = 0;

  // Second pass: parse all measurement objects
  int res = 0;
  obj_id = 0;
  idx = 1;
  while (BTHomeGetObject(_buf, length, idx, obj_id, dlength, dformat, dfactor)) {

    if (obj_id == last_obj_id) {
      multi++;
    } else {
      multi = 0;
    }
    last_obj_id = obj_id;

    uint32_t value_uint = 0;
    int value_int = 0;
    if (0 == dformat) {      // uint
      for (uint32_t l = 0; l < dlength; l++) {
        value_uint |= ((uint32_t)_buf[idx+l] << (l * 8));
      }
    }
    else if (1 == dformat) { // int
      if (1 == dlength) {
        value_int = (int8_t)_buf[idx];
      }
      else if (2 == dlength) {
        value_int = (int16_t)(_buf[idx] | ((uint32_t)_buf[idx+1] << 8));
      }
      else if (4 == dlength) {
        value_int = (int)(_buf[idx] | ((uint32_t)_buf[idx+1] << 8) | ((uint32_t)_buf[idx+2] << 16) | ((uint32_t)_buf[idx+3] << 24));
      }
    }

    float factor = 1.0f;
    if (7 == dfactor) {
      factor = 0.35f;
    } else {
      for (uint32_t f = 0; f < dfactor; f++) {
        factor /= 10.0f;
      }
    }
    float value_float = ((1 == dformat) ? (float)value_int : (float)value_uint) * factor;

//    AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("%s obj %02x, Vars int %d, uint %d, float %*_f"), log_name, obj_id, value_int, value_uint, dfactor, &value_float);

    switch (obj_id) {
      case 0x00:   // Packet ID (already used above)
        break;

      case 0x01: { // Battery (uint8, %)
        MIBLEsensors[_slot].bat = value_uint;
        MIBLEsensors[_slot].feature.bat    = 1;
        MIBLEsensors[_slot].eventType.bat  = 1;
      } break;

      case 0x02:   // Temperature (int16, 0.01 °C)
      case 0x45:   // Temperature (int16, 0.1 °C) — lower-precision variant
      case 0x57:   // Temperature (int8, 1 °C)
      case 0x58: { // Temperature (int8, 0.35 °C)
        if (value_float > -40.0f && value_float < 85.0f) {
          MIBLEsensors[_slot].temp = value_float;
          MIBLEsensors[_slot].feature.temp   = 1;
          MIBLEsensors[_slot].eventType.temp = 1;
          hasTemp = true;
        }
      } break;

      case 0x03:   // Humidity (uint16, 0.01 %)
      case 0x2E: { // Humidity (uint8, 1 %)
        if (value_float >= 0.0f && value_float <= 100.0f) {
          MIBLEsensors[_slot].hum = value_float;
          MIBLEsensors[_slot].feature.hum   = 1;
          MIBLEsensors[_slot].eventType.hum = 1;
          hasHum = true;
        }
      } break;

      case 0x04: { // Pressure (0.01 hPa) uint24
        MIBLEsensors[_slot].pres = value_float;
        MIBLEsensors[_slot].feature.pres   = 1;
        MIBLEsensors[_slot].eventType.pres = 1;
      } break;

      case 0x05: { // Illuminance (uint24, 0.01 lx)
        MIBLEsensors[_slot].lux = (uint32_t)value_float;
        MIBLEsensors[_slot].feature.lux   = 1;
        MIBLEsensors[_slot].eventType.lux = 1;
      } break;

      case 0x06:   // Mass kg (0.01 kg) uint16
      case 0x07: { // Mass lbs (0.01 lbs) uint16
        MIBLEsensors[_slot].weight = value_float;
        MIBLEsensors[_slot].feature.scale   = 1;
        MIBLEsensors[_slot].eventType.scale = 1;
        snprintf_P(MIBLEsensors[_slot].weight_unit, sizeof(MIBLEsensors[_slot].weight_unit), PSTR("%s"), (0x06 == obj_id)?"kg":"lbs");
      } break;

      case 0x08:   // Dewpoint — Tasmota calculates dew point internally
        break;

      case 0x09:   // Count (1) uint8
      case 0x3D:   // Count (1) uint16
      case 0x3E:   // Count (1) uint32
      case 0x59:   // Count (1) int8
      case 0x5A:   // Count (1) int16
      case 0x5B: { // Count (1) int32
        MIBLEsensors[_slot].count = (obj_id >= 0x59) ? value_int : value_uint;
        MIBLEsensors[_slot].feature.count   = 1;
        MIBLEsensors[_slot].eventType.count = 1;
      } break;
/*
      case 0x0A: { // Energy (0.001 kWh) uint24
      } break;

      case 0x0B: { // Power (0.01 W) uint24
      } break;
*/
      case 0x0C:   // Voltage (0.001 V) uint16
      case 0x4A: { // Voltage (0.1 V) uint16
        MIBLEsensors[_slot].volt = value_float;
        MIBLEsensors[_slot].feature.volt   = 1;
        MIBLEsensors[_slot].eventType.volt = 1;
      } break;

      case 0x0F ... 0x11:
      case 0x15 ... 0x2D: { // Binary sensor catch all (uint8, 0 or 1)
        if (0 == multi_event) {  // Reset events
          for (uint32_t i = 0; i < sizeof(MIBLEsensors[_slot].bthome_event); i++) {
            MIBLEsensors[_slot].bthome_event[i] = 0;
          }
        }
        if (multi_event < sizeof(MIBLEsensors[_slot].bthome_event)) {
          MIBLEsensors[_slot].bthome_event[multi_event++] = value_uint << 7 | obj_id;
          MIBLEsensors[_slot].events++;
          MIBLEsensors[_slot].eventType.event = 1;
          res = 1;
        }
      } break;

      case 0x14:   // Moisture (uint16, 0.01%)
      case 0x2F: { // Moisture (uint8, 1%)
        MIBLEsensors[_slot].moisture = (uint8_t)value_float;
        MIBLEsensors[_slot].feature.moist   = 1;
        MIBLEsensors[_slot].eventType.moist = 1;
      } break;

      case 0x3A: { // Button (uint8, event)
        if (multi < sizeof(MIBLEsensors[_slot].button)) {
          if (255 == value_uint) {  // Older version of BTHome for Hold button
            value_uint = 128;       // Current value for Hold button
          }
          if (0 == multi) {  // Reset events
            for (uint32_t i = 0; i < sizeof(MIBLEsensors[_slot].button); i++) {
              MIBLEsensors[_slot].button[i] = 0;
            }
          }
          MIBLEsensors[_slot].Btn = value_uint;  // Last button
          MIBLEsensors[_slot].button[multi] = (0 == value_uint) ? 255 : value_uint;
          MIBLEsensors[_slot].feature.Btn = 1;
          if (value_uint > 0) {  // No event if 0x00
            MIBLEsensors[_slot].eventType.Btn = 1;
            res = 1;
          }
        }
      } break;

      case 0x3B: { // Event command 0..2 uint16, 3..4 uint24
        AddLog(LOG_LEVEL_DEBUG, PSTR("%s command %*_H"), log_name, dlength, _buf + idx);
      } break;

      case 0x50: { // Timestamp uint32
        AddLog(LOG_LEVEL_DEBUG, PSTR("%s timestamp %s UTC"), log_name, GetDT(value_uint).c_str());
      } break;

      case 0x51:   // Acceleration (0.001 m/s2) uint16
      case 0x63: { // Acceleration (0.000001 m/s2) int32
        if ((multi >= 0) && (multi < 4)) {  // Support for up to 4 accelerations
          MIBLEsensors[_slot].acceleration[multi] = value_float;
          MIBLEsensors[_slot].accel_used = multi;
          MIBLEsensors[_slot].feature.accel   = 1;
          MIBLEsensors[_slot].eventType.accel = 1;
        }
      } break;

      case 0x53: { // Text
        char text[dlength +1];
        ext_snprintf_P(text, sizeof(text), PSTR("%s"), _buf + idx);
        AddLog(LOG_LEVEL_DEBUG, PSTR("%s text %s"), log_name, text);
      } break;

      case 0x54: { // Raw
        AddLog(LOG_LEVEL_DEBUG, PSTR("%s raw %*_H"), log_name, dlength, _buf + idx);
      } break;

      case 0x64: { // Light level (0 = Dark, 1 = Twilight, 2 = Bright) 
        MIBLEsensors[_slot].light = value_uint;
        MIBLEsensors[_slot].feature.light = 1;
        MIBLEsensors[_slot].eventType.light = 1;
        res = 1;
      } break;

      case 0xF0: { // Device type id uint16
        AddLog(LOG_LEVEL_DEBUG, PSTR("%s device type id %d"), log_name, value_uint);
      } break;


      case 0xF1: { // Firmware version (F100010204 = 4.2.1.0} uint32
        AddLog(LOG_LEVEL_DEBUG, PSTR("%s firmware version %d.%d.%d.%d"), log_name, _buf[idx+3], _buf[idx+2], _buf[idx+1], _buf[idx]);
      } break;


      case 0xF2: { // Firmware version (F2000106 = 6.1.0} uint24
        AddLog(LOG_LEVEL_DEBUG, PSTR("%s firmware version %d.%d.%d"), log_name, _buf[idx+2], _buf[idx+1], _buf[idx]);
      } break;

      default: {
        uint32_t unparsed_slots = sizeof(MIBLEsensors[_slot].unparsed);
        if (multi_unparsed < unparsed_slots) {
          if (0 == multi_unparsed) {  // Reset
            for (uint32_t i = 0; i < unparsed_slots; i++) {
              MIBLEsensors[_slot].unparsed[i] = 0;
            }
          }
          MIBLEsensors[_slot].unparsed[multi_unparsed] = obj_id;
          MIBLEsensors[_slot].unparsed_float[multi_unparsed] = value_float;
          MIBLEsensors[_slot].feature.unparsed = 1;
          multi_unparsed++;
          res = 1;
        } else {
          AddLog(LOG_LEVEL_DEBUG, PSTR("%s unparsed obj id 0x%02x (%d), data %*_H = %*_f"), log_name, obj_id, obj_id, dlength, _buf + idx, dfactor, &value_float);
        }
      } break;
    }
    idx += dlength;
  }

  // If both temperature and humidity were received, mark the combined feature
  if (hasTemp && hasHum) {
    MIBLEsensors[_slot].feature.tempHum   = 1;
    MIBLEsensors[_slot].eventType.tempHum = 1;
  }

  if (res || MI32.option.directBridgeMode) {
    MIBLEsensors[_slot].shallSendMQTT = 1;
    MI32.mode.shallTriggerTele = 1;
  }

  AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("%s slot %u"), log_name, _slot);
}

////////////////////////////////////////////////////////////
// this SHOULD parse any MI payload.
int MI32parseMiPayload(int _slot, struct mi_beacon_data_t *parsed){
  struct mi_beacon_data_payload_data_t *pld =
    (struct mi_beacon_data_payload_data_t *) &parsed->payload.data;
  int res = 1;

  if (!parsed->payloadpresent){
    return 0;
  }

  char tmp[20];
  BLE_ESP32::dump(tmp, 20, (uint8_t*)&(parsed->payload), parsed->payload.size + 3);
  AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG_MORE], PSTR("M32: %s: slot %d, payload %s"), MIaddrStr(MIBLEsensors[_slot].MAC), _slot, tmp);

  // clear this for every payload
  MIBLEsensors[_slot].pairing = 0;
  MIBLEsensors[_slot].eventType.PairBtn = 0;

  //https://iot.mi.com/new/doc/embedded-development/ble/object-definition

  switch(parsed->payload.type){
    case 0x0002: // related to pair button? 'easypairing'
      MIBLEsensors[_slot].pairing = 1;
      MIBLEsensors[_slot].eventType.PairBtn = 1;
      MIBLEsensors[_slot].feature.pairing = 1;
      MI32.mode.shallTriggerTele = 1;
      MIBLEsensors[_slot].shallSendMQTT = 1;
      break;
    case 0x0003: {// motion? 1 byte 'near'
      uint8_t motion = parsed->payload.data[0];
      res = 0;
    }break;
    case 0x000f: // 'Someone is moving (with light)'
      MIBLEsensors[_slot].eventType.motion = 1;
//      MIBLEsensors[_slot].lastTime = Rtc.local_time;
      MIBLEsensors[_slot].events++;
      MIBLEsensors[_slot].lux = pld->lux;
      MIBLEsensors[_slot].eventType.lux = 1;
      MIBLEsensors[_slot].NMT = 0;
      MI32.mode.shallTriggerTele = 1;
      MIBLEsensors[_slot].shallSendMQTT = 1;
      MIBLEsensors[_slot].feature.lux = 1;
      MIBLEsensors[_slot].feature.NMT = 1;
      MIBLEsensors[_slot].feature.events = 1;

      // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: PIR: primary"),MIBLEsensors[_slot].lux );
    break;

    case 0x1001: // button press
      MIBLEsensors[_slot].Btn = pld->Btn.num + (pld->Btn.longPress/2)*6;
      MIBLEsensors[_slot].feature.Btn = 1;
      MIBLEsensors[_slot].eventType.Btn = 1;
      MI32.mode.shallTriggerTele = 1;
      MIBLEsensors[_slot].shallSendMQTT = 1;
      break;
    //case 0x1002: // 'sleep'
    //case 0x1003: // 'RSSI'

    case 0x1004:{ // 'temperature'
      float _tempFloat=(float)(pld->temp)/10.0f;
      if(_tempFloat<60){
        MIBLEsensors[_slot].temp=_tempFloat;
        MIBLEsensors[_slot].feature.temp = 1;
        MIBLEsensors[_slot].eventType.temp = 1;
        AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG_MORE],PSTR("M32: Mode 4: temp updated"));
      } else {
        AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG_MORE],PSTR("M32: Mode 4: temp ignored > 60 (%2_f)"), &_tempFloat);
      }
      // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Mode 4: U16:  %u Temp"), _beacon.temp );
    } break;
    // 0x1005 - not documented
    case 0x1006: { // 'humidity'
      float _tempFloat=(float)(pld->hum)/10.0f;
      if(_tempFloat<101){
        MIBLEsensors[_slot].hum=_tempFloat;
        MIBLEsensors[_slot].feature.hum = 1;
        MIBLEsensors[_slot].eventType.hum = 1;
        AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG_MORE],PSTR("M32: Mode 6: hum updated"));
      } else {
        AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG_MORE],PSTR("M32: Mode 6: hum ignored > 101 (%2_f)"), &_tempFloat);
      }
      // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Mode 6: U16:  %u Hum"), _beacon.hum);
    } break;
    case 0x1007: // 'Light illuminance'
      MIBLEsensors[_slot].lux=pld->lux & 0x00ffffff;
      if(MIBLEsensors[_slot].type==MI_MJYD2S){
        MIBLEsensors[_slot].eventType.noMotion  = 1;
      }
      MIBLEsensors[_slot].eventType.lux  = 1;
      MIBLEsensors[_slot].feature.lux = 1;
      // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Mode 7: U24: %u Lux"), _beacon.lux & 0x00ffffff);
    break;
    case 0x1008: //'Soil moisture'
      MIBLEsensors[_slot].moisture=pld->moist;
      MIBLEsensors[_slot].eventType.moist  = 1;
      MIBLEsensors[_slot].feature.moist = 1;
      AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG_MORE],PSTR("M32: Mode 8: moisture updated"));
      // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Mode 8: U8: %u Moisture"), _beacon.moist);
    break;
    case 0x1009: // 'conductivity' / 'Soil EC value'
      MIBLEsensors[_slot].fertility=pld->fert;
      MIBLEsensors[_slot].eventType.fert  = 1;
      MIBLEsensors[_slot].feature.fert = 1;
      AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG_MORE],PSTR("M32: Mode 9: fertility updated"));
      // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Mode 9: U16: %u Fertility"), _beacon.fert);
    break;
    case 0x100a:// 'Electricity'
      if(MI32.option.ignoreBogusBattery){
        if(MIBLEsensors[_slot].type==MI_LYWSD03MMC || MIBLEsensors[_slot].type==MI_MHOC401){
          res = 0;
          break;
        }
      }
      if (pld->bat < 101) {
        MIBLEsensors[_slot].bat = pld->bat;
        AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG_MORE], PSTR("M32: %s: Mode a: bat updated (%d)"), MIaddrStr(MIBLEsensors[_slot].MAC), pld->bat);
      } else {
        MIBLEsensors[_slot].bat = 100;
        AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG_MORE], PSTR("M32: %s: Mode a: bat > 100 (%d)"), MIaddrStr(MIBLEsensors[_slot].MAC), pld->bat);
      }
      MIBLEsensors[_slot].eventType.bat  = 1;
      MIBLEsensors[_slot].feature.bat = 1;
      // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Mode a: U8: %u %%"), _beacon.bat);
    break;
    // 100b-100d -> undefined in docs.
    case 0x100d:{ // is this right????
      MIBLEsensors[_slot].feature.tempHum = 1;
      float _tempFloat=(float)(pld->HT.temp)/10.0f;
      if(_tempFloat < 60){
        MIBLEsensors[_slot].temp = _tempFloat;
        AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG_MORE],PSTR("M32: Mode d: temp updated"));
      } else {
        AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG_MORE],PSTR("M32: Mode d: temp ignored > 60 (%2_f)"), &_tempFloat);
      }
      _tempFloat=(float)(pld->HT.hum)/10.0f;
      if(_tempFloat < 100){
        MIBLEsensors[_slot].hum = _tempFloat;
        AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG_MORE],PSTR("M32: Mode d: hum updated"));
      } else {
        AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG_MORE],PSTR("M32: Mode d: hum ignored > 100 (%2_f)"), &_tempFloat);
      }
      MIBLEsensors[_slot].eventType.tempHum  = 1;
      // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Mode d: U16:  %x Temp U16: %x Hum"), _beacon.HT.temp,  _beacon.HT.hum);
    } break;
    // 100e = 'lock'
    // 100f = 'door'
    case 0x1010:{ // 'formaldehide'
      const uint16_t f = uint16_t(parsed->payload.data[0]) | (uint16_t(parsed->payload.data[1]) << 8);
      float formaldehyde = (float)f / 100.0f;
      res = 0;
    } break;
    // 1011 = 'bind'
    case 0x1012:{ // 'switch'
      int active = parsed->payload.data[0];
      res = 0;
    } break;
    case 0x1013:{ // 'Remaining amount of consumables' - mosquito tablet
      int tablet = parsed->payload.data[0];
      res = 0;
    } break;
    //Flooding	0x1014	1	1
    case 0x1014:{                                                      //'Flooding sensor' - 0=dry, 1=wet SJWS01LM
      MIBLEsensors[_slot].flooding = (uint8_t)parsed->payload.data[0]; // just an 8 bit value in a union.
      MIBLEsensors[_slot].eventType.flooding = 1;
      MI32.mode.shallTriggerTele = 1;
      MIBLEsensors[_slot].shallSendMQTT = 1;
      MIBLEsensors[_slot].feature.flooding = 1;
    }
    break;
    //smoke	0x1015	1	1
    //Gas	0x1016
    case 0x1017:{ // 'No one moves'
      const uint32_t idle_time =
          uint32_t(parsed->payload.data[0]) | (uint32_t(parsed->payload.data[1]) << 8) | (uint32_t(parsed->payload.data[2]) << 16) | (uint32_t(parsed->payload.data[2]) << 24);
      float idlemins = (float)idle_time / 60.0f;
      int has_motion = (idle_time) ? 0 : 0;

      MIBLEsensors[_slot].NMT = pld->NMT;
      MIBLEsensors[_slot].eventType.NMT = 1;
      MI32.mode.shallTriggerTele = 1;
      MIBLEsensors[_slot].shallSendMQTT = 1;
      MIBLEsensors[_slot].feature.NMT = 1;

      // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Mode 17: NMT: %u seconds"), _beacon.NMT);
    } break;
    //Light intensity	0x1018
    case 0x1018:{ //'Light intensity' - 0=dark, 1=light? - MCCGQ02HL
      MIBLEsensors[_slot].light = parsed->payload.data[0];
      MIBLEsensors[_slot].eventType.light = 1;
      MI32.mode.shallTriggerTele = 1;
      MIBLEsensors[_slot].shallSendMQTT = 1;
      MIBLEsensors[_slot].feature.light = 1;
    } break;
    case 0x1019:{ //'Door sensor' - 0=open, 1=closed, 2=timeout? - MCCGQ02HL
      MIBLEsensors[_slot].Btn = (uint8_t) parsed->payload.data[0]; // just an 8 bit value in a union.
      MIBLEsensors[_slot].eventType.Btn = 1;
      MI32.mode.shallTriggerTele = 1;
      MIBLEsensors[_slot].shallSendMQTT = 1;
      MIBLEsensors[_slot].feature.Btn = 1;
    } break;
    case 0x4c01:{ // 'temperature'
      const uint32_t payload32 = uint32_t(parsed->payload.data[0]) | (uint32_t(parsed->payload.data[1]) << 8) | (uint32_t(parsed->payload.data[2]) << 16) | (uint32_t(parsed->payload.data[3]) << 24);
      float _tempFloat;
      memcpy(&_tempFloat, &payload32, sizeof(_tempFloat));
      MIBLEsensors[_slot].temp = _tempFloat;
      MIBLEsensors[_slot].feature.temp = 1;
      MIBLEsensors[_slot].eventType.temp = 1;
    } break;
    case 0x4c02:{ // 'humidity'
      const uint8_t payload8 = parsed->payload.data[0];
      float _tempFloat = (float)payload8 / 1.0f;
      MIBLEsensors[_slot].hum = _tempFloat;
      MIBLEsensors[_slot].feature.hum = 1;
      MIBLEsensors[_slot].eventType.hum = 1;
    } break;

    //Weight attributes	0x101A	600	0
    //No one moves over time	0x101B	1	1
    //Smart pillow	0x101C	60	1
    //Formaldehyde (new)	0x101D

    default: {
      AddLog(LOG_LEVEL_DEBUG, PSTR("M32: %s: Unknown MI pld type: %x - %s"), MIaddrStr(MIBLEsensors[_slot].MAC), parsed->payload.type, tmp);
      res = 0;
    } break;
  }

  if(res && MI32.option.directBridgeMode) {
    MIBLEsensors[_slot].shallSendMQTT = 1;
    MI32.mode.shallTriggerTele = 1;
  }

  return res;
}


/*
  AT Button Manufacturer Data
  0 Company ID = 2 bytes
  2 Unknown = 4 bytes 7fffffff
  6 Counter = 1 byte
  7 Unknown = 1 byte 05
  8 Switch = 2 bytes
  10 Unknown = 1 byte 00
  11 Button = 1 bytes 0x (x = 1 to 3) 
  12 Unknown = 4 bytes 50000100
*/
struct __attribute__ ((packed)) ATBtn_data
{
  uint16_t company_id;
  uint32_t unknown1;
  uint8_t counter;
  uint8_t unknown2;
  union {
    struct {
      uint8_t switch1;
      uint8_t switch2;
    };
    uint16_t switch_id;
  };
  uint8_t unknown3;
  uint8_t button_id;
  uint32_t unknown4;
};

void MI32ParseATBtn(uint8_t *buf, uint16_t bufsize, const uint8_t* addr, int RSSI) {

  ATBtn_data* data = (ATBtn_data*)buf;
  // AddLog(LOG_LEVEL_INFO,PSTR("ATBTN %04x, %d, Counter(%u) Switch(%04x), Button(%x) %x"), data->company_id, bufsize, data->counter, data->switch_id, data->button_id, kMI32DeviceID[AT_BTN-1]);
  if (bufsize != sizeof(ATBtn_data) || data->company_id != kMI32DeviceID[AT_BTN-1])
    return;
  // since all have same address, we'll move the "switch Id" into the last two addr fields to give each switch a unique MAC address
  uint8_t _addr[6];
  memcpy(_addr,addr,6);
  _addr[4] = data->switch1;
  _addr[5] = data->switch2;
  uint32_t _slot = MIBLEgetSensorSlot(_addr, kMI32DeviceID[AT_BTN-1], data->counter, 1);
  if(_slot == 0xff) return;

  // AddLog(LOG_LEVEL_DEBUG,PSTR("%s at slot %u"), MI32getDeviceName(_slot),_slot);
  MIBLEsensors[_slot].RSSI=RSSI;
  MIBLEsensors[_slot].lastTime = Rtc.local_time;
  MIBLEsensors[_slot].eventType.Btn = 1;
  MI32.mode.shallTriggerTele = 1;
  MIBLEsensors[_slot].shallSendMQTT = 1;
  MIBLEsensors[_slot].feature.Btn = 1;
  MIBLEsensors[_slot].Btn = data->button_id;
}

////////////////////////////////////////////////////////////
// this SHOULD parse any MI packet, including encrypted.
void MI32ParseResponse(const uint8_t *buf, uint16_t bufsize, const uint8_t* addr, int RSSI) {
  struct mi_beacon_data_t parsed;
  memset(&parsed, 0, sizeof(parsed));
  int res = MIParsePacket(addr, &parsed, buf, bufsize);

  uint8_t addrrev[6];
  memcpy(addrrev, addr, 6);
  MI32_ReverseMAC(addrrev);

  if (memcmp(addrrev, parsed.macdata.mac, 6)){
    AddLog(LOG_LEVEL_ERROR, PSTR("M32: MI packet with MAC addr mismatch - is this mesh?"));
    memcpy(addrrev, parsed.macdata.mac, 6);
    MI32_ReverseMAC(addrrev);
    addr = addrrev;
  }

  uint16_t _slot = MIBLEgetSensorSlot(addr, parsed.devicetype, parsed.framecnt);
  if(_slot == 0xff) return;
  if ((_slot >= 0) && (_slot < MIBLEsensors.size())){
    if (parsed.needkey != KEY_REQUIREMENT_UNKNOWN){
      MIBLEsensors[_slot].needkey = parsed.needkey;
    }
    MIBLEsensors[_slot].RSSI=RSSI;
    if (!res){ // - if the payload is not valid
      AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("M32: %s: MIParsePacket returned %d"), MIaddrStr(addr), res);
      return;
    }
    MIBLEsensors[_slot].lastTime = Rtc.local_time;
    MI32parseMiPayload(_slot, &parsed);
  }
}

bool MI32isInBlockList(const uint8_t* MAC){
  bool isBlocked = false;
  for(auto &_blockedMAC : MIBLEBlockList){
    if(!memcmp(_blockedMAC.buf, MAC, 6)) isBlocked = true;
  }
  return isBlocked;
}

void MI32removeMIBLEsensor(uint8_t* MAC){
  // this will take and keep the mutex until the function is over
  TasAutoMutex localmutex(&slotmutex, "Mi32Rem");

  MIBLEsensors.erase( std::remove_if( MIBLEsensors.begin() , MIBLEsensors.end(), [MAC]( mi_sensor_t _sensor )->bool
  { return (!memcmp(_sensor.MAC, MAC, 6)); }
  ), end( MIBLEsensors ) );
}

/***********************************************************************\
 * Read data from connections
\***********************************************************************/

void MI32notifyHT_LY(int _slot, char *_buf, int len){
  AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG_MORE],PSTR("M32: %s: Raw data %02x%02x%02x%02x%02x%02x%02x"),MIaddrStr(MIBLEsensors[_slot].MAC), _buf[0], _buf[1], _buf[2], _buf[3], _buf[4], _buf[5], _buf[6]);
  // the value 0b00 is 28.16 C?
  if (_buf[0] || _buf[1]){
    memcpy(&LYWSD0x_HT, (void *)_buf, sizeof(LYWSD0x_HT));
    AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("M32: %s: T * 100: %u, H: %u, V: %u"), MIaddrStr(MIBLEsensors[_slot].MAC), LYWSD0x_HT.temp, LYWSD0x_HT.hum, LYWSD0x_HT.volt);
    AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG_MORE], PSTR("M32: %s: Sensor slot %u"), MIaddrStr(MIBLEsensors[_slot].MAC), _slot);
    static float _tempFloat;
    _tempFloat = (float)(LYWSD0x_HT.temp) / 100.0f;
    if(_tempFloat < 60){
      MIBLEsensors[_slot].temp = _tempFloat;
      AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG_MORE], PSTR("M32: %s: LYWSD0x Temp updated %1_f"), MIaddrStr(MIBLEsensors[_slot].MAC), &MIBLEsensors[_slot].temp);
        // MIBLEsensors[_slot].showedUp=255; // this sensor is real
    }
    _tempFloat=(float)LYWSD0x_HT.hum;
    if(_tempFloat < 100){
      MIBLEsensors[_slot].hum = _tempFloat;
      AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG_MORE], PSTR("M32: %s: LYWSD0x Hum updated %1_f"), MIaddrStr(MIBLEsensors[_slot].MAC), &MIBLEsensors[_slot].hum);
    }
    MIBLEsensors[_slot].eventType.tempHum  = 1;
    if (MIBLEsensors[_slot].type == MI_LYWSD02MMC || MIBLEsensors[_slot].type == MI_LYWSD02MMC2 || MIBLEsensors[_slot].type == MI_LYWSD03MMC || MIBLEsensors[_slot].type == MI_MHOC401){
      // ok, so CR2032 is 3.0v, but drops immediately to ~2.9.
      // so we'll go with the 2.1 min, 2.95 max.
      float minVolts = 2100.0;
      //float maxVolts = 2950.0;
      //float range = maxVolts - minVolts;
      //float divisor = range/100; // = 8.5
      float percent = (((float)LYWSD0x_HT.volt) - minVolts) / 8.5; //divisor;
      if (percent > 100) percent = 100;

      MIBLEsensors[_slot].bat = (int)percent;
      AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG_MORE], PSTR("M32: %s: LYWSD0x Bat updated %d"), MIaddrStr(MIBLEsensors[_slot].MAC), MIBLEsensors[_slot].bat);
      MIBLEsensors[_slot].eventType.bat  = 1;
    }
    if(MI32.option.directBridgeMode) {
      MIBLEsensors[_slot].shallSendMQTT = 1;
      MI32.mode.shallTriggerTele = 1;
    }
  }
}

/**
 * @brief Launch functions from Core 1 to make race conditions less likely
 *
 */

void MI32Every50mSecond(){

  if(MI32.mode.shallTriggerTele){
      MI32.mode.shallTriggerTele = 0;
      MI32triggerTele();
  }
}

/**
 * @brief Main loop of the driver, "high level"-loop
 *
 */

void MI32EverySecond(bool restart){

//  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("M32: onesec"));
  MI32TimeoutSensors();

  if (MI32.option.MQTTType == 0){
    // show tas style sensor MQTT
    MI32ShowSomeSensors();
  }

  if (MI32.option.MQTTType > 0
#ifdef USE_HOME_ASSISTANT
    ||
    Settings->flag.hass_discovery
#endif
  ) {
    // these two share a counter
    // discovery only sent if hass_discovery
    MI32DiscoveryOneMISensor();
    // show independent style sensor MQTT
    // note - if !MQTTType, then this is IN ADDITION to 'normal'
    MI32ShowOneMISensor(MI32.option.MQTTType < 2);
  }

  // read a battery if
  // MI32.batteryreader.slot < filled and !MI32.batteryreader.active
  readOneBat();

  // read a sensor if
  // MI32.sensorreader.slot < filled and !MI32.sensorreader.active
  // for sensors which need to get data through notify...
  readOneSensor();

  if (MI32.secondsCounter >= MI32.period){
    // only if we finished the last read
    if (MI32.sensorreader.slot >= MIBLEsensors.size()){
      AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Kick off readOneSensor"));
      // kick off notification sensor reading every period.
      MI32.sensorreader.slot = 0;
      MI32.secondsCounter = 0;
    }
  }
  MI32.secondsCounter++;

  if (MI32.secondsCounter2 >= MI32.period){
    if (MI32.mqttCurrentSlot >= MIBLEsensors.size()){
      AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Kick off tele sending"));
      MI32.mqttCurrentSlot = 0;
      MI32.secondsCounter2 = 0;
      MI32.mqttCurrentSingleSlot = 0;
    } else {
      AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Hit tele time, restarted but not finished last - lost from slot %d")+MI32.mqttCurrentSlot);
      MI32.mqttCurrentSlot = 0;
      MI32.secondsCounter2 = 0;
      MI32.mqttCurrentSingleSlot = 0;
    }
  }
  MI32.secondsCounter2++;

  static uint32_t _counter = MI32.period - 15;
  static uint32_t _nextSensorSlot = 0;
  uint32_t _idx = 0;

  int numsensors = MIBLEsensors.size();
  for (uint32_t i = 0; i < numsensors; i++) {
    if(MIBLEsensors[i].type==MI_NLIGHT || MIBLEsensors[i].type==MI_MJYD2S){
      MIBLEsensors[i].NMT++;
    }
  }

  if(MI32.mode.shallShowStatusInfo == 1){
    MI32StatusInfo();
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndMi32Period(void) {
  if (XdrvMailbox.data_len > 0) {
    if (1 == XdrvMailbox.payload) {
      MI32EverySecond(true);
    } else {
      MI32.period = XdrvMailbox.payload;
    }
  }
  ResponseCmndNumber(MI32.period);
}

int findSlot(char *addrOrAlias){
  uint8_t mac[7];
  int res = BLE_ESP32::getAddr(mac, addrOrAlias);
  if (!res) return -1;

  for (int i = MIBLEsensors.size()-1; i >= 0 ; i--) {
    if (!memcmp(MIBLEsensors[i].MAC, mac, 6)){
      return i;
    }
  }
  return -1;
}

void CmndMi32Time(void) {
  if (XdrvMailbox.data_len > 0) {
    int slot = findSlot(XdrvMailbox.data);
    if (slot < 0) {
      slot = XdrvMailbox.payload;
    }
    if (MIBLEsensors.size() > slot) {
      int res = genericTimeWriteFn(slot);
      if (res > 0){
        AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("M32: will set Time"));
        ResponseCmndNumber(slot);
        return;
      }
      if (res < 0) {
        AddLog(LOG_LEVEL_ERROR, PSTR("M32: cannot set Time on sensor type"));
      }
      if (res == 0) {
        AddLog(LOG_LEVEL_ERROR, PSTR("M32: cannot set Time right now"));
      }
    }
  }
  ResponseCmndChar_P("fail");
}

void CmndMi32Page(void) {
  if (XdrvMailbox.payload > 0) {
    MI32.perPage = XdrvMailbox.payload;
  }
  ResponseCmndNumber(MI32.perPage);
}

// read ALL battery values where we can?
void CmndMi32Battery(void) {
  // trigger a read cycle
  MI32.batteryreader.slot = 0;
  ResponseCmndDone();
}

void CmndMi32Unit(void) {
  if (XdrvMailbox.data_len > 0) {
    int slot = findSlot(XdrvMailbox.data);
    if (slot < 0) {
      slot = XdrvMailbox.payload;
    }

    if (MIBLEsensors.size() > slot) {
      // TOGGLE unit?
      int res = genericUnitWriteFn(slot, -1);
      if (res > 0){
        AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], PSTR("M32: will toggle Unit"));
        ResponseCmndNumber(slot);
        return;
      }
      if (res < 0) {
        AddLog(LOG_LEVEL_ERROR, PSTR("M32: cannot toggle Unit on sensor type"));
      }
      if (res == 0) {
        AddLog(LOG_LEVEL_ERROR, PSTR("M32: cannot toggle Unit right now"));
      }
    }
  }
  ResponseCmndIdxChar(PSTR("Invalid"));
}

#ifdef USE_MI_DECRYPTION
void CmndMi32Key(void) {
  if (44 == XdrvMailbox.data_len) {  // a KEY-MAC-string
    MI32AddKey(XdrvMailbox.data, nullptr);
    MI32KeyListResp();
  } else {
    ResponseCmndIdxChar(PSTR("Invalid"));
  }
}
#endif  // USE_MI_DECRYPTION

void MI32BlockListResp(){
  Response_P(PSTR("{\"MI32Block\":{"));
  for (int i = 0; i < MIBLEBlockList.size(); i++){
    if (i){
      ResponseAppend_P(PSTR(","));
    }
    char tmp[20];
    ToHex_P(MIBLEBlockList[i].buf,6,tmp,20,0);
    ResponseAppend_P(PSTR("\"%s\":1"), tmp);
  }
  ResponseAppend_P(PSTR("}}"));
}

void CmndMi32Block(void){
  if (XdrvMailbox.data_len == 0) {
    switch (XdrvMailbox.index) {
      case 0: {
        //TasAutoMutex localmutex(&slotmutex, "Mi32Block1");
        MIBLEBlockList.clear();
      } break;
      default:
      case 1:
        break;
    }
    MI32BlockListResp();
    return;
  }

  MAC_t _MACasBytes;
  int res = BLE_ESP32::getAddr(_MACasBytes.buf, XdrvMailbox.data);
  if (!res){
    ResponseCmndIdxChar(PSTR("Addr invalid"));
    return;
  }

  //MI32HexStringToBytes(XdrvMailbox.data,_MACasBytes.buf);
  switch (XdrvMailbox.index) {
    case 0: {
      //TasAutoMutex localmutex(&slotmutex, "Mi32Block2");
      MIBLEBlockList.erase( std::remove_if( begin( MIBLEBlockList ), end( MIBLEBlockList ), [_MACasBytes]( MAC_t& _entry )->bool
        { return (!memcmp(_entry.buf, _MACasBytes.buf, 6)); }
        ), end( MIBLEBlockList ) );
    } break;
    case 1: {
      //TasAutoMutex localmutex(&slotmutex, "Mi32Block3");
      bool _notYetInList = true;
      for (auto &_entry : MIBLEBlockList) {
        if (!memcmp(_entry.buf, _MACasBytes.buf, 6)){
          _notYetInList = false;
        }
      }
      if (_notYetInList) {
        MIBLEBlockList.push_back(_MACasBytes);
        MI32removeMIBLEsensor(_MACasBytes.buf);
      }
      // AddLog(LOG_LEVEL_INFO,PSTR("M32: size of ilist: %u"), MIBLEBlockList.size());
    } break;
  }
  MI32BlockListResp();
}

void CmndMi32Option(void){
  bool set = false;
  if (strlen(XdrvMailbox.data)){
    set = true;
  }
  int value = atoi(XdrvMailbox.data);
  switch(XdrvMailbox.index) {
    case 0:
      if (set){
        MI32.option.allwaysAggregate = value;
      } else {
        value = MI32.option.allwaysAggregate;
      }
      break;
    case 1:
      if (set){
        MI32.option.noSummary = value;
      } else {
        value = MI32.option.noSummary;
      }
      break;
    case 2:
      if (set){
        MI32.option.directBridgeMode = value;
      } else {
        value = MI32.option.directBridgeMode;
      }
      break;
    case 4:{
      if (set){
        MI32.option.ignoreBogusBattery = value;
      } else {
        value = MI32.option.ignoreBogusBattery;
      }
    } break;
    case 5:{
      if (set){
        MI32.option.onlyAliased = value;
        if (MI32.option.onlyAliased){
          // discard all sensors for a restart
          MIBLEsensors.clear();
        }
      } else {
        value = MI32.option.onlyAliased;
      }
    } break;
    case 6:{
      if (set){
        MI32.option.MQTTType = value;
      } else {
        value = MI32.option.MQTTType;
      }
    } break;
    default:{
      return;  // Command Error
    } break;
  }
  ResponseCmndIdxNumber(value);
  return;
}

void CmndMi32Topic(void) {
  if (!XdrvMailbox.grpflg && (XdrvMailbox.data_len > 0)) {
    MakeValidMqtt(0, XdrvMailbox.data);
    if (SC_CLEAR == Shortcut())
      MI32.bleTopic = MI32_BLE_TOPIC;
    else
      MI32.bleTopic = XdrvMailbox.data;
  }
  ResponseCmndChar(MI32.bleTopic.c_str());
}

void MI32KeyListResp(){
  Response_P(PSTR("{\"MIKeys\":{"));
  for (int i = 0; i < MIBLEbindKeys.size(); i++){
    if (i){
      ResponseAppend_P(PSTR(","));
    }
    char tmp[20];
    ToHex_P(MIBLEbindKeys[i].MAC,6,tmp,20,0);
    char key[16*2+1];
    ToHex_P(MIBLEbindKeys[i].key,16,key,33,0);

    ResponseAppend_P(PSTR("\"%s\":\"%s\""), tmp, key);
  }
  ResponseAppend_P(PSTR("}}"));
}

void CmndMi32Keys(void){
#ifdef BLE_ESP32_ALIASES
  int op = XdrvMailbox.index;
  AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG],PSTR("M32: Key %d %s"), op, XdrvMailbox.data);

  int res = -1;
  switch(op){
    case 0:
    case 1:{
      char *p = strtok(XdrvMailbox.data, " ,=");
      bool trigger = false;
      int added = 0;

      do {
        if (!p || !(*p)){
          break;
        }

        uint8_t addr[7];
        char *mac = p;
        int addrres = BLE_ESP32::getAddr(addr, p);
        if (!addrres){
          ResponseCmndChar("invalidmac");
          return;
        }

        p = strtok(nullptr, " ,=");
        char *key = p;
        if (!p || !(*p)){
          int i = 0;
          for (i = 0; i < MIBLEbindKeys.size(); i++){
            mi_bindKey_t *key = &MIBLEbindKeys[i];
            if (!memcmp(key->MAC, addr, 6)){
              MIBLEbindKeys.erase(MIBLEbindKeys.begin() + i);
              MI32KeyListResp();
              return;
            }
          }
          ResponseCmndChar("invalidmac");
          return;
        }

        AddLog(LOG_LEVEL_INFO, PSTR("M32: Add key mac %s = key %s"), mac, key);
        char tmp[20];
        // convert mac back to string
        ToHex_P(addr,6,tmp,20,0);
        if (MI32AddKey(tmp, key)){
          added++;
        }
        p = strtok(nullptr, " ,=");
      } while (p);

      if (added){
        AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG],PSTR("M32: Added %d Keys"), added);
        MI32KeyListResp();
      } else {
        MI32KeyListResp();
      }
      return;
    } break;
    case 2:{ // clear
      AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG],PSTR("M32: Keys clearing %d"), MIBLEbindKeys.size());
      for (int i = MIBLEbindKeys.size()-1; i >= 0; i--){
        MIBLEbindKeys.pop_back();
      }
      MI32KeyListResp();
      return;
    } break;
  }
  ResponseCmndChar("invalididx");
#endif
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

const char HTTP_MI32[] PROGMEM = "{s}MI ESP32 " MI32_VERSION "{m}%u%s / %u{e}";
const char HTTP_MI32_TYPE[] PROGMEM = "{s}%s " D_SENSOR"{m}%s{e}";
const char HTTP_MI32_MAC[] PROGMEM = "{s}%s " D_MAC_ADDRESS "{m}%s{e}";
const char HTTP_MI32_RSSI[] PROGMEM = "{s}%s " D_RSSI "{m}%d%% (%d dBm){e}";
const char HTTP_MI32_BATTERY[] PROGMEM = "{s}%s " D_BATTERY "{m}%u %%{e}";
const char HTTP_MI32_LASTBUTTON[] PROGMEM = "{s}%s Last Button{m}%u{e}";
const char HTTP_MI32_ACCELERATION[] PROGMEM = "{s}%s Acceleration{m}%s{e}";
const char HTTP_MI32_EVENTS[] PROGMEM = "{s}%s Events{m}%u{e}";
const char HTTP_MI32_NMT[] PROGMEM = "{s}%s No motion{m}> %u " D_SECONDS "{e}";
const char HTTP_MI32_FLORA_DATA[] PROGMEM = "{s}%s Fertility{m}%u " D_UNIT_MICROSIEMENS_PER_CM "{e}";
const char HTTP_MI32_LIGHT[] PROGMEM = "{s}%s " D_LIGHT "{m}%d{e}";
const char HTTP_MI32_COUNT[] PROGMEM = "{s}%s " D_COUNT "{m}%d{e}";
const char HTTP_MISCALE_WEIGHT[] PROGMEM = "{s}%s " D_WEIGHT "{m}%*_f %s{e}";
const char HTTP_MISCALE_WEIGHT_REMOVED[] PROGMEM = "{s}%s Weight removed{m}%s{e}";
const char HTTP_MISCALE_WEIGHT_STABILIZED[] PROGMEM = "{s}%s Weight stabilized{m}%s{e}";
const char HTTP_MISCALE_IMPEDANCE[] PROGMEM = "{s}%s Impedance{m}%u{e}";
const char HTTP_MISCALE_IMPEDANCE_STABILIZED[] PROGMEM = "{s}%s Impedance stabilized{m}%s{e}";
const char HTTP_SJWS01LM_FLOODING[] PROGMEM = "{s}%s Flooding{m}%u{e}";

const char HTTP_NEEDKEY[] PROGMEM = "{s}%s Key{m}<a target='_blank' href='https://tasmota.github.io/ble_key_extractor?mac=%s&cb=http%%3A%%2F%%2F%s%%2Fmikey'>%s</a>{e}";

const char HTTP_PAIRING[] PROGMEM = "{s}%s Pair button pressed{m} {e}";

const char HTTP_KEY_ERROR[] PROGMEM = "Key error %s";
const char HTTP_MAC_ERROR[] PROGMEM = "MAC error %s";
const char HTTP_KEY_ADDED[] PROGMEM = "Cmnd: MI32Keys %s=%s";
const char HTTP_MI_KEY_STYLE[] PROGMEM = "";

#define D_MI32_KEY "MI32 Set Key"

void HandleMI32Key(){
  AddLog(LOG_LEVEL_DEBUG, PSTR("M32: HandleMI32Key hit"));
  if (!HttpCheckPriviledgedAccess()) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("M32: !HttpCheckPriviledgedAccess()"));
    return;
  }
  WSContentStart_P(PSTR(D_MI32_KEY));
  WSContentSendStyle_P(HTTP_MI_KEY_STYLE);

  char key[64] = {0};
  WebGetArg("key", key, sizeof(key));

  if (strlen(key) != 16*2){
    WSContentSend_P(HTTP_KEY_ERROR, key);
    WSContentStop();
    return;
  }

  char mac[13] = {0};
  WebGetArg("mac", mac, sizeof(mac));
  if (strlen(mac) != 12){
    WSContentSend_P(HTTP_MAC_ERROR, mac);
    WSContentStop();
    return;
  }

  WSContentSend_P(HTTP_KEY_ADDED, mac, key);

  strlcat(key, mac, sizeof(key));
  MI32AddKey(key, nullptr);

//  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();
}

void MI32TimeoutSensors(){
  // whatever, this function access all the arrays....
  // so block for as long as it takes.

  // PROBLEM: when we take this, it hangs the BLE loop.
  // BUT, devicePresent uses the
  // remove devices for which the adverts have timed out
  for (int i = MIBLEsensors.size()-1; i >= 0; i--) {
    //if (MIBLEsensors[i].MAC[2] || MIBLEsensors[i].MAC[3] || MIBLEsensors[i].MAC[4] || MIBLEsensors[i].MAC[5]){
      // Since we use a pseudo MAC for the ATBTN slots we need to ignore these warnings
      if (!memcmp(MIBLEsensors[i].MAC, ATBTN_Addr, 4)) { // Skip pseudo AT BTN addresses
        continue;
      }
#ifdef USE_SENSOR_ICON
      // Keep devices which have set lastTime to be used in GUI
      if (Rtc.local_time < (MIBLEsensors[i].lastTime + (4 * 24 * 60 * 60))) {  // Keep for 4 days
        continue;
      }
#endif
      if (!BLE_ESP32::devicePresent(MIBLEsensors[i].MAC)){
        AddLog(LOG_LEVEL_DEBUG, PSTR("M32: %s: Dev no longer present"), MIaddrStr(MIBLEsensors[i].MAC));
        TasAutoMutex localmutex(&slotmutex, "Mi32Timeout");
        MIBLEsensors.erase(MIBLEsensors.begin() + i);
      }
    //}
  }
}

// this assumes that we're adding to a ResponseTime_P
void MI32GetOneSensorJson(int slot, int hidename){
  mi_sensor_t *p;
  p = &MIBLEsensors[slot];

  // remove hyphen - make it difficult to configure HASS
  if (!hidename) {
    ResponseAppend_P(PSTR("\"%s%02x%02x%02x\":{"),
          kMI32DeviceType[p->type-1],
          p->MAC[3], p->MAC[4], p->MAC[5]);
  }

  const char *alias = BLE_ESP32::getAlias(p->MAC);
  if (alias && alias[0]){
    ResponseAppend_P(PSTR("\"Alias\":\"%s\","),
          alias);
  }

  ResponseAppend_P(PSTR("\"MAC\":\"%02x%02x%02x%02x%02x%02x\""),
        p->MAC[0], p->MAC[1], p->MAC[2],
        p->MAC[3], p->MAC[4], p->MAC[5]);

  if((!MI32.mode.triggeredTele && !MI32.option.minimalSummary)||MI32.mode.triggeredTele){
    bool tempHumSended = false;
    if(p->feature.tempHum){
      if(p->eventType.tempHum || !MI32.mode.triggeredTele || MI32.option.allwaysAggregate){
        if (!isnan(p->hum) && !isnan(p->temp)
#ifdef USE_HOME_ASSISTANT
          ||(hass_mode!=-1)
#endif //USE_HOME_ASSISTANT
        ) {
          ResponseAppend_P(PSTR(","));
          ResponseAppendTHD(ConvertTempToFahrenheit(p->temp), p->hum);  // convert if SO8 on
          tempHumSended = true;
        }
      }
    }
    if(p->feature.temp && !tempHumSended){
      if(p->eventType.temp || !MI32.mode.triggeredTele || MI32.option.allwaysAggregate) {
        if (!isnan(p->temp)
#ifdef USE_HOME_ASSISTANT
          ||(hass_mode!=-1)
#endif //USE_HOME_ASSISTANT
        ) {
          float temp = ConvertTempToFahrenheit(p->temp); // convert if SO8 on
          ResponseAppend_P(PSTR(",\"" D_JSON_TEMPERATURE "\":%*_f"),
            Settings->flag2.temperature_resolution, &temp);
        }
      }
    }
    if(p->feature.hum && !tempHumSended){
      if(p->eventType.hum || !MI32.mode.triggeredTele || MI32.option.allwaysAggregate) {
        if (!isnan(p->hum)
#ifdef USE_HOME_ASSISTANT
          ||(hass_mode!=-1)
#endif //USE_HOME_ASSISTANT
        ) {
          char hum[FLOATSZ];
          dtostrfd(p->hum, Settings->flag2.humidity_resolution, hum);
          ResponseAppend_P(PSTR(",\"" D_JSON_HUMIDITY "\":%s"), hum);
        }
      }
    }

    if (p->feature.pres){
      if(p->eventType.pres || !MI32.mode.triggeredTele || MI32.option.allwaysAggregate
#ifdef USE_HOME_ASSISTANT
          ||(hass_mode==2)
#endif //USE_HOME_ASSISTANT
      ){
        float fpressure = ConvertPressure(p->pres);
        ResponseAppend_P(PSTR(",\"" D_JSON_PRESSURE "\":%*_f"), Settings->flag2.pressure_resolution, &fpressure);
      }
    }

    if (p->feature.lux){
      if(p->eventType.lux || !MI32.mode.triggeredTele || MI32.option.allwaysAggregate){
#ifdef USE_HOME_ASSISTANT
        if ((hass_mode != -1) && (p->lux == 0x0ffffff)) {
          ResponseAppend_P(PSTR(",\"" D_JSON_ILLUMINANCE "\":null"));
        } else
#endif //USE_HOME_ASSISTANT
        if ((p->lux != 0x0ffffff)
#ifdef USE_HOME_ASSISTANT
          || (hass_mode != -1)
#endif //USE_HOME_ASSISTANT
        ) { // this is the error code -> no lux
          ResponseAppend_P(PSTR(",\"" D_JSON_ILLUMINANCE "\":%u"), p->lux);
        }
      }
    }
    if (p->feature.light){
      if(p->eventType.light || !MI32.mode.triggeredTele || MI32.option.allwaysAggregate
#ifdef USE_HOME_ASSISTANT
          ||(hass_mode==2)
#endif //USE_HOME_ASSISTANT
      ){
        ResponseAppend_P(PSTR(",\"Light\":%d"), p->light);
      }
    }
    if (p->feature.moist){
      if(p->eventType.moist || !MI32.mode.triggeredTele || MI32.option.allwaysAggregate){
#ifdef USE_HOME_ASSISTANT
        if ((hass_mode != -1) && (p->moisture == 0xff)) {
          ResponseAppend_P(PSTR(",\"" D_JSON_MOISTURE "\":null"));
        } else
#endif //USE_HOME_ASSISTANT
        if ((p->moisture != 0xff)
#ifdef USE_HOME_ASSISTANT
          || (hass_mode != -1)
#endif //USE_HOME_ASSISTANT
        ) {
          ResponseAppend_P(PSTR(",\"" D_JSON_MOISTURE "\":%u"), p->moisture);
        }
      }
    }
    if (p->feature.fert){
      if(p->eventType.fert || !MI32.mode.triggeredTele || MI32.option.allwaysAggregate){
#ifdef USE_HOME_ASSISTANT
        if ((hass_mode != -1) && (p->fertility == 0xffff)) {
          ResponseAppend_P(PSTR(",\"Fertility\":null"));
        } else
#endif //USE_HOME_ASSISTANT
        if ((p->fertility != 0xffff)
#ifdef USE_HOME_ASSISTANT
          || (hass_mode != -1)
#endif //USE_HOME_ASSISTANT
        ) {
          ResponseAppend_P(PSTR(",\"Fertility\":%u"), p->fertility);
        }
      }
    }
    if (p->feature.scale){
      if(p->eventType.scale || !MI32.mode.triggeredTele || MI32.option.allwaysAggregate
#ifdef USE_HOME_ASSISTANT
          ||(hass_mode==2)
#endif //USE_HOME_ASSISTANT
      ){
        if (MI32.option.directBridgeMode) {
          ResponseAppend_P(PSTR(",\"WeightRemoved\":%u"), p->weight_removed);
          ResponseAppend_P(PSTR(",\"WeightStabilized\":%u"), p->weight_stabilized);
        }
        ResponseAppend_P(PSTR(",\"WeightUnit\":\"%s\""), p->weight_unit);
        ResponseAppend_P(PSTR(",\"Weight\":%*_f"),
          Settings->flag2.weight_resolution, &p->weight);
        if (p->feature.impedance) {
          if (MI32.option.directBridgeMode) {
            ResponseAppend_P(PSTR(",\"ImpedanceStabilized\":%u"), p->impedance_stabilized);
          }
          ResponseAppend_P(PSTR(",\"Impedance\":%u"), p->impedance);
        }
      }
    }
    if (p->feature.accel){
      if(p->eventType.accel || !MI32.mode.triggeredTele || MI32.option.allwaysAggregate
#ifdef USE_HOME_ASSISTANT
          ||(hass_mode==2)
#endif //USE_HOME_ASSISTANT
      ){
        ResponseAppend_P(PSTR(",\"Acceleration\":"));
        for (uint32_t i = 0; i <= p->accel_used; i++) {
          ResponseAppend_P(PSTR("%c%6_f"), (!i)?'[':',', &p->acceleration[i]);
        }
        ResponseAppend_P(PSTR("]"));
      }
    }
    if (p->feature.Btn){
      if(p->eventType.Btn || !MI32.mode.triggeredTele || MI32.option.allwaysAggregate
#ifdef USE_HOME_ASSISTANT
          ||(hass_mode==2)
#endif //USE_HOME_ASSISTANT
      ){
        if ((p->type == MI_BTHOME) && (p->button[1] != 0)){
          for (uint32_t i = 0; i < sizeof(p->button); i++) {
            if (p->button[i] > 0) {
              uint32_t button = (255 == p->button[i]) ? 0 : p->button[i];
              ResponseAppend_P(PSTR(",\"Btn%d\":%d"), i +1, button);
            }
          }
        } else {
          ResponseAppend_P(PSTR(",\"Btn\":%d"),p->Btn);
        }
      }
    }
    if (p->feature.flooding){
      if(p->eventType.flooding || !MI32.mode.triggeredTele || MI32.option.allwaysAggregate
#ifdef USE_HOME_ASSISTANT
          ||(hass_mode==2)
#endif //USE_HOME_ASSISTANT
      ){
        ResponseAppend_P(PSTR(",\"Flooding\":%d"),p->flooding);
      }
    }
    if(p->eventType.PairBtn && p->pairing){
        ResponseAppend_P(PSTR(",\"Pair\":%u"),p->pairing);
    }

    if (p->feature.volt){
      if(p->eventType.volt || !MI32.mode.triggeredTele || MI32.option.allwaysAggregate
#ifdef USE_HOME_ASSISTANT
          ||(hass_mode==2)
#endif //USE_HOME_ASSISTANT
      ){
        ResponseAppend_P(PSTR(",\"" D_JSON_VOLTAGE "\":%*_f"), Settings->flag2.voltage_resolution, &p->volt);
      }
    }

  } // minimal summary

  if ((p->type == MI_BTHOME) && p->eventType.event) {
    for (uint32_t i = 0; i < sizeof(p->bthome_event); i++) {
      if (p->bthome_event[i] > 0) {
        ResponseAppend_P(PSTR(",\"Evt%d\":%d"),
          p->bthome_event[i] & 0x7F, p->bthome_event[i] >> 7);
      }
    }
  }

  if (p->feature.PIR){
    if(p->eventType.motion || !MI32.mode.triggeredTele){
      if(MI32.mode.triggeredTele) ResponseAppend_P(PSTR(",\"PIR\":1")); // only real-time
      ResponseAppend_P(PSTR(",\"Events\":%u"),p->events);
    }
    else if(p->eventType.noMotion && MI32.mode.triggeredTele){
      ResponseAppend_P(PSTR(",\"PIR\":0"));
    }
  }

  if (p->type == MI_FLORA && !MI32.mode.triggeredTele) {
    if (p->firmware[0] != '\0') { // this is the error code -> no firmware
      ResponseAppend_P(PSTR(",\"Firmware\":\"%s\""), p->firmware);
    }
  }

  if (p->feature.NMT || !MI32.mode.triggeredTele){
    if(p->eventType.NMT){
      ResponseAppend_P(PSTR(",\"NMT\":%u"), p->NMT);
    }
  }

  if (p->feature.count){
    if(p->eventType.count){
        ResponseAppend_P(PSTR(",\"" D_JSON_COUNT "\":%d"), p->count);
    }
  }

  if ((p->type == MI_BTHOME) && p->feature.unparsed) {
    for (uint32_t i = 0; i < sizeof(p->unparsed); i++) {
      if (p->unparsed[i] > 0) {
        uint32_t length;
        uint32_t format;
        uint32_t factor;
        if (BTHomeGetObjectData(p->unparsed[i], length, format, factor)) {
          ResponseAppend_P(PSTR(",\"Obj%d\":%*_f"),
            p->unparsed[i], factor, &p->unparsed_float[i]);
        }
      }
    }
  }

  if (p->feature.bat){
    if(p->eventType.bat || !MI32.mode.triggeredTele || MI32.option.allwaysAggregate){
#ifdef USE_HOME_ASSISTANT
      if ((hass_mode != -1) && (p->bat == 0x00)) {
        ResponseAppend_P(PSTR(",\"Battery\":null"));
      } else
#endif //USE_HOME_ASSISTANT
      if ((p->bat != 0x00)
#ifdef USE_HOME_ASSISTANT
        || (hass_mode != -1)
#endif //USE_HOME_ASSISTANT
      ) {
        ResponseAppend_P(PSTR(",\"Battery\":%u"), p->bat);
      }
    }
  }
  if (MI32.option.showRSSI) ResponseAppend_P(PSTR(",\"RSSI\":%d"), p->RSSI);

  if (!hidename) {
    ResponseAppend_P(PSTR("}"));
  }
  p->eventType.raw = 0;  // Reset all events
  p->shallSendMQTT = 0;

}


///////////////////////////////////////////////
// starts a completely fresh MQTT message.
// sends up to 4 sensors
// triggered by setting MI32.mqttCurrentSlot = 0
void MI32ShowSomeSensors(){
  // don't detect half-added ones here
  int numsensors = MIBLEsensors.size();
  if (MI32.mqttCurrentSlot >= numsensors){
    // if we got to the end of the sensors, then don't send more
    return;
  }

#ifdef USE_HOME_ASSISTANT
  bool _noSummarySave = MI32.option.noSummary;
  bool _minimalSummarySave = MI32.option.minimalSummary;
  if(hass_mode==2){
    if(MI32.option.holdBackFirstAutodiscovery){
      if(!MI32.mode.firstAutodiscoveryDone){
        MI32.mode.firstAutodiscoveryDone = 1;
        return;
      }
    }
    MI32.option.noSummary = false;
    MI32.option.minimalSummary = false;
  }
#endif //USE_HOME_ASSISTANT

  ResponseTime_P(PSTR(""));
  int cnt = 0;
  int maxcnt = 4;
  mi_sensor_t *p;
  for (; (MI32.mqttCurrentSlot < numsensors) && (cnt < maxcnt); MI32.mqttCurrentSlot++, cnt++) {
    ResponseAppend_P(PSTR(","));
    p = &MIBLEsensors[MI32.mqttCurrentSlot];

    MI32GetOneSensorJson(MI32.mqttCurrentSlot, (maxcnt == 1));
    int mlen = ResponseLength();

    // if we ran out of room, leave here.
    if (ResponseSize() - mlen < 100){
      MI32.mqttCurrentSlot++;
      break;
    }
    cnt++;
  }
  MqttAppendSensorUnits();
  ResponseAppend_P(PSTR("}"));
  //AddLog(LOG_LEVEL_DEBUG,PSTR("M32: show some %d %s"), MI32.mqttCurrentSlot, ResponseData());
  MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR(D_RSLT_SENSOR), Settings->flag.mqtt_sensor_retain);

#ifdef USE_HOME_ASSISTANT
  if(hass_mode==2){
    MI32.option.noSummary = _noSummarySave;
    MI32.option.minimalSummary = _minimalSummarySave;
  }
#endif //USE_HOME_ASSISTANT
}


///////////////////////////////////////////////
// starts a completely fresh MQTT message.
// sends ONE sensor on a dedicated topic NOT related to this TAS
// triggered by setting MI32.mqttCurrentSingleSlot = 0
void MI32ShowOneMISensor(bool hidename){
  // don't detect half-added ones here
  int numsensors = MIBLEsensors.size();
  if (MI32.mqttCurrentSingleSlot >= numsensors){
    // if we got to the end of the sensors, then don't send more
    return;
  }

  if(
#ifdef USE_HOME_ASSISTANT
    Settings->flag.hass_discovery
    ||
#endif //USE_HOME_ASSISTANT
    MI32.option.MQTTType > 0
    ){

    ResponseTime_P(PSTR(","));
    MI32GetOneSensorJson(MI32.mqttCurrentSingleSlot, hidename);
    mi_sensor_t *p;
    p = &MIBLEsensors[MI32.mqttCurrentSingleSlot];

    if (ResponseContains_P(PSTR(D_JSON_TEMPERATURE))) {
      ResponseAppend_P(PSTR(",\"" D_JSON_TEMPERATURE_UNIT "\":\"%c\""), TempUnit());
    }
    ResponseAppend_P(PSTR("}"));

    char idstr[32];
    const char *alias = BLE_ESP32::getAlias(p->MAC);
    const char *id = idstr;
    if (alias && *alias){
      id = alias;
    } else {
      sprintf(idstr, PSTR("%s%02x%02x%02x"),
            kMI32DeviceType[p->type-1],
            p->MAC[3], p->MAC[4], p->MAC[5]);
    }
    char SensorTopic[TOPSZ];
    GetTopic_P(SensorTopic, TELE, MI32.bleTopic.c_str(), id);

    //AddLog(LOG_LEVEL_DEBUG,PSTR("M32: show one %d %s"), MI32.mqttCurrentSlot, ResponseData());
    MqttPublish(SensorTopic, Settings->flag.mqtt_sensor_retain);
    if (MI32.option.MQTTType > 1)
      XdrvRulesProcess(0);
  }
  MI32.mqttCurrentSingleSlot++;
}

///////////////////////////////////////////////
// starts a completely fresh MQTT message.
// sends ONE sensor's worth of HA discovery msg
const char MI_HA_DISCOVERY_TEMPLATE[] PROGMEM =
  "{\"availability\":[],\"device\":"
  "{\"identifiers\":[\"BLE%s\"],"
  "\"name\":\"%s\","
  "\"manufacturer\":\"tas\","
  "\"model\":\"%s\","
  "\"via_device\":\"%s\""
  "},"
  "%s%s%s"
  "\"expire_after\":600,"
  "\"json_attr_t\":\"%s\","
  "\"name\":\"%s_%s\","
  "\"state_topic\":\"%s\","
  "\"uniq_id\":\"%s_%s\","
  "%s%s%s"
  "\"val_tpl\":\"{{ %s%s%s }}\"}";

// careful - a missing comma causes a crash!!!!
// because of the way we loop?
const char *classes[] = {
  // 0
  "temperature",
  "Temperature",
  "°C",

  // 1
  "humidity",
  "Humidity",
  "%",

  // 2
  "temperature",
  "DewPoint",
  "°C",

  // 3
  "battery",
  "Battery",
  "%",

  // 4
  "signal_strength",
  "RSSI",
  "dB",

  // 5
  "",//- empty device class
  "Btn",
  "",

  // 6
  "", //- empty device class
  "Light",
  "",

  // 7
  "", //- empty device class
  "Moisture",
  "%",

  // 8
  "", //- empty device class
  "Illuminance",
  "lx",

  // 9
  "", //- empty device class
  "Fertility",
  "µS/cm",

  // 10
  "", //- empty device class
  "Firmware",
  "",

  // 11
  "", //- empty device class
  "Weight",
  "", // Will be set to p->weight_unit

  // 12
  "", //- empty device class
  "Impedance",
  "Ω",
};

void MI32DiscoveryOneMISensor(){
  // don't detect half-added ones here
  int numsensors = MIBLEsensors.size();
  if (MI32.mqttCurrentSingleSlot >= numsensors){
    // if we got to the end of the sensors, then don't send more
    return;
  }

#ifdef USE_HOME_ASSISTANT
  if(Settings->flag.hass_discovery){
    mi_sensor_t *p;
    p = &MIBLEsensors[MI32.mqttCurrentSingleSlot];

    int datacount = (sizeof(classes)/sizeof(*classes))/3;

    if (p->nextDiscoveryData >= datacount){
      p->nextDiscoveryData = 0;
    }

    char DiscoveryTopic[80];
    const char *host = NetworkHostname();
    const char *devtype = kMI32DeviceType[p->type-1];
    char idstr[32];
    const char *alias = BLE_ESP32::getAlias(p->MAC);
    const char *id = idstr;
    if (alias && *alias){
      id = alias;
    } else {
      sprintf(idstr, PSTR("%s%02x%02x%02x"),
            devtype,
            p->MAC[3], p->MAC[4], p->MAC[5]);
    }

    char SensorTopic[TOPSZ];
    GetTopic_P(SensorTopic, TELE, MI32.bleTopic.c_str(), id);

    //int i = p->nextDiscoveryData*3;
    for (int i = 0; i < datacount*3; i += 3){
      if (!classes[i] || !classes[i+1] || !classes[i+2]){
        return;
      }
      uint8_t isBinary = 0;

      ResponseClear();

      switch(i/3){
        case 0: // temp
          if (!p->feature.temp && !p->feature.tempHum){
            continue;
          }
          break;
        case 1:// hum
          if (!p->feature.hum && !p->feature.tempHum){
            continue;
          }
          break;
        case 2: //dew
          if (!p->feature.tempHum && !(p->feature.temp && p->feature.hum)){
            continue;
          }
          break;

        case 3: //bat
          if (!p->feature.bat){
            continue;
          }
          break;

        case 4: //rssi - all
          break;

        case 5: // button
          if (!p->feature.Btn){
            continue;
          }
          //isBinary = 2; // invert payload
          break;

        case 6: // binary light sense
          if (!p->feature.light){
            continue;
          }
          //isBinary = 1;
          break;
        case 7: // moisture
          if (!p->feature.moist){
            continue;
          }
          //isBinary = 1;
          break;
        case 8: // lux
          if (!p->feature.lux){
            continue;
          }
          break;
        case 9: // fertility
          if (!p->feature.fert){
            continue;
          }
          break;
        case 10: // firmware
          if (!p->feature.fert){ // Flora only
            continue;
          }
          break;
        case 11: // weight
          if (!p->feature.scale){ // Mi Scale V1 and V2 only
            continue;
          }
          break;
        case 12: // impedance
          if (!p->feature.impedance){ // Mi Scale V2 only
            continue;
          }
          break;
      }

    /*
    {"availability":[],"device":{"identifiers":["TasmotaBLEa4c1387fc1e1"],"manufacturer":"simon","model":"someBLEsensor","name":"TASBLEa4c1387fc1e1","sw_version":"0.0.0"},"dev_cla":"temperature","json_attr_t":"tele/tasmota_esp32/SENSOR","name":"TASLYWSD037fc1e1Temp","state_topic":"tele/tasmota_esp32/SENSOR","uniq_id":"Tasmotaa4c1387fc1e1temp","unit_of_meas":"°C","val_tpl":"{{ value_json.LYWSD037fc1e1.Temperature }}"}
    {"availability":[],"device":{"identifiers":["TasmotaBLEa4c1387fc1e1"],
    "name":"TASBLEa4c1387fc1e1"},"dev_cla":"temperature",
    "json_attr_t":"tele/tasmota_esp32/SENSOR",
    "name":"TASLYWSD037fc1e1Temp","state_topic":  "tele/tasmota_esp32/SENSOR",
    "uniq_id":"Tasmotaa4c1387fc1e1temp","unit_of_meas":"°C",
    "val_tpl":"{{ value_json.LYWSD037fc1e1.Temperature }}"}
    */

      ResponseAppend_P(MI_HA_DISCOVERY_TEMPLATE,
      //"{\"identifiers\":[\"BLE%s\"],"
        id,
      //"\"name\":\"%s\"},"
        id,
      //\"model\":\"%s\",
        devtype,
      //\"via_device\":\"%s\"
        host,
      //"\"dev_cla\":\"%s\","
      (classes[i][0]?"\"dev_cla\":\"":""),
        classes[i],
      (classes[i][0]?"\",":""),
      //"\"json_attr_t\":\"%s\"," - the topic the sensor publishes on
        SensorTopic,
      //"\"name\":\"%s_%s\"," - the name of this DATA
        id, classes[i+1],
      //"\"state_topic\":\"%s\","  - the topic the sensor publishes on?
        SensorTopic,
      //"\"uniq_id\":\"%s_%s\"," - unique for this data,
        id, classes[i+1],
      //"\"unit_of_meas\":\"%s\"," - the measure of this type of data
        ((i/3==11)||classes[i+2][0]?"\"unit_of_meas\":\"":""),
        (i/3==11)?p->weight_unit:classes[i+2],
        ((i/3==11)||classes[i+2][0]?"\",":""),
      //"\"val_tpl\":\"{{ %s%s }}") // e.g. Temperature
      // inverted binary - {{ 'off' if value_json.posn else 'on' }}
      // binary - {{ 'on' if value_json.posn else 'off' }}

        ((isBinary < 1)?"value_json.":
          ((isBinary < 2)?"value_json.":"'off' if value_json.")
        ),
        classes[i+1],
        ((isBinary < 1)?"":
          ((isBinary < 2)?"":" else 'on'")
        )

          //
      );

      sprintf(DiscoveryTopic, "homeassistant/%ssensor/%s/%s/config",
        (isBinary? "binary_":""), id, classes[i+1]);

      MqttPublish(DiscoveryTopic);
      p->nextDiscoveryData++;
      //vTaskDelay(100/ portTICK_PERIOD_MS);
    }
  } // end if hass discovery
  //AddLog(LOG_LEVEL_DEBUG,PSTR("M32: show some %d %s"), MI32.mqttCurrentSlot, ResponseData());
#endif //USE_HOME_ASSISTANT
}

///////////////////////////////////////////////
// starts a completely fresh MQTT message.
// sends up to 4 sensors pe5r msg
// sends only those which are raw and triggered.
// triggered by setting MI32.mode.triggeredTele = 1
void MI32ShowTriggeredSensors(){
  if (!MI32.mode.triggeredTele) return; // none to show
  MI32.mode.triggeredTele = 0;

  // don't detect half-added ones here
  int numsensors = MIBLEsensors.size();

  int sensor = 0;

  int maxcnt = 4;
  bool hidename = false;
  if(
#ifdef USE_HOME_ASSISTANT
    Settings->flag.hass_discovery
    ||
#endif //USE_HOME_ASSISTANT
    MI32.option.MQTTType > 0
    ){
    maxcnt = 1;
    hidename = MI32.option.MQTTType < 2;
  }

  do {
    ResponseTime_P(PSTR(""));
    int cnt = 0;
    mi_sensor_t *p;
    for (; (sensor < numsensors) && (cnt < maxcnt); sensor++) {
      p = &MIBLEsensors[sensor];
      if(p->eventType.raw == 0) continue;
      if(p->shallSendMQTT == 0) continue;

      cnt++;
      ResponseAppend_P(PSTR(","));
      // hide sensor name if HASS or option6
      MI32GetOneSensorJson(sensor, hidename);
      int mlen = ResponseLength();

      // if we ran out of room, leave here.
      if (ResponseSize() - mlen < 100){
        sensor++;
        break;
      }
    }
    if (cnt){ // if we got one, then publish
      MqttAppendSensorUnits();
      ResponseAppend_P(PSTR("}"));
      if(
    #ifdef USE_HOME_ASSISTANT
        Settings->flag.hass_discovery
        ||
    #endif //USE_HOME_ASSISTANT
        MI32.option.MQTTType > 0
        ){
        char SensorTopic[TOPSZ];
        char idstr[32];
        const char *alias = BLE_ESP32::getAlias(p->MAC);
        const char *id = idstr;
        if (alias && *alias){
          id = alias;
        } else {
          snprintf_P(idstr, sizeof(idstr), PSTR("%s%02x%02x%02x"),
                kMI32DeviceType[p->type-1],
                p->MAC[3], p->MAC[4], p->MAC[5]);
        }
        GetTopic_P(SensorTopic, TELE, MI32.bleTopic.c_str(), id);
        //AddLog(LOG_LEVEL_DEBUG, PSTR("M32: triggered %d %s"), sensor, ResponseData());
        MqttPublish(SensorTopic, Settings->flag.mqtt_sensor_retain);
        XdrvRulesProcess(0);
      } else {
        //AddLog(LOG_LEVEL_DEBUG, PSTR("M32: triggered2 %d %s"), sensor, ResponseData());
        MqttPublishPrefixTopicRulesProcess_P(STAT, PSTR(D_RSLT_SENSOR), Settings->flag.mqtt_sensor_retain);
      }

    } else { // else don't and clear
      ResponseClear();
    }
  } while (sensor < numsensors);
}

#ifdef USE_SENSOR_ICON
const char SI_WEB_CSS[] PROGMEM =
  "</table>"         // Terminate current two column table...
  "<style>"          // Send STYLE for icon sensors
  // Table CSS
  ".itd td:not(:first-child){width:20px;font-size:70%%}"
  ".itd td:last-child{width:45px}"
  ".itd .bt{margin-right:10px;}" // Margin right should be half of the not-first width
  ".itr{line-height:20px}"
  // Signal Strength Indicator
  ".si{display:inline-flex;align-items:flex-end;height:15px;padding:0;"
  "i{width:3px;margin-right:1px;border-radius:3px;background-color:var(--c_txt)}"
  ".b0{height:25%%}.b1{height:50%%}.b2{height:75%%}.b3{height:100%%}}.o30{opacity:.3}"
  "</style>"
  "{t}";             // ... and open new table for devices

const char SI_WEB_STATUS_LINE[] PROGMEM =
  "<tr class='itd itr'>"
  "<td><b title='0x%6_H'>%s</b></td>" // name
  "<td>%s</td>" // sbatt (Battery Indicator)
  "<td><div title='RSSI %s' class='si'>"; // slqi

const char SI_WEB_BATTERY[] PROGMEM =
  "<div title='%s%d%%%s'><i class='bt' style='--bl:%dpx;color:#%02x%02x%02x'></i></div>";  // Need div for touch media tooltip to work

const char SI_WEB_LAST_SEEN[] PROGMEM =
  "<td style='color:#%02x%02x%02x'>&#x1F557;%02d%c";

const char SI_WEB_LINE_START[] PROGMEM =
  "<tr class='htr'><td colspan=4>&#9478;";

const char SI_WEB_END_STATUS[] PROGMEM =
  "</div></td>" // Close RSSI
  "%s{e}"; // dhm (Last Seen)

const char SI_WEB_LINE_END[] PROGMEM =
  "</table>{t}";     // Terminate device table and open a new one for buttons and output of following drivers

const char HTTP_MI32_VERSION[] PROGMEM =
  "<div style='text-align:right;font-size:11px;color:#aaa;'>MI ESP32 " MI32_VERSION "</div>";

// Get Alias or Type name
const char *MI32DeviceName(char *name, uint32_t name_size, uint32_t index) {
  const char *label = BLE_ESP32::getAlias(MIBLEsensors[index].MAC);
  bool valid_alias = (label && *label);
  if (!valid_alias) {
    const char *typeName = kMI32DeviceType[MIBLEsensors[index].type-1];
    ext_snprintf_P(name, name_size, PSTR("%s%3_H"), typeName, MIBLEsensors[index].MAC +3);
    label = name;
  }
  return label;
}

// Comparator function used to sort MI32 devices by alphabetical order
int MI32DeviceCmp(uint8_t a, uint8_t b) {
  char name_a[32];
  const char *label_a = MI32DeviceName(name_a, sizeof(name_a), a);
  char name_b[32];
  const char *label_b = MI32DeviceName(name_b, sizeof(name_b), b);
  return strcasecmp(label_a, label_b);
}

// Convert seconds to a string representing days, hours or minutes present in the n-value.
// The string will contain the most coarse time only, rounded down (61m == 01h, 01h37m == 01h).
// Inputs:
// - seconds: uint32_t representing some number of seconds
// Outputs:
// - char for unit (d for day, h for hour, m for minute)
// - the hex color to be used to display the text
//
uint32_t SIconvert_seconds_to_dhm(uint32_t seconds,  char *unit, uint32_t *color, bool days = false){
  static const uint32_t conversions[3] = {24 * 3600, 3600, 60};
  static const char     units[3] = { 'd', 'h', 'm'};   // day, hour, minute
  static const uint32_t color_threshold_hours[2] = {24 * 3600, 3600};               // 0 - 1 hour - 1 day
  static const uint32_t color_threshold_days[2] = {7 * 24 * 3600, 2 * 24 * 3600};    // 0 - 2 days - 7 days

  uint32_t color_text_8 = WebColor(COL_TEXT);    // color of text on 8 bits
  uint8_t color_text_8_r = (color_text_8 & 0xFF0000) >> 16;
  uint8_t color_text_8_g = (color_text_8 & 0x00FF00) >> 8;
  uint8_t color_text_8_b = (color_text_8 & 0x0000FF);

  uint32_t color_back_8 = WebColor(COL_BACKGROUND);    // color of background on 8 bits
  uint8_t color_back_8_r = (color_back_8 & 0xFF0000) >> 16;
  uint8_t color_back_8_g = (color_back_8 & 0x00FF00) >> 8;
  uint8_t color_back_8_b = (color_back_8 & 0x0000FF);

  int32_t colors[3] = {
    ((changeUIntScale( 6, 0, 16, color_back_8_r, color_text_8_r) & 0xFF) << 16U) |   //  6/16 of text
    ((changeUIntScale( 6, 0, 16, color_back_8_g, color_text_8_g) & 0xFF) <<  8U) |   //  6/16 of text
    ( changeUIntScale( 6, 0, 16, color_back_8_b, color_text_8_r) & 0xFF),            //  6/16 of text

    ((changeUIntScale(10, 0, 16, color_back_8_r, color_text_8_r) & 0xFF) << 16U) |   // 10/16 of text
    ((changeUIntScale(10, 0, 16, color_back_8_g, color_text_8_g) & 0xFF) <<  8U) |   // 10/16 of text
    ( changeUIntScale(10, 0, 16, color_back_8_b, color_text_8_r) & 0xFF),            // 10/16 of text

    (color_text_8_r << 16U) |
    (color_text_8_g <<  8U) |
    (color_text_8_b)
  };

  *color = (uint32_t)colors[2];
  for (uint32_t i = 0; i < 2; i++) {
    if (seconds > (days ? color_threshold_days[i] : color_threshold_hours[i])) {
      *color = (uint32_t)colors[i];
      break;
    }
  }
  for(uint32_t i = 0; i < 3; ++i) {
    *unit = units[i];
    if (seconds > conversions[i]) {    // always pass even if 00m
      return seconds / conversions[i];
    }
  }
  return 0;
}

#endif  // USE_SENSOR_ICON

void MI32Show(bool json)
{
  // don't detect half-added ones here
  int numsensors = MIBLEsensors.size();

  if (json) {
    // TELE JSON messages now do nothing here, apart from set MI32.mqttCurrentSlot
    // which will trigger send next second of up to 4 sensors, then the next four in the next second, etc.
    //MI32.mqttCurrentSlot = 0;

#ifdef USE_WEBSERVER
  } else {
    if (!Settings->flag5.mi32_enable) return;

#ifdef USE_SENSOR_ICON
    if (numsensors > 255) {
      numsensors = 255;  // Display max 255 sensors
    }
    // sort elements by alias and type name
    uint8_t sorted_idx[numsensors];
    for (uint32_t i = 0; i < numsensors; i++) {
      sorted_idx[i] = i;
    }
    // insertion sort
    for (uint32_t i = 1; i < numsensors; i++) {
      uint8_t key = sorted_idx[i];
      uint8_t j = i;
      while ((j > 0) && (MI32DeviceCmp(sorted_idx[j - 1], key) > 0)) {
        sorted_idx[j] = sorted_idx[j - 1];
        j--;
      }
      sorted_idx[j] = key;
    }

    uint32_t now = Rtc.local_time;

    WSContentSend_P(SI_WEB_CSS);

    // Iterate through devices by alphabetical order
    for (uint32_t i = 0; i < numsensors; i++) {
      mi_sensor_t *p;
      p = &MIBLEsensors[sorted_idx[i]];

      char name[32];
      const char *label = MI32DeviceName(name, sizeof(name), sorted_idx[i]);

      char sbatt[106];
      char dhm[48];
      snprintf_P(sbatt, sizeof(sbatt), PSTR("&nbsp;"));
      if (p->bat != 0x00) {
        char bv[10] = { 0 };
        if (p->feature.volt) {
          ext_snprintf_P(bv, sizeof(bv), PSTR("%*_fV, "), Settings->flag2.voltage_resolution, &p->volt);
        }

        char unit;
        uint32_t color = WebColor(COL_TEXT);    // color of text
        dhm[0] = 0;   // start with empty string
/*
        if (p->batLastSeen) {
          uint16_t val = SIconvert_seconds_to_dhm(now - p->batLastSeen, &unit, &color, true);
          if (val < 100) {
            snprintf_P(dhm, sizeof(dhm), PSTR(" (%02d%c)"), val, unit);
          }
        }
*/
        snprintf_P(sbatt, sizeof(sbatt), SI_WEB_BATTERY,
          bv, p->bat, dhm,
          changeUIntScale(p->bat, 0, 100, 0, 14),
          (color & 0xFF0000) >> 16, (color & 0x00FF00) >> 8, (color & 0x0000FF)
        );
      }

      uint32_t rssi_as_quality = WifiGetRssiAsQuality(p->RSSI);
      uint32_t num_bars = changeUIntScale(rssi_as_quality, 0, 100, 0, 4);
      char rssi[16];
      snprintf_P(rssi, sizeof(rssi), PSTR("%d (%d%%)"), p->RSSI, rssi_as_quality);

      // New line: Device name, battery state, RSSI and last seen
      WSContentSend_PD(SI_WEB_STATUS_LINE,
        p->MAC,
        HtmlEscape(label).c_str(), sbatt, rssi);

      for(uint32_t j = 0; j < 4; j++) {
        WSContentSend_P(PSTR("<i class='b%d%s'></i>"), j, (j >= num_bars) ? PSTR(" o30") : PSTR(""));
      }
      snprintf_P(dhm, sizeof(dhm), PSTR("<td>&nbsp;"));

      if (p->lastTime != 0) {
        char unit;
        uint32_t color;
        uint16_t val = SIconvert_seconds_to_dhm(now - p->lastTime, &unit, &color);
        if (val < 100) {
          snprintf_P(dhm, sizeof(dhm), SI_WEB_LAST_SEEN,                         
                                       (color & 0xFF0000) >> 16, (color & 0x00FF00) >> 8, (color & 0x0000FF),
                                       val, unit);
        }
      }

      WSContentSend_PD(SI_WEB_END_STATUS, dhm);

/*
      // Icon test line
      WSContentSend_P(SI_WEB_LINE_START);
      WSContentSend_PD(PSTR(" 1'&#x1F3C3;' 2'&#x270B;' 3'&#x26C5;' 4'&#x1F680;' 5'&#x1F30A;' 6'&#x2696;' 7'&#x2126;' 8'&#x1FAD8'"));
      WSContentSend_P(PSTR("{e}"));
*/
      // Optional new line: Sensors
      if (!isnan(p->temp) || !isnan(p->hum) ||
           p->feature.pres || p->feature.accel || p->feature.flooding || p->feature.scale || 
           p->feature.NMT || p->feature.events || p->feature.count ||
          (p->feature.moist && (p->moisture != 0xff)) ||
          (p->feature.fert && (p->fertility != 0xffff)) ||
          (p->feature.lux && (p->lux!=0x00ffffff))
         ) {
        WSContentSend_P(SI_WEB_LINE_START);
        if (!isnan(p->temp)) {
          float temp = ConvertTempToFahrenheit(p->temp); // convert if SO8 on
          WSContentSend_PD(PSTR(" &#x2600;&#xFE0F; %*_f" D_UNIT_DEGREE "%c"), Settings->flag2.temperature_resolution, &temp, TempUnit());  // ☀ Sun
        }
        if (!isnan(p->hum)) {
          WSContentSend_PD(PSTR(" &#x1F4A7; %*_f%%"), Settings->flag2.humidity_resolution, &p->hum);  // 💧
        }
        if (!isnan(p->temp) && !isnan(p->hum)) {
          float dewpoint = CalcTempHumToDew(p->temp, p->hum);
          WSContentSend_PD(PSTR(" %*_f" D_UNIT_DEGREE "%c"), Settings->flag2.temperature_resolution, &dewpoint, TempUnit());
        }
        if (p->feature.moist && (p->moisture != 0xff)) {
          WSContentSend_PD(PSTR(" &#x1F4A6; %d%%"), p->moisture);  // 💦
        }
        if (p->feature.fert && (p->fertility != 0xffff)) {  // Electrical conductivity measuring salt or fertilizer
          WSContentSend_PD(PSTR(" &#x1F340; %u" D_UNIT_MICROSIEMENS_PER_CM), p->fertility);  // 🍀
        }
        if (p->feature.pres) {
          float pressure = ConvertPressure(p->pres);  // SetOption24 - Switch between hPa or mmHg pressure unit, SetOption139 - Switch between mmHg or inHg pressure unit
          WSContentSend_PD(PSTR(" &#x26C5; %*_f%s"), Settings->flag2.pressure_resolution, &pressure, PressureUnit().c_str());  // ⛅
        }
        if (p->feature.events){
          WSContentSend_PD(PSTR(" &#x1F3C3; %u"), p->events);  // Number of events / alarms - 🏃 Runner
        }
        if (p->feature.NMT){
          // no motion time
          if (p->NMT > 0) {
            WSContentSend_PD(PSTR(" &#x270B; %usec"), p->NMT);  // No motion time in seconds - ✋ hand stop
          }
        }
        if (p->feature.lux) {
          if (p->lux!=0x00ffffff) { // this is the error code -> no valid value
            WSContentSend_PD(PSTR(" &#x1F506; %u" D_UNIT_LUX), p->lux);  // 🔆 bright button
          }
        }
        if (p->feature.scale){
          WSContentSend_PD(PSTR(" &#x2696; %*_f%s"), Settings->flag2.weight_resolution, &p->weight, p->weight_unit);  // ⚖
          if (MI32.option.directBridgeMode) {
            WSContentSend_P(PSTR(" &#x%s;"), (p->weight_removed) ? PSTR("1F90F") : PSTR("1F44C"));  // Yes = 🤏 / No = 👌
            WSContentSend_P(PSTR(" &#x1F44%c;"), (p->weight_stabilized) ? 'D' : 'E');  // Yes = 👍 / No = 👎
          }
          if (p->feature.impedance) {
            WSContentSend_PD(PSTR(" &#x1F463; %u"), p->impedance);  // 👣 feet - Scale impedance (BIA)
            if (MI32.option.directBridgeMode) {
              WSContentSend_P(PSTR(" &#x1F44%c;"), (p->impedance_stabilized) ? 'D' : 'E');  // Yes = 👍 / No = 👎
            }
          }
        }
        if (p->feature.accel) {
          for (uint32_t b = 0; b <= p->accel_used; b++) {
            WSContentSend_PD(PSTR(" &#x1F680; %3_fm/s²"), &p->acceleration[b]);  // 🚀 rocket
          }
        }
        if (p->feature.count) {
          WSContentSend_PD(PSTR(" &#x1FAD8; %d"), p->count);  // 🫘 beans
        }
        if (p->feature.flooding) {
          WSContentSend_PD(PSTR(" &#x1F30A; %u"), p->flooding);  // 🌊 wave
        }
/*
        if (p->feature.co2) {
          WSContentSend_P(PSTR(" &#x1FAE7; %.0f" D_UNIT_PARTS_PER_MILLION), p->co2);  // 🫧 bubbles
        }
*/
        WSContentSend_P(PSTR("{e}"));
      }

      // Optional new line: Lights and buttons
      if (p->feature.light || p->feature.Btn) {
        WSContentSend_P(SI_WEB_LINE_START);

        if (p->feature.light){
          WSContentSend_PD(PSTR(" &#x1F4A1; %d"), p->light);  // 💡 bulb
        }

        if (p->feature.Btn) {
          if ((p->type == MI_BTHOME) && (p->button[1] != 0)){
            for (uint32_t i = 0; i < sizeof(p->button); i++) {
              if (p->button[i] > 0) {
                uint32_t button = (255 == p->button[i]) ? 0 : p->button[i];
                WSContentSend_PD(PSTR(" %d&#xFE0F;&#x20E3; %d"), i +1, button);  // 1️⃣ .. 8️⃣
              }
            }
          } else {
            WSContentSend_PD(PSTR(" 1&#xFE0F;&#x20E3; %d"), p->Btn);  // 1️⃣
          }
        }

        WSContentSend_P(PSTR("{e}"));
      }

#ifdef USE_MI_DECRYPTION
      char tmp[16];
      if (p->needkey > KEY_REQUIRED_AND_INVALID) {
        snprintf(tmp, sizeof(tmp), PSTR("?%d?"), p->needkey);
      } else {
        GetTextIndexed(tmp, sizeof(tmp), p->needkey, kKeyNeeded);
      }

      // adds the link to get the key.
      // provides mac and callback address to receive the key, if we had a website which did this
      // (future work)
      // Optional new line: Encryption key
      if (p->needkey != KEY_NOT_REQUIRED) {
        WSContentSend_P(SI_WEB_LINE_START);
        char MAC[18];
        BLE_ESP32::dump(MAC, 13, p->MAC, 6);
        WSContentSend_P(PSTR(" &#x1F511; <a target='_blank' href='https://tasmota.github.io/ble_key_extractor?mac=%s&cb=http%%3A%%2F%%2F%s%%2Fmikey'>%s</a>"), MAC, IPGetListeningAddressStr().c_str(), tmp);  // 🔑 key

        if (p->pairing){
          WSContentSend_P(PSTR(" &#x1F91D;"));  // 🤝 handshake
        }

        WSContentSend_P(PSTR("{e}"));
      }
#endif  // USE_MI_DECRYPTION

    }

    WSContentSend_P(SI_WEB_LINE_END);  // Terminate current multi column table and open new table
    WSContentSend_P(PSTR("<tr><td colspan=2><div style='text-align:right;font-size:11px;color:#aaa;'>MI ESP32 " MI32_VERSION "</div>{e}"));

#else  // don't USE_SENSOR_ICON

    static  uint16_t _page = 0;
    static  uint16_t _counter = 0;
    int32_t i = _page * MI32.perPage;
    uint32_t j = i + MI32.perPage;

    if (j+1 > numsensors){
      j = numsensors;
    }
    char stemp[5] ={0};
    if (numsensors-(_page*MI32.perPage)>1 && MI32.perPage!=1) {
      sprintf_P(stemp,"-%u",j);
    }
    if (numsensors == 0) i=-1; // only for the GUI

    WSContentSend_P(HTTP_MI32, i + 1, stemp, numsensors);

    for (i; i<j; i++) {
      WSContentSend_P(HTTP_SNS_HR_THIN);
      mi_sensor_t *p;
      p = &MIBLEsensors[i];

      const char *label;
      const char *typeName = kMI32DeviceType[p->type-1];
      const char *alias = BLE_ESP32::getAlias(p->MAC);
      if (alias && *alias){
        label = alias;
        WSContentSend_P(HTTP_MI32_TYPE, label, typeName);
      } else {
        label = typeName;
      }
      char _MAC[18];
      ToHex_P(p->MAC,6,_MAC,18);//,':');
      WSContentSend_P(HTTP_MI32_MAC, label, _MAC);
      WSContentSend_PD(HTTP_MI32_RSSI, label, WifiGetRssiAsQuality(p->RSSI), p->RSSI);

      // for some reason, display flora differently
      switch(p->type){
        case MI_FLORA:{
          if (!isnan(p->temp)) {
            WSContentSend_Temp(label, ConvertTempToFahrenheit(p->temp));  // convert if SO8 on
          }
          if (p->moisture!=0xff) {
            WSContentSend_PD(HTTP_SNS_MOISTURE, label, p->moisture);
          }
          if (p->fertility!=0xffff) {
            WSContentSend_PD(HTTP_MI32_FLORA_DATA, label, p->fertility);
          }
        } break;
        default:{
          if (!isnan(p->hum) && !isnan(p->temp)) {
            WSContentSend_THD(label, ConvertTempToFahrenheit(p->temp), p->hum);  // convert if SO8 on
          }
          else if (!isnan(p->temp)) {
            WSContentSend_Temp(label, ConvertTempToFahrenheit(p->temp));  // convert if SO8 on
          }
        }
      }

      if (p->feature.pres)
      {
        float fpressure = ConvertPressure(p->pres);
        char pressure[33];
        dtostrfd(fpressure, Settings->flag2.pressure_resolution, pressure);
        WSContentSend_PD(HTTP_SNS_PRESSURE, label, pressure, PressureUnit().c_str());
      }

#ifdef USE_MI_DECRYPTION
      bool showkey = false;
      char tmp[40];
      strlcpy(tmp, PSTR("KeyRqd"), sizeof(tmp));
      switch(p->needkey) {
        default:{
          snprintf(tmp, 39, PSTR("?%d?"), p->needkey );
          showkey = true;
        } break;
        case KEY_REQUIREMENT_UNKNOWN: {
          strlcpy(tmp, PSTR("WAIT"), sizeof(tmp));
          showkey = true;
        } break;
        case KEY_NOT_REQUIRED: {
          strlcpy(tmp, PSTR("NOTKEY"), sizeof(tmp));
          //showkey = true;
        } break;
        case KEY_REQUIRED_BUT_NOT_FOUND: {
          strlcpy(tmp, PSTR("NoKey"), sizeof(tmp));
          showkey = true;
        } break;
        case KEY_REQUIRED_AND_FOUND: {
          strlcpy(tmp, PSTR("KeyOk"), sizeof(tmp));
          showkey = true;
        } break;
        case KEY_REQUIRED_AND_INVALID: {
          strlcpy(tmp, PSTR("KeyInv"), sizeof(tmp));
          showkey = true;
        } break;
      }

      // adds the link to get the key.
      // provides mac and callback address to receive the key, if we had a website which did this
      // (future work)
      if (showkey){
        BLE_ESP32::dump(_MAC, 13, p->MAC, 6);
        WSContentSend_P(HTTP_NEEDKEY, label, _MAC, IPGetListeningAddressStr().c_str(), tmp);
      }

#endif //USE_MI_DECRYPTION

      if (p->feature.events){
        WSContentSend_PD(HTTP_MI32_EVENTS, label, p->events);
      }
      if (p->feature.NMT){
        // no motion time
        if(p->NMT>0) WSContentSend_PD(HTTP_MI32_NMT, label, p->NMT);
      }

      if (p->feature.lux){
        if (p->lux!=0x00ffffff) { // this is the error code -> no valid value
          WSContentSend_PD(HTTP_SNS_ILLUMINANCE, label, p->lux);
        }
      }
      if (p->feature.light){
        WSContentSend_PD(HTTP_MI32_LIGHT, label, p->light);
      }
      if (p->feature.scale){
        WSContentSend_PD(HTTP_MISCALE_WEIGHT, label, Settings->flag2.weight_resolution, &p->weight, p->weight_unit);
        if (MI32.option.directBridgeMode) {
          WSContentSend_PD(HTTP_MISCALE_WEIGHT_REMOVED, label, p->weight_removed? PSTR("yes") : PSTR("no"));
          WSContentSend_PD(HTTP_MISCALE_WEIGHT_STABILIZED, label, p->weight_stabilized ? PSTR("yes") : PSTR("no"));
        }
        if (p->feature.impedance) {
          WSContentSend_PD(HTTP_MISCALE_IMPEDANCE, label, p->impedance);
          if (MI32.option.directBridgeMode) {
            WSContentSend_PD(HTTP_MISCALE_IMPEDANCE_STABILIZED, label, p->impedance_stabilized? PSTR("yes") : PSTR("no"));
          }
        }
      }
      if(p->bat!=0x00){
        WSContentSend_PD(HTTP_MI32_BATTERY, label, p->bat);
      }
      if (p->feature.count) {
        WSContentSend_PD(HTTP_MI32_COUNT, label, p->count);
      }
      if (p->feature.volt) {
        WSContentSend_PD(HTTP_SNS_F_VOLTAGE, label, Settings->flag2.voltage_resolution, &p->volt);
      }
      if (p->feature.accel){
        char accelerations[64] = { 0 };
        for (uint32_t b = 0; b <= p->accel_used; b++) {
          ext_snprintf_P(accelerations, sizeof(accelerations), PSTR("%s%s%6_f m/s²"), 
            accelerations, (!b)?"":"<br>", &p->acceleration[b]);
        }
        WSContentSend_P(HTTP_MI32_ACCELERATION, label, accelerations);
      }
      if (p->feature.Btn) {
        WSContentSend_P(HTTP_MI32_LASTBUTTON, label, p->Btn);
      }
      if (p->feature.flooding)
      {
        WSContentSend_PD(HTTP_SJWS01LM_FLOODING, label, p->flooding);
      }
      if (p->pairing){
        WSContentSend_PD(HTTP_PAIRING, label);
      }
    }

    _counter++;
    if(_counter>3) {
      _page++;
      _counter=0;
    }
    if (MIBLEsensors.size()%MI32.perPage == 0 && _page==MIBLEsensors.size()/MI32.perPage) { _page = 0; }
    if (_page>MIBLEsensors.size()/MI32.perPage) { _page = 0; }

#endif  // don't USE_SENSOR_ICON

#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
#define WEB_HANDLE_MI32 "mikey"

bool Xsns62(uint32_t function)
{
//  if (!Settings->flag5.mi32_enable) { return false; }  // SetOption115 - Enable ESP32 MI32 BLE
//  return false;

  bool result = false;

  if (FUNC_INIT == function) {
    MI32Init();
  }
  if (MI32.mode.init) {
    switch (function) {
      case FUNC_EVERY_50_MSECOND:
        MI32Every50mSecond();
        break;
      case FUNC_EVERY_SECOND:
        MI32EverySecond(false);
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kMI32_Commands, MI32_Commands);
        break;
      case FUNC_JSON_APPEND:
        // we are not in control of when this is called...
        //MI32Show(1);
        break;
  #ifdef USE_WEBSERVER
      case FUNC_WEB_ADD_HANDLER:
        WebServer_on(PSTR("/" WEB_HANDLE_MI32), HandleMI32Key);
        break;
      case FUNC_WEB_SENSOR:
        MI32Show(0);
        break;
  #endif  // USE_WEBSERVER
      }
  }
  return result;
}
#endif  // USE_MI_ESP32
#endif  // CONFIG_IDF_TARGET_ESP32 or CONFIG_IDF_TARGET_ESP32C3
#endif  // ESP32

#endif
