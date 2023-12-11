/*
   MIT License

  Copyright (c) 2021 Felix Biego

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#ifndef CST816S_H
#define CST816S_H

#include <Arduino.h>

#define CST816S_address 0x15

enum GESTURE
{
    NONE = 0x00,
    SWIPE_UP = 0x01,
    SWIPE_DOWN = 0x02,
    SWIPE_LEFT = 0x03,
    SWIPE_RIGHT = 0x04,
    SINGLE_CLICK = 0x05,
    DOUBLE_CLICK = 0x0B,
    LONG_PRESS = 0x0C

};

struct data_struct
{
    byte gestureID; // Gesture ID
    byte points;    // Number of touch points
    byte event;     // Event (0 = Down, 1 = Up, 2 = Contact)
    int x;
    int y;
    uint8_t version;
    uint8_t versionInfo[3];
};

class CST816S
{

public:
    CST816S(TwoWire *use_wire, int8_t irq = 0, int8_t rst = 1);
    void begin(int interrupt = RISING);
    void sleep();
    bool available();
    data_struct data;
    String gesture();

private:
    TwoWire *wire;
    int8_t _rst;
    int8_t _irq;
    bool _event_available;

    void IRAM_ATTR handleISR();
    void read_touch();

    uint8_t i2c_read(uint16_t addr, uint8_t reg_addr, uint8_t *reg_data, size_t length);
    uint8_t i2c_write(uint8_t addr, uint8_t reg_addr, const uint8_t *reg_data, size_t length);
};

#endif