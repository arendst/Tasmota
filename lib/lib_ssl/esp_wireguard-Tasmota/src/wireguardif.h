/*
 * Copyright (c) 2021 Daniel Hope (www.floorsense.nz)
 * Copyright (c) 2023 Simone Rossetto <simros85@gmail.com>
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


#ifndef _WIREGUARDIF_H_
#define _WIREGUARDIF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_wireguard.h"

#include <time.h>
#include "lwip/arch.h"
#include "lwip/netif.h"
#include "lwip/ip_addr.h"

// Default MTU for WireGuard is 1420 bytes
#define WIREGUARDIF_MTU (1420)

#define WIREGUARDIF_DEFAULT_PORT		(51820)

typedef struct wireguardif_init_data {
	// Required: the private key of this WireGuard network interface
	wg_key_t private_key2;
	// Required: What UDP port to listen on
	uint16_t listen_port;
	// Optional: restrict send/receive of encapsulated WireGuard traffic to this network interface only (NULL to use routing table)
	struct netif *bind_netif;
} wireguardif_init_data_t;

typedef struct wireguardif_peer {
	wg_key_t public_key2;
	// Optional pre-shared key (32 bytes) - make sure this is NULL if not to be used
	wg_key_t preshared_key2;
	// tai64n of largest timestamp we have seen during handshake to avoid replays
	uint8_t greatest_timestamp[12];

	// Allowed ip/netmask (can add additional later but at least one is required)
	ip_addr_t allowed_ip;
	ip_addr_t allowed_mask;

	// End-point details (may be blank)
	ip_addr_t endpoint_ip;
	uint16_t endport_port;
	uint16_t keep_alive;
} wireguardif_peer_t;

#define WIREGUARDIF_INVALID_INDEX (0xFF)

// Initialise a new WireGuard network interface (netif)
err_t wireguardif_init(struct netif *netif);

// Helper to initialise the peer struct with defaults
void wireguardif_peer_init(wireguardif_peer_t *peer);

// Add a new peer to the specified interface - see wireguard.h for maximum number of peers allowed
// On success the peer_index can be used to reference this peer in future function calls
err_t wireguardif_add_peer(struct netif *netif, wireguardif_peer_t *peer, u8_t *peer_index);

// Remove the given peer from the network interface
err_t wireguardif_remove_peer(struct netif *netif, u8_t peer_index);

// Try and connect to the given peer
err_t wireguardif_connect(struct netif *netif, u8_t peer_index);

// Stop trying to connect to the given peer
err_t wireguardif_disconnect(struct netif *netif, u8_t peer_index);

// Shutdown the WireGuard interface
void wireguardif_shutdown(struct netif *netif);

// Finalize the WireGuard interface after the netif is removed
void wireguardif_fini(struct netif *netif);

// Is the given peer "up"? A peer is up if it has a valid session key it can communicate with
err_t wireguardif_peer_is_up(struct netif *netif, u8_t peer_index, ip_addr_t *current_ip, u16_t *current_port);

// Get timestamp of latest handshake (with seconds resolution)
// Return 0 if no handshake already done or in case of errors
time_t wireguardif_latest_handshake(struct netif *netif, u8_t peer_index);

// Add ip/mask to the list of allowed ips of the given peer
err_t wireguardif_add_allowed_ip(struct netif *netif, u8_t peer_index, const ip_addr_t& ip, const ip_addr_t& mask);

#ifdef __cplusplus
}
#endif

#endif /* _WIREGUARDIF_H_ */
