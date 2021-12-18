/*
 * GC2145 register definitions.
 */
#ifndef __GC2145_REG_REGS_H__
#define __GC2145_REG_REGS_H__

#define CHIP_ID_HIGH 0XF0
#define CHIP_ID_LOW 0XF1
#define PLL_MODE1 0XF7
#define PLL_MODE2 0XF8
#define CM_MODE 0XF9
#define CLK_DIV_MODE 0XFA
#define RESET_RELATED   0xfe    // Bit[7]: Software reset 
                                // Bit[6]: cm reset 
                                // Bit[5]: mipi reset 
                                // Bit[4]: CISCTL_restart_n
                                // Bit[3]: NA
                                // Bit[2:0]: page select
                                //  000:page0
                                //  001:page1
                                //  010:page2
                                //  011:page3

//-page0----------------

#define P0_EXPOSURE_HIGH 0X03
#define P0_EXPOSURE_LOW 0X04
#define P0_HB_HIGH 0X05
#define P0_HB_LOW 0X06
#define P0_VB_HIGH 0X07
#define P0_VB_LOW 0X08
#define P0_ROW_START_HIGH 0X09
#define P0_ROW_START_LOW 0X0A
#define P0_COL_START_HIGH 0X0B
#define P0_COL_START_LOW 0X0C

#define P0_WIN_HEIGHT_HIGH 0X0D
#define P0_WIN_HEIGHT_LOW 0X0E
#define P0_WIN_WIDTH_HIGH 0X0F
#define P0_WIN_WIDTH_LOW 0X10
#define P0_ANALOG_MODE1 0X17
#define P0_ANALOG_MODE2 0X18

#define P0_SPECIAL_EFFECT 0X83
#define P0_OUTPUT_FORMAT 0x84 // Format select
                                // Bit[7]:YUV420 row switch
                                // Bit[6]:YUV420 col switch
                                // Bit[7]:YUV420_legacy
                                // Bit[4:0]:output data mode
                                //  5’h00 Cb Y Cr Y
                                //  5’h01 Cr Y Cb Y
                                //  5’h02 Y Cb Y Cr
                                //  5’h03 Y Cr Y Cb
                                //  5’h04 LSC bypass, C/Y
                                //  5’h05 LSC bypass, Y/C
                                //  5’h06 RGB 565
                                //  5’h0f bypass 10bits
                                //  5’h17 switch odd/even column /row to controls output Bayer pattern
                                //    00 RGBG
                                //    01 RGGB
                                //    10 BGGR
                                //    11 GBRG
                                //  5'h18 DNDD out mode
                                //  5'h19 LSC out mode
                                //  5;h1b EEINTP out mode
#define P0_FRAME_START 0X85
#define P0_SYNC_MODE 0X86
#define P0_MODULE_GATING 0X88
#define P0_BYPASS_MODE 0X89
#define P0_DEBUG_MODE2 0X8C
#define P0_DEBUG_MODE3 0X8D
#define P0_CROP_ENABLE 0X90
#define P0_OUT_WIN_Y1_HIGH 0X91
#define P0_OUT_WIN_Y1_LOW 0X92
#define P0_OUT_WIN_X1_HIGH 0X93
#define P0_OUT_WIN_X1_LOW 0X94
#define P0_OUT_WIN_HEIGHT_HIGH 0X95
#define P0_OUT_WIN_HEIGHT_LOW 0X96
#define P0_OUT_WIN_WIDTH_HIGH 0X97
#define P0_OUT_WIN_WIDTH_LOW 0X98
#define P0_SUBSAMPLE 0X99
#define P0_SUBSAMPLE_MODE 0X9A


#endif // __GC2145_REG_REGS_H__
