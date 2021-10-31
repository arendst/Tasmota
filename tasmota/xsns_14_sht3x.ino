/*
  xsns_14_sht3x.ino - SHT3X temperature and humidity sensor support for Tasmota

  Copyright (C) 2021  Theo Arends

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
#define XI2C_15             15         // See I2CDEVICES.md

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
  for (uint32_t i = 0; i < 6; i++) {
    data[i] = Wire.read();             // cTemp msb, cTemp lsb, cTemp crc, humidity msb, humidity lsb, humidity crc
  };
  t = ConvertTemp((float)((((data[0] << 8) | data[1]) * 175) / 65535.0) - 45);
  h = ConvertHumidity((float)((((data[3] << 8) | data[4]) * 100) / 65535.0));
  return (!isnan(t) && !isnan(h) && (h != 0));
}

/********************************************************************************************/

void Sht3xDetect(void)
{
  for (uint32_t i = 0; i < SHT3X_MAX_SENSORS; i++) {
    if (!I2cSetDevice(sht3x_addresses[i])) { continue; }
    float t;
    float h;
    if (Sht3xRead(t, h, sht3x_addresses[i])) {
      sht3x_sensors[sht3x_count].address = sht3x_addresses[i];
      GetTextIndexed(sht3x_sensors[sht3x_count].types, sizeof(sht3x_sensors[sht3x_count].types), i, kShtTypes);
      I2cSetActiveFound(sht3x_sensors[sht3x_count].address, sht3x_sensors[sht3x_count].types);
      sht3x_count++;
    }
  }
}

void Sht3xShow(bool json)
{
  for (uint32_t i = 0; i < sht3x_count; i++) {
    float t;
    float h;
    if (Sht3xRead(t, h, sht3x_sensors[i].address)) {
      char types[11];
      strlcpy(types, sht3x_sensors[i].types, sizeof(types));
      if (sht3x_count > 1) {
        snprintf_P(types, sizeof(types), PSTR("%s%c%02X"), sht3x_sensors[i].types, IndexSeparator(), sht3x_sensors[i].address);  // "SHT3X-0xXX"
      }
      TempHumDewShow(json, ((0 == TasmotaGlobal.tele_period) && (0 == i)), types, t, h);
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns14(uint8_t function)
{
  if (!I2cEnabled(XI2C_15)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Sht3xDetect();
  }
  else if (sht3x_count) {
    switch (function) {
      case FUNC_JSON_APPEND:
        Sht3xShow(1);
        break;
  #ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Sht3xShow(0);
        break;
  #endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_SHT3X
#endif  // USE_I2C
