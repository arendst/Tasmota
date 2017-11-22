// Copyright 2017 stufisher

#include "ir_Trotec.h"
#include "IRremoteESP8266.h"
#include "IRutils.h"

// Constants
#define TROTEC_HDR_MARK   5952U
#define TROTEC_HDR_SPACE  7364U
#define TROTEC_ONE_MARK    592U
#define TROTEC_ONE_SPACE  1560U
#define TROTEC_ZERO_MARK   592U
#define TROTEC_ZERO_SPACE  592U
#define TROTEC_GAP        6184U
#define TROTEC_GAP_END    1500U  // made up value

#if SEND_TROTEC

void IRsend::sendTrotec(unsigned char data[], uint16_t nbytes,
                        uint16_t repeat) {
  if (nbytes < TROTEC_COMMAND_LENGTH)
    return;

  enableIROut(36);

  for (uint16_t r = 0; r <= repeat; r++) {
    // Header
    mark(TROTEC_HDR_MARK);
    space(TROTEC_HDR_SPACE);

    // Data
    for (uint16_t i = 0; i < nbytes; i++)
      sendData(TROTEC_ONE_MARK, TROTEC_ONE_SPACE, TROTEC_ZERO_MARK,
               TROTEC_ZERO_SPACE, data[i], 8, false);

    // Footer
    mark(TROTEC_ONE_MARK);
    space(TROTEC_GAP);
    mark(TROTEC_ONE_MARK);
    space(TROTEC_GAP_END);
  }
}

IRTrotecESP::IRTrotecESP(uint16_t pin) : _irsend(pin) {
  stateReset();
}

void IRTrotecESP::begin() {
  _irsend.begin();
}

void IRTrotecESP::send() {
  checksum();
  _irsend.sendTrotec(trotec);
}

void IRTrotecESP::checksum() {
  uint8_t sum = 0;
  uint8_t i;

  for (i = 2; i < 8; i++) sum += trotec[i];

  trotec[8] = sum & 0xFF;
}

void IRTrotecESP::stateReset() {
  for (uint8_t i = 2; i < TROTEC_COMMAND_LENGTH; i++)
    trotec[i] = 0x0;

  trotec[0] = TROTEC_INTRO1;
  trotec[1] = TROTEC_INTRO2;

  setPower(false);
  setTemp(TROTEC_DEF_TEMP);
  setSpeed(TROTEC_FAN_MED);
  setMode(TROTEC_AUTO);
}

uint8_t* IRTrotecESP::getRaw() {
  checksum();
  return trotec;
}

void IRTrotecESP::setPower(bool state) {
  if (state)
    trotec[2] |= (TROTEC_ON << 3);
  else
    trotec[2] &= ~(TROTEC_ON << 3);
}

uint8_t IRTrotecESP::getPower() {
  return trotec[2] & (TROTEC_ON << 3);
}

void IRTrotecESP::setSpeed(uint8_t speed) {
  trotec[2] = (trotec[2] & 0xcf) | (speed << 4);
}

uint8_t IRTrotecESP::getSpeed() {
  return trotec[2] & 0x30;
}

void IRTrotecESP::setMode(uint8_t mode) {
  trotec[2] = (trotec[2] & 0xfc) | mode;
}

uint8_t IRTrotecESP::getMode() {
  return trotec[2] & 0x03;
}

void IRTrotecESP::setTemp(uint8_t temp) {
  if (temp < TROTEC_MIN_TEMP)
    temp = TROTEC_MIN_TEMP;
  else if (temp > TROTEC_MAX_TEMP)
    temp = TROTEC_MAX_TEMP;

  trotec[3] = (trotec[3] & 0x80) | (temp - TROTEC_MIN_TEMP);
}

uint8_t IRTrotecESP::getTemp() {
  return trotec[3] & 0x7f;
}

void IRTrotecESP::setSleep(bool sleep) {
  if (sleep)
    trotec[3] |= (TROTEC_SLEEP_ON << 7);
  else
    trotec[3] &= ~(TROTEC_SLEEP_ON << 7);
}

bool IRTrotecESP::getSleep(void) {
  return trotec[3] & (TROTEC_SLEEP_ON << 7);
}

void IRTrotecESP::setTimer(uint8_t timer) {
  if (timer > TROTEC_MAX_TIMER) timer = TROTEC_MAX_TIMER;

  if (timer) {
    trotec[5] |= (TROTEC_TIMER_ON << 6);
    trotec[6] = timer;
  } else {
    trotec[5] &= ~(TROTEC_TIMER_ON << 6);
    trotec[6] = 0;
  }
}

uint8_t IRTrotecESP::getTimer() {
  return trotec[6];
}

#endif  // SEND_TROTEC
