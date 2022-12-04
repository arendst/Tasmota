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
#include <WiFiUdp46.h>
#include "be_mapping.h"

// Tasmota Logging
extern void AddLog(uint32_t loglevel, PGM_P formatP, ...);
enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE};

extern "C" {

  // init()
  WiFiUDP46 *be_udp_init_ntv(void) {
    return new WiFiUDP46();
  }
  int32_t be_udp_init(struct bvm *vm) {
    return be_call_c_func(vm, (void*) &be_udp_init_ntv, "+.p", "");
  }

  // deinit()
  void *be_udp_deinit_ntv(WiFiUDP46 *udp) {
    if (udp != nullptr) { delete udp; }
    return nullptr;
  }
  int32_t be_udp_deinit(struct bvm *vm) {
    return be_call_c_func(vm, (void*) &be_udp_deinit_ntv, "=.p", "");
  }

  // udp.begin(address:string, port:int) -> bool
  int32_t be_udp_begin_ntv(WiFiUDP46 *udp, int32_t port) {   
    IPAddress46 addr;
    // AddLog(LOG_LEVEL_DEBUG, "BRY: udp.begin listening to '%s'", addr.toString().c_str());
    return udp->begin(addr, port);
  }
  int32_t be_udp_begin(struct bvm *vm) {
    if (be_top(vm) >= 3 && be_isstring(vm, 2)) {
      // legacy string parameter, now ignored
      return be_call_c_func(vm, (void*) &be_udp_begin_ntv, "b", ".-i");
    } else {
      return be_call_c_func(vm, (void*) &be_udp_begin_ntv, "b", ".i");
    }
  }

  // udp.stop() -> nil
  void be_udp_stop_ntv(WiFiUDP46 *udp) {
    udp->stop();
  }
  int32_t be_udp_stop(struct bvm *vm) {
    return be_call_c_func(vm, (void*) &be_udp_stop_ntv, "b", ".");
  }

  // udp.begin_multicast(address:string, port:int) -> nil
  int32_t be_udp_begin_mcast_ntv(WiFiUDP46 *udp, const char *host, int32_t port) {
    IPAddress46 addr;
    if(!WiFiGeneric46::hostByName(host, addr)){
        return 0;
    }
    return udp->WiFiUDP46::beginMulticast(addr, port);
  }
  int32_t be_udp_begin_mcast(struct bvm *vm) {
    return be_call_c_func(vm, (void*) &be_udp_begin_mcast_ntv, "b", ".si");
  }

  // udp.send(address:string, port:int, payload:bytes) -> bool
  int32_t be_udp_send_ntv(WiFiUDP46 *udp, const char *host, int32_t port, const uint8_t* buf, int32_t len) {
    IPAddress46 addr;
    if (!WiFiGeneric46::hostByName(host, addr)){
        return 0;
    }
    // AddLog(LOG_LEVEL_DEBUG, "BRY: udp.begin got host '%s'", addr.toString().c_str());
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
  int32_t be_udp_send_mcast_ntv(WiFiUDP46 *udp, const uint8_t* buf, int32_t len) {
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
    WiFiUDP46 *udp = (WiFiUDP46*) be_convert_single_elt(vm, 1, NULL, NULL);
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
      IPAddress46 remote_ip = udp->remoteIP();
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
    close, func(be_udp_stop)
}
@const_object_info_end */

#endif // USE_WEBCLIENT
