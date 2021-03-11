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

#ifndef _HAP_CHAR_H_
#define _HAP_CHAR_H_

#include <stdint.h>
#include <stdbool.h>
#include <sys/errno.h>

#include <hap.h>
#include <esp_hap_serv.h>

#ifdef __cplusplus
extern "C" {
#endif
#define HAP_CHAR_MIN_FLAG		(1 << 0)
#define HAP_CHAR_MAX_FLAG		(1 << 1)
#define HAP_CHAR_STEP_FLAG		(1 << 2)
#define HAP_CHAR_MAXLEN_FLAG		(1 << 3)
#define HAP_CHAR_MAXDATALEN_FLAG	(1 << 4)

/**
 * @brief characteristics object information
 */
typedef struct  {
    uint32_t iid;        /* Characteristic instance ID */
    const char *type_uuid;       /* Apple's characteristic UUID */
    uint16_t permission; /* Characteristic permission */
    hap_char_format_t      format;   /* data type of the value */
    hap_val_t       val;
    bool ev;         /* check if characteristics supports event */
    char *description;      /* characteristics's description */
    char *unit;

    /* Characteristics's father subsystem */
    hap_serv_t                *parent;

    uint8_t constraint_flags;

    hap_val_t       max;       /* maximum value, maxlen, max data len*/
    hap_val_t       min;       /* minimum value */
    hap_val_t       step;      /* step value */

    hap_char_t *next_char;
    /* Bitmap to indicate which controllers have enabled notifications
     */
	uint16_t ev_ctrls;

    /* Bitmap indicating the last controller that modified the value.
     * No notification should be sent to the owner
     */
    uint16_t owner_ctrl;

    /* Pointer to a valid values range. It will be a 2 byte array, if set from application */
    uint8_t *valid_vals_range;
    /* Since a list of valid values can have any length, using a pointer here,
     * which will be allocated if valid values are set for a characteristic
     */
    uint8_t *valid_vals;
    size_t valid_vals_cnt;
    bool update_called;
} __hap_char_t;

void hap_char_manage_notification(hap_char_t *hc, int index, bool ev);
bool hap_char_is_ctrl_subscribed(hap_char_t *hc, int index);
void hap_char_set_owner_ctrl(hap_char_t *hc, int index);
bool hap_char_is_ctrl_owner(hap_char_t *hc, int index);
void hap_disable_all_char_notif(int index);
int hap_char_check_val_constraints(__hap_char_t *_hc, hap_val_t *val);
int hap_event_queue_init();
hap_char_t * hap_get_pending_notif_char();
#ifdef __cplusplus
}
#endif

#endif /* _HAP_CHAR_H_ */
