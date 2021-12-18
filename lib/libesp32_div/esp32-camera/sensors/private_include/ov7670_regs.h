/*
 * This file is for the OpenMV project so the OV7670 can be used
 * author: Juan Schiavoni <juanjoseschiavoni@hotmail.com>
 *
 * OV7670 register definitions.
 */
#ifndef __OV7670_REG_REGS_H__
#define __OV7670_REG_REGS_H__
#define GAIN                    0x00 /* AGC – Gain control gain setting  */
#define BLUE                    0x01 /* AWB – Blue channel gain setting  */
#define RED                     0x02 /* AWB – Red channel gain setting   */
#define VREF                    0x03 /* AWB – Green channel gain setting */
#define COM1			        0x04 /* Common Control 1 */
#define BAVG                    0x05 /* U/B Average Level   */
#define GAVG                    0x06 /* Y/Gb Average Level  */
#define AECH                    0x07 /* Exposure VAlue - AEC MSB 5 bits  */
#define RAVG                    0x08 /* V/R Average Level */

#define COM2                    0x09 /* Common Control 2 */
#define COM2_SOFT_SLEEP         0x10 /* Soft sleep mode  */
#define COM2_OUT_DRIVE_1x       0x00 /* Output drive capability 1x */
#define COM2_OUT_DRIVE_2x       0x01 /* Output drive capability 2x */
#define COM2_OUT_DRIVE_3x       0x02 /* Output drive capability 3x */
#define COM2_OUT_DRIVE_4x       0x03 /* Output drive capability 4x */

#define REG_PID                 0x0A /* Product ID Number MSB */
#define REG_VER                 0x0B /* Product ID Number LSB */

#define COM3                    0x0C /* Common Control 3 		 */
#define COM3_SWAP_OUT           0x40 /* Output data MSB/LSB swap */
#define COM3_TRI_CLK            0x20 /* Tri-state output clock   */
#define COM3_TRI_DATA           0x10 /* Tri-state option output  */
#define COM3_SCALE_EN           0x08 /* Scale enable             */
#define COM3_DCW                0x04 /* DCW enable               */

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
#define COM7_FMT_RGB            0x04 /* Output format RGB        */
#define COM7_FMT_R_BAYER        0x03 /* Output format Bayer RAW  */
#define COM7_SET_FMT(r, x)      ((r&0xFC)|((x&0x5)<<0))

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

#define RSVD_16                 0x16 /* Reserved register */

#define HSTART                  0x17  /* Horizontal Frame (HREF column) Start high 8-bit(low 3 bits are at HREF[2:0]) */
#define HSTOP                   0x18  /* Horizontal Frame (HREF column) end high 8-bit (low 3 bits are at HREF[5:3])  */
#define VSTART                  0x19  /* Vertical Frame (row) Start high 8-bit (low 2 bits are at VREF[1:0]) */
#define VSTOP                   0x1A  /* Vertical Frame (row) End high 8-bit (low 2 bits are at VREF[3:2]) */
#define PSHFT                   0x1B  /* Data Format - Pixel Delay Select */
#define REG_MIDH                0x1C  /* Manufacturer ID Byte – High */
#define REG_MIDL                0x1D  /* Manufacturer ID Byte – Low */

#define MVFP			        0x1E  /* Mirror/Vflip Enable */
#define   MVFP_MIRROR	        0x20  /* Mirror image */
#define   MVFP_FLIP	            0x10  /* Vertical flip */
#define   MVFP_SUN	            0x02  /* Black sun enable */
#define MVFP_SET_MIRROR(r,x)	((r&0xDF)|((x&1)<<5)) /* change only bit5 according to x */
#define MVFP_SET_FLIP(r,x)	    ((r&0xEF)|((x&1)<<4)) /* change only bit4 according to x */

#define LAEC                    0x1F /* Fine AEC Value - defines exposure value less than one row period (Reserved?) */
#define ADCCTR0                 0x20 /* ADC control */
#define ADCCTR1			        0x21 /* reserved */
#define ADCCTR2                 0x22 /* reserved */
#define ADCCTR3                 0x23 /* reserved */
#define AEW                     0x24 /* AGC/AEC - Stable Operating Region (Upper Limit) */
#define AEB                     0x25 /* AGC/AEC - Stable Operating Region (Lower Limit) */
#define VPT                     0x26 /* AGC/AEC Fast Mode Operating Region */
#define BBIAS 			        0x27 /* B channel signal output bias (effective only when COM6[3]=1) */
#define GbBIAS                  0x28 /* Gb channel signal output bias (effective only when COM6[3]=1) */
#define RSVD_29                 0x29 /* reserved */
#define EXHCH                   0x2A /* Dummy Pixel Insert MSB */
#define EXHCL                   0x2B /* Dummy Pixel Insert LSB */
#define RBIAS                   0x2C /* R channel signal output bias (effective only when COM6[3]=1) */
#define ADVFL                   0x2D /* LSB of Insert Dummy Rows in Vertical Sync (1 bit equals 1 row)  */
#define ADVFH                   0x2E /* MSB of Insert Dummy Rows in Vertical Sync */
#define YAVE                    0x2F /* Y/G Channel Average Value */
#define HSYST                   0x30 /* HSync rising edge delay */
#define HSYEN                   0x31 /* HSync falling edge delay  */
#define HREF                    0x32 /* Image Start and Size Control DIFFERENT CONTROL SEQUENCE	 */
#define CHLF                    0x33 /* Array Current control  */
#define ARBLM                   0x34 /* Array reference control */
#define RSVD_35                 0x35 /* Reserved */
#define RSVD_36                 0x36 /* Reserved */
#define ADC                     0x37 /* ADC control */
#define ACOM                    0x38 /* ADC and analog common mode control */
#define OFON                    0x39 /* ADC offset control */
#define TSLB                    0x3A /* Line buffer test option  */

#define COM11                   0x3B /* Common control 11 */
#define   COM11_EXP		        0x02
#define   COM11_HZAUTO		    0x10 /* Auto detect 50/60 Hz */

#define COM12                   0x3C /* Common control 12 */

#define COM13                   0x3D /* Common control 13 */
#define   COM13_GAMMA	        0x80 /* Gamma enable */
#define	  COM13_UVSAT	        0x40 /* UV saturation auto adjustment */

#define COM14                   0x3E /* Common Control 14 */

#define EDGE                    0x3F /* edge enhancement adjustment */
#define COM15                   0x40 /* Common Control 15 DIFFERENT CONTROLS */
#define COM15_SET_RGB565(r,x)	((r&0xEF)|((x&1)<<4)) /* set rgb565 mode */
#define   COM15_RGB565	        0x10 /* RGB565 output */
#define   COM15_R00FF           0xC0 /* Output range: [00] to [FF] */

#define COM16                   0x41 /* Common Control 16 DIFFERENT CONTROLS */
#define COM16_AWBGAIN		    0x08 /* AWB gain enable */
#define COM17                   0x42 /* Common Control 17   */

#define AWBC1                   0x43 /* Reserved */
#define AWBC2                  	0x44 /* Reserved */
#define AWBC3                  	0x45 /* Reserved */
#define AWBC4                   0x46 /* Reserved */
#define AWBC5                  	0x47 /* Reserved */
#define AWBC6                  	0x48 /* Reserved */

#define RSVD_49			        0x49 /* Reserved */
#define RSVD_4A			        0x4A /* Reserved */

#define REG4B                   0x4B /* Register 4B */
#define DNSTH                   0x4C /* Denoise strength */

#define RSVD_4D			        0x4D /* Reserved */
#define RSVD_4E			        0x4E /* Reserved */

#define MTX1                    0x4F /* Matrix coefficient 1 */
#define MTX2                    0x50 /* Matrix coefficient 2 */
#define MTX3                    0x51 /* Matrix coefficient 3 */
#define MTX4                    0x52 /* Matrix coefficient 4 */
#define MTX5                    0x53 /* Matrix coefficient 5 */
#define MTX6                    0x54 /* Matrix coefficient 6 */
#define BRIGHTNESS              0x55 /* Brightness control */
#define CONTRAST		        0x56 /* Contrast control */
#define CONTRASCENTER           0x57 /* Contrast center */
#define MTXS			        0x58 /* Matrix coefficient sign for coefficient 5 to 0*/

#define RSVD_59			        0x59 /* Reserved */
#define RSVD_5A			        0x5A /* Reserved */
#define RSVD_5B			        0x5B /* Reserved */
#define RSVD_5C			        0x5C /* Reserved */
#define RSVD_5D			        0x5D /* Reserved */
#define RSVD_5E			        0x5E /* Reserved */
#define RSVD_5F			        0x5F /* Reserved */
#define RSVD_60			        0x60 /* Reserved */
#define RSVD_61			        0x61 /* Reserved */

#define LCC1                    0x62 /* Lens correction option 1  */

#define LCC2                    0x63 /* Lens correction option 2 */
#define LCC3 			        0x64 /* Lens correction option 3 */
#define LCC4			        0x65 /* Lens correction option 4 */
#define LCC5			        0x66 /* Lens correction option 5 */

#define MANU       		        0x67 /* Manual U Value      */
#define MANV      		        0x68 /* Manual V Value */
#define GFIX                    0x69 /* Fix gain control */
#define GGAIN                   0x6A /* G channel AWB gain */

#define DBLV               	    0x6B /* PLL and clock ? */

#define AWBCTR3               	0x6C /* AWB Control 3  */
#define AWBCTR2	                0x6D /* AWB Control 2  */
#define AWBCTR1                 0x6E /* AWB Control 1  */
#define AWBCTR0                 0x6F /* AWB Control 0  */
#define SCALING_XSC             0x70 /* test pattern and horizontal scaling factor */
#define SCALING_XSC_CBAR(r)	(r&0x7F) /* make sure bit7 is 0 for color bar */
#define SCALING_YSC             0x71 /* test pattern and vertical scaling factor */
#define SCALING_YSC_CBAR(r,x)	((r&0x7F)|((x&1)<<7)) /* change bit7 for color bar on/off */
#define SCALING_DCWCTR          0x72 /* DCW control */
#define SCALING_PCLK_DIV        0x73 /*  */
#define REG74                   0x74 /*  */
#define REG75                   0x75 /*  */
#define REG76                   0x76 /*  */
#define REG77	             	0x77 /*  */

#define RSVD_78      		    0x78 /* Reserved */
#define RSVD_79              	0x79 /* Reserved */

#define SLOP	                0x7A /* Gamma curve highest segment slope */
#define GAM1	                0x7B /* Gamma Curve 1st Segment Input End Point 0x04 Output Value */
#define GAM2	                0x7C /* Gamma Curve 2nd Segment Input End Point 0x08 Output Value */
#define GAM3                    0x7D /* Gamma Curve 3rd Segment Input End Point 0x10 Output Value */
#define GAM4                    0x7E /* Gamma Curve 4th Segment Input End Point 0x20 Output Value */
#define GAM5                    0x7F /* Gamma Curve 5th Segment Input End Point 0x28 Output Value */
#define GAM6                    0x80 /* Gamma Curve 6rd Segment Input End Point 0x30 Output Value */
#define GAM7                    0x81 /* Gamma Curve 7th Segment Input End Point 0x38 Output Value */
#define GAM8                    0x82 /* Gamma Curve 8th Segment Input End Point 0x40 Output Value */
#define GAM9                    0x83 /* Gamma Curve 9th Segment Input End Point 0x48 Output Value */
#define GAM10                   0x84 /* Gamma Curve 10th Segment Input End Point 0x50 Output Value */
#define GAM11                   0x85 /* Gamma Curve 11th Segment Input End Point 0x60 Output Value */
#define GAM12                   0x86 /* Gamma Curve 12th Segment Input End Point 0x70 Output Value */
#define GAM13                   0x87 /* Gamma Curve 13th Segment Input End Point 0x90 Output Value */
#define GAM14                   0x88 /* Gamma Curve 14th Segment Input End Point 0xB0 Output Value */
#define GAM15                   0x89 /* Gamma Curve 15th Segment Input End Point 0xD0 Output Value */

#define RSVD_8A      		    0x8A /* Reserved */
#define RSVD_8B              	0x8B /* Reserved */

#define RGB444                  0x8C /*  */

#define RSVD_8D      		    0x8D /* Reserved */
#define RSVD_8E              	0x8E /* Reserved */
#define RSVD_8F      		    0x8F /* Reserved */
#define RSVD_90              	0x90 /* Reserved */
#define RSVD_91      		    0x91 /* Reserved */

#define DM_LNL                  0x92 /* Dummy line low 8 bit */
#define DM_LNH                  0x93 /* Dummy line high 8 bit */
#define LCC6                    0x94 /* Lens correction option 6 */
#define LCC7                    0x95 /* Lens correction option 7 */

#define RSVD_96      		    0x96 /* Reserved */
#define RSVD_97              	0x97 /* Reserved */
#define RSVD_98      		    0x98 /* Reserved */
#define RSVD_99              	0x99 /* Reserved */
#define RSVD_9A      		    0x9A /* Reserved */
#define RSVD_9B              	0x9B /* Reserved */
#define RSVD_9C      		    0x9C /* Reserved */

#define BD50ST			        0x9D /* 50 Hz banding filter value */
#define BD60ST                  0x9E /* 60 Hz banding filter value */
#define HAECC1                  0x9F /* Histogram-based AEC/AGC control 1 */
#define HAECC2                  0xA0 /* Histogram-based AEC/AGC control 2 */

#define RSVD_A1      		    0xA1 /* Reserved */

#define SCALING_PCLK_DELAY      0xA2 /* Pixel clock delay */

#define RSVD_A3      		    0xA3 /* Reserved */

#define NT_CNTRL                0xA4 /*  */
#define BD50MAX			        0xA5 /* 50 Hz banding step limit */
#define HAECC3                  0xA6 /* Histogram-based AEC/AGC control 3  */
#define HAECC4 	   	            0xA7 /* Histogram-based AEC/AGC control 4           */
#define HAECC5		            0xA8 /* Histogram-based AEC/AGC control 5         */
#define HAECC6		            0xA9 /* Histogram-based AEC/AGC control 6           */

#define HAECC7		            0xAA /* Histogram-based AEC/AGC control 7           */
#define 	HAECC_EN      	    0x80 /* Histogram-based AEC algorithm enable      */

#define BD60MAX                 0xAB /* 60 Hz banding step limit */

#define STR_OPT                 0xAC /* Register AC */
#define STR_R			        0xAD /* R gain for led output frame */
#define STR_G			        0xAE /* G gain for led output frame */
#define STR_B			        0xAF /* B gain for led output frame */
#define RSVD_B0      		    0xB0 /* Reserved */
#define ABLC1			        0xB1 /* */
#define RSVD_B2      		    0xB2 /* Reserved */
#define THL_ST			        0xB3 /* ABLC target */
#define THL_DLT			        0xB5 /* ABLC stable range */

#define RSVD_B6      		    0xB6 /* Reserved */
#define RSVD_B7      		    0xB7 /* Reserved */
#define RSVD_B8      		    0xB8 /* Reserved */
#define RSVD_B9      		    0xB9 /* Reserved */
#define RSVD_BA      		    0xBA /* Reserved */
#define RSVD_BB      		    0xBB /* Reserved */
#define RSVD_BC      		    0xBC /* Reserved */
#define RSVD_BD      	    	0xBD /* Reserved */

#define AD_CHB			        0xBE /* blue channel black level compensation */
#define AD_CHR			        0xBF /* Red channel black level compensation */
#define AD_CHGb			        0xC0 /* Gb channel black level compensation */
#define AD_CHGr			        0xC1 /* Gr channel black level compensation */

#define RSVD_C2      		    0xC2 /* Reserved */
#define RSVD_C3      		    0xC3 /* Reserved */
#define RSVD_C4      		    0xC4 /* Reserved */
#define RSVD_C5      		    0xC5 /* Reserved */
#define RSVD_C6      		    0xC6 /* Reserved */
#define RSVD_C7      		    0xC7 /* Reserved */
#define RSVD_C8      		    0xC8 /* Reserved */

#define SATCTR			        0xC9 /* Saturation control */
#define SET_REG(reg, x)         (##reg_DEFAULT|x)

#endif //__OV7670_REG_REGS_H__
