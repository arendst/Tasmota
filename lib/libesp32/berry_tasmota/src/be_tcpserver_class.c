/********************************************************************
 * Tcp socket
 *******************************************************************/
#include "be_constobj.h"
#include "be_mapping.h"
#include "be_ctypes.h"

#ifdef USE_BERRY_TCPSERVER

extern const void * tcpserver_init(struct bvm *vm, int32_t port);       BE_FUNC_CTYPE_DECLARE(tcpserver_init, "+_p", "@i")
extern void tcpserver_deinit(void *server_tcp);                         BE_FUNC_CTYPE_DECLARE(tcpserver_deinit, "", ".")

extern void tcpserver_close(void *server);                              BE_FUNC_CTYPE_DECLARE(tcpserver_close, "", ".")
extern bbool tcpserver_hasclient(void *server);                         BE_FUNC_CTYPE_DECLARE(tcpserver_hasclient, "b", ".")
extern void * tcpserver_accept(struct bvm *vm, void *server);           BE_FUNC_CTYPE_DECLARE(tcpserver_accept, "tcpclient", "@.")
extern void * tcpserver_acceptasync(struct bvm *vm, void *server);      BE_FUNC_CTYPE_DECLARE(tcpserver_acceptasync, "tcpclientasync", "@.")

#include "be_fixed_be_class_tcpserver.h"

/* @const_object_info_begin
class be_class_tcpserver (scope: global, name: tcpserver) {
    _p, var
    init, ctype_func(tcpserver_init)
    deinit, ctype_func(tcpserver_deinit)

    close, ctype_func(tcpserver_close)
    hasclient, ctype_func(tcpserver_hasclient)
    accept, ctype_func(tcpserver_accept)
    acceptasync, ctype_func(tcpserver_acceptasync)
}
@const_object_info_end */

#endif // USE_BERRY_TCPSERVER
