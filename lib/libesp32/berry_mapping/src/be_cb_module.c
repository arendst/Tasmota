/********************************************************************
 * Callback module
 * 
 * To use: `import cb`
 * 
 *******************************************************************/
#include "be_constobj.h"

#include "be_mapping.h"
#include "be_gc.h"
#include "be_exec.h"
#include "be_vm.h"
#include "be_mem.h"

/*********************************************************************************************\
 * Callback structures
 * 
 * We allow 4 parameters, or 3 if method (first arg is `self`)
 * This could be extended if needed
\*********************************************************************************************/
typedef int32_t (*berry_callback_t)(int32_t v0, int32_t v1, int32_t v2, int32_t v3);
static int32_t call_berry_cb(int32_t num, int32_t v0, int32_t v1, int32_t v2, int32_t v3);

#define BERRY_CB(n) int32_t berry_cb_##n(int32_t v0, int32_t v1, int32_t v2, int32_t v3) { return call_berry_cb(n, v0, v1, v2, v3); }
// list the callbacks
BERRY_CB(0);
BERRY_CB(1);
BERRY_CB(2);
BERRY_CB(3);
BERRY_CB(4);
BERRY_CB(5);
BERRY_CB(6);
BERRY_CB(7);
BERRY_CB(8);
BERRY_CB(9);
BERRY_CB(10);
BERRY_CB(11);
BERRY_CB(12);
BERRY_CB(13);
BERRY_CB(14);
BERRY_CB(15);
BERRY_CB(16);
BERRY_CB(17);
BERRY_CB(18);
BERRY_CB(19);

// array of callbacks
static const berry_callback_t berry_callback_array[BE_MAX_CB] = {
  berry_cb_0,
  berry_cb_1,
  berry_cb_2,
  berry_cb_3,
  berry_cb_4,
  berry_cb_5,
  berry_cb_6,
  berry_cb_7,
  berry_cb_8,
  berry_cb_9,
  berry_cb_10,
  berry_cb_11,
  berry_cb_12,
  berry_cb_13,
  berry_cb_14,
  berry_cb_15,
  berry_cb_16,
  berry_cb_17,
  berry_cb_18,
  berry_cb_19,
};

typedef struct be_callback_hook {
  bvm *vm;
  bvalue f;
} be_callback_hook;

typedef struct be_callback_handler_list_t {
  bvm *vm;
  bvalue f;
  struct be_callback_handler_list_t *next;
} be_callback_handler_list_t;

static be_callback_hook be_cb_hooks[BE_MAX_CB] = {0};

static int32_t be_cb_gen_cb(bvm *vm);
static be_callback_handler_list_t be_callback_default_gen_cb = {
  NULL,
  be_const_func(&be_cb_gen_cb),
  NULL
};

static be_callback_handler_list_t *be_callback_handler_list_head = &be_callback_default_gen_cb;    /* linked list of handlers */

/*********************************************************************************************\
 * `add_handler`: Add an external handler to `make_cb()`
 * 
 * This is typically used by LVGL mapping to handle widget callbacks, the handler
 * needs to record additional infomation to disambiguate the call
 * 
 * arg1: function (or closure)
\*********************************************************************************************/
static int32_t be_cb_add_handler(bvm *vm) {
  int32_t top = be_top(vm);
  if (top >= 1 && be_isfunction(vm, 1)) {
    bvalue *v = be_indexof(vm, 1);

    be_callback_handler_list_t *elt = be_os_malloc(sizeof(be_callback_handler_list_t));
    if (!elt) { be_throw(vm, BE_MALLOC_FAIL); }

    if (be_isgcobj(v)) {
      be_gc_fix_set(vm, v->v.gc, btrue);    // mark the function as non-gc
    }
    elt->vm = vm;
    elt->f = *v;
    elt->next = be_callback_handler_list_head;  /* insert as new head */
    be_callback_handler_list_head = elt;
    be_return_nil(vm);
  }
  be_raise(vm, "value_error", "arg must be a function");
}

/*********************************************************************************************\
 * `list_handlers`: List all cb handlers registered for this VM
 * 
 * Used for debugging only
 * 
 * No args
\*********************************************************************************************/
static int32_t be_cb_list_handlers(bvm *vm) {
  be_newobject(vm, "list");
  for (be_callback_handler_list_t *elt = be_callback_handler_list_head; elt != NULL; elt = elt->next) {
    if (elt->vm == vm) { /* on purpose don't show the default handler, just pretend it's not there since it's default */
      bvalue *top = be_incrtop(vm);
      *top = elt->f;
      be_data_push(vm, -2);
      be_pop(vm, 1);
    }
  }
  be_pop(vm, 1);
  be_return(vm);
}

/*********************************************************************************************\
 * `make_cb`: high-level call for creating a callback.
 * 
 * This function is called by `be_mapping` when generating a callback with a type name.
 * LVGL typically needs to register typed callbacks
 * 
 * arg1: function (or closure)
 * arg2: type name for callback (optional)
 * argN: any other callback specific arguments (unlimited number, passed as-is)
\*********************************************************************************************/
static int32_t be_cb_make_cb(bvm *vm) {
  int32_t argc = be_top(vm);
  if (argc >= 1 && be_isfunction(vm, 1)) {

    bvalue *v = be_indexof(vm, 1);
    for (be_callback_handler_list_t *elt = be_callback_handler_list_head; elt != NULL; elt = elt->next) {
      if (elt->vm == vm || elt->vm == NULL) {   // if elt->vm is NULL then we accept any VM
        // call the handler and check result
        bvalue *top = be_incrtop(vm);
        *top = elt->f;
        // var_setobj(top, elt->f->type, elt->f);  // push function - arg0
        for (int i=1; i<=argc; i++) {           // push all arguments including function
          be_pushvalue(vm, i);
        }
        be_call(vm, argc);                      // call handler
        be_pop(vm, argc);                       // remove arguments, top of stack has the result
        // if top of stack is `comptr` then it is successful
        if (be_iscomptr(vm, -1)) {
          be_return(vm);
        } else {
          be_pop(vm, 1);                        // remove top, rinse and repeat
        }
      }
    }

    // if we are here, it means that no handler has handled the request
  }
  be_raise(vm, "value_error", "arg must be a function");
}

/*********************************************************************************************\
 * `gen_cb`: Generate a new callback
 * 
 * arg1: function (or closure)
\*********************************************************************************************/
static int32_t be_cb_gen_cb(bvm *vm) {
  int32_t top = be_top(vm);
  if (top >= 1 && be_isfunction(vm, 1)) {
    // find first available slot
    int32_t slot;
    for (slot = 0; slot < BE_MAX_CB; slot++) {
      if (be_cb_hooks[slot].f.type == BE_NIL) break;
    }
    bvalue *v = be_indexof(vm, 1);
    if (slot < BE_MAX_CB) {
      if (be_isgcobj(v)) {
        be_gc_fix_set(vm, v->v.gc, btrue);    // mark the function as non-gc
      }
      // record pointers
      be_cb_hooks[slot].vm = vm;
      be_cb_hooks[slot].f = *v;
      be_pushcomptr(vm, (void*) berry_callback_array[slot]);
      be_return(vm);
    } else {
      be_raise(vm, "internal_error", "no more callbacks available, increase BE_MAX_CB");
    }
  }
  be_raise(vm, "value_error", "arg must be a function");
}

/*********************************************************************************************\
 * `get_cb_list`: Return the list of callbacks for this vm
 * 
\*********************************************************************************************/
static int32_t be_cb_get_cb_list(bvm *vm) {
  be_newobject(vm, "list");
  int32_t i;
  for (uint32_t i=0; i < BE_MAX_CB; i++) {
    if (be_cb_hooks[i].vm) {
      if (vm == be_cb_hooks[i].vm) {  // make sure it corresponds to this vm
        bvalue *top = be_incrtop(vm);
        *top = be_cb_hooks[i].f;
        // be_pushcomptr(vm, be_cb_hooks[i].f);
        be_data_push(vm, -2);
        be_pop(vm, 1);
      }
    } else {
      break;
    }
  }
  be_pop(vm, 1);
  be_return(vm);       
}

/*********************************************************************************************\
 * Callback structures
 * 
 * We allow 4 parameters, or 3 if method (first arg is `self`)
 * This could be extended if needed
\*********************************************************************************************/
static int32_t call_berry_cb(int32_t num, int32_t v0, int32_t v1, int32_t v2, int32_t v3) {
  // call berry cb dispatcher
  int32_t ret = 0;
  // retrieve vm and function
  if (num < 0 || num >= BE_MAX_CB || be_cb_hooks[num].vm == NULL) return 0;   // invalid call, avoid a crash

  bvm * vm = be_cb_hooks[num].vm;
  bvalue *f = &be_cb_hooks[num].f;

  // push function (don't check type)
  bvalue *top = be_incrtop(vm);
  *top = *f;
  // push args
  be_pushint(vm, v0);
  be_pushint(vm, v1);
  be_pushint(vm, v2);
  be_pushint(vm, v3);

  ret = be_pcall(vm, 4);   // 4 arguments
  if (ret != 0) {
    if (vm->obshook != NULL) (*vm->obshook)(vm, BE_OBS_PCALL_ERROR);
    be_pop(vm, be_top(vm));       // clear Berry stack
    return 0;
  }
  ret = be_toint(vm, -5);
  be_pop(vm, 5);    // remove result
  return ret;
}

/* @const_object_info_begin
module cb (scope: global) {
    gen_cb, func(be_cb_gen_cb)
    get_cb_list, func(be_cb_get_cb_list)

    add_handler, func(be_cb_add_handler)
    list_handlers, func(be_cb_list_handlers)
    make_cb, func(be_cb_make_cb)
}
@const_object_info_end */
#include "../../berry/generate/be_fixed_cb.h"
