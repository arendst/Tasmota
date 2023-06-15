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

#include <string.h>

//#include "trezor.h"
#include "messages.h"
#include "debug.h"
//#include "fsm.h"
#include "util.h"
#include "options.h"
#include "gettext.h"

#include "pb_decode.h"
#include "pb_encode.h"
#include "messages.pb.h"

struct MessagesMap_t {
	char type;	// n = normal, d = debug
	char dir; 	// i = in, o = out
	uint16_t msg_id;
	const pb_field_t *fields;
	void (*process_func)(void *ptr);
};


static const struct MessagesMap_t MessagesMap[] = {
//#include "messages_map.h"
	// end
	{0, 0, 0, 0, 0}
};


const pb_field_t *MessageFields(char type, char dir, uint16_t msg_id)
{
	const struct MessagesMap_t *m = MessagesMap;
	while (m->type) {
#if EMULATOR
		(void) type;
		if (dir == m->dir && msg_id == m->msg_id) {
#else
		if (type == m->type && dir == m->dir && msg_id == m->msg_id) {
#endif
			return m->fields;
		}
		m++;
	}
	return 0;
}

void MessageProcessFunc(char type, char dir, uint16_t msg_id, void *ptr)
{
	const struct MessagesMap_t *m = MessagesMap;
	while (m->type) {
#if EMULATOR
		(void) type;
		if (dir == m->dir && msg_id == m->msg_id) {
#else
		if (type == m->type && dir == m->dir && msg_id == m->msg_id) {
#endif
			m->process_func(ptr);
			return;
		}
		m++;
	}
}

static uint32_t msg_out_start = 0;
static uint32_t msg_out_end = 0;
static uint32_t msg_out_cur = 0;
static uint8_t msg_out[MSG_OUT_SIZE];

#if DEBUG_LINK

static uint32_t msg_debug_out_start = 0;
static uint32_t msg_debug_out_end = 0;
static uint32_t msg_debug_out_cur = 0;
static uint8_t msg_debug_out[MSG_DEBUG_OUT_SIZE];

#endif

static inline void msg_out_append(uint8_t c)
{
	if (msg_out_cur == 0) {
		msg_out[msg_out_end * 64] = '?';
		msg_out_cur = 1;
	}
	msg_out[msg_out_end * 64 + msg_out_cur] = c;
	msg_out_cur++;
	if (msg_out_cur == 64) {
		msg_out_cur = 0;
		msg_out_end = (msg_out_end + 1) % (MSG_OUT_SIZE / 64);
	}
}

#if DEBUG_LINK

static inline void msg_debug_out_append(uint8_t c)
{
	if (msg_debug_out_cur == 0) {
		msg_debug_out[msg_debug_out_end * 64] = '?';
		msg_debug_out_cur = 1;
	}
	msg_debug_out[msg_debug_out_end * 64 + msg_debug_out_cur] = c;
	msg_debug_out_cur++;
	if (msg_debug_out_cur == 64) {
		msg_debug_out_cur = 0;
		msg_debug_out_end = (msg_debug_out_end + 1) % (MSG_DEBUG_OUT_SIZE / 64);
	}
}

#endif

static inline void msg_out_pad(void)
{
	if (msg_out_cur == 0) return;
	while (msg_out_cur < 64) {
		msg_out[msg_out_end * 64 + msg_out_cur] = 0;
		msg_out_cur++;
	}
	msg_out_cur = 0;
	msg_out_end = (msg_out_end + 1) % (MSG_OUT_SIZE / 64);
}

#if DEBUG_LINK

static inline void msg_debug_out_pad(void)
{
	if (msg_debug_out_cur == 0) return;
	while (msg_debug_out_cur < 64) {
		msg_debug_out[msg_debug_out_end * 64 + msg_debug_out_cur] = 0;
		msg_debug_out_cur++;
	}
	msg_debug_out_cur = 0;
	msg_debug_out_end = (msg_debug_out_end + 1) % (MSG_DEBUG_OUT_SIZE / 64);
}

#endif

static bool pb_callback_out(pb_ostream_t *stream, const uint8_t *buf, size_t count)
{
	(void)stream;
	for (size_t i = 0; i < count; i++) {
		msg_out_append(buf[i]);
	}
	return true;
}

#if DEBUG_LINK

static bool pb_debug_callback_out(pb_ostream_t *stream, const uint8_t *buf, size_t count)
{
	(void)stream;
	for (size_t i = 0; i < count; i++) {
		msg_debug_out_append(buf[i]);
	}
	return true;
}

#endif

bool msg_write_common(char type, uint16_t msg_id, const void *msg_ptr)
{
	const pb_field_t *fields = MessageFields(type, 'o', msg_id);
	if (!fields) { // unknown message
		return false;
	}

	pb_ostream_t sizestream = {0, 0, SIZE_MAX, 0, 0};
	bool status = pb_encode(&sizestream, fields, msg_ptr);

	if (!status) {
		return false;
	}

	void (*append)(uint8_t);
	bool (*pb_callback)(pb_ostream_t *, const uint8_t *, size_t);

	if (type == 'n') {
		append = msg_out_append;
		pb_callback = pb_callback_out;
	} else
#if DEBUG_LINK
	if (type == 'd') {
		append = msg_debug_out_append;
		pb_callback = pb_debug_callback_out;
	} else
#endif
	{
		return false;
	}

	uint32_t len = sizestream.bytes_written;
	append('#');
	append('#');
	append((msg_id >> 8) & 0xFF);
	append(msg_id & 0xFF);
	append((len >> 24) & 0xFF);
	append((len >> 16) & 0xFF);
	append((len >> 8) & 0xFF);
	append(len & 0xFF);
	pb_ostream_t stream = {pb_callback, 0, SIZE_MAX, 0, 0};
	status = pb_encode(&stream, fields, msg_ptr);
	if (type == 'n') {
		msg_out_pad();
	}
#if DEBUG_LINK
	else if (type == 'd') {
		msg_debug_out_pad();
	}
#endif
	return status;
}

enum {
	READSTATE_IDLE,
	READSTATE_READING,
};

void msg_process(char type, uint16_t msg_id, const pb_field_t *fields, uint8_t *msg_raw, uint32_t msg_size)
{
	static CONFIDENTIAL uint8_t msg_data[MSG_IN_SIZE];
	memset(msg_data, 0, sizeof(msg_data));
	pb_istream_t stream = pb_istream_from_buffer(msg_raw, msg_size);
	bool status = pb_decode(&stream, fields, msg_data);
	if (status) {
		MessageProcessFunc(type, 'i', msg_id, msg_data);
	} else {
		//fsm_sendFailure(FailureType_Failure_DataError, stream.errmsg);
	}
}

void msg_read_common(char type, const uint8_t *buf, int len)
{
	static char read_state = READSTATE_IDLE;
	static CONFIDENTIAL uint8_t msg_in[MSG_IN_SIZE];
	static uint16_t msg_id = 0xFFFF;
	static uint32_t msg_size = 0;
	static uint32_t msg_pos = 0;
	static const pb_field_t *fields = 0;

	if (len != 64) return;

	if (read_state == READSTATE_IDLE) {
		if (buf[0] != '?' || buf[1] != '#' || buf[2] != '#') {	// invalid start - discard
			return;
		}
		msg_id = (buf[3] << 8) + buf[4];
		msg_size = (buf[5] << 24)+ (buf[6] << 16) + (buf[7] << 8) + buf[8];

		fields = MessageFields(type, 'i', msg_id);
		if (!fields) { // unknown message
			//fsm_sendFailure(FailureType_Failure_UnexpectedMessage, _("Unknown message"));
			return;
		}
		if (msg_size > MSG_IN_SIZE) { // message is too big :(
			//fsm_sendFailure(FailureType_Failure_DataError, _("Message too big"));
			return;
		}

		read_state = READSTATE_READING;

		memcpy(msg_in, buf + 9, len - 9);
		msg_pos = len - 9;
	} else
	if (read_state == READSTATE_READING) {
		if (buf[0] != '?') {	// invalid contents
			read_state = READSTATE_IDLE;
			return;
		}
		memcpy(msg_in + msg_pos, buf + 1, len - 1);
		msg_pos += len - 1;
	}

	if (msg_pos >= msg_size) {
		msg_process(type, msg_id, fields, msg_in, msg_size);
		msg_pos = 0;
		read_state = READSTATE_IDLE;
	}
}

const uint8_t *msg_out_data(void)
{
	if (msg_out_start == msg_out_end) return 0;
	uint8_t *data = msg_out + (msg_out_start * 64);
	msg_out_start = (msg_out_start + 1) % (MSG_OUT_SIZE / 64);
	debugLog(0, "", "msg_out_data");
	return data;
}

#if DEBUG_LINK

const uint8_t *msg_debug_out_data(void)
{
	if (msg_debug_out_start == msg_debug_out_end) return 0;
	uint8_t *data = msg_debug_out + (msg_debug_out_start * 64);
	msg_debug_out_start = (msg_debug_out_start + 1) % (MSG_DEBUG_OUT_SIZE / 64);
	debugLog(0, "", "msg_debug_out_data");
	return data;
}

#endif

CONFIDENTIAL uint8_t msg_tiny[64];
uint16_t msg_tiny_id = 0xFFFF;

void msg_read_tiny(const uint8_t *buf, int len)
{
	if (len != 64) return;
	if (buf[0] != '?' || buf[1] != '#' || buf[2] != '#') {
		return;
	}
	uint16_t msg_id = (buf[3] << 8) + buf[4];
	uint32_t msg_size = (buf[5] << 24) + (buf[6] << 16) + (buf[7] << 8) + buf[8];
	if (msg_size > 64 || len - msg_size < 9) {
		return;
	}

	const pb_field_t *fields = 0;
	// upstream nanopb is missing const qualifier, so we have to cast :-/
	pb_istream_t stream = pb_istream_from_buffer((uint8_t *)buf + 9, msg_size);

	switch (msg_id) {
		case MessageType_MessageType_PinMatrixAck:
			fields = PinMatrixAck_fields;
			break;
		case MessageType_MessageType_ButtonAck:
			fields = ButtonAck_fields;
			break;
		case MessageType_MessageType_PassphraseAck:
			fields = PassphraseAck_fields;
			break;
		case MessageType_MessageType_Cancel:
			fields = Cancel_fields;
			break;
		case MessageType_MessageType_Initialize:
			fields = Initialize_fields;
			break;
#if DEBUG_LINK
		case MessageType_MessageType_DebugLinkDecision:
			fields = DebugLinkDecision_fields;
			break;
		case MessageType_MessageType_DebugLinkGetState:
			fields = DebugLinkGetState_fields;
			break;
#endif
	}
	if (fields) {
		bool status = pb_decode(&stream, fields, msg_tiny);
		if (status) {
			msg_tiny_id = msg_id;
		} else {
			//fsm_sendFailure(FailureType_Failure_DataError, stream.errmsg);
			msg_tiny_id = 0xFFFF;
		}
	} else {
		//fsm_sendFailure(FailureType_Failure_UnexpectedMessage, _("Unknown message"));
		msg_tiny_id = 0xFFFF;
	}
}
