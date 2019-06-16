/*
  xsns_46_MLX90614.ino - Support for MLX ir temperature sensor

  Copyright (C) 2019   Gerhard Mutz and Theo Arends

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

#define XSNS_46                          46

#define I2_ADR_IRT      0x5a

uint8_t mlx_ready;
float obj_temp;
float amb_temp;

void MLX90614_Init() {

  if (!I2cDevice(I2_ADR_IRT)) {
    return;
  }

  mlx_ready=1;

  // not needed on tasmota
  //Wire.begin();
  //delay(500);
}

#define MLX90614_RAWIR1 0x04
#define MLX90614_RAWIR2 0x05
#define MLX90614_TA 0x06
#define MLX90614_TOBJ1 0x07
#define MLX90614_TOBJ2 0x08

// return ir temp
// 0 = chip, 1 = object temperature
// * 0.02 - 273.15
uint16_t read_irtmp(uint8_t flag) {
    uint8_t hig,low;
    uint16_t val;

    Wire.beginTransmission(I2_ADR_IRT);
    if (!flag) Wire.write(MLX90614_TA);
    else Wire.write(MLX90614_TOBJ1);
    Wire.endTransmission(false);

    Wire.requestFrom(I2_ADR_IRT, (uint8_t)3);
    low=Wire.read();
    hig=Wire.read();
    Wire.read();

    val=((uint16_t)hig<<8)|low;
    return val;
}

void MLX90614_Every_Second(void) {

  if (!mlx_ready) return;
  uint16_t uval=read_irtmp(1);
  if (uval&0x8000) {
    obj_temp=-999;
  } else {
    obj_temp=((float)uval*0.02)-273.15;
  }
  uval=read_irtmp(0);
  if (uval&0x8000) {
    amb_temp=-999;
  } else {
    amb_temp=((float)uval*0.02)-273.15;
  }
}

#ifdef USE_WEBSERVER
 const char HTTP_IRTMP[] PROGMEM =
  "{s}MXL90614 " "OBJ-" D_TEMPERATURE "{m}%s C" "{e}"
  "{s}MXL90614 " "AMB-" D_TEMPERATURE "{m}%s C" "{e}";

void MLX90614_Show(uint8_t json) {

  if (!mlx_ready) return;

  char obj_tstr[16];
  dtostrfd(obj_temp, Settings.flag2.temperature_resolution, obj_tstr);
  char amb_tstr[16];
  dtostrfd(amb_temp, Settings.flag2.temperature_resolution, amb_tstr);

  if (json) {
    ResponseAppend_P(PSTR(",\"MLX90614\":{\"OBJTMP\":%s,\"AMBTMP\":%s}"), obj_tstr,amb_tstr);
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_IRTMP,obj_tstr,amb_tstr);
#endif
  }

}
#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns46(byte function)
{
  bool result = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_INIT:
        MLX90614_Init();
        break;
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
