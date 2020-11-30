/*
  xsns_255_BLE_ESP32.ino - MI-BLE-sensors via ESP32 support for Tasmota

  Copyright (C) 2020  Christian Baars and Theo Arends and Simon Hailes

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

*/
#define ESP32
#ifdef ESP32                       // ESP32 only. Use define USE_HM10 for ESP8266 support

#define USE_BLE_ESP32
#ifdef USE_BLE_ESP32

#define XSNS_99                    99
#define USE_MI_DECRYPTION

#include <NimBLEDevice.h>
#include <vector>
#include <deque>
#include <string.h>
#ifdef USE_MI_DECRYPTION
#include <t_bearssl.h>
#endif //USE_MI_DECRYPTION

namespace BLE99 {


SemaphoreHandle_t  BLEOperationsMutex;

struct {
  uint16_t perPage = 4;
  uint32_t period;             // set manually in addition to TELE-period, is set to TELE-period after start
  union {
    struct {
      uint32_t init:1;
      uint32_t connected:1;
      uint32_t autoScan:1;
      uint32_t canScan:1;
      uint32_t runningScan:1;
      uint32_t canConnect:1;
      uint32_t willConnect:1;
      uint32_t readingDone:1;
      uint32_t shallSetTime:1;
      uint32_t willSetTime:1;
      uint32_t shallReadBatt:1;
      uint32_t willReadBatt:1;
      uint32_t shallSetUnit:1;
      uint32_t willSetUnit:1;
      uint32_t shallTriggerTele:1;
      uint32_t triggeredTele:1;
      uint32_t shallClearResults:1;   // BLE scan results
      uint32_t shallShowStatusInfo:1; // react to amount of found sensors via RULES
      uint32_t firstAutodiscoveryDone:1;
      uint32_t activeBeacon:1;
      uint32_t shallShowScanResult:1;
      uint32_t shallShowBlockList:1;
    };
    uint32_t all = 0;
  } mode;
  struct {
    uint8_t sensor;             // points to to the number 0...255
    uint8_t beaconScanCounter;  // countdown timer in seconds
  } state;
  struct {
    uint32_t allwaysAggregate:1; // always show all known values of one sensor in brdigemode
    uint32_t noSummary:1;        // no sensor values at TELE-period
    uint32_t directBridgeMode:1; // send every received BLE-packet as a MQTT-message in real-time
    uint32_t holdBackFirstAutodiscovery:1; // allows to trigger it later
    uint32_t showRSSI:1;
    uint32_t ignoreBogusBattery:1;
    uint32_t minimalSummary:1;   // DEPRECATED!!
  } option;
} BLE;

#pragma pack(1)  // byte-aligned structures to read the sensor data



#pragma pack(0)


// generic sensor type used as during
// connect/read/wrtie/notify operations
// only one operation will happen at a time 
struct generic_sensor_t {
  uint16_t state;
  uint32_t opid; // incrementing id so we can find them
  
  // uint8_t cancel; 
  // uint8_t requestType; 
  char MAC[13];
  char serviceStr[100];
  char characteristicStr[100];
  char notificationCharacteristicStr[100];
  int RSSI;
  uint64_t notifytimer;
  uint8_t dataToWrite[100];
  uint8_t writelen;
  uint8_t dataRead[100];
  uint8_t readlen;
  uint8_t readtruncated;
  uint8_t dataNotify[100];
  uint8_t notifylen;
  uint8_t notifytruncated;

  NimBLEClient *pClient;

  void *callback; // OPCOMPLETE_CALLBACK function, used by external drivers
};


struct ble_advertisment_t {
  const uint8_t *addr;
  int RSSI;
  const char *name;

  const uint8_t *payload;
  uint8_t payloadLen;

  const uint8_t *manufacturerData;
  uint8_t manufacturerDataLen;

  uint8_t svcdataCount;
  struct {
    const ble_uuid_any_t* serviceUUID;
    char serviceUUIDStr[40]; // longest UUID 36 chars?
    const uint8_t* serviceData;
    uint8_t serviceDataLen;
  } svcdata[5];
  uint8_t serviceCount;
  struct {
    const ble_uuid_any_t* serviceUUID;
    char serviceUUIDStr[40]; // longest UUID 36 chars?
  } services[5];
};

// callback types to be used by external drivers
typedef bool ADVERTISMENT_CALLBACK(ble_advertisment_t *pStruct);
typedef bool OPCOMPLETE_CALLBACK(generic_sensor_t *pStruct);

static ble_advertisment_t BLEAdvertisment;


// only run from main thread, becaus eit deletes things that were newed there...
static void mainThreadOpCallbacks();
void addOperation(std::deque<generic_sensor_t*> *ops, generic_sensor_t** op);
generic_sensor_t* nextOperation(std::deque<generic_sensor_t*> *ops);
std::string BLETriggerResponse(generic_sensor_t *toSend);
static void BLEscanEndedCB(NimBLEScanResults results);
static void BLEGenNotifyCB(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);


static void BLEShow(bool json);
static void BLEPostMQTT(bool json);
static void BLEStartOperationTask();

// these are only run from the run task
static void runCurrentOperation(generic_sensor_t** pCurrentOperation);
static void runTaskDoneOperation(generic_sensor_t** op);



#define EXAMPLE_ADVERTISMENT_CALLBACK
#define EXAMPLE_OPERATION_CALLBACK

#ifdef EXAMPLE_ADVERTISMENT_CALLBACK
bool myAdvertCallback(ble_advertisment_t *pStruct);
#endif
#ifdef EXAMPLE_OPERATION_CALLBACK
bool myOpCallback(generic_sensor_t *pStruct);
bool myOpCallback2(generic_sensor_t *pStruct);
#endif


void registerForAdvertismentCallbacks(ADVERTISMENT_CALLBACK* pFn);
void registerForOpCallbacks(OPCOMPLETE_CALLBACK* pFn);

bool extQueueOperation(generic_sensor_t** op);


#define GEN_STATE_IDLE 0
#define GEN_STATE_START 1

#define GEN_STATE_READDONE 7
#define GEN_STATE_WRITEDONE 8
#define GEN_STATE_WAITNOTIFY 9
#define GEN_STATE_WAITINDICATE 10

#define GEN_STATE_NOTIFIED 11

#define GEN_STATE_TODELETE 12



#define GEN_STATE_FAILED 0x100
#define GEN_STATE_FAILED_CANTNOTIFYORINDICATE 0x101
#define GEN_STATE_FAILED_CANTREAD 0x102
#define GEN_STATE_FAILED_CANTWRITE 0x103
#define GEN_STATE_FAILED_NOSERVICE 0x104
#define GEN_STATE_FAILED_NO_RW_CHAR 0x105
#define GEN_STATE_FAILED_NONOTIFYCHAR 0x106
#define GEN_STATE_FAILED_NOTIFYTIMEOUT 0x107
#define GEN_STATE_FAILED_READ 0x108
#define GEN_STATE_FAILED_WRITE 0x109
#define GEN_STATE_FAILED_CONNECT 0x10A
#define GEN_STATE_FAILED_NOTIFY 0x10B
#define GEN_STATE_FAILED_INDICATE 0x10C
#define GEN_STATE_FAILED_NODEVICE 0x10D
#define GEN_STATE_FAILED_NOREADWRITE 0x110
#define GEN_STATE_FAILED_CANCEL 0x111

struct scan_entry_t {
  uint8_t MAC[6];
  uint16_t CID;
  uint16_t SVC;
  uint16_t UUID;
  int32_t RSSI;
};

struct MAC_t {
  uint8_t buf[6];
};


static BLEScan* BLEScan = nullptr;
// time we last started a scan in uS using esp_timer_get_time();
// used to setect a scan which did not call back?
uint64_t BLEScanStartetdAt = 0; 
int BLEScanTimeS = 20; // scan duraiton in S
int BLEMaxTimeBetweenAdverts = 40; // we expect an advert at least this frequently, else restart BLE (in S)
uint64_t BLEScanLastAdvertismentAt = 0;
int sendMQTT = 0;

// operation being prepared through commands
generic_sensor_t* prepOperation = nullptr;

// operations which have been queued
std::deque<generic_sensor_t*> queuedOperations;
// operations in progress (at the moment, only one)
std::deque<generic_sensor_t*> currentOperations;
// operaitons which have completed or failed, ready to send to MQTT
std::deque<generic_sensor_t*> completedOperations;

// list of registered callbacks for advertisments
// register using void registerForAdvertismentCallbacks(ADVERTISMENT_CALLBACK* pFN);
std::deque<ADVERTISMENT_CALLBACK*> advertismentCallbacks;

std::deque<OPCOMPLETE_CALLBACK*> operationsCallbacks;

uint32_t lastopid = 0;

/*********************************************************************************************\
 * constants
\*********************************************************************************************/

#define D_CMND_BLE "BLE"

const char kBLE_Commands[] PROGMEM = D_CMND_BLE "|"
  "Period|Option|Op|Mode";

static void CmndBLEPeriod(void);
static void CmndBLEOption(void);
static void CmndBLEOperation(void);
static void CmndBLEMode(void);

void (*const BLE_Commands[])(void) PROGMEM = {
  &BLE99::CmndBLEPeriod, &BLE99::CmndBLEOption, &BLE99::CmndBLEOperation, &BLE99::CmndBLEMode };

/*********************************************************************************************\
 * enumerations
\*********************************************************************************************/

enum BLE_Commands {          // commands useable in console or rules
  CMND_BLE_PERIOD,           // set period like TELE-period in seconds between read-cycles
  CMND_BLE_OPTION,           // change driver options at runtime
  CMND_BLE_OP,                // connect/read/write/notify operations
  CMND_BLE_MODE              // change mode of ble - BLE_MODES 
  };

enum {
  BLEModeDisabled = 0, // BLE is disabled
  BLEModeByCommand = 1, // BLE is activeated by commands only
  BLEModeActive = 2, // BLE is scanning all the time 
} BLE_MODES;


uint8_t BLEMode = BLEModeActive;
uint8_t BLENextMode = BLEModeActive;


/*********************************************************************************************\
 * Helper functions
\*********************************************************************************************/


/**
 * @brief Simple pair of functions to dump to a hex string.
 *
 */
static const char h[] PROGMEM = "0123456789ABCDEF";
void hex(char *dest, uint8_t v){
  *(dest++) = h[(v>>4)&0xf];
  *(dest++) = h[v&0xf];
  *(dest) = 0;
}

// convert from binary to hex.
// add a '+' on the end if not enough room.
char * dump(char *dest, int maxchars, uint8_t *src, int len){
  int lenmax = maxchars/2-2;
  int actuallen = 0;
  for (actuallen = 0; actuallen < lenmax && actuallen < len; actuallen++){
    if (actuallen < lenmax){
      hex(dest+actuallen*2, src[actuallen]);
    }
  }
  if (actuallen != len){
    *(dest+actuallen*2) = '+';
    *(dest+actuallen*2+1) = 0;
  }
  return dest;
}

// convert from a hex string to binary
int fromHex(uint8_t *dest, char *src, int maxlen){
  int srclen = strlen(src)/2; 
  if (srclen > maxlen){
    return 0;
  }

  for (int i = 0; i < srclen; i++){
    char t[3];
    t[0] = src[i*2];
    t[1] = src[i*2 + 1];
    t[2] = 0;

    int byte = strtol(t, NULL, 16);
    *dest++ = byte;
  }
  return srclen;
}


/**
 * @brief Reverse an array of 6 bytes
 *
 * @param _mac a byte array of size 6 (typicalliy representing a MAC address)
 */
static void BLE_ReverseMAC(uint8_t _mac[]){
  uint8_t _reversedMAC[6];
  for (uint8_t i=0; i<6; i++){
    _reversedMAC[5-i] = _mac[i];
  }
  memcpy(_mac,_reversedMAC, sizeof(_reversedMAC));
}




/*********************************************************************************************\
 * Example Advertisment callback
\*********************************************************************************************/

#ifdef EXAMPLE_ADVERTISMENT_CALLBACK

#define FLORA       1
#define MJ_HT_V1    2
#define LYWSD02     3
#define LYWSD03MMC  4
#define CGG1        5
#define CGD1        6
#define NLIGHT      7
#define MJYD2S      8
#define YEERC       9
#define MHOC401     10
#define MHOC303     11
#define ATC         12

// match ADVERTISMENT_CALLBACK
bool myAdvertCallback(ble_advertisment_t *pStruct){

/*
struct ble_advertisment_t {
  uint8_t *addr;
  int RSSI;
  char *name;

  uint8_t *payload;
  uint8_t payloadLen;

  uint8_t svcdataCount;
  struct {
    ble_uuid_any_t* serviceUUID;
    char serviceUUIDStr[40]; // longest UUID 36 chars?
    uint8_t serviceData;
    uint8_t serviceDataLen;
  } svcdata[5];
  uint8_t serviceCount;
  struct {
    ble_uuid_any_t* serviceUUID;
    char serviceUUIDStr[40]; // longest UUID 36 chars?
  } services[5];
};
*/

  if (pStruct->svcdataCount){
    if (pStruct->svcdata[0].serviceUUID->u.type == 16){
      int UUID = pStruct->svcdata[0].serviceUUID->u16.value;
      switch(UUID){
        case 0xfe95:{
          if (pStruct->svcdata[0].serviceDataLen < 9){
            // skip NLIGHT
          } else {
            int type = (pStruct->svcdata[0].serviceData[3]<<8) | pStruct->svcdata[0].serviceData[2];
            AddLog_P(LOG_LEVEL_DEBUG,PSTR("BLE: Sensor type %x"), type);
            switch(type){
              case MJ_HT_V1:
              case CGG1:
              case YEERC:
                return true; // eat this, no further callbacks will be called.
                break;
            }
          }
        }break;
        case 0xfdcd:
          break;
        case 0x181a:
          break;
        default:
          break;
      }
    }
  }

  // indicate others can also hear this
  // to say 'I want this exclusively', return true.
  return false;

}
#endif
/*********************************************************************************************\
 * End of Example Advertisment callback
\*********************************************************************************************/


/*********************************************************************************************\
 * Example Operations callbacks
\*********************************************************************************************/
#ifdef EXAMPLE_OPERATION_CALLBACK

// this one is used to demonstrate processing ALL operations
bool myOpCallback(generic_sensor_t *pStruct){
  AddLog_P(LOG_LEVEL_INFO,PSTR("myOpCallback"));
  return false; // return true to block MQTT broadcast
}

// this one is used to demonstrate processing of ONE specific operation
bool myOpCallback2(generic_sensor_t *pStruct){
  AddLog_P(LOG_LEVEL_INFO,PSTR("myOpCallback2"));
  return true; // return true to block MQTT broadcast
}
#endif
/*********************************************************************************************\
 * End of Example Operations callbacks
\*********************************************************************************************/



/*********************************************************************************************\
 * Classes
\*********************************************************************************************/

// does not reallt take any action
class BLESensorCallback : public NimBLEClientCallbacks {
  void onConnect(NimBLEClient* pclient) {
    AddLog_P(LOG_LEVEL_DEBUG,PSTR("onConnect"));
    BLE.mode.connected = 1;
  }
  void onDisconnect(NimBLEClient* pclient) {
    AddLog_P(LOG_LEVEL_DEBUG,PSTR("onDisconnect"));
    BLE.mode.connected = 0;
  }
  bool onConnParamsUpdateRequest(NimBLEClient* BLEClient, const ble_gap_upd_params* params) {
    AddLog_P(LOG_LEVEL_DEBUG,PSTR("onConnParamsUpdateRequest"));
    if(params->itvl_min < 24) { /** 1.25ms units */
      return false;
    } else if(params->itvl_max > 40) { /** 1.25ms units */
      return false;
    } else if(params->latency > 2) { /** Number of intervals allowed to skip */
      return false;
    } else if(params->supervision_timeout > 100) { /** 10ms units */
      return false;
    }
    return true;
  }
};

static BLESensorCallback clientCB;


class BLEAdvCallbacks: public NimBLEAdvertisedDeviceCallbacks {
  void onResult(NimBLEAdvertisedDevice* advertisedDevice) {
    AddLog_P(LOG_LEVEL_DEBUG,PSTR("Advertised Device"));
    uint64_t now = esp_timer_get_time();
    BLEScanLastAdvertismentAt = now; // note the time of the last advertisment

    memset(&BLEAdvertisment, 0, sizeof(BLEAdvertisment));

    int RSSI = advertisedDevice->getRSSI();
    uint8_t addr[6];
    memcpy(addr,advertisedDevice->getAddress().getNative(),6);
    MI32_ReverseMAC(addr);
    BLEAdvertisment.addr = addr;
    BLEAdvertisment.RSSI = RSSI;

    char addrstr[20];
    dump(addrstr, 20, addr, 6);

    int svcDataCount = advertisedDevice->getServiceDataCount();
    int svcUUIDCount = advertisedDevice->getServiceUUIDCount();

    uint8_t* payload = advertisedDevice->getPayload();
    size_t payloadlen = advertisedDevice->getPayloadLength();

    BLEAdvertisment.payload = payload;
    BLEAdvertisment.payloadLen = payloadlen;

    char payloadhex[100];
    dump(payloadhex, 100, payload, payloadlen);

    // this mjust survive the scope of the callbacks
    std::string name;
    if (advertisedDevice->haveName()){
      name = advertisedDevice->getName();
      const char *n = name.c_str();
      BLEAdvertisment.name = n;
      AddLog_P(LOG_LEVEL_DEBUG,PSTR("Name: %s"), n);
    }

    if (advertisedDevice->haveManufacturerData()){
      std::string data = advertisedDevice->getManufacturerData();
      char mfgstr[100];
      int len = data.length();

      BLEAdvertisment.manufacturerData = (const uint8_t *)data.data();
      BLEAdvertisment.manufacturerDataLen = len;

      dump(mfgstr, 100, (uint8_t*)data.data(), len);

      AddLog_P(LOG_LEVEL_DEBUG,PSTR("Mfg: %s"), mfgstr);
    }

    if (svcDataCount == 0) {
      AddLog_P(LOG_LEVEL_DEBUG,PSTR("%s RSSI:%d payloadonly %s"), addrstr, RSSI, payloadhex);
    } else {
      AddLog_P(LOG_LEVEL_DEBUG,PSTR("%s RSSI:%d payload %s service data:"), addrstr, RSSI, payloadhex);
      for (int i = 0; i < svcDataCount; i++){
        NimBLEUUID UUID = advertisedDevice->getServiceDataUUID(i);//.getNative()->u16.value;
        std::string ServiceData = advertisedDevice->getServiceData(i);
        
        size_t ServiceDataLength = ServiceData.length();
        const uint8_t *serviceData = (const uint8_t *)ServiceData.data();


        char svcdatastr[80];
        dump(svcdatastr, 80, (uint8_t*)serviceData, ServiceDataLength);
        //char svcuuidstr[20];
        std::string strUUID = UUID;
        //int uuidLen = strUUID.length();
        //dump(svcuuidstr, 20, (uint8_t*)strUUID.c_str(), uuidLen);
      
        const ble_uuid_any_t* native = UUID.getNative();
        if (i < 5){
          BLEAdvertisment.svcdata[i].serviceData = serviceData;
          BLEAdvertisment.svcdata[i].serviceDataLen = ServiceDataLength;
          BLEAdvertisment.svcdata[i].serviceUUID = native;
          strncpy(BLEAdvertisment.svcdata[i].serviceUUIDStr, strUUID.c_str(), sizeof(BLEAdvertisment.svcdata[i].serviceUUIDStr));
          BLEAdvertisment.svcdataCount = i+1;
        }
        uint16_t UUID16 = native->u16.value;
      // AddLog_P(LOG_LEVEL_DEBUG,PSTR("UUID: %x"),UUID);
        AddLog_P(LOG_LEVEL_DEBUG,PSTR("UUIDType:%d UUID16:%x UUID:%s svcdata:%s"), native->u.type, UUID16, strUUID.c_str(), svcdatastr);
      }

      for (int i = 0; i < svcUUIDCount; i++){
        NimBLEUUID UUID = advertisedDevice->getServiceUUID(i);//.getNative()->u16.value;
        //char svcuuidstr[20];
        std::string strUUID = UUID;
        //int uuidLen = strUUID.length();
        //dump(svcuuidstr, 20, (uint8_t*)strUUID.c_str(), uuidLen);
        const ble_uuid_any_t* native = UUID.getNative();
        if (i < 5){
          BLEAdvertisment.services[i].serviceUUID = native;
          strncpy(BLEAdvertisment.services[i].serviceUUIDStr, strUUID.c_str(), sizeof(BLEAdvertisment.services[i].serviceUUIDStr));
          BLEAdvertisment.serviceCount = i+1;
        }
        AddLog_P(LOG_LEVEL_DEBUG,PSTR("Service UUID:%s"), strUUID.c_str());
      }
    }

    // call anyone who asked about advertisments
    for (int i = 0; i < advertismentCallbacks.size(); i++) {
      try {
        ADVERTISMENT_CALLBACK* pFN;
        pFN = advertismentCallbacks[i];
        pFN(&BLEAdvertisment);
      } catch(const std::exception& e){
        AddLog_P(LOG_LEVEL_ERROR,PSTR("exception in advertismentCallbacks"));
      }
    }

 }
};


static BLEAdvCallbacks BLEScanCallbacks;
static BLESensorCallback BLESensorCB;

/*********************************************************************************************\
 * BLE callback functions
\*********************************************************************************************/

static void BLEscanEndedCB(NimBLEScanResults results){
  AddLog_P(LOG_LEVEL_DEBUG,PSTR("Scan ended"));
  BLE.mode.runningScan = 0;
}

static void BLEGenNotifyCB(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify){
    NimBLEClient *pRClient;

    AddLog_P(LOG_LEVEL_DEBUG,PSTR("Notified length: %u"),length);

    // find the operation this is associated with
    NimBLERemoteService *pSvc = pRemoteCharacteristic->getRemoteService();

    pRClient = pSvc->getClient();
    NimBLEAddress devaddr = pRClient->getPeerAddress();
    uint8_t addr[6];

    memcpy(addr,devaddr.getNative(),6);
    MI32_ReverseMAC(addr);
    char addrstr[20];
    dump(addrstr, 20, addr, 6);

    generic_sensor_t *thisop = nullptr; 
    for (int i = 0; i < currentOperations.size(); i++){
      generic_sensor_t *op = currentOperations[i];
      if (!strcasecmp(op->MAC, addrstr)){
        thisop = op;
        break;
      }
    }

    pRemoteCharacteristic->unsubscribe();

    if (!thisop){
      AddLog_P(LOG_LEVEL_DEBUG,PSTR("no op for notify"));
      return;
    }

    for (int i = 0; i < length && i < sizeof(thisop->dataNotify); i++){
      thisop->dataNotify[i] = pData[i];
    }
    thisop->notifylen = length;
    if (length > sizeof(thisop->dataNotify)){
      thisop->notifytruncated = 1;
    } else {
      thisop->notifytruncated = 0;
    }
    thisop->state = GEN_STATE_NOTIFIED;
    thisop->notifytimer = 0;

}




/*********************************************************************************************\
 * common functions
\*********************************************************************************************/


/*********************************************************************************************\
 * init NimBLE
\*********************************************************************************************/
static void BLEStartScanTask();

void registerForAdvertismentCallbacks(ADVERTISMENT_CALLBACK* pFn){
  advertismentCallbacks.push_back(pFn);
}

void registerForOpCallbacks(OPCOMPLETE_CALLBACK* pFn){
  operationsCallbacks.push_back(pFn);
}



static void BLEPreInit(void) {
  BLE.mode.init = false;
  prepOperation = nullptr;
  BLEOperationsMutex = xSemaphoreCreateMutex();

#ifdef EXAMPLE_ADVERTISMENT_CALLBACK
  BLE99::registerForAdvertismentCallbacks(&BLE99::myAdvertCallback);
#endif

#ifdef EXAMPLE_OPERATION_CALLBACK
  BLE99:registerForOpCallbacks(&BLE99::myOpCallback);
#endif


  BLE99::BLEStartOperationTask();
}

static void StartBLE(void) {
  BLE.mode.init = 0;
}

static void StopBLE(void){
  if (BLEScan){
    BLEScan->stop();
    BLEScan->clearResults();
    BLEScan = NULL;
  }

  // need to ensure we have nothing left BEFORE we do this
  //NimBLEDevice::deinit(true);
}	


static void BLEInit(void) {
  if (BLEMode == BLEModeDisabled) return;

  if (BLE.mode.init) { return; }

  if (TasmotaGlobal.global_state.wifi_down) { return; }
  if (WiFi.getSleep() == false) {
//    AddLog_P(LOG_LEVEL_DEBUG,PSTR("BLE: WiFi modem not in sleep mode, BLE cannot start yet"));
    if (0 == Settings.flag3.sleep_normal) {
      AddLog_P(LOG_LEVEL_DEBUG,PSTR("BLE: About to restart to put WiFi modem in sleep mode"));
      Settings.flag3.sleep_normal = 1;  // SetOption60 - Enable normal sleep instead of dynamic sleep
      TasmotaGlobal.restart_flag = 2;
    }
    return;
  }

  if (!BLE.mode.init) {
    uint64_t now = esp_timer_get_time();
    BLEScanLastAdvertismentAt = now; // initialise the time of the last advertisment

    NimBLEDevice::init("");
    AddLog_P(LOG_LEVEL_INFO,PSTR("BLE: init BLE device"));
    BLE.mode.canScan = 1;
    BLE.mode.init = 1;
    BLE.period = Settings.tele_period;

    BLE99::BLEStartScanTask(); // Let's get started !!
  }
  return;
}

/*********************************************************************************************\
 * Task section
\*********************************************************************************************/

static void BLEScanTask(void *pvParameters);

static void BLEStartScanTask(){
    AddLog_P(LOG_LEVEL_DEBUG,PSTR("%s: Start scanning"),D_CMND_BLE);

    BLE99::BLEScanTask(NULL);
    return;
}

// this is actually NOT a task - it just starts a scan in the main thread
static void BLEScanTask(void *pvParameters){
  if (BLEMode == BLEModeDisabled) return;
  AddLog_P(LOG_LEVEL_DEBUG,PSTR("BLEScanTask"));
  if (BLEScan == nullptr) BLEScan = NimBLEDevice::getScan();
  // DEBUG_SENSOR_LOG(PSTR("%s: Scan Cache Length: %u"),D_CMND_BLE, BLEScan->getResults().getCount());
  BLEScan->setInterval(120);
  BLEScan->setWindow(100);
  BLEScan->setAdvertisedDeviceCallbacks(&BLEScanCallbacks,true);
  BLEScan->setActiveScan(false);
  AddLog_P(LOG_LEVEL_DEBUG,PSTR("Startscan from BLEScanTaskStart"));
  BLEScan->start(20, BLEscanEndedCB, false); // never stop scanning, will pause automatically while connecting
  BLE.mode.runningScan = 1;
}

static void BLEOperationTask(void *pvParameters);

static void BLEStartOperationTask(){
    AddLog_P(LOG_LEVEL_DEBUG,PSTR("%s: Start operations"),D_CMND_BLE);

    //if (BLE.mode.connected) return;
    xTaskCreatePinnedToCore(
      BLE99::BLEOperationTask,    /* Function to implement the task */
      "BLEOperationTask",  /* Name of the task */
      4096,//2048,             /* Stack size in words */
      NULL,             /* Task input parameter */
      0,                /* Priority of the task */
      NULL,             /* Task handle. */
      0);               /* Core where the task should run */
}


// this IS as task.
// we MAY be able to run a few of these simultaneously, but this is not yet tested.
// and probably not required.  But everything is there to do so....
static void BLEOperationTask(void *pvParameters){
  uint32_t timer = 0;
  // operation which is currently in progress in THIS TASK
  generic_sensor_t* currentOperation = nullptr;

  for(;;){
    if (BLEScan){
      AddLog_P(LOG_LEVEL_DEBUG,PSTR("Operation loop"));

      BLE99::runCurrentOperation(&currentOperation);
    }
    // come around every second
    vTaskDelay(1000/ portTICK_PERIOD_MS);
  }

  AddLog_P(LOG_LEVEL_DEBUG,PSTR("Leave operation task"));
  vTaskDelete( NULL );
}




/*********************************************************************************************\
 * parse the response from advertisements
\*********************************************************************************************/

/**
 * @brief Present BLE scan in the console, after that deleting the scan data
 *
 */
static void BLEshowScanResults(){
}

/***********************************************************************\
 * Read data from connections
\***********************************************************************/


/**
 * @brief Launch functions from Core 1 to make race conditions less likely
 *
 */

static void BLEEvery50mSecond(){
  if(BLE.mode.shallTriggerTele){
      BLE.mode.shallTriggerTele = 0;
      //BLEtriggerTele();
  }
}

/**
 * @brief Main loop of the driver, "high level"-loop
 *
 */

static void BLEEverySecond(bool restart){
  //BLEStartScanTask();
  AddLog_P(LOG_LEVEL_DEBUG,PSTR("one sec (scanning %d)"),BLE.mode.runningScan);

  // check for application callbacks here.
  // this may remove complete items.
  BLE99::mainThreadOpCallbacks();

  // post any MQTT data if we completed anything in the last second
  if (completedOperations.size()){
    BLE99::BLEPostMQTT(true); // send only completed
  }

  if (BLEScan){
    // restart scanning if it ended and we don;t have any current operations in progress.
    if (!BLE.mode.runningScan && (!currentOperations.size())){
      //BLEScan->clearResults();
      if ((BLEMode != BLEModeDisabled) && (BLEMode != BLEModeByCommand)){
        AddLog_P(LOG_LEVEL_DEBUG,PSTR("Startscan from onesec"));
        BLEScan->start(BLEScanTimeS, BLEscanEndedCB, true); // 20s scans, restarted when theyn finish
        BLE.mode.runningScan = 1;
        BLEScanStartetdAt = esp_timer_get_time();
      }
    } else {

      // check for a couple of possible failure modes.
      uint64_t now = esp_timer_get_time();
      if (BLEScanTimeS){
        uint64_t diff = now - BLEScanStartetdAt;
        diff = diff / 1000; // convert to ms;
        if (diff > (BLEScanTimeS + 5) * 1000){
          AddLog_P(LOG_LEVEL_ERROR,PSTR("Scan did not end on time"));
          BLEScan->stop();
          BLE.mode.runningScan = 0;
        }
      }
      uint64_t diff = now - BLEScanLastAdvertismentAt;
      diff = diff / 1000; // convert to ms;
      if (diff > (BLEMaxTimeBetweenAdverts) * 1000){
        BLEScanLastAdvertismentAt = now;
        AddLog_P(LOG_LEVEL_ERROR,PSTR("Long time between Advertisments - restarting BLE"));
        StopBLE();
        StartBLE();
      }
    }
   
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

static void CmndBLEPeriod(void) {
  if (XdrvMailbox.data_len > 0) {
    if (1 == XdrvMailbox.payload) {
      BLEEverySecond(true);
    } else {
      BLE.period = XdrvMailbox.payload;
    }
  }
  ResponseCmndNumber(BLE.period);
}


void CmndBLEOption(void){
  ResponseCmndDone();
}




// this disconnects from a device if necessary, and then
// moves the operation from 'currentOperations' to 'completedOperations'.

// for safety's sake, only call from the run task
static void runTaskDoneOperation(generic_sensor_t** op){
  if ((*op)->pClient){
    AddLog_P(LOG_LEVEL_DEBUG,PSTR("disconnect in done"));
    try {
      (*op)->pClient->disconnect();
    } catch(const std::exception& e){
      AddLog_P(LOG_LEVEL_DEBUG,PSTR("exception in disconnect"));
    }
    try {
      NimBLEDevice::deleteClient((*op)->pClient);
    } catch(const std::exception& e){
      AddLog_P(LOG_LEVEL_DEBUG,PSTR("exception in delete"));
    }
    (*op)->pClient = NULL;
  }

  xSemaphoreTake(BLEOperationsMutex, portMAX_DELAY);
  // find this operation in currentOperations, and remove it.
  for (int i = 0; i < currentOperations.size(); i++){
    if (currentOperations[i]->opid == (*op)->opid){
      currentOperations.erase(currentOperations.begin() + i);
      break;
    }
  }
  xSemaphoreGive(BLEOperationsMutex); // release mutex

  // by adding it to this list, this will cause it to be sent to MQTT
  AddLog_P(LOG_LEVEL_DEBUG,PSTR("add to completedOperations in done"));
  addOperation(&completedOperations, op);
  return;
}

// this retrievs the next operation from the passed list, and removes it from the list.
// or returns null if none.
generic_sensor_t* nextOperation(std::deque<generic_sensor_t*> *ops){
  generic_sensor_t* op = nullptr;
  if (ops->size()){
    xSemaphoreTake(BLEOperationsMutex, portMAX_DELAY);
    op = (*ops)[0]; 
    ops->pop_front();
    xSemaphoreGive(BLEOperationsMutex); // release mutex
    AddLog_P(LOG_LEVEL_DEBUG,PSTR("new currentOperation"));
  }
  return op;
}

// this adds an operation to the end of passed list.
// it also sets the op pointer passed to null.
void addOperation(std::deque<generic_sensor_t*> *ops, generic_sensor_t** op){
  xSemaphoreTake(BLEOperationsMutex, portMAX_DELAY);
  if (ops->size() < 10){
    ops->push_back(*op);
    *op = nullptr;
  }     
  xSemaphoreGive(BLEOperationsMutex); // release mutex
  AddLog_P(LOG_LEVEL_DEBUG,PSTR("added operation"));
  return;
}


bool extQueueOperation(generic_sensor_t** op){
  if (!op) {
    AddLog_P(LOG_LEVEL_ERROR,PSTR("op invalid in extQueueOperation"));
    return false;
  }
  if (!(*op)){
    AddLog_P(LOG_LEVEL_ERROR,PSTR("*op invalid in extQueueOperation"));
    return false;
  }
  (*op)->state = GEN_STATE_START; // trigger request later
  (*op)->opid = lastopid++;

  addOperation(&queuedOperations, op);
  AddLog_P(LOG_LEVEL_DEBUG,PSTR("op added id %d in extQueueOperation"), (lastopid-1));
  return true;
}



// this runs one operation
// if the passed pointer is empty, it tries to get a next one.
static void runCurrentOperation(generic_sensor_t** pCurrentOperation){
  if (!pCurrentOperation) return;
  if (!BLEScan) return;

  if (!*pCurrentOperation) {
    *pCurrentOperation = nextOperation(&queuedOperations);
    if (*pCurrentOperation){
      generic_sensor_t* temp = *pCurrentOperation;
      //this will null it out, so save and restore.
      addOperation(&currentOperations, pCurrentOperation); 
      *pCurrentOperation = temp;
    }
  }
  if (!*pCurrentOperation) return;



  // if awaiting notification
  if ((*pCurrentOperation)->notifytimer){
    // if it took too long, then disconnect
    uint64_t now = esp_timer_get_time();
    uint64_t diff = now - (*pCurrentOperation)->notifytimer;
    diff = diff/1000;
    if (diff > 20000){
      AddLog_P(LOG_LEVEL_DEBUG,PSTR("notify timeout"));
      (*pCurrentOperation)->state = GEN_STATE_FAILED_NOTIFYTIMEOUT;
      (*pCurrentOperation)->notifytimer = 0;
    }
    // we can't process any further, because op will be at state readdone or writedone
    return;
  }

  switch((*pCurrentOperation)->state){
    case GEN_STATE_READDONE:
    case GEN_STATE_WRITEDONE:
    case GEN_STATE_NOTIFIED:
      // just stay here until this is removed by the main thread
      AddLog_P(LOG_LEVEL_DEBUG,PSTR("operation complete"));
      BLE99::runTaskDoneOperation(pCurrentOperation);
      return;
      break;

    case GEN_STATE_START:
      // continue to start the process here.
      break;

    default:
      break;
  }


  if (!*pCurrentOperation) return;

  if ((*pCurrentOperation)->state & GEN_STATE_FAILED){
    AddLog_P(LOG_LEVEL_ERROR,PSTR("operation failed"));
    BLE99::runTaskDoneOperation(pCurrentOperation);
    return;
  }

  if ((*pCurrentOperation)->state != GEN_STATE_START){
    return;
  }


  if (BLEScan){
    generic_sensor_t* op = *pCurrentOperation;
    NimBLEScanResults results = BLEScan->getResults();
    int deviceCount = results.getCount();
    int i = 0;
    for(i = 0; i < deviceCount; i++) {
        NimBLEAdvertisedDevice device = results.getDevice(i);
        NimBLEAddress devaddr = device.getAddress();
        uint8_t addr[6];
        memcpy(addr,devaddr.getNative(),6);
        MI32_ReverseMAC(addr);
        char addrstr[20];
        dump(addrstr, 20, addr, 6);
        if (!strcasecmp(op->MAC, addrstr)){
          NimBLEUUID serviceUuid(op->serviceStr);
          if (device.isAdvertisingService(serviceUuid)) {
            // create a client and connect
            op->pClient = 
              NimBLEDevice::createClient();
            op->pClient->setClientCallbacks(&clientCB, false);
            /** Set initial connection parameters: These settings are 15ms interval, 0 latency, 120ms timout. 
             *  These settings are safe for 3 clients to connect reliably, can go faster if you have less 
             *  connections. Timeout should be a multiple of the interval, minimum is 100ms.
             *  Min interval: 12 * 1.25ms = 15, Max interval: 12 * 1.25ms = 15, 0 latency, 51 * 10ms = 510ms timeout 
             */
            op->pClient->setConnectionParams(12,12,0,51);
            /** Set how long we are willing to wait for the connection to complete (seconds), default is 30. */
            op->pClient->setConnectTimeout(5);
                
            if (op->pClient->connect(&device)) {
              NimBLERemoteService *pService = op->pClient->getService(serviceUuid);
              
              if (pService != nullptr) {
                // read or write characteristic - we always need this?
                NimBLERemoteCharacteristic *pCharacteristic = 
                  pService->getCharacteristic(op->characteristicStr);
                AddLog_P(LOG_LEVEL_DEBUG,PSTR("got service"));

                // pre-set to fail if no operations requested
                op->state = GEN_STATE_FAILED_NOREADWRITE;

                // if we have been asked to get a notification
                if (op->notificationCharacteristicStr){
                  NimBLERemoteCharacteristic *pNCharacteristic = 
                    pService->getCharacteristic(op->notificationCharacteristicStr);
                  if (pNCharacteristic != nullptr) {
                    op->notifylen = 0;
                    if(pNCharacteristic->canNotify()) {
                      if(pNCharacteristic->subscribe(true, BLE99::BLEGenNotifyCB)) {
                        AddLog_P(LOG_LEVEL_DEBUG,PSTR("subscribe for notify"));
                        uint64_t now = esp_timer_get_time();
                        op->notifytimer = now;
                        // this will get changed to read or write,
                        // but here in case it's notify only (can that happen?)
                        op->state = GEN_STATE_WAITNOTIFY;
                      } else {
                        AddLog_P(LOG_LEVEL_DEBUG,PSTR("failed subscribe for notify"));
                        op->state = GEN_STATE_FAILED_NOTIFY;
                      }
                    } else {
                      if(pNCharacteristic->canIndicate()) {
                        if(pNCharacteristic->subscribe(false, BLE99::BLEGenNotifyCB)) {
                          AddLog_P(LOG_LEVEL_DEBUG,PSTR("subscribe for indicate"));
                          op->state = GEN_STATE_WAITINDICATE;
                          uint64_t now = esp_timer_get_time();
                          op->notifytimer = now;
                        } else {
                          AddLog_P(LOG_LEVEL_DEBUG,PSTR("failed subscribe for indicate"));
                          op->state = GEN_STATE_FAILED_INDICATE;
                        }
                      } else {
                        op->state = GEN_STATE_FAILED_CANTNOTIFYORINDICATE;
                      }
                    }
                  } else {
                    op->state = GEN_STATE_FAILED_NONOTIFYCHAR;
                  }
                } // no supplied notify char is ok

                if (pCharacteristic != nullptr) {
                  AddLog_P(LOG_LEVEL_DEBUG,PSTR("got characteristic"));
                  op->state = GEN_STATE_FAILED_NOREADWRITE; // overwritten on failure

                  if (op->readlen){
                    if(pCharacteristic->canRead()) {
                      std::string value = pCharacteristic->readValue();
                      op->readlen = value.length();
                      memcpy(op->dataRead, value.data(), 
                        (op->readlen>sizeof(op->dataRead))?
                          sizeof(op->dataRead): 
                          op->readlen);
                      if (op->readlen>sizeof(op->dataRead)){
                        op->readtruncated = 1;
                      } else {
                        op->readtruncated = 0;
                      }
                      op->state = GEN_STATE_READDONE;
                      AddLog_P(LOG_LEVEL_DEBUG,PSTR("read characteristic"));
                    } else {
                      op->state = GEN_STATE_FAILED_CANTREAD;
                    }
                  }
                  if (op->writelen){
                    if(pCharacteristic->canRead()) {
                      if (!pCharacteristic->writeValue(op->dataToWrite, op->writelen, true)){
                        op->state = GEN_STATE_FAILED_WRITE;
                        AddLog_P(LOG_LEVEL_DEBUG,PSTR("characteristic write fail"));
                      } else {
                        op->state = GEN_STATE_WRITEDONE; // overwritten on failure
                        AddLog_P(LOG_LEVEL_DEBUG,PSTR("write characteristic"));
                      }
                    } else {
                      op->state = GEN_STATE_FAILED_CANTWRITE;
                    }
                  }
                  // print or do whatever you need with the value
                  
                } else {
                  op->state = GEN_STATE_FAILED_NO_RW_CHAR;
                  AddLog_P(LOG_LEVEL_DEBUG,PSTR("characteristic not in service"));
                }
              }

              // disconnect if not waiting for notify, 
              if (!op->notifytimer){
                AddLog_P(LOG_LEVEL_DEBUG,PSTR("disconnect in runoperation"));
                try {
                  op->pClient->disconnect();
                } catch(const std::exception& e){
                  AddLog_P(LOG_LEVEL_DEBUG,PSTR("except in disconnect"));
                }
                try {
                  NimBLEDevice::deleteClient(op->pClient);
                } catch(const std::exception& e){
                  AddLog_P(LOG_LEVEL_DEBUG,PSTR("except in delete"));
                }
                op->pClient = NULL;
              }    

            } else {
              op->state = GEN_STATE_FAILED_CONNECT;
              // failed to connect
              AddLog_P(LOG_LEVEL_DEBUG,PSTR("fauled to connect to device"));
            }            
          } else {
            op->state = GEN_STATE_FAILED_NOSERVICE;
            AddLog_P(LOG_LEVEL_DEBUG,PSTR("service not in device"));
          }
          break;
        }
       
    }
    if (i == deviceCount){
      // device not present
      op->state = GEN_STATE_FAILED_NODEVICE;
      AddLog_P(LOG_LEVEL_DEBUG,PSTR("device not present"));
    }
  }
}



void CmndBLEMode(void){
  switch(XdrvMailbox.index){
    case BLEModeDisabled:
      BLENextMode = BLEModeDisabled;
      if (BLEMode != BLEModeDisabled){
        StopBLE();
        BLEMode = BLEModeDisabled;
      }
      break;
    case BLEModeByCommand:
      BLENextMode = BLEModeByCommand;
      if (BLEMode == BLEModeDisabled){
        StartBLE();
        BLEMode = BLEModeByCommand;
      }
      break;
    case BLEModeActive:
      BLENextMode = BLEModeActive;
      if (BLEMode == BLEModeDisabled){
        StartBLE();
        BLEMode = BLEModeActive;
      }
      break;
    default:
      break;
  }
  ResponseCmndDone();
}




//////////////////////////////////////////////////////////////////////////
// Command to cause BLE read/write/notify operations to be run.
//////////////////////////////////////////////////////////////////////////

// we expect BLEOp0 - poll state
// we expect BLEOp1 <mac>
// we expect BLEOp2 <service>
// we expect BLEOp3 <characteristic>
// we expect BLEOp4 <hex data to write>
// we expect BLEOp5 - request a read
// we expect BLEOp6 <notifycharacteristic> - if specified, then it waits for a notify
// we expect BLEOp10 1|2|3 - trigger start by setting state to this.
// we expect BLEOp11 1 - cancel op

// all cmds will fail if one is in progress
void CmndBLEOperation(void){

  int op = XdrvMailbox.index;

  AddLog_P(LOG_LEVEL_INFO,PSTR("op %d"), op);

  int res = -1;

  // if in progress, only op 0 or 11 are allowed
  switch(op) {
    case 0:
      AddLog_P(LOG_LEVEL_INFO,PSTR("preview"));
      BLE99::BLEPostMQTT(false); // show all operations, not just completed
      break;
    case 1:
      if (prepOperation){
        delete prepOperation;
      }
      prepOperation = new generic_sensor_t;
      memset(prepOperation, 0, sizeof(generic_sensor_t));
      strncpy(prepOperation->MAC, XdrvMailbox.data, sizeof(prepOperation->MAC)-1);
      AddLog_P(LOG_LEVEL_INFO,PSTR("MAC Set %s"),prepOperation->MAC);
      break;
    case 2:
      if (!prepOperation) {
        ResponseCmndNumber(res);
        return;      
      }
      strncpy(prepOperation->serviceStr, XdrvMailbox.data, sizeof(prepOperation->serviceStr)-1);
      AddLog_P(LOG_LEVEL_INFO,PSTR("serviceStr Set %s"),prepOperation->serviceStr);
      break;
    case 3:
      if (!prepOperation) {
        ResponseCmndNumber(res);
        return;      
      }
      strncpy(prepOperation->characteristicStr, XdrvMailbox.data, sizeof(prepOperation->characteristicStr)-1);
      AddLog_P(LOG_LEVEL_INFO,PSTR("characteristicStr Set %s"),prepOperation->characteristicStr);
      break;
    case 4:
      if (!prepOperation) {
        ResponseCmndNumber(res);
        return;      
      }
      prepOperation->writelen = fromHex(prepOperation->dataToWrite, XdrvMailbox.data, sizeof(prepOperation->dataToWrite));
      AddLog_P(LOG_LEVEL_INFO,PSTR("dataToWrite Set %s"),XdrvMailbox.data);
      break;
    case 5:
      if (!prepOperation) {
        ResponseCmndNumber(res);
        return;      
      }
      prepOperation->readlen = 1;
      AddLog_P(LOG_LEVEL_INFO,PSTR("readlen Set %d"),prepOperation->readlen);
      break;
    case 6:
      if (!prepOperation) {
        ResponseCmndNumber(res);
        return;      
      }
      strncpy(prepOperation->notificationCharacteristicStr, XdrvMailbox.data, sizeof(prepOperation->notificationCharacteristicStr)-1);
      AddLog_P(LOG_LEVEL_INFO,PSTR("notififcationCharacteristicStr Set %s"),prepOperation->notificationCharacteristicStr);
      break;

    case 9:
      AddLog_P(LOG_LEVEL_INFO,PSTR("preview"));
      BLE99::BLEPostMQTT(false); // show all operations, not just completed
      break;
    case 10:
      if (!prepOperation) {
        ResponseCmndNumber(res);
        return;      
      }
      //prepOperation->requestType = atoi(XdrvMailbox.data);
      prepOperation->state = GEN_STATE_START; // trigger request later
      prepOperation->opid = lastopid++;

      AddLog_P(LOG_LEVEL_INFO,PSTR("State:%d -> queued:%d"), prepOperation->state, queuedOperations.size());

      // this will set prepOperaiton to null
      addOperation(&queuedOperations, &prepOperation);
      BLE99::BLEPostMQTT(false);
      break;
    /*case 11:
      if (!currentOperation) {
        ResponseCmndNumber(res);
        return;      
      }
      currentOperation->cancel = atoi(XdrvMailbox.data);
      AddLog_P(LOG_LEVEL_INFO,PSTR("cancel Set"));
      break;*/

      case 99: { // test programatically added operation
        generic_sensor_t *op = new generic_sensor_t;
        memset(op, 0, sizeof(generic_sensor_t));
        strncpy(op->MAC, "001A22092EE0", sizeof(op->MAC)-1);
        strncpy(op->serviceStr, "3e135142-654f-9090-134a-a6ff5bb77046", sizeof(op->serviceStr)-1);
        strncpy(op->characteristicStr, "3fa4585a-ce4a-3bad-db4b-b8df8179ea09", sizeof(op->characteristicStr)-1);
        strncpy(op->notificationCharacteristicStr, "d0e8434d-cd29-0996-af41-6c90f4e0eb2a", sizeof(op->notificationCharacteristicStr)-1);
        op->writelen = fromHex(op->dataToWrite, (char *)"4040", sizeof(op->dataToWrite));
        op->callback = (void *)myOpCallback2;
        BLE99::extQueueOperation(&op);

        // dump what we have as diags
        BLE99::BLEPostMQTT(false); // show all operations, not just completed
      } break;
  }

  res = 100;
  ResponseCmndDone();
  return;
}


/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

static void BLEPostMQTT(bool onlycompleted) {
  if (prepOperation || completedOperations.size() || queuedOperations.size() || currentOperations.size()){
    AddLog_P(LOG_LEVEL_INFO,PSTR("some to show"));
    xSemaphoreTake(BLEOperationsMutex, portMAX_DELAY);
    if (prepOperation && !onlycompleted){
      std::string out = BLETriggerResponse(prepOperation);
      xSemaphoreGive(BLEOperationsMutex); // release mutex
      snprintf_P(TasmotaGlobal.mqtt_data, sizeof(TasmotaGlobal.mqtt_data), PSTR("%s"), out.c_str());
      MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);
      AddLog_P(LOG_LEVEL_INFO,PSTR("prep sent %s"), out.c_str());
    } else {
      xSemaphoreGive(BLEOperationsMutex); // release mutex
    }
  

    if (queuedOperations.size() && !onlycompleted){
      AddLog_P(LOG_LEVEL_INFO,PSTR("queued %d"), queuedOperations.size());
      for (int i = 0; i < queuedOperations.size(); i++){
        xSemaphoreTake(BLEOperationsMutex, portMAX_DELAY);
        generic_sensor_t *toSend = queuedOperations[i];
        if (!toSend) {
          xSemaphoreGive(BLEOperationsMutex); // release mutex
          break;
        } else {
          std::string out = BLETriggerResponse(toSend);
          xSemaphoreGive(BLEOperationsMutex); // release mutex
          snprintf_P(TasmotaGlobal.mqtt_data, sizeof(TasmotaGlobal.mqtt_data), PSTR("%s"), out.c_str());
          MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);
          AddLog_P(LOG_LEVEL_INFO,PSTR("queued %d sent %s"), i, out.c_str());
          //break;
        }
      }
    }

    if (currentOperations.size() && !onlycompleted){
      AddLog_P(LOG_LEVEL_INFO,PSTR("current %d"), currentOperations.size());
      for (int i = 0; i < currentOperations.size(); i++){
        xSemaphoreTake(BLEOperationsMutex, portMAX_DELAY);
        generic_sensor_t *toSend = currentOperations[i];
        if (!toSend) {
          xSemaphoreGive(BLEOperationsMutex); // release mutex
          break;
        } else {
          std::string out = BLETriggerResponse(toSend);
          xSemaphoreGive(BLEOperationsMutex); // release mutex
          snprintf_P(TasmotaGlobal.mqtt_data, sizeof(TasmotaGlobal.mqtt_data), PSTR("%s"), out.c_str());
          MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);
          AddLog_P(LOG_LEVEL_INFO,PSTR("curr %d sent %s"), i, out.c_str());
          //break;
        }
      }
    }

    if (completedOperations.size()){
      AddLog_P(LOG_LEVEL_INFO,PSTR("completed %d"), completedOperations.size());
      do {
        generic_sensor_t *toSend = nextOperation(&completedOperations);
        if (!toSend) {
          break;
        } else {
          if (toSend->state != GEN_STATE_TODELETE){
            std::string out = BLETriggerResponse(toSend);
            snprintf_P(TasmotaGlobal.mqtt_data, sizeof(TasmotaGlobal.mqtt_data), PSTR("%s"), out.c_str());
            MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);
            toSend->state = GEN_STATE_TODELETE;
          }
          //break;
        }
        //break;
      } while (1);
    }
  } else {
    snprintf_P(TasmotaGlobal.mqtt_data, sizeof(TasmotaGlobal.mqtt_data), PSTR("{\"BLEOperation\":{}}"));
    MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);
  }
}

static void mainThreadOpCallbacks() {
  if (completedOperations.size()){
    //AddLog_P(LOG_LEVEL_INFO,PSTR("completed %d"), completedOperations.size());

    xSemaphoreTake(BLEOperationsMutex, portMAX_DELAY);
    // find this operation in currentOperations, and remove it.
    // in reverse so we can erase them safely.
    for (int i = completedOperations.size()-1; i >= 0 ; i--){
      generic_sensor_t *op = completedOperations[i];

      bool callbackres = false;

      if (op->callback){
        try {
          OPCOMPLETE_CALLBACK *pFn = (OPCOMPLETE_CALLBACK *)(op->callback);
          callbackres = pFn(op);
          AddLog_P(LOG_LEVEL_DEBUG,PSTR("op->callback %d"), callbackres);
        } catch(const std::exception& e){
          AddLog_P(LOG_LEVEL_ERROR,PSTR("exception in op->callback"));
        }
      }

      if (!callbackres){
        for (int i = 0; i < operationsCallbacks.size(); i++){
          try {
            OPCOMPLETE_CALLBACK *pFn = operationsCallbacks[i];
            callbackres = pFn(op);
            AddLog_P(LOG_LEVEL_DEBUG,PSTR("operationsCallbacks %d %d"), i, callbackres);
            if (callbackres){
              break; // this callback ate the op.
            }
          } catch(const std::exception& e){
            AddLog_P(LOG_LEVEL_ERROR,PSTR("exception in operationsCallbacks"));
          }
        }
      }

      // if some callback told us not to send on MQTT, then remove from completed and delete the data
      if (callbackres){
        AddLog_P(LOG_LEVEL_DEBUG,PSTR("callbackres true -> delete op"));
        completedOperations.erase(completedOperations.begin() + i);
        delete op;
      }
    }
    xSemaphoreGive(BLEOperationsMutex); // release mutex
  }
}


static void BLEShow(bool json)
{

  AddLog_P(LOG_LEVEL_INFO,PSTR("show json %d"),json);

  return;
  if (json){

  }
#ifdef USE_WEBSERVER
  else {
  //WSContentSend_PD(HTTP_MI32, i+1,stemp,MIBLEsensors.size());
  }
#endif  // USE_WEBSERVER
    
}


/**
 * @brief creates a JSON representing a single operation.
 *
 */
std::string BLETriggerResponse(generic_sensor_t *toSend){
  char temp[100];
  char t[10];
  if (!toSend) return "";
  ResponseClear();
  std::string out = "{\"BLEOperation\":{\"opid\":\"";
  sprintf(t, "%d", toSend->opid);
  out = out + t;
  out = out + ",\"state\":\"";
  sprintf(t, "%d", toSend->state);
  out = out + t;
  out = out + ",\"MAC\":\"";
  out = out + toSend->MAC;
  out = out + "\",\"svc\":\"";
  out = out + toSend->serviceStr;
  out = out + "\",\"char\":\"";
  out = out + toSend->characteristicStr;
  if (toSend->readlen){
    dump(temp, 99, toSend->dataRead, toSend->readlen);
    if (toSend->readtruncated){
      strcat(temp, "+");
    }
    out = out + "\",\"read\":\"";
    out = out + temp;
  }
  if (toSend->writelen){
    dump(temp, 99, toSend->dataToWrite, toSend->writelen);
    out = out + "\",\"wrote\":\"";
    out = out + temp;
  }
  if (toSend->notifylen){
    dump(temp, 99, toSend->dataNotify, toSend->notifylen);
    if (toSend->notifytruncated){
      strcat(temp, "+");
    }
    out = out + "\",\"notify\":\"";
    out = out + temp;
  }
  out = out + "}}";
  return out;
}



}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns99(uint8_t function)
{
  if (!Settings.flag5.mi32_enable) { return false; }  // SetOption115 - Enable ESP32 BLE BLE

  bool result = false;

  if (FUNC_INIT == function){
    BLE99::BLEPreInit();
  }

  if (!BLE99::BLE.mode.init) {
    if (function == FUNC_EVERY_250_MSECOND) {
      BLE99::BLEInit();
    }
    return result;
  }
  switch (function) {
    case FUNC_EVERY_50_MSECOND:
      BLE99::BLEEvery50mSecond();
      break;
    case FUNC_EVERY_SECOND:
      BLE99::BLEEverySecond(false);
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(BLE99::kBLE_Commands, BLE99::BLE_Commands);
      break;
    case FUNC_JSON_APPEND:
      BLE99::BLEShow(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      BLE99::BLEShow(0);
      break;
#endif  // USE_WEBSERVER
    }
  return result;
}
#endif  // USE_MI_ESP32
#endif  // ESP32
