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

  Depending on order oth the wired 8x8 matrix modules you have got a display of size pixel_width x pixel_height.
  The size has to be set with the commands "DisplayWidth <pixel_width>" and "DisplayHeight <pixel_height>"

  After the ESP8266/ESP32 module restarts again, turn ON the display with the command "Power 1"


  Now, the following "Display" commands can be used:

  DisplayText  text 
    Sends the text to the display. 
    If the text fits into the display, it is shown in the center.
    Otherwise it scrolls to the left and repeats as long it is cleared or new "DisplayText" overwrites it.

  DisplayDimmer [0..100]
    Sets the intensity of the display.

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

  DisplayClock  [0|1|2]
    Displays a clock.
    Commands "DisplayClock 1"     // 12 hr format
            "DisplayClock 2"     // 24 hr format
            "DisplayClock 0"     // turn off clock



\*********************************************************************************************/

#define XDSP_19 19

#include <LedMatrix.h>

#ifdef USE_DISPLAY_MODES1TO5
#include <time.h>
#endif

LedMatrix *max7219_Matrix = nullptr;
bool max2791Matrix_initDriver_done = false;
struct
{
    byte modulesPerRow = 4;
    byte modulesPerCol = 1;
    byte scroll_delay = 0;
    byte scroll_iteration = 0;
    bool show_clock = false;
    const char *timeFormat;

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
    if (Settings->display_width) // [pixel]
    {
        LedMatrix_settings.modulesPerRow = (Settings->display_width - 1) / 8 + 1;
    }
    Settings->display_width = 8 * LedMatrix_settings.modulesPerRow;
    Settings->display_cols[0] = Settings->display_width;
    if (Settings->display_height) // [pixel]
    {
        LedMatrix_settings.modulesPerCol = (Settings->display_height - 1) / 8 + 1;
    }
    Settings->display_height = 8 * LedMatrix_settings. modulesPerCol;
    Settings->display_rows = Settings->display_height;
    Settings->display_cols[1] = Settings->display_height;
    max7219_Matrix = new LedMatrix(Pin(GPIO_MAX7219DIN), Pin(GPIO_MAX7219CLK), Pin(GPIO_MAX7219CS), LedMatrix_settings.modulesPerRow, LedMatrix_settings.modulesPerCol);
    max2791Matrix_initDriver_done = true;

    return MAX7291Matrix_init();
}

// FUNC_DISPLAY_INIT
bool MAX7291Matrix_init(void)
{
    int intensity = GetDisplayDimmer16(); // 0..15
    max7219_Matrix->setIntensity(intensity);
    int orientation = Settings->display_rotate;
    max7219_Matrix->setOrientation((LedMatrix::ModuleOrientation)orientation );
    AddLog(LOG_LEVEL_INFO, PSTR("MTX: MAX7291Matrix_init %dx%d modules, orientation: %d, intensity: %d"), LedMatrix_settings.modulesPerRow , LedMatrix_settings.modulesPerCol, orientation, intensity);

    //max7219_Matrix->test();
    AddLog(LOG_LEVEL_INFO, PSTR("MTX: display test"));
    return true;
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
    LedMatrix_settings.scroll_iteration++;
    if (LedMatrix_settings.scroll_delay)
        LedMatrix_settings.scroll_iteration = LedMatrix_settings.scroll_iteration % LedMatrix_settings.scroll_delay;
    else
        LedMatrix_settings.scroll_iteration = 0;
    if (LedMatrix_settings.scroll_iteration)
        return false;

    return max7219_Matrix->scrollText();
}

#ifdef USE_DISPLAY_MODES1TO5
// FUNC_DISPLAY_CLOCK
bool MAX7291Matrix_clock(void)
{
    LedMatrix_settings.show_clock = XdrvMailbox.payload;
    if (ArgC() == 0)
        XdrvMailbox.payload = 1;
    if (XdrvMailbox.payload > 1)
    {
        LedMatrix_settings.timeFormat = "%H:%M";
        if(LedMatrix_settings.modulesPerRow > 6)
        {
            LedMatrix_settings.timeFormat = "%H:%M:%S";
        }
        XdrvMailbox.payload = 2;
    }
    else
    {
        LedMatrix_settings.timeFormat = "%I:%M";
        if(LedMatrix_settings.modulesPerRow > 6)
        {
            LedMatrix_settings.timeFormat = "%I:%M:%S";
        }
        XdrvMailbox.payload = 1;
    }

    AddLog(LOG_LEVEL_DEBUG, PSTR("MTX: LedMatrix_settings.show_clock %d, timeFormat %s"), LedMatrix_settings.show_clock, LedMatrix_settings.timeFormat);

    MAX7291Matrix_showTime();
    return true;
}

// FUNC_DISPLAY_EVERY_SECOND
bool MAX7291Matrix_showTime()
{
    time_t rawtime;
    struct tm *timeinfo;
    char timeStr[10];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timeStr, 10, LedMatrix_settings.timeFormat, timeinfo);

    max7219_Matrix->drawText(timeStr);
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
            result = max7219_Matrix->drawText(dsp_str);
            break;
        case FUNC_DISPLAY_SCROLLDELAY:
            result = MAX7291Matrix_scrollDelay();
            break;
        case FUNC_DISPLAY_EVERY_50_MSECOND:
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
