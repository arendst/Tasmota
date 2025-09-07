//
// ESP32-S3 SIMD optimized code
// Written by Larry Bank
// Copyright (c) 2024 BitBank Software, Inc.
// Project started Jan 21, 2024
//
#ifdef ARDUINO_ESP32S3_DEV

#include "dsps_fft2r_platform.h"
#if (dsps_fft2r_sc16_aes3_enabled == 1)
	.text
	.align 4
//
// Inverse DCT dequantization for JPEG decompression
//                        A2               A3
// Call as void s3_dequant(int16_t *pMCU, int16_t *pQuant);
	.global s3_dequant
    .type   s3_dequant,@function

s3_dequant:
	# no idea what this frequency keyword does
#	.frequency 1.000 0.000
	entry	a1,16
  ee.vld.128.ip	q0,a2,16   # load MCU rows 0-3 into Q0,Q1,Q2,Q3
  ee.vld.128.ip q4,a3,16   # load quantization values into Q4,Q5,Q6,Q7
  ee.vld.128.ip q1,a2,16
  ee.vld.128.ip q5,a3,16
  ee.vld.128.ip q2,a2,16
  ee.vld.128.ip q6,a3,16
  ee.vld.128.ip q3,a2,16
  ee.vld.128.ip q7,a3,16
  movi.n  a4,0           # load the shift register with 0
  wsr.sar  a2             # put it in the SAR (shift amount register)
  ee.vmul.s16 q0,q0,q4   # de-quantize each row
  ee.vmul.s16 q1,q1,q5
  ee.vmul.s16 q2,q2,q6
  ee.vmul.s16 q3,q3,q7
  addi.n a2,a2,64        # point to first row of MCUs to store dequantized values
  ee.vst.128.ip q0,a2,16  # write back dequantized rows 0-3
  ee.vst.128.ip q1,a2,16
  ee.vst.128.ip q2,a2,16
  ee.vst.128.ip q3,a2,16
// repeat for rows 4-7
  ee.vld.128.ip   q0,a2,16   # load MCU rows 4-7 into Q0,Q1,Q2,Q3
  ee.vld.128.ip   q4,a3,16   # load quantization values into Q4,Q5,Q6,Q7
  ee.vld.128.ip   q1,a2,16
  ee.vld.128.ip   q5,a3,16
  ee.vld.128.ip   q2,a2,16
  ee.vld.128.ip   q6,a3,16
  ee.vld.128.ip   q3,a2,16
  ee.vld.128.ip   q7,a3,16

  ee.vmul.s16 q0,q0,q4   # de-quantize rows 4-7
  ee.vmul.s16 q1,q1,q5
  ee.vmul.s16 q2,q2,q6
  ee.vmul.s16 q3,q3,q7
  addi.n a2,a2,64        # point to 4th row of MCUs
  ee.vst.128.ip q0,a2,16  # write back dequantized rows 4-7
  ee.vst.128.ip q1,a2,16
  ee.vst.128.ip q2,a2,16
  ee.vst.128.ip q3,a2,16
  retw.n            # done
#endif // dsps_fft2r_sc16_aes3_enabled
#endif // ESP32
