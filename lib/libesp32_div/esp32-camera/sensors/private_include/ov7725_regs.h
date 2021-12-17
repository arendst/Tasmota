/*
 * This file is part of the OpenMV project.
 * Copyright (c) 2013/2014 Ibrahim Abdelkader <i.abdalkader@gmail.com>
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * OV2640 register definitions.
 */
#ifndef __REG_REGS_H__
#define __REG_REGS_H__
#define GAIN                    0x00 /* AGC – Gain control gain setting  */
#define BLUE                    0x01 /* AWB – Blue channel gain setting  */
#define RED                     0x02 /* AWB – Red channel gain setting   */
#define GREEN                   0x03 /* AWB – Green channel gain setting */
#define BAVG                    0x05 /* U/B Average Level   */
#define GAVG                    0x06 /* Y/Gb Average Level  */
#define RAVG                    0x07 /* V/R Average Level   */
#define AECH                    0x08 /* Exposure Value – AEC MSBs */

#define COM2                    0x09 /* Common Control 2 */
#define COM2_SOFT_SLEEP         0x10 /* Soft sleep mode  */
#define COM2_OUT_DRIVE_1x       0x00 /* Output drive capability 1x */
#define COM2_OUT_DRIVE_2x       0x01 /* Output drive capability 2x */
#define COM2_OUT_DRIVE_3x       0x02 /* Output drive capability 3x */
#define COM2_OUT_DRIVE_4x       0x03 /* Output drive capability 4x */

#define REG_PID                     0x0A /* Product ID Number MSB */
#define REG_VER                     0x0B /* Product ID Number LSB */

#define COM3                    0x0C /* Common Control 3                                        */
#define COM3_VFLIP              0x80 /* Vertical flip image ON/OFF selection                    */
#define COM3_MIRROR             0x40 /* Horizontal mirror image ON/OFF selection                */
#define COM3_SWAP_BR            0x20 /* Swap B/R output sequence in RGB output mode             */
#define COM3_SWAP_YUV           0x10 /* Swap Y/UV output sequence in YUV output mode            */
#define COM3_SWAP_MSB           0x08 /* Swap output MSB/LSB                                     */
#define COM3_TRI_CLOCK          0x04 /* Tri-state option for output clock at power-down period  */
#define COM3_TRI_DATA           0x02 /* Tri-state option for output data at power-down period   */
#define COM3_COLOR_BAR          0x01 /* Sensor color bar test pattern output enable             */
#define COM3_SET_CBAR(r, x)     ((r&0xFE)|((x&1)<<0))
#define COM3_SET_MIRROR(r, x)   ((r&0xBF)|((x&1)<<6))
#define COM3_SET_FLIP(r, x)     ((r&0x7F)|((x&1)<<7))

#define COM4                    0x0D /* Common Control 4         */
#define COM4_PLL_BYPASS         0x00 /* Bypass PLL               */
#define COM4_PLL_4x             0x40 /* PLL frequency 4x         */
#define COM4_PLL_6x             0x80 /* PLL frequency 6x         */
#define COM4_PLL_8x             0xc0 /* PLL frequency 8x         */
#define COM4_AEC_FULL           0x00 /* AEC evaluate full window */
#define COM4_AEC_1_2            0x10 /* AEC evaluate 1/2 window  */
#define COM4_AEC_1_4            0x20 /* AEC evaluate 1/4 window  */
#define COM4_AEC_2_3            0x30 /* AEC evaluate 2/3 window  */

#define COM5                    0x0E /* Common Control 5 */
#define COM5_AFR                0x80 /* Auto frame rate control ON/OFF selection (night mode) */
#define COM5_AFR_SPEED          0x40 /* Auto frame rate control speed selection */
#define COM5_AFR_0              0x00 /* No reduction of frame rate          */
#define COM5_AFR_1_2            0x10 /* Max reduction to 1/2 frame rate     */
#define COM5_AFR_1_4            0x20 /* Max reduction to 1/4 frame rate     */
#define COM5_AFR_1_8            0x30 /* Max reduction to 1/8 frame rate     */
#define COM5_AFR_4x             0x04 /* Add frame when AGC reaches 4x gain  */
#define COM5_AFR_8x             0x08 /* Add frame when AGC reaches 8x gain  */
#define COM5_AFR_16x            0x0c /* Add frame when AGC reaches 16x gain */
#define COM5_AEC_NO_LIMIT       0x01 /* No limit to AEC increase step       */

#define COM6                    0x0F /* Common Control 6 */
#define COM6_AUTO_WINDOW        0x01 /* Auto window setting ON/OFF selection when format changes */

#define AEC                     0x10 /* AEC[7:0] (see register AECH for AEC[15:8]) */
#define CLKRC                   0x11 /* Internal Clock */

#define COM7                    0x12 /* Common Control 7         */
#define COM7_RESET              0x80 /* SCCB Register Reset      */
#define COM7_RES_VGA            0x00 /* Resolution VGA           */
#define COM7_RES_QVGA           0x40 /* Resolution QVGA          */
#define COM7_BT656              0x20 /* BT.656 protocol ON/OFF   */
#define COM7_SENSOR_RAW         0x10 /* Sensor RAW               */
#define COM7_FMT_GBR422         0x00 /* RGB output format GBR422 */
#define COM7_FMT_RGB565         0x04 /* RGB output format RGB565 */
#define COM7_FMT_RGB555         0x08 /* RGB output format RGB555 */
#define COM7_FMT_RGB444         0x0C /* RGB output format RGB444 */
#define COM7_FMT_YUV            0x00 /* Output format YUV        */
#define COM7_FMT_P_BAYER        0x01 /* Output format Processed Bayer RAW */
#define COM7_FMT_RGB            0x02 /* Output format RGB        */
#define COM7_FMT_R_BAYER        0x03 /* Output format Bayer RAW  */
#define COM7_SET_FMT(r, x)      ((r&0xFC)|((x&0x3)<<0))
#define COM7_SET_RGB(r, x)      ((r&0xF0)|(x&0x0C)|COM7_FMT_RGB)

#define COM8                    0x13 /* Common Control 8                */
#define COM8_FAST_AUTO          0x80 /* Enable fast AGC/AEC algorithm   */
#define COM8_STEP_VSYNC         0x00 /* AEC - Step size limited to vertical blank */
#define COM8_STEP_UNLIMIT       0x40 /* AEC - Step size unlimited step size       */
#define COM8_BANDF_EN           0x20 /* Banding filter ON/OFF */
#define COM8_AEC_BANDF          0x10 /* Enable AEC below banding value */
#define COM8_AEC_FINE_EN        0x08 /* Fine AEC ON/OFF control */
#define COM8_AGC_EN             0x04 /* AGC Enable */
#define COM8_AWB_EN             0x02 /* AWB Enable */
#define COM8_AEC_EN             0x01 /* AEC Enable */
#define COM8_SET_AGC(r, x)      ((r&0xFB)|((x&0x1)<<2))
#define COM8_SET_AWB(r, x)      ((r&0xFD)|((x&0x1)<<1))
#define COM8_SET_AEC(r, x)      ((r&0xFE)|((x&0x1)<<0))

#define COM9                    0x14 /* Common Control 9 */
#define COM9_HISTO_AVG          0x80 /* Histogram or average based AEC/AGC selection */
#define COM9_AGC_GAIN_2x        0x00 /* Automatic Gain Ceiling 2x  */
#define COM9_AGC_GAIN_4x        0x10 /* Automatic Gain Ceiling 4x  */
#define COM9_AGC_GAIN_8x        0x20 /* Automatic Gain Ceiling 8x  */
#define COM9_AGC_GAIN_16x       0x30 /* Automatic Gain Ceiling 16x */
#define COM9_AGC_GAIN_32x       0x40 /* Automatic Gain Ceiling 32x */
#define COM9_DROP_VSYNC         0x04 /* Drop VSYNC output of corrupt frame */
#define COM9_DROP_HREF          0x02 /* Drop HREF output of corrupt frame  */
#define COM9_SET_AGC(r, x)      ((r&0x8F)|((x&0x07)<<4))

#define COM10                   0x15 /* Common Control 10 */
#define COM10_NEGATIVE          0x80 /* Output negative data */
#define COM10_HSYNC_EN          0x40 /* HREF changes to HSYNC */
#define COM10_PCLK_FREE         0x00 /* PCLK output option: free running PCLK */
#define COM10_PCLK_MASK         0x20 /* PCLK output option: masked during horizontal blank  */
#define COM10_PCLK_REV          0x10 /* PCLK reverse */
#define COM10_HREF_REV          0x08 /* HREF reverse */
#define COM10_VSYNC_FALLING     0x00 /* VSYNC changes on falling edge of PCLK */
#define COM10_VSYNC_RISING      0x04 /* VSYNC changes on rising edge of PCLK */
#define COM10_VSYNC_NEG         0x02 /* VSYNC negative */
#define COM10_OUT_RANGE_8       0x01 /* Output data range: Full range */
#define COM10_OUT_RANGE_10      0x00 /* Output data range: Data from [10] to [F0] (8 MSBs) */

#define REG16                   0x16 /* Register 16 */
#define REG16_BIT_SHIFT         0x80 /* Bit shift test pattern options */
#define HSTART                  0x17 /* Horizontal Frame (HREF column) Start 8 MSBs (2 LSBs are at HREF[5:4]) */
#define HSIZE                   0x18 /* Horizontal Sensor Size (2 LSBs are at HREF[1:0]) */
#define VSTART                  0x19 /* Vertical Frame (row) Start 8 MSBs (1 LSB is at HREF[6]) */
#define VSIZE                   0x1A /* Vertical Sensor Size (1 LSB is at HREF[2]) */
#define PSHFT                   0x1B /* Data Format - Pixel Delay Select */
#define REG_MIDH                    0x1C /* Manufacturer ID Byte – High */
#define REG_MIDL                    0x1D /* Manufacturer ID Byte – Low */
#define LAEC                    0x1F /* Fine AEC Value - defines exposure value less than one row period */

#define COM11                   0x20 /* Common Control 11 */
#define COM11_SNGL_FRAME_EN     0x02 /* Single frame ON/OFF selection */
#define COM11_SNGL_XFR_TRIG     0x01 /* Single frame transfer trigger */

#define BDBASE                  0x22 /* Banding Filter Minimum AEC Value */
#define DBSTEP                  0x23 /* Banding Filter Maximum Step */
#define AEW                     0x24 /* AGC/AEC - Stable Operating Region (Upper Limit) */
#define AEB                     0x25 /* AGC/AEC - Stable Operating Region (Lower Limit) */
#define VPT                     0x26 /* AGC/AEC Fast Mode Operating Region */
#define REG28                   0x28 /* Selection on the number of dummy rows, N */
#define HOUTSIZE                0x29 /* Horizontal Data Output Size MSBs (2 LSBs at register EXHCH[1:0]) */
#define EXHCH                   0x2A /* Dummy Pixel Insert MSB */
#define EXHCL                   0x2B /* Dummy Pixel Insert LSB */
#define VOUTSIZE                0x2C /* Vertical Data Output Size MSBs (LSB at register EXHCH[2])       */
#define ADVFL                   0x2D /* LSB of Insert Dummy Rows in Vertical Sync (1 bit equals 1 row)  */
#define ADVFH                   0x2E /* MSB of Insert Dummy Rows in Vertical Sync */
#define YAVE                    0x2F /* Y/G Channel Average Value */
#define LUMHTH                  0x30 /* Histogram AEC/AGC Luminance High Level Threshold */
#define LUMLTH                  0x31 /* Histogram AEC/AGC Luminance Low Level Threshold  */
#define HREF                    0x32 /* Image Start and Size Control */
#define DM_LNL                  0x33 /* Dummy Row Low 8 Bits  */
#define DM_LNH                  0x34 /* Dummy Row High 8 Bits */
#define ADOFF_B                 0x35 /* AD Offset Compensation Value for B Channel  */
#define ADOFF_R                 0x36 /* AD Offset Compensation Value for R Channel  */
#define ADOFF_GB                0x37 /* AD Offset Compensation Value for GB Channel */
#define ADOFF_GR                0x38 /* AD Offset Compensation Value for GR Channel */
#define OFF_B                   0x39 /* AD Offset Compensation Value for B Channel  */
#define OFF_R                   0x3A /* AD Offset Compensation Value for R Channel  */
#define OFF_GB                  0x3B /* AD Offset Compensation Value for GB Channel */
#define OFF_GR                  0x3C /* AD Offset Compensation Value for GR Channel */
#define COM12                   0x3D /* DC offset compensation for analog process */

#define COM13                   0x3E /* Common Control 13 */
#define COM13_BLC_EN            0x80 /* BLC enable */
#define COM13_ADC_EN            0x40 /* ADC channel BLC ON/OFF control */
#define COM13_ANALOG_BLC        0x20 /* Analog processing channel BLC ON/OFF control */
#define COM13_ABLC_GAIN_EN      0x04 /* ABLC gain trigger enable */

#define COM14                   0x3F /* Common Control 14 */
#define COM15                   0x40 /* Common Control 15 */
#define COM16                   0x41 /* Common Control 16 */
#define TGT_B                   0x42 /* BLC Blue Channel Target Value   */
#define TGT_R                   0x43 /* BLC Red Channel Target Value    */
#define TGT_GB                  0x44 /* BLC Gb Channel Target Value     */
#define TGT_GR                  0x45 /* BLC Gr Channel Target Value     */

#define LC_CTR                  0x46 /* Lens Correction Control */
#define LC_CTR_RGB_COMP_1       0x00 /* R, G, and B channel compensation coefficient is set by LC_COEF (0x49) */
#define LC_CTR_RGB_COMP_3       0x04 /* R, G, and B channel compensation coefficient is set by registers
                                        LC_COEFB (0x4B), LC_COEF (0x49), and LC_COEFR (0x4C), respectively */
#define LC_CTR_EN               0x01 /* Lens correction enable */
#define LC_XC                   0x47 /* X Coordinate of Lens Correction Center Relative to Array Center */
#define LC_YC                   0x48 /* Y Coordinate of Lens Correction Center Relative to Array Center */
#define LC_COEF                 0x49 /* Lens Correction Coefficient */
#define LC_RADI                 0x4A /* Lens Correction Radius */
#define LC_COEFB                0x4B /* Lens Correction B Channel Compensation Coefficient */
#define LC_COEFR                0x4C /* Lens Correction R Channel Compensation Coefficient */

#define FIXGAIN                 0x4D /* Analog Fix Gain Amplifier */
#define AREF0                   0x4E /* Sensor Reference Control */
#define AREF1                   0x4F /* Sensor Reference Current Control */
#define AREF2                   0x50 /* Analog Reference Control */
#define AREF3                   0x51 /* ADC Reference Control */
#define AREF4                   0x52 /* ADC Reference Control */
#define AREF5                   0x53 /* ADC Reference Control */
#define AREF6                   0x54 /* Analog Reference Control */
#define AREF7                   0x55 /* Analog Reference Control */
#define UFIX                    0x60 /* U Channel Fixed Value Output */
#define VFIX                    0x61 /* V Channel Fixed Value Output */
#define AWBB_BLK                0x62 /* AWB Option for Advanced AWB  */

#define AWB_CTRL0               0x63 /* AWB Control Byte 0   */
#define AWB_CTRL0_GAIN_EN       0x80 /* AWB gain enable      */
#define AWB_CTRL0_CALC_EN       0x40 /* AWB calculate enable */
#define AWB_CTRL0_WBC_MASK      0x0F /* WBC threshold 2      */

#define DSP_CTRL1               0x64 /* DSP Control Byte 1                  */
#define DSP_CTRL1_FIFO_EN       0x80 /* FIFO enable/disable selection       */
#define DSP_CTRL1_UV_EN         0x40 /* UV adjust function ON/OFF selection */
#define DSP_CTRL1_SDE_EN        0x20 /* SDE enable                          */
#define DSP_CTRL1_MTRX_EN       0x10 /* Color matrix ON/OFF selection       */
#define DSP_CTRL1_INTRP_EN      0x08 /* Interpolation ON/OFF selection      */
#define DSP_CTRL1_GAMMA_EN      0x04 /* Gamma function ON/OFF selection     */
#define DSP_CTRL1_BLACK_EN      0x02 /* Black defect auto correction ON/OFF */
#define DSP_CTRL1_WHITE_EN      0x01 /* White defect auto correction ON/OFF */

#define DSP_CTRL2               0x65 /* DSP Control Byte 2          */
#define DSP_CTRL2_VDCW_EN       0x08 /* Vertical DCW enable         */
#define DSP_CTRL2_HDCW_EN       0x04 /* Horizontal DCW enable       */
#define DSP_CTRL2_VZOOM_EN      0x02 /* Vertical zoom out enable    */
#define DSP_CTRL2_HZOOM_EN      0x01 /* Horizontal zoom out enable  */

#define DSP_CTRL3               0x66 /* DSP Control Byte 3                      */
#define DSP_CTRL3_UV_EN         0x80 /* UV output sequence option               */
#define DSP_CTRL3_CBAR_EN       0x20 /* DSP color bar ON/OFF selection          */
#define DSP_CTRL3_FIFO_EN       0x08 /* FIFO power down ON/OFF selection        */
#define DSP_CTRL3_SCAL1_PWDN    0x04 /* Scaling module power down control 1     */
#define DSP_CTRL3_SCAL2_PWDN    0x02 /* Scaling module power down control 2     */
#define DSP_CTRL3_INTRP_PWDN    0x01 /* Interpolation module power down control */
#define DSP_CTRL3_SET_CBAR(r, x)    ((r&0xDF)|((x&1)<<5))


#define DSP_CTRL4               0x67 /* DSP Control Byte 4          */
#define DSP_CTRL4_YUV_RGB       0x00 /* Output selection YUV or RGB */
#define DSP_CTRL4_RAW8          0x02 /* Output selection RAW8       */
#define DSP_CTRL4_RAW10         0x03 /* Output selection RAW10      */


#define AWB_BIAS                0x68 /* AWB BLC Level Clip */
#define AWB_CTRL1               0x69 /* AWB Control 1 */
#define AWB_CTRL2               0x6A /* AWB Control 2 */

#define AWB_CTRL3               0x6B /* AWB Control 3 */
#define AWB_CTRL3_ADVANCED      0x80 /* AWB mode select - Advanced AWB */
#define AWB_CTRL3_SIMPLE        0x00 /* AWB mode select - Simple AWB */

#define AWB_CTRL4               0x6C /* AWB Control 4  */
#define AWB_CTRL5               0x6D /* AWB Control 5  */
#define AWB_CTRL6               0x6E /* AWB Control 6  */
#define AWB_CTRL7               0x6F /* AWB Control 7  */
#define AWB_CTRL8               0x70 /* AWB Control 8  */
#define AWB_CTRL9               0x71 /* AWB Control 9  */
#define AWB_CTRL10              0x72 /* AWB Control 10 */
#define AWB_CTRL11              0x73 /* AWB Control 11 */
#define AWB_CTRL12              0x74 /* AWB Control 12 */
#define AWB_CTRL13              0x75 /* AWB Control 13 */
#define AWB_CTRL14              0x76 /* AWB Control 14 */
#define AWB_CTRL15              0x77 /* AWB Control 15 */
#define AWB_CTRL16              0x78 /* AWB Control 16 */
#define AWB_CTRL17              0x79 /* AWB Control 17 */
#define AWB_CTRL18              0x7A /* AWB Control 18 */
#define AWB_CTRL19              0x7B /* AWB Control 19 */
#define AWB_CTRL20              0x7C /* AWB Control 20 */
#define AWB_CTRL21              0x7D /* AWB Control 21 */
#define GAM1                    0x7E /* Gamma Curve 1st Segment Input End Point 0x04 Output Value */
#define GAM2                    0x7F /* Gamma Curve 2nd Segment Input End Point 0x08 Output Value */
#define GAM3                    0x80 /* Gamma Curve 3rd Segment Input End Point 0x10 Output Value */
#define GAM4                    0x81 /* Gamma Curve 4th Segment Input End Point 0x20 Output Value */
#define GAM5                    0x82 /* Gamma Curve 5th Segment Input End Point 0x28 Output Value */
#define GAM6                    0x83 /* Gamma Curve 6th Segment Input End Point 0x30 Output Value */
#define GAM7                    0x84 /* Gamma Curve 7th Segment Input End Point 0x38 Output Value */
#define GAM8                    0x85 /* Gamma Curve 8th Segment Input End Point 0x40 Output Value */
#define GAM9                    0x86 /* Gamma Curve 9th Segment Input End Point 0x48 Output Value */
#define GAM10                   0x87 /* Gamma Curve 10th Segment Input End Point 0x50 Output Value */
#define GAM11                   0x88 /* Gamma Curve 11th Segment Input End Point 0x60 Output Value */
#define GAM12                   0x89 /* Gamma Curve 12th Segment Input End Point 0x70 Output Value */
#define GAM13                   0x8A /* Gamma Curve 13th Segment Input End Point 0x90 Output Value */
#define GAM14                   0x8B /* Gamma Curve 14th Segment Input End Point 0xB0 Output Value */
#define GAM15                   0x8C /* Gamma Curve 15th Segment Input End Point 0xD0 Output Value */
#define SLOP                    0x8D /* Gamma Curve Highest Segment Slope */
#define DNSTH                   0x8E /* De-noise Threshold */
#define EDGE0                   0x8F /* Edge Enhancement Strength Control */
#define EDGE1                   0x90 /* Edge Enhancement Threshold Control */
#define DNSOFF                  0x91 /* Auto De-noise Threshold Control */
#define EDGE2                   0x92 /* Edge Enhancement Strength Upper Limit */
#define EDGE3                   0x93 /* Edge Enhancement Strength Upper Limit */
#define MTX1                    0x94 /* Matrix Coefficient 1 */
#define MTX2                    0x95 /* Matrix Coefficient 2 */
#define MTX3                    0x96 /* Matrix Coefficient 3 */
#define MTX4                    0x97 /* Matrix Coefficient 4 */
#define MTX5                    0x98 /* Matrix Coefficient 5 */
#define MTX6                    0x99 /* Matrix Coefficient 6 */

#define MTX_CTRL                0x9A /* Matrix Control */
#define MTX_CTRL_DBL_EN         0x80 /* Matrix double ON/OFF selection */

#define BRIGHTNESS              0x9B /* Brightness Control */
#define CONTRAST                0x9C /* Contrast Gain */
#define UVADJ0                  0x9E /* Auto UV Adjust Control 0 */
#define UVADJ1                  0x9F /* Auto UV Adjust Control 1 */
#define SCAL0                   0xA0 /* DCW Ratio Control */
#define SCAL1                   0xA1 /* Horizontal Zoom Out Control */
#define SCAL2                   0xA2 /* Vertical Zoom Out Control */
#define FIFODLYM                0xA3 /* FIFO Manual Mode Delay Control */
#define FIFODLYA                0xA4 /* FIFO Auto Mode Delay Control */

#define SDE                     0xA6 /* Special Digital Effect Control  */
#define SDE_NEGATIVE_EN         0x40 /* Negative image enable           */
#define SDE_GRAYSCALE_EN        0x20 /* Gray scale image enable         */
#define SDE_V_FIXED_EN          0x10 /* V fixed value enable            */
#define SDE_U_FIXED_EN          0x08 /* U fixed value enable            */
#define SDE_CONT_BRIGHT_EN      0x04 /* Contrast/Brightness enable      */
#define SDE_SATURATION_EN       0x02 /* Saturation enable               */
#define SDE_HUE_EN              0x01 /* Hue enable                      */

#define USAT                    0xA7 /* U Component Saturation Gain     */
#define VSAT                    0xA8 /* V Component Saturation Gain     */
#define HUECOS                  0xA9 /* Cosine value × 0x80             */
#define HUESIN                  0xAA /* Sine value × 0x80               */
#define SIGN_BIT                0xAB /* Sign Bit for Hue and Brightness */

#define DSPAUTO                 0xAC /* DSP Auto Function ON/OFF Control */
#define DSPAUTO_AWB_EN          0x80 /* AWB auto threshold control */
#define DSPAUTO_DENOISE_EN      0x40 /* De-noise auto threshold control */
#define DSPAUTO_EDGE_EN         0x20 /* Sharpness (edge enhancement) auto strength control */
#define DSPAUTO_UV_EN           0x10 /* UV adjust auto slope control */
#define DSPAUTO_SCAL0_EN        0x08 /* Auto scaling factor control (register SCAL0 (0xA0)) */
#define DSPAUTO_SCAL1_EN        0x04 /* Auto scaling factor control (registers SCAL1 (0xA1 and SCAL2 (0xA2))*/
#define SET_REG(reg, x)         (##reg_DEFAULT|x)
#endif //__REG_REGS_H__
