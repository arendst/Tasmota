/*
  xsns_09_bmp.ino - BMP pressure, temperature, humidity and gas sensor support for Tasmota

  Copyright (C) 2021  Heiko Krupp and Theo Arends

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

#define XSNS_09              9
#define XI2C_10              10  // See I2CDEVICES.md

#ifdef USE_BME680
#define USE_BME68X
#endif

#define BMP_ADDR1            0x76
#define BMP_ADDR2            0x77

#define BMP180_CHIPID        0x55
#define BMP280_CHIPID        0x58
#define BME280_CHIPID        0x60
#define BME680_CHIPID        0x61

#define BMP_REGISTER_CHIPID  0xD0

#define BMP_REGISTER_RESET   0xE0  // Register to reset to power on defaults (used for sleep)

#define BMP_CMND_RESET       0xB6  // I2C Parameter for RESET to put BMP into reset state
#define BMP_FAIL_LIMIT       5     // Failed BMx280 reads (seconds) before values are reported as null

#define BMP_MAX_SENSORS      2 * MAX_I2C  // Busses

#define BME680_IAQ_BURNIN        2880
#define BME680_IAQ_HILL_K        3.0f
#define BME680_IAQ_TVOC_MAX      600.0f
#define BME680_IAQ_TVOC_HALF     0.5f
#define BME680_IAQ_RISE_ALPHA    0.005f
#define BME680_IAQ_AGING_ALPHA   0.00001f
#define BME680_IAQ_R_MIN         1.0f
#define BME680_IAQ_R_MAX         5000.0f
#define BME680_IAQ_BASE_MAX      20000.0f
#define BME680_IAQ_PH_SLOPE      0.03f

typedef struct {
  float    baseline;
  float    sum;
  uint16_t count;
  float    iaq;
  float    tvoc;
} bme68x_iaq_t;

const char kBmpTypes[] PROGMEM = "BMP180|BMP280|BME280|BME680";

#define D_PRFX_BMP_IAQ       "BmpIaq"
const char kBmpIaqNames[] PROGMEM = D_PRFX_BMP_IAQ;
const char kBmpCommands[] PROGMEM = D_PRFX_BMP_IAQ "|";
void (* const BmpCommand[])(void) PROGMEM = { &Bme680IaqCommand };

typedef struct {
  uint8_t bmp_address;    // I2C address
  uint8_t bmp_bus;        // I2C bus
  char bmp_name[7];       // Sensor name - "BMPXXX"
  uint8_t bmp_type;
  uint8_t bmp_model;
  uint8_t bmp_fail;       // Consecutive failed BMx280 reads
#ifdef USE_BME68X
  uint8_t bme680_state;
  float bmp_gas_resistance;
  bme68x_iaq_t bme680_iaq;
#endif  // USE_BME68X
  float bmp_temperature;
  float bmp_pressure;
  float bmp_humidity;
} bmp_sensors_t;

uint8_t bmp_addresses[] = { BMP_ADDR1, BMP_ADDR2 };
uint8_t bmp_count = 0;
#ifdef USE_DEEPSLEEP
uint8_t bmp_deepsleep = 0;  // Prevent updating measurments once BMP has been put to sleep (just before ESP enters deepsleep)
#endif

bmp_sensors_t *bmp_sensors = nullptr;

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

typedef struct {
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
} bmp180_cal_data_t;

bmp180_cal_data_t *bmp180_cal_data = nullptr;

bool Bmp180Calibration(uint8_t bmp_idx) {
  if (!bmp180_cal_data) {
    bmp180_cal_data = (bmp180_cal_data_t*)malloc(BMP_MAX_SENSORS * sizeof(bmp180_cal_data_t));
  }
  if (!bmp180_cal_data) { return false; }

  uint8_t address = bmp_sensors[bmp_idx].bmp_address;
  uint8_t bus = bmp_sensors[bmp_idx].bmp_bus;
  bmp180_cal_data[bmp_idx].cal_ac1 = I2cRead16(address, BMP180_AC1, bus);
  bmp180_cal_data[bmp_idx].cal_ac2 = I2cRead16(address, BMP180_AC2, bus);
  bmp180_cal_data[bmp_idx].cal_ac3 = I2cRead16(address, BMP180_AC3, bus);
  bmp180_cal_data[bmp_idx].cal_ac4 = I2cRead16(address, BMP180_AC4, bus);
  bmp180_cal_data[bmp_idx].cal_ac5 = I2cRead16(address, BMP180_AC5, bus);
  bmp180_cal_data[bmp_idx].cal_ac6 = I2cRead16(address, BMP180_AC6, bus);
  bmp180_cal_data[bmp_idx].cal_b1  = I2cRead16(address, BMP180_VB1, bus);
  bmp180_cal_data[bmp_idx].cal_b2  = I2cRead16(address, BMP180_VB2, bus);
  bmp180_cal_data[bmp_idx].cal_mc  = I2cRead16(address, BMP180_MC, bus);
  bmp180_cal_data[bmp_idx].cal_md  = I2cRead16(address, BMP180_MD, bus);
  // Check for Errors in calibration data. Value never is 0x0000 or 0xFFFF
  if (!bmp180_cal_data[bmp_idx].cal_ac1 |
      !bmp180_cal_data[bmp_idx].cal_ac2 |
      !bmp180_cal_data[bmp_idx].cal_ac3 |
      !bmp180_cal_data[bmp_idx].cal_ac4 |
      !bmp180_cal_data[bmp_idx].cal_ac5 |
      !bmp180_cal_data[bmp_idx].cal_ac6 |
      !bmp180_cal_data[bmp_idx].cal_b1 |
      !bmp180_cal_data[bmp_idx].cal_b2 |
      !bmp180_cal_data[bmp_idx].cal_mc |
      !bmp180_cal_data[bmp_idx].cal_md) {
    return false;
  }

  if ((bmp180_cal_data[bmp_idx].cal_ac1 == (int16_t)0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_ac2 == (int16_t)0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_ac3 == (int16_t)0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_ac4 == 0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_ac5 == 0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_ac6 == 0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_b1 == (int16_t)0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_b2 == (int16_t)0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_mc == (int16_t)0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_md == (int16_t)0xFFFF)) {
    return false;
  }
  return true;
}

void Bmp180Read(uint8_t bmp_idx) {
  if (!bmp180_cal_data) { return; }

  uint8_t address = bmp_sensors[bmp_idx].bmp_address;
  uint8_t bus = bmp_sensors[bmp_idx].bmp_bus;
  I2cWrite8(address, BMP180_REG_CONTROL, BMP180_TEMPERATURE, bus);
  delay(5); // 5ms conversion time
  int ut = I2cRead16(address, BMP180_REG_RESULT, bus);
  int32_t xt1 = (ut - (int32_t)bmp180_cal_data[bmp_idx].cal_ac6) * ((int32_t)bmp180_cal_data[bmp_idx].cal_ac5) >> 15;
  int32_t xt2 = ((int32_t)bmp180_cal_data[bmp_idx].cal_mc << 11) / (xt1 + (int32_t)bmp180_cal_data[bmp_idx].cal_md);
  int32_t bmp180_b5 = xt1 + xt2;
  bmp_sensors[bmp_idx].bmp_temperature = ((bmp180_b5 + 8) >> 4) / 10.0f;

  I2cWrite8(address, BMP180_REG_CONTROL, BMP180_PRESSURE3, bus); // Highest resolution
  delay(2 + (4 << BMP180_OSS));                                 // 26ms conversion time at ultra high resolution
  uint32_t up = I2cRead24(address, BMP180_REG_RESULT, bus);
  up >>= (8 - BMP180_OSS);

  int32_t b6 = bmp180_b5 - 4000;
  int32_t x1 = ((int32_t)bmp180_cal_data[bmp_idx].cal_b2 * ((b6 * b6) >> 12)) >> 11;
  int32_t x2 = ((int32_t)bmp180_cal_data[bmp_idx].cal_ac2 * b6) >> 11;
  int32_t x3 = x1 + x2;
  int32_t b3 = ((((int32_t)bmp180_cal_data[bmp_idx].cal_ac1 * 4 + x3) << BMP180_OSS) + 2) >> 2;

  x1 = ((int32_t)bmp180_cal_data[bmp_idx].cal_ac3 * b6) >> 13;
  x2 = ((int32_t)bmp180_cal_data[bmp_idx].cal_b1 * ((b6 * b6) >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;
  uint32_t b4 = ((uint32_t)bmp180_cal_data[bmp_idx].cal_ac4 * (uint32_t)(x3 + 32768)) >> 15;
  uint32_t b7 = ((uint32_t)up - b3) * (uint32_t)(50000UL >> BMP180_OSS);

  int32_t p;
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
  bmp_sensors[bmp_idx].bmp_pressure = (float)p / 100.0f;  // convert to mbar
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

typedef struct {
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
  int16_t  dig_H2;
  int16_t  dig_H4;
  int16_t  dig_H5;
  uint8_t  dig_H1;
  uint8_t  dig_H3;
  int8_t   dig_H6;
} Bme280CalibrationData_t;

Bme280CalibrationData_t *Bme280CalibrationData = nullptr;

bool Bmx280Calibrate(uint8_t bmp_idx) {
  //  if (I2cRead8(bmp_address, BMP_REGISTER_CHIPID, bus) != BME280_CHIPID) return false;

  if (!Bme280CalibrationData) {
    Bme280CalibrationData = (Bme280CalibrationData_t*)malloc(BMP_MAX_SENSORS * sizeof(Bme280CalibrationData_t));
  }
  if (!Bme280CalibrationData) { return false; }

  uint8_t address = bmp_sensors[bmp_idx].bmp_address;
  uint8_t bus = bmp_sensors[bmp_idx].bmp_bus;
  Bme280CalibrationData[bmp_idx].dig_T1 = I2cRead16LE(address, BME280_REGISTER_DIG_T1, bus);
  Bme280CalibrationData[bmp_idx].dig_T2 = I2cReadS16_LE(address, BME280_REGISTER_DIG_T2, bus);
  Bme280CalibrationData[bmp_idx].dig_T3 = I2cReadS16_LE(address, BME280_REGISTER_DIG_T3, bus);
  Bme280CalibrationData[bmp_idx].dig_P1 = I2cRead16LE(address, BME280_REGISTER_DIG_P1, bus);
  Bme280CalibrationData[bmp_idx].dig_P2 = I2cReadS16_LE(address, BME280_REGISTER_DIG_P2, bus);
  Bme280CalibrationData[bmp_idx].dig_P3 = I2cReadS16_LE(address, BME280_REGISTER_DIG_P3, bus);
  Bme280CalibrationData[bmp_idx].dig_P4 = I2cReadS16_LE(address, BME280_REGISTER_DIG_P4, bus);
  Bme280CalibrationData[bmp_idx].dig_P5 = I2cReadS16_LE(address, BME280_REGISTER_DIG_P5, bus);
  Bme280CalibrationData[bmp_idx].dig_P6 = I2cReadS16_LE(address, BME280_REGISTER_DIG_P6, bus);
  Bme280CalibrationData[bmp_idx].dig_P7 = I2cReadS16_LE(address, BME280_REGISTER_DIG_P7, bus);
  Bme280CalibrationData[bmp_idx].dig_P8 = I2cReadS16_LE(address, BME280_REGISTER_DIG_P8, bus);
  Bme280CalibrationData[bmp_idx].dig_P9 = I2cReadS16_LE(address, BME280_REGISTER_DIG_P9, bus);
  if (BME280_CHIPID == bmp_sensors[bmp_idx].bmp_type) {  // #1051
    Bme280CalibrationData[bmp_idx].dig_H1 = I2cRead8(address, BME280_REGISTER_DIG_H1, bus);
    Bme280CalibrationData[bmp_idx].dig_H2 = I2cReadS16_LE(address, BME280_REGISTER_DIG_H2, bus);
    Bme280CalibrationData[bmp_idx].dig_H3 = I2cRead8(address, BME280_REGISTER_DIG_H3, bus);
    Bme280CalibrationData[bmp_idx].dig_H4 = (I2cRead8(address, BME280_REGISTER_DIG_H4, bus) << 4) | (I2cRead8(address, BME280_REGISTER_DIG_H4 + 1, bus) & 0xF);
    Bme280CalibrationData[bmp_idx].dig_H5 = (I2cRead8(address, BME280_REGISTER_DIG_H5 + 1, bus) << 4) | (I2cRead8(address, BME280_REGISTER_DIG_H5, bus) >> 4);
    Bme280CalibrationData[bmp_idx].dig_H6 = (int8_t)I2cRead8(address, BME280_REGISTER_DIG_H6, bus);
    I2cWrite8(address, BME280_REGISTER_CONTROL, 0x00, bus);      // sleep mode since writes to config can be ignored in normal mode (Datasheet 5.4.5/6 page 27)
    // Set before CONTROL_meas (DS 5.4.3)
    I2cWrite8(address, BME280_REGISTER_CONTROLHUMID, 0x01, bus); // 1x oversampling
    I2cWrite8(address, BME280_REGISTER_CONFIG, 0xA0, bus);       // 1sec standby between measurements (to limit self heating), IIR filter off
    I2cWrite8(address, BME280_REGISTER_CONTROL, 0x27, bus);      // 1x oversampling, normal mode
  } else {
    I2cWrite8(address, BME280_REGISTER_CONTROL, 0xB7, bus);      // 16x oversampling, normal mode (Adafruit)
  }
  return true;
}

void Bme280ReadFailed(uint8_t bmp_idx) {
  if (bmp_sensors[bmp_idx].bmp_fail < BMP_FAIL_LIMIT) {
    bmp_sensors[bmp_idx].bmp_fail++;
  } else {                                                // Report null instead of stale or bogus values
    bmp_sensors[bmp_idx].bmp_temperature = NAN;
    bmp_sensors[bmp_idx].bmp_pressure = NAN;
    bmp_sensors[bmp_idx].bmp_humidity = NAN;
  }
}

void Bme280Read(uint8_t bmp_idx) {
  if (!Bme280CalibrationData) { return; }

  uint8_t address = bmp_sensors[bmp_idx].bmp_address;
  uint8_t bus = bmp_sensors[bmp_idx].bmp_bus;

  uint8_t ctrl;
  if (!I2cValidRead8(&ctrl, address, BME280_REGISTER_CONTROL, bus)) {
    Bme280ReadFailed(bmp_idx);
    return;
  }
  if ((ctrl & 0x03) != 0x03) {                            // Not in normal mode: sensor was reset (e.g. supply glitch)
    AddLog(LOG_LEVEL_INFO, PSTR("BMP: %s lost config (ctrl %02X), re-init"), bmp_sensors[bmp_idx].bmp_name, ctrl);
    Bmx280Calibrate(bmp_idx);
    Bme280ReadFailed(bmp_idx);
    return;
  }
  int32_t adc_T;
  int32_t adc_P;
  if (!I2cValidRead24(&adc_T, address, BME280_REGISTER_TEMPDATA, bus) ||
      !I2cValidRead24(&adc_P, address, BME280_REGISTER_PRESSUREDATA, bus) ||
      (0x800000 == adc_T) || (0x800000 == adc_P)) {       // 0x800000 = reset value / measurement skipped
    Bme280ReadFailed(bmp_idx);
    return;
  }
  bmp_sensors[bmp_idx].bmp_fail = 0;
  adc_T >>= 4;

  int32_t vart1 = ((((adc_T >> 3) - ((int32_t)Bme280CalibrationData[bmp_idx].dig_T1 << 1))) * ((int32_t)Bme280CalibrationData[bmp_idx].dig_T2)) >> 11;
  int32_t vart2 = (((((adc_T >> 4) - ((int32_t)Bme280CalibrationData[bmp_idx].dig_T1)) * ((adc_T >> 4) - ((int32_t)Bme280CalibrationData[bmp_idx].dig_T1))) >> 12) *
    ((int32_t)Bme280CalibrationData[bmp_idx].dig_T3)) >> 14;
  int32_t t_fine = vart1 + vart2;
  float T = (t_fine * 5 + 128) >> 8;
  bmp_sensors[bmp_idx].bmp_temperature = T / 100.0f;

  adc_P >>= 4;

  int64_t var1 = ((int64_t)t_fine) - 128000;
  int64_t var2 = var1 * var1 * (int64_t)Bme280CalibrationData[bmp_idx].dig_P6;
  var2 = var2 + ((var1 * (int64_t)Bme280CalibrationData[bmp_idx].dig_P5) << 17);
  var2 = var2 + (((int64_t)Bme280CalibrationData[bmp_idx].dig_P4) << 35);
  var1 = ((var1 * var1 * (int64_t)Bme280CalibrationData[bmp_idx].dig_P3) >> 8) + ((var1 * (int64_t)Bme280CalibrationData[bmp_idx].dig_P2) << 12);
  var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)Bme280CalibrationData[bmp_idx].dig_P1) >> 33;
  if (0 == var1) {
    return; // avoid exception caused by division by zero
  }
  int64_t p = 1048576 - adc_P;
  p = (((p << 31) - var2) * 3125) / var1;
  var1 = (((int64_t)Bme280CalibrationData[bmp_idx].dig_P9) * (p >> 13) * (p >> 13)) >> 25;
  var2 = (((int64_t)Bme280CalibrationData[bmp_idx].dig_P8) * p) >> 19;
  p = ((p + var1 + var2) >> 8) + (((int64_t)Bme280CalibrationData[bmp_idx].dig_P7) << 4);
  bmp_sensors[bmp_idx].bmp_pressure = (float)p / 25600.0f;

  if (BMP280_CHIPID == bmp_sensors[bmp_idx].bmp_type) { return; }

  int32_t adc_H = I2cRead16(address, BME280_REGISTER_HUMIDDATA, bus);

  int32_t v_x1_u32r = (t_fine - ((int32_t)76800));
  v_x1_u32r = (((((adc_H << 14) - (((int32_t)Bme280CalibrationData[bmp_idx].dig_H4) << 20) -
    (((int32_t)Bme280CalibrationData[bmp_idx].dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *
    (((((((v_x1_u32r * ((int32_t)Bme280CalibrationData[bmp_idx].dig_H6)) >> 10) *
    (((v_x1_u32r * ((int32_t)Bme280CalibrationData[bmp_idx].dig_H3)) >> 11) + ((int32_t)32768))) >> 10) +
    ((int32_t)2097152)) * ((int32_t)Bme280CalibrationData[bmp_idx].dig_H2) + 8192) >> 14));
  v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
    ((int32_t)Bme280CalibrationData[bmp_idx].dig_H1)) >> 4));
  v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
  v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
  float h = (v_x1_u32r >> 12);
  bmp_sensors[bmp_idx].bmp_humidity = h / 1024.0f;
}

#ifdef USE_BME68X
/*********************************************************************************************\
 * BME68x support by Bosch https://github.com/BoschSensortec/BME68x-Sensor-API
\*********************************************************************************************/

//#define BME68X_DO_NOT_USE_FPU

#include <bme68x.h>

struct bme68x_dev *bme_dev = nullptr;
struct bme68x_conf *bme_conf = nullptr;
struct bme68x_heatr_conf *bme_heatr_conf = nullptr;

uint8_t bmp68x_bus = 0;

// bme68x callbacks
static void Bme68x_Delayus(uint32_t period, void *intf_ptr) {
  delayMicroseconds(period);
}
int8_t Bme68x_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {
  uint8_t dev_addr = *(uint8_t*)intf_ptr;
  return I2cReadBuffer(dev_addr, reg_addr, reg_data, (uint16_t)len, bmp68x_bus);
}
int8_t Bme68x_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {
  uint8_t dev_addr = *(uint8_t*)intf_ptr;
  return I2cWriteBuffer(dev_addr, reg_addr, (uint8_t *)reg_data, (uint16_t)len, bmp68x_bus);
}

static float Bme680AbsHumidity(float temp_c, float hum_pct) {
  float rho_max = (6.112f * 100.0f * expf((17.62f * temp_c) / (243.12f + temp_c))) / (461.52f * (temp_c + 273.15f));
  return hum_pct * 10.0f * rho_max;
}

static float Bme680IaqCompFactor(float temp_c, float hum_pct) {
  return expf(BME680_IAQ_PH_SLOPE * Bme680AbsHumidity(temp_c, hum_pct));
}

void Bme680IaqStore(void);

static void Bme680IaqReset(uint8_t bmp_idx) {
  bme68x_iaq_t *s = &bmp_sensors[bmp_idx].bme680_iaq;
  s->baseline = 0.0f;
  s->sum = 0.0f;
  s->count = 0;
  s->iaq = 0.0f;
  s->tvoc = 0.0f;
}

static void Bme680IaqUpdate(uint8_t bmp_idx, float gas_kohm, float temp_c, float hum_pct) {
  bme68x_iaq_t *s = &bmp_sensors[bmp_idx].bme680_iaq;

  if (!(gas_kohm >= BME680_IAQ_R_MIN) || !(gas_kohm <= BME680_IAQ_R_MAX)) { return; }

  float comp_gas = gas_kohm * Bme680IaqCompFactor(temp_c, hum_pct);

  if (s->baseline <= 0.0f) {
    s->sum += comp_gas;
    s->count++;
    if (s->count >= BME680_IAQ_BURNIN) {
      s->baseline = s->sum / s->count;
      s->sum = 0.0f;
      s->count = 0;
      Bme680IaqStore();
      SettingsSave(0);
    }
    return;
  }

  float x = (s->baseline / comp_gas) - 1.0f;
  if (x < 0.0f) { x = 0.0f; }

  float u = BME680_IAQ_HILL_K * x;
  u = u * u;
  s->iaq = 500.0f * u / (1.0f + u);
  if (s->iaq < 0.0f) { s->iaq = 0.0f; }
  if (s->iaq > 500.0f) { s->iaq = 500.0f; }

  if (x > 0.0f) {
    s->tvoc = BME680_IAQ_TVOC_MAX * x / (x + BME680_IAQ_TVOC_HALF);
  } else {
    s->tvoc = 0.0f;
  }

  if (comp_gas > s->baseline) {
    s->baseline += BME680_IAQ_RISE_ALPHA * (comp_gas - s->baseline);
  } else {
    s->baseline += BME680_IAQ_AGING_ALPHA * (comp_gas - s->baseline);
  }
}

static int16_t Bme680IaqProgress(uint8_t bmp_idx) {
  bme68x_iaq_t *s = &bmp_sensors[bmp_idx].bme680_iaq;
  if (s->baseline > 0.0f) { return -1; }
  return (int16_t)((uint32_t)s->count * 100 / BME680_IAQ_BURNIN);
}

void Bme680IaqRestore(void) {
  if (!Settings || bmp_count < 1) { return; }
  float stored = Settings->bme680_iaq_baseline[0];
  if (stored >= BME680_IAQ_R_MIN && stored < BME680_IAQ_BASE_MAX) {
    if (BME680_CHIPID == bmp_sensors[0].bmp_type) {
      bmp_sensors[0].bme680_iaq.baseline = stored;
    }
  }
}

void Bme680IaqStore(void) {
  if (!Settings || bmp_count < 1) { return; }
  if (BME680_CHIPID == bmp_sensors[0].bmp_type) {
    Settings->bme680_iaq_baseline[0] = bmp_sensors[0].bme680_iaq.baseline;
  } else {
    Settings->bme680_iaq_baseline[0] = 0.0f;
  }
}

void Bme680IaqCommand(void) {
  char arg[16];
  char str[FLOATSZ];

  int8_t bmp_idx = -1;
  for (uint32_t i = 0; i < bmp_count; i++) {
    if (bmp_sensors[i].bmp_type == BME680_CHIPID) { bmp_idx = i; break; }
  }

  if (bmp_idx < 0) {
    Response_P(PSTR("{\"%s\":{\"Error\":\"No BME680 detected\"}}"), kBmpIaqNames);
    return;
  }

  if (XdrvMailbox.data_len == 0) {
    float factor = Bme680IaqCompFactor(bmp_sensors[bmp_idx].bmp_temperature, bmp_sensors[bmp_idx].bmp_humidity);
    dtostrfd(bmp_sensors[bmp_idx].bme680_iaq.baseline / factor, 0, str);
    Response_P(PSTR("{\"%s\":{\"Baseline\":\"%s kOhm\"}}"), kBmpIaqNames, str);
    return;
  }

  strlcpy(arg, Trim(XdrvMailbox.data), sizeof(arg));
  for (uint32_t i = 0; i < sizeof(arg); i++) {
    if (arg[i] == ',' || arg[i] == 0) { arg[i] = 0; break; }
    arg[i] = toupper(arg[i]);
  }

  if (strcmp(arg, "RESET") == 0) {
    Bme680IaqReset(bmp_idx);
    Response_P(PSTR("{\"%s\":{\"Reset\":\"Burn-in restarts\"}}"), kBmpIaqNames);
  }
  else {
    float baseline = XdrvMailbox.payload;
    if (baseline >= BME680_IAQ_R_MIN && baseline <= BME680_IAQ_R_MAX) {
      float factor = Bme680IaqCompFactor(bmp_sensors[bmp_idx].bmp_temperature, bmp_sensors[bmp_idx].bmp_humidity);
      bmp_sensors[bmp_idx].bme680_iaq.baseline = baseline * factor;
      SettingsSaveAll();
      dtostrfd(baseline, 0, str);
      Response_P(PSTR("{\"%s\":{\"Baseline\":\"%s kOhm stored\"}}"), kBmpIaqNames, str);
    } else {
      Response_P(PSTR("{\"%s\":{\"Error\":\"Value out of range 1..%d kOhm\"}}"), kBmpIaqNames, (int)BME680_IAQ_R_MAX);
    }
  }
}

bool Bme680Init(uint8_t bmp_idx) {
  bmp68x_bus = bmp_sensors[bmp_idx].bmp_bus;

  if (!bme_dev) {
    bme_heatr_conf = (bme68x_heatr_conf*)malloc(BMP_MAX_SENSORS * sizeof(bme68x_heatr_conf));
    bme_conf = (bme68x_conf*)malloc(BMP_MAX_SENSORS * sizeof(bme68x_conf));
    bme_dev = (bme68x_dev*)malloc(BMP_MAX_SENSORS * sizeof(bme68x_dev));
  }
  if (!bme_dev) { return false; }

  bme_dev[bmp_idx].intf_ptr = &bmp_sensors[bmp_idx].bmp_address;
  bme_dev[bmp_idx].intf = BME68X_I2C_INTF;
  bme_dev[bmp_idx].read = Bme68x_i2c_read;
  bme_dev[bmp_idx].write = Bme68x_i2c_write;
  bme_dev[bmp_idx].delay_us = Bme68x_Delayus;
  // amb_temp can be set to 25 prior to configuring the gas sensor
  // or by performing a few temperature readings without operating the gas sensor.
  bme_dev[bmp_idx].amb_temp = 25;
  int8_t rslt = bme68x_init(&bme_dev[bmp_idx]);
  if (rslt != BME68X_OK) { return false; }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("BME: Gas variant %d"), bme_dev[bmp_idx].variant_id);

//  rslt = bme68x_get_conf(&bme_conf[bmp_idx], &bme_dev[bmp_idx]);
//  if (rslt != BME68X_OK) { return false; }
  // Set the temperature, pressure and humidity settings
  bme_conf[bmp_idx].os_hum = BME68X_OS_2X;
  bme_conf[bmp_idx].os_pres = BME68X_OS_4X;
  bme_conf[bmp_idx].os_temp = BME68X_OS_8X;
  bme_conf[bmp_idx].filter = BME68X_FILTER_SIZE_3;
  bme_conf[bmp_idx].odr = BME68X_ODR_NONE;          // This parameter defines the sleep duration after each profile
  rslt = bme68x_set_conf(&bme_conf[bmp_idx], &bme_dev[bmp_idx]);
  if (rslt != BME68X_OK) { return false; }

  // Set the gas sensor settings
  bme_heatr_conf[bmp_idx].enable = BME68X_ENABLE;
  // Create a ramp heat waveform in 3 steps
  bme_heatr_conf[bmp_idx].heatr_temp = 320;  // degree Celsius
  bme_heatr_conf[bmp_idx].heatr_dur = 150;   // milliseconds
  rslt = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &bme_heatr_conf[bmp_idx], &bme_dev[bmp_idx]);
  if (rslt != BME68X_OK) { return false; }

  bmp_sensors[bmp_idx].bme680_state = 0;
  Bme680IaqReset(bmp_idx);

  return true;
}

void Bme680Read(uint8_t bmp_idx) {
  if (!bme_dev) { return; }

  bmp68x_bus = bmp_sensors[bmp_idx].bmp_bus;

  int8_t rslt = BME68X_OK;

  if (BME680_CHIPID == bmp_sensors[bmp_idx].bmp_type) {
    if (0 == bmp_sensors[bmp_idx].bme680_state) {
      // Trigger the next measurement if you would like to read data out continuously
      rslt = bme68x_set_op_mode(BME68X_FORCED_MODE, &bme_dev[bmp_idx]);
      if (rslt != BME68X_OK) { return; }

      // Calculate delay period in microseconds
//      del_period = bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &bme) + (heatr_conf.heatr_dur * 1000);
//      bme.delay_us(del_period, bme.intf_ptr);

      bmp_sensors[bmp_idx].bme680_state = 1;
    } else {
bmp_sensors[bmp_idx].bme680_state = 0;

      struct bme68x_data data;
      uint8_t n_fields;
      rslt = bme68x_get_data(BME68X_FORCED_MODE, &data, &n_fields, &bme_dev[bmp_idx]);
      if (rslt != BME68X_OK) { return; }

#ifdef BME68X_DO_NOT_USE_FPU
      bmp_sensors[bmp_idx].bmp_temperature = data.temperature / 100.0f;  // Temperature in degree celsius x100
      bmp_sensors[bmp_idx].bmp_humidity = data.humidity / 1000.0f;       // Humidity in % relative humidity x1000
#else
      bmp_sensors[bmp_idx].bmp_temperature = data.temperature;          // Temperature in degree celsius
      bmp_sensors[bmp_idx].bmp_humidity = data.humidity;                // Humidity in % relative humidity
#endif
      bmp_sensors[bmp_idx].bmp_pressure = data.pressure / 100.0f;        // Pressure in Pascal (converted to hPa)
      // Avoid using measurements from an unstable heating setup
      if (data.status & BME68X_GASM_VALID_MSK) {
        bmp_sensors[bmp_idx].bmp_gas_resistance = data.gas_resistance / 1000.0f;  // Gas resistance in Ohms (converted to kOhm)
        Bme680IaqUpdate(0, bmp_sensors[bmp_idx].bmp_gas_resistance, bmp_sensors[bmp_idx].bmp_temperature, bmp_sensors[bmp_idx].bmp_humidity);
      } else {
        bmp_sensors[bmp_idx].bmp_gas_resistance = 0;
      }
    }
  }
  return;
}

#endif  // USE_BME68X

/********************************************************************************************/

void BmpDetect(void) {
  if (!bmp_sensors) {
    bmp_sensors = (bmp_sensors_t*)calloc(BMP_MAX_SENSORS, sizeof(bmp_sensors_t));
  }
  if (!bmp_sensors) { return; }

  for (uint32_t i = 0; i < BMP_MAX_SENSORS; i++) {
    uint8_t bus = i >> 1;
    if (!I2cSetDevice(bmp_addresses[i & 1], bus)) { continue; }
    uint8_t bmp_type = I2cRead8(bmp_addresses[i & 1], BMP_REGISTER_CHIPID, bus);
    if (bmp_type) {
      bmp_sensors[bmp_count].bmp_address = bmp_addresses[i &1];
      bmp_sensors[bmp_count].bmp_bus = bus;
      bmp_sensors[bmp_count].bmp_type = bmp_type;
      bmp_sensors[bmp_count].bmp_model = 0;

      bool success = false;
      switch (bmp_type) {
        case BMP180_CHIPID:
          success = Bmp180Calibration(bmp_count);
          break;
        case BME280_CHIPID:
          bmp_sensors[bmp_count].bmp_model++;  // 2
        case BMP280_CHIPID:
          bmp_sensors[bmp_count].bmp_model++;  // 1
          success = Bmx280Calibrate(bmp_count);
          break;
#ifdef USE_BME68X
        case BME680_CHIPID:
          bmp_sensors[bmp_count].bmp_model = 3;  // 3
          success = Bme680Init(bmp_count);
          break;
#endif  // USE_BME68X
      }
      if (success) {
        GetTextIndexed(bmp_sensors[bmp_count].bmp_name, sizeof(bmp_sensors[bmp_count].bmp_name), bmp_sensors[bmp_count].bmp_model, kBmpTypes);
        I2cSetActiveFound(bmp_sensors[bmp_count].bmp_address, bmp_sensors[bmp_count].bmp_name, bmp_sensors[bmp_count].bmp_bus);
        bmp_count++;
      }
    }
  }
#ifdef USE_BME68X
  Bme680IaqRestore();
#endif  // USE_BME68X
}

void BmpRead(void) {
#ifdef USE_DEEPSLEEP
  // Prevent updating measurments once BMP has been put to sleep (just before ESP enters deepsleep)
  if (bmp_deepsleep) return;
#endif
  for (uint32_t bmp_idx = 0; bmp_idx < bmp_count; bmp_idx++) {
    switch (bmp_sensors[bmp_idx].bmp_type) {
      case BMP180_CHIPID:
        Bmp180Read(bmp_idx);
        break;
      case BMP280_CHIPID:
      case BME280_CHIPID:
        Bme280Read(bmp_idx);
        break;
#ifdef USE_BME68X
      case BME680_CHIPID:
        Bme680Read(bmp_idx);
        break;
#endif  // USE_BME68X
    }
  }
}

void BmpShow(bool json) {
  for (uint32_t bmp_idx = 0; bmp_idx < bmp_count; bmp_idx++) {
    if (bmp_sensors[bmp_idx].bmp_type) {
      float bmp_sealevel = ConvertPressureForSeaLevel(bmp_sensors[bmp_idx].bmp_pressure);
      float bmp_temperature = ConvertTemp(bmp_sensors[bmp_idx].bmp_temperature);
      float bmp_pressure = ConvertPressure(bmp_sensors[bmp_idx].bmp_pressure);

      char name[16];
      // BMP280
      strlcpy(name, bmp_sensors[bmp_idx].bmp_name, sizeof(bmp_sensors[bmp_idx].bmp_name));
      if (bmp_count > 1) {
        // BMP280-77
        snprintf_P(name, sizeof(name), PSTR("%s%c%02X"), name, IndexSeparator(), bmp_sensors[bmp_idx].bmp_address);
#if MAX_I2C > 1
        if (TasmotaGlobal.i2c_enabled[1]) {           // Second bus enabled
          uint8_t bus = bmp_sensors[0].bmp_bus;
          for (uint32_t i = 1; i < bmp_count; i++) {
            if (bus != bmp_sensors[i].bmp_bus) {     // Different busses
              // BMP280-77-1
              snprintf_P(name, sizeof(name), PSTR("%s%c%d"), name, IndexSeparator(), bmp_sensors[bmp_idx].bmp_bus +1);
              break;
            }
          }
        }
#endif  // MAX_I2C
      }

      char pressure[33];
      dtostrfd(bmp_pressure, Settings->flag2.pressure_resolution, pressure);
      char sea_pressure[33];
      dtostrfd(bmp_sealevel, Settings->flag2.pressure_resolution, sea_pressure);

      float bmp_humidity = ConvertHumidity(bmp_sensors[bmp_idx].bmp_humidity);
      char humidity[33];
      dtostrfd(bmp_humidity, Settings->flag2.humidity_resolution, humidity);
      float f_dewpoint = CalcTempHumToDew(bmp_temperature, bmp_humidity);
      char dewpoint[33];
      dtostrfd(f_dewpoint, Settings->flag2.temperature_resolution, dewpoint);
#ifdef USE_HEAT_INDEX
      float f_heatindex = CalcTemHumToHeatIndex(bmp_temperature, bmp_humidity);
      char heatindex[33];
      dtostrfd(f_heatindex, Settings->flag2.temperature_resolution, heatindex);
#endif  // USE_HEAT_INDEX
#ifdef USE_BME68X
      char gas_resistance[33];
      dtostrfd(bmp_sensors[bmp_idx].bmp_gas_resistance, 2, gas_resistance);
      bool iaq_valid = (bmp_sensors[bmp_idx].bme680_iaq.baseline > 0.0f);
      char iaq[33];
      char tvoc[33];
      char iaq_quality[8];
      if (iaq_valid) {
        dtostrfd(bmp_sensors[bmp_idx].bme680_iaq.iaq, 1, iaq);
        dtostrfd(bmp_sensors[bmp_idx].bme680_iaq.tvoc, 0, tvoc);
        dtostrfd(100.0f * (1.0f - bmp_sensors[bmp_idx].bme680_iaq.iaq / 500.0f), 0, iaq_quality);
      }
#endif  // USE_BME68X

      if (json) {
        char json_humidity[100];
#ifdef USE_HEAT_INDEX
        snprintf_P(json_humidity, sizeof(json_humidity), PSTR(",\"" D_JSON_HUMIDITY "\":%s,\"" D_JSON_DEWPOINT "\":%s,\"" D_JSON_HEATINDEX "\":%s"), humidity, dewpoint, heatindex);
#else
        snprintf_P(json_humidity, sizeof(json_humidity), PSTR(",\"" D_JSON_HUMIDITY "\":%s,\"" D_JSON_DEWPOINT "\":%s"), humidity, dewpoint);
#endif  // USE_HEAT_INDEX
        char json_sealevel[40];
        snprintf_P(json_sealevel, sizeof(json_sealevel), PSTR(",\"" D_JSON_PRESSUREATSEALEVEL "\":%s"), sea_pressure);
#ifdef USE_BME68X
        char json_gas[72];
        if (iaq_valid) {
          snprintf_P(json_gas, sizeof(json_gas),
            PSTR(",\"" D_JSON_GAS "\":%s,\"" D_JSON_AIRQUALITY "\":%s,\"" D_JSON_TVOC "\":%s"),
            gas_resistance, iaq, tvoc);
        } else {
          snprintf_P(json_gas, sizeof(json_gas), PSTR(",\"" D_JSON_GAS "\":%s"), gas_resistance);
        }

        ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_TEMPERATURE "\":%*_f%s,\"" D_JSON_PRESSURE "\":%s%s%s}"),
          name,
          Settings->flag2.temperature_resolution, &bmp_temperature,
          (bmp_sensors[bmp_idx].bmp_model >= 2) ? json_humidity : "",
          pressure,
          (Settings->altitude != 0) ? json_sealevel : "",
          (bmp_sensors[bmp_idx].bmp_model >= 3) ? json_gas : "");
#else
        ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_TEMPERATURE "\":%*_f%s,\"" D_JSON_PRESSURE "\":%s%s}"),
          name, Settings->flag2.temperature_resolution, &bmp_temperature, (bmp_sensors[bmp_idx].bmp_model >= 2) ? json_humidity : "", pressure, (Settings->altitude != 0) ? json_sealevel : "");
#endif  // USE_BME68X

#ifdef USE_DOMOTICZ
        if ((0 == TasmotaGlobal.tele_period) && (0 == bmp_idx)) {  // We want the same first sensor to report to Domoticz in case a read is missed
          DomoticzTempHumPressureSensor(bmp_temperature, bmp_humidity, bmp_pressure);
#ifdef USE_BME68X
          if (bmp_sensors[bmp_idx].bmp_model >= 3) { DomoticzSensor(DZ_AIRQUALITY, (uint32_t)bmp_sensors[bmp_idx].bmp_gas_resistance); }
#endif  // USE_BME68X
        }
#endif  // USE_DOMOTICZ

#ifdef USE_KNX
        if (0 == TasmotaGlobal.tele_period) {
          KnxSensor(KNX_TEMPERATURE, bmp_temperature);
          KnxSensor(KNX_HUMIDITY, bmp_humidity);
        }
#endif  // USE_KNX

#ifdef USE_WEBSERVER
      } else {
        WSContentSend_Temp(name, bmp_temperature);
        if (bmp_sensors[bmp_idx].bmp_model >= 2) {
          WSContentSend_PD(HTTP_SNS_HUM, name, humidity);
          WSContentSend_PD(HTTP_SNS_DEW, name, dewpoint, TempUnit());
#ifdef USE_HEAT_INDEX
          WSContentSend_PD(HTTP_SNS_HEATINDEX, name, heatindex, TempUnit());
#endif  // USE_HEAT_INDEX
        }
        WSContentSend_PD(HTTP_SNS_PRESSURE, name, pressure, PressureUnit().c_str());
        if (Settings->altitude != 0) {
          WSContentSend_PD(HTTP_SNS_SEAPRESSURE, name, sea_pressure, PressureUnit().c_str());
        }
#ifdef USE_BME68X
        if (bmp_sensors[bmp_idx].bmp_model >= 3) {
          WSContentSend_PD(PSTR("{s}%s " D_GAS "{m}%s " D_UNIT_KILOOHM "{e}"), name, gas_resistance);
          if (iaq_valid) {
            WSContentSend_PD(PSTR("{s}%s " D_AIRQUALITY "{m}%s{e}"), name, iaq_quality);
            WSContentSend_PD(PSTR("{s}%s " D_JSON_TVOC "{m}%s " D_UNIT_PARTS_PER_BILLION "{e}"), name, tvoc);
          } else {
            int16_t iaq_prog = Bme680IaqProgress(bmp_idx);
            char iaq_prog_str[8];
            snprintf_P(iaq_prog_str, sizeof(iaq_prog_str), PSTR("%d"), iaq_prog);
            WSContentSend_PD(PSTR("{s}%s " D_CALIBRATION "{m}%s%%{e}"), name, iaq_prog_str);
            WSContentSend_P(PSTR("{s}" D_IAQ_CALIBRATING "{e}"));
          }
        }
#endif  // USE_BME68X

#endif  // USE_WEBSERVER
      }
    }
  }
}

#ifdef USE_DEEPSLEEP

void BMP_EnterSleep(void) {
  if (DeepSleepEnabled()) {
    for (uint32_t bmp_idx = 0; bmp_idx < bmp_count; bmp_idx++) {
      switch (bmp_sensors[bmp_idx].bmp_type) {
        case BMP180_CHIPID:
        case BMP280_CHIPID:
        case BME280_CHIPID:
          I2cWrite8(bmp_sensors[bmp_idx].bmp_address, BMP_REGISTER_RESET, BMP_CMND_RESET, bmp_sensors[bmp_idx].bmp_bus);
          break;
        default:
          break;
      }
    }
  }
}

#endif // USE_DEEPSLEEP

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns09(uint32_t function) {
  if (!I2cEnabled(XI2C_10)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    BmpDetect();
  }
  else if (bmp_count) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        BmpRead();
        break;
      case FUNC_JSON_APPEND:
        BmpShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        BmpShow(0);
        break;
#endif  // USE_WEBSERVER
      case FUNC_SAVE_BEFORE_RESTART:
#ifdef USE_BME68X
        Bme680IaqStore();
#endif
#ifdef USE_DEEPSLEEP
        BMP_EnterSleep();
        bmp_deepsleep = 1;
#endif
        break;
#ifdef USE_BME68X
      case FUNC_COMMAND:
        result = DecodeCommand(kBmpCommands, BmpCommand);
        break;
#endif  // USE_BME68X
    }
  }
  return result;
}

#endif  // USE_BMP
#endif  // USE_I2C
