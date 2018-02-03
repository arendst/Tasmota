/*
  xsns_08_htu21.ino - HTU21 temperature and humidity sensor support for Sonoff-Tasmota

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
#ifdef USE_HTU
/*********************************************************************************************\
 * HTU21 - Temperature and Humidy
 *
 * Source: Heiko Krupp
 *
 * I2C Address: 0x40
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

const char kHtuTypes[] PROGMEM = "HTU21|SI7013|SI7020|SI7021|T/RH?";

uint8_t htu_address;
uint8_t htu_type = 0;
uint8_t delay_temp;
uint8_t delay_humidity = 50;
char htu_types[7];

uint8_t HtuCheckCrc8(uint16_t data)
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

uint8_t HtuReadDeviceId(void)
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
  if (HtuCheckCrc8(deviceID) == checksum) {
    deviceID = deviceID >> 8;
  } else {
    deviceID = 0;
  }
  return (uint8_t)deviceID;
}

void HtuSetResolution(uint8_t resolution)
{
  uint8_t current = I2cRead8(HTU21_ADDR, HTU21_READREG);
  current &= 0x7E;          // Replace current resolution bits with 0
  current |= resolution;    // Add new resolution bits to register
  I2cWrite8(HTU21_ADDR, HTU21_WRITEREG, current);
}

void HtuReset(void)
{
  Wire.beginTransmission(HTU21_ADDR);
  Wire.write(HTU21_RESET);
  Wire.endTransmission();
  delay(15);                // Reset takes 15ms
}

void HtuHeater(uint8_t heater)
{
  uint8_t current = I2cRead8(HTU21_ADDR, HTU21_READREG);

  switch(heater)
  {
    case HTU21_HEATER_ON  : current |= heater;
                            break;
    case HTU21_HEATER_OFF : current &= heater;
                            break;
    default               : current &= heater;
                            break;
  }
  I2cWrite8(HTU21_ADDR, HTU21_WRITEREG, current);
}

void HtuInit()
{
  HtuReset();
  HtuHeater(HTU21_HEATER_OFF);
  HtuSetResolution(HTU21_RES_RH12_T14);
}

float HtuReadHumidity(void)
{
  uint8_t  checksum = 0;
  uint16_t sensorval = 0;
  float    humidity = 0.0;

  Wire.beginTransmission(HTU21_ADDR);
  Wire.write(HTU21_READHUM);
  if (Wire.endTransmission() != 0) {
    return 0.0; // In case of error
  }
  delay(delay_humidity);                              // Sensor time at max resolution

  Wire.requestFrom(HTU21_ADDR, 3);
  if (3 <= Wire.available()) {
    sensorval = Wire.read() << 8;             // MSB
    sensorval |= Wire.read();                 // LSB
    checksum = Wire.read();
  }
  if (HtuCheckCrc8(sensorval) != checksum) {
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

float HtuReadTemperature()
{
  uint8_t  checksum=0;
  uint16_t sensorval=0;
  float t;

  Wire.beginTransmission(HTU21_ADDR);
  Wire.write(HTU21_READTEMP);
  if (Wire.endTransmission() != 0) {
    return 0.0; // In case of error
  }
  delay(delay_temp);                          // Sensor time at max resolution

  Wire.requestFrom(HTU21_ADDR, 3);
  if (3 == Wire.available()) {
    sensorval = Wire.read() << 8;         // MSB
    sensorval |= Wire.read();             // LSB
    checksum = Wire.read();
  }
  if (HtuCheckCrc8(sensorval) != checksum) {
    return 0.0; // Checksum mismatch
  }

  t = ConvertTemp(0.002681 * (float)sensorval - 46.85);
  return t;
}

float HtuCompensatedHumidity(float humidity, float temperature)
{
  if(humidity == 0.00 && temperature == 0.00) {
    return 0.0;
  }
  if(temperature > 0.00 && temperature < 80.00) {
    return (-0.15)*(25-temperature)+humidity;
  }
  return humidity;
}

/********************************************************************************************/

void HtuDetect()
{
  if (htu_type) {
    return;
  }

  htu_address = HTU21_ADDR;
  htu_type = HtuReadDeviceId();
  if (htu_type) {
    uint8_t index = 0;
    HtuInit();
    switch (htu_type) {
      case HTU21_CHIPID:
        delay_temp = 50;
        delay_humidity = 16;
        break;
      case SI7021_CHIPID:
        index++;  // 3
      case SI7020_CHIPID:
        index++;  // 2
      case SI7013_CHIPID:
        index++;  // 1
        delay_temp = 12;
        delay_humidity = 23;
        break;
      default:
        index = 4;
        delay_temp = 50;
        delay_humidity = 23;
    }
    GetTextIndexed(htu_types, sizeof(htu_types), index, kHtuTypes);
    snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, htu_types, htu_address);
    AddLog(LOG_LEVEL_DEBUG);
  }
}

void HtuShow(boolean json)
{
  if (htu_type) {
    char temperature[10];
    char humidity[10];

    float t = HtuReadTemperature();
    float h = HtuReadHumidity();
    h = HtuCompensatedHumidity(h, t);
    dtostrfd(t, Settings.flag2.temperature_resolution, temperature);
    dtostrfd(h, Settings.flag2.humidity_resolution, humidity);

    if (json) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), JSON_SNS_TEMPHUM, mqtt_data, htu_types, temperature, humidity);
#ifdef USE_DOMOTICZ
      DomoticzTempHumSensor(temperature, humidity);
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TEMP, mqtt_data, htu_types, temperature, TempUnit());
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_HUM, mqtt_data, htu_types, humidity);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_08

boolean Xsns08(byte function)
{
  boolean result = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_PREP_BEFORE_TELEPERIOD:
        HtuDetect();
        break;
      case FUNC_JSON_APPEND:
        HtuShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        HtuShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_HTU
#endif  // USE_I2C

