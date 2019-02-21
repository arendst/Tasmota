/**
 *  @filename   :   epdif.h
 *  @brief      :   Header file of epdif.cpp providing EPD interface functions
 *                  Users have to implement all the functions in epdif.cpp
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     August 10 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef EPDIF_H
#define EPDIF_H

#include <Arduino.h>

/*
SPI Pin definition
Default pins:
  SCK             18
  MISO            19
  MOSI            23
  SS               5
  Use default pin -1
*/
/*
#define SCK_PIN   -1
#define MISO_PIN   4
#define MOSI_PIN   5
#define SS_PIN     2

// EPD Pin definition
#define RST_PIN   23
#define DC_PIN    21
#define CS_PIN    19
#define BUSY_PIN  22
*/

#define CS_PIN 5
#define MOSI_PIN 0
#define SCLK_PIN 4


class EpdIf {
public:
    EpdIf(void);
    ~EpdIf(void);
    static void DelayMs(unsigned int delaytime);
    static int  DigitalRead(int pin);
    /*
    static int  IfInit(void);
    static void DigitalWrite(int pin, int value);


    static void SpiTransfer(unsigned char data);
    */
    //static void fastSPIwrite(uint8_t d,uint8_t dc);
};

#endif
