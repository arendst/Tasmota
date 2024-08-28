/*
  xnrg_07_ade7953.ino - ADE7953 energy sensor support for Tasmota

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

#if defined(ESP32) && defined(USE_SPI)
#define USE_ESP32_SPI
#endif
#if defined(USE_I2C) || defined(USE_ESP32_SPI)
#ifdef USE_ENERGY_SENSOR
#ifdef USE_ADE7953
/*********************************************************************************************\
 * ADE7953 - Energy used in Shelly 2.5 (model 1), EM (model 2), Plus 2PM (model 3), Pro 1PM (model 4), Pro 2PM (model 5) and Pro 4PM (model 6)
 *
 * {"NAME":"Shelly 2.5","GPIO":[320,0,32,0,224,193,0,0,640,192,608,225,3456,4736],"FLAG":0,"BASE":18}
 * {"NAME":"Shelly EM","GPIO":[0,0,0,0,0,0,0,0,640,3457,608,224,8832,1],"FLAG":0,"BASE":18}
 * {"NAME":"Shelly Plus 2PM PCB v0.1.5","GPIO":[320,0,192,0,0,0,1,1,225,224,0,0,0,0,193,0,0,0,0,0,0,608,3840,32,0,0,0,0,0,640,0,0,3458,4736,0,0],"FLAG":0,"BASE":1,"CMND":"AdcParam1 2,32000,40000,3350"}
 * {"NAME":"Shelly Plus 2PM PCB v0.1.9","GPIO":[320,0,0,0,32,192,0,0,225,224,0,0,0,0,193,0,0,0,0,0,0,608,640,3458,0,0,0,0,0,9472,0,4736,0,0,0,0],"FLAG":0,"BASE":1,"CMND":"AdcParam1 2,10000,10000,3350"}
 * {"NAME":"Shelly Pro 1PM","GPIO":[9568,1,9472,1,768,0,0,0,672,704,736,0,0,0,5600,6214,0,0,0,5568,0,0,0,0,0,0,0,0,3459,0,0,32,4736,0,160,0],"FLAG":0,"BASE":1,"CMND":"AdcParam1 2,5600,4700,3350"}
 * {"NAME":"Shelly Pro 2PM","GPIO":[9568,1,9472,1,768,0,0,0,672,704,736,9569,0,0,5600,6214,0,0,0,5568,0,0,0,0,0,0,0,0,3460,0,0,32,4736,4737,160,161],"FLAG":0,"BASE":1,"CMND":"AdcParam1 2,5600,4700,3350;AdcParam2 2,5600,4700,3350"}
 * {"NAME":"Shelly Pro 4PM","GPIO":[0,6210,0,6214,9568,0,0,0,0,0,9569,0,768,0,5600,0,0,0,0,5568,0,0,0,0,0,0,0,0,736,704,3461,0,4736,0,0,672],"FLAG":0,"BASE":1,"CMND":"AdcParam1 2,5600,4700,3350"}
 *
 * Based on datasheet from https://www.analog.com/en/products/ade7953.html
 *
 * Model differences:
 * Function                        Model1   Model2   Model3   Model4  Model5  Model6  Remark
 * ------------------------------  -------  -------  -------  ------  ------  ------  -------------------------------------------------
 * Shelly                          2.5      EM       Plus2PM  Pro1PM  Pro2PM  Pro4PM
 * Processor                       ESP8266  ESP8266  ESP32    ESP32   ESP32   ESP32
 * Interface                       I2C      I2C      I2C      SPI     SPI     SPI     Interface type used
 * Number of inputs                2        2        2        1       2       4       Count of ADE9753 inputs used
 * Number of ADE9753 chips         1        1        1        1       2       2       Count of ADE9753 chips
 * ADE9753 IRQ                     1        2        3        4       5       6       Index defines model number
 * Current measurement device      shunt    CT       shunt    shunt   shunt   shunt   CT = Current Transformer
 * Common voltage                  Yes      Yes      Yes      No      No      Yes     Show common voltage in GUI/JSON
 * Common frequency                Yes      Yes      Yes      No      No      Yes     Show common frequency in GUI/JSON
 * Swapped channel A/B             Yes      No       No       No      No      No      Defined by hardware design - Fixed by Tasmota
 * Support Export Active           No       Yes      No       No      No      No      Only EM supports correct negative value detection
 * Show negative (reactive) power  No       Yes      No       No      No      No      Only EM supports correct negative value detection
 * Default phase calibration       0        200      0        0       0       0       CT needs different phase calibration than shunts
 * Default reset pin on ESP8266    -        16       -        -       -       -       Legacy support. Replaced by GPIO ADE7953RST
 *
 * I2C Address: 0x38
 *********************************************************************************************
 * Optionally allowing users to tweak calibration registers:
 * - In addition to possible rules add a rule containing the calib.dat string like:
 *   - rule3 on file#calib.dat do {"angles":{"angle0":180,"angle1":176}} endon
 *   - rule3 on file#calib.dat do {"rms":{"current_a":4194303,"current_b":4194303,"voltage":1613194},"angles":{"angle0":200,"angle1":200},"powers":{"totactive":{"a":2723574,"b":2723574},"apparent":{"a":2723574,"b":2723574},"reactive":{"a":2723574,"b":2723574}}} endon
 * - Restart Tasmota and obeserve that the results seem calibrated as Tasmota now uses the information from calib.dat
 * To restore standard calibration using commands like VoltSet remove above entry from rule3
\*********************************************************************************************/

#define XNRG_07                   7
#define XI2C_07                   7  // See I2CDEVICES.md

#define ADE7953_ADDR              0x38

/*********************************************************************************************/

#define ADE7953_ACCU_ENERGY                  // Use accumulating energy instead of instant power

//#define ADE7953_DUMP_REGS

#define ADE7953_PREF              1540       // 4194304 / (1540 / 1000) = 2723574 (= WGAIN, VAGAIN and VARGAIN)
#define ADE7953_UREF              26000      // 4194304 / (26000 / 10000) = 1613194 (= VGAIN)
#define ADE7953_IREF              10000      // 4194304 / (10000 / 10000) = 4194303 (= IGAIN, needs to be different than 4194304 in order to use calib.dat)
#define ADE7953_NO_LOAD_THRESHOLD 29196      // According to ADE7953 datasheet the default threshold for no load detection is 58,393 use half this value to measure lower (5w) powers.
#define ADE7953_NO_LOAD_ENABLE    0          // Set DISNOLOAD register to 0 to enable No-load detection
#define ADE7953_NO_LOAD_DISABLE   7          // Set DISNOLOAD register to 7 to disable No-load detection

// Default calibration parameters can be overridden by a rule as documented above.
#define ADE7953_GAIN_DEFAULT      4194304    // = 0x400000 range 2097152 (min) to 6291456 (max)
#define ADE7953_PHCAL_DEFAULT     0          // = range -383 to 383 - Default phase calibration for Shunts
#define ADE7953_PHCAL_DEFAULT_CT  200        // = range -383 to 383 - Default phase calibration for Current Transformers (Shelly EM)

#define ADE7953_MAX_CHANNEL       4

enum Ade7953Models { ADE7953_SHELLY_25, ADE7953_SHELLY_EM, ADE7953_SHELLY_PLUS_2PM, ADE7953_SHELLY_PRO_1PM, ADE7953_SHELLY_PRO_2PM, ADE7953_SHELLY_PRO_4PM };

enum Ade7953_8BitRegisters {
  // Register Name                    Addres  R/W  Bt  Ty  Default     Description
  // ----------------------------     ------  ---  --  --  ----------  --------------------------------------------------------------------
  ADE7953_SAGCYC = 0x000,          // 0x000   R/W  8   U   0x00        Sag line cycles
  ADE7953_DISNOLOAD,               // 0x001   R/W  8   U   0x00        No-load detection disable (see Table 16)
  ADE7953_RESERVED_0X002,          // 0x002
  ADE7953_RESERVED_0X003,          // 0x003
  ADE7953_LCYCMODE,                // 0x004   R/W  8   U   0x40        Line cycle accumulation mode configuration (see Table 17)
  ADE7953_RESERVED_0X005,          // 0x005
  ADE7953_RESERVED_0X006,          // 0x006
  ADE7953_PGA_V,                   // 0x007   R/W  8   U   0x00        Voltage channel gain configuration (Bits[2:0])
  ADE7953_PGA_IA,                  // 0x008   R/W  8   U   0x00        Current Channel A gain configuration (Bits[2:0])
  ADE7953_PGA_IB                   // 0x009   R/W  8   U   0x00        Current Channel B gain configuration (Bits[2:0])
};

enum Ade7953_16BitRegisters {
  // Register Name                    Addres  R/W  Bt  Ty  Default     Description
  // ----------------------------     ------  ---  --  --  ----------  --------------------------------------------------------------------
  ADE7953_ZXTOUT = 0x100,          // 0x100   R/W  16  U   0xFFFF      Zero-crossing timeout
  ADE7953_LINECYC,                 // 0x101   R/W  16  U   0x0000      Number of half line cycles for line cycle energy accumulation mode
  ADE7953_CONFIG,                  // 0x102   R/W  16  U   0x8004      Configuration register (see Table 18)
  ADE7953_CF1DEN,                  // 0x103   R/W  16  U   0x003F      CF1 frequency divider denominator. When modifying this register, two sequential write operations must be performed to ensure that the write is successful.
  ADE7953_CF2DEN,                  // 0x104   R/W  16  U   0x003F      CF2 frequency divider denominator. When modifying this register, two sequential write operations must be performed to ensure that the write is successful.
  ADE7953_RESERVED_0X105,          // 0x105
  ADE7953_RESERVED_0X106,          // 0x106
  ADE7953_CFMODE,                  // 0x107   R/W  16  U   0x0300      CF output selection (see Table 19)
  ADE7943_PHCALA,                  // 0x108   R/W  16  S   0x0000      Phase calibration register (Current Channel A). This register is in sign magnitude format.
  ADE7943_PHCALB,                  // 0x109   R/W  16  S   0x0000      Phase calibration register (Current Channel B). This register is in sign magnitude format.
  ADE7943_PFA,                     // 0x10A   R    16  S   0x0000      Power factor (Current Channel A)
  ADE7943_PFB,                     // 0x10B   R    16  S   0x0000      Power factor (Current Channel B)
  ADE7943_ANGLE_A,                 // 0x10C   R    16  S   0x0000      Angle between the voltage input and the Current Channel A input
  ADE7943_ANGLE_B,                 // 0x10D   R    16  S   0x0000      Angle between the voltage input and the Current Channel B input
  ADE7943_Period,                  // 0x10E   R    16  U   0x0000      Period register

  ADE7953_RESERVED_0X120 = 0x120   // 0x120                            This register should be set to 30h to meet the performance specified in Table 1. To modify this register, it must be unlocked by setting Register Address 0xFE to 0xAD immediately prior.
};

enum Ade7953_32BitRegisters {
  // Register Name                    Addres  R/W  Bt  Ty  Default     Description
  // ----------------------------     ------  ---  --  --  ----------  --------------------------------------------------------------------
  ADE7953_ACCMODE = 0x301,         // 0x301   R/W  24  U   0x000000    Accumulation mode (see Table 21)

  ADE7953_AP_NOLOAD = 0x303,       // 0x303   R/W  24  U   0x00E419    No load threshold for actual power
  ADE7953_VAR_NOLOAD,              // 0x304   R/W  24  U   0x00E419    No load threshold for reactive power
  ADE7953_VA_NOLOAD,               // 0x305   R/W  24  U   0x000000    No load threshold for appearant power

  ADE7953_AVA = 0x310,             // 0x310   R    24  S   0x000000    Instantaneous apparent power (Current Channel A)
  ADE7953_BVA,                     // 0x311   R    24  S   0x000000    Instantaneous apparent power (Current Channel B)
  ADE7953_AWATT,                   // 0x312   R    24  S   0x000000    Instantaneous active power (Current Channel A)
  ADE7953_BWATT,                   // 0x313   R    24  S   0x000000    Instantaneous active power (Current Channel B)
  ADE7953_AVAR,                    // 0x314   R    24  S   0x000000    Instantaneous reactive power (Current Channel A)
  ADE7953_BVAR,                    // 0x315   R    24  S   0x000000    Instantaneous reactive power (Current Channel B)
  ADE7953_IA,                      // 0x316   R    24  S   0x000000    Instantaneous current (Current Channel A)
  ADE7953_IB,                      // 0x317   R    24  S   0x000000    Instantaneous current (Current Channel B)
  ADE7953_V,                       // 0x318   R    24  S   0x000000    Instantaneous voltage (voltage channel)
  ADE7953_RESERVED_0X319,          // 0x319
  ADE7953_IRMSA,                   // 0x31A   R    24  U   0x000000    IRMS register (Current Channel A)
  ADE7953_IRMSB,                   // 0x31B   R    24  U   0x000000    IRMS register (Current Channel B)
  ADE7953_VRMS,                    // 0x31C   R    24  U   0x000000    VRMS register
  ADE7953_RESERVED_0X31D,          // 0x31D
  ADE7953_AENERGYA,                // 0x31E   R    24  S   0x000000    Active energy (Current Channel A)
  ADE7953_AENERGYB,                // 0x31F   R    24  S   0x000000    Active energy (Current Channel B)
  ADE7953_RENERGYA,                // 0x320   R    24  S   0x000000    Reactive energy (Current Channel A)
  ADE7953_RENERGYB,                // 0x321   R    24  S   0x000000    Reactive energy (Current Channel B)
  ADE7953_APENERGYA,               // 0x322   R    24  S   0x000000    Apparent energy (Current Channel A)
  ADE7953_APENERGYB,               // 0x323   R    24  S   0x000000    Apparent energy (Current Channel B)
  ADE7953_OVLVL,                   // 0x324   R/W  24  U   0xFFFFFF    Overvoltage level
  ADE7953_OILVL,                   // 0x325   R/W  24  U   0xFFFFFF    Overcurrent level
  ADE7953_VPEAK,                   // 0x326   R    24  U   0x000000    Voltage channel peak
  ADE7953_RSTVPEAK,                // 0x327   R    24  U   0x000000    Read voltage peak with reset
  ADE7953_IAPEAK,                  // 0x328   R    24  U   0x000000    Current Channel A peak
  ADE7953_RSTIAPEAK,               // 0x329   R    24  U   0x000000    Read Current Channel A peak with reset
  ADE7953_IBPEAK,                  // 0x32A   R    24  U   0x000000    Current Channel B peak
  ADE7953_RSTIBPEAK,               // 0x32B   R    24  U   0x000000    Read Current Channel B peak with reset
  ADE7953_IRQENA,                  // 0x32C   R/W  24  U   0x100000    Interrupt enable (Current Channel A, see Table 22)
  ADE7953_IRQSTATA,                // 0x32D   R    24  U   0x000000    Interrupt status (Current Channel A, see Table 23)
  ADE7953_RSTIRQSTATA,             // 0x32E   R    24  U   0x000000    Reset interrupt status (Current Channel A)
  ADE7953_IRQENB,                  // 0x32F   R/W  24  U   0x000000    Interrupt enable (Current Channel B, see Table 24)
  ADE7953_IRQSTATB,                // 0x330   R    24  U   0x000000    Interrupt status (Current Channel B, see Table 25)
  ADE7953_RSTIRQSTATB,             // 0x331   R    24  U   0x000000    Reset interrupt status (Current Channel B)

  ADE7953_CRC = 0x37F,             // 0x37F   R    32  U   0xFFFFFFFF  Checksum
  ADE7953_AIGAIN,                  // 0x380   R/W  24  U   0x400000    Current channel gain (Current Channel A)
  ADE7953_AVGAIN,                  // 0x381   R/W  24  U   0x400000    Voltage channel gain
  ADE7953_AWGAIN,                  // 0x382   R/W  24  U   0x400000    Active power gain (Current Channel A)
  ADE7953_AVARGAIN,                // 0x383   R/W  24  U   0x400000    Reactive power gain (Current Channel A)
  ADE7953_AVAGAIN,                 // 0x384   R/W  24  U   0x400000    Apparent power gain (Current Channel A)
  ADE7953_RESERVED_0X385,          // 0x385
  ADE7953_AIRMSOS,                 // 0x386   R/W  24  S   0x000000    IRMS offset (Current Channel A)
  ADE7953_RESERVED_0X387,          // 0x387
  ADE7953_VRMSOS,                  // 0x388   R/W  24  S   0x000000    VRMS offset
  ADE7953_AWATTOS,                 // 0x389   R/W  24  S   0x000000    Active power offset correction (Current Channel A)
  ADE7953_AVAROS,                  // 0x38A   R/W  24  S   0x000000    Reactive power offset correction (Current Channel A)
  ADE7953_AVAOS,                   // 0x38B   R/W  24  S   0x000000    Apparent power offset correction (Current Channel A)
  ADE7953_BIGAIN,                  // 0x38C   R/W  24  U   0x400000    Current channel gain (Current Channel B)
  ADE7953_BVGAIN,                  // 0x38D   R/W  24  U   0x400000    This register should not be modified.
  ADE7953_BWGAIN,                  // 0x38E   R/W  24  U   0x400000    Active power gain (Current Channel B)
  ADE7953_BVARGAIN,                // 0x38F   R/W  24  U   0x400000    Reactive power gain (Current Channel B)
  ADE7953_BVAGAIN,                 // 0x390   R/W  24  U   0x400000    Apparent power gain (Current Channel B)
  ADE7953_RESERVED_0X391,          // 0x391
  ADE7953_BIRMSOS,                 // 0x392   R/W  24  S   0x000000    IRMS offset (Current Channel B)
  ADE7953_RESERVED_0X393,          // 0x393
  ADE7953_RESERVED_0X394,          // 0x394
  ADE7953_BWATTOS,                 // 0x395   R/W  24  S   0x000000    Active power offset correction (Current Channel B)
  ADE7953_BVAROS,                  // 0x396   R/W  24  S   0x000000    Reactive power offset correction (Current Channel B)
  ADE7953_BVAOS                    // 0x397   R/W  24  S   0x000000    Apparent power offset correction (Current Channel B)
};

enum Ade7953CalibrationRegisters {
  ADE7953_CAL_VGAIN,
  ADE7953_CAL_IGAIN,
  ADE7953_CAL_WGAIN,
  ADE7953_CAL_VAGAIN,
  ADE7953_CAL_VARGAIN,
  ADE7943_CAL_PHCAL
};

const uint8_t  ADE7953_CALIBREGS = 6;
const uint16_t Ade7953CalibRegs[2][ADE7953_CALIBREGS] {
  { ADE7953_AVGAIN, ADE7953_AIGAIN, ADE7953_AWGAIN, ADE7953_AVAGAIN, ADE7953_AVARGAIN, ADE7943_PHCALA },
  { ADE7953_BVGAIN, ADE7953_BIGAIN, ADE7953_BWGAIN, ADE7953_BVAGAIN, ADE7953_BVARGAIN, ADE7943_PHCALB }
};

const uint8_t  ADE7953_REGISTERS = 6;
const uint16_t Ade7953Registers[2][ADE7953_REGISTERS] {
#ifdef ADE7953_ACCU_ENERGY
  { ADE7953_IRMSA, ADE7953_AENERGYA, ADE7953_APENERGYA, ADE7953_RENERGYA, ADE7953_VRMS, ADE7943_Period },
  { ADE7953_IRMSB, ADE7953_AENERGYB, ADE7953_APENERGYB, ADE7953_RENERGYB, ADE7953_VRMS, ADE7943_Period }
#else   // No ADE7953_ACCU_ENERGY
  { ADE7953_IRMSA, ADE7953_AWATT, ADE7953_AVA, ADE7953_AVAR, ADE7953_VRMS, ADE7943_Period },
  { ADE7953_IRMSB, ADE7953_BWATT, ADE7953_BVA, ADE7953_BVAR, ADE7953_VRMS, ADE7943_Period }
#endif  // ADE7953_ACCU_ENERGY
};

#ifdef ADE7953_ACCU_ENERGY
const float ADE7953_LSB_PER_WATTSECOND = 2.5;
const float ADE7953_POWER_CORRECTION = 23.41494;  // See https://github.com/arendst/Tasmota/pull/16941
#else   // No ADE7953_ACCU_ENERGY
const float ADE7953_LSB_PER_WATTSECOND = 44;
#endif  // ADE7953_ACCU_ENERGY

typedef struct {
  uint32_t voltage_rms;
  uint32_t current_rms;
  uint32_t active_power;
  int32_t calib_data[ADE7953_CALIBREGS];
} tAde7953Channel;

struct Ade7953 {
  uint32_t last_update;
  uint32_t voltage_rms[ADE7953_MAX_CHANNEL] = { 0, 0 };
  uint32_t current_rms[ADE7953_MAX_CHANNEL] = { 0, 0 };
  uint32_t active_power[ADE7953_MAX_CHANNEL] = { 0, 0 };
  int32_t calib_data[ADE7953_MAX_CHANNEL][ADE7953_CALIBREGS];
  uint8_t init_step = 0;
  uint8_t model = 0;             // 0 = Shelly 2.5, 1 = Shelly EM, 2 = Shelly Plus 2PM, 3 = Shelly Pro 1PM, 4 = Shelly Pro 2PM, 5 = Shelly Pro 4PM
  uint8_t cs_index;
#ifdef USE_ESP32_SPI
  int8_t pin_cs[ADE7953_MAX_CHANNEL / 2];
#endif  // USE_ESP32_SPI
  bool use_spi;
} Ade7953;

/*********************************************************************************************/

int Ade7953RegSize(uint16_t reg) {
  int size = 0;
  switch ((reg >> 8) & 0x0F) {
    case 0x03:  // 32-bit
      size++;
    case 0x02:  // 24-bit
      size++;
    case 0x01:  // 16-bit
      size++;
    case 0x00:  // 8-bit
    case 0x07:
    case 0x08:
      size++;
  }
  return size;
}

#ifdef USE_ESP32_SPI
void Ade7953SpiEnable(void) {
  digitalWrite(Ade7953.pin_cs[Ade7953.cs_index], 0);
  delayMicroseconds(1);                              // CS 1uS to SCLK edge
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));  // Set up SPI at 1MHz, MSB first, Capture at rising edge
}

void Ade7953SpiDisable(void) {
  SPI.endTransaction();
  delayMicroseconds(2);                              // CS high 1.2uS after SCLK edge (when writing to COMM_LOCK bit)
  digitalWrite(Ade7953.pin_cs[Ade7953.cs_index], 1);
}
#endif  // USE_ESP32_SPI

void Ade7953Write(uint16_t reg, uint32_t val) {
  int size = Ade7953RegSize(reg);
  if (size) {

//      AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: Write %08X"), val);

#ifdef USE_ESP32_SPI
    if (Ade7953.use_spi) {
      Ade7953SpiEnable();
      SPI.transfer16(reg);
      SPI.transfer(0x00);                            // Write
      while (size--) {
        SPI.transfer((val >> (8 * size)) & 0xFF);    // Write data, MSB first
      }
      Ade7953SpiDisable();
    } else {
#endif  // USE_ESP32_SPI
      Wire.beginTransmission(ADE7953_ADDR);
      Wire.write((reg >> 8) & 0xFF);
      Wire.write(reg & 0xFF);
      while (size--) {
        Wire.write((val >> (8 * size)) & 0xFF);      // Write data, MSB first
      }
      Wire.endTransmission();
      delayMicroseconds(5);                          // Bus-free time minimum 4.7us
#ifdef USE_ESP32_SPI
    }
#endif  // USE_ESP32_SPI
  }
}

int32_t Ade7953Read(uint16_t reg) {
  uint32_t response = 0;

  int size = Ade7953RegSize(reg);
  if (size) {
#ifdef USE_ESP32_SPI
    if (Ade7953.use_spi) {
      Ade7953SpiEnable();
      SPI.transfer16(reg);
      SPI.transfer(0x80);                            // Read
      while (size--) {
        response = response << 8 | SPI.transfer(0xFF);  // receive DATA (MSB first)
      }
      Ade7953SpiDisable();
    } else {
#endif  // USE_ESP32_SPI
      Wire.beginTransmission(ADE7953_ADDR);
      Wire.write((reg >> 8) & 0xFF);
      Wire.write(reg & 0xFF);
      Wire.endTransmission(0);
      Wire.requestFrom(ADE7953_ADDR, size);
      if (size <= Wire.available()) {
        for (uint32_t i = 0; i < size; i++) {
          response = response << 8 | Wire.read();    // receive DATA (MSB first)
        }
      }
#ifdef USE_ESP32_SPI
    }
#endif  // USE_ESP32_SPI
  }
  return response;
}

#ifdef ADE7953_DUMP_REGS
void Ade7953DumpRegs(uint32_t chip) {
  AddLog(LOG_LEVEL_DEBUG, PSTR("ADE: *** Chip%d ****  SAGCYC DISNOLD  Resrvd  Resrvd LCYCMOD  Resrvd  Resrvd    PGAV   PGAIA   PGAIB"), chip +1);
  char data[200] = { 0 };
  for (uint32_t i = 0; i < 10; i++) {
    int32_t value = Ade7953Read(ADE7953_SAGCYC + i);
    snprintf_P(data, sizeof(data), PSTR("%s      %02X"), data, value);  // 8-bit regs
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR("ADE: *** 0x000..009%s"), data);
  AddLog(LOG_LEVEL_DEBUG, PSTR("ADE: ***             ZXTOUT LINECYC  CONFIG  CF1DEN  CF2DEN  Resrvd  Resrvd  CFMODE  PHCALA  PHCALB     PFA     PFB  ANGLEA  ANGLEB  Period"));
  data[0] = '\0';
  for (uint32_t i = 0; i < 15; i++) {
    int32_t value = Ade7953Read(ADE7953_ZXTOUT + i);
    snprintf_P(data, sizeof(data), PSTR("%s    %04X"), data, value);  // 16-bit regs
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR("ADE: *** 0x100..10E%s"), data);
  AddLog(LOG_LEVEL_DEBUG, PSTR("ADE: ***              IGAIN   VGAIN   WGAIN VARGAIN  VAGAIN  Resrvd  IRMSOS  Resrvd  VRMSOS  WATTOS   VAROS    VAOS"));
  data[0] = '\0';
  for (uint32_t i = 0; i < 12; i++) {
    int32_t value = Ade7953Read(ADE7953_AIGAIN + i);
    snprintf_P(data, sizeof(data), PSTR("%s  %06X"), data, value);  // 24-bit regs
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR("ADE: *** 0x380..38B%s"), data);
  data[0] = '\0';
  for (uint32_t i = 0; i < 12; i++) {
    int32_t value = Ade7953Read(ADE7953_BIGAIN + i);
    snprintf_P(data, sizeof(data), PSTR("%s  %06X"), data, value);  // 24-bit regs
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR("ADE: *** 0x38C..397%s"), data);
}
#endif  // ADE7953_DUMP_REGS

void Ade7953SetCalibration(uint32_t regset, uint32_t calibset) {
  for (uint32_t i = 0; i < ADE7953_CALIBREGS; i++) {
    int32_t value = Ade7953.calib_data[calibset][i];
    if (ADE7943_CAL_PHCAL == i) {
//      if (ADE7953_PHCAL_DEFAULT == value) { continue; }  // ADE7953 reset does NOT always reset all registers
      if (value < 0) {
        value = abs(value) + 0x200;                  // Add sign magnitude
      }
    }
//    if (ADE7953_GAIN_DEFAULT == value) { continue; }  // ADE7953 reset does NOT always reset all registers
    Ade7953Write(Ade7953CalibRegs[regset][i], value);
  }
}

void Ade7953Init(void) {
  uint32_t chips = 1;
#ifdef USE_ESP32_SPI
  chips = (Ade7953.pin_cs[1] >= 0) ? 2 : 1;
#endif  // USE_ESP32_SPI

  // Init ADE7953 with calibration settings
  for (uint32_t chip = 0; chip < chips; chip++) {
    Ade7953.cs_index = chip;

#ifdef ADE7953_DUMP_REGS
    Ade7953DumpRegs(chip);
#endif  // ADE7953_DUMP_REGS

    Ade7953Write(ADE7953_CONFIG, 0x0004);                        // Locking the communication interface (Clear bit COMM_LOCK), Enable HPF
    Ade7953Write(0x0FE, 0x00AD);                                 // Unlock register 0x120
    Ade7953Write(ADE7953_RESERVED_0X120, 0x0030);                // Configure optimum setting
    Ade7953Write(ADE7953_DISNOLOAD, 0x07);                       // Disable no load detection, required before setting thresholds
    Ade7953Write(ADE7953_AP_NOLOAD, ADE7953_NO_LOAD_THRESHOLD);  // Set no load treshold for active power
    Ade7953Write(ADE7953_VAR_NOLOAD, ADE7953_NO_LOAD_THRESHOLD); // Set no load treshold for reactive power
    Ade7953Write(ADE7953_DISNOLOAD, 0x00);                       // Enable no load detection

#ifdef USE_ESP32_SPI
//    int32_t value = Ade7953Read(0x702);              // Silicon version
//    AddLog(LOG_LEVEL_DEBUG, PSTR("ADE: Chip%d version %d"), chip +1, value);
    if (1 == chip) {
      switch (Ade7953.model) {
        case ADE7953_SHELLY_PRO_2PM:
          Ade7953SetCalibration(0, 1);               // Second ADE7953 A registers set with calibration set 1
          break;
        case ADE7953_SHELLY_PRO_4PM:
          Ade7953SetCalibration(0, 2);               // Second ADE7953 A registers set with calibration set 2
          Ade7953SetCalibration(1, 3);               // Second ADE7953 B registers set with calibration set 3
      }
    } else {
#endif  // USE_ESP32_SPI
      Ade7953SetCalibration(0, 0);                   // First ADE7953 A registers set with calibration set 0
      switch (Ade7953.model) {
        case ADE7953_SHELLY_25:
        case ADE7953_SHELLY_EM:
        case ADE7953_SHELLY_PLUS_2PM:
//        case ADE7953_SHELLY_PRO_1PM:               // Uses defaults for B registers
        case ADE7953_SHELLY_PRO_4PM:
          Ade7953SetCalibration(1, 1);               // First ADE7953 B registers set with calibration set 1
      }
#ifdef USE_ESP32_SPI
    }
#endif  // USE_ESP32_SPI
  }

  // Report set calibration settings
  int32_t regs[ADE7953_CALIBREGS];
  for (uint32_t chip = 0; chip < chips; chip++) {
    Ade7953.cs_index = chip;
    for (uint32_t channel = 0; channel < 2; channel++) {
      for (uint32_t i = 0; i < ADE7953_CALIBREGS; i++) {
        regs[i] = Ade7953Read(Ade7953CalibRegs[channel][i]);
        if (ADE7943_CAL_PHCAL == i) {
          if (regs[i] >= 0x0200) {
            regs[i] &= 0x01FF;                       // Clear sign magnitude
            regs[i] *= -1;                           // Make negative
          }
        }
      }
#ifdef USE_ESP32_SPI
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("ADE: Chip%d CalibRegs%c V %d, I %d, W %d, VA %d, VAr %d, Ph %d"),
        chip +1, 'A'+channel, regs[0], regs[1], regs[2], regs[3], regs[4], regs[5]);
#else
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("ADE: CalibRegs%c V %d, I %d, W %d, VA %d, VAr %d, Ph %d"),
        'A'+channel, regs[0], regs[1], regs[2], regs[3], regs[4], regs[5]);
#endif  // USE_ESP32_SPI
    }

#ifdef ADE7953_DUMP_REGS
    Ade7953DumpRegs(chip);
#endif  // ADE7953_DUMP_REGS
  }
}

void Ade7953GetData(void) {
  uint32_t acc_mode = 0;
  int32_t reg[ADE7953_MAX_CHANNEL][ADE7953_REGISTERS];

#ifdef USE_ESP32_SPI
  if (Ade7953.use_spi) {
    uint32_t channel = 0;
    for (uint32_t chip = 0; chip < ADE7953_MAX_CHANNEL / 2; chip++) {
      if (Ade7953.pin_cs[chip] < 0) { continue; }
      Ade7953.cs_index = chip;
      for (uint32_t i = 0; i < ADE7953_REGISTERS; i++) {
        reg[channel][i] = Ade7953Read(Ade7953Registers[0][i]);    // IRMSa, WATTa, VAa, VARa, VRMS, Period
      }
      channel++;
      if (4 == Energy->phase_count) {
        for (uint32_t i = 0; i < ADE7953_REGISTERS; i++) {
          reg[channel][i] = Ade7953Read(Ade7953Registers[1][i]);  // IRMSb, WATTb, VAb, VARb, VRMS, Period
        }
        channel++;
      }
    }
  } else {
#endif  // USE_ESP32_SPI
    for (uint32_t channel = 0; channel < 2; channel++) {
      uint32_t channel_swap = (ADE7953_SHELLY_25 == Ade7953.model) ? !channel : channel;
      for (uint32_t i = 0; i < ADE7953_REGISTERS; i++) {
        reg[channel_swap][i] = Ade7953Read(Ade7953Registers[channel][i]);
      }
    }
    acc_mode = Ade7953Read(ADE7953_ACCMODE);         // Accumulation mode
#ifdef USE_ESP32_SPI
  }
#endif  // USE_ESP32_SPI

  for (uint32_t i = 0; i < Energy->phase_count; i++) {
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("ADE: Channel%d ACCMODE 0x%06X, VRMS %d, Period %d, IRMS %d, WATT %d, VA %d, VAR %d"),
      i+1, acc_mode, reg[i][4], reg[i][5], reg[i][0], reg[i][1], reg[i][2], reg[i][3]);
  }

  // If the device is initializing, we read the energy registers to reset them, but don't report the values as the first read may be inaccurate
  if (Ade7953.init_step) { return; }

  uint32_t apparent_power[ADE7953_MAX_CHANNEL] = { 0, 0 };
  uint32_t reactive_power[ADE7953_MAX_CHANNEL] = { 0, 0 };

  for (uint32_t channel = 0; channel < Energy->phase_count; channel++) {
    Ade7953.voltage_rms[channel] = reg[channel][4];
    Ade7953.current_rms[channel] = reg[channel][0];
    Ade7953.active_power[channel] = abs(reg[channel][1]);
    apparent_power[channel] = abs(reg[channel][2]);
    reactive_power[channel] = abs(reg[channel][3]);
    if ((ADE7953_SHELLY_EM == Ade7953.model) && (bitRead(acc_mode, 18 +(channel * 3)))) {  // VARNLOAD
      reactive_power[channel] = 0;
    }
  }

  if (Energy->power_on) {                              // Powered on

#ifdef USE_ESP32_SPI
    float correction = 1.0f;
    if (Ade7953.use_spi) {        // SPI
      uint32_t time = millis();
      if (Ade7953.last_update) {
        uint32_t difference = time - Ade7953.last_update;
        correction = (float)difference / 1000;    // Correction to 1 second

//        AddLog(LOG_LEVEL_DEBUG, PSTR("ADE: Correction %4_f"), &correction);
      }
      Ade7953.last_update = time;
    }
#endif  // USE_ESP32_SPI

    float divider;
    for (uint32_t channel = 0; channel < Energy->phase_count; channel++) {
      Energy->data_valid[channel] = 0;

      float power_calibration = (float)EnergyGetCalibration(ENERGY_POWER_CALIBRATION, channel) / 10;
#ifdef ADE7953_ACCU_ENERGY
      power_calibration /= ADE7953_POWER_CORRECTION;
#endif  // ADE7953_ACCU_ENERGY
      float voltage_calibration = (float)EnergyGetCalibration(ENERGY_VOLTAGE_CALIBRATION, channel);
      float current_calibration = (float)EnergyGetCalibration(ENERGY_CURRENT_CALIBRATION, channel) * 10;

      Energy->frequency[channel] = 223750.0f / ((float)reg[channel][5] + 1);

      divider = (Ade7953.calib_data[channel][ADE7953_CAL_VGAIN] != ADE7953_GAIN_DEFAULT) ? 10000 : voltage_calibration;
      Energy->voltage[channel] = (float)Ade7953.voltage_rms[channel] / divider;

      divider = (Ade7953.calib_data[channel][ADE7953_CAL_WGAIN] != ADE7953_GAIN_DEFAULT) ? ADE7953_LSB_PER_WATTSECOND : power_calibration;
      Energy->active_power[channel] = (float)Ade7953.active_power[channel] / divider;

      divider = (Ade7953.calib_data[channel][ADE7953_CAL_VARGAIN] != ADE7953_GAIN_DEFAULT) ? ADE7953_LSB_PER_WATTSECOND : power_calibration;
      Energy->reactive_power[channel] = (float)reactive_power[channel] / divider;

      if (ADE7953_SHELLY_EM == Ade7953.model) {
        if (bitRead(acc_mode, 10 +channel)) {        // APSIGN
          Energy->active_power[channel] *= -1;
        }
        if (bitRead(acc_mode, 12 +channel)) {        // VARSIGN
          Energy->reactive_power[channel] *= -1;
        }
      }

      divider = (Ade7953.calib_data[channel][ADE7953_CAL_VAGAIN] != ADE7953_GAIN_DEFAULT) ? ADE7953_LSB_PER_WATTSECOND : power_calibration;
      Energy->apparent_power[channel] = (float)apparent_power[channel] / divider;

#ifdef USE_ESP32_SPI
      if (Ade7953.use_spi) {        // SPI
        Energy->active_power[channel] /= correction;
        Energy->reactive_power[channel] /= correction;
        Energy->apparent_power[channel] /= correction;
      }
#endif  // USE_ESP32_SPI

      if (0 == Energy->active_power[channel]) {
        Energy->current[channel] = 0;
      } else {
        divider = (Ade7953.calib_data[channel][ADE7953_CAL_IGAIN] != ADE7953_GAIN_DEFAULT) ? 100000 : current_calibration;
        Energy->current[channel] = (float)Ade7953.current_rms[channel] / divider;
        Energy->kWhtoday_delta[channel] += Energy->active_power[channel] * 1000 / 36;
      }
    }
    EnergyUpdateToday();
  }
}

void Ade7953EnergyEverySecond(void) {
  if (Ade7953.init_step) {
    if (2 == Ade7953.init_step) { Ade7953Init(); }
    if (1 == Ade7953.init_step) { Ade7953GetData(); }  // Read registers but do not display yet
    Ade7953.init_step--;
  } else {
    Ade7953GetData();
  }
}

/*********************************************************************************************/

bool Ade7953SetDefaults(const char* json) {
  // {"angles":{"angle0":180,"angle1":176}}
  // {"rms":{"current_a":4194303,"current_b":4194303,"voltage":1613194},"angles":{"angle0":0,"angle1":0},"powers":{"totactive":{"a":2723574,"b":2723574},"apparent":{"a":2723574,"b":2723574},"reactive":{"a":2723574,"b":2723574}}}
  // {"rms":{"current_a":21865738,"current_b":1558533,"voltage_a":1599149,"voltage_b":1597289},"angles":{"angle0":0,"angle1":0},"powers":{"totactive":{"a":106692616,"b":3540894}}}
  uint32_t len = strlen(json) +1;
  if (len < 7) { return false; }                     // Too short

  char json_buffer[len];
  memcpy(json_buffer, json, len);                    // Keep original safe
  JsonParser parser(json_buffer);
  JsonParserObject root = parser.getRootObject();
  if (!root) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("ADE: Invalid JSON"));
    return false;
  }

  // All parameters are optional allowing for partial changes
  JsonParserToken val;
  char field[20];
  for (uint32_t i = 0; i < ADE7953_MAX_CHANNEL; i++) {
    JsonParserObject rms = root[PSTR("rms")].getObject();
    if (rms) {
      val = rms[PSTR("voltage")];
      if (val) {
        Ade7953.calib_data[i][ADE7953_CAL_VGAIN] = val.getInt();
      }
  #ifdef USE_ESP32_SPI
      snprintf_P(field, sizeof(field), PSTR("voltage_%c"), 'a'+i);
      val = rms[field];                              // "voltage_a" .. "voltage_d"
      if (val) { Ade7953.calib_data[i][ADE7953_CAL_VGAIN] = val.getInt(); }
  #endif  // USE_ESP32_SPI
      snprintf_P(field, sizeof(field), PSTR("current_%c"), 'a'+i);
      val = rms[field];                              // "current_a" .. "current_d"
      if (val) { Ade7953.calib_data[i][ADE7953_CAL_IGAIN] = val.getInt(); }
    }
    JsonParserObject angles = root[PSTR("angles")].getObject();
    if (angles) {
      snprintf_P(field, sizeof(field), PSTR("angle%c"), '0'+i);
      val = angles[field];                           // "angle0" .. "angle3"
      if (val) { Ade7953.calib_data[i][ADE7943_CAL_PHCAL] = val.getInt(); }
    }
    JsonParserObject powers = root[PSTR("powers")].getObject();
    if (powers) {
      snprintf_P(field, sizeof(field), PSTR("%c"), 'a'+i);
      JsonParserObject totactive = powers[PSTR("totactive")].getObject();
      if (totactive) {
        val = totactive[field];                      // "a" .. "d"
        if (val) { Ade7953.calib_data[i][ADE7953_CAL_WGAIN] = val.getInt(); }
      }
      JsonParserObject apparent = powers[PSTR("apparent")].getObject();
      if (apparent) {
        val = apparent[field];                       // "a" .. "d"
        if (val) { Ade7953.calib_data[i][ADE7953_CAL_VAGAIN] = val.getInt(); }
      }
      JsonParserObject reactive = powers[PSTR("reactive")].getObject();
      if (reactive) {
        val = reactive[field];                       // "a" .. "d"
        if (val) { Ade7953.calib_data[i][ADE7953_CAL_VARGAIN] = val.getInt(); }
      }
    }
  }
  return true;
}

void Ade7953Defaults(void) {
  for (uint32_t channel = 0; channel < ADE7953_MAX_CHANNEL; channel++) {
    for (uint32_t i = 0; i < ADE7953_CALIBREGS; i++) {
      if (ADE7943_CAL_PHCAL == i) {
        Ade7953.calib_data[channel][i] = (ADE7953_SHELLY_EM == Ade7953.model) ? ADE7953_PHCAL_DEFAULT_CT : ADE7953_PHCAL_DEFAULT;
      } else {
        Ade7953.calib_data[channel][i] = ADE7953_GAIN_DEFAULT;
      }
    }
  }

  String calib = "";
#ifdef USE_UFILESYS
  calib = TfsLoadString("/calib.dat");
#endif  // USE_UFILESYS
#ifdef USE_RULES
  // rule3 on file#calib.dat do {"angles":{"angle0":180,"angle1":176}} endon
  if (!calib.length()) {
    calib = RuleLoadFile("CALIB.DAT");
  }
#endif  // USE_RULES
  if (calib.length()) {
//    AddLog(LOG_LEVEL_DEBUG, PSTR("ADE: File '%s'"), calib.c_str());
    Ade7953SetDefaults(calib.c_str());
  }
}

void Ade7953DrvInit(void) {
  if (PinUsed(GPIO_ADE7953_IRQ, GPIO_ANY)) {         // Irq is not supported...
    uint32_t pin_irq = Pin(GPIO_ADE7953_IRQ, GPIO_ANY);
    pinMode(pin_irq, INPUT);                         // Related to resetPins() - Must be set to input
    // 0 (1 = Shelly 2.5), 1 (2 = Shelly EM), 2 (3 = Shelly Plus 2PM), 3 (4 = Shelly Pro 1PM), 4 (5 = Shelly Pro 2PM), 5 (6 = Shelly Pro 4PM)
    Ade7953.model = GetPin(pin_irq) - AGPIO(GPIO_ADE7953_IRQ);

    int pin_reset = Pin(GPIO_ADE7953_RST);           // -1 if not defined
#ifdef ESP8266
    if (ADE7953_SHELLY_EM == Ade7953.model) {
      if (-1 == pin_reset) {
        pin_reset = 16;
      }
    }
#endif
    if (pin_reset >= 0) {
      digitalWrite(pin_reset, 0);
      pinMode(pin_reset, OUTPUT);                    // Reset pin ADE7953
      delay(1);                                      // To initiate a hardware reset, this pin must be brought low for a minimum of 10 μs.
      digitalWrite(pin_reset, 1);
      if (Ade7953.model < ADE7953_SHELLY_PRO_1PM) {
        pinMode(pin_reset, INPUT);
      }
    }
#ifdef USE_ESP32_SPI
#if (defined(USE_SHELLY_PRO) && defined(USE_MCP23XXX_DRV)) || defined(USE_SHELLY_PRO_V2)
    if (Ade7953.model == ADE7953_SHELLY_PRO_4PM) {
      ShellyPro4Reset();
    }
#endif  // USE_SHELLY_PRO
#endif  // USE_ESP32_SPI
    delay(100);                                      // Need 100mS to init ADE7953

#ifdef USE_ESP32_SPI
    Ade7953.pin_cs[0] = -1;
    Ade7953.pin_cs[1] = -1;
    if (Ade7953.model >= ADE7953_SHELLY_PRO_1PM) {   // SPI
      if (PinUsed(GPIO_ADE7953_CS)) {                // ADE7953 CS1 enabled (Pro 1PM/2PM)
        Ade7953.pin_cs[0] = Pin(GPIO_ADE7953_CS);
        digitalWrite(Ade7953.pin_cs[0], 1);          // ADE7953 CS1 enabled (Pro 2PM)
        pinMode(Ade7953.pin_cs[0], OUTPUT);
        Ade7953.pin_cs[1] = Pin(GPIO_ADE7953_CS, 1);
        if (Ade7953.pin_cs[1] > -1) {                // ADE7953 CS2 enabled (Pro 2PM)
          digitalWrite(Ade7953.pin_cs[1], 1);
          pinMode(Ade7953.pin_cs[1], OUTPUT);
        } else {
          Ade7953.model = ADE7953_SHELLY_PRO_1PM;
        }
        Ade7953.cs_index = 0;
        Ade7953.use_spi = true;
        SPI.begin(Pin(GPIO_SPI_CLK), Pin(GPIO_SPI_MISO), Pin(GPIO_SPI_MOSI), -1);
        AddLog(LOG_LEVEL_INFO, PSTR("SPI: ADE7953 found"));
      } else {
        return;                                       // No CS pin defined
      }
    } else {
#endif  // USE_ESP32_SPI
      if (!I2cSetDevice(ADE7953_ADDR)) {
        return;
      }
      I2cSetActiveFound(ADE7953_ADDR, "ADE7953");
#ifdef USE_ESP32_SPI
    }
#endif  // USE_ESP32_SPI
    if (EnergyGetCalibration(ENERGY_POWER_CALIBRATION) == HLW_PREF_PULSE) {
      for (uint32_t i = 0; i < ADE7953_MAX_CHANNEL; i++) {
        EnergySetCalibration(ENERGY_POWER_CALIBRATION, ADE7953_PREF, i);
        EnergySetCalibration(ENERGY_VOLTAGE_CALIBRATION, ADE7953_UREF, i);
        EnergySetCalibration(ENERGY_CURRENT_CALIBRATION, ADE7953_IREF, i);
      }
    }

    Ade7953Defaults();

    Ade7953.init_step = 3;

//    Energy->phase_count = 1;
//    Energy->voltage_common = false;
//    Energy->frequency_common = false;
//    Energy->use_overtemp = false;
    if (ADE7953_SHELLY_PRO_1PM == Ade7953.model) {
    } else {
      Energy->phase_count = 2;                        // Handle two channels as two phases
      if (ADE7953_SHELLY_PRO_2PM == Ade7953.model) {
      } else {
        Energy->voltage_common = true;                // Use common voltage
        Energy->frequency_common = true;              // Use common frequency
        if (ADE7953_SHELLY_PRO_4PM == Ade7953.model) {
          Energy->phase_count = 4;
        }
      }
    }
    Energy->use_overtemp = true;                      // Use global temperature for overtemp detection
    if (ADE7953_SHELLY_EM == Ade7953.model) {
      Energy->local_energy_active_export = true;
    }
    TasmotaGlobal.energy_driver = XNRG_07;
  }
}

bool Ade7953Command(void) {
  bool serviced = true;

  if (XdrvMailbox.index > ADE7953_MAX_CHANNEL) { return false; };
  uint32_t channel = XdrvMailbox.index -1;
  if (ADE7953_SHELLY_PRO_4PM != Ade7953.model) {
    channel = (2 == XdrvMailbox.index) ? 1 : 0;
  }
  uint32_t value = (uint32_t)(CharToFloat(XdrvMailbox.data) * 100);  // 1.23 = 123

  if (CMND_POWERCAL == Energy->command_code) {
    if (1 == XdrvMailbox.payload) { XdrvMailbox.payload = ADE7953_PREF; }
    // Service in xdrv_03_energy.ino
  }
  else if (CMND_VOLTAGECAL == Energy->command_code) {
    if (1 == XdrvMailbox.payload) { XdrvMailbox.payload = ADE7953_UREF; }
    // Service in xdrv_03_energy.ino
  }
  else if (CMND_CURRENTCAL == Energy->command_code) {
    if (1 == XdrvMailbox.payload) { XdrvMailbox.payload = ADE7953_IREF; }
    // Service in xdrv_03_energy.ino
  }
  else if (CMND_POWERSET == Energy->command_code) {
    if (XdrvMailbox.data_len && Ade7953.active_power[channel]) {
      if ((value > 100) && (value < 2000000)) {       // Between 1W and 20000W
#ifdef ADE7953_ACCU_ENERGY
        float power_calibration = (float)(Ade7953.active_power[channel] * 1000) / value;  // 0.00 W
        power_calibration *= ADE7953_POWER_CORRECTION;
        XdrvMailbox.payload = (uint32_t)power_calibration;  // 0.00 W
#else   // No ADE7953_ACCU_ENERGY
        XdrvMailbox.payload = (Ade7953.active_power[channel] * 1000) / value;  // 0.00 W
#endif  // ADE7953_ACCU_ENERGY
      }
    }
  }
  else if (CMND_VOLTAGESET == Energy->command_code) {
    if (XdrvMailbox.data_len && Ade7953.voltage_rms[channel]) {
      if ((value > 10000) && (value < 40000)) {      // Between 100V and 400V
        XdrvMailbox.payload = (Ade7953.voltage_rms[channel] * 100) / value;  // 0.00 V
      }
    }
  }
  else if (CMND_CURRENTSET == Energy->command_code) {
    if (XdrvMailbox.data_len && Ade7953.current_rms[channel]) {
      if ((value > 2000) && (value < 10000000)) {     // Between 20mA and 100A
        XdrvMailbox.payload = ((Ade7953.current_rms[channel] * 100) / value) * 100;  // 0.00 mA
      }
    }
  }
  else serviced = false;                             // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg07(uint32_t function) {
  if (!I2cEnabled(XI2C_07) && (SPI_MOSI_MISO != TasmotaGlobal.spi_enabled)) { return false; }

  bool result = false;

  switch (function) {
#ifdef USE_ESP32_SPI
    case FUNC_ENERGY_EVERY_SECOND:  // Use energy interrupt timer (fails on SPI)
      if (!Ade7953.use_spi) {       // No SPI
        Ade7953EnergyEverySecond();
      }
      break;
    case FUNC_EVERY_SECOND:         // Use loop timer (without interrupt)
      if (Ade7953.use_spi) {        // SPI
        Ade7953EnergyEverySecond();
      }
      break;
#else   // ESP8266
    case FUNC_ENERGY_EVERY_SECOND:  // Use energy interrupt timer
      Ade7953EnergyEverySecond();
      break;
#endif  // USE_ESP32_SPI
    case FUNC_COMMAND:
      result = Ade7953Command();
      break;
    case FUNC_PRE_INIT:
      Ade7953DrvInit();
      break;
  }
  return result;
}

#endif  // USE_ADE7953
#endif  // USE_ENERGY_SENSOR
#endif  // USE_I2C or USE_ESP_SPI
