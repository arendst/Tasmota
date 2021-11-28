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


#include <json_parser.h>
#include <hap_platform_memory.h>
#include <esp_mfi_debug.h>
#include <esp_hap_main.h>
#include <esp_hap_pair_setup.h>
#include <esp_hap_pair_verify.h>
#include <esp_hap_pairings.h>
#include <esp_hap_network_io.h>
#include <esp_hap_secure_message.h>
#include <esp_hap_acc.h>
#include <esp_hap_serv.h>
#include <esp_hap_char.h>
#include <esp_hap_mdns.h>
#include <esp_hap_wac.h>
#include <esp_hap_wifi.h>
#include <esp_hap_database.h>
#include <esp_mfi_base64.h>
#include <esp_timer.h>
#include <hexdump.h>
#include <lwip/sockets.h>
#include <esp_http_server.h>
#include <hap_platform_httpd.h>
#include <hap_platform_os.h>
#include <esp_hap_ip_services.h>
#include <json_generator.h> //???

#ifdef ESP_MFI_DEBUG_ENABLE
#define ESP_MFI_DEBUG_PLAIN(fmt, ...)   \
    if (http_debug) {                   \
        printf("\e[1;35m" fmt "\e[0m", ##__VA_ARGS__); \
    }
#else /* ESP_MFI_DEBUG_ENABLE */
#define ESP_MFI_DEBUG_PLAIN(fmt, ...)
#endif /* ESP_MFI_DEBUG_ENABLE */

static bool http_debug;

int hap_http_session_not_authorized(httpd_req_t *req)
{
    char buf[50];
    httpd_resp_set_status(req, "470 Connection Authorization Required");
    httpd_resp_set_type(req, "application/hap+json");
    snprintf(buf, sizeof(buf),"{\"status\":-70401}");
    httpd_resp_send(req, buf, strlen(buf));
    return HAP_SUCCESS;

}

int hap_httpd_get_data(httpd_req_t *req, char *buffer, int len)
{
    int read_len = 0;
    while (read_len < len) {
        int tmp_len = httpd_req_recv(req, buffer + read_len, len - read_len);
        if (tmp_len <= 0) {
            return read_len;
        }
        read_len += tmp_len;
    }
    return read_len;
}

static int hap_http_pair_setup_handler(httpd_req_t *req)
{
	uint8_t buf[1200];
	int ret, ret1, outlen;
	void *ctx = (hap_secure_session_t *)hap_platform_httpd_get_sess_ctx(req);
    int fd = httpd_req_to_sockfd(req);
    ESP_MFI_DEBUG_PLAIN("Socket fd: %d; HTTP Request %s %s\n", httpd_req_to_sockfd(req), hap_platform_httpd_get_req_method(req), hap_platform_httpd_get_req_uri(req));
	if (!ctx) {
		if (hap_pair_setup_context_init(fd, &ctx, buf, sizeof(buf), &outlen) == HAP_SUCCESS) {
            hap_platform_httpd_set_sess_ctx(req, ctx, hap_pair_setup_ctx_clean, true);
		} else {
			httpd_resp_set_type(req, "application/pairing+tlv8");
			httpd_resp_send(req, (char *)buf, outlen);
			return HAP_SUCCESS;
		}
	}
	int data_len = httpd_req_recv(req, (char *)buf, sizeof(buf));
	ret = hap_pair_setup_process(&ctx, buf, data_len, sizeof(buf), &outlen);
	httpd_resp_set_type(req, "application/pairing+tlv8");
	ret1 = httpd_resp_send(req, (char *)buf, outlen);
	if (ret != HAP_SUCCESS) {
		hap_pair_setup_ctx_clean(ctx);
		ctx = NULL;
	} else {
        /* A pair verify function is called here, because for Software Token Authentication,
         * secure session keys are generated at the step M4 of Pair Setup, and there onwards,
         * the behavior is like a pair verified session.
         */
		if (hap_pair_verify_get_state(ctx) == STATE_VERIFIED) {
			/* Saving socket fd since it will later be required for
			 * event notifications.
			 */
			((hap_secure_session_t *)ctx)->conn_identifier = fd;
            hap_platform_httpd_set_sess_ctx(req, ctx, hap_free_session, true);
            httpd_sess_set_send_override(hap_priv.server, fd, hap_httpd_send);
            httpd_sess_set_recv_override(hap_priv.server, fd, hap_httpd_recv);
		}
    }
	/* Context will be NULL, either if there was an error and a cleanup was required,
	 * or if the pair_setup_process cleared it after successful pairing.
	 * For both the cases, we will set the sess_ctx and free_ctx to NULL
	 */
	if (!ctx) {
        hap_platform_httpd_set_sess_ctx(req, NULL, NULL, true);
	}
	return ret1;
}
static struct httpd_uri hap_pair_setup = {
	.uri = "/pair-setup",
    .method = HTTP_POST,
    .handler = hap_http_pair_setup_handler,
};

static int hap_http_pair_verify_handler(httpd_req_t *req)
{
	uint8_t buf[512];
	int ret, outlen;
	void *ctx = hap_platform_httpd_get_sess_ctx(req);
    ESP_MFI_DEBUG_PLAIN("Socket fd: %d; HTTP Request %s %s\n", httpd_req_to_sockfd(req), hap_platform_httpd_get_req_method(req), hap_platform_httpd_get_req_uri(req));
	if (!ctx) {
		if (hap_pair_verify_context_init(&ctx, buf, sizeof(buf), &outlen) == HAP_SUCCESS) {
            hap_platform_httpd_set_sess_ctx(req, ctx, NULL, true);
		}
	}
	int data_len = httpd_req_recv(req, (char *)buf, sizeof(buf));
	ret = hap_pair_verify_process(&ctx, buf, data_len, sizeof(buf), &outlen);
	httpd_resp_set_type(req, "application/pairing+tlv8");
	int ret1 = httpd_resp_send(req, (char *)buf, outlen);
	if (ret == HAP_SUCCESS) {
		if (hap_pair_verify_get_state(ctx) == STATE_VERIFIED) {
			/* Saving socket fd since it will later be required for
			 * event notifications.
			 */
            int fd = httpd_req_to_sockfd(req);
			((hap_secure_session_t *)ctx)->conn_identifier = fd;

            struct timeval timeout;
            timeout.tv_sec = hap_priv.cfg.recv_timeout;
            timeout.tv_usec = 0;
            if (setsockopt (fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
                        sizeof(timeout)) < 0) {
                 ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "setsockopt on pair verified socket failed for SO_RCVTIMEO");
            }

            timeout.tv_sec = hap_priv.cfg.send_timeout;
            timeout.tv_usec = 0;
            if (setsockopt (fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
                        sizeof(timeout)) < 0) {
                 ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "setsockopt on pair verified socket failed for SO_SNDTIMEO");
            }


            // ----
            const int s = fd;
            const int yes = 1; /* enable sending keepalive probes for socket */
			setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(yes));

			const int idle = 15; /* 180 sec idle before start sending probes */
			setsockopt(s, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(idle));

			const int interval = 5; /* 30 sec between probes */
			setsockopt(s, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(interval));

			const int maxpkt = 3; /* Drop connection after 4 probes without response */
			setsockopt(s, IPPROTO_TCP, TCP_KEEPCNT, &maxpkt, sizeof(maxpkt));
            // ----

            hap_platform_httpd_set_sess_ctx(req, ctx, hap_free_session, true);
            httpd_sess_set_send_override(hap_priv.server, fd, hap_httpd_send);
            httpd_sess_set_recv_override(hap_priv.server, fd, hap_httpd_recv);


            hap_secure_session_t* session  = (hap_secure_session_t *)ctx;
            int socket = session->conn_identifier;

            //struct sockaddr_storage addr;
            struct sockaddr_in6 addr;
            socklen_t len = sizeof addr;
            getpeername(fd, (struct sockaddr*)&addr, &len);
            /*
            struct sockaddr_in *sockaddr = (struct sockaddr_in *)&addr;
            in_addr_t a = sockaddr->sin_addr.s_addr;
            union {
            	uint8_t ui8[4];
            	uint32_t ui32;
            }ip;
            ip.ui32 = (uint32_t)a;
			*/
            //uint16_t port = ntohs(sockaddr->sin6_port);
            uint16_t port = addr.sin6_port;
            printf("---- STATE_VERIFIED ----\n");
            //printf("ip32: %u\n", a);
            //printf("fd: %d, %u.%u.%u.%u:%u \n", socket,
            		//ip.ui8[0], ip.ui8[1], ip.ui8[2], ip.ui8[3], port);
            //https://github.com/espressif/esp-idf/issues/4863
            printf("fd: %d, remote: %s:%u\n", fd, inet_ntoa(addr.sin6_addr.un.u32_addr[3]), port);
            printf("ctrl: %s\n", session->ctrl->info.id);

            printf("------------------------\n");
		}
	}
	return ret1;
}

static struct httpd_uri hap_pair_verify = {
	.uri = "/pair-verify",
    .method = HTTP_POST,
    .handler = hap_http_pair_verify_handler,
};

static int hap_add_char_val_json(hap_char_format_t format, const char *key,
		hap_val_t *val, json_gen_str_t *jptr)
{
	switch (format) {
		case HAP_CHAR_FORMAT_BOOL : {
			json_gen_obj_set_bool(jptr, key, val->b);
			break;
		}
		case HAP_CHAR_FORMAT_UINT8:
		case HAP_CHAR_FORMAT_UINT16:
		case HAP_CHAR_FORMAT_UINT32:
		case HAP_CHAR_FORMAT_INT: {
			json_gen_obj_set_int(jptr, key, val->i);
			break;
		}
		case HAP_CHAR_FORMAT_FLOAT : {
			json_gen_obj_set_float(jptr, key, val->f);
			break;
		}
		case HAP_CHAR_FORMAT_STRING : {
            if (val->s) {
			    json_gen_obj_set_string(jptr, key, val->s);
            } else {
                json_gen_obj_set_null(jptr, key);
            }
			break;
		}
        case HAP_CHAR_FORMAT_DATA:
        case HAP_CHAR_FORMAT_TLV8: {
            if (val->d.buf) {
                json_gen_obj_start_long_string(jptr, key, NULL);
                uint8_t *buf = val->d.buf;
                uint32_t buflen = val->d.buflen;
                char tmp[100];
                while (buflen) {
                    int tmp_len = sizeof(tmp);
                    if (buflen > 60) {
                        esp_mfi_base64_encode((char *)buf, 60, tmp, tmp_len, &tmp_len);
                        buflen -= 60;
                        buf += 60;
                    } else {
                        esp_mfi_base64_encode((char *)buf, buflen, tmp, tmp_len, &tmp_len);
                        buflen -= buflen;
                    }
                    tmp[tmp_len] = 0;
                    json_gen_add_to_long_string(jptr, tmp);
                }
                json_gen_end_long_string(jptr);
            } else {
                json_gen_obj_set_null(jptr, key);
            }
            break;
        }
		default :
			break;
	}
	return HAP_SUCCESS;
}

static int hap_add_char_format_json(__hap_char_t *hc, json_gen_str_t *jptr)
{
	switch (hc->format) {
		case HAP_CHAR_FORMAT_UINT8:
			return json_gen_obj_set_string(jptr, "format", "uint8");
		case HAP_CHAR_FORMAT_UINT16:
			return json_gen_obj_set_string(jptr, "format", "uint16");
		case HAP_CHAR_FORMAT_UINT32:
			return json_gen_obj_set_string(jptr, "format", "uint32");
		case HAP_CHAR_FORMAT_INT:
			return json_gen_obj_set_string(jptr, "format", "int");
		case HAP_CHAR_FORMAT_BOOL:
			return json_gen_obj_set_string(jptr, "format", "bool");
		case HAP_CHAR_FORMAT_STRING:
			return json_gen_obj_set_string(jptr, "format", "string");
		case HAP_CHAR_FORMAT_FLOAT:
			return json_gen_obj_set_string(jptr, "format", "float");
		case HAP_CHAR_FORMAT_DATA:
			return json_gen_obj_set_string(jptr, "format", "data");
		case HAP_CHAR_FORMAT_TLV8:
			return json_gen_obj_set_string(jptr, "format", "tlv8");
		default:
			break;
	}
	return HAP_SUCCESS;
}

static int hap_add_char_type(__hap_char_t *hc, json_gen_str_t *jptr)
{
	return json_gen_obj_set_string(jptr, "type", (char *)hc->type_uuid);
}

static int hap_add_char_meta(__hap_char_t *hc, json_gen_str_t *jptr)
{
	hap_add_char_format_json(hc, jptr);

	if (hc->constraint_flags & HAP_CHAR_MIN_FLAG)
		hap_add_char_val_json(hc->format, "minValue", &hc->min, jptr);
	if (hc->constraint_flags & HAP_CHAR_MAX_FLAG)
		hap_add_char_val_json(hc->format, "maxValue", &hc->max, jptr);
	if (hc->constraint_flags & HAP_CHAR_STEP_FLAG)
		hap_add_char_val_json(hc->format, "minStep", &hc->step, jptr);

	/* maxLen and maxDataLen are constraints for "string" and "data" format
	 * of characteristics, respectively. However, the constraints themselves
	 * are integers. So, we pass the format as HAP_CHAR_FORMAT_INT
	 */
	if (hc->constraint_flags & HAP_CHAR_MAXLEN_FLAG)
		hap_add_char_val_json(HAP_CHAR_FORMAT_INT, "maxLen", &hc->max, jptr);
	if (hc->constraint_flags & HAP_CHAR_MAXDATALEN_FLAG)
		hap_add_char_val_json(HAP_CHAR_FORMAT_INT, "maxDataLen", &hc->max, jptr);

	if (hc->description)
		json_gen_obj_set_string(jptr, "description", hc->description);
	if (hc->unit)
		json_gen_obj_set_string(jptr, "unit", hc->unit);

	return HAP_SUCCESS;
}

static int hap_add_char_perms(__hap_char_t *hc, json_gen_str_t *jptr)
{
	json_gen_push_array(jptr, "perms");
	if (hc->permission & HAP_CHAR_PERM_PR)
		json_gen_arr_set_string(jptr, "pr");
	if (hc->permission & HAP_CHAR_PERM_PW)
		json_gen_arr_set_string(jptr, "pw");
	if (hc->permission & HAP_CHAR_PERM_EV)
		json_gen_arr_set_string(jptr, "ev");
	if (hc->permission & HAP_CHAR_PERM_AA)
		json_gen_arr_set_string(jptr, "aa");
	if (hc->permission & HAP_CHAR_PERM_TW)
		json_gen_arr_set_string(jptr, "tw");
	if (hc->permission & HAP_CHAR_PERM_HD)
		json_gen_arr_set_string(jptr, "hd");
	json_gen_pop_array(jptr);
	return HAP_SUCCESS;
}

static int hap_add_char_ev(__hap_char_t *hc, json_gen_str_t *jptr, uint8_t session_index)
{
    if (hap_char_is_ctrl_subscribed((hap_char_t *)hc, session_index)) {
        return json_gen_obj_set_bool(jptr, "ev", true);
    } else {
	    return json_gen_obj_set_bool(jptr, "ev", false);
    }
}

static int hap_add_char_valid_vals(__hap_char_t *hc, json_gen_str_t *jptr)
{
    if (hc->valid_vals) {
        json_gen_push_array(jptr, "valid-values");
        int i;
        for (i = 0; i < hc->valid_vals_cnt; i++) {
            json_gen_arr_set_int(jptr, hc->valid_vals[i]);
        }
        json_gen_pop_array(jptr);
    }
    if (hc->valid_vals_range) {
        json_gen_push_array(jptr, "valid-values-range");
        json_gen_arr_set_int(jptr, hc->valid_vals_range[0]);
        json_gen_arr_set_int(jptr, hc->valid_vals_range[1]);
        json_gen_pop_array(jptr);
    }
    return HAP_SUCCESS;
}

static int hap_prepare_char_db(__hap_char_t *hc, json_gen_str_t *jptr, int session_index)
{
	json_gen_start_object(jptr);

	json_gen_obj_set_int(jptr, "iid", hc->iid);

    /* If the Update API has not been called from the service read routine,
     * reset the owner controller value.
     * Else, the controller will  miss the next notification.
     */
    if (!hc->update_called)   {
        hc->owner_ctrl = 0;
    }
    hc->update_called = false;

	if (hc->permission & HAP_CHAR_PERM_PR) {
        if (hc->permission & HAP_CHAR_PERM_SPECIAL_READ) {
            json_gen_obj_set_null(jptr, "value");
        } else {
            hap_add_char_val_json(hc->format, "value", &hc->val, jptr);
        }
	}
	hap_add_char_type(hc, jptr);
	hap_add_char_perms(hc, jptr);
	hap_add_char_ev(hc, jptr, session_index);
	hap_add_char_meta(hc, jptr);
    hap_add_char_valid_vals(hc, jptr);

	json_gen_end_object(jptr);

	return HAP_SUCCESS;
}

static int hap_prepare_serv_db(__hap_serv_t *hs, json_gen_str_t *jptr, int session_index)
{
	json_gen_start_object(jptr);
	json_gen_obj_set_int(jptr, "iid", hs->iid);
	json_gen_obj_set_string(jptr, "type", hs->type_uuid);
	if (hs->hidden)
		json_gen_obj_set_bool(jptr, "hidden", "true");
	if (hs->primary)
		json_gen_obj_set_bool(jptr, "primary", "true");
    if (hs->linked_servs) {
        hap_linked_serv_t *linked = hs->linked_servs;
        json_gen_push_array(jptr, "linked");
        while (linked) {
            json_gen_arr_set_int(jptr, ((__hap_serv_t *)linked->hs)->iid);
            linked = linked->next;
        }
        json_gen_pop_array(jptr);
    }

	json_gen_push_array(jptr, "characteristics");
    int char_cnt = 0;
	hap_char_t *hc;
    for (hc = hap_serv_get_first_char((hap_serv_t *)hs); hc; hc = hap_char_get_next(hc)) {
        if (((__hap_char_t *)hc)->permission & HAP_CHAR_PERM_PR) {
            char_cnt++;
        }
    }
    if (char_cnt) {
        hap_read_data_t *read_arr = hap_platform_memory_calloc(char_cnt, sizeof(hap_read_data_t));
        if (!read_arr) {
            return HAP_FAIL;
        }

        hap_status_t *status_codes = hap_platform_memory_calloc(char_cnt, sizeof(hap_status_t));
        if (!status_codes) {
            hap_platform_memory_free(read_arr);
            return HAP_FAIL;
        }

        /* Create an array of characteristics to read, and then read them in one go */
        char_cnt = 0;
        for (hc = hap_serv_get_first_char((hap_serv_t *)hs); hc; hc = hap_char_get_next(hc)) {
            if (((__hap_char_t *)hc)->permission & HAP_CHAR_PERM_PR) {
                hap_char_set_owner_ctrl(hc, session_index);
                ((__hap_char_t *)hc)->update_called = false;
                read_arr[char_cnt].hc = hc;
                status_codes[char_cnt] = HAP_STATUS_SUCCESS;
                read_arr[char_cnt].status = &status_codes[char_cnt];
                char_cnt++;
            }
        }

        hs->bulk_read(&read_arr[0], char_cnt, hs->priv, NULL);
        hap_platform_memory_free(read_arr);
        hap_platform_memory_free(status_codes);
    }
    for (hc = hap_serv_get_first_char((hap_serv_t *)hs); hc; hc = hap_char_get_next(hc)) {
		hap_prepare_char_db((__hap_char_t *)hc, jptr, session_index);
	}

	json_gen_pop_array(jptr);
	json_gen_end_object(jptr);
	return HAP_SUCCESS;
}

static int hap_prepare_acc_db(__hap_acc_t *ha, json_gen_str_t *jptr, int session_index)
{
	json_gen_start_object(jptr);
	json_gen_obj_set_int(jptr, "aid", ha->aid);
	json_gen_push_array(jptr, "services");
	hap_serv_t *hs;
	for (hs = hap_acc_get_first_serv((hap_acc_t *)ha); hs; hs = hap_serv_get_next(hs)) {
		hap_prepare_serv_db((__hap_serv_t *)hs, jptr, session_index);
	}
	json_gen_pop_array(jptr);
	json_gen_end_object(jptr);
	return HAP_SUCCESS;
}

static int hap_prepare_json_database(char *buf, int bufsize, json_gen_flush_cb_t flush_cb, httpd_req_t *req)
{
    if (!req) {
        return HAP_FAIL;
    }
    hap_secure_session_t *session = (hap_secure_session_t *)hap_platform_httpd_get_sess_ctx(req);
    if (!session) {
        return HAP_FAIL;
    }
	json_gen_str_t jstr;
	json_gen_str_start(&jstr, buf, bufsize, flush_cb, req);
	json_gen_start_object(&jstr);
	json_gen_push_array(&jstr, "accessories");
	hap_acc_t *ha;
	for (ha = hap_get_first_acc(); ha; ha = hap_acc_get_next(ha)) {
		hap_prepare_acc_db((__hap_acc_t *)ha, &jstr, hap_get_ctrl_session_index(session));
	}
	json_gen_pop_array(&jstr);
	json_gen_end_object(&jstr);
	json_gen_str_end(&jstr);
	return HAP_SUCCESS;
}

static void hap_http_json_flush_chunk(char *data, void *priv)
{
    ESP_MFI_DEBUG_PLAIN("%s", data);
	httpd_resp_send_chunk((httpd_req_t *)priv, data, strlen(data));
}

static int hap_http_get_accessories(httpd_req_t *req)
{
	char buf[1000];
    ESP_MFI_DEBUG_PLAIN("Socket fd: %d; HTTP Request %s %s\n", httpd_req_to_sockfd(req), hap_platform_httpd_get_req_method(req), hap_platform_httpd_get_req_uri(req));
    hap_secure_session_t *session = (hap_secure_session_t *)hap_platform_httpd_get_sess_ctx(req);
    if (!hap_is_req_secure(session)) {
        return hap_http_session_not_authorized(req);
    }
	httpd_resp_set_type(req, "application/hap+json");
    ESP_MFI_DEBUG_PLAIN("Generating HTTP Response\n");
    /* Using chunked encoding since the response can be large, especially for bridges */
	hap_prepare_json_database(buf, sizeof(buf), hap_http_json_flush_chunk, req);
    /* This indicates the last chunk */
    httpd_resp_send_chunk(req, NULL, 0);
    ESP_MFI_DEBUG_PLAIN("\n");

    hap_report_event(HAP_EVENT_GET_ACC_COMPLETED, NULL, 0);
	return HAP_SUCCESS;
}
static struct httpd_uri hap_accessories = {
	.uri = "/accessories",
    .method = HTTP_GET,
    .handler = hap_http_get_accessories,
};

static void hap_set_char_report_status(bool *include_status, json_gen_str_t *jstr,
		int aid, int iid, int status)
{
	if (!*include_status) {
		json_gen_start_object(jstr);
		json_gen_push_array(jstr, "characteristics");
		*include_status = true;
	}
	json_gen_start_object(jstr);
	json_gen_obj_set_int(jstr, "aid", aid);
	json_gen_obj_set_int(jstr, "iid", iid);
	json_gen_obj_set_int(jstr, "status", status);
	json_gen_end_object(jstr);
}

static int hap_http_handle_set_char(jparse_ctx_t *jctx, char *outbuf, int buf_size,
		httpd_req_t *req)
{
	int cnt = 0, char_cnt = 0, i;
	bool include_status = false;
    uint64_t pid;
    bool valid_tw = false;
    bool req_tw = false;
	hap_secure_session_t *session = (hap_secure_session_t *)hap_platform_httpd_get_sess_ctx(req);
    if (!session)
        return HAP_FAIL;

    int64_t cur_time = esp_timer_get_time() / 1000;
    int64_t prepare_time = session->prepare_time;
    if (prepare_time) {
        /* If prepare time is non zero, it means that a prepare was received
         * before this request, and so this write needs to be timed write
         */
        req_tw = true;
        /* Reset prepare_time to 0, since a prepare is valid only for the immediate
         * following write
         */
        session->prepare_time = 0;
    }
    if (json_obj_get_int64(jctx, "pid", (int64_t *)&pid) == OS_SUCCESS) {
        /* If the pid value is present, this must be a timed write.
         * However, if there was no preceding prepare, the check below will
         * fail (as ttl will be 0) and appropriate error will be reported subsequently
         */
        req_tw = true;
        if ((pid == session->pid) && ((cur_time - prepare_time) <= session->ttl)) {
            valid_tw = true;
        }
    }
    /* Resetting the values so that the session is ready for next prepare or write */
    session->pid = 0;
    session->ttl = 0;

	json_obj_get_array(jctx, "characteristics", &cnt);
	if (cnt <= 0)
		return HAP_FAIL;

    hap_write_data_t *write_arr = hap_platform_memory_calloc(cnt, sizeof(hap_write_data_t));
	hap_status_t *status_arr = hap_platform_memory_calloc(cnt, sizeof(hap_status_t));
	if (!write_arr || !status_arr)
		goto set_char_end;

	json_gen_str_t jstr;
	json_gen_str_start(&jstr, outbuf, buf_size, hap_http_json_flush_chunk, req);
    /* Dummy get, so that the loop can start by leaving the previous
     * object and getting newer one
     */
    json_arr_get_object(jctx, 0);
	/* Loop through all characteristic objects {aid,iid,value}, handle
	 * errors if any, and if there are no errors, put the characteristic
	 * pointer and value in an array (with char_cnt)
	 */
	for (i = 0; i < cnt; i++) {
        /* Leave the previous object and get a newer one from the array */
        json_arr_leave_object(jctx);
		int aid = 0, iid = 0;
		json_arr_get_object(jctx, i);
		json_obj_get_int(jctx, "aid", &aid);
		json_obj_get_int(jctx, "iid", &iid);
		hap_acc_t *ha = hap_acc_get_by_aid(aid);
		__hap_char_t *hc = (__hap_char_t *)hap_acc_get_char_by_iid(ha, iid);
		if (!ha || !hc) {
			hap_set_char_report_status(&include_status, &jstr,
					aid, iid, HAP_STATUS_RES_ABSENT);
			continue;
		}

        /* If the previous request was a prepare, but the current
         * one was not a valid timed write, report error.
         */
        if (req_tw == true && valid_tw == false) {
            hap_set_char_report_status(&include_status, &jstr,
                    aid, iid, HAP_STATUS_VAL_INVALID);
            continue;
        }

        /* For characteristic that require a Mandatory Timed Write, return
         * error if this write is not a valid timed write
         */
        if (hc->permission & HAP_CHAR_PERM_TW) {
            if (valid_tw == false) {
                hap_set_char_report_status(&include_status, &jstr,
                        aid, iid, HAP_STATUS_VAL_INVALID);
                continue;
            }
        }

        /* Check if this write is just to enable/disable event notifications.
         * This is valid even for read-only characteristics that support event
         * notifications (like sensor readings), so we do not check the HAP_CHAR_PERM_PW
         * here.
         */
		bool ev;
		if (json_obj_get_bool(jctx, "ev", &ev) == HAP_SUCCESS) {
            if (hc->permission & HAP_CHAR_PERM_EV) {
                int index = hap_get_ctrl_session_index(session);
                hap_char_manage_notification((hap_char_t *)hc, index, ev);
                ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Events %s for aid=%d iid=%d",
                        ev ? "Enabled" : "Disabled", aid, iid);
            } else {
				hap_set_char_report_status(&include_status, &jstr,
						aid, iid, HAP_STATUS_NO_NOTIF);
			}
			continue;
		}

        /* Check if the characteristic has write permission */
		if (!(hc->permission & HAP_CHAR_PERM_PW)) {
			hap_set_char_report_status(&include_status, &jstr,
					aid, iid, HAP_STATUS_WR_ON_RDONLY);
			continue;
		}

        /* Check if the characteristic needs Authorization Data.
         * Actual authData value will be read later.
         */
        if (hc->permission & HAP_CHAR_PERM_AA) {
            int tmp_len;
            if (json_obj_get_strlen(jctx, "authData", &tmp_len) != HAP_SUCCESS) {
			hap_set_char_report_status(&include_status, &jstr,
					aid, iid, HAP_STATUS_INSUFFICIENT_AUTH);
			continue;
            }
        }
        /* If there is no write routine registered, there is no point of having
         * this write request. Return an error.
         */
		if (!((__hap_serv_t *)(hap_char_get_parent((hap_char_t *)hc)))->write_cb) {
			hap_set_char_report_status(&include_status, &jstr,
					aid, iid, HAP_STATUS_VAL_INVALID);
			continue;
		}

        hap_auth_data_t auth_data = {
            .data = NULL,
            .len = 0,
        };
		hap_val_t val;
		int json_ret = HAP_FAIL;
		switch (hc->format) {
			case HAP_CHAR_FORMAT_BOOL:
				json_ret = json_obj_get_bool(jctx, "value", &val.b);
				break;
			case HAP_CHAR_FORMAT_UINT8:
			case HAP_CHAR_FORMAT_UINT16:
			case HAP_CHAR_FORMAT_UINT32:
			case HAP_CHAR_FORMAT_INT:
				json_ret = json_obj_get_int(jctx, "value", &val.i);
				break;
			case HAP_CHAR_FORMAT_FLOAT:
				json_ret = json_obj_get_float(jctx, "value", &val.f);
				break;
			case HAP_CHAR_FORMAT_STRING: {
				int str_len = 0;
				json_ret = json_obj_get_strlen(jctx, "value", &str_len);
				if (json_ret == HAP_SUCCESS) {
                    /* Increment string length, for NULL termination byte */
                    str_len++;
                    val.s = hap_platform_memory_calloc(str_len, 1);
                    if (!val.s) {
                        hap_set_char_report_status(&include_status, &jstr,
                                aid, iid, HAP_STATUS_OO_RES);
                        continue;
                    }
                    json_obj_get_string(jctx, "value", val.s, str_len);
				}
				break;
			}
            case HAP_CHAR_FORMAT_DATA:
            case HAP_CHAR_FORMAT_TLV8: {
				int str_len = 0;
				json_ret = json_obj_get_strlen(jctx, "value", &str_len);
				if (json_ret == HAP_SUCCESS) {
					val.d.buf = hap_platform_memory_calloc(1, str_len + 1);
                    if (!val.d.buf) {
                        hap_set_char_report_status(&include_status, &jstr,
                                aid, iid, HAP_STATUS_OO_RES);
                        continue;
                    }
                    val.d.buflen = str_len + 1;
					json_obj_get_string(jctx, "value", (char *)val.d.buf, val.d.buflen);
                    if (esp_mfi_base64_decode((const char *)val.d.buf, strlen((char *)val.d.buf),
                                (char *)val.d.buf, val.d.buflen, (int *)&val.d.buflen) != 0) {
                        hap_platform_memory_free(val.d.buf);
                        hap_set_char_report_status(&include_status, &jstr,
                                aid, iid, HAP_STATUS_VAL_INVALID);
                        continue;
                    }
				}
				break;
			}
			default:
				json_ret = HAP_FAIL;
		}
		if (json_ret != HAP_SUCCESS) {
			hap_set_char_report_status(&include_status, &jstr,
					aid, iid, HAP_STATUS_VAL_INVALID);
			continue;
		}

        /* Check if the value is within constraints */
        if (hap_char_check_val_constraints(hc, &val) != HAP_SUCCESS) {
			hap_set_char_report_status(&include_status, &jstr,
					aid, iid, HAP_STATUS_VAL_INVALID);
			continue;
        }

        if (json_obj_get_strlen(jctx, "authData", &auth_data.len) == HAP_SUCCESS) {
            auth_data.data = hap_platform_memory_calloc(1, auth_data.len + 1);
            json_obj_get_string(jctx, "authData", (char *)auth_data.data, auth_data.len + 1);
            esp_mfi_base64_decode((const char *)auth_data.data, auth_data.len, (char *)auth_data.data, auth_data.len + 1, &auth_data.len);
        }
        bool remote = false;
        json_obj_get_bool(jctx, "remote", &remote);

        int index = hap_get_ctrl_session_index(session);
        hap_char_set_owner_ctrl((hap_char_t *)hc, index);
		/* No errors in the object data itself. Save the characteristic
		 * pointer and value, to be used later
		 */
		write_arr[char_cnt].hc = (hap_char_t *)hc;
		write_arr[char_cnt].val = val;
        write_arr[char_cnt].auth_data = auth_data;
        write_arr[char_cnt].remote = remote;
        write_arr[char_cnt].status = &status_arr[char_cnt];
		char_cnt++;
	}
	if (!char_cnt)
		goto set_char_end;

	/* The logic here is to loop through all the saved characteristic
	 * pointers, and invoke a single write callback for all consecutive
	 * characteristics of the same service.
	 * The write callback will be invoked if the service changes or
	 * if the last characteristic in the array is reached
	 */
	int hs_index = 0;
	bool write_err = false;
	__hap_serv_t *hs = (__hap_serv_t *)hap_char_get_parent(write_arr[0].hc);
	/* The counter here will go till char_cnt instead of char_cnt - 1.
	 * When i == char_cnt, it will mean that all elements in the array
	 * have been looped through.
	 * So, last iteration will invoke the write callback for the last
	 * set of characteritics.
	 */
	for (i = 0; i <= char_cnt; i++) {
		if ((i < char_cnt) && ((hap_serv_t *)hs == hap_char_get_parent(write_arr[i].hc)))
			continue;
		else {
			/* Passing the pointers to the first elements of the array
			 * for a given service (indicated by hs_index).
			 * Number of elements of the array are indicated by
			 * i - hs_index
			 */
			if (hs->write_cb(&write_arr[hs_index], i - hs_index,
					hs->priv, hap_platform_httpd_get_sess_ctx(req)) != HAP_SUCCESS)
				write_err = true;
			if (i < char_cnt) {
				hs = (__hap_serv_t *)hap_char_get_parent(write_arr[i].hc);
				hs_index = i;
			}
		}
	}
	if (write_err || include_status) {
		for (i = 0; i < char_cnt; i++) {
            /* TODO: The code to get aid looks complex. Simplify */
			hap_set_char_report_status(&include_status, &jstr,
				((__hap_acc_t *)hap_serv_get_parent(hap_char_get_parent(write_arr[i].hc)))->aid,
				((__hap_char_t *)(write_arr[i].hc))->iid, *write_arr[i].status);
		}
	}

	int ret = HAP_SUCCESS;
set_char_end:
	if (include_status) {
		json_gen_pop_array(&jstr);
		json_gen_end_object(&jstr);
		json_gen_str_end(&jstr);
		ret = HAP_FAIL;
	}

	if (write_arr) {
		for (i = 0; i < char_cnt; i++) {
			if (((__hap_char_t *)write_arr[i].hc)->format == HAP_CHAR_FORMAT_STRING) {
				if (write_arr[i].val.s) {
                    hap_platform_memory_free(write_arr[i].val.s);
                }
			} else if ((((__hap_char_t *)write_arr[i].hc)->format == HAP_CHAR_FORMAT_DATA) ||
                    (((__hap_char_t *)write_arr[i].hc)->format == HAP_CHAR_FORMAT_TLV8)) {
                    hap_platform_memory_free(write_arr[i].val.d.buf);
            }
            if (write_arr[i].auth_data.data) {
                hap_platform_memory_free(write_arr[i].auth_data.data);
            }
		}
	}
	if (write_arr)
		hap_platform_memory_free(write_arr);
	if (status_arr)
		hap_platform_memory_free(status_arr);
	return ret;
}

static int hap_http_put_characteristics(httpd_req_t *req)
{
    char stack_inbuf[512] = {0};
    char outbuf[512] = {0};
    char *heap_inbuf = NULL;
    char *inbuf = stack_inbuf;

    ESP_MFI_DEBUG_PLAIN("Socket fd: %d; HTTP Request %s %s\n", httpd_req_to_sockfd(req), hap_platform_httpd_get_req_method(req), hap_platform_httpd_get_req_uri(req));
    hap_secure_session_t *session = (hap_secure_session_t *)hap_platform_httpd_get_sess_ctx(req);
    if (!hap_is_req_secure(session)) {
        return hap_http_session_not_authorized(req);
    }

    /* If received content is larger than the buffer on stack, allocate one from heap.
     * This will mostly be required only in case of bridges, wherein there could be a request to
     * control all/many accessories at once.
     */
    int content_len = hap_platform_httpd_get_content_len(req);
    if (content_len > sizeof(stack_inbuf)) {
        heap_inbuf = hap_platform_memory_calloc(content_len + 1, 1); /* Allocating an extra byte for NULL termination */
        if (!heap_inbuf) {
            ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Failed to read HTTPD Data");
            httpd_resp_set_status(req, HTTPD_500);
            return httpd_resp_send(req, NULL, 0);
        }
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Allocated buffer of size %d for the large PUT",
                    content_len + 1)
        inbuf = heap_inbuf;
    }
	int data_len = hap_httpd_get_data(req, inbuf, content_len);
	if (data_len < 0) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Failed to read HTTPD Data");
		httpd_resp_set_status(req, HTTPD_500);
        if (heap_inbuf) {
            hap_platform_memory_free(heap_inbuf);
            ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Freed allocated buffer for PUT");
        }
		return httpd_resp_send(req, NULL, 0);
	}
    ESP_MFI_DEBUG_PLAIN("Data Received: %s\n", inbuf);
	jparse_ctx_t jctx;
	if (json_parse_start(&jctx, inbuf, data_len) != HAP_SUCCESS) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Failed to parse HTTPD JSON Data");
		httpd_resp_set_status(req, HTTPD_500);
        if (heap_inbuf) {
            hap_platform_memory_free(heap_inbuf);
            ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Freed allocated buffer for PUT");
        }
		return httpd_resp_send(req, NULL, 0);
	}

	httpd_resp_set_type(req, "application/hap+json");
	/* Setting response type to indicate error.
	 * This will be actually sent out only if there is some error
	 * to be reported while handling the characteristic writes.
	 * Else, the response type will be set to 204
	 */
	httpd_resp_set_status(req, HTTPD_207);
	if (hap_http_handle_set_char(&jctx, outbuf, sizeof(outbuf), req) == HAP_SUCCESS)
	{
		snprintf(outbuf, sizeof(outbuf), "HTTP/1.1 %s\r\n\r\n", HTTPD_204);
		httpd_send(req, outbuf, strlen(outbuf));
	} else {
        /* If a failure was encountered, it would mean that a response has been generated,
         * which will be chunk encoded. So, sending the last chunk here and also printing
         * a new line to end the prints of the error string.
         */
        httpd_resp_send_chunk(req, NULL, 0);
        ESP_MFI_DEBUG_PLAIN("\n");
    }
    json_parse_end(&jctx);

    if (heap_inbuf) {
        hap_platform_memory_free(heap_inbuf);
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Freed allocated buffer for PUT");
    }

    hap_report_event(HAP_EVENT_SET_CHAR_COMPLETED, NULL, 0);
    return HAP_SUCCESS;
}

static bool hap_get_bool_url_param(const char *query_str, const char *key)
{
	char val[6]; /* Max string will be "false" */
	if (httpd_query_key_value(query_str, key, val, sizeof(val)) == HAP_SUCCESS) {
		if (!strcmp(val, "true") || !strcmp(val, "1"))
			return true;
	}
	return false;
}

static int hap_http_get_characteristics(httpd_req_t *req)
{
    char outbuf[512];
    char stack_val_buf[512] = {0};
    char *heap_val_buf = NULL;
    char *val = stack_val_buf;

    ESP_MFI_DEBUG_PLAIN("Socket fd: %d; HTTP Request %s %s\n", httpd_req_to_sockfd(req), hap_platform_httpd_get_req_method(req), hap_platform_httpd_get_req_uri(req));

    hap_secure_session_t *session = (hap_secure_session_t *)hap_platform_httpd_get_sess_ctx(req);
    if (!hap_is_req_secure(session)) {
        return hap_http_session_not_authorized(req);
    }
    const char *uri = hap_platform_httpd_get_req_uri(req);
    /* Allocate on heap, if URI is longer */
    if (strlen(uri) > sizeof(stack_val_buf)) {
        heap_val_buf = hap_platform_memory_calloc(strlen(uri) + 1, 1); /* Allocating an extra byte for NULL termination */
        if (!heap_val_buf) {
            ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Failed to read URL");
            httpd_resp_set_status(req, HTTPD_500);
            return httpd_resp_send(req, NULL, 0);
        }
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Allocated buffer of size %d for the large GET",
                    strlen(uri) + 1)
        val = heap_val_buf;
    }
    size_t url_query_str_len = httpd_req_get_url_query_len(req);
    char * url_query_str = hap_platform_memory_calloc(1, url_query_str_len + 1);
    if (!url_query_str) {
		httpd_resp_set_status(req, HTTPD_400);
		httpd_resp_set_type(req, "application/hap+json");
		snprintf(outbuf, sizeof(outbuf),"{\"status\":-70409}");
		httpd_resp_send(req, outbuf, strlen(outbuf));
        goto get_char_return;
    }
    httpd_req_get_url_query_str(req, url_query_str, url_query_str_len + 1);
	/* Check for all the optional URL query paramaters */
	bool meta = hap_get_bool_url_param(url_query_str, "meta");
	bool perms = hap_get_bool_url_param(url_query_str, "perms");
	bool type = hap_get_bool_url_param(url_query_str, "type");
	bool ev = hap_get_bool_url_param(url_query_str, "ev");

	/* Check for the mandatory "id" URL query parameter.
	 * If not found, return success
	 */
	if (httpd_query_key_value(url_query_str, "id", val, strlen(uri) + 1) != HAP_SUCCESS) {
		httpd_resp_set_status(req, HTTPD_400);
		httpd_resp_set_type(req, "application/hap+json");
		snprintf(outbuf, sizeof(outbuf),"{\"status\":-70409}");
		httpd_resp_send(req, outbuf, strlen(outbuf));
        goto get_char_return;
    }

	/* Get the total count of characteristics in the request. This will be
	 * required to decide the size of the characteristic pointers' array
	 */
	int char_cnt = 0;
	char *val_ptr = val;
	/* We do not do any error checking here and just assume that the id
	 * tag has a characteristic list which is a comma separated list
	 * of <aid>.<iid> elements
	 */
	char *p = strsep(&val_ptr, ",");
	while (p) {
		char_cnt++;
		p = strsep(&val_ptr, ",");
	}

	/* Normally, it would have been fine to just go on parsing the
	 * characteristics in the URL, fetch their values and prepare
	 * the response. However, if there is error for any characteristic
	 * a "status" field needs to be added for all characteristics.
	 *
	 * So, it is better to maintain a list of characteristics pointers,
	 * read all the values, and only then create the response
	 */
	hap_read_data_t *read_arr = hap_platform_memory_calloc(char_cnt, sizeof(hap_read_data_t));
    if (!read_arr) {
		httpd_resp_set_status(req, HTTPD_500);
		httpd_resp_set_type(req, "application/hap+json");
		snprintf(outbuf, sizeof(outbuf),"{\"status\":-70407}");
		httpd_resp_send(req, outbuf, strlen(outbuf));
        goto get_char_return;
    }
    hap_status_t *status_codes = hap_platform_memory_calloc(char_cnt, sizeof(hap_status_t));
    if (!status_codes) {
        hap_platform_memory_free(read_arr);
		httpd_resp_set_status(req, HTTPD_500);
		httpd_resp_set_type(req, "application/hap+json");
		snprintf(outbuf, sizeof(outbuf),"{\"status\":-70407}");
		httpd_resp_send(req, outbuf, strlen(outbuf));
        goto get_char_return;
    }

    ESP_MFI_DEBUG_PLAIN("Generating HTTP Response\n");
	/* Generate the JSON response */
	bool include_status = 0;
	httpd_resp_set_status(req, HTTPD_207);
	httpd_resp_set_type(req, "application/hap+json");
	json_gen_str_t jstr;
	json_gen_str_start(&jstr, outbuf, sizeof(outbuf), hap_http_json_flush_chunk, req);

	/* Get the ids once again. Not checking for success since that
	 * would be redundant
	 */
	httpd_query_key_value(url_query_str, "id", val, strlen(uri) + 1);
    char_cnt = 0;
	int aid, iid;
	val_ptr = val;
	/* Parse the AIDs and IIDs in the "id" field and fetch the
	 * characteristic pointer for each
	 */
	p = strsep(&val_ptr, ".");
	while (p) {
		aid = atoi(p);
		p = strsep(&val_ptr, ",");
		iid = atoi(p);
		p = strsep(&val_ptr, ".");
		hap_char_t *hc = hap_acc_get_char_by_iid(hap_acc_get_by_aid(aid), iid);
		if (!hc) {
			hap_set_char_report_status(&include_status, &jstr,
					aid, iid, HAP_STATUS_RES_ABSENT);
			continue;
		}
		if (!(((__hap_char_t *)hc)->permission & HAP_CHAR_PERM_PR)) {
			hap_set_char_report_status(&include_status, &jstr,
					aid, iid, HAP_STATUS_RD_ON_WRONLY);
			continue;
		}
        hap_char_set_owner_ctrl(hc, hap_get_ctrl_session_index(session));
        ((__hap_char_t *)hc)->update_called = false;
        /* Add the characteristic to the read array */
		read_arr[char_cnt].hc = hc;
        status_codes[char_cnt] = HAP_STATUS_SUCCESS;
        read_arr[char_cnt].status = &status_codes[char_cnt];
		char_cnt++;
	}

    if (!char_cnt) {
        goto get_char_end;
    }


	int hs_index = 0;
	bool read_err = false;
	__hap_serv_t *hs = (__hap_serv_t *)hap_char_get_parent(read_arr[0].hc);
    /* Read all the values first, before preparing the response, so that it
     * would be known in advance, if any read error is encountered
     */
    int i;
	/* The counter here will go till char_cnt instead of char_cnt - 1.
	 * When i == char_cnt, it will mean that all elements in the array
	 * have been looped through.
	 * So, last iteration will invoke the read callback for the last
	 * set of characteritics.
	 */
	for (i = 0; i <= char_cnt; i++) {
		if ((i < char_cnt) && ((hap_serv_t *)hs == hap_char_get_parent(read_arr[i].hc)))
			continue;
		else {
			/* Passing the pointers to the first elements of the array
			 * for a given service (indicated by hs_index).
			 * Number of elements of the array are indicated by
			 * i - hs_index
			 */
			if (hs->bulk_read(&read_arr[hs_index], i - hs_index, hs->priv, hap_platform_httpd_get_sess_ctx(req)) != HAP_SUCCESS)
				read_err = true;
			if (i < char_cnt) {
				hs = (__hap_serv_t *)hap_char_get_parent(read_arr[i].hc);
				hs_index = i;
			}
		}
	}
    if (!include_status) {
        if (!read_err) {
            /* If "include_status" is false, it means there
             * were no errors.
             * So, set response type to 200 OK
             */
            httpd_resp_set_status(req, HTTPD_200);
        }
        json_gen_start_object(&jstr);
        json_gen_push_array(&jstr, "characteristics");
    }
	/* Loop through the characteristics and include their data
	 */
	for (i = 0; i < char_cnt; i++) {
		__hap_char_t *hc = (__hap_char_t *)read_arr[i].hc;
        /* If the Update API has not been called from the service read routine,
         * reset the owner controller value.
         * Else, the controller will  miss the next notification.
         */
        if (!hc->update_called) {
            hc->owner_ctrl = 0;
        }
        hc->update_called = false;

		json_gen_start_object(&jstr);
		__hap_acc_t *ha = (__hap_acc_t *)hap_serv_get_parent(hc->parent);
		json_gen_obj_set_int(&jstr, "aid", ha->aid);
		json_gen_obj_set_int(&jstr, "iid", hc->iid);

        if (hc->permission & HAP_CHAR_PERM_SPECIAL_READ) {
            json_gen_obj_set_null(&jstr, "value");
        } else {
            /* Include "value" only if status is SUCCESS */
            if (*read_arr[i].status == HAP_STATUS_SUCCESS) {
                hap_add_char_val_json(hc->format, "value", &hc->val, &jstr);
            }
        }
		/* Include status only if it was already included because of
		 * some parsing errors, or if an error was encountered while
         * actually reading the characteristics.
		 */
		if (include_status || read_err) {
			json_gen_obj_set_int(&jstr, "status", *read_arr[i].status);
		}
		if (type)
			hap_add_char_type(hc, &jstr);
		if (perms)
			hap_add_char_perms(hc, &jstr);
		if (ev) {
			hap_add_char_ev(hc, &jstr, hap_get_ctrl_session_index(session));
        }
		if (meta)
			hap_add_char_meta(hc, &jstr);
		json_gen_end_object(&jstr);
	}
get_char_end:
	json_gen_pop_array(&jstr);
	json_gen_end_object(&jstr);
	json_gen_str_end(&jstr);

	hap_platform_memory_free(read_arr);
    hap_platform_memory_free(status_codes);

    /* This indicates the last chunk */
    httpd_resp_send_chunk(req, NULL, 0);
    ESP_MFI_DEBUG_PLAIN("\n");
get_char_return:
    if (heap_val_buf) {
        hap_platform_memory_free(heap_val_buf);
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Freed allocated buffer for GET");
    }

    if (url_query_str) {
        hap_platform_memory_free(url_query_str);
    }

    hap_report_event(HAP_EVENT_GET_CHAR_COMPLETED, NULL, 0);
	return HAP_SUCCESS;
}
static struct httpd_uri hap_characteristics_get = {
	.uri = "/characteristics",
    .method = HTTP_GET,
    .handler = hap_http_get_characteristics,
};
static struct httpd_uri hap_characteristics_put = {
	.uri = "/characteristics",
    .method = HTTP_PUT,
    .handler = hap_http_put_characteristics,
};

static int hap_http_pairings_handler(httpd_req_t *req)
{
	uint8_t buf[2048]; /* Large buffer to accommodate 16 pairings list */
	void *ctx = hap_platform_httpd_get_sess_ctx(req);
    ESP_MFI_DEBUG_PLAIN("Socket fd: %d; HTTP Request %s %s\n", httpd_req_to_sockfd(req), hap_platform_httpd_get_req_method(req), hap_platform_httpd_get_req_uri(req));
	int data_len = httpd_req_recv(req, (char *)buf, sizeof(buf));
	int outlen;
    hap_secure_session_t *session = (hap_secure_session_t *)ctx;
    if (!hap_is_req_secure(session)) {
        /* Only setting the HTTP status here. Actual error TLV will be added
         * by the hap_pairings_process() call
         */
        httpd_resp_set_status(req, "470 Connection Authorization Required");
    }
	hap_pairings_process(ctx, buf, data_len, sizeof(buf), &outlen);
	httpd_resp_set_type(req, "application/pairing+tlv8");
	return httpd_resp_send(req, (char *)buf, outlen);
}
static struct httpd_uri hap_pairings = {
	.uri = "/pairings",
    .method = HTTP_POST,
    .handler = hap_http_pairings_handler,
};

static int hap_http_post_identify(httpd_req_t *req)
{
	char buf[100];
    ESP_MFI_DEBUG_PLAIN("Socket fd: %d\nHTTP Request %s %s\n", httpd_req_to_sockfd(req), hap_platform_httpd_get_req_method(req), hap_platform_httpd_get_req_uri(req));
	if (is_accessory_paired()) {
		httpd_resp_set_status(req, HTTPD_400);
		httpd_resp_set_type(req, "application/hap+json");
		snprintf(buf, sizeof(buf),"{\"status\":-70401}");
		httpd_resp_send(req, buf, strlen(buf));
	} else {
		hap_acc_t *ha = hap_get_first_acc();
        __hap_acc_t *_ha = (__hap_acc_t *)ha;
		_ha->identify_routine(ha);
		snprintf(buf, sizeof(buf), "HTTP/1.1 %s\r\n\r\n", HTTPD_204);
		httpd_send(req, buf, strlen(buf));
	}
	return HAP_SUCCESS;
}

static struct httpd_uri hap_identify = {
	.uri = "/identify",
    .method = HTTP_POST,
    .handler = hap_http_post_identify,
};

static int hap_http_put_prepare(httpd_req_t *req)
{
    char buf[512] = {0};

    ESP_MFI_DEBUG_PLAIN("Socket fd: %d; HTTP Request %s %s\n", httpd_req_to_sockfd(req), hap_platform_httpd_get_req_method(req), hap_platform_httpd_get_req_uri(req));
    hap_secure_session_t *session = (hap_secure_session_t *)hap_platform_httpd_get_sess_ctx(req);
    if (!hap_is_req_secure(session)) {
        return hap_http_session_not_authorized(req);
    }
	int data_len = httpd_req_recv(req, (char *)buf, sizeof(buf));
	if (data_len < 0) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Failed to read HTTPD Data");
		httpd_resp_set_status(req, HTTPD_500);
		return httpd_resp_send(req, NULL, 0);
	}
    ESP_MFI_DEBUG_PLAIN("Data Received: %s\n", buf);
	jparse_ctx_t jctx;
	if (json_parse_start(&jctx, buf, data_len) != HAP_SUCCESS) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Failed to parse HTTPD JSON Data");
		httpd_resp_set_status(req, HTTPD_500);
		return httpd_resp_send(req, NULL, 0);
	}

	httpd_resp_set_type(req, "application/hap+json");
    uint64_t pid;
    int64_t ttl;
    if ((json_obj_get_int64(&jctx, "pid", (int64_t *)&pid) != OS_SUCCESS) ||
        (json_obj_get_int64(&jctx, "ttl", &ttl) != OS_SUCCESS)) {
		snprintf(buf, sizeof(buf),"{\"status\":-70410}");
    } else {
        session->pid = pid;
        session->ttl = ttl;
        session->prepare_time = esp_timer_get_time() / 1000; /* Set current time in msec */
        snprintf(buf, sizeof(buf),"{\"status\":0}");
    }
    json_parse_end(&jctx);
    httpd_resp_send(req, buf, strlen(buf));
    return HAP_SUCCESS;
}

static struct httpd_uri hap_prepare = {
	.uri = "/prepare",
    .method = HTTP_PUT,
    .handler = hap_http_put_prepare,
};

static void hap_send_notification(void *arg)
{
    int num_char = hap_priv.cfg.max_event_notif_chars;
    hap_char_t *hc;
    hap_char_t **char_arr = hap_platform_memory_calloc(num_char, sizeof(hap_char_t *));

    if (!char_arr) {
        return;
    }

    int i, num_notif_chars;
    for (i = 0; i < num_char; i++) {
        hc = hap_get_pending_notif_char();
        if (hc) {
            char_arr[i] = hc;
        } else {
            break;
        }
    }
    /* If no characteristic notifications are pending, free char_arr and exit */ 
    if (i == 0) {
	hap_platform_memory_free(char_arr); 
        return;
    }
    num_notif_chars = i;
	hap_secure_session_t *session;
    /* Flag to indicate if any controller was connected */
    bool ctrl_connected = false;
	char buf[250];
	for (i = 0; i < HAP_MAX_SESSIONS; i++) {
		session = hap_priv.sessions[i];
		if (!session)
			continue;
        ctrl_connected = true;
		int fd = session->conn_identifier;
#define HTTPD_HDR_STR      "EVENT/1.0 200 OK\r\n"                   \
		"Content-Type: application/hap+json\r\n"           \
		"Content-Length: %d\r\n"
		char notif_json[1024];
		json_gen_str_t jstr;
		json_gen_str_start(&jstr, notif_json, sizeof(notif_json), NULL, NULL);
		json_gen_start_object(&jstr);
		json_gen_push_array(&jstr, "characteristics");

        int j;
        bool notif_to_send = false;
        for (j = 0; j < num_notif_chars; j++) {
            hc = char_arr[j];
            __hap_char_t *_hc = ( __hap_char_t *)hc;
            /* If the controller is the owner, dont send notification to it */
            if (hap_char_is_ctrl_owner(hc, i)) {
                /* Since there can be only one owner, which we are anyways skipping,
                 * we can reset owner value to 0
                 */
                _hc->owner_ctrl = 0;
                continue;
            }
            if (!hap_char_is_ctrl_subscribed(hc, i))
                continue;

            json_gen_start_object(&jstr);
            hap_acc_t *ha = hap_serv_get_parent(hap_char_get_parent(hc));
            int aid = ((__hap_acc_t *)ha)->aid;
            json_gen_obj_set_int(&jstr, "aid", aid);
            json_gen_obj_set_int(&jstr, "iid", _hc->iid);
            hap_add_char_val_json(_hc->format, "value", &_hc->val, &jstr);
            json_gen_end_object(&jstr);
            notif_to_send = true;
        }
        if (!notif_to_send) {
            /* No notification required for this controller. Just continue */
            continue;
        }

        json_gen_pop_array(&jstr);
		json_gen_end_object(&jstr);
		json_gen_str_end(&jstr);

		snprintf(buf, sizeof(buf), HTTPD_HDR_STR,
				strlen(notif_json));
		hap_httpd_send(hap_priv.server, fd, buf, strlen(buf), 0);
		/* Space for sending additional headers based on set_header */
		hap_httpd_send(hap_priv.server, fd, "\r\n", strlen("\r\n"), 0);
		hap_httpd_send(hap_priv.server, fd, notif_json, strlen(notif_json), 0);
        httpd_sess_update_lru_counter(hap_priv.server, fd);
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Notification Sent");
        ESP_MFI_DEBUG_PLAIN("Socket fd: %d; Event message: %s\n", fd, notif_json);
	}
    /* If no controller was connected and no disconnected event was sent,
     * reannaounce mDNS. That will increment state number as required
     * by HAP Spec R15.
     */
    if (!ctrl_connected && !hap_priv.disconnected_event_sent) {
        hap_mdns_announce(false);
        hap_priv.disconnected_event_sent = true;
    }
    hap_platform_memory_free(char_arr);
}

void hap_http_debug_enable()
{
    http_debug = true;
}

void hap_http_debug_disable()
{
    http_debug = false;
}

void hap_http_send_notif()
{
	httpd_queue_work(hap_priv.server, hap_send_notification, NULL);
}

static bool hap_http_registered;
int hap_register_http_handlers()
{
    if (!hap_http_registered) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Registering HomeKit web handlers");
        httpd_register_uri_handler(hap_priv.server, &hap_pair_setup);
        httpd_register_uri_handler(hap_priv.server, &hap_pair_verify);
        httpd_register_uri_handler(hap_priv.server, &hap_pairings);
        httpd_register_uri_handler(hap_priv.server, &hap_accessories);
        httpd_register_uri_handler(hap_priv.server, &hap_characteristics_get);
        httpd_register_uri_handler(hap_priv.server, &hap_characteristics_put);
        httpd_register_uri_handler(hap_priv.server, &hap_identify);
        httpd_register_uri_handler(hap_priv.server, &hap_prepare);
        if (hap_priv.features & HAP_FF_SW_TOKEN_AUTH) {
            hap_register_secure_message_handler(hap_priv.server);
        }
    }
    hap_http_registered = true;
    return HAP_SUCCESS;
}

int hap_unregister_http_handlers()
{
    if (hap_http_registered) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Unegistering HomeKit web handlers");
        httpd_unregister_uri_handler(hap_priv.server, "/pair-setup", HTTP_POST);
        httpd_unregister_uri_handler(hap_priv.server, "/pair-verify", HTTP_POST);
        httpd_unregister_uri_handler(hap_priv.server, "/pairings", HTTP_POST);
        httpd_unregister_uri_handler(hap_priv.server, "/accessories", HTTP_GET);
        httpd_unregister_uri_handler(hap_priv.server, "/characteristics", HTTP_GET);
        httpd_unregister_uri_handler(hap_priv.server, "/characteristics", HTTP_PUT);
        httpd_unregister_uri_handler(hap_priv.server, "/identify", HTTP_POST);
        httpd_unregister_uri_handler(hap_priv.server, "/prepare", HTTP_PUT);
        if (hap_priv.features & HAP_FF_SW_TOKEN_AUTH) {
            hap_unregister_secure_message_handler(hap_priv.server);
        }
    }
    hap_http_registered = false;
    return HAP_SUCCESS;
}

int hap_httpd_start(void)
{
    if (hap_platform_httpd_start(&hap_priv.server) == ESP_OK) {
        return HAP_SUCCESS;
    }
    return HAP_FAIL;
}

httpd_handle_t * hap_httpd_get_handle()
{
    return &hap_priv.server;
}
static bool first_announce_done;

int hap_mdns_deannounce(void)
{
    int ret = HAP_SUCCESS;
    if (first_announce_done) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Deannouncing _hap._tcp mDNS service");
        ret = hap_mdns_serv_stop(&hap_priv.hap_mdns_handle);
        if (ret == HAP_SUCCESS) {
            /* Wait for some time for the packets to go out on network */
            vTaskDelay(2000 / hap_platform_os_get_msec_per_tick());
            first_announce_done = false;
        }
    }
    return ret;
}

int hap_mdns_announce(bool first)
{
    /* If the API is called with the "first" argument as true, Force announce the service,
     * rather than just sending a re-announce packet
     */
    if (first) {
        first_announce_done = false;
    }
    static char config_num[6]; /* Max value can be 65535 */
    static char state_num[6]; /* Max value can be 65535 */
    static char ff[4];
    static char sf[4];
    static char ci[4];

    mdns_txt_item_t txt[9];
    int i = 0;

    snprintf(config_num, sizeof(config_num), "%d", hap_priv.config_num);
    txt[i].key = "c#";
    txt[i++].value = config_num;


    uint8_t features = 0;
    /* Either hardware authentication, or software authentication
     * can be enabled at a time.
     */
    if (hap_priv.features & HAP_FF_HARDWARE_AUTH) {
        features |= HAP_FF_HARDWARE_AUTH;
    } else if (hap_priv.features & HAP_FF_SW_TOKEN_AUTH) {
        features |= HAP_FF_SW_TOKEN_AUTH;
    }
    snprintf(ff, sizeof(ff), "%d", features);
    txt[i].key = "ff";
    txt[i++].value = ff;

    txt[i].key = "id";
    txt[i++].value = hap_priv.acc_id;

    txt[i].key = "md";
    txt[i++].value = hap_priv.primary_acc.model;

    txt[i].key = "pv";
    txt[i++].value = "1.1"; /* As per HAP Spec R10 */

    if (first_announce_done) {
        /* If first announcement was already done, this is a republish.
         * Update the state number, since HAP Spec R15 requires that any Bonjour republish
         * should update state number.
         */
        if (is_accessory_paired()) {
            hap_increment_and_save_state_num();
        }
    }
    snprintf(state_num, sizeof(state_num), "%u", hap_priv.state_num);
    txt[i].key = "s#";
    txt[i++].value = state_num;

    uint8_t status_flags = is_accessory_paired() ? 0 : HAP_SF_ACC_UNPAIRED;
    if (!hap_is_network_configured())
        status_flags |= HAP_SF_ACC_UNCONFIGURED;
    snprintf(sf, sizeof(sf), "%d", status_flags);
    txt[i].key = "sf";
    txt[i++].value = sf;

    snprintf(ci, sizeof(ci), "%d", hap_priv.cid);
    txt[i].key = "ci";
    txt[i++].value = ci;

    txt[i].key = "sh";
    txt[i++].value = hap_priv.setup_hash_str;

    int ret;
    /* If first announce is not done, the service will be added instead of just updating.
     * Else, first add the service, instead of just updating.
     */
    if (!first_announce_done) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Announcing _hap._tcp mDNS service");
        ret = hap_mdns_serv_start(&hap_priv.hap_mdns_handle,
            hap_priv.primary_acc.name, "_hap", "_tcp", hap_platform_httpd_get_port(), txt, i);
        first_announce_done = true;
    } else {
        /* Else, just update TXT records. Not add new service.*/
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Re-announcing _hap._tcp mDNS service");
        ret = hap_mdns_serv_update_txt(&hap_priv.hap_mdns_handle, txt, i);
    }
    if (ret != 0) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Failed to announce _hap mDNS service");
        return HAP_FAIL;
    }
    return HAP_SUCCESS;
}

int hap_ip_services_start()
{
    static bool hap_ip_services_started;
    if (hap_ip_services_started) {
        return HAP_SUCCESS;
    }
    hap_register_http_handlers();
    if (hap_mdns_announce(false) != HAP_SUCCESS) {
        hap_unregister_http_handlers();
        return HAP_FAIL;
    }
    hap_ip_services_started = true;
    return HAP_SUCCESS;
}
