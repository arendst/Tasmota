/*
  xnrg_02_cse7766.ino - CSE7766 and HLW8032 energy sensor support for Tasmota

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
#ifdef USE_CSE7766
/*********************************************************************************************\
 * CSE7766 - Energy (Sonoff S31 and Sonoff Pow R2)
 * HLW8032 - Energy (Blitzwolf SHP5)
 *
 * Based on datasheet from http://www.chipsea.com/UploadFiles/2017/08/11144342F01B5662.pdf
\*********************************************************************************************/

#define XNRG_02                     2

#define CSE_MAX_INVALID_POWER       128        // Number of invalid power receipts before deciding active power is zero

#define CSE_NOT_CALIBRATED          0xAA

#define CSE_PULSES_NOT_INITIALIZED  -1

#define CSE_PREF                    1000
#define CSE_UREF                    100

struct CSE {
  long voltage_cycle = 0;
  long current_cycle = 0;
  long power_cycle = 0;
  long power_cycle_first = 0;
  long cf_pulses = 0;
  long cf_pulses_last_time = CSE_PULSES_NOT_INITIALIZED;

  uint8_t power_invalid = 0;
  bool received = false;
} Cse;

void CseReceived(void)
{
  //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
  // F2 5A 02 F7 60 00 03 61 00 40 10 05 72 40 51 A6 58 63 10 1B E1 7F 4D 4E - F2 = Power cycle exceeds range - takes too long - No load
  // 55 5A 02 F7 60 00 03 5A 00 40 10 04 8B 9F 51 A6 58 18 72 75 61 AC A1 30 - 55 = Ok, 61 = Power not valid (load below 5W)
  // 55 5A 02 F7 60 00 03 AB 00 40 10 02 60 5D 51 A6 58 03 E9 EF 71 0B 7A 36 - 55 = Ok, 71 = Ok
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
  Cse.voltage_cycle = serial_in_buffer[5] << 16 | serial_in_buffer[6] << 8 | serial_in_buffer[7];
  Cse.current_cycle = serial_in_buffer[11] << 16 | serial_in_buffer[12] << 8 | serial_in_buffer[13];
  Cse.power_cycle = serial_in_buffer[17] << 16 | serial_in_buffer[18] << 8 | serial_in_buffer[19];
  Cse.cf_pulses = serial_in_buffer[21] << 8 | serial_in_buffer[22];

  if (Energy.power_on) {  // Powered on
    if (adjustement & 0x40) {  // Voltage valid
      Energy.voltage[0] = (float)(Settings.energy_voltage_calibration * CSE_UREF) / (float)Cse.voltage_cycle;
    }
    if (adjustement & 0x10) {  // Power valid
      Cse.power_invalid = 0;
      if ((header & 0xF2) == 0xF2) {  // Power cycle exceeds range
        Energy.active_power[0] = 0;
      } else {
        if (0 == Cse.power_cycle_first) { Cse.power_cycle_first = Cse.power_cycle; }  // Skip first incomplete Cse.power_cycle
        if (Cse.power_cycle_first != Cse.power_cycle) {
          Cse.power_cycle_first = -1;
          Energy.active_power[0] = (float)(Settings.energy_power_calibration * CSE_PREF) / (float)Cse.power_cycle;
        } else {
          Energy.active_power[0] = 0;
        }
      }
    } else {
      if (Cse.power_invalid < Settings.param[P_CSE7766_INVALID_POWER]) {  // Allow measurements down to about 1W
        Cse.power_invalid++;
      } else {
        Cse.power_cycle_first = 0;
        Energy.active_power[0] = 0;  // Powered on but no load
      }
    }
    if (adjustement & 0x20) {  // Current valid
      if (0 == Energy.active_power[0]) {
        Energy.current[0] = 0;
      } else {
        Energy.current[0] = (float)Settings.energy_current_calibration / (float)Cse.current_cycle;
      }
    }
  } else {  // Powered off
    Cse.power_cycle_first = 0;
    Energy.voltage[0] = 0;
    Energy.active_power[0] = 0;
    Energy.current[0] = 0;
  }
}

bool CseSerialInput(void)
{
  if (Cse.received) {
    serial_in_buffer[serial_in_byte_counter++] = serial_in_byte;
    if (24 == serial_in_byte_counter) {

      AddLogSerial(LOG_LEVEL_DEBUG_MORE);

      uint8_t checksum = 0;
      for (uint32_t i = 2; i < 23; i++) { checksum += serial_in_buffer[i]; }
      if (checksum == serial_in_buffer[23]) {
        Energy.data_valid[0] = 0;
        CseReceived();
        Cse.received = false;
        return true;
      } else {
        AddLog_P(LOG_LEVEL_DEBUG, PSTR("CSE: " D_CHECKSUM_FAILURE));
        do {  // Sync buffer with data (issue #1907 and #3425)
          memmove(serial_in_buffer, serial_in_buffer +1, 24);
          serial_in_byte_counter--;
        } while ((serial_in_byte_counter > 2) && (0x5A != serial_in_buffer[1]));
        if (0x5A != serial_in_buffer[1]) {
          Cse.received = false;
          serial_in_byte_counter = 0;
        }
      }
    }
  } else {
    if ((0x5A == serial_in_byte) && (1 == serial_in_byte_counter)) {  // 0x5A - Packet header 2
      Cse.received = true;
    } else {
      serial_in_byte_counter = 0;
    }
    serial_in_buffer[serial_in_byte_counter++] = serial_in_byte;
  }
  serial_in_byte = 0;  // Discard
  return false;
}

/********************************************************************************************/

void CseEverySecond(void)
{
  if (Energy.data_valid[0] > ENERGY_WATCHDOG) {
    Cse.voltage_cycle = 0;
    Cse.current_cycle = 0;
    Cse.power_cycle = 0;
  } else {
    long cf_frequency = 0;

    if (CSE_PULSES_NOT_INITIALIZED == Cse.cf_pulses_last_time) {
      Cse.cf_pulses_last_time = Cse.cf_pulses;  // Init after restart
    } else {
      if (Cse.cf_pulses < Cse.cf_pulses_last_time) {  // Rolled over after 65535 pulses
        cf_frequency = (65536 - Cse.cf_pulses_last_time) + Cse.cf_pulses;
      } else {
        cf_frequency = Cse.cf_pulses - Cse.cf_pulses_last_time;
      }
      if (cf_frequency && Energy.active_power[0])  {
        unsigned long delta = (cf_frequency * Settings.energy_power_calibration) / 36;
        // prevent invalid load delta steps even checksum is valid (issue #5789):
        if (delta <= (3680*100/36) * 10 ) {  // max load for S31/Pow R2: 3.68kW
          Cse.cf_pulses_last_time = Cse.cf_pulses;
          Energy.kWhtoday_delta += delta;
        }
        else {
          AddLog_P(LOG_LEVEL_DEBUG, PSTR("CSE: Load overflow"));
          Cse.cf_pulses_last_time = CSE_PULSES_NOT_INITIALIZED;
        }
        EnergyUpdateToday();
      }
    }
  }
}

void CseDrvInit(void)
{
  if ((3 == pin[GPIO_CSE7766_RX]) && (1 == pin[GPIO_CSE7766_TX])) {  // As it uses 8E1 currently only hardware serial is supported
    PrepSerial(4800, TS_SERIAL_8E1);
    if (0 == Settings.param[P_CSE7766_INVALID_POWER]) {
      Settings.param[P_CSE7766_INVALID_POWER] = CSE_MAX_INVALID_POWER;  // SetOption39 1..255
    }
    Cse.power_invalid = Settings.param[P_CSE7766_INVALID_POWER];
    energy_flg = XNRG_02;
  }
}

bool CseCommand(void)
{
  bool serviced = true;

  if (CMND_POWERSET == Energy.command_code) {
    if (XdrvMailbox.data_len && Cse.power_cycle) {
      Settings.energy_power_calibration = (unsigned long)(CharToFloat(XdrvMailbox.data) * Cse.power_cycle) / CSE_PREF;
    }
  }
  else if (CMND_VOLTAGESET == Energy.command_code) {
    if (XdrvMailbox.data_len && Cse.voltage_cycle) {
      Settings.energy_voltage_calibration = (unsigned long)(CharToFloat(XdrvMailbox.data) * Cse.voltage_cycle) / CSE_UREF;
    }
  }
  else if (CMND_CURRENTSET == Energy.command_code) {
    if (XdrvMailbox.data_len && Cse.current_cycle) {
      Settings.energy_current_calibration = (unsigned long)(CharToFloat(XdrvMailbox.data) * Cse.current_cycle) / 1000;
    }
  }
  else serviced = false;  // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg02(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_SERIAL:
      result = CseSerialInput();
      break;
    case FUNC_ENERGY_EVERY_SECOND:
      CseEverySecond();
      break;
    case FUNC_COMMAND:
      result = CseCommand();
      break;
    case FUNC_PRE_INIT:
      CseDrvInit();
      break;
  }
  return result;
}

#endif  // USE_CSE7766
#endif  // USE_ENERGY_SENSOR
