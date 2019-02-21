// Copyright 2017 stufisher

#include "ir_Trotec.h"
#include "IRremoteESP8266.h"
#include "IRutils.h"

// Constants
const uint16_t kTrotecHdrMark = 5952;
const uint16_t kTrotecHdrSpace = 7364;
const uint16_t kTrotecOneMark = 592;
const uint16_t kTrotecOneSpace = 1560;
const uint16_t kTrotecZeroMark = 592;
const uint16_t kTrotecZeroSpace = 592;
const uint16_t kTrotecGap = 6184;
const uint16_t kTrotecGapEnd = 1500;  // made up value

#if SEND_TROTEC

void IRsend::sendTrotec(unsigned char data[], uint16_t nbytes,
                        uint16_t repeat) {
  if (nbytes < kTrotecStateLength) return;

  for (uint16_t r = 0; r <= repeat; r++) {
    sendGeneric(kTrotecHdrMark, kTrotecHdrSpace, kTrotecOneMark,
                kTrotecOneSpace, kTrotecZeroMark, kTrotecZeroSpace,
                kTrotecOneMark, kTrotecGap, data, nbytes, 36, false,
                0,  // Repeats handled elsewhere
                50);
    // More footer
    enableIROut(36);
    mark(kTrotecOneMark);
    space(kTrotecGapEnd);
  }
}
#endif  // SEND_TROTEC

IRTrotecESP::IRTrotecESP(uint16_t pin) : _irsend(pin) { stateReset(); }

void IRTrotecESP::begin() { _irsend.begin(); }

#if SEND_TROTEC
void IRTrotecESP::send() {
  checksum();
  _irsend.sendTrotec(trotec);
}
#endif  // SEND_TROTEC

void IRTrotecESP::checksum() {
  uint8_t sum = 0;
  uint8_t i;

  for (i = 2; i < 8; i++) sum += trotec[i];

  trotec[8] = sum & 0xFF;
}

void IRTrotecESP::stateReset() {
  for (uint8_t i = 2; i < kTrotecStateLength; i++) trotec[i] = 0x0;

  trotec[0] = kTrotecIntro1;
  trotec[1] = kTrotecIntro2;

  setPower(false);
  setTemp(kTrotecDefTemp);
  setSpeed(kTrotecFanMed);
  setMode(kTrotecAuto);
}

uint8_t* IRTrotecESP::getRaw() {
  checksum();
  return trotec;
}

void IRTrotecESP::setPower(bool state) {
  if (state)
    trotec[2] |= (kTrotecOn << 3);
  else
    trotec[2] &= ~(kTrotecOn << 3);
}

uint8_t IRTrotecESP::getPower() { return trotec[2] & (kTrotecOn << 3); }

void IRTrotecESP::setSpeed(uint8_t speed) {
  trotec[2] = (trotec[2] & 0xcf) | (speed << 4);
}

uint8_t IRTrotecESP::getSpeed() { return trotec[2] & 0x30; }

void IRTrotecESP::setMode(uint8_t mode) {
  trotec[2] = (trotec[2] & 0xfc) | mode;
}

uint8_t IRTrotecESP::getMode() { return trotec[2] & 0x03; }

void IRTrotecESP::setTemp(uint8_t temp) {
  if (temp < kTrotecMinTemp)
    temp = kTrotecMinTemp;
  else if (temp > kTrotecMaxTemp)
    temp = kTrotecMaxTemp;

  trotec[3] = (trotec[3] & 0x80) | (temp - kTrotecMinTemp);
}

uint8_t IRTrotecESP::getTemp() { return trotec[3] & 0x7f; }

void IRTrotecESP::setSleep(bool sleep) {
  if (sleep)
    trotec[3] |= (kTrotecSleepOn << 7);
  else
    trotec[3] &= ~(kTrotecSleepOn << 7);
}

bool IRTrotecESP::getSleep(void) { return trotec[3] & (kTrotecSleepOn << 7); }

void IRTrotecESP::setTimer(uint8_t timer) {
  if (timer > kTrotecMaxTimer) timer = kTrotecMaxTimer;

  if (timer) {
    trotec[5] |= (kTrotecTimerOn << 6);
    trotec[6] = timer;
  } else {
    trotec[5] &= ~(kTrotecTimerOn << 6);
    trotec[6] = 0;
  }
}

uint8_t IRTrotecESP::getTimer() { return trotec[6]; }
