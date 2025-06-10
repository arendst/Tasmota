/*
 * Copyright (c) 2021 Daniel Hope (www.floorsense.nz)
 * Copyright (c) 2024 Simone Rossetto <simros85@gmail.com>
 * Copyright (c) 2025 Stephan Hadinger
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *  list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this
 *  list of conditions and the following disclaimer in the documentation and/or
 *  other materials provided with the distribution.
 *
 * 3. Neither the name of "Floorsense Ltd", "Agile Workspace Ltd" nor the names of
 *  its contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Daniel Hope <daniel.hope@smartalock.com>
 */

#include <Arduino.h>
#include "wireguard.h"

#include "crypto.h"

// For HMAC calculation
#define WIREGUARD_BLAKE2S_BLOCK_SIZE (64)

//**************************************************************************************************************
// enable AddLog support within a C++ library
extern void AddLog(uint32_t loglevel, PGM_P formatP, ...);
enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE};
//**************************************************************************************************************

// 5.4 Messages
// Constants
static const uint8_t PROGMEM CONSTRUCTION[] = "Noise_IKpsk2_25519_ChaChaPoly_BLAKE2s"; // The UTF-8 string literal "Noise_IKpsk2_25519_ChaChaPoly_BLAKE2s", 37 bytes of output
static const uint8_t PROGMEM IDENTIFIER[] = "WireGuard v1 zx2c4 Jason@zx2c4.com"; // The UTF-8 string literal "WireGuard v1 zx2c4 Jason@zx2c4.com", 34 bytes of output
static const uint8_t PROGMEM LABEL_MAC1[] = "mac1----"; // Label-Mac1 The UTF-8 string literal "mac1----", 8 bytes of output.
static const uint8_t PROGMEM LABEL_COOKIE[] = "cookie--"; // Label-Cookie The UTF-8 string literal "cookie--", 8 bytes of output

static const uint8_t zero_key[WIREGUARD_PUBLIC_KEY_LEN] = { 0 };

// Calculated in wireguard_init
static uint8_t construction_hash[WIREGUARD_HASH_LEN];
static uint8_t identifier_hash[WIREGUARD_HASH_LEN];


void wireguard_init() {
	wireguard_blake2s_ctx ctx;
	// Pre-calculate chaining key hash
	wireguard_blake2s_init(&ctx, WIREGUARD_HASH_LEN, NULL, 0);
	wireguard_blake2s_update(&ctx, CONSTRUCTION, sizeof(CONSTRUCTION) - 1);		// remove terminating NULL char
	wireguard_blake2s_final(&ctx, construction_hash);
	// Pre-calculate initial handshake hash - uses construction_hash calculated above
	wireguard_blake2s_init(&ctx, WIREGUARD_HASH_LEN, NULL, 0);
	wireguard_blake2s_update(&ctx, construction_hash, sizeof(construction_hash));
	wireguard_blake2s_update(&ctx, IDENTIFIER, sizeof(IDENTIFIER) - 1);		// remove terminating NULL char
	wireguard_blake2s_final(&ctx, identifier_hash);
}

wireguard_peer_t *wireguard_peer_alloc(wireguard_device_t *device) {
	for (uint32_t x=0; x < WIREGUARD_MAX_PEERS; x++) {
		if (!device->peers[x].valid) {
			return &device->peers[x];
		}
	}
	return NULL;
}

wireguard_peer_t *wireguard_peer_lookup_by_pubkey(struct wireguard_device *device, uint8_t *public_key) {
	wireguard_peer_t *result = NULL;
	wireguard_peer_t *tmp;
	for (int32_t x=0; x < WIREGUARD_MAX_PEERS; x++) {
		if (device->peers[x].valid) {
			if (memcmp(device->peers[x].public_key, public_key, WIREGUARD_PUBLIC_KEY_LEN) == 0) {
				return &device->peers[x];
			}
		}
	}
	return NULL;
}

uint8_t wireguard_peer_index(struct wireguard_device *device, wireguard_peer_t *peer) {
	for (int32_t x=0; x < WIREGUARD_MAX_PEERS; x++) {
		if (peer == &device->peers[x]) {
			return x;
		}
	}
	return 0xFF;		// not found
}

wireguard_peer_t *wireguard_peer_lookup_by_peer_index(struct wireguard_device *device, uint8_t peer_index) {
	if (peer_index < WIREGUARD_MAX_PEERS) {
		if (device->peers[peer_index].valid) {
			return &device->peers[peer_index];
		}
	}
	return NULL;
}

wireguard_peer_t *wireguard_peer_lookup_by_receiver(struct wireguard_device *device, uint32_t receiver) {
	for (int32_t x=0; x < WIREGUARD_MAX_PEERS; x++) {
		wireguard_peer_t *cur_key = &device->peers[x];
		if (cur_key->valid) {
			if ((cur_key->curr_keypair.valid && (cur_key->curr_keypair.local_index == receiver)) ||
				(cur_key->next_keypair.valid && (cur_key->next_keypair.local_index == receiver)) ||
				(cur_key->prev_keypair.valid && (cur_key->prev_keypair.local_index == receiver))
				) {
				return cur_key;
			}
		}
	}
	return NULL;
}

wireguard_peer_t *wireguard_peer_lookup_by_handshake(struct wireguard_device *device, uint32_t receiver) {
	for (int32_t x=0; x < WIREGUARD_MAX_PEERS; x++) {
		wireguard_peer_t *cur_key = &device->peers[x];
		if (cur_key->valid) {
			if (cur_key->handshake.valid && cur_key->handshake.initiator && (cur_key->handshake.local_index == receiver)) {
				return cur_key;
			}
		}
	}
	return NULL;
}

bool wireguard_expired(uint32_t created_millis, uint32_t valid_seconds) {
	uint32_t diff = wireguard_sys_now() - created_millis;
	return (diff >= (valid_seconds * 1000));
}


static void generate_cookie_secret(struct wireguard_device *device) {
	wireguard_random_bytes(device->cookie_secret, WIREGUARD_HASH_LEN);
	device->cookie_secret_millis = wireguard_sys_now();
}

static void generate_peer_cookie(struct wireguard_device *device, uint8_t *cookie, uint8_t *source_addr_port, size_t source_length) {
	wireguard_blake2s_ctx ctx;

	if (wireguard_expired(device->cookie_secret_millis, COOKIE_SECRET_MAX_AGE)) {
		// Generate new random bytes
		generate_cookie_secret(device);
	}

	// Mac(key, input) Keyed-Blake2s(key, input, 16), the keyed MAC variant of the BLAKE2s hash function, returning 16 bytes of output
	wireguard_blake2s_init(&ctx, WIREGUARD_COOKIE_LEN, device->cookie_secret, WIREGUARD_HASH_LEN);
	// 5.4.7 Under Load: Cookie Reply Message
	// Mix in the IP address and port - have the IP layer pass this in as byte array to avoid using Lwip specific APIs in this module
	if ((source_addr_port) && (source_length > 0)) {
		wireguard_blake2s_update(&ctx, source_addr_port, source_length);
	}
	wireguard_blake2s_final(&ctx, cookie);
}

static void wireguard_mac(uint8_t *dst, const void *message, size_t len, const uint8_t *key, size_t keylen) {
	wireguard_blake2s(dst, WIREGUARD_COOKIE_LEN, key, keylen, message, len);
}

static void wireguard_mac_key(uint8_t *key, const uint8_t *public_key, const uint8_t *label, size_t label_len) {
	blake2s_ctx ctx;
	blake2s_init(&ctx, WIREGUARD_SESSION_KEY_LEN, NULL, 0);
	blake2s_update(&ctx, label, label_len);
	blake2s_update(&ctx, public_key, WIREGUARD_PUBLIC_KEY_LEN);
	blake2s_final(&ctx, key);
}

static void wireguard_mix_hash(uint8_t *hash, const uint8_t *src, size_t src_len) {
	wireguard_blake2s_ctx ctx;
	wireguard_blake2s_init(&ctx, WIREGUARD_HASH_LEN, NULL, 0);
	wireguard_blake2s_update(&ctx, hash, WIREGUARD_HASH_LEN);
	wireguard_blake2s_update(&ctx, src, src_len);
	wireguard_blake2s_final(&ctx, hash);
}

static void wireguard_hmac(uint8_t *digest, const uint8_t *key, size_t key_len, const uint8_t *text, size_t text_len) {
	// Adapted from appendix example in RFC2104 to use BLAKE2S instead of MD5 - https://tools.ietf.org/html/rfc2104
	wireguard_blake2s_ctx ctx;
	uint8_t k_ipad[WIREGUARD_BLAKE2S_BLOCK_SIZE]; // inner padding - key XORd with ipad
	uint8_t k_opad[WIREGUARD_BLAKE2S_BLOCK_SIZE]; // outer padding - key XORd with opad

	uint8_t tk[WIREGUARD_HASH_LEN];
	int i;
	// if key is longer than BLAKE2S_BLOCK_SIZE bytes reset it to key=BLAKE2S(key)
	if (key_len > WIREGUARD_BLAKE2S_BLOCK_SIZE) {
		wireguard_blake2s_ctx tctx;
		wireguard_blake2s_init(&tctx, WIREGUARD_HASH_LEN, NULL, 0);
		wireguard_blake2s_update(&tctx, key, key_len);
		wireguard_blake2s_final(&tctx, tk);
		key = tk;
		key_len = WIREGUARD_HASH_LEN;
	}

	// the HMAC transform looks like:
	// HASH(K XOR opad, HASH(K XOR ipad, text))
	// where K is an n byte key
	// ipad is the byte 0x36 repeated BLAKE2S_BLOCK_SIZE times
	// opad is the byte 0x5c repeated BLAKE2S_BLOCK_SIZE times
	// and text is the data being protected
	memset(k_ipad, 0, sizeof(k_ipad));
	memset(k_opad, 0, sizeof(k_opad));
	memcpy(k_ipad, key, key_len);
	memcpy(k_opad, key, key_len);

	// XOR key with ipad and opad values
	for (i=0; i < WIREGUARD_BLAKE2S_BLOCK_SIZE; i++) {
		k_ipad[i] ^= 0x36;
		k_opad[i] ^= 0x5c;
	}
	// perform inner HASH
	wireguard_blake2s_init(&ctx, WIREGUARD_HASH_LEN, NULL, 0); // init context for 1st pass
	wireguard_blake2s_update(&ctx, k_ipad, WIREGUARD_BLAKE2S_BLOCK_SIZE); // start with inner pad
	wireguard_blake2s_update(&ctx, text, text_len); // then text of datagram
	wireguard_blake2s_final(&ctx, digest); // finish up 1st pass

	// perform outer HASH
	wireguard_blake2s_init(&ctx, WIREGUARD_HASH_LEN, NULL, 0); // init context for 2nd pass
	wireguard_blake2s_update(&ctx, k_opad, WIREGUARD_BLAKE2S_BLOCK_SIZE); // start with outer pad
	wireguard_blake2s_update(&ctx, digest, WIREGUARD_HASH_LEN); // then results of 1st hash
	wireguard_blake2s_final(&ctx, digest); // finish up 2nd pass
}

static void wireguard_kdf1(uint8_t *tau1, const uint8_t *chaining_key, const uint8_t *data, size_t data_len) {
	uint8_t tau0[WIREGUARD_HASH_LEN];
	uint8_t output[WIREGUARD_HASH_LEN + 1];

	// tau0 = Hmac(key, input)
	wireguard_hmac(tau0, chaining_key, WIREGUARD_HASH_LEN, data, data_len);
	// tau1 := Hmac(tau0, 0x1)
	output[0] = 1;
	wireguard_hmac(output, tau0, WIREGUARD_HASH_LEN, output, 1);
	memcpy(tau1, output, WIREGUARD_HASH_LEN);

	// Wipe intermediates
	crypto_zero(tau0, sizeof(tau0));
	crypto_zero(output, sizeof(output));
}

static void wireguard_kdf2(uint8_t *tau1, uint8_t *tau2, const uint8_t *chaining_key, const uint8_t *data, size_t data_len) {
	uint8_t tau0[WIREGUARD_HASH_LEN];
	uint8_t output[WIREGUARD_HASH_LEN + 1];

	// tau0 = Hmac(key, input)
	wireguard_hmac(tau0, chaining_key, WIREGUARD_HASH_LEN, data, data_len);
	// tau1 := Hmac(tau0, 0x1)
	output[0] = 1;
	wireguard_hmac(output, tau0, WIREGUARD_HASH_LEN, output, 1);
	memcpy(tau1, output, WIREGUARD_HASH_LEN);

	// tau2 := Hmac(tau0,tau1 || 0x2)
	output[WIREGUARD_HASH_LEN] = 2;
	wireguard_hmac(output, tau0, WIREGUARD_HASH_LEN, output, WIREGUARD_HASH_LEN + 1);
	memcpy(tau2, output, WIREGUARD_HASH_LEN);

	// Wipe intermediates
	crypto_zero(tau0, sizeof(tau0));
	crypto_zero(output, sizeof(output));
}

static void wireguard_kdf3(uint8_t *tau1, uint8_t *tau2, uint8_t *tau3, const uint8_t *chaining_key, const uint8_t *data, size_t data_len) {
	uint8_t tau0[WIREGUARD_HASH_LEN];
	uint8_t output[WIREGUARD_HASH_LEN + 1];

	// tau0 = Hmac(key, input)
	wireguard_hmac(tau0, chaining_key, WIREGUARD_HASH_LEN, data, data_len);
	// tau1 := Hmac(tau0, 0x1)
	output[0] = 1;
	wireguard_hmac(output, tau0, WIREGUARD_HASH_LEN, output, 1);
	memcpy(tau1, output, WIREGUARD_HASH_LEN);

	// tau2 := Hmac(tau0,tau1 || 0x2)
	output[WIREGUARD_HASH_LEN] = 2;
	wireguard_hmac(output, tau0, WIREGUARD_HASH_LEN, output, WIREGUARD_HASH_LEN + 1);
	memcpy(tau2, output, WIREGUARD_HASH_LEN);

	// tau3 := Hmac(tau0,tau1,tau2 || 0x3)
	output[WIREGUARD_HASH_LEN] = 3;
	wireguard_hmac(output, tau0, WIREGUARD_HASH_LEN, output, WIREGUARD_HASH_LEN + 1);
	memcpy(tau3, output, WIREGUARD_HASH_LEN);

	// Wipe intermediates
	crypto_zero(tau0, sizeof(tau0));
	crypto_zero(output, sizeof(output));
}

bool wireguard_check_replay(wireguard_keypair_t *keypair, uint64_t seq) {
	// AddLog(LOG_LEVEL_INFO, ">>>: wireguard_check_replay");
	// Implementation of packet replay window - as per RFC2401
	// Adapted from code in Appendix C at https://tools.ietf.org/html/rfc2401
	uint32_t diff;
	bool result = false;
	size_t ReplayWindowSize = sizeof(keypair->replay_bitmap) * CHAR_BIT; // 32 bits

	// WireGuard data packet counter starts from 0 but algorithm expects packet numbers to start from 1
	seq++;

	if (seq != 0) {
		if (seq > keypair->replay_counter) {
			// new larger sequence number
			diff = seq - keypair->replay_counter;
			if (diff < ReplayWindowSize) {
				// In window
				keypair->replay_bitmap <<= diff;
				// set bit for this packet
				keypair->replay_bitmap |= 1;
			} else {
				// This packet has a "way larger"
				keypair->replay_bitmap = 1;
			}
			keypair->replay_counter = seq;
			// larger is good
			result = true;
		} else {
			diff = keypair->replay_counter - seq;
			if (diff < ReplayWindowSize) {
				if (keypair->replay_bitmap & ((uint32_t)1 << diff)) {
					// already seen
				} else {
					// mark as seen
					keypair->replay_bitmap |= ((uint32_t)1 << diff);
					// out of order but good
					result = true;
				}
			} else {
				// too old or wrapped
			}
		}
	} else {
		// first == 0 or wrapped
	}
	return result;
}

wireguard_keypair_t *get_peer_keypair_for_idx(wireguard_peer_t *peer, uint32_t idx) {
	if (peer->curr_keypair.valid && peer->curr_keypair.local_index == idx) {
		return &peer->curr_keypair;
	} else if (peer->next_keypair.valid && peer->next_keypair.local_index == idx) {
		return &peer->next_keypair;
	} else if (peer->prev_keypair.valid && peer->prev_keypair.local_index == idx) {
		return &peer->prev_keypair;
	}
	return NULL;
}

static uint32_t wireguard_generate_unique_index(struct wireguard_device *device) {
	// We need a random 32-bit number but make sure it's not already been used in the context of this device
	uint32_t result;
	uint8_t buf[4];
	wireguard_peer_t *peer;
	bool existing = false;
	do {
		do {
			wireguard_random_bytes(buf, 4);
			result = U8TO32_LITTLE(buf);
		} while ((result == 0) || (result == 0xFFFFFFFF)); // Don't allow 0 or 0xFFFFFFFF as valid values

		existing = false;
		for (int32_t x=0; x < WIREGUARD_MAX_PEERS; x++) {
			peer = &device->peers[x];
			existing = (result == peer->curr_keypair.local_index) ||
					(result == peer->prev_keypair.local_index) ||
					(result == peer->next_keypair.local_index) ||
					(result == peer->handshake.local_index);

		}
	} while (existing);

	return result;
}

static void wireguard_clamp_private_key(uint8_t *key) {
	key[0] &= 248;
	key[31] = (key[31] & 127) | 64;
}

static void wireguard_generate_private_key(uint8_t *key) {
	wireguard_random_bytes(key, WIREGUARD_PRIVATE_KEY_LEN);
	wireguard_clamp_private_key(key);
}

static bool wireguard_generate_public_key(uint8_t *public_key, const uint8_t *private_key) {
	static const uint8_t basepoint[WIREGUARD_PUBLIC_KEY_LEN] = { 9 };
	bool result = false;
	if (memcmp(private_key, zero_key, WIREGUARD_PUBLIC_KEY_LEN) != 0) {
		result = (wireguard_x25519(public_key, private_key, basepoint) == 0);
	}
	return result;
}

bool wireguard_check_mac1(struct wireguard_device *device, const uint8_t *data, size_t len, const uint8_t *mac1) {
	bool result = false;
	uint8_t calculated[WIREGUARD_COOKIE_LEN];
	wireguard_mac(calculated, data, len, device->label_mac1_key, WIREGUARD_SESSION_KEY_LEN);
	if (crypto_equal(calculated, mac1, WIREGUARD_COOKIE_LEN)) {
		result = true;
	}
	return result;
}

bool wireguard_check_mac2(struct wireguard_device *device, const uint8_t *data, size_t len, uint8_t *source_addr_port, size_t source_length, const uint8_t *mac2) {
	// AddLog(LOG_LEVEL_INFO, ">>>: wireguard_check_mac2");
	bool result = false;
	uint8_t cookie[WIREGUARD_COOKIE_LEN];
	uint8_t calculated[WIREGUARD_COOKIE_LEN];

	generate_peer_cookie(device, cookie, source_addr_port, source_length);

	wireguard_mac(calculated, data, len, cookie, WIREGUARD_COOKIE_LEN);
	if (crypto_equal(calculated, mac2, WIREGUARD_COOKIE_LEN)) {
		result = true;
	}
	return result;
}

void keypair_destroy(wireguard_keypair_t *keypair) {
	crypto_zero(keypair, sizeof(wireguard_keypair_t));
	keypair->valid = false;
}

void keypair_update(wireguard_peer_t *peer, wireguard_keypair_t *received_keypair) {
	bool key_is_next = (received_keypair == &peer->next_keypair);
	if (key_is_next) {
		peer->prev_keypair = peer->curr_keypair;
		peer->curr_keypair = peer->next_keypair;
		keypair_destroy(&peer->next_keypair);
	}
}

static void add_new_keypair(wireguard_peer_t *peer, wireguard_keypair_t new_keypair) {
	if (new_keypair.initiator) {
		if (peer->next_keypair.valid) {
			peer->prev_keypair = peer->next_keypair;
			keypair_destroy(&peer->next_keypair);
		} else  {
			peer->prev_keypair = peer->curr_keypair;
		}
		peer->curr_keypair = new_keypair;
	} else {
		peer->next_keypair =  new_keypair;
		keypair_destroy(&peer->prev_keypair);
	}
	peer->latest_handshake_millis = new_keypair.keypair_millis;
}

void wireguard_start_session(wireguard_peer_t *peer, bool initiator) {
	wireguard_handshake_t *handshake = &peer->handshake;
	wireguard_keypair_t new_keypair;

	crypto_zero(&new_keypair, sizeof(wireguard_keypair_t));
	new_keypair.initiator = initiator;
	new_keypair.local_index = handshake->local_index;
	new_keypair.remote_index = handshake->remote_index;

	new_keypair.keypair_millis = wireguard_sys_now();
	new_keypair.sending_valid = true;
	new_keypair.receiving_valid = true;

	// 5.4.5 Transport Data Key Derivation
	// (Tsendi = Trecvr, Trecvi = Tsendr) := Kdf2(Ci = Cr,E)
	if (new_keypair.initiator) {
		wireguard_kdf2(new_keypair.sending_key, new_keypair.receiving_key, handshake->chaining_key, NULL, 0);
	} else {
		wireguard_kdf2(new_keypair.receiving_key, new_keypair.sending_key, handshake->chaining_key, NULL, 0);
	}

	new_keypair.replay_bitmap = 0;
	new_keypair.replay_counter = 0;

	new_keypair.last_tx = 0;
	new_keypair.last_rx = 0; // No packets received yet

	new_keypair.valid = true;

	// Eprivi = Epubi = Eprivr = Epubr = Ci = Cr := E
	crypto_zero(handshake->ephemeral_private, WIREGUARD_PUBLIC_KEY_LEN);
	crypto_zero(handshake->remote_ephemeral, WIREGUARD_PUBLIC_KEY_LEN);
	crypto_zero(handshake->hash, WIREGUARD_HASH_LEN);
	crypto_zero(handshake->chaining_key, WIREGUARD_HASH_LEN);
	handshake->remote_index = 0;
	handshake->local_index = 0;
	handshake->valid = false;

	add_new_keypair(peer, new_keypair);
}

uint8_t wireguard_get_message_type(const uint8_t *data, size_t len) {
	uint8_t result = MESSAGE_INVALID;
	if (len >= 4) {
		if ((data[1] == 0) && (data[2] == 0) && (data[3] == 0)) {
			switch (data[0]) {
				case MESSAGE_HANDSHAKE_INITIATION:
					if (len == sizeof(struct message_handshake_initiation)) {
						result = MESSAGE_HANDSHAKE_INITIATION;
					}
					break;
				case MESSAGE_HANDSHAKE_RESPONSE:
					if (len == sizeof(struct message_handshake_response)) {
						result = MESSAGE_HANDSHAKE_RESPONSE;
					}
					break;
				case MESSAGE_COOKIE_REPLY:
					if (len == sizeof(struct message_cookie_reply)) {
						result = MESSAGE_COOKIE_REPLY;
					}
					break;
				case MESSAGE_TRANSPORT_DATA:
					if (len >= sizeof(struct message_transport_data) + WIREGUARD_AUTHTAG_LEN) {
						result = MESSAGE_TRANSPORT_DATA;
					}
					break;
				default:
					break;
			}
		}
	}
	return result;
}

wireguard_peer_t *wireguard_process_initiation_message(struct wireguard_device *device, struct message_handshake_initiation *msg) {
	// AddLog(LOG_LEVEL_DEBUG, ">>>: wireguard_process_initiation_message");
	wireguard_peer_t *ret_peer = NULL;
	wireguard_peer_t *peer = NULL;
	wireguard_handshake_t *handshake;
	uint8_t key[WIREGUARD_SESSION_KEY_LEN];
	uint8_t chaining_key[WIREGUARD_HASH_LEN];
	uint8_t hash[WIREGUARD_HASH_LEN];
	uint8_t s[WIREGUARD_PUBLIC_KEY_LEN];
	uint8_t e[WIREGUARD_PUBLIC_KEY_LEN];
	uint8_t t[WIREGUARD_TAI64N_LEN];
	uint8_t dh_calculation[WIREGUARD_PUBLIC_KEY_LEN];
	uint32_t now;
	bool rate_limit;
	bool replay;

	// We are the responder, other end is the initiator

	// Ci := Hash(Construction) (precalculated hash)
	memcpy(chaining_key, construction_hash, WIREGUARD_HASH_LEN);

	// Hi := Hash(Ci || Identifier
	memcpy(hash, identifier_hash, WIREGUARD_HASH_LEN);

	// Hi := Hash(Hi || Spubr)
	wireguard_mix_hash(hash, device->public_key, WIREGUARD_PUBLIC_KEY_LEN);

	 // Ci := Kdf1(Ci, Epubi)
	wireguard_kdf1(chaining_key, chaining_key, msg->ephemeral, WIREGUARD_PUBLIC_KEY_LEN);

	// msg.ephemeral := Epubi
	memcpy(e, msg->ephemeral, WIREGUARD_PUBLIC_KEY_LEN);

	// Hi := Hash(Hi || msg.ephemeral)
	wireguard_mix_hash(hash, msg->ephemeral, WIREGUARD_PUBLIC_KEY_LEN);

	// Calculate DH(Eprivi,Spubr)
	wireguard_x25519(dh_calculation, device->private_key, e);
	if (!crypto_equal(dh_calculation, zero_key, WIREGUARD_PUBLIC_KEY_LEN)) {

		// (Ci,k) := Kdf2(Ci,DH(Eprivi,Spubr))
		wireguard_kdf2(chaining_key, key, chaining_key, dh_calculation, WIREGUARD_PUBLIC_KEY_LEN);

		// msg.static := AEAD(k, 0, Spubi, Hi)
		if (wireguard_aead_decrypt(s, msg->enc_static, sizeof(msg->enc_static), hash, WIREGUARD_HASH_LEN, 0, key)) {
			// Hi := Hash(Hi || msg.static)
			wireguard_mix_hash(hash, msg->enc_static, sizeof(msg->enc_static));

			peer = wireguard_peer_lookup_by_pubkey(device, s);
			if (peer) {
				handshake = &peer->handshake;

				// (Ci,k) := Kdf2(Ci,DH(Sprivi,Spubr))
				wireguard_kdf2(chaining_key, key, chaining_key, peer->public_key_dh, WIREGUARD_PUBLIC_KEY_LEN);

				// msg.timestamp := AEAD(k, 0, Timestamp(), Hi)
				if (wireguard_aead_decrypt(t, msg->enc_timestamp, sizeof(msg->enc_timestamp), hash, WIREGUARD_HASH_LEN, 0, key)) {
					// Hi := Hash(Hi || msg.timestamp)
					wireguard_mix_hash(hash, msg->enc_timestamp, sizeof(msg->enc_timestamp));

					now = wireguard_sys_now();

					// Check that timestamp is increasing and we haven't had too many initiations (should only get one per peer every 5 seconds max?)
					replay = (memcmp(t, peer->greatest_timestamp, WIREGUARD_TAI64N_LEN) <= 0); // tai64n is big endian so we can use memcmp to compare
					rate_limit = (peer->last_initiation_rx - now) < (1000 / MAX_INITIATIONS_PER_SECOND);

					if (!replay && !rate_limit) {
						// Success! Copy everything to peer
						peer->last_initiation_rx = now;
						if (memcmp(t, peer->greatest_timestamp, WIREGUARD_TAI64N_LEN) > 0) {
							memcpy(peer->greatest_timestamp, t, WIREGUARD_TAI64N_LEN);
							// TODO: Need to notify if the higher layers want to persist latest timestamp/nonce somewhere
						}
						memcpy(handshake->remote_ephemeral, e, WIREGUARD_PUBLIC_KEY_LEN);
						memcpy(handshake->hash, hash, WIREGUARD_HASH_LEN);
						memcpy(handshake->chaining_key, chaining_key, WIREGUARD_HASH_LEN);
						handshake->remote_index = msg->sender;
						handshake->valid = true;
						handshake->initiator = false;
						ret_peer = peer;

					} else {
						// Ignore
					}
				} else {
					AddLog(LOG_LEVEL_INFO, PSTR("WG : wireguard_process_initiation_message: failed to decrypt"));
				}
			} else {
				AddLog(LOG_LEVEL_INFO, PSTR("WG : peer not found"));
			}
		} else {
			AddLog(LOG_LEVEL_INFO, PSTR("WG : Failed to decrypt"));
		}
	} else {
		AddLog(LOG_LEVEL_INFO, PSTR("WG : Bad X25519"));
	}

	crypto_zero(key, sizeof(key));
	crypto_zero(hash, sizeof(hash));
	crypto_zero(chaining_key, sizeof(chaining_key));
	crypto_zero(dh_calculation, sizeof(dh_calculation));

	return ret_peer;
}

bool wireguard_process_handshake_response(struct wireguard_device *device, wireguard_peer_t *peer, struct message_handshake_response *src) {
	wireguard_handshake_t *handshake = &peer->handshake;

	bool result = false;
	uint8_t key[WIREGUARD_SESSION_KEY_LEN];
	uint8_t hash[WIREGUARD_HASH_LEN];
	uint8_t chaining_key[WIREGUARD_HASH_LEN];
	uint8_t e[WIREGUARD_PUBLIC_KEY_LEN];
	uint8_t ephemeral_private[WIREGUARD_PUBLIC_KEY_LEN];
	uint8_t static_private[WIREGUARD_PUBLIC_KEY_LEN];
	uint8_t preshared_key[WIREGUARD_SESSION_KEY_LEN];
	uint8_t dh_calculation[WIREGUARD_PUBLIC_KEY_LEN];
	uint8_t tau[WIREGUARD_PUBLIC_KEY_LEN];

	if (handshake->valid && handshake->initiator) {

		memcpy(hash, handshake->hash, WIREGUARD_HASH_LEN);
		memcpy(chaining_key, handshake->chaining_key, WIREGUARD_HASH_LEN);
		memcpy(ephemeral_private, handshake->ephemeral_private, WIREGUARD_PUBLIC_KEY_LEN);
		memcpy(preshared_key, peer->preshared_key, WIREGUARD_SESSION_KEY_LEN);

		// (Eprivr, Epubr) := DH-Generate()
		// Not required

		// Cr := Kdf1(Cr,Epubr)
		wireguard_kdf1(chaining_key, chaining_key, src->ephemeral, WIREGUARD_PUBLIC_KEY_LEN);

		// msg.ephemeral := Epubr
		memcpy(e, src->ephemeral, WIREGUARD_PUBLIC_KEY_LEN);

		// Hr := Hash(Hr || msg.ephemeral)
		wireguard_mix_hash(hash, src->ephemeral, WIREGUARD_PUBLIC_KEY_LEN);

		// Cr := Kdf1(Cr, DH(Eprivr, Epubi))
		// Calculate DH(Eprivr, Epubi)
		wireguard_x25519(dh_calculation, ephemeral_private, e);
		if (!crypto_equal(dh_calculation, zero_key, WIREGUARD_PUBLIC_KEY_LEN)) {
			wireguard_kdf1(chaining_key, chaining_key, dh_calculation, WIREGUARD_PUBLIC_KEY_LEN);

			// Cr := Kdf1(Cr, DH(Eprivr, Spubi))
			// CalculateDH(Eprivr, Spubi)
			wireguard_x25519(dh_calculation, device->private_key, e);
			if (!crypto_equal(dh_calculation, zero_key, WIREGUARD_PUBLIC_KEY_LEN)) {
				wireguard_kdf1(chaining_key, chaining_key, dh_calculation, WIREGUARD_PUBLIC_KEY_LEN);

				// (Cr, t, k) := Kdf3(Cr, Q)
				wireguard_kdf3(chaining_key, tau, key, chaining_key, peer->preshared_key, WIREGUARD_SESSION_KEY_LEN);

				// Hr := Hash(Hr | t)
				wireguard_mix_hash(hash, tau, WIREGUARD_HASH_LEN);

				// msg.empty := AEAD(k, 0, E, Hr)
				if (wireguard_aead_decrypt(NULL, src->enc_empty, sizeof(src->enc_empty), hash, WIREGUARD_HASH_LEN, 0, key)) {
					// Hr := Hash(Hr | msg.empty)
					// Not required as discarded

					//Copy details to handshake
					memcpy(handshake->remote_ephemeral, e, WIREGUARD_HASH_LEN);
					memcpy(handshake->hash, hash, WIREGUARD_HASH_LEN);
					memcpy(handshake->chaining_key, chaining_key, WIREGUARD_HASH_LEN);
					handshake->remote_index = src->sender;

					result = true;
				} else {
					AddLog(LOG_LEVEL_DEBUG, PSTR("WG : Decrypt failed"));
				}

			} else {
				AddLog(LOG_LEVEL_DEBUG, PSTR("WG : X25519 fail"));
			}

		} else {
			AddLog(LOG_LEVEL_DEBUG, PSTR("WG : X25519 fail 2"));
		}

	}
	crypto_zero(key, sizeof(key));
	crypto_zero(hash, sizeof(hash));
	crypto_zero(chaining_key, sizeof(chaining_key));
	crypto_zero(ephemeral_private, sizeof(ephemeral_private));
	crypto_zero(static_private, sizeof(static_private));
	crypto_zero(preshared_key, sizeof(preshared_key));
	crypto_zero(tau, sizeof(tau));

	// AddLog(LOG_LEVEL_DEBUG, ">>>: OUT wireguard_process_handshake_response result %d (1==SUCCESS)");
	return result;
}

bool wireguard_process_cookie_message(struct wireguard_device *device, wireguard_peer_t *peer, struct message_cookie_reply *src) {
	// AddLog(LOG_LEVEL_INFO, ">>>: wireguard_process_cookie_message");
	uint8_t cookie[WIREGUARD_COOKIE_LEN];
	bool result = false;

	if (peer->handshake_mac1_valid) {

		result = wireguard_xaead_decrypt(cookie, src->enc_cookie, sizeof(src->enc_cookie), peer->handshake_mac1, WIREGUARD_COOKIE_LEN, src->nonce, peer->label_cookie_key);

		if (result) {
			// 5.4.7 Under Load: Cookie Reply Message
			// Upon receiving this message, if it is valid, the only thing the recipient of this message should do is store the cookie along with the time at which it was received
			memcpy(peer->cookie, cookie, WIREGUARD_COOKIE_LEN);
			peer->cookie_millis = wireguard_sys_now();
			peer->handshake_mac1_valid = false;
		}
	} else {
		// We didn't send any initiation packet so we shouldn't be getting a cookie reply!
	}
	return result;
}

bool wireguard_create_handshake_initiation(struct wireguard_device *device, wireguard_peer_t *peer, struct message_handshake_initiation *dst) {
	uint8_t timestamp[WIREGUARD_TAI64N_LEN];
	uint8_t key[WIREGUARD_SESSION_KEY_LEN];
	uint8_t dh_calculation[WIREGUARD_PUBLIC_KEY_LEN];
	bool result = false;

	wireguard_handshake_t *handshake = &peer->handshake;

	memset(dst, 0, sizeof(struct message_handshake_initiation));

	// Ci := Hash(Construction) (precalculated hash)
	memcpy(handshake->chaining_key, construction_hash, WIREGUARD_HASH_LEN);

	// Hi := Hash(Ci || Identifier)
	memcpy(handshake->hash, identifier_hash, WIREGUARD_HASH_LEN);

	// Hi := Hash(Hi || Spubr)
	wireguard_mix_hash(handshake->hash, peer->public_key, WIREGUARD_PUBLIC_KEY_LEN);

	// (Eprivi, Epubi) := DH-Generate()
	wireguard_generate_private_key(handshake->ephemeral_private);
	if (wireguard_generate_public_key(dst->ephemeral, handshake->ephemeral_private)) {
		// Ci := Kdf1(Ci, Epubi)
		wireguard_kdf1(handshake->chaining_key, handshake->chaining_key, dst->ephemeral, WIREGUARD_PUBLIC_KEY_LEN);

		// msg.ephemeral := Epubi
		// Done above - public keys is calculated into dst->ephemeral

		// Hi := Hash(Hi || msg.ephemeral)
		wireguard_mix_hash(handshake->hash, dst->ephemeral, WIREGUARD_PUBLIC_KEY_LEN);

		// Calculate DH(Eprivi,Spubr)
		wireguard_x25519(dh_calculation, handshake->ephemeral_private, peer->public_key);
		if (!crypto_equal(dh_calculation, zero_key, WIREGUARD_PUBLIC_KEY_LEN)) {

			// (Ci,k) := Kdf2(Ci,DH(Eprivi,Spubr))
			wireguard_kdf2(handshake->chaining_key, key, handshake->chaining_key, dh_calculation, WIREGUARD_PUBLIC_KEY_LEN);

			// msg.static := AEAD(k,0,Spubi, Hi)
			wireguard_aead_encrypt(dst->enc_static, device->public_key, WIREGUARD_PUBLIC_KEY_LEN, handshake->hash, WIREGUARD_HASH_LEN, 0, key);

			// Hi := Hash(Hi || msg.static)
			wireguard_mix_hash(handshake->hash, dst->enc_static, sizeof(dst->enc_static));

			// (Ci,k) := Kdf2(Ci,DH(Sprivi,Spubr))
			// note DH(Sprivi,Spubr) is precomputed per peer
			wireguard_kdf2(handshake->chaining_key, key, handshake->chaining_key, peer->public_key_dh, WIREGUARD_PUBLIC_KEY_LEN);

			// msg.timestamp := AEAD(k, 0, Timestamp(), Hi)
			wireguard_tai64n_now(timestamp);
			wireguard_aead_encrypt(dst->enc_timestamp, timestamp, WIREGUARD_TAI64N_LEN, handshake->hash, WIREGUARD_HASH_LEN, 0, key);

			// Hi := Hash(Hi || msg.timestamp)
			wireguard_mix_hash(handshake->hash, dst->enc_timestamp, sizeof(dst->enc_timestamp));

			dst->type = MESSAGE_HANDSHAKE_INITIATION;
			dst->sender = wireguard_generate_unique_index(device);

			handshake->valid = true;
			handshake->initiator = true;
			handshake->local_index = dst->sender;

			result = true;
		}
	}

	if (result) {
		// 5.4.4 Cookie MACs
		// msg.mac1 := Mac(Hash(Label-Mac1 || Spubm' ), msgA)
		// The value Hash(Label-Mac1 || Spubm' ) above can be pre-computed
		wireguard_mac(dst->mac1, dst, (sizeof(struct message_handshake_initiation)-(2*WIREGUARD_COOKIE_LEN)), peer->label_mac1_key, WIREGUARD_SESSION_KEY_LEN);

		// if Lm = E or Lm ≥ 120:
		if ((peer->cookie_millis == 0) || wireguard_expired(peer->cookie_millis, COOKIE_SECRET_MAX_AGE)) {
			// msg.mac2 := 0
			crypto_zero(dst->mac2, WIREGUARD_COOKIE_LEN);
		} else {
			// msg.mac2 := Mac(Lm, msgB)
			wireguard_mac(dst->mac2, dst, (sizeof(struct message_handshake_initiation)-(WIREGUARD_COOKIE_LEN)), peer->cookie, WIREGUARD_COOKIE_LEN);

		}
	}

	crypto_zero(key, sizeof(key));
	crypto_zero(dh_calculation, sizeof(dh_calculation));
	return result;
}

bool wireguard_create_handshake_response(struct wireguard_device *device, wireguard_peer_t *peer, struct message_handshake_response *dst) {
	wireguard_handshake_t *handshake = &peer->handshake;
	uint8_t key[WIREGUARD_SESSION_KEY_LEN];
	uint8_t dh_calculation[WIREGUARD_PUBLIC_KEY_LEN];
	uint8_t tau[WIREGUARD_HASH_LEN];
	bool result = false;

	memset(dst, 0, sizeof(struct message_handshake_response));

	if (handshake->valid && !handshake->initiator) {

		// (Eprivr, Epubr) := DH-Generate()
		wireguard_generate_private_key(handshake->ephemeral_private);
		if (wireguard_generate_public_key(dst->ephemeral, handshake->ephemeral_private)) {

			// Cr := Kdf1(Cr,Epubr)
			wireguard_kdf1(handshake->chaining_key, handshake->chaining_key, dst->ephemeral, WIREGUARD_PUBLIC_KEY_LEN);

			// msg.ephemeral := Epubr
			// Copied above when generated

			// Hr := Hash(Hr || msg.ephemeral)
			wireguard_mix_hash(handshake->hash, dst->ephemeral, WIREGUARD_PUBLIC_KEY_LEN);

			// Cr := Kdf1(Cr, DH(Eprivr, Epubi))
			// Calculate DH(Eprivi,Spubr)
			wireguard_x25519(dh_calculation, handshake->ephemeral_private, handshake->remote_ephemeral);
			if (!crypto_equal(dh_calculation, zero_key, WIREGUARD_PUBLIC_KEY_LEN)) {
				wireguard_kdf1(handshake->chaining_key, handshake->chaining_key, dh_calculation, WIREGUARD_PUBLIC_KEY_LEN);

				// Cr := Kdf1(Cr, DH(Eprivr, Spubi))
				// Calculate DH(Eprivi,Spubr)
				wireguard_x25519(dh_calculation, handshake->ephemeral_private, peer->public_key);
				if (!crypto_equal(dh_calculation, zero_key, WIREGUARD_PUBLIC_KEY_LEN)) {
					wireguard_kdf1(handshake->chaining_key, handshake->chaining_key, dh_calculation, WIREGUARD_PUBLIC_KEY_LEN);

					// (Cr, t, k) := Kdf3(Cr, Q)
					wireguard_kdf3(handshake->chaining_key, tau, key, handshake->chaining_key, peer->preshared_key, WIREGUARD_SESSION_KEY_LEN);

					// Hr := Hash(Hr | t)
					wireguard_mix_hash(handshake->hash, tau, WIREGUARD_HASH_LEN);

					// msg.empty := AEAD(k, 0, E, Hr)
					wireguard_aead_encrypt(dst->enc_empty, NULL, 0, handshake->hash, WIREGUARD_HASH_LEN, 0, key);

					// Hr := Hash(Hr | msg.empty)
					wireguard_mix_hash(handshake->hash, dst->enc_empty, sizeof(dst->enc_empty));

					dst->type = MESSAGE_HANDSHAKE_RESPONSE;
					dst->receiver = handshake->remote_index;
					dst->sender = wireguard_generate_unique_index(device);
					// Update handshake object too
					handshake->local_index = dst->sender;

					result = true;
				} else {
					// Bad x25519
				}
			} else {
				// Bad x25519
			}

		} else {
			// Failed to generate DH
		}
	}

	if (result) {
		// 5.4.4 Cookie MACs
		// msg.mac1 := Mac(Hash(Label-Mac1 || Spubm' ), msgA)
		// The value Hash(Label-Mac1 || Spubm' ) above can be pre-computed
		wireguard_mac(dst->mac1, dst, (sizeof(struct message_handshake_response)-(2*WIREGUARD_COOKIE_LEN)), peer->label_mac1_key, WIREGUARD_SESSION_KEY_LEN);

		// if Lm = E or Lm ≥ 120:
		if ((peer->cookie_millis == 0) || wireguard_expired(peer->cookie_millis, COOKIE_SECRET_MAX_AGE)) {
			// msg.mac2 := 0
			crypto_zero(dst->mac2, WIREGUARD_COOKIE_LEN);
		} else {
			// msg.mac2 := Mac(Lm, msgB)
			wireguard_mac(dst->mac2, dst, (sizeof(struct message_handshake_response)-(WIREGUARD_COOKIE_LEN)), peer->cookie, WIREGUARD_COOKIE_LEN);
		}
	}

	crypto_zero(key, sizeof(key));
	crypto_zero(dh_calculation, sizeof(dh_calculation));
	crypto_zero(tau, sizeof(tau));
	return result;
}

void wireguard_create_cookie_reply(struct wireguard_device *device, struct message_cookie_reply *dst, const uint8_t *mac1, uint32_t index, uint8_t *source_addr_port, size_t source_length) {
	uint8_t cookie[WIREGUARD_COOKIE_LEN];
	crypto_zero(dst, sizeof(struct message_cookie_reply));
	dst->type = MESSAGE_COOKIE_REPLY;
	dst->receiver = index;
	wireguard_random_bytes(dst->nonce, COOKIE_NONCE_LEN);
	generate_peer_cookie(device, cookie, source_addr_port, source_length);
	wireguard_xaead_encrypt(dst->enc_cookie, cookie, WIREGUARD_COOKIE_LEN, mac1, WIREGUARD_COOKIE_LEN, dst->nonce, device->label_cookie_key);
}

bool wireguard_peer_init(struct wireguard_device *device, wireguard_peer_t *peer, const uint8_t *public_key, const uint8_t *preshared_key) {
	// Clear out structure
	memset(peer, 0, sizeof(wireguard_peer_t));
	if (device->valid) {
		// Copy across the public key into our peer structure
		memcpy(peer->public_key, public_key, WIREGUARD_PUBLIC_KEY_LEN);
		memcpy(peer->preshared_key, preshared_key, WIREGUARD_SESSION_KEY_LEN);

		// AddLog(LOG_LEVEL_INFO, ">>>: wireguard_peer_init pub_key %*_H priv_key %*_H peer_pub %*_H", 32, peer->public_key_dh, 32, device->private_key, 32, peer->public_key);
		if (wireguard_x25519(peer->public_key_dh, device->private_key, peer->public_key) == 0) {
			// Zero out handshake
			memset(&peer->handshake, 0, sizeof(wireguard_handshake_t));
			peer->handshake.valid = false;

			// Zero out any cookie info - we haven't received one yet
			peer->cookie_millis = 0;
			memset(&peer->cookie, 0, WIREGUARD_COOKIE_LEN);

			// Precompute keys to deal with mac1/2 calculation
			wireguard_mac_key(peer->label_mac1_key, peer->public_key, LABEL_MAC1, sizeof(LABEL_MAC1) - 1);
			wireguard_mac_key(peer->label_cookie_key, peer->public_key, LABEL_COOKIE, sizeof(LABEL_COOKIE) - 1);

			peer->valid = true;
		} else {
			crypto_zero(peer->public_key_dh, WIREGUARD_PUBLIC_KEY_LEN);
		}
	}
	return peer->valid;
}

bool wireguard_device_init(struct wireguard_device *device, const uint8_t *private_key) {
	// Set the private key and calculate public key from it
	memcpy(device->private_key, private_key, WIREGUARD_PRIVATE_KEY_LEN);
	// Ensure private key is correctly "clamped"
	wireguard_clamp_private_key(device->private_key);
	device->valid = wireguard_generate_public_key(device->public_key, private_key);
	if (device->valid) {
		// AddLog(LOG_LEVEL_DEBUG, ">>>: wireguard_device_init public_key %*_H", 32, device->public_key);
		generate_cookie_secret(device);
		// AddLog(LOG_LEVEL_DEBUG, ">>>: wireguard_device_init cookie secret %*_H", 32, device->cookie_secret);
		// 5.4.4 Cookie MACs - The value Hash(Label-Mac1 || Spubm' ) above can be pre-computed.
		wireguard_mac_key(device->label_mac1_key, device->public_key, LABEL_MAC1, sizeof(LABEL_MAC1) - 1);
		// AddLog(LOG_LEVEL_DEBUG, ">>>: wireguard_device_init label_mac1_key %*_H", 32, device->label_mac1_key);
		// 5.4.7 Under Load: Cookie Reply Message - The value Hash(Label-Cookie || Spubm) above can be pre-computed.
		wireguard_mac_key(device->label_cookie_key, device->public_key, LABEL_COOKIE, sizeof(LABEL_COOKIE) - 1);
		// AddLog(LOG_LEVEL_DEBUG, ">>>: wireguard_device_init label_cookie_key %*_H", 32, device->label_cookie_key);
		
	} else {
		crypto_zero(device->private_key, WIREGUARD_PRIVATE_KEY_LEN);
	}
	return device->valid;
}

void wireguard_encrypt_packet(uint8_t *dst, const uint8_t *src, size_t src_len, wireguard_keypair_t *keypair) {
	wireguard_aead_encrypt(dst, src, src_len, NULL, 0, keypair->sending_counter, keypair->sending_key);
	keypair->sending_counter++;
}

bool wireguard_decrypt_packet(uint8_t *dst, const uint8_t *src, size_t src_len, uint64_t counter, wireguard_keypair_t *keypair) {
	return wireguard_aead_decrypt(dst, src, src_len, NULL, 0, counter, keypair->receiving_key);
}
