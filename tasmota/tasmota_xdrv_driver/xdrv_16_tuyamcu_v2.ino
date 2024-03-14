/*
  xdrv_16_tuyamcu.ino - Tuya MCU support for Tasmota

  Copyright (C) 2021  Federico Leoni, digiblur, Joel Stein and Theo Arends

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

#ifdef USE_LIGHT
#ifdef USE_TUYA_MCU_V2
/*********************************************************************************************\
 * Tuya MCU V2

  rework 2022-04-03 SH:
  the intent of the rework is:
  1/ make the protocol parsing more robust.
    to this end, it now observes a byte timeout on receive, and looks for 55AA rather than just 55 to reset.
    Parsing has been split from message processing for better readability.
  2/ try to observe to 'tuya' state machine as per thier documentation.
    - at least for startup.
    ALL sends should originate from the state machine (except manual sends?).
    ALL sends which need a return (synchronous) wait for an expected command before
    the next command can be sent. (commands were observed being dropped by the MCU).
  3/ sending of DP data is now 'requested', and only sent if the DP value is
    different to the one the MCU has (fixes an issue with Athom dimmer).
    (data is stored in DPStore[], and serviced in the state machine)

\*********************************************************************************************/

#define XDRV_16                16
#define XNRG_32                32   // Needs to be the last XNRG_xx

#ifndef TUYA_DIMMER_ID
#define TUYA_DIMMER_ID         0
#endif

//#define TUYA_MORE_DEBUG

/*********************************************************************************************/

#define TUYA_CMD_HEARTBEAT          0x00
#define TUYA_CMD_QUERY_PRODUCT      0x01
#define TUYA_CMD_MCU_CONF           0x02
#define TUYA_CMD_WIFI_STATE         0x03
#define TUYA_CMD_WIFI_RESET         0x04
#define TUYA_CMD_WIFI_SELECT        0x05
#define TUYA_CMD_SET_DP             0x06
#define TUYA_CMD_STATE              0x07
#define TUYA_CMD_QUERY_STATE        0x08
#define TUYA_CMD_INITIATING_UPGRADE 0x0A // not implemented
#define TUYA_CMD_UPGRADE_PACKAGE    0x0B // not implemented

// MCU sends to request a 0x0c return of GMT
#define TUYA_CMD_GET_TIME_GMT       0x0C // not implemented

// MCU sends
#define TUYA_CMD_TEST_WIFI          0x0E // not implemented
// MCU sends
#define TUYA_CMD_GET_MEMORY         0x0F // not implemented

// MCU sends to request a 0x1c return of Local Time
// we send unsolicited??
#define TUYA_CMD_SET_TIME           0x1C

// MCU Sends, we must respond with 0x22, len1 00 failure, 01 success
#define TUYA_CMD_STATUS_SYNC        0x22 // not implemented
#define TUYA_CMD_STATUS_SYNC_RES    0x23 // not implemented

// From MCU.  Response should be 0x0b 00/01/02
// subcmd 03 -> request weather data
// subcmd 06 -> get map id
#define TUYA_CMD_REPORT_STATUS_RECORD_TYPE    0x34 // not implemented

// MCU sends 'after 01 bute before 02'.  Response should be 0x37, len 2, 00, 00/01/02
// subcmd 01->file download notification
#define TUYA_CMD_NEW_FEATURES       0x37 // not implemented

// MCU sends
#define TUYA_CMD_ENABLE_WEATHER     0x20 // not implemented
// we send every 30 mins, if enabled, MCU acks with 0x21
#define TUYA_CMD_SEND_WEATHER       0x21 // not implemented

// MCU sends, response 0x24, len 1, -ve dB or 0
#define TUYA_CMD_GET_WIFI_STRENGTH  0x24 // not implemented

// MCU sends, response 0x25
#define TUYA_CMD_DISABLE_HEARTBEAT  0x25 // not implemented
// MCU sends JSON,  response 0x2A, len1 00/01/02/03
#define TUYA_CMD_SERIAL_PAIRING     0x2A // not implemented

#define TUYA_CMD_VACUUM_MAP_STREAMING      0x28 // not implemented
#define TUYA_CMD_VACUUM_MAP_STREAMING_MULTIPLE      0x30 // not implemented

// MCU sends,  response 0x2D
#define TUYA_CMD_GET_MAC            0x2D // not implemented
// MCU sends,  response 0x2E
#define TUYA_CMD_IR_STATUS          0x2E // not implemented
// MCU sends,  response 0x2E
#define TUYA_CMD_IR_TEST            0x2F // not implemented
// We send, response 0x33
// uses subcommands, 01->learning, 02->data, 03->report.
#define TUYA_CMD_RF                 0x33 // not implemented

#define TUYA_LOW_POWER_CMD_WIFI_STATE   0x02
#define TUYA_LOW_POWER_CMD_WIFI_RESET   0x03
#define TUYA_LOW_POWER_CMD_WIFI_CONFIG  0x04
#define TUYA_LOW_POWER_CMD_STATE        0x05

#define TUYA_TYPE_RAW          0x00
#define TUYA_TYPE_BOOL         0x01
#define TUYA_TYPE_VALUE        0x02
#define TUYA_TYPE_STRING       0x03
#define TUYA_TYPE_ENUM         0x04

// limit to what we store for DPs of type string
#define TUYA_MAX_STRING_SIZE   16

#define TUYA_BUFFER_SIZE       256

#define TUYA_BYTE_TIMEOUT_MS   500

#define HEARTBEAT_INTERVAL_S   15

#define TUYAREAD32FROMPTR(x) (((uint8_t*)x)[0] << 24 | ((uint8_t*)x)[1] << 16 | ((uint8_t*)x)[2] << 8 | ((uint8_t*)x)[3])

enum {
  TUYA_STARTUP_STATE_INIT = 0,
  TUYA_STARTUP_STATE_WAIT_ACK_INIT, // 1

  TUYA_STARTUP_STATE_PRODUCT, //2
  TUYA_STARTUP_STATE_WAIT_ACK_PRODUCT, // 3

  TUYA_STARTUP_STATE_WAIT_OPTIONAL_NEW_FEATURES, // 4

  TUYA_STARTUP_STATE_CONF, // 5
  TUYA_STARTUP_STATE_WAIT_ACK_CONF, //6

  TUYA_STARTUP_STATE_WIFI_STATE, //7
  TUYA_STARTUP_STATE_WAIT_ACK_WIFI,//8

  TUYA_STARTUP_STATE_QUERY_STATE,//9
  TUYA_STARTUP_STATE_WAIT_ACK_QUERY,//10

  TUYA_STARTUP_STATE_SEND_CMD,//11
  TUYA_STARTUP_STATE_WAIT_ACK_CMD,//12

  TUYA_STARTUP_STATE_SEND_HEARTBEAT,//13
  TUYA_STARTUP_STATE_WAIT_ACK_HEARTBEAT,//14

};

#include <TasmotaSerial.h>
TasmotaSerial *TuyaSerial = nullptr;

#define TUYA_MAX_STORED_DPs 10
typedef struct TUYA_DP_STORE_tag {
  uint8_t DPid;
  uint8_t Type;
  uint8_t rxedValueLen;
  uint8_t desiredValueLen;
  // NOTE - THESE MUST BE 32 bit ALIGNED, hence uint32_t
  // DPValues are changed by every TUYA_CMD_STATE
  uint32_t rxedValue[(TUYA_MAX_STRING_SIZE+3)/4];
  // desired DPValues are set, and toSet is set to request
  uint32_t desiredValue[(TUYA_MAX_STRING_SIZE+3)/4];
  // set to 1 if desired value changed
  uint8_t toSet;
  // flag to indicate we saw it at least once from MCU.
  uint8_t rxed;
} TUYA_DP_STORE;

typedef struct TUYA_STRUCT_tag {
  // variables to handle setting of a DP.
  // DPIds are filled out by initial TUYA_CMD_QUERY_STATE
  // or by an easrly set request.
  TUYA_DP_STORE DPStore[TUYA_MAX_STORED_DPs];
  uint8_t numRxedDPids;

  // set to indicate the command which is an ack to the last sent command.
  // e.g. 7 is ack to 6 and 8
  // if state is TUYA_STARTUP_STATE_WAIT_ACK_CMD
  uint8_t expectedResponseCmd;

  uint16_t Levels[5];       // Array to store the values of TuyaMCU channels
  uint16_t Snapshot[5];     // Array to store a snapshot of Tasmota actual channel values
  uint16_t EnumState[4];      // Array to store up to four Enum type 4 values
  char RGBColor[7];            // Stores RGB Color string in Hex format
  uint16_t CTMin;                   // Minimum CT level allowed - When SetOption82 is enabled will default to 200
  uint16_t CTMax;                   // Maximum CT level allowed - When SetOption82 is enabled will default to 380
  int16_t Sensors[14];                    // Stores the values of Sensors connected to the Tuya Device
  bool ModeSet;                   // Controls 0 - Single Tone light, 1 - RGB Light
  bool SensorsValid[14];                  // Bool used for nullify the sensor value until a real value is received from the MCU
  bool SuspendTopic;              // Used to reduce the load at init time or when polling the configuraton on demand
  bool ignore_dim;                // Flag to skip serial send to prevent looping when processing inbound states from the faceplate interaction
  uint32_t ignore_topic_timeout;      // Suppress the /STAT topic (if enabled) to avoid data overflow until the configuration is over
  uint8_t cmd_status;                 // Current status of serial-read
  uint8_t cmd_checksum;               // Checksum of tuya command
  uint8_t data_len;                   // Data lenght of command
  uint8_t wifi_state;                // Keep MCU wifi-status in sync with WifiState()
  uint8_t heartbeat_timer;            // 10 second heartbeat timer for tuya module

  // flasg which trigger sends of certain messages
  uint8_t send_heartbeat;             // trigger heartbeat when we can next send.
  uint8_t send_time;                  // trigger time send when we can in running mode.

#ifdef USE_ENERGY_SENSOR
  uint32_t lastPowerCheckTime;        // Time when last power was checked
#endif // USE_ENERGY_SENSOR
  unsigned char buffer[TUYA_BUFFER_SIZE];      // Serial receive buffer
  int byte_counter;                   // Index in serial receive buffer
  uint32_t ignore_dimmer_cmd_timeout; // Time until which received dimmer commands should be ignored
  bool low_power_mode;            // Normal or Low power mode protocol
  bool send_success_next_second;  // Second command success in low power mode
  bool active;

  int timeout; // command timeout in ms
  // every time we get a long press, add 10000.  If we get to 20000, then go to wifimanager mode, if enabled
  // decremented by 1000 each second.
  int wifiTimer;

  char inStateMachine;
  char startup_state;
  char timeout_state; // state to go to if timeout.

  unsigned char lastByte;
  unsigned int lastByteTime; // time of last byte receipt.

  unsigned int errorcnt; // increment every time something goes awry
  unsigned int lasterrorcnt; // used to choose when to log errorcnt

  int dimDelay_ms[2]; // SIGNED the delay after a dim result after which we are allowed to send a dim value
  int defaultDimDelay_ms; // the delay after a dim result after which we are allowed to send a dim value
  uint8_t dimCmdEnable; // we are allowed to send a dim command - bitfield
  uint8_t dimDebug; // enables a single dim debug - bitfield

  uint8_t last_button;

  int sends;
  int rxs;

} TUYA_STRUCT;

TUYA_STRUCT *pTuya = (TUYA_STRUCT *)0;
//void TuyaSendCmd(uint8_t cmd, uint8_t payload[] = nullptr, uint16_t payload_len = 0);

void TuyaSendState(uint8_t id, uint8_t type, uint8_t* value, int len);

int init_tuya_struct() {
  if (pTuya) return 0;  // done already
  pTuya = (TUYA_STRUCT *)calloc(sizeof(TUYA_STRUCT), 1);
  if (!pTuya) return 0;
  strcpy(pTuya->RGBColor, "000000");            // Stores RGB Color string in Hex format
  pTuya->CTMin = 153;                   // Minimum CT level allowed - When SetOption82 is enabled will default to 200
  pTuya->CTMax = 500;                   // Maximum CT level allowed - When SetOption82 is enabled will default to 380
  pTuya->wifi_state = -2;                // Keep MCU wifi-status in sync with WifiState()
  pTuya->defaultDimDelay_ms = 2000;     // 2s delay from a power command to a dim command, or from an rxed dim command to sending one.
#ifdef TUYA_MORE_DEBUG
  AddLog(LOG_LEVEL_INFO, PSTR("TYA: Init struct done"));
#endif
  return 1;
}

#define D_JSON_TUYA_MCU_RECEIVED "TuyaReceived"

#define D_PRFX_TUYA "Tuya"
#define D_CMND_TUYA_MCU "MCU"
#define D_CMND_TUYA_MCU_SEND_STATE "Send"
#define D_CMND_TUYARGB "RGB"
#define D_CMND_TUYA_ENUM "Enum"
#define D_CMND_TUYA_ENUM_LIST "EnumList"
// #define D_CMND_TUYA_SET_TEMP "SetTemp"
// #define D_CMND_TUYA_SET_HUM "SetHum"
// #define D_CMND_TUYA_SET_TIMER "SetTimer"

const char kTuyaSensors[] PROGMEM = // List of available sensors (can be expanded in the future)
//          71              72          73            74            75
  "" D_JSON_TEMPERATURE "|TempSet|" D_JSON_HUMIDITY "|HumSet|" D_JSON_ILLUMINANCE
//         76            77             78              79                      80                     81     82     83     84
  "|" D_JSON_TVOC "|" D_JSON_ECO2 "|" D_JSON_CO2 "|" D_JSON_GAS "|" D_ENVIRONMENTAL_CONCENTRATION "||Timer1|Timer2|Timer3|TImer4";

const char kTuyaCommand[] PROGMEM = D_PRFX_TUYA "|"  // Prefix
  D_CMND_TUYA_MCU "|" D_CMND_TUYA_MCU_SEND_STATE "|" D_CMND_TUYARGB "|" D_CMND_TUYA_ENUM "|" D_CMND_TUYA_ENUM_LIST "|TempSetRes|DimDelay";

void (* const TuyaCommand[])(void) PROGMEM = {
  &CmndTuyaMcu, &CmndTuyaSend, &CmndTuyaRgb, &CmndTuyaEnum, &CmndTuyaEnumList, &CmndTuyaTempSetRes, &CmdTuyaSetDimDelay
};

const uint8_t TuyaExcludeCMDsFromMQTT[] PROGMEM = { // don't publish this received commands via MQTT if SetOption66 and SetOption137 is active (can be expanded in the future)
  TUYA_CMD_HEARTBEAT, TUYA_CMD_WIFI_STATE, TUYA_CMD_SET_TIME, TUYA_CMD_UPGRADE_PACKAGE
};

/*********************************************************************************************\
 * Web Interface
\*********************************************************************************************/

bool IsModuleTuya(void) {
  if (!pTuya) return false;
  bool is_tuya = pTuya->active;
//#ifdef ESP8266
    // This is not a Tuya driven device. It uses a Tuya provided ESP8266. Why it was here is a mystery to me.
//  if (SK03_TUYA == TasmotaGlobal.module_type) {
//    is_tuya = true;
//  }
//#endif
  return is_tuya;
}

bool AsModuleTuyaMS(void) // ModeSet Layout
{
  return ((TasmotaGlobal.light_type > LT_RGB) && TuyaGetDpId(TUYA_MCU_FUNC_MODESET) != 0);
}

bool TuyaModeSet(void) // ModeSet Status
{
  if (!pTuya) return false;
  return pTuya->ModeSet;
}

/*********************************************************************************************\
 * Web Interface
\*********************************************************************************************/

/*
TuyaSend<x> dpId,data

TuyaSend0 -> Sends TUYA_CMD_QUERY_STATE
TuyaSend1 11,1 -> Sends boolean (Type 1) data 0/1 to dpId 11 (Max data length 1 byte)
TuyaSend2 11,100 -> Sends integer (Type 2) data 100 to dpId 11 (Max data length 4 bytes)
TuyaSend2 11,0xAABBCCDD -> Sends 4 bytes (Type 2) data to dpId 11 (Max data length 4 bytes)
TuyaSend3 11,ThisIsTheData -> Sends the supplied string (Type 3) to dpId 11 ( Max data length not-known)
TuyaSend4 11,1 -> Sends enum (Type 4) data 1 to dpId 11 (Max data length 1 bytes)
TuyaSend5 11,ABCD -> Sends an HEX string (Type 3) data to dpId
TuyaSend6 11,ABCD -> Sends raw (Type 0) data to dpId

TuyaSend8 -> Sends TUYA_CMD_QUERY_PRODUCT ?
*/

void CmndTuyaSend(void) {
  if (!pTuya) return;
  switch(XdrvMailbox.index){
    case 0:
    TuyaRequestState(0);
      break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    {
    if (XdrvMailbox.data_len > 0) {
      char *p;
      const char *data = "";
      uint8_t i = 0;
      uint8_t dpId = 0;
      for (char *str = strtok_r(XdrvMailbox.data, ", ", &p); str && i < 2; str = strtok_r(nullptr, ", ", &p)) {
        if ( i == 0) {
          // note: can be a number, or 0xnn for hex because base is 0
          dpId = strtoul(str, nullptr, 0);
        } else {
          data = str;
        }
        i++;
      }

        if (1 == XdrvMailbox.index) {
          TuyaSendBool(dpId, strtoul(data, nullptr, 0));
        } else if (2 == XdrvMailbox.index) {
          TuyaSendValue(dpId, strtoull(data, nullptr, 0));
        } else if (3 == XdrvMailbox.index) {
          TuyaSendString(dpId, data);
        } else if (5 == XdrvMailbox.index) {
          TuyaSendHexString(dpId, data);
        } else if (4 == XdrvMailbox.index) {
          TuyaSendEnum(dpId, strtoul(data, nullptr, 0));
        } else if (6 == XdrvMailbox.index) {
          TuyaSendRaw(dpId, data);
      } else if (7 == XdrvMailbox.index) {
        uint8_t cmd = dpId;
        // send ANY cmd with payload from hex string
        // calculates length and checksum for you.
        // like "0," to send a heartbeat, "3,4" to set wifi led mode,
        // "0x1c,0110041305060702" - set local time
        // sends immediately....
        TuyaSendRawCmd(cmd, data);
      }
    }
    } break;

    case 8:
      // product info
      TuyaRequestState(8);
      break;
    case 9:
      Settings->tuyamcu_topic = !Settings->tuyamcu_topic;
      AddLog(LOG_LEVEL_INFO, PSTR("TYA: TuyaMCU Stat Topic %s"), (Settings->tuyamcu_topic ? PSTR("enabled") : PSTR("disabled")));
      break;
  }
  ResponseCmndDone();
}


void CmdTuyaSetDimDelay(void) {
  if (!pTuya) return;

  switch(XdrvMailbox.index){
    case 1: {
      if (XdrvMailbox.data_len > 0) {
        int32_t delay = strtol(XdrvMailbox.data, nullptr, 0);
        pTuya->defaultDimDelay_ms = delay;
      } else {
        // report only
        Response_P(PSTR("{\"%s\":{\"dimdelay\":%d}}"), XdrvMailbox.command, pTuya->defaultDimDelay_ms);  // Builds TuyaMCU
        return;
    }
    } break;

    default: // no response
      return;
  }
  ResponseCmndDone();
}

// TuyaMcu fnid,dpid

void CmndTuyaMcu(void) {
  if (!pTuya) return;
  if (XdrvMailbox.data_len > 0) {
    char *p;
    uint8_t i = 0;
    uint8_t parm[3] = { 0 };
    for (char *str = strtok_r(XdrvMailbox.data, ", ", &p); str && i < 2; str = strtok_r(nullptr, ", ", &p)) {
      parm[i] = strtoul(str, nullptr, 0);
      i++;
    }

    if (TuyaFuncIdValid(parm[0])) {
      bool DualDim;
      if (TUYA_MCU_FUNC_DIMMER2 == parm[0] && parm[1] != 0) {
        if (TuyaGetDpId(TUYA_MCU_FUNC_DIMMER) != 0) { DualDim = true; }
      } else if (TUYA_MCU_FUNC_DIMMER == parm[0] && parm[1] != 0) {
        if (TuyaGetDpId(TUYA_MCU_FUNC_DIMMER2) != 0) { DualDim = true; }
      } else if ((TUYA_MCU_FUNC_DIMMER == parm[0] && parm[1] == 0) || (TUYA_MCU_FUNC_DIMMER2 == parm[0] && parm[1] == 0)) { DualDim = false; };
      if (DualDim) { // If the second dimmer is enabled CT, RGB or WHITE function must be removed
        if (TuyaGetDpId(TUYA_MCU_FUNC_CT) != 0) { TuyaAddMcuFunc(TUYA_MCU_FUNC_CT, 0); }
        if (TuyaGetDpId(TUYA_MCU_FUNC_RGB) != 0) { TuyaAddMcuFunc(TUYA_MCU_FUNC_RGB, 0); }
        if (TuyaGetDpId(TUYA_MCU_FUNC_WHITE) != 0) { TuyaAddMcuFunc(TUYA_MCU_FUNC_WHITE, 0); }
        Settings->flag3.pwm_multi_channels = 1;
      } else { Settings->flag3.pwm_multi_channels = 0; }
      TuyaAddMcuFunc(parm[0], parm[1]);
      TasmotaGlobal.restart_flag = 2;
    } else {
      AddLog(LOG_LEVEL_ERROR, PSTR("TYA: TuyaMcu Invalid function id=%d"), parm[0]);
    }
  }

  Response_P(PSTR("{\"%s\":["), XdrvMailbox.command);  // Builds TuyaMCU
  bool added = false;
  for (uint8_t i = 0; i < MAX_TUYA_FUNCTIONS; i++) {
    if (Settings->tuya_fnid_map[i].fnid != 0) {
      if (added) {
        ResponseAppend_P(PSTR(","));
      }
      ResponseAppend_P(PSTR("{\"fnId\":%d,\"dpId\":%d}" ), Settings->tuya_fnid_map[i].fnid, Settings->tuya_fnid_map[i].dpid);
      added = true;
    }
  }
  ResponseAppend_P(PSTR("]}"));
}

void CmndTuyaRgb(void) { // Command to control the RGB format
  if (!pTuya) return;

  uint16_t payload = XdrvMailbox.payload;

  if (XdrvMailbox.data_len > 0) {
    if (payload < 0 || payload > 3 || TuyaGetDpId(TUYA_MCU_FUNC_RGB) == 0) {
      return;
    } else {
      if (payload != Settings->tuya_fnid_map[230].dpid) { // fnid 230 is reserved for RGB
        Settings->tuya_fnid_map[230].fnid = 230;
        Settings->tuya_fnid_map[230].dpid = payload;
      }
    }
  }
  ResponseCmndNumber(Settings->tuya_fnid_map[230].dpid);
}

void CmndTuyaTempSetRes(void)
{
  if (!pTuya) return;
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
    Settings->mbflag2.temperature_set_res = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->mbflag2.temperature_set_res);
}

void CmndTuyaEnum(void) { // Command to control up to four type 4 Enum
  if (!pTuya) return;
  uint16_t EnumIdx = XdrvMailbox.index;
  int32_t payload = XdrvMailbox.payload;

  if (EnumIdx > 4 || TuyaGetDpId(TUYA_MCU_FUNC_ENUM1 + (EnumIdx-1)) == 0) {
    return;
  }

  if (XdrvMailbox.data_len > 0) {
    if (payload < 0 || payload > Settings->tuya_fnid_map[EnumIdx + 230].dpid ) {
      return;
    } else {
      if (payload != pTuya->EnumState[EnumIdx-1]) {
        pTuya->EnumState[EnumIdx-1] = payload;
        TuyaSendEnum(TuyaGetDpId(TUYA_MCU_FUNC_ENUM1 + (EnumIdx-1)), payload);
      }
      ResponseCmndIdxNumber(pTuya->EnumState[EnumIdx-1]);
    }
  } else {
    Response_P(PSTR("{\"%s\":{"), XdrvMailbox.command);  // Builds TuyaEnum
    bool added = false;
    for (uint8_t i = 0; i <= 3; i++) {
      if (TuyaGetDpId(TUYA_MCU_FUNC_ENUM1 + i) != 0) {
        if (added) {
          ResponseAppend_P(PSTR(","));
        }
        ResponseAppend_P(PSTR("\"Enum%d\":%d"), i + 1, pTuya->EnumState[i]); // Returns the actual values of Enum as list
        added = true;
      }
    }
    ResponseAppend_P(PSTR("}}"));
  }
}

void CmndTuyaEnumList(void) { // Command to declare the number of items in list for up to four type 4 enum. Min = 0, Max = 31, Default = 0
  if (!pTuya) return;

  if (XdrvMailbox.data_len > 0) {
    char *p;
    uint8_t i = 0;
    uint8_t parm[3] = { 0 };
    for (char *str = strtok_r(XdrvMailbox.data, ", ", &p); str && i < 2; str = strtok_r(nullptr, ", ", &p)) {
      parm[i] = strtoul(str, nullptr, 0);
      i++;
    }
    if ((parm[0] >= 1 && parm[0] <= 4) && (parm[1] >= 1 && parm[1] <= 31)) {
      uint16_t idx = parm[0] + 230; // fnid 231, 232, 233 and 234 are reserved for enum
      Settings->tuya_fnid_map[idx].fnid = idx;
      Settings->tuya_fnid_map[idx].dpid = parm[1];
    }
  }
  if ((TuyaGetDpId(TUYA_MCU_FUNC_ENUM1) != 0) || (TuyaGetDpId(TUYA_MCU_FUNC_ENUM3) != 0) ||
      (TuyaGetDpId(TUYA_MCU_FUNC_ENUM3) != 0) || (TuyaGetDpId(TUYA_MCU_FUNC_ENUM4) != 0)) {
    Response_P(PSTR("{\"%s\":{"), XdrvMailbox.command);  // Builds TuyaEnumList
    bool added = false;
    for (uint8_t i = 0; i <= 3; i++) {
      if (TuyaGetDpId(TUYA_MCU_FUNC_ENUM1 + i) != 0) {
        if (added) {
          ResponseAppend_P(PSTR(","));
          if ( Settings->tuya_fnid_map[i + 231].dpid > 31 ) { Settings->tuya_fnid_map[i + 231].dpid = 0; } // default to 0 it the value exceed the range
        }
        ResponseAppend_P(PSTR("\"Enum%d\":%d"), i + 1, Settings->tuya_fnid_map[i + 231].dpid); // fnid 231, 232, 233 and 234 are reserved for Enum
        added = true;
      }
    }
    ResponseAppend_P(PSTR("}}"));
  } else { return; }
}

int StrCmpNoCase(char const *Str1, char const *Str2) // Compare case sensistive RGB strings
{
  for (;; Str1++, Str2++) {
    int StrCmp = tolower((unsigned char)*Str1) - tolower((unsigned char)*Str2);
    if (StrCmp != 0 || !*Str1) { return StrCmp; }
  }
}

float TuyaAdjustedTemperature(int16_t packetValue, uint8_t res)
{
    switch (res)
    {
    case 1:
        return (float)packetValue / 10.0;
        break;
    case 2:
        return (float)packetValue / 100.0;
        break;
    case 3:
        return (float)packetValue / 1000.0;
        break;
    default:
        return (float)packetValue;
        break;
    }
}

/*********************************************************************************************\
 * Internal Functions
\*********************************************************************************************/

void TuyaAddMcuFunc(uint8_t fnId, uint8_t dpId) {
  bool added = false;

  if (fnId == 0 || dpId == 0) { // Delete entry
    for (uint8_t i = 0; i < MAX_TUYA_FUNCTIONS; i++) {
      if ((dpId > 0 && Settings->tuya_fnid_map[i].dpid == dpId) || (fnId > TUYA_MCU_FUNC_NONE && Settings->tuya_fnid_map[i].fnid == fnId)) {
        Settings->tuya_fnid_map[i].fnid = TUYA_MCU_FUNC_NONE;
        Settings->tuya_fnid_map[i].dpid = 0;
        break;
      }
    }
  } else { // Add or update
    for (uint8_t i = 0; i < MAX_TUYA_FUNCTIONS; i++) {
      if (Settings->tuya_fnid_map[i].dpid == dpId || Settings->tuya_fnid_map[i].dpid == 0 || Settings->tuya_fnid_map[i].fnid == fnId || Settings->tuya_fnid_map[i].fnid == 0) {
        if (!added) { // Update entry if exisiting entry or add
          Settings->tuya_fnid_map[i].fnid = fnId;
          Settings->tuya_fnid_map[i].dpid = dpId;
          added = true;
        } else if (Settings->tuya_fnid_map[i].dpid == dpId || Settings->tuya_fnid_map[i].fnid == fnId) { // Remove existing entry if added to empty place
          Settings->tuya_fnid_map[i].fnid = TUYA_MCU_FUNC_NONE;
          Settings->tuya_fnid_map[i].dpid = 0;
        }
      }
    }
  }
  UpdateDevices();
}

void UpdateDevices() {
  for (uint8_t i = 0; i < MAX_TUYA_FUNCTIONS; i++) {
    uint8_t fnId = Settings->tuya_fnid_map[i].fnid;
    if (fnId > TUYA_MCU_FUNC_NONE && Settings->tuya_fnid_map[i].dpid > 0) {

      if (fnId >= TUYA_MCU_FUNC_REL1 && fnId <= TUYA_MCU_FUNC_REL8) { //Relay
        bitClear(TasmotaGlobal.rel_inverted, fnId - TUYA_MCU_FUNC_REL1);
      } else if (fnId >= TUYA_MCU_FUNC_REL1_INV && fnId <= TUYA_MCU_FUNC_REL8_INV) { // Inverted Relay
        bitSet(TasmotaGlobal.rel_inverted, fnId - TUYA_MCU_FUNC_REL1_INV);
      }
    }
  }
}

inline bool TuyaFuncIdValid(uint8_t fnId) {
  return (fnId >= TUYA_MCU_FUNC_SWT1 && fnId <= TUYA_MCU_FUNC_SWT4) ||
          (fnId >= TUYA_MCU_FUNC_REL1 && fnId <= TUYA_MCU_FUNC_REL8) ||
          (fnId >= TUYA_MCU_FUNC_DIMMER && fnId <= TUYA_MCU_FUNC_REPORT2) ||
          (fnId >= TUYA_MCU_FUNC_POWER && fnId <= TUYA_MCU_FUNC_POWER_TOTAL) ||
          (fnId >= TUYA_MCU_FUNC_REL1_INV && fnId <= TUYA_MCU_FUNC_REL8_INV) ||
          (fnId >= TUYA_MCU_FUNC_ENUM1 && fnId <= TUYA_MCU_FUNC_ENUM4) ||
          (fnId >= TUYA_MCU_FUNC_MOTOR_DIR && fnId <= TUYA_MCU_FUNC_DUMMY) ||
          (fnId == TUYA_MCU_FUNC_LOWPOWER_MODE) ||
          (fnId >= TUYA_MCU_FUNC_TEMP && fnId <= TUYA_MCU_FUNC_HUMSET) ||
          (fnId >= TUYA_MCU_FUNC_LX && fnId <= TUYA_MCU_FUNC_PM25) ||
          (fnId >= TUYA_MCU_FUNC_TIMER1 && fnId <= TUYA_MCU_FUNC_TIMER4);
}

uint8_t TuyaGetFuncId(uint8_t dpid) {
  for (uint8_t i = 0; i < MAX_TUYA_FUNCTIONS; i++) {
    if (Settings->tuya_fnid_map[i].dpid == dpid) {
      return Settings->tuya_fnid_map[i].fnid;
    }
  }
  return TUYA_MCU_FUNC_NONE;
}

uint8_t TuyaGetDpId(uint8_t fnId) {
  for (uint8_t i = 0; i < MAX_TUYA_FUNCTIONS; i++) {
    if (Settings->tuya_fnid_map[i].fnid == fnId) {
      return Settings->tuya_fnid_map[i].dpid;
    }
  }
  return 0;
}

uint8_t TuyaFnIdIsDimmer(uint8_t fnId){
  //TUYA_MCU_FUNC_DIMMER = 21, TUYA_MCU_FUNC_DIMMER2, TUYA_MCU_FUNC_CT, TUYA_MCU_FUNC_RGB, TUYA_MCU_FUNC_WHITE,

  if ((fnId >= TUYA_MCU_FUNC_DIMMER) &&
      (fnId <= TUYA_MCU_FUNC_WHITE)){
    return 1;
  }
  return 0;
}

uint8_t TuyaDpIdIsDimmer(uint8_t dpId){
  uint8_t fnId = TuyaGetFuncId(dpId);
  return TuyaFnIdIsDimmer(fnId);
}

// pTuya->timeout hit zero
void Tuya_timeout(){
  // timeout_state should have been set to the correct state to go to after the timeout
  if (pTuya->startup_state != TUYA_STARTUP_STATE_WAIT_ACK_QUERY){
    AddLog(LOG_LEVEL_ERROR, PSTR("TYA: Protocol timeout state %d -> %d"), pTuya->startup_state, pTuya->timeout_state);
  }
  pTuya->startup_state = pTuya->timeout_state;
  pTuya->timeout = 0;
}

void TuyaSendCmd(uint8_t cmd, uint8_t payload[] = nullptr, uint16_t payload_len = 0, int noerror = 0)
{
  if (!pTuya->inStateMachine && !noerror){
    AddLog(LOG_LEVEL_ERROR, PSTR("TYA: TuyaSendCmd should only be called from within the state machine! - if this was a manual command, then ok."));
  }

  // include ver = 0 in checksum for completeness
  uint8_t checksum = (0xFF + 0x00 + cmd + (payload_len >> 8) + (payload_len & 0xFF));
  TuyaSerial->write(0x55);                  // Tuya header 55AA
  TuyaSerial->write(0xAA);
  TuyaSerial->write((uint8_t)0x00);         // version 00 - send TO MCU
  TuyaSerial->write(cmd);                   // Tuya command
  TuyaSerial->write(payload_len >> 8);      // following data length (Hi)
  TuyaSerial->write(payload_len & 0xFF);    // following data length (Lo)
  char log_data[700];                       // Was MAX_LOGSZ
  snprintf_P(log_data, sizeof(log_data), PSTR("T:>\"55aa00%02x%02x%02x"), cmd, payload_len >> 8, payload_len & 0xFF);
  for (uint32_t i = 0; i < payload_len; ++i) {
    TuyaSerial->write(payload[i]);
    checksum += payload[i];
    snprintf_P(log_data, sizeof(log_data), PSTR("%s%02x"), log_data, payload[i]);
  }
  TuyaSerial->write(checksum);
  TuyaSerial->flush();
  snprintf_P(log_data, sizeof(log_data), PSTR("%s%02x\""), log_data, checksum);
  AddLogData(LOG_LEVEL_DEBUG, log_data);
  pTuya->sends ++;
}


// note: normally regularly called with defaults
// ONLY called with cmd, len, data from an incoming command with ver 03.
// - and this is ONLY used to recognise acks when they occur/are needed.
void Tuya_statemachine(int cmd = -1, int len = 0, unsigned char *payload = (unsigned char *)0) {
  pTuya->inStateMachine = 1;
  int state = pTuya->startup_state;
  switch (pTuya->startup_state){
    case TUYA_STARTUP_STATE_INIT://0
      if (cmd >= 0) break;
      TuyaSendCmd(TUYA_CMD_HEARTBEAT);
      pTuya->timeout = 3000;
      pTuya->timeout_state = TUYA_STARTUP_STATE_INIT;
      pTuya->startup_state = TUYA_STARTUP_STATE_WAIT_ACK_INIT;
      break;
    case TUYA_STARTUP_STATE_WAIT_ACK_INIT: // 1
      if (cmd == TUYA_CMD_HEARTBEAT){
        pTuya->timeout = 0;
        pTuya->timeout_state = TUYA_STARTUP_STATE_INIT;
        pTuya->startup_state = TUYA_STARTUP_STATE_PRODUCT;
      }
      break;
    case TUYA_STARTUP_STATE_PRODUCT: //2
      if (cmd >= 0) break;
      TuyaSendCmd(TUYA_CMD_QUERY_PRODUCT);
      pTuya->timeout = 1000;
      pTuya->timeout_state = TUYA_STARTUP_STATE_INIT;
      pTuya->startup_state = TUYA_STARTUP_STATE_WAIT_ACK_PRODUCT;
      break;
    case TUYA_STARTUP_STATE_WAIT_ACK_PRODUCT: // 3
      if (cmd == TUYA_CMD_QUERY_PRODUCT){
        pTuya->timeout = 300; // Optional - we will wait 300ms for this after TUYA_CMD_QUERY_PRODUCT response
        pTuya->timeout_state = TUYA_STARTUP_STATE_CONF; // move on at timeout.
        pTuya->startup_state = TUYA_STARTUP_STATE_WAIT_OPTIONAL_NEW_FEATURES;
      }
      break;
    case TUYA_STARTUP_STATE_WAIT_OPTIONAL_NEW_FEATURES: // 4
      /* Optional - we will wait 300ms for this after TUYA_CMD_QUERY_PRODUCT response
         After the device is powered on,
         the MCU sends this command to notify the module of feature configuration
         after the command 0x01 and before the command 0x02.*/
      if (cmd == TUYA_CMD_NEW_FEATURES){
        pTuya->timeout = 0;
        pTuya->timeout_state = TUYA_STARTUP_STATE_CONF;
        pTuya->startup_state = TUYA_STARTUP_STATE_CONF;
      }
      break;
    case TUYA_STARTUP_STATE_CONF: // 5
      if (cmd >= 0) break;
      TuyaSendCmd(TUYA_CMD_MCU_CONF);
      pTuya->timeout = 1000;
      pTuya->timeout_state = TUYA_STARTUP_STATE_INIT;
      pTuya->startup_state = TUYA_STARTUP_STATE_WAIT_ACK_CONF;
      break;
    case TUYA_STARTUP_STATE_WAIT_ACK_CONF: //
      if (cmd == TUYA_CMD_MCU_CONF){
        if (len > 0){
          pTuya->startup_state = TUYA_STARTUP_STATE_QUERY_STATE;
        } else {
          pTuya->startup_state = TUYA_STARTUP_STATE_WIFI_STATE;
        }
        pTuya->timeout = 0;
        pTuya->timeout_state = TUYA_STARTUP_STATE_INIT;
      }
      break;

    case TUYA_STARTUP_STATE_WIFI_STATE: {//
      uint8_t t = 0x04;
      if (cmd >= 0) break;
      TuyaSendCmd(TUYA_CMD_WIFI_STATE, &t, 1);
      pTuya->timeout = 1000;
      pTuya->timeout_state = TUYA_STARTUP_STATE_INIT;
      pTuya->startup_state = TUYA_STARTUP_STATE_WAIT_ACK_WIFI;
    } break;
    case TUYA_STARTUP_STATE_WAIT_ACK_WIFI://
      if (cmd == TUYA_CMD_WIFI_STATE){
        pTuya->timeout = 0;
        pTuya->timeout_state = TUYA_STARTUP_STATE_INIT;
        pTuya->startup_state = TUYA_STARTUP_STATE_QUERY_STATE;
      }
      break;
    case TUYA_STARTUP_STATE_QUERY_STATE://
      if (cmd >= 0) break;
      TuyaSendCmd(TUYA_CMD_QUERY_STATE);
      pTuya->timeout = 1000;
      pTuya->timeout_state = TUYA_STARTUP_STATE_INIT;
      pTuya->startup_state = TUYA_STARTUP_STATE_WAIT_ACK_QUERY;
      break;
    case TUYA_STARTUP_STATE_WAIT_ACK_QUERY://
      if (cmd == TUYA_CMD_STATE){
        // wait a further 500ms for the next command.
        // only on timeout, move on to runtime
        pTuya->timeout = 500;
        pTuya->timeout_state = TUYA_STARTUP_STATE_SEND_CMD;
        pTuya->startup_state = TUYA_STARTUP_STATE_WAIT_ACK_QUERY;
      }
      break;

    // we hit runtime.....
    case TUYA_STARTUP_STATE_SEND_CMD://
      if (pTuya->send_time & 1){
        TuyaSetTime();
        pTuya->send_time &= 0xfe;
        // ??? TODO - if no MCU support, we'll just wait 100ms.
        // we should *not* get a response - as this is normally a command which is requested
        pTuya->expectedResponseCmd = TUYA_CMD_SET_TIME;
        // always wait a bit before sending anything else, otherwise we may overflow the MCU input buffer.
        pTuya->timeout = 200;
        pTuya->timeout_state = TUYA_STARTUP_STATE_SEND_CMD;
        pTuya->startup_state = TUYA_STARTUP_STATE_WAIT_ACK_CMD;
        break;
      }

      if (pTuya->wifi_state != TuyaGetTuyaWifiState()) {
        pTuya->wifi_state = TuyaGetTuyaWifiState();
        AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Set WiFi LED %d (%d)"), pTuya->wifi_state, WifiState());
        if (pTuya->low_power_mode) {
          TuyaSendCmd(TUYA_LOW_POWER_CMD_WIFI_STATE, &pTuya->wifi_state, 1);
          pTuya->expectedResponseCmd = TUYA_CMD_WIFI_STATE; // ???  TODO
          pTuya->timeout = 300;
          pTuya->timeout_state = TUYA_STARTUP_STATE_SEND_CMD;
          pTuya->startup_state = TUYA_STARTUP_STATE_WAIT_ACK_CMD;
          break;
        } else {
          TuyaSendCmd(TUYA_CMD_WIFI_STATE, &pTuya->wifi_state, 1);
          pTuya->expectedResponseCmd = TUYA_CMD_WIFI_STATE;
          pTuya->timeout = 300;
          pTuya->timeout_state = TUYA_STARTUP_STATE_SEND_CMD;
          pTuya->startup_state = TUYA_STARTUP_STATE_WAIT_ACK_CMD;
          break;
        }
      }

      if (pTuya->low_power_mode) {
        if (pTuya->send_success_next_second) {
          uint8_t success = 1;
          TuyaSendCmd(TUYA_LOW_POWER_CMD_STATE, &success, 1);
          pTuya->expectedResponseCmd = TUYA_LOW_POWER_CMD_STATE; // ?? TODO - if no resp, we'll wait 300ms
          pTuya->timeout = 300;
          pTuya->timeout_state = TUYA_STARTUP_STATE_SEND_CMD;
          pTuya->startup_state = TUYA_STARTUP_STATE_WAIT_ACK_CMD;
          pTuya->send_success_next_second = false;
          break;
        }
        //TuyaSendLowPowerSuccessIfNeeded();
      }

      // send any DP chang requests
      // only if different from the received or last send values
      int i;
      for (i = 0; i < pTuya->numRxedDPids; i++){
        TUYA_DP_STORE *dp = &pTuya->DPStore[i];
        // if set requested, and MCU has reported at least once
        if (dp->toSet) {
          // if value is different
          if ((dp->rxedValueLen != dp->desiredValueLen) || memcmp(dp->rxedValue, dp->desiredValue, dp->desiredValueLen)){
            uint8_t send = 1;
            if (TuyaDpIdIsDimmer(dp->DPid)){
              uint8_t fnId = TuyaGetFuncId(dp->DPid);
              uint8_t dimindex = 0;
              if (fnId == TUYA_MCU_FUNC_DIMMER2){ // first dimmer
                dimindex = 1;
              }
              // set every time a dim value is rxed, and if just turned on
              if (pTuya->dimDelay_ms[dimindex]){
                if (pTuya->dimDebug & (1<<dimindex)){
                  AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Dim%d wait %d"), dimindex, pTuya->dimDelay_ms[dimindex]);
                  pTuya->dimDebug &= (0xff ^ (1<<dimindex));
                }
                send = 0;
              }
              // only enabled if on.
              if (!(pTuya->dimCmdEnable & (1<<dimindex))){
                if (pTuya->dimDebug & (1<<dimindex)){
                  AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Dim%d disabled"), dimindex);
                  pTuya->dimDebug &= (0xff ^ (1<<dimindex));
                }
                send = 0;
              }
            }

            if (send){
              TuyaSendState(dp->DPid,
                dp->Type,
                (uint8_t*)dp->desiredValue, dp->desiredValueLen);
              // assume it worked? maybe not
              //dp->rxedValueLen = dp->desiredValueLen;
              //memcpy(dp->rxedValue, dp->desiredValue, dp->desiredValueLen);
              dp->toSet = 0;
              pTuya->expectedResponseCmd = TUYA_CMD_STATE;
              pTuya->timeout = 300;
              pTuya->timeout_state = TUYA_STARTUP_STATE_SEND_CMD;
              pTuya->startup_state = TUYA_STARTUP_STATE_WAIT_ACK_CMD;
#ifdef TUYA_MORE_DEBUG
              MqttPublishLoggingAsync(false);
              SyslogAsync(false);
#endif

              break;
            }
          } else {
            // if equal values, ignore set
            AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: DPset ign-same value dp%d=0x%x,%d"), dp->DPid, dp->desiredValue[0], dp->desiredValueLen);
            dp->toSet = 0;
          }
        }
      }

      // triggered from second timer
      if (pTuya->send_heartbeat) {
        pTuya->send_heartbeat = 0;
        pTuya->startup_state = TUYA_STARTUP_STATE_SEND_HEARTBEAT;
        break;
      }
      break;

    case TUYA_STARTUP_STATE_WAIT_ACK_CMD:
      if (cmd == pTuya->expectedResponseCmd){
        pTuya->timeout = 0;
        pTuya->timeout_state = TUYA_STARTUP_STATE_SEND_CMD;
        pTuya->startup_state = TUYA_STARTUP_STATE_SEND_CMD;
      }
      break;

    case TUYA_STARTUP_STATE_SEND_HEARTBEAT://
      // send a heartbeat, and wait up to a second
      if (cmd >= 0) break;
      TuyaSendCmd(TUYA_CMD_HEARTBEAT);
      pTuya->timeout = 1000;
      pTuya->timeout_state = TUYA_STARTUP_STATE_INIT;
      pTuya->startup_state = TUYA_STARTUP_STATE_WAIT_ACK_HEARTBEAT;
      break;

    // wait for heartbeat return
    case TUYA_STARTUP_STATE_WAIT_ACK_HEARTBEAT://
      if (cmd == TUYA_CMD_HEARTBEAT){
        pTuya->timeout = 0;
        pTuya->timeout_state = TUYA_STARTUP_STATE_SEND_CMD;
        pTuya->startup_state = TUYA_STARTUP_STATE_SEND_CMD;
        if (len > 0 && payload[0] == 0){
          pTuya->startup_state = TUYA_STARTUP_STATE_INIT;
        }
      }
      if (cmd == TUYA_CMD_STATE){
        // dealt with in receive
      }
      break;
  }

  if (state != pTuya->startup_state){
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TYA: %d->%d-%d/%d"), state, pTuya->startup_state, pTuya->sends, pTuya->rxs);
  }

  pTuya->inStateMachine = 0;

}

void TuyaDumpDPStore(){
#ifdef TUYA_MORE_DEBUG
  for (int i = 0; i < pTuya->numRxedDPids; i++){
    TUYA_DP_STORE *dp = &pTuya->DPStore[i];

/*
            dp->DPid;
            dp->Type;
            dp->rxedValue[TUYA_MAX_STRING_SIZE];
            dp->desiredValue[TUYA_MAX_STRING_SIZE];
            dp->toSet;
            dp->rxed;
*/

    switch(dp->Type){
      case TUYA_TYPE_RAW:
        AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: DP%d T%d m:0x%X d0x%X s%d r%d"),
          dp->DPid, dp->Type, dp->rxedValue[0], dp->desiredValue[0], dp->toSet, dp->rxed);
        break;
      case TUYA_TYPE_BOOL:
      case TUYA_TYPE_ENUM:
        AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: DP%d T%d m0x%X d0x%X s%d r%d"),
          dp->DPid, dp->Type, dp->rxedValue[0], dp->desiredValue[0], dp->toSet, dp->rxed);
        break;
      case TUYA_TYPE_VALUE:
        AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: DP%d T%d m%d d%d s%d r%d"),
          dp->DPid, dp->Type, TUYAREAD32FROMPTR(dp->rxedValue), TUYAREAD32FROMPTR(dp->desiredValue), dp->toSet, dp->rxed);
        break;
      case TUYA_TYPE_STRING:
        AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: DP%d T%d m0x%X d0x%X s%d r%d"),
          dp->DPid, dp->Type, dp->rxedValue[0], dp->desiredValue[0], dp->toSet, dp->rxed);
        break;
    }
  }
#endif
}

// sets a desired value for a DPId,
// and sets a flag to say we want this value.
// this is then serviced later to ensure we don't send the same value twice.
// this solves an issue with some wallplate touch dimmers
// which are killed by sending off when the device is already off.
void TuyaPostState(uint8_t id, uint8_t type, uint8_t *value, int len = 4){
  int i;
  if (!pTuya){
    AddLog(LOG_LEVEL_ERROR, PSTR("TYA: PostState before pTuya DP %d"), id);
    return;
  }

  for (i = 0; i < pTuya->numRxedDPids; i++){
    TUYA_DP_STORE *dp = &pTuya->DPStore[i];
/*
            dp->DPid;
            dp->Type;
            dp->rxedValue[TUYA_MAX_STRING_SIZE];
            dp->desiredValue[TUYA_MAX_STRING_SIZE];
            dp->toSet;
            dp->rxed;
*/
    if (id == dp->DPid){
      if (type == dp->Type){
        if (len <= TUYA_MAX_STRING_SIZE){
          memcpy(dp->desiredValue, value, len);
          dp->desiredValueLen = len;
          dp->toSet = 1;
          AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: DP%d des v set (0x%x,%d)"), id, dp->desiredValue[0], dp->desiredValueLen);

          if (TuyaDpIdIsDimmer(id)){
            pTuya->dimDebug = 1; // enable debug to be printed once.
          }
        } else {
          AddLog(LOG_LEVEL_ERROR, PSTR("TYA: DP %d value over len (%d > %d)"), id, len, TUYA_MAX_STRING_SIZE);
        }
        break;
      } else {
        AddLog(LOG_LEVEL_ERROR, PSTR("TYA: set of dpid %d ignored - type %d != requred %d"), id, type, dp->Type);
        TuyaDumpDPStore();
        return;
      }
    }
  }
  if (i == pTuya->numRxedDPids){
    if (pTuya->numRxedDPids < TUYA_MAX_STORED_DPs){
      TUYA_DP_STORE *dp = &pTuya->DPStore[pTuya->numRxedDPids];
      dp->DPid = id;
      dp->Type = type;
      if (len <= TUYA_MAX_STRING_SIZE){
        memcpy(dp->desiredValue, value, len);
        dp->desiredValueLen = len;
        dp->toSet = 1;
        AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: NEW DP %d desiredvalue set (0x%08x len %d)"), id, dp->desiredValue[0], dp->desiredValueLen);
      } else {
        AddLog(LOG_LEVEL_ERROR, PSTR("TYA: DP %d value over len (%d > %d)"), id, len, TUYA_MAX_STRING_SIZE);
      }
      pTuya->numRxedDPids++;
#ifdef TUYA_MORE_DEBUG
      AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Add unknown dpid %d in set - num DP:%d"), id, pTuya->numRxedDPids);
#endif
    } else {
      AddLog(LOG_LEVEL_ERROR, PSTR("TYA: Max Stored DPs exceeded at set of unknown dpid %d ignored - num DP:%d"), id, pTuya->numRxedDPids);
    }
  }
#ifdef TUYA_MORE_DEBUG
  switch(type){
    case TUYA_TYPE_RAW:
    case TUYA_TYPE_BOOL:
    case TUYA_TYPE_ENUM:
      AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: dp%d to %d req"), id, *(uint8_t*)value);
      break;
    case TUYA_TYPE_VALUE:
      AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: dp%d to %d req"), id, TUYAREAD32FROMPTR(value));
      break;
    case TUYA_TYPE_STRING:
      AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: dp%d to (?) req"), id);
      break;
  }
  TuyaDumpDPStore();
#endif
}


// note - direct send using TuyaSendCmd
void TuyaSendState(uint8_t id, uint8_t type, uint8_t* value, int len)
{
  uint16_t payload_len = 4;
  uint8_t payload_buffer[8+TUYA_MAX_STRING_SIZE];
  payload_buffer[0] = id;
  payload_buffer[1] = type;
  switch (type) {
    case TUYA_TYPE_BOOL:
    case TUYA_TYPE_ENUM:
      payload_len += 1;
      payload_buffer[2] = 0x00;
      payload_buffer[3] = 0x01;
      payload_buffer[4] = value[0];
      break;
    case TUYA_TYPE_VALUE:
      payload_len += 4;
      payload_buffer[2] = 0x00;
      payload_buffer[3] = 0x04;
      // note - already reversed
      payload_buffer[4] = value[0];
      payload_buffer[5] = value[1];
      payload_buffer[6] = value[2];
      payload_buffer[7] = value[3];
      break;
    case TUYA_TYPE_STRING:
    case TUYA_TYPE_RAW:
      payload_buffer[2] = len >> 8;
      payload_buffer[3] = len & 0xFF;

      if (len <= TUYA_MAX_STRING_SIZE){
        for (uint16_t i = 0; i < len; i++) {
          payload_buffer[payload_len++] = value[i];
        }
      } else {
        AddLog(LOG_LEVEL_ERROR, PSTR("TYA: SendState: DP %d value over len (%d > %d)"), id, len, TUYA_MAX_STRING_SIZE);
        return;
      }
      break;
  }

  TuyaSendCmd(TUYA_CMD_SET_DP, payload_buffer, payload_len);
}

void TuyaSendBool(uint8_t id, bool value)
{
  TuyaPostState(id, TUYA_TYPE_BOOL, (uint8_t*)&value, 1);
}

void TuyaSendValue(uint8_t id, uint32_t value)
{
  uint32_t reversed = TUYAREAD32FROMPTR(&value);
  TuyaPostState(id, TUYA_TYPE_VALUE, (uint8_t*)&reversed, 4);
}

void TuyaSendEnum(uint8_t id, uint32_t value)
{
  TuyaPostState(id, TUYA_TYPE_ENUM, (uint8_t*)&value, 1);
}

static uint16_t convertHexStringtoBytes (uint8_t * dest, const char src[], uint16_t dest_len){
  if (NULL == dest || NULL == src || 0 == dest_len){
    return 0;
  }

  char hexbyte[3];
  hexbyte[2] = 0;
  uint16_t i;

  for (i = 0; i < dest_len; i++) {
    hexbyte[0] = src[2*i];
    hexbyte[1] = src[2*i+1];
    dest[i] = strtol(hexbyte, NULL, 16);
  }

  return i;
}

// note - send immediate, not deferred
void TuyaSendHexString(uint8_t id, const char data[]) {

  uint16_t len = strlen(data)/2;
  uint8_t value[len];
  convertHexStringtoBytes(value, data, len);
  TuyaPostState(id, TUYA_TYPE_STRING, value, len);
}

// note - send immediate, not deferred
void TuyaSendString(uint8_t id, const char data[]) {
  uint16_t len = strlen(data);
  TuyaPostState(id, TUYA_TYPE_STRING, (uint8_t*) data, len);
}

void TuyaSendRaw(uint8_t id, const char data[]) {
  const char* beginPos = strchr(data, 'x');
  if(!beginPos) {
    beginPos = strchr(data, 'X');
  }
  if(!beginPos) {
    beginPos = data;
  } else {
    beginPos += 1;
  }
  uint16_t strSize = strlen(beginPos);
  uint16_t len = strSize/2;
  uint8_t value[len];
  convertHexStringtoBytes(value, beginPos, len);
  TuyaPostState(id, TUYA_TYPE_RAW, value, len);
}

// send ANY cmd with payload from hex string
void TuyaSendRawCmd(uint8_t cmd, const char data[]) {
  uint16_t strSize = strlen(data);
  uint16_t len = strSize/2;
  uint8_t value[len];
  convertHexStringtoBytes(value, data, len);
  TuyaSendCmd(cmd, value, len, 1);
}


bool TuyaSetPower(void)
{
  bool status = false;

  uint8_t rpower = XdrvMailbox.index;
  int16_t source = XdrvMailbox.payload;

  uint8_t dpid = TuyaGetDpId(TUYA_MCU_FUNC_REL1 + TasmotaGlobal.active_device - 1);
  if (dpid == 0) dpid = TuyaGetDpId(TUYA_MCU_FUNC_REL1_INV + TasmotaGlobal.active_device - 1);
  uint8_t dev = TasmotaGlobal.active_device-1;
  uint8_t value = bitRead(rpower, dev) ^ bitRead(TasmotaGlobal.rel_inverted, dev);

  // Ignore the command if the source is SRC_SWITCH, to prevent loop from pushing state
  // from faceplate interaction. (This is probably unnecessary, as we store the latest state
  // and will not update it with the same value.)
  if (source != SRC_SWITCH && TuyaSerial && dpid) {
    TuyaSendBool(dpid, value);
    AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: post rpower%d v%d dp%d s%d d%d"), rpower, value, dpid, source, dev);
    status = true;
  } else {
    AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: rpower%d v%d dp%d ignored s%d d%d"), rpower, value, dpid, source, dev);
  }
  return status;
}

bool TuyaSetChannels(void)
{
  uint16_t hue, TuyaData;
  uint8_t sat, bri;
  uint8_t TuyaIdx = 0;
  char hex_char[15];
  bool noupd = false;

  AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: setchan"));

  bool LightMode = TuyaGetDpId(TUYA_MCU_FUNC_MODESET) != 0;
  uint8_t idx = 0;
  snprintf_P(hex_char, sizeof(hex_char), PSTR("000000000000"));

  if (LT_SERIAL1 == TasmotaGlobal.light_type) {
    pTuya->Snapshot[0] = light_state.getDimmer();
  }

  if (LT_SERIAL2 == TasmotaGlobal.light_type || LT_RGBWC == TasmotaGlobal.light_type) {
    idx = 1;
    if (LT_SERIAL2 == TasmotaGlobal.light_type &&
        Settings->flag3.pwm_multi_channels &&
        (TuyaGetDpId(TUYA_MCU_FUNC_DIMMER2) != 0)) {
      // Special setup for dual dimmer (like the MOES 2 Way Dimmer) emulating 2 PWM channels
      pTuya->Snapshot[0] = changeUIntScale(Light.current_color[0], 0, 255, 0, 100);
      pTuya->Snapshot[1] = changeUIntScale(Light.current_color[1], 0, 255, 0, 100);
    } else { // CT Light or RGBWC
      getCTRange(&pTuya->CTMin, &pTuya->CTMax); // SetOption82 - Reduce the CT range from 153..500 to 200..380 to accomodate with Alexa range
      pTuya->Snapshot[0] = light_state.getDimmer();
      pTuya->Snapshot[1] = light_state.getCT();
    }
  }

  if (LT_RGBW == TasmotaGlobal.light_type) {
    idx = 1;
    pTuya->Snapshot[0] = light_state.getDimmer(1);
    pTuya->Snapshot[1] = light_state.getDimmer(2);
  }

  if (TasmotaGlobal.light_type > LT_BASIC) {

    if (LT_RGB != TasmotaGlobal.light_type) {
      for (uint8_t i = 0; i <= idx; i++) {
        if (pTuya->Snapshot[i] != pTuya->Levels[i]) {
          if (i == 0 && LightMode && pTuya->ModeSet ) { noupd = true;}
          if (!noupd) {
            LightSerialDuty(pTuya->Snapshot[i], &hex_char[0], i+1);
            //pTuya->Levels[i] = pTuya->Snapshot[i];
          }
          noupd = false;
        }
      }
    }

    if (TasmotaGlobal.light_type >= LT_RGB) {

      // There are two types of rgb format, configure the correct one using TuyaRGB command.
      // The most common is 0HUE0SAT0BRI0 and the less common is RRGGBBFFFF6464 and sometimes both are case sensitive:
      // 0  type 1 Uppercase - 00DF00DC0244
      // 1  Type 1 Lowercase - 008003e8037a
      // 2  Type 2 Uppercase - 00FF00FFFF6464
      // 3  Type 2 Lowercase - 00e420ffff6464

      uint8_t RGBType = Settings->tuya_fnid_map[230].dpid; // Select the type of RGB payload
      char scolor[7];
      LightGetColor(scolor, 1); // Always get the color in hex format
      light_state.getHSB(&hue, &sat, &bri);
      sat = changeUIntScale(sat, 0, 255, 0, 100);
      bri = changeUIntScale(bri, 0, 255, 0, 100);

      if ((RGBType > 1 && (StrCmpNoCase(scolor, pTuya->RGBColor) != 0)) ||
          (RGBType <= 1 && ((hue != pTuya->Snapshot[2]) || (sat != pTuya->Snapshot[3]) || (bri != pTuya->Snapshot[4])) )) {

        if ((LightMode && pTuya->ModeSet) ||
            LT_RGB == TasmotaGlobal.light_type) {
          if (TuyaGetDpId(TUYA_MCU_FUNC_RGB) != 0) {
            switch (RGBType) {
            case 0: // Uppercase Type 1 payload
              snprintf_P(hex_char, sizeof(hex_char), PSTR("%04X%04X%04X"), hue, sat * 10, bri * 10);
              break;
            case 1: // Lowercase Type 1 payload
              snprintf_P(hex_char, sizeof(hex_char), PSTR("%04x%04x%04x"), hue, sat * 10, bri * 10);
              break;
            case 2: // Uppercase Type 2 payload
              snprintf_P(hex_char, sizeof(hex_char), PSTR("%sFFFF6464"), scolor);
              break;
            case 3: // Lowercase Type 2 payload
              snprintf_P(hex_char, sizeof(hex_char), PSTR("%sffff6464"), LowerCase(scolor, scolor));
              break;
            }
            memcpy_P(pTuya->RGBColor, scolor, strlen(scolor));
            pTuya->Snapshot[2] = hue;
            pTuya->Snapshot[3] = sat;
            pTuya->Snapshot[4] = bri;
          }
          LightSerialDuty(0, &hex_char[0], 3);
        }
      }
    }
  }
  return true;
}

void LightSerialDuty(uint16_t duty, char *hex_char, uint8_t TuyaIdx)
{
  uint8_t dpid = TuyaGetDpId(TUYA_MCU_FUNC_DIMMER);
  bool CTLight = false;

  if (TuyaIdx > 0 && TuyaIdx <= 2) {

    if (TuyaIdx == 2) {
      if (!Settings->flag3.pwm_multi_channels) {
      CTLight = true;
      dpid = TuyaGetDpId(TUYA_MCU_FUNC_CT);
      } else {
        dpid = TuyaGetDpId(TUYA_MCU_FUNC_DIMMER2);
    }
    }

//    if (pTuya->ignore_dim && pTuya->ignore_dimmer_cmd_timeout < millis()) {
//      pTuya->ignore_dim = false;
//    }
    pTuya->ignore_dim = false;

    if (duty > 0 && !pTuya->ignore_dim && TuyaSerial && dpid > 0) {
      if (TuyaIdx == 2 && CTLight) {
        duty = changeUIntScale(duty, pTuya->CTMin, pTuya->CTMax, Settings->dimmer_hw_max, 0);
      } else {
        duty = changeUIntScale(duty, 0, 100, Settings->dimmer_hw_min, Settings->dimmer_hw_max);
      }

      // dimming acts odd below 25(10%) - this mirrors the threshold set on the faceplate itself
      if (duty < Settings->dimmer_hw_min) {
        duty = Settings->dimmer_hw_min;
      }

      //pTuya->ignore_dimmer_cmd_timeout = millis() + 250; // Ignore serial received dim commands for the next 250ms
      if (pTuya->ModeSet &&
          (TuyaGetDpId(TUYA_MCU_FUNC_MODESET) != 0) &&
          TasmotaGlobal.light_type > LT_RGB) {
          TuyaSendEnum(TuyaGetDpId(TUYA_MCU_FUNC_MODESET), 0);
        }
        TuyaSendValue(dpid, duty);

    } else {
      if (dpid > 0 && TuyaIdx <= 2) {
        int tasduty = duty;

        pTuya->ignore_dim = false;  // reset flag

      if (TuyaIdx == 2 && CTLight) {
          duty = changeUIntScale(duty, pTuya->CTMin, pTuya->CTMax, Settings->dimmer_hw_max, 0);
      } else {
        duty = changeUIntScale(duty, 0, 100, Settings->dimmer_hw_min, Settings->dimmer_hw_max);
      }
        AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: dim skip duty%d v%d dp%d"), tasduty, duty, dpid);  // due to 0 or already set
    } else {
        AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Cannot set dimmer. Dimmer Id unknown"));
      }
    }
  }

  if (TuyaIdx == 3) {
    dpid = TuyaGetDpId(TUYA_MCU_FUNC_RGB);
    if (!pTuya->ModeSet &&
        (TuyaGetDpId(TUYA_MCU_FUNC_MODESET) != 0) &&
        TasmotaGlobal.light_type > LT_RGB) {
        TuyaSendEnum(TuyaGetDpId(TUYA_MCU_FUNC_MODESET), 1);
    }
    TuyaSendString(dpid, hex_char);
    AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: TX RGB hex %s to dpId %d"), hex_char, dpid);
  }
}

// only use manually!!!
void TuyaRequestState(uint8_t state_type)
{
  if (TuyaSerial) {
    // Get current status of MCU
#ifdef TUYA_MORE_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Read MCU state"));
#endif
    pTuya->SuspendTopic = true;
    pTuya->ignore_topic_timeout = millis() + 1000; // suppress /STAT topic for 1000ms to limit data
    switch (state_type) {
    case 0:
      TuyaSendCmd(TUYA_CMD_QUERY_STATE);
      break;
    case 8:
      TuyaSendCmd(TUYA_CMD_QUERY_PRODUCT);
      break;
    }
  }
}

void TuyaResetWifi(void)
{
  if (!Settings->flag.button_restrict) {  // SetOption1 - Control button multipress
    if (pTuya->wifiTimer > 20000){
    char scmnd[20];
    snprintf_P(scmnd, sizeof(scmnd), D_CMND_WIFICONFIG " %d", 2);
    ExecuteCommand(scmnd, SRC_BUTTON);
  }
    pTuya->wifiTimer += 10000;
    AddLog(LOG_LEVEL_ERROR, PSTR("TYA: Wifi reset button pressed.  Press repeatedly to achieve reset"));
  }
}


///////////////////////////////////////
// store all DPs received with values,
// these are used so we don't send DP value which are the same
// as the MCU has already - e.g. double 'off' can crash dimmers
void TuyaStoreRxedDP(uint8_t dpid, uint8_t type, uint8_t *data, int dpDataLen){
  int i;
  for (i = 0; i < pTuya->numRxedDPids; i++){
    TUYA_DP_STORE *dp = &pTuya->DPStore[i];
    if (dp->DPid == dpid){
      if (type != dp->Type){
        AddLog(LOG_LEVEL_ERROR, PSTR("TYA: Type change in rxed dpId=%d type %d -> %d"), dpid, dp->Type, type);
        dp->Type = type;
      }

      // if a relay command, then set dim delay, and if value is zero, disable dim command.
      // if 1, enable dim command.
      // make it work for single or dual dimmers...
      uint8_t fnId = TuyaGetFuncId(dpid);
      if ((fnId == TUYA_MCU_FUNC_REL1) ||
          (fnId == TUYA_MCU_FUNC_REL1_INV) ||
          (fnId == TUYA_MCU_FUNC_REL2) ||
          (fnId == TUYA_MCU_FUNC_REL2_INV)){
        uint8_t dimindex = 0;
        if ((fnId == TUYA_MCU_FUNC_REL2) ||
            (fnId == TUYA_MCU_FUNC_REL2_INV)){
          dimindex = 1;
        }

        pTuya->dimDelay_ms[dimindex] = pTuya->defaultDimDelay_ms;
        int value = data[0];
        if (!value){
          pTuya->dimCmdEnable &= 0xfe; (0xff ^ (1<<dimindex));
        } else {
          pTuya->dimCmdEnable |= (1<<dimindex);
        }
      }

      if (dpDataLen <= TUYA_MAX_STRING_SIZE){
        memcpy(dp->rxedValue, data, dpDataLen);
        dp->rxedValueLen = dpDataLen;
      } else {
        AddLog(LOG_LEVEL_ERROR, PSTR("TYA: DP len exceeded dpId=%d type %d (%d > %d"), dpid, type, dpDataLen, TUYA_MAX_STRING_SIZE);
      }
      dp->rxed = 1;
      break;
    }
  }
  if (i == pTuya->numRxedDPids){
    if (i < TUYA_MAX_STORED_DPs){
      TUYA_DP_STORE *dp = &pTuya->DPStore[i];
      dp->DPid = dpid;
      dp->Type = type;
      if (dpDataLen <= TUYA_MAX_STRING_SIZE){
        memcpy(dp->rxedValue, data, dpDataLen);
        dp->rxedValueLen = dpDataLen;
      } else {
        AddLog(LOG_LEVEL_ERROR, PSTR("TYA: DP len exceeded dpId=%d type %d (%d > %d"), dpid, type, dpDataLen, TUYA_MAX_STRING_SIZE);
      }
      dp->rxed = 1;
      pTuya->numRxedDPids++;
    } else {
      AddLog(LOG_LEVEL_ERROR, PSTR("TYA: Max stored DPs exceeded dpId=%d type %d"), dpid, type);
    }
  }
  TuyaDumpDPStore();
}


////////////////////////////////////////////////
// process ONE rxed DP value
void TuyaProcessRxedDP(uint8_t dpid, uint8_t type, uint8_t *data, int dpDataLen){
  char scmnd[20];
  bool PowerOff = false;
  bool tuya_energy_enabled = (XNRG_32 == TasmotaGlobal.energy_driver);
  uint8_t fnId = TuyaGetFuncId(dpid);
  uint32_t value = 0;

  if (TuyaFnIdIsDimmer(fnId)){
    if (fnId == TUYA_MCU_FUNC_DIMMER){ // first dimmer
      pTuya->dimDelay_ms[0] = pTuya->defaultDimDelay_ms;
    } else {
      if (fnId == TUYA_MCU_FUNC_DIMMER2){ // second dimmer
        pTuya->dimDelay_ms[1] = pTuya->defaultDimDelay_ms;
      } else { // must be other light, single dimmable thing
        pTuya->dimDelay_ms[0] = pTuya->defaultDimDelay_ms;
      }
    }
  }

  ////////////////////
  // get value for types that fit in 4 byte as uint32_t
  switch(type){
    case TUYA_TYPE_RAW: // variable length combined?
      break;
    case TUYA_TYPE_BOOL: // 1 = 1 byte bool
      value = data[0];
      break;
    case TUYA_TYPE_VALUE: // 2 = 32 bit int
      value = TUYAREAD32FROMPTR(data);
      break;
    case TUYA_TYPE_STRING: // 3
      break;
    case TUYA_TYPE_ENUM: // 4 = 1 byte value
      value = data[0];
      break;
  }
  //////////////////


  // incorporated into logs below to save one log line.
  //AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: fnId=%d is set for dpId=%d"), fnId, dpid);
  switch(type) {
    case TUYA_TYPE_RAW: {
#ifdef USE_ENERGY_SENSOR
        if (tuya_energy_enabled && fnId == TUYA_MCU_FUNC_POWER_COMBINED) {
        if (dpDataLen >= 8) {
          // data is pTuya->buffer[dpidStart + 4];
          uint16_t tmpVol = data[4 - 4] << 8 | data[5 - 4];
          uint16_t tmpCur = data[7 - 4] << 8 | data[8 - 4];
          uint16_t tmpPow = data[10 - 4] << 8 | data[11 - 4];
/*          uint16_t tmpVol = pTuya->buffer[dpidStart + 4] << 8 | pTuya->buffer[dpidStart + 5];
          uint16_t tmpCur = pTuya->buffer[dpidStart + 7] << 8 | pTuya->buffer[dpidStart + 8];
          uint16_t tmpPow = pTuya->buffer[dpidStart + 10] << 8 | pTuya->buffer[dpidStart + 11];*/
          Energy->voltage[0] = (float)tmpVol / 10;
          Energy->current[0] = (float)tmpCur / 1000;
          Energy->active_power[0] = (float)tmpPow;

          AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: fnId=%d Rx ID=%d Voltage=%d Current=%d Active_Power=%d"), fnId, dpid, tmpVol, tmpCur, tmpPow);

          if (RtcTime.valid) {
            if (pTuya->lastPowerCheckTime != 0 && Energy->active_power[0] > 0) {
              Energy->kWhtoday[0] += Energy->active_power[0] * (float)(Rtc.utc_time - pTuya->lastPowerCheckTime) / 36.0;
              EnergyUpdateToday();
            }
            pTuya->lastPowerCheckTime = Rtc.utc_time;
          }
        } else {
          AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: fnId=%d Rx ID=%d INV_LEN=%d"), fnId, dpid, dpDataLen);
        }
        }
        #endif // USE_ENERGY_SENSOR
    } break;

    case TUYA_TYPE_BOOL: {  // Data Type 1
        if (fnId >= TUYA_MCU_FUNC_REL1 && fnId <= TUYA_MCU_FUNC_REL8) {
          AddLog(LOG_LEVEL_DEBUG, PSTR("T:fn%d Relay%d-->M%s T%s"), fnId, fnId - TUYA_MCU_FUNC_REL1 + 1, value?"On":"Off",bitRead(TasmotaGlobal.power, fnId - TUYA_MCU_FUNC_REL1)?"On":"Off");
          if (value != bitRead(TasmotaGlobal.power, fnId - TUYA_MCU_FUNC_REL1)) {
            if (!value) { PowerOff = true; }
            ExecuteCommandPower(fnId - TUYA_MCU_FUNC_REL1 + 1, value, SRC_SWITCH);
          }
        } else if (fnId >= TUYA_MCU_FUNC_REL1_INV && fnId <= TUYA_MCU_FUNC_REL8_INV) {
          AddLog(LOG_LEVEL_DEBUG, PSTR("T:fn%d Relay%d-Inv-->M%s T%s"), fnId, fnId - TUYA_MCU_FUNC_REL1_INV + 1, value?"Off":"On",bitRead(TasmotaGlobal.power, fnId - TUYA_MCU_FUNC_REL1_INV) ^ 1?"Off":"On");
          if (value != bitRead(TasmotaGlobal.power, fnId - TUYA_MCU_FUNC_REL1_INV) ^ 1) {
            ExecuteCommandPower(fnId - TUYA_MCU_FUNC_REL1_INV + 1, value ^ 1, SRC_SWITCH);
            if (value) { PowerOff = true; }
          }
        } else if (fnId >= TUYA_MCU_FUNC_SWT1 && fnId <= TUYA_MCU_FUNC_SWT4) {
          uint32_t switch_index = fnId - TUYA_MCU_FUNC_SWT1;
          uint32_t switch_state = SwitchGetState(switch_index);
          AddLog(LOG_LEVEL_DEBUG, PSTR("T:fn%d Switch%d --> M%d T%d"), fnId, switch_index +1, value, switch_state);
          if (switch_state != value) {
            SwitchSetState(switch_index, value);
          }
        }
      //if (PowerOff) { pTuya->ignore_dimmer_cmd_timeout = millis() + 250; }
    } break;

    case TUYA_TYPE_VALUE: {  // Data Type 2
      uint32_t packetValue = value; // TYpe 2 is a 32 bit integer
        uint8_t dimIndex;
        bool SnsUpdate = false;

        if ((fnId >= TUYA_MCU_FUNC_TEMP) && (fnId <= TUYA_MCU_FUNC_TIMER4)) {      // Sensors start from fnId 71
        if (packetValue != pTuya->Sensors[fnId-71]) {
          pTuya->SensorsValid[fnId-71] = true;
          pTuya->Sensors[fnId-71] = packetValue;
            SnsUpdate = true;
          }
        }

        if (SnsUpdate) {
          char sname[20];
          char tempval[5];
          uint8_t res;
          bool dont_publish = Settings->flag5.tuyasns_no_immediate;

          if (TasmotaGlobal.uptime < 8) { // delay to avoid multiple topics at the same time at boot time
            return;
          } else {
            if (fnId > 80 || fnId == 74 || fnId == 72) {
              dont_publish = false;
            }
            if (fnId > 74) {
              res = 0;
            } else if (fnId > 72) {
              res = Settings->flag2.humidity_resolution;
            } else if (fnId == 72) {
              res = Settings->mbflag2.temperature_set_res;
            } else {
              res = Settings->flag2.temperature_resolution;
            }
            GetTextIndexed(sname, sizeof(sname), (fnId-71), kTuyaSensors);
            ResponseClear(); // Clear retained message
            Response_P(PSTR("{\"TuyaSNS\":{\"%s\":%s}}"), sname, dtostrfd(TuyaAdjustedTemperature(packetValue, res), res, tempval)); // sensor update is just on change
            if (dont_publish) {
              XdrvRulesProcess(0);
            } else {
              MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR(D_CMND_SENSOR));
            }
          }
        }

        if (fnId == TUYA_MCU_FUNC_DIMMER || fnId == TUYA_MCU_FUNC_REPORT1) { dimIndex = 0; }

        if (fnId == TUYA_MCU_FUNC_DIMMER2 || fnId == TUYA_MCU_FUNC_REPORT2 || fnId == TUYA_MCU_FUNC_CT) { dimIndex = 1; }

        if (dimIndex == 1 && !Settings->flag3.pwm_multi_channels) {
        pTuya->Levels[1] = changeUIntScale(packetValue, 0, Settings->dimmer_hw_max, pTuya->CTMax, pTuya->CTMin);
        } else {
        pTuya->Levels[dimIndex] = changeUIntScale(packetValue, Settings->dimmer_hw_min, Settings->dimmer_hw_max, 0, 100);
        }

      AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: fn%d value %d dp%d "), fnId, packetValue, dpid);

        if ((fnId == TUYA_MCU_FUNC_DIMMER) || (fnId == TUYA_MCU_FUNC_REPORT1) ||
            (fnId == TUYA_MCU_FUNC_DIMMER2) || (fnId == TUYA_MCU_FUNC_REPORT2) ||
            (fnId == TUYA_MCU_FUNC_CT) || (fnId == TUYA_MCU_FUNC_WHITE)) {

        // SetOption54 - Apply SetOption20 settings to Tuya device / SetOption131 Allow save dimmer = 0 receved by MCU
        if (1) {//pTuya->ignore_dimmer_cmd_timeout < millis()) {
          if ((TasmotaGlobal.power || Settings->flag3.tuya_apply_o20) &&
              ((pTuya->Levels[dimIndex] > 0 || Settings->flag5.tuya_allow_dimmer_0) &&
               (pTuya->Levels[dimIndex] != pTuya->Snapshot[dimIndex]))) {
            //pTuya->ignore_dim = true;
              TasmotaGlobal.skip_light_fade = true;

              scmnd[0] = '\0';
              if ((fnId == TUYA_MCU_FUNC_DIMMER) || (fnId == TUYA_MCU_FUNC_REPORT1)) {
              if (Settings->flag3.pwm_multi_channels && (abs(pTuya->Levels[0] - changeUIntScale(Light.current_color[0], 0, 255, 0, 100))) > 1) {
                snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_CHANNEL "1 %d"), pTuya->Levels[0]);
                }
              else if ((abs(pTuya->Levels[0] - light_state.getDimmer())) > 1) {
                snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_DIMMER "3 %d"), pTuya->Levels[0]);
                }
              }
              if (((fnId == TUYA_MCU_FUNC_DIMMER2) || (fnId == TUYA_MCU_FUNC_REPORT2)) &&
                  Settings->flag3.pwm_multi_channels && (abs(pTuya->Levels[1] - changeUIntScale(Light.current_color[1], 0, 255, 0, 100))) > 1) {
              snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_CHANNEL "2 %d"), pTuya->Levels[1]);
              }
            if ((fnId == TUYA_MCU_FUNC_CT) && (abs(pTuya->Levels[1] - light_state.getCT())) > 1) {
              snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_COLORTEMPERATURE " %d"), pTuya->Levels[1]);
              }
            if ((fnId == TUYA_MCU_FUNC_WHITE) && (abs(pTuya->Levels[1] - light_state.getDimmer(2))) > 1) {
              snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_WHITE " %d"), pTuya->Levels[1]);
              }
              if (scmnd[0] != '\0') {
                ExecuteCommand(scmnd, SRC_SWITCH);
              }
            }
          pTuya->Snapshot[dimIndex] = pTuya->Levels[dimIndex];
          }
        }
  #ifdef USE_ENERGY_SENSOR
        else if (tuya_energy_enabled && fnId == TUYA_MCU_FUNC_VOLTAGE) {
          Energy->voltage[0] = (float)packetValue / 10;
        AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: fnId=%d Rx ID=%d Voltage=%d"), fnId, dpid, packetValue);
        } else if (tuya_energy_enabled && fnId == TUYA_MCU_FUNC_CURRENT) {
          Energy->current[0] = (float)packetValue / 1000;
        AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: fnId=%d Rx ID=%d Current=%d"), fnId, dpid, packetValue);
        } else if (tuya_energy_enabled && fnId == TUYA_MCU_FUNC_POWER) {
          Energy->active_power[0] = (float)packetValue / 10;
        AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: fnId=%d Rx ID=%d Active_Power=%d"), fnId, dpid, packetValue);

          if (RtcTime.valid) {
          if (pTuya->lastPowerCheckTime != 0 && Energy->active_power[0] > 0) {
            Energy->kWhtoday[0] += Energy->active_power[0] * (float)(Rtc.utc_time - pTuya->lastPowerCheckTime) / 36.0;
              EnergyUpdateToday();
            }
          pTuya->lastPowerCheckTime = Rtc.utc_time;
          }
        } else if (tuya_energy_enabled && fnId == TUYA_MCU_FUNC_POWER_TOTAL) {
          Energy->import_active[0] = (float)packetValue / 100;
        AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: fnId=%d Rx ID=%d Total_Power=%d"), fnId, dpid, packetValue);
          EnergyUpdateTotal();
        }
  #endif // USE_ENERGY_SENSOR
    } break;

    case TUYA_TYPE_STRING: {  // Data Type 3
      const unsigned char *dpData = (unsigned char*)data;
      AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: (str) fnId=%d is set for dpId=%d"), dpid);
        if ((TuyaGetDpId(TUYA_MCU_FUNC_RGB) != 0)) {

          uint8_t RGBType = Settings->tuya_fnid_map[230].dpid; // Select the type of hex configured
          char RgbData[15];
          char RGB[7];
          char HSB1[5], HSB2[5], HSB3[5];
          scmnd[0] = '\0';
          snprintf_P(RgbData, sizeof(RgbData), PSTR("%.*s"), dpDataLen, dpData);

          if (RGBType <= 1 && dpDataLen == 12) {
            snprintf_P(HSB1, sizeof(HSB1), PSTR("%.4s\n"), &RgbData[0]);
            snprintf_P(HSB2, sizeof(HSB2), PSTR("%.4s\n"), &RgbData[4]);
            snprintf_P(HSB3, sizeof(HSB3), PSTR("%.4s\n"), &RgbData[8]);
          if ((pTuya->Snapshot[2] != ((int)strtol(HSB1, NULL, 16)) ||
              (pTuya->Snapshot[3] != ((int)strtol(HSB2, NULL, 16)) / 10) ||
              (pTuya->Snapshot[4] !=((int)strtol(HSB3, NULL, 16)) / 10)) ) {
            pTuya->Snapshot[2] = ((int)strtol(HSB1, NULL, 16));
            pTuya->Snapshot[3] = ((int)strtol(HSB2, NULL, 16)) / 10;
            pTuya->Snapshot[4] = ((int)strtol(HSB3, NULL, 16)) / 10;
            snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_HSBCOLOR " %d,%d,%d"), ((int)strtol(HSB1, NULL, 16)),
                      ((int)strtol(HSB2, NULL, 16)) / 10, ((int)strtol(HSB3, NULL, 16)) / 10);
            }
          }
          if (RGBType > 1 && dpDataLen == 14) {
            snprintf_P(RGB, sizeof(RGB), PSTR("%.6s\n"), &RgbData[0]);
          if (StrCmpNoCase(RGB, pTuya->RGBColor) != 0) {
              snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_COLOR " %s"), RGB);
            memcpy_P(pTuya->RGBColor, RGB, strlen(RGB));
            }
          }
          if (scmnd[0] != '\0') {
            ExecuteCommand(scmnd, SRC_SWITCH);
          }
        }

    } break;

    case TUYA_TYPE_ENUM: {  // Data Type 4
      AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: (enum) fnId=%d is set for dpId=%d"), dpid);
        if ((fnId == TUYA_MCU_FUNC_MODESET)) { // Toggle light type
        pTuya->ModeSet = value;
        pTuya->Levels[3] = value;
        }
        if ((fnId >= TUYA_MCU_FUNC_ENUM1) && (fnId <= TUYA_MCU_FUNC_ENUM4)) {
          for (uint8_t i = 0; i <= 3; i++) {
            bool noupdate = false;
            if ((TUYA_MCU_FUNC_ENUM1 + i) == fnId) {
            if (pTuya->EnumState[i] != value) {
              pTuya->EnumState[i] = value;
              snprintf_P(scmnd, sizeof(scmnd), PSTR(D_PRFX_TUYA D_CMND_TUYA_ENUM "%d %d"), i+1, value);
                ExecuteCommand(scmnd, SRC_SWITCH);
              }
            }
          }
        }
    } break;
  }
}


void TuyaProcessStatePacket(void) {
  uint8_t dpidStart = 6;

  while (dpidStart + 4 < pTuya->byte_counter) {
    uint8_t dpid = pTuya->buffer[dpidStart];
    uint8_t type = pTuya->buffer[dpidStart + 1];
    uint8_t *data = pTuya->buffer + dpidStart + 4;
    uint16_t dpDataLen = pTuya->buffer[dpidStart + 2] << 8 | pTuya->buffer[dpidStart + 3];
    TuyaStoreRxedDP(dpid, type, data, dpDataLen);
    TuyaProcessRxedDP(dpid, type, data, dpDataLen);
      dpidStart += dpDataLen + 4;
  }
}

void TuyaLowPowerModePacketProcess(void) {
  switch (pTuya->buffer[3]) {
    case TUYA_CMD_QUERY_PRODUCT:
      TuyaHandleProductInfoPacket();
      //TuyaSetWifiLed();
      break;

    case TUYA_LOW_POWER_CMD_STATE:
      TuyaProcessStatePacket();
      pTuya->send_success_next_second = true;
      break;
  }

}

void TuyaHandleProductInfoPacket(void) {
  uint16_t dataLength = pTuya->buffer[4] << 8 | pTuya->buffer[5];
  char *data = (char *)&pTuya->buffer[6];
  AddLog(LOG_LEVEL_INFO, PSTR("TYA: MCU Product ID: %.*s"), dataLength, data);
}

void TuyaNormalPowerModePacketProcess(void)
{
  switch (pTuya->buffer[3]) {
    case TUYA_CMD_QUERY_PRODUCT:
      TuyaHandleProductInfoPacket();
      // next send now done as part of startup state machine
      //TuyaSendCmd(TUYA_CMD_MCU_CONF);
      break;

    case TUYA_CMD_HEARTBEAT:
#ifdef TUYA_MORE_DEBUG
      AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Hbt"));
#endif
      if (pTuya->buffer[6] == 0) {
        AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Detected MCU restart"));
        pTuya->wifi_state = -2;
      }
      break;

    case TUYA_CMD_STATE:
      TuyaProcessStatePacket();
      break;

    case TUYA_CMD_WIFI_RESET:
      AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: RX WiFi Reset"));
      // ack MCU request immediately
      // https://developer.tuya.com/en/docs/iot/wifi-module-mcu-development-overview-for-homekit?id=Kaa8fvusmgapc
      // set 'noerror', as we are responding
      TuyaSendCmd(TUYA_CMD_WIFI_RESET, nullptr, 0, 1);
      TuyaResetWifi();
      break;
    case TUYA_CMD_WIFI_SELECT:
      AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: RX WiFi Select"));
      // ack MCU request immediately
      // https://developer.tuya.com/en/docs/iot/wifi-module-mcu-development-overview-for-homekit?id=Kaa8fvusmgapc
      // set 'noerror', as we are responding
      TuyaSendCmd(TUYA_CMD_WIFI_SELECT, nullptr, 0, 1);
      TuyaResetWifi();
      break;

    case TUYA_CMD_WIFI_STATE:
      AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: RX WiFi LED set ACK"));
      pTuya->wifi_state = TuyaGetTuyaWifiState();
      break;

    case TUYA_CMD_MCU_CONF:
      AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: RX MCU configuration Mode=%d"), pTuya->buffer[5]);

      if (pTuya->buffer[5] == 2) { // Processing by ESP module mode
        uint8_t led1_gpio = pTuya->buffer[6];
        uint8_t key1_gpio = pTuya->buffer[7];
        bool key1_set = false;
        bool led1_set = false;
        for (uint32_t i = 0; i < nitems(Settings->my_gp.io); i++) {
          if (Settings->my_gp.io[i] == AGPIO(GPIO_LED1)) led1_set = true;
          else if (Settings->my_gp.io[i] == AGPIO(GPIO_KEY1)) key1_set = true;
        }
        if (!Settings->my_gp.io[led1_gpio] && !led1_set) {
          Settings->my_gp.io[led1_gpio] = AGPIO(GPIO_LED1);
          TasmotaGlobal.restart_flag = 2;
        }
        if (!Settings->my_gp.io[key1_gpio] && !key1_set) {
          Settings->my_gp.io[key1_gpio] = AGPIO(GPIO_KEY1);
          TasmotaGlobal.restart_flag = 2;
        }
      }
      // next send now done as part of startup state machine
      //TuyaRequestState(0);
      break;
#ifdef USE_TUYA_TIME
    case TUYA_CMD_SET_TIME:
      // send from state machine.
      pTuya->send_time = 3;
      //TuyaSetTime();
      break;
#endif
    default:
      AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: RX unknown command"));
  }
}

/*********************************************************************************************\
 * API Functions
\*********************************************************************************************/

bool TuyaModuleSelected(void) {
#ifdef ESP8266
  if (TUYA_DIMMER != TasmotaGlobal.module_type) { return false; }

  if (!PinUsed(GPIO_TUYA_RX) || !PinUsed(GPIO_TUYA_TX)) {  // fallback to hardware-serial if not explicitly selected
    SetPin(1, AGPIO(GPIO_TUYA_TX));
    SetPin(3, AGPIO(GPIO_TUYA_RX));
    Settings->my_gp.io[1] = AGPIO(GPIO_TUYA_TX);
    Settings->my_gp.io[3] = AGPIO(GPIO_TUYA_RX);
    TasmotaGlobal.restart_flag = 2;
  }
#endif
  if (!PinUsed(GPIO_TUYA_RX) || !PinUsed(GPIO_TUYA_TX)) { return false; }
  // allocate and initialise the sturcture only if we will use it.
  init_tuya_struct();

  if (!pTuya){ return false; }

  if (TuyaGetDpId(TUYA_MCU_FUNC_DIMMER) == 0 && TUYA_DIMMER_ID > 0) {
    TuyaAddMcuFunc(TUYA_MCU_FUNC_DIMMER, TUYA_DIMMER_ID);
  }

  bool relaySet = false;

  for (uint8_t i = 0 ; i < MAX_TUYA_FUNCTIONS; i++) {
    if ((Settings->tuya_fnid_map[i].fnid >= TUYA_MCU_FUNC_REL1 && Settings->tuya_fnid_map[i].fnid <= TUYA_MCU_FUNC_REL8 ) ||
    (Settings->tuya_fnid_map[i].fnid >= TUYA_MCU_FUNC_REL1_INV && Settings->tuya_fnid_map[i].fnid <= TUYA_MCU_FUNC_REL8_INV )) {
      relaySet = true;
      UpdateDevicesPresent(1);
    }
  }

  if (!relaySet && TuyaGetDpId(TUYA_MCU_FUNC_DUMMY) == 0) { //by default the first relay is created automatically the dummy let remove it if not needed
    TuyaAddMcuFunc(TUYA_MCU_FUNC_REL1, 1);
    UpdateDevicesPresent(1);
    SettingsSaveAll();
  }

  // Possible combinations for Lights:
  // 0: NONE = LT_BASIC
  // 1: DIMMER = LT_SERIAL1 - Common one channel dimmer
  // 2: DIMMER, DIMMER2 = LT_SERIAL2 - Two channels dimmer (special setup used with SetOption68)
  // 3: DIMMER, CT = LT_SERIAL2 - Dimmable light and White Color Temperature
  // 4: DIMMER, RGB = LT_RGB - RGB Light
  // 5: DIMMER, RGB, CT = LT_RGBWC - RGB LIght and White Color Temperature
  // 6: DIMMER, RGB, WHITE = LT_RGBW - RGB LIght and White

  if (TuyaGetDpId(TUYA_MCU_FUNC_DIMMER) != 0) {
    if (TuyaGetDpId(TUYA_MCU_FUNC_RGB) != 0) {
      if (TuyaGetDpId(TUYA_MCU_FUNC_CT) != 0) {
        TasmotaGlobal.light_type = LT_RGBWC;
      } else if (TuyaGetDpId(TUYA_MCU_FUNC_WHITE) != 0) {
        TasmotaGlobal.light_type = LT_RGBW;
      } else { TasmotaGlobal.light_type = LT_RGB; }
    } else if (TuyaGetDpId(TUYA_MCU_FUNC_CT) != 0 || TuyaGetDpId(TUYA_MCU_FUNC_DIMMER2) != 0) {
      if (TuyaGetDpId(TUYA_MCU_FUNC_RGB) != 0) {
        TasmotaGlobal.light_type = LT_RGBWC;
      } else { TasmotaGlobal.light_type = LT_SERIAL2; }
    } else { TasmotaGlobal.light_type = LT_SERIAL1; }
  } else {
    TasmotaGlobal.light_type = LT_BASIC;
  }

  if (TuyaGetDpId(TUYA_MCU_FUNC_LOWPOWER_MODE) != 0) {
    pTuya->low_power_mode = true;
    Settings->flag3.fast_power_cycle_disable = true;  // SetOption65 - Disable fast power cycle detection for device reset
  }

  UpdateDevices();
  return true;
}

void TuyaInit(void) {
  int baudrate = 9600;
  if (Settings->flag4.tuyamcu_baudrate) { baudrate = 115200; }  // SetOption97 - Set Baud rate for TuyaMCU serial communication (0 = 9600 or 1 = 115200)

  TuyaSerial = new TasmotaSerial(Pin(GPIO_TUYA_RX), Pin(GPIO_TUYA_TX), 2);
  if (TuyaSerial->begin(baudrate)) {
    if (TuyaSerial->hardwareSerial()) { ClaimSerial(); }
#ifdef ESP32
    AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Serial UART%d"), TuyaSerial->getUart());
#endif  // ESP32
    // Get MCU Configuration
    pTuya->SuspendTopic = true;
    pTuya->ignore_topic_timeout = millis() + 1000; // suppress /STAT topic for 1000ms to avoid data overflow
    AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Request MCU configuration at %d bps"), baudrate);

    pTuya->heartbeat_timer = 0; // init heartbeat timer when dimmer init is done
    return;
  }
  pTuya->active = false;
}

// dump a buffer to debug
void TuyaDump(unsigned char *buffer, int len){
  AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Raw Data %*_H"), len, buffer);
}

// here we have a complete packet with valid checksum
void TuyaProcessCommand(unsigned char *buffer){
  uint16_t len = buffer[4] << 8 | buffer[5];
  uint16_t totallen = len + 7;
  uint8_t ver = buffer[2];
  uint8_t cmd = buffer[3];

  pTuya->rxs ++;


  // see if we are awaiting this cmd ack
  // some MCU send 0x00 in ver field,
  // so we could assume some send 0x01, some 0x02, some 0x03?
  // I'v not seen any docs about what the VER means!....
  //if (0x03 == ver) {
    Tuya_statemachine(cmd, len, (unsigned char *)buffer+6);
  //}

  Response_P(PSTR("{\"" D_JSON_TUYA_MCU_RECEIVED "\":{\"Data\":\"%*_H\",\"Cmnd\":%d"), totallen, buffer, cmd);

  uint16_t DataVal = 0;
  uint8_t dpId = 0;
  uint8_t dpDataType = 0;
  char DataStr[15];
  bool isCmdToSuppress = false;

  if (len > 0) {
    ResponseAppend_P(PSTR(",\"CmndData\":\"%*_H\""), len, (unsigned char*)&buffer[6]);
    if (TUYA_CMD_STATE == cmd) {
      //55 AA 03 07 00 0D 01 04 00 01 02 02 02 00 04 00 00 00 1A 40
      // 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19
      uint8_t dpidStart = 6;
      while (dpidStart + 4 < totallen-1) {
        dpId = buffer[dpidStart];
        dpDataType = buffer[dpidStart + 1];
        uint16_t dpDataLen = buffer[dpidStart + 2] << 8 | buffer[dpidStart + 3];
        const unsigned char *dpData = (unsigned char*)&buffer[dpidStart + 4];

        if (TUYA_CMD_STATE == buffer[3]) {
          ResponseAppend_P(PSTR(",\"DpType%uId%u\":"), dpDataType, dpId);
          if (TUYA_TYPE_BOOL == dpDataType && dpDataLen == 1) {
            ResponseAppend_P(PSTR("%u"), dpData[0]);
            DataVal = dpData[0];
          } else if (TUYA_TYPE_VALUE == dpDataType && dpDataLen == 4) {
            uint32_t dpValue = (uint32_t)dpData[0] << 24 | (uint32_t)dpData[1] << 16 | (uint32_t)dpData[2] << 8 | (uint32_t)dpData[3] << 0;
            ResponseAppend_P(PSTR("%u"), dpValue);
            DataVal = dpValue;
          } else if (TUYA_TYPE_STRING == dpDataType) {
            ResponseAppend_P(PSTR("\"%.*s\""), dpDataLen, dpData);
            snprintf_P(DataStr, sizeof(DataStr), PSTR("%.*s"), dpDataLen, dpData);
          } else if (TUYA_TYPE_ENUM == dpDataType && dpDataLen == 1) {
            ResponseAppend_P(PSTR("%u"), dpData[0]);
            DataVal = dpData[0];
          } else {
            ResponseAppend_P(PSTR("\"0x%*_H\""), dpDataLen, dpData);
            snprintf_P(DataStr, sizeof(DataStr), PSTR("%*_H"), dpDataLen, dpData);
          }
        }

        ResponseAppend_P(PSTR(",\"%d\":{\"DpId\":%d,\"DpIdType\":%d,\"DpIdData\":\"%*_H\""), dpId, dpId, dpDataType, dpDataLen, dpData);
        if (TUYA_TYPE_STRING == dpDataType) {
          ResponseAppend_P(PSTR(",\"Type3Data\":\"%.*s\""), dpDataLen, dpData);
        }
        ResponseAppend_P(PSTR("}"));
        dpidStart += dpDataLen + 4;
      }
    }
  }
  ResponseAppend_P(PSTR("}}"));

  // SetOption66 - Enable TuyaMcuReceived messages over Mqtt
  if (Settings->flag3.tuya_serial_mqtt_publish) {
/*
    for (uint8_t cmdsID = 0; sizeof(TuyaExcludeCMDsFromMQTT) > cmdsID; cmdsID++){
	  if (TuyaExcludeCMDsFromMQTT[cmdsID] == cmd) {
        isCmdToSuppress = true;
        break;
      }
    }
*/
    for (uint8_t cmdsID = 0; cmdsID < sizeof(TuyaExcludeCMDsFromMQTT); cmdsID++) {
      if (pgm_read_byte(TuyaExcludeCMDsFromMQTT +cmdsID) == cmd) {
        isCmdToSuppress = true;
        break;
      }
    }

    // SetOption137 - (Tuya) When Set, avoid the (MQTT-) publish of defined Tuya CMDs
    // (see TuyaExcludeCMDsFromMQTT) if SetOption66 is active
    if (!(isCmdToSuppress && Settings->flag5.tuya_exclude_from_mqtt)) {
      MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_TUYA_MCU_RECEIVED));
    }
  } else {
    AddLog(LOG_LEVEL_DEBUG, ResponseData());
  }
  XdrvRulesProcess(0);

  if (dpId != 0 && Settings->tuyamcu_topic) { // Publish a /STAT Topic ready to use for any home automation system
    if (!pTuya->SuspendTopic) {
      char scommand[13];
      snprintf_P(scommand, sizeof(scommand), PSTR("DpType%uId%u"), dpDataType, dpId);
      if (dpDataType != 3 && dpDataType != 5) { Response_P(PSTR("%u"), DataVal); }
      else { Response_P(PSTR("%s"), DataStr); }
      MqttPublishPrefixTopic_P(STAT, scommand);
    }
  }

  if (!pTuya->low_power_mode) {
    TuyaNormalPowerModePacketProcess();
  } else {
    TuyaLowPowerModePacketProcess();
  }
}

// processes one byte of Tuya input.
// on packet complete with good CS, calls TuyaProcessCommand
void TuyaProcessByte(unsigned char serial_in_byte){

/* I don't think it's safe to do this with RGB possibly containing 55AA in the message.
  if ((pTuya->cmd_status != 1) &&
      (pTuya->lastByte == 0x55) &&
      (serial_in_byte == 0xAA)){
    AddLog(LOG_LEVEL_ERROR, PSTR("TYA: Reset by unexpected 55AA"));
    TuyaDump(pTuya->buffer, pTuya->byte_counter);

    pTuya->byte_counter = 0;
    pTuya->buffer[pTuya->byte_counter++] = 0x55;
    pTuya->cmd_status = 1;
    pTuya->errorcnt ++;
  }
*/

  unsigned int  now = millis();
  if ((pTuya->cmd_status != 0) && (now - pTuya->lastByteTime > TUYA_BYTE_TIMEOUT_MS)){
    AddLog(LOG_LEVEL_ERROR, PSTR("TYA: Reset by char timeout %u > %dms"), now - pTuya->lastByteTime, TUYA_BYTE_TIMEOUT_MS);
    TuyaDump(pTuya->buffer, pTuya->byte_counter);
    pTuya->cmd_status = 0;
    pTuya->errorcnt ++;
  }
  pTuya->lastByteTime = now;

  switch(pTuya->cmd_status){
    case 0: {// wait 55
      if (serial_in_byte == 0x55) {            // Start TUYA Packet
        pTuya->cmd_status = 1;
        pTuya->byte_counter = 0;
        pTuya->buffer[pTuya->byte_counter++] = serial_in_byte;
      } else {
        AddLog(LOG_LEVEL_ERROR, PSTR("TYA: E55 0x%02X"), serial_in_byte);
        pTuya->errorcnt ++;
      }
    } break;
    case 1: {// wait AA
      if (serial_in_byte == 0xAA) {            // Start TUYA Packet
        pTuya->cmd_status = 2;
        pTuya->buffer[pTuya->byte_counter++] = serial_in_byte;
        pTuya->cmd_checksum = 0xFF;
      } else {
        // no AA, return to wait for 55
        AddLog(LOG_LEVEL_ERROR, PSTR("TYA: EAA 0x%02X"), serial_in_byte);
        pTuya->errorcnt ++;
        pTuya->cmd_status = 0;
      }
    } break;
    case 2: {// wait len
      if (pTuya->byte_counter == 5) { // Get length of data
        pTuya->data_len = serial_in_byte;
        pTuya->data_len += pTuya->buffer[4] << 8;
        if (pTuya->data_len == 0){
          // straight to CS
          pTuya->cmd_status = 4;
        } else {
          pTuya->cmd_status = 3;
        }
      }
      pTuya->cmd_checksum += serial_in_byte;
      pTuya->cmd_checksum &= 0xff;
      pTuya->buffer[pTuya->byte_counter++] = serial_in_byte;
    } break;
    case 3: {// wait Data
      pTuya->buffer[pTuya->byte_counter++] = serial_in_byte;
      pTuya->cmd_checksum += serial_in_byte;
      pTuya->cmd_checksum &= 0xff;
      if (pTuya->byte_counter == (6 + pTuya->data_len)) {
        pTuya->cmd_status = 4;
      }

      if (pTuya->byte_counter > TUYA_BUFFER_SIZE-3){
        AddLog(LOG_LEVEL_ERROR, PSTR("TYA: input overflow"));
        TuyaDump(pTuya->buffer, pTuya->byte_counter);
        pTuya->cmd_status = 0;
        pTuya->byte_counter = 0;
    }
    } break;
    case 4: {// wait CS
      pTuya->buffer[pTuya->byte_counter++] = serial_in_byte;
      pTuya->cmd_checksum &= 0xff;
      if (pTuya->cmd_checksum == serial_in_byte) { // Compare checksum and process packet
        TuyaProcessCommand(pTuya->buffer);
        pTuya->cmd_status = 0;
        pTuya->byte_counter = 0;
      } else {
        AddLog(LOG_LEVEL_ERROR, PSTR("TYA: Bad CS 0x%02X != 0x%02X"), serial_in_byte, pTuya->cmd_checksum);
        TuyaDump(pTuya->buffer, pTuya->byte_counter);
        pTuya->cmd_status = 0;
        pTuya->byte_counter = 0;
        pTuya->errorcnt ++;
      }
    } break;
  }
  pTuya->lastByte = serial_in_byte;
}


void TuyaSerialInput(void)
{
  while (TuyaSerial->available()) {
    yield();
    uint8_t serial_in_byte = TuyaSerial->read();
    TuyaProcessByte(serial_in_byte);
  }
}

bool TuyaButtonPressed(void) {
  bool result = false;
  uint32_t button = XdrvMailbox.payload;
  if (!XdrvMailbox.index && ((PRESSED == button) && (NOT_PRESSED == pTuya->last_button))) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Reset GPIO triggered"));
    TuyaResetWifi();
    result = true;  // Reset GPIO served here
  }
  pTuya->last_button = button;
  return result;   // Don't serve other buttons
}

uint8_t TuyaGetTuyaWifiState(void) {

  uint8_t wifi_state = 0x02;
  switch(WifiState()){
    case WIFI_MANAGER:
      wifi_state = 0x01;
      break;
    case WIFI_RESTART:
      wifi_state =  0x03;
      break;
  }

  if (MqttIsConnected()) {
    wifi_state = 0x04;
  }

  return wifi_state;
}

#ifdef USE_TUYA_TIME
void TuyaSetTime(void) {
  if (!RtcTime.valid) { return; }

  uint16_t payload_len = 8;
  uint8_t payload_buffer[8];
  uint8_t tuya_day_of_week;
  if (RtcTime.day_of_week == 1) {
    tuya_day_of_week = 7;
  } else {
    tuya_day_of_week = RtcTime.day_of_week-1;
  }

  payload_buffer[0] = 0x01;
  payload_buffer[1] = RtcTime.year %100;
  payload_buffer[2] = RtcTime.month;
  payload_buffer[3] = RtcTime.day_of_month;
  payload_buffer[4] = RtcTime.hour;
  payload_buffer[5] = RtcTime.minute;
  payload_buffer[6] = RtcTime.second;
  payload_buffer[7] = tuya_day_of_week; //1 for Monday in TUYA Doc

  TuyaSendCmd(TUYA_CMD_SET_TIME, payload_buffer, payload_len);
}
#endif //USE_TUYA_TIME

/*********************************************************************************************\
 * Sensors
\*********************************************************************************************/

void TuyaSensorsShow(bool json)
{
  bool RootName = false;
  bool added = false;
  char sname[20];
  char tempval[5];
  uint8_t res;

  for (uint8_t sensor = TUYA_MCU_FUNC_TEMP; sensor <= TUYA_MCU_FUNC_TIMER4; sensor++) { // Sensors start from fnId 71
    if (json) {
      if (TuyaGetDpId(sensor) != 0) {

        if (!RootName) {
          ResponseAppend_P(PSTR(",\"TuyaSNS\":{"));
          RootName = true;
        }
        if (added) {
          ResponseAppend_P(PSTR(","));
        }
        if (sensor > 74) {
          res = 0;
        } else if (sensor > 72) {
          res = Settings->flag2.humidity_resolution;
        } else if (sensor == 72) {
          res = Settings->mbflag2.temperature_set_res;
        } else {
          res = Settings->flag2.temperature_resolution;
        }

        GetTextIndexed(sname, sizeof(sname), (sensor-71), kTuyaSensors);
        ResponseAppend_P(PSTR("\"%s\":%s"), sname,
                        (pTuya->SensorsValid[sensor-71] ? dtostrfd(TuyaAdjustedTemperature(pTuya->Sensors[sensor-71], res), res, tempval) : PSTR("null")));
        added = true;
      }
  #ifdef USE_WEBSERVER
    } else {
      if (TuyaGetDpId(sensor) != 0) {
        switch (sensor) {
          case 71:
            WSContentSend_Temp("", TuyaAdjustedTemperature(pTuya->Sensors[0], Settings->flag2.temperature_resolution));
            break;
          case 72:
            WSContentSend_PD(PSTR("{s}" D_TEMPERATURE " Set{m}%s " D_UNIT_DEGREE "%c{e}"),
                            dtostrfd(TuyaAdjustedTemperature(pTuya->Sensors[1], Settings->mbflag2.temperature_set_res), Settings->mbflag2.temperature_set_res, tempval), TempUnit());
            break;
          case 73:
            WSContentSend_PD(HTTP_SNS_HUM, "", dtostrfd(TuyaAdjustedTemperature(pTuya->Sensors[2], Settings->flag2.humidity_resolution), Settings->flag2.humidity_resolution, tempval));
            break;
          case 74:
            WSContentSend_PD(PSTR("{s}" D_HUMIDITY " Set{m}%s " D_UNIT_PERCENT "{e}"),
                            dtostrfd(TuyaAdjustedTemperature(pTuya->Sensors[3], Settings->flag2.humidity_resolution), Settings->flag2.humidity_resolution, tempval));
            break;
          case 75:
            WSContentSend_PD(HTTP_SNS_ILLUMINANCE, "", pTuya->Sensors[4]);
            break;
          case 76:
            WSContentSend_PD(PSTR("{s}" D_TVOC "{m}%d " D_UNIT_PARTS_PER_MILLION "{e}"), pTuya->Sensors[5]);
            break;
          case 77:
            WSContentSend_PD(HTTP_SNS_CO2, "", pTuya->Sensors[6]);
            break;
          case 78:
            WSContentSend_PD(HTTP_SNS_CO2EAVG, "", pTuya->Sensors[7]);
            break;
          case 79:
            WSContentSend_PD(HTTP_SNS_GAS, "", pTuya->Sensors[8]);
            break;
          case 80:
            WSContentSend_PD(PSTR("{s}" D_ENVIRONMENTAL_CONCENTRATION " 2.5 " D_UNIT_MICROMETER "{m}%d " D_UNIT_MICROGRAM_PER_CUBIC_METER "{e}"), pTuya->Sensors[9]);
            break;
          case 81:
          case 82:
          case 83:
          case 84:
            WSContentSend_PD(PSTR("{s}Timer%d{m}%d{e}"), (sensor-80), pTuya->Sensors[sensor-71]); // No UoM for timers since they can be sec or min
            break;
        }
      }

  #endif  // USE_WEBSERVER
    }
  }
#ifdef USE_WEBSERVER
  if (AsModuleTuyaMS()) {
    WSContentSend_P(PSTR("{s}" D_JSON_IRHVAC_MODE "{m}%d{e}"), pTuya->ModeSet);
  }
#endif  // USE_WEBSERVER

  if (RootName) { ResponseJsonEnd();}
}

#ifdef USE_WEBSERVER

void TuyaAddButton(void) {
  if (AsModuleTuyaMS()) {
    WSContentSend_P(HTTP_TABLE100);
    WSContentSend_P(PSTR("<tr><div></div>"));
    char stemp[33];
    snprintf_P(stemp, sizeof(stemp), PSTR("" D_JSON_IRHVAC_MODE ""));
    WSContentSend_P(HTTP_DEVICE_CONTROL, 26, TasmotaGlobal.devices_present + 1,
      (strlen(GetWebButton(TasmotaGlobal.devices_present))) ? HtmlEscape(GetWebButton(TasmotaGlobal.devices_present)).c_str() : stemp, "");
    WSContentSend_P(PSTR("</tr></table>"));
  }
}

#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#ifdef USE_ENERGY_SENSOR

bool Xnrg32(uint32_t function)
{
  bool result = false;

  if (pTuya && pTuya->active) {
    if (FUNC_PRE_INIT == function) {
      if (TuyaGetDpId(TUYA_MCU_FUNC_POWER) != 0 || TuyaGetDpId(TUYA_MCU_FUNC_POWER_COMBINED) != 0) {
        if (TuyaGetDpId(TUYA_MCU_FUNC_CURRENT) == 0 && TuyaGetDpId(TUYA_MCU_FUNC_POWER_COMBINED) == 0) {
          Energy->current_available = false;
        }
        if (TuyaGetDpId(TUYA_MCU_FUNC_VOLTAGE) == 0 && TuyaGetDpId(TUYA_MCU_FUNC_POWER_COMBINED) == 0) {
          Energy->voltage_available = false;
        }
        TasmotaGlobal.energy_driver = XNRG_32;
      }
    }
  }
  return result;
}
#endif  // USE_ENERGY_SENSOR

bool Xdrv16(uint32_t function) {
  bool result = false;

  if (FUNC_MODULE_INIT == function) {
    result = TuyaModuleSelected();
    if (pTuya) pTuya->active = result;
  }
  else if (pTuya && pTuya->active) {
    switch (function) {
      case FUNC_LOOP:
      case FUNC_SLEEP_LOOP:
        if (TuyaSerial) { TuyaSerialInput(); }
        break;
      case FUNC_PRE_INIT:
        TuyaInit();
        break;
      case FUNC_SET_DEVICE_POWER:
        result = TuyaSetPower();
#ifdef TUYA_MORE_DEBUG
        MqttPublishLoggingAsync(false);
        SyslogAsync(false);
#endif
        break;
      case FUNC_BUTTON_PRESSED:
        result = TuyaButtonPressed();
#ifdef TUYA_MORE_DEBUG
        MqttPublishLoggingAsync(false);
        SyslogAsync(false);
#endif
        break;
      case FUNC_EVERY_100_MSECOND:
        if (pTuya->timeout){
          pTuya->timeout -= 100;
          if (pTuya->timeout <= 0){
            Tuya_timeout();
          }
        }
        for (int i = 0; i < 2; i++){
          if (pTuya->dimDelay_ms[i]){
            pTuya->dimDelay_ms[i] -= 100;
            if (pTuya->dimDelay_ms[i] <= 0){
              pTuya->dimDelay_ms[i] = 0;
              AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: DimDelay%d->0"), i);
            }
          }
        }
        Tuya_statemachine();
        break;
      case FUNC_EVERY_SECOND:
        if (pTuya->wifiTimer > 0){
          pTuya->wifiTimer -= 1000;
          if (pTuya->wifiTimer <= 0){
            AddLog(LOG_LEVEL_ERROR, PSTR("TYA: Wifi reset aborted."));
            pTuya->wifiTimer = 0;
          }
        }
        // now done in state machine
        //if (TuyaSerial && pTuya->wifi_state != TuyaGetTuyaWifiState()) { TuyaSetWifiLed(); }
        if (!pTuya->low_power_mode) {
          pTuya->heartbeat_timer++;
          if (pTuya->heartbeat_timer >= HEARTBEAT_INTERVAL_S) {
            pTuya->heartbeat_timer = 0;
            pTuya->send_heartbeat = 1;
          }
#ifdef USE_TUYA_TIME
          if (!(TasmotaGlobal.uptime % 60)) {
            // if we have never been asked for time, send it ourselves
            // as this was the original TAS implementation.
            // if we DO get asked for time, then send_time & 2 will be set, so we no longer send from here.
            if (!pTuya->send_time & 2){
              pTuya->send_time |= 1;
            }
          }
#endif  //USE_TUYA_TIME
        } else {
          // done in state machine
          //TuyaSendLowPowerSuccessIfNeeded();
        }
        if (pTuya->ignore_topic_timeout < millis()) { pTuya->SuspendTopic = false; }
        if (pTuya->lasterrorcnt != pTuya->errorcnt){
          AddLog(LOG_LEVEL_ERROR, PSTR("TYA: Errorcnt %d->%d"), pTuya->lasterrorcnt, pTuya->errorcnt);
          pTuya->lasterrorcnt = pTuya->errorcnt;
        }
        break;
      case FUNC_SET_CHANNELS:
        result = TuyaSetChannels();
#ifdef TUYA_MORE_DEBUG
        MqttPublishLoggingAsync(false);
        SyslogAsync(false);
#endif
        break;
      case FUNC_MQTT_INIT:
        // why here?  done as part of startup state machine
        //TuyaSendCmd(TUYA_CMD_QUERY_PRODUCT);
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kTuyaCommand, TuyaCommand);
        break;
      case FUNC_JSON_APPEND:
        TuyaSensorsShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_ADD_MAIN_BUTTON:
        TuyaAddButton();
        break;
      case FUNC_WEB_SENSOR:
        TuyaSensorsShow(0);
        break;
#endif  // USE_WEBSERVER
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif  // USE_TUYA_MCU
#endif  // USE_LIGHT