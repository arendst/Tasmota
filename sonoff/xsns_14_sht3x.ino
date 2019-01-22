/*
  xsns_14_sht3x.ino - SHT3X temperature and humidity sensor support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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
#ifdef USE_SHT3X
/*********************************************************************************************\
 * SHT3X and SHTC3 - Temperature and Humidity
 *
 * I2C Address: 0x44, 0x45 or 0x70 (SHTC3)
\*********************************************************************************************/

#define XSNS_14             14

#define SHT3X_ADDR_GND      0x44       // address pin low (GND)
#define SHT3X_ADDR_VDD      0x45       // address pin high (VDD)
#define SHTC3_ADDR          0x70       // address for shtc3 sensor

#define SHT3X_MAX_SENSORS   3

const char kShtTypes[] PROGMEM = "SHT3X|SHT3X|SHTC3";
uint8_t sht3x_addresses[] = { SHT3X_ADDR_GND, SHT3X_ADDR_VDD, SHTC3_ADDR };

uint8_t sht3x_count = 0;
struct SHT3XSTRUCT {
  uint8_t address;    // I2C bus address
  char types[6];      // Sensor type name and address - "SHT3X-0xXX"
} sht3x_sensors[SHT3X_MAX_SENSORS];

bool Sht3xRead(float &t, float &h, uint8_t sht3x_address)
{
  unsigned int data[6];

  t = NAN;
  h = NAN;

  Wire.beginTransmission(sht3x_address);
  if (SHTC3_ADDR == sht3x_address) {
    Wire.write(0x35);                  // Wake from
    Wire.write(0x17);                  // sleep
    Wire.endTransmission();
    Wire.beginTransmission(sht3x_address);
    Wire.write(0x78);                  // Disable clock stretching ( I don't think that wire library support clock stretching )
    Wire.write(0x66);                  // High resolution
  } else {
    Wire.write(0x2C);                  // Enable clock stretching
    Wire.write(0x06);                  // High repeatability
  }
  if (Wire.endTransmission() != 0) {   // Stop I2C transmission
    return false;
  }
  delay(30);                           // Timing verified with logic analyzer (10 is to short)
  Wire.requestFrom(sht3x_address, (uint8_t)6);   // Request 6 bytes of data
  for (int i = 0; i < 6; i++) {
    data[i] = Wire.read();             // cTemp msb, cTemp lsb, cTemp crc, humidity msb, humidity lsb, humidity crc
  };
  t = ConvertTemp((float)((((data[0] << 8) | data[1]) * 175) / 65535.0) - 45);
  h = (float)((((data[3] << 8) | data[4]) * 100) / 65535.0);
  return (!isnan(t) && !isnan(h));
}

/********************************************************************************************/

void Sht3xDetect(void)
{
  if (sht3x_count) return;

  float t;
  float h;
  for (byte i = 0; i < SHT3X_MAX_SENSORS; i++) {
    if (Sht3xRead(t, h, sht3x_addresses[i])) {
      sht3x_sensors[sht3x_count].address = sht3x_addresses[i];
      GetTextIndexed(sht3x_sensors[sht3x_count].types, sizeof(sht3x_sensors[sht3x_count].types), i, kShtTypes);
      snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, sht3x_sensors[sht3x_count].types, sht3x_sensors[sht3x_count].address);
      AddLog(LOG_LEVEL_DEBUG);
      sht3x_count++;
    }
  }
}

void Sht3xShow(boolean json)
{
  if (sht3x_count) {
    float t;
    float h;
    char types[11];
    for (byte i = 0; i < sht3x_count; i++) {
      if (Sht3xRead(t, h, sht3x_sensors[i].address)) {

        if (0 == i) { SetGlobalValues(t, h); }

        char temperature[33];
        dtostrfd(t, Settings.flag2.temperature_resolution, temperature);
        char humidity[33];
        dtostrfd(h, Settings.flag2.humidity_resolution, humidity);
        snprintf_P(types, sizeof(types), PSTR("%s-0x%02X"), sht3x_sensors[i].types, sht3x_sensors[i].address);  // "SHT3X-0xXX"

        if (json) {
          snprintf_P(mqtt_data, sizeof(mqtt_data), JSON_SNS_TEMPHUM, mqtt_data, types, temperature, humidity);
#ifdef USE_DOMOTICZ
          if ((0 == tele_period) && (0 == i)) {  // We want the same first sensor to report to Domoticz in case a read is missed
            DomoticzTempHumSensor(temperature, humidity);
          }
#endif  // USE_DOMOTICZ

#ifdef USE_KNX
        if (0 == tele_period) {
          KnxSensor(KNX_TEMPERATURE, t);
          KnxSensor(KNX_HUMIDITY, h);
        }
#endif  // USE_KNX

#ifdef USE_WEBSERVER
        } else {
          snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TEMP, mqtt_data, types, temperature, TempUnit());
          snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_HUM, mqtt_data, types, humidity);
#endif  // USE_WEBSERVER
        }
      }
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xsns14(byte function)
{
  boolean result = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_INIT:
        Sht3xDetect();
        break;
      case FUNC_JSON_APPEND:
        Sht3xShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        Sht3xShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_SHT3X
#endif  // USE_I2C
