/*
 * Copyright (c) 2013, Thingsquare, http://www.thingsquare.com/.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef _RF233_ARCH_H_
#define _RF233_ARCH_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
void wake_from_sleep(void);
void goto_sleep(void);
/*
void     rf233_arch_init(void);
void     rf233_arch_reset(void);
uint8_t  rf233_arch_status(void);

int      rf233_arch_spi_busy(void);
uint8_t  rf233_arch_read_reg(uint8_t reg);
int      rf233_arch_write_reg(uint8_t reg, uint8_t val);
int      rf233_arch_read_frame(uint8_t *buf, uint8_t len);
int      rf233_arch_write_frame(uint8_t *buf, uint8_t len);
int      rf233_arch_burstread_sram(uint8_t *buf, uint8_t offset, uint8_t len);
int      rf233_arch_burstwrite_sram(uint8_t *buf, uint8_t offset, uint8_t len);*/

#endif  /* _RF233_ARCH_H_ */
