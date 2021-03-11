/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2020 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS products only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <esp_hap_pair_common.h>

void hap_tlv_data_init(hap_tlv_data_t *tlv_data, uint8_t *buf, int buf_size)
{
	tlv_data->bufptr = buf;
	tlv_data->bufsize = buf_size;
	tlv_data->curlen = 0;
}

int get_tlv_length(uint8_t *buf, int buflen, uint8_t type)
{
	if (!buf )
		return -1;
	int curlen = 0;
	int val_len = 0;
	bool found = false;
	while (buflen > 0) {
		if (buf[curlen] == type) {
			uint8_t len = buf[curlen + 1];
			if ((buflen - len) < 2)
				return -1;
			val_len += len;
			if (len < 255)
				return val_len;
			else
				found = true;

		} else if (found)
			return val_len;

		/* buf[curlen +1] will give the Length */
		buflen -= (2 + buf[curlen + 1]);
		curlen += (2 + buf[curlen + 1]);
	}
	return -1;
}
int get_value_from_tlv(uint8_t *buf, int buflen, uint8_t type, void *val, int val_size)
{
	if (!buf || !val)
		return -1;
	int curlen = 0;
	int val_len = 0;
	bool found = false;
	while (buflen > 0) {
		if (buf[curlen] == type) {
			uint8_t len = buf[curlen + 1];
			if ((val_size < len) || ((buflen - len) < 2))
				return -1;
			memcpy(val + val_len, &buf[curlen + 2], len);
			val_len += len;
			val_size -= len;
			if (len < 255)
				return val_len;
			else
				found = true;

		} else if (found)
			return val_len;

		/* buf[curlen +1] will give the Length */
		buflen -= (2 + buf[curlen + 1]);
		curlen += (2 + buf[curlen + 1]);
	}
	return -1;
}

int add_tlv(hap_tlv_data_t *tlv_data, uint8_t type, int len, void *val)
{
	if(!tlv_data->bufptr || ((len + 2) > (tlv_data->bufsize - tlv_data->curlen)))
		return -1;
	uint8_t *buf_ptr = (uint8_t *)val;
	int orig_len = tlv_data->curlen;
	do {
		tlv_data->bufptr[tlv_data->curlen++] = type;
		int tmp_len;
		if (len > 255)
			tmp_len = 255;
		else
			tmp_len = len;
		tlv_data->bufptr[tlv_data->curlen++] = tmp_len;
		memcpy(&tlv_data->bufptr[tlv_data->curlen], buf_ptr, tmp_len);
		tlv_data->curlen += tmp_len;
		buf_ptr += tmp_len;
		len -= tmp_len;
	} while (len);
	return tlv_data->curlen - orig_len;
}
void hap_prepare_error_tlv(uint8_t state, uint8_t error, void *buf, int bufsize, int *outlen)
{
	hap_tlv_data_t tlv_data;
	tlv_data.bufptr = buf;
	tlv_data.bufsize = bufsize;
	tlv_data.curlen = 0;
	/* Not doing any error handling because the size required for "state" and "error" will
	 * be too small to cause any error, and we anyways dont have any specific action to
	 * do in case if error in add_tlv()
	 */
	add_tlv(&tlv_data, kTLVType_State, sizeof(state), &state);
	add_tlv(&tlv_data, kTLVType_Error, sizeof(error), &error);
	*outlen = tlv_data.curlen;
}
