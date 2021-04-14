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
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <esp_hap_database.h>
#include <esp_hap_controllers.h>
#include <esp_hap_pair_common.h>
#include <esp_hap_pair_verify.h>
#include <esp_hap_main.h>
#include <esp_mfi_debug.h>

bool hap_is_req_admin(void *__session)
{
    hap_secure_session_t *session = (hap_secure_session_t *)__session;
    if (!session) {
        return false;
    }
	if ((session->state == STATE_VERIFIED) && (session->ctrl->info.perms == 1)) {
		return true;
    }
    return false;
}

char *hap_req_get_ctrl_id(void *__session)
{
    hap_secure_session_t *session = (hap_secure_session_t *)__session;
    if (!session)
        return NULL;

    if (session->state == STATE_VERIFIED) {
        return session->ctrl->info.id;
    }
    return NULL;
}

bool hap_is_req_secure(hap_secure_session_t *session)
{
    if (!session) {
        return false;
    }
    if (session->state == STATE_VERIFIED) {
        return true;
    }
    return false;
}

void hap_remove_all_controllers()
{
	int i;
	for (i = 0; i < HAP_MAX_CONTROLLERS; i++) {
		if (hap_priv.controllers[i].valid) {
			hap_close_ctrl_sessions(&hap_priv.controllers[i]);
			hap_controller_remove(&hap_priv.controllers[i]);
		}
	}
}
static int hap_process_pair_remove(uint8_t *buf, int inlen, int bufsize, int *outlen)
{
    bool acc_unpaired = false;
	char ctrl_id[HAP_CTRL_ID_LEN];
	memset(ctrl_id, 0, HAP_CTRL_ID_LEN);
	if (get_value_from_tlv(buf, inlen, kTLVType_Identifier,
					ctrl_id, sizeof(ctrl_id)) < 0) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Identifier not found");
		hap_prepare_error_tlv(STATE_M2, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
	}

	ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Removing Controller %s", ctrl_id);
	hap_ctrl_data_t *ctrl = hap_get_controller(ctrl_id);
	hap_close_ctrl_sessions(ctrl);
	hap_controller_remove(ctrl);

	if (!is_admin_paired()) {
        acc_unpaired = true;
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Last Admin controller removed. Removing all other controllers.");
		hap_remove_all_controllers();
    }

	hap_tlv_data_t tlv_data = {
		.bufptr = buf,
		.bufsize = bufsize,
		.curlen = 0,
	};
	uint8_t state = STATE_M2;
	if (add_tlv(&tlv_data, kTLVType_State, sizeof(state), &state) < 0) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "TLV creation failed");
		hap_prepare_error_tlv(STATE_M2, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
	}
	*outlen = tlv_data.curlen;
    if (acc_unpaired) {
        hap_send_event(HAP_INTERNAL_EVENT_ACC_UNPAIRED);
    }
	return HAP_SUCCESS;
}

static int hap_process_pair_add(uint8_t *buf, int inlen, int bufsize, int *outlen)
{
	char ctrl_id[HAP_CTRL_ID_LEN];
	uint8_t ltpkc[ED_KEY_LEN];
	uint8_t perms;
	memset(ctrl_id, 0, HAP_CTRL_ID_LEN);
	if ((get_value_from_tlv(buf, inlen, kTLVType_Identifier,
						ctrl_id, sizeof(ctrl_id)) < 0) ||
		(get_value_from_tlv(buf, inlen, kTLVType_PublicKey,
				    ltpkc, sizeof(ltpkc)) < 0) ||
		 (get_value_from_tlv(buf, inlen, kTLVType_Permissions,
				     &perms, sizeof(perms)) < 0)) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Invalid TLVs received");
		hap_prepare_error_tlv(STATE_M4, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
	}

	hap_ctrl_data_t *ctrl = hap_get_controller(ctrl_id);
	if (ctrl) {
		/* If controller already exists, but the key does not match, return error */
		if (memcmp(ltpkc, ctrl->info.ltpk, ED_KEY_LEN)) {
			hap_prepare_error_tlv(STATE_M2, kTLVError_Unknown, buf, bufsize, outlen);
			return HAP_FAIL;
		} else {
			/* Else, just change the permissions */
			ctrl->info.perms = perms;
	        ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Modifying Existing Controller %s", ctrl_id);
			hap_controller_save(ctrl);
		}
	} else {
		/* Check if there is space available for a new controller */
		ctrl = 	hap_controller_get_empty_loc();
		if (!ctrl) {
			hap_prepare_error_tlv(STATE_M2, kTLVError_MaxPeers, buf, bufsize, outlen);
			return HAP_FAIL;
		}
		strcpy(ctrl->info.id, ctrl_id);
		memcpy(ctrl->info.ltpk, ltpkc, ED_KEY_LEN);
		ctrl->info.perms = perms;
		hap_controller_save(ctrl);
	    ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Added Controller %s", ctrl_id);
	}
	hap_tlv_data_t tlv_data = {
		.bufptr = buf,
		.bufsize = bufsize,
		.curlen = 0,
	};
	uint8_t state = STATE_M2;
	if (add_tlv(&tlv_data, kTLVType_State, sizeof(state), &state) < 0) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "TLV creation failed");
		hap_prepare_error_tlv(STATE_M2, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
	}
	*outlen = tlv_data.curlen;
	return HAP_SUCCESS;

}
static int hap_process_pair_list(uint8_t *buf, int inlen, int bufsize, int *outlen)
{
	hap_tlv_data_t tlv_data = {
		.bufptr = buf,
		.bufsize = bufsize,
		.curlen = 0,
	};
	int i;
	hap_ctrl_data_t *ctrl;
    hap_ctrl_info_t *ctrl_info;
	bool entry_added = false;
	for (i = 0; i < HAP_MAX_CONTROLLERS; i++) {
		ctrl = &hap_priv.controllers[i];
		if (!ctrl->valid)
			continue;
        /* If an entry is already added, include a separator */
        if (entry_added) {
            if (add_tlv(&tlv_data, kTLVType_Separator, 0, NULL) < 0) {
                ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "TLV creation failed");
                hap_prepare_error_tlv(STATE_M2, kTLVError_Unknown,
                        buf, bufsize, outlen);
                return HAP_FAIL;
            }
        } else {
            /* Else, since this is the first entry, add the state */
	        uint8_t state = STATE_M2;
            if (add_tlv(&tlv_data, kTLVType_State, sizeof(state), &state) < 0) {
                ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "TLV creation failed");
                hap_prepare_error_tlv(STATE_M2, kTLVError_Unknown,
                        buf, bufsize, outlen);
                return HAP_FAIL;
            }
        }
        ctrl_info = &ctrl->info;
		if ((add_tlv(&tlv_data, kTLVType_Identifier,
						strlen(ctrl_info->id), ctrl_info->id) < 0) ||
				(add_tlv(&tlv_data, kTLVType_PublicKey,
					 ED_KEY_LEN, ctrl_info->ltpk) < 0) ||
				(add_tlv(&tlv_data, kTLVType_Permissions,
					 1, &ctrl_info->perms) < 0)) {
			ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "TLV creation failed");
			hap_prepare_error_tlv(STATE_M2, kTLVError_Unknown, buf, bufsize, outlen);
			return HAP_FAIL;
		}
		entry_added = true;
	}
	*outlen = tlv_data.curlen;
	return HAP_SUCCESS;
}

int hap_pairings_process(void *ctx, uint8_t *buf, int inlen, int bufsize, int *outlen)
{
	if (!hap_is_req_admin(ctx)) {
		hap_prepare_error_tlv(STATE_M2, kTLVError_Authentication, buf, bufsize, outlen);
		return HAP_FAIL;
	}
	uint8_t state, method;
	if ((get_value_from_tlv(buf, inlen, kTLVType_State, &state, sizeof(state)) < 0) ||
			(get_value_from_tlv(buf, inlen, kTLVType_Method,
					    &method, sizeof(method)) < 0) ||
			(state != STATE_M1)) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Invalid TLVs received");
		hap_prepare_error_tlv(STATE_M2, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
	}
	if (method == HAP_METHOD_ADD_PAIRING) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Add Pairing received");
		return hap_process_pair_add(buf, inlen, bufsize, outlen);
	} else if (method == HAP_METHOD_REMOVE_PAIRING) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Remove Pairing received");
		return hap_process_pair_remove(buf, inlen, bufsize, outlen);
	} else if (method == HAP_METHOD_LIST_PAIRINGS) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "List Pairings received");
		return hap_process_pair_list(buf, inlen, bufsize, outlen);
	}
	hap_prepare_error_tlv(STATE_M2, kTLVError_Unknown, buf, bufsize, outlen);
	return HAP_FAIL;
}
