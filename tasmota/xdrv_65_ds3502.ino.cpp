/*
  xdrv_65_ds3502.ino - Support for I2C DS3502 digital potentiometer driver on Tasmota

  Copyright (C) 2021  Andre Thomas and Theo Arends

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
#ifdef USE_DS3502
/*********************************************************************************************\
 * DS3502 - digital potentiometer (https://datasheets.maximintegrated.com/en/ds/DS3502.pdf)
 *
 * I2C Address: 0x28 .. 0x2B
\*********************************************************************************************/

#define XDRV_65                     65
#define XI2C_65                     65  // See I2CDEVICES.md

// #define DS3502_REG_MODE1           0x00
// #define DS3502_REG_LED0_ON_L       0x06
// #define DS3502_REG_PRE_SCALE       0xFE

#ifndef USE_DS3502_ADDR
  #define USE_DS3502_ADDR          0x28
#endif

bool DS3502_detected = false;
uint8_t DS3502_potiValue;

void DS3502_Detect(void)
{
  if (!I2cSetDevice(USE_DS3502_ADDR)) { return; }

  uint8_t buffer;
  // TODO: init routine
//   if (I2cValidRead8(&buffer, USE_DS3502_ADDR, DS3502_REG_MODE1)) {
//     I2cWrite8(USE_DS3502_ADDR, DS3502_REG_MODE1, 0x20);
//     if (I2cValidRead8(&buffer, USE_DS3502_ADDR, DS3502_REG_MODE1)) {
//       if (0x20 == buffer) {
//         DS3502_detected = true;
//         I2cSetActiveFound(USE_DS3502_ADDR, "DS3502");
//         DS3502_Reset(); // Reset the controller
//       }
//     }
//   }
}

void DS3502_Reset(void)
{
  //I2cWrite8(USE_DS3502_ADDR, DS3502_REG_MODE1, 0x80);
//  DS3502_SetPotifreq(USE_DS3502_FREQ);
  DS3502_SetPoti(0);
  DS3502_potiValue = 0;
  Response_P(PSTR("{\"DS3502\":{\"RESET\":\"OK\"}}"));
}

void DS3502_SetPoti_Reg(uint8_t val) {
//   uint8_t led_reg = DS3502_REG_LED0_ON_L + 4 * pin;
//   uint32_t led_data = 0;
//   I2cWrite8(USE_DS3502_ADDR, led_reg, on);
//   I2cWrite8(USE_DS3502_ADDR, led_reg+1, (on >> 8));
//   I2cWrite8(USE_DS3502_ADDR, led_reg+2, off);
//   I2cWrite8(USE_DS3502_ADDR, led_reg+3, (off >> 8));
}

void DS3502_SetPoti(uint8_t poti) {
  if (127 == poti) {
    DS3502_SetPoti_Reg(127); // Special use additional bit causes channel to turn on completely without PWM
  } else {
    DS3502_SetPoti_Reg(poti);
  }
  DS3502_potiValue = poti;
}

bool DS3502_Command(void)
{
  bool serviced = true;
  bool validpin = false;
  uint8_t paramcount = 0;
  if (XdrvMailbox.data_len > 0) {
    paramcount=1;
  } else {
    serviced = false;
    return serviced;
  }
  char argument[XdrvMailbox.data_len];
  for (uint32_t ca=0;ca<XdrvMailbox.data_len;ca++) {
    if ((' ' == XdrvMailbox.data[ca]) || ('=' == XdrvMailbox.data[ca])) { XdrvMailbox.data[ca] = ','; }
    if (',' == XdrvMailbox.data[ca]) { paramcount++; }
  }
  UpperCase(XdrvMailbox.data,XdrvMailbox.data);

  if (!strcmp(ArgV(argument, 1),"RESET"))  {  DS3502_Reset(); return serviced; }

  if (!strcmp(ArgV(argument, 1),"STATUS"))  { DS3502_OutputTelemetry(false); return serviced; }

  if (!strcmp(ArgV(argument, 1),"PWM")) {
    if (paramcount > 1) {
      uint8_t pin = 0;//atoi(ArgV(argument, 2));
      if (paramcount > 2) {
        if (!strcmp(ArgV(argument, 3), "ON")) {
          DS3502_SetPoti(127);
          Response_P(PSTR("{\"DS3502\":{\"PIN\":%i,\"PWM\":%i}}"),pin,4096);
          serviced = true;
          return serviced;
        }
        if (!strcmp(ArgV(argument, 3), "OFF")) {
          DS3502_SetPoti(0);
          Response_P(PSTR("{\"DS3502\":{\"PIN\":%i,\"PWM\":%i}}"),pin,0);
          serviced = true;
          return serviced;
        }
        uint16_t pwm = atoi(ArgV(argument, 3));
        if ((pin >= 0 && pin <= 15) && (pwm >= 0 && pwm <= 127)) {
          DS3502_SetPoti(pwm);
          Response_P(PSTR("{\"DS3502\":{\"PIN\":%i,\"PWM\":%i}}"),pin,pwm);
          serviced = true;
          return serviced;
        }
      }
    }
  }
  return serviced;
}

void DS3502_OutputTelemetry(bool telemetry)
{
  ResponseTime_P(PSTR(",\"DS3502\":{\"PWM_FREQ\":%i,"),DS3502_freq);
  for (uint32_t pin=0;pin<16;pin++) {
    ResponseAppend_P(PSTR("\"PWM%i\":%i,"),pin,DS3502_potiValue[pin]);
  }
  ResponseAppend_P(PSTR("\"END\":1}}"));
  if (telemetry) {
    MqttPublishTeleSensor();
  }
}

bool Xdrv65(uint8_t function)
{
  if (!I2cEnabled(XI2C_65)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    DS3502_Detect();
  }
  else if (DS3502_detected) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        if (TasmotaGlobal.tele_period == 0) {
          DS3502_OutputTelemetry(true);
        }
        break;
      case FUNC_COMMAND_DRIVER:
        if (XDRV_65 == XdrvMailbox.index) {
          result = DS3502_Command();
        }
        break;
    }
  }
  return result;
}

#endif // USE_DS3502
#endif // USE_IC2
