/*
  xsns_42_neotrellis.ino - Support for Adafruit Neotrellis RGB driver PCB

  Copyright (C) 2019  Bert Outtier and Theo Arends

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

#ifdef USE_I2C
#ifdef USE_NEOTRELLIS

/*********************************************************************************************\
 * Adafruit Neotrellis RGB driver PCB
 * The Adafruit_seesaw library is used.
 *
 * More info at https://www.adafruit.com/product/3954 and https://github.com/adafruit/Adafruit_Seesaw
 *
 * I2C Address: 0x2E
\*********************************************************************************************/

#define XSNS_42

#include <Adafruit_NeoTrellis.h>

bool neotrellis_suspend_update = false;

Adafruit_seesaw seesaw(&Wire);
seesaw_NeoPixel pixels(NEO_TRELLIS_NUM_KEYS, NEO_TRELLIS_NEOPIX_PIN, NEO_GRB + NEO_KHZ800);

uint8_t neotrellis_flag = 1;

void NeotrellisForceSuspend () {
  neotrellis_suspend_update = true;
}

void NeotrellisForceUpdate () {
  neotrellis_suspend_update = false;
  pixels.show();
}

void NeotrellisSetColor(uint16_t led, uint8_t red, uint8_t green, uint8_t blue, uint8_t white)
{
  uint32_t lcolor = blue | (green << 8) | (red << 16);

  if (led) {
    pixels.setPixelColor(led -1, lcolor);  // Led 1 is strip Led 0 -> substract offset 1
  } else {
    for (uint16_t i = 0; i < Settings.light_pixels; i++) {
      pixels.setPixelColor(i, lcolor);
    }
  }

  if (!neotrellis_suspend_update) {
    pixels.show();
  }
}

char* NeotrellisGetColor(uint16_t led, char* scolor)
{
  uint8_t sl_ledcolor[4];

  uint32_t c = pixels.getPixelColor(led -1);
  sl_ledcolor[0] = (uint8_t)(c >> 16);
  sl_ledcolor[1] = (uint8_t)(c >>  8);
  sl_ledcolor[2] = (uint8_t)c;
  scolor[0] = '\0';
  for (byte i = 0; i < light_subtype; i++) {
    if (Settings.flag.decimal_text) {
      snprintf_P(scolor, 25, PSTR("%s%s%d"), scolor, (i > 0) ? "," : "", sl_ledcolor[i]);
    } else {
      snprintf_P(scolor, 25, PSTR("%s%02X"), scolor, sl_ledcolor[i]);
    }
  }
  return scolor;
}

void NeotrellisClear()
{
  for (uint16_t i=0; i<pixels.numPixels(); i++) {
    pixels.setPixelColor(i, 0x000000);
  }
  pixels.show();
}

void Neotrellis_loop()
{
  uint8_t count = seesaw.getKeypadCount();
  delayMicroseconds(500);
  if(count > 0){
      count = count + 2;
      keyEventRaw e[count];
      seesaw.readKeypad(e, count);
      for(int i=0; i<count; i++){
          e[i].bit.NUM = NEO_TRELLIS_SEESAW_KEY(e[i].bit.NUM);
          if(e[i].bit.NUM < NEO_TRELLIS_NUM_KEYS) {
            if (e[i].bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
              /* button is pressed */
              // pixels.setPixelColor(e[i].bit.NUM, 0x00FF0000); // set color on
              snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"Button_%i\":\"DOWN\"}"), e[i].bit.NUM + 1);
            } else if (e[i].bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING) {
              /* button is released */
              // pixels.setPixelColor(e[i].bit.NUM, 0); // set color off
              snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"Button_%i\":\"UP\"}"), e[i].bit.NUM + 1);
            }
            MqttPublishPrefixTopic_P(RESULT_OR_STAT, mqtt_data);
          }
      }
      pixels.show();
  }
}

void NeotrellisInit()
{
  neotrellis_flag = 0;
  bool ret = false;

  ret = pixels.begin(NEO_TRELLIS_ADDR, -1);
  if (!ret) {
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "Could not start trellis pixels, check wiring?"));
    AddLog(LOG_LEVEL_ALL);
    return;
  }

  ret = seesaw.begin(NEO_TRELLIS_ADDR, -1, false);
  if (!ret) {
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "Could not start trellis, check wiring?"));
    AddLog(LOG_LEVEL_ALL);
    return;
  }

  seesaw.enableKeypadInterrupt();

  //activate all keys and set callbacks
  for(int i=0; i<NEO_TRELLIS_NUM_KEYS; i++){
    seesaw.setKeypadEvent(NEO_TRELLIS_KEY(i), SEESAW_KEYPAD_EDGE_RISING, true);
    seesaw.setKeypadEvent(NEO_TRELLIS_KEY(i), SEESAW_KEYPAD_EDGE_FALLING, true);
  }

  NeotrellisClear();
  neotrellis_flag = 1;
}

/*********************************************************************************************
   Interface
*********************************************************************************************/

bool Xsns42(byte function)
{
  bool result = false;
  if (neotrellis_flag) {
    switch (function) {
      case FUNC_INIT:
        NeotrellisInit();
        break;
      case FUNC_EVERY_50_MSECOND:
        Neotrellis_loop();
        break;
    }
  }
  return result;
}

#endif // USE_NEOTRELLIS
#endif // USE_I2C
