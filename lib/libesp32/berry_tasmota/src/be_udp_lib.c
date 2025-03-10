/********************************************************************
 * UDP lib
 *
 * To use: `d = udp()`
 *
 *******************************************************************/
#include "be_constobj.h"
#include "be_mapping.h"

extern int be_udp_read(struct bvm *vm);

extern void *be_udp_init_ntv(void);
BE_FUNC_CTYPE_DECLARE(be_udp_init_ntv, "+.p", "")

extern void *be_udp_deinit_ntv(void *udp);
BE_FUNC_CTYPE_DECLARE(be_udp_deinit_ntv, "=.p", "")

extern int32_t be_udp_begin_ntv(void *udp, const char *host, int32_t port);
BE_FUNC_CTYPE_DECLARE(be_udp_begin_ntv, "b", ".si")

extern void be_udp_stop_ntv(void *udp);
BE_FUNC_CTYPE_DECLARE(be_udp_stop_ntv, "", ".")

extern int32_t be_udp_begin_mcast_ntv(void *udp, const char *host, int32_t port);
BE_FUNC_CTYPE_DECLARE(be_udp_begin_mcast_ntv, "b", ".si")

extern int32_t be_udp_send_ntv(void *udp, const char *host, int32_t port, const uint8_t* buf, int32_t len);
BE_FUNC_CTYPE_DECLARE(be_udp_send_ntv, "b", ".si(bytes)~")

extern int32_t be_udp_send_mcast_ntv(void *udp, const uint8_t* buf, int32_t len);
BE_FUNC_CTYPE_DECLARE(be_udp_send_mcast_ntv, "b", ".(bytes)~")

#include "be_mapping.h"
#include "be_fixed_be_class_udp.h"

/* @const_object_info_begin

class be_class_udp (scope: global, name: udp) {
    .p, var
    remote_ip, var
    remote_port, var
    init, ctype_func(be_udp_init_ntv)
    deinit, ctype_func(be_udp_deinit_ntv)

    send, ctype_func(be_udp_send_ntv)
    send_multicast, ctype_func(be_udp_send_mcast_ntv)

    begin, ctype_func(be_udp_begin_ntv)
    begin_multicast, ctype_func(be_udp_begin_mcast_ntv)
    read, func(be_udp_read)
    close, ctype_func(be_udp_stop_ntv)
}
@const_object_info_end */
