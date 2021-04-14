
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
#ifndef _HAP_PLATFORM_HTTPD_H_
#define _HAP_PLATFORM_HTTPD_H_
#include <esp_http_server.h>
#ifdef __cplusplus
extern "C" {
#endif

/** Start the webserver
 *
 * This API will be called by the HAP Core to start the webserver.
 *
 * @param[out] handle Pointer to the handle that should be populated by this
 * function on a success.
 *
 * @return ESP_OK on success
 * @return error code otherwise
 */
int hap_platform_httpd_start(httpd_handle_t *handle);

/** Stop the webserver
 *
 * This API will be called by the HAP Core to stop the webserver.
 *
 * @param[in] handle Pointer to the handle created in hap_platform_httpd_start
 *
 * @return ESP_OK on success
 * @return error code otherwise
 */
int hap_platform_httpd_stop(httpd_handle_t *handle);

/** Get the current HTTP Port
 *
 * This API will be called by the HAP Core to get the HTTP Port being used. This will
 * be used for the mDNS announcement
 *
 * @return Configured HTTP Port
 */
int hap_platform_httpd_get_port();

/** Get the HTTPD session context
 *
 * This API will be called by the HAP Core to get the context associated with a given
 * session. It is generally used to maintain pairing information.
 *
 * @param[in] req HTTPD Request structure.
 *
 * @return pointer to the session context.
 */
void * hap_platform_httpd_get_sess_ctx(httpd_req_t *req);

/** Set the HTTPD session context
 *
 * This API will be called by the HAP Core to set the context associated with a given
 * session. It is generally used to maintain pairing information.
 *
 * @param[in] req HTTPD Request structure.
 * @param[in] ctx The context to be set for the given session.
 * @param[in] free_fn Pointer to a function that will be used by the HTTP Server to clear the context when the session closes
 * @param[in] ignore_ctx_changes Flag to indicate if the HTTP Server should ignore changes to the context across different requests.
 * If set to false, the server will try clearing the old context, if it detects any change.
 *
 * @return ESP_OK in success
 * @return ESP_FAIL if req is nULL
 */
esp_err_t hap_platform_httpd_set_sess_ctx(httpd_req_t *req, void *ctx, httpd_free_ctx_fn_t free_ctx, bool ignore_ctx_changes);

/** Get the HTTP request method
 *
 * This API will be called by the HAP Core to get the HTTP request method
 * in string format (Eg. GET, POST, etc.)
 *
 * @param[in] HTTPD Request structure.
 *
 * @return Pointer to a string indicating the HTTP method.
 */
const char *hap_platform_httpd_get_req_method(httpd_req_t *req);

/** Get the HTTP request URI
 *
 * This API will be called by the HAP Core to get the request URI string.
 *
 * @param[in] HTTPD Request structure.
 *
 * @return Pointer to the URI string.
 */
const char *hap_platform_httpd_get_req_uri(httpd_req_t *req);

/** Get the HTTP request content length
 *
 * This API will be called by the HAP Core to get the content length of the request.
 *
 * @param[in] HTTPD Request structure.
 *
 * @return Content length.
 */
int hap_platform_httpd_get_content_len(httpd_req_t *req);

/**
 * Get the HAP HTTPD Handle
 *
 * If an application wants to register additional HTTPD endpoints on the same server
 * instance being used by HomeKit, this API can be used to get the handle. This
 * should be used only after hap_start().
 *
 * @return HomeKit HTTPD Handle
 */
httpd_handle_t *hap_platform_httpd_get_handle();

#ifdef __cplusplus
}
#endif
#endif /* _HAP_PLATFORM_HTTPD_H_ */
