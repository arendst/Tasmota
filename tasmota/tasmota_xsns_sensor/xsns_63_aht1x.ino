/*
  xsns_63_AHT1x.ino - AHT10 I2C temperature and humidity sensor support for Tasmota

  Copyright (C) 2021  Martin Wagner

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
#if defined(USE_AHT1x) || defined(USE_AHT2x) || defined(USE_AHT3x)

/*********************************************************************************************\
 * AHT10/15/20 - Temperature and Humidity
 *
 * AHT1x I2C Address: 0x38, 0x39
 * the driver supports two I2c adresses but only one Sensor allowed.
 *
 * Attention: the AHT10/15 Sensor is incompatible with other I2C devices on I2C bus.
 *
 * The Datasheet write:
 * "Only a single AHT10 can be connected to the I2C bus and no other I2C
 *  devices can be connected".
 *
 * after lot of search and tests, now is confirmed that works only reliable with one sensor
 * on I2C Bus
 *
 * 27.08.2020 support for AHT20 added. Now, the AHT20 should support standard I2C Protokoll
 *            and allows other I2C Devices on the bus but have only one I2C Address (0x38)
 * 14.09.2021 support AHT20 without enabling AHT1x 
 *
 * 26.10.2023 support for AHT30 added.
 *
 * AHT20 I2C Address: 0x38
 * AHT30 I2C Address: 0x38
\*********************************************************************************************/

#define XSNS_63              63
#define XI2C_43              43  // See I2CDEVICES.md

#define AHT1X_ADDR1          0x38
#define AHT1X_ADDR2          0x39

#define AHT1X_MAX_SENSORS    2

#define AHT_HUMIDITY_CONST      100
#define AHT_TEMPERATURE_CONST   200
#define AHT_TEMPERATURE_OFFSET  50
#define KILOBYTE_CONST          1048576.0f

#define AHT1X_CMD_DELAY      20
#define AHT1X_RST_DELAY      10
#define AHT1X_MEAS_DELAY     80 // over 75ms in datasheet

#ifdef USE_AHT3x
  #define AHTX_CMD     0xBE // Cmd for AHT3x
  const char ahtTypes[] PROGMEM = "AHT3X|AHT3X";
#elif defined(USE_AHT2x)
  #define AHTX_CMD     0xBE // Cmd for AHT2x
  const char ahtTypes[] PROGMEM = "AHT2X|AHT2X";
#else
  #define AHTX_CMD     0xE1 // Cmd for AHT1x
  const char ahtTypes[] PROGMEM = "AHT1X|AHT1X";
#endif

uint8_t AHTSetCalCmd[3]    = { AHTX_CMD, 0x08, 0x00 }; // load factory calibration coeff
uint8_t AHTSetCycleCmd[3]  = { AHTX_CMD, 0x28, 0x00 }; // enable cycle mode
uint8_t AHTMeasureCmd[3]   = { 0xAC, 0x33, 0x00 };     // start measurment command
uint8_t AHTResetCmd        =   0xBA;                   // soft reset command

struct {
  bool write_ok = false;
  uint8_t addresses[2] = { AHT1X_ADDR1, AHT1X_ADDR2 };
  uint8_t count  = 0;
  uint8_t Pcount = 0;
} aht1x;

struct {
  float   humidity = NAN;
  float   temperature = NAN;
  uint8_t address;     // bus address
  char    types[6];   // Sensor type name and address -
} aht1x_sensors[AHT1X_MAX_SENSORS];

bool AHT1XWrite(uint8_t aht1x_idx) {
  Wire.beginTransmission(aht1x_sensors[aht1x_idx].address);
  Wire.write(AHTMeasureCmd, 3);
  if (Wire.endTransmission() != 0)
    return false;
  delay(AHT1X_MEAS_DELAY);
  return true;
}

bool AHT1XRead(uint8_t aht1x_idx) {
  uint8_t data[6];
  Wire.requestFrom(aht1x_sensors[aht1x_idx].address, (uint8_t) 6);

  for(uint8_t i = 0; Wire.available() > 0; i++) {
     data[i] = Wire.read();
  }
  if (data[0] & 0x80)
    return false; //device is busy

  aht1x_sensors[aht1x_idx].humidity    = (((data[1] << 12)| (data[2] << 4) | data[3] >> 4) * AHT_HUMIDITY_CONST / KILOBYTE_CONST);
  aht1x_sensors[aht1x_idx].temperature = ((AHT_TEMPERATURE_CONST * (((data[3] & 0x0F) << 16) | (data[4] << 8) | data[5]) / KILOBYTE_CONST) - AHT_TEMPERATURE_OFFSET);

  return (!isnan(aht1x_sensors[aht1x_idx].temperature) && !isnan(aht1x_sensors[aht1x_idx].humidity) && (aht1x_sensors[aht1x_idx].humidity != 0));
}

/********************************************************************************************/

// Polling the device without delays
// Incompatible with other devices on I2C bus
void AHT1XPoll(void) {  // We have 100ms for read. Sensor needs 80-95 ms
  aht1x.Pcount++;
  switch (aht1x.Pcount) {
    case 10:
      aht1x.write_ok = AHT1XWrite(0);
      break;
    case 11:
      if (aht1x.write_ok) AHT1XRead(0);
      aht1x.Pcount = 0;
      break;
  }
}

unsigned char AHT1XReadStatus(uint8_t aht1x_address) {
  uint8_t result = 0;
  // Need for AHT20?
  //Wire.beginTransmission(aht1x_address);
  //Wire.write(0x71);
  //if (Wire.endTransmission() != 0) return false;
  Wire.requestFrom(aht1x_address, (uint8_t) 1);
  result = Wire.read();
  return result;
}

void AHT1XReset(uint8_t aht1x_address) {
  Wire.beginTransmission(aht1x_address);
  Wire.write(AHTResetCmd);
  Wire.endTransmission();
  delay(AHT1X_RST_DELAY);
}

/********************************************************************************************/
bool AHT1XInit(uint8_t aht1x_address) {
  Wire.beginTransmission(aht1x_address);
  Wire.write(AHTSetCalCmd, 3);
  if (Wire.endTransmission() != 0)
    return false;
  delay(AHT1X_CMD_DELAY);
  if(AHT1XReadStatus(aht1x_address) & 0x08) // Sensor calibrated?
    return true;
  return false;
}

void AHT1XDetect(void) {
  for (uint8_t i = 0; i < AHT1X_MAX_SENSORS; i++) {
    if (!I2cSetDevice(aht1x.addresses[i])) {continue;}
    if (AHT1XInit(aht1x.addresses[i])) {
      aht1x_sensors[aht1x.count].address = aht1x.addresses[i];
      GetTextIndexed(aht1x_sensors[aht1x.count].types, sizeof(aht1x_sensors[aht1x.count].types), i, ahtTypes);
      I2cSetActiveFound(aht1x_sensors[aht1x.count].address, aht1x_sensors[aht1x.count].types);
      aht1x.count++;
      break; // Only one Sensor allowed at the moment (I2C Sensor-Bug)
    }
  }
}

void AHT1XShow(bool json) {
  for (uint32_t i = 0; i < aht1x.count; i++) {
    float tem = ConvertTemp(aht1x_sensors[i].temperature);
    float hum = ConvertHumidity(aht1x_sensors[i].humidity);
    char types[11]; // AHT1X-0x38
    strlcpy(types, aht1x_sensors[i].types, sizeof(types));
    if (aht1x.count > 1) {
      snprintf_P(types, sizeof(types), PSTR("%s%c%02X"), aht1x_sensors[i].types, IndexSeparator(), aht1x_sensors[i].address);  // "X-0xXX"
    }
    TempHumDewShow(json, ((0 == TasmotaGlobal.tele_period) && (0 == i)), types, tem, hum);
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns63(uint32_t function)
{
  if (!I2cEnabled(XI2C_43)) { return false; }
  bool result = false;

  if (FUNC_INIT == function) {
    AHT1XDetect();
  }
  else if (aht1x.count) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        AHT1XPoll();
        break;
      case FUNC_JSON_APPEND:
        AHT1XShow(1);
        break;
  #ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        AHT1XShow(0);
        break;
  #endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_AHT1X
#endif  // USE_I2C
