// ======================================================
// uDisplay_QSPI_panel.h - QSPI (quad SPI) LCD/AMOLED Panel
// ======================================================
//
// For MIPI-DCS style controllers wired in QSPI mode (SH8601, CO5300, RM67162...)
// Every command is sent in its own CS frame as:
//   0x02 (1 line), 24-bit address 0x00 <cmd> 0x00 (1 line), parameters (1 line)
// Pixels are sent as:
//   0x32 (1 line), 24-bit address 0x00 2C|3C 0x00 (1 line), pixel data on 4 lines
//   or with <lines> = 1: 0x02 (1 line), same address, pixel data on D0 only (~4x slower)
//
// Descriptor header:
//   :H,<name>,<w>,<h>,16,QSPI,<bus>,<cs>,<clk>,<d0>,<d1>,<d2>,<d3>,<bl>,<rst>,<mhz>[,<lines>]
//   bus: 1 = SPI2 (FSPI), 2 = SPI3 (if available on the SoC)
//   lines: pixel data lines, 4 (default) or 1

#pragma once

#ifdef ESP32

#include "uDisplay_panel.h"
#include "uDisplay_SPI_panel.h"     // SPIPanelConfig (commands, rotation, offsets)
#include "driver/spi_master.h"

struct QSPIBusConfig {
    uint8_t bus_nr;       // 1 = SPI2_HOST, 2 = SPI3_HOST
    int8_t cs;
    int8_t clk;
    int8_t d0;
    int8_t d1;
    int8_t d2;
    int8_t d3;
    uint8_t speed_mhz;
    uint8_t data_lines;   // pixel data lines: 4 (default) or 1 (D0 only, 0x02 framing)
};

class QSPIPanel : public UniversalPanel {
public:
    QSPIPanel(const SPIPanelConfig& config, const QSPIBusConfig& bus,
              const uint8_t* init_cmds, uint16_t init_cmds_len);
    ~QSPIPanel();

    bool isReady() const { return ready; }

    // ===== UniversalPanel Interface =====
    bool drawPixel(int16_t x, int16_t y, uint16_t color) override;
    bool fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
    bool pushColors(uint16_t *data, uint32_t len, bool not_swapped = false) override;
    bool setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1) override;
    bool drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override;
    bool drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) override;

    bool displayOnff(int8_t on) override;
    bool invertDisplay(bool invert) override;
    bool setRotation(uint8_t rotation) override;
    bool updateFrame() override;

    // Send a DCS command with optional parameters (1-line, own CS frame)
    void writeCommand(uint8_t cmd, const uint8_t* params = nullptr, uint32_t len = 0);

private:
    static constexpr uint32_t CHUNK_PIXELS = 2048;     // pixels per DMA transaction
    static constexpr uint8_t  CMD_WRITE_1LINE = 0x02;
    static constexpr uint8_t  CMD_WRITE_4LINE = 0x32;
    static constexpr uint8_t  DCS_RAMWR_CONTINUE = 0x3C;

    SPIPanelConfig cfg;
    QSPIBusConfig bus_cfg;
    spi_host_device_t host;
    spi_device_handle_t dev = nullptr;
    bool bus_initialized = false;
    bool ready = false;

    uint16_t* dma_buf[2] = { nullptr, nullptr };
    spi_transaction_ext_t trans[2];

    uint8_t rotation = 0;
    uint16_t width;
    uint16_t height;
    bool ram_write_start = true;    // next pixel push starts with RAMWR (2C) instead of continue (3C)

    void csLow();
    void csHigh();
    void sendInitCommands(const uint8_t* cmds, uint16_t len);
    void setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);   // inclusive coordinates
    void sendPixels(const uint16_t* data, uint32_t len, bool swap);
    void sendRepeat(uint16_t color, uint32_t len);
    void startPixelTrans(uint32_t idx, const uint16_t* buf, uint32_t n, bool first);
};

#endif // ESP32
