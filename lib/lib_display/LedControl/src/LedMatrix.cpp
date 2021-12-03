/*
 *    LedMatrix.h - Extends the Library LedControl for multiple 8x8 LED dot matrix maxDevices, based on MAX7219/MAX7221
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

//the opcodes for the MAX7221 and MAX7219
#define OP_NOOP   0
#define OP_DIGIT0 1
#define OP_DIGIT1 2
#define OP_DIGIT2 3
#define OP_DIGIT3 4
#define OP_DIGIT4 5
#define OP_DIGIT5 6
#define OP_DIGIT6 7
#define OP_DIGIT7 8
#define OP_DECODEMODE  9
#define OP_INTENSITY   10
#define OP_SCANLIMIT   11
#define OP_SHUTDOWN    12
#define OP_DISPLAYTEST 15

// test
#include "LedControl.h"
LedControl* ledControl = nullptr;
// end text
LedMatrix::LedMatrix(int dataPin, int clkPin, int csPin, unsigned int colums, unsigned int rows)
{
    if (colums * rows > MAX72XX_MAX_DEVICES)
    {
        // dimension exeeds maximum buffer size
        if (colums >= MAX72XX_MAX_DEVICES)
        {
            colums = MAX72XX_MAX_DEVICES;
            rows = 1;
        }
        else
        {
            rows = MAX72XX_MAX_DEVICES / colums;
        }
    }

    charWidth = font_char_width; // defined in header file of font
    charHeight = font_char_height; // defined in header file of font
    modulesPerRow = colums;
    modulesPerCol = rows;
    displayWidth = colums * 8;
    displayHeight = rows * 8;
    maxDevices = colums * rows;
    moduleOrientation = ORIENTATION_UPSIDE_DOWN; // use setOrientation() to turn it
    textBuf[0] = 0;
    textWidth = 0;
    textPosX = 0;
    textPosY = 0;
    appendTextBuf[0] = 0;
    setScrollAppendText("   ");

    // initialize all connected MAX7219/MAX7221 devices
    SPI_MOSI = dataPin;
    SPI_CLK = clkPin;
    SPI_CS = csPin;
    pinMode(SPI_MOSI, OUTPUT);
    pinMode(SPI_CLK, OUTPUT);
    pinMode(SPI_CS, OUTPUT);
    SPI_MOSI = dataPin;

    //spiTransfer_value(OP_DISPLAYTEST, 0); // display test
    spiTransfer_value(OP_SCANLIMIT, 7); // scanlimit is set to max on startup
    spiTransfer_value(OP_DECODEMODE, 0); // decode is done in source
    clearDisplay();
    //spiTransfer_value(OP_SHUTDOWN, 0); //we go into shutdown-mode (LEDs off) on startup
    setIntensity(7); // initialize with the half of the maximum intensity [0..15]
    power(true); // power on;
}

bool LedMatrix::drawText( const char *str, bool clearBefore)
{
    if(clearBefore) clearDisplay();
    strncpy(textBuf, str, TEXT_BUFFER_SIZE -1);
    textPosX = 0;
    textPosY = 0;
    textWidth = strlen(textBuf) * charWidth;
    if(textWidth < displayWidth)
    {
        // text fits into the display, place it into the center
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
    byte value = 0; // 0: shutdown
    if(on) value = 1; // 1: power on
    spiTransfer_value(OP_SHUTDOWN, value); // power(false) shuts down the display
}

bool LedMatrix::clearDisplay(void)
{
    memset(textBuf, 0, TEXT_BUFFER_SIZE);
    textWidth = 0;
    memset(buffer, 0, MATRIX_BUFFER_SIZE);
    for (int row = 0; row < 8; row++)
    {
        spiTransfer_value(row + 1, 0);
    }
    return true;
}

bool LedMatrix::setIntensity(byte intensity)
{
    if (intensity < 0 || intensity > 15)
        return false;

    spiTransfer_value(OP_INTENSITY, intensity);
    return true;
}

bool LedMatrix::setOrientation(LedMatrix::ModuleOrientation orientation)
{
    if(moduleOrientation != orientation)
    {
        moduleOrientation = orientation;
        refresh();
    }
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
    int col = 0;
    int pixelRow = 0;
    int bufPos = 0;
    int deviceRow = 0;
    for(int ledRow = 7; ledRow >= 0; ledRow--) // refresh from buttom to top
    {
        for( int addr = 0; addr < maxDevices; addr++)
        {
            if(moduleOrientation == ORIENTATION_NORMAL || moduleOrientation == ORIENTATION_UPSIDE_DOWN)
            {
                    col = addr % modulesPerRow;
                    pixelRow = (addr / modulesPerRow) * 8 + ledRow;
                    bufPos = pixelRow * modulesPerRow + col;
                    if(moduleOrientation == ORIENTATION_NORMAL)
                    {
                        // ORIENTATION_NORMAL
                        deviceDataBuff[addr] = buffer[bufPos];
                        deviceRow = ledRow;
                    }
                    else
                    {
                        // ORIENTATION_UPSIDE_DOWN
                        deviceDataBuff[addr] = revereBitorder(buffer[bufPos]); // mirror
                        deviceRow = 7 - ledRow;  // upside down
                    }
            }
        }
        setRow_allDevices(deviceRow, deviceDataBuff); 
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

byte LedMatrix::revereBitorder (byte b)
{
    static const byte lookup[16] = {
        0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe,
        0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf
    };
    return (lookup[b & 0b1111] << 4) | lookup[b >> 4];
}

void LedMatrix::appendSpace()
{
    strncat(textBuf, appendTextBuf, TEXT_BUFFER_SIZE -1);
    textWidth = strlen(textBuf) * charWidth;
}

void LedMatrix::setRow_allDevices(int row, byte *data)
{
    if (row < 0 || row > 7)
        return;
    spiTransfer_array(row + 1, data);
}

void LedMatrix::spiTransfer_array(byte opcode, const byte* data) {
    // create an array with the data to shift out
    for (int addr = 0; addr < maxDevices; addr++)
    {
        spidata[addr * 2 + 1] = opcode;
        spidata[addr * 2] = data[addr];
    }
    // enable the line
    digitalWrite(SPI_CS, LOW);
    // shift out the data
    for (int i = maxDevices * 2 -1; i >= 0; i--)
    {
        shiftOut(SPI_MOSI, SPI_CLK, MSBFIRST, spidata[i]);
    }
    // latch the data onto the display
    digitalWrite(SPI_CS, HIGH);
}

void LedMatrix::spiTransfer_value(byte opcode, byte value)
{
    memset(deviceDataBuff, (byte)value, maxDevices);
    spiTransfer_array(opcode, deviceDataBuff);
}



