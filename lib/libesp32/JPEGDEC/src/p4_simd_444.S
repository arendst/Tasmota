//
// ESP32-S3 SIMD optimized code
// Written by Larry Bank
// Copyright (c) 2024 BitBank Software, Inc.
// Project started Jan 21, 2024
//
// RISC-V register naming
// Registers X0-X31:
// zero = X0
// RA = X1 return address
// SP = X2 stack pointer
// GP = X3 global pointer
// TP = X4 thread pointer
// T0-T2 = X5-X7 general purpose, not preserved
// S0-S1 = X8-X9 callee saved register 0 / frame pointer must be preserved across fn calls
// A0-A7 = X10-X17 parameters and return value (A0)
// S2-S11 = X18-X27
// T3-T6 = X28-X31
#if defined( ARDUINO_ESP32P4_DEV ) || defined ( ARDUINO_ESP32S31_DEV )
	.text
	.align 4

// Convert 8 pixels of YCbCr 4:4:4 to RGB565
//                                       A0            A1            A2           A3               A4          A5
// Call as void s3_ycbcr_convert_444(uint8_t *pY, uint8_t *pCB, uint8_t *pCR, uint16_t *pOut, int16_t *pConsts, uint8_t ucPixelType);
// supported pixel types: 0 = RGB656_LE, 1 = RGB565_BE, 2 = RGBA8888
	.global s3_ycbcr_convert_444
    .type   s3_ycbcr_convert_444,@function

s3_ycbcr_convert_444:
  li t2,2                    # constant to compare output pixel type
  esp.vldbc.16.ip  q3,a4,2   # get constant 0x80 as 16-bits in all 128 bits of q3
  esp.vld.l.64.ip	q0,a0,0   # load 8 Y values into Q0
  esp.vld.l.64.ip	q1,a1,0   # load 8 Cb values into Q1
  esp.vld.l.64.ip	q2,a2,0   # load 8 Cr values into Q2
  esp.vext.u8 q0,q4,q0        # expand 8-bit Y data to 16-bits
  esp.vext.u8 q1,q4,q1        # expand 8-bit Cb data to 16-bits
  esp.vext.u8 q2,q4,q2        # expand 8-bit Cr data to 16-bits

  esp.vsub.s16 q1,q1,q3      # subtract 0x0080 from Cb's
  esp.vsub.s16 q2,q2,q3      # subtract 0x0080 from Cr's
  esp.vldbc.16.ip  q3,a4,2   # get constant 1.77200 as 16-bits in all 128 bits of q3
  li t3,6                    # load the shift register with 6
  esp.movx.w.sar t3          # put it in the SAR (shift amount register)
  esp.vmul.s16 q6,q1,q3      # (Cb *= 1.77200) >> 6
  esp.vadd.s16 q6,q6,q0      # Cb += y (8 blue pixels in q6)
  esp.vldbc.16.ip  q3,a4,2   # get constant 1.402 as 16-bits in all 128 bits of q3
  esp.vmul.s16 q7,q2,q3      # (Cr *= 1.402) >> 6
  esp.vadd.s16 q7,q7,q0  # Cr += y (8 red pixels in q7)
  esp.vldbc.16.ip  q3,a4,2   # get constant 0.34414 as 16-bits in all 128 bits of q3
  li t3,0                    # load the shift register with 0
  esp.movx.w.sar t3          # put it in the SAR (shift amount register)
  esp.vmul.s16 q4,q1,q3   # (Cb * 0.34414) >> 0
  esp.vldbc.16.ip  q3,a4,2   # get constant 0.71414 as 16-bits in all 128 bits of q3
  esp.vmul.s16 q3,q2,q3   # (Cr * 0.71414) >> 0
  esp.vadd.s16 q3,q3,q4  # (Cb * 0.34414) + (Cr * 0.71414)
  esp.vldbc.16.ip  q4,a4,2   # get constant 1 (so we can do a 16-bit shift)
  li t3,6                    # load the shift register with 6
  esp.movx.w.sar t3          # put it in the SAR (shift amount register)
  esp.vmul.s16  q3,q3,q4  # shift right by 6
  esp.vsub.s16 q3,q0,q3   # Y - ((Cb * 0.34414) + (Cr * 0.71414)) = green in Q3
// saturate to 8 bits
  esp.xorq q0,q0,q0
  esp.vmax.s16 q3,q3,q0
  esp.vmax.s16 q6,q6,q0
  esp.vmax.s16 q7,q7,q0
  esp.vcmp.eq.s16 q1,q1,q1	# create 255
  esp.vzip.8 q1,q0
  esp.vmin.s16 q3,q3,q1         # clamp to 255
  esp.vmin.s16 q6,q6,q1
  esp.vmin.s16 q7,q7,q1
// Now we have RGB888, is that the output pixel type?
  beq a5,t2,.rgb8888_output
// either RGB565 LE or BE from here
  li t3,3                 # load the shift register with 3 (for blue and red)
  esp.movx.w.sar t3       # put it in the SAR (shift amount register)
  esp.vmul.u16 q6,q4,q6   # shift blue right by 3
  esp.vmul.u16 q7,q4,q7   # shift red right by 3
  li t3,2                 # load the shift register with 2 (for green)
  esp.movx.w.sar t3       # put it in the SAR (shift amount register)
  esp.vmul.u16 q3,q4,q3   # shift green right by 2
// now combine to form RGB565 pixels
  li t3,0
  esp.movx.w.sar t3       # no shift after multiply
  esp.vldbc.16.ip  q4,a4,2   # get constant value 32 (to shift green left by 5 bits)
  esp.vldbc.16.ip  q5,a4,2   # get constant value 2048 (to shift red left by 11 bits)
  esp.vmul.u16 q3,q4,q3   # shift green left by 5
  esp.vmul.u16 q7,q5,q7   # shift red left by 11
  esp.orq q6,q6,q3        # combine blue + green
  esp.orq q6,q6,q7        # combine blue + green + red
  esp.orq q5,q6,q6        # in case we're generating little endian output
  beq a5,x0,.rgb565_exit # RGB565 little endian?
  esp.vunzip.8 q6,q5      # swap the byte order to be big-endian
  esp.vzip.8 q5,q6
.rgb565_exit:
  esp.vst.128.ip q5,a3,0  # store the 8 RGB565 pixels
  ret
// Create RGBA (32-bit) pixels
.rgb8888_output:
  li t3,8       # shift 8 bits
  esp.movx.w.sar t3
  esp.vsl.32 q3,q3   # shift green over 8 bits
  esp.orq q7,q7,q3   # combine red and green
  esp.vcmp.eq.s16 q1,q1,q1  # create FFs
  esp.xorq q2,q2,q2  # create 00s
  esp.vzip.8 q2,q1   # create FF00 for Alpha
  esp.orq q2,q2,q6   # combine blue + alpha
  esp.vzip.16 q7,q2  # create RGB8888 pixels
  esp.vst.128.ip q7,a3,16  # store 8 x RGB8888 pixels = 32 bytes
  esp.vst.128.ip q2,a3,0
  ret              # done
#endif // ESP32-P4/S31
