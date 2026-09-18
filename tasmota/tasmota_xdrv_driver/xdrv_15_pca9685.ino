/*
  xdrv_15_pca9685.ino - Support for I2C PCA9685 12bit 16 pin hardware PWM driver on Tasmota

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
#ifdef USE_PCA9685
/*********************************************************************************************\
 * PCA9685 - 16-channel 12-bit pwm driver
 *
 * I2C Address: 0x40 .. 0x47
\*********************************************************************************************/

#define XDRV_15                     15
#define XI2C_01                     1  // See I2CDEVICES.md

#define PCA9685_REG_MODE1           0x00
#define PCA9685_REG_LED0_ON_L       0x06
#define PCA9685_REG_PRE_SCALE       0xFE

#ifndef USE_PCA9685_ADDR
  #define USE_PCA9685_ADDR          0x40
#endif
#ifndef USE_PCA9685_FREQ
  #define USE_PCA9685_FREQ          50
#endif

bool pca9685_inverted = false; // invert PWM for open-collector load
bool pca9685_detected = false;
uint8_t pca9685_bus;
uint16_t pca9685_freq = USE_PCA9685_FREQ;
uint16_t pca9685_pin_pwm_value[16];

/********************************************************************************************/

bool PCA9685_I2cValidRead8(uint8_t *data, uint8_t reg) {
  return I2cValidRead8(data, USE_PCA9685_ADDR, reg, pca9685_bus);
}

uint8_t PCA9685_I2cRead8(uint8_t reg) {
  return I2cRead8(USE_PCA9685_ADDR, reg, pca9685_bus);
}

bool PCA9685_I2cWrite8(uint8_t reg, uint32_t val) {
  return I2cWrite8(USE_PCA9685_ADDR, reg, val, pca9685_bus);
}

/********************************************************************************************/

void PCA9685_Detect(void) {
  for (pca9685_bus = 0; pca9685_bus < MAX_I2C; pca9685_bus++) {
    if (!I2cSetDevice(USE_PCA9685_ADDR, pca9685_bus)) { continue; }

    uint8_t buffer;
    if (PCA9685_I2cValidRead8(&buffer, PCA9685_REG_MODE1)) {
      PCA9685_I2cWrite8(PCA9685_REG_MODE1, 0x20);
      if (PCA9685_I2cValidRead8(&buffer, PCA9685_REG_MODE1)) {
        if (0x20 == buffer) {
          pca9685_detected = true;
          I2cSetActiveFound(USE_PCA9685_ADDR, "PCA9685", pca9685_bus);
          PCA9685_Reset(); // Reset the controller
          return;
        }
      }
    }
  }
}

void PCA9685_Reset(void)
{
  PCA9685_I2cWrite8(PCA9685_REG_MODE1, 0x80);  // Disable ALLCALL (uses I2C address 0x70)
  PCA9685_SetPWMfreq(USE_PCA9685_FREQ);
  pca9685_inverted = false;
  for (uint32_t pin=0;pin<16;pin++) {
    PCA9685_SetPWM(pin,0,pca9685_inverted);
    pca9685_pin_pwm_value[pin] = PCA9685_GetPWMvalue(0, pca9685_inverted);
  }
  Response_P(PSTR("{\"PCA9685\":{\"RESET\":\"OK\"}}"));
}

uint16_t PCA9685_GetPWMvalue(uint16_t pwm, bool inverted) {
  uint16_t pwm_val = pwm;
  if (inverted) {
    pwm_val = 4096-pwm;
  }
  return pwm_val;
}

void PCA9685_SetPWMfreq(double freq) {
/*
 7.3.5 from datasheet
 prescale value = round(25000000/(4096*freq))-1;
 */
  if (freq > 23 && freq < 1527) {
   pca9685_freq=freq;
  } else {
   pca9685_freq=50;
  }
  uint8_t pre_scale_osc = round(25000000/(4096*pca9685_freq))-1;
  if (1526 == pca9685_freq) pre_scale_osc=0xFF; // force setting for 24hz because rounding causes 1526 to be 254
  uint8_t current_mode1 = PCA9685_I2cRead8(PCA9685_REG_MODE1); // read current value of MODE1 register
  uint8_t sleep_mode1 = (current_mode1&0x7F) | 0x10; // Determine register value to put PCA to sleep
  PCA9685_I2cWrite8(PCA9685_REG_MODE1, sleep_mode1); // Let's sleep a little
  PCA9685_I2cWrite8(PCA9685_REG_PRE_SCALE, pre_scale_osc); // Set the pre-scaler
  PCA9685_I2cWrite8(PCA9685_REG_MODE1, current_mode1 | 0xA0); // Reset MODE1 register to original state and enable auto increment
}

void PCA9685_SetPWM_Reg(uint8_t pin, uint16_t on, uint16_t off) {
  uint8_t led_reg = PCA9685_REG_LED0_ON_L + 4 * pin;
  uint32_t led_data = 0;
  PCA9685_I2cWrite8(led_reg, on);
  PCA9685_I2cWrite8(led_reg+1, (on >> 8));
  PCA9685_I2cWrite8(led_reg+2, off);
  PCA9685_I2cWrite8(led_reg+3, (off >> 8));
}

void PCA9685_SetPWM(uint8_t pin, uint16_t pwm, bool inverted) {
  uint16_t pwm_val = PCA9685_GetPWMvalue(pwm, inverted);
  if (4096 == pwm_val) {
    PCA9685_SetPWM_Reg(pin, 4096, 0); // Special use additional bit causes channel to turn on completely without PWM
  } else {
    PCA9685_SetPWM_Reg(pin, 0, pwm_val);
  }
  pca9685_pin_pwm_value[pin] = pwm_val;
}

bool PCA9685_Command(void) {
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
    if ((' ' == XdrvMailbox.data[ca]) || ('=' == XdrvMailbox.data[ca])) { 
      XdrvMailbox.data[ca] = ',';
    }
    if (',' == XdrvMailbox.data[ca]) { 
      paramcount++;
    }
  }
  UpperCase(XdrvMailbox.data,XdrvMailbox.data);

  if (!strcmp(ArgV(argument, 1), "RESET")) {
    // Driver15 reset
    PCA9685_Reset();
    return serviced;
  }

  if (!strcmp(ArgV(argument, 1), "STATUS")) {
    // Driver15 status
    ResponseTime_P(PSTR(""));
    PCA9685_Show(1);
    ResponseJsonEnd();
    return serviced;
  }

  if (!strcmp(ArgV(argument, 1), "INVERT")) {
    // Driver15 invert   - Show current state
    // Driver15 invert 0 - Invert OFF
    // Driver15 invert 1 - Invert ON
    if (paramcount > 1) {
      pca9685_inverted = (1 == atoi(ArgV(argument, 2)));
    }
    Response_P(PSTR("{\"PCA9685\":{\"INVERT\":%i"), pca9685_inverted ? 1 : 0);
    if (paramcount > 1) {
      ResponseAppend_P(PSTR(",\"Result\":\"OK\""));
    }
    ResponseJsonEndEnd();
    return serviced;
  }

  if (!strcmp(ArgV(argument, 1), "PWMF")) {
    // Driver15 pwmf    - Show current frequency
    // Driver15 pwmf 50 - Frequency set to 50Hz
    bool valid = false;
    if (paramcount > 1) {
      uint16_t new_freq = atoi(ArgV(argument, 2));
      if ((new_freq >= 24) && (new_freq <= 1526)) {
        PCA9685_SetPWMfreq(new_freq);
        valid = true;
      }
    }
    Response_P(PSTR("{\"PCA9685\":{\"PWMF\":%i"), pca9685_freq);
    if (valid) {
      ResponseAppend_P(PSTR(",\"Result\":\"OK\""));
    }
    ResponseJsonEndEnd();
    return serviced;
  }

  if (!strcmp(ArgV(argument, 1),"PWM")) {
    // Driver15 pwm 0..15 ON
    // Driver15 pwm 0..15 OFF
    // Driver15 pwm 0..15 0..4096
    if (paramcount > 1) {
      uint8_t pin = atoi(ArgV(argument, 2));
      if (paramcount > 2) {
        uint16_t pwm = atoi(ArgV(argument, 3));
        if (!strcmp(ArgV(argument, 3), "ON")) {
          pwm = 4096;
        }
        else if (!strcmp(ArgV(argument, 3), "OFF")) {
          pwm = 0;
        }
        if ((pin >= 0 && pin <= 15 || pin == 61) && (pwm >= 0 && pwm <= 4096)) {
          PCA9685_SetPWM(pin, pwm, pca9685_inverted);
          Response_P(PSTR("{\"PCA9685\":{\"PIN\":%i,\"PWM\":%i}}"), pin, pwm);
          serviced = true;
          return serviced;
        }
      }
    }
  }
  return serviced;
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void PCA9685_Show(bool json) {
  ResponseAppend_P(PSTR(",\"PCA9685\":{\"PWM_FREQ\":%i"),pca9685_freq);
  ResponseAppend_P(PSTR(",\"INVERT\":%i"), pca9685_inverted?1:0);
  for (uint32_t pin = 0; pin < 16; pin++) {
    uint16_t pwm_val = PCA9685_GetPWMvalue(pca9685_pin_pwm_value[pin], pca9685_inverted); // return logical (possibly inverted) pwm value
    ResponseAppend_P(PSTR(",\"PWM%i\":%i"), pin, pwm_val);
  }
  ResponseJsonEnd();
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv15(uint32_t function) {
  if (!I2cEnabled(XI2C_01)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    PCA9685_Detect();
  }
  else if (pca9685_detected) {
    switch (function) {
      case FUNC_JSON_APPEND:
        PCA9685_Show(1);
        break;
      case FUNC_COMMAND_DRIVER:
        if (XDRV_15 == XdrvMailbox.index) {
          result = PCA9685_Command();
        }
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif // USE_PCA9685
#endif // USE_IC2
