// ======================================================
// uDisplay_QSPI_panel.cpp - QSPI (quad SPI) LCD/AMOLED Panel
// ======================================================

#ifdef ESP32

#include "uDisplay_QSPI_panel.h"
#include <Arduino.h>
#include "esp_heap_caps.h"
#include "soc/soc_caps.h"

extern void AddLog(uint32_t loglevel, const char* formatP, ...);

QSPIPanel::QSPIPanel(const SPIPanelConfig& config, const QSPIBusConfig& bus,
                     const uint8_t* init_cmds, uint16_t init_cmds_len)
    : cfg(config), bus_cfg(bus)
{
    width = cfg.width;
    height = cfg.height;
    memset(trans, 0, sizeof(trans));

#if SOC_SPI_PERIPH_NUM > 2
    host = (bus_cfg.bus_nr == 2) ? SPI3_HOST : SPI2_HOST;
#else
    host = SPI2_HOST;
#endif

    uint32_t speed_mhz = bus_cfg.speed_mhz;
    if (speed_mhz == 0 || speed_mhz == 0xFF) { speed_mhz = 40; }

    // CS is driven manually: it must stay low across several transactions
    // (pixel stream split into DMA chunks)
    if (bus_cfg.cs >= 0) {
        pinMode(bus_cfg.cs, OUTPUT);
        digitalWrite(bus_cfg.cs, HIGH);
    }

    spi_bus_config_t buscfg;
    memset(&buscfg, 0, sizeof(buscfg));
    buscfg.data0_io_num = bus_cfg.d0;
    buscfg.data1_io_num = bus_cfg.d1;
    buscfg.sclk_io_num = bus_cfg.clk;
    buscfg.data2_io_num = bus_cfg.d2;
    buscfg.data3_io_num = bus_cfg.d3;
    buscfg.data4_io_num = -1;
    buscfg.data5_io_num = -1;
    buscfg.data6_io_num = -1;
    buscfg.data7_io_num = -1;
    buscfg.max_transfer_sz = CHUNK_PIXELS * 2;
    buscfg.flags = SPICOMMON_BUSFLAG_MASTER | SPICOMMON_BUSFLAG_GPIO_PINS | SPICOMMON_BUSFLAG_QUAD;

    esp_err_t ret = spi_bus_initialize(host, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        AddLog(1, "DSP: QSPI bus init failed %d", ret);
        return;
    }
    bus_initialized = true;

    spi_device_interface_config_t devcfg;
    memset(&devcfg, 0, sizeof(devcfg));
    devcfg.command_bits = 8;
    devcfg.address_bits = 24;
    devcfg.dummy_bits = 0;
    devcfg.mode = 0;
    devcfg.clock_speed_hz = (int)(speed_mhz * 1000000);
    devcfg.spics_io_num = -1;               // manual CS
    devcfg.flags = SPI_DEVICE_HALFDUPLEX;
    devcfg.queue_size = 1;

    ret = spi_bus_add_device(host, &devcfg, &dev);
    if (ret != ESP_OK) {
        AddLog(1, "DSP: QSPI add device failed %d", ret);
        dev = nullptr;
        return;
    }

    for (uint32_t i = 0; i < 2; i++) {
        dma_buf[i] = (uint16_t*)heap_caps_malloc(CHUNK_PIXELS * 2, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
        if (!dma_buf[i]) {
            AddLog(1, "DSP: QSPI no DMA memory");
            return;
        }
    }

    ready = true;
    sendInitCommands(init_cmds, init_cmds_len);
    AddLog(2, "DSP: QSPI panel host %d cs %d clk %d d0-3 %d,%d,%d,%d %d MHz, %d data line%s",
           host, bus_cfg.cs, bus_cfg.clk, bus_cfg.d0, bus_cfg.d1, bus_cfg.d2, bus_cfg.d3, speed_mhz,
           bus_cfg.data_lines, (bus_cfg.data_lines == 1) ? "" : "s");
}

QSPIPanel::~QSPIPanel() {
    if (dev) { spi_bus_remove_device(dev); }
    if (bus_initialized) { spi_bus_free(host); }
    for (uint32_t i = 0; i < 2; i++) {
        if (dma_buf[i]) { heap_caps_free(dma_buf[i]); }
    }
}

// ===== Low level =====

void QSPIPanel::csLow() {
    if (bus_cfg.cs >= 0) { digitalWrite(bus_cfg.cs, LOW); }
}

void QSPIPanel::csHigh() {
    if (bus_cfg.cs >= 0) { digitalWrite(bus_cfg.cs, HIGH); }
}

void QSPIPanel::writeCommand(uint8_t cmd, const uint8_t* params, uint32_t len) {
    if (!ready) { return; }
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.cmd = CMD_WRITE_1LINE;
    t.addr = ((uint32_t)cmd) << 8;
    if (params && len) {
        if (len > CHUNK_PIXELS * 2) { len = CHUNK_PIXELS * 2; }
        if (len <= 4) {
            t.flags = SPI_TRANS_USE_TXDATA;
            memcpy(t.tx_data, params, len);
        } else {
            memcpy(dma_buf[0], params, len);
            t.tx_buffer = dma_buf[0];
        }
        t.length = len * 8;
    }
    csLow();
    spi_device_polling_transmit(dev, &t);
    csHigh();
}

// Init stream uses the uDisplay SPI format: CMD,CNT,ARG1..ARGn
// CNT & 0x1F = number of args, CNT & 0xE0 = delay code (80=150ms, A0=10ms, E0=500ms)
void QSPIPanel::sendInitCommands(const uint8_t* cmds, uint16_t len) {
    if (!cmds) { return; }
    uint16_t i = 0;
    while (i + 1 < len) {
        uint8_t cmd = cmds[i++];
        uint8_t cnt = cmds[i++];
        uint32_t nargs = cnt & 0x1F;
        if (i + nargs > len) { nargs = len - i; }
        writeCommand(cmd, &cmds[i], nargs);
        i += nargs;
        uint32_t delay_ms = 0;
        switch (cnt & 0xE0) {
            case 0x80: delay_ms = 150; break;
            case 0xA0: delay_ms = 10;  break;
            case 0xE0: delay_ms = 500; break;
        }
        if (delay_ms) { delay(delay_ms); }
    }
}

void QSPIPanel::setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    x0 += cfg.x_addr_offset[rotation];
    x1 += cfg.x_addr_offset[rotation];
    y0 += cfg.y_addr_offset[rotation];
    y1 += cfg.y_addr_offset[rotation];
    uint8_t xa[4] = { (uint8_t)(x0 >> 8), (uint8_t)x0, (uint8_t)(x1 >> 8), (uint8_t)x1 };
    uint8_t ya[4] = { (uint8_t)(y0 >> 8), (uint8_t)y0, (uint8_t)(y1 >> 8), (uint8_t)y1 };
    writeCommand(cfg.cmd_set_addr_x, xa, 4);
    writeCommand(cfg.cmd_set_addr_y, ya, 4);
    ram_write_start = true;
}

// Start a pixel transaction. The first one of a CS frame carries the
// 0x32 (or 0x02 in 1-line mode) + RAMWR header, the following ones are raw
// data continuing the stream.
void QSPIPanel::startPixelTrans(uint32_t idx, const uint16_t* buf, uint32_t n, bool first) {
    spi_transaction_ext_t* t = &trans[idx];
    memset(t, 0, sizeof(spi_transaction_ext_t));
    bool quad = (bus_cfg.data_lines != 1);
    uint32_t mode_flags = quad ? SPI_TRANS_MODE_QIO : 0;
    if (first) {
        uint8_t ramcmd = (ram_write_start && cfg.cmd_write_ram != 0xFF) ? cfg.cmd_write_ram : DCS_RAMWR_CONTINUE;
        t->base.flags = mode_flags;
        t->base.cmd = quad ? CMD_WRITE_4LINE : CMD_WRITE_1LINE;
        t->base.addr = ((uint32_t)ramcmd) << 8;
        ram_write_start = false;
    } else {
        t->base.flags = mode_flags | SPI_TRANS_VARIABLE_CMD | SPI_TRANS_VARIABLE_ADDR | SPI_TRANS_VARIABLE_DUMMY;
        t->command_bits = 0;
        t->address_bits = 0;
        t->dummy_bits = 0;
    }
    t->base.tx_buffer = buf;
    t->base.length = n * 16;
    spi_device_polling_start(dev, &t->base, portMAX_DELAY);
}

// Send pixels, converting to big-endian (MSB first) if `swap`.
// Double buffered: next chunk is prepared while the previous one is transferred.
void QSPIPanel::sendPixels(const uint16_t* data, uint32_t len, bool swap) {
    bool first = true;
    bool pending = false;
    uint32_t idx = 0;
    csLow();
    while (len) {
        uint32_t n = (len > CHUNK_PIXELS) ? CHUNK_PIXELS : len;
        uint16_t* buf = dma_buf[idx];
        if (swap) {
            for (uint32_t k = 0; k < n; k++) {
                uint16_t c = data[k];
                buf[k] = (c << 8) | (c >> 8);
            }
        } else {
            memcpy(buf, data, n * 2);
        }
        data += n;
        len -= n;
        if (pending) { spi_device_polling_end(dev, portMAX_DELAY); }
        startPixelTrans(idx, buf, n, first);
        pending = true;
        first = false;
        idx ^= 1;
    }
    if (pending) { spi_device_polling_end(dev, portMAX_DELAY); }
    csHigh();
}

void QSPIPanel::sendRepeat(uint16_t color, uint32_t len) {
    if (!len) { return; }
    uint16_t be_color = (color << 8) | (color >> 8);
    uint32_t fill = (len > CHUNK_PIXELS) ? CHUNK_PIXELS : len;
    uint16_t* buf = dma_buf[0];
    for (uint32_t k = 0; k < fill; k++) { buf[k] = be_color; }

    bool first = true;
    bool pending = false;
    uint32_t idx = 0;
    csLow();
    while (len) {
        uint32_t n = (len > CHUNK_PIXELS) ? CHUNK_PIXELS : len;
        len -= n;
        if (pending) { spi_device_polling_end(dev, portMAX_DELAY); }
        startPixelTrans(idx, buf, n, first);     // same read-only buffer, separate transaction structs
        pending = true;
        first = false;
        idx ^= 1;
    }
    if (pending) { spi_device_polling_end(dev, portMAX_DELAY); }
    csHigh();
}

// ===== UniversalPanel Interface =====

bool QSPIPanel::drawPixel(int16_t x, int16_t y, uint16_t color) {
    return fillRect(x, y, 1, 1, color);
}

bool QSPIPanel::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if (!ready) { return true; }
    if (x < 0) { w += x; x = 0; }
    if (y < 0) { h += y; y = 0; }
    if ((x >= width) || (y >= height) || (w <= 0) || (h <= 0)) { return true; }
    if ((x + w) > width)  { w = width - x; }
    if ((y + h) > height) { h = height - y; }
    setWindow(x, y, x + w - 1, y + h - 1);
    sendRepeat(color, (uint32_t)w * h);
    return true;
}

bool QSPIPanel::pushColors(uint16_t *data, uint32_t len, bool not_swapped) {
    if (!ready || !data || !len) { return true; }
    // not_swapped == true: native RGB565, needs MSB first on the wire
    sendPixels(data, len, not_swapped);
    return true;
}

// x1/y1 are exclusive (LVGL passes x1 + width); (0,0,0,0) ends a transfer
bool QSPIPanel::setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    if (!ready) { return true; }
    if (!x0 && !y0 && !x1 && !y1) { return true; }
    if (x1 <= x0 || y1 <= y0) { return true; }
    setWindow(x0, y0, x1 - 1, y1 - 1);
    return true;
}

bool QSPIPanel::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    return fillRect(x, y, w, 1, color);
}

bool QSPIPanel::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    return fillRect(x, y, 1, h, color);
}

bool QSPIPanel::displayOnff(int8_t on) {
    if (on && cfg.cmd_display_on != 0xFF) {
        writeCommand(cfg.cmd_display_on);
    } else if (!on && cfg.cmd_display_off != 0xFF) {
        writeCommand(cfg.cmd_display_off);
    }
    return false;   // let uDisplay handle the backlight pin, if any
}

bool QSPIPanel::invertDisplay(bool invert) {
    if (invert && cfg.cmd_invert_on != 0xFF) {
        writeCommand(cfg.cmd_invert_on);
    } else if (!invert && cfg.cmd_invert_off != 0xFF) {
        writeCommand(cfg.cmd_invert_off);
    }
    return true;
}

bool QSPIPanel::setRotation(uint8_t rot) {
    rotation = rot & 3;
    if (cfg.cmd_memory_access != 0xFF && cfg.rot_cmd[rotation] != 0xFF) {
        writeCommand(cfg.cmd_memory_access, &cfg.rot_cmd[rotation], 1);
    }
    if (rotation & 1) {
        width = cfg.height;
        height = cfg.width;
    } else {
        width = cfg.width;
        height = cfg.height;
    }
    return true;
}

bool QSPIPanel::updateFrame() {
    return false;
}

#endif // ESP32
