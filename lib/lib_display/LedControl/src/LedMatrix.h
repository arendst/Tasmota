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

#ifndef LedMatrix_h
#define LedMatrix_h

#include <LedControl.h>

#define MATRIX_MAX_MODULES 32
#define MATRIX_BUFFER_SIZE MATRIX_MAX_MODULES * 8 // 8 bytes per modul. One byte represents 8 LEDs.


/**
 * @brief LedMatric controls multiple 8x8 LED dot matrx modules.
 * All modules in rows and clolums together build a common matrix.
 * 
 */
class LedMatrix
{
    public:
        enum ModuleOrientation // orientation of 8x8 LED dot matrix mdules (first module is top left)
        { 
            ORIENTATION_NORMAL,         // first pixel is on top left, no turn is necessary
            ORIENTATION_TURN_RIGHT,     // first pixel is on bottom left, for correction it has to turn 90° right
            ORIENTATION_UPSIDE_DOWN,    // first pixel is on bottom right, fpr correction it has to turn 180°
            ORIENTATION_TURN_LEFT,      // first pixel is on top right, for correction is has to turn 90° left.
        };

    private:
        unsigned int modulesPerRow;
        unsigned int modulesPerCol;
        unsigned int width; // matrix width [pixel]
        unsigned int height; // matrix height [pixel]
        unsigned int modules; // number of 8x8 mudules
        ModuleOrientation moduleOrientation;
        byte buffer[MATRIX_BUFFER_SIZE];
        LedControl* ledControl;


    public:
        /**
         * @brief Construct a new Led Matrix object
         * 
         * @param colums of 8x8 LED dot matrix modules
         * @param rows of 8x8 LED dot matrix modules
         */
        LedMatrix(int dataPin, int clkPin, int csPin, unsigned int colums, unsigned int rows);

        /**
         * @brief Set all LEDs off.
         * 
         */
        bool clear(void);

        /**
         * @brief Set the brightness of the display
         * 
         * @param dim 0..15 
         */
        bool setIntensity(byte dim);

        /**
         * @brief Switches the display on or off
         * 
         * @param on true: on false: off
         */
        void power( bool on );

        /**
         * @brief Set the orientation of the 8x8 LED dot matrix module
         * 
         * @param orientation 
         */
        bool setOrientation(ModuleOrientation orientation);

        /**
         * @brief Set the Pixel object
         * 
         * @param x horizontal position from left
         * @param y vertical position from top
         * @param on true: on, false: off 
         */
        bool setPixel( int x, int y, bool on);
        void test();

    private:
        bool shutdown(bool b);

        /**
         * @brief sends the changed content of the buffer to the display
         * 
         */
        void refresh();

        void refreshByteOfBuffer( int i);

        byte revereBitorder (byte b);

};

#endif //LedMatrix_h