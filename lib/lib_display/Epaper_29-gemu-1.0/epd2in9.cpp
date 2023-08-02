/**
 *  @filename   :   epd2in9.cpp
 *  @brief      :   Implements for e-paper library
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     September 9 2017
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
#include "epd2in9.h"
#include "tasmota_options.h"

#ifndef EPD_29_V1
#define EPD_29_V2
#endif


//#define BUSY_PIN 16

Epd::Epd(int16_t width, int16_t height) :
Paint(width,height) {
}

void Epd::DisplayOnff(int8_t on) {
}

void Epd::Updateframe() {
#ifdef   EPD_29_V2
  if (mode == DISPLAY_INIT_PARTIAL) {
    SetFrameMemory_Partial(framebuffer, 0, 0, EPD_WIDTH,EPD_HEIGHT);
    DisplayFrame_Partial();
  } else {
    SetFrameMemory(framebuffer, 0, 0, EPD_WIDTH,EPD_HEIGHT);
    DisplayFrame();
  }
#else
  SetFrameMemory(framebuffer, 0, 0, EPD_WIDTH,EPD_HEIGHT);
  DisplayFrame();
#endif
  //Serial.printf("update\n");
}

void Epd::DisplayInit(int8_t p,int8_t size,int8_t rot,int8_t font) {
// ignore update mode
  if (p == DISPLAY_INIT_PARTIAL) {
    Init(lut_partial_update);
    //ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
    DisplayFrame();
    delay_busy(500);
    return;
    //Serial.printf("partial\n");
  } else if (p == DISPLAY_INIT_FULL) {
    Init(lut_full_update);
    //ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
    DisplayFrame();
    delay_busy(3500);
    //Serial.printf("full\n");
    return;
  } else {
    Updateframe();
  }
  setRotation(rot);
  invertDisplay(false);
  setTextWrap(false);         // Allow text to run off edges
  cp437(true);
  setTextFont(font&3);
  setTextSize(size&7);
  setTextColor(WHITE,BLACK);
  setCursor(0,0);
  fillScreen(BLACK);
  disp_bpp = 1;
}

void Epd::Begin(int16_t cs,int16_t mosi,int16_t sclk, int16_t rst, int16_t busy) {
  cs_pin = cs;
  mosi_pin = mosi;
  sclk_pin = sclk;
  rst_pin = rst;
  busy_pin = busy;
#ifdef BUSY_PIN
  busy_pin = BUSY_PIN;
#endif
}

void Epd::Init(int8_t p) {
  if (p == DISPLAY_INIT_PARTIAL) {
    Init(lut_partial_update);
  } else {
    Init(lut_full_update);
  }
  mode = p;
  ClearFrameMemory(0xFF);
  DisplayFrame();
  if (p == DISPLAY_INIT_PARTIAL) {
    delay_busy(350);
  } else {
    delay_busy(3500);
  }
}

int Epd::Init(const unsigned char* lut) {

    if (iniz) {
#ifndef EPD_29_V2
      this->lut = lut;
      SetLut(this->lut);
#endif
      return 0;
    }
    framebuffer = (uint8_t*)malloc(EPD_WIDTH * EPD_HEIGHT / 8);
    if (!framebuffer) return -1;

    pinMode(cs_pin, OUTPUT);
    pinMode(mosi_pin, OUTPUT);
    pinMode(sclk_pin, OUTPUT);

    digitalWrite(cs_pin,HIGH);
    digitalWrite(mosi_pin,LOW);
    digitalWrite(sclk_pin,LOW);

    if (rst_pin >= 0) {
      pinMode(rst_pin, OUTPUT);
      digitalWrite(rst_pin, HIGH);
    }

    if (busy_pin >= 0) {
      pinMode(busy_pin, INPUT_PULLUP);
    }

    width = EPD_WIDTH;
    height = EPD_HEIGHT;

#ifdef EPD_29_V2
    /* EPD hardware init start */
    Reset();

    SendCommand(0x12);  //SWRESET
    delay_busy(100);

    SendCommand(0x01); //Driver output control
    SendData(0x27);
    SendData(0x01);
    SendData(0x00);

    SendCommand(0x11); //data entry mode
    SendData(0x03);

    SetMemoryArea(0, 0, width-1, height-1);

    SendCommand(0x21); //  Display update control
    SendData(0x00);
    SendData(0x80);

    SetMemoryPointer(0, 0);
    delay_busy(10);

    SetLut_by_host(lut_full_update);
    mode = DISPLAY_INIT_FULL;

#else
    /* EPD hardware init start */
    this->lut = lut;
    Reset();
    SendCommand(DRIVER_OUTPUT_CONTROL);
    SendData((EPD_HEIGHT - 1) & 0xFF);
    SendData(((EPD_HEIGHT - 1) >> 8) & 0xFF);
    SendData(0x00);                     // GD = 0; SM = 0; TB = 0;
    SendCommand(BOOSTER_SOFT_START_CONTROL);
    SendData(0xD7);
    SendData(0xD6);
    SendData(0x9D);
    SendCommand(WRITE_VCOM_REGISTER);
    SendData(0xA8);                     // VCOM 7C
    SendCommand(SET_DUMMY_LINE_PERIOD);
    SendData(0x1A);                     // 4 dummy lines per gate
    SendCommand(SET_GATE_TIME);
    SendData(0x08);                     // 2us per line
    SendCommand(DATA_ENTRY_MODE_SETTING);
    SendData(0x03);                     // X increment; Y increment
    SetLut(this->lut);
#endif
    /* EPD hardware init end */
    iniz = 1;
    return 0;
}

/**
 *  @brief: basic function for sending commands
 */
void Epd::SendCommand(unsigned char command) {
    //DigitalWrite(dc_pin, LOW);
    //SpiTransfer(command);
    fastSPIwrite(command,0);
}

/**
 *  @brief: basic function for sending data
 */
void Epd::SendData(unsigned char data) {
  fastSPIwrite(data,1);
  //  DigitalWrite(dc_pin, HIGH);
  //  SpiTransfer(data);
}



void Epd::delay_busy(uint32_t wait) {
  if (busy_pin >= 0) {
    while (digitalRead(busy_pin) == HIGH) {      //LOW: idle, HIGH: busy
      delay(10);
    }
  } else {
    delay(wait);
  }
}


/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd::Sleep();
 */
void Epd::Reset(void) {
    if (rst_pin >= 0) {
      digitalWrite(rst_pin, LOW);                //module reset
      delay(200);
      digitalWrite(rst_pin, HIGH);
      delay(200);
    } else {
#ifdef EPD_29_V2
      SendCommand(0x12);
#endif
    }
}

#ifdef EPD_29_V2
void Epd::SetLut(const unsigned char *lut) {
	unsigned char count;
	SendCommand(0x32);
	for(count=0; count<153; count++)
		SendData(lut[count]);
	delay_busy(50);
}


void Epd::SetLut_by_host(const unsigned char *lut) {
  SetLut((unsigned char *)lut);
	SendCommand(0x3f);
	SendData(*(lut+153));
	SendCommand(0x03);	// gate voltage
	SendData(*(lut+154));
	SendCommand(0x04);	// source voltage
	SendData(*(lut+155));	// VSH
	SendData(*(lut+156));	// VSH2
	SendData(*(lut+157));	// VSL
	SendCommand(0x2c);		// VCOM
	SendData(*(lut+158));
}
#else

void Epd::SetLut_by_host(const unsigned char *lut)  {

}
/**
 *  @brief: set the look-up table register
 */
void Epd::SetLut(const unsigned char* lut) {
    this->lut = lut;
    SendCommand(WRITE_LUT_REGISTER);
    /* the length of look-up table is 30 bytes */
    for (int i = 0; i < 30; i++) {
        SendData(this->lut[i]);
    }
}
#endif

/**
 *  @brief: put an image buffer to the frame memory.
 *          this won't update the display.
 */
void Epd::SetFrameMemory(
    const unsigned char* image_buffer,
    uint16_t x,
    uint16_t y,
    uint16_t image_width,
    uint16_t image_height
) {

    uint16_t x_end;
    uint16_t y_end;

    if (
        image_buffer == NULL ||
        x < 0 || image_width < 0 ||
        y < 0 || image_height < 0
    ) {
        return;
    }

    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    x &= 0xFFF8;
    image_width &= 0xFFF8;
    if (x + image_width >= this->width) {
        x_end = this->width - 1;
    } else {
        x_end = x + image_width - 1;
    }
    if (y + image_height >= this->height) {
        y_end = this->height - 1;
    } else {
        y_end = y + image_height - 1;
    }

    if (!x && !y && image_width==this->width && image_height==this->height) {
      SetFrameMemory(image_buffer);
      return;
    }

    SetMemoryArea(x, y, x_end, y_end);
    SetMemoryPointer(x, y);
    SendCommand(WRITE_RAM);
    /* send the image data */
    for (uint16_t j = 0; j < y_end - y + 1; j++) {
        for (uint16_t i = 0; i < (x_end - x + 1) / 8; i++) {
            SendData(image_buffer[i + j * (image_width / 8)]^0xff);
        }
    }
}

/**
 *  @brief: put an image buffer to the frame memory.
 *          this won't update the display.
 *
 *          Question: When do you use this function instead of
 *          void SetFrameMemory(
 *              const unsigned char* image_buffer,
 *              int x,
 *              int y,
 *              int image_width,
 *              int image_height
 *          );
 *          Answer: SetFrameMemory with parameters only reads image data
 *          from the RAM but not from the flash in AVR chips (for AVR chips,
 *          you have to use the function pgm_read_byte to read buffers
 *          from the flash).
 */
void Epd::SetFrameMemory(const unsigned char* image_buffer) {
    SetMemoryArea(0, 0, this->width - 1, this->height - 1);
    SetMemoryPointer(0, 0);
    SendCommand(WRITE_RAM);
    /* send the image data */
    for (int i = 0; i < this->width / 8 * this->height; i++) {
        SendData(pgm_read_byte(&image_buffer[i])^0xff);
    }
}

/**
 *  @brief: clear the frame memory with the specified color.
 *          this won't update the display.
 */
void Epd::ClearFrameMemory(unsigned char color) {
    SetMemoryArea(0, 0, this->width - 1, this->height - 1);
    SetMemoryPointer(0, 0);
    SendCommand(WRITE_RAM);
    /* send the color data */
    for (int i = 0; i < this->width / 8 * this->height; i++) {
        SendData(color);
    }
}

/**
 *  @brief: update the display
 *          there are 2 memory areas embedded in the e-paper display
 *          but once this function is called,
 *          the the next action of SetFrameMemory or ClearFrame will
 *          set the other memory area.
 */
void Epd::DisplayFrame(void) {
    SendCommand(DISPLAY_UPDATE_CONTROL_2); // 0x22
#ifdef EPD_29_V2
    SendData(0xC7);
#else
    SendData(0xC4);
#endif
    SendCommand(MASTER_ACTIVATION); // 0x20
#ifndef EPD_29_V2
    SendCommand(TERMINATE_FRAME_READ_WRITE);
#endif
    delay_busy(10);
}

void Epd::DisplayFrame_Partial(void) {
    SendCommand(0x22);
    SendData(0x0F);
    SendCommand(0x20);
    delay_busy(10);
}

#ifdef EPD_29_V2

void Epd::SetFrameMemory_Partial(const unsigned char* image_buffer, int x, int y, int image_width, int image_height) {
    int x_end;
    int y_end;

    if (
        image_buffer == NULL ||
        x < 0 || image_width < 0 ||
        y < 0 || image_height < 0
    ) {
        return;
    }
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    x &= 0xF8;
    image_width &= 0xF8;
    if (x + image_width >= this->width) {
        x_end = this->width - 1;
    } else {
        x_end = x + image_width - 1;
    }
    if (y + image_height >= this->height) {
        y_end = this->height - 1;
    } else {
        y_end = y + image_height - 1;
    }

    if (rst_pin >= 0) {
      digitalWrite(rst_pin, LOW);
      delay(2);
      digitalWrite(rst_pin, HIGH);
      delay(2);
    } else {
      SendCommand(0x12);
    }
	SetLut(lut_partial_update);
	SendCommand(0x37);
	SendData(0x00);
	SendData(0x00);
	SendData(0x00);
	SendData(0x00);
	SendData(0x00);
	SendData(0x40);
	SendData(0x00);
	SendData(0x00);
	SendData(0x00);
	SendData(0x00);

	SendCommand(0x3C); //BorderWavefrom
	SendData(0x80);

	SendCommand(0x22);
	SendData(0xC0);
	SendCommand(0x20);
	delay_busy(100);

  SetMemoryArea(x, y, x_end, y_end);
  SetMemoryPointer(x, y);
  SendCommand(0x24);
  /* send the image data */
  for (int j = 0; j < y_end - y + 1; j++) {
    for (int i = 0; i < (x_end - x + 1) / 8; i++) {
        SendData(image_buffer[i + j * (image_width / 8)]^0xff);
    }
  }
}


/**
 *  @brief: private function to specify the memory area for data R/W
 */
void Epd::SetMemoryArea(int x_start, int y_start, int x_end, int y_end) {
    SendCommand(0x44);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    SendData((x_start >> 3) & 0xFF);
    SendData((x_end >> 3) & 0xFF);
    SendCommand(0x45);
    SendData(y_start & 0xFF);
    SendData((y_start >> 8) & 0xFF);
    SendData(y_end & 0xFF);
    SendData((y_end >> 8) & 0xFF);
}
#else

void Epd::SetFrameMemory_Partial(
    const unsigned char* image_buffer,
    int x,
    int y,
    int image_width,
    int image_height
) {
}
/**
 *  @brief: private function to specify the memory area for data R/W
 */
void Epd::SetMemoryArea(int x_start, int y_start, int x_end, int y_end) {
    SendCommand(SET_RAM_X_ADDRESS_START_END_POSITION);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    SendData((x_start >> 3) & 0xFF);
    SendData((x_end >> 3) & 0xFF);
    SendCommand(SET_RAM_Y_ADDRESS_START_END_POSITION);
    SendData(y_start & 0xFF);
    SendData((y_start >> 8) & 0xFF);
    SendData(y_end & 0xFF);
    SendData((y_end >> 8) & 0xFF);
}
#endif


#ifdef EPD_29_V2

/**
 *  @brief: private function to specify the start point for data R/W
 */
void Epd::SetMemoryPointer(int x, int y) {
    SendCommand(0x4E);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    SendData((x >> 3) & 0xFF);
    SendCommand(0x4F);
    SendData(y & 0xFF);
    SendData((y >> 8) & 0xFF);
    delay_busy(10);
}
#else
/**
 *  @brief: private function to specify the start point for data R/W
 */
void Epd::SetMemoryPointer(int x, int y) {
    SendCommand(SET_RAM_X_ADDRESS_COUNTER);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    SendData((x >> 3) & 0xFF);
    SendCommand(SET_RAM_Y_ADDRESS_COUNTER);
    SendData(y & 0xFF);
    SendData((y >> 8) & 0xFF);
    delay_busy(10);
}
#endif

/**
 *  @brief: After this command is transmitted, the chip would enter the
 *          deep-sleep mode to save power.
 *          The deep sleep mode would return to standby by hardware reset.
 *          You can use Epd::Init() to awaken
 */
void Epd::Sleep() {
    SendCommand(DEEP_SLEEP_MODE);
    delay_busy(10);
}

#ifdef EPD_29_V2

const unsigned char lut_partial_update[159] =
{
0x0,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x80,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x40,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0A,0x0,0x0,0x0,0x0,0x0,0x2,
0x1,0x0,0x0,0x0,0x0,0x0,0x0,
0x1,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x22,0x22,0x22,0x22,0x22,0x22,0x0,0x0,0x0,
0x22,0x17,0x41,0xB0,0x32,0x36,
};

const unsigned char lut_full_update[159] =
{
0x80,	0x66,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x40,	0x0,	0x0,	0x0,
0x10,	0x66,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x20,	0x0,	0x0,	0x0,
0x80,	0x66,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x40,	0x0,	0x0,	0x0,
0x10,	0x66,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x20,	0x0,	0x0,	0x0,
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x14,	0x8,	0x0,	0x0,	0x0,	0x0,	0x1,
0xA,	0xA,	0x0,	0xA,	0xA,	0x0,	0x1,
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x14,	0x8,	0x0,	0x1,	0x0,	0x0,	0x1,
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x1,
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x44,	0x44,	0x44,	0x44,	0x44,	0x44,	0x0,	0x0,	0x0,
0x22,	0x17,	0x41,	0x0,	0x32,	0x36
};

#else

const unsigned char lut_full_update[] =
{
    0x02, 0x02, 0x01, 0x11, 0x12, 0x12, 0x22, 0x22,
    0x66, 0x69, 0x69, 0x59, 0x58, 0x99, 0x99, 0x88,
    0x00, 0x00, 0x00, 0x00, 0xF8, 0xB4, 0x13, 0x51,
    0x35, 0x51, 0x51, 0x19, 0x01, 0x00
};

const unsigned char lut_partial_update[] =
{
    0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x13, 0x14, 0x44, 0x12,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif // EPD_29_V2

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
void ICACHE_RAM_ATTR Epd::fastSPIwrite(uint8_t d,uint8_t dc) {

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
extern void ICACHE_RAM_ATTR xdigitalWrite(uint8_t pin, uint8_t val) {
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
void Epd::fastSPIwrite(uint8_t d,uint8_t dc) {

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
