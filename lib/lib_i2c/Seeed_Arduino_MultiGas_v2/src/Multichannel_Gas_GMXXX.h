/*
    Multichannel_Gas_GMXXX.h
    Description: A drive for Seeed Grove Multichannel gas sensor V2.0.
    2019 Copyright (c) Seeed Technology Inc.  All right reserved.
    Author: Hongtai Liu(lht856@foxmail.com)
    2019-6-18

    The MIT License (MIT)
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.1  USA
*/
#ifndef __GAS_GMXXX__
#define __GAS_GMXXX__

//#define SOFTWAREWIRE
#ifdef SOFTWAREWIRE
    #include <SoftwareWire.h>
#else
    #include <Wire.h>
#endif

#if defined(ARDUINO_ARCH_AVR) // AVR verf 5V
#define GM_VERF 5
#else
#define GM_VERF 3.3
#endif

#define GM_RESOLUTION 1023

//command
#define GM_102B 0x01
#define GM_302B 0x03
//#define GM_402B 0x04
#define GM_502B 0x05
#define GM_702B 0x07
//#define GM_802B 0x08
#define CHANGE_I2C_ADDR 0x55
#define WARMING_UP 0xFE
#define WARMING_DOWN  0xFF

template <class T>
class GAS_GMXXX {
  public:
    GAS_GMXXX();
    void begin(T& wire = Wire, uint8_t address = 0x08);
    void setAddress(uint8_t address = 0x08);
    void preheated();
    void unPreheated();
    void changeGMXXXAddr(uint8_t address = 0x08);
    uint32_t measure_NO2(){
        return getGM102B();
    };
    uint32_t getGM102B();
    uint32_t measure_C2H5OH(){
        return getGM302B();
    };
    uint32_t getGM302B();
    #ifdef GM_402B
    uint32_t getGM402B();
    #endif
    uint32_t measure_VOC(){
        return getGM502B();
    };
    uint32_t getGM502B();
    uint32_t measure_CO(){
        return getGM702B();
    };
    uint32_t getGM702B();
    #ifdef GM_802B
    uint32_t getGM802B();
    #endif
    inline float calcVol(uint32_t adc, float verf = GM_VERF, int resolution = GM_RESOLUTION) {
        return (adc * verf) / (resolution * 1.0);
    };
  private:
    T* _Wire;
    bool isPreheated;
    uint8_t GMXXX_ADDRESS;
    void GMXXXWriteByte(uint8_t cmd);
    uint32_t GMXXXRead32();
};
#endif
