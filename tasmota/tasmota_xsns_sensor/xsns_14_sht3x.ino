/*
  xsns_14_sht3x.ino - SHT3X, SHT4X and SHTCX temperature and humidity sensor support for Tasmota

  Copyright (C) 2024  Theo Arends, Stefan Tibus, Jan-David Förster

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
 * - SHT4x series: SHT40, SHT41, SHT45 (addresses: A: 0x44, B: 0x45, C: 0x46)
\*********************************************************************************************/

#define XSNS_14             14
#define XI2C_15             15         // See I2CDEVICES.md

#define SHT3X_TYPES         3          // SHT3X, SHTCX and SHT4X
#define SHT3X_ADDRESSES     4          // 0x44, 0x45, 0x46 and 0x70

enum SHT3X_Types { SHT3X_TYPE_SHT3X, SHT3X_TYPE_SHTCX, SHT3X_TYPE_SHT4X };
const char kSht3xTypes[] PROGMEM = "SHT3X|SHTC3|SHT4X";

uint8_t sht3x_addresses[] = { 0x44, 0x45, 0x46, 0x70 };

uint8_t sht3x_count = 0;
struct SHT3XSTRUCT {
  float   humi = NAN;
  float   temp = NAN;
  uint8_t valid = 0;
  uint8_t type;        // Sensor type
  uint8_t address;     // I2C bus address
  uint8_t bus;
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

bool Sht3xRead(uint32_t sensor) {
  if (sht3x_sensors[sensor].valid) { sht3x_sensors[sensor].valid--; }
/*
  TwoWire& myWire = I2cGetWire(sht3x_sensors[sensor].bus);
  if (&myWire == nullptr) { return false; }   // No valid I2c bus
  uint32_t type = sht3x_sensors[sensor].type;
  uint8_t i2c_address = sht3x_sensors[sensor].address;
  myWire.beginTransmission(i2c_address);
  switch (type) {
    case SHT3X_TYPE_SHT3X:
      // TODO: Clock stretching is used for SHT3x but not for SHTC3. Why?
      myWire.write(0x2C);                     // Enable clock stretching
      myWire.write(0x06);                     // High repeatability measurement
      break;
    case SHT3X_TYPE_SHTCX:
      myWire.write(0x35);                     // Wake from
      myWire.write(0x17);                     // sleep
      myWire.endTransmission();
      myWire.beginTransmission(i2c_address);
      // TODO: Clock stretching is used for SHT3x but not for SHTC3. Why?
      myWire.write(0x78);                     // Disable clock stretching
      myWire.write(0x66);                     // Normal mode measurement
      break;
    case SHT3X_TYPE_SHT4X:
      myWire.write(0xFD);                     // High repeatability measurement
      break;
  }
  if (myWire.endTransmission() != 0) {        // Stop I2C transmission
    return false;
  }
*/
  uint32_t type = sht3x_sensors[sensor].type;
  uint8_t i2c_address = sht3x_sensors[sensor].address;
  uint8_t i2c_bus = sht3x_sensors[sensor].bus;
  switch (type) {
    case SHT3X_TYPE_SHT3X:
      // TODO: Clock stretching is used for SHT3x but not for SHTC3. Why?
      if (!I2cWrite8(i2c_address, 0x2C, 0x06, i2c_bus)) {  // Enable clock stretching / High repeatability measurement
        return false;
      }
      break;
    case SHT3X_TYPE_SHTCX:
      if (!I2cWrite8(i2c_address, 0x35, 0x17, i2c_bus)) {  // Wake from sleep
        return false;
      }
      // TODO: Clock stretching is used for SHT3x but not for SHTC3. Why?
      if (!I2cWrite8(i2c_address, 0x78, 0x66, i2c_bus)) {  // Disable clock stretching / Normal mode measurement
        return false;
      }
      break;
    case SHT3X_TYPE_SHT4X:
      if (!I2cWrite0(i2c_address, 0xFD, i2c_bus)) {  // High repeatability measurement
        return false;
      }
      break;
  }

  delay(30);                                  // Timing verified with logic analyzer (10 is too short)
  uint8_t data[6];
/*
  myWire.requestFrom(i2c_address, (uint8_t)6); // Request 6 bytes of data
  for (uint32_t i = 0; i < 6; i++) {
    data[i] = myWire.read();                  // temperature (MSB, LSB, CRC), humidity (MSB, LSB, CRC)
  };
*/
  I2cReadBuffer0(i2c_address, data, 6, i2c_bus);

  if ((Sht3xComputeCrc(&data[0], 2) != data[2]) || (Sht3xComputeCrc(&data[3], 2) != data[5])) {
    return false;
  }
  int32_t t_100 = ((((data[0] << 8) | data[1]) * 17500) >> 16) - 4500;
  int32_t h_100;
  if (type == SHT3X_TYPE_SHT4X) {
    h_100 = ((((data[3] << 8) | data[4]) * 12500) >> 16) - 600;
  } else {
    h_100 = ((((data[3] << 8) | data[4]) * 10000) >> 16);
  }
  sht3x_sensors[sensor].temp = ConvertTemp(t_100/100.0f);
  sht3x_sensors[sensor].humi = ConvertHumidity(h_100/100.0f);
  if (isnan(sht3x_sensors[sensor].temp) || isnan(sht3x_sensors[sensor].humi)) { return false; }
    sht3x_sensors[sensor].valid = SENSOR_MAX_MISS;
  return true;
}

/********************************************************************************************/

void Sht3xDetect(void) {
  for (uint32_t bus = 0; bus < 2; bus++) {
    for (uint32_t k = 0; k < SHT3X_TYPES; k++) {
      for (uint32_t i = 0; i < SHT3X_ADDRESSES; i++) {
        if (!I2cSetDevice(sht3x_addresses[i], bus)) { continue; }
        sht3x_sensors[sht3x_count].type = k;
        sht3x_sensors[sht3x_count].address = sht3x_addresses[i];
        sht3x_sensors[sht3x_count].bus = bus;
        if (Sht3xRead(sht3x_count)) {
          GetTextIndexed(sht3x_sensors[sht3x_count].types, sizeof(sht3x_sensors[sht3x_count].types), sht3x_sensors[sht3x_count].type, kSht3xTypes);
          I2cSetActiveFound(sht3x_sensors[sht3x_count].address, sht3x_sensors[sht3x_count].types, sht3x_sensors[sht3x_count].bus);
          sht3x_count++;
          if (SHT3X_ADDRESSES == sht3x_count) {
            return;
          }
        }
      }
    }
  }
}


void Sht3xUpdate(void) {
    for (uint32_t idx = 0; idx < sht3x_count; idx++) {
      if (!Sht3xRead(idx)) {
        AddLogMissed(sht3x_sensors[idx].types, sht3x_sensors[idx].valid);
      }
  }
}

void Sht3xShow(bool json) {
  char types[11];

  for (uint32_t idx = 0; idx < sht3x_count; idx++) {
    if (sht3x_sensors[idx].valid) {
      strlcpy(types, sht3x_sensors[idx].types, sizeof(types));
      if (sht3x_count > 1) {
        snprintf_P(types, sizeof(types), PSTR("%s%c%02X"), types, IndexSeparator(), sht3x_sensors[idx].address);  // "SHT3X-0xXX"  
#ifdef USE_I2C_BUS2
        if (TasmotaGlobal.i2c_enabled[1]) {
          for (uint32_t i = 1; i < sht3x_count; i++) {
            if (sht3x_sensors[0].bus != sht3x_sensors[i].bus) {
              snprintf_P(types, sizeof(types), PSTR("%s%c%d"), types, IndexSeparator(), sht3x_sensors[idx].bus + 1); // "SHT3X-0xXX-X"  
              break;
            }
          }
        }
#endif  // USE_I2C_BUS2
      }
      TempHumDewShow(json, ((0 == TasmotaGlobal.tele_period) && (0 == idx)), types, sht3x_sensors[idx].temp, sht3x_sensors[idx].humi);
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
      case FUNC_EVERY_SECOND:
        Sht3xUpdate();
        break;
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
