/*
 * Copyright (c) 2019, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SENSIRION_ARCH_CONFIG_H
#define SENSIRION_ARCH_CONFIG_H

/**
 * If your platform does not provide the library stdlib.h you have to remove the
 * include and define NULL yourself (see below).
 */
#include <stdlib.h>

/**
 * #ifndef NULL
 * #define NULL ((void *)0)
 * #endif
 */

/**
 * If your platform does not provide the library stdint.h you have to
 * define the integral types yourself (see below).
 */
#include <stdint.h>

/**
 * Typedef section for types commonly defined in <stdint.h>
 * If your system does not provide stdint headers, please define them
 * accordingly. Please make sure to define int64_t and uint64_t.
 */
/* typedef unsigned long long int uint64_t;
 * typedef long long int int64_t;
 * typedef long int32_t;
 * typedef unsigned long uint32_t;
 * typedef short int16_t;
 * typedef unsigned short uint16_t;
 * typedef char int8_t;
 * typedef unsigned char uint8_t; */

#ifndef __cplusplus

/**
 * If your platform doesn't define the bool type we define it as int. Depending
 * on your system update the definition below.
 */
#if __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else

#ifndef bool
#define bool int
#define true 1
#define false 0
#endif /* bool */

#endif /* __STDC_VERSION__ */

#endif /* __cplusplus */

/**
 * The clock period of the i2c bus in microseconds. Increase this, if your GPIO
 * ports cannot support a 200 kHz output rate. (2 * 1 / 10usec == 200Khz)
 *
 * This is only relevant for the sw-i2c HAL (bit-banging on GPIO pins). The
 * pulse length is half the clock period, the number should thus be even.
 */
#define SENSIRION_I2C_CLOCK_PERIOD_USEC 10

#endif /* SENSIRION_ARCH_CONFIG_H */
