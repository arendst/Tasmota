/*
  xlgt_07_lsc_mcsl.ino - GPE Multi color smart light support for Tasmota

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

#ifdef USE_LIGHT
#ifdef USE_LSC_MCSL
/*********************************************************************************************\
 * Golden Power Electronics Integrated Switching Power Supply with Built-in Controller
 * GP-SW084-052 uses an ESP8285 (TYWE2S) and submicrocontroller controlling two wire RGB leds
 * https://www.gp-electronic.com/product/integrated-switching-power-supply-with-built-in-controller/eugs/european-vertical-6w8-function.html
 *
 * Template:
 * {"NAME":"LSC MC Lights","GPIO":[0,0,0,0,544,32,0,0,3840,0,3872,0,0,0],"FLAG":0,"BASE":18}
 *
 * NL: Action LSC Multi color smart lights
 *
 * Button usage:
 * Single press = Power On / Off
 * Double press = Cycle Colors
 * Triple press = Cycle Effects
 *
 * HSBColor    Color
 * ----------  -----------
 *   1 .. 45   R   (Red)
 *  46 .. 90   RG  (Yellow)
 *  91 .. 135  G   (Green)
 * 136 .. 179  GB  (Light Blue)
 * 180 .. 224  B   (Blue)
 * 225 .. 269  RB  (Purple)
 * 270 .. 314  RGB (White)
 * 315 .. 360      (Alternating)
 *
 * Dimmer      Effect
 * ----------  ---------------------------
 *   0 .. 12   Bright (Steady On)
 *  13 .. 24   Gradually (Slow Fade)
 *  25 .. 37   Star (Sequential)
 *  38 .. 49   Flower (In Waves)
 *  50 .. 62   Marquee (Chasing / Flash)
 *  63 .. 74   Fireworks (Twinkle / Flash)
 *  75 .. 87   Meteor
 *  88 .. 100  Stream
 *
 * GPIO04 = Green led
 * GPIO05 = Button
 * GPIO12 = Data to submicrocontroller
 * GPIO14 = Reset to submicrocontroller (26ms active high)
\*********************************************************************************************/

#define XLGT_07               7

#define LSCMC_GPIO_DATA       12
#define LSCMC_GPIO_RESET      14

#define LSCMC_BIT_TIME        600   // us

const uint16_t kLscMcData[] PROGMEM = {
  //R     RG      G       GB      B       RB      RGB     RGBa
  0x0F4D, 0x0745, 0x08B5, 0x0340, 0x0CB0, 0x04B8, 0x0B48, 0x0143,    // Bright (Steady on)
  0x8FED, 0x87E5, 0x8815, 0x83E0, 0x8C10, 0x8418, 0x8BE8, 0x81E3,    // Gradually (Slow Fade)
  0x70ED, 0x78E5, 0x7715, 0x7CE0, 0x7510, 0x7B18, 0x74E8, 0x7EE3,    // Star (Sequential)
  0xCF92, 0xC79A, 0xC86A, 0xC39F, 0xCC6F, 0xC467, 0xCB97, 0xC19C,    // Flower (In Waves)
  0x3093, 0x389A, 0x376A, 0x3C9F, 0x336F, 0x3B67, 0x3497, 0x3E9C,    // Marquee (Chasing / Flash)
  0xB02D, 0xB825, 0xB7D5, 0xBC20, 0xB3D0, 0xBBD8, 0xB428, 0xBE23,    // Fireworks (Twinkle / Flash)
  0x4F2D, 0x4725, 0x48D5, 0x4320, 0x4CD0, 0x44D8, 0x4B28, 0x4123,    // Meteor
  0xEFAD, 0xE7A5, 0xE855, 0xE3A1, 0xEC50, 0xE458, 0xEBA8, 0xE1A3     // Stream
};

#include <Ticker.h>
Ticker LscMcStartDelay;

struct LSCMC {
  uint32_t last_send;
  uint8_t function;
  uint8_t color;
  uint8_t dimmer;
  uint8_t power;
  uint8_t scheme_offset;
  uint8_t pin;
} Lscmc;

/*********************************************************************************************/

void LscMcSend(void) {
  Lscmc.function &= 0x7;
  Lscmc.color &= 0x7;
  uint16_t fc = pgm_read_word(kLscMcData + (8 * Lscmc.function) + Lscmc.color);

  uint32_t data;
  if (Lscmc.power) {
    data = 0xC2FF0000 | fc;
  } else {
    data = Lscmc.last_send & 0xFF00FFFF;
    data ^= 0x0000FF00;
  }

  if (data == Lscmc.last_send) { return; }

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("LSC: Function %d, Color %d, State %d, Send %08X"),
    Lscmc.function, Lscmc.color, Lscmc.power, data);

  Lscmc.last_send = data;

  digitalWrite(Lscmc.pin, LOW);
  delayMicroseconds(LSCMC_BIT_TIME * 15);
  digitalWrite(Lscmc.pin, HIGH);
  delayMicroseconds(LSCMC_BIT_TIME * 7 + (LSCMC_BIT_TIME / 2));
  digitalWrite(Lscmc.pin, LOW);
  delayMicroseconds(LSCMC_BIT_TIME);
  digitalWrite(Lscmc.pin, HIGH);
  bool last_bit = 0;
  bool bit = 0;
  uint32_t bit_mask = 0x80000000;
  for (uint32_t i = 0; i < 32; i++) {
    bit = (data & bit_mask);
    if (bit != last_bit) {
      delayMicroseconds(LSCMC_BIT_TIME * 3);  // Switch bit state
    } else {
      delayMicroseconds(LSCMC_BIT_TIME);
    }
    digitalWrite(Lscmc.pin, LOW);
    delayMicroseconds(LSCMC_BIT_TIME);
    digitalWrite(Lscmc.pin, HIGH);
    last_bit = bit;
    bit_mask >>= 1;
  }
}

/*********************************************************************************************/

bool LscMcSetChannelsFromFunc(void) {
  // Use power for Off / On
  uint8_t power = Light.power;
  bool power_changed = (Lscmc.power != power);
  Lscmc.power = power;

  // Use dimmer for function
  uint8_t dimmer = light_state.getDimmer();
/*
  dimmer = changeUIntScale(dimmer, 0, 100, 0, 3);
  dimmer = changeUIntScale(dimmer, 0, 3, 0, 255);
  bool dimmer_changed = (Lscmc.dimmer != dimmer);
  Lscmc.dimmer= dimmer;
*/
  uint8_t function = changeUIntScale(dimmer, 0, 100, 0, 7);
  bool function_changed = (Lscmc.function != function);
  Lscmc.function = function;

  uint16_t hue;
  uint8_t sat;
  LightGetHSB(&hue, &sat, nullptr);
/*
  // Use saturation for function (won't work as if sat = 0 this function is not called)
  uint8_t function = changeUIntScale(sat, 0, 255, 0, 7);
  bool function_changed = (Lscmc.function != function);
  Lscmc.function = function;
*/
  // Use hue for color
  uint8_t color = changeUIntScale(hue, 1, 359, 0, 7);
  bool color_changed = (Lscmc.color != color);
  Lscmc.color = color;

//  AddLog(LOG_LEVEL_DEBUG, PSTR("LSC: Power %d, Hue %d = Color %d, Dimmer %d = Function %d"),
//    Lscmc.power, hue, Lscmc.color, dimmer, Lscmc.function);

  if (!power_changed && !function_changed && !color_changed) { return true; }

  static bool first_call = true;
  if (first_call) {
    LscMcStartDelay.once_ms(900, LscMcSend);  // Allow startup time for microcontroller
    first_call = false;
  } else {
    LscMcSend();
  }

  return true;
}

bool LscMcMultiButtonPressed(void) {
  if (XdrvMailbox.index != 0) { return false; }  // button_index

  char command[20];

  uint32_t press_counter = XdrvMailbox.payload;
  if (2 == press_counter) {         // Color rotate
    uint32_t color = Lscmc.color +1;
    if (color > 7) { color = 0; }
    snprintf_P(command, sizeof(command), PSTR(D_CMND_HSBCOLOR " %d"), (color * (360 / 8)) + ((360 / 8) / 2));
    ExecuteCommand(command, SRC_BUTTON);
  } else if (3 == press_counter) {  // Function rotate
    uint32_t function = Lscmc.function +1;
    if (function > 7) { function = 0; }
    snprintf_P(command, sizeof(command), PSTR(D_CMND_DIMMER " %d"), (function * (100 / 8)) + ((100 / 8) / 2));
    ExecuteCommand(command, SRC_BUTTON);
  }
  return true;
}

void LscMcModuleSelected(void) {
  if (!ValidTemplate(PSTR("LSC MC Lights"))) { return; }
  if (!PinUsed(GPIO_OUTPUT_HI) || !PinUsed(GPIO_OUTPUT_LO)) { return; }

  Lscmc.pin = Pin(GPIO_OUTPUT_HI);

  uint32_t pin_reset = Pin(GPIO_OUTPUT_LO);
  digitalWrite(pin_reset, HIGH);
  delay(26);
  digitalWrite(pin_reset, LOW);

  Settings->flag.button_single = 0;           // SetOption13 - We need multi press detection
  Settings->flag3.slider_dimmer_stay_on = 1;  // SetOption77 - We need dimmer to keep power on at 0

  Lscmc.last_send = 0xC2FFEBA8;

  TasmotaGlobal.light_type = LT_RGB;
  TasmotaGlobal.light_driver = XLGT_07;
//  AddLog(LOG_LEVEL_DEBUG, PSTR("LGT: LSC Multi Color Found"));
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xlgt07(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_SET_CHANNELS:
      result = LscMcSetChannelsFromFunc();
      break;
    case FUNC_BUTTON_MULTI_PRESSED:
      result = LscMcMultiButtonPressed();
      break;
    case FUNC_MODULE_INIT:
      LscMcModuleSelected();
      break;
  }
  return result;
}

#endif  // USE_LSC_MCSL
#endif  // USE_LIGHT