/*
  xsns_89_t67xx.ino - Telaire T6700 Series CO2 sensor support for Tasmota

  Copyright (c) 2021  Alexander Savchenko (alexander@savchenko.by)

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
#ifdef USE_T67XX
/*********************************************************************************************\
 * Telaire T6703/T6713 - CO2
 *
 * Based on https://github.com/drug123/T67XX/ by Yaroslav Osadchyy
 * (drug123@gmail.com)
 *
 * I2C Address: 0x15
\*********************************************************************************************/

#define XSNS_89 89
#define XI2C_61 61 // See I2CDEVICES.md

#define T67XX_I2C_ADDR 0x15
#define T67XX_REG_VAL_ENABLE 0xFF00
#define T67XX_REG_VAL_DISABLE 0x0000

#define T67XX_READ_DELAY 10 // Recommended value

#define T67XX_MEASURE_INTERVAL 15 // Seconds
#define T67XX_MEASURE_DELAY 115   // Seconds

// Registers
enum
{
  T67XX_REG_FIRMWARE = 0x1389,
  T67XX_REG_STATUS = 0x138A,
  T67XX_REG_PPM = 0x138B,
  T67XX_REG_RESET = 0x03E8,
  T67XX_REG_SPCAL = 0x03EC,
  T67XX_REG_FLASH_UPDATE = 0x03ED,
  T67XX_REG_ADDRESS = 0x0FA5,
  T67XX_REG_ABC_LOGIC = 0x03EE,
  T67XX_REG_MOD_MODE = 0x100B
};

class T67XX
{
public:
  T67XX();
  void Read(void);
  uint16_t readPPM(void);
  uint16_t PPM(void);
  uint16_t getStatus(void);
  uint16_t getFirmwareVersion(void);
  void detect(void);
  void reset(void);
  void setABCMode(bool Enabled);
  void setSlaveAddress(uint8_t Address);
  void flashUpdate(void);
  void beginCalibration(void);
  void endCalibration(void);
  const char *getLastStatusMsg(void);
  bool found(void);

  // 0: no error; 1: error has occurred
  struct status
  {
    uint16_t ERROR : 1;
    uint16_t FLASH_ERROR : 1;
    uint16_t CALIBRATION_ERROR : 1;
    uint16_t REBOOT : 1;
    uint16_t WARMUP : 1;
    uint16_t SINGLE_POINT_CAL : 1;

    void set(uint16_t data)
    {
      ERROR = data & 0x01;
      FLASH_ERROR = (data >> 0x01) & 0x01;
      CALIBRATION_ERROR = (data >> 0x02) & 0x01;
      REBOOT = (data >> 0x0A) & 0x01;
      WARMUP = (data >> 0x0B) & 0x01;
      SINGLE_POINT_CAL = (data >> 0x0F) & 0x01;
    }
  };

private:
  uint8_t _data[6];
  uint16_t _ppm = 0;
  const char *getStatusMsg(uint16_t sensorStatus);
  uint16_t read16(uint16_t reg);
  void write16(uint16_t reg, uint16_t data);

  uint8_t _init = 0;
  uint8_t _sta = 0;
  status _status;
  bool _found = 0;
};

T67XX::T67XX()
{
}

uint16_t T67XX::readPPM(void)
{
  _ppm = read16(T67XX_REG_PPM);
  return _ppm;
}

uint16_t T67XX::PPM(void)
{
  return _ppm;
}

uint16_t T67XX::getStatus(void)
{
  _sta = read16(T67XX_REG_STATUS);
  _status.set(_sta);

  return _sta;
};

uint16_t T67XX::getFirmwareVersion(void)
{
  return read16(T67XX_REG_FIRMWARE);
};

void T67XX::detect(void)
{
  if (!I2cSetDevice(T67XX_I2C_ADDR))
  {
    return;
  }

  reset();
  delay(100);
  uint16_t fw = getFirmwareVersion();

  if ((fw > 0) && (fw < 0xFFFF))
  {
    AddLog(LOG_LEVEL_INFO, PSTR("T67XX firmware version: %d"), fw);
    I2cSetActiveFound(T67XX_I2C_ADDR, "T67XX");
    _found = true;
  }
}

bool T67XX::found(void)
{
  return _found;
}

void T67XX::reset(void)
{
  write16(T67XX_REG_RESET, T67XX_REG_VAL_ENABLE);
  _init = 1;
};

void T67XX::setABCMode(bool Enabled)
{
  write16(T67XX_REG_ABC_LOGIC,
          Enabled ? T67XX_REG_VAL_ENABLE : T67XX_REG_VAL_DISABLE);
};

void T67XX::setSlaveAddress(uint8_t Address){
    // TODO:
};

void T67XX::flashUpdate(void)
{
  write16(T67XX_REG_FLASH_UPDATE, T67XX_REG_VAL_ENABLE);
}

void T67XX::beginCalibration(void)
{
  write16(T67XX_REG_SPCAL, T67XX_REG_VAL_ENABLE);
};

void T67XX::endCalibration(void)
{
  write16(T67XX_REG_SPCAL, T67XX_REG_VAL_DISABLE);
};

/*** Private Section ***/

uint16_t T67XX::read16(uint16_t reg)
{
  Wire.beginTransmission(T67XX_I2C_ADDR);
  Wire.write(0x04);
  Wire.write(byte(reg >> 8));
  Wire.write(byte(reg & 0xFF));
  Wire.write(0x00);
  Wire.write(0x01);
  Wire.endTransmission();

  delay(T67XX_READ_DELAY);

  Wire.requestFrom(int(T67XX_I2C_ADDR), 4);
  _data[0] = Wire.read();
  _data[1] = Wire.read();
  _data[2] = Wire.read();
  _data[3] = Wire.read();
  return ((_data[2] << 8) | _data[3]);
}

void T67XX::write16(uint16_t reg, uint16_t data)
{
  Wire.beginTransmission(T67XX_I2C_ADDR);
  Wire.write(0x05);
  Wire.write(byte(reg >> 8));
  Wire.write(byte(reg & 0xFF));
  Wire.write(byte(data >> 8));
  Wire.write(byte(data & 0xFF));
  Wire.endTransmission();

  delay(T67XX_READ_DELAY);

  Wire.requestFrom(int(T67XX_I2C_ADDR), 5);
  _data[0] = Wire.read();
  _data[1] = Wire.read();
  _data[2] = Wire.read();
  _data[3] = Wire.read();
  _data[4] = Wire.read();
}

const char *T67XX::getLastStatusMsg()
{
  return getStatusMsg(_sta);
}

const char *T67XX::getStatusMsg(uint16_t sensorStatus)
{
  T67XX::status statusStruct;

  statusStruct.set(sensorStatus);

  if (sensorStatus)
  {
    if (statusStruct.ERROR)
      return PSTR("GENERAL ERROR");
    if (statusStruct.CALIBRATION_ERROR)
      return PSTR("CALIBRATION ERROR");
    if (statusStruct.FLASH_ERROR)
      return PSTR("FLASH ERROR");
    if (statusStruct.REBOOT)
      return PSTR("REBOOT");
    if (statusStruct.WARMUP)
      return PSTR("WARMUP");
    if (statusStruct.SINGLE_POINT_CAL)
      return PSTR("CALIBRATING");
  }

  if (TasmotaGlobal.uptime < T67XX_MEASURE_DELAY)
  {
    return PSTR("WARMUP");
  }

  return PSTR("OK");
}

void T67XX::Read(void)
{
  if ((TasmotaGlobal.uptime < T67XX_MEASURE_DELAY) || (TasmotaGlobal.uptime % T67XX_MEASURE_INTERVAL > 0))
  {
    return;
  }

  uint16_t sensorStatus = getStatus();
  if (!sensorStatus)
  {
    if (_init)
    {
      setABCMode(true);
      _init = 0;
    }
    _ppm = readPPM();
    //AddLog_P(LOG_LEVEL_INFO, PSTR("T67XX %d PPM"), _ppm);
  }
  else
  {
    //AddLog_P(LOG_LEVEL_INFO, PSTR("T67XX status: %s"), getLastStatusMsg());
  }
}

T67XX t67xx;

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns89(uint8_t function)
{
  uint16_t ppm = 0;
  if (!I2cEnabled(XI2C_61))
  {
    return false;
  }

  bool result = false;

  if (FUNC_INIT == function)
  {
    t67xx.detect();
  }
  else if (t67xx.found())
  {
    switch (function)
    {
    case FUNC_INIT:
      break;
    case FUNC_EVERY_SECOND:
      t67xx.Read();
      break;
    case FUNC_JSON_APPEND:
      ppm = t67xx.PPM();
      if ((ppm > 100) && (ppm < 5000))
      {
        ResponseAppend_P(PSTR(",\"T67XX\":{\"" D_JSON_CO2 "\":%d,\"" D_JSON_STATUS "\":\"%s\"}"), t67xx.PPM(), t67xx.getLastStatusMsg());
      }
      else
      {
        ResponseAppend_P(PSTR(",\"T67XX\":{\"" D_JSON_CO2 "\":null,\"" D_JSON_STATUS "\":\"%s\"}"), t67xx.getLastStatusMsg());
      }
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      ppm = t67xx.PPM();
      if (ppm > 0)
      {
        WSContentSend_PD(PSTR("{s}T67XX " D_CO2 "{m}%d " D_UNIT_PARTS_PER_MILLION "{e}"), ppm);
      }
      break;
#endif // USE_WEBSERVER
    }
  }

  return result;
}

#endif // USE_T67xx
#endif // USE_I2C
