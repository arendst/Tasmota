/********************************************************************
 * Webclient mapped to Arduino framework
 * 
 * To use: `d = webclient()`
 * 
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_WEBCLIENT

extern int wc_tcp_init(bvm *vm);
extern int wc_tcp_deinit(bvm *vm);

extern int wc_tcp_connect(bvm *vm);
extern int wc_tcp_connected(bvm *vm);
extern int wc_tcp_close(bvm *vm);
extern int wc_tcp_available(bvm *vm);

extern int wc_tcp_write(bvm *vm);
extern int wc_tcp_read(bvm *vm);
extern int wc_tcp_readbytes(bvm *vm);

#include "be_fixed_be_class_tcpclient.h"

void be_load_tcpclient_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_tcpclient);
    be_setglobal(vm, "tcpclient");
    be_pop(vm, 1);
}
/* @const_object_info_begin

class be_class_tcpclient (scope: global, name: tcpclient) {
    .w, var
    init, func(wc_tcp_init)
    deinit, func(wc_tcp_deinit)

    connect, func(wc_tcp_connect)
    connected, func(wc_tcp_connected)
    close, func(wc_tcp_close)
    available, func(wc_tcp_available)

    write, func(wc_tcp_write)
    read, func(wc_tcp_read)
    readbytes, func(wc_tcp_readbytes)
}
@const_object_info_end */

#endif // USE_WEBCLIENT
