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
#include "font_6x8_base.h"
//#include "font_6x8_UTF8_C2.h" // additional characters if needed
//#include "font_6x8_UTF8_C3.h" // additional characters (latin1) if needed
#include "../../../../tasmota/my_user_config.h" // to check compiler option USE_UTF8_LATIN1
#ifdef USE_UTF8_LATIN1
    #include "font_6x8_UTF8_C2.h" // 256 bytes
    #include "font_6x8_UTF8_C3.h" // 512 bytes
#endif


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
    powerIsOn = false;

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
    textLen = countChars(str);
    textWidth = textLen * charWidth;
    if(textWidth <= displayWidth)
    {
        // text fits into the display, place it into the center
        textPosX = (displayWidth - textWidth) / 2; // center
    }
    else
    {
        // The text ist longer than the display width. Scrolling is needed.
        // Add a space in front of text to have a distance to the pervious scroll text.
        addSpace();
    }
    drawTextAt(textBuf, textPosX, textPosY);
    refresh(); // refresh display with the new drawed string content
    return true;
}

bool LedMatrix::drawTextAt( const char *str, const int x, const int y )
{
    // draw character by character
    int xPos = x;
    const char* fontChar = nullptr;
    for (unsigned int i = 0; (i<TEXT_BUFFER_SIZE && str[i]!=0); i++)
    {
        char c = str[i];
        fontChar = font_20_7F[char('_') - 0x20]; // default character in case of non printable or undefined
        if( c >= 0x20 && c < 0x80) // basic font
        {
            fontChar = font_20_7F[c-0x20];
        }

#ifdef font_6x8_UTF8_C2_h
        else if(c == 0xC2) // UTF special characters
        {
            i++;
            c= str[i];
            if(c>= 0xA0 && c < 0xC0)
            {
                fontChar = font_UTF_C2_A0_BF[c - 0xA0];
            }
        }
#endif // font_6x8_UTF8_C2_h

#ifdef font_6x8_UTF8_C3_h
        else if(c == 0xC3) // UTF latin1
        {
            i++;
            c= str[i];
            if(c>= 0x80 && c < 0xC0)
            {
                fontChar = font_UTF_C3_80_BF[c - 0x80];
            }
        }
#endif // font_6x8_UTF8_C3_h

        else if(c>= 0xC0 && c <= 0xDF)
        {
            i += 1;  // 2 byte UTF sequence
        }
        else if(c>= 0xE0 && c <= 0xEF)
        {
            i += 2; // 3 byte UTF sequence
        }
        else if(c>= 0xF0 && c <= 0xF7)
        {
            i += 3; // 4 byte UTF sequence
        }

        drawCharAt(fontChar, xPos, y);
        xPos += charWidth;
    }
    return true;
}

int LedMatrix::countChars( const char* utfText)
{
    int len = 0;
    for( int i = 0; (i<TEXT_BUFFER_SIZE && utfText[i]!=0); i++)
    {
        char c = utfText[i];
        if( c < 0xC0) 
        {
            // 1 byte UTF sequence
        }
        else if(c <= 0xDF)
        {
            i += 1;  // 2 byte UTF sequence
        }
        else if(c <= 0xEF)
        {
            i += 2; // 3 byte UTF sequence
        }
        else if(c <= 0xF7)
        {
            i += 3; // 4 byte UTF sequence
        }
        len++;
    }
    return len;
}

bool LedMatrix::scrollText()
{
    if(textWidth <= displayWidth) return false; // do not scroll when text fits into the display

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
    powerIsOn = on;
    byte value = 0; // 0: shutdown
    if(on) value = 1; // 1: power on
    spiTransfer_value(OP_SHUTDOWN, value); // power(false) shuts down the display
}

bool LedMatrix::isPowerOn()
{
    return powerIsOn;
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
                        deviceDataBuff[addr] = revereBitorder(buffer[bufPos]); // mirror
                        deviceRow = 7 - ledRow;  // upside down
                    }
                    else
                    {
                        // ORIENTATION_UPSIDE_DOWN
                        deviceDataBuff[maxDevices -1 - addr] = buffer[bufPos];
                        deviceRow = ledRow;
                    }
            }
            else // ORIENTATION_TURN_RIGHT || ORIENTATION_TURN_LEFT
            {
                col = addr % modulesPerRow;
                pixelRow = (addr / modulesPerRow) * 8 + ledRow;
                bufPos = pixelRow * modulesPerRow + col;

                if (moduleOrientation == ORIENTATION_TURN_RIGHT)
                {
                    // ORIENTATION_TURN_RIGHT
                    deviceDataBuff[addr] = buffer[bufPos];
                    deviceRow = ledRow;
                }
                else
                {
                    // ORIENTATION_TURN_LEFT
                    deviceDataBuff[maxDevices - 1 - addr] = revereBitorder(buffer[bufPos]);
                    deviceRow = 7 - ledRow; // upside down
                }
            }
        }
        setRow_allDevices(deviceRow, deviceDataBuff); 
    }
}

// private functions
/**
 * @brief 
 * 
 * @param fontChar defines the pixelrows of a character. const char fontChar[charHeight]
 * @param x 
 * @param y 
 */
bool LedMatrix::drawCharAt( const char* fontChar, const int x, const int y)
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
        char pixelRow = (fontChar[charY]) << charOffset; // skip the first bits when the character width is smaller than 8 pixel
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

void LedMatrix::addSpace()
{
    strncat(textBuf, appendTextBuf, TEXT_BUFFER_SIZE -1);
    textPosX = strlen(appendTextBuf) * charWidth; // start scrolling with space
    textLen = countChars(textBuf);
    textWidth = countChars(textBuf) * charWidth;
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



