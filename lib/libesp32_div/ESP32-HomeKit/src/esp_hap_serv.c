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
#include <string.h>
#include <hap_platform_memory.h>

#include <esp_hap_serv.h>
#include <esp_mfi_debug.h>

void hap_serv_mark_primary(hap_serv_t *hs)
{
    if (hs) {
        ((__hap_serv_t *)hs)->primary = true;
    }
}

void hap_serv_mark_hidden(hap_serv_t *hs)
{
    if (hs) {
        ((__hap_serv_t *)hs)->hidden = true;
    }
}

/**
 * @brief get service hidden attribute
 */
bool hap_serv_get_hidden(hap_serv_t *hs)
{
    if (hs) {
        return ((__hap_serv_t *)hs)->hidden;
    }
    return false;
}


void hap_serv_set_iid(hap_serv_t *hs, int32_t iid)
{
    if (hs) {
        ((__hap_serv_t *)hs)->iid = iid;
    }
}

/**
 * @brief get service "primary" attribute
 */
bool hap_serv_get_primary(hap_serv_t *hs)
{
    if (hs) {
        return ((__hap_serv_t *)hs)->primary;
    }
    return false;
}

hap_char_t *hap_serv_get_first_char(hap_serv_t *hs)
{
    return ((__hap_serv_t *)hs)->chars;
}
/**
 * @brief get target characteristics by it's IID
 */
hap_char_t *hap_serv_get_char_by_iid(hap_serv_t *hs, int32_t iid)
{
    if (!hs)
        return NULL;

    hap_char_t *hc;
    for (hc = hap_serv_get_first_char(hs); hc; hc = hap_char_get_next(hc)) {
        if (((__hap_char_t *)hc)->iid == iid)
            return hc;
    }
    return NULL;
}

/**
 * @brief get target characteristics by it's UUID
 */
hap_char_t *hap_serv_get_char_by_uuid(hap_serv_t *hs, const char *uuid)
{
    if (!hs | !uuid)
        return NULL;

    hap_char_t *hc;
    for (hc = hap_serv_get_first_char(hs); hc; hc = hap_char_get_next(hc)) {
        if (!strcmp(((__hap_char_t *)hc)->type_uuid, uuid))
            return hc;
    }
    return NULL;
}

/**
 * @brief get characteristics UUID prefix number
 */
 char *hap_serv_get_uuid(hap_serv_t *hs)
 {
     return ((__hap_serv_t *)hs)->type_uuid;
 }

/**
 * Default service bulk read callback, which will be registered while creating a service.
 * It will internally call the actual read routines
 */
static int hap_serv_def_bulk_read_cb(hap_read_data_t read_data[], int count,
        void *serv_priv, void *read_priv)
{
    int i;
    if (!count) {
        return HAP_FAIL;
    }
    __hap_serv_t *hs = (__hap_serv_t *)hap_char_get_parent(read_data[0].hc);
    /* If no read routine is registered, just return success so that the cached values
     * will be used
     */
    if (!hs->read_cb) {
        return HAP_SUCCESS;
    }

    int ret = HAP_SUCCESS;
    for (i = 0; i < count; i++) {
       if (hs->read_cb(read_data[i].hc, read_data[i].status, serv_priv, read_priv) != HAP_SUCCESS) {
           ret = HAP_FAIL;
       }
    }
    return ret;
}
/**
 * @brief HAP create a service
 */
hap_serv_t *hap_serv_create(char *type_uuid)
{
    ESP_MFI_ASSERT(type_uuid);
    __hap_serv_t *_hs = hap_platform_memory_calloc(1, sizeof(__hap_serv_t));
    if (!_hs) {
        return NULL;
    }

    _hs->type_uuid = type_uuid;
    _hs->bulk_read = hap_serv_def_bulk_read_cb;

    return (hap_serv_t *)_hs;
}

int hap_serv_link_serv(hap_serv_t *hs, hap_serv_t *linked_serv)
{
    if (!hs || !linked_serv)
        return HAP_FAIL;

    hap_linked_serv_t *cur = hap_platform_memory_calloc(1, sizeof(hap_linked_serv_t));
    if (!cur)
        return HAP_FAIL;
    cur->hs = linked_serv;

    __hap_serv_t *_hs = (__hap_serv_t *)hs;
    hap_linked_serv_t *linked = _hs->linked_servs;

    if (!linked) {
        _hs->linked_servs = cur;
        return HAP_SUCCESS;
    } else {
        while(linked->next) {
            linked = linked->next;
        }
        linked->next = cur;
        return HAP_SUCCESS;
    }
    return HAP_FAIL;
}

/**
 * @brief HAP get target service IID
 */
uint32_t hap_serv_get_iid(hap_serv_t *hs)
{
    if (!hs)
       return 0;

    __hap_serv_t *tmp = (__hap_serv_t *)hs;

    return tmp->iid;
}

char *hap_serv_get_type_uuid(hap_serv_t *hs)
{
    if (!hs)
       return 0;

    __hap_serv_t *tmp = (__hap_serv_t *)hs;

    return tmp->type_uuid;
}

/**
 * @brief HAP delete target service
 */
void hap_serv_delete(hap_serv_t *hs)
{
	/* Returning success even if pointer is NULL, because it means
	 * that the service is absent and as good as deleted
	 */
	if (!hs)
		return;
	__hap_serv_t *_hs = (__hap_serv_t *)hs;
	__hap_char_t *_hc = (__hap_char_t *)_hs->chars;
	while (_hc) {
		_hs->chars = _hc->next_char;
		hap_char_delete((hap_char_t *)_hc);
		_hc = (__hap_char_t *)_hs->chars;
	}
    if (_hs->linked_servs) {
        hap_linked_serv_t *cur = _hs->linked_servs;
        hap_linked_serv_t *next = cur->next;
        while (next) {
            hap_platform_memory_free(cur);
            cur = next;
            next = cur->next;
        }
        hap_platform_memory_free(cur);
    }
    hap_platform_memory_free(hs);
}

/**
 * @brief add a characteristics to a service
 */
int hap_serv_add_char(hap_serv_t *hs, hap_char_t *hc)
{
    if (!hs || !hc)
        return -1;
    __hap_serv_t *_hs = (__hap_serv_t *)hs;
    __hap_char_t *_hc = (__hap_char_t *)hc;

    if (_hc->parent) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Characteristic already added");
        return HAP_FAIL;
    }

    /* If the service has no characteristics, add this as the first */
    if (!_hs->chars) {
        _hs->chars = hc;
    } else {
        /* Else loop through the characteristics to get to the last one,
         * and add this at the end
         */
        __hap_char_t *temp = (__hap_char_t *)_hs->chars;
        while (temp->next_char)
            temp = (__hap_char_t *)temp->next_char;
        temp->next_char = hc;
    }
    if (_hs->parent) {
        _hc->iid = ((__hap_acc_t *)(_hs->parent))->next_iid++;
    }
    _hc->parent = hs;
    return 0;
}

void hap_serv_set_write_cb(hap_serv_t *hs, hap_serv_write_t write)
{
    if (hs) {
        ((__hap_serv_t *)hs)->write_cb = write;
    }
}

void hap_serv_set_read_cb(hap_serv_t *hs, hap_serv_read_t read)
{
    if (hs) {
        ((__hap_serv_t *)hs)->read_cb = read;
    }
}

void hap_serv_set_bulk_read_cb(hap_serv_t *hs, hap_serv_bulk_read_t read)
{
    if (hs) {
        ((__hap_serv_t *)hs)->bulk_read = read;
    }
}


hap_serv_t *hap_serv_get_next(hap_serv_t *hs)
{
    if (hs) {
        return ((__hap_serv_t *)hs)->next_serv;
    } else {
        return NULL;
    }
}

hap_acc_t *hap_serv_get_parent(hap_serv_t *hs)
{
    if (hs) {
        return ((__hap_serv_t *)hs)->parent;
    } else {
        return NULL;
    }
}

void hap_serv_set_priv(hap_serv_t *hs, void *priv)
{
    if (hs) {
        ((__hap_serv_t *)hs)->priv = priv;
    }
}

void *hap_serv_get_priv(hap_serv_t *hs)
{
    if (hs) {
        return ((__hap_serv_t *)hs)->priv;
    } else {
        return NULL;
    }
}
