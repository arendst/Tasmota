/*
  xsns_92_hdc1080.ino - Texas Instruments HDC1080 temperature and humidity sensor support for Tasmota

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

#define XSNS_92             92
#define XI2C_92             92      // See I2CDEVICES.md

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

#define HDC1080_CONV_TIME   25      // Assume 6.50 + 6.35 ms + x of conversion delay for this device
#define HDC1080_TEMP_MULT   0.0025177
#define HDC1080_RH_MULT     0.0025177
#define HDC1080_TEMP_OFFSET 40.0

const char* hdc_type_name = "HDC1080";
uint8_t hdc_address;
uint16_t hdc_manufacturer_id = 0;
uint16_t hdc_device_id = 0;

float hdc_temperature = 0.0;
float hdc_humidity = 0.0;

uint8_t hdc_valid = 0;

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
 * Configures the acquisition mode of the sensor. The
 * HDC1080 supports the acquisition of temperature
 * and humidity in a single I2C transaction.
 * 
 * MODE = 0 -> Temperature or Humidity is acquired.
 * MODE = 1 -> Temperature and Humidity are acquired in sequence, Temperature first
 * 
 */
void HdcSetAcqMode(uint8_t mode) {
  uint16_t current = I2cRead16(HDC1080_ADDR, HDC_REG_CONFIG);

  // bit 12 of the register contains the MODE field
  // so we shift our value to that position and 
  // apply the bit mask to preserve the remaining bits
  // of the register:

  current = (current & 0xEFFF) |  ((uint16_t) (mode << 12));

  I2cWrite16(HDC1080_ADDR, HDC_REG_CONFIG, current);  
}

/**
 * Configures the temperature sampling resolution of the sensor.
 * 
 * This particular device provides two options:
 * 
 * TRES = 0 -> 14 bit resolution
 * TRES = 1 -> 11 bit resolution
 * 
 */
void HdcSetTemperatureResolution(uint8_t resolution) {
  uint16_t current = I2cRead16(HDC1080_ADDR, HDC_REG_CONFIG);
  
  // bit 10 of the register contains the TRES field
  // so we shift our value to that position and 
  // apply the bit mask to preserve the remaining bits
  // of the register:

  current = (current & 0xFBFF) | ((uint16_t) (resolution << 10));

  I2cWrite16(HDC1080_ADDR, HDC_REG_CONFIG, current);
}

/**
 * Configures the humidity sampling resolution of the sensor.
 * 
 * This particular device provides three options:
 * 
 * HRES = 0 -> 14 bit resolution
 * HRES = 1 -> 11 bit resolution
 * HRES = 2 -> 8 bit resolution
 * 
 */
void HdcSetHumidityResolution(uint8_t resolution) {
  uint16_t current = I2cRead16(HDC1080_ADDR, HDC_REG_CONFIG);
  
  // bits 9:8 of the register contain the HRES field
  // so we shift our value to that position and 
  // apply the bit mask to preserve the remaining bits
  // of the register:

  current = (current & 0xFCFF) | ((uint16_t) (resolution << 8));

  I2cWrite16(HDC1080_ADDR, HDC_REG_CONFIG, current);
}

/**
 * Runs the heater in order to reduce the accumulated
 * offset when the sensor is exposed for long periods
 * at high humidity levels.
 * 
 * HEAT = 0 -> heater off
 * HEAT = 1 -> heater on
 * 
 */
void HdcHeater(uint8_t heater) {
  uint16_t current = I2cRead16(HDC1080_ADDR, HDC_REG_CONFIG);
  
  // bits 13 of the configuration  register contains the HEAT flag
  // so we set it according to the value of the heater argument:

  current = (current | 0xDFFF) | ((uint16_t) (heater << 13));

  I2cWrite16(HDC1080_ADDR, HDC_REG_CONFIG, current);
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

  delay(30);  // Not sure how long it takes to reset. Assuming 15ms
}


/**
 * The various initialization steps for this sensor.
 * 
 */
void HdcInit(void)  {
  HdcReset();
  //HdcHeater(HDC1080_HEAT_OFF);
  //HdcSetAcqMode(HDC1080_ACQ_SEQ_ON);
  //HdcSetAcqMode(HDC1080_ACQ_SEQ_OFF);
  //HdcSetTemperatureResolution(HDC1080_MEAS_RES_14);
  //HdcSetHumidityResolution(HDC1080_MEAS_RES_14);
  
  HdcConfig(0);
}

/**
 * Performs a temperature and humidity measurement, and calls
 * the conversion function providing the results in the correct
 * unit according to the device settings.
 * 
 */
bool HdcRead(void) {
  int8_t status = 0;
  //uint16_t sensor_data[2];

  uint16_t sensor_data = 0;

  // In this sensor we must start by performing a write to the 
  // temperature register. This signals the sensor to begin a 
  // measurement:

  Wire.beginTransmission(HDC1080_ADDR);
  Wire.write(HDC_REG_TEMP);

  if (Wire.endTransmission() != 0) { // In case of error
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("HdcRead: failed to write to device for performing acquisition."));

    return false; 
  }           

  delay(HDC1080_CONV_TIME);  // Apply sensor conversion time at max resolution

  // reads the temperature and humidity in a single transaction:

  //status = I2cReadBuffer(HDC1080_ADDR, HDC_REG_TEMP, (uint8_t*) sensor_data, 4);

  sensor_data = I2cRead16(HDC1080_ADDR, HDC_REG_TEMP);

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("HdcRead: temperature raw data: 0x%04x"), sensor_data);

  // status = I2cReadBuffer(HDC1080_ADDR, HDC_REG_TEMP, (uint8_t*) &sensor_data, 2);

/*
  if(status != 0) {
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("HdcRead: failed to read HDC_REG_TEMP. Status = %d"), status);

    return false;
  }
*/
  // read the temperature from the first 16 bits of the result

  //hdc_temperature = ConvertTemp(HDC1080_TEMP_MULT * (float) (sensor_data[0]) - HDC1080_TEMP_OFFSET);

  hdc_temperature = ConvertTemp(HDC1080_TEMP_MULT * (float) (sensor_data) - HDC1080_TEMP_OFFSET);

  //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("HdcRead: temperature successfully converted. Value = %f"), hdc_temperature);

  // read the humidity from the last 16 bits of the result

  sensor_data = I2cRead16(HDC1080_ADDR, HDC_REG_RH);

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("HdcRead: humidity raw data: 0x%04x"), sensor_data);

  //hdc_humidity = HDC1080_RH_MULT * (float) (sensor_data[1]);

  hdc_humidity = HDC1080_RH_MULT * (float) (sensor_data);

  //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("HdcRead: humidity successfully converted. Value = %f"), hdc_humidity);

  if (hdc_humidity > 100) { hdc_humidity = 100.0; }
  if (hdc_humidity < 0) { hdc_humidity = 0.01; }

  ConvertHumidity(hdc_humidity);  // Set global humidity

  hdc_valid = SENSOR_MAX_MISS;

  return true;
}

/********************************************************************************************/

void HdcDetect(void) {
  hdc_address = HDC1080_ADDR;

  if (I2cActive(hdc_address)) { 
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("HdcDetect: Address = 0x%02X already in use."), hdc_address);

    return; 
  }

  hdc_manufacturer_id = HdcReadManufacturerId();
  hdc_device_id = HdcReadDeviceId();

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("HdcDetect: detected device with manufacturerId = 0x%04X and deviceId = 0x%04X"), hdc_manufacturer_id, hdc_device_id);

  if (hdc_device_id == HDC1080_DEV_ID) {
    HdcInit();
    I2cSetActiveFound(hdc_address, hdc_type_name);
  }
}

void HdcEverySecond(void) {
  if (uptime &1) {  // Every 2 seconds
    if (!HdcRead()) {
      AddLogMissed((char*) hdc_type_name, hdc_valid);
    }
  }
}

void HdcShow(bool json) {
  if (hdc_valid) {
    char temperature[33];

    dtostrfd(hdc_temperature, Settings.flag2.temperature_resolution, temperature);
    char humidity[33];
    dtostrfd(hdc_humidity, Settings.flag2.humidity_resolution, humidity);

    if (json) {
      ResponseAppend_P(JSON_SNS_TEMPHUM, hdc_device_id, temperature, humidity);
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

bool Xsns92(uint8_t function)
{
  if (!I2cEnabled(XI2C_92)) { 
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("Xsns92: I2C driver not enabled for this device."));

    return false; 
  }

  bool result = false;

  if (FUNC_INIT == function) {
    HdcDetect();
  }
  else if (hdc_device_id) {
    switch (function) {
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

