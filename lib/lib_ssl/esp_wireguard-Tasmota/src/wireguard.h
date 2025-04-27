/*
 * Copyright (c) 2021 Daniel Hope (www.floorsense.nz)
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

#ifndef _WIREGUARD_H_
#define _WIREGUARD_H_

#ifdef __cplusplus
extern "C" {
#endif

// Note: these are only required for definitions in device/peer for netif, udp_pcb, ip_addr_t and u16_t
#include "lwip/netif.h"
#include "lwip/ip_addr.h"
#include "lwip/arch.h"

// Platform-specific functions that need to be implemented per-platform
#include "wireguard-platform.h"

// tai64n contains 64-bit seconds and 32-bit nano offset (12 bytes)
#define WIREGUARD_TAI64N_LEN		(12)
// Auth algorithm is chacha20pol1305 which is 128bit (16 byte) authenticator
#define WIREGUARD_AUTHTAG_LEN		(16)
// Hash algorithm is blake2s which makes 32 byte hashes
#define WIREGUARD_HASH_LEN			(32)
// Public key algo is curve22519 which uses 32 byte keys
#define WIREGUARD_PUBLIC_KEY_LEN	(32)
// Public key algo is curve22519 which uses 32 byte keys
#define WIREGUARD_PRIVATE_KEY_LEN	(32)
// Symmetric session keys are chacha20/poly1305 which uses 32 byte keys
#define WIREGUARD_SESSION_KEY_LEN	(32)

// Timers / Limits
#define WIREGUARD_COOKIE_LEN		(16)
#define COOKIE_SECRET_MAX_AGE		(2 * 60)
#define COOKIE_NONCE_LEN			(24)

#define REKEY_AFTER_MESSAGES		(1ULL << 60)
#define REJECT_AFTER_MESSAGES		(0xFFFFFFFFFFFFFFFFULL - (1ULL << 13))
#define REKEY_AFTER_TIME			(120)
#define REJECT_AFTER_TIME			(180)
#define REKEY_TIMEOUT				(5)

typedef struct wireguard_keypair {
	bool valid;
	bool initiator; // Did we initiate this session (send the initiation packet rather than sending the response packet)
	uint32_t keypair_millis;

	uint8_t sending_key[WIREGUARD_SESSION_KEY_LEN];
	bool sending_valid;
	uint64_t sending_counter;

	uint8_t receiving_key[WIREGUARD_SESSION_KEY_LEN];
	bool receiving_valid;

	uint32_t last_tx;
	uint32_t last_rx;

	uint32_t replay_bitmap;
	uint64_t replay_counter;

	uint32_t local_index; // This is the index we generated for our end
	uint32_t remote_index; // This is the index on the other end
} wireguard_keypair_t;

typedef struct wireguard_handshake {
	bool valid;
	bool initiator;
	uint32_t local_index;
	uint32_t remote_index;
	uint8_t ephemeral_private[WIREGUARD_PRIVATE_KEY_LEN];
	uint8_t remote_ephemeral[WIREGUARD_PUBLIC_KEY_LEN];
	uint8_t hash[WIREGUARD_HASH_LEN];
	uint8_t chaining_key[WIREGUARD_HASH_LEN];
} wireguard_handshake_t;

typedef struct wireguard_allowed_ip {
	bool valid;
	ip_addr_t ip;
	ip_addr_t mask;
} wireguard_allowed_ip_t;

typedef struct wireguard_peer {
	bool valid; // Is this peer initialised?
	bool active; // Should we be actively trying to connect?

	// This is the configured IP of the peer (endpoint)
	ip_addr_t connect_ip;
	u16_t connect_port;
	// This is the latest received IP/port
	ip_addr_t ip;
	u16_t port;
	// keep-alive interval in seconds, 0 is disable
	uint16_t keepalive_interval;

	struct wireguard_allowed_ip allowed_source_ips[WIREGUARD_MAX_SRC_IPS];

	uint8_t public_key[WIREGUARD_PUBLIC_KEY_LEN];
	uint8_t preshared_key[WIREGUARD_SESSION_KEY_LEN];

	// Precomputed DH(Sprivi,Spubr) with device private key, and peer public key
	uint8_t public_key_dh[WIREGUARD_PUBLIC_KEY_LEN];

	// Session keypairs
	struct wireguard_keypair curr_keypair;
	struct wireguard_keypair prev_keypair;
	struct wireguard_keypair next_keypair;

	// 5.1 Silence is a Virtue: The responder keeps track of the greatest timestamp received per peer
	uint8_t greatest_timestamp[WIREGUARD_TAI64N_LEN];

	// The active handshake that is happening
	struct wireguard_handshake handshake;

	// The time of the latest completed handshake
	uint32_t latest_handshake_millis;

	// Decrypted cookie from the responder
	uint32_t cookie_millis;
	uint8_t cookie[WIREGUARD_COOKIE_LEN];

	// The latest mac1 we sent with initiation
	bool handshake_mac1_valid;
	uint8_t handshake_mac1[WIREGUARD_COOKIE_LEN];

	// Precomputed keys for use in mac validation
	uint8_t label_cookie_key[WIREGUARD_SESSION_KEY_LEN];
	uint8_t label_mac1_key[WIREGUARD_SESSION_KEY_LEN];

	// The last time we received a valid initiation message
	uint32_t last_initiation_rx;
	// The last time we sent an initiation message to this peer
	uint32_t last_initiation_tx;

	// last_tx and last_rx of data packets
	uint32_t last_tx;
	uint32_t last_rx;

	// We set this flag on RX/TX of packets if we think that we should initiate a new handshake
	bool send_handshake;
} wireguard_peer_t;

typedef struct wireguard_device {
	// Maybe have a "Device private" member to abstract these?
	struct netif *netif;
	struct udp_pcb *udp_pcb;

	struct netif *underlying_netif;

	uint8_t public_key[WIREGUARD_PUBLIC_KEY_LEN];
	uint8_t private_key[WIREGUARD_PRIVATE_KEY_LEN];

	uint8_t cookie_secret[WIREGUARD_HASH_LEN];
	uint32_t cookie_secret_millis;

	// Precalculated
 	uint8_t label_cookie_key[WIREGUARD_SESSION_KEY_LEN];
	uint8_t label_mac1_key[WIREGUARD_SESSION_KEY_LEN];

	// List of peers associated with this device
 	struct wireguard_peer peers[WIREGUARD_MAX_PEERS];

	bool valid;
} wireguard_device_t;

#define MESSAGE_INVALID					0
#define MESSAGE_HANDSHAKE_INITIATION	1
#define MESSAGE_HANDSHAKE_RESPONSE		2
#define MESSAGE_COOKIE_REPLY			3
#define MESSAGE_TRANSPORT_DATA			4


// 5.4.2 First Message: Initiator to Responder
typedef struct message_handshake_initiation {
	uint8_t type;
	uint8_t reserved[3];
	uint32_t sender;
	uint8_t ephemeral[32];
	uint8_t enc_static[32 + WIREGUARD_AUTHTAG_LEN];
	uint8_t enc_timestamp[WIREGUARD_TAI64N_LEN + WIREGUARD_AUTHTAG_LEN];
	uint8_t mac1[WIREGUARD_COOKIE_LEN];
	uint8_t mac2[WIREGUARD_COOKIE_LEN];
} __attribute__ ((__packed__)) message_handshake_initiation_t;

// 5.4.3 Second Message: Responder to Initiator
typedef struct message_handshake_response {
	uint8_t type;
	uint8_t reserved[3];
	uint32_t sender;
	uint32_t receiver;
	uint8_t ephemeral[32];
	uint8_t enc_empty[0 + WIREGUARD_AUTHTAG_LEN];
	uint8_t mac1[WIREGUARD_COOKIE_LEN];
	uint8_t mac2[WIREGUARD_COOKIE_LEN];
} __attribute__ ((__packed__)) message_handshake_response_t;

// 5.4.7 Under Load: Cookie Reply Message
typedef struct message_cookie_reply {
	uint8_t type;
	uint8_t reserved[3];
	uint32_t receiver;
	uint8_t nonce[COOKIE_NONCE_LEN];
	uint8_t enc_cookie[WIREGUARD_COOKIE_LEN + WIREGUARD_AUTHTAG_LEN];
} __attribute__ ((__packed__)) message_cookie_reply_t;

// 5.4.6 Subsequent Messages: Transport Data Messages
typedef struct message_transport_data {
	uint8_t type;
	uint8_t reserved[3];
	uint32_t receiver;
	uint8_t counter[8];
	// Followed by encrypted data
	uint8_t enc_packet[];
} __attribute__ ((__packed__)) message_transport_data_t;

// Initialise the WireGuard system - need to call this before anything else
void wireguard_init(void);
bool wireguard_device_init(struct wireguard_device *device, const uint8_t *private_key);
bool wireguard_peer_init(struct wireguard_device *device, wireguard_peer_t *peer, const uint8_t *public_key, const uint8_t *preshared_key);

wireguard_peer_t *wireguard_peer_alloc(wireguard_device_t *device);
uint8_t wireguard_peer_index(struct wireguard_device *device, wireguard_peer_t *peer);
wireguard_peer_t *wireguard_peer_lookup_by_pubkey(struct wireguard_device *device, uint8_t *public_key);
wireguard_peer_t *wireguard_peer_lookup_by_peer_index(struct wireguard_device *device, uint8_t peer_index);
wireguard_peer_t *wireguard_peer_lookup_by_receiver(struct wireguard_device *device, uint32_t receiver);
wireguard_peer_t *wireguard_peer_lookup_by_handshake(struct wireguard_device *device, uint32_t receiver);

void wireguard_start_session(wireguard_peer_t *peer, bool initiator);

void keypair_update(wireguard_peer_t *peer, wireguard_keypair_t *received_keypair);
void keypair_destroy(wireguard_keypair_t *keypair);

wireguard_keypair_t *get_peer_keypair_for_idx(wireguard_peer_t *peer, uint32_t idx);
bool wireguard_check_replay(wireguard_keypair_t *keypair, uint64_t seq);

uint8_t wireguard_get_message_type(const uint8_t *data, size_t len);

wireguard_peer_t *wireguard_process_initiation_message(struct wireguard_device *device, struct message_handshake_initiation *msg);
bool wireguard_process_handshake_response(struct wireguard_device *device, wireguard_peer_t *peer, struct message_handshake_response *src);
bool wireguard_process_cookie_message(struct wireguard_device *device, wireguard_peer_t *peer, struct message_cookie_reply *src);

bool wireguard_create_handshake_initiation(struct wireguard_device *device, wireguard_peer_t *peer, struct message_handshake_initiation *dst);
bool wireguard_create_handshake_response(struct wireguard_device *device, wireguard_peer_t *peer, struct message_handshake_response *dst);
void wireguard_create_cookie_reply(struct wireguard_device *device, struct message_cookie_reply *dst, const uint8_t *mac1, uint32_t index, uint8_t *source_addr_port, size_t source_length);


bool wireguard_check_mac1(struct wireguard_device *device, const uint8_t *data, size_t len, const uint8_t *mac1);
bool wireguard_check_mac2(struct wireguard_device *device, const uint8_t *data, size_t len, uint8_t *source_addr_port, size_t source_length, const uint8_t *mac2);

bool wireguard_expired(uint32_t created_millis, uint32_t valid_seconds);

void wireguard_encrypt_packet(uint8_t *dst, const uint8_t *src, size_t src_len, wireguard_keypair_t *keypair);
bool wireguard_decrypt_packet(uint8_t *dst, const uint8_t *src, size_t src_len, uint64_t counter, wireguard_keypair_t *keypair);

#ifdef __cplusplus
}
#endif

#endif /* _WIREGUARD_H_ */
