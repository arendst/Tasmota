/////////////////////////////////////////////////////////////////
/*
MIT License

Copyright (c) 2019 lewis he

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

FT5206.h - Arduino library for FT5206 chip.
Created by Lewis on April 17, 2019.
github:https://github.com/lewisxhe/FT5206_Library
*/
/////////////////////////////////////////////////////////////////
#pragma once

#include <Arduino.h>
#include <Wire.h>

#define FT5206_SLAVE_ADDRESS    (0x38)
#define FT5206_MODE_REG         (0x00)
#define FT5206_TOUCHES_REG      (0x02)
#define FT5206_VENDID_REG       (0xA8)
#define FT5206_CHIPID_REG       (0xA3)
#define FT5206_THRESHHOLD_REG   (0x80)
#define FT5206_POWER_REG        (0x87)

#define FT5206_MONITOR         (0x01)
#define FT5206_SLEEP_IN        (0x03)

#define FT5206_VENDID           0x11
#define FT6206_CHIPID           0x06
#define FT6236_CHIPID           0x36
#define FT6236U_CHIPID          0x64
#define FT5206U_CHIPID          0x64

#define FT5316_CHIPID           0x0a

#define DEVIDE_MODE 0x00
#define TD_STATUS   0x02
#define TOUCH1_XH   0x03
#define TOUCH1_XL   0x04
#define TOUCH1_YH   0x05
#define TOUCH1_YL   0x06

class TP_Point
{
public:
    TP_Point(void)
    {
        x = 0;
        y = 0;
    }
    TP_Point(int16_t _x, int16_t _y)
    {
        x = _x;
        y = _y;
    }
    int16_t x;
    int16_t y;
};


class FT5206_Class
{
public:
    FT5206_Class() {};
    int begin(TwoWire &port = Wire, uint8_t addr = FT5206_SLAVE_ADDRESS);
    // valid touching detect threshold.
    void adjustTheshold(uint8_t thresh);
    TP_Point getPoint(uint8_t num = 0);
    uint8_t touched();
    void enterSleepMode();
    void enterMonitorMode();
private:
    void _readRegister();
    int _readByte(uint8_t reg, uint8_t nbytes, uint8_t *data)
    {
        _i2cPort->beginTransmission(_address);
        _i2cPort->write(reg);
        _i2cPort->endTransmission();
        _i2cPort->requestFrom(_address, nbytes);
        uint8_t index = 0;
        while (_i2cPort->available())
            data[index++] = _i2cPort->read();
        return 0;
    }

    int _writeByte(uint8_t reg, uint8_t nbytes, uint8_t *data)
    {
        _i2cPort->beginTransmission(_address);
        _i2cPort->write(reg);
        for (uint8_t i = 0; i < nbytes; i++) {
            _i2cPort->write(data[i]);
        }
        _i2cPort->endTransmission();
        return 0;
    }

    uint8_t _address;
    uint8_t _data[16];
    uint16_t _x[2];
    uint16_t _y[2];
    uint16_t _id[2];
    uint8_t _touches = 0;
    bool _init = false;
    TwoWire *_i2cPort;
};
