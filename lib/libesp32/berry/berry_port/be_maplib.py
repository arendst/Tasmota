"""
berry_port/be_maplib.py — Port of src/be_maplib.c

Map class methods: init, tostring, remove, item, setitem, find,
contains, insert, size, tobool, iter, keys.

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

def _lazy_be_func():
    import berry_port.be_func as m; return m

def _lazy_be_strlib():
    import berry_port.be_strlib as m; return m


# ============================================================================
# Helpers (macros from C)
# ============================================================================

# #define map_check_data(vm, argc)
#     if (!be_ismap(vm, -1) || be_top(vm) - 1 < argc) {
#         be_return_nil(vm);
#     }
def _map_check_data(vm, argc):
    """Return True if check passes, False if caller should return nil."""
    be_api = _lazy_be_api()
    if not be_api.be_ismap(vm, -1) or be_api.be_top(vm) - 1 < argc:
        return False
    return True

# #define map_check_ref(vm)
#     if (be_refcontains(vm, 1)) {
#         be_pushstring(vm, "{...}");
#         be_return(vm);
#     }
def _map_check_ref(vm):
    """Return True if circular ref detected (caller should return)."""
    be_api = _lazy_be_api()
    if be_api.be_refcontains(vm, 1):
        be_api.be_pushstring(vm, "{...}")
        return True
    return False


# ============================================================================
# m_init
# ============================================================================

# static int m_init(bvm *vm)
# {
#     if (be_top(vm) > 1 && be_ismap(vm, 2)) {
#         be_pushvalue(vm, 2);
#         be_setmember(vm, 1, ".p");
#     } else {
#         be_newmap(vm);
#         be_setmember(vm, 1, ".p");
#     }
#     be_return_nil(vm);
# }
def m_init(vm):
    be_api = _lazy_be_api()
    if be_api.be_top(vm) > 1 and be_api.be_ismap(vm, 2):
        be_api.be_pushvalue(vm, 2)
        be_api.be_setmember(vm, 1, ".p")
    else:
        be_api.be_newmap(vm)
        be_api.be_setmember(vm, 1, ".p")
    return be_api.be_returnnilvalue(vm)


# ============================================================================
# push_key / push_value helpers for m_tostring
# ============================================================================

# static void push_key(bvm *vm)
# {
#     be_toescape(vm, -2, 'x'); /* escape string */
#     be_pushvalue(vm, -2); /* push to top */
#     be_strconcat(vm, -5);
#     be_pop(vm, 1);
# }
def push_key(vm):
    be_api = _lazy_be_api()
    be_strlib = _lazy_be_strlib()
    be_strlib.be_toescape(vm, -2, 'x')  # escape string
    be_api.be_pushvalue(vm, -2)  # push to top
    be_api.be_strconcat(vm, -5)
    be_api.be_pop(vm, 1)

# static void push_value(bvm *vm)
# {
#     be_toescape(vm, -1, 'x'); /* escape string */
#     be_strconcat(vm, -4);
#     be_pop(vm, 2);
#     if (be_iter_hasnext(vm, -3)) {
#         be_pushstring(vm, ", ");
#         be_strconcat(vm, -3);
#         be_pop(vm, 1);
#     }
# }
def push_value(vm):
    be_api = _lazy_be_api()
    be_strlib = _lazy_be_strlib()
    be_strlib.be_toescape(vm, -1, 'x')  # escape string
    be_api.be_strconcat(vm, -4)
    be_api.be_pop(vm, 2)
    if be_api.be_iter_hasnext(vm, -3):
        be_api.be_pushstring(vm, ", ")
        be_api.be_strconcat(vm, -3)
        be_api.be_pop(vm, 1)


# ============================================================================
# m_tostring
# ============================================================================

# static int m_tostring(bvm *vm)
# {
#     be_getmember(vm, 1, ".p");
#     map_check_data(vm, 1);
#     map_check_ref(vm);
#     be_refpush(vm, 1);
#     be_pushstring(vm, "{");
#     be_pushiter(vm, -2); /* map iterator use 1 register */
#     while (be_iter_hasnext(vm, -3)) {
#         be_iter_next(vm, -3);
#         push_key(vm); /* key.tostring() */
#         be_pushstring(vm, ": "); /* add ': ' */
#         be_strconcat(vm, -5);
#         be_pop(vm, 1);
#         push_value(vm); /* value.tostring() */
#     }
#     be_pop(vm, 1); /* pop iterator */
#     be_pushstring(vm, "}");
#     be_strconcat(vm, -2);
#     be_pop(vm, 1);
#     be_refpop(vm);
#     be_return(vm);
# }
def m_tostring(vm):
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, ".p")
    if not _map_check_data(vm, 1):
        return be_api.be_returnnilvalue(vm)
    if _map_check_ref(vm):
        return be_api.be_returnvalue(vm)
    be_api.be_refpush(vm, 1)
    be_api.be_pushstring(vm, "{")
    be_api.be_pushiter(vm, -2)  # map iterator uses 1 register
    while be_api.be_iter_hasnext(vm, -3):
        be_api.be_iter_next(vm, -3)
        push_key(vm)  # key.tostring()
        be_api.be_pushstring(vm, ": ")  # add ': '
        be_api.be_strconcat(vm, -5)
        be_api.be_pop(vm, 1)
        push_value(vm)  # value.tostring()
    be_api.be_pop(vm, 1)  # pop iterator
    be_api.be_pushstring(vm, "}")
    be_api.be_strconcat(vm, -2)
    be_api.be_pop(vm, 1)
    be_api.be_refpop(vm)
    return be_api.be_returnvalue(vm)


# ============================================================================
# m_remove
# ============================================================================

# static int m_remove(bvm *vm)
# {
#     be_getmember(vm, 1, ".p");
#     map_check_data(vm, 2);
#     be_pushvalue(vm, 2);
#     be_data_remove(vm, -2);
#     be_return_nil(vm);
# }
def m_remove(vm):
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, ".p")
    if not _map_check_data(vm, 2):
        return be_api.be_returnnilvalue(vm)
    be_api.be_pushvalue(vm, 2)
    be_api.be_data_remove(vm, -2)
    return be_api.be_returnnilvalue(vm)


# ============================================================================
# m_item
# ============================================================================

# static int m_item(bvm *vm)
# {
#     be_getmember(vm, 1, ".p");
#     map_check_data(vm, 2);
#     be_pushvalue(vm, 2);
#     if (!be_getindex(vm, -2)) {
#         be_raise(vm, "key_error", be_tostring(vm, 2));
#     }
#     be_return(vm);
# }
def m_item(vm):
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, ".p")
    if not _map_check_data(vm, 2):
        return be_api.be_returnnilvalue(vm)
    be_api.be_pushvalue(vm, 2)
    if not be_api.be_getindex(vm, -2):
        be_api.be_raise(vm, "key_error", be_api.be_tostring(vm, 2))
    return be_api.be_returnvalue(vm)


# ============================================================================
# m_setitem
# ============================================================================

# static int m_setitem(bvm *vm)
# {
#     be_getmember(vm, 1, ".p");
#     map_check_data(vm, 3);
#     be_pushvalue(vm, 2);
#     be_pushvalue(vm, 3);
#     be_setindex(vm, -3);
#     be_return_nil(vm);
# }
def m_setitem(vm):
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, ".p")
    if not _map_check_data(vm, 3):
        return be_api.be_returnnilvalue(vm)
    be_api.be_pushvalue(vm, 2)
    be_api.be_pushvalue(vm, 3)
    be_api.be_setindex(vm, -3)
    return be_api.be_returnnilvalue(vm)


# ============================================================================
# m_find
# ============================================================================

# static int m_find(bvm *vm)
# {
#     int argc = be_top(vm);
#     be_getmember(vm, 1, ".p");
#     map_check_data(vm, 2);
#     be_pushvalue(vm, 2);
#     /* not find and has default value */
#     if (!be_getindex(vm, -2) && argc >= 3) {
#         be_pushvalue(vm, 3);
#     }
#     be_return(vm);
# }
def m_find(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    be_api.be_getmember(vm, 1, ".p")
    if not _map_check_data(vm, 2):
        return be_api.be_returnnilvalue(vm)
    be_api.be_pushvalue(vm, 2)
    # not found and has default value
    if not be_api.be_getindex(vm, -2) and argc >= 3:
        be_api.be_pushvalue(vm, 3)
    return be_api.be_returnvalue(vm)


# ============================================================================
# m_contains
# ============================================================================

# static int m_contains(bvm *vm)
# {
#     be_getmember(vm, 1, ".p");
#     map_check_data(vm, 2);
#     be_pushvalue(vm, 2);
#     be_pushbool(vm, be_getindex(vm, -2));
#     be_return(vm);
# }
def m_contains(vm):
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, ".p")
    if not _map_check_data(vm, 2):
        return be_api.be_returnnilvalue(vm)
    be_api.be_pushvalue(vm, 2)
    be_api.be_pushbool(vm, be_api.be_getindex(vm, -2))
    return be_api.be_returnvalue(vm)


# ============================================================================
# m_insert
# ============================================================================

# static int m_insert(bvm *vm)
# {
#     bbool res;
#     be_getmember(vm, 1, ".p");
#     map_check_data(vm, 3);
#     be_pushvalue(vm, 2);
#     be_pushvalue(vm, 3);
#     res = be_data_insert(vm, -3);
#     be_pushbool(vm, res);
#     be_return(vm);
# }
def m_insert(vm):
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, ".p")
    if not _map_check_data(vm, 3):
        return be_api.be_returnnilvalue(vm)
    be_api.be_pushvalue(vm, 2)
    be_api.be_pushvalue(vm, 3)
    res = be_api.be_data_insert(vm, -3)
    be_api.be_pushbool(vm, res)
    return be_api.be_returnvalue(vm)


# ============================================================================
# m_size
# ============================================================================

# static int m_size(bvm *vm)
# {
#     be_getmember(vm, 1, ".p");
#     map_check_data(vm, 1);
#     be_pushint(vm, be_data_size(vm, -1));
#     be_return(vm);
# }
def m_size(vm):
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, ".p")
    if not _map_check_data(vm, 1):
        return be_api.be_returnnilvalue(vm)
    be_api.be_pushint(vm, be_api.be_data_size(vm, -1))
    return be_api.be_returnvalue(vm)


# ============================================================================
# m_tobool
# ============================================================================

# static int m_tobool(bvm *vm)
# {
#     be_getmember(vm, 1, ".p");
#     map_check_data(vm, 1);
#     be_pushbool(vm, be_data_size(vm, -1) > 0);
#     be_return(vm);
# }
def m_tobool(vm):
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, ".p")
    if not _map_check_data(vm, 1):
        return be_api.be_returnnilvalue(vm)
    be_api.be_pushbool(vm, be_api.be_data_size(vm, -1) > 0)
    return be_api.be_returnvalue(vm)


# ============================================================================
# iter_closure — value iterator for maps
# ============================================================================

# static int iter_closure(bvm *vm)
# {
#     /* for better performance, we operate the upvalues
#      * directly without using by the stack. */
#     bntvclos *func = var_toobj(vm->cf->func);
#     bvalue *uv0 = be_ntvclos_upval(func, 0)->value; /* list value */
#     bvalue *uv1 = be_ntvclos_upval(func, 1)->value; /* iter value */
#     bmapiter iter = var_toobj(uv1);
#     bmapnode *next = be_map_next(var_toobj(uv0), &iter);
#     if (next == NULL) {
#         be_stop_iteration(vm);
#         be_return_nil(vm); /* will not be executed */
#     }
#     var_setobj(uv1, BE_COMPTR, iter); /* set upvale[1] (iter value) */
#     /* push next value to top */
#     var_setval(vm->top, &next->value);
#     be_incrtop(vm);
#     be_return(vm);
# }
def iter_closure(vm):
    be_api = _lazy_be_api()
    be_obj = _lazy_be_object()
    be_func = _lazy_be_func()
    be_mp = _lazy_be_map()

    func = be_obj.var_toobj(vm.stack[vm.cf.func])
    uv0 = be_func.be_ntvclos_upval(func, 0).value  # map value
    uv1 = be_func.be_ntvclos_upval(func, 1).value  # iter value

    # In Python, the iterator is stored as an integer index (or None).
    # be_map_next expects iter_ref = [current_index_or_None].
    cur_iter = be_obj.var_toobj(uv1)
    iter_ref = [cur_iter]
    mp = be_obj.var_toobj(uv0)
    next_node, iter_ref = be_mp.be_map_next(mp, iter_ref)

    if next_node is None:
        be_api.be_stop_iteration(vm)
        return be_api.be_returnnilvalue(vm)  # will not be executed

    # Update the iterator upvalue
    be_obj.var_setobj(uv1, be_obj.BE_COMPTR, iter_ref[0])

    # Push next value to top
    be_obj.var_setval(vm.stack[vm.top_idx], next_node.value)
    be_api.be_incrtop(vm)
    return be_api.be_returnvalue(vm)


# ============================================================================
# m_iter
# ============================================================================

# static int m_iter(bvm *vm)
# {
#     be_pushntvclosure(vm, iter_closure, 2);
#     be_getmember(vm, 1, ".p");
#     be_setupval(vm, -2, 0);
#     be_pushiter(vm, -1);
#     be_setupval(vm, -3, 1);
#     be_pop(vm, 2);
#     be_return(vm);
# }
def m_iter(vm):
    be_api = _lazy_be_api()
    be_api.be_pushntvclosure(vm, iter_closure, 2)
    be_api.be_getmember(vm, 1, ".p")
    be_api.be_setupval(vm, -2, 0)
    be_api.be_pushiter(vm, -1)
    be_api.be_setupval(vm, -3, 1)
    be_api.be_pop(vm, 2)
    return be_api.be_returnvalue(vm)


# ============================================================================
# keys_iter_closure — key iterator for maps
# ============================================================================

# static int keys_iter_closure(bvm *vm)
# {
#     /* for better performance, we operate the upvalues
#      * directly without using by the stack. */
#     bntvclos *func = var_toobj(vm->cf->func);
#     bvalue *uv0 = be_ntvclos_upval(func, 0)->value; /* list value */
#     bvalue *uv1 = be_ntvclos_upval(func, 1)->value; /* iter value */
#     bmapiter iter = var_toobj(uv1);
#     bmapnode *next = be_map_next(var_toobj(uv0), &iter);
#     if (next == NULL) {
#         be_stop_iteration(vm);
#         be_return_nil(vm); /* will not be executed */
#     }
#     var_setobj(uv1, BE_COMPTR, iter); /* set upvale[1] (iter value) */
#     /* push next value to top */
#     var_setobj(vm->top, next->key.type, next->key.v.p);
#     be_incrtop(vm);
#     be_return(vm);
# }
def keys_iter_closure(vm):
    be_api = _lazy_be_api()
    be_obj = _lazy_be_object()
    be_func = _lazy_be_func()
    be_mp = _lazy_be_map()

    func = be_obj.var_toobj(vm.stack[vm.cf.func])
    uv0 = be_func.be_ntvclos_upval(func, 0).value  # map value
    uv1 = be_func.be_ntvclos_upval(func, 1).value  # iter value

    cur_iter = be_obj.var_toobj(uv1)
    iter_ref = [cur_iter]
    mp = be_obj.var_toobj(uv0)
    next_node, iter_ref = be_mp.be_map_next(mp, iter_ref)

    if next_node is None:
        be_api.be_stop_iteration(vm)
        return be_api.be_returnnilvalue(vm)  # will not be executed

    # Update the iterator upvalue
    be_obj.var_setobj(uv1, be_obj.BE_COMPTR, iter_ref[0])

    # Push next key to top — copy key fields into a bvalue
    dst = vm.stack[vm.top_idx]
    dst.type = next_node.key.type
    dst.v = next_node.key.v
    be_api.be_incrtop(vm)
    return be_api.be_returnvalue(vm)


# ============================================================================
# m_keys
# ============================================================================

# static int m_keys(bvm *vm)
# {
#     be_pushntvclosure(vm, keys_iter_closure, 2);
#     be_getmember(vm, 1, ".p");
#     be_setupval(vm, -2, 0);
#     be_pushiter(vm, -1);
#     be_setupval(vm, -3, 1);
#     be_pop(vm, 2);
#     be_return(vm);
# }
def m_keys(vm):
    be_api = _lazy_be_api()
    be_api.be_pushntvclosure(vm, keys_iter_closure, 2)
    be_api.be_getmember(vm, 1, ".p")
    be_api.be_setupval(vm, -2, 0)
    be_api.be_pushiter(vm, -1)
    be_api.be_setupval(vm, -3, 1)
    be_api.be_pop(vm, 2)
    return be_api.be_returnvalue(vm)


# ============================================================================
# be_load_maplib — register the map class
# ============================================================================

# void be_load_maplib(bvm *vm)
# {
#     static const bnfuncinfo members[] = {
#         { ".p", NULL },
#         { "init", m_init },
#         { "tostring", m_tostring },
#         { "remove", m_remove },
#         { "item", m_item },
#         { "setitem", m_setitem },
#         { "find", m_find },
#         { "contains", m_contains },
#         { "size", m_size },
#         { "insert", m_insert },
#         { "iter", m_iter },
#         { "keys", m_keys },
#         { "tobool", m_tobool },
#         { NULL, NULL }
#     };
#     be_regclass(vm, "map", members);
# }
def be_load_maplib(vm):
    be_api = _lazy_be_api()
    members = [
        (".p", None),
        ("init", m_init),
        ("tostring", m_tostring),
        ("remove", m_remove),
        ("item", m_item),
        ("setitem", m_setitem),
        ("find", m_find),
        ("contains", m_contains),
        ("size", m_size),
        ("insert", m_insert),
        ("iter", m_iter),
        ("keys", m_keys),
        ("tobool", m_tobool),
    ]
    be_api.be_regclass(vm, "map", members)
