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

// Tasmota Logging
extern void tasmota_log_C(uint32_t loglevel, const char * berry_buf, ...);
enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE};

/*********************************************************************************************\
 * Callback structures
 * 
 * We allow 5 parameters, or 4 if method (first arg is `self`)
 * This could be extended if needed
\*********************************************************************************************/
typedef int (*berry_callback_t)(int v0, int v1, int v2, int v3, int v4);
static int call_berry_cb(int num, int v0, int v1, int v2, int v3, int v4);

#define BERRY_CB(n) int berry_cb_##n(int v0, int v1, int v2, int v3, int v4) { return call_berry_cb(n, v0, v1, v2, v3, v4); }
// list the callbacks
BERRY_CB(0)
BERRY_CB(1)
BERRY_CB(2)
BERRY_CB(3)
BERRY_CB(4)
BERRY_CB(5)
BERRY_CB(6)
BERRY_CB(7)
BERRY_CB(8)
BERRY_CB(9)
BERRY_CB(10)
BERRY_CB(11)
BERRY_CB(12)
BERRY_CB(13)
BERRY_CB(14)
BERRY_CB(15)
BERRY_CB(16)
BERRY_CB(17)
BERRY_CB(18)
BERRY_CB(19)

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

static int be_cb_gen_cb(bvm *vm);
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
static int be_cb_add_handler(bvm *vm) {
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
static int be_cb_list_handlers(bvm *vm) {
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
static int be_cb_make_cb(bvm *vm) {
  int32_t argc = be_top(vm);
  if (argc >= 1 && be_isfunction(vm, 1)) {

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
 * `gen_cb`: Generate a new callback - SECURITY PATCHED
 * 
 * SECURITY IMPROVEMENTS:
 * - Added input validation and bounds checking
 * - Resource limit enforcement per VM
 * - Protection against callback slot exhaustion attacks
 * 
 * arg1: function (or closure)
\*********************************************************************************************/
static int be_cb_gen_cb(bvm *vm) {
  int32_t top = be_top(vm);
  
#if BE_MAPPING_ENABLE_INPUT_VALIDATION
  // SECURITY: Input validation
  if (top < 1) {
    be_raise(vm, "value_error", "gen_cb requires at least 1 argument");
  }
  
  if (!be_isfunction(vm, 1)) {
    be_raise(vm, "value_error", "arg must be a function");
  }
  
  // SECURITY: Count existing callbacks for this VM to prevent resource exhaustion
  int32_t vm_callback_count = 0;
  for (int32_t i = 0; i < BE_MAX_CB; i++) {
    if (be_cb_hooks[i].vm == vm) {
      vm_callback_count++;
    }
  }
  
  // SECURITY: Enforce per-VM callback limit
  #define MAX_CALLBACKS_PER_VM 10
  if (vm_callback_count >= MAX_CALLBACKS_PER_VM) {
    be_raise(vm, "resource_error", "Too many callbacks for this VM (max 10)");
  }
#endif // BE_MAPPING_ENABLE_INPUT_VALIDATION
  
  // Find first available slot
  int32_t slot;
  for (slot = 0; slot < BE_MAX_CB; slot++) {
    if (be_cb_hooks[slot].f.type == BE_NIL) break;
  }
  
  if (slot >= BE_MAX_CB) {
    be_raise(vm, "internal_error", "no more callbacks available, increase BE_MAX_CB");
  }
  
  bvalue *v = be_indexof(vm, 1);
  
  // SECURITY: Validate the function value
  if (v == NULL) {
    be_raise(vm, "internal_error", "Invalid function value");
  }
  
  // Fix GC object if needed
  if (be_isgcobj(v)) {
    be_gc_fix_set(vm, v->v.gc, btrue);    // mark the function as non-gc
  }
  
  // Record pointers
  be_cb_hooks[slot].vm = vm;
  be_cb_hooks[slot].f = *v;
  
  be_pushcomptr(vm, (void*) berry_callback_array[slot]);
  be_return(vm);
}

/*********************************************************************************************\
 * `get_cb_list`: Return the list of callbacks for this vm
 * 
\*********************************************************************************************/
static int be_cb_get_cb_list(bvm *vm) {
  be_newobject(vm, "list");
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
 * Callback execution dispatcher - SECURITY PATCHED
 * 
 * SECURITY IMPROVEMENTS (BM-002 Patch):
 * - Enhanced bounds checking and validation
 * - Type safety verification before callback execution
 * - Protection against callback hijacking
 * - Comprehensive error handling
 * 
 * We allow 5 parameters, or 4 if method (first arg is `self`)
 * This could be extended if needed
\*********************************************************************************************/
static int call_berry_cb(int num, int v0, int v1, int v2, int v3, int v4) {
  // SECURITY: Comprehensive input validation - BM-002 patch
  if (num < 0 || num >= BE_MAX_CB) {
    return 0;   // Invalid call, avoid a crash
  }
  
  if (be_cb_hooks[num].vm == NULL) {
    return 0;   // VM was cleaned up
  }
  
  // SECURITY: Validate callback function type - BM-002 patch
  if (be_cb_hooks[num].f.type == BE_NIL) {
    return 0;   // Function was cleared
  }
  
  // Check if the stored value is a function (any function type)
  if ((be_cb_hooks[num].f.type & 0x1F) != BE_FUNCTION) {
    return 0;   // Not a valid function
  }

  int32_t ret = 0;
  bvm * vm = be_cb_hooks[num].vm;
  bvalue *f = &be_cb_hooks[num].f;

  // Push function (with type validation already done above)
  bvalue *top = be_incrtop(vm);
  if (top == NULL) {
    return 0;
  }
  *top = *f;
  
  // Push arguments
  be_pushint(vm, v0);
  be_pushint(vm, v1);
  be_pushint(vm, v2);
  be_pushint(vm, v3);
  be_pushint(vm, v4);

  // SECURITY: Protected call with error handling
  ret = be_pcall(vm, 5);   // 5 arguments
  if (ret != 0) {
    if (vm->obshook != NULL) (*vm->obshook)(vm, BE_OBS_PCALL_ERROR);
    be_pop(vm, be_top(vm));       // clear Berry stack
    return 0;
  }
  
  // SECURITY: Validate return value
  if (be_top(vm) < 6) {
    be_pop(vm, be_top(vm));
    return 0;
  }
  
  ret = be_toint(vm, -6);
  be_pop(vm, 6);    // remove result and arguments
  
  return ret;
}

/*********************************************************************************************\
 * `be_cb_deinit`: SECURITY PATCHED
 *  Clean any callback for this VM, they shouldn't call the registered function anymore
 *  
 *  SECURITY IMPROVEMENTS (BM-004 Patch):
 *  - Fixed use-after-free vulnerability in callback handler cleanup
 *  - Proper memory deallocation to prevent memory leaks
 *  - Safe iteration through linked list during deletion
 *  - Added bounds checking and validation
\*********************************************************************************************/
void be_cb_deinit(bvm *vm) {
  // SECURITY: Clear all callbacks for this VM - prevent use-after-free
  for (int32_t slot = 0; slot < BE_MAX_CB; slot++) {
    if (be_cb_hooks[slot].vm == vm) {
      // Clear the callback entry
      be_cb_hooks[slot].vm = NULL;
      be_cb_hooks[slot].f.type = BE_NIL;
    }
  }
  
  // SECURITY: Safe removal of callback handlers - BM-004 patch
  // Use safe iteration to avoid use-after-free when removing nodes
  be_callback_handler_list_t **current_ptr = &be_callback_handler_list_head;
  
  while (*current_ptr != NULL) {
    be_callback_handler_list_t *current = *current_ptr;
    
    // Skip the default handler (it has vm == NULL and should never be removed)
    if (current->vm == NULL) {
      current_ptr = &current->next;
      continue;
    }
    
    // Check if this handler belongs to the VM being cleaned up
    if (current->vm == vm) {
      // SECURITY: Safe removal - update pointer before freeing
      *current_ptr = current->next;
      
      // SECURITY: Unfix GC object if it was fixed
      if (be_isgcobj(&current->f)) {
        be_gc_fix_set(vm, current->f.v.gc, bfalse);
      }
      
      // SECURITY: Clear the structure before freeing
      current->vm = NULL;
      current->f.type = BE_NIL;
      current->next = NULL;
      
      // SECURITY: Free the memory - fixes memory leak
      be_os_free(current);
      
      // Don't advance current_ptr since we removed the current node
      // The next iteration will check the new node at this position
    } else {
      // Move to next node
      current_ptr = &current->next;
    }
  }
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
