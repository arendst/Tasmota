/*
Code to emulate IR Kelvinator YALIF remote control unit, which should control
at least the following Kelvinator A/C units:
  KSV26CRC, KSV26HRC, KSV35CRC, KSV35HRC, KSV53HRC, KSV62HRC, KSV70CRC,
  KSV70HRC, KSV80HRC.

Note:
  * Unsupported:
    - All Sleep modes.
    - All Timer modes.
    - "I Feel" button & mode.
    - Energy Saving mode.
    - Low Heat mode.
    - Farenheit.
*/

#include <IRKelvinator.h>

IRKelvinatorAC::IRKelvinatorAC(int pin) : _irsend(pin) {
  stateReset();
}

void IRKelvinatorAC::stateReset() {
  for (uint8_t i = 0; i < KELVINATOR_STATE_LENGTH; i++)
    remote_state[i] = 0x0;
  remote_state[3] = 0x50;
  remote_state[11] = 0x70;
}

void IRKelvinatorAC::begin() {
    _irsend.begin();
}

void IRKelvinatorAC::fixup() {
  // X-Fan mode is only valid in COOL or DRY modes.
  if (getMode() != KELVINATOR_COOL && getMode() != KELVINATOR_DRY)
    setXFan(false);
  checksum();  // Calculate the checksums
}

void IRKelvinatorAC::send() {
  fixup();   // Ensure correct settings before sending.
  _irsend.sendKelvinator(remote_state);
}

uint8_t* IRKelvinatorAC::getRaw() {
  fixup();   // Ensure correct settings before sending.
  return remote_state;
}

// Many Bothans died to bring us this information.
void IRKelvinatorAC::checksum() {
  // For each command + options block.
  for (uint8_t offset = 0; offset < KELVINATOR_STATE_LENGTH; offset += 8) {
    uint8_t sum = KELVINATOR_CHECKSUM_START;
    // Sum the lower half of the first 4 bytes of this block.
    for(uint8_t i = 0; i < 4; i++) {
      sum += (remote_state[i + offset] & 0xFU);
    }
    // then sum the upper half of the next 3 bytes.
    for(uint8_t i = 4; i < 7; i++) {
      sum += (remote_state[i + offset] >> 4);
    }
    // Trim it down to fit into the 4 bits allowed. i.e. Mod 16.
    sum &= 0xFU;
    // Place it into the IR code in the top half of the 8th & 16th byte.
    remote_state[7 + offset] = (sum << 4) | (remote_state[7 + offset] & 0xFU);
  }
}

void IRKelvinatorAC::on() {
    //state = ON;
    remote_state[0] |= KELVINATOR_POWER;
    remote_state[8] = remote_state[0];  // Duplicate to the 2nd command chunk.
}

void IRKelvinatorAC::off() {
    //state = OFF;
    remote_state[0] &= ~KELVINATOR_POWER;
    remote_state[8] = remote_state[0];  // Duplicate to the 2nd command chunk.
}

void IRKelvinatorAC::setPower(bool state) {
  if (state)
    on();
  else
    off();
}

bool IRKelvinatorAC::getPower() {
    return ((remote_state[0] & KELVINATOR_POWER) != 0);
}

// Set the temp. in deg C
void IRKelvinatorAC::setTemp(uint8_t temp) {
    temp = max(KELVINATOR_MIN_TEMP, temp);
    temp = min(KELVINATOR_MAX_TEMP, temp);
    remote_state[1] = (remote_state[1] & 0xF0U) | (temp - KELVINATOR_MIN_TEMP);
    remote_state[9] = remote_state[1];  // Duplicate to the 2nd command chunk.
}

// Return the set temp. in deg C
uint8_t IRKelvinatorAC::getTemp() {
    return ((remote_state[1] & 0xFU) + KELVINATOR_MIN_TEMP);
}

// Set the speed of the fan, 0-5, 0 is auto, 1-5 is the speed
void IRKelvinatorAC::setFan(uint8_t fan) {
  fan = min(KELVINATOR_FAN_MAX, fan);  // Bounds check

  // Only change things if we need to.
  if (fan != getFan()) {
    // Set the basic fan values.
    uint8_t fan_basic = min(KELVINATOR_BASIC_FAN_MAX, fan);
    remote_state[0] = (remote_state[0] & KELVINATOR_BASIC_FAN_MASK) |
        (fan_basic << KELVINATOR_FAN_OFFSET);
    remote_state[8] = remote_state[0];  // Duplicate to the 2nd command chunk.
    // Set the advanced(?) fan value.
    remote_state[14] = (remote_state[14] & KELVINATOR_FAN_MASK) |
        (fan << KELVINATOR_FAN_OFFSET);
    setTurbo(false);  // Turbo mode is turned off if we change the fan settings.
  }
}

uint8_t IRKelvinatorAC::getFan() {
    return ((remote_state[14] & ~KELVINATOR_FAN_MASK) >> KELVINATOR_FAN_OFFSET);
}

uint8_t IRKelvinatorAC::getMode() {
  /*
  KELVINATOR_AUTO
  KELVINATOR_COOL
  KELVINATOR_DRY
  KELVINATOR_FAN
  KELVINATOR_HEAT
  */
    return (remote_state[0] & ~KELVINATOR_MODE_MASK);
}

void IRKelvinatorAC::setMode(uint8_t mode) {
  // If we get an unexpected mode, default to AUTO.
  if (mode > KELVINATOR_HEAT) mode = KELVINATOR_AUTO;
  remote_state[0] = (remote_state[0] & KELVINATOR_MODE_MASK) | mode;
  remote_state[8] = remote_state[0];  // Duplicate to the 2nd command chunk.
  if (mode == KELVINATOR_AUTO)
    // When the remote is set to Auto, it defaults to 25C and doesn't show it.
    setTemp(KELVINATOR_AUTO_TEMP);
}

void IRKelvinatorAC::setSwingVertical(bool state) {
  if (state) {
    remote_state[0] |= KELVINATOR_VENT_SWING;
    remote_state[4] |= KELVINATOR_VENT_SWING_V;
  }
  else {
    remote_state[4] &= ~KELVINATOR_VENT_SWING_V;
    if (! getSwingHorizontal())
      remote_state[0] &= ~KELVINATOR_VENT_SWING;
  }
  remote_state[8] = remote_state[0];  // Duplicate to the 2nd command chunk.
}

bool IRKelvinatorAC::getSwingVertical() {
  return ((remote_state[4] & KELVINATOR_VENT_SWING_V) != 0);
}

void IRKelvinatorAC::setSwingHorizontal(bool state) {
  if (state) {
    remote_state[0] |= KELVINATOR_VENT_SWING;
    remote_state[4] |= KELVINATOR_VENT_SWING_H;
  }
  else {
    remote_state[4] &= ~KELVINATOR_VENT_SWING_H;
    if (! getSwingVertical())
      remote_state[0] &= ~KELVINATOR_VENT_SWING;
  }
  remote_state[8] = remote_state[0];  // Duplicate to the 2nd command chunk.
}

bool IRKelvinatorAC::getSwingHorizontal() {
  return ((remote_state[4] & KELVINATOR_VENT_SWING_H) != 0);
}

void IRKelvinatorAC::setQuiet(bool state) {
  remote_state[12] &= ~KELVINATOR_QUIET;
  remote_state[12] |= (state << KELVINATOR_QUIET_OFFSET);
}

bool IRKelvinatorAC::getQuiet() {
  return ((remote_state[12] & KELVINATOR_QUIET) != 0);
}

void IRKelvinatorAC::setIonFilter(bool state) {
  remote_state[2] &= ~KELVINATOR_ION_FILTER;
  remote_state[2] |= (state << KELVINATOR_ION_FILTER_OFFSET);
  remote_state[10] = remote_state[2];  // Duplicate to the 2nd command chunk.
}

bool IRKelvinatorAC::getIonFilter() {
  return ((remote_state[2] & KELVINATOR_ION_FILTER) != 0);
}

void IRKelvinatorAC::setLight(bool state) {
  remote_state[2] &= ~KELVINATOR_LIGHT;
  remote_state[2] |= (state << KELVINATOR_LIGHT_OFFSET);
  remote_state[10] = remote_state[2];  // Duplicate to the 2nd command chunk.
}

bool IRKelvinatorAC::getLight() {
  return ((remote_state[2] & KELVINATOR_LIGHT) != 0);
}

// Note: XFan mode is only valid in Cool or Dry mode.
void IRKelvinatorAC::setXFan(bool state) {
  remote_state[2] &= ~KELVINATOR_XFAN;
  remote_state[2] |= (state << KELVINATOR_XFAN_OFFSET);
  remote_state[10] = remote_state[2];  // Duplicate to the 2nd command chunk.
}

bool IRKelvinatorAC::getXFan() {
  return ((remote_state[2] & KELVINATOR_XFAN) != 0);
}

// Note: Turbo mode is turned off if the fan speed is changed.
void IRKelvinatorAC::setTurbo(bool state) {
  remote_state[2] &= ~KELVINATOR_TURBO;
  remote_state[2] |= (state << KELVINATOR_TURBO_OFFSET);
  remote_state[10] = remote_state[2];  // Duplicate to the 2nd command chunk.
}

bool IRKelvinatorAC::getTurbo() {
  return ((remote_state[2] & KELVINATOR_TURBO) != 0);
}
