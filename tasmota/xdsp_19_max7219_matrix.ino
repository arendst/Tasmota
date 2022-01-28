/*
  xdsp_19_max7219_matrix.ino.ino - Support for MAX7219 based 8x8 dot matrix displays for Tasmota

  Copyright (C) 2021  Michael Beuss

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

#ifdef USE_DISPLAY
#ifdef USE_DISPLAY_MAX7219_MATRIX
/*********************************************************************************************\
  This driver enables the display of ascii text on MAX7219 based 8x8 LED dot matrix modules (1088AS).

  Connect the MAX7219 display module's pins to any free GPIOs of the ESP8266 or ESP32 module.
  VCC should be 5V. Depending on the number of used modules and the brightness, the used current can be more than 500 mA.

  Connect the 5 outgoing pins (VCC, GND, DI, CS, CLK) of the first module to the next one. 
  With this you can connect up to 32 modules. 
  To extend the display hieght, multiple rows are supported. Each module row starts from left to right.

  Assign the pins as follows from Tasmota's GUI:

  DIN hardware pin --> "MAX7219 DIN"
  CS hardware pin --> "MAX7219 CS"
  CLK hardware pin --> "MAX7219 CLK"


  Once the GPIO configuration is saved and the ESP8266/ESP32 module restarts,
  set the Display Model to 19 and Display Mode to 0

  Depending on order of the wired 8x8 matrix modules you have got a display of size pixel_width x pixel_height.
  The size has to be set with the commands "DisplayWidth <pixel_width>" and "DisplayHeight <pixel_height>"

  After the ESP8266/ESP32 module restarts again, turn ON the display with the command "Power 1"


  Now, the following "Display" commands can be used:

  DisplayText  text 
    Sends the text to the display. 
    If the text fits into the display, it is shown in the center.
    Otherwise it scrolls to the left and repeats as long it is cleared or new "DisplayText" overwrites it.

  DisplayDimmer [0..100]
    Sets the intensity of the display.

  DisplayBlinkrate [0..3]
    0: not blinking
    1: slow, 2: medium 3: fast blinking

  Power [ON|OFF]
    Sitches the display on or off. When "off", the display buffer is not cleared and will be shown again when after "Power ON". 
    Other display commands are still active when off.

  DisplayClear
    Clears the display

  DisplayScrollDelay [0..15]   // default = 0
    Sets the speed of text scroll. Smaller delay = faster scrolling.
    The maximum scroll speed is 50ms per pixel on DisplayScrollDelay 0.

  DisplayWidth [8..256]
    Sets the pixel width of the display (8x number of modules in a row)

  DisplayHeight [8..256]
    Sets the pixel height of the display (8x number of module rows)

  DisplayRotate [0|2]
    0: normal orientation; devide 0 starts at top left
    2: upside down; device 0 starts at bottom right

  DisplayClock  [0|1|2]
    Displays a clock.
    Commands "DisplayClock 1"     // 12 hr format
            "DisplayClock 2"     // 24 hr format
            "DisplayClock 0"     // turn off clock; please use additional cammand: DisplayMode 0

  If you would like to use the UTF8 latin1 character set, it cam be added by copile option: 
  #define USE_UTF8_LATIN1

\*********************************************************************************************/

#define XDSP_19 19

#include <LedMatrix.h>

LedMatrix *max7219_Matrix = nullptr;
bool max2791Matrix_initDriver_done = false;
struct
{
    byte modulesPerRow = 4;
    byte modulesPerCol = 1;
    byte scroll_delay = 0;
    byte scroll_iteration = 0;
    bool show_clock = false;
    bool timeFormat24 = true;
    byte blink_delay = 0; // 0: not blinking
    byte blink_iteration = 0;

} LedMatrix_settings;

// FUNC_DISPLAY_INIT_DRIVER
bool MAX7291Matrix_initDriver(void)
{
    if (!PinUsed(GPIO_MAX7219DIN) || !PinUsed(GPIO_MAX7219CLK) || !PinUsed(GPIO_MAX7219CS))
    {
        AddLog(LOG_LEVEL_INFO, PSTR("DSP: MAX7291Matrix_init GPIO pins missing DIN:%d, CLK:%d, CS:%d"), Pin(GPIO_MAX7219DIN), Pin(GPIO_MAX7219CLK), Pin(GPIO_MAX7219CS)  );
        return false; // ensure necessariy pins are configurated
    }

    Settings->display_model = XDSP_19;
    renderer = nullptr; // renderer not yet used
    if (Settings->display_width) // [pixel]
    {
        LedMatrix_settings.modulesPerRow = (Settings->display_width - 1) / 8 + 1;
    }
    Settings->display_width = 8 * LedMatrix_settings.modulesPerRow;
    Settings->display_cols[0] = LedMatrix_settings.modulesPerRow;
    if (Settings->display_height) // [pixel]
    {
        LedMatrix_settings.modulesPerCol = (Settings->display_height - 1) / 8 + 1;
    }
    Settings->display_height = 8 * LedMatrix_settings.modulesPerCol;
    Settings->display_rows = LedMatrix_settings.modulesPerCol;
    Settings->display_cols[1] = LedMatrix_settings.modulesPerCol;
    max7219_Matrix = new LedMatrix(Pin(GPIO_MAX7219DIN), Pin(GPIO_MAX7219CLK), Pin(GPIO_MAX7219CS), LedMatrix_settings.modulesPerRow, LedMatrix_settings.modulesPerCol);
    max2791Matrix_initDriver_done = true;

    AddLog(LOG_LEVEL_INFO, PSTR("MTX: MAX7291Matrix_initDriver DIN:%d CLK:%d CS:%d size(%dx%d)"), Pin(GPIO_MAX7219DIN), Pin(GPIO_MAX7219CLK), Pin(GPIO_MAX7219CS), LedMatrix_settings.modulesPerRow, LedMatrix_settings.modulesPerCol);
    return MAX7291Matrix_init();
}

// FUNC_DISPLAY_INIT
bool MAX7291Matrix_init(void)
{
    Settings->display_mode = 0; // text mode
    LedMatrix_settings.show_clock = 0; // no 
    LedMatrix_settings.blink_delay = 0; // no blinking

    int intensity = GetDisplayDimmer16(); // 0..15
    max7219_Matrix->setIntensity(intensity);

    max7219_Matrix->power(true); // power on
    if(Settings->display_rotate <= 3)
    {
        max7219_Matrix->setOrientation((LedMatrix::ModuleOrientation)Settings->display_rotate );
    }
    else
    {
        // default for most 32x8 modules
        Settings->display_rotate = LedMatrix::ORIENTATION_UPSIDE_DOWN; 
        max7219_Matrix->setOrientation( LedMatrix::ORIENTATION_UPSIDE_DOWN ); 
    }
    AddLog(LOG_LEVEL_INFO, PSTR("MTX: MAX7291Matrix_init orientation: %d, intensity: %d"), Settings->display_rotate, intensity);
    return true;
}

bool MAX7291Matrix_setText(bool clearBefore=true)
{
    if(Settings->display_mode != 0) MAX7291Matrix_init();
    LedMatrix_settings.blink_delay = 0; // no blinking    
    return max7219_Matrix->drawText(XdrvMailbox.data, clearBefore); 
}

// FUNC_DISPLAY_SCROLLDELAY
bool MAX7291Matrix_scrollDelay(void)
{
  if (ArgC() == 0)
  {
    XdrvMailbox.payload = LedMatrix_settings.scroll_delay;
    return true;
  }
  if (LedMatrix_settings.scroll_delay < 0)
    LedMatrix_settings.scroll_delay = 0;
  LedMatrix_settings.scroll_delay = XdrvMailbox.payload;
  return true;
}

// FUNC_DISPLAY_EVERY_50_MSECOND
bool MAX7291Matrix_scrollText(void)
{
    // This function is called every 50 ms.
    // scroll_delay defines the number of cycles to be ignored until the display scrolls by one pixel to the left.
    // e.g. scrall_delay = 4 causes a scroll each 200 ms.

    if(!max7219_Matrix->isPowerOn()) return false; // do not scroll on power off
    LedMatrix_settings.scroll_iteration++;
    if (LedMatrix_settings.scroll_delay)
        LedMatrix_settings.scroll_iteration = LedMatrix_settings.scroll_iteration % LedMatrix_settings.scroll_delay;
    else
        LedMatrix_settings.scroll_iteration = 0;
    if (LedMatrix_settings.scroll_iteration)
        return false;

    return max7219_Matrix->scrollText();
}

bool MAX7291Matrix_blink(void)
{
    // This function is called every 50 ms.
    // blink_delay defines the number of cycles to be ignored until the blinkstate changes.
    if(LedMatrix_settings.blink_delay == 0) return false;

    LedMatrix_settings.blink_iteration++;
    if(LedMatrix_settings.blink_iteration == LedMatrix_settings.blink_delay)
    {
        max7219_Matrix->power(false);
    }
    else if(LedMatrix_settings.blink_iteration == 2* LedMatrix_settings.blink_delay )
    {
        LedMatrix_settings.blink_iteration = 0;
        max7219_Matrix->power(true);
    }
    return true;
}


bool MAX7291Matrix_setBlinkRate()
{
    LedMatrix_settings.blink_iteration = 0;
    max7219_Matrix->power(true);
    if (ArgC() == 0)
    {
        XdrvMailbox.payload = 0;
    }
    if (XdrvMailbox.payload)
        LedMatrix_settings.blink_delay = 20 / XdrvMailbox.payload; // 1: once per second; 2: twice per second; 3: three times per second
    else    
        LedMatrix_settings.blink_delay  = 0; // do not blink
    return true;
}

#ifdef USE_DISPLAY_MODES1TO5
// FUNC_DISPLAY_CLOCK
bool MAX7291Matrix_clock(void)
{
    LedMatrix_settings.show_clock = XdrvMailbox.payload;
    if (ArgC() == 0)
        XdrvMailbox.payload = 2;
        switch(XdrvMailbox.payload)
        {
            case 0:
                // no clock, switch to text mode
                Settings->display_mode = 0;
                return true;
            case 1:
                // 12 h clock
                LedMatrix_settings.timeFormat24 = false;
                Settings->display_mode = 1;
                break;
            case 2:
                // 24 h clock
                LedMatrix_settings.timeFormat24 = true;
                Settings->display_mode = 1;
                break;
            default:
                return false;
        }

    AddLog(LOG_LEVEL_DEBUG, PSTR("MTX: LedMatrix_settings.show_clock %d, 24h: %b"), LedMatrix_settings.show_clock, LedMatrix_settings.timeFormat24);

    max7219_Matrix->clearDisplay();
    MAX7291Matrix_showTime();
    return true;
}

// FUNC_DISPLAY_EVERY_SECOND
bool MAX7291Matrix_showTime()
{
    if(!LedMatrix_settings.show_clock) return false;

    uint8_t hr = RtcTime.hour;
    uint8_t mn = RtcTime.minute;
    uint8_t sc = RtcTime.second;
    char timeStr[10];
    if(!LedMatrix_settings.timeFormat24)
    {
        if(hr == 0) hr = 12;
        if(hr > 12 ) hr -= 12;
    }

    if(LedMatrix_settings.modulesPerRow >= 6)
    {
        snprintf(timeStr, 10, "%02d:%02d:%02d", hr , mn, sc);
    }
    else
    {
        snprintf(timeStr, 10, "%02d:%02d", hr , mn);
    }
    max7219_Matrix->drawText(timeStr, false); // false: do not clear desplay on update to prevent flicker
    return true;
}

#endif // USE_DISPLAY_MODES1TO5


bool Xdsp19(uint8_t function)
{
    bool result = false;

    if (FUNC_DISPLAY_INIT_DRIVER == function && !max2791Matrix_initDriver_done )
    {
        result = MAX7291Matrix_initDriver();
    }
    else if (max2791Matrix_initDriver_done && max7219_Matrix && (XDSP_19 == Settings->display_model))
    {
        switch (function)
        {
        case FUNC_DISPLAY_INIT:
            result = MAX7291Matrix_init();
            break;
        case FUNC_DISPLAY_POWER:
            max7219_Matrix->power(disp_power!=0);
            break;
        case FUNC_DISPLAY_MODEL:
            result = true;
            break;
        case FUNC_DISPLAY_CLEAR:
            result = max7219_Matrix->clearDisplay();
            break;
        case FUNC_DISPLAY_DIM:
            result = max7219_Matrix->setIntensity(GetDisplayDimmer16());
            break;
        case FUNC_DISPLAY_DRAW_STRING:
        case FUNC_DISPLAY_SCROLLTEXT:
        case FUNC_DISPLAY_SEVENSEG_TEXT:
            result = MAX7291Matrix_setText(true); // true: clears display before drawing text
            break;
        case FUNC_DISPLAY_SEVENSEG_TEXTNC:
            result = MAX7291Matrix_setText(false); // false: does not clear display before drawing text
            break;
        case FUNC_DISPLAY_SCROLLDELAY:
            result = MAX7291Matrix_scrollDelay();
            break;
        case FUNC_DISPLAY_BLINKRATE:
        {
            result = MAX7291Matrix_setBlinkRate();
            break;
        }
        case FUNC_DISPLAY_EVERY_50_MSECOND:
            MAX7291Matrix_blink();
            result = MAX7291Matrix_scrollText();
            break;

#ifdef USE_DISPLAY_MODES1TO5
        case FUNC_DISPLAY_CLOCK:
            result = MAX7291Matrix_clock();
            break;
        case FUNC_DISPLAY_EVERY_SECOND:
            if (LedMatrix_settings.show_clock)
            {
                result = MAX7291Matrix_showTime();
            }
            break;
#endif // USE_DISPLAY_MODES1TO5            

        default:
            result = false;
        }
    }
    return result;
}

#endif // USE_DISPLAY_MAX7219_MATRIX
#endif // USE_DISPLAY
