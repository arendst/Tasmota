/*
  be_wsserver_lib.c - WebSocket server support for Berry using ESP-IDF HTTP server

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

#ifdef USE_BERRY_WSSERVER

#ifndef LOG_LOCAL_LEVEL
#define LOG_LOCAL_LEVEL ESP_LOG_INFO
#endif

#include "be_constobj.h"
#include "be_mapping.h"
#include "be_exec.h"
#include "be_vm.h"

// Standard C includes
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// ESP-IDF includes
#include "esp_http_server.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_timer.h"

// Socket-related includes
#include <sys/socket.h>
#include <netinet/in.h>

#include "be_object.h"
#include "be_string.h"
#include "be_gc.h"
#include "be_exec.h"
#include "be_debug.h"
#include "be_map.h"
#include "be_list.h"
#include "be_module.h"
#include "be_vm.h"

static const char *TAG = "WSS";

// Max number of concurrent clients
#define MAX_WS_CLIENTS 5

// Define event types
#define WSSERVER_EVENT_CONNECT    0
#define WSSERVER_EVENT_DISCONNECT 1
#define WSSERVER_EVENT_MESSAGE    2

// Message types for queue - match those in be_httpserver_lib.c
#define HTTP_MSG_WEBSOCKET 1
#define HTTP_MSG_FILE 2
#define HTTP_MSG_WEB 3

// Forward declaration for the HTTP server handle getter function
extern httpd_handle_t be_httpserver_get_handle(void);

// Declarations for functions used by httpserver_lib
extern bool httpserver_has_queue(void);
extern bool httpserver_queue_message(int msg_type, int client_id, 
                                const void *data, size_t data_len, void *user_data);

// Client tracking structure
typedef struct {
    int sockfd;
    bool active;
    int64_t last_activity;  // Timestamp of any client activity
} ws_client_t;

// Callback structure to properly store Berry callbacks
typedef struct be_wsserver_callback_t {
  bvm *vm;                // VM instance
  bvalue func;            // Berry function value
  bool active;            // Whether this callback is active
} be_wsserver_callback_t;

// Forward declarations for all functions to prevent compiler errors
static void init_clients(void);
static int find_free_client_slot(void);
static int add_client(int sockfd);
static int find_client_by_fd(int sockfd);
static void remove_client(int slot);
static bool is_client_valid(int slot);
static void log_ws_frame_info(httpd_ws_frame_t *ws_pkt);
static void handle_ws_message(int client_id, const char *message, size_t len);
static void handle_client_disconnect(int client_slot);
static void callBerryWsDispatcher(bvm *vm, int client_id, const char *event_name, const char *payload, int arg_count);
static void http_server_disconnect_handler(void* arg, int sockfd);
static void check_clients(void);

// Globals
static httpd_handle_t ws_server = NULL;
static bool wsserver_running = false;
static uint32_t ping_interval_s;  // Ping interval in seconds
static uint32_t ping_timeout_s;   // Activity timeout in seconds

// Client status and context
static ws_client_t ws_clients[MAX_WS_CLIENTS] = {0};

// Storage for Berry callback functions
static be_wsserver_callback_t wsserver_callbacks[3]; // CONNECT, DISCONNECT, MESSAGE

// Forward declaration for processing WebSocket messages in main task context
void be_wsserver_handle_message(bvm *vm, int client_id, const char* data, size_t len);

// Utility function to log WebSocket frame information
static void log_ws_frame_info(httpd_ws_frame_t *ws_pkt) {
    if (!ws_pkt) return;
    
    const char* type_str = "UNKNOWN";
    switch (ws_pkt->type) {
        case HTTPD_WS_TYPE_CONTINUE: type_str = "CONTINUE"; break;
        case HTTPD_WS_TYPE_TEXT: type_str = "TEXT"; break;
        case HTTPD_WS_TYPE_BINARY: type_str = "BINARY"; break;
        case HTTPD_WS_TYPE_CLOSE: type_str = "CLOSE"; break;
        case HTTPD_WS_TYPE_PING: type_str = "PING"; break;
        case HTTPD_WS_TYPE_PONG: type_str = "PONG"; break;
    }
    
    ESP_LOGI(TAG, "WS frame: type=%s, len=%d, final=%d", 
             type_str, ws_pkt->len, ws_pkt->final ? 1 : 0);
}

// Call a Berry callback function registered by wsserver.on()
// CONTEXT: Main Tasmota Task (Berry VM Context)
// This function is only called from the main task when processing queued events
static void callBerryWsDispatcher(bvm *vm, int client_id, const char *event_name, const char *payload, int arg_count) {
    if (!vm) {
        ESP_LOGE(TAG, "Berry VM is NULL in callBerryWsDispatcher");
        return;
    }
    
    // Ensure parameters are valid
    if (client_id < 0 || client_id >= MAX_WS_CLIENTS || !event_name) {
        ESP_LOGE(TAG, "Invalid parameters in callBerryWsDispatcher");
        return;
    }
    
    ESP_LOGI(TAG, "Calling Berry callback for event '%s', client %d, payload: %s", 
             event_name, client_id, payload ? payload : "nil");
    
    // Map event name to event type
    int event_type = -1;
    if (strcmp(event_name, "connect") == 0) {
        event_type = WSSERVER_EVENT_CONNECT;
    } else if (strcmp(event_name, "disconnect") == 0) {
        event_type = WSSERVER_EVENT_DISCONNECT;
    } else if (strcmp(event_name, "message") == 0) {
        event_type = WSSERVER_EVENT_MESSAGE;
    } else {
        ESP_LOGE(TAG, "Unknown event type: %s", event_name);
        return;
    }
    
    // Check if we have a registered callback for this event type
    if (!wsserver_callbacks[event_type].active) {
        ESP_LOGI(TAG, "No callback registered for event type %d (%s)", event_type, event_name);
        return;
    }
    
    ESP_LOGI(TAG, "Using registered callback for event type %d (%s)", event_type, event_name);
    
    // Save initial stack position for diagnostic logging
    int initial_top = be_top(vm);
    ESP_LOGI(TAG, "Stack before function push: top=%d", initial_top);
    
    // Push the callback function onto the stack
    bvalue *reg = vm->top;
    var_setval(reg, &wsserver_callbacks[event_type].func);
    be_incrtop(vm);
    
    ESP_LOGI(TAG, "Stack after function push: top=%d", be_top(vm));
    
    // Push the client ID first, event name second, payload third (if applicable)
    // This matches Berry function signature: function(client, event, message)
    be_pushint(vm, client_id);  // Push client ID argument
    be_pushstring(vm, event_name);  // Push event name argument
    
    // Push payload argument for message events (optional)
    if (arg_count > 2 && payload) {
        be_pushstring(vm, payload);
    }
    
    // Log the arguments about to be passed
    ESP_LOGI(TAG, "Stack after arg push: top=%d, with %d arguments", be_top(vm), arg_count);
    
    // Call the callback function with the appropriate number of arguments
    int call_result = be_pcall(vm, arg_count);
    
    ESP_LOGI(TAG, "Stack after be_pcall: top=%d", be_top(vm));
    
    // Handle the Berry call result
    if (call_result != BE_OK) {
        ESP_LOGE(TAG, "Berry callback error for event '%s': %s", 
                 event_name, be_tostring(vm, -1));
        
        be_error_pop_all(vm);
    } else {
        // Pop all arguments (including return value)
        be_pop(vm, arg_count);
        
        ESP_LOGI(TAG, "Stack after arg pop: top=%d", be_top(vm));
        
        // Pop the function separately
        be_pop(vm, 1);
        
        ESP_LOGI(TAG, "Final stack after cleanup: top=%d", be_top(vm));
    }
}

// Process a WebSocket message in the main task context
// CONTEXT: Main Tasmota Task (Berry VM Context)
// This function processes messages from the queue in the main task
void be_wsserver_handle_message(bvm *vm, int client_id, const char* data, size_t len) {
    if (!data) {
        // This is either a connect or disconnect event (no data)
        // Determine event type based on client state
        if (is_client_valid(client_id)) {
            // Client exists and is active - this is a connect event
            ESP_LOGI(TAG, "Handling WebSocket connect event in main task: client=%d", client_id);
            callBerryWsDispatcher(vm, client_id, "connect", NULL, 2);
        } else {
            // Client no longer active - this is a disconnect event
            ESP_LOGI(TAG, "Handling WebSocket disconnect event in main task: client=%d", client_id);
            callBerryWsDispatcher(vm, client_id, "disconnect", NULL, 2);
            
            // Mark client as inactive after processing
            if (client_id >= 0 && client_id < MAX_WS_CLIENTS) {
                ws_clients[client_id].sockfd = -1;
                ws_clients[client_id].active = false;
            }
        }
    } else {
        // Normal message event with data
        ESP_LOGI(TAG, "Handling WebSocket message event in main task: client=%d, len=%d, data=%s", 
                client_id, (int)len, data);
        callBerryWsDispatcher(vm, client_id, "message", data, 3);
    }
}

// WebSocket Event Handler
// CONTEXT: ESP-IDF HTTP Server Task
static esp_err_t ws_handler(httpd_req_t *req) {
    if (req->method == HTTP_GET) {
        // Handshake handling
        ESP_LOGI(TAG, "WebSocket handshake received");
        int sockfd = httpd_req_to_sockfd(req);        
        int client_slot = add_client(sockfd); 
        
        if (client_slot >= 0) {
            ESP_LOGI(TAG, "Client %d connected, socket fd: %d", client_slot, sockfd);
            
            // Queue connect event for processing in main task
            // TRANSITION: ESP-IDF HTTP Server Task → Main Tasmota Task
            // Use NULL as data to indicate this is a connect event
            if (httpserver_has_queue()) {
                bool queue_result = httpserver_queue_message(HTTP_MSG_WEBSOCKET, client_slot, 
                                                   NULL, 0, NULL);
                
                if (queue_result) {
                    ESP_LOGI(TAG, "WebSocket connect event successfully queued");
                } else {
                    ESP_LOGE(TAG, "Failed to queue WebSocket connect event!");
                }
            } else {
                ESP_LOGW(TAG, "No queue available for connect event - fallback to direct processing");
                // Fallback to direct processing (not recommended)
                if (wsserver_callbacks[WSSERVER_EVENT_CONNECT].active) {
                    const char *event_name = "connect";
                    callBerryWsDispatcher(wsserver_callbacks[WSSERVER_EVENT_CONNECT].vm, 
                                         client_slot, event_name, NULL, 2);
                }
            }
        }
        
        // Return success for the handshake
        return ESP_OK;
    }
    
    ESP_LOGD(TAG, "WebSocket frame received");
    int sockfd = httpd_req_to_sockfd(req);
    int client_slot = find_client_by_fd(sockfd);
    
    if (client_slot < 0) {
        ESP_LOGE(TAG, "Received frame from unknown client (socket: %d)", sockfd);
        return ESP_FAIL;
    }
    
    // ----- PRE-PROCESSING: Frame Validation & Memory Allocation -----
    
    // Update activity time for ANY frame
    ws_clients[client_slot].last_activity = esp_timer_get_time() / 1000;
    
    // Standard ESP-IDF WebSocket frame reception
    httpd_ws_frame_t ws_pkt = {0};
    uint8_t *buf = NULL;

    ws_pkt.type = HTTPD_WS_TYPE_TEXT;  // Default type

    // Get the frame length
    esp_err_t ret = httpd_ws_recv_frame(req, &ws_pkt, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "httpd_ws_recv_frame failed with %d", ret);
        return ret;
    }
    
    ESP_LOGD(TAG, "Frame len is %d, type is %d", ws_pkt.len, ws_pkt.type);
    
    // Handle control frames immediately without involving Berry VM
    if (ws_pkt.type == HTTPD_WS_TYPE_PONG) {
        ESP_LOGI(TAG, "Received PONG from client %d", client_slot);
        return ESP_OK;
    } 
    
    if (ws_pkt.type == HTTPD_WS_TYPE_CLOSE) {
        ESP_LOGI(TAG, "Received CLOSE from client %d", client_slot);
        handle_client_disconnect(client_slot);
        return ESP_OK;
    }

    if (ws_pkt.type == HTTPD_WS_TYPE_PING) {
        ESP_LOGI(TAG, "Received PING from client %d", client_slot);
        httpd_ws_frame_t pong = {0};
        pong.type = HTTPD_WS_TYPE_PONG;
        pong.len = 0;
        ret = httpd_ws_send_frame(req, &pong);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to send PONG: %d", ret);
        }
        return ESP_OK;
    }
    
    // Not a control frame , so allocate memory for the message payload
    if (ws_pkt.len) {
        buf = calloc(1, ws_pkt.len + 1); 
        if (buf == NULL) {
            ESP_LOGE(TAG, "Failed to allocate memory for WS message");
            return ESP_ERR_NO_MEM;
        }
        ws_pkt.payload = buf;  
        // Receive the actual message
        ret = httpd_ws_recv_frame(req, &ws_pkt, ws_pkt.len);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "httpd_ws_recv_frame failed with %d", ret);
            free(buf);
            return ret;
        }
        
        // Ensure null-termination for text frames
        if (ws_pkt.type == HTTPD_WS_TYPE_TEXT) {
            buf[ws_pkt.len] = 0;
        }
    } else {
        // Empty message, nothing to process
        return ESP_OK;
    }
    
    // Process message based on type
    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT) {
        ESP_LOGI(TAG, "Received TEXT WebSocket message: '%s'", buf);
        handle_ws_message(client_slot, (char*)buf, ws_pkt.len);
    } else if (ws_pkt.type == HTTPD_WS_TYPE_BINARY) {
        ESP_LOGI(TAG, "Received BINARY WebSocket message, len=%d", ws_pkt.len);
        handle_ws_message(client_slot, (char*)buf, ws_pkt.len);
    }
    
    // Free the data copy regardless of send result
    free(buf);
    
    return ESP_OK;
}

// Handle a WebSocket message from a client
void handle_ws_message(int client_id, const char *message, size_t len) {
    if (!message || len == 0) {
        ESP_LOGE(TAG, "Received empty message from client %d", client_id);
        return;
    }
    
    // Update activity timestamp
    if (client_id >= 0 && client_id < MAX_WS_CLIENTS && ws_clients[client_id].active) {
        ws_clients[client_id].last_activity = esp_timer_get_time() / 1000;
    }
    
    // If queue available, send to main task for processing
    if (httpserver_has_queue()) {
        ESP_LOGD(TAG, "Queueing WebSocket message from client %d: '%.*s'", client_id, (int)len, message);
        
        // Make a copy of the message data for the queue
        char *data_copy = malloc(len);
        if (!data_copy) {
            ESP_LOGE(TAG, "Failed to allocate memory for message copy");
            return;
        }
        
        memcpy(data_copy, message, len);
        
        // Queue the message
        if (!httpserver_queue_message(HTTP_MSG_WEBSOCKET, client_id, data_copy, len, NULL)) {
            ESP_LOGE(TAG, "Failed to queue WebSocket message");
            free(data_copy);
        }
    } else {
        // No queue available, process directly (not recommended but fallback)
        ESP_LOGW(TAG, "Processing WebSocket message directly without queue - this may be unsafe");
        callBerryWsDispatcher(NULL, client_id, "message", message, 2);
    }
}

// Handle client disconnection
// CONTEXT: ESP-IDF HTTP Server Task
// Called when the server detects a client disconnection
static void handle_client_disconnect(int client_slot) {
    if (!is_client_valid(client_slot)) {
        return;
    }
    
    ESP_LOGI(TAG, "Client %d disconnected", client_slot);
    
    // Queue disconnect event for processing in main task
    // Use NULL as data to indicate this is a disconnect event
    if (httpserver_has_queue()) {
        // Save sockfd for later removal
        int sockfd = ws_clients[client_slot].sockfd;
        
        // Mark client as inactive BEFORE queuing the event
        // This ensures the event processor knows it's a disconnect event
        ws_clients[client_slot].active = false;
        
        bool queue_result = httpserver_queue_message(HTTP_MSG_WEBSOCKET, client_slot, 
                                           NULL, 0, NULL);
        
        if (queue_result) {
            ESP_LOGD(TAG, "WebSocket disconnect event successfully queued");
        } else {
            ESP_LOGE(TAG, "Failed to queue WebSocket disconnect event!");
            
            // Fallback to direct processing
            if (wsserver_callbacks[WSSERVER_EVENT_DISCONNECT].active) {
                const char *event_name = "disconnect";
                callBerryWsDispatcher(wsserver_callbacks[WSSERVER_EVENT_DISCONNECT].vm, 
                                     client_slot, event_name, NULL, 2);
            }
            
            // Ensure cleanup happens if queue fails
            ws_clients[client_slot].sockfd = -1;
        }
    } else {
        ESP_LOGW(TAG, "No queue available for disconnect event - fallback to direct processing");
        
        // Fallback to direct processing (not recommended)
        if (wsserver_callbacks[WSSERVER_EVENT_DISCONNECT].active) {
            const char *event_name = "disconnect";
            callBerryWsDispatcher(wsserver_callbacks[WSSERVER_EVENT_DISCONNECT].vm, 
                                 client_slot, event_name, NULL, 2);
        }
        
        // Always clean up client slot
        ws_clients[client_slot].sockfd = -1;
        ws_clients[client_slot].active = false;
    }
}

// Timer callback for pinging clients
static void ws_ping_timer_callback(void* arg) {
    // Skip if server is not running or pings disabled
    if (!wsserver_running || !ws_server || ping_interval_s == 0) return;
    
    int64_t now = esp_timer_get_time();
    
    for (int i = 0; i < MAX_WS_CLIENTS; i++) {
        if (ws_clients[i].active) {
            // Check for inactivity timeout if enabled
            if (ping_timeout_s > 0) {
                // Calculate inactivity time based on milliseconds
                int64_t inactivity_s = ((now / 1000) - ws_clients[i].last_activity) / 1000; // now/1000 for ms, then /1000 for seconds
                
                if (inactivity_s > ping_timeout_s) {
                    ESP_LOGI(TAG, "Client %d timed out (inactive for %d seconds)", 
                             i, (int)inactivity_s);
                    
                    // Trigger session close
                    handle_client_disconnect(i);
                    continue;
                }
            }
            
            // Send ping
            httpd_ws_frame_t ping = {0};
            ping.type = HTTPD_WS_TYPE_PING;
            
            ESP_LOGI(TAG, "Sending PING to client %d", i);
            esp_err_t ret = httpd_ws_send_frame_async(ws_server, ws_clients[i].sockfd, &ping);
            
            if (ret != ESP_OK) {
                ESP_LOGE(TAG, "Failed to send PING to client %d: %d", i, ret);
                
                // Trigger session close
                handle_client_disconnect(i);
            }
        }
    }
}

// Client Management Functions
static void init_clients(void) {
    for (int i = 0; i < MAX_WS_CLIENTS; i++) {
        ws_clients[i].active = false;
        ws_clients[i].sockfd = -1;
    }
}

static int find_free_client_slot(void) {
    for (int i = 0; i < MAX_WS_CLIENTS; i++) {
        if (!ws_clients[i].active) {
            return i;
        }
    }
    return -1; // No free slots
}

static int add_client(int sockfd) {
    int slot = find_free_client_slot();
    if (slot >= 0) {
        ws_clients[slot].sockfd = sockfd;
        ws_clients[slot].active = true;
        ws_clients[slot].last_activity = esp_timer_get_time() / 1000;
        ESP_LOGI(TAG, "Added client %d with socket %d", slot, sockfd);
        return slot;
    }
    ESP_LOGE(TAG, "No free client slots available");
    return -1;
}

static int find_client_by_fd(int sockfd) {
    for (int i = 0; i < MAX_WS_CLIENTS; i++) {
        if (ws_clients[i].active && ws_clients[i].sockfd == sockfd) {
            return i;
        }
    }
    return -1;
}

static void remove_client(int slot) {
    if (slot >= 0 && slot < MAX_WS_CLIENTS) {
        ESP_LOGI(TAG, "Removing client %d with socket %d", slot, ws_clients[slot].sockfd);
        ws_clients[slot].active = false;
        ws_clients[slot].sockfd = -1;
    }
}

// Check if a client is valid and connected
static bool is_client_valid(int client_id) {
    // Check for valid client ID range
    if (client_id < 0 || client_id >= MAX_WS_CLIENTS) {
        ESP_LOGE(TAG, "Invalid client ID: %d", client_id);
        return false;
    }
    
    // Check if client is active and has a valid socket
    if (!ws_clients[client_id].active || ws_clients[client_id].sockfd < 0) {
        ESP_LOGE(TAG, "Client %d is not active (active=%d, sockfd=%d)", 
                client_id, ws_clients[client_id].active, ws_clients[client_id].sockfd);
        return false;
    }
    
    return true;
}

// Berry Interface Functions
static int w_wsserver_start(bvm *vm) {
    if (wsserver_running) {
        ESP_LOGI(TAG, "WebSocket server already running");
        // Don't just return true - we should still update the path or other parameters
        // if they've changed, but don't re-register the handlers
        if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
            const char* path = be_tostring(vm, 1);
            
            // Get optional ping parameters
            if (be_top(vm) >= 3 && be_isint(vm, 3)) {
                ping_interval_s = be_toint(vm, 3);
                ESP_LOGD(TAG, "Updated ping interval to %d seconds", ping_interval_s);
            }
            
            if (be_top(vm) >= 4 && be_isint(vm, 4)) {
                ping_timeout_s = be_toint(vm, 4);
                ESP_LOGD(TAG, "Updated ping timeout to %d seconds", ping_timeout_s);
            }
        }
        
        be_pushbool(vm, true);
        be_return (vm);
    }
    
    ESP_LOGI(TAG, "Init WebSocket server");
    if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
        const char* path = be_tostring(vm, 1);
        ESP_LOGI(TAG, "Starting WebSocket server on path '%s'", path);
        
        // Get optional http_handle parameter
        bool use_existing_handle = false;
        if (be_top(vm) >= 2) {
            // First try as comptr
            if (be_iscomptr(vm, 2)) {
                void* http_handle = be_tocomptr(vm, 2);
                use_existing_handle = true;
                if (http_handle == NULL) {
                    ESP_LOGE(TAG, "HTTP server handle is NULL");
                    be_pushbool(vm, false);
                    be_return (vm);
                }
                
                // Assign to global ws_server variable after validating it's not NULL
                ws_server = http_handle;
                ESP_LOGD(TAG, "Using existing HTTP server handle (comptr): %p", ws_server);
            } 
            // If not comptr, try to get httpd_handle_t from Berry externally
            else {
                // First log that we're attempting to get the handle
                ESP_LOGD(TAG, "Attempting to retrieve HTTP server handle via be_httpserver_get_handle()");
                
                // Try to get the handle from the HTTP server
                httpd_handle_t handle = be_httpserver_get_handle();
                ESP_LOGD(TAG, "Got HTTP server handle: %p", handle);
                if (handle) {
                    use_existing_handle = true;
                    ws_server = handle;
                    ESP_LOGD(TAG, "Using HTTP server handle from httpserver module: %p", ws_server);
                } else {
                    ESP_LOGE(TAG, "HTTP server module returned NULL handle");
                }
            }
        }
        
        // Get optional ping_interval parameter (0 = disabled)
        ping_interval_s = 5;  // Default: 5 seconds
        ping_timeout_s = 10;  // Default: 10 seconds
        
        if (be_top(vm) >= 3 && be_isint(vm, 3)) {
            ping_interval_s = be_toint(vm, 3);
            
            // Optional ping_timeout parameter
            if (be_top(vm) >= 4 && be_isint(vm, 4)) {
                ping_timeout_s = be_toint(vm, 4);
            }
            
            ESP_LOGI(TAG, "WebSocket ping configured: interval=%ds, timeout=%ds", 
                     ping_interval_s, ping_timeout_s);
        }
        
        // Initialize the client array
        init_clients();
        
        // Handle HTTP server setup
        if (use_existing_handle) {
            // If using an existing handle, just register our disconnect handler
            ESP_LOGI(TAG, "Using existing HTTP server, skipping server creation");
            ESP_LOGI(TAG, "Registering disconnect handler with existing server");
            be_httpserver_set_disconnect_handler(http_server_disconnect_handler);
        } else {
            // Create a new HTTP server
            ESP_LOGI(TAG, "No HTTP server provided, creating a new one");
            // Configure and start the HTTP server
            httpd_config_t config = HTTPD_DEFAULT_CONFIG();
            config.server_port = 8080;  // Use a different port from Tasmota web server
            config.max_open_sockets = MAX_WS_CLIENTS + 2;  // +2 for admin connections
            config.max_uri_handlers = 2;  // For WebSocket and potential health check
            config.lru_purge_enable = true;  // Enable LRU connection purging
            config.recv_wait_timeout = 10;  // 10 seconds timeout for receiving
            config.send_wait_timeout = 10;  // 10 seconds timeout for sending
            config.core_id = 1;           // Run on second core for better performance isolation
            
            // Set disconnect handler
            config.close_fn = http_server_disconnect_handler;
            ESP_LOGD(TAG, "Registering disconnect handler for new WebSocket server");
            
            ESP_LOGI(TAG, "Starting new HTTP server on port %d", config.server_port);
            esp_err_t ret = httpd_start(&ws_server, &config);
            if (ret != ESP_OK) {
                ESP_LOGE(TAG, "Failed to start server: %d (0x%x)", ret, ret);
                wsserver_running = false;
                ws_server = NULL;
                be_pushbool(vm, false);
                be_return (vm);
            }
            ESP_LOGI(TAG, "New HTTP server started on port %d", config.server_port);
        }
        
        // Register URI handler for WebSocket endpoint
        httpd_uri_t ws_uri = {
            .uri        = path,
            .method     = HTTP_GET,
            .handler    = ws_handler,  // Use the connect handler which properly registers clients
            .user_ctx   = NULL,
            .is_websocket = true,
            .handle_ws_control_frames = true  // Allow ESP-IDF to properly handle control frames
        };
        
        ESP_LOGI(TAG, "Registering WebSocket handler for '%s'", path);
        esp_err_t ret = httpd_register_uri_handler(ws_server, &ws_uri);
        if (ret != ESP_OK) {
            // Check if the error is just that the handler already exists
            if (ret == ESP_ERR_HTTPD_HANDLER_EXISTS) {
                ESP_LOGW(TAG, "WebSocket handler for '%s' already exists, continuing", path);
                // This is actually OK, just continue
            } else {
                ESP_LOGE(TAG, "Failed to register URI handler: %d (0x%x)", ret, ret);
                // Only stop the server if we created it
                if (!use_existing_handle) {
                    httpd_stop(ws_server);
                } else {
                    // When using an existing handle, we need to be careful not to nullify it
                    // since it's managed externally
                    ESP_LOGI(TAG, "Using existing HTTP server, not stopping it despite URI registration failure");
                }
                wsserver_running = false;  // Mark as not running
                be_pushbool(vm, false);
                be_return (vm);
            }
        }
                
        // Set up ping timer if enabled - direct conversion from seconds to microseconds
        if (ping_interval_s > 0) {
            ESP_LOGI(TAG, "Starting ping timer with interval %ds", ping_interval_s);
            esp_timer_handle_t ping_timer = NULL;
            esp_timer_create_args_t timer_args = {
                .callback = ws_ping_timer_callback,
                .name = "ws_ping"
            };
            esp_timer_create(&timer_args, &ping_timer);
            esp_timer_start_periodic(ping_timer, ping_interval_s * 1000000); // seconds to microseconds
        }
        
        wsserver_running = true;
        ESP_LOGI(TAG, "WebSocket server started successfully");
        be_pushbool(vm, true);
        be_return (vm);
    }
    
    ESP_LOGE(TAG, "Invalid path parameter");
    be_pushbool(vm, false);
    be_return (vm);
}

static int w_wsserver_client_info(bvm *vm) {
    int initial_top = be_top(vm);  // Save initial stack position for debugging
    
    ESP_LOGI(TAG, "client_info: Initial stack top: %d", initial_top);
    
    if (be_top(vm) >= 1 && be_isint(vm, 1)) {
        int client_slot = be_toint(vm, 1);
        
        if (!is_client_valid(client_slot)) {
            ESP_LOGE(TAG, "client_info: Invalid client %d, returning nil", client_slot);
            be_pushnil(vm);
            
            // Check stack for consistency
            int final_top = be_top(vm);
            ESP_LOGI(TAG, "client_info: Invalid client path - Final stack: %d (expected %d)", 
                     final_top, initial_top + 1);
            be_return (vm);
        }
        
        // Calculate time since last activity
        int64_t now = esp_timer_get_time();
        int64_t inactive_time_ms = ((now / 1000) - ws_clients[client_slot].last_activity) / 1000; // convert to milliseconds
        
        // Create map with client info - this adds one item to the stack
        be_newmap(vm);
        int after_map_top = be_top(vm);
        ESP_LOGI(TAG, "client_info: After map creation - Stack: %d", after_map_top);
        
        // Add client socket fd - Key
        be_pushstring(vm, "socket");
        // Add client socket fd - Value
        be_pushint(vm, ws_clients[client_slot].sockfd);
        // Insert into map - consumes the key and value, leaving map on stack
        be_data_insert(vm, -3);
        // No need for explicit pop here as be_data_insert consumes key and value
        
        // Add last activity timestamp - Key
        be_pushstring(vm, "last_activity");
        // Add last activity timestamp - Value
        be_pushint(vm, (int)(ws_clients[client_slot].last_activity / 1000000)); // seconds
        // Insert into map - consumes the key and value, leaving map on stack
        be_data_insert(vm, -3);
        // No need for explicit pop here as be_data_insert consumes key and value
        
        // Add inactivity duration - Key
        be_pushstring(vm, "inactive_ms");
        // Add inactivity duration - Value
        be_pushint(vm, (int)inactive_time_ms);
        // Insert into map - consumes the key and value, leaving map on stack
        be_data_insert(vm, -3);
        // No need for explicit pop here as be_data_insert consumes key and value
        
        // At this point there should be exactly one item on the stack (the map)
        // beyond what was there when we started
        int final_top = be_top(vm);
        ESP_LOGI(TAG, "client_info: Final stack: %d (expected %d)", final_top, initial_top + 1);
        
        // The map is already on the stack as our return value
        be_return (vm);
    }
    
    // If we reach here, either there were no parameters or the parameter was not an integer
    ESP_LOGI(TAG, "client_info: Invalid parameters, returning nil");
    be_pushnil(vm);
    
    // Check stack for consistency
    int final_top = be_top(vm);
    ESP_LOGI(TAG, "client_info: Error path - Final stack: %d (expected %d)", 
             final_top, initial_top + 1);
    be_return (vm);
}

static int w_wsserver_count_clients(bvm *vm) {
    int initial_top = be_top(vm);
    ESP_LOGI(TAG, "wsserver_count_clients: Initial stack top: %d", initial_top);
    
    int count = 0;
    for (int i = 0; i < MAX_WS_CLIENTS; i++) {
        if (ws_clients[i].active) {
            count++;
        }
    }
    
    be_pushint(vm, count);
    
    int final_top = be_top(vm);
    ESP_LOGI(TAG, "wsserver_count_clients: Final stack: %d (expected %d), returning %d clients", 
             final_top, initial_top + 1, count);
    be_return (vm);
}

static int w_wsserver_send(bvm *vm) {
    int initial_top = be_top(vm);
    ESP_LOGI(TAG, "wsserver_send: Initial stack top: %d", initial_top);
    
    // First validate parameters
    if (be_top(vm) < 2 || !be_isint(vm, 1)) {
        ESP_LOGE(TAG, "Invalid parameters for send");
        be_pushbool(vm, false);
        
        int final_top = be_top(vm);
        ESP_LOGI(TAG, "wsserver_send: Error path - Final stack: %d (expected %d)", 
                 final_top, initial_top + 1);
        be_return (vm);
    }
    
    int client_slot = be_toint(vm, 1);
    size_t len = 0;
    const char* data = NULL;
    uint8_t *data_copy = NULL;
    
    // First check if client is valid before processing the message
    if (!is_client_valid(client_slot)) {
        ESP_LOGE(TAG, "Invalid client ID: %d", client_slot);
        be_pushbool(vm, false);
        
        int final_top = be_top(vm);
        ESP_LOGI(TAG, "wsserver_send: Invalid client - Final stack: %d (expected %d)", 
                 final_top, initial_top + 1);
        be_return (vm);
    }

    // Check if we're dealing with a string or bytes object
    bool is_valid_data = false;
    bool is_bytes = false;
    
    if (be_isstring(vm, 2)) {
        is_valid_data = true;
    } else if (be_isbytes(vm, 2)) {
        is_valid_data = true;
        is_bytes = true;
    }
    
    if (!is_valid_data) {
        ESP_LOGE(TAG, "Data must be a string or bytes object");
        be_pushbool(vm, false);
        
        int final_top = be_top(vm);
        ESP_LOGI(TAG, "wsserver_send: Invalid data type - Final stack: %d (expected %d)", 
                 final_top, initial_top + 1);
        be_return (vm);
    }
    
    // Get data and length based on type
    if (is_bytes) {
        // It's a bytes object - get raw length
        data = be_tobytes(vm, 2, &len);
        ESP_LOGI(TAG, "Got bytes object with length: %d", (int)len);
    } else {
        // For normal strings, get the length from Berry
        len = be_strlen(vm, 2);
        data = be_tostring(vm, 2);
        ESP_LOGI(TAG, "Got string with length: %d", (int)len);
    }
    
    if (len == 0 || data == NULL) {
        ESP_LOGE(TAG, "Invalid data (empty or NULL)");
        be_pushbool(vm, false);
        
        int final_top = be_top(vm);
        ESP_LOGI(TAG, "wsserver_send: Empty data - Final stack: %d (expected %d)", 
                 final_top, initial_top + 1);
        be_return (vm);
    }
    
    // Check for optional binary flag parameter
    bool is_binary = false;
    if (be_top(vm) >= 3 && be_isbool(vm, 3)) {
        is_binary = be_tobool(vm, 3);
    }
    
    ESP_LOGI(TAG, "Sending %d bytes to client %d (type: %s)", 
            len, client_slot, is_binary ? "binary" : "text");
    
    // Client was already checked at the start, but double check again for safety
    if (!is_client_valid(client_slot)) {
        ESP_LOGE(TAG, "Client ID %d became invalid during processing", client_slot);
        be_pushbool(vm, false);
        
        int final_top = be_top(vm);
        ESP_LOGI(TAG, "wsserver_send: Client became invalid - Final stack: %d (expected %d)", 
                 final_top, initial_top + 1);
        be_return (vm);
    }
    
    httpd_ws_frame_t ws_pkt = {0};
    
    // Create a copy of the data to ensure it remains valid during async operation
    data_copy = (uint8_t *)malloc(len);
    if (data_copy == NULL) {
        ESP_LOGE(TAG, "Failed to allocate memory for data copy of size %d", len);
        be_pushbool(vm, false);
        
        int final_top = be_top(vm);
        ESP_LOGI(TAG, "wsserver_send: Memory allocation failed - Final stack: %d (expected %d)", 
                 final_top, initial_top + 1);
        be_return (vm);
    }
    
    // Copy the data
    memcpy(data_copy, data, len);
    
    ws_pkt.payload = data_copy;
    ws_pkt.len = len;
    ws_pkt.type = is_binary ? HTTPD_WS_TYPE_BINARY : HTTPD_WS_TYPE_TEXT;
    ws_pkt.final = true;
    ws_pkt.fragmented = false;
    
    // Get the client's socket
    int sockfd = ws_clients[client_slot].sockfd;
    
    // Track last send attempt for this client
    int64_t now = esp_timer_get_time();
    ws_clients[client_slot].last_activity = now;
    
    // Send the frame asynchronously
    esp_err_t ret = httpd_ws_send_frame_async(ws_server, sockfd, &ws_pkt);
    
    // Free the data copy regardless of send result
    free(data_copy);
    
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to send message to client %d: %d (0x%x)", 
                client_slot, ret, ret);
        
        // Check if this is a fatal error indicating disconnection
        if (ret == ESP_ERR_HTTPD_INVALID_REQ || ret == ESP_ERR_HTTPD_RESP_SEND) {
            ESP_LOGE(TAG, "Fatal error sending to client %d, removing client", client_slot);
            
            // Trigger session close
            handle_client_disconnect(client_slot);
        }
        
        be_pushbool(vm, false);
        
        int final_top = be_top(vm);
        ESP_LOGI(TAG, "wsserver_send: Send failed - Final stack: %d (expected %d)", 
                 final_top, initial_top + 1);
        be_return (vm);
    }
    
    // Success!
    ESP_LOGI(TAG, "Successfully sent message to client %d", client_slot);
    be_pushbool(vm, true);
    
    int final_top = be_top(vm);
    ESP_LOGI(TAG, "wsserver_send: Success - Final stack: %d (expected %d)", 
             final_top, initial_top + 1);
    be_return (vm);
}

static int w_wsserver_close(bvm *vm) {
    if (be_top(vm) >= 1 && be_isint(vm, 1)) {
        int client_slot = be_toint(vm, 1);
        
        ESP_LOGI(TAG, "Closing client %d", client_slot);
        
        if (!is_client_valid(client_slot)) {
            ESP_LOGE(TAG, "Invalid client ID: %d", client_slot);
            be_pushbool(vm, false);
            be_return (vm);
        }
        
        // Send a close frame
        httpd_ws_frame_t ws_pkt = {0};
        ws_pkt.type = HTTPD_WS_TYPE_CLOSE;
        ws_pkt.len = 0;
        
        int sockfd = ws_clients[client_slot].sockfd;
        esp_err_t ret = httpd_ws_send_frame_async(ws_server, sockfd, &ws_pkt);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to send close frame: %d", ret);
        }
        
        // Trigger session close
        handle_client_disconnect(client_slot);
        
        be_pushbool(vm, (ret == ESP_OK));
        be_return (vm);
    }
    
    ESP_LOGE(TAG, "Invalid parameters for close");
    be_pushbool(vm, false);
    be_return (vm);
}

static int w_wsserver_on(bvm *vm) {
    // Save initial stack position for balance checking
    int initial_top = be_top(vm);
    
    ESP_LOGI(TAG, "[ON-STACK] Initial stack position: %d", initial_top);
    
    if (be_top(vm) >= 2 && be_isint(vm, 1) && 
        (be_isfunction(vm, 2) || be_isclosure(vm, 2))) {  // Accept both function and closure types
        
        // Extract event type value (we'll need this in both phases)
        int event_type = be_toint(vm, 1);
        
        // Validate event type early
        if (event_type < 0 || event_type >= 3) {
            ESP_LOGE(TAG, "[ON-ERROR] Invalid event type: %d", event_type);
            be_pushbool(vm, false);
            be_return (vm);
        }

        // Log function type for debugging
        const char *type_str = be_isfunction(vm, 2) ? "function" : "closure";
        ESP_LOGI(TAG, "[ON-STACK] Registering %s callback (type %d)", type_str, event_type);
        
        // Make a safe copy of the function value - CRITICAL for stack stability
        bvalue func_copy = *be_indexof(vm, 2);
        bool is_gc_obj = be_isgcobj(&func_copy);

        // If there's already an active callback, unmark it for GC first
        if (wsserver_callbacks[event_type].active) {
            if (be_isgcobj(&wsserver_callbacks[event_type].func)) {
                ESP_LOGI(TAG, "[ON-GC] Unmarking existing callback for event %d", event_type);
                be_gc_fix_set(vm, wsserver_callbacks[event_type].func.v.gc, bfalse);
            }
        }
        
        // Store the callback information in our global registry
        wsserver_callbacks[event_type].vm = vm;
        wsserver_callbacks[event_type].active = true;
        wsserver_callbacks[event_type].func = func_copy;  // Store our copied function
        
        // Protect function from garbage collection if needed
        if (is_gc_obj) {
            ESP_LOGI(TAG, "[ON-GC] Marking callback as protected from GC for event %d", event_type);
            be_gc_fix_set(vm, func_copy.v.gc, btrue);
        }
        
        // Log event information
        const char* event_name = 
            event_type == WSSERVER_EVENT_CONNECT ? "connect" :
            event_type == WSSERVER_EVENT_DISCONNECT ? "disconnect" : "message";
        
        ESP_LOGI(TAG, "[ON-EVENT] Registered %s callback (event %d)", event_name, event_type);
        
        // Return success
        be_pushbool(vm, true);
        be_return (vm);
    }
    
    ESP_LOGE(TAG, "[ON-ERROR] Invalid parameters for on");
    be_pushbool(vm, false);
    be_return (vm);
}

static int w_wsserver_is_connected(bvm *vm) {
    int initial_top = be_top(vm);
    ESP_LOGI(TAG, "wsserver_is_connected: Initial stack top: %d", initial_top);
    
    if (be_top(vm) >= 1 && be_isint(vm, 1)) {
        int client_slot = be_toint(vm, 1);
        bool valid = is_client_valid(client_slot);
        ESP_LOGI(TAG, "Checking if client %d is connected: %s", client_slot, valid ? "yes" : "no");
        be_pushbool(vm, valid);
        
        int final_top = be_top(vm);
        ESP_LOGI(TAG, "wsserver_is_connected: Success - Final stack: %d (expected %d)", 
                 final_top, initial_top + 1);
        be_return (vm);
    }
    
    ESP_LOGE(TAG, "Invalid parameters for is_connected");
    be_pushbool(vm, false);
    
    int final_top = be_top(vm);
    ESP_LOGI(TAG, "wsserver_is_connected: Invalid parameters - Final stack: %d (expected %d)", 
             final_top, initial_top + 1);
    be_return (vm);
}

static int w_wsserver_stop(bvm *vm) {
    if (!wsserver_running) {
        ESP_LOGI(TAG, "WebSocket server not running");
        be_pushbool(vm, true);
        
        int final_top = be_top(vm);
        ESP_LOGI(TAG, "wsserver_stop: Server not running - Final stack: %d (expected %d)", 
                 final_top, 1);
        be_return (vm);
    }
    
    ESP_LOGI(TAG, "Stopping WebSocket server");
    
    // ---- PHASE 1: Prepare data and handle Berry resources ----
    // First clear callbacks to release GC holds
    for (int i = 0; i < 3; i++) {
        if (wsserver_callbacks[i].active) {
            if (be_isgcobj(&wsserver_callbacks[i].func)) {
                ESP_LOGI(TAG, "Unmarking callback for event %d from GC protection", i);
                be_gc_fix_set(vm, wsserver_callbacks[i].func.v.gc, bfalse);
            }
        }
    }
    
    // ---- PHASE 2: External system operations ----
    // Close all client connections
    int client_count = 0;
    for (int i = 0; i < MAX_WS_CLIENTS; i++) {
        if (ws_clients[i].active) {
            client_count++;
            
            // Send close frame
            httpd_ws_frame_t ws_pkt = {0};
            ws_pkt.type = HTTPD_WS_TYPE_CLOSE;
            ws_pkt.len = 0;
            
            ESP_LOGI(TAG, "Sending close frame to client %d (socket: %d)", 
                    i, ws_clients[i].sockfd);
            
            httpd_ws_send_frame_async(ws_server, ws_clients[i].sockfd, &ws_pkt);
            
            // Trigger session close
            handle_client_disconnect(i);
        }
    }
    
    // Log how many clients were closed
    ESP_LOGI(TAG, "Closed connections to %d client(s)", client_count);
    
    // Stop HTTP server
    ESP_LOGI(TAG, "Stopping HTTP server");
    esp_err_t ret = httpd_stop(ws_server);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to stop server: %d (0x%x)", ret, ret);
    } else {
        ESP_LOGI(TAG, "HTTP server stopped successfully");
    }
    
    ws_server = NULL;
    wsserver_running = false;
    
    be_pushbool(vm, (ret == ESP_OK));
    
    int final_top = be_top(vm);
    ESP_LOGI(TAG, "wsserver_stop: Final stack: %d (expected %d), success: %s", 
             final_top, 2, (ret == ESP_OK) ? "true" : "false");
    be_return (vm);
}

// Deinitialize callbacks for VM shutdown
void be_wsserver_cb_deinit(bvm *vm) {
    ESP_LOGI(TAG, "[DEINIT] Starting callback deinitialization for VM %p", vm);
    
    // Track all callbacks we're operating on
    int count_active = 0;
    int count_gc_protected = 0;
    
    // Clear all callbacks for this VM instance
    for (int i = 0; i < 3; i++) {
        if (wsserver_callbacks[i].vm == vm) {
            const char* event_name = 
                i == WSSERVER_EVENT_CONNECT ? "connect" :
                i == WSSERVER_EVENT_DISCONNECT ? "disconnect" : "message";
            
            ESP_LOGI(TAG, "[DEINIT] Found active callback for event %d (%s)", i, event_name);
            count_active++;
            
            // Check if it's GC protected
            if (wsserver_callbacks[i].active && be_isgcobj(&wsserver_callbacks[i].func)) {
                ESP_LOGI(TAG, "[DEINIT-GC] Callback for event %d is GC protected, unmarking", i);
                count_gc_protected++;
                
                // Log the type of the function before unmarking
                int func_type = wsserver_callbacks[i].func.type;
                const char *type_str = 
                    func_type == BE_FUNCTION ? "function" :
                    func_type == BE_CLOSURE ? "closure" :
                    func_type == BE_NTVCLOS ? "native_closure" : "other";
                ESP_LOGI(TAG, "[DEINIT-GC] Callback type: %s (%d)", type_str, func_type);
                
                // Unmark from garbage collection
                be_gc_fix_set(vm, wsserver_callbacks[i].func.v.gc, bfalse);
                ESP_LOGI(TAG, "[DEINIT-GC] Successfully unmarked callback for event %d", i);
            } else if (wsserver_callbacks[i].active) {
                ESP_LOGI(TAG, "[DEINIT-GC] Callback for event %d is not a GC object, no need to unmark", i);
            }
            
            // Mark as inactive
            wsserver_callbacks[i].active = false;
            ESP_LOGI(TAG, "[DEINIT] Deactivated callback for event %d", i);
        }
    }
    
    ESP_LOGI(TAG, "[DEINIT] Completed callback deinitialization: %d active callbacks, %d GC protected", 
             count_active, count_gc_protected);
}

// Handle disconnect event from the HTTP server
static void http_server_disconnect_handler(void* arg, int sockfd) {
    ESP_LOGI(TAG, "HTTP server disconnect handler called for socket %d", sockfd);
    
    // Find the client slot for this socket
    int client_slot = find_client_by_fd(sockfd);
    if (client_slot < 0) {
        ESP_LOGI(TAG, "No WebSocket client found for socket %d", sockfd);
        return;
    }
    
    ESP_LOGI(TAG, "Found WebSocket client %d for socket %d", client_slot, sockfd);
    
    // Queue disconnect event for processing in main task
    if (httpserver_has_queue()) {
        // Mark client as inactive BEFORE queuing the event
        // This ensures the event processor knows it's a disconnect event
        ws_clients[client_slot].active = false;
        
        bool queue_result = httpserver_queue_message(HTTP_MSG_WEBSOCKET, client_slot, 
                                           NULL, 0, NULL);
        
        if (queue_result) {
            ESP_LOGI(TAG, "WebSocket disconnect event successfully queued from HTTP handler");
        } else {
            ESP_LOGE(TAG, "Failed to queue WebSocket disconnect event from HTTP handler!");
            // Fallback to direct handling
            handle_client_disconnect(client_slot);
        }
    } else {
        ESP_LOGW(TAG, "No queue available for disconnect event - fallback to direct processing");
        handle_client_disconnect(client_slot);
    }
}

// Disconnect handler for httpd_register_uri_handler
esp_err_t websocket_disconnect_handler(httpd_handle_t hd, int sockfd) {
    ESP_LOGI(TAG, "WebSocket disconnect handler called for socket %d", sockfd);
    
    int client_slot = find_client_by_fd(sockfd);
    if (client_slot >= 0) {
        // Call Berry disconnect callback directly
        handle_client_disconnect(client_slot);
    }
    
    return ESP_OK;
}

// Module definition
/* @const_object_info_begin
module wsserver (scope: global, strings: weak) {
    CONNECT, int(WSSERVER_EVENT_CONNECT)
    DISCONNECT, int(WSSERVER_EVENT_DISCONNECT)
    MESSAGE, int(WSSERVER_EVENT_MESSAGE)
    
    start, func(w_wsserver_start)
    stop, func(w_wsserver_stop)
    send, func(w_wsserver_send)
    close, func(w_wsserver_close)
    on, func(w_wsserver_on)
    is_connected, func(w_wsserver_is_connected)
    client_info, func(w_wsserver_client_info)
    count_clients, func(w_wsserver_count_clients)
    
    // Constants for supported frame types
    TEXT, int(HTTPD_WS_TYPE_TEXT)
    BINARY, int(HTTPD_WS_TYPE_BINARY)
    
    // Constants for maximum clients
    MAX_CLIENTS, int(MAX_WS_CLIENTS)
}
@const_object_info_end */
#include "be_fixed_wsserver.h"

#endif // USE_BERRY_WSSERVER
