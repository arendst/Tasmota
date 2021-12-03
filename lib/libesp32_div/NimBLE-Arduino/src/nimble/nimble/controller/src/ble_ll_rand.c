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
#ifndef ESP_PLATFORM

#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#include "nimble/porting/nimble/include/os/os.h"
#include "nimble/nimble/include/nimble/ble.h"
#include "nimble/nimble/include/nimble/nimble_opt.h"
#include "../include/controller/ble_hw.h"
#include "../include/controller/ble_ll.h"
#if MYNEWT_VAL(TRNG)
#include "trng/trng.h"
#endif

#if MYNEWT_VAL(TRNG)
static struct trng_dev *g_trng;
#else
/* This is a simple circular buffer for holding N samples of random data */
struct ble_ll_rnum_data
{
    uint8_t *rnd_in;
    uint8_t *rnd_out;
    volatile uint8_t rnd_size;
};

struct ble_ll_rnum_data g_ble_ll_rnum_data;
uint8_t g_ble_ll_rnum_buf[MYNEWT_VAL(BLE_LL_RNG_BUFSIZE)];

#define IS_RNUM_BUF_END(x)  \
    (x == &g_ble_ll_rnum_buf[MYNEWT_VAL(BLE_LL_RNG_BUFSIZE) - 1])

void
ble_ll_rand_sample(uint8_t rnum)
{
    os_sr_t sr;

    OS_ENTER_CRITICAL(sr);
    if (g_ble_ll_rnum_data.rnd_size < MYNEWT_VAL(BLE_LL_RNG_BUFSIZE)) {
        ++g_ble_ll_rnum_data.rnd_size;
        g_ble_ll_rnum_data.rnd_in[0] = rnum;
        if (IS_RNUM_BUF_END(g_ble_ll_rnum_data.rnd_in)) {
            g_ble_ll_rnum_data.rnd_in = g_ble_ll_rnum_buf;
        } else {
            ++g_ble_ll_rnum_data.rnd_in;
        }
    } else {
        /* Stop generating random numbers as we are full */
        ble_hw_rng_stop();
    }
    OS_EXIT_CRITICAL(sr);
}
#endif

/* Get 'len' bytes of random data */
int
ble_ll_rand_data_get(uint8_t *buf, uint8_t len)
{
#if MYNEWT_VAL(TRNG)
    size_t num;

    while (len) {
        num = trng_read(g_trng, buf, len);
        buf += num;
        len -= num;
    }
#else
    uint8_t rnums;
    os_sr_t sr;

    while (len != 0) {
        OS_ENTER_CRITICAL(sr);
        rnums = g_ble_ll_rnum_data.rnd_size;
        if (rnums > len) {
            rnums = len;
        }
        len -= rnums;
        g_ble_ll_rnum_data.rnd_size -= rnums;
        while (rnums) {
            buf[0] = g_ble_ll_rnum_data.rnd_out[0];
            if (IS_RNUM_BUF_END(g_ble_ll_rnum_data.rnd_out)) {
                g_ble_ll_rnum_data.rnd_out = g_ble_ll_rnum_buf;
            } else {
                ++g_ble_ll_rnum_data.rnd_out;
            }
            ++buf;
            --rnums;
        }
        OS_EXIT_CRITICAL(sr);

        /* Make sure rng is started! */
        ble_hw_rng_start();

        /* Wait till bytes are in buffer. */
        if (len) {
            while ((g_ble_ll_rnum_data.rnd_size < len) &&
                   (g_ble_ll_rnum_data.rnd_size < MYNEWT_VAL(BLE_LL_RNG_BUFSIZE))) {
                /* Spin here */
            }
        }
    }
#endif
    return BLE_ERR_SUCCESS;
}

/**
 * Called to obtain a "prand" as defined in core V4.2 Vol 6 Part B 1.3.2.2
 *
 * @param prand
 */
void
ble_ll_rand_prand_get(uint8_t *prand)
{
    uint16_t sum;

    while (1) {
        /* Get 24 bits of random data */
        ble_ll_rand_data_get(prand, 3);

        /* Prand cannot be all zeros or 1's. */
        sum = prand[0] + prand[1] + prand[2];
        if ((sum != 0) && (sum != (3 * 0xff))) {
            break;
        }
    }

    /* Upper two bits must be 01 */
    prand[2] &= ~0xc0;
    prand[2] |= 0x40;
}

/**
 * Start the generation of random numbers
 *
 * @return int
 */
int
ble_ll_rand_start(void)
{
#if MYNEWT_VAL(TRNG)
    /* Nothing to do - this is handled by driver */
#else
    /* Start the generation of numbers if we are not full */
    if (g_ble_ll_rnum_data.rnd_size < MYNEWT_VAL(BLE_LL_RNG_BUFSIZE)) {
        ble_hw_rng_start();
    }
#endif
    return 0;
}

/**
 * Initialize LL random number generation. Should be called only once on
 * initialization.
 *
 * @return int
 */
int
ble_ll_rand_init(void)
{
#if MYNEWT_VAL(TRNG)
    g_trng = (struct trng_dev *) os_dev_open("trng", OS_TIMEOUT_NEVER, NULL);
#else
    g_ble_ll_rnum_data.rnd_in = g_ble_ll_rnum_buf;
    g_ble_ll_rnum_data.rnd_out = g_ble_ll_rnum_buf;
    ble_hw_rng_init(ble_ll_rand_sample, 1);
#endif
    return 0;
}

#endif
