/*
  xnrg_04_mcp39f501.ino - MCP39F501 energy sensor support for Sonoff-Tasmota

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

#ifdef USE_ENERGY_SENSOR
#ifdef USE_MCP39F501
/*********************************************************************************************\
 * MCP39F501 - Energy (Shelly 2)
 *
 * Based on datasheet from https://www.microchip.com/wwwproducts/en/MCP39F501
 * and https://github.com/OLIMEX/olimex-iot-firmware-esp8266/blob/7a7f9bb56d4b72770dba8d0f18eaa9d956dd0baf/olimex/user/modules/mod_emtr.c
\*********************************************************************************************/

#define XNRG_04                 4

#define MCP_BAUDRATE            4800
#define MCP_TIMEOUT             4
#define MCP_CALIBRATION_TIMEOUT 2

#define MCP_CALIBRATE_POWER     0x001
#define MCP_CALIBRATE_VOLTAGE   0x002
#define MCP_CALIBRATE_CURRENT   0x004
#define MCP_CALIBRATE_FREQUENCY 0x008
#define MCP_SINGLE_WIRE_FLAG    0x100

#define MCP_START_FRAME         0xA5
#define MCP_ACK_FRAME           0x06
#define MCP_ERROR_NAK           0x15
#define MCP_ERROR_CRC           0x51

#define MCP_SINGLE_WIRE         0xAB

#define MCP_SET_ADDRESS         0x41

#define MCP_READ                0x4E
#define MCP_READ_16             0x52
#define MCP_READ_32             0x44

#define MCP_WRITE               0x4D
#define MCP_WRITE_16            0x57
#define MCP_WRITE_32            0x45

#define MCP_SAVE_REGISTERS      0x53

#define MCP_CALIBRATION_BASE    0x0028
#define MCP_CALIBRATION_LEN     52

#define MCP_FREQUENCY_REF_BASE  0x0094
#define MCP_FREQUENCY_GAIN_BASE 0x00AE
#define MCP_FREQUENCY_LEN       4

#define MCP_BUFFER_SIZE         60

#include <TasmotaSerial.h>
TasmotaSerial *McpSerial = NULL;

typedef struct mcp_cal_registers_type {
  uint16_t gain_current_rms;
  uint16_t gain_voltage_rms;
  uint16_t gain_active_power;
  uint16_t gain_reactive_power;
  sint32_t offset_current_rms;
  sint32_t offset_active_power;
  sint32_t offset_reactive_power;
  sint16_t dc_offset_current;
  sint16_t phase_compensation;
  uint16_t apparent_power_divisor;

  uint32_t system_configuration;
  uint16_t dio_configuration;
  uint32_t range;

  uint32_t calibration_current;
  uint16_t calibration_voltage;
  uint32_t calibration_active_power;
  uint32_t calibration_reactive_power;
  uint16_t accumulation_interval;
} mcp_cal_registers_type;

char *mcp_buffer = NULL;
unsigned long mcp_window = 0;
unsigned long mcp_kWhcounter = 0;
uint32_t mcp_system_configuration = 0x03000000;
uint32_t mcp_active_power;
//uint32_t mcp_reactive_power;
//uint32_t mcp_apparent_power;
uint32_t mcp_current_rms;
uint16_t mcp_voltage_rms;
uint16_t mcp_line_frequency;
//sint16_t mcp_power_factor;
uint8_t mcp_address = 0;
uint8_t mcp_calibration_active = 0;
uint8_t mcp_init = 0;
uint8_t mcp_timeout = 0;
uint8_t mcp_calibrate = 0;
uint8_t mcp_byte_counter = 0;

/*********************************************************************************************\
 * Olimex tools
 * https://github.com/OLIMEX/olimex-iot-firmware-esp8266/blob/7a7f9bb56d4b72770dba8d0f18eaa9d956dd0baf/olimex/user/modules/mod_emtr.c
\*********************************************************************************************/

uint8_t McpChecksum(uint8_t *data)
{
  uint8_t checksum = 0;
  uint8_t offset = 0;
  uint8_t len = data[1] -1;

  for (uint8_t i = offset; i < len; i++) { checksum += data[i];	}
  return checksum;
}

unsigned long McpExtractInt(char *data, uint8_t offset, uint8_t size)
{
	unsigned long result = 0;
	unsigned long pow = 1;

	for (uint8_t i = 0; i < size; i++) {
		result = result + (uint8_t)data[offset + i] * pow;
		pow = pow * 256;
	}
	return result;
}

void McpSetInt(unsigned long value, uint8_t *data, uint8_t offset, size_t size)
{
	for (uint8_t i = 0; i < size; i++) {
		data[offset + i] = ((value >> (i * 8)) & 0xFF);
	}
}

void McpSend(uint8_t *data)
{
  if (mcp_timeout) { return; }
  mcp_timeout = MCP_TIMEOUT;

  data[0] = MCP_START_FRAME;
  data[data[1] -1] = McpChecksum(data);

//  AddLogBuffer(LOG_LEVEL_DEBUG_MORE, data, data[1]);

  for (uint8_t i = 0; i < data[1]; i++) {
    McpSerial->write(data[i]);
  }
}

/********************************************************************************************/

void McpGetAddress(void)
{
  uint8_t data[] = { MCP_START_FRAME, 7, MCP_SET_ADDRESS, 0x00, 0x26, MCP_READ_16, 0x00 };

  McpSend(data);
}

void McpAddressReceive(void)
{
  // 06 05 004D 58
  mcp_address = mcp_buffer[3];
}

/********************************************************************************************/

void McpGetCalibration(void)
{
  if (mcp_calibration_active) { return; }
  mcp_calibration_active = MCP_CALIBRATION_TIMEOUT;

  uint8_t data[] = { MCP_START_FRAME, 8, MCP_SET_ADDRESS, (MCP_CALIBRATION_BASE >> 8) & 0xFF, MCP_CALIBRATION_BASE & 0xFF, MCP_READ, MCP_CALIBRATION_LEN, 0x00 };

  McpSend(data);
}

void McpParseCalibration(void)
{
  bool action = false;
  mcp_cal_registers_type cal_registers;

  // 06 37 C882 B6AD 0781 9273 06000000 00000000 00000000 0000 D3FF 0300 00000003 9204 120C1300 204E0000 9808 E0AB0000 D9940000 0200 24
  cal_registers.gain_current_rms           = McpExtractInt(mcp_buffer,  2, 2);
  cal_registers.gain_voltage_rms           = McpExtractInt(mcp_buffer,  4, 2);
  cal_registers.gain_active_power          = McpExtractInt(mcp_buffer,  6, 2);
  cal_registers.gain_reactive_power        = McpExtractInt(mcp_buffer,  8, 2);
  cal_registers.offset_current_rms         = McpExtractInt(mcp_buffer, 10, 4);
  cal_registers.offset_active_power        = McpExtractInt(mcp_buffer, 14, 4);
  cal_registers.offset_reactive_power      = McpExtractInt(mcp_buffer, 18, 4);
  cal_registers.dc_offset_current          = McpExtractInt(mcp_buffer, 22, 2);
  cal_registers.phase_compensation         = McpExtractInt(mcp_buffer, 24, 2);
  cal_registers.apparent_power_divisor     = McpExtractInt(mcp_buffer, 26, 2);

  cal_registers.system_configuration       = McpExtractInt(mcp_buffer, 28, 4);
  cal_registers.dio_configuration          = McpExtractInt(mcp_buffer, 32, 2);
  cal_registers.range                      = McpExtractInt(mcp_buffer, 34, 4);

  cal_registers.calibration_current        = McpExtractInt(mcp_buffer, 38, 4);
  cal_registers.calibration_voltage        = McpExtractInt(mcp_buffer, 42, 2);
  cal_registers.calibration_active_power   = McpExtractInt(mcp_buffer, 44, 4);
  cal_registers.calibration_reactive_power = McpExtractInt(mcp_buffer, 48, 4);
  cal_registers.accumulation_interval      = McpExtractInt(mcp_buffer, 52, 2);

  if (mcp_calibrate & MCP_CALIBRATE_POWER) {
    cal_registers.calibration_active_power = Settings.energy_power_calibration;
    if (McpCalibrationCalc(&cal_registers, 16)) { action = true; }
  }
  if (mcp_calibrate & MCP_CALIBRATE_VOLTAGE) {
    cal_registers.calibration_voltage = Settings.energy_voltage_calibration;
    if (McpCalibrationCalc(&cal_registers, 0)) { action = true; }
  }
  if (mcp_calibrate & MCP_CALIBRATE_CURRENT) {
    cal_registers.calibration_current = Settings.energy_current_calibration;
    if (McpCalibrationCalc(&cal_registers, 8)) { action = true; }
  }
  mcp_timeout = 0;
  if (action) { McpSetCalibration(&cal_registers); }

  mcp_calibrate = 0;

  Settings.energy_power_calibration = cal_registers.calibration_active_power;
  Settings.energy_voltage_calibration = cal_registers.calibration_voltage;
  Settings.energy_current_calibration = cal_registers.calibration_current;

  mcp_system_configuration = cal_registers.system_configuration;

  if (mcp_system_configuration & MCP_SINGLE_WIRE_FLAG) {
    mcp_system_configuration &= ~MCP_SINGLE_WIRE_FLAG;    // Reset SingleWire flag
	  McpSetSystemConfiguration(2);
  }
}

bool McpCalibrationCalc(struct mcp_cal_registers_type *cal_registers, uint8_t range_shift)
{
  uint32_t measured;
  uint32_t expected;
  uint16_t *gain;
  uint32_t new_gain;

  if (range_shift == 0) {
    measured = mcp_voltage_rms;
    expected = cal_registers->calibration_voltage;
    gain = &(cal_registers->gain_voltage_rms);
  } else if (range_shift == 8) {
    measured = mcp_current_rms;
    expected = cal_registers->calibration_current;
    gain = &(cal_registers->gain_current_rms);
  } else if (range_shift == 16) {
    measured = mcp_active_power;
    expected = cal_registers->calibration_active_power;
    gain = &(cal_registers->gain_active_power);
  } else {
    return false;
  }

  if (measured == 0) {
    return false;
  }

  uint32_t range = (cal_registers->range >> range_shift) & 0xFF;

calc:
  new_gain = (*gain) * expected / measured;

  if (new_gain < 25000) {
    range++;
    if (measured > 6) {
      measured = measured / 2;
      goto calc;
    }
  }

  if (new_gain > 55000) {
    range--;
    measured = measured * 2;
    goto calc;
  }

  *gain = new_gain;
  uint32_t old_range = (cal_registers->range >> range_shift) & 0xFF;
  cal_registers->range = cal_registers->range ^ (old_range << range_shift);
  cal_registers->range = cal_registers->range | (range << range_shift);

  return true;
}
/*
void McpCalibrationReactivePower(void)
{
  cal_registers.gain_reactive_power = cal_registers.gain_reactive_power * cal_registers.calibration_reactive_power / mcp_reactive_power;
}
*/
void McpSetCalibration(struct mcp_cal_registers_type *cal_registers)
{
  uint8_t data[7 + MCP_CALIBRATION_LEN + 2 + 1];

  data[1] = sizeof(data);
  data[2] = MCP_SET_ADDRESS;                         // Set address pointer
  data[3] = (MCP_CALIBRATION_BASE >> 8) & 0xFF;      // address
  data[4] = (MCP_CALIBRATION_BASE >> 0) & 0xFF;      // address

  data[5] = MCP_WRITE;                               // Write N bytes
  data[6] = MCP_CALIBRATION_LEN;

  McpSetInt(cal_registers->gain_current_rms,            data,  0+7, 2);
  McpSetInt(cal_registers->gain_voltage_rms,            data,  2+7, 2);
  McpSetInt(cal_registers->gain_active_power,           data,  4+7, 2);
  McpSetInt(cal_registers->gain_reactive_power,         data,  6+7, 2);
  McpSetInt(cal_registers->offset_current_rms,          data,  8+7, 4);
  McpSetInt(cal_registers->offset_active_power,         data, 12+7, 4);
  McpSetInt(cal_registers->offset_reactive_power,       data, 16+7, 4);
  McpSetInt(cal_registers->dc_offset_current,           data, 20+7, 2);
  McpSetInt(cal_registers->phase_compensation,          data, 22+7, 2);
  McpSetInt(cal_registers->apparent_power_divisor,      data, 24+7, 2);

  McpSetInt(cal_registers->system_configuration,        data, 26+7, 4);
  McpSetInt(cal_registers->dio_configuration,           data, 30+7, 2);
  McpSetInt(cal_registers->range,                       data, 32+7, 4);

  McpSetInt(cal_registers->calibration_current,         data, 36+7, 4);
  McpSetInt(cal_registers->calibration_voltage,         data, 40+7, 2);
  McpSetInt(cal_registers->calibration_active_power,    data, 42+7, 4);
  McpSetInt(cal_registers->calibration_reactive_power,  data, 46+7, 4);
  McpSetInt(cal_registers->accumulation_interval,       data, 50+7, 2);

  data[MCP_CALIBRATION_LEN+7] = MCP_SAVE_REGISTERS;  // Save registers to flash
  data[MCP_CALIBRATION_LEN+8] = mcp_address;         // Device address

  McpSend(data);
}

/********************************************************************************************/

void McpSetSystemConfiguration(uint16 interval)
{
  // A5 11 41 00 42 45 03 00 01 00 41 00 5A 57 00 06 7A
  uint8_t data[17];

  data[ 1] = sizeof(data);
  data[ 2] = MCP_SET_ADDRESS;                          // Set address pointer
  data[ 3] = 0x00;                                     // address
  data[ 4] = 0x42;                                     // address
  data[ 5] = MCP_WRITE_32;                             // Write 4 bytes
  data[ 6] = (mcp_system_configuration >> 24) & 0xFF;  // system_configuration
  data[ 7] = (mcp_system_configuration >> 16) & 0xFF;  // system_configuration
  data[ 8] = (mcp_system_configuration >>  8) & 0xFF;  // system_configuration
  data[ 9] = (mcp_system_configuration >>  0) & 0xFF;  // system_configuration
  data[10] = MCP_SET_ADDRESS;                          // Set address pointer
  data[11] = 0x00;                                     // address
  data[12] = 0x5A;                                     // address
  data[13] = MCP_WRITE_16;                             // Write 2 bytes
  data[14] = (interval >>  8) & 0xFF;                  // interval
  data[15] = (interval >>  0) & 0xFF;                  // interval

  McpSend(data);
}

/********************************************************************************************/

void McpGetFrequency(void)
{
  if (mcp_calibration_active) { return; }
  mcp_calibration_active = MCP_CALIBRATION_TIMEOUT;

  uint8_t data[] = { MCP_START_FRAME, 11, MCP_SET_ADDRESS, (MCP_FREQUENCY_REF_BASE >> 8) & 0xFF,  MCP_FREQUENCY_REF_BASE & 0xFF,  MCP_READ_16,
                                          MCP_SET_ADDRESS, (MCP_FREQUENCY_GAIN_BASE >> 8) & 0xFF, MCP_FREQUENCY_GAIN_BASE & 0xFF, MCP_READ_16, 0x00 };

  McpSend(data);
}

void McpParseFrequency(void)
{
  // 06 07 C350 8000 A0
  uint16_t line_frequency_ref  = mcp_buffer[2] * 256 + mcp_buffer[3];
  uint16_t gain_line_frequency = mcp_buffer[4] * 256 + mcp_buffer[5];

  if (mcp_calibrate & MCP_CALIBRATE_FREQUENCY) {
    line_frequency_ref = Settings.energy_frequency_calibration;

    if ((0xFFFF == mcp_line_frequency) || (0 == gain_line_frequency)) {  // Reset values to 50Hz
      mcp_line_frequency  = 50000;
      gain_line_frequency = 0x8000;
    }
    gain_line_frequency = gain_line_frequency * line_frequency_ref / mcp_line_frequency;

    mcp_timeout = 0;
    McpSetFrequency(line_frequency_ref, gain_line_frequency);
  }

  Settings.energy_frequency_calibration = line_frequency_ref;

  mcp_calibrate = 0;
}

void McpSetFrequency(uint16_t line_frequency_ref, uint16_t gain_line_frequency)
{
  // A5 11 41 00 94 57 C3 B4 41 00 AE 57 7E 46 53 4D 03
  uint8_t data[17];

  data[ 1] = sizeof(data);
  data[ 2] = MCP_SET_ADDRESS;                        // Set address pointer
  data[ 3] = (MCP_FREQUENCY_REF_BASE >> 8) & 0xFF;   // address
  data[ 4] = (MCP_FREQUENCY_REF_BASE >> 0) & 0xFF;   // address

  data[ 5] = MCP_WRITE_16;                           // Write register
  data[ 6] = (line_frequency_ref >> 8) & 0xFF;       // line_frequency_ref high
  data[ 7] = (line_frequency_ref >> 0) & 0xFF;       // line_frequency_ref low

  data[ 8] = MCP_SET_ADDRESS;                        // Set address pointer
  data[ 9] = (MCP_FREQUENCY_GAIN_BASE >> 8) & 0xFF;  // address
  data[10] = (MCP_FREQUENCY_GAIN_BASE >> 0) & 0xFF;  // address

  data[11] = MCP_WRITE_16;                           // Write register
  data[12] = (gain_line_frequency >> 8) & 0xFF;      // gain_line_frequency high
  data[13] = (gain_line_frequency >> 0) & 0xFF;      // gain_line_frequency low

  data[14] = MCP_SAVE_REGISTERS;                     // Save registers to flash
  data[15] = mcp_address;                            // Device address

  McpSend(data);
}

/********************************************************************************************/

void McpGetData(void)
{
  uint8_t data[] = { MCP_START_FRAME, 8, MCP_SET_ADDRESS, 0x00, 0x04, MCP_READ, 22, 0x00 };

  McpSend(data);
}

void McpParseData(void)
{
  //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
  // 06 19 61 06 00 00 FE 08 9B 0E 00 00 0B 00 00 00 97 0E 00 00 FF 7F 0C C6 35
  // 06 19 CE 18 00 00 F2 08 3A 38 00 00 66 00 00 00 93 38 00 00 36 7F 9A C6 B7
  // Ak Ln Current---- Volt- ActivePower ReActivePow ApparentPow Factr Frequ Ck

  mcp_current_rms    = McpExtractInt(mcp_buffer,  2, 4);
  mcp_voltage_rms    = McpExtractInt(mcp_buffer,  6, 2);
  mcp_active_power   = McpExtractInt(mcp_buffer,  8, 4);
//  mcp_reactive_power = McpExtractInt(mcp_buffer, 12, 4);
//  mcp_power_factor   = McpExtractInt(mcp_buffer, 20, 2);
  mcp_line_frequency = McpExtractInt(mcp_buffer, 22, 2);

  if (energy_power_on) {  // Powered on
    energy_frequency = (float)mcp_line_frequency / 1000;
    energy_voltage = (float)mcp_voltage_rms / 10;
    energy_active_power = (float)mcp_active_power / 100;
    if (0 == energy_active_power) {
      energy_current = 0;
    } else {
      energy_current = (float)mcp_current_rms / 10000;
    }
  } else {  // Powered off
    energy_frequency = 0;
    energy_voltage = 0;
    energy_active_power = 0;
    energy_current = 0;
  }
}

/********************************************************************************************/

void McpSerialInput(void)
{
  while ((McpSerial->available()) && (mcp_byte_counter < MCP_BUFFER_SIZE)) {
    yield();
    mcp_buffer[mcp_byte_counter++] = McpSerial->read();
    mcp_window = millis();
  }

  // Ignore until non received after 2 chars (= 12 bits/char) time
  if ((mcp_byte_counter) && (millis() - mcp_window > (24000 / MCP_BAUDRATE) +1)) {
    AddLogBuffer(LOG_LEVEL_DEBUG_MORE, (uint8_t*)mcp_buffer, mcp_byte_counter);

    if (MCP_BUFFER_SIZE == mcp_byte_counter) {
//      AddLog_P(LOG_LEVEL_DEBUG, PSTR("MCP: Overflow"));
    }
    else if (1 == mcp_byte_counter) {
      if (MCP_ERROR_CRC == mcp_buffer[0]) {
//        AddLog_P(LOG_LEVEL_DEBUG, PSTR("MCP: Send " D_CHECKSUM_FAILURE));
        mcp_timeout = 0;
      }
      else if (MCP_ERROR_NAK == mcp_buffer[0]) {
//        AddLog_P(LOG_LEVEL_DEBUG, PSTR("MCP: NAck"));
        mcp_timeout = 0;
      }
    }
    else if (MCP_ACK_FRAME == mcp_buffer[0]) {
      if (mcp_byte_counter == mcp_buffer[1]) {

        if (McpChecksum((uint8_t *)mcp_buffer) != mcp_buffer[mcp_byte_counter -1]) {
          AddLog_P(LOG_LEVEL_DEBUG, PSTR("MCP: " D_CHECKSUM_FAILURE));
        } else {
          if (5 == mcp_buffer[1]) { McpAddressReceive(); }
          if (25 == mcp_buffer[1]) { McpParseData(); }
          if (MCP_CALIBRATION_LEN + 3 == mcp_buffer[1]) { McpParseCalibration(); }
          if (MCP_FREQUENCY_LEN + 3 == mcp_buffer[1]) { McpParseFrequency(); }
        }

      }
      mcp_timeout = 0;
    }
    else if (MCP_SINGLE_WIRE == mcp_buffer[0]) {
      mcp_timeout = 0;
    }

    mcp_byte_counter = 0;
    McpSerial->flush();
  }
}

/********************************************************************************************/

void McpEverySecond(void)
{
  if (mcp_active_power) {
    energy_kWhtoday_delta += ((mcp_active_power * 10) / 36);
    EnergyUpdateToday();
  }

  if (mcp_timeout) {
    mcp_timeout--;
  }
  else if (mcp_calibration_active) {
    mcp_calibration_active--;
  }
  else if (mcp_init) {
    if (2 == mcp_init) {
      McpGetCalibration();           // Get calibration parameters and disable SingleWire mode if enabled
    }
    else if (1 == mcp_init) {
      McpGetFrequency();             // Get calibration parameter
    }
    mcp_init--;
  }
  else if (!mcp_address) {
    McpGetAddress();                 // Get device address for future calibration changes
  }
  else {
    McpGetData();                    // Get energy data
  }
}

void McpSnsInit(void)
{
  // Software serial init needs to be done here as earlier (serial) interrupts may lead to Exceptions
  McpSerial = new TasmotaSerial(pin[GPIO_MCP39F5_RX], pin[GPIO_MCP39F5_TX], 1);
  if (McpSerial->begin(MCP_BAUDRATE)) {
    if (McpSerial->hardwareSerial()) {
      ClaimSerial();
      mcp_buffer = serial_in_buffer;  // Use idle serial buffer to save RAM
    } else {
      mcp_buffer = (char*)(malloc(MCP_BUFFER_SIZE));
    }
    if (pin[GPIO_MCP39F5_RST] < 99) {
      digitalWrite(pin[GPIO_MCP39F5_RST], 1);  // MCP enable
    }
  } else {
    energy_flg = ENERGY_NONE;
  }
}

void McpDrvInit(void)
{
  if (!energy_flg) {
    if ((pin[GPIO_MCP39F5_RX] < 99) && (pin[GPIO_MCP39F5_TX] < 99)) {
      if (pin[GPIO_MCP39F5_RST] < 99) {
        pinMode(pin[GPIO_MCP39F5_RST], OUTPUT);
        digitalWrite(pin[GPIO_MCP39F5_RST], 0);  // MCP disable - Reset Delta Sigma ADC's
      }
      mcp_calibrate = 0;
      mcp_timeout = 2;               // Initial wait
      mcp_init = 2;                  // Initial setup steps
      energy_flg = XNRG_04;
    }
  }
}

bool McpCommand(void)
{
  bool serviced = true;
  unsigned long value = 0;

  if (CMND_POWERSET == energy_command_code) {
    if (XdrvMailbox.data_len && mcp_active_power) {
      value = (unsigned long)(CharToDouble(XdrvMailbox.data) * 100);
      if ((value > 100) && (value < 200000)) {  // Between 1W and 2000W
        Settings.energy_power_calibration = value;
        mcp_calibrate |= MCP_CALIBRATE_POWER;
        McpGetCalibration();
      }
    }
  }
  else if (CMND_VOLTAGESET == energy_command_code) {
    if (XdrvMailbox.data_len && mcp_voltage_rms) {
      value = (unsigned long)(CharToDouble(XdrvMailbox.data) * 10);
      if ((value > 1000) && (value < 2600)) {  // Between 100V and 260V
        Settings.energy_voltage_calibration = value;
        mcp_calibrate |= MCP_CALIBRATE_VOLTAGE;
        McpGetCalibration();
      }
    }
  }
  else if (CMND_CURRENTSET == energy_command_code) {
    if (XdrvMailbox.data_len && mcp_current_rms) {
      value = (unsigned long)(CharToDouble(XdrvMailbox.data) * 10);
      if ((value > 100) && (value < 80000)) {  // Between 10mA and 8A
        Settings.energy_current_calibration = value;
        mcp_calibrate |= MCP_CALIBRATE_CURRENT;
        McpGetCalibration();
      }
    }
  }
  else if (CMND_FREQUENCYSET == energy_command_code) {
    if (XdrvMailbox.data_len && mcp_line_frequency) {
      value = (unsigned long)(CharToDouble(XdrvMailbox.data) * 1000);
      if ((value > 45000) && (value < 65000)) {  // Between 45Hz and 65Hz
        Settings.energy_frequency_calibration = value;
        mcp_calibrate |= MCP_CALIBRATE_FREQUENCY;
        McpGetFrequency();
      }
    }
  }
  else serviced = false;  // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

int Xnrg04(uint8_t function)
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
      case FUNC_LOOP:
        if (McpSerial) { McpSerialInput(); }
        break;
      case FUNC_EVERY_SECOND:
        if (McpSerial) { McpEverySecond(); }
        break;
      case FUNC_COMMAND:
        result = McpCommand();
        break;
    }
  }
  return result;
}

#endif  // USE_MCP39F501
#endif  // USE_ENERGY_SENSOR
