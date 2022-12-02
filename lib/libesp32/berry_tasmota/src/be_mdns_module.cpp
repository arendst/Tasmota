/********************************************************************
 * Berry module `mdns`
 * 
 * To use: `import mdns`
 * 
 * MDNS support
 *******************************************************************/
#include "be_constobj.h"
#include "be_mapping.h"
#include "be_mem.h"

#ifdef USE_DISCOVERY
#include "mdns.h"
#include <string.h>

//
// `mdsn.start([hostname:string]) -> nil`
// start or restart mdns, specify hostname or use tasmota.hostname() if none provided (default)
extern char* NetworkHostname(void);
static void m_mdns_start(struct bvm *vm, const char* hostname) {
  esp_err_t err = mdns_init();
  if (err != ESP_OK) {
    be_raisef(vm, "internal_error", "could not initialize mdns err=%i", err);
  }
  if (hostname == NULL) {
    hostname = NetworkHostname();   // revert to default hostname if none is specified
  }
  err = mdns_hostname_set(hostname);
  if (err != ESP_OK) {
    be_raisef(vm, "internal_error", "could not set hostname err=%i", err);
  }
}
BE_FUNC_CTYPE_DECLARE(m_mdns_start, "", "@[s]")

//
// `msdn.stop() -> nil`
// free all mdns resources
static void m_mdns_stop(void) {
  mdns_free();
}
BE_FUNC_CTYPE_DECLARE(m_mdns_stop, "", "")

//
// `mdns.set_hostname(hostname:string) -> nil`
// change the hostname
static void m_mdns_set_hostname(struct bvm *vm, const char * hostname) {
  esp_err_t err = mdns_hostname_set(hostname);
  if (err != ESP_OK) {
    be_raisef(vm, "internal_error", "mdns set_hostname err=%i", err);
  }
}
BE_FUNC_CTYPE_DECLARE(m_mdns_set_hostname, "", "@s")

//
// `mdns.add_service(service:string, proto:string, port:int, txt:map) -> nil`
//
// add a service declaration using the current hostname as instance name, and specify TXT fields as a `map`
//
// Test:
//    import mdns mdns.add_service("_arduino","_tcp",1111, {"board":"tasmota", "tcp_check":"no", "ssh_upload":"no", "auth_upload":"no"})
//
//    import mdns mdns.add_service("_matterc","_udp", 5540, {"VP":"65521+32768", "SII":5000, "SAI":300, "T":1, "D":3840, "CM":1, "PH":33, "PI":""})
static int32_t m_mdns_add_service(struct bvm *vm) {
  int32_t top = be_top(vm);
  if (top >= 3 && be_isstring(vm, 1) && be_isstring(vm, 2) && be_isint(vm, 3)) {
    const char* service_type = be_tostring(vm, 1);
    const char* proto = be_tostring(vm, 2);
    uint16_t port = be_toint(vm, 3);
    
    mdns_txt_item_t * txt_items = NULL;
    int32_t txt_num = 0;
    if (top >= 4 && be_ismapinstance(vm, 4)) {
      // parse txt map
        be_getmember(vm, 4, ".p");
      int32_t map_len = be_data_size(vm, -1);
      if (map_len > 0) {
        uint32_t i= 0;
        txt_items = (mdns_txt_item_t*) be_os_malloc(sizeof(mdns_txt_item_t) * map_len);
        if (txt_items != NULL) {
          be_pushiter(vm, -1); /* map iterator use 1 register */
          while (be_iter_hasnext(vm, -2) && i < map_len) {
            be_iter_next(vm, -2);
            const char* key = be_tostring(vm, -2);
            const char* val = be_tostring(vm, -1);
            size_t key_len = strlen(key)+1;
            txt_items[i].key = (const char*)be_os_malloc(key_len);
            if (txt_items[i].key) { strcpy((char*)txt_items[i].key, key); }
            size_t val_len = strlen(val)+1;
            txt_items[i].value = (const char*)be_os_malloc(val_len);
            if (txt_items[i].value) { strcpy((char*)txt_items[i].value, val); }
            be_pop(vm, 2);
            i++;
          }
          txt_num = i;
        } else {
          txt_num = 0;    // failed to allocate, pretend it's empty
        }
        be_pop(vm, 1); /* pop iterator */
      }
    }
    esp_err_t err = mdns_service_add(NULL, service_type, proto, port, txt_items, txt_num);
    // free all allocated memory
    if (txt_items != NULL) {
      for (uint32_t i = 0; i < txt_num; i++) {
        if (txt_items[i].key != NULL) { be_os_free((void*)txt_items[i].key); }
        if (txt_items[i].value != NULL) { be_os_free((void*)txt_items[i].value); }
      }
      be_os_free(txt_items);
    }
    if (err != ESP_OK) {
      be_raisef(vm, "internal_error", "mdns service_add err=%i", err);
    }
    be_return_nil(vm);
  }
  be_raise(vm, "value_error", "wrong or missing arguments");
}

/* @const_object_info_begin
module mdns (scope: global) {
    start, ctype_func(m_mdns_start)
    stop, ctype_func(m_mdns_stop)
    set_hostname, ctype_func(m_mdns_set_hostname)
    add_service, func(m_mdns_add_service)
}
@const_object_info_end */
#include "be_fixed_mdns.h"

#endif // USE_DISCOVERY