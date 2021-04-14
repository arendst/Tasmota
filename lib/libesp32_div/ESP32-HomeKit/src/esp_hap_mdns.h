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
#ifndef _HAP_MDNS_H_
#define _HAP_MDNS_H_

#include <mdns.h>
#include <hap.h>

typedef struct {
    char type[32];
    char proto[32];
} hap_mdns_handle_t;

int hap_mdns_serv_start(hap_mdns_handle_t *handle, const char *name, const char *type,
        const char *protocol, int port, mdns_txt_item_t *txt_records, size_t num_txt);
int hap_mdns_serv_update_txt(hap_mdns_handle_t *handle, mdns_txt_item_t *txt_records, size_t num_txt);
int hap_mdns_serv_name_change(hap_mdns_handle_t *handle, const char * instance_name);
int hap_mdns_serv_stop(hap_mdns_handle_t *handle);
int hap_mdns_init();
int hap_mdns_deinit();

#endif /* _HAP_MDNS_H_ */
