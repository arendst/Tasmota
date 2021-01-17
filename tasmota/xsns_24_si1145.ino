/*
  xsns_24_si1145.ino - SI1145/46/47 UV Index / IR / Visible light sensor support for Tasmota

  Copyright (C) 2021  Theo Arends

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
#ifdef USE_SI1145
/*********************************************************************************************\
 * SI1145/46/47 - UV Index / IR / Visible light
 *
 * Based on library https://github.com/SeeedDocument/Grove-Sunlight_Sensor/
 *
 * I2C Addresses: 0x60
\*********************************************************************************************/

#define XSNS_24                             24
#define XI2C_19                             19  // See I2CDEVICES.md

#define SI114X_ADDR                         0X60
//
//commands
//
#define SI114X_QUERY                        0X80
#define SI114X_SET                          0XA0
#define SI114X_NOP                          0X00
#define SI114X_RESET                        0X01
#define SI114X_BUSADDR                      0X02
#define SI114X_PS_FORCE                     0X05
#define SI114X_GET_CAL                      0X12
#define SI114X_ALS_FORCE                    0X06
#define SI114X_PSALS_FORCE                  0X07
#define SI114X_PS_PAUSE                     0X09
#define SI114X_ALS_PAUSE                    0X0A
#define SI114X_PSALS_PAUSE                  0X0B
#define SI114X_PS_AUTO                      0X0D
#define SI114X_ALS_AUTO                     0X0E
#define SI114X_PSALS_AUTO                   0X0F
//
//IIC REGISTERS
//
#define SI114X_PART_ID                      0X00
#define SI114X_REV_ID                       0X01
#define SI114X_SEQ_ID                       0X02
#define SI114X_INT_CFG                      0X03
#define SI114X_IRQ_ENABLE                   0X04
#define SI114X_IRQ_MODE1                    0x05
#define SI114X_IRQ_MODE2                    0x06
#define SI114X_HW_KEY                       0X07
#define SI114X_MEAS_RATE0                   0X08
#define SI114X_MEAS_RATE1                   0X09
#define SI114X_PS_RATE                      0X0A
#define SI114X_PS_LED21                     0X0F
#define SI114X_PS_LED3                      0X10
#define SI114X_UCOEFF0                      0X13
#define SI114X_UCOEFF1                      0X14
#define SI114X_UCOEFF2                      0X15
#define SI114X_UCOEFF3                      0X16
#define SI114X_WR                           0X17
#define SI114X_COMMAND                      0X18
#define SI114X_RESPONSE                     0X20
#define SI114X_IRQ_STATUS                   0X21
#define SI114X_ALS_VIS_DATA0                0X22
#define SI114X_ALS_VIS_DATA1                0X23
#define SI114X_ALS_IR_DATA0                 0X24
#define SI114X_ALS_IR_DATA1                 0X25
#define SI114X_PS1_DATA0                    0X26
#define SI114X_PS1_DATA1                    0X27
#define SI114X_PS2_DATA0                    0X28
#define SI114X_PS2_DATA1                    0X29
#define SI114X_PS3_DATA0                    0X2A
#define SI114X_PS3_DATA1                    0X2B
#define SI114X_AUX_DATA0_UVINDEX0           0X2C
#define SI114X_AUX_DATA1_UVINDEX1           0X2D
#define SI114X_RD                           0X2E
#define SI114X_CHIP_STAT                    0X30
//
//Parameters
//
#define SI114X_CHLIST                       0X01
#define SI114X_CHLIST_ENUV                  0x80
#define SI114X_CHLIST_ENAUX                 0x40
#define SI114X_CHLIST_ENALSIR               0x20
#define SI114X_CHLIST_ENALSVIS              0x10
#define SI114X_CHLIST_ENPS1                 0x01
#define SI114X_CHLIST_ENPS2                 0x02
#define SI114X_CHLIST_ENPS3                 0x04

#define SI114X_PSLED12_SELECT               0X02
#define SI114X_PSLED3_SELECT                0X03

#define SI114X_PS_ENCODE                    0X05
#define SI114X_ALS_ENCODE                   0X06

#define SI114X_PS1_ADCMUX                   0X07
#define SI114X_PS2_ADCMUX                   0X08
#define SI114X_PS3_ADCMUX                   0X09

#define SI114X_PS_ADC_COUNTER               0X0A
#define SI114X_PS_ADC_GAIN                  0X0B
#define SI114X_PS_ADC_MISC                  0X0C

#define SI114X_ALS_IR_ADC_MUX               0X0E
#define SI114X_AUX_ADC_MUX                  0X0F

#define SI114X_ALS_VIS_ADC_COUNTER          0X10
#define SI114X_ALS_VIS_ADC_GAIN             0X11
#define SI114X_ALS_VIS_ADC_MISC             0X12

#define SI114X_LED_REC                      0X1C

#define SI114X_ALS_IR_ADC_COUNTER           0X1D
#define SI114X_ALS_IR_ADC_GAIN              0X1E
#define SI114X_ALS_IR_ADC_MISC              0X1F
//
//USER SETTINGS DEFINE
//
//ADCMUX
#define SI114X_ADCMUX_SMALL_IR              0x00
#define SI114X_ADCMUX_VISIABLE              0x02
#define SI114X_ADCMUX_LARGE_IR              0x03
#define SI114X_ADCMUX_NO                    0x06
#define SI114X_ADCMUX_GND                   0x25
#define SI114X_ADCMUX_TEMPERATURE           0x65
#define SI114X_ADCMUX_VDD                   0x75
//LED SEL
#define SI114X_PSLED12_SELECT_PS1_NONE      0x00
#define SI114X_PSLED12_SELECT_PS1_LED1      0x01
#define SI114X_PSLED12_SELECT_PS1_LED2      0x02
#define SI114X_PSLED12_SELECT_PS1_LED3      0x04
#define SI114X_PSLED12_SELECT_PS2_NONE      0x00
#define SI114X_PSLED12_SELECT_PS2_LED1      0x10
#define SI114X_PSLED12_SELECT_PS2_LED2      0x20
#define SI114X_PSLED12_SELECT_PS2_LED3      0x40
#define SI114X_PSLED3_SELECT_PS2_NONE       0x00
#define SI114X_PSLED3_SELECT_PS2_LED1       0x10
#define SI114X_PSLED3_SELECT_PS2_LED2       0x20
#define SI114X_PSLED3_SELECT_PS2_LED3       0x40
//ADC GAIN DIV
#define SI114X_ADC_GAIN_DIV1                0X00
#define SI114X_ADC_GAIN_DIV2                0X01
#define SI114X_ADC_GAIN_DIV4                0X02
#define SI114X_ADC_GAIN_DIV8                0X03
#define SI114X_ADC_GAIN_DIV16               0X04
#define SI114X_ADC_GAIN_DIV32               0X05
//LED CURRENT
#define SI114X_LED_CURRENT_5MA              0X01
#define SI114X_LED_CURRENT_11MA             0X02
#define SI114X_LED_CURRENT_22MA             0X03
#define SI114X_LED_CURRENT_45MA             0X04
//Recovery period the  ADC takes before making a PS measurement
#define SI114X_ADC_COUNTER_1ADCCLK          0X00
#define SI114X_ADC_COUNTER_7ADCCLK          0X01
#define SI114X_ADC_COUNTER_15ADCCLK         0X02
#define SI114X_ADC_COUNTER_31ADCCLK         0X03
#define SI114X_ADC_COUNTER_63ADCCLK         0X04
#define SI114X_ADC_COUNTER_127ADCCLK        0X05
#define SI114X_ADC_COUNTER_255ADCCLK        0X06
#define SI114X_ADC_COUNTER_511ADCCLK        0X07
//ADC MISC
#define SI114X_ADC_MISC_LOWRANGE            0X00
#define SI114X_ADC_MISC_HIGHRANGE           0X20
#define SI114X_ADC_MISC_ADC_NORMALPROXIMITY 0X00
#define SI114X_ADC_MISC_ADC_RAWADC          0X04
//INT OE
#define SI114X_INT_CFG_INTOE                0X01
//IRQ ENABLE
#define SI114X_IRQEN_ALS                    0x01
#define SI114X_IRQEN_PS1                    0x04
#define SI114X_IRQEN_PS2                    0x08
#define SI114X_IRQEN_PS3                    0x10

uint16_t si1145_visible;
uint16_t si1145_infrared;
uint16_t si1145_uvindex;

bool si1145_type = false;
uint8_t si1145_valid = 0;

/********************************************************************************************/

uint8_t Si1145ReadByte(uint8_t reg)
{
  return I2cRead8(SI114X_ADDR, reg);
}

uint16_t Si1145ReadHalfWord(uint8_t reg)
{
  return I2cRead16LE(SI114X_ADDR, reg);
}

void Si1145WriteByte(uint8_t reg, uint16_t val)
{
  I2cWrite8(SI114X_ADDR, reg, val);
}

uint8_t Si1145WriteParamData(uint8_t p, uint8_t v)
{
  Si1145WriteByte(SI114X_WR, v);
  Si1145WriteByte(SI114X_COMMAND, p | SI114X_SET);
  return Si1145ReadByte(SI114X_RD);
}

/********************************************************************************************/

bool Si1145Present(void)
{
  return (Si1145ReadByte(SI114X_PART_ID) == 0X45);
}

void Si1145Reset(void)
{
  Si1145WriteByte(SI114X_MEAS_RATE0, 0);
  Si1145WriteByte(SI114X_MEAS_RATE1, 0);
  Si1145WriteByte(SI114X_IRQ_ENABLE, 0);
  Si1145WriteByte(SI114X_IRQ_MODE1, 0);
  Si1145WriteByte(SI114X_IRQ_MODE2, 0);
  Si1145WriteByte(SI114X_INT_CFG, 0);
  Si1145WriteByte(SI114X_IRQ_STATUS, 0xFF);

  Si1145WriteByte(SI114X_COMMAND, SI114X_RESET);
  delay(10);
  Si1145WriteByte(SI114X_HW_KEY, 0x17);
  delay(10);
}

void Si1145DeInit(void)
{
  //ENABLE UV reading
  //these reg must be set to the fixed value
  Si1145WriteByte(SI114X_UCOEFF0, 0x29);
  Si1145WriteByte(SI114X_UCOEFF1, 0x89);
  Si1145WriteByte(SI114X_UCOEFF2, 0x02);
  Si1145WriteByte(SI114X_UCOEFF3, 0x00);
  Si1145WriteParamData(SI114X_CHLIST, SI114X_CHLIST_ENUV | SI114X_CHLIST_ENALSIR | SI114X_CHLIST_ENALSVIS | SI114X_CHLIST_ENPS1);
  //
  //set LED1 CURRENT(22.4mA)(It is a normal value for many LED)
  //
  Si1145WriteParamData(SI114X_PS1_ADCMUX, SI114X_ADCMUX_LARGE_IR);
  Si1145WriteByte(SI114X_PS_LED21, SI114X_LED_CURRENT_22MA);
  Si1145WriteParamData(SI114X_PSLED12_SELECT, SI114X_PSLED12_SELECT_PS1_LED1);
  //
  //PS ADC SETTING
  //
  Si1145WriteParamData(SI114X_PS_ADC_GAIN, SI114X_ADC_GAIN_DIV1);
  Si1145WriteParamData(SI114X_PS_ADC_COUNTER, SI114X_ADC_COUNTER_511ADCCLK);
  Si1145WriteParamData(SI114X_PS_ADC_MISC, SI114X_ADC_MISC_HIGHRANGE | SI114X_ADC_MISC_ADC_RAWADC);
  //
  //VIS ADC SETTING
  //
  Si1145WriteParamData(SI114X_ALS_VIS_ADC_GAIN, SI114X_ADC_GAIN_DIV1);
  Si1145WriteParamData(SI114X_ALS_VIS_ADC_COUNTER, SI114X_ADC_COUNTER_511ADCCLK);
  Si1145WriteParamData(SI114X_ALS_VIS_ADC_MISC, SI114X_ADC_MISC_HIGHRANGE);
  //
  //IR ADC SETTING
  //
  Si1145WriteParamData(SI114X_ALS_IR_ADC_GAIN, SI114X_ADC_GAIN_DIV1);
  Si1145WriteParamData(SI114X_ALS_IR_ADC_COUNTER, SI114X_ADC_COUNTER_511ADCCLK);
  Si1145WriteParamData(SI114X_ALS_IR_ADC_MISC, SI114X_ADC_MISC_HIGHRANGE);
  //
  //interrupt enable
  //
  Si1145WriteByte(SI114X_INT_CFG, SI114X_INT_CFG_INTOE);
  Si1145WriteByte(SI114X_IRQ_ENABLE, SI114X_IRQEN_ALS);
  //
  //AUTO RUN
  //
  Si1145WriteByte(SI114X_MEAS_RATE0, 0xFF);
  Si1145WriteByte(SI114X_COMMAND, SI114X_PSALS_AUTO);
}

bool Si1145Begin(void)
{
  if (!Si1145Present()) { return false; }

  Si1145Reset();
  Si1145DeInit();
  return true;
}

// returns the UV index * 100 (divide by 100 to get the index)
uint16_t Si1145ReadUV(void)
{
  return Si1145ReadHalfWord(SI114X_AUX_DATA0_UVINDEX0);
}

// returns visible+IR light levels
uint16_t Si1145ReadVisible(void)
{
  return Si1145ReadHalfWord(SI114X_ALS_VIS_DATA0);
}

// returns IR light levels
uint16_t Si1145ReadIR(void)
{
  return Si1145ReadHalfWord(SI114X_ALS_IR_DATA0);
}

/********************************************************************************************/

bool Si1145Read(void)
{
  if (si1145_valid) { si1145_valid--; }

  if (!Si1145Present()) { return false; }

  si1145_visible = Si1145ReadVisible();
  si1145_infrared = Si1145ReadIR();
  si1145_uvindex = Si1145ReadUV();
  si1145_valid = SENSOR_MAX_MISS;
  return true;
}

void Si1145Detect(void)
{
  if (I2cActive(SI114X_ADDR)) { return; }

  if (Si1145Begin()) {
    si1145_type = true;
    I2cSetActiveFound(SI114X_ADDR, "SI1145");
  }
}

void Si1145Update(void)
{
  if (!Si1145Read()) {
    AddLogMissed("SI1145", si1145_valid);
  }
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_SI1145[] PROGMEM =
  "{s}SI1145 " D_ILLUMINANCE "{m}%d " D_UNIT_LUX "{e}"     // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
  "{s}SI1145 " D_INFRARED "{m}%d " D_UNIT_LUX "{e}"
  "{s}SI1145 " D_UV_INDEX "{m}%d.%d{e}";
#endif  // USE_WEBSERVER

void Si1145Show(bool json)
{
  if (si1145_valid) {
    if (json) {
      ResponseAppend_P(PSTR(",\"SI1145\":{\"" D_JSON_ILLUMINANCE "\":%d,\"" D_JSON_INFRARED "\":%d,\"" D_JSON_UV_INDEX "\":%d.%d}"),
        si1145_visible, si1145_infrared, si1145_uvindex /100, si1145_uvindex %100);
#ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) DomoticzSensor(DZ_ILLUMINANCE, si1145_visible);
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_SI1145, si1145_visible, si1145_infrared, si1145_uvindex /100, si1145_uvindex %100);
#endif
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns24(uint8_t function)
{
  if (!I2cEnabled(XI2C_19)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Si1145Detect();
  }
  else if (si1145_type) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        Si1145Update();
        break;
      case FUNC_JSON_APPEND:
        Si1145Show(1);
        break;
  #ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Si1145Show(0);
        break;
  #endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_SI1145
#endif  // USE_I2C
