/*
  xdrv_16_tuyamcu.ino - Tuya MCU support for Tasmota

  Copyright (C) 2020 Federico Leoni, digiblur, Joel Stein and Theo Arends

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
  uint16_t Snapshot[5] = {0,0,0,0,0};     // Array to store a snapshot of Tasmota actual values for channels
  char HSBColor[13];                      // Stores HSB Color string in Hex format
  uint16_t CTMin = 153;                   // Minimum CT level allowed - When SetOption82 is enabled will default to 200
  uint16_t CTMax = 500;                   // Maximum CT level allowed - When SetOption82 is enabled will default to 380
  bool ModeSet = false;                   // Controls 0 - Single Tone light, 1 - RGB Light
  uint8_t FanState = 0;                   // Stores the current fan speed
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

const char kTuyaCommand[] PROGMEM = "|"  // No prefix
  D_CMND_TUYA_MCU "|" D_CMND_TUYA_MCU_SEND_STATE;

void (* const TuyaCommand[])(void) PROGMEM = {
  &CmndTuyaMcu, &CmndTuyaSend
};

/*********************************************************************************************\
 * Web Interface
\*********************************************************************************************/
bool IsModuleTuya(void)
{
  return ((TUYA_DIMMER == my_module_type) || (SK03_TUYA == my_module_type));
}

bool AsModuleTuyaMS(void) // ModeSet Layout
{
  return ((light_type > LT_RGB) && TuyaGetDpId(TUYA_MCU_FUNC_MODESET) != 0);
}

bool IsTuyaFanCtrl(void) // Fan Speed Controller Layout
{
  return ((TuyaGetDpId(TUYA_MCU_FUNC_FAN3) != 0) || (TuyaGetDpId(TUYA_MCU_FUNC_FAN4) != 0) ||
          (TuyaGetDpId(TUYA_MCU_FUNC_FAN5) != 0) || (TuyaGetDpId(TUYA_MCU_FUNC_FAN6) != 0));
}

bool TuyaModeSet(void) // ModeSet Status
{
  return Tuya.ModeSet;
}

uint8_t TuyaFanSpeeds(void) // Number of Fan Speeds for WebUI
{
  uint8_t FanSpeeds = 0;
  for (uint32_t i = 0; i <= 3; i++) {
    if (TuyaGetDpId(TUYA_MCU_FUNC_FAN3 + i) != 0) {
      FanSpeeds = i + 2;
    }
  }
  return FanSpeeds;
}

uint8_t TuyaFanState(void) // Fan Speed Status
{
  return Tuya.FanState;
}
// Web Interface

/*
TuyaSend<x> dpId,data

TuyaSend0 -> Sends TUYA_CMD_QUERY_STATE
TuyaSend1 11,1 -> Sends boolean (Type 1) data 0/1 to dpId 11 (Max data length 1 byte)
TuyaSend2 11,100 -> Sends integer (Type 2) data 100 to dpId 11 (Max data length 4 bytes)
TuyaSend2 11,0xAABBCCDD -> Sends 4 bytes (Type 2) data to dpId 11 (Max data length 4 bytes)
TuyaSend3 11,ThisIsTheData -> Sends the supplied string (Type 3) to dpId 11 ( Max data length not-known)
TuyaSend4 11,1 -> Sends enum (Type 4) data 0/1/2/3/4/5 to dpId 11 (Max data length 1 bytes)
*/

void CmndTuyaSend(void) {
  if (XdrvMailbox.index > 4 && XdrvMailbox.index < 8) {
    return;
  }
  if (XdrvMailbox.index == 0) {
    TuyaRequestState(0);
  } else if (XdrvMailbox.index == 8) {
    TuyaRequestState(8);
  } else if (XdrvMailbox.index == 9) { // TuyaSend Topic Toggle
    if (Settings.tuyamcu_topic) { Settings.tuyamcu_topic = 0; } else { Settings.tuyamcu_topic = 1; }
    AddLog_P2(LOG_LEVEL_INFO, PSTR("TYA: TuyaMCU Stat Topic %s"), (Settings.tuyamcu_topic ? PSTR("enabled") : PSTR("disabled")));

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
    //   TuyaAddMcuFunc(parm[0], parm[1]);
    //   restart_flag = 2;
    // } else {
    //   AddLog_P2(LOG_LEVEL_ERROR, PSTR("TYA: TuyaMcu Invalid function id=%d"), parm[0]);
    // }
      bool DualDim;
      if (TUYA_MCU_FUNC_DIMMER2 == parm[0] && parm[1] != 0) {
        if (TuyaGetDpId(TUYA_MCU_FUNC_DIMMER) != 0) { DualDim = true; }
      } else if (TUYA_MCU_FUNC_DIMMER == parm[0] && parm[1] != 0) {
        if (TuyaGetDpId(TUYA_MCU_FUNC_DIMMER2) != 0) { DualDim = true; }
      } else if ((TUYA_MCU_FUNC_DIMMER == parm[0] && parm[1] == 0) || (TUYA_MCU_FUNC_DIMMER2 == parm[0] && parm[1] == 0)) { DualDim = false; };
      if (DualDim) {
        if (TuyaGetDpId(TUYA_MCU_FUNC_CT) != 0) { TuyaAddMcuFunc(TUYA_MCU_FUNC_CT, 0); } // If the second dimmer is enabled CT, RGB or WHITE function must be removed
        if (TuyaGetDpId(TUYA_MCU_FUNC_RGB) != 0) { TuyaAddMcuFunc(TUYA_MCU_FUNC_RGB, 0); }
        if (TuyaGetDpId(TUYA_MCU_FUNC_WHITE) != 0) { TuyaAddMcuFunc(TUYA_MCU_FUNC_WHITE, 0); }
        Settings.flag3.pwm_multi_channels = 1;
      } else { Settings.flag3.pwm_multi_channels = 0; }
      TuyaAddMcuFunc(parm[0], parm[1]);
      restart_flag = 2;
    } else {
      AddLog_P2(LOG_LEVEL_ERROR, PSTR("TYA: TuyaMcu Invalid function id=%d"), parm[0]);
    }
  }

  Response_P(PSTR("{\"" D_CMND_TUYA_MCU "\":["));
  bool added = false;
  for (uint8_t i = 0; i < MAX_TUYA_FUNCTIONS; i++) {
    if (Settings.tuya_fnid_map[i].fnid != 0) {
      if (added) {
        ResponseAppend_P(PSTR(","));
      }
      ResponseAppend_P(PSTR("{\"fnId\":%d,\"dpId\":%d}" ), Settings.tuya_fnid_map[i].fnid, Settings.tuya_fnid_map[i].dpid);
      added = true;
    }
  }
  ResponseAppend_P(PSTR("]}"));
}

/*********************************************************************************************\
 * Internal Functions
\*********************************************************************************************/

void TuyaAddMcuFunc(uint8_t fnId, uint8_t dpId) {
  bool added = false;

  if (fnId == 0 || dpId == 0) { // Delete entry
    for (uint8_t i = 0; i < MAX_TUYA_FUNCTIONS; i++) {
      if ((dpId > 0 && Settings.tuya_fnid_map[i].dpid == dpId) || (fnId > TUYA_MCU_FUNC_NONE && Settings.tuya_fnid_map[i].fnid == fnId)) {
        Settings.tuya_fnid_map[i].fnid = TUYA_MCU_FUNC_NONE;
        Settings.tuya_fnid_map[i].dpid = 0;
        break;
      }
    }
  } else { // Add or update
    for (uint8_t i = 0; i < MAX_TUYA_FUNCTIONS; i++) {
      if (Settings.tuya_fnid_map[i].dpid == dpId || Settings.tuya_fnid_map[i].dpid == 0 || Settings.tuya_fnid_map[i].fnid == fnId || Settings.tuya_fnid_map[i].fnid == 0) {
        if (!added) { // Update entry if exisiting entry or add
          Settings.tuya_fnid_map[i].fnid = fnId;
          Settings.tuya_fnid_map[i].dpid = dpId;
          added = true;
        } else if (Settings.tuya_fnid_map[i].dpid == dpId || Settings.tuya_fnid_map[i].fnid == fnId) { // Remove existing entry if added to empty place
          Settings.tuya_fnid_map[i].fnid = TUYA_MCU_FUNC_NONE;
          Settings.tuya_fnid_map[i].dpid = 0;
        }
      }
    }
  }
  UpdateDevices();
}

void UpdateDevices() {
  for (uint8_t i = 0; i < MAX_TUYA_FUNCTIONS; i++) {
    uint8_t fnId = Settings.tuya_fnid_map[i].fnid;
    if (fnId > TUYA_MCU_FUNC_NONE && Settings.tuya_fnid_map[i].dpid > 0) {

      if (fnId >= TUYA_MCU_FUNC_REL1 && fnId <= TUYA_MCU_FUNC_REL8) { //Relay
        bitClear(rel_inverted, fnId - TUYA_MCU_FUNC_REL1);
      } else if (fnId >= TUYA_MCU_FUNC_REL1_INV && fnId <= TUYA_MCU_FUNC_REL8_INV) { // Inverted Relay
        bitSet(rel_inverted, fnId - TUYA_MCU_FUNC_REL1_INV);
      }

    }
  }
}

inline bool TuyaFuncIdValid(uint8_t fnId) {
  return (fnId >= TUYA_MCU_FUNC_SWT1 && fnId <= TUYA_MCU_FUNC_SWT4) ||
          (fnId >= TUYA_MCU_FUNC_REL1 && fnId <= TUYA_MCU_FUNC_REL8) ||
          (fnId >= TUYA_MCU_FUNC_DIMMER && fnId <= TUYA_MCU_FUNC_REPORT2) ||
          (fnId >= TUYA_MCU_FUNC_POWER && fnId <= TUYA_MCU_FUNC_BATTERY_PERCENTAGE) ||
          (fnId >= TUYA_MCU_FUNC_REL1_INV && fnId <= TUYA_MCU_FUNC_REL8_INV) ||
          (fnId >= TUYA_MCU_FUNC_FAN3 && fnId <= TUYA_MCU_FUNC_FAN6) ||
          (fnId >= TUYA_MCU_FUNC_MOTOR_DIR && fnId <= TUYA_MCU_FUNC_DUMMY) ||
          (fnId == TUYA_MCU_FUNC_LOWPOWER_MODE);
}

uint8_t TuyaGetFuncId(uint8_t dpid) {
  for (uint8_t i = 0; i < MAX_TUYA_FUNCTIONS; i++) {
    if (Settings.tuya_fnid_map[i].dpid == dpid) {
      return Settings.tuya_fnid_map[i].fnid;
    }
  }
  return TUYA_MCU_FUNC_NONE;
}

uint8_t TuyaGetDpId(uint8_t fnId) {
  for (uint8_t i = 0; i < MAX_TUYA_FUNCTIONS; i++) {
    if (Settings.tuya_fnid_map[i].fnid == fnId) {
      return Settings.tuya_fnid_map[i].dpid;
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
  snprintf_P(log_data, sizeof(log_data), PSTR("TYA: Send \"55aa00%02x%02x%02x"), cmd, payload_len >> 8, payload_len & 0xFF);
  for (uint32_t i = 0; i < payload_len; ++i) {
    TuyaSerial->write(payload[i]);
    checksum += payload[i];
    snprintf_P(log_data, sizeof(log_data), PSTR("%s%02x"), log_data, payload[i]);
  }
  TuyaSerial->write(checksum);
  TuyaSerial->flush();
  snprintf_P(log_data, sizeof(log_data), PSTR("%s%02x\""), log_data, checksum);
  AddLog(LOG_LEVEL_DEBUG);
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

  uint8_t dpid = TuyaGetDpId(TUYA_MCU_FUNC_REL1 + active_device - 1);
  if (dpid == 0) dpid = TuyaGetDpId(TUYA_MCU_FUNC_REL1_INV + active_device - 1);

  if (source != SRC_SWITCH && TuyaSerial) {  // ignore to prevent loop from pushing state from faceplate interaction
    TuyaSendBool(dpid, bitRead(rpower, active_device-1) ^ bitRead(rel_inverted, active_device-1));
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
  char hex_char[13];
  bool noupd = false;
  bool LightMode = TuyaGetDpId(TUYA_MCU_FUNC_MODESET) != 0;
  uint8_t idx = 0;
  snprintf_P(hex_char, sizeof(hex_char), PSTR("000000000000"));

  if (LT_SERIAL1 == light_type) {
    Tuya.Snapshot[0] = light_state.getDimmer();
  }
  if (LT_SERIAL2 == light_type || LT_RGBWC == light_type) {
    idx = 1;
    if (LT_SERIAL2 == light_type && Settings.flag3.pwm_multi_channels && (TuyaGetDpId(TUYA_MCU_FUNC_DIMMER2) != 0)) {
      // Special setup for dual dimmer (like the MOES 2 Way Dimmer) emulating 2 PWM channels
      Tuya.Snapshot[0] = changeUIntScale(Light.current_color[0], 0, 255, 0, 100);
      Tuya.Snapshot[1] = changeUIntScale(Light.current_color[1], 0, 255, 0, 100);
    } else { // CT Light or RGBWC
      light_state.getCTRange(&Tuya.CTMin, &Tuya.CTMax); // SetOption82 - Reduce the CT range from 153..500 to 200..380 to accomodate with Alexa range
      Tuya.Snapshot[0] = light_state.getDimmer();
      Tuya.Snapshot[1] = light_state.getCT();
    }
  }
  if (LT_RGBW == light_type) {
    idx = 1;
    Tuya.Snapshot[0] = light_state.getDimmer(1);
    Tuya.Snapshot[1] = light_state.getDimmer(2);
  }

  if (light_type > LT_BASIC) {

    if (LT_RGB != light_type) {
      for (uint8_t i = 0; i <= idx; i++) {
        if (Tuya.Snapshot[i] != Tuya.Levels[i]) {
          if (i == 0 && LightMode && Tuya.ModeSet ) { noupd = true;}
          if (!noupd) {
            LightSerialDuty(Tuya.Snapshot[i], &hex_char[0], i+1);
            Tuya.Levels[i] = Tuya.Snapshot[i];
          }
          noupd = false;
        }
      }
    }

    if (light_type >= LT_RGB) {

      light_state.getHSB(&hue, &sat, &bri);
      sat = changeUIntScale(sat, 0, 255, 0, 100);
      bri = changeUIntScale(bri, 0, 255, 0, 100);
      if (hue != Tuya.Snapshot[2] || sat != Tuya.Snapshot[3] || bri != Tuya.Snapshot[4]) {
        if ((LightMode && Tuya.ModeSet) || LT_RGB == light_type) {
          snprintf_P(hex_char, sizeof(hex_char), PSTR("%04X%04X%04X"), hue, sat * 10, bri * 10); // Create a TuyaMCU readable RGB payload
          LightSerialDuty(0, &hex_char[0], 3);
          memcpy_P(Tuya.HSBColor, hex_char, strlen(hex_char));
          Tuya.Snapshot[2] = hue;
          Tuya.Snapshot[3] = sat;
          Tuya.Snapshot[4] = bri;
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
      if (!Settings.flag3.pwm_multi_channels) {
      CTLight = true;
      dpid = TuyaGetDpId(TUYA_MCU_FUNC_CT);
      } else { dpid = TuyaGetDpId(TUYA_MCU_FUNC_DIMMER2); }
    }

    if (duty > 0 && !Tuya.ignore_dim && TuyaSerial && dpid > 0) {
      if (TuyaIdx == 2 && CTLight) {
        duty = changeUIntScale(duty, Tuya.CTMin, Tuya.CTMax, Settings.dimmer_hw_max, 0);
      } else { duty = changeUIntScale(duty, 0, 100, 0, Settings.dimmer_hw_max); }

      if (duty < Settings.dimmer_hw_min) { duty = Settings.dimmer_hw_min; }  // dimming acts odd below 25(10%) - this mirrors the threshold set on the faceplate itself
        Tuya.ignore_dimmer_cmd_timeout = millis() + 250; // Ignore serial received dim commands for the next 250ms
        if (Tuya.ModeSet && (TuyaGetDpId(TUYA_MCU_FUNC_MODESET) != 0) && light_type > LT_RGB) {
          TuyaSendEnum(TuyaGetDpId(TUYA_MCU_FUNC_MODESET), 0);
        }
        TuyaSendValue(dpid, duty);

    } else if (dpid > 0 && TuyaIdx <= 2) {

      Tuya.ignore_dim = false;  // reset flag

      if (TuyaIdx == 2 && CTLight) {
        duty = changeUIntScale(duty, Tuya.CTMin, Tuya.CTMax, Settings.dimmer_hw_max, 0);
      } else {
        duty = changeUIntScale(duty, 0, 100, 0, Settings.dimmer_hw_max);
      }
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TYA: Send dim skipped value %d for dpid %d"), duty, dpid);  // due to 0 or already set
    } else {
      AddLog_P(LOG_LEVEL_DEBUG, PSTR("TYA: Cannot set dimmer. Dimmer Id unknown"));  //
    }
  }

  if (TuyaIdx == 3) {
    dpid = TuyaGetDpId(TUYA_MCU_FUNC_RGB);
    if (!Tuya.ModeSet && (TuyaGetDpId(TUYA_MCU_FUNC_MODESET) != 0) && light_type > LT_RGB) {
        TuyaSendEnum(TuyaGetDpId(TUYA_MCU_FUNC_MODESET), 1);
    }
    TuyaSendString(dpid, hex_char);
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TYA: TX RGB hex %s to dpId %d"), hex_char, dpid);
  }
}

void TuyaRequestState(uint8_t state_type)
{
  if (TuyaSerial) {
    // Get current status of MCU
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("TYA: Read MCU state"));
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
  if (!Settings.flag.button_restrict) {  // SetOption1 - Control button multipress
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

  while (dpidStart + 4 < Tuya.byte_counter) {
    dpDataLen = Tuya.buffer[dpidStart + 2] << 8 | Tuya.buffer[dpidStart + 3];
    fnId = TuyaGetFuncId(Tuya.buffer[dpidStart]);

    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TYA: fnId=%d is set for dpId=%d"), fnId, Tuya.buffer[dpidStart]);
    // if (TuyaFuncIdValid(fnId)) {
      if (Tuya.buffer[dpidStart + 1] == 1) {  // Data Type 1

        if (fnId >= TUYA_MCU_FUNC_REL1 && fnId <= TUYA_MCU_FUNC_REL8) {
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TYA: RX Relay-%d --> MCU State: %s Current State:%s"), fnId - TUYA_MCU_FUNC_REL1 + 1, Tuya.buffer[dpidStart + 4]?"On":"Off",bitRead(power, fnId - TUYA_MCU_FUNC_REL1)?"On":"Off");
          if ((power || Settings.light_dimmer > 0) && (Tuya.buffer[dpidStart + 4] != bitRead(power, fnId - TUYA_MCU_FUNC_REL1))) {
            ExecuteCommandPower(fnId - TUYA_MCU_FUNC_REL1 + 1, Tuya.buffer[dpidStart + 4], SRC_SWITCH);  // send SRC_SWITCH? to use as flag to prevent loop from inbound states from faceplate interaction
          }
        } else if (fnId >= TUYA_MCU_FUNC_REL1_INV && fnId <= TUYA_MCU_FUNC_REL8_INV) {
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TYA: RX Relay-%d-Inverted --> MCU State: %s Current State:%s"), fnId - TUYA_MCU_FUNC_REL1_INV + 1, Tuya.buffer[dpidStart + 4]?"Off":"On",bitRead(power, fnId - TUYA_MCU_FUNC_REL1_INV) ^ 1?"Off":"On");
          if (Tuya.buffer[dpidStart + 4] != bitRead(power, fnId - TUYA_MCU_FUNC_REL1_INV) ^ 1) {
            ExecuteCommandPower(fnId - TUYA_MCU_FUNC_REL1_INV + 1, Tuya.buffer[dpidStart + 4] ^ 1, SRC_SWITCH);  // send SRC_SWITCH? to use as flag to prevent loop from inbound states from faceplate interaction
          }
        } else if (fnId >= TUYA_MCU_FUNC_SWT1 && fnId <= TUYA_MCU_FUNC_SWT4) {
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TYA: RX Switch-%d --> MCU State: %d Current State:%d"),fnId - TUYA_MCU_FUNC_SWT1 + 1,Tuya.buffer[dpidStart + 4], SwitchGetVirtual(fnId - TUYA_MCU_FUNC_SWT1));

          if (SwitchGetVirtual(fnId - TUYA_MCU_FUNC_SWT1) != Tuya.buffer[dpidStart + 4]) {
            SwitchSetVirtual(fnId - TUYA_MCU_FUNC_SWT1, Tuya.buffer[dpidStart + 4]);
            SwitchHandler(1);
          }
        }
      }
      else if (Tuya.buffer[dpidStart + 1] == 2) {  // Data Type 2
        bool tuya_energy_enabled = (XNRG_32 == energy_flg);
        uint16_t packetValue = Tuya.buffer[dpidStart + 6] << 8 | Tuya.buffer[dpidStart + 7];
        uint8_t dimIndex;
        if ((fnId == TUYA_MCU_FUNC_FAN3) || (fnId == TUYA_MCU_FUNC_FAN4) ||
            (fnId == TUYA_MCU_FUNC_FAN5) || (fnId == TUYA_MCU_FUNC_FAN6)) {
          Tuya.FanState = packetValue;
        }

        if ((fnId == TUYA_MCU_FUNC_DIMMER) || (fnId == TUYA_MCU_FUNC_REPORT1)) {
          dimIndex = 0;
        }

        if (fnId == TUYA_MCU_FUNC_DIMMER2 || fnId == TUYA_MCU_FUNC_REPORT2 || fnId == TUYA_MCU_FUNC_CT) {
          dimIndex = 1;
          if (Settings.flag3.pwm_multi_channels) {
            Tuya.Levels[dimIndex] = changeUIntScale(packetValue, 0, Settings.dimmer_hw_max, 0, 100);
          } else {
            Tuya.Levels[dimIndex] = changeUIntScale(packetValue, 0, Settings.dimmer_hw_max, Tuya.CTMax, Tuya.CTMin);
          }
        }

        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TYA: RX value %d from dpId %d "), packetValue, Tuya.buffer[dpidStart]);

        if (Tuya.ignore_dimmer_cmd_timeout < millis()) {

          if ((power || Settings.flag3.tuya_apply_o20) && ((Tuya.Levels[dimIndex] > 0) && (Tuya.Levels[dimIndex] != Tuya.Snapshot[dimIndex]))) { // SetOption54 - Apply SetOption20 settings to Tuya device

            Tuya.ignore_dim = true;
            skip_light_fade = true;

            if ((fnId == TUYA_MCU_FUNC_DIMMER) || (fnId == TUYA_MCU_FUNC_REPORT1)) {
              if (Settings.flag3.pwm_multi_channels && (abs(Tuya.Levels[0] - changeUIntScale(Light.current_color[0], 0, 255, 0, 100))) > 1) {
                snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_CHANNEL "1 %d"), Tuya.Levels[0]);
              } else {
                if ((abs(Tuya.Levels[0] - light_state.getDimmer())) > 1) { snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_DIMMER "3 %d"), Tuya.Levels[0]); }
              }
              ExecuteCommand(scmnd, SRC_SWITCH);
            }

            if (((fnId == TUYA_MCU_FUNC_DIMMER2) || (fnId == TUYA_MCU_FUNC_REPORT2)) &&
                  Settings.flag3.pwm_multi_channels && (abs(Tuya.Levels[1] - changeUIntScale(Light.current_color[1], 0, 255, 0, 100))) > 1) {
              snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_CHANNEL "2 %d"), Tuya.Levels[1]);
              ExecuteCommand(scmnd, SRC_SWITCH);
            }

            if ((fnId == TUYA_MCU_FUNC_CT) && (abs(Tuya.Levels[1] - light_state.getCT())) > 1) {
              snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_COLORTEMPERATURE " %d"), Tuya.Levels[1]);
              ExecuteCommand(scmnd, SRC_SWITCH);
            }

            if ((fnId == TUYA_MCU_FUNC_WHITE) && (abs(Tuya.Levels[1] - light_state.getDimmer(2))) > 1) {
              snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_WHITE " %d"), Tuya.Levels[1]);
              ExecuteCommand(scmnd, SRC_SWITCH);
            }
          }
        }
  #ifdef USE_ENERGY_SENSOR
        else if (tuya_energy_enabled && fnId == TUYA_MCU_FUNC_VOLTAGE) {
          Energy.voltage[0] = (float)packetValue / 10;
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TYA: Rx ID=%d Voltage=%d"), Tuya.buffer[dpidStart], packetValue);
        } else if (tuya_energy_enabled && fnId == TUYA_MCU_FUNC_CURRENT) {
          Energy.current[0] = (float)packetValue / 1000;
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TYA: Rx ID=%d Current=%d"), Tuya.buffer[dpidStart], packetValue);
        } else if (tuya_energy_enabled && fnId == TUYA_MCU_FUNC_POWER) {
          Energy.active_power[0] = (float)packetValue / 10;
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TYA: Rx ID=%d Active_Power=%d"), Tuya.buffer[dpidStart], packetValue);

          if (Tuya.lastPowerCheckTime != 0 && Energy.active_power[0] > 0) {
            Energy.kWhtoday += (float)Energy.active_power[0] * (Rtc.utc_time - Tuya.lastPowerCheckTime) / 36;
            EnergyUpdateToday();
          }
          Tuya.lastPowerCheckTime = Rtc.utc_time;
        }
  #endif // USE_ENERGY_SENSOR
      }
      else if (Tuya.buffer[dpidStart + 1] == 3) {  // Data Type 3
        const unsigned char *dpData = (unsigned char*)&Tuya.buffer[dpidStart + 4];
        if ((TuyaGetDpId(TUYA_MCU_FUNC_RGB) != 0) && dpDataLen == 12) { //Decode the RGB hex and transmit HSBCOLOR command

          //Tuya.ignore_dim = true;
          char RgbData[13];
          snprintf_P(RgbData, sizeof(RgbData), PSTR("%.*s"), dpDataLen, dpData);
          char HSB1[5], HSB2[5], HSB3[5];
          snprintf_P(HSB1, sizeof(HSB1), PSTR("%.4s\n"), &RgbData[0]);
          snprintf_P(HSB2, sizeof(HSB2), PSTR("%.4s\n"), &RgbData[4]);
          snprintf_P(HSB3, sizeof(HSB3), PSTR("%.4s\n"), &RgbData[8]);

          if ((Tuya.Snapshot[2] != ((int)strtol(HSB1, NULL, 16)) ||
               Tuya.Snapshot[3] != ((int)strtol(HSB2, NULL, 16)) / 10 || Tuya.Snapshot[4] !=((int)strtol(HSB3, NULL, 16)) / 10)) {

            snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_HSBCOLOR " %d,%d,%d"), ((int)strtol(HSB1, NULL, 16)),
                      ((int)strtol(HSB2, NULL, 16)) / 10, ((int)strtol(HSB3, NULL, 16)) / 10);
            ExecuteCommand(scmnd, SRC_SWITCH);

            memcpy_P(Tuya.HSBColor, RgbData, strlen(RgbData));
          }
        }
      }
      else if (Tuya.buffer[dpidStart + 1] == 4) {  // Data Type 4
        const unsigned char *dpData = (unsigned char*)&Tuya.buffer[dpidStart + 4];
        if (fnId == TUYA_MCU_FUNC_MODESET) { // toggle light type
          Tuya.ModeSet = dpData[0];
          Tuya.Levels[3] = dpData[0];
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
  AddLog_P2(LOG_LEVEL_INFO, PSTR("TYA: MCU Product ID: %.*s"), dataLength, data);
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
      AddLog_P(LOG_LEVEL_DEBUG, PSTR("TYA: Heartbeat"));
      if (Tuya.buffer[6] == 0) {
        AddLog_P(LOG_LEVEL_DEBUG, PSTR("TYA: Detected MCU restart"));
        Tuya.wifi_state = -2;
      }
      break;

    case TUYA_CMD_STATE:
      TuyaProcessStatePacket();
      break;

    case TUYA_CMD_WIFI_RESET:
    case TUYA_CMD_WIFI_SELECT:
      AddLog_P(LOG_LEVEL_DEBUG, PSTR("TYA: RX WiFi Reset"));
      TuyaResetWifi();
      break;

    case TUYA_CMD_WIFI_STATE:
      AddLog_P(LOG_LEVEL_DEBUG, PSTR("TYA: RX WiFi LED set ACK"));
      Tuya.wifi_state = TuyaGetTuyaWifiState();
      break;

    case TUYA_CMD_MCU_CONF:
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TYA: RX MCU configuration Mode=%d"), Tuya.buffer[5]);

      if (Tuya.buffer[5] == 2) { // Processing by ESP module mode
        uint8_t led1_gpio = Tuya.buffer[6];
        uint8_t key1_gpio = Tuya.buffer[7];
        bool key1_set = false;
        bool led1_set = false;
        for (uint32_t i = 0; i < ARRAY_SIZE(Settings.my_gp.io); i++) {
          if (Settings.my_gp.io[i] == AGPIO(GPIO_LED1)) led1_set = true;
          else if (Settings.my_gp.io[i] == AGPIO(GPIO_KEY1)) key1_set = true;
        }
        if (!Settings.my_gp.io[led1_gpio] && !led1_set) {
          Settings.my_gp.io[led1_gpio] = AGPIO(GPIO_LED1);
          restart_flag = 2;
        }
        if (!Settings.my_gp.io[key1_gpio] && !key1_set) {
          Settings.my_gp.io[key1_gpio] = AGPIO(GPIO_KEY1);
          restart_flag = 2;
        }
      }
      TuyaRequestState(0);
      break;

    default:
      AddLog_P(LOG_LEVEL_DEBUG, PSTR("TYA: RX unknown command"));
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
    Settings.my_gp.io[1] = AGPIO(GPIO_TUYA_TX);
    Settings.my_gp.io[3] = AGPIO(GPIO_TUYA_RX);
    restart_flag = 2;
  }

  if (TuyaGetDpId(TUYA_MCU_FUNC_DIMMER) == 0 && TUYA_DIMMER_ID > 0) {
    TuyaAddMcuFunc(TUYA_MCU_FUNC_DIMMER, TUYA_DIMMER_ID);
  }

  bool relaySet = false;

  for (uint8_t i = 0 ; i < MAX_TUYA_FUNCTIONS; i++) {
    if ((Settings.tuya_fnid_map[i].fnid >= TUYA_MCU_FUNC_REL1 && Settings.tuya_fnid_map[i].fnid <= TUYA_MCU_FUNC_REL8 ) ||
    (Settings.tuya_fnid_map[i].fnid >= TUYA_MCU_FUNC_REL1_INV && Settings.tuya_fnid_map[i].fnid <= TUYA_MCU_FUNC_REL8_INV )) {
      relaySet = true;
      devices_present++;
    }
  }

  if (!relaySet && TuyaGetDpId(TUYA_MCU_FUNC_DUMMY) == 0) { //by default the first relay is created automatically the dummy let remove it if not needed
    TuyaAddMcuFunc(TUYA_MCU_FUNC_REL1, 1);
    devices_present++;
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
        light_type = LT_RGBWC;
      } else if (TuyaGetDpId(TUYA_MCU_FUNC_WHITE) != 0) {
        light_type = LT_RGBW;
      } else { light_type = LT_RGB; }
    } else if (TuyaGetDpId(TUYA_MCU_FUNC_CT) != 0 || TuyaGetDpId(TUYA_MCU_FUNC_DIMMER2) != 0) {
      if (TuyaGetDpId(TUYA_MCU_FUNC_RGB) != 0) {
        light_type = LT_RGBWC;
      } else { light_type = LT_SERIAL2; }
    } else { light_type = LT_SERIAL1; }
  } else {
    light_type = LT_BASIC;
  }

  if (TuyaGetDpId(TUYA_MCU_FUNC_LOWPOWER_MODE) != 0) {
    Tuya.low_power_mode = true;
    Settings.flag3.fast_power_cycle_disable = true;  // SetOption65 - Disable fast power cycle detection for device reset
  }

  UpdateDevices();
  return true;
}

void TuyaInit(void)
{
  int baudrate = 9600;
  if (Settings.flag4.tuyamcu_baudrate) { baudrate = 115200; }  // SetOption97 - Set Baud rate for TuyaMCU serial communication (0 = 9600 or 1 = 115200)

  Tuya.buffer = (char*)(malloc(TUYA_BUFFER_SIZE));
  if (Tuya.buffer != nullptr) {
    TuyaSerial = new TasmotaSerial(Pin(GPIO_TUYA_RX), Pin(GPIO_TUYA_TX), 2);
    if (TuyaSerial->begin(baudrate)) {
      if (TuyaSerial->hardwareSerial()) { ClaimSerial(); }
      // Get MCU Configuration
      Tuya.SuspendTopic = true;
      Tuya.ignore_topic_timeout = millis() + 1000; // suppress /STAT topic for 1000ms to avoid data overflow
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TYA: Request MCU configuration at %d bps"), baudrate);
      TuyaSendCmd(TUYA_CMD_QUERY_PRODUCT);

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
      char DataStr[13];

      if (len > 0) {
        ResponseAppend_P(PSTR(",\"CmndData\":\"%s\""), ToHex_P((unsigned char*)&Tuya.buffer[6], len, hex_char, sizeof(hex_char)));
        if (TUYA_CMD_STATE == Tuya.buffer[3]) {
          //55 AA 03 07 00 0D 01 04 00 01 02 02 02 00 04 00 00 00 1A 40
          // 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19
          uint8_t dpidStart = 6;
          //snprintf_P(DataStr, sizeof(DataStr), PSTR("000000000000"));
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

      if (Settings.flag3.tuya_serial_mqtt_publish) {  // SetOption66 - Enable TuyaMcuReceived messages over Mqtt
        MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_TUYA_MCU_RECEIVED));
      } else {
        AddLog_P(LOG_LEVEL_DEBUG, mqtt_data);
      }
      XdrvRulesProcess();

      if (dpId != 0 && Settings.tuyamcu_topic) { // Publish a /STAT Topic ready to use for any home automation system
        if (!Tuya.SuspendTopic) {
          char scommand[10];
          snprintf_P(scommand, sizeof(scommand), PSTR("TuyaSend%d"), dpDataType);

          if (dpDataType != 3 && dpDataType != 5) {  Response_P(PSTR("%d,%u"), dpId, DataVal); }
          else { Response_P(PSTR("%d,%s"), dpId, DataStr); }
          MqttPublishPrefixTopic_P(STAT, (PSTR("%s"), scommand));
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
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("TYA: Reset GPIO triggered"));
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
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TYA: Set WiFi LED %d (%d)"), Tuya.wifi_state, WifiState());

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
  payload_buffer[0] = 0x01;
  payload_buffer[1] = RtcTime.year %100;
  payload_buffer[2] = RtcTime.month;
  payload_buffer[3] = RtcTime.day_of_month;
  payload_buffer[4] = RtcTime.hour;
  payload_buffer[5] = RtcTime.minute;
  payload_buffer[6] = RtcTime.second;
  payload_buffer[7] = RtcTime.day_of_week;

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

  if (TUYA_DIMMER == my_module_type) {
    if (FUNC_PRE_INIT == function) {
      if (TuyaGetDpId(TUYA_MCU_FUNC_POWER) != 0) {
        if (TuyaGetDpId(TUYA_MCU_FUNC_CURRENT) == 0) {
          Energy.current_available = false;
        }
        if (TuyaGetDpId(TUYA_MCU_FUNC_VOLTAGE) == 0) {
          Energy.voltage_available = false;
        }
        energy_flg = XNRG_32;
      }
    }
  }
  return result;
}
#endif  // USE_ENERGY_SENSOR

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv16(uint8_t function)
{
  bool result = false;

  if (TUYA_DIMMER == my_module_type) {
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
        TuyaSetChannels();
        if (TuyaSerial && Tuya.wifi_state != TuyaGetTuyaWifiState()) { TuyaSetWifiLed(); }
        if (!Tuya.low_power_mode) {
          Tuya.heartbeat_timer++;
          if (Tuya.heartbeat_timer > 10) {
            Tuya.heartbeat_timer = 0;
            TuyaSendCmd(TUYA_CMD_HEARTBEAT);
          }
#ifdef USE_TUYA_TIME
          if (!(uptime % 60)) {
            TuyaSetTime();
          }
#endif  //USE_TUYA_TIME
        } else {
          TuyaSendLowPowerSuccessIfNeeded();
        }
        if (Tuya.ignore_topic_timeout < millis()) { Tuya.SuspendTopic = false; }
        break;
      // case FUNC_SET_CHANNELS:
      //   result = TuyaSetChannels();
      //   break;
      case FUNC_COMMAND:
        result = DecodeCommand(kTuyaCommand, TuyaCommand);
        break;
    }
  }
  return result;
}

#endif  // USE_TUYA_MCU
#endif  // USE_LIGHT
