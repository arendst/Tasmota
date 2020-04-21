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

#include "host/ble_monitor.h"

#if BLE_MONITOR

#if MYNEWT_VAL(BLE_MONITOR_UART) && MYNEWT_VAL(BLE_MONITOR_RTT)
#error "Cannot enable monitor over UART and RTT at the same time!"
#endif

#include <stdarg.h>
#include <stdio.h>
#include <inttypes.h>
#include "os/os.h"
#include "log/log.h"
#if MYNEWT_VAL(BLE_MONITOR_UART)
#include "uart/uart.h"
#endif
#if MYNEWT_VAL(BLE_MONITOR_RTT)
#include "rtt/SEGGER_RTT.h"
#endif
#include "ble_hs_priv.h"
#include "ble_monitor_priv.h"

struct ble_npl_mutex lock;

#if MYNEWT_VAL(BLE_MONITOR_UART)
struct uart_dev *uart;

static uint8_t tx_ringbuf[MYNEWT_VAL(BLE_MONITOR_UART_BUFFER_SIZE)];
static uint8_t tx_ringbuf_head;
static uint8_t tx_ringbuf_tail;
#endif

#if MYNEWT_VAL(BLE_MONITOR_RTT)
static uint8_t rtt_buf[MYNEWT_VAL(BLE_MONITOR_RTT_BUFFER_SIZE)];
static int rtt_index;
#if MYNEWT_VAL(BLE_MONITOR_RTT_BUFFERED)
static uint8_t rtt_pktbuf[MYNEWT_VAL(BLE_MONITOR_RTT_BUFFER_SIZE)];
static size_t rtt_pktbuf_pos;
static struct {
    bool dropped;
    struct ble_npl_callout tmo;
    struct ble_monitor_drops_hdr drops_hdr;
} rtt_drops;

#endif
#endif

#if MYNEWT_VAL(BLE_MONITOR_UART)
static inline int
inc_and_wrap(int i, int max)
{
    return (i + 1) & (max - 1);
}

static int
monitor_uart_rx_discard(void *arg, uint8_t ch)
{
    return 0;
}

static int
monitor_uart_tx_char(void *arg)
{
    uint8_t ch;

    /* No more data */
    if (tx_ringbuf_head == tx_ringbuf_tail) {
        return -1;
    }

    ch = tx_ringbuf[tx_ringbuf_tail];
    tx_ringbuf_tail = inc_and_wrap(tx_ringbuf_tail, sizeof(tx_ringbuf));

    return ch;
}

static void
monitor_uart_queue_char(uint8_t ch)
{
    int sr;

    OS_ENTER_CRITICAL(sr);

    /* We need to try flush some data from ringbuffer if full */
    while (inc_and_wrap(tx_ringbuf_head, sizeof(tx_ringbuf)) ==
            tx_ringbuf_tail) {
        uart_start_tx(uart);
        OS_EXIT_CRITICAL(sr);
        if (os_started()) {
            os_time_delay(1);
        }
        OS_ENTER_CRITICAL(sr);
    }

    tx_ringbuf[tx_ringbuf_head] = ch;
    tx_ringbuf_head = inc_and_wrap(tx_ringbuf_head, sizeof(tx_ringbuf));

    OS_EXIT_CRITICAL(sr);
}

static void
monitor_write(const void *buf, size_t len)
{
    const uint8_t *ch = buf;

    while (len--) {
        monitor_uart_queue_char(*ch++);
    }

    uart_start_tx(uart);
}
#endif

#if MYNEWT_VAL(BLE_MONITOR_RTT)

#if MYNEWT_VAL(BLE_MONITOR_RTT_BUFFERED)
static void
update_drop_counters(struct ble_monitor_hdr *failed_hdr)
{
    uint8_t *cnt;

    rtt_drops.dropped = true;

    switch (failed_hdr->opcode) {
    case BLE_MONITOR_OPCODE_COMMAND_PKT:
        cnt = &rtt_drops.drops_hdr.cmd;
        break;
    case BLE_MONITOR_OPCODE_EVENT_PKT:
        cnt = &rtt_drops.drops_hdr.evt;
        break;
    case BLE_MONITOR_OPCODE_ACL_TX_PKT:
        cnt = &rtt_drops.drops_hdr.acl_tx;
        break;
    case BLE_MONITOR_OPCODE_ACL_RX_PKT:
        cnt = &rtt_drops.drops_hdr.acl_rx;
        break;
    default:
        cnt = &rtt_drops.drops_hdr.other;
        break;
    }

    if (*cnt < UINT8_MAX) {
        (*cnt)++;
        ble_npl_callout_reset(&rtt_drops.tmo, OS_TICKS_PER_SEC);
    }
}

static void
reset_drop_counters(void)
{
    rtt_drops.dropped = false;
    rtt_drops.drops_hdr.cmd = 0;
    rtt_drops.drops_hdr.evt = 0;
    rtt_drops.drops_hdr.acl_tx = 0;
    rtt_drops.drops_hdr.acl_rx = 0;
    rtt_drops.drops_hdr.other = 0;

    ble_npl_callout_stop(&rtt_drops.tmo);
}
#endif

static void
monitor_write(const void *buf, size_t len)
{
#if MYNEWT_VAL(BLE_MONITOR_RTT_BUFFERED)
    struct ble_monitor_hdr *hdr = (struct ble_monitor_hdr *) rtt_pktbuf;
    bool discard;
    unsigned ret = 0;

    /* We will discard any packet which exceeds length of intermediate buffer */
    discard = rtt_pktbuf_pos + len > sizeof(rtt_pktbuf);

    if (!discard) {
        memcpy(rtt_pktbuf + rtt_pktbuf_pos, buf, len);
    }

    rtt_pktbuf_pos += len;
    if (rtt_pktbuf_pos < sizeof(hdr->data_len) + hdr->data_len) {
        return;
    }

    if (!discard) {
        ret = SEGGER_RTT_WriteNoLock(rtt_index, rtt_pktbuf, rtt_pktbuf_pos);
    }

    if (ret > 0) {
        reset_drop_counters();
    } else {
        update_drop_counters(hdr);
    }

    rtt_pktbuf_pos = 0;
#else
    SEGGER_RTT_WriteNoLock(rtt_index, buf, len);
#endif
}
#endif

static void
monitor_write_header(uint16_t opcode, uint16_t len)
{
    struct ble_monitor_hdr hdr;
    struct ble_monitor_ts_hdr ts_hdr;
    uint8_t hdr_len;
    int64_t ts;

    hdr_len = sizeof(ts_hdr);
#if MYNEWT_VAL(BLE_MONITOR_RTT) && MYNEWT_VAL(BLE_MONITOR_RTT_BUFFERED)
    if (rtt_drops.dropped) {
        hdr_len += sizeof(rtt_drops.drops_hdr);
    }
#endif

    hdr.data_len = htole16(4 + hdr_len + len);
    hdr.hdr_len  = hdr_len;
    hdr.opcode   = htole16(opcode);
    hdr.flags    = 0;

    /* Use uptime for timestamp */
    ts = os_get_uptime_usec();

    /*
     * btsnoop specification states that fields of extended header must be
     * sorted in increasing order so we will send drops (if any) headers before
     * timestamp header.
     */

    monitor_write(&hdr, sizeof(hdr));

#if MYNEWT_VAL(BLE_MONITOR_RTT) && MYNEWT_VAL(BLE_MONITOR_RTT_BUFFERED)
    if (rtt_drops.dropped) {
        monitor_write(&rtt_drops.drops_hdr, sizeof(rtt_drops.drops_hdr));
    }
#endif

    ts_hdr.type = BLE_MONITOR_EXTHDR_TS32;
    ts_hdr.ts32 = htole32(ts / 100);

    monitor_write(&ts_hdr, sizeof(ts_hdr));
}

static size_t
btmon_write(FILE *instance, const char *bp, size_t n)
{
    monitor_write(bp, n);

    return n;
}

static FILE *btmon = (FILE *) &(struct File) {
    .vmt = &(struct File_methods) {
        .write = btmon_write,
    },
};

#if MYNEWT_VAL(BLE_MONITOR_RTT) && MYNEWT_VAL(BLE_MONITOR_RTT_BUFFERED)
static void
drops_tmp_cb(struct ble_npl_event *ev)
{
    ble_npl_mutex_pend(&lock, OS_TIMEOUT_NEVER);

    /*
     * There's no "nop" in btsnoop protocol so we just send empty system note
     * to indicate drops.
     */

    monitor_write_header(BLE_MONITOR_OPCODE_SYSTEM_NOTE, 1);
    monitor_write("", 1);

    ble_npl_mutex_release(&lock);
}
#endif

int
ble_monitor_init(void)
{
#if MYNEWT_VAL(BLE_MONITOR_UART)
    struct uart_conf uc = {
        .uc_speed = MYNEWT_VAL(BLE_MONITOR_UART_BAUDRATE),
        .uc_databits = 8,
        .uc_stopbits = 1,
        .uc_parity = UART_PARITY_NONE,
        .uc_flow_ctl = UART_FLOW_CTL_NONE,
        .uc_tx_char = monitor_uart_tx_char,
        .uc_rx_char = monitor_uart_rx_discard,
        .uc_cb_arg = NULL,
    };

    uart = (struct uart_dev *)os_dev_open(MYNEWT_VAL(BLE_MONITOR_UART_DEV),
                                          OS_TIMEOUT_NEVER, &uc);
    if (!uart) {
        return -1;
    }
#endif

#if MYNEWT_VAL(BLE_MONITOR_RTT)
#if MYNEWT_VAL(BLE_MONITOR_RTT_BUFFERED)
    ble_npl_callout_init(&rtt_drops.tmo, ble_hs_evq_get(), drops_tmp_cb, NULL);

    /* Initialize types in header (we won't touch them later) */
    rtt_drops.drops_hdr.type_cmd = BLE_MONITOR_EXTHDR_COMMAND_DROPS;
    rtt_drops.drops_hdr.type_evt = BLE_MONITOR_EXTHDR_EVENT_DROPS;
    rtt_drops.drops_hdr.type_acl_tx = BLE_MONITOR_EXTHDR_ACL_TX_DROPS;
    rtt_drops.drops_hdr.type_acl_rx = BLE_MONITOR_EXTHDR_ACL_RX_DROPS;
    rtt_drops.drops_hdr.type_other = BLE_MONITOR_EXTHDR_OTHER_DROPS;

    rtt_index = SEGGER_RTT_AllocUpBuffer(MYNEWT_VAL(BLE_MONITOR_RTT_BUFFER_NAME),
                                         rtt_buf, sizeof(rtt_buf),
                                         SEGGER_RTT_MODE_NO_BLOCK_SKIP);
#else
    rtt_index = SEGGER_RTT_AllocUpBuffer(MYNEWT_VAL(BLE_MONITOR_RTT_BUFFER_NAME),
                                         rtt_buf, sizeof(rtt_buf),
                                         SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
#endif

    if (rtt_index < 0) {
        return -1;
    }
#endif

    ble_npl_mutex_init(&lock);

    return 0;
}

int
ble_monitor_send(uint16_t opcode, const void *data, size_t len)
{
    ble_npl_mutex_pend(&lock, OS_TIMEOUT_NEVER);

    monitor_write_header(opcode, len);
    monitor_write(data, len);

    ble_npl_mutex_release(&lock);

    return 0;
}

int
ble_monitor_send_om(uint16_t opcode, const struct os_mbuf *om)
{
    const struct os_mbuf *om_tmp;
    uint16_t length = 0;

    om_tmp = om;
    while (om_tmp) {
        length += om_tmp->om_len;
        om_tmp = SLIST_NEXT(om_tmp, om_next);
    }

    ble_npl_mutex_pend(&lock, OS_TIMEOUT_NEVER);

    monitor_write_header(opcode, length);

    while (om) {
        monitor_write(om->om_data, om->om_len);
        om = SLIST_NEXT(om, om_next);
    }

    ble_npl_mutex_release(&lock);

    return 0;
}

int
ble_monitor_new_index(uint8_t bus, uint8_t *addr, const char *name)
{
    struct ble_monitor_new_index pkt;

    pkt.type = 0; /* Primary controller, we don't support other */
    pkt.bus = bus;
    memcpy(pkt.bdaddr, addr, 6);
    strncpy(pkt.name, name, sizeof(pkt.name) - 1);
    pkt.name[sizeof(pkt.name) - 1] = '\0';

    ble_monitor_send(BLE_MONITOR_OPCODE_NEW_INDEX, &pkt, sizeof(pkt));

    return 0;
}

int
ble_monitor_log(int level, const char *fmt, ...)
{
    static const char id[] = "nimble";
    struct ble_monitor_user_logging ulog;
    va_list va;
    int len;

    va_start(va, fmt);
    len = vsnprintf(NULL, 0, fmt, va);
    va_end(va);

    switch (level) {
    case LOG_LEVEL_ERROR:
        ulog.priority = 3;
        break;
    case LOG_LEVEL_WARN:
        ulog.priority = 4;
        break;
    case LOG_LEVEL_INFO:
        ulog.priority = 6;
        break;
    case LOG_LEVEL_DEBUG:
        ulog.priority = 7;
        break;
    default:
        ulog.priority = 8;
        break;
    }

    ulog.ident_len = sizeof(id);

    ble_npl_mutex_pend(&lock, OS_TIMEOUT_NEVER);

    monitor_write_header(BLE_MONITOR_OPCODE_USER_LOGGING,
                         sizeof(ulog) + sizeof(id) + len + 1);
    monitor_write(&ulog, sizeof(ulog));
    monitor_write(id, sizeof(id));

    va_start(va, fmt);
    vfprintf(btmon, fmt, va);
    va_end(va);

    /* null-terminate string */
    monitor_write("", 1);

    ble_npl_mutex_release(&lock);

    return 0;
}

int
ble_monitor_out(int c)
{
    static char buf[MYNEWT_VAL(BLE_MONITOR_CONSOLE_BUFFER_SIZE)];
    static size_t len;

    if (c != '\n') {
        buf[len++] = c;

        if (len < sizeof(buf) - 1) {
            return c;
        }
    }

    buf[len++] = '\0';

    ble_monitor_send(BLE_MONITOR_OPCODE_SYSTEM_NOTE, buf, len);
    len = 0;

    return c;
}

#endif
