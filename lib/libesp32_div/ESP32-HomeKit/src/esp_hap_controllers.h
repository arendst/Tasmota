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
#ifndef _HAP_CONTROLLERS_H_
#define _HAP_CONTROLLERS_H_

#include <stdint.h>
#include <stdbool.h>

#define HAP_MAX_CONTROLLERS 16
#define HAP_CTRL_ID_LEN		64
#define ED_KEY_LEN		32


typedef struct {
	char id[HAP_CTRL_ID_LEN];
	uint8_t ltpk[ED_KEY_LEN];
	uint8_t perms;
} __attribute__((packed)) hap_ctrl_info_t;

typedef struct {
    hap_ctrl_info_t info;
	/* If "valid" is false, it means that the entry is invalid,
	 * irrespective of the values of other members, and can be
	 * used to store new controller info
	 */
	bool valid;
    /* Index is used just for better managing the keystore data */
    uint8_t index;
} hap_ctrl_data_t;

int hap_controllers_init();
bool is_accessory_paired();
bool is_admin_paired();
hap_ctrl_data_t *hap_controller_get_empty_loc();
int hap_controller_save(hap_ctrl_data_t *ctrl_data);
void hap_controller_remove(hap_ctrl_data_t *ctrl_data);
hap_ctrl_data_t *hap_get_controller(char *ctrl_id);
void hap_erase_controller_info();

#endif /* _HAP_CONTROLLERS_H_ */
