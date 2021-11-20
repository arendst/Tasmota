/*
 *    LedMatrix.h - Extends the Library LedControl for multiple LED dot matrix modules, based on MAX7219/MAX7221
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

    modulesPerRow = colums;
    modulesPerCol = rows;
    width = colums * 8;
    height = rows * 8;
    modules = colums * rows;
    moduleOrientation = ORIENTATION_UPSIDE_DOWN;
    ledControl = new LedControl(dataPin, clkPin, csPin, modules);
    shutdown(false); // false: on, true: off
    clear();
    setIntensity(7);
}

bool LedMatrix::clear(void)
{
    for (int i = 0; i < MATRIX_BUFFER_SIZE; i++)
    {
        buffer[i] = 0;
    }
    for (int addr = 0; addr < modules; addr++)
    {
        ledControl->clearDisplay(addr);
    }
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

bool LedMatrix::setPixel(int x, int y, bool on)
{
    if (x >= width || y >= height)
        return false;

    int modul_col = x / 8; // x pos divided by 8 is the index of the modul to the right
    int buffer_pos = modul_col + y * width / 8;
    byte buffer_byte = 8 >> (x % 8);
    if (on)
    {
        buffer[buffer_pos] |= buffer_byte; // set bit
    }
    else
    {
        buffer[buffer_pos] &= ~buffer_byte; // reset bit
    }
    refreshByteOfBuffer(buffer_pos);
    return true;
}

void LedMatrix::test()
{
    const static byte testMatrix[] PROGMEM = {
        B00000010, B00111100, B00111100, B00001000,
        B00000110, B01000010, B01000010, B00010000,
        B00001010, B00000010, B00000010, B00100000,
        B00000010, B00000100, B00001100, B01000100,
        B00000010, B00011000, B00000010, B01111110,
        B00000010, B00100000, B01000010, B00000100,
        B00000000, B11111110, B00111100, B00000100,
        B00000000, B00000000, B00000000, B00000000,
    };
    for (int i = 0; i < 32; i++)
    {
        buffer[i] = testMatrix[i];
    }
    refresh();
}

// private
bool LedMatrix::shutdown(bool b)
{
    for (int addr = 0; addr < modules; addr++)
    {
        ledControl->shutdown(addr, b); // b: false: on, true: off
    }
    return true;
}

void LedMatrix::refresh()
{
    for (int i = 0; i < modulesPerRow * height; i++)
    {
        refreshByteOfBuffer(i);
    }
}

void LedMatrix::refreshByteOfBuffer(int i)
{
    int line = i / modulesPerRow;
    int addr = line / 8 + i % modulesPerRow;
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
