/*
  xdrv_64_pca9632.ino - Support for I2C PCA9632 4-channel 8-bit hardware PWM driver on Tasmota

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
 * PCA9632 - 4-channel 8-bit pwm driver
 *
 * I2C Address: 0x60 .. 0x63
\*********************************************************************************************/

#define XDRV_64                     64
#define XI2C_75                     75  // See I2CDEVICES.md

#define PCA9632_REG_MODE1           0x00
#define PCA9632_REG_MODE2           0x01
#define PCA9632_REG_PWM_BASE        0x02
#define PCA9632_REG_PWM_1           PCA9632_REG_PWM_BASE + 0
#define PCA9632_REG_PWM_2           PCA9632_REG_PWM_BASE + 1
#define PCA9632_REG_PWM_3           PCA9632_REG_PWM_BASE + 2
#define PCA9632_REG_PWM_4           PCA9632_REG_PWM_BASE + 3
#define PCA9632_REG_GRPPWM          0x06
#define PCA9632_REG_GRPGREQ         0x07
#define PCA9632_REG_LEDOUT          0x08
#define PCA9632_AUTO_INC            0x80

#ifndef USE_PCA9632_ADDR
  #define USE_PCA9632_ADDR          0x60
#endif

#ifndef USE_PCA9632_CM_0
  #define USE_PCA9632_CM_0 0
#endif

#ifndef USE_PCA9632_CM_1
  #define USE_PCA9632_CM_1 1
#endif

#ifndef USE_PCA9632_CM_2
  #define USE_PCA9632_CM_2 2
#endif

#ifndef USE_PCA9632_CM_3
  #define USE_PCA9632_CM_3 3
#endif

bool pca9632_inverted = false; // invert PWM for open-collector load
bool pca9632_detected = false;
uint8_t pca9632_pin_pwm_value[4];

bool PCA9632_Detect(void) {
  if (I2cSetDevice(USE_PCA9632_ADDR)) {
    uint8_t buffer;
    if (I2cValidRead8(&buffer, USE_PCA9632_ADDR, PCA9632_REG_MODE1)) {
      I2cWrite8(USE_PCA9632_ADDR, PCA9632_REG_MODE1, 0x10);
      if (I2cValidRead8(&buffer, USE_PCA9632_ADDR, PCA9632_REG_MODE1)) {
        if (0x10 == buffer) {
          I2cSetActiveFound(USE_PCA9632_ADDR, "PCA9632");
          PCA9632_Reset(); // Reset the controller
          return pca9632_detected = true;
        }
      }
    }
  }

  return false;
}

void PCA9632_Init(void) {
  // configure none inverted and totem pole
  I2cWrite8(USE_PCA9632_ADDR, PCA9632_REG_MODE2, 0x14);

  // turn off sleep mode
  I2cWrite8(USE_PCA9632_ADDR, PCA9632_REG_MODE1, 0x1);
}

void PCA9632_Reset(void) {
  I2cWrite8(USE_PCA9632_ADDR, PCA9632_REG_MODE1, 0x6);
  pca9632_inverted = false;
  for (uint8_t pin = 0; pin < 4; pin++) {
    PCA9632_SetPWM(pin, 0);
    pca9632_pin_pwm_value[pin] = 0;
  }
  Response_P(PSTR("{\"PCA9632\":{\"RESET\":\"OK\"}}"));
}

bool PCA9632_SetInvert(bool on) {
  uint8_t buffer;
  if(I2cValidRead8(&buffer, USE_PCA9632_ADDR, PCA9632_REG_MODE2)) {
    I2cWrite8(USE_PCA9632_ADDR, PCA9632_REG_MODE2, buffer | ((on ? 1 : 0) >> 4));
  }
  return on;
}

bool PCA9632_SetPWM(uint8_t pin, uint8_t pwm) {

  uint8_t pin_mapping = PCA9632_PinMapping(pin);
  I2cWrite8(USE_PCA9632_ADDR, PCA9632_REG_PWM_BASE + pin_mapping, pwm);
  pca9632_pin_pwm_value[pin_mapping] = pwm;

  return pwm > 0;

}

bool PCA9632_SetPWM_Bulk(uint8_t *pwm, uint16_t len) {
  uint8_t buffer[4];

  // map the pwm values to the final pins
  for (uint8_t pin = 0; pin < 4; pin++) {
    uint8_t pin_mapping = PCA9632_PinMapping(pin);
    buffer[pin_mapping] = pwm[pin];
  }

  I2cWriteBuffer(USE_PCA9632_ADDR, PCA9632_REG_PWM_BASE | PCA9632_AUTO_INC, buffer, len);

  // set the pwm values for later use
  bool enable = false;
  for (uint8_t pin = 0; pin < 4; pin++) {
    uint8_t value = buffer[pin];

    pca9632_pin_pwm_value[pin] = value;

    if (value > 0) {
      enable |= true;
    }
  }

  return enable;
}

void PCA9632_Enable(bool enable) {
  DEBUG_TRACE_LOG(PSTR("DRV: PCA9632 enable %d"), enable);
  I2cWrite8(USE_PCA9632_ADDR, PCA9632_REG_LEDOUT, enable ? 0xFF : 0x0);
}

bool PCA9632_Command(void) {
  bool serviced = true;
  bool validpin = false;
  uint8_t paramcount = 0;
  if (XdrvMailbox.data_len > 0) {
    paramcount = 1;
  } else {
    serviced = false;
    return serviced;
  }
  char argument[XdrvMailbox.data_len];
  for (uint32_t ca = 0; ca < XdrvMailbox.data_len; ca++) {
    if ((' ' == XdrvMailbox.data[ca]) || ('=' == XdrvMailbox.data[ca])) { XdrvMailbox.data[ca] = ','; }
    if (',' == XdrvMailbox.data[ca]) { paramcount++; }
  }
  UpperCase(XdrvMailbox.data, XdrvMailbox.data);

  if (!strcmp(ArgV(argument, 1),"RESET"))  {  PCA9632_Reset(); return serviced; }

  if (!strcmp(ArgV(argument, 1),"STATUS"))  { PCA9632_OutputTelemetry(false); return serviced; }

  if (!strcmp(ArgV(argument, 1),"INVERT")) {
    if (paramcount > 1) {
      pca9632_inverted = PCA9632_SetInvert(1 == atoi(ArgV(argument, 2)));
      Response_P(PSTR("{\"PCA9632\":{\"INVERT\":%i, \"Result\":\"OK\"}}"), pca9632_inverted);
      return serviced;
    } else { // No parameter was given for invert, so we return current setting
      Response_P(PSTR("{\"PCA9632\":{\"INVERT\":%i}}"), pca9632_inverted);
      return serviced;
    }
  }
  if (!strcmp(ArgV(argument, 1),"PWM")) {
    if (paramcount > 1) {
      uint8_t pin = atoi(ArgV(argument, 2));
      if (paramcount > 2) {
        if (!strcmp(ArgV(argument, 3), "ON")) {
          PCA9632_SetPWM(pin, 255);
          Response_P(PSTR("{\"PCA9632\":{\"PIN\":%i,\"PWM\":%i}}"), pin, 255);
          serviced = true;
          return serviced;
        }
        if (!strcmp(ArgV(argument, 3), "OFF")) {
          PCA9632_SetPWM(pin, 0);
          Response_P(PSTR("{\"PCA9632\":{\"PIN\":%i,\"PWM\":%i}}"), pin, 0);
          serviced = true;
          return serviced;
        }
        uint16_t pwm = atoi(ArgV(argument, 3));
        if ((pin >= 0 && pin <= 3) && (pwm >= 0 && pwm <= 255)) {
          PCA9632_SetPWM(pin, pwm);
          Response_P(PSTR("{\"PCA9632\":{\"PIN\":%i,\"PWM\":%i}}"), pin, pwm);
          serviced = true;
          return serviced;
        }
      }
    }
  }
  if (!strcmp(ArgV(argument, 1),"ENABLE")) {
    PCA9632_Enable(true);
    Response_P(PSTR("{\"PCA9632\":{\"ENABLE\":true}}"));
  }
  if (!strcmp(ArgV(argument, 1),"DISABLE")) {
    PCA9632_Enable(false);
    Response_P(PSTR("{\"PCA9632\":{\"ENABLE\":false}}"));
  }
  return serviced;
}

void PCA9632_OutputTelemetry(bool telemetry) {
  ResponseAppend_P(PSTR("\"INVERT\":%i,"), pca9632_inverted?1:0);
  for (uint32_t pin = 0; pin < 4; pin++) {
    uint8_t pin_mapping = PCA9632_PinMapping(pin);
    ResponseAppend_P(PSTR("\"PWM%i\":%i,"), pin_mapping, pca9632_pin_pwm_value[pin_mapping]);
  }
  ResponseAppend_P(PSTR("\"END\":1}}"));
  if (telemetry) {
    MqttPublishTeleSensor();
  }
}

uint8_t PCA9632_PinMapping(uint8_t pin) {
  switch(pin) {
    case 0:
      return USE_PCA9632_CM_0;
    case 1:
      return USE_PCA9632_CM_1;
    case 2:
      return USE_PCA9632_CM_2;
    case 3:
      return USE_PCA9632_CM_3;
    default:
      return USE_PCA9632_CM_0;
  }
}

bool Xdrv64(uint32_t function) {
  if (!I2cEnabled(XI2C_75)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    if (PCA9632_Detect()) {
      PCA9632_Init();
    }
  }
  else if (pca9632_detected) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        if (TasmotaGlobal.tele_period == 0) {
          PCA9632_OutputTelemetry(true);
        }
        break;
      case FUNC_COMMAND_DRIVER:
        if (XDRV_64 == XdrvMailbox.index) {
          result = PCA9632_Command();
        }
        break;
    }
  }
  return result;
}

#endif // USE_PCA9632
#endif // USE_IC2
