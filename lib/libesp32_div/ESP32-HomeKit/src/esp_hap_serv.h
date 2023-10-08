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

#ifndef _HAP_SERV_H_
#define _HAP_SERV_H_

#include <hap.h>
#include <esp_hap_char.h>
#include <esp_hap_acc.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hap_linked_serv {
    hap_serv_t *hs;
    struct hap_linked_serv *next;
} hap_linked_serv_t;

typedef struct hap_linked_serv hap_linked_serv_t;

/**
 * HAP service information
 */
typedef struct {
    const char           *type_uuid;      /* String that defines the type of the service. */

    uint32_t             iid;        /* service instance ID */

    bool                hidden;     /* If set it to be True, the service is not visible to user. */
    bool                primary;    /* If set it to be True, this is the primary service of the accessory. */

    /**
     * List of Characteristic objects. Must not be empty. The maximum number of characteristics
     * allowed is 100, and each characteristic in the array must have a unique type.
     */
    hap_char_t *chars;
    hap_acc_t  *parent;
    hap_serv_t *next_serv;
    
    hap_serv_write_t write_cb;
    hap_serv_read_t read_cb;
    hap_serv_bulk_read_t bulk_read;
    hap_linked_serv_t *linked_servs;
    void *priv;
} __hap_serv_t;

bool hap_serv_get_hidden(hap_serv_t *hs);
bool hap_serv_get_primary(hap_serv_t *hs);
hap_char_t *hap_serv_get_char_by_iid(hap_serv_t *hs, int32_t iid);
const char *hap_serv_get_uuid(hap_serv_t *hs);
hap_serv_t *hap_serv_create(const char *type_uuid);
void hap_serv_delete(hap_serv_t *hs);
int hap_serv_add_char(hap_serv_t *hs, hap_char_t *hc);
#ifdef __cplusplus
}
#endif

#endif /* _HAP_SERV_H_ */
