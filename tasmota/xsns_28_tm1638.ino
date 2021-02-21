/*
  xsns_28_tm1638.ino - TM1638 8 switch, led and 7 segment unit support for Tasmota

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

#define XSNS_28             28

#define TM1638_COLOR_NONE   0
#define TM1638_COLOR_RED    1
#define TM1638_COLOR_GREEN  2

#define TM1638_CLOCK_DELAY  1  // uSec

uint8_t tm1638_type = 1;
int8_t tm1638_clock_pin = 0;
int8_t tm1638_data_pin = 0;
int8_t tm1638_strobe_pin = 0;
uint8_t tm1638_displays = 8;
uint8_t tm1638_active_display = 1;
uint8_t tm1638_intensity = 0;
uint8_t tm1638_state = 0;

/*********************************************************************************************\
 * Pieces from library https://github.com/rjbatista/tm1638-library
 *    and from library https://github.com/MartyMacGyver/TM1638-demos-and-examples
\*********************************************************************************************/

void Tm16XXSend(uint8_t data)
{
	for (uint32_t i = 0; i < 8; i++) {
    digitalWrite(tm1638_data_pin, !!(data & (1 << i)));
    digitalWrite(tm1638_clock_pin, LOW);
    delayMicroseconds(TM1638_CLOCK_DELAY);
    digitalWrite(tm1638_clock_pin, HIGH);
  }
}

void Tm16XXSendCommand(uint8_t cmd)
{
  digitalWrite(tm1638_strobe_pin, LOW);
  Tm16XXSend(cmd);
  digitalWrite(tm1638_strobe_pin, HIGH);
}

void TM16XXSendData(uint8_t address, uint8_t data)
{
  Tm16XXSendCommand(0x44);
  digitalWrite(tm1638_strobe_pin, LOW);
  Tm16XXSend(0xC0 | address);
  Tm16XXSend(data);
  digitalWrite(tm1638_strobe_pin, HIGH);
}

uint8_t Tm16XXReceive(void)
{
  uint8_t temp = 0;

  // Pull-up on
  pinMode(tm1638_data_pin, INPUT);
  digitalWrite(tm1638_data_pin, HIGH);

  for (uint32_t i = 0; i < 8; ++i) {
    digitalWrite(tm1638_clock_pin, LOW);
    delayMicroseconds(TM1638_CLOCK_DELAY);
    temp |= digitalRead(tm1638_data_pin) << i;
    digitalWrite(tm1638_clock_pin, HIGH);
  }

  // Pull-up off
  pinMode(tm1638_data_pin, OUTPUT);
  digitalWrite(tm1638_data_pin, LOW);

  return temp;
}

/*********************************************************************************************/

void Tm16XXClearDisplay(void)
{
  for (uint32_t i = 0; i < tm1638_displays; i++) {
    TM16XXSendData(i << 1, 0);
  }
}

void Tm1638SetLED(uint8_t color, uint8_t pos)
{
  TM16XXSendData((pos << 1) + 1, color);
}

void Tm1638SetLEDs(word leds)
{
  for (uint32_t i = 0; i < tm1638_displays; i++) {
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

uint8_t Tm1638GetButtons(void)
{
  uint8_t keys = 0;

  digitalWrite(tm1638_strobe_pin, LOW);
  Tm16XXSend(0x42);
  for (uint32_t i = 0; i < 4; i++) {
    keys |= Tm16XXReceive() << i;
  }
  digitalWrite(tm1638_strobe_pin, HIGH);

  return keys;
}

/*********************************************************************************************/

void TmInit(void)
{
  tm1638_type = 0;
  if (PinUsed(GPIO_TM16CLK) && PinUsed(GPIO_TM16DIO) && PinUsed(GPIO_TM16STB)) {
    tm1638_clock_pin = Pin(GPIO_TM16CLK);
    tm1638_data_pin = Pin(GPIO_TM16DIO);
    tm1638_strobe_pin = Pin(GPIO_TM16STB);

    pinMode(tm1638_data_pin, OUTPUT);
    pinMode(tm1638_clock_pin, OUTPUT);
    pinMode(tm1638_strobe_pin, OUTPUT);

    digitalWrite(tm1638_strobe_pin, HIGH);
    digitalWrite(tm1638_clock_pin, HIGH);

    Tm16XXSendCommand(0x40);
    Tm16XXSendCommand(0x80 | (tm1638_active_display ? 8 : 0) | tmin(7, tm1638_intensity));

    digitalWrite(tm1638_strobe_pin, LOW);
    Tm16XXSend(0xC0);
    for (uint32_t i = 0; i < 16; i++) {
      Tm16XXSend(0x00);
    }
    digitalWrite(tm1638_strobe_pin, HIGH);

    tm1638_type = 1;
    tm1638_state = 1;
  }
}

void TmLoop(void)
{
  if (tm1638_state) {
    uint8_t buttons = Tm1638GetButtons();
    for (uint32_t i = 0; i < MAX_SWITCHES; i++) {
      SwitchSetVirtual(i, (buttons &1) ^1);
      uint8_t color = (SwitchGetVirtual(i)) ? TM1638_COLOR_NONE : TM1638_COLOR_RED;
      Tm1638SetLED(color, i);
      buttons >>= 1;
    }
    SwitchHandler(1);
  }
}

/*
void TmShow(bool json)
{
  if (tm1638_type) {

  }
}
*/

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns28(uint8_t function)
{
  bool result = false;

  if (tm1638_type) {
    switch (function) {
      case FUNC_INIT:
        TmInit();
        break;
      case FUNC_EVERY_50_MSECOND:
        TmLoop();
        break;
/*
      case FUNC_JSON_APPEND:
        TmShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        TmShow(0);
        break;
#endif  // USE_WEBSERVER
*/
    }
  }
  return result;
}

#endif  // USE_TM1638