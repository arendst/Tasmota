/*
  xdsp_19_max7219_matrix.ino.ino - Support for MAX7219 based dot matrix displays for Tasmota

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
  This driver enables the display of ascii text on MAX7219 based LED dot matrix modules.

  Connect the MAX7219 display module's pins to any free GPIOs of the ESP8266 or ESP32 module
  and assign the pins as follows from Tasmota's GUI:

  DIN hardware pin --> "MAX7219 DIN"
  CS hardware pin --> "MAX7219 CS"
  CLK hardware pin --> "MAX7219 CLK"

  Once the GPIO configuration is saved and the ESP8266/ESP32 module restarts,
  set the Display Model to 19 and Display Mode to 0
  using the command "Backlog DisplayModel 15 ; DisplayMode 0"

  If your display is a TM1637 with 6 digits, set Display Width to the number of digits your
  display has, using the command "DisplayWidth 6".

  After the ESP8266/ESP32 module restarts again, turn ON the display with the command "Power 1"

  Now, the following "Display" commands can be used:


  DisplayClear

                               Clears the display, command: "DisplayClear"


  DisplayNumber         num [,position {0-(Settings->display_width-1))} [,leading_zeros {0|1} [,length {1 to Settings->display_width}]]]

                               Clears and then displays number without decimal. command e.g., "DisplayNumber 1234"
                               Control 'leading zeros', 'length' and 'position' with  "DisplayNumber 1234, <position>, <leadingZeros>, <length>"
                               'leading zeros' can be 1 or 0 (default), 'length' can be 1 to Settings->display_width, 'position' can be 0 (left-most) to Settings->display_width (right-most).
                               See function description below for more details.

  DisplayNumberNC       num [,position {0-(Settings->display_width-1))} [,leading_zeros {0|1} [,length {1 to Settings->display_width}]]]

                               Display integer number as above, but without clearing first. e.g., "DisplayNumberNC 1234". Usage is same as above.



  DisplayFloat          num [,position {0-(Settings->display_width-1)} [,precision {0-Settings->display_width} [,length {1 to Settings->display_width}]]]

                               Clears and then displays float (with decimal point)  command e.g., "DisplayFloat 12.34"
                               See function description below for more details.



  DisplayFloatNC        num [,position {0-(Settings->display_width-1)} [,precision {0-Settings->display_width} [,length {1 to Settings->display_width}]]]

                               Displays float (with decimal point) as above, but without clearing first. command e.g., "DisplayFloatNC 12.34"
                               See function description below for more details.



  DisplayRaw            position {0-(Settings->display_width-1)},length {1 to Settings->display_width}, num1 [, num2[, num3[, num4[, ...upto Settings->display_width numbers]]]]]

                               Takes upto Settings->display_width comma-separated integers (0-255) and displays raw segments. Each number represents a
                               7-segment digit. Each 8-bit number represents individual segments of a digit.
                               For example, the command "DisplayRaw 0, 4, 255, 255, 255, 255" would display "[8.8.8.8.]"



  DisplayText           text [, position {0-(Settings->display_width-1)} [,length {1 to Settings->display_width}]]

                               Clears and then displays basic text.  command e.g., "DisplayText ajith vasudevan"
                               Control 'length' and 'position' with  "DisplayText <text>, <position>, <length>"
                               'length' can be 1 to Settings->display_width, 'position' can be 0 (left-most) to Settings->display_width-1 (right-most)
                               A caret(^) symbol in the text input is dispayed as the degrees(°) symbol. This is useful for displaying Temperature!
                               For example, the command "DisplayText 22.5^" will display "22.5°".


  DisplayTextNC         text [, position {0-Settings->display_width-1} [,length {1 to Settings->display_width}]]

                               Clears first, then displays text. Usage is same as above.



  DisplayScrollText     text [, num_loops]

                              Displays scrolling text indefinitely, until another Display- command (other than DisplayScrollText 
                              or DisplayScrollDelay is issued). Optionally, stop scrolling after num_loops iterations.



  DisplayScrollDelay delay {0-15}   // default = 4

                               Sets the speed of text scroll. Smaller delay = faster scrolling.



  DisplayLevel          num {0-100}

                               Display a horizontal bar graph (0-100) command e.g., "DisplayLevel 50" will display [||||    ]



  DisplayClock  1|2|0

                               Displays a clock.
                               Commands "DisplayClock 1"     // 12 hr format
                                        "DisplayClock 2"     // 24 hr format
                                        "DisplayClock 0"     // turn off clock


In addition, setting DisplayMode to 1 shows the time, setting it to 2 shows the date
and setting it to 3 alternates between time and date.



\*********************************************************************************************/

#define XDSP_19 19

#include <time.h> 
#include <LedMatrix.h>

LedMatrix *max7219_Matrix = nullptr;
bool max2791Matrix_initDriver_done = false;
struct
{
    byte modulesPerRow = 4;
    byte modulesPerCol = 1;
    bool show_clock = false;
    const char* timeFormat;

} LedMatrix_settings;

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

    //max7219_Matrix->clearDisplay();
    MAX7291Matrix_showTime();
    return true;
}

bool MAX7291Matrix_showTime()
{
    time_t rawtime;
    struct tm *timeinfo;
    char timeStr[10];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timeStr, 10, LedMatrix_settings.timeFormat, timeinfo);
    //AddLog(LOG_LEVEL_DEBUG, PSTR("MTX: showTime:%s"), timeStr);

    max7219_Matrix->drawText(timeStr);
    return true;
}

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
        case FUNC_DISPLAY_CLOCK:
            result = MAX7291Matrix_clock();
            break;
        case FUNC_DISPLAY_SEVENSEG_TEXT:
        case FUNC_DISPLAY_SEVENSEG_TEXTNC:
        case FUNC_DISPLAY_NUMBER:
        case FUNC_DISPLAY_NUMBERNC:
        case FUNC_DISPLAY_FLOAT:
        case FUNC_DISPLAY_FLOATNC:
        case FUNC_DISPLAY_RAW:
        case FUNC_DISPLAY_LEVEL:
        case FUNC_DISPLAY_SCROLLTEXT:
        case FUNC_DISPLAY_DRAW_STRING:
            result = max7219_Matrix->drawText(dsp_str);
            break;
        case FUNC_DISPLAY_EVERY_SECOND:
            if (LedMatrix_settings.show_clock)
            {
                result = MAX7291Matrix_showTime();
            }

            break;
        case FUNC_DISPLAY_EVERY_50_MSECOND:
            result = max7219_Matrix->scrollText();
        default:
            result = false;
        }
    }
    return result;
}

#endif // USE_DISPLAY_MAX7219_MATRIX
#endif // USE_DISPLAY
