/*
  xdrv_37_sonoff_d1.ino - sonoff D1 dimmer support for Tasmota

  Copyright (C) 2020  Theo Arends and robbz23 (protocol analysis)

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
 *
 * Based on Gravitate1:
 * When I switch the light ON via the app, I get:
 * AA 55 01 04 00 0A 01 64 FF FF FF FF FF FF FF FF 6C
 *
 * When I switch it OFF, I get:
 * AA 55 01 04 00 0A 00 64 FF FF FF FF FF FF FF FF 6B
 *
 * When I set it to 1%, I get:
 * AA 55 01 04 00 0A FF 01 FF FF FF FF FF FF FF FF 07
 * AB 55 FD F7 FF FF F5 01 FF FF FF FF FF FF FF FF 09
 *
 * When I set it to 6%, I get:
 * AA 55 01 04 00 0A FF 06 FF FF FF FF FF FF FF FF 0C
 * AB 55 FD F7 FF FF F5 06 FF FF FF FF FF FF FF FF 0E
 *
 * When I set it to 100%, I get:
 * AA 55 01 04 00 0A FF 64 FF FF FF FF FF FF FF FF 6A
 * AB 55 FD F7 FF FF F5 64 FF FF FF FF FF FF FF FF 6C
 *
 * Based on robbz23:
 * 00:17:59 CMD: Baudrate 9600
 * 00:17:59 SER: Set to 8N1 9600 bit/s
 * 00:17:59 RSL: stat/tasmota_D9E56D/RESULT = {"Baudrate":9600}
 *
 * 00:25:32 CMD: SerialSend5 aa 55 01 04 00 0a 01 22 ffffffffffffffff 29
 * 00:25:32 RSL: stat/tasmota_D9E56D/RESULT = {"SerialSend":"Done"}
 *
 * 00:26:35 CMD: SerialSend5 aa 55 01 04 00 0a 01 22 ffffffffffffffff 2a
 * 00:26:35 RSL: stat/tasmota_D9E56D/RESULT = {"SerialSend":"Done"}
 * 00:26:35 RSL: tele/tasmota_D9E56D/RESULT = {"SerialReceived":AA 55 01 04 00 00 05}
 *
 * 00:28:58 CMD: SerialSend5 aa 55 01 04 00 0a 01 01 ffffffffffffffff 09
 * 00:28:58 RSL: stat/tasmota_D9E56D/RESULT = {"SerialSend":"Done"}
 * 00:28:58 RSL: tele/tasmota_D9E56D/RESULT = {"SerialReceived":AA 55 01 04 00 00 05}
 *
 * 00:29:12 RSL: tele/tasmota_D9E56D/RESULT = {"SerialReceived":AA 55 01 04 00 0A 01 3C FF FF FF FF FF FF FF FF 44}
 * 00:29:43 RSL: tele/tasmota_D9E56D/RESULT = {"SerialReceived":AA 55 01 04 00 0A 01 01 FF FF FF FF FF FF FF FF 09}
 * 00:29:53 RSL: tele/tasmota_D9E56D/RESULT = {"SerialReceived":AA 55 01 04 00 0A 01 64 FF FF FF FF FF FF FF FF 6C}
 *
 * 00:30:02 RSL: tele/tasmota_D9E56D/RESULT = {"SerialReceived":AA 55 01 04 00 0A FF 1E FF FF FF FF FF FF FF FF 24}
\*********************************************************************************************/

#define XDRV_37                   37

struct SONOFFD1 {
  uint8_t receive_flag = 0;
  uint8_t dimmer;
} SnfD1;

/********************************************************************************************/

void SonoffD1Received(void)
{
  char svalue[32];

  uint8_t action = serial_in_buffer[6];
  uint8_t dimmer = serial_in_buffer[7];

  if (action < 2) {
    // AA 55 01 04 00 0A 01 64 FF FF FF FF FF FF FF FF 6C - Power On, Dimmer 100%
    // AA 55 01 04 00 0A 00 64 FF FF FF FF FF FF FF FF 6B - Power Off, Dimmer 100%
    bool is_switch_change = (action != power);
    if (is_switch_change) {
      ExecuteCommandPower(1, action, SRC_SWITCH);
    }
  }
  else if (0xFF == action) {
    SnfD1.dimmer = dimmer;
    bool is_brightness_change = SnfD1.dimmer != Settings.light_dimmer;
    if (power && (SnfD1.dimmer > 0) && is_brightness_change) {
      char scmnd[20];
      snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_DIMMER " %d"), SnfD1.dimmer);
      ExecuteCommand(scmnd, SRC_SWITCH);
    }
  }

  // Send Acknowledge - Copy first 5 bytes, reset byte 6 and store crc in byte 7
  // AA 55 01 04 00 00 05
  serial_in_buffer[5] = 0;                      // Ack
  serial_in_buffer[6] = 0;                      // Crc
  for (uint32_t i = 0; i < 7; i++) {
    if ((i > 1) && (i < 6)) { serial_in_buffer[6] += serial_in_buffer[i]; }
    Serial.write(serial_in_buffer[i]);
  }
}

bool SonoffD1SerialInput(void)
{
  uint8_t packet_length = 0;

  if (0xAA == serial_in_byte) {               // 0xAA - Start of text
    serial_in_byte_counter = 0;
    SnfD1.receive_flag = true;
  }
  if (SnfD1.receive_flag) {
    serial_in_buffer[serial_in_byte_counter++] = serial_in_byte;
    if (serial_in_byte_counter == 6) {
      packet_length = 7 + serial_in_byte;  // 8 or 17
    }
    if (serial_in_byte_counter == packet_length) {

      // Sonoff D1 codes
      // AA 55 01 04 00 0A 01 64 FF FF FF FF FF FF FF FF 6C - Power On, Dimmer 100%
      // AA 55 01 04 00 0A 00 64 FF FF FF FF FF FF FF FF 6B - Power Off, Dimmer 100%
      // AA 55 01 04 00 0A FF 01 FF FF FF FF FF FF FF FF 07 - Power ignore, Dimmer 1%
      // AB 55 FD F7 FF FF F5 01 FF FF FF FF FF FF FF FF 09 - Response 2
      // AA 55 01 04 00 0A FF 06 FF FF FF FF FF FF FF FF 0C - Power ignore, Dimmer 6%
      // AB 55 FD F7 FF FF F5 06 FF FF FF FF FF FF FF FF 0E - Response 2
      // AA 55 01 04 00 0A FF 64 FF FF FF FF FF FF FF FF 6A - Power ignore, Dimmer 100%
      // AB 55 FD F7 FF FF F5 64 FF FF FF FF FF FF FF FF 6C - Response 2

      AddLogSerial(LOG_LEVEL_DEBUG);
      uint8_t crc = 0;
      for (uint32_t i = 2; i < packet_length -1; i++) {
        crc += serial_in_buffer[i];
      }
      if (crc == serial_in_buffer[packet_length -1]) {
        SonoffD1Received();
        SnfD1.receive_flag = false;
        return true;
      }
    }
    serial_in_byte = 0;
  }
  return false;
}

/********************************************************************************************/

bool SonoffD1SendPower(void)
{
  uint8_t buffer[17] = { 0xAA,0x55,0x01,0x04,0x00,0x0A,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00 };

  buffer[6] = XdrvMailbox.index;

  for (uint32_t i = 0; i < sizeof(buffer); i++) {
    if ((i > 1) && (i < sizeof(buffer) -1)) { buffer[16] += buffer[i]; }
    Serial.write(buffer[i]);
  }
  return true;
}

bool SonoffD1SendDimmer(void)
{
  uint8_t buffer[17] = { 0xAA,0x55,0x01,0x04,0x00,0x0A,0xFF,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00 };

  buffer[7] = changeUIntScale(((uint16_t *)XdrvMailbox.data)[0], 0, 255, 0, 100);

  for (uint32_t i = 0; i < sizeof(buffer); i++) {
    if ((i > 1) && (i < sizeof(buffer) -1)) { buffer[16] += buffer[i]; }
    Serial.write(buffer[i]);
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
