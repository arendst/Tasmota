/*
  xnrg_02_cse7766.ino - CSE7766 and HLW8032 energy sensor support for Tasmota

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

#ifdef USE_ENERGY_SENSOR
#ifdef USE_CSE7766
/*********************************************************************************************\
 * CSE7759 and CSE7766 - Energy (Sonoff S31 and Sonoff Pow R2/R3)
 * HLW8032 - Energy (Blitzwolf SHP5)
 *
 * Needs GPIO_CSE7766_RX only
 *
 * Based on datasheet from http://www.chipsea.com/UploadFiles/2017/08/11144342F01B5662.pdf
\*********************************************************************************************/

#define XNRG_02                     2

#define CSE_MAX_INVALID_POWER       128        // Number of invalid power receipts before deciding active power is zero

#define CSE_NOT_CALIBRATED          0xAA

#define CSE_PULSES_NOT_INITIALIZED  -1

#define CSE_PREF                    1000
#define CSE_UREF                    100

#define CSE_BUFFER_SIZE             25

#include <TasmotaSerial.h>

TasmotaSerial *CseSerial = nullptr;

struct CSE {
  long voltage_cycle = 0;
  long current_cycle = 0;
  long power_cycle = 0;
  long power_cycle_first = 0;
  long cf_pulses = 0;
  long cf_pulses_last_time = CSE_PULSES_NOT_INITIALIZED;

  int byte_counter = 0;
  uint8_t *rx_buffer = nullptr;
  uint8_t power_invalid = 0;
  bool received = false;
} Cse;

void CseReceived(void) {
  //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
  // F2 5A 02 F7 60 00 03 61 00 40 10 05 72 40 51 A6 58 63 10 1B E1 7F 4D 4E - F2 = Power cycle exceeds range - takes too long - No load
  // 55 5A 02 F7 60 00 03 5A 00 40 10 04 8B 9F 51 A6 58 18 72 75 61 AC A1 30 - 55 = Ok, 61 = Power not valid (load below 5W)
  // 55 5A 02 F7 60 00 03 AB 00 40 10 02 60 5D 51 A6 58 03 E9 EF 71 0B 7A 36 - 55 = Ok, 71 = Ok, F1 = CF overflow, no problem
  // 55 5A 02 F1 E8 00 07 9D 00 3F 3E 00 35 F8 50 DB 38 00 B2 A2 F1 D6 97 3E - CF overflow

  // 55 5A 02 DB 40 00 03 25 00 3D 18 03 8E CD 4F 0A 60 2A 56 85 61 01 02 1A - OK voltage
  // 55 5A 02 DB 40 07 17 1D 00 3D 18 03 8E CD 4F 0A 60 2B EF EA 61 01 02 2C - Wrong voltage
  // Hd Id VCal---- Voltage- ICal---- Current- PCal---- Power--- Ad CF--- Ck

  uint8_t header = Cse.rx_buffer[0];
  if ((header & 0xFC) == 0xFC) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CSE: Abnormal hardware"));
    return;
  }

  // Get chip calibration data (coefficients) and use as initial defaults
  if (HLW_UREF_PULSE == EnergyGetCalibration(ENERGY_VOLTAGE_CALIBRATION)) {
    long voltage_coefficient = 191200;  // uSec
    if (CSE_NOT_CALIBRATED != header) {
      voltage_coefficient = Cse.rx_buffer[2] << 16 | Cse.rx_buffer[3] << 8 | Cse.rx_buffer[4];
    }
    EnergySetCalibration(ENERGY_VOLTAGE_CALIBRATION, voltage_coefficient / CSE_UREF);
  }
  if (HLW_IREF_PULSE == EnergyGetCalibration(ENERGY_CURRENT_CALIBRATION)) {
    long current_coefficient = 16140;  // uSec
    if (CSE_NOT_CALIBRATED != header) {
      current_coefficient = Cse.rx_buffer[8] << 16 | Cse.rx_buffer[9] << 8 | Cse.rx_buffer[10];
    }
    EnergySetCalibration(ENERGY_CURRENT_CALIBRATION, current_coefficient);
  }
  if (HLW_PREF_PULSE == EnergyGetCalibration(ENERGY_POWER_CALIBRATION)) {
    long power_coefficient = 5364000;  // uSec
    if (CSE_NOT_CALIBRATED != header) {
      power_coefficient = Cse.rx_buffer[14] << 16 | Cse.rx_buffer[15] << 8 | Cse.rx_buffer[16];
    }
    EnergySetCalibration(ENERGY_POWER_CALIBRATION, power_coefficient / CSE_PREF);
  }

  uint8_t adjustement = Cse.rx_buffer[20];
  Cse.voltage_cycle = Cse.rx_buffer[5] << 16 | Cse.rx_buffer[6] << 8 | Cse.rx_buffer[7];
  Cse.current_cycle = Cse.rx_buffer[11] << 16 | Cse.rx_buffer[12] << 8 | Cse.rx_buffer[13];
  Cse.power_cycle = Cse.rx_buffer[17] << 16 | Cse.rx_buffer[18] << 8 | Cse.rx_buffer[19];
  Cse.cf_pulses = Cse.rx_buffer[21] << 8 | Cse.rx_buffer[22];

  if (Energy->power_on) {  // Powered on
    if (adjustement & 0x40) {  // Voltage valid
      Energy->voltage[0] = (float)(EnergyGetCalibration(ENERGY_VOLTAGE_CALIBRATION) * CSE_UREF) / (float)Cse.voltage_cycle;
    }
    if (adjustement & 0x10) {  // Power valid
      Cse.power_invalid = 0;
      if ((header & 0xF2) == 0xF2) {  // Power cycle exceeds range
        Energy->active_power[0] = 0;
      } else {
        if (0 == Cse.power_cycle_first) { Cse.power_cycle_first = Cse.power_cycle; }  // Skip first incomplete Cse.power_cycle
        if (Cse.power_cycle_first != Cse.power_cycle) {
          Cse.power_cycle_first = -1;
          Energy->active_power[0] = (float)(EnergyGetCalibration(ENERGY_POWER_CALIBRATION) * CSE_PREF) / (float)Cse.power_cycle;
        } else {
          Energy->active_power[0] = 0;
        }
      }
    } else {
      if (Cse.power_invalid < Settings->param[P_CSE7766_INVALID_POWER]) {  // Allow measurements down to about 1W
        Cse.power_invalid++;
      } else {
        Cse.power_cycle_first = 0;
        Energy->active_power[0] = 0;  // Powered on but no load
      }
    }
    if (adjustement & 0x20) {  // Current valid
      if (0 == Energy->active_power[0]) {
        Energy->current[0] = 0;
      } else {
        Energy->current[0] = (float)EnergyGetCalibration(ENERGY_CURRENT_CALIBRATION) / (float)Cse.current_cycle;
      }
    }
  } else {  // Powered off
    Cse.power_cycle_first = 0;
    Energy->voltage[0] = 0;
    Energy->active_power[0] = 0;
    Energy->current[0] = 0;
  }
}

void CseSerialInput(void) {
  while (CseSerial->available()) {
    yield();
    uint8_t serial_in_byte = CseSerial->read();

    if (Cse.received) {
      Cse.rx_buffer[Cse.byte_counter++] = serial_in_byte;
      if (24 == Cse.byte_counter) {

        AddLogBuffer(LOG_LEVEL_DEBUG_MORE, Cse.rx_buffer, 24);

        uint8_t checksum = 0;
        for (uint32_t i = 2; i < 23; i++) { checksum += Cse.rx_buffer[i]; }
        if (checksum == Cse.rx_buffer[23]) {
          Energy->data_valid[0] = 0;
          CseReceived();
          Cse.received = false;
          return;
        } else {
          do {  // Sync buffer with data (issue #1907 and #3425)
            memmove(Cse.rx_buffer, Cse.rx_buffer +1, 24);
            Cse.byte_counter--;
          } while ((Cse.byte_counter > 2) && (0x5A != Cse.rx_buffer[1]));
          if (0x5A != Cse.rx_buffer[1]) {
            AddLog(LOG_LEVEL_DEBUG, PSTR("CSE: " D_CHECKSUM_FAILURE));
            Cse.received = false;
            Cse.byte_counter = 0;
          }
        }
      }
    } else {
      if ((0x5A == serial_in_byte) && (1 == Cse.byte_counter)) {  // 0x5A - Packet header 2
        Cse.received = true;
      } else {
        Cse.byte_counter = 0;
      }
      Cse.rx_buffer[Cse.byte_counter++] = serial_in_byte;
    }
  }
}

/********************************************************************************************/

void CseEverySecond(void) {
  if (Energy->data_valid[0] > ENERGY_WATCHDOG) {
    Cse.voltage_cycle = 0;
    Cse.current_cycle = 0;
    Cse.power_cycle = 0;
  } else {
    if (CSE_PULSES_NOT_INITIALIZED == Cse.cf_pulses_last_time) {
      Cse.cf_pulses_last_time = Cse.cf_pulses;  // Init after restart
    } else {
      uint32_t cf_pulses = 0;
      if (Cse.cf_pulses < Cse.cf_pulses_last_time) {  // Rolled over after 0xFFFF (65535) pulses
        cf_pulses = (0x10000 - Cse.cf_pulses_last_time) + Cse.cf_pulses;
      } else {
        cf_pulses = Cse.cf_pulses - Cse.cf_pulses_last_time;
      }
      if (cf_pulses && Energy->active_power[0])  {
        uint32_t delta = (cf_pulses * EnergyGetCalibration(ENERGY_POWER_CALIBRATION)) / 36;
        // prevent invalid load delta steps even checksum is valid (issue #5789):
        // prevent invalid load delta steps even checksum is valid but allow up to 4kW (issue #7155):
//        if (delta <= (4000 * 1000 / 36)) {  // max load for S31/Pow R2: 4.00kW
        // prevent invalid load delta steps even checksum is valid but allow up to 5.5kW (issue #14156):
        if (delta <= (5500 * 1000 / 36)) {  // max load for Pow R3: 5.50kW
          Cse.cf_pulses_last_time = Cse.cf_pulses;
          Energy->kWhtoday_delta[0] += delta;
        }
        else {
          AddLog(LOG_LEVEL_DEBUG, PSTR("CSE: Overload"));
          Cse.cf_pulses_last_time = CSE_PULSES_NOT_INITIALIZED;
        }
        EnergyUpdateToday();
      }
    }
  }
}

void CseSnsInit(void) {
  // Software serial init needs to be done here as earlier (serial) interrupts may lead to Exceptions
//  CseSerial = new TasmotaSerial(Pin(GPIO_CSE7766_RX), Pin(GPIO_CSE7766_TX), 1);
  CseSerial = new TasmotaSerial(Pin(GPIO_CSE7766_RX), -1, 1);
  if (CseSerial->begin(4800, SERIAL_8E1)) {
    if (CseSerial->hardwareSerial()) {
      SetSerial(4800, TS_SERIAL_8E1);
      ClaimSerial();
    }
#ifdef ESP32
    AddLog(LOG_LEVEL_DEBUG, PSTR("CSE: Serial UART%d"), CseSerial->getUart());
#endif
    if (0 == Settings->param[P_CSE7766_INVALID_POWER]) {
      Settings->param[P_CSE7766_INVALID_POWER] = CSE_MAX_INVALID_POWER;  // SetOption39 1..255
    }
    Cse.power_invalid = Settings->param[P_CSE7766_INVALID_POWER];
    Energy->use_overtemp = true;                 // Use global temperature for overtemp detection
  } else {
    TasmotaGlobal.energy_driver = ENERGY_NONE;
  }
}

void CseDrvInit(void) {
//  if (PinUsed(GPIO_CSE7766_RX) && PinUsed(GPIO_CSE7766_TX)) {
  if (PinUsed(GPIO_CSE7766_RX)) {
    Cse.rx_buffer = (uint8_t*)(malloc(CSE_BUFFER_SIZE));
    if (Cse.rx_buffer != nullptr) {
      TasmotaGlobal.energy_driver = XNRG_02;
    }
  }
}

bool CseCommand(void) {
  bool serviced = true;

  float value = CharToFloat(XdrvMailbox.data);

  if ((CMND_POWERCAL == Energy->command_code) ||
      (CMND_VOLTAGECAL == Energy->command_code) ||
      (CMND_CURRENTCAL == Energy->command_code)) {
    // Service in xdrv_03_energy.ino
  }
  else if (CMND_POWERSET == Energy->command_code) {    // xxx W
    if (XdrvMailbox.data_len && Cse.power_cycle) {
      XdrvMailbox.payload = (uint32_t)(value * Cse.power_cycle) / CSE_PREF;
    }
  }
  else if (CMND_VOLTAGESET == Energy->command_code) {  // xxx V
    if (XdrvMailbox.data_len && Cse.voltage_cycle) {
      XdrvMailbox.payload = (uint32_t)(value * Cse.voltage_cycle) / CSE_UREF;
    }
  }
  else if (CMND_CURRENTSET == Energy->command_code) {  // xxx mA
    if (XdrvMailbox.data_len && Cse.current_cycle) {
      XdrvMailbox.payload = (uint32_t)(value * Cse.current_cycle) / 1000;
    }
  }
  else serviced = false;  // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg02(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_LOOP:
      if (CseSerial) { CseSerialInput(); }
      break;
    case FUNC_EVERY_SECOND:
      CseEverySecond();
      break;
    case FUNC_COMMAND:
      result = CseCommand();
      break;
    case FUNC_INIT:
      CseSnsInit();
      break;
    case FUNC_PRE_INIT:
      CseDrvInit();
      break;
  }
  return result;
}

#endif  // USE_CSE7766
#endif  // USE_ENERGY_SENSOR
