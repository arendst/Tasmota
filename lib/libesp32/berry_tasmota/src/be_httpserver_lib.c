/*
  be_httpserver_lib.c - HTTP server support for Berry using ESP-IDF HTTP server

  Copyright (C) 2025 Jonathan E. Peace

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef USE_BERRY_HTTPSERVER

#include <stddef.h>    // For NULL, size_t
#include <stdbool.h>   // For bool, true, false
#include <string.h>    // For string functions
#include <stdlib.h>    // For malloc/free

// ESP-IDF includes
#define LOG_LOCAL_LEVEL ESP_LOG_INFO
#include "esp_log.h"
#include "esp_http_server.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

// Berry includes
#include "be_mapping.h"
#include "be_exec.h" 
#include "be_vm.h"
#include "be_gc.h"

// External function declarations
httpd_handle_t be_httpserver_get_handle(void);
void be_httpserver_set_disconnect_handler(httpd_close_func_t handler);
bool httpserver_has_queue(void);

// Message types for queue
typedef enum {
    HTTP_MSG_WEBSOCKET = 1,
    HTTP_MSG_FILE = 2,
    HTTP_MSG_WEB = 3
} http_msg_type_t;

// Message structure for queue
typedef struct {
    http_msg_type_t type;
    int client_id;
    void *data;
    size_t data_len;
    void *user_data;
    bvalue func;  // Berry function value for web handlers
    httpd_req_t *req;  // HTTP request handle for async processing
} http_queue_msg_t;

// Forward declarations for internal functions
void be_httpserver_process_websocket_msg(bvm *vm, int client_id, const char *data, size_t len);
bool httpserver_queue_message(http_msg_type_t type, int client_id, const void *data, size_t data_len, void *user_data);
bool httpserver_queue_web_request(int handler_id, httpd_req_t *req, bvalue func);

// Logger tag
static const char *TAG = "HTTPSERVER";

// Global queue for handling messages in the main task context
static QueueHandle_t http_msg_queue = NULL;
static SemaphoreHandle_t http_queue_mutex = NULL;
static bool http_queue_initialized = false;

// Maximum number of HTTP handlers
#define HTTP_HANDLER_MAX 5

// Handler storage
typedef struct {
    bool active;          // Whether this handler is in use
    bvm *vm;              // Berry VM instance
    bvalue func;          // Berry function to call for requests
} http_handler_t;

static http_handler_t http_handlers[HTTP_HANDLER_MAX];

// Maximum concurrent HTTP server connections
#define HTTPD_MAX_CONNECTIONS 8

// Handle to HTTP server
static httpd_handle_t http_server = NULL;

// Disconnect handler for WebSocket connections
static httpd_close_func_t http_server_disconn_handler = NULL;

// Current HTTP request being processed (for Berry access)
static httpd_req_t *current_request = NULL;

// Connection tracking
static struct {
    int count;
    SemaphoreHandle_t mutex;
} connection_tracking = {0, NULL};

// Connection cleanup function
// CONTEXT: ESP-IDF HTTP Server Task
// Called automatically by ESP-IDF when a client disconnects
static void http_connection_cleanup(void *arg) {
    if (xSemaphoreTake(connection_tracking.mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        connection_tracking.count--;
        xSemaphoreGive(connection_tracking.mutex);
        
        // Call WebSocket disconnect handler if registered
        if (http_server_disconn_handler) {
            // arg is the socket file descriptor in this context
            http_server_disconn_handler(NULL, (int)(intptr_t)arg);
        }
    }
}

// WebSocket message processing function
// CONTEXT: Main Tasmota Task (Berry VM Context)
// This function runs in the main task when processing queued WebSocket messages
void be_httpserver_process_websocket_msg(bvm *vm, int client_id, const char *data, size_t data_len) {
    // Log message details safely (handling NULL data for connect events)
    if (data) {
        ESP_LOGD(TAG, "Processing WebSocket message in main task context: client=%d, data='%s', len=%d", 
                 client_id, data, (int)data_len);
    } else {
        ESP_LOGD(TAG, "Processing WebSocket event in main task context: client=%d", client_id);
    }
    
    // Forward to the WebSocket handler in be_wsserver_lib.c
    be_wsserver_handle_message(vm, client_id, data, data_len);
}

// File request processing function
static void be_httpserver_process_file_request(bvm *vm, void *user_data) {
    ESP_LOGI(TAG, "Processing file request (placeholder)");
    // Placeholder for file handling - to be extended as needed
}


// Initialize the message queue
static bool init_http_queue() {
    if (!http_queue_initialized) {
        http_msg_queue = xQueueCreate(10, sizeof(http_queue_msg_t));
        http_queue_mutex = xSemaphoreCreateMutex();
        
        if (http_msg_queue != NULL && http_queue_mutex != NULL) {
            http_queue_initialized = true;
            ESP_LOGI(TAG, "HTTP queue initialized");
            return true;
        } else {
            ESP_LOGE(TAG, "Failed to create HTTP queue");
            return false;
        }
    }
    return true;
}

// Queue a message for processing in the main task
// CONTEXT: Any Task (typically ESP-IDF HTTP Server Task)
// This function is called to queue messages for processing in the main task
// TRANSITION: Current Task → Main Tasmota Task
bool httpserver_queue_message(http_msg_type_t type, int client_id, const void *data, size_t data_len, void *user_data) {
    if (!http_queue_initialized) {
        ESP_LOGE(TAG, "Queue not initialized");
        return false;
    }
    
    if (!data && data_len > 0) {
        ESP_LOGE(TAG, "Invalid data pointer with non-zero length");
        return false;
    }
    
    // Take mutex to protect queue
    if (xSemaphoreTake(http_queue_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        ESP_LOGE(TAG, "Failed to take mutex");
        return false;
    }
    
    // Create a message to queue
    http_queue_msg_t msg = {0};;

    msg.type = type;
    msg.client_id = client_id;
    msg.user_data = user_data;
    
    // Special case for HTTP_MSG_WEB is handled by httpserver_queue_web_request
    if (type == HTTP_MSG_WEB) {
        ESP_LOGE(TAG, "HTTP_MSG_WEB must use httpserver_queue_web_request");
        xSemaphoreGive(http_queue_mutex);
        return false;
    }
    
    // For other message types, copy the data if needed
    if (data_len > 0) {
        char *data_copy = malloc(data_len + 1);
        if (!data_copy) {
            ESP_LOGE(TAG, "Failed to allocate memory for data copy");
            xSemaphoreGive(http_queue_mutex);
            return false;
        }
        
        memcpy(data_copy, data, data_len);
        data_copy[data_len] = '\0';  // Ensure null termination
        
        msg.data = data_copy;
        msg.data_len = data_len;
    }
    
    // Queue the message
    if (xQueueSend(http_msg_queue, &msg, 0) != pdTRUE) {
        // Queue is full, free the data if we allocated it
        if (msg.data) {
            free(msg.data);
        }
        ESP_LOGE(TAG, "Failed to queue message - queue is full");
        xSemaphoreGive(http_queue_mutex);
        return false;
    }
    
    // Message successfully queued
    ESP_LOGD(TAG, "Message queued successfully (type %d, client %d)", type, client_id);
    ESP_LOGD(TAG, "DIAGNOSTIC: Queue has %d messages waiting", uxQueueMessagesWaiting(http_msg_queue));
    
    if (msg.data) {
        ESP_LOGD(TAG, "QUEUE ITEM: type=%d, client=%d, data_len=%d, data_ptr=%p, user_data=%p", 
                msg.type, msg.client_id, (int)msg.data_len, msg.data, msg.user_data);
        ESP_LOGD(TAG, "QUEUE DATA: '%s'", msg.data);
    }
    
    xSemaphoreGive(http_queue_mutex);
    return true;
}

// Specialized function for queuing HTTP web requests with a Berry function
bool httpserver_queue_web_request(int handler_id, httpd_req_t *req, bvalue func) {
    if (!http_queue_initialized) {
        ESP_LOGE(TAG, "Queue not initialized");
        return false;
    }
    
    if (!req) {
        ESP_LOGE(TAG, "NULL request for web request");
        return false;
    }
    
    // Create a copy of the request that we can process asynchronously
    httpd_req_t *req_copy = NULL;
    esp_err_t err = httpd_req_async_handler_begin(req, &req_copy);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create async request: %d", err);
        return false;
    }
    
    // Take mutex to protect queue
    if (xSemaphoreTake(http_queue_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        ESP_LOGE(TAG, "Failed to take mutex");
        // Release the request copy since we won't be using it
        httpd_req_async_handler_complete(req_copy);
        return false;
    }
    
    // Create a message to queue
    http_queue_msg_t msg = {0};
    
    msg.type = HTTP_MSG_WEB;
    msg.client_id = handler_id;
    msg.user_data = NULL;
    msg.func = func;     // Store the function reference
    msg.req = req_copy;  // Store the COPY of the request handle
    
    // Queue the message
    if (xQueueSend(http_msg_queue, &msg, 0) != pdTRUE) {
        // Queue is full
        ESP_LOGE(TAG, "Failed to queue web request - queue is full");
        // Release the request copy since we won't be using it
        httpd_req_async_handler_complete(req_copy);
        xSemaphoreGive(http_queue_mutex);
        return false;
    }
    
    // Message successfully queued
    ESP_LOGD(TAG, "HTTP request queued successfully (type %d, handler %d)", msg.type, msg.client_id);
    ESP_LOGD(TAG, "DIAGNOSTIC: Queue has %d messages waiting", uxQueueMessagesWaiting(http_msg_queue));
    ESP_LOGD(TAG, "QUEUE ITEM: type=%d, client=%d, data_len=%d, data_ptr=%p, user_data=%p, req=%p", 
            msg.type, msg.client_id, (int)msg.data_len, msg.data, msg.user_data, msg.req);
    
    xSemaphoreGive(http_queue_mutex);
    return true;
}


// ------------------------------------------------------------------------
// Web request processing function
// CONTEXT: Main Tasmota Task (Berry VM Context)
// This function processes queued web requests in the main task context
// ------------------------------------------------------------------------

void be_httpserver_process_web_request(bvm *vm, http_queue_msg_t *msg) {
  ESP_LOGD(TAG, "Processing web request: msg=%p", msg);
  
  if (!msg) {
      ESP_LOGE(TAG, "Web request has NULL message handle");
      return;
  }
  
  ESP_LOGD(TAG, "Request details: req=%p, client_id=%d", msg->req, msg->client_id);
  
  if (!msg->req) {
      ESP_LOGE(TAG, "Web request has NULL request handle");
      return;
  }
  
  // Get handler ID (passed in client_id field)
  int handler_id = msg->client_id;
  if (handler_id < 0 || handler_id >= HTTP_HANDLER_MAX || !http_handlers[handler_id].active) {
      ESP_LOGE(TAG, "Invalid handler ID from queue: %d", handler_id);
      httpd_resp_set_status(msg->req, "500 Internal Server Error");
      httpd_resp_sendstr(msg->req, "Invalid handler ID");
      httpd_req_async_handler_complete(msg->req);
      return;
  }
  
  ESP_LOGI(TAG, "Processing web request for URI: %s with handler %d", msg->req->uri, handler_id);
  
  // Get the Berry VM and handler function
  bvm *handler_vm = http_handlers[handler_id].vm;
  
  if (handler_vm == NULL) {
      ESP_LOGE(TAG, "Berry VM is NULL for handler %d", handler_id);
      httpd_resp_set_status(msg->req, "500 Internal Server Error");
      httpd_resp_sendstr(msg->req, "VM error");
      httpd_req_async_handler_complete(msg->req);
      return;
  }
  
  ESP_LOGI(TAG, "STACK: Before pushing function, stack top = %d", be_top(handler_vm));
  
  // Push the function stored in the message
  be_pushnil(handler_vm);
  bvalue *top = be_indexof(handler_vm, -1);
  *top = msg->func;
  
  current_request = msg->req;
  
  // Push URI as argument
  be_pushstring(handler_vm, current_request->uri);
  
  // Check if this is a POST request and handle POST data
  int arg_count = 1;  // Start with 1 for the URI
  
  if (current_request->method == HTTP_POST) {
      ESP_LOGI(TAG, "Processing POST request data");
      
      // Get content length
      int content_len = current_request->content_len;
      ESP_LOGI(TAG, "POST content length: %d", content_len);
      
      if (content_len > 0) {
          // Allocate buffer for POST data
          char *post_data = malloc(content_len + 1);
          if (post_data) {
              // Read POST data
              int received = httpd_req_recv(current_request, post_data, content_len);
              if (received > 0) {
                  // Null-terminate the data
                  post_data[received] = '\0';
                  ESP_LOGI(TAG, "Received POST data: %s", post_data);
                  
                  // Push POST data as second argument
                  be_pushstring(handler_vm, post_data);
                  arg_count = 2;  // Now we have 2 arguments
              } else {
                  ESP_LOGW(TAG, "Failed to read POST data, received: %d", received);
                  // Push nil as second argument
                  be_pushnil(handler_vm);
                  arg_count = 2;
              }
              free(post_data);
          } else {
              ESP_LOGE(TAG, "Failed to allocate memory for POST data");
              // Push nil as second argument
              be_pushnil(handler_vm);
              arg_count = 2;
          }
      } else {
          // No content, push empty string as second argument
          be_pushstring(handler_vm, "");
          arg_count = 2;
      }
  }
  
  // Call the Berry function
  int result = be_pcall(handler_vm, arg_count);
  
  // Log stack state after call
  ESP_LOGI(TAG, "STACK: After be_pcall, stack top = %d, result = %d", be_top(handler_vm), result);
  
  // Check for errors
  if (result != 0) {
      const char *err_msg = be_tostring(handler_vm, -1);
      ESP_LOGE(TAG, "Berry handler error: %s", err_msg);
      
      // Send error response
      httpd_resp_set_status(msg->req, "500 Internal Server Error");
      httpd_resp_sendstr(msg->req, (char*)err_msg);
      
      be_error_pop_all(handler_vm);  // Clear entire stack on error
  } else {
      // Get return value
      const char *response = be_tostring(handler_vm, -1);
      ESP_LOGI(TAG, "Request processed. Response: %s", response ? response : "(null)");
      
      // Send success response if httpserver.send() wasn't used
      if (response != NULL) {
          httpd_resp_set_type(msg->req, "text/html"); 
          httpd_resp_sendstr(msg->req, response);
      }
      
      // Pop the argument (which has been replaced by the return value)
      be_pop(handler_vm, 1);
  }
  
  // Clear current_request AFTER all processing is done
  current_request = NULL;
  
  // Complete the async request - ALWAYS call this to release the request
  httpd_req_async_handler_complete(msg->req);
  
  // Pop the function if we didn't encounter an error
  if (result == 0) {
      // Pop the function
      be_pop(handler_vm, 1);  // Pop the function reference
  } else {
      ESP_LOGE(TAG, "Function parsing error: %d", result);
  }
  
  // Log final stack state
  ESP_LOGI(TAG, "STACK: Final state, stack top = %d", be_top(handler_vm));
}


// ------------------------------------------------------------------------
// Berry mapped C function to process queued messages
// CONTEXT: Main Tasmota Task (Berry VM Context)
// This function is registered to fast_loop() by the Berry app and is called
// periodically to process queued HTTP/WebSocket messages from the ESP-IDF HTTP server
// ------------------------------------------------------------------------

static int w_httpserver_process_queue(bvm *vm) {
    if (!http_msg_queue) {
        be_pushnil(vm);
        be_return(vm);
    }

    // Count of messages processed in this call
    int processed = 0;
    
    // Process up to 5 messages in a single call to avoid blocking
    for (int i = 0; i < 5; i++) {
        // Take mutex before accessing queue
        if (xSemaphoreTake(http_queue_mutex, 0) != pdTRUE) {
            ESP_LOGW(TAG, "Failed to take mutex, will retry");
            break;
        }
        
        // Process one message from the queue
        http_queue_msg_t msg;
        if (xQueueReceive(http_msg_queue, &msg, 0) == pdTRUE) {
            // Release mutex while processing message
            xSemaphoreGive(http_queue_mutex);
            
            // Count this message
            processed++;
            
            // Diagnostic logging for queue state
            ESP_LOGD(TAG, "QUEUE ITEM: type=%d, client=%d, data_len=%d, data_ptr=%p, user_data=%p, req=%p", 
                    msg.type, msg.client_id, msg.data_len, msg.data, msg.user_data, msg.req);
            
            // Process message based on type
            switch (msg.type) {
                case HTTP_MSG_WEBSOCKET:
                    if (msg.data) {
                        ESP_LOGD(TAG, "QUEUE DATA: '%.*s'", msg.data_len, (char*)msg.data);
                    } else {
                        ESP_LOGD(TAG, "QUEUE DATA: '' (connect/disconnect event)");
                    }
                    be_httpserver_process_websocket_msg(vm, msg.client_id, msg.data, msg.data_len);
                    // Free the data buffer we allocated
                    if (msg.data) {
                        free(msg.data);
                    }
                    break;
                    
                case HTTP_MSG_FILE:
                    ESP_LOGI(TAG, "Processing file request");
                    be_httpserver_process_file_request(vm, msg.user_data);
                    // user_data is not allocated by us, so don't free it
                    break;
                    
                case HTTP_MSG_WEB:
                    ESP_LOGD(TAG, "Processing web request from queue");
                    if (msg.req == NULL) {
                        ESP_LOGE(TAG, "CRITICAL ERROR: HTTP request pointer is NULL, skipping processing");
                        break;
                    }
                    be_httpserver_process_web_request(vm, &msg);
                    break;
                    
                default:
                    ESP_LOGW(TAG, "Unknown message type: %d", msg.type);
                    // Free data if it was allocated
                    if (msg.data) {
                        free(msg.data);
                    }
                    // If it's a request that wasn't processed, complete it
                    if (msg.req) {
                        httpd_req_async_handler_complete(msg.req);
                    }
                    break;
            }
        } else {
            // No messages in queue
            xSemaphoreGive(http_queue_mutex);
            break;
        }
    }

    // Return the number of messages processed
    be_pushint(vm, processed);
    be_return(vm);
}


// ------------------------------------------------------------------------
// HTTP Handler implementation
// ------------------------------------------------------------------------

// Forward declaration for handler implementation
static esp_err_t berry_http_handler_impl(httpd_req_t *req, int handler_id);

// Macro to create handler functions for each supported endpoint
#define HTTP_HANDLER_FUNC(n) \
    static esp_err_t berry_http_handler_##n(httpd_req_t *req) { \
        return berry_http_handler_impl(req, n); \
    }

// Generate handler functions
HTTP_HANDLER_FUNC(0)
HTTP_HANDLER_FUNC(1)
HTTP_HANDLER_FUNC(2)
HTTP_HANDLER_FUNC(3)
HTTP_HANDLER_FUNC(4)

// Array of handler function pointers
typedef esp_err_t (*http_handler_func_t)(httpd_req_t *req);
static const http_handler_func_t berry_handlers[HTTP_HANDLER_MAX] = {
    berry_http_handler_0,
    berry_http_handler_1,
    berry_http_handler_2,
    berry_http_handler_3,
    berry_http_handler_4
};

// Implementation of HTTP handler dispatched by each numbered handler
static esp_err_t berry_http_handler_impl(httpd_req_t *req, int handler_id) {
    if (handler_id < 0 || handler_id >= HTTP_HANDLER_MAX || !http_handlers[handler_id].active) {
        ESP_LOGE(TAG, "Invalid or inactive handler ID: %d", handler_id);
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Invalid handler");
        httpd_req_async_handler_complete(req);
        return ESP_FAIL;
    }
    
    // Store current request for access in Berry functions
    current_request = req;
    
    // Get the Berry VM and handler function
    bvm *vm = http_handlers[handler_id].vm;
    
    if (vm == NULL) {
        ESP_LOGE(TAG, "Berry VM is NULL for handler %d", handler_id);
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "VM error");
        current_request = NULL;
        httpd_req_async_handler_complete(req);
        return ESP_FAIL;
    }
    
    // Log initial stack state
    ESP_LOGI(TAG, "HANDLER: Initial stack top = %d", be_top(vm));
    
    // Queue message for processing in main task if available
    if (httpserver_has_queue()) {
        ESP_LOGI(TAG, "Queueing request for %s", req->uri);
        
        // Queue the request with the stored function value
        if (!httpserver_queue_web_request(handler_id, req, http_handlers[handler_id].func)) {
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to queue message");
            current_request = NULL;
            httpd_req_async_handler_complete(req);
            return ESP_FAIL;
        }
        
        // Note: We don't send a response here - that will be done asynchronously
        current_request = NULL;
        
        // Log final stack state
        ESP_LOGI(TAG, "HANDLER: Final stack top = %d", be_top(vm));
        
        return ESP_OK;
    }
    
    // If no queue, we'll process directly with caution
    ESP_LOGW(TAG, "Processing request directly - this may be unsafe!");
    
    // Start the async handler
    httpd_req_t *async_req = NULL;
    esp_err_t ret = httpd_req_async_handler_begin(req, &async_req);
    if (ret != ESP_OK || async_req == NULL) {
        ESP_LOGE(TAG, "Failed to start async handler");
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to start async handler");
        current_request = NULL;
        httpd_req_async_handler_complete(req);
        return ESP_FAIL;
    }
    
    // Get the initial stack size
    int top = be_top(vm);
    
    // Push the handler function directly onto the stack (copy from stored value)
    be_pushnil(vm);  // Push a temporary placeholder 
    bvalue *top_ptr = be_indexof(vm, -1);
    *top_ptr = http_handlers[handler_id].func;  // Replace placeholder with stored function
    
    // Push the URI string (argument) onto the stack
    be_pushstring(vm, req->uri);
    
    // Call the handler function with the URI as single argument
    if (be_pcall(vm, 1) != 0) {
        const char *err_msg = be_tostring(vm, -1);
        ESP_LOGE(TAG, "Berry error: %s", err_msg ? err_msg : "unknown error");
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Handler call failed");
        be_error_pop_all(vm);  // Special case - clears entire stack on error
        current_request = NULL;
        httpd_req_async_handler_complete(req);
        return ESP_FAIL;
    }
    
    // Get the response string
    const char *response = be_tostring(vm, -1);
    if (response == NULL) {
        ESP_LOGE(TAG, "Handler returned nil response");
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Handler returned nil response");
        be_error_pop_all(vm);  // Special case - clears entire stack on error
        current_request = NULL;
        httpd_req_async_handler_complete(req);
        return ESP_FAIL;
    }
    
    // Send the response
    httpd_resp_set_type(req, "text/html");
    httpd_resp_sendstr(req, response);
    
    // Clean up
    be_pop(vm, 1);  // Pop return value
    be_pop(vm, 1);  // Pop function
    current_request = NULL;
    
    // Complete the async handler
    httpd_req_async_handler_complete(async_req);
    
    return ESP_OK;
}

// ------------------------------------------------------------------------
// Berry API Implementation
// ------------------------------------------------------------------------

// Start the HTTP server
static int w_httpserver_start(bvm *vm) {
    int top = be_top(vm);
    
    if (http_server != NULL) {
        be_pushbool(vm, true);  // Server already running
        be_return (vm);
    }
    
    // Initialize connection tracking
    connection_tracking.mutex = xSemaphoreCreateMutex();
    if (!connection_tracking.mutex) {
        ESP_LOGE(TAG, "Failed to create connection tracking mutex");
        be_pushbool(vm, false);
        be_return (vm);
    }
    
    // Configure the server
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.stack_size = 8192;
    config.max_uri_handlers = HTTP_HANDLER_MAX;
    config.max_open_sockets = HTTPD_MAX_CONNECTIONS;
    config.lru_purge_enable = true;  // Enable LRU purging of connections
    config.uri_match_fn = httpd_uri_match_wildcard;  // Enable wildcard URI matching
    
    // Handle port parameter if provided
    if (top > 0 && be_isint(vm, 1)) {
        config.server_port = be_toint(vm, 1);
    }
    
    ESP_LOGI(TAG, "Starting HTTP server on port %d", config.server_port);
    esp_err_t ret = httpd_start(&http_server, &config);
    
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start HTTP server: %d", ret);
        vSemaphoreDelete(connection_tracking.mutex);
        be_pushbool(vm, false);
        be_return (vm);
    }
    
    ESP_LOGI(TAG, "HTTP server started successfully");
    
    // Initialize the queue for thread-safe message passing
    init_http_queue();
    
    be_pushbool(vm, true);
    be_return (vm);
}

// Register a URI handler
static int w_httpserver_on(bvm *vm) {
    int top = be_top(vm);
    httpd_method_t http_method = HTTP_GET; // Default method
    
    if (top < 2 || http_server == NULL) {
        be_raise(vm, "value_error", top < 2 ? "Missing arguments" : "Server not started");
        be_return(vm);
    }
    
    if (!be_isstring(vm, 1) || !be_isfunction(vm, 2)) {
        be_raise(vm, "type_error", "String and function required");
        be_return(vm);
    }
    
    // Check for optional method argument
    if (top >= 3) {
        if (!be_isstring(vm, 3)) {
            be_raise(vm, "type_error", "Method must be a string");
            be_return(vm);
        }
        const char *method_str = be_tostring(vm, 3);
        if (strcasecmp(method_str, "POST") == 0) {
            http_method = HTTP_POST;
        } else if (strcasecmp(method_str, "GET") != 0) {
            be_raise(vm, "value_error", "Method must be 'GET' or 'POST'");
            be_return(vm);
        }
    }
    
    const char *uri = be_tostring(vm, 1);
    ESP_LOGI(TAG, "Registering handler for URI: %s, Method: %s", uri, 
             http_method == HTTP_GET ? "GET" : "POST");
    
    // Find a free handler slot
    int slot = -1;
    for (int i = 0; i < HTTP_HANDLER_MAX; i++) {
        if (!http_handlers[i].active) {
            slot = i;
            break;
        }
    }
    
    if (slot < 0) {
        be_raise(vm, "runtime_error", "No more handler slots available");
        be_return(vm);
    }
    
    // Store handler info
    http_handlers[slot].vm = vm;
    http_handlers[slot].active = true;
    
    // Store the function reference
    be_pushvalue(vm, 2);
    bvalue *v = be_indexof(vm, -1);
    http_handlers[slot].func = *v;
    be_pop(vm, 1);
    
    // Register the handler with ESP-IDF HTTP server
    httpd_uri_t http_uri = {
        .uri       = uri,
        .method    = http_method,
        .handler   = berry_handlers[slot],
        .user_ctx  = NULL
    };
    
    esp_err_t ret = httpd_register_uri_handler(http_server, &http_uri);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to register URI handler: %d", ret);
        http_handlers[slot].active = false;
        be_pushbool(vm, false);
        be_return (vm);
    }
    
    // Return the handler slot
    be_pushint(vm, slot);
    be_return (vm);
}

// Stop the HTTP server
static int w_httpserver_stop(bvm *vm) {
    if (http_server == NULL) {
        be_pushbool(vm, false);  // Server not running
        be_return (vm);
    }
    
    // Clean up handler registrations
    for (int i = 0; i < HTTP_HANDLER_MAX; i++) {
        if (http_handlers[i].active) {
            http_handlers[i].active = false;
            // Potentially unregister URI handlers here if needed
        }
    }
    
    // Stop the server
    esp_err_t ret = httpd_stop(http_server);
    http_server = NULL;
    
    // Clean up connection tracking
    if (connection_tracking.mutex) {
        vSemaphoreDelete(connection_tracking.mutex);
        connection_tracking.mutex = NULL;
    }
    
    be_pushbool(vm, ret == ESP_OK);
    be_return (vm);
}

// Simple wrapper around httpd_resp_sendstr
static int w_httpserver_send(bvm *vm) {
    int argc = be_top(vm);
    if (argc >= 1 && be_isstring(vm, 1)) {
        const char* content = be_tostring(vm, 1);
        
        // Get the current request from the async message
        httpd_req_t* req = current_request;
        if (!req) {
            be_raisef(vm, "request_error", "No active request");
            // Note: Don't call httpd_req_async_handler_complete here as there's no valid request
            return 0;
        }
        
        // Send the response
        esp_err_t ret = httpd_resp_sendstr(req, content);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to send response: %d", ret);
            // Don't complete the handler here - let the main handler do it
            be_pushbool(vm, false);
            be_return (vm);
        }
        
        be_pushbool(vm, true);
        be_return (vm);
    }
    
    be_return_nil(vm);
}

// Set WebSocket disconnect handler
void be_httpserver_set_disconnect_handler(httpd_close_func_t handler) {
    http_server_disconn_handler = handler;
}

// Get HTTP server handle
httpd_handle_t be_httpserver_get_handle(void) {
    return http_server;
}

// Function to check if message queue is available (referenced by wsserver)
bool httpserver_has_queue() {
    return http_queue_initialized;
}

/* @const_object_info_begin
module httpserver (scope: global, strings: weak) {
    start, func(w_httpserver_start)
    on, func(w_httpserver_on)
    send, func(w_httpserver_send)
    stop, func(w_httpserver_stop)
    process_queue, func(w_httpserver_process_queue)
}
@const_object_info_end */

#include "be_fixed_httpserver.h"

#endif // USE_BERRY_HTTPSERVER