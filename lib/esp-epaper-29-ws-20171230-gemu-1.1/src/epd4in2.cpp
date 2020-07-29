/**
 *  @filename   :   epd4in2.cpp
 *  @brief      :   Implements for Dual-color e-paper library
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     August 10 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdlib.h>
#include <epd4in2.h>


//#define SSPI_USEANYPIN

extern uint8_t *buffer;
uint8_t epd42_mode;

Epd42::Epd42(int16_t width, int16_t height) :
Paint(width,height) {
}

void Epd42::DisplayOnff(int8_t on) {
}


#define DISPLAY_INIT_MODE 0
#define DISPLAY_INIT_PARTIAL 1
#define DISPLAY_INIT_FULL 2

void Epd42::Updateframe() {
  //SetFrameMemory(buffer, 0, 0, EPD_WIDTH,EPD_HEIGHT);
  SetPartialWindow(buffer, 0, 0, width,height,2);
  if (epd42_mode==DISPLAY_INIT_PARTIAL) {
    DisplayFrameQuick();
  } else {
    DisplayFrame();
  }
  //Serial.printf("update\n");
}

void Epd42::DisplayInit(int8_t p,int8_t size,int8_t rot,int8_t font) {
// ignore update mode
  if (p==DISPLAY_INIT_PARTIAL) {
    epd42_mode=p;
    //Init(lut_partial_update);
    //ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
    DisplayFrameQuick();
    delay(500);
    return;
    //Serial.printf("partial\n");
  } else if (p==DISPLAY_INIT_FULL) {
    epd42_mode=p;
    //Init(lut_full_update);
    //ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
    DisplayFrame();
    delay(4500);
    return;
    //Serial.printf("full\n");
  } else {
    epd42_mode=DISPLAY_INIT_FULL;
    Updateframe();
  }

  setRotation(rot);
  invertDisplay(false);
  setTextWrap(false);         // Allow text to run off edges
  cp437(true);
  setTextFont(font);
  setTextSize(size);
  setTextColor(WHITE,BLACK);
  setCursor(0,0);
  fillScreen(BLACK);
}

int16_t Epd42::Begin(int16_t cs,int16_t mosi,int16_t sclk) {
  cs_pin=cs;
  mosi_pin=mosi;
  sclk_pin=sclk;
}

void Epd42::Init(int8_t p) {
  epd42_mode=p;
  DisplayFrame();
  delay(4000);
}

int Epd42::Init(void) {
    pinMode(cs_pin, OUTPUT);
    pinMode(mosi_pin, OUTPUT);
    pinMode(sclk_pin, OUTPUT);

    digitalWrite(cs_pin,HIGH);
    digitalWrite(mosi_pin,LOW);
    digitalWrite(sclk_pin,LOW);

    width = EPD_WIDTH42;
    height = EPD_HEIGHT42;

    Reset();
    SendCommand(POWER_SETTING);
    SendData(0x03);                  // VDS_EN, VDG_EN
    SendData(0x00);                  // VCOM_HV, VGHL_LV[1], VGHL_LV[0]
    SendData(0x2b);                  // VDH
    SendData(0x2b);                  // VDL
    SendData(0xff);                  // VDHR
    SendCommand(BOOSTER_SOFT_START);
    SendData(0x17);
    SendData(0x17);
    SendData(0x17);                  //07 0f 17 1f 27 2F 37 2f
    SendCommand(POWER_ON);
    WaitUntilIdle();
    SendCommand(PANEL_SETTING);
   // SendData(0xbf);    // KW-BF   KWR-AF  BWROTP 0f
  //  SendData(0x0b);
//  SendData(0x0F);  //300x400 Red mode, LUT from OTP
//  SendData(0x1F);  //300x400 B/W mode, LUT from OTP
  SendData(0x3F); //300x400 B/W mode, LUT set by register
//  SendData(0x2F); //300x400 Red mode, LUT set by register

    SendCommand(PLL_CONTROL);
    SendData(0x3C);        // 3A 100Hz   29 150Hz   39 200Hz    31 171Hz       3C 50Hz (default)    0B 10Hz
  //SendData(0x0B);   //0B is 10Hz
    /* EPD hardware init end */
    return 0;
}

/**
 *  @brief: basic function for sending commands
 */
void Epd42::SendCommand(unsigned char command) {
    //DigitalWrite(dc_pin, LOW);
    //SpiTransfer(command);
    fastSPIwrite(command,0);
}

/**
 *  @brief: basic function for sending data
 */
void Epd42::SendData(unsigned char data) {
    //DigitalWrite(dc_pin, HIGH);
    //SpiTransfer(data);
    fastSPIwrite(data,1);
}

/**
 *  @brief: Wait until the busy_pin goes HIGH
 */
void Epd42::WaitUntilIdle(void) {
  //  while(DigitalRead(busy_pin) == 0) {      //0: busy, 1: idle
  //      DelayMs(1);
  //  }
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd::Sleep();
 */
void Epd42::Reset(void) {
    //DigitalWrite(reset_pin, LOW);
    //DelayMs(200);
    //DigitalWrite(reset_pin, HIGH);
    //DelayMs(200);
}
/**
 *  @brief: transmit partial data to the SRAM.  The final parameter chooses between dtm=1 and dtm=2
 */
void Epd42::SetPartialWindow(const unsigned char* buffer_black, int x, int y, int w, int l, int dtm) {
    SendCommand(PARTIAL_IN);
    SendCommand(PARTIAL_WINDOW);
    SendData(x >> 8);
    SendData(x & 0xf8);     // x should be the multiple of 8, the last 3 bit will always be ignored
    SendData(((x & 0xf8) + w  - 1) >> 8);
    SendData(((x & 0xf8) + w  - 1) | 0x07);
    SendData(y >> 8);
    SendData(y & 0xff);
    SendData((y + l - 1) >> 8);
    SendData((y + l - 1) & 0xff);
    SendData(0x01);         // Gates scan both inside and outside of the partial window. (default)
  //  DelayMs(2);
    SendCommand((dtm == 1) ? DATA_START_TRANSMISSION_1 : DATA_START_TRANSMISSION_2);
    if (buffer_black != NULL) {
        for(int i = 0; i < w  / 8 * l; i++) {
            SendData(buffer_black[i]^0xff);
        }
    } else {
        for(int i = 0; i < w  / 8 * l; i++) {
            SendData(0x00);
        }
    }
 //   DelayMs(2);
    SendCommand(PARTIAL_OUT);
}



/**
 *  @brief: set the look-up table
 */
void Epd42::SetLut(void) {
    unsigned int count;
    SendCommand(LUT_FOR_VCOM);                            //vcom
    for(count = 0; count < 44; count++) {
        SendData(pgm_read_byte(&lut_vcom0[count]));
    }

    SendCommand(LUT_WHITE_TO_WHITE);                      //ww --
    for(count = 0; count < 42; count++) {
        SendData(pgm_read_byte(&lut_ww[count]));
    }

    SendCommand(LUT_BLACK_TO_WHITE);                      //bw r
    for(count = 0; count < 42; count++) {
        SendData(pgm_read_byte(&lut_bw[count]));
    }

    SendCommand(LUT_WHITE_TO_BLACK);                      //wb w
    for(count = 0; count < 42; count++) {
        SendData(pgm_read_byte(&lut_wb[count]));
    }

    SendCommand(LUT_BLACK_TO_BLACK);                      //bb b
    for(count = 0; count < 42; count++) {
        SendData(pgm_read_byte(&lut_bb[count]));
    }
}


/**
 *  @brief: set the look-up table for quick display (partial refresh)
 */

void Epd42::SetLutQuick(void) {
    unsigned int count;
    SendCommand(LUT_FOR_VCOM);                            //vcom
    for(count = 0; count < 44; count++) {
        SendData(pgm_read_byte(&lut_vcom0_quick[count]));
    }

    SendCommand(LUT_WHITE_TO_WHITE);                      //ww --
    for(count = 0; count < 42; count++) {
        SendData(pgm_read_byte(&lut_ww_quick[count]));
    }

    SendCommand(LUT_BLACK_TO_WHITE);                      //bw r
    for(count = 0; count < 42; count++) {
        SendData(pgm_read_byte(&lut_bw_quick[count]));
    }

    SendCommand(LUT_WHITE_TO_BLACK);                      //wb w
    for(count = 0; count < 42; count++) {
        SendData(pgm_read_byte(&lut_wb_quick[count]));
    }

    SendCommand(LUT_BLACK_TO_BLACK);                      //bb b
    for(count = 0; count < 42; count++) {
        SendData(pgm_read_byte(&lut_bb_quick[count]));
    }
}


/**
 * @brief: refresh and displays the frame
 */
void Epd42::DisplayFrame(const unsigned char* frame_buffer) {
    SendCommand(RESOLUTION_SETTING);
    SendData(width >> 8);
    SendData(width & 0xff);
    SendData(height >> 8);
    SendData(height & 0xff);

    SendCommand(VCM_DC_SETTING);
    SendData(0x12);

    SendCommand(VCOM_AND_DATA_INTERVAL_SETTING);
    SendCommand(0x97);    //VBDF 17|D7 VBDW 97  VBDB 57  VBDF F7  VBDW 77  VBDB 37  VBDR B7

    if (frame_buffer != NULL) {
        SendCommand(DATA_START_TRANSMISSION_1);
        for(int i = 0; i < width / 8 * height; i++) {
            SendData(0xFF);      // bit set: white, bit reset: black
        }
        delay(2);
        SendCommand(DATA_START_TRANSMISSION_2);
        for(int i = 0; i < width / 8 * height; i++) {
            SendData(pgm_read_byte(&frame_buffer[i]));
        }
        delay(2);
    }

    SetLut();

    SendCommand(DISPLAY_REFRESH);
    delay(100);
    WaitUntilIdle();
}




/**
 * @brief: clear the frame data from the SRAM, this won't refresh the display
 */
void Epd42::ClearFrame(void) {
    SendCommand(RESOLUTION_SETTING);
    SendData(width >> 8);
    SendData(width & 0xff);
    SendData(height >> 8);
    SendData(height & 0xff);

    SendCommand(DATA_START_TRANSMISSION_1);
    delay(2);
    for(int i = 0; i < width / 8 * height; i++) {
        SendData(0xFF);
    }
    delay(2);
    SendCommand(DATA_START_TRANSMISSION_2);
    delay(2);
    for(int i = 0; i < width / 8 * height; i++) {
        SendData(0xFF);
    }
    delay(2);
}



/**
 * @brief: This displays the frame data from SRAM
 */
void Epd42::DisplayFrame(void) {
    SetLut();
    SendCommand(DISPLAY_REFRESH);
    delay(100);
    WaitUntilIdle();
}

void Epd42::DisplayFrameQuick(void) {
    SetLutQuick();
    SendCommand(DISPLAY_REFRESH);
  //  DelayMs(100);
  //  WaitUntilIdle();
}


/**
 * @brief: After this command is transmitted, the chip would enter the deep-sleep mode to save power.
 *         The deep sleep mode would return to standby by hardware reset. The only one parameter is a
 *         check code, the command would be executed if check code = 0xA5.
 *         You can use Epd::Reset() to awaken and use Epd::Init() to initialize.
 */
void Epd42::Sleep() {
    SendCommand(VCOM_AND_DATA_INTERVAL_SETTING);
    SendData(0x17);                       //border floating
    SendCommand(VCM_DC_SETTING);          //VCOM to 0V
    SendCommand(PANEL_SETTING);
    delay(100);

    SendCommand(POWER_SETTING);           //VG&VS to 0V fast
    SendData(0x00);
    SendData(0x00);
    SendData(0x00);
    SendData(0x00);
    SendData(0x00);
    delay(100);

    SendCommand(POWER_OFF);          //power off
    WaitUntilIdle();
    SendCommand(DEEP_SLEEP);         //deep sleep
    SendData(0xA5);
}

const unsigned char lut_vcom0[] PROGMEM =
{
0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
0x00, 0x17, 0x17, 0x00, 0x00, 0x02,
0x00, 0x0A, 0x01, 0x00, 0x00, 0x01,
0x00, 0x0E, 0x0E, 0x00, 0x00, 0x02,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char lut_vcom0_quick[] PROGMEM =
{
0x00, 0x0E, 0x00, 0x00, 0x00, 0x01,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};



const unsigned char lut_ww[] PROGMEM =
{
0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
0x40, 0x0A, 0x01, 0x00, 0x00, 0x01,
0xA0, 0x0E, 0x0E, 0x00, 0x00, 0x02,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char lut_ww_quick[] PROGMEM =
{
0xA0, 0x0E, 0x00, 0x00, 0x00, 0x01,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


const unsigned char lut_bw[] PROGMEM =
{
0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
0x40, 0x0A, 0x01, 0x00, 0x00, 0x01,
0xA0, 0x0E, 0x0E, 0x00, 0x00, 0x02,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


const unsigned char lut_bw_quick[] PROGMEM =
{
0xA0, 0x0E, 0x00, 0x00, 0x00, 0x01,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char lut_bb[] PROGMEM =
{
0x80, 0x17, 0x00, 0x00, 0x00, 0x02,
0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
0x80, 0x0A, 0x01, 0x00, 0x00, 0x01,
0x50, 0x0E, 0x0E, 0x00, 0x00, 0x02,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char lut_bb_quick[] PROGMEM =
{
0x50, 0x0E, 0x00, 0x00, 0x00, 0x01,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


const unsigned char lut_wb[] PROGMEM =
{
0x80, 0x17, 0x00, 0x00, 0x00, 0x02,
0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
0x80, 0x0A, 0x01, 0x00, 0x00, 0x01,
0x50, 0x0E, 0x0E, 0x00, 0x00, 0x02,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char lut_wb_quick[] PROGMEM =
{
0x50, 0x0E, 0x00, 0x00, 0x00, 0x01,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

#define PIN_OUT_SET 0x60000304
#define PIN_OUT_CLEAR 0x60000308

#ifdef ESP32
#define SSPI_USEANYPIN 1
#define PWRITE digitalWrite
#else
#define PWRITE ydigitalWrite
#endif

#ifndef SSPI_USEANYPIN
// uses about 2.75 usecs, 365 kb /sec
// however does not work with GPIO 16 !!!!
void ICACHE_RAM_ATTR Epd42::fastSPIwrite(uint8_t d,uint8_t dc) {

  WRITE_PERI_REG( PIN_OUT_CLEAR, 1<<cs_pin);
  WRITE_PERI_REG( PIN_OUT_CLEAR, 1<<sclk_pin);
  if(dc) WRITE_PERI_REG( PIN_OUT_SET, 1<<mosi_pin);
  else   WRITE_PERI_REG( PIN_OUT_CLEAR, 1<<mosi_pin);
  WRITE_PERI_REG( PIN_OUT_SET, 1<<sclk_pin);

  for(uint8_t bit = 0x80; bit; bit >>= 1) {
    WRITE_PERI_REG( PIN_OUT_CLEAR, 1<<sclk_pin);
    if(d&bit) WRITE_PERI_REG( PIN_OUT_SET, 1<<mosi_pin);
    else   WRITE_PERI_REG( PIN_OUT_CLEAR, 1<<mosi_pin);
    WRITE_PERI_REG( PIN_OUT_SET, 1<<sclk_pin);
  }
  WRITE_PERI_REG( PIN_OUT_SET, 1<<cs_pin);
}
#else

#ifndef ESP32
extern void ICACHE_RAM_ATTR ydigitalWrite(uint8_t pin, uint8_t val) {
  //stopWaveform(pin);
  if(pin < 16){
    if(val) GPOS = (1 << pin);
    else GPOC = (1 << pin);
  } else if(pin == 16){
    if(val) GP16O |= 1;
    else GP16O &= ~1;
  }
}
#endif
// about 13 us => 76 kb / sec
// can use any pin
void Epd42::fastSPIwrite(uint8_t d,uint8_t dc) {

  PWRITE(cs_pin, LOW);

  // transfer dc
  PWRITE(sclk_pin, LOW);
  if(dc) PWRITE(mosi_pin, HIGH);
  else        PWRITE(mosi_pin, LOW);
  PWRITE(sclk_pin, HIGH);

  for(uint8_t bit = 0x80; bit; bit >>= 1) {
    PWRITE(sclk_pin, LOW);
    if(d & bit) PWRITE(mosi_pin, HIGH);
    else        PWRITE(mosi_pin, LOW);
    PWRITE(sclk_pin, HIGH);
  }

  PWRITE(cs_pin, HIGH);
}
#endif

/* END OF FILE */
