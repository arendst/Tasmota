// Copyright 2017 Jonny Graham
#ifndef IR_FUJITSU_H_
#define IR_FUJITSU_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include "IRremoteESP8266.h"
#include "IRsend.h"


// FUJITSU A/C support added by Jonny Graham

// Constants

#define FUJITSU_AC_MODE_AUTO      0x00U
#define FUJITSU_AC_MODE_COOL      0x01U
#define FUJITSU_AC_MODE_DRY       0x02U
#define FUJITSU_AC_MODE_FAN       0x03U
#define FUJITSU_AC_MODE_HEAT      0x04U

#define FUJITSU_AC_CMD_STAY_ON    0x00U
#define FUJITSU_AC_CMD_TURN_ON    0x01U
#define FUJITSU_AC_CMD_TURN_OFF   0x02U
#define FUJITSU_AC_CMD_STEP_HORIZ 0x79U
#define FUJITSU_AC_CMD_STEP_VERT  0x6CU

#define FUJITSU_AC_FAN_AUTO       0x00U
#define FUJITSU_AC_FAN_HIGH       0x01U
#define FUJITSU_AC_FAN_MED        0x02U
#define FUJITSU_AC_FAN_LOW        0x03U
#define FUJITSU_AC_FAN_QUIET      0x04U

#define FUJITSU_AC_MIN_TEMP         16U  // 16C
#define FUJITSU_AC_MAX_TEMP         30U  // 30C

#define FUJITSU_AC_SWING_OFF      0x00U
#define FUJITSU_AC_SWING_VERT     0x01U
#define FUJITSU_AC_SWING_HORIZ    0x02U
#define FUJITSU_AC_SWING_BOTH     0x03U

#define FUJITSU_AC_STATE_LENGTH      16
#define FUJITSU_AC_STATE_LENGTH_SHORT 7

#if SEND_FUJITSU_AC
class IRFujitsuAC {
 public:
  explicit IRFujitsuAC(uint16_t pin);

  void stateReset();
  void send();
  void begin();
  void off();
  void stepHoriz();
  void stepVert();
  void setCmd(uint8_t cmd);
  uint8_t getCmd();
  void setTemp(uint8_t temp);
  uint8_t getTemp();
  void setFanSpeed(uint8_t fan);
  uint8_t getFanSpeed();
  void setMode(uint8_t mode);
  uint8_t getMode();
  void setSwing(uint8_t mode);
  uint8_t getSwing();
  uint8_t* getRaw();

 private:
  uint8_t remote_state[FUJITSU_AC_STATE_LENGTH];
  uint8_t getCommandLength();
  IRsend _irsend;
  uint8_t _temp;
  uint8_t _fanSpeed;
  uint8_t _mode;
  uint8_t _swingMode;
  uint8_t _cmd;
};

#endif

#endif  // IR_FUJITSU_H_
