#include "Adafruit_CCS811.h"

Adafruit_CCS811::~Adafruit_CCS811(void) {
  if (i2c_dev)
    delete i2c_dev;
}

/**************************************************************************/
/*!
    @brief  Setups the I2C interface and hardware and checks for communication.
    @param  addr Optional I2C address the sensor can be found on. Default is
   0x5A
    @param theWire Optional pointer to I2C interface, &Wire is used by default
    @returns True if device is set up, false on any failure
*/
/**************************************************************************/
bool Adafruit_CCS811::begin(uint8_t addr, TwoWire *theWire) {
  if (i2c_dev)
    delete i2c_dev;
  i2c_dev = new Adafruit_I2CDevice(addr, theWire);
  if (!i2c_dev->begin()) {
    return false;
  }
#ifdef ESP8266
  theWire->setClockStretchLimit(500);
#endif

  SWReset();
  delay(100);

  // check that the HW id is correct
  if (this->read8(CCS811_HW_ID) != CCS811_HW_ID_CODE)
    return false;

  // try to start the app
  this->write(CCS811_BOOTLOADER_APP_START, NULL, 0);
  delay(100);

  // make sure there are no errors and we have entered application mode
  if (checkError())
    return false;
  if (!_status.FW_MODE)
    return false;

  disableInterrupt();

  // default to read every second
  setDriveMode(CCS811_DRIVE_MODE_1SEC);

  return true;
}

/**************************************************************************/
/*!
    @brief  sample rate of the sensor.
    @param  mode one of CCS811_DRIVE_MODE_IDLE, CCS811_DRIVE_MODE_1SEC,
   CCS811_DRIVE_MODE_10SEC, CCS811_DRIVE_MODE_60SEC, CCS811_DRIVE_MODE_250MS.
*/
void Adafruit_CCS811::setDriveMode(uint8_t mode) {
  _meas_mode.DRIVE_MODE = mode;
  this->write8(CCS811_MEAS_MODE, _meas_mode.get());
}

/**************************************************************************/
/*!
    @brief  enable the data ready interrupt pin on the device.
*/
/**************************************************************************/
void Adafruit_CCS811::enableInterrupt() {
  _meas_mode.INT_DATARDY = 1;
  this->write8(CCS811_MEAS_MODE, _meas_mode.get());
}

/**************************************************************************/
/*!
    @brief  disable the data ready interrupt pin on the device
*/
/**************************************************************************/
void Adafruit_CCS811::disableInterrupt() {
  _meas_mode.INT_DATARDY = 0;
  this->write8(CCS811_MEAS_MODE, _meas_mode.get());
}

/**************************************************************************/
/*!
    @brief  checks if data is available to be read.
    @returns True if data is ready, false otherwise.
*/
/**************************************************************************/
bool Adafruit_CCS811::available() {
  _status.set(read8(CCS811_STATUS));
  if (!_status.DATA_READY)
    return false;
  else
    return true;
}

/**************************************************************************/
/*!
    @brief  read and store the sensor data. This data can be accessed with
   getTVOC(), geteCO2(), getCurrentSelected() and getRawADCreading()
    @returns 0 if no error, error code otherwise.
*/
/**************************************************************************/
uint8_t Adafruit_CCS811::readData() {
  if (!available())
    return false;
  else {
    uint8_t buf[8];
    this->read(CCS811_ALG_RESULT_DATA, buf, 8);

    _eCO2 = ((uint16_t)buf[0] << 8) | ((uint16_t)buf[1]);
    _TVOC = ((uint16_t)buf[2] << 8) | ((uint16_t)buf[3]);
    _currentSelected = ((uint16_t)buf[6] >> 2);
    _rawADCreading = ((uint16_t)(buf[6] & 3) << 8) | ((uint16_t)buf[7]);

    if (_status.ERROR)
      return buf[5];

    else
      return 0;
  }
}

/**************************************************************************/
/*!
    @brief  set the humidity and temperature compensation for the sensor.
    @param humidity the humidity data as a percentage. For 55.5% humidity, pass
   in 55.5
    @param temperature the temperature in degrees C as a decimal number.
   For 25.5 degrees C, pass in 25.5
*/
/**************************************************************************/
void Adafruit_CCS811::setEnvironmentalData(float humidity, float temperature) {
  /* Humidity is stored as an unsigned 16 bits in 1/512%RH. The
  default value is 50% = 0x64, 0x00. As an example 48.5%
  humidity would be 0x61, 0x00.*/

  /* Temperature is stored as an unsigned 16 bits integer in 1/512
  degrees; there is an offset: 0 maps to -25°C. The default value is
  25°C = 0x64, 0x00. As an example 23.5% temperature would be
  0x61, 0x00.
  The internal algorithm uses these values (or default values if
  not set by the application) to compensate for changes in
  relative humidity and ambient temperature.*/

  uint16_t hum_conv = humidity * 512.0f + 0.5f;
  uint16_t temp_conv = (temperature + 25.0f) * 512.0f + 0.5f;

  uint8_t buf[] = {
      (uint8_t)((hum_conv >> 8) & 0xFF), (uint8_t)(hum_conv & 0xFF),
      (uint8_t)((temp_conv >> 8) & 0xFF), (uint8_t)(temp_conv & 0xFF)};

  this->write(CCS811_ENV_DATA, buf, 4);
}

/**************************************************************************/
/*!
    @brief  get the current baseline from the sensor.
    @returns the baseline as 16 bit integer. This value is not human readable.
*/
/**************************************************************************/
uint16_t Adafruit_CCS811::getBaseline() {
  /* baseline is not in a human readable format, the two bytes are assembled
  to an uint16_t for easy handling/passing around */

  uint8_t buf[2];

  this->read(CCS811_BASELINE, buf, 2);

  return ((uint16_t)buf[0] << 8) | ((uint16_t)buf[1]);
}

/**************************************************************************/
/*!
    @brief  set the baseline for the sensor.
    @param baseline the baseline to be set. Has to be a value retrieved by
    getBaseline().
*/
/**************************************************************************/
void Adafruit_CCS811::setBaseline(uint16_t baseline) {
  /* baseline is not in a human readable format, byte ordering matches
  getBaseline() */

  uint8_t buf[] = {(uint8_t)((baseline >> 8) & 0xFF),
                   (uint8_t)(baseline & 0xFF)};

  this->write(CCS811_BASELINE, buf, 2);
}

/**************************************************************************/
/*!
    @deprecated hardware support removed by vendor
    @brief  calculate the temperature using the onboard NTC resistor.
    @returns temperature as a double.
*/
/**************************************************************************/
double Adafruit_CCS811::calculateTemperature() {
  uint8_t buf[4];
  this->read(CCS811_NTC, buf, 4);

  uint32_t vref = ((uint32_t)buf[0] << 8) | buf[1];
  uint32_t vntc = ((uint32_t)buf[2] << 8) | buf[3];

  // from ams ccs811 app note
  uint32_t rntc = vntc * CCS811_REF_RESISTOR / vref;

  double ntc_temp;
  ntc_temp = log((double)rntc / CCS811_REF_RESISTOR); // 1
  ntc_temp /= 3380;                                   // 2
  ntc_temp += 1.0 / (25 + 273.15);                    // 3
  ntc_temp = 1.0 / ntc_temp;                          // 4
  ntc_temp -= 273.15;                                 // 5
  return ntc_temp - _tempOffset;
}

/**************************************************************************/
/*!
    @brief  set interrupt thresholds
    @param low_med the level below which an interrupt will be triggered.
    @param med_high the level above which the interrupt will ge triggered.
    @param hysteresis optional histeresis level. Defaults to 50
*/
/**************************************************************************/
void Adafruit_CCS811::setThresholds(uint16_t low_med, uint16_t med_high,
                                    uint8_t hysteresis) {
  uint8_t buf[] = {(uint8_t)((low_med >> 8) & 0xF), (uint8_t)(low_med & 0xF),
                   (uint8_t)((med_high >> 8) & 0xF), (uint8_t)(med_high & 0xF),
                   hysteresis};

  this->write(CCS811_THRESHOLDS, buf, 5);
}

/**************************************************************************/
/*!
    @brief  trigger a software reset of the device
*/
/**************************************************************************/
void Adafruit_CCS811::SWReset() {
  // reset sequence from the datasheet
  uint8_t seq[] = {0x11, 0xE5, 0x72, 0x8A};
  this->write(CCS811_SW_RESET, seq, 4);
}

/**************************************************************************/
/*!
    @brief   read the status register and store any errors.
    @returns the error bits from the status register of the device.
*/
/**************************************************************************/
bool Adafruit_CCS811::checkError() {
  _status.set(read8(CCS811_STATUS));
  return _status.ERROR;
}

/**************************************************************************/
/*!
    @brief  write one byte of data to the specified register
    @param  reg the register to write to
    @param  value the value to write
*/
/**************************************************************************/
void Adafruit_CCS811::write8(byte reg, byte value) {
  this->write(reg, &value, 1);
}

/**************************************************************************/
/*!
    @brief  read one byte of data from the specified register
    @param  reg the register to read
    @returns one byte of register data
*/
/**************************************************************************/
uint8_t Adafruit_CCS811::read8(byte reg) {
  uint8_t ret;
  this->read(reg, &ret, 1);

  return ret;
}

void Adafruit_CCS811::read(uint8_t reg, uint8_t *buf, uint8_t num) {
  uint8_t buffer[1] = {reg};
  i2c_dev->write_then_read(buffer, 1, buf, num);
}

void Adafruit_CCS811::write(uint8_t reg, uint8_t *buf, uint8_t num) {
  uint8_t prefix[1] = {reg};
  i2c_dev->write(buf, num, true, prefix, 1);
}
