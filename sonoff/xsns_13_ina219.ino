/*
  xsns_13_ina219.ino - INA219 Current Sensor support for Sonoff-Tasmota

  Copyright (C) 2018  Stefan Bode and Theo Arends

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
#define INA219_CONFIG_BADCRES_9BIT              (0x0080)  // 9-bit bus res = 0..511
#define INA219_CONFIG_BADCRES_10BIT             (0x0100)  // 10-bit bus res = 0..1023
#define INA219_CONFIG_BADCRES_11BIT             (0x0200)  // 11-bit bus res = 0..2047
#define INA219_CONFIG_BADCRES_12BIT             (0x0400)  // 12-bit bus res = 0..4097

#define INA219_CONFIG_SADCRES_MASK              (0x0078)  // Shunt ADC Resolution and Averaging Mask
#define INA219_CONFIG_SADCRES_9BIT_1S_84US      (0x0000)  // 1 x 9-bit shunt sample
#define INA219_CONFIG_SADCRES_10BIT_1S_148US    (0x0008)  // 1 x 10-bit shunt sample
#define INA219_CONFIG_SADCRES_11BIT_1S_276US    (0x0010)  // 1 x 11-bit shunt sample
#define INA219_CONFIG_SADCRES_12BIT_1S_532US    (0x0018)  // 1 x 12-bit shunt sample
#define INA219_CONFIG_SADCRES_12BIT_2S_1060US   (0x0048)  // 2 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_4S_2130US   (0x0050)  // 4 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_8S_4260US   (0x0058)  // 8 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_16S_8510US  (0x0060)  // 16 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_32S_17MS    (0x0068)  // 32 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_64S_34MS    (0x0070)  // 64 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_128S_69MS   (0x0078)  // 128 x 12-bit shunt samples averaged together

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

uint8_t ina219_type = 0;
uint8_t ina219_address;
uint8_t ina219_addresses[] = { INA219_ADDRESS1, INA219_ADDRESS2, INA219_ADDRESS3, INA219_ADDRESS4 };

uint32_t ina219_cal_value = 0;
// The following multiplier is used to convert raw current values to mA, taking into account the current config settings
uint32_t ina219_current_divider_ma = 0;

uint8_t ina219_valid = 0;
float ina219_voltage = 0;
float ina219_current = 0;
char ina219_types[] = "INA219";

bool Ina219SetCalibration(uint8_t mode)
{
  uint16_t config = 0;

  switch (mode &3) {
    case 0:  // 32V 2A
    case 3:
      ina219_cal_value = 4096;
      ina219_current_divider_ma = 10;  // Current LSB = 100uA per bit (1000/100 = 10)
      config = INA219_CONFIG_BVOLTAGERANGE_32V | INA219_CONFIG_GAIN_8_320MV | INA219_CONFIG_BADCRES_12BIT | INA219_CONFIG_SADCRES_12BIT_1S_532US | INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
      break;
    case 1:  // 32V 1A
      ina219_cal_value = 10240;
      ina219_current_divider_ma = 25;  // Current LSB = 40uA per bit (1000/40 = 25)
      config |= INA219_CONFIG_BVOLTAGERANGE_32V | INA219_CONFIG_GAIN_8_320MV | INA219_CONFIG_BADCRES_12BIT | INA219_CONFIG_SADCRES_12BIT_1S_532US | INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
      break;
    case 2:  // 16V 0.4A
      ina219_cal_value = 8192;
      ina219_current_divider_ma = 20;  // Current LSB = 50uA per bit (1000/50 = 20)
      config |= INA219_CONFIG_BVOLTAGERANGE_16V | INA219_CONFIG_GAIN_1_40MV | INA219_CONFIG_BADCRES_12BIT | INA219_CONFIG_SADCRES_12BIT_1S_532US | INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
      break;
  }
  // Set Calibration register to 'Cal' calculated above
  bool success = I2cWrite16(ina219_address, INA219_REG_CALIBRATION, ina219_cal_value);
  if (success) {
    // Set Config register to take into account the settings above
    I2cWrite16(ina219_address, INA219_REG_CONFIG, config);
  }
  return success;
}

float Ina219GetShuntVoltage_mV(void)
{
  // raw shunt voltage (16-bit signed integer, so +-32767)
  int16_t value = I2cReadS16(ina219_address, INA219_REG_SHUNTVOLTAGE);
  // shunt voltage in mV (so +-327mV)
  return value * 0.01;
}

float Ina219GetBusVoltage_V(void)
{
  // Shift to the right 3 to drop CNVR and OVF and multiply by LSB
  // raw bus voltage (16-bit signed integer, so +-32767)
  int16_t value = (int16_t)(((uint16_t)I2cReadS16(ina219_address, INA219_REG_BUSVOLTAGE) >> 3) * 4);
  // bus voltage in volts
  return value * 0.001;
}

float Ina219GetCurrent_mA(void)
{
  // Sometimes a sharp load will reset the INA219, which will reset the cal register,
  // meaning CURRENT and POWER will not be available ... avoid this by always setting
  // a cal value even if it's an unfortunate extra step
  I2cWrite16(ina219_address, INA219_REG_CALIBRATION, ina219_cal_value);
  // Now we can safely read the CURRENT register!
  // raw current value (16-bit signed integer, so +-32767)
  float value = I2cReadS16(ina219_address, INA219_REG_CURRENT);
  value /= ina219_current_divider_ma;
  // current value in mA, taking into account the config settings and current LSB
  return value;
}

bool Ina219Read(void)
{
  ina219_voltage = Ina219GetBusVoltage_V() + (Ina219GetShuntVoltage_mV() / 1000);
  ina219_current = Ina219GetCurrent_mA() / 1000;
  ina219_valid = SENSOR_MAX_MISS;
  return true;
}

/*********************************************************************************************\
 * Command Sensor13
 *
 * 0 - Max 32V 2A range
 * 1 - Max 32V 1A range
 * 2 - Max 16V 0.4A range
\*********************************************************************************************/

bool Ina219CommandSensor(void)
{
  boolean serviced = true;

  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) {
    Settings.ina219_mode = XdrvMailbox.payload;
    restart_flag = 2;
  }
  snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_SENSOR_INDEX_NVALUE, XSNS_13, Settings.ina219_mode);

  return serviced;
}

/********************************************************************************************/

void Ina219Detect(void)
{
  if (ina219_type) { return; }

  for (byte i = 0; i < sizeof(ina219_addresses); i++) {
    ina219_address = ina219_addresses[i];
    if (Ina219SetCalibration(Settings.ina219_mode)) {
      ina219_type = 1;
      snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, ina219_types, ina219_address);
      AddLog(LOG_LEVEL_DEBUG);
      break;
    }
  }
}

void Ina219EverySecond(void)
{
  if (87 == (uptime %100)) {
    // 2mS
    Ina219Detect();
  }
  else {
    // 3mS
    if (ina219_type) {
      if (!Ina219Read()) {
        AddLogMissed(ina219_types, ina219_valid);
//        if (!ina219_valid) { ina219_type = 0; }
      }
    }
  }
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_INA219_DATA[] PROGMEM = "%s"
  "{s}INA219 " D_VOLTAGE "{m}%s " D_UNIT_VOLT "{e}"
  "{s}INA219 " D_CURRENT "{m}%s " D_UNIT_AMPERE "{e}"
  "{s}INA219 " D_POWERUSAGE "{m}%s " D_UNIT_WATT "{e}";
#endif  // USE_WEBSERVER

void Ina219Show(boolean json)
{
  if (ina219_valid) {
    char voltage[10];
    char current[10];
    char power[10];

    float fpower = ina219_voltage * ina219_current;
    dtostrfd(ina219_voltage, Settings.flag2.voltage_resolution, voltage);
    dtostrfd(fpower, Settings.flag2.wattage_resolution, power);
    dtostrfd(ina219_current, Settings.flag2.current_resolution, current);

    if (json) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"%s\":{\"" D_JSON_VOLTAGE "\":%s,\"" D_JSON_CURRENT "\":%s,\"" D_JSON_POWERUSAGE "\":%s}"),
        mqtt_data, ina219_types, voltage, current, power);
#ifdef USE_DOMOTICZ
      if (0 == tele_period) {
        DomoticzSensor(DZ_VOLTAGE, voltage);
        DomoticzSensor(DZ_CURRENT, current);
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_INA219_DATA, mqtt_data, voltage, current, power);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xsns13(byte function)
{
  boolean result = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_COMMAND:
        if ((XSNS_13 == XdrvMailbox.index) && (ina219_type)) {
          result = Ina219CommandSensor();
        }
        break;
      case FUNC_INIT:
        Ina219Detect();
        break;
      case FUNC_EVERY_SECOND:
        Ina219EverySecond();
        break;
      case FUNC_JSON_APPEND:
        Ina219Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        Ina219Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_INA219
#endif  // USE_I2C
