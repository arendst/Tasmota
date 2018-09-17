/*
  xnrg_04_mcp39f501.ino - MCP39F501 energy sensor support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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

#ifdef USE_ENERGY_SENSOR
#ifdef USE_MCP39F501
/*********************************************************************************************\
 * MCP39F501 - Energy (Shelly 2)
 *
 * Based on datasheet from https://www.microchip.com/wwwproducts/en/MCP39F501
 * and https://github.com/OLIMEX/olimex-iot-firmware-esp8266/blob/7a7f9bb56d4b72770dba8d0f18eaa9d956dd0baf/olimex/user/modules/mod_emtr.c
\*********************************************************************************************/

#define XNRG_04                  4

#define MCP_START_FRAME       0xA5
#define MCP_ACK_FRAME         0x06
#define MCP_ERROR_NAK         0x15
#define MCP_ERROR_CRC         0x51

#define MCP_SINGLE_WIRE       0xAB

#define MCP_SET_ADDRESS       0x41

#define MCP_READ              0x4E
#define MCP_READ_16           0x52
#define MCP_READ_32           0x44

#define MCP_WRITE             0x4D
#define MCP_WRITE_16          0x57
#define MCP_WRITE_32          0x45

#define MCP_SAVE_REGISTERS    0x53

#define MCP_FLASH_READ        0x42
#define MCP_FLASH_WRITE       0x50

uint32 mcp_system_configuration = 0x03000000;
uint8_t mcp_single_wire_active = 0;

/*********************************************************************************************\
 * Olimex tools
 * https://github.com/OLIMEX/olimex-iot-firmware-esp8266/blob/7a7f9bb56d4b72770dba8d0f18eaa9d956dd0baf/olimex/user/modules/mod_emtr.c
\*********************************************************************************************/


unsigned long McpExtractInt(uint8_t *data, uint8_t offset, uint8_t size)
{
	unsigned long result = 0;
	unsigned long pow = 1;

	for (byte i = 0; i < size; i++) {
		result = result + data[offset + i] * pow;
		pow = pow * 256;
	}
	return result;
}

void McpSetSystemConfiguration(uint16 interval)
{
  uint8_t data[17];

  data[ 0] = MCP_START_FRAME;
  data[ 1] = sizeof(data);
  data[ 2] = MCP_SET_ADDRESS;     // Set address pointer
  data[ 3] = 0x00;                // address
  data[ 4] = 0x42;                // address
  data[ 5] = MCP_WRITE_32;        // Write 4 bytes
  data[ 6] = (mcp_system_configuration >> 24) & 0xFF; // system_configuration
  data[ 7] = (mcp_system_configuration >> 16) & 0xFF; // system_configuration
  data[ 8] = (mcp_system_configuration >>  8) & 0xFF; // system_configuration
  data[ 9] = (mcp_system_configuration >>  0) & 0xFF; // system_configuration
  data[10] = MCP_SET_ADDRESS;     // Set address pointer
  data[11] = 0x00;                // address
  data[12] = 0x5A;                // address
  data[13] = MCP_WRITE_16;        // Write 2 bytes
  data[14] = (interval >>  8) & 0xFF; // interval
  data[15] = (interval >>  0) & 0xFF; // interval
  uint8_t checksum = 0;
  for (byte i = 0; i < sizeof(data) -1; i++) { checksum += (uint8_t)data[i]; }
  data[16] = checksum;

  // A5 11 41 00 42 45 03 00 01 00 41 00 5A 57 00 06 7A
  AddLogSerial(LOG_LEVEL_DEBUG, data, sizeof(data));

  for (byte i = 0; i < sizeof(data); i++) { Serial.write(data[i]); }
}

void McpSingleWireStart()
{
  if ((mcp_system_configuration & (1 << 8)) != 0) { return; }
  mcp_system_configuration = mcp_system_configuration | (1 << 8);
	McpSetSystemConfiguration(6);  // 64
  mcp_single_wire_active = 1;
}

void McpSingleWireStop()
{
  if ((mcp_system_configuration & (1 << 8)) == 0) { return; }
  mcp_system_configuration = mcp_system_configuration & (~(1 << 8));
	McpSetSystemConfiguration(2);  // 4
  mcp_single_wire_active = 0;
}

/********************************************************************************************/

unsigned long mcp_current = 0;
unsigned long mcp_voltage = 0;
unsigned long mcp_power = 0;
unsigned long mcp_frequency = 0;

void McpParseData(uint8_t single_wire)
{
  if (single_wire) {
    //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
    // AB CD EF 51 06 00 00 B8 08 FC 0D 00 00 0A C4 11
    // Header-- Current---- Volt- Power------ Freq- Ck

    mcp_current = McpExtractInt((uint8_t*)serial_in_buffer,  3, 4);
    mcp_voltage = McpExtractInt((uint8_t*)serial_in_buffer,  7, 2);
    mcp_power = McpExtractInt((uint8_t*)serial_in_buffer,  9, 4);
    mcp_frequency = McpExtractInt((uint8_t*)serial_in_buffer,  13, 2);
  } else {
    //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
    // 06 19 61 06 00 00 FE 08 9B 0E 00 00 0B 00 00 00 97 0E 00 00 FF 7F 0C C6 35
    // Ak Ln Current---- Volt- ActivePower ReActivePow ApparentPow Factr Frequ Ck

    mcp_current = McpExtractInt((uint8_t*)serial_in_buffer, 2, 4);
    mcp_voltage = McpExtractInt((uint8_t*)serial_in_buffer, 6, 2);
    mcp_power = McpExtractInt((uint8_t*)serial_in_buffer, 8, 4);
    mcp_frequency = McpExtractInt((uint8_t*)serial_in_buffer, 22, 2);
  }

  if (energy_power_on) {  // Powered on
    energy_frequency = (float)mcp_frequency / 1000;
    energy_voltage = (float)mcp_voltage / 10;
    energy_power = (float)mcp_power / 100;
    if (0 == energy_power) {
      energy_current = 0;
    } else {
      energy_current = (float)mcp_current / 10000;
    }
  } else {  // Powered off
    energy_frequency = 0;
    energy_voltage = 0;
    energy_power = 0;
    energy_current = 0;
  }
}

bool McpSerialInput()
{
  Settings.flag.mqtt_serial = 0;  // Disable possible SerialReceive handling

  serial_in_buffer[serial_in_byte_counter++] = serial_in_byte;
  if (MCP_ERROR_CRC == serial_in_buffer[0]) {
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("MCP: Send " D_CHECKSUM_FAILURE));
    return 1;
  }
  else if (MCP_ACK_FRAME == serial_in_buffer[0]) {
    if ((serial_in_byte_counter > 1) && (serial_in_byte_counter == serial_in_buffer[1])) {

      AddLogSerial(LOG_LEVEL_DEBUG_MORE);

      uint8_t checksum = 0;
      for (byte i = 0; i < serial_in_byte_counter -1; i++) { checksum += (uint8_t)serial_in_buffer[i]; }
      if (checksum != serial_in_buffer[serial_in_byte_counter -1]) {
        AddLog_P(LOG_LEVEL_DEBUG, PSTR("MCP: " D_CHECKSUM_FAILURE));
      } else {
        if (25 == serial_in_buffer[1]) { McpParseData(0); }
      }
      return 1;
    }
  }
  else if (MCP_SINGLE_WIRE == serial_in_buffer[0]) {
    if (serial_in_byte_counter == 16) {

      AddLogSerial(LOG_LEVEL_DEBUG_MORE);

      uint8_t checksum = 0;
      for (byte i = 3; i < serial_in_byte_counter -1; i++) { checksum += (uint8_t)serial_in_buffer[i]; }
//      if (~checksum != serial_in_buffer[serial_in_byte_counter -1]) {
//        AddLog_P(LOG_LEVEL_DEBUG, PSTR("MCP: " D_CHECKSUM_FAILURE));
//      } else {
        McpParseData(1);
//      }
      return 1;
    }
  }
  else {
    return 1;
  }
  serial_in_byte = 0;             // Discard
  return 0;
}

/********************************************************************************************/

void McpEverySecond()
{
  if (!mcp_single_wire_active) {
    char get_state[] = "A5084100044E1656";
    SerialSendRaw(get_state, sizeof(get_state));
  }

  energy_kWhtoday += (energy_power / 36);
  EnergyUpdateToday();
}

void McpSnsInit()
{
  digitalWrite(15, 1);               // GPIO15 - MCP enable
}

void McpDrvInit()
{
  if (!energy_flg) {
    if (SHELLY2 == Settings.module) {
      pinMode(15, OUTPUT);
      digitalWrite(15, 0);           // GPIO15 - MCP disable - Reset Delta Sigma ADC's
      baudrate = 4800;
      energy_calc_power_factor = 1;  // Calculate power factor from data
      energy_flg = XNRG_04;
    }
  }
}

boolean McpCommand()
{
  boolean serviced = true;

  if ((CMND_POWERCAL == energy_command_code) || (CMND_VOLTAGECAL == energy_command_code) || (CMND_CURRENTCAL == energy_command_code)) {

  }
  else if (CMND_POWERSET == energy_command_code) {
    if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 3601) && power_cycle) {
//      Settings.energy_power_calibration = (XdrvMailbox.payload * power_cycle) / CSE_PREF;
    }
  }
  else if (CMND_VOLTAGESET == energy_command_code) {
    if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 501) && voltage_cycle) {
//      Settings.energy_voltage_calibration = (XdrvMailbox.payload * voltage_cycle) / CSE_UREF;
    }
  }
  else if (CMND_CURRENTSET == energy_command_code) {
    if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 16001) && current_cycle) {
//      Settings.energy_current_calibration = (XdrvMailbox.payload * current_cycle) / 1000;
    }
  }
  else serviced = false;  // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

int Xnrg04(byte function)
{
  int result = 0;

  if (FUNC_PRE_INIT == function) {
    McpDrvInit();
  }
  else if (XNRG_04 == energy_flg) {
    switch (function) {
      case FUNC_INIT:
        McpSnsInit();
        break;
      case FUNC_EVERY_SECOND:
        McpEverySecond();
        break;
      case FUNC_COMMAND:
        result = McpCommand();
        break;
      case FUNC_SERIAL:
        result = McpSerialInput();
        break;
    }
  }
  return result;
}

#endif  // USE_MCP39F501
#endif  // USE_ENERGY_SENSOR
