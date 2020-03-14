/*
  xsns_65_hdc1080.ino - Texas Instruments HDC1080 temperature and humidity sensor support for Tasmota

  Copyright (C) 2020  Luis Teixeira

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
#ifdef USE_HDC1080

/*********************************************************************************************\
 * HDC1080 - Temperature and Humidy sensor
 *
 * Source: Luis Teixeira
 *
 * I2C Address: 0x40
\*********************************************************************************************/

#define XSNS_65             65
#define XI2C_45             45      // See I2CDEVICES.md

#define HDC1080_ADDR        0x40

// Registers:

#define HDC_REG_TEMP        0x00    // Temperature register
#define HDC_REG_RH          0x01    // Humidity register
#define HDC_REG_CONFIG      0x02    // Configuration register
#define HDC_REG_SERIAL1     0xFB    // First 2 bytes of the serial ID
#define HDC_REG_SERIAL2     0xFC    // Mid 2 bytes of the serial ID
#define HDC_REG_SERIAL3     0xFD    // Last bits of the serial ID
#define HDC_REG_MAN_ID      0xFE    // Manufacturer ID
#define HDC_REG_DEV_ID      0xFF    // Device ID

// Expected constant values of some of the registers:

#define HDC1080_MAN_ID      0x5449  // Manufacturer ID (Texas Instruments)
#define HDC1080_DEV_ID      0x1050  // Device ID (valid for the HDC1080)

// Possible values for the configuration register fields:

#define HDC1080_RST_ON      0x8000
#define HDC1080_HEAT_ON     0x2000
#define HDC1080_MODE_ON     0x1000  // acquision mode (temperature + humidity)
#define HDC1080_TRES_11     0x400
#define HDC1080_HRES_11     0x100
#define HDC1080_HRES_8      0x80

// Constants:

#define HDC1080_CONV_TIME   15      // Assume 6.50 + 6.35 ms + x of conversion delay for this device
#define HDC1080_TEMP_MULT   0.0025177
#define HDC1080_RH_MULT     0.0025177
#define HDC1080_TEMP_OFFSET 40.0

const char* hdc_type_name = "HDC1080";
uint16_t hdc_manufacturer_id = 0;
uint16_t hdc_device_id = 0;

float hdc_temperature = 0.0;
float hdc_humidity = 0.0;

uint8_t hdc_valid = 0;

bool is_reading = false;
uint32_t hdc_next_read;

/**
 * Reads the device ID register.
 *
 */
uint16_t HdcReadDeviceId(void) {
  return I2cRead16(HDC1080_ADDR, HDC_REG_DEV_ID);
}

/**
 * Reads the manufacturer ID register.
 *
 */
uint16_t HdcReadManufacturerId(void) {
  return I2cRead16(HDC1080_ADDR, HDC_REG_MAN_ID);
}

/**
 * Overwrites the configuration register with the provided config
 */
void HdcConfig(uint16_t config) {
  I2cWrite16(HDC1080_ADDR, HDC_REG_CONFIG, config);
}

/**
 * Performs a soft reset on the device.
 *
 * RST = 1 -> software reset
 *
 */
void HdcReset(void) {
  uint16_t current = I2cRead16(HDC1080_ADDR, HDC_REG_CONFIG);

  // bit 15 of the configuration register contains the RST flag
  // so we set it to 1:

  current |= 0x8000;

  I2cWrite16(HDC1080_ADDR, HDC_REG_CONFIG, current);

  delay(HDC1080_CONV_TIME);  // Not sure how long it takes to reset. Assuming this value.
}

/**
 * Performs the write portion of the HDC1080 sensor transaction. This
 * action of writing to a register signals the beginning of the operation
 * (e.g. data acquisition).
 *
 * addr: the address of the I2C device we are talking to.
 * reg: the register where we are writing to.
 *
 * returns: 0 if the transmission was successfully completed, != 0 otherwise.
 */
int8_t HdcTransactionOpen(uint8_t addr, uint8_t reg) {
  Wire.beginTransmission((uint8_t) addr);
  Wire.write((uint8_t) reg);
  return Wire.endTransmission();
}

/**
 * Performs the read portion of the HDC1080 sensor transaction.
 *
 * addr: the address of the I2C device we are talking to.
 * reg_data: the pointer to the memory location where we will place the bytes that were read from the device
 * len: the number of bytes we expect to read
 *
 * returns: if the read operation was successful. != 0 otherwise.
 */
int8_t HdcTransactionClose(uint8_t addr, uint8_t *reg_data, uint16_t len) {
  if (len != Wire.requestFrom((uint8_t) addr, (uint8_t) len)) {
    return 1;
  }

  while (len--) {
    *reg_data = (uint8_t)Wire.read();
    reg_data++;
  }

  return 0;
}

/**
 * The various initialization steps for this sensor.
 *
 */
void HdcInit(void)  {
  HdcReset();
  HdcConfig(HDC1080_MODE_ON);
}

/**
  * Triggers the single transaction read of the T/RH sensor.
  *
 */
bool HdcTriggerRead(void) {
  int8_t status = HdcTransactionOpen(HDC1080_ADDR, HDC_REG_TEMP);

  hdc_next_read = millis() + HDC1080_CONV_TIME;

  if(status) {
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("HdcTriggerRead: failed to open the transaction for HDC_REG_TEMP. Status = %d"), status);

    return false;
  }

  is_reading = true;

  return true;
}

/**
 * Performs a temperature and humidity measurement, and calls
 * the conversion function providing the results in the correct
 * unit according to the device settings.
 *
 * returns: false if something failed during the read process.
 *
 */
bool HdcRead(void) {
  int8_t status = 0;
  uint8_t sensor_data[4];
  uint16_t temp_data = 0;
  uint16_t rh_data = 0;

  is_reading = false;

  status = HdcTransactionClose(HDC1080_ADDR, sensor_data, 4);

  if(status) {
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("HdcRead: failed to read HDC_REG_TEMP. Status = %d"), status);

    return false;
  }

  temp_data = (uint16_t) ((sensor_data[0] << 8) | sensor_data[1]);
  rh_data = (uint16_t) ((sensor_data[2] << 8) | sensor_data[3]);

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("HdcRead: temperature raw data: 0x%04x; humidity raw data: 0x%04x"), temp_data, rh_data);

  // read the temperature from the first 16 bits of the result

  hdc_temperature = ConvertTemp(HDC1080_TEMP_MULT * (float) (temp_data) - HDC1080_TEMP_OFFSET);

  hdc_humidity = HDC1080_RH_MULT * (float) (rh_data);

  if (hdc_humidity > 100) { hdc_humidity = 100.0; }
  if (hdc_humidity < 0) { hdc_humidity = 0.01; }

  ConvertHumidity(hdc_humidity);  // Set global humidity

  hdc_valid = SENSOR_MAX_MISS;

  return true;
}

/**
 * Performs the detection of the HTC1080 sensor.
 *
 */

void HdcDetect(void) {
  if (I2cActive(HDC1080_ADDR)) {
//    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("HdcDetect: Address = 0x%02X already in use."), HDC1080_ADDR);

    return;
  }

  hdc_manufacturer_id = HdcReadManufacturerId();
  hdc_device_id = HdcReadDeviceId();

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("HdcDetect: detected device with manufacturerId = 0x%04X and deviceId = 0x%04X"), hdc_manufacturer_id, hdc_device_id);

  if (hdc_device_id == HDC1080_DEV_ID) {
    HdcInit();
    I2cSetActiveFound(HDC1080_ADDR, hdc_type_name);
  }
}

/**
 * As the name suggests, this function is called every second
 * for performing driver related logic.
 *
 */
void HdcEverySecond(void) {
  if (uptime &1) {  // Every 2 seconds
    if (!HdcTriggerRead()) {
      AddLogMissed((char*) hdc_type_name, hdc_valid);
    }
  }
}

/**
 * Tasmota boilerplate for presenting the sensor data in the web UI, JSON for
 * the MQTT messages, and so on.
 *
 */
void HdcShow(bool json) {
  if (hdc_valid) {
    char temperature[33];

    dtostrfd(hdc_temperature, Settings.flag2.temperature_resolution, temperature);
    char humidity[33];
    dtostrfd(hdc_humidity, Settings.flag2.humidity_resolution, humidity);

    if (json) {
      ResponseAppend_P(JSON_SNS_TEMPHUM, hdc_type_name, temperature, humidity);
#ifdef USE_DOMOTICZ
      if (0 == tele_period) {
        DomoticzTempHumSensor(temperature, humidity);
      }
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
      if (0 == tele_period) {
        KnxSensor(KNX_TEMPERATURE, hdc_temperature);
        KnxSensor(KNX_HUMIDITY, hdc_humidity);
      }
#endif  // USE_KNX
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_TEMP, hdc_type_name, temperature, TempUnit());
      WSContentSend_PD(HTTP_SNS_HUM, hdc_type_name, humidity);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns65(uint8_t function)
{
  if (!I2cEnabled(XI2C_45)) {
//    AddLog_P(LOG_LEVEL_DEBUG, PSTR("Xsns65: I2C driver not enabled for this device."));

    return false;
  }

  bool result = false;

  if (FUNC_INIT == function) {
    HdcDetect();
  }
  else if (hdc_device_id) {
    switch (function) {
      case FUNC_EVERY_50_MSECOND:
        if(is_reading && TimeReached(hdc_next_read)) {
          if(!HdcRead()) {
            AddLogMissed((char*) hdc_type_name, hdc_valid);
          }
        }
        break;
      case FUNC_EVERY_SECOND:
        HdcEverySecond();
        break;
      case FUNC_JSON_APPEND:
        HdcShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        HdcShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_HDC1080
#endif  // USE_I2C

