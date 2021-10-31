#ifndef _IT8951_DEFINES_H_
#define _IT8951_DEFINES_H_

/*-----------------------------------------------------------------------
IT8951 Command defines
------------------------------------------------------------------------*/

//Built in I80 Command Code
#define IT8951_TCON_SYS_RUN         0x0001
#define IT8951_TCON_STANDBY         0x0002
#define IT8951_TCON_SLEEP           0x0003
#define IT8951_TCON_REG_RD          0x0010
#define IT8951_TCON_REG_WR          0x0011

#define IT8951_TCON_MEM_BST_RD_T    0x0012
#define IT8951_TCON_MEM_BST_RD_S    0x0013
#define IT8951_TCON_MEM_BST_WR      0x0014
#define IT8951_TCON_MEM_BST_END     0x0015

#define IT8951_TCON_LD_IMG          0x0020
#define IT8951_TCON_LD_IMG_AREA     0x0021
#define IT8951_TCON_LD_IMG_END      0x0022

//I80 User defined command code
#define IT8951_I80_CMD_DPY_AREA     0x0034
#define IT8951_I80_CMD_GET_DEV_INFO 0x0302
#define IT8951_I80_CMD_DPY_BUF_AREA 0x0037
#define IT8951_I80_CMD_VCOM         0x0039

/*-----------------------------------------------------------------------
 IT8951 Mode defines
------------------------------------------------------------------------*/

//Rotate mode
#define IT8951_ROTATE_0         0
#define IT8951_ROTATE_90        1
#define IT8951_ROTATE_180       2
#define IT8951_ROTATE_270       3

//Pixel mode (Bit per Pixel)
#define IT8951_2BPP             0
#define IT8951_3BPP             1
#define IT8951_4BPP             2
#define IT8951_8BPP             3

//Endian Type
#define IT8951_LDIMG_L_ENDIAN   0
#define IT8951_LDIMG_B_ENDIAN   1

/*-----------------------------------------------------------------------
IT8951 Registers defines
------------------------------------------------------------------------*/
//Register Base Address
#define IT8951_DISPLAY_REG_BASE     0x1000 //Register RW access

//Base Address of Basic LUT Registers
#define IT8951_LUT0EWHR    (IT8951_DISPLAY_REG_BASE + 0x00) //LUT0 Engine Width Height Reg
#define IT8951_LUT0XYR     (IT8951_DISPLAY_REG_BASE + 0x40) //LUT0 XY Reg
#define IT8951_LUT0BADDR   (IT8951_DISPLAY_REG_BASE + 0x80) //LUT0 Base Address Reg
#define IT8951_LUT0MFN     (IT8951_DISPLAY_REG_BASE + 0xC0) //LUT0 Mode and Frame number Reg
#define IT8951_LUT01AF     (IT8951_DISPLAY_REG_BASE + 0x114) //LUT0 and LUT1 Active Flag Reg

//Update Parameter Setting Register
#define IT8951_UP0SR       (IT8951_DISPLAY_REG_BASE + 0x134) //Update Parameter0 Setting Reg
#define IT8951_UP1SR       (IT8951_DISPLAY_REG_BASE + 0x138) //Update Parameter1 Setting Reg
#define IT8951_LUT0ABFRV   (IT8951_DISPLAY_REG_BASE + 0x13C) //LUT0 Alpha blend and Fill rectangle Value
#define IT8951_UPBBADDR    (IT8951_DISPLAY_REG_BASE + 0x17C) //Update Buffer Base Address
#define IT8951_LUT0IMXY    (IT8951_DISPLAY_REG_BASE + 0x180) //LUT0 Image buffer X/Y offset Reg
#define IT8951_LUTAFSR     (IT8951_DISPLAY_REG_BASE + 0x224) //LUT Status Reg (status of All LUT Engines)
#define IT8951_BGVR        (IT8951_DISPLAY_REG_BASE + 0x250) //Bitmap (1bpp) image color table

//System Registers
#define IT8951_SYS_REG_BASE         0x0000

//Address of System Registers
#define IT8951_I80CPCR              (IT8951_SYS_REG_BASE + 0x04)

//Memory Converter Registers
#define IT8951_MCSR_BASE_ADDR       0x0200
#define IT8951_MCSR                 (IT8951_MCSR_BASE_ADDR + 0x0000)
#define IT8951_LISAR                (IT8951_MCSR_BASE_ADDR + 0x0008)

#endif
