/**
 *  @filename   :   epd2in9-demo.ino
 *  @brief      :   2.9inch e-paper display demo
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     September 9 2017
 *
 *   @author     :   Updated by krzychb@gazeta,pl
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

#include <SPI.h>
#include <epd2in9.h>
#include <epdpaint.h>
#include "imagedata.h"

#define COLORED     0
#define UNCOLORED   1

unsigned char image[4736];
Paint paint(image, 0, 0);    // width should be the multiple of 8 
Epd epd;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");
  if (epd.Init(lut_full_update) != 0) {
      Serial.println("e-Paper init failed");
      return;
  }
  Serial.println("Init done");
}

void loop() {

  // Clear image memory
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  Serial.println("Cleared frame memory");

  // Welcome text
  paint.SetRotate(ROTATE_270);
  paint.SetWidth(128);
  paint.SetHeight(296);
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(50, 50, "Hello world!", &Font24, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
  Serial.println("Displayed welcome text");
  delay(3000);

  epd.SetFrameMemory(IMAGE_DATA);
  epd.DisplayFrame();
  Serial.println("Displayed image data");
  delay(3000);

  // Black screen
  epd.ClearFrameMemory(0);
  epd.DisplayFrame();
  Serial.println("Displayed black screen");
  delay(3000);

  epd.Reset();
}

