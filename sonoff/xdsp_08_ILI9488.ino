/*
  xdsp_08_ILI9488.ino - Display ILI9488 support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends, Gerhard Mutz

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

#ifdef USE_SOFTSPI
#ifdef USE_DISPLAY
#ifdef USE_DISPLAY_ILI9488

#define XDSP_08                8

#define COLORED                1
#define UNCOLORED              0

// using font 8 is opional (num=3)
// very badly readable, but may be useful for graphs
#define USE_TINY_FONT
#define USE_TOUCH_BUTTONS

#include <ILI9488.h>
#include <ILI9488.h>
#include <FT6236.h>

TouchLocation pLoc;
uint8_t ctouch_counter = 0;

// currently fixed
#define BACKPLANE_PIN 2

extern uint8_t *buffer;
extern uint8_t color_type;
ILI9488 *ili9488;

#ifdef USE_TOUCH_BUTTONS
extern Adafruit_GFX_Button *buttons[];
#endif

extern const uint16_t picture[];

/*********************************************************************************************/

void ILI9488_InitDriver()
{
  if (!Settings.display_model) {
    Settings.display_model = XDSP_08;
  }

  if (XDSP_08 == Settings.display_model) {

    // disable screen buffer
    buffer=NULL;

    // default colors
    fg_color = ILI9488_WHITE;
    bg_color = ILI9488_BLACK;

    uint8_t bppin=BACKPLANE_PIN;
    if  (pin[GPIO_COLDISP_BL]<99) {
      bppin=pin[GPIO_COLDISP_BL];
    }

    // init renderer
#ifdef USE_SOFTSPI
    if  ((pin[GPIO_SSPI_CS]<99) && (pin[GPIO_SSPI_MOSI]<99) && (pin[GPIO_SSPI_SCLK]<99)){
      ili9488  = new ILI9488(pin[GPIO_SSPI_CS],pin[GPIO_SSPI_MOSI],pin[GPIO_SSPI_SCLK],bppin);
    } else {
      return;
    }
#else
    if ((pin[GPIO_SPI_CS]<99) && (pin[GPIO_SPI_MOSI]<99) && (pin[GPIO_SPI_CLK]<99)) {
      ili9488  = new ILI9488(pin[GPIO_SPI_CS],pin[GPIO_SPI_MOSI],pin[GPIO_SPI_CLK],bppin);
    } else {
      return;
    }
#endif
    ili9488->begin();
    renderer = ili9488;
    renderer->DisplayInit(DISPLAY_INIT_MODE,Settings.display_size,Settings.display_rotate,Settings.display_font);

#ifdef SHOW_SPLASH
    // Welcome text
    renderer->setTextFont(2);
    renderer->setTextColor(ILI9488_WHITE,ILI9488_BLACK);
    renderer->DrawStringAt(50, 50, (char*)(const char*)"ILI9488 TFT Display!", ILI9488_WHITE,0);
    delay(1000);

    //renderer->drawRGBBitmap(100,100, picture,51,34);
#endif

    color_type = COLOR_COLOR;
    // start digitizer with fixed adress
    FT6236begin(0x38);
  }
}

// check digitizer hit
void FT6236Check() {
uint16_t temp;
ctouch_counter++;
if (2 == ctouch_counter) {
  // every 100 ms should be enough
  ctouch_counter=0;
  if (FT6236readTouchLocation(&pLoc,1)) {
    // did find a hit
    if (renderer) {
      uint8_t rot=renderer->getRotation();
      switch (rot) {
        case 0:
          temp=pLoc.y;
          pLoc.y=renderer->height()-pLoc.x;
          pLoc.x=temp;
          break;
        case 1:
          break;
        case 2:
          break;
        case 3:
          temp=pLoc.y;
          pLoc.y=pLoc.x;
          pLoc.x=renderer->width()-temp;
          break;
      }
      // now must compare with defined buttons
      for (uint8_t count=0; count<MAXBUTTONS; count++) {
        if (buttons[count]) {
            if (buttons[count]->contains(pLoc.x,pLoc.y)) {
                // did hit
                buttons[count]->press(true);
                if (buttons[count]->justPressed()) {
                  buttons[count]->drawButton(true);
                  // should send mqtt that button was pressed independant of relais etc
                  //SendKey(0, count+1, POWER_TOGGLE);

                  if (!SendKey(0, count +1, POWER_TOGGLE)) {  // Execute Toggle command via MQTT if ButtonTopic is set
                    ExecuteCommandPower(count +1, POWER_TOGGLE, SRC_BUTTON);  // Execute Toggle command internally
                  }
                  /*
                  char stopic[TOPSZ];
                  char key_topic[sizeof(Settings.button_topic)];
                  char *tmp = (0) ? Settings.switch_topic : Settings.button_topic;
                  Format(key_topic, tmp, sizeof(key_topic));

                  GetTopic_P(stopic, CMND, mqtt_topic, "");
                  MqttPublishDirect(stopic, (0) ? Settings.flag.mqtt_switch_retain : Settings.flag.mqtt_button_retain);
                  */
                }
            }
        }
      }
    }
  } else {
    // no hit
    for (uint8_t count=0; count<MAXBUTTONS; count++) {
      if (buttons[count]) {
        buttons[count]->press(false);
        if (buttons[count]->justReleased()) {
            // redraw button
            buttons[count]->drawButton(false);
        }
      }
    }
    pLoc.x=0;
    pLoc.y=0;
  }
}
}
/*********************************************************************************************/
/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
boolean Xdsp08(byte function)
{
  boolean result = false;

    if (FUNC_DISPLAY_INIT_DRIVER == function) {
      ILI9488_InitDriver();
    }
    else if (XDSP_08 == Settings.display_model) {

      switch (function) {
        case FUNC_DISPLAY_MODEL:
          result = true;
          break;
        case FUNC_DISPLAY_EVERY_50_MSECOND:
          FT6236Check();
          break;
      }
    }
  //}
  return result;
}

#endif  // USE_DISPLAY_ILI9488
#endif  // USE_DISPLAY
#endif  // USE_SPI
