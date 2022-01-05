/********************************************************************
 * UDP lib
 * 
 * To use: `d = udp()`
 * 
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_WEBCLIENT

// extern int be_udp_init(bvm *vm);
// extern int be_udp_deinit(bvm *vm);
// extern int be_udp_begin(bvm *vm);
// extern int be_udp_begin_mcast(bvm *vm);

#include <Arduino.h>
#include <WiFiGeneric.h>
#include <WiFiUdp.h>
#include "be_mapping.h"

extern "C" {
  
  // init()
  WiFiUDP *be_udp_init_ntv(void) {
    return new WiFiUDP();
  }
  int32_t be_udp_init(struct bvm *vm) {
    return be_call_c_func(vm, (void*) &be_udp_init_ntv, "+.p", "");
  }

  // deinit()
  void *be_udp_deinit_ntv(WiFiUDP *udp) {
    if (udp != nullptr) { delete udp; }
    return nullptr;
  }
  int32_t be_udp_deinit(struct bvm *vm) {
    return be_call_c_func(vm, (void*) &be_udp_deinit_ntv, "=.p", "");
  }

  // udp.begin(address:string, port:int) -> nil
  int32_t be_udp_begin_ntv(WiFiUDP *udp, const char *host, int32_t port) {
    IPAddress addr(INADDR_ANY);
    // if no host or host is "" then we defult to INADDR_ANY (0.0.0.0)
    if(host && (*host != 0) && !WiFiGenericClass::hostByName(host, addr)){
        return 0;
    }
    return udp->begin(addr, port);
  }
  int32_t be_udp_begin(struct bvm *vm) {
    return be_call_c_func(vm, (void*) &be_udp_begin_ntv, "b", ".si");
  }

  // udp.begin_multicast(address:string, port:int) -> nil
  int32_t be_udp_begin_mcast_ntv(WiFiUDP *udp, const char *host, int32_t port) {
    IPAddress addr((uint32_t)0);
    if(!WiFiGenericClass::hostByName(host, addr)){
        return 0;
    }
    return udp->WiFiUDP::beginMulticast(addr, port);
  }
  int32_t be_udp_begin_mcast(struct bvm *vm) {
    return be_call_c_func(vm, (void*) &be_udp_begin_mcast_ntv, "b", ".si");
  }

  // udp.send(address:string, port:int, payload:bytes) -> bool
  int32_t be_udp_send_ntv(WiFiUDP *udp, const char *host, int32_t port, const uint8_t* buf, int32_t len) {
    IPAddress addr((uint32_t)0);
    if (!WiFiGenericClass::hostByName(host, addr)){
        return 0;
    }
    if (!udp->beginPacket(addr, port)) { return 0; }
    int bw = udp->write(buf, len);
    if (!bw) { return 0; }
    if (!udp->endPacket()) { return 0; }
    return btrue;
  }
  int32_t be_udp_send(struct bvm *vm) {
    return be_call_c_func(vm, (void*) &be_udp_send_ntv, "b", ".si(bytes)~");
  }

  // udp.send_multicast(payload:bytes) -> bool
  int32_t be_udp_send_mcast_ntv(WiFiUDP *udp, const uint8_t* buf, int32_t len) {
    if (!udp->beginMulticastPacket()) { return 0; }
    int bw = udp->write(buf, len);
    if (!bw) { return 0; }
    if (!udp->endPacket()) { return 0; }
    return btrue;
  }
  int32_t be_udp_send_mcast(struct bvm *vm) {
    return be_call_c_func(vm, (void*) &be_udp_send_mcast_ntv, "b", ".(bytes)~");
  }

  // udp.read() -> bytes or nil
  int32_t be_udp_read(struct bvm *vm) {
    WiFiUDP *udp = (WiFiUDP*) be_convert_single_elt(vm, 1, NULL, NULL);
    if (udp->parsePacket()) {
      int btr = udp->available();
      uint8_t * buf = (uint8_t*) be_pushbuffer(vm, btr);
      int32_t btr2 = udp->read(buf, btr);
      be_pushbytes(vm, buf, btr2);

      // set remotet ip
      IPAddress remote_ip = udp->remoteIP();
      be_pushstring(vm, remote_ip.toString().c_str());
      be_setmember(vm, 1, "remote_ip");
      be_pop(vm, 1);

      // set remote port
      be_pushint(vm, udp->remotePort());
      be_setmember(vm, 1, "remote_port");
      be_pop(vm, 1);

      be_return(vm);  /* return code */
    } else {
      be_return_nil(vm);
    }
  }

  #include "be_fixed_be_class_udp.h"

  void be_load_udp_lib(bvm *vm) {
      be_pushntvclass(vm, &be_class_udp);
      be_setglobal(vm, "udp");
      be_pop(vm, 1);
  }
}
/* @const_object_info_begin

class be_class_udp (scope: global, name: udp) {
    .p, var
    remote_ip, var
    remote_port, var
    init, func(be_udp_init)
    deinit, func(be_udp_deinit)

    send, func(be_udp_send)
    send_multicast, func(be_udp_send_mcast)

    begin, func(be_udp_begin)
    begin_multicast, func(be_udp_begin_mcast)
    read, func(be_udp_read)
}
@const_object_info_end */

#endif // USE_WEBCLIENT
