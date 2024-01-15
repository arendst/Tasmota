/*
  xdrv_52_3_berry_udp.ino - Berry scripting language, UDP client

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


#ifdef USE_WEBCLIENT

// extern int be_udp_init(bvm *vm);
// extern int be_udp_deinit(bvm *vm);
// extern int be_udp_begin(bvm *vm);
// extern int be_udp_begin_mcast(bvm *vm);

#include <Arduino.h>
#include <WiFiUdp.h>
#include "be_mapping.h"

extern bool WifiHostByName(const char* aHostname, IPAddress & aResult);

extern "C" {

  // init()
  WiFiUDP *be_udp_init_ntv(void) {
    WiFiUDP *udp = new WiFiUDP();
    return udp;
  }

  // deinit()
  void *be_udp_deinit_ntv(WiFiUDP *udp) {
    if (udp != nullptr) { delete udp; }
    return nullptr;
  }

  // udp.begin(interface:string, port:int) -> bool
  int32_t be_udp_begin_ntv(WiFiUDP *udp, const char *host, int32_t port) {
#ifdef USE_IPV6
    IPAddress addr(IPv6);
#else
    IPAddress addr;
#endif
    // if no host or host is "" then we defult to INADDR_ANY
    if(host && (*host != 0) && !WifiHostByName(host, addr)){
        return 0;
    }
    return udp->begin(addr, port);
  }

  // udp.stop() -> nil
  void be_udp_stop_ntv(WiFiUDP *udp) {
    udp->stop();
  }

  // udp.begin_multicast(address:string, port:int) -> nil
  int32_t be_udp_begin_mcast_ntv(WiFiUDP *udp, const char *host, int32_t port) {
#ifdef USE_IPV6
    IPAddress addr(IPv6);
#else
    IPAddress addr;
#endif
    if(!WifiHostByName(host, addr)){
        return 0;
    }
    return udp->WiFiUDP::beginMulticast(addr, port);
  }

  // udp.send(address:string, port:int, payload:bytes) -> bool
  int32_t be_udp_send_ntv(WiFiUDP *udp, const char *host, int32_t port, const uint8_t* buf, int32_t len) {
    IPAddress addr;
    if (!WifiHostByName(host, addr)){
        return 0;
    }
    // AddLog(LOG_LEVEL_DEBUG, "BRY: be_udp_send_ntv host '%s'", addr.toString().c_str());
    if (!udp->beginPacket(addr, port)) { return 0; }
    int bw = udp->write(buf, len);
    if (!bw) { return 0; }
    if (!udp->endPacket()) { return 0; }
    return btrue;
  }

  // udp.send_multicast(payload:bytes) -> bool
  int32_t be_udp_send_mcast_ntv(WiFiUDP *udp, const uint8_t* buf, int32_t len) {
    if (!udp->beginMulticastPacket()) { return 0; }
    int bw = udp->write(buf, len);
    if (!bw) { return 0; }
    if (!udp->endPacket()) { return 0; }
    return btrue;
  }

  // udp.read() -> bytes or nil
  int32_t be_udp_read(struct bvm *vm) {
    WiFiUDP *udp = (WiFiUDP*) be_convert_single_elt(vm, 1, NULL, NULL);
    if (udp->parsePacket()) {
      int btr = udp->available();   // btr contains the size of bytes_to_read

      int argc = be_top(vm);
      if (argc >= 2 && be_isbytes(vm, 2)) {
        // we have already a bytes() buffer
        be_pushvalue(vm, 2);    // push on top
        // resize to btr
        be_getmember(vm, -1, "resize");
        be_pushvalue(vm, -2);
        be_pushint(vm, btr);
        be_call(vm, 2);
        be_pop(vm, 3);
      } else {
        be_pushbytes(vm, nullptr, btr); // allocate a buffer of size btr filled with zeros
      }

      // get the address of the buffer
      be_getmember(vm, -1, "_buffer");
      be_pushvalue(vm, -2);
      be_call(vm, 1);
      uint8_t * buf = (uint8_t*) be_tocomptr(vm, -2);
      be_pop(vm, 2);

      int32_t btr2 = udp->read(buf, btr);

      // set remotet ip
      IPAddress remote_ip = udp->remoteIP();
      be_pushstring(vm, remote_ip.toString(true).c_str());
      be_setmember(vm, 1, "remote_ip");
      be_pop(vm, 1);

      // set remote port
      be_pushint(vm, udp->remotePort());
      be_setmember(vm, 1, "remote_port");
      be_pop(vm, 1);

      be_return(vm);
    } else {
      be_return_nil(vm);
    }
  }
}

#endif // USE_WEBCLIENT
#endif  // USE_BERRY
