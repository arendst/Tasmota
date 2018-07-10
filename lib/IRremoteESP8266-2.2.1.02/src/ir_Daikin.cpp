/*
An Arduino sketch to emulate IR Daikin ARC433** remote control unit
Read more at:
http://harizanov.com/2012/02/control-daikin-air-conditioner-over-the-internet/

Copyright 2016 sillyfrog
*/

#include "ir_Daikin.h"
#include <algorithm>
#include "IRremoteESP8266.h"
#include "IRutils.h"

//                DDDDD     AAA   IIIII KK  KK IIIII NN   NN
//                DD  DD   AAAAA   III  KK KK   III  NNN  NN
//                DD   DD AA   AA  III  KKKK    III  NN N NN
//                DD   DD AAAAAAA  III  KK KK   III  NN  NNN
//                DDDDDD  AA   AA IIIII KK  KK IIIII NN   NN

// Constants
// Ref:
//   https://github.com/mharizanov/Daikin-AC-remote-control-over-the-Internet/tree/master/IRremote
#define DAIKIN_HDR_MARK            3650U  // DAIKIN_ZERO_MARK * 8
#define DAIKIN_HDR_SPACE           1623U  // DAIKIN_ZERO_MARK * 4
#define DAIKIN_ONE_SPACE           1280U
#define DAIKIN_ONE_MARK             428U
#define DAIKIN_ZERO_MARK            428U
#define DAIKIN_ZERO_SPACE           428U
#define DAIKIN_GAP                29000U

#if SEND_DAIKIN
// Send a Daikin A/C message.
//
// Args:
//   data: An array of DAIKIN_COMMAND_LENGTH bytes containing the IR command.
//
// Status: STABLE
//
// Ref:
//   IRDaikinESP.cpp
//   https://github.com/mharizanov/Daikin-AC-remote-control-over-the-Internet/tree/master/IRremote
void IRsend::sendDaikin(unsigned char data[], uint16_t nbytes,
                        uint16_t repeat) {
  if (nbytes < DAIKIN_COMMAND_LENGTH)
    return;  // Not enough bytes to send a proper message.
  // Set IR carrier frequency
  enableIROut(38);
  for (uint16_t r = 0; r <= repeat; r++) {
    // Header #1
    mark(DAIKIN_HDR_MARK);
    space(DAIKIN_HDR_SPACE);
    // Data #1
    for (uint16_t i = 0; i < 8 && i < nbytes; i++)
      sendData(DAIKIN_ONE_MARK, DAIKIN_ONE_SPACE, DAIKIN_ZERO_MARK,
               DAIKIN_ZERO_SPACE, data[i], 8, false);
    // Footer #1
    mark(DAIKIN_ONE_MARK);
    space(DAIKIN_ZERO_SPACE + DAIKIN_GAP);

    // Header #2
    mark(DAIKIN_HDR_MARK);
    space(DAIKIN_HDR_SPACE);
    // Data #2
    for (uint16_t i = 8; i < nbytes; i++)
      sendData(DAIKIN_ONE_MARK, DAIKIN_ONE_SPACE, DAIKIN_ZERO_MARK,
               DAIKIN_ZERO_SPACE, data[i], 8, false);
    // Footer #2
    mark(DAIKIN_ONE_MARK);
    space(DAIKIN_ZERO_SPACE + DAIKIN_GAP);
  }
}

IRDaikinESP::IRDaikinESP(uint16_t pin) : _irsend(pin) {
  stateReset();
}

void IRDaikinESP::begin() {
  _irsend.begin();
}

void IRDaikinESP::send() {
  _irsend.sendDaikin(daikin);
}

void IRDaikinESP::checksum() {
  uint8_t sum = 0;
  uint8_t i;

  for (i = 0; i <= 6; i++)
    sum += daikin[i];

  daikin[7] = sum & 0xFF;
  sum = 0;
  for (i = 8; i <= 25; i++)
    sum += daikin[i];
  daikin[26] = sum & 0xFF;
}


void IRDaikinESP::stateReset() {
  for (uint8_t i = 4; i < DAIKIN_COMMAND_LENGTH; i++)
    daikin[i] = 0x0;

  daikin[0] = 0x11;
  daikin[1] = 0xDA;
  daikin[2] = 0x27;
  daikin[3] = 0xF0;
  daikin[7] = 0x20;
  daikin[8] = 0x11;
  daikin[9] = 0xDA;
  daikin[10] = 0x27;
  daikin[13] = 0x41;
  daikin[14] = 0x1E;
  daikin[16] = 0xB0;
  daikin[23] = 0xC0;
  daikin[26] = 0xE3;
  checksum();
}

uint8_t* IRDaikinESP::getRaw() {
  checksum();   // Ensure correct settings before sending.
  return daikin;
}

void IRDaikinESP::on() {
  // state = ON;
  daikin[13] |= 0x01;
  checksum();
}

void IRDaikinESP::off() {
  // state = OFF;
  daikin[13] &= 0xFE;
  checksum();
}

void IRDaikinESP::setPower(bool state) {
  if (state)
    on();
  else
    off();
}

uint8_t IRDaikinESP::getPower() {
  return daikin[13] & 0x01;
}

// DAIKIN_SILENT or DAIKIN_POWERFUL
void IRDaikinESP::setAux(uint8_t aux) {
  daikin[21] = aux;
  checksum();
}

uint8_t IRDaikinESP::getAux() {
  return daikin[21];
}

void IRDaikinESP::setQuiet(bool state) {
  if (state)
    setAux(DAIKIN_SILENT);
  else
    setAux(0x0);
}

bool IRDaikinESP::getQuiet() {
  return (getAux() == DAIKIN_SILENT);
}

void IRDaikinESP::setPowerful(bool state) {
  if (state)
    setAux(DAIKIN_POWERFUL);
  else
    setAux(0x0);
}

bool IRDaikinESP::getPowerful() {
  return (getAux() == DAIKIN_POWERFUL);
}

// Set the temp in deg C
void IRDaikinESP::setTemp(uint8_t temp) {
  if (temp < DAIKIN_MIN_TEMP)
    temp = DAIKIN_MIN_TEMP;
  else if (temp > DAIKIN_MAX_TEMP)
    temp = DAIKIN_MAX_TEMP;
  daikin[14] = temp * 2;
  checksum();
}

uint8_t IRDaikinESP::getTemp() {
  return daikin[14] / 2;
}

// Set the speed of the fan, 0-5, 0 is auto, 1-5 is the speed
void IRDaikinESP::setFan(uint8_t fan) {
  // Set the fan speed bits, leave low 4 bits alone
  uint8_t fanset;
  daikin[16] &= 0x0F;
  fan = std::min(fan, DAIKIN_FAN_MAX);
  if (fan == DAIKIN_FAN_AUTO)
    fanset = 0xA0;
  else
    fanset = 0x20 + (0x10 * fan);
  daikin[16] |= fanset;
  checksum();
}

uint8_t IRDaikinESP::getFan() {
  uint8_t fan = daikin[16] >> 4;
  fan -= 2;
  if (fan > DAIKIN_FAN_MAX)
    fan = DAIKIN_FAN_AUTO;
  return fan;
}

uint8_t IRDaikinESP::getMode() {
  /*
  DAIKIN_COOL
  DAIKIN_HEAT
  DAIKIN_FAN
  DAIKIN_AUTO
  DAIKIN_DRY
  */
  return daikin[13] >> 4;
}

void IRDaikinESP::setMode(uint8_t mode) {
  switch (mode) {
    case DAIKIN_COOL:
    case DAIKIN_HEAT:
    case DAIKIN_FAN:
    case DAIKIN_DRY:
      break;
    default:
      mode = DAIKIN_AUTO;
  }
  daikin[13] = (mode << 4) | getPower();
  checksum();
}

void IRDaikinESP::setSwingVertical(bool state) {
  if (state)
    daikin[16] |= 0x0F;
  else
    daikin[16] &= 0xF0;
  checksum();
}

bool IRDaikinESP::getSwingVertical() {
  return daikin[16] & 0x01;
}

void IRDaikinESP::setSwingHorizontal(bool state) {
  if (state)
    daikin[17] |= 0x0F;
  else
    daikin[17] &= 0xF0;
  checksum();
}

bool IRDaikinESP::getSwingHorizontal() {
  return daikin[17] & 0x01;
}
#endif  // SEND_DAIKIN

#if DECODE_DAIKIN
// TODO(crankyoldgit): NOT WORKING. This needs to be finished.
// Decode the supplied Daikin A/C message. (NOT WORKING - DO NOT USE)
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of bits to expect in the data portion. Typically SAMSUNG_BITS.
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: UNFINISHED / Completely not working, not even vaguely.
//
// Ref:
//   https://github.com/mharizanov/Daikin-AC-remote-control-over-the-Internet/tree/master/IRremote
bool IRrecv::decodeDaikin(decode_results *results, uint16_t nbits,
                          bool strict) {
  if (results->rawlen < 2 * nbits + HEADER + FOOTER)
    return false;

  // Compliance
  if (strict && nbits != DAIKIN_BITS)
    return false;

  uint32_t data = 0;
  uint16_t offset = OFFSET_START;

  // Header
  if (!matchMark(results->rawbuf[offset++], DAIKIN_HDR_MARK))
      return false;
  if (!matchSpace(results->rawbuf[offset++], DAIKIN_HDR_SPACE))
      return false;

  // Data (#1)
  for (uint8_t i = 0; i < sizeof(data) * 8; i++, offset++) {
    if (!matchMark(results->rawbuf[offset++], DAIKIN_ONE_MARK))
      return false;
    if (matchSpace(results->rawbuf[offset], DAIKIN_ONE_SPACE))
      data = (data << 1) | 1;  // 1
    else if (matchSpace(results->rawbuf[offset], DAIKIN_ZERO_SPACE))
      data <<= 1;  // 0
    else
      return false;
  }

  uint32_t number = data;  // some number...
  uint32_t reversed = reverseBits(number, sizeof(number) * 8)

  DPRINT("Code ");
  DPRINTLN(reversed, HEX);

  // Data (#2)
  for (uint8_t i = 0; i < sizeof(data) * 8; i++, offset++) {
    if (!matchMark(results->rawbuf[offset++], DAIKIN_ONE_MARK))
      return false;
    if (matchSpace(results->rawbuf[offset], DAIKIN_ONE_SPACE))
      data = (data << 1) | 1;  // 1
    else if (matchSpace(results->rawbuf[offset], DAIKIN_ZERO_SPACE))
      data <<= 1;  // 0
    else
      return false;
  }

  number = data;  // some number...
  reversed = reverseBits(number, sizeof(number) * 8)

  DPRINT("Code2 ");
  DPRINTLN(reversed, HEX);

  if (!matchSpace(results->rawbuf[offset++], DAIKIN_GAP)) {
    DPRINTLN("no gap");
    return false;
  }

  // Success
  results->bits = DAIKIN_BITS;
  results->value = reversed;
  results->decode_type = DAIKIN;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif  // DECODE_DAIKIN
