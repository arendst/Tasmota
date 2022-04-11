/*
  xsns_97_sht4x.ino - SHT4X temperature and humidity sensor support for Tasmota

  Copyright (C) 2022  Stefan Tibus and Theo Arends

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
#ifdef USE_SHT4X
/*********************************************************************************************\
 * SHT4X - Temperature and Humidity
 *
 * I2C Address: 0x44, 0x45
\*********************************************************************************************/

#define XSNS_97             97
#define XI2C_68             68         // See I2CDEVICES.md

#define SHT4X_ADDR_A        0x44       // address SHT4x A
#define SHT4X_ADDR_B        0x45       // address SHT4x B

#define SHT4X_MAX_SENSORS   2

const char kSht4Types[] PROGMEM = "SHT4X|SHT4X";
uint8_t sht4x_addresses[] = { SHT4X_ADDR_A, SHT4X_ADDR_B };

uint8_t sht4x_count = 0;
struct SHT4XSTRUCT {
  uint8_t address;    // I2C bus address
  char types[6];      // Sensor type name and address - "SHT4X-0xXX"
} sht4x_sensors[SHT4X_MAX_SENSORS];

bool Sht4xRead(float &t, float &h, uint8_t sht4x_address)
{
  unsigned int data[6];

  t = NAN;
  h = NAN;

  Wire.beginTransmission(sht4x_address);
  Wire.write(0xFD);                  // High repeatability
  if (Wire.endTransmission() != 0) {   // Stop I2C transmission
    return false;
  }
  delay(10);                           // Datasheet: 8.3 ms, Sensirion code: 10 ms
  Wire.requestFrom(sht4x_address, (uint8_t)6);   // Request 6 bytes of data
  for (uint32_t i = 0; i < 6; i++) {
    data[i] = Wire.read();             // cTemp msb, cTemp lsb, cTemp crc, humidity msb, humidity lsb, humidity crc
  };
  t = ConvertTemp((float)((((data[0] << 8) | data[1]) * 175) / 65535.0) - 45);
  h = ConvertHumidity((float)((((data[3] << 8) | data[4]) * 125) / 65535.0) - 6);
  return (!isnan(t) && !isnan(h) && (h != 0));
}

/********************************************************************************************/

void Sht4xDetect(void)
{
  for (uint32_t i = 0; i < SHT4X_MAX_SENSORS; i++) {
    if (!I2cSetDevice(sht4x_addresses[i])) { continue; }
    float t;
    float h;
    if (Sht4xRead(t, h, sht4x_addresses[i])) {
      sht4x_sensors[sht4x_count].address = sht4x_addresses[i];
      GetTextIndexed(sht4x_sensors[sht4x_count].types, sizeof(sht4x_sensors[sht4x_count].types), i, kSht4Types);
      I2cSetActiveFound(sht4x_sensors[sht4x_count].address, sht4x_sensors[sht4x_count].types);
      sht4x_count++;
    }
  }
}

void Sht4xShow(bool json)
{
  for (uint32_t i = 0; i < sht4x_count; i++) {
    float t;
    float h;
    if (Sht4xRead(t, h, sht4x_sensors[i].address)) {
      char types[11];
      strlcpy(types, sht4x_sensors[i].types, sizeof(types));
      if (sht4x_count > 1) {
        snprintf_P(types, sizeof(types), PSTR("%s%c%02X"), sht4x_sensors[i].types, IndexSeparator(), sht4x_sensors[i].address);  // "SHT4X-0xXX"
      }
      TempHumDewShow(json, ((0 == TasmotaGlobal.tele_period) && (0 == i)), types, t, h);
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns97(uint8_t function)
{
  if (!I2cEnabled(XI2C_68)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Sht4xDetect();
  }
  else if (sht4x_count) {
    switch (function) {
      case FUNC_JSON_APPEND:
        Sht4xShow(1);
        break;
  #ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Sht4xShow(0);
        break;
  #endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_SHT4X
#endif  // USE_I2C
