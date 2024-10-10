/*
  xsns_66_iAQ.ino - Support for iAQ-Core - Indoor Air Quality Sensor Module

  Copyright (C) 2021   Christian Baars and Theo Arends

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
#ifdef USE_IAQ
/*********************************************************************************************\
 * iAQ-Core - Indoor Air Quality Sensor
 *
 * I2C Address: 0x5A
\*********************************************************************************************/

#define XSNS_66            66
#define XI2C_46            46      // See I2CDEVICES.md

#define I2_ADR_IAQ         0x5a    // collides with MLX90614 and maybe others

#define IAQ_STATUS_OK      0x00
#define IAQ_STATUS_BUSY    0x01
#define IAQ_STATUS_WARM    0x10
#define IAQ_STATUS_ERR     0x80
#define IAQ_STATUS_I2C_ERR 0xFF

struct  {
  int32_t   resistance;
  uint16_t  pred;
  uint16_t  Tvoc;
  uint8_t   i2c_address;
  uint8_t   i2c_bus;
  uint8_t   status;
  bool      ready;
} iAQ;

bool IAQ_Read(void) {
/*  
  TwoWire& myWire = I2cGetWire(iAQ.i2c_bus);
  if (&myWire == nullptr) { return false; }  // No valid I2c bus

  uint8_t buf[9];
  buf[2] = IAQ_STATUS_I2C_ERR; // populate entry with error code
  myWire.requestFrom(iAQ.i2c_address, sizeof(buf));
  for (uint32_t i = 0; i < 9; i++) {
    buf[i] = myWire.read();
  }
*/
  uint8_t buf[9];
  buf[2] = IAQ_STATUS_I2C_ERR; // populate entry with error code
  I2cReadBuffer0(iAQ.i2c_address, buf, sizeof(buf), iAQ.i2c_bus);

  // AddLog(LOG_LEVEL_DEBUG, "iAQ: buffer %x %x %x %x %x %x %x %x %x ", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8]);
  if (IAQ_STATUS_I2C_ERR == buf[2]) {
    return false;
  }
  iAQ.pred = (buf[0]<<8) + buf[1];
  iAQ.status = buf[2];
  iAQ.resistance =  ((uint32_t)buf[3]<<24) + ((uint32_t)buf[4]<<16) + ((uint32_t)buf[5]<<8) + (uint32_t)buf[6];
  iAQ.Tvoc =  (buf[7]<<8) + buf[8];
  return true;
}

void IAQ_Init(void) {
  for (uint32_t bus = 0; bus < 2; bus++) {
    if (!I2cSetDevice(I2_ADR_IAQ, bus)) { continue; }
    iAQ.i2c_address = I2_ADR_IAQ;
    iAQ.i2c_bus = bus;
    if (!IAQ_Read()) { continue; }
    I2cSetActiveFound(I2_ADR_IAQ, "IAQ", bus);
    iAQ.ready = true;
    break;
  }

/*
  for (iAQ.i2c_address = I2_ADR_IAQ; iAQ.i2c_address < I2_ADR_IAQ +5; iAQ.i2c_address++) {
    if (I2cActive(iAQ.i2c_address)) { continue; }
    if (I2cSetDevice(iAQ.i2c_address)) {
      I2cSetActiveFound(iAQ.i2c_address, "IAQ");
      iAQ.ready = true;
      break;
    }
  }
*/
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER
const char HTTP_SNS_IAQ[] PROGMEM =
  "{s}iAQ-Core " D_ECO2 "{m}%d " D_UNIT_PARTS_PER_MILLION "{e}"     // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
  "{s}iAQ-Core " D_TVOC "{m}%d " D_UNIT_PARTS_PER_BILLION "{e}";

const char HTTP_SNS_IAQ_ERROR[] PROGMEM =
  "{s}iAQ-Core {m} %s {e}";
#endif

void IAQ_Show(uint8_t json)
{
  IAQ_Read();

  if (json) {
    if (iAQ.status!=IAQ_STATUS_OK){
      AddLog(LOG_LEVEL_INFO, PSTR("iAQ: " D_ERROR " %x" ),iAQ.status);
      return;
  }
  else {
    ResponseAppend_P(PSTR(",\"IAQ\":{\"" D_JSON_ECO2 "\":%u,\"" D_JSON_TVOC "\":%u,\"" D_JSON_RESISTANCE "\":%u}"), iAQ.pred, iAQ.Tvoc, iAQ.resistance);
#ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) DomoticzSensor(DZ_AIRQUALITY, iAQ.pred);
#endif  // USE_DOMOTICZ
  }
#ifdef USE_WEBSERVER
    } else {
      switch(iAQ.status){
        case IAQ_STATUS_OK:
          WSContentSend_PD(HTTP_SNS_IAQ, iAQ.pred, iAQ.Tvoc);
          break;
        case IAQ_STATUS_WARM:
          WSContentSend_PD(HTTP_SNS_IAQ_ERROR, D_START);
          break;
        default:
          WSContentSend_PD(HTTP_SNS_IAQ_ERROR, D_ERROR);
      }
#endif
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns66(uint32_t function)
{
  if (!I2cEnabled(XI2C_46)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    IAQ_Init();
  }
  else if (iAQ.ready) {
    switch (function) {
      case FUNC_JSON_APPEND:
        IAQ_Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        IAQ_Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_IAQ
#endif  // USE_I2C