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
 * {"NAME":"Sonoff Dual R3","GPIO":[32,0,0,0,0,0,0,0,0,576,225,0,0,0,0,0,0,0,0,0,0,7296,7328,224,0,0,0,0,160,161,0,0,0,0,0,0],"FLAG":0,"BASE":1}
 *
 * Based on datasheet from ChipSea and analysing serial data
 * See https://github.com/arendst/Tasmota/discussions/10793
\*********************************************************************************************/

#define XNRG_19                    19

//#define CSE7761_SIMULATE

#define CSE7761_UREF               42563        // RmsUc
#define CSE7761_IREF               52241        // RmsIAC
#define CSE7761_PREF               44513        // PowerPAC

#define CSE7761_REG_SYSCON         0x00         // System Control Register
#define CSE7761_REG_EMUCON         0x01         // Metering control register
#define CSE7761_REG_EMUCON2        0x13         // Metering control register 2

#define CSE7761_REG_UFREQ          0x23         // Voltage Frequency Register
#define CSE7761_REG_RMSIA          0x24         // The effective value of channel A current
#define CSE7761_REG_RMSIB          0x25         // The effective value of channel B current
#define CSE7761_REG_RMSU           0x26         // Voltage RMS
#define CSE7761_REG_POWERPA        0x2C         // Channel A active power, update rate 27.2Hz
#define CSE7761_REG_POWERPB        0x2D         // Channel B active power, update rate 27.2Hz
#define CSE7761_REG_SYSSTATUS      0x43         // System status register

#define CSE7761_REG_COEFFOFFSET    0x6E         // Coefficient checksum offset (0xFFFF)
#define CSE7761_REG_COEFFCHKSUM    0x6F         // Coefficient checksum
#define CSE7761_REG_RMSIAC         0x70         // Channel A effective current conversion coefficient
#define CSE7761_REG_RMSIBC         0x71         // Channel B effective current conversion coefficient
#define CSE7761_REG_RMSUC          0x72         // Effective voltage conversion coefficient
#define CSE7761_REG_POWERPAC       0x73         // Channel A active power conversion coefficient
#define CSE7761_REG_POWERPBC       0x74         // Channel B active power conversion coefficient
#define CSE7761_REG_POWERSC        0x75         // Apparent power conversion coefficient
#define CSE7761_REG_ENERGYAC       0x76         // Channel A energy conversion coefficient
#define CSE7761_REG_ENERGYBC       0x77         // Channel B energy conversion coefficient

#define CSE7761_SPECIAL_COMMAND    0xEA         // Start special command
#define CSE7761_CMD_RESET          0x96         // Reset command, after receiving the command, the chip resets
#define CSE7761_CMD_CHAN_A_SELECT  0x5A         // Current channel A setting command, which specifies the current used to calculate apparent power,
                                                //   Power factor, phase angle, instantaneous active power, instantaneous apparent power and
                                                //   The channel indicated by the signal of power overload is channel A
#define CSE7761_CMD_CHAN_B_SELECT  0xA5         // Current channel B setting command, which specifies the current used to calculate apparent power,
                                                //   Power factor, phase angle, instantaneous active power, instantaneous apparent power and
                                                //   The channel indicated by the signal of power overload is channel B
#define CSE7761_CMD_CLOSE_WRITE    0xDC         // Close write operation
#define CSE7761_CMD_ENABLE_WRITE   0xE5         // Enable write operation

enum CSE7761 { RmsIAC, RmsIBC, RmsUC, PowerPAC, PowerPBC, PowerSC, EnergyAC, EnergyBC };

#include <TasmotaSerial.h>

TasmotaSerial *Cse7761Serial = nullptr;

struct {
  uint32_t voltage_rms = 0;
  uint32_t current_rms[2] = { 0 };
  uint32_t energy[2] = { 0 };
  uint32_t active_power[2] = { 0 };
  uint16_t coefficient[8] = { 0 };
  uint8_t energy_update = 0;
  uint8_t init = 4;
  uint8_t ready = 0;
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

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("C61: Tx %*_H"), len, buffer);
}

uint32_t Cse7761ReadOnce(uint32_t log_level, uint32_t reg, uint32_t size) {
  while (Cse7761Serial->available()) { Cse7761Serial->read(); }

  Cse7761Write(reg, 0);

  uint8_t buffer[8] = { 0 };
  uint32_t rcvd = 0;
  uint32_t timeout = millis() + 3;

  while (!TimeReached(timeout) && (rcvd <= size)) {
//  while (!TimeReached(timeout)) {
    int value = Cse7761Serial->read();
    if ((value > -1) && (rcvd < sizeof(buffer) -1)) {
      buffer[rcvd++] = value;
    }
  }

  if (!rcvd) {
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("C61: Rx none"));
    return 0;
  }
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("C61: Rx %*_H"), rcvd, buffer);
  if (rcvd > 5) {
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("C61: Rx overflow"));
    return 0;
  }

  rcvd--;
  uint32_t result = 0;
  uint8_t crc = 0xA5 + reg;
  for (uint32_t i = 0; i < rcvd; i++) {
    result = (result << 8) | buffer[i];
    crc += buffer[i];
  }
  crc = ~crc;
  if (crc != buffer[rcvd]) {
    AddLog(log_level, PSTR("C61: Rx %*_H, CRC error %02X"), rcvd +1, buffer, crc);
    return 1;
  }

  return result;
}

uint32_t Cse7761Read(uint32_t reg, uint32_t size) {
  uint32_t value = 0;
  uint32_t retry = 3;
  while ((value < 2) && (retry)) {
    retry--;
    value = Cse7761ReadOnce((retry) ? LOG_LEVEL_DEBUG_MORE : LOG_LEVEL_DEBUG, reg, size);
  }
  return value;
}

uint32_t Cse7761ReadFallback(uint32_t reg, uint32_t prev, uint32_t size) {
  uint32_t value = Cse7761Read(reg, size);
  if (1 == value) {  // CRC Error so use previous value read
    value = prev;
  }
  return value;
}

uint32_t Cse7761Ref(uint32_t unit) {
  switch (unit) {
    case RmsUC: return 0x400000 * 100 / CSE7761Data.coefficient[RmsUC];
    case RmsIAC: return (0x800000 * 100 / CSE7761Data.coefficient[RmsIAC]) * 10;  // Stay within 32 bits
    case PowerPAC: return 0x80000000 / CSE7761Data.coefficient[PowerPAC];
  }
  return 0;
}

bool Cse7761ChipInit(void) {
  uint16_t calc_chksum = 0xFFFF;
  for (uint32_t i = 0; i < 8; i++) {
    CSE7761Data.coefficient[i] = Cse7761Read(CSE7761_REG_RMSIAC + i, 2);
    calc_chksum += CSE7761Data.coefficient[i];
  }
  calc_chksum = ~calc_chksum;
//  uint16_t dummy = Cse7761Read(CSE7761_REG_COEFFOFFSET, 2);
  uint16_t coeff_chksum = Cse7761Read(CSE7761_REG_COEFFCHKSUM, 2);
  if ((calc_chksum != coeff_chksum) || (!calc_chksum)) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("C61: Default calibration"));
    CSE7761Data.coefficient[RmsIAC] = CSE7761_IREF;
//    CSE7761Data.coefficient[RmsIBC] = 0xCC05;
    CSE7761Data.coefficient[RmsUC] = CSE7761_UREF;
    CSE7761Data.coefficient[PowerPAC] = CSE7761_PREF;
//    CSE7761Data.coefficient[PowerPBC] = 0xADD7;
  }
  if (HLW_PREF_PULSE == Settings.energy_power_calibration) {
    Settings.energy_voltage_calibration = Cse7761Ref(RmsUC);
    Settings.energy_current_calibration = Cse7761Ref(RmsIAC);
    Settings.energy_power_calibration = Cse7761Ref(PowerPAC);
  }

  Cse7761Write(CSE7761_SPECIAL_COMMAND, CSE7761_CMD_ENABLE_WRITE);

//  delay(8);  // Exception on ESP8266
//  uint32_t timeout = millis() + 8;
//  while (!TimeReached(timeout)) { }

  uint8_t sys_status = Cse7761Read(CSE7761_REG_SYSSTATUS, 1);
#ifdef CSE7761_SIMULATE
  sys_status = 0x11;
#endif
  if (sys_status & 0x10) {       // Write enable to protected registers (WREN)
/*
    System Control Register (SYSCON)  Addr:0x00  Default value: 0x0A04
    Bit    name               Function description
    15-11  NC                 -, the default is 1
    10     ADC2ON
                              =1, means ADC current channel B is on (Sonoff Dual R3 Pow)
                              =0, means ADC current channel B is closed
    9      NC                 -, the default is 1.
    8-6    PGAIB[2:0]         Current channel B analog gain selection highest bit
                              =1XX, PGA of current channel B=16 (Sonoff Dual R3 Pow)
                              =011, PGA of current channel B=8
                              =010, PGA of current channel B=4
                              =001, PGA of current channel B=2
                              =000, PGA of current channel B=1
    5-3    PGAU[2:0]          Highest bit of voltage channel analog gain selection
                              =1XX, PGA of voltage U=16
                              =011, PGA of voltage U=8
                              =010, PGA of voltage U=4
                              =001, PGA of voltage U=2
                              =000, PGA of voltage U=1 (Sonoff Dual R3 Pow)
    2-0    PGAIA[2:0]         Current channel A analog gain selection highest bit
                              =1XX, PGA of current channel A=16 (Sonoff Dual R3 Pow)
                              =011, PGA of current channel A=8
                              =010, PGA of current channel A=4
                              =001, PGA of current channel A=2
                              =000, PGA of current channel A=1
*/
    Cse7761Write(CSE7761_REG_SYSCON | 0x80, 0xFF04);
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
    Cse7761Write(CSE7761_REG_EMUCON | 0x80, 0x1003);
/*
    Energy Measure Control Register (EMUCON2)  Addr: 0x13  Default value: 0x0001
    Bit    name               Function description
    15-13  NC                 -
    12     SDOCmos
                              =1, SDO pin CMOS open-drain output
                              =0, SDO pin CMOS output (Sonoff Dual R3 Pow)
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
    Cse7761Write(CSE7761_REG_EMUCON2 | 0x80, 0x0FC1);
  } else {
    AddLog(LOG_LEVEL_DEBUG, PSTR("C61: Write failed"));
    return false;
  }
  return true;
}

void Cse7761GetData(void) {
  // The effective value of current and voltage Rms is a 24-bit signed number, the highest bit is 0 for valid data,
  //   and when the highest bit is 1, the reading will be processed as zero
  // The active power parameter PowerA/B is in two’s complement format, 32-bit data, the highest bit is Sign bit.
  uint32_t value = Cse7761ReadFallback(CSE7761_REG_RMSU, CSE7761Data.voltage_rms, 3);
#ifdef CSE7761_SIMULATE
  value = 2342160;  // 237.7V
#endif
  CSE7761Data.voltage_rms = (value >= 0x800000) ? 0 : value;

  value = Cse7761ReadFallback(CSE7761_REG_RMSIA, CSE7761Data.current_rms[0], 3);
#ifdef CSE7761_SIMULATE
  value = 455;
#endif
  CSE7761Data.current_rms[0] = ((value >= 0x800000) || (value < 1600)) ? 0 : value;  // No load threshold of 10mA
  value = Cse7761ReadFallback(CSE7761_REG_POWERPA, CSE7761Data.active_power[0], 4);
#ifdef CSE7761_SIMULATE
  value = 217;
#endif
  CSE7761Data.active_power[0] = (0 == CSE7761Data.current_rms[0]) ? 0 : (value & 0x80000000) ? (~value) + 1 : value;

  value = Cse7761ReadFallback(CSE7761_REG_RMSIB, CSE7761Data.current_rms[1], 3);
#ifdef CSE7761_SIMULATE
  value = 29760;  // 0.185A
#endif
  CSE7761Data.current_rms[1] = ((value >= 0x800000) || (value < 1600)) ? 0 : value;  // No load threshold of 10mA
  value = Cse7761ReadFallback(CSE7761_REG_POWERPB, CSE7761Data.active_power[1], 4);
#ifdef CSE7761_SIMULATE
  value = 2126641;  // 44.05W
#endif
  CSE7761Data.active_power[1] = (0 == CSE7761Data.current_rms[1]) ? 0 : (value & 0x80000000) ? (~value) + 1 : value;

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("C61: U%d, I%d/%d, P%d/%d"),
    CSE7761Data.voltage_rms,
    CSE7761Data.current_rms[0], CSE7761Data.current_rms[1],
    CSE7761Data.active_power[0], CSE7761Data.active_power[1]);

  if (Energy.power_on) {  // Powered on
    // Voltage = RmsU * RmsUC * 10 / 0x400000
    // Energy.voltage[0] = (float)(((uint64_t)CSE7761Data.voltage_rms * CSE7761Data.coefficient[RmsUC] * 10) >> 22) / 1000;  // V
    Energy.voltage[0] = ((float)CSE7761Data.voltage_rms / Settings.energy_voltage_calibration);  // V

    for (uint32_t channel = 0; channel < 2; channel++) {
      Energy.data_valid[channel] = 0;
      // Active power = PowerPA * PowerPAC * 1000 / 0x80000000
      // Energy.active_power[channel] = (float)(((uint64_t)CSE7761Data.active_power[channel] * CSE7761Data.coefficient[PowerPAC + channel] * 1000) >> 31) / 1000;  // W
      Energy.active_power[channel] = (float)CSE7761Data.active_power[channel] / Settings.energy_power_calibration;  // W
      if (0 == Energy.active_power[channel]) {
        Energy.current[channel] = 0;
      } else {
        // Current = RmsIA * RmsIAC / 0x800000
        // Energy.current[channel] = (float)(((uint64_t)CSE7761Data.current_rms[channel] * CSE7761Data.coefficient[RmsIAC + channel]) >> 23) / 1000;  // A
        Energy.current[channel] = (float)CSE7761Data.current_rms[channel] / Settings.energy_current_calibration;  // A
        CSE7761Data.energy[channel] += Energy.active_power[channel];
        CSE7761Data.energy_update++;
      }
    }
  }
}

/********************************************************************************************/

void Cse7761Every200ms(void) {
  if (2 == CSE7761Data.ready) {
    Cse7761GetData();
  }
}

void Cse7761EverySecond(void) {
  if (CSE7761Data.init) {
    if (3 == CSE7761Data.init) {
      Cse7761Write(CSE7761_SPECIAL_COMMAND, CSE7761_CMD_RESET);
    }
    else if (2 == CSE7761Data.init) {
      uint16_t syscon = Cse7761Read(0x00, 2);      // Default 0x0A04
#ifdef CSE7761_SIMULATE
      syscon = 0x0A04;
#endif
      if ((0x0A04 == syscon) && Cse7761ChipInit()) {
        CSE7761Data.ready = 1;
      }
    }
    else if (1 == CSE7761Data.init) {
      if (1 == CSE7761Data.ready) {
        Cse7761Write(CSE7761_SPECIAL_COMMAND, CSE7761_CMD_CLOSE_WRITE);
        AddLog(LOG_LEVEL_INFO, PSTR("C61: CSE7761 found"));
        CSE7761Data.ready = 2;
      }
    }
    CSE7761Data.init--;
  }
  else {
    if (2 == CSE7761Data.ready) {
      if (CSE7761Data.energy_update) {
        uint32_t energy_sum = ((CSE7761Data.energy[0] + CSE7761Data.energy[1]) * 1000) / CSE7761Data.energy_update;
        if (energy_sum) {
          Energy.kWhtoday_delta += energy_sum / 36;
          EnergyUpdateToday();
        }
      }
      CSE7761Data.energy[0] = 0;
      CSE7761Data.energy[1] = 0;
      CSE7761Data.energy_update = 0;
    }
  }
}

void Cse7761SnsInit(void) {
  // Software serial init needs to be done here as earlier (serial) interrupts may lead to Exceptions
  Cse7761Serial = new TasmotaSerial(Pin(GPIO_CSE7761_RX), Pin(GPIO_CSE7761_TX), 1);
  if (Cse7761Serial->begin(38400, SERIAL_8E1)) {
    if (Cse7761Serial->hardwareSerial()) {
      SetSerial(38400, TS_SERIAL_8E1);
      ClaimSerial();
    }
  } else {
    TasmotaGlobal.energy_driver = ENERGY_NONE;
  }
}

void Cse7761DrvInit(void) {
  if (PinUsed(GPIO_CSE7761_RX) && PinUsed(GPIO_CSE7761_TX)) {
    CSE7761Data.ready = 0;
    CSE7761Data.init = 4;                       // Init setup steps
    Energy.phase_count = 2;                     // Handle two channels as two phases
    Energy.voltage_common = true;               // Use common voltage
    TasmotaGlobal.energy_driver = XNRG_19;
  }
}

bool Cse7761Command(void) {
  bool serviced = true;

  uint32_t channel = (2 == XdrvMailbox.index) ? 1 : 0;
  uint32_t value = (uint32_t)(CharToFloat(XdrvMailbox.data) * 100);  // 1.23 = 123

  if (CMND_POWERCAL == Energy.command_code) {
    if (1 == XdrvMailbox.payload) { XdrvMailbox.payload = Cse7761Ref(PowerPAC); }
    // Service in xdrv_03_energy.ino
  }
  else if (CMND_VOLTAGECAL == Energy.command_code) {
    if (1 == XdrvMailbox.payload) { XdrvMailbox.payload = Cse7761Ref(RmsUC); }
    // Service in xdrv_03_energy.ino
  }
  else if (CMND_CURRENTCAL == Energy.command_code) {
    if (1 == XdrvMailbox.payload) { XdrvMailbox.payload = Cse7761Ref(RmsIAC); }
    // Service in xdrv_03_energy.ino
  }
  else if (CMND_POWERSET == Energy.command_code) {
    if (XdrvMailbox.data_len && CSE7761Data.active_power[channel]) {
      if ((value > 100) && (value < 200000)) {  // Between 1W and 2000W
        Settings.energy_power_calibration = ((CSE7761Data.active_power[channel]) / value) * 100;
      }
    }
  }
  else if (CMND_VOLTAGESET == Energy.command_code) {
    if (XdrvMailbox.data_len && CSE7761Data.voltage_rms) {
      if ((value > 10000) && (value < 26000)) {  // Between 100V and 260V
        Settings.energy_voltage_calibration = (CSE7761Data.voltage_rms * 100) / value;
      }
    }
  }
  else if (CMND_CURRENTSET == Energy.command_code) {
    if (XdrvMailbox.data_len && CSE7761Data.current_rms[channel]) {
      if ((value > 1000) && (value < 1000000)) {  // Between 10mA and 10A
        Settings.energy_current_calibration = ((CSE7761Data.current_rms[channel] * 100) / value) * 1000;
      }
    }
  }
  else serviced = false;  // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg19(uint8_t function) {
  bool result = false;

  switch (function) {
    case FUNC_EVERY_200_MSECOND:
      Cse7761Every200ms();
      break;
    case FUNC_ENERGY_EVERY_SECOND:
      Cse7761EverySecond();
      break;
    case FUNC_COMMAND:
      result = Cse7761Command();
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
