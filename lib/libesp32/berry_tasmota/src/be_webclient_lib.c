/********************************************************************
 * Webclient mapped to Arduino framework
 *
 * To use: `d = webclient()`
 *
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_WEBCLIENT

extern int wc_init(bvm *vm);
extern int wc_deinit(bvm *vm);
extern int wc_urlencode(bvm *vm);
extern int wc_begin(bvm *vm);
extern int wc_set_timeouts(bvm *vm);
extern int wc_set_useragent(bvm *vm);
extern int wc_set_follow_redirects(bvm *vm);
extern int wc_set_auth(bvm *vm);
extern int wc_collect_headers(bvm *vm);
extern int wc_get_header(bvm *vm);
extern int wc_connected(bvm *vm);
extern int wc_close(bvm *vm);
extern int wc_addheader(bvm *vm);
extern int wc_GET(bvm *vm);
extern int wc_POST(bvm *vm);
extern int wc_PUT(bvm *vm);
extern int wc_PATCH(bvm *vm);
extern int wc_DELETE(bvm *vm);
extern int wc_getstring(bvm *vm);
extern int wc_writefile(bvm *vm);
extern int wc_writeflash(bvm *vm);
extern int wc_getsize(bvm *vm);
extern int wc_getbytes(bvm *vm);

#include "be_fixed_be_class_webclient.h"

void be_load_webclient_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_webclient);
    be_setglobal(vm, "webclient");
    be_pop(vm, 1);
}
/* @const_object_info_begin

class be_class_webclient (scope: global, name: webclient) {
    .p, var
    .w, var
    init, func(wc_init)
    deinit, func(wc_deinit)
    url_encode, static_func(wc_urlencode)

    begin, func(wc_begin)
    set_timeouts, func(wc_set_timeouts)
    set_useragent, func(wc_set_useragent)
    set_follow_redirects, func(wc_set_follow_redirects)
    set_auth, func(wc_set_auth)

    // collect response headers
    collect_headers, func(wc_collect_headers)
    get_header, func(wc_get_header)

    close, func(wc_close)
    add_header, func(wc_addheader)
    GET, func(wc_GET)
    POST, func(wc_POST)
    PUT, func(wc_PUT)
    PATCH, func(wc_PATCH)
    DELETE, func(wc_DELETE)
    get_string, func(wc_getstring)
    write_file, func(wc_writefile)
    write_flash, func(wc_writeflash)
    get_size, func(wc_getsize)
    get_bytes, func(wc_getbytes)
}
@const_object_info_end */

#endif // USE_WEBCLIENT
