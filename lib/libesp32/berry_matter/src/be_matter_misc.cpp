/*
  be_matter_misc.c - misc functions for `matter` module

  Copyright (C) 2023  Stephan Hadinger & Theo Arends

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

/********************************************************************
 * Matter global module
 * 
 *******************************************************************/

#include "be_constobj.h"
#include "be_mapping.h"
#include "IPAddress.h"

static uint8_t ip_bytes[16] = {};
// Convert an IP address from string to raw bytes
extern "C" const void* matter_get_ip_bytes(const char* ip_str, size_t* ret_len) {
  IPAddress ip;
  if (ip.fromString(ip_str)) {
#ifdef USE_IPV6
    if (ip.type() == IPv4) {
      uint32_t ip_32 = ip;
      memcpy(ip_bytes, &ip_32, 4);
      *ret_len = 4;
    } else {
      ip_addr_t ip_addr;
      ip.to_ip_addr_t(&ip_addr);
      memcpy(ip_bytes, &ip_addr.u_addr.ip6, 16);
      *ret_len = 16;
    }
#else
    uint32_t ip_32 = ip;
    memcpy(ip_bytes, &ip_32, 4);
    *ret_len = 4;
#endif
    return ip_bytes;
  } else {
    *ret_len = 0;
    return nullptr;
  }
}
