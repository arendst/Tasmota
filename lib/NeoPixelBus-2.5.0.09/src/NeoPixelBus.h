/*-------------------------------------------------------------------------
NeoPixel library 

Written by Michael C. Miller.

I invest time and resources providing this open source code,
please support me by dontating (see https://github.com/Makuna/NeoPixelBus)

-------------------------------------------------------------------------
This file is part of the Makuna/NeoPixelBus library.

NeoPixelBus is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

NeoPixelBus is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with NeoPixel.  If not, see
<http://www.gnu.org/licenses/>.
-------------------------------------------------------------------------*/
#pragma once

#include <Arduino.h>

// some platforms do not define this standard progmem type for some reason
//
#ifndef PGM_VOID_P
#define PGM_VOID_P const void *
#endif

// '_state' flags for internal state
#define NEO_DIRTY   0x80 // a change was made to pixel data that requires a show

#include "internal/NeoHueBlend.h"

#include "internal/RgbColor.h"
#include "internal/HslColor.h"
#include "internal/HsbColor.h"
#include "internal/HtmlColor.h"
#include "internal/RgbwColor.h"

#include "internal/NeoColorFeatures.h"
#include "internal/DotStarColorFeatures.h"

#include "internal/Layouts.h"
#include "internal/NeoTopology.h"
#include "internal/NeoRingTopology.h"
#include "internal/NeoTiles.h"
#include "internal/NeoMosaic.h"

#include "internal/NeoBufferContext.h"
#include "internal/NeoBufferMethods.h"
#include "internal/NeoBuffer.h"
#include "internal/NeoSpriteSheet.h"
#include "internal/NeoDib.h"
#include "internal/NeoBitmapFile.h"

#include "internal/NeoEase.h"
#include "internal/NeoGamma.h"

#if defined(ARDUINO_ARCH_ESP8266)

#include "internal/NeoEsp8266DmaMethod.h"
#include "internal/NeoEsp8266UartMethod.h"
#include "internal/NeoEspBitBangMethod.h"
#include "internal/DotStarGenericMethod.h"

#elif defined(ARDUINO_ARCH_ESP32)

#include "internal/NeoEsp32I2sMethod.h"
#include "internal/NeoEsp32RmtMethod.h"
#include "internal/NeoEspBitBangMethod.h"
#include "internal/DotStarGenericMethod.h"

#elif defined(__arm__) // must be before ARDUINO_ARCH_AVR due to Teensy incorrectly having it set

#include "internal/NeoArmMethod.h"
#include "internal/DotStarGenericMethod.h"

#elif defined(ARDUINO_ARCH_AVR)

#include "internal/NeoAvrMethod.h"
#include "internal/DotStarAvrMethod.h"

#else
#error "Platform Currently Not Supported, please add an Issue at Github/Makuna/NeoPixelBus"
#endif

#if !defined(__AVR_ATtiny85__)
#include "internal/DotStarSpiMethod.h"
#endif


template<typename T_COLOR_FEATURE, typename T_METHOD> class NeoPixelBus
{
public:
    // Constructor: number of LEDs, pin number
    // NOTE:  Pin Number maybe ignored due to hardware limitations of the method.
   
    NeoPixelBus(uint16_t countPixels, uint8_t pin) :
        _countPixels(countPixels),
        _state(0),
        _method(pin, countPixels, T_COLOR_FEATURE::PixelSize)
    {
    }

    NeoPixelBus(uint16_t countPixels, uint8_t pinClock, uint8_t pinData) :
        _countPixels(countPixels),
        _state(0),
        _method(pinClock, pinData, countPixels, T_COLOR_FEATURE::PixelSize)
    {
    }

    NeoPixelBus(uint16_t countPixels) :
        _countPixels(countPixels),
        _state(0),
        _method(countPixels, T_COLOR_FEATURE::PixelSize)
    {
    }

    ~NeoPixelBus()
    {
    }

    operator NeoBufferContext<T_COLOR_FEATURE>()
    {
        Dirty(); // we assume you are playing with bits
        return NeoBufferContext<T_COLOR_FEATURE>(_method.getPixels(), _method.getPixelsSize());
    }

    void Begin()
    {
        _method.Initialize();
        Dirty();
    }

    // used by DotStartSpiMethod if pins can be configured
    void Begin(int8_t sck, int8_t miso, int8_t mosi, int8_t ss)
    {
        _method.Initialize(sck, miso, mosi, ss);
        Dirty();
    }

    void Show(bool maintainBufferConsistency = true)
    {
        if (!IsDirty())
        {
            return;
        }

        _method.Update(maintainBufferConsistency);

        ResetDirty();
    }

    inline bool CanShow() const
    { 
        return _method.IsReadyToUpdate();
    };

    bool IsDirty() const
    {
        return  (_state & NEO_DIRTY);
    };

    void Dirty()
    {
        _state |= NEO_DIRTY;
    };

    void ResetDirty()
    {
        _state &= ~NEO_DIRTY;
    };

    uint8_t* Pixels() 
    {
        return _method.getPixels();
    };

    size_t PixelsSize() const
    {
        return _method.getPixelsSize();
    };

    size_t PixelSize() const
    {
        return T_COLOR_FEATURE::PixelSize;
    };

    uint16_t PixelCount() const
    {
        return _countPixels;
    };

    void SetPixelColor(uint16_t indexPixel, typename T_COLOR_FEATURE::ColorObject color)
    {
        if (indexPixel < _countPixels)
        {
            T_COLOR_FEATURE::applyPixelColor(_method.getPixels(), indexPixel, color);
            Dirty();
        }
    };

    typename T_COLOR_FEATURE::ColorObject GetPixelColor(uint16_t indexPixel) const
    {
        if (indexPixel < _countPixels)
        {
            return T_COLOR_FEATURE::retrievePixelColor(_method.getPixels(), indexPixel);
        }
        else
        {
            // Pixel # is out of bounds, this will get converted to a 
            // color object type initialized to 0 (black)
            return 0;
        }
    };

    void ClearTo(typename T_COLOR_FEATURE::ColorObject color)
    {
        uint8_t temp[T_COLOR_FEATURE::PixelSize]; 
        uint8_t* pixels = _method.getPixels();

        T_COLOR_FEATURE::applyPixelColor(temp, 0, color);

        T_COLOR_FEATURE::replicatePixel(pixels, temp, _countPixels);

        Dirty();
    };

    void ClearTo(typename T_COLOR_FEATURE::ColorObject color, uint16_t first, uint16_t last)
    {
        if (first < _countPixels &&
            last < _countPixels &&
            first <= last)
        {
            uint8_t temp[T_COLOR_FEATURE::PixelSize];
            uint8_t* pixels = _method.getPixels();
            uint8_t* pFront = T_COLOR_FEATURE::getPixelAddress(pixels, first);

            T_COLOR_FEATURE::applyPixelColor(temp, 0, color);

            T_COLOR_FEATURE::replicatePixel(pFront, temp, last - first + 1);

            Dirty();
        }
    }

    void RotateLeft(uint16_t rotationCount)
    {
        if ((_countPixels - 1) >= rotationCount)
        {
            _rotateLeft(rotationCount, 0, _countPixels - 1);
        }
    }

    void RotateLeft(uint16_t rotationCount, uint16_t first, uint16_t last)
    {
        if (first < _countPixels &&
            last < _countPixels &&
            first < last &&
            (last - first) >= rotationCount)
        {
            _rotateLeft(rotationCount, first, last);
        }
    }

    void ShiftLeft(uint16_t shiftCount)
    {
        if ((_countPixels - 1) >= shiftCount)
        {
            _shiftLeft(shiftCount, 0, _countPixels - 1);
            Dirty();
        }
    }

    void ShiftLeft(uint16_t shiftCount, uint16_t first, uint16_t last)
    {
        if (first < _countPixels && 
            last < _countPixels && 
            first < last &&
            (last - first) >= shiftCount)
        {
            _shiftLeft(shiftCount, first, last);
            Dirty();
        }
    }

    void RotateRight(uint16_t rotationCount)
    {
        if ((_countPixels - 1) >= rotationCount)
        {
            _rotateRight(rotationCount, 0, _countPixels - 1);
        }
    }

    void RotateRight(uint16_t rotationCount, uint16_t first, uint16_t last)
    {
        if (first < _countPixels &&
            last < _countPixels &&
            first < last &&
            (last - first) >= rotationCount)
        {
            _rotateRight(rotationCount, first, last);
        }
    }

    void ShiftRight(uint16_t shiftCount)
    {
        if ((_countPixels - 1) >= shiftCount)
        {
            _shiftRight(shiftCount, 0, _countPixels - 1);
            Dirty();
        }
    }

    void ShiftRight(uint16_t shiftCount, uint16_t first, uint16_t last)
    {
        if (first < _countPixels &&
            last < _countPixels &&
            first < last &&
            (last - first) >= shiftCount)
        {
            _shiftRight(shiftCount, first, last);
            Dirty();
        }
    }
    
    void SwapPixelColor(uint16_t indexPixelOne, uint16_t indexPixelTwo)
    {
        auto colorOne = GetPixelColor(indexPixelOne);
        auto colorTwo = GetPixelColor(indexPixelTwo);

        SetPixelColor(indexPixelOne, colorTwo);
        SetPixelColor(indexPixelTwo, colorOne);
    };
 
protected:
    const uint16_t _countPixels; // Number of RGB LEDs in strip

    uint8_t _state;     // internal state
    T_METHOD _method;

    void _rotateLeft(uint16_t rotationCount, uint16_t first, uint16_t last)
    {
        // store in temp
        uint8_t temp[rotationCount * T_COLOR_FEATURE::PixelSize];
        uint8_t* pixels = _method.getPixels();

        uint8_t* pFront = T_COLOR_FEATURE::getPixelAddress(pixels, first);

        T_COLOR_FEATURE::movePixelsInc(temp, pFront, rotationCount);

        // shift data
        _shiftLeft(rotationCount, first, last);

        // move temp back
        pFront = T_COLOR_FEATURE::getPixelAddress(pixels, last - (rotationCount - 1));
        T_COLOR_FEATURE::movePixelsInc(pFront, temp, rotationCount);

        Dirty();
    }

    void _shiftLeft(uint16_t shiftCount, uint16_t first, uint16_t last)
    {
        uint16_t front = first + shiftCount;
        uint16_t count = last - front + 1;

        uint8_t* pixels = _method.getPixels();
        uint8_t* pFirst = T_COLOR_FEATURE::getPixelAddress(pixels, first);
        uint8_t* pFront = T_COLOR_FEATURE::getPixelAddress(pixels, front);

        T_COLOR_FEATURE::movePixelsInc(pFirst, pFront, count);

        // intentional no dirty
    }

    void _rotateRight(uint16_t rotationCount, uint16_t first, uint16_t last)
    {
        // store in temp
        uint8_t temp[rotationCount * T_COLOR_FEATURE::PixelSize];
        uint8_t* pixels = _method.getPixels();

        uint8_t* pFront = T_COLOR_FEATURE::getPixelAddress(pixels, last - (rotationCount - 1));

        T_COLOR_FEATURE::movePixelsDec(temp, pFront, rotationCount);

        // shift data
        _shiftRight(rotationCount, first, last);

        // move temp back
        pFront = T_COLOR_FEATURE::getPixelAddress(pixels, first);
        T_COLOR_FEATURE::movePixelsDec(pFront, temp, rotationCount);

        Dirty();
    }

    void _shiftRight(uint16_t shiftCount, uint16_t first, uint16_t last)
    {
        uint16_t front = first + shiftCount;
        uint16_t count = last - front + 1;

        uint8_t* pixels = _method.getPixels();
        uint8_t* pFirst = T_COLOR_FEATURE::getPixelAddress(pixels, first);
        uint8_t* pFront = T_COLOR_FEATURE::getPixelAddress(pixels, front);

        T_COLOR_FEATURE::movePixelsDec(pFront, pFirst, count);
        // intentional no dirty
    }
};


