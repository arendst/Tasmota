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
#define ROTARY_MAX_STEPS     10                // Rotary step boundary
#endif

// 1 pulse per step
const uint8_t rotary_dimmer_increment = 100 / ROTARY_MAX_STEPS;  // Dimmer 1..100 = 100
const uint8_t rotary_ct_increment = 350 / ROTARY_MAX_STEPS;      // Ct 153..500 = 347
const uint8_t rotary_color_increment = 360 / ROTARY_MAX_STEPS;   // Hue 0..359 = 360

const uint8_t ROTARY_TIMEOUT = 10;             // 10 * RotaryHandler() call which is usually 10 * 0.05 seconds

struct ROTARY {
  bool present = false;
} Rotary;

struct ENCODER {
  uint32_t debounce = 0;
  int8_t abs_position1 = 0;
  int8_t abs_position2 = 0;
  int8_t direction = 0;                        // Control consistent direction
  int8_t pin = -1;
  uint8_t position = 128;
  uint8_t last_position = 128;
  uint8_t timeout = 0;                         // Disallow direction change within 0.5 second
  bool changed = false;
  bool busy = false;
} Encoder[MAX_ROTARIES];

/********************************************************************************************/

void ICACHE_RAM_ATTR RotaryIsr(uint32_t index) {
  if (Encoder[index].busy) { return; }

  uint32_t time = micros();
  if (Encoder[index].debounce < time) {
    int direction = (digitalRead(Encoder[index].pin)) ? -1 : 1;
    if ((0 == Encoder[index].direction) || (direction == Encoder[index].direction)) {
      Encoder[index].position += direction;
      Encoder[index].direction = direction;
    }
    Encoder[index].debounce = time +50;        // Experimental debounce in microseconds
  }
}

void ICACHE_RAM_ATTR RotaryIsr1(void) {
  RotaryIsr(0);
}

void ICACHE_RAM_ATTR RotaryIsr2(void) {
  RotaryIsr(1);
}

bool RotaryButtonPressed(uint32_t button_index) {
  if (!Rotary.present) { return false; }

  for (uint32_t index = 0; index < MAX_ROTARIES; index++) {
    if (-1 == Encoder[index].pin) { continue; }
    if (index != button_index) { continue; }

    bool powered_on = (power);
#ifdef USE_LIGHT
    if (!Settings.flag4.rotary_uses_rules) {   // SetOption98 - Use rules instead of light control
      powered_on = LightPower();
    }
#endif  // USE_LIGHT
    if (Encoder[index].changed && powered_on) {
      Encoder[index].changed = false;          // Color (temp) changed, no need to turn of the light
      return true;
    }
    return false;
  }
  return false;
}

void RotaryInit(void) {
  Rotary.present = false;
  for (uint32_t index = 0; index < MAX_ROTARIES; index++) {
#ifdef ESP8266
    uint32_t idx = index *2;
#else  // ESP32
    uint32_t idx = index;
#endif  // ESP8266 or ESP32
    if (PinUsed(GPIO_ROT1A, idx) && PinUsed(GPIO_ROT1B, idx)) {
      Encoder[index].pin = Pin(GPIO_ROT1B, idx);
      pinMode(Encoder[index].pin, INPUT_PULLUP);
      pinMode(Pin(GPIO_ROT1A, idx), INPUT_PULLUP);
      if (0 == index) {
        attachInterrupt(Pin(GPIO_ROT1A, idx), RotaryIsr1, FALLING);
      } else {
        attachInterrupt(Pin(GPIO_ROT1A, idx), RotaryIsr2, FALLING);
      }
    }
    Rotary.present |= (Encoder[index].pin > -1);
  }
}

/*********************************************************************************************\
 * Rotary handler
\*********************************************************************************************/

void RotaryHandler(void) {
  if (!Rotary.present) { return; }

  for (uint32_t index = 0; index < MAX_ROTARIES; index++) {
    if (-1 == Encoder[index].pin) { continue; }

    if (Encoder[index].timeout) {
      Encoder[index].timeout--;
      if (!Encoder[index].timeout) {
#ifdef USE_LIGHT
        if (!Settings.flag4.rotary_uses_rules) {  // SetOption98 - Use rules instead of light control
          ResponseLightState(0);
          MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_CMND_STATE));
          XdrvRulesProcess();
        }
#endif  // USE_LIGHT
        Encoder[index].direction = 0;
      }
    }
    if (Encoder[index].last_position == Encoder[index].position) { continue; }
    Encoder[index].busy = true;

    Encoder[index].timeout = ROTARY_TIMEOUT;   // Prevent fast direction changes within 0.5 second

    int rotary_position = Encoder[index].position - Encoder[index].last_position;

    if (Settings.save_data && (save_data_counter < 2)) {
      save_data_counter = 2;                   // Postpone flash writes while rotary is turned
    }

    bool button_pressed = (Button.hold_timer[index]);  // Button is pressed: set color temperature
    if (button_pressed) { Encoder[index].changed = true; }
//    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ROT: Button1 %d, Position %d"), button_pressed, rotary_position);

#ifdef USE_LIGHT
    if (!Settings.flag4.rotary_uses_rules) {   // SetOption98 - Use rules instead of light control
      bool second_rotary = (Encoder[1].pin > -1);
      if (0 == index) {                        // Rotary1
        if (button_pressed) {
          if (second_rotary) {                 // Color RGB
            LightColorOffset(rotary_position * rotary_color_increment);
          } else {                             // Color Temperature or Color RGB
            if (!LightColorTempOffset(rotary_position * rotary_ct_increment)) {
              LightColorOffset(rotary_position * rotary_color_increment);
            }
          }
        } else {                               // Dimmer RGBCW or RGB only if second rotary
          LightDimmerOffset(second_rotary ? 1 : 0, rotary_position * rotary_dimmer_increment);
        }
      } else {                                 // Rotary2
        if (button_pressed) {                  // Color Temperature
          LightColorTempOffset(rotary_position * rotary_ct_increment);
        } else {                               // Dimmer CW
          LightDimmerOffset(2, rotary_position * rotary_dimmer_increment);
        }
      }
    } else {
#endif  // USE_LIGHT
      if (button_pressed) {
        Encoder[index].abs_position2 += rotary_position;
        if (Encoder[index].abs_position2 < 0) { Encoder[index].abs_position2 = 0; }
        if (Encoder[index].abs_position2 > ROTARY_MAX_STEPS) { Encoder[index].abs_position2 = ROTARY_MAX_STEPS; }
      } else {
        Encoder[index].abs_position1 += rotary_position;
        if (Encoder[index].abs_position1 < 0) { Encoder[index].abs_position1 = 0; }
        if (Encoder[index].abs_position1 > ROTARY_MAX_STEPS) { Encoder[index].abs_position1 = ROTARY_MAX_STEPS; }
      }
      Response_P(PSTR("{\"Rotary%d\":{\"Pos1\":%d,\"Pos2\":%d}}"), index +1, Encoder[index].abs_position1, Encoder[index].abs_position2);
      XdrvRulesProcess();
#ifdef USE_LIGHT
    }
#endif  // USE_LIGHT

    Encoder[index].last_position = 128;
    Encoder[index].position = 128;
    Encoder[index].busy = false;
  }
}

#endif  // ROTARY_V1
