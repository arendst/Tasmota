// slimmed down version of the Open Book IL0398 driver.
// https://github.com/joeycastillo/The-Open-Book/blob/master/src/Grayscale_IL0398.h

/**************************************************************************/
/*!
    @brief  Class for interfacing with IL0398 EPD drivers
*/
/**************************************************************************/

#include "Adafruit_EPD.h"

class Grayscale_IL0398 : public Adafruit_EPD {
 public:
    Grayscale_IL0398(int width, int height, int8_t SID, int8_t SCLK, int8_t DC, int8_t RST, int8_t CS, int8_t SRCS, int8_t MISO, int8_t BUSY = -1);
    Grayscale_IL0398(int width, int height, int8_t DC, int8_t RST, int8_t CS, int8_t SRCS, int8_t BUSY = -1, SPIClass *spi = &SPI);

    void begin(bool reset=true);
    void powerUp();
    void update();
    void powerDown();
    void drawPixel(int16_t x, int16_t y, uint16_t color);
protected:
    uint8_t writeRAMCommand(uint8_t index);
    void setRAMAddress(uint16_t x, uint16_t y);
    void busy_wait();
    
private:
    static const unsigned char LUT_VCOM_GRAYSCALE[];
    static const unsigned char LUT_WW_GRAYSCALE[];
    static const unsigned char LUT_WB_GRAYSCALE[];
    static const unsigned char LUT_BW_GRAYSCALE[];
    static const unsigned char LUT_BB_GRAYSCALE[];
};

/*!
 * @file Grayscale_IL0398.h
 *
 * Forked from Adafruit_IL0398.h; copyright notce below.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Dean Miller for Adafruit Industries.
 * Open Book additions by Joey Castillo.
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */
