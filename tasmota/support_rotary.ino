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
//#define ROTARY_V1
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
  uint8_t interrupts_in_use_count = 0;
  uint8_t changed = 0;
} Rotary;

/********************************************************************************************/

#ifndef ARDUINO_ESP8266_RELEASE_2_3_0      // Fix core 2.5.x ISR not in IRAM Exception
void update_rotary(void) ICACHE_RAM_ATTR;
#endif  // ARDUINO_ESP8266_RELEASE_2_3_0

void update_rotary(void)
{
  if (MI_DESK_LAMP == my_module_type) {
    if (LightPowerIRAM()) {
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
  }
}

bool RotaryButtonPressed(void)
{
  if ((MI_DESK_LAMP == my_module_type) && (Rotary.changed) && LightPower()) {
    Rotary.changed = 0;                    // Color temp changed, no need to turn of the light
    return true;
  }
  return false;
}

void RotaryInit(void)
{
  Rotary.present = 0;
  if ((Pin(GPIO_ROT1A) < 99) && (Pin(GPIO_ROT1B) < 99)) {
    Rotary.present++;
    pinMode(Pin(GPIO_ROT1A), INPUT_PULLUP);
    pinMode(Pin(GPIO_ROT1B), INPUT_PULLUP);

    // GPIO6-GPIO11 are typically used to interface with the flash memory IC on
    // most esp8266 modules, so we should avoid adding interrupts to these pins.

    if ((Pin(GPIO_ROT1A) < 6) || (Pin(GPIO_ROT1A) > 11)) {
      attachInterrupt(digitalPinToInterrupt(Pin(GPIO_ROT1A)), update_rotary, CHANGE);
      Rotary.interrupts_in_use_count++;
    }
    if ((Pin(GPIO_ROT1B) < 6) || (Pin(GPIO_ROT1B) > 11)) {
      attachInterrupt(digitalPinToInterrupt(Pin(GPIO_ROT1B)), update_rotary, CHANGE);
      Rotary.interrupts_in_use_count++;
    }
  }
}

/*********************************************************************************************\
 * Rotary handler
\*********************************************************************************************/

void RotaryHandler(void)
{
  if (Rotary.interrupts_in_use_count < 2) {
    noInterrupts();
    update_rotary();
  } else {
    noInterrupts();
  }
  if (Rotary.last_position != Rotary.position) {
    if (MI_DESK_LAMP == my_module_type) { // Mi Desk lamp
      if (Button.hold_timer[0]) {
        Rotary.changed = 1;
        // button1 is pressed: set color temperature
        int16_t t = LightGetColorTemp();
        t = t + (Rotary.position - Rotary.last_position);
        if (t < 153) {
          t = 153;
        }
        if (t > 500) {
          t = 500;
        }
        DEBUG_CORE_LOG(PSTR("ROT: " D_CMND_COLORTEMPERATURE " %d"), Rotary.position - Rotary.last_position);
        LightSetColorTemp((uint16_t)t);
      } else {
        int8_t d = Settings.light_dimmer;
        d = d + (Rotary.position - Rotary.last_position);
        if (d < 1) {
          d = 1;
        }
        if (d > 100) {
          d = 100;
        }
        DEBUG_CORE_LOG(PSTR("ROT: " D_CMND_DIMMER " %d"), Rotary.position - Rotary.last_position);

        LightSetDimmer((uint8_t)d);
        Settings.light_dimmer = d;
      }
    }
    Rotary.last_position = 128;
    Rotary.position = 128;
  }
  interrupts();
}

void RotaryLoop(void)
{
  if (Rotary.present) {
    if (TimeReached(Rotary.debounce)) {
      SetNextTimeInterval(Rotary.debounce, Settings.button_debounce); // Using button_debounce setting for this as well
      RotaryHandler();
    }
  }
}

#endif  // ROTARY_V1
#endif  // USE_LIGHT
