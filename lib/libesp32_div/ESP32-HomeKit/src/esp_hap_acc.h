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

#ifndef _HAP_ACC_H_
#define _HAP_ACC_H_

#include <hap.h>
#include <esp_hap_serv.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef struct esp_mfi_accessory {
    struct esp_mfi_accessory *next;
    uint32_t             aid;            /* accessory AID */
    hap_serv_t      *servs;   /* service list */
    bool                power_off;
    uint32_t next_iid;
    hap_identify_routine_t identify_routine;
} __hap_acc_t;
hap_char_t *hap_acc_get_char_by_iid(hap_acc_t *ha, int32_t iid);
hap_acc_t *hap_acc_get_by_aid(int32_t aid);
int hap_acc_get_info(hap_acc_cfg_t *acc_cfg);
const hap_val_t *hap_get_product_data();
#ifdef __cplusplus
}
#endif

#endif /* _HAP_ACC_H_ */
