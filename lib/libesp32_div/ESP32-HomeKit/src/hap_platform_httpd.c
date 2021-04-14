/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2019 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
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
#include <esp_http_server.h>
#include <_esp_hap_config.h>

httpd_handle_t *int_handle;
int hap_platform_httpd_start(httpd_handle_t *handle)
{
    httpd_config_t config = {
        .task_priority  = tskIDLE_PRIORITY+5,
        .stack_size         = CONFIG_HAP_HTTP_STACK_SIZE,
        .server_port        = CONFIG_HAP_HTTP_SERVER_PORT,
        .ctrl_port          = CONFIG_HAP_HTTP_CONTROL_PORT,
        .max_open_sockets   = CONFIG_HAP_HTTP_MAX_OPEN_SOCKETS,
        .max_uri_handlers   = CONFIG_HAP_HTTP_MAX_URI_HANDLERS,
        .max_resp_headers   = 8,
        .backlog_conn       = 5,
        .lru_purge_enable   = true,
        .recv_wait_timeout  = 5,
        .send_wait_timeout  = 5,
    };
    esp_err_t err =  httpd_start(handle, &config);
    if (err == ESP_OK) {
        int_handle = handle;
    }
    return err;
}

int hap_platform_httpd_stop(httpd_handle_t *handle)
{
    esp_err_t err = httpd_stop(*handle);
    if (err == ESP_OK) {
        int_handle = NULL;
    }
    return err;
}

int hap_platform_httpd_get_port()
{
    return CONFIG_HAP_HTTP_SERVER_PORT;
}

void * hap_platform_httpd_get_sess_ctx(httpd_req_t *req)
{
    if (req) {
        return req->sess_ctx;
    }
    return NULL;
}

esp_err_t hap_platform_httpd_set_sess_ctx(httpd_req_t *req, void *ctx, httpd_free_ctx_fn_t free_ctx, bool ignore_ctx_changes)
{
    if (req) {
        req->sess_ctx = ctx;
        req->free_ctx =  free_ctx;
#ifndef CONFIG_IDF_TARGET_ESP8266
        req->ignore_sess_ctx_changes = ignore_ctx_changes;
#endif
        return ESP_OK;
    }
    return ESP_FAIL;
}

static char * hap_platform_httpd_rqtype_to_string(int method)
{
    switch (method) {
        case HTTP_GET:
            return "GET";
        case HTTP_POST:
            return "POST";
        case HTTP_PUT:
            return "PUT";
        default:
            return "INVALID";
    }
}

const char *hap_platform_httpd_get_req_method(httpd_req_t *req)
{
    if (req) {
        return hap_platform_httpd_rqtype_to_string(req->method);
    }
    return NULL;
}

const char *hap_platform_httpd_get_req_uri(httpd_req_t *req)
{
    if (req) {
        return req->uri;
    }
    return NULL;
}

int hap_platform_httpd_get_content_len(httpd_req_t *req)
{
    if (req) {
        return req->content_len;
    }
    return -1;
}

httpd_handle_t *hap_platform_httpd_get_handle()
{
    return int_handle;
}
