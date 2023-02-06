/*
  xdrv_66_tm1638.ino - TM1638 8 switch, led and 7 segment unit support for Tasmota

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

#ifdef USE_TM1638
/*********************************************************************************************\
 * TM1638 8 switch, led and 7 segment
 *
 * Uses GPIO TM1638 DIO, TM1638 CLK and TM1638 STB
\*********************************************************************************************/

#define XDRV_66               66

#ifdef TM1638_USE_AS_BUTTON
#define TM1638_USE_BUTTONS         // Use keys as buttons
#endif
#ifdef TM1638_USE_AS_SWITCH
#undef TM1638_USE_BUTTONS          // Use keys as switches
#endif
#ifndef TM1638_MAX_DISPLAYS
#define TM1638_MAX_DISPLAYS   8
#endif
#ifndef TM1638_MAX_KEYS
#define TM1638_MAX_KEYS       8
#endif
#ifndef TM1638_MAX_LEDS
#define TM1638_MAX_LEDS       8
#endif

#define TM1638_COLOR_NONE     0
#define TM1638_COLOR_RED      1
#define TM1638_COLOR_GREEN    2

#define TM1638_CLOCK_DELAY    1    // uSec

struct TM1638 {
  int8_t clock_pin = 0;
  int8_t data_pin = 0;
  int8_t strobe_pin = 0;
  int8_t key_offset;
  int8_t led_offset;
  uint8_t displays;
  uint8_t power;

  uint8_t active_display = 1;
  uint8_t intensity = 0;
  bool detected = false;
} Tm1638;

/*********************************************************************************************\
 * Pieces from library https://github.com/rjbatista/tm1638-library
 *    and from library https://github.com/MartyMacGyver/TM1638-demos-and-examples
\*********************************************************************************************/

void Tm16XXSend(uint8_t data) {
	for (uint32_t i = 0; i < 8; i++) {
    digitalWrite(Tm1638.data_pin, !!(data & (1 << i)));
    digitalWrite(Tm1638.clock_pin, LOW);
    delayMicroseconds(TM1638_CLOCK_DELAY);
    digitalWrite(Tm1638.clock_pin, HIGH);
  }
}

void Tm16XXSendCommand(uint8_t cmd) {
  digitalWrite(Tm1638.strobe_pin, LOW);
  Tm16XXSend(cmd);
  digitalWrite(Tm1638.strobe_pin, HIGH);
}

void TM16XXSendData(uint8_t address, uint8_t data) {
  Tm16XXSendCommand(0x44);
  digitalWrite(Tm1638.strobe_pin, LOW);
  Tm16XXSend(0xC0 | address);
  Tm16XXSend(data);
  digitalWrite(Tm1638.strobe_pin, HIGH);
}

uint8_t Tm16XXReceive(void) {
  uint8_t temp = 0;

  // Pull-up on
  pinMode(Tm1638.data_pin, INPUT);
  digitalWrite(Tm1638.data_pin, HIGH);

  for (uint32_t i = 0; i < 8; ++i) {
    digitalWrite(Tm1638.clock_pin, LOW);
    delayMicroseconds(TM1638_CLOCK_DELAY);
    temp |= digitalRead(Tm1638.data_pin) << i;
    digitalWrite(Tm1638.clock_pin, HIGH);
  }

  // Pull-up off
  pinMode(Tm1638.data_pin, OUTPUT);
  digitalWrite(Tm1638.data_pin, LOW);

  return temp;
}

/*********************************************************************************************/

void Tm16XXClearDisplay(void) {
  for (uint32_t i = 0; i < Tm1638.displays; i++) {
    TM16XXSendData(i << 1, 0);
  }
}

void Tm1638SetLED(uint8_t color, uint8_t pos) {
  TM16XXSendData((pos << 1) + 1, color);
}

void Tm1638SetLEDs(word leds) {
  for (uint32_t i = 0; i < Tm1638.displays; i++) {
    uint8_t color = 0;

    if ((leds & (1 << i)) != 0) {
      color |= TM1638_COLOR_RED;
    }

    if ((leds & (1 << (i + 8))) != 0) {
      color |= TM1638_COLOR_GREEN;
    }

    Tm1638SetLED(color, i);
  }
}

uint8_t Tm1638GetButtons(void) {
  uint8_t keys = 0;

  digitalWrite(Tm1638.strobe_pin, LOW);
  Tm16XXSend(0x42);
  for (uint32_t i = 0; i < 4; i++) {
    keys |= Tm16XXReceive() << i;
  }
  digitalWrite(Tm1638.strobe_pin, HIGH);

  return keys;
}

/*********************************************************************************************/

void TmInit(void) {
  if (PinUsed(GPIO_TM1638CLK) && PinUsed(GPIO_TM1638DIO) && PinUsed(GPIO_TM1638STB)) {
    Tm1638.clock_pin = Pin(GPIO_TM1638CLK);
    Tm1638.data_pin = Pin(GPIO_TM1638DIO);
    Tm1638.strobe_pin = Pin(GPIO_TM1638STB);

    pinMode(Tm1638.data_pin, OUTPUT);
    pinMode(Tm1638.clock_pin, OUTPUT);
    pinMode(Tm1638.strobe_pin, OUTPUT);

    digitalWrite(Tm1638.strobe_pin, HIGH);
    digitalWrite(Tm1638.clock_pin, HIGH);

    Tm16XXSendCommand(0x40);
    Tm16XXSendCommand(0x80 | (Tm1638.active_display ? 8 : 0) | tmin(7, Tm1638.intensity));

    digitalWrite(Tm1638.strobe_pin, LOW);
    Tm16XXSend(0xC0);
    for (uint32_t i = 0; i < 16; i++) {
      Tm16XXSend(0x00);
    }
    digitalWrite(Tm1638.strobe_pin, HIGH);

    // Dirty hack to offset TM1638 leds from GPIO relays
    // At this time in code sequence the number of GPIO relays has not been established
    uint32_t bi_device = 0;
    uint32_t devices_present = 0;
    for (uint32_t i = 0; i < MAX_RELAYS; i++) {
      if (PinUsed(GPIO_REL1, i)) {
        devices_present++;
        if (bitRead(TasmotaGlobal.rel_bistable, i)) {
          if (bi_device &1) { devices_present--; }
          bi_device++;
        }
      }
    }

    Tm1638.led_offset = devices_present;
    TasmotaGlobal.devices_present += TM1638_MAX_LEDS;
    Tm1638.key_offset = -1;
    Tm1638.displays = TM1638_MAX_DISPLAYS;
    Tm1638.detected = true;
  }
}

void TmLoop(void) {
  uint8_t buttons = Tm1638GetButtons();
  for (uint32_t i = 0; i < TM1638_MAX_KEYS; i++) {
    uint32_t state = buttons &1;
#ifdef TM1638_USE_BUTTONS
    ButtonSetVirtualPinState(Tm1638.key_offset +i, state);
#else
    SwitchSetVirtualPinState(Tm1638.key_offset +i, state ^1);
#endif
    buttons >>= 1;
  }
}

void TmPower(void) {
  power_t rpower = XdrvMailbox.index >> Tm1638.led_offset;
  for (uint32_t i = 0; i < TM1638_MAX_LEDS; i++) {
    uint32_t state = rpower &1;
    uint8_t color = (state) ? TM1638_COLOR_RED : TM1638_COLOR_NONE;
    Tm1638SetLED(color, i);
    rpower >>= 1;                             // Select next power
  }
}

bool TmAddKey(void) {
  // XdrvMailbox.index = button/switch index
  if (Tm1638.key_offset < 0) { Tm1638.key_offset = XdrvMailbox.index; }
  uint32_t index = XdrvMailbox.index - Tm1638.key_offset;
  if (index >= TM1638_MAX_KEYS) { return false; }
  uint8_t buttons = Tm1638GetButtons();
  uint32_t state = bitRead(buttons, index);
#ifdef TM1638_USE_BUTTONS
  XdrvMailbox.index = state | BUTTON_INVERT;  // Invert - default is 0
#else
  XdrvMailbox.index = state ^1;               // Invert - default is 0
#endif
  return true;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv66(uint32_t function) {
  bool result = false;

  if (FUNC_MODULE_INIT == function) {
    TmInit();
  } else if (Tm1638.detected) {
    switch (function) {
      case FUNC_EVERY_50_MSECOND:
        TmLoop();
        break;
      case FUNC_SET_POWER:
        TmPower();
        break;
#ifdef TM1638_USE_BUTTONS
      case FUNC_ADD_BUTTON:
#else
      case FUNC_ADD_SWITCH:
#endif
        result = TmAddKey();
        break;
    }
  }
  return result;
}

#endif  // USE_TM1638