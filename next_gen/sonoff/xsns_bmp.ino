/*
 Copyright (c) 2017 Heiko Krupp and Theo Arends.  All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 - Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
 - Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef USE_I2C
#ifdef USE_BMP
/*********************************************************************************************\
 * BMP085, BMP180, BMP280, BME280 - Pressure and Temperature and Humidy (BME280 only)
 *
 * Source: Heiko Krupp and Adafruit Industries
\*********************************************************************************************/

#define BMP_ADDR             0x77

#define BMP180_CHIPID        0x55
#define BMP280_CHIPID        0x58
#define BME280_CHIPID        0x60

#define BMP_REGISTER_CHIPID  0xD0

uint8_t bmpaddr, bmptype = 0;
char bmpstype[7];

/*********************************************************************************************\
 * BMP085 and BME180
 *
 * Programmer : Heiko Krupp with changes from Theo Arends
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

int16_t cal_ac1,cal_ac2,cal_ac3,cal_b1,cal_b2,cal_mc,cal_md;
uint16_t cal_ac4,cal_ac5,cal_ac6;
int32_t bmp180_b5 = 0;

boolean bmp180_calibration()
{
  cal_ac1 = i2c_read16(bmpaddr, BMP180_AC1);
  cal_ac2 = i2c_read16(bmpaddr, BMP180_AC2);
  cal_ac3 = i2c_read16(bmpaddr, BMP180_AC3);
  cal_ac4 = i2c_read16(bmpaddr, BMP180_AC4);
  cal_ac5 = i2c_read16(bmpaddr, BMP180_AC5);
  cal_ac6 = i2c_read16(bmpaddr, BMP180_AC6);
  cal_b1  = i2c_read16(bmpaddr, BMP180_VB1);
  cal_b2  = i2c_read16(bmpaddr, BMP180_VB2);
  cal_mc  = i2c_read16(bmpaddr, BMP180_MC);
  cal_md  = i2c_read16(bmpaddr, BMP180_MD);

  // Check for Errors in calibration data. Value never is 0x0000 or 0xFFFF
  if(!cal_ac1 | !cal_ac2 | !cal_ac3 | !cal_ac4 | !cal_ac5 |
     !cal_ac6 | !cal_b1 | !cal_b2 | !cal_mc | !cal_md)
     return false;

  if((cal_ac1==0xFFFF)|
     (cal_ac2==0xFFFF)|
     (cal_ac3==0xFFFF)|
     (cal_ac4==0xFFFF)|
     (cal_ac5==0xFFFF)|
     (cal_ac6==0xFFFF)|
     (cal_b1==0xFFFF)|
     (cal_b2==0xFFFF)|
     (cal_mc==0xFFFF)|
     (cal_md==0xFFFF))
     return false;

  return true;
}

double bmp180_readTemperature()
{
  i2c_write8(bmpaddr, BMP180_REG_CONTROL, BMP180_TEMPERATURE);
  delay(5); // 5ms conversion time
  int ut = i2c_read16(bmpaddr, BMP180_REG_RESULT);
  int32_t x1 = (ut - (int32_t)cal_ac6) * ((int32_t)cal_ac5) >> 15;
  int32_t x2 = ((int32_t)cal_mc << 11) / (x1+(int32_t)cal_md);
  bmp180_b5=x1+x2;

  return ((bmp180_b5+8)>>4)/10.0;
}

double bmp180_readPressure()
{
  int32_t p;
  uint8_t msb,lsb,xlsb;

  i2c_write8(bmpaddr, BMP180_REG_CONTROL, BMP180_PRESSURE3); // Highest resolution
  delay(2 + (4 << BMP180_OSS)); // 26ms conversion time at ultra high resolution
  uint32_t up = i2c_read24(bmpaddr, BMP180_REG_RESULT);
  up >>= (8 - BMP180_OSS);

  int32_t b6 = bmp180_b5 - 4000;
  int32_t x1 = ((int32_t)cal_b2 * ( (b6 * b6)>>12 )) >> 11;
  int32_t x2 = ((int32_t)cal_ac2 * b6) >> 11;
  int32_t x3 = x1 + x2;
  int32_t b3 = ((((int32_t)cal_ac1*4 + x3) << BMP180_OSS) + 2)>>2;

  x1 = ((int32_t)cal_ac3 * b6) >> 13;
  x2 = ((int32_t)cal_b1 * ((b6 * b6) >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;
  uint32_t b4 = ((uint32_t)cal_ac4 * (uint32_t)(x3 + 32768)) >> 15;
  uint32_t b7 = ((uint32_t)up - b3) * (uint32_t)( 50000UL >> BMP180_OSS);

  if (b7 < 0x80000000) {
    p = (b7 * 2) / b4;
  } else {
    p = (b7 / b4) * 2;
  }

  x1 = (p >> 8) * (p >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * p) >> 16;

  p += ((x1 + x2 + (int32_t)3791)>>4);
  return p/100.0;  // convert to mbar
}

double bmp180_calcSealevelPressure(float pAbs, float altitude_meters)
{
  double pressure = pAbs*100.0;
  return (double)(pressure / pow(1.0-altitude_meters/44330, 5.255))/100.0;
}

/*********************************************************************************************\
 * BMP280 and BME280
 *
 * Programmer : BMP280/BME280 Datasheet and Adafruit with changes by Theo Arends
\*********************************************************************************************/

#define BME280_REGISTER_CONTROLHUMID   0xF2
#define BME280_REGISTER_CONTROL        0xF4
#define BME280_REGISTER_PRESSUREDATA   0xF7
#define BME280_REGISTER_TEMPDATA       0xFA
#define BME280_REGISTER_HUMIDDATA      0xFD

#define BME280_REGISTER_DIG_T1         0x88
#define BME280_REGISTER_DIG_T2         0x8A
#define BME280_REGISTER_DIG_T3         0x8C
#define BME280_REGISTER_DIG_P1         0x8E
#define BME280_REGISTER_DIG_P2         0x90
#define BME280_REGISTER_DIG_P3         0x92
#define BME280_REGISTER_DIG_P4         0x94
#define BME280_REGISTER_DIG_P5         0x96
#define BME280_REGISTER_DIG_P6         0x98
#define BME280_REGISTER_DIG_P7         0x9A
#define BME280_REGISTER_DIG_P8         0x9C
#define BME280_REGISTER_DIG_P9         0x9E
#define BME280_REGISTER_DIG_H1         0xA1
#define BME280_REGISTER_DIG_H2         0xE1
#define BME280_REGISTER_DIG_H3         0xE3
#define BME280_REGISTER_DIG_H4         0xE4
#define BME280_REGISTER_DIG_H5         0xE5
#define BME280_REGISTER_DIG_H6         0xE7

struct bme280_calib_data
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
} _bme280_calib;

int32_t t_fine;

boolean bmp280_calibrate()
{
//  if (i2c_read8(bmpaddr, BMP_REGISTER_CHIPID) != BMP280_CHIPID) return false;

  _bme280_calib.dig_T1 = i2c_read16_LE(bmpaddr, BME280_REGISTER_DIG_T1);
  _bme280_calib.dig_T2 = i2c_readS16_LE(bmpaddr, BME280_REGISTER_DIG_T2);
  _bme280_calib.dig_T3 = i2c_readS16_LE(bmpaddr, BME280_REGISTER_DIG_T3);
  _bme280_calib.dig_P1 = i2c_read16_LE(bmpaddr, BME280_REGISTER_DIG_P1);
  _bme280_calib.dig_P2 = i2c_readS16_LE(bmpaddr, BME280_REGISTER_DIG_P2);
  _bme280_calib.dig_P3 = i2c_readS16_LE(bmpaddr, BME280_REGISTER_DIG_P3);
  _bme280_calib.dig_P4 = i2c_readS16_LE(bmpaddr, BME280_REGISTER_DIG_P4);
  _bme280_calib.dig_P5 = i2c_readS16_LE(bmpaddr, BME280_REGISTER_DIG_P5);
  _bme280_calib.dig_P6 = i2c_readS16_LE(bmpaddr, BME280_REGISTER_DIG_P6);
  _bme280_calib.dig_P7 = i2c_readS16_LE(bmpaddr, BME280_REGISTER_DIG_P7);
  _bme280_calib.dig_P8 = i2c_readS16_LE(bmpaddr, BME280_REGISTER_DIG_P8);
  _bme280_calib.dig_P9 = i2c_readS16_LE(bmpaddr, BME280_REGISTER_DIG_P9);

//  i2c_write8(bmpaddr, BME280_REGISTER_CONTROL, 0x3F);       // Temp 1x oversampling, Press 16x oversampling, normal mode (Adafruit)
  i2c_write8(bmpaddr, BME280_REGISTER_CONTROL, 0xB7);       // 16x oversampling, normal mode (Adafruit)

  return true;
}

boolean bme280_calibrate()
{
//  if (i2c_read8(bmpaddr, BMP_REGISTER_CHIPID) != BME280_CHIPID) return false;

  _bme280_calib.dig_T1 = i2c_read16_LE(bmpaddr, BME280_REGISTER_DIG_T1);
  _bme280_calib.dig_T2 = i2c_readS16_LE(bmpaddr, BME280_REGISTER_DIG_T2);
  _bme280_calib.dig_T3 = i2c_readS16_LE(bmpaddr, BME280_REGISTER_DIG_T3);
  _bme280_calib.dig_P1 = i2c_read16_LE(bmpaddr, BME280_REGISTER_DIG_P1);
  _bme280_calib.dig_P2 = i2c_readS16_LE(bmpaddr, BME280_REGISTER_DIG_P2);
  _bme280_calib.dig_P3 = i2c_readS16_LE(bmpaddr, BME280_REGISTER_DIG_P3);
  _bme280_calib.dig_P4 = i2c_readS16_LE(bmpaddr, BME280_REGISTER_DIG_P4);
  _bme280_calib.dig_P5 = i2c_readS16_LE(bmpaddr, BME280_REGISTER_DIG_P5);
  _bme280_calib.dig_P6 = i2c_readS16_LE(bmpaddr, BME280_REGISTER_DIG_P6);
  _bme280_calib.dig_P7 = i2c_readS16_LE(bmpaddr, BME280_REGISTER_DIG_P7);
  _bme280_calib.dig_P8 = i2c_readS16_LE(bmpaddr, BME280_REGISTER_DIG_P8);
  _bme280_calib.dig_P9 = i2c_readS16_LE(bmpaddr, BME280_REGISTER_DIG_P9);
  _bme280_calib.dig_H1 = i2c_read8(bmpaddr, BME280_REGISTER_DIG_H1);
  _bme280_calib.dig_H2 = i2c_readS16_LE(bmpaddr, BME280_REGISTER_DIG_H2);
  _bme280_calib.dig_H3 = i2c_read8(bmpaddr, BME280_REGISTER_DIG_H3);
  _bme280_calib.dig_H4 = (i2c_read8(bmpaddr, BME280_REGISTER_DIG_H4) << 4) | (i2c_read8(bmpaddr, BME280_REGISTER_DIG_H4 + 1) & 0xF);
  _bme280_calib.dig_H5 = (i2c_read8(bmpaddr, BME280_REGISTER_DIG_H5 + 1) << 4) | (i2c_read8(bmpaddr, BME280_REGISTER_DIG_H5) >> 4);
  _bme280_calib.dig_H6 = (int8_t)i2c_read8(bmpaddr, BME280_REGISTER_DIG_H6);

  // Set before CONTROL_meas (DS 5.4.3)
  i2c_write8(bmpaddr, BME280_REGISTER_CONTROLHUMID, 0x05);  // 16x oversampling (Adafruit)
  i2c_write8(bmpaddr, BME280_REGISTER_CONTROL, 0xB7);       // 16x oversampling, normal mode (Adafruit)

  return true;
}

double bmp280_readTemperature(void)
{
  int32_t var1, var2;

  int32_t adc_T = i2c_read24(bmpaddr, BME280_REGISTER_TEMPDATA);
  adc_T >>= 4;

  var1 = ((((adc_T>>3) - ((int32_t)_bme280_calib.dig_T1 <<1))) * ((int32_t)_bme280_calib.dig_T2)) >> 11;
  var2 = (((((adc_T>>4) - ((int32_t)_bme280_calib.dig_T1)) * ((adc_T>>4) - ((int32_t)_bme280_calib.dig_T1))) >> 12) *
    ((int32_t)_bme280_calib.dig_T3)) >> 14;
  t_fine = var1 + var2;
  double T  = (t_fine * 5 + 128) >> 8;
  return T / 100.0;
}

double bmp280_readPressure(void)
{
  int64_t var1, var2, p;

// Must be done first to get the t_fine variable set up
//  bmp280_readTemperature();

  int32_t adc_P = i2c_read24(bmpaddr, BME280_REGISTER_PRESSUREDATA);
  adc_P >>= 4;

  var1 = ((int64_t)t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)_bme280_calib.dig_P6;
  var2 = var2 + ((var1 * (int64_t)_bme280_calib.dig_P5) << 17);
  var2 = var2 + (((int64_t)_bme280_calib.dig_P4) << 35);
  var1 = ((var1 * var1 * (int64_t)_bme280_calib.dig_P3) >> 8) + ((var1 * (int64_t)_bme280_calib.dig_P2) << 12);
  var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)_bme280_calib.dig_P1) >> 33;
  if (var1 == 0) {
    return 0;  // avoid exception caused by division by zero
  }
  p = 1048576 - adc_P;
  p = (((p << 31) - var2) * 3125) / var1;
  var1 = (((int64_t)_bme280_calib.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
  var2 = (((int64_t)_bme280_calib.dig_P8) * p) >> 19;
  p = ((p + var1 + var2) >> 8) + (((int64_t)_bme280_calib.dig_P7) << 4);
  return (double)p / 25600.0;
}

double bme280_readHumidity(void)
{
  int32_t v_x1_u32r;

// Must be done first to get the t_fine variable set up
//  bmp280_readTemperature();

  int32_t adc_H = i2c_read16(bmpaddr, BME280_REGISTER_HUMIDDATA);

  v_x1_u32r = (t_fine - ((int32_t)76800));

  v_x1_u32r = (((((adc_H << 14) - (((int32_t)_bme280_calib.dig_H4) << 20) -
    (((int32_t)_bme280_calib.dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *
    (((((((v_x1_u32r * ((int32_t)_bme280_calib.dig_H6)) >> 10) *
    (((v_x1_u32r * ((int32_t)_bme280_calib.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) +
    ((int32_t)2097152)) * ((int32_t)_bme280_calib.dig_H2) + 8192) >> 14));
  v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
    ((int32_t)_bme280_calib.dig_H1)) >> 4));
  v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
  v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
  double h = (v_x1_u32r >> 12);
  return  h / 1024.0;
}

/*********************************************************************************************\
 * BMP
\*********************************************************************************************/

double bmp_convertCtoF(double c)
{
  return c * 1.8 + 32;
}

double bmp_readTemperature(bool S)
{
  double t = NAN;
  
  switch (bmptype) {
  case BMP180_CHIPID:
    t = bmp180_readTemperature();
    break;
  case BMP280_CHIPID:
  case BME280_CHIPID:
    t = bmp280_readTemperature();
  }
  if (!isnan(t)) {
    if(S) t = bmp_convertCtoF(t);
    return t;
  }
  return 0;
}

double bmp_readPressure(void)
{
  switch (bmptype) {
  case BMP180_CHIPID:
    return bmp180_readPressure();
  case BMP280_CHIPID:
  case BME280_CHIPID:
    return bmp280_readPressure();
  }
  return 0;
}

double bmp_readHumidity(void)
{
  switch (bmptype) {
  case BMP180_CHIPID:
  case BMP280_CHIPID:
    break;
  case BME280_CHIPID:
    return bme280_readHumidity();
  }
  return 0;
}

boolean bmp_detect()
{
  if (bmptype) return true;

  char log[LOGSZ];
  boolean success = false;

  bmpaddr = BMP_ADDR;
  bmptype = i2c_read8(bmpaddr, BMP_REGISTER_CHIPID);
  if (!bmptype) {
    bmpaddr--;
    bmptype = i2c_read8(bmpaddr, BMP_REGISTER_CHIPID);
  }
  snprintf_P(bmpstype, sizeof(bmpstype), PSTR("BMP"));
  switch (bmptype) {
  case BMP180_CHIPID:
    success = bmp180_calibration();
    snprintf_P(bmpstype, sizeof(bmpstype), PSTR("BMP180"));
    break;
  case BMP280_CHIPID:
    success = bmp280_calibrate();
    snprintf_P(bmpstype, sizeof(bmpstype), PSTR("BMP280"));
    break;
  case BME280_CHIPID:
    success = bme280_calibrate();
    snprintf_P(bmpstype, sizeof(bmpstype), PSTR("BME280"));
  }
  if (success) {
    snprintf_P(log, sizeof(log), PSTR("I2C: %s found at address 0x%x"), bmpstype, bmpaddr);
    addLog(LOG_LEVEL_DEBUG, log);
  } else {
    bmptype = 0;
  }
  return success;
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void bmp_mqttPresent(char* svalue, uint16_t ssvalue, uint8_t* djson)
{
  if (!bmptype) return;

  char stemp1[10], stemp2[10], stemp3[10];

  double t_bmp = bmp_readTemperature(TEMP_CONVERSION);
  double p_bmp = bmp_readPressure();
  double h_bmp = bmp_readHumidity();
  dtostrf(t_bmp, 1, TEMP_RESOLUTION &3, stemp1);
  dtostrf(p_bmp, 1, PRESSURE_RESOLUTION &3, stemp2);
  dtostrf(h_bmp, 1, HUMIDITY_RESOLUTION &3, stemp3);
  if (!strcmp(bmpstype,"BME280")) {
    snprintf_P(svalue, ssvalue, PSTR("%s, \"%s\":{\"Temperature\":\"%s\", \"Humidity\":\"%s\", \"Pressure\":\"%s\"}"),
      svalue, bmpstype, stemp1, stemp3, stemp2);
  } else {
    snprintf_P(svalue, ssvalue, PSTR("%s, \"%s\":{\"Temperature\":\"%s\", \"Pressure\":\"%s\"}"),
      svalue, bmpstype, stemp1, stemp2);
  }
  *djson = 1;
}

#ifdef USE_WEBSERVER
String bmp_webPresent()
{
  String page = "";
  if (bmptype) {
    char itemp[10], iconv[10];

    snprintf_P(iconv, sizeof(iconv), PSTR("&deg;%c"), (TEMP_CONVERSION) ? 'F' : 'C');
    double t_bmp = bmp_readTemperature(TEMP_CONVERSION);
    double p_bmp = bmp_readPressure();
    double h_bmp = bmp_readHumidity();
    dtostrf(t_bmp, 1, TEMP_RESOLUTION &3, itemp);
    page += F("<tr><td>BMP Temperature: </td><td>"); page += itemp; page += iconv; page += F("</td></tr>");
    if (!strcmp(bmpstype,"BME280")) {
      dtostrf(h_bmp, 1, HUMIDITY_RESOLUTION &3, itemp);
      page += F("<tr><td>BMP Humidity: </td><td>"); page += itemp; page += F("%</td></tr>");
    }
    dtostrf(p_bmp, 1, PRESSURE_RESOLUTION &3, itemp);
    page += F("<tr><td>BMP Pressure: </td><td>"); page += itemp; page += F(" hPa</td></tr>");
  }
  return page;
}
#endif  // USE_WEBSERVER
#endif  // USE_BMP
#endif  // USE_I2C

