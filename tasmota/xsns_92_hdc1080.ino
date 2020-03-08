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

#define HDC1080_HEAT_OFF    0x00
#define HDC1080_HEAT_ON     0x01
#define HDC1080_ACQ_SEQ_ON  1
#define HDC1080_ACQ_SEQ_OFF 0
#define HDC1080_MEAS_RES_14 0x00
#define HDC1080_MEAS_RES_11 0x01
#define HDC1080_MEAS_RES_8  0x02

#define HDC1080_CONV_TIME   15      // Assume 6.50 + 6.35 ms + x of conversion delay for this device
#define HDC1080_TEMP_MULT   0.0025177
#define HDC1080_RH_MULT     0.0025177
#define HDC1080_TEMP_OFFSET 40

const char kHdcTypes[] PROGMEM = "HDC1080";

uint8_t hdc_address;
uint8_t hdc_type = 0;

float hdc_temperature = 0;
float hdc_humidity = 0;
uint8_t hdc_valid = 0;
char hdc_types[1];

/**
 * Reads the device ID register.
 * 
 */
uint16_t HdcReadDeviceId(void) {
  uint16_t deviceID = 0;

  deviceID = I2cRead16(HDC1080_ADDR, HDC_REG_DEV_ID);

  return deviceID;
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
void HdcSetAcqMode(bool mode) {
  uint16_t current = I2cRead16(HDC1080_ADDR, HDC_REG_CONFIG);

  // bit 12 of the register contains the MODE field
  // so we shift our value to that position and 
  // apply the bit mask to preserve the remaining bits
  // of the register:

  current &= (uint16_t) ((mode << 12) | 0xEFFF);

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

  current &= (uint16_t) ((resolution << 10) | 0xFBFF);

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

  current &= (uint16_t) ((resolution << 8) | 0xFCFF);

  I2cWrite16(HDC1080_ADDR, HDC_REG_CONFIG, current);
}

/**
 * Performs a soft reset on the device.
 * 
 * RST = 1 -> software reset
 * 
 */
void HdcReset(void) {
  uint16_t current = I2cRead16(HDC1080_ADDR, HDC_REG_CONFIG);
  
  // bits 9:8 of the register contain the RST flag
  // so we set it to 1:

  current |= 0x8000;

  I2cWrite16(HDC1080_ADDR, HDC_REG_CONFIG, current);

  delay(15);  // Not sure how long it takes to reset. Assuming 15ms
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
  
  // bits 13 of the register contain the HEAT flag
  // so we set it according to the value of the heater argument:

  current &= (uint16_t) ((heater << 13) | 0xDFFF);

  I2cWrite16(HDC1080_ADDR, HDC_REG_CONFIG, current);
}

void HdcInit(void)
{
  HdcReset();
  HdcHeater(HDC1080_HEAT_OFF);
  HdcSetAcqMode(HDC1080_ACQ_SEQ_ON);
  HdcSetTemperatureResolution(HDC1080_MEAS_RES_14);
  HdcSetHumidityResolution(HDC1080_MEAS_RES_14);
}

/**
 * Performs a temperature and humidity measurement, and calls
 * the conversion function providing the results in the correct
 * unit according to the device settings.
 * 
 */
bool HdcRead(void) {
  int8_t status = 0;
  uint16_t sensor_data[2];

  // In this sensor we must start by performing a write to the 
  // temperature register. This signals the sensor to begin a 
  // measurement:

  // TODO initialize the measurement mode and 
  // read both registers in a single transaction:

  Wire.beginTransmission(HDC1080_ADDR);
  Wire.write(HDC_REG_TEMP);

  if (Wire.endTransmission() != 0) { // In case of error
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("HdcRead: failed to write to device for performing acquisition."));

    return false; 
  }           

  delay(HDC1080_CONV_TIME);                                    // Sensor time at max resolution

  // reads the temperature and humidity in a single transaction:

  status = I2cReadBuffer(HDC1080_ADDR, HDC_REG_TEMP, (uint8_t*) sensor_data, 4);

  if(status != 0) {
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("HdcRead: failed to read HDC_REG_TEMP. Status = %d"), status);

    return false;
  }

  // read the temperature from the first 16 bits of the result

  hdc_temperature = ConvertTemp(HDC1080_TEMP_MULT * (float) (sensor_data[0]) - HDC1080_TEMP_OFFSET);

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("HdcRead: temperature successfully converted. Value = %f"), hdc_temperature);

  // read the humidity from the last 16 bits of the result

  hdc_humidity = HDC1080_RH_MULT * (float) (sensor_data[1]);

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("HdcRead: humidity successfully converted. Value = %f"), hdc_humidity);

  if (hdc_humidity > 100) { hdc_humidity = 100.0; }

  if (hdc_humidity < 0) { hdc_humidity = 0.01; }

  ConvertHumidity(hdc_humidity);  // Set global humidity

  hdc_valid = SENSOR_MAX_MISS;

  return true;
}

/********************************************************************************************/

void HdcDetect(void)
{
  hdc_address = HDC1080_ADDR;

  if (I2cActive(hdc_address)) { 
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("HdcDetect: Address = 0x%02X already in use."), hdc_address);

    return; 
  }

  hdc_type = HdcReadDeviceId();

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("HdcDetect: detected device with id = 0x%04X"), hdc_type);

  if (hdc_type == HDC1080_DEV_ID) {
    HdcInit();
    GetTextIndexed(hdc_types, sizeof(hdc_types), 0, kHdcTypes);
    I2cSetActiveFound(hdc_address, hdc_types);
  }
}

void HdcEverySecond(void) {
  if (uptime &1) {  // Every 2 seconds
    if (!HdcRead()) {
      AddLogMissed(hdc_types, hdc_valid);
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
      ResponseAppend_P(JSON_SNS_TEMPHUM, hdc_type, temperature, humidity);
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
      WSContentSend_PD(HTTP_SNS_TEMP, hdc_types, temperature, TempUnit());
      WSContentSend_PD(HTTP_SNS_HUM, hdc_types, humidity);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns92(uint8_t function)
{
  if (!I2cEnabled(XI2C_92)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    HdcDetect();
  }
  else if (hdc_type) {
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

