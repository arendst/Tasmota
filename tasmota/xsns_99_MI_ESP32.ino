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
  uint8_t state;
  uint32_t opid; // incrementing id so we can find them
  
  uint8_t cancel; 
  uint8_t requestType; 
  char MAC[13];
  char serviceStr[100];
  char characteristicStr[100];
  char notificationCharacteristicStr[100];
  int RSSI;
  uint64_t time;
  uint8_t dataToWrite[100];
  uint8_t writelen;
  uint8_t dataRead[100];
  uint8_t readlen;
  uint8_t readtruncated;
  uint8_t dataNotify[100];
  uint8_t notifylen;
  uint8_t notifytruncated;

  NimBLEClient *pClient;


};


void doneOperation(generic_sensor_t** op);
void addOperation(std::deque<generic_sensor_t*> *ops, generic_sensor_t** op);
generic_sensor_t* nextOperation(std::deque<generic_sensor_t*> *ops);
std::string BLETriggerResponse(generic_sensor_t *toSend);
static void BLEscanEndedCB(NimBLEScanResults results);
static void BLEnotifyCB(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);
static void BLEGenNotifyCB(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);
static void runCurrentOperation(generic_sensor_t** pCurrentOperation);
static void BLEShow(bool json);
static void BLEPostMQTT(bool json);
static void BLEStartOperationTask();


#define GEN_STATE_IDLE 0
#define GEN_STATE_START 1

#define GEN_STATE_READWRITEDONE 8
#define GEN_STATE_WAITNOTIFY 9
#define GEN_STATE_NOTIFIED 10
#define GEN_STATE_CANTNOTIFY 11

#define GEN_STATE_FAILED 99

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

generic_sensor_t* currentOperation = nullptr;
// operation being prepared through commands
generic_sensor_t* prepOperation = nullptr;

// operations which have been queued
std::deque<generic_sensor_t*> queuedOperations;
// operations in progress (at the moment, only one)
std::deque<generic_sensor_t*> currentOperations;
// operaitons which have completed or failed, ready to send to MQTT
std::deque<generic_sensor_t*> completedOperations;

uint32_t lastopid = 0;

enum {
  BLEModeDisabled = 0, // BLE is disabled
  BLEModeByCommand = 1, // BLE is activeated by commands only
  BLEModeActive = 2, // BLE is scanning all the time 
};

uint8_t BLEMode = 2;
uint8_t BLENextMode = 2;

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
  CMND_BLE_OP                // connect/read/write/notify operations
  };

enum BLE_TASK {
       BLE_TASK_SCAN = 0,
       BLE_TASK_CONN = 1,
       BLE_TASK_TIME = 2,
       BLE_TASK_BATT = 3,
       BLE_TASK_UNIT = 4,
};


static const char h[] PROGMEM = "0123456789ABCDEF";
void hex(char *dest, uint8_t v){
  *(dest++) = h[(v>>4)&0xf];
  *(dest++) = h[v&0xf];
  *(dest) = 0;
}

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


/*********************************************************************************************\
 * Classes
\*********************************************************************************************/

class BLESensorCallback : public NimBLEClientCallbacks {
  void onConnect(NimBLEClient* pclient) {
    AddLog_P(LOG_LEVEL_DEBUG,PSTR("onConnect"));
    BLE.mode.willConnect = 0;
    BLE.mode.connected = 1;
  }
  void onDisconnect(NimBLEClient* pclient) {
    AddLog_P(LOG_LEVEL_DEBUG,PSTR("onDisconnect"));
    BLE.mode.connected = 0;
    BLE.mode.willReadBatt = 0;
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

    int RSSI = advertisedDevice->getRSSI();
    uint8_t addr[6];
    memcpy(addr,advertisedDevice->getAddress().getNative(),6);
    MI32_ReverseMAC(addr);
    char addrstr[20];
    dump(addrstr, 20, addr, 6);

    int svcDataCount = advertisedDevice->getServiceDataCount();
    int svcUUIDCount = advertisedDevice->getServiceUUIDCount();

    uint8_t* payload = advertisedDevice->getPayload();
    size_t payloadlen = advertisedDevice->getPayloadLength();
    char payloadhex[100];
    dump(payloadhex, 100, payload, payloadlen);

    if (advertisedDevice->haveName()){
      std::string name = advertisedDevice->getName();
      AddLog_P(LOG_LEVEL_DEBUG,PSTR("Name: %s"), name.c_str());
    }

    if (advertisedDevice->haveManufacturerData()){
      std::string data = advertisedDevice->getManufacturerData();
      char mfgstr[100];
      int len = data.length();
      dump(mfgstr, 100, (uint8_t*)data.c_str(), len);

      AddLog_P(LOG_LEVEL_DEBUG,PSTR("Mfg: %s"), mfgstr);
    }

    if (svcDataCount == 0) {
      AddLog_P(LOG_LEVEL_DEBUG,PSTR("%s RSSI:%d payloadonly %s"), addrstr, RSSI, payloadhex);
      return;
    }


    AddLog_P(LOG_LEVEL_DEBUG,PSTR("%s RSSI:%d payload %s service data:"), addrstr, RSSI, payloadhex);
    for (int i = 0; i < svcDataCount; i++){
      NimBLEUUID UUID = advertisedDevice->getServiceDataUUID(i);//.getNative()->u16.value;
      std::string ServiceData = advertisedDevice->getServiceData(i);
      
      size_t ServiceDataLength = ServiceData.length();
      const char *serviceData = ServiceData.data();
      char svcdatastr[80];
      dump(svcdatastr, 80, (uint8_t*)serviceData, ServiceDataLength);
      //char svcuuidstr[20];
      std::string strUUID = UUID;
      //int uuidLen = strUUID.length();
      //dump(svcuuidstr, 20, (uint8_t*)strUUID.c_str(), uuidLen);
     
      const ble_uuid_any_t* native = advertisedDevice->getServiceDataUUID(i).getNative();
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
      
      AddLog_P(LOG_LEVEL_DEBUG,PSTR("Service UUID:%s"), strUUID.c_str());
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

    for (int i = 0; i < length && i < sizeof(currentOperation->dataNotify); i++){
      thisop->dataNotify[i] = pData[i];
    }
    thisop->notifylen = length;
    if (length > sizeof(thisop->dataNotify)){
      thisop->notifytruncated = 1;
    } else {
      thisop->notifytruncated = 0;
    }
    thisop->state = GEN_STATE_NOTIFIED;
}


/*********************************************************************************************\
 * Helper functions
\*********************************************************************************************/

/**
 * @brief Remove all colons from null terminated char array
 *
 * @param _string Typically representing a MAC-address like AA:BB:CC:DD:EE:FF
 */
static void BLEstripColon(char* _string){
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

static void BLEHexStringToBytes(char* _string, uint8_t* _byteArray) {
  BLEstripColon(_string);
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
static void BLE_ReverseMAC(uint8_t _mac[]){
  uint8_t _reversedMAC[6];
  for (uint8_t i=0; i<6; i++){
    _reversedMAC[5-i] = _mac[i];
  }
  memcpy(_mac,_reversedMAC, sizeof(_reversedMAC));
}



/*********************************************************************************************\
 * common functions
\*********************************************************************************************/


/*********************************************************************************************\
 * init NimBLE
\*********************************************************************************************/
static void BLEStartScanTask();

static void BLEPreInit(void) {
  BLE.mode.init = false;
  currentOperation = nullptr;
  prepOperation = nullptr;
  BLEOperationsMutex = xSemaphoreCreateMutex();

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

static void BLEOperationTask(void *pvParameters){
  uint32_t timer = 0;
  for(;;){
    if (BLEScan){
      AddLog_P(LOG_LEVEL_DEBUG,PSTR("Operation loop"));

      BLE99::runCurrentOperation(&currentOperation);
    }
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
  AddLog_P(LOG_LEVEL_DEBUG,PSTR("one sec scnning %d"),BLE.mode.runningScan);

  // one op per second for now;
  //BLE99::runCurrentOperation(&currentOperation);

  if (completedOperations.size()){
    BLE99::BLEPostMQTT(true);
  }


  if (BLEScan){
    if (!BLE.mode.runningScan && (!currentOperations.size())){
      //BLEScan->clearResults();
      if ((BLEMode != BLEModeDisabled) && (BLEMode != BLEModeByCommand)){
        AddLog_P(LOG_LEVEL_DEBUG,PSTR("Startscan from onesec"));
        BLEScan->start(BLEScanTimeS, BLEscanEndedCB, true); // 20s scans, restarted when theyn finish
        BLE.mode.runningScan = 1;
        BLEScanStartetdAt = esp_timer_get_time();
      }
    } else {
      uint64_t now = esp_timer_get_time();
      if (BLEScanTimeS){
        uint64_t diff = now - BLEScanStartetdAt;
        diff = diff / 1000; // convert to ms;
        if (diff > (BLEScanTimeS + 5) * 1000){
          AddLog_P(LOG_LEVEL_ERROR,PSTR("Scan did not end on t1me"));
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
  bool onOff = atoi(XdrvMailbox.data);
  switch(XdrvMailbox.index) {
    case 0:
      BLE.option.allwaysAggregate = onOff;
      break;
    case 1:
      BLE.option.noSummary = onOff;
      break;
    case 2:
      BLE.option.directBridgeMode = onOff;
      break;
  }
  ResponseCmndDone();
}


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

void doneOperation(generic_sensor_t** op){
  if ((*op)->pClient){
    AddLog_P(LOG_LEVEL_DEBUG,PSTR("disconnect in done"));
    try {
      (*op)->pClient->disconnect();
    } catch(const std::exception& e){
      AddLog_P(LOG_LEVEL_DEBUG,PSTR("except in disconnect"));
    }
    try {
      NimBLEDevice::deleteClient((*op)->pClient);
    } catch(const std::exception& e){
      AddLog_P(LOG_LEVEL_DEBUG,PSTR("except in delete"));
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

  addOperation(&completedOperations, op);
  return;
}


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





static void runCurrentOperation(generic_sensor_t** pCurrentOperation){
  if (!pCurrentOperation) return;
  if (!BLEScan) return;

  if (!*pCurrentOperation) {
    *pCurrentOperation = nextOperation(&queuedOperations);
    if (*pCurrentOperation){
      generic_sensor_t* temp = *pCurrentOperation;
      //this will null it out, so ave and restore.
      addOperation(&currentOperations, pCurrentOperation); 
      *pCurrentOperation = temp;
    }
  }
  if (!*pCurrentOperation) return;

  switch((*pCurrentOperation)->state){
    case GEN_STATE_CANTNOTIFY:
      // if it took too long, then disconnect
      AddLog_P(LOG_LEVEL_DEBUG,PSTR("notify timeout"));
      (*pCurrentOperation)->state = GEN_STATE_FAILED;
      doneOperation(pCurrentOperation);
      break;

    case GEN_STATE_WAITNOTIFY:{
      // if it took too long, then disconnect
      uint64_t now = esp_timer_get_time();
      uint64_t diff = now - (*pCurrentOperation)->time;
      diff = diff/1000;
      if (diff > 20000){
        AddLog_P(LOG_LEVEL_DEBUG,PSTR("notify timeout"));
        (*pCurrentOperation)->state = GEN_STATE_FAILED;
        doneOperation(pCurrentOperation);
      }
    } break;

    case GEN_STATE_READWRITEDONE:
      // post result data
      AddLog_P(LOG_LEVEL_DEBUG,PSTR("readwrite done"));
      doneOperation(pCurrentOperation);
      break;

    case GEN_STATE_NOTIFIED:
      doneOperation(pCurrentOperation);
      // post result data
      AddLog_P(LOG_LEVEL_DEBUG,PSTR("notify done"));
      break;

    case GEN_STATE_START:
      // continue to start the process here.
      break;

    case GEN_STATE_FAILED:
      // post fail data
      AddLog_P(LOG_LEVEL_DEBUG,PSTR("operation failed"));
      doneOperation(pCurrentOperation);
      break;

    default:
      return;
      break;
  }

  if (!*pCurrentOperation) return;
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
                  NimBLERemoteCharacteristic *pCharacteristic = 
                    pService->getCharacteristic(op->characteristicStr);
                  AddLog_P(LOG_LEVEL_DEBUG,PSTR("got service"));
                  op->state = GEN_STATE_READWRITEDONE;

                  if (op->notificationCharacteristicStr){
                    NimBLERemoteCharacteristic *pNCharacteristic = 
                      pService->getCharacteristic(op->notificationCharacteristicStr);
                    if (pNCharacteristic != nullptr) {
                      op->notifylen = 0;
                      if(pNCharacteristic->canNotify()) {
                        if(pNCharacteristic->subscribe(true, BLE99::BLEGenNotifyCB)) {
                          AddLog_P(LOG_LEVEL_DEBUG,PSTR("subscribe for notify"));
                          op->state = GEN_STATE_WAITNOTIFY;
                          uint64_t now = esp_timer_get_time();
                          op->time = now;
                        } else {
                          AddLog_P(LOG_LEVEL_DEBUG,PSTR("failed subscribe for notify"));
                          op->state = GEN_STATE_FAILED;
                        }
                      } else {
                        if(pNCharacteristic->canIndicate()) {
                          if(pNCharacteristic->subscribe(false, BLE99::BLEGenNotifyCB)) {
                            AddLog_P(LOG_LEVEL_DEBUG,PSTR("subscribe for indicate"));
                            op->state = GEN_STATE_WAITNOTIFY;
                            uint64_t now = esp_timer_get_time();

                            op->time = now;
                          } else {
                            AddLog_P(LOG_LEVEL_DEBUG,PSTR("failed subscribe for indicate"));
                            op->state = GEN_STATE_FAILED;
                          }
                        }
                      }
                    }
                  }

                  if (pCharacteristic != nullptr) {
                    AddLog_P(LOG_LEVEL_DEBUG,PSTR("got characteristic"));

                    if (op->readlen){
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
                      AddLog_P(LOG_LEVEL_DEBUG,PSTR("read characteristic"));
                    }
                    if (op->writelen){
                      if (!pCharacteristic->writeValue(op->dataToWrite, op->writelen, true)){
                        op->state = GEN_STATE_FAILED;
                        AddLog_P(LOG_LEVEL_DEBUG,PSTR("characteristic write fail"));
                      } else {
                        AddLog_P(LOG_LEVEL_DEBUG,PSTR("write characteristic"));
                      }
                    }
                    // print or do whatever you need with the value
                    
                  } else {
                    op->state = GEN_STATE_FAILED;
                    AddLog_P(LOG_LEVEL_DEBUG,PSTR("characteristic not in service"));
                  }
                }

                switch (op->state){
                  case GEN_STATE_WAITNOTIFY:
                    break;
                  default:
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

                    break;
                } 
            } else {
              op->state = GEN_STATE_FAILED;
              // failed to connect
              AddLog_P(LOG_LEVEL_DEBUG,PSTR("fauled to connect to device"));
            }            
          } else {
            op->state = GEN_STATE_FAILED;
            AddLog_P(LOG_LEVEL_DEBUG,PSTR("service not in device"));
          }
          break;
        }
       
    }
    if (i == deviceCount){
      // device not present
      op->state = GEN_STATE_FAILED;
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
        BLEMode = BLEModeByCommand;
      }
      break;
    default:
      break;
  }
  ResponseCmndDone();
}



// we expect BLEOp0 - poll state
// we expect BLEOp1 <mac>
// we expect BLEOp2 <service>
// we expect BLEOp3 <characteristic>
// we expect BLEOp4 <hex data to write>
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
      BLE99::BLEPostMQTT(true);
      break;
    case 10:
      if (!prepOperation) {
        ResponseCmndNumber(res);
        return;      
      }
      //prepOperation->requestType = atoi(XdrvMailbox.data);
      prepOperation->state = GEN_STATE_START; // trigger request later
      prepOperation->opid = lastopid++;

      AddLog_P(LOG_LEVEL_INFO,PSTR("State Set %d -> %d"), prepOperation->state, queuedOperations.size());

      // this will set prepOperaiton to null
      addOperation(&queuedOperations, &prepOperation);
      BLE99::BLEPostMQTT(true);
      break;
    case 11:
      if (!currentOperation) {
        ResponseCmndNumber(res);
        return;      
      }
      currentOperation->cancel = atoi(XdrvMailbox.data);
      AddLog_P(LOG_LEVEL_INFO,PSTR("cancel Set"));
      break;
  }

  res = 100;
  if (currentOperation){
    res = currentOperation->state;
  }
  ResponseCmndDone();
  return;
}


/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

const char HTTP_BLE[] PROGMEM = "{s}MI ESP32 v0917a{m}%u%s / %u{e}";
const char HTTP_BLE_MAC[] PROGMEM = "{s}%s %s{m}%s{e}";

static void BLEPostMQTT(bool json) {

  if (prepOperation || completedOperations.size() || queuedOperations.size() || currentOperations.size()){
    AddLog_P(LOG_LEVEL_INFO,PSTR("some to show"));
    xSemaphoreTake(BLEOperationsMutex, portMAX_DELAY);
    if (prepOperation){
      std::string out = BLETriggerResponse(prepOperation);
      xSemaphoreGive(BLEOperationsMutex); // release mutex
      snprintf_P(TasmotaGlobal.mqtt_data, sizeof(TasmotaGlobal.mqtt_data), PSTR("%s"), out.c_str());
      MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);
      AddLog_P(LOG_LEVEL_INFO,PSTR("prep sent %s"), out.c_str());
    } else {
      xSemaphoreGive(BLEOperationsMutex); // release mutex
    }

    if (currentOperations.size()){
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

    if (queuedOperations.size()){
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
    
    if (completedOperations.size()){
      AddLog_P(LOG_LEVEL_INFO,PSTR("completed %d"), completedOperations.size());
      do {
        generic_sensor_t *toSend = nextOperation(&completedOperations);
        if (!toSend) {
          break;
        } else {
          std::string out = BLETriggerResponse(toSend);
          snprintf_P(TasmotaGlobal.mqtt_data, sizeof(TasmotaGlobal.mqtt_data), PSTR("%s"), out.c_str());
          MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);
          delete toSend;
          //break;
        }
        //break;
      } while (1);
    }
  } else {
    snprintf_P(TasmotaGlobal.mqtt_data, sizeof(TasmotaGlobal.mqtt_data), PSTR("{\"opcount\":%d}"), 0);
    MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);
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
 * @brief trigger real-time message for PIR or RC
 *
 */
std::string BLETriggerResponse(generic_sensor_t *toSend){
  char temp[100];
  char t[10];
  if (!toSend) return "";
  ResponseClear();
  std::string out = "{\"opid\":\"";
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
  out = out + "}";
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
