/*
 * Copyright (c) 2022 Tomoyuki Sakurai <y@trombik.org>
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
 */

#include <Arduino.h>
#include "esp_wireguard.h"

#include "WiFiHelper.h"

#include "lwip/ip.h"
#include "lwip/ip_addr.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "lwip/err.h"

#include "wireguard-platform.h"
#include "wireguardif.h"

//**************************************************************************************************************
// enable AddLog support within a C++ library
extern void AddLog(uint32_t loglevel, PGM_P formatP, ...);
enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE};
//**************************************************************************************************************

#if defined(CONFIG_LWIP_IPV6)
#define WG_ADDRSTRLEN  INET6_ADDRSTRLEN
#else
#define WG_ADDRSTRLEN  INET_ADDRSTRLEN
#endif

static struct netif wg_netif_struct = {0};
static struct netif *wg_netif = NULL;
static wireguardif_peer_t peer = {0};
static uint8_t wireguard_peer_index = WIREGUARDIF_INVALID_INDEX;

static esp_err_t esp_wireguard_peer_init(const wireguard_config_t *config, wireguardif_peer_t *peer)
{
    if (!config || !peer) { return ESP_ERR_INVALID_ARG; }

    if (ip_addr_isany(&(config->endpoint_ip))) {
        AddLog(LOG_LEVEL_INFO, PSTR("WG : peer_init: invalid endpoint ip: `%s`"), ipaddr_ntoa(&(config->endpoint_ip)));
        return ESP_ERR_INVALID_ARG;
    }
    ip_addr_copy(peer->endpoint_ip, config->endpoint_ip);

    memmove(peer->public_key2, config->public_key2, WG_KEY_LEN);
    memmove(peer->preshared_key2, config->preshared_key2, WG_KEY_LEN);
    peer->keep_alive = config->persistent_keepalive;

    /* Allow device's own address through tunnel */
    peer->allowed_ip = config->address2;
    peer->allowed_mask = config->subnet;

    
    AddLog(LOG_LEVEL_DEBUG, PSTR("WG : Default allowed_ips %s/%s"), IPAddress(&peer->allowed_ip).toString().c_str(), 
                                                              IPAddress(&peer->allowed_mask).toString().c_str());

    peer->endport_port = config->port;
    peer->keep_alive = config->persistent_keepalive;
    return ESP_OK;
}

static esp_err_t esp_wireguard_netif_create(const wireguard_config_t *config)
{
    esp_err_t err;
    ip_addr_t gateway = IPADDR4_INIT_BYTES(0, 0, 0, 0);
    struct wireguardif_init_data wg = {0};

    if (!config) { return ESP_ERR_INVALID_ARG;}

    /* Setup the WireGuard device structure */
    memmove(wg.private_key2, config->private_key2, WG_KEY_LEN);
    wg.listen_port = config->listen_port;
    wg.bind_netif = NULL;    

    /* Register the new WireGuard network interface with lwIP */
    // AddLog(LOG_LEVEL_DEBUG, "WG : Creating netif addr %_I mask %_I gateway %_I",
    //                         config->address2.u_addr.ip4.addr, config->netmask2.u_addr.ip4.addr, gateway.u_addr.ip4.addr);
    
    wg_netif = netif_add(
            &wg_netif_struct,
            ip_2_ip4(&config->address2),
            ip_2_ip4(&config->netmask2),
            ip_2_ip4(&gateway),
            &wg, &wireguardif_init,
            &ip_input);
    
    if (wg_netif == NULL) {
        AddLog(LOG_LEVEL_INFO, PSTR("WG : netif_add: failed"));
        return ESP_FAIL;
    }

    /* Mark the interface as administratively up, link up flag is set
     * automatically when peer connects */
    netif_set_up(wg_netif);
    return ESP_OK;
}

esp_err_t esp_wireguard_init(wireguard_config_t *config, wireguard_ctx_t *ctx)
{
    if (!config || !ctx) { return ESP_ERR_INVALID_ARG; }
    ctx->config = config;
    ctx->netif = NULL;
    return ESP_OK;
}

esp_err_t esp_wireguard_connect(wireguard_ctx_t *ctx)
{
    esp_err_t err = ESP_FAIL;
    err_t lwip_err = -1;
    IPAddress remote_addr;

    if (!ctx) {
        err = ESP_ERR_INVALID_ARG;
        goto fail;
    }

    if (ctx->netif == NULL) {
        err = esp_wireguard_netif_create(ctx->config);
        if (err != ESP_OK) {
            AddLog(LOG_LEVEL_INFO, PSTR("WG : netif_create err %d"), err);
            goto fail;
        }
        ctx->netif = wg_netif;
        // ctx->netif_default = netif_default;
    }

    // Add include "ESP8266WiFi.h" for this to work
    if (!WiFiHelper::hostByName(ctx->config->endpoint, remote_addr)) {
        AddLog(LOG_LEVEL_INFO, PSTR("WG : hostByName failed '%s'"), ctx->config->endpoint);
        goto fail;
    }
#ifdef ESP32
    remote_addr.to_ip_addr_t(&ctx->config->endpoint_ip);
#else
    ctx->config->endpoint_ip = remote_addr;
#endif
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("WG : hostByName '%s' resolved to %s"), ctx->config->endpoint, remote_addr.toString().c_str());

    /* Initialize the first WireGuard peer structure */
    err = esp_wireguard_peer_init(ctx->config, &peer);
    if (err != ESP_OK) {
        AddLog(LOG_LEVEL_INFO, PSTR("WG : peer_init: %d"), err);
        goto fail;
    }

    /* Register the new WireGuard peer with the network interface */
    lwip_err = wireguardif_add_peer(ctx->netif, &peer, &wireguard_peer_index);
    if (lwip_err != ERR_OK || wireguard_peer_index == WIREGUARDIF_INVALID_INDEX) {
        AddLog(LOG_LEVEL_INFO, PSTR("WG : wireguardif_add_peer: %i"), lwip_err);
        err = ESP_FAIL;
        goto fail;
    }
    if (ip_addr_isany(&peer.endpoint_ip)) {
        err = ESP_FAIL;
        goto fail;
    }

    AddLog(LOG_LEVEL_DEBUG, PSTR("WG : Connecting to %s (%s) port %i"), ctx->config->endpoint, ipaddr_ntoa(&(peer.endpoint_ip)), peer.endport_port);
    lwip_err = wireguardif_connect(ctx->netif, wireguard_peer_index);
    if (lwip_err != ERR_OK) {
        AddLog(LOG_LEVEL_INFO, PSTR("WG : wireguardif_connect: %i"), lwip_err);
        err = ESP_FAIL;
        goto fail;
    }
    err = ESP_OK;
fail:
    // AddLog(LOG_LEVEL_DEBUG, ">>>: OUT esp_wireguard_connect %d", err);
    return err;
}

esp_err_t esp_wireguard_disconnect(wireguard_ctx_t *ctx)
{
    err_t lwip_err;
    if (!ctx || !ctx->netif) { return ESP_ERR_INVALID_ARG; }

    // Clear the IP address to gracefully disconnect any clients while the
    // peers are still valid
    netif_set_ipaddr(ctx->netif, IP4_ADDR_ANY4);

    lwip_err = wireguardif_disconnect(ctx->netif, wireguard_peer_index);
    if (lwip_err != ERR_OK) {
        AddLog(LOG_LEVEL_INFO, PSTR("WG : wireguardif_disconnect: peer_index: %" PRIu8 " err: %i"), wireguard_peer_index, lwip_err);
    }

    lwip_err = wireguardif_remove_peer(ctx->netif, wireguard_peer_index);
    if (lwip_err != ERR_OK) {
        AddLog(LOG_LEVEL_INFO, PSTR("WG : wireguardif_remove_peer: peer_index: %" PRIu8 " err: %i"), wireguard_peer_index, lwip_err);
    }

    wireguard_peer_index = WIREGUARDIF_INVALID_INDEX;
    wireguardif_shutdown(ctx->netif);
    netif_remove(ctx->netif);
    wireguardif_fini(ctx->netif);
    ctx->netif = NULL;

    return ESP_OK;
}

esp_err_t esp_wireguard_peer_is_up(const wireguard_ctx_t *ctx)
{
    err_t lwip_err;
    if (!ctx || !ctx->netif) { return ESP_ERR_INVALID_ARG; }

    lwip_err = wireguardif_peer_is_up(
            ctx->netif,
            wireguard_peer_index,
            &peer.endpoint_ip,
            &peer.endport_port);

    return (lwip_err != ERR_OK) ? ESP_FAIL : ESP_OK;
}

esp_err_t esp_wireguard_latest_handshake(const wireguard_ctx_t *ctx, time_t *result)
{
    if (!ctx || !ctx->netif) { return ESP_ERR_INVALID_ARG; }

    *result = wireguardif_latest_handshake(ctx->netif, wireguard_peer_index);
    return (*result > 0) ? ESP_OK : ESP_FAIL;
}

esp_err_t esp_wireguard_add_allowed_ip(const wireguard_ctx_t *ctx, const ip_addr_t& allowed_ip, const ip_addr_t& allowed_ip_mask)
{
    err_t lwip_err;
    if (!ctx || !ctx->netif) { return ESP_ERR_INVALID_ARG; }

    // AddLog(LOG_LEVEL_DEBUG, "WG : Added allowed_ips %_I/%_I", allowed_ip.u_addr.ip4.addr, allowed_ip_mask.u_addr.ip4.addr);
    lwip_err = wireguardif_add_allowed_ip(ctx->netif, wireguard_peer_index, allowed_ip, allowed_ip_mask);
    return (lwip_err == ERR_OK ? ESP_OK : ESP_FAIL);
}
