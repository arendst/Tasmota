// slimmed down version of the Open Book IL0398 driver.
// https://github.com/joeycastillo/The-Open-Book/blob/master/src/Grayscale_IL0398.cpp

#include "Grayscale_IL0398.h"

#define BUSY_WAIT 500

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif
/**************************************************************************/
/*!
    @brief constructor if using external SRAM chip and software SPI
    @param width the width of the display in pixels
    @param height the height of the display in pixels
    @param SID the SID pin to use
    @param SCLK the SCLK pin to use
    @param DC the data/command pin to use
    @param RST the reset pin to use
    @param CS the chip select pin to use
    @param SRCS the SRAM chip select pin to use
    @param MISO the MISO pin to use
    @param BUSY the busy pin to use
*/
/**************************************************************************/
Grayscale_IL0398::Grayscale_IL0398(int width, int height, 
         int8_t SID, int8_t SCLK, int8_t DC, int8_t RST, 
         int8_t CS, int8_t SRCS, int8_t MISO, int8_t BUSY) : 
  Adafruit_EPD(width, height, SID, SCLK, DC, RST, CS, SRCS, MISO, BUSY) {

  buffer1_size = ((uint32_t)width * (uint32_t)height) / 8;
  buffer2_size = buffer1_size;

  if (SRCS >= 0) {
    use_sram = true;
    buffer1_addr = 0;
    buffer2_addr = buffer1_size;
    buffer1 = buffer2 = NULL;
  } else {
    buffer1 = (uint8_t *)malloc(buffer1_size);
    buffer2 = (uint8_t *)malloc(buffer2_size);
  }
}

// constructor for hardware SPI - we indicate DataCommand, ChipSelect, Reset
/**************************************************************************/
/*!
    @brief constructor if using on-chip RAM and hardware SPI
    @param width the width of the display in pixels
    @param height the height of the display in pixels
    @param DC the data/command pin to use
    @param RST the reset pin to use
    @param CS the chip select pin to use
    @param SRCS the SRAM chip select pin to use
    @param BUSY the busy pin to use
*/
/**************************************************************************/
Grayscale_IL0398::Grayscale_IL0398(int width, int height, 
         int8_t DC, int8_t RST, 
         int8_t CS, int8_t SRCS, int8_t BUSY, SPIClass *spi) :
  Adafruit_EPD(width, height, DC, RST, CS, SRCS, BUSY, spi) {

  buffer1_size = ((uint32_t)width * (uint32_t)height) / 8;
  buffer2_size = buffer1_size;

  if (SRCS >= 0) {
    use_sram = true;
    buffer1_addr = 0;
    buffer2_addr = 0;
    buffer1 = buffer2 = NULL;
  } else {
    buffer1 = (uint8_t *)malloc(buffer1_size);
    buffer2 = (uint8_t *)malloc(buffer1_size);
  }
}

/**************************************************************************/
/*!
    @brief wait for busy signal to end
*/
/**************************************************************************/
void Grayscale_IL0398::busy_wait(void)
{
  if (_busy_pin > -1) {
    do {
      EPD_command(IL0398_GETSTATUS);
      delay(10);
    } while (!digitalRead(_busy_pin)); //wait for busy high
    delay(200);
  } else {
    delay(BUSY_WAIT);
  }

}

/**************************************************************************/
/*!
    @brief begin communication with and set up the display.
    @param reset if true the reset pin will be toggled.
*/
/**************************************************************************/
void Grayscale_IL0398::begin(bool reset)
{
  Adafruit_EPD::begin(reset);
  setBlackBuffer(0, true);
  powerDown();
}

/**************************************************************************/
/*!
    @brief signal the display to update
*/
/**************************************************************************/
void Grayscale_IL0398::update()
{
  EPD_command(IL0398_DISPLAY_REFRESH);
  delay(100);

  busy_wait();
  if (_busy_pin <= -1) {
    delay(5000);
  }
}

const unsigned char Grayscale_IL0398::LUT_VCOM_GRAYSCALE[] PROGMEM = 
{
    0x00, 0x0A, 0x00, 0x00, 0x00, 0x01,
    0x60, 0x14, 0x14, 0x00, 0x00, 0x01,
    0x00, 0x14, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x13, 0x0A, 0x01, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00,
    0x00
};

const unsigned char Grayscale_IL0398::LUT_WW_GRAYSCALE[] PROGMEM = 
{
    0x40, 0x0A, 0x00, 0x00, 0x00, 0x01,
    0x90, 0x14, 0x14, 0x00, 0x00, 0x01,
    0x10, 0x14, 0x0A, 0x00, 0x00, 0x01,
    0xA0, 0x13, 0x01, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char Grayscale_IL0398::LUT_WB_GRAYSCALE[] PROGMEM = 
{
    0x40, 0x0A, 0x00, 0x00, 0x00, 0x01,
    0x90, 0x14, 0x14, 0x00, 0x00, 0x01,
    0x00, 0x14, 0x0A, 0x00, 0x00, 0x01,
    0x99, 0x0B, 0x04, 0x04, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char Grayscale_IL0398::LUT_BW_GRAYSCALE[] PROGMEM = 
{
    0x40, 0x0A, 0x00, 0x00, 0x00, 0x01,
    0x90, 0x14, 0x14, 0x00, 0x00, 0x01,
    0x00, 0x14, 0x0A, 0x00, 0x00, 0x01,
    0x99, 0x0C, 0x01, 0x03, 0x04, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char Grayscale_IL0398::LUT_BB_GRAYSCALE[] PROGMEM = 
{
    0x80, 0x0A, 0x00, 0x00, 0x00, 0x01,
    0x90, 0x14, 0x14, 0x00, 0x00, 0x01,
    0x20, 0x14, 0x0A, 0x00, 0x00, 0x01,
    0x50, 0x13, 0x01, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

/**************************************************************************/
/*!
    @brief start up the display. Same as init, but here for compatibility with
           Adafruit_EPD; you can call Grayscale_IL0398::init with more options.
*/
/**************************************************************************/
void Grayscale_IL0398::powerUp() {
  uint8_t buf[5];

  hardwareReset();

  buf[0] = 0x03;    // Panel will generate VDH and VDL (1<<0) and VGH and VGL (1<<1)
  buf[1] = 0x00;    // VCOMH=VDH+VCOMDC and VCOML=VDL+VCOMDC; VGH and VGL are 16v and -16v respectively
  buf[2] = 0x2b;    // VDH= 11V
  buf[3] = 0x2b;    // VDL=-11V
  buf[4] = 0x13;    // VDHR=6.2V
  EPD_command(IL0398_POWER_SETTING, buf, 5);

  buf[0] = 0x17;    // phase A: soft start 10ms, driving strength 3, off time 6.58us
  buf[1] = 0x17;    // phase B: soft start 10ms, driving strength 3, off time 6.58us
  buf[2] = 0x17;    // phase C: driving strength 3, off time 6.58us
  EPD_command(IL0398_BOOSTER_SOFT_START, buf, 3);

  buf[0] = 0x3F; // (1<<4) sets display to monochrome and (1<<5) enables custom LUTs
  EPD_command(IL0398_PANEL_SETTING, buf, 1);
  
  buf[0] = 0x3C; // 50 Hz
  EPD_command(IL0398_PLL, buf, 1);

  buf[0] = (HEIGHT >> 8) & 0xFF;
  buf[1] = HEIGHT & 0xFF;
  buf[2] = (WIDTH >> 8) & 0xFF;
  buf[3] = WIDTH & 0xFF;
  EPD_command(IL0398_RESOLUTION, buf, 4);
  
  buf[0] = 0x12; // VCOM_DC = -1.5v
  EPD_command(IL0398_VCM_DC_SETTING, buf, 1);
  
  buf[0] = 0xD7; // 0x57 for black border. 0x97 for white border. 0xD7 for floating border.
  EPD_command(IL0398_VCOM, buf, 1);
  
  EPD_command(IL0398_LUT1, LUT_VCOM_GRAYSCALE, sizeof(LUT_VCOM_GRAYSCALE));
  EPD_command(IL0398_LUTWW, LUT_WW_GRAYSCALE, sizeof(LUT_WW_GRAYSCALE));
  EPD_command(IL0398_LUTBW, LUT_BW_GRAYSCALE, sizeof(LUT_BW_GRAYSCALE));
  EPD_command(IL0398_LUTWB, LUT_WB_GRAYSCALE, sizeof(LUT_WB_GRAYSCALE));
  EPD_command(IL0398_LUTBB, LUT_BB_GRAYSCALE, sizeof(LUT_BB_GRAYSCALE));

  EPD_command(IL0398_POWER_ON);
  busy_wait();

  delay(20);
}

/**************************************************************************/
/*!
    @brief wind down the display
*/
/**************************************************************************/
void Grayscale_IL0398::powerDown()
{
  uint8_t buf[4];

  // power off
  buf[0] = 0xF7; // border floating
  EPD_command(IL0398_VCOM, buf, 1);
  EPD_command(IL0398_POWER_OFF);
  busy_wait();
  buf[0] = 0xA5; // deep sleep
  EPD_command(IL0398_DEEP_SLEEP, buf, 1);
  delay(100);
}

/**************************************************************************/
/*!
    @brief draw a single pixel on the screen
  @param x the x axis position
  @param y the y axis position
  @param color the color of the pixel
*/
/**************************************************************************/
void Grayscale_IL0398::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((x < 0) || (x >= width()) || (y < 0) || (y >= height()))
  return;
      
  uint8_t *pByte1;
  uint8_t *pByte2;

  // check rotation, move pixel around if necessary
  switch (getRotation()) {
    case 1:
    EPD_swap(x, y);
    x = WIDTH - x - 1;
    break;
    case 2:
    x = WIDTH - x - 1;
    y = HEIGHT - y - 1;
    break;
    case 3:
    EPD_swap(x, y);
    y = HEIGHT - y - 1;
    break;
  }
      
  uint16_t addr = ( (uint32_t)(WIDTH - 1 - x) * (uint32_t)HEIGHT + y)/8;

  if (use_sram) {
    uint8_t byte1 = sram.read8(blackbuffer_addr + addr);
    uint8_t byte2 = sram.read8(colorbuffer_addr + addr);
    pByte1 = &byte1;
    pByte2 = &byte2;
  } else {
    pByte1 = black_buffer + addr;
    pByte2 = color_buffer + addr;
  }
  
  switch (color) {
    case EPD_BLACK:   
        *pByte1 &= ~(1 << (7 - (y%8)));
        *pByte2 &= ~(1 << (7 - (y%8)));
      break;
    case EPD_DARK:
        *pByte1 |= (1 << (7 - (y%8)));
        *pByte2 &= ~(1 << (7 - (y%8)));
        break;
    case EPD_LIGHT:
        *pByte1 &= ~(1 << (7 - (y%8)));
        *pByte2 |= (1 << (7 - (y%8)));
        break;
    case EPD_WHITE:
        *pByte1 |= (1 << (7 - (y%8)));
        *pByte2 |= (1 << (7 - (y%8)));
        break;
  }
  
  if (use_sram) {
    sram.write8(addr, *pByte1);
    sram.write8(addr + buffer1_size, *pByte2);
  }
}

/**************************************************************************/
/*!
    @brief Send the specific command to start writing to EPD display RAM
    @param index The index for which buffer to write (0 or 1 or tri-color displays)
    Ignored for monochrome displays.
    @returns The byte that is read from SPI at the same time as sending the command
*/
/**************************************************************************/
uint8_t Grayscale_IL0398::writeRAMCommand(uint8_t index) {
  if (index == 0) {
    return EPD_command(IL0398_DTM2, false);
  }
  if (index == 1) {
    return EPD_command(IL0398_DTM1, false);
  }
  return 0;
}

/**************************************************************************/
/*!
    @brief Some displays require setting the RAM address pointer
    @param x X address counter value
    @param y Y address counter value
*/
/**************************************************************************/
void Grayscale_IL0398::setRAMAddress(uint16_t x, uint16_t y) {
  // on this chip we do nothing
}

/*!
 * @file Adafruit_IL0398.cpp
 *
 * Forked from Adafruit_IL0398.cpp; copyright notce below.
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
