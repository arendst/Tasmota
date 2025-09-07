//
// ESP32-S3 SIMD optimized code
// Written by Larry Bank
// Copyright (c) 2024 BitBank Software, Inc.
// Project started Jan 21, 2024
//
#if defined (ARDUINO_ARCH_ESP32) && !defined(NO_SIMD)
#if __has_include ("dsps_fft2r_platform.h")
#include "dsps_fft2r_platform.h"
#if (dsps_fft2r_sc16_aes3_enabled == 1)
	.text
	.align 4

// Convert 8 pixels of YCbCr 4:4:4 to RGB565
//                                       A2            A3            A4           A5               A6          A7
// Call as void s3_ycbcr_convert_444(uint8_t *pY, uint8_t *pCB, uint8_t *pCR, uint16_t *pOut, int16_t *pConsts, uint8_t ucPixelType);
// supported pixel types: 0 = RGB656_LE, 1 = RGB565_BE, 2 = RGBA8888
	.global s3_ycbcr_convert_444
    .type   s3_ycbcr_convert_444,@function

s3_ycbcr_convert_444:
	# no idea what this frequency keyword does
#	.frequency 1.000 0.000
	entry	a1,16
  ee.xorq q4,q4,q4          # load Q4 with 0's
  ee.vldbc.16.ip  q3,a6,2   # get constant 0x80 as 16-bits in all 128 bits of q3
  ee.vld.l.64.ip	q0,a2,0   # load 8 Y values into Q0
  ee.vld.l.64.ip	q1,a3,0   # load 8 Cb values into Q1
  ee.vld.l.64.ip	q2,a4,0   # load 8 Cr values into Q2
  ee.vzip.8 q0,q4           # expand 8-bit Y data to 16-bits
  ee.xorq q4,q4,q4        # need to reset to 0's
  ee.vzip.8 q1,q4           # expand 8-bit Cb data to 16-bits
  ee.xorq q4,q4,q4
  ee.vzip.8 q2,q4           # expand 8-bit Cr data to 16-bits

  ee.vsubs.s16 q1,q1,q3     # subtract 0x0080 from Cb's
  ee.vsubs.s16 q2,q2,q3     # subtract 0x0080 from Cr's
  ee.vldbc.16.ip  q3,a6,2   # get constant 1.77200 as 16-bits in all 128 bits of q3
 	movi.n	a2,6           # load the shift register with 6
	wsr.sar	a2             # put it in the SAR (shift amount register)
  ee.vmul.s16 q6,q1,q3   # (Cb *= 1.77200) >> 6
  ee.vadds.s16 q6,q6,q0  # Cb += y (8 blue pixels in q6)
  ee.vldbc.16.ip  q3,a6,2   # get constant 1.402 as 16-bits in all 128 bits of q3
  ee.vmul.s16 q7,q2,q3   # (Cr *= 1.402) >> 6
  ee.vadds.s16 q7,q7,q0  # Cr += y (8 red pixels in q7)
  ee.vldbc.16.ip  q3,a6,2   # get constant 0.34414 as 16-bits in all 128 bits of q3
 	movi.n	a2,0           # load the shift register with 0
	wsr.sar	a2             # put it in the SAR (shift amount register)
  ee.vmul.s16 q4,q1,q3   # (Cb * 0.34414) >> 0
  ee.vldbc.16.ip  q3,a6,2   # get constant 0.71414 as 16-bits in all 128 bits of q3
  ee.vmul.s16 q3,q2,q3   # (Cr * 0.71414) >> 0
  ee.vadds.s16 q3,q3,q4  # (Cb * 0.34414) + (Cr * 0.71414)
  ee.vldbc.16.ip  q4,a6,2   # get constant 1 (so we can do a 16-bit shift)
 	movi.n	a2,6           # load the shift register with 6
	wsr.sar	a2             # put it in the SAR (shift amount register)
  ee.vmul.s16  q3,q3,q4  # shift right by 6
  ee.vsubs.s16 q3,q0,q3   # Y - ((Cb * 0.34414) + (Cr * 0.71414)) = green in Q3
// saturate to 8 bits
  ee.xorq q0,q0,q0
  ee.vmax.s16 q3,q3,q0
  ee.vmax.s16 q6,q6,q0
  ee.vmax.s16 q7,q7,q0
  ee.vcmp.eq.s16 q1,q1,q1	# create 255
  ee.vzip.8 q1,q0
  ee.vmin.s16 q3,q3,q1         # clamp to 255
  ee.vmin.s16 q6,q6,q1
  ee.vmin.s16 q7,q7,q1
// Now we have RGB888, is that the output pixel type?
  beqi a7,2,.rgb8888_output
// either RGB565 LE or BE from here
 	movi.n	a2,3           # load the shift register with 3 (for blue and red)
	wsr.sar	a2             # put it in the SAR (shift amount register)
  ee.vmul.s16 q6,q4,q6   # shift blue right by 3
  ee.vmul.s16 q7,q4,q7   # shift red right by 3
 	movi.n	a2,2           # load the shift register with 2 (for green)
	wsr.sar	a2             # put it in the SAR (shift amount register)
  ee.vmul.s16 q3,q4,q3   # shift green right by 2
// now combine to form RGB565 pixels
 	movi.n	a2,0
	wsr.sar	a2              # no shift after multiply
  ee.vldbc.16.ip  q4,a6,2   # get constant value 32 (to shift green left by 5 bits)
  ee.vldbc.16.ip  q5,a6,2   # get constant value 2048 (to shift red left by 11 bits)
  ee.vmul.s16 q3,q4,q3   # shift green left by 5
  ee.vmul.s16 q7,q5,q7   # shift red left by 11
  ee.orq q6,q6,q3        # combine blue + green
  ee.orq q6,q6,q7        # combine blue + green + red
  mv.qr q5,q6            # in case we're generating little endian output
  beqi a7,0,.rgb565_exit # RGB565 little endian?
  ee.vunzip.8 q6,q5      # swap the byte order to be big-endian
  ee.vzip.8 q5,q6
.rgb565_exit:
  ee.vst.128.ip q5,a5,0  # store the 8 RGB565 pixels
  retw.n
// Create RGBA (32-bit) pixels
.rgb8888_output:
  movi.n a2,8       # shift 8 bits
  wsr.sar a2
  ee.vsl.32 q3,q3   # shift green over 8 bits
  ee.orq q7,q7,q3   # combine red and green
  ee.vcmp.eq.s16 q1,q1,q1  # create FFs
  ee.xorq q2,q2,q2  # create 00s
  ee.vzip.8 q2,q1   # create FF00 for Alpha
  ee.orq q2,q2,q6   # combine blue + alpha
  ee.vzip.16 q7,q2  # create RGB8888 pixels
  ee.vst.128.ip q7,a5,16  # store 8 x RGB8888 pixels = 32 bytes
  ee.vst.128.ip q2,a5,0
  retw.n            # done
#endif // dsps_fft2r_sc16_aes3_enabled
#endif // __has_include
#endif // ESP32
