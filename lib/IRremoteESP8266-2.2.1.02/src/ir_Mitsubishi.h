// Copyright 2009 Ken Shirriff
// Copyright 2017 David Conran
#ifndef IR_MITSUBISHI_H_
#define IR_MITSUBISHI_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include "IRremoteESP8266.h"
#include "IRsend.h"

//    MMMMM  IIIII TTTTT   SSSS  U   U  BBBB   IIIII   SSSS  H   H  IIIII
//    M M M    I     T    S      U   U  B   B    I    S      H   H    I
//    M M M    I     T     SSS   U   U  BBBB     I     SSS   HHHHH    I
//    M   M    I     T        S  U   U  B   B    I        S  H   H    I
//    M   M  IIIII   T    SSSS    UUU   BBBBB  IIIII  SSSS   H   H  IIIII

// Mitsubishi (TV) decoding added from https://github.com/z3t0/Arduino-IRremote
// Mitsubishi (TV) sending & Mitsubishi A/C support added by David Conran

// Constants
#define MITSUBISHI_AC_AUTO           0x20U
#define MITSUBISHI_AC_COOL           0x18U
#define MITSUBISHI_AC_DRY            0x10U
#define MITSUBISHI_AC_HEAT           0x08U
#define MITSUBISHI_AC_POWER          0x20U
#define MITSUBISHI_AC_FAN_AUTO          0U
#define MITSUBISHI_AC_FAN_MAX           5U
#define MITSUBISHI_AC_FAN_REAL_MAX      4U
#define MITSUBISHI_AC_FAN_SILENT        6U
#define MITSUBISHI_AC_MIN_TEMP         16U  // 16C
#define MITSUBISHI_AC_MAX_TEMP         31U  // 31C
#define MITSUBISHI_AC_VANE_AUTO         0U
#define MITSUBISHI_AC_VANE_AUTO_MOVE    7U

#if SEND_MITSUBISHI_AC
class IRMitsubishiAC {
 public:
  explicit IRMitsubishiAC(uint16_t pin);

  void stateReset();
  void send();
  void begin();
  void on();
  void off();
  void setPower(bool state);
  bool getPower();
  void setTemp(uint8_t temp);
  uint8_t getTemp();
  void setFan(uint8_t fan);
  uint8_t getFan();
  void setMode(uint8_t mode);
  uint8_t getMode();
  void setVane(uint8_t mode);
  uint8_t getVane();
  uint8_t* getRaw();

 private:
  uint8_t remote_state[MITSUBISHI_AC_STATE_LENGTH];
  void checksum();
  IRsend _irsend;
};

#endif

#endif  // IR_MITSUBISHI_H_
