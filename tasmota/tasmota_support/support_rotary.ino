/*
  support_rotary.ino - rotary switch support for Tasmota

  Copyright (C) 2021  Theo Arends

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
#ifndef ROTARY_START_DIM
#define ROTARY_START_DIM     1                 // Minimal dimmer value after power on with SetOption113 1
#endif
#ifndef ROTARY_TIMEOUT
#define ROTARY_TIMEOUT       2                 // 2 * RotaryHandler() call which is usually 2 * 0.05 seconds
#endif
#ifndef ROTARY_DEBOUNCE
#define ROTARY_DEBOUNCE      10                // Debounce time in milliseconds
#endif

const uint8_t rotary_offset = 128;
#ifdef ESP8266
const int8_t rotary_state_pos[16] = { 0, 1, -1, 2, -1, 0, -2, 1, 1, -2, 0, -1, 2, -1, 1, 0 };
#endif  // ESP8266

struct ROTARY {
  uint8_t no_pullup_mask_a = 0;                // Rotary A pull-up bitmask flags
  uint8_t no_pullup_mask_b = 0;                // Rotary B pull-up bitmask flags
  uint8_t dimmer_increment;
  uint8_t ct_increment;
  uint8_t color_increment;
  uint8_t model;
  bool present;
} Rotary;

struct tEncoder {
  volatile uint32_t debounce = 0;
  volatile uint8_t state = 0;
  volatile uint8_t position;
  volatile int8_t direction = 0;               // Control consistent direction
  volatile int8_t pina;
  volatile int8_t pinb;
  uint8_t timeout = 0;                         // Disallow direction change within 0.5 second
  int8_t abs_position[2] = { 0 };
  int8_t rel_position = 0;                     // Relative position for scripter. Cleared after being read.
  bool changed = false;
};
tEncoder Encoder[MAX_ROTARIES];

/********************************************************************************************/

void RotaryAPullupFlag(uint32 switch_bit) {
  bitSet(Rotary.no_pullup_mask_a, switch_bit);
}

void RotaryBPullupFlag(uint32 switch_bit) {
  bitSet(Rotary.no_pullup_mask_b, switch_bit);
}

bool RotaryButtonPressed(uint32_t button_index) {
  if (!Rotary.present) { return false; }

  for (uint32_t index = 0; index < MAX_ROTARIES; index++) {
    if (-1 == Encoder[index].pinb) { continue; }
    if (index != button_index) { continue; }

    bool powered_on = (TasmotaGlobal.power);
#ifdef USE_LIGHT
    if (!Settings->flag4.rotary_uses_rules) {   // SetOption98 - Use rules instead of light control
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

static void IRAM_ATTR RotaryIsrArgMiDesk(void *arg) {
  tEncoder* encoder = static_cast<tEncoder*>(arg);

  // https://github.com/PaulStoffregen/Encoder/blob/master/Encoder.h
  uint32_t state = encoder->state & 3;
  if (digitalRead(encoder->pina)) { state |= 4; }
  if (digitalRead(encoder->pinb)) { state |= 8; }

#ifdef ESP32
//  This fails intermittendly with panic (Cache disabled but cached memory region accessed) if not in DRAM
//  https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/memory-types.html
  const static DRAM_ATTR int8_t rotary_state_pos[16] = { 0, 1, -1, 2, -1, 0, -2, 1, 1, -2, 0, -1, 2, -1, 1, 0 };
#endif  // ESP32

  encoder->position += rotary_state_pos[state];
  encoder->state = (state >> 2);
}

static void IRAM_ATTR RotaryIsrArg(void *arg) {
  tEncoder* encoder = static_cast<tEncoder*>(arg);

  // Theo Arends
  uint32_t time = millis();
  if ((encoder->debounce < time) || (encoder->debounce > time + ROTARY_DEBOUNCE)) {
    int direction = (digitalRead(encoder->pinb)) ? -1 : 1;
    if ((0 == encoder->direction) || (direction == encoder->direction)) {
      encoder->position += direction;
      encoder->direction = direction;
    }
    encoder->debounce = time + ROTARY_DEBOUNCE;  // Experimental debounce
  }
}

void RotaryInitMaxSteps(void) {
  if (0 == Settings->param[P_ROTARY_MAX_STEP]) {
    Settings->param[P_ROTARY_MAX_STEP] = ROTARY_MAX_STEPS;  // SetOption43
  }
  uint8_t max_steps = Settings->param[P_ROTARY_MAX_STEP];
  if (!Rotary.model) { max_steps *= 3; }
  Rotary.dimmer_increment = 100 / min((uint8_t)100, max_steps);  // Dimmer 1..100 = 100
  Rotary.ct_increment =     350 / min((uint8_t)350, max_steps);  // Ct 153..500 = 347
  Rotary.color_increment =  360 / min((uint8_t)360, max_steps);  // Hue 0..359 = 360
}

void RotaryInit(void) {
  Rotary.present = false;

  Rotary.model = !TasmotaGlobal.gpio_optiona.rotary_mi_desk;  // Option_A5
#ifdef ESP8266
  if (MI_DESK_LAMP == TasmotaGlobal.module_type) {
    Rotary.model = 0;
  }
#endif  // ESP8266

  AddLog(LOG_LEVEL_DEBUG, PSTR("ROT: Mode %d"), Rotary.model);

  RotaryInitMaxSteps();

  for (uint32_t index = 0; index < MAX_ROTARIES; index++) {
    Encoder[index].pinb = -1;
    if (PinUsed(GPIO_ROT1A, index) && PinUsed(GPIO_ROT1B, index)) {
      Encoder[index].position = rotary_offset;
      Encoder[index].pina = Pin(GPIO_ROT1A, index);
      Encoder[index].pinb = Pin(GPIO_ROT1B, index);
      pinMode(Encoder[index].pina, bitRead(Rotary.no_pullup_mask_a, index) ? INPUT : INPUT_PULLUP);
      pinMode(Encoder[index].pinb, bitRead(Rotary.no_pullup_mask_b, index) ? INPUT : INPUT_PULLUP);
      if (0 == Rotary.model) {
        attachInterruptArg(Encoder[index].pina, RotaryIsrArgMiDesk, &Encoder[index], CHANGE);
        attachInterruptArg(Encoder[index].pinb, RotaryIsrArgMiDesk, &Encoder[index], CHANGE);
      } else {
        attachInterruptArg(Encoder[index].pina, RotaryIsrArg, &Encoder[index], FALLING);
      }
    }
    Rotary.present |= (Encoder[index].pinb >= 0);
  }
}

/*********************************************************************************************\
 * Rotary handler
\*********************************************************************************************/

void RotaryHandler(void) {
  if (!Rotary.present) { return; }

  for (uint32_t index = 0; index < MAX_ROTARIES; index++) {
    if (-1 == Encoder[index].pinb) { continue; }

    if (Encoder[index].timeout) {
      Encoder[index].timeout--;
      if (!Encoder[index].timeout) {
#ifdef USE_LIGHT
        if (!Settings->flag4.rotary_uses_rules) {  // SetOption98 - Use rules instead of light control
          ResponseLightState(0);
          MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR(D_CMND_STATE));
        }
#endif  // USE_LIGHT
        Encoder[index].direction = 0;
      }
    }
    if (rotary_offset == Encoder[index].position) { continue; }

    Encoder[index].timeout = ROTARY_TIMEOUT;   // Prevent fast direction changes within 0.5 second

    noInterrupts();
    int rotary_position = Encoder[index].position - rotary_offset;
    Encoder[index].position = rotary_offset;
    interrupts();

    if (Settings->save_data && (TasmotaGlobal.save_data_counter < 2)) {
      TasmotaGlobal.save_data_counter = 3;                   // Postpone flash writes while rotary is turned
    }

    bool button_pressed = (Button.hold_timer[index]);  // Button is pressed: set color temperature
    if (button_pressed) { Encoder[index].changed = true; }
//    AddLog(LOG_LEVEL_DEBUG, PSTR("ROT: Button1 %d, Position %d"), button_pressed, rotary_position);

#ifdef USE_LIGHT
    if (!Settings->flag4.rotary_uses_rules) {   // SetOption98 - Use rules instead of light control
      bool second_rotary = (Encoder[1].pinb >= 0);
      if (0 == index) {                        // Rotary1
        if (button_pressed) {
          if (second_rotary) {                 // Color RGB
            LightColorOffset(rotary_position * Rotary.color_increment);
          } else {                             // Color Temperature or Color RGB
            if (!LightColorTempOffset(rotary_position * Rotary.ct_increment)) {
              LightColorOffset(rotary_position * Rotary.color_increment);
            }
          }
        } else {                               // Dimmer RGBCW or RGB only if second rotary
          uint32_t dimmer_index = second_rotary ? 1 : 0;
          if (!Settings->flag4.rotary_poweron_dimlow || TasmotaGlobal.power) {  // SetOption113 - On rotary dial after power off set dimmer low
            LightDimmerOffset(dimmer_index, rotary_position * Rotary.dimmer_increment);
          } else {
            if (rotary_position > 0) {         // Only power on if rotary increase
              LightDimmerOffset(dimmer_index, -LightGetDimmer(dimmer_index) + ROTARY_START_DIM);
            }
          }
        }
      } else {                                 // Rotary2
        if (button_pressed) {                  // Color Temperature
          LightColorTempOffset(rotary_position * Rotary.ct_increment);
        } else {                               // Dimmer CW
          LightDimmerOffset(2, rotary_position * Rotary.dimmer_increment);
        }
      }
    } else {
#endif  // USE_LIGHT
      Encoder[index].abs_position[button_pressed] += rotary_position;
      if (Encoder[index].abs_position[button_pressed] < 0) {
        Encoder[index].abs_position[button_pressed] = 0;
      }
      if (Encoder[index].abs_position[button_pressed] > Settings->param[P_ROTARY_MAX_STEP]) {  // SetOption43 - Rotary steps
        Encoder[index].abs_position[button_pressed] = Settings->param[P_ROTARY_MAX_STEP];      // SetOption43 - Rotary steps
      }
      Encoder[index].rel_position += rotary_position;
      if (Encoder[index].rel_position > Settings->param[P_ROTARY_MAX_STEP]) {
        Encoder[index].rel_position = Settings->param[P_ROTARY_MAX_STEP];
      }
      if (Encoder[index].rel_position < -(Settings->param[P_ROTARY_MAX_STEP])) {
        Encoder[index].rel_position = -(Settings->param[P_ROTARY_MAX_STEP]);
      }

      Response_P(PSTR("{\"Rotary%d\":{\"Pos1\":%d,\"Pos2\":%d}}"), index +1, Encoder[index].abs_position[0], Encoder[index].abs_position[1]);
      XdrvRulesProcess(0);
#ifdef USE_LIGHT
    }
#endif  // USE_LIGHT
  }
}

#endif  // ROTARY_V1
