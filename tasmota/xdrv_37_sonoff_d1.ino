/*
  xdrv_37_sonoff_d1.ino - sonoff D1 dimmer support for Tasmota

  Copyright (C) 2020  Theo Arends

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

#ifdef USE_SONOFF_D1
/*********************************************************************************************\
 * Sonoff D1 dimmer 433
 *
 *  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F 10
 * AA 55                                              - Header
 *       01 04                                        - Version?
 *             00 0A                                  - Following data length (10 bytes)
 *                   01                               - Power state (00 = off, 01 = on, FF = ignore)
 *                      64                            - Dimmer percentage (01 to 64 = 1 to 100%)
 *                         FF FF FF FF FF FF FF FF    - Not used
 *                                                 6C - CRC over bytes 2 to F (Addition)
\*********************************************************************************************/

#define XDRV_37                   37

struct SONOFFD1 {
  uint8_t receive_len = 0;
  uint8_t power = 255;      // Not initialized
  uint8_t dimmer = 255;     // Not initialized
} SnfD1;

/********************************************************************************************/

void SonoffD1Received(void)
{
  if (serial_in_byte_counter < 8) { return; }  // Received ack from Rf chip (aa 55 01 04 00 00 05)

  uint8_t action = serial_in_buffer[6] & 1;
  if (action != SnfD1.power) {
    SnfD1.power = action;

//    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SD1: Remote power (%d, %d)"), SnfD1.power, SnfD1.dimmer);

    ExecuteCommandPower(1, action, SRC_SWITCH);
  }

  uint8_t dimmer = serial_in_buffer[7];
  if (dimmer != SnfD1.dimmer) {
    SnfD1.dimmer = dimmer;

//    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SD1: Remote dimmer (%d, %d)"), SnfD1.power, SnfD1.dimmer);

    char scmnd[20];
    snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_DIMMER " %d"), SnfD1.dimmer);
    ExecuteCommand(scmnd, SRC_SWITCH);
  }

/*
  // Send Acknowledge - Copy first 5 bytes, reset byte 6 and store crc in byte 7
  // AA 55 01 04 00 00 05
  serial_in_buffer[5] = 0;                      // Ack
  serial_in_buffer[6] = 0;                      // Crc
  for (uint32_t i = 0; i < 7; i++) {
    if ((i > 1) && (i < 6)) { serial_in_buffer[6] += serial_in_buffer[i]; }
    Serial.write(serial_in_buffer[i]);
  }
*/
}

bool SonoffD1SerialInput(void)
{
  if (0xAA == serial_in_byte) {               // 0xAA - Start of text
    serial_in_byte_counter = 0;
    SnfD1.receive_len = 7;
  }
  if (SnfD1.receive_len) {
    serial_in_buffer[serial_in_byte_counter++] = serial_in_byte;
    if (6 == serial_in_byte_counter) {
      SnfD1.receive_len += serial_in_byte;  // 8 or 17
    }
    if (serial_in_byte_counter == SnfD1.receive_len) {

      // Sonoff D1 codes
      // aa 55 01 04 00 0a 01 01 ff ff ff ff ff ff ff ff 09 - Power On, Dimmer 1%
      // aa 55 01 04 00 0a 01 28 ff ff ff ff ff ff ff ff 30 - Power On, Dimmer 40%
      // aa 55 01 04 00 0a 01 3c ff ff ff ff ff ff ff ff 44 - Power On, Dimmer 60%
      // aa 55 01 04 00 0a 01 64 ff ff ff ff ff ff ff ff 6c - Power On, Dimmer 100%
      // aa 55 01 04 00 0a 00 64 ff ff ff ff ff ff ff ff 6b - Power Off (with last dimmer 100%)
      // aa 55 01 04 00 0a 01 64 ff ff ff ff ff ff ff ff 6c - Power On (with last dimmer 100%)

      AddLogSerial(LOG_LEVEL_DEBUG);
      uint8_t crc = 0;
      for (uint32_t i = 2; i < SnfD1.receive_len -1; i++) {
        crc += serial_in_buffer[i];
      }
      if (crc == serial_in_buffer[SnfD1.receive_len -1]) {
        SonoffD1Received();
        SnfD1.receive_len = 0;
        return true;
      }
    }
    serial_in_byte = 0;
  }
  return false;
}

/********************************************************************************************/

void SonoffD1Send()
{
  //                        0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16
  uint8_t buffer[17] = { 0xAA,0x55,0x01,0x04,0x00,0x0A,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00 };

  buffer[6] = SnfD1.power;
  buffer[7] = SnfD1.dimmer;

  for (uint32_t i = 0; i < sizeof(buffer); i++) {
    if ((i > 1) && (i < sizeof(buffer) -1)) { buffer[16] += buffer[i]; }
    Serial.write(buffer[i]);
  }
}

bool SonoffD1SendPower(void)
{
  uint8_t action = XdrvMailbox.index &1;
  if (action != SnfD1.power) {
    SnfD1.power = action;

//    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SD1: Tasmota power (%d, %d)"), SnfD1.power, SnfD1.dimmer);

    SonoffD1Send();
  }
  return true;
}

bool SonoffD1SendDimmer(void)
{
  uint8_t dimmer = LightGetDimmer(1);
  dimmer = (dimmer < Settings.dimmer_hw_min) ? Settings.dimmer_hw_min : dimmer;
  dimmer = (dimmer > Settings.dimmer_hw_max) ? Settings.dimmer_hw_max : dimmer;
  if (dimmer != SnfD1.dimmer) {
    SnfD1.dimmer = dimmer;

//    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SD1: Tasmota dimmer (%d, %d)"), SnfD1.power, SnfD1.dimmer);

    SonoffD1Send();
  }
  return true;
}

bool SonoffD1ModuleSelected(void)
{
  SetSerial(9600, TS_SERIAL_8N1);

  devices_present++;
  light_type = LT_SERIAL1;

  return true;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv37(uint8_t function)
{
  bool result = false;

  if (SONOFF_D1 == my_module_type) {
    switch (function) {
      case FUNC_SERIAL:
        result = SonoffD1SerialInput();
        break;
      case FUNC_SET_DEVICE_POWER:
        result = SonoffD1SendPower();
        break;
      case FUNC_SET_CHANNELS:
        result = SonoffD1SendDimmer();
        break;
      case FUNC_MODULE_INIT:
        result = SonoffD1ModuleSelected();
        break;
    }
  }
  return result;
}

#endif  // USE_SONOFF_D1
