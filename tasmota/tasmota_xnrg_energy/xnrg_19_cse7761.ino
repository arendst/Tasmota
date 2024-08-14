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
 * CSE7761 - Energy  (Sonoff Dual R3 and Pow CT)
 *
 * Without zero-cross detection
 * {"NAME":"Sonoff Dual R3","GPIO":[32,0,0,0,0,0,0,0,0,576,225,0,0,0,0,0,0,0,0,0,0,7296,7328,224,0,0,0,0,160,161,0,0,0,0,0,0],"FLAG":0,"BASE":1}
 *
 * With zero-cross detection
 * {"NAME":"Sonoff Dual R3 (ZCD)","GPIO":[32,0,0,0,7552,0,0,0,0,576,225,0,0,0,0,0,0,0,0,0,0,7296,7328,224,0,0,0,0,160,161,0,0,0,0,0,0],"FLAG":0,"BASE":1}
 *
 * {"NAME":"Sonoff POWCT","GPIO":[32,0,0,0,0,9280,0,0,0,288,0,576,0,9184,9216,0,0,224,0,9248,0,7329,7296,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
 * 
 * Based on datasheet from ChipSea and analysing serial data
 * See https://github.com/arendst/Tasmota/discussions/10793
 * https://goldenrelay.en.alibaba.com/product/62119012875-811845870/GOLDEN_GI_1A_5LH_SPST_5V_5A_10A_250VAC_NO_18_5_10_5_15_3mm_sealed_type_all_certificate_compliances_class_F_SPDT_Form_available.html
\*********************************************************************************************/

#define XNRG_19                       19

//#define CSE7761_SIMULATE                           // Enable simulation of CSE7761
#define CSE7761_FREQUENCY                          // Add support for frequency monitoring
  #define CSE7761_ZEROCROSS                        // Add zero cross detection
    #define CSE7761_ZEROCROSS_OFFSET  2200         // Zero cross offset due to chip calculation (microseconds)
    #define CSE7761_RELAY_SWITCHTIME  3950         // Relay (Golden GI-1A-5LH 15ms max) switch power on time (microseconds)

#define CSE7761_UREF                  42563        // RmsUc
#define CSE7761_IREF                  52241        // RmsIAC
#define CSE7761_PREF                  44513        // PowerPAC
#define CSE7761_FREF                  3579545      // System clock (3.579545MHz) as used in frequency calculation

#define CSE7761_REG_SYSCON            0x00         // (2) System Control Register (0x0A04)
#define CSE7761_REG_EMUCON            0x01         // (2) Metering control register (0x0000)
#define CSE7761_REG_EMUCON2           0x13         // (2) Metering control register 2 (0x0001)
#define CSE7761_REG_PULSE1SEL         0x1D         // (2) Pin function output select register (0x3210)

#define CSE7761_REG_UFREQ             0x23         // (2) Voltage Frequency (0x0000)
#define CSE7761_REG_RMSIA             0x24         // (3) The effective value of channel A current (0x000000)
#define CSE7761_REG_RMSIB             0x25         // (3) The effective value of channel B current (0x000000)
#define CSE7761_REG_RMSU              0x26         // (3) Voltage RMS (0x000000)
#define CSE7761_REG_POWERFACTOR       0x27         // (3) Power factor register, select by command: channel A Power factor or channel B power factor (0x7FFFFF)
#define CSE7761_REG_POWERPA           0x2C         // (4) Channel A active power, update rate 27.2Hz (0x00000000)
#define CSE7761_REG_POWERPB           0x2D         // (4) Channel B active power, update rate 27.2Hz (0x00000000)
#define CSE7761_REG_SYSSTATUS         0x43         // (1) System status register

#define CSE7761_REG_COEFFOFFSET       0x6E         // (2) Coefficient checksum offset (0xFFFF)
#define CSE7761_REG_COEFFCHKSUM       0x6F         // (2) Coefficient checksum
#define CSE7761_REG_RMSIAC            0x70         // (2) Channel A effective current conversion coefficient
#define CSE7761_REG_RMSIBC            0x71         // (2) Channel B effective current conversion coefficient
#define CSE7761_REG_RMSUC             0x72         // (2) Effective voltage conversion coefficient
#define CSE7761_REG_POWERPAC          0x73         // (2) Channel A active power conversion coefficient
#define CSE7761_REG_POWERPBC          0x74         // (2) Channel B active power conversion coefficient
#define CSE7761_REG_POWERSC           0x75         // (2) Apparent power conversion coefficient
#define CSE7761_REG_ENERGYAC          0x76         // (2) Channel A energy conversion coefficient
#define CSE7761_REG_ENERGYBC          0x77         // (2) Channel B energy conversion coefficient

#define CSE7761_SPECIAL_COMMAND       0xEA         // Start special command
#define CSE7761_CMD_RESET             0x96         // Reset command, after receiving the command, the chip resets
#define CSE7761_CMD_CHAN_A_SELECT     0x5A         // Current channel A setting command, which specifies the current used to calculate apparent power,
                                                   //   Power factor, phase angle, instantaneous active power, instantaneous apparent power and
                                                   //   The channel indicated by the signal of power overload is channel A
#define CSE7761_CMD_CHAN_B_SELECT     0xA5         // Current channel B setting command, which specifies the current used to calculate apparent power,
                                                   //   Power factor, phase angle, instantaneous active power, instantaneous apparent power and
                                                   //   The channel indicated by the signal of power overload is channel B
#define CSE7761_CMD_CLOSE_WRITE       0xDC         // Close write operation
#define CSE7761_CMD_ENABLE_WRITE      0xE5         // Enable write operation

enum CSE7761 { RmsIAC, RmsIBC, RmsUC, PowerPAC, PowerPBC, PowerSC, EnergyAC, EnergyBC };

#include <TasmotaSerial.h>

TasmotaSerial *Cse7761Serial = nullptr;

enum CSE7761Model { CSE7761_MODEL_DUALR3, CSE7761_MODEL_POWCT };  // Model index number starting from 0

struct {
  uint32_t frequency = 0;
  uint32_t voltage_rms = 0;
  uint32_t current_rms[2] = { 0 };
  uint32_t energy[2] = { 0 };
  uint32_t active_power[2] = { 0 };
  uint16_t coefficient[8] = { 0 };
  uint8_t energy_update[2] = { 0 };
  uint8_t init = 4;
  uint8_t ready = 0;
  uint8_t model;
} CSE7761Data;

/********************************************************************************************/

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

bool Cse7761ReadOnce(uint32_t log_level, uint32_t reg, uint32_t size, uint32_t* value) {
  while (Cse7761Serial->available()) { Cse7761Serial->read(); }

  Cse7761Write(reg, 0);

  uint8_t buffer[8] = { 0 };
  uint32_t rcvd = 0;
  uint32_t timeout = millis() + 6;

  while (!TimeReached(timeout) && (rcvd <= size)) {
//  while (!TimeReached(timeout)) {
    int value = Cse7761Serial->read();
    if ((value > -1) && (rcvd < sizeof(buffer) -1)) {
      buffer[rcvd++] = value;
    }
  }

  if (!rcvd) {
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("C61: Rx none"));
    return false;
  }
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("C61: Rx %*_H"), rcvd, buffer);
  if (rcvd > 5) {
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("C61: Rx overflow"));
    return false;
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
    return false;
  }

  *value = result;
  return true;
}

uint32_t Cse7761Read(uint32_t reg, uint32_t size) {
  bool result = false;  // Start loop
  uint32_t retry = 3;   // Retry up to three times
  uint32_t value = 0;   // Default no value
  while (!result && retry) {
    retry--;
    result = Cse7761ReadOnce((retry) ? LOG_LEVEL_DEBUG_MORE : LOG_LEVEL_DEBUG, reg, size, &value);
  }
  return value;
}

uint32_t Cse7761ReadFallback(uint32_t reg, uint32_t prev, uint32_t size) {
  uint32_t value = Cse7761Read(reg, size);
  if (!value) {    // Error so use previous value read
    value = prev;
  }
  return value;
}

/********************************************************************************************/

uint32_t Cse7761Ref(uint32_t unit) {
  uint32_t coeff = 1;
  if (CSE7761_MODEL_POWCT == CSE7761Data.model) {
    coeff = 5;
  }
  switch (unit) {
    case RmsUC: return 0x400000 * 100 / CSE7761Data.coefficient[RmsUC];
    case RmsIAC: return (0x800000 * 100 / (CSE7761Data.coefficient[RmsIAC] * coeff)) * 10;  // Stay within 32 bits
    case PowerPAC: return 0x80000000 / (CSE7761Data.coefficient[PowerPAC] * coeff);
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
  if (HLW_PREF_PULSE == EnergyGetCalibration(ENERGY_POWER_CALIBRATION)) {
    for (uint32_t i = 0; i < 2; i++) {
      EnergySetCalibration(ENERGY_POWER_CALIBRATION, Cse7761Ref(PowerPAC), i);
      EnergySetCalibration(ENERGY_VOLTAGE_CALIBRATION, Cse7761Ref(RmsUC), i);
      EnergySetCalibration(ENERGY_CURRENT_CALIBRATION, Cse7761Ref(RmsIAC), i);
      EnergySetCalibration(ENERGY_FREQUENCY_CALIBRATION, CSE7761_FREF, i);
    }
  }
  // Just to fix intermediate users
  if (EnergyGetCalibration(ENERGY_FREQUENCY_CALIBRATION) < CSE7761_FREF / 2) {
    EnergySetCalibration(ENERGY_FREQUENCY_CALIBRATION, CSE7761_FREF);
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
                              =1, means ADC current channel B is on (Sonoff Dual R3)
                              =0, means ADC current channel B is closed (Pow CT)
    9      NC                 -, the default is 1.
    8-6    PGAIB[2:0]         Current channel B analog gain selection highest bit
                              =1XX, PGA of current channel B=16 (Sonoff Dual R3)
                              =011, PGA of current channel B=8
                              =010, PGA of current channel B=4
                              =001, PGA of current channel B=2
                              =000, PGA of current channel B=1 (Pow CT)
    5-3    PGAU[2:0]          Highest bit of voltage channel analog gain selection
                              =1XX, PGA of voltage U=16
                              =011, PGA of voltage U=8
                              =010, PGA of voltage U=4
                              =001, PGA of voltage U=2
                              =000, PGA of voltage U=1 (Sonoff Dual R3 / Pow CT)
    2-0    PGAIA[2:0]         Current channel A analog gain selection highest bit
                              =1XX, PGA of current channel A=16 (Sonoff Dual R3)
                              =011, PGA of current channel A=8
                              =010, PGA of current channel A=4
                              =001, PGA of current channel A=2
                              =000, PGA of current channel A=1 (Pow CT)
*/
    if (CSE7761_MODEL_POWCT == CSE7761Data.model) {
//      Cse7761Write(CSE7761_REG_SYSCON | 0x80, 0x0A00);  // Pow CT
      Cse7761Write(CSE7761_REG_SYSCON | 0x80, 0xFE00);  // Pow CT - Tasmota (enable B)
    } else {      
      Cse7761Write(CSE7761_REG_SYSCON | 0x80, 0xFF04);  // Sonoff Dual R3
    }

/*
    Energy Measure Control Register (EMUCON)  Addr:0x01  Default value: 0x0000
    Bit    name               Function description
    15-14  Tsensor_Step[1:0]  Measurement steps of temperature sensor:
                              =2'b00 The first step of temperature sensor measurement, the Offset of OP1 and OP2 is +/+. (Sonoff Dual R3 / Pow CT)
                              =2'b01 The second step of temperature sensor measurement, the Offset of OP1 and OP2 is +/-.
                              =2'b10 The third step of temperature sensor measurement, the Offset of OP1 and OP2 is -/+.
                              =2'b11 The fourth step of temperature sensor measurement, the Offset of OP1 and OP2 is -/-.
                              After measuring these four results and averaging, the AD value of the current measured temperature can be obtained.
    13     tensor_en          Temperature measurement module control
                              =0 when the temperature measurement module is closed; (Sonoff Dual R3 / Pow CT)
                              =1 when the temperature measurement module is turned on;
    12     comp_off           Comparator module close signal:
                              =0 when the comparator module is in working state
                              =1 when the comparator module is off (Sonoff Dual R3 / Pow CT)
    11-10  Pmode[1:0]         Selection of active energy calculation method:
                              Pmode =00, both positive and negative active energy participate in the accumulation,
                                the accumulation method is algebraic sum mode, the reverse REVQ symbol indicates to active power; (Sonoff Dual R3 / Pow CT)
                              Pmode = 01, only accumulate positive active energy;
                              Pmode = 10, both positive and negative active energy participate in the accumulation,
                                and the accumulation method is absolute value method. No reverse active power indication;
                              Pmode =11, reserved, the mode is the same as Pmode =00
    9      NC                 -
    8      ZXD1               The initial value of ZX output is 0, and different waveforms are output according to the configuration of ZXD1 and ZXD0:
                              =0, it means that the ZX output changes only at the selected zero-crossing point (Sonoff Dual R3 / Pow CT)
                              =1, indicating that the ZX output changes at both the positive and negative zero crossings
    7      ZXD0
                              =0, indicates that the positive zero-crossing point is selected as the zero-crossing detection signal (Sonoff Dual R3 / Pow CT)
                              =1, indicating that the negative zero-crossing point is selected as the zero-crossing detection signal
    6      HPFIBOFF
                              =0, enable current channel B digital high-pass filter (Sonoff Dual R3)
                              =1, turn off the digital high-pass filter of current channel B (Pow CT)
    5      HPFIAOFF
                              =0, enable current channel A digital high-pass filter (Sonoff Dual R3 / Pow CT)
                              =1, turn off the digital high-pass filter of current channel A
    4      HPFUOFF
                              =0, enable U channel digital high pass filter (Sonoff Dual R3 / Pow CT)
                              =1, turn off the U channel digital high-pass filter
    3-2    NC                 -
    1      PBRUN
                              =1, enable PFB pulse output and active energy register accumulation; (Sonoff Dual R3 / Pow CT)
                              =0 (default), turn off PFB pulse output and active energy register accumulation.
    0      PARUN
                              =1, enable PFA pulse output and active energy register accumulation; (Sonoff Dual R3 / Pow CT)
                              =0 (default), turn off PFA pulse output and active energy register accumulation.
*/
//    Cse7761Write(CSE7761_REG_EMUCON | 0x80, 0x1043);  // Pow CT
//    Cse7761Write(CSE7761_REG_EMUCON | 0x80, 0x1003);  // Sonoff Dual R3
    Cse7761Write(CSE7761_REG_EMUCON | 0x80, 0x1183);  // Tasmota enable zero cross detection on both positive and negative signal
/*
    Energy Measure Control Register (EMUCON2)  Addr: 0x13  Default value: 0x0001
    Bit    name               Function description
    15-13  NC                 -
    12     SDOCmos
                              =1, SDO pin CMOS open-drain output
                              =0, SDO pin CMOS output (Sonoff Dual R3 / Pow CT)
    11     EPB_CB             Energy_PB clear signal control, the default is 0, and it needs to be configured to 1 in UART mode.
                                Clear after reading is not supported in UART mode
                              =1, Energy_PB will not be cleared after reading; (Sonoff Dual R3 / Pow CT)
                              =0, Energy_PB is cleared after reading;
    10     EPA_CB             Energy_PA clear signal control, the default is 0, it needs to be configured to 1 in UART mode,
                                Clear after reading is not supported in UART mode
                              =1, Energy_PA will not be cleared after reading; (Sonoff Dual R3 / Pow CT)
                              =0, Energy_PA is cleared after reading;
    9-8    DUPSEL[1:0]        Average register update frequency control
                              =00, Update frequency 3.4Hz
                              =01, Update frequency 6.8Hz
                              =10, Update frequency 13.65Hz
                              =11, Update frequency 27.3Hz (Sonoff Dual R3 / Pow CT)
    7      CHS_IB             Current channel B measurement selection signal
                              =1, measure the current of channel B (Sonoff Dual R3 / Pow CT)
                              =0, measure the internal temperature of the chip
    6      PfactorEN          Power factor function enable
                              =1, turn on the power factor output function (Sonoff Dual R3 / Pow CT)
                              =0, turn off the power factor output function
    5      WaveEN             Waveform data, instantaneous data output enable signal
                              =1, turn on the waveform data output function (Tasmota add frequency)
                              =0, turn off the waveform data output function (Sonoff Dual R3 / Pow CT)
    4      SAGEN              Voltage drop detection enable signal, WaveEN=1 must be configured first
                              =1, turn on the voltage drop detection function
                              =0, turn off the voltage drop detection function (Sonoff Dual R3 / Pow CT)
    3      OverEN             Overvoltage, overcurrent, and overload detection enable signal, WaveEN=1 must be configured first
                              =1, turn on the overvoltage, overcurrent, and overload detection functions
                              =0, turn off the overvoltage, overcurrent, and overload detection functions (Sonoff Dual R3 / Pow CT)
    2      ZxEN               Zero-crossing detection, phase angle, voltage frequency measurement enable signal
                              =1, turn on the zero-crossing detection, phase angle, and voltage frequency measurement functions (Tasmota add frequency)
                              =0, disable zero-crossing detection, phase angle, voltage frequency measurement functions (Sonoff Dual R3 / Pow CT)
    1      PeakEN             Peak detect enable signal
                              =1, turn on the peak detection function
                              =0, turn off the peak detection function (Sonoff Dual R3 / Pow CT)
    0      NC                 Default is 1
*/
#ifndef CSE7761_FREQUENCY
    Cse7761Write(CSE7761_REG_EMUCON2 | 0x80, 0x0FC1);  // Sonoff Dual R3 / Pow CT
#else
    Cse7761Write(CSE7761_REG_EMUCON2 | 0x80, 0x0FE5);  // Tasmota add Frequency

#ifdef CSE7761_ZEROCROSS
/*
    Pin function output selection register (PULSE1SEL)  Addr: 0x1D  Default value: 0x3210
    Bit    name               Function description
    15-13  NC                 -
    12     SDOCmos
                              =1, SDO pin CMOS open-drain output

    15-12  NC                 NC, the default value is 4'b0011
    11-8   NC                 NC, the default value is 4'b0010
    7-4    P2Sel              Pulse2 Pin output function selection, see the table below
    3-0    P1Sel              Pulse1 Pin output function selection, see the table below

    Table Pulsex function output selection list
    Pxsel  Select description
    0000   Output of energy metering calibration pulse PFA
    0001   The output of the energy metering calibration pulse PFB
    0010   Comparator indication signal comp_sign
    0011   Interrupt signal IRQ output (the default is high level, if it is an interrupt, set to 0)
    0100   Signal indication of power overload: only PA or PB can be selected
    0101   Channel A negative power indicator signal
    0110   Channel B negative power indicator signal
    0111   Instantaneous value update interrupt output
    1000   Average update interrupt output
    1001   Voltage channel zero-crossing signal output (Tasmota add zero-cross detection)
    1010   Current channel A zero-crossing signal output
    1011   Current channel B zero crossing signal output
    1100   Voltage channel overvoltage indication signal output
    1101   Voltage channel undervoltage indication signal output
    1110   Current channel A overcurrent signal indication output
    1111   Current channel B overcurrent signal indication output
*/
  Cse7761Write(CSE7761_REG_PULSE1SEL | 0x80, 0x3290);
#endif  // CSE7761_ZEROCROSS
#endif  // CSE7761_FREQUENCY
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

#ifdef CSE7761_FREQUENCY
  value = Cse7761ReadFallback(CSE7761_REG_UFREQ, CSE7761Data.frequency, 2);
#ifdef CSE7761_SIMULATE
  value = 8948;  // 49.99Hz
#endif
  CSE7761Data.frequency = (value >= 0x8000) ? 0 : value;
#endif  // CSE7761_FREQUENCY

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

  if (2 == Energy->phase_count) {
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
  }

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("C61: F%d, U%d, I%d/%d, P%d/%d"),
    CSE7761Data.frequency, CSE7761Data.voltage_rms,
    CSE7761Data.current_rms[0], CSE7761Data.current_rms[1],
    CSE7761Data.active_power[0], CSE7761Data.active_power[1]);

  if (Energy->power_on) {  // Powered on
    for (uint32_t channel = 0; channel < Energy->phase_count; channel++) {
      if (0 == channel) {
        // Voltage = RmsU * RmsUC * 10 / 0x400000
        // Energy->voltage[0] = (float)(((uint64_t)CSE7761Data.voltage_rms * CSE7761Data.coefficient[RmsUC] * 10) >> 22) / 1000;  // V
        Energy->voltage[0] = ((float)CSE7761Data.voltage_rms / EnergyGetCalibration(ENERGY_VOLTAGE_CALIBRATION));  // V
#ifdef CSE7761_FREQUENCY
        Energy->frequency[0] = (CSE7761Data.frequency) ? ((float)EnergyGetCalibration(ENERGY_FREQUENCY_CALIBRATION) / 8 / CSE7761Data.frequency) : 0;  // Hz
#endif
      } else {
        Energy->voltage[1] = Energy->voltage[0];
#ifdef CSE7761_FREQUENCY
        Energy->frequency[1] = Energy->frequency[0];
#endif
      }
      Energy->data_valid[channel] = 0;
      uint32_t power_calibration = EnergyGetCalibration(ENERGY_POWER_CALIBRATION, channel);
      // Active power = PowerPA * PowerPAC * 1000 / 0x80000000
      // Energy->active_power[channel] = (float)(((uint64_t)CSE7761Data.active_power[channel] * CSE7761Data.coefficient[PowerPAC + channel] * 1000) >> 31) / 1000;  // W
      Energy->active_power[channel] = (float)CSE7761Data.active_power[channel] / power_calibration;  // W
      if (0 == Energy->active_power[channel]) {
        Energy->current[channel] = 0;
      } else {
        uint32_t current_calibration = EnergyGetCalibration(ENERGY_CURRENT_CALIBRATION, channel);
        // Current = RmsIA * RmsIAC / 0x800000
        // Energy->current[channel] = (float)(((uint64_t)CSE7761Data.current_rms[channel] * CSE7761Data.coefficient[RmsIAC + channel]) >> 23) / 1000;  // A
        Energy->current[channel] = (float)CSE7761Data.current_rms[channel] / current_calibration;  // A
        CSE7761Data.energy[channel] += Energy->active_power[channel];
        CSE7761Data.energy_update[channel]++;
      }
    }
  }
}

/********************************************************************************************/
/*
void Cse7761DumpRegs(void) {
  uint32_t registers[23] = { 0 };
  uint32_t reg_num[23] = { 0 };
  reg_num[0] = 0x00; registers[0] = Cse7761Read(0x00, 2);
  reg_num[1] = 0x01; registers[1] = Cse7761Read(0x01, 2);
  reg_num[2] = 0x02; registers[2] = Cse7761Read(0x02, 2);
  reg_num[3] = 0x13; registers[3] = Cse7761Read(0x13, 2);
  reg_num[4] = 0x1D; registers[4] = Cse7761Read(0x1D, 2);
  reg_num[5] = 0x2F; registers[5] = Cse7761Read(0x2F, 3);
  reg_num[6] = 0x40; registers[6] = Cse7761Read(0x40, 2);
  reg_num[7] = 0x41; registers[7] = Cse7761Read(0x41, 2);
  reg_num[8] = 0x42; registers[8] = Cse7761Read(0x42, 2);
  reg_num[9] = 0x43; registers[9] = Cse7761Read(0x43, 1);
  reg_num[10] = 0x44; registers[10] = Cse7761Read(0x44, 4);
  reg_num[11] = 0x45; registers[11] = Cse7761Read(0x45, 2);
  reg_num[12] = 0x6E; registers[12] = Cse7761Read(0x6E, 2);
  reg_num[13] = 0x6F; registers[13] = Cse7761Read(0x6F, 2);
  reg_num[14] = 0x70; registers[14] = Cse7761Read(0x70, 2);
  reg_num[15] = 0x71; registers[15] = Cse7761Read(0x71, 2);
  reg_num[16] = 0x72; registers[16] = Cse7761Read(0x72, 2);
  reg_num[17] = 0x73; registers[17] = Cse7761Read(0x73, 2);
  reg_num[18] = 0x74; registers[18] = Cse7761Read(0x74, 2);
  reg_num[19] = 0x75; registers[19] = Cse7761Read(0x75, 2);
  reg_num[20] = 0x76; registers[20] = Cse7761Read(0x76, 2);
  reg_num[21] = 0x77; registers[21] = Cse7761Read(0x77, 2);
  reg_num[22] = 0x7F; registers[22] = Cse7761Read(0x7F, 3);

  char reg_data[320];
  reg_data[0] = '\0';
  for (uint32_t i = 0; i < 23; i++) {
    snprintf_P(reg_data, sizeof(reg_data), PSTR("%s%s%8X"), reg_data, (i) ? "," : "", reg_num[i]);
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR("C61: RegDump %s"), reg_data);

  reg_data[0] = '\0';
  for (uint32_t i = 0; i < 23; i++) {
    snprintf_P(reg_data, sizeof(reg_data), PSTR("%s%s%08X"), reg_data, (i) ? "," : "", registers[i]);
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR("C61: RegDump %s"), reg_data);
}
*/

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
      for (uint32_t channel = 0; channel < Energy->phase_count; channel++) {
        if (CSE7761Data.energy_update[channel]) {
          Energy->kWhtoday_delta[channel] += ((CSE7761Data.energy[channel] * 1000) / CSE7761Data.energy_update[channel]) / 36;
          CSE7761Data.energy[channel] = 0;
          CSE7761Data.energy_update[channel] = 0;
        }
        EnergyUpdateToday();
      }
    }
  }
}

void Cse7761SnsInit(void) {
  // Software serial init needs to be done here as earlier (serial) interrupts may lead to Exceptions
  Cse7761Serial = new TasmotaSerial(Pin(GPIO_CSE7761_RX, GPIO_ANY), Pin(GPIO_CSE7761_TX), 1);
  if (Cse7761Serial->begin(38400, SERIAL_8E1)) {
    if (Cse7761Serial->hardwareSerial()) {
      SetSerial(38400, TS_SERIAL_8E1);
      ClaimSerial();
    }
#ifdef ESP32
    AddLog(LOG_LEVEL_DEBUG, PSTR("C61: Serial UART%d"), Cse7761Serial->getUart());
#endif

#ifdef CSE7761_FREQUENCY
#ifdef CSE7761_ZEROCROSS
    ZeroCrossInit(CSE7761_ZEROCROSS_OFFSET + CSE7761_RELAY_SWITCHTIME);
#endif  // CSE7761_ZEROCROSS
#endif  // CSE7761_FREQUENCY

  } else {
    TasmotaGlobal.energy_driver = ENERGY_NONE;
  }
}

void Cse7761DrvInit(void) {
  if (PinUsed(GPIO_CSE7761_RX, GPIO_ANY) && PinUsed(GPIO_CSE7761_TX)) {
    CSE7761Data.model = GetPin(Pin(GPIO_CSE7761_RX, GPIO_ANY)) - AGPIO(GPIO_CSE7761_RX);
    CSE7761Data.ready = 0;
    CSE7761Data.init = 4;                        // Init setup steps

//    Energy->phase_count = 1;                     // Handle one channel (default set by xdrv_03_energy.ino)
    if (CSE7761_MODEL_DUALR3 == CSE7761Data.model) {
      Energy->phase_count = 2;                   // Handle two channels as two phases
    }
    Energy->voltage_common = true;               // Use common voltage
#ifdef CSE7761_FREQUENCY
    Energy->frequency_common = true;             // Use common frequency
#endif
    Energy->use_overtemp = true;                 // Use global temperature for overtemp detection
    TasmotaGlobal.energy_driver = XNRG_19;
  }
}

bool Cse7761Command(void) {
  bool serviced = true;

  uint32_t channel = 0;
  if (Energy->phase_count > 1) {
    channel = (2 == XdrvMailbox.index) ? 1 : 0;
  }
  uint32_t value = (uint32_t)(CharToFloat(XdrvMailbox.data) * 100);  // 1.23 = 123

  if (CMND_POWERCAL == Energy->command_code) {
    if (1 == XdrvMailbox.payload) { XdrvMailbox.payload = Cse7761Ref(PowerPAC); }
    // Service in xdrv_03_energy.ino
  }
  else if (CMND_POWERSET == Energy->command_code) {
    if (XdrvMailbox.data_len && CSE7761Data.active_power[channel]) {
      if ((value > 100) && (value < 2000000)) {  // Between 1W and 20000W
        XdrvMailbox.payload = ((CSE7761Data.active_power[channel]) / value) * 100;
      }
    }
  }
  else if (CMND_VOLTAGECAL == Energy->command_code) {
    if (1 == XdrvMailbox.payload) { XdrvMailbox.payload = Cse7761Ref(RmsUC); }
    // Service in xdrv_03_energy.ino
  }
  else if (CMND_VOLTAGESET == Energy->command_code) {
    if (XdrvMailbox.data_len && CSE7761Data.voltage_rms) {
      if ((value > 10000) && (value < 40000)) {  // Between 100V and 400V
        XdrvMailbox.payload = (CSE7761Data.voltage_rms * 100) / value;
      }
    }
  }
  else if (CMND_CURRENTCAL == Energy->command_code) {
    if (1 == XdrvMailbox.payload) { XdrvMailbox.payload = Cse7761Ref(RmsIAC); }
    // Service in xdrv_03_energy.ino
  }
  else if (CMND_CURRENTSET == Energy->command_code) {
    if (XdrvMailbox.data_len && CSE7761Data.current_rms[channel]) {
      if ((value > 1000) && (value < 10000000)) {  // Between 10mA and 100A
        XdrvMailbox.payload = ((CSE7761Data.current_rms[channel] * 100) / value) * 1000;
      }
    }
  }
#ifdef CSE7761_FREQUENCY
  else if (CMND_FREQUENCYCAL == Energy->command_code) {
    if (1 == XdrvMailbox.payload) { XdrvMailbox.payload = CSE7761_FREF; }
    // Service in xdrv_03_energy.ino
  }
  else if (CMND_FREQUENCYSET == Energy->command_code) {
    if (XdrvMailbox.data_len && CSE7761Data.frequency) {
      if ((value > 4500) && (value < 6500)) {  // Between 45.00Hz and 65.00Hz
        XdrvMailbox.payload = (CSE7761Data.frequency * 8 * value) / 100;
      }
    }
  }
#endif
  else serviced = false;  // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg19(uint32_t function) {
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
