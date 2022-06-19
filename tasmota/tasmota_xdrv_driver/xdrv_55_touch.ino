/*
  xdrv_55_touch.ino - Touch contolers

  Copyright (C) 2021 Gerhard Mutz, Theo Arends & Stephan Hadinger

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

/*******************************************************************************************\
 * Universal TouchScreen driver, extensible via Berry
 * 
 * API:
 *   void Touch_Init() - TODO
 * 
 *   uint32_t Touch_Status(int32_t sel)
 *     0: return 1 if TSGlobal.touched
 *     1: return x
 *     2: return y
 *    -1: return raw x (before conersion for resistive)
 *    -2: return raw y
 * 
 *   void Touch_Check(void(*rotconvert)(int16_t *x, int16_t *y))
 * 
 *   void TS_RotConvert(int16_t *x, int16_t *y) - calls the renderer's rotation converter
\*******************************************************************************************/


#if defined(USE_LVGL_TOUCHSCREEN) || defined(USE_FT5206) || defined(USE_XPT2046) || defined(USE_LILYGO47) || defined(USE_TOUCH_BUTTONS)

#ifdef USE_DISPLAY_LVGL_ONLY
#undef USE_TOUCH_BUTTONS
#endif

#include <renderer.h>

#define XDRV_55             55

// Codes for gestures, when supported by the Touch Screen controller
enum TS_Gesture {
  TS_Gest_None = 0,
  TS_Gest_Move_Up = 0x10,
  TS_Gest_Move_Down = 0x11,
  TS_Gest_Move_Left = 0x12,
  TS_Gest_Move_Right = 0x13,
  TS_Gest_Zoom_In = 0x20,
  TS_Gest_Zoom_Out = 0x21,
};

typedef struct TSGlobal_t {
  int16_t raw_touch_xp = 0;
  int16_t raw_touch_yp = 0;
  int16_t touch_xp = 0;
  int16_t touch_yp = 0;
  uint8_t touches = 0;    // number of touches for multi-touch
  uint8_t gesture = 0;    // gesture code
  // multi-point is not yet supported
  bool touched = false;
  bool external_ts = false;
} TSGlobal_t;

TSGlobal_t TSGlobal;

bool FT5206_found = false;
bool XPT2046_found = false;

#ifndef MAX_TOUCH_BUTTONS
#define MAX_TOUCH_BUTTONS 16
#endif

#ifdef USE_TOUCH_BUTTONS
VButton *buttons[MAX_TOUCH_BUTTONS];
#endif

void Touch_SetStatus(uint8_t touches, uint16_t raw_x, uint16_t raw_y, uint8_t gesture) {
  TSGlobal.external_ts = true;
  TSGlobal.gesture = gesture;
  TSGlobal.touches = touches;
  TSGlobal.touched = (TSGlobal.touches > 0);
  TSGlobal.touch_xp = TSGlobal.raw_touch_xp = raw_x;
  TSGlobal.touch_yp = TSGlobal.raw_touch_yp = raw_y;
  TS_RotConvert(&TSGlobal.touch_xp, &TSGlobal.touch_yp);
}

// return true if succesful, false if not configured
bool Touch_GetStatus(uint8_t* touches, uint16_t* x, uint16_t* y, uint8_t* gesture,
                     uint16_t* raw_x, uint16_t* raw_y) {
  if (TSGlobal.external_ts || FT5206_found || XPT2046_found) {
    if (touches)    { *touches = TSGlobal.touches; }
    if (x)          { *x = TSGlobal.touch_xp; }
    if (y)          { *y = TSGlobal.touch_yp; }
    if (raw_x)      { *raw_x = TSGlobal.raw_touch_xp; }
    if (raw_y)      { *raw_y = TSGlobal.raw_touch_yp; }
    if (gesture)    { *touches = TSGlobal.gesture; }
    return true;
  }
  return false;
}

uint32_t Touch_Status(int32_t sel) {
  if (TSGlobal.external_ts || FT5206_found || XPT2046_found) {
    switch (sel) {
      case 0:
        return  TSGlobal.touched;
      case 1:
        return TSGlobal.touch_xp;
      case 2:
        return TSGlobal.touch_yp;
      case -1:                  // before calibration
        return TSGlobal.raw_touch_xp;
      case -2:
        return TSGlobal.raw_touch_yp;
    }
    return 0;
  } else {
    return 0;
  }
}

#ifdef USE_M5STACK_CORE2
uint8_t tbstate[3];
#endif // USE_M5STACK_CORE2

#ifdef USE_FT5206
#include <FT5206.h>
// touch panel controller
#undef FT5206_address
#define FT5206_address 0x38

FT5206_Class *FT5206_touchp;

bool FT5206_Touch_Init(TwoWire &i2c) {
  FT5206_found = false;
  FT5206_touchp = new FT5206_Class();
  if (FT5206_touchp->begin(i2c, FT5206_address)) {
    I2cSetActiveFound(FT5206_address, "FT5206");
    FT5206_found = true;
  }
  //AddLog(LOG_LEVEL_INFO, PSTR("TS: FT5206 %d"),FT5206_found);
  return FT5206_found;
}

bool FT5206_touched() {
  return FT5206_touchp->touched();
}
int16_t FT5206_x() {
  TP_Point pLoc = FT5206_touchp->getPoint(0);
  return pLoc.x;
}
int16_t FT5206_y() {
  TP_Point pLoc = FT5206_touchp->getPoint(0);
  return pLoc.y;
}
#endif  // USE_FT5206

#ifdef USE_XPT2046
#include <XPT2046_Touchscreen.h>
XPT2046_Touchscreen *XPT2046_touchp;


bool XPT2046_Touch_Init(uint16_t CS) {
  XPT2046_touchp = new XPT2046_Touchscreen(CS);
  XPT2046_found = XPT2046_touchp->begin();
  if (XPT2046_found) {
	   AddLog(LOG_LEVEL_INFO, PSTR("TS: XPT2046"));
  }
  return XPT2046_found;
}
bool XPT2046_touched() {
  return XPT2046_touchp->touched();
}
int16_t XPT2046_x() {
  TS_Point pLoc = XPT2046_touchp->getPoint();
  return pLoc.x;
}
int16_t XPT2046_y() {
  TS_Point pLoc = XPT2046_touchp->getPoint();
  return pLoc.y;
}
#endif  // USE_XPT2046



void Touch_Check(void(*rotconvert)(int16_t *x, int16_t *y)) {
  static bool was_touched = false;    // flag used to log the data sent when the screen was just released

#ifdef USE_FT5206
  if (FT5206_found) {
    TSGlobal.touched = FT5206_touched();
    if (TSGlobal.touched) {
      TSGlobal.raw_touch_xp = FT5206_x();
      TSGlobal.raw_touch_yp = FT5206_y();
    }
  }
#endif // USE_FT5206

#ifdef USE_XPT2046
  if (XPT2046_found) {
    TSGlobal.touched = XPT2046_touched();
    if (TSGlobal.touched) {
      TSGlobal.raw_touch_xp = XPT2046_x();
      TSGlobal.raw_touch_yp = XPT2046_y();
    }
  }
#endif // USE_XPT2046
  TSGlobal.touch_xp = TSGlobal.raw_touch_xp;
  TSGlobal.touch_yp = TSGlobal.raw_touch_yp;

  if (TSGlobal.touched) {
    was_touched = true;
#ifdef USE_TOUCH_BUTTONS
#ifdef USE_M5STACK_CORE2
    // handle  3 built in touch buttons
    uint16_t xcenter = 80;
#define TDELTA 30
#define TYPOS 275
    for (uint32_t tbut = 0; tbut < 3; tbut++) {
      if (TSGlobal.touch_xp > (xcenter - TDELTA) && TSGlobal.touch_xp < (xcenter + TDELTA) && TSGlobal.touch_yp > (TYPOS - TDELTA) && TSGlobal.touch_yp < (TYPOS + TDELTA)) {
        // hit a button
        if (!(tbstate[tbut] & 1)) {
          // pressed
          tbstate[tbut] |= 1;
          //AddLog(LOG_LEVEL_INFO, PSTR("tbut: %d pressed"), tbut);
          Touch_MQTT(tbut, "BIB", tbstate[tbut] & 1);
        }
      }
      xcenter += 100;
    }
#endif  // USE_M5STACK_CORE2
#endif // USE_TOUCH_BUTTONS

    rotconvert(&TSGlobal.touch_xp, &TSGlobal.touch_yp);
    AddLog(LOG_LEVEL_DEBUG_MORE, "TS : TSGlobal.touched x=%i y=%i (raw x=%i y=%i)", TSGlobal.touch_xp, TSGlobal.touch_yp, TSGlobal.raw_touch_xp, TSGlobal.raw_touch_yp);

#ifdef USE_TOUCH_BUTTONS
    CheckTouchButtons(TSGlobal.touched, TSGlobal.touch_xp, TSGlobal.touch_yp);
#endif // USE_TOUCH_BUTTONS

  } else {
#ifdef USE_M5STACK_CORE2
    for (uint32_t tbut = 0; tbut < 3; tbut++) {
      if (tbstate[tbut] & 1) {
        // released
        tbstate[tbut] &= 0xfe;
        Touch_MQTT(tbut, "BIB", tbstate[tbut] & 1);
        //AddLog(LOG_LEVEL_INFO, PSTR("tbut: %d released"), tbut);
      }
    }
#endif  // USE_M5STACK_CORE2

    rotconvert(&TSGlobal.touch_xp, &TSGlobal.touch_yp);   // still do rot convert if not TSGlobal.touched
    if (was_touched) {
      AddLog(LOG_LEVEL_DEBUG_MORE, "TS : released x=%i y=%i (raw x=%i y=%i)", TSGlobal.touch_xp, TSGlobal.touch_yp, TSGlobal.raw_touch_xp, TSGlobal.raw_touch_yp);
      was_touched = false;
    }
#ifdef USE_TOUCH_BUTTONS
    CheckTouchButtons(TSGlobal.touched, TSGlobal.touch_xp, TSGlobal.touch_yp);
#endif // USE_TOUCH_BUTTONS

  }
}

#ifdef USE_TOUCH_BUTTONS
void Touch_MQTT(uint8_t index, const char *cp, uint32_t val) {
#ifdef USE_FT5206
  if (FT5206_found) ResponseTime_P(PSTR(",\"FT5206\":{\"%s%d\":\"%d\"}}"), cp, index+1, val);
#endif
#ifdef USE_XPT2046
  if (XPT2046_found) ResponseTime_P(PSTR(",\"XPT2046\":{\"%s%d\":\"%d\"}}"), cp, index+1, val);
#endif  // USE_XPT2046
  MqttPublishTeleSensor();
}

void Touch_RDW_BUTT(uint32_t count, uint32_t pwr) {
  buttons[count]->xdrawButton(pwr);
  if (pwr) buttons[count]->vpower.on_off = 1;
  else buttons[count]->vpower.on_off = 0;
}

void CheckTouchButtons(bool touched, int16_t touch_x, int16_t touch_y) {
  uint16_t temp;
  uint8_t rbutt=0;
  uint8_t vbutt=0;

  if (!renderer) return;
    if (TSGlobal.touched) {
      // AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("touch after convert %d - %d"), pLoc.x, pLoc.y);
      // now must compare with defined buttons
      for (uint8_t count = 0; count < MAX_TOUCH_BUTTONS; count++) {
        if (buttons[count]) {
          if (!buttons[count]->vpower.slider) {
            if (!buttons[count]->vpower.disable) {
              if (buttons[count]->contains(touch_x, touch_y)) {
                // did hit
                buttons[count]->press(true);
                if (buttons[count]->justPressed()) {
                  if (!buttons[count]->vpower.is_virtual) {
                    uint8_t pwr=bitRead(TasmotaGlobal.power, rbutt);
                    if (!SendKey(KEY_BUTTON, rbutt+1, POWER_TOGGLE)) {
                      ExecuteCommandPower(rbutt+1, POWER_TOGGLE, SRC_BUTTON);
                      Touch_RDW_BUTT(count, !pwr);
                    }
                  } else {
                    // virtual button
                    const char *cp;
                    if (!buttons[count]->vpower.is_pushbutton) {
                      // toggle button
                      buttons[count]->vpower.on_off ^= 1;
                      cp="TBT";
                    } else {
                      // push button
                      buttons[count]->vpower.on_off = 1;
                      cp="PBT";
                    }
                    buttons[count]->xdrawButton(buttons[count]->vpower.on_off);
                    Touch_MQTT(count, cp, buttons[count]->vpower.on_off);
                  }
                }
              }
              if (!buttons[count]->vpower.is_virtual) {
                rbutt++;
              } else {
                vbutt++;
              }
            }
          } else {
            // slider
            if (buttons[count]->didhit(touch_x, touch_y)) {
              uint16_t value = buttons[count]->UpdateSlider(touch_x, touch_y);
              Touch_MQTT(count, "SLD", value);
            }
          }
        }
      }

  } else {
    // no hit
    for (uint8_t count = 0; count < MAX_TOUCH_BUTTONS; count++) {
      if (buttons[count]) {
        if (!buttons[count]->vpower.slider) {
          buttons[count]->press(false);
          if (buttons[count]->justReleased()) {
            if (buttons[count]->vpower.is_virtual) {
              if (buttons[count]->vpower.is_pushbutton) {
                // push button
                buttons[count]->vpower.on_off = 0;
                Touch_MQTT(count,"PBT", buttons[count]->vpower.on_off);
                buttons[count]->xdrawButton(buttons[count]->vpower.on_off);
              }
            }
          }
          if (!buttons[count]->vpower.is_virtual) {
            // check if power button stage changed
            uint8_t pwr = bitRead(TasmotaGlobal.power, rbutt);
            uint8_t vpwr = buttons[count]->vpower.on_off;
            if (pwr != vpwr) {
              Touch_RDW_BUTT(count, pwr);
            }
            rbutt++;
          }
        }
      }
    }
    TSGlobal.raw_touch_xp = TSGlobal.touch_xp = 0;
    TSGlobal.raw_touch_yp = TSGlobal.touch_yp = 0;
  }
}
#endif // USE_TOUCH_BUTTONS

void TS_RotConvert(int16_t *x, int16_t *y) {
  if (renderer) renderer->TS_RotConvert(x, y);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdrv55(uint8_t function) {
  bool result = false;

  switch (function) {
    case FUNC_INIT:
      break;
    case FUNC_EVERY_100_MSECOND:
      if (FT5206_found || XPT2046_found) {
        Touch_Check(TS_RotConvert);
      }
      break;
  }
  return result;
}

#else  // #if defined(USE_FT5206) || defined(USE_XPT2046) || defined(USE_LILYGO47) || defined(USE_TOUCH_BUTTONS)

// dummy for LVGL without a touch controller
uint32_t Touch_Status(int32_t sel) {
  return 0;
}

#endif  // #if defined(USE_FT5206) || defined(USE_XPT2046) || defined(USE_LILYGO47) || defined(USE_TOUCH_BUTTONS)
