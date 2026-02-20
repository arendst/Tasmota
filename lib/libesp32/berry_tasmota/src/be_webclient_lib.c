/********************************************************************
 * Webclient ASYNC mapped to Arduino framework
 * 
 * 2026 -  ASYNC Webclient integration (#define USE_BERRY_WEBCLIENT_ASYNC) by HexaMaster 
 *
 * To use: `d = webclient()`
 *
 *******************************************************************/


#include "be_constobj.h" 

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

/* --- TLS pinning extensions --- */
extern int wc_tls_pin_pubkey(bvm *vm);
extern int wc_tls_clear_pins(bvm *vm);
extern int wc_tls_set_rsa_only(bvm *vm);
/* ------------------------------- */

/* --- Async extensions --- */
extern int wc_async_get_start(bvm *vm);
extern int wc_async_post_start(bvm *vm);   /* payload: string|bytes */
extern int wc_async_state(bvm *vm);
extern int wc_async_abort(bvm *vm);
/* ------------------------ */

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

    // async native layer
    ._last_code, var
    ._last_rx, var
    .__async_hold, var

    // public mirrors async
    last_code, var
    last_rx, var

    init, func(wc_init)
    deinit, func(wc_deinit)
    url_encode, static_func(wc_urlencode)

    // TLS pinning API
    tls_pin_pubkey, func(wc_tls_pin_pubkey)
    tls_clear_pins, func(wc_tls_clear_pins)
    tls_set_rsa_only, func(wc_tls_set_rsa_only)


    begin, func(wc_begin)
    set_timeouts, func(wc_set_timeouts)
    set_useragent, func(wc_set_useragent)
    set_follow_redirects, func(wc_set_follow_redirects)
    set_auth, func(wc_set_auth)

    // collect response headers
    collect_headers, func(wc_collect_headers)
    get_header, func(wc_get_header)

    close, func(wc_close)
    // header helpers (both spellings kept for compatibility)
    addheader, func(wc_addheader)
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

    // --- async controls ---
    async_get_start, func(wc_async_get_start)
    async_post_start, func(wc_async_post_start)
    async_state, func(wc_async_state)
    async_abort, func(wc_async_abort)
}
@const_object_info_end */
