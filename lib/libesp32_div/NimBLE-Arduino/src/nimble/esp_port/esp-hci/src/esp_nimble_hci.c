/*
 * SPDX-FileCopyrightText: 2015-2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifdef ESP_PLATFORM

#include <assert.h>
#include "nimble/porting/nimble/include/sysinit/sysinit.h"
#include "nimble/nimble/include/nimble/hci_common.h"
#include "nimble/nimble/host/include/host/ble_hs.h"
#include "nimble/porting/nimble/include/nimble/nimble_port.h"
#include "nimble/porting/npl/freertos/include/nimble/nimble_port_freertos.h"
#include "../include/esp_nimble_hci.h"
#include "../../port/include/esp_nimble_mem.h"
#include <esp_bt.h>
#include <freertos/semphr.h>
#include "../include/esp_compiler.h"
/* IPC is used to improve performance when calls come from a processor not running the NimBLE stack */
/* but does not exist for solo */
#ifndef CONFIG_FREERTOS_UNICORE
  #include "esp_ipc.h"
#endif

#define NIMBLE_VHCI_TIMEOUT_MS  2000
#define BLE_HCI_EVENT_HDR_LEN               (2)
#define BLE_HCI_CMD_HDR_LEN                 (3)

static ble_hci_trans_rx_cmd_fn *ble_hci_rx_cmd_hs_cb;
static void *ble_hci_rx_cmd_hs_arg;

static ble_hci_trans_rx_acl_fn *ble_hci_rx_acl_hs_cb;
static void *ble_hci_rx_acl_hs_arg;

static struct os_mbuf_pool ble_hci_acl_mbuf_pool;
static struct os_mempool_ext ble_hci_acl_pool;
/*
 * The MBUF payload size must accommodate the HCI data header size plus the
 * maximum ACL data packet length. The ACL block size is the size of the
 * mbufs we will allocate.
 */
#define ACL_BLOCK_SIZE  OS_ALIGN(MYNEWT_VAL(BLE_ACL_BUF_SIZE) \
        + BLE_MBUF_MEMBLOCK_OVERHEAD \
        + BLE_HCI_DATA_HDR_SZ, OS_ALIGNMENT)

static os_membuf_t *ble_hci_acl_buf;

static struct os_mempool ble_hci_cmd_pool;
static os_membuf_t *ble_hci_cmd_buf;

static struct os_mempool ble_hci_evt_hi_pool;
static os_membuf_t *ble_hci_evt_hi_buf;

static struct os_mempool ble_hci_evt_lo_pool;
static os_membuf_t *ble_hci_evt_lo_buf;

static SemaphoreHandle_t vhci_send_sem;
const static char *TAG = "NimBLE";

int os_msys_buf_alloc(void);
void os_msys_buf_free(void);

void ble_hci_trans_cfg_hs(ble_hci_trans_rx_cmd_fn *cmd_cb,
                     void *cmd_arg,
                     ble_hci_trans_rx_acl_fn *acl_cb,
                     void *acl_arg)
{
    ble_hci_rx_cmd_hs_cb = cmd_cb;
    ble_hci_rx_cmd_hs_arg = cmd_arg;
    ble_hci_rx_acl_hs_cb = acl_cb;
    ble_hci_rx_acl_hs_arg = acl_arg;
}

/* Added; Called from the core NimBLE is running on, not used for unicore */
#ifndef CONFIG_FREERTOS_UNICORE
void ble_hci_trans_hs_cmd_tx_on_core(void *arg)
{
    // Ugly but necessary as the arduino core does not provide enough IPC stack for variables.
    esp_vhci_host_send_packet((uint8_t*)arg, *((uint8_t*)arg + 3) + 1 + BLE_HCI_CMD_HDR_LEN);
}
#endif

/* Modified to use ipc calls in arduino to correct performance issues */
int ble_hci_trans_hs_cmd_tx(uint8_t *cmd)
{
    uint16_t len;
    uint8_t rc = 0;

    assert(cmd != NULL);
    *cmd = BLE_HCI_UART_H4_CMD;
    len = BLE_HCI_CMD_HDR_LEN + cmd[3] + 1;
    if (!esp_vhci_host_check_send_available()) {
        ESP_LOGD(TAG, "Controller not ready to receive packets");
    }

    if (xSemaphoreTake(vhci_send_sem, NIMBLE_VHCI_TIMEOUT_MS / portTICK_PERIOD_MS) == pdTRUE) {
/* esp_ipc_call_blocking does not exist for solo */
#ifndef CONFIG_FREERTOS_UNICORE
        if (xPortGetCoreID() != CONFIG_BT_NIMBLE_PINNED_TO_CORE && !xPortInIsrContext()) {
            esp_ipc_call_blocking(CONFIG_BT_NIMBLE_PINNED_TO_CORE,
                                  ble_hci_trans_hs_cmd_tx_on_core, cmd);
        } else {
            esp_vhci_host_send_packet(cmd, len);
        }
#else /* Unicore */
        esp_vhci_host_send_packet(cmd, len);
#endif
    } else {
        rc = BLE_HS_ETIMEOUT_HCI;
    }

    ble_hci_trans_buf_free(cmd);
    return rc;
}

int ble_hci_trans_ll_evt_tx(uint8_t *hci_ev)
{
    int rc = ESP_FAIL;

    if (ble_hci_rx_cmd_hs_cb) {
        rc = ble_hci_rx_cmd_hs_cb(hci_ev, ble_hci_rx_cmd_hs_arg);
    }
    return rc;
}

/* Added; Called from the core NimBLE is running on, not used for unicore */
#ifndef CONFIG_FREERTOS_UNICORE
void ble_hci_trans_hs_acl_tx_on_core(void *arg)
{
    // Ugly but necessary as the arduino core does not provide enough IPC stack for variables.
    esp_vhci_host_send_packet((uint8_t*)arg + 2, *(uint16_t*)arg);
}
#endif

/* Modified to use ipc calls in arduino to correct performance issues */
int ble_hci_trans_hs_acl_tx(struct os_mbuf *om)
{
    uint16_t len = 0;
    uint8_t data[MYNEWT_VAL(BLE_ACL_BUF_SIZE) + 3], rc = 0;
#ifndef CONFIG_FREERTOS_UNICORE
    bool tx_using_nimble_core = 0;
#endif
    /* If this packet is zero length, just free it */
    if (OS_MBUF_PKTLEN(om) == 0) {
        os_mbuf_free_chain(om);
        return 0;
    }

    if (!esp_vhci_host_check_send_available()) {
        ESP_LOGD(TAG, "Controller not ready to receive packets");
    }

    len = 1 + OS_MBUF_PKTLEN(om);
/* Don't check core ID if unicore */
#ifndef CONFIG_FREERTOS_UNICORE
    tx_using_nimble_core = xPortGetCoreID() != CONFIG_BT_NIMBLE_PINNED_TO_CORE;
    if (tx_using_nimble_core && !xPortInIsrContext()) {
        data[0] = len;
        data[1] = (len >> 8);
        data[2] = BLE_HCI_UART_H4_ACL;
        os_mbuf_copydata(om, 0, OS_MBUF_PKTLEN(om), &data[3]);
    } else {
        data[0] = BLE_HCI_UART_H4_ACL;
        os_mbuf_copydata(om, 0, OS_MBUF_PKTLEN(om), &data[1]);
    }
#else /* Unicore */
    data[0] = BLE_HCI_UART_H4_ACL;
    os_mbuf_copydata(om, 0, OS_MBUF_PKTLEN(om), &data[1]);
#endif

    if (xSemaphoreTake(vhci_send_sem, NIMBLE_VHCI_TIMEOUT_MS / portTICK_PERIOD_MS) == pdTRUE) {
/* esp_ipc_call_blocking does not exist for solo */
#ifndef CONFIG_FREERTOS_UNICORE
        if (tx_using_nimble_core && !xPortInIsrContext()) {
            esp_ipc_call_blocking(CONFIG_BT_NIMBLE_PINNED_TO_CORE,
                                  ble_hci_trans_hs_acl_tx_on_core, data);
        } else {
            esp_vhci_host_send_packet(data, len);
        }
#else /* Unicore */
        esp_vhci_host_send_packet(data, len);
#endif
    } else {
        rc = BLE_HS_ETIMEOUT_HCI;
    }

    os_mbuf_free_chain(om);

    return rc;
}

int ble_hci_trans_ll_acl_tx(struct os_mbuf *om)
{
    int rc = ESP_FAIL;

    if (ble_hci_rx_acl_hs_cb) {
        rc = ble_hci_rx_acl_hs_cb(om, ble_hci_rx_acl_hs_arg);
    }
    return rc;
}

uint8_t *ble_hci_trans_buf_alloc(int type)
{
    uint8_t *buf;

    switch (type) {
    case BLE_HCI_TRANS_BUF_CMD:
        buf = os_memblock_get(&ble_hci_cmd_pool);
        break;

    case BLE_HCI_TRANS_BUF_EVT_HI:
        buf = os_memblock_get(&ble_hci_evt_hi_pool);
        if (buf == NULL) {
            /* If no high-priority event buffers remain, try to grab a
             * low-priority one.
             */
            buf = ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_EVT_LO);
        }
        break;

    case BLE_HCI_TRANS_BUF_EVT_LO:
        buf = os_memblock_get(&ble_hci_evt_lo_pool);
        break;

    default:
        assert(0);
        buf = NULL;
    }

    return buf;
}

void ble_hci_trans_buf_free(uint8_t *buf)
{
    int rc;
    /* XXX: this may look a bit odd, but the controller uses the command
    * buffer to send back the command complete/status as an immediate
    * response to the command. This was done to insure that the controller
    * could always send back one of these events when a command was received.
    * Thus, we check to see which pool the buffer came from so we can free
    * it to the appropriate pool
    */
    if (os_memblock_from(&ble_hci_evt_hi_pool, buf)) {
        rc = os_memblock_put(&ble_hci_evt_hi_pool, buf);
        assert(rc == 0);
    } else if (os_memblock_from(&ble_hci_evt_lo_pool, buf)) {
        rc = os_memblock_put(&ble_hci_evt_lo_pool, buf);
        assert(rc == 0);
    } else {
        assert(os_memblock_from(&ble_hci_cmd_pool, buf));
        rc = os_memblock_put(&ble_hci_cmd_pool, buf);
        assert(rc == 0);
    }
}

/**
 * Unsupported; the RAM transport does not have a dedicated ACL data packet
 * pool.
 */
int ble_hci_trans_set_acl_free_cb(os_mempool_put_fn *cb, void *arg)
{
    ble_hci_acl_pool.mpe_put_cb = cb;
    ble_hci_acl_pool.mpe_put_arg = arg;
    return 0;
}

int ble_hci_trans_reset(void)
{
    /* No work to do.  All allocated buffers are owned by the host or
     * controller, and they will get freed by their owners.
     */
    return 0;
}

/**
 * Allocates a buffer (mbuf) for ACL operation.
 *
 * @return                      The allocated buffer on success;
 *                              NULL on buffer exhaustion.
 */
static struct os_mbuf *ble_hci_trans_acl_buf_alloc(void)
{
    struct os_mbuf *m;
    uint8_t usrhdr_len;

#if MYNEWT_VAL(BLE_DEVICE)
    usrhdr_len = sizeof(struct ble_mbuf_hdr);
#elif MYNEWT_VAL(BLE_HS_FLOW_CTRL)
    usrhdr_len = BLE_MBUF_HS_HDR_LEN;
#else
    usrhdr_len = 0;
#endif

    m = os_mbuf_get_pkthdr(&ble_hci_acl_mbuf_pool, usrhdr_len);
    return m;
}

static void ble_hci_rx_acl(uint8_t *data, uint16_t len)
{
    struct os_mbuf *m;
    int rc;
    int sr;
    if (len < BLE_HCI_DATA_HDR_SZ || len > MYNEWT_VAL(BLE_ACL_BUF_SIZE)) {
        return;
    }

    m = ble_hci_trans_acl_buf_alloc();

    if (!m) {
        ESP_LOGE(TAG, "%s failed to allocate ACL buffers; increase ACL_BUF_COUNT", __func__);
        return;
    }
    if ((rc = os_mbuf_append(m, data, len)) != 0) {
        ESP_LOGE(TAG, "%s failed to os_mbuf_append; rc = %d", __func__, rc);
        os_mbuf_free_chain(m);
        return;
    }
    OS_ENTER_CRITICAL(sr);
    if (ble_hci_rx_acl_hs_cb) {
        ble_hci_rx_acl_hs_cb(m, NULL);
    }
    OS_EXIT_CRITICAL(sr);
}

static void ble_hci_transport_init(void)
{
    int rc;

    /* Ensure this function only gets called by sysinit. */
    SYSINIT_ASSERT_ACTIVE();

    rc = os_mempool_ext_init(&ble_hci_acl_pool,
                             MYNEWT_VAL(BLE_ACL_BUF_COUNT),
                             ACL_BLOCK_SIZE,
                             ble_hci_acl_buf,
                             "ble_hci_acl_pool");
    SYSINIT_PANIC_ASSERT(rc == 0);

    rc = os_mbuf_pool_init(&ble_hci_acl_mbuf_pool,
                           &ble_hci_acl_pool.mpe_mp,
                           ACL_BLOCK_SIZE,
                           MYNEWT_VAL(BLE_ACL_BUF_COUNT));
    SYSINIT_PANIC_ASSERT(rc == 0);

    /*
     * Create memory pool of HCI command buffers. NOTE: we currently dont
     * allow this to be configured. The controller will only allow one
     * outstanding command. We decided to keep this a pool in case we allow
     * allow the controller to handle more than one outstanding command.
     */
    rc = os_mempool_init(&ble_hci_cmd_pool,
                         1,
                         BLE_HCI_TRANS_CMD_SZ,
                         ble_hci_cmd_buf,
                         "ble_hci_cmd_pool");
    SYSINIT_PANIC_ASSERT(rc == 0);

    rc = os_mempool_init(&ble_hci_evt_hi_pool,
                         MYNEWT_VAL(BLE_HCI_EVT_HI_BUF_COUNT),
                         MYNEWT_VAL(BLE_HCI_EVT_BUF_SIZE),
                         ble_hci_evt_hi_buf,
                         "ble_hci_evt_hi_pool");
    SYSINIT_PANIC_ASSERT(rc == 0);

    rc = os_mempool_init(&ble_hci_evt_lo_pool,
                         MYNEWT_VAL(BLE_HCI_EVT_LO_BUF_COUNT),
                         MYNEWT_VAL(BLE_HCI_EVT_BUF_SIZE),
                         ble_hci_evt_lo_buf,
                         "ble_hci_evt_lo_pool");
    SYSINIT_PANIC_ASSERT(rc == 0);
}

/*
 * @brief: BT controller callback function, used to notify the upper layer that
 *         controller is ready to receive command
 */
static void controller_rcv_pkt_ready(void)
{
    if (vhci_send_sem) {
        xSemaphoreGive(vhci_send_sem);
    }
}

/*
 * @brief: BT controller callback function, to transfer data packet to the host
 */
static int host_rcv_pkt(uint8_t *data, uint16_t len)
{

    if (data[0] == BLE_HCI_UART_H4_EVT) {
        uint8_t *evbuf;
        int totlen;
        int rc;

        totlen = BLE_HCI_EVENT_HDR_LEN + data[2];
        assert(totlen <= UINT8_MAX + BLE_HCI_EVENT_HDR_LEN);

        if (totlen > MYNEWT_VAL(BLE_HCI_EVT_BUF_SIZE)) {
            ESP_LOGE(TAG, "Received HCI data length at host (%d) exceeds maximum configured HCI event buffer size (%d).",
                     totlen, MYNEWT_VAL(BLE_HCI_EVT_BUF_SIZE));
            ble_hs_sched_reset(BLE_HS_ECONTROLLER);
            return 0;
        }

        if (data[1] == BLE_HCI_EVCODE_HW_ERROR) {
            assert(0);
        }

        /* Allocate LE Advertising Report Event from lo pool only */
        if ((data[1] == BLE_HCI_EVCODE_LE_META) && (data[3] == BLE_HCI_LE_SUBEV_ADV_RPT)) {
            evbuf = ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_EVT_LO);
            /* Skip advertising report if we're out of memory */
            if (!evbuf) {
                return 0;
            }
        } else {
            evbuf = ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_EVT_HI);
            assert(evbuf != NULL);
        }

        memcpy(evbuf, &data[1], totlen);

        rc = ble_hci_trans_ll_evt_tx(evbuf);
        assert(rc == 0);
    } else if (data[0] == BLE_HCI_UART_H4_ACL) {
        ble_hci_rx_acl(data + 1, len - 1);
    }
    return 0;
}

static const esp_vhci_host_callback_t vhci_host_cb = {
    .notify_host_send_available = controller_rcv_pkt_ready,
    .notify_host_recv = host_rcv_pkt,
};

static void ble_buf_free(void)
{
    os_msys_buf_free();

    nimble_platform_mem_free(ble_hci_evt_hi_buf);
    ble_hci_evt_hi_buf = NULL;
    nimble_platform_mem_free(ble_hci_evt_lo_buf);
    ble_hci_evt_lo_buf = NULL;
    nimble_platform_mem_free(ble_hci_cmd_buf);
    ble_hci_cmd_buf = NULL;
    nimble_platform_mem_free(ble_hci_acl_buf);
    ble_hci_acl_buf = NULL;
}

static esp_err_t ble_buf_alloc(void)
{
    if (os_msys_buf_alloc()) {
        return ESP_ERR_NO_MEM;
    }

    ble_hci_evt_hi_buf = (os_membuf_t *) nimble_platform_mem_calloc(1,
        (sizeof(os_membuf_t) * OS_MEMPOOL_SIZE(MYNEWT_VAL(BLE_HCI_EVT_HI_BUF_COUNT),
        MYNEWT_VAL(BLE_HCI_EVT_BUF_SIZE))));

    ble_hci_evt_lo_buf = (os_membuf_t *) nimble_platform_mem_calloc(1,
        (sizeof(os_membuf_t) * OS_MEMPOOL_SIZE(MYNEWT_VAL(BLE_HCI_EVT_LO_BUF_COUNT),
        MYNEWT_VAL(BLE_HCI_EVT_BUF_SIZE))));

    ble_hci_cmd_buf = (os_membuf_t *) nimble_platform_mem_calloc(1,
        (sizeof(os_membuf_t) * OS_MEMPOOL_SIZE(1, BLE_HCI_TRANS_CMD_SZ)));

    ble_hci_acl_buf = (os_membuf_t *) nimble_platform_mem_calloc(1,
        (sizeof(os_membuf_t) * OS_MEMPOOL_SIZE(MYNEWT_VAL(BLE_ACL_BUF_COUNT),
        ACL_BLOCK_SIZE)));

    if (!ble_hci_evt_hi_buf || !ble_hci_evt_lo_buf || !ble_hci_cmd_buf || !ble_hci_acl_buf) {
        ble_buf_free();
        return ESP_ERR_NO_MEM;
    }
    return ESP_OK;
}

esp_err_t esp_nimble_hci_init(void)
{
    esp_err_t ret;

    ret = ble_buf_alloc();
    if (ret != ESP_OK) {
        goto err;
    }
    if ((ret = esp_vhci_host_register_callback(&vhci_host_cb)) != ESP_OK) {
        goto err;
    }

    ble_hci_transport_init();

    vhci_send_sem = xSemaphoreCreateBinary();
    if (vhci_send_sem == NULL) {
        ret = ESP_ERR_NO_MEM;
        goto err;
    }

    xSemaphoreGive(vhci_send_sem);

    return ret;
err:
    ble_buf_free();
    return ret;

}

esp_err_t esp_nimble_hci_and_controller_init(void)
{
    esp_err_t ret;

    esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    if ((ret = esp_bt_controller_init(&bt_cfg)) != ESP_OK) {
        return ret;
    }

    if ((ret = esp_bt_controller_enable(ESP_BT_MODE_BLE)) != ESP_OK) {
        return ret;
    }
    return esp_nimble_hci_init();
}

static esp_err_t ble_hci_transport_deinit(void)
{
    int ret = 0;

    ret += os_mempool_clear(&ble_hci_evt_lo_pool);

    ret += os_mempool_clear(&ble_hci_evt_hi_pool);

    ret += os_mempool_clear(&ble_hci_cmd_pool);

    ret += os_mempool_ext_clear(&ble_hci_acl_pool);

    if (ret) {
        return ESP_FAIL;
    } else {
        return ESP_OK;
    }
}

esp_err_t esp_nimble_hci_deinit(void)
{
    if (vhci_send_sem) {
        /* Dummy take & give semaphore before deleting */
        xSemaphoreTake(vhci_send_sem, portMAX_DELAY);
        xSemaphoreGive(vhci_send_sem);
        vSemaphoreDelete(vhci_send_sem);
        vhci_send_sem = NULL;
    }
    esp_err_t ret = ble_hci_transport_deinit();
    if (ret != ESP_OK) {
        return ret;
    }

    ble_buf_free();

    return ESP_OK;
}

esp_err_t esp_nimble_hci_and_controller_deinit(void)
{
    int ret;
    ret = esp_nimble_hci_deinit();
    if (ret != ESP_OK) {
        return ret;
    }

    ret = esp_bt_controller_disable();
    if (ret != ESP_OK) {
        return ret;
    }

    ret = esp_bt_controller_deinit();
    if (ret != ESP_OK) {
        return ret;
    }

    return ESP_OK;
}

#endif
