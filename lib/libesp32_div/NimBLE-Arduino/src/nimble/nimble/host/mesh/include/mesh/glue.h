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

#ifndef _MESH_GLUE_
#define _MESH_GLUE_

#include <assert.h>
#include <errno.h>

#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#include "nimble/porting/nimble/include/logcfg/logcfg.h"
#include "nimble/porting/nimble/include/modlog/modlog.h"
#include "nimble/nimble/include/nimble/nimble_npl.h"

#include "nimble/porting/nimble/include/os/os_mbuf.h"
#include "nimble/porting/nimble/include/os/queue.h"

#include "nimble/nimble/include/nimble/ble.h"
#include "nimble/nimble/host/include/host/ble_hs.h"
#include "nimble/nimble/host/include/host/ble_uuid.h"
#include "nimble/nimble/host/src/ble_sm_priv.h"
#include "nimble/nimble/host/src/ble_hs_hci_priv.h"

#if MYNEWT_VAL(BLE_CRYPTO_STACK_MBEDTLS)
#include "mbedtls/aes.h"
#include "mbedtls/cipher.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/cmac.h"
#include "mbedtls/ecdh.h"
#include "mbedtls/ecp.h"

#else
#include "nimble/ext/tinycrypt/include/tinycrypt/aes.h"
#include "nimble/ext/tinycrypt/include/tinycrypt/constants.h"
#include "nimble/ext/tinycrypt/include/tinycrypt/utils.h"
#include "nimble/ext/tinycrypt/include/tinycrypt/cmac_mode.h"
#include "nimble/ext/tinycrypt/include/tinycrypt/ecc_dh.h"
#endif

#if MYNEWT_VAL(BLE_MESH_SETTINGS)
#include "config/config.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Helper to declare elements of bt_data arrays
 *
 *  This macro is mainly for creating an array of struct bt_data
 *  elements which is then passed to bt_le_adv_start().
 *
 *  @param _type Type of advertising data field
 *  @param _data Pointer to the data field payload
 *  @param _data_len Number of bytes behind the _data pointer
 */
#define BT_DATA(_type, _data, _data_len) \
    { \
        .type = (_type), \
        .data_len = (_data_len), \
        .data = (const uint8_t *)(_data), \
    }

/** @brief Helper to declare elements of bt_data arrays
 *
 *  This macro is mainly for creating an array of struct bt_data
 *  elements which is then passed to bt_le_adv_start().
 *
 *  @param _type Type of advertising data field
 *  @param _bytes Variable number of single-byte parameters
 */
#define BT_DATA_BYTES(_type, _bytes...) \
    BT_DATA(_type, ((uint8_t []) { _bytes }), \
        sizeof((uint8_t []) { _bytes }))

/* EIR/AD data type definitions */
#define BT_DATA_FLAGS                   0x01 /* AD flags */
#define BT_DATA_UUID16_SOME             0x02 /* 16-bit UUID, more available */
#define BT_DATA_UUID16_ALL              0x03 /* 16-bit UUID, all listed */
#define BT_DATA_UUID32_SOME             0x04 /* 32-bit UUID, more available */
#define BT_DATA_UUID32_ALL              0x05 /* 32-bit UUID, all listed */
#define BT_DATA_UUID128_SOME            0x06 /* 128-bit UUID, more available */
#define BT_DATA_UUID128_ALL             0x07 /* 128-bit UUID, all listed */
#define BT_DATA_NAME_SHORTENED          0x08 /* Shortened name */
#define BT_DATA_NAME_COMPLETE           0x09 /* Complete name */
#define BT_DATA_TX_POWER                0x0a /* Tx Power */
#define BT_DATA_SOLICIT16               0x14 /* Solicit UUIDs, 16-bit */
#define BT_DATA_SOLICIT128              0x15 /* Solicit UUIDs, 128-bit */
#define BT_DATA_SVC_DATA16              0x16 /* Service data, 16-bit UUID */
#define BT_DATA_GAP_APPEARANCE          0x19 /* GAP appearance */
#define BT_DATA_SOLICIT32               0x1f /* Solicit UUIDs, 32-bit */
#define BT_DATA_SVC_DATA32              0x20 /* Service data, 32-bit UUID */
#define BT_DATA_SVC_DATA128             0x21 /* Service data, 128-bit UUID */
#define BT_DATA_URI                     0x24 /* URI */
#define BT_DATA_MESH_PROV               0x29 /* Mesh Provisioning PDU */
#define BT_DATA_MESH_MESSAGE            0x2a /* Mesh Networking PDU */
#define BT_DATA_MESH_BEACON             0x2b /* Mesh Beacon */

#define BT_DATA_MANUFACTURER_DATA       0xff /* Manufacturer Specific Data */

#define BT_LE_AD_LIMITED                0x01 /* Limited Discoverable */
#define BT_LE_AD_GENERAL                0x02 /* General Discoverable */
#define BT_LE_AD_NO_BREDR               0x04 /* BR/EDR not supported */

#define sys_put_be16(a,b) put_be16(b, a)
#define sys_put_le16(a,b) put_le16(b, a)
#define sys_put_le24(a,b) put_le24(b, a)
#define sys_put_be24(a,b) put_be24(b, a)
#define sys_put_be32(a,b) put_be32(b, a)
#define sys_get_be16(a) get_be16(a)
#define sys_get_be24(a) get_be24(a)
#define sys_get_le16(a) get_le16(a)
#define sys_get_le24(a) get_le24(a)
#define sys_get_be32(a) get_be32(a)
#define sys_cpu_to_be16(a) htobe16(a)
#define sys_cpu_to_be32(a) htobe32(a)
#define sys_be32_to_cpu(a) be32toh(a)
#define sys_be16_to_cpu(a) be16toh(a)
#define sys_le16_to_cpu(a) le16toh(a)

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

#define CODE_UNREACHABLE __builtin_unreachable()
#define __ASSERT(code, str) \
    do {                          \
        if (!(code)) BT_ERR(str); \
        assert(code);             \
    } while (0);

#define __ASSERT_NO_MSG(test) __ASSERT(test, "")

/* Mesh is designed to not use mbuf chains */
#if BT_DBG_ENABLED
#define ASSERT_NOT_CHAIN(om) assert(SLIST_NEXT(om, om_next) == NULL)
#else
#define ASSERT_NOT_CHAIN(om) (void)(om)
#endif

#define __packed    __attribute__((__packed__))

#define MSEC_PER_SEC   (1000)
#define K_MSEC(ms)     (ms)
#define K_SECONDS(s)   K_MSEC((s) * MSEC_PER_SEC)
#define K_MINUTES(m)   K_SECONDS((m) * 60)
#define K_HOURS(h)     K_MINUTES((h) * 60)

#ifndef BIT
#define BIT(n)  (1UL << (n))
#endif

#define BIT_MASK(n) (BIT(n) - 1)

#define BT_GAP_ADV_FAST_INT_MIN_1               0x0030  /* 30 ms    */
#define BT_GAP_ADV_FAST_INT_MAX_1               0x0060  /* 60 ms    */
#define BT_GAP_ADV_FAST_INT_MIN_2               0x00a0  /* 100 ms   */
#define BT_GAP_ADV_FAST_INT_MAX_2               0x00f0  /* 150 ms   */
#define BT_GAP_ADV_SLOW_INT_MIN                 0x0640  /* 1 s      */
#define BT_GAP_ADV_SLOW_INT_MAX                 0x0780  /* 1.2 s    */

#ifndef MESH_LOG_MODULE
#define MESH_LOG_MODULE BLE_MESH_LOG
#endif

#define CAT(a, ...) PRIMITIVE_CAT(a, __VA_ARGS__)
#define PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__

#define BLE_MESH_LOG(lvl, ...) CAT(MESH_LOG_MODULE, CAT(_, lvl))(__VA_ARGS__)

#define BT_DBG(fmt, ...)    BLE_MESH_LOG(DEBUG, "%s: " fmt "\n", __func__, ## __VA_ARGS__);
#define BT_INFO(fmt, ...)   BLE_MESH_LOG(INFO, "%s: " fmt "\n", __func__, ## __VA_ARGS__);
#define BT_WARN(fmt, ...)   BLE_MESH_LOG(WARN, "%s: " fmt "\n", __func__, ## __VA_ARGS__);
#define BT_ERR(fmt, ...)    BLE_MESH_LOG(ERROR, "%s: " fmt "\n", __func__, ## __VA_ARGS__);
#define BT_GATT_ERR(_att_err)   (-(_att_err))

typedef ble_addr_t bt_addr_le_t;

#define k_fifo_init(queue) ble_npl_eventq_init(queue)
#define net_buf_simple_tailroom(buf) OS_MBUF_TRAILINGSPACE(buf)
#define net_buf_tailroom(buf) net_buf_simple_tailroom(buf)
#define net_buf_headroom(buf) ((buf)->om_data - &(buf)->om_databuf[buf->om_pkthdr_len])
#define net_buf_simple_headroom(buf) net_buf_headroom(buf)
#define net_buf_simple_tail(buf) ((buf)->om_data + (buf)->om_len)

struct net_buf_simple_state {
    /** Offset of the data pointer from the beginning of the storage */
    uint16_t offset;
    /** Length of data */
    uint16_t len;
};

static inline struct os_mbuf * NET_BUF_SIMPLE(uint16_t size)
{
    struct os_mbuf *buf;

    buf = os_msys_get(size, 0);
    assert(buf);

    return buf;
}

#define K_NO_WAIT (0)
#define K_FOREVER (-1)

#if MYNEWT_VAL(BLE_EXT_ADV)
#define BT_MESH_ADV_INST     (MYNEWT_VAL(BLE_MULTI_ADV_INSTANCES))

#if MYNEWT_VAL(BLE_MESH_PROXY)
/* Note that BLE_MULTI_ADV_INSTANCES contains number of additional instances.
 * Instance 0 is always there
 */
#if MYNEWT_VAL(BLE_MULTI_ADV_INSTANCES) < 1
#error "Mesh needs at least BLE_MULTI_ADV_INSTANCES set to 1"
#endif
#define BT_MESH_ADV_GATT_INST     (MYNEWT_VAL(BLE_MULTI_ADV_INSTANCES) - 1)
#endif /* BLE_MESH_PROXY */
#endif /* BLE_EXT_ADV */

/* This is by purpose */
static inline void net_buf_simple_init(struct os_mbuf *buf,
                                       size_t reserve_head)
{
    /* This is called in Zephyr after init.
     * Note in Mynewt case we don't care abour reserved head*/
    buf->om_data = &buf->om_databuf[buf->om_pkthdr_len] + reserve_head;
    buf->om_len = 0;
}

#define net_buf_simple_init_with_data(buf, data, size)  \
    os_mbuf_copyinto(buf, 0, data, size);

static inline void net_buf_simple_reset(struct os_mbuf *om)
{
    net_buf_simple_init(om, 0);
}

void net_buf_put(struct ble_npl_eventq *fifo, struct os_mbuf *buf);
void * net_buf_ref(struct os_mbuf *om);
void net_buf_unref(struct os_mbuf *om);
uint16_t net_buf_simple_pull_le16(struct os_mbuf *om);
uint16_t net_buf_simple_pull_be16(struct os_mbuf *om);
uint32_t net_buf_simple_pull_be32(struct os_mbuf *om);
uint32_t net_buf_simple_pull_le32(struct os_mbuf *om);
uint8_t net_buf_simple_pull_u8(struct os_mbuf *om);
void net_buf_simple_add_le16(struct os_mbuf *om, uint16_t val);
void net_buf_simple_add_be16(struct os_mbuf *om, uint16_t val);
void net_buf_simple_add_le24(struct os_mbuf *om, uint32_t val);
void net_buf_simple_add_u8(struct os_mbuf *om, uint8_t val);
void net_buf_simple_add_be32(struct os_mbuf *om, uint32_t val);
void net_buf_simple_add_le32(struct os_mbuf *om, uint32_t val);
void net_buf_add_zeros(struct os_mbuf *om, uint8_t len);
void net_buf_simple_push_le16(struct os_mbuf *om, uint16_t val);
void net_buf_simple_push_be16(struct os_mbuf *om, uint16_t val);
void net_buf_simple_push_be24(struct os_mbuf *om, uint32_t val);
void net_buf_simple_push_u8(struct os_mbuf *om, uint8_t val);
void *net_buf_simple_pull(struct os_mbuf *om, uint8_t len);
void *net_buf_simple_pull_mem(struct os_mbuf *om, uint8_t len);
void *net_buf_simple_add(struct os_mbuf *om, uint8_t len);
bool k_fifo_is_empty(struct ble_npl_eventq *q);
void *net_buf_get(struct ble_npl_eventq *fifo,int32_t t);
uint8_t *net_buf_simple_push(struct os_mbuf *om, uint8_t len);
void net_buf_reserve(struct os_mbuf *om, size_t reserve);

#define net_buf_add_mem(a,b,c) os_mbuf_append(a,b,c)
#define net_buf_simple_add_mem(a,b,c) os_mbuf_append(a,b,c)
#define net_buf_add_u8(a,b) net_buf_simple_add_u8(a,b)
#define net_buf_add(a,b) net_buf_simple_add(a,b)

#define net_buf_clone(a, b) os_mbuf_dup(a)
#define net_buf_add_be32(a, b) net_buf_simple_add_be32(a, b)
#define net_buf_add_be16(a, b) net_buf_simple_add_be16(a, b)
#define net_buf_pull(a, b) net_buf_simple_pull_mem(a, b)
#define net_buf_pull_mem(a, b) net_buf_simple_pull_mem(a, b)
#define net_buf_pull_u8(a) net_buf_simple_pull_u8(a)
#define net_buf_pull_be16(a) net_buf_simple_pull_be16(a)
#define net_buf_skip(a, b) net_buf_simple_pull_mem(a, b)

#define BT_GATT_CCC_NOTIFY BLE_GATT_CHR_PROP_NOTIFY

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

/** Description of different data types that can be encoded into
  * advertising data. Used to form arrays that are passed to the
  * bt_le_adv_start() function.
  */
struct bt_data {
    uint8_t type;
    uint8_t data_len;
    const uint8_t *data;
};

struct bt_pub_key_cb {
    /** @brief Callback type for Public Key generation.
     *
     *  Used to notify of the local public key or that the local key is not
     *  available (either because of a failure to read it or because it is
     *  being regenerated).
     *
     *  @param key The local public key, or NULL in case of no key.
     */
    void (*func)(const uint8_t key[64]);

    struct bt_pub_key_cb *_next;
};

typedef void (*bt_dh_key_cb_t)(const uint8_t key[32]);
int bt_dh_key_gen(const uint8_t remote_pk[64], bt_dh_key_cb_t cb);
int bt_pub_key_gen(struct bt_pub_key_cb *new_cb);
uint8_t *bt_pub_key_get(void);
int bt_rand(void *buf, size_t len);
const char * bt_hex(const void *buf, size_t len);
int bt_encrypt_be(const uint8_t *key, const uint8_t *plaintext, uint8_t *enc_data);
int bt_ccm_decrypt(const uint8_t key[16], uint8_t nonce[13], const uint8_t *enc_data,
		   size_t len, const uint8_t *aad, size_t aad_len,
		   uint8_t *plaintext, size_t mic_size);
int bt_ccm_encrypt(const uint8_t key[16], uint8_t nonce[13], const uint8_t *enc_data,
		   size_t len, const uint8_t *aad, size_t aad_len,
		   uint8_t *plaintext, size_t mic_size);
void bt_mesh_register_gatt(void);
int bt_le_adv_start(const struct ble_gap_adv_params *param,
                    const struct bt_data *ad, size_t ad_len,
                    const struct bt_data *sd, size_t sd_len);
int bt_le_adv_stop(bool proxy);

struct k_delayed_work {
    struct ble_npl_callout work;
};

void k_work_init(struct ble_npl_callout *work, ble_npl_event_fn handler);
void k_delayed_work_init(struct k_delayed_work *w, ble_npl_event_fn *f);
void k_delayed_work_cancel(struct k_delayed_work *w);
bool k_delayed_work_pending(struct k_delayed_work *w);
void k_delayed_work_submit(struct k_delayed_work *w, uint32_t ms);
int64_t k_uptime_get(void);
uint32_t k_uptime_get_32(void);
void k_sleep(int32_t duration);
void k_work_submit(struct ble_npl_callout *w);
void k_work_add_arg(struct ble_npl_callout *w, void *arg);
void k_delayed_work_add_arg(struct k_delayed_work *w, void *arg);
uint32_t k_delayed_work_remaining_get(struct k_delayed_work *w);

static inline void net_buf_simple_save(struct os_mbuf *buf,
                       struct net_buf_simple_state *state)
{
    state->offset = net_buf_simple_headroom(buf);
    state->len = buf->om_len;
}

static inline void net_buf_simple_restore(struct os_mbuf *buf,
                                          struct net_buf_simple_state *state)
{
      buf->om_data = &buf->om_databuf[buf->om_pkthdr_len] + state->offset;
      buf->om_len = state->len;
}

static inline void sys_memcpy_swap(void *dst, const void *src, size_t length)
{
    __ASSERT(((src < dst && (src + length) <= dst) ||
          (src > dst && (dst + length) <= src)),
         "Source and destination buffers must not overlap");

    src += length - 1;

    for (; length > 0; length--) {
        *((uint8_t *)dst++) = *((uint8_t *)src--);
    }
}

#define popcount(x) __builtin_popcount(x)

static inline unsigned int find_lsb_set(uint32_t op)
{
    return __builtin_ffs(op);
}

static inline unsigned int find_msb_set(uint32_t op)
{
    if (!op)
        return 0;

    return 32 - __builtin_clz(op);
}

#define CONFIG_BT_MESH_FRIEND                 BLE_MESH_FRIEND
#define CONFIG_BT_MESH_GATT_PROXY             BLE_MESH_GATT_PROXY
#define CONFIG_BT_MESH_IV_UPDATE_TEST         BLE_MESH_IV_UPDATE_TEST
#define CONFIG_BT_MESH_LOW_POWER              BLE_MESH_LOW_POWER
#define CONFIG_BT_MESH_LPN_SUB_ALL_NODES_ADDR BLE_MESH_LPN_SUB_ALL_NODES_ADDR
#define CONFIG_BT_MESH_LPN_AUTO               BLE_MESH_LPN_AUTO
#define CONFIG_BT_MESH_LPN_ESTABLISHMENT      BLE_MESH_LPN_ESTABLISHMENT
#define CONFIG_BT_MESH_PB_ADV                 BLE_MESH_PB_ADV
#define CONFIG_BT_MESH_PB_GATT                BLE_MESH_PB_GATT
#define CONFIG_BT_MESH_PROV                   BLE_MESH_PROV
#define CONFIG_BT_MESH_PROXY                  BLE_MESH_PROXY
#define CONFIG_BT_TESTING                     BLE_MESH_TESTING
#define CONFIG_BT_SETTINGS                    BLE_MESH_SETTINGS
#define CONFIG_SETTINGS                       BLE_MESH_SETTINGS
#define CONFIG_BT_MESH_PROVISIONER            BLE_MESH_PROVISIONER
#define CONFIG_BT_MESH_PROV_DEVICE            BLE_MESH_PROV_DEVICE
#define CONFIG_BT_MESH_CDB                    BLE_MESH_CDB

/* Above flags are used with IS_ENABLED macro */
#define IS_ENABLED(config) MYNEWT_VAL(config)

#define CONFIG_BT_MESH_LPN_GROUPS                MYNEWT_VAL(BLE_MESH_LPN_GROUPS)
#define CONFIG_BT_MESH_ADV_BUF_COUNT             MYNEWT_VAL(BLE_MESH_ADV_BUF_COUNT)
#define CONFIG_BT_MESH_SEG_BUFS                  MYNEWT_VAL(BLE_MESH_SEG_BUFS )
#define CONFIG_BT_MESH_FRIEND_QUEUE_SIZE         MYNEWT_VAL(BLE_MESH_FRIEND_QUEUE_SIZE)
#define CONFIG_BT_MESH_FRIEND_RECV_WIN           MYNEWT_VAL(BLE_MESH_FRIEND_RECV_WIN)
#define CONFIG_BT_MESH_LPN_POLL_TIMEOUT          MYNEWT_VAL(BLE_MESH_LPN_POLL_TIMEOUT)
#define CONFIG_BT_MESH_MODEL_GROUP_COUNT         MYNEWT_VAL(BLE_MESH_MODEL_GROUP_COUNT)
#define CONFIG_BT_MESH_MODEL_KEY_COUNT           MYNEWT_VAL(BLE_MESH_MODEL_KEY_COUNT)
#define CONFIG_BT_MESH_NODE_ID_TIMEOUT           MYNEWT_VAL(BLE_MESH_NODE_ID_TIMEOUT)
#define CONFIG_BT_MAX_CONN                       MYNEWT_VAL(BLE_MAX_CONNECTIONS)
#define CONFIG_BT_MESH_SEQ_STORE_RATE            MYNEWT_VAL(BLE_MESH_SEQ_STORE_RATE)
#define CONFIG_BT_MESH_RPL_STORE_TIMEOUT         MYNEWT_VAL(BLE_MESH_RPL_STORE_TIMEOUT)
#define CONFIG_BT_MESH_APP_KEY_COUNT             MYNEWT_VAL(BLE_MESH_APP_KEY_COUNT)
#define CONFIG_BT_MESH_SUBNET_COUNT              MYNEWT_VAL(BLE_MESH_SUBNET_COUNT)
#define CONFIG_BT_MESH_STORE_TIMEOUT             MYNEWT_VAL(BLE_MESH_STORE_TIMEOUT)
#define CONFIG_BT_MESH_IVU_DIVIDER               MYNEWT_VAL(BLE_MESH_IVU_DIVIDER)
#define CONFIG_BT_DEVICE_NAME                    MYNEWT_VAL(BLE_MESH_DEVICE_NAME)
#define CONFIG_BT_RX_SEG_MAX                     MYNEWT_VAL(BLE_MESH_RX_SEG_MAX)
#define CONFIG_BT_MESH_TX_SEG_MAX                MYNEWT_VAL(BLE_MESH_TX_SEG_MAX)
#define CONFIG_BT_MESH_RX_SEG_MAX                MYNEWT_VAL(BLE_MESH_RX_SEG_MAX)
#define CONFIG_BT_MESH_RX_SEG_MSG_COUNT          MYNEWT_VAL(BLE_MESH_RX_SEG_MSG_COUNT)
#define CONFIG_BT_MESH_LABEL_COUNT               MYNEWT_VAL(BLE_MESH_LABEL_COUNT)
#define CONFIG_BT_MESH_NODE_COUNT                MYNEWT_VAL(BLE_MESH_CDB_NODE_COUNT)
#define CONFIG_BT_GATT_PROXY_ENABLED             MYNEWT_VAL(BLE_MESH_GATT_PROXY_ENABLED)
#define CONFIG_BT_MESH_DEFAULT_TTL               MYNEWT_VAL(BLE_MESH_DEFAULT_TTL)
#define CONFIG_BT_MESH_NETWORK_TRANSMIT_COUNT    MYNEWT_VAL(BLE_MESH_NETWORK_TRANSMIT_COUNT)
#define CONFIG_BT_MESH_NETWORK_TRANSMIT_INTERVAL MYNEWT_VAL(BLE_MESH_NETWORK_TRANSMIT_INTERVAL)
#define CONFIG_BT_MESH_RELAY_ENABLED             MYNEWT_VAL(BLE_MESH_RELAY_ENABLED)
#define CONFIG_BT_MESH_RELAY_RETRANSMIT_INTERVAL MYNEWT_VAL(BLE_MESH_RELAY_RETRANSMIT_INTERVAL)
#define CONFIG_BT_MESH_BEACON_ENABLED            MYNEWT_VAL(BLE_MESH_BEACON_ENABLED)
#define CONFIG_BT_MESH_FRIEND_ENABLED            MYNEWT_VAL(BLE_MESH_FRIEND_ENABLED)
#define CONFIG_BT_MESH_RELAY                     MYNEWT_VAL(BLE_MESH_RELAY)
#define CONFIG_BT_MESH_RELAY_RETRANSMIT_COUNT    MYNEWT_VAL(BLE_MESH_RELAY_RETRANSMIT_COUNT)
#define CONFIG_BT_MESH_GATT_PROXY_ENABLED        MYNEWT_VAL(BLE_MESH_GATT_PROXY_ENABLED)

#define printk console_printf

#define CONTAINER_OF(ptr, type, field) \
	((type *)(((char *)(ptr)) - offsetof(type, field)))


#define k_sem ble_npl_sem

static inline void k_sem_init(struct k_sem *sem, unsigned int initial_count,
			      unsigned int limit)
{
	ble_npl_sem_init(sem, initial_count);
}

static inline int k_sem_take(struct k_sem *sem, int32_t timeout)
{
	uint32_t ticks;

	ble_npl_time_ms_to_ticks(timeout, &ticks);
	return - ble_npl_sem_pend(sem,  ticks);
}

static inline void k_sem_give(struct k_sem *sem)
{
	ble_npl_sem_release(sem);
}

/* Helpers to access the storage array, since we don't have access to its
 * type at this point anymore.
 */

#define BUF_SIZE(pool) (pool->omp_pool->mp_block_size)

static inline int net_buf_id(struct os_mbuf *buf)
{
	struct os_mbuf_pool *pool = buf->om_omp;
	uint8_t *pool_start = (uint8_t *)pool->omp_pool->mp_membuf_addr;
	uint8_t *buf_ptr = (uint8_t *)buf;

	return (buf_ptr - pool_start) / BUF_SIZE(pool);
}

/* XXX: We should not use os_mbuf_pkthdr chains to represent a list of
 * packets, this is a hack. For now this is not an issue, because mesh
 * does not use os_mbuf chains. We should change this in the future.
 */
STAILQ_HEAD(net_buf_slist_t, os_mbuf_pkthdr);

void net_buf_slist_init(struct net_buf_slist_t *list);
bool net_buf_slist_is_empty(struct net_buf_slist_t *list);
struct os_mbuf *net_buf_slist_peek_head(struct net_buf_slist_t *list);
struct os_mbuf *net_buf_slist_peek_next(struct os_mbuf *buf);
struct os_mbuf *net_buf_slist_get(struct net_buf_slist_t *list);
void net_buf_slist_put(struct net_buf_slist_t *list, struct os_mbuf *buf);
void net_buf_slist_remove(struct net_buf_slist_t *list, struct os_mbuf *prev,
			  struct os_mbuf *cur);
void net_buf_slist_merge_slist(struct net_buf_slist_t *list,
			       struct net_buf_slist_t *list_to_append);
#define NET_BUF_SLIST_FOR_EACH_NODE(head, var) STAILQ_FOREACH(var, head, omp_next)

#if MYNEWT_VAL(BLE_MESH_SETTINGS)

#define settings_load conf_load
int settings_bytes_from_str(char *val_str, void *vp, int *len);
char *settings_str_from_bytes(const void *vp, int vp_len,
			      char *buf, int buf_len);

#define snprintk snprintf
#define BT_SETTINGS_SIZE(in_size) ((((((in_size) - 1) / 3) * 4) + 4) + 1)
#define settings_save_one conf_save_one

#else

static inline int
settings_load(void)
{
	return 0;
}

#endif /* MYNEWT_VAL(MYNEWT_VAL_BLE_MESH_SETTINGS) */

#define BUILD_ASSERT(cond) _Static_assert(cond, "")


/* Memory slabs/blocks */

/** Memory slab structure */
struct k_mem_slab {
    /**
     * _wait_q_t is not required now, as we don't implement zephyr timeouts -
     * if slab couldn't be allocated, we simply return error  
     */
    uint32_t num_blocks; /** number of memory blocks available for allocation */
    size_t block_size; /** size of single block */
	/**
     * buffer for blocks - must be alligned to N-byte, where N is a power of 2.
     * Minimal size of buffer is num_blocks * block_size
     */
    char *buffer;
	char *free_list; /** list of free memory blocks */
	uint32_t num_used; /** count of used memory blocks */
};

struct k_mem_block_id {
	uint32_t pool : 8;
	uint32_t level : 4;
	uint32_t block : 20;
};

struct k_mem_block {
	void *data;
	struct k_mem_block_id id;
};

extern void k_mem_slab_free(struct k_mem_slab *slab, void **mem);
extern int k_mem_slab_alloc(struct k_mem_slab *slab, void **mem);
static inline uint32_t k_mem_slab_num_free_get(struct k_mem_slab *slab)
{
	return slab->num_blocks - slab->num_used;
}

int create_free_list(struct k_mem_slab *slab);

#ifdef __cplusplus
}
#endif

#endif /* _MESH_GLUE_ */
