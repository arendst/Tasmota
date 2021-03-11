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
#ifndef _HAP_PAIR_COMMON_H_
#define _HAP_PAIR_COMMON_H_

#include <stdint.h>
#include <esp_hap_controllers.h>
#define ENCRYPT_KEY_LEN		32
#define POLY_AUTHTAG_LEN	16
#define CURVE_KEY_LEN		32
#define ED_SIGN_LEN		64
#define NONCE_LEN		8

#define STATE_M0		0
#define STATE_M1		1
#define STATE_M2		2
#define STATE_M3		3
#define STATE_M4		4
#define STATE_M5		5
#define STATE_M6		6
#define STATE_VERIFIED		0x55
#define STATE_INVALID		0xaa

typedef enum {
	HAP_METHOD_RESERVED = 0,
	HAP_METHOD_PAIR_SETUP = 1,
	HAP_METHOD_PAIR_VERIFY = 2,
	HAP_METHOD_ADD_PAIRING = 3,
	HAP_METHOD_REMOVE_PAIRING = 4,
	HAP_METHOD_LIST_PAIRINGS = 5,
} hap_pairing_methods_t;


typedef enum {
	kTLVError_Unknown = 0x01,
	kTLVError_Authentication = 0x02,
	kTLVError_Backoff = 0x03,
	kTLVError_MaxPeers = 0x04,
	kTLVError_MaxTries = 0x05,
	kTLVError_Unavailable = 0x06,
	kTLVError_Busy = 0x07,
} hap_tlv_error_t;

typedef enum {
	kTLVType_Method = 0x00,
	kTLVType_Identifier = 0x01,
	kTLVType_Salt = 0x02,
	kTLVType_PublicKey = 0x03,
	kTLVType_Proof = 0x04,
	kTLVType_EncryptedData = 0x05,
	kTLVType_State = 0x06,
	kTLVType_Error = 0x07,
	kTLVType_RetryDelay = 0x08,
	kTLVType_Certificate = 0x09,
	kTLVType_Signature = 0x0a,
	kTLVType_Permissions = 0x0b,
	kTLVType_FragmentedData = 0x0c,
	kTLVType_FragmentLast = 0x0d,
    kTLVType_Flags = 0x13,
    kTLVType_OwnershipProofToken = 0x1A,
    kTLVType_ProductData = 0x1C,
	kTLVType_Separator = 0xff,
} hap_tlv_type_t;

typedef struct {
	uint8_t *bufptr;
	int bufsize;
	int curlen;
} hap_tlv_data_t;

typedef struct {
	uint8_t state;
	uint8_t encrypt_key[ENCRYPT_KEY_LEN];
	uint8_t decrypt_key[ENCRYPT_KEY_LEN];
	uint8_t encrypt_nonce[NONCE_LEN];
	uint8_t decrypt_nonce[NONCE_LEN];
	hap_ctrl_data_t *ctrl;
    uint64_t pid;
    int64_t ttl;
    int64_t prepare_time;
	/* TODO: As of now, this identifier will be the socket
	 * number, since only http is supported.
	 * Need to make this generic later.
	 */
	int conn_identifier;
} hap_secure_session_t;

void hap_tlv_data_init(hap_tlv_data_t *tlv_data, uint8_t *buf, int buf_size);
int get_value_from_tlv(uint8_t *buf, int buf_len, uint8_t type, void *val, int val_size);
int get_tlv_length(uint8_t *buf, int buflen, uint8_t type);
int add_tlv(hap_tlv_data_t *tlv_data, uint8_t type, int len, void *val);
void hap_prepare_error_tlv(uint8_t state, uint8_t error, void *buf, int buf_size, int *out_len);
#endif /* _HAP_PAIR_COMMON_H_ */
