#ifndef _UDISPLAY_SPI_CONTROLLER_H_
#define _UDISPLAY_SPI_CONTROLLER_H_

#include <Arduino.h>
#include <SPI.h>

#ifdef ESP32
#include "soc/spi_reg.h"
#include "soc/spi_struct.h"
#include "esp32-hal-spi.h"
#include "driver/spi_master.h"
#include "soc/gpio_periph.h"
#endif

#ifndef ESP32
#include "spi_register.h"
#endif

struct SPIControllerConfig {
    uint8_t bus_nr;
    int8_t cs;
    int8_t clk;
    int8_t mosi;
    int8_t dc;
    int8_t miso;
    uint32_t speed;
};

/**
 * Minimal SPIController - wraps low-level SPI functions
 * Extracted from uDisplay_spi.cpp
 */
class SPIController {
public:
    SPIController(const SPIControllerConfig& config);
    ~SPIController() = default;

    // ===== Pin Control =====
    void csLow();
    void csHigh();
    void dcLow();
    void dcHigh();

    // ===== Transaction Control =====
    void beginTransaction();
    void endTransaction();

    // ===== High-Level Write Functions =====
    void writeCommand(uint8_t cmd);
    void writeData8(uint8_t data);
    void writeData16(uint16_t data); 
    void writeData32(uint32_t data);

    // ===== RA8876 Specific =====
    uint8_t writeReg16(uint8_t reg, uint16_t wval);
    uint8_t readData(void);
    uint8_t readStatus(void);

    // ===== Direct Access =====
    SPIClass* getSPI() { return spi; }
    // SPISettings getSPISettings() { return spi_settings; }

    // ===== DMA =====
#ifdef ESP32
    bool initDMA(uint16_t width, uint16_t height, uint8_t data);
    void dmaWait(void);
    bool dmaBusy(void);
    void pushPixelsDMA(uint16_t* image, uint32_t len);
    void pushPixels3DMA(uint8_t* image, uint32_t len);
#endif
    SPIControllerConfig spi_config; // make this private in the future again!

private:
    SPIClass* spi;
    SPISettings spi_settings;
    
    // ===== Low-Level Write Functions =====
    void write8(uint8_t val);
    void write8_slow(uint8_t val);
    void write9(uint8_t val, uint8_t dc);
    void write9_slow(uint8_t val, uint8_t dc);
    void write16(uint16_t val);
    void write32(uint32_t val);
    void hw_write9(uint8_t val, uint8_t dc);

#ifdef ESP32
    bool dma_enabled = false;
    bool async_dma_enabled = false;

    spi_host_device_t spi_host = VSPI_HOST;
    bool DMA_Enabled = false;
    uint8_t spiBusyCheck;
    spi_device_handle_t dmaHAL = nullptr;  // For DMA
    spi_transaction_t trans;
#endif  //ESP32
};

#endif // _UDISPLAY_SPI_CONTROLLER_H_