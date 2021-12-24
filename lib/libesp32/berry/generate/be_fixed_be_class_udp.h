#include "be_constobj.h"

static be_define_const_map_slots(be_class_udp_map) {
    { be_const_key(send_multicast, 6), be_const_func(be_udp_send_mcast) },
    { be_const_key(send, -1), be_const_func(be_udp_send) },
    { be_const_key(deinit, -1), be_const_func(be_udp_deinit) },
    { be_const_key(remote_ip, -1), be_const_var(0) },
    { be_const_key(read, -1), be_const_func(be_udp_read) },
    { be_const_key(begin_multicast, 8), be_const_func(be_udp_begin_mcast) },
    { be_const_key(begin, -1), be_const_func(be_udp_begin) },
    { be_const_key(remote_port, -1), be_const_var(1) },
    { be_const_key(init, -1), be_const_func(be_udp_init) },
    { be_const_key(_X2Ep, 4), be_const_var(2) },
};

static be_define_const_map(
    be_class_udp_map,
    10
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_udp,
    3,
    NULL,
    udp
);
