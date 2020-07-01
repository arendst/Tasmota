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

#ifdef USE_LIGHT
#ifdef ROTARY_V1
/*********************************************************************************************\
 * Rotary support
\*********************************************************************************************/

#define ROTARY_OPTION1
//#define ROTARY_OPTION2

#ifdef ROTARY_OPTION1
const uint8_t rotary_dimmer_increment = 1;
const uint8_t rotary_ct_increment = 2;
const uint8_t rotary_color_increment = 4;
#endif

#ifdef ROTARY_OPTION2
const uint8_t rotary_dimmer_increment = 2;
const uint8_t rotary_ct_increment = 8;
const uint8_t rotary_color_increment = 8;
#endif

struct ROTARY {
  uint8_t present = 0;
  uint8_t state = 0;
  uint8_t prevNextCode;
  uint16_t store;
  int8_t position = 128;
  int8_t last_position = 128;
  uint8_t changed = 0;
  bool busy = false;
} Rotary;

/********************************************************************************************/

void update_rotary(void) ICACHE_RAM_ATTR;
void update_rotary(void) {
  if (Rotary.busy || !LightPowerIRAM()) { return; }

#ifdef ROTARY_OPTION1
  // https://github.com/PaulStoffregen/Encoder/blob/master/Encoder.h
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
#endif

#ifdef ROTARY_OPTION2
  // https://github.com/FrankBoesing/EncoderBounce/blob/master/EncoderBounce.h
  const uint16_t rot_enc = 0b0110100110010110;

  uint8_t p1val = digitalRead(Pin(GPIO_ROT1B));
  uint8_t p2val = digitalRead(Pin(GPIO_ROT1A));
  uint8_t t = Rotary.prevNextCode;
  t <<= 2;
  if (p1val) { t |= 0x02; }
  if (p2val) { t |= 0x01; }
  t &= 0x0f;
  Rotary.prevNextCode = t;

  // If valid then store as 16 bit data.
  if (rot_enc & (1 << t)) {
    Rotary.store = (Rotary.store << 4) | Rotary.prevNextCode;
    if (Rotary.store == 0xd42b) { Rotary.position++; }
    else if (Rotary.store == 0xe817) { Rotary.position--; }
    else if ((Rotary.store & 0xff) == 0x2b) { Rotary.position--; }
    else if ((Rotary.store & 0xff) == 0x17) { Rotary.position++; }
  }
#endif
}

bool RotaryButtonPressed(void) {
  if (Rotary.changed && LightPower()) {
    Rotary.changed = 0;                    // Color temp changed, no need to turn of the light
    return true;
  }
  return false;
}

void RotaryInit(void) {
  Rotary.present = 0;
  if (PinUsed(GPIO_ROT1A) && PinUsed(GPIO_ROT1B)) {
    Rotary.present++;
    pinMode(Pin(GPIO_ROT1A), INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(Pin(GPIO_ROT1A)), update_rotary, CHANGE);
    pinMode(Pin(GPIO_ROT1B), INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(Pin(GPIO_ROT1B)), update_rotary, CHANGE);
  }
}

/*********************************************************************************************\
 * Rotary handler
\*********************************************************************************************/

void RotaryHandler(void) {
  if (Rotary.last_position == Rotary.position) { return; }

  Rotary.busy = true;
  int rotary_position = Rotary.position - Rotary.last_position;
  Rotary.last_position = Rotary.position;

  if (Settings.save_data && (save_data_counter < 2)) {
    save_data_counter = 2;     // Postpone flash writes while rotary is turned
  }

  if (Button.hold_timer[0]) {  // Button1 is pressed: set color temperature
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ROT: CT/Color position %d"), rotary_position);
    Rotary.changed = 1;
    if (!LightColorTempOffset(rotary_position * rotary_ct_increment)) {  // Ct 153..500 = (500 - 153) / 8 = 43 steps
      LightColorOffset(rotary_position * rotary_color_increment);        // Hue 0..359 = 360 / 8 = 45 steps
    }
  } else {
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ROT: Dimmer position %d"), rotary_position);
    LightDimmerOffset(rotary_position * rotary_dimmer_increment);        // Dimmer 1..100 = 100 / 2 = 50 steps
  }

//    Rotary.last_position = 128;
//    Rotary.position = 128;

  Rotary.busy = false;
}

#endif  // ROTARY_V1
#endif  // USE_LIGHT
