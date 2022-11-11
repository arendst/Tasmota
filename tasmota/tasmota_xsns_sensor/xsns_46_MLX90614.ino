/*
  xsns_46_MLX90614.ino - Support for MLX ir temperature sensor

  Copyright (C) 2021   Gerhard Mutz and Theo Arends

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
#ifdef USE_MLX90614

#define XSNS_46         46
#define XI2C_32         32  // See I2CDEVICES.md

#define I2_ADR_IRT      0x5a

#define MLX90614_RAWIR1 0x04
#define MLX90614_RAWIR2 0x05
#define MLX90614_TA     0x06
#define MLX90614_TOBJ1  0x07
#define MLX90614_TOBJ2  0x08

struct  {
  union {
    uint16_t value;
    uint32_t i2c_buf;
    };
  float obj_temp;
  float amb_temp;
  bool ready = false;
} mlx90614;

void MLX90614_Init(void)
{
  if (!I2cSetDevice(I2_ADR_IRT)) { return; }
  I2cSetActiveFound(I2_ADR_IRT, "MLX90614");
  mlx90614.ready = true;
}

void MLX90614_Every_Second(void)
{
    //mlx90614.i2c_buf = I2cRead24(I2_ADR_IRT, MLX90614_TOBJ1);
    mlx90614.value = MLX90614_read16(I2_ADR_IRT, MLX90614_TOBJ1);
    if (mlx90614.value & 0x8000) {
      mlx90614.obj_temp = -999;
    } else {
      mlx90614.obj_temp = ((float)mlx90614.value * 0.02f) - 273.15f;
    }
    //mlx90614.i2c_buf = I2cRead24(I2_ADR_IRT,MLX90614_TA);
    mlx90614.value = MLX90614_read16(I2_ADR_IRT, MLX90614_TA);
    if (mlx90614.value & 0x8000) {
      mlx90614.amb_temp = -999;
    } else {
      mlx90614.amb_temp = ((float)mlx90614.value * 0.02f) - 273.15f;
    }
}

#ifdef USE_WEBSERVER
 const char HTTP_IRTMP[] PROGMEM =
  "{s}MXL90614 " "OBJ-" D_TEMPERATURE "{m}%s C" "{e}"
  "{s}MXL90614 " "AMB-" D_TEMPERATURE "{m}%s C" "{e}";
#endif  // USE_WEBSERVER

void MLX90614_Show(uint8_t json)
{
  char obj_tstr[16];
  dtostrfd(mlx90614.obj_temp, Settings->flag2.temperature_resolution, obj_tstr);
  char amb_tstr[16];
  dtostrfd(mlx90614.amb_temp, Settings->flag2.temperature_resolution, amb_tstr);

  if (json) {
    ResponseAppend_P(PSTR(",\"MLX90614\":{\"OBJTMP\":%s,\"AMBTMP\":%s}"), obj_tstr, amb_tstr);
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_IRTMP, obj_tstr, amb_tstr);
#endif
  }
}

uint16_t MLX90614_read16(uint8_t addr, uint8_t a) {
  uint16_t ret;

  Wire.beginTransmission(addr);
  Wire.write(a);
  Wire.endTransmission(false);

  Wire.requestFrom(addr, (size_t)3);
  uint8_t buff[5];
  buff[0] = addr << 1;
  buff[1] = a;
  buff[2] = (addr << 1) | 1;
  buff[3] = Wire.read();
  buff[4] = Wire.read();
  ret = buff[3] | (buff[4] << 8);
  uint8_t pec = Wire.read();
  uint8_t cpec = MLX90614_crc8(buff, sizeof(buff));
  //AddLog(LOG_LEVEL_INFO,PSTR("%x - %x"),pec, cpec);

  if (pec != cpec) {
    AddLog(LOG_LEVEL_INFO,PSTR("mlx checksum error"));
  }
  return ret;
}


uint8_t MLX90614_crc8(uint8_t *addr, uint8_t len)
// The PEC calculation includes all bits except the START, REPEATED START, STOP,
// ACK, and NACK bits. The PEC is a CRC-8 with polynomial X8+X2+X1+1.
{
  uint8_t crc = 0;
  while (len--) {
    uint8_t inbyte = *addr++;
    for (uint8_t i = 8; i; i--) {
      uint8_t carry = (crc ^ inbyte) & 0x80;
      crc <<= 1;
      if (carry)
        crc ^= 0x7;
      inbyte <<= 1;
    }
  }
  return crc;
}


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns46(uint32_t function)
{
  if (!I2cEnabled(XI2C_32)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    MLX90614_Init();
  }
  else if (mlx90614.ready) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        MLX90614_Every_Second();
        break;
      case FUNC_JSON_APPEND:
        MLX90614_Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        MLX90614_Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_MLX90614
#endif  // USE_I2C
