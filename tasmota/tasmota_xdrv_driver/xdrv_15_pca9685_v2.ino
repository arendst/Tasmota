/*
  xdrv_15_pca9685_v2.ino - Support for I2C PCA9685 12bit 16 pin hardware PWM driver on Tasmota

  Copyright (C) 2021  Andre Thomas and Theo Arends
                2023  Fabrizio Amodio

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
#ifdef USE_PCA9685_V2
/*********************************************************************************************\
 * PCA9685 - 16-channel 12-bit pwm driver
 *
 * I2C Address: 0x40 .. 0x47
\*********************************************************************************************/
/*
  2023-06-05  v2.0  Changelog by F.Amodio
  - Code Refactoring

  - Support for multiple PCA9685 without breaking support for previuos driver version
    all command (PWM,INVERT,etc) now support the suffix from 0 to PCA9685_MAX_COUNT to address the board,
    without the suffix the command is relative to the board 0, e.g.
        DRIVER15 PWM 0 350     Board address 0 pin 0 value 350
        DRIVER15 PWM0 2 300    Board address 0 pin 2 value 300
        DRIVER15 PWM1 3 235    Board address 1 pin 3 value 235
        DRIVER15 RESET2        Reset Board 2

  - new command INTCLK to fine tuning the internal clock setting, unit: 0.1 MHz
    this is a not permanent setting!
        DRIVER15 INTCLK 270   Set to 27.0 MHz
        DRIVER15 INTCLK 250   Set to 25.0 Mhz (default value, use USE_PCA9685_INT_CLOCK to change it at compile time)

  - new command PWMTO to move all the required pin from the current pin position to a new one, all the move will be completed into the required time, so each motor will be stepped relative to timing

  - new command PWMSTOP to stop all running movement

  Updated Command List:

    DRIVER15 STATUS                   // Will return a JSON string containing all the current settings / parameters for all board
    DRIVER15 RESET[0-8]               // Reset to power-up settings - i.e. F=50hz and all pins in OFF state for a specific board
    DRIVER15 INVERT[0-8],pin[,0-1]    // print or set the inversion bit on pin of the specific board
    DRIVER15 INTCLK[0-8],clock        // where clock is the Interal Clock value in 1/10 MHz (default USE_PCA9685_INT_CLOCK = 250)
    DRIVER15 PMWF[0-8],frequency      // where frequency is the PWM frequency from 24 to 1526 in Hz
    DRIVER15 PWM[0-8],pin,pwmvalue    // where pin=LED, pin 0 through 15 and pwmvalue is the pulse width between 0 and 4096
    DRIVER15 PWM[0-8],pin,ON          // Fully turn a specific board/pin/LED ON
    DRIVER15 PWM[0-8],pin,OFF         // Fully turn a specific board/pin/LED OFF
    
    DRIVER15 PWMTO[0-8] tensecs,pin,value[,pin,value[,pin,value...]]  // Move all the specified pin to a new location in the specified time (1/10 sec resolution), if "tensecs" is zero it's equivalent to PWM command for all the pins
      
      e.g.  
        PWMTO 40 0 327 1 550 2 187 3 200
          this move the PIN0 of the Board 0 from the current position to 327
                    the PIN1 of the Board 0 from the current position to 550
                    the PIN2 of the Board 0 from the current position to 187
                    the PIN3 of the Board 0 from the current position to 200
          
          all the movements will be completed in 4 seconds, every PIN will be stepped relative to that @ 50ms step.

        PWMTO1 40 0 327 1 550 2 187 3 200
          same logic on the board #1

    DRIVER15 PWMSTOP[0-8]   // stop all the moment on the relative board
*/

#define XDRV_15 15
#define XI2C_01 1 // See I2CDEVICES.md

/*
  default prescale value from datasheet 7.3.5

    round(25000000/(4096*freq))-1;
*/
#ifndef USE_PCA9685_INT_CLOCK
#define USE_PCA9685_INT_CLOCK 250
#endif

#ifndef USE_PCA9685_ADDR
#define USE_PCA9685_ADDR 0x40
#endif

#ifndef USE_PCA9685_FREQ
#define USE_PCA9685_FREQ 50
#endif

#ifndef PCA9685_MAX_COUNT
#define PCA9685_MAX_COUNT 4
#endif

#define PCA9685_REG_MODE1 0x00
#define PCA9685_REG_LED0_ON_L 0x06
#define PCA9685_REG_PRE_SCALE 0xFE

typedef struct
{
  uint16_t pwm;
  bool running;
  uint16_t step;
  int16_t every;
  uint16_t target;
  int16_t direction; // 1 == UP , 0 == stop; -1 == down
} tMotor;

struct PCA9685
{
  uint8_t count;
  char name[10];
  bool inverted[PCA9685_MAX_COUNT];
  bool detected[PCA9685_MAX_COUNT];
  uint16_t intclk[PCA9685_MAX_COUNT];
  uint16_t freq[PCA9685_MAX_COUNT];
  tMotor motor[PCA9685_MAX_COUNT][16];
} pca9685;

#include <Ticker.h>

Ticker TickerPCA9685;

void PCA9685_SetName(uint8_t pca)
{
  if (pca9685.count > 1)
  {
    pca9685.name[7] = IndexSeparator();
    pca9685.name[8] = '0' + pca;
    pca9685.name[9] = 0;
  }
  else
  {
    pca9685.name[7] = 0;
  }
}

void PCA9685_Detect(void)
{
  memset(&pca9685, 0x0, sizeof(PCA9685));

  strcpy_P(pca9685.name, PSTR("PCA9685"));

  for (uint8_t dev = 0; dev < PCA9685_MAX_COUNT; dev++)
  {
    uint32_t addr = USE_PCA9685_ADDR + dev;

    if (!I2cSetDevice(addr))
    {
      continue;
    }

    pca9685.freq[dev] = USE_PCA9685_FREQ;
    pca9685.intclk[dev] = USE_PCA9685_INT_CLOCK;

    uint8_t buffer;
    if (I2cValidRead8(&buffer, addr, PCA9685_REG_MODE1))
    {
      I2cWrite8(addr, PCA9685_REG_MODE1, 0x20);
      if (I2cValidRead8(&buffer, addr, PCA9685_REG_MODE1))
      {
        if (0x20 == buffer)
        {
          // AddLog(LOG_LEVEL_DEBUG, "PCA9685[%02x] found", addr);
          pca9685.count++;
          pca9685.detected[dev] = true;
          I2cSetActiveFound(addr, PSTR("PCA9685"));
          PCA9685_Reset(dev); // Reset the controller
        }
      }
    }
  }

  if (pca9685.count > 0)
  {
    TickerPCA9685.attach_ms(50, PCA9685_RunMotor);
  }
}

void PCA9685_Reset(uint8_t pca)
{
  if (!pca9685.detected[pca])
  {
    return;
  }

  PCA9685_SetName(pca);

  I2cWrite8(USE_PCA9685_ADDR + pca, PCA9685_REG_MODE1, 0x80);
  PCA9685_SetPWMfreq(pca, USE_PCA9685_FREQ);
  pca9685.inverted[pca] = false;
  for (uint32_t pin = 0; pin < 16; pin++)
  {
    PCA9685_SetPWM(pca, pin, 0, pca9685.inverted[pca]);
    pca9685.motor[pca][pin].pwm = PCA9685_GetPWMvalue(0, pca9685.inverted[pca]);
  }
  Response_P(PSTR("{\"%s\":"), pca9685.name);
  ResponseAppend_P(S_JSON_COMMAND_SVALUE, D_CMND_RESET, PSTR("OK"));
  ResponseJsonEnd();
}

uint16_t PCA9685_GetPWMvalue(uint16_t pwm, bool inverted)
{
  uint16_t pwm_val = pwm;
  if (inverted)
  {
    pwm_val = 4096 - pwm;
  }
  return pwm_val;
}

void PCA9685_SetPWMfreq(uint8_t pca, double freq)
{
  if (freq > 23 && freq < 1527)
  {
    pca9685.freq[pca] = freq;
  }
  else
  {
    pca9685.freq[pca] = 50;
  }
  uint8_t pre_scale_osc = round((pca9685.intclk[pca] * 100000) / (4096 * pca9685.freq[pca])) - 1;
  if (1526 == pca9685.freq[pca])
    pre_scale_osc = 0xFF;                                                      // force setting for 24hz because rounding causes 1526 to be 254
  uint8_t current_mode1 = I2cRead8(USE_PCA9685_ADDR + pca, PCA9685_REG_MODE1); // read current value of MODE1 register
  uint8_t sleep_mode1 = (current_mode1 & 0x7F) | 0x10;                         // Determine register value to put PCA to sleep
  I2cWrite8(USE_PCA9685_ADDR + pca, PCA9685_REG_MODE1, sleep_mode1);           // Let's sleep a little
  I2cWrite8(USE_PCA9685_ADDR + pca, PCA9685_REG_PRE_SCALE, pre_scale_osc);     // Set the pre-scaler
  I2cWrite8(USE_PCA9685_ADDR + pca, PCA9685_REG_MODE1, current_mode1 | 0xA0);  // Reset MODE1 register to original state and enable auto increment
}

void PCA9685_SetPWM_Reg(uint8_t pca, uint8_t pin, uint16_t on, uint16_t off)
{
  uint8_t led_reg = PCA9685_REG_LED0_ON_L + 4 * pin;
  uint32_t led_data = 0;
  I2cWrite8(USE_PCA9685_ADDR + pca, led_reg, on);
  I2cWrite8(USE_PCA9685_ADDR + pca, led_reg + 1, (on >> 8));
  I2cWrite8(USE_PCA9685_ADDR + pca, led_reg + 2, off);
  I2cWrite8(USE_PCA9685_ADDR + pca, led_reg + 3, (off >> 8));
}

void PCA9685_SetPWM(uint8_t pca, uint8_t pin, uint16_t pwm, bool inverted)
{
  uint16_t pwm_val = PCA9685_GetPWMvalue(pwm, inverted);
  if (4096 == pwm_val)
  {
    PCA9685_SetPWM_Reg(pca, pin, 4096, 0); // Special use additional bit causes channel to turn on completely without PWM
  }
  else
  {
    PCA9685_SetPWM_Reg(pca, pin, 0, pwm_val);
  }
  pca9685.motor[pca][pin].pwm = pwm_val;
}

void PCA9685_RunMotor()
{
  for (uint8_t dev = 0; dev < PCA9685_MAX_COUNT; dev++)
  {
    if (!pca9685.detected[dev])
      continue;

    for (uint8_t pin = 0; pin < 15; pin++)
    {
      tMotor *m = &(pca9685.motor[dev][pin]);

      if (!m->running)
        continue;

      if (m->every == -1 || (m->direction > 0 && m->pwm >= m->target) || (m->direction < 0 && m->pwm <= m->target))
      {
        m->running = false;
        if (m->pwm != m->target)
        {
          PCA9685_SetPWM(dev, pin, m->target, pca9685.inverted[dev]);
        }
        continue;
      }

      if (m->step == 0 || (m->step % m->every == 0))
      {
        // AddLog(LOG_LEVEL_DEBUG, "PCA9685: MOTOR dev=%u pin=%u s=%u e=%u pwm=%lu target=%lu dir=%d",
        //        dev,
        //        pin,
        //        m->step,
        //        m->every,
        //        m->pwm,
        //        m->target,
        //        m->direction);

        PCA9685_SetPWM(dev, pin, m->pwm + m->direction, pca9685.inverted[dev]);
      }

      m->step++;
    }
  }
}

void PCA9685_getCmdSuffix(char *command, uint8_t *suffixNumber)
{
  size_t commandLength = strlen(command);
  uint8_t result = 0;
  *suffixNumber = 0;

  if (isdigit(command[commandLength - 1]))
  {
    result = command[commandLength - 1] - '0';
    if (result >= 0 && result <= 7)
    {
      *suffixNumber = result;
      command[commandLength - 1] = '\0';
    }
  }
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
  char argument[XdrvMailbox.data_len];
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

  char command[CMDSZ] = {0};
  char pcaName[10];
  uint8_t dev;

  PCA9685_getCmdSuffix(ArgV(command, 1), &dev);

  if (!strcmp(command, "RESET"))
  {
    PCA9685_Reset(dev);
    return serviced;
  }

  if (!strcmp(command, "STATUS"))
  {
    PCA9685_OutputTelemetry(false);
    return serviced;
  }

  PCA9685_SetName(dev);

  if (!strcmp(command, "INVERT"))
  {
    if (paramcount > 1)
    {
      pca9685.inverted[dev] = (1 == atoi(ArgV(argument, 2)));
      Response_P(PSTR("{\"%s\":{\"INVERT\":%i, \"Result\":\"OK\"}}"), pca9685.name, pca9685.inverted[dev] ? 1 : 0);
      return serviced;
    }
    else
    { // No parameter was given for invert, so we return current setting
      Response_P(PSTR("{\"%s\":{\"INVERT\":%i}}"), pca9685.name, pca9685.inverted[dev] ? 1 : 0);
      return serviced;
    }
  }

  if (!strcmp(command, "INTCLK"))
  {
    if (paramcount > 1)
    {
      pca9685.intclk[dev] = atoi(ArgV(argument, 2));
      Response_P(PSTR("{\"%s\":{\"INTCLK\":%lu, \"Result\":\"OK\"}}"), pca9685.name, pca9685.intclk[dev]);
      return serviced;
    }
    else
    {
      Response_P(PSTR("{\"%s\":{\"INTCLK\":%lu}}"), pca9685.name, pca9685.intclk[dev]);
      return serviced;
    }
  }

  /*
    PWMTO timeinsec,pin,target[[,pin,target]...]
  */
  if (!strcmp(command, "PWMTO"))
  {
    uint8_t paramFrom = 1;
    while (true)
    {
      if (paramcount > (2 + paramFrom))
      {
        uint16_t tids = atoi(ArgV(argument, 2)); // time in 1/10 of second to complete all the motors move

        uint16_t pin = atoi(ArgV(argument, 2 + paramFrom));
        /*
            Sanity check - To be refactored
        */
        if (pin > 15)
          pin = 15;
        if (tids < 2)
          tids = 0; // min 2/10 seconds to complete all the moves
        if (tids > 600)
          tids = 600; // max 60 seconds to complete all the moves

        tMotor *m = &pca9685.motor[dev][pin];
        m->target = atoi(ArgV(argument, 2 + paramFrom + 1));
        if (m->target != m->pwm)
        {
          m->step = 0;
          m->direction = m->target < m->pwm ? -1 : 1;

          if( tids == 0 )
          {
            m->every = -1;
            m->running = true;
          } else {
            // AddLog(LOG_LEVEL_DEBUG, "PCA9685: PWMTO dev=%u pin=%u tids=%u e=? pwm=%lu target=%lu dir=%d",
            //        dev,
            //        pin,
            //        tids,
            //        m->pwm,
            //        m->target,
            //        m->direction);

            m->every = 0;
            while (m->every < 1)
            {
              uint16_t stepValue = abs((int16_t)m->pwm - (int16_t)m->target) / abs(m->direction);
              if (stepValue < 1)
              {
                m->direction += m->target < m->pwm ? -1 : 1;
                continue;
              }

              m->every = round((tids * 200) / stepValue);
              if (m->every < 1)
              {
                m->direction += m->target < m->pwm ? -1 : 1;
                continue;
              }
            }

            m->running = true;
          }
        }
        else
        {
          m->running = false;
        }

        paramFrom += 2;
      }
      else
      {
        break;
      }
    }

    Response_P(PSTR("{\"%s\":{\"PWMTO\":\"OK\"}}"), pca9685.name);
    return serviced;
  }

  if (!strcmp(command, "PWMSTOP"))
  {
    if (pca9685.detected[dev])
    {
      for (uint8_t pin = 0; pin < 15; pin++)
      {
        pca9685.motor[dev][pin].running = false;
      }

      Response_P(PSTR("{\"%s\":{\"PWMSTOP\":\"OK\"}}"), pca9685.name);
      return serviced;
    }
  }

  if (!strcmp(command, "PWMF"))
  {
    if (paramcount > 1)
    {
      uint16_t new_freq = atoi(ArgV(argument, 2));
      if ((new_freq >= 24) && (new_freq <= 1526))
      {
        PCA9685_SetPWMfreq(dev, new_freq);
        Response_P(PSTR("{\"%s\":{\"PWMF\":%i, \"Result\":\"OK\"}}"), pca9685.name, new_freq);
        return serviced;
      }
    }
    else
    { // No parameter was given for setfreq, so we return current setting
      Response_P(PSTR("{\"%s\":{\"PWMF\":%i}}"), pca9685.name, pca9685.freq[dev]);
      return serviced;
    }
  }

  if (!strcmp(command, "PWM"))
  {
    if (paramcount > 1)
    {
      uint8_t pin = atoi(ArgV(argument, 2));
      if (paramcount > 2)
      {
        // force motor stop
        pca9685.motor[dev][pin].running = false;

        if (!strcmp(ArgV(argument, 3), "ON"))
        {
          PCA9685_SetPWM(dev, pin, 4096, pca9685.inverted[dev]);
          Response_P(PSTR("{\"%s\":{\"PIN\":%i,\"PWM\":%i}}"), pca9685.name, pin, 4096);
          serviced = true;
          return serviced;
        }
        if (!strcmp(ArgV(argument, 3), "OFF"))
        {
          PCA9685_SetPWM(dev, pin, 0, pca9685.inverted[dev]);
          Response_P(PSTR("{\"%s\":{\"PIN\":%i,\"PWM\":%i}}"), pca9685.name, pin, 0);
          serviced = true;
          return serviced;
        }
        uint16_t pwm = atoi(ArgV(argument, 3));
        if ((pin >= 0 && pin <= 15 || pin == 61) && (pwm >= 0 && pwm <= 4096))
        {
          PCA9685_SetPWM(dev, pin, pwm, pca9685.inverted[dev]);
          Response_P(PSTR("{\"%s\":{\"PIN\":%i,\"PWM\":%i}}"), pca9685.name, pin, pwm);
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
  ResponseTime_P(PSTR(""));

  for (uint8_t dev = 0; dev < PCA9685_MAX_COUNT; dev++)
  {
    if (!pca9685.detected[dev])
    {
      continue;
    }

    PCA9685_SetName(dev);

    ResponseAppend_P(PSTR(",\"%s\":{\"PWM_FREQ\":%i"), pca9685.name, pca9685.freq[dev]);
    ResponseAppend_P(PSTR(",\"INVERT\":%i"), pca9685.inverted[dev] ? 1 : 0);
    ResponseAppend_P(PSTR(",\"INTCLK\":%lu"), pca9685.intclk[dev]);

    for (uint32_t pin = 0; pin < 16; pin++)
    {
      uint16_t pwm_val = PCA9685_GetPWMvalue(pca9685.motor[dev][pin].pwm, pca9685.inverted[dev]); // return logical (possibly inverted) pwm value
      ResponseAppend_P(PSTR(",\"PWM%i\":%i"), pin, pwm_val);
    }

    ResponseJsonEnd();
  }

  ResponseJsonEnd();

  if (telemetry)
  {
    MqttPublishTeleSensor();
  }
}

bool Xdrv15(uint32_t function)
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
  else if (pca9685.count > 0)
  {
    switch (function)
    {
    case FUNC_EVERY_SECOND:
      if (TasmotaGlobal.tele_period == 0)
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

#endif // USE_PCA9685_V2
#endif // USE_IC2