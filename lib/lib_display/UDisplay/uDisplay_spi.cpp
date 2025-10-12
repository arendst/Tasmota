#include "uDisplay.h"
#include "uDisplay_config.h"
#include "uDisplay_spi.h"
#ifndef ESP32
#include "spi_register.h"
#endif

// ===== Low-Level SPI Write Functions =====
// ===== RA8876 Controller Commands =====
static constexpr uint8_t RA8876_DATA_WRITE  = 0x80;
static constexpr uint8_t RA8876_DATA_READ   = 0xC0;
static constexpr uint8_t RA8876_CMD_WRITE   = 0x00;
static constexpr uint8_t RA8876_STATUS_READ = 0x40;

#ifdef ESP32
void uDisplay::hw_write9(uint8_t val, uint8_t dc) {
    if (spi_dc < -1) {
        // RA8876 mode
        if (!dc) {
            uspi->write(RA8876_CMD_WRITE);
            uspi->write(val);
        } else {
            uspi->write(RA8876_DATA_WRITE);
            uspi->write(val);
        }
    } else {
        uint32_t regvalue = val >> 1;
        if (dc) regvalue |= 0x80;
        else regvalue &= 0x7f;
        if (val & 1) regvalue |= 0x8000;

        REG_SET_BIT(SPI_USER_REG(3), SPI_USR_MOSI);
        REG_WRITE(SPI_MOSI_DLEN_REG(3), 9 - 1);
        uint32_t *dp = (uint32_t*)SPI_W0_REG(3);
        *dp = regvalue;
        REG_SET_BIT(SPI_CMD_REG(3), SPI_USR);
        while (REG_GET_FIELD(SPI_CMD_REG(3), SPI_USR));
    }
}
#else
void uDisplay::hw_write9(uint8_t val, uint8_t dc) {
    if (spi_dc < -1) {
        // RA8876 mode
        if (!dc) {
            uspi->write(RA8876_CMD_WRITE);
            uspi->write(val);
        } else {
            uspi->write(RA8876_DATA_WRITE);
            uspi->write(val);
        }
    } else {
        uint32_t regvalue;
        uint8_t bytetemp;
        if (!dc) {
            bytetemp = (val>> 1) & 0x7f;
        } else {
            bytetemp = (val >> 1) | 0x80;
        }
        regvalue = ((8 & SPI_USR_COMMAND_BITLEN) << SPI_USR_COMMAND_BITLEN_S) | ((uint32)bytetemp);
        if (val & 0x01) regvalue |= BIT15;
        while (READ_PERI_REG(SPI_CMD(1)) & SPI_USR);
        WRITE_PERI_REG(SPI_USER2(1), regvalue);
        SET_PERI_REG_MASK(SPI_CMD(1), SPI_USR);
    }
}
#endif

// ===== Software SPI (Slow) Functions =====

void uDisplay::write8(uint8_t val) {
    for (uint8_t bit = 0x80; bit; bit >>= 1) {
        GPIO_CLR(spi_clk);
        if (val & bit) GPIO_SET(spi_mosi);
        else   GPIO_CLR(spi_mosi);
        GPIO_SET(spi_clk);
    }
}

void uDisplay::write8_slow(uint8_t val) {
    for (uint8_t bit = 0x80; bit; bit >>= 1) {
        GPIO_CLR_SLOW(spi_clk);
        if (val & bit) GPIO_SET_SLOW(spi_mosi);
        else   GPIO_CLR_SLOW(spi_mosi);
        GPIO_SET_SLOW(spi_clk);
    }
}

void uDisplay::write9(uint8_t val, uint8_t dc) {
    GPIO_CLR(spi_clk);
    if (dc) GPIO_SET(spi_mosi);
    else  GPIO_CLR(spi_mosi);
    GPIO_SET(spi_clk);

    for (uint8_t bit = 0x80; bit; bit >>= 1) {
        GPIO_CLR(spi_clk);
        if (val & bit) GPIO_SET(spi_mosi);
        else   GPIO_CLR(spi_mosi);
        GPIO_SET(spi_clk);
    }
}

void uDisplay::write9_slow(uint8_t val, uint8_t dc) {
    GPIO_CLR_SLOW(spi_clk);
    if (dc) GPIO_SET_SLOW(spi_mosi);
    else  GPIO_CLR_SLOW(spi_mosi);
    GPIO_SET_SLOW(spi_clk);

    for (uint8_t bit = 0x80; bit; bit >>= 1) {
        GPIO_CLR_SLOW(spi_clk);
        if (val & bit) GPIO_SET_SLOW(spi_mosi);
        else   GPIO_CLR_SLOW(spi_mosi);
        GPIO_SET_SLOW(spi_clk);
    }
}

void uDisplay::write16(uint16_t val) {
    for (uint16_t bit = 0x8000; bit; bit >>= 1) {
        GPIO_CLR(spi_clk);
        if (val & bit) GPIO_SET(spi_mosi);
        else   GPIO_CLR(spi_mosi);
        GPIO_SET(spi_clk);
    }
}

void uDisplay::write32(uint32_t val) {
    for (uint32_t bit = 0x80000000; bit; bit >>= 1) {
        GPIO_CLR(spi_clk);
        if (val & bit) GPIO_SET(spi_mosi);
        else   GPIO_CLR(spi_mosi);
        GPIO_SET(spi_clk);
    }
}

// ===== RA8876 Specific Functions =====

uint8_t uDisplay::writeReg16(uint8_t reg, uint16_t wval) {
    hw_write9(reg, 0);
    hw_write9(wval, 1);
    hw_write9(reg + 1, 0);
    hw_write9(wval >> 8, 1);
    return 0;
}

uint8_t uDisplay::readData(void) {
    uspi->write(RA8876_DATA_READ);
    uint8_t val = uspi->transfer(0);
    return val;
}

uint8_t uDisplay::readStatus(void) {
    uspi->write(RA8876_STATUS_READ);
    uint8_t val = uspi->transfer(0);
    return val;
}