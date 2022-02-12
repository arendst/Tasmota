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

#include <assert.h>
#include <stdlib.h>
#include "nimble/nimble/include/nimble/ble.h"
#include "../include/controller/ble_ll.h"
#include "../include/controller/ble_ll_utils.h"

/* 37 bits require 5 bytes */
#define BLE_LL_CHMAP_LEN (5)

/* Sleep clock accuracy table (in ppm) */
static const uint16_t g_ble_sca_ppm_tbl[8] = {
    500, 250, 150, 100, 75, 50, 30, 20
};

uint32_t
ble_ll_utils_calc_access_addr(void)
{
    uint32_t aa;
    uint16_t aa_low;
    uint16_t aa_high;
    uint32_t temp;
    uint32_t mask;
    uint32_t prev_bit;
    uint8_t bits_diff;
    uint8_t consecutive;
    uint8_t transitions;
    uint8_t ones;
    int tmp;

    /* Calculate a random access address */
    aa = 0;
    while (1) {
        /* Get two, 16-bit random numbers */
        aa_low = rand() & 0xFFFF;
        aa_high = rand() & 0xFFFF;

        /* All four bytes cannot be equal */
        if (aa_low == aa_high) {
            continue;
        }

        /* Upper 6 bits must have 2 transitions */
        tmp = (int16_t)aa_high >> 10;
        if (__builtin_popcount(tmp ^ (tmp >> 1)) < 2) {
            continue;
        }

        /* Cannot be access address or be 1 bit different */
        aa = aa_high;
        aa = (aa << 16) | aa_low;
        bits_diff = 0;
        temp = aa ^ BLE_ACCESS_ADDR_ADV;
        for (mask = 0x00000001; mask != 0; mask <<= 1) {
            if (mask & temp) {
                ++bits_diff;
                if (bits_diff > 1) {
                    break;
                }
            }
        }
        if (bits_diff <= 1) {
            continue;
        }

        /* Cannot have more than 24 transitions */
        transitions = 0;
        consecutive = 1;
        ones = 0;
        mask = 0x00000001;
        while (mask < 0x80000000) {
            prev_bit = aa & mask;
            mask <<= 1;
            if (mask & aa) {
                if (prev_bit == 0) {
                    ++transitions;
                    consecutive = 1;
                } else {
                    ++consecutive;
                }
            } else {
                if (prev_bit == 0) {
                    ++consecutive;
                } else {
                    ++transitions;
                    consecutive = 1;
                }
            }

            if (prev_bit) {
                ones++;
            }

            /* 8 lsb should have at least three 1 */
            if (mask == 0x00000100 && ones < 3) {
                break;
            }

            /* 16 lsb should have no more than 11 transitions */
            if (mask == 0x00010000 && transitions > 11) {
                break;
            }

            /* This is invalid! */
            if (consecutive > 6) {
                /* Make sure we always detect invalid sequence below */
                mask = 0;
                break;
            }
        }

        /* Invalid sequence found */
        if (mask != 0x80000000) {
            continue;
        }

        /* Cannot be more than 24 transitions */
        if (transitions > 24) {
            continue;
        }

        /* We have a valid access address */
        break;
    }
    return aa;
}

uint8_t
ble_ll_utils_remapped_channel(uint8_t remap_index, const uint8_t *chanmap)
{
    uint8_t cntr;
    uint8_t mask;
    uint8_t usable_chans;
    uint8_t chan;
    int i, j;

    /* NOTE: possible to build a map but this would use memory. For now,
     * we just calculate
     * Iterate through channel map to find this channel
     */
    chan = 0;
    cntr = 0;
    for (i = 0; i < BLE_LL_CHMAP_LEN; i++) {
        usable_chans = chanmap[i];
        if (usable_chans != 0) {
            mask = 0x01;
            for (j = 0; j < 8; j++) {
                if (usable_chans & mask) {
                    if (cntr == remap_index) {
                        return (chan + j);
                    }
                    ++cntr;
                }
                mask <<= 1;
            }
        }
        chan += 8;
    }

    /* we should never reach here */
    BLE_LL_ASSERT(0);
    return 0;
}

uint8_t
ble_ll_utils_calc_num_used_chans(const uint8_t *chmap)
{
    int i;
    int j;
    uint8_t mask;
    uint8_t chanbyte;
    uint8_t used_channels;

    used_channels = 0;
    for (i = 0; i < BLE_LL_CHMAP_LEN; ++i) {
        chanbyte = chmap[i];
        if (chanbyte) {
            if (chanbyte == 0xff) {
                used_channels += 8;
            } else {
                mask = 0x01;
                for (j = 0; j < 8; ++j) {
                    if (chanbyte & mask) {
                        ++used_channels;
                    }
                    mask <<= 1;
                }
            }
        }
    }
    return used_channels;
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CSA2)
static uint16_t
ble_ll_utils_csa2_perm(uint16_t in)
{
    uint16_t out = 0;
    int i;

    for (i = 0; i < 8; i++) {
        out |= ((in >> i) & 0x00000001) << (7 - i);
    }

    for (i = 8; i < 16; i++) {
        out |= ((in >> i) & 0x00000001) << (15 + 8 - i);
    }

    return out;
}

static uint16_t
ble_ll_utils_csa2_prng(uint16_t counter, uint16_t ch_id)
{
    uint16_t prn_e;

    prn_e = counter ^ ch_id;

    prn_e = ble_ll_utils_csa2_perm(prn_e);
    prn_e = (prn_e * 17) + ch_id;

    prn_e = ble_ll_utils_csa2_perm(prn_e);
    prn_e = (prn_e * 17) + ch_id;

    prn_e = ble_ll_utils_csa2_perm(prn_e);
    prn_e = (prn_e * 17) + ch_id;

    prn_e = prn_e ^ ch_id;

    return prn_e;
}

uint8_t
ble_ll_utils_calc_dci_csa2(uint16_t event_cntr, uint16_t channel_id,
                           uint8_t num_used_chans, const uint8_t *chanmap)
{
    uint16_t channel_unmapped;
    uint8_t remap_index;

    uint16_t prn_e;
    uint8_t bitpos;

    prn_e = ble_ll_utils_csa2_prng(event_cntr, channel_id);

    channel_unmapped = prn_e % 37;

    /*
     * If unmapped channel is the channel index of a used channel it is used
     * as channel index.
     */
    bitpos = 1 << (channel_unmapped & 0x07);
    if (chanmap[channel_unmapped >> 3] & bitpos) {
        return channel_unmapped;
    }

    remap_index = (num_used_chans * prn_e) / 0x10000;

    return ble_ll_utils_remapped_channel(remap_index, chanmap);
}
#endif

uint32_t
ble_ll_utils_calc_window_widening(uint32_t anchor_point,
                                  uint32_t last_anchor_point,
                                  uint8_t master_sca)
{
    uint32_t total_sca_ppm;
    uint32_t window_widening;
    int32_t time_since_last_anchor;
    uint32_t delta_msec;

    window_widening = 0;

    time_since_last_anchor = (int32_t)(anchor_point - last_anchor_point);
    if (time_since_last_anchor > 0) {
        delta_msec = os_cputime_ticks_to_usecs(time_since_last_anchor) / 1000;
        total_sca_ppm = g_ble_sca_ppm_tbl[master_sca] +
                                          MYNEWT_VAL(BLE_LL_OUR_SCA);
        window_widening = (total_sca_ppm * delta_msec) / 1000;
    }

    return window_widening;
}
#endif
