/*
  xsns_14_sht3x.ino - SHT3X temperature and humidity sensor support for Tasmota

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
 * - SHT3x series: SHT30, SHT31, SHT35 (adresses: A: 0x44, B: 0x45)
 * - SHTC series: SHTC1, SHTC3 (address: 0x70)
 * - SHT4x series: SHT40, SHT41, SHT45 (addresses: A: 0x44, B: 0x45)
\*********************************************************************************************/

#define XSNS_14             14
#define XI2C_15             15         // See I2CDEVICES.md

#define SHT3X_ADDRESSES     2          // 2 addresses for SHT3x
#define SHT3X_ADDR_GND      0x44       // Address A pin low (GND)
#define SHT3X_ADDR_VDD      0x45       // Address B pin high (VDD)
#define SHTCX_ADDRESSES     1          // 1 address for SHTCx
#define SHTCX_ADDR          0x70       // Address for SHTCx sensors
#define SHT4X_ADDRESSES     2          // 2 addresses for SHT4x
#define SHT4X_ADDR_A        0x44       // Address SHT4x A
#define SHT4X_ADDR_B        0x45       // Address SHT4x B
#define SHT3X_MAX_SENSORS   SHT3X_ADDRESSES + SHTC3_ADDRESSES + SHT4X_ADDRESSES

enum SHT3X_Types {
  SHT3X_TYPE_SHT3X,
  SHT3X_TYPE_SHTCX,
  SHT3X_TYPE_SHT4X
};
const char kSht3xTypes[] PROGMEM = "SHT3X|SHTC3|SHT4X";

uint8_t sht3x_addresses[] = { SHT3X_ADDR_GND, SHT3X_ADDR_VDD };
uint8_t shtcx_addresses[] = { SHTCX_ADDR };
uint8_t sht4x_addresses[] = { SHT4X_ADDR_A, SHT4X_ADDR_B };

uint8_t sht3x_count = 0;
struct SHT3XSTRUCT {
  uint8_t type;       // Sensor type
  uint8_t address;    // I2C bus address
  char typeString[6]; // Sensor type name and address, e.g. "SHT3X-0xXX"
} sht3x_sensors[SHT3X_ADDRESSES];

uint8_t computeCrc(uint8_t data[], uint8_t len)
// Compute CRC as per datasheet
{
  uint8_t crc = 0xFF;

  for (uint8_t x = 0 ; x < len ; x++)
  {
    crc ^= data[x];
    for (uint8_t i = 0 ; i < 8 ; i++)
    {
      if (crc & 0x80)
        crc = (crc << 1) ^ 0x31;
      else
        crc <<= 1;
    }
  }
  return crc;
}

float convertValue(uint8_t data[], float A, float B, float C)
{
  return A + (B * (float)((data[0] << 8) | data[1])) / C;
}

bool Sht3xRead(float &t, float &h, uint8_t sht3x_address)
// Read temperature and humidity for SHT3x series
{
  uint8_t data[6];

  t = NAN;
  h = NAN;

  Wire.beginTransmission(sht3x_address);
  // TODO: Clock stretching is used for SHT3x but not for SHTC3. Why?
  Wire.write(0x2C);                     // Enable clock stretching
  Wire.write(0x06);                     // High repeatability measurement
  if (Wire.endTransmission() != 0) {    // Stop I2C transmission
    return false;
  }
  delay(15); // Max. duration as per datasheet and example code
  Wire.requestFrom(sht3x_address, (uint8_t)6); // Request 6 bytes of data
  for (uint32_t i = 0; i < 6; i++) {
    data[i] = Wire.read(); // temperature (MSB, LSB, CRC), humidity (MSB, LSB, CRC)
  };
  // Check CRCs
  if ((computeCrc(&data[0], 2) != data[2]) || (computeCrc(&data[3], 2) != data[5])) {
    return false;
  }
  t = convertValue(&data[0], -45.0, 175.0, 65535.0);
  h = convertValue(&data[3], 0.0, 100.0, 65535.0);
  return (!isnan(t) && !isnan(h));
}

bool ShtcxReadId(uint16_t &id, uint8_t shtcx_address)
// Read id register of SHTCx series
{
  uint8_t data[3];

  Wire.beginTransmission(shtcx_address);
  Wire.write(0x35);                    // Wake from
  Wire.write(0x17);                    // sleep
  Wire.endTransmission();
  Wire.beginTransmission(shtcx_address);
  Wire.write(0xEF);                    // Read-out
  Wire.write(0xC8);                    // id register
  if (Wire.endTransmission() != 0) {   // Stop I2C transmission
    return false;
  }
  delay(1);
  Wire.requestFrom(shtcx_address, (uint8_t)3); // Request 3 bytes of data
  for (uint32_t i = 0; i < 3; i++) {
    data[i] = Wire.read(); // Id (MSB, LSB, CRC)
  };
  // Check CRC
  if (computeCrc(&data[0], 2) != data[2]) {
    return false;
  }
  id = (data[0] << 8) | data[1];
  return true;
}

bool ShtcxRead(float &t, float &h, uint8_t shtcx_address)
// Read temperature and humidity for SHTCx series
{
  uint8_t data[6];

  t = NAN;
  h = NAN;

  Wire.beginTransmission(shtcx_address);
  Wire.write(0x35);                    // Wake from
  Wire.write(0x17);                    // sleep
  Wire.endTransmission();
  Wire.beginTransmission(shtcx_address);
  // TODO: Clock stretching is used for SHT3x but not for SHTC3. Why?
  Wire.write(0x78);                    // Disable clock stretching
  Wire.write(0x66);                    // Normal mode measurement
  if (Wire.endTransmission() != 0) {   // Stop I2C transmission
    return false;
  }
  delay(15); // Max. duration as per datasheet and sample code
  Wire.requestFrom(shtcx_address, (uint8_t)6); // Request 6 bytes of data
  for (uint32_t i = 0; i < 6; i++) {
    data[i] = Wire.read(); // temperature (MSB, LSB, CRC), humidity (MSB, LSB, CRC)
  };
  // Check CRCs
  if ((computeCrc(&data[0], 2) != data[2]) || (computeCrc(&data[3], 2) != data[5])) {
    return false;
  }
  t = convertValue(&data[0], -45.0, 175.0, 65535.0);
  h = convertValue(&data[3], 0.0, 100.0, 65535.0);
  return (!isnan(t) && !isnan(h));
}

bool Sht4xReadSerial(uint32_t &serial, uint8_t sht4x_address)
// Read serial number of SHT4x series
{
  uint8_t data[6];

  Wire.beginTransmission(sht4x_address);
  Wire.write(0x89);                    // Send command: read serial number
  if (Wire.endTransmission() != 0) {   // Stop I2C transmission
    return false;
  }
  delay(1);
  Wire.requestFrom(sht4x_address, (uint8_t)6); // Request 6 bytes of data
  for (uint32_t i = 0; i < 6; i++) {
    data[i] = Wire.read(); // serial (MSB1, LSB1, CRC1, MSB2, LSB2, CRC2)
  };
  // Check CRCs
  if ((computeCrc(&data[0], 2) != data[2]) || (computeCrc(&data[3], 2) != data[5])) {
    return false;
  }
  // Convert to 32-bit integer
  serial = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
  return true;
}

bool Sht4xRead(float &t, float &h, uint8_t sht4x_address)
// Read temperature and humidity for SHT4x series
{
  uint8_t data[6];

  t = NAN;
  h = NAN;

  Wire.beginTransmission(sht4x_address);
  Wire.write(0xFD);                    // High repeatability measurement
  if (Wire.endTransmission() != 0) {   // Stop I2C transmission
    return false;
  }
  delay(10); // Max. duration as per datasheet and example code
  Wire.requestFrom(sht4x_address, (uint8_t)6); // Request 6 bytes of data
  for (uint32_t i = 0; i < 6; i++) {
    data[i] = Wire.read(); // temperature (MSB, LSB, CRC), humidity (MSB, LSB, CRC)
  };
  // Check CRCs
  if ((computeCrc(&data[0], 2) != data[2]) || (computeCrc(&data[3], 2) != data[5])) {
    return false;
  }
  t = convertValue(&data[0], -45.0, 175.0, 65535.0);
  h = convertValue(&data[3], -6.0, 125.0, 65535.0);
  return (!isnan(t) && !isnan(h));
}

/********************************************************************************************/

void Sht3xDetect(void)
{
  uint32_t i;
  // Probe SHT3x: Try reading temperature and humidity
  for (i = 0; i < SHT3X_ADDRESSES; i++) {
    if (!I2cSetDevice(sht3x_addresses[i])) { continue; }
    float t;
    float h;
    if (Sht3xRead(t, h, sht3x_addresses[i])) {
      sht3x_sensors[sht3x_count].type = SHT3X_TYPE_SHT3X;
      sht3x_sensors[sht3x_count].address = sht3x_addresses[i];
      GetTextIndexed(sht3x_sensors[sht3x_count].typeString, sizeof(sht3x_sensors[sht3x_count].typeString), SHT3X_TYPE_SHT3X, kSht3xTypes);
      I2cSetActiveFound(sht3x_sensors[sht3x_count].address, sht3x_sensors[sht3x_count].typeString);
      sht3x_count++;
    }
  }
  // Probe SHTCx: Try reading id
  for (i = 0; i < SHTCX_ADDRESSES; i++) {
    if (!I2cSetDevice(shtcx_addresses[i])) { continue; }
    uint16_t id;
    if (ShtcxReadId(id, shtcx_addresses[i])) {
      sht3x_sensors[sht3x_count].type = SHT3X_TYPE_SHTCX;
      sht3x_sensors[sht3x_count].address = shtcx_addresses[i];
      GetTextIndexed(sht3x_sensors[sht3x_count].typeString, sizeof(sht3x_sensors[sht3x_count].typeString), SHT3X_TYPE_SHTCX, kSht3xTypes);
      I2cSetActiveFound(sht3x_sensors[sht3x_count].address, sht3x_sensors[sht3x_count].typeString);
      sht3x_count++;
    }
  }
  // Probe SHT4x: Try reading serial number
  for (i = 0; i < SHT4X_ADDRESSES; i++) {
    if (!I2cSetDevice(sht4x_addresses[i])) { continue; }
    uint32_t serial;
    if (Sht4xReadSerial(serial, sht4x_addresses[i])) {
      sht3x_sensors[sht3x_count].type = SHT3X_TYPE_SHT4X;
      sht3x_sensors[sht3x_count].address = sht4x_addresses[i];
      GetTextIndexed(sht3x_sensors[sht3x_count].typeString, sizeof(sht3x_sensors[sht3x_count].typeString), SHT3X_TYPE_SHT4X, kSht3xTypes);
      I2cSetActiveFound(sht3x_sensors[sht3x_count].address, sht3x_sensors[sht3x_count].typeString);
      sht3x_count++;
    }
  }
}

void Sht3xShow(bool json)
{
  bool result;
  float t;
  float h;
  char types[11];

  for (uint32_t i = 0; i < sht3x_count; i++) {
    switch (sht3x_sensors[i].type) {
      case SHT3X_TYPE_SHT3X:
        result = Sht3xRead(t, h, sht3x_sensors[i].address);
        break;
      case SHT3X_TYPE_SHTCX:
        result = ShtcxRead(t, h, sht3x_sensors[i].address);
        break;
      case SHT3X_TYPE_SHT4X:
        result = Sht4xRead(t, h, sht3x_sensors[i].address);
        break;
    }
    if (result) {
      t = ConvertTemp(t);
      h = ConvertHumidity(h);
      strlcpy(types, sht3x_sensors[i].typeString, sizeof(types));
      if (sht3x_count > 1) {
        snprintf_P(types, sizeof(types), PSTR("%s%c%02X"), sht3x_sensors[i].typeString, IndexSeparator(), sht3x_sensors[i].address);  // "SHT3X-0xXX"
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
