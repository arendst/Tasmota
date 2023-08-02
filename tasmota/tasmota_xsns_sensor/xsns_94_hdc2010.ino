/*
  xsns_94_hdc2010.ino - Texas Instruments HDC1080 temperature and humidity sensor support for Tasmota

  Copyright (C) 2021  Luc Boudreau

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
#ifdef USE_HDC2010

/*********************************************************************************************\
 * HDC2010 - Temperature and Humidity sensor
 *
 * Source:        Luc Boudreau
 * Other sources: Luis Teixeira from the HDC1080 code (GPL3+)
 *                sSense arduino library (Public Domain)
 *
 * I2C Address: 0x40
\*********************************************************************************************/

#define XSNS_94             94
#define XI2C_64             64      // See I2CDEVICES.md

#define HDC2010_ADDR        0x40

// Registers:

#define HDC2010_REG_TEMP_LSB        0x00    // Temperature register LSB
#define HDC2010_REG_TEMP_MSB        0x01    // Temperature register MSB
#define HDC2010_REG_RH_LSB          0x02    // Humidity register LSB
#define HDC2010_REG_RH_MSB          0x03    // Humidity register MSB
#define HDC2010_REG_INTR_DRDY       0x04    // Interrupt / Data Ready register
#define HDC2010_REG_CONFIG          0x0F    // Configuration register
#define HDC2010_REG_RESET           0x0E    // Reset register
#define HDC2010_REG_MAN_LSB         0xFC    // Manufacturer LSB
#define HDC2010_REG_MAN_MSB         0xFD    // Manufacturer MSB
#define HDC2010_REG_DEV_LSB         0xFE    // Device ID LSB
#define HDC2010_REG_DEV_MSB         0xFF    // Device ID MSB

// Note: These are bit flipped. Actual IDs need to shift byte 0 and 1
#define HDC2010_MAN_ID      0x4954  // Manufacturer ID (Texas Instruments)
#define HDC2010_DEV_ID      0xD007  // Device ID (valid for the HDC2010)

#define HDC2010_CONV_TIME   50

struct HDC2010 {
  const char* hdc_type_name = "HDC2010";
  uint16_t hdc_manufacturer_id = 0;
  uint16_t hdc_device_id = 0;
  float hdc_temperature = 0.0;
  float hdc_humidity = 0.0;
  uint8_t hdc_valid = 0;
} HDC2010;

/**
 * Reads the device ID register.
 */
uint16_t Hdc2010ReadDeviceId(void) {
  return I2cRead16(HDC2010_ADDR, HDC2010_REG_DEV_LSB);
}

/**
 * Reads the manufacturer ID register.
 */
uint16_t Hdc2010ReadManufacturerId(void) {
  return I2cRead16(HDC2010_ADDR, HDC2010_REG_MAN_LSB);
}

/**
 * The various initialization steps for this sensor.
 */
void Hdc2010Init(void)  {
  Hdc2010Reset();
  Hdc2010SetMeasurementConfig();
  HDC2010.hdc_valid = 1;
}

/**
 * Performs a temp and humidity read
 */
void Hdc2010Read(void) {
  uint8_t byte[2];
	uint16_t temp;
  uint16_t humidity;
	byte[0] = I2cRead8(HDC2010_ADDR, HDC2010_REG_TEMP_LSB);
	byte[1] = I2cRead8(HDC2010_ADDR, HDC2010_REG_TEMP_MSB);
  //AddLog(LOG_LEVEL_DEBUG, PSTR("HdcRead: temperature raw data: 0x%02x 0x%02x"), byte[0], byte[1]);
	temp = (unsigned int)(byte[1]) << 8 | (unsigned int) byte[0];
  //AddLog(LOG_LEVEL_DEBUG, PSTR("HdcRead: temperature shifted data: %d"), temp);
	HDC2010.hdc_temperature = (float)(temp) * 165 / 65536 - 40;
  //AddLog(LOG_LEVEL_DEBUG, PSTR("HdcRead: temperature : %2_f"), &hdc_temperature);

	byte[0] = I2cRead8(HDC2010_ADDR, HDC2010_REG_RH_LSB);
	byte[1] = I2cRead8(HDC2010_ADDR, HDC2010_REG_RH_MSB);
  //AddLog(LOG_LEVEL_DEBUG, PSTR("HdcRead: humidity raw data: 0x%02x 0x%02x"), byte[0], byte[1]);
	humidity = (unsigned int)byte[1] << 8 | byte[0];
  //AddLog(LOG_LEVEL_DEBUG, PSTR("HdcRead: humidity shifted data: %d"), humidity);
	HDC2010.hdc_humidity = (float)(humidity)/( 65536 )* 100;
  //AddLog(LOG_LEVEL_DEBUG, PSTR("HdcRead: humidity : %2_f"), &hdc_humidity);
}

/**
 * Performs a reset of the sensor (slow oper)
 */
void Hdc2010Reset(void) {
  uint8_t current = I2cRead8(HDC2010_ADDR, HDC2010_REG_RESET);
  //AddLog(LOG_LEVEL_DEBUG, PSTR("HdcReset: current reset registry value = %d"), current);
  current |= 0x80;
  //AddLog(LOG_LEVEL_DEBUG, PSTR("HdcReset: new reset registry value = %d"), current);
  I2cWrite8(HDC2010_ADDR, HDC2010_REG_RESET, current);
  delay(HDC2010_CONV_TIME);
}

/**
 * Detects the sensor
 */
bool Hdc2010Detect(void) {
  if (!I2cSetDevice(HDC2010_ADDR)) { return false; }

  HDC2010.hdc_manufacturer_id = Hdc2010ReadManufacturerId();
  HDC2010.hdc_device_id = Hdc2010ReadDeviceId();

  AddLog(LOG_LEVEL_DEBUG, PSTR("HdcDetect: detected device with manufacturerId = 0x%04X and deviceId = 0x%04X"), HDC2010.hdc_manufacturer_id, HDC2010.hdc_device_id);

  if (HDC2010.hdc_device_id == HDC2010_DEV_ID && HDC2010.hdc_manufacturer_id == HDC2010_MAN_ID) {
    //AddLog(LOG_LEVEL_DEBUG, PSTR("HdcDetect: adding device HDC2010"));
    Hdc2010Init();
    I2cSetActiveFound(HDC2010_ADDR, HDC2010.hdc_type_name);
    return true;
  }
  return false;
}

/**
 * Shows the sensor in the ui
 */
void Hdc2010Show(bool json) {
  if (HDC2010.hdc_valid) {
    TempHumDewShow(json, (0 == TasmotaGlobal.tele_period), HDC2010.hdc_type_name, HDC2010.hdc_temperature, HDC2010.hdc_humidity);
  }
}

/**
 * Configures measurement settings
 */
void Hdc2010SetMeasurementConfig() {
  uint8_t resetContents = I2cRead8(HDC2010_ADDR, HDC2010_REG_RESET);
  //AddLog(LOG_LEVEL_DEBUG, PSTR("HdcSetMeasureRate: current reset registry value = %d"), resetContents);
  // Measure twice per second
  resetContents |= 0x60;
  //AddLog(LOG_LEVEL_DEBUG, PSTR("HdcSetMeasureRate: new reset registry value = %d"), resetContents);
  I2cWrite8(HDC2010_ADDR, HDC2010_REG_RESET, resetContents);

  uint8_t configContents = I2cRead8(HDC2010_ADDR, HDC2010_REG_CONFIG);
  //AddLog(LOG_LEVEL_DEBUG, PSTR("HdcSetMeasureRate: current config registry value = %d"), configContents);
  // Measure both temp and humidity
  configContents |= 0x01;
  //AddLog(LOG_LEVEL_DEBUG, PSTR("HdcSetMeasureRate: new config registry value = %d"), configContents);
  I2cWrite8(HDC2010_ADDR, HDC2010_REG_CONFIG, configContents);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns94(uint32_t function)
{
  if (!I2cEnabled(XI2C_64)) {
    return false;
  }

  bool result = false;

  if (FUNC_INIT == function) {
    result = Hdc2010Detect();
  }
  else if (HDC2010.hdc_device_id) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        if (HDC2010.hdc_valid) {
          Hdc2010Read();
          result = true;
        }
        break;
      case FUNC_JSON_APPEND:
        if (HDC2010.hdc_valid) {
          Hdc2010Show(1);
        }
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Hdc2010Show(0);
        result = true;
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_HDC2010
#endif  // USE_I2C

