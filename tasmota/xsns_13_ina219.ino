/*
  xsns_13_ina219.ino - INA219 Current Sensor support for Tasmota

  Copyright (C) 2021  Stefan Bode and Theo Arends

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

#ifdef USE_I2C
#ifdef USE_INA219
/*********************************************************************************************\
 * INA219 - Low voltage (max 32V!) Current sensor
 *
 * Source: Adafruit Industries
 *
 * I2C Address: 0x40, 0x41 0x44 or 0x45
\*********************************************************************************************/

#define XSNS_13                                 13
#define XI2C_14                                 14        // See I2CDEVICES.md

#define INA219_ADDRESS1                         (0x40)    // 1000000 (A0+A1=GND)
#define INA219_ADDRESS2                         (0x41)    // 1000000 (A0=Vcc, A1=GND)
#define INA219_ADDRESS3                         (0x44)    // 1000000 (A0=GND, A1=Vcc)
#define INA219_ADDRESS4                         (0x45)    // 1000000 (A0+A1=Vcc)

#define INA219_READ                             (0x01)
#define INA219_REG_CONFIG                       (0x00)

#define INA219_CONFIG_RESET                     (0x8000)  // Reset Bit

#define INA219_CONFIG_BVOLTAGERANGE_MASK        (0x2000)  // Bus Voltage Range Mask
#define INA219_CONFIG_BVOLTAGERANGE_16V         (0x0000)  // 0-16V Range
#define INA219_CONFIG_BVOLTAGERANGE_32V         (0x2000)  // 0-32V Range

#define INA219_CONFIG_GAIN_MASK                 (0x1800)  // Gain Mask
#define INA219_CONFIG_GAIN_1_40MV               (0x0000)  // Gain 1, 40mV Range
#define INA219_CONFIG_GAIN_2_80MV               (0x0800)  // Gain 2, 80mV Range
#define INA219_CONFIG_GAIN_4_160MV              (0x1000)  // Gain 4, 160mV Range
#define INA219_CONFIG_GAIN_8_320MV              (0x1800)  // Gain 8, 320mV Range

#define INA219_CONFIG_BADCRES_MASK              (0x0780)  // Bus ADC Resolution Mask
#define INA219_CONFIG_BADCRES_9BIT_1S_84US      (0x0<<7)  // 9-bit bus res = 0..511
#define INA219_CONFIG_BADCRES_10BIT_1S_148US    (0x1<<7)  // 10-bit bus res = 0..1023
#define INA219_CONFIG_BADCRES_11BIT_1S_276US    (0x2<<7)  // 11-bit bus res = 0..2047
#define INA219_CONFIG_BADCRES_12BIT_1S_532US    (0x3<<7)  // 12-bit bus res = 0..4097
#define INA219_CONFIG_BADCRES_12BIT_2S_1060US   (0x9<<7)  // 2 x 12-bit bus samples averaged together
#define INA219_CONFIG_BADCRES_12BIT_4S_2130US   (0xA<<7)  // 4 x 12-bit bus samples averaged together
#define INA219_CONFIG_BADCRES_12BIT_8S_4260US   (0xB<<7)  // 8 x 12-bit bus samples averaged together
#define INA219_CONFIG_BADCRES_12BIT_16S_8510US  (0xC<<7)  // 16 x 12-bit bus samples averaged together
#define INA219_CONFIG_BADCRES_12BIT_32S_17MS    (0xD<<7)  // 32 x 12-bit bus samples averaged together
#define INA219_CONFIG_BADCRES_12BIT_64S_34MS    (0xE<<7)  // 64 x 12-bit bus samples averaged together
#define INA219_CONFIG_BADCRES_12BIT_128S_69MS   (0xF<<7)  // 128 x 12-bit bus samples averaged together

#define INA219_CONFIG_SADCRES_MASK              (0x0078)  // Shunt ADC Resolution and Averaging Mask
#define INA219_CONFIG_SADCRES_9BIT_1S_84US      (0x0<<3)  // 1 x 9-bit shunt sample
#define INA219_CONFIG_SADCRES_10BIT_1S_148US    (0x1<<3)  // 1 x 10-bit shunt sample
#define INA219_CONFIG_SADCRES_11BIT_1S_276US    (0x2<<3)  // 1 x 11-bit shunt sample
#define INA219_CONFIG_SADCRES_12BIT_1S_532US    (0x3<<3)  // 1 x 12-bit shunt sample
#define INA219_CONFIG_SADCRES_12BIT_2S_1060US   (0x9<<3)  // 2 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_4S_2130US   (0xA<<3)  // 4 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_8S_4260US   (0xB<<3)  // 8 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_16S_8510US  (0xC<<3)  // 16 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_32S_17MS    (0xD<<3)  // 32 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_64S_34MS    (0xE<<3)  // 64 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_128S_69MS   (0xF<<3)  // 128 x 12-bit shunt samples averaged together

#define INA219_CONFIG_MODE_MASK                 (0x0007)  // Operating Mode Mask
#define INA219_CONFIG_MODE_POWERDOWN            (0x0000)
#define INA219_CONFIG_MODE_SVOLT_TRIGGERED      (0x0001)
#define INA219_CONFIG_MODE_BVOLT_TRIGGERED      (0x0002)
#define INA219_CONFIG_MODE_SANDBVOLT_TRIGGERED  (0x0003)
#define INA219_CONFIG_MODE_ADCOFF               (0x0004)
#define INA219_CONFIG_MODE_SVOLT_CONTINUOUS     (0x0005)
#define INA219_CONFIG_MODE_BVOLT_CONTINUOUS     (0x0006)
#define INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS (0x0007)

#define INA219_REG_SHUNTVOLTAGE                 (0x01)
#define INA219_REG_BUSVOLTAGE                   (0x02)
#define INA219_REG_POWER                        (0x03)
#define INA219_REG_CURRENT                      (0x04)
#define INA219_REG_CALIBRATION                  (0x05)

#define INA219_DEFAULT_SHUNT_RESISTOR_MILLIOHMS (100.0) // 0.1 Ohm

uint8_t ina219_type[4] = {0,0,0,0};
uint8_t ina219_addresses[] = { INA219_ADDRESS1, INA219_ADDRESS2, INA219_ADDRESS3, INA219_ADDRESS4 };

#ifdef DEBUG_TASMOTA_SENSOR
// temporary strings for floating point in debug messages
char __ina219_dbg1[10];
char __ina219_dbg2[10];
#endif

// The following multiplier is used to convert shunt voltage (in mV) to current (in A)
// Current_A = ShuntVoltage_mV / ShuntResistor_milliOhms = ShuntVoltage_mV * ina219_current_multiplier
// ina219_current_multiplier = 1 / ShuntResistor_milliOhms
float ina219_current_multiplier;

uint8_t ina219_valid[4] = {0,0,0,0};
float ina219_voltage[4] = {0,0,0,0};
float ina219_current[4] = {0,0,0,0};
char ina219_types[] = "INA219";
uint8_t ina219_count = 0;

/*********************************************************************************************\
 * Calculate current multiplier depending on the selected mode
 * For mode = 0, 1, 2 : legacy modes simplified as Vmax: 32V, Imax: 3.2A range
 * For mode = 10..255 : specify Rshunt encoded as RRM where resistor value is RR * 10^M milliOhms
 *                      Vmax: 32V, Imax: 0.320 / Rshunt
 * Exemple:
 *     10: Rshunt = 1 * 10^0 =  1 millOhms   => Max current = 320A !
 *     11: Rshunt = 1 * 10^1 = 10 milliOhms  => Max current = 32A
 *     21: Rshunt = 2 * 10^1 = 20 milliOhms  => Max current = 16A
 *     12: Rshunt = 1 * 10^2 = 100 milliOhms => Max current = 3.2A == mode 0,1,2
 *     13: Rshunt = 1 * 10^3 =   1 Ohms      => Max current = 320mA
 * Note that some shunt values can be represented by 2 different encoded values such as
 *     11 or 100 both present 10 milliOhms
 * Because it is difficult to make a range check on such encoded value, none is performed
\*********************************************************************************************/
bool Ina219SetCalibration(uint8_t mode, uint16_t addr)
{
  uint16_t config = 0;

  DEBUG_SENSOR_LOG("Ina219SetCalibration: mode=%d",mode);
  if (mode < 5)
  {
    // All legacy modes 0..2 are handled the same and consider default 0.1 shunt resistor
    ina219_current_multiplier = 1.0 / INA219_DEFAULT_SHUNT_RESISTOR_MILLIOHMS;
    #ifdef DEBUG_TASMOTA_SENSOR
    dtostrfd(ina219_current_multiplier,5,__ina219_dbg1);
    DEBUG_SENSOR_LOG("Ina219SetCalibration: cur_mul=%s",__ina219_dbg1);
    #endif
  }
  else if (mode >= 10)
  {
    int mult = mode % 10;
    int shunt_milliOhms = mode / 10;
    for ( ; mult > 0 ; mult-- )
      shunt_milliOhms *= 10;
    ina219_current_multiplier = 1.0 / shunt_milliOhms;
    #ifdef DEBUG_TASMOTA_SENSOR
    dtostrfd(ina219_current_multiplier,5,__ina219_dbg1);
    DEBUG_SENSOR_LOG("Ina219SetCalibration: shunt=%dmO => cur_mul=%s",shunt_milliOhms,__ina219_dbg1);
    #endif
  }
  config = INA219_CONFIG_BVOLTAGERANGE_32V
         | INA219_CONFIG_GAIN_8_320MV               // Use max scale
         | INA219_CONFIG_BADCRES_12BIT_16S_8510US   // use averaging to improve accuracy
         | INA219_CONFIG_SADCRES_12BIT_16S_8510US   // use averaging to improve accuracy
         | INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
  // Set Config register to take into account the settings above
  return I2cWrite16(addr, INA219_REG_CONFIG, config);
}

float Ina219GetShuntVoltage_mV(uint16_t addr)
{
  // raw shunt voltage (16-bit signed integer, so +-32767)
  int16_t value = I2cReadS16(addr, INA219_REG_SHUNTVOLTAGE);
  DEBUG_SENSOR_LOG("Ina219GetShuntVoltage_mV: ShReg = 0x%04X",value);
  // convert to shunt voltage in mV (so +-327mV) (LSB=10µV=0.01mV)
  return value * 0.01;
}

float Ina219GetBusVoltage_V(uint16_t addr)
{
  // Shift 3 to the right to drop CNVR and OVF as unsigned
  uint16_t value = I2cRead16(addr, INA219_REG_BUSVOLTAGE) >> 3;
  DEBUG_SENSOR_LOG("Ina219GetBusVoltage_V: BusReg = 0x%04X",value);
  // and multiply by LSB raw bus voltage to return bus voltage in volts (LSB=4mV=0.004V)
  return value * 0.004;
}

/* Not used any more
float Ina219GetCurrent_mA(uint16_t addr)
{
  // Sometimes a sharp load will reset the INA219, which will reset the cal register,
  // meaning CURRENT and POWER will not be available ... avoid this by always setting
  // a cal value even if it's an unfortunate extra step
  I2cWrite16(addr, INA219_REG_CALIBRATION, ina219_cal_value);
  // Now we can safely read the CURRENT register!
  // raw current value (16-bit signed integer, so +-32767)
  float value = I2cReadS16(addr, INA219_REG_CURRENT);
  value /= ina219_current_divider_ma;
  // current value in mA, taking into account the config settings and current LSB
  return value;
}
*/

bool Ina219Read(void)
{
  for (int i=0; i<sizeof(ina219_type); i++) {
    if (!ina219_type[i]) { continue; }
    uint16_t addr = ina219_addresses[i];
    float bus_voltage_V = Ina219GetBusVoltage_V(addr);
    float shunt_voltage_mV = Ina219GetShuntVoltage_mV(addr);
    #ifdef DEBUG_TASMOTA_SENSOR
    dtostrfd(bus_voltage_V,5,__ina219_dbg1);
    dtostrfd(shunt_voltage_mV,5,__ina219_dbg2);
    DEBUG_SENSOR_LOG("Ina219Read: bV=%sV, sV=%smV",__ina219_dbg1,__ina219_dbg2);
    #endif
    // we return the power-supply-side voltage (as bus_voltage register provides the load-side voltage)
    ina219_voltage[i] = bus_voltage_V + (shunt_voltage_mV / 1000);
    // current is simply calculted from shunt voltage using pre-calculated multiplier
    ina219_current[i] = shunt_voltage_mV * ina219_current_multiplier;
    #ifdef DEBUG_TASMOTA_SENSOR
    dtostrfd(ina219_voltage[i],5,__ina219_dbg1);
    dtostrfd(ina219_current[i],5,__ina219_dbg2);
    DEBUG_SENSOR_LOG("Ina219Read: V=%sV, I=%smA",__ina219_dbg1,__ina219_dbg2);
    #endif
    ina219_valid[i] = SENSOR_MAX_MISS;
    //  AddLogMissed(ina219_types, ina219_valid);
  }
  return true;
}

/*********************************************************************************************\
 * Command Sensor13
\*********************************************************************************************/

bool Ina219CommandSensor(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 255)) {
    Settings->ina219_mode = XdrvMailbox.payload;
    TasmotaGlobal.restart_flag = 2;
  }
  Response_P(S_JSON_SENSOR_INDEX_NVALUE, XSNS_13, Settings->ina219_mode);

  return true;
}

/********************************************************************************************/

void Ina219Detect(void)
{
  for (uint32_t i = 0; i < sizeof(ina219_type); i++) {
    uint16_t addr = ina219_addresses[i];
    if (!I2cSetDevice(addr)) { continue; }
    if (Ina219SetCalibration(Settings->ina219_mode, addr)) {
      I2cSetActiveFound(addr, ina219_types);
      ina219_type[i] = 1;
      ina219_count++;
    }
  }
}

void Ina219EverySecond(void)
{
  // 4 x 3mS
  Ina219Read();
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_INA219_DATA[] PROGMEM =
  "{s}%s " D_VOLTAGE "{m}%s " D_UNIT_VOLT "{e}"
  "{s}%s " D_CURRENT "{m}%s " D_UNIT_AMPERE "{e}"
  "{s}%s " D_POWERUSAGE "{m}%s " D_UNIT_WATT "{e}";
#endif  // USE_WEBSERVER

void Ina219Show(bool json)
{
  int num_found=0;
  for (int i=0; i<sizeof(ina219_type); i++)
    if (ina219_type[i] && ina219_valid[i])
      num_found++;

  int sensor_num = 0;
  for (int i=0; i<sizeof(ina219_type); i++) {
    if (!ina219_type[i] || !ina219_valid[i])
      continue;
    sensor_num++;

    char voltage[16];
    dtostrfd(ina219_voltage[i], Settings->flag2.voltage_resolution, voltage);
    char current[16];
    dtostrfd(ina219_current[i], Settings->flag2.current_resolution, current);
    char power[16];
    dtostrfd(ina219_voltage[i] * ina219_current[i], Settings->flag2.wattage_resolution, power);
    char name[16];
    if (num_found>1)
      snprintf_P(name, sizeof(name), PSTR("%s%c%d"), ina219_types, IndexSeparator(), sensor_num);
    else
      snprintf_P(name, sizeof(name), PSTR("%s"), ina219_types);

    if (json) {
      ResponseAppend_P(PSTR(",\"%s\":{\"Id\":%02x,\"" D_JSON_VOLTAGE "\":%s,\"" D_JSON_CURRENT "\":%s,\"" D_JSON_POWERUSAGE "\":%s}"),
                       name, ina219_addresses[i], voltage, current, power);
#ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) {
        DomoticzSensor(DZ_VOLTAGE, voltage);
        DomoticzSensor(DZ_CURRENT, current);
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_INA219_DATA, name, voltage, name, current, name, power);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns13(uint8_t function)
{
  if (!I2cEnabled(XI2C_14)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Ina219Detect();
  }
  else if (ina219_count) {
    switch (function) {
      case FUNC_COMMAND_SENSOR:
        if (XSNS_13 == XdrvMailbox.index) {
          result = Ina219CommandSensor();
        }
        break;
      case FUNC_EVERY_SECOND:
        Ina219EverySecond();
        break;
      case FUNC_JSON_APPEND:
        Ina219Show(1);
        break;
  #ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Ina219Show(0);
        break;
  #endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_INA219
#endif  // USE_I2C
