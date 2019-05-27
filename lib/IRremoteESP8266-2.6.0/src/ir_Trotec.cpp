// Copyright 2017 stufisher

#include "ir_Trotec.h"
#include <algorithm>
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
void IRTrotecESP::send(const uint16_t repeat) {
  checksum();
  _irsend.sendTrotec(remote_state, kTrotecStateLength, repeat);
}
#endif  // SEND_TROTEC

void IRTrotecESP::checksum() {
  uint8_t sum = 0;

  for (uint8_t i = 2; i < 8; i++) sum += remote_state[i];
  remote_state[8] = sum & 0xFF;
}

void IRTrotecESP::stateReset() {
  for (uint8_t i = 2; i < kTrotecStateLength; i++) remote_state[i] = 0x0;

  remote_state[0] = kTrotecIntro1;
  remote_state[1] = kTrotecIntro2;

  setPower(false);
  setTemp(kTrotecDefTemp);
  setSpeed(kTrotecFanMed);
  setMode(kTrotecAuto);
}

uint8_t* IRTrotecESP::getRaw() {
  checksum();
  return remote_state;
}

void IRTrotecESP::setPower(const bool on) {
  if (on)
    remote_state[2] |= kTrotecPowerBit;
  else
    remote_state[2] &= ~kTrotecPowerBit;
}

bool IRTrotecESP::getPower() { return remote_state[2] & kTrotecPowerBit; }

void IRTrotecESP::setSpeed(const uint8_t fan) {
  uint8_t speed = std::min(fan, kTrotecFanHigh);
  remote_state[2] = (remote_state[2] & 0b11001111) | (speed << 4);
}

uint8_t IRTrotecESP::getSpeed() { return (remote_state[2] & 0b00110000) >> 4; }

void IRTrotecESP::setMode(const uint8_t mode) {
  switch (mode) {
    case kTrotecAuto:
    case kTrotecCool:
    case kTrotecDry:
    case kTrotecFan:
      remote_state[2] = (remote_state[2] & 0b11111100) | mode;
      return;
    default:
      this->setMode(kTrotecAuto);
  }
}

uint8_t IRTrotecESP::getMode() { return remote_state[2] & 0b00000011; }

void IRTrotecESP::setTemp(const uint8_t celsius) {
  uint8_t temp = std::max(celsius, kTrotecMinTemp);
  temp = std::min(temp, kTrotecMaxTemp);
  remote_state[3] = (remote_state[3] & 0x80) | (temp - kTrotecMinTemp);
}

uint8_t IRTrotecESP::getTemp() {
  return (remote_state[3] & 0b01111111) + kTrotecMinTemp;
}

void IRTrotecESP::setSleep(bool sleep) {
  if (sleep)
    remote_state[3] |= kTrotecSleepBit;
  else
    remote_state[3] &= ~kTrotecSleepBit;
}

bool IRTrotecESP::getSleep(void) { return remote_state[3] & kTrotecSleepBit; }

void IRTrotecESP::setTimer(const uint8_t timer) {
  if (timer)
    remote_state[5] |= kTrotecTimerBit;
  else
    remote_state[5] &= ~kTrotecTimerBit;
  remote_state[6] = (timer > kTrotecMaxTimer) ? kTrotecMaxTimer : timer;
}

uint8_t IRTrotecESP::getTimer() { return remote_state[6]; }

// Convert a standard A/C mode into its native mode.
uint8_t IRTrotecESP::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool:
      return kTrotecCool;
    case stdAc::opmode_t::kDry:
      return kTrotecDry;
    case stdAc::opmode_t::kFan:
      return kTrotecFan;
    // Note: No Heat mode.
    default:
      return kTrotecAuto;
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRTrotecESP::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:
      return kTrotecFanLow;
    case stdAc::fanspeed_t::kMedium:
      return kTrotecFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:
      return kTrotecFanHigh;
    default:
      return kTrotecFanMed;
  }
}
