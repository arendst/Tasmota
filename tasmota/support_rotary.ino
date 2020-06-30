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

struct ROTARY {
  unsigned long debounce = 0;          // Rotary debounce timer
  uint8_t present = 0;
  uint8_t state = 0;
  uint8_t position = 128;
  uint8_t last_position = 128;
  uint8_t changed = 0;
  bool busy = false;
} Rotary;

/********************************************************************************************/

void update_rotary(void) ICACHE_RAM_ATTR;
void update_rotary(void) {
  if (Rotary.busy || !LightPowerIRAM()) { return; }

  /*
  * https://github.com/PaulStoffregen/Encoder/blob/master/Encoder.h
  */
  uint8_t s = Rotary.state & 3;
  if (digitalRead(Pin(GPIO_ROT1A))) { s |= 4; }
  if (digitalRead(Pin(GPIO_ROT1B))) { s |= 8; }
  switch (s) {
    case 0: case 5: case 10: case 15:
      break;
    case 1: case 7: case 8: case 14:
      Rotary.position++; break;
    case 2: case 4: case 11: case 13:
      Rotary.position--; break;
    case 3: case 12:
      Rotary.position = Rotary.position + 2; break;
    default:
      Rotary.position = Rotary.position - 2; break;
  }
  Rotary.state = (s >> 2);
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
  if (Rotary.last_position != Rotary.position) {
    Rotary.busy = true;
    int rotary_position = Rotary.position - Rotary.last_position;

    if (Settings.save_data) {
      if (save_data_counter < 2) {
        save_data_counter = 2;   // Postpone flash writes while rotary is turned
      }
    }

    if (Button.hold_timer[0]) {  // Button1 is pressed: set color temperature
//      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ROT: " D_CMND_COLORTEMPERATURE " %d"), rotary_position);
      Rotary.changed = 1;
      if (!LightColorTempOffset(rotary_position * 4)) {  // Ct from 153 - 500
        LightColorOffset(rotary_position * 4);           // Hue from 0 - 359
      }
    } else {
//      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ROT: " D_CMND_DIMMER " %d"), rotary_position);
      LightDimmerOffset(rotary_position);
    }

    Rotary.last_position = 128;
    Rotary.position = 128;
    Rotary.busy = false;
  }
}

void RotaryLoop(void) {
  if (Rotary.present) {
    if (TimeReached(Rotary.debounce)) {
      SetNextTimeInterval(Rotary.debounce, Settings.button_debounce);  // Using button_debounce setting for this as well
      RotaryHandler();
    }
  }
}

#endif  // ROTARY_V1
#endif  // USE_LIGHT
