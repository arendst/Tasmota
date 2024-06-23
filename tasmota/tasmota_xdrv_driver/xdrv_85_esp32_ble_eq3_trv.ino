/*
  xdrv_85_esp32_ble_eq3_trv.ino - EQ3 radiator valve sense and control via BLE_ESP32 support for Tasmota

  Copyright (C) 2020  Simon Hailes

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
  1.0.1.0 20240113  publish - Add some values to WebUI; code cleanup
  1.0.0.0 20210910  publish - renamed to xdrv_85, and checked with TAS latest dev branch
  0.0.0.0 20201213  created - initial version
*/


/*
Commands:
e.g.
trv 001A22092EE0 settemp 22.5

trvperiod n - set polling period in seconds (default teleperiod at boot)
trvonlyaliased *0/1 - only hear devices with BLEAlias set
trvretries n - set the number of retries (default 4 at boot)
trvMatchPrefix 0/*1 - if set, then it will add trvs to the seen list which have mac starting with :
  macs in macprefixes, currently only 001a22
Note: anything with BLEAlias starting "EQ3" will be added to the seen list.
trvHideFailedPoll 0/*1 - if set, then failed polls will not be sent to EQ3
trvMinRSSI -n - the minimum RSSI value at which to attempt to poll


trv reset - clear device list
trv devlist - report seen devices.  Active scanning required, not passive, as it looks for names
trv scan - same as devlist
trv <mac> state - report general state (see below for MQTT)
trv <mac> raw <hex to send> - send a raw command
trv <mac> on - set temp to 30 -> display ON on EQ3
trv <mac> off - set temp to 4.5 -> display OFF on EQ3
trv <mac> boost - set boost
trv <mac> unboost - turn off boost
trv <mac> lock - manual lock of physical buttons
trv <mac> unlock - manual unlock of physical buttons
trv <mac> auto - set EQ3 to auto mode
trv <mac> manual - set EQ3 to manual mode
trv <mac> mode auto|manual - set EQ3 to mode auto|manual?
trv <mac> day - set EQ3 to day temp
trv <mac> night - set EQ3 to night temp
trv <mac> settemp 20.5 - set EQ3 to temp
trv <mac> settime - set time to Tasmota time (untested)
trv <mac> settime <hex as per esp32_mqtt_eq3> - set time
trv <mac> offset 1.5 - set offset temp
trv <mac> setdaynight 22 17.5 - set day and night mode temps
trv <mac> setwindowtempdur 12.5 30 - set window open temp and duration in mins

trv <mac> reqprofile <0-6> - request a profile for a day fo the week.
trv <mac> setprofile <0-6> 20.5-07:30,17-17:00,22.5-22:00,17-24:00 (up to 7 temp-HH:MM) - set a profile for a day fo the week.

Responses:
normal:
stat/EQ3/001A22092C9A = {
  "cmd":"state",
  "result":"ok",
  "RSSI":-83,
  "stattime":1613814193,
  "temp":21.0,
  "posn":0,
  "mode":"auto",
  "boost":"inactive",
  "dst":"set",
  "window":"closed",
  "state":"unlocked",
  "battery":"GOOD",
  "windowtemp": 12.0,
  "windowdur": 15,
  "day": 21.0,
  "night": 17.0,
  "offset": 0.0
}

holiday:
as above, but adds ,"holidayend":"YY-MM-DD HH:MM"

when trv <mac> reqprofile is used, adds:
  "profiledayN":"20.5-07:30,17.0-17:00,22.5-22:00,17.0-24:00"
where N is the day (0-6) (0 = saturday?).

when trv <mac> setprofile is used, adds:
"profiledayset":N
where N is the day (0-6) (0 = saturday?).

on error:
  "result":"fail",

The driver will try a command three times before reporting


4 digit pin calculation: (just for info)
serialno = "REQ0123456"
pin = []

x = str((ord(serialno[3]) ^ ord(serialno[7])) % 10)
pin.append(x)
x = str((ord(serialno[4]) ^ ord(serialno[8])) % 10)
pin.append(x)
x = str((ord(serialno[5]) ^ ord(serialno[9])) % 10)
pin.append(x)
x = str((ord(serialno[0]) - ord('A') ^ ord(serialno[6]) - ord('0')) % 10)
pin.append(x)
print("".join(pin))
*/

//#define VSCODE_DEV

#ifdef VSCODE_DEV
#define ESP32
#define USE_BLE_ESP32
#define USE_EQ3_ESP32
#endif

#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C6 || CONFIG_IDF_TARGET_ESP32S3
#ifdef USE_EQ3_ESP32
#ifdef ESP32                       // ESP32 only. Use define USE_HM10 for ESP8266 support
#ifdef USE_BLE_ESP32

#define XDRV_85                    85
#define D_CMND_EQ3 "trv"

// uncomment for more debug messages
//#define EQ3_DEBUG

namespace EQ3_ESP32 {

void CmndTrv(void);
void CmndTrvPeriod(void);
void CmndTrvRetries(void);
void CmndTrvOnlyAliased(void);
void CmndTrvMatchPrefix(void);
void CmndTrvMinRSSI(void);
void CmndTrvHideFailedPoll(void);

const char kEQ3_Commands[] PROGMEM = D_CMND_EQ3"|"
  "|"
  "period|"
  "retries|"
  "onlyaliased|"
  "MatchPrefix|"
  "MinRSSI|"
  "HideFailedPoll";

void (*const EQ3_Commands[])(void) PROGMEM = {
  &CmndTrv,
  &CmndTrvPeriod,
  &CmndTrvRetries,
  &CmndTrvOnlyAliased,
  &CmndTrvMatchPrefix,
  &CmndTrvMinRSSI,
  &CmndTrvHideFailedPoll
};

const char *cmdnames[] = {
  "poll",
  "raw",
  "state",
  "settime",
  "settemp",
  "offset",
  "setdaynight",
  "setwindowtempdur",
  "setholiday",
  "boost",
  "unboost",
  "unlock",
  "auto",
  "manual",
  "eco",
  "on",
  "off",
  "valve",
  "mode",
  "day",
  "night",
  "reqprofile",
  "setprofile",
  "lock",
};

const uint8_t *macprefixes[1] = {
  (uint8_t *)"\x00\x1a\x22"
};

int EQ3GenericOpCompleteFn(BLE_ESP32::generic_sensor_t *pStruct);

const char EQ3_Svc[] PROGMEM = "3e135142-654f-9090-134a-a6ff5bb77046";
const char EQ3_rw_Char[] PROGMEM = "3fa4585a-ce4a-3bad-db4b-b8df8179ea09";
const char EQ3_notify_Char[] PROGMEM = "d0e8434d-cd29-0996-af41-6c90f4e0eb2a";

struct eq3_device_tag{
  uint8_t addr[7];
  int8_t RSSI;
  uint64_t timeoutTime;
  uint8_t pairing;
  uint8_t lastStatus[16]; // last received 02 stat
  uint8_t lastStatusLen;
  uint32_t lastStatusTime; // in utc
  uint8_t nextDiscoveryData;
  float TargetTemp;
  uint8_t DutyCycle;
  bool Battery;
} eq3_device_t;

/*********************************************************************************************\
 * variables to control operation
\*********************************************************************************************/
int retries = 0;
// allow 240s before timeout of sa device - based on that we restart BLE if we don't see adverts for 120s
#define EQ3_TIMEOUT 240L

uint8_t pairingaddr[7] = {0,0,0,0,0,0};
char pairingserial[20];
uint8_t pairing = 0;

#define EQ3_NUM_DEVICESLOTS 16
eq3_device_tag EQ3Devices[EQ3_NUM_DEVICESLOTS];
SemaphoreHandle_t EQ3mutex = nullptr;

int EQ3Period = 300;
int EQ3Retries = 4;
uint8_t EQ3OnlyAliased = 0;
uint8_t EQ3MatchPrefix = 1;
uint8_t opInProgress = 0;
int seconds = 20;
int EQ3CurrentSingleSlot = 0;

uint8_t EQ3TopicStyle = 1;
uint8_t EQ3HideFailedPoll = 1;
int8_t trvMinRSSI = -99;

// control of timing of sending polling.
// we leave an interval between polls to allow scans to take place
int intervalSeconds = 10; // min seconds between operations
int intervalSecondsCounter = 0; // set when an operation is over to intervalSeconds
int nextEQ3Poll = EQ3_NUM_DEVICESLOTS; // set to zero to start a poll cycle

#pragma pack( push, 1 )  // aligned structures for size
struct op_t {
  uint8_t addr[7];
  uint8_t towrite[16];
  uint8_t writelen;
  uint8_t cmdtype;
};
#pragma pack(pop)

std::deque<EQ3_ESP32::op_t*> opQueue;


/*********************************************************************************************\
 * Functions
\*********************************************************************************************/

const char *addrStr(const uint8_t *addr, int useAlias = 0){
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

char *topicPrefix(int prefix, const uint8_t *addr, int useAlias){
  static char stopic[TOPSZ];
  const char *id = addrStr(addr, useAlias);
  if (!EQ3TopicStyle){
    GetTopic_P(stopic, prefix, TasmotaGlobal.mqtt_topic, PSTR(""));
    strcat(stopic, PSTR("EQ3/"));
    strcat(stopic, id);
  } else {
    char p[] = "EQ3";
    GetTopic_P(stopic, prefix, p, id);
  }
  return stopic;
}

// return 0+ if we find the addr has one of our listed prefixes
// return -1 if we don't recognise the mac
int matchPrefix(const uint8_t *addr){
  for (int i = 0; i < sizeof(macprefixes)/sizeof(*macprefixes); i++){
    if (!memcmp(addr, macprefixes[i], 3)){
      return i;
    }
  }
  return -1;
}

bool EQ3Operation(const uint8_t *MAC, const uint8_t *data, int datalen, int cmdtype, int retries_in = 0) {
  BLE_ESP32::generic_sensor_t *op = nullptr;

  // ALWAYS use this function to create a new one.
  int res = BLE_ESP32::newOperation(&op);
  if (!res){
    AddLog(LOG_LEVEL_ERROR, PSTR("EQ3: %s: Can't get a newOperation from BLE"), addrStr(MAC, cmdtype & 0x80));
    retries = 0;
    return 0;
  } else {
#ifdef EQ3_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR("EQ3: %s: Got a newOperation from BLE"), addrStr(MAC, cmdtype & 0x80));
#endif
  }

  NimBLEAddress addr((uint8_t *)MAC);
  op->addr = addr;

  bool havechar = false;
  op->serviceUUID = NimBLEUUID(EQ3_Svc);
  op->characteristicUUID = NimBLEUUID(EQ3_rw_Char);
  op->notificationCharacteristicUUID = NimBLEUUID(EQ3_notify_Char);

  if (data && datalen) {
    op->writelen = datalen;
    memcpy(op->dataToWrite, data, datalen);
  } else {
    op->writelen = 1;
    op->dataToWrite[0] = 0x03; // just request status
  }

  // this op will call us back on complete or failure.
  op->completecallback = (void *)EQ3GenericOpCompleteFn;
  // store this away for later
  op->context = (void *)cmdtype;

  res = BLE_ESP32::extQueueOperation(&op);
  if (!res){
    // if it fails to add to the queue, do please delete it
    BLE_ESP32::freeOperation(&op);
    AddLog(LOG_LEVEL_ERROR, PSTR("EQ3: %s: Failed to queue new operation - deleted"), addrStr(MAC, cmdtype & 0x80));
    retries = 0;
  } else {
    if (retries_in){
      retries = retries_in;
    }
  }

  return res;
}

int EQ3DoOp(){
  if (!opInProgress){
    if (opQueue.size()){
      op_t* op = opQueue[0];
      if (EQ3Operation(op->addr, op->towrite, op->writelen, op->cmdtype, EQ3Retries)){
        opQueue.pop_front();
        opInProgress = 1;
        AddLog(LOG_LEVEL_DEBUG, PSTR("EQ3: %s: Op dequeued len now %d"), addrStr(op->addr, (op->cmdtype & 0x80)), opQueue.size());
        delete op;
        return 1;
      } else {
        AddLog(LOG_LEVEL_ERROR, PSTR("EQ3: %s: Op BLE could not start op queue len %d"), addrStr(op->addr, (op->cmdtype & 0x80)), opQueue.size());
      }
    }
  }
  return 0;
}

int EQ3QueueOp(const uint8_t *MAC, const uint8_t *data, int datalen, int cmdtype, int useAlias) {
  op_t* newop = new op_t;
  memcpy(newop->addr, MAC, 6);
  memcpy(newop->towrite, data, datalen);
  newop->writelen = datalen;
  newop->cmdtype = cmdtype | (useAlias?0x80:0);
  opQueue.push_back(newop);
  int qlen = opQueue.size();
  AddLog(LOG_LEVEL_DEBUG, PSTR("EQ3: %s: Op queued len now %d"), addrStr(newop->addr, (newop->cmdtype & 0x80)), qlen);
  EQ3DoOp();
  return qlen;
}

int EQ3ParseOp(BLE_ESP32::generic_sensor_t *op, bool success, int retries){
  int res = 0;
  opInProgress = 0;
  ResponseClear();

  uint8_t addrev[7];
  const uint8_t *native = op->addr.getNative();
  memcpy(addrev, native, 6);
  BLE_ESP32::ReverseMAC(addrev);

  eq3_device_tag *eq3 = nullptr;

  int free = -1;
  for (int i = 0; i < EQ3_NUM_DEVICESLOTS; i++){
    if (!memcmp(EQ3Devices[i].addr, addrev, 6)){
      eq3 = &EQ3Devices[i];
      break;
    }
  }

  int cmdtype = (((uint32_t)op->context) & 0xff);
  const char *cmdType = PSTR("invalid");
  int useAlias = cmdtype & 0x80;
  cmdtype &= 0x7f;
  if ((cmdtype >= 0) && (cmdtype < sizeof(cmdnames)/sizeof(*cmdnames))){
    cmdType = cmdnames[cmdtype];
  }

  ResponseAppend_P(PSTR("{"));
  ResponseAppend_P(PSTR("\"cmd\":\"%s\""), cmdType);
  ResponseAppend_P(PSTR(",\"result\":\"%s\""), success? "ok":"fail");
  ResponseAppend_P(PSTR(",\"MAC\":\"%s\""), addrStr(addrev));
  const char *host = NetworkHostname();
  ResponseAppend_P(PSTR(",\"tas\":\"%s\""), host);
  if (cmdtype == 1){
    char raw[40];
    BLE_ESP32::dump(raw, 40, op->dataNotify, op->notifylen);
    ResponseAppend_P(PSTR(",\"raw\":\"%s\""), raw);
  }

  uint8_t *status = {0};
  uint8_t statlen = 0;
  uint32_t stattime = 0;

  if (success){
    if ((op->notifylen >= 6) && (op->dataNotify[0] == 2) && (op->dataNotify[1] == 1)){
      if (eq3){
        memcpy(eq3->lastStatus, op->dataNotify, (op->notifylen <= 10)?op->notifylen:16);
        eq3->lastStatusLen = (op->notifylen <= 10)?op->notifylen:16;
        eq3->lastStatusTime = UtcTime();
      }
    }

    status = op->dataNotify;
    statlen = op->notifylen;
    stattime = UtcTime();
  }

  if (eq3){
    status = eq3->lastStatus;
    statlen = eq3->lastStatusLen;
    stattime = eq3->lastStatusTime;
    ResponseAppend_P(PSTR(",\"RSSI\":%d"), eq3->RSSI);
  }

  if ((statlen >= 6) && (status[0] == 2) && (status[1] == 1)) {
    ResponseAppend_P(PSTR(",\"stattime\":%u"), stattime);
    eq3->TargetTemp = (float)status[5] / 2;
    ResponseAppend_P(PSTR(",\"temp\":%2.1f"), eq3->TargetTemp);
    eq3->DutyCycle = status[3];
    ResponseAppend_P(PSTR(",\"posn\":%d"), eq3->DutyCycle);
    int stat = status[2];
    ResponseAppend_P(PSTR(",\"mode\":"));
    switch (stat & 3){
      case 0:
        ResponseAppend_P(PSTR("\"auto\""));
        break;
      case 1:
        ResponseAppend_P(PSTR("\"manual\""));
        break;
      case 2:
        ResponseAppend_P(PSTR("\"holiday\""));
        break;
      case 3:
        ResponseAppend_P(PSTR("\"manualholiday\""));
        break;
    }

    ResponseAppend_P(PSTR(",\"hassmode\":"));
    do {
      //HASS allowed modes [“auto”, “off”, “cool”, “heat”, “dry”, “fan_only”]
      //0201283B042A
      // If its in auto or holiday, set to auto
      if ((stat & 3) == 0) { ResponseAppend_P(PSTR("\"auto\"")); break; }
      // If its in manual and 4.5°C, set to off
      if (((stat & 3) == 1) && (status[5] == 9)) { ResponseAppend_P(PSTR("\"off\"")); break; }
      // If its in manual above 4.5°C and valve is open, set to heat
      if (((stat & 3) == 1) && (status[5] > 9) && (status[3] > 0)) { ResponseAppend_P(PSTR("\"heat\"")); break; }
      // If its in manual above 4.5°C and valve is closed, set to off
      if (((stat & 3) == 1) && (status[5] > 9)) { ResponseAppend_P(PSTR("\"off\"")); break; }
      //Fallback off
      ResponseAppend_P(PSTR("\"off\""));
      break;
    } while (0);

    ResponseAppend_P(PSTR(",\"boost\":\"%s\""), (stat & 4) ? "active" : "inactive");
    ResponseAppend_P(PSTR(",\"dst\":\"%s\""), (stat & 8) ? "set" : "unset");
    ResponseAppend_P(PSTR(",\"window\":\"%s\""), (stat & 16) ? "open" : "closed");
    ResponseAppend_P(PSTR(",\"state\":\"%s\""), (stat & 32) ? "locked" : "unlocked");
    eq3->Battery = stat & 128;
    ResponseAppend_P(PSTR(",\"battery\":\"%s\""), eq3->Battery ? "LOW" : "GOOD");
  }

  if ((statlen >= 10) && (status[0] == 2) && (status[1] == 1)){
    int mm = status[8] * 30;
    int hh = mm / 60;
    mm = mm % 60;
    ResponseAppend_P(PSTR(",\"holidayend\":\"%02d-%02d-%02d %02d:%02d\""),
      status[7],
      status[9],
      status[6],
      hh, mm
      );

    if (statlen >= 15) {
      ResponseAppend_P(PSTR(",\"windowtemp\":%2.1f"), ((float)status[10]) /  2);
      ResponseAppend_P(PSTR(",\"windowdur\":%d"), ((int)status[11]) * 5);
      ResponseAppend_P(PSTR(",\"day\":%2.1f"), ((float)status[12]) / 2);
      ResponseAppend_P(PSTR(",\"night\":%2.1f"), ((float)status[13]) / 2);
      ResponseAppend_P(PSTR(",\"offset\":%2.1f"), ((float)status[14] - 7) / 2);
    }

  }

  if (success) {
    // now to parse other data - this may not have been a stat message
    if ((op->notifylen >= 3) && (op->dataNotify[0] == 2) && (op->dataNotify[1] == 2)){
      ResponseAppend_P(PSTR(",\"profiledayset\":%d"), op->dataNotify[2]);
    }

    if ((op->notifylen >= 16) && (op->dataNotify[0] == 0x21)){
//YY is the time, coded as (minutes/10), up to which to maintain the temperature declared in XX
//XX represents the temperature to be maintained until then, codified as (temperature*2)
// byte 0: 21 (default value)
// byte 1: 02 (Monday = 0x02)
// byte (2,3): 22 24 (17°C up to 06:00)
// byte (4,5): 2A 36 (21°C up to 09:00)
// byte (6,7): 22 66 (17°C up to 17:00)
// byte (8,9): 2A 8A (21°C up to 23:00)
// byte (10,11): 22 90 (17°C up to 24:00)
// byte (12,13): 22 90 (unused)
// byte (14,15): 22 90 (unused)
      ResponseAppend_P(PSTR(",\"profileday%d\":\""), op->dataNotify[1]);
      uint8_t *data = op->dataNotify + 2;
      for (int i = 0; i < 7; i++){
        float t = *(data++);
        t /= 2;
        int mm = *(data++);
        mm *= 10;
        int hh = mm / 60;
        mm = mm % 60;
        ResponseAppend_P(PSTR("%2.1f-%02d:%02d"), t, hh, mm);
        // stop if the last one is 24.
        if (hh == 24){
          break;
        }

        if (i < 6){
          ResponseAppend_P(PSTR(","));
        }
      }
      ResponseAppend_P(PSTR("\""));
    }

    res = 1;
  }

  ResponseAppend_P(PSTR("}"));

  int type = STAT;
  if (cmdtype){
    type = STAT;
  } else {
    // it IS a poll command
    if (EQ3HideFailedPoll){
      if (!success){
        AddLog(LOG_LEVEL_DEBUG, PSTR("EQ3: %s: Poll fail not sent because EQ3HideFailedPoll"), addrStr(addrev));
        return res;
      }
    }
  }

  char *topic = topicPrefix(type, addrev, useAlias);
  MqttPublish(topic, false);
  return res;
}

int EQ3GenericOpCompleteFn(BLE_ESP32::generic_sensor_t *op){
  uint32_t context = (uint32_t) op->context;
  opInProgress = 0;

  if (op->state <= GEN_STATE_FAILED){
    uint8_t addrev[7];
    const uint8_t *native = op->addr.getNative();
    memcpy(addrev, native, 6);
    BLE_ESP32::ReverseMAC(addrev);

    if (retries > 1){
      retries--;

      if (EQ3Operation(addrev, op->dataToWrite, op->writelen, (int)op->context)){
        //EQ3ParseOp(op, false, retries);
        AddLog(LOG_LEVEL_ERROR, PSTR("EQ3: %s: trv operation failed - retrying %d"), addrStr(addrev), op->state);
        opInProgress = 1;
      } else {
        retries = 0;
        EQ3ParseOp(op, false, 0);
        AddLog(LOG_LEVEL_ERROR, PSTR("EQ3: %s: trv operation failed to send op %d"), addrStr(addrev), op->state);
      }
    } else {
      retries = 0;
      EQ3ParseOp(op, false, 0);
      AddLog(LOG_LEVEL_ERROR, PSTR("EQ3: %s: trv operation failed - no more retries %d"), addrStr(addrev), op->state);
    }
    return 0;
  }

  retries = 0;

  EQ3ParseOp(op, true, 0);
  return 0;
}



/*********************************************************************************************\
 * Functons actualy called from within the BLE task
\*********************************************************************************************/

int ispairing2(const uint8_t *payload, int len, char *name, int namelen, char *serial, int seriallen ){
  while (len){
    int l = *payload;
    //BLE_ESP32::dump(temp, 40, payload, l+1);
    //AddLog(LOG_LEVEL_ERROR, PSTR("EQ3: %s"), temp);

    payload++;
    len--;
    if (len < l){
      //AddLog(LOG_LEVEL_ERROR, PSTR("EQ3: part len er %d<%d"),len, l);
      return 0;
    }
    switch (*payload){
      case 0xff: {// parse the EQ3 advert payload looking for nnFF01ssssssss
        payload++;
        len--;
        l--;
        if (*payload == 1){
          payload++;
          len--;
          l--;
          //char serialstr[20];
          //strncpy(serialstr, (const char *)payload, l);
          //AddLog(LOG_LEVEL_DEBUG, PSTR("EQ3: adv part FF01 detected %s"), serialstr);
          // we don;t use these, but that's what they seem to be....
          uint8_t copylen = (l > seriallen)?seriallen:l;
          strncpy(serial, (const char *)payload, copylen);
          serial[seriallen-1] = 0;
          payload += l;
          len -= l;
          return 1;
        } else {
          payload += l;
          len -= l;
        }
      } break;
      case 0x09: {
        payload++;
        len--;
        l--;
        if (*payload == 1){
          payload++;
          len--;
          l--;
          //char serialstr[20];
          //strncpy(serialstr, (const char *)payload, l);
          //AddLog(LOG_LEVEL_DEBUG, PSTR("EQ3: adv part FF01 detected %s"), serialstr);
          // we don;t use these, but that's what they seem to be....
          uint8_t copylen = (l > namelen)?namelen:l;
          strncpy(name, (const char *)payload, copylen);
          name[namelen-1] = 0;
          payload += l;
          len -= l;
          //return 1;
        } else {
          payload += l;
          len -= l;
        }
      } break;
      default:{
        payload += l;
        len -= l;
      } break;
    }
  }
  return 0;
}

int ispairing(const uint8_t *payload, int len){
  //char temp[40];
  //BLE_ESP32::dump(temp, 40, payload, len);
  //AddLog(LOG_LEVEL_DEBUG, PSTR("EQ3: pair%d %s"), len, temp);
  while (len){
    int l = *payload;
    //BLE_ESP32::dump(temp, 40, payload, l+1);
    //AddLog(LOG_LEVEL_ERROR, PSTR("EQ3: %s"), temp);

    payload++;
    len--;
    if (len < l){
      //AddLog(LOG_LEVEL_ERROR, PSTR("EQ3: part len er %d<%d"),len, l);
      return 0;
    }
    if (*payload == 0xff){
      payload++;
      len--;
      l--;
      if (*payload == 1){
        payload++;
        len--;
        l--;
        //char serialstr[20];
        //strncpy(serialstr, (const char *)payload, l);
        //AddLog(LOG_LEVEL_DEBUG, PSTR("EQ3: adv part FF01 detected %s"), serialstr);
        // we don;t use these, but that's what they seem to be....
        const uint8_t *serial = payload;
        uint8_t seriallen = l;
        payload += l;
        len -= l;
        return 1;
      } else {
        payload += l;
        len -= l;
      }
    } else {
      payload += l;
      len -= l;
    }
  }
  return 0;
}

int TaskEQ3AddDevice(int8_t RSSI, const uint8_t* addr, char *serial){
  int free = -1;
  int i = 0;
  uint64_t now = esp_timer_get_time();

  if (serial && serial[0] && !pairing){
    memcpy(pairingaddr, addr, 6);
    strncpy(pairingserial, serial, sizeof(pairingserial));
    pairingserial[sizeof(pairingserial)-1] = 0;
    pairing = 1;
  }

  for(i = 0; i < EQ3_NUM_DEVICESLOTS; i++) {
    if(!memcmp(addr, EQ3Devices[i].addr, 6)) {
      break;
    }
    if (EQ3Devices[i].timeoutTime && (EQ3Devices[i].timeoutTime < now)) {
#ifdef EQ3_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR("EQ3: %s: timeout at %d"), addrStr(EQ3Devices[i].addr), i);
#endif
      EQ3Devices[i].timeoutTime = 0L;
    }
    if (!EQ3Devices[i].timeoutTime){
      if (free == -1){
        free = i;
      }
    }
  }

  if (i == EQ3_NUM_DEVICESLOTS){
    if (free >= 0){
      i = free;
    } else {
      AddLog(LOG_LEVEL_ERROR, PSTR("EQ3: %s: lost > %d devices"), addrStr(addr), EQ3_NUM_DEVICESLOTS);
      return 0;
    }
  }

#ifdef EQ3_DEBUG
  if (!EQ3Devices[i].timeoutTime)
    AddLog(LOG_LEVEL_INFO, PSTR("EQ3: %s: added at %d"), addrStr(addr), i);
#endif

  EQ3Devices[i].timeoutTime = now + (1000L*1000L)*EQ3_TIMEOUT;
  memcpy(EQ3Devices[i].addr, addr, 6);
  EQ3Devices[i].RSSI = RSSI;

  EQ3Devices[i].pairing = (serial && serial[0])?1:0;

  return 1;
}


const char *EQ3Names[] = {
  "CC-RT-BLE",
  "CC-RT-BLE-EQ",
  "CC-RT-M-BLE"
};

int TaskEQ3advertismentCallback(BLE_ESP32::ble_advertisment_t *pStruct)
{
  // we will try not to use this...
  BLEAdvertisedDevice *advertisedDevice = pStruct->advertisedDevice;

  std::string sname = advertisedDevice->getName();

  bool found = false;
  const char *nameStr = sname.c_str();
  int8_t RSSI = pStruct->RSSI;
  const uint8_t *addr = pStruct->addr;


  const char *alias = BLE_ESP32::getAlias(addr);
  if (EQ3OnlyAliased){
    // ignore unless we have an alias.
    if (!alias || !(*alias)){
      return 0;
    }
  }
  if (!alias) alias = "";

  for (int i = 0; i < sizeof(EQ3Names)/sizeof(*EQ3Names); i++){
    if (!strcmp(nameStr, EQ3Names[i])){
      found = true;
      break;
    }
  }

  if (!found && !strncmp(alias, "EQ3", 3)){
    found = true;
  }

  // if the addr matches the EQ2 mfg prefix, add it?
  if (!found && EQ3MatchPrefix && (matchPrefix(addr) >= 0)){
    found = true;
  }

  if (!found) return 0;

#ifdef EQ3_DEBUG
  if (BLE_ESP32::BLEDebugMode) AddLog(LOG_LEVEL_DEBUG, PSTR("EQ3: %s: saw device"),advertisedDevice->getAddress().toString().c_str());
#endif

  uint8_t* payload = advertisedDevice->getPayload();
  size_t payloadlen = advertisedDevice->getPayloadLength();

  char name[20] = {0};
  char serial[20] = {0};
  int pairing = 0;
  ispairing2(payload, payloadlen, name, 20, serial, 20);

  // this will take and keep the mutex until the function is over
  TasAutoMutex localmutex(&EQ3mutex);
  TaskEQ3AddDevice(RSSI, addr, serial);
  return 0;
}

/*********************************************************************************************\
 * Helper functions
\*********************************************************************************************/

/*********************************************************************************************\
 * init
\*********************************************************************************************/
void EQ3Init(void) {
  memset(&EQ3Devices, 0, sizeof(EQ3Devices));
  BLE_ESP32::registerForAdvertismentCallbacks((const char *)"EQ3", TaskEQ3advertismentCallback);
#ifdef EQ3_DEBUG
  AddLog(LOG_LEVEL_INFO, PSTR("EQ3: init: request callbacks"));
#endif

  EQ3Period = Settings->tele_period;

  return;
}

/***********************************************************************\
 * Regular
\***********************************************************************/

void EQ3Every50mSecond(){

}

/**
 * @brief Main loop of the driver, "high level"-loop
 *
 */
int EQ3Send(const uint8_t* addr, const char *cmd, char* param, char* param2, int useAlias);

void EQ3EverySecond(bool restart){
  if (pairing){
    char p[40]; // used in dump
    BLE_ESP32::dump(p, 20, pairingaddr, 6);
    Response_P(PSTR("{\"pairing\":\"%s\",\"serial\":\"%s\"}"), p, pairingserial);
    char addrstr[4+8*2+2] = "EQ3/";
    BLE_ESP32::dump(&addrstr[4], 8*2+2, pairingaddr, 6);
    char *topic = topicPrefix(STAT, pairingaddr, 1);
    MqttPublish(topic, false);
    pairing = 0;
  }

  seconds --;
  if (seconds <= 0){
    if (EQ3Period){
      if (nextEQ3Poll >= EQ3_NUM_DEVICESLOTS){
        AddLog(LOG_LEVEL_DEBUG, PSTR("EQ3: poll cycle starting"));
        nextEQ3Poll = 0;
      } else {
        AddLog(LOG_LEVEL_ERROR, PSTR("EQ3: poll overrun, deferred - last loop only got to %d, not %d"), nextEQ3Poll, EQ3_NUM_DEVICESLOTS);
      }
    }
    seconds = EQ3Period;
  }

  if (EQ3Period){
    int qlen = opQueue.size();
    if ((nextEQ3Poll < EQ3_NUM_DEVICESLOTS) && (qlen == 0) && (!opInProgress)){
      if (intervalSecondsCounter){
        intervalSecondsCounter--;
      } else {
        // queue a EQ3Status op against each known EQ3.
        // mark it as a regular stat rather than a use cmd.
        for(int i = nextEQ3Poll; i < EQ3_NUM_DEVICESLOTS; i++){
          if (!EQ3Devices[i].timeoutTime){
            nextEQ3Poll = i+1;
            continue;
          }

          // trvMinRSSI
          // find the device in BLE to get RSSI
          if (EQ3Devices[i].RSSI < trvMinRSSI){
            AddLog(LOG_LEVEL_DEBUG, PSTR("EQ3: %s: RSSI %d < min %d, poll suppressed"), addrStr(EQ3Devices[i].addr), EQ3Devices[i].RSSI, trvMinRSSI);
            nextEQ3Poll = i+1;
            continue;
          }

          EQ3Send(EQ3Devices[i].addr, PSTR("poll"), nullptr, nullptr, 1);
          nextEQ3Poll = i+1;
          intervalSecondsCounter = intervalSeconds;
          break;
        }
      }
    }
  }

  // start next op now, if we have any queued
  EQ3DoOp();

}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/
int EQ3SendCurrentDevices(){
  // send the active devices
  ResponseClear();
  ResponseAppend_P(PSTR("{\"devices\":{"));
  int added = 0;
  for(int i = 0; i < EQ3_NUM_DEVICESLOTS; i++){
    char p[40];
    if (!EQ3Devices[i].timeoutTime)
      continue;
    if (added){
      ResponseAppend_P(PSTR(","));
    }
    BLE_ESP32::dump(p, 20, EQ3Devices[i].addr, 6);
    ResponseAppend_P(PSTR("\"%s\":%d"), p, EQ3Devices[i].RSSI);
    added = 1;
  }
  ResponseAppend_P(PSTR("}}"));
  MqttPublishPrefixTopic_P(STAT, PSTR("EQ3"), false);
  return 0;
}

int EQ3SendResult(char *requested, const char *result){
  // send the result
  Response_P(PSTR("{\"result\":\"%s\"}"), result);
  static char stopic[TOPSZ];
  GetTopic_P(stopic, STAT, TasmotaGlobal.mqtt_topic, PSTR(""));
  strcat(stopic, PSTR("EQ3/"));
  strcat(stopic, requested);
  MqttPublish(stopic, false);
  return 0;
}

#ifdef USE_WEBSERVER
const char HTTP_EQ3_ALIAS[]        PROGMEM = "{s}EQ3 %d Alias{m}%s{e}";
const char HTTP_EQ3_MAC[]          PROGMEM = "{s}EQ3 %d " D_MAC_ADDRESS "{m}%s{e}";
const char HTTP_EQ3_RSSI[]         PROGMEM = "{s}EQ3 %d " D_RSSI "{m}%d dBm{e}";
const char HTTP_EQ3_TEMPERATURE[]  PROGMEM = "{s}EQ3 %d %s{m}%*_f " D_UNIT_DEGREE "%c{e}";
const char HTTP_EQ3_DUTY_CYCLE[]   PROGMEM = "{s}EQ3 %d " D_THERMOSTAT_VALVE_POSITION "{m}%d " D_UNIT_PERCENT "{e}";
const char HTTP_EQ3_BATTERY[]      PROGMEM = "{s}EQ3 %d " D_BATTERY "{m}%s{e}";

void EQ3Show(void)
{
  char c_unit = D_UNIT_CELSIUS[0]; // ToDo: Check if fahrenheit is possible -> temp_format==TEMP_CELSIUS ? D_UNIT_CELSIUS[0] : D_UNIT_FAHRENHEIT[0];
  bool FirstSensorShown = false;

  for (int i = 0; i < EQ3_NUM_DEVICESLOTS; i++) {
    if (EQ3Devices[i].timeoutTime) {
      if (FirstSensorShown) WSContentSend_P(HTTP_SNS_HR_THIN);
      FirstSensorShown = true;
      const char *alias = BLE_ESP32::getAlias(EQ3Devices[i].addr);
      if (alias && *alias){
        WSContentSend_P(HTTP_EQ3_ALIAS, i + 1, alias);
      }
      WSContentSend_P(HTTP_EQ3_MAC, i + 1, addrStr(EQ3Devices[i].addr));
      WSContentSend_PD(HTTP_EQ3_RSSI, i + 1, EQ3Devices[i].RSSI);
      WSContentSend_PD(HTTP_EQ3_TEMPERATURE, i + 1, D_THERMOSTAT_SET_POINT, Settings->flag2.temperature_resolution, &EQ3Devices[i].TargetTemp, c_unit);
      WSContentSend_P(HTTP_EQ3_DUTY_CYCLE, i + 1, EQ3Devices[i].DutyCycle);
      WSContentSend_P(HTTP_EQ3_BATTERY, i + 1, EQ3Devices[i].Battery ? D_NEOPOOL_LOW : D_OK);
    }
  }
}
#endif // USE_WEBSERVER

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void simpletolower(char *p){
  if (!p) return;
  while (*p){
    *p = *p | 0x20;
    p++;
  }
}

//
// great description here:
// https://reverse-engineering-ble-devices.readthedocs.io/en/latest/protocol_description/00_protocol_description.html
// not all implemented yet.
//
int EQ3Send(const uint8_t* addr, const char *cmd, char* param, char* param2, int useAlias){

  char p[] = "";
  if (!param) param = p;
  if (!param2) param2 = p;
  uint8_t d[20];
  memset(d, 0, sizeof(d));
  int dlen = 0;
#ifdef EQ3_DEBUG
  AddLog(LOG_LEVEL_INFO, PSTR("EQ3: %s: cmd: [%s] [%s] [%s]"), addrStr(addr), cmd, param, param2);
#endif

/* done on whole string before here.
  simpletolower(cmd);
  simpletolower(param);
  simpletolower(param2);
*/

  int cmdtype = 0;

  do {
    if (!strcmp(cmd, "raw")){
      cmdtype = 1;
      if (!param || param[0] == 0){
        return -1;
      }
      int len = strlen(param) / 2;
      if (len > 20){
        AddLog(LOG_LEVEL_ERROR, PSTR("EQ3: raw len of %s = %d > 20"), param, len);
        return -1;
      }
      BLE_ESP32::fromHex(d, param, len);
      dlen = len;
      break;
    }

/*    if (!strcmp(cmd, "state")){
      d[0] = 0x03;
      dlen = 1;
      break;
    }
*/
    if (!strcmp(cmd, "settime") || !strcmp(cmd, "state") || !strcmp(cmd, "poll")){
      if (!strcmp(cmd, "poll")){
        cmdtype = 0;
      }
      if (!strcmp(cmd, "state")){
        cmdtype = 2;
      }
      if (!strcmp(cmd, "settime")){
        cmdtype = 3;
      }
      if (!param || param[0] == 0){

        if (RtcTime.valid) {
          d[0] = 0x03;
          d[1] = (RtcTime.year % 100);
          d[2] = RtcTime.month;
          d[3] = RtcTime.day_of_month;
          d[4] = RtcTime.hour;
          d[5] = RtcTime.minute;
          d[6] = RtcTime.second;
        } else {
          return -1;
        }

        // time_t now = 0;
        // struct tm timeinfo = { 0 };
        // time(&now);
        // localtime_r(&now, &timeinfo);
        // d[0] = 0x03;
        // d[1] = timeinfo.tm_year % 100;
        // d[2] = timeinfo.tm_mon + 1;
        // d[3] = timeinfo.tm_mday;
        // d[4] = timeinfo.tm_hour;
        // d[5] = timeinfo.tm_min;
        // d[6] = timeinfo.tm_sec;

      } else {
        d[0] = 0x03;
        BLE_ESP32::fromHex(d+1, param, 6);
      }
      dlen = 7;
      break;
    }

    if (!strcmp(cmd, "settemp")){
      cmdtype = 4;
      if (!param || param[0] == 0){
        return -1;
      }
      float ftemp = 20;
      sscanf(param, "%f", &ftemp);
      if (ftemp < 4.5) ftemp = 4.5;
      if (ftemp > 30) ftemp = 30;
      ftemp *= 2;
      uint8_t ctemp = (uint8_t) ftemp;
      d[0] = 0x41; d[1] = ctemp; dlen = 2;
      break;
    }

    if (!strcmp(cmd, "offset")){
      cmdtype = 5;
      if (!param || param[0] == 0){
        return 0;
      }
      float ftemp = 20;
      sscanf(param, "%f", &ftemp);
      ftemp *= 2;
      int8_t ctemp = (int8_t) ftemp;
      ctemp += 7;
      d[0] = 0x13; d[1] = ctemp; dlen = 2;
      break;
    }

    if (!strcmp(cmd, "setdaynight")){
      cmdtype = 6;
      if (!param || param[0] == 0){
        return -1;
      }
      if (!param2 || param2[0] == 0){
        return -1;
      }
      float ftemp = 15;
      sscanf(param, "%f", &ftemp);
      if (ftemp < 5) ftemp = 5;
      ftemp *= 2;
      uint8_t dtemp = (uint8_t) ftemp;

      ftemp = 20;
      sscanf(param2, "%f", &ftemp);
      if (ftemp < 5) ftemp = 5;
      ftemp *= 2;
      uint8_t ntemp = (uint8_t) ftemp;

      d[0] = 0x11; d[1] = dtemp; d[2] = ntemp; dlen = 3;
      break;
    }

    if (!strcmp(cmd, "setwindowtempdur")){
      cmdtype = 7;
      if (!param || param[0] == 0){
        return -1;
      }
      if (!param2 || param2[0] == 0){
        return -1;
      }
      float ftemp = 15;
      sscanf(param, "%f", &ftemp);
      if (ftemp < 5) ftemp = 5;
      ftemp *= 2;
      uint8_t temp = (uint8_t) ftemp;

      int dur = 0;
      sscanf(param2, "%d", &dur);
      d[0] = 0x14; d[1] = temp; d[2] = (dur/5); dlen = 3;
      break;
    }

    if (!strcmp(cmd, "setholiday")){
      cmdtype = 8;
      //40941C152402
      // 40 94
      if (!param || param[0] == 0){
        return -1;
      }
      if (!param2 || param2[0] == 0){
        return -1;
      }

      int yy = 0;
      int mm = 0;
      int dd = 0;
      int hour = 0;
      int min = 0;
      char *p = param;
      p = strtok(p, "-");
      if (!p || p[0] == 0) return -1;
      sscanf(p, "%d", &yy);
      p = strtok(nullptr, "-");
      if (!p || p[0] == 0) return -1;
      sscanf(p, "%d", &mm);
      p = strtok(nullptr, ",");
      if (!p || p[0] == 0) return -1;
      sscanf(p, "%d", &dd);
      p = strtok(nullptr, ":");
      if (!p || p[0] == 0) return -1;
      sscanf(p, "%d", &hour);
      p = strtok(nullptr, "");
      if (!p || p[0] == 0) return -1;
      sscanf(p, "%d", &min);

      min += hour*60;
      int tt = min / 30;

      float ftemp = 15;
      sscanf(param2, "%f", &ftemp);
      if (ftemp < 5) ftemp = 5;
      ftemp *= 2;
      uint8_t temp = (uint8_t) ftemp + 128;

      d[0] = 0x40;
      d[1] = temp;
      d[2] = dd;
      d[3] = yy;
      d[4] = tt;
      d[5] = mm;
      dlen = 6;
      break;
    }


    if (!strcmp(cmd, "boost"))    {
      cmdtype = 9;
      d[0] = 0x45; d[1] = 0x01;
      if (param && (!strcmp(param, "off") || param[0] == '0')){
        d[1] = 0x00;
      }
      dlen = 2; break;
    }
    if (!strcmp(cmd, "unboost"))  {
      cmdtype = 10;
      d[0] = 0x45; d[1] = 0x00; dlen = 2; break; }
    if (!strcmp(cmd, "lock"))     { cmdtype = 23; d[0] = 0x80; d[1] = 0x01;
      if (param && (!strcmp(param, "off") || param[0] == '0')){
        d[1] = 0x00;
      }
      dlen = 2; break;
    }
    if (!strcmp(cmd, "unlock"))   { cmdtype = 11; d[0] = 0x80; d[1] = 0x00; dlen = 2; break; }
    if (!strcmp(cmd, "auto"))     { cmdtype = 12; d[0] = 0x40; d[1] = 0x00; dlen = 2; break; }
    if (!strcmp(cmd, "manual"))   { cmdtype = 13; d[0] = 0x40; d[1] = 0x40; dlen = 2; break; }
    // this is basically 'cancel holiday' - mode auto does that.
    //if (!strcmp(cmd, "eco"))      { cmdtype = 14; d[0] = 0x40; d[1] = 0x80; dlen = 2; break; }
    if (!strcmp(cmd, "on"))       {
      int res = EQ3Send(addr, "manual", nullptr, nullptr, useAlias);
      char tmp[] = "30";
      int res2 = EQ3Send(addr, "settemp", tmp, nullptr, useAlias);
      return res2;
    }
    if (!strcmp(cmd, "off"))      {
      int res = EQ3Send(addr, "manual", nullptr, nullptr, useAlias);
      char tmp[] = "4.5";
      int res2 = EQ3Send(addr, "settemp", tmp, nullptr, useAlias);
      return res2;
    }
    if (!strcmp(cmd, "valve"))     { cmdtype = 17; d[0] = 0x41; d[1] = 0x3c;
      if (!param || param[0] == 0){
        return -1;
      }
      if ((!strcmp(param, "off") || param[0] == '0')){
        d[1] = 0x09;
      }
      dlen = 2; break;
    }
    if (!strcmp(cmd, "mode"))     { cmdtype = 18; d[0] = 0x40; d[1] = 0xff;// invlaid

      if (!param || param[0] == 0){
        return -1;
      }
      if (!strcmp(param, "auto")){
        d[1] = 0x00;
      }
      if (!strcmp(param, "manual") || !strcmp(param, "heat" )){
        d[1] = 0x40;
      }
      if (!strcmp(param, "on")) {
        int res = EQ3Send(addr, "manual", nullptr, nullptr, useAlias);
        char tmp[] = "30";
        int res2 = EQ3Send(addr, "settemp", tmp, nullptr, useAlias);
        return res2;
      }
      if (!strcmp(param, "off") || !strcmp(param, "cool") || !strcmp(param, "fan_only")) {
        int res = EQ3Send(addr, "manual", nullptr, nullptr, useAlias);
        char tmp[] = "4.5";
        int res2 = EQ3Send(addr, "settemp", tmp, nullptr, useAlias);
        return res2;
      }

      if (d[1] == 0xff){ // no valid mode selection found
        return -1;
      }
      // this is basically 'cancel holiday' - mode auto does that.
      //if (!strcmp(param, "eco")){
      //  d[1] = 0x80;
      //}
      dlen = 2; break;
    }
    if (!strcmp(cmd, "day"))      { cmdtype = 19; d[0] = 0x43; dlen = 1; break; }
    if (!strcmp(cmd, "night"))    { cmdtype = 20; d[0] = 0x44; dlen = 1; break; }

    if (!strcmp(cmd, "reqprofile"))     { cmdtype = 21;
      if (!param || param[0] == 0){
        return -1;
      }
      d[0] = 0x20; d[1] = atoi(param); dlen = 2;
      break;
    }

    if (!strcmp(cmd, "setprofile"))     { cmdtype = 22;
      if (!param || param[0] == 0){
        return -1;
      }
      if (!param2 || param2[0] == 0){
        return -1;
      }
      d[0] = 0x10; d[1] = atoi(param);

      // default
      uint8_t temps[7] = {0x22,0x22,0x22,0x22,0x22,0x22,0x22};
      uint8_t times[7] = {0x90,0x90,0x90,0x90,0x90,0x90,0x90};

      // 20.5-17:30,
      const char *p = strtok(param2, ",");
      int i = 0;
      while (p){
        float t = 17;
        int mm = 0;
        int hh = 24;
        sscanf(p, "%f-%d:%d", &t, &hh, &mm);
        t *= 2;
        temps[i] = (uint8_t) t;
        int time = hh*60+mm;
        time = time / 10;
        times[i] = time;
        p = strtok(nullptr, ",");
        i++;
        if (i >= 7) break;
      }

      // remaining left at 00 00
      for (int j = 0; j < 7; j++){
        d[2+j*2] = temps[j];
        d[2+j*2+1] = times[j];
      }

      dlen = 2+14;
      break;
    }

    break;
  } while(0);

  if (dlen){
    dlen = 16;
    return EQ3QueueOp(addr, d, dlen, cmdtype, useAlias);

    //return EQ3Operation(addr, d, dlen, 4);
  }

  return -1;
}


const char *responses[] = {
  PSTR("Done"),
  PSTR("queued"),
  PSTR("ignoredbusy"),
  PSTR("invcmd"),
  PSTR("cmdfail"),
  PSTR("invidx"),
  PSTR("invaddr")
};


int CmndTrvNext(int index, char *data){
  AddLog(LOG_LEVEL_DEBUG, PSTR("EQ3: cmd index: %d"), index);
  //simpletolower(data);

  switch(index){
    case 0:
    case 1: {

      char *p = strtok(data, " ");
      bool trigger = false;
      if (!strcmp(p, "reset")){
        retries = 0;
        for (int i = 0; i < EQ3_NUM_DEVICESLOTS; i++){
          EQ3Devices[i].timeoutTime = 0L;
        }
        return 0;
      }

      if (!strcmp(p, "scan")){
#ifdef EQ3_DEBUG
        AddLog(LOG_LEVEL_DEBUG, PSTR("EQ3: cmd: %s"), p);
#endif
        EQ3SendCurrentDevices();
        return 0;
      }
      if (!strcmp(p, "devlist")){
#ifdef EQ3_DEBUG
        AddLog(LOG_LEVEL_DEBUG, PSTR("EQ3: cmd: %s"), p);
#endif
        EQ3SendCurrentDevices();
        return 0;
      }

      // only allow one command in progress
      if (retries){
        //return 2;
      }


      int useAlias = 0;
      uint8_t addrbin[7];
      int addrres = BLE_ESP32::getAddr(addrbin, p);
      if (addrres){
        if (addrres == 2){
          AddLog(LOG_LEVEL_DEBUG, PSTR("EQ3: addr used alias: %s"), p);
          useAlias = 1;
        }
        NimBLEAddress addr(addrbin, addrbin[6]);

#ifdef EQ3_DEBUG
        //AddLog(LOG_LEVEL_INFO, PSTR("EQ3: cmd addr: %s -> %s"), p, addr.toString().c_str());
#endif
      } else {
        AddLog(LOG_LEVEL_ERROR, PSTR("EQ3: addr invalid: %s"), p);
        return 3;
      }

      // get next part of cmd
      char *cmd = strtok(nullptr, " ");
      if (!cmd){
        return 3;
      }

      char *param = strtok(nullptr, " ");
      char *param2 = nullptr;
      if (param){
        param2 = strtok(nullptr, " ");
      }
      int res = EQ3Send(addrbin, cmd, param, param2, useAlias);
      if (res > 0) {
        // succeeded to queue
        AddLog(LOG_LEVEL_ERROR, PSTR("EQ3: queued"));
        return 1;
      }

      if (res < 0) { // invalid in some way
        AddLog(LOG_LEVEL_ERROR, PSTR("EQ3: invalid"));
        return 3;
      }

      AddLog(LOG_LEVEL_ERROR, PSTR("EQ3: failed to queue"));
      // failed to queue
      return 4;
    } break;

    case 2:
      retries = 0;
      return 0;
      break;
  }

  return 4;
}

void CmndTrv(void) {
  int res = CmndTrvNext(XdrvMailbox.index, XdrvMailbox.data);
  ResponseCmndChar(responses[res]);
}

void CmndTrvPeriod(void) {
  if (XdrvMailbox.data_len > 0) {
    if (1 == XdrvMailbox.payload){
      seconds = 0;
    } else {
      EQ3Period = XdrvMailbox.payload;
      if (seconds > EQ3Period){
        seconds = EQ3Period;
      }
    }
  }
  ResponseCmndNumber(EQ3Period);
}

void CmndTrvRetries(void) {
  if (XdrvMailbox.data_len > 0) {
    EQ3Retries = XdrvMailbox.payload;
  }
  ResponseCmndNumber(EQ3Retries);
}

void CmndTrvOnlyAliased(void){
  if (XdrvMailbox.data_len > 0) {
    EQ3OnlyAliased = XdrvMailbox.payload;
  }
  ResponseCmndNumber(EQ3OnlyAliased);
}

void CmndTrvMatchPrefix(void){
  if (XdrvMailbox.data_len > 0) {
    EQ3MatchPrefix = XdrvMailbox.payload;
  }
  ResponseCmndNumber(EQ3MatchPrefix);
}

void CmndTrvMinRSSI(void){
  if (XdrvMailbox.data_len > 0) {
    trvMinRSSI = atoi(XdrvMailbox.data);
  }
  // signed number
  Response_P(PSTR("{\"%s\":%d}"), XdrvMailbox.command, trvMinRSSI);
}

void CmndTrvHideFailedPoll(void){
  if (XdrvMailbox.data_len > 0) {
    EQ3HideFailedPoll = XdrvMailbox.payload;
  }
  ResponseCmndNumber(EQ3HideFailedPoll);
}


#define EQ3_TOPIC "EQ3"
static char tmp[120];

bool mqtt_direct(){
  char stopic[TOPSZ];
  strncpy(stopic, XdrvMailbox.topic, TOPSZ);
  XdrvMailbox.topic[TOPSZ-1] = 0;

  AddLog(LOG_LEVEL_DEBUG, PSTR("EQ3: mqtt: %s:%s"), stopic, XdrvMailbox.data);

  char *items[10];
  char *p = stopic;
  int cnt = 0;
  do {
    items[cnt] = strtok(p, "/");
    cnt++;
    p = nullptr;
  } while (items[cnt-1]);
  cnt--; // repreents the number of items

  if (cnt < 4){ // not for us?
    //AddLog(LOG_LEVEL_INFO, PSTR("cnt: %d < 4"), cnt);
    return false;
  }

  for (int i = 0; i < cnt; i++){
    //AddLog(LOG_LEVEL_INFO, PSTR("cnt %d:%s"), i, items[i]);
  }

  int EQ3index = 0;
  int MACindex = 0;
  int CMDindex = 0;
  if (strcasecmp_P(items[cnt-3], PSTR(EQ3_TOPIC)) != 0) {
    //AddLog(LOG_LEVEL_INFO, PSTR("cnt-3 not %s"), PSTR(EQ3_TOPIC));
    if (strcasecmp_P(items[cnt-2], PSTR(EQ3_TOPIC)) != 0) {
      //AddLog(LOG_LEVEL_INFO, PSTR("cnt-2 not %s"), PSTR(EQ3_TOPIC));
      return false; // not for us
    } else {
      EQ3index = cnt-2;
      MACindex = cnt-1;
    }
  } else {
    EQ3index = cnt-3;
    MACindex = cnt-2;
    CMDindex = cnt-1;
  }

  int remains = 120;
  memset(tmp, 0, sizeof(tmp));
  p = tmp;
  uint8_t addr[7];
  int useAlias = BLE_ESP32::getAddr(addr, items[MACindex]);
  int res = 6; // invalid address/alias

  // if address or alias valid
  if (useAlias){
    strncpy(p, items[MACindex], remains-6);
    p += strlen(p);
    *(p++) = 0x20;
    remains = 120 - (p-tmp);

    if (CMDindex){
      strncpy(p, items[CMDindex], remains-6);
      p += strlen(p);
      *(p++) = 0x20;
      remains = 120 - (p-tmp);
    }

    strncpy(p, XdrvMailbox.data, remains-6);
    p += strlen(p);
    *(p++) = 0x20;
    remains = 120 - (p-tmp);
    *(p++) = 0;

    AddLog(LOG_LEVEL_DEBUG, PSTR("EQ3: mqtt->cmdstr %s"), tmp);
    res = CmndTrvNext(1, tmp);
  }

  // post result to stat/tas/EQ3/<MAC> {"result":"<string>"}
  EQ3SendResult(items[MACindex], responses[res]);

  return true;
}


///////////////////////////////////////////////
// starts a completely fresh MQTT message.
// sends ONE sensor's worth of HA discovery msg
const char EQ3_HA_DISCOVERY_TEMPLATE[] PROGMEM =
  "{\"availability\":[],\"device\":"
  "{\"identifiers\":[\"BLE%s\"],"
  "\"name\":\"%s\","
  "\"manufacturer\":\"tas\","
  "\"model\":\"%s\","
  "\"via_device\":\"%s\""
  "},"
  "\"dev_cla\":\"%s\","
  "\"expire_after\":600,"
  "\"json_attr_t\":\"%s\","
  "\"name\":\"%s_%s\","
  "\"state_topic\":\"%s\","
  "\"uniq_id\":\"%s_%s\","
  "\"unit_of_meas\":\"%s\","
  "\"val_tpl\":\"{{ value_json.%s }}\"}";

///////////TODO - unfinished.....
void EQ3DiscoveryOneEQ3(){
  // don't detect half-added ones here
  if (EQ3CurrentSingleSlot >= EQ3_NUM_DEVICESLOTS){
    // if we got to the end of the sensors, then don't send more
    return;
  }

#ifdef USE_HOME_ASSISTANT
  if(Settings->flag.hass_discovery){
    eq3_device_tag *p;
    do {
      p = &EQ3Devices[EQ3CurrentSingleSlot];
      if (0 == p->timeoutTime){
        EQ3CurrentSingleSlot++;
      }
    } while ((0 == p->timeoutTime) && (EQ3CurrentSingleSlot <= EQ3_NUM_DEVICESLOTS));

    if (EQ3CurrentSingleSlot >= EQ3_NUM_DEVICESLOTS){
      return;
    }

    // careful - a missing comma causes a crash!!!!
    // because of the way we loop?
    const char *classes[] = {
      "temperature",
      "temp",
      "°C",
      "signal_strength",
      "RSSI",
      "dB"
    };

    int datacount = (sizeof(classes)/sizeof(*classes))/3;

    if (p->nextDiscoveryData >= datacount){
      p->nextDiscoveryData = 0;
    }

    char DiscoveryTopic[80];
    const char *host = NetworkHostname();
    const char *devtype = PSTR("EQ3");
    char idstr[32];
    const char *alias = BLE_ESP32::getAlias(p->addr);
    const char *id = idstr;
    if (alias && *alias){
      id = alias;
    } else {
      sprintf(idstr, PSTR("%s%02x%02x%02x"),
            devtype,
            p->addr[3], p->addr[4], p->addr[5]);
    }

    char SensorTopic[60];
    sprintf(SensorTopic, "stat/%s/EQ3/%s",
      host, id);

    //int i = p->nextDiscoveryData*3;
    for (int i = 0; i < datacount*3; i += 3){
      if (!classes[i] || !classes[i+1] || !classes[i+2]){
        return;
      }

      ResponseClear();

    /*
    {"availability":[],"device":{"identifiers":["TasmotaBLEa4c1387fc1e1"],"manufacturer":"simon","model":"someBLEsensor","name":"TASBLEa4c1387fc1e1","sw_version":"0.0.0"},"dev_cla":"temperature","json_attr_t":"stat/tasmota_esp32/SENSOR","name":"TASLYWSD037fc1e1Temp","state_topic":"tele/tasmota_esp32/SENSOR","uniq_id":"Tasmotaa4c1387fc1e1temp","unit_of_meas":"°C","val_tpl":"{{ value_json.LYWSD037fc1e1.Temperature }}"}
    {"availability":[],"device":{"identifiers":["TasmotaBLEa4c1387fc1e1"],
    "name":"TASBLEa4c1387fc1e1"},"dev_cla":"temperature",
    "json_attr_t":"tele/tasmota_esp32/SENSOR",
    "name":"TASLYWSD037fc1e1Temp","state_topic":  "tele/tasmota_esp32/SENSOR",
    "uniq_id":"Tasmotaa4c1387fc1e1temp","unit_of_meas":"°C",
    "val_tpl":"{{ value_json.LYWSD037fc1e1.Temperature }}"}
    */

      ResponseAppend_P(EQ3_HA_DISCOVERY_TEMPLATE,
      //"{\"identifiers\":[\"BLE%s\"],"
        id,
      //"\"name\":\"%s\"},"
        id,
      //\"model\":\"%s\",
        devtype,
      //\"via_device\":\"%s\"
        host,
      //"\"dev_cla\":\"%s\","
        classes[i],
      //"\"json_attr_t\":\"%s\"," - the topic the sensor publishes on
        SensorTopic,
      //"\"name\":\"%s_%s\"," - the name of this DATA
        id, classes[i+1],
      //"\"state_topic\":\"%s\","  - the topic the sensor publishes on?
        SensorTopic,
      //"\"uniq_id\":\"%s_%s\"," - unique for this data,
        id, classes[i+1],
      //"\"unit_of_meas\":\"%s\"," - the measure of this type of data
        classes[i+2],
      //"\"val_tpl\":\"{{ value_json.%s }}") // e.g. Temperature
        classes[i+1]
          //
      );

      sprintf(DiscoveryTopic, "homeassistant/sensor/%s/%s/config",
        id, classes[i+1]);

      MqttPublish(DiscoveryTopic);
      p->nextDiscoveryData++;
      //vTaskDelay(100/ portTICK_PERIOD_MS);
    }
  } // end if hass discovery
  //AddLog_P(LOG_LEVEL_DEBUG, PSTR("M32: %s: show some %d %s"), D_CMND_MI32, MI32.mqttCurrentSlot, TasmotaGlobal.mqtt_data);
#endif //USE_HOME_ASSISTANT

}

} // end namespace EQ3_ESP32

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv85(uint32_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_INIT:
      EQ3_ESP32::EQ3Init();
      break;
    case FUNC_EVERY_50_MSECOND:
      EQ3_ESP32::EQ3Every50mSecond();
      break;
    case FUNC_EVERY_SECOND:
      EQ3_ESP32::EQ3EverySecond(false);
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(EQ3_ESP32::kEQ3_Commands, EQ3_ESP32::EQ3_Commands);
      break;
    case FUNC_MQTT_DATA:
      //AddLog(LOG_LEVEL_INFO, PSTR("topic %s"), XdrvMailbox.topic);
      result = EQ3_ESP32::mqtt_direct();
      break;
    case FUNC_JSON_APPEND:
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      EQ3_ESP32::EQ3Show();
      break;
#endif  // USE_WEBSERVER
    case FUNC_ACTIVE:
      result = true;
      break;
    }
  return result;
}
#endif  //
#endif  // ESP32

#endif
#endif  // CONFIG_IDF_TARGET_ESP32
