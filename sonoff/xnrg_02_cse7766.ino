/*
  xnrg_02_cse7766.ino - CSE7766 energy sensor support for Sonoff-Tasmota

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
#ifdef USE_CSE7766
/*********************************************************************************************\
 * CSE7766 - Energy (Sonoff S31 and Sonoff Pow R2)
 *
 * Based on datasheet from http://www.chipsea.com/UploadFiles/2017/08/11144342F01B5662.pdf
\*********************************************************************************************/

#define XNRG_02                     2

#define CSE_MAX_INVALID_POWER       128        // Number of invalid power receipts before deciding active power is zero

#define CSE_NOT_CALIBRATED          0xAA

#define CSE_PULSES_NOT_INITIALIZED  -1

#define CSE_PREF                    1000
#define CSE_UREF                    100

uint8_t cse_receive_flag = 0;

long voltage_cycle = 0;
long current_cycle = 0;
long power_cycle = 0;
unsigned long power_cycle_first = 0;
long cf_pulses = 0;
long cf_pulses_last_time = CSE_PULSES_NOT_INITIALIZED;
uint8_t cse_power_invalid = CSE_MAX_INVALID_POWER;

void CseReceived(void)
{
  //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
  // 55 5A 02 F7 60 00 03 5A 00 40 10 04 8B 9F 51 A6 58 18 72 75 61 AC A1 30 - Power not valid (load below 5W)
  // 55 5A 02 F7 60 00 03 AB 00 40 10 02 60 5D 51 A6 58 03 E9 EF 71 0B 7A 36
  // Hd Id VCal---- Voltage- ICal---- Current- PCal---- Power--- Ad CF--- Ck

  uint8_t header = serial_in_buffer[0];
  if ((header & 0xFC) == 0xFC) {
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("CSE: Abnormal hardware"));
    return;
  }

  // Get chip calibration data (coefficients) and use as initial defaults
  if (HLW_UREF_PULSE == Settings.energy_voltage_calibration) {
    long voltage_coefficient = 191200;  // uSec
    if (CSE_NOT_CALIBRATED != header) {
      voltage_coefficient = serial_in_buffer[2] << 16 | serial_in_buffer[3] << 8 | serial_in_buffer[4];
    }
    Settings.energy_voltage_calibration = voltage_coefficient / CSE_UREF;
  }
  if (HLW_IREF_PULSE == Settings.energy_current_calibration) {
    long current_coefficient = 16140;  // uSec
    if (CSE_NOT_CALIBRATED != header) {
      current_coefficient = serial_in_buffer[8] << 16 | serial_in_buffer[9] << 8 | serial_in_buffer[10];
    }
    Settings.energy_current_calibration = current_coefficient;
  }
  if (HLW_PREF_PULSE == Settings.energy_power_calibration) {
    long power_coefficient = 5364000;  // uSec
    if (CSE_NOT_CALIBRATED != header) {
      power_coefficient = serial_in_buffer[14] << 16 | serial_in_buffer[15] << 8 | serial_in_buffer[16];
    }
    Settings.energy_power_calibration = power_coefficient / CSE_PREF;
  }

  uint8_t adjustement = serial_in_buffer[20];
  voltage_cycle = serial_in_buffer[5] << 16 | serial_in_buffer[6] << 8 | serial_in_buffer[7];
  current_cycle = serial_in_buffer[11] << 16 | serial_in_buffer[12] << 8 | serial_in_buffer[13];
  power_cycle = serial_in_buffer[17] << 16 | serial_in_buffer[18] << 8 | serial_in_buffer[19];
  cf_pulses = serial_in_buffer[21] << 8 | serial_in_buffer[22];

  if (energy_power_on) {  // Powered on
    if (adjustement & 0x40) {  // Voltage valid
      energy_voltage = (float)(Settings.energy_voltage_calibration * CSE_UREF) / (float)voltage_cycle;
    }
    if (adjustement & 0x10) {  // Power valid
      cse_power_invalid = 0;
      if ((header & 0xF2) == 0xF2) {  // Power cycle exceeds range
        energy_active_power = 0;
      } else {
        if (0 == power_cycle_first) { power_cycle_first = power_cycle; }  // Skip first incomplete power_cycle
        if (power_cycle_first != power_cycle) {
          power_cycle_first = -1;
          energy_active_power = (float)(Settings.energy_power_calibration * CSE_PREF) / (float)power_cycle;
        } else {
          energy_active_power = 0;
        }
      }
    } else {
      if (cse_power_invalid < CSE_MAX_INVALID_POWER) {  // Allow measurements down to about 1W
        cse_power_invalid++;
      } else {
        power_cycle_first = 0;
        energy_active_power = 0;  // Powered on but no load
      }
    }
    if (adjustement & 0x20) {  // Current valid
      if (0 == energy_active_power) {
        energy_current = 0;
      } else {
        energy_current = (float)Settings.energy_current_calibration / (float)current_cycle;
      }
    }
  } else {  // Powered off
    power_cycle_first = 0;
    energy_voltage = 0;
    energy_active_power = 0;
    energy_current = 0;
  }
}

bool CseSerialInput(void)
{
  if (cse_receive_flag) {
    serial_in_buffer[serial_in_byte_counter++] = serial_in_byte;
    if (24 == serial_in_byte_counter) {

      AddLogSerial(LOG_LEVEL_DEBUG_MORE);

      uint8_t checksum = 0;
      for (byte i = 2; i < 23; i++) { checksum += serial_in_buffer[i]; }
      if (checksum == serial_in_buffer[23]) {
        CseReceived();
        cse_receive_flag = 0;
        return 1;
      } else {
        AddLog_P(LOG_LEVEL_DEBUG, PSTR("CSE: " D_CHECKSUM_FAILURE));
        do {  // Sync buffer with data (issue #1907 and #3425)
          memmove(serial_in_buffer, serial_in_buffer +1, 24);
          serial_in_byte_counter--;
        } while ((serial_in_byte_counter > 2) && (0x5A != serial_in_buffer[1]));
        if (0x5A != serial_in_buffer[1]) {
          cse_receive_flag = 0;
          serial_in_byte_counter = 0;
        }
      }
    }
  } else {
    if ((0x5A == serial_in_byte) && (1 == serial_in_byte_counter)) {  // 0x5A - Packet header 2
      cse_receive_flag = 1;
    } else {
      serial_in_byte_counter = 0;
    }
    serial_in_buffer[serial_in_byte_counter++] = serial_in_byte;
  }
  serial_in_byte = 0;  // Discard
  return 0;
}

/********************************************************************************************/

void CseEverySecond(void)
{
  long cf_frequency = 0;

  if (CSE_PULSES_NOT_INITIALIZED == cf_pulses_last_time) {
    cf_pulses_last_time = cf_pulses;  // Init after restart
  } else {
    if (cf_pulses < cf_pulses_last_time) {  // Rolled over after 65535 pulses
      cf_frequency = (65536 - cf_pulses_last_time) + cf_pulses;
    } else {
      cf_frequency = cf_pulses - cf_pulses_last_time;
    }
    if (cf_frequency && energy_active_power)  {
      cf_pulses_last_time = cf_pulses;
      energy_kWhtoday_delta += (cf_frequency * Settings.energy_power_calibration) / 36;
      EnergyUpdateToday();
    }
  }
}

void CseDrvInit(void)
{
  if (!energy_flg) {
    if ((SONOFF_S31 == Settings.module) || (SONOFF_POW_R2 == Settings.module)) {     // Sonoff S31 or Sonoff Pow R2
      baudrate = 4800;
      serial_config = SERIAL_8E1;
      energy_flg = XNRG_02;
    }
  }
}

boolean CseCommand(void)
{
  boolean serviced = true;

  if (CMND_POWERSET == energy_command_code) {
    if (XdrvMailbox.data_len && power_cycle) {
      Settings.energy_power_calibration = ((unsigned long)CharToDouble(XdrvMailbox.data) * power_cycle) / CSE_PREF;
    }
  }
  else if (CMND_VOLTAGESET == energy_command_code) {
    if (XdrvMailbox.data_len && voltage_cycle) {
      Settings.energy_voltage_calibration = ((unsigned long)CharToDouble(XdrvMailbox.data) * voltage_cycle) / CSE_UREF;
    }
  }
  else if (CMND_CURRENTSET == energy_command_code) {
    if (XdrvMailbox.data_len && current_cycle) {
      Settings.energy_current_calibration = ((unsigned long)CharToDouble(XdrvMailbox.data) * current_cycle) / 1000;
    }
  }
  else serviced = false;  // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

int Xnrg02(byte function)
{
  int result = 0;

  if (FUNC_PRE_INIT == function) {
    CseDrvInit();
  }
  else if (XNRG_02 == energy_flg) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        CseEverySecond();
        break;
      case FUNC_COMMAND:
        result = CseCommand();
        break;
      case FUNC_SERIAL:
        result = CseSerialInput();
        break;
    }
  }
  return result;
}

#endif  // USE_CSE7766
#endif  // USE_ENERGY_SENSOR
