#include "lv_berry.h"
#include "be_mapping.h"

#include "be_exec.h"
#include "be_vm.h"
#include "be_mem.h"

#include <stdio.h>
#include <string.h>

// Backport configuration from LVGL 8 to LVGL 9
#ifdef BE_LV_WIDGET_BTN
  #undef BE_LV_WIDGET_BUTTON
  #define BE_LV_WIDGET_BUTTON
#endif
#ifdef BE_LV_WIDGET_BTNMATRIX
  #undef BE_LV_WIDGET_BUTTONMATRIX
  #define BE_LV_WIDGET_BUTTONMATRIX
#endif
#ifdef BE_LV_WIDGET_IMG
  #undef BE_LV_WIDGET_IMAGE
  #define BE_LV_WIDGET_IMAGE
#endif
#ifdef BE_LV_WIDGET_IMGBTN
  #undef BE_LV_WIDGET_IMAGEBUTTON
  #define BE_LV_WIDGET_IMAGEBUTTON
#endif

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
    lv_style_init(style);
  }
  be_pushcomptr(vm, style);
  be_setmember(vm, 1, "_p");
  be_return_nil(vm);
}
/*********************************************************************************************\
 * Delete style
 * 
 * Use with caution, it shouldn't be referenced after this call
\*********************************************************************************************/
int lv_be_style_del(bvm *vm) {
  be_getmember(vm, 1, "_p");
  lv_style_t * style = be_tocomptr(vm, -1);
  if (style != NULL) {
    be_free(vm, style, sizeof(lv_style_t));
  }
  be_pushcomptr(vm, NULL);
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

/*********************************************************************************************\
 * Find a method in a compact class table (see `lv_ntv_method_t` in lv_berry.h)
 *
 * The name is split into a verb prefix (longest match in `lv_method_verbs`) and the rest,
 * entries are sorted by (verb code, rest) so a binary search finds the method.
 * Returns NULL if not found.
\*********************************************************************************************/
static const lv_ntv_method_t * lv_method_find(const lv_ntv_class_def_t * cl, const char * name) {
  // split verb prefix, longest match wins
  uint32_t verb = 0;
  size_t verb_len = 0;
  for (uint32_t v = 1; v < LV_METHOD_VERBS_NB; v++) {
    size_t len = strlen(lv_method_verbs[v]);
    if (len > verb_len && strncmp(name, lv_method_verbs[v], len) == 0) {
      verb = v;
      verb_len = len;
    }
  }
  const char * rest = name + verb_len;

  int32_t low = 0;
  int32_t high = (int32_t)cl->size - 1;
  while (low <= high) {
    int32_t mid = (low + high) / 2;
    const lv_ntv_method_t * m = &cl->methods[mid];
    int32_t comp = (int32_t)verb - (int32_t)(m->sig >> LV_METHOD_SIG_BITS);
    if (comp == 0) {
      comp = strcmp(rest, &lv_method_names[m->name]);
    }
    if (comp < 0) {
      high = mid - 1;
    } else if (comp > 0) {
      low = mid + 1;
    } else {
      return m;
    }
  }
  return NULL;
}

/*********************************************************************************************\
 * Generic style accessors (see `lv_style_ntv_prop_t` in lv_berry.h)
 *
 * `obj.set_style_<p>(v, sel)`, `obj.get_style_<p>(part)` and `style.set_<p>(v)` are not
 * stored as individual methods. They are resolved from `lv_style_props[]` and call
 * the generic LVGL API, with exactly the same semantics as the generated LVGL functions.
 *
 * The C wrappers below are called by `be_call_c_func()`. The second argument `pk` is
 * inserted by `lv_x_call_style()` and holds the property id (8 low bits) and the
 * value kind (LV_SKIND_*, next bits).
\*********************************************************************************************/
static lv_style_value_t lvbe_style_value(uint32_t kind, intptr_t v) {
  lv_style_value_t sv;
  memset(&sv, 0, sizeof(sv));
  switch (kind) {
    case LV_SKIND_U8:     sv.num = (uint8_t) v;              break;
    case LV_SKIND_U16:    sv.num = (uint16_t) v;             break;
    case LV_SKIND_BOOL:   sv.num = (v != 0);                 break;
    case LV_SKIND_COLOR:  // Berry passes `lv.color` as 0xRRGGBB, same bits as `lv_color_t` passed by value
      sv.color.blue  = (v      ) & 0xFF;
      sv.color.green = (v >>  8) & 0xFF;
      sv.color.red   = (v >> 16) & 0xFF;
      break;
    case LV_SKIND_PTR:    sv.ptr = (const void *) v;         break;
    default:              sv.num = (int32_t) v;              break;
  }
  return sv;
}

static void lvbe_obj_set_style_prop(lv_obj_t * obj, int32_t pk, intptr_t value, lv_style_selector_t selector) {
  LV_CHECK_OBJ(obj, &lv_obj_class, return);
  lv_obj_set_local_style_prop(obj, (lv_style_prop_t)(pk & 0xFF), lvbe_style_value(pk >> 8, value), selector);
}

static intptr_t lvbe_obj_get_style_prop(lv_obj_t * obj, int32_t pk, lv_part_t part) {
  LV_CHECK_OBJ(obj, &lv_obj_class, return 0);
  lv_style_value_t v = lv_obj_get_style_prop(obj, part, (lv_style_prop_t)(pk & 0xFF));
  switch (pk >> 8) {
    case LV_SKIND_U8:     return (uint8_t) v.num;
    case LV_SKIND_U16:    return (uint16_t) v.num;
    case LV_SKIND_BOOL:   return v.num != 0;
    case LV_SKIND_COLOR:  return v.color.blue | (v.color.green << 8) | (v.color.red << 16);
    case LV_SKIND_PTR:    return (intptr_t) v.ptr;
    default:              return v.num;
  }
}

static void lvbe_style_set_prop(lv_style_t * style, int32_t pk, intptr_t value) {
  lv_style_set_prop(style, (lv_style_prop_t)(pk & 0xFF), lvbe_style_value(pk >> 8, value));
}

// native closure, upvals: 0=func, 1=return type, 2=arg type, 3=pk
// inserts `pk` as second argument (after self) and calls `be_call_c_func`
static int lv_x_call_style(bvm *vm) {
  be_getupval(vm, 0, 0);
  void * func = be_tocomptr(vm, -1);
  be_getupval(vm, 0, 1);
  const char * return_type = be_tostring(vm, -1);
  be_getupval(vm, 0, 2);
  const char * arg_type = be_tostring(vm, -1);
  be_getupval(vm, 0, 3);
  int32_t pk = be_toint(vm, -1);
  be_pop(vm, 4);

  // shift arguments 2..top by one and put `pk` at index 2
  int32_t top = be_top(vm);
  be_pushnil(vm);
  for (int32_t i = top; i >= 2; i--) {
    be_moveto(vm, i, i + 1);
  }
  be_pushint(vm, pk);
  be_moveto(vm, -1, 2);
  be_pop(vm, 1);
  return be_call_c_func(vm, func, return_type, arg_type);
}

// Try to resolve a generic style accessor for class `class_name`
// Returns true and pushes a native closure if found
static bbool lv_style_member(bvm *vm, const char * class_name, const char * name) {
  uint32_t flag;
  size_t verb_len;
  if (strcmp(class_name, "lv_obj") == 0) {
    if (strncmp(name, "set_style_", 10) == 0)       { flag = LV_SPROP_OBJ_SET;    verb_len = 10; }
    else if (strncmp(name, "get_style_", 10) == 0)  { flag = LV_SPROP_OBJ_GET;    verb_len = 10; }
    else { return bfalse; }
  } else if (strcmp(class_name, "lv_style") == 0 && strncmp(name, "set_", 4) == 0) {
    flag = LV_SPROP_STYLE_SET;
    verb_len = 4;
  } else {
    return bfalse;
  }
  const char * rest = name + verb_len;

  // binary search in `lv_style_props`, sorted by name
  const lv_style_ntv_prop_t * p = NULL;
  int32_t low = 0;
  int32_t high = (int32_t)LV_STYLE_PROPS_NB - 1;
  while (low <= high) {
    int32_t mid = (low + high) / 2;
    int32_t comp = strcmp(rest, &lv_method_names[lv_style_props[mid].name]);
    if (comp < 0)       { high = mid - 1; }
    else if (comp > 0)  { low = mid + 1; }
    else                { p = &lv_style_props[mid]; break; }
  }
  if (p == NULL || ((p->info >> 5) & flag) == 0) { return bfalse; }

  const lv_style_ntv_type_t * t = &lv_style_types[p->info & 0x1F];
  char arg_type[48];
  const char * return_type = "";
  const void * func;
  if (flag == LV_SPROP_OBJ_SET) {
    snprintf(arg_type, sizeof(arg_type), "(lv.obj)i%si", t->value_arg);
    func = (const void *) &lvbe_obj_set_style_prop;
  } else if (flag == LV_SPROP_OBJ_GET) {
    snprintf(arg_type, sizeof(arg_type), "(lv.obj)ii");
    return_type = t->return_type;
    func = (const void *) &lvbe_obj_get_style_prop;
  } else {
    snprintf(arg_type, sizeof(arg_type), "(lv.style)i%s", t->value_arg);
    func = (const void *) &lvbe_style_set_prop;
  }

  be_pushntvclosure(vm, &lv_x_call_style, 4);   // 4 upvals
  be_pushcomptr(vm, (void *) func);
  be_setupval(vm, -2, 0);
  be_pop(vm, 1);
  be_pushstring(vm, return_type);
  be_setupval(vm, -2, 1);
  be_pop(vm, 1);
  be_pushstring(vm, arg_type);
  be_setupval(vm, -2, 2);
  be_pop(vm, 1);
  be_pushint(vm, p->prop | (t->kind << 8));
  be_setupval(vm, -2, 3);
  be_pop(vm, 1);
  return btrue;
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
        const lv_ntv_method_t * method = lv_method_find(&lv_classes[class_idx], method_name);
        if (method != NULL) {
          // method found
          const lv_ntv_sig_t * sig = &lv_method_sigs[method->sig & LV_METHOD_SIG_MASK];
          // push native closure
          be_pushntvclosure(vm, &lv_x_call_c, 3);   // 3 upvals

          be_pushcomptr(vm, (void*) method->func);
          be_setupval(vm, -2, 0);
          be_pop(vm, 1);

          be_pushstring(vm, sig->return_type);
          be_setupval(vm, -2, 1);
          be_pop(vm, 1);

          be_pushstring(vm, sig->arg_type);
          be_setupval(vm, -2, 2);
          be_pop(vm, 1);

          // all good
          be_return(vm);
        }
        // generic style accessors for `lv_obj` and `lv_style`
        if (lv_style_member(vm, class_name, method_name)) {
          be_return(vm);
        }
      }

      // get super if any, or nil if none
      be_getsuper(vm, 1);
      be_moveto(vm, -1, 1);
      be_pop(vm, 1);
    }
    // berry_log_C("lv_x_member method not found");
    /* not found, return module 'undefined' */
    be_getmodule(vm, "undefined");
    be_return(vm);
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
  uint32_t color32 = 0;

  if (argc > 1) {
    if (be_isint(vm, 2)) {        // color is RGB 24 bits
      color32 = be_toint(vm, 2);
    } else if (be_iscomptr(vm, 2)) {
      color32 = (intptr_t) be_tocomptr(vm, 2);
    }
  }
  be_pushint(vm, color32);
  be_setmember(vm, 1, "_p");
  be_return_nil(vm);
}

int lco_tostring(bvm *vm) {
  be_getmember(vm, 1, "_p");
  uint32_t color32 = be_toint(vm, -1);
  be_pop(vm, 1);  // remove attribute
  char s[32];
  snprintf(s, sizeof(s), "lv_color(0x%06x)", color32);
  be_pushnstring(vm, s, strlen(s)); /* make escape string from buffer */
  be_return(vm);
}

int lco_toint(bvm *vm) {
  be_getmember(vm, 1, "_p");
  uint32_t color32 = be_toint(vm, -1);
  be_pushint(vm, color32);
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
    lv_font_t * font = lv_binfont_create(be_tostring(vm, 1));
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

extern const lv_const_group_t lv0_constants_groups[];
extern const size_t lv0_constants_groups_size;

#define LV_CONST_GROUP_PREFIX_MAX   15      // must match CONST_GROUP_MAX_PREFIX_LEN in tools/convert.py

int lv0_constants_as_hash(bvm *vm);     // defined below

/*********************************************************************************************\
 * Search constants grouped by prefix, ex: `STYLE_BG_COLOR` is "BG_COLOR" in group "STYLE"
 *
 * Supports the same encodings as `be_const_member_dual()` for int, `$` string,
 * `&` comptr and `@` native function (the generator only groups these).
 * Returns true and pushes the value if found.
\*********************************************************************************************/
static bbool lv0_member_grouped(bvm *vm, const char * needle) {
  const char * sep = strchr(needle, '_');
  if (sep == NULL) { return bfalse; }
  size_t prefix_len = sep - needle;
  if (prefix_len == 0 || prefix_len > LV_CONST_GROUP_PREFIX_MAX) { return bfalse; }
  char prefix[LV_CONST_GROUP_PREFIX_MAX + 1];
  memcpy(prefix, needle, prefix_len);
  prefix[prefix_len] = 0;

  int32_t g = be_map_bin_search(prefix, &lv0_constants_groups[0].prefix, sizeof(lv0_constants_groups[0]), lv0_constants_groups_size);
  if (g < 0) { return bfalse; }
  const lv_const_group_t * group = &lv0_constants_groups[g];
  int32_t idx = be_map_bin_search(sep + 1, &group->members[0].name, sizeof(group->members[0]), group->size);
  if (idx < 0) { return bfalse; }

  const be_const_member_t * m = &group->members[idx];
  switch (m->name[0]) {
    case '$': be_pushstring(vm, (const char*) m->value);           break;
    case '&': be_pushcomptr(vm, (void*) m->value);                 break;
    case '@': be_pushntvfunction(vm, (bntvfunc) m->value);         break;
    default:  be_pushint(vm, m->value);                            break;
  }
  return btrue;
}

int lv0_member(bvm *vm);
int lv0_member(bvm *vm) {
  // first try the standard way
  if (be_const_module_member(vm, lv0_constants, lv0_constants_size)) {
    be_return(vm);
  }
  // then constants grouped by prefix
  if (be_top(vm) == 1 && be_isstring(vm, 1)) {
    const char * needle = be_tostring(vm, 1);
    if (lv0_member_grouped(vm, needle)) {
      be_return(vm);
    }
    // `lv._constants()` returns a map of all int constants (see tools/lv_export_const.be)
    if (strcmp(needle, "_constants") == 0) {
      be_pushntvfunction(vm, lv0_constants_as_hash);
      be_return(vm);
    }
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

/*********************************************************************************************\
 * Return a hash of all LV int constants
\*********************************************************************************************/
// add one int constant to the map at top of stack, skip non-int (string, pointer, function)
static void lv0_constants_hash_add(bvm *vm, const char * prefix, const be_const_member_t * m) {
  char first_char = m->name[0];
  if ( !(first_char >= 'a' && first_char <='z') && !(first_char >= 'A' && first_char <='Z') ) {
    return;
  }
  if (prefix) {
    char name[64];
    snprintf(name, sizeof(name), "%s_%s", prefix, m->name);   // rebuild full name, ex: "STYLE" + "BG_COLOR"
    be_pushstring(vm, name);
  } else {
    be_pushstring(vm, m->name);
  }
  be_pushint(vm, m->value);
  be_data_insert(vm, -3);
  be_pop(vm, 2);
}

// `lv._constants()` returns a map of all int constants with their numerical value
// used by `tools/lv_export_const.be`
int lv0_constants_as_hash(bvm *vm) {
  be_newobject(vm, "map");

  for (size_t i = 0; i < lv0_constants_size; i++) {
    lv0_constants_hash_add(vm, NULL, &lv0_constants[i]);
  }
  for (size_t g = 0; g < lv0_constants_groups_size; g++) {
    const lv_const_group_t * group = &lv0_constants_groups[g];
    for (size_t i = 0; i < group->size; i++) {
      lv0_constants_hash_add(vm, group->prefix, &group->members[i]);
    }
  }
  be_pop(vm, 1);
  be_return(vm);
}

/*********************************************************************************************\
 * temporarily fix lv_span_get_style()
\*********************************************************************************************/
#include "widgets/bar/lv_bar_private.h"
lv_area_t * lv_bar_get_indic_area(lv_obj_t * bar) {
  return &((lv_bar_t*)bar)->indic_area;
}
