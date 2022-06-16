#include "lv_berry.h"
#include "be_mapping.h"

#include "be_exec.h"
#include "be_vm.h"
#include "be_mem.h"

#include <stdio.h>

#include "../generate/be_lv_c_mapping.h"

/*********************************************************************************************\
 * Retrieve the value of `self._p`
\*********************************************************************************************/
// Get the `_p` member of instance at `index`
void * lv_get_arg(bvm *vm, int index) {
  void * ret = NULL;
  if (be_isinstance(vm, index)) {
    be_getmember(vm, index, "_p");
    ret = be_tocomptr(vm, -1);
    be_pop(vm, 1);  // remove _p attribute
  }
  return ret;
}

/*********************************************************************************************\
 * Support for lv_indev and objects that don't need creator
\*********************************************************************************************/
int lv0_init(bvm *vm);
int lv0_init(bvm *vm) {
  // "+_p" indicates that there must be an non NULL argument, either passed as comptr or returned by the function
  // Here, there is no function, so calling the constructor without a non-null comptr argument is rejected
  return be_call_c_func(vm, NULL, "=_p", NULL);
}

/*********************************************************************************************\
 * Generalized tostring method, shows class and _p value
\*********************************************************************************************/
int lv_x_tostring(bvm *vm) {
  lv_obj_t * obj = (lv_obj_t*) lv_get_arg(vm, 1);
  const char * classname = be_classname(vm, 1);
  char s[48];
  snprintf(s, sizeof(s), "<instance: %s(0x%08X)>", classname, obj);
  be_pushnstring(vm, s, strlen(s)); /* make escape string from buffer */
  be_return(vm);
}

/*********************************************************************************************\
 * Support for lv_style `init()`
 * 
 * Either encapsulate the pointer passed as `comptr` as arg1
 * Or allocate a new empty style structure in memory. In this case, it is never freed.
\*********************************************************************************************/
int lv_be_style_init(bvm *vm) {
  int argc = be_top(vm);
  lv_style_t * style = NULL;

  if (argc > 1) {
    style = (lv_style_t*) be_convert_single_elt(vm, 2, NULL, NULL);
  }
  if (style == NULL) {
    // if no valid pointer passed, allocate a new empty style
    style = (lv_style_t*) be_malloc(vm, sizeof(lv_style_t));
    if (style == NULL) {
        be_throw(vm, BE_MALLOC_FAIL);
    }
    if (style != NULL) {
      lv_style_init(style);
    }
  }
  be_pushcomptr(vm, style);
  be_setmember(vm, 1, "_p");
  be_return_nil(vm);
}

/*********************************************************************************************\
 * Support for lv_anim `init()`
 * 
 * Either encapsulate the pointer passed as `comptr` as arg1
 * Or allocate a new empty style structure in memory. In this case, it is never freed.
\*********************************************************************************************/
int lv_be_anim_init(bvm *vm) {
  int argc = be_top(vm);
  lv_anim_t * anim = NULL;

  if (argc > 1) {
    anim = (lv_anim_t*) be_convert_single_elt(vm, 2, NULL, NULL);
  }
  if (anim == NULL) {
    // if no valid pointer passed, allocate a new empty style
    anim = (lv_anim_t*) be_malloc(vm, sizeof(lv_anim_t));
    if (anim == NULL) {
        be_throw(vm, BE_MALLOC_FAIL);
    }
    if (anim != NULL) {
      lv_anim_init(anim);
    }
  }
  be_pushcomptr(vm, anim);
  be_setmember(vm, 1, "_p");
  be_return_nil(vm);
}

// native closure to call `be_call_c_func`
int lv_x_call_c(bvm *vm) {
  // berry_log_C("lv_x_call_c enter");
  // keep parameters unchanged
  be_getupval(vm, 0, 0);    // if index is zero, it's the current native closure
  void * func = be_tocomptr(vm, -1);
  be_getupval(vm, 0, 1);    // if index is zero, it's the current native closure
  const char * return_type = be_tostring(vm, -1);
  be_getupval(vm, 0, 2);    // if index is zero, it's the current native closure
  const char * arg_type = be_tostring(vm, -1);
  be_pop(vm, 3);            // remove 3 upvals

  // berry_log_C("lv_x_call_c %p '%s' <- (%s)", func, return_type, arg_type);
  return be_call_c_func(vm, func, return_type, arg_type);
}

// virtual method, arg1: instance, arg2: name of method
int lv_x_member(bvm *vm) {
  int32_t argc = be_top(vm); // Get the number of arguments
  if (argc == 2 && be_isinstance(vm, 1) && be_isstring(vm, 2)) {
    const char * method_name = be_tostring(vm, 2);    // the method we are looking for
    while (be_isinstance(vm, 1)) {
      const char * class_name = be_classname(vm, 1);
      // berry_log_C("lv_x_member looking for method '%s' of class '%s'", method_name, class_name);

      // look for class descriptor
      int32_t class_idx = be_map_bin_search(class_name, &lv_classes[0].name, sizeof(lv_classes[0]), lv_classes_size);
      if (class_idx >= 0) {
        const be_ntv_func_def_t * methods_calls = lv_classes[class_idx].func_table;
        size_t methods_size = lv_classes[class_idx].size;

        int32_t method_idx = be_map_bin_search(method_name, methods_calls, sizeof(be_ntv_func_def_t), methods_size);
        if (method_idx >= 0) {
          // method found
          const be_ntv_func_def_t * method = &methods_calls[method_idx];
          // berry_log_C("lv_x_member method found func=%p return_type=%s arg_type=%s", method->func, method->return_type, method->arg_type);
          // push native closure
          be_pushntvclosure(vm, &lv_x_call_c, 3);   // 3 upvals

          be_pushcomptr(vm, method->args.func);
          be_setupval(vm, -2, 0);
          be_pop(vm, 1);

          be_pushstring(vm, method->args.return_type);
          be_setupval(vm, -2, 1);
          be_pop(vm, 1);

          be_pushstring(vm, method->args.arg_type);
          be_setupval(vm, -2, 2);
          be_pop(vm, 1);

          // all good
          be_return(vm);
        }
      }

      // get super if any, or nil if none
      be_getsuper(vm, 1);
      be_moveto(vm, -1, 1);
      be_pop(vm, 1);
    }
    // berry_log_C("lv_x_member method not found");
    be_return_nil(vm);
  }
  be_raise(vm, "type_error", NULL);
}

// lv_color - constructor
//
// Supports either new initialization taking 24 bits RGB
// or an existing color using native enconding
//
// Arg1 - the instance of the new lv_color object created
// Arg2 - 1/ if `int` then color is 24 bits 0xRRGGBB
//        2/ if `comptr` then color is native format (probably 16 bits)
//        3/ if no Arg2, color is 0x000000 (black)
int lco_init(bvm *vm) {
  int argc = be_top(vm);
  lv_color_t lv_color = {};       // default value is all zeroes (black)

  if (argc > 1) {
    if (be_isint(vm, 2)) {        // color is RGB 24 bits
      lv_color = lv_color_hex(be_toint(vm, 2));
    } else if (be_iscomptr(vm, 2)) {
      lv_color.full = (intptr_t) be_tocomptr(vm, 2);
    }
  }
  be_pushint(vm, lv_color_to_uint32(lv_color));
  be_setmember(vm, 1, "_p");
  be_return_nil(vm);
}

int lco_tostring(bvm *vm) {
  lv_color_t lv_color = {};
  be_getmember(vm, 1, "_p");
  uint32_t ntv_color = be_toint(vm, -1);
  lv_color = lv_color_from_uint32(ntv_color);
  uint32_t color = lv_color_to32(lv_color) & 0xFFFFFF;
  be_pop(vm, 1);  // remove attribute
  char s[48];
  snprintf(s, sizeof(s), "lv_color(0x%06x - native:0x%04x)", color, ntv_color);
  be_pushnstring(vm, s, strlen(s)); /* make escape string from buffer */
  be_return(vm);
}

int lco_toint(bvm *vm) {
  be_getmember(vm, 1, "_p");
  uint32_t ntv_color = be_toint(vm, -1);
  be_pushint(vm, ntv_color);
  be_return(vm);
}



// function is (void) -> lv_obt_t*
typedef lv_obj_t* (*fn_lvobj__void)(void);  // f() -> newly created lv_obj()
int lv0_lvobj__void_call(bvm *vm, fn_lvobj__void func) {
  lv_obj_t * obj = (*func)();
  be_find_global_or_module_member(vm, "lv.lv_obj");
  be_pushcomptr(vm, (void*) -1);         // stack = class, -1
  be_pushcomptr(vm, (void*) obj);         // stack = class, -1, ptr
  be_call(vm, 2);                 // instanciate, stack = instance (don't call init() )
  be_pop(vm, 2);                  // stack = instance
  be_return(vm);
}

/*********************************************************************************************\
 * Support for lv_fonts
\*********************************************************************************************/
// load font by name on file-system
int lv0_load_font(bvm *vm) {
  int argc = be_top(vm);
  if (argc == 1 && be_isstring(vm, 1)) {
    lv_font_t * font = lv_font_load(be_tostring(vm, 1));
    if (font != NULL) {
      be_find_global_or_module_member(vm, "lv.lv_font");
      be_pushcomptr(vm, font);
      be_call(vm, 1);
      be_pop(vm, 1);
      be_return(vm);
    } else {
      be_return_nil(vm);
    }
  }
  be_raise(vm, "type_error", NULL);
}

/*********************************************************************************************\
 * Get Touch Screen calibration information
\*********************************************************************************************/
lv_ts_calibration_t lv_ts_calibration = {
  0, 0,
  0, 0,
  LV_INDEV_STATE_RELEASED
};

lv_ts_calibration_t * lv_get_ts_calibration(void) {
  return &lv_ts_calibration;
}

/*********************************************************************************************\
 * LVGL top level virtual members
 * 
 * Responds to virtual constants
\*********************************************************************************************/

extern const be_ntv_func_def_t lv_func[];
extern const size_t lv_func_size;

extern const be_const_member_t lv0_constants[];
extern const size_t lv0_constants_size;

extern const be_ctypes_class_by_name_t be_ctypes_lvgl_classes[];
extern const size_t be_ctypes_lvgl_classes_size;

int lv0_member(bvm *vm);
int lv0_member(bvm *vm) {
  // first try the standard way
  if (be_const_module_member(vm, lv0_constants, lv0_constants_size)) {
    be_return(vm);
  }
  // try alternative members
  int32_t argc = be_top(vm); // Get the number of arguments
  if (argc == 1 && be_isstring(vm, 1)) {
    const char * needle = be_tostring(vm, 1);
    int32_t idx;

    // search for a class with this name
    char cl_prefixed[32];
    snprintf(cl_prefixed, sizeof(cl_prefixed), "lv_%s", needle);    // we try both actual name and prefixed with `lv_` so both `lv.obj` and `lv.lv_obj` work
    idx = be_map_bin_search(cl_prefixed, &lv_classes[0].name, sizeof(lv_classes[0]), lv_classes_size);
    if (idx < 0) {
      idx = be_map_bin_search(needle, &lv_classes[0].name, sizeof(lv_classes[0]), lv_classes_size);
    }
    if (idx >= 0) {
      // we did have a match
      be_pushntvclass(vm, lv_classes[idx].cl);
      be_return(vm);
    }
    // same search for ctypes
    idx = be_map_bin_search(cl_prefixed, &be_ctypes_lvgl_classes[0].name, sizeof(be_ctypes_lvgl_classes[0]), be_ctypes_lvgl_classes_size);
    if (idx < 0) {
      idx = be_map_bin_search(needle, &be_ctypes_lvgl_classes[0].name, sizeof(be_ctypes_lvgl_classes[0]), be_ctypes_lvgl_classes_size);
    }
    if (idx >= 0) {
      // we did have a match
      be_pushntvclass(vm, be_ctypes_lvgl_classes[idx].cl);
      be_return(vm);
    }

    // search for a method with this name
    idx = be_map_bin_search(needle, &lv_func[0].name, sizeof(lv_func[0]), lv_func_size);
    if (idx >= 0) {
      const be_ntv_func_def_t * method = &lv_func[idx];
      // push native closure
      be_pushntvclosure(vm, &lv_x_call_c, 3);   // 3 upvals

      be_pushcomptr(vm, method->args.func);
      be_setupval(vm, -2, 0);
      be_pop(vm, 1);

      be_pushstring(vm, method->args.return_type);
      be_setupval(vm, -2, 1);
      be_pop(vm, 1);

      be_pushstring(vm, method->args.arg_type);
      be_setupval(vm, -2, 2);
      be_pop(vm, 1);

      // all good
      be_return(vm);
    }
  }
  be_module_load(vm, be_newstr(vm, "undefined"));
  be_return(vm);
}
