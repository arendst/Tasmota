/*
  xdrv_79_esp32_ble.ino - BLE via ESP32 support for Tasmota

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
  -       20210402  additions expand MAC storage to 7, enables type of mac.
                              Add bledetails4
  --------------------------------------------------------------------------------------------
*/

// TEMPORARILY define ESP32 and USE_BLE_ESP32 so VSCODE shows highlighting....
//#define VSCODE_DEV
#ifdef VSCODE_DEV
#define ESP32
#define USE_BLE_ESP32
#endif

#ifdef ESP32                       // ESP32 family only. Use define USE_HM10 for ESP8266 support
#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C6 || CONFIG_IDF_TARGET_ESP32S3
#ifdef USE_BLE_ESP32

/*
  xdrv_79:
  This driver uses the ESP32 BLE functionality to hopefully provide enough
  BLE functionality to implement specific drivers on top of it.

  As a generic driver, it can:
    Be asked to
      connect/write to a MAC/Service/Characteristic
      connect/read from a MAC/Service/Characteristic
      connect/write/awaitnotify from a MAC/Service/Characteristic/NotifyCharacteristic
      connect/read/awaitnotify from a MAC/Service/Characteristic/NotifyCharacteristic

    Notes on MAC addresses:
      BLE MAC addresses come in different 'flavours'
      Anywhere where you enter a MNAC addr, you can now use these forms:
        AABBCCDDEEFF
        AABBCCDDEEFF/n (n = 0..3) where n indicates the TYPE of address
        most notably, if you wish to connect to a random address (n = 1), then you must specify,
        else it will not connect.
        e.g. to alias a random address to fred:
        BLEAlias fred=1234567890/1
        to connect and read fred's name:
        BLEName fred
        BLEName 1234567890/1 - would succeed (if freds name can be read)
        BLEName 1234567890 - would fail
      technical changes: all MAC storage has been increased to 7 bytes, with the last byte being the type...

    Cmnds:
      BLEPeriod
        set the period for BLE tele
      BLEAdv - unused
      BLEOp
        advanced - perform a BLE active operation
        mainly for debug or unsupported devices.
      BLEMode
        control scanning mode
        0 - stop BLE
        1 - run BLE but manual scan
        *2 - run BLE with auto scan
      BLEDetails
        display details of adverts
        BLEdetails0 - no display
        BLEdetails2 <mac|alias> - display for one device
        BLEdetails3 - display for ALL devices
        BLEdetails4 - display for all aliased devices
      BLEScan
        performs a manual scan or set passive/active
        *BLEScan0 0 - set passive scan
        BLEScan0 1 - set active scan (you may get names)
        BLEScan1 nn - start a manula scan for nn seconds
      BLEAlias
        <mac>=<name> <mac>=<name> - set one or more aliases for addresses
      BLEName
        read/write the name from a device using active read of a std characteristic
        BLEName <mac|alias> - read the name
        BLEName <mac|alias> <name> - write the name - few devices support this
      BLEDebug
        enable more debug
        BLEDebug0 - turn off
        BLEDebug|BLEDebug1 - turn on
      BLEDevices
        display or clear the devices list
        BLEDevices0 - clear list
        BLEDevices1 - publish on tele
      BLEMaxAge
        display or set the max age of a BLE address before being forgotten
        BLEMaxAge - display the setting
        BLEMaxAge nn - set to nn seconds
      BLEAddrFilter
        *0/1/2/3 - the maximum 'type' of BLE address recevied
      BLEEnableUnsaved
        *0/1 - if BLE is disabled, this can be used to enable BLE without
        it being saved - useful as the last command in autoexec.bat

  Other drivers can add callbacks to receive advertisements
  Other drivers can add 'operations' to be performed and receive callbacks from the operation's success or failure

Example BLEOp:
Write and request next notify:
BLEOp M:4C65A8DAF43A s:00001530-1212-efde-1523-785feabcd123 n:00001531-1212-efde-1523-785feabcd123 c:00001531-1212-efde-1523-785feabcd123 w:00 go
12:45:12 MQT: tele/tasmota_esp32/BLE = {"BLEOperation":{"opid":"11","stat":"7","state":"DONENOTIFIED","MAC":"4C65A8DAF43A","svc":"00001530-1212-efde-1523-785feabcd123","char":"00001531-1212-efde-1523-785feabcd123","notifychar":"00001531-1212-efde-1523-785feabcd123","write":"00","notify":"100003"}}


The driver can also be used by other drivers, using the functions:

void registerForAdvertismentCallbacks(char *loggingtag, ADVERTISMENT_CALLBACK* pFn);
void registerForOpCallbacks(char *loggingtag, OPCOMPLETE_CALLBACK* pFn);
bool extQueueOperation(generic_sensor_t** op);

These allow other code to
  receive advertisements
  receive operation callbacks.
  create and start an operation, and get a callback when done/failed.

i.e. the Bluetooth of the ESP can be shared without conflict.

*/

#define BLE_ESP32_ALIASES

// uncomment for more diagnostic/information messages - + more flash use.
//#define BLE_ESP32_DEBUG

#define XDRV_79                    79

#include <vector>
#include <deque>
#include <string.h>
#include <cstdarg>

#include <NimBLEDevice.h>
#include <NimBLEAdvertisedDevice.h>
#include "NimBLEEddystoneURL.h"
#include "NimBLEEddystoneTLM.h"
#include "NimBLEBeacon.h"

// assume this hack is still valid.
#define DEPENDSONNIMBLEARDUINO 1
#ifdef DEPENDSONNIMBLEARDUINO        
// from ble_gap.c
extern "C" void ble_gap_conn_broken(uint16_t conn_handle, int reason);
#endif

#ifdef BLE_ESP32_EXAMPLES
void installExamples();
void sendExample();
#endif


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

#define BLE_ESP32_MAXNAMELEN 32
#define BLE_ESP32_MAXALIASLEN 32


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
// structure for callbacks from other drivers from advertisements.
struct ble_advertisment_t {
  BLEAdvertisedDevice *advertisedDevice; // the full NimBLE advertisment, in case people need MORE info.
  uint32_t totalCount;

  uint8_t addr[6];
  uint8_t addrtype;
  int8_t RSSI;
  char name[BLE_ESP32_MAXNAMELEN+1];
};

struct ble_alias_t {
  uint8_t addr[7];
  char name[BLE_ESP32_MAXALIASLEN+1];
};

/*
This is probabyl what you are looking for:
ble_gap_addr_t gap_addr;
gap_addr.addr_type = BLE_GAP_ADDR_TYPE_PUBLIC; //Public address 0x00
gap_addr.addr_type = BLE_GAP_ADDR_TYPE_RANDOM_STATIC; //Random static address 0x01
gap_addr.addr_type = BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE; //Random private resolvable address 0x02
gap_addr.addr_type = BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_NON_RESOLVABLE; //Random private non-resolvable address 0x03
*/

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

#define USE_NATIVE_LOGGING


// a temporay safe logging mechanism.  This has a max of 40 chars, and a max of 15 slots per 50ms
//int SafeAddLog_P(uint32_t loglevel, PGM_P formatP, ...);

static void BLEDiag();
const char *getAlias(const uint8_t *addr);
//void BLEAliasMqttList();
void BLEAliasListResp();
////////////////////////////////////////////////////////////////////////
// utilities
// dump a binary to hex
char * dump(char *dest, int maxchars, const uint8_t *src, int len);




struct BLE_simple_device_t {
  uint8_t mac[6];
  uint8_t addrtype;
  char name[BLE_ESP32_MAXNAMELEN+1];
  int8_t RSSI;
  uint64_t lastseen;  // last seen us
  uint16_t maxAge;    // maximum observed age of this device
};



// this protects our queues, which can be accessed by multiple tasks
SemaphoreHandle_t  BLEOperationsRecursiveMutex;
SemaphoreHandle_t  BLEDevicesMutex;


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
static void BLEPostMQTTSeenDevices(int type);

static void BLEShowStats();
static void BLEPostMQTT(bool json);
static void BLEStartOperationTask();

// these are only run from the run task
static void BLETaskRunCurrentOperation(BLE_ESP32::generic_sensor_t** pCurrentOperation, NimBLEClient **ppClient);
static void BLETaskRunTaskDoneOperation(BLE_ESP32::generic_sensor_t** op, NimBLEClient **ppClient);
int BLETaskStartScan(int time);


// these are run from main thread
static int StartBLE(void);
static int StopBLE(void);

// called from advert callback
void setDetails(ble_advertisment_t *ad);

#undef EXAMPLE_ADVERTISMENT_CALLBACK
#undef EXAMPLE_OPERATION_CALLBACK

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
static uint8_t BLEEnableUnsaved = 0;
static uint8_t BLEEnableMask = 1;


static int BLEInitState = 0;
static int BLERunningScan = 0;
static uint32_t BLEScanCount = 0;
static uint8_t BLEScanActiveMode = 0;
static uint32_t BLELoopCount = 0;
static uint32_t BLEOpCount = 0;

static int BLEPublishDevices = 0; // causes MQTT publish of device list (each scan end)
static BLEScan* ble32Scan = nullptr;
bool BLERunning = false;
// time we last started a scan in uS using esp_timer_get_time();
// used to setect a scan which did not call back?
uint64_t BLEScanStartedAt = 0;
uint64_t BLEScanToEndBefore = 0;
uint8_t BLEStopScan = 0;
uint8_t BLEOtaStallBLE = 0;
uint8_t BLEDebugMode = 0;
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
uint8_t BLEAliasListTrigger = 0;
// triggers send for ALL operations known about
uint8_t BLEPostMQTTTrigger = 0;
int BLEMaxAge = 60*10; // 10 minutes
int BLEAddressFilter = 0;


//////////////////////////////////////////////////


// operation being prepared through commands
BLE_ESP32::generic_sensor_t* prepOperation = nullptr;

// operations which have been queued
std::deque<BLE_ESP32::generic_sensor_t*> queuedOperations;
// operations in progress (at the moment, only one)
std::deque<BLE_ESP32::generic_sensor_t*> currentOperations;
// operations which have completed or failed
std::deque<BLE_ESP32::generic_sensor_t*> completedOperations;
// operations which are ready to send to MQTT
std::deque<BLE_ESP32::generic_sensor_t*> mqttOperations;

// seen devices
#define MAX_BLE_DEVICES_LOGGED 80
std::deque<BLE_ESP32::BLE_simple_device_t*> seenDevices;
std::deque<BLE_ESP32::BLE_simple_device_t*> freeDevices;



// list of registered callbacks for advertisements
// register using void registerForAdvertismentCallbacks(const char *somename ADVERTISMENT_CALLBACK* pFN);
std::deque<BLE_ESP32::ADVERTISMENT_CALLBACK*> advertismentCallbacks;

std::deque<BLE_ESP32::OPCOMPLETE_CALLBACK*> operationsCallbacks;

std::deque<BLE_ESP32::SCANCOMPLETE_CALLBACK*> scancompleteCallbacks;


#ifdef BLE_ESP32_ALIASES
std::deque<BLE_ESP32::ble_alias_t*> aliases;
#endif


/*********************************************************************************************\
 * constants
\*********************************************************************************************/

#define D_CMND_BLE "BLE"

const char kBLE_Commands[] PROGMEM = D_CMND_BLE "|"
  "Period|Adv|Op|Mode|Details|Scan|Alias|Name|Debug|Devices|MaxAge|AddrFilter|EnableUnsaved";

static void CmndBLEPeriod(void);
static void CmndBLEAdv(void);
static void CmndBLEOperation(void);
static void CmndBLEMode(void);
static void CmndBLEDetails(void);
static void CmndBLEScan(void);
static void CmndBLEAlias(void);
static void CmndBLEName(void);
static void CmndBLEDebug(void);
static void CmndBLEDevices(void);
static void CmndBLEMaxAge(void);
static void CmndBLEAddrFilter(void);
static void CmndBLEEnableUnsaved(void);

void (*const BLE_Commands[])(void) PROGMEM = {
  &BLE_ESP32::CmndBLEPeriod,
  &BLE_ESP32::CmndBLEAdv,
  &BLE_ESP32::CmndBLEOperation,
  &BLE_ESP32::CmndBLEMode,
  &BLE_ESP32::CmndBLEDetails,
  &BLE_ESP32::CmndBLEScan,
  &BLE_ESP32::CmndBLEAlias,
  &BLE_ESP32::CmndBLEName,
  &BLE_ESP32::CmndBLEDebug,
  &BLE_ESP32::CmndBLEDevices,
  &BLE_ESP32::CmndBLEMaxAge,
  &BLE_ESP32::CmndBLEAddrFilter,
  &BLE_ESP32::CmndBLEEnableUnsaved
};

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
uint64_t BLEStopAt = 0;

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

int addSeenDevice(const uint8_t *mac, uint8_t addrtype, const char *name, int8_t RSSI){
  int res = 0;
  uint64_t now = esp_timer_get_time();
  TasAutoMutex localmutex(&BLEDevicesMutex, "BLEAdd");

  int devicefound = 0;
  // do we already know this device?
  for (int i = 0; i < seenDevices.size(); i++){
    if (!memcmp(seenDevices[i]->mac, mac, 6)){
      seenDevices[i]->lastseen = now;
      seenDevices[i]->addrtype = addrtype;
      seenDevices[i]->RSSI = RSSI;
      if ((!seenDevices[i]->name[0]) && name[0]){
        strncpy(seenDevices[i]->name, name, sizeof(seenDevices[i]->name));
        seenDevices[i]->name[sizeof(seenDevices[i]->name)-1] = 0;
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
#ifdef BLE_ESP32_DEBUG
        if (BLEDebugMode > 0) AddLog(LOG_LEVEL_INFO,PSTR("BLE: New seendev slot %d"), total);
#endif
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
      dev->name[sizeof(dev->name)-1] = 0;
      dev->lastseen = now;
      dev->addrtype = addrtype;
      dev->RSSI = RSSI;
      dev->maxAge = 1;
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
    TasAutoMutex localmutex(&BLEDevicesMutex, "BLEDel");

    for (int i = seenDevices.size()-1; i >= 0; i--){
        BLE_ESP32::BLE_simple_device_t* dev = seenDevices[i];
        uint64_t lastseen = dev->lastseen/1000L;
        lastseen = lastseen/1000L;
        uint32_t lastseenS = (uint32_t) lastseen;
        uint32_t del_at = lastseenS + ageS;
        uint32_t devAge = nowS - lastseenS;
        if (dev->maxAge < devAge){
          dev->maxAge = devAge;
        }

        uint8_t filter = 0;
        if (dev->addrtype > BLEAddressFilter){
          filter = 1;
        }

        if ((del_at < nowS) || (ageS == 0) || filter){
#ifdef BLE_ESP32_DEBUG
          char addr[20];
          dump(addr, 20, dev->mac, 6);
          const char *alias = getAlias(dev->mac);
          if (!filter){
            AddLog(LOG_LEVEL_INFO,PSTR("BLE: Delete device %s(%s) by age lastseen %u + maxage %u < now %u."),
              addr, alias, lastseenS, ageS, nowS);
          } else {
            AddLog(LOG_LEVEL_INFO,PSTR("BLE: Delete device %s(%s) by addrtype filter %d > %d."),
              addr, alias, dev->addrtype, BLEAddressFilter);
          }
#endif
          seenDevices.erase(seenDevices.begin()+i);
          freeDevices.push_back(dev);
          res++;
        }
    }
  }
  if (res){
#ifdef BLE_ESP32_DEBUG
    AddLog(LOG_LEVEL_INFO,PSTR("BLE: Deleted %d devices"), res);
#endif
  }
  return res;
}

int deleteSeenDevice(uint8_t *mac){
  int res = 0;
  TasAutoMutex localmutex(&BLEDevicesMutex, "BLEDel2");
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


void checkDeviceTimouts(){
  if (BLEMaxAge){
    deleteSeenDevices(BLEMaxAge);
  }
}


///////////////////////////////////////////////////////
// returns age of device or 0.  if age IS0, returns 1s
uint32_t devicePresent(uint8_t *mac){
  int res = 0;
  uint64_t now = esp_timer_get_time();
  now = now/1000L;
  now = now/1000L;
  uint32_t nowS = (uint32_t)now;

  TasAutoMutex localmutex(&BLEDevicesMutex, "BLEPRes");
  for (int i = 0; i < seenDevices.size(); i++){
    if (!memcmp(seenDevices[i]->mac, mac, 6)){
      uint64_t lastseen = seenDevices[i]->lastseen/1000L;
      lastseen = lastseen/1000L;
      uint32_t lastseenS = (uint32_t) lastseen;
      uint32_t ageS = nowS-lastseenS;
      if (!ageS) ageS++;
      res = ageS;
      break;
    }
  }
  return res;
}


// the MAX we could expect.
#define MAX_DEV_JSON_NAME_LEN BLE_ESP32_MAXNAMELEN
#define MAX_DEV_JSON_RSSI_LEN 3
#define MAX_DEV_JSON_INDEX_LEN 3
#define MAX_DEV_JSON_ALIAS_LEN BLE_ESP32_MAXALIASLEN
// "001122334455":{"i":123,"n":"01234567890123456789","r":-77}\0
#define MIN_REQUIRED_DEVJSON_LEN \
  (1+12+1 + 1 + 1 + \
  +4 + MAX_DEV_JSON_INDEX_LEN \
  +1 + 4 + MAX_DEV_JSON_NAME_LEN + 2 \
  +1 + 4 + MAX_DEV_JSON_RSSI_LEN + 2 \
  +1 + 4 + MAX_DEV_JSON_ALIAS_LEN + 2 \
  +1 +1 \
  )
int getSeenDeviceToJson(int index, BLE_ESP32::BLE_simple_device_t* dev, char **dest, int *maxlen){
  char *p = *dest;
  // add 20 to be sure
  if (*maxlen < MIN_REQUIRED_DEVJSON_LEN+20){
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

  if (dev->name[0]){
    *((*dest)++) = ',';
    *((*dest)++) = '"';
    *((*dest)++) = 'n';
    *((*dest)++) = '"';
    *((*dest)++) = ':';
    *((*dest)++) = '"';
    *(*dest) = 0; // must term, else it adds to the *end* of old data!
    strncat((*dest), dev->name, MAX_DEV_JSON_NAME_LEN);
    (*dest) += strlen((*dest));
    *((*dest)++) = '"';
  }
  *((*dest)++) = ',';
  *((*dest)++) = '"';
  *((*dest)++) = 'r';
  *((*dest)++) = '"';
  *((*dest)++) = ':';
  sprintf((*dest), "%d", dev->RSSI);
  (*dest) += strlen((*dest));

  const char *alias = getAlias(dev->mac);
  if (alias && alias[0]){
    *((*dest)++) = ',';
    *((*dest)++) = '"';
    *((*dest)++) = 'a';
    *((*dest)++) = '"';
    *((*dest)++) = ':';
    *((*dest)++) = '"';
    sprintf((*dest), "%s", alias);
    (*dest) += strlen((*dest));
    *((*dest)++) = '"';
  }

  *((*dest)++) = '}';
  *maxlen -= (*dest - p);
  return 1;
}


int nextSeenDev = 0;

int getSeenDevicesToJson(char *dest, int maxlen){

  if ((nextSeenDev == 0) || (nextSeenDev >= seenDevices.size())){
    nextSeenDev = 0;
  }

  // deliberate test of SafeAddLog_P from main thread...
  //AddLog(LOG_LEVEL_INFO,PSTR("BLE: getSeen %d"), seenDevices.size());


  int len;
  if (!maxlen) return 0;
  strcpy((dest), ",\"BLEDevices\":{");
  len = strlen(dest);
  dest += len;
  maxlen -= len;

  int added = 0;
  TasAutoMutex localmutex(&BLEDevicesMutex, "BLEGet");

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
      int res = getSeenDeviceToJson(nextSeenDev, seenDevices[nextSeenDev], &dest, &maxlen);
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

/*
#ifdef BLE_ESP32_DEBUG
  #define MAX_SAFELOG_LEN 40
  #define MAX_SAFELOG_COUNT 25
#else
  #define MAX_SAFELOG_LEN 20
  #define MAX_SAFELOG_COUNT 5
#endif

struct safelogdata {
  int level;
  char log_data[MAX_SAFELOG_LEN];
};

std::deque<BLE_ESP32::safelogdata*> freelogs;
std::deque<BLE_ESP32::safelogdata*> filledlogs;
uint8_t filledlogsOverflows = 0;
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
  if ((loglevel > Settings->weblog_level) &&
      (loglevel > TasmotaGlobal.seriallog_level) &&
      (loglevel > Settings->mqttlog_level) &&
      (loglevel > TasmotaGlobal.syslog_level)){
    return added;
  }

  char BLE_temp_log_data[MAX_SAFELOG_LEN];
  // as these are'expensive', let's not bother unless they are lower than the serial log level
#ifndef USE_NATIVE_LOGGING
  xSemaphoreTake(SafeLogMutex, portMAX_DELAY);
#endif
  int maxlen = sizeof(BLE_temp_log_data)-3;
  if (thistask == TasmotaMainTask){
    maxlen -= 13; // room for "-!MAINTHREAD!"
  }
  // assume this is thread safe - it may not be
  va_list arg;
  va_start(arg, formatP);
  vsnprintf_P(BLE_temp_log_data, maxlen, formatP, arg);
  va_end(arg);
#ifdef USE_NATIVE_LOGGING
  AddLog(loglevel, PSTR("%s"), BLE_temp_log_data);
  return 1;
#else
  if (thistask == TasmotaMainTask){
    loglevel = LOG_LEVEL_ERROR;
    snprintf(BLE_temp_log_data + strlen(BLE_temp_log_data), 13, "-!MAINTHREAD!");
    xSemaphoreGive(SafeLogMutex); // release mutex
    AddLog(loglevel, PSTR("%s"), BLE_temp_log_data);
    return 0;
  }

  if (freelogs.size()){
    BLE_ESP32::safelogdata* logdata = (freelogs)[0];
    freelogs.pop_front();
    logdata->level = loglevel;
    memcpy(logdata->log_data, BLE_temp_log_data, sizeof(logdata->log_data));
    filledlogs.push_back(logdata);
    added = 1;
  } else {
    // can't log it?
    filledlogsOverflows++;
  }
  xSemaphoreGive(SafeLogMutex); // release mutex
  return added;
#endif
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
*/

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

  memset(dest, 0, maxlen);

  for (int i = 0; i < srclen; i++){
    char t[3];

    t[0] = src[i*2];
    t[1] = src[i*2 + 1];
    t[2] = 0;
    if (t[0] == '/'){
      t[0] = '0';
    }
    if (t[1] == '/'){
      t[1] = '0';
    }

    if (!isalnum(t[0])){
      return 0;
    }
    if (!isalnum(t[1])){
      return 0;
    }


    t[0] |= 0x20;
    t[1] |= 0x20;
    if (isalpha(t[0])){
      if (t[0] < 'a' || t[0] > 'f'){
        return 0;
      }
    }
    if (isalpha(t[1])){
      if (t[1] < 'a' || t[1] > 'f'){
        return 0;
      }
    }

    int byte = strtol(t, NULL, 16);
    *dest++ = byte;
  }
  return srclen;
}

int fromMAC(uint8_t *dest, const char *src, int maxlen){
  int len = fromHex(dest, src, maxlen);
  if ((len == 6) && (maxlen == 7)){
    dest[6] = 0;
    len = 7;
  }
  return len;
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
  TasAutoMutex localmutex(&BLEOperationsRecursiveMutex, "BLESetDet");
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
  strcpy(p, "\"DetailsBLE\":{");
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
  if (ad->addrtype){
    *(p++) = '/';
    *(p++) = 0x30+ad->addrtype;
  }
  *(p++) = '\"'; maxlen--;

  const char *alias = BLE_ESP32::getAlias(ad->addr);
  if (alias && (*alias)){
    strcpy(p, ",\"a\":\"");
    len = strlen(p);
    p += len;
    maxlen -= len;
    strcpy(p, alias);
    len = strlen(p);
    p += len;
    maxlen -= len;
    *(p++) = '\"'; maxlen--;
  }

  sprintf(p, ",\"RSSI\":%d", ad->RSSI);
  len = strlen(p);
  p += len;

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
//  if (TasmotaGlobal.ota_state_flag) return;

  TasAutoMutex localmutex(&BLEOperationsRecursiveMutex, "BLEPostAdd");
  if (BLEAdvertismentDetailsJsonSet){

//    strncpy(TasmotaGlobal.mqtt_data, BLEAdvertismentDetailsJson, sizeof(TasmotaGlobal.mqtt_data));
//    TasmotaGlobal.mqtt_data[sizeof(TasmotaGlobal.mqtt_data)-1] = 0;
    Response_P(BLEAdvertismentDetailsJson);

    BLEAdvertismentDetailsJsonSet = 0;
    // we got the data, give before MQTT call.
    localmutex.give();
    // no retain - this is present devices, not historic
    MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR("BLE"), 0);
  } else {
  }
}



/*********************************************************************************************\
 * Classes
\*********************************************************************************************/

// does not really take any action
class BLESensorCallback : public NimBLEClientCallbacks {
  void onConnect(NimBLEClient* pClient) {
#ifdef BLE_ESP32_DEBUG
    if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: onConnect %s"), ((std::string)pClient->getPeerAddress()).c_str());
#endif
  }
  void onDisconnect(NimBLEClient* pClient, int reason) {
#ifdef BLE_ESP32_DEBUG
    if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: onDisconnect %s"), ((std::string)pClient->getPeerAddress()).c_str());
#endif
  }
  bool onConnParamsUpdateRequest(NimBLEClient* pClient, const ble_gap_upd_params* params) {
#ifdef BLE_ESP32_DEBUG
    if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: onConnParamsUpdateRequest %s"), ((std::string)pClient->getPeerAddress()).c_str());
#endif

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


class BLEAdvCallbacks: public NimBLEScanCallbacks {
  void onScanEnd(NimBLEScanResults results) {
    BLEscanEndedCB(results);
  }

  void onResult(NimBLEAdvertisedDevice* advertisedDevice) {
    TasAutoMutex localmutex(&BLEOperationsRecursiveMutex, "BLEAddCB");
    uint64_t now = esp_timer_get_time();
    BLEScanLastAdvertismentAt = now; // note the time of the last advertisment

    uint32_t totalCount = BLEAdvertisment.totalCount;
    memset(&BLEAdvertisment, 0, sizeof(BLEAdvertisment));
    BLEAdvertisment.totalCount = totalCount+1;

    BLEAdvertisment.advertisedDevice = advertisedDevice;

    // keep sign - char seems unsigned
    int8_t RSSI = (char)advertisedDevice->getRSSI();
    NimBLEAddress address = advertisedDevice->getAddress();

    BLEAdvertisment.addrtype = address.getType();

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
      strncpy(BLEAdvertisment.name, namestr, sizeof(BLEAdvertisment.name)-1);
      BLEAdvertisment.name[sizeof(BLEAdvertisment.name)-1] = 0;
    }


    // log this device safely
    if (BLEAdvertisment.addrtype <= BLEAddressFilter){
      addSeenDevice(BLEAdvertisment.addr, BLEAdvertisment.addrtype, BLEAdvertisment.name, BLEAdvertisment.RSSI);
    }

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
          // ignore from here on if filtered on addrtype
          if (BLEAdvertisment.addrtype > BLEAddressFilter){
            return;
          }
          setDetails(&BLEAdvertisment);
        } break;
        case 4:{ // all adverts for all aliased DEVICES - may not get them all
          // ignore from here on if filtered on addrtype
          const char *alias = BLE_ESP32::getAlias(BLEAdvertisment.addr);
          if (!alias || !(*alias)){
            return;
          }
          setDetails(&BLEAdvertisment);
        } break;
      }
    }

    // ignore from here on if filtered on addrtype
    if (BLEAdvertisment.addrtype > BLEAddressFilter){
      return;
    }

    // call anyone who asked about advertisements
    for (int i = 0; i < advertismentCallbacks.size(); i++) {
      ADVERTISMENT_CALLBACK* pFN;
      pFN = advertismentCallbacks[i];
      int res = pFN(&BLEAdvertisment);

      // if this callback wants to stop here, then do so.
      if (1 == res) break;

      // if this callback wants to kill this device
      if (2 == res) {
        //BLEScan->erase(address);
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

#ifdef BLE_ESP32_DEBUG
  if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: Scan ended"));
#endif
  for (int i = 0; i < scancompleteCallbacks.size(); i++){
    SCANCOMPLETE_CALLBACK *pFn = scancompleteCallbacks[i];
    int callbackres = pFn(results);
#ifdef BLE_ESP32_DEBUG
    if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: scancompleteCallbacks %d %d"), i, callbackres);
#endif
  }

  BLERunningScan = 2;
  BLEScanToEndBefore = 0L;
  BLEScanCount++;
}


///////////////////////////////////////////////////////////////////////
// !!!!!!!!!!@@@@@@@@@@@@@@@@
// NOTE: this can callback BEFORE the write is completed.
// so we should not do any actions against the device if we can help it
// this COULD be the reason for the BLE stack hanging up....
///////////////////////////////////////////////////////////////////////
static void BLEGenNotifyCB(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify){
  NimBLEClient *pRClient;

  if (!pRemoteCharacteristic){
#ifdef BLE_ESP32_DEBUG
    AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: Notify: no remote char!!??"));
#endif
    return;
  }


#ifdef BLE_ESP32_DEBUG
  if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: Notified length: %u"),length);
#endif
  // find the operation this is associated with
  NimBLERemoteService *pSvc = pRemoteCharacteristic->getRemoteService();

  if (!pSvc){
#ifdef BLE_ESP32_DEBUG
    AddLog(LOG_LEVEL_ERROR,PSTR("BLE: Notify: no remote service found"));
#endif
    return;
  }

  pRClient = pSvc->getClient();
  if (!pRClient){
#ifdef BLE_ESP32_DEBUG
    AddLog(LOG_LEVEL_ERROR,PSTR("BLE: Notify: no remote client!!??"));
#endif
    return;
  }
  NimBLEAddress devaddr = pRClient->getPeerAddress();

  generic_sensor_t *thisop = nullptr;
  {
    // make sure we are not disturbed
    TasAutoMutex localmutex(&BLEOperationsRecursiveMutex, "BLENotif");

    for (int i = 0; i < currentOperations.size(); i++){
      generic_sensor_t *op = currentOperations[i];
      if (!op){
#ifdef BLE_ESP32_DEBUG
        AddLog(LOG_LEVEL_ERROR,PSTR("BLE: Notify: null op in currentOperations!!??"));
#endif
      } else {
        if (devaddr == op->addr){
          if (op->notifytimer){
            thisop = op;
            break;
          } else {
            AddLog(LOG_LEVEL_ERROR,PSTR("BLE: notify: op addr match but op found which is not waiting."));
          }
        }
      }
    }
  }

  // we'll try without
  //pRemoteCharacteristic->unsubscribe();

  if (!thisop){
#ifdef BLE_ESP32_DEBUG
    AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: no op for notify"));
#endif
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
#ifdef BLE_ESP32_DEBUG
  AddLog(LOG_LEVEL_INFO,PSTR("BLE: registerForAdvertismentCallbacks %s:%x"), tag, (uint32_t) pFn);
#endif
  advertismentCallbacks.push_back(pFn);
}

void registerForOpCallbacks(const char *tag, BLE_ESP32::OPCOMPLETE_CALLBACK* pFn){
#ifdef BLE_ESP32_DEBUG
  AddLog(LOG_LEVEL_INFO,PSTR("BLE: registerForOpCallbacks %s:%x"), tag, (uint32_t) pFn);
#endif
  operationsCallbacks.push_back(pFn);
}

void registerForScanCallbacks(const char *tag, BLE_ESP32::SCANCOMPLETE_CALLBACK* pFn){
#ifdef BLE_ESP32_DEBUG
  AddLog(LOG_LEVEL_INFO,PSTR("BLE: registerForScnCallbacks %s:%x"), tag, (uint32_t) pFn);
#endif
  scancompleteCallbacks.push_back(pFn);
}


/*********************************************************************************************\
 * init NimBLE
\*********************************************************************************************/
static void BLEPreInit(void) {
  BLEInitState = 0;
  prepOperation = nullptr;
}


static void BLEInit(void) {
  if (BLEMode == BLEModeDisabled) return;

  if (BLEInitState) { return; }

  if (TasmotaGlobal.global_state.wifi_down && TasmotaGlobal.global_state.eth_down) { return; }

  if (!TasmotaGlobal.global_state.wifi_down) {
    TasmotaGlobal.wifi_stay_asleep = true;
    if (WiFi.getSleep() == false) {
      AddLog(LOG_LEVEL_DEBUG,PSTR("%s: Put WiFi modem in sleep mode"), "BLE");
      WiFi.setSleep(true); // Sleep
    }
  }

#ifdef BLE_ESP32_EXAMPLES
// this is only for testing, does nothin if examples are undefed
  installExamples();
  //initSafeLog();
  initSeenDevices();
#endif

  uint64_t now = esp_timer_get_time();
  BLEScanLastAdvertismentAt = now; // initialise the time of the last advertisment
  BLELastLoopTime = now;

  BLEInitState = 1;

  return;
}

/*********************************************************************************************\
 * Task section
\*********************************************************************************************/

static void BLEOperationTask(void *pvParameters);

static void BLEStartOperationTask(){
  if (BLERunning == false){
#ifdef BLE_ESP32_DEBUG
    AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: %s: Start operations"),D_CMND_BLE);
#endif
    BLERunning = true;

    xTaskCreatePinnedToCore(
      BLE_ESP32::BLEOperationTask,    /* Function to implement the task */
      "BLEOperationTask",  /* Name of the task */
      4096,             /* Stack size in bytes */
      NULL,             /* Task input parameter */
      0,                /* Priority of the task */
      NULL,             /* Task handle. */
#ifdef CONFIG_FREERTOS_UNICORE
      0);               /* Core where the task should run */
#else
      1);               /* Core where the task should run */
#endif
  }
}


static void BLETaskStopStartNimBLE(NimBLEClient **ppClient, bool start = true){

  if (*ppClient){
    AddLog(LOG_LEVEL_ERROR,PSTR("BLE: Task:Stopping NimBLE"));

    (*ppClient)->setClientCallbacks(nullptr, false);

    if ((*ppClient)->isConnected()){
#ifdef BLE_ESP32_DEBUG
      AddLog(LOG_LEVEL_INFO,PSTR("BLE: disconnecting connected client"));
#endif
      (*ppClient)->disconnect();
    }
    NimBLEDevice::deleteClient((*ppClient));
    (*ppClient) = nullptr;
#ifdef BLE_ESP32_DEBUG
    AddLog(LOG_LEVEL_INFO,PSTR("BLE: deleted client"));
#endif

    if (ble32Scan){
      ble32Scan->setScanCallbacks(nullptr,true);
      ble32Scan->stop();
      ble32Scan = nullptr;
    }

    // wait second
    vTaskDelay(100/ portTICK_PERIOD_MS);
    NimBLEDevice::deinit(true);
  }
  BLERunningScan = 0;

  if (start){
    AddLog(LOG_LEVEL_INFO,PSTR("BLE: BLETask:Starting NimBLE"));
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
    // this is now in ms!!!! despite docs.
    // let's just leave it at the default 30s?
    //(*ppClient)->setConnectTimeout(15 * 1000);
  }

  uint64_t now = esp_timer_get_time();

  // don't restart because of these for a while
  BLELastLoopTime = now; // initialise the time of the last advertisment
  BLEScanLastAdvertismentAt = now; // initialise the time of the last advertisment

}

int BLETaskStartScan(int time){
  if (!ble32Scan) return -1;
  if (BLEMode == BLEModeDisabled) return -4;
  // don't scan whilst OTA in progress
  if (BLEOtaStallBLE) return -5;
  //if (currentOperations.size()) return -3;

  if (BLERunningScan) {

    // if we hit 2, wait one more time before starting
    if (BLERunningScan == 2){
      // wait 10ms
      vTaskDelay(10/ portTICK_PERIOD_MS);
      BLERunningScan = 0;
    } else {
      return -2;
    }
  }

#ifdef BLE_ESP32_DEBUG
  if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: BLETask: Startscan"));
#endif
  //vTaskDelay(500/ portTICK_PERIOD_MS);
  ble32Scan->setActiveScan(BLEScanActiveMode ? 1: 0);
  // we read the results dynamically as they come in.
  ble32Scan->setMaxResults(0);

  // seems we could get the callback within the start call....
  // so set these before starting
  BLERunningScan = 1;
  BLEScanStartedAt = esp_timer_get_time();
  if (BLETriggerScan){
    time = BLETriggerScan;
    BLETriggerScan = 0;
  }

  // note: this is documented as being seconds.  However, experience and Apache docs tells us ms.
  time = time * 1000;
  ble32Scan->start(time, false); // 20s scans, restarted when then finish
  
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
#ifdef BLE_ESP32_DEBUG
      if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: BLETask: new currentOperation"));
#endif
      BLEOpCount++;
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
#ifdef BLE_ESP32_DEBUG
      AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: BLETask: notify timeout"));
#endif
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
#ifdef BLE_ESP32_DEBUG
      if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: BLETask: notify operation complete"));
#endif
      BLE_ESP32::BLETaskRunTaskDoneOperation(pCurrentOperation, ppClient);
      pClient = *ppClient;
      return;
      break;
    case GEN_STATE_READDONE:
    case GEN_STATE_WRITEDONE:
    case GEN_STATE_NOTIFIED: // - may have completed DURING our read/write to get here
      // just stay here until this is removed by the main thread
#ifdef BLE_ESP32_DEBUG
      if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: BLETask: operation complete"));
#endif
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
#ifdef BLE_ESP32_DEBUG
    AddLog(LOG_LEVEL_ERROR,PSTR("BLE: BLETask: op failed %d"), (*pCurrentOperation)->state);
#endif
    BLE_ESP32::BLETaskRunTaskDoneOperation(pCurrentOperation, ppClient);
    pClient = *ppClient;
    return;
  }

  if ((*pCurrentOperation)->state != GEN_STATE_START){
    return;
  }

  if (pClient->isConnected()){
    // don't do anything if we are still connected
#ifdef BLE_ESP32_DEBUG
    AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: BLETask: still connected"));
#endif
    return;
  }


  // if we managed to run operations back to back with long connection timeouts,
  // then we may NOT see advertisements.
  // so to prevent triggering of the advert timeout restart mechanism,
  // set the last advert time each time we start an operation
  uint64_t now = esp_timer_get_time();
  BLEScanLastAdvertismentAt = now; // initialise the time of the last advertisment


  generic_sensor_t* op = *pCurrentOperation;

  int newstate = GEN_STATE_STARTED;
  op->state = GEN_STATE_STARTED;

#ifdef BLE_ESP32_DEBUG
  if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: BLETask: attempt connect %s"), ((std::string)op->addr).c_str());
#endif

  if (!op->serviceUUID.bitSize()){
    op->state = GEN_STATE_FAILED_NOSERVICE;
    return;
  }

  if (pClient->connect(op->addr, true)) {

    // as soon as connected, start another scan if possible
    BLE_ESP32::BLETaskStartScan(20);

#ifdef BLE_ESP32_DEBUG
    if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: connected %s -> getservice"), ((std::string)op->addr).c_str());
#endif
    NimBLERemoteService *pService = pClient->getService(op->serviceUUID);
    int waitNotify = false;
    int notifystate = 0;
    op->notifytimer = 0L;

    if (pService != nullptr) {
#ifdef BLE_ESP32_DEBUG
      if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: got service"));
#endif
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
#ifdef BLE_ESP32_DEBUG
          if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: got notify characteristic"));
#endif
          op->notifylen = 0;
          if(pNCharacteristic->canNotify()) {
            uint64_t now = esp_timer_get_time();
            op->notifytimer = now;
            if(pNCharacteristic->subscribe(true, BLE_ESP32::BLEGenNotifyCB)) {
#ifdef BLE_ESP32_DEBUG
              if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: subscribe for notify"));
#endif
              // this will get changed to read or write,
              // but here in case it's notify only (can that happen?)
              notifystate = GEN_STATE_WAITNOTIFY;
              waitNotify = true;
            } else {
#ifdef BLE_ESP32_DEBUG
              AddLog(LOG_LEVEL_ERROR,PSTR("BLE: failed subscribe for notify"));
#endif
              newstate = GEN_STATE_FAILED_NOTIFY;
              op->notifytimer = 0L;
            }
          } else {
            if(pNCharacteristic->canIndicate()) {
              uint64_t now = esp_timer_get_time();
              op->notifytimer = now;
              if(pNCharacteristic->subscribe(false, BLE_ESP32::BLEGenNotifyCB)) {
#ifdef BLE_ESP32_DEBUG
                AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: subscribe for indicate"));
#endif
                notifystate = GEN_STATE_WAITINDICATE;
                waitNotify = true;
              } else {
#ifdef BLE_ESP32_DEBUG
                AddLog(LOG_LEVEL_ERROR,PSTR("BLE: failed subscribe for indicate"));
#endif
                newstate = GEN_STATE_FAILED_INDICATE;
                op->notifytimer = 0L;
              }
            } else {
              newstate = GEN_STATE_FAILED_CANTNOTIFYORINDICATE;
#ifdef BLE_ESP32_DEBUG
              AddLog(LOG_LEVEL_ERROR,PSTR("BLE: characteristic can't notify"));
#endif
            }
          }
        } else {
          newstate = GEN_STATE_FAILED_NONOTIFYCHAR;
#ifdef BLE_ESP32_DEBUG
          AddLog(LOG_LEVEL_ERROR,PSTR("BLE: notify characteristic not found"));
#endif
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
#ifdef BLE_ESP32_DEBUG
            if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: got read/write characteristic"));
#endif
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
#ifdef BLE_ESP32_DEBUG
                  if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: read characteristic with readmodifywritecallback"));
#endif
                  pFn(op);
                } else {
#ifdef BLE_ESP32_DEBUG
                  if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: read characteristic"));
#endif
                }

                // only change it to a 'finished' state if we really are
                if (!waitNotify && !op->writelen) newstate = GEN_STATE_READDONE;

              } else {
                newstate = GEN_STATE_FAILED_CANTREAD;
              }
            }
            if (op->writelen){
              if(pCharacteristic->canWrite() || pCharacteristic->canWriteNoResponse() ) {
                if (!pCharacteristic->writeValue(op->dataToWrite, op->writelen, !pCharacteristic->canWriteNoResponse())){ // request response, unless we can't
                  newstate = GEN_STATE_FAILED_WRITE;
#ifdef BLE_ESP32_DEBUG
                  AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: characteristic write fail"));
#endif
                } else {
                  if (!waitNotify) newstate = GEN_STATE_WRITEDONE;
#ifdef BLE_ESP32_DEBUG
                  if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: write characteristic"));
#endif
                }
              } else {
                newstate = GEN_STATE_FAILED_CANTWRITE;
              }
            }
            // print or do whatever you need with the value

          } else {
            newstate = GEN_STATE_FAILED_NO_RW_CHAR;
#ifdef BLE_ESP32_DEBUG
            AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: r/w characteristic not found"));
#endif
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
#ifdef BLE_ESP32_DEBUG
      AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: failed - svc not on device?"));
#endif
    }

  } else { // connect itself failed
    newstate = GEN_STATE_FAILED_CONNECT;
    int rc = pClient->getLastError();

    switch (rc){
      case (0x0200+BLE_ERR_CONN_LIMIT ):
        AddLog(LOG_LEVEL_ERROR,PSTR("BLE: Hit connection limit? - restarting NimBLE"));
        BLERestartNimBLE = 1;
        BLERestartBLEReason = BLE_RESTART_BLE_REASON_CONN_LIMIT;
        break;
      case (0x0200+BLE_ERR_ACL_CONN_EXISTS):
        AddLog(LOG_LEVEL_ERROR,PSTR("BLE: Connection exists? - restarting NimBLE"));
        BLERestartNimBLE = 1;
        BLERestartBLEReason = BLE_RESTART_BLE_REASON_CONN_EXISTS;
        break;
    }
    if (rc){
      AddLog(LOG_LEVEL_ERROR,PSTR("BLE: failed to connect to device low level rc 0x%x"), rc);
    }
    // failed to connect
    AddLog(LOG_LEVEL_ERROR,PSTR("BLE: failed to connect to device"));
  }
  op->state = newstate;
}



// this disconnects from a device if necessary, and then
// moves the operation from 'currentOperations' to 'completedOperations'.

// for safety's sake, only call from the run task
static void BLETaskRunTaskDoneOperation(BLE_ESP32::generic_sensor_t** op, NimBLEClient **ppClient){
  if ((*ppClient)->isConnected()){
#ifdef BLE_ESP32_DEBUG
    if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: runTaskDoneOperation: disconnecting connected client"));
#endif
    (*ppClient)->disconnect();
    // wait for 1/2 second after disconnect
    int waits = 0;
    do {
      vTaskDelay(500/ portTICK_PERIOD_MS);
      if (waits) {
        //(*ppClient)->disconnect();
        // we will stall here forever!!! - as testing
#ifdef BLE_ESP32_DEBUG
        AddLog(LOG_LEVEL_ERROR,PSTR("BLE: wait discon%d"), waits);
#endif
        vTaskDelay(500/ portTICK_PERIOD_MS);
      }
      waits++;
      if (waits == 5){
        int conn_id = (*ppClient)->getConnId();
#ifdef DEPENDSONNIMBLEARDUINO        
        ble_gap_conn_broken(conn_id, -1);
#endif        
#ifdef BLE_ESP32_DEBUG
        AddLog(LOG_LEVEL_ERROR,PSTR("BLE: wait discon%d - kill connection"), waits);
#endif
      }
      if (waits == 60){
        AddLog(LOG_LEVEL_ERROR,PSTR("BLE: >60s waiting -> BLE Failed, restart Tasmota %d"), waits);
        BLEStop = 1;
        BLEStopAt = esp_timer_get_time();

        BLERestartTasmota = 10;
        BLERestartTasmotaReason = BLE_RESTART_TEAMOTA_REASON_BLE_DISCONNECT_FAIL;
        break;
      }
    } while ((*ppClient)->isConnected());
  }


  {
    TasAutoMutex localmutex(&BLEOperationsRecursiveMutex, "BLEDoneOp");

    // find this operation in currentOperations, and remove it.
    for (int i = 0; i < currentOperations.size(); i++){
      if (currentOperations[i]->opid == (*op)->opid){
        currentOperations.erase(currentOperations.begin() + i);
        break;
      }
    }
  }


  // by adding it to this list, this will cause it to be sent to MQTT
#ifdef BLE_ESP32_DEBUG
  if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: runTaskDoneOperation: add to completedOperations"));
#endif
  addOperation(&completedOperations, op);
  return;
}




// this IS as task.
// we MAY be able to run a few of these simultaneously, but this is not yet tested.
// and probably not required.  But everything is there to do so....
static void BLEOperationTask(void *pvParameters){

  BLELoopCount = 0;
  BLEOpCount = 0;;

  uint32_t timer = 0;
  // operation which is currently in progress in THIS TASK
  generic_sensor_t* currentOperation = nullptr;

  NimBLEClient *pClient = nullptr;
  BLE_ESP32::BLETaskStopStartNimBLE(&pClient);

  for(;;){
    BLELastLoopTime = esp_timer_get_time();
    BLELoopCount++;

    BLE_ESP32::BLETaskRunCurrentOperation(&currentOperation, &pClient);

    // start a scan if possible
    if ((BLEMode == BLEModeRegularScan) || (BLETriggerScan)){
      BLEScan* lastScan = ble32Scan;
      ble32Scan = NimBLEDevice::getScan();
      if (lastScan != ble32Scan){
        //ble32Scan->setInterval(70);
        //ble32Scan->setWindow(50);
        ble32Scan->setInterval(0x40);
        ble32Scan->setWindow(0x20);
        ble32Scan->setScanCallbacks(&BLEScanCallbacks,true);
      }

      BLE_ESP32::BLETaskStartScan(20);
    }

    if (BLEStopScan){
      ble32Scan->stop();
      BLEStopScan = 0;
    }

    // come around every 1/10s
    vTaskDelay(100/ portTICK_PERIOD_MS);

    if (BLEStop == 1){
      break;
    }

    if (BLERestartNimBLE){
      BLERestartNimBLE = 0;
      BLERestartTasmota = 10;
      BLERestartTasmotaReason = BLE_RESTART_TEAMOTA_REASON_RESTARTING_BLE_TIMEOUT;
      AddLog(LOG_LEVEL_ERROR,PSTR("BLE: BLETask: Restart NimBLE - restart Tasmota in 10 if not complt"));
      BLE_ESP32::BLETaskStopStartNimBLE(&pClient);
      BLERestartTasmotaReason = BLE_RESTART_TEAMOTA_REASON_UNKNOWN;
      BLERestartTasmota = 0;
      BLEResets ++;
    }
  }

  BLE_ESP32::BLETaskStopStartNimBLE(&pClient, false);

  // wait 1/10 second
  vTaskDelay(100/ portTICK_PERIOD_MS);

#ifdef BLE_ESP32_DEBUG
  AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: BLEOperationTask: Left task"));
#endif
  deleteSeenDevices();

  BLEStop = 2;
  BLERunning = false;
  vTaskDelete( NULL );
}





/***********************************************************************\
 * Regular Tasmota called functions
 *
\***********************************************************************/
void BLEEvery50mSecond(){
/*  if (BLEAliasListTrigger){
    BLEAliasListTrigger = 0;
    BLEAliasMqttList();
  }*/
  postAdvertismentDetails();
}


static void stopStartBLE(){
  // dont start of disabled
  uint8_t enable = (Settings->flag5.mi32_enable || BLEEnableUnsaved) && BLEEnableMask;

  if (enable != BLEMasterEnable){
    if (enable){
      if (StartBLE()){
        BLEMasterEnable = enable;
      }
    } else {
      if (StopBLE()){
        BLEMasterEnable = enable;
      }
    }
    AddLog(LOG_LEVEL_INFO,PSTR("BLE: MasterEnable->%d"), BLEMasterEnable);
  }
}

/**
 * @brief Main loop of the driver, "high level"-loop
 *
 */

static void BLEEverySecond(bool restart){

  BLEDiag();

  checkDeviceTimouts();

  stopStartBLE();

  // check for application callbacks here.
  // this may remove complete items.
  BLE_ESP32::mainThreadOpCallbacks();

  // post any MQTT data if we completed anything in the last second
  if (mqttOperations.size()){
    BLE_ESP32::BLEPostMQTT(true); // send only completed
  }

  // request send of ALL oeprations prepped, queued, in progress -
  // in separate MQTT messages.
  if (BLEPostMQTTTrigger){
    BLEPostMQTTTrigger = 0;
    BLE_ESP32::BLEPostMQTT(false); // show all operations, not just completed
  }

  if (BLEPublishDevices){
    BLEPostMQTTSeenDevices(BLEPublishDevices);
    BLEShowStats();
    BLEPublishDevices = 0;
  }

  // we have been asked to restart in this many seconds....
  if (BLERestartTasmota){
    BLERestartTasmota--;
    // 2 seconds to go, post to BLE topic on MQTT our reason
    if (BLERestartTasmota == 2){
      if (!BLERestartTasmotaReason) BLERestartTasmotaReason = BLE_RESTART_TEAMOTA_REASON_UNKNOWN;
      Response_P(PSTR("{\"reboot\":\"%s\"}"), BLERestartTasmotaReason);
      MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR("BLE"), Settings->flag.mqtt_sensor_retain);
      AddLog(LOG_LEVEL_ERROR,PSTR("BLE: Failure! Restarting Tasmota in %d seconds because %s"), BLERestartTasmota, BLERestartTasmotaReason);
    }

    if (!BLERestartTasmota){
      AddLog(LOG_LEVEL_ERROR,PSTR("BLE: Failure! Restarting Tasmota because %s"), BLERestartTasmotaReason);
      // just a normal restart
      TasmotaGlobal.restart_flag = 1;
    }
  }

  if (BLERestartBLEReason){ // just use the ptr as the trigger to send MQTT
    Response_P(PSTR("{\"blerestart\":\"%s\"}"), BLERestartBLEReason);
    MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR("BLE"), Settings->flag.mqtt_sensor_retain);
    AddLog(LOG_LEVEL_ERROR,PSTR("BLE: Failure! Restarting BLE Stack because %s"), BLERestartBLEReason);
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
    TasAutoMutex localmutex(&BLEOperationsRecursiveMutex, "BLENExtOp");
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
    TasAutoMutex localmutex(&BLEOperationsRecursiveMutex, "BLEAddOp");
    if (ops->size() < 10){
      ops->push_back(*op);
      *op = nullptr;
      res = 1;
    }
  }
  if (res){
    //AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: added operation"));
  } else {
    AddLog(LOG_LEVEL_ERROR,PSTR("BLE: op - no room"));
  }
  return res;
}


int newOperation(BLE_ESP32::generic_sensor_t** op){
  if (!op) {
    AddLog(LOG_LEVEL_ERROR,PSTR("BLE: op inv in newOperation"));
    return 0;
  }

  BLE_ESP32::generic_sensor_t *o = new BLE_ESP32::generic_sensor_t;

  // clear to zeros, but not the NimBLE classes
  o->state = 0;
  o->opid = 0; // incrementing id so we can find them
  o->notifytimer = 0L;
  //uint8_t writeRead[MAX_BLE_DATA_LEN];
  o->writelen = 0;
  //uint8_t dataRead[MAX_BLE_DATA_LEN];
  o->readlen = 0;
  o->readtruncated = 0;
  //uint8_t dataNotify[MAX_BLE_DATA_LEN];
  o->notifylen = 0;
  o->notifytruncated = 0;
  o->readmodifywritecallback = nullptr; // READ_CALLBACK function, used by external drivers
  o->completecallback = nullptr; // OPCOMPLETE_CALLBACK function, used by external drivers
  o->context = nullptr; // opaque context, used by external drivers, or can be set to a long for MQTT

  (*op) = o;
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
  if (!op || !(*op)) {
    AddLog(LOG_LEVEL_ERROR,PSTR("BLE: op invalid"));
    return 0;
  }

  if (!BLEMasterEnable){
    AddLog(LOG_LEVEL_ERROR,PSTR("BLE: extQueueOperation: BLE is deiabled"));
    return 0;
  }

  (*op)->state = GEN_STATE_START; // trigger request later
  (*op)->opid = lastopid++;

  int res = addOperation(&queuedOperations, op);
  if (!res){
    AddLog(LOG_LEVEL_ERROR,PSTR("BLE: extQueueOperation: op adding id %d failed"), (lastopid-1));
  }
  return res;
}


/*********************************************************************************************\
 * BLE Name alisaes
\*********************************************************************************************/
#ifdef BLE_ESP32_ALIASES
int addAlias( uint8_t *addr, char *name){
  if (!addr || !name){
    return 0;
  }

  int count = aliases.size();
  // replace name for existing address
  for (int i = 0; i < count; i++){
    if (!memcmp(aliases[i]->addr, addr, 6)){
      strncpy(aliases[i]->name, name, sizeof(aliases[i]->name));
      aliases[i]->name[sizeof(aliases[i]->name)-1] = 0;
      return 2;
    }
  }

  // replace addr for existing name
  for (int i = 0; i < count; i++){
    if (!strcmp(aliases[i]->name, name)){
      memcpy(aliases[i]->addr, addr, 7);
      return 2;
    }
  }

  BLE_ESP32::ble_alias_t *alias = new BLE_ESP32::ble_alias_t;
  memcpy(alias->addr, addr, 7);
  strncpy(alias->name, name, sizeof(alias->name));
  alias->name[sizeof(alias->name)-1] = 0;
  aliases.push_back(alias);
  return 1;
}
#endif

/**
 * @brief Remove all colons from null terminated char array
 *
 * @param _string Typically representing a MAC-address like AA:BB:CC:DD:EE:FF
 */
void stripColon(char* _string){
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


//////////////////////////////////////////////////
// use this for address interpretaton from string
// it looks for aliases, and converts AABBCCDDEEFF and AA:BB:CC:DD:EE:FF
// it also accepts AABBCCDDEEFF/1 to indicate random
int getAddr(uint8_t *dest, char *src){
  if (!dest || !src){
    return 0;
  }
#ifdef BLE_ESP32_ALIASES
  for (int i = 0; i < aliases.size(); i++){
    if (!strcmp(aliases[i]->name, src)){
      memcpy(dest, aliases[i]->addr, 7);
      return 2; //was an alias
    }
  }
#endif

  char tmp[12+5+1+2];
  int srclen = strlen(src);
  if ((srclen == 12+5) || (srclen == 12+5+2)){
    strcpy(tmp, src);
    stripColon(tmp);
    src = tmp;
  }

  int len = fromMAC(dest, src, 7);
  if (len == 7){
    return 1;
  }
  // not found
  return 0;
}

static const char *noAlias = PSTR("");

////////////////////////////////////////////
// use to display the alias name if required
const char *getAlias(const uint8_t *addr){
  if (!addr){
    return noAlias;
  }
#ifdef BLE_ESP32_ALIASES
  for (int i = 0; i < aliases.size(); i++){
    if (!memcmp(aliases[i]->addr, addr, 6)){
      return aliases[i]->name; //was an alias
    }
  }
#endif
  return noAlias;
}


/*********************************************************************************************\
 * Highest level BLE task control functions
\*********************************************************************************************/

static int StartBLE(void) {
  if (BLEStop != 1){
    BLE_ESP32::BLEStartOperationTask();
    return 1;
  }
  AddLog(LOG_LEVEL_ERROR,PSTR("BLE: StartBLE - wait as BLEStop==1"));

  return 0;
}

static int StopBLE(void){
  if (BLERunning){
    if (BLEStop != 1){
      BLEStop = 1;
      AddLog(LOG_LEVEL_INFO,PSTR("BLE: StopBLE - BLEStop->1"));
      BLEStopAt = esp_timer_get_time();
      // give a little time for it to stop.
      vTaskDelay(100/ portTICK_PERIOD_MS);
      return 1;
    }
    AddLog(LOG_LEVEL_ERROR,PSTR("BLE: StopBLE - wait as BLEStop==1"));
    return 0;
  } else {
    AddLog(LOG_LEVEL_ERROR,PSTR("BLE: StopBLE - was not running"));
    return 1;
  }
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

  ResponseCmndNumber(BLEAdvertMode);
}


//////////////////////////////////////////////////////////////
// Determine what to do with advertisements
// BLEAdv0 -> suppress MQTT about devices found
// BLEAdv1 -> send MQTT about devices found after each scan
void CmndBLEDebug(void){
  BLEDebugMode = XdrvMailbox.index;
  ResponseCmndNumber(BLEDebugMode);
}

void CmndBLEDevices(void){
  switch(XdrvMailbox.index){
    case 0:{
      // clear devices delete
      deleteSeenDevices();
    } break;
    case 1:{
      BLEPublishDevices = 2; // mqtt publish as 'STAT'
    } break;
  }
  ResponseCmndDone();
}

void CmndBLEMaxAge(void){
  switch(XdrvMailbox.index){
    case 1:{
      if (XdrvMailbox.data_len > 0) {
        BLEMaxAge = XdrvMailbox.payload;
      }
    } break;
  }
  ResponseCmndIdxNumber(BLEMaxAge);
  if (BLEMaxAge) deleteSeenDevices(BLEMaxAge);
}

void CmndBLEAddrFilter(void){
  switch(XdrvMailbox.index){
    case 1:{
      if (XdrvMailbox.data_len > 0) {
        BLEAddressFilter = XdrvMailbox.payload;
      }
    } break;
  }
  ResponseCmndIdxNumber(BLEAddressFilter);
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
    case 0:{
      if (XdrvMailbox.data_len > 0) {
        BLEScanActiveMode = XdrvMailbox.payload;
        ResponseCmndNumber(BLEScanActiveMode);
      } else {
        ResponseCmndChar("Invalid");
      }
    } break;

    case 1: // do a manual scan now
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
  int val = XdrvMailbox.index;
  if (XdrvMailbox.data_len > 0) {
    val = XdrvMailbox.payload;
  }

  switch(val){
    case BLEModeDisabled:{
      if (BLEMode != BLEModeDisabled){
        BLEMode = BLEModeDisabled;
        StopBLE();
        ResponseCmndChar("StoppingBLE");
      } else {
        ResponseCmndChar("Disabled");
      }
    } break;
    case BLEModeScanByCommand:{
      uint64_t now = esp_timer_get_time();
      switch(BLEMode){
        // when changing from regular to by command,
        // stop the scan next loop
        case BLEModeRegularScan: {
          BLEMode = BLEModeScanByCommand;
          BLEStopScan = 1;
          ResponseCmndChar("BLEStopScan");
        } break;
        case BLEModeDisabled: {
          BLEMode = BLEModeScanByCommand;
          StartBLE();
          ResponseCmndChar("StartingBLE");
        } break;
        case BLEModeScanByCommand:{
          ResponseCmndChar("BLERunning");
        } break;
      }
      BLEScanLastAdvertismentAt = now; // note the time of the last advertisment
    } break;
    case BLEModeRegularScan:{
      uint64_t now = esp_timer_get_time();
      switch(BLEMode){
        case BLEModeDisabled: {
          BLEMode = BLEModeRegularScan;
          StartBLE();
          ResponseCmndChar("StartingBLE");
        } break;
        case BLEModeScanByCommand:{
          BLEMode = BLEModeRegularScan;
          ResponseCmndChar("BLEEnableScan");
        } break;
        case BLEModeRegularScan:{
          BLEMode = BLEModeRegularScan;
          ResponseCmndChar("BLERunning");
        } break;
      }
      BLEScanLastAdvertismentAt = now; // note the time of the last advertisment
    } break;
    default:
      ResponseCmndChar("InvalidIndex");
      break;
  }
}

//////////////////////////////////////////////////////////////
// Enables BLE even if master enable is unset
// use to temporarily enable after boot - e.g. at the end of autoexec
void CmndBLEEnableUnsaved(void){
  int val = -1;
  if (XdrvMailbox.data_len > 0) {
    val = XdrvMailbox.payload;
  }

  if (val >= 0){
    BLEEnableUnsaved = val;
  }
  ResponseCmndNumber(BLEEnableUnsaved);
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
      ResponseCmndNumber(BLEDetailsRequest);
      break;

    case 1:
    case 2:{
      BLEDetailsRequest = 0;
      if (getAddr(BLEDetailsMac, XdrvMailbox.data)){
        BLEDetailsRequest = XdrvMailbox.index;
        ResponseCmndIdxChar(XdrvMailbox.data);
      } else {
        ResponseCmndChar("InvalidMac");
      }
    } break;

    case 3:{
      BLEDetailsRequest = XdrvMailbox.index;
      ResponseCmndNumber(BLEDetailsRequest);
    } break;

    case 4:{
      BLEDetailsRequest = XdrvMailbox.index;
      ResponseCmndNumber(BLEDetailsRequest);
    } break;

    default:
      ResponseCmndChar("InvalidIndex");
      break;
  }
}


void CmndBLEAlias(void){
#ifdef BLE_ESP32_ALIASES
  int op = XdrvMailbox.index;
  if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: Alias %d %s"), op, XdrvMailbox.data);

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
        int len = fromMAC(addr, p, sizeof(addr));
        if (len != 7){
          AddLog(LOG_LEVEL_ERROR,PSTR("BLE: Alias invalid mac %s"), p);
          ResponseCmndChar("invalidmac");
          return;
        }

        p = strtok(nullptr, " ,=");
        char *name = p;
        if (!p || !(*p)){
          int i = 0;
          for (i = 0; i < aliases.size(); i++){
            BLE_ESP32::ble_alias_t *alias = aliases[i];
            if (!memcmp(alias->addr, addr, 6)){
              aliases.erase(aliases.begin() + i);
              BLEAliasListResp();
              return;
            }
          }
          ResponseCmndChar("invalidmac");
          return;
        }

        AddLog(LOG_LEVEL_ERROR,PSTR("BLE: Add Alias mac %s = name %s"), mac, p);
        if (addAlias( addr, name )){
          added++;
        }
        p = strtok(nullptr, " ,=");
      } while (p);

      if (added){
        if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: Added %d Aliases"), added);
        BLEAliasListResp();
      } else {
        BLEAliasListResp();
      }
      return;
    } break;
    case 2:{ // clear
      if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: Alias clearing %d"), aliases.size());
      for (int i = aliases.size()-1; i >= 0; i--){
        BLE_ESP32::ble_alias_t *alias = aliases[i];
        aliases.pop_back();
        delete alias;
      }
      BLEAliasListResp();
      return;
    } break;
  }
  ResponseCmndChar("invalididx");
#endif
}


// SET the BLE name for a device -
// uses s:1800 c:2a00 and writes name to DEVICE
void CmndBLEName(void) {
  char *p = strtok(XdrvMailbox.data, " ");

  if (!p || !(*p)){
    ResponseCmndIdxChar(PSTR("invalid"));
    return;
  }

  uint8_t addrbin[7];
  int addrres = BLE_ESP32::getAddr(addrbin, p);
  NimBLEAddress addr(addrbin, addrbin[6]);

  if (addrres){
    if (addrres == 2){
      if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: addr used alias: %s"), p);
    }

//#ifdef EQ3_DEBUG
    if (BLEDebugMode > 0) AddLog(LOG_LEVEL_INFO,PSTR("BLE: cmd addr: %s -> %s"), p, addr.toString().c_str());
//#endif
  } else {
    AddLog(LOG_LEVEL_ERROR,PSTR("BLE: addr invalid: %s"), p);
    ResponseCmndIdxChar(PSTR("invalidaddr"));
    return;
  }

  BLE_ESP32::generic_sensor_t *op = nullptr;
  // ALWAYS use this function to create a new one.
  int res = BLE_ESP32::newOperation(&op);
  if (!res){
    AddLog(LOG_LEVEL_ERROR,PSTR("BLE: Can't get a newOperation"));
    ResponseCmndChar(PSTR("FAIL"));
    return;
  } else {
    if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: got a newOperation from BLE"));
  }

  op->addr = addr;
  op->serviceUUID = NimBLEUUID("1800");
  op->characteristicUUID = NimBLEUUID("2A00");

  // get next part of cmd
  char *name = strtok(nullptr, " ");
  bool write = false;
  if (name && *name){
    if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: write name %s"), name);
    op->writelen = strlen(name);
    memcpy(op->dataToWrite, name, op->writelen);
    write = true;
  } else {
    if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: read name"));
    op->readlen = 1;
  }

  res = BLE_ESP32::extQueueOperation(&op);
  if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: queue res %d"), res);
  if (!res){
    // if it fails to add to the queue, do please delete it
    BLE_ESP32::freeOperation(&op);
    AddLog(LOG_LEVEL_ERROR,PSTR("BLE: Failed to queue new operation - deleted"));
    ResponseCmndChar(PSTR("QUEUEFAIL"));
    return;
  }

  if (write){
    if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG, PSTR("BLE: will write name"));
  } else {
    if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG, PSTR("BLE: will read name"));
  }
  ResponseCmndDone();
  return;
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

  //AddLog(LOG_LEVEL_INFO,PSTR("BLE: op %d"), op);

  int res = -1;

  // if in progress, only op 0 or 11 are allowed
  switch(op) {
    case 0:
#ifdef BLE_ESP32_DEBUG
      if (BLEDebugMode > 0) AddLog(LOG_LEVEL_INFO,PSTR("BLE: preview"));
#endif
      BLEPostMQTTTrigger = 1;
      break;
    case 1: {
      if (prepOperation){
        BLE_ESP32::freeOperation(&prepOperation);
      }
      int opres = BLE_ESP32::newOperation(&prepOperation);
      if (!opres){
#ifdef BLE_ESP32_DEBUG
        AddLog(LOG_LEVEL_ERROR,PSTR("BLE: Could not create new operation"));
#endif
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
            uint8_t addr[7];
            if (getAddr(addr, p+2)){
              prepOperation->addr = NimBLEAddress(addr, addr[6]);
            } else {
              prepOperation->addr = NimBLEAddress();
            }
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
#ifdef BLE_ESP32_DEBUG
          AddLog(LOG_LEVEL_ERROR,PSTR("BLE: Could not queue new operation"));
#endif
          ResponseCmndChar("FailQueue");
          return;
        } else {
          // NOTE: prepOperation has been set to null if we queued sucessfully.
#ifdef BLE_ESP32_DEBUG
          if (BLEDebugMode > 0) AddLog(LOG_LEVEL_INFO,PSTR("BLE: Operations queued:%d"), queuedOperations.size());
#endif
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
#ifdef BLE_ESP32_DEBUG
        AddLog(LOG_LEVEL_ERROR,PSTR("BLE: Could not queue new operation"));
#endif
        ResponseCmndChar("FailQueue");
      } else {
        // NOTE: prepOperation has been set to null if we queued sucessfully.
#ifdef BLE_ESP32_DEBUG
        if (BLEDebugMode > 0) AddLog(LOG_LEVEL_INFO,PSTR("BLE: Operations queued:%d"), queuedOperations.size());
#endif
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
static void BLEPostMQTTSeenDevices(int type) {
  int remains = 0;
  nextSeenDev = 0;

  int maxlen = 1024;
  char dest[maxlen];
  do {
    remains = getSeenDevicesToJson(dest, maxlen);
    ResponseTime_P(dest);
    // no retain - this is present devices, not historic
    MqttPublishPrefixTopicRulesProcess_P((1 == type) ? TELE : STAT, PSTR("BLE"));
  } while (remains);
}

static void BLEPostMQTT(bool onlycompleted) {
//  if (TasmotaGlobal.ota_state_flag) return;


  if (prepOperation || mqttOperations.size() || queuedOperations.size() || currentOperations.size()){
#ifdef BLE_ESP32_DEBUG
    if (BLEDebugMode > 0) AddLog(LOG_LEVEL_INFO,PSTR("BLE: some to show"));
#endif
    if (prepOperation && !onlycompleted){
      std::string out = BLETriggerResponse(prepOperation);
      Response_P(PSTR("%s"), out.c_str());
      MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR("BLE"), Settings->flag.mqtt_sensor_retain);
#ifdef BLE_ESP32_DEBUG
      if (BLEDebugMode > 0) AddLog(LOG_LEVEL_INFO,PSTR("BLE: prep sent %s"), out.c_str());
#endif
    }

    if (queuedOperations.size() && !onlycompleted){
#ifdef BLE_ESP32_DEBUG
      if (BLEDebugMode > 0) AddLog(LOG_LEVEL_INFO,PSTR("BLE: queued %d"), queuedOperations.size());
#endif
      for (int i = 0; i < queuedOperations.size(); i++){
        TasAutoMutex localmutex(&BLEOperationsRecursiveMutex, "BLEPost1");

        generic_sensor_t *toSend = queuedOperations[i];
        if (!toSend) {
          break;
        } else {
          std::string out = BLETriggerResponse(toSend);
          localmutex.give();
          Response_P(PSTR("%s"), out.c_str());
          MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR("BLE"), Settings->flag.mqtt_sensor_retain);
#ifdef BLE_ESP32_DEBUG
          if (BLEDebugMode > 0) AddLog(LOG_LEVEL_INFO,PSTR("BLE: queued %d sent %s"), i, out.c_str());
#endif
          //break;
        }
      }
    }

    if (currentOperations.size() && !onlycompleted){
#ifdef BLE_ESP32_DEBUG
      if (BLEDebugMode > 0) AddLog(LOG_LEVEL_INFO,PSTR("BLE: current %d"), currentOperations.size());
#endif
      for (int i = 0; i < currentOperations.size(); i++){
        TasAutoMutex localmutex(&BLEOperationsRecursiveMutex, "BLEPost2");
        generic_sensor_t *toSend = currentOperations[i];
        if (!toSend) {
          break;
        } else {
          std::string out = BLETriggerResponse(toSend);
          localmutex.give();
          Response_P(PSTR("%s"), out.c_str());
          MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR("BLE"), Settings->flag.mqtt_sensor_retain);
#ifdef BLE_ESP32_DEBUG
          if (BLEDebugMode > 0) AddLog(LOG_LEVEL_INFO,PSTR("BLE: curr %d sent %s"), i, out.c_str());
#endif
          //break;
        }
      }
    }

    if (mqttOperations.size()){
#ifdef BLE_ESP32_DEBUG
      if (BLEDebugMode > 0) AddLog(LOG_LEVEL_INFO,PSTR("BLE: completed %d"), mqttOperations.size());
#endif
      do {
        generic_sensor_t *toSend = nextOperation(&mqttOperations);
        if (!toSend) {
          break; // break from while loop
        } else {
#ifdef BLE_ESP32_DEBUG
          if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: mqttOperation removed opid %d"), toSend->opid);
#endif
          std::string out = BLETriggerResponse(toSend);
          Response_P(PSTR("%s"), out.c_str());
          MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR("BLE"), Settings->flag.mqtt_sensor_retain);
          // we alreayd removed this from the queues, so now delete
          delete toSend;
          //break;
        }
        //break;
      } while (1);
    }
  } else {
    Response_P(PSTR("{\"BLEOperation\":{}}"));
    MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR("BLE"), Settings->flag.mqtt_sensor_retain);
  }
}

static void mainThreadBLETimeouts() {
  uint64_t now = esp_timer_get_time();

  if (!BLERunning){
    BLELastLoopTime = now; // initialise the time of the last advertisment
    BLEScanLastAdvertismentAt = now; // initialise the time of the last advertisment
    return;
  }

  if (BLEStop == 1){
    if (BLEStopAt + 30L*1000L*1000L < now){ // if asked to stop > 30s ago...
      AddLog(LOG_LEVEL_ERROR,PSTR("BLE: Stop Timeout - restart Tasmota"));
      BLERestartTasmota = 2;
      BLEStopAt = now;
    }
    AddLog(LOG_LEVEL_ERROR,PSTR("BLE: Awaiting BLEStop"));
    return;
  }

  // if no adverts for 120s, and BLE is running, retsart NimBLE.
  // belt and braces....
  uint64_t adTimeout = ((uint64_t)BLEMaxTimeBetweenAdverts)*1000L*1000L;
  if (BLEScanLastAdvertismentAt + adTimeout < now){
    BLEScanLastAdvertismentAt = now; // initialise the time of the last advertisment
    BLERestartNimBLE = 1;
    AddLog(LOG_LEVEL_ERROR,PSTR("BLE: scan stall? no adverts > 120s, restart BLE"));

    BLERestartBLEReason = BLE_RESTART_BLE_REASON_ADVERT_BLE_TIMEOUT;
  }

  // if stuck and have not done task for 120s, something is seriously wrong.
  // restart Tasmota completely. (belt and braces)
  uint64_t bleLoopTimeout = ((uint64_t)BLEMaxTaskLoopTime)*1000L*1000L;
  if (BLELastLoopTime + bleLoopTimeout < now){
    BLELastLoopTime = now; // initialise the time of the last advertisment
    BLERestartTasmota = 10;
    AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: BLETask stall > 120s, restart Tasmota in 10s"));
    BLERestartTasmotaReason = BLE_RESTART_TEAMOTA_REASON_BLE_LOOP_STALLED;
  }
}


static void mainThreadOpCallbacks() {
  if (completedOperations.size()){
    //AddLog(LOG_LEVEL_INFO,PSTR("BLE: completed %d"), completedOperations.size());
    TasAutoMutex localmutex(&BLEOperationsRecursiveMutex, "BLEMainCB");

    // find this operation in currentOperations, and remove it.
    // in reverse so we can erase them safely.
    for (int i = completedOperations.size()-1; i >= 0 ; i--){
      generic_sensor_t *op = completedOperations[i];

      bool callbackres = false;

      if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: op->completecallback is %u opid %d"), op->completecallback, op->opid);
      if (op->completecallback){
        OPCOMPLETE_CALLBACK *pFn = (OPCOMPLETE_CALLBACK *)(op->completecallback);
        callbackres = pFn(op);
#ifdef BLE_ESP32_DEBUG
        if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: op->completecallback %d opid %d"), callbackres, op->opid);
#endif
      }

      if (!callbackres){
        for (int i = 0; i < operationsCallbacks.size(); i++){
          if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: operationsCallbacks %d is %u"), i, operationsCallbacks[i]);
          OPCOMPLETE_CALLBACK *pFn = operationsCallbacks[i];
          callbackres = pFn(op);
#ifdef BLE_ESP32_DEBUG
          if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: operationsCallbacks %d %d"), i, callbackres);
#endif
          if (callbackres){
            break; // this callback ate the op.
          }
        }
      }

      // always remove from here
      completedOperations.erase(completedOperations.begin() + i);
      // unless some callback told us not to send on MQTT, then remove from completed and
      // add to mqtt list
      if (!callbackres){
        addOperation(&mqttOperations, &op);
      } else {
#ifdef BLE_ESP32_DEBUG
        if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: callbackres true -> delete op opid %d"), op->opid);
#endif
        delete op;
      }
    }
  }
}

static void BLEShowStats(){
  uint32_t totalCount = BLEAdvertisment.totalCount;
  uint32_t deviceCount = seenDevices.size();
  ResponseTime_P(PSTR(",\"BLE\":{\"scans\":%u,\"adverts\":%u,\"devices\":%u,\"resets\":%u}}"), BLEScanCount, totalCount, deviceCount, BLEResets);
  MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR("BLE"), 0);
}


/*void BLEAliasMqttList(){
  ResponseTime_P(PSTR(",\"BLEAlias\":["));
  for (int i = 0; i < aliases.size(); i++){
    if (i){
      ResponseAppend_P(PSTR(","));
    }
    char tmp[20];
    ToHex_P(aliases[i]->addr,6,tmp,20,0);
    ResponseAppend_P(PSTR("{\"%s\":\"%s\"}"), tmp, aliases[i]->name);
  }
  ResponseAppend_P(PSTR("]}"));
  MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR("BLE"), Settings->flag.mqtt_sensor_retain);
}*/

void BLEAliasListResp(){
  Response_P(PSTR("{\"BLEAlias\":{"));
  for (int i = 0; i < aliases.size(); i++){
    if (i){
      ResponseAppend_P(PSTR(","));
    }
    char tmp[20];
    ToHex_P(aliases[i]->addr,6,tmp,20,0);
    ResponseAppend_P(PSTR("\"%s\":\"%s\""), tmp, aliases[i]->name);
  }
  ResponseAppend_P(PSTR("}}"));
}


static void BLEDiag()
{
  uint32_t totalCount = BLEAdvertisment.totalCount;
  uint32_t deviceCount = seenDevices.size();
#ifdef BLE_ESP32_DEBUG
  if (BLEDebugMode > 0) AddLog(LOG_LEVEL_INFO,PSTR("BLE: scans:%u,advertisements:%u,devices:%u,resets:%u,BLEStop:%d,BLERunning:%d,BLERunningScan:%d,BLELoopCount:%u,BLEOpCount:%u"), BLEScanCount, totalCount, deviceCount, BLEResets, BLEStop, BLERunning, BLERunningScan, BLELoopCount, BLEOpCount);
#endif
}

/**
 * @brief creates a JSON representing a single operation.
 *
 */
std::string BLETriggerResponse(generic_sensor_t *toSend){
  char temp[100];
  if (!toSend) return "";
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
  "<tr><th><label>mac(type)</label></th><th><label>alias</label></th><th><label>name</label></th><th><label>RSSI</label></th><th><label>Age(max)</label></th></tr>";
const char HTTP_BLE_DEV[] PROGMEM =
  "<tr><td><label>%s(%d)</label></td><td><label>%s</label></td><td><label>%s</label></td><td><label>%d</label></td><td><label>%d(%d)</label></td></tr>";
const char HTTP_BLE_DEV_END[] PROGMEM =
  "</table></fieldset>";

void HandleBleConfiguration(void)
{

#ifdef BLE_ESP32_DEBUG
  AddLog(LOG_LEVEL_DEBUG, PSTR("BLE: HandleBleConfiguration"));
#endif

  if (!HttpCheckPriviledgedAccess()) {
#ifdef BLE_ESP32_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR("BLE: !HttpCheckPriviledgedAccess()"));
#endif
    return;
  }

#ifdef BLE_ESP32_DEBUG
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_CONFIGURE_BLE));
#endif

  char tmp[20];
  WebGetArg("en", tmp, sizeof(tmp));

#ifdef BLE_ESP32_DEBUG
  if (BLEDebugMode > 0) AddLog(LOG_LEVEL_DEBUG, PSTR("BLE: arg en is %s"), tmp);
#endif

  if (Webserver->hasArg("save")) {
#ifdef BLE_ESP32_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR("BLE: SETTINGS SAVE"));
#endif
    Settings->flag5.mi32_enable = Webserver->hasArg("e0");  //
    BLEScanActiveMode = (Webserver->hasArg("e1")?1:0);  //

    SettingsSaveAll();
    HandleConfiguration();
    return;
  }
#ifdef BLE_ESP32_DEBUG
  AddLog(LOG_LEVEL_DEBUG, PSTR("BLE: !SAVE"));
#endif
  char str[TOPSZ];

  WSContentStart_P(PSTR(D_CONFIGURE_BLE));
  WSContentSendStyle_P(HTTP_BLE_DEV_STYLE);
  //WSContentSendStyle();
  WSContentSend_P(HTTP_FORM_BLE,
    (Settings->flag5.mi32_enable) ? " checked" : "",
    (BLEScanActiveMode) ? " checked" : ""
    );
  WSContentSend_P(HTTP_FORM_END);


  {
    //TasAutoMutex localmutex(&BLEOperationsRecursiveMutex, "BLEConf");
    int number = seenDevices.size();
    if (number){
      WSContentSend_P(HTTP_BLE_DEV_START);
      uint64_t now = esp_timer_get_time();
      now = now/1000L;
      now = now/1000L;
      uint32_t nowS = (uint32_t)now;

      for (int i = 0; i < number; i++){
        BLE_ESP32::BLE_simple_device_t* dev = seenDevices[i];
        char addr[20];
        dump(addr, 20, dev->mac, 6);
        uint8_t addrtype = dev->addrtype;
        const char *alias = getAlias(dev->mac);
        uint64_t lastseen = dev->lastseen/1000L;
        lastseen = lastseen/1000L;
        uint32_t lastseenS = (uint32_t) lastseen;
        uint32_t ageS = nowS-lastseenS;

        WSContentSend_P(HTTP_BLE_DEV, addr, addrtype, alias, dev->name, dev->RSSI, ageS, dev->maxAge);
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

int ExtStopBLE(){
  AddLog(LOG_LEVEL_INFO, PSTR("BLE: Stopping if active"));
  BLE_ESP32::BLEEnableMask = 0;
  BLE_ESP32::stopStartBLE();
  return 0;
}

int ExtRestartBLEIfEnabled(){
  AddLog(LOG_LEVEL_INFO, PSTR("BLE: Starting if active"));
  BLE_ESP32::BLEEnableMask = 1;
  BLE_ESP32::stopStartBLE();
  return 0;
}

bool Xdrv79(uint32_t function)
{
  //if (!Settings->flag5.mi32_enable) { return false; }  // SetOption115 - Enable ESP32 BLE BLE

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
      //############################# DEBUG
      TasmotaGlobal.seriallog_timer = 0;
      break;
    case FUNC_EVERY_SECOND:
      BLE_ESP32::BLEEverySecond(false);
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(BLE_ESP32::kBLE_Commands, BLE_ESP32::BLE_Commands);
      break;
    case FUNC_JSON_APPEND:
      break;

    // next second, we will publish to our MQTT topic.
    case FUNC_AFTER_TELEPERIOD:
      BLE_ESP32::BLEPublishDevices = 1;  // mqtt publish as 'TELE'
      break;

    case FUNC_INTERRUPT_STOP:
      ExtStopBLE();
      break;
/*
    case FUNC_INTERRUPT_START:
      break;
*/
#ifdef USE_WEBSERVER
    case FUNC_WEB_ADD_BUTTON:
      WSContentSend_P(BLE_ESP32::HTTP_BTN_MENU_BLE);
      break;
    case FUNC_WEB_ADD_HANDLER:
      WebServer_on(PSTR("/" WEB_HANDLE_BLE), BLE_ESP32::HandleBleConfiguration);
      break;
#endif  // USE_WEBSERVER
    case FUNC_ACTIVE:
      result = true;
      break;
    }
  return result;
}



#ifdef BLE_ESP32_EXAMPLES


/*********************************************************************************************\
 * Example Advertisment callback
\*********************************************************************************************/

#ifdef EXAMPLE_ADVERTISMENT_CALLBACK

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
  AddLog(LOG_LEVEL_INFO,PSTR("BLE: myOpCallback"));
  return 0; // return true to block MQTT broadcast
}

// this one is used to demonstrate processing of ONE specific operation
int myOpCallback2(BLE_ESP32::generic_sensor_t *pStruct){
  AddLog(LOG_LEVEL_INFO,PSTR("BLE: myOpCallback2"));
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
    AddLog(LOG_LEVEL_ERROR,PSTR("BLE: Could not create new operation"));
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
    AddLog(LOG_LEVEL_ERROR,PSTR("BLE: Failed to queue new operation - deleted"));
    return;
  }

#endif
}
// end #ifdef BLE_ESP32_EXAMPLES
#endif


#endif
#endif  // CONFIG_IDF_TARGET_ESP32 or CONFIG_IDF_TARGET_ESP32C3
#endif  // ESP32
