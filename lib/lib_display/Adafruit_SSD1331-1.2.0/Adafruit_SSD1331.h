/*!
 * @file Adafruit_SSD1331.h
 */

#include "Arduino.h"
#include <Adafruit_GFX.h>
// Tasmota change: use custom version of Adafruit_SPITFT which extends Renderer instead of Adafruit_GFX
#include <Adafruit_SPITFT_Renderer.h>
#include <Adafruit_SPITFT_Macros.h>
#include <SPI.h>

/*!
 * @brief Select one of these defines to set the pixel color order
 */
#define SSD1331_COLORORDER_RGB
// #define SSD1331_COLORORDER_BGR

#if defined SSD1331_COLORORDER_RGB && defined SSD1331_COLORORDER_BGR
#error "RGB and BGR can not both be defined for SSD1331_COLORODER."
#endif

// Timing Delays
#define SSD1331_DELAYS_HWFILL (3) //!< Fill delay
#define SSD1331_DELAYS_HWLINE (1) //!< Line delay

// SSD1331 Commands
#define SSD1331_CMD_DRAWLINE 0x21      //!< Draw line
#define SSD1331_CMD_DRAWRECT 0x22      //!< Draw rectangle
#define SSD1331_CMD_FILL 0x26          //!< Fill enable/disable
#define SSD1331_CMD_SETCOLUMN 0x15     //!< Set column address
#define SSD1331_CMD_SETROW 0x75        //!< Set row adress
#define SSD1331_CMD_CONTRASTA 0x81     //!< Set contrast for color A
#define SSD1331_CMD_CONTRASTB 0x82     //!< Set contrast for color B
#define SSD1331_CMD_CONTRASTC 0x83     //!< Set contrast for color C
#define SSD1331_CMD_MASTERCURRENT 0x87 //!< Master current control
#define SSD1331_CMD_SETREMAP 0xA0      //!< Set re-map & data format
#define SSD1331_CMD_STARTLINE 0xA1     //!< Set display start line
#define SSD1331_CMD_DISPLAYOFFSET 0xA2 //!< Set display offset
#define SSD1331_CMD_NORMALDISPLAY 0xA4 //!< Set display to normal mode
#define SSD1331_CMD_DISPLAYALLON 0xA5  //!< Set entire display ON
#define SSD1331_CMD_DISPLAYALLOFF 0xA6 //!< Set entire display OFF
#define SSD1331_CMD_INVERTDISPLAY 0xA7 //!< Invert display
#define SSD1331_CMD_SETMULTIPLEX 0xA8  //!< Set multiplex ratio
#define SSD1331_CMD_SETMASTER 0xAD     //!< Set master configuration
#define SSD1331_CMD_DISPLAYOFF 0xAE    //!< Display OFF (sleep mode)
#define SSD1331_CMD_DISPLAYON 0xAF     //!< Normal Brightness Display ON
#define SSD1331_CMD_POWERMODE 0xB0     //!< Power save mode
#define SSD1331_CMD_PRECHARGE 0xB1     //!< Phase 1 and 2 period adjustment
#define SSD1331_CMD_CLOCKDIV                                                   \
  0xB3 //!< Set display clock divide ratio/oscillator frequency
#define SSD1331_CMD_PRECHARGEA 0x8A //!< Set second pre-charge speed for color A
#define SSD1331_CMD_PRECHARGEB 0x8B //!< Set second pre-charge speed for color B
#define SSD1331_CMD_PRECHARGEC 0x8C //!< Set second pre-charge speed for color C
#define SSD1331_CMD_PRECHARGELEVEL 0xBB //!< Set pre-charge voltage
#define SSD1331_CMD_VCOMH 0xBE          //!< Set Vcomh voltge

/// Class to manage hardware interface with SSD1331 chipset
class Adafruit_SSD1331 : public Adafruit_SPITFT_Renderer {
public:
  Adafruit_SSD1331(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst);
  Adafruit_SSD1331(int8_t cs, int8_t dc, int8_t rst);
  // 3-4 args using hardware SPI (must specify peripheral) (reset optional)
  Adafruit_SSD1331(SPIClass *spi, int8_t cs, int8_t dc, int8_t rst = -1);

  // commands
  void begin(uint32_t begin = 8000000);

  void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

  void enableDisplay(boolean enable);

  static const int16_t TFTWIDTH = 96;  ///< The width of the display
  static const int16_t TFTHEIGHT = 64; ///< The height of the display

private:
};
