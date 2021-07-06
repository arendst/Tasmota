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
#ifdef USE_TUYA_MCU

#define XDRV_16                16
#define XNRG_32                32   // Needs to be the last XNRG_xx

#ifndef TUYA_DIMMER_ID
#define TUYA_DIMMER_ID         0
#endif

#define TUYA_CMD_HEARTBEAT     0x00
#define TUYA_CMD_QUERY_PRODUCT 0x01
#define TUYA_CMD_MCU_CONF      0x02
#define TUYA_CMD_WIFI_STATE    0x03
#define TUYA_CMD_WIFI_RESET    0x04
#define TUYA_CMD_WIFI_SELECT   0x05
#define TUYA_CMD_SET_DP        0x06
#define TUYA_CMD_STATE         0x07
#define TUYA_CMD_QUERY_STATE   0x08
#define TUYA_CMD_SET_TIME      0x1C

#define TUYA_LOW_POWER_CMD_WIFI_STATE   0x02
#define TUYA_LOW_POWER_CMD_WIFI_RESET   0x03
#define TUYA_LOW_POWER_CMD_WIFI_CONFIG  0x04
#define TUYA_LOW_POWER_CMD_STATE        0x05

#define TUYA_TYPE_BOOL         0x01
#define TUYA_TYPE_VALUE        0x02
#define TUYA_TYPE_STRING       0x03
#define TUYA_TYPE_ENUM         0x04

#define TUYA_BUFFER_SIZE       256

#include <TasmotaSerial.h>

TasmotaSerial *TuyaSerial = nullptr;

struct TUYA {
  uint16_t Levels[5] = {0,0,0,0,0};       // Array to store the values of TuyaMCU channels
  uint16_t Snapshot[5] = {0,0,0,0,0};     // Array to store a snapshot of Tasmota actual channel values
  uint16_t EnumState[4] = {0,0,0,0};      // Array to store up to four Enum type 4 values
  char RGBColor[7] = "000000";            // Stores RGB Color string in Hex format
  uint16_t CTMin = 153;                   // Minimum CT level allowed - When SetOption82 is enabled will default to 200
  uint16_t CTMax = 500;                   // Maximum CT level allowed - When SetOption82 is enabled will default to 380
  bool ModeSet = false;                   // Controls 0 - Single Tone light, 1 - RGB Light
  int16_t Sensors[14];                    // Stores the values of Sensors connected to the Tuya Device
  bool SensorsValid[14];                  // Bool used for nullify the sensor value until a real value is received from the MCU
  bool SuspendTopic = false;              // Used to reduce the load at init time or when polling the configuraton on demand
  uint32_t ignore_topic_timeout = 0;      // Suppress the /STAT topic (if enabled) to avoid data overflow until the configuration is over
  bool ignore_dim = false;                // Flag to skip serial send to prevent looping when processing inbound states from the faceplate interaction
  uint8_t cmd_status = 0;                 // Current status of serial-read
  uint8_t cmd_checksum = 0;               // Checksum of tuya command
  uint8_t data_len = 0;                   // Data lenght of command
  uint8_t wifi_state = -2;                // Keep MCU wifi-status in sync with WifiState()
  uint8_t heartbeat_timer = 0;            // 10 second heartbeat timer for tuya module
#ifdef USE_ENERGY_SENSOR
  uint32_t lastPowerCheckTime = 0;        // Time when last power was checked
#endif // USE_ENERGY_SENSOR
  char *buffer = nullptr;                 // Serial receive buffer
  int byte_counter = 0;                   // Index in serial receive buffer
  bool low_power_mode = false;            // Normal or Low power mode protocol
  bool send_success_next_second = false;  // Second command success in low power mode
  uint32_t ignore_dimmer_cmd_timeout = 0; // Time until which received dimmer commands should be ignored
  bool ignore_tuyareceived = false;       // When a modeset changes ignore stat
} Tuya;

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
  "" D_JSON_TEMPERATURE "|TempSet|" D_JSON_HUMIDITY "|HumSet|" D_JSON_ILLUMINANCE
  "|" D_JSON_TVOC "|" D_JSON_ECO2 "|" D_JSON_CO2 "|" D_JSON_GAS "||Timer1|Timer2|Timer3|TImer4";

const char kTuyaCommand[] PROGMEM = D_PRFX_TUYA "|"  // Prefix
  D_CMND_TUYA_MCU "|" D_CMND_TUYA_MCU_SEND_STATE "|" D_CMND_TUYARGB "|" D_CMND_TUYA_ENUM "|" D_CMND_TUYA_ENUM_LIST "|TempSetRes";

void (* const TuyaCommand[])(void) PROGMEM = {
  &CmndTuyaMcu, &CmndTuyaSend, &CmndTuyaRgb, &CmndTuyaEnum, &CmndTuyaEnumList, &CmndTuyaTempSetRes
};

/*********************************************************************************************\
 * Web Interface
\*********************************************************************************************/
bool IsModuleTuya(void)
{
  return ((TUYA_DIMMER == TasmotaGlobal.module_type) || (SK03_TUYA == TasmotaGlobal.module_type));
}

bool AsModuleTuyaMS(void) // ModeSet Layout
{
  return ((TasmotaGlobal.light_type > LT_RGB) && TuyaGetDpId(TUYA_MCU_FUNC_MODESET) != 0);
}

bool TuyaModeSet(void) // ModeSet Status
{
  return Tuya.ModeSet;
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
*/

void CmndTuyaSend(void) {
  if (XdrvMailbox.index > 5 && XdrvMailbox.index < 8) {
    return;
  }
  if (XdrvMailbox.index == 0) {
    TuyaRequestState(0);
  } else if (XdrvMailbox.index == 8) {
    TuyaRequestState(8);
  } else if (XdrvMailbox.index == 9) { // TuyaSend Topic Toggle
    Settings->tuyamcu_topic = !Settings->tuyamcu_topic;
    AddLog(LOG_LEVEL_INFO, PSTR("TYA: TuyaMCU Stat Topic %s"), (Settings->tuyamcu_topic ? PSTR("enabled") : PSTR("disabled")));

  } else {
    if (XdrvMailbox.data_len > 0) {
      char *p;
      char *data;
      uint8_t i = 0;
      uint8_t dpId = 0;
      for (char *str = strtok_r(XdrvMailbox.data, ", ", &p); str && i < 2; str = strtok_r(nullptr, ", ", &p)) {
        if ( i == 0) {
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
      }
    }
  }
  ResponseCmndDone();
}

// TuyaMcu fnid,dpid

void CmndTuyaMcu(void) {
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
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
    Settings->mbflag2.temperature_set_res = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->mbflag2.temperature_set_res);
}

void CmndTuyaEnum(void) { // Command to control up to four type 4 Enum
  uint16_t EnumIdx = XdrvMailbox.index;
  int32_t payload = XdrvMailbox.payload;

  if (EnumIdx > 4 || TuyaGetDpId(TUYA_MCU_FUNC_ENUM1 + (EnumIdx-1)) == 0) {
    return;
  }

  if (XdrvMailbox.data_len > 0) {
    if (payload < 0 || payload > Settings->tuya_fnid_map[EnumIdx + 230].dpid ) {
      return;
    } else {
      if (payload != Tuya.EnumState[EnumIdx-1]) {
        Tuya.EnumState[EnumIdx-1] = payload;
        TuyaSendEnum(TuyaGetDpId(TUYA_MCU_FUNC_ENUM1 + (EnumIdx-1)), payload);
      }
      ResponseCmndIdxNumber(Tuya.EnumState[EnumIdx-1]);
    }
  } else {
    Response_P(PSTR("{\"%s\":{"), XdrvMailbox.command);  // Builds TuyaEnum
    bool added = false;
    for (uint8_t i = 0; i <= 3; i++) {
      if (TuyaGetDpId(TUYA_MCU_FUNC_ENUM1 + i) != 0) {
        if (added) {
          ResponseAppend_P(PSTR(","));
        }
        ResponseAppend_P(PSTR("\"Enum%d\":%d"), i + 1, Tuya.EnumState[i]); // Returns the actual values of Enum as list
        added = true;
      }
    }
    ResponseAppend_P(PSTR("}}"));
  }
}

void CmndTuyaEnumList(void) { // Command to declare the number of items in list for up to four type 4 enum. Min = 0, Max = 31, Default = 0

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
          (fnId >= TUYA_MCU_FUNC_LX && fnId <= TUYA_MCU_FUNC_GAS) ||
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

void TuyaSendCmd(uint8_t cmd, uint8_t payload[] = nullptr, uint16_t payload_len = 0)
{
  uint8_t checksum = (0xFF + cmd + (payload_len >> 8) + (payload_len & 0xFF));
  TuyaSerial->write(0x55);                  // Tuya header 55AA
  TuyaSerial->write(0xAA);
  TuyaSerial->write((uint8_t)0x00);         // version 00
  TuyaSerial->write(cmd);                   // Tuya command
  TuyaSerial->write(payload_len >> 8);      // following data length (Hi)
  TuyaSerial->write(payload_len & 0xFF);    // following data length (Lo)
  char log_data[700];                       // Was MAX_LOGSZ
  snprintf_P(log_data, sizeof(log_data), PSTR("TYA: Send \"55aa00%02x%02x%02x"), cmd, payload_len >> 8, payload_len & 0xFF);
  for (uint32_t i = 0; i < payload_len; ++i) {
    TuyaSerial->write(payload[i]);
    checksum += payload[i];
    snprintf_P(log_data, sizeof(log_data), PSTR("%s%02x"), log_data, payload[i]);
  }
  TuyaSerial->write(checksum);
  TuyaSerial->flush();
  snprintf_P(log_data, sizeof(log_data), PSTR("%s%02x\""), log_data, checksum);
  AddLogData(LOG_LEVEL_DEBUG, log_data);
}

void TuyaSendState(uint8_t id, uint8_t type, uint8_t* value)
{
  uint16_t payload_len = 4;
  uint8_t payload_buffer[8];
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
      payload_buffer[4] = value[3];
      payload_buffer[5] = value[2];
      payload_buffer[6] = value[1];
      payload_buffer[7] = value[0];
      break;

  }

  TuyaSendCmd(TUYA_CMD_SET_DP, payload_buffer, payload_len);
}

void TuyaSendBool(uint8_t id, bool value)
{
  TuyaSendState(id, TUYA_TYPE_BOOL, (uint8_t*)&value);
}

void TuyaSendValue(uint8_t id, uint32_t value)
{
  TuyaSendState(id, TUYA_TYPE_VALUE, (uint8_t*)(&value));
}

void TuyaSendEnum(uint8_t id, uint32_t value)
{
  TuyaSendState(id, TUYA_TYPE_ENUM, (uint8_t*)(&value));
}

void TuyaSendHexString(uint8_t id, char data[]) {

  uint16_t len = strlen(data)/2;
  uint16_t payload_len = 4 + len;
  uint8_t payload_buffer[payload_len];
  payload_buffer[0] = id;
  payload_buffer[1] = TUYA_TYPE_STRING;
  payload_buffer[2] = len >> 8;
  payload_buffer[3] = len & 0xFF;

  char hexbyte[3];
  hexbyte[2] = 0;

  for (uint16_t i = 0; i < len; i++) {
    hexbyte[0] = data[2*i];
    hexbyte[1] = data[2*i+1];
    payload_buffer[4+i] = strtol(hexbyte,NULL,16);
  }

  TuyaSendCmd(TUYA_CMD_SET_DP, payload_buffer, payload_len);
}

void TuyaSendString(uint8_t id, char data[]) {

  uint16_t len = strlen(data);
  uint16_t payload_len = 4 + len;
  uint8_t payload_buffer[payload_len];
  payload_buffer[0] = id;
  payload_buffer[1] = TUYA_TYPE_STRING;
  payload_buffer[2] = len >> 8;
  payload_buffer[3] = len & 0xFF;

  for (uint16_t i = 0; i < len; i++) {
    payload_buffer[4+i] = data[i];
  }

  TuyaSendCmd(TUYA_CMD_SET_DP, payload_buffer, payload_len);
}

bool TuyaSetPower(void)
{
  bool status = false;

  uint8_t rpower = XdrvMailbox.index;
  int16_t source = XdrvMailbox.payload;

  uint8_t dpid = TuyaGetDpId(TUYA_MCU_FUNC_REL1 + TasmotaGlobal.active_device - 1);
  if (dpid == 0) dpid = TuyaGetDpId(TUYA_MCU_FUNC_REL1_INV + TasmotaGlobal.active_device - 1);

  if (source != SRC_SWITCH && TuyaSerial) {  // ignore to prevent loop from pushing state from faceplate interaction
    TuyaSendBool(dpid, bitRead(rpower, TasmotaGlobal.active_device-1) ^ bitRead(TasmotaGlobal.rel_inverted, TasmotaGlobal.active_device-1));
    delay(20); // Hack when power is off and dimmer is set then both commands go too soon to Serial out.
    status = true;
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
  bool LightMode = TuyaGetDpId(TUYA_MCU_FUNC_MODESET) != 0;
  uint8_t idx = 0;
  snprintf_P(hex_char, sizeof(hex_char), PSTR("000000000000"));

  if (LT_SERIAL1 == TasmotaGlobal.light_type) {
    Tuya.Snapshot[0] = light_state.getDimmer();
  }
  if (LT_SERIAL2 == TasmotaGlobal.light_type || LT_RGBWC == TasmotaGlobal.light_type) {
    idx = 1;
    if (LT_SERIAL2 == TasmotaGlobal.light_type && Settings->flag3.pwm_multi_channels && (TuyaGetDpId(TUYA_MCU_FUNC_DIMMER2) != 0)) {
      // Special setup for dual dimmer (like the MOES 2 Way Dimmer) emulating 2 PWM channels
      Tuya.Snapshot[0] = changeUIntScale(Light.current_color[0], 0, 255, 0, 100);
      Tuya.Snapshot[1] = changeUIntScale(Light.current_color[1], 0, 255, 0, 100);
    } else { // CT Light or RGBWC
      getCTRange(&Tuya.CTMin, &Tuya.CTMax); // SetOption82 - Reduce the CT range from 153..500 to 200..380 to accomodate with Alexa range
      Tuya.Snapshot[0] = light_state.getDimmer();
      Tuya.Snapshot[1] = light_state.getCT();
    }
  }
  if (LT_RGBW == TasmotaGlobal.light_type) {
    idx = 1;
    Tuya.Snapshot[0] = light_state.getDimmer(1);
    Tuya.Snapshot[1] = light_state.getDimmer(2);
  }

  if (TasmotaGlobal.light_type > LT_BASIC) {

    if (LT_RGB != TasmotaGlobal.light_type) {
      for (uint8_t i = 0; i <= idx; i++) {
        if (Tuya.Snapshot[i] != Tuya.Levels[i]) {
          if (i == 0 && LightMode && Tuya.ModeSet ) { noupd = true;}
          if (!noupd) {
            LightSerialDuty(Tuya.Snapshot[i], &hex_char[0], i+1);
            //Tuya.Levels[i] = Tuya.Snapshot[i];
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

      if ((RGBType > 1 && (StrCmpNoCase(scolor, Tuya.RGBColor) != 0)) || (RGBType <= 1 && ((hue != Tuya.Snapshot[2]) ||
          (sat != Tuya.Snapshot[3]) || (bri != Tuya.Snapshot[4])))) {

        if ((LightMode && Tuya.ModeSet) || LT_RGB == TasmotaGlobal.light_type) {
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
            memcpy_P(Tuya.RGBColor, scolor, strlen(scolor));
            Tuya.Snapshot[2] = hue;
            Tuya.Snapshot[3] = sat;
            Tuya.Snapshot[4] = bri;
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
      } else { dpid = TuyaGetDpId(TUYA_MCU_FUNC_DIMMER2); }
    }

    if (Tuya.ignore_dim && Tuya.ignore_dimmer_cmd_timeout < millis()) {
      Tuya.ignore_dim = false;
    }

    if (duty > 0 && !Tuya.ignore_dim && TuyaSerial && dpid > 0) {
      if (TuyaIdx == 2 && CTLight) {
        duty = changeUIntScale(duty, Tuya.CTMin, Tuya.CTMax, Settings->dimmer_hw_max, 0);
      } else { duty = changeUIntScale(duty, 0, 100, 0, Settings->dimmer_hw_max); }

      if (duty < Settings->dimmer_hw_min) { duty = Settings->dimmer_hw_min; }  // dimming acts odd below 25(10%) - this mirrors the threshold set on the faceplate itself
        Tuya.ignore_dimmer_cmd_timeout = millis() + 250; // Ignore serial received dim commands for the next 250ms
        if (Tuya.ModeSet && (TuyaGetDpId(TUYA_MCU_FUNC_MODESET) != 0) && TasmotaGlobal.light_type > LT_RGB) {
          TuyaSendEnum(TuyaGetDpId(TUYA_MCU_FUNC_MODESET), 0);
        }
        TuyaSendValue(dpid, duty);

    } else if (dpid > 0 && TuyaIdx <= 2) {

      Tuya.ignore_dim = false;  // reset flag

      if (TuyaIdx == 2 && CTLight) {
        duty = changeUIntScale(duty, Tuya.CTMin, Tuya.CTMax, Settings->dimmer_hw_max, 0);
      } else {
        duty = changeUIntScale(duty, 0, 100, 0, Settings->dimmer_hw_max);
      }
      AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Send dim skipped value %d for dpid %d"), duty, dpid);  // due to 0 or already set
    } else {
      AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Cannot set dimmer. Dimmer Id unknown"));
    }
  }

  if (TuyaIdx == 3) {
    dpid = TuyaGetDpId(TUYA_MCU_FUNC_RGB);
    if (!Tuya.ModeSet && (TuyaGetDpId(TUYA_MCU_FUNC_MODESET) != 0) && TasmotaGlobal.light_type > LT_RGB) {
        TuyaSendEnum(TuyaGetDpId(TUYA_MCU_FUNC_MODESET), 1);
    }
    TuyaSendString(dpid, hex_char);
    AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: TX RGB hex %s to dpId %d"), hex_char, dpid);
  }
}

void TuyaRequestState(uint8_t state_type)
{
  if (TuyaSerial) {
    // Get current status of MCU
    AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Read MCU state"));
    Tuya.SuspendTopic = true;
    Tuya.ignore_topic_timeout = millis() + 1000; // suppress /STAT topic for 1000ms to limit data
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
    char scmnd[20];
    snprintf_P(scmnd, sizeof(scmnd), D_CMND_WIFICONFIG " %d", 2);
    ExecuteCommand(scmnd, SRC_BUTTON);
  }
}

void TuyaProcessStatePacket(void) {
  char scmnd[20];
  uint8_t dpidStart = 6;
  uint8_t fnId;
  uint16_t dpDataLen;
  bool PowerOff = false;
  bool tuya_energy_enabled = (XNRG_32 == TasmotaGlobal.energy_driver);

  while (dpidStart + 4 < Tuya.byte_counter) {
    dpDataLen = Tuya.buffer[dpidStart + 2] << 8 | Tuya.buffer[dpidStart + 3];
    fnId = TuyaGetFuncId(Tuya.buffer[dpidStart]);

    AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: fnId=%d is set for dpId=%d"), fnId, Tuya.buffer[dpidStart]);
    if (Tuya.buffer[dpidStart + 1] == 0) {
#ifdef USE_ENERGY_SENSOR
        if (tuya_energy_enabled && fnId == TUYA_MCU_FUNC_POWER_COMBINED) {
          if (dpDataLen == 8) {
            uint16_t tmpVol = Tuya.buffer[dpidStart + 4] << 8 | Tuya.buffer[dpidStart + 5];
            uint16_t tmpCur = Tuya.buffer[dpidStart + 7] << 8 | Tuya.buffer[dpidStart + 8];
            uint16_t tmpPow = Tuya.buffer[dpidStart + 10] << 8 | Tuya.buffer[dpidStart + 11];
          Energy.voltage[0] = (float)tmpVol / 10;
          Energy.current[0] = (float)tmpCur / 1000;
          Energy.active_power[0] = (float)tmpPow;
          AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Rx ID=%d Voltage=%d"), Tuya.buffer[dpidStart], tmpVol);
          AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Rx ID=%d Current=%d"), Tuya.buffer[dpidStart], tmpCur);
          AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Rx ID=%d Active_Power=%d"), Tuya.buffer[dpidStart], tmpPow);

          if (RtcTime.valid) {
            if (Tuya.lastPowerCheckTime != 0 && Energy.active_power[0] > 0) {
              Energy.kWhtoday += Energy.active_power[0] * (float)(Rtc.utc_time - Tuya.lastPowerCheckTime) / 36.0;
              EnergyUpdateToday();
            }
            Tuya.lastPowerCheckTime = Rtc.utc_time;
          }
        } else {
          AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Rx ID=%d INV_LEN=%d"), Tuya.buffer[dpidStart], dpDataLen);
        }
        }
        #endif // USE_ENERGY_SENSOR
    }
    else if (Tuya.buffer[dpidStart + 1] == 1) {  // Data Type 1

        if (fnId >= TUYA_MCU_FUNC_REL1 && fnId <= TUYA_MCU_FUNC_REL8) {
          AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: RX Relay-%d --> MCU State: %s Current State:%s"), fnId - TUYA_MCU_FUNC_REL1 + 1, Tuya.buffer[dpidStart + 4]?"On":"Off",bitRead(TasmotaGlobal.power, fnId - TUYA_MCU_FUNC_REL1)?"On":"Off");
          if ((TasmotaGlobal.power || Settings->light_dimmer > 0) && (Tuya.buffer[dpidStart + 4] != bitRead(TasmotaGlobal.power, fnId - TUYA_MCU_FUNC_REL1))) {
            if (!Tuya.buffer[dpidStart + 4]) { PowerOff = true; }
            ExecuteCommandPower(fnId - TUYA_MCU_FUNC_REL1 + 1, Tuya.buffer[dpidStart + 4], SRC_SWITCH);  // send SRC_SWITCH? to use as flag to prevent loop from inbound states from faceplate interaction
          }
        } else if (fnId >= TUYA_MCU_FUNC_REL1_INV && fnId <= TUYA_MCU_FUNC_REL8_INV) {
          AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: RX Relay-%d-Inverted --> MCU State: %s Current State:%s"), fnId - TUYA_MCU_FUNC_REL1_INV + 1, Tuya.buffer[dpidStart + 4]?"Off":"On",bitRead(TasmotaGlobal.power, fnId - TUYA_MCU_FUNC_REL1_INV) ^ 1?"Off":"On");
          if (Tuya.buffer[dpidStart + 4] != bitRead(TasmotaGlobal.power, fnId - TUYA_MCU_FUNC_REL1_INV) ^ 1) {
            ExecuteCommandPower(fnId - TUYA_MCU_FUNC_REL1_INV + 1, Tuya.buffer[dpidStart + 4] ^ 1, SRC_SWITCH);  // send SRC_SWITCH? to use as flag to prevent loop from inbound states from faceplate interaction
            if (Tuya.buffer[dpidStart + 4]) { PowerOff = true; }
          }
        } else if (fnId >= TUYA_MCU_FUNC_SWT1 && fnId <= TUYA_MCU_FUNC_SWT4) {
          AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: RX Switch-%d --> MCU State: %d Current State:%d"),fnId - TUYA_MCU_FUNC_SWT1 + 1,Tuya.buffer[dpidStart + 4], SwitchGetVirtual(fnId - TUYA_MCU_FUNC_SWT1));

          if (SwitchGetVirtual(fnId - TUYA_MCU_FUNC_SWT1) != Tuya.buffer[dpidStart + 4]) {
            SwitchSetVirtual(fnId - TUYA_MCU_FUNC_SWT1, Tuya.buffer[dpidStart + 4]);
            SwitchHandler(1);
          }
        }
        if (PowerOff) { Tuya.ignore_dimmer_cmd_timeout = millis() + 250; }
      }
      else if (Tuya.buffer[dpidStart + 1] == 2) {  // Data Type 2
        uint32_t packetValue = Tuya.buffer[dpidStart + 4] << 24 | Tuya.buffer[dpidStart + 5] << 16 | Tuya.buffer[dpidStart + 6] << 8 | Tuya.buffer[dpidStart + 7]; // TYpe 2 is a 32 bit integer
        uint8_t dimIndex;
        bool SnsUpdate = false;

        if ((fnId >= TUYA_MCU_FUNC_TEMP) && (fnId <= TUYA_MCU_FUNC_TIMER4)) {      // Sensors start from fnId 71
          if (packetValue != Tuya.Sensors[fnId-71]) {
            Tuya.SensorsValid[fnId-71] = true;
            Tuya.Sensors[fnId-71] = packetValue;
            SnsUpdate = true;
          }
        }

        if (SnsUpdate) {
          char sname[20];
          char tempval[5];
          uint8_t res;

          if (TasmotaGlobal.uptime < 8) { // delay to avoid multiple topics at the same time at boot time
            return;
          } else {
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
            MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR(D_CMND_SENSOR));
          }
        }

        if (fnId == TUYA_MCU_FUNC_DIMMER || fnId == TUYA_MCU_FUNC_REPORT1) { dimIndex = 0; }

        if (fnId == TUYA_MCU_FUNC_DIMMER2 || fnId == TUYA_MCU_FUNC_REPORT2 || fnId == TUYA_MCU_FUNC_CT) { dimIndex = 1; }

        if (dimIndex == 1 && !Settings->flag3.pwm_multi_channels) {
          Tuya.Levels[1] = changeUIntScale(packetValue, 0, Settings->dimmer_hw_max, Tuya.CTMax, Tuya.CTMin);
        } else {
          Tuya.Levels[dimIndex] = changeUIntScale(packetValue, 0, Settings->dimmer_hw_max, 0, 100);
        }

        AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: RX value %d from dpId %d "), packetValue, Tuya.buffer[dpidStart]);

        if ((fnId == TUYA_MCU_FUNC_DIMMER) || (fnId == TUYA_MCU_FUNC_REPORT1) ||
            (fnId == TUYA_MCU_FUNC_DIMMER2) || (fnId == TUYA_MCU_FUNC_REPORT2) ||
            (fnId == TUYA_MCU_FUNC_CT) || (fnId == TUYA_MCU_FUNC_WHITE)) {

          if (Tuya.ignore_dimmer_cmd_timeout < millis()) {
            if ((TasmotaGlobal.power || Settings->flag3.tuya_apply_o20) && ((Tuya.Levels[dimIndex] > 0) && (Tuya.Levels[dimIndex] != Tuya.Snapshot[dimIndex]))) { // SetOption54 - Apply SetOption20 settings to Tuya device
              Tuya.ignore_dim = true;
              TasmotaGlobal.skip_light_fade = true;

              scmnd[0] = '\0';
              if ((fnId == TUYA_MCU_FUNC_DIMMER) || (fnId == TUYA_MCU_FUNC_REPORT1)) {
                if (Settings->flag3.pwm_multi_channels && (abs(Tuya.Levels[0] - changeUIntScale(Light.current_color[0], 0, 255, 0, 100))) > 1) {
                  snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_CHANNEL "1 %d"), Tuya.Levels[0]);
                }
                else if ((abs(Tuya.Levels[0] - light_state.getDimmer())) > 1) {
                  snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_DIMMER "3 %d"), Tuya.Levels[0]);
                }
              }
              if (((fnId == TUYA_MCU_FUNC_DIMMER2) || (fnId == TUYA_MCU_FUNC_REPORT2)) &&
                    Settings->flag3.pwm_multi_channels && (abs(Tuya.Levels[1] - changeUIntScale(Light.current_color[1], 0, 255, 0, 100))) > 1) {
                snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_CHANNEL "2 %d"), Tuya.Levels[1]);
              }
              if ((fnId == TUYA_MCU_FUNC_CT) && (abs(Tuya.Levels[1] - light_state.getCT())) > 1) {
                snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_COLORTEMPERATURE " %d"), Tuya.Levels[1]);
              }
              if ((fnId == TUYA_MCU_FUNC_WHITE) && (abs(Tuya.Levels[1] - light_state.getDimmer(2))) > 1) {
                snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_WHITE " %d"), Tuya.Levels[1]);
              }
              if (scmnd[0] != '\0') {
                ExecuteCommand(scmnd, SRC_SWITCH);
              }
            }
            Tuya.Snapshot[dimIndex] = Tuya.Levels[dimIndex];
          }
        }
  #ifdef USE_ENERGY_SENSOR
        else if (tuya_energy_enabled && fnId == TUYA_MCU_FUNC_VOLTAGE) {
          Energy.voltage[0] = (float)packetValue / 10;
          AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Rx ID=%d Voltage=%d"), Tuya.buffer[dpidStart], packetValue);
        } else if (tuya_energy_enabled && fnId == TUYA_MCU_FUNC_CURRENT) {
          Energy.current[0] = (float)packetValue / 1000;
          AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Rx ID=%d Current=%d"), Tuya.buffer[dpidStart], packetValue);
        } else if (tuya_energy_enabled && fnId == TUYA_MCU_FUNC_POWER) {
          Energy.active_power[0] = (float)packetValue / 10;
          AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Rx ID=%d Active_Power=%d"), Tuya.buffer[dpidStart], packetValue);

          if (RtcTime.valid) {
            if (Tuya.lastPowerCheckTime != 0 && Energy.active_power[0] > 0) {
              Energy.kWhtoday += Energy.active_power[0] * (float)(Rtc.utc_time - Tuya.lastPowerCheckTime) / 36.0;
              EnergyUpdateToday();
            }
            Tuya.lastPowerCheckTime = Rtc.utc_time;
          }
        } else if (tuya_energy_enabled && fnId == TUYA_MCU_FUNC_POWER_TOTAL) {
          EnergyUpdateTotal((float)packetValue / 100,true);
          AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Rx ID=%d Total_Power=%d"), Tuya.buffer[dpidStart], packetValue);
        }
  #endif // USE_ENERGY_SENSOR
      }
      else if (Tuya.buffer[dpidStart + 1] == 3) {  // Data Type 3
        const unsigned char *dpData = (unsigned char*)&Tuya.buffer[dpidStart + 4];
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
            if ((Tuya.Snapshot[2] != ((int)strtol(HSB1, NULL, 16)) ||
              Tuya.Snapshot[3] != ((int)strtol(HSB2, NULL, 16)) / 10 || Tuya.Snapshot[4] !=((int)strtol(HSB3, NULL, 16)) / 10)) {
            Tuya.Snapshot[2] = ((int)strtol(HSB1, NULL, 16));
            Tuya.Snapshot[3] = ((int)strtol(HSB2, NULL, 16)) / 10;
            Tuya.Snapshot[4] = ((int)strtol(HSB3, NULL, 16)) / 10;
            snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_HSBCOLOR " %d,%d,%d"), ((int)strtol(HSB1, NULL, 16)),
                      ((int)strtol(HSB2, NULL, 16)) / 10, ((int)strtol(HSB3, NULL, 16)) / 10);
            }
          }
          if (RGBType > 1 && dpDataLen == 14) {
            snprintf_P(RGB, sizeof(RGB), PSTR("%.6s\n"), &RgbData[0]);
            if (StrCmpNoCase(RGB, Tuya.RGBColor) != 0) {
              snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_COLOR " %s"), RGB);
              memcpy_P(Tuya.RGBColor, RGB, strlen(RGB));
            }
          }
          if (scmnd[0] != '\0') {
            ExecuteCommand(scmnd, SRC_SWITCH);
          }
        }

      }
      else if (Tuya.buffer[dpidStart + 1] == 4) {  // Data Type 4
        const unsigned char *dpData = (unsigned char*)&Tuya.buffer[dpidStart + 4];

        if ((fnId == TUYA_MCU_FUNC_MODESET)) { // Toggle light type
          Tuya.ModeSet = dpData[0];
          Tuya.Levels[3] = dpData[0];
        }
        if ((fnId >= TUYA_MCU_FUNC_ENUM1) && (fnId <= TUYA_MCU_FUNC_ENUM4)) {
          for (uint8_t i = 0; i <= 3; i++) {
            bool noupdate = false;
            if ((TUYA_MCU_FUNC_ENUM1 + i) == fnId) {
              if (Tuya.EnumState[i] != dpData[0]) {
                Tuya.EnumState[i] = dpData[0];
                snprintf_P(scmnd, sizeof(scmnd), PSTR(D_PRFX_TUYA D_CMND_TUYA_ENUM "%d %d"), i+1, dpData[0]);
                ExecuteCommand(scmnd, SRC_SWITCH);
              }
            }
          }
        }
      }
      dpidStart += dpDataLen + 4;
  }
}
void TuyaLowPowerModePacketProcess(void) {
  switch (Tuya.buffer[3]) {
    case TUYA_CMD_QUERY_PRODUCT:
      TuyaHandleProductInfoPacket();
      TuyaSetWifiLed();
      break;

    case TUYA_LOW_POWER_CMD_STATE:
      TuyaProcessStatePacket();
      Tuya.send_success_next_second = true;
      break;
  }

}

void TuyaHandleProductInfoPacket(void) {
  uint16_t dataLength = Tuya.buffer[4] << 8 | Tuya.buffer[5];
  char *data = &Tuya.buffer[6];
  AddLog(LOG_LEVEL_INFO, PSTR("TYA: MCU Product ID: %.*s"), dataLength, data);
}

void TuyaSendLowPowerSuccessIfNeeded(void) {
  uint8_t success = 1;

  if (Tuya.send_success_next_second) {
    TuyaSendCmd(TUYA_LOW_POWER_CMD_STATE, &success, 1);
    Tuya.send_success_next_second = false;
  }
}

void TuyaNormalPowerModePacketProcess(void)
{
  switch (Tuya.buffer[3]) {
    case TUYA_CMD_QUERY_PRODUCT:
      TuyaHandleProductInfoPacket();
      TuyaSendCmd(TUYA_CMD_MCU_CONF);
      break;

    case TUYA_CMD_HEARTBEAT:
      AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Heartbeat"));
      if (Tuya.buffer[6] == 0) {
        AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Detected MCU restart"));
        Tuya.wifi_state = -2;
      }
      break;

    case TUYA_CMD_STATE:
      TuyaProcessStatePacket();
      break;

    case TUYA_CMD_WIFI_RESET:
    case TUYA_CMD_WIFI_SELECT:
      AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: RX WiFi Reset"));
      TuyaResetWifi();
      break;

    case TUYA_CMD_WIFI_STATE:
      AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: RX WiFi LED set ACK"));
      Tuya.wifi_state = TuyaGetTuyaWifiState();
      break;

    case TUYA_CMD_MCU_CONF:
      AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: RX MCU configuration Mode=%d"), Tuya.buffer[5]);

      if (Tuya.buffer[5] == 2) { // Processing by ESP module mode
        uint8_t led1_gpio = Tuya.buffer[6];
        uint8_t key1_gpio = Tuya.buffer[7];
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
      TuyaRequestState(0);
      break;
#ifdef USE_TUYA_TIME
    case TUYA_CMD_SET_TIME:
      TuyaSetTime();
      break;
#endif
    default:
      AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: RX unknown command"));
  }
}

/*********************************************************************************************\
 * API Functions
\*********************************************************************************************/

bool TuyaModuleSelected(void)
{
  if (!PinUsed(GPIO_TUYA_RX) || !PinUsed(GPIO_TUYA_TX)) {  // fallback to hardware-serial if not explicitly selected
    SetPin(1, AGPIO(GPIO_TUYA_TX));
    SetPin(3, AGPIO(GPIO_TUYA_RX));
    Settings->my_gp.io[1] = AGPIO(GPIO_TUYA_TX);
    Settings->my_gp.io[3] = AGPIO(GPIO_TUYA_RX);
    TasmotaGlobal.restart_flag = 2;
  }

  if (TuyaGetDpId(TUYA_MCU_FUNC_DIMMER) == 0 && TUYA_DIMMER_ID > 0) {
    TuyaAddMcuFunc(TUYA_MCU_FUNC_DIMMER, TUYA_DIMMER_ID);
  }

  bool relaySet = false;

  for (uint8_t i = 0 ; i < MAX_TUYA_FUNCTIONS; i++) {
    if ((Settings->tuya_fnid_map[i].fnid >= TUYA_MCU_FUNC_REL1 && Settings->tuya_fnid_map[i].fnid <= TUYA_MCU_FUNC_REL8 ) ||
    (Settings->tuya_fnid_map[i].fnid >= TUYA_MCU_FUNC_REL1_INV && Settings->tuya_fnid_map[i].fnid <= TUYA_MCU_FUNC_REL8_INV )) {
      relaySet = true;
      TasmotaGlobal.devices_present++;
    }
  }

  if (!relaySet && TuyaGetDpId(TUYA_MCU_FUNC_DUMMY) == 0) { //by default the first relay is created automatically the dummy let remove it if not needed
    TuyaAddMcuFunc(TUYA_MCU_FUNC_REL1, 1);
    TasmotaGlobal.devices_present++;
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
    Tuya.low_power_mode = true;
    Settings->flag3.fast_power_cycle_disable = true;  // SetOption65 - Disable fast power cycle detection for device reset
  }

  UpdateDevices();
  return true;
}

void TuyaInit(void)
{
  int baudrate = 9600;
  if (Settings->flag4.tuyamcu_baudrate) { baudrate = 115200; }  // SetOption97 - Set Baud rate for TuyaMCU serial communication (0 = 9600 or 1 = 115200)

  Tuya.buffer = (char*)(malloc(TUYA_BUFFER_SIZE));
  if (Tuya.buffer != nullptr) {
    TuyaSerial = new TasmotaSerial(Pin(GPIO_TUYA_RX), Pin(GPIO_TUYA_TX), 2);
    if (TuyaSerial->begin(baudrate)) {
      if (TuyaSerial->hardwareSerial()) { ClaimSerial(); }
      // Get MCU Configuration
      Tuya.SuspendTopic = true;
      Tuya.ignore_topic_timeout = millis() + 1000; // suppress /STAT topic for 1000ms to avoid data overflow
      AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Request MCU configuration at %d bps"), baudrate);


    }
  }
  Tuya.heartbeat_timer = 0; // init heartbeat timer when dimmer init is done
}

void TuyaSerialInput(void)
{
  while (TuyaSerial->available()) {
    yield();
    uint8_t serial_in_byte = TuyaSerial->read();

    if (serial_in_byte == 0x55) {            // Start TUYA Packet
      Tuya.cmd_status = 1;
      Tuya.buffer[Tuya.byte_counter++] = serial_in_byte;
      Tuya.cmd_checksum += serial_in_byte;
    }
    else if (Tuya.cmd_status == 1 && serial_in_byte == 0xAA) { // Only packtes with header 0x55AA are valid
      Tuya.cmd_status = 2;

      Tuya.byte_counter = 0;
      Tuya.buffer[Tuya.byte_counter++] = 0x55;
      Tuya.buffer[Tuya.byte_counter++] = 0xAA;
      Tuya.cmd_checksum = 0xFF;
    }
    else if (Tuya.cmd_status == 2) {
      if (Tuya.byte_counter == 5) { // Get length of data
        Tuya.cmd_status = 3;
        Tuya.data_len = serial_in_byte;
      }
      Tuya.cmd_checksum += serial_in_byte;
      Tuya.buffer[Tuya.byte_counter++] = serial_in_byte;
    }
    else if ((Tuya.cmd_status == 3) && (Tuya.byte_counter == (6 + Tuya.data_len)) && (Tuya.cmd_checksum == serial_in_byte)) { // Compare checksum and process packet
      Tuya.buffer[Tuya.byte_counter++] = serial_in_byte;

      char hex_char[(Tuya.byte_counter * 2) + 2];
      uint16_t len = Tuya.buffer[4] << 8 | Tuya.buffer[5];

      Response_P(PSTR("{\"" D_JSON_TUYA_MCU_RECEIVED "\":{\"Data\":\"%s\",\"Cmnd\":%d"), ToHex_P((unsigned char*)Tuya.buffer, Tuya.byte_counter, hex_char, sizeof(hex_char)), Tuya.buffer[3]);

      uint16_t DataVal = 0;
      uint8_t dpId = 0;
      uint8_t dpDataType = 0;
      char DataStr[15];

      if (len > 0) {
        ResponseAppend_P(PSTR(",\"CmndData\":\"%s\""), ToHex_P((unsigned char*)&Tuya.buffer[6], len, hex_char, sizeof(hex_char)));
        if (TUYA_CMD_STATE == Tuya.buffer[3]) {
          //55 AA 03 07 00 0D 01 04 00 01 02 02 02 00 04 00 00 00 1A 40
          // 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19
          uint8_t dpidStart = 6;
          while (dpidStart + 4 < Tuya.byte_counter) {
            dpId = Tuya.buffer[dpidStart];
            dpDataType = Tuya.buffer[dpidStart + 1];
            uint16_t dpDataLen = Tuya.buffer[dpidStart + 2] << 8 | Tuya.buffer[dpidStart + 3];
            const unsigned char *dpData = (unsigned char*)&Tuya.buffer[dpidStart + 4];
            const char *dpHexData = ToHex_P(dpData, dpDataLen, hex_char, sizeof(hex_char));

            if (TUYA_CMD_STATE == Tuya.buffer[3]) {
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
                ResponseAppend_P(PSTR("\"0x%s\""), dpHexData);
                snprintf_P(DataStr, sizeof(DataStr), PSTR("%s"), dpHexData);
              }
            }

            ResponseAppend_P(PSTR(",\"%d\":{\"DpId\":%d,\"DpIdType\":%d,\"DpIdData\":\"%s\""), dpId, dpId, dpDataType, dpHexData);
            if (TUYA_TYPE_STRING == dpDataType) {
              ResponseAppend_P(PSTR(",\"Type3Data\":\"%.*s\""), dpDataLen, dpData);
            }
            ResponseAppend_P(PSTR("}"));
            dpidStart += dpDataLen + 4;
          }
        }
      }
      ResponseAppend_P(PSTR("}}"));

      if (Settings->flag3.tuya_serial_mqtt_publish) {  // SetOption66 - Enable TuyaMcuReceived messages over Mqtt
        MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_TUYA_MCU_RECEIVED));
      } else {
#ifdef MQTT_DATA_STRING
        AddLog(LOG_LEVEL_DEBUG, TasmotaGlobal.mqtt_data.c_str());
#else
        AddLog(LOG_LEVEL_DEBUG, TasmotaGlobal.mqtt_data);
#endif
      }
      XdrvRulesProcess(0);

      if (dpId != 0 && Settings->tuyamcu_topic) { // Publish a /STAT Topic ready to use for any home automation system
        if (!Tuya.SuspendTopic) {
          char scommand[13];
          snprintf_P(scommand, sizeof(scommand), PSTR("DpType%uId%u"), dpDataType, dpId);
          if (dpDataType != 3 && dpDataType != 5) { Response_P(PSTR("%u"), DataVal); }
          else { Response_P(PSTR("%s"), DataStr); }
          MqttPublishPrefixTopic_P(STAT, scommand);
        }
      }

      if (!Tuya.low_power_mode) {
        TuyaNormalPowerModePacketProcess();
      } else {
        TuyaLowPowerModePacketProcess();
      }

      Tuya.byte_counter = 0;
      Tuya.cmd_status = 0;
      Tuya.cmd_checksum = 0;
      Tuya.data_len = 0;
    }                                                    // read additional packets from TUYA
    else if (Tuya.byte_counter < TUYA_BUFFER_SIZE -1) {  // add char to string if it still fits
      Tuya.buffer[Tuya.byte_counter++] = serial_in_byte;
      Tuya.cmd_checksum += serial_in_byte;
    } else {
      Tuya.byte_counter = 0;
      Tuya.cmd_status = 0;
      Tuya.cmd_checksum = 0;
      Tuya.data_len = 0;
    }
  }
}

bool TuyaButtonPressed(void)
{
  if (!XdrvMailbox.index && ((PRESSED == XdrvMailbox.payload) && (NOT_PRESSED == Button.last_state[XdrvMailbox.index]))) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Reset GPIO triggered"));
    TuyaResetWifi();
    return true;  // Reset GPIO served here
  }
  return false;   // Don't serve other buttons
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

void TuyaSetWifiLed(void)
{
  Tuya.wifi_state = TuyaGetTuyaWifiState();
  AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: Set WiFi LED %d (%d)"), Tuya.wifi_state, WifiState());

  if (Tuya.low_power_mode) {
    TuyaSendCmd(TUYA_LOW_POWER_CMD_WIFI_STATE, &Tuya.wifi_state, 1);
  } else {
    TuyaSendCmd(TUYA_CMD_WIFI_STATE, &Tuya.wifi_state, 1);
  }
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

#ifdef USE_ENERGY_SENSOR

/*********************************************************************************************\
 * Energy Interface
\*********************************************************************************************/

bool Xnrg32(uint8_t function)
{
  bool result = false;

  if (TUYA_DIMMER == TasmotaGlobal.module_type) {
    if (FUNC_PRE_INIT == function) {
      if (TuyaGetDpId(TUYA_MCU_FUNC_POWER) != 0 || TuyaGetDpId(TUYA_MCU_FUNC_POWER_COMBINED) != 0) {
        if (TuyaGetDpId(TUYA_MCU_FUNC_CURRENT) == 0 && TuyaGetDpId(TUYA_MCU_FUNC_POWER_COMBINED) == 0) {
          Energy.current_available = false;
        }
        if (TuyaGetDpId(TUYA_MCU_FUNC_VOLTAGE) == 0 && TuyaGetDpId(TUYA_MCU_FUNC_POWER_COMBINED) == 0) {
          Energy.voltage_available = false;
        }
        TasmotaGlobal.energy_driver = XNRG_32;
      }
    }
  }
  return result;
}
#endif  // USE_ENERGY_SENSOR

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
                        (Tuya.SensorsValid[sensor-71] ? dtostrfd(TuyaAdjustedTemperature(Tuya.Sensors[sensor-71], res), res, tempval) : PSTR("null")));
        added = true;
      }
  #ifdef USE_WEBSERVER
    } else {
      if (TuyaGetDpId(sensor) != 0) {
        switch (sensor) {
          case 71:
            WSContentSend_Temp("", TuyaAdjustedTemperature(Tuya.Sensors[0], Settings->flag2.temperature_resolution));
            break;
          case 72:
            WSContentSend_PD(PSTR("{s}" D_TEMPERATURE " Set{m}%s " D_UNIT_DEGREE "%c{e}"),
                            dtostrfd(TuyaAdjustedTemperature(Tuya.Sensors[1], Settings->mbflag2.temperature_set_res), Settings->mbflag2.temperature_set_res, tempval), TempUnit());
            break;
          case 73:
            WSContentSend_PD(HTTP_SNS_HUM, "", dtostrfd(TuyaAdjustedTemperature(Tuya.Sensors[2], Settings->flag2.humidity_resolution), Settings->flag2.humidity_resolution, tempval));
            break;
          case 74:
            WSContentSend_PD(PSTR("{s}" D_HUMIDITY " Set{m}%s " D_UNIT_PERCENT "{e}"),
                            dtostrfd(TuyaAdjustedTemperature(Tuya.Sensors[3], Settings->flag2.humidity_resolution), Settings->flag2.humidity_resolution, tempval));
            break;
          case 75:
            WSContentSend_PD(HTTP_SNS_ILLUMINANCE, "", Tuya.Sensors[4]);
            break;
          case 76:
            WSContentSend_PD(PSTR("{s}" D_TVOC "{m}%d " D_UNIT_PARTS_PER_MILLION "{e}"), Tuya.Sensors[5]);
            break;
          case 77:
            WSContentSend_PD(HTTP_SNS_CO2, "", Tuya.Sensors[6]);
            break;
          case 78:
            WSContentSend_PD(HTTP_SNS_CO2EAVG, "", Tuya.Sensors[7]);
            break;
          case 79:
            WSContentSend_PD(HTTP_SNS_GAS, "", Tuya.Sensors[8]);
            break;
          case 81:
          case 82:
          case 83:
          case 84:
            WSContentSend_PD(PSTR("{s}Timer%d{m}%d{e}"), (sensor-80), Tuya.Sensors[sensor-71]); // No UoM for timers since they can be sec or min
            break;
        }
      }

  #endif  // USE_WEBSERVER
    }
  }
  #ifdef USE_WEBSERVER
  if (AsModuleTuyaMS()) {
    WSContentSend_P(PSTR("{s}" D_JSON_IRHVAC_MODE "{m}%d{e}"), Tuya.ModeSet);
  }
  #endif  // USE_WEBSERVER

  if (RootName) { ResponseJsonEnd();}
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv16(uint8_t function)
{
  bool result = false;

  if (TUYA_DIMMER == TasmotaGlobal.module_type) {
    switch (function) {
      case FUNC_LOOP:
        if (TuyaSerial) { TuyaSerialInput(); }
        break;
      case FUNC_MODULE_INIT:
        result = TuyaModuleSelected();
        break;
      case FUNC_PRE_INIT:
        TuyaInit();
        break;
      case FUNC_SET_DEVICE_POWER:
        result = TuyaSetPower();
        break;
      case FUNC_BUTTON_PRESSED:
        result = TuyaButtonPressed();
        break;
      case FUNC_EVERY_SECOND:
        if (TuyaSerial && Tuya.wifi_state != TuyaGetTuyaWifiState()) { TuyaSetWifiLed(); }
        if (!Tuya.low_power_mode) {
          Tuya.heartbeat_timer++;
          if (Tuya.heartbeat_timer > 10) {
            Tuya.heartbeat_timer = 0;
            TuyaSendCmd(TUYA_CMD_HEARTBEAT);
          }
#ifdef USE_TUYA_TIME
          if (!(TasmotaGlobal.uptime % 60)) {
            TuyaSetTime();
          }
#endif  //USE_TUYA_TIME
        } else {
          TuyaSendLowPowerSuccessIfNeeded();
        }
        if (Tuya.ignore_topic_timeout < millis()) { Tuya.SuspendTopic = false; }
        break;
      case FUNC_SET_CHANNELS:
        result = TuyaSetChannels();
        break;
      case FUNC_MQTT_INIT:
        TuyaSendCmd(TUYA_CMD_QUERY_PRODUCT);
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kTuyaCommand, TuyaCommand);
        break;
      case FUNC_JSON_APPEND:
        TuyaSensorsShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        TuyaSensorsShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_TUYA_MCU
#endif  // USE_LIGHT
