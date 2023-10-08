/*
  xsns_14_sht3x.ino - SHT3X, SHT4X and SHTCX temperature and humidity sensor support for Tasmota

  Copyright (C) 2022  Theo Arends, Stefan Tibus

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
 * Sensirion I2C temperature and humidity sensors
 *
 * This driver supports the following sensors:
 * - SHT3x series: SHT30, SHT31, SHT35 (addresses: A: 0x44, B: 0x45)
 * - SHTC series:  SHTC1, SHTC3 (address: 0x70)
 * - SHT4x series: SHT40, SHT41, SHT45 (addresses: A: 0x44, B: 0x45)
\*********************************************************************************************/

#define XSNS_14             14
#define XI2C_15             15         // See I2CDEVICES.md

#define SHT3X_TYPES         3          // SHT3X, SHTCX and SHT4X
#define SHT3X_ADDRESSES     3          // 0x44, 0x45 and 0x70

enum SHT3X_Types {
  SHT3X_TYPE_SHT3X,
  SHT3X_TYPE_SHTCX,
  SHT3X_TYPE_SHT4X
};
const char kSht3xTypes[] PROGMEM = "SHT3X|SHTC3|SHT4X";

uint8_t sht3x_addresses[] = { 0x44, 0x45, 0x70 };

uint8_t sht3x_count = 0;
struct SHT3XSTRUCT {
  uint8_t type;        // Sensor type
  uint8_t address;     // I2C bus address
  char types[6];  // Sensor type name and address, e.g. "SHT3X"
} sht3x_sensors[SHT3X_ADDRESSES];

uint8_t Sht3xComputeCrc(uint8_t data[], uint8_t len) {
  // Compute CRC as per datasheet
  uint8_t crc = 0xFF;

  for (uint8_t x = 0; x < len; x++) {
    crc ^= data[x];
    for (uint8_t i = 0; i < 8; i++) {
      if (crc & 0x80) {
        crc = (crc << 1) ^ 0x31;
      } else {
        crc <<= 1;
      }
    }
  }
  return crc;
}

bool Sht3xRead(uint32_t type, float &t, float &h, uint8_t i2c_address) {
  uint8_t data[6];

  t = NAN;
  h = NAN;

  Wire.beginTransmission(i2c_address);
  switch (type) {
    case SHT3X_TYPE_SHT3X:
      // TODO: Clock stretching is used for SHT3x but not for SHTC3. Why?
      Wire.write(0x2C);                     // Enable clock stretching
      Wire.write(0x06);                     // High repeatability measurement
      break;
    case SHT3X_TYPE_SHTCX:
      Wire.write(0x35);                     // Wake from
      Wire.write(0x17);                     // sleep
      Wire.endTransmission();
      Wire.beginTransmission(i2c_address);
      // TODO: Clock stretching is used for SHT3x but not for SHTC3. Why?
      Wire.write(0x78);                     // Disable clock stretching
      Wire.write(0x66);                     // Normal mode measurement
      break;
    case SHT3X_TYPE_SHT4X:
      Wire.write(0xFD);                     // High repeatability measurement
      break;
  }
  if (Wire.endTransmission() != 0) {        // Stop I2C transmission
    return false;
  }
  delay(30);                                // Timing verified with logic analyzer (10 is to short)
  Wire.requestFrom(i2c_address, (uint8_t)6); // Request 6 bytes of data
  for (uint32_t i = 0; i < 6; i++) {
    data[i] = Wire.read();                  // temperature (MSB, LSB, CRC), humidity (MSB, LSB, CRC)
  };
  if ((Sht3xComputeCrc(&data[0], 2) != data[2]) || (Sht3xComputeCrc(&data[3], 2) != data[5])) {
    return false;
  }
  t = ((float)(((data[0] << 8) | data[1]) * 175) / 65535.0) - 45.0;
  if (type == SHT3X_TYPE_SHT4X) {
    h = ((float)(((data[3] << 8) | data[4]) * 125) / 65535.0) - 6.0;
  } else {
    h = ((float)(((data[3] << 8) | data[4]) * 100) / 65535.0);
  }
  return (!isnan(t) && !isnan(h));
}

/********************************************************************************************/

void Sht3xDetect(void) {
  float t;
  float h;

  for (uint32_t k = 0; k < SHT3X_TYPES; k++) {
    sht3x_sensors[sht3x_count].type = k;
    for (uint32_t i = 0; i < SHT3X_ADDRESSES; i++) {
      if (!I2cSetDevice(sht3x_addresses[i])) { continue; }
      sht3x_sensors[sht3x_count].address = sht3x_addresses[i];
      if (Sht3xRead(sht3x_sensors[sht3x_count].type, t, h, sht3x_sensors[sht3x_count].address)) {
        GetTextIndexed(sht3x_sensors[sht3x_count].types, sizeof(sht3x_sensors[sht3x_count].types), sht3x_sensors[sht3x_count].type, kSht3xTypes);
        I2cSetActiveFound(sht3x_sensors[sht3x_count].address, sht3x_sensors[sht3x_count].types);
        sht3x_count++;
      }
    }
  }
}

void Sht3xShow(bool json) {
  float t;
  float h;
  char types[11];

  for (uint32_t i = 0; i < sht3x_count; i++) {
    if (Sht3xRead(sht3x_sensors[i].type, t, h, sht3x_sensors[i].address)) {
      t = ConvertTemp(t);
      h = ConvertHumidity(h);
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

bool Xsns14(uint32_t function) {
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
