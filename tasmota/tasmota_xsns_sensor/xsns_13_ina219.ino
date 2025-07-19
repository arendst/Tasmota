/*
  xsns_13_ina219.ino - INA219 & ISL28022 Current Sensor support for Tasmota

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
 * Supports also ISL28022
 *
 * Source: Adafruit Industries
 *
 * I2C Address: 0x40, 0x41 0x44 or 0x45
\*********************************************************************************************/

#define XSNS_13                                 13
#define XI2C_14                                 14        // See I2CDEVICES.md

#ifndef INA219_MAX_COUNT
#define INA219_MAX_COUNT                        4
#endif
#if (INA219_MAX_COUNT > 4)
#error "**** INA219_MAX_COUNT can't be greater than 4 ****"
#endif
#ifndef INA219_FIRST_ADDRESS
#define INA219_FIRST_ADDRESS  (0)
#endif
#if ((INA219_FIRST_ADDRESS + INA219_MAX_COUNT) > 4)
#error "**** INA219 bad combination for FIRST_ADDRESS and MAX_COUNT ****"
#endif

#ifndef INA219_SHUNT_RESISTOR
#define INA219_SHUNT_RESISTOR                   (0.100) // 0.1 Ohm default on most INA219 modules
#endif

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
#define ISL28022_CONFIG_BVOLTAGERANGE_60V       (0x6000)  // 0-60V Range for ISL28022

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

// Note: for IS28022, the ADC has 3 more bits and approximatively similar conversion times
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
#define ISL28022_REG_SHUNTTHRESHOLD             (0x06)
#define ISL28022_REG_BUSTHRESHOLD               (0x07)
#define ISL28022_REG_INTRSTATUS                 (0x08)
#define ISL28022_REG_AUXCTRL                    (0x09)

#define INA219_BUS_ADC_LSB                      (0.004)  // VBus   ADC LSB=4mV=0.004V
#define INA219_SHUNT_ADC_LSB_MV                 (0.01)   // VShunt ADC LSB=10µV=0.01mV



#ifdef DEBUG_TASMOTA_SENSOR
// temporary strings for floating point in debug messages
char __ina219_dbg1[FLOATSZ];
char __ina219_dbg2[FLOATSZ];
#endif

#define INA219_MODEL     1
#define ISL28022_MODEL   2

struct INA219_Data {
  float     voltage;
  float     current;
  // The following multiplier is used to convert shunt voltage (in mV) to current (in A)
  // Current_A = ShuntVoltage_mV / ShuntResistor_milliOhms = ShuntVoltage_mV * ina219_current_multiplier
  // ina219_current_multiplier = 1 / ShuntResistor_milliOhms
  float     current_multiplier;
  uint8_t   model;
  uint8_t   addr;
};

struct INA219_Data *Ina219Data = nullptr;
uint8_t Ina219Count = 0;

const char INA219_SENSORCMND_START[] PROGMEM = "{\"" D_CMND_SENSOR "%d\":{\"mode\":%d,\"rshunt\":[";
const char INA219_SENSORCMND_END[] PROGMEM = "]}}";

const char *INA219_TYPE[] = { "INA219", "ISL28022" };
const uint8_t INA219_ADDRESSES[] = { INA219_ADDRESS1, INA219_ADDRESS2, INA219_ADDRESS3, INA219_ADDRESS4 };


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
void Ina219SetShuntMode(uint8_t index, uint8_t mode, float shunt)
{
  if (mode < 10) {
    // All legacy modes: shunt is INA219_SHUNT_RESISTOR unless provided by `Sensor13 <n> <shunt>`
    // Shunt value provided this way is NOT stored in flash and requires an "on system#boot" rule
  } else {
    // Modes >= 10 allow to provide shunt values that is stored in flash but limited in possible
    // values due to the encoding mode used to store the value in a single uint8_t
    int mult = mode % 10;
    int shunt_milliOhms = mode / 10;
    shunt = shunt_milliOhms / 1000.0;
    for ( ; mult > 0 ; mult-- )
      shunt *= 10.0;
  }
  Ina219Data[index].current_multiplier = 0.001 / shunt;
  #ifdef DEBUG_TASMOTA_SENSOR
  dtostrfd(shunt,6,__ina219_dbg1);
  dtostrfd(Ina219Data[index].current_multiplier,5,__ina219_dbg2);
  DEBUG_SENSOR_LOG("Ina219SetShuntMode[%d]: mode=%d, shunt=%s, cur_mul=%s", index, mode, __ina219_dbg1, __ina219_dbg2);
  #endif
}

float Ina219GetShunt(uint8_t index)
{
  return 0.001 / Ina219Data[index].current_multiplier;
}

/*********************************************************************************************\
 * Return 0 if configuration failed
 * Return 1 if chip identified as INA219
 * Return 2 if chip identified as ISL28022
\*********************************************************************************************/
uint8_t Ina219Init(uint16_t addr)
{
  uint16_t config = 0;

  config = ISL28022_CONFIG_BVOLTAGERANGE_60V        // If INA219 0..32V, If ISL28022 0..60V
         | INA219_CONFIG_GAIN_8_320MV               // Use max scale
         | INA219_CONFIG_BADCRES_12BIT_16S_8510US   // use averaging to improve accuracy
         | INA219_CONFIG_SADCRES_12BIT_16S_8510US   // use averaging to improve accuracy
         | INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
  DEBUG_SENSOR_LOG(PSTR("Ina219Init: Config=0x%04X (%d)"), config, config);
  // Set Config register to take into account the settings above
  if (!I2cWrite16(addr, INA219_REG_CONFIG, config))
    return 0;

  uint16_t intr_reg = 0x0FFFF;
  bool status = I2cValidRead16(&intr_reg, addr, ISL28022_REG_INTRSTATUS);
  DEBUG_SENSOR_LOG(PSTR("Ina219Init: IntrReg=0x%04X (%d)"), intr_reg, status);

  if (status && 0 == intr_reg)
    return ISL28022_MODEL; // ISL28022
  return INA219_MODEL; // INA219
}

float Ina219GetShuntVoltage_mV(uint16_t addr)
{
  // raw shunt voltage (16-bit signed integer, so +-32767)
  int16_t shunt_voltage = I2cReadS16(addr, INA219_REG_SHUNTVOLTAGE);
  DEBUG_SENSOR_LOG("Ina219GetShuntVoltage_mV: ShReg = 0x%04X (%d)",shunt_voltage, shunt_voltage);
  // convert to shunt voltage in mV (so +-327mV) (LSB=10µV=0.01mV)
  return (float)shunt_voltage * INA219_SHUNT_ADC_LSB_MV;
}

float Ina219GetBusVoltage_V(uint16_t addr, uint8_t model)
{
  uint16_t bus_voltage = I2cRead16(addr, INA219_REG_BUSVOLTAGE);
  bus_voltage >>= (ISL28022_MODEL == model) ? 2 : 3;
  DEBUG_SENSOR_LOG("Ina219GetBusVoltage_V: BusReg = 0x%04X (%d)",bus_voltage, bus_voltage);
  // and multiply by LSB raw bus voltage to return bus voltage in volts (LSB=4mV=0.004V)
  return (float)bus_voltage * INA219_BUS_ADC_LSB;
}

bool Ina219Read(void)
{
  for (int i=0 ; i < Ina219Count; i++) {
    uint16_t addr = Ina219Data[i].addr;
    if (!addr) { continue; }
    float bus_voltage_V = Ina219GetBusVoltage_V(addr, Ina219Data[i].model);
    float shunt_voltage_mV = Ina219GetShuntVoltage_mV(addr);
    #ifdef DEBUG_TASMOTA_SENSOR
    dtostrfd(bus_voltage_V,5,__ina219_dbg1);
    dtostrfd(shunt_voltage_mV,5,__ina219_dbg2);
    DEBUG_SENSOR_LOG("Ina219Read[%d]: bV=%sV, sV=%smV", i, __ina219_dbg1, __ina219_dbg2);
    #endif
    // we return the power-supply-side voltage (as bus_voltage register provides the load-side voltage)
    Ina219Data[i].voltage = bus_voltage_V + (shunt_voltage_mV / 1000);
    // current is simply calculted from shunt voltage using pre-calculated multiplier
    Ina219Data[i].current = shunt_voltage_mV * Ina219Data[i].current_multiplier;
    #ifdef DEBUG_TASMOTA_SENSOR
    dtostrfd(Ina219Data[i].voltage,5,__ina219_dbg1);
    dtostrfd(Ina219Data[i].current,5,__ina219_dbg2);
    DEBUG_SENSOR_LOG("Ina219Read[%d]: V=%sV, I=%smA", i, __ina219_dbg1,__ina219_dbg2);
    #endif
  }
  return true;
}

/*********************************************************************************************\
 * Command Sensor13
\*********************************************************************************************/

bool Ina219CommandSensor(void)
{
  char argument[XdrvMailbox.data_len];

  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 255)) {
    Settings->ina219_mode = XdrvMailbox.payload;
    for (int i=0; i < Ina219Count; i++) {
      float shunt = INA219_SHUNT_RESISTOR;
      if (ArgC() > (i +1)) {
        shunt = CharToFloat(ArgV(argument, 2 +i));
      }
      Ina219SetShuntMode(i, Settings->ina219_mode, shunt);
    }
  }
  Response_P(INA219_SENSORCMND_START, XSNS_13, Settings->ina219_mode);
  for (int i = 0 ; i < Ina219Count ; i++ ) {
    dtostrfd(Ina219GetShunt(i),5,argument);
    ResponseAppend_P(PSTR("%s%c"), argument, ((i < (Ina219Count-1))?',':'\0'));
  }
  ResponseAppend_P(INA219_SENSORCMND_END);

  return true;
}

/********************************************************************************************/

void Ina219Detect(void)
{
  for (uint32_t i = 0; i < INA219_MAX_COUNT; i++) {
    uint16_t addr = INA219_ADDRESSES[INA219_FIRST_ADDRESS +i];
    if (!I2cSetDevice(addr)) { continue; }
    if (!Ina219Data) {
      Ina219Data = (struct INA219_Data*)calloc(INA219_MAX_COUNT,sizeof(struct INA219_Data));
      if (!Ina219Data) {
        AddLog(LOG_LEVEL_ERROR,PSTR("INA219: Mem Error"));
        return;
      }
    }
    int model = Ina219Init(addr);
    if (model) {
      I2cSetActiveFound(addr, INA219_TYPE[model-1]);
      Ina219SetShuntMode(Ina219Count, Settings->ina219_mode, INA219_SHUNT_RESISTOR);
      Ina219Data[Ina219Count].model = model;
      Ina219Data[Ina219Count].addr = addr;
      Ina219Count++;
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
  for (int i = 0; i < Ina219Count; i++) {
    if (!Ina219Data[i].model)
      continue;

    char voltage[16];
    dtostrfd(Ina219Data[i].voltage, Settings->flag2.voltage_resolution, voltage);
    char current[16];
    dtostrfd(Ina219Data[i].current, Settings->flag2.current_resolution, current);
    char power[16];
    dtostrfd(Ina219Data[i].voltage * Ina219Data[i].current, Settings->flag2.wattage_resolution, power);
    char name[16];
    if (Ina219Count>1)
      snprintf_P(name, sizeof(name), PSTR("%s%c%d"), INA219_TYPE[Ina219Data[i].model-1], IndexSeparator(), i +1);
    else
      snprintf_P(name, sizeof(name), PSTR("%s"), INA219_TYPE[Ina219Data[i].model-1]);

    if (json) {
      ResponseAppend_P(PSTR(",\"%s\":{\"Id\":%02x,\"" D_JSON_VOLTAGE "\":%s,\"" D_JSON_CURRENT "\":%s,\"" D_JSON_POWERUSAGE "\":%s}"),
                       name, INA219_ADDRESSES[i], voltage, current, power);
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

bool Xsns13(uint32_t function)
{
  if (!I2cEnabled(XI2C_14)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Ina219Detect();
  }
  else if (Ina219Data) {
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
