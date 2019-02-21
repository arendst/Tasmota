/**
 *  @filename   :   epdif.cpp
 *  @brief      :   Implements EPD interface functions
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

#include "epdif.h"
#include <SPI.h>

EpdIf::EpdIf() {
};

EpdIf::~EpdIf() {
};

void EpdIf::DelayMs(unsigned int delaytime) {
    delay(delaytime);
}

int EpdIf::DigitalRead(int pin) {
    return digitalRead(pin);
}

/*
void EpdIf::DigitalWrite(int pin, int value) {
    digitalWrite(pin, value);
}





void EpdIf::SpiTransfer(unsigned char data) {
    digitalWrite(cs_pin, LOW);
    //SPI.transfer(data);
    digitalWrite(cs_pin, HIGH);
}



int EpdIf::IfInit(void) {


    pinMode(cs_pin, OUTPUT);
    pinMode(mosi_pin, OUTPUT);
    pinMode(mosi_pin, OUTPUT);
    //pinMode(RST_PIN, OUTPUT);
    //pinMode(DC_PIN, OUTPUT);
    //pinMode(BUSY_PIN, INPUT);
    //SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
    //SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
    return 0;
}


void EpdIf::fastSPIwrite(uint8_t d,uint8_t dc) {

  digitalWrite(cs_pin, LOW);

  // transfer dc
  digitalWrite(sclk_pin, LOW);
  if(dc) digitalWrite(mosi_pin, HIGH);
  else        digitalWrite(mosi_pin, LOW);
  digitalWrite(sclk_pin, HIGH);

  for(uint8_t bit = 0x80; bit; bit >>= 1) {
    digitalWrite(sclk_pin, LOW);
    if(d & bit) digitalWrite(mosi_pin, HIGH);
    else        digitalWrite(mosi_pin, LOW);
    digitalWrite(sclk_pin, HIGH);
  }

  digitalWrite(cs_pin, HIGH);
}
*/
