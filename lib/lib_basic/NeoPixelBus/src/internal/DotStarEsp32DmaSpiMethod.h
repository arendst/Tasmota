/*-------------------------------------------------------------------------
NeoPixel library helper functions for DotStars using Esp32, DMA and SPI (APA102).

Written by Michael C. Miller.
DotStarEsp32DmaSpiMethod written by Louis Beaudoin (Pixelvation)

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

#include "driver/spi_master.h"

#if (defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32C2) || defined(CONFIG_IDF_TARGET_ESP32C5) || defined(CONFIG_IDF_TARGET_ESP32C6)) && !defined(HSPI_HOST)
// HSPI_HOST depreciated in C3
#define HSPI_HOST   SPI2_HOST
#endif

#if !defined(CONFIG_IDF_TARGET_ESP32S2) && !defined(CONFIG_IDF_TARGET_ESP32C3) && !defined(CONFIG_IDF_TARGET_ESP32C2) && !defined(CONFIG_IDF_TARGET_ESP32C5) && !defined(CONFIG_IDF_TARGET_ESP32C6)
class Esp32VspiBus
{
public:
    const static spi_host_device_t SpiHostDevice = VSPI_HOST;
    const static int DmaChannel = 1;    // arbitrary assignment, but based on the fact there are only two DMA channels and two available SPI ports, we need to split them somehow
    const static int ParallelBits = 1;
};
#endif

class Esp32HspiBus
{
public:
    const static spi_host_device_t SpiHostDevice = HSPI_HOST;
    const static int DmaChannel = 2;    // arbitrary assignment, but based on the fact there are only two DMA channels and two available SPI ports, we need to split them somehow
    const static int ParallelBits = 1;
};

#if !defined(CONFIG_IDF_TARGET_ESP32S2) && !defined(CONFIG_IDF_TARGET_ESP32C3) && !defined(CONFIG_IDF_TARGET_ESP32C2) && !defined(CONFIG_IDF_TARGET_ESP32C5) && !defined(CONFIG_IDF_TARGET_ESP32C6)
class Esp32Vspi2BitBus
{
public:
    const static spi_host_device_t SpiHostDevice = VSPI_HOST;
    const static int DmaChannel = 1;    // arbitrary assignment, but based on the fact there are only two DMA channels and two available SPI ports, we need to split them somehow
    const static int ParallelBits = 2;
};
#endif

class Esp32Hspi2BitBus
{
public:
    const static spi_host_device_t SpiHostDevice = HSPI_HOST;
    const static int DmaChannel = 2;    // arbitrary assignment, but based on the fact there are only two DMA channels and two available SPI ports, we need to split them somehow
    const static int ParallelBits = 2;
};

#if !defined(CONFIG_IDF_TARGET_ESP32S2) && !defined(CONFIG_IDF_TARGET_ESP32C3) && !defined(CONFIG_IDF_TARGET_ESP32C2) && !defined(CONFIG_IDF_TARGET_ESP32C5) && !defined(CONFIG_IDF_TARGET_ESP32C6)
class Esp32Vspi4BitBus
{
public:
    const static spi_host_device_t SpiHostDevice = VSPI_HOST;
    const static int DmaChannel = 1;    // arbitrary assignment, but based on the fact there are only two DMA channels and two available SPI ports, we need to split them somehow
    const static int ParallelBits = 4;
};
#endif

class Esp32Hspi4BitBus
{
public:
    const static spi_host_device_t SpiHostDevice = HSPI_HOST;
    const static int DmaChannel = 2;    // arbitrary assignment, but based on the fact there are only two DMA channels and two available SPI ports, we need to split them somehow
    const static int ParallelBits = 4;
};

template<typename T_SPISPEED, typename T_SPIBUS> class DotStarEsp32DmaSpiMethod
{
public:
    typedef typename T_SPISPEED::SettingsObject SettingsObject;

    DotStarEsp32DmaSpiMethod(uint16_t pixelCount, size_t elementSize, size_t settingsSize) :
        _sizePixelData(pixelCount * elementSize + settingsSize),
        _sizeEndFrame((pixelCount + 15) / 16) // 16 = div 2 (bit for every two pixels) div 8 (bits to bytes)
    {
        _spiBufferSize = _sizeStartFrame + _sizePixelData + _sizeEndFrame;

        // must have a 4 byte aligned buffer for i2s
        uint32_t alignment = _spiBufferSize % 4;
        if (alignment)
        {
            _spiBufferSize += 4 - alignment;
        }

        _data = static_cast<uint8_t*>(malloc(_spiBufferSize));
        _dmadata = static_cast<uint8_t*>(heap_caps_malloc(_spiBufferSize, MALLOC_CAP_DMA));

        // data cleared later in NeoPixelBus::Begin()
    }

    // Support constructor specifying pins by ignoring pins
    DotStarEsp32DmaSpiMethod(uint8_t, uint8_t, uint16_t pixelCount, size_t elementSize, size_t settingsSize) :
        DotStarEsp32DmaSpiMethod(pixelCount, elementSize, settingsSize)
    {
    }

    ~DotStarEsp32DmaSpiMethod()
    {
        if (_spiHandle)
        {
            deinitSpiDevice();
            esp_err_t ret = spi_bus_free(T_SPIBUS::SpiHostDevice);
            ESP_ERROR_CHECK(ret);            
        }
        free(_data);
        free(_dmadata);
        _spiHandle = NULL;
    }

    bool IsReadyToUpdate() const
    {
        spi_transaction_t t;
        spi_transaction_t * tptr = &t;
        esp_err_t ret = spi_device_get_trans_result(_spiHandle, &tptr, 0);

        // We know the previous transaction completed if we got ESP_OK, and we know there's no transactions queued if tptr is unmodified
        return (ret == ESP_OK || tptr == &t);
    }

    void Initialize(int8_t sck, int8_t dat0, int8_t dat1, int8_t dat2, int8_t dat3, int8_t ss)
    {
        memset(_data, 0x00, _sizeStartFrame);
        memset(_data + _sizeStartFrame + _sizePixelData, 0x00, _spiBufferSize - (_sizeStartFrame + _sizePixelData));

        _ssPin = ss;

        esp_err_t ret;
        spi_bus_config_t buscfg;
        memset(&buscfg, 0x00, sizeof(buscfg));

        buscfg.miso_io_num = dat1;
        buscfg.mosi_io_num = dat0;
        buscfg.sclk_io_num = sck;
        buscfg.quadwp_io_num = dat2;
        buscfg.quadhd_io_num = dat3;
        buscfg.max_transfer_sz = _spiBufferSize;

        //Initialize the SPI bus
        ret = spi_bus_initialize(T_SPIBUS::SpiHostDevice, &buscfg, T_SPIBUS::DmaChannel);
        ESP_ERROR_CHECK(ret);

        initSpiDevice();
    }

    void Initialize(int8_t sck, int8_t miso, int8_t mosi, int8_t ss)
    {
        Initialize(sck, mosi, miso, -1, -1, ss);
    }

    // If pins aren't specified, initialize bus with just the default SCK and MOSI pins for the SPI peripheral (no SS, no >1-bit pins)
    void Initialize()
    {
#if !defined(CONFIG_IDF_TARGET_ESP32S2) && !defined(CONFIG_IDF_TARGET_ESP32C3) && !defined(CONFIG_IDF_TARGET_ESP32C2) && !defined(CONFIG_IDF_TARGET_ESP32C5) && !defined(CONFIG_IDF_TARGET_ESP32C6)
        if (T_SPIBUS::SpiHostDevice == VSPI_HOST)
        {
            Initialize(SCK, -1, MOSI, -1, -1, -1);
        }
        else
        {
            Initialize(14, -1, 13, -1, -1, -1);
        }
#else
        Initialize(SCK, -1, MOSI, -1, -1, -1);
#endif
    }

    void Update(bool)
    {
        while(!IsReadyToUpdate());

        memcpy(_dmadata, _data, _spiBufferSize);

        memset(&_spiTransaction, 0, sizeof(spi_transaction_t));
        _spiTransaction.length = (_spiBufferSize) * 8; // in bits not bytes!
        if (T_SPIBUS::ParallelBits == 1)
        {
            _spiTransaction.flags = 0;
        }
        if (T_SPIBUS::ParallelBits == 2)
        {
            _spiTransaction.flags = SPI_TRANS_MODE_DIO;
        }
        if (T_SPIBUS::ParallelBits == 4)
        {
            _spiTransaction.flags = SPI_TRANS_MODE_QIO;
        }
        _spiTransaction.tx_buffer = _dmadata;

        esp_err_t ret = spi_device_queue_trans(_spiHandle, &_spiTransaction, 0);  //Transmit!
        assert(ret == ESP_OK);            //Should have had no issues.
    }

    uint8_t* getData() const
    {
        return _data + _sizeStartFrame;
    };

    size_t getDataSize() const
    {
        return _sizePixelData;
    };

    void applySettings(const SettingsObject& settings)
    {
        _speed.applySettings(settings);
        if (_spiHandle)
        {
            deinitSpiDevice();
            initSpiDevice();
        }
    }

private:
    void initSpiDevice()
    {
        spi_device_interface_config_t devcfg = {};

        devcfg.clock_speed_hz = _speed.Clock;
        devcfg.mode = 0;                 //SPI mode 0
        devcfg.spics_io_num = _ssPin;    //CS pin
        devcfg.queue_size = 1;
        if (T_SPIBUS::ParallelBits == 1)
        {
            devcfg.flags = 0;
        }
        if (T_SPIBUS::ParallelBits >= 2)
        {
            devcfg.flags = SPI_DEVICE_HALFDUPLEX;
        }

        //Allocate the LEDs on the SPI bus
        esp_err_t ret = spi_bus_add_device(T_SPIBUS::SpiHostDevice, &devcfg, &_spiHandle);
        ESP_ERROR_CHECK(ret);
    }

    void deinitSpiDevice()
    {
        while(!IsReadyToUpdate());
        esp_err_t ret = spi_bus_remove_device(_spiHandle);
        ESP_ERROR_CHECK(ret);
    }

    const size_t             _sizeStartFrame = 4;
    const size_t             _sizePixelData;   // Size of '_data' buffer below, minus (_sizeStartFrame + _sizeEndFrame)
    const size_t             _sizeEndFrame;

    size_t                  _spiBufferSize;
    uint8_t*                _data;       // Holds start/end frames and LED color values
    uint8_t*                _dmadata;    // Holds start/end frames and LED color values
    spi_device_handle_t     _spiHandle = NULL;
    spi_transaction_t       _spiTransaction;
    T_SPISPEED              _speed;
    int8_t                  _ssPin;
};

#if !defined(CONFIG_IDF_TARGET_ESP32S2) && !defined(CONFIG_IDF_TARGET_ESP32C3) && !defined(CONFIG_IDF_TARGET_ESP32C2) && !defined(CONFIG_IDF_TARGET_ESP32C5) && !defined(CONFIG_IDF_TARGET_ESP32C6)
// Clock Speed and Default Definitions for DotStarEsp32DmaVspi
typedef DotStarEsp32DmaSpiMethod<SpiSpeed40Mhz, Esp32VspiBus> DotStarEsp32DmaVspi40MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed20Mhz, Esp32VspiBus> DotStarEsp32DmaVspi20MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed10Mhz, Esp32VspiBus> DotStarEsp32DmaVspi10MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed5Mhz, Esp32VspiBus> DotStarEsp32DmaVspi5MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed2Mhz, Esp32VspiBus> DotStarEsp32DmaVspi2MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed1Mhz, Esp32VspiBus> DotStarEsp32DmaVspi1MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed500Khz, Esp32VspiBus> DotStarEsp32DmaVspi500KhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeedHz, Esp32VspiBus> DotStarEsp32DmaVspiHzMethod;

typedef DotStarEsp32DmaVspi10MhzMethod DotStarEsp32DmaVspiMethod;
#endif

// Clock Speed and Default Definitions for DotStarEsp32DmaHspi
typedef DotStarEsp32DmaSpiMethod<SpiSpeed40Mhz, Esp32HspiBus> DotStarEsp32DmaHspi40MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed20Mhz, Esp32HspiBus> DotStarEsp32DmaHspi20MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed10Mhz, Esp32HspiBus> DotStarEsp32DmaHspi10MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed5Mhz, Esp32HspiBus> DotStarEsp32DmaHspi5MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed2Mhz, Esp32HspiBus> DotStarEsp32DmaHspi2MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed1Mhz, Esp32HspiBus> DotStarEsp32DmaHspi1MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed500Khz, Esp32HspiBus> DotStarEsp32DmaHspi500KhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeedHz, Esp32HspiBus> DotStarEsp32DmaHspiHzMethod;

typedef DotStarEsp32DmaHspi10MhzMethod DotStarEsp32DmaHspiMethod;

#if !defined(CONFIG_IDF_TARGET_ESP32S2) && !defined(CONFIG_IDF_TARGET_ESP32C3) && !defined(CONFIG_IDF_TARGET_ESP32C2) && !defined(CONFIG_IDF_TARGET_ESP32C5) && !defined(CONFIG_IDF_TARGET_ESP32C6)
// Clock Speed and Default Definitions for DotStarEsp32DmaVspi2Bit
typedef DotStarEsp32DmaSpiMethod<SpiSpeed40Mhz,Esp32Vspi2BitBus> DotStarEsp32DmaVspi2Bit40MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed20Mhz,Esp32Vspi2BitBus> DotStarEsp32DmaVspi2Bit20MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed10Mhz,Esp32Vspi2BitBus> DotStarEsp32DmaVspi2Bit10MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed5Mhz,Esp32Vspi2BitBus> DotStarEsp32DmaVspi2Bit5MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed2Mhz,Esp32Vspi2BitBus> DotStarEsp32DmaVspi2Bit2MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed1Mhz,Esp32Vspi2BitBus> DotStarEsp32DmaVspi2Bit1MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed500Khz,Esp32Vspi2BitBus> DotStarEsp32DmaVspi2Bit500KhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeedHz,Esp32Vspi2BitBus> DotStarEsp32DmaVspi2BitHzMethod;

typedef DotStarEsp32DmaVspi2Bit10MhzMethod DotStarEsp32DmaVspi2BitMethod;
#endif

// Clock Speed and Default Definitions for DotStarEsp32DmaHspi2Bit
typedef DotStarEsp32DmaSpiMethod<SpiSpeed40Mhz,Esp32Hspi2BitBus> DotStarEsp32DmaHspi2Bit40MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed20Mhz,Esp32Hspi2BitBus> DotStarEsp32DmaHspi2Bit20MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed10Mhz,Esp32Hspi2BitBus> DotStarEsp32DmaHspi2Bit10MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed5Mhz,Esp32Hspi2BitBus> DotStarEsp32DmaHspi2Bit5MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed2Mhz,Esp32Hspi2BitBus> DotStarEsp32DmaHspi2Bit2MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed1Mhz,Esp32Hspi2BitBus> DotStarEsp32DmaHspi2Bit1MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed500Khz,Esp32Hspi2BitBus> DotStarEsp32DmaHspi2Bit500KhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeedHz,Esp32Hspi2BitBus> DotStarEsp32DmaHspi2BitHzMethod;

typedef DotStarEsp32DmaHspi2Bit10MhzMethod DotStarEsp32DmaHspi2BitMethod;

#if !defined(CONFIG_IDF_TARGET_ESP32S2) && !defined(CONFIG_IDF_TARGET_ESP32C3) && !defined(CONFIG_IDF_TARGET_ESP32C2) && !defined(CONFIG_IDF_TARGET_ESP32C5) && !defined(CONFIG_IDF_TARGET_ESP32C6)
// Clock Speed and Default Definitions for DotStarEsp32DmaVspi4Bit
typedef DotStarEsp32DmaSpiMethod<SpiSpeed40Mhz,Esp32Vspi4BitBus> DotStarEsp32DmaVspi4Bit40MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed20Mhz,Esp32Vspi4BitBus> DotStarEsp32DmaVspi4Bit20MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed10Mhz,Esp32Vspi4BitBus> DotStarEsp32DmaVspi4Bit10MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed5Mhz,Esp32Vspi4BitBus> DotStarEsp32DmaVspi4Bit5MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed2Mhz,Esp32Vspi4BitBus> DotStarEsp32DmaVspi4Bit2MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed1Mhz,Esp32Vspi4BitBus> DotStarEsp32DmaVspi4Bit1MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed500Khz,Esp32Vspi4BitBus> DotStarEsp32DmaVspi4Bit500KhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeedHz,Esp32Vspi4BitBus> DotStarEsp32DmaVspi4BitHzMethod;

typedef DotStarEsp32DmaVspi4Bit10MhzMethod DotStarEsp32DmaVspi4BitMethod;
#endif

// Clock Speed and Default Definitions for DotStarEsp32DmaHspi4Bit
typedef DotStarEsp32DmaSpiMethod<SpiSpeed40Mhz,Esp32Hspi4BitBus> DotStarEsp32DmaHspi4Bit40MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed20Mhz,Esp32Hspi4BitBus> DotStarEsp32DmaHspi4Bit20MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed10Mhz,Esp32Hspi4BitBus> DotStarEsp32DmaHspi4Bit10MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed5Mhz,Esp32Hspi4BitBus> DotStarEsp32DmaHspi4Bit5MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed2Mhz,Esp32Hspi4BitBus> DotStarEsp32DmaHspi4Bit2MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed1Mhz,Esp32Hspi4BitBus> DotStarEsp32DmaHspi4Bit1MhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeed500Khz,Esp32Hspi4BitBus> DotStarEsp32DmaHspi4Bit500KhzMethod;
typedef DotStarEsp32DmaSpiMethod<SpiSpeedHz,Esp32Hspi4BitBus> DotStarEsp32DmaHspi4BitHzMethod;

typedef DotStarEsp32DmaHspi4Bit10MhzMethod DotStarEsp32DmaHspi4BitMethod;
