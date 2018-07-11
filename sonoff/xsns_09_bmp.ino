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

int32_t  bmp180_b5 = 0;
void Bmp180Read()
  int32_t xt1 = (ut - (int32_t)cal_ac6) * ((int32_t)cal_ac5) >> 15;
  int32_t xt2 = ((int32_t)cal_mc << 11) / (xt1 + (int32_t)cal_md);
  int32_t bmp180_b5 = xt1 + xt2;

  bmp_temperature = ((bmp180_b5 + 8) >> 4) / 10.0;
}

double Bmp180ReadPressure()
{
  int32_t p;


  bmp_pressure = (float)p / 100.0;  // convert to mbar
int32_t t_fine;

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
void Bme280Read(void)
  int32_t var1;
  int32_t var2;

  int32_t vart1 = ((((adc_T >> 3) - ((int32_t)Bme280CalibrationData.dig_T1 << 1))) * ((int32_t)Bme280CalibrationData.dig_T2)) >> 11;
  int32_t vart2 = (((((adc_T >> 4) - ((int32_t)Bme280CalibrationData.dig_T1)) * ((adc_T >> 4) - ((int32_t)Bme280CalibrationData.dig_T1))) >> 12) *
  int32_t t_fine = vart1 + vart2;
  float T = (t_fine * 5 + 128) >> 8;
  bmp_temperature = T / 100.0;
}
double Bme280ReadPressure(void)
{
  int64_t var1;
  int64_t var2;
  int64_t p;

  // Must be done first to get the t_fine variable set up
  //  Bme280ReadTemperature();

  int64_t var1 = ((int64_t)t_fine) - 128000;
  int64_t var2 = var1 * var1 * (int64_t)Bme280CalibrationData.dig_P6;
    return; // avoid exception caused by division by zero
  int64_t p = 1048576 - adc_P;
  bmp_pressure = (float)p / 25600.0;
}
double Bme280ReadHumidity(void)
{
  int32_t v_x1_u32r;

  // Must be done first to get the t_fine variable set up
  //  Bme280ReadTemperature();
  int32_t v_x1_u32r = (t_fine - ((int32_t)76800));

  float h = (v_x1_u32r >> 12);
  bmp_humidity = h / 1024.0;

uint8_t bme680_state = 0;
float bme680_temperature;
float bme680_pressure;
float bme680_humidity;
float bme680_gas_resistance;
void Bme680Read()
      bmp_temperature = data.temperature / 100.0;
      bmp_humidity = data.humidity / 1000.0;
      bmp_pressure = data.pressure / 100.0;
        bmp_gas_resistance = data.gas_resistance / 1000.0;
        bmp_gas_resistance = 0;
  if (bmp_type) { return; }
    return;
  }
        bmp_model++;  // 1
      case BME280_CHIPID:
        bmp_model = 2;  // 2
        success = Bmx280Calibrate();
        break;
        bmp_model = 3;  // 3
void BmpRead()
  if (bmp_type) {
    float t = 0.0;
    float p = 0.0;
    float h = 0.0;
    float g = 0.0;
    float bmp_sealevel = 0.0;

        t = Bmp180ReadTemperature();
      Bmp180Read();
      Bme280Read();
      case BMP280_CHIPID:
        t = Bme280ReadTemperature();
        p = Bme280ReadPressure();
      Bme680Read();
        p = bme680_pressure / 100.0;
        h = bme680_humidity;
        g = bme680_gas_resistance / 1000.0;
    if (t != 0.0) {
    BmpDetect();
  else if (uptime &1) {
    BmpRead();
  }
    dtostrfd(t, Settings.flag2.temperature_resolution, temperature);
    dtostrfd(p, Settings.flag2.pressure_resolution, pressure);
    char humidity[10];
    dtostrfd(bmp_humidity, Settings.flag2.humidity_resolution, humidity);
    dtostrfd(bmp_gas_resistance, 2, gas_resistance);
        if (bmp_model >= 3) { DomoticzSensor(DZ_AIRQUALITY, (uint32_t)bmp_gas_resistance); }
        KnxSensor(KNX_TEMPERATURE, bmp_temperature);
        KnxSensor(KNX_HUMIDITY, bmp_humidity);
      case FUNC_INIT:
        if (tele_period == Settings.tele_period -2) {  // Allow 2 seconds to prepare BME680 readings
        break;
      case FUNC_EVERY_SECOND:
        BmpEverySecond();
#endif  // USE_BME680
