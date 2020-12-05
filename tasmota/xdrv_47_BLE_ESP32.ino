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
0x100 + -> failure (see GEN_STATE_FAILED_XXXX constants below.)


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
#define VSCODE_DEV

#ifdef VSCODE_DEV
#define ESP32
#define USE_BLE_ESP32
#endif

#ifdef ESP32                       // ESP32 only. Use define USE_HM10 for ESP8266 support

#ifdef USE_BLE_ESP32

#define XDRV_47                    47
#define USE_MI_DECRYPTION

#include "xdrv_47_BLE_ESP32.h"
#include <vector>
#include <deque>
#include <string.h>
#include <cstdarg>
#ifdef USE_MI_DECRYPTION
#include <t_bearssl.h>
#endif //USE_MI_DECRYPTION



void installExamples();
void sendExample();


namespace BLE_ESP32 {

struct BLE_simple_device_t {
  uint8_t mac[6];
  char name[20];
  uint64_t lastseen;
  uint8_t getAdvert;
};





// this protects our queues, which can be accessed by multiple tasks 
SemaphoreHandle_t  BLEOperationsRecursiveMutex;



// only run from main thread, becaus eit deletes things that were newed there...
static void mainThreadOpCallbacks();
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
static void runCurrentOperation(BLE_ESP32::generic_sensor_t** pCurrentOperation, NimBLEClient *pClient);
static void runTaskDoneOperation(BLE_ESP32::generic_sensor_t** op, NimBLEClient *pClient);

// called from advert callback
void setDetails(ble_advertisment_t *ad);

#define EXAMPLE_ADVERTISMENT_CALLBACK
#define EXAMPLE_OPERATION_CALLBACK

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

static int BLEInitState = 0;
static int BLERunningScan = 0;
static int BLEPublishDevices = 0; // causes MQTT publish of device list (each scan end)
static BLEScan* BLEScan = nullptr;
// time we last started a scan in uS using esp_timer_get_time();
// used to setect a scan which did not call back?
uint64_t BLEScanStartetdAt = 0; 
int BLEScanTimeS = 20; // scan duraiton in S
int BLEMaxTimeBetweenAdverts = 40; // we expect an advert at least this frequently, else restart BLE (in S)
uint64_t BLEScanLastAdvertismentAt = 0;
uint32_t lastopid = 0; // incrementing uinique opid

// controls request of details about one device
uint8_t BLEDetailsRequest = 0;
uint8_t BLEDetailsMac[6];


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
  "Period|Adv|Op|Mode|Details";

static void CmndBLEPeriod(void);
static void CmndBLEAdv(void);
static void CmndBLEOperation(void);
static void CmndBLEMode(void);
static void CmndBLEDetails(void);

void (*const BLE_Commands[])(void) PROGMEM = {
  &BLE_ESP32::CmndBLEPeriod, &BLE_ESP32::CmndBLEAdv, &BLE_ESP32::CmndBLEOperation, &BLE_ESP32::CmndBLEMode, &BLE_ESP32::CmndBLEDetails };

/*********************************************************************************************\
 * enumerations
\*********************************************************************************************/

enum BLE_Commands {          // commands useable in console or rules
  CMND_BLE_PERIOD,           // set period like TELE-period in seconds between read-cycles
  CMND_BLE_ADV,             // change advertisment options at runtime
  CMND_BLE_OP,                // connect/read/write/notify operations
  CMND_BLE_MODE              // change mode of ble - BLE_MODES 
  };

enum {
  BLEModeDisabled = 0, // BLE is disabled
  BLEModeByCommand = 1, // BLE is activeated by commands only
  BLEModeActive = 2, // BLE is scanning all the time 
} BLE_MODES;

// values of BLEAdvertMode
enum {
  BLE_NO_ADV_SEND = 0, // driver is silent on MQTT regarding adverts
  BLE_ADV_TELE = 1, // driver sends a summary at tele period
  //BLE_ADV_ALL = 2,  // driver sends every advert with full data to MQTT
} BLEADVERTMODE;


uint8_t BLEMode = BLEModeActive;
uint8_t BLENextMode = BLEModeActive;
uint8_t BLEAdvertMode = BLE_ADV_TELE;
uint8_t BLEdeviceLimitReached = 0;

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

int addSeenDevice(const uint8_t *mac, const char *name){
  int res = 0;
  uint64_t now = esp_timer_get_time();
  xSemaphoreTakeRecursive(BLEOperationsRecursiveMutex, portMAX_DELAY);
  int devicefound = 0;
  // do we already know this device?
  for (int i = 0; i < seenDevices.size(); i++){
    if (!memcmp(seenDevices[i]->mac, mac, 6)){
      seenDevices[i]->lastseen = now; 
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
      seenDevices.push_back(dev);
      res = 2; // added
    }
  } else {
    res = 1; // already there
  }
  xSemaphoreGiveRecursive(BLEOperationsRecursiveMutex); // release mutex
  return res;
}

// remove devices from the seen list by age, and add them to the free list
// set ageS to 0 to delete all...
int deleteSeenDevices(int ageS = 0){
  int res = 0;
  uint64_t now = esp_timer_get_time();
  uint64_t mintime = now - (ageS*1000*1000);
  xSemaphoreTakeRecursive(BLEOperationsRecursiveMutex, portMAX_DELAY);
  for (int i = seenDevices.size()-1; i >= 0; i--){
      BLE_ESP32::BLE_simple_device_t* dev = seenDevices[i];
      if (dev->lastseen < mintime){
        BLE_ESP32::SafeAddLog_P(LOG_LEVEL_INFO,PSTR("delete device by age"));
        seenDevices.erase(seenDevices.begin()+i);
        freeDevices.push_back(dev);
        res++;
      }
  }
  xSemaphoreGiveRecursive(BLEOperationsRecursiveMutex); // release mutex
  return res;
}

int deleteSeenDevice(uint8_t *mac){
  int res = 0;
  xSemaphoreTakeRecursive(BLEOperationsRecursiveMutex, portMAX_DELAY);
  for (int i = 0; i < seenDevices.size(); i++){
    if (!memcmp(seenDevices[i]->mac, mac, 6)){
      BLE_ESP32::BLE_simple_device_t* dev = seenDevices[i];
      seenDevices.erase(seenDevices.begin()+i);
      freeDevices.push_back(dev);
      res = 1;
      break;
    }
  }
  xSemaphoreGiveRecursive(BLEOperationsRecursiveMutex); // release mutex
  return res;
}



// the MAX we could expect.
#define MAX_DEV_JSON_NAME_LEN 20
// "001122334455":{"n":"01234567890123456789"}\0 == 43
#define MIN_REQUIRED_DEVJSON_LEN (1+12+1 + 1 + 1+1+1+1+1 + MAX_DEV_JSON_NAME_LEN + 2 + 2)
int getSeenDeviceToJson(BLE_ESP32::BLE_simple_device_t* dev, char **dest, int *maxlen){
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
  *((*dest)++) = 'n';
  *((*dest)++) = '"';
  *((*dest)++) = ':';
  *((*dest)++) = '"';
  strncpy((*dest), dev->name, MAX_DEV_JSON_NAME_LEN);
  (*dest) += strlen((*dest));
  *((*dest)++) = '"';
  *((*dest)++) = '}';
  *maxlen -= (*dest - p);
  return 1;
}


int nextSeenDev = 0;

int getSeenDevicesToJson(char *dest, int maxlen){
  int res = 0;

  if ((nextSeenDev == 0) || (nextSeenDev >= seenDevices.size())){
    // delete devices not seen in last 240s
    //deleteSeenDevices(240);
    nextSeenDev = 0;
  }

  // deliberate test of SafeAddLog_P from main thread...
  BLE_ESP32::SafeAddLog_P(LOG_LEVEL_INFO,PSTR("getSeen %d"), seenDevices.size());


  if (!maxlen) return 0;
  *(dest++) = '{';
  maxlen--;
  int added = 0;
  xSemaphoreTakeRecursive(BLEOperationsRecursiveMutex, portMAX_DELAY);
  for (int i = nextSeenDev; i < seenDevices.size(); i++){
    if (maxlen > MIN_REQUIRED_DEVJSON_LEN - 3){
      if (added){
        *(dest++) = ',';
        maxlen--;
      }
      res += getSeenDeviceToJson(seenDevices[i], &dest, &maxlen);
      if (res) {
        added++;
      } else {
        if (added){
          dest--;
          maxlen++;
        }
      }
    }
  }
  xSemaphoreGiveRecursive(BLEOperationsRecursiveMutex); // release mutex
  *(dest++) = '}';
  *(dest++) = 0;
  return added;
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

  if (thistask == TasmotaMainTask){
    // safelogp called from main thread?
    va_list arg;
    va_start(arg, formatP);
    AddLog_P(loglevel, formatP, arg);
    va_end(arg);
    return 1;
  }
  
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
      // assume this is thread safe - it may not be
      va_list arg;
      va_start(arg, formatP);
      vsnprintf_P(logdata->log_data, sizeof(logdata->log_data) - 5, formatP, arg);
      va_end(arg);
      snprintf(logdata->log_data + strlen(logdata->log_data), 5, " %d", freelogs.size());
      logdata->level = loglevel;
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
char * dump(char *dest, int maxchars, uint8_t *src, int len){
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
 * Advertisment details
\*********************************************************************************************/

ble_advertisment_t BLEAdvertismentDetails;
#define MAX_ADVERT_DETAILS 200
char BLEAdvertismentDetailsJson[MAX_ADVERT_DETAILS];
uint8_t BLEAdvertismentDetailsJsonSet = 0;
uint8_t BLEAdvertismentDetailsJsonLost = 0;


void setDetails(ble_advertisment_t *ad){
  xSemaphoreTakeRecursive(BLEOperationsRecursiveMutex, portMAX_DELAY);
  if (BLEAdvertismentDetailsJsonSet){
    BLEAdvertismentDetailsJsonLost = 1;
    xSemaphoreGiveRecursive(BLEOperationsRecursiveMutex); // release mutex
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

/* they already have the name!
  if (ad->name[0] && (maxlen > 30)){
    strcpy(p, ",\"n\":\"");
    p += 6;
    maxlen -= 6;
    strcpy(p, ad->name);
    int len = strlen(ad->name);
    p += len;
    maxlen -= len;
    *(p++) = '\"'; maxlen--;
  }*/
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


  *(p++) = '}'; maxlen--;
  *(p++) = '}'; maxlen--;
  *(p++) = 0; maxlen--;
  
  BLEAdvertismentDetailsJsonSet = 1;
  xSemaphoreGiveRecursive(BLEOperationsRecursiveMutex); // release mutex
}


// call from main thread only!
// post advertisment detail if available, then clear.
void postAdvertismentDetails(){
  xSemaphoreTakeRecursive(BLEOperationsRecursiveMutex, portMAX_DELAY);
  if (BLEAdvertismentDetailsJsonSet){
    strncpy(TasmotaGlobal.mqtt_data, BLEAdvertismentDetailsJson, sizeof(TasmotaGlobal.mqtt_data));
    BLEAdvertismentDetailsJsonSet = 0;
    xSemaphoreGiveRecursive(BLEOperationsRecursiveMutex); // release mutex
    // no retain - this is present devices, not historic
    MqttPublishPrefixTopic_P(TELE, PSTR("BLE"), 0);
  } else {
    xSemaphoreGiveRecursive(BLEOperationsRecursiveMutex); // release mutex
  }
}



/*********************************************************************************************\
 * Classes
\*********************************************************************************************/

// does not really take any action
class BLESensorCallback : public NimBLEClientCallbacks {
  void onConnect(NimBLEClient* pClient) {
    BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("onConnect"));
  }
  void onDisconnect(NimBLEClient* pClient) {
    BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("onDisconnect"));
  }
  bool onConnParamsUpdateRequest(NimBLEClient* BLEClient, const ble_gap_upd_params* params) {
    BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("onConnParamsUpdateRequest"));
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
    uint64_t now = esp_timer_get_time();
    BLEScanLastAdvertismentAt = now; // note the time of the last advertisment

    uint32_t totalCount = BLEAdvertisment.totalCount;
    memset(&BLEAdvertisment, 0, sizeof(BLEAdvertisment));
    BLEAdvertisment.totalCount = totalCount+1;

    BLEAdvertisment.advertisedDevice = advertisedDevice;

    int RSSI = advertisedDevice->getRSSI();
    NimBLEAddress address = advertisedDevice->getAddress(); 
    memcpy(BLEAdvertisment.addr, address.getNative(), 6);
    MI32_ReverseMAC(BLEAdvertisment.addr);
    
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
    addSeenDevice(BLEAdvertisment.addr, BLEAdvertisment.name);


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

    if (BLEDetailsRequest && !memcmp(BLEDetailsMac, BLEAdvertisment.addr, 6)){
      if (BLEDetailsRequest == 1){
        BLEDetailsRequest = 0; // only one requested  if 2, it's a request all
      }
      setDetails(&BLEAdvertisment);
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
          BLEScan->erase(address);
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

static void BLEGenNotifyCB(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify){
    NimBLEClient *pRClient;

    BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("Notified length: %u"),length);

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
    thisop->state = GEN_STATE_NOTIFIED;
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
  BLEOperationsRecursiveMutex = xSemaphoreCreateRecursiveMutex();

  // this is only for testing, does nothin if examples are undefed
  installExamples();
  initSafeLog();
  initSeenDevices();

  BLE_ESP32::BLEStartOperationTask();
}

static void StartBLE(void) {
  BLEInitState = 0;
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

  if (BLEInitState) { return; }

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

  if (!BLEInitState) {
    uint64_t now = esp_timer_get_time();
    BLEScanLastAdvertismentAt = now; // initialise the time of the last advertisment

    NimBLEDevice::init("");
    
    if (BLEScan == nullptr) BLEScan = NimBLEDevice::getScan();

    AddLog_P(LOG_LEVEL_INFO,PSTR("BLE: init BLE device"));
    BLEInitState = 1;
  }
  return;
}

/*********************************************************************************************\
 * Task section
\*********************************************************************************************/

static void BLEOperationTask(void *pvParameters);

static void BLEStartOperationTask(){
    AddLog_P(LOG_LEVEL_DEBUG,PSTR("%s: Start operations"),D_CMND_BLE);

    xTaskCreatePinnedToCore(
      BLE_ESP32::BLEOperationTask,    /* Function to implement the task */
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

  NimBLEClient *pClient = NimBLEDevice::createClient();
  pClient->setClientCallbacks(&clientCB, false);
  /** Set initial connection parameters: These settings are 15ms interval, 0 latency, 120ms timout. 
   *  These settings are safe for 3 clients to connect reliably, can go faster if you have less 
   *  connections. Timeout should be a multiple of the interval, minimum is 100ms.
   *  Min interval: 12 * 1.25ms = 15, Max interval: 12 * 1.25ms = 15, 0 latency, 51 * 10ms = 510ms timeout 
   */
  pClient->setConnectionParams(12,12,0,51);
  /** Set how long we are willing to wait for the connection to complete (seconds), default is 30. */
  pClient->setConnectTimeout(5);

  for(;;){
    if (BLEScan){
      BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR("Operation loop"));

      BLE_ESP32::runCurrentOperation(&currentOperation, pClient);
    }
    // come around every second
    vTaskDelay(1000/ portTICK_PERIOD_MS);
  }

  try {
    NimBLEDevice::deleteClient(pClient);
    pClient = nullptr;
  } catch(const std::exception& e){
    BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("exception in delete"));
  }

  BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("Leave operation task"));
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
  //BLEStartScanTask();
  AddLog_P(LOG_LEVEL_DEBUG,PSTR("one sec (scanning %d) (adv count %d)"), BLERunningScan, BLEAdvertisment.totalCount);

  // check for application callbacks here.
  // this may remove complete items.
  BLE_ESP32::mainThreadOpCallbacks();

  // post any MQTT data if we completed anything in the last second
  if (completedOperations.size()){
    BLE_ESP32::BLEPostMQTT(true); // send only completed
  }

  if (BLEPublishDevices){
    BLEPublishDevices = 0;
    if (BLEAdvertMode != BLE_ESP32::BLE_NO_ADV_SEND){
      BLEPostMQTTSeenDevices();
    }
  }


  if (BLEScan){
    // restart scanning if it ended and we don't have any current operations in progress.
    if (!BLERunningScan && (!currentOperations.size())){
      //BLEScan->clearResults();
      if ((BLEMode != BLEModeDisabled) && (BLEMode != BLEModeByCommand)){
        AddLog_P(LOG_LEVEL_DEBUG,PSTR("Startscan from onesec"));
        BLEScan->setInterval(70);
        BLEScan->setWindow(50);
        BLEScan->setAdvertisedDeviceCallbacks(&BLEScanCallbacks,true);
        BLEScan->setActiveScan(true);

        // seems we could get the callback within the start call....
        // so set these before starting
        BLERunningScan = 1;
        BLEScanStartetdAt = esp_timer_get_time();
        BLEScan->start(BLEScanTimeS, BLEscanEndedCB, true); // 20s scans, restarted when then finish
      }
    } else {

      // check for a couple of possible failure modes.
      uint64_t now = esp_timer_get_time();
      if (BLEScanTimeS){
        uint64_t diff = now - BLEScanStartetdAt;
        diff = diff / 1000; // convert to ms;
        if (diff > (BLEScanTimeS + 5) * 1000){
          AddLog_P(LOG_LEVEL_ERROR,PSTR("Scan did not end on time"));
          BLERunningScan = 0;
          BLEScan->stop();
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
 * Operations functions - all to do with read/write and notify for a device
\*********************************************************************************************/

// this disconnects from a device if necessary, and then
// moves the operation from 'currentOperations' to 'completedOperations'.

// for safety's sake, only call from the run task
static void runTaskDoneOperation(BLE_ESP32::generic_sensor_t** op, NimBLEClient *pClient){
  BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("disconnect in done"));
  try {
    pClient->disconnect();
  } catch(const std::exception& e){
    BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("exception in disconnect"));
  }

  xSemaphoreTakeRecursive(BLEOperationsRecursiveMutex, portMAX_DELAY);
  // find this operation in currentOperations, and remove it.
  for (int i = 0; i < currentOperations.size(); i++){
    if (currentOperations[i]->opid == (*op)->opid){
      currentOperations.erase(currentOperations.begin() + i);
      break;
    }
  }
  xSemaphoreGiveRecursive(BLEOperationsRecursiveMutex); // release mutex

  // by adding it to this list, this will cause it to be sent to MQTT
  BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("add to completedOperations in done"));
  addOperation(&completedOperations, op);
  return;
}

// this retrievs the next operation from the passed list, and removes it from the list.
// or returns null if none.
generic_sensor_t* nextOperation(std::deque<generic_sensor_t*> *ops){
  generic_sensor_t* op = nullptr;
  if (ops->size()){
    xSemaphoreTakeRecursive(BLEOperationsRecursiveMutex, portMAX_DELAY);
    op = (*ops)[0]; 
    ops->pop_front();
    xSemaphoreGiveRecursive(BLEOperationsRecursiveMutex); // release mutex
  }
  return op;
}

// this adds an operation to the end of passed list.
// it also sets the op pointer passed to null.
int addOperation(std::deque<generic_sensor_t*> *ops, generic_sensor_t** op){
  int res = 0;
  xSemaphoreTakeRecursive(BLEOperationsRecursiveMutex, portMAX_DELAY);
  if (ops->size() < 10){
    ops->push_back(*op);
    *op = nullptr;
    res = 1;
  }     
  xSemaphoreGiveRecursive(BLEOperationsRecursiveMutex); // release mutex
  if (res){
    BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("added operation"));
  } else {
    BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("added operation - no room"));
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
    AddLog_P(LOG_LEVEL_ERROR,PSTR("op invalid in extQueueOperation"));
    return 0;
  }
  if (!(*op)){
    AddLog_P(LOG_LEVEL_ERROR,PSTR("*op invalid in extQueueOperation"));
    return 0;
  }
  (*op)->state = GEN_STATE_START; // trigger request later
  (*op)->opid = lastopid++;

  int res = addOperation(&queuedOperations, op);
  if (!res){
    AddLog_P(LOG_LEVEL_ERROR,PSTR("op added id %d in extQueueOperation failed"), (lastopid-1));
  }
  return res;
}



// this runs one operation
// if the passed pointer is empty, it tries to get a next one.
static void runCurrentOperation(BLE_ESP32::generic_sensor_t** pCurrentOperation, NimBLEClient *pClient){
  if (!pCurrentOperation) return;
  if (!BLEScan) return;

  if (!*pCurrentOperation) {
    *pCurrentOperation = nextOperation(&queuedOperations);
    if (*pCurrentOperation){
      BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("new currentOperation"));

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
      BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("notify timeout"));
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
      BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("operation complete"));
      BLE_ESP32::runTaskDoneOperation(pCurrentOperation, pClient);
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
    BLE_ESP32::SafeAddLog_P(LOG_LEVEL_ERROR,PSTR("operation failed"));
    BLE_ESP32::runTaskDoneOperation(pCurrentOperation, pClient);
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
          int newstate = GEN_STATE_STARTED;
          op->state = GEN_STATE_STARTED;

          NimBLEUUID serviceUuid(op->serviceStr);
          if (device.isAdvertisingService(serviceUuid)) {
            // create a client and connect
                
            if (pClient->connect(&device)) {
              NimBLERemoteService *pService = pClient->getService(serviceUuid);
              
              if (pService != nullptr) {
                BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("got service"));

                // read or write characteristic - we always need this?
                NimBLERemoteCharacteristic *pCharacteristic = nullptr;

                if (op->characteristicStr[0]){
                  pCharacteristic = pService->getCharacteristic(op->characteristicStr);
                }

                // pre-set to fail if no operations requested
                //newstate = GEN_STATE_FAILED_NOREADWRITE;

                int waitNotify = false;

                // if we have been asked to get a notification
                if (op->notificationCharacteristicStr[0]){
                  NimBLERemoteCharacteristic *pNCharacteristic = 
                    pService->getCharacteristic(op->notificationCharacteristicStr);
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
                        newstate = GEN_STATE_WAITNOTIFY;
                        waitNotify = true;
                      } else {
                        BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("failed subscribe for notify"));
                        newstate = GEN_STATE_FAILED_NOTIFY;
                      }
                    } else {
                      if(pNCharacteristic->canIndicate()) {
                        if(pNCharacteristic->subscribe(false, BLE_ESP32::BLEGenNotifyCB)) {
                          BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("subscribe for indicate"));
                          newstate = GEN_STATE_WAITINDICATE;
                          uint64_t now = esp_timer_get_time();
                          op->notifytimer = now;
                        } else {
                          BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("failed subscribe for indicate"));
                          newstate = GEN_STATE_FAILED_INDICATE;
                        }
                      } else {
                        newstate = GEN_STATE_FAILED_CANTNOTIFYORINDICATE;
                      }
                    }
                  } else {
                    newstate = GEN_STATE_FAILED_NONOTIFYCHAR;
                  }
                } // no supplied notify char is ok

                // this will only happen if you ask for a notify char which is not there?
                if (!(newstate & GEN_STATE_FAILED)){
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
                    BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("characteristic not in service"));
                  }
                }
              }

              // disconnect if not waiting for notify, 
              if (!op->notifytimer){
                BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("disconnect in runoperation"));
                try {
                  pClient->disconnect();
                } catch(const std::exception& e){
                  BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("exception in disconnect"));
                }
              }    

            } else {
              newstate = GEN_STATE_FAILED_CONNECT;
              // failed to connect
              BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("failed to connect to device"));
            }            
          } else {
            newstate = GEN_STATE_FAILED_NOSERVICE;
            BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("service not in device"));
          }
          op->state = newstate;
          break;
        }
       
    }
    if (i == deviceCount){
      // device not present
      op->state = GEN_STATE_FAILED_NODEVICE;
      BLE_ESP32::SafeAddLog_P(LOG_LEVEL_DEBUG,PSTR("device not present"));
    }
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
  }

  ResponseCmndDone();
}


//////////////////////////////////////////////////////////////
// Determine what to do with advertismaents
// BLEMode0 -> kill BLE completely
// BLEMode1 -> start BLE
// BLEMode2 -> start BLE
void CmndBLEMode(void){
  switch(XdrvMailbox.index){
    case BLEModeDisabled:
      BLENextMode = BLEModeDisabled;
      if (BLEMode != BLEModeDisabled){
        StopBLE();
        BLEMode = BLEModeDisabled;
        ResponseCmndChar("StoppingBLE");
      } else {
        ResponseCmndChar("Disabled");
      }
      break;
    case BLEModeByCommand:
      BLENextMode = BLEModeByCommand;
      if (BLEMode == BLEModeDisabled){
        StartBLE();
        BLEMode = BLEModeByCommand;
        ResponseCmndChar("StartingBLE");
      } else {
        ResponseCmndChar("BLERunning");
      }
      break;
    case BLEModeActive:
      BLENextMode = BLEModeActive;
      if (BLEMode == BLEModeDisabled){
        StartBLE();
        BLEMode = BLEModeActive;
        ResponseCmndChar("StartingBLE");
      } else {
        ResponseCmndChar("BLERunning");
      }
      break;
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

    default:
      ResponseCmndChar("InvalidIndex");
      break;
  }
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
// we expect BLEOp10 - trigger queue of op.  return is opid
// we expect BLEOp11 1 - cancel op

// returns: Done|FailCreate|FailNoOp|FailQueue|InvalidIndex|<opid>

// BLEop0/10 will cause an MQTT send of ops currently known.
// on op complete/op fail, a MQTT send is triggered of all known ops, and the completed/failed op removed.

// example: 
// backlog BLEOp1 001A22092CDB; BLEOp2 3e135142-654f-9090-134a-a6ff5bb77046; BLEOp3 3fa4585a-ce4a-3bad-db4b-b8df8179ea09; BLEOp4 03; BLEOp6 d0e8434d-cd29-0996-af41-6c90f4e0eb2a; bleop10;
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
      BLE_ESP32::BLEPostMQTT(false); // show all operations, not just completed
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
      strncpy(prepOperation->MAC, XdrvMailbox.data, sizeof(prepOperation->MAC)-1);
      AddLog_P(LOG_LEVEL_INFO,PSTR("MAC Set %s"),prepOperation->MAC);
    } break;
    case 2:
      if (!prepOperation) {
        ResponseCmndChar("FailNoOp");
        return;      
      }
      strncpy(prepOperation->serviceStr, XdrvMailbox.data, sizeof(prepOperation->serviceStr)-1);
      AddLog_P(LOG_LEVEL_INFO,PSTR("serviceStr Set %s"),prepOperation->serviceStr);
      break;
    case 3:
      if (!prepOperation) {
        ResponseCmndChar("FailNoOp");
        return;      
      }
      strncpy(prepOperation->characteristicStr, XdrvMailbox.data, sizeof(prepOperation->characteristicStr)-1);
      AddLog_P(LOG_LEVEL_INFO,PSTR("characteristicStr Set %s"),prepOperation->characteristicStr);
      break;
    case 4:
      if (!prepOperation) {
        ResponseCmndChar("FailNoOp");
        return;      
      }
      prepOperation->writelen = fromHex(prepOperation->dataToWrite, XdrvMailbox.data, sizeof(prepOperation->dataToWrite));
      AddLog_P(LOG_LEVEL_INFO,PSTR("dataToWrite Set %s"),XdrvMailbox.data);
      break;
    case 5:
      if (!prepOperation) {
        ResponseCmndChar("FailNoOp");
        return;      
      }
      prepOperation->readlen = 1;
      AddLog_P(LOG_LEVEL_INFO,PSTR("readlen Set %d"),prepOperation->readlen);
      break;
    case 6:
      if (!prepOperation) {
        ResponseCmndChar("FailNoOp");
        return;      
      }
      strncpy(prepOperation->notificationCharacteristicStr, XdrvMailbox.data, sizeof(prepOperation->notificationCharacteristicStr)-1);
      AddLog_P(LOG_LEVEL_INFO,PSTR("notififcationCharacteristicStr Set %s"),prepOperation->notificationCharacteristicStr);
      break;

    case 9:
      AddLog_P(LOG_LEVEL_INFO,PSTR("preview"));
      BLE_ESP32::BLEPostMQTT(false); // show all operations, not just completed
      break;
    case 10: {
      if (!prepOperation) {
        ResponseCmndChar("FailNoOp");
        return;      
      }
      //prepOperation->requestType = atoi(XdrvMailbox.data);
      prepOperation->state = GEN_STATE_START; // trigger request later
      prepOperation->opid = lastopid++;

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
        ResponseCmndNumber(lastopid-1);
        BLE_ESP32::BLEPostMQTT(false);
      }
      return;
    } break;
    /*case 11:
      if (!currentOperation) {
        ResponseCmndNumber(res);
        return;      
      }
      currentOperation->cancel = atoi(XdrvMailbox.data);
      AddLog_P(LOG_LEVEL_INFO,PSTR("cancel Set"));
      break;*/

    case 99: { // test programatically added operation
        sendExample();

        // dump what we have as diags
        BLE_ESP32::BLEPostMQTT(false); // show all operations, not just completed
      } break;
    default:
      ResponseCmndChar("InvalidIndex");
      return;
  }

  res = 100;
  ResponseCmndDone();
  return;
}


/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/
static void BLEPostMQTTSeenDevices() {
  getSeenDevicesToJson(TasmotaGlobal.mqtt_data, sizeof(TasmotaGlobal.mqtt_data));
  // no retain - this is present devices, not historic
  MqttPublishPrefixTopic_P(TELE, PSTR("BLE"), 0);
}

static void BLEPostMQTT(bool onlycompleted) {
  if (prepOperation || completedOperations.size() || queuedOperations.size() || currentOperations.size()){
    AddLog_P(LOG_LEVEL_INFO,PSTR("some to show"));
    xSemaphoreTakeRecursive(BLEOperationsRecursiveMutex, portMAX_DELAY);
    if (prepOperation && !onlycompleted){
      std::string out = BLETriggerResponse(prepOperation);
      xSemaphoreGiveRecursive(BLEOperationsRecursiveMutex); // release mutex
      snprintf_P(TasmotaGlobal.mqtt_data, sizeof(TasmotaGlobal.mqtt_data), PSTR("%s"), out.c_str());
      MqttPublishPrefixTopic_P(TELE, PSTR("BLE"), Settings.flag.mqtt_sensor_retain);
      AddLog_P(LOG_LEVEL_INFO,PSTR("prep sent %s"), out.c_str());
    } else {
      xSemaphoreGiveRecursive(BLEOperationsRecursiveMutex); // release mutex
    }
  

    if (queuedOperations.size() && !onlycompleted){
      AddLog_P(LOG_LEVEL_INFO,PSTR("queued %d"), queuedOperations.size());
      for (int i = 0; i < queuedOperations.size(); i++){
        xSemaphoreTakeRecursive(BLEOperationsRecursiveMutex, portMAX_DELAY);
        generic_sensor_t *toSend = queuedOperations[i];
        if (!toSend) {
          xSemaphoreGiveRecursive(BLEOperationsRecursiveMutex); // release mutex
          break;
        } else {
          std::string out = BLETriggerResponse(toSend);
          xSemaphoreGiveRecursive(BLEOperationsRecursiveMutex); // release mutex
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
        xSemaphoreTakeRecursive(BLEOperationsRecursiveMutex, portMAX_DELAY);
        generic_sensor_t *toSend = currentOperations[i];
        if (!toSend) {
          xSemaphoreGiveRecursive(BLEOperationsRecursiveMutex); // release mutex
          break;
        } else {
          std::string out = BLETriggerResponse(toSend);
          xSemaphoreGiveRecursive(BLEOperationsRecursiveMutex); // release mutex
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
        AddLog_P(LOG_LEVEL_DEBUG,PSTR("completedOperation removed"));

        if (!toSend) {
          break; // break from while loop
        } else {
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

static void mainThreadOpCallbacks() {
  if (completedOperations.size()){
    //AddLog_P(LOG_LEVEL_INFO,PSTR("completed %d"), completedOperations.size());

    xSemaphoreTakeRecursive(BLEOperationsRecursiveMutex, portMAX_DELAY);
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
    xSemaphoreGiveRecursive(BLEOperationsRecursiveMutex); // release mutex
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
  out = out + "\",\"state\":\"";
  sprintf(t, "%d", toSend->state);
  out = out + t;
  out = out + "\",\"MAC\":\"";
  out = out + toSend->MAC;
  out = out + "\",\"svc\":\"";
  out = out + toSend->serviceStr;
  out = out + "\",\"char\":\"";
  out = out + toSend->characteristicStr;
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
    out = out + ",\"wrote\":\"";
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



}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv47(uint8_t function)
{
  if (!Settings.flag5.mi32_enable) { return false; }  // SetOption115 - Enable ESP32 BLE BLE

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


