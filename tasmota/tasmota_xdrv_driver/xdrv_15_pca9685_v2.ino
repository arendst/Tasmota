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
    DRIVER15 RESET[0-7]               // Reset to power-up settings - i.e. F=50hz and all pins in OFF state for a specific board
    DRIVER15 INVERT[0-7],pin[,0-1]    // print or set the inversion bit on pin of the specific board
    DRIVER15 INTCLK[0-7],clock        // where clock is the Interal Clock value in 1/10 MHz (default USE_PCA9685_INT_CLOCK = 250)
    DRIVER15 PMWF[0-7],frequency      // where frequency is the PWM frequency from 24 to 1526 in Hz
    DRIVER15 PWM[0-7],pin,pwmvalue    // where pin=LED, pin 0 through 15 and pwmvalue is the pulse width between 0 and 4096
    DRIVER15 PWM[0-7],pin,ON          // Fully turn a specific board/pin/LED ON
    DRIVER15 PWM[0-7],pin,OFF         // Fully turn a specific board/pin/LED OFF
    
    DRIVER15 PWMTO[0-7] tensecs,pin,value[,pin,value[,pin,value...]]  // Move all the specified pin to a new location in the specified time (1/10 sec resolution), if "tensecs" is zero it's equivalent to PWM command for all the pins
      
      e.g.  
        PWMTO 40 0 327 1 550 2 187 3 200
          this move the PIN0 of the Board 0 from the current position to 327
                    the PIN1 of the Board 0 from the current position to 550
                    the PIN2 of the Board 0 from the current position to 187
                    the PIN3 of the Board 0 from the current position to 200
          
          all the movements will be completed in 4 seconds, every PIN will be stepped relative to that @ 50ms step.

        PWMTO1 40 0 327 1 550 2 187 3 200
          same logic on the board #1

    DRIVER15 PWMSTOP[0-7]   // stop all the moment on the relative board
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

#include <Ticker.h>

typedef struct {
  uint16_t pwm;
  bool running;
  uint16_t step;
  int16_t every;
  uint16_t target;
  int16_t direction; // 1 == UP , 0 == stop; -1 == down
} tPca9685Motor;

typedef struct {
  tPca9685Motor motor[16];
  uint16_t intclk;
  uint16_t freq;
  uint8_t addr;
  uint8_t bus;
  bool inverted;
} tPca9685;

struct PCA9685 {
  tPca9685 *device[PCA9685_MAX_COUNT];
  Ticker ticker;
  uint8_t count;
} pca9685;

/********************************************************************************************/

bool PCA9685_I2cValidRead8(uint32_t pca, uint8_t *data, uint8_t reg) {
  return I2cValidRead8(data, pca9685.device[pca]->addr, reg, pca9685.device[pca]->bus);
}

uint8_t PCA9685_I2cRead8(uint32_t pca, uint8_t reg) {
  return I2cRead8(pca9685.device[pca]->addr, reg, pca9685.device[pca]->bus);
}

bool PCA9685_I2cWrite8(uint32_t pca, uint8_t reg, uint32_t val) {
  return I2cWrite8(pca9685.device[pca]->addr, reg, val, pca9685.device[pca]->bus);
}

/********************************************************************************************/

void PCA9685_SetName(char *name, uint8_t pca) {
  strcpy_P(name, PSTR("PCA9685"));
  if (pca9685.count > 1) {
    snprintf_P(name, 16, PSTR("%s%c%d"), name, IndexSeparator(), pca +1);
  }
}

void PCA9685_Detect(void) {
  for (uint32_t bus = 0; bus < MAX_I2C; bus++) {
    uint32_t address = USE_PCA9685_ADDR;
    while ((pca9685.count < PCA9685_MAX_COUNT) && (address < USE_PCA9685_ADDR + PCA9685_MAX_COUNT)) {
      if (I2cSetDevice(address, bus)) {
        uint8_t buffer;
        if (I2cValidRead8(&buffer, address, PCA9685_REG_MODE1, bus)) {
          I2cWrite8(address, PCA9685_REG_MODE1, 0x20, bus);
          if (I2cValidRead8(&buffer, address, PCA9685_REG_MODE1, bus)) {
            if (0x20 == buffer) {
              pca9685.device[pca9685.count] = (tPca9685*)calloc(sizeof(tPca9685), 1);  // Need calloc to reset registers to 0/false
              if (nullptr == pca9685.device[pca9685.count]) { 
                pca9685.count = 0;
                return;
              }
              pca9685.device[pca9685.count]->addr = address;
              pca9685.device[pca9685.count]->bus = bus;
              pca9685.device[pca9685.count]->freq = USE_PCA9685_FREQ;
              pca9685.device[pca9685.count]->intclk = USE_PCA9685_INT_CLOCK;
              pca9685.count++;

              PCA9685_Reset(pca9685.count -1); // Reset the controller
              I2cSetActiveFound(address, PSTR("PCA9685"), bus);
            }
          }
        }

      }
      address++;
    }
  }
  if (pca9685.count > 0) {
    pca9685.ticker.attach_ms(50, PCA9685_RunMotor);
  }
}

void PCA9685_Reset(uint8_t pca) {
  if (pca >= pca9685.count) { return; }

  PCA9685_I2cWrite8(pca, PCA9685_REG_MODE1, 0x80);  // Disable ALLCALL (uses I2C address 0x70)
  PCA9685_SetPWMfreq(pca, USE_PCA9685_FREQ);
  pca9685.device[pca]->inverted = false;
  for (uint32_t pin = 0; pin < 16; pin++) {
    PCA9685_SetPWM(pca, pin, 0, false);
    pca9685.device[pca]->motor[pin].pwm = PCA9685_GetPWMvalue(0, false);
  }

  char name[16];
  PCA9685_SetName(name, pca);
  Response_P(PSTR("{\"%s\":"), name);
  ResponseAppend_P(S_JSON_COMMAND_SVALUE, D_CMND_RESET, PSTR("OK"));
  ResponseJsonEnd();
}

uint16_t PCA9685_GetPWMvalue(uint16_t pwm, bool inverted) {
  uint16_t pwm_val = pwm;
  if (inverted) {
    pwm_val = 4096 - pwm;
  }
  return pwm_val;
}

void PCA9685_SetPWMfreq(uint8_t pca, double freq) {
  if (freq > 23 && freq < 1527) {
    pca9685.device[pca]->freq = freq;
  } else {
    pca9685.device[pca]->freq = 50;
  }
  uint8_t pre_scale_osc = round((pca9685.device[pca]->intclk * 100000) / (4096 * pca9685.device[pca]->freq)) - 1;
  if (1526 == pca9685.device[pca]->freq) {
    pre_scale_osc = 0xFF;                                           // force setting for 24hz because rounding causes 1526 to be 254
  }
  uint8_t current_mode1 = PCA9685_I2cRead8(pca, PCA9685_REG_MODE1); // read current value of MODE1 register
  uint8_t sleep_mode1 = (current_mode1 & 0x7F) | 0x10;              // Determine register value to put PCA to sleep
  PCA9685_I2cWrite8(pca, PCA9685_REG_MODE1, sleep_mode1);           // Let's sleep a little
  PCA9685_I2cWrite8(pca, PCA9685_REG_PRE_SCALE, pre_scale_osc);     // Set the pre-scaler
  PCA9685_I2cWrite8(pca, PCA9685_REG_MODE1, current_mode1 | 0xA0);  // Reset MODE1 register to original state and enable auto increment
}

void PCA9685_SetPWM_Reg(uint8_t pca, uint8_t pin, uint16_t on, uint16_t off) {
  uint8_t led_reg = PCA9685_REG_LED0_ON_L + 4 * pin;
  uint32_t led_data = 0;
  PCA9685_I2cWrite8(pca, led_reg, on);
  PCA9685_I2cWrite8(pca, led_reg + 1, (on >> 8));
  PCA9685_I2cWrite8(pca, led_reg + 2, off);
  PCA9685_I2cWrite8(pca, led_reg + 3, (off >> 8));
}

void PCA9685_SetPWM(uint8_t pca, uint8_t pin, uint16_t pwm, bool inverted) {
  uint16_t pwm_val = PCA9685_GetPWMvalue(pwm, inverted);
  if (4096 == pwm_val) {
    PCA9685_SetPWM_Reg(pca, pin, 4096, 0); // Special use additional bit causes channel to turn on completely without PWM
  } else {
    PCA9685_SetPWM_Reg(pca, pin, 0, pwm_val);
  }
  pca9685.device[pca]->motor[pin].pwm = pwm_val;
}

void PCA9685_RunMotor(void) {
  for (uint8_t dev = 0; dev < pca9685.count; dev++) {
    for (uint8_t pin = 0; pin < 15; pin++) {
      tPca9685Motor *m = &(pca9685.device[dev]->motor[pin]);
      if (!m->running) { continue; }

      if (m->every == -1 || (m->direction > 0 && m->pwm >= m->target) || (m->direction < 0 && m->pwm <= m->target)) {
        m->running = false;
        if (m->pwm != m->target) {
          PCA9685_SetPWM(dev, pin, m->target, pca9685.device[dev]->inverted);
        }
        continue;
      }

      if (m->step == 0 || (m->step % m->every == 0)) {
        // --- SECURITY-FIX START ---
        
        // We temporarily use "int32_t" (signed and large enough)
        // so that we don't run into overflows.
        int32_t next_val = (int32_t)m->pwm + (int32_t)m->direction;

        // we dim UP (direction > 0)
        if (m->direction > 0) {
            // // If we overshoot the target -> clamp to the target
            if (next_val > m->target) next_val = m->target;
            // Absolute hardware upper limit (4096 is the “fully on” bit)
            if (next_val > 4096) next_val = 4096; 
        }

        // we dim DOWN (direction < 0)
        if (m->direction < 0) {
            // If we were to fall below the target -> clamp to the target
            if (next_val < m->target) next_val = m->target;
            // Absolute lower limit
            if (next_val < 0) next_val = 0;
        }

        // Now write the safe, verified value
        PCA9685_SetPWM(dev, pin, (uint16_t)next_val, pca9685.device[dev]->inverted);
        
        // --- SECURITY-FIX END ---
      }
      m->step++;
    }
  }
}

void PCA9685_getCmdSuffix(char *command, uint8_t *suffixNumber) {
  size_t commandLength = strlen(command);
  uint8_t result = 0;
  *suffixNumber = 0;

  if (isdigit(command[commandLength - 1])) {
    result = command[commandLength - 1] - '0';
    if (result >= 0 && result <= 7) {
      *suffixNumber = result;
      command[commandLength - 1] = '\0';
    }
  }
}

bool PCA9685_Command(void) {
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
    if ((' ' == XdrvMailbox.data[ca]) || ('=' == XdrvMailbox.data[ca])) {
      XdrvMailbox.data[ca] = ',';
    }
    if (',' == XdrvMailbox.data[ca]) {
      paramcount++;
    }
  }
  UpperCase(XdrvMailbox.data, XdrvMailbox.data);

  char command[CMDSZ] = {0};
  uint8_t dev;
  PCA9685_getCmdSuffix(ArgV(command, 1), &dev);
  char name[16];
  PCA9685_SetName(name, dev);

  if (!strcmp(command, "RESET")) {
    // Driver15 reset[0..7]
    PCA9685_Reset(dev);
    return serviced;
  }

  if (!strcmp(command, "STATUS")) {
    // Driver15 status
    ResponseTime_P(PSTR(""));
    PCA9685_Show(1);
    ResponseJsonEnd();
    return serviced;
  }

  if (!strcmp(command, "INVERT")) {
    // Driver15 invert[0..7]   - Show current state
    // Driver15 invert[0..7] 0 - Invert OFF
    // Driver15 invert[0..7] 1 - Invert ON
    if (paramcount > 1) {
      pca9685.device[dev]->inverted = (1 == atoi(ArgV(argument, 2)));
    }
    Response_P(PSTR("{\"%s\":{\"INVERT\":%i"), name, pca9685.device[dev]->inverted ? 1 : 0);
    if (paramcount > 1) {
      ResponseAppend_P(PSTR(",\"Result\":\"OK\""));
    }
    ResponseJsonEndEnd();
    return serviced;
  }

  if (!strcmp(command, "INTCLK")) {
    // Driver15 intclk[0..7]     - Show current state
    // Driver15 intclk[0..7] 250 - Set internal clock
    if (paramcount > 1) {
      pca9685.device[dev]->intclk = atoi(ArgV(argument, 2));
    }
    Response_P(PSTR("{\"%s\":{\"INTCLK\":%lu"), name, pca9685.device[dev]->intclk);
    if (paramcount > 1) {
      ResponseAppend_P(PSTR(",\"Result\":\"OK\""));
    }
    ResponseJsonEndEnd();
    return serviced;
  }

  /*
    PWMTO timeinsec,pin,target[[,pin,target]...]
  */
  if (!strcmp(command, "PWMTO")) {
    uint8_t paramFrom = 1;
    while (true) {
      if (paramcount > (2 + paramFrom)) {
        uint16_t tids = atoi(ArgV(argument, 2)); // time in 1/10 of second to complete all the motors move

        uint16_t pin = atoi(ArgV(argument, 2 + paramFrom));
        /*
            Sanity check - To be refactored
        */
        if (pin > 15) { pin = 15; }
        if (tids < 2) { tids = 0; }      // min 2/10 seconds to complete all the moves
        if (tids > 600) { tids = 600; }  // max 60 seconds to complete all the moves

        tPca9685Motor *m = &pca9685.device[dev]->motor[pin];
        m->target = atoi(ArgV(argument, 2 + paramFrom + 1));
        if (m->target != m->pwm) {
          m->step = 0;
          m->direction = m->target < m->pwm ? -1 : 1;

          if( tids == 0 ) {
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

            // calculate total difference (e.g. 4095 steps)
            uint16_t totalSteps = abs((int16_t)m->pwm - (int16_t)m->target);
            
            // calculate how many 50ms-ticks we have at all
            // tids is in 0.1s. so tids * 2 is the amount of 50ms ticks.
            uint16_t totalTicks = tids * 2; 

            if (totalTicks < 1) { totalTicks = 1; }  // lowest possible

            // calculate steps per tick
            uint16_t stepsPerTick = totalSteps / totalTicks;

            if (stepsPerTick < 1) {
                // Slow fade (we have morew time than steps)
                m->direction = m->target < m->pwm ? -1 : 1;
                m->every = totalTicks / totalSteps;
            } else {
                // Fast Fade (LED Mode: We need to make big steps)
                m->direction = (m->target < m->pwm ? -1 : 1) * stepsPerTick;
                m->every = 1; // Fire every tick!
            }
            
            m->running = true;
          }
        } else {
          m->running = false;
        }

        paramFrom += 2;
      } else {
        break;
      }
    }

    Response_P(PSTR("{\"%s\":{\"PWMTO\":\"OK\"}}"), name);
    return serviced;
  }

  if (!strcmp(command, "PWMSTOP")) {
    for (uint8_t pin = 0; pin < 15; pin++) {
      pca9685.device[dev]->motor[pin].running = false;
    }
    Response_P(PSTR("{\"%s\":{\"PWMSTOP\":\"OK\"}}"), name);
    return serviced;
  }

  if (!strcmp(command, "PWMF")) {
    // Driver15 pwmf[0..7]    - Show current frequency
    // Driver15 pwmf[0..7] 50 - Frequency set to 50Hz
    bool valid = false;
    if (paramcount > 1) {
      uint16_t new_freq = atoi(ArgV(argument, 2));
      if ((new_freq >= 24) && (new_freq <= 1526)) {
        PCA9685_SetPWMfreq(dev, new_freq);
        valid = true;
      }
    }
    Response_P(PSTR("{\"%s\":{\"PWMF\":%i"), name, pca9685.device[dev]->freq);
    if (valid) {
      ResponseAppend_P(PSTR(",\"Result\":\"OK\""));
    }
    ResponseJsonEndEnd();
    return serviced;
  }

  if (!strcmp(command, "PWM")) {
    // Driver15 pwm[0..7] 0..15 ON
    // Driver15 pwm[0..7] 0..15 OFF
    // Driver15 pwm[0..7] 0..15 0..4096
    if (paramcount > 1) {
      uint8_t pin = atoi(ArgV(argument, 2));
      if (paramcount > 2) {
        // force motor stop
        pca9685.device[dev]->motor[pin].running = false;
        uint16_t pwm = atoi(ArgV(argument, 3));
        if (!strcmp(ArgV(argument, 3), "ON")) {
          pwm = 4096;
        }
        else if (!strcmp(ArgV(argument, 3), "OFF")) {
          pwm = 0;
        }
        if ((pin >= 0 && pin <= 15 || pin == 61) && (pwm >= 0 && pwm <= 4096)) {
          PCA9685_SetPWM(dev, pin, pwm, pca9685.device[dev]->inverted);
          Response_P(PSTR("{\"%s\":{\"PIN\":%i,\"PWM\":%i}}"), name, pin, pwm);
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
  char name[16];
  for (uint8_t dev = 0; dev < pca9685.count; dev++) {
    PCA9685_SetName(name, dev);

    ResponseAppend_P(PSTR(",\"%s\":{\"PWM_FREQ\":%i"), name, pca9685.device[dev]->freq);
    ResponseAppend_P(PSTR(",\"INVERT\":%i"), pca9685.device[dev]->inverted ? 1 : 0);
    ResponseAppend_P(PSTR(",\"INTCLK\":%lu"), pca9685.device[dev]->intclk);
    for (uint32_t pin = 0; pin < 16; pin++) {
      uint16_t pwm_val = PCA9685_GetPWMvalue(pca9685.device[dev]->motor[pin].pwm, pca9685.device[dev]->inverted); // return logical (possibly inverted) pwm value
      ResponseAppend_P(PSTR(",\"PWM%i\":%i"), pin, pwm_val);
    }
    ResponseJsonEnd();
  }
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
  else if (pca9685.count) {
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

#endif // USE_PCA9685_V2
#endif // USE_IC2
