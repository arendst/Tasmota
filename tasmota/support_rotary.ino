/*
  support_rotary.ino - rotary switch support for Tasmota

  Copyright (C) 2020  Theo Arends

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

#ifdef ROTARY_V1
/*********************************************************************************************\
 * Rotary support
 *
 * Supports full range in 10 steps of the Rotary Encoder:
 * - Light Dimmer
 * - Light Color for RGB lights when Button1 pressed
 * - Light Color Temperature for CW lights when Button1 pressed
 *
 *                                _______         _______
 *              GPIO_ROT1A ______|       |_______|       |______ GPIO_ROT1A
 * negative <--               _______         _______         __            --> positive
 *              GPIO_ROT1B __|       |_______|       |_______|   GPIO_ROT1B
 *
\*********************************************************************************************/

#ifndef ROTARY_MAX_STEPS
#define ROTARY_MAX_STEPS     10              // Rotary step boundary
#endif

//#define ROTARY_OPTION1                       // Up to 4 interrupts and pulses per step
//#define ROTARY_OPTION2                       // Up to 4 interrupts but 1 pulse per step
#define ROTARY_OPTION3                       // 1 interrupt and pulse per step

#ifdef ROTARY_OPTION1
// up to 4 pulses per step
const uint8_t rotary_dimmer_increment = 100 / (ROTARY_MAX_STEPS * 3);  // Dimmer 1..100 = 100
const uint8_t rotary_ct_increment = 350 / (ROTARY_MAX_STEPS * 3);      // Ct 153..500 = 347
const uint8_t rotary_color_increment = 360 / (ROTARY_MAX_STEPS * 3);   // Hue 0..359 = 360
#endif  // ROTARY_OPTION1

#ifdef ROTARY_OPTION2
// 1 pulse per step
const uint8_t rotary_dimmer_increment = 100 / ROTARY_MAX_STEPS;        // Dimmer 1..100 = 100
const uint8_t rotary_ct_increment = 350 / ROTARY_MAX_STEPS;            // Ct 153..500 = 347
const uint8_t rotary_color_increment = 360 / ROTARY_MAX_STEPS;         // Hue 0..359 = 360
#endif  // ROTARY_OPTION2

#ifdef ROTARY_OPTION3
// 1 pulse per step
const uint8_t rotary_dimmer_increment = 100 / ROTARY_MAX_STEPS;        // Dimmer 1..100 = 100
const uint8_t rotary_ct_increment = 350 / ROTARY_MAX_STEPS;            // Ct 153..500 = 347
const uint8_t rotary_color_increment = 360 / ROTARY_MAX_STEPS;         // Hue 0..359 = 360
#endif  // ROTARY_OPTION3

const uint8_t ROTARY_TIMEOUT = 10;           // 10 * RotaryHandler() call which is usually 10 * 0.05 seconds

struct ROTARY {
#ifdef ROTARY_OPTION1
  uint8_t state = 0;
#endif  // ROTARY_OPTION1
#ifdef ROTARY_OPTION2
  uint16_t store;
  uint8_t prev_next_code;
#endif  // ROTARY_OPTION2
#ifdef ROTARY_OPTION3
  uint32_t debounce = 0;
#endif  // ROTARY_OPTION3
  int8_t abs_position1 = 0;
  int8_t abs_position2 = 0;
  int8_t direction = 0;                      // Control consistent direction
  uint8_t present = 0;
  uint8_t position = 128;
  uint8_t last_position = 128;
  uint8_t timeout = 0;                       // Disallow direction change within 0.5 second
  bool changed = false;
  bool busy = false;
} Rotary;

/********************************************************************************************/

void update_rotary(void) ICACHE_RAM_ATTR;
void update_rotary(void) {
  if (Rotary.busy) { return; }
  bool powered_on = (power);
#ifdef USE_LIGHT
  if (!Settings.flag4.rotary_uses_rules) {   // SetOption98 - Use rules instead of light control
    powered_on = (LightPowerIRAM());
  }
#endif  // USE_LIGHT
  if (!powered_on) { return; }

#ifdef ROTARY_OPTION1
  // https://github.com/PaulStoffregen/Encoder/blob/master/Encoder.h
/*
  uint8_t p1val = digitalRead(Pin(GPIO_ROT1A));
  uint8_t p2val = digitalRead(Pin(GPIO_ROT1B));
  uint8_t state = Rotary.state & 3;
  if (p1val) { state |= 4; }
  if (p2val) { state |= 8; }
  Rotary.state = (state >> 2);
  switch (state) {
    case 1: case 7: case 8: case 14:
      Rotary.position++;
      return;
    case 2: case 4: case 11: case 13:
      Rotary.position--;
      return;
    case 3: case 12:
      Rotary.position += 2;
      return;
    case 6: case 9:
      Rotary.position -= 2;
      return;
  }
*/
  uint8_t p1val = digitalRead(Pin(GPIO_ROT1A));
  uint8_t p2val = digitalRead(Pin(GPIO_ROT1B));
  uint8_t state = Rotary.state & 3;
  if (p1val) { state |= 4; }
  if (p2val) { state |= 8; }
  Rotary.state = (state >> 2);
  int direction = 0;
  int multiply = 1;
  switch (state) {
    case 3: case 12:
      multiply = 2;
    case 1: case 7: case 8: case 14:
      direction = 1;
      break;
    case 6: case 9:
      multiply = 2;
    case 2: case 4: case 11: case 13:
      direction = -1;
      break;
  }
  if ((0 == Rotary.direction) || (direction == Rotary.direction)) {
    Rotary.position += (direction * multiply);
    Rotary.direction = direction;
  }
#endif  // ROTARY_OPTION1

#ifdef ROTARY_OPTION2
  // https://github.com/FrankBoesing/EncoderBounce/blob/master/EncoderBounce.h
/*
  const uint16_t rot_enc = 0b0110100110010110;

  uint8_t p1val = digitalRead(Pin(GPIO_ROT1B));
  uint8_t p2val = digitalRead(Pin(GPIO_ROT1A));
  uint8_t t = Rotary.prev_next_code;
  t <<= 2;
  if (p1val) { t |= 0x02; }
  if (p2val) { t |= 0x01; }
  t &= 0x0f;
  Rotary.prev_next_code = t;

  // If valid then store as 16 bit data.
  if (rot_enc & (1 << t)) {
    Rotary.store = (Rotary.store << 4) | Rotary.prev_next_code;
    if (Rotary.store == 0xd42b) { Rotary.position++; }
    else if (Rotary.store == 0xe817) { Rotary.position--; }
    else if ((Rotary.store & 0xff) == 0x2b) { Rotary.position--; }
    else if ((Rotary.store & 0xff) == 0x17) { Rotary.position++; }
  }
*/
  const uint16_t rot_enc = 0b0110100110010110;

  uint8_t p1val = digitalRead(Pin(GPIO_ROT1B));
  uint8_t p2val = digitalRead(Pin(GPIO_ROT1A));
  uint8_t t = Rotary.prev_next_code;
  t <<= 2;
  if (p1val) { t |= 0x02; }
  if (p2val) { t |= 0x01; }
  t &= 0x0f;
  Rotary.prev_next_code = t;

  // If valid then store as 16 bit data.
  if (rot_enc & (1 << t)) {
    Rotary.store = (Rotary.store << 4) | Rotary.prev_next_code;
    int direction = 0;
    if (Rotary.store == 0xd42b) { direction = 1; }
    else if (Rotary.store == 0xe817) { direction = -1; }
    else if ((Rotary.store & 0xff) == 0x2b) { direction = -1; }
    else if ((Rotary.store & 0xff) == 0x17) { direction = 1; }
    if ((0 == Rotary.direction) || (direction == Rotary.direction)) {
      Rotary.position += direction;
      Rotary.direction = direction;
    }
  }
#endif  // ROTARY_OPTION2

#ifdef ROTARY_OPTION3
  // Theo Arends
  uint32_t time = micros();
  if (Rotary.debounce < time) {
    int direction = (digitalRead(Pin(GPIO_ROT1B))) ? 1 : -1;
    if ((0 == Rotary.direction) || (direction == Rotary.direction)) {
      Rotary.position += direction;
      Rotary.direction = direction;
    }
    Rotary.debounce = time +20;              // Experimental debounce
  }
#endif  // ROTARY_OPTION3
}

bool RotaryButtonPressed(void) {
  bool powered_on = (power);
#ifdef USE_LIGHT
  if (!Settings.flag4.rotary_uses_rules) {   // SetOption98 - Use rules instead of light control
    powered_on = LightPower();
  }
#endif  // USE_LIGHT
  if (Rotary.changed && powered_on) {
    Rotary.changed = false;                  // Color (temp) changed, no need to turn of the light
    return true;
  }
  return false;
}

void RotaryInit(void) {
  Rotary.present = 0;
  if (PinUsed(GPIO_ROT1A) && PinUsed(GPIO_ROT1B)) {
    Rotary.present++;
    pinMode(Pin(GPIO_ROT1A), INPUT_PULLUP);
    pinMode(Pin(GPIO_ROT1B), INPUT_PULLUP);
#ifdef ROTARY_OPTION3
    attachInterrupt(Pin(GPIO_ROT1A), update_rotary, RISING);
#else
    attachInterrupt(Pin(GPIO_ROT1A), update_rotary, CHANGE);
    attachInterrupt(Pin(GPIO_ROT1B), update_rotary, CHANGE);
#endif
  }
}

/*********************************************************************************************\
 * Rotary handler
\*********************************************************************************************/

void RotaryHandler(void) {
  if (Rotary.timeout) {
    Rotary.timeout--;
    if (!Rotary.timeout) {
      Rotary.direction = 0;
    }
  }
  if (Rotary.last_position == Rotary.position) { return; }
  Rotary.busy = true;

  Rotary.timeout = ROTARY_TIMEOUT;           // Prevent fast direction changes within 0.5 second

  int rotary_position = Rotary.position - Rotary.last_position;

  if (Settings.save_data && (save_data_counter < 2)) {
    save_data_counter = 2;                   // Postpone flash writes while rotary is turned
  }

  bool button_pressed = (Button.hold_timer[0]);  // Button1 is pressed: set color temperature
  if (button_pressed) { Rotary.changed = true; }
//  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ROT: Button1 %d, Position %d"), button_pressed, rotary_position);

#ifdef USE_LIGHT
  if (!Settings.flag4.rotary_uses_rules) {   // SetOption98 - Use rules instead of light control
    if (button_pressed) {
      if (!LightColorTempOffset(rotary_position * rotary_ct_increment)) {
        LightColorOffset(rotary_position * rotary_color_increment);
      }
    } else {
      LightDimmerOffset(rotary_position * rotary_dimmer_increment);
    }
  } else {
#endif  // USE_LIGHT
    if (button_pressed) {
      Rotary.abs_position2 += rotary_position;
      if (Rotary.abs_position2 < 0) { Rotary.abs_position2 = 0; }
      if (Rotary.abs_position2 > ROTARY_MAX_STEPS) { Rotary.abs_position2 = ROTARY_MAX_STEPS; }
    } else {
      Rotary.abs_position1 += rotary_position;
      if (Rotary.abs_position1 < 0) { Rotary.abs_position1 = 0; }
      if (Rotary.abs_position1 > ROTARY_MAX_STEPS) { Rotary.abs_position1 = ROTARY_MAX_STEPS; }
    }
    Response_P(PSTR("{\"Rotary1\":{\"Pos1\":%d,\"Pos2\":%d}}"), Rotary.abs_position1, Rotary.abs_position2);
    XdrvRulesProcess();
#ifdef USE_LIGHT
  }
#endif  // USE_LIGHT

  Rotary.last_position = 128;
  Rotary.position = 128;
  Rotary.busy = false;
}

#endif  // ROTARY_V1
