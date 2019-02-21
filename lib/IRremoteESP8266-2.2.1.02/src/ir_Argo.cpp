/*
Node MCU/ESP8266 Sketch to emulate Argo Ulisse 13 DCI remote
Controls Argo Ulisse 13 DCI A/C
Copyright 2017 Schmolders
*/

#include "ir_Argo.h"
#include <algorithm>
#include "IRremoteESP8266.h"
#include "IRutils.h"

// Constants
// using SPACE modulation. MARK is always const 400u
#define ARGO_PREAMBLE_1           6400U  // Mark
#define ARGO_PREAMBLE_2           3300U  // Space
#define ARGO_MARK                  400U
#define ARGO_ONE_SPACE            2200U
#define ARGO_ZERO_SPACE            900U

#if SEND_ARGO
// Send a Argo A/C message.
//
// Args:
//   data: An array of ARGO_COMMAND_LENGTH bytes containing the IR command.
//
// Status: ALPHA / Untested.
//
// Overloading the IRSend Function

void IRsend::sendArgo(unsigned char data[], uint16_t nbytes, uint16_t repeat) {
  // Check if we have enough bytes to send a proper message.
  if (nbytes < ARGO_COMMAND_LENGTH) return;
  // Set IR carrier frequency
  enableIROut(38);
  for (uint16_t r = 0; r <= repeat; r++) {
    // Header
    // TODO(kaschmo): validate
    mark(ARGO_PREAMBLE_1);
    space(ARGO_PREAMBLE_2);
    // send data, defined in IRSend.cpp
    for (uint16_t i = 0; i < nbytes; i++)
      sendData(ARGO_MARK, ARGO_ONE_SPACE, ARGO_MARK,
               ARGO_ZERO_SPACE, data[i], 8, false);
               // send LSB first reverses the bit order in array for sending.
  }
}

IRArgoAC::IRArgoAC(uint16_t pin) : _irsend(pin) {
  stateReset();
}

void IRArgoAC::begin() {
  _irsend.begin();
}

void IRArgoAC::send() {
  // Serial.println("Sending IR code"); // Only for Debug
  checksum();  // Create valid checksum before sending
  _irsend.sendArgo(argo);
}

void IRArgoAC::checksum() {
  uint8_t sum = 2;  // Corresponds to byte 11 being constant 0b01
  uint8_t i;

  // Only add up bytes to 9. byte 10 is 0b01 constant anyway.
  // Assume that argo array is MSB first (left)
  for (i = 0; i < 10; i++)
    sum += argo[i];

  sum = sum % 256;  // modulo 256
  // Append sum to end of array
  // Set const part of checksum bit 10
  argo[10] = 0b00000010;
  argo[10] += sum << 2;  // Shift up 2 bits and append to byte 10
  argo[11] = sum >> 6;  // Shift down 6 bits and add in two LSBs of bit 11
}

void IRArgoAC::stateReset() {
  for (uint8_t i = 0; i < ARGO_COMMAND_LENGTH; i++)
    argo[i] = 0x0;

  // Argo Message. Store MSB left.
  // Default message:
  argo[0] = 0b10101100;  // LSB first (as sent) 0b00110101; //const preamble
  argo[1] = 0b11110101;  // LSB first: 0b10101111; //const preamble
  // Keep payload 2-9 at zero
  argo[10] = 0b00000010;  // Const 01, checksum 6bit
  argo[11] = 0b00000000;  // Checksum 2bit

  this->off();
  this->setTemp(20);
  this->setRoomTemp(25);
  this->setCoolMode(ARGO_COOL_AUTO);
  this->setFan(ARGO_FAN_AUTO);
}

uint8_t* IRArgoAC::getRaw() {
  checksum();  // Ensure correct bit array before returning
  return argo;
}

void IRArgoAC::on() {
  // state = ON;
  ac_state = 1;
  // Bit 5 of byte 9 is on/off
  // in MSB first
  argo[9] = argo[9] | 0b00100000;  // Set ON/OFF bit to 1
}

void IRArgoAC::off() {
  // state = OFF;
  ac_state = 0;
  // in MSB first
  // bit 5 of byte 9 to off
  argo[9] = argo[9] & 0b11011111;  // Set on/off bit to 0
}

void IRArgoAC::setPower(bool state) {
  if (state)
    on();
  else
    off();
}

uint8_t IRArgoAC::getPower() {
  return ac_state;
}

void IRArgoAC::setMax(bool state) {
  max_mode = state;
  if (max_mode)
    argo[9] |= 0b00001000;
  else
    argo[9] &= 0b11110111;
}

bool IRArgoAC::getMax() {
  return max_mode;
}

// Set the temp in deg C
// Sending 0 equals +4
void IRArgoAC::setTemp(uint8_t temp) {
  if (temp < ARGO_MIN_TEMP)
    temp = ARGO_MIN_TEMP;
  else if (temp > ARGO_MAX_TEMP)
    temp = ARGO_MAX_TEMP;

  // Store in attributes
  set_temp = temp;
  // offset 4 degrees. "If I want 12 degrees, I need to send 8"
  temp -= 4;
  // Settemp = Bit 6,7 of byte 2, and bit 0-2 of byte 3
  // mask out bits
  // argo[13] & 0x00000100;  // mask out ON/OFF Bit
  argo[2] &= 0b00111111;
  argo[3] &= 0b11111000;

  argo[2] += temp << 6;  // append to bit 6,7
  argo[3] += temp >> 2;  // remove lowest to bits and append in 0-2
}

uint8_t IRArgoAC::getTemp() {
  return set_temp;
}

// Set the speed of the fan
void IRArgoAC::setFan(uint8_t fan) {
  // Set the fan speed bits, leave low 4 bits alone
  fan_mode = fan;
  // Mask out bits
  argo[3] &= 0b11100111;
  // Set fan mode at bit positions
  argo[3] += fan << 3;
}

uint8_t IRArgoAC::getFan() {
  return fan_mode;
}

void IRArgoAC::setFlap(uint8_t flap) {
  flap_mode = flap;
  // TODO(kaschmo): set correct bits for flap mode
}

uint8_t IRArgoAC::getFlap() {
  return flap_mode;
}

uint8_t IRArgoAC::getMode() {
  // return cooling 0, heating 1
  return ac_mode;
}

void IRArgoAC::setCoolMode(uint8_t mode) {
  ac_mode = 0;  // Set ac mode to cooling
  cool_mode = mode;
  // Mask out bits, also leave bit 5 on 0 for cooling
  argo[2] &= 0b11000111;

  // Set cool mode at bit positions
  argo[2] += mode << 3;
}

uint8_t IRArgoAC::getCoolMode() {
  return cool_mode;
}

void IRArgoAC::setHeatMode(uint8_t mode) {
  ac_mode = 1;  // Set ac mode to heating
  heat_mode = mode;
  // Mask out bits
  argo[2] &= 0b11000111;
  // Set heating bit
  argo[2] |= 0b00100000;
  // Set cool mode at bit positions
  argo[2] += mode << 3;
}

uint8_t IRArgoAC::getHeatMode() {
  return heat_mode;
}

void IRArgoAC::setNight(bool state) {
  night_mode = state;
  if (night_mode)
    // Set bit at night position: bit 2
    argo[9] |= 0b00000100;
  else
    argo[9] &= 0b11111011;
}

bool IRArgoAC::getNight() {
  return night_mode;
}

void IRArgoAC::setiFeel(bool state) {
  ifeel_mode = state;
  if (ifeel_mode)
    // Set bit at iFeel position: bit 7
    argo[9] |= 0b10000000;
  else
    argo[9] &= 0b01111111;
}

bool IRArgoAC::getiFeel() {
  return ifeel_mode;
}

void IRArgoAC::setTime() {
  // TODO(kaschmo): use function call from checksum to set time first
}

void IRArgoAC::setRoomTemp(uint8_t temp) {
  temp -= 4;
  // Mask out bits
  argo[3] &= 0b00011111;
  argo[4] &= 0b11111100;

  argo[3] += temp << 5;  // Append to bit 5,6,7
  argo[4] += temp >> 3;  // Remove lowest 3 bits and append in 0,1
}
#endif  // SEND_ARGO
