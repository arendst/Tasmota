/*
  xsns_09_bmp.ino - BMP pressure, temperature, humidity and gas sensor support for Sonoff-Tasmota

  Copyright (C) 2018  Heiko Krupp and Theo Arends

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
#ifdef USE_BMP
/*********************************************************************************************\
 * BMP085, BMP180, BMP280, BME280, BME680 - Pressure, Temperature, Humidity (BME280/BME680) and gas (BME680)
 *
 * Source: Heiko Krupp and Adafruit Industries
 *
 * I2C Address: 0x76 or 0x77
\*********************************************************************************************/

#define BMP_ADDR1            0x77
#define BMP_ADDR2            0x76

#define BMP180_CHIPID        0x55
#define BMP280_CHIPID        0x58
#define BME280_CHIPID        0x60
#define BME680_CHIPID        0x61

#define BMP_REGISTER_CHIPID  0xD0

const char kBmpTypes[] PROGMEM = "BMP180|BMP280|BME280|BME680";

uint8_t bmp_address;
uint8_t bmp_addresses[] = { BMP_ADDR1, BMP_ADDR2 };
uint8_t bmp_type = 0;
uint8_t bmp_model = 0;
char bmp_name[7];

/*********************************************************************************************\
 * BMP085 and BME180
\*********************************************************************************************/

#define BMP180_REG_CONTROL   0xF4
#define BMP180_REG_RESULT    0xF6
#define BMP180_TEMPERATURE   0x2E
#define BMP180_PRESSURE3     0xF4 // Max. oversampling -> OSS = 3

#define BMP180_AC1           0xAA
#define BMP180_AC2           0xAC
#define BMP180_AC3           0xAE
#define BMP180_AC4           0xB0
#define BMP180_AC5           0xB2
#define BMP180_AC6           0xB4
#define BMP180_VB1           0xB6
#define BMP180_VB2           0xB8
#define BMP180_MB            0xBA
#define BMP180_MC            0xBC
#define BMP180_MD            0xBE

#define BMP180_OSS           3

int16_t  cal_ac1;
int16_t  cal_ac2;
int16_t  cal_ac3;
int16_t  cal_b1;
int16_t  cal_b2;
int16_t  cal_mc;
int16_t  cal_md;
uint16_t cal_ac4;
uint16_t cal_ac5;
uint16_t cal_ac6;
int32_t  bmp180_b5 = 0;

boolean Bmp180Calibration()
{
  cal_ac1 = I2cRead16(bmp_address, BMP180_AC1);
  cal_ac2 = I2cRead16(bmp_address, BMP180_AC2);
  cal_ac3 = I2cRead16(bmp_address, BMP180_AC3);
  cal_ac4 = I2cRead16(bmp_address, BMP180_AC4);
  cal_ac5 = I2cRead16(bmp_address, BMP180_AC5);
  cal_ac6 = I2cRead16(bmp_address, BMP180_AC6);
  cal_b1  = I2cRead16(bmp_address, BMP180_VB1);
  cal_b2  = I2cRead16(bmp_address, BMP180_VB2);
  cal_mc  = I2cRead16(bmp_address, BMP180_MC);
  cal_md  = I2cRead16(bmp_address, BMP180_MD);

  // Check for Errors in calibration data. Value never is 0x0000 or 0xFFFF
  if (!cal_ac1 | !cal_ac2 | !cal_ac3 | !cal_ac4 | !cal_ac5 | !cal_ac6 | !cal_b1 | !cal_b2 | !cal_mc | !cal_md) {
    return false;
  }

  if ((cal_ac1 == (int16_t)0xFFFF) |
      (cal_ac2 == (int16_t)0xFFFF) |
      (cal_ac3 == (int16_t)0xFFFF) |
      (cal_ac4 == 0xFFFF) |
      (cal_ac5 == 0xFFFF) |
      (cal_ac6 == 0xFFFF) |
      (cal_b1 == (int16_t)0xFFFF) |
      (cal_b2 == (int16_t)0xFFFF) |
      (cal_mc == (int16_t)0xFFFF) |
      (cal_md == (int16_t)0xFFFF)) {
    return false;
  }
  return true;
}

double Bmp180ReadTemperature()
{
  I2cWrite8(bmp_address, BMP180_REG_CONTROL, BMP180_TEMPERATURE);
  delay(5); // 5ms conversion time
  int ut = I2cRead16(bmp_address, BMP180_REG_RESULT);
  int32_t x1 = (ut - (int32_t)cal_ac6) * ((int32_t)cal_ac5) >> 15;
  int32_t x2 = ((int32_t)cal_mc << 11) / (x1 + (int32_t)cal_md);
  bmp180_b5 = x1 + x2;

  return ((bmp180_b5 + 8) >> 4) / 10.0;
}

double Bmp180ReadPressure()
{
  int32_t p;

  I2cWrite8(bmp_address, BMP180_REG_CONTROL, BMP180_PRESSURE3); // Highest resolution
  delay(2 + (4 << BMP180_OSS));                                 // 26ms conversion time at ultra high resolution
  uint32_t up = I2cRead24(bmp_address, BMP180_REG_RESULT);
  up >>= (8 - BMP180_OSS);

  int32_t b6 = bmp180_b5 - 4000;
  int32_t x1 = ((int32_t)cal_b2 * ((b6 * b6) >> 12)) >> 11;
  int32_t x2 = ((int32_t)cal_ac2 * b6) >> 11;
  int32_t x3 = x1 + x2;
  int32_t b3 = ((((int32_t)cal_ac1 * 4 + x3) << BMP180_OSS) + 2) >> 2;

  x1 = ((int32_t)cal_ac3 * b6) >> 13;
  x2 = ((int32_t)cal_b1 * ((b6 * b6) >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;
  uint32_t b4 = ((uint32_t)cal_ac4 * (uint32_t)(x3 + 32768)) >> 15;
  uint32_t b7 = ((uint32_t)up - b3) * (uint32_t)(50000UL >> BMP180_OSS);

  if (b7 < 0x80000000) {
    p = (b7 * 2) / b4;
  }
  else {
    p = (b7 / b4) * 2;
  }

  x1 = (p >> 8) * (p >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * p) >> 16;

  p += ((x1 + x2 + (int32_t)3791) >> 4);
  return p / 100.0;  // convert to mbar
}

/*********************************************************************************************\
 * BMP280 and BME280
 *
 * Programmer : BMP280/BME280 Datasheet and Adafruit with changes by Theo Arends
\*********************************************************************************************/

#define BME280_REGISTER_CONTROLHUMID  0xF2
#define BME280_REGISTER_CONTROL       0xF4
#define BME280_REGISTER_CONFIG        0xF5
#define BME280_REGISTER_PRESSUREDATA  0xF7
#define BME280_REGISTER_TEMPDATA      0xFA
#define BME280_REGISTER_HUMIDDATA     0xFD

#define BME280_REGISTER_DIG_T1        0x88
#define BME280_REGISTER_DIG_T2        0x8A
#define BME280_REGISTER_DIG_T3        0x8C
#define BME280_REGISTER_DIG_P1        0x8E
#define BME280_REGISTER_DIG_P2        0x90
#define BME280_REGISTER_DIG_P3        0x92
#define BME280_REGISTER_DIG_P4        0x94
#define BME280_REGISTER_DIG_P5        0x96
#define BME280_REGISTER_DIG_P6        0x98
#define BME280_REGISTER_DIG_P7        0x9A
#define BME280_REGISTER_DIG_P8        0x9C
#define BME280_REGISTER_DIG_P9        0x9E
#define BME280_REGISTER_DIG_H1        0xA1
#define BME280_REGISTER_DIG_H2        0xE1
#define BME280_REGISTER_DIG_H3        0xE3
#define BME280_REGISTER_DIG_H4        0xE4
#define BME280_REGISTER_DIG_H5        0xE5
#define BME280_REGISTER_DIG_H6        0xE7

struct BME280CALIBDATA
{
  uint16_t dig_T1;
  int16_t  dig_T2;
  int16_t  dig_T3;
  uint16_t dig_P1;
  int16_t  dig_P2;
  int16_t  dig_P3;
  int16_t  dig_P4;
  int16_t  dig_P5;
  int16_t  dig_P6;
  int16_t  dig_P7;
  int16_t  dig_P8;
  int16_t  dig_P9;
  uint8_t  dig_H1;
  int16_t  dig_H2;
  uint8_t  dig_H3;
  int16_t  dig_H4;
  int16_t  dig_H5;
  int8_t   dig_H6;
} Bme280CalibrationData;

int32_t t_fine;

boolean Bmx280Calibrate()
{
  //  if (I2cRead8(bmp_address, BMP_REGISTER_CHIPID) != BME280_CHIPID) return false;

  Bme280CalibrationData.dig_T1 = I2cRead16LE(bmp_address, BME280_REGISTER_DIG_T1);
  Bme280CalibrationData.dig_T2 = I2cReadS16_LE(bmp_address, BME280_REGISTER_DIG_T2);
  Bme280CalibrationData.dig_T3 = I2cReadS16_LE(bmp_address, BME280_REGISTER_DIG_T3);
  Bme280CalibrationData.dig_P1 = I2cRead16LE(bmp_address, BME280_REGISTER_DIG_P1);
  Bme280CalibrationData.dig_P2 = I2cReadS16_LE(bmp_address, BME280_REGISTER_DIG_P2);
  Bme280CalibrationData.dig_P3 = I2cReadS16_LE(bmp_address, BME280_REGISTER_DIG_P3);
  Bme280CalibrationData.dig_P4 = I2cReadS16_LE(bmp_address, BME280_REGISTER_DIG_P4);
  Bme280CalibrationData.dig_P5 = I2cReadS16_LE(bmp_address, BME280_REGISTER_DIG_P5);
  Bme280CalibrationData.dig_P6 = I2cReadS16_LE(bmp_address, BME280_REGISTER_DIG_P6);
  Bme280CalibrationData.dig_P7 = I2cReadS16_LE(bmp_address, BME280_REGISTER_DIG_P7);
  Bme280CalibrationData.dig_P8 = I2cReadS16_LE(bmp_address, BME280_REGISTER_DIG_P8);
  Bme280CalibrationData.dig_P9 = I2cReadS16_LE(bmp_address, BME280_REGISTER_DIG_P9);
/*
  if (BME280_CHIPID == bmp_type) {
    Bme280CalibrationData.dig_H1 = I2cRead8(bmp_address, BME280_REGISTER_DIG_H1);
    Bme280CalibrationData.dig_H2 = I2cReadS16_LE(bmp_address, BME280_REGISTER_DIG_H2);
    Bme280CalibrationData.dig_H3 = I2cRead8(bmp_address, BME280_REGISTER_DIG_H3);
    Bme280CalibrationData.dig_H4 = (I2cRead8(bmp_address, BME280_REGISTER_DIG_H4) << 4) | (I2cRead8(bmp_address, BME280_REGISTER_DIG_H4 + 1) & 0xF);
    Bme280CalibrationData.dig_H5 = (I2cRead8(bmp_address, BME280_REGISTER_DIG_H5 + 1) << 4) | (I2cRead8(bmp_address, BME280_REGISTER_DIG_H5) >> 4);
    Bme280CalibrationData.dig_H6 = (int8_t)I2cRead8(bmp_address, BME280_REGISTER_DIG_H6);

    // Set before CONTROL_meas (DS 5.4.3)
    I2cWrite8(bmp_address, BME280_REGISTER_CONTROLHUMID, 0x05); // 16x oversampling (Adafruit)
  }
  I2cWrite8(bmp_address, BME280_REGISTER_CONTROL, 0xB7);      // 16x oversampling, normal mode (Adafruit)
*/
  if (BME280_CHIPID == bmp_type) {  // #1051
    Bme280CalibrationData.dig_H1 = I2cRead8(bmp_address, BME280_REGISTER_DIG_H1);
    Bme280CalibrationData.dig_H2 = I2cReadS16_LE(bmp_address, BME280_REGISTER_DIG_H2);
    Bme280CalibrationData.dig_H3 = I2cRead8(bmp_address, BME280_REGISTER_DIG_H3);
    Bme280CalibrationData.dig_H4 = (I2cRead8(bmp_address, BME280_REGISTER_DIG_H4) << 4) | (I2cRead8(bmp_address, BME280_REGISTER_DIG_H4 + 1) & 0xF);
    Bme280CalibrationData.dig_H5 = (I2cRead8(bmp_address, BME280_REGISTER_DIG_H5 + 1) << 4) | (I2cRead8(bmp_address, BME280_REGISTER_DIG_H5) >> 4);
    Bme280CalibrationData.dig_H6 = (int8_t)I2cRead8(bmp_address, BME280_REGISTER_DIG_H6);

    I2cWrite8(bmp_address, BME280_REGISTER_CONTROL, 0x00);      // sleep mode since writes to config can be ignored in normal mode (Datasheet 5.4.5/6 page 27)
    // Set before CONTROL_meas (DS 5.4.3)
    I2cWrite8(bmp_address, BME280_REGISTER_CONTROLHUMID, 0x01); // 1x oversampling
    I2cWrite8(bmp_address, BME280_REGISTER_CONFIG, 0xA0);       // 1sec standby between measurements (to limit self heating), IIR filter off
    I2cWrite8(bmp_address, BME280_REGISTER_CONTROL, 0x27);      // 1x oversampling, normal mode
  } else {
    I2cWrite8(bmp_address, BME280_REGISTER_CONTROL, 0xB7);      // 16x oversampling, normal mode (Adafruit)
  }

  return true;
}

double Bme280ReadTemperature(void)
{
  int32_t var1;
  int32_t var2;

  int32_t adc_T = I2cRead24(bmp_address, BME280_REGISTER_TEMPDATA);
  adc_T >>= 4;

  var1 = ((((adc_T >> 3) - ((int32_t)Bme280CalibrationData.dig_T1 << 1))) * ((int32_t)Bme280CalibrationData.dig_T2)) >> 11;
  var2 = (((((adc_T >> 4) - ((int32_t)Bme280CalibrationData.dig_T1)) * ((adc_T >> 4) - ((int32_t)Bme280CalibrationData.dig_T1))) >> 12) *
    ((int32_t)Bme280CalibrationData.dig_T3)) >> 14;
  t_fine = var1 + var2;
  double T = (t_fine * 5 + 128) >> 8;
  return T / 100.0;
}

double Bme280ReadPressure(void)
{
  int64_t var1;
  int64_t var2;
  int64_t p;

  // Must be done first to get the t_fine variable set up
  //  Bme280ReadTemperature();

  int32_t adc_P = I2cRead24(bmp_address, BME280_REGISTER_PRESSUREDATA);
  adc_P >>= 4;

  var1 = ((int64_t)t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)Bme280CalibrationData.dig_P6;
  var2 = var2 + ((var1 * (int64_t)Bme280CalibrationData.dig_P5) << 17);
  var2 = var2 + (((int64_t)Bme280CalibrationData.dig_P4) << 35);
  var1 = ((var1 * var1 * (int64_t)Bme280CalibrationData.dig_P3) >> 8) + ((var1 * (int64_t)Bme280CalibrationData.dig_P2) << 12);
  var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)Bme280CalibrationData.dig_P1) >> 33;
  if (0 == var1) {
    return 0; // avoid exception caused by division by zero
  }
  p = 1048576 - adc_P;
  p = (((p << 31) - var2) * 3125) / var1;
  var1 = (((int64_t)Bme280CalibrationData.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
  var2 = (((int64_t)Bme280CalibrationData.dig_P8) * p) >> 19;
  p = ((p + var1 + var2) >> 8) + (((int64_t)Bme280CalibrationData.dig_P7) << 4);
  return (double)p / 25600.0;
}

double Bme280ReadHumidity(void)
{
  int32_t v_x1_u32r;

  // Must be done first to get the t_fine variable set up
  //  Bme280ReadTemperature();

  int32_t adc_H = I2cRead16(bmp_address, BME280_REGISTER_HUMIDDATA);

  v_x1_u32r = (t_fine - ((int32_t)76800));

  v_x1_u32r = (((((adc_H << 14) - (((int32_t)Bme280CalibrationData.dig_H4) << 20) -
    (((int32_t)Bme280CalibrationData.dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *
    (((((((v_x1_u32r * ((int32_t)Bme280CalibrationData.dig_H6)) >> 10) *
    (((v_x1_u32r * ((int32_t)Bme280CalibrationData.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) +
    ((int32_t)2097152)) * ((int32_t)Bme280CalibrationData.dig_H2) + 8192) >> 14));
  v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
    ((int32_t)Bme280CalibrationData.dig_H1)) >> 4));
  v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
  v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
  double h = (v_x1_u32r >> 12);
  return h / 1024.0;
}

#ifdef USE_BME680
/*********************************************************************************************\
 * BME680
\*********************************************************************************************/

#include <Adafruit_BME680.h>
Adafruit_BME680 bme680;

void Bme680PerformReading()
{
  if (BME680_CHIPID == bmp_type) {
    bme680.performReading();
  }
}

#endif  // USE_BME680

/********************************************************************************************/

void BmpDetect()
{
  if (bmp_type) {
    return;
  }

  for (byte i = 0; i < sizeof(bmp_addresses); i++) {
    bmp_address = bmp_addresses[i];
    bmp_type = I2cRead8(bmp_address, BMP_REGISTER_CHIPID);
    if (bmp_type) {
      break;
    }
  }
  if (bmp_type) {
    boolean success = false;
    switch (bmp_type) {
      case BMP180_CHIPID:
        success = Bmp180Calibration();
        break;
      case BMP280_CHIPID:
        bmp_model = 1;  // 1
        success = Bmx280Calibrate();
        break;
      case BME280_CHIPID:
        bmp_model = 2;  // 2
        success = Bmx280Calibrate();
        break;
#ifdef USE_BME680
      case BME680_CHIPID:
        bmp_model = 3;  // 2
        success = bme680.begin(bmp_address);
        break;
#endif  // USE_BME680
    }
    if (success) {
      GetTextIndexed(bmp_name, sizeof(bmp_name), bmp_model, kBmpTypes);
      snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, bmp_name, bmp_address);
      AddLog(LOG_LEVEL_DEBUG);
    }
    else {
      bmp_type = 0;
    }
  }
}

void BmpShow(boolean json)
{
  if (bmp_type) {
    float t = 0.0;
    float p = 0.0;
    float h = 0.0;
    float g = 0.0;
    float bmp_sealevel = 0.0;

    switch (bmp_type) {
      case BMP180_CHIPID:
        t = Bmp180ReadTemperature();
        p = Bmp180ReadPressure();
        break;
      case BME280_CHIPID:
        h = Bme280ReadHumidity();
      case BMP280_CHIPID:
        t = Bme280ReadTemperature();
        p = Bme280ReadPressure();
        break;
#ifdef USE_BME680
      case BME680_CHIPID:
        t = bme680.temperature;
        p = bme680.pressure / 100.0;
        h = bme680.humidity;
        g = bme680.gas_resistance / 1000.0;
        break;
#endif  // USE_BME680
    }
    if (t != 0.0) {
      t = ConvertTemp(t);
    }
    if (p != 0.0) {
//    bmp_sealevel = p / pow(1.0 - ((float)Settings.altitude / 44330.0), 5.255);  // pow adds 8k to the code
      bmp_sealevel = (p / FastPrecisePow(1.0 - ((float)Settings.altitude / 44330.0), 5.255)) - 21.6;
    }

    char temperature[10];
    dtostrfd(t, Settings.flag2.temperature_resolution, temperature);
    char pressure[10];
    dtostrfd(p, Settings.flag2.pressure_resolution, pressure);
    char sea_pressure[10];
    dtostrfd(bmp_sealevel, Settings.flag2.pressure_resolution, sea_pressure);
    char humidity[10];
    dtostrfd(h, Settings.flag2.humidity_resolution, humidity);
#ifdef USE_BME680
    char gas_resistance[10];
    dtostrfd(g, 2, gas_resistance);
#endif  // USE_BME680

    if (json) {
      char json_humidity[40];
      snprintf_P(json_humidity, sizeof(json_humidity), PSTR(",\"" D_JSON_HUMIDITY "\":%s"), humidity);
      char json_sealevel[40];
      snprintf_P(json_sealevel, sizeof(json_sealevel), PSTR(",\"" D_JSON_PRESSUREATSEALEVEL "\":%s"), sea_pressure);
#ifdef USE_BME680
      char json_gas[40];
      snprintf_P(json_gas, sizeof(json_gas), PSTR(",\"" D_JSON_GAS "\":%s"), gas_resistance);
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"%s\":{\"" D_JSON_TEMPERATURE "\":%s%s,\"" D_JSON_PRESSURE "\":%s%s%s}"),
        mqtt_data, bmp_name, temperature, (bmp_model >= 2) ? json_humidity : "", pressure, (Settings.altitude != 0) ? json_sealevel : "", (bmp_model >= 3) ? json_gas : "");
#else
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"%s\":{\"" D_JSON_TEMPERATURE "\":%s%s,\"" D_JSON_PRESSURE "\":%s%s}"),
        mqtt_data, bmp_name, temperature, (bmp_model >= 2) ? json_humidity : "", pressure, (Settings.altitude != 0) ? json_sealevel : "");
#endif  // USE_BME680
#ifdef USE_DOMOTICZ
      DomoticzTempHumPressureSensor(temperature, humidity, pressure);
#endif // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TEMP, mqtt_data, bmp_name, temperature, TempUnit());
      if (bmp_model >= 2) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_HUM, mqtt_data, bmp_name, humidity);
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_PRESSURE, mqtt_data, bmp_name, pressure);
      if (Settings.altitude != 0) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_SEAPRESSURE, mqtt_data, bmp_name, sea_pressure);
      }
#ifdef USE_BME680
      if (bmp_model >= 3) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s{s}%s " D_GAS "{m}%s " D_UNIT_KILOOHM "{e}"), mqtt_data, bmp_name, gas_resistance);
      }
#endif  // USE_BME680
#endif // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_09

boolean Xsns09(byte function)
{
  boolean result = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_PREP_BEFORE_TELEPERIOD:
        BmpDetect();
        break;
      case FUNC_EVERY_SECOND:
#ifdef USE_BME680
        if ((Settings.tele_period - tele_period) < 300) {  // 5 minute stabilization time
          if (tele_period &1) {
            Bme680PerformReading();  // Keep BME680 busy every two seconds
          }
        }
#endif  // USE_BME680
        break;
      case FUNC_JSON_APPEND:
        BmpShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        BmpShow(0);
#ifdef USE_BME680
        Bme680PerformReading();
#endif  // USE_BME680
        break;
#endif // USE_WEBSERVER
    }
  }
  return result;
}

#endif // USE_BMP
#endif // USE_I2C
