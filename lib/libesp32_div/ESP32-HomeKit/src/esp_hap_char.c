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

#include <hap_platform_memory.h>
#include <math.h>
#include <string.h>
#include "esp_mfi_debug.h"

#include <esp_hap_main.h>
#include <esp_hap_acc.h>
#include <esp_hap_char.h>
#include <esp_hap_ip_services.h>
#include <esp_hap_database.h>

static QueueHandle_t hap_event_queue;

/**
 * @brief get characteristics's value
 */
hap_val_t *esp_mfi_characteristics_get_value(__hap_char_t *_hc)
{
    return &_hc->val;
}

/**
 * @brief check if characteristics has the specific permission
 */
bool hap_char_has_permission(__hap_char_t *_hc, uint16_t permission)
{
    return _hc->permission & permission ? true : false;
}

/**
 * @brief compute the mod of a and b: a % b
 */
static double esp_mfi_fmod(double a, double b)
{
    if (a < 1e-06 || b < 1e-06)
        return 0;

    while (!((long long)a) || !((long long)b)) {
        a *= 10.0;
        b *= 10.0;
    }

    return fmod(a, b);
}

int hap_event_queue_init()
{
    hap_event_queue = xQueueCreate( hap_priv.cfg.max_event_notif_chars,
                    sizeof(hap_char_t *) );
    if (hap_event_queue) {
        return HAP_SUCCESS;
    } else {
        return HAP_FAIL;
    }
}

hap_char_t * hap_get_pending_notif_char()
{
    hap_char_t *hc;
    if (xQueueReceive(hap_event_queue, &hc, 0) != pdTRUE) {
        return NULL;
    }
    return hc;
}

static int hap_queue_event(hap_char_t *hc)
{
    int ret;
    if (!hap_event_queue) {
        return HAP_FAIL;
    }
    if (xPortInIsrContext() == pdTRUE) {
        ret = xQueueSendFromISR(hap_event_queue, &hc, NULL);
    } else {
        ret = xQueueSend(hap_event_queue, &hc, 0);
    }
    if (ret == pdTRUE) {
        hap_send_event(HAP_INTERNAL_EVENT_TRIGGER_NOTIF);
        return HAP_SUCCESS;
    }
    return HAP_FAIL;
}


/**
 * @brief check if characteristics value is at the range
 */
int hap_char_check_val_constraints(__hap_char_t *_hc, hap_val_t *val)
{
    if (!_hc->constraint_flags)
        return HAP_SUCCESS;

    if (_hc->format == HAP_CHAR_FORMAT_INT) {
        int value = val->i;
        int remainder;

        if (value > _hc->max.i
            || value < _hc->min.i)
            return HAP_FAIL;

        if (!_hc->step.i)
            return HAP_SUCCESS;

        remainder = (value - _hc->min.i) % _hc->step.i;
        if (remainder)
            return HAP_FAIL;
    } else if (_hc->format == HAP_CHAR_FORMAT_FLOAT) {
        float value = val->f;

        if (value > _hc->max.f
            || value < _hc->min.f)
            return HAP_FAIL;
# if 0
        /* Check for step value for floats has a high chance of failure,
         * because of precision issues. Hence, better to skip it.
         */
        double remainder;
        if (_hc->step.f == 0.0)
            return HAP_SUCCESS;

        remainder = esp_mfi_fmod(value - _hc->min.f, _hc->step.f);
        if (remainder != 0.0)
            return HAP_FAIL;
#endif
    } else if (_hc->format == HAP_CHAR_FORMAT_UINT8
               || _hc->format == HAP_CHAR_FORMAT_UINT16
               || _hc->format == HAP_CHAR_FORMAT_UINT32) {
        uint32_t value = val->u;
        uint32_t remainder;


        if (value > _hc->max.u
            || value < _hc->min.u)
            return HAP_FAIL;

        if (!_hc->step.u)
            return HAP_SUCCESS;

        remainder = (value - _hc->min.u) % _hc->step.u;
        if (remainder)
            return HAP_FAIL;
    } else if (_hc->format == HAP_CHAR_FORMAT_UINT64) {
        /* TODO: Add support. Currently, there is no particular 64-bit characteristic */
    }

    return HAP_SUCCESS;
}

/**
 * @brief user update characteristics value, preparing for notification
 */
int hap_char_update_val(hap_char_t *hc, hap_val_t *val)
{
    if (!hc || !val) {
        return HAP_FAIL;
    }
    __hap_char_t *_hc = (__hap_char_t *)hc;
    _hc->update_called = true;
    if (hap_char_check_val_constraints(_hc, val) != HAP_SUCCESS)
        return HAP_FAIL;
	/* Boolean to track if the value has changed.
	 * This will be later used to decide if an event notification
	 * is required or not. If the new and old values are same,
	 * there is no need of a notification
	 */
	bool value_changed = false;

	switch (_hc->format) {
		case HAP_CHAR_FORMAT_BOOL:
			if (_hc->val.b != val->b) {
				_hc->val.b = val->b;
				value_changed = true;
			}
			break;
		case HAP_CHAR_FORMAT_INT:
		case HAP_CHAR_FORMAT_UINT8:
		case HAP_CHAR_FORMAT_UINT16:
		case HAP_CHAR_FORMAT_UINT32:
			if (_hc->val.i != val->i) {
				_hc->val.i = val->i;
				value_changed = true;
			}
			break;
		case HAP_CHAR_FORMAT_FLOAT:
			if (_hc->val.f != val->f) {
				_hc->val.f = val->f;
				value_changed = true;
			}
			break;
		case HAP_CHAR_FORMAT_STRING:
			/* Not checking all combinations to find if value has changed,
			 * since generally we do not expect dynamic string values
			 *
			 * Eg.
			 * Both (old and new) values being NULL
			 * Old value NULL, New non-NULL
			 * Old value non-NULL, new NULL
			 */
			if (_hc->val.s && val->s && !strcmp(_hc->val.s, val->s))
				value_changed = false;
			else
				value_changed = true;

			if (_hc->val.s) {
				hap_platform_memory_free(_hc->val.s);
                _hc->val.s = NULL;
            }

			if (val->s) {
				_hc->val.s = strdup(val->s);
				if (!_hc->val.s)
					return HAP_FAIL;
			}
			break;
        case HAP_CHAR_FORMAT_DATA:
        case HAP_CHAR_FORMAT_TLV8: {
            _hc->val.d.buf = val->d.buf;
            _hc->val.d.buflen = val->d.buflen;
            value_changed = true;
            }
            break;
		default:
			break;
	}
	if (value_changed || (_hc->permission & HAP_CHAR_PERM_SPECIAL_READ)) {
		ESP_MFI_DEBUG_INTR(ESP_MFI_DEBUG_INFO, "Value Changed");
        hap_queue_event(hc);
	} else {
        /* If there is no value change, reset the owner flag here itself, as no notification
         * is being sent anyways. In the absence of this, if there is a GET /characteristics
         * followed by some value change from hardware, the owner_ctrl stays assigned to a
         * stale value, and so the controller misses a notification.
         */
        _hc->owner_ctrl = 0;
    }
	return HAP_SUCCESS;
}

const hap_val_t *hap_char_get_val(hap_char_t *hc)
{
    if (!hc)
        return NULL;
    return &((__hap_char_t *)hc)->val;
}

/**
 * @brief Get the minimum value of characteristic
 */
const hap_val_t *hap_char_get_min_val(hap_char_t *hc)
{
    if (hc) {
        if(((__hap_char_t *)hc)->constraint_flags & HAP_CHAR_MIN_FLAG) {
            return &((__hap_char_t *)hc)->min;
        }
    }
    return NULL;
}

/**
 * @brief Get the maximum value of characteristic
 */
const hap_val_t *hap_char_get_max_val(hap_char_t *hc)
{
    if (hc) {
        if(((__hap_char_t *)hc)->constraint_flags & HAP_CHAR_MAX_FLAG || ((__hap_char_t *)hc)->constraint_flags & HAP_CHAR_MAXLEN_FLAG) {
            return &((__hap_char_t *)hc)->max;
        }
    }
    return NULL;
}

/**
 * @brief Get the step value of characteristic
 */
const hap_val_t *hap_char_get_step_val(hap_char_t *hc)
{
    if (hc) {
        if (((__hap_char_t *)hc)->constraint_flags & HAP_CHAR_STEP_FLAG) {
            return &((__hap_char_t *)hc)->step;
        }
    }
    return NULL;
}

/**
 * @brief HAP create a characteristics
 */
static hap_char_t *hap_char_create(const char *type_uuid, uint32_t permission, hap_char_format_t format, hap_val_t val)
{
    __hap_char_t *new_ch;

    ESP_MFI_ASSERT(type_uuid);

    if (HAP_CHAR_FORMAT_STRING == format && val.s) {
        if (strlen(val.s) > HAP_CHAR_STRING_MAX_LEN)
            return NULL;
    }

    new_ch = hap_platform_memory_calloc(1, sizeof(__hap_char_t));
    if (!new_ch) {
        return NULL;
    }

    new_ch->val = val;
    new_ch->type_uuid = type_uuid;
    new_ch->format = format;
    new_ch->permission = permission;

    return (hap_char_t *) new_ch;
}

hap_char_t *hap_char_bool_create(const char *type_uuid, uint16_t perms, bool b)
{
    hap_val_t val = {.b = b};
    return hap_char_create(type_uuid, perms, HAP_CHAR_FORMAT_BOOL, val);
}
hap_char_t *hap_char_uint8_create(const char *type_uuid, uint16_t perms, uint8_t u8)
{
    hap_val_t val = {.u = u8};
    return hap_char_create(type_uuid, perms, HAP_CHAR_FORMAT_UINT8, val);
}
hap_char_t *hap_char_uint16_create(const char *type_uuid, uint16_t perms, uint16_t u16)
{
    hap_val_t val = {.u = u16};
    return hap_char_create(type_uuid, perms, HAP_CHAR_FORMAT_UINT16, val);
}
hap_char_t *hap_char_uint32_create(const char *type_uuid, uint16_t perms, uint32_t u32)
{
    hap_val_t val = {.u = u32};
    return hap_char_create(type_uuid, perms, HAP_CHAR_FORMAT_UINT32, val);
}
hap_char_t *hap_char_uint64_create(const char *type_uuid, uint16_t perms, uint64_t u64)
{
    hap_val_t val = {.i64 = u64};
    return hap_char_create(type_uuid, perms, HAP_CHAR_FORMAT_UINT64, val);
}
hap_char_t *hap_char_int_create(const char *type_uuid, uint16_t perms, int i32)
{
    hap_val_t val = {.i = i32};
    return hap_char_create(type_uuid, perms, HAP_CHAR_FORMAT_INT, val);
}
hap_char_t *hap_char_float_create(const char *type_uuid, uint16_t perms, float f)
{
    hap_val_t val = {.f = f};
    return hap_char_create(type_uuid, perms, HAP_CHAR_FORMAT_FLOAT, val);
}
hap_char_t *hap_char_string_create(const char *type_uuid, uint16_t perms, const char *s)
{
    hap_val_t val;
    if (s)
        val.s = strdup(s);
    else
        val.s = NULL;
    return hap_char_create(type_uuid, perms, HAP_CHAR_FORMAT_STRING, val);
}

hap_char_t *hap_char_data_create(const char *type_uuid, uint16_t perms, hap_data_val_t *d)
{
    hap_val_t val = {0};
    if (d) {
        val.d = *d;
    }
    return hap_char_create(type_uuid, perms, HAP_CHAR_FORMAT_DATA, val);
}

hap_char_t *hap_char_tlv8_create(const char *type_uuid, uint16_t perms, hap_tlv8_val_t *t)
{
    hap_val_t val = {0};
    if (t) {
        val.t = *t;
    }
    return hap_char_create(type_uuid, perms, HAP_CHAR_FORMAT_TLV8, val);
}

/**
 * @brief HAP get target characteristics IID
 */
uint32_t hap_char_get_iid(hap_char_t *hc)
{
    ESP_MFI_ASSERT(hc);
    __hap_char_t *tmp = (__hap_char_t *)hc;

    return tmp->iid;
}

/**
 * @brief HAP get target characteristics type UUID
 */
const char * hap_char_get_type_uuid(hap_char_t *hc)
{
    ESP_MFI_ASSERT(hc);
    __hap_char_t *tmp = (__hap_char_t *)hc;

    return tmp->type_uuid;
}

uint16_t hap_char_get_perm(hap_char_t *hc)
{
    if (!hc)
        return 0;

    __hap_char_t *tmp = (__hap_char_t *)hc;
    return tmp->permission;
}

hap_char_format_t hap_char_get_format(hap_char_t *hc)
{
    if (!hc)
        return 0;

    __hap_char_t *tmp = (__hap_char_t *)hc;
    return tmp->format;
}

/**
 * @brief HAP delete target characteristics
 */
void hap_char_delete(hap_char_t *hc)
{
    ESP_MFI_ASSERT(hc);
    __hap_char_t *_hc = (__hap_char_t *)hc;
    if (_hc->format == HAP_CHAR_FORMAT_STRING) {
        if (_hc->val.s) {
            hap_platform_memory_free(_hc->val.s);
        }
    }
    if (_hc->valid_vals) {
        hap_platform_memory_free(_hc->valid_vals);
    }
    if (_hc->valid_vals_range) {
        hap_platform_memory_free(_hc->valid_vals_range);
    }
    hap_platform_memory_free(_hc);
}

/**
 * @brief HAP configure the characteristics's value description
 */
void hap_char_int_set_constraints(hap_char_t *hc, int min, int max, int step)
{
    ESP_MFI_ASSERT(hc);
    __hap_char_t *tmp = (__hap_char_t *)hc;
    tmp->min.i = min;
    tmp->max.i = max;
    tmp->step.i = step;
    if (step) {
        tmp->constraint_flags |= (HAP_CHAR_MIN_FLAG | HAP_CHAR_MAX_FLAG | HAP_CHAR_STEP_FLAG);
    } else {
        tmp->constraint_flags |= (HAP_CHAR_MIN_FLAG | HAP_CHAR_MAX_FLAG);
    }
}
void hap_char_float_set_constraints(hap_char_t *hc, float min, float max, float step)
{
    ESP_MFI_ASSERT(hc);
    __hap_char_t *tmp = (__hap_char_t *)hc;
    tmp->min.f = min;
    tmp->max.f = max;
    tmp->step.f = step;
    if (step) {
        tmp->constraint_flags |= (HAP_CHAR_MIN_FLAG | HAP_CHAR_MAX_FLAG | HAP_CHAR_STEP_FLAG);
    } else {
        tmp->constraint_flags |= (HAP_CHAR_MIN_FLAG | HAP_CHAR_MAX_FLAG);
    }
}

void hap_char_string_set_maxlen(hap_char_t *hc, int maxlen)
{
    ESP_MFI_ASSERT(hc);
    __hap_char_t *tmp = (__hap_char_t *)hc;
    if (maxlen > HAP_CHAR_STRING_MAX_LEN) {
        maxlen = HAP_CHAR_STRING_MAX_LEN;
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_WARN, "Characteristic string length larger than maximum value(%d), falling back to the maximum value.", HAP_CHAR_STRING_MAX_LEN);
    }
    tmp->max.i = maxlen;
    tmp->constraint_flags |= HAP_CHAR_MAXLEN_FLAG;
}

void hap_char_add_description(hap_char_t *hc, const char *description)
{
    ESP_MFI_ASSERT(hc);
    __hap_char_t *tmp = (__hap_char_t *)hc;
    tmp->description = (char *)description;
}
void hap_char_add_unit(hap_char_t *hc, const char *unit)
{
    ESP_MFI_ASSERT(hc);
    __hap_char_t *tmp = (__hap_char_t *)hc;
    tmp->unit = (char *)unit;
}
hap_char_t *hap_char_get_next(hap_char_t *hc)
{
    return ((__hap_char_t *)hc)->next_char;
}

hap_serv_t *hap_char_get_parent(hap_char_t *hc)
{
    return ((__hap_char_t *)hc)->parent;

}

#define set_bit(val, index)	((val) |= (1 << index))
#define reset_bit(val, index)	((val) &= ~(1 << index))
void hap_char_manage_notification(hap_char_t *hc, int index, bool ev)
{
	__hap_char_t *_hc = (__hap_char_t *)hc;
	if (ev)
		set_bit(_hc->ev_ctrls, index);
	else
		reset_bit(_hc->ev_ctrls, index);
}

bool hap_char_is_ctrl_subscribed(hap_char_t *hc, int index)
{
	__hap_char_t *_hc = (__hap_char_t *)hc;
	return (_hc->ev_ctrls & (1 << index)) ? true : false;
}

void hap_char_set_owner_ctrl(hap_char_t *hc, int index)
{
	__hap_char_t *_hc = (__hap_char_t *)hc;
    _hc->owner_ctrl = 0;
    set_bit(_hc->owner_ctrl, index);
}

bool hap_char_is_ctrl_owner(hap_char_t *hc, int index)
{
	__hap_char_t *_hc = (__hap_char_t *)hc;
	return (_hc->owner_ctrl & (1 << index)) ? true : false;
}

void hap_char_set_iid(hap_char_t *hc, int32_t iid)
{
    if (hc) {
        ((__hap_char_t *)hc)->iid = iid;
    }
}

void hap_disable_all_char_notif(int index)
{
    /* Just loop through all characteristic objects and reset the
     * bit indicating event notifications. This is the simplest way
     */
    hap_acc_t *ha;
    hap_serv_t *hs;
    hap_char_t *hc;
    for (ha = hap_get_first_acc(); ha; ha = hap_acc_get_next(ha)) {
        for (hs = hap_acc_get_first_serv(ha); hs; hs = hap_serv_get_next(hs)) {
            for (hc = hap_serv_get_first_char(hs); hc; hc = hap_char_get_next(hc)) {
                reset_bit(((__hap_char_t *)hc)->ev_ctrls, index);
            }
        }
    }
}

void hap_char_add_valid_vals(hap_char_t *hc, const uint8_t *valid_vals, size_t valid_val_cnt)
{
    if (!hc)
        return;
    __hap_char_t *_hc = (__hap_char_t *)hc;
    _hc->valid_vals = hap_platform_memory_malloc(valid_val_cnt);
    if (_hc->valid_vals) {
        memcpy(_hc->valid_vals, valid_vals, valid_val_cnt);
        _hc->valid_vals_cnt = valid_val_cnt;
    }
}

void hap_char_add_valid_vals_range(hap_char_t *hc, uint8_t start_val, uint8_t end_val)
{
    if (!hc)
        return;
    __hap_char_t *_hc = (__hap_char_t *)hc;
    _hc->valid_vals_range = hap_platform_memory_malloc(sizeof(uint8_t));
    if (_hc->valid_vals_range) {
        _hc->valid_vals_range[0] = start_val;
        _hc->valid_vals_range[1] = end_val;
    }
}
