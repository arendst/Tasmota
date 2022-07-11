#include "be_constobj.h"

static be_define_const_map_slots(be_class_tcpserver_map) {
    { be_const_key(init, -1), be_const_ctype_func(tcpserver_init) },
    { be_const_key(accept, -1), be_const_ctype_func(tcpserver_accept) },
    { be_const_key(_p, -1), be_const_var(0) },
    { be_const_key(close, 0), be_const_ctype_func(tcpserver_close) },
    { be_const_key(deinit, 5), be_const_ctype_func(tcpserver_deinit) },
    { be_const_key(hasclient, -1), be_const_ctype_func(tcpserver_hasclient) },
};

static be_define_const_map(
    be_class_tcpserver_map,
    6
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_tcpserver,
    1,
    NULL,
    tcpserver
);
