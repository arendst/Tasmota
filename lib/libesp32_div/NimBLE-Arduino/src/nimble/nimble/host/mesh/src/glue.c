/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#if MYNEWT_VAL(BLE_MESH)

#define MESH_LOG_MODULE BLE_MESH_LOG

#include "../include/mesh/glue.h"
#include "adv.h"
#ifndef MYNEWT
#include "nimble/porting/nimble/include/nimble/nimble_port.h"
#endif

#if MYNEWT_VAL(BLE_MESH_SETTINGS)
#include "base64/base64.h"
#endif

extern u8_t g_mesh_addr_type;

#if MYNEWT_VAL(BLE_EXT_ADV)
/* Store configuration for different bearers */
#define BT_MESH_ADV_IDX          (0)
#define BT_MESH_GATT_IDX         (1)
static struct ble_gap_adv_params ble_adv_cur_conf[2];
#endif

const char *
bt_hex(const void *buf, size_t len)
{
    static const char hex[] = "0123456789abcdef";
    static char hexbufs[4][137];
    static u8_t curbuf;
    const u8_t *b = buf;
    char *str;
    int i;

    str = hexbufs[curbuf++];
    curbuf %= ARRAY_SIZE(hexbufs);

    len = min(len, (sizeof(hexbufs[0]) - 1) / 2);

    for (i = 0; i < len; i++) {
        str[i * 2] = hex[b[i] >> 4];
        str[i * 2 + 1] = hex[b[i] & 0xf];
    }

    str[i * 2] = '\0';

    return str;
}

void
net_buf_put(struct ble_npl_eventq *fifo, struct os_mbuf *om)
{
    struct ble_npl_event *ev;

    assert(OS_MBUF_IS_PKTHDR(om));
    ev = &BT_MESH_ADV(om)->ev;
    assert(ev);
    assert(ble_npl_event_get_arg(ev));

    ble_npl_eventq_put(fifo, ev);
}

void *
net_buf_ref(struct os_mbuf *om)
{
    struct bt_mesh_adv *adv;

    /* For bufs with header we count refs*/
    if (OS_MBUF_USRHDR_LEN(om) == 0) {
        return om;
    }

    adv = BT_MESH_ADV(om);
    adv->ref_cnt++;

    return om;
}

void
net_buf_unref(struct os_mbuf *om)
{
    struct bt_mesh_adv *adv;

    /* For bufs with header we count refs*/
    if (OS_MBUF_USRHDR_LEN(om) == 0) {
        goto free;
    }

    adv = BT_MESH_ADV(om);
    if (--adv->ref_cnt > 0) {
        return;
    }

free:
    os_mbuf_free_chain(om);
}

#if MYNEWT_VAL(BLE_CRYPTO_STACK_MBEDTLS)
int
bt_encrypt_be(const uint8_t *key, const uint8_t *plaintext, uint8_t *enc_data)
{
    mbedtls_aes_context s = {0};
    mbedtls_aes_init(&s);

    if (mbedtls_aes_setkey_enc(&s, key, 128) != 0) {
        mbedtls_aes_free(&s);
        return BLE_HS_EUNKNOWN;
    }

    if (mbedtls_aes_crypt_ecb(&s, MBEDTLS_AES_ENCRYPT, plaintext, enc_data) != 0) {
        mbedtls_aes_free(&s);
        return BLE_HS_EUNKNOWN;
    }

    mbedtls_aes_free(&s);
    return 0;
}

#else
int
bt_encrypt_be(const uint8_t *key, const uint8_t *plaintext, uint8_t *enc_data)
{
    struct tc_aes_key_sched_struct s = {0};

    if (tc_aes128_set_encrypt_key(&s, key) == TC_CRYPTO_FAIL) {
        return BLE_HS_EUNKNOWN;
    }

    if (tc_aes_encrypt(enc_data, plaintext, &s) == TC_CRYPTO_FAIL) {
        return BLE_HS_EUNKNOWN;
    }

    return 0;
}
#endif

uint16_t
net_buf_simple_pull_le16(struct os_mbuf *om)
{
    uint16_t val;
    struct os_mbuf *old = om;

    om = os_mbuf_pullup(om, sizeof(val));
    assert(om == old);
    val = get_le16(om->om_data);
    os_mbuf_adj(om, sizeof(val));

    return val;
}

uint16_t
net_buf_simple_pull_be16(struct os_mbuf *om)
{
    uint16_t val;
    struct os_mbuf *old = om;

    om = os_mbuf_pullup(om, sizeof(val));
    assert(om == old);
    val = get_be16(om->om_data);
    os_mbuf_adj(om, sizeof(val));

    return val;
}

uint32_t
net_buf_simple_pull_be32(struct os_mbuf *om)
{
    uint32_t val;
    struct os_mbuf *old = om;

    om = os_mbuf_pullup(om, sizeof(val));
    assert(om == old);
    val = get_be32(om->om_data);
    os_mbuf_adj(om, sizeof(val));

    return val;
}

uint32_t
net_buf_simple_pull_le32(struct os_mbuf *om)
{
    uint32_t val;
    struct os_mbuf *old = om;

    om = os_mbuf_pullup(om, sizeof(val));
    assert(om == old);
    val = get_le32(om->om_data);
    os_mbuf_adj(om, sizeof(val));

    return val;
}

uint8_t
net_buf_simple_pull_u8(struct os_mbuf *om)
{
    uint8_t val;
    struct os_mbuf *old = om;

    om = os_mbuf_pullup(om, sizeof(val));
    assert(om == old);
    val = om->om_data[0];
    os_mbuf_adj(om, 1);

    return val;
}

void
net_buf_simple_add_le16(struct os_mbuf *om, uint16_t val)
{
    val = htole16(val);
    os_mbuf_append(om, &val, sizeof(val));
    ASSERT_NOT_CHAIN(om);
}

void
net_buf_simple_add_be16(struct os_mbuf *om, uint16_t val)
{
    val = htobe16(val);
    os_mbuf_append(om, &val, sizeof(val));
    ASSERT_NOT_CHAIN(om);
}

void
net_buf_simple_add_be32(struct os_mbuf *om, uint32_t val)
{
    val = htobe32(val);
    os_mbuf_append(om, &val, sizeof(val));
    ASSERT_NOT_CHAIN(om);
}

void
net_buf_simple_add_le32(struct os_mbuf *om, uint32_t val)
{
    val = htole32(val);
    os_mbuf_append(om, &val, sizeof(val));
    ASSERT_NOT_CHAIN(om);
}

void
net_buf_simple_add_u8(struct os_mbuf *om, uint8_t val)
{
    os_mbuf_append(om, &val, 1);
    ASSERT_NOT_CHAIN(om);
}

void
net_buf_simple_push_le16(struct os_mbuf *om, uint16_t val)
{
    uint8_t headroom = om->om_data - &om->om_databuf[om->om_pkthdr_len];

    assert(headroom >= 2);
    om->om_data -= 2;
    put_le16(om->om_data, val);
    om->om_len += 2;

    if (om->om_pkthdr_len) {
        OS_MBUF_PKTHDR(om)->omp_len += 2;
    }
    ASSERT_NOT_CHAIN(om);
}

void
net_buf_simple_push_be16(struct os_mbuf *om, uint16_t val)
{
    uint8_t headroom = om->om_data - &om->om_databuf[om->om_pkthdr_len];

    assert(headroom >= 2);
    om->om_data -= 2;
    put_be16(om->om_data, val);
    om->om_len += 2;

    if (om->om_pkthdr_len) {
        OS_MBUF_PKTHDR(om)->omp_len += 2;
    }
    ASSERT_NOT_CHAIN(om);
}

void
net_buf_simple_push_u8(struct os_mbuf *om, uint8_t val)
{
    uint8_t headroom = om->om_data - &om->om_databuf[om->om_pkthdr_len];

    assert(headroom >= 1);
    om->om_data -= 1;
    om->om_data[0] = val;
    om->om_len += 1;

    if (om->om_pkthdr_len) {
        OS_MBUF_PKTHDR(om)->omp_len += 1;
    }
    ASSERT_NOT_CHAIN(om);
}

void
net_buf_add_zeros(struct os_mbuf *om, uint8_t len)
{
    uint8_t z[len];
    int rc;

    memset(z, 0, len);

    rc = os_mbuf_append(om, z, len);
    if(rc) {
        assert(0);
    }
    ASSERT_NOT_CHAIN(om);
}

void *
net_buf_simple_pull(struct os_mbuf *om, uint8_t len)
{
    os_mbuf_adj(om, len);
    return om->om_data;
}

void *
net_buf_simple_pull_mem(struct os_mbuf *om, uint8_t len)
{
    void *data = om->om_data;

    net_buf_simple_pull(om, len);
    return data;
}

void*
net_buf_simple_add(struct os_mbuf *om, uint8_t len)
{
    void * tmp;

    tmp = os_mbuf_extend(om, len);
    ASSERT_NOT_CHAIN(om);

    return tmp;
}

bool
k_fifo_is_empty(struct ble_npl_eventq *q)
{
    return ble_npl_eventq_is_empty(q);
}

void * net_buf_get(struct ble_npl_eventq *fifo, s32_t t)
{
    struct ble_npl_event *ev = ble_npl_eventq_get(fifo, 0);

    if (ev) {
        return ble_npl_event_get_arg(ev);
    }

    return NULL;
}

uint8_t *
net_buf_simple_push(struct os_mbuf *om, uint8_t len)
{
    uint8_t headroom = om->om_data - &om->om_databuf[om->om_pkthdr_len];

    assert(headroom >= len);
    om->om_data -= len;
    om->om_len += len;

    return om->om_data;
}

void
net_buf_reserve(struct os_mbuf *om, size_t reserve)
{
    /* We need reserve to be done on fresh buf */
    assert(om->om_len == 0);
    om->om_data += reserve;
}

void
k_work_init(struct ble_npl_callout *work, ble_npl_event_fn handler)
{
#ifndef MYNEWT
    ble_npl_callout_init(work, nimble_port_get_dflt_eventq(), handler, NULL);
#else
    ble_npl_callout_init(work, ble_npl_eventq_dflt_get(), handler, NULL);
#endif
}

void
k_delayed_work_init(struct k_delayed_work *w, ble_npl_event_fn *f)
{
#ifndef MYNEWT
    ble_npl_callout_init(&w->work, nimble_port_get_dflt_eventq(), f, NULL);
#else
    ble_npl_callout_init(&w->work, ble_npl_eventq_dflt_get(), f, NULL);
#endif
}

void
k_delayed_work_cancel(struct k_delayed_work *w)
{
    ble_npl_callout_stop(&w->work);
}

void
k_delayed_work_submit(struct k_delayed_work *w, uint32_t ms)
{
    uint32_t ticks;

    if (ble_npl_time_ms_to_ticks(ms, &ticks) != 0) {
        assert(0);
    }
    ble_npl_callout_reset(&w->work, ticks);
}

void
k_work_submit(struct ble_npl_callout *w)
{
    ble_npl_callout_reset(w, 0);
}

void
k_work_add_arg(struct ble_npl_callout *w, void *arg)
{
    ble_npl_callout_set_arg(w, arg);
}

void
k_delayed_work_add_arg(struct k_delayed_work *w, void *arg)
{
    k_work_add_arg(&w->work, arg);
}

uint32_t
k_delayed_work_remaining_get (struct k_delayed_work *w)
{
    int sr;
    ble_npl_time_t t;

    OS_ENTER_CRITICAL(sr);

    t = ble_npl_callout_remaining_ticks(&w->work, ble_npl_time_get());

    OS_EXIT_CRITICAL(sr);

    return ble_npl_time_ticks_to_ms32(t);
}

int64_t k_uptime_get(void)
{
    /* We should return ms */
    return ble_npl_time_ticks_to_ms32(ble_npl_time_get());
}

u32_t k_uptime_get_32(void)
{
    return k_uptime_get();
}

void k_sleep(int32_t duration)
{
    uint32_t ticks;

    ticks = ble_npl_time_ms_to_ticks32(duration);

    ble_npl_time_delay(ticks);
}

static uint8_t pub[64];
static uint8_t priv[32];
static bool has_pub = false;

int
bt_dh_key_gen(const u8_t remote_pk[64], bt_dh_key_cb_t cb)
{
    uint8_t dh[32];

    if (ble_sm_alg_gen_dhkey((uint8_t *)&remote_pk[0], (uint8_t *)&remote_pk[32],
                              priv, dh)) {
        return -1;
    }

    cb(dh);
    return 0;
}

int
bt_rand(void *buf, size_t len)
{
    int rc;
    rc = ble_hs_hci_util_rand(buf, len);
    if (rc != 0) {
        return -1;
    }

    return 0;
}

int
bt_pub_key_gen(struct bt_pub_key_cb *new_cb)
{

    if (ble_sm_alg_gen_key_pair(pub, priv)) {
        assert(0);
        return -1;
    }

    new_cb->func(pub);
    has_pub = true;

    return 0;
}

uint8_t *
bt_pub_key_get(void)
{
    if (!has_pub) {
        return NULL;
    }

    return pub;
}

static int
set_ad(const struct bt_data *ad, size_t ad_len, u8_t *buf, u8_t *buf_len)
{
    int i;

    for (i = 0; i < ad_len; i++) {
        buf[(*buf_len)++] = ad[i].data_len + 1;
        buf[(*buf_len)++] = ad[i].type;

        memcpy(&buf[*buf_len], ad[i].data,
               ad[i].data_len);
        *buf_len += ad[i].data_len;
    }

    return 0;
}

#if MYNEWT_VAL(BLE_EXT_ADV)
static void
ble_adv_copy_to_ext_param(struct ble_gap_ext_adv_params *ext_param,
                          const struct ble_gap_adv_params *param)
{
    memset(ext_param, 0, sizeof(*ext_param));

    ext_param->legacy_pdu = 1;

    if (param->conn_mode != BLE_GAP_CONN_MODE_NON) {
        ext_param->connectable = 1;
        ext_param->scannable = 1;
    }

    ext_param->itvl_max = param->itvl_max;
    ext_param->itvl_min = param->itvl_min;
    ext_param->channel_map = param->channel_map;
    ext_param->high_duty_directed = param->high_duty_cycle;
    ext_param->own_addr_type = g_mesh_addr_type;
}

static int
ble_adv_conf_adv_instance(const struct ble_gap_adv_params *param, int *instance)
{
    struct ble_gap_ext_adv_params ext_params;
    struct ble_gap_adv_params *cur_conf;
    int err = 0;

    if (param->conn_mode == BLE_GAP_CONN_MODE_NON) {
        *instance = BT_MESH_ADV_INST;
        cur_conf = &ble_adv_cur_conf[BT_MESH_ADV_IDX];
    } else {
#if MYNEWT_VAL(BLE_MESH_PROXY)
        *instance = BT_MESH_ADV_GATT_INST;
        cur_conf = &ble_adv_cur_conf[BT_MESH_GATT_IDX];
#else
        assert(0);
#endif
    }

    /* Checking interval max as it has to be in place if instance was configured
     * before.
     */
    if (cur_conf->itvl_max == 0) {
        goto configure;
    }

    if (memcmp(param, cur_conf, sizeof(*cur_conf)) == 0) {
        /* Same parameters - skip reconfiguring */
        goto done;
    }

    ble_gap_ext_adv_stop(*instance);
    err = ble_gap_ext_adv_remove(*instance);
    if (err) {
        assert(0);
        goto done;
    }

configure:
    ble_adv_copy_to_ext_param(&ext_params, param);

    err  = ble_gap_ext_adv_configure(*instance, &ext_params, 0,
                                            ble_adv_gap_mesh_cb, NULL);
    if (!err) {
        memcpy(cur_conf, param, sizeof(*cur_conf));
    }

done:
    return err;
}

int
bt_le_adv_start(const struct ble_gap_adv_params *param,
                const struct bt_data *ad, size_t ad_len,
                const struct bt_data *sd, size_t sd_len)
{
    struct os_mbuf *data;
    int instance;
    int err;
    uint8_t buf[BLE_HS_ADV_MAX_SZ];
    uint8_t buf_len = 0;

    err = ble_adv_conf_adv_instance(param, &instance);
    if (err) {
        return err;
    }

    if (ad_len > 0) {
        err = set_ad(ad, ad_len, buf, &buf_len);
        if (err) {
            return err;
        }

        /* For now let's use msys pool. We are not putting more then legacy */
        data = os_msys_get_pkthdr(BLE_HS_ADV_MAX_SZ, 0);
        if (!data) {
            return OS_ENOMEM;
        }

        err = os_mbuf_append(data, buf, buf_len);
        if (err) {
            goto error;
        }

        err = ble_gap_ext_adv_set_data(instance, data);
        if (err) {
            return err;
        }

        data = NULL;
    }

    if (sd_len > 0) {
        buf_len = 0;

        err = set_ad(sd, sd_len, buf, &buf_len);
        if (err) {
            return err;
        }

        /* For now let's use msys pool. We are not putting more then legace*/
        data = os_msys_get_pkthdr(BLE_HS_ADV_MAX_SZ, 0);
        if (!data) {
            return OS_ENOMEM;
        }

        err = os_mbuf_append(data, buf, buf_len);
        if (err) {
            goto error;
        }

        err = ble_gap_ext_adv_rsp_set_data(instance, data);
        if (err) {
            goto error;
        }
    }

    /*TODO: We could use duration and max events in the future */
    err = ble_gap_ext_adv_start(instance, 0, 0);
    return err;

error:
    if (data) {
        os_mbuf_free_chain(data);
    }

    return err;
}

int bt_le_adv_stop(bool proxy)
{
#if MYNEWT_VAL(BLE_MESH_PROXY)
    int rc;

    if (proxy) {
        rc = ble_gap_ext_adv_stop(BT_MESH_ADV_GATT_INST);
    } else {
        rc = ble_gap_ext_adv_stop(BT_MESH_ADV_INST);
    }

    return rc;
#else
    return ble_gap_ext_adv_stop(BT_MESH_ADV_INST);
#endif
}

#else

int
bt_le_adv_start(const struct ble_gap_adv_params *param,
                const struct bt_data *ad, size_t ad_len,
                const struct bt_data *sd, size_t sd_len)
{
    uint8_t buf[BLE_HS_ADV_MAX_SZ];
    uint8_t buf_len = 0;
    int err;

    err = set_ad(ad, ad_len, buf, &buf_len);
    if (err) {
        return err;
    }

    err = ble_gap_adv_set_data(buf, buf_len);
    if (err != 0) {
        return err;
    }

    if (sd) {
        buf_len = 0;

        err = set_ad(sd, sd_len, buf, &buf_len);
        if (err) {
            BT_ERR("Advertising failed: err %d", err);
            return err;
        }

        err = ble_gap_adv_rsp_set_data(buf, buf_len);
        if (err != 0) {
            BT_ERR("Advertising failed: err %d", err);
            return err;
        }
    }

    err = ble_gap_adv_start(g_mesh_addr_type, NULL, BLE_HS_FOREVER, param,
                            NULL, NULL);
    if (err) {
        BT_ERR("Advertising failed: err %d", err);
        return err;
    }

    return 0;
}

int bt_le_adv_stop(bool proxy)
{
	return ble_gap_adv_stop();
}

#endif

#if MYNEWT_VAL(BLE_MESH_PROXY)
int bt_mesh_proxy_svcs_register(void);
#endif

void
bt_mesh_register_gatt(void)
{
#if MYNEWT_VAL(BLE_MESH_PROXY)
    bt_mesh_proxy_svcs_register();
#endif
}

void net_buf_slist_init(struct net_buf_slist_t *list)
{
	STAILQ_INIT(list);
}

bool net_buf_slist_is_empty(struct net_buf_slist_t *list)
{
	return STAILQ_EMPTY(list);
}

struct os_mbuf *net_buf_slist_peek_head(struct net_buf_slist_t *list)
{
	struct os_mbuf_pkthdr *pkthdr;

	/* Get mbuf pointer from packet header pointer */
	pkthdr = STAILQ_FIRST(list);
	if (!pkthdr) {
		return NULL;
	}

	return OS_MBUF_PKTHDR_TO_MBUF(pkthdr);
}

struct os_mbuf *net_buf_slist_peek_next(struct os_mbuf *buf)
{
	struct os_mbuf_pkthdr *pkthdr;

	/* Get mbuf pointer from packet header pointer */
	pkthdr = OS_MBUF_PKTHDR(buf);
	pkthdr = STAILQ_NEXT(pkthdr, omp_next);
	if (!pkthdr) {
		return NULL;
	}

	return OS_MBUF_PKTHDR_TO_MBUF(pkthdr);
}

struct os_mbuf *net_buf_slist_get(struct net_buf_slist_t *list)
{
	os_sr_t sr;
	struct os_mbuf *m;

	m = net_buf_slist_peek_head(list);
	if (!m) {
		return NULL;
	}

	/* Remove from queue */
	OS_ENTER_CRITICAL(sr);
	STAILQ_REMOVE_HEAD(list, omp_next);
	OS_EXIT_CRITICAL(sr);
	return m;
}

void net_buf_slist_put(struct net_buf_slist_t *list, struct os_mbuf *buf)
{
	struct os_mbuf_pkthdr *pkthdr;

	pkthdr = OS_MBUF_PKTHDR(buf);
	STAILQ_INSERT_TAIL(list, pkthdr, omp_next);
}

void net_buf_slist_remove(struct net_buf_slist_t *list, struct os_mbuf *prev,
			  struct os_mbuf *cur)
{
	struct os_mbuf_pkthdr *pkthdr, *cur_pkthdr;

	cur_pkthdr = OS_MBUF_PKTHDR(cur);

	STAILQ_FOREACH(pkthdr, list, omp_next) {
		if (cur_pkthdr == pkthdr) {
			STAILQ_REMOVE(list, cur_pkthdr, os_mbuf_pkthdr, omp_next);
			break;
		}
	}
}

void net_buf_slist_merge_slist(struct net_buf_slist_t *list,
			       struct net_buf_slist_t *list_to_append)
{
	if (!STAILQ_EMPTY(list_to_append)) {
		*(list)->stqh_last = list_to_append->stqh_first;
		(list)->stqh_last = list_to_append->stqh_last;
		STAILQ_INIT(list_to_append);
	}
}

#if MYNEWT_VAL(BLE_MESH_SETTINGS)

int settings_bytes_from_str(char *val_str, void *vp, int *len)
{
    *len = base64_decode(val_str, vp);
    return 0;
}

char *settings_str_from_bytes(const void *vp, int vp_len,
			      char *buf, int buf_len)
{
    if (BASE64_ENCODE_SIZE(vp_len) > buf_len) {
        return NULL;
    }

    base64_encode(vp, vp_len, buf, 1);

    return buf;
}

#endif /* MYNEWT_VAL(BLE_MESH_SETTINGS) */
#endif
