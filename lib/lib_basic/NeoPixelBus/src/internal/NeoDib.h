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

template<typename T_COLOR_OBJECT> class NeoShaderNop
{
public:
    NeoShaderNop()
    {
    }

    bool IsDirty() const
    {
        return true;
    };

    void Dirty()
    {
    };

    void ResetDirty()
    {
    };

    T_COLOR_OBJECT Apply(uint16_t, T_COLOR_OBJECT color)
    {
        return color;
    };
};

class NeoShaderBase
{
public:
    NeoShaderBase() :
        _state(0)
    {
    }

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

protected:
    uint8_t _state;     // internal state
};

template<typename T_COLOR_OBJECT> class NeoDib
{
public:
    NeoDib(uint16_t countPixels) :
        _countPixels(countPixels),
        _state(0)
    {
        _pixels = (T_COLOR_OBJECT*)malloc(PixelsSize());
        ResetDirty();
    }

    ~NeoDib()
    {
        free((uint8_t*)_pixels);
    }

    NeoDib& operator=(const NeoDib& other)
    {
        // check for self-assignment
        if (&other == this)
        {
            return *this;
        }

        uint16_t copyCount = other.PixelCount() < PixelCount() ? other.PixelCount() : PixelCount();

        for (uint16_t pixel = 0; pixel < copyCount; pixel++)
        {
            _pixels[pixel] = other.Pixels()[pixel];
        }

        Dirty();
        return *this;
    }

    T_COLOR_OBJECT* Pixels() const
    {
        return _pixels;
    };

    uint16_t PixelCount() const
    {
        return _countPixels;
    };

    size_t PixelsSize() const
    {
        return _countPixels * PixelSize();
    };

    size_t PixelSize() const
    {
        return sizeof(T_COLOR_OBJECT);
    };

    void SetPixelColor(
        uint16_t indexPixel,
        T_COLOR_OBJECT color)
    {
        if (indexPixel < PixelCount())
        {
            _pixels[indexPixel] = color;
            Dirty();
        }
    };

    T_COLOR_OBJECT GetPixelColor(
        uint16_t indexPixel) const
    {
        if (indexPixel >= PixelCount())
        {
            return 0;
        }
        return _pixels[indexPixel];
    };

    void ClearTo(T_COLOR_OBJECT color)
    {
        for (uint16_t pixel = 0; pixel < PixelCount(); pixel++)
        {
            _pixels[pixel] = color;
        }
        Dirty();
    };

    template <typename T_COLOR_FEATURE, typename T_SHADER> 
    void Render(NeoBufferContext<T_COLOR_FEATURE> destBuffer, T_SHADER& shader, uint16_t destIndexPixel = 0)
    {
        if (IsDirty() || shader.IsDirty())
        {
            uint16_t countPixels = destBuffer.PixelCount();

            if (countPixels > _countPixels)
            {
                countPixels = _countPixels;
            }

            for (uint16_t indexPixel = 0; indexPixel < countPixels; indexPixel++)
            {
                T_COLOR_OBJECT color = shader.Apply(indexPixel, _pixels[indexPixel]);
                T_COLOR_FEATURE::applyPixelColor(destBuffer.Pixels, destIndexPixel + indexPixel, color);
            }

            shader.ResetDirty();
            ResetDirty();
        }
    }

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

private:
    const uint16_t _countPixels; // Number of RGB LEDs in strip
    T_COLOR_OBJECT* _pixels;
    uint8_t _state;     // internal state
};