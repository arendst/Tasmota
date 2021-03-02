/*
  xnrg_19_cse7761.ino - CSE7761 energy sensor support for Tasmota

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
#ifdef USE_CSE7761
/*********************************************************************************************\
 * CSE7761 - Energy  (Sonoff Dual R3 Pow)
 *
 * Based on datasheet from ChipSea
\*********************************************************************************************/

#define XNRG_19             19

#define CSE7761_K1          2             // Current channel sampling resistance in milli Ohm
#define CSE7761_K2          2             // Voltage divider resistance in 1k/1M

#define CSE7761_2POWER22    4194304
#define CSE7761_2POWER23    8388608
#define CSE7761_2POWER31    2147483648

enum CSE7761 { RmsIAC, RmsIBC, RmsUC, PowerPAC, PowerPBC, PowerSC, EnergyAc, EnergyBC };

#include <TasmotaSerial.h>

TasmotaSerial *Cse7761Serial = nullptr;

struct {
  uint32_t voltage_rms = 0;
  uint32_t current_rms[2] = { 0 };
  uint32_t active_power[2] = { 0 };
  uint16_t coefficient[8] = { 0 };
  uint8_t init = 0;
  bool found = false;
} CSE7761Data;

void Cse7761Write(uint32_t reg, uint32_t data) {
  uint8_t buffer[5];

  buffer[0] = 0xA5;
  buffer[1] = reg;
  uint32_t len = 2;
  if (data) {
    if (data < 0xFF) {
      buffer[2] = data & 0xFF;
      len = 3;
    } else {
      buffer[2] = (data >> 8) & 0xFF;
      buffer[3] = data & 0xFF;
      len = 4;
    }
    uint8_t crc = 0;
    for (uint32_t i = 0; i < len; i++) {
      crc += buffer[i];
    }
    buffer[len] = ~crc;
    len++;
  }
  Cse7761Serial->write(buffer, len);

  AddLog(LOG_LEVEL_DEBUG, PSTR("C61: Send %d, Data %*_H"), len, len, buffer);
}

uint32_t Cse7761Read(uint32_t reg, uint32_t request) {
  delay(3);
  Cse7761Write(reg, 0);

  uint8_t buffer[5];
  uint32_t rcvd = 0;
  uint32_t timeout = millis() + 3;
  while (!TimeReached(timeout) && (rcvd <= request) && (rcvd <= sizeof(buffer))) {
    int value = Cse7761Serial->read();
    if (value > -1) {
      buffer[rcvd++] = value;
    }
  }

  if (!rcvd) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("C61: Rcvd %d"), rcvd);
    return 0;
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR("C61: Rcvd %d, Data %*_H"), rcvd, rcvd, buffer);

  int result = 0;
  uint8_t crc = 0xA5 + reg;
  for (uint32_t i = 0; i < rcvd -1; i++) {
    result = (result << 8) | buffer[i];
    crc += buffer[i];
  }
  crc = ~crc;
  if (crc != buffer[rcvd]) {
    result = 0;
  }

  return result;
}

bool Cse7761ChipInit(void) {
  uint16_t calc_chksum = 0xFFFF;
  for (uint32_t i = 0; i < 8; i++) {
    CSE7761Data.coefficient[i] = Cse7761Read(0x70 + i, 2);
    calc_chksum += CSE7761Data.coefficient[i];
  }
  uint16_t dummy = Cse7761Read(0x6E, 2);
  uint16_t coeff_chksum = Cse7761Read(0x6F, 2);
  if (calc_chksum != coeff_chksum) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("C61: Coefficients invalid"));
//    return false;
  }

  delay(3);
  Cse7761Write(0xEA, 0xE5);      // Enable write operation
  delay(5);
  uint8_t sys_status = Cse7761Read(0x43, 1);
  if (sys_status & 0x10) {       // Write enable to protected registers (WREN)
    delay(3);
/*
    System Control Register (SYSCON)  Addr:0x00  Default value: 0x0A04
    Bit    name               Function description
    15-11  NC                 -, the default is 1
    10     ADC2ON
                              =1, means ADC current channel B is on (Sonoff Dual R3 Pow)
                              =0, means ADC current channel B is closed
    9      NC                 -, the default is 1.
    8-6    PGAIB[2:0]         Current channel B analog gain selection highest bit
                              =1XX, PGA of current channel B=16
                              =011, PGA of current channel B=8
                              =010, PGA of current channel B=4
                              =001, PGA of current channel B=2
                              =000, PGA of current channel B=1 (Sonoff Dual R3 Pow)
    5-3    PGAU[2:0]          Highest bit of voltage channel analog gain selection
                              =1XX, PGA of current channel U=16
                              =011, PGA of current channel U=8
                              =010, PGA of current channel U=4
                              =001, PGA of current channel U=2 (Sonoff Dual R3 Pow)
                              =000, PGA of current channel U=1
    2-0    PGAIA[2:0]         Current channel A analog gain selection highest bit
                              =1XX, PGA of current channel A=16
                              =011, PGA of current channel A=8
                              =010, PGA of current channel A=4
                              =001, PGA of current channel A=2
                              =000, PGA of current channel A=1 (Sonoff Dual R3 Pow)
*/
    Cse7761Write(0x80, 0xFF04);  // Set SYSCON
/*
    Energy Measure Control Register (EMUCON)  Addr:0x01  Default value: 0x0000
    Bit    name               Function description
    15-14  Tsensor_Step[1:0]  Measurement steps of temperature sensor:
                              =2'b00 The first step of temperature sensor measurement, the Offset of OP1 and OP2 is +/+. (Sonoff Dual R3 Pow)
                              =2'b01 The second step of temperature sensor measurement, the Offset of OP1 and OP2 is +/-.
                              =2'b10 The third step of temperature sensor measurement, the Offset of OP1 and OP2 is -/+.
                              =2'b11 The fourth step of temperature sensor measurement, the Offset of OP1 and OP2 is -/-.
                              After measuring these four results and averaging, the AD value of the current measured temperature can be obtained.
    13     tensor_en          Temperature measurement module control
                              =0 when the temperature measurement module is closed; (Sonoff Dual R3 Pow)
                              =1 when the temperature measurement module is turned on;
    12     comp_off           Comparator module close signal:
                              =0 when the comparator module is in working state
                              =1 when the comparator module is off (Sonoff Dual R3 Pow)
    11-10  Pmode[1:0]         Selection of active energy calculation method:
                              Pmode =00, both positive and negative active energy participate in the accumulation,
                                the accumulation method is algebraic sum mode, the reverse REVQ symbol indicates to active power; (Sonoff Dual R3 Pow)
                              Pmode = 01, only accumulate positive active energy;
                              Pmode = 10, both positive and negative active energy participate in the accumulation,
                                and the accumulation method is absolute value method. No reverse active power indication;
                              Pmode =11, reserved, the mode is the same as Pmode =00
    9      NC                 -
    8      ZXD1               The initial value of ZX output is 0, and different waveforms are output according to the configuration of ZXD1 and ZXD0:
                              =0, it means that the ZX output changes only at the selected zero-crossing point (Sonoff Dual R3 Pow)
                              =1, indicating that the ZX output changes at both the positive and negative zero crossings
    7      ZXD0
                              =0, indicates that the positive zero-crossing point is selected as the zero-crossing detection signal (Sonoff Dual R3 Pow)
                              =1, indicating that the negative zero-crossing point is selected as the zero-crossing detection signal
    6      HPFIBOFF
                              =0, enable current channel B digital high-pass filter (Sonoff Dual R3 Pow)
                              =1, turn off the digital high-pass filter of current channel B
    5      HPFIAOFF
                              =0, enable current channel A digital high-pass filter (Sonoff Dual R3 Pow)
                              =1, turn off the digital high-pass filter of current channel A
    4      HPFUOFF
                              =0, enable U channel digital high pass filter (Sonoff Dual R3 Pow)
                              =1, turn off the U channel digital high-pass filter
    3-2    NC                 -
    1      PBRUN
                              =1, enable PFB pulse output and active energy register accumulation; (Sonoff Dual R3 Pow)
                              =0 (default), turn off PFB pulse output and active energy register accumulation.
    0      PARUN
                              =1, enable PFA pulse output and active energy register accumulation; (Sonoff Dual R3 Pow)
                              =0 (default), turn off PFA pulse output and active energy register accumulation.
*/
    Cse7761Write(0x81, 0x1003);  // Set EMUCON
/*
    Energy Measure Control Register (EMUCON2)  Addr: 0x13  Default value: 0x0001
    Bit    name               Function description
    15-13  NC                 -
    12     SDOCmos
                              =1, SDO pin CMOS open-drain output (Sonoff Dual R3 Pow)
                              =0, SDO pin CMOS output
    11     EPB_CB             Energy_PB clear signal control, the default is 0, and it needs to be configured to 1 in UART mode.
                                Clear after reading is not supported in UART mode
                              =1, Energy_PB will not be cleared after reading; (Sonoff Dual R3 Pow)
                              =0, Energy_PB is cleared after reading;
    10     EPA_CB             Energy_PA clear signal control, the default is 0, it needs to be configured to 1 in UART mode,
                                Clear after reading is not supported in UART mode
                              =1, Energy_PA will not be cleared after reading; (Sonoff Dual R3 Pow)
                              =0, Energy_PA is cleared after reading;
    9-8    DUPSEL[1:0]        Average register update frequency control
                              =00, Update frequency 3.4Hz
                              =01, Update frequency 6.8Hz
                              =10, Update frequency 13.65Hz
                              =11, Update frequency 27.3Hz (Sonoff Dual R3 Pow)
    7      CHS_IB             Current channel B measurement selection signal
                              =1, measure the current of channel B (Sonoff Dual R3 Pow)
                              =0, measure the internal temperature of the chip
    6      PfactorEN          Power factor function enable
                              =1, turn on the power factor output function (Sonoff Dual R3 Pow)
                              =0, turn off the power factor output function
    5      WaveEN             Waveform data, instantaneous data output enable signal
                              =1, turn on the waveform data output function
                              =0, turn off the waveform data output function (Sonoff Dual R3 Pow)
    4      SAGEN              Voltage drop detection enable signal, WaveEN=1 must be configured first
                              =1, turn on the voltage drop detection function
                              =0, turn off the voltage drop detection function (Sonoff Dual R3 Pow)
    3      OverEN             Overvoltage, overcurrent, and overload detection enable signal, WaveEN=1 must be configured first
                              =1, turn on the overvoltage, overcurrent, and overload detection functions
                              =0, turn off the overvoltage, overcurrent, and overload detection functions (Sonoff Dual R3 Pow)
    2      ZxEN               Zero-crossing detection, phase angle, voltage frequency measurement enable signal
                              =1, turn on the zero-crossing detection, phase angle, and voltage frequency measurement functions
                              =0, disable zero-crossing detection, phase angle, voltage frequency measurement functions (Sonoff Dual R3 Pow)
    1      PeakEN             Peak detect enable signal
                              =1, turn on the peak detection function
                              =0, turn off the peak detection function (Sonoff Dual R3 Pow)
    0      NC                 Default is 1
*/
    Cse7761Write(0x93, 0x0FC1);  // Set EMUCON2
  } else {
    AddLog(LOG_LEVEL_DEBUG, PSTR("C61: Write enable failed"));
//    return false;
  }
  delay(80);
  Cse7761Write(0xEA, 0xDC);      // Close write operation
  return true;
}

void Cse7761GetData(void) {
  CSE7761Data.voltage_rms = Cse7761Read(0x26, 3);
  CSE7761Data.current_rms[0] = Cse7761Read(0x24, 3);
  CSE7761Data.active_power[0] = Cse7761Read(0x2C, 4);
  CSE7761Data.current_rms[1] = Cse7761Read(0x25, 3);
  CSE7761Data.active_power[1] = Cse7761Read(0x2D, 4);

  if (Energy.power_on) {  // Powered on
    Energy.voltage[0] = ((float)CSE7761Data.voltage_rms * ((double)CSE7761Data.coefficient[RmsUC] / (CSE7761_K2 * 2 * CSE7761_2POWER22))) / 1000;  // V

    for (uint32_t channel = 0; channel < 2; channel++) {
      Energy.data_valid[channel] = 0;
      Energy.active_power[channel] = (float)CSE7761Data.active_power[channel] * ((double)CSE7761Data.coefficient[PowerPAC + channel] / (CSE7761_K1 * CSE7761_K2 * 2 * CSE7761_2POWER31));  // W
      if (0 == Energy.active_power[channel]) {
        Energy.current[channel] = 0;
      } else {
        Energy.current[channel] = (float)CSE7761Data.current_rms[channel] * ((double)CSE7761Data.coefficient[RmsIAC + channel] / (CSE7761_K1 * 2 * CSE7761_2POWER23));  // mA
      }
    }

    uint32_t active_power_sum = (Energy.active_power[0] + Energy.active_power[1]) * 1000;
    if (active_power_sum) {
      Energy.kWhtoday_delta += active_power_sum / 36;
      EnergyUpdateToday();
    }
  } else {  // Powered off
    Energy.data_valid[0] = ENERGY_WATCHDOG;
    Energy.data_valid[1] = ENERGY_WATCHDOG;
  }
}

/********************************************************************************************/

void Cse7761EverySecond(void) {
  if (CSE7761Data.init) {
    if (2 == CSE7761Data.init) {
      Cse7761Write(0xEA, 0x96);                 // Reset chip
    }
    else if (1 == CSE7761Data.init) {
      uint16_t syscon = Cse7761Read(0x00, 2);   // Default 0x0A04
      if (0x0A04 == syscon) {
        CSE7761Data.found = Cse7761ChipInit();
      }
      if (CSE7761Data.found) {
        AddLog(LOG_LEVEL_INFO, PSTR("C61: CSE7761 found"));
      }
    }
    CSE7761Data.init--;
  }
  else {
    if (CSE7761Data.found) {
      Cse7761GetData();
    }
  }
}

void Cse7761SnsInit(void) {
  // Software serial init needs to be done here as earlier (serial) interrupts may lead to Exceptions
  Cse7761Serial = new TasmotaSerial(Pin(GPIO_CSE7766_RX), Pin(GPIO_CSE7766_TX), 1);
  if (Cse7761Serial->begin(38400, SERIAL_8E1)) {
    if (Cse7761Serial->hardwareSerial()) {
//      SetSerial(38400, TS_SERIAL_8E1);
      ClaimSerial();
    }
  } else {
    TasmotaGlobal.energy_driver = ENERGY_NONE;
  }
}

void Cse7761DrvInit(void) {
  if (PinUsed(GPIO_CSE7761_RX) && PinUsed(GPIO_CSE7761_TX)) {
    CSE7761Data.found = false;
    CSE7761Data.init = 3;                       // Init setup steps
    Energy.phase_count = 2;                     // Handle two channels as two phases
    Energy.voltage_common = true;               // Use common voltage
    TasmotaGlobal.energy_driver = XNRG_19;
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg19(uint8_t function) {
  bool result = false;

  switch (function) {
    case FUNC_EVERY_SECOND:
      Cse7761EverySecond();
      break;
    case FUNC_INIT:
      Cse7761SnsInit();
      break;
    case FUNC_PRE_INIT:
      Cse7761DrvInit();
      break;
  }
  return result;
}

#endif  // USE_CSE7761
#endif  // USE_ENERGY_SENSOR
