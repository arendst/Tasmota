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

#if defined(ARDUINO_ARCH_NRF5) && defined(NRF52_SERIES)

#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#include "nimble/porting/nimble/include/os/os.h"
#include "../include/ble/xcvr.h"
#include "nimble/nimble/include/nimble/ble.h"
#include "nimble/nimble/include/nimble/nimble_opt.h"
#include "nrf.h"
#include "nimble/nimble/controller/include/controller/ble_hw.h"
#if MYNEWT
#include "mcu/cmsis_nvic.h"
#else
#include "core_cm4.h"
#include "nimble/porting/npl/freertos/include/nimble/nimble_npl_os.h"
#endif
#include "nimble/porting/nimble/include/os/os_trace_api.h"

/* Total number of resolving list elements */
#define BLE_HW_RESOLV_LIST_SIZE     (16)

/* We use this to keep track of which entries are set to valid addresses */
static uint8_t g_ble_hw_whitelist_mask;

/* Random number generator isr callback */
ble_rng_isr_cb_t g_ble_rng_isr_cb;

/* If LL privacy is enabled, allocate memory for AAR */
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)

/* The NRF51 supports up to 16 IRK entries */
#if (MYNEWT_VAL(BLE_LL_RESOLV_LIST_SIZE) < 16)
#define NRF_IRK_LIST_ENTRIES    (MYNEWT_VAL(BLE_LL_RESOLV_LIST_SIZE))
#else
#define NRF_IRK_LIST_ENTRIES    (16)
#endif

/* NOTE: each entry is 16 bytes long. */
uint32_t g_nrf_irk_list[NRF_IRK_LIST_ENTRIES * 4];

/* Current number of IRK entries */
uint8_t g_nrf_num_irks;

#endif

/* Returns public device address or -1 if not present */
int
ble_hw_get_public_addr(ble_addr_t *addr)
{
    uint32_t addr_high;
    uint32_t addr_low;

    /* Does FICR have a public address */
    if ((NRF_FICR->DEVICEADDRTYPE & 1) != 0) {
        return -1;
    }

    /* Copy into device address. We can do this because we know platform */
    addr_low = NRF_FICR->DEVICEADDR[0];
    addr_high = NRF_FICR->DEVICEADDR[1];
    memcpy(addr->val, &addr_low, 4);
    memcpy(&addr->val[4], &addr_high, 2);
    addr->type = BLE_ADDR_PUBLIC;

    return 0;
}

/* Returns random static address or -1 if not present */
int
ble_hw_get_static_addr(ble_addr_t *addr)
{
    int rc;

    if ((NRF_FICR->DEVICEADDRTYPE & 1) == 1) {
        memcpy(addr->val, (void *)&NRF_FICR->DEVICEADDR[0], 4);
        memcpy(&addr->val[4], (void *)&NRF_FICR->DEVICEADDR[1], 2);
        addr->val[5] |= 0xc0;
        addr->type = BLE_ADDR_RANDOM;
        rc = 0;
    } else {
        rc = -1;
    }

    return rc;
}

/**
 * Clear the whitelist
 *
 * @return int
 */
void
ble_hw_whitelist_clear(void)
{
    NRF_RADIO->DACNF = 0;
    g_ble_hw_whitelist_mask = 0;
}

/**
 * Add a device to the hw whitelist
 *
 * @param addr
 * @param addr_type
 *
 * @return int 0: success, BLE error code otherwise
 */
int
ble_hw_whitelist_add(const uint8_t *addr, uint8_t addr_type)
{
    int i;
    uint32_t mask;

    /* Find first ununsed device address match element */
    mask = 0x01;
    for (i = 0; i < BLE_HW_WHITE_LIST_SIZE; ++i) {
        if ((mask & g_ble_hw_whitelist_mask) == 0) {
            NRF_RADIO->DAB[i] = get_le32(addr);
            NRF_RADIO->DAP[i] = get_le16(addr + 4);
            if (addr_type == BLE_ADDR_RANDOM) {
                NRF_RADIO->DACNF |= (mask << 8);
            }
            g_ble_hw_whitelist_mask |= mask;
            return BLE_ERR_SUCCESS;
        }
        mask <<= 1;
    }

    return BLE_ERR_MEM_CAPACITY;
}

/**
 * Remove a device from the hw whitelist
 *
 * @param addr
 * @param addr_type
 *
 */
void
ble_hw_whitelist_rmv(const uint8_t *addr, uint8_t addr_type)
{
    int i;
    uint8_t cfg_addr;
    uint16_t dap;
    uint16_t txadd;
    uint32_t dab;
    uint32_t mask;

    /* Find first ununsed device address match element */
    dab = get_le32(addr);
    dap = get_le16(addr + 4);
    txadd = NRF_RADIO->DACNF >> 8;
    mask = 0x01;
    for (i = 0; i < BLE_HW_WHITE_LIST_SIZE; ++i) {
        if (mask & g_ble_hw_whitelist_mask) {
            if ((dab == NRF_RADIO->DAB[i]) && (dap == NRF_RADIO->DAP[i])) {
                cfg_addr = txadd & mask;
                if (addr_type == BLE_ADDR_RANDOM) {
                    if (cfg_addr != 0) {
                        break;
                    }
                } else {
                    if (cfg_addr == 0) {
                        break;
                    }
                }
            }
        }
        mask <<= 1;
    }

    if (i < BLE_HW_WHITE_LIST_SIZE) {
        g_ble_hw_whitelist_mask &= ~mask;
        NRF_RADIO->DACNF &= ~mask;
    }
}

/**
 * Returns the size of the whitelist in HW
 *
 * @return int Number of devices allowed in whitelist
 */
uint8_t
ble_hw_whitelist_size(void)
{
    return BLE_HW_WHITE_LIST_SIZE;
}

/**
 * Enable the whitelisted devices
 */
void
ble_hw_whitelist_enable(void)
{
    /* Enable the configured device addresses */
    NRF_RADIO->DACNF |= g_ble_hw_whitelist_mask;
}

/**
 * Disables the whitelisted devices
 */
void
ble_hw_whitelist_disable(void)
{
    /* Disable all whitelist devices */
    NRF_RADIO->DACNF &= 0x0000ff00;
}

/**
 * Boolean function which returns true ('1') if there is a match on the
 * whitelist.
 *
 * @return int
 */
int
ble_hw_whitelist_match(void)
{
    return (int)NRF_RADIO->EVENTS_DEVMATCH;
}

/* Encrypt data */
int
ble_hw_encrypt_block(struct ble_encryption_block *ecb)
{
    int rc;
    uint32_t end;
    uint32_t err;

    /* Stop ECB */
    NRF_ECB->TASKS_STOPECB = 1;
    /* XXX: does task stop clear these counters? Anyway to do this quicker? */
    NRF_ECB->EVENTS_ENDECB = 0;
    NRF_ECB->EVENTS_ERRORECB = 0;
    NRF_ECB->ECBDATAPTR = (uint32_t)ecb;

    /* Start ECB */
    NRF_ECB->TASKS_STARTECB = 1;

    /* Wait till error or done */
    rc = 0;
    while (1) {
        end = NRF_ECB->EVENTS_ENDECB;
        err = NRF_ECB->EVENTS_ERRORECB;
        if (end || err) {
            if (err) {
                rc = -1;
            }
            break;
        }
    }

    return rc;
}

/**
 * Random number generator ISR.
 */
static void
ble_rng_isr(void)
{
    uint8_t rnum;

    os_trace_isr_enter();

    /* No callback? Clear and disable interrupts */
    if (g_ble_rng_isr_cb == NULL) {
        NRF_RNG->INTENCLR = 1;
        NRF_RNG->EVENTS_VALRDY = 0;
        (void)NRF_RNG->SHORTS;
        os_trace_isr_exit();
        return;
    }

    /* If there is a value ready grab it */
    if (NRF_RNG->EVENTS_VALRDY) {
        NRF_RNG->EVENTS_VALRDY = 0;
        rnum = (uint8_t)NRF_RNG->VALUE;
        (*g_ble_rng_isr_cb)(rnum);
    }

    os_trace_isr_exit();
}

/**
 * Initialize the random number generator
 *
 * @param cb
 * @param bias
 *
 * @return int
 */
int
ble_hw_rng_init(ble_rng_isr_cb_t cb, int bias)
{
    /* Set bias */
    if (bias) {
        NRF_RNG->CONFIG = 1;
    } else {
        NRF_RNG->CONFIG = 0;
    }

    /* If we were passed a function pointer we need to enable the interrupt */
    if (cb != NULL) {
#ifndef RIOT_VERSION
        NVIC_SetPriority(RNG_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
#endif
#if MYNEWT
        NVIC_SetVector(RNG_IRQn, (uint32_t)ble_rng_isr);
#else
        ble_npl_hw_set_isr(RNG_IRQn, ble_rng_isr);
#endif
        NVIC_EnableIRQ(RNG_IRQn);
        g_ble_rng_isr_cb = cb;
    }

    return 0;
}

/**
 * Start the random number generator
 *
 * @return int
 */
int
ble_hw_rng_start(void)
{
    os_sr_t sr;

    /* No need for interrupt if there is no callback */
    OS_ENTER_CRITICAL(sr);
    NRF_RNG->EVENTS_VALRDY = 0;
    if (g_ble_rng_isr_cb) {
        NRF_RNG->INTENSET = 1;
    }
    NRF_RNG->TASKS_START = 1;
    OS_EXIT_CRITICAL(sr);

    return 0;
}

/**
 * Stop the random generator
 *
 * @return int
 */
int
ble_hw_rng_stop(void)
{
    os_sr_t sr;

    /* No need for interrupt if there is no callback */
    OS_ENTER_CRITICAL(sr);
    NRF_RNG->INTENCLR = 1;
    NRF_RNG->TASKS_STOP = 1;
    NRF_RNG->EVENTS_VALRDY = 0;
    OS_EXIT_CRITICAL(sr);

    return 0;
}

/**
 * Read the random number generator.
 *
 * @return uint8_t
 */
uint8_t
ble_hw_rng_read(void)
{
    uint8_t rnum;

    /* Wait for a sample */
    while (NRF_RNG->EVENTS_VALRDY == 0) {
    }

    NRF_RNG->EVENTS_VALRDY = 0;
    rnum = (uint8_t)NRF_RNG->VALUE;

    return rnum;
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
/**
 * Clear the resolving list
 *
 * @return int
 */
void
ble_hw_resolv_list_clear(void)
{
    g_nrf_num_irks = 0;
}

/**
 * Add a device to the hw resolving list
 *
 * @param irk   Pointer to IRK to add
 *
 * @return int 0: success, BLE error code otherwise
 */
int
ble_hw_resolv_list_add(uint8_t *irk)
{
    uint32_t *nrf_entry;

    /* Find first ununsed device address match element */
    if (g_nrf_num_irks == NRF_IRK_LIST_ENTRIES) {
        return BLE_ERR_MEM_CAPACITY;
    }

    /* Copy into irk list */
    nrf_entry = &g_nrf_irk_list[4 * g_nrf_num_irks];
    memcpy(nrf_entry, irk, 16);

    /* Add to total */
    ++g_nrf_num_irks;
    return BLE_ERR_SUCCESS;
}

/**
 * Remove a device from the hw resolving list
 *
 * @param index Index of IRK to remove
 */
void
ble_hw_resolv_list_rmv(int index)
{
    uint32_t *irk_entry;

    if (index < g_nrf_num_irks) {
        --g_nrf_num_irks;
        irk_entry = &g_nrf_irk_list[index];
        if (g_nrf_num_irks > index) {
            memmove(irk_entry, irk_entry + 4, 16 * (g_nrf_num_irks - index));
        }
    }
}

/**
 * Returns the size of the resolving list. NOTE: this returns the maximum
 * allowable entries in the HW. Configuration options may limit this.
 *
 * @return int Number of devices allowed in resolving list
 */
uint8_t
ble_hw_resolv_list_size(void)
{
    return BLE_HW_RESOLV_LIST_SIZE;
}

/**
 * Called to determine if the address received was resolved.
 *
 * @return int  Negative values indicate unresolved address; positive values
 *              indicate index in resolving list of resolved address.
 */
int
ble_hw_resolv_list_match(void)
{
    uint32_t index;

    if (NRF_AAR->EVENTS_END) {
        if (NRF_AAR->EVENTS_RESOLVED) {
            index = NRF_AAR->STATUS;
            return (int)index;
        }
    }

    return -1;
}
#endif
#endif
