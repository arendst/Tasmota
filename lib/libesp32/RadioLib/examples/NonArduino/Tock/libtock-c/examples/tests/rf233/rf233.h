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

#ifndef _RF233_H_
#define _RF233_H_

#include <stdint.h>
/*---------------------------------------------------------------------------*/

int rf233_init(uint16_t channel, uint16_t from_addr, uint16_t pan_id); 
int rf233_tx_data(uint16_t to_addr, void* payload, int payload_len); 
int rf233_rx_data(int (*callback)(void*, int, uint16_t, uint16_t, uint16_t)); 
// TODO assume callback passes buffer that is long enough? 

// TODO moved to .h 
int rf233_on(void);
int rf233_off(void);
int rf233_sleep(void);

int rf233_interrupt_poll(void);
int rf_get_channel(void);
int rf_set_channel(uint8_t ch);
int rf233_get_txp(void);
int rf233_set_txp(uint8_t txp);
uint8_t rf233_status(void);
void SetIEEEAddr(uint8_t *ieee_addr);
void SetPanId(uint16_t panId);
void SetShortAddr(uint16_t addr);
/*---------------------------------------------------------------------------*/
#endif  /* _RF233_H_ */
