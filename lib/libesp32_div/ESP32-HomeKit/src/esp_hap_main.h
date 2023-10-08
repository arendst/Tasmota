/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2020 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS products only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef _HAP_MAIN_LOOP_H_
#define _HAP_MAIN_LOOP_H_
#include <stdbool.h>
#include <hap.h>

#define HAP_FF_HARDWARE_AUTH    0x01
#define HAP_FF_SW_TOKEN_AUTH    0x02


#define HAP_SF_ACC_UNPAIRED         0x01
#define HAP_SF_ACC_UNCONFIGURED     0x02
#define HAP_SF_PROBLEM_DETECTED     0x04

typedef enum {
    HAP_INTERNAL_EVENT_LOOP_STOP = 1,
    HAP_INTERNAL_EVENT_ACC_PAIRED,
    HAP_INTERNAL_EVENT_ACC_UNPAIRED,
    HAP_INTERNAL_EVENT_CONFIG_NUM_UPDATED,
    HAP_INTERNAL_EVENT_BCT_CHANGE_NAME,
    HAP_INTERNAL_EVENT_BCT_HOT_PLUG,
    HAP_INTERNAL_EVENT_RESET_PAIRINGS,
    HAP_INTERNAL_EVENT_RESET_TO_FACTORY,
    HAP_INTERNAL_EVENT_REBOOT,
    HAP_INTERNAL_EVENT_RESET_NETWORK,
    HAP_INTERNAL_EVENT_TRIGGER_NOTIF,
    HAP_INTERNAL_EVENT_RESET_HOMEKIT_DATA,
} hap_internal_event_t;

typedef struct {
    hap_internal_event_t event;
} hap_event_ctx_t;

typedef enum {
    HAP_STATE_NONE = 0,
    HAP_STATE_NW_UNCONFIGURED,
    HAP_STATE_NW_CONFIGURED,
} hap_state_t;

int hap_loop_start();
int hap_loop_stop();
int hap_send_event(hap_internal_event_t event);
int hap_update_config_number();
bool is_hap_loop_started();
void hap_report_event(hap_event_t event, void *data, size_t data_size);
int hap_enable_hw_auth(void);
int hap_enable_sw_auth(void);

#endif /* _HAP_MAIN_LOOP_H_ */
