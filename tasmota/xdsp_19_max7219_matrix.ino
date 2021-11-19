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
#define CMD_MAX_LEN 55


#include <LedControl.h>

LedControl *max7219_Matrix;
bool max2791Matrix_init_done = false;
byte modulesPerRow = 4;


bool MAX7291Matrix_init(void)
{
    if (!PinUsed(GPIO_MAX7219DIN) || !PinUsed(GPIO_MAX7219CLK) || !PinUsed(GPIO_MAX7219CS))
    {
        AddLog(LOG_LEVEL_INFO, PSTR("DSP: MAX7291Matrix_init GPIO pins missing DIN, CLK or CS"));
        return false; // ensure necessariy pins are configurated
    }

    Settings->display_model = XDSP_19;
    Settings->display_cols[0] = Settings->display_width;
    Settings->display_height = 1;
    Settings->display_rows = Settings->display_height;
    max7219_Matrix = new LedControl(Pin(GPIO_MAX7219DIN), Pin(GPIO_MAX7219CLK), Pin(GPIO_MAX7219CS), modulesPerRow);
    MAX7291Matrix_On(true);
    MAX7291Matrix_Clear();
    MAX7291Matrix_Dim();
    AddLog(LOG_LEVEL_INFO, PSTR("DSP: MAX7291Matrix_init"));
    max2791Matrix_init_done = true;

    max7219_Matrix->setLed(0, 3, 3, true);
    AddLog(LOG_LEVEL_INFO, PSTR("DSP: setLed 3,3"));
    return true;
}

bool MAX7291Matrix_On( bool on )
{
    for (int addr = 0; addr < modulesPerRow; addr++)
    {
        max7219_Matrix->shutdown (addr, !on); // false: on, true: off
    }
    AddLog(LOG_LEVEL_INFO, PSTR("MTX: On %d"), on);
    return true;

}

bool MAX7291Matrix_Dim(void)
{
    int dim = GetDisplayDimmer16();
    for (int addr = 0; addr < modulesPerRow; addr++)
    {
        max7219_Matrix->setIntensity(addr, dim); // 1..15
    }
    AddLog(LOG_LEVEL_INFO, PSTR("DSP: Dim %d"), dim);
    return true;
}

// /*********************************************************************************************\
// * Clears the display
// * Command:  DisplayClear
// \*********************************************************************************************/
bool MAX7291Matrix_Clear(void)
{
    for(int addr=0; addr<modulesPerRow; addr++){
        max7219_Matrix->clearDisplay(addr);
    }
    AddLog(LOG_LEVEL_INFO, PSTR("DSP: Clear"));
    return true;
}

// /*********************************************************************************************\
// * Clears the display
// * Command:  DisplayText
// \*********************************************************************************************/
bool MAX7291Matrix_Text()
{
    char sString[CMD_MAX_LEN + 1];
    subStr(sString, XdrvMailbox.data, ",", 1);
    MAX7291Matrix_Clear;
    AddLog(LOG_LEVEL_DEBUG, PSTR("MTX: sString %s"), sString);

    // test
    uint8_t length = strlen(sString);
    //if(length > modulesPerRow) 
        length = modulesPerRow;

    for(int addr = 0; addr<length; addr++ ){
        max7219_Matrix->setLed(addr, 0, 1, true);
        AddLog(LOG_LEVEL_DEBUG, PSTR("MTX: setLed() %d, 0, 1)"), addr);
    }

    return true;
}

bool Xdsp19(uint8_t function)
{
  bool result = false;


    if (FUNC_DISPLAY_INIT_DRIVER == function)
    {
        result = MAX7291Matrix_init();
    }
    else{
    //if (max2791Matrix_init_done && (XDSP_19 == Settings->display_model)) {
        switch (function) {
            case FUNC_DISPLAY_MODEL:
                result = true;
                break;
            case FUNC_DISPLAY_CLEAR:
                result = MAX7291Matrix_Clear();
                break;
            case FUNC_DISPLAY_DIM:
                result = MAX7291Matrix_Dim();
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
            case FUNC_DISPLAY_CLOCK:
            case FUNC_DISPLAY_DRAW_STRING:
                result = MAX7291Matrix_Text();
                break;
            case FUNC_DISPLAY_EVERY_50_MSECOND:
            case FUNC_DISPLAY_EVERY_SECOND:
                // ignore
                return false;
            default:
                result = false;
        }
    }
    return result;
}



#endif // USE_DISPLAY_MAX7219_MATRIX
#endif // USE_DISPLAY
