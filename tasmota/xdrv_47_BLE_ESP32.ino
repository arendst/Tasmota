/*
  xdrv_47_BLE_ESP32.ino - BLE via ESP32 support for Tasmota

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

/*
  xdrv_47:
  This driver uses the ESP32 BLE functionality to hopefully provide enough
  BLE functionality to implement specific drivers on top of it.

  As a generic driver, it can:
    Be asked to 
      connect/write to a MAC/Service/Characteristic
      connect/read from a MAC/Service/Characteristic
      connect/write/awaitnotify from a MAC/Service/Characteristic/NotifyCharacteristic
      connect/read/awaitnotify from a MAC/Service/Characteristic/NotifyCharacteristic

    Cmnds:
      BLEOp0 - requests status of operations
      BLEOp1 MAC - create an operation in preparation, and populate it's MAC address
      BLEOp2 Service - add a serviceUUID to the operation in preparation
      BLEOp3 Characteristic - add a CharacteristicUUID to the operation in preparation for read/write
      BLEOp4 writedata - optional:add data to write to the operation in preparation - hex string
      BLEOp5 - optional:signify that a read should be done
      BLEOp6 NotifyCharacteristic - optional:add a NotifyCharacteristicUUID to the operation in preparation to wait for a notify
      BLEOp9 - publish the 'operation in preparation' to MQTT.
      BLEOp10 - add the 'operation in preparation' to the queue of operations to perform.

  Other drivers can add callbacks to receive advertisments
  Other drivers can add 'operations' to be performed and receive callbacks from the operation's success or failure

Example:
Write and request next notify:
backlog BLEOp1 001A22092EE0; BLEOp2 3e135142-654f-9090-134a-a6ff5bb77046; BLEOp3 3fa4585a-ce4a-3bad-db4b-b8df8179ea09; BLEOp4 03; BLEOp6 d0e8434d-cd29-0996-af41-6c90f4e0eb2a;
BLEOp10 -> 
19:25:08 RSL: tele/tasmota_E89E98/SENSOR = {"BLEOperation":{"opid":"3,"state":"1,"MAC":"001A22092EE0","svc":"3e135142-654f-9090-134a-a6ff5bb77046","char":"3fa4585a-ce4a-3bad-db4b-b8df8179ea09","wrote":"03}}
19:25:08 queued 0 sent {"BLEOperation":{"opid":"3,"state":"1,"MAC":"001A22092EE0","svc":"3e135142-654f-9090-134a-a6ff5bb77046","char":"3fa4585a-ce4a-3bad-db4b-b8df8179ea09","wrote":"03}}
19:25:08 RSL: stat/tasmota_E89E98/RESULT = {"BLEOp":"Done"}
.....
19:25:11 RSL: tele/tasmota_E89E98/SENSOR = {"BLEOperation":{"opid":"3,"state":"11,"MAC":"001A22092EE0","svc":"3e135142-654f-9090-134a-a6ff5bb77046","char":"3fa4585a-ce4a-3bad-db4b-b8df8179ea09","wrote":"03","notify":"020109000428}}

state: 1 -> starting, 
7 -> read complete
8 -> write complete 
11 -> notify complete
-ve + -> failure (see GEN_STATE_FAILED_XXXX constants below.)


The driver can also be used by other drivers, using the functions:

void registerForAdvertismentCallbacks(char *loggingtag, ADVERTISMENT_CALLBACK* pFn);
void registerForOpCallbacks(char *loggingtag, OPCOMPLETE_CALLBACK* pFn);
bool extQueueOperation(generic_sensor_t** op);

These allow other code to
  receive advertisments
  receive operation callbacks.
  create and start an operation, and get a callback when done/failed.

i.e. the Bluetooth of the ESP can be shared without conflict.

*/


// TEMPORARILY define ESP32 and USE_BLE_ESP32 so VSCODE shows highlighting....
//#define VSCODE_DEV

#ifdef VSCODE_DEV
#define ESP32
#define USE_BLE_ESP32
#endif

#ifdef ESP32                       // ESP32 only. Use define USE_HM10 for ESP8266 support

#ifdef USE_BLE_ESP32

#define XDRV_47                    47
#define USE_MI_DECRYPTION

#include <vector>
#include <deque>
#include <string.h>
#include <cstdarg>
#ifdef USE_MI_DECRYPTION
#include <t_bearssl.h>
#endif //USE_MI_DECRYPTION

#include <NimBLEDevice.h>
#include <NimBLEAdvertisedDevice.h>
#include "NimBLEEddystoneURL.h"
#include "NimBLEEddystoneTLM.h"
#include "NimBLEBeacon.h"

// from ble_gap.c
extern "C" void ble_gap_conn_broken(uint16_t conn_handle, int reason);

void installExamples();
void sendExample();

namespace BLE_ESP32 {


// generic sensor type used as during
// connect/read/wrtie/notify operations
// only one operation will happen at a time 

#pragma pack( push, 0 )  // aligned structures for speed.  but be sepcific

/////////////////////////////////////////////////////
// states for runTaskDoneOperation
#define GEN_STATE_IDLE 0
#define GEN_STATE_START 1
#define GEN_STATE_STARTED 2

#define GEN_STATE_READDONE 3
#define GEN_STATE_WRITEDONE 4
#define GEN_STATE_WAITNOTIFY 5
#define GEN_STATE_WAITINDICATE 6

#define GEN_STATE_NOTIFIED 7


// Errors are all base on 0x100
#define GEN_STATE_FAILED -1
#define GEN_STATE_FAILED_CANTNOTIFYORINDICATE -2
#define GEN_STATE_FAILED_CANTREAD -3
#define GEN_STATE_FAILED_CANTWRITE -4
#define GEN_STATE_FAILED_NOSERVICE -5
#define GEN_STATE_FAILED_NO_RW_CHAR -6
#define GEN_STATE_FAILED_NONOTIFYCHAR -7
#define GEN_STATE_FAILED_NOTIFYTIMEOUT -8
#define GEN_STATE_FAILED_READ -9
#define GEN_STATE_FAILED_WRITE -10
#define GEN_STATE_FAILED_CONNECT -11
#define GEN_STATE_FAILED_NOTIFY -12
#define GEN_STATE_FAILED_INDICATE -13
#define GEN_STATE_FAILED_NODEVICE -14
#define GEN_STATE_FAILED_NOREADWRITE -15
#define GEN_STATE_FAILED_CANCEL -16
//
/////////////////////////////////////////////////////


#define MAX_BLE_DATA_LEN 100
struct generic_sensor_t {
  int16_t state;
  uint32_t opid; // incrementing id so we can find them
  uint64_t notifytimer;
  
  // uint8_t cancel; 
  // uint8_t requestType; 
  NimBLEAddress addr;
  NimBLEUUID serviceUUID;
  NimBLEUUID characteristicUUID;
  NimBLEUUID notificationCharacteristicUUID;
  uint8_t dataToWrite[MAX_BLE_DATA_LEN];
  uint8_t writelen;
  uint8_t dataRead[MAX_BLE_DATA_LEN];
  uint8_t readlen;
  uint8_t readtruncated;
  uint8_t dataNotify[MAX_BLE_DATA_LEN];
  uint8_t notifylen;
  uint8_t notifytruncated;

  // NOTE!!!: this callback is called DIRECTLY from the operation task, so be careful about cross-thread access of data
  // if is called after read, so that you can do a read/modify/write operation on a characteristic.
  // i.e. modify dataToWrite and writelen according to what you see in readData and readlen.
  // for a normal read, please use the OPCOMPLETE_CALLBACK 'completecallback'
  // normally null
  void *readmodifywritecallback; // READ_CALLBACK function, used by external drivers

  void *completecallback; // OPCOMPLETE_CALLBACK function, used by external drivers
  void *context; // opaque context, used by external drivers, or can be set to a long for MQTT
};


////////////////////////////////////////////////////////////////
// structure for callbacks from other drivers from advertisments.
struct ble_advertisment_t {
  BLEAdvertisedDevice *advertisedDevice; // the full NimBLE advertisment, in case people need MORE info.
  uint32_t totalCount; 

  uint8_t addr[6];
  int RSSI;
  char name[20];
};

#pragma pack( pop )  // byte-aligned structures to read the sensor data

////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// External interface to this driver for use by others.
//
// callback types to be used by external drivers
//
// returns - 
//  0 = let others see this, 
//  1 = I processed this, no need to give it to the next callback
//  2 = I want this device erased from the scan
typedef int ADVERTISMENT_CALLBACK(BLE_ESP32::ble_advertisment_t *pStruct);
// returns - 0 = let others see this, 1 = I processed this, no need to give it to the next callback, or post on MQTT
typedef int OPCOMPLETE_CALLBACK(BLE_ESP32::generic_sensor_t *pStruct);

// NOTE!!!: this callback is called DIRECTLY from the operation task, so be careful about cross-thread access of data
// if is called after read, so that you can do a read/modify/write operation on a characteristic.
typedef int READ_CALLBACK(BLE_ESP32::generic_sensor_t *pStruct);

typedef int SCANCOMPLETE_CALLBACK(NimBLEScanResults results);

// tag is just a name for logging
void registerForAdvertismentCallbacks(const char *tag, BLE_ESP32::ADVERTISMENT_CALLBACK* pFn);
void registerForOpCallbacks(const char *tag, BLE_ESP32::OPCOMPLETE_CALLBACK* pFn);
void registerForScanCallbacks(const char *tag, BLE_ESP32::SCANCOMPLETE_CALLBACK* pFn);

////////////////////////////////////////////////////
// BLE operations: these are currently 'new'ed and 'delete'ed.
// in the future, they may be allocated from some constant menory store to avoid fragmentation.
// so PLEASE don't create or destroy them yourselves.
// create a new BLE operation. 
int newOperation(BLE_ESP32::generic_sensor_t** op);
// free a BLE operation - this should be done if you did not call extQueueOperation for some reason
int freeOperation(BLE_ESP32::generic_sensor_t** op);
// queue a BLE operation - it will happen some time in the future. 
// Note: you do not need to free an operation once it have been queued.  it will be freed by the driver.
int extQueueOperation(BLE_ESP32::generic_sensor_t** op);
const char * getStateString(int state);
///////////////////////////////////////////////////////////////////////


// a temporay safe logging mechanism.  This has a max of 40 chars, and a max of 15 slots per 50ms
int SafeAddLog_P(uint32_t loglevel, PGM_P formatP, ...);



////////////////////////////////////////////////////////////////////////
// utilities
// dump a binary to hex
char * dump(char *dest, int maxchars, const uint8_t *src, int len);

//////////////////////////////////////////
// automutex.
// create a mute in your driver with:
// void *mutex = nullptr;
// BLE_ESP32::BLEAutoMutex::init(&mutex);
//
// then protect any function with
// BLE_ESP32::BLEAutoMutex m(mutex);
// - it will be automagically released when the funtcion is over.
// - the same thread can take multiple times (recursive).
// - advanced options m.give() and m.take() allow you fine control within a function.
class BLEAutoMutex {
  SemaphoreHandle_t mutex;
  bool taken;
  public:
    BLEAutoMutex(void * mutex, bool take=true);
    ~BLEAutoMutex();
    void give();
    void take();
    static void init(void ** ptr);
};
//////////////////////////////////////////





struct BLE_simple_device_t {
  uint8_t mac[6];
  char name[20];
  int RSSI;
  uint64_t lastseen;
  uint8_t getAdvert;
};





// this protects our queues, which can be accessed by multiple tasks 
SemaphoreHandle_t  BLEOperationsRecursiveMutex;



// only run from main thread, because it deletes things that were newed there...
static void mainThreadOpCallbacks();
static void mainThreadBLETimeouts();

int addOperation(std::deque<BLE_ESP32::generic_sensor_t*> *ops, BLE_ESP32::generic_sensor_t** op);
BLE_ESP32::generic_sensor_t* nextOperation(std::deque<BLE_ESP32::generic_sensor_t*> *ops);
std::string BLETriggerResponse(BLE_ESP32::generic_sensor_t *toSend);
static void BLEscanEndedCB(NimBLEScanResults results);
static void BLEGenNotifyCB(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);

// this is called from the advert callback, be careful
void BLEPostAdvert(ble_advertisment_t *Advertisment);
static void BLEPostMQTTSeenDevices();

static void BLEShow(bool json);
static void BLEPostMQTT(bool json);
static void BLEStartOperationTask();

// these are only run from the run task
static void BLETaskRunCurrentOperation(BLE_ESP32::generic_sensor_t** pCurrentOperation, NimBLEClient **ppClient);
static void BLETaskRunTaskDoneOperation(BLE_ESP32::generic_sensor_t** op, NimBLEClient **ppClient);
int BLETaskStartScan(int time);


// these are run from main thread
static void StartBLE(void);
static void StopBLE(void);

// called from advert callback
void setDetails(ble_advertisment_t *ad);

//#define EXAMPLE_ADVERTISMENT_CALLBACK
//#define EXAMPLE_OPERATION_CALLBACK

#ifdef EXAMPLE_ADVERTISMENT_CALLBACK
int myAdvertCallback(BLE_ESP32::ble_advertisment_t *pStruct);
#endif
#ifdef EXAMPLE_OPERATION_CALLBACK
int myOpCallback(BLE_ESP32::generic_sensor_t *pStruct);
int myOpCallback2(BLE_ESP32::generic_sensor_t *pStruct);
#endif


// single storage for advert callbacks....
static ble_advertisment_t BLEAdvertisment;


//////////////////////////////////////////////////
// general variables for running the driver
TaskHandle_t TasmotaMainTask; 


static int BLEMasterEnable = 0;
static int BLEInitState = 0;
static int BLERunningScan = 0;
static uint32_t BLEScanCount = 0;
static bool BLEScanActiveMode = false;

static int BLEPublishDevices = 0; // causes MQTT publish of device list (each scan end)
static BLEScan* ble32Scan = nullptr;
bool BLERunning = false;
// time we last started a scan in uS using esp_timer_get_time();
// used to setect a scan which did not call back?
uint64_t BLEScanStartedAt = 0; 
uint64_t BLEScanToEndBefore = 0;

int BLEMaxTaskLoopTime = 120; // we expect the task to NOT take > 120s per loop!!!
uint64_t BLELastLoopTime = 0;
int BLEScanTimeS = 20; // scan duraiton in S
int BLEMaxTimeBetweenAdverts = 120; // we expect an advert at least this frequently, else restart BLE (in S)
uint64_t BLEScanLastAdvertismentAt = 0;
uint32_t lastopid = 0; // incrementing uinique opid
uint32_t BLEResets = 0;
// controls request of details about one device
uint8_t BLEDetailsRequest = 0;
uint8_t BLEDetailsMac[6];

// triggers send for ALL operations known about
uint8_t BLEPostMQTTTrigger = 0;




//////////////////////////////////////////////////


// operation being prepared through commands
BLE_ESP32::generic_sensor_t* prepOperation = nullptr;

// operations which have been queued
std::deque<BLE_ESP32::generic_sensor_t*> queuedOperations;
// operations in progress (at the moment, only one)
std::deque<BLE_ESP32::generic_sensor_t*> currentOperations;
// operations which have completed or failed, ready to send to MQTT
std::deque<BLE_ESP32::generic_sensor_t*> completedOperations;

// seen devices
#define MAX_BLE_DEVICES_LOGGED 20
std::deque<BLE_ESP32::BLE_simple_device_t*> seenDevices;
std::deque<BLE_ESP32::BLE_simple_device_t*> freeDevices;



// list of registered callbacks for advertisments
// register using void registerForAdvertismentCallbacks(const char *somename ADVERTISMENT_CALLBACK* pFN);
std::deque<BLE_ESP32::ADVERTISMENT_CALLBACK*> advertismentCallbacks;

std::deque<BLE_ESP32::OPCOMPLETE_CALLBACK*> operationsCallbacks;

std::deque<BLE_ESP32::SCANCOMPLETE_CALLBACK*> scancompleteCallbacks;

/*********************************************************************************************\
 * constants
\*********************************************************************************************/

#define D_CMND_BLE "BLE"

const char kBLE_Commands[] PROGMEM = D_CMND_BLE "|"
  "Period|Adv|Op|Mode|Details|Scan";

static void CmndBLEPeriod(void);
static void CmndBLEAdv(void);
static void CmndBLEOperation(void);
static void CmndBLEMode(void);
static void CmndBLEDetails(void);
static void CmndBLEScan(void);

void (*const BLE_Commands[])(void) PROGMEM = {
  &BLE_ESP32::CmndBLEPeriod, &BLE_ESP32::CmndBLEAdv, &BLE_ESP32::CmndBLEOperation, &BLE_ESP32::CmndBLEMode, &BLE_ESP32::CmndBLEDetails, &BLE_ESP32::CmndBLEScan };

const char *successStates[] PROGMEM = {
  PSTR("IDLE"), // 0
  PSTR("START"),
  PSTR("STARTED"),
  PSTR("DONEREAD"),
  PSTR("DONEWRITE"),
  PSTR("WAITNOTIFY"),
  PSTR("WAITINDICATE"),
  PSTR("DONENOTIFIED") // 7
};

const char *failStates[] PROGMEM = {
  PSTR("IDLE"), //0
  PSTR("FAILED"), //-1
  PSTR("FAILCANTNOTIFYORINDICATE"),
  PSTR("FAILCANTREAD"),
  PSTR("FAILCANTWRITE"),
  PSTR("FAILNOSERVICE"),
  PSTR("FAILNORWCHAR"), //-6
  PSTR("FAILNONOTIFYCHAR"),
  PSTR("FAILNOTIFYTIMEOUT"),
  PSTR("FAILEREAD"),
  PSTR("FAILWRITE"),
  PSTR("FAILCONNECT"),
  PSTR("FAILNOTIFY"),
  PSTR("FAILINDICATE"),
  PSTR("FAILNODEVICE"),
  PSTR("FAILNOREADWRITE"),
  PSTR("FAILCANCEL")// -16
};

const char * getStateString(int state){
  if ((state >= 0) && (state < sizeof(successStates)/sizeof(*successStates))){
    return successStates[state];
  }

  state = -state;
  if ((state >= 0) && (state < sizeof(failStates)/sizeof(*failStates))){
    return failStates[state];
  }

  return PSTR("STATEINVALID");
} 

/*********************************************************************************************\
 * enumerations
\*********************************************************************************************/

enum BLE_Commands {          // commands useable in console or rules
  CMND_BLE_PERIOD,           // set period like TELE-period in seconds between read-cycles
  CMND_BLE_ADV,             // change advertisment options at runtime
  CMND_BLE_OP,                // connect/read/write/notify operations
  CMND_BLE_MODE,              // change mode of ble - BLE_MODES 
  CMND_BLE_DETAILS,           // get details for one device's adverts 
  CMND_BLE_SCAN              // Scan control
  };

enum {
  BLEModeDisabled = 0, // BLE is disabled
  BLEModeScanByCommand = 1, // BLE is activeated by commands only
  BLEModeRegularScan = 2, // BLE is scanning all the time 
} BLE_SCAN_MODES;

// values of BLEAdvertMode
enum {
  BLE_NO_ADV_SEND = 0, // driver is silent on MQTT regarding adverts
  BLE_ADV_TELE = 1, // driver sends a summary at tele period
  //BLE_ADV_ALL = 2,  // driver sends every advert with full data to MQTT
} BLEADVERTMODE;


uint8_t BLEMode = BLEModeRegularScan;
//uint8_t BLEMode = BLEModeScanByCommand;
uint8_t BLETriggerScan = 0;
uint8_t BLEAdvertMode = BLE_ADV_TELE;
uint8_t BLEdeviceLimitReached = 0;

uint8_t BLEStop = 0;
uint8_t BLERestartTasmota = 0;
uint8_t BLERestartNimBLE = 0;
const char *BLE_RESTART_TEAMOTA_REASON_UNKNOWN = PSTR("unknown");
const char *BLE_RESTART_TEAMOTA_REASON_RESTARTING_BLE_TIMEOUT = PSTR("restarting BLE took > 5s");
const char *BLE_RESTART_TEAMOTA_REASON_BLE_LOOP_STALLED = PSTR("BLE loop stalled > 120s");
const char *BLE_RESTART_TEAMOTA_REASON_BLE_DISCONNECT_FAIL = PSTR("BLE disconnect taking > 60s");
const char *BLERestartTasmotaReason = BLE_RESTART_TEAMOTA_REASON_UNKNOWN;

const char *BLE_RESTART_BLE_REASON_UNKNOWN = PSTR("unknown");
const char *BLE_RESTART_BLE_REASON_ADVERT_BLE_TIMEOUT = PSTR("no adverts in 120s");
const char *BLE_RESTART_BLE_REASON_CONN_LIMIT = PSTR("connect failed with connection limit reached");
const char *BLE_RESTART_BLE_REASON_CONN_EXISTS = PSTR("connect failed with connection exists");
const char *BLERestartBLEReason = nullptr;

BLEAutoMutex::BLEAutoMutex(void * mutex, bool take){
  if(mutex){
    if (take){
      xSemaphoreTakeRecursive(mutex, portMAX_DELAY);
      this->taken = true;
    }
    this->mutex = (SemaphoreHandle_t ) mutex;
  } else {
    this->mutex = (SemaphoreHandle_t )nullptr;
  }
}

BLEAutoMutex::~BLEAutoMutex(){
  if (this->mutex){
    if (this->taken){
      xSemaphoreGiveRecursive(this->mutex);
      this->taken = false;
    }
  }
}

void BLEAutoMutex::init(void ** ptr){
  SemaphoreHandle_t mutext = xSemaphoreCreateRecursiveMutex();
  (*ptr) = (void *) mutext;
}

void BLEAutoMutex::give(){
  if (this->mutex){
    if (this->taken){
      xSemaphoreGiveRecursive(this->mutex);
      this->taken= false;
    }
  }
}
void BLEAutoMutex::take(){
  if (this->mutex){
    if (!this->taken){
      xSemaphoreTakeRecursive(this->mutex, portMAX_DELAY);
      this->taken = true;
    }
  }
}

/*********************************************************************************************\
 * log of all devices present
\*********************************************************************************************/

void initSeenDevices(){
  /* added dynamically below, but never removed.
  for (int i = 0; i < MAX_BLE_DEVICES_LOGGED; i++){
    BLE_ESP32::BLE_simple_device_t* dev = new BLE_ESP32::BLE_simple_device_t;
    freeDevices.push_back(dev);
  }
  */
  return;
}

int addSeenDevice(const uint8_t *mac, const char *name, int RSSI){
  int res = 0;
  uint64_t now = esp_timer_get_time();
  BLEAutoMutex localmutex(BLEOperationsRecursiveMutex);

  int devicefound = 0;
  // do we already know this device?
  for (int i = 0; i < seenDevices.size(); i++){
    if (!memcmp(seenDevices[i]->mac, mac, 6)){
      seenDevices[i]->lastseen = now; 
      seenDevices[i]->RSSI = RSSI;
      if ((!seenDevices[i]->name[0]) && name[0]){
        strncpy(seenDevices[i]->name, name, sizeof(seenDevices[i]->name));
      }
      devicefound = 1;
      break;
    }
  }
  if (!devicefound){
    // if no free slots, add one if we have not reached our limit
    if (!freeDevices.size()){
      int total = seenDevices.size();
      if (total < MAX_BLE_DEVICES_LOGGED){
        BLE_ESP32::SafeAddLog_P(LOG_LEVEL_INFO,PSTR("new seendev slot %d"), total);

        BLE_ESP32::BLE_simple_device_t* dev = new BLE_ESP32::BLE_simple_device_t;
        freeDevices.push_back(dev);
      } else {
        // flag we hit the limit
        BLEdeviceLimitReached ++;
        if (BLEdeviceLimitReached >= 254){
          BLEdeviceLimitReached = 254;
        }
      }
    }

    // get a new device from the free list
    if (freeDevices.size()){
      BLE_ESP32::BLE_simple_device_t* dev = freeDevices[0];
      freeDevices.erase(freeDevices.begin());
      memcpy(dev->mac, mac, 6);
      strncpy(dev->name, name, sizeof(dev->name));
      dev->lastseen = now; 
      dev->RSSI = RSSI;
      seenDevices.push_back(dev);
      res = 2; // added
    }
  } else {
    res = 1; // already there
  }
  return res;
}

// remove devices from the seen list by age, and add them to the free list
// set ageS to 0 to delete all...
int deleteSeenDevices(int ageS = 0){
  int res = 0;
  uint64_t now = esp_timer_get_time();
  now = now/1000L;
  now = now/1000L;
  uint32_t nowS = (uint32_t)now;   
  uint32_t mintime = nowS - ageS;

  {
    BLEAutoMutex localmutex(BLEOperationsRecursiveMutex);

    for (int i = seenDevices.size()-1; i >= 0; i--){
        BLE_ESP32::BLE_simple_device_t* dev = seenDevices[i];
        uint64_t lastseen = dev->lastseen/1000L;
        lastseen = lastseen/1000L;
        uint32_t lastseenS = (uint32_t) lastseen; 
        uint32_t del_at = lastseenS + ageS;
        if ((del_at < nowS) || (ageS == 0)){
          AddLog_P(LOG_LEVEL_DEBUG,PSTR("delete device by age lastseen %u + maxage %u < now %u."), 
            lastseenS, ageS, nowS);
          seenDevices.erase(seenDevices.begin()+i);
          freeDevices.push_back(dev);
          res++;
        }
    }
  }
  if (res){
    AddLog_P(LOG_LEVEL_INFO,PSTR("BLE deleted %d devices"), res); 
  }
  return res;
}

int deleteSeenDevice(uint8_t *mac){
  int res = 0;
  BLEAutoMutex localmutex(BLEOperationsRecursiveMutex);
  for (int i = 0; i < seenDevices.size(); i++){
    if (!memcmp(seenDevices[i]->mac, mac, 6)){
      BLE_ESP32::BLE_simple_device_t* dev = seenDevices[i];
      seenDevices.erase(seenDevices.begin()+i);
      freeDevices.push_back(dev);
      res = 1;
      break;
    }
  }
  return res;
}



// the MAX we could expect.
#define MAX_DEV_JSON_NAME_LEN 20
#define MAX_DEV_JSON_RSSI_LEN 3
#define MAX_DEV_JSON_INDEX_LEN 3
// "001122334455":{"i":123,"n":"01234567890123456789","r":-77}\0
#define MIN_REQUIRED_DEVJSON_LEN \
  (1+12+1 + 1 + 1 + \
  +4 + MAX_DEV_JSON_INDEX_LEN \
  +1 + 4 + MAX_DEV_JSON_NAME_LEN + 2 \
  +1 + 4 + MAX_DEV_JSON_RSSI_LEN + 2 \
  +1 +1 \
  )
int getSeenDeviceToJson(int index, BLE_ESP32::BLE_simple_device_t* dev, char **dest, int *maxlen){
  char *p = *dest;
  if (*maxlen < MIN_REQUIRED_DEVJSON_LEN){
    return 0;
  }
  // add mac as key
  *((*dest)++) = '"';
  dump((*dest), 20, dev->mac, 6);
  (*dest) += 12;
  *((*dest)++) = '"';
  *((*dest)++) = ':';

  // add a structure, so we COULD add more than name later
  *((*dest)++) = '{';
  *((*dest)++) = '"';
  *((*dest)++) = 'i'; // index
  *((*dest)++) = '"';
  *((*dest)++) = ':';
  sprintf((*dest), "%d", index);
  (*dest) += strlen((*dest));
  *((*dest)++) = ',';
  *((*dest)++) = '"';
  *((*dest)++) = 'n';
  *((*dest)++) = '"';
  *((*dest)++) = ':';
  *((*dest)++) = '"';
  strncpy((*dest), dev->name, MAX_DEV_JSON_NAME_LEN);
  (*dest) += strlen((*dest));
  *((*dest)++) = '"';
  *((*dest)++) = ',';
  *((*dest)++) = '"';
  *((*dest)++) = 'r';
  *((*dest)++) = '"';
  *((*dest)++) = ':';
  sprintf((*dest), "%d", dev->RSSI);
  (*dest) += strlen((*dest));
  *((*dest)++) = '}';
  *maxlen -= (*dest - p);
  return 1;
}


int nextSeenDev = 0;

int getSeenDevicesToJson(char *dest, int maxlen){
  int res = 0;

  if ((nextSeenDev == 0) || (nextSeenDev >= seenDevices.size())){
    // delete devices not seen in last 240s
    deleteSeenDevices(240);
    nextSeenDev = 0;
  }

  // deliberate test of SafeAddLog_P from main thread...
  //BLE_ESP32::SafeAddLog_P(LOG_LEVEL_INFO,PSTR("getSeen %d"), seenDevices.size());


  int len;
  if (!maxlen) return 0;
  strcpy((dest), "{\"active\":{");
  len = strlen(dest);
  dest += len;
  maxlen -= len;

  int added = 0;
  BLEAutoMutex localmutex(BLEOperationsRecursiveMutex);

  snprintf((dest), maxlen-5, "\"total\":%d", seenDevices.size());
  len = strlen(dest);
  dest += len;
  maxlen -= len;
  added = 1; // trigger ','

  for (; nextSeenDev < seenDevices.size(); nextSeenDev++){
    if (maxlen > MIN_REQUIRED_DEVJSON_LEN + 3){
      if (added){
        *(dest++) = ',';
        maxlen--;
      }
      res += getSeenDeviceToJson(nextSeenDev, seenDevices[nextSeenDev], &dest, &maxlen);
      if (res) {
        added++;
      } else {
        if (added){
          dest--; // reverse out comma it the string did not get added
          maxlen++;
          break;
        }
      }
    } else {
      break;
    }
  }
  *(dest++) = '}';
  *(dest++) = '}';
  *(dest++) = 0;
  int remains = (seenDevices.size() - nextSeenDev);
  return remains;
} 




/*********************************************************************************************\
 * Mutex protected logging - max 5 logs of 40 chars
\*********************************************************************************************/

#define MAX_SAFELOG_LEN 80
#define MAX_SAFELOG_COUNT 30

struct safelogdata {
  int level;
  char log_data[MAX_SAFELOG_LEN];
};

std::deque<BLE_ESP32::safelogdata*> freelogs;
std::deque<BLE_ESP32::safelogdata*> filledlogs;
SemaphoreHandle_t  SafeLogMutex;


void initSafeLog(){
  TasmotaMainTask = xTaskGetCurrentTaskHandle();
  SafeLogMutex = xSemaphoreCreateMutex();

  for (int i = 0; i < MAX_SAFELOG_COUNT; i++){
    BLE_ESP32::safelogdata* logdata = new BLE_ESP32::safelogdata;
    freelogs.push_back(logdata);
  }
}

int SafeAddLog_P(uint32_t loglevel, PGM_P formatP, ...) {
  TaskHandle_t thistask = xTaskGetCurrentTaskHandle();
  int added = 0;

  // if the log would not be output do nothing here.
  if ((loglevel > Settings.weblog_level) &&
      (loglevel > TasmotaGlobal.seriallog_level) && 
      (loglevel > Settings.mqttlog_level) &&
      (loglevel > TasmotaGlobal.syslog_level)){
    return added;
  }

  // as these are'expensive', let's not bother unless they are lower than the serial log level
  xSemaphoreTake(SafeLogMutex, portMAX_DELAY);
  if (freelogs.size()){
    BLE_ESP32::safelogdata* logdata = (freelogs)[0]; 
    freelogs.pop_front();
    if (freelogs.size() > 1){
      int maxlen = sizeof(logdata->log_data);
      if (thistask == TasmotaMainTask){
        maxlen -= 13; // room for "-!MAINTHREAD!"
      }
      // assume this is thread safe - it may not be
      va_list arg;
      va_start(arg, formatP);
      vsnprintf_P(logdata->log_data, maxlen, formatP, arg);
      va_end(arg);
      logdata->level = loglevel;
      if (thistask == TasmotaMainTask){
        loglevel = LOG_LEVEL_ERROR;
        snprintf(logdata->log_data + strlen(logdata->log_data), 13, "-!MAINTHREAD!");
      }
      added = 1;
    } else {
      logdata->level = LOG_LEVEL_ERROR;
      strncpy(logdata->log_data, "SafeAddLog_P overflow", sizeof(logdata->log_data));
    }
    filledlogs.push_back(logdata);
  } else {
    // can't log it?
  }
  xSemaphoreGive(SafeLogMutex); // release mutex

  return added;
}

BLE_ESP32::safelogdata* GetSafeLog() {
  xSemaphoreTake(SafeLogMutex, portMAX_DELAY);
  if (filledlogs.size()){
    BLE_ESP32::safelogdata* logdata = (filledlogs)[0]; 
    filledlogs.pop_front();
    xSemaphoreGive(SafeLogMutex); // release mutex
    return logdata;
  }
  xSemaphoreGive(SafeLogMutex); // release mutex
  return nullptr;
}

void ReleaseSafeLog(BLE_ESP32::safelogdata* logdata){
  xSemaphoreTake(SafeLogMutex, portMAX_DELAY);
  freelogs.push_back(logdata);
  xSemaphoreGive(SafeLogMutex); // release mutex
}


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
char * dump(char *dest, int maxchars, const uint8_t *src, int len){
  int lenmax = (maxchars-1)/2;
  int actuallen = 0;
  for (actuallen = 0; actuallen < lenmax && actuallen < len; actuallen++){
    if (actuallen < lenmax){
      hex(dest+actuallen*2, src[actuallen]);
    }
  }
  if (actuallen != len){
    *(dest+(actuallen*2)) = '+';
    *(dest+(actuallen*2)+1) = 0;
  }
  return dest;
}

// convert from a hex string to binary
int fromHex(uint8_t *dest, const char *src, int maxlen){
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
void ReverseMAC(uint8_t _mac[]){
  uint8_t _reversedMAC[6];
  for (uint8_t i=0; i<6; i++){
    _reversedMAC[5-i] = _mac[i];
  }
  memcpy(_mac,_reversedMAC, sizeof(_reversedMAC));
}




/*********************************************************************************************\
 * Advertisment details
\*********************************************************************************************/

//ble_advertisment_t BLEAdvertismentDetails;
#define MAX_ADVERT_DETAILS 200
char BLEAdvertismentDetailsJson[MAX_ADVERT_DETAILS];
uint8_t BLEAdvertismentDetailsJsonSet = 0;
uint8_t BLEAdvertismentDetailsJsonLost = 0;


void setDetails(ble_advertisment_t *ad){
  BLEAutoMutex localmutex(BLEOperationsRecursiveMutex);
  if (BLEAdvertismentDetailsJsonSet){
    BLEAdvertismentDetailsJsonLost = 1;
    return;
  }
  char *p = BLEAdvertismentDetailsJson;
  int maxlen = sizeof(BLEAdvertismentDetailsJson);
  // just in case someone tries to read whilst we are writing
  BLEAdvertismentDetailsJson[sizeof(BLEAdvertismentDetailsJson)-1] = 0;

  *(p++) = '{';
  maxlen--;
  strcpy(p, "\"details\":{");
  int len = strlen(p);
  p += len;
  maxlen -= len;

  strcpy(p, "\"mac\":\"");
  len = strlen(p);
  p += len;
  maxlen -= len;
  dump(p, 14, ad->addr, 6);
  len = strlen(p);
  p += len;
  maxlen -= len;
  *(p++) = '\"'; maxlen--;

  if (BLEAdvertismentDetailsJsonLost){
    BLEAdvertismentDetailsJsonLost = 0;
    strcpy(p, ",\"lost\":true");
    len = strlen(p);
    p += len;
    maxlen -= len;
  }

  BLEAdvertisedDevice *advertisedDevice = ad->advertisedDevice;

  uint8_t* payload = advertisedDevice->getPayload();
  size_t payloadlen = advertisedDevice->getPayloadLength();
  if (payloadlen  && (maxlen > 30)){ // will truncate if not enough space
    strcpy(p, ",\"p\":\"");
    p += 6;
    maxlen -= 6;
    dump(p, maxlen-10, payload, payloadlen);
    int len = strlen(p);
    p += len;
    maxlen -= len;
    *(p++) = '\"'; maxlen--;
  }

  int svcdataCount = advertisedDevice->getServiceDataCount();
  if (svcdataCount){
    for (int i = 0; i < svcdataCount; i++){
      NimBLEUUID UUID = advertisedDevice->getServiceDataUUID(i);//.getNative()->u16.value;
      std::string ServiceData = advertisedDevice->getServiceData(i);
      
      size_t ServiceDataLength = ServiceData.length();
      const uint8_t *serviceData = (const uint8_t *)ServiceData.data();

      //char svcuuidstr[20];
      std::string strUUID = UUID;

      int svclen = strUUID.length();
      svclen++; // ,
      svclen += 3; // "":
      svclen += ServiceDataLength*2;
      svclen += 3; // ""}

      if (maxlen -10 > svclen){
        *(p++) = ',';
        *(p++) = '\"';
        strcpy(p, strUUID.c_str());
        p += strUUID.length();
        *(p++) = '\"';
        *(p++) = ':';
        *(p++) = '\"';
        dump(p, ServiceDataLength*2+2, (uint8_t*)serviceData, ServiceDataLength);
        int len = strlen(p);
        p += len;
        *(p++) = '\"';
        maxlen -= len;
      } 
    }
  }

  *(p++) = '}'; maxlen--;
  *(p++) = '}'; maxlen--;
  *(p++) = 0; maxlen--;
  
  BLEAdvertismentDetailsJsonSet = 1;
}


// call from main thread only!
// post advertisment detail if available, then clear.
void postAdvertismentDetails(){
  BLEAutoMutex localmutex(BLEOperationsRecursiveMutex);
  if (BLEAdvertismentDetailsJsonSet){
    strncpy(TasmotaGlobal.mqtt_data, BLEAdvertismentDetailsJson, sizeof(TasmotaGlobal.mqtt_data));
    BLEAdvertismentDetailsJsonSet = 0;
    // no retain - this is present devices, not historic
    MqttPublishPrefixTopic_P(TELE, PSTR("BLE"), 0);
  } else {
  }
}



/*********************************************************************************************\
 * Classes
\*********************************************************************************************/

// does not really take any action
class BLESensorCallback : public NimBLEClientCallbacks {
  void onConnect(NimBLEClient* pClient) {
    BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("onConnect %s"), ((std::string)pClient->getPeerAddress()).c_str());
  }
  void onDisconnect(NimBLEClient* pClient) {
    BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("onDisconnect %s"), ((std::string)pClient->getPeerAddress()).c_str());
  }
  bool onConnParamsUpdateRequest(NimBLEClient* pClient, const ble_gap_upd_params* params) {
    BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("onConnParamsUpdateRequest %s"), ((std::string)pClient->getPeerAddress()).c_str());


//    if(params->itvl_min < 24) { /** 1.25ms units */
//      return false;
//    } else if(params->itvl_max > 300) { /** 1.25ms units */
//      return false;
//    } else if(params->latency > 2) { /** Number of intervals allowed to skip */
//      return false;
//    } else if(params->supervision_timeout > 6000) { /** 10ms units */
//      return false;
//    }

/*
    if(params->itvl_min < 24) { // 1.25ms units
      return false;
    } else if(params->itvl_max > 40) { // 1.25ms units 
      return false;
    } else if(params->latency > 2) { // Number of intervals allowed to skip 
      return false;
    } else if(params->supervision_timeout > 200) { /// 10ms units 
      return false;
    }

    return true;
*/
    // just always reject thiers, and use ours.
    return false;

  }
};

static BLESensorCallback clientCB;


class BLEAdvCallbacks: public NimBLEAdvertisedDeviceCallbacks {
  void onResult(NimBLEAdvertisedDevice* advertisedDevice) {
    BLEAutoMutex localmutex(BLEOperationsRecursiveMutex);
    uint64_t now = esp_timer_get_time();
    BLEScanLastAdvertismentAt = now; // note the time of the last advertisment

    uint32_t totalCount = BLEAdvertisment.totalCount;
    memset(&BLEAdvertisment, 0, sizeof(BLEAdvertisment));
    BLEAdvertisment.totalCount = totalCount+1;

    BLEAdvertisment.advertisedDevice = advertisedDevice;

    int RSSI = advertisedDevice->getRSSI();
    NimBLEAddress address = advertisedDevice->getAddress(); 
    memcpy(BLEAdvertisment.addr, address.getNative(), 6);
    ReverseMAC(BLEAdvertisment.addr);
    
    BLEAdvertisment.RSSI = RSSI;

    char addrstr[20];
    dump(addrstr, 20, BLEAdvertisment.addr, 6);

    // this mjust survive the scope of the callbacks
    std::string name = "";
    const char *namestr = name.c_str();
    if (advertisedDevice->haveName()){
      name = advertisedDevice->getName();
      namestr = name.c_str();
      strncpy(BLEAdvertisment.name, namestr, sizeof(BLEAdvertisment.name));
    }

    // log this device safely
    addSeenDevice(BLEAdvertisment.addr, BLEAdvertisment.name, BLEAdvertisment.RSSI);


/* we don't put these fields in our structure anymore
    if (svcDataCount == 0) {
      BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR("ADV:%s (%s) RSSI:%d pld:%s Mfg:%s"), addrstr, namestr, RSSI, payloadhex, mfgstr);
    } else {
      BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR("ADV:%s (%s) RSSI:%d pld:%s Mfg:%s svcdata:"), addrstr, namestr, RSSI, payloadhex, mfgstr);
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
          if (ServiceDataLength > sizeof(BLEAdvertisment.svcdata[i].serviceData)){
            ServiceDataLength = sizeof(BLEAdvertisment.svcdata[i].serviceData);
          }
          memcpy(BLEAdvertisment.svcdata[i].serviceData, serviceData, ServiceDataLength );
          BLEAdvertisment.svcdata[i].serviceDataLen = ServiceDataLength;
          if (native->u.type == 16){
            BLEAdvertisment.svcdata[i].serviceUUID16 = native->u16.value;
          } else {
            BLEAdvertisment.svcdata[i].serviceUUID16 = 0;
          }
          strncpy(BLEAdvertisment.svcdata[i].serviceUUIDStr, strUUID.c_str(), sizeof(BLEAdvertisment.svcdata[i].serviceUUIDStr));
          BLEAdvertisment.svcdataCount = i+1;
        }
        uint16_t UUID16 = native->u16.value;
      // AddLog_P(LOG_LEVEL_DEBUG,PSTR("UUID: %x"),UUID);
        BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR("UUIDType:%d UUID16:%x UUID:%s svcdata:%s"), native->u.type, UUID16, strUUID.c_str(), svcdatastr);
      }

      for (int i = 0; i < svcUUIDCount; i++){
        NimBLEUUID UUID = advertisedDevice->getServiceUUID(i);//.getNative()->u16.value;
        //char svcuuidstr[20];
        std::string strUUID = UUID;
        //int uuidLen = strUUID.length();
        //dump(svcuuidstr, 20, (uint8_t*)strUUID.c_str(), uuidLen);
        const ble_uuid_any_t* native = UUID.getNative();
        if (i < 5){
          if (native->u.type == 16){
            BLEAdvertisment.services[i].serviceUUID16 = native->u16.value;
          } else {
            BLEAdvertisment.services[i].serviceUUID16 = 0;
          }
          strncpy(BLEAdvertisment.services[i].serviceUUIDStr, strUUID.c_str(), sizeof(BLEAdvertisment.services[i].serviceUUIDStr));
          BLEAdvertisment.serviceCount = i+1;
        }
        BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR("Svc UUID:%s"), strUUID.c_str());
      }
    }
*/

    if (BLEDetailsRequest){
      switch (BLEDetailsRequest){
        case 1:{ // one advert for one device
          BLEDetailsRequest = 0; // only one requested  if 2, it's a request all
          if (!memcmp(BLEDetailsMac, BLEAdvertisment.addr, 6)){
            setDetails(&BLEAdvertisment);
          }
        } break;
        case 2:{ // all adverts for one device - may not get them all
          if (!memcmp(BLEDetailsMac, BLEAdvertisment.addr, 6)){
            setDetails(&BLEAdvertisment);
          }
        } break;
        case 3:{ // all adverts for ALL DEVICES - may not get them all
          setDetails(&BLEAdvertisment);
        } break;
      }
    }

    // call anyone who asked about advertisments
    for (int i = 0; i < advertismentCallbacks.size(); i++) {
      try {
        ADVERTISMENT_CALLBACK* pFN;
        pFN = advertismentCallbacks[i];
        int res = pFN(&BLEAdvertisment);

        // if this callback wants to stop here, then do so.
        if (1 == res) break;

        // if this callback wants to kill this device
        if (2 == res) {
          //BLEScan->erase(address);
        }
      } catch(const std::exception& e){
        BLE_ESP32::SafeAddLog_P(LOG_LEVEL_ERROR,PSTR("exception in advertismentCallbacks"));
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
  BLERunningScan = 0;
  BLEScanToEndBefore = 0L;

  BLEScanCount++;

  BLEPublishDevices = 1;

  BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("Scan ended"));

  for (int i = 0; i < scancompleteCallbacks.size(); i++){
    try {
      SCANCOMPLETE_CALLBACK *pFn = scancompleteCallbacks[i];
      int callbackres = pFn(results);
      BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("scancompleteCallbacks %d %d"), i, callbackres);
    } catch(const std::exception& e){
      BLE_ESP32::SafeAddLog_P(LOG_LEVEL_ERROR,PSTR("exception in operationsCallbacks"));
    }
  }
}


///////////////////////////////////////////////////////////////////////
// !!!!!!!!!!@@@@@@@@@@@@@@@@
// NOTE: this can callback BEFORE the write is completed.
// so we should not do any actions against the device if we can help it
// this COULD be the reason for the BLE stack hanging up....
///////////////////////////////////////////////////////////////////////
static void BLEGenNotifyCB(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify){
  NimBLEClient *pRClient;

  BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("Notified length: %u"),length);

  // find the operation this is associated with
  NimBLERemoteService *pSvc = pRemoteCharacteristic->getRemoteService();

  pRClient = pSvc->getClient();
  NimBLEAddress devaddr = pRClient->getPeerAddress();

  generic_sensor_t *thisop = nullptr; 
  for (int i = 0; i < currentOperations.size(); i++){
    generic_sensor_t *op = currentOperations[i];
    if (devaddr == op->addr){
      thisop = op;
      break;
    }
  }

  // we'll try without
  //pRemoteCharacteristic->unsubscribe();

  if (!thisop){
    BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("no op for notify"));
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
  // we will NOT change the state here...
  // rely on thisop->notifylen as a flag notify is complete
  //thisop->state = GEN_STATE_NOTIFIED;

  // this triggers our notify complete, either at the end of read/write, or next 1s cycle.
  thisop->notifytimer = 0;

}




/*********************************************************************************************\
 * functions for registering callbacks against the driver
\*********************************************************************************************/

void registerForAdvertismentCallbacks(const char *tag, BLE_ESP32::ADVERTISMENT_CALLBACK* pFn){
  AddLog_P(LOG_LEVEL_INFO,PSTR("BLE: registerForAdvertismentCallbacks %s:%x"), tag, (uint32_t) pFn);

  advertismentCallbacks.push_back(pFn);
}

void registerForOpCallbacks(const char *tag, BLE_ESP32::OPCOMPLETE_CALLBACK* pFn){
  AddLog_P(LOG_LEVEL_INFO,PSTR("BLE: registerForOpCallbacks %s:%x"), tag, (uint32_t) pFn);
  operationsCallbacks.push_back(pFn);
}

void registerForScanCallbacks(const char *tag, BLE_ESP32::SCANCOMPLETE_CALLBACK* pFn){
  AddLog_P(LOG_LEVEL_INFO,PSTR("BLE: registerForScnCallbacks %s:%x"), tag, (uint32_t) pFn);
  scancompleteCallbacks.push_back(pFn);
}


/*********************************************************************************************\
 * init NimBLE
\*********************************************************************************************/
static void BLEPreInit(void) {
  BLEInitState = 0;
  prepOperation = nullptr;

  // allow the same task to take the mutex several times...
  BLEAutoMutex::init(&BLEOperationsRecursiveMutex);

//  BLEOperationsRecursiveMutex = xSemaphoreCreateRecursiveMutex();
}


static void BLEInit(void) {
  if (BLEMode == BLEModeDisabled) return;

  if (BLEInitState) { return; }

  if (TasmotaGlobal.global_state.wifi_down) { return; }
  if (WiFi.getSleep() == false) {
    AddLog_P(LOG_LEVEL_DEBUG,PSTR("BLE: WiFi modem not in sleep mode, BLE cannot start yet"));
    if (0 == Settings.flag3.sleep_normal) {
      AddLog_P(LOG_LEVEL_DEBUG,PSTR("BLE: About to restart to put WiFi modem in sleep mode"));
      Settings.flag3.sleep_normal = 1;  // SetOption60 - Enable normal sleep instead of dynamic sleep
      TasmotaGlobal.restart_flag = 2;
    }
    return;
  }


  // this is only for testing, does nothin if examples are undefed
  installExamples();
  initSafeLog();
  initSeenDevices();

  uint64_t now = esp_timer_get_time();
  BLEScanLastAdvertismentAt = now; // initialise the time of the last advertisment
  BLELastLoopTime = now;

  BLEInitState = 1;

  // dont start of disabled
  BLEMasterEnable = Settings.flag5.mi32_enable;
  if (!BLEMasterEnable) return;


  StartBLE();

  return;
}

/*********************************************************************************************\
 * Task section
\*********************************************************************************************/

static void BLEOperationTask(void *pvParameters);

static void BLEStartOperationTask(){
  if (BLERunning == false){
    AddLog_P(LOG_LEVEL_DEBUG,PSTR("%s: Start operations"),D_CMND_BLE);
    BLERunning = true;

    xTaskCreatePinnedToCore(
      BLE_ESP32::BLEOperationTask,    /* Function to implement the task */
      "BLEOperationTask",  /* Name of the task */
      8192,             /* Stack size in words */
      NULL,             /* Task input parameter */
      0,                /* Priority of the task */
      NULL,             /* Task handle. */
      1);               /* Core where the task should run */
  }
}


static void BLETaskStopStartNimBLE(NimBLEClient **ppClient, bool start = true){

  if (*ppClient){
    BLE_ESP32::SafeAddLog_P(LOG_LEVEL_ERROR,PSTR("BLETask:Stopping NimBLE"));

    (*ppClient)->setClientCallbacks(nullptr, false);

    try {
      if ((*ppClient)->isConnected()){
        BLE_ESP32::SafeAddLog_P(LOG_LEVEL_INFO,PSTR("disconnecting connected client"));
        (*ppClient)->disconnect();
      }
      NimBLEDevice::deleteClient((*ppClient));
      (*ppClient) = nullptr;
      BLE_ESP32::SafeAddLog_P(LOG_LEVEL_INFO,PSTR("deleted client"));
    } catch(const std::exception& e){
      BLE_ESP32::SafeAddLog_P(LOG_LEVEL_ERROR,PSTR("Stopping NimBLE:exception in delete client"));
    }

    if (ble32Scan){
      ble32Scan->setAdvertisedDeviceCallbacks(nullptr,true);
      ble32Scan->stop();
      ble32Scan = nullptr;
    }

    // wait second
    vTaskDelay(1000/ portTICK_PERIOD_MS);
    NimBLEDevice::deinit(true);
  }
  BLERunningScan = 0;

  if (start){
    BLE_ESP32::SafeAddLog_P(LOG_LEVEL_INFO,PSTR("BLETask:Starting NimBLE"));
    NimBLEDevice::init("BLE_ESP32");

    *ppClient = NimBLEDevice::createClient();
    (*ppClient)->setClientCallbacks(&clientCB, false);
    /** Set initial connection parameters: These settings are 15ms interval, 0 latency, 120ms timout. 
     *  These settings are safe for 3 clients to connect reliably, can go faster if you have less 
     *  connections. Timeout should be a multiple of the interval, minimum is 100ms.
     *  Min interval: 12 * 1.25ms = 15, Max interval: 12 * 1.25ms = 15, 0 latency, 51 * 10ms = 510ms timeout 
     */
    (*ppClient)->setConnectionParams(12,12,0,51);
    /** Set how long we are willing to wait for the connection to complete (seconds), default is 30. */
    (*ppClient)->setConnectTimeout(15);
  }
}

int BLETaskStartScan(int time){
  if (!ble32Scan) return -1;
  if (BLERunningScan) return -2;
  if (currentOperations.size()) return -3;
  if (BLEMode == BLEModeDisabled) return -4;

  BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("BLETask: Startscan"));
  //vTaskDelay(500/ portTICK_PERIOD_MS);
  ble32Scan->setActiveScan(BLEScanActiveMode);


  // seems we could get the callback within the start call....
  // so set these before starting
  BLERunningScan = 1;
  BLEScanStartedAt = esp_timer_get_time();
  if (BLETriggerScan){
    time = BLETriggerScan;
    BLETriggerScan = 0;
  }
  ble32Scan->start(time, BLEscanEndedCB, true); // 20s scans, restarted when then finish
  //vTaskDelay(500/ portTICK_PERIOD_MS);
  return 0;
}

// this runs one operation
// if the passed pointer is empty, it tries to get a next one.
static void BLETaskRunCurrentOperation(BLE_ESP32::generic_sensor_t** pCurrentOperation, NimBLEClient **ppClient){
  if (!pCurrentOperation) return;

  NimBLEClient *pClient = *ppClient;
  if (!*pCurrentOperation) {
    *pCurrentOperation = nextOperation(&queuedOperations);
    if (*pCurrentOperation){
      BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("BLETask: new currentOperation"));

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
    if (diff > 20000){ // 20s
      BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("BLETask: notify timeout"));
      (*pCurrentOperation)->state = GEN_STATE_FAILED_NOTIFYTIMEOUT;
      (*pCurrentOperation)->notifytimer = 0;
    }
    // we can't process any further, because op will be at state readdone or writedone
    return;
  }


  switch((*pCurrentOperation)->state){
    case GEN_STATE_WAITINDICATE:
    case GEN_STATE_WAITNOTIFY:
      //(*pCurrentOperation)->notifytimer == 0 at this point, so must be done
      (*pCurrentOperation)->state = GEN_STATE_NOTIFIED; 
      // just stay here until this is removed by the main thread
      BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("BLETask: notify operation complete"));
      BLE_ESP32::BLETaskRunTaskDoneOperation(pCurrentOperation, ppClient);
      pClient = *ppClient;
      return;
      break;
    case GEN_STATE_READDONE:
    case GEN_STATE_WRITEDONE:
    case GEN_STATE_NOTIFIED: // - may have completed DURING our read/write to get here
      // just stay here until this is removed by the main thread
      BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("BLETask: operation complete"));
      BLE_ESP32::BLETaskRunTaskDoneOperation(pCurrentOperation, ppClient);
      pClient = *ppClient;
      return;
      break;

    case GEN_STATE_START:
      // continue to start the process here.
      break;

    default:
      break;
  }


  if (!*pCurrentOperation) return;

  if ((*pCurrentOperation)->state <= GEN_STATE_FAILED){
    BLE_ESP32::SafeAddLog_P(LOG_LEVEL_ERROR,PSTR("BLETask: op failed %d"), (*pCurrentOperation)->state);
    BLE_ESP32::BLETaskRunTaskDoneOperation(pCurrentOperation, ppClient);
    pClient = *ppClient;
    return;
  }

  if ((*pCurrentOperation)->state != GEN_STATE_START){
    return;
  }

  if (pClient->isConnected()){
    // don't do anything if we are still connected
    BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("BLETask: still connected"));
    return;
  }


  // if we managed to run operations back to back with long connection timeouts,
  // then we may NOT see advertisments.
  // so to prevent triggering of the advert timeout restart mechanism, 
  // set the last advert time each time we start an operation
  uint64_t now = esp_timer_get_time();
  BLEScanLastAdvertismentAt = now; // initialise the time of the last advertisment


  generic_sensor_t* op = *pCurrentOperation;

  int newstate = GEN_STATE_STARTED;
  op->state = GEN_STATE_STARTED;

  BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("BLETask: attempt connect %s"), ((std::string)op->addr).c_str());
  if (pClient->connect(op->addr, true)) {

    BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("connected %s -> getservice"), ((std::string)op->addr).c_str());
    NimBLERemoteService *pService = pClient->getService(op->serviceUUID);
    int waitNotify = false;
    int notifystate = 0;
    op->notifytimer = 0L;

    if (pService != nullptr) {
      BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("got service"));

      // pre-set to fail if no operations requested
      //newstate = GEN_STATE_FAILED_NOREADWRITE;

      ///////////////////////////////////////////////////////////////////////
      // !!!!!!!!!!@@@@@@@@@@@@@@@@
      // NOTE: Notify callback can happen BEFORE the read/write is completed.
      // this COULD be the reason for the BLE stack hanging up....
      ///////////////////////////////////////////////////////////////////////

      // if we have been asked to get a notification
      if (op->notificationCharacteristicUUID.bitSize()) {
        NimBLERemoteCharacteristic *pNCharacteristic = 
          pService->getCharacteristic(op->notificationCharacteristicUUID);
        if (pNCharacteristic != nullptr) {
          BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("got notify characteristic"));
          op->notifylen = 0;
          if(pNCharacteristic->canNotify()) {
            if(pNCharacteristic->subscribe(true, BLE_ESP32::BLEGenNotifyCB)) {
              BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("subscribe for notify"));
              uint64_t now = esp_timer_get_time();
              op->notifytimer = now;
              // this will get changed to read or write,
              // but here in case it's notify only (can that happen?)
              notifystate = GEN_STATE_WAITNOTIFY;
              waitNotify = true;
            } else {
              BLE_ESP32::SafeAddLog_P(LOG_LEVEL_ERROR,PSTR("failed subscribe for notify"));
              newstate = GEN_STATE_FAILED_NOTIFY;
            }
          } else {
            if(pNCharacteristic->canIndicate()) {
              if(pNCharacteristic->subscribe(false, BLE_ESP32::BLEGenNotifyCB)) {
                BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("subscribe for indicate"));
                notifystate = GEN_STATE_WAITINDICATE;
                uint64_t now = esp_timer_get_time();
                op->notifytimer = now;
                waitNotify = true;
              } else {
                BLE_ESP32::SafeAddLog_P(LOG_LEVEL_ERROR,PSTR("failed subscribe for indicate"));
                newstate = GEN_STATE_FAILED_INDICATE;
              }
            } else {
              newstate = GEN_STATE_FAILED_CANTNOTIFYORINDICATE;
              BLE_ESP32::SafeAddLog_P(LOG_LEVEL_ERROR,PSTR("characteristic can't notify"));
            }
          }
        } else {
          newstate = GEN_STATE_FAILED_NONOTIFYCHAR;
          BLE_ESP32::SafeAddLog_P(LOG_LEVEL_ERROR,PSTR("notify characteristic not found"));
        }

        // force the 'error' of the notify coming in before the read/write for testing
        //vTaskDelay(1000/ portTICK_PERIOD_MS);
      } // no supplied notify char is ok

      // this will only happen if you ask for a notify char which is not there?
      if (!(newstate <= GEN_STATE_FAILED)){
        if (op->characteristicUUID.bitSize()) {
          // read or write characteristic - we always need this?
          NimBLERemoteCharacteristic *pCharacteristic = nullptr;

          pCharacteristic = pService->getCharacteristic(op->characteristicUUID);
          if (pCharacteristic != nullptr) {
            BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("got read/write characteristic"));
            newstate = GEN_STATE_FAILED_NOREADWRITE; // overwritten on failure

            if (op->readlen){
              if(pCharacteristic->canRead()) {
                std::string value = pCharacteristic->readValue();
                op->readlen = value.length();
                memcpy(op->dataRead, value.data(), 
                  (op->readlen > sizeof(op->dataRead))?
                    sizeof(op->dataRead): 
                    op->readlen);
                if (op->readlen > sizeof(op->dataRead)){
                  op->readtruncated = 1;
                } else {
                  op->readtruncated = 0;
                }
                if (op->readmodifywritecallback){
                  READ_CALLBACK *pFn = (READ_CALLBACK *)op->readmodifywritecallback;
                  BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("read characteristic with readmodifywritecallback"));
                  pFn(op);
                } else {
                  BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("read characteristic"));
                }

                // only change it to a 'finished' state if we really are
                if (!waitNotify && !op->writelen) newstate = GEN_STATE_READDONE;

              } else {
                newstate = GEN_STATE_FAILED_CANTREAD;
              }
            }
            if (op->writelen){
              if(pCharacteristic->canWrite()) {
                if (!pCharacteristic->writeValue(op->dataToWrite, op->writelen, true)){
                  newstate = GEN_STATE_FAILED_WRITE;
                  BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("characteristic write fail"));
                } else {
                  if (!waitNotify) newstate = GEN_STATE_WRITEDONE;
                  BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("write characteristic"));
                }
              } else {
                newstate = GEN_STATE_FAILED_CANTWRITE;
              }
            }
            // print or do whatever you need with the value
            
          } else {
            newstate = GEN_STATE_FAILED_NO_RW_CHAR;
            BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("r/w characteristic not found"));
          }
        }
      }


      // disconnect if not waiting for notify, 
      if (!op->notifytimer){
        if (waitNotify){
          vTaskDelay(50/ portTICK_PERIOD_MS);
          // must have completed during our read/write operation
          newstate = GEN_STATE_NOTIFIED;
        }
      } else {
        newstate = notifystate;
      } 
    } else {
      newstate = GEN_STATE_FAILED_NOSERVICE;
      // failed to get a service
      BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("failed - svc not on device?"));
    }            

  } else { // connect itself failed
    newstate = GEN_STATE_FAILED_CONNECT;
    int rc = (*ppClient)->getResult();

    switch (rc){
      case (0x0200+BLE_ERR_CONN_LIMIT ):
        BLE_ESP32::SafeAddLog_P(LOG_LEVEL_ERROR,PSTR("Hit connection limit? - restarting NimBLE"));
        BLERestartNimBLE = 1;
        BLERestartBLEReason = BLE_RESTART_BLE_REASON_CONN_LIMIT;
        break;
      case (0x0200+BLE_ERR_ACL_CONN_EXISTS):
        BLE_ESP32::SafeAddLog_P(LOG_LEVEL_ERROR,PSTR("Connection exists? - restarting NimBLE"));
        BLERestartNimBLE = 1;
        BLERestartBLEReason = BLE_RESTART_BLE_REASON_CONN_EXISTS;
        break;
    }
    // failed to connect
    BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("failed to connect to device %d"), rc);
  }            
  op->state = newstate;
}



// this disconnects from a device if necessary, and then
// moves the operation from 'currentOperations' to 'completedOperations'.

// for safety's sake, only call from the run task
static void BLETaskRunTaskDoneOperation(BLE_ESP32::generic_sensor_t** op, NimBLEClient **ppClient){
  try {
    if ((*ppClient)->isConnected()){
      BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("runTaskDoneOperation: disconnecting connected client"));
      (*ppClient)->disconnect();
      // wait for 1/2 second after disconnect
      int waits = 0;
      do {
        vTaskDelay(500/ portTICK_PERIOD_MS);
        if (waits) {
          //(*ppClient)->disconnect();
          // we will stall here forever!!! - as testing 
          BLE_ESP32::SafeAddLog_P(LOG_LEVEL_ERROR,PSTR("############################################# wait disconnect %d"), waits);
          vTaskDelay(500/ portTICK_PERIOD_MS);
        }
        waits++;
        if (waits == 5){
          int conn_id = (*ppClient)->getConnId();
          ble_gap_conn_broken(conn_id, -1);
          BLE_ESP32::SafeAddLog_P(LOG_LEVEL_ERROR,PSTR("################# kill connection manually wait disconnect %d"), waits);
        }
        if (waits == 60){
          BLE_ESP32::SafeAddLog_P(LOG_LEVEL_ERROR,PSTR("################# >60s waiting -> BLE Failed, restarting Tasmota %d"), waits);
          BLEStop = 1;
          BLERestartTasmota = 10;
          BLERestartTasmotaReason = BLE_RESTART_TEAMOTA_REASON_BLE_DISCONNECT_FAIL;
        }
      } while ((*ppClient)->isConnected());
    }
  } catch(const std::exception& e){
    BLE_ESP32::SafeAddLog_P(LOG_LEVEL_ERROR,PSTR("runTaskDoneOperation: exception in disconnect"));
  }


  {
    BLEAutoMutex localmutex(BLEOperationsRecursiveMutex);

    // find this operation in currentOperations, and remove it.
    for (int i = 0; i < currentOperations.size(); i++){
      if (currentOperations[i]->opid == (*op)->opid){
        currentOperations.erase(currentOperations.begin() + i);
        break;
      }
    }
  }


  // by adding it to this list, this will cause it to be sent to MQTT
  BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("runTaskDoneOperation: add to completedOperations"));
  addOperation(&completedOperations, op);
  return;
}




// this IS as task.
// we MAY be able to run a few of these simultaneously, but this is not yet tested.
// and probably not required.  But everything is there to do so....
static void BLEOperationTask(void *pvParameters){
  uint32_t timer = 0;
  // operation which is currently in progress in THIS TASK
  generic_sensor_t* currentOperation = nullptr;

  NimBLEClient *pClient = nullptr;
  BLE_ESP32::BLETaskStopStartNimBLE(&pClient);

  for(;;){
    BLELastLoopTime = esp_timer_get_time();

    BLE_ESP32::BLETaskRunCurrentOperation(&currentOperation, &pClient);

    // start a scan if possible
    if ((BLEMode == BLEModeRegularScan) || (BLETriggerScan)){
      BLEScan* lastScan = ble32Scan;
      ble32Scan = NimBLEDevice::getScan();
      if (lastScan != ble32Scan){
        ble32Scan->setInterval(70);
        ble32Scan->setWindow(50);
        ble32Scan->setAdvertisedDeviceCallbacks(&BLEScanCallbacks,true);
      }

      BLE_ESP32::BLETaskStartScan(20);
    }

    // come around every s
    vTaskDelay(1000/ portTICK_PERIOD_MS);

    if (BLEStop == 1){
      break;
    }

    if (BLERestartNimBLE){
      BLERestartNimBLE = 0;
      BLERestartTasmota = 10;
      BLERestartTasmotaReason = BLE_RESTART_TEAMOTA_REASON_RESTARTING_BLE_TIMEOUT;
      BLE_ESP32::SafeAddLog_P(LOG_LEVEL_ERROR,PSTR("BLETask: Restarting NimBLE - will restart Tasmota if not complete in 10s"));
      BLE_ESP32::BLETaskStopStartNimBLE(&pClient);
      BLERestartTasmotaReason = BLE_RESTART_TEAMOTA_REASON_UNKNOWN;
      BLERestartTasmota = 0;
    }
  }

  BLE_ESP32::BLETaskStopStartNimBLE(&pClient, false);

  // wait second
  vTaskDelay(1000/ portTICK_PERIOD_MS);

  BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("BLEOperationTask: Left task"));
  BLEStop = 2;
  BLERunning = false;
  BLERunningScan = 0;
  deleteSeenDevices();

  vTaskDelete( NULL );
}





/***********************************************************************\
 * Regular Tasmota called functions
 * 
\***********************************************************************/
static void BLEEvery50mSecond(){
  safelogdata* logdata = nullptr;
  do{
    int free = freelogs.size();
    int filled = filledlogs.size();
    logdata = GetSafeLog();
    if (logdata){
      AddLog_P(logdata->level, PSTR("SL%d-%d %s"), filled, free, logdata->log_data);
      ReleaseSafeLog(logdata);
    }
  } while (logdata);

  postAdvertismentDetails();
}



/**
 * @brief Main loop of the driver, "high level"-loop
 *
 */

static void BLEEverySecond(bool restart){

  if (Settings.flag5.mi32_enable != BLEMasterEnable){
    BLEMasterEnable = Settings.flag5.mi32_enable;

    if (BLEMasterEnable){
      StartBLE();
      AddLog_P(LOG_LEVEL_INFO,PSTR("BLE: starting because MasterEnable changed"));
    } else {
      StopBLE();
      // delete all seen devices;
      deleteSeenDevices();
      AddLog_P(LOG_LEVEL_INFO,PSTR("BLE: stopping because MasterEnable changed"));
    }
  }


  // check for application callbacks here.
  // this may remove complete items.
  BLE_ESP32::mainThreadOpCallbacks();

  // post any MQTT data if we completed anything in the last second
  if (completedOperations.size()){
    BLE_ESP32::BLEPostMQTT(true); // send only completed
  }

  // request send of ALL oeprations prepped, queued, in progress - 
  // in separate MQTT messages.
  if (BLEPostMQTTTrigger){
    BLEPostMQTTTrigger = 0;
    BLE_ESP32::BLEPostMQTT(false); // show all operations, not just completed
  }

  if (BLEPublishDevices){
    BLEPublishDevices = 0;
    if (BLEAdvertMode != BLE_ESP32::BLE_NO_ADV_SEND){
      BLEPostMQTTSeenDevices();
    }
  }

  // we have been asked to restart in this many seconds....
  if (BLERestartTasmota){
    BLERestartTasmota--;
    // 2 seconds to go, post to BLE topic on MQTT our reason
    if (BLERestartTasmota == 2){
      if (!BLERestartTasmotaReason) BLERestartTasmotaReason = BLE_RESTART_TEAMOTA_REASON_UNKNOWN;
      snprintf_P(TasmotaGlobal.mqtt_data, sizeof(TasmotaGlobal.mqtt_data), PSTR("{\"reboot\":\"%s\"}"), BLERestartTasmotaReason);
      MqttPublishPrefixTopic_P(TELE, PSTR("BLE"), Settings.flag.mqtt_sensor_retain);
      AddLog_P(LOG_LEVEL_ERROR,PSTR("BLE Failure! Restarting Tasmota in %d seconds because %s"), BLERestartTasmota, BLERestartTasmotaReason);
    }

    if (!BLERestartTasmota){
      AddLog_P(LOG_LEVEL_ERROR,PSTR("BLE Failure! Restarting Tasmota because %s"), BLERestartTasmotaReason);
      // just a normal restart
      TasmotaGlobal.restart_flag = 1;
    }
  }

  if (BLERestartBLEReason){ // just use the ptr as the trigger to send MQTT
    snprintf_P(TasmotaGlobal.mqtt_data, sizeof(TasmotaGlobal.mqtt_data), PSTR("{\"blerestart\":\"%s\"}"), BLERestartBLEReason);
    MqttPublishPrefixTopic_P(TELE, PSTR("BLE"), Settings.flag.mqtt_sensor_retain);
    AddLog_P(LOG_LEVEL_ERROR,PSTR("BLE Failure! Restarting BLE Stack because %s"), BLERestartBLEReason);
    BLERestartBLEReason = nullptr;
  }


  BLE_ESP32::mainThreadBLETimeouts();
  if (!BLEMasterEnable){
    return;
  }

}





/*********************************************************************************************\
 * Operations queue functions - all to do with read/write and notify for a device
\*********************************************************************************************/

// this retrievs the next operation from the passed list, and removes it from the list.
// or returns null if none.
generic_sensor_t* nextOperation(std::deque<generic_sensor_t*> *ops){
  generic_sensor_t* op = nullptr;
  if (ops->size()){
    BLEAutoMutex localmutex(BLEOperationsRecursiveMutex);
    op = (*ops)[0]; 
    ops->pop_front();
  }
  return op;
}

// this adds an operation to the end of passed list.
// it also sets the op pointer passed to null.
int addOperation(std::deque<generic_sensor_t*> *ops, generic_sensor_t** op){
  int res = 0;
  {
    BLEAutoMutex localmutex(BLEOperationsRecursiveMutex);
    if (ops->size() < 10){
      ops->push_back(*op);
      *op = nullptr;
      res = 1;
    }     
  }
  if (res){
    //BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("added operation"));
  } else {
    BLE_ESP32::SafeAddLog_P(LOG_LEVEL_ERROR,PSTR("added operation - no room"));
  }
  return res;
}


int newOperation(BLE_ESP32::generic_sensor_t** op){
  if (!op) {
    BLE_ESP32::SafeAddLog_P(LOG_LEVEL_ERROR,PSTR("op invalid in newOperation"));
    return 0;
  }

  (*op) = new BLE_ESP32::generic_sensor_t;
  memset((*op), 0, sizeof(BLE_ESP32::generic_sensor_t));
  return 1;
}

int freeOperation(BLE_ESP32::generic_sensor_t** op){
  if (!op) {
    return 0;
  }

  delete (*op);
  (*op) = nullptr;
  return 1;
}


int extQueueOperation(BLE_ESP32::generic_sensor_t** op){
  if (!op) {
    AddLog_P(LOG_LEVEL_ERROR,PSTR("extQueueOperation: op invalid"));
    return 0;
  }
  if (!(*op)){
    AddLog_P(LOG_LEVEL_ERROR,PSTR("extQueueOperation: *op invalid"));
    return 0;
  }
  (*op)->state = GEN_STATE_START; // trigger request later
  (*op)->opid = lastopid++;

  int res = addOperation(&queuedOperations, op);
  if (!res){
    AddLog_P(LOG_LEVEL_ERROR,PSTR("extQueueOperation: op added id %d failed"), (lastopid-1));
  }
  return res;
}




/*********************************************************************************************\
 * Highest level BLE task control functions
\*********************************************************************************************/

static void StartBLE(void) {
  BLE_ESP32::BLEStartOperationTask();
}

static void StopBLE(void){
  BLEStop = 1;
}	



/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

static void CmndBLEPeriod(void) {
  //ResponseCmndNumber(BLE.period);
  ResponseCmndDone();
}


//////////////////////////////////////////////////////////////
// Determine what to do with advertismaents
// BLEAdv0 -> suppress MQTT about devices found
// BLEAdv1 -> send MQTT about devices found after each scan
void CmndBLEAdv(void){
  switch(XdrvMailbox.index){
    case 0:
      BLEAdvertMode = BLE_ESP32::BLE_NO_ADV_SEND;
      break;
    case 1:
      BLEAdvertMode = BLE_ESP32::BLE_ADV_TELE;
      break;
    /*case 2:
      BLEAdvertMode = BLE_ADV_ALL;
      break;*/
    case 3:
      break;
  }

  ResponseCmndDone();
}


//////////////////////////////////////////////////////////////
// Scan options
// BLEScan0 -> do a scan now if BLEMode == BLEModeScanByCommand
// BLEScan0 <timesec> -> do a scan now if BLEMode == BLEModeScanByCommand for timesec seconds
// BLEScan1 0 -> Scans are passive
// BLEScan1 1 -> Scans are active
// more options could be added...
void CmndBLEScan(void){
  switch(XdrvMailbox.index){
    case 0: // do a manual scan now
      switch (BLEMode){
        case BLEModeScanByCommand: {
          int time = 20;
          if (XdrvMailbox.data_len > 0) {
            time = XdrvMailbox.payload;
            if (time < 2) time = 2;
            if (time > 40) time = 40;
          }
          BLETriggerScan = time;
          ResponseCmndNumber(time); // -ve for fail for a few reasons
        } break;
        case BLEModeDisabled:
          ResponseCmndChar("BLEDisabled");
          break;
        case BLEModeRegularScan:
          ResponseCmndChar("BLEActive");
          break;
      }
      break;
    case 1:{
      if (XdrvMailbox.data_len > 0) {
        if(XdrvMailbox.payload){
          BLEScanActiveMode = true;
          ResponseCmndChar("ActiveScan");
        } else {
          BLEScanActiveMode = false;
          ResponseCmndChar("PassiveScan");
        }
      } else {
        ResponseCmndChar("Invalid");
      }
    } break;

    default:
      ResponseCmndChar("Invalid");
      break;
  }
}


//////////////////////////////////////////////////////////////
// Determine what to do with advertismaents
// BLEMode0 -> kill BLE completely
// BLEMode1 -> start BLE, scan by command
// BLEMode2 -> start BLE, regular scan
void CmndBLEMode(void){
  switch(XdrvMailbox.index){
    case BLEModeDisabled:{
      if (BLEMode != BLEModeDisabled){
        StopBLE();
        BLEMode = BLEModeDisabled;
        ResponseCmndChar("StoppingBLE");
      } else {
        ResponseCmndChar("Disabled");
      }
    } break;
    case BLEModeScanByCommand:{
      if (BLEMode == BLEModeDisabled){
        StartBLE();
        ResponseCmndChar("StartingBLE");
      } else {
        ResponseCmndChar("BLERunning");
      }
      uint64_t now = esp_timer_get_time();
      BLEScanLastAdvertismentAt = now; // note the time of the last advertisment
      BLEMode = BLEModeScanByCommand;
    } break;
    case BLEModeRegularScan:{
      if (BLEMode == BLEModeDisabled){
        StartBLE();
        ResponseCmndChar("StartingBLE");
      } else {
        ResponseCmndChar("BLERunning");
      }
      uint64_t now = esp_timer_get_time();
      BLEScanLastAdvertismentAt = now; // note the time of the last advertisment
      BLEMode = BLEModeRegularScan;
    } break;
    default:
      ResponseCmndChar("InvalidIndex");
      break;
  }
}


//////////////////////////////////////////
// get more drtails for a single MAC address
// BLEDetails0 -> don;t send me anything
// BLEDetails1 <MAC> -> send me details for <mac> once
// BLEDetails2 <MAC> -> send me details for <mac> every advert if possible
// example: BLEDetails1 001A22092C9A
// details look like: 
// MQT: tele/tasmota_esp32/BLE = {"details":{"mac":"001A22092C9A","p":"0C0943432D52542D4D2D424C450CFF0000000000000000000000"}}
// and incliude mac, complete advert payload, plus optional ,"lost":true if an advert was not captured because MQTT we already 
// had one waiting to be sent 
void CmndBLEDetails(void){
  switch(XdrvMailbox.index){
    case 0:
      BLEDetailsRequest = 0;
      ResponseCmndDone();
      break;

    case 1:
    case 2:{
      BLEDetailsRequest = 0;
      if (12 == XdrvMailbox.data_len) { // MAC-string without colons
        fromHex(BLEDetailsMac, XdrvMailbox.data, 6);
        BLEDetailsRequest = XdrvMailbox.index;
      }
      ResponseCmndDone();
    } break;

    case 3:{
      BLEDetailsRequest = XdrvMailbox.index;
      ResponseCmndDone();
    } break;

    default:
      ResponseCmndChar("InvalidIndex");
      break;
  }
}

//////////////////////////////////////////////////////////////////////////
// Command to cause BLE read/write/notify operations to be run.
//////////////////////////////////////////////////////////////////////////

// we expect BLEOp0 - poll state
// we expect BLEOp1 m:MAC s:svc <c:characteristic> <n:notifychar> <w:hextowrite> <r> <go>
// we expect BLEOp2 trigger queue of op.  return is opid

// returns: Done|FailCreate|FailNoOp|FailQueue|InvalidIndex|<opid>

// BLEop0/1/2 will cause an MQTT send of ops currently known.
// on op complete/op fail, a MQTT send is triggered of all known ops, and the completed/failed op removed.

// example: 
// BLEOp1 M:001A22092CDB s:3e135142-654f-9090-134a-a6ff5bb77046 c:3fa4585a-ce4a-3bad-db4b-b8df8179ea09 w:03 n:d0e8434d-cd29-0996-af41-6c90f4e0eb2a go
// requests write of 03, and request wait for notify.

// You may queue up operations.  they are currently processed serially.
void CmndBLEOperation(void){

  int op = XdrvMailbox.index;

  //AddLog_P(LOG_LEVEL_INFO,PSTR("op %d"), op);

  int res = -1;

  // if in progress, only op 0 or 11 are allowed
  switch(op) {
    case 0:
      AddLog_P(LOG_LEVEL_INFO,PSTR("preview"));
      BLEPostMQTTTrigger = 1;
      break;
    case 1: {
      if (prepOperation){
        BLE_ESP32::freeOperation(&prepOperation);
      }
      int opres = BLE_ESP32::newOperation(&prepOperation);
      if (!opres){
        AddLog_P(LOG_LEVEL_ERROR,PSTR("Could not create new operation"));
        ResponseCmndChar("FailCreate");
        return;
      }
      // expect m:MAC s:svc <c:characteristic> <n:notifychar> <w:hextowrite> <r> <go>
      // < > are optional
      char *p = strtok(XdrvMailbox.data, " ,");
      bool trigger = false;

      while (p){
        switch(*p | 0x20){
          case 'm':{
            uint8_t addr[6];
            fromHex(addr, p+2, sizeof(addr));
            //ReverseMAC(addr);
            prepOperation->addr = NimBLEAddress(addr);
          } break;
          case 's':{
            prepOperation->serviceUUID = NimBLEUUID(p+2);
          } break;
          case 'c':
            prepOperation->characteristicUUID = NimBLEUUID(p+2);
            //strncpy(prepOperation->characteristicStr, p+2, sizeof(prepOperation->characteristicStr)-1);
            break;
          case 'n':
            prepOperation->notificationCharacteristicUUID = NimBLEUUID(p+2);
            //strncpy(prepOperation->notificationCharacteristicStr, p+2, sizeof(prepOperation->notificationCharacteristicStr)-1);
            break;
          case 'w':
            prepOperation->writelen = fromHex(prepOperation->dataToWrite, p+2, sizeof(prepOperation->dataToWrite));
            break;
          case 'u': // 'unique' context for this request
            prepOperation->context = (void *)atoi(p+2);
            break;
          case 'r':
            prepOperation->readlen = 1;
            break;
          case 'g':
            if ((*(p+1))|0x20 == 'o'){
              trigger = true;
            }
            break;
        }

        p = strtok(nullptr, " ,");
      }

      if (trigger){
        int u = (int)prepOperation->context;
        int opres = BLE_ESP32::extQueueOperation(&prepOperation);
        if (!opres){
          // NOTE: prepOperation will NOT have been deleted.  
          // this means you could retry with another BLEOp10.
          // it WOULD be deleted if you sent another BELOP1 <MAC>
          AddLog_P(LOG_LEVEL_ERROR,PSTR("Could not queue new operation"));
          ResponseCmndChar("FailQueue");
          return;
        } else {
          // NOTE: prepOperation has been set to null if we queued sucessfully.
          AddLog_P(LOG_LEVEL_INFO,PSTR("Operations queued:%d"), queuedOperations.size());
          char temp[40];
          sprintf(temp, "{\"opid\":%d,\"u\":%d}", lastopid-1, u);
          Response_P(S_JSON_COMMAND_XVALUE, XdrvMailbox.command, temp);
          // don't do this here... overwrites response
          //BLE_ESP32::BLEPostMQTT(false);
          return;
        }
      } else {
        ResponseCmndChar("Prepared");
        //BLE_ESP32::BLEPostMQTT(false);
        return;
      }
    } break;

    case 2: {
      if (!prepOperation) {
        ResponseCmndChar("FailNoOp");
        return;      
      }
      //prepOperation->requestType = atoi(XdrvMailbox.data);
      int u = (int)prepOperation->context;
      int opres = BLE_ESP32::extQueueOperation(&prepOperation);
      if (!opres){
        // NOTE: prepOperation will NOT have been deleted.  
        // this means you could retry with another BLEOp10.
        // it WOULD be deleted if you sent another BELOP1 <MAC>
        AddLog_P(LOG_LEVEL_ERROR,PSTR("Could not queue new operation"));
        ResponseCmndChar("FailQueue");
      } else {
        // NOTE: prepOperation has been set to null if we queued sucessfully.
        AddLog_P(LOG_LEVEL_INFO,PSTR("Operations queued:%d"), queuedOperations.size());
        char temp[40];
        sprintf(temp, "{\"opid\":%d,\"u\":%d}", lastopid-1, u);
        Response_P(S_JSON_COMMAND_XVALUE, XdrvMailbox.command, temp);
      }
      return;
    } break;

    default:
      ResponseCmndChar("InvalidIndex");
      return;
  }

  ResponseCmndDone();
  return;
}


/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/
static void BLEPostMQTTSeenDevices() {
  int remains = 0;
  do {
    nextSeenDev = 0;
    remains = getSeenDevicesToJson(TasmotaGlobal.mqtt_data, sizeof(TasmotaGlobal.mqtt_data));
    // no retain - this is present devices, not historic
    MqttPublishPrefixTopic_P(TELE, PSTR("BLE"), 0);
  } while (remains);
}

static void BLEPostMQTT(bool onlycompleted) {
  if (prepOperation || completedOperations.size() || queuedOperations.size() || currentOperations.size()){
    AddLog_P(LOG_LEVEL_INFO,PSTR("some to show"));

    if (prepOperation && !onlycompleted){
      std::string out = BLETriggerResponse(prepOperation);
      snprintf_P(TasmotaGlobal.mqtt_data, sizeof(TasmotaGlobal.mqtt_data), PSTR("%s"), out.c_str());
      MqttPublishPrefixTopic_P(TELE, PSTR("BLE"), Settings.flag.mqtt_sensor_retain);
      AddLog_P(LOG_LEVEL_INFO,PSTR("prep sent %s"), out.c_str());
    }

    if (queuedOperations.size() && !onlycompleted){
      AddLog_P(LOG_LEVEL_INFO,PSTR("queued %d"), queuedOperations.size());
      for (int i = 0; i < queuedOperations.size(); i++){
        BLEAutoMutex localmutex(BLEOperationsRecursiveMutex);

        generic_sensor_t *toSend = queuedOperations[i];
        if (!toSend) {
          break;
        } else {
          std::string out = BLETriggerResponse(toSend);
          localmutex.give();
          snprintf_P(TasmotaGlobal.mqtt_data, sizeof(TasmotaGlobal.mqtt_data), PSTR("%s"), out.c_str());
          MqttPublishPrefixTopic_P(TELE, PSTR("BLE"), Settings.flag.mqtt_sensor_retain);
          AddLog_P(LOG_LEVEL_INFO,PSTR("queued %d sent %s"), i, out.c_str());
          //break;
        }
      }
    }

    if (currentOperations.size() && !onlycompleted){
      AddLog_P(LOG_LEVEL_INFO,PSTR("current %d"), currentOperations.size());
      for (int i = 0; i < currentOperations.size(); i++){
        BLEAutoMutex localmutex(BLEOperationsRecursiveMutex);
        generic_sensor_t *toSend = currentOperations[i];
        if (!toSend) {
          break;
        } else {
          std::string out = BLETriggerResponse(toSend);
          localmutex.give();
          snprintf_P(TasmotaGlobal.mqtt_data, sizeof(TasmotaGlobal.mqtt_data), PSTR("%s"), out.c_str());
          MqttPublishPrefixTopic_P(TELE, PSTR("BLE"), Settings.flag.mqtt_sensor_retain);
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
          break; // break from while loop
        } else {
          AddLog_P(LOG_LEVEL_DEBUG,PSTR("BLE:completedOperation removed"));
          std::string out = BLETriggerResponse(toSend);
          snprintf_P(TasmotaGlobal.mqtt_data, sizeof(TasmotaGlobal.mqtt_data), PSTR("%s"), out.c_str());
          MqttPublishPrefixTopic_P(TELE, PSTR("BLE"), Settings.flag.mqtt_sensor_retain);
          // we alreayd removed this from the queues, so now delete
          delete toSend; 
          //break;
        }
        //break;
      } while (1);
    }
  } else {
    snprintf_P(TasmotaGlobal.mqtt_data, sizeof(TasmotaGlobal.mqtt_data), PSTR("{\"BLEOperation\":{}}"));
    MqttPublishPrefixTopic_P(TELE, PSTR("BLE"), Settings.flag.mqtt_sensor_retain);
  }
}

static void mainThreadBLETimeouts() {
  uint64_t now = esp_timer_get_time();

  if (!BLERunning){
    BLELastLoopTime = now; // initialise the time of the last advertisment
    BLEScanLastAdvertismentAt = now; // initialise the time of the last advertisment
  }

  // if no adverts for 120s, and BLE is running, retsart NimBLE.
  // belt and braces....
  uint64_t adTimeout = ((uint64_t)BLEMaxTimeBetweenAdverts)*1000L*1000L;
  if (BLEScanLastAdvertismentAt + adTimeout < now){
    BLEScanLastAdvertismentAt = now; // initialise the time of the last advertisment
    BLERestartNimBLE = 1;
    AddLog_P(LOG_LEVEL_DEBUG,PSTR("BLE: BLEScan stalled? no adverts > 120s, restarting BLE"));
    BLERestartBLEReason = BLE_RESTART_BLE_REASON_ADVERT_BLE_TIMEOUT;
  }

  // if stuck and have not done task for 120s, something is seriously wrong.
  // restart Tasmota completely. (belt and braces)
  uint64_t bleLoopTimeout = ((uint64_t)BLEMaxTaskLoopTime)*1000L*1000L;
  if (BLELastLoopTime + bleLoopTimeout < now){
    BLELastLoopTime = now; // initialise the time of the last advertisment
    BLERestartTasmota = 10;
    AddLog_P(LOG_LEVEL_DEBUG,PSTR("BLE: BLETask stalled > 120s, restarting Tasmota in 10s"));
    BLERestartTasmotaReason = BLE_RESTART_TEAMOTA_REASON_BLE_LOOP_STALLED;
  }
}


static void mainThreadOpCallbacks() {
  if (completedOperations.size()){
    //AddLog_P(LOG_LEVEL_INFO,PSTR("completed %d"), completedOperations.size());
    BLEAutoMutex localmutex(BLEOperationsRecursiveMutex);

    // find this operation in currentOperations, and remove it.
    // in reverse so we can erase them safely.
    for (int i = completedOperations.size()-1; i >= 0 ; i--){
      generic_sensor_t *op = completedOperations[i];

      bool callbackres = false;

      if (op->completecallback){
        try {
          OPCOMPLETE_CALLBACK *pFn = (OPCOMPLETE_CALLBACK *)(op->completecallback);
          callbackres = pFn(op);
          AddLog_P(LOG_LEVEL_DEBUG,PSTR("op->completecallback %d"), callbackres);
        } catch(const std::exception& e){
          AddLog_P(LOG_LEVEL_ERROR,PSTR("exception in op->completecallback"));
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
  }
}


static void BLEShow(bool json)
{
  if (json){
    AddLog_P(LOG_LEVEL_INFO,PSTR("show json %d"),json);
    uint32_t totalCount = BLEAdvertisment.totalCount;
    uint32_t deviceCount = seenDevices.size();

    ResponseAppend_P(PSTR(",\"BLE\":{\"scans\":%u,\"advertisments\":%u,\"devices\":%u,\"resets\":%u}"), BLEScanCount, totalCount, deviceCount, BLEResets);
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
  if (!toSend) return "";
  ResponseClear();
  std::string out = "{\"BLEOperation\":{\"opid\":\"";
  sprintf(temp, "%d", toSend->opid); // note only 10 long!
  out = out + temp;
/*  out = out + "\",\"state\":\"";
  sprintf(t, "%d", toSend->state);
  out = out + t;*/
  out = out + "\",\"stat\":\"";
  sprintf(temp, "%d", toSend->state);
  out = out + temp;
  out = out + "\",\"state\":\"";
  out = out + getStateString(toSend->state);
  
  if (toSend->addr != NimBLEAddress()){
    out = out + "\",\"MAC\":\"";
    uint8_t addrrev[6];
    memcpy(addrrev, toSend->addr.getNative(), 6);
    ReverseMAC(addrrev);
    dump(temp, 13, addrrev, 6);
    out = out + temp;
  }
  if (toSend->context){
    out = out + "\",\"u\":\"";
    sprintf(temp, "%d", (int32_t)toSend->context);
    out = out + temp;
  }
  if (toSend->serviceUUID.bitSize()){
    out = out + "\",\"svc\":\"";
    out = out + toSend->serviceUUID.toString();
  }
  if (toSend->characteristicUUID.bitSize()){
    out = out + "\",\"char\":\"";
    out = out + toSend->characteristicUUID.toString();
  }
  if (toSend->notificationCharacteristicUUID.bitSize()){
    out = out + "\",\"notifychar\":\"";
    out = out + toSend->notificationCharacteristicUUID.toString();
  }
  out = out + "\"";
  if (toSend->readlen){
    dump(temp, 99, toSend->dataRead, toSend->readlen);
    if (toSend->readtruncated){
      strcat(temp, "+");
    }
    out = out + ",\"read\":\"";
    out = out + temp;
    out = out + "\"";
  }
  if (toSend->writelen){
    dump(temp, 99, toSend->dataToWrite, toSend->writelen);
    out = out + ",\"write\":\"";
    out = out + temp;
    out = out + "\"";
  }
  if (toSend->notifylen){
    dump(temp, 99, toSend->dataNotify, toSend->notifylen);
    if (toSend->notifytruncated){
      strcat(temp, "+");
    }
    out = out + ",\"notify\":\"";
    out = out + temp;
    out = out + "\"";
  }
  out = out + "}}";
  return out;
}

#ifdef USE_WEBSERVER

#define WEB_HANDLE_BLE "ble"
#define D_CONFIGURE_BLE "Configure BLE"
#define D_BLE_PARAMETERS "Bluetooth Settings"
#define D_MQTT_BLE_ENABLE "Enable Bluetooth"
#define D_MQTT_BLE_ACTIVESCAN "Enable Active Scan(*)"
#define D_BLE_DEVICES "Devices Seen"

const char HTTP_BTN_MENU_BLE[] PROGMEM =
  "<p><form action='" WEB_HANDLE_BLE "' method='get'><button>" D_CONFIGURE_BLE "</button></form></p>";

const char HTTP_FORM_BLE[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_BLE_PARAMETERS "&nbsp;</b></legend>"
  "<form method='get' action='" WEB_HANDLE_BLE "'>"
  "<p><label><input id='e0' type='checkbox'%s><b>" D_MQTT_BLE_ENABLE "</b></label></p>"
  "<p><label><input id='e1' type='checkbox'%s><b>" D_MQTT_BLE_ACTIVESCAN "</b></label></p>"
  "<p>items marked (*) are not stored in config</p>";


const char HTTP_BLE_DEV_STYLE[] PROGMEM = "th, td { padding-left:5px; }";
const char HTTP_BLE_DEV_START[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_BLE_DEVICES "&nbsp;</b></legend><table>"
  "<tr><th><label>mac</label></th><th><label>name</label></th><th><label>RSSI</label></th></tr>";
const char HTTP_BLE_DEV[] PROGMEM =
  "<tr><td><label>%s</label></td><td><label>%s</label></td><td><label>%d</label></td></tr>";
const char HTTP_BLE_DEV_END[] PROGMEM =
  "</table></fieldset>";

void HandleBleConfiguration(void)
{
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("HandleBleConfiguration"));


  if (!HttpCheckPriviledgedAccess()) { 
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("!HttpCheckPriviledgedAccess()"));
    return; 
  }

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_CONFIGURE_BLE));


  char tmp[20];
  WebGetArg("en", tmp, sizeof(tmp));

  AddLog_P(LOG_LEVEL_DEBUG, PSTR("arg en is %s"), tmp);

  if (Webserver->hasArg("save")) {
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("BLE SETTINGS SAVE"));
    Settings.flag5.mi32_enable = Webserver->hasArg("e0");  // 
    BLEScanActiveMode = Webserver->hasArg("e1");  // 

    SettingsSaveAll();
    HandleConfiguration();
    return;
  }
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("!SAVE"));

  char str[TOPSZ];

  WSContentStart_P(PSTR(D_CONFIGURE_BLE));
  WSContentSendStyle_P(HTTP_BLE_DEV_STYLE);
  //WSContentSendStyle();
  WSContentSend_P(HTTP_FORM_BLE, 
    (Settings.flag5.mi32_enable) ? " checked" : "",
    (BLEScanActiveMode) ? " checked" : ""
    );
  WSContentSend_P(HTTP_FORM_END);


  {
    BLEAutoMutex localmutex(BLEOperationsRecursiveMutex);
    if (seenDevices.size()){
      WSContentSend_P(HTTP_BLE_DEV_START);

      for (int i = 0; i < seenDevices.size(); i++){
        BLE_ESP32::BLE_simple_device_t* dev = seenDevices[i];
        char addr[20];
        dump(addr, 20, dev->mac, 6);
        WSContentSend_P(HTTP_BLE_DEV, addr, dev->name, dev->RSSI);
      }
      WSContentSend_P(HTTP_BLE_DEV_END);
    }
  }

  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();

}
#endif


} // end namespace BLE_ESP32

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv47(uint8_t function)
{
  //if (!Settings.flag5.mi32_enable) { return false; }  // SetOption115 - Enable ESP32 BLE BLE

  bool result = false;

  if (FUNC_INIT == function){
    BLE_ESP32::BLEPreInit();
  }

  if (!BLE_ESP32::BLEInitState) {
    if (function == FUNC_EVERY_250_MSECOND) {
      BLE_ESP32::BLEInit();
    }
    return result;
  }
  switch (function) {
    case FUNC_EVERY_50_MSECOND:
      BLE_ESP32::BLEEvery50mSecond();
      break;
    case FUNC_EVERY_SECOND:
      BLE_ESP32::BLEEverySecond(false);
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(BLE_ESP32::kBLE_Commands, BLE_ESP32::BLE_Commands);
      break;
    case FUNC_JSON_APPEND:
      BLE_ESP32::BLEShow(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_ADD_BUTTON:
      WSContentSend_P(BLE_ESP32::HTTP_BTN_MENU_BLE);
      break;
    case FUNC_WEB_ADD_HANDLER:
      WebServer_on(PSTR("/" WEB_HANDLE_BLE), BLE_ESP32::HandleBleConfiguration);
      break;

    case FUNC_WEB_SENSOR:
      BLE_ESP32::BLEShow(0);
      break;
#endif  // USE_WEBSERVER
    }
  return result;
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
int myAdvertCallback(BLE_ESP32::ble_advertisment_t *pStruct) {

  // indicate others can also hear this
  // to say 'I want this exclusively', return true.
  return 0;

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
int myOpCallback(BLE_ESP32::generic_sensor_t *pStruct){
  BLE_ESP32::SafeAddLog_P(LOG_LEVEL_INFO,PSTR("myOpCallback"));
  return 0; // return true to block MQTT broadcast
}

// this one is used to demonstrate processing of ONE specific operation
int myOpCallback2(BLE_ESP32::generic_sensor_t *pStruct){
  BLE_ESP32::SafeAddLog_P(LOG_LEVEL_INFO,PSTR("myOpCallback2"));
  return 1; // return true to block MQTT broadcast
}
#endif
/*********************************************************************************************\
 * End of Example Operations callbacks
\*********************************************************************************************/

void installExamples(){
#ifdef EXAMPLE_ADVERTISMENT_CALLBACK
  BLE_ESP32::registerForAdvertismentCallbacks((const char *)"test myOpCallback", &myAdvertCallback);
#endif

#ifdef EXAMPLE_OPERATION_CALLBACK
  BLE_ESP32:registerForOpCallbacks((const char *)"test myOpCallback", &myOpCallback);
#endif
}

void sendExample(){
#ifdef EXAMPLE_OPERATION_CALLBACK
  BLE_ESP32::generic_sensor_t *op = nullptr;
  int res = BLE_ESP32::newOperation(&op);
  if (!res){
    AddLog_P(LOG_LEVEL_ERROR,PSTR("Could not create new operation"));
    return;
  }
  strncpy(op->MAC, "001A22092EE0", sizeof(op->MAC));
  strncpy(op->serviceStr, "3e135142-654f-9090-134a-a6ff5bb77046", sizeof(op->serviceStr));
  strncpy(op->characteristicStr, "3fa4585a-ce4a-3bad-db4b-b8df8179ea09", sizeof(op->characteristicStr));
  strncpy(op->notificationCharacteristicStr, "d0e8434d-cd29-0996-af41-6c90f4e0eb2a", sizeof(op->notificationCharacteristicStr));
  op->writelen = BLE_ESP32::fromHex(op->dataToWrite, (char *)"4040", sizeof(op->dataToWrite));

  // this op will call us back on complete or failure.
  op->completecallback = (void *)myOpCallback2;
  res = BLE_ESP32::extQueueOperation(&op);
  if (!res){
    // if it fails to add to the queue, do please delete it
    BLE_ESP32::freeOperation(&op);
    AddLog_P(LOG_LEVEL_ERROR,PSTR("Failed to queue new operation - deleted"));
    return;
  }

#endif
}



#endif  
#endif  // ESP32


