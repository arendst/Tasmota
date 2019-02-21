/***************************************************************************
  This is a library for the BME680 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME680 Breakout
  ----> http://www.adafruit.com/products/2650

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/
#include "Arduino.h"
#include "Adafruit_BME680.h"

//#define BME680_DEBUG

// must be global in order to work with underlying library
int8_t _BME680_SoftwareSPI_MOSI, _BME680_SoftwareSPI_MISO, _BME680_SoftwareSPI_SCK;

// Our hardware interface functions
static int8_t i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
static int8_t i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
static int8_t spi_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
static int8_t spi_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
static uint8_t spi_transfer(uint8_t x);
static void delay_msec(uint32_t ms);

/***************************************************************************
 PUBLIC FUNCTIONS
 ***************************************************************************/

/**************************************************************************/
/*!
    @brief  Instantiates sensor with Hardware SPI or I2C.
    @param  cspin SPI chip select. If not passed in, I2C will be used
*/
/**************************************************************************/
Adafruit_BME680::Adafruit_BME680(int8_t cspin)
  : _cs(cspin)
{
  _BME680_SoftwareSPI_MOSI = -1;
  _BME680_SoftwareSPI_MISO = -1;
  _BME680_SoftwareSPI_SCK = -1;
  _filterEnabled = _tempEnabled = _humEnabled = _presEnabled = _gasEnabled = false;
}


/**************************************************************************/
/*!
    @brief  Instantiates sensor with Software (bit-bang) SPI.
    @param  cspin SPI chip select
    @param  mosipin SPI MOSI (Data from microcontroller to sensor)
    @param  misopin SPI MISO (Data to microcontroller from sensor)
    @param  sckpin SPI Clock
*/
/**************************************************************************/
Adafruit_BME680::Adafruit_BME680(int8_t cspin, int8_t mosipin, int8_t misopin, int8_t sckpin)
  : _cs(cspin)
{
  _BME680_SoftwareSPI_MOSI = mosipin;
  _BME680_SoftwareSPI_MISO = misopin;
  _BME680_SoftwareSPI_SCK = sckpin;
  _filterEnabled = _tempEnabled = _humEnabled = _presEnabled = _gasEnabled = false;
}



/**************************************************************************/
/*!
    @brief Initializes the sensor

    Hardware ss initialized, verifies it is in the I2C or SPI bus, then reads
    calibration data in preparation for sensor reads.

    @param  addr Optional parameter for the I2C address of BME680. Default is 0x77
    @return True on sensor initialization success. False on failure.
*/
/**************************************************************************/
bool Adafruit_BME680::begin(uint8_t addr) {
  _i2caddr = addr;

  if (_cs == -1) {
    // i2c
    Wire.begin();

    gas_sensor.dev_id = addr;
    gas_sensor.intf = BME680_I2C_INTF;
    gas_sensor.read = &i2c_read;
    gas_sensor.write = &i2c_write;
  } else {
    digitalWrite(_cs, HIGH);
    pinMode(_cs, OUTPUT);

    if (_BME680_SoftwareSPI_SCK == -1) {
      // hardware SPI
      SPI.begin();
    } else {
      // software SPI
      pinMode(_BME680_SoftwareSPI_SCK, OUTPUT);
      pinMode(_BME680_SoftwareSPI_MOSI, OUTPUT);
      pinMode(_BME680_SoftwareSPI_MISO, INPUT);
    }

    gas_sensor.dev_id = _cs;
    gas_sensor.intf = BME680_SPI_INTF;
    gas_sensor.read = &spi_read;
    gas_sensor.write = &spi_write;
  }

  gas_sensor.delay_ms = delay_msec;

  int8_t rslt = BME680_OK;
  rslt = bme680_init(&gas_sensor);
#ifdef BME680_DEBUG
  Serial.print("Result: "); Serial.println(rslt);
#endif

  if (rslt != BME680_OK) 
    return false;

#ifdef BME680_DEBUG
  Serial.print("T1 = "); Serial.println(gas_sensor.calib.par_t1);
  Serial.print("T2 = "); Serial.println(gas_sensor.calib.par_t2);
  Serial.print("T3 = "); Serial.println(gas_sensor.calib.par_t3);
  Serial.print("P1 = "); Serial.println(gas_sensor.calib.par_p1);
  Serial.print("P2 = "); Serial.println(gas_sensor.calib.par_p2);
  Serial.print("P3 = "); Serial.println(gas_sensor.calib.par_p3);
  Serial.print("P4 = "); Serial.println(gas_sensor.calib.par_p4);
  Serial.print("P5 = "); Serial.println(gas_sensor.calib.par_p5);
  Serial.print("P6 = "); Serial.println(gas_sensor.calib.par_p6);
  Serial.print("P7 = "); Serial.println(gas_sensor.calib.par_p7);
  Serial.print("P8 = "); Serial.println(gas_sensor.calib.par_p8);
  Serial.print("P9 = "); Serial.println(gas_sensor.calib.par_p9);
  Serial.print("P10 = "); Serial.println(gas_sensor.calib.par_p10);
  Serial.print("H1 = "); Serial.println(gas_sensor.calib.par_h1);
  Serial.print("H2 = "); Serial.println(gas_sensor.calib.par_h2);
  Serial.print("H3 = "); Serial.println(gas_sensor.calib.par_h3);
  Serial.print("H4 = "); Serial.println(gas_sensor.calib.par_h4);
  Serial.print("H5 = "); Serial.println(gas_sensor.calib.par_h5);
  Serial.print("H6 = "); Serial.println(gas_sensor.calib.par_h6);
  Serial.print("H7 = "); Serial.println(gas_sensor.calib.par_h7);
  Serial.print("G1 = "); Serial.println(gas_sensor.calib.par_gh1);
  Serial.print("G2 = "); Serial.println(gas_sensor.calib.par_gh2);
  Serial.print("G3 = "); Serial.println(gas_sensor.calib.par_gh3);
  Serial.print("G1 = "); Serial.println(gas_sensor.calib.par_gh1);
  Serial.print("G2 = "); Serial.println(gas_sensor.calib.par_gh2);
  Serial.print("G3 = "); Serial.println(gas_sensor.calib.par_gh3);
  Serial.print("Heat Range = "); Serial.println(gas_sensor.calib.res_heat_range);
  Serial.print("Heat Val = "); Serial.println(gas_sensor.calib.res_heat_val);
  Serial.print("SW Error = "); Serial.println(gas_sensor.calib.range_sw_err);
#endif

  setTemperatureOversampling(BME680_OS_8X);
  setHumidityOversampling(BME680_OS_2X);
  setPressureOversampling(BME680_OS_4X);
  setIIRFilterSize(BME680_FILTER_SIZE_3);
  setGasHeater(320, 150); // 320*C for 150 ms
  
  // don't do anything till we request a reading
  gas_sensor.power_mode = BME680_FORCED_MODE;

  return true;
}



/**************************************************************************/
/*!
    @brief Performs a reading and returns the ambient temperature.
    @return Temperature in degrees Centigrade
*/
/**************************************************************************/
float Adafruit_BME680::readTemperature(void) {
  performReading();
  return temperature;
}


/**************************************************************************/
/*!
    @brief Performs a reading and returns the barometric pressure.
    @return Barometic pressure in Pascals
*/
/**************************************************************************/
float Adafruit_BME680::readPressure(void) {
  performReading();
  return pressure;
}


/**************************************************************************/
/*!
    @brief Performs a reading and returns the relative humidity.
    @return Relative humidity as floating point
*/
/**************************************************************************/
float Adafruit_BME680::readHumidity(void) {
  performReading();
  return humidity;
}

/**************************************************************************/
/*!
    @brief Calculates the resistance of the MOX gas sensor. 
    @return Resistance in Ohms
*/
/**************************************************************************/
uint32_t Adafruit_BME680::readGas(void) {
  performReading();
  return gas_resistance;
}


/**************************************************************************/
/*!
    @brief Calculates the altitude (in meters). 

    Reads the current atmostpheric pressure (in hPa) from the sensor and calculates
    via the provided sea-level pressure (in hPa).

    @param  seaLevel      Sea-level pressure in hPa
    @return Altitude in meters
*/
/**************************************************************************/
float Adafruit_BME680::readAltitude(float seaLevel)
{
    // Equation taken from BMP180 datasheet (page 16):
    //  http://www.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf

    // Note that using the equation from wikipedia can give bad results
    // at high altitude. See this thread for more information:
    //  http://forums.adafruit.com/viewtopic.php?f=22&t=58064

    float atmospheric = readPressure() / 100.0F;
    return 44330.0 * (1.0 - pow(atmospheric / seaLevel, 0.1903));
}

/**************************************************************************/
/*!
    @brief Performs a full reading of all 4 sensors in the BME680. 

    Assigns the internal Adafruit_BME680#temperature, Adafruit_BME680#pressure, Adafruit_BME680#humidity 
    and Adafruit_BME680#gas_resistance member variables

    @return True on success, False on failure
*/
/**************************************************************************/
bool Adafruit_BME680::performReading(void) {
  uint8_t set_required_settings = 0;
  struct bme680_field_data data;
  int8_t rslt;

  /* Select the power mode */
  /* Must be set before writing the sensor configuration */
  gas_sensor.power_mode = BME680_FORCED_MODE;

  /* Set the required sensor settings needed */
  if (_tempEnabled)
    set_required_settings |= BME680_OST_SEL;
  if (_humEnabled)
    set_required_settings |= BME680_OSH_SEL;
  if (_presEnabled)
    set_required_settings |= BME680_OSP_SEL;
  if (_filterEnabled)
    set_required_settings |= BME680_FILTER_SEL;
  if (_gasEnabled) 
    set_required_settings |= BME680_GAS_SENSOR_SEL;

  /* Set the desired sensor configuration */
  //Serial.println("Setting sensor settings");
  rslt = bme680_set_sensor_settings(set_required_settings, &gas_sensor);
  if (rslt != BME680_OK) 
    return false;
  
  /* Set the power mode */
  //Serial.println("Setting power mode");
  rslt = bme680_set_sensor_mode(&gas_sensor);
  if (rslt != BME680_OK) 
    return false;

  /* Get the total measurement duration so as to sleep or wait till the
   * measurement is complete */
  uint16_t meas_period;
  bme680_get_profile_dur(&meas_period, &gas_sensor);
  //Serial.print("Waiting (ms) "); Serial.println(meas_period);
  delay(meas_period * 2); /* Delay till the measurement is ready */
  
  //Serial.print("t_fine = "); Serial.println(gas_sensor.calib.t_fine);

  //Serial.println("Getting sensor data");
  rslt = bme680_get_sensor_data(&data, &gas_sensor);
  if (rslt != BME680_OK) 
    return false;

  if (_tempEnabled) {
    //Serial.print("Temp: "); Serial.println(data.temperature / 100.0, 2);
    temperature = data.temperature / 100.0;
  } else {
    temperature = NAN;
  }

  if (_humEnabled) {
    //Serial.print("Hum:  "); Serial.println(data.humidity / 1000.0, 2);
    humidity = data.humidity / 1000.0;
  } else {
    humidity = NAN;
  }

  if (_presEnabled) {
    //Serial.print("Pres: "); Serial.println(data.pressure / 100.0, 2);
    pressure = data.pressure;
  } else {
    pressure = NAN;
  }

  /* Avoid using measurements from an unstable heating setup */
  if (_gasEnabled) {
    if (data.status & BME680_HEAT_STAB_MSK) {
      //Serial.print("Gas resistance: "); Serial.println(data.gas_resistance);
      gas_resistance = data.gas_resistance;
    } else {
      gas_resistance = 0;
      //Serial.println("Gas reading unstable!");
    }
  }

  return true;
}

/**************************************************************************/
/*!
    @brief  Enable and configure gas reading + heater
    @param  heaterTemp Desired temperature in degrees Centigrade
    @param  heaterTime Time to keep heater on in milliseconds
    @return True on success, False on failure
*/
/**************************************************************************/
bool Adafruit_BME680::setGasHeater(uint16_t heaterTemp, uint16_t heaterTime) {
  gas_sensor.gas_sett.heatr_temp = heaterTemp;
  gas_sensor.gas_sett.heatr_dur = heaterTime;

  if ( (heaterTemp == 0) || (heaterTime == 0) ) {
    // disabled!
    gas_sensor.gas_sett.run_gas = BME680_DISABLE_GAS_MEAS;
    _gasEnabled = false;
  } else {
    gas_sensor.gas_sett.run_gas = BME680_ENABLE_GAS_MEAS;
    _gasEnabled = true;
  }
  return true;
}


/**************************************************************************/
/*!
    @brief  Setter for Temperature oversampling
    @param  oversample Oversampling setting, can be BME680_OS_NONE (turn off Temperature reading), 
    BME680_OS_1X, BME680_OS_2X, BME680_OS_4X, BME680_OS_8X or BME680_OS_16X
    @return True on success, False on failure
*/
/**************************************************************************/

bool Adafruit_BME680::setTemperatureOversampling(uint8_t oversample) {
  if (oversample > BME680_OS_16X) return false;

  gas_sensor.tph_sett.os_temp = oversample;

  if (oversample == BME680_OS_NONE)
    _tempEnabled = false;
  else
    _tempEnabled = true;

  return true;
}


/**************************************************************************/
/*!
    @brief  Setter for Humidity oversampling
    @param  oversample Oversampling setting, can be BME680_OS_NONE (turn off Humidity reading), 
    BME680_OS_1X, BME680_OS_2X, BME680_OS_4X, BME680_OS_8X or BME680_OS_16X
    @return True on success, False on failure
*/
/**************************************************************************/

bool Adafruit_BME680::setHumidityOversampling(uint8_t oversample) {
  if (oversample > BME680_OS_16X) return false;

  gas_sensor.tph_sett.os_hum = oversample;

  if (oversample == BME680_OS_NONE)
    _humEnabled = false;
  else
    _humEnabled = true;

  return true;
}


/**************************************************************************/
/*!
    @brief  Setter for Pressure oversampling
    @param  oversample Oversampling setting, can be BME680_OS_NONE (turn off Pressure reading), 
    BME680_OS_1X, BME680_OS_2X, BME680_OS_4X, BME680_OS_8X or BME680_OS_16X
    @return True on success, False on failure
*/
/**************************************************************************/
bool Adafruit_BME680::setPressureOversampling(uint8_t oversample) {
  if (oversample > BME680_OS_16X) return false;

  gas_sensor.tph_sett.os_pres = oversample;

  if (oversample == BME680_OS_NONE)
    _presEnabled = false;
  else
    _presEnabled = true;

  return true;
}

/**************************************************************************/
/*!
    @brief  Setter for IIR filter.
    @param filtersize Size of the filter (in samples). Can be BME680_FILTER_SIZE_0 (no filtering), BME680_FILTER_SIZE_1, BME680_FILTER_SIZE_3, BME680_FILTER_SIZE_7, BME680_FILTER_SIZE_15, BME680_FILTER_SIZE_31, BME680_FILTER_SIZE_63, BME680_FILTER_SIZE_127
    @return True on success, False on failure
    
*/
/**************************************************************************/
bool Adafruit_BME680::setIIRFilterSize(uint8_t filtersize) {
  if (filtersize > BME680_FILTER_SIZE_127) return false;

  gas_sensor.tph_sett.filter = filtersize;

  if (filtersize == BME680_FILTER_SIZE_0)
    _filterEnabled = false;
  else
    _filterEnabled = true;

  return true;
}

/**************************************************************************/
/*!
    @brief  Reads 8 bit values over I2C
*/
/**************************************************************************/
int8_t i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len) {
#ifdef BME680_DEBUG
  Serial.print("\tI2C $"); Serial.print(reg_addr, HEX); Serial.print(" => ");
#endif

  Wire.beginTransmission((uint8_t)dev_id);
  Wire.write((uint8_t)reg_addr);
  Wire.endTransmission();
  if (len != Wire.requestFrom((uint8_t)dev_id, (byte)len)) {
#ifdef BME680_DEBUG
    Serial.print("Failed to read "); Serial.print(len); Serial.print(" bytes from "); Serial.println(dev_id, HEX);
#endif
    return 1;
  }
  while (len--) {
    *reg_data = (uint8_t)Wire.read();
#ifdef BME680_DEBUG
    Serial.print("0x"); Serial.print(*reg_data, HEX); Serial.print(", ");
#endif
    reg_data++;
  }
#ifdef BME680_DEBUG
  Serial.println("");
#endif
  return 0;
}

/**************************************************************************/
/*!
    @brief  Writes 8 bit values over I2C
*/
/**************************************************************************/
int8_t i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len) {
#ifdef BME680_DEBUG
  Serial.print("\tI2C $"); Serial.print(reg_addr, HEX); Serial.print(" <= ");
#endif
  Wire.beginTransmission((uint8_t)dev_id);
  Wire.write((uint8_t)reg_addr);
  while (len--) {
    Wire.write(*reg_data);
#ifdef BME680_DEBUG
    Serial.print("0x"); Serial.print(*reg_data, HEX); Serial.print(", ");
#endif
    reg_data++;
  }
  Wire.endTransmission();
#ifdef BME680_DEBUG
  Serial.println("");
#endif
  return 0;
}



/**************************************************************************/
/*!
    @brief  Reads 8 bit values over SPI
*/
/**************************************************************************/
static int8_t spi_read(uint8_t cspin, uint8_t reg_addr, uint8_t *reg_data, uint16_t len) {
#ifdef BME680_DEBUG
  Serial.print("\tSPI $"); Serial.print(reg_addr, HEX); Serial.print(" => ");
#endif

  digitalWrite(cspin, LOW);

  // If hardware SPI we should use transactions!
  if (_BME680_SoftwareSPI_SCK == -1) {
    SPI.beginTransaction(SPISettings(BME680_DEFAULT_SPIFREQ, MSBFIRST, SPI_MODE0));
  }

  spi_transfer(reg_addr);

  while (len--) {
    *reg_data = spi_transfer(0x00);
#ifdef BME680_DEBUG
    Serial.print("0x"); Serial.print(*reg_data, HEX); Serial.print(", ");
#endif
    reg_data++;
  }

  if (_BME680_SoftwareSPI_SCK == -1) {
    SPI.endTransaction();
  }

  digitalWrite(cspin, HIGH);

#ifdef BME680_DEBUG
  Serial.println("");
#endif
  return 0;
}

/**************************************************************************/
/*!
    @brief  Writes 8 bit values over SPI
*/
/**************************************************************************/
static int8_t spi_write(uint8_t cspin, uint8_t reg_addr, uint8_t *reg_data, uint16_t len) {
#ifdef BME680_DEBUG
  Serial.print("\tSPI $"); Serial.print(reg_addr, HEX); Serial.print(" <= ");
#endif

  digitalWrite(cspin, LOW);

  // If hardware SPI we should use transactions!
  if (_BME680_SoftwareSPI_SCK == -1) {
    SPI.beginTransaction(SPISettings(BME680_DEFAULT_SPIFREQ, MSBFIRST, SPI_MODE0));
  }

  spi_transfer(reg_addr);
  while (len--) {
    spi_transfer(*reg_data);
#ifdef BME680_DEBUG
    Serial.print("0x"); Serial.print(*reg_data, HEX); Serial.print(", ");
#endif
    reg_data++;
  }

  if (_BME680_SoftwareSPI_SCK == -1) {
    SPI.endTransaction();
  }

  digitalWrite(cspin, HIGH);

#ifdef BME680_DEBUG
  Serial.println("");
#endif
  return 0;
}


static uint8_t spi_transfer(uint8_t x) {
  if (_BME680_SoftwareSPI_SCK == -1)
    return SPI.transfer(x);

  // software spi
  //Serial.println("Software SPI");
  uint8_t reply = 0;
  for (int i=7; i>=0; i--) {
    reply <<= 1;
    digitalWrite(_BME680_SoftwareSPI_SCK, LOW);
    digitalWrite(_BME680_SoftwareSPI_MOSI, x & (1<<i));
    digitalWrite(_BME680_SoftwareSPI_SCK, HIGH);
    if (digitalRead(_BME680_SoftwareSPI_MISO))
      reply |= 1;
  }
  return reply;
}


static void delay_msec(uint32_t ms){
  delay(ms);
}
