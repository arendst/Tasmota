/*
  support_rotary.ino - rotary switch support for Sonoff-Tasmota

  Copyright (C) 2019  Theo Arends

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

#define ROTARY_V1
#ifdef ROTARY_V1
/*********************************************************************************************\
 * Rotary support
\*********************************************************************************************/

unsigned long rotary_debounce = 0;          // Rotary debounce timer
uint8_t rotaries_found = 0;
uint8_t rotary_state = 0;
uint8_t rotary_position = 128;
uint8_t rotary_last_position = 128;
uint8_t interrupts_in_use = 0;
uint8_t rotary_changed = 0;

/********************************************************************************************/

void update_position(void)
{
  uint8_t s;

  /*
   * https://github.com/PaulStoffregen/Encoder/blob/master/Encoder.h
   */

  s = rotary_state & 3;
  if (digitalRead(pin[GPIO_ROT1A])) s |= 4;
  if (digitalRead(pin[GPIO_ROT1B])) s |= 8;
  switch (s) {
    case 0: case 5: case 10: case 15:
      break;
    case 1: case 7: case 8: case 14:
      rotary_position++; break;
    case 2: case 4: case 11: case 13:
      rotary_position--; break;
    case 3: case 12:
      rotary_position = rotary_position + 2; break;
    default:
      rotary_position = rotary_position - 2; break;
  }
  rotary_state = (s >> 2);
}

void update_rotary(void)
{
  if (MI_DESK_LAMP == my_module_type){
    if (light_power) {
      update_position();
    }
  }
}

void RotaryInit(void)
{
  rotaries_found = 0;
  if ((pin[GPIO_ROT1A] < 99) && (pin[GPIO_ROT1B] < 99)) {
    rotaries_found++;
    pinMode(pin[GPIO_ROT1A], INPUT_PULLUP);
    pinMode(pin[GPIO_ROT1B], INPUT_PULLUP);

    // GPIO6-GPIO11 are typically used to interface with the flash memory IC on
    // most esp8266 modules, so we should avoid adding interrupts to these pins.

    if ((pin[GPIO_ROT1A] < 6) || (pin[GPIO_ROT1A] > 11)) {
      attachInterrupt(digitalPinToInterrupt(pin[GPIO_ROT1A]), update_rotary, CHANGE);
      interrupts_in_use++;
    }
    if ((pin[GPIO_ROT1B] < 6) || (pin[GPIO_ROT1B] > 11)) {
      attachInterrupt(digitalPinToInterrupt(pin[GPIO_ROT1B]), update_rotary, CHANGE);
      interrupts_in_use++;
    }
  }
}

/*********************************************************************************************\
 * Rotary handler
\*********************************************************************************************/

void RotaryHandler(void)
{
  if (interrupts_in_use < 2) {
    noInterrupts();
    update_rotary();
  } else {
    noInterrupts();
  }
  if (rotary_last_position != rotary_position) {
    if (MI_DESK_LAMP == my_module_type) { // Mi Desk lamp
      if (holdbutton[0]) {
        rotary_changed = 1;
        // button1 is pressed: set color temperature
        int16_t t = LightGetColorTemp();
        t = t + (rotary_position - rotary_last_position);
        if (t < 153) {
          t = 153;
        }
        if (t > 500) {
          t = 500;
        }
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_CMND_COLORTEMPERATURE " %d"), rotary_position - rotary_last_position);
        LightSetColorTemp((uint16_t)t);
      } else {
        int8_t d = Settings.light_dimmer;
        d = d + (rotary_position - rotary_last_position);
        if (d < 1) {
          d = 1;
        }
        if (d > 100) {
          d = 100;
        }
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_CMND_DIMMER " %d"), rotary_position - rotary_last_position);

        LightSetDimmer((uint8_t)d);
        Settings.light_dimmer = d;
      }
    }
    rotary_last_position = 128;
    rotary_position = 128;
  }
  interrupts();
}

void RotaryLoop(void)
{
  if (rotaries_found) {
    if (TimeReached(rotary_debounce)) {
      SetNextTimeInterval(rotary_debounce, Settings.button_debounce); // Using button_debounce setting for this as well
      RotaryHandler();
    }
  }
}

#endif  // ROTARY_V1
