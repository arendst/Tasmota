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

/*********************************************************************************************\
 * AHT20/AHT30 handling for deep sleep
 *
 * The AHT20 stays powered while the ESP deep sleeps, so it gets initialised again on every wake.
 * The old path sent 0xBE on every boot and never checked that a measurement started, so a stale
 * result could be read back as new. With deep sleep, most readings were repeats.
 * AHT20/AHT30 now: soft reset at init, status via 0x71, 0xBE only when not calibrated (datasheet),
 * a check that the busy bit is set after the 0xAC trigger, and a 7-byte read with CRC8.
 * Faulty cycles keep the previous values and are retried the next second.
 * Optional #define USE_AHT_DIAG adds "AHT2XDiag" with status bytes and counters to the sensor JSON.
 * The AHT1x path is unchanged.
\*********************************************************************************************/
#if defined(USE_AHT2x) || defined(USE_AHT3x)
  #define AHT_X0_ROBUST
#endif
#define AHT2X_RST_DELAY      20   // soft reset < 20 ms (AHT20 datasheet)
#define AHT_STATUS_BUSY      0x80
#define AHT_STATUS_CAL       0x08
#define AHT_MAX_RETRIES      3    // fast retries (1 s apart) before falling back to the 11 s cycle

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
  uint8_t Pcount = 9;
#ifdef AHT_X0_ROBUST
  uint8_t retries = 0;
  uint8_t init_status = 0;   // status after init (expect 0x18: calibrated, idle)
  uint8_t trig_status = 0;   // status right after 0xAC (expect busy bit 0x80)
  uint8_t read_status = 0;   // status byte of the last data read
  uint16_t noack = 0;        // triggers the chip did not start
  uint16_t crc_err = 0;
  uint16_t read_err = 0;     // short read, busy at read time, or all-zero data
  uint16_t resets = 0;
#endif
} aht1x;

struct {
  float   humidity = NAN;
  float   temperature = NAN;
  uint8_t address;     // bus address
  char    types[6];   // Sensor type name and address -
} aht1x_sensors[AHT1X_MAX_SENSORS];

bool AHT1XWrite(uint8_t aht1x_idx) {
  TwoWire &wire = I2cGetWire();
  wire.beginTransmission(aht1x_sensors[aht1x_idx].address);
  wire.write(AHTMeasureCmd, 3);
  if (wire.endTransmission() != 0)
    return false;
  delay(AHT1X_MEAS_DELAY);
  return true;
}

bool AHT1XRead(uint8_t aht1x_idx) {
  uint8_t data[6];
  TwoWire &wire = I2cGetWire();
  wire.requestFrom(aht1x_sensors[aht1x_idx].address, (uint8_t) 6);

  for(uint8_t i = 0; wire.available() > 0; i++) {
     data[i] = wire.read();
  }
  if (data[0] & 0x80)
    return false; //device is busy

  aht1x_sensors[aht1x_idx].humidity    = (((data[1] << 12)| (data[2] << 4) | data[3] >> 4) * AHT_HUMIDITY_CONST / KILOBYTE_CONST);
  aht1x_sensors[aht1x_idx].temperature = ((AHT_TEMPERATURE_CONST * (((data[3] & 0x0F) << 16) | (data[4] << 8) | data[5]) / KILOBYTE_CONST) - AHT_TEMPERATURE_OFFSET);

  return (!isnan(aht1x_sensors[aht1x_idx].temperature) && !isnan(aht1x_sensors[aht1x_idx].humidity) && (aht1x_sensors[aht1x_idx].humidity != 0));
}

/********************************************************************************************/

#ifdef AHT_X0_ROBUST
uint8_t AHT2XCrc8(const uint8_t *data, uint32_t len) {  // CRC8, poly 0x31, init 0xFF
  uint8_t crc = 0xFF;
  for (uint32_t i = 0; i < len; i++) {
    crc ^= data[i];
    for (uint32_t b = 0; b < 8; b++) {
      crc = (crc & 0x80) ? (crc << 1) ^ 0x31 : (crc << 1);
    }
  }
  return crc;
}

// with_cmd: send 0x71 first (idle chip). Without it, a plain read returns the status byte
// without disturbing a running measurement.
bool AHT2XGetStatus(uint8_t address, uint8_t &status, bool with_cmd) {
  TwoWire &wire = I2cGetWire();
  if (with_cmd) {
    wire.beginTransmission(address);
    wire.write(0x71);
    if (wire.endTransmission() != 0) { return false; }
  }
  if (wire.requestFrom(address, (uint8_t) 1) != 1) { return false; }
  status = wire.read();
  return true;
}

bool AHT2XInit(uint8_t address) {
  TwoWire &wire = I2cGetWire();
  wire.beginTransmission(address);
  wire.write(AHTResetCmd);
  if (wire.endTransmission() != 0) { return false; }
  delay(AHT2X_RST_DELAY);

  uint8_t status;
  if (!AHT2XGetStatus(address, status, true)) { return false; }
  if (!(status & AHT_STATUS_CAL)) {
    wire.beginTransmission(address);
    wire.write(AHTSetCalCmd, 3);
    if (wire.endTransmission() != 0) { return false; }
    delay(AHT1X_CMD_DELAY);
    if (!AHT2XGetStatus(address, status, true)) { return false; }
  }
  aht1x.init_status = status;
  return (status & AHT_STATUS_CAL);
}

bool AHT2XWrite(uint8_t aht1x_idx) {
  uint8_t address = aht1x_sensors[aht1x_idx].address;
  TwoWire &wire = I2cGetWire();
  wire.beginTransmission(address);
  wire.write(AHTMeasureCmd, 3);
  if (wire.endTransmission() != 0) { return false; }
  uint8_t status = 0;
  bool ok = AHT2XGetStatus(address, status, false);
  aht1x.trig_status = ok ? status : 0;
  delay(AHT1X_MEAS_DELAY);
  if (!ok || !(status & AHT_STATUS_BUSY)) {  // measurement not started: data would be stale
    aht1x.noack++;
    AddLog(LOG_LEVEL_DEBUG, PSTR("AHT: Trigger not acknowledged (status %02X)"), aht1x.trig_status);
    return false;
  }
  return true;
}

bool AHT2XRead(uint8_t aht1x_idx) {
  uint8_t data[7] = { 0 };
  TwoWire &wire = I2cGetWire();
  if (wire.requestFrom(aht1x_sensors[aht1x_idx].address, (uint8_t) 7) != 7) {
    aht1x.read_err++;
    return false;
  }
  for (uint32_t i = 0; i < 7; i++) { data[i] = wire.read(); }
  aht1x.read_status = data[0];
  if (data[0] & AHT_STATUS_BUSY) {
    aht1x.read_err++;
    return false;
  }
  if (AHT2XCrc8(data, 6) != data[6]) {
    aht1x.crc_err++;
    AddLog(LOG_LEVEL_DEBUG, PSTR("AHT: CRC error"));
    return false;
  }
  uint32_t raw_hum = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | (data[3] >> 4);
  uint32_t raw_tem = (((uint32_t)data[3] & 0x0F) << 16) | ((uint32_t)data[4] << 8) | data[5];
  if ((0 == raw_hum) && (0 == raw_tem)) {  // would read -50 C / 0 %
    aht1x.read_err++;
    return false;
  }
  aht1x_sensors[aht1x_idx].humidity    = raw_hum * AHT_HUMIDITY_CONST / KILOBYTE_CONST;
  aht1x_sensors[aht1x_idx].temperature = (AHT_TEMPERATURE_CONST * raw_tem / KILOBYTE_CONST) - AHT_TEMPERATURE_OFFSET;
  return true;
}

// After a failed cycle, start a new one the next second, at most AHT_MAX_RETRIES times in a row.
void AHT2XRetry(void) {
  if (aht1x.retries < AHT_MAX_RETRIES) {
    aht1x.retries++;
    aht1x.Pcount = 9;   // next call writes again
  } else {
    aht1x.retries = 0;
    aht1x.Pcount = 0;   // back to the normal 11 s cycle
  }
}

// A failed cycle keeps the old values. An unacknowledged trigger also resets the chip.
void AHT2XPoll(void) {
  aht1x.Pcount++;
  switch (aht1x.Pcount) {
    case 10:
      aht1x.write_ok = AHT2XWrite(0);
      if (!aht1x.write_ok) {
        aht1x.resets++;
        AHT2XInit(aht1x_sensors[0].address);
        AHT2XRetry();
      }
      break;
    case 11:
      if (AHT2XRead(0)) {
        aht1x.retries = 0;
        aht1x.Pcount = 0;
      } else {
        AHT2XRetry();
      }
      break;
  }
}
#endif  // AHT_X0_ROBUST

// Polling the device without delays
// Incompatible with other devices on I2C bus
void AHT1XPoll(void) {  // We have 100ms for read. Sensor needs 80-95 ms
#ifdef AHT_X0_ROBUST
  AHT2XPoll();
#else
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
#endif  // AHT_X0_ROBUST
}

unsigned char AHT1XReadStatus(uint8_t aht1x_address) {
  uint8_t result = 0;
  // Need for AHT20?
  //Wire.beginTransmission(aht1x_address);
  //Wire.write(0x71);
  //if (Wire.endTransmission() != 0) return false;
  TwoWire &wire = I2cGetWire();
  wire.requestFrom(aht1x_address, (uint8_t) 1);
  result = wire.read();
  return result;
}

void AHT1XReset(uint8_t aht1x_address) {
  TwoWire &wire = I2cGetWire();
  wire.beginTransmission(aht1x_address);
  wire.write(AHTResetCmd);
  wire.endTransmission();
  delay(AHT1X_RST_DELAY);
}

/********************************************************************************************/
bool AHT1XInit(uint8_t aht1x_address) {
#ifdef AHT_X0_ROBUST
  return AHT2XInit(aht1x_address);
#else
  TwoWire &wire = I2cGetWire();
  wire.beginTransmission(aht1x_address);
  wire.write(AHTSetCalCmd, 3);
  if (wire.endTransmission() != 0)
    return false;
  delay(AHT1X_CMD_DELAY);
  if(AHT1XReadStatus(aht1x_address) & 0x08) // Sensor calibrated?
    return true;
  return false;
#endif  // AHT_X0_ROBUST
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
#if defined(AHT_X0_ROBUST) && defined(USE_AHT_DIAG)
  if (json && aht1x.count) {
    ResponseAppend_P(PSTR(",\"AHT2XDiag\":{\"Init\":\"%02X\",\"Trig\":\"%02X\",\"Read\":\"%02X\",\"NoAck\":%u,\"Crc\":%u,\"ReadErr\":%u,\"Reset\":%u}"),
      aht1x.init_status, aht1x.trig_status, aht1x.read_status, aht1x.noack, aht1x.crc_err, aht1x.read_err, aht1x.resets);
  }
#endif
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
