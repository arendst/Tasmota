/*
  xdrv_16_tuyamcu.ino - Tuya MCU support for Tasmota

  Copyright (C) 2020  digiblur, Joel Stein and Theo Arends

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
#define XNRG_16                16   // Needs to be the last XNRG_xx

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
  uint16_t new_dim = 0;                   // Tuya dimmer value temp
  bool ignore_dim = false;               // Flag to skip serial send to prevent looping when processing inbound states from the faceplate interaction
  uint8_t cmd_status = 0;                // Current status of serial-read
  uint8_t cmd_checksum = 0;              // Checksum of tuya command
  uint8_t data_len = 0;                  // Data lenght of command
  uint8_t wifi_state = -2;                // Keep MCU wifi-status in sync with WifiState()
  uint8_t heartbeat_timer = 0;           // 10 second heartbeat timer for tuya module
#ifdef USE_ENERGY_SENSOR
  uint32_t lastPowerCheckTime = 0;       // Time when last power was checked
#endif // USE_ENERGY_SENSOR
  char *buffer = nullptr;                // Serial receive buffer
  int byte_counter = 0;                  // Index in serial receive buffer
  bool low_power_mode = false;           // Normal or Low power mode protocol
  bool send_success_next_second = false; // Second command success in low power mode
} Tuya;


enum TuyaSupportedFunctions {
  TUYA_MCU_FUNC_NONE,
  TUYA_MCU_FUNC_SWT1 = 1,           // Buttons
  TUYA_MCU_FUNC_SWT2,
  TUYA_MCU_FUNC_SWT3,
  TUYA_MCU_FUNC_SWT4,
  TUYA_MCU_FUNC_REL1 = 11,           // Relays
  TUYA_MCU_FUNC_REL2,
  TUYA_MCU_FUNC_REL3,
  TUYA_MCU_FUNC_REL4,
  TUYA_MCU_FUNC_REL5,
  TUYA_MCU_FUNC_REL6,
  TUYA_MCU_FUNC_REL7,
  TUYA_MCU_FUNC_REL8,
  TUYA_MCU_FUNC_DIMMER = 21,
  TUYA_MCU_FUNC_POWER = 31,
  TUYA_MCU_FUNC_CURRENT,
  TUYA_MCU_FUNC_VOLTAGE,
  TUYA_MCU_FUNC_BATTERY_STATE,
  TUYA_MCU_FUNC_BATTERY_PERCENTAGE,
  TUYA_MCU_FUNC_REL1_INV = 41,           // Inverted Relays
  TUYA_MCU_FUNC_REL2_INV,
  TUYA_MCU_FUNC_REL3_INV,
  TUYA_MCU_FUNC_REL4_INV,
  TUYA_MCU_FUNC_REL5_INV,
  TUYA_MCU_FUNC_REL6_INV,
  TUYA_MCU_FUNC_REL7_INV,
  TUYA_MCU_FUNC_REL8_INV,
  TUYA_MCU_FUNC_LOWPOWER_MODE = 51,
  TUYA_MCU_FUNC_LAST = 255
};

const char kTuyaCommand[] PROGMEM = "|"  // No prefix
  D_CMND_TUYA_MCU "|" D_CMND_TUYA_MCU_SEND_STATE;

void (* const TuyaCommand[])(void) PROGMEM = {
  &CmndTuyaMcu, &CmndTuyaSend
};

/*

TuyaSend<x> dpId,data

TuyaSend1 11,1 -> Sends boolean (Type 1) data 0/1 to dpId 11 (Max data length 1 byte)
TuyaSend2 11,100 -> Sends integer (Type 2) data 100 to dpId 11 (Max data length 4 bytes)
TuyaSend2 11,0xAABBCCDD -> Sends 4 bytes (Type 2) data to dpId 11 (Max data length 4 bytes)
TuyaSend3 11,ThisIsTheData -> Sends the supplied string (Type 3) to dpId 11 ( Max data length not-known)
TuyaSend4 11,1 -> Sends enum (Type 4) data 0/1/2/3/4/5 to dpId 11 (Max data length 1 bytes)

*/


void CmndTuyaSend(void) {
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 4)) {
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
    ResponseCmndDone();
  }
}

/*

TuyaMcu fnid,dpid

*/

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
    fnId == TUYA_MCU_FUNC_DIMMER ||
    (fnId >= TUYA_MCU_FUNC_POWER && fnId <= TUYA_MCU_FUNC_VOLTAGE) ||
    (fnId >= TUYA_MCU_FUNC_REL1_INV && fnId <= TUYA_MCU_FUNC_REL8_INV) ||
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
    status = true;
  }
  return status;
}

bool TuyaSetChannels(void)
{
  LightSerialDuty(((uint8_t*)XdrvMailbox.data)[0]);
  delay(20); // Hack when power is off and dimmer is set then both commands go too soon to Serial out.
  return true;
}

void LightSerialDuty(uint16_t duty)
{
  uint8_t dpid = TuyaGetDpId(TUYA_MCU_FUNC_DIMMER);
  if (duty > 0 && !Tuya.ignore_dim && TuyaSerial && dpid > 0) {
    duty = changeUIntScale(duty, 0, 255, 0, Settings.dimmer_hw_max);
    if (duty < Settings.dimmer_hw_min) { duty = Settings.dimmer_hw_min; }  // dimming acts odd below 25(10%) - this mirrors the threshold set on the faceplate itself
    if (Tuya.new_dim != duty) {
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TYA: Send dim value=%d (id=%d)"), duty, dpid);
      TuyaSendValue(dpid, duty);
    }
  } else if (dpid > 0) {
    Tuya.ignore_dim = false;  // reset flag
    duty = changeUIntScale(duty, 0, 255, 0, Settings.dimmer_hw_max);
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TYA: Send dim skipped value=%d"), duty);  // due to 0 or already set
  } else {
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("TYA: Cannot set dimmer. Dimmer Id unknown"));  //
  }
}

void TuyaRequestState(void)
{
  if (TuyaSerial) {
    // Get current status of MCU
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("TYA: Read MCU state"));

    TuyaSendCmd(TUYA_CMD_QUERY_STATE);
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
        bool tuya_energy_enabled = (XNRG_16 == energy_flg);
        uint16_t packetValue = Tuya.buffer[dpidStart + 6] << 8 | Tuya.buffer[dpidStart + 7];
        if (fnId == TUYA_MCU_FUNC_DIMMER) {
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TYA: RX Dim State=%d"), packetValue);
          Tuya.new_dim = changeUIntScale(packetValue, 0, Settings.dimmer_hw_max, 0, 100);
          if ((power || Settings.flag3.tuya_apply_o20) &&  // SetOption54 - Apply SetOption20 settings to Tuya device
              (Tuya.new_dim > 0) && (abs(Tuya.new_dim - Settings.light_dimmer) > 1)) {
            Tuya.ignore_dim = true;

            snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_DIMMER " %d"), Tuya.new_dim );
            ExecuteCommand(scmnd, SRC_SWITCH);
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
    // } else {
    //   AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TYA: Unknown FnId=%s for dpId=%s"), fnId, Tuya.buffer[6]);
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
        for (uint32_t i = 0; i < sizeof(Settings.my_gp); i++) {
          if (Settings.my_gp.io[i] == GPIO_LED1) led1_set = true;
          else if (Settings.my_gp.io[i] == GPIO_KEY1) key1_set = true;
        }
        if (!Settings.my_gp.io[led1_gpio] && !led1_set) {
          Settings.my_gp.io[led1_gpio] = GPIO_LED1;
          restart_flag = 2;
        }
        if (!Settings.my_gp.io[key1_gpio] && !key1_set) {
          Settings.my_gp.io[key1_gpio] = GPIO_KEY1;
          restart_flag = 2;
        }
      }
      TuyaRequestState();
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
  if (!(pin[GPIO_TUYA_RX] < 99) || !(pin[GPIO_TUYA_TX] < 99)) {  // fallback to hardware-serial if not explicitly selected
    pin[GPIO_TUYA_TX] = 1;
    pin[GPIO_TUYA_RX] = 3;
    Settings.my_gp.io[1] = GPIO_TUYA_TX;
    Settings.my_gp.io[3] = GPIO_TUYA_RX;
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

  if (!relaySet) {
    TuyaAddMcuFunc(TUYA_MCU_FUNC_REL1, 1);
    devices_present++;
    SettingsSaveAll();
  }

  if (TuyaGetDpId(TUYA_MCU_FUNC_DIMMER) != 0) {
    light_type = LT_SERIAL1;
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
  Tuya.buffer = (char*)(malloc(TUYA_BUFFER_SIZE));
  if (Tuya.buffer != nullptr) {
    TuyaSerial = new TasmotaSerial(pin[GPIO_TUYA_RX], pin[GPIO_TUYA_TX], 2);
    if (TuyaSerial->begin(9600)) {
      if (TuyaSerial->hardwareSerial()) { ClaimSerial(); }
      // Get MCU Configuration
      AddLog_P(LOG_LEVEL_DEBUG, PSTR("TYA: Request MCU configuration"));

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

      if (len > 0) {
        ResponseAppend_P(PSTR(",\"CmndData\":\"%s\""), ToHex_P((unsigned char*)&Tuya.buffer[6], len, hex_char, sizeof(hex_char)));
        if (TUYA_CMD_STATE == Tuya.buffer[3]) {
          //55 AA 03 07 00 0D 01 04 00 01 02 02 02 00 04 00 00 00 1A 40
          // 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19
          uint8_t dpidStart = 6;
          while (dpidStart + 4 < Tuya.byte_counter) {
            uint16_t dpDataLen = Tuya.buffer[dpidStart + 2] << 8 | Tuya.buffer[dpidStart + 3];
            ResponseAppend_P(PSTR(",\"%d\":{\"DpId\":%d,\"DpIdType\":%d,\"DpIdData\":\"%s\""), Tuya.buffer[dpidStart], Tuya.buffer[dpidStart], Tuya.buffer[dpidStart + 1], ToHex_P((unsigned char*)&Tuya.buffer[dpidStart + 4], dpDataLen, hex_char, sizeof(hex_char)));
            if (TUYA_TYPE_STRING == Tuya.buffer[dpidStart + 1]) {
              ResponseAppend_P(PSTR(",\"Type3Data\":\"%.*s\""), dpDataLen, (char *)&Tuya.buffer[dpidStart + 4]);
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

      if (!Tuya.low_power_mode) {
        TuyaNormalPowerModePacketProcess();
      } else {
        TuyaLowPowerModePacketProcess();
      }

      Tuya.byte_counter = 0;
      Tuya.cmd_status = 0;
      Tuya.cmd_checksum = 0;
      Tuya.data_len = 0;
    }                               // read additional packets from TUYA
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

#ifdef USE_ENERGY_SENSOR
/*********************************************************************************************\
 * Energy Interface
\*********************************************************************************************/

bool Xnrg16(uint8_t function)
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
        energy_flg = XNRG_16;
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
        if (TuyaSerial && Tuya.wifi_state != TuyaGetTuyaWifiState()) { TuyaSetWifiLed(); }
        if (!Tuya.low_power_mode) {
          Tuya.heartbeat_timer++;
          if (Tuya.heartbeat_timer > 10) {
            Tuya.heartbeat_timer = 0;
            TuyaSendCmd(TUYA_CMD_HEARTBEAT);
          }
        } else {
            TuyaSendLowPowerSuccessIfNeeded();
        }
        break;
      case FUNC_SET_CHANNELS:
        result = TuyaSetChannels();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kTuyaCommand, TuyaCommand);
        break;
    }
  }
  return result;
}

#endif  // USE_TUYA_MCU
#endif  // USE_LIGHT
