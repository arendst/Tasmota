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
  1.0.1.1 20251204  changed - display RSSI in general format "xx% (-yy dBm)"
                              view on UI only when BLE enabled
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
trvReset - clear device list
trvDevList - report seen devices.  Active scanning required, not passive, as it looks for names
trvScan - same as trvDevList


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

#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C5 || CONFIG_IDF_TARGET_ESP32C6 || CONFIG_IDF_TARGET_ESP32S3
#ifdef USE_EQ3_ESP32
#ifdef ESP32                       // ESP32 only. Use define USE_HM10 for ESP8266 support
#ifdef USE_BLE_ESP32

#define XDRV_85                    85
#define D_CMND_EQ3 "TRV"

// uncomment for more debug messages
//#define EQ3_DEBUG

// Simulation - uncomment to simulate a device
//#define EQ3_SIMULATION
#ifdef EQ3_SIMULATION
#define TESTADDR1 (uint8_t[]){0x11, 0x11, 0x11, 0x11, 0x11, 0x11}
#endif // EQ3_SIMULATION

namespace EQ3_ESP32 {

int EQ3Send(const uint8_t* addr, uint8_t CmdIdx, const char* param1, const char* param2, int useAlias);
int EQ3GenericOpCompleteFn(BLE_ESP32::generic_sensor_t *pStruct);

void CmndTrv(void);
void CmndTrvPeriod(void);
void CmndTrvRetries(void);
void CmndTrvOnlyAliased(void);
void CmndTrvMatchPrefix(void);
void CmndTrvMinRSSI(void);
void CmndTrvHideFailedPoll(void);
void CmndTrvReset(void);
void CmndTrvDevList(void);

constexpr const char kEQ3_Commands[] = D_CMND_EQ3 "|"
  "|"
  "Period|"
  "Retries|"
  "OnlyAliased|"
  "MatchPrefix|"
  "MinRSSI|"
  "HideFailedPoll|"
  "Reset|"
  "DevList|"
  "Scan";  // same as devlist

constexpr void (*EQ3_Commands[])(void) = {
  &CmndTrv,
  &CmndTrvPeriod,
  &CmndTrvRetries,
  &CmndTrvOnlyAliased,
  &CmndTrvMatchPrefix,
  &CmndTrvMinRSSI,
  &CmndTrvHideFailedPoll,
  &CmndTrvReset,
  &CmndTrvDevList,
  &CmndTrvDevList  // trvScan
};

enum TrvSubCommand : uint8_t {
  TRV_POLL = 0,
  TRV_RAW,
  TRV_STATE,
  TRV_SETTIME,
  TRV_SETTEMP,
  TRV_OFFSET,
  TRV_SETDAYNIGHT,
  TRV_SETWINDOWTEMPDUR,
  TRV_SETHOLIDAY,
  TRV_BOOST,
  TRV_UNBOOST,
  TRV_LOCK,
  TRV_UNLOCK,
  TRV_AUTO,
  TRV_MANUAL,
  TRV_ECO,
  TRV_ON,
  TRV_OFF,
  TRV_HEAT,
  TRV_COOL,
  TRV_FANONLY,
  TRV_VALVE,
  TRV_MODE,
  TRV_DAY,
  TRV_NIGHT,
  TRV_REQPROFILE,
  TRV_SETPROFILE,
  TRV_COUNT, // Maxcount - Must match to TrvSubCmds
  TRV_UNKNOWN = 255 // Helper for error
};

constexpr const char* const TrvSubCmds[] = {
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
  "lock",
  "unlock",
  "auto",
  "manual",
  "eco",
  "on",
  "off",
  "heat",
  "cool",
  "fan_only",
  "valve",
  "mode",
  "day",
  "night",
  "reqprofile",
  "setprofile"
};

enum TrvResponse : uint8_t {
  TRV_DONE = 0,
  TRV_QUEUED,
  TRV_IGNOREDBUSY,
  TRV_INVCMD,
  TRV_CMDFAIL,
  TRV_INVADDR
};
 
constexpr const char* const TrvResponses[] = {
  "Done",
  "queued",
  "ignoredbusy",
  "invcmd",
  "cmdfail",
  "invaddr"
};

const uint8_t *const macprefixes[] = {
  (uint8_t *)"\x00\x1a\x22"
};

constexpr const char EQ3_Svc[]         = "3e135142-654f-9090-134a-a6ff5bb77046";
constexpr const char EQ3_rw_Char[]     = "3fa4585a-ce4a-3bad-db4b-b8df8179ea09";
constexpr const char EQ3_notify_Char[] = "d0e8434d-cd29-0996-af41-6c90f4e0eb2a";

constexpr const char* const EQ3Names[] = {
  "CC-RT-BLE",
  "CC-RT-BLE-EQ",
  "CC-RT-M-BLE"
};

struct eq3_device_t {
  uint64_t timeoutTime;      // 8 Bytes
  float TargetTemp;          // 4 Bytes
  uint32_t lastStatusTime;   // 4 Bytes
  int8_t RSSI;               // 1 Byte
  uint8_t nextDiscoveryData; // 1 Byte
  uint8_t DutyCycle;         // 1 Byte
  uint8_t lastStatusLen;     // 1 Byte
  bool pairing;              // 1 Byte
  bool Battery;              // 1 Byte
  uint8_t addr[6];           // 6 Bytes
  uint8_t lastStatus[16];    // 16 Bytes
};

/*********************************************************************************************\
 * variables to control operation
\*********************************************************************************************/
int retries = 0;
// allow 240s before timeout of sa device - based on that we restart BLE if we don't see adverts for 120s
#define EQ3_TIMEOUT 240L

uint8_t pairingaddr[6] = {};
char pairingserial[20];
bool pairing = false;

#define EQ3_NUM_DEVICESLOTS 16
eq3_device_t EQ3Devices[EQ3_NUM_DEVICESLOTS];
SemaphoreHandle_t EQ3mutex = nullptr;

uint16_t EQ3Period = 300;
int EQ3Retries = 4;
uint8_t EQ3OnlyAliased = 0;
uint8_t EQ3MatchPrefix = 1;
bool opInProgress = false;
int EQ3CurrentSingleSlot = 0;

uint8_t EQ3TopicStyle = 1;
uint8_t EQ3HideFailedPoll = 1;
int8_t trvMinRSSI = -99;

// control of timing of sending polling.
// we leave an interval between polls to allow scans to take place
uint8_t nextEQ3Poll = 0;
uint16_t NextPollSeconds = 20;

struct op_t {
  uint8_t addr[6];
  uint8_t towrite[16];
  uint8_t writelen;
  uint8_t cmdtype;
};

std::deque<std::unique_ptr<EQ3_ESP32::op_t>> opQueue;

/*********************************************************************************************\
 * Functions
\*********************************************************************************************/

const char *addrStr(const uint8_t *addr, int useAlias = 0) {
  static char addrstr[32];

  const char *id = nullptr;
  if (useAlias) {
    id = BLE_ESP32::getAlias(addr);
  }
  if (!id || !*id) {
    id = addrstr;
    BLE_ESP32::dump(addrstr, 13, addr, 6);
  }
  return id;
}

uint8_t TrvCmdToIdx(const char* input) {
  if (!input || !*input) return TRV_UNKNOWN;
  for (uint8_t i = 0; i < TRV_COUNT; i++)
    if (!strcasecmp(input, TrvSubCmds[i])) return i;
  return TRV_UNKNOWN;
}

const char* IdxToTrvCmd(uint8_t index) {
  if (index >= TRV_COUNT) return "unknown";
  return TrvSubCmds[index];
}

char *topicPrefix(int prefix, const uint8_t *addr, int useAlias) {
  static char stopic[TOPSZ];
  const char *id = addrStr(addr, useAlias);
  if (!EQ3TopicStyle) {
    GetTopic_P(stopic, prefix, TasmotaGlobal.mqtt_topic, "");
    strlcat(stopic, "EQ3/", sizeof(stopic));
    strlcat(stopic, id, sizeof(stopic));
  } else {
    char p[] = "EQ3";
    GetTopic_P(stopic, prefix, p, id);
  }
  return stopic;
}

bool matchPrefix(const uint8_t *addr) {
  for (const auto *prefix : macprefixes) {
    if (!memcmp(addr, prefix, 3)) return true;
  }
  return false;
}

bool EQ3Operation(const uint8_t *MAC, const uint8_t *data, int datalen, int cmdtype, int retries_in = 0) {
  BLE_ESP32::generic_sensor_t *op = nullptr;

  // ALWAYS use this function to create a new one.
  int res = BLE_ESP32::newOperation(&op);
  if (!res) {
    AddLog(LOG_LEVEL_ERROR, "EQ3: %s: Can't get a newOperation \"%s\" from BLE", addrStr(MAC, cmdtype & 0x80), IdxToTrvCmd(cmdtype & 0x7f));
    retries = 0;
    return 0;
  } else {
#ifdef EQ3_DEBUG
    AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], "EQ3: %s: Got a newOperation \"%s\" from BLE", addrStr(MAC, cmdtype & 0x80), IdxToTrvCmd(cmdtype & 0x7f));
#endif
  }

  NimBLEAddress addr((uint8_t *)MAC,0); //type 0 is public
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
  if (!res) {
    // if it fails to add to the queue, do please delete it
    BLE_ESP32::freeOperation(&op);
    AddLog(LOG_LEVEL_ERROR, "EQ3: %s: Failed to queue new operation \"%s\" - deleted", addrStr(MAC, cmdtype & 0x80), IdxToTrvCmd(cmdtype & 0x7f));
    retries = 0;
  } else {
    if (retries_in) {
      retries = retries_in;
    }
  }

  return res;
}

void EQ3DoOp() {
  if (!opInProgress && !opQueue.empty()) {
    op_t* op = opQueue.front().get();

/// Simulation
#ifdef EQ3_SIMULATION
    if (!memcmp(op->addr, TESTADDR1, 6)) {
      AddLog(LOG_LEVEL_DEBUG, "EQ3: %s: Operation 1/%u \"%s\" only simulated -> removed", addrStr(op->addr, (op->cmdtype & 0x80)), opQueue.size(), IdxToTrvCmd(op->cmdtype & 0x7f));
      opQueue.pop_front(); 
      return;
    }
#endif // EQ3_SIMULATION

    if (EQ3Operation(op->addr, op->towrite, op->writelen, op->cmdtype, EQ3Retries)) {
      opInProgress = true;
      AddLog(LOG_LEVEL_DEBUG, "EQ3: %s: Operation 1/%u \"%s\" processing", addrStr(op->addr, (op->cmdtype & 0x80)), opQueue.size(), IdxToTrvCmd(op->cmdtype & 0x7f));
    } else {
      AddLog(LOG_LEVEL_ERROR, "EQ3: %s: Operation 1/%u \"%s\" not started -> removed", addrStr(op->addr, (op->cmdtype & 0x80)), opQueue.size(), IdxToTrvCmd(op->cmdtype & 0x7f));
      opQueue.pop_front(); 
    }
  }
}

int EQ3QueueOp(const uint8_t *MAC, const uint8_t *data, int datalen, int cmdtype, int useAlias) {
  auto newop = std::make_unique<op_t>();
  memcpy(newop->addr, MAC, 6);
  memcpy(newop->towrite, data, datalen);
  newop->writelen = datalen;
  newop->cmdtype = cmdtype | (useAlias ? 0x80 : 0);
  size_t qlen = opQueue.size() + 1;
  AddLog(LOG_LEVEL_DEBUG, "EQ3: %s: Operation 1/%u \"%s\" queued", addrStr(newop->addr, (newop->cmdtype & 0x80)), qlen, IdxToTrvCmd(newop->cmdtype & 0x7f));
  opQueue.push_back(std::move(newop));
  EQ3DoOp();
  return qlen;
}

int EQ3ParseOp(BLE_ESP32::generic_sensor_t *op, bool success, int retries) {
  int res = 0;
  opInProgress = false;
  ResponseClear();

  uint8_t addrev[6];
  const uint8_t *native = op->addr.getVal();
  memcpy(addrev, native, 6);
  BLE_ESP32::ReverseMAC(addrev);

  eq3_device_t *eq3 = nullptr;

  int free = -1;
  for (int i = 0; i < EQ3_NUM_DEVICESLOTS; i++) {
    if (!memcmp(EQ3Devices[i].addr, addrev, 6)) {
      eq3 = &EQ3Devices[i];
      break;
    }
  }

  int cmdtype = (((uint32_t)op->context) & 0xff);
  int useAlias = cmdtype & 0x80;
  cmdtype &= 0x7f;

  ResponseAppend_P("{");
  ResponseAppend_P("\"cmd\":\"%s\"", IdxToTrvCmd(cmdtype));
  ResponseAppend_P(",\"result\":\"%s\"", success ? "ok" : "fail");
  ResponseAppend_P(",\"MAC\":\"%s\"", addrStr(addrev));
  const char *host = NetworkHostname();
  ResponseAppend_P(",\"tas\":\"%s\"", host);
  if (cmdtype == TRV_RAW) {
    char raw[40];
    BLE_ESP32::dump(raw, 40, op->dataNotify, op->notifylen);
    ResponseAppend_P(",\"raw\":\"%s\"", raw);
  }

  uint8_t *status = {0};
  uint8_t statlen = 0;
  uint32_t stattime = 0;

  if (success) {
    if ((op->notifylen >= 6) && (op->dataNotify[0] == 2) && (op->dataNotify[1] == 1)) {
      if (eq3) {
        memcpy(eq3->lastStatus, op->dataNotify, (op->notifylen <= 10)?op->notifylen:16);
        eq3->lastStatusLen = (op->notifylen <= 10)?op->notifylen:16;
        eq3->lastStatusTime = UtcTime();
      }
    }

    status = op->dataNotify;
    statlen = op->notifylen;
    stattime = UtcTime();
  }

  if (eq3) {
    status = eq3->lastStatus;
    statlen = eq3->lastStatusLen;
    stattime = eq3->lastStatusTime;
    ResponseAppend_P(",\"RSSI\":%d", eq3->RSSI);
  }

  if ((statlen >= 6) && (status[0] == 2) && (status[1] == 1)) {
    ResponseAppend_P(",\"stattime\":%u", stattime);
    eq3->TargetTemp = (float)status[5] / 2;
    ResponseAppend_P(",\"temp\":%1_f", &(eq3->TargetTemp));
    eq3->DutyCycle = status[3];
    ResponseAppend_P(",\"posn\":%d", eq3->DutyCycle);
    int stat = status[2];
    ResponseAppend_P(",\"mode\":");
    switch (stat & 3) {
      case 0:
        ResponseAppend_P("\"auto\"");
        break;
      case 1:
        ResponseAppend_P("\"manual\"");
        break;
      case 2:
        ResponseAppend_P("\"holiday\"");
        break;
      case 3:
        ResponseAppend_P("\"manualholiday\"");
        break;
    }

    ResponseAppend_P(",\"hassmode\":");
    do {
      //HASS allowed modes [“auto”, “off”, “cool”, “heat”, “dry”, “fan_only”]
      //0201283B042A
      // If its in auto or holiday, set to auto
      if ((stat & 3) == 0) {
        ResponseAppend_P("\"auto\"");
        break;
      }
      // If its in manual and 4.5°C, set to off
      if (((stat & 3) == 1) && (status[5] == 9)) {
        ResponseAppend_P("\"off\"");
        break;
      }
      // If its in manual above 4.5°C and valve is open, set to heat
      if (((stat & 3) == 1) && (status[5] > 9) && (status[3] > 0)) {
        ResponseAppend_P("\"heat\"");
        break;
      }
      // If its in manual above 4.5°C and valve is closed, set to off
      if (((stat & 3) == 1) && (status[5] > 9)) {
        ResponseAppend_P("\"off\"");
        break;
      }
      //Fallback off
      ResponseAppend_P("\"off\"");
      break;
    } while (0);

    ResponseAppend_P(",\"boost\":\"%s\"", (stat & 4) ? "active" : "inactive");
    ResponseAppend_P(",\"dst\":\"%s\"", (stat & 8) ? "set" : "unset");
    ResponseAppend_P(",\"window\":\"%s\"", (stat & 16) ? "open" : "closed");
    ResponseAppend_P(",\"state\":\"%s\"", (stat & 32) ? "locked" : "unlocked");
    eq3->Battery = stat & 128;
    ResponseAppend_P(",\"battery\":\"%s\"", eq3->Battery ? "LOW" : "GOOD");
  }

  if ((statlen >= 10) && (status[0] == 2) && (status[1] == 1)) {
    int mm = status[8] * 30;
    int hh = mm / 60;
    mm = mm % 60;
    ResponseAppend_P(",\"holidayend\":\"%02d-%02d-%02d %02d:%02d\"", status[7], status[9], status[6], hh, mm);

    if (statlen >= 15) {
      float f_temp;
      f_temp = ((float)status[10]) /  2;
      ResponseAppend_P(",\"windowtemp\":%1_f", &f_temp);
      ResponseAppend_P(",\"windowdur\":%d", ((int)status[11]) * 5);
      f_temp = ((float)status[12]) / 2;
      ResponseAppend_P(",\"day\":%1_f", &f_temp);
      f_temp = ((float)status[13]) / 2;
      ResponseAppend_P(",\"night\":%1_f", &f_temp);
      f_temp = ((float)status[14] - 7) / 2;
      ResponseAppend_P(",\"offset\":%1_f", &f_temp);
    }
  }

  if (success) {
    // now to parse other data - this may not have been a stat message
    if ((op->notifylen >= 3) && (op->dataNotify[0] == 2) && (op->dataNotify[1] == 2)) {
      ResponseAppend_P(",\"profiledayset\":%d", op->dataNotify[2]);
    }

    if ((op->notifylen >= 16) && (op->dataNotify[0] == 0x21)) {
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
      ResponseAppend_P(",\"profileday%d\":\"", op->dataNotify[1]);
      uint8_t *data = op->dataNotify + 2;
      for (int i = 0; i < 7; i++) {
        float t = *(data++);
        t /= 2;
        int mm = *(data++);
        mm *= 10;
        int hh = mm / 60;
        mm = mm % 60;
        ResponseAppend_P("%1_f-%02d:%02d", &t, hh, mm);
        // stop if the last one is 24.
        if (hh == 24) {
          break;
        }

        if (i < 6) {
          ResponseAppend_P(",");
        }
      }
      ResponseAppend_P("\"");
    }

    res = 1;
  }

  ResponseAppend_P("}");

  int type = STAT;
  if (cmdtype) {
    type = STAT;
  } else {
    // it IS a poll command
    if (EQ3HideFailedPoll) {
      if (!success) {
        AddLog(LOG_LEVEL_DEBUG, "EQ3: %s: Poll fail not sent because EQ3HideFailedPoll", addrStr(addrev));
        return res;
      }
    }
  }

  char *topic = topicPrefix(type, addrev, useAlias);
  MqttPublish(topic, false);
  return res;
}

int EQ3GenericOpCompleteFn(BLE_ESP32::generic_sensor_t *op) {
  const bool is_failed = (op->state <= GEN_STATE_FAILED);
  const uint32_t context = (uintptr_t)op->context;

  uint8_t addrev[6];
  std::copy_n(op->addr.getVal(), 6, addrev);
  BLE_ESP32::ReverseMAC(addrev);

  if (is_failed && retries > 1) {
    retries--;

    if (EQ3Operation(addrev, op->dataToWrite, op->writelen, (int)context)) {
      AddLog(LOG_LEVEL_INFO, "EQ3: %s: Operation 1/%u \"%s\" failed - retries left: %d - State: %d", addrStr(addrev), opQueue.size(), IdxToTrvCmd(context & 0x7f), retries, op->state);
      opInProgress = true;
      return 0; 
    }
    retries = 0; 
  }

  if (is_failed) {
    AddLog(LOG_LEVEL_ERROR, "EQ3: %s: Operation 1/%u \"%s\" final fail - State: %d", addrStr(addrev), opQueue.size(), IdxToTrvCmd(context & 0x7f), op->state);
  } else {
    AddLog(LOG_LEVEL_DEBUG, "EQ3: %s: Operation 1/%u \"%s\" done", addrStr(addrev), opQueue.size(), IdxToTrvCmd(context & 0x7f));
  }
  
  EQ3ParseOp(op, !is_failed, 0);
  retries = 0;
  opInProgress = false;

  if (!opQueue.empty()) opQueue.pop_front();

  if (opQueue.empty()) {
    AddLog(LOG_LEVEL_DEBUG, "EQ3: %s: Operation queue is empty", addrStr(addrev));
  }

  return 0;
}

/*********************************************************************************************\
 * Functons actualy called from within the BLE task
\*********************************************************************************************/

int ispairing2(const uint8_t *payload, int len, char *name, int nameLen, char *serial, int serialLen ) {
  while (len) {
    int l = *payload;
    //BLE_ESP32::dump(temp, 40, payload, l+1);
    //AddLog(LOG_LEVEL_ERROR, PSTR("EQ3: %s"), temp);

    payload++;
    len--;
    if (len < l) {
      //AddLog(LOG_LEVEL_ERROR, PSTR("EQ3: part len er %d<%d"),len, l);
      return 0;
    }
    switch (*payload) {
      case 0xff: {// parse the EQ3 advert payload looking for nnFF01ssssssss
        payload++;
        len--;
        l--;
        if (*payload == 1) {
          payload++;
          len--;
          l--;
          //char serialstr[20];
          //strncpy(serialstr, (const char *)payload, l);
          //AddLog(LOG_LEVEL_DEBUG, PSTR("EQ3: adv part FF01 detected %s"), serialstr);
          // we don;t use these, but that's what they seem to be....
          strncpy(serial, (const char *)payload, tmin(l, serialLen));
          serial[serialLen - 1] = 0;
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
        if (*payload == 1) {
          payload++;
          len--;
          l--;
          //char serialstr[20];
          //strncpy(serialstr, (const char *)payload, l);
          //AddLog(LOG_LEVEL_DEBUG, PSTR("EQ3: adv part FF01 detected %s"), serialstr);
          // we don;t use these, but that's what they seem to be....
          strncpy(serial, (const char *)payload, tmin(l, serialLen));
          name[nameLen - 1] = 0;
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

int ispairing(const uint8_t *payload, int len) {
  //char temp[40];
  //BLE_ESP32::dump(temp, 40, payload, len);
  //AddLog(LOG_LEVEL_DEBUG, PSTR("EQ3: pair%d %s"), len, temp);
  while (len) {
    int l = *payload;
    //BLE_ESP32::dump(temp, 40, payload, l+1);
    //AddLog(LOG_LEVEL_ERROR, PSTR("EQ3: %s"), temp);

    payload++;
    len--;
    if (len < l) {
      //AddLog(LOG_LEVEL_ERROR, PSTR("EQ3: part len er %d<%d"),len, l);
      return 0;
    }
    if (*payload == 0xff) {
      payload++;
      len--;
      l--;
      if (*payload == 1) {
        payload++;
        len--;
        l--;
        //char serialstr[20];
        //strncpy(serialstr, (const char *)payload, l);
        //AddLog(LOG_LEVEL_DEBUG, PSTR("EQ3: adv part FF01 detected %s"), serialstr);
        // we don;t use these, but that's what they seem to be....
        const uint8_t *serial = payload;
        uint8_t serialLen = l;
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

void TaskEQ3AddDevice(int8_t RSSI, const uint8_t* addr, char *serial) {
  eq3_device_t* targetDevice = nullptr;
  eq3_device_t* firstFreeSlot = nullptr;

  if (serial && *serial && !pairing) {
    memcpy(pairingaddr, addr, 6);
    strncpy(pairingserial, serial, sizeof(pairingserial));
    pairingserial[sizeof(pairingserial) - 1] = 0;
    pairing = true;
  }

  for (auto& device : EQ3Devices) {
    if (!memcmp(addr, device.addr, 6)) {
      targetDevice = &device;
      break; 
    }
    if (!device.timeoutTime && !firstFreeSlot) {
      firstFreeSlot = &device;
    }
  }

  if (!targetDevice) {
    if (firstFreeSlot) {
      targetDevice = firstFreeSlot;
      NextPollSeconds = 1;
#ifdef EQ3_DEBUG
      AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_INFO], "EQ3: %s: New TRV at #%d", addrStr(addr), targetDevice - EQ3Devices);
#endif
    } else {
      AddLog(LOG_LEVEL_ERROR, "EQ3: %s: All %d slots used", addrStr(addr), EQ3_NUM_DEVICESLOTS);
      return;
    }
  }

  targetDevice->timeoutTime = esp_timer_get_time() + 1000000ULL * EQ3_TIMEOUT;
  memcpy(targetDevice->addr, addr, 6);
  targetDevice->RSSI = RSSI;
  targetDevice->pairing = (serial && *serial);
}

int TaskEQ3advertismentCallback(BLE_ESP32::ble_advertisment_t *pStruct)
{
  // we will try not to use this...
  const BLEAdvertisedDevice *advertisedDevice = pStruct->advertisedDevice;

  std::string sname = advertisedDevice->getName();

  bool found = false;
  const char *nameStr = sname.c_str();
  int8_t RSSI = pStruct->RSSI;
  const uint8_t *addr = pStruct->addr;


  const char *alias = BLE_ESP32::getAlias(addr);
  if (EQ3OnlyAliased) {
    // ignore unless we have an alias.
    if (!alias || !*alias) {
      return 0;
    }
  }
  if (!alias) alias = "";

  for (size_t i = 0; i < sizeof(EQ3Names) / sizeof(*EQ3Names); i++) {
    if (!strcmp(nameStr, EQ3Names[i])) {
      found = true;
      break;
    }
  }

  if (!found && !strncmp(alias, "EQ3", 3)) {
    found = true;
  }

  // if the addr matches the EQ3 mfg prefix, add it?
  if (!found && EQ3MatchPrefix && matchPrefix(addr)) {
    found = true;
  }

  if (!found) return 0;

#ifdef EQ3_DEBUG
  AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], "EQ3: %s: Device seen", addrStr(addr));
#endif

  uint8_t* payload = (uint8_t *)advertisedDevice->getPayload().data();
  size_t payloadlen = advertisedDevice->getPayload().size();

  char name[20] = {0};
  char serial[20] = {0};
  bool pairing = false;
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
  AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_INFO], "EQ3: init: request callbacks");
#endif

  EQ3Period = tmax(Settings->tele_period, EQ3_NUM_DEVICESLOTS);

  return;
}

/***********************************************************************\
 * Regular
\***********************************************************************/

void EQ3Every50mSecond() {

}

/**
 * @brief Main loop of the driver, "high level"-loop
 *
 */

void EQ3EverySecond(void) {

/// Handle pairing ////
  if (pairing) {
    Response_P("{\"pairing\":\"%s\",\"serial\":\"%s\"}", addrStr(pairingaddr), pairingserial);
    char addrstr[4 + 8 * 2 + 2] = "EQ3/";
    BLE_ESP32::dump(&addrstr[4], 8 * 2 + 2, pairingaddr, 6);
    char *topic = topicPrefix(STAT, pairingaddr, 1);
    MqttPublish(topic, false);
    pairing = false;
  }

/// Check for timeout and cleanup devices ////
  for (auto& device : EQ3Devices) {
    if (device.timeoutTime && device.timeoutTime < esp_timer_get_time()) {
      AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], "EQ3: %s: timed out", addrStr(device.addr));
      device = eq3_device_t{}; 
    }
  }

/// Handle polling ////
  if (NextPollSeconds) NextPollSeconds--;
  if (!NextPollSeconds && EQ3Period && opQueue.empty() && !opInProgress && !pairing) {
    uint8_t activeDevices = 0;
    for (const auto& dev : EQ3Devices) {
      if (dev.timeoutTime) activeDevices++;
    }
    if (activeDevices) {
      for (uint8_t i = 0; i < EQ3_NUM_DEVICESLOTS; i++) {
        uint8_t currentIdx = (nextEQ3Poll + i) % EQ3_NUM_DEVICESLOTS;
        auto& device = EQ3Devices[currentIdx];
        if (!device.timeoutTime) continue;
        if (device.RSSI < trvMinRSSI) {
          AddLog(LOG_LEVEL_DEBUG, "EQ3: %s: RSSI %d < min %d, poll suppressed", addrStr(device.addr), device.RSSI, trvMinRSSI);
          continue; 
        }
        EQ3Send(device.addr, TRV_POLL, nullptr, nullptr, 1);
        nextEQ3Poll = (currentIdx + 1) % EQ3_NUM_DEVICESLOTS;
        NextPollSeconds = tmax(EQ3Period / activeDevices, 3);
        break;
      }
    } else {
      NextPollSeconds = EQ3Period; 
    }
  }

//// start next op now, if we have any queued ////
  EQ3DoOp();

//// Simulation ////
#ifdef EQ3_SIMULATION
  TaskEQ3AddDevice(-RtcTime.second - 30, TESTADDR1, nullptr);
  if (TasmotaGlobal.uptime < 120) {
    for (auto& dev : EQ3Devices) {
      if (!memcmp(dev.addr, TESTADDR1, 6)) dev.lastStatusTime = UtcTime();
    }
  }
#endif // EQ3_SIMULATION

}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void EQ3SendCurrentDevices(void) {
  bool added = false;
  ResponseClear();
  ResponseAppend_P("{\"devices\":{");
  for (const auto& device : EQ3Devices) {
    if (!device.timeoutTime) continue;
    if (added) ResponseAppend_P(",");
    ResponseAppend_P("\"%s\":%d", addrStr(device.addr), device.RSSI);
    added = true;
  }
  ResponseAppend_P("}}");
  MqttPublishPrefixTopic_P(STAT, "EQ3", false);
}

int EQ3SendResult(char *requested, const char *result) {
  // send the result
  Response_P("{\"result\":\"%s\"}", result);
  static char stopic[TOPSZ];
  GetTopic_P(stopic, STAT, TasmotaGlobal.mqtt_topic, "");
  strlcat(stopic, "EQ3/", sizeof(stopic));
  strlcat(stopic, requested, sizeof(stopic));
  MqttPublish(stopic, false);
  return 0;
}

#ifdef USE_WEBSERVER
constexpr const char HTTP_EQ3_TYPE[]        = "{s}%s " D_NEOPOOL_TYPE "{m}EQ3{e}";
constexpr const char HTTP_EQ3_MAC[]         = "{s}%s " D_MAC_ADDRESS "{m}%s{e}";
constexpr const char HTTP_EQ3_RSSI[]        = "{s}%s " D_RSSI "{m}%d%% (%d dBm){e}";
constexpr const char HTTP_EQ3_TEMPERATURE[] = "{s}%s " D_THERMOSTAT_SET_POINT "{m}%*_f " D_UNIT_DEGREE "%c{e}";
constexpr const char HTTP_EQ3_DUTY_CYCLE[]  = "{s}%s " D_THERMOSTAT_VALVE_POSITION "{m}%d " D_UNIT_PERCENT "{e}";
constexpr const char HTTP_EQ3_BATTERY[]     = "{s}%s " D_BATTERY "{m}%s{e}";

void EQ3Show(void)
{
  if (!Settings->flag5.mi32_enable) return;

  char c_unit = D_UNIT_CELSIUS[0]; // ToDo: Check if fahrenheit is possible -> temp_format==TEMP_CELSIUS ? D_UNIT_CELSIUS[0] : D_UNIT_FAHRENHEIT[0];
  bool FirstSensorShown = false;

  for (const auto& device : EQ3Devices) {
    if (device.timeoutTime) {
      if (FirstSensorShown) WSContentSend_P(HTTP_SNS_HR_THIN);
      FirstSensorShown = true;
      const char *label;
      const char *alias = BLE_ESP32::getAlias(device.addr);
      char tlabel[8];
      if (alias && *alias) {
        label = alias;
        WSContentSend_P(HTTP_EQ3_TYPE, label);
      } else {
        snprintf(tlabel, sizeof(tlabel), "EQ3-%d", (&device - EQ3Devices) + 1);
        label = tlabel;
      }
      WSContentSend_P(HTTP_EQ3_MAC, label, addrStr(device.addr));
      WSContentSend_PD(HTTP_EQ3_RSSI, label, WifiGetRssiAsQuality(device.RSSI), device.RSSI);
      if (!EQ3Period || device.lastStatusTime + (EQ3Period * 10) > UtcTime()) {
        WSContentSend_PD(HTTP_EQ3_TEMPERATURE, label, Settings->flag2.temperature_resolution, &device.TargetTemp, c_unit);
        WSContentSend_P(HTTP_EQ3_DUTY_CYCLE, label, device.DutyCycle);
        WSContentSend_P(HTTP_EQ3_BATTERY, label, device.Battery ? D_NEOPOOL_LOW : D_OK);
      }
    }
  }
}
#endif // USE_WEBSERVER

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/
//
// great description here:
// https://reverse-engineering-ble-devices.readthedocs.io/en/latest/protocol_description/00_protocol_description.html
// not all implemented yet.
//
int EQ3Send(const uint8_t* addr, uint8_t CmdIdx, const char* param1, const char* param2, int useAlias) {
  if (!param1) param1 = "";
  if (!param2) param2 = "";
  uint8_t d[20];
  memset(d, 0, sizeof(d));
  uint8_t dlen = 0;
#ifdef EQ3_DEBUG
  AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_INFO], "EQ3: %s: cmd: [%s] [%s] [%s]", addrStr(addr), IdxToTrvCmd(CmdIdx), param1, param2);
#endif

  switch (CmdIdx) {
    case TRV_RAW: {
      if (!*param1) return -1;

      int len = strlen(param1) / 2;
      if (len > 20) {
        AddLog(LOG_LEVEL_ERROR, "EQ3: raw len of %s = %d > 20", param1, len);
        return -1;
      }
      BLE_ESP32::fromHex(d, param1, len);
      dlen = len;
      break;
    }

    case TRV_POLL:
    case TRV_STATE:
    case TRV_SETTIME: {
      if (!*param1) {
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
      } else {
        d[0] = 0x03;
        BLE_ESP32::fromHex(d + 1, param1, 6);
      }
      dlen = 7;
      break;
    }

    case TRV_SETTEMP: {
      if (!*param1) return -1;

      float ftemp = atof(param1);
      if (ftemp < 4.5) ftemp = 4.5;
      if (ftemp > 30) ftemp = 30;

      d[0] = 0x41;
      d[1] = (uint8_t)(ftemp * 2);
      dlen = 2;
      break;
    }

    case TRV_OFFSET: {
      if (!*param1) return -1;

      float ftemp = atof(param1);
      if (ftemp < -3.5) ftemp = -3.5;
      if (ftemp > 3.5) ftemp = 3.5;

      d[0] = 0x13;
      d[1] = (int8_t)(ftemp * 2 + 7);
      dlen = 2;
      break;
    }

    case TRV_SETDAYNIGHT: {
      if (!*param1 || !*param2) return -1;

      d[0] = 0x11;

      float ftemp = atof(param1);
      if (ftemp < 4.5) ftemp = 4.5;
      if (ftemp > 30)  ftemp = 30;
      d[1] = (uint8_t)(ftemp * 2);

      ftemp = atof(param2);
      if (ftemp < 4.5) ftemp = 4.5;
      if (ftemp > 30)  ftemp = 30;
      d[2] = (uint8_t)(ftemp * 2);

      dlen = 3;
      break;
    }

    case TRV_SETWINDOWTEMPDUR: {
      if (!*param1 || !*param2) return -1;

      float ftemp = atof(param1);
      if (ftemp < 4.5) ftemp = 4.5;
      if (ftemp > 30)  ftemp = 30;

      uint8_t dur = atoi(param2);
      if (dur > 20) dur = 20;

      d[0] = 0x14;
      d[1] = (uint8_t)(ftemp * 2);
      d[2] = dur / 5;
      dlen = 3;
      break;
    }

    case TRV_SETHOLIDAY: {
      //40941C152402
      // 40 94
      if (!*param1 || !*param2) return -1;

      char *p1 = nullptr, *p2 = nullptr, *p3 = nullptr, *p4 = nullptr, *p5 = nullptr;
      uint8_t yy   = (int)strtol(param1, &p1, 10);
      uint8_t mm   = (p1 && p1[0] == '-') ? (int)strtol(p1 + 1, &p2, 10) : 255;
      uint8_t dd   = (p2 && p2[0] == '-') ? (int)strtol(p2 + 1, &p3, 10) : 255;
      uint8_t hour = (p3 && p3[0] == ',') ? (int)strtol(p3 + 1, &p4, 10) : 255;
      uint8_t min  = (p4 && p4[0] == ':') ? (int)strtol(p4 + 1, &p5, 10) : 255;
      float ftemp = atof(param2);

      AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], "EQ3: setholiday: %02d-%02d-%02d,%02d:%02d %1_f", yy, mm, dd, hour, min, &ftemp);

      if (yy > 99 || !mm || mm > 12 || !dd || dd > 31 || hour > 23 || min > 59) return -1;

      if (ftemp < 4.5)  ftemp = 4.5;
      if (ftemp > 29.5) ftemp = 29.5; // Cannot be set to more than 29.5 in this command

      d[0] = 0x40;
      d[1] = (uint8_t)(ftemp * 2 + 128);
      d[2] = dd;
      d[3] = yy;
      d[4] = (hour * 60 + min) / 30;
      d[5] = mm;
      dlen = 6;
      break;
    }

    case TRV_BOOST: {
      d[0] = 0x45;
      d[1] = 0x01;
      if (*param1 == '0' || !strcasecmp(param1, "off")) d[1] = 0x00;
      dlen = 2;
      break;
    }

    case TRV_UNBOOST: {
      d[0] = 0x45;
      d[1] = 0x00;
      dlen = 2;
      break;
    }

    case TRV_LOCK: {
      d[0] = 0x80;
      d[1] = 0x01;
      if (*param1 == '0' || !strcasecmp(param1, "off")) d[1] = 0x00;
      dlen = 2;
      break;
    }

    case TRV_UNLOCK: {
      d[0] = 0x80;
      d[1] = 0x00;
      dlen = 2;
      break;
    }

    case TRV_AUTO: {
      d[0] = 0x40;
      d[1] = 0x00;
      dlen = 2;
      break;
    }

    case TRV_HEAT:
    case TRV_MANUAL: {
      d[0] = 0x40;
      d[1] = 0x40;
      dlen = 2;
      break;
    }

/*
    // this is basically 'cancel holiday' - mode auto does that.
    case TRV_ECO: {
      d[0] = 0x40;
      d[1] = 0x80;
      dlen = 2;
      break;
    }
*/

    case TRV_ON: {
      if (EQ3Send(addr, TRV_MANUAL, nullptr, nullptr, useAlias) < 1) return -1;
      return EQ3Send(addr, TRV_SETTEMP, "30", nullptr, useAlias);
    }

    case TRV_COOL:
    case TRV_FANONLY:
    case TRV_OFF: {
      if (EQ3Send(addr, TRV_MANUAL, nullptr, nullptr, useAlias) < 1) return -1;
      return EQ3Send(addr, TRV_SETTEMP, "4.5", nullptr, useAlias);
    }

    case TRV_VALVE: {
      if (!*param1) return -1;
      d[0] = 0x41;
      d[1] = 0x3c;
      if (*param1 == '0' || !strcasecmp(param1, "off")) d[1] = 0x09;
      dlen = 2;
      break;
    }

    case TRV_MODE: {
      if (!*param1) return -1;
      return EQ3Send(addr, TrvCmdToIdx(param1), nullptr, nullptr, useAlias);
    }

    case TRV_DAY: {
      d[0] = 0x43;
      dlen = 1;
      break;
    }
    
    case TRV_NIGHT: {
      d[0] = 0x44;
      dlen = 1;
      break;
    }

    case TRV_REQPROFILE: {
      if (!*param1) return -1;
      d[0] = 0x20;
      d[1] = atoi(param1);
      dlen = 2;
      break;
    }

    case TRV_SETPROFILE: {
      if (!*param1 || !*param2) return -1;

      d[0] = 0x10;
      d[1] = atoi(param1);

      // default
      uint8_t temps[7] = {0x22,0x22,0x22,0x22,0x22,0x22,0x22};
      uint8_t times[7] = {0x90,0x90,0x90,0x90,0x90,0x90,0x90};

      // 20.5-17:30,
      const char *p = param2;
      uint8_t i = 0;
      while (p && *p) {
        char *p1 = nullptr, *p2 = nullptr, *p3 = nullptr;
        float ftemp = strtof(p, &p1);
        uint8_t hh = (p1 && *p1 == '-') ? (int)strtol(p1 + 1, &p2, 10) : 255;
        uint8_t mm = (p2 && *p2 == ':') ? (int)strtol(p2 + 1, &p3, 10) : 255;

        AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], "EQ3: setprofile%d[%d]: %1_f-%02d:%02d", d[1], i, &ftemp, hh, mm);

        if (hh > 24 || mm > 59) return -1;

        if (ftemp < 4.5) ftemp = 4.5;
        if (ftemp > 30)  ftemp = 30;

        temps[i] = (uint8_t)(ftemp * 2);
        times[i] = (hh * 60 + mm) / 10;
        p = (p3 && *p3 == ',') ? p3 + 1 : nullptr;
        i++;
        if (i >= 7) break;
      }

      // remaining left at 00 00
      for (uint8_t j = 0; j < 7; j++) {
        d[2 + j * 2] = temps[j];
        d[2 + j * 2 + 1] = times[j];
      }

      dlen = 16; // 2 + 14
      break;
    }

    default: {
      return -1;
    }
  }

  if (dlen) {
    dlen = 16;
    return EQ3QueueOp(addr, d, dlen, CmdIdx, useAlias);

    //return EQ3Operation(addr, d, dlen, 4);
  }
  return -1;
}

uint8_t CmndTrvNext(char *data) {

  char *p = strtok(data, " ");
  if (!strcasecmp(p, "reset")) {
    CmndTrvReset(); // only for compability and will removed in future - new command is TrvReset
    return TRV_DONE;
  }

  if (!strcasecmp(p, "devlist") || !strcasecmp(p, "scan")) {
#ifdef EQ3_DEBUG
    AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_DEBUG], "EQ3: cmd: %s", p);
#endif
    CmndTrvDevList(); // only for compability and will removed in future - new commands are TrvDevList or TrvScan
    return TRV_DONE;
  }

  // only allow one command in progress
  //if (retries) return TRV_IGNOREDBUSY;

  int useAlias = 0;
  uint8_t addrbin[6];
  int addrres = BLE_ESP32::getAddr(addrbin, p);
  if (addrres) {
    if (addrres == 2) {
      AddLog(LOG_LEVEL_DEBUG, "EQ3: addr used alias: %s", p);
      useAlias = 1;
    }
    NimBLEAddress addr(addrbin, addrbin[6]);

#ifdef EQ3_DEBUG
    //AddLog(BLE_ESP32::BLELogLevel[LOG_LEVEL_INFO], PSTR("EQ3: cmd addr: %s -> %s"), p, addr.toString().c_str());
#endif
  } else {
    AddLog(LOG_LEVEL_ERROR, "EQ3: addr invalid: %s", p);
    return TRV_INVADDR;
  }

  // get index of next part of cmd
  char *cmd = strtok(nullptr, " ");
  uint8_t CmdIdx = TrvCmdToIdx(cmd);
  if (CmdIdx == TRV_UNKNOWN) return TRV_INVCMD;

  char *param1 = strtok(nullptr, " ");
  char *param2 = nullptr;
  if (param1) {
    param2 = strtok(nullptr, " ");
  }

  int res = EQ3Send(addrbin, CmdIdx, param1, param2, useAlias);

  if (res > 0) { // succeeded to queue
    AddLog(LOG_LEVEL_INFO, "EQ3: Command \"%s\" queued", IdxToTrvCmd(CmdIdx));
    return TRV_QUEUED;
  }

  if (res < 0) { // invalid in some way
    AddLog(LOG_LEVEL_ERROR, "EQ3: Command \"%s\" failed", IdxToTrvCmd(CmdIdx));
    return TRV_INVCMD;
  }

  // failed to queue
  AddLog(LOG_LEVEL_ERROR, "EQ3: Command \"%s\" failed to queue", IdxToTrvCmd(CmdIdx));
  return TRV_CMDFAIL;
}

void CmndTrv(void) {
  uint8_t res = CmndTrvNext(XdrvMailbox.data);
  ResponseCmndChar(TrvResponses[res]);
}

void CmndTrvPeriod(void) {
  if (XdrvMailbox.data_len) {
    if (XdrvMailbox.payload == 1) {
      NextPollSeconds = 1;
    } else {
      EQ3Period = XdrvMailbox.payload;
      if (EQ3Period) EQ3Period = tmax(EQ3Period, EQ3_NUM_DEVICESLOTS);
      NextPollSeconds = tmin(NextPollSeconds, EQ3Period);
    }
  }
  ResponseCmndNumber(EQ3Period);
}

void CmndTrvRetries(void) {
  if (XdrvMailbox.data_len) {
    EQ3Retries = XdrvMailbox.payload;
  }
  ResponseCmndNumber(EQ3Retries);
}

void CmndTrvOnlyAliased(void) {
  if (XdrvMailbox.data_len) {
    EQ3OnlyAliased = XdrvMailbox.payload;
  }
  ResponseCmndNumber(EQ3OnlyAliased);
}

void CmndTrvMatchPrefix(void) {
  if (XdrvMailbox.data_len) {
    EQ3MatchPrefix = XdrvMailbox.payload;
  }
  ResponseCmndNumber(EQ3MatchPrefix);
}

void CmndTrvMinRSSI(void) {
  if (XdrvMailbox.data_len) {
    trvMinRSSI = atoi(XdrvMailbox.data);
  }
  // signed number
  Response_P("{\"%s\":%d}", XdrvMailbox.command, trvMinRSSI);
}

void CmndTrvHideFailedPoll(void) {
  if (XdrvMailbox.data_len) {
    EQ3HideFailedPoll = XdrvMailbox.payload;
  }
  ResponseCmndNumber(EQ3HideFailedPoll);
}

void CmndTrvReset(void) {
  retries = 0;
  for (auto& device : EQ3Devices) device = eq3_device_t{};
  opQueue.clear();
  opInProgress = false;
  ResponseCmndDone();
}

void CmndTrvDevList(void) {
  EQ3SendCurrentDevices();
  ResponseCmndDone();
}

#define EQ3_TOPIC "EQ3"
static char tmp[120];

bool mqtt_direct() {
  char stopic[TOPSZ];
  strncpy(stopic, XdrvMailbox.topic, TOPSZ);
  XdrvMailbox.topic[TOPSZ-1] = 0;

  AddLog(LOG_LEVEL_DEBUG, "EQ3: mqtt: %s:%s", stopic, XdrvMailbox.data);

  char *items[10];
  char *p = stopic;
  int cnt = 0;
  do {
    items[cnt] = strtok(p, "/");
    cnt++;
    p = nullptr;
  } while (items[cnt-1]);
  cnt--; // represents the number of items

  if (cnt < 4) { // not for us?
    //AddLog(LOG_LEVEL_INFO, PSTR("cnt: %d < 4"), cnt);
    return false;
  }

  for (int i = 0; i < cnt; i++) {
    //AddLog(LOG_LEVEL_INFO, PSTR("cnt %d:%s"), i, items[i]);
  }

  int EQ3index = 0;
  int MACindex = 0;
  int CMDindex = 0;
  if (strcasecmp(items[cnt-3], EQ3_TOPIC)) {
    //AddLog(LOG_LEVEL_INFO, PSTR("cnt-3 not %s"), PSTR(EQ3_TOPIC));
    if (strcasecmp(items[cnt-2], EQ3_TOPIC)) {
      //AddLog(LOG_LEVEL_INFO, PSTR("cnt-2 not %s"), PSTR(EQ3_TOPIC));
      return false; // not for us
    } else {
      EQ3index = cnt - 2;
      MACindex = cnt - 1;
    }
  } else {
    EQ3index = cnt - 3;
    MACindex = cnt - 2;
    CMDindex = cnt - 1;
  }

  int remains = 120;
  memset(tmp, 0, sizeof(tmp));
  p = tmp;
  uint8_t addr[6];
  int useAlias = BLE_ESP32::getAddr(addr, items[MACindex]);
  int res = 6; // invalid address/alias

  // if address or alias valid
  if (useAlias) {
    strncpy(p, items[MACindex], remains - 6);
    p += strlen(p);
    *(p++) = 0x20;
    remains = 120 - (p - tmp);

    if (CMDindex) {
      strncpy(p, items[CMDindex], remains - 6);
      p += strlen(p);
      *(p++) = 0x20;
      remains = 120 - (p - tmp);
    }

    strncpy(p, XdrvMailbox.data, remains - 6);
    p += strlen(p);
    *(p++) = 0x20;
    remains = 120 - (p - tmp);
    *(p++) = 0;

    AddLog(LOG_LEVEL_DEBUG, "EQ3: mqtt->cmdstr %s", tmp);
    res = CmndTrvNext(tmp);
  }

  // post result to stat/tas/EQ3/<MAC> {"result":"<string>"}
  EQ3SendResult(items[MACindex], TrvResponses[res]);

  return true;
}


///////////////////////////////////////////////
// starts a completely fresh MQTT message.
// sends ONE sensor's worth of HA discovery msg
constexpr const char EQ3_HA_DISCOVERY_TEMPLATE[] =
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
void EQ3DiscoveryOneEQ3() {
  // don't detect half-added ones here
  if (EQ3CurrentSingleSlot >= EQ3_NUM_DEVICESLOTS) {
    // if we got to the end of the sensors, then don't send more
    return;
  }

#ifdef USE_HOME_ASSISTANT
  if(Settings->flag.hass_discovery) {
    eq3_device_t *p;
    do {
      p = &EQ3Devices[EQ3CurrentSingleSlot];
      if (0 == p->timeoutTime) {
        EQ3CurrentSingleSlot++;
      }
    } while ((0 == p->timeoutTime) && (EQ3CurrentSingleSlot <= EQ3_NUM_DEVICESLOTS));

    if (EQ3CurrentSingleSlot >= EQ3_NUM_DEVICESLOTS) {
      return;
    }

    // careful - a missing comma causes a crash!!!!
    // because of the way we loop?
    constexpr const char* classes[] = {
      "temperature",
      "temp",
      "°C",
      "signal_strength",
      "RSSI",
      "dB"
    };

    constexpr size_t datacount = sizeof(classes) / sizeof(*classes) / 3;

    if (p->nextDiscoveryData >= datacount) {
      p->nextDiscoveryData = 0;
    }

    char DiscoveryTopic[80];
    const char *host = NetworkHostname();
    const char *devtype = "EQ3";
    char idstr[32];
    const char *alias = BLE_ESP32::getAlias(p->addr);
    const char *id = idstr;
    if (alias && *alias) {
      id = alias;
    } else {
      sprintf(idstr, "%s%02x%02x%02x",
            devtype,
            p->addr[3], p->addr[4], p->addr[5]);
    }

    char SensorTopic[60];
    sprintf(SensorTopic, "stat/%s/EQ3/%s",
      host, id);

    //int i = p->nextDiscoveryData*3;
    for (size_t i = 0; i < datacount * 3; i += 3) {
      if (!classes[i] || !classes[i + 1] || !classes[i + 2]) {
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
      EQ3_ESP32::EQ3EverySecond();
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
