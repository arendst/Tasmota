/*
  xdrv_15_pca9632.ino - Support for I2C PCA9632 4bit 8 pin hardware PWM driver on Tasmota

  Copyright (C) 2022  Pascal Heinrich

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
#ifdef USE_PCA9632
/*********************************************************************************************\
 * PCA9632 - 4-channel 4-bit pwm driver
 *
 * I2C Address: 0x60
\*********************************************************************************************/

#define XDRV_91                     91
#define XI2C_75                     75  // See I2CDEVICES.md

#define PCA9632_REG_MODE1           0x00
#define PCA9632_REG_MODE2           0x01
#define PCA9632_REG_PWM0            0x02
#define PCA9632_REG_PWM1            0x03
#define PCA9632_REG_PWM2            0x04
#define PCA9632_REG_PWM3            0x05
#define PCA9632_REG_GRPPWM          0x06
#define PCA9632_REG_GRPGREQ         0x07
#define PCA9632_REG_LEDOUT          0x08

#ifndef USE_PCA9632_ADDR
  #define USE_PCA9632_ADDR          0x60
#endif

bool pca9632_inverted = false; // invert PWM for open-collector load
bool pca9632_detected = false;
uint8_t pca9632_pin_pwm_value[4];

void PCA9632_Detect(void)
{
  if (!I2cSetDevice(USE_PCA9632_ADDR)) { return; }

  uint8_t buffer;
  if (I2cValidRead8(&buffer, USE_PCA9632_ADDR, PCA9632_REG_MODE1)) {
    I2cWrite8(USE_PCA9632_ADDR, PCA9632_REG_MODE1, 0x20);
    if (I2cValidRead8(&buffer, USE_PCA9632_ADDR, PCA9632_REG_MODE1)) {
      if (0x20 == buffer) {
        pca9632_detected = true;
        I2cSetActiveFound(USE_PCA9632_ADDR, "PCA9632");
        PCA9632_Reset(); // Reset the controller
      }
    }
  }
}

void PCA9632_Reset(void)
{
  I2cWrite8(USE_PCA9632_ADDR, PCA9632_REG_MODE1, 0x80);
  pca9632_inverted = false;
  for (uint32_t pin=0;pin<4;pin++) {
    PCA9632_SetPWM(pin,0,pca9632_inverted);
    pca9632_pin_pwm_value[pin] = PCA9632_GetPWMvalue(0, pca9632_inverted);
  }
  Response_P(PSTR("{\"PCA9632\":{\"RESET\":\"OK\"}}"));
}

uint8_t PCA9632_GetPWMvalue(uint8_t pwm, bool inverted) {
  uint8_t pwm_val = pwm;
  if (inverted) {
    pwm_val = 255-pwm;
  }
  return pwm_val;
}

void PCA9632_SetPWM_Reg(uint8_t pin, uint16_t on, uint16_t off) {
  uint8_t led_reg = PCA9685_REG_LED0_ON_L + 4 * pin;
  uint32_t led_data = 0;
  I2cWrite8(USE_PCA9632_ADDR, led_reg, on);
  I2cWrite8(USE_PCA9632_ADDR, led_reg+1, (on >> 8));
  I2cWrite8(USE_PCA9632_ADDR, led_reg+2, off);
  I2cWrite8(USE_PCA9632_ADDR, led_reg+3, (off >> 8));
}

void PCA9632_SetPWM(uint8_t pin, uint8_t pwm, bool inverted) {
  uint8_t pwm_val = PCA9685_GetPWMvalue(pwm, inverted);
  if (255 == pwm_val) {
    PCA9632_SetPWM_Reg(pin, 255, 0); // Special use additional bit causes channel to turn on completely without PWM
  } else {
    PCA9632_SetPWM_Reg(pin, 0, pwm_val);
  }
  pca9632_pin_pwm_value[pin] = pwm_val;
}

bool PCA9632_Command(void)
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

  if (!strcmp(ArgV(argument, 1),"RESET"))  {  PCA9632_Reset(); return serviced; }

  if (!strcmp(ArgV(argument, 1),"STATUS"))  { PCA9632_OutputTelemetry(false); return serviced; }

  if (!strcmp(ArgV(argument, 1),"INVERT")) {
    if (paramcount > 1) {
      pca9632_inverted = (1 == atoi(ArgV(argument, 2)));
      Response_P(PSTR("{\"PCA9632\":{\"INVERT\":%i, \"Result\":\"OK\"}}"), pca9632_inverted?1:0);
      return serviced;
    } else { // No parameter was given for invert, so we return current setting
      Response_P(PSTR("{\"PCA9632\":{\"INVERT\":%i}}"), pca9632_inverted?1:0);
      return serviced;
    }
  }
  if (!strcmp(ArgV(argument, 1),"PWM")) {
    if (paramcount > 1) {
      uint8_t pin = atoi(ArgV(argument, 2));
      if (paramcount > 2) {
        if (!strcmp(ArgV(argument, 3), "ON")) {
          PCA9632_SetPWM(pin, 255, pca9632_inverted);
          Response_P(PSTR("{\"PCA9632\":{\"PIN\":%i,\"PWM\":%i}}"),pin,255);
          serviced = true;
          return serviced;
        }
        if (!strcmp(ArgV(argument, 3), "OFF")) {
          PCA9632_SetPWM(pin, 0, pca9632_inverted);
          Response_P(PSTR("{\"PCA9632\":{\"PIN\":%i,\"PWM\":%i}}"),pin,0);
          serviced = true;
          return serviced;
        }
        uint16_t pwm = atoi(ArgV(argument, 3));
        if ((pin >= 0 && pin <= 3 || pin==61) && (pwm >= 0 && pwm <= 255)) {
          PCA9632_SetPWM(pin, pwm, pca9685_inverted);
          Response_P(PSTR("{\"PCA9632\":{\"PIN\":%i,\"PWM\":%i}}"),pin,pwm);
          serviced = true;
          return serviced;
        }
      }
    }
  }
  return serviced;
}

void PCA9632_OutputTelemetry(bool telemetry)
{
  ResponseAppend_P(PSTR("\"INVERT\":%i,"), pca9632_inverted?1:0);
  for (uint32_t pin=0;pin<4;pin++) {
    uint16_t pwm_val = PCA9632_GetPWMvalue(pca9632_pin_pwm_value[pin], pca9632_inverted); // return logical (possibly inverted) pwm value
    ResponseAppend_P(PSTR("\"PWM%i\":%i,"),pin,pwm_val);
  }
  ResponseAppend_P(PSTR("\"END\":1}}"));
  if (telemetry) {
    MqttPublishTeleSensor();
  }
}

bool Xdrv15(uint32_t function)
{
  if (!I2cEnabled(XI2C_75)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    PCA9632_Detect();
  }
  else if (pca9632_detected) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        if (TasmotaGlobal.tele_period == 0) {
          PCA9632_OutputTelemetry(true);
        }
        break;
      case FUNC_COMMAND_DRIVER:
        if (XDRV_91 == XdrvMailbox.index) {
          result = PCA9632_Command();
        }
        break;
    }
  }
  return result;
}

#endif // USE_PCA9632
#endif // USE_IC2
