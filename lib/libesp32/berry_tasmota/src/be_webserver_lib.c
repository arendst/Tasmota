/********************************************************************
 * Berry module `webserver`
 * 
 * To use: `import webserver`
 * 
 * Allows to respond to HTTP request
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_WEBSERVER

#include "be_exec.h"
#include "be_vm.h"

extern int w_webserver_member(bvm *vm);
extern int w_webserver_on(bvm *vm);
extern int w_webserver_remove_route(bvm *vm);
extern int w_webserver_state(bvm *vm);

extern int w_webserver_check_privileged_access(bvm *vm);
extern int w_webserver_redirect(bvm *vm);
extern int w_webserver_content_start(bvm *vm);
extern int w_webserver_content_open(bvm *vm);
extern int w_webserver_content_send(bvm *vm);
extern int w_webserver_content_response(bvm *vm);
extern int w_webserver_content_send_style(bvm *vm);
extern int w_webserver_content_flush(bvm *vm);
extern int w_webserver_content_stop(bvm *vm);
extern int w_webserver_content_close(bvm *vm);
extern int w_webserver_content_button(bvm *vm);
extern int w_webserver_content_status_sticker(bvm *vm);

extern int w_webserver_html_escape(bvm *vm);

extern int w_webserver_argsize(bvm *vm);
extern int w_webserver_arg(bvm *vm);
extern int w_webserver_arg_name(bvm *vm);
extern int w_webserver_has_arg(bvm *vm);

extern int w_webserver_header(bvm *vm);

// To allow a full restart of the Berry VM, we need to supplement the webserver Request Handler
// model from Arduino framework.
// We use our own list of callbacks

#define WEBSERVER_REQ_HANDLER_HOOK_MAX       32      // max number of callbacks, each callback requires a distinct address
typedef struct be_webserver_callback_hook_t {
  bvm *vm;                // make sure we are using the same VM
  bvalue f;               // the Berry function to call
} be_webserver_callback_hook_t;

static be_webserver_callback_hook_t be_webserver_cb_hooks[WEBSERVER_REQ_HANDLER_HOOK_MAX];

static void be_call_webserver_hook_cb(int32_t num);
typedef void (*berry_webserver_cb_t)(void);
#define WEBSERVER_HOOK_CB(n) void berry_webserver_cb_##n(void) { be_call_webserver_hook_cb(n); }
// list the callbacks
WEBSERVER_HOOK_CB(0);
WEBSERVER_HOOK_CB(1);
WEBSERVER_HOOK_CB(2);
WEBSERVER_HOOK_CB(3);
WEBSERVER_HOOK_CB(4);
WEBSERVER_HOOK_CB(5);
WEBSERVER_HOOK_CB(6);
WEBSERVER_HOOK_CB(7);
WEBSERVER_HOOK_CB(8);
WEBSERVER_HOOK_CB(9);
WEBSERVER_HOOK_CB(10);
WEBSERVER_HOOK_CB(11);
WEBSERVER_HOOK_CB(12);
WEBSERVER_HOOK_CB(13);
WEBSERVER_HOOK_CB(14);
WEBSERVER_HOOK_CB(15);
WEBSERVER_HOOK_CB(16);
WEBSERVER_HOOK_CB(17);
WEBSERVER_HOOK_CB(18);
WEBSERVER_HOOK_CB(19);
WEBSERVER_HOOK_CB(20);
WEBSERVER_HOOK_CB(21);
WEBSERVER_HOOK_CB(22);
WEBSERVER_HOOK_CB(23);
WEBSERVER_HOOK_CB(24);
WEBSERVER_HOOK_CB(25);
WEBSERVER_HOOK_CB(26);
WEBSERVER_HOOK_CB(27);
WEBSERVER_HOOK_CB(28);
WEBSERVER_HOOK_CB(29);
WEBSERVER_HOOK_CB(30);
WEBSERVER_HOOK_CB(31);

// array of callbacks
static const berry_webserver_cb_t berry_callback_array[WEBSERVER_REQ_HANDLER_HOOK_MAX] = {
  berry_webserver_cb_0,
  berry_webserver_cb_1,
  berry_webserver_cb_2,
  berry_webserver_cb_3,
  berry_webserver_cb_4,
  berry_webserver_cb_5,
  berry_webserver_cb_6,
  berry_webserver_cb_7,
  berry_webserver_cb_8,
  berry_webserver_cb_9,
  berry_webserver_cb_10,
  berry_webserver_cb_11,
  berry_webserver_cb_12,
  berry_webserver_cb_13,
  berry_webserver_cb_14,
  berry_webserver_cb_15,
  berry_webserver_cb_16,
  berry_webserver_cb_17,
  berry_webserver_cb_18,
  berry_webserver_cb_19,
  berry_webserver_cb_20,
  berry_webserver_cb_21,
  berry_webserver_cb_22,
  berry_webserver_cb_23,
  berry_webserver_cb_24,
  berry_webserver_cb_25,
  berry_webserver_cb_26,
  berry_webserver_cb_27,
  berry_webserver_cb_28,
  berry_webserver_cb_29,
  berry_webserver_cb_30,
  berry_webserver_cb_31,
};

// Return slot number
// -1 if no more available
berry_webserver_cb_t be_webserver_allocate_hook(bvm *vm, int32_t slot, bvalue *f) {
  if (slot < 0 || slot >= WEBSERVER_REQ_HANDLER_HOOK_MAX) return NULL;   // invalid call, avoid a crash
  if (be_isgcobj(f)) {
    be_gc_fix_set(vm, f->v.gc, btrue);    // mark the function as non-gc
  }
  be_webserver_cb_hooks[slot].vm = vm;
  be_webserver_cb_hooks[slot].f = *f;
  return berry_callback_array[slot];
}

bbool be_webserver_deallocate_hook(bvm *vm, int32_t slot) {
  if (slot < 0 || slot >= WEBSERVER_REQ_HANDLER_HOOK_MAX) return bfalse;   // invalid call, avoid a crash
  bvalue f = be_webserver_cb_hooks[slot].f;
  if (be_isgcobj(&f)) {
    be_gc_fix_set(vm, f.v.gc, bfalse);    // remove the marker for non-gc
  }
  var_setnil(&be_webserver_cb_hooks[slot].f);
  return btrue;
}

/*********************************************************************************************\
 * `be_webserver_cb_deinit`:
 *  Clean any callback for this VM
\*********************************************************************************************/
void be_webserver_cb_deinit(bvm *vm) {
  for (int32_t i = 0; i < WEBSERVER_REQ_HANDLER_HOOK_MAX; i++) {
    if (be_webserver_cb_hooks[i].vm == vm) {
      be_webserver_cb_hooks[i].vm = NULL;
    }
  }
}

/*********************************************************************************************\
 * Callback structures
 * 
\*********************************************************************************************/
void be_call_webserver_hook_cb(int32_t num) {
  // call berry cb dispatcher
  int32_t ret = 0;
  // retrieve vm and function
  if (num < 0 || num >= WEBSERVER_REQ_HANDLER_HOOK_MAX || be_webserver_cb_hooks[num].vm == NULL) return;   // invalid call, avoid a crash

  bvm * vm = be_webserver_cb_hooks[num].vm;
  bvalue *f = &be_webserver_cb_hooks[num].f;

  // push function (don't check type)
  bvalue *top = be_incrtop(vm);
  *top = *f;

  ret = be_pcall(vm, 0);   // 4 arguments
  if (ret != 0) {
    if (vm->obshook != NULL) (*vm->obshook)(vm, BE_OBS_PCALL_ERROR);
    be_pop(vm, be_top(vm));       // clear Berry stack
    return;
  }
  be_pop(vm, 1);    // remove result
  return;
}

/* @const_object_info_begin
module webserver (scope: global) {
    member, func(w_webserver_member)

    on, func(w_webserver_on)
    remove_route, func(w_webserver_remove_route)
    state, func(w_webserver_state)

    check_privileged_access, func(w_webserver_check_privileged_access)
    redirect, func(w_webserver_redirect)
    content_send, func(w_webserver_content_send)
    content_response, func(w_webserver_content_response)
    content_send_style, func(w_webserver_content_send_style)
    content_flush, func(w_webserver_content_flush)
    content_open, func(w_webserver_content_open)
    content_start, func(w_webserver_content_start)
    content_stop, func(w_webserver_content_stop)
    content_close, func(w_webserver_content_close)
    content_button, func(w_webserver_content_button)

    content_status_sticker, func(w_webserver_content_status_sticker)

    html_escape, func(w_webserver_html_escape)

    arg_size, func(w_webserver_argsize)
    arg, func(w_webserver_arg)
    arg_name, func(w_webserver_arg_name)
    has_arg, func(w_webserver_has_arg)

    header, func(w_webserver_header)
}
@const_object_info_end */
#include "be_fixed_webserver.h"

#endif // USE_WEBSERVER
