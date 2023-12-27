/*
  xdrv_34_wemos_motor_v1.ino - Support for I2C WEMOS motor shield (6612FNG)

  Copyright (C) 2021  Denis Sborets, Theo Arends, Peter Franck

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
#ifdef USE_WEMOS_MOTOR_V1
/*********************************************************************************************\
 * WEMOS_MOTOR_V1 - DC motor driver shield (6612FNG) v 1.0.0
 * WEMOS_MOTOR_V2 - DC motor driver shield (6612FNG) v 2.0.0 (#define WEMOS_MOTOR_V2)
 *
 * I2C Address: 0x30
 *
 * Command format:
 * driver44 <command>,<motor>,<direction>{,<duty>}
 * command:
 *    RESET - reset a motor shield
 *    SETMOTOR - set motor state
 * motor:
 *    0 - motor A
 *    1 - motor B
 *    2 - both motors (V2 only)
 * direction:
 *    0 - short break
 *    1 - CCW
 *    2 - CW
 *    3 - stop
 *    4 - standby
 * duty (optional):
 *    0 - 100% (100% by default)
\*********************************************************************************************/

#define XDRV_34              34
#define XI2C_44              44          // See I2CDEVICES.md

#ifndef WEMOS_MOTOR_V1_ADDR
#define WEMOS_MOTOR_V1_ADDR  0x30        // Default I2C address 0x30
#endif
#ifndef WEMOS_MOTOR_V1_FREQ
#define WEMOS_MOTOR_V1_FREQ  1000        // Default frequency
#endif

#define MOTOR_A 0
#define MOTOR_B 1

#define SHORT_BRAKE 0
#define CCW 1
#define CW 2
#define STOP 3
#define STANDBY 4

#ifdef WEMOS_MOTOR_V2                    // Support latest Lolin board
// #define DEBUG_WEMOS_MOTOR             // be more verbose
#define WEMOS_MOTOR_PID_V2   0x02        // Product ID of V2.0.0
#define WMTR_V2_BUFFSZ       5           // I2C command buffer size
enum WEMOS_MOTOR_V2_CMD
{
  WV2_GET_SLAVE_STATUS = 0x01,
  WV2_RESET_SLAVE,
  WV2_CHANGE_I2C_ADDRESS,
  WV2_CHANGE_STATUS,
  WV2_CHANGE_FREQ,
  WV2_CHANGE_DUTY
};
enum WEMOS_MOTOR_V2_STATUS
{
  WV2_MOTOR_STATUS_STOP = 0x00,
  WV2_MOTOR_STATUS_CCW,
  WV2_MOTOR_STATUS_CW,
  WV2_MOTOR_STATUS_SHORT_BRAKE,
  WV2_MOTOR_STATUS_STANDBY
};
enum WEMOS_MOTOR_V2_CHANNEL
{
  WV2_MOTOR_CH_A=0x00,
  WV2_MOTOR_CH_B,
  WV2_MOTOR_CH_BOTH
};
const char WemosMotorDriver[] = "WEMOS_MOTOR_V2";
#else  // WEMOS_MOTOR_V2
const char WemosMotorDriver[] = "WEMOS_MOTOR_V1";
#endif // WEMOS_MOTOR_V2

struct WMOTORV1 {
  bool detected = false;
  uint8_t motor;
} WMotorV1;

void WMotorV1Detect(void)
{
  if (I2cSetDevice(WEMOS_MOTOR_V1_ADDR)) {
#ifdef WEMOS_MOTOR_V2
    uint8_t i2c_data[WMTR_V2_BUFFSZ];
    // Check product ID & version
    i2c_data[0] = WV2_GET_SLAVE_STATUS;
    WMotorV2command(i2c_data, 1);
    if (i2c_data[0] == WEMOS_MOTOR_PID_V2) {
      #ifdef DEBUG_WEMOS_MOTOR
      AddLog(LOG_LEVEL_INFO, PSTR("WEM: %s Rev. %u found"), WemosMotorDriver, i2c_data[1]);
      #endif // DEBUG_WEMOS_MOTOR
    } else { return; }
#endif // WEMOS_MOTOR_V2
    WMotorV1.detected = true;
    I2cSetActiveFound(WEMOS_MOTOR_V1_ADDR, WemosMotorDriver);
    WMotorV1Reset();
  }
}

void WMotorV1Reset(void)
{
  WMotorV1SetFrequency(WEMOS_MOTOR_V1_FREQ);
}

void WMotorV1SetFrequency(uint32_t freq)
{
#ifdef WEMOS_MOTOR_V2
  uint8_t i2c_data[WMTR_V2_BUFFSZ];
  i2c_data[0] = WV2_CHANGE_FREQ;
  i2c_data[1] = WV2_MOTOR_CH_BOTH;
  i2c_data[2] = (uint8_t)(freq & 0xff);
  i2c_data[3] = (uint8_t)((freq >> 8) & 0xff);
  i2c_data[4] = (uint8_t)((freq >> 16) & 0xff);
  WMotorV2command(i2c_data, 5);
#else  // WEMOS_MOTOR_V2
  Wire.beginTransmission(WEMOS_MOTOR_V1_ADDR);
  Wire.write(((byte)(freq >> 16)) & (byte)0x0f);
  Wire.write((byte)(freq >> 16));
  Wire.write((byte)(freq >> 8));
  Wire.write((byte)freq);
  Wire.endTransmission();
#endif // WEMOS_MOTOR_V2
}

void WMotorV1SetMotor(uint8_t motor, uint8_t dir, float pwm_val)
{
#ifdef WEMOS_MOTOR_V2
  uint8_t i2c_data[WMTR_V2_BUFFSZ];
  // send command
  uint8_t cmd = dir;
  if (cmd == 0 || cmd == 3) {
    cmd ^= 3;                   // short brake and stop swapped
  }
  i2c_data[0] = WV2_CHANGE_STATUS;
  i2c_data[1] = (uint8_t)motor;
  i2c_data[2] = (uint8_t)cmd;
  WMotorV2command(i2c_data, 3);
  #ifdef DEBUG_WEMOS_MOTOR
  AddLog(LOG_LEVEL_INFO, PSTR("WEM: Ch: %u Cmd: %u"), motor, cmd);
  #endif // DEBUG_WEMOS_MOTOR
  // set duty cycle
  uint16_t duty = (pwm_val > 100.0) ? 10000 : (uint16_t)(pwm_val * 100);
  #ifdef DEBUG_WEMOS_MOTOR
  AddLog(LOG_LEVEL_INFO, PSTR("WEM: Duty: %u"), duty);
  #endif // DEBUG_WEMOS_MOTOR
  i2c_data[0] = WV2_CHANGE_DUTY;
  i2c_data[1] = (uint8_t)motor;
  i2c_data[2] = (uint8_t)(duty & 0xff);
  i2c_data[3] = (uint8_t)((duty >> 8) & 0xff);
  WMotorV2command(i2c_data,4);
#else  // WEMOS_MOTOR_V2
  Wire.beginTransmission(WEMOS_MOTOR_V1_ADDR);
  Wire.write(motor | (byte)0x10);
  Wire.write(dir);

  uint16_t _pwm_val = uint16_t(pwm_val * 100);
  if (_pwm_val > 10000) {
    _pwm_val = 10000;
  }

  Wire.write((byte)(_pwm_val >> 8));
  Wire.write((byte)_pwm_val);
  Wire.endTransmission();
#endif // WEMOS_MOTOR_V2
}

bool WMotorV1Command(void)
{
  uint8_t args_count = 0;

  if (XdrvMailbox.data_len > 0) {
    args_count = 1;
  } else {
    return false;
  }

  for (uint32_t idx = 0; idx < XdrvMailbox.data_len; idx++) {
    if (' ' == XdrvMailbox.data[idx]) {
      XdrvMailbox.data[idx] = ',';
    }
    if (',' == XdrvMailbox.data[idx]) {
      args_count++;
    }
  }
  UpperCase(XdrvMailbox.data, XdrvMailbox.data);

  char *command = strtok(XdrvMailbox.data, ",");

  if (strcmp(command, "RESET") == 0) {
#ifdef WEMOS_MOTOR_V2       // do a 'real' reset
    uint8_t i2c_data[WMTR_V2_BUFFSZ];
    i2c_data[0] = WV2_RESET_SLAVE;
    WMotorV2command(i2c_data, 1);
#endif // WEMOS_MOTOR_V2
    WMotorV1Reset();
    Response_P(PSTR("{\"%s\":{\"RESET\":\"OK\"}}"), WemosMotorDriver);
    return true;
  }

  if (strcmp(command, "SETMOTOR") == 0) {
    if (args_count >= 3) {

      int motor = atoi(strtok(NULL, ","));
      int dir = atoi(strtok(NULL, ","));
      int duty = 100;
      if (args_count == 4) {
        duty = atoi(strtok(NULL, ","));
      }

      WMotorV1SetMotor(motor, dir, duty);
      Response_P(PSTR("{\"%s\":{\"SETMOTOR\":\"OK\"}}"), WemosMotorDriver);
      return true;
    }
  }
  return false;
}

#ifdef WEMOS_MOTOR_V2
void WMotorV2command(uint8_t *data, uint8_t len)    // process V2 request
{
  int i;
  Wire.beginTransmission(WEMOS_MOTOR_V1_ADDR);
  for (i = 0; i < len; i++) {
    Wire.write(data[i]);
  }
  Wire.endTransmission();

  if (data[0] == WV2_GET_SLAVE_STATUS) {
    Wire.requestFrom(WEMOS_MOTOR_V1_ADDR, 2);
  } else {
    Wire.requestFrom(WEMOS_MOTOR_V1_ADDR, 1);
  }
  i = 0;
  bzero(data, WMTR_V2_BUFFSZ);
  while (Wire.available())
  {
    data[i] = Wire.read();
    i++;
  }
}
#endif // WEMOS_MOTOR_V2


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv34(uint32_t function)
{
  if (!I2cEnabled(XI2C_44)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    WMotorV1Detect();
  }
  else if (WMotorV1.detected) {
    switch (function) {
      case FUNC_COMMAND_DRIVER:
        if (XI2C_44 == XdrvMailbox.index) {
          result = WMotorV1Command();
        }
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif // USE_WEMOS_MOTOR_V1
#endif // USE_IC2
