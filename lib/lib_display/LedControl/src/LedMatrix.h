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

#ifndef LedMatrix_h
#define LedMatrix_h

#include <LedControl.h>

#define MATRIX_MAX_MODULES 32  // maximum number of modules that can be used
#define MATRIX_BUFFER_SIZE MATRIX_MAX_MODULES * 8 // 8 bytes per modul. One byte represents 8 LEDs.
#define TEXT_BUFFER_SIZE 256 // maximum text length that can be scrolled
#define TEXT_APPEND_BUFFER_SIZE 16 // used for characters that are appended to the scroll text, before it repeats


/**
 * @brief LedMatric controls multiple 8x8 LED dot matrx modules.
 * All modules in rows and clolums together build a common display pixel matrix.
 * 
 */
class LedMatrix
{
    public:
        enum ModuleOrientation // orientation of 8x8 LED dot matrix mdules (first module starts at top left)
        { 
            ORIENTATION_NORMAL,         // first pixel is on top left, no turn is necessary
            ORIENTATION_TURN_RIGHT,     // first pixel is on bottom left, for correction it has to turn 90° right
            ORIENTATION_UPSIDE_DOWN,    // first pixel is on bottom right, fpr correction it has to turn 180°
            ORIENTATION_TURN_LEFT,      // first pixel is on top right, for correction is has to turn 90° left.
        };

    public:
        /**
         * @brief Construct a new LED Matrix object
         * 
         * @param colums of 8x8 LED dot matrix modules
         * @param rows of 8x8 LED dot matrix modules
         */
        LedMatrix(int dataPin, int clkPin, int csPin, unsigned int colums, unsigned int rows);

        /**
         * @brief Draws a string to the display.
         * When the text fits into the display, it will be shown in the center.
         * When the text is longer than than the display width, it can be scrolled per pixel with function scrollText().
         * 
         * @param str string to display
         */
        bool drawText( const char *str );

        /**
         * @brief Dwaws a character string to a defined display position. The position (x,y) is used for the upper left pixel of the text. 
         * Existing text before the x position will not be cleared. Use refresh() after all text parts are drawed.
         * 
         * @param str string to display
         * @param x horizantal pixel position to start with string (0 is most left)
         * @param y vertical pixel position for the top position of the string (0 is top)
         */
        bool drawTextAt( const char *str, const int x, const int y );

        /**
         * @brief Scroll the current text one pixel to the left.
         * Repeat with from start when end of text reached. This function can be called every 50 ms to get a propper scroll speed.
         * 
         */
        bool scrollText();

        /**
         * @brief switches the display on or off
         * 
         * @param on true: on false: off
         */

        void power( bool on );

        /**
         * @brief cleares the display and text buffer
         * 
         */
        bool clearDisplay(void);

        /**
         * @brief Set the brightness of the display
         * 
         * @param dim 0..15 (0: dark .. 15: light)
         */
        bool setIntensity(byte dim);

        /**
         * @brief Set the orientation of the 8x8 LED dot matrix module
         * 
         * @param orientation 
         */
        bool setOrientation(ModuleOrientation orientation);

        /**
         * @brief Set ap pixel at a defined position.
         * After all Pixels are set, call refresh() to send it to the display.
         * 
         * @param x horizontal position from left
         * @param y vertical position from top
         * @param on true: on, false: off 
         */

        /**
         * @brief Set the a pending string to the scrolling text to set a distance to the repeating text. Usually some spaces are used.
         * 
         * @param append text to append to the scrolling text before repeating.
         */
        bool setScrollAppendText(const char* append );

        bool setPixel( const int x, const int y, bool on=true);
        /**
         * @brief sends the changed content of the buffer to the display
         * 
         */
        void refresh();

    private:
        bool drawCharAt( char c, int x, int y ); // Draws a character to a defined position
        bool shutdown(bool b); // shutdown(true) switches the display off. Text and pixels can be set while it is off. shutdown(false) switches the display on.
        bool clear(void); // clears the display content
        void refreshByteOfBuffer( int i); // sends one byte of the buffer to the display. This updates an 8 pixel row of one matrix module.
        byte revereBitorder(byte b); // returnes the byte in the reverse bit order.
        void appendSpace(); // appends characters to the end of the text to get a distance to the repeating scroll text

    private:
        unsigned int modulesPerRow;
        unsigned int modulesPerCol;
        unsigned int displayWidth; // matrix width [pixel]
        unsigned int displayHeight; // matrix height [pixel]
        unsigned int modules; // number of 8x8 mudules
        ModuleOrientation moduleOrientation;
        byte buffer[MATRIX_BUFFER_SIZE];
        LedControl* ledControl;
        int charWidth;
        int charHeight;
        char textBuf[TEXT_BUFFER_SIZE];
        char appendTextBuf[TEXT_APPEND_BUFFER_SIZE];
        int textWidth; // width of text [pixel]
        int textPosX; // horizontal pixel position of scrolling text
        int textPosY; // vertical pixelposition of scrolling text;

};

#endif //LedMatrix_h