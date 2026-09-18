"""
Berry module system.
Mirrors: src/be_module.c / src/be_module.h

This module implements the Berry module system including:
- bmodule structure with table (bmap), info union, gray
- Module creation, deletion, attribute access, member setting
- Module loading: native modules, cached modules, file-based loading
- Module search path management
- Native module registration with bntvmodule structure

Original C code is included as comments for each function.
"""

# /********************************************************************
# ** Copyright (c) 2018-2020 Guan Wenliang
# ** This file is part of the Berry default interpreter.
# ** skiars@qq.com, https://github.com/Skiars/berry
# ** See Copyright Notice in the LICENSE file or at
# ** https://github.com/Skiars/berry/blob/master/LICENSE
# ********************************************************************/

import os

from berry_port.be_object import (
    # Type constants
    BE_NONE, BE_NIL, BE_INT, BE_REAL, BE_BOOL, BE_FUNCTION, BE_MODULE,
    BE_STRING, BE_COMPTR,
    BE_CNIL, BE_CINT, BE_CREAL, BE_CBOOL, BE_CFUNCTION, BE_CSTRING, BE_CMODULE,
    BE_MODULE_NAME, BE_OK, BE_IO_ERROR, BE_EXCEPTION,
    # Structures
    bvalue, bmodule,
    # var_* helpers
    var_type, var_basetype, var_isnil, var_isstr, var_isinstance, var_ismodule,
    var_setnil, var_setint, var_setreal, var_setbool, var_setntvfunc,
    var_setstr, var_setmodule, var_setval,
    var_tobool, var_toobj, var_tostr,
    # GC helpers
    gc_isconst, gc_exmark, gc_setexmark,
    GC_CONST,
    # Cast helpers
    basetype,
)


# ============================================================================
# Constants
# ============================================================================

# /* the maximum suffix length */
# #define SUFFIX_LEN      5 /* length of (.be .bec .so .dll) + 1 */
SUFFIX_LEN = 5


# ============================================================================
# Lazy imports to break circular dependencies
# ============================================================================

def _lazy_be_map():
    from berry_port import be_map
    return be_map

def _lazy_be_string():
    from berry_port import be_string
    return be_string

def _lazy_be_gc():
    from berry_port import be_gc
    return be_gc

def _lazy_be_list():
    from berry_port import be_list
    return be_list

def _lazy_be_exec():
    from berry_port import be_exec
    return be_exec


# ============================================================================
# Module table — list of registered native modules
# ============================================================================

# extern BERRY_LOCAL const bntvmodule_t* const be_module_table[];
# In the Python port, this is a mutable list that modules register into.
be_module_table = []



# ============================================================================
# find_native (from be_module.c)
# ============================================================================

# static const bntvmodule_t* find_native(bstring *path)
# {
#     const bntvmodule_t *module;
#     const bntvmodule_t* const *node = be_module_table;
#     for (; (module = *node) != NULL; ++node) {
#         if (!strcmp(module->name, str(path))) {
#             return module;
#         }
#     }
#     return NULL;
# }
def _find_native(path):
    """Search the native module table for a module matching the given name.

    path is a bstring. Returns the bntvmodule or None.
    """
    be_string = _lazy_be_string()
    name = be_string.be_str2cstr(path)
    for module in be_module_table:
        if module is not None and module.name == name:
            return module
    return None


# ============================================================================
# insert_attrs (from be_module.c)
# ============================================================================

# static void insert_attrs(bvm *vm, bmap *table, const bntvmodule_t *nm)
# {
#     size_t i;
#     for (i = 0; i < nm->size; ++i) {
#         const bntvmodobj_t *node = nm->attrs + i;
#         bstring *name = be_newstr(vm, node->name);
#         bvalue *v = be_map_insertstr(vm, table, name, NULL);
#         be_assert(node->type <= BE_CMODULE);
#         switch (node->type) {
#         case BE_CNIL:
#             var_setnil(v);
#             break;
#         case BE_CINT:
#             var_setint(v, node->u.i);
#             break;
#         case BE_CREAL:
#             var_setreal(v, node->u.r);
#             break;
#         case BE_CBOOL:
#             var_setbool(v, node->u.b);
#             break;
#         case BE_CFUNCTION:
#             var_setntvfunc(v, node->u.f);
#             break;
#         case BE_CSTRING:
#             var_setstr(v, be_newstr(vm, node->u.s));
#             break;
#         case BE_CMODULE:
#             native_module(vm, node->u.o, v);
#             break;
#         default: /* error */
#             break;
#         }
#     }
# }
def _insert_attrs(vm, table, nm):
    """Insert all attributes from a native module descriptor into a map."""
    be_map = _lazy_be_map()
    be_string = _lazy_be_string()

    if nm.attrs is None:
        return
    for i in range(nm.size):
        node = nm.attrs[i]
        name = be_string.be_newstr(vm, node.name)
        v = be_map.be_map_insertstr(vm, table, name, None)
        assert node.type <= BE_CMODULE
        if node.type == BE_CNIL:
            var_setnil(v)
        elif node.type == BE_CINT:
            var_setint(v, node.u)
        elif node.type == BE_CREAL:
            var_setreal(v, node.u)
        elif node.type == BE_CBOOL:
            var_setbool(v, node.u)
        elif node.type == BE_CFUNCTION:
            var_setntvfunc(v, node.u)
        elif node.type == BE_CSTRING:
            var_setstr(v, be_string.be_newstr(vm, node.u))
        elif node.type == BE_CMODULE:
            _native_module(vm, node.u, v)


# ============================================================================
# new_module (from be_module.c)
# ============================================================================

# static bmodule* new_module(bvm *vm, const bntvmodule_t *nm)
# {
#     bgcobject *gco = be_gcnew(vm, BE_MODULE, bmodule);
#     bmodule *obj = cast_module(gco);
#     if (obj) {
#         var_setmodule(vm->top, obj);
#         be_incrtop(vm);
#         obj->info.native = nm;
#         obj->table = NULL; /* gc protection */
#         obj->table = be_map_new(vm);
#         insert_attrs(vm, obj->table, nm);
#         be_map_compact(vm, obj->table); /* clear space */
#         be_stackpop(vm, 1);
#     }
#     return obj;
# }
def _new_module(vm, nm):
    """Create a new module from a native module descriptor.

    The module is temporarily pushed onto the VM stack to protect it
    from GC during map creation and attribute insertion.
    """
    be_map = _lazy_be_map()
    be_gc = _lazy_be_gc()

    obj = bmodule()
    obj.type = BE_MODULE
    obj.marked = 0  # GC_WHITE

    # Push onto stack for GC protection
    var_setmodule(vm.stack[vm.top_idx], obj)
    vm.top_idx += 1

    obj.info = nm  # info.native = nm
    obj.table = None  # gc protection
    obj.table = be_map.be_map_new(vm)
    _insert_attrs(vm, obj.table, nm)
    be_map.be_map_compact(vm, obj.table)

    # Mark the table as const to match C pre-compiled modules (GC_CONST).
    # This ensures be_module_setmember dispatches to 'setmember' for
    # modules like 'global' instead of directly inserting into the map.
    obj.table.marked |= GC_CONST

    # Pop from stack
    vm.top_idx -= 1

    return obj


# ============================================================================
# native_module (from be_module.c)
# ============================================================================

# static bmodule* native_module(bvm *vm, const bntvmodule_t *nm, bvalue *dst)
# {
#     if (nm) {
#         bmodule *obj;
#         if (nm->module) {
#             obj = (bmodule *)nm->module;
#         } else { /* new module */
#             obj = new_module(vm, nm);
#         }
#         if (obj && dst) {
#             var_setmodule(dst, obj);
#         }
#         return obj;
#     }
#     return NULL;
# }
def _native_module(vm, nm, dst=None):
    """Get or create a module from a native module descriptor.

    If nm.module is already set (pre-built constant module), use it directly.
    Otherwise create a new module. If dst is provided, set it to the module.
    """
    if nm is not None:
        if nm.module is not None:
            obj = nm.module
        else:
            obj = _new_module(vm, nm)
        if obj is not None and dst is not None:
            var_setmodule(dst, obj)
        return obj
    return None



# ============================================================================
# Path helpers (from be_module.c)
# ============================================================================

# static char* fixpath(bvm *vm, bstring *path, size_t *size)
# {
#     char *buffer;
#     const char *split, *base;
# #if BE_DEBUG_SOURCE_FILE
#     bvalue *func = vm->cf->func;
#     bclosure *cl = var_toobj(func);
#     if (var_isclosure(func)) {
#         base = str(cl->proto->source); /* get the source file path */
#     } else {
#         base = "/";
#     }
# #else
#     base = "/";
# #endif
#     split = be_splitpath(base);
#     *size = split - base + (size_t)str_len(path) + SUFFIX_LEN;
#     buffer = be_malloc(vm, *size);
#     strncpy(buffer, base, split - base);
#     strcpy(buffer + (split - base), str(path));
#     return buffer;
# }
def _fixpath(vm, path):
    """Build a full path relative to the current source file's directory.

    Returns (fullpath_str, size) where size includes room for suffix.
    """
    be_string = _lazy_be_string()
    from berry_port.berry_conf import BE_DEBUG_SOURCE_FILE
    from berry_port.be_object import var_isclosure, var_toobj

    base = ""
    if BE_DEBUG_SOURCE_FILE and vm.cf is not None:
        func_idx = vm.cf.func
        if func_idx >= 0 and func_idx < len(vm.stack):
            func_val = vm.stack[func_idx]
            if var_isclosure(func_val):
                cl = var_toobj(func_val)
                if cl.proto is not None and cl.proto.source is not None:
                    base = be_string.be_str2cstr(cl.proto.source)

    # be_splitpath equivalent: find last '/' or start of string
    split_pos = base.rfind('/')
    if split_pos >= 0:
        dir_part = base[:split_pos + 1]
    else:
        dir_part = ""

    path_str = be_string.be_str2cstr(path)
    fullpath = dir_part + path_str
    return fullpath


# static char* conpath(bvm *vm, bstring *path1, bstring *path2, size_t *size)
# {
#     char *buffer;
#     int len1 = str_len(path1);
#     *size = (size_t)len1 + (size_t)str_len(path2) + 1 + SUFFIX_LEN;
#     buffer = be_malloc(vm, *size);
#     strcpy(buffer, str(path1));
#     buffer[len1] = '/';
#     strcpy(buffer + len1 + 1, str(path2));
#     return buffer;
# }
def _conpath(vm, path1, path2):
    """Concatenate two path strings with '/' separator.

    Returns the combined path string.
    """
    be_string = _lazy_be_string()
    s1 = be_string.be_str2cstr(path1)
    s2 = be_string.be_str2cstr(path2)
    return s1 + '/' + s2


# ============================================================================
# File loading helpers (from be_module.c)
# ============================================================================

# static int open_script(bvm *vm, char *path)
# {
#     int res = be_loadmodule(vm, path);
#     if (res == BE_OK)
#         be_call(vm, 0);
#     return res;
# }
def _open_script(vm, path):
    """Load and execute a Berry script file.

    Uses be_loadmodule (from be_exec) to load the file, then calls it.
    Returns BE_OK on success, or an error code.
    """
    be_exec = _lazy_be_exec()
    from berry_port import be_api
    res = be_exec.be_loadmodule(vm, path)
    if res == BE_OK:
        # Call the loaded closure with 0 arguments
        be_api.be_call(vm, 0)
    return res


# static int open_libfile(bvm *vm, char *path, size_t size)
# {
#     int res, idx = 0;
#     const char *sfxs[] = { "", ".bec", ".be" };
#     do {
#         strcpy(path + size - SUFFIX_LEN, sfxs[idx]);
#         res = open_script(vm, path);
#     } while (idx++ < 2 && res == BE_IO_ERROR);
#     if (res == BE_IO_ERROR) {
# #if BE_USE_SHARED_LIB
#         strcpy(path + size - SUFFIX_LEN, DLL_SUFFIX);
#         res = open_dllib(vm, path);
# #endif
#     }
#     be_free(vm, path, size);
#     return res;
# }
def _open_libfile(vm, basepath):
    """Try loading a script with different suffixes: '', '.bec', '.be'.

    Returns BE_OK on success, or BE_IO_ERROR if none found.
    Shared library loading is not supported in the Python port.
    If an exception occurs during loading, dump it and return the error.
    """
    from berry_port.be_api import be_dumpexcept

    suffixes = ["", ".bec", ".be"]
    res = BE_IO_ERROR
    for sfx in suffixes:
        fullpath = basepath + sfx
        res = _open_script(vm, fullpath)
        if res != BE_IO_ERROR:
            break
    # Shared library loading not supported in Python port
    if res == BE_EXCEPTION:
        be_dumpexcept(vm)
    return res


# ============================================================================
# load_path / load_cwd / load_package (from be_module.c)
# ============================================================================

# static int load_path(bvm *vm, bstring *path, bstring *mod)
# {
#     size_t size;
#     char *fullpath = conpath(vm, path, mod, &size);
#     return open_libfile(vm, fullpath, size);
# }
def _load_path(vm, path, mod):
    """Load a module from a specific search path directory."""
    fullpath = _conpath(vm, path, mod)
    return _open_libfile(vm, fullpath)


# static int load_cwd(bvm *vm, bstring *path)
# {
#     size_t size;
#     char *fullpath = fixpath(vm, path, &size);
#     return open_libfile(vm, fullpath, size);
# }
def _load_cwd(vm, path):
    """Load a module relative to the current source file's directory."""
    fullpath = _fixpath(vm, path)
    return _open_libfile(vm, fullpath)


# static int load_package(bvm *vm, bstring *path)
# {
#     int res = load_cwd(vm, path); /* load from current directory */
#     if (res == BE_IO_ERROR && vm->module.path) {
#         blist *list = vm->module.path;
#         bvalue *v = be_list_end(list) - 1;
#         bvalue *first = be_list_data(list);
#         for (; res == BE_IO_ERROR && v >= first; v--) {
#             if (var_isstr(v)) {
#                 res = load_path(vm, var_tostr(v), path);
#             }
#         }
#     }
#     return res;
# }
def _load_package(vm, path):
    """Load a module by searching the current directory then the path list.

    The path list is searched in reverse order (last added first).
    """
    res = _load_cwd(vm, path)
    if res == BE_IO_ERROR and vm.module.path is not None:
        list_ = vm.module.path
        if list_.data is not None:
            # Search in reverse order
            for i in range(list_.count - 1, -1, -1):
                v = list_.data[i]
                if var_isstr(v):
                    res = _load_path(vm, var_tostr(v), path)
                    if res != BE_IO_ERROR:
                        break
    return res


# ============================================================================
# load_native (from be_module.c)
# ============================================================================

# static int load_native(bvm *vm, bstring *path)
# {
#     const bntvmodule_t *nm = find_native(path);
#     bmodule *mod = native_module(vm, nm, NULL);
#     if (mod != NULL) {
#         /* the pointer vm->top may be changed */
#         var_setmodule(vm->top, mod);
#         be_incrtop(vm);
#         return BE_OK;
#     }
#     return BE_IO_ERROR;
# }
def _load_native(vm, path):
    """Try to load a native (built-in) module by name.

    If found, pushes the module onto the VM stack and returns BE_OK.
    Otherwise returns BE_IO_ERROR.
    """
    nm = _find_native(path)
    mod = _native_module(vm, nm, None)
    if mod is not None:
        var_setmodule(vm.stack[vm.top_idx], mod)
        vm.top_idx += 1
        return BE_OK
    return BE_IO_ERROR


# ============================================================================
# load_cached (from be_module.c)
# ============================================================================

# static bvalue* load_cached(bvm *vm, bstring *path)
# {
#     bvalue *v = NULL;
#     if (vm->module.loaded) {
#         v = be_map_findstr(vm, vm->module.loaded, path);
#         if (v) {
#             *vm->top = *v;
#             be_incrtop(vm);
#         }
#     }
#     return v;
# }
def _load_cached(vm, path):
    """Check if a module is already loaded and cached.

    If found, pushes the cached module onto the stack and returns the value.
    Otherwise returns None.
    """
    be_map = _lazy_be_map()
    if vm.module.loaded is not None:
        v = be_map.be_map_findstr(vm, vm.module.loaded, path)
        if v is not None:
            var_setval(vm.stack[vm.top_idx], v)
            vm.top_idx += 1
            return v
    return None


# ============================================================================
# be_cache_module (from be_module.c)
# ============================================================================

# void be_cache_module(bvm *vm, bstring *name)
# {
#     bvalue *v;
#     if (vm->module.loaded == NULL) {
#         vm->module.loaded = be_map_new(vm);
#     }
#     v = be_map_insertstr(vm, vm->module.loaded, name, NULL);
#     *v = vm->top[-1];
# }
def be_cache_module(vm, name):
    """Cache a loaded module in the module.loaded map.

    The module value is taken from the top of the stack (top[-1]).
    """
    be_map = _lazy_be_map()
    if vm.module.loaded is None:
        vm.module.loaded = be_map.be_map_new(vm)
    v = be_map.be_map_insertstr(vm, vm.module.loaded, name, None)
    # Copy top[-1] into the cached slot
    top_val = vm.stack[vm.top_idx - 1]
    v.type = top_val.type
    v.v = top_val.v


# ============================================================================
# module_init (from be_module.c)
# ============================================================================

# /* Try to run 'init(m)' function of module. Module is already loaded. */
# static void module_init(bvm *vm) {
#     if (be_ismodule(vm, -1)) {
#         if (be_getmember(vm, -1, "init")) {
#             /* found, call it with current module as parameter */
#             be_pushvalue(vm, -2);
#             be_call(vm, 1);
#             /* the result of init() is cached and returned */
#             be_pop(vm, 1);
#             be_remove(vm, -2);  /* remove initial module */
#         } else {
#             be_pop(vm, 1);
#         }
#     }
# }
def _module_init(vm):
    """Try to run the 'init(m)' function of a freshly loaded module.

    If the module at top-of-stack has an 'init' member, call it with
    the module as parameter. The result replaces the module on the stack.
    If no 'init' member, the module stays as-is.
    """
    be_string = _lazy_be_string()
    be_map = _lazy_be_map()
    be_exec = _lazy_be_exec()
    from berry_port import be_api

    # Check if top-of-stack is a module
    top_val = vm.stack[vm.top_idx - 1]
    if not var_ismodule(top_val):
        return

    module = var_toobj(top_val)
    init_str = be_string.be_newstr(vm, "init")

    # Look up 'init' in the module's table
    init_val = be_map.be_map_findstr(vm, module.table, init_str)
    if init_val is not None and basetype(var_type(init_val)) == BE_FUNCTION:
        # Push the init function
        var_setval(vm.stack[vm.top_idx], init_val)
        vm.top_idx += 1
        # Push the module as argument
        var_setmodule(vm.stack[vm.top_idx], module)
        vm.top_idx += 1
        # Call init(module)
        be_api.be_call(vm, 1)
        # After be_call, top_idx is restored to pre-call value (N+3).
        # Return value is at the func slot (N+1).
        # Mirror C: be_pop(vm, 1) then be_remove(vm, -2)
        vm.top_idx -= 1          # pop the arg slot -> top_idx = N+2
        # Now return value is at stack[top_idx - 1] = stack[N+1]
        # Original module is at stack[top_idx - 2] = stack[N]
        # Move return value down over the original module
        result = vm.stack[vm.top_idx - 1]
        vm.stack[vm.top_idx - 2].type = result.type
        vm.stack[vm.top_idx - 2].v = result.v
        vm.top_idx -= 1          # remove the extra slot -> top_idx = N+1
    else:
        # No init function found — if we pushed a nil for the lookup, pop it
        pass



# ============================================================================
# be_module_load (from be_module.c)
# ============================================================================

# /* load module to vm->top */
# int be_module_load(bvm *vm, bstring *path)
# {
#     int res = BE_OK;
#     if (!load_cached(vm, path)) {
#         res = load_native(vm, path);
#         if (res == BE_IO_ERROR)
#             res = load_package(vm, path);
#         if (res == BE_OK) {
#             /* on first load of the module, try running the 'init' function */
#             module_init(vm);
#             if (!nocache) { /* cache the module if it is loaded successfully */
#                 be_cache_module(vm, path);
#             }
#         }
#     }
#     return res;
# }
def be_module_load_nocache(vm, path, nocache=False):
    """Load a module by name, pushing it onto the VM stack.

    Search order:
    1. Module cache (already loaded)
    2. Native module table
    3. File-based package search (cwd, then path list)

    On first load, runs the module's 'init' function if present,
    then caches the result (unless nocache is True).

    Returns BE_OK on success, or an error code.
    """
    res = BE_OK
    if _load_cached(vm, path) is None:
        res = _load_native(vm, path)
        if res == BE_IO_ERROR:
            res = _load_package(vm, path)
        if res == BE_OK:
            _module_init(vm)
            if not nocache:
                be_cache_module(vm, path)
    return res


# /* load module to vm->top */
# int be_module_load(bvm *vm, bstring *path)
# {
#     return be_module_load_nocache(vm, path, bfalse);
# }
def be_module_load(vm, path):
    """Load a module by name with caching enabled (default behavior)."""
    return be_module_load_nocache(vm, path, False)


# ============================================================================
# be_getmodule (from be_module.c)
# ============================================================================

# BERRY_API bbool be_getmodule(bvm *vm, const char *k)
# {
#     int res = be_module_load(vm, be_newstr(vm, k));
#     return res == BE_OK;
# }
def be_getmodule(vm, k):
    """Load a module by C string name. Returns True on success."""
    be_string = _lazy_be_string()
    res = be_module_load(vm, be_string.be_newstr(vm, k))
    return res == BE_OK


# ============================================================================
# be_module_new (from be_module.c)
# ============================================================================

# bmodule* be_module_new(bvm *vm)
# {
#     bgcobject *gco = be_gcnew(vm, BE_MODULE, bmodule);
#     bmodule *obj = cast_module(gco);
#     if (obj) {
#         var_setmodule(vm->top, obj);
#         be_incrtop(vm);
#         obj->info.native = NULL;
#         obj->table = NULL; /* gc protection */
#         obj->table = be_map_new(vm);
#         be_stackpop(vm, 1);
#     }
#     return obj;
# }
def be_module_new(vm):
    """Create a new empty module with a fresh attribute map.

    The module is temporarily pushed onto the VM stack to protect it
    from GC during map creation.
    """
    be_map = _lazy_be_map()

    obj = bmodule()
    obj.type = BE_MODULE
    obj.marked = 0  # GC_WHITE

    # Push onto stack for GC protection
    var_setmodule(vm.stack[vm.top_idx], obj)
    vm.top_idx += 1

    obj.info = None  # info.native = NULL
    obj.table = None  # gc protection
    obj.table = be_map.be_map_new(vm)

    # Pop from stack
    vm.top_idx -= 1

    return obj


# ============================================================================
# be_module_delete (from be_module.c)
# ============================================================================

# void be_module_delete(bvm *vm, bmodule *module)
# {
#     be_free(vm, module, sizeof(bmodule));
# }
def be_module_delete(vm, module):
    """Free a module object.

    In the Python port, the module's map is managed by GC separately.
    We just drop the reference and let Python handle cleanup.
    """
    module.table = None
    module.info = None


# ============================================================================
# be_module_attr (from be_module.c)
# ============================================================================

# int be_module_attr(bvm *vm, bmodule *module, bstring *attr, bvalue *dst)
# {
#     bvalue *member = be_map_findstr(vm, module->table, attr);
#     if (!member) {  /* try the 'member' function */
#         /* if 'init' does not exist, don't call member() */
#         if (strcmp(str(attr), "init") == 0) {
#             var_setntvfunc(dst, be_default_init_native_function);
#             return var_primetype(dst);
#         }
#         member = be_map_findstr(vm, module->table, str_literal(vm, "member"));
#         if (member && var_basetype(member) == BE_FUNCTION) {
#             bvalue *top = vm->top;
#             top[0] = *member;
#             var_setstr(&top[1], attr);
#             vm->top += 2;   /* prevent collection results */
#             be_dofunc(vm, top, 1); /* call method 'method' */
#             vm->top -= 2;
#             *dst = *vm->top;   /* copy result to R(A) */
#
#             int type = var_type(dst);
#             if (type == BE_MODULE) {
#                 /* check if the module is named `undefined` */
#                 bmodule *mod = var_toobj(dst);
#                 if (strcmp(be_module_name(mod), "undefined") == 0) {
#                     return BE_NONE;
#                 }
#             }
#             return type;
#         }
#         return BE_NONE;
#     }
#     *dst = *member;
#     return var_type(dst);
# }
def be_module_attr(vm, module, attr, dst):
    """Look up an attribute in a module.

    First searches the module's table directly. If not found, tries
    calling the module's 'member' function (dynamic attribute lookup).
    Special case: looking up 'init' returns a default no-op function.

    Returns the type of the found attribute, or BE_NONE if not found.
    """
    be_map = _lazy_be_map()
    be_string = _lazy_be_string()

    member = be_map.be_map_findstr(vm, module.table, attr)
    if member is None:
        # Special case: 'init' returns a default no-op
        attr_str = be_string.be_str2cstr(attr)
        if attr_str == "init":
            var_setntvfunc(dst, _be_default_init_native_function)
            from berry_port.be_object import var_primetype
            return var_primetype(dst)

        # Try the 'member' function for dynamic lookup
        member_str = be_string.be_newstr(vm, "member")
        member_fn = be_map.be_map_findstr(vm, module.table, member_str)
        if member_fn is not None and basetype(var_type(member_fn)) == BE_FUNCTION:
            top_idx = vm.top_idx
            var_setval(vm.stack[top_idx], member_fn)
            var_setstr(vm.stack[top_idx + 1], attr)
            vm.top_idx += 2  # prevent collection of results

            # Call member(attr)
            from berry_port.be_vm import be_dofunc
            be_dofunc(vm, top_idx, 1)

            vm.top_idx -= 2
            # Copy result to dst
            result = vm.stack[vm.top_idx]
            dst.type = result.type
            dst.v = result.v

            type_ = var_type(dst)
            if type_ == BE_MODULE:
                # Check if the module is named 'undefined'
                mod = var_toobj(dst)
                if be_module_name(mod) == "undefined":
                    return BE_NONE
            return type_

        return BE_NONE

    # Direct member found
    dst.type = member.type
    dst.v = member.v
    return var_type(dst)


# ============================================================================
# Default init native function (no-op)
# ============================================================================

def _be_default_init_native_function(vm):
    """Default 'init' function for modules that don't define one.

    This is a no-op that returns nil.
    """
    return 0  # BE_OK, return nothing


# ============================================================================
# be_module_setmember (from be_module.c)
# ============================================================================

# bbool be_module_setmember(bvm *vm, bmodule *module, bstring *attr, bvalue *src)
# {
#     be_assert(src);
#     bmap *attrs = module->table;
#     if (!gc_isconst(attrs)) {
#         bvalue *v = be_map_findstr(vm, attrs, attr);
#         if (v == NULL) {
#             v = be_map_insertstr(vm, attrs, attr, NULL);
#         }
#         if (v) {
#             *v = *src;
#             return btrue;
#         }
#     } else {
#         /* if not writable, try 'setmember' */
#         int type = be_module_attr(vm, module, str_literal(vm, "setmember"), vm->top);
#         if (basetype(type) == BE_FUNCTION) {
#             bvalue *top = vm->top;
#             // top[0] already has 'setmember'
#             var_setstr(&top[1], attr);  /* attribute name */
#             top[2] = *src;  /* new value */
#             vm->top += 3;   /* prevent collection results */
#             be_dofunc(vm, top, 2); /* call method 'setmember' */
#             vm->top -= 3;
#             int type = var_type(vm->top);
#             if (type == BE_BOOL) {
#                 bbool ret = var_tobool(vm->top);
#                 if (!ret) {
#                     return bfalse;
#                 }
#             } else if (type == BE_MODULE) {
#                 /* check if the module is named `undefined` */
#                 bmodule *mod = var_toobj(vm->top);
#                 if (strcmp(be_module_name(mod), "undefined") == 0) {
#                     return bfalse;
#                 }
#             }
#             return btrue;
#         }
#     }
#     return bfalse;
# }
def be_module_setmember(vm, module, attr, src):
    """Set a member attribute in a module.

    If the module's table is not const, directly insert/update the value.
    If the table is const (solidified module), try calling the module's
    'setmember' function for dynamic attribute setting.

    Returns True on success, False on failure.
    """
    be_map = _lazy_be_map()
    be_string = _lazy_be_string()

    assert src is not None
    attrs = module.table
    if not gc_isconst(attrs):
        v = be_map.be_map_findstr(vm, attrs, attr)
        if v is None:
            v = be_map.be_map_insertstr(vm, attrs, attr, None)
        if v is not None:
            v.type = src.type
            v.v = src.v
            return True
    else:
        # Const table — try 'setmember' dynamic dispatch
        setmember_str = be_string.be_newstr(vm, "setmember")
        top_idx = vm.top_idx
        type_ = be_module_attr(vm, module, setmember_str, vm.stack[top_idx])
        if basetype(type_) == BE_FUNCTION:
            # top[0] already has 'setmember' function
            var_setstr(vm.stack[top_idx + 1], attr)  # attribute name
            vm.stack[top_idx + 2].type = src.type     # new value
            vm.stack[top_idx + 2].v = src.v
            vm.top_idx += 3  # prevent collection

            from berry_port.be_vm import be_dofunc
            be_dofunc(vm, top_idx, 2)

            vm.top_idx -= 3
            result = vm.stack[vm.top_idx]
            result_type = var_type(result)
            if result_type == BE_BOOL:
                ret = var_tobool(result)
                if not ret:
                    return False
            elif result_type == BE_MODULE:
                mod = var_toobj(result)
                if be_module_name(mod) == "undefined":
                    return False
            return True

    return False



# ============================================================================
# be_module_name (from be_module.c)
# ============================================================================

# const char* be_module_name(bmodule *module)
# {
#     if (gc_isconst(module)) {
#         return module->info.name;
#     }
#     if (gc_exmark(module) & BE_MODULE_NAME) {
#         return str(module->info.sname);
#     }
#     if (module->info.native) {
#         return module->info.native->name;
#     }
#     return NULL;
# }
def be_module_name(module):
    """Get the name of a module as a Python string.

    For const modules (solidified), the name is stored directly as a string.
    For runtime modules with BE_MODULE_NAME flag, the name is a bstring.
    For native modules, the name comes from the native module descriptor.
    Returns None if no name is available.
    """
    be_string = _lazy_be_string()

    if gc_isconst(module):
        # Const module: info is a string (const char* name)
        if module.info is not None:
            if isinstance(module.info, str):
                return module.info
            # Could be a bntvmodule with .name
            if hasattr(module.info, 'name'):
                return module.info.name
        return None

    if gc_exmark(module) & BE_MODULE_NAME:
        # Runtime module with name set via be_module_setname
        # info holds a bstring (sname)
        if module.info is not None:
            return be_string.be_str2cstr(module.info)
        return None

    if module.info is not None:
        # Native module: info is a bntvmodule descriptor
        if hasattr(module.info, 'name'):
            return module.info.name

    return None


# ============================================================================
# be_module_setname (from be_module.c)
# ============================================================================

# bbool be_module_setname(bmodule *module, bstring *name)
# {
#     if (!gc_isconst(module)) {
#         module->info.sname = name;
#         gc_setexmark(module, BE_MODULE_NAME);
#         return btrue;
#     }
#     return bfalse;
# }
def be_module_setname(module, name):
    """Set the name of a non-const module.

    The name is stored as a bstring in the info field, and the
    BE_MODULE_NAME flag is set in the extra mark bits.

    Returns True on success, False if the module is const.
    """
    if not gc_isconst(module):
        module.info = name  # info.sname = name
        gc_setexmark(module, BE_MODULE_NAME)
        return True
    return False


# ============================================================================
# Path list management (from be_module.c)
# ============================================================================

# static blist* pathlist(bvm *vm)
# {
#     if (!vm->module.path) {
#         vm->module.path = be_list_new(vm);
#     }
#     return vm->module.path;
# }
def _pathlist(vm):
    """Get or create the module search path list."""
    be_list = _lazy_be_list()
    if vm.module.path is None:
        vm.module.path = be_list.be_list_new(vm)
    return vm.module.path


# /* push the path list to the top */
# BERRY_API void be_module_path(bvm *vm)
# {
#     blist *list = pathlist(vm);
#     bvalue *reg = be_incrtop(vm);
#     var_setlist(reg, list);
# }
def be_module_path(vm):
    """Push the module search path list onto the VM stack."""
    from berry_port.be_object import var_setlist
    list_ = _pathlist(vm)
    var_setlist(vm.stack[vm.top_idx], list_)
    vm.top_idx += 1


# BERRY_API void be_module_path_set(bvm *vm, const char *path)
# {
#     blist *list = pathlist(vm);
#     bvalue *value = be_list_push(vm, list, NULL);
#     var_setnil(value);
#     var_setstr(value, be_newstr(vm, path))
# }
def be_module_path_set(vm, path):
    """Add a directory to the module search path list."""
    be_list = _lazy_be_list()
    be_string = _lazy_be_string()

    list_ = _pathlist(vm)
    value = be_list.be_list_push(vm, list_, None)
    var_setnil(value)
    var_setstr(value, be_string.be_newstr(vm, path))


# ============================================================================
# Shared library support — NOT ported (Python port is pure Python)
# ============================================================================

# #if BE_USE_SHARED_LIB
# ...
# BERRY_API int be_loadlib(bvm *vm, const char *path)
# ...
# #endif
def be_loadlib(vm, path):
    """Load a shared library module.

    Not supported in the Python port — always returns BE_IO_ERROR.
    """
    return BE_IO_ERROR
