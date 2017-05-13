/*
  xsns_htu21.ino - HTU21 temperature and humidity sensor support for Sonoff-Tasmota

  Copyright (C) 2017  Heiko Krupp and Theo Arends

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
#ifdef USE_HTU
/*********************************************************************************************\
 * HTU21 - Temperature and Humidy
 *
 * Source: Heiko Krupp
\*********************************************************************************************/

#define HTU21_ADDR          0x40

#define SI7013_CHIPID       0x0D
#define SI7020_CHIPID       0x14
#define SI7021_CHIPID       0x15
#define HTU21_CHIPID        0x32

#define HTU21_READTEMP      0xE3
#define HTU21_READHUM       0xE5
#define HTU21_WRITEREG      0xE6
#define HTU21_READREG       0xE7
#define HTU21_RESET         0xFE
#define HTU21_HEATER_WRITE  0x51
#define HTU21_HEATER_READ   0x11
#define HTU21_SERIAL2_READ1 0xFC    /* Read 3rd two Serial bytes */
#define HTU21_SERIAL2_READ2 0xC9    /* Read 4th two Serial bytes */

#define HTU21_HEATER_ON     0x04
#define HTU21_HEATER_OFF    0xFB

#define HTU21_RES_RH12_T14  0x00  // Default
#define HTU21_RES_RH8_T12   0x01
#define HTU21_RES_RH10_T13  0x80
#define HTU21_RES_RH11_T11  0x81

#define HTU21_CRC8_POLYNOM  0x13100

uint8_t htuaddr;
uint8_t htutype = 0;
uint8_t delayT;
uint8_t delayH = 50;
char htustype[7];

uint8_t check_crc8(uint16_t data)
{
  for (uint8_t bit = 0; bit < 16; bit++) {
    if (data & 0x8000) {
      data =  (data << 1) ^ HTU21_CRC8_POLYNOM;
    } else {
      data <<= 1;
    }
  }
  return data >>= 8;
}

uint8_t htu21_readDeviceID(void)
{
  uint16_t deviceID = 0;
  uint8_t checksum = 0;

  Wire.beginTransmission(HTU21_ADDR);
  Wire.write(HTU21_SERIAL2_READ1);
  Wire.write(HTU21_SERIAL2_READ2);
  Wire.endTransmission();

  Wire.requestFrom(HTU21_ADDR, 3);
  deviceID  = Wire.read() << 8;
  deviceID |= Wire.read();
  checksum  = Wire.read();
  if (check_crc8(deviceID) == checksum) {
    deviceID = deviceID >> 8;
  } else {
    deviceID = 0;
  }
  return (uint8_t)deviceID;
}

void htu21_setRes(uint8_t resolution)
{
  uint8_t current = i2c_read8(HTU21_ADDR, HTU21_READREG);
  current &= 0x7E;                  // Replace current resolution bits with 0
  current |= resolution;            // Add new resolution bits to register
  i2c_write8(HTU21_ADDR, HTU21_WRITEREG, current);
}

void htu21_reset(void)
{
  Wire.beginTransmission(HTU21_ADDR);
  Wire.write(HTU21_RESET);
  Wire.endTransmission();
  delay(15);                // Reset takes 15ms
}

void htu21_heater(uint8_t heater)
{
  uint8_t current = i2c_read8(HTU21_ADDR, HTU21_READREG);

  switch(heater)
  {
    case HTU21_HEATER_ON  : current |= heater;
                            break;
    case HTU21_HEATER_OFF : current &= heater;
                            break;
    default               : current &= heater;
                            break;
  }
  i2c_write8(HTU21_ADDR, HTU21_WRITEREG, current);
}

boolean htu21_init()
{
  htu21_reset();
  htu21_heater(HTU21_HEATER_OFF);
  htu21_setRes(HTU21_RES_RH12_T14);
  return true;
}

float htu21_readHumidity(void)
{
  uint8_t  checksum = 0;
  uint16_t sensorval = 0;
  float    humidity = 0.0;

  Wire.beginTransmission(HTU21_ADDR);
  Wire.write(HTU21_READHUM);
  if (Wire.endTransmission() != 0) {
    return 0.0; // In case of error
  }
  delay(delayH);                              // Sensor time at max resolution

  Wire.requestFrom(HTU21_ADDR, 3);
  if (3 <= Wire.available()) {
    sensorval = Wire.read() << 8;             // MSB
    sensorval |= Wire.read();                 // LSB
    checksum = Wire.read();
  }
  if (check_crc8(sensorval) != checksum) {
    return 0.0; // Checksum mismatch
  }

  sensorval ^= 0x02;      // clear status bits
  humidity = 0.001907 * (float)sensorval - 6;

  if (humidity > 100) {
    return 100.0;
  }
  if (humidity < 0) {
    return 0.01;
  }

  return humidity;
}

float htu21_readTemperature()
{
  uint8_t  checksum=0;
  uint16_t sensorval=0;
  float t;

  Wire.beginTransmission(HTU21_ADDR);
  Wire.write(HTU21_READTEMP);
  if (Wire.endTransmission() != 0) {
    return 0.0; // In case of error
  }
  delay(delayT);                          // Sensor time at max resolution

  Wire.requestFrom(HTU21_ADDR, 3);
  if (3 == Wire.available()) {
    sensorval = Wire.read() << 8;         // MSB
    sensorval |= Wire.read();             // LSB
    checksum = Wire.read();
  }
  if (check_crc8(sensorval) != checksum) {
    return 0.0; // Checksum mismatch
  }

  t = convertTemp(0.002681 * (float)sensorval - 46.85);
  return t;
}

float htu21_compensatedHumidity(float humidity, float temperature)
{
  if(humidity == 0.00 && temperature == 0.00) {
    return 0.0;
  }
  if(temperature > 0.00 && temperature < 80.00) {
    return (-0.15)*(25-temperature)+humidity;
  }
}

uint8_t htu_detect()
{
  if (htutype) {
    return true;
  }

  char log[LOGSZ];
  boolean success = false;

  htuaddr = HTU21_ADDR;
  htutype = htu21_readDeviceID();
  success = htu21_init();
  switch (htutype) {
  case HTU21_CHIPID:
    strcpy_P(htustype, PSTR("HTU21"));
    delayT=50;
    delayH=16;
    break;
  case SI7013_CHIPID:
    strcpy_P(htustype, PSTR("SI7013"));
    delayT=12;
    delayH=23;
    break;
  case SI7020_CHIPID:
    strcpy_P(htustype, PSTR("SI7020"));
    delayT=12;
    delayH=23;
    break;
  case SI7021_CHIPID:
    strcpy_P(htustype, PSTR("SI7021"));
    delayT=12;
    delayH=23;
    break;
  default:
    strcpy_P(htustype, PSTR("T/RH?"));
    delayT=50;
    delayH=23;
  }
  if (success) {
    snprintf_P(log, sizeof(log), PSTR("I2C: %s found at address 0x%x"), htustype, htuaddr);
    addLog(LOG_LEVEL_DEBUG, log);
  } else {
    htutype = 0;
  }
  return success;
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void htu_mqttPresent(char* svalue, uint16_t ssvalue, uint8_t* djson)
{
  if (!htutype) {
    return;
  }

  char stemp1[10];
  char stemp2[10];

  float t = htu21_readTemperature();
  float h = htu21_readHumidity();
  h = htu21_compensatedHumidity(h, t);
  dtostrf(t, 1, sysCfg.flag.temperature_resolution, stemp1);
  dtostrf(h, 1, sysCfg.flag.humidity_resolution, stemp2);
  snprintf_P(svalue, ssvalue, JSON_SNS_TEMPHUM, svalue, htustype, stemp1, stemp2);
  *djson = 1;
#ifdef USE_DOMOTICZ
  domoticz_sensor2(stemp1, stemp2);
#endif  // USE_DOMOTICZ
}

#ifdef USE_WEBSERVER
String htu_webPresent()
{
  String page = "";
  if (htutype) {
    char stemp[10];
    char sensor[80];

    float t_htu21 = htu21_readTemperature();
    float h_htu21 = htu21_readHumidity();
    h_htu21 = htu21_compensatedHumidity(h_htu21, t_htu21);
    dtostrf(t_htu21, 1, sysCfg.flag.temperature_resolution, stemp);
    snprintf_P(sensor, sizeof(sensor), HTTP_SNS_TEMP, htustype, stemp, tempUnit());
    page += sensor;
    dtostrf(h_htu21, 1, sysCfg.flag.humidity_resolution, stemp);
    snprintf_P(sensor, sizeof(sensor), HTTP_SNS_HUM, htustype, stemp);
    page += sensor;
  }
  return page;
}
#endif  // USE_WEBSERVER
#endif  // USE_HTU
#endif  // USE_I2C

