#pragma once
//
//    FILE: AGS02MA.h
//  AUTHOR: Rob Tillaart, Viktor Balint, Beanow
//    DATE: 2021-08-12
// VERSION: 0.4.3
// PURPOSE: Arduino library for AGS02MA TVOC sensor
//     URL: https://github.com/RobTillaart/AGS02MA


#include "Arduino.h"
#include "Wire.h"


#define AGS02MA_LIB_VERSION         (F("0.4.3"))

#define AGS02MA_OK                  0
#define AGS02MA_ERROR               -10
#define AGS02MA_ERROR_CRC           -11
#define AGS02MA_ERROR_READ          -12
#define AGS02MA_ERROR_NOT_READY     -13
#define AGS02MA_ERROR_REQUEST       -14


#define AGS02MA_I2C_CLOCK           25000    //  max 30000


class AGS02MA
{
public:
  struct RegisterData
  {
    uint8_t data[4];
    uint8_t crc;
    bool    crcValid;
  };

  struct ZeroCalibrationData
  {
    /**
     *  Warning, the exact meaning of this status is not fully documented.
     *  It seems like it's a bit mask:
     *  0000 1100 | 0x0C | 12  | Typical value
     *  0000 1101 | 0x0D | 13  | Sometimes seen on v117
     *  0111 1101 | 0x7D | 125 | Seen on v118, after power-off (gives different data than 12!)
     */
    uint16_t status;
    uint16_t value;
  };

  //  address 26 = 0x1A
  explicit AGS02MA(const uint8_t deviceAddress = 26, TwoWire *wire = &Wire);

  bool     begin();
  bool     isConnected();
  void     reset();

  bool     isHeated() { return (millis() - _startTime) > 120000UL; };


  //  CONFIGURATION
  bool     setAddress(const uint8_t deviceAddress);
  uint8_t  getAddress() { return _address; };
  uint8_t  getSensorVersion();
  uint32_t getSensorDate();

  //  to set the speed the I2C bus should return to
  //  as the device operates at very low bus speed of 30 kHz.
  void     setI2CResetSpeed(uint32_t speed) { _I2CResetSpeed = speed; };
  uint32_t getI2CResetSpeed() { return _I2CResetSpeed; };

  //  to be called after at least 5 minutes in fresh air.
  bool     zeroCalibration() { return manualZeroCalibration(0); };

  /**
   *  Set the zero calibration value manually.
   *  To be called after at least 5 minutes in fresh air.
   *  For v117: 0-65535 = automatic calibration.
   *  For v118: 0 = automatic calibration, 1-65535 manual calibration.
   */
  bool     manualZeroCalibration(uint16_t value = 0);
  bool     getZeroCalibrationData(ZeroCalibrationData &data);


  //  MODE
  bool     setPPBMode();
  bool     setUGM3Mode();
  uint8_t  getMode()    { return _mode; };


  //  READ functions
  uint32_t readPPB();   //  parts per billion 10^9
  uint32_t readUGM3();  //  microgram per cubic meter

  //  derived read functions
  float    readPPM()    { return readPPB()  * 0.001; };         //  parts per million
  float    readMGM3()   { return readUGM3() * 0.001; };         //  milligram per cubic meter
  float    readUGF3()   { return readUGM3() * 0.0283168466; };  //  microgram per cubic feet

  float    lastPPM()    { return _lastPPB   * 0.001; };
  uint32_t lastPPB()    { return _lastPPB;  };    //  fetch last PPB measurement
  uint32_t lastUGM3()   { return _lastUGM3; };    //  fetch last UGM3 measurement


  //  STATUS
  uint32_t lastRead()   { return _lastRead; };    //  timestamp last measurement
  int      lastError();
  uint8_t  lastStatus() { return _status; };
  uint8_t  dataReady()  { return _status & 0x01; };

  //  Reading registers
  bool     readRegister(uint8_t address, RegisterData &reg);


private:
  uint32_t _readSensor();
  bool     _readRegister(uint8_t reg);
  bool     _writeRegister(uint8_t reg);

  uint32_t _I2CResetSpeed = 100000;
  uint32_t _startTime     = 0;
  uint32_t _lastRead      = 0;
  uint32_t _lastRegTime   = 0;
  uint32_t _lastPPB       = 0;
  uint32_t _lastUGM3      = 0;
  uint8_t  _address       = 0;
  uint8_t  _mode          = 255;
  uint8_t  _status        = 0;
  uint8_t  _buffer[5];

  void     _setI2CLowSpeed();
  void     _setI2CHighSpeed();
  uint16_t _getDataMSB();
  uint16_t _getDataLSB();
  uint8_t  _CRC8(uint8_t * buf, uint8_t size);
  uint8_t  _bin2bcd(uint8_t val);

  int      _error = AGS02MA_OK;

  TwoWire*  _wire;
};


//  -- END OF FILE --

