/*
  xdrv_22_sonoff_ifan.ino - sonoff iFan02, iFan03 and iFan04 support for Tasmota

  Copyright (C) 2021  Theo Arends

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

#ifdef USE_SONOFF_IFAN
/*********************************************************************************************\
 * Sonoff iFan02, iFan03 and iFan04
 * 
 * For iFan04-H activate below template:
 * {"NAME":"Sonoff iFan04-H","GPIO":[32,3200,5735,3232,0,0,256,512,226,320,225,227,0,0],"FLAG":0,"BASE":71}
 * 
 * #ifdef USE_SONOFF_IFAN_CONFIG
 * No template change needed. For iFan04-H select
 * - a rule like: rule3 on file#ifan.dat do {"Name":"iFan04-H","Speed":[0,1,2,4,5,6],"Sequence":[[0,4,5,3],[0,1,2,3],[0,1,2,3],[0,4,5,3]]} endon
 * - a script like: -y{"Name":"iFan04-H","Speed":[0,1,2,4,5,6],"Sequence":[[0,4,5,3],[0,1,2,3],[0,1,2,3],[0,4,5,3]]}#
 * - a file called ifan.dat with contents: {"Name":"iFan04-H","Speed":[0,1,2,4,5,6],"Sequence":[[0,4,5,3],[0,1,2,3],[0,1,2,3],[0,4,5,3]]}
 * 
 * For reference only:
 * rule3 on file#ifan.dat do {"Name":"iFan03","Speed":[0,1,3,4,5,6],"Sequence":[[0,2,2,2],[0,1,2,4],[1,1,2,5],[4,4,5,3]]} endon
\*********************************************************************************************/

#define XDRV_22                   22

//#define USE_SONOFF_IFAN_CONFIG              // Enable ifan user config

const uint8_t MAX_FAN_SPEED = 4;            // Max number of iFan02 fan speeds (0 .. 3)

const uint8_t kIFan02Speed[MAX_FAN_SPEED] = { 0x00, 0x01, 0x03, 0x05 };
const uint8_t kIFan04Speed[MAX_FAN_SPEED +2] = { 0x00, 0x01, 0x02, 0x04, 0x05, 0x06 };
const uint8_t kIFan04Sequence[MAX_FAN_SPEED][MAX_FAN_SPEED] = {{0, 4, 5, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 4, 5, 3}};

const char kSonoffIfanCommands[] PROGMEM = "|"  // No prefix
  D_CMND_FANSPEED;

void (* const SonoffIfanCommand[])(void) PROGMEM = {
  &CmndFanspeed };

struct IFAN {
  uint8_t speed[MAX_FAN_SPEED +2] = { 0x00, 0x01, 0x03, 0x04, 0x05, 0x06 };
  uint8_t sequence[MAX_FAN_SPEED][MAX_FAN_SPEED] = {{0, 2, 2, 2}, {0, 1, 2, 4}, {1, 1, 2, 5}, {4, 4, 5, 3}};
  uint8_t fanspeed_timer = 0;
  uint8_t fanspeed_goal = 0;
  bool receive_flag = false;
  bool restart_flag = true;
} Ifan;

/*********************************************************************************************/

bool IsModuleIfan(void) {
  return ((SONOFF_IFAN02 == TasmotaGlobal.module_type) || (SONOFF_IFAN03 == TasmotaGlobal.module_type));
}

uint8_t MaxFanspeed(void) {
  return MAX_FAN_SPEED;
}

uint8_t GetFanspeed(void) {
  if (Ifan.fanspeed_timer) {
    return Ifan.fanspeed_goal;                     // Do not show sequence fanspeed
  } else {
    /* Fanspeed is controlled by relay 2, 3 and 4 as in Sonoff 4CH.
      000x = 0
      001x = 1
      011x = 2
      101x = 3 (ifan02) or 100x = 3 (ifan03)
    */
    uint8_t fanspeed = (uint8_t)(TasmotaGlobal.power &0xF) >> 1;
    if (fanspeed) { fanspeed = (fanspeed >> 1) +1; }  // 0, 1, 2, 3
    return fanspeed;
  }
}

/*********************************************************************************************/

void SonoffIFanSetFanspeed(uint8_t fanspeed, bool sequence) {
  Ifan.fanspeed_timer = 0;                         // Stop any sequence
  Ifan.fanspeed_goal = fanspeed;

  uint8_t fanspeed_now = GetFanspeed();
  if (fanspeed == fanspeed_now) { return; }

  uint8_t fans = kIFan02Speed[fanspeed];
  if (SONOFF_IFAN03 == TasmotaGlobal.module_type) {
    if (sequence) {
      fanspeed = Ifan.sequence[fanspeed_now][Ifan.fanspeed_goal];
      if (fanspeed != Ifan.fanspeed_goal) {
        if (0 == fanspeed_now) {
          Ifan.fanspeed_timer = 20;                // Need extra time to power up fan
        } else {
          Ifan.fanspeed_timer = 2;
        }
      }
    }
    fans = Ifan.speed[fanspeed];
  }
  for (uint32_t i = 2; i < 5; i++) {
    uint8_t state = (fans &1) + POWER_OFF_NO_STATE;  // Add no publishPowerState
    ExecuteCommandPower(i, state, SRC_IGNORE);     // Use relay 2, 3 and 4
    fans >>= 1;
  }

#ifdef USE_DOMOTICZ
  if (sequence) { DomoticzUpdateFanState(); }      // Command FanSpeed feedback
#endif  // USE_DOMOTICZ
}

/*********************************************************************************************/

void SonoffIfanReceived(void) {
  char svalue[32];

  uint8_t mode = TasmotaGlobal.serial_in_buffer[3];
  uint8_t action = TasmotaGlobal.serial_in_buffer[6];

  if (4 == mode) {
    if (action < 4) {
      // AA 55 01 04 00 01 00 06 - Fan 0
      // AA 55 01 04 00 01 01 07 - Fan 1
      // AA 55 01 04 00 01 02 08 - Fan 2
      // AA 55 01 04 00 01 03 09 - Fan 3
      if (action != GetFanspeed()) {
        snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_FANSPEED " %d"), action);
        ExecuteCommand(svalue, SRC_REMOTE);
#ifdef USE_BUZZER
        BuzzerEnabledBeep((action) ? action : 1, (action) ? 1 : 4);  // Beep action times
#endif
      }
    } else {
      // AA 55 01 04 00 01 04 0A - Light
      ExecuteCommandPower(1, POWER_TOGGLE, SRC_REMOTE);
    }
  }
  if (6 == mode) {
    // AA 55 01 06 00 01 01 09 - Buzzer
    Settings->flag3.buzzer_enable = !Settings->flag3.buzzer_enable;  // SetOption67 - Enable buzzer when available
  }
  if (7 == mode) {
    // AA 55 01 07 00 01 01 0A - Rf long press - forget RF codes
#ifdef USE_BUZZER
    BuzzerEnabledBeep(4, 1);                       // Beep four times
#endif
  }

  // Send Acknowledge - Copy first 5 bytes, reset byte 6 and store crc in byte 7
  // AA 55 01 04 00 00 05
  TasmotaGlobal.serial_in_buffer[5] = 0;                      // Ack
  TasmotaGlobal.serial_in_buffer[6] = 0;                      // Crc
  for (uint32_t i = 0; i < 7; i++) {
    if ((i > 1) && (i < 6)) { TasmotaGlobal.serial_in_buffer[6] += TasmotaGlobal.serial_in_buffer[i]; }
    Serial.write(TasmotaGlobal.serial_in_buffer[i]);
  }
}

bool SonoffIfanSerialInput(void) {
  if (SONOFF_IFAN03 != TasmotaGlobal.module_type) { return false; }

  if (0xAA == TasmotaGlobal.serial_in_byte) {               // 0xAA - Start of text
    TasmotaGlobal.serial_in_byte_counter = 0;
    Ifan.receive_flag = true;
  }
  if (Ifan.receive_flag) {
    TasmotaGlobal.serial_in_buffer[TasmotaGlobal.serial_in_byte_counter++] = TasmotaGlobal.serial_in_byte;
    if (TasmotaGlobal.serial_in_byte_counter == 8) {
      // AA 55 01 01 00 01 01 04 - Wifi long press - start wifi setup
      // AA 55 01 01 00 01 02 05 - Rf and Wifi short press
      // AA 55 01 04 00 01 00 06 - Fan 0
      // AA 55 01 04 00 01 01 07 - Fan 1
      // AA 55 01 04 00 01 02 08 - Fan 2
      // AA 55 01 04 00 01 03 09 - Fan 3
      // AA 55 01 04 00 01 04 0A - Light
      // AA 55 01 06 00 01 01 09 - Buzzer
      // AA 55 01 07 00 01 01 0A - Rf long press - forget RF codes
      AddLogSerial();
      uint8_t crc = 0;
      for (uint32_t i = 2; i < 7; i++) {
        crc += TasmotaGlobal.serial_in_buffer[i];
      }
      if (crc == TasmotaGlobal.serial_in_buffer[7]) {
        SonoffIfanReceived();
        Ifan.receive_flag = false;
        return true;
      }
    }
    TasmotaGlobal.serial_in_byte = 0;
  }
  return false;
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndFanspeed(void) {
  if (XdrvMailbox.data_len > 0) {
    if ('-' == XdrvMailbox.data[0]) {
      XdrvMailbox.payload = (int16_t)GetFanspeed() -1;
      if (XdrvMailbox.payload < 0) { XdrvMailbox.payload = MAX_FAN_SPEED -1; }
    }
    else if ('+' == XdrvMailbox.data[0]) {
      XdrvMailbox.payload = GetFanspeed() +1;
      if (XdrvMailbox.payload > MAX_FAN_SPEED -1) { XdrvMailbox.payload = 0; }
    }
  }
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < MAX_FAN_SPEED)) {
    SonoffIFanSetFanspeed(XdrvMailbox.payload, true);
  }
  ResponseCmndNumber(GetFanspeed());
}

/*********************************************************************************************/

void SonoffIFanParameters(void) {
  if (TasmotaGlobal.gpio_optiona.ifan04_h) {     // GPIO Option_A8
    memcpy(Ifan.speed, kIFan04Speed, sizeof(Ifan.speed));
    memcpy(Ifan.sequence, kIFan04Sequence, sizeof(Ifan.sequence));
  }

#ifdef USE_SONOFF_IFAN_CONFIG

  String ifantmplt = "";
#ifdef USE_UFILESYS
  ifantmplt = TfsLoadString("/ifan.dat");
#endif  // USE_UFILESYS
#ifdef USE_RULES
  if (!ifantmplt.length()) {
    ifantmplt = RuleLoadFile("IFAN.DAT");
  }
#endif  // USE_RULES
#ifdef USE_SCRIPT
  if (!ifantmplt.length()) {
    ifantmplt = ScriptLoadSection(">y");
  }
#endif  // USE_SCRIPT
  if (ifantmplt.length() > 75) {
    JsonParser parser((char*)ifantmplt.c_str());
    JsonParserObject root = parser.getRootObject();
    if (root) { 
      // rule3 on file#ifan.dat do {"Name":"iFan03","Speed":[0,1,3,4,5,6],"Sequence":[[0,2,2,2],[0,1,2,4],[1,1,2,5],[4,4,5,3]]} endon
      // rule3 on file#ifan.dat do {"Name":"iFan04-H","Speed":[0,1,2,4,5,6],"Sequence":[[0,4,5,3],[0,1,2,3],[0,1,2,3],[0,4,5,3]]} endon
      JsonParserArray arr_speed = root[PSTR("SPEED")];
      if (arr_speed) {
        for (uint32_t i = 0; i < sizeof(Ifan.speed); i++) {
          JsonParserToken val = arr_speed[i];
          if (!val) { break; }
          Ifan.speed[i] = val.getUInt() & 0x07;
        }
      }
      JsonParserArray arr_sequence = root[PSTR("SEQUENCE")];
      if (arr_sequence) {
        for (uint32_t j = 0; j < MAX_FAN_SPEED; j++) {
          JsonParserArray arr_sequence2 = arr_sequence[j];
          if (!arr_sequence2) { break; }
          for (uint32_t i = 0; i < MAX_FAN_SPEED; i++) {
            JsonParserToken val = arr_sequence2[i];
            if (!val) { break; }
            Ifan.sequence[j][i] = val.getUInt() & 0x07;
          }
        }
      }
      JsonParserToken val = root[PSTR(D_JSON_NAME)];
      AddLog(LOG_LEVEL_DEBUG, PSTR("IFN: Loaded%s%s"), (val)?" ":"", (val)?val.getStr():"");
    }
  }

#endif  // USE_SONOFF_IFAN_CONFIG

//  AddLog(LOG_LEVEL_DEBUG, PSTR("IFN: Speed %6_H, Sequence %16_H"), Ifan.speed, Ifan.sequence);
}

bool SonoffIfanInit(void) {
  if (SONOFF_IFAN03 == TasmotaGlobal.module_type) {
    SetSerial(9600, TS_SERIAL_8N1);
    SonoffIFanParameters();
  }
  return false;  // Continue init chain
}

void SonoffIfanUpdate(void) {
  if (SONOFF_IFAN03 == TasmotaGlobal.module_type) {
    if (Ifan.fanspeed_timer) {
      Ifan.fanspeed_timer--;
      if (!Ifan.fanspeed_timer) {
        SonoffIFanSetFanspeed(Ifan.fanspeed_goal, false);
      }
    }
  }

  if (Ifan.restart_flag && (4 == TasmotaGlobal.uptime) && (SONOFF_IFAN02 == TasmotaGlobal.module_type)) {  // Microcontroller needs 3 seconds before accepting commands
    Ifan.restart_flag = false;
    SetDevicePower(1, SRC_RETRY);      // Sync with default power on state microcontroller being Light ON and Fan OFF
    SetDevicePower(TasmotaGlobal.power, SRC_RETRY);  // Set required power on state
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv22(uint32_t function) {
  bool result = false;

  if (IsModuleIfan()) {
    switch (function) {
      case FUNC_EVERY_250_MSECOND:
        SonoffIfanUpdate();
        break;
      case FUNC_SERIAL:
        result = SonoffIfanSerialInput();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kSonoffIfanCommands, SonoffIfanCommand);
        break;
      case FUNC_MODULE_INIT:
        result = SonoffIfanInit();
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif  // USE_SONOFF_IFAN
