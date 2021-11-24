/*
 *    LedMatrix.h - Extends the Library LedControl for multiple 8x8 LED dot matrix modules, based on MAX7219/MAX7221
 *    Copyright (c) 2021 Michael Beuss
 * 
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 * 
 *    This permission notice shall be included in all copies or 
 *    substantial portions of the Software.
 * 
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */

#include "LedMatrix.h"
//#include "font_5x8_horizontal_MSB.h"
#include "font_6x8_horizontal_MSB.h"
//#include "font_8x8_horizontal_latin_MSB.h"

// public
LedMatrix::LedMatrix(int dataPin, int clkPin, int csPin, unsigned int colums, unsigned int rows)
{
    if (colums * rows > MATRIX_MAX_MODULES)
    {
        // dimension exeeds maximum buffer size
        if (colums >= MATRIX_MAX_MODULES)
        {
            colums = MATRIX_MAX_MODULES;
            rows = 1;
        }
        else
        {
            rows = MATRIX_MAX_MODULES / colums;
        }
    }

    charWidth = font_char_width; // defined in header file of font
    charHeight = font_char_height; // defined in header file of font
    modulesPerRow = colums;
    modulesPerCol = rows;
    displayWidth = colums * 8;
    displayHeight = rows * 8;
    modules = colums * rows;
    moduleOrientation = ORIENTATION_UPSIDE_DOWN; // use setOrientation() to turn it
    ledControl = new LedControl(dataPin, clkPin, csPin, modules); // initializes all connected LED matrix modules
    textBuf[0] = 0;
    textWidth = 0;
    textPosX = 0;
    textPosY = 0;
    appendTextBuf[0] = 0;
    setScrollAppendText("   ");
    shutdown(false); // false: on, true: off
    clear();
    setIntensity(7);
}

bool LedMatrix::drawText( const char *str)
{
    strncpy(textBuf, str, TEXT_BUFFER_SIZE -1);
    textPosX = 0;
    textPosY = 0;
    textWidth = strlen(textBuf) * charWidth;
    if(textWidth < displayWidth)
    {
        // text fits into the display, place it into the center
        clear();
        textPosX = (displayWidth - textWidth) / 2; // center
    }
    else
    {
        // The text ist longer than the display width. Scrolling is needed.
        // Append a space between end of text and the beginning of the repeting text.
        appendSpace();
    }
    drawTextAt(textBuf, textPosX, textPosY);
    refresh(); // refresh display with the new drawed string content
    return true;
}

bool LedMatrix::drawTextAt( const char *str, const int x, const int y )
{
    // draw character by character
    unsigned int len = strlen(str);
    int xPos = x;
    for (unsigned int i = 0; i < len; i++)
    {
        drawCharAt(str[i], xPos, y);
        xPos += charWidth;
    }
    return true;
}

bool LedMatrix::scrollText()
{
    if(textWidth < displayWidth) return false; // do not scroll when text fits into the display

    textPosX--;
    if(textPosX + textWidth < (int)0)
    {
        textPosX = 0; // start from the beginning after text scrolled out of display;
    }
    drawTextAt(textBuf, textPosX, textPosY);

    int startOfRepeatingTextPos = textPosX + textWidth;
    if(startOfRepeatingTextPos < displayWidth)
    {
        // draw repeating text
        drawTextAt(textBuf, startOfRepeatingTextPos, textPosY);
    }
    refresh();
    return true;
}

void LedMatrix::power(bool on)
{
    shutdown(!on); // power(false) shuts down the display with shutdown(true)
}

bool LedMatrix::clearDisplay(void)
{
    textBuf[0] = 0;
    memset(textBuf, 0, TEXT_BUFFER_SIZE);
    textWidth = 0;
    clear();
    return true;
}

bool LedMatrix::setIntensity(byte dim)
{
    for (int addr = 0; addr < modules; addr++)
    {
        ledControl->setIntensity(addr, dim); // 1..15
    }
    return true;
}

bool LedMatrix::setOrientation(ModuleOrientation orientation)
{
    moduleOrientation = orientation;
    return true;
}

bool LedMatrix::setScrollAppendText(const char* append )
{
    strncpy(appendTextBuf, append, TEXT_APPEND_BUFFER_SIZE -1);
    return (strlen(append) < TEXT_APPEND_BUFFER_SIZE);
}

bool LedMatrix::setPixel(const int x, const int y, bool on)
{
    if (x >= displayWidth || y >= displayHeight)
        return false;

    int modul_col = x / 8; // x pos divided by 8 is the index of the modul to the right
    int buffer_pos = modul_col + y * modulesPerRow;
    byte buffer_byte = 0x80 >> (x % 8);
    if (on)
    {
        buffer[buffer_pos] |= buffer_byte; // set bit
    }
    else
    {
        buffer[buffer_pos] &= ~buffer_byte; // reset bit
    }
    return true;
}

void LedMatrix::refresh()
{
    for (int i = 0; i < modulesPerRow * displayHeight; i++)
    {
        refreshByteOfBuffer(i);
    }
}

// private functions
bool LedMatrix::drawCharAt( char c, const int x, const int y)
{
    // ignore when the character position is not visible on the display
    bool visible = (
        x > 0 - (int)charWidth  && x < (int)displayWidth &&
        y > 0 - (int)charHeight && y < (int)displayHeight
    );
    if (!visible) return false;

    // ignore the leading bits above charWidth of the font definition
    static const byte charOffset = 8 - charWidth;

    for (byte charY = 0; charY < charHeight; charY++)
    {
        char pixelRow = (font[c][charY]) << charOffset; // skip the first bits when the character width is smaller than 8 pixel
        for (byte charX = 0; charX < charWidth; charX++)
        {
            bool pixel = (pixelRow & 0x80); // pixel=true when upper bit is set
            setPixel(x + charX, y + charY, pixel);
            pixelRow = pixelRow << 1; // next pixel
        }
    }
    return true;
}

bool LedMatrix::shutdown(bool b)
{
    for (int addr = 0; addr < modules; addr++)
    {
        ledControl->shutdown(addr, b); // b: false: on, true: off
    }
    return true;
}

bool LedMatrix::clear(void)
{
    memset(buffer, 0, MATRIX_BUFFER_SIZE);
    for (int addr = 0; addr < modules; addr++)
    {
        ledControl->clearDisplay(addr);
    }
    return true;
}

void LedMatrix::refreshByteOfBuffer(int i)
{
    int line = i / modulesPerRow;
    int addr = (line / 8) * modulesPerRow + i % modulesPerRow;
    byte b = buffer[i];
    if (moduleOrientation == ORIENTATION_NORMAL || moduleOrientation == ORIENTATION_UPSIDE_DOWN)
    {
        int rowOfAddr = 0;
        if (moduleOrientation == ORIENTATION_NORMAL)
        {
            rowOfAddr = line % 8; // ORIENTATION_NORMAL
        }
        else
        {
            rowOfAddr = 7 - line % 8; // ORIENTATION_UPSIDE_DOWN
            b = revereBitorder(b);
        }
        ledControl->setRow(addr, rowOfAddr, b);
    }
    else
    {
        // ORIENTATION_TURN_RIGHT or ORIENTATION_TURN_LEFT
        int colOfAddr = 0;
        if (moduleOrientation == ORIENTATION_TURN_LEFT)
        {
            colOfAddr = line % 8; // ORIENTATION_TURN_LEFT
        }
        else
        {
            colOfAddr = 7 - line % 8; // ORIENTATION_TURN_RIGHT
            b = revereBitorder(b);
        }
        ledControl->setColumn(addr, colOfAddr, b);
    }
}

byte LedMatrix::revereBitorder (byte b)
{
    const static byte lookup[] PROGMEM = {
        0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe,
        0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf, 
    };
    return (lookup[b & 0b1111] << 4) | lookup[b >> 4];
}

void LedMatrix::appendSpace()
{
    strncat(textBuf, appendTextBuf, TEXT_BUFFER_SIZE -1);
    textWidth = strlen(textBuf) * charWidth;
}
