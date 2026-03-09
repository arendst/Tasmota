/**
 *  @filename   :   epd4in2.h
 *  @brief      :   Header file for Dual-color e-paper library epd4in2.cpp
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

#ifndef EPD4IN2_H
#define EPD4IN2_H

#include "epdpaint.h"

// Display resolution
#define EPD_WIDTH42       400
#define EPD_HEIGHT42      300

// EPD4IN2 commands
#define EPD_42_PANEL_SETTING                               0x00
#define EPD_42_POWER_SETTING                               0x01
#define EPD_42_POWER_OFF                                   0x02
#define EPD_42_POWER_OFF_SEQUENCE_SETTING                  0x03
#define EPD_42_POWER_ON                                    0x04
#define EPD_42_POWER_ON_MEASURE                            0x05
#define EPD_42_BOOSTER_SOFT_START                          0x06
#define EPD_42_DEEP_SLEEP                                  0x07
#define EPD_42_DATA_START_TRANSMISSION_1                   0x10
#define EPD_42_DATA_STOP                                   0x11
#define EPD_42_DISPLAY_REFRESH                             0x12
#define EPD_42_DATA_START_TRANSMISSION_2                   0x13
#define EPD_42_LUT_FOR_VCOM                                0x20
#define EPD_42_LUT_WHITE_TO_WHITE                          0x21
#define EPD_42_LUT_BLACK_TO_WHITE                          0x22
#define EPD_42_LUT_WHITE_TO_BLACK                          0x23
#define EPD_42_LUT_BLACK_TO_BLACK                          0x24
#define EPD_42_PLL_CONTROL                                 0x30
#define EPD_42_TEMPERATURE_SENSOR_COMMAND                  0x40
#define EPD_42_TEMPERATURE_SENSOR_SELECTION                0x41
#define EPD_42_TEMPERATURE_SENSOR_WRITE                    0x42
#define EPD_42_TEMPERATURE_SENSOR_READ                     0x43
#define EPD_42_VCOM_AND_DATA_INTERVAL_SETTING              0x50
#define EPD_42_LOW_POWER_DETECTION                         0x51
#define EPD_42_TCON_SETTING                                0x60
#define EPD_42_RESOLUTION_SETTING                          0x61
#define EPD_42_GSST_SETTING                                0x65
#define EPD_42_GET_STATUS                                  0x71
#define EPD_42_AUTO_MEASUREMENT_VCOM                       0x80
#define EPD_42_READ_VCOM_VALUE                             0x81
#define EPD_42_VCM_DC_SETTING                              0x82
#define EPD_42_PARTIAL_WINDOW                              0x90
#define EPD_42_PARTIAL_IN                                  0x91
#define EPD_42_PARTIAL_OUT                                 0x92
#define EPD_42_PROGRAM_MODE                                0xA0
#define EPD_42_ACTIVE_PROGRAMMING                          0xA1
#define EPD_42_READ_OTP                                    0xA2
#define EPD_42_POWER_SAVING                                0xE3

extern const unsigned char lut_vcom0[];
extern const unsigned char lut_ww[];
extern const unsigned char lut_bw[];
extern const unsigned char lut_bb[];
extern const unsigned char lut_wb[];

extern const unsigned char lut_vcom0_quick[];
extern const unsigned char lut_ww_quick[];
extern const unsigned char lut_bw_quick[];
extern const unsigned char lut_bb_quick[];
extern const unsigned char lut_wb_quick[];




class Epd42 : public Paint  {
public:
    Epd42(int16_t width, int16_t height);

    int  Init(void);
    void Init(int8_t p);
    void SendCommand(unsigned char command);
    void SendData(unsigned char data);
    void WaitUntilIdle(void);
    void Reset(void);

    void SetPartialWindow(const unsigned char* frame_buffer, int x, int y, int w, int l, int dtm);

    void SetPartialWindowBlack(const unsigned char* buffer_black, int x, int y, int w, int l);
    void SetPartialWindowRed(const unsigned char* buffer_red, int x, int y, int w, int l);
    void SetLut(void);
	  void SetLutQuick(void);
    void DisplayFrame(const unsigned char* frame_buffer);
    void DisplayFrame(void);
	  void DisplayFrameQuick(void);
    void ClearFrame(void);
    void Sleep(void);

    void DisplayOnff(int8_t on);
    void DisplayInit(int8_t p,int8_t size,int8_t rot,int8_t font);
    void Begin(int16_t p1,int16_t p2,int16_t p3);
    void Updateframe();

private:
  void fastSPIwrite(uint8_t d,uint8_t dc);
  uint8_t cs_pin;
  uint8_t mosi_pin;
  uint8_t sclk_pin;
  int16_t width;
  int16_t height;
};

#endif /* EPD4IN2_H */

/* END OF FILE */
