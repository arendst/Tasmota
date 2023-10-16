/*
  xdrv_52_3_berry_mdns.ino - Berry scripting language, mDNS

  Copyright (C) 2021 Stephan Hadinger, Berry language by Guan Wenliang https://github.com/Skiars/berry

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

// also includes tcp_client

#ifdef USE_BERRY
#ifdef USE_DISCOVERY

#include "mdns.h"
#include <string.h>
#include "IPAddress.h"

extern "C" {
  //
  // `mdsn.start([hostname:string]) -> nil`
  // start or restart mdns, specify hostname or use tasmota.hostname() if none provided (default)
  void m_mdns_start(struct bvm *vm, const char* hostname) {
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
  void m_mdns_stop(void) {
    mdns_free();
  }
  BE_FUNC_CTYPE_DECLARE(m_mdns_stop, "", "")

  //
  // `mdns.set_hostname(hostname:string) -> nil`
  // change the hostname
  void m_mdns_set_hostname(struct bvm *vm, const char * hostname) {
    esp_err_t err = mdns_hostname_set(hostname);
    if (err != ESP_OK) {
      be_raisef(vm, "internal_error", "mdns set_hostname err=%i", err);
    }
  }
  BE_FUNC_CTYPE_DECLARE(m_mdns_set_hostname, "", "@s")

  //
  // `mdns.add_service(service:string, proto:string, port:int, txt:map [, instance:string, hostname:string]) -> nil`
  //
  // add a service declaration using the current hostname as instance name, and specify TXT fields as a `map`
  //
  // Test:
  //    import mdns mdns.add_service("_arduino","_tcp",1111, {"board":"tasmota", "tcp_check":"no", "ssh_upload":"no", "auth_upload":"no"})
  //
  //    import mdns mdns.add_service("_matterc","_udp", 5540, {"VP":"65521+32768", "SII":5000, "SAI":300, "T":1, "D":3840, "CM":1, "PH":33, "PI":""})
  int32_t m_mdns_add_service(struct bvm *vm) {
    int32_t top = be_top(vm);
    if (top >= 3 && be_isstring(vm, 1) && be_isstring(vm, 2) && be_isint(vm, 3)) {
      const char* service_type = be_tostring(vm, 1);
      const char* proto = be_tostring(vm, 2);
      uint16_t port = be_toint(vm, 3);
      const char * instance = nullptr;
      const char * hostname = nullptr;
      if (top >= 5 && be_isstring(vm, 5)) {
        instance = be_tostring(vm, 5);
      }
      if (top >= 6 && be_isstring(vm, 6)) {
        hostname = be_tostring(vm, 6);
      }
      
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
      esp_err_t err;
      if (hostname == nullptr) {
        err = mdns_service_add(instance, service_type, proto, port, txt_items, txt_num);
      } else {
        err = mdns_service_add_for_host(instance, service_type, proto, hostname, port, txt_items, txt_num);
      }
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


  //
  // `mdns.remove_service(service:string, proto:string [, instance:string, hostname:string]) -> nil`
  //
  // remove service from mDNS server with hostname.
  //
  // Test:
  //    import mdns mdns.remove_service("_arduino","_tcp")
  //
  //    import mdns mdns.remove_service("_matterc","_udp")
  int32_t m_mdns_remove_service(struct bvm *vm) {
    int32_t top = be_top(vm);
    if (top >= 2 && be_isstring(vm, 1) && be_isstring(vm, 2)) {
      const char* service_type = be_tostring(vm, 1);
      const char* proto = be_tostring(vm, 2);
      const char * instance = nullptr;
      const char * hostname = nullptr;
      if (top >= 3 && be_isstring(vm, 3)) {
        instance = be_tostring(vm, 3);
      }
      if (top >= 4 && be_isstring(vm, 4)) {
        hostname = be_tostring(vm, 4);
      }

      esp_err_t err;
      if (hostname == nullptr) {
        err = mdns_service_remove(service_type, proto);
      } else {
        err = mdns_service_remove_for_host(instance, service_type, proto, hostname);
      }
      if (err != ESP_OK) {
        be_raisef(vm, "internal_error", "mdns service_remove err=%i", err);
      }
      be_return_nil(vm);
    }
    be_raise(vm, "value_error", "wrong or missing arguments");
  }

  // `mdns_service_subtype_add_for_host()` is only available in most recent esp-protocols version
  //
  // This alias makes sure that the compilation succeeds even if the function is not available
  //
  extern "C" esp_err_t __tasmota_mdns_service_subtype_add_for_host (const char *instance_name, const char *service_type, const char *proto, const char *hostname, const char *subtype)
  { return ESP_OK; }
  extern "C" esp_err_t mdns_service_subtype_add_for_host(const char *instance_name, const char *service_type, const char *proto, const char *hostname, const char *subtype) __attribute__ ((weak, alias ("__tasmota_mdns_service_subtype_add_for_host")));

  //
  // `mdns.add_subtype(service:string, proto:string, instance:string, hostname:string, subtype:string) -> nil`
  //
  // add a subtype
  //
  int32_t m_dns_add_subtype(struct bvm *vm) {
    int32_t top = be_top(vm);
    if (top >= 5 && be_isstring(vm, 1) && be_isstring(vm, 2) && be_isstring(vm, 3) && be_isstring(vm, 4) && be_isstring(vm, 5)) {
      const char* service_type = be_tostring(vm, 1);
      const char* proto = be_tostring(vm, 2);
      const char* instance = be_tostring(vm, 3);
      const char* hostname = be_tostring(vm, 4);
      const char* subtype = be_tostring(vm, 5);

      esp_err_t err;
      // Waiting for support of `mdns_service_subtype_add_for_host()`
      err = mdns_service_subtype_add_for_host(instance, service_type, proto, hostname, subtype);
      if (err != ESP_OK) {
        be_raisef(vm, "internal_error", "mdns add_subtype err=%i", err);
      }
      be_return_nil(vm);
    }
    be_raise(vm, "value_error", "wrong or missing arguments");
  }

  //
  // `mdns.add_hostname(hostname:string, ip:string [, ip:string]*) -> nil`
  //
  // add a delegate hostname
  //
  int32_t m_dns_add_hostname(struct bvm *vm) {
    int32_t top = be_top(vm);
    if (top >= 2 && be_isstring(vm, 1) && be_isstring(vm, 2)) {
      mdns_ip_addr_t * head = nullptr;     // head of the linked list of addresses
      esp_err_t err = ESP_OK;             // return status

      const char* hostname = be_tostring(vm, 1);
      const char* ip_text = nullptr;
      for (uint16_t i = 2; i <= top; i++) {
        const char* ip_text = be_tostring(vm, i);
        if (ip_text == nullptr || ip_text[0] == 0) { continue; }    // ignore empty string
        IPAddress ip;
        if (!ip.fromString(ip_text)) { err = -1; break; }

        // allocate new slot
        mdns_ip_addr_t *new_head = (mdns_ip_addr_t*) be_os_malloc(sizeof(mdns_ip_addr_t));
        if (new_head == nullptr) { err = -2; break; }
        new_head->next = head;
        head = new_head;

  #ifdef USE_IPV6
        // 
        ip_addr_t *ip_addr = (ip_addr_t*) ip;
        head->addr.type = ip_addr->type;
        if (ip_addr->type == ESP_IPADDR_TYPE_V6) {
          memcpy(head->addr.u_addr.ip6.addr, ip_addr->u_addr.ip6.addr, 16);
        } else {
          head->addr.u_addr.ip4.addr = ip_addr->u_addr.ip4.addr;
        }
  #else
        head->addr.u_addr.ip4.addr = (uint32_t) ip;
  #endif
      } while (0);

      if (err == ESP_OK && head != nullptr) {
        err = mdns_delegate_hostname_add(hostname, head);
      }

      // deallocate all memory
      while (head != nullptr) {
        mdns_ip_addr_t * next = head->next;
        be_os_free(head);
        head = next;
      }

      if (err == -1) {
        be_raisef(vm, "value_error", "Invalid IP Address '%s'", ip_text);
      } else if (err == -2) {
        be_raise(vm, "memory_error", nullptr);
      } else if (err != ESP_OK) {
        be_raisef(vm, "value_error", "mdns_delegate_hostname_add err=%i", err);
      }
      be_return_nil(vm);

    }
    be_raise(vm, "value_error", "wrong or missing arguments");
  }

  //
  // `mdns.find_service(service:string, proto:string [timeout_ms:int(3000), max_responses:int(20)]) -> map`
  //
  int32_t m_dns_find_service(struct bvm *vm) {
    static const char * ip_protocol_str[] = {"v4", "v6", "max"};
    int32_t top = be_top(vm);
    if (top >= 2 && be_isstring(vm, 1) && be_isstring(vm, 2)) {
      const char* service_name = be_tostring(vm, 1);
      const char* proto = be_tostring(vm, 2);
      int32_t timeout_ms = 3000;
      if (top >= 3 && be_isint(vm, 3)) {
        timeout_ms = be_toint(vm, 3);
      }
      int32_t max_responses = 20;
      if (top >= 4 && be_isint(vm, 4)) {
        max_responses = be_toint(vm, 4);
      }

      mdns_result_t * results = NULL;
      esp_err_t err = mdns_query_ptr(service_name, proto, timeout_ms, max_responses,  &results);
      if (err != ESP_OK) { be_raisef(vm, "value_error", "mdns_query_ptr err=%i", err); }
      if (results == NULL) { be_return_nil(vm); }

      mdns_result_t * r = results;
      mdns_ip_addr_t * a = NULL;
      be_newobject(vm, "list");
      while (r) {
        be_newobject(vm, "map");
        be_map_insert_str(vm, "type", ip_protocol_str[r->ip_protocol]);
        if (r->instance_name) { be_map_insert_str(vm, "instance", r->instance_name); }
        if (r->hostname) { be_map_insert_str(vm, "hostname", r->hostname); }
        // TXT
        be_pushstring(vm, "txt");
        be_newobject(vm, "map");
        for (int32_t t=0; t < r->txt_count; t++){
          be_map_insert_str(vm, r->txt[t].key, r->txt[t].value);
        }
        //
        be_pop(vm, 1);
        be_data_insert(vm, -3);
        be_pop(vm, 2);
        //

        // IP addresses
        be_pushstring(vm, "ip");
        be_newobject(vm, "list");
        //
        for (a = r->addr; a != NULL; a = a->next) {
  #ifdef USE_IPV6
          ip_addr_t ip_addr;
          if (a->addr.type == IPADDR_TYPE_V6) {
            ip_addr_copy_from_ip6(ip_addr, a->addr.u_addr.ip6);
          } else if (a->addr.type == IPADDR_TYPE_V4) {
            ip_addr_copy_from_ip4(ip_addr, a->addr.u_addr.ip4);
          } else {
            continue;
          }
  #else
          uint32_t ip_addr = a->addr.u_addr.ip4.addr;
  #endif
          be_pushstring(vm, IPAddress(ip_addr).toString().c_str());
          be_data_push(vm, -2);
          be_pop(vm, 1);
        }
        //
        be_pop(vm, 1);
        be_data_insert(vm, -3);
        be_pop(vm, 2);

        be_pop(vm, 1);
        be_data_push(vm, -2);
        be_pop(vm, 1);

        r = r->next;
      }
      be_pop(vm, 1);                  // now list is on top

      mdns_query_results_free(results);
      be_return(vm);
    }
    be_raise(vm, "value_error", "wrong or missing arguments");
  }
}

#endif // USE_DISCOVERY
#endif  // USE_BERRY
