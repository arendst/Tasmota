/********************************************************************
 * TCP client non-blocking (async)
 * 
 * 
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_WEBCLIENT

extern int wc_tcpasync_init(bvm *vm);
extern int wc_tcpasync_deinit(bvm *vm);

extern int wc_tcpasync_connect(bvm *vm);
extern int wc_tcpasync_connected(bvm *vm);
extern int wc_tcpasync_listening(bvm *vm);
extern int wc_tcpasync_info(bvm *vm);
extern int wc_tcpasync_close(bvm *vm);
extern int wc_tcpasync_available(bvm *vm);

extern int wc_tcpasync_write(bvm *vm);
extern int wc_tcpasync_read(bvm *vm);
extern int wc_tcpasync_readbytes(bvm *vm);

#include "be_fixed_be_class_tcpclientasync.h"

/* @const_object_info_begin

class be_class_tcpclientasync (scope: global, name: tcpclientasync) {
    .p, var
    init, func(wc_tcpasync_init)
    deinit, func(wc_tcpasync_deinit)

    connect, func(wc_tcpasync_connect)
    connected, func(wc_tcpasync_connected)
    listening, func(wc_tcpasync_listening)
    info, func(wc_tcpasync_info)
    close, func(wc_tcpasync_close)
    available, func(wc_tcpasync_available)

    write, func(wc_tcpasync_write)
    read, func(wc_tcpasync_read)
    readbytes, func(wc_tcpasync_readbytes)
}
@const_object_info_end */

#endif // USE_WEBCLIENT
