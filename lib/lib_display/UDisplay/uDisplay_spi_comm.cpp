#include "uDisplay.h"
#include "uDisplay_config.h"
#include "uDisplay_spi.h"  // Your SPI header

// ===== High-Level SPI Communication Functions =====

void uDisplay::ulcd_command(uint8_t val) {
    if (interface == _UDSP_SPI) {
        if (spi_dc < 0) {
            if (spi_nr > 2) {
                if (spi_nr == 3) {
                    write9(val, 0);
                } else {
                    write9_slow(val, 0);
                }
            } else {
                hw_write9(val, 0);
            }
        } else {
            SPI_DC_LOW
            if (spi_nr > 2) {
                if (spi_nr == 3) {
                    write8(val);
                } else {
                    write8_slow(val);
                }
            } else {
                uspi->write(val);
            }
            SPI_DC_HIGH
        }
        return;
    }

#ifdef USE_ESP32_S3
    if (interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {
        pb_writeCommand(val, 8);
    }
#endif
}

void uDisplay::ulcd_data8(uint8_t val) {
    if (interface == _UDSP_SPI) {
        if (spi_dc < 0) {
            if (spi_nr > 2) {
                if (spi_nr == 3) {
                    write9(val, 1);
                } else {
                    write9_slow(val, 1);
                }
            } else {
                hw_write9(val, 1);
            }
        } else {
            if (spi_nr > 2) {
                if (spi_nr == 3) {
                    write8(val);
                } else {
                    write8_slow(val);
                }
            } else {
                uspi->write(val);
            }
        }
        return;
    }

#ifdef USE_ESP32_S3
    if (interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {
        pb_writeData(val, 8);
    }
#endif
}

void uDisplay::ulcd_data16(uint16_t val) {
    if (interface == _UDSP_SPI) {
        if (spi_dc < 0) {
            if (spi_nr > 2) {
                write9(val >> 8, 1);
                write9(val, 1);
            } else {
                hw_write9(val >> 8, 1);
                hw_write9(val, 1);
            }
        } else {
            if (spi_nr > 2) {
                write16(val);
            } else {
                uspi->write16(val);
            }
        }
        return;
    }

#ifdef USE_ESP32_S3
    if (interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {
        pb_writeData(val, 16);
    }
#endif
}

void uDisplay::ulcd_data32(uint32_t val) {
    if (interface == _UDSP_SPI) {
        if (spi_dc < 0) {
            if (spi_nr > 2) {
                write9(val >> 24, 1);
                write9(val >> 16, 1);
                write9(val >> 8, 1);
                write9(val, 1);
            } else {
                hw_write9(val >> 24, 1);
                hw_write9(val >> 16, 1);
                hw_write9(val >> 8, 1);
                hw_write9(val, 1);
            }
        } else {
            if (spi_nr > 2) {
                write32(val);
            } else {
                uspi->write32(val);
            }
        }
        return;
    }

#ifdef USE_ESP32_S3
    if (interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {
        pb_writeData(val, 32);
    }
#endif
}

void uDisplay::ulcd_command_one(uint8_t val) {
    if (interface == _UDSP_SPI) {
        SPI_BEGIN_TRANSACTION
        SPI_CS_LOW
        ulcd_command(val);
        SPI_CS_HIGH
        SPI_END_TRANSACTION
    }
}

void uDisplay::WriteColor(uint16_t color) {
    if (col_mode == 18) {
        uint8_t r = (color & 0xF800) >> 11;
        uint8_t g = (color & 0x07E0) >> 5;
        uint8_t b = color & 0x001F;
        r = (r * 255) / 31;
        g = (g * 255) / 63;
        b = (b * 255) / 31;

        ulcd_data8(r);
        ulcd_data8(g);
        ulcd_data8(b);
    } else {
        ulcd_data16(color);
    }
}