/*
 * Copyright (c) 2021 Daniel Hope (www.floorsense.nz)
 * Copyright (c) 2021 Kenta Ida (fuga@fugafuga.org)
 * Copyright (c) 2023-2024 Simone Rossetto <simros85@gmail.com>
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

#include "wireguardif.h"
#include <Arduino.h>
#include <IPAddress.h>

#include "lwip/netif.h"
#include "lwip/ip.h"
#include "lwip/udp.h"
#include "lwip/mem.h"
#include "lwip/sys.h"
#include "lwip/timeouts.h"

#include "esp_wireguard_err.h"

#ifdef ESP32
	#include "esp_netif.h"
#endif

#include "wireguard.h"
#include "crypto.h"

#define WIREGUARDIF_TIMER_MSECS 400

//**************************************************************************************************************
// enable AddLog support within a C++ library
extern void AddLog(uint32_t loglevel, PGM_P formatP, ...);
enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE};
uint32_t HighestLogLevel(void);
//**************************************************************************************************************


static void update_peer_addr(wireguard_peer_t *peer, const ip_addr_t *addr, u16_t port) {
	peer->ip = *addr;
	peer->port = port;
}

static wireguard_peer_t *peer_lookup_by_allowed_ip(wireguard_device_t *device, const ip_addr_t *ipaddr) {
	for (int32_t x=0; x < WIREGUARD_MAX_PEERS; x++) {
		// AddLog(LOG_LEVEL_DEBUG, ">>>: peer_lookup_by_allowed_ip peer %i (valid %i active %i) ip %_I", x, tmp->valid, tmp->active, ipaddr->u_addr.ip4.addr);
		if (device->peers[x].valid) {
			for (int32_t y=0; y < WIREGUARD_MAX_SRC_IPS; y++) {
				wireguard_allowed_ip *allowed_ip = &device->peers[x].allowed_source_ips[y];
				if (allowed_ip->valid) {
					// AddLog(LOG_LEVEL_DEBUG, "WG : filter ip#%i %_I ip %_I mask %_I", y, allowed_ip->ip.u_addr.ip4.addr, allowed_ip->mask.u_addr.ip4.addr, allowed_ip->mask.u_addr.ip4.addr);
					if (ip_addr_netcmp(ipaddr, &allowed_ip->ip, ip_2_ip4(&allowed_ip->mask))) {
						// AddLog(LOG_LEVEL_DEBUG, "WG : peer_lookup_by_allowed_ip MATCH");
						return &device->peers[x];
					}
				}
			}
		}
	}
	return NULL;
}

static bool wireguardif_can_send_initiation(wireguard_peer_t *peer) {
	return ((peer->last_initiation_tx == 0) || (wireguard_expired(peer->last_initiation_tx, REKEY_TIMEOUT)));
}

static err_t wireguardif_peer_output(struct netif *netif, struct pbuf *q, wireguard_peer_t *peer) {
	wireguard_device_t *device = (wireguard_device_t*) netif->state;
	// Send to last know port, not the connect port
	//TODO: Support DSCP and ECN - lwip requires this set on PCB globally, not per packet
	return udp_sendto_if(device->udp_pcb, q, &peer->ip, peer->port, device->underlying_netif);
}

static err_t wireguardif_device_output(wireguard_device_t *device, struct pbuf *q, const ip_addr_t *ipaddr, u16_t port) {
	return udp_sendto_if(device->udp_pcb, q, ipaddr, port, device->underlying_netif);
}

static err_t wireguardif_output_to_peer(struct netif *netif, struct pbuf *q, const ip_addr_t *ipaddr, wireguard_peer_t *peer) {
	// AddLog(LOG_LEVEL_DEBUG, ">>>: wireguardif_output_to_peer");
	// The LWIP IP layer wants to send an IP packet out over the interface - we need to encrypt and send it to the peer
	struct message_transport_data *hdr;
	struct pbuf *pbuf;
	err_t result;
	size_t unpadded_len;
	size_t padded_len;
	size_t header_len = 16;
	uint8_t *dst;
	uint32_t now;
	wireguard_keypair_t *keypair = &peer->curr_keypair;

	// Note: We may not be able to use the current keypair if we haven't received data, may need to resort to using previous keypair
	if (keypair->valid && (!keypair->initiator) && (keypair->last_rx == 0)) {
		keypair = &peer->prev_keypair;
	}

	if (keypair->valid && (keypair->initiator || keypair->last_rx != 0)) {

		if (
				!wireguard_expired(keypair->keypair_millis, REJECT_AFTER_TIME) &&
				(keypair->sending_counter < REJECT_AFTER_MESSAGES)
		) {

			// Calculate the outgoing packet size - round up to next 16 bytes, add 16 bytes for header
			if (q) {
				// This is actual transport data
				unpadded_len = q->tot_len;
			} else {
				// This is a keep-alive
				unpadded_len = 0;
			}
			padded_len = (unpadded_len + 15) & 0xFFFFFFF0; // Round up to next 16 byte boundary

			// The buffer needs to be allocated from "transport" pool to leave room for LwIP generated IP headers
			// The IP packet consists of 16 byte header (struct message_transport_data), data padded upto 16 byte boundary + encrypted auth tag (16 bytes)
			pbuf = pbuf_alloc(PBUF_TRANSPORT, header_len + padded_len + WIREGUARD_AUTHTAG_LEN, PBUF_RAM);
			if (pbuf) {
				// Note: allocating pbuf from RAM above guarantees that the pbuf is in one section and not chained
				// - i.e payload points to the contiguous memory region
				memset(pbuf->payload, 0, pbuf->tot_len);

				hdr = (struct message_transport_data *)pbuf->payload;

				hdr->type = MESSAGE_TRANSPORT_DATA;
				hdr->receiver = keypair->remote_index;
				// Alignment required... pbuf_alloc has probably aligned data, but want to be sure
				U64TO8_LITTLE(hdr->counter, keypair->sending_counter);

				// Copy the encrypted (padded) data to the output packet - chacha20poly1305_encrypt() can encrypt data in-place which avoids call to mem_malloc
				dst = &hdr->enc_packet[0];
				if ((padded_len > 0) && q) {
					// Note: before copying make sure we have inserted the IP header checksum
					// The IP header checksum (and other checksums in the IP packet - e.g. ICMP) need to be calculated by LWIP before calling
					// The Wireguard interface always needs checksums to be generated in software but the base netif may have some checksums generated by hardware

					// Copy pbuf to memory - handles case where pbuf is chained
					pbuf_copy_partial(q, dst, unpadded_len, 0);
				}

				// Then encrypt
				struct ip_hdr *iphdr = (struct ip_hdr *)dst;
				// AddLog(LOG_LEVEL_DEBUG, "WG : Send inner addr src %_I dest %_I proto %i %*_H", iphdr->src, iphdr->dest, iphdr->_proto, padded_len, dst);

				wireguard_encrypt_packet(dst, dst, padded_len, keypair);

				result = wireguardif_peer_output(netif, pbuf, peer);
				// AddLog(LOG_LEVEL_DEBUG, "WG : send packet to %_I:%i result %i", peer->ip.u_addr.ip4.addr, peer->port, result);

				if (result == ERR_OK) {
					now = wireguard_sys_now();
					peer->last_tx = now;
					keypair->last_tx = now;
				}

				pbuf_free(pbuf);

				// Check to see if we should rekey
				if (keypair->sending_counter >= REKEY_AFTER_MESSAGES) {
					peer->send_handshake = true;
				} else if (keypair->initiator && wireguard_expired(keypair->keypair_millis, REKEY_AFTER_TIME)) {
					peer->send_handshake = true;
				}

			} else {
				// Failed to allocate memory
				result = ERR_MEM;
			}
		} else {
			// key has expired...
			keypair_destroy(keypair);
			result = ERR_CONN;
		}
	} else {
		// No valid keys!
		result = ERR_CONN;
	}
	// AddLog(LOG_LEVEL_DEBUG, ">>>: OUT wireguardif_output_to_peer err %d", result);
	return result;
}

// This is used as the output function for the Wireguard netif
// The ipaddr here is the one inside the VPN which we use to lookup the correct peer/endpoint
static err_t wireguardif_output(struct netif *netif, struct pbuf *q, const ip4_addr_t *ip4addr) {
	// AddLog(LOG_LEVEL_DEBUG, ">>>: wireguardif_output state %p", netif->state);
	if (netif->state == NULL) {
		// maybe the underlying interface has already been deactivated?
		return ERR_IF;
	}
	wireguard_device_t *device = (wireguard_device_t *)netif->state;
	ip_addr_t ipaddr;

	if (!device) {
		// AddLog(LOG_LEVEL_INFO, ">>>: wireguardif_output NULL device");
		return ERR_RTE;
	}

	// Send to peer that matches dest IP
	ip_addr_copy_from_ip4(ipaddr, *ip4addr);
	wireguard_peer_t *peer = peer_lookup_by_allowed_ip(device, &ipaddr);
	// AddLog(LOG_LEVEL_DEBUG, ">>>: wireguardif_output ipaddr %s peer %p", IPAddress(&ipaddr).toString().c_str(), peer);
	if (peer) {
		return wireguardif_output_to_peer(netif, q, &ipaddr, peer);
	} else {
		return ERR_RTE;
	}
}

static void wireguardif_send_keepalive(wireguard_device_t *device, wireguard_peer_t *peer) {
	if (HighestLogLevel() >= LOG_LEVEL_DEBUG) {
		AddLog(LOG_LEVEL_DEBUG, PSTR("WG : Sending keep-alive"));
	}
	// Send a NULL packet as a keep-alive
	wireguardif_output_to_peer(device->netif, NULL, NULL, peer);
}

static void wireguardif_process_response_message(wireguard_device_t *device, wireguard_peer_t *peer, struct message_handshake_response *response, const ip_addr_t *addr, u16_t port) {
	// AddLog(LOG_LEVEL_DEBUG, ">>>: wireguardif_process_response_message");
	if (wireguard_process_handshake_response(device, peer, response)) {
		// Packet is good
		// Update the peer location
		update_peer_addr(peer, addr, port);

		wireguard_start_session(peer, true);
		wireguardif_send_keepalive(device, peer);

		// Set the IF-UP flag on netif
		netif_set_link_up(device->netif);
	} else {
		// Packet bad
	}
}

static bool peer_add_ip(wireguard_peer_t *peer, const ip_addr_t& ip, const ip_addr_t& mask) {
	bool result = false;
	struct wireguard_allowed_ip *allowed;
	int x;
	// Look for existing match first
	for (x=0; x < WIREGUARD_MAX_SRC_IPS; x++) {
		allowed = &peer->allowed_source_ips[x];
		if ((allowed->valid) && ip_addr_cmp(&allowed->ip, &ip) && ip_addr_cmp(&allowed->mask, &mask)) {
			result = true;
			break;
		}
	}
	if (!result) {
		// Look for a free slot
		for (x=0; x < WIREGUARD_MAX_SRC_IPS; x++) {
			allowed = &peer->allowed_source_ips[x];
			if (!allowed->valid) {
				allowed->valid = true;
				allowed->ip = ip;
				allowed->mask = mask;
				result = true;
				break;
			}
		}
	}
	return result;
}

static void wireguardif_process_data_message(wireguard_device_t *device, wireguard_peer_t *peer, struct message_transport_data *data_hdr, size_t data_len, const ip_addr_t *addr, u16_t port) {
	wireguard_keypair_t *keypair;
	uint64_t nonce;
	uint8_t *src;
	size_t src_len;
	struct pbuf *pbuf;
	struct ip_hdr *iphdr;
	ip_addr_t dest;
	bool dest_ok = false;
	int x;
	uint32_t now;
	uint16_t header_len = 0xFFFF;
	uint32_t idx = data_hdr->receiver;

	keypair = get_peer_keypair_for_idx(peer, idx);

	if (keypair) {
		if (
				(keypair->receiving_valid) &&
				!wireguard_expired(keypair->keypair_millis, REJECT_AFTER_TIME) &&
				(keypair->sending_counter < REJECT_AFTER_MESSAGES)

		) {

			nonce = U8TO64_LITTLE(data_hdr->counter);
			src = &data_hdr->enc_packet[0];
			src_len = data_len;

			// We don't know the unpadded size until we have decrypted the packet and validated/inspected the IP header
			pbuf = pbuf_alloc(PBUF_TRANSPORT, src_len - WIREGUARD_AUTHTAG_LEN, PBUF_RAM);
			if (pbuf) {
				// Decrypt the packet
				memset(pbuf->payload, 0, pbuf->tot_len);
				if (wireguard_decrypt_packet((uint8_t*)pbuf->payload, src, src_len, nonce, keypair)) {

					// 3. Since the packet has authenticated correctly, the source IP of the outer UDP/IP packet is used to update the endpoint for peer TrMv...WXX0.
					// Update the peer location
					update_peer_addr(peer, addr, port);
					// AddLog(LOG_LEVEL_DEBUG, "WG : received (%_I:%i) %*_H", addr->u_addr.ip4.addr, port, src_len, pbuf->payload);

					now = wireguard_sys_now();
					keypair->last_rx = now;
					peer->last_rx = now;

					// Might need to shuffle next key --> current keypair
					keypair_update(peer, keypair);

					// Check to see if we should rekey
					if (keypair->initiator && wireguard_expired(keypair->keypair_millis, REJECT_AFTER_TIME - peer->keepalive_interval - REKEY_TIMEOUT)) {
						peer->send_handshake = true;
					}

					// Make sure that link is reported as up
					netif_set_link_up(device->netif);

					if (pbuf->tot_len > 0) {
						//4a. Once the packet payload is decrypted, the interface has a plaintext packet. If this is not an IP packet, it is dropped.
						iphdr = (struct ip_hdr *)pbuf->payload;
						// Check for packet replay / dupes
						if (wireguard_check_replay(keypair, nonce)) {

							// 4b. Otherwise, WireGuard checks to see if the source IP address of the plaintext inner-packet routes correspondingly in the cryptokey routing table
							// Also check packet length!
#if LWIP_IPV4
							if (IPH_V(iphdr) == 4) {
								ip_addr_copy_from_ip4(dest, iphdr->dest);
								// AddLog(LOG_LEVEL_DEBUG, "WG : inner addr src %_I dest %_I proto %i", iphdr->src, iphdr->dest, iphdr->_proto);
								for (x=0; x < WIREGUARD_MAX_SRC_IPS; x++) {
									if (peer->allowed_source_ips[x].valid) {
										if (ip_addr_netcmp(&dest, &peer->allowed_source_ips[x].ip, ip_2_ip4(&peer->allowed_source_ips[x].mask))) {
											dest_ok = true;
											header_len = PP_NTOHS(IPH_LEN(iphdr));
											break;
										}
									}
								}
							}
#endif /* LWIP_IPV4 */
#if LWIP_IPV6
							if (IPH_V(iphdr) == 6) {
								// TODO: IPV6 support for route filtering
								header_len = PP_NTOHS(IPH_LEN(iphdr));
								dest_ok = true;
							}
#endif /* LWIP_IPV6 */
							if (header_len <= pbuf->tot_len) {

								// 5. If the plaintext packet has not been dropped, it is inserted into the receive queue of the wg0 interface.
								if (dest_ok) {
									// Send packet to be process by LWIP
									ip_input(pbuf, device->netif);
									// pbuf is owned by IP layer now
									pbuf = NULL;
								}
							} else {
								// IP header is corrupt or lied about packet size
							}
						} else {
							// AddLog(LOG_LEVEL_DEBUG, "WG : Duplicate packet");
							// This is a duplicate packet / replayed / too far out of order
						}
					} else {
						if (HighestLogLevel() >= LOG_LEVEL_DEBUG) {
							AddLog(LOG_LEVEL_DEBUG, PSTR("WG : Received keep-alive"));
						}
						// This was a keep-alive packet
					}
				}

				if (pbuf) {
					pbuf_free(pbuf);
				}
			}


		} else {
			//After Reject-After-Messages transport data messages or after the current secure session is Reject- After-Time seconds old,
			// whichever comes first, WireGuard will refuse to send or receive any more transport data messages using the current secure session,
			// until a new secure session is created through the 1-RTT handshake
			keypair_destroy(keypair);
		}

	} else {
		// Could not locate valid keypair for remote index
	}
}

static struct pbuf *wireguardif_initiate_handshake(wireguard_device_t *device, wireguard_peer_t *peer, struct message_handshake_initiation *msg, err_t *error) {
	// AddLog(LOG_LEVEL_INFO, ">>>: wireguardif_initiate_handshake");
	struct pbuf *pbuf = NULL;
	err_t err = ERR_OK;
	if (wireguard_create_handshake_initiation(device, peer, msg)) {
		// AddLog(LOG_LEVEL_DEBUG, ">>>: sending initiation packet");
		pbuf = pbuf_alloc(PBUF_TRANSPORT, sizeof(struct message_handshake_initiation), PBUF_RAM);
		if (pbuf) {
			err = pbuf_take(pbuf, msg, sizeof(struct message_handshake_initiation));
			if (err == ERR_OK) {
				// OK!
			} else {
				pbuf_free(pbuf);
				pbuf = NULL;
			}
		} else {
			err = ERR_MEM;
		}
	} else {
		err = ERR_ARG;
	}
	if (error) {
		*error = err;
	}
	// AddLog(LOG_LEVEL_INFO, ">>>: OUT wireguardif_initiate_handshake pbuf %p", pbuf);
	return pbuf;
}

static void wireguardif_send_handshake_response(wireguard_device_t *device, wireguard_peer_t *peer) {
	// AddLog(LOG_LEVEL_DEBUG, ">>>: wireguardif_send_handshake_response");
	struct message_handshake_response packet;
	struct pbuf *pbuf = NULL;
	err_t err = ERR_OK;

	if (wireguard_create_handshake_response(device, peer, &packet)) {

		wireguard_start_session(peer, false);

		// AddLog(LOG_LEVEL_DEBUG, "WG : sending handshake response packet");
		pbuf = pbuf_alloc(PBUF_TRANSPORT, sizeof(struct message_handshake_response), PBUF_RAM);
		if (pbuf) {
			err = pbuf_take(pbuf, &packet, sizeof(struct message_handshake_response));
			if (err == ERR_OK) {
				// OK!
				wireguardif_peer_output(device->netif, pbuf, peer);
			}
			pbuf_free(pbuf);
		}
	}
}

static size_t get_source_addr_port(const ip_addr_t *addr, u16_t port, uint8_t *buf, size_t buflen) {
	size_t result = 0;

#if LWIP_IPV4
	if (IP_IS_V4(addr) && (buflen >= 4)) {
		U32TO8_BIG(buf + result, PP_NTOHL(ip4_addr_get_u32(ip_2_ip4(addr))));
		result += 4;
	}
#endif
#if LWIP_IPV6
	if (IP_IS_V6(addr) && (buflen >= 16)) {
		U16TO8_BIG(buf + result + 0, IP6_ADDR_BLOCK1(ip_2_ip6(addr)));
		U16TO8_BIG(buf + result + 2, IP6_ADDR_BLOCK2(ip_2_ip6(addr)));
		U16TO8_BIG(buf + result + 4, IP6_ADDR_BLOCK3(ip_2_ip6(addr)));
		U16TO8_BIG(buf + result + 6, IP6_ADDR_BLOCK4(ip_2_ip6(addr)));
		U16TO8_BIG(buf + result + 8, IP6_ADDR_BLOCK5(ip_2_ip6(addr)));
		U16TO8_BIG(buf + result + 10, IP6_ADDR_BLOCK6(ip_2_ip6(addr)));
		U16TO8_BIG(buf + result + 12, IP6_ADDR_BLOCK7(ip_2_ip6(addr)));
		U16TO8_BIG(buf + result + 14, IP6_ADDR_BLOCK8(ip_2_ip6(addr)));
		result += 16;
	}
#endif
	if (buflen >= result + 2) {
		U16TO8_BIG(buf + result, port);
		result += 2;
	}
	return result;
}

static void wireguardif_send_handshake_cookie(wireguard_device_t *device, const uint8_t *mac1, uint32_t index, const ip_addr_t *addr, u16_t port) {
	// AddLog(LOG_LEVEL_DEBUG, ">>>: wireguardif_send_handshake_cookie");
	struct message_cookie_reply packet;
	struct pbuf *pbuf = NULL;
	err_t err = ERR_OK;
	uint8_t source_buf[18];
	size_t source_len = get_source_addr_port(addr, port, source_buf, sizeof(source_buf));

	wireguard_create_cookie_reply(device, &packet, mac1, index, source_buf, source_len);

	// Send this packet out!
	pbuf = pbuf_alloc(PBUF_TRANSPORT, sizeof(struct message_cookie_reply), PBUF_RAM);
	if (pbuf) {
		err = pbuf_take(pbuf, &packet, sizeof(struct message_cookie_reply));
		if (err == ERR_OK) {
			wireguardif_device_output(device, pbuf, addr, port);
		}
		pbuf_free(pbuf);
	}
}

static bool wireguardif_check_initiation_message(wireguard_device_t *device, struct message_handshake_initiation *msg, const ip_addr_t *addr, u16_t port) {
	// AddLog(LOG_LEVEL_DEBUG, ">>>: wireguardif_check_initiation_message");
	bool result = false;
	uint8_t *data = (uint8_t *)msg;
	uint8_t source_buf[18];
	size_t source_len;
	// We received an initiation packet check it is valid

	if (wireguard_check_mac1(device, data, sizeof(struct message_handshake_initiation) - (2 * WIREGUARD_COOKIE_LEN), msg->mac1)) {
		// mac1 is valid!
		result = true;
		// skip the following since wireguard_is_under_load() is always false
		// if (wireguard_is_under_load()) {
		// 	// If we are under load then check mac2
		// 	source_len = get_source_addr_port(addr, port, source_buf, sizeof(source_buf));

		// 	result = wireguard_check_mac2(device, data, sizeof(struct message_handshake_initiation) - (WIREGUARD_COOKIE_LEN), source_buf, source_len, msg->mac2);

		// 	if (!result) {
		// 		// mac2 is invalid (cookie may have expired) or not present
		// 		// 5.3 Denial of Service Mitigation & Cookies
		// 		// If the responder receives a message with a valid msg.mac1 yet with an invalid msg.mac2, and is under load, it may respond with a cookie reply message
		// 		wireguardif_send_handshake_cookie(device, msg->mac1, msg->sender, addr, port);
		// 	}
		// }

	} else {
		// mac1 is invalid
	}
	return result;
}

static bool wireguardif_check_response_message(wireguard_device_t *device, struct message_handshake_response *msg, const ip_addr_t *addr, u16_t port) {
	bool result = false;
	uint8_t *data = (uint8_t *)msg;
	uint8_t source_buf[18];
	size_t source_len;
	// We received an initiation packet check it is valid

	if (wireguard_check_mac1(device, data, sizeof(struct message_handshake_response) - (2 * WIREGUARD_COOKIE_LEN), msg->mac1)) {
		// mac1 is valid!
		result = true;
		// skip the following since wireguard_is_under_load() is always false
		// if (wireguard_is_under_load()) {
		// 	// If we are under load then check mac2
		// 	source_len = get_source_addr_port(addr, port, source_buf, sizeof(source_buf));

		// 	result = wireguard_check_mac2(device, data, sizeof(struct message_handshake_response) - (WIREGUARD_COOKIE_LEN), source_buf, source_len, msg->mac2);

		// 	// AddLog(LOG_LEVEL_DEBUG, ">>>: wireguardif_check_response_message MAC2 %d (1==SUCCESS)", result);
		// 	if (!result) {
		// 		// mac2 is invalid (cookie may have expired) or not present
		// 		// 5.3 Denial of Service Mitigation & Cookies
		// 		// If the responder receives a message with a valid msg.mac1 yet with an invalid msg.mac2, and is under load, it may respond with a cookie reply message
		// 		wireguardif_send_handshake_cookie(device, msg->mac1, msg->sender, addr, port);
		// 	}
		// }

	} else {
		// mac1 is invalid
	}
	return result;
}


void wireguardif_network_rx(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port) {
	// LWIP_ASSERT("wireguardif_network_rx: invalid arg", arg != NULL);
	// LWIP_ASSERT("wireguardif_network_rx: invalid pbuf", p != NULL);
	// We have received a packet from the base_netif to our UDP port - process this as a possible Wireguard packet
	wireguard_device_t *device = (wireguard_device_t *)arg;
	wireguard_peer_t *peer;
	uint8_t *data = (uint8_t*) p->payload;
	size_t len = p->len; // This buf, not chained ones

	struct message_handshake_initiation *msg_initiation;
	struct message_handshake_response *msg_response;
	struct message_cookie_reply *msg_cookie;
	struct message_transport_data *msg_data;

	uint8_t type = wireguard_get_message_type(data, len);

	switch (type) {
		case MESSAGE_HANDSHAKE_INITIATION:
			// AddLog(LOG_LEVEL_DEBUG, "");
			AddLog(LOG_LEVEL_DEBUG, PSTR("WG : RX MESSAGE_HANDSHAKE_INITIATION"));
			msg_initiation = (struct message_handshake_initiation *)data;

			// Check mac1 (and optionally mac2) are correct - note it may internally generate a cookie reply packet
			if (wireguardif_check_initiation_message(device, msg_initiation, addr, port)) {

				peer = wireguard_process_initiation_message(device, msg_initiation);
				if (peer) {
					// Update the peer location
					update_peer_addr(peer, addr, port);

					// Send back a handshake response
					wireguardif_send_handshake_response(device, peer);
				}
			}
			break;

		case MESSAGE_HANDSHAKE_RESPONSE:
			// AddLog(LOG_LEVEL_DEBUG, "");
			AddLog(LOG_LEVEL_DEBUG, PSTR("WG : RX MESSAGE_HANDSHAKE_RESPONSE from %s:%i"), IPAddress(addr).toString().c_str(), port);
			msg_response = (struct message_handshake_response *)data;

			// Check mac1 (and optionally mac2) are correct - note it may internally generate a cookie reply packet
			if (wireguardif_check_response_message(device, msg_response, addr, port)) {

				peer = wireguard_peer_lookup_by_handshake(device, msg_response->receiver);
				// AddLog(LOG_LEVEL_DEBUG, ">>>: RX MESSAGE_HANDSHAKE_RESPONSE peer %p", peer);
				if (peer) {
					// Process the handshake response
					wireguardif_process_response_message(device, peer, msg_response, addr, port);
				}
			}
			break;

		case MESSAGE_COOKIE_REPLY:
			msg_cookie = (struct message_cookie_reply *)data;
			peer = wireguard_peer_lookup_by_handshake(device, msg_cookie->receiver);
			// AddLog(LOG_LEVEL_DEBUG, "");
			AddLog(LOG_LEVEL_DEBUG, PSTR("WG : RX MESSAGE_COOKIE_REPLY peer=%p"), peer);
			if (peer) {
				if (wireguard_process_cookie_message(device, peer, msg_cookie)) {
					// Update the peer location
					update_peer_addr(peer, addr, port);

					// Don't send anything out - we stay quiet until the next initiation message
				}
			}
			break;

		case MESSAGE_TRANSPORT_DATA:
			// AddLog(LOG_LEVEL_DEBUG, "");
			msg_data = (struct message_transport_data *)data;
			peer = wireguard_peer_lookup_by_receiver(device, msg_data->receiver);
			AddLog(LOG_LEVEL_DEBUG, PSTR("WG : RX MESSAGE_TRANSPORT_DATA from %s"), IPAddress(addr).toString().c_str());
			if (peer) {
				// header is 16 bytes long so take that off the length
				wireguardif_process_data_message(device, peer, msg_data, len - 16, addr, port);
			}
			break;

		default:
			// AddLog(LOG_LEVEL_DEBUG, "");
			AddLog(LOG_LEVEL_DEBUG, PSTR("WG : RX UNKNOWN"));
			// Unknown or bad packet header
			break;
	}
	// Release data!
	pbuf_free(p);
}

static err_t wireguard_start_handshake(struct netif *netif, wireguard_peer_t *peer) {
	// AddLog(LOG_LEVEL_INFO, ">>>: wireguard_start_handshake");
	wireguard_device_t *device = (wireguard_device_t *)netif->state;
	err_t result;
	struct pbuf *pbuf;
	struct message_handshake_initiation msg;

	// AddLog(LOG_LEVEL_DEBUG, ">>>: starting handshake");
	pbuf = wireguardif_initiate_handshake(device, peer, &msg, &result);
	if (pbuf) {
		// AddLog(LOG_LEVEL_INFO, ">>>: wireguard_start_handshake ready to send packet");
		result = wireguardif_peer_output(netif, pbuf, peer);
		if (result != ERR_OK) {
			// AddLog(LOG_LEVEL_DEBUG, ">>>: wireguardif_peer_output: %i", result);
		}
		pbuf_free(pbuf);
		peer->send_handshake = false;
		peer->last_initiation_tx = wireguard_sys_now();
		memcpy(peer->handshake_mac1, msg.mac1, WIREGUARD_COOKIE_LEN);
		peer->handshake_mac1_valid = true;
	}
	// AddLog(LOG_LEVEL_INFO, ">>>: OUT wireguard_start_handshake result %d", result);
	return result;
}

static err_t wireguardif_lookup_peer(struct netif *netif, u8_t peer_index, wireguard_peer_t **out) {
	// LWIP_ASSERT("netif != NULL", (netif != NULL));
	// LWIP_ASSERT("state != NULL", (netif->state != NULL));
	wireguard_device_t *device = (wireguard_device_t *)netif->state;
	wireguard_peer_t *peer = NULL;
	err_t result;

	if (device->valid) {
		peer = wireguard_peer_lookup_by_peer_index(device, peer_index);
		if (peer) {
			result = ERR_OK;
		} else {
			AddLog(LOG_LEVEL_DEBUG, PSTR("WG : wireguard_peer_lookup_by_peer_index: peer not found"));
			result = ERR_ARG;
		}
	} else {
		AddLog(LOG_LEVEL_DEBUG, PSTR("WG : wireguardif_lookup_peer: invalid device"));
		result = ERR_ARG;
	}
	*out = peer;
	return result;
}

err_t wireguardif_connect(struct netif *netif, u8_t peer_index) {
	// AddLog(LOG_LEVEL_DEBUG, ">>>: wireguardif_connect %d", peer_index);
	wireguard_peer_t *peer;
	err_t result = wireguardif_lookup_peer(netif, peer_index, &peer);
	if (result == ERR_OK) {
		// AddLog(LOG_LEVEL_DEBUG, ">>>: wireguardif_connect lookup ok");
		// Check that a valid connect ip and port have been set
		if (!ip_addr_isany(&peer->connect_ip) && (peer->connect_port > 0)) {
			// Set the flag that we want to try connecting
			peer->active = true;
			peer->ip = peer->connect_ip;
			peer->port = peer->connect_port;
			result = ERR_OK;
		} else {
			result = ERR_ARG;
		}
	}
	// AddLog(LOG_LEVEL_DEBUG, ">>>: OUT wireguardif_connect %d", result);
	return result;
}

err_t wireguardif_disconnect(struct netif *netif, u8_t peer_index) {
	wireguard_peer_t *peer;
	err_t result = wireguardif_lookup_peer(netif, peer_index, &peer);
	if (result == ERR_OK) {
		// Set the flag that we want to try connecting
		peer->active = false;
		// Wipe out current keys
		keypair_destroy(&peer->next_keypair);
		keypair_destroy(&peer->curr_keypair);
		keypair_destroy(&peer->prev_keypair);
		result = ERR_OK;
	}
	return result;
}

err_t wireguardif_peer_is_up(struct netif *netif, u8_t peer_index, ip_addr_t *current_ip, u16_t *current_port) {
	wireguard_peer_t *peer;
	err_t result = wireguardif_lookup_peer(netif, peer_index, &peer);
	if (result == ERR_OK) {
		if ((peer->curr_keypair.valid) || (peer->prev_keypair.valid)) {
			result = ERR_OK;
		} else {
			// AddLog(LOG_LEVEL_DEBUG, "WG : wireguardif_peer_is_up: invalid keypairs");
			result = ERR_CONN;
		}
		if (current_ip) {
			*current_ip = peer->ip;
		}
		if (current_port) {
			*current_port = peer->port;
		}
	}
	return result;
}

time_t wireguardif_latest_handshake(struct netif *netif, u8_t peer_index) {
	time_t result = 0;
	wireguard_peer_t *peer;
	err_t err = wireguardif_lookup_peer(netif, peer_index, &peer);
	if (err == ERR_OK) {
		if (peer->valid && peer->latest_handshake_millis > 0) {
			/*
			 * The time() function returns the current timestamp (seconds since epoch),
			 * the wireguard_sys_now() function returns milliseconds since device boot up,
			 * so their difference is the timestamp (since epoch) of device boot time,
			 * so the latest handshake (saved executing wireguard_sys_now) plus timestamp
			 * of device boot time is the timestamp (since epoch) of the latest
			 * completed handshake. With ~1 second precision.
			 */
			result = (peer->latest_handshake_millis / 1000) + (time(NULL) - (wireguard_sys_now() / 1000));
		} else {
			AddLog(LOG_LEVEL_DEBUG, PSTR("WG : wireguardif_latest_handshake: valid=%ld, lhs=%ld"), (long) peer->valid, (long) peer->latest_handshake_millis);
		}
	}
	return result;
}

err_t wireguardif_add_allowed_ip(struct netif *netif, u8_t peer_index, const ip_addr_t& ip, const ip_addr_t& mask) {
	wireguard_peer_t *peer;
	err_t result = wireguardif_lookup_peer(netif, peer_index, &peer);
	if (result == ERR_OK) {
		if(peer_add_ip(peer, ip, mask)) {
			result = ERR_OK;
		} else {
			result = ERR_MEM;
		}
	}
	return result;
}

err_t wireguardif_remove_peer(struct netif *netif, u8_t peer_index) {
	wireguard_peer_t *peer;
	err_t result = wireguardif_lookup_peer(netif, peer_index, &peer);
	if (result == ERR_OK) {
		crypto_zero(peer, sizeof(wireguard_peer_t));
		peer->valid = false;
		result = ERR_OK;
	}
	return result;
}

err_t wireguardif_add_peer(struct netif *netif, wireguardif_peer_t *p, u8_t *peer_index) {
	wireguard_device_t *device = (wireguard_device_t *)netif->state;
	err_t result;
	// uint8_t public_key[WIREGUARD_PUBLIC_KEY_LEN];
	wireguard_peer_t *peer = NULL;

	uint32_t t1 = wireguard_sys_now();
    // AddLog(LOG_LEVEL_DEBUG, ">>>: wireguardif_add_peer now %d", t1);


	// size_t len = decode_base64_length((unsigned char*)p->public_key);
	// if (len == WIREGUARD_PUBLIC_KEY_LEN) {
	// 	len = decode_base64((unsigned char*)p->public_key, (unsigned char*)public_key);
	// 	// AddLog(LOG_LEVEL_DEBUG, ">>>: wireguardif_add_peer public_key %*_H", WIREGUARD_PUBLIC_KEY_LEN, public_key);
	// 	// See if the peer is already registered
	peer = wireguard_peer_lookup_by_pubkey(device, p->public_key2);
	// AddLog(LOG_LEVEL_DEBUG, ">>>: wireguardif_add_peer peer %p", peer);
	if (!peer) {
		// Not active - see if we have room to allocate a new one
		peer = wireguard_peer_alloc(device);
		if (peer) {

			if (wireguard_peer_init(device, peer, p->public_key2, p->preshared_key2)) {

				peer->connect_ip = p->endpoint_ip;
				peer->connect_port = p->endport_port;
				peer->ip = peer->connect_ip;
				peer->port = peer->connect_port;
				peer->keepalive_interval = p->keep_alive;
				peer_add_ip(peer, p->allowed_ip, p->allowed_mask);
				memcpy(peer->greatest_timestamp, p->greatest_timestamp, sizeof(peer->greatest_timestamp));

				result = ERR_OK;
			} else {
				result = ERR_ARG;
			}
		} else {
			result = ERR_MEM;
		}
	}
	// 	} else {
	// 		result = ERR_OK;
	// 	}
	// } else {
	// 	result = ERR_BUF;
	// }

	uint32_t t2 = wireguard_sys_now();
	// AddLog(LOG_LEVEL_DEBUG, ">>>: adding took %" PRIu32 "ms", (t2-t1));

	if (peer_index) {
		if (peer) {
			*peer_index = wireguard_peer_index(device, peer);
		} else {
			*peer_index = WIREGUARDIF_INVALID_INDEX;
		}
	}
    // AddLog(LOG_LEVEL_DEBUG, ">>>: OUT wireguardif_add_peer %d", result);
	return result;
}

static bool should_send_initiation(wireguard_peer_t *peer) {
	bool result = false;
	if (wireguardif_can_send_initiation(peer)) {
		if (peer->send_handshake) {
			result = true;
		} else if (peer->curr_keypair.valid && !peer->curr_keypair.initiator && wireguard_expired(peer->curr_keypair.keypair_millis, REJECT_AFTER_TIME - peer->keepalive_interval)) {
			result = true;
		} else if (!peer->curr_keypair.valid && peer->active) {
			result = true;
		}
	}
	return result;
}

static bool should_send_keepalive(wireguard_peer_t *peer) {
	bool result = false;
	if (peer->keepalive_interval > 0) {
		if ((peer->curr_keypair.valid) || (peer->prev_keypair.valid)) {
			if (wireguard_expired(peer->last_tx, peer->keepalive_interval)) {
				result = true;
			}
		}
	}
	return result;
}

static bool should_destroy_current_keypair(wireguard_peer_t *peer) {
	bool result = false;
	if (peer->curr_keypair.valid &&
			(wireguard_expired(peer->curr_keypair.keypair_millis, REJECT_AFTER_TIME) ||
			(peer->curr_keypair.sending_counter >= REJECT_AFTER_MESSAGES))
		) {
		result = true;
	}
	return result;
}

static bool should_reset_peer(wireguard_peer_t *peer) {
	bool result = false;
	if (peer->curr_keypair.valid && (wireguard_expired(peer->curr_keypair.keypair_millis, REJECT_AFTER_TIME * 3))) {
		result = true;
	}
	return result;
}

static void wireguardif_tmr(void *arg) {
	wireguard_device_t *device = (wireguard_device_t *)arg;
	wireguard_peer_t *peer;
	// Reschedule this timer
	sys_timeout(WIREGUARDIF_TIMER_MSECS, wireguardif_tmr, device);

	// Check periodic things
	bool link_up = false;
	for (int32_t x=0; x < WIREGUARD_MAX_PEERS; x++) {
		peer = &device->peers[x];
		if (peer->valid) {
			// Do we need to rekey / send a handshake?
			if (should_reset_peer(peer)) {
				// Nothing back for too long - we should wipe out all crypto state
				keypair_destroy(&peer->next_keypair);
				keypair_destroy(&peer->curr_keypair);
				keypair_destroy(&peer->prev_keypair);
				// TODO: Also destroy handshake?

				// Revert back to default IP/port if these were altered
				peer->ip = peer->connect_ip;
				peer->port = peer->connect_port;
			}
			if (should_destroy_current_keypair(peer)) {
				// Destroy current keypair
				keypair_destroy(&peer->curr_keypair);
			}
			if (should_send_keepalive(peer)) {
				wireguardif_send_keepalive(device, peer);
			}
			if (should_send_initiation(peer)) {
				wireguard_start_handshake(device->netif, peer);
			}

			if ((peer->curr_keypair.valid) || (peer->prev_keypair.valid)) {
				link_up = true;
			}
		}
	}

	if (!link_up) {
		// Clear the IF-UP flag on netif
		netif_set_link_down(device->netif);
	}
}


err_t wireguardif_init(struct netif *netif) {
	err_t result;
	esp_err_t err = ESP_FAIL;
	struct wireguardif_init_data *init_data;
	wireguard_device_t *device;
	struct udp_pcb *udp;

	struct netif* underlying_netif = NULL;

#ifdef ESP32
	char lwip_netif_name[8] = {0,};

	// list of interfaces to try to bind wireguard to
	static const char* ifkeys[3] = {"ETH_DEF", "WIFI_STA_DEF", "PPP_DEF"};

	// ifkey will contain the selected interface key
	const char* ifkey = NULL;

	// AddLog(LOG_LEVEL_DEBUG, "WG : looking for available network interface");
	for (int32_t i = 0; i < sizeof(ifkeys) / sizeof(char *) && err != ESP_OK; i++) {
		ifkey = ifkeys[i];
		if (esp_netif_is_netif_up(esp_netif_get_handle_from_ifkey(ifkey))) {
			err = esp_netif_get_netif_impl_name(esp_netif_get_handle_from_ifkey(ifkey), lwip_netif_name);
		}
		if (err == ESP_OK) {
			AddLog(LOG_LEVEL_DEBUG, PSTR("WG : Found available network interface: %s"), lwip_netif_name);
		}
	}
	if (err != ESP_OK) {
		AddLog(LOG_LEVEL_INFO, PSTR("WG : could not find an available network interface"));
		return ERR_IF;
	}

	underlying_netif = netif_find(lwip_netif_name);

	if (underlying_netif == NULL) {
		AddLog(LOG_LEVEL_INFO, PSTR("WG : netif_find: cannot find %s (%s)"), ifkey, lwip_netif_name);
		return ERR_IF;
	}
#else	// Now in ESP8266
	underlying_netif = netif_default;

	if (underlying_netif == NULL) {
		AddLog(LOG_LEVEL_INFO, PSTR("WG : netif_find: cannot find default netif"));
		return ERR_IF;
	}
#endif // ESP32
	

	// We need to initialise the wireguard module
	wireguard_init();

	if (netif && netif->state) {

		// The init data is passed into the netif_add call as the 'state' - we will replace this with our private state data
		init_data = (struct wireguardif_init_data *)netif->state;

		// Clear out and set if function is successful
		netif->state = NULL;


		// size_t len = decode_base64_length((unsigned char*)init_data->private_key);
		// if (len == WIREGUARD_PRIVATE_KEY_LEN) {
		// 	len = decode_base64((unsigned char*)init_data->private_key, (unsigned char*)private_key);

		udp = udp_new();

		if (udp) {
			result = udp_bind(udp, IP_ADDR_ANY, init_data->listen_port); // Note this listens on all interfaces! Really just want the passed netif
			// AddLog(LOG_LEVEL_DEBUG, "WG : udp_bind result %i port %i", result, init_data->listen_port);
			if (result == ERR_OK) {
				device = (wireguard_device_t *) mem_calloc(1, sizeof(wireguard_device_t));
				if (device) {
					device->netif = netif;
					device->underlying_netif = underlying_netif;
					udp_bind_netif(udp, underlying_netif);

					device->udp_pcb = udp;
					// Per-wireguard netif/device setup
					uint32_t t1 = wireguard_sys_now();
					if (wireguard_device_init(device, init_data->private_key2)) {
						uint32_t t2 = wireguard_sys_now();
						// AddLog(LOG_LEVEL_DEBUG, "WG : device init took %" PRIi32 "ms", (t2-t1));

#if LWIP_CHECKSUM_CTRL_PER_NETIF
						NETIF_SET_CHECKSUM_CTRL(netif, NETIF_CHECKSUM_ENABLE_ALL);
#endif
						netif->state = device;
						netif->name[0] = 'w';
						netif->name[1] = 'g';
						netif->output = wireguardif_output;
						netif->linkoutput = NULL;
						netif->hwaddr_len = 0;
						netif->mtu = WIREGUARDIF_MTU;
						// We set up no state flags here - caller should set them
						// NETIF_FLAG_LINK_UP is automatically set/cleared when at least one peer is connected
						netif->flags = 0;

						udp_recv(udp, wireguardif_network_rx, device);

						// Start a periodic timer for this wireguard device
						sys_timeout(WIREGUARDIF_TIMER_MSECS, wireguardif_tmr, device);

						result = ERR_OK;
					} else {
						mem_free(device);
						device = NULL;
						udp_remove(udp);
						result = ERR_ARG;
					}
				} else {
					udp_remove(udp);
					result = ERR_MEM;
				}
			} else {
				udp_remove(udp);
			}
		}
	} else {
		result = ERR_ARG;
	}
fail:
	// AddLog(LOG_LEVEL_DEBUG, ">>>: OUT wireguardif_init err=%i", result);
	return result;
}

void wireguardif_peer_init(wireguardif_peer_t *peer) {
	if (peer) {
		memset(peer, 0, sizeof(wireguardif_peer_t));
		ip_addr_set_any(false, &peer->endpoint_ip);
		peer->endport_port = WIREGUARDIF_DEFAULT_PORT;
		peer->keep_alive = 0;
		ip_addr_set_any(false, &peer->allowed_ip);
		ip_addr_set_any(false, &peer->allowed_mask);
		memset(peer->greatest_timestamp, 0, sizeof(peer->greatest_timestamp));
		memset(peer->public_key2, 0, sizeof(peer->public_key2));
		memset(peer->preshared_key2, 0, sizeof(peer->preshared_key2));
	}
}

void wireguardif_shutdown(struct netif *netif) {
	if (netif && netif->state) {
		wireguard_device_t *device = (wireguard_device_t *)netif->state;
		// Disable timer.
		sys_untimeout(wireguardif_tmr, device);
		// remove UDP context.
		if (device->udp_pcb) {
			udp_disconnect(device->udp_pcb);
			udp_remove(device->udp_pcb);
			device->udp_pcb = NULL;
		}
	}
}

void wireguardif_fini(struct netif *netif) {
	if (netif && netif->state) {
		free(netif->state);
		netif->state = NULL;
	}
}
