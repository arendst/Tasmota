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
extern int w_webserver_redirect(bvm *vm);
extern int w_webserver_content_start(bvm *vm);
extern int w_webserver_content_send(bvm *vm);
extern int w_webserver_content_send_style(bvm *vm);
extern int w_webserver_content_flush(bvm *vm);
extern int w_webserver_content_stop(bvm *vm);
extern int w_webserver_content_button(bvm *vm);

extern int w_webserver_argsize(bvm *vm);
extern int w_webserver_arg(bvm *vm);
extern int w_webserver_arg_name(bvm *vm);
extern int w_webserver_has_arg(bvm *vm);


/* @const_object_info_begin
module webserver (scope: global) {
    member, func(w_webserver_member)

    on, func(w_webserver_on)
    state, func(w_webserver_state)

    check_privileged_access, func(w_webserver_check_privileged_access)
    redirect, func(w_webserver_redirect)
    content_send, func(w_webserver_content_send)
    content_send_style, func(w_webserver_content_send_style)
    content_flush, func(w_webserver_content_flush)
    content_start, func(w_webserver_content_start)
    content_stop, func(w_webserver_content_stop)
    content_button, func(w_webserver_content_button)

    arg_size, func(w_webserver_argsize)
    arg, func(w_webserver_arg)
    arg_name, func(w_webserver_arg_name)
    has_arg, func(w_webserver_has_arg)
}
@const_object_info_end */
#include "../generate/be_fixed_webserver.h"

#endif // USE_WEBSERVER
