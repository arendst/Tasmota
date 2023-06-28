/*
 * This file is part of the TREZOR project, https://trezor.io/
 *
 * Copyright (C) 2014 Pavol Rusnak <stick@satoshilabs.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include <stdint.h>
#include <stdbool.h>
//#include "trezor.h"

#define MSG_IN_SIZE (12*1024)

#define MSG_OUT_SIZE (12*1024)

#define msg_read(buf, len) msg_read_common('n', (buf), (len))
#define msg_write(id, ptr) msg_write_common('n', (id), (ptr))
const uint8_t *msg_out_data(void);

#if DEBUG_LINK

#define MSG_DEBUG_OUT_SIZE (4*1024)

#define msg_debug_read(buf, len) msg_read_common('d', (buf), (len))
#define msg_debug_write(id, ptr) msg_write_common('d', (id), (ptr))
const uint8_t *msg_debug_out_data(void);

#endif

void msg_read_common(char type, const uint8_t *buf, int len);
bool msg_write_common(char type, uint16_t msg_id, const void *msg_ptr);

void msg_read_tiny(const uint8_t *buf, int len);
void msg_debug_read_tiny(const uint8_t *buf, int len);
extern uint8_t msg_tiny[64];
extern uint16_t msg_tiny_id;

#endif
