/********************************************************************
 * UDP lib
 *
 * To use: `d = udp()`
 *
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_WEBCLIENT

extern int be_udp_init(struct bvm *vm);
extern int be_udp_deinit(struct bvm *vm);
extern int be_udp_begin(struct bvm *vm);
extern int be_udp_stop(struct bvm *vm);
extern int be_udp_begin_mcast(struct bvm *vm);
extern int be_udp_send(struct bvm *vm);
extern int be_udp_send_mcast(struct bvm *vm);
extern int be_udp_read(struct bvm *vm);

#include "be_mapping.h"
#include "be_fixed_be_class_udp.h"

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
