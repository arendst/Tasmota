/*
  be_webfiles_lib.c - Static file server for Berry using ESP-IDF HTTP server

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

#ifdef USE_BERRY_WEBFILES

#ifndef LOG_LOCAL_LEVEL
#define LOG_LOCAL_LEVEL ESP_LOG_INFO
#endif

#include "be_constobj.h"
#include "be_mapping.h"

// ESP-IDF includes
#include "esp_log.h"
#include "esp_http_server.h"
#include "esp_vfs.h"

// External Berry/Tasmota includes
extern httpd_handle_t be_httpserver_get_handle(void);
extern bool httpserver_queue_message(int type, int client_id, 
                                const char* data, size_t len, void* user_data);

// Tag for logging
static const char *TAG = "WEBFILES";

// Default base path for files
static char base_path[64] = "/files";

// URI prefix for the file server
static char uri_prefix[32] = "/";

// Maximum file path length
#define FILE_PATH_MAX 128

// Scratch buffer size for file transfer
#define SCRATCH_BUFSIZE 4096   // 4KB scratch buffer for chunks

// Static buffer for file sending - fixed allocation
static char scratch_buffer[SCRATCH_BUFSIZE];

// MIME Type Mapping
static const struct {
    const char *extension;
    const char *mimetype;
} mime_types[] = {
    {".html", "text/html"},
    {".htm", "text/html"},
    {".js", "application/javascript"},
    {".mjs", "application/javascript"}, // ES modules
    {".css", "text/css"},
    {".png", "image/png"},
    {".jpg", "image/jpeg"},
    {".jpeg", "image/jpeg"},
    {".gif", "image/gif"},
    {".ico", "image/x-icon"},
    {".svg", "image/svg+xml"},
    {".json", "application/json"},
    {".txt", "text/plain"},
    {".md", "text/markdown"},
    {".wasm", "application/wasm"}, // WebAssembly
    {".map", "application/json"}, // Source maps
    {".woff", "font/woff"},
    {".woff2", "font/woff2"},
    {".ttf", "font/ttf"},
    {".otf", "font/otf"},
    {".bin", "application/octet-stream"},
    {NULL, NULL}
};

// Get MIME type based on file extension
static const char* get_mime_type(const char *path) {
    const char *ext = strrchr(path, '.');
    if (!ext) return "text/plain";
    
    for (int i = 0; mime_types[i].extension; i++) {
        if (strcasecmp(mime_types[i].extension, ext) == 0) {
            return mime_types[i].mimetype;
        }
    }
    return "text/plain";
}

// Build full path including base path
static const char* get_full_path(char *dest, const char *uri, size_t destsize) {
    size_t base_len = strlen(base_path);
    size_t prefix_len = strlen(uri_prefix);
    size_t uri_len = strlen(uri);
    
    // Handle query parameters and fragments in URI
    const char *query = strchr(uri, '?');
    if (query) {
        uri_len = query - uri;
    }
    const char *fragment = strchr(uri, '#');
    if (fragment && (!query || fragment < query)) {
        uri_len = fragment - uri;
    }
    
    // Skip the URI prefix to get the relative path
    const char *relative_path = uri;
    if (prefix_len > 1 && strncmp(uri, uri_prefix, prefix_len) == 0) {
        relative_path = uri + prefix_len - 1; // -1 because we want to keep the leading slash
        uri_len -= (prefix_len - 1);
    }
    
    // Check if path will fit in destination buffer
    if (base_len + uri_len + 1 > destsize) {
        ESP_LOGE(TAG, "Path too long");
        return NULL;
    }
    
    // Construct full path
    strcpy(dest, base_path);
    if (base_len > 0 && base_path[base_len-1] == '/' && relative_path[0] == '/') {
        // Avoid double slash
        strlcpy(dest + base_len, relative_path + 1, uri_len);
    } else {
        strlcpy(dest + base_len, relative_path, uri_len + 1);
    }
    
    return dest;
}

// Set content type based on file extension
static void set_content_type_from_file(httpd_req_t *req, const char *filepath) {
    const char* mime_type = get_mime_type(filepath);
    httpd_resp_set_type(req, mime_type);
    
    // Set Cache-Control header for static assets
    // Don't cache HTML, but cache other static assets
    if (strstr(mime_type, "text/html") == NULL) {
        // Cache for 1 hour (3600 seconds)
        httpd_resp_set_hdr(req, "Cache-Control", "max-age=3600");
    } else {
        // Don't cache HTML content
        httpd_resp_set_hdr(req, "Cache-Control", "no-cache");
    }
    
    // Add CORS headers for development convenience
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
}

//checks if a .html, .css, or .js file exists with a .br suffix and serve that Brotli-compressed version instead

static esp_err_t webfiles_handler(httpd_req_t *req) {
  char filepath[FILE_PATH_MAX];
  char compressed_filepath[FILE_PATH_MAX];
  FILE *file = NULL;
  struct stat file_stat;
  bool use_compression = false;
  const char* compression_type = NULL;  // Only Gzip for now

  // Process any URL query parameters if needed
  char *query = strchr(req->uri, '?');
  if (query) {
      ESP_LOGI(TAG, "Request has query params: %s", query);
      *query = '\0'; // Temporarily terminate URI at the query string for path resolution
  }
  
  // Get the full file path from the URI
  if (get_full_path(filepath, req->uri, sizeof(filepath)) == NULL) {
      ESP_LOGE(TAG, "Failed to get file path for URI: %s", req->uri);
      httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "File not found");
      return ESP_FAIL;
  }
  
  // Restore query string if we modified it
  if (query) *query = '?';
  
  ESP_LOGI(TAG, "Requested file: %s", filepath);
  
  // Check if file is .html, .css, .js, or .svg and if a compressed version exists
  const char *ext = strrchr(filepath, '.');
  if (ext && (strcasecmp(ext, ".html") == 0 || strcasecmp(ext, ".css") == 0 ||
               strcasecmp(ext, ".js") == 0 || strcasecmp(ext, ".svg") == 0)) {
      
      // Check what compression formats the client supports
      char accept_encoding[64] = {0};
      if (httpd_req_get_hdr_value_str(req, "Accept-Encoding", accept_encoding, sizeof(accept_encoding)) == ESP_OK) {
          ESP_LOGI(TAG, "Client supports compression: %s", accept_encoding);
          if (!use_compression && strstr(accept_encoding, "gzip") != NULL) {
              // Construct Gzip filepath
              snprintf(compressed_filepath, sizeof(compressed_filepath), "%s.gz", filepath);
              if (stat(compressed_filepath, &file_stat) == 0 && S_ISREG(file_stat.st_mode)) {
                  use_compression = true;
                  compression_type = "gzip";
                  strcpy(filepath, compressed_filepath); // Use the .gz file
                  ESP_LOGI(TAG, "Found Gzip version: %s", filepath);
              }
          }
      }
  }

  // Check if file exists
  if (stat(filepath, &file_stat) != 0) {
      ESP_LOGE(TAG, "File does not exist: %s", filepath);
      httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "File not found");
      return ESP_FAIL;
  }
  
  // Check if it's a regular file
  if (!S_ISREG(file_stat.st_mode)) {
      ESP_LOGE(TAG, "Not a regular file: %s", filepath);
      httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Not a file");
      return ESP_FAIL;
  }
  
  // Open the file for reading
  file = fopen(filepath, "r");
  if (!file) {
      ESP_LOGE(TAG, "Failed to open file: %s", filepath);
      httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to open file");
      return ESP_FAIL;
  }
  
  // Set content type based on file extension (use original path for MIME type if compressed)
  char original_filepath[FILE_PATH_MAX];
  if (use_compression) {
      // Strip compression extension for MIME type detection
      strcpy(original_filepath, filepath);
      char* dot_pos = strrchr(original_filepath, '.');
      if (dot_pos) {
          *dot_pos = '\0';  // Remove compression extension
      }
      set_content_type_from_file(req, original_filepath);
  } else {
      set_content_type_from_file(req, filepath);
  }
  
  // Set compression headers if applicable
  if (use_compression) {
      httpd_resp_set_hdr(req, "Content-Encoding", compression_type);
      httpd_resp_set_hdr(req, "Vary", "Accept-Encoding");
  }

  // Send file in chunks for efficiency
  size_t chunk_size;
  size_t total_sent = 0;
  
  while ((chunk_size = fread(scratch_buffer, 1, SCRATCH_BUFSIZE, file)) > 0) {
      if (httpd_resp_send_chunk(req, scratch_buffer, chunk_size) != ESP_OK) {
          ESP_LOGE(TAG, "File send failed");
          fclose(file);
          httpd_resp_send_chunk(req, NULL, 0);
          return ESP_FAIL;
      }
      total_sent += chunk_size;
  }
  
  // Close file
  fclose(file);
  
  // Finish the HTTP response
  httpd_resp_send_chunk(req, NULL, 0);
  ESP_LOGI(TAG, "File sent successfully (%d bytes, %s)", (int)total_sent, use_compression ? compression_type : "uncompressed");
  
  return ESP_OK;
}

/****************************************************************
 * Berry Interface Functions
 ****************************************************************/

// webfiles.serve(base_path, uri_prefix) -> bool
// Serve files from base_path at uri_prefix
static int w_webfiles_serve(bvm *vm) {
    int initial_top = be_top(vm);
    
    if (be_top(vm) >= 2 && be_isstring(vm, 1) && be_isstring(vm, 2)) {
        const char* path = be_tostring(vm, 1);
        const char* prefix = be_tostring(vm, 2);
        
        ESP_LOGI(TAG, "Setting up file server with base path: %s, uri prefix: %s", path, prefix);
        
        struct stat st;
        if (stat(path, &st) != 0 || !S_ISDIR(st.st_mode)) {
            ESP_LOGE(TAG, "Input path is not a valid directory: %s", path);
            be_pushbool(vm, false);
            // Clean up stack before returning
            while (be_top(vm) > initial_top) {
                be_pop(vm, 1);
            }
            be_return (vm); // Return directly
        }

        httpd_handle_t server = be_httpserver_get_handle();
        if (!server) {
            ESP_LOGE(TAG, "HTTP server not running");
            be_pushbool(vm, false);
            while (be_top(vm) > initial_top) {
                be_pop(vm, 1);
            }
            be_return (vm);
        }

        strlcpy(base_path, path, sizeof(base_path));
        strlcpy(uri_prefix, prefix, sizeof(uri_prefix));
        

        // Use a static buffer for the URI pattern
        static char registered_uri_pattern[64]; // ADD static keyword
        // Ensure it's null-terminated even if snprintf truncates
        registered_uri_pattern[sizeof(registered_uri_pattern) - 1] = '\0';
        snprintf(registered_uri_pattern, sizeof(registered_uri_pattern), "%s*", prefix);

        ESP_LOGI(TAG, "Registering URI handler with pattern: %s", registered_uri_pattern);

        httpd_uri_t uri_handler = {
            // Point to the static buffer
            .uri      = registered_uri_pattern, // Use the static buffer
            .method   = HTTP_GET,
            .handler  = webfiles_handler,
            .is_websocket = false,
            .user_ctx = NULL
        };

        esp_err_t ret = httpd_register_uri_handler(server, &uri_handler);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to register URI handler: %d", ret);
            be_pushbool(vm, false);
            be_pop(vm, be_top(vm) - initial_top);
            be_return (vm);
        }
        
        be_pushbool(vm, true);
        be_pop(vm, be_top(vm) - initial_top);
        be_return (vm);
    }
    
    be_pushbool(vm, false);
    be_pop(vm, be_top(vm) - initial_top);
    be_return (vm);
}

// webfiles.serve_file(file_path, uri) -> bool
// Serve a specific file at a specific URI
static int w_webfiles_serve_file(bvm *vm) {
    int initial_top = be_top(vm);
    
    if (be_top(vm) >= 2 && be_isstring(vm, 1) && be_isstring(vm, 2)) {
        const char* file_path = be_tostring(vm, 1);
        const char* uri = be_tostring(vm, 2);
        
        // Check if file exists
        struct stat file_stat;
        if (stat(file_path, &file_stat) == -1) {
            ESP_LOGE(TAG, "File not found: %s", file_path);
            be_pushbool(vm, false);
            be_pop(vm, be_top(vm) - initial_top);
            be_return (vm);
        }
        
        // TODO: Implement custom handler for specific files
        // This would require keeping track of file mappings
        
        ESP_LOGW(TAG, "serve_file not yet implemented");
        be_pushbool(vm, false);
        be_pop(vm, be_top(vm) - initial_top);
        be_return (vm);
    }
    
    be_pushbool(vm, false);
    be_pop(vm, be_top(vm) - initial_top);
    be_return (vm);
}

// Module definition
/* @const_object_info_begin
module webfiles (scope: global, strings: weak) {
    serve, func(w_webfiles_serve)
    serve_file, func(w_webfiles_serve_file)
    
    // MIME type constants
    MIME_HTML, str("text/html")
    MIME_JS, str("application/javascript")
    MIME_CSS, str("text/css")
    MIME_JSON, str("application/json")
    MIME_TEXT, str("text/plain")
    MIME_BINARY, str("application/octet-stream")
}
@const_object_info_end */
#include "be_fixed_webfiles.h"

#endif // USE_BERRY_WEBFILES
