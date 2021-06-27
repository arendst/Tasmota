/*
  xsns_62_esp32_mi_ble.ino - MI-BLE-sensors via ESP32 support for Tasmota
  enabled by ESP32 && USE_BLE_ESP32
  if (ESP32 && !USE_BLE_ESP32) then xsns_62_esp32_mi.ino is used - the older driver


  Copyright (C) 2020  Christian Baars and Theo Arends
  Also Simon Hailes and Robert Klauco

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


  --------------------------------------------------------------------------------------------
  Version yyyymmdd  Action    Description
  --------------------------------------------------------------------------------------------
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

#ifdef ESP32                       // ESP32 only. Use define USE_HM10 for ESP8266 support
#if CONFIG_IDF_TARGET_ESP32

#ifdef USE_MI_ESP32

#define XSNS_62                    62
#define USE_MI_DECRYPTION

#include <vector>
#ifdef USE_MI_DECRYPTION
#include <t_bearssl.h>
#endif //USE_MI_DECRYPTION

void MI32scanEndedCB(NimBLEScanResults results);
void MI32notifyCB(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);

struct {
  uint16_t perPage = 4;
  uint8_t mqttCurrentSlot = 0;
  uint8_t mqttCurrentSingleSlot = 0;
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
    uint32_t allwaysAggregate:1; // always show all known values of one sensor in brdigemode
    uint32_t noSummary:1;        // no sensor values at TELE-period
    uint32_t directBridgeMode:1; // send every received BLE-packet as a MQTT-message in real-time
    uint32_t holdBackFirstAutodiscovery:1; // allows to trigger it later
    uint32_t showRSSI:1;
    uint32_t ignoreBogusBattery:1;
    uint32_t minimalSummary:1;   // DEPRECATED!!
    uint32_t onlyAliased:1;      // only include sensors that are aliased
    uint32_t MQTTType:1;
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
    };
    uint32_t raw;
  } eventType;

  int RSSI;
  uint8_t pairing;
  int8_t light; // binary light sensor - initialise to -1
  int16_t Btn; // moved so we can initialise to -1

  uint32_t lastTime;
  uint32_t lux;
  float temp; //Flora, MJ_HT_V1, LYWSD0x, CGx
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
};

struct MAC_t {
  uint8_t buf[7];
};

std::vector<mi_sensor_t> MIBLEsensors;
std::vector<mi_bindKey_t> MIBLEbindKeys;
std::vector<MAC_t> MIBLEBlockList;

void *slotmutex = nullptr;

/*********************************************************************************************\
 * constants
\*********************************************************************************************/

#define D_CMND_MI32 "MI32"

const char kMI32_Commands[] PROGMEM = D_CMND_MI32 "|"
#ifdef USE_MI_DECRYPTION
  "Key|"
  "Keys|"
#endif  // USE_MI_DECRYPTION
  "Period|Time|Page|Battery|Unit|Block|Option";

void (*const MI32_Commands[])(void) PROGMEM = {
#ifdef USE_MI_DECRYPTION
  &CmndMi32Key,
  &CmndMi32Keys,
#endif  // USE_MI_DECRYPTION
  &CmndMi32Period, &CmndMi32Time, &CmndMi32Page, &CmndMi32Battery, &CmndMi32Unit, &CmndMi32Block, &CmndMi32Option };


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

#define MI_MI32_TYPES    14 //count this manually

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
  0x0a1c,  // ATC -> this is a fake ID
  0x098b  // door/window sensor
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
const char * kMI32DeviceType[] PROGMEM = {kMI32DeviceType0,kMI32DeviceType1,kMI32DeviceType2,kMI32DeviceType3,kMI32DeviceType4,kMI32DeviceType5,kMI32DeviceType6,kMI32DeviceType7,kMI32DeviceType8,kMI32DeviceType9,kMI32DeviceType10,kMI32DeviceType11,kMI32DeviceType12,kMI32DeviceType13};

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
    if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG, PSTR("M32: Got a newOperation"));
  }

  if (slot >= 0){
    op->addr = NimBLEAddress(MIBLEsensors[slot].MAC);
  } else {
    if (!addr){
      AddLog(LOG_LEVEL_ERROR, PSTR("M32: No addr"));
      BLE_ESP32::freeOperation(&op);
      return 0;
    }
    op->addr = NimBLEAddress(addr);
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

  if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG, PSTR("M32: MI s:%d op:%s"), slot, BLE_ESP32::BLETriggerResponse(op).c_str());

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
    if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_INFO, PSTR("M32: Req batt read slot %d type %d queued"), slot, MIBLEsensors[slot].type);
  } else {
    if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_INFO, PSTR("M32: Req batt read slot %d type %d non-queued res %d"), slot, MIBLEsensors[slot].type, res);
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
      if (MIBLEsensors[slot].needkey == KEY_REQUIRED_AND_FOUND && !force) return -2;
      res = MI32Operation(slot, OP_READ_HT_LY, LYWSD03_Svc, nullptr, LYWSD03_BattNotifyChar);
      break;
    case MI_MHOC401:
      // don't read if key present and we've decoded at least one advert
      if (MIBLEsensors[slot].needkey == KEY_REQUIRED_AND_FOUND && !force) return -2;
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
    if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG, PSTR("M32: genericSensorReadFn slot %d res %d"), MI32.sensorreader.slot, res);

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
  if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG, PSTR("M32: readOneSensor reading for slot %d res %d"), MI32.sensorreader.slot-1, res);

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
      if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_INFO, PSTR("M32: Batt loop complete at %d"), MI32.batteryreader.slot);
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
    if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_INFO, PSTR("M32: Batt loop will complete at %d"), MI32.batteryreader.slot);
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

  if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG, PSTR("M32: MI op complete context %x"), context);

  int opType = context >> 24;
  int devType = (context >> 16) & 0xff;
  int slot = (context) & 0xff;

  char slotMAC[13];
  BLE_ESP32::dump(slotMAC, sizeof(slotMAC), MIBLEsensors[slot].MAC, 6) ;
  uint8_t addrrev[6];
  memcpy(addrrev, MIBLEsensors[slot].MAC, 6);
  //BLE_ESP32::ReverseMAC(addrrev);
  NimBLEAddress addr(addrrev);

  bool fail = false;
  if (op->addr != addr){
    // slot changed during operation?
    AddLog(LOG_LEVEL_ERROR, PSTR("M32: Slot mac changed during an operation"));
    fail = true;
  }

  if (op->state <= GEN_STATE_FAILED){
    AddLog(LOG_LEVEL_ERROR, PSTR("M32: Operation failed %d for %s"), op->state, slotMAC);
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
      AddLog(LOG_LEVEL_DEBUG, PSTR("M32: Time write for %s complete"), slotMAC);
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
      AddLog(LOG_LEVEL_INFO, PSTR("M32: Batt read slot %d done state %x"), slot, op->state);

    } return 0;

    case OP_UNIT_WRITE: // nothing more to do?
      AddLog(LOG_LEVEL_DEBUG, PSTR("M32: Unit write for %s complete"), slotMAC);
      return 0;

    case OP_UNIT_READ: {
      uint8_t currUnit = op->dataRead[0];
      AddLog(LOG_LEVEL_DEBUG, PSTR("M32: Unit read for %s complete %d"), slotMAC, currUnit);
    } return 0;

    case OP_UNIT_TOGGLE: {
      uint8_t currUnit = op->dataToWrite[0];
      AddLog(LOG_LEVEL_DEBUG, PSTR("M32: Unit toggle for %s complete %d->%d; datasize was %d"), slotMAC, op->dataRead[0], op->dataToWrite[0], op->readlen);
    } return 0;

    case OP_READ_HT_LY: {
      // allow another...
      MI32.sensorreader.active = 0;
      MI32notifyHT_LY(slot, (char*)op->dataNotify, op->notifylen);
      if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG, PSTR("M32: HT_LY notify for %s complete"), slotMAC);
    } return 0;

    default:
      AddLog(LOG_LEVEL_ERROR, PSTR("M32: OpType %d not recognised?"), opType);
      return 0;
  }

  return 0;
}

int MI32advertismentCallback(BLE_ESP32::ble_advertisment_t *pStruct)
{
  // we will try not to use this...
  BLEAdvertisedDevice *advertisedDevice = pStruct->advertisedDevice;

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

  int svcdataCount = advertisedDevice->getServiceDataCount();

  if (svcdataCount == 0) {
    return 0;
  }

  NimBLEUUID UUIDBig = advertisedDevice->getServiceDataUUID(0);//.getNative()->u16.value;

  const ble_uuid_any_t* native = UUIDBig.getNative();
  if (native->u.type != 16){
    //not interested in 128 bit;
    return 0;
  }
  uint16_t UUID = native->u16.value;

  char temp[60];
  BLE_ESP32::dump(temp, 13, addr, 6);

  if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("M32: MI:%s svc[0] UUID (%x)"), temp, UUID);
  std::string ServiceDataStr = advertisedDevice->getServiceData(0);

  uint32_t  ServiceDataLength = ServiceDataStr.length();
  const uint8_t *ServiceData = (const uint8_t *)ServiceDataStr.data();
  BLE_ESP32::dump(temp, 60, ServiceData, ServiceDataLength);
  if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("M32: MI:%s"), temp);


  if (UUID){
    // this will take and keep the mutex until the function is over
    TasAutoMutex localmutex(&slotmutex, "Mi32AdCB2");
    switch(UUID){
      case 0xfe95: // std MI?
      case 0xfdcd: // CGD1?
      {
        MI32ParseResponse(ServiceData, ServiceDataLength, addr, RSSI);
      } break;
      case 0x181a: { //ATC
        MI32ParseATCPacket(ServiceData, ServiceDataLength, addr, RSSI);
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
  for(uint32_t i=0; i<MIBLEbindKeys.size(); i++){
    if(memcmp(keyMAC.MAC,MIBLEbindKeys[i].MAC,sizeof(keyMAC.MAC))==0){
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
  uint8_t payload[32];
  uint8_t mac[6];
  memcpy(mac, macin, 6);
  MI32_ReverseMAC(mac);
  uint8_t _bindkey[32] = {0x0};
  const unsigned char authData[16] = {0x11};
  bool foundNoKey = true;
  if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG, PSTR("M32: Search key for MAC: %02x%02x%02x%02x%02x%02x"), mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  for(uint32_t i=0; i<MIBLEbindKeys.size(); i++){
    if(memcmp(mac, MIBLEbindKeys[i].MAC, 6)==0){
      memcpy(_bindkey, MIBLEbindKeys[i].key, 16);
      if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Decryption Key found"));
      foundNoKey = false;
      break;
    }
  }
  if(foundNoKey){
    AddLog(LOG_LEVEL_DEBUG,PSTR("M32: No Key found"));
    return -2; // indicates needs key
  }

  br_aes_small_ctrcbc_keys keyCtx;
  br_aes_small_ctrcbc_init(&keyCtx, _bindkey, 16);

  br_ccm_context ctx;
  br_ccm_init(&ctx, &keyCtx.vtable);
  br_ccm_reset(&ctx, nonce, 12, 1, len, 4);
  br_ccm_aad_inject(&ctx, authData, 1);
  br_ccm_flip(&ctx);

  memcpy(payload, data, len); //we want to be sure about 4-byte alignement
  br_ccm_run(&ctx, 0, payload, len);
  memcpy(data, payload, len); //back to the packet


  // crashed in here - why?, so give it more space to work with?
  // returns 1 if matched, else 0
  int ret = br_ccm_check_tag(&ctx, &tag);

  if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Err:%i, Decrypted : %02x %02x %02x %02x %02x"), ret, payload[1],payload[2],payload[3],payload[4],payload[5]);
  return ret-1; // -> -1=fail, 0=success
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

const char *MIaddrStr(const uint8_t *addr, int useAlias = 0){
  static char addrstr[32];

  const char *id = nullptr;
  if (useAlias){
    id = BLE_ESP32::getAlias(addr);
  }
  if (!id || !(*id)){
    id = addrstr;
    BLE_ESP32::dump(addrstr, 13, addr, 6);
  } else {
  }

  return id;
}

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
  //if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG, PSTR("M32: MI frame %d"), parsed->framecnt);
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
  }

  int decres = 1;
  // everything after MAC is encrypted if specified?
  if (parsed->framedata.isencrypted){
    if (len < byteindex + 3+4+1){
      return 0;
    }
    const uint8_t* mac = slotmac;
    if (parsed->framedata.MACFlag){
      mac = parsed->macdata.mac;
    }
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
      if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("M32 %s: Payload decrypted"), MIaddrStr(slotmac));
      break;
    case -1: // key failed to work
      parsed->needkey = KEY_REQUIRED_AND_INVALID;
      AddLog(LOG_LEVEL_ERROR,PSTR("M32 %s: Payload decrypt failed"), MIaddrStr(slotmac));
      parsed->payloadpresent = 0;
      return 0;
      break;
    case -2: // key not present
      parsed->needkey = KEY_REQUIRED_BUT_NOT_FOUND;
      AddLog(LOG_LEVEL_ERROR,PSTR("M32 %s: Payload encrypted but no key"), MIaddrStr(slotmac));
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
    if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("M32 %s: No payload"), MIaddrStr(slotmac));
    parsed->payload.size = 0;
    parsed->payloadpresent = 0;
    return 0;
  }

  // we have payload which did not need decrypt.
  if (decres == 1){
    parsed->needkey = KEY_NOT_REQUIRED;
    if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("M32 %s: Payload unencrypted"), MIaddrStr(slotmac));
  }

  // already decrypted if required
  parsed->payloadpresent = 1;
  memcpy(&parsed->payload, (data + byteindex), (len - byteindex));
  if (parsed->payload.size != (len - byteindex) - 3){
    AddLog(LOG_LEVEL_DEBUG,PSTR("M32 %s: Payload length mismatch"), MIaddrStr(slotmac));
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
 * @return uint32_t   Known or new slot in the sensors-vector
 */
uint32_t MIBLEgetSensorSlot(const uint8_t *mac, uint16_t _type, uint8_t counter){

  //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: %s: will test ID-type: %x"),D_CMND_MI32, _type);
  bool _success = false;
  for (uint32_t i=0; i < MI_MI32_TYPES; i++){ // i < sizeof(kMI32DeviceID) gives compiler warning
    if(_type == kMI32DeviceID[i]){
      _type = i+1;
      _success = true;
      break;
    }
    else {
      //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: %s: ID-type is not: %x"),D_CMND_MI32,kMI32DeviceID[i]);
    }
  }
  if(!_success) {
    _type = 1; // unknown
  }

  //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: %s: vector size %u"),D_CMND_MI32, MIBLEsensors.size());
  for(uint32_t i=0; i<MIBLEsensors.size(); i++){
    if(memcmp(mac, MIBLEsensors[i].MAC, 6)==0){
      // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Counters: %x %x"),MIBLEsensors[i].lastCnt, counter);
      if(MIBLEsensors[i].lastCnt==counter) {
        // AddLog(LOG_LEVEL_DEBUG,PSTR("Old packet"));
        if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: %s: slot: %u/%u - ign repeat"),D_CMND_MI32, i, MIBLEsensors.size());
        //return 0xff; // packet received before, stop here
      }
      if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Frame %d, last %d"), counter, MIBLEsensors[i].lastCnt);
      MIBLEsensors[i].lastCnt = counter;
      if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: %s: slot: %u/%u"),D_CMND_MI32, i, MIBLEsensors.size());

      if (MIBLEsensors[i].type != _type){
        // this happens on incorrectly configured pvvx ATC firmware
        AddLog(LOG_LEVEL_ERROR,PSTR("M32: %s: slot: %u - device type 0x%04x(%s) -> 0x%04x(%s) - check device is only sending one type of advert."),D_CMND_MI32, i,
          kMI32DeviceID[MIBLEsensors[i].type-1], kMI32DeviceType[MIBLEsensors[i].type-1], kMI32DeviceID[_type-1], kMI32DeviceType[_type-1]);
        MIBLEsensors[i].type = _type;
      }

      return i;
    }
    //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: %s: i: %x %x %x %x %x %x"),D_CMND_MI32, MIBLEsensors[i].MAC[5], MIBLEsensors[i].MAC[4],MIBLEsensors[i].MAC[3],MIBLEsensors[i].MAC[2],MIBLEsensors[i].MAC[1],MIBLEsensors[i].MAC[0]);
    //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: %s: n: %x %x %x %x %x %x"),D_CMND_MI32, mac[5], mac[4], mac[3],mac[2],mac[1],mac[0]);
  }
  //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: %s: new sensor -> slot: %u"),D_CMND_MI32, MIBLEsensors.size());
  //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: %s: found new sensor"),D_CMND_MI32);
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
    default:
      _newSensor.hum=NAN;
      _newSensor.feature.temp=1;
      _newSensor.feature.hum=1;
      _newSensor.feature.tempHum=1;
      _newSensor.feature.bat=1;
      break;
    }
  MIBLEsensors.push_back(_newSensor);
  AddLog(LOG_LEVEL_DEBUG,PSTR("M32: %s: new %s at slot: %u"),D_CMND_MI32, kMI32DeviceType[_type-1],MIBLEsensors.size()-1);
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
  if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Scan complete"));
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
  MI32.option.allwaysAggregate = 1;
  MI32.option.noSummary = 0;
  MI32.option.minimalSummary = 0;
  MI32.option.directBridgeMode = 0;
  MI32.option.showRSSI = 1;
  MI32.option.ignoreBogusBattery = 1; // from advertisements
  MI32.option.holdBackFirstAutodiscovery = 1;

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
        AddLog(LOG_LEVEL_DEBUG,PSTR("M32: %s: FLORA Firmware: %s"),D_CMND_MI32,MIBLEsensors[slot].firmware);
      }
    }
    MIBLEsensors[slot].eventType.bat  = 1;
    MIBLEsensors[slot].shallSendMQTT = 1;
    MI32.mode.shallTriggerTele = 1;
    if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Batt read for %s complete %d"), slotMAC, value);
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
      if(_slot==0xff) return;

      if ((_slot >= 0) && (_slot < MIBLEsensors.size())){
        if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("M32: %s:pvvx at slot %u"), kMI32DeviceType[MIBLEsensors[_slot].type-1],_slot);
        MIBLEsensors[_slot].RSSI=RSSI;
        MIBLEsensors[_slot].needkey=KEY_NOT_REQUIRED;

        MIBLEsensors[_slot].temp = (float)(ppv_packet->temperature)/100.0f;
        MIBLEsensors[_slot].hum = (float)(ppv_packet->humidity)/100.0f;
        MIBLEsensors[_slot].eventType.tempHum  = 1;
        MIBLEsensors[_slot].bat = ppv_packet->battery_level;
        MIBLEsensors[_slot].eventType.bat  = 1;

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

  if(_slot==0xff) return;

  if ((_slot >= 0) && (_slot < MIBLEsensors.size())){
    if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("M32: %s at slot %u"), kMI32DeviceType[MIBLEsensors[_slot].type-1],_slot);
    MIBLEsensors[_slot].RSSI=RSSI;
    MIBLEsensors[_slot].needkey=KEY_NOT_REQUIRED;

    MIBLEsensors[_slot].temp = (float)(int16_t(__builtin_bswap16(_packet->temp)))/10.0f;
    MIBLEsensors[_slot].hum = (float)_packet->hum;
    MIBLEsensors[_slot].eventType.tempHum  = 1;
    MIBLEsensors[_slot].bat = _packet->batPer;
    MIBLEsensors[_slot].eventType.bat  = 1;

    if(MI32.option.directBridgeMode) {
      MIBLEsensors[_slot].shallSendMQTT = 1;
      MI32.mode.shallTriggerTele = 1;
    }
  } else {

  }
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
  BLE_ESP32::dump(tmp, 20, (uint8_t*)&(parsed->payload), parsed->payload.size+3);
  if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: MI%d payload %s"), _slot, tmp);

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
      MIBLEsensors[_slot].lastTime = millis();
      MIBLEsensors[_slot].events++;
      MIBLEsensors[_slot].lux = pld->lux;
      MIBLEsensors[_slot].eventType.lux = 1;
      MIBLEsensors[_slot].NMT = 0;
      MI32.mode.shallTriggerTele = 1;
      MIBLEsensors[_slot].shallSendMQTT = 1;
      MIBLEsensors[_slot].feature.lux = 1;
      MIBLEsensors[_slot].feature.NMT = 1;
      MIBLEsensors[_slot].feature.events=1;

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
        if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: Mode 4: temp updated"));
      } else {
        if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: Mode 4: temp ignored > 60 (%f)"), _tempFloat);
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
        if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: Mode 6: hum updated"));
      } else {
        if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: Mode 6: hum ignored > 101 (%f)"), _tempFloat);
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
      if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: Mode 8: moisture updated"));
      // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Mode 8: U8: %u Moisture"), _beacon.moist);
    break;
    case 0x1009: // 'conductivity' / 'Soil EC value'
      MIBLEsensors[_slot].fertility=pld->fert;
      MIBLEsensors[_slot].eventType.fert  = 1;
      MIBLEsensors[_slot].feature.fert = 1;
      if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: Mode 9: fertility updated"));
      // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Mode 9: U16: %u Fertility"), _beacon.fert);
    break;
    case 0x100a:// 'Electricity'
      if(MI32.option.ignoreBogusBattery){
        if(MIBLEsensors[_slot].type==MI_LYWSD03MMC || MIBLEsensors[_slot].type==MI_MHOC401){
          res = 0;
          break;
        }
      }
      MIBLEsensors[_slot].feature.bat = 1;
      if(pld->bat<101){
        MIBLEsensors[_slot].bat = pld->bat;
        MIBLEsensors[_slot].eventType.bat  = 1;
        if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: Mode a: bat updated"));
      } else {
        MIBLEsensors[_slot].bat = 100;
        MIBLEsensors[_slot].eventType.bat  = 1;
        if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: Mode a: bat > 100 (%d)"), pld->bat);
      }
      // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Mode a: U8: %u %%"), _beacon.bat);
    break;
    // 100b-100d -> undefioend in docs.
    case 0x100d:{ // is this right????
      MIBLEsensors[_slot].feature.tempHum = 1;
      float _tempFloat=(float)(pld->HT.temp)/10.0f;
      if(_tempFloat < 60){
        MIBLEsensors[_slot].temp = _tempFloat;
        if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: Mode d: temp updated"));
      } else {
        if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: Mode d: temp ignored > 60 (%f)"), _tempFloat);
      }
      _tempFloat=(float)(pld->HT.hum)/10.0f;
      if(_tempFloat < 100){
          MIBLEsensors[_slot].hum = _tempFloat;
          if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: Mode d: hum updated"));
      } else {
        if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: Mode d: hum ignored > 100 (%f)"), _tempFloat);
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

    //Weight attributes	0x101A	600	0
    //No one moves over time	0x101B	1	1
    //Smart pillow	0x101C	60	1
    //Formaldehyde (new)	0x101D

    default: {
      AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Unknown MI pld type %x %s"), parsed->payload.type, tmp);
      res = 0;
    } break;
  }

  if(res && MI32.option.directBridgeMode) {
    MIBLEsensors[_slot].shallSendMQTT = 1;
    MI32.mode.shallTriggerTele = 1;
  }

  return res;
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

  uint16_t _slot = MIBLEgetSensorSlot( addr, parsed.devicetype, parsed.framecnt );
  if(_slot==0xff) return;
  if ((_slot >= 0) && (_slot < MIBLEsensors.size())){
    if (parsed.needkey != KEY_REQUIREMENT_UNKNOWN){
      MIBLEsensors[_slot].needkey = parsed.needkey;
    }
    MIBLEsensors[_slot].RSSI=RSSI;
    if (!res){ // - if the payload is not valid
      if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG, PSTR("M32: MIParsePacket returned %d"), res);
      return;
    } else {
    }
    MI32parseMiPayload(_slot, &parsed);
  }
}

bool MI32isInBlockList(const uint8_t* MAC){
  bool isBlocked = false;
  for(auto &_blockedMAC : MIBLEBlockList){
    if(memcmp(_blockedMAC.buf,MAC,6) == 0) isBlocked = true;
  }
  return isBlocked;
}

void MI32removeMIBLEsensor(uint8_t* MAC){
  // this will take and keep the mutex until the function is over
  TasAutoMutex localmutex(&slotmutex, "Mi32Rem");

  MIBLEsensors.erase( std::remove_if( MIBLEsensors.begin() , MIBLEsensors.end(), [MAC]( mi_sensor_t _sensor )->bool
  { return (memcmp(_sensor.MAC,MAC,6) == 0); }
  ), end( MIBLEsensors ) );
}
/***********************************************************************\
 * Read data from connections
\***********************************************************************/

void MI32notifyHT_LY(int slot, char *_buf, int len){
  if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: %s: raw data: %x%x%x%x%x%x%x"),D_CMND_MI32,_buf[0],_buf[1],_buf[2],_buf[3],_buf[4],_buf[5],_buf[6]);
  // the value 0b00 is 28.16 C?
  if(_buf[0] != 0 || _buf[1] != 0){
    memcpy(&LYWSD0x_HT,(void *)_buf,sizeof(LYWSD0x_HT));
    if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG, PSTR("M32: %s: T * 100: %u, H: %u, V: %u"),D_CMND_MI32,LYWSD0x_HT.temp,LYWSD0x_HT.hum, LYWSD0x_HT.volt);
    uint32_t _slot = slot;

    if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: MIBLE: Sensor slot: %u"), _slot);
    static float _tempFloat;
    _tempFloat=(float)(LYWSD0x_HT.temp)/100.0f;
    if(_tempFloat<60){
        MIBLEsensors[_slot].temp=_tempFloat;
        // MIBLEsensors[_slot].showedUp=255; // this sensor is real
    }
    _tempFloat=(float)LYWSD0x_HT.hum;
    if(_tempFloat<100){
      MIBLEsensors[_slot].hum = _tempFloat;
      if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: LYWSD0x: hum updated"));
    }
    MIBLEsensors[_slot].eventType.tempHum  = 1;
    if (MIBLEsensors[_slot].type == MI_LYWSD03MMC || MIBLEsensors[_slot].type == MI_MHOC401){
      // ok, so CR2032 is 3.0v, but drops immediately to ~2.9.
      // so we'll go with the 2.1 min, 2.95 max.
      float minVolts = 2100.0;
      //float maxVolts = 2950.0;
      //float range = maxVolts - minVolts;
      //float divisor = range/100; // = 8.5
      float percent = (((float)LYWSD0x_HT.volt) - minVolts)/ 8.5; //divisor;
      if (percent > 100) percent = 100;

      MIBLEsensors[_slot].bat = (int) percent;
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

  if (MI32.option.MQTTType == 1
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
    MI32ShowOneMISensor();
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
  MI32.secondsCounter ++;

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
        if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG, PSTR("M32: will set Time"));
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
        if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG, PSTR("M32: will toggle Unit"));
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
        { return (memcmp(_entry.buf,_MACasBytes.buf,6) == 0); }
        ), end( MIBLEBlockList ) );
    } break;
    case 1: {
      //TasAutoMutex localmutex(&slotmutex, "Mi32Block3");
      bool _notYetInList = true;
      for (auto &_entry : MIBLEBlockList) {
        if (memcmp(_entry.buf,_MACasBytes.buf,6) == 0){
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
  int onOff = atoi(XdrvMailbox.data);
  switch(XdrvMailbox.index) {
    case 0:
      if (set){
        MI32.option.allwaysAggregate = onOff;
      } else {
        onOff = MI32.option.allwaysAggregate;
      }
      break;
    case 1:
      if (set){
        MI32.option.noSummary = onOff;
      } else {
        onOff = MI32.option.noSummary;
      }
      break;
    case 2:
      if (set){
        MI32.option.directBridgeMode = onOff;
      } else {
        onOff = MI32.option.directBridgeMode;
      }
      break;
    case 4:{
      if (set){
        MI32.option.ignoreBogusBattery = onOff;
      } else {
        onOff = MI32.option.ignoreBogusBattery;
      }
    } break;
    case 5:{
      if (set){
        MI32.option.onlyAliased = onOff;
        if (MI32.option.onlyAliased){
          // discard all sensors for a restart
          MIBLEsensors.clear();
        }
      } else {
        onOff = MI32.option.onlyAliased;
      }
    } break;
    case 6:{
      if (set){
        MI32.option.MQTTType = onOff;
      } else {
        onOff = MI32.option.MQTTType;
      }
    } break;
    default:{
      ResponseCmndIdxError();
      return;
    } break;
  }
  ResponseCmndIdxNumber(onOff);
  return;
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
  if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Key %d %s"), op, XdrvMailbox.data);

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

        AddLog(LOG_LEVEL_ERROR,PSTR("M32: Add key mac %s = key %s"), mac, key);
        char tmp[20];
        // convert mac back to string
        ToHex_P(addr,6,tmp,20,0);
        if (MI32AddKey(tmp, key)){
          added++;
        }
        p = strtok(nullptr, " ,=");
      } while (p);

      if (added){
        if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Added %d Keys"), added);
        MI32KeyListResp();
      } else {
        MI32KeyListResp();
      }
      return;
    } break;
    case 2:{ // clear
      if (BLE_ESP32::BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Keys clearing %d"), MIBLEbindKeys.size());
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

const char HTTP_MI32[] PROGMEM = "{s}MI ESP32 v0921{m}%u%s / %u{e}";
const char HTTP_MI32_ALIAS[] PROGMEM = "{s}%s Alias {m}%s{e}";
const char HTTP_MI32_MAC[] PROGMEM = "{s}%s %s{m}%s{e}";
const char HTTP_RSSI[] PROGMEM = "{s}%s " D_RSSI "{m}%d dBm{e}";
const char HTTP_BATTERY[] PROGMEM = "{s}%s" " Battery" "{m}%u %%{e}";
const char HTTP_LASTBUTTON[] PROGMEM = "{s}%s Last Button{m}%u {e}";
const char HTTP_EVENTS[] PROGMEM = "{s}%s Events{m}%u {e}";
const char HTTP_NMT[] PROGMEM = "{s}%s No motion{m}> %u seconds{e}";
const char HTTP_MI32_FLORA_DATA[] PROGMEM = "{s}%s" " Fertility" "{m}%u us/cm{e}";
const char HTTP_MI32_HL[] PROGMEM = "{s}<hr>{m}<hr>{e}";
const char HTTP_MI32_LIGHT[] PROGMEM = "{s}%s" " Light" "{m}%d{e}";

//const char HTTP_NEEDKEY[] PROGMEM = "{s}%s <a target=\"_blank\" href=\""
//  "https://atc1441.github.io/TelinkFlasher.html?mac=%s&cb=http%%3A%%2F%%2F%s%%2Fmikey"
//  "\">%s</a>{m} {e}";

//const char HTTP_NEEDKEY[] PROGMEM = "{s}%s <a target=\"_blank\" href=\""
//  "http://127.0.0.1:8887/keys/TelinkFlasher.html?mac=%s&cb=http%%3A%%2F%%2F%s%%2Fmikey"
//  "\">%s</a>{m} {e}";
const char HTTP_NEEDKEY[] PROGMEM = "{s}%s <a target=\"_blank\" href=\""
  "https://btsimonh.github.io/atc1441.github.io/TelinkFlasherTasmota.html?mac=%s&cb=http%%3A%%2F%%2F%s%%2Fmikey"
  "\">%s</a>{m} {e}";


const char HTTP_PAIRING[] PROGMEM = "{s}%s Pair Button Pressed{m} {e}";


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

  strncat(key, mac, sizeof(key));
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
  for (int i = MIBLEsensors.size()-1; i >= 0 ; i--) {
    //if (MIBLEsensors[i].MAC[2] || MIBLEsensors[i].MAC[3] || MIBLEsensors[i].MAC[4] || MIBLEsensors[i].MAC[5]){
      if (!BLE_ESP32::devicePresent(MIBLEsensors[i].MAC)){
        uint8_t *mac = MIBLEsensors[i].MAC;
        AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Dev no longer present MAC: %02x%02x%02x%02x%02x%02x"), mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
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
    ResponseAppend_P(PSTR("\"alias\":\"%s\","),
          alias);
  }

  ResponseAppend_P(PSTR("\"mac\":\"%02x%02x%02x%02x%02x%02x\""),
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
          ResponseAppendTHD(p->temp, p->hum);
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
          ResponseAppend_P(PSTR(",\"" D_JSON_TEMPERATURE "\":%*_f"),
            Settings->flag2.temperature_resolution, &p->temp);
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
    if (p->feature.Btn){
      if(p->eventType.Btn || !MI32.mode.triggeredTele || MI32.option.allwaysAggregate
#ifdef USE_HOME_ASSISTANT
          ||(hass_mode==2)
#endif //USE_HOME_ASSISTANT
      ){
        ResponseAppend_P(PSTR(",\"Btn\":%d"),p->Btn);
      }
    }
    if(p->eventType.PairBtn && p->pairing){
        ResponseAppend_P(PSTR(",\"Pair\":%u"),p->pairing);
    }
  } // minimal summary


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
  p->eventType.raw = 0;
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
  ResponseAppend_P(PSTR("}"));
  MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR(D_RSLT_SENSOR), Settings->flag.mqtt_sensor_retain);
#ifdef MQTT_DATA_STRING
  //AddLog(LOG_LEVEL_DEBUG,PSTR("M32: %s: show some %d %s"),D_CMND_MI32, MI32.mqttCurrentSlot, TasmotaGlobal.mqtt_data.c_str());
#else
  //AddLog(LOG_LEVEL_DEBUG,PSTR("M32: %s: show some %d %s"),D_CMND_MI32, MI32.mqttCurrentSlot, TasmotaGlobal.mqtt_data);
#endif

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
void MI32ShowOneMISensor(){
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
    MI32.option.MQTTType == 1
    ){

    ResponseTime_P(PSTR(","));
    MI32GetOneSensorJson(MI32.mqttCurrentSingleSlot, 1);
    mi_sensor_t *p;
    p = &MIBLEsensors[MI32.mqttCurrentSingleSlot];

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
    char SensorTopic[60];
    sprintf(SensorTopic, "tele/tasmota_ble/%s",
      id);

    MqttPublish(SensorTopic);
#ifdef MQTT_DATA_STRING
    //AddLog(LOG_LEVEL_DEBUG,PSTR("M32: %s: show some %d %s"),D_CMND_MI32, MI32.mqttCurrentSlot, TasmotaGlobal.mqtt_data.c_str());
#else
    //AddLog(LOG_LEVEL_DEBUG,PSTR("M32: %s: show some %d %s"),D_CMND_MI32, MI32.mqttCurrentSlot, TasmotaGlobal.mqtt_data);
#endif
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

    char SensorTopic[60];
    sprintf(SensorTopic, "tele/tasmota_ble/%s",
      id);


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
        (classes[i+2][0]?"\"unit_of_meas\":\"":""),
        classes[i+2],
        (classes[i+2][0]?"\",":""),
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
#ifdef MQTT_DATA_STRING
  //AddLog(LOG_LEVEL_DEBUG,PSTR("M32: %s: show some %d %s"),D_CMND_MI32, MI32.mqttCurrentSlot, TasmotaGlobal.mqtt_data.c_str());
#else
  //AddLog(LOG_LEVEL_DEBUG,PSTR("M32: %s: show some %d %s"),D_CMND_MI32, MI32.mqttCurrentSlot, TasmotaGlobal.mqtt_data);
#endif
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
  if(
#ifdef USE_HOME_ASSISTANT
    Settings->flag.hass_discovery
    ||
#endif //USE_HOME_ASSISTANT
    MI32.option.MQTTType == 1
    ){
    maxcnt = 1;
  }


  do {
    ResponseTime_P(PSTR(""));
    int cnt = 0;
    mi_sensor_t *p;
    for (; (sensor < numsensors) && (cnt < maxcnt); sensor++) {
      p = &MIBLEsensors[sensor];
      if(p->eventType.raw == 0) continue;
      if(p->shallSendMQTT==0) continue;

      cnt++;
      ResponseAppend_P(PSTR(","));
      // hide sensor name if HASS or option6
      MI32GetOneSensorJson(sensor, (maxcnt == 1));
      int mlen = ResponseLength();

      // if we ran out of room, leave here.
      if (ResponseSize() - mlen < 100){
        sensor++;
        break;
      }
    }
    if (cnt){ // if we got one, then publish
      ResponseAppend_P(PSTR("}"));
      if(
    #ifdef USE_HOME_ASSISTANT
        Settings->flag.hass_discovery
        ||
    #endif //USE_HOME_ASSISTANT
        MI32.option.MQTTType == 1
        ){
        char SensorTopic[60];
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
        sprintf(SensorTopic, "tele/tasmota_ble/%s",
          id);
        MqttPublish(SensorTopic);
      } else {
        MqttPublishPrefixTopic_P(STAT, PSTR(D_RSLT_SENSOR), Settings->flag.mqtt_sensor_retain);
      }
#ifdef MQTT_DATA_STRING
      AddLog(LOG_LEVEL_DEBUG,PSTR("M32: %s: triggered %d %s"),D_CMND_MI32, sensor, TasmotaGlobal.mqtt_data.c_str());
#else
      AddLog(LOG_LEVEL_DEBUG,PSTR("M32: %s: triggered %d %s"),D_CMND_MI32, sensor, TasmotaGlobal.mqtt_data);
#endif
      XdrvRulesProcess(0);

    } else { // else don't and clear
      ResponseClear();
    }
  } while (sensor < numsensors);
}


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
    if (numsensors==0) i=-1; // only for the GUI

    WSContentSend_PD(HTTP_MI32, i+1,stemp,numsensors);
    for (i; i<j; i++) {
      WSContentSend_PD(HTTP_MI32_HL);
      mi_sensor_t *p;
      p = &MIBLEsensors[i];

      const char *typeName = kMI32DeviceType[p->type-1];
      const char *alias = BLE_ESP32::getAlias(p->MAC);
      if (alias && *alias){
        WSContentSend_PD(HTTP_MI32_ALIAS, typeName, alias);
      }
      char _MAC[18];
      ToHex_P(p->MAC,6,_MAC,18);//,':');
      WSContentSend_PD(HTTP_MI32_MAC, typeName, D_MAC_ADDRESS, _MAC);
      WSContentSend_PD(HTTP_RSSI, typeName, p->RSSI);


      // for some reason, display flora differently
      switch(p->type){
        case MI_FLORA:{
          if (!isnan(p->temp)) {
            WSContentSend_Temp(typeName, p->temp);
          }
          if (p->moisture!=0xff) {
            WSContentSend_PD(HTTP_SNS_MOISTURE, typeName, p->moisture);
          }
          if (p->fertility!=0xffff) {
            WSContentSend_PD(HTTP_MI32_FLORA_DATA, typeName, p->fertility);
          }
        } break;
        default:{
          if (!isnan(p->hum) && !isnan(p->temp)) {
            WSContentSend_THD(typeName, p->temp, p->hum);
          }
        }
      }

#ifdef USE_MI_DECRYPTION
      bool showkey = false;
      char tmp[40];
      strcpy(tmp, PSTR("KeyRqd"));
      switch(p->needkey) {
        default:{
          snprintf(tmp, 39, PSTR("?%d?"), p->needkey );
          showkey = true;
        } break;
        case KEY_REQUIREMENT_UNKNOWN: {
          strcpy(tmp, PSTR("WAIT"));
          showkey = true;
        } break;
        case KEY_NOT_REQUIRED: {
          strcpy(tmp, PSTR("NOTKEY"));
          //showkey = true;
        } break;
        case KEY_REQUIRED_BUT_NOT_FOUND: {
          strcpy(tmp, PSTR("NoKey"));
          showkey = true;
        } break;
        case KEY_REQUIRED_AND_FOUND: {
          strcpy(tmp, PSTR("KeyOk"));
          showkey = true;
        } break;
        case KEY_REQUIRED_AND_INVALID: {
          strcpy(tmp, PSTR("KeyInv"));
          showkey = true;
        } break;
      }

      // adds the link to get the key.
      // provides mac and callback address to receive the key, if we had a website which did this
      // (future work)
      if (showkey){
        BLE_ESP32::dump(_MAC, 13, p->MAC,6);
        WSContentSend_PD(HTTP_NEEDKEY, typeName, _MAC, Webserver->client().localIP().toString().c_str(), tmp );
      }

#endif //USE_MI_DECRYPTION

      if (p->feature.events){
        WSContentSend_PD(HTTP_EVENTS, typeName, p->events);
      }
      if (p->feature.NMT){
        // no motion time
        if(p->NMT>0) WSContentSend_PD(HTTP_NMT, typeName, p->NMT);
      }

      if (p->feature.lux){
        if (p->lux!=0x00ffffff) { // this is the error code -> no valid value
          WSContentSend_PD(HTTP_SNS_ILLUMINANCE, typeName, p->lux);
        }
      }
      if (p->feature.light){
        WSContentSend_PD(HTTP_MI32_LIGHT, typeName, p->light);
      }

      if(p->bat!=0x00){
          WSContentSend_PD(HTTP_BATTERY, typeName, p->bat);
      }
      if (p->feature.Btn){
        WSContentSend_PD(HTTP_LASTBUTTON, typeName, p->Btn);
      }
      if (p->pairing){
        WSContentSend_PD(HTTP_PAIRING, typeName);
      }
    }
    _counter++;
    if(_counter>3) {
      _page++;
      _counter=0;
    }
    if (MIBLEsensors.size()%MI32.perPage==0 && _page==MIBLEsensors.size()/MI32.perPage) { _page = 0; }
    if (_page>MIBLEsensors.size()/MI32.perPage) { _page = 0; }
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
#define WEB_HANDLE_MI32 "mikey"

bool Xsns62(uint8_t function)
{
//  if (!Settings->flag5.mi32_enable) { return false; }  // SetOption115 - Enable ESP32 MI32 BLE
//  return false;

  bool result = false;

  switch (function) {
    case FUNC_INIT:
      MI32Init();
      break;
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
  return result;
}
#endif  // USE_MI_ESP32
#endif  // CONFIG_IDF_TARGET_ESP32
#endif  // ESP32

#endif