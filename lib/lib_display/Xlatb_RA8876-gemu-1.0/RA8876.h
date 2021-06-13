/***************************************************
  STM32 Support added by Jaret Burkett at OSHlab.com

  This is our library for the Adafruit  RA8876 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#ifndef _RA8876H_
#define _RA8876H_


#include "Arduino.h"
#include <SPI.h>
#include <renderer.h>
#ifdef ESP32
#include "driver/spi_master.h"
#endif

#undef SPRINT
#define SPRINT(A) {char str[32];sprintf(str,"val: %d ",A);Serial.println((char*)str);}


#define RA8876_TFTWIDTH  1024
#define RA8876_TFTHEIGHT 600

struct SdramInfo
{
  int speed;       // MHz
  int casLatency;  // CAS latency (2 or 3)
  int banks;       // Banks (2 or 4)
  int rowBits;     // Row addressing bits (11-13)
  int colBits;     // Column addressing bits (8-12)
  int refresh;     // Refresh time in microseconds
};

struct DisplayInfo
{
  int      width;     // Display width
  int      height;    // Display height

  uint32_t dotClock;  // Pixel clock in kHz

  int      hFrontPorch;  // Will be rounded to the nearest multiple of 8
  int      hBackPorch;
  int      hPulseWidth;  // Will be rounded to the nearest multiple of 8

  int      vFrontPorch;
  int      vBackPorch;
  int      vPulseWidth;
};

// Data sheet section 6.1.
// Output frequency is: (m_oscClock * (n + 1)) / (2 ** k)
// There is also a PLL parameter named 'm', but it's unclear how its value could ever be non-zero.
//  When it is zero, the divisor is (2 ** 0) = 1, so we simply ignore it.
struct PllParams
{
  uint32_t freq;  // Frequency in kHz
  int      n;     // Multiplier less 1 (range 1..63)
  int      k;     // Divisor power of 2 (range 0..3 for CCLK/MCLK; range 0..7 for SCLK)
};

#define RGB332(r, g, b) (((r) & 0xE0) | (((g) & 0xE0) >> 3) | (((b) & 0xE0) >> 6))
#define RGB565(r, g, b) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | (((b) & 0xF8) >> 3))

enum FontSource
{
  RA8876_FONT_SOURCE_INTERNAL,  // CGROM with four 8-bit ISO Latin variants
  RA8876_FONT_SOURCE_EXT_ROM    // External font ROM chip
};

enum FontSize
{
  RA8876_FONT_SIZE_16 = 0x00,
  RA8876_FONT_SIZE_24 = 0x01,
  RA8876_FONT_SIZE_32 = 0x02
};

enum FontEncoding
{
  RA8876_FONT_ENCODING_GB2312   = 0x00,  // GB2312 (Simplified Chinese)
  RA8876_FONT_ENCODING_GB18030  = 0x01,  // GB12345/GB18030 (Chinese)
  RA8876_FONT_ENCODING_BIG5     = 0x02,  // Big5 (Traditional Chinese)

  RA8876_FONT_ENCODING_UNICODE  = 0x03,  // Unicode (UCS-2?)

  RA8876_FONT_ENCODING_ASCII    = 0x04,  // ASCII

  RA8876_FONT_ENCODING_UNIJAPAN = 0x05,  // Uni-Japanese (?)
  RA8876_FONT_ENCODING_JIS0208  = 0x06,  // JIS X 0208 (Shift JIS?)

  RA8876_FONT_ENCODING_LGCATH   = 0x07,  // Latin/Greek/Cyrillic/Arabic/Thai/Hebrew (?)

  RA8876_FONT_ENCODING_8859_1   = 0x11,  // ISO 8859-1 (Latin 1)
  RA8876_FONT_ENCODING_8859_2   = 0x12,  // ISO 8859-2 (Latin 2: Eastern European)
  RA8876_FONT_ENCODING_8859_3   = 0x13,  // ISO 8859-3 (Latin 3: South European)
  RA8876_FONT_ENCODING_8859_4   = 0x14,  // ISO 8859-4 (Latin 4: Northern European)
  RA8876_FONT_ENCODING_8859_5   = 0x15,  // ISO 8859-5 (Latin/Cyrillic)
  RA8876_FONT_ENCODING_8859_7   = 0x16,  // ISO 8859-7 (Latin/Greek)
  RA8876_FONT_ENCODING_8859_8   = 0x17,  // ISO 8859-8 (Latin/Hebrew)
  RA8876_FONT_ENCODING_8859_9   = 0x18,  // ISO 8859-9 (Latin 5: Turkish)
  RA8876_FONT_ENCODING_8859_10  = 0x19,  // ISO 8859-10 (Latin 6: Nordic)
  RA8876_FONT_ENCODING_8859_11  = 0x1A,  // ISO 8859-11 (Latin/Thai)
  RA8876_FONT_ENCODING_8859_13  = 0x1B,  // ISO 8859-13 (Latin 7: Baltic Rim)
  RA8876_FONT_ENCODING_8859_14  = 0x1C,  // ISO 8859-14 (Latin 8: Celtic)
  RA8876_FONT_ENCODING_8859_15  = 0x1D,  // ISO 8859-15 (Latin 9: Western European)
  RA8876_FONT_ENCODING_8859_16  = 0x1E   // ISO 8859-16 (Latin 10: South-Eastern European)
};

enum ExternalFontRom
{
  RA8876_FONT_ROM_GT21L16T1W = 0,
  RA8876_FONT_ROM_GT30L16U2W = 1,
  RA8876_FONT_ROM_GT30L24T3Y = 2,
  RA8876_FONT_ROM_GT30L24M1Z = 3,
  RA8876_FONT_ROM_GT30L32S4W = 4,
  RA8876_FONT_ROM_GT20L24F6Y = 5,
  RA8876_FONT_ROM_GT21L24S1W = 6
};

struct ExternalFontRomInfo
{
  bool present;
  int  spiInterface;          // SPI interface that font ROM is connected to (0 or 1)
  int  spiClockDivisor;       // SPI interface clock divisor (2..512 in steps of 2)
  enum ExternalFontRom chip;  // Chip type
};

enum ExternalFontFamily
{
  RA8876_FONT_FAMILY_FIXED = 0,
  RA8876_FONT_FAMILY_ARIAL = 1,
  RA8876_FONT_FAMILY_TIMES = 2,
  RA8876_FONT_FAMILY_FIXED_BOLD = 3
};

typedef uint8_t FontFlags;
#define RA8876_FONT_FLAG_XLAT_FULLWIDTH 0x01  // Translate ASCII to Unicode fullwidth forms

// 1MHz. TODO: Figure out actual speed to use
// Data sheet section 5.2 says maximum SPI clock is 50MHz.
//#define RA8876_SPI_SPEED 10000000
#define RA8876_SPI_SPEED 25000000

// With SPI, the RA8876 expects an initial byte where the top two bits are meaningful. Bit 7
// is A0, bit 6 is WR#. See data sheet section 7.3.2 and section 19.
// A0: 0 for command/status, 1 for data
// WR#: 0 for write, 1 for read
#define RA8876_DATA_WRITE  0x80
#define RA8876_DATA_READ   0xC0
#define RA8876_CMD_WRITE   0x00
#define RA8876_STATUS_READ 0x40

// Data sheet 19.2: Chip configuration registers
#define RA8876_REG_SRR     0x00  // Software Reset Register
#define RA8876_REG_CCR     0x01  // Chip Configuration Register
#define RA8876_REG_MACR    0x02  // Memory Access Control Register
#define RA8876_REG_ICR     0x03  // Input Control Register
#define RA8876_REG_MRWDP   0x04  // Memory Read/Write Data Port

// Data sheet 19.3: PLL setting registers
#define RA8876_REG_PPLLC1  0x05  // SCLK PLL control register 1
#define RA8876_REG_PPLLC2  0x06  // SCLK PLL control register 2
#define RA8876_REG_MPLLC1  0x07  // MCLK PLL control register 1
#define RA8876_REG_MPLLC2  0x08  // MCLK PLL control register 2
#define RA8876_REG_SPLLC1  0x09  // CCLK PLL control register 1
#define RA8876_REG_SPLLC2  0x0A  // CCLK PLL control register 2

// Data sheet 19.5: LCD display control registers
#define RA8876_REG_MPWCTR  0x10  // Main/PIP Window Control Register
#define RA8876_REG_PIPCDEP 0x11  // PIP Window Color Depth register
#define RA8876_REG_DPCR    0x12  // Display configuration register
#define RA8876_REG_PCSR    0x13  // Panel scan clock and data setting register
#define RA8876_REG_HDWR    0x14  // Horizontal Display Width Register
#define RA8876_REG_HDWFTR  0x15  // Horizontal Display Width Fine Tuning Register
#define RA8876_REG_HNDR    0x16  // Horizontal Non-Display Period Register
#define RA8876_REG_HNDFTR  0x17  // Horizontal Non-Display Period Fine Tuning Register
#define RA8876_REG_HSTR    0x18  // HSYNC start position register
#define RA8876_REG_HPWR    0x19  // HSYNC Pulse Width Register
#define RA8876_REG_VDHR0   0x1A  // Vertical Display Height Register 0
#define RA8876_REG_VDHR1   0x1B  // Vertical Display Height Register 1
#define RA8876_REG_VNDR0   0x1C  // Vertical Non-Display Period Register 0
#define RA8876_REG_VNDR1   0x1D  // Vertical Non-Display Period Register 1
#define RA8876_REG_VSTR    0x1E  // VSYNC start position register
#define RA8876_REG_VPWR    0x1F  // VSYNC pulse width register
#define RA8876_REG_MISA0   0x20  // Main Image Start Address 0
#define RA8876_REG_MISA1   0x21  // Main Image Start Address 1
#define RA8876_REG_MISA2   0x22  // Main Image Start Address 2
#define RA8876_REG_MISA3   0x23  // Main Image Start Address 3
#define RA8876_REG_MIW0    0x24  // Main Image Width 0
#define RA8876_REG_MIW1    0x25  // Main Image Width 1
#define RA8876_REG_MWULX0  0x26  // Main Window Upper-Left X coordinate 0
#define RA8876_REG_MWULX1  0x27  // Main Window Upper-Left X coordinate 1
#define RA8876_REG_MWULY0  0x28  // Main Window Upper-Left Y coordinate 0
#define RA8876_REG_MWULY1  0x29  // Main Window Upper-Left Y coordinate 1

// Data sheet 19.6: Geometric engine control registers
#define RA8876_REG_CVSSA0     0x50  // Canvas Start Address 0
#define RA8876_REG_CVSSA1     0x51  // Canvas Start Address 1
#define RA8876_REG_CVSSA2     0x52  // Canvas Start Address 2
#define RA8876_REG_CVSSA3     0x53  // Canvas Start Address 3
#define RA8876_REG_CVS_IMWTH0 0x54  // Canvas image width 0
#define RA8876_REG_CVS_IMWTH1 0x55  // Canvas image width 1
#define RA8876_REG_AWUL_X0    0x56  // Active Window Upper-Left X coordinate 0
#define RA8876_REG_AWUL_X1    0x57  // Active Window Upper-Left X coordinate 1
#define RA8876_REG_AWUL_Y0    0x58  // Active Window Upper-Left Y coordinate 0
#define RA8876_REG_AWUL_Y1    0x59  // Active Window Upper-Left Y coordinate 1
#define RA8876_REG_AW_WTH0    0x5A  // Active Window Width 0
#define RA8876_REG_AW_WTH1    0x5B  // Active Window Width 1
#define RA8876_REG_AW_HT0     0x5C  // Active Window Height 0
#define RA8876_REG_AW_HT1     0x5D  // Active Window Height 1
#define RA8876_REG_AW_COLOR   0x5E  // Color Depth of canvas & active window
#define RA8876_REG_CURH0      0x5F  // Graphic read/write horizontal position 0
#define RA8876_REG_CURH1      0x60  // Graphic read/write horizontal position 1
#define RA8876_REG_CURV0      0x61  // Graphic read/write vertical position 0
#define RA8876_REG_CURV1      0x62  // Graphic read/write vertical position 1
#define RA8876_REG_F_CURX0    0x63  // Text cursor X-coordinate register 0
#define RA8876_REG_F_CURX1    0x64  // Text cursor X-coordinate register 1
#define RA8876_REG_F_CURY0    0x65  // Text cursor Y-coordinate register 0
#define RA8876_REG_F_CURY1    0x66  // Text cursor Y-coordinate register 1

#define RA8876_REG_DCR0       0x67  // Draw shape control register 0

#define RA8876_REG_DLHSR0     0x68  // Draw shape point 1 X coordinate register 0
#define RA8876_REG_DLHSR1     0x69  // Draw shape point 1 X coordinate register 1
#define RA8876_REG_DLVSR0     0x6A  // Draw shape point 1 Y coordinate register 0
#define RA8876_REG_DLVSR1     0x6B  // Draw shape point 1 Y coordinate register 1

#define RA8876_REG_DLHER0     0x6C  // Draw shape point 2 X coordinate register 0
#define RA8876_REG_DLHER1     0x6D  // Draw shape point 2 X coordinate register 1
#define RA8876_REG_DLVER0     0x6E  // Draw shape point 2 Y coordinate register 0
#define RA8876_REG_DLVER1     0x6F  // Draw shape point 2 Y coordinate register 1

#define RA8876_REG_DTPH0      0x70  // Draw shape point 3 X coordinate register 0
#define RA8876_REG_DTPH1      0x71  // Draw shape point 3 X coordinate register 1
#define RA8876_REG_DTPV0      0x72  // Draw shape point 3 Y coordinate register 0
#define RA8876_REG_DTPV1      0x73  // Draw shape point 3 Y coordinate register 1

#define RA8876_REG_DCR1       0x76  // Draw shape control register 1

#define RA8876_REG_ELL_A0     0x77  // Draw ellipse major radius 0
#define RA8876_REG_ELL_A1     0x78  // Draw ellipse major radius 1
#define RA8876_REG_ELL_B0     0x79  // Draw ellipse minor radius 0
#define RA8876_REG_ELL_B1     0x7A  // Draw ellipse minor radius 1

#define RA8876_REG_DEHR0      0x7B  // Draw ellipse centre X coordinate register 0
#define RA8876_REG_DEHR1      0x7C  // Draw ellipse centre X coordinate register 1
#define RA8876_REG_DEVR0      0x7D  // Draw ellipse centre Y coordinate register 0
#define RA8876_REG_DEVR1      0x7E  // Draw ellipse centre Y coordinate register 1

// Data sheet 19.7: PWM timer control registers
#define RA8876_REG_PSCLR      0x84  // PWM prescaler register
#define RA8876_REG_PMUXR      0x85  // PWM clock mux register
#define RA8876_REG_PCFGR      0x86  // PWM configuration register

#define RA8876_REG_TCMPB0L      0x88
#define RA8876_REG_TCMPB0H      0x89
#define RA8876_REG_TCNTB0L      0x8A
#define RA8876_REG_TCNTB0H      0x8B
#define RA8876_REG_TCMPB1L      0x8C
#define RA8876_REG_TCMPB1H      0x8D
#define RA8876_REG_TCNTB1L      0x8E
#define RA8876_REG_TCNTB1H      0x8F

#define RA8876_REG_BTE_CTRL0     0x90
#define RA8876_REG_BTE_CTRL1     0x91
#define RA8876_REG_BTE_COLR      0x92

#define RA8876_REG_S1_STR0   0x9D
#define RA8876_REG_S1_STR1   0x9E
#define RA8876_REG_S1_STR2   0x9F
#define RA8876_REG_S1_STR3   0xA0

#define RA8876_REG_S1_WTH0   0xA1
#define RA8876_REG_S1_WTH1   0xA2

#define RA8876_REG_S1_X0   0xA3
#define RA8876_REG_S1_X1   0xA4
#define RA8876_REG_S1_Y0   0xA5
#define RA8876_REG_S1_Y1   0xA6

#define RA8876_REG_DT_STR0      0xA7
#define RA8876_REG_DT_STR1      0xA8
#define RA8876_REG_DT_STR2      0xA9
#define RA8876_REG_DT_STR3      0xAA

#define RA8876_REG_DT_WTH0      0xAB
#define RA8876_REG_DT_WTH1      0xAC

#define RA8876_REG_DT_X0      0xAD
#define RA8876_REG_DT_X1     0xAE
#define RA8876_REG_DT_Y0     0xAF
#define RA8876_REG_DT_Y1      0xB0

#define RA8876_REG_BTE_WTH0      0xB1
#define RA8876_REG_BTE_WTH1      0xB2
#define RA8876_REG_BTE_HIG0      0xB3
#define RA8876_REG_BTE_HIG1      0xB4


// Data sheet 19.9: Serial flash & SPI master control registers
#define RA8876_REG_SFL_CTRL   0xB7  // Serial flash/ROM control register
#define RA8876_REG_SPI_DIVSOR 0xBB  // SPI clock period

// Data sheet 19.10: Text engine
#define RA8876_REG_CCR0       0xCC  // Character Control Register 0
#define RA8876_REG_CCR1       0xCD  // Character Control Register 1
#define RA8876_REG_GTFNT_SEL  0xCE  // Genitop character ROM select
#define RA8876_REG_GTFNT_CR   0xCF  // Genitop character ROM control register

#define RA8876_REG_FLDR       0xD0  // Chracter line gap register
#define RA8876_REG_F2FSSR     0xD1  // Chracter to character space setting register
#define RA8876_REG_FGCR       0xD2  // Foreground colour register - red
#define RA8876_REG_FGCG       0xD3  // Foreground colour register - green
#define RA8876_REG_FGCB       0xD4  // Foreground colour register - blue

#define RA8876_REG_BGCR       0xD5  // background colour register - red
#define RA8876_REG_BGCG       0xD6  // background colour register - green
#define RA8876_REG_BGCB       0xD7  // background colour register - blue

// Data sheet 19.12: SDRAM control registers
#define RA8876_REG_SDRAR         0xE0  // SDRAM attribute register
#define RA8876_REG_SDRMD         0xE1  // SDRAM mode & extended mode register
#define RA8876_REG_SDR_REF_ITVL0 0xE2  // SDRAM auto refresh interval 0
#define RA8876_REG_SDR_REF_ITVL1 0xE3  // SDRAM auto refresh interval 1
#define RA8876_REG_SDRCR         0xE4  // SDRAM control register


// Color definitions
#define RA8876_BLACK       0x0000      /*   0,   0,   0 */
#define RA8876_NAVY        0x000F      /*   0,   0, 128 */
#define RA8876_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define RA8876_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define RA8876_MAROON      0x7800      /* 128,   0,   0 */
#define RA8876_PURPLE      0x780F      /* 128,   0, 128 */
#define RA8876_OLIVE       0x7BE0      /* 128, 128,   0 */
#define RA8876_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define RA8876_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define RA8876_BLUE        0x001F      /*   0,   0, 255 */
#define RA8876_GREEN       0x07E0      /*   0, 255,   0 */
#define RA8876_CYAN        0x07FF      /*   0, 255, 255 */
#define RA8876_RED         0xF800      /* 255,   0,   0 */
#define RA8876_MAGENTA     0xF81F      /* 255,   0, 255 */
#define RA8876_YELLOW      0xFFE0      /* 255, 255,   0 */
#define RA8876_WHITE       0xFFFF      /* 255, 255, 255 */
#define RA8876_ORANGE      0xFD20      /* 255, 165,   0 */
#define RA8876_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define RA8876_PINK        0xF81F


#define RA8876_PSCLR   0x84
#define RA8876_PRESCALER  0x03
#define RA8876_PMUXR  0x85
#define RA8876_PWM_TIMER_DIV1   0
#define RA8876_PWM_TIMER_DIV2   1
#define RA8876_PWM_TIMER_DIV4   2
#define RA8876_PWM_TIMER_DIV8   3
#define RA8876_XPWM1_OUTPUT_ERROR_FLAG   0
#define RA8876_XPWM1_OUTPUT_PWM_TIMER1   2
#define RA8876_XPWM1_OUTPUT_OSC_CLK   3
#define RA8876_XPWM0_GPIO_C7   0
#define RA8876_XPWM0_OUTPUT_PWM_TIMER0   2
#define RA8876_XPWM0_OUTPUT_CORE_CLK   3

#define RA8876_PCFGR   0x86
#define RA8876_PWM_TIMER1_INVERTER_OFF  0
#define RA8876_PWM_TIMER1_INVERTER_ON   1
#define RA8876_PWM_TIMER1_ONE_SHOT   0
#define RA8876_PWM_TIMER1_AUTO_RELOAD   1
#define RA8876_PWM_TIMER1_STOP  0
#define RA8876_PWM_TIMER1_START  1
#define RA8876_PWM_TIMER0_DEAD_ZONE_DISABLE   0
#define RA8876_PWM_TIMER0_DEAD_ZONE_ENABLE   1
#define RA8876_PWM_TIMER0_INVERTER_OFF   0
#define RA8876_PWM_TIMER0_INVERTER_ON   1
#define RA8876_PWM_TIMER0_ONE_SHOT   0
#define RA8876_PWM_TIMER0_AUTO_RELOAD   1
#define RA8876_PWM_TIMER0_STOP  0
#define RA8876_PWM_TIMER0_START  1

#define RA8876_BTE_CTRL0   0x90
#define RA8876_BTE_ENABLE   1
#define RA8876_PATTERN_FORMAT8X8   0
#define RA8876_PATTERN_FORMAT16X16   1
#define RA8876_BTE_CTRL1  0x91
#define RA8876_DESTINATION_COLOR_DEPTH_16BPP  1
#define RA8876_S0_COLOR_DEPTH_16BPP  1
#define RA8876_S1_COLOR_DEPTH_16BPP  1

#define RA8876_BTE_ROP_CODE_0     0   //0 ( Blackness )
#define RA8876_BTE_ROP_CODE_1     1   //~S0・~S1 or ~ ( S0+S1 )
#define RA8876_BTE_ROP_CODE_2     2   //~S0・S1
#define RA8876_BTE_ROP_CODE_3     3   //~S0
#define RA8876_BTE_ROP_CODE_4     4   //S0・~S1
#define RA8876_BTE_ROP_CODE_5     5   //~S1
#define RA8876_BTE_ROP_CODE_6     6   //S0^S1
#define RA8876_BTE_ROP_CODE_7     7   //~S0+~S1 or ~ ( S0・S1 )
#define RA8876_BTE_ROP_CODE_8     8   //S0・S1
#define RA8876_BTE_ROP_CODE_9     9   //~ ( S0^S1 )
#define RA8876_BTE_ROP_CODE_10    10  //S1
#define RA8876_BTE_ROP_CODE_11    11  //~S0+S1
#define RA8876_BTE_ROP_CODE_12    12  //S0
#define RA8876_BTE_ROP_CODE_13    13  //S0+~S1
#define RA8876_BTE_ROP_CODE_14    14  //S0+S1
#define RA8876_BTE_ROP_CODE_15    15  //1 ( Whiteness )
#define RA8876_BTE_ROP_BUS_WIDTH8  7
#define RA8876_BTE_ROP_BUS_WIDTH16  15

#define RA8876_BTE_MPU_WRITE_WITH_ROP    0
#define RA8876_BTE_MEMORY_COPY_WITH_ROP   2
#define RA8876_BTE_MPU_WRITE_WITH_CHROMA   4
#define RA8876_BTE_MEMORY_COPY_WITH_CHROMA  5
#define RA8876_BTE_PATTERN_FILL_WITH_ROP  6
#define RA8876_BTE_PATTERN_FILL_WITH_CHROMA  7
#define RA8876_BTE_MPU_WRITE_COLOR_EXPANSION   8
#define RA8876_BTE_MPU_WRITE_COLOR_EXPANSION_WITH_CHROMA   9
#define RA8876_BTE_MEMORY_COPY_WITH_OPACITY  10
#define RA8876_BTE_MPU_WRITE_WITH_OPACITY  11
#define RA8876_BTE_SOLID_FILL  12
#define RA8876_BTE_MEMORY_COPY_WITH_COLOR_EXPANSION  14
#define RA8876_BTE_MEMORY_COPY_WITH_COLOR_EXPANSION_CHROMA  15

#define RA8876_BTE_COLR  0x92

class RA8876 : public Renderer {

 public:

  RA8876(int8_t cs,int8_t mosi,int8_t miso,int8_t sclk,int8_t bp);

  bool begin(void);

  // Dimensions
  int getWidth() { return m_width; };
  int getHeight() { return m_height; };

  void DisplayInit(int8_t p,int8_t size,int8_t rot,int8_t font);

  // Test
  void colorBarTest(bool enabled);

  // Drawing
  void drawPixel(int16_t x, int16_t y, uint16_t color);
  void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
  void drawRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
  void fillRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
  void fillScreen(uint16_t color);
  void drawTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color);
  void fillTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color);

  void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);


  void drawCircle(int16_t x, int16_t y, int16_t radius, uint16_t color);
  void fillCircle(int16_t x, int16_t y, int16_t radius, uint16_t color);
  void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
  void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);

  void clearScreen(uint16_t color);

  // Text cursor
  void setCursor(int16_t x, int16_t y);
  int getCursorX(void);
  int getCursorY(void);

  void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
  void pushColors(uint16_t *data, uint16_t len, boolean first);

  // Text
  void selectInternalFont(enum FontSize size, enum FontEncoding enc = RA8876_FONT_ENCODING_8859_1);
  void selectExternalFont(enum ExternalFontFamily family, enum FontSize size, enum FontEncoding enc, FontFlags flags = 0);
  int getTextSizeY(void);
  //void setTextColor(uint16_t color);
  //void setTextColor(uint16_t c, uint16_t bg);
  void setTextScale(int scale);
  void setTextSize(uint8_t s);
  void setTextScale(int xScale, int yScale);
  void putChar(char c) { putChars(&c, 1); };
  void putChars(const char *buffer, size_t size);
  void putChar16(uint16_t c) { putChars16(&c, 1); };
  void putChars16(const uint16_t *buffer, unsigned int count);

  // Internal for Print class
  size_t xwrite(uint8_t c);
  size_t xwrite(const uint8_t *buffer, size_t size);

  uint16_t GetColorFromIndex(uint8_t index);
  void DisplayOnff(int8_t on);
  void setRotation(uint8_t m);
  void setDrawMode(uint8_t mode);
  void setDrawMode_reg(uint8_t mode);
  void dim(uint8_t contrast);
  void FastString(uint16_t x,uint16_t y,uint16_t tcolor, const char* str);

 private:
  uint8_t  tabcolor;
  void PWM_init(void);
  void wait_ready(void);
  void softReset(void);
  void writeCmd(uint8_t x);
  void writeData(uint8_t x);
  uint8_t readData(void);
  uint8_t readStatus(void);
  void writeReg(uint8_t reg, uint8_t x);
  void writeReg16(uint8_t reg, uint16_t x);
  uint8_t readReg(uint8_t reg);
  uint16_t readReg16(uint8_t reg);
  void waitWriteFifo(void);
  void waitTaskBusy(void);

 bool calcPllParams(uint32_t targetFreq, int kMax, PllParams *pll);
 bool calcClocks(void);
 void dumpClocks(void);

 bool initPLL(void);
 bool initMemory(SdramInfo *info);
 bool initDisplay(void);

 // Font utils
 uint8_t internalFontEncoding(enum FontEncoding enc);

 // Text/graphics mode
 void setTextMode(void);
 void setGraphicsMode(void);

 // Low-level shapes
 void drawTwoPointShape(int x1, int y1, int x2, int y2, uint16_t color, uint8_t reg, uint8_t cmd);  // drawLine, drawRect, fillRect
 void drawThreePointShape(int x1, int y1, int x2, int y2, int x3, int y3, uint16_t color, uint8_t reg, uint8_t cmd);  // drawTriangle, fillTriangle
 void drawEllipseShape(int x, int y, int xrad, int yrad, uint16_t color, uint8_t cmd);  // drawCircle, fillCircle
 void drawThreePointShape1(int x1, int y1, int x2, int y2, int x3, int y3, uint16_t color, uint8_t reg, uint8_t cmd);

  int8_t  m_csPin, _mosi, _miso, _sclk, dimmer, _hwspi;
  uint16_t m_width;
  uint16_t m_height;
  uint16_t m_depth;
  uint32_t m_oscClock;   // OSC clock (external crystal) frequency in kHz

  PllParams m_memPll;   // MCLK (memory) PLL parameters
  PllParams m_corePll;  // CCLK (core) PLL parameters
  PllParams m_scanPll;  // SCLK (LCD panel scan) PLL parameters

  SPISettings m_spiSettings;

  SdramInfo *m_sdramInfo;

  DisplayInfo *m_displayInfo;

  ExternalFontRomInfo m_fontRomInfo;

  uint16_t addrw_x1;
  uint16_t addrw_x2;

  //uint16_t m_textColor;
  int      m_textScaleX;
  int      m_textScaleY;

  enum FontSource m_fontSource;
  enum FontSize   m_fontSize;
  FontFlags       m_fontFlags;

#ifdef ESP32
  // dma section
  bool DMA_Enabled = false;
  uint8_t  spiBusyCheck = 0;
  spi_transaction_t trans;
  spi_device_handle_t dmaHAL;
  spi_host_device_t spi_host = VSPI_HOST;
  // spi_host_device_t spi_host = VSPI_HOST;
  bool initDMA();
  void deInitDMA(void);
  bool dmaBusy(void);
  void dmaWait(void);
  void pushPixelsDMA(uint16_t* image, uint32_t len);
#endif // ESP32
};

#endif
