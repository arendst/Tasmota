"""
berry_port/be_preproclib.py — Port of src/be_preproclib.c + src/be_preproc.c

Native module `preproc` — exposes the VM's preprocessor macro map to Berry
scripts, so that `#define` / `#undef` directives (consumed by the lexer) can
be introspected and mutated at runtime. Gated by BE_USE_PREPROCESSOR.

Original C code is included as comments.
"""

# /********************************************************************
# ** Copyright (c) 2018-2020 Guan Wenliang
# ** This file is part of the Berry default interpreter.
# ** skiars@qq.com, https://github.com/Skiars/berry
# ** See Copyright Notice in the LICENSE file or at
# ** https://github.com/Skiars/berry/blob/master/LICENSE
# ********************************************************************/


# ============================================================================
# Imports — lazy to avoid circular dependencies
# ============================================================================

def _lazy_be_api():
    import berry_port.be_api as m; return m

def _lazy_be_object():
    import berry_port.be_object as m; return m

def _lazy_be_map():
    import berry_port.be_map as m; return m

def _lazy_be_string():
    import berry_port.be_string as m; return m


# ============================================================================
# Internal helpers (from be_preproc.c)
# ============================================================================

# static bmap* pp_ensure_map(bvm *vm)
def _pp_ensure_map(vm):
    """Ensure the VM's preprocessor map exists, lazily allocating on first use."""
    be_map = _lazy_be_map()
    if getattr(vm, 'preprocessor', None) is None:
        vm.preprocessor = be_map.be_map_new(vm)
    return vm.preprocessor


# static bbool pp_parse_int(const char *s, int len, bint *out)
def _pp_parse_int(s):
    """Try to parse a string as an integer. Returns (True, value) or (False, 0)."""
    length = len(s)
    if length == 0:
        return False, 0
    i = 0
    neg = False
    if s[0] == '-':
        neg = True
        i = 1
    elif s[0] == '+':
        i = 1
    if i >= length:
        return False, 0
    val = 0
    while i < length:
        if s[i] < '0' or s[i] > '9':
            return False, 0
        val = val * 10 + (ord(s[i]) - ord('0'))
        i += 1
    return True, (-val if neg else val)


# static void pp_store_value(bvm *vm, bmap *map, bstring *key, const char *value)
def _pp_store_value(vm, pp_map, key, value):
    """Store a macro value following ingestion rules:
    - None         -> BE_INT 1 (truthy empty define)
    - quoted str   -> BE_STRING with quotes stripped
    - numeric str  -> BE_INT with parsed value
    - otherwise    -> BE_STRING with raw value
    """
    be_obj = _lazy_be_object()
    be_map = _lazy_be_map()
    be_string = _lazy_be_string()

    val = be_obj.bvalue()
    if value is None:
        be_obj.var_setint(val, 1)
    else:
        length = len(value)
        if length >= 2 and (
            (value[0] == '"' and value[-1] == '"') or
            (value[0] == "'" and value[-1] == "'")
        ):
            # strip quotes and store as string
            s = be_string.be_newstrn(vm, value[1:-1], length - 2)
            be_obj.var_setstr(val, s)
        else:
            ok, ival = _pp_parse_int(value)
            if ok:
                be_obj.var_setint(val, ival)
            else:
                s = be_string.be_newstrn(vm, value, length)
                be_obj.var_setstr(val, s)
    be_map.be_map_insertstr(vm, pp_map, key, val)


# ============================================================================
# BERRY_API functions (from be_preproc.c)
# ============================================================================

# BERRY_API void be_preprocessor_define(bvm *vm, const char *name, const char *value)
def be_preprocessor_define(vm, name, value):
    be_string = _lazy_be_string()
    pp_map = _pp_ensure_map(vm)
    key = be_string.be_newstr(vm, name)
    _pp_store_value(vm, pp_map, key, value)


# BERRY_API void be_preprocessor_undef(bvm *vm, const char *name)
def be_preprocessor_undef(vm, name):
    be_string = _lazy_be_string()
    be_map = _lazy_be_map()
    if getattr(vm, 'preprocessor', None) is not None:
        key = be_string.be_newstr(vm, name)
        be_map.be_map_removestr(vm, vm.preprocessor, key)


# BERRY_API void be_preprocessor_clear(bvm *vm)
def be_preprocessor_clear(vm):
    """Detach the map from the VM. The GC will collect it."""
    vm.preprocessor = None


# ============================================================================
# Native module methods (from be_preproclib.c)
# ============================================================================

# static int m_define(bvm *vm)
def m_define(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    if argc >= 1 and be_api.be_isstring(vm, 1):
        name = be_api.be_tostring(vm, 1)
        value = None
        if argc >= 2:
            value = be_api.be_tostring(vm, 2)
        be_preprocessor_define(vm, name, value)
        return be_api.be_returnnilvalue(vm)
    be_api.be_raise(vm, "type_error", "name must be a string")
    return be_api.be_returnnilvalue(vm)


# static int m_undef(bvm *vm)
def m_undef(vm):
    be_api = _lazy_be_api()
    if be_api.be_top(vm) >= 1 and be_api.be_isstring(vm, 1):
        be_preprocessor_undef(vm, be_api.be_tostring(vm, 1))
        return be_api.be_returnnilvalue(vm)
    be_api.be_raise(vm, "type_error", "name must be a string")
    return be_api.be_returnnilvalue(vm)


# static int m_defined(bvm *vm)
def m_defined(vm):
    be_api = _lazy_be_api()
    be_map = _lazy_be_map()
    be_string = _lazy_be_string()
    if be_api.be_top(vm) >= 1 and be_api.be_isstring(vm, 1):
        name = be_api.be_tostring(vm, 1)
        found = False
        if getattr(vm, 'preprocessor', None) is not None:
            key = be_string.be_newstr(vm, name)
            found = be_map.be_map_findstr(vm, vm.preprocessor, key) is not None
        be_api.be_pushbool(vm, found)
        return be_api.be_returnvalue(vm)
    be_api.be_raise(vm, "type_error", "name must be a string")
    return be_api.be_returnnilvalue(vm)


# static int m_value(bvm *vm)
def m_value(vm):
    be_api = _lazy_be_api()
    be_obj = _lazy_be_object()
    be_map = _lazy_be_map()
    be_string = _lazy_be_string()
    if be_api.be_top(vm) >= 1 and be_api.be_isstring(vm, 1):
        name = be_api.be_tostring(vm, 1)
        if getattr(vm, 'preprocessor', None) is not None:
            key = be_string.be_newstr(vm, name)
            val = be_map.be_map_findstr(vm, vm.preprocessor, key)
            if val is not None:
                if be_obj.var_isint(val):
                    be_api.be_pushint(vm, be_obj.var_toint(val))
                elif be_obj.var_isstr(val):
                    be_api.be_pushstring(vm, be_string.be_str2cstr(be_obj.var_tostr(val)))
                else:
                    be_api.be_pushnil(vm)
                return be_api.be_returnvalue(vm)
        be_api.be_pushnil(vm)
        return be_api.be_returnvalue(vm)
    be_api.be_raise(vm, "type_error", "name must be a string")
    return be_api.be_returnnilvalue(vm)


# static int m_clear(bvm *vm)
def m_clear(vm):
    be_api = _lazy_be_api()
    be_preprocessor_clear(vm)
    return be_api.be_returnnilvalue(vm)


# ============================================================================
# Module table builder
# ============================================================================

# be_native_module_attr_table(preproc) { ... }
def be_preproc_module_table():
    """Return the attribute table for the preproc module."""
    return [
        ("define", m_define),
        ("undef", m_undef),
        ("defined", m_defined),
        ("value", m_value),
        ("clear", m_clear),
    ]
