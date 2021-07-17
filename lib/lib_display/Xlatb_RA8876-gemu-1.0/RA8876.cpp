/*MIT License

Copyright (c) 2017 xlatb

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <SPI.h>
#include "RA8876.h"
#include <limits.h>

uint8_t initdone;
/* TODO

font 0 x and y size with line,col cmd
support for rotation
fast picture write

*/

//void SHOW_STAGE(uint8_t stage) {
//  Serial.printf(">%d,\n", stage);
//

const uint16_t RA8876_colors[]={RA8876_BLACK,RA8876_WHITE,RA8876_RED,RA8876_GREEN,RA8876_BLUE,RA8876_CYAN,RA8876_MAGENTA,\
  RA8876_YELLOW,RA8876_NAVY,RA8876_DARKGREEN,RA8876_DARKCYAN,RA8876_MAROON,RA8876_PURPLE,RA8876_OLIVE,\
RA8876_LIGHTGREY,RA8876_DARKGREY,RA8876_ORANGE,RA8876_GREENYELLOW,RA8876_PINK};

uint16_t RA8876::GetColorFromIndex(uint8_t index) {
  if (index>=sizeof(RA8876_colors)/2) index=0;
  return RA8876_colors[index];
}


// Constructor when using software SPI.  All output pins are configurable.
RA8876::RA8876(int8_t cs,int8_t mosi,int8_t miso,int8_t sclk,int8_t bp) : Renderer(RA8876_TFTWIDTH, RA8876_TFTHEIGHT) {
  m_csPin = cs;
  _mosi  = mosi;
  _miso  = miso;
  _sclk = sclk;
    /*
  _bp = bp;
  if (_bp<99) {
    pinMode(_bp, OUTPUT);
    digitalWrite(_bp,HIGH);
  }*/
  _hwspi = 1;
}


//#define RA8876_CS_LOW digitalWrite(m_csPin, LOW)
//#define RA8876_CS_HIGH digitalWrite(m_csPin, HIGH)

#ifndef ESP32
#define RA8876_CS_LOW GPOC=(1<<m_csPin);
#define RA8876_CS_HIGH GPOS=(1<<m_csPin);
#else
#define RA8876_CS_LOW digitalWrite(m_csPin,0);
#define RA8876_CS_HIGH digitalWrite(m_csPin,1);
#endif

/*
extern void ICACHE_RAM_ATTR RA8876_digitalWrite(uint8_t pin, uint8_t val) {
  //stopWaveform(pin);
  if(pin < 16){
    if(val) GPOS = (1 << pin);
    else GPOC = (1 << pin);
  } else if(pin == 16){
    if(val) GP16O |= 1;
    else GP16O &= ~1;
  }
}
*/

void RA8876::writeCmd(uint8_t x) {
  RA8876_CS_LOW
  SPI.transfer(RA8876_CMD_WRITE);
  SPI.transfer(x);
  RA8876_CS_HIGH
}

void RA8876::writeData(uint8_t x) {
  RA8876_CS_LOW
  SPI.transfer(RA8876_DATA_WRITE);
  SPI.transfer(x);
  RA8876_CS_HIGH
}

uint8_t RA8876::readData(void) {
  RA8876_CS_LOW
  SPI.transfer(RA8876_DATA_READ);
  uint8_t x = SPI.transfer(0);
  RA8876_CS_HIGH
  return x;
}

// Reads the special status register.
// This register uses a special cycle type instead of having an address like other registers.
// See data sheet section 19.1.
uint8_t RA8876::readStatus(void) {
  RA8876_CS_LOW
  SPI.transfer(RA8876_STATUS_READ);
  uint8_t x = SPI.transfer(0);
  RA8876_CS_HIGH
  return x;
}

void RA8876::writeReg(uint8_t reg, uint8_t x) {
  writeCmd(reg);
  writeData(x);
  if (!initdone) {
  //  Serial.printf("%02x, %02x\n", reg, x);
  }
}

// Like writeReg(), but does two successive register writes of a 16-bit value, low byte first.
void RA8876::writeReg16(uint8_t reg, uint16_t x) {
  writeCmd(reg);
  writeData(x & 0xFF);
  writeCmd(reg + 1);
  writeData(x >> 8);
}

uint8_t RA8876::readReg(uint8_t reg) {
  writeCmd(reg);
  return readData();
}

// Like readReg(), but does two successive register reads of a 16-bit value, low byte first.
uint16_t RA8876::readReg16(uint8_t reg) {
  uint16_t v;

  writeCmd(reg);
  v = readData();
  writeCmd(reg + 1);
  v |= readData() << 8;

  return v;
}

// Trigger a soft reset. Note that the data sheet section 19.2 says that this only resets the
//  "internal state machine", not any configuration registers.
void RA8876::softReset(void) {
  SPI.beginTransaction(m_spiSettings);

  // Trigger soft reset
  writeReg(RA8876_REG_SRR, 0x01);
  delay(5);

  // Wait for status register to show "normal operation".
  uint8_t status;
  for (int i = 0; i < 250; i++) {
    delay(1);
    if (((status = readStatus()) & 0x02) == 0)
      break;
  }
  SPI.endTransaction();
  return;
}

void RA8876::DisplayOnff(int8_t on) {
  uint8_t dpcr;
  SPI.beginTransaction(m_spiSettings);
  dpcr = readReg(RA8876_REG_DPCR);
  if (on) {
    dpcr |= 0x40;  // Display on
  } else {
    dpcr &= 0x40^0xff;  // Display off
    // backlight off
    writeReg(RA8876_REG_TCMPB0L,0);
    writeReg(RA8876_REG_TCMPB0H,0);
  }
  writeReg(RA8876_REG_DPCR, dpcr);
  SPI.endTransaction();
  if (on) {
    dim(dimmer);
  }
}

// 0-15
void RA8876::dim(uint8_t contrast) {
  SPI.beginTransaction(m_spiSettings);
  dimmer=contrast;
  // pwm0 duty
  uint32_t duty=(contrast*1024)/15;
  writeReg(RA8876_REG_TCMPB0L,duty);
  writeReg(RA8876_REG_TCMPB0H,duty>>8);
  SPI.endTransaction();
}

// pwm needed for backlight dimmer
void RA8876::PWM_init(void) {
  SPI.beginTransaction(m_spiSettings);
  writeReg(RA8876_REG_PSCLR,3);

  uint8_t val=RA8876_PWM_TIMER_DIV4<<6|RA8876_PWM_TIMER_DIV4<<4|RA8876_XPWM1_OUTPUT_PWM_TIMER1<<2|RA8876_XPWM0_OUTPUT_PWM_TIMER0;
  writeReg(RA8876_REG_PMUXR,val);

  val=RA8876_PWM_TIMER1_INVERTER_ON<<6|RA8876_PWM_TIMER1_AUTO_RELOAD<<5|RA8876_PWM_TIMER1_START<<4|RA8876_PWM_TIMER0_DEAD_ZONE_DISABLE<<3|
                  RA8876_PWM_TIMER0_INVERTER_OFF<<2|RA8876_PWM_TIMER0_AUTO_RELOAD<<1|RA8876_PWM_TIMER0_START;
  writeReg(RA8876_REG_PCFGR,val);

  uint16_t duty=0x00ff;
  // pwm0 duty for backlight
  writeReg(RA8876_REG_TCMPB0L,duty);
  writeReg(RA8876_REG_TCMPB0H,duty>>8);

  // pwm1 duty not needed
  duty=0x00ff;
  writeReg(RA8876_REG_TCMPB1L,duty);
  writeReg(RA8876_REG_TCMPB1H,duty>>8);

  // clocksperiod
  uint16_t clocks_per_period=1024;

  writeReg(RA8876_REG_TCNTB0L,clocks_per_period);
  writeReg(RA8876_REG_TCNTB0H,clocks_per_period>>8);

  writeReg(RA8876_REG_TCNTB1L,clocks_per_period);
  writeReg(RA8876_REG_TCNTB1H,clocks_per_period>>8);
  SPI.endTransaction();
}

// Given a target frequency in kHz, finds PLL parameters k and n to reach as
//  close as possible to the target frequency without exceeding it.
// The k parameter will be constrained to the range 1..kMax.
// Returns true iff PLL params were found, even if not an exact match.
bool RA8876::calcPllParams(uint32_t targetFreq, int kMax, PllParams *pll) {
  bool found = false;
  int foundk, foundn;
  uint32_t foundFreq;
  uint32_t foundError;  // Amount lower than requested frequency

  // k of 0 (i.e. 2 ** 0 = 1) is possible, but not sure if it's a good idea.
  for (int testk = 1; testk <= kMax; testk++)
  {
    if (m_oscClock % (1 << testk))
      continue;  // Step size with this k would be fractional

    int testn = (targetFreq / (m_oscClock / (1 << testk))) - 1;
    if ((testn < 1) || (testn > 63))
      continue;  // Param n out of range for this k

    // Fvco constraint found in data sheet section 6.1.2
    uint32_t fvco = m_oscClock * (testn + 1);
    if ((fvco < 100000) && (fvco > 600000))
      continue;  // Fvco out of range

    // Found some usable params, but perhaps at a lower frequency than requested.
    uint32_t freq = (m_oscClock * (testn + 1)) / (1 << testk);
    uint32_t error = targetFreq - freq;
    if ((!found) || (found && (foundError > error)))
    {
      found = true;
      foundk = testk;
      foundn = testn;
      foundFreq = freq;
      foundError = error;

      // No need to keep searching if the frequency match was exact
      if (foundError == 0)
        break;
    }
  }

  if (found)
  {
    pll->freq = foundFreq;
    pll->k    = foundk;
    pll->n    = foundn;
  }

  return found;
}

// Calculates the clock frequencies and their PLL parameters.
bool RA8876::calcClocks(void) {
  // Data sheet section 5.2 gives max clocks:
  //  memClock : 166 MHz
  //  coreClock: 120 MHz (133MHz if not using internal font)
  //  scanClock: 100 MHz

  // Mem clock target is the same as SDRAM speed, but capped at 166 MHz
  uint32_t memClock = m_sdramInfo->speed * 1000;
  if (memClock > 166000)
    memClock = 166000;

  if (!calcPllParams(memClock, 3, &m_memPll))
    return false;

  // Core clock target will be the same as the mem clock, but capped to
  //  120 MHz, because that is the max frequency if we want to use the
  //  internal font.
  uint32_t coreClock = m_memPll.freq;
  if (coreClock > 120000)
    coreClock = 120000;

  if (!calcPllParams(coreClock, 3, &m_corePll))
    return false;

  // Scan clock target will be the display's dot clock, but capped at 100 MHz
  uint32_t scanClock = m_displayInfo->dotClock;
  if (scanClock > 100000)
    scanClock = 100000;

  if (!calcPllParams(scanClock, 7, &m_scanPll))
    return false;

  //dumpClocks();

  // Data sheet section 6.1.1 rules:
  // 1. Core clock must be less than or equal to mem clock
  if (m_corePll.freq > m_memPll.freq)
    return false;

  // 2. Core clock must be greater than half mem clock
  if ((m_corePll.freq * 2) <= m_memPll.freq)
    return false;

  // 3. Core clock must be greater than (scan clock * 1.5)
  if (m_corePll.freq <= (m_scanPll.freq + (m_scanPll.freq >> 1)))
    return false;

  return true;
}

// Dump clock info to serial monitor.
/*
void RA8876::dumpClocks(void)
{
  Serial.println("\nMem\n---");
  Serial.print("Requested kHz: "); Serial.println(m_sdramInfo->speed * 1000);
  Serial.print("Actual kHz   : "); Serial.println(m_memPll.freq);
  Serial.print("PLL k        : "); Serial.println(m_memPll.k);
  Serial.print("PLL n        : "); Serial.println(m_memPll.n);

  Serial.println("\nCore\n----");
  Serial.print("kHz          : "); Serial.println(m_corePll.freq);
  Serial.print("PLL k        : "); Serial.println(m_corePll.k);
  Serial.print("PLL n        : "); Serial.println(m_corePll.n);

  Serial.println("\nScan\n----");
  Serial.print("Requested kHz: "); Serial.println(m_displayInfo->dotClock);
  Serial.print("Actual kHz   : "); Serial.println(m_scanPll.freq);
  Serial.print("PLL k        : "); Serial.println(m_scanPll.k);
  Serial.print("PLL n        : "); Serial.println(m_scanPll.n);

  // TODO: Frame rate?

  return;
}
*/

bool RA8876::initPLL(void) {
  //Serial.println("init PLL");

  SPI.beginTransaction(m_spiSettings);

  //Serial.print("DRAM_FREQ "); Serial.println(m_memPll.freq);
  //Serial.print("7: "); Serial.println(m_memPll.k << 1);
  //Serial.print("8: "); Serial.println(m_memPll.n);
  writeReg(RA8876_REG_MPLLC1, m_memPll.k << 1);
  writeReg(RA8876_REG_MPLLC2, m_memPll.n);

  //Serial.print("CORE_FREQ "); Serial.println(m_corePll.freq);
  //Serial.print("9: "); Serial.println(m_corePll.k << 1);
  //Serial.print("A: "); Serial.println(m_corePll.n);
  writeReg(RA8876_REG_SPLLC1, m_corePll.k << 1);
  writeReg(RA8876_REG_SPLLC2, m_corePll.n);

  // Per the data sheet, there are two divider fields for the scan clock, but the math seems
  //  to work out if we treat k as a single 3-bit number in bits 3..1.
  //Serial.print("SCAN_FREQ "); Serial.println(m_scanPll.freq);
  //Serial.print("5: "); Serial.println(m_scanPll.k << 1);
  //Serial.print("6: "); Serial.println(m_scanPll.n);
  writeReg(RA8876_REG_PPLLC1, m_scanPll.k << 1);
  writeReg(RA8876_REG_PPLLC2, m_scanPll.n);

  // Toggle bit 7 of the CCR register to trigger a reconfiguration of the PLLs
  writeReg(RA8876_REG_CCR, 0x00);
  delay(2);
  writeReg(RA8876_REG_CCR, 0x80);
  delay(2);

  uint8_t ccr = readReg(RA8876_REG_CCR);

  SPI.endTransaction();

  return (ccr & 0x80) ? true : false;
}

// Initialize SDRAM interface.
bool RA8876::initMemory(SdramInfo *info) {
  //Serial.println("init memory");

  uint32_t sdramRefreshRate;
  uint8_t sdrar = 0x00;
  uint8_t sdrmd = 0x00;

  // Refresh rate
  sdramRefreshRate = ((uint32_t) info->refresh * info->speed * 1000) >> info->rowBits;

  // Number of banks
  if (info->banks == 2)
    ;  // NOP
  else if (info->banks == 4)
    sdrar |= 0x20;
  else
    return false;  // Unsupported number of banks

  // Number of row bits
  if ((info->rowBits < 11) || (info->rowBits > 13))
    return false;  // Unsupported number of row bits
  else
    sdrar |= ((info->rowBits - 11) & 0x03) << 3;

  // Number of column bits
  if ((info->colBits < 8) || (info->colBits > 12))
    return false;  // Unsupported number of column bits
  else
    sdrar |= info->colBits & 0x03;

  // CAS latency
  if ((info->casLatency < 2) || (info->casLatency > 3))
    return false;  // Unsupported CAS latency
  else
    sdrmd |= info->casLatency & 0x03;

  SPI.beginTransaction(m_spiSettings);

  //Serial.print("SDRAR: "); Serial.println(sdrar);  // Expected: 0x29 (41 decimal)
  writeReg(RA8876_REG_SDRAR, sdrar);

  //Serial.print("SDRMD: "); Serial.println(sdrmd);
  writeReg(RA8876_REG_SDRMD, sdrmd);

  //Serial.print("sdramRefreshRate: "); Serial.println(sdramRefreshRate);
  writeReg(RA8876_REG_SDR_REF_ITVL0, sdramRefreshRate & 0xFF);
  writeReg(RA8876_REG_SDR_REF_ITVL1, sdramRefreshRate >> 8);

  // Trigger SDRAM initialization
  writeReg(RA8876_REG_SDRCR, 0x01);

  // Wait for SDRAM to be ready
  uint8_t status;
  for (int i = 0; i < 250; i++) {
    delay(1);
    if ((status = readStatus()) & 0x40)
      break;
  }
  SPI.endTransaction();

  //Serial.println(status);

  return (status & 0x40) ? true : false;
}

void RA8876::DisplayInit(int8_t p,int8_t size,int8_t rot,int8_t font) {
  setRotation(rot);
  setTextWrap(false);         // Allow text to run off edges
  cp437(true);
  setTextFont(font&3);
  setTextSize(size&7);
  setTextColor(RA8876_WHITE,RA8876_BLACK);
  setCursor(0,0);
  fillScreen(RA8876_BLACK);
  setDrawMode(drawmode);
  PWM_init();
}

bool RA8876::initDisplay() {

  lvgl_param.fluslines = 10;

  SPI.beginTransaction(m_spiSettings);

  // Set chip config register
  uint8_t ccr = readReg(RA8876_REG_CCR);
  ccr &= 0xE7;  // 24-bit LCD output
  ccr &= 0xFE;  // 8-bit host data bus
  writeReg(RA8876_REG_CCR, ccr);

  writeReg(RA8876_REG_MACR, 0x00);  // Direct write, left-to-right-top-to-bottom memory

  writeReg(RA8876_REG_ICR, 0x00);  // Graphics mode, memory is SDRAM

  uint8_t dpcr = readReg(RA8876_REG_DPCR);
  dpcr &= 0xFB;  // Vertical scan top to bottom
  dpcr &= 0xF8;  // Colour order RGB
  dpcr |= 0x80;  // Panel fetches PDAT at PCLK falling edge
  writeReg(RA8876_REG_DPCR, dpcr);

  uint8_t pcsr = readReg(RA8876_REG_PCSR);
  pcsr |= 0x80;  // XHSYNC polarity high
  pcsr |= 0x40;  // XVSYNC polarity high
  pcsr &= 0xDF;  // XDE polarity high
  writeReg(RA8876_REG_PCSR, pcsr);

  // Set display width
  writeReg(RA8876_REG_HDWR, (m_displayInfo->width / 8) - 1);
  writeReg(RA8876_REG_HDWFTR, (m_displayInfo->width % 8));

  // Set display height
  writeReg(RA8876_REG_VDHR0, (m_displayInfo->height - 1) & 0xFF);
  writeReg(RA8876_REG_VDHR1, (m_displayInfo->height - 1) >> 8);

  // Set horizontal non-display (back porch)
  writeReg(RA8876_REG_HNDR, (m_displayInfo->hBackPorch / 8) - 1);
  writeReg(RA8876_REG_HNDFTR, (m_displayInfo->hBackPorch % 8));

  // Set horizontal start position (front porch)
  writeReg(RA8876_REG_HSTR, ((m_displayInfo->hFrontPorch + 4) / 8) - 1);

  // Set HSYNC pulse width
  writeReg(RA8876_REG_HPWR, ((m_displayInfo->hPulseWidth + 4) / 8) - 1);

  // Set vertical non-display (back porch)
  writeReg(RA8876_REG_VNDR0, (m_displayInfo->vBackPorch - 1) & 0xFF);
  writeReg(RA8876_REG_VNDR1, (m_displayInfo->vBackPorch - 1) >> 8);

  // Set vertical start position (front porch)
  writeReg(RA8876_REG_VSTR, m_displayInfo->vFrontPorch - 1);

  // Set VSYNC pulse width
  writeReg(RA8876_REG_VPWR, m_displayInfo->vPulseWidth - 1);

  // Set main window to 16 bits per pixel
  writeReg(RA8876_REG_MPWCTR, 0x04);  // PIP windows disabled, 16-bpp, enable sync signals

  // Set main window start address to 0
  writeReg(RA8876_REG_MISA0, 0);
  writeReg(RA8876_REG_MISA1, 0);
  writeReg(RA8876_REG_MISA2, 0);
  writeReg(RA8876_REG_MISA3, 0);

  // Set main window image width
  writeReg(RA8876_REG_MIW0, m_width & 0xFF);
  writeReg(RA8876_REG_MIW1, m_width >> 8);

  // Set main window start coordinates
  writeReg(RA8876_REG_MWULX0, 0);
  writeReg(RA8876_REG_MWULX1, 0);
  writeReg(RA8876_REG_MWULY0, 0);
  writeReg(RA8876_REG_MWULY1, 0);

  // Set canvas start address
  writeReg(RA8876_REG_CVSSA0, 0);
  writeReg(RA8876_REG_CVSSA1, 0);
  writeReg(RA8876_REG_CVSSA2, 0);
  writeReg(RA8876_REG_CVSSA3, 0);

  // Set canvas width
  writeReg(RA8876_REG_CVS_IMWTH0, m_width & 0xFF);
  writeReg(RA8876_REG_CVS_IMWTH1, m_width >> 8);

  // Set active window start coordinates
  writeReg(RA8876_REG_AWUL_X0, 0);
  writeReg(RA8876_REG_AWUL_X1, 0);
  writeReg(RA8876_REG_AWUL_Y0, 0);
  writeReg(RA8876_REG_AWUL_Y1, 0);

  // Set active window dimensions
  writeReg(RA8876_REG_AW_WTH0, m_width & 0xFF);
  writeReg(RA8876_REG_AW_WTH1, m_width >> 8);
  writeReg(RA8876_REG_AW_HT0, m_height & 0xFF);
  writeReg(RA8876_REG_AW_HT1, m_height >> 8);

  // Set canvas addressing mode/colour depth
  uint8_t aw_color = 0x00;  // 2d addressing mode
  if (m_depth == 16)
    aw_color |= 0x01;
  else if (m_depth == 24)
    aw_color |= 0x02;
  writeReg(RA8876_REG_AW_COLOR, aw_color);

  // Turn on display
  dpcr = readReg(RA8876_REG_DPCR);
  dpcr |= 0x40;  // Display on
  writeReg(RA8876_REG_DPCR, dpcr);

  // TODO: Track backlight pin and turn on backlight

  SPI.endTransaction();
initdone = 1;
  return true;
}

void RA8876::setRotation(uint8_t m) {
return;
/*
  SPI.beginTransaction(m_spiSettings);

  rotation = m % 4; // can't be higher than 3
  switch (rotation) {
   case 0:
     writeReg(RA8876_REG_MACR, 0x00);
     _width  = RA8876_TFTWIDTH;
     _height = RA8876_TFTHEIGHT;
     break;
   case 1:
     writeReg(RA8876_REG_MACR, 0x02);
     _width  = RA8876_TFTHEIGHT;
     _height = RA8876_TFTWIDTH;
     break;
  case 2:
    writeReg(RA8876_REG_MACR, 0x01);
     _width  = RA8876_TFTWIDTH;
     _height = RA8876_TFTHEIGHT;
    break;
   case 3:
     writeReg(RA8876_REG_MACR, 0x03);
     _width  = RA8876_TFTHEIGHT;
     _height = RA8876_TFTWIDTH;
     break;
  }
  m_width = _width;
  m_height = _height;
  SPI.endTransaction();*/
}

SdramInfo defaultSdramInfo =
{
  120, // 120 MHz
  3,   // CAS latency 3
  4,   // 4 banks
  12,  // 12-bit row addresses
  9,   // 9-bit column addresses
  64   // 64 millisecond refresh time
};

DisplayInfo defaultDisplayInfo =
{
  1024,   // Display width
  600,    // Display height
  50000,  // Pixel clock in kHz

  160,    // Horizontal front porch
  160,    // Horizontal back porch
  70,     // HSYNC pulse width

  12,     // Vertical front porch
  23,     // Vertical back porch
  10      // VSYNC pulse width
};

bool RA8876::begin(void) {
  m_sdramInfo = &defaultSdramInfo;
  m_displayInfo = &defaultDisplayInfo;
  m_width  = m_displayInfo->width;
  m_height = m_displayInfo->height;
  m_depth  = 16;
  m_oscClock = 10000;  // 10000kHz or 10MHz
  textcolor = 0xFFFF; // White
  textbgcolor = 0; // black
  m_fontRomInfo.present = false;  // No external font ROM chip

  // Set up chip select pin
  pinMode(m_csPin, OUTPUT);
  digitalWrite(m_csPin, HIGH);

  if (!calcClocks()) {
    //Serial.println("calcClocks failed");
    return false;
  }

#ifndef ESP32
  SPI.begin();
#else
  SPI.begin(_sclk,_miso,_mosi , -1);
#endif

  m_spiSettings = SPISettings(RA8876_SPI_SPEED, MSBFIRST, SPI_MODE3);

#ifdef RA8876_DEBUG
  Serial.printf("RA8876 init\n");
#endif

  softReset();

  if (!initPLL()) {
    //Serial.println("initPLL failed");
    return false;
  }

#ifdef RA8876_DEBUG
  Serial.printf("RA8876 init pll OK\n");
#endif

  if (!initMemory(m_sdramInfo)) {
    //Serial.println("initMemory failed");
    return false;
  }

#ifdef RA8876_DEBUG
  Serial.printf("RA8876 init sdram OK\n");
#endif

  if (!initDisplay()) {
    //Serial.println("initDisplay failed");
    return false;
  }

#ifdef RA8876_DEBUG
  Serial.printf("RA8876 init display OK\n");
#endif

  // Set default font
  selectInternalFont(RA8876_FONT_SIZE_16);
  setTextScale(1);

#ifdef RA8876_DEBUG
  Serial.printf("RA8876 set scale OK\n");
#endif

  setRotation(0);

  clearScreen(0);

#ifdef RA8876_DEBUG
  Serial.printf("RA8876 init complete\n");
#endif

  return true;
}

void RA8876::fillScreen(uint16_t color) {
  clearScreen(color);
}

// Show colour bars of 8 colours in repeating horizontal bars.
// This does not alter video memory, but rather instructs the video controller to display
//  the pattern rather than the contents of memory.
void RA8876::colorBarTest(bool enabled) {
  SPI.beginTransaction(m_spiSettings);

  uint8_t dpcr = readReg(RA8876_REG_DPCR);

  if (enabled)
    dpcr = dpcr | 0x20;
  else
    dpcr = dpcr & ~0x20;

  writeReg(RA8876_REG_DPCR, dpcr);

  SPI.endTransaction();
}


void RA8876::drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
  drawTwoPointShape(x1, y1, x2, y2, color, RA8876_REG_DCR0, 0x80);
};
void RA8876::drawRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
  drawTwoPointShape(x1, y1, x1+x2, y1+y2, color, RA8876_REG_DCR1, 0xA0);
};
void RA8876::fillRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
  drawTwoPointShape(x1, y1, x1+x2, y1+y2, color, RA8876_REG_DCR1, 0xE0);
};
void RA8876::drawTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color) {
  drawThreePointShape(x1, y1, x2, y2, x3, y3, color, RA8876_REG_DCR0, 0xA2);
};
void RA8876::fillTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color) {
  drawThreePointShape(x1, y1, x2, y2, x3, y3, color, RA8876_REG_DCR0, 0xE2);
};
void RA8876::drawCircle(int16_t x, int16_t y, int16_t radius, uint16_t color) {
  drawEllipseShape(x, y, radius, radius, color, 0x80);
};
void RA8876::fillCircle(int16_t x, int16_t y, int16_t radius, uint16_t color) {
  drawEllipseShape(x, y, radius, radius, color, 0xC0);
};

void RA8876::drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color) {
  drawThreePointShape1(x0, y0, x0+w, y0+h, radius, radius, color, RA8876_REG_DCR1, 0xB0);
}
void RA8876::fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color) {
  drawThreePointShape1(x0, y0, x0+w, y0+h, radius, radius, color, RA8876_REG_DCR1, 0xF0);
}

void RA8876::clearScreen(uint16_t color) {
    setCursor(0, 0); fillRect(0, 0, m_width, m_height, color);
};

void RA8876::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
  drawTwoPointShape(x, y, x, y+h, color, RA8876_REG_DCR1, 0xE0);
}
void RA8876::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
  drawTwoPointShape(x, y, x+w, y, color, RA8876_REG_DCR1, 0xE0);
}

void RA8876::setTextSize(uint8_t s) {
   setTextScale(s, s);
   textsize_x=s;
   textsize_y=s;
   //Serial.printf("scale %d\n",s);
}

// Picture16bppBteMpuWriteColorExpansionWithChromaKey
// PAGE1_START_ADDR,SCREEN_WIDTH, 50, 50+128+50+10, 128, 128,COLOR65K_WHITE,COLOR65K_BLACK,"sun.bin");
//   ra8876lite.bteMpuWriteWithROP(s1_addr, s1_image_width, s1_x, s1_y, des_addr, des_image_width, des_x, des_y, width, height, rop_code);


//dCardShowPicture16bppBteMpuWriteWithROP(PAGE1_START_ADDR, SCREEN_WIDTH, 50+128, 50, PAGE1_START_ADDR, SCREEN_WIDTH, 50+128, 50,
//                                           128, 128,RA8876_BTE_ROP_CODE_6,"appli.bin");

/*bte_Source1_MemoryStartAddr(s1_addr);
  bte_Source1_ImageWidth(s1_image_width);

  bte_Source1_WindowStartXY(s1_x,s1_y);
  bte_DestinationMemoryStartAddr(des_addr);

  bte_DestinationImageWidth(des_image_width);

  bte_DestinationWindowStartXY(des_x,des_y);
  bte_WindowSize(width,height);

  lcdRegDataWrite(RA8876_BTE_CTRL1,rop_code<<4|RA8876_BTE_MPU_WRITE_WITH_ROP);//91h
  lcdRegDataWrite(RA8876_BTE_COLR,RA8876_S0_COLOR_DEPTH_16BPP<<5|RA8876_S1_COLOR_DEPTH_16BPP<<2|RA8876_DESTINATION_COLOR_DEPTH_16BPP);//92h
  lcdRegDataWrite(RA8876_BTE_CTRL0,RA8876_BTE_ENABLE<<4);//90h
  ramAccessPrepare();

// put picturess
  activeWindowXY(x,y);
activeWindowWH(width,height);
setPixelCursor(x,y);

  */
void RA8876::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
  uint16_t xs=x1-x0;
  uint16_t ys=y1-y0;
  cursor_x=x0;
  cursor_y=y0;
  addrw_x1=x0;
  addrw_x2=x1;
  uint8_t flag=0;

  if (!x0 && !y0 && !x1 && !y1) {
    x0=0;
    y0=0;
    x1=_width;
    y1=_height;
    flag=1;
  }

  if (x1>_width) x1=_width;
  if (y1>_height) y1=_height;

  SPI.beginTransaction(m_spiSettings);

#if 1
//  activeWindowXY(x,y);
  writeReg(RA8876_REG_AWUL_X0,x0);//56h
  writeReg(RA8876_REG_AWUL_X1,x0>>8);//57h
  writeReg(RA8876_REG_AWUL_Y0,y0);//58h
  writeReg(RA8876_REG_AWUL_Y1,y0>>8);//59h

  //activeWindowWH(width,height);
  writeReg(RA8876_REG_AW_WTH0,xs);//5ah
  writeReg(RA8876_REG_AW_WTH1,xs>>8);//5bh
  writeReg(RA8876_REG_AW_HT0,ys);//5ch
  writeReg(RA8876_REG_AW_HT1,ys>>8);//5dh

  //setPixelCursor(x,y);
  writeReg(RA8876_REG_CURH0,x0); //5fh
  writeReg(RA8876_REG_CURH1,x0>>8);//60h
  writeReg(RA8876_REG_CURV0,y0);//61h
  writeReg(RA8876_REG_CURV1,y0>>8);//62h

  //ramAccessPrepare();

  #else
  // source address, page1=0
  // bte_Source1_MemoryStartAddr
  uint32_t addr=0;
  writeReg(RA8876_REG_S1_STR0,addr);//9dh
  writeReg(RA8876_REG_S1_STR1,addr>>8);//9eh
  writeReg(RA8876_REG_S1_STR2,addr>>16);//9fh
  writeReg(RA8876_REG_S1_STR3,addr>>24);//a0h
  // screen width
  // bte_Source1_ImageWidth
  uint16_t width=_width;
  writeReg(RA8876_REG_S1_WTH0,width);//abh
  writeReg(RA8876_REG_S1_WTH1,width>>8);//ach

  // source window
  // bte_Source1_WindowStartXY
  writeReg(RA8876_REG_S1_X0,x0);//adh
  writeReg(RA8876_REG_S1_X1,x0>>8);//aeh
  writeReg(RA8876_REG_S1_Y0,y0);//afh
  writeReg(RA8876_REG_S1_Y1,y0>>8);//b0h

  // dest mem
  // bte_DestinationMemoryStartAddr
  writeReg(RA8876_REG_DT_STR0,addr);//a7h
  writeReg(RA8876_REG_DT_STR1,addr>>8);//a8h
  writeReg(RA8876_REG_DT_STR2,addr>>16);//a9h
  writeReg(RA8876_REG_DT_STR3,addr>>24);//aah

  // bte_DestinationImageWidth
  writeReg(RA8876_REG_DT_WTH0,width);//abh
  writeReg(RA8876_REG_DT_WTH1,width>>8);//ach

  //bte_DestinationWindowStartXY(des_x,des_y);
  writeReg(RA8876_REG_DT_X0,x0);//adh
  writeReg(RA8876_REG_DT_X1,x0>>8);//aeh
  writeReg(RA8876_REG_DT_Y0,y0);//afh
  writeReg(RA8876_REG_DT_Y1,y0>>8);//b0h

  // bte_WindowSize
  writeReg(RA8876_REG_BTE_WTH0,xs);//b1h
  writeReg(RA8876_REG_BTE_WTH1,xs>>8);//b2h
  writeReg(RA8876_REG_BTE_HIG0,ys);//b3h
  writeReg(RA8876_REG_BTE_HIG1,ys>>8);//b4h


  writeReg(RA8876_BTE_CTRL1,RA8876_BTE_ROP_CODE_12<<4|RA8876_BTE_MPU_WRITE_WITH_ROP);//91h
  writeReg(RA8876_BTE_COLR,RA8876_S0_COLOR_DEPTH_16BPP<<5|RA8876_S1_COLOR_DEPTH_16BPP<<2|RA8876_DESTINATION_COLOR_DEPTH_16BPP);//92h
  writeReg(RA8876_BTE_CTRL0,RA8876_BTE_ENABLE<<4);//90h

  #endif

  writeCmd(RA8876_REG_MRWDP); //04h();

  //if (flag) SPI.endTransaction();
  SPI.endTransaction();
}

// pixel color is swapped in contrast to other controllers
void RA8876::pushColors(uint16_t *data, uint16_t len, boolean not_swapped) {

  if (not_swapped == false) {
    // coming from LVGL
#ifdef ESP32
    SPI.beginTransaction(m_spiSettings);
    RA8876_CS_LOW
    SPI.write(RA8876_DATA_WRITE);
    if (lvgl_param.use_dma) {
      // will need swapping !
      pushPixelsDMA(data, len);
    } else {
      SPI.writePixels(data, len * 2);
    }
    RA8876_CS_HIGH
    SPI.endTransaction();
#endif
  } else {
    // coming from displaytext
    SPI.beginTransaction(m_spiSettings);
    RA8876_CS_LOW
    SPI.transfer(RA8876_DATA_WRITE);

#ifdef ESP32
    SPI.writeBytes((uint8_t*)data, len * 2);
#endif

#ifdef ESP8266
    while (len--) {
      uint16_t color = *data++;
      SPI.write(color&0xff);
      SPI.write(color>>8);
    }
#endif
    RA8876_CS_HIGH
    SPI.endTransaction();
  }
}

void RA8876::drawPixel(int16_t x, int16_t y, uint16_t color) {
  //Serial.println("drawPixel");
  //Serial.println(readStatus());

  SPI.beginTransaction(m_spiSettings);

  writeReg(RA8876_REG_CURH0, x & 0xFF);
  writeReg(RA8876_REG_CURH1, x >> 8);

  writeReg(RA8876_REG_CURV0, y & 0xFF);
  writeReg(RA8876_REG_CURV1, y >> 8);

  writeReg(RA8876_REG_MRWDP, color & 0xFF);
  writeReg(RA8876_REG_MRWDP, color >> 8);

  SPI.endTransaction();
}

void RA8876::drawTwoPointShape(int x1, int y1, int x2, int y2, uint16_t color, uint8_t reg, uint8_t cmd) {
  //Serial.println("drawTwoPointShape");

  SPI.beginTransaction(m_spiSettings);

  // First point
  writeReg(RA8876_REG_DLHSR0, x1 & 0xFF);
  writeReg(RA8876_REG_DLHSR1, x1 >> 8);
  writeReg(RA8876_REG_DLVSR0, y1 & 0xFF);
  writeReg(RA8876_REG_DLVSR1, y1 >> 8);

  // Second point
  writeReg(RA8876_REG_DLHER0, x2 & 0xFF);
  writeReg(RA8876_REG_DLHER1, x2 >> 8);
  writeReg(RA8876_REG_DLVER0, y2 & 0xFF);
  writeReg(RA8876_REG_DLVER1, y2 >> 8);

  // Colour
  writeReg(RA8876_REG_FGCR, color >> 11 << 3);
  writeReg(RA8876_REG_FGCG, ((color >> 5) & 0x3F) << 2);
  writeReg(RA8876_REG_FGCB, (color & 0x1F) << 3);

  // Draw
  writeReg(reg, cmd);  // Start drawing

  // Wait for completion
  wait_ready();

  SPI.endTransaction();
}


// 1 us  => 10 ms
#define RA8876_TIMEOUT 10000

void RA8876::waitWriteFifo(void) {
  uint8_t status = readStatus();
  uint32_t iter = 0;
  while (status & 0x80) {
    status = readStatus();
    iter++;
    if (iter>RA8876_TIMEOUT) {
      // timeout, soft reset
      softReset();
      SPI.beginTransaction(m_spiSettings);
#ifdef RA8876_DEBUG
      Serial.printf("iter timeout fifo\n");
#endif
      return;
    }
  }
};

void RA8876::waitTaskBusy(void) {
  //while (readStatus() & 0x08);
  wait_ready();
};

void RA8876::wait_ready(void) {
  uint8_t status = readStatus();
  uint32_t iter = 0;
  while (status & 0x08) {
    status = readStatus();
    iter++;
    if (iter>RA8876_TIMEOUT) {
      // timeout, soft reset
      softReset();
      SPI.beginTransaction(m_spiSettings);
#ifdef RA8876_DEBUG
      Serial.printf("iter timeout cmd\n");
#endif
      return;
    }
  }
  // got at max 1800
  // Serial.printf("iter: %d\n",iter);
}

void RA8876::drawThreePointShape(int x1, int y1, int x2, int y2, int x3, int y3, uint16_t color, uint8_t reg, uint8_t cmd) {
  //Serial.println("drawThreePointShape");

  SPI.beginTransaction(m_spiSettings);

  // First point
  writeReg(RA8876_REG_DLHSR0, x1 & 0xFF);
  writeReg(RA8876_REG_DLHSR1, x1 >> 8);
  writeReg(RA8876_REG_DLVSR0, y1 & 0xFF);
  writeReg(RA8876_REG_DLVSR1, y1 >> 8);

  // Second point
  writeReg(RA8876_REG_DLHER0, x2 & 0xFF);
  writeReg(RA8876_REG_DLHER1, x2 >> 8);
  writeReg(RA8876_REG_DLVER0, y2 & 0xFF);
  writeReg(RA8876_REG_DLVER1, y2 >> 8);

  // Third point
  writeReg(RA8876_REG_DTPH0, x3 & 0xFF);
  writeReg(RA8876_REG_DTPH1, x3 >> 8);
  writeReg(RA8876_REG_DTPV0, y3 & 0xFF);
  writeReg(RA8876_REG_DTPV1, y3 >> 8);

  // Colour
  writeReg(RA8876_REG_FGCR, color >> 11 << 3);
  writeReg(RA8876_REG_FGCG, ((color >> 5) & 0x3F) << 2);
  writeReg(RA8876_REG_FGCB, (color & 0x1F) << 3);

  // Draw
  writeReg(reg, cmd);  // Start drawing

  // Wait for completion
  wait_ready();

  SPI.endTransaction();
}

void RA8876::drawThreePointShape1(int x1, int y1, int x2, int y2, int x3, int y3, uint16_t color, uint8_t reg, uint8_t cmd) {
  //Serial.println("drawThreePointShape");

  SPI.beginTransaction(m_spiSettings);

  // First point
  writeReg(RA8876_REG_DLHSR0, x1 & 0xFF);
  writeReg(RA8876_REG_DLHSR1, x1 >> 8);
  writeReg(RA8876_REG_DLVSR0, y1 & 0xFF);
  writeReg(RA8876_REG_DLVSR1, y1 >> 8);

  // Second point
  writeReg(RA8876_REG_DLHER0, x2 & 0xFF);
  writeReg(RA8876_REG_DLHER1, x2 >> 8);
  writeReg(RA8876_REG_DLVER0, y2 & 0xFF);
  writeReg(RA8876_REG_DLVER1, y2 >> 8);

  // corner radius
  writeReg(RA8876_REG_ELL_A0, x3 & 0xFF);
  writeReg(RA8876_REG_ELL_A1, x3 >> 8);
  writeReg(RA8876_REG_ELL_B0, y3 & 0xFF);
  writeReg(RA8876_REG_ELL_B1, y3 >> 8);

  #define RA8876_REG_ELL_A0     0x77  // Draw ellipse major radius 0
  #define RA8876_REG_ELL_A1     0x78  // Draw ellipse major radius 1
  #define RA8876_REG_ELL_B0     0x79  // Draw ellipse minor radius 0
  #define RA8876_REG_ELL_B1     0x7A  // Draw ellipse minor radius 1

  // Colour
  writeReg(RA8876_REG_FGCR, color >> 11 << 3);
  writeReg(RA8876_REG_FGCG, ((color >> 5) & 0x3F) << 2);
  writeReg(RA8876_REG_FGCB, (color & 0x1F) << 3);

  // Draw
  writeReg(reg, cmd);  // Start drawing

  // Wait for completion
  wait_ready();

  SPI.endTransaction();
}

void RA8876::drawEllipseShape(int x, int y, int xrad, int yrad, uint16_t color, uint8_t cmd) {
  //Serial.println("drawEllipseShape");

  SPI.beginTransaction(m_spiSettings);

  // First point
  writeReg16(RA8876_REG_DEHR0, x);
  writeReg16(RA8876_REG_DEVR0, y);

  // Radii
  writeReg16(RA8876_REG_ELL_A0, xrad);
  writeReg16(RA8876_REG_ELL_B0, yrad);

  // Colour
  writeReg(RA8876_REG_FGCR, color >> 11 << 3);
  writeReg(RA8876_REG_FGCG, ((color >> 5) & 0x3F) << 2);
  writeReg(RA8876_REG_FGCB, (color & 0x1F) << 3);

  // Draw
  writeReg(RA8876_REG_DCR1, cmd);  // Start drawing

  // Wait for completion
  wait_ready();

  SPI.endTransaction();
}

void RA8876::setCursor(int16_t x, int16_t y) {
  SPI.beginTransaction(m_spiSettings);

  writeReg16(RA8876_REG_F_CURX0, x);
  writeReg16(RA8876_REG_F_CURY0, y);

  SPI.endTransaction();
  //Serial.printf("curs %d:%d\n",x,y);

  cursor_x = x;
  cursor_y = y;
}

int RA8876::getCursorX(void) {
  SPI.beginTransaction(m_spiSettings);

  int x = readReg16(RA8876_REG_F_CURX0);

  SPI.endTransaction();

  return x;
}

int RA8876::getCursorY(void) {
  SPI.beginTransaction(m_spiSettings);

  int y = readReg16(RA8876_REG_F_CURY0);

  SPI.endTransaction();

  return y;
}

// Given a font encoding value, returns the corresponding bit pattern for
//  use by internal fonts.
uint8_t RA8876::internalFontEncoding(enum FontEncoding enc) {
  uint8_t e;
  switch (enc)
  {
  case RA8876_FONT_ENCODING_8859_2:
    e = 0x01;
    break;
  case RA8876_FONT_ENCODING_8859_4:
    e = 0x02;
    break;
  case RA8876_FONT_ENCODING_8859_5:
    e = 0x03;
    break;
  default:
    e = 0x00;  // ISO-8859-1
    break;
  }

  return e;
}

void RA8876::setTextMode(void) {
  // Restore text colour
  textcolor=textcolor;
  writeReg(RA8876_REG_FGCR, textcolor >> 11 << 3);
  writeReg(RA8876_REG_FGCG, ((textcolor >> 5) & 0x3F) << 2);
  writeReg(RA8876_REG_FGCB, (textcolor & 0x1F) << 3);

  writeReg(RA8876_REG_BGCR, textbgcolor >> 11 << 3);
  writeReg(RA8876_REG_BGCG, ((textbgcolor >> 5) & 0x3F) << 2);
  writeReg(RA8876_REG_BGCB, (textbgcolor & 0x1F) << 3);

  waitTaskBusy();

  // Enable text mode
  uint8_t icr = readReg(RA8876_REG_ICR);
  writeReg(RA8876_REG_ICR, icr | 0x04);

  SPI.endTransaction();
  if (textcolor==textbgcolor) {
    setDrawMode_reg(1);
  } else {
    setDrawMode_reg(0);
  }
  SPI.beginTransaction(m_spiSettings);

}

void RA8876::setGraphicsMode(void) {
  waitTaskBusy();

  // Disable text mode
  uint8_t icr = readReg(RA8876_REG_ICR);
  writeReg(RA8876_REG_ICR, icr & ~0x04);
}

void RA8876::selectInternalFont(enum FontSize size, enum FontEncoding enc) {
  m_fontSource = RA8876_FONT_SOURCE_INTERNAL;
  m_fontSize   = size;
  m_fontFlags  = 0;

  SPI.beginTransaction(m_spiSettings);

  writeReg(RA8876_REG_CCR0, 0x00 | ((size & 0x03) << 4) | internalFontEncoding(enc));

  uint8_t ccr1 = readReg(RA8876_REG_CCR1);
  //ccr1 |= 0x40;  // Transparent background
  ccr1 &=0x40^0xff;
  writeReg(RA8876_REG_CCR1, ccr1);

  SPI.endTransaction();
}

void RA8876::setDrawMode(uint8_t mode) {
  drawmode=mode;
  setDrawMode_reg(mode);
}

void RA8876::setDrawMode_reg(uint8_t mode)  {
  SPI.beginTransaction(m_spiSettings);
  uint8_t ccr1 = readReg(RA8876_REG_CCR1);
  if (mode) {
    ccr1 |= 0x40;  // Transparent background
  } else {
    ccr1 &=0x40^0xff; // opaque background
  }
  writeReg(RA8876_REG_CCR1, ccr1);
  SPI.endTransaction();
}

void RA8876::selectExternalFont(enum ExternalFontFamily family, enum FontSize size, enum FontEncoding enc, FontFlags flags) {
  m_fontSource = RA8876_FONT_SOURCE_EXT_ROM;
  m_fontSize   = size;
  m_fontFlags  = flags;

  SPI.beginTransaction(m_spiSettings);

  //Serial.print("CCR0: "); Serial.println(0x40 | ((size & 0x03) << 4), HEX);
  writeReg(RA8876_REG_CCR0, 0x40 | ((size & 0x03) << 4));  // Select external font ROM and size

  uint8_t ccr1 = readReg(RA8876_REG_CCR1);
  ccr1 |= 0x40;  // Transparent background
  //Serial.print("CCR1: "); Serial.println(ccr1, HEX);
  writeReg(RA8876_REG_CCR1, ccr1);

  //Serial.print("GTFNT_CR: "); Serial.println((enc << 3) | (family & 0x03), HEX);
  writeReg(RA8876_REG_GTFNT_CR, (enc << 3) | (family & 0x03));  // Character encoding and family

  SPI.endTransaction();
}

/*
void RA8876::setTextColor(uint16_t color) {
   textcolor = color;
   textbgcolor = color;
 };

 void RA8876::setTextColor(uint16_t c, uint16_t bg) {
   textcolor = c;
   textbgcolor=bg;
 }
 */

void RA8876::setTextScale(int scale) {
   setTextScale(scale, scale);
 };

int RA8876::getTextSizeY(void) {
  return ((m_fontSize + 2) * 8) * m_textScaleY;
}

void RA8876::setTextScale(int xScale, int yScale) {
  xScale = constrain(xScale, 1, 4);
  yScale = constrain(yScale, 1, 4);

  m_textScaleX = xScale;
  m_textScaleY = yScale;

  SPI.beginTransaction(m_spiSettings);

  uint8_t ccr1 = readReg(RA8876_REG_CCR1);
  ccr1 = (ccr1 & 0xF0) | ((xScale - 1) << 2) | (yScale - 1);
  //Serial.println(ccr1, HEX);
  writeReg(RA8876_REG_CCR1, ccr1);

  SPI.endTransaction();
}

// Similar to write(), but does no special handling of control characters.
void RA8876::putChars(const char *buffer, size_t size) {
  SPI.beginTransaction(m_spiSettings);

  setTextMode();

  // Write characters
  writeCmd(RA8876_REG_MRWDP);
  for (unsigned int i = 0; i < size; i++)
  {
    waitWriteFifo();
    writeData(buffer[i]);
  }

  setGraphicsMode();

  SPI.endTransaction();
}

void RA8876::putChars16(const uint16_t *buffer, unsigned int count) {
  SPI.beginTransaction(m_spiSettings);

  setTextMode();

  // Write characters
  writeCmd(RA8876_REG_MRWDP);
  for (unsigned int i = 0; i < count; i++)
  {
    waitWriteFifo();
    writeData(buffer[i] >> 8);

    waitWriteFifo();
    writeData(buffer[i] & 0xFF);
  }

  setGraphicsMode();

  SPI.endTransaction();
}


extern uint8_t wr_redir;

size_t RA8876::xwrite(uint8_t c) {
  return xwrite(&c, 1);
};

//#define RA8876_DEBUG

size_t RA8876::xwrite(const uint8_t *buffer, size_t size) {

#ifdef RA8876_DEBUG
  char buff[128];
  memcpy(buff,buffer,size);
  buff[size]=0;
  Serial.printf("write start: %s\n",buff);
#endif
  SPI.beginTransaction(m_spiSettings);

  setTextMode();

  writeCmd(RA8876_REG_MRWDP);  // Set current register for writing to memory
  for (unsigned int i = 0; i < size; i++)
  {
    char c = buffer[i];

    if (!c) continue;

    if (c == '\r')
      ;  // Ignored
    else if (c == '\n')
    {
      SPI.endTransaction();
      setCursor(0, getCursorY() + getTextSizeY());
      SPI.beginTransaction(m_spiSettings);
      writeCmd(RA8876_REG_MRWDP);  // Reset current register for writing to memory
    }
    else if ((m_fontFlags & RA8876_FONT_FLAG_XLAT_FULLWIDTH) && ((c >= 0x21) || (c <= 0x7F)))
    {
      // Translate ASCII to Unicode fullwidth form (for Chinese fonts that lack ASCII)
      uint16_t fwc = c - 0x21 + 0xFF01;

      waitWriteFifo();
      writeData(fwc >> 8);

      waitWriteFifo();
      writeData(fwc & 0xFF);
    }
    else
    {
      waitWriteFifo();
      writeData(c);
    }
  }

  setGraphicsMode();

  SPI.endTransaction();

  #ifdef RA8876_DEBUG
    Serial.printf("write end:\n");
  #endif

  return size;
}

void RA8876::FastString(uint16_t x,uint16_t y,uint16_t tcolor, const char* str) {
  setCursor(x,y);
  setTextColor(tcolor,textbgcolor);
  xwrite((uint8_t*)str,strlen(str));
}

// ESP 32 DMA section , derived from TFT_eSPI
#ifdef ESP32

/***************************************************************************************
** Function name:           initDMA
** Description:             Initialise the DMA engine - returns true if init OK
***************************************************************************************/
bool RA8876::initDMA()
{
  if (DMA_Enabled) return false;

  esp_err_t ret;
  spi_bus_config_t buscfg = {
    .mosi_io_num = _mosi,
    .miso_io_num = _miso,
    .sclk_io_num = _sclk,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = width() * height() * 2 + 8, // TFT screen size
    .flags = 0,
    .intr_flags = 0
  };


  spi_device_interface_config_t devcfg = {
    .command_bits = 0,
    .address_bits = 0,
    .dummy_bits = 0,
    .mode = SPI_MODE3,
    .duty_cycle_pos = 0,
    .cs_ena_pretrans = 0,
    .cs_ena_posttrans = 0,
    .clock_speed_hz = RA8876_SPI_SPEED,
    .input_delay_ns = 0,
    .spics_io_num = -1,
    .flags = SPI_DEVICE_NO_DUMMY, //0,
    .queue_size = 1,
    .pre_cb = 0, //dc_callback, //Callback to handle D/C line
    .post_cb = 0
  };
  ret = spi_bus_initialize(spi_host, &buscfg, 1);
  ESP_ERROR_CHECK(ret);
  ret = spi_bus_add_device(spi_host, &devcfg, &dmaHAL);
  ESP_ERROR_CHECK(ret);

  DMA_Enabled = true;
  spiBusyCheck = 0;
  return true;
}

/***************************************************************************************
** Function name:           deInitDMA
** Description:             Disconnect the DMA engine from SPI
***************************************************************************************/
void RA8876::deInitDMA(void) {
  if (!DMA_Enabled) return;
  spi_bus_remove_device(dmaHAL);
  spi_bus_free(spi_host);
  DMA_Enabled = false;
}

/***************************************************************************************
** Function name:           dmaBusy
** Description:             Check if DMA is busy
***************************************************************************************/
bool RA8876::dmaBusy(void) {
  if (!DMA_Enabled || !spiBusyCheck) return false;

  spi_transaction_t *rtrans;
  esp_err_t ret;
  uint8_t checks = spiBusyCheck;
  for (int i = 0; i < checks; ++i) {
    ret = spi_device_get_trans_result(dmaHAL, &rtrans, 0);
    if (ret == ESP_OK) spiBusyCheck--;
  }

  //Serial.print("spiBusyCheck=");Serial.println(spiBusyCheck);
  if (spiBusyCheck == 0) return false;
  return true;
}


/***************************************************************************************
** Function name:           dmaWait
** Description:             Wait until DMA is over (blocking!)
***************************************************************************************/
void RA8876::dmaWait(void) {
  if (!DMA_Enabled || !spiBusyCheck) return;
  spi_transaction_t *rtrans;
  esp_err_t ret;
  for (int i = 0; i < spiBusyCheck; ++i) {
    ret = spi_device_get_trans_result(dmaHAL, &rtrans, portMAX_DELAY);
    assert(ret == ESP_OK);
  }
  spiBusyCheck = 0;
}


/***************************************************************************************
** Function name:           pushPixelsDMA
** Description:             Push pixels to TFT (len must be less than 32767)
***************************************************************************************/
// This will byte swap the original image if setSwapBytes(true) was called by sketch.
void RA8876::pushPixelsDMA(uint16_t* image, uint32_t len) {

  if ((len == 0) || (!DMA_Enabled)) return;

  dmaWait();

  esp_err_t ret;

  memset(&trans, 0, sizeof(spi_transaction_t));

  trans.user = (void *)1;
  trans.tx_buffer = image;  //finally send the line data
  trans.length = len * 16;        //Data length, in bits
  trans.flags = 0;                //SPI_TRANS_USE_TXDATA flag

  ret = spi_device_queue_trans(dmaHAL, &trans, portMAX_DELAY);
  assert(ret == ESP_OK);

  spiBusyCheck++;
}
#endif // ESP32
