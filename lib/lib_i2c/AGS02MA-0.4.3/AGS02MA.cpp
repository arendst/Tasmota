//
//    FILE: AGS02MA.cpp
//  AUTHOR: Rob Tillaart, Viktor Balint, Beanow
//    DATE: 2021-08-12
// VERSION: 0.4.3
// PURPOSE: Arduino library for AGS02MA TVOC sensor
//     URL: https://github.com/RobTillaart/AGS02MA


#include "AGS02MA.h"


//  REGISTERS
#define AGS02MA_DATA                  0x00
#define AGS02MA_CALIBRATION           0x01
#define AGS02MA_VERSION               0x11
#define AGS02MA_SLAVE_ADDRESS         0x21



AGS02MA::AGS02MA(const uint8_t deviceAddress, TwoWire *wire)
{
  _address = deviceAddress;
  _wire    = wire;
  reset();
}


bool AGS02MA::begin()
{
  _startTime = millis();  //  PREHEAT TIMING
  return isConnected();
}


bool AGS02MA::isConnected()
{
  _setI2CLowSpeed();
  _wire->beginTransmission(_address);
  bool rv = ( _wire->endTransmission(true) == 0);
  _setI2CHighSpeed();
  return rv;
}


void AGS02MA::reset()
{
  _I2CResetSpeed = 100000;
  _startTime     = millis();
  _lastRead      = 0;
  _lastPPB       = 0;
  _mode          = 255;
  _status        = AGS02MA_OK;
  _error         = AGS02MA_OK;
}


bool AGS02MA::setAddress(const uint8_t deviceAddress)
{
  if ((deviceAddress < 10) or (deviceAddress > 119)) return false;
  _buffer[2] = _buffer[0] = deviceAddress;
  _buffer[3] = _buffer[1] = 0xFF ^ deviceAddress;
  _buffer[4] = _CRC8(_buffer, 4);
  if (_writeRegister(AGS02MA_SLAVE_ADDRESS))
  {
    _address = deviceAddress;
  }
  return isConnected();
}


uint8_t AGS02MA::getSensorVersion()
{
  uint8_t version = 0xFF;
  if (_readRegister(AGS02MA_VERSION))
  {
    // for (int i = 0; i < 5; i++)
    // {
      // Serial.print(_buffer[i]);
      // Serial.print('\t');
    // }
    // Serial.println();
    // unclear what the other bytes have for information.
    // datasheet names these 3 bytes as KEEP.
    // BUFFER        VALUE  MEANING
    // buffer [0] == 20     year ?
    // buffer [1] == 07     month ?
    // buffer [2] == 28     day ?
    // buffer [3] == 117    VERSION
    // buffer [4] ==        CRC
    version = _buffer[3];
    if (_CRC8(_buffer, 5) != 0)
    {
      _error = AGS02MA_ERROR_CRC;
    }
  }
  return version;
}


uint32_t AGS02MA::getSensorDate()
{
  uint32_t date = 0xFFFFFFFF;
  if (_readRegister(AGS02MA_VERSION))
  {
    date = 0x20;
    date <<= 8;
    date += _bin2bcd(_buffer[0]);
    date <<= 8;
    date += _bin2bcd(_buffer[1]);
    date <<= 8;
    date += _bin2bcd(_buffer[2]);
    //  version = _buffer[3];
    if (_CRC8(_buffer, 5) != 0)
    {
      _error = AGS02MA_ERROR_CRC;
    }
  }
  return date;
}


bool AGS02MA::setPPBMode()
{
  _buffer[0] = 0x00;
  _buffer[1] = 0xFF;
  _buffer[2] = 0x00;
  _buffer[3] = 0xFF;
  _buffer[4] = 0x30;
  if (_writeRegister(AGS02MA_DATA))
  {
    _mode = 0;
    return true;
  }
  return false;
}


bool AGS02MA::setUGM3Mode()
{
  _buffer[0] = 0x02;
  _buffer[1] = 0xFD;
  _buffer[2] = 0x02;
  _buffer[3] = 0xFD;
  _buffer[4] = 0x00;
  if (_writeRegister(AGS02MA_DATA))
  {
    _mode = 1;
    return true;
  }
  return false;
}


uint32_t AGS02MA::readPPB()
{
  uint32_t value = _readSensor();
  if (_error == AGS02MA_OK)
  {
    _lastRead = millis();
    _lastPPB = value;
  }
  else
  {
    value = _lastPPB;
  }
  return value;
}


uint32_t AGS02MA::readUGM3()
{
  uint32_t value = _readSensor();
  if (_error == AGS02MA_OK)
  {
    _lastRead = millis();
    _lastUGM3 = value;
  }
  else
  {
    value = _lastUGM3;
  }
  return value;
}


bool AGS02MA::manualZeroCalibration(uint16_t value)
{
  _buffer[0] = 0x00;
  _buffer[1] = 0x0C;
  _buffer[2] = (uint8_t) (value >> 8);
  _buffer[3] = (uint8_t) (value & 0x00FF);
  _buffer[4] = _CRC8(_buffer, 4);
  return _writeRegister(AGS02MA_CALIBRATION);
}


bool AGS02MA::getZeroCalibrationData(AGS02MA::ZeroCalibrationData &data) {
  if (!_readRegister(AGS02MA_CALIBRATION))
  {
    return false;
  }

  if (_CRC8(_buffer, 5) != 0)
  {
    _error = AGS02MA_ERROR_CRC;
    return false;
  }

  _error = AGS02MA_OK;
  //  Don't pollute the struct given to us, until we've handled all error cases.
  data.status = _getDataMSB();
  data.value = _getDataLSB();
  return true;
}


int AGS02MA::lastError()
{
  int e = _error;
  _error = AGS02MA_OK;  //  reset error after read
  return e;
}


bool AGS02MA::readRegister(uint8_t address, AGS02MA::RegisterData &reg) {
  if (!_readRegister(address))
  {
    return false;
  }

  if (_CRC8(_buffer, 5) != 0)
  {
    _error = AGS02MA_ERROR_CRC;
    return false;
  }

  _error = AGS02MA_OK;
  //  Don't pollute the struct given to us, until we've handled all error cases.
  reg.data[0] = _buffer[0];
  reg.data[1] = _buffer[1];
  reg.data[2] = _buffer[2];
  reg.data[3] = _buffer[3];
  reg.crc = _buffer[4];
  reg.crcValid = true;  //  checked above.
  return true;
}


/////////////////////////////////////////////////////////
//
//  PRIVATE
//
uint32_t AGS02MA::_readSensor()
{
  uint32_t value = 0;
  if (_readRegister(AGS02MA_DATA))
  {
    _error = AGS02MA_OK;
    _status = _buffer[0];
    if (_status & 0x01)
    {
      _error = AGS02MA_ERROR_NOT_READY;
    }
    value =  _buffer[1] * 65536UL;
    value += _buffer[2] * 256;
    value += _buffer[3];
    if (_CRC8(_buffer, 5) != 0)
    {
      _error = AGS02MA_ERROR_CRC;
    }
  }
  return value;
}


bool AGS02MA::_readRegister(uint8_t reg)
{
  while (millis() - _lastRegTime < 30) yield();

  _setI2CLowSpeed();

  _wire->beginTransmission(_address);
  _wire->write(reg);
  _error = _wire->endTransmission(true);
  if (_error != 0)
  {
    //  _error will be I2C error code
    _setI2CHighSpeed();
    return false;
  }
  //  TODO investigate async interface
  delay(30);

  if (_wire->requestFrom(_address, (uint8_t)5) != 5)
  {
    _error = AGS02MA_ERROR_READ;
    _setI2CHighSpeed();
    return false;
  }
  for (uint8_t i = 0; i < 5; i++)
  {
    _buffer[i] = _wire->read();
  }
  _error = AGS02MA_OK;
  _setI2CHighSpeed();
  return true;
}


bool AGS02MA::_writeRegister(uint8_t reg)
{
  while (millis() - _lastRegTime < 30) yield();
  _lastRegTime = millis();

  _setI2CLowSpeed();

  _wire->beginTransmission(_address);
  _wire->write(reg);
  for (uint8_t i = 0; i < 5; i++)
  {
    _wire->write(_buffer[i]);
  }
  _error = _wire->endTransmission(true);
  _setI2CHighSpeed();
  return (_error == 0);
}


void AGS02MA::_setI2CLowSpeed()
{
#if defined (__AVR__)
  //  TWBR = 255;    //  == 30.4 KHz with TWSR = 0x00
  TWBR = 78;         //  == 25.0 KHZ
  TWSR = 0x01;       //  pre-scaler = 4
#else
  _wire->setClock(AGS02MA_I2C_CLOCK);
#endif
}


void AGS02MA::_setI2CHighSpeed()
{
#if defined (__AVR__)
  TWSR = 0x00;
#endif
  _wire->setClock(_I2CResetSpeed);
}


uint16_t AGS02MA::_getDataMSB()
{
  return (_buffer[0] << 8) + _buffer[1];
}


uint16_t AGS02MA::_getDataLSB()
{
  return (_buffer[2] << 8) + _buffer[3];
}


uint8_t AGS02MA::_CRC8(uint8_t * buf, uint8_t size)
{
  uint8_t crc = 0xFF;  //  start value
  for (uint8_t b = 0; b < size; b++)
  {
    crc ^= buf[b];
    for (uint8_t i = 0; i < 8; i++)
    {
      if (crc & 0x80) crc = (crc << 1) ^ 0x31;
      else crc = (crc << 1);
    }
  }
  return crc;
}


uint8_t AGS02MA::_bin2bcd (uint8_t value)
{
  return value + 6 * (value / 10);
}


//  -- END OF FILE --

