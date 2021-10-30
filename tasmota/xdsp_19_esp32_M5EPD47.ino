/*
  xdsp_19_esp32_M5EPD47.ino - M5Stack e-paper 4.7 inch support for Tasmota

  Copyright (C) 2021  Theo Arends, Gerhard Mutz and M5Stack

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

#ifdef ESP32
#ifdef USE_DISPLAY
#ifdef USE_M5EPD47

#define XDSP_19               19

#define M5EPD47_BLACK  0
#define M5EPD47_WHITE 15

#include <m5epd4in7.h>


M5Epd47 *m5epd47;
bool m5epd47_init_done = false;
extern uint8_t color_type;
extern uint16_t fg_color;
extern uint16_t bg_color;

/*********************************************************************************************/

void M5EpdInitDriver47(void) {

  if (PinUsed(GPIO_EPD_DATA)) {
  //  if (1) {

    Settings->display_model = XDSP_19;

    if (Settings->display_width != M5EPD47_WIDTH) {
      Settings->display_width = M5EPD47_WIDTH;
    }
    if (Settings->display_height != M5EPD47_HEIGHT) {
      Settings->display_height = M5EPD47_HEIGHT;
    }

    // init renderer
    m5epd47  = new M5Epd47(Settings->display_width, Settings->display_height);
    m5epd47->Init();

    renderer = m5epd47;
    renderer->DisplayInit(DISPLAY_INIT_MODE, Settings->display_size, Settings->display_rotate, Settings->display_font);
    renderer->setTextColor(M5EPD47_BLACK, M5EPD47_WHITE);

#ifdef SHOW_SPLASH
    // Welcome text
    renderer->setTextFont(2);
    renderer->DrawStringAt(50, 50, "M5Stack 4.7 E-Paper Display!", M5EPD47_BLACK, 0);
    renderer->Updateframe();
#endif

    fg_color = M5EPD47_BLACK;
    bg_color = M5EPD47_WHITE;
    color_type = COLOR_COLOR;

#ifdef USE_TOUCH_BUTTONS
    // start digitizer
    M5EPD47_Touch_Init();
#endif // USE_TOUCH_BUTTONS

    m5epd47_init_done = true;
    AddLog(LOG_LEVEL_INFO, PSTR("DSP: M5 E-Paper 4.7"));
  }
}

/*********************************************************************************************/


#ifdef USE_TOUCH_BUTTONS
// GT911 touch controller
#include "GT911.H"


#define M5EPD47_address 0x5D

GT911 *M5EPD47_touchp;

void M5EPD47_Touch_Init(void) {
FT5206_found = false;
M5EPD47_touchp = new GT911();

if (ESP_OK == M5EPD47_touchp->begin(21, 22, 36)) {
    I2cSetActiveFound(M5EPD47_address, "M5EPD47");
    FT5206_found = true;
  }
}

uint8_t M5EPD47_ctouch_counter = 0;
// no rotation support
void M5EPD47_RotConvert(int16_t *x, int16_t *y) {
int16_t temp;
  if (renderer) {
    uint8_t rot=renderer->getRotation();
    switch (rot) {
      case 0:
        break;
      case 1:
        temp=*y;
        *y=renderer->height()-*x;
        *x=temp;
        break;
      case 2:
        *x=renderer->width()-*x;
        *y=renderer->height()-*y;
        break;
      case 3:
        temp=*y;
        *y=*x;
        *x=renderer->width()-temp;
        break;
    }
  }
}

// check digitizer hit
void M5EPD47_CheckTouch(void) {
  M5EPD47_ctouch_counter++;
  if (2 == M5EPD47_ctouch_counter) {
    // every 100 ms should be enough
    M5EPD47_ctouch_counter = 0;
    if (M5EPD47_touchp->avaliable()) {
      M5EPD47_touchp->update();
      touched = !M5EPD47_touchp->isFingerUp();
      if (touched) {
        //M5EPD47_touchp->update();
        tp_finger_t FingerItem = M5EPD47_touchp->readFinger(0);

        touch_xp = M5EPD47_touchp->readFingerX(0);
        touch_yp = M5EPD47_touchp->readFingerY(0);
        if (touch_xp < 0) touch_xp = 0;
        if (touch_yp < 0) touch_yp = 0;
        if (touch_xp >= M5EPD47_WIDTH) touch_xp = M5EPD47_WIDTH - 1;
        if (touch_yp >= M5EPD47_HEIGHT) touch_yp = M5EPD47_HEIGHT - 1;

      //  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("touch before convert %d - %d"), touch_xp, touch_yp);

        M5EPD47_RotConvert(&touch_xp, &touch_yp);

      //  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("touch after convert %d - %d"), touch_xp, touch_yp);

      }
    }
    //Touch_Check(EPD47_RotConvert);
  }
}
#endif // USE_TOUCH_BUTTONS



/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdsp19(uint8_t function)
{
  bool result = false;

  if (FUNC_DISPLAY_INIT_DRIVER == function) {
    M5EpdInitDriver47();
  }
  else if (m5epd47_init_done && (XDSP_19 == Settings->display_model)) {
    switch (function) {
      case FUNC_DISPLAY_MODEL:
        result = true;
        break;
#ifdef USE_TOUCH_BUTTONS
      case FUNC_DISPLAY_EVERY_50_MSECOND:
        if (FT5206_found) {
          M5EPD47_CheckTouch();
        }
        break;
#endif // USE_TOUCH_BUTTONS
    }
  }
  return result;
}

#endif  // USE_M5EPD47
#endif  // USE_DISPLAY
#endif  // ESP32
