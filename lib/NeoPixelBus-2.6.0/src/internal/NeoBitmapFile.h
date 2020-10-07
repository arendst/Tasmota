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

const uint16_t c_BitmapFileId = 0x4d42; // "BM"

#pragma pack(push, 2)
struct BitmapFileHeader
{
    uint16_t FileId; // only c_BitmapFileId is supported
    uint32_t FileSize;
    uint16_t Reserved0;
    uint16_t Reserved1;
    uint32_t PixelAddress;
};

struct BitmapInfoHeader
{
    uint32_t Size;
    int32_t Width;
    int32_t Height;
    uint16_t Planes; // only support 1
    uint16_t BitsPerPixel; // only support 24 and 32
    uint32_t Compression; // only support BI_Rgb
    uint32_t RawDateSize; // can be zero
    int32_t XPpm;
    int32_t YPpm;
    uint32_t PaletteLength;
    uint32_t ImportantColorCount;
};
#pragma pack(pop)

enum BmpCompression
{
    BI_Rgb, 
    BI_Rle8,
    BI_Rle4,
    BI_Bitfields,
    BI_Jpeg,
    BI_Png,
    BI_AlphaBitfields,
    BI_Cmyk = 11,
    BI_CmykRle8,
    BI_CmykRle4
};

template<typename T_COLOR_FEATURE, typename T_FILE_METHOD> class NeoBitmapFile
{
public:
    NeoBitmapFile() :
        _fileAddressPixels(0),
        _width(0),
        _height(0),
        _sizeRow(0),
        _bytesPerPixel(0),
        _bottomToTop(true)
    {
    }
 
    ~NeoBitmapFile()
    {
        _file.close();
    }

    bool Begin(T_FILE_METHOD file)
    {
        if (_file)
        {
            _file.close();
        }

        if (!file || !file.seek(0))
        {
            goto error;
        }

        _file = file;

        BitmapFileHeader bmpHeader;
        BitmapInfoHeader bmpInfoHeader;
        size_t result;

        result = _file.read((uint8_t*)(&bmpHeader), sizeof(bmpHeader));

        if (result != sizeof(bmpHeader) ||
            bmpHeader.FileId != c_BitmapFileId ||
            bmpHeader.FileSize != _file.size())
        {
            goto error;
        }

        result = _file.read((uint8_t*)(&bmpInfoHeader), sizeof(bmpInfoHeader));

        if (result != sizeof(bmpInfoHeader) ||
            result != bmpInfoHeader.Size ||
            1 != bmpInfoHeader.Planes ||
            BI_Rgb != bmpInfoHeader.Compression)
        {
            goto error;
        }

        if (!(24 == bmpInfoHeader.BitsPerPixel ||
            32 == bmpInfoHeader.BitsPerPixel))
        {
            goto error;
        }

        // save the interesting information
        _width = abs(bmpInfoHeader.Width);
        _height = abs(bmpInfoHeader.Height);
        _fileAddressPixels = bmpHeader.PixelAddress;
        // negative height means rows are top to bottom
        _bottomToTop = (bmpInfoHeader.Height > 0);
        // rows are 32 bit aligned so they may have padding on each row
        _sizeRow = (bmpInfoHeader.BitsPerPixel * _width + 31) / 32 * 4;
        _bytesPerPixel = bmpInfoHeader.BitsPerPixel / 8;

        return true;

    error:
        _fileAddressPixels = 0;
        _width = 0;
        _height = 0;
        _sizeRow = 0;
        _bytesPerPixel = 0;

        _file.close();
        return false;
    };

    size_t PixelSize() const
    {
        return T_COLOR_FEATURE::PixelSize;
    };

    uint16_t PixelCount() const
    {
        return _width * _height;
    };

    uint16_t Width() const
    {
        return _width;
    };

    uint16_t Height() const
    {
        return _height;
    };

    typename T_COLOR_FEATURE::ColorObject GetPixelColor(int16_t x, int16_t y) 
    {
        if (x < 0 || x >= _width || y < 0 || y >= _height)
        {
            // Pixel # is out of bounds, this will get converted to a 
            // color object type initialized to 0 (black)
            return 0;
        }

        typename T_COLOR_FEATURE::ColorObject color;
        if (!seek(x, y) || !readPixel(&color))
        {
            return 0;
        }
        
        return color;
    };


    template <typename T_SHADER> void Render(NeoBufferContext<T_COLOR_FEATURE> destBuffer,
        T_SHADER& shader,
        uint16_t indexPixel,
        int16_t xSrc,
        int16_t ySrc,
        int16_t wSrc)
    {
        const uint16_t destPixelCount = destBuffer.PixelCount();
        typename T_COLOR_FEATURE::ColorObject color(0);
        xSrc = constrainX(xSrc);
        ySrc = constrainY(ySrc);

        if (seek(xSrc, ySrc))
        {
            for (int16_t x = 0; x < wSrc && indexPixel < destPixelCount; x++, indexPixel++)
            {
                if ((uint16_t)xSrc < _width)
                {
                    if (readPixel(&color))
                    {
                        color = shader.Apply(indexPixel, color);
                        xSrc++;
                    }
                }

                T_COLOR_FEATURE::applyPixelColor(destBuffer.Pixels, indexPixel, color);
            }
        }
    }
    
    void Blt(NeoBufferContext<T_COLOR_FEATURE> destBuffer,
        uint16_t indexPixel,
        int16_t xSrc,
        int16_t ySrc,
        int16_t wSrc)
    {
        NeoShaderNop<typename T_COLOR_FEATURE::ColorObject> shaderNop;

        Render<NeoShaderNop<typename T_COLOR_FEATURE::ColorObject>>(destBuffer, shaderNop, indexPixel, xSrc, ySrc, wSrc);
    };

    template <typename T_SHADER> void Render(NeoBufferContext<T_COLOR_FEATURE> destBuffer,
        T_SHADER& shader,
        int16_t xDest,
        int16_t yDest,
        int16_t xSrc,
        int16_t ySrc,
        int16_t wSrc,
        int16_t hSrc,
        LayoutMapCallback layoutMap)
    {
        const uint16_t destPixelCount = destBuffer.PixelCount();
        typename T_COLOR_FEATURE::ColorObject color(0);

        for (int16_t y = 0; y < hSrc; y++)
        {
            int16_t xFile = constrainX(xSrc);
            int16_t yFile = constrainY(ySrc + y);

            if (seek(xFile, yFile))
            {
                for (int16_t x = 0; x < wSrc; x++)
                {
                    uint16_t indexDest = layoutMap(xDest + x, yDest + y);

                    if ((uint16_t)xFile < _width)
                    {
                        if (readPixel(&color))
                        {
                            color = shader.Apply(indexDest, color);
                            xFile++;
                        }
                    }

                    if (indexDest < destPixelCount)
                    {
                        T_COLOR_FEATURE::applyPixelColor(destBuffer.Pixels, indexDest, color);
                    }
                }
            }
        }
    };

    void Blt(NeoBufferContext<T_COLOR_FEATURE> destBuffer,
        int16_t xDest,
        int16_t yDest,
        int16_t xSrc,
        int16_t ySrc,
        int16_t wSrc,
        int16_t hSrc,
        LayoutMapCallback layoutMap)
    {
        NeoShaderNop<typename T_COLOR_FEATURE::ColorObject> shaderNop;

        Render<NeoShaderNop<typename T_COLOR_FEATURE::ColorObject>>(destBuffer,
            shaderNop, 
            xDest,
            yDest,
            xSrc,
            ySrc,
            wSrc,
            hSrc,
            layoutMap);
    };


private:
    T_FILE_METHOD _file;
    uint32_t _fileAddressPixels;
    uint16_t _width;
    uint16_t _height;
    uint32_t _sizeRow;
    uint8_t _bytesPerPixel;
    bool _bottomToTop;

    int16_t constrainX(int16_t x) const
    {
        if (x < 0)
        {
            x = 0;
        }
        else if ((uint16_t)x >= _width)
        {
            x = _width - 1;
        }
        return x;
    };

    int16_t constrainY(int16_t y) const
    {
        if (y < 0)
        {
            y = 0;
        }
        else if ((uint16_t)y >= _height)
        {
            y = _height - 1;
        }
        return y;
    };

    bool seek(int16_t x, int16_t y)
    {
        if (_bottomToTop)
        {
            y = (_height - 1) - y;
        }

        uint32_t pos = y * _sizeRow + x * _bytesPerPixel;
        pos += _fileAddressPixels;

        return _file.seek(pos);
    };

    bool readPixel(RgbColor* color)
    {
        uint8_t bgr[4];
        int result;

        result = _file.read(bgr, _bytesPerPixel);

        if (result != _bytesPerPixel)
        {
            *color = 0;
            return false;
        }

        color->B = bgr[0];
        color->G = bgr[1];
        color->R = bgr[2];

        return true;
    };

    bool readPixel(RgbwColor* color)
    {
        uint8_t bgr[4];
        int result;

        bgr[3] = 0; // init white channel as read maybe only 3 bytes
        result = _file.read(bgr, _bytesPerPixel);

        if (result != _bytesPerPixel)
        {
            *color = 0;
            return false;
        }

        color->B = bgr[0];
        color->G = bgr[1];
        color->R = bgr[2];
        color->W = bgr[3];

        return true;
    };
};