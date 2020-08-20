/*
  xdrv_15_pca9685.ino - Support for I2C PCA9685 12bit 16 pin hardware PWM driver on Tasmota

  Copyright (C) 2020  Andre Thomas and Theo Arends

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
*
* If more than one PCA9685 is attached, the addresses must be consecutive
\*********************************************************************************************/

#define XDRV_15                     15
#define XI2C_01                     1 // See I2CDEVICES.md

#define PCA9685_REG_MODE1           0x00
  #define PCA9685_REG_LED0_ON_L     0x06
#define PCA9685_REG_PRE_SCALE       0xFE

#ifndef USE_PCA9685_ADDR
  #define USE_PCA9685_ADDR          0x40 //Start address if multiples attached
#endif
#ifndef USE_PCA9685_FREQ
  #define USE_PCA9685_FREQ          50
#endif

#ifndef NUM_DEVICES
  #define NUM_DEVICES               2 //How many PCA9685 device are present. Default is 1
#endif
#ifndef CHANNELS_PER_DEVICE
  #define CHANNELS_PER_DEVICE       16
#endif

/**
* 12 bits PWM to CIE Luminance conversion
* L* = 116(Y/Yn)^1/3 - 16 , Y/Yn > 0.008856
* L* = 903.3(Y/Yn), Y/Yn <= 0.008856
*/
//lookup table for 256 CIE lab brightness corrected values with 12 bit resolution
const uint16_t CIEL_8_12[] PROGMEM = {
  0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 18, 20, 21, 23, 25, 27, 28, 30, 32, 34, 36, 37, 39, 41, 43, 45, 47, 49, 52, 54, 56, 59, 61, 64, 66, 69, 72, 75, 77, 80, 83, 87, 90, 93, 97, 100, 103, 107, 111, 115, 118, 122, 126,
  131, 135, 139, 144, 148, 153, 157, 162, 167, 172, 177, 182, 187, 193, 198, 204, 209, 215, 221, 227, 233, 239, 246, 252, 259, 265, 272, 279, 286, 293, 300, 308, 315, 323, 330, 338, 346, 354, 362, 371,
  379, 388, 396, 405, 414, 423, 432, 442, 451, 461, 471, 480, 490, 501, 511, 521, 532, 543, 554, 565, 576, 587, 599, 610, 622, 634, 646, 658, 670, 683, 696, 708, 721, 734, 748, 761, 775, 789, 802, 817,
  831, 845, 860, 875, 890, 905, 920, 935, 951, 967, 983, 999, 1015, 1032, 1048, 1065, 1082, 1099, 1117, 1134, 1152, 1170, 1188, 1206, 1225, 1243, 1262, 1281, 1301, 1320, 1340, 1359, 1379, 1400, 1420,
  1441, 1461, 1482, 1504, 1525, 1547, 1568, 1590, 1613, 1635, 1658, 1681, 1704, 1727, 1750, 1774, 1798, 1822, 1846, 1871, 1896, 1921, 1946, 1971, 1997, 2023, 2049, 2075, 2101, 2128, 2155, 2182, 2210,
  2237, 2265, 2293, 2322, 2350, 2379, 2408, 2437, 2467, 2497, 2527, 2557, 2587, 2618, 2649, 2680, 2712, 2743, 2775, 2807, 2840, 2872, 2905, 2938, 2972, 3006, 3039, 3074, 3108, 3143, 3178, 3213, 3248,
  3284, 3320, 3356, 3393, 3430, 3467, 3504, 3542, 3579, 3617, 3656, 3694, 3733, 3773, 3812, 3852, 3892, 3932, 3973, 4013, 4055, 4095 };

//Array to keep track of current lookup value index
uint8_t pca9685_pin_lookup_value[NUM_DEVICES][CHANNELS_PER_DEVICE];
#ifndef FADE_DELAY
  #define FADE_DELAY 10 //In milliseconds
#endif

bool pca9685_detected[NUM_DEVICES];
uint16_t pca9685_freq = USE_PCA9685_FREQ;
uint16_t pca9685_pin_pwm_value[NUM_DEVICES][CHANNELS_PER_DEVICE];

void PCA9685_Detect(void)
{
  for (int i = 0; i < NUM_DEVICES; i++)
  {
    PCA9685_DetectHelper(USE_PCA9685_ADDR + i, pca9685_detected[i]);
  }
}

void PCA9685_DetectHelper(uint8_t address, bool deviceDetected)
{
  if (I2cActive(address))
  {
    return;
  }

  deviceDetected = false;
  uint8_t buffer;
  if (I2cValidRead8(&buffer, address, PCA9685_REG_MODE1))
  {
    I2cWrite8(address, PCA9685_REG_MODE1, 0x20);
    if (I2cValidRead8(&buffer, address, PCA9685_REG_MODE1))
    {
      if (0x20 == buffer)
      {
        deviceDetected = true;
        I2cSetActiveFound(address, "PCA9685");
        PCA9685_Reset(); // Reset the controller
      }
    }
  }
}

bool detectHighChip()
{
  for (int i = 0; i < NUM_DEVICES; i++)
  {
    detectHighChipHelper(USE_PCA9685_ADDR + i, pca9685_detected[i]);
  }
}

bool detectHighChipHelper(uint8_t address, bool deviceDetected)
{
  if (I2cActive(address))
  {
    return false;
  }

  deviceDetected = false;
  uint8_t buffer;
  if (I2cValidRead8(&buffer, address, PCA9685_REG_MODE1))
  {
    I2cWrite8(address, PCA9685_REG_MODE1, 0x20);
    if (I2cValidRead8(&buffer, address, PCA9685_REG_MODE1))
    {
      if (0x20 == buffer)
      {
        deviceDetected = true;
        I2cSetActiveFound(address, "PCA9685");
        PCA9685_Reset(); // Reset the controller
      }
    }
  }
}

void PCA9685_Reset(void)
{
  for (int i = 0; i < NUM_DEVICES; i++)
  {
    PCA9685_ResetHelper(USE_PCA9685_ADDR + i, &pca9685_pin_pwm_value[i][0]);
  }
}

void PCA9685_ResetHelper(uint8_t address, uint16_t *pinArray)
{
  I2cWrite8(address, PCA9685_REG_MODE1, 0x80);
  PCA9685_SetPWMfreq(USE_PCA9685_FREQ);

  for (uint32_t pin = 0; pin < CHANNELS_PER_DEVICE; pin++)
  {
    PCA9685_SetPWM(pin, 0, false);
    pinArray[pin] = 0;
  }
  Response_P(PSTR("{\"PCA9685\":{\"RESET\":\"OK\"}}"));
}

void PCA9685_SetPWMfreq(double freq)
{
  for (int i = 0; i < NUM_DEVICES; i++)
  {
    PCA9685_SetPWMfreqHelper(USE_PCA9685_ADDR + i, freq);
  }
}

void PCA9685_SetPWMfreqHelper(uint8_t address, double freq)
{
  /*
  7.3.5 from datasheet
  prescale value = round(25000000/(4096*freq))-1;
  */
  if (freq > 23 && freq < 1527)
  {
    pca9685_freq = freq;
  }
  else
  {
    pca9685_freq = 50;
  }

  uint8_t pre_scale_osc = round(25000000 / (4096 * pca9685_freq)) - 1;
  if (1526 == pca9685_freq)
    pre_scale_osc = 0xFF; // force setting for 24hz because rounding causes 1526 to be 254

  uint8_t current_mode1 = I2cRead8(address, PCA9685_REG_MODE1); // read current value of MODE1 register
  uint8_t sleep_mode1 = (current_mode1 & 0x7F) | 0x10;          // Determine register value to put PCA to sleep

  I2cWrite8(address, PCA9685_REG_MODE1, sleep_mode1);          // Let's sleep a little
  I2cWrite8(address, PCA9685_REG_PRE_SCALE, pre_scale_osc);    // Set the pre-scaler
  I2cWrite8(address, PCA9685_REG_MODE1, current_mode1 | 0xA0); // Reset MODE1 register to original state and enable auto increment
}

void PCA9685_SetPWM_Reg(uint8_t pin, uint16_t on, uint16_t off)
{
  uint8_t deviceOffset = pin / CHANNELS_PER_DEVICE;
  uint8_t devicePin = pin - (deviceOffset * CHANNELS_PER_DEVICE);

  PCA9685_SetPWM_RegHelper(USE_PCA9685_ADDR + deviceOffset, devicePin, on, off);
}

void PCA9685_SetPWM_RegHelper(uint8_t address, uint8_t pin, uint16_t on, uint16_t off)
{
  uint8_t led_reg = PCA9685_REG_LED0_ON_L + 4 * pin;
  uint32_t led_data = 0;

  I2cWrite8(address, led_reg, on);
  I2cWrite8(address, led_reg + 1, (on >> 8));
  I2cWrite8(address, led_reg + 2, off);
  I2cWrite8(address, led_reg + 3, (off >> 8));
}

void PCA9685_SetPWM(uint8_t pin, uint16_t pwm, bool inverted)
{
  uint8_t deviceOffset = pin / CHANNELS_PER_DEVICE;
  uint8_t devicePin = pin - (deviceOffset * CHANNELS_PER_DEVICE);

  if (4096 == pwm)
  {
    PCA9685_SetPWM_Reg(pin, 4096, 0); // Special use additional bit causes channel to turn on completely without PWM
  }
  else
  {
    PCA9685_SetPWM_Reg(pin, 0, pwm);
  }
  pca9685_pin_pwm_value[deviceOffset][devicePin] = pwm;
}

bool PCA9685_Command(void)
{
  bool serviced = true;
  bool validpin = false;
  uint8_t paramcount = 0;

  if (XdrvMailbox.data_len > 0)
  {
    paramcount = 1;
  }
  else
  {
    serviced = false;
    return serviced;
  }

  char sub_string[XdrvMailbox.data_len];
  for (uint32_t ca = 0; ca < XdrvMailbox.data_len; ca++)
  {
    if ((' ' == XdrvMailbox.data[ca]) || ('=' == XdrvMailbox.data[ca]))
    {
      XdrvMailbox.data[ca] = ',';
    }

    if (',' == XdrvMailbox.data[ca])
    {
      paramcount++;
    }
  }

  UpperCase(XdrvMailbox.data, XdrvMailbox.data);
  if (!strcmp(subStr(sub_string, XdrvMailbox.data, ",", 1), "RESET"))
  {
    PCA9685_Reset();
    return serviced;
  }

  if (!strcmp(subStr(sub_string, XdrvMailbox.data, ",", 1), "STATUS"))
  {
    PCA9685_OutputTelemetry(false);
    return serviced;
  }

  if (!strcmp(subStr(sub_string, XdrvMailbox.data, ",", 1), "PWMF"))
  {
    if (paramcount > 1)
    {
      uint16_t new_freq = atoi(subStr(sub_string, XdrvMailbox.data, ",", 2));
      if ((new_freq >= 24) && (new_freq <= 1526))
      {
        PCA9685_SetPWMfreq(new_freq);
        Response_P(PSTR("{\"PCA9685\":{\"PWMF\":%i, \"Result\":\"OK\"}}"), new_freq);

        return serviced;
      }
    }
    else
    { // No parameter was given for setfreq, so we return current setting
      Response_P(PSTR("{\"PCA9685\":{\"PWMF\":%i}}"), pca9685_freq);
      return serviced;
    }
  }

  if (!strcmp(subStr(sub_string, XdrvMailbox.data, ",", 1), "PWM"))
  {
    if (paramcount > 1)
    {
      uint8_t pin = atoi(subStr(sub_string, XdrvMailbox.data, ",", 2));
      if (paramcount > 2)
      {
        if (!strcmp(subStr(sub_string, XdrvMailbox.data, ",", 3), "ON"))
        {
          PCA9685_SetPWM(pin, 4096, false);
          Response_P(PSTR("{\"PCA9685\":{\"PIN\":%i,\"PWM\":%i}}"), pin, 4096);

          serviced = true;
          return serviced;
        }

        if (!strcmp(subStr(sub_string, XdrvMailbox.data, ",", 3), "OFF"))
        {
          PCA9685_SetPWM(pin, 0, false);
          Response_P(PSTR("{\"PCA9685\":{\"PIN\":%i,\"PWM\":%i}}"), pin, 0);

          serviced = true;
          return serviced;
        }

        uint16_t pwm = atoi(subStr(sub_string, XdrvMailbox.data, ",", 3));
        if ((pin >= 0 && pin <= (CHANNELS_PER_DEVICE * NUM_DEVICES - 1) && (pwm >= 0 && pwm <= 4096)))
        {
          PCA9685_SetPWM(pin, pwm, false);
          Response_P(PSTR("{\"PCA9685\":{\"PIN\":%i,\"PWM\":%i}}"), pin, pwm);

          serviced = true;
          return serviced;
        }
      }
    }
  }

  if (!strcmp(subStr(sub_string, XdrvMailbox.data, ",", 1), "LEDFADE"))
  {
    if (paramcount > 1)
    {
      uint8_t pin = atoi(subStr(sub_string, XdrvMailbox.data, ",", 2));
      if (paramcount > 2)
      {
        uint16_t fade_to_value = atoi(subStr(sub_string, XdrvMailbox.data, ",", 3));
        if ((pin >= 0 && pin <= (CHANNELS_PER_DEVICE * NUM_DEVICES - 1) && (fade_to_value >= 0 && fade_to_value <= 255)))
        {
          PCA9685_LEDFade(pin, fade_to_value);
          Response_P(PSTR("{\"PCA9685\":{\"PIN\":%i,\"LEDFADE\":%i}}"), pin, fade_to_value);

          serviced = true;
          return serviced;
        }
      }
    }
  }

  return serviced;
}

void PCA9685_OutputTelemetry(bool telemetry)
{
  ResponseTime_P(PSTR(",\"PCA9685\":{\"PWM_FREQ\":%i,"), pca9685_freq);

  for (uint32_t device = 0; device < NUM_DEVICES; device++)
  {
    for (uint32_t pin = 0; pin < CHANNELS_PER_DEVICE; pin++)
    {
      ResponseAppend_P(PSTR("\"PWM%i\":%i,"), pin + (device * CHANNELS_PER_DEVICE), pca9685_pin_pwm_value[device][pin]);
    }
  }

  ResponseAppend_P(PSTR("\"END\":1}}"));
  if (telemetry)
  {
    MqttPublishTeleSensor();
  }
}

bool Xdrv15(uint8_t function)
{
  if (!I2cEnabled(XI2C_01))
  {
    return false;
  }

  bool result = false;
  if (FUNC_INIT == function)
  {
    PCA9685_Detect();
  }

  else if (pca9685_detected)
  {
    switch (function)
    {
    case FUNC_EVERY_SECOND:
      if (tele_period == 0)
      {
        PCA9685_OutputTelemetry(true);
      }
      break;
    case FUNC_COMMAND_DRIVER:
      if (XDRV_15 == XdrvMailbox.index)
      {
        result = PCA9685_Command();
      }
      break;
    }
  }
  return result;
}

/*
* Sets the brightness from 0 to 255 using the CIE1931 lookup value
*/
void PCA9685_LEDBrightness(uint8_t pin, uint8_t brightness, bool storeCurrentIndex = true)
{
  uint16_t pwmValue = pgm_read_word_near(CIEL_8_12 + brightness);
  
  uint8_t deviceOffset = pin / CHANNELS_PER_DEVICE;
  uint8_t devicePin = pin - (deviceOffset * CHANNELS_PER_DEVICE);

  PCA9685_SetPWM(pin, pwmValue, false);

  if (storeCurrentIndex)
      pca9685_pin_lookup_value[deviceOffset][devicePin] = brightness;
}

/*
* Fades from current brightness to the target. Will fade up or down depending on desired target value.
*/
void PCA9685_LEDFade(uint8_t pin, uint8_t target)
{
  uint8_t deviceOffset = pin / CHANNELS_PER_DEVICE;
  uint8_t devicePin = pin - (deviceOffset * CHANNELS_PER_DEVICE);

  uint8_t currentIndex = pca9685_pin_lookup_value[deviceOffset][devicePin];

  if (currentIndex == target) //Nothing to change
    return;
  
  int step = 1;  //Fade up by default
  if (currentIndex > target) 
    step = -1;  //Fade down
  
  uint32_t now = millis();
  for (int i = currentIndex+step; i != target+step; i+=step)
  {
    while (millis() < (now + FADE_DELAY))
    {
      yield();
    }
    PCA9685_LEDBrightness(pin, i, false);
    now = millis();
  }

  //Remember the last current setting
  pca9685_pin_lookup_value[deviceOffset][devicePin] = target;
}

#endif // USE_PCA9685
#endif // USE_IC2