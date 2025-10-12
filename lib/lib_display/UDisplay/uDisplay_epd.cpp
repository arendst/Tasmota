#include "uDisplay.h"
#include "uDisplay_config.h"
#include "uDisplay_spi.h"


// ===== EPD Command Definitions =====
static constexpr uint8_t DRIVER_OUTPUT_CONTROL                       = 0x01;
static constexpr uint8_t BOOSTER_SOFT_START_CONTROL                  = 0x0C;
static constexpr uint8_t GATE_SCAN_START_POSITION                    = 0x0F;
static constexpr uint8_t DEEP_SLEEP_MODE                             = 0x10;
static constexpr uint8_t DATA_ENTRY_MODE_SETTING                     = 0x11;
static constexpr uint8_t SW_RESET                                    = 0x12;
static constexpr uint8_t TEMPERATURE_SENSOR_CONTROL                  = 0x1A;
static constexpr uint8_t MASTER_ACTIVATION                           = 0x20;
static constexpr uint8_t DISPLAY_UPDATE_CONTROL_1                    = 0x21;
static constexpr uint8_t DISPLAY_UPDATE_CONTROL_2                    = 0x22;
static constexpr uint8_t WRITE_RAM                                   = 0x24;
static constexpr uint8_t WRITE_VCOM_REGISTER                         = 0x2C;
static constexpr uint8_t WRITE_LUT_REGISTER                          = 0x32;
static constexpr uint8_t SET_DUMMY_LINE_PERIOD                       = 0x3A;
static constexpr uint8_t SET_GATE_TIME                               = 0x3B;
static constexpr uint8_t BORDER_WAVEFORM_CONTROL                     = 0x3C;
static constexpr uint8_t SET_RAM_X_ADDRESS_START_END_POSITION        = 0x44;
static constexpr uint8_t SET_RAM_Y_ADDRESS_START_END_POSITION        = 0x45;
static constexpr uint8_t SET_RAM_X_ADDRESS_COUNTER                   = 0x4E;
static constexpr uint8_t SET_RAM_Y_ADDRESS_COUNTER                   = 0x4F;
static constexpr uint8_t TERMINATE_FRAME_READ_WRITE                  = 0xFF;

// ===== EPD Initialization and Control =====

void uDisplay::Init_EPD(int8_t p) {
    if (p == DISPLAY_INIT_PARTIAL) {
        if (lutpsize) {
            AddLog(LOG_LEVEL_DEBUG, PSTR("DSP: init partial epaper mode"));
            SetLut(lut_partial);
            Updateframe_EPD();
            delay_sync(lutptime * 10);
        }
        return;
    } else if (p == DISPLAY_INIT_FULL) {
        AddLog(LOG_LEVEL_DEBUG, PSTR("DSP: init full epaper mode"));
        if (lutfsize) {
            SetLut(lut_full);
            Updateframe_EPD();
        }
        if (ep_mode == 2) {
            ClearFrame_42();
            DisplayFrame_42();
        }
        delay_sync(lutftime * 10);
        return;
    }

    if (ep_mode == 2) Init_EPD(DISPLAY_INIT_FULL);
}

void uDisplay::SetLut(const unsigned char* lut) {
    spi_command_EPD(lut_cmd[0]);
    for (int i = 0; i < lutfsize; i++) {
        spi_data8_EPD(lut[i]);
    }
}

void uDisplay::SetLuts(void) {
    uint8_t index, count;
    for (index = 0; index < MAX_LUTS; index++) {
        spi_command_EPD(lut_cmd[index]);
        for (count = 0; count < lut_cnt[index]; count++) {
            spi_data8_EPD(lut_array[index][count]);
        }
    }
}

// ===== EPD Frame Memory Management =====

void uDisplay::ClearFrameMemory(unsigned char color) {
    SetMemoryArea(0, 0, gxs - 1, gys - 1);
    SetMemoryPointer(0, 0);
    spi_command_EPD(WRITE_RAM);
    for (int i = 0; i < gxs / 8 * gys; i++) {
        spi_data8_EPD(color);
    }
}

void uDisplay::ClearFrame_42(void) {
    spi_command_EPD(saw_1);
    for (uint16_t j = 0; j < gys; j++) {
        for (uint16_t i = 0; i < gxs; i++) {
            spi_data8_EPD(0xFF);
        }
    }

    spi_command_EPD(saw_2);
    for (uint16_t j = 0; j < gys; j++) {
        for (uint16_t i = 0; i < gxs; i++) {
            spi_data8_EPD(0xFF);
        }
    }

    spi_command_EPD(saw_3);
    delay_sync(100);
    AddLog(LOG_LEVEL_DEBUG, PSTR("DSP: EPD Clearframe"));
}

void uDisplay::DisplayFrame_42(void) {
    spi_command_EPD(saw_1);
    for(int i = 0; i < gxs / 8 * gys; i++) {
        spi_data8_EPD(0xFF);
    }
    delay(2);

    spi_command_EPD(saw_2);
    for(int i = 0; i < gxs / 8 * gys; i++) {
        spi_data8_EPD(framebuffer[i]^0xff);
    }
    delay(2);

    SetLuts();

    spi_command_EPD(saw_3);
    delay_sync(100);

    AddLog(LOG_LEVEL_DEBUG, PSTR("DSP: EPD Displayframe"));
}

void uDisplay::DisplayFrame_29(void) {
    spi_command_EPD(DISPLAY_UPDATE_CONTROL_2);
    spi_data8_EPD(0xC4);
    spi_command_EPD(MASTER_ACTIVATION);
    spi_data8_EPD(TERMINATE_FRAME_READ_WRITE);
}

// ===== EPD Memory Addressing =====

void uDisplay::SetMemoryArea(int x_start, int y_start, int x_end, int y_end) {
    int x_start1 = (x_start >> 3) & 0xFF;
    int x_end1 = (x_end >> 3) & 0xFF;
    int y_start1 = y_start & 0xFF;
    int y_start2 = (y_start >> 8) & 0xFF;
    int y_end1 = y_end & 0xFF;
    int y_end2 = (y_end >> 8) & 0xFF;

    if (ep_mode == 3) {
        spi_command_EPD(SET_RAM_X_ADDRESS_START_END_POSITION);
        spi_data8_EPD(x_start1);
        spi_data8_EPD(x_end1);
        spi_command_EPD(SET_RAM_Y_ADDRESS_START_END_POSITION);
        spi_data8_EPD(y_end1);
        spi_data8_EPD(y_end2);
        spi_data8_EPD(y_start1);
        spi_data8_EPD(y_start2);
    } else {
        spi_command_EPD(SET_RAM_X_ADDRESS_START_END_POSITION);
        spi_data8_EPD(x_start1);
        spi_data8_EPD(x_end1);
        spi_command_EPD(SET_RAM_Y_ADDRESS_START_END_POSITION);
        spi_data8_EPD(y_start1);
        spi_data8_EPD(y_start2);
        spi_data8_EPD(y_end1);
        spi_data8_EPD(y_end2);
    }
}

void uDisplay::SetMemoryPointer(int x, int y) {
    int x1;
    int y1;
    int y2;

    if (ep_mode == 3) {
        x1 = (x >> 3) & 0xFF;
        y--;
        y1 = y & 0xFF;
        y2 = (y >> 8) & 0xFF;
    } else {
        x1 = (x >> 3) & 0xFF;
        y1 = y & 0xFF;
        y2 = (y >> 8) & 0xFF;
    }

    spi_command_EPD(SET_RAM_X_ADDRESS_COUNTER);
    spi_data8_EPD(x1);
    spi_command_EPD(SET_RAM_Y_ADDRESS_COUNTER);
    spi_data8_EPD(y1);
    spi_data8_EPD(y2);
}

// ===== EPD Frame Updates =====

void uDisplay::Updateframe_EPD(void) {
    if (ep_mode == 1 || ep_mode == 3) {
        switch (ep_update_mode) {
            case DISPLAY_INIT_PARTIAL:
                if (epc_part_cnt) {
                    send_spi_cmds(epcoffs_part, epc_part_cnt);
                }
                break;
            case DISPLAY_INIT_FULL:
                if (epc_full_cnt) {
                    send_spi_cmds(epcoffs_full, epc_full_cnt);
                }
                break;
            default:
                SetFrameMemory(framebuffer, 0, 0, gxs, gys);
                DisplayFrame_29();
        }
    } else {
        DisplayFrame_42();
    }
}

void uDisplay::SetFrameMemory(const unsigned char* image_buffer) {
    SetMemoryArea(0, 0, gxs - 1, gys - 1);
    SetMemoryPointer(0, 0);
    spi_command_EPD(WRITE_RAM);
    for (int i = 0; i < gxs / 8 * gys; i++) {
        spi_data8_EPD(image_buffer[i] ^ 0xff);
    }
}

void uDisplay::SetFrameMemory(const unsigned char* image_buffer,
    uint16_t x, uint16_t y, uint16_t image_width, uint16_t image_height) {
    
    uint16_t x_end;
    uint16_t y_end;

    if (image_buffer == NULL || x < 0 || image_width < 0 || y < 0 || image_height < 0) {
        return;
    }

    x &= 0xFFF8;
    image_width &= 0xFFF8;
    if (x + image_width >= gxs) {
        x_end = gxs - 1;
    } else {
        x_end = x + image_width - 1;
    }
    if (y + image_height >= gys) {
        y_end = gys - 1;
    } else {
        y_end = y + image_height - 1;
    }

    if (!x && !y && image_width == gxs && image_height == gys) {
        SetFrameMemory(image_buffer);
        return;
    }

    SetMemoryArea(x, y, x_end, y_end);
    SetMemoryPointer(x, y);
    spi_command_EPD(WRITE_RAM);
    for (uint16_t j = 0; j < y_end - y + 1; j++) {
        for (uint16_t i = 0; i < (x_end - x + 1) / 8; i++) {
            spi_data8_EPD(image_buffer[i + j * (image_width / 8)] ^ 0xff);
        }
    }
}

void uDisplay::Send_EP_Data() {
    uint16_t image_width = gxs & 0xFFF8;
    uint16_t x = 0;
    uint16_t y = 0;
    uint16_t x_end = gxs - 1;
    uint16_t y_end = gys - 1;

    for (uint16_t j = 0; j < y_end - y + 1; j++) {
        for (uint16_t i = 0; i < (x_end - x + 1) / 8; i++) {
            spi_data8_EPD(framebuffer[i + j * (image_width / 8)] ^ 0xff);
        }
    }
}

// ===== EPD Drawing Primitives =====
#define IF_INVERT_COLOR 1
#define renderer_swap(a, b) { int16_t t = a; a = b; b = t; }

void uDisplay::DrawAbsolutePixel(int x, int y, int16_t color) {
    int16_t w = width(), h = height();
    if (cur_rot == 1 || cur_rot == 3) {
        renderer_swap(w, h);
    }

    if (x < 0 || x >= w || y < 0 || y >= h) {
        return;
    }
    
    if (IF_INVERT_COLOR) {
        if (color) {
            framebuffer[(x + y * w) / 8] |= 0x80 >> (x % 8);
        } else {
            framebuffer[(x + y * w) / 8] &= ~(0x80 >> (x % 8));
        }
    } else {
        if (color) {
            framebuffer[(x + y * w) / 8] &= ~(0x80 >> (x % 8));
        } else {
            framebuffer[(x + y * w) / 8] |= 0x80 >> (x % 8);
        }
    }
}

void uDisplay::drawPixel_EPD(int16_t x, int16_t y, uint16_t color) {
    if (!framebuffer) return;
    if ((x < 0) || (x >= width()) || (y < 0) || (y >= height()))
        return;

    switch (cur_rot) {
    case 1:
        renderer_swap(x, y);
        x = gxs - x - 1;
        break;
    case 2:
        x = gxs - x - 1;
        y = gys - y - 1;
        break;
    case 3:
        renderer_swap(x, y);
        y = gys - y - 1;
        break;
    }

    DrawAbsolutePixel(x, y, color);
}

void uDisplay::fillRect_EPD(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    for (uint32_t yp = y; yp < y + h; yp++) {
        for (uint32_t xp = x; xp < x + w; xp++) {
            drawPixel_EPD(xp, yp, color);
        }
    }
}

void uDisplay::drawFastVLine_EPD(int16_t x, int16_t y, int16_t h, uint16_t color) {
    while (h--) {
        drawPixel_EPD(x, y, color);
        y++;
    }
}

void uDisplay::drawFastHLine_EPD(int16_t x, int16_t y, int16_t w, uint16_t color) {
    while (w--) {
        drawPixel_EPD(x, y, color);
        x++;
    }
}

// ===== EPD SPI Helpers =====

void uDisplay::spi_command_EPD(uint8_t val) {
    SPI_BEGIN_TRANSACTION
    SPI_CS_LOW
    ulcd_command(val);
    SPI_CS_HIGH
    SPI_END_TRANSACTION
}

void uDisplay::spi_data8_EPD(uint8_t val) {
    SPI_BEGIN_TRANSACTION
    SPI_CS_LOW
    ulcd_data8(val);
    SPI_CS_HIGH
    SPI_END_TRANSACTION
}