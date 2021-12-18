/*
 * GC032A register definitions.
 */
#ifndef __GC032A_REG_REGS_H__
#define __GC032A_REG_REGS_H__

#define SENSOR_ID_HIGH 0XF0
#define SENSOR_ID_LOW 0XF1
#define PAD_VB_HIZ_MODE 0XF2
#define SYNC_OUTPUT 0XF3
#define I2C_CONFIG 0XF4
#define PLL_MODE1 0XF7
#define PLL_MODE2 0XF8
#define CM_MODE 0XF9
#define ISP_DIV_MODE 0XFA
#define I2C_DEVICE_ID 0XFB
#define ANALOG_PWC 0XFC
#define ISP_DIV_MODE2 0XFD
#define RESET_RELATED   0XFE    // Bit[7]: Software reset 
                                // Bit[6]: cm reset 
                                // Bit[5]: spi reset 
                                // Bit[4]: CISCTL_restart_n
                                // Bit[3]: PLL_rst
                                // Bit[2:0]: page select
                                //  000:page0
                                //  001:page1
                                //  010:page2
                                //  011:page3

//----page0-----------------------------
#define P0_EXPOSURE_HIGH 0X03
#define P0_EXPOSURE_LOW 0X04
#define P0_HB_HIGH 0X05
#define P0_HB_LOW 0X06
#define P0_VB_HIGH 0X07
#define P0_VB_LOW 0X08
#define P0_ROW_START_HIGH 0X09
#define P0_ROW_START_LOW 0X0A
#define P0_COLUMN_START_HIGH 0X0B
#define P0_COLUMN_START_LOW 0X0C
#define P0_WINDOW_HEIGHT_HIGH 0X0D
#define P0_WINDOW_HEIGHT_LOW 0X0E
#define P0_WINDOW_WIDTH_HIGH 0X0F
#define P0_WINDOW_WIDTH_LOW 0X10
#define P0_SH_DELAY 0X11
#define P0_VS_ST 0X12
#define P0_VS_ET 0X13
#define P0_CISCTL_MODE1 0X17

#define P0_BLOCK_ENABLE_1 0X40
#define P0_AAAA_ENABLE 0X42
#define P0_SPECIAL_EFFECT 0X43
#define P0_SYNC_MODE 0X46
#define P0_GAIN_CODE 0X48
#define P0_DEBUG_MODE2 0X4C
#define P0_WIN_MODE 0X50
#define P0_OUT_WIN_Y1_HIGH 0X51
#define P0_OUT_WIN_Y1_LOW 0X52
#define P0_OUT_WIN_X1_HIGH 0X53
#define P0_OUT_WIN_X1_LOW 0X54
#define P0_OUT_WIN_HEIGHT_HIGH 0X55
#define P0_OUT_WIN_HEIGHT_LOW 0X56
#define P0_OUT_WIN_WIDTH_HIGH 0X57
#define P0_OUT_WIN_WIDTH_LOW 0X58

#define P0_GLOBAL_SATURATION 0XD0
#define P0_SATURATION_CB 0XD1
#define P0_SATURATION_CR 0XD2
#define P0_LUMA_CONTRAST 0XD3
#define P0_CONTRAST_CENTER 0XD4
#define P0_LUMA_OFFSET 0XD5
#define P0_FIXED_CB 0XDA
#define P0_FIXED_CR 0XDB

//----page3-----------------------------
#define P3_IMAGE_WIDTH_LOW 0X5B
#define P3_IMAGE_WIDTH_HIGH 0X5C
#define P3_IMAGE_HEIGHT_LOW 0X5D
#define P3_IMAGE_HEIGHT_HIGH 0X5E


#endif //__GC032A_REG_REGS_H__
