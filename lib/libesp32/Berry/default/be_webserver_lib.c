/********************************************************************
 * Berry module `webserver`
 * 
 * To use: `import webserver`
 * 
 * Allows to respond to HTTP request
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_WEBSERVER

extern int w_webserver_member(bvm *vm);
extern int w_webserver_on(bvm *vm);
extern int w_webserver_state(bvm *vm);

extern int w_webserver_check_privileged_access(bvm *vm);
extern int w_webserver_content_start(bvm *vm);
extern int w_webserver_content_send(bvm *vm);
extern int w_webserver_content_send_style(bvm *vm);
extern int w_webserver_content_flush(bvm *vm);
extern int w_webserver_content_stop(bvm *vm);
extern int w_webserver_content_button(bvm *vm);

extern int w_webserver_argsize(bvm *vm);
extern int w_webserver_arg(bvm *vm);
extern int w_webserver_has_arg(bvm *vm);

#if !BE_USE_PRECOMPILED_OBJECT
be_native_module_attr_table(webserver) {
    be_native_module_function("member", w_webserver_member),

    be_native_module_function("on", w_webserver_on),
    be_native_module_function("state", w_webserver_state),

    be_native_module_function("check_privileged_access", w_webserver_check_privileged_access),
    be_native_module_function("content_start", w_webserver_content_start),
    be_native_module_function("content_send", w_webserver_content_send),
    be_native_module_function("content_send_style", w_webserver_content_send_style),
    be_native_module_function("content_flush", w_webserver_content_flush),
    be_native_module_function("content_start", w_webserver_content_start),
    be_native_module_function("content_stop", w_webserver_content_stop),
    be_native_module_function("content_button", w_webserver_content_button),

    be_native_module_function("arg_size", w_webserver_argsize),
    be_native_module_function("arg", w_webserver_arg),
    be_native_module_function("has_arg", w_webserver_has_arg),

};

be_define_native_module(webserver, NULL);
#else
/* @const_object_info_begin
module webserver (scope: global) {
    member, func(w_webserver_member)

    on, func(w_webserver_on)
    state, func(w_webserver_state)

    check_privileged_access, func(w_webserver_check_privileged_access)
    content_start, func(w_webserver_content_start)
    content_send, func(w_webserver_content_send)
    content_send_style, func(w_webserver_content_send_style)
    content_flush, func(w_webserver_content_flush)
    content_start, func(w_webserver_content_start)
    content_stop, func(w_webserver_content_stop)
    content_button, func(w_webserver_content_button)

    arg_size, func(w_webserver_argsize)
    arg, func(w_webserver_arg)
    has_arg, func(w_webserver_has_arg)
}
@const_object_info_end */
#include "../generate/be_fixed_webserver.h"
#endif

#endif // USE_WEBSERVER
