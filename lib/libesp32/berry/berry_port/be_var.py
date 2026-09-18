"""
be_var.py — Variable resolution (ported from be_var.c / be_var.h)

Implements global and built-in variable tables, variable lookup,
and compiler option flags (COMP_NAMED_GBL, COMP_STRICT).
"""

# ============================================================================
# Original C copyright notice
# ============================================================================
# /********************************************************************
# ** Copyright (c) 2018-2020 Guan Wenliang
# ** This file is part of the Berry default interpreter.
# ** skiars@qq.com, https://github.com/Skiars/berry
# ** See Copyright Notice in the LICENSE file or at
# ** https://github.com/Skiars/berry/blob/master/LICENSE
# ********************************************************************/

from berry_port.be_object import (
    BE_STRING, GC_FIXED,
    bvalue, gc_object,
    var_setnil, var_setint, var_setclass, var_toidx, var_isstr,
)
from berry_port.be_vector import (
    be_vector_init, be_vector_delete, be_vector_at,
    be_vector_resize, be_vector_release, be_vector_count,
)
from berry_port.be_map import (
    be_map_new, be_map_findstr, be_map_insertstr, be_map_compact,
)
from berry_port.be_string import be_str2cstr

# ============================================================================
# Compiler option flags (from be_vm.h)
# ============================================================================

# typedef enum {
#     COMP_NAMED_GBL = 0x00,  /* compile with named globals */
#     COMP_STRICT = 0x01,     /* compile with named globals */
#     COMP_GC_DEBUG = 0x02,   /* compile with gc debug */
# } compoptmask;
COMP_NAMED_GBL = 0x00
COMP_STRICT = 0x01
COMP_GC_DEBUG = 0x02

# #define comp_is_named_gbl(vm)       ((vm)->compopt & (1<<COMP_NAMED_GBL))
# #define comp_set_named_gbl(vm)      ((vm)->compopt |= (1<<COMP_NAMED_GBL))
# #define comp_clear_named_gbl(vm)    ((vm)->compopt &= ~(1<<COMP_NAMED_GBL))
def comp_is_named_gbl(vm):
    return (vm.compopt & (1 << COMP_NAMED_GBL)) != 0

def comp_set_named_gbl(vm):
    vm.compopt |= (1 << COMP_NAMED_GBL)

def comp_clear_named_gbl(vm):
    vm.compopt &= ~(1 << COMP_NAMED_GBL)

# #define comp_is_strict(vm)          ((vm)->compopt & (1<<COMP_STRICT))
# #define comp_set_strict(vm)         ((vm)->compopt |= (1<<COMP_STRICT))
# #define comp_clear_strict(vm)       ((vm)->compopt &= ~(1<<COMP_STRICT))
def comp_is_strict(vm):
    return (vm.compopt & (1 << COMP_STRICT)) != 0

def comp_set_strict(vm):
    vm.compopt |= (1 << COMP_STRICT)

def comp_clear_strict(vm):
    vm.compopt &= ~(1 << COMP_STRICT)

# #define comp_is_gc_debug(vm)       ((vm)->compopt & (1<<COMP_GC_DEBUG))
def comp_is_gc_debug(vm):
    return (vm.compopt & (1 << COMP_GC_DEBUG)) != 0


# ============================================================================
# Macros from be_var.h
# ============================================================================

# #define be_global_count(vm)
#     be_vector_count(&(vm)->gbldesc.global.vlist)
def be_global_count(vm):
    return be_vector_count(vm.gbldesc.global_.vlist)

# #define be_builtin_count(vm)
#     be_vector_count(&(vm)->gbldesc.builtin.vlist)
def be_builtin_count(vm):
    if _builtin(vm).vlist is None:
        return 0
    return be_vector_count(vm.gbldesc.builtin.vlist)


# ============================================================================
# Convenience accessors (mirrors C macros)
# ============================================================================

# #define global(vm)      ((vm)->gbldesc.global)
# #define builtin(vm)     ((vm)->gbldesc.builtin)
def _global(vm):
    return vm.gbldesc.global_

def _builtin(vm):
    return vm.gbldesc.builtin


# ============================================================================
# GC fix stub (same pattern as be_lexer.py)
# ============================================================================

def _be_gc_fix(vm, obj):
    """Stub for be_gc_fix — marks object as GC-fixed."""
    if hasattr(obj, 'marked'):
        obj.marked |= GC_FIXED


# ============================================================================
# Native class table (extern BERRY_LOCAL bclass_array be_class_table)
# ============================================================================

# extern BERRY_LOCAL bclass_array be_class_table;
# This is populated at VM init time by be_libs.py or equivalent.
# For now, default to an empty list.
be_class_table = []


# ============================================================================
# be_globalvar_init (from be_var.c)
# ============================================================================

# void be_globalvar_init(bvm *vm)
# {
#     global(vm).vtab = be_map_new(vm);
#     be_gc_fix(vm, gc_object(global(vm).vtab));
#     be_vector_init(vm, &global(vm).vlist, sizeof(bvalue));
# }
def be_globalvar_init(vm):
    """Initialize global and builtin variable tables."""
    g = _global(vm)
    g.vtab = be_map_new(vm)
    _be_gc_fix(vm, gc_object(g.vtab))
    be_vector_init(vm, g.vlist, 1)  # sizeof(bvalue) → 1 (Python list elements)


# ============================================================================
# be_globalvar_deinit (from be_var.c)
# ============================================================================

# void be_globalvar_deinit(bvm *vm)
# {
#     global(vm).vtab = NULL;
#     be_vector_delete(vm, &global(vm).vlist);
# }
def be_globalvar_deinit(vm):
    """Deinitialize global and builtin variable tables."""
    g = _global(vm)
    g.vtab = None
    be_vector_delete(vm, g.vlist)


# ============================================================================
# global_native_class_find (static, from be_var.c)
# ============================================================================

# /* This function is called when the global was not found */
# /* If the name looked for matches a static class, a global with the class
#    name is created lazily */
# /* Pre: no global exists with name `name`*/
# /* Returns: idx of newly created global, or -1 if not found */
# static int global_native_class_find(bvm *vm, bstring *name)
# {
#     const char* cl_name = str(name);
#     bclass_ptr* class_p = &be_class_table[0];
#     for (; *class_p; class_p++) {
#         const bclass* cl = *class_p;
#
#         if (!strcmp(str(cl->name), cl_name)) {
#             /* class name matches */
#             int idx = be_global_new(vm, name);
#             bvalue *v = be_global_var(vm, idx);
#             var_setclass(v, (void*) cl);
#             return idx;
#         }
#     }
#     return -1;
# }
def _global_native_class_find(vm, name):
    """Lazily create a global for a native class if its name matches."""
    cl_name = be_str2cstr(name)
    for cl in be_class_table:
        if cl is None:
            break
        if be_str2cstr(cl.name) == cl_name:
            # class name matches — create a global for it
            idx = be_global_new(vm, name)
            v = be_global_var(vm, idx)
            var_setclass(v, cl)
            return idx
    return -1


# ============================================================================
# global_find (static, from be_var.c)
# ============================================================================

# static int global_find(bvm *vm, bstring *name)
# {
#     bvalue *res = be_map_findstr(vm, global(vm).vtab, name);
#     if (res) {
#         int idx = var_toidx(res);
#         if (idx >= 0) {
#             return idx + be_builtin_count(vm);
#         } else {
#             return idx;     /* the global does not exist (-1) or was
#                                undefined (< -1)*/
#         }
#     }
#     return -1; /* not found */
# }
def _global_find(vm, name):
    """Look up a global variable by name. Returns index or negative."""
    res = be_map_findstr(vm, _global(vm).vtab, name)
    if res is not None:
        idx = var_toidx(res)
        if idx >= 0:
            return idx + be_builtin_count(vm)
        else:
            return idx  # does not exist (-1) or was undefined (< -1)
    return -1  # not found


# ============================================================================
# be_global_undef (from be_var.c)
# ============================================================================

# bbool be_global_undef(bvm *vm, bstring *name)
# {
#     int idx = global_find(vm, name);
#     if (idx >= 0) {
#         bvalue *desc = be_map_findstr(vm, global(vm).vtab, name);
#         int index = var_toidx(desc);
#         var_setint(desc, -index - 2);   /* negate the index to mark it
#                                            as undefined */
#
#         bvalue* val = be_vector_at(&global(vm).vlist, index);
#         var_setnil(val);
#         return btrue;
#     }
#     return bfalse;
# }
def be_global_undef(vm, name):
    """Undefine a global variable. Returns True if found, False otherwise."""
    idx = _global_find(vm, name)
    if idx >= 0:
        desc = be_map_findstr(vm, _global(vm).vtab, name)
        index = var_toidx(desc)
        var_setint(desc, -index - 2)  # negate to mark as undefined

        val = be_vector_at(_global(vm).vlist, index)
        var_setnil(val)
        return True
    return False


# ============================================================================
# be_global_find (from be_var.c)
# ============================================================================

# int be_global_find(bvm *vm, bstring *name)
# {
#     int res = global_find(vm, name);
#     if (res < 0) {
#         res = be_builtin_find(vm, name);
#     }
#     if (res < 0) {
#         res = global_native_class_find(vm, name);
#     }
#     return res;
# }
def be_global_find(vm, name):
    """Find a variable: check globals, then builtins, then native classes."""
    res = _global_find(vm, name)
    if res < 0:
        res = be_builtin_find(vm, name)
    if res < 0:
        res = _global_native_class_find(vm, name)
    return res


# ============================================================================
# global_new_anonymous (static, from be_var.c)
# ============================================================================

# static int global_new_anonymous(bvm *vm)
# {
#     int idx = be_global_count(vm);
#     /* allocate space for new variables */
#     be_vector_resize(vm, &global(vm).vlist, idx + 1);
#     /* set the new variable to nil */
#     var_setnil((bvalue *)global(vm).vlist.end);
#     return idx;
# }
def _global_new_anonymous(vm):
    """Allocate a new anonymous global slot, set to nil."""
    idx = be_global_count(vm)
    g = _global(vm)
    be_vector_resize(vm, g.vlist, idx + 1)
    # set the new variable to nil — end is an index into data[]
    new_val = bvalue()
    var_setnil(new_val)
    g.vlist.data[g.vlist.end] = new_val
    return idx


# ============================================================================
# be_global_new (from be_var.c)
# ============================================================================

# int be_global_new(bvm *vm, bstring *name)
# {
#     int idx = global_find(vm, name);
#     if (idx < 0) {
#         bvalue *desc;
#         if (idx == -1) {
#             idx = global_new_anonymous(vm);
#             desc = be_map_insertstr(vm, global(vm).vtab, name, NULL);
#             var_setint(desc, idx);
#         } else {
#             /* the global exists but was undefined */
#             idx = -idx - 2;
#             desc = be_map_findstr(vm, global(vm).vtab, name);
#             var_setint(desc, idx);
#         }
#         idx += be_builtin_count(vm);
#     }
#     return idx;
# }
def be_global_new(vm, name):
    """Create or re-enable a global variable. Returns its combined index."""
    idx = _global_find(vm, name)
    if idx < 0:
        if idx == -1:
            idx = _global_new_anonymous(vm)
            desc = be_map_insertstr(vm, _global(vm).vtab, name, None)
            var_setint(desc, idx)
        else:
            # the global exists but was undefined — re-enable it
            idx = -idx - 2
            desc = be_map_findstr(vm, _global(vm).vtab, name)
            var_setint(desc, idx)
        idx += be_builtin_count(vm)
    return idx


# ============================================================================
# be_global_var (from be_var.c)
# ============================================================================

# bvalue* be_global_var(bvm *vm, int index)
# {
#     int bcnt = be_builtin_count(vm);
#     if (index < bcnt) {
#         return be_vector_at(&builtin(vm).vlist, index);
#     }
#     index -= bcnt;
#     return be_vector_at(&global(vm).vlist, index);
# }
def be_global_var(vm, index):
    """Get a bvalue* for a global/builtin variable by combined index."""
    bcnt = be_builtin_count(vm)
    if index < bcnt:
        return be_vector_at(_builtin(vm).vlist, index)
    index -= bcnt
    return be_vector_at(_global(vm).vlist, index)


# ============================================================================
# be_global_release_space (from be_var.c)
# ============================================================================

# void be_global_release_space(bvm *vm)
# {
#     be_map_compact(vm, global(vm).vtab);
#     be_vector_release(vm, &global(vm).vlist);
# }
def be_global_release_space(vm):
    """Compact the global variable map and release excess vector space."""
    g = _global(vm)
    be_map_compact(vm, g.vtab)
    be_vector_release(vm, g.vlist)


# ============================================================================
# be_builtin_find (from be_var.c)
# ============================================================================

# int be_builtin_find(bvm *vm, bstring *name)
# {
#     bvalue *res = be_map_findstr(vm, builtin(vm).vtab, name);
#     if (res) {
#         return var_toidx(res);
#     }
#     return -1; /* not found */
# }
def be_builtin_find(vm, name):
    """Find a built-in variable by name. Returns index or -1."""
    vtab = _builtin(vm).vtab
    if vtab is None:
        return -1  # builtin table not yet initialized
    res = be_map_findstr(vm, vtab, name)
    if res is not None:
        return var_toidx(res)
    return -1  # not found


# ============================================================================
# be_builtin_name (from be_var.c)
# ============================================================================

# bstring* be_builtin_name(bvm *vm, int index)
# {
#     bmap *map = builtin(vm).vtab;
#     bmapnode *end, *node = map->slots;
#     for (end = node + map->size; node < end; ++node) {
#         if (var_isstr(&node->key) && node->value.v.i == index) {
#             return node->key.v.s;
#         }
#     }
#     return NULL;
# }
def be_builtin_name(vm, index):
    """Get the name string for a built-in variable by index."""
    map_ = _builtin(vm).vtab
    if map_ is None or map_.slots is None:
        return None
    for i in range(map_.size):
        node = map_.slots[i]
        # var_isstr on a bmapkey: check key.type == BE_STRING
        if node.key.type == BE_STRING and node.value.v == index:
            return node.key.v
    return None


# ============================================================================
# be_const_builtin_set (from be_var.c)
# ============================================================================

# void be_const_builtin_set(bvm *vm, const bmap *map, const bvector *vec)
# {
#     builtin(vm).vtab = cast(bmap*, map);
#     builtin(vm).vlist = *vec;
# }
def be_const_builtin_set(vm, map_, vec):
    """Set the builtin variable table from precompiled constant objects."""
    b = _builtin(vm)
    b.vtab = map_
    # Copy vector fields (equivalent to struct assignment in C)
    b.vlist.capacity = vec.capacity
    b.vlist.size = vec.size
    b.vlist.count = vec.count
    b.vlist.data = vec.data
    b.vlist.end = vec.end
