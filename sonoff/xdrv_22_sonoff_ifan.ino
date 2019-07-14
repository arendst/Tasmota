/*
  xdrv_22_sonoff_ifan.ino - sonoff iFan02 and iFan03 support for Sonoff-Tasmota

  Copyright (C) 2019  Theo Arends

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

/*********************************************************************************************\
  Sonoff iFan02 and iFan03
\*********************************************************************************************/

#define XDRV_22                   22

const uint8_t MAX_FAN_SPEED = 4;            // Max number of iFan02 fan speeds (0 .. 3)

const uint8_t kIFan02Speed[MAX_FAN_SPEED] = { 0x00, 0x01, 0x03, 0x05 };
const uint8_t kIFan03Speed[MAX_FAN_SPEED +2] = { 0x00, 0x01, 0x03, 0x04, 0x05, 0x06 };
const uint8_t kIFan03Sequence[MAX_FAN_SPEED][MAX_FAN_SPEED] = {{0, 2, 2, 2}, {0, 1, 2, 4}, {1, 1, 2, 5}, {4, 4, 5, 3}};

uint8_t ifan_fanspeed_timer = 0;
uint8_t ifan_fanspeed_goal = 0;
bool ifan_receive_flag = false;

/*********************************************************************************************/

bool IsModuleIfan()
{
  return ((SONOFF_IFAN02 == my_module_type) || (SONOFF_IFAN03 == my_module_type));
}

uint8_t MaxFanspeed(void)
{
  return MAX_FAN_SPEED;
}

uint8_t GetFanspeed(void)
{
  if (ifan_fanspeed_timer) {
    return ifan_fanspeed_goal;
  } else {
    uint8_t fanspeed = 0;

//    if (SONOFF_IFAN02 == my_module_type) {
      /* Fanspeed is controlled by relay 2, 3 and 4 as in Sonoff 4CH.
        000x = 0
        001x = 1
        011x = 2
        101x = 3 (ifan02) or 100x = 3 (ifan03)
      */
      fanspeed = (uint8_t)(power &0xF) >> 1;
      if (fanspeed) { fanspeed = (fanspeed >> 1) +1; }  // 0, 1, 2, 3
//    }
    return fanspeed;
  }
}

/*********************************************************************************************/

void SonoffIFanSetFanspeed(uint8_t fanspeed, bool sequence)
{
  ifan_fanspeed_timer = 0;                         // Stop any sequence
  ifan_fanspeed_goal = fanspeed;

  uint8_t fanspeed_now = GetFanspeed();

  if (fanspeed == fanspeed_now) { return; }

  uint8_t fans = kIFan02Speed[fanspeed];
  if (SONOFF_IFAN03 == my_module_type) {
    if (sequence) {
      fanspeed = kIFan03Sequence[fanspeed_now][ifan_fanspeed_goal];
      if (fanspeed != ifan_fanspeed_goal) {
        if (0 == fanspeed_now) {
          ifan_fanspeed_timer = 20;                // Need extra time to power up fan
        } else {
          ifan_fanspeed_timer = 2;
        }
      }
    }
    fans = kIFan03Speed[fanspeed];
  }
  for (uint32_t i = 2; i < 5; i++) {
    uint8_t state = (fans &1) + 6;                 // Add no publishPowerState
    ExecuteCommandPower(i, state, SRC_IGNORE);  // Use relay 2, 3 and 4
    fans >>= 1;
  }

#ifdef USE_DOMOTICZ
  if (sequence) { DomoticzUpdateFanState(); }      // Command FanSpeed feedback
#endif  // USE_DOMOTICZ
}

/*********************************************************************************************/

void SonoffIfanSendAck()
{
  // AA 55 01 04 00 00 05
  serial_in_buffer[5] = 0;
  uint8_t crc = 0;
  for (uint32_t i = 2; i < 5; i++) {
    crc += serial_in_buffer[i];
  }
  serial_in_buffer[6] = crc;
  for (uint32_t i = 0; i < 7; i++) {
    Serial.write(serial_in_buffer[i]);
  }
}

void SonoffIfanReceived(void)
{
  char svalue[32];

  uint8_t mode = serial_in_buffer[3];
  uint8_t action = serial_in_buffer[6];

  if (4 == mode) {
    if (action < 4) {
      // AA 55 01 04 00 01 00 06 - Fan 0
      // AA 55 01 04 00 01 01 07 - Fan 1
      // AA 55 01 04 00 01 02 08 - Fan 2
      // AA 55 01 04 00 01 03 09 - Fan 3
      if (action != GetFanspeed()) {
        snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_FANSPEED " %d"), action);
        ExecuteCommand(svalue, SRC_REMOTE);
        buzzer_count = 2;                        // Beep once
      }
    } else {
      // AA 55 01 04 00 01 04 0A - Light
      ExecuteCommandPower(1, POWER_TOGGLE, SRC_REMOTE);
    }
  }
  if (6 == mode) {
    // AA 55 01 06 00 01 01 09 - Buzzer
    Settings.flag3.buzzer_enable = !Settings.flag3.buzzer_enable;  // SetOption67
  }
  if (7 == mode) {
    // AA 55 01 07 00 01 01 0A - Rf long press - forget RF codes
    buzzer_count = 6;                           // Beep three times
  }
  SonoffIfanSendAck();
}

bool SonoffIfanSerialInput(void)
{
  if (SONOFF_IFAN03 == my_module_type) {
    if (0xAA == serial_in_byte) {               // 0xAA - Start of text
      serial_in_byte_counter = 0;
      ifan_receive_flag = true;
    }
    if (ifan_receive_flag) {
      // AA 55 01 01 00 01 01 04 - Wifi long press - start wifi setup
      // AA 55 01 01 00 01 02 05 - Rf and Wifi short press
      // AA 55 01 04 00 01 00 06 - Fan 0
      // AA 55 01 04 00 01 01 07 - Fan 1
      // AA 55 01 04 00 01 02 08 - Fan 2
      // AA 55 01 04 00 01 03 09 - Fan 3
      // AA 55 01 04 00 01 04 0A - Light
      // AA 55 01 06 00 01 01 09 - Buzzer
      // AA 55 01 07 00 01 01 0A - Rf long press - forget RF codes
      serial_in_buffer[serial_in_byte_counter++] = serial_in_byte;
      if (serial_in_byte_counter == 8) {

        AddLogSerial(LOG_LEVEL_DEBUG);

        uint8_t crc = 0;
        for (uint32_t i = 2; i < 7; i++) {
          crc += serial_in_buffer[i];
        }
        if (crc == serial_in_buffer[7]) {
          SonoffIfanReceived();
          ifan_receive_flag = false;
          return true;
        }
      }
      serial_in_byte = 0;
    }
    return false;
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

enum SonoffIfanCommands { CMND_FANSPEED };
const char kSonoffIfanCommands[] PROGMEM = D_CMND_FANSPEED;

bool SonoffIfanCommand(void)
{
  char command [CMDSZ];
  bool serviced = true;

  int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, kSonoffIfanCommands);
  if (-1 == command_code) {
    serviced = false;  // Unknown command
  }
  else if (CMND_FANSPEED == command_code) {
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
    Response_P(S_JSON_COMMAND_NVALUE, command, GetFanspeed());
  } else serviced = false;  // Unknown command

  return serviced;
}

/*********************************************************************************************/

bool SonoffIfanInit(void)
{
  if (SONOFF_IFAN03 == my_module_type) {
    Settings.flag.mqtt_serial = 0;
    baudrate = 9600;
    SetSeriallog(LOG_LEVEL_NONE);
  }
  return false;  // Continue init chain
}

void SonoffIfanUpdate(void)
{
  if (SONOFF_IFAN03 == my_module_type) {
    if (ifan_fanspeed_timer) {
      ifan_fanspeed_timer--;
      if (!ifan_fanspeed_timer) {
        SonoffIFanSetFanspeed(ifan_fanspeed_goal, false);
      }
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv22(uint8_t function)
{
  bool result = false;

  if (IsModuleIfan()) {
    switch (function) {
      case FUNC_MODULE_INIT:
        result = SonoffIfanInit();
        break;
      case FUNC_EVERY_250_MSECOND:
        SonoffIfanUpdate();
        break;
      case FUNC_COMMAND:
        result = SonoffIfanCommand();
        break;
      case FUNC_SERIAL:
        result = SonoffIfanSerialInput();
        break;
    }
  }
  return result;
}
