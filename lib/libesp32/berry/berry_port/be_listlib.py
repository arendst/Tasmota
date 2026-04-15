"""
berry_port/be_listlib.py — Port of src/be_listlib.c

List class methods: init, tostring, push, pop, insert, remove, item,
find, setitem, size, tobool, resize, clear, iter, connect, merge,
concat, reverse, copy, keys, equal, nequal.

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

def _lazy_be_list():
    import berry_port.be_list as m; return m

def _lazy_be_strlib():
    import berry_port.be_strlib as m; return m

def _lazy_be_func():
    import berry_port.be_func as m; return m

def _lazy_be_string():
    import berry_port.be_string as m; return m

def _lazy_be_vm():
    import berry_port.be_vm as m; return m


# ============================================================================
# Helpers (macros from C)
# ============================================================================

# #define list_check_data(vm, argc)
#     if (!be_islist(vm, -1) || be_top(vm) - 1 < argc) {
#         be_return_nil(vm);
#     }
def _list_check_data(vm, argc):
    """Return True if check passes, False if we should return nil."""
    be_api = _lazy_be_api()
    if not be_api.be_islist(vm, -1) or be_api.be_top(vm) - 1 < argc:
        return False
    return True


# #define list_check_ref(vm)
#     if (be_refcontains(vm, 1)) {
#         be_pushstring(vm, "[...]");
#         be_return(vm);
#     }
def _list_check_ref(vm):
    """Return True if circular ref detected (caller should return)."""
    be_api = _lazy_be_api()
    if be_api.be_refcontains(vm, 1):
        be_api.be_pushstring(vm, "[...]")
        return True
    return False


# ============================================================================
# list_getindex — helper
# ============================================================================

# static void list_getindex(bvm *vm, int index)
# {
#     if (!be_getindex(vm, index)) {
#         be_raise(vm, "index_error", "list index out of range");
#     }
# }
def list_getindex(vm, index):
    be_api = _lazy_be_api()
    if not be_api.be_getindex(vm, index):
        be_api.be_raise(vm, "index_error", "list index out of range")


# ============================================================================
# m_init
# ============================================================================

# static int m_init(bvm *vm)
# {
#     int i, argc = be_top(vm);
#     if (argc > 1 && be_islist(vm, 2)) {
#         be_pushvalue(vm, 2);
#         be_setmember(vm, 1, ".p");
#     } else {
#         be_newlist(vm);
#         be_setmember(vm, 1, ".p");
#         for (i = 2; i <= argc; ++i) {
#             be_pushvalue(vm, i);
#             be_data_push(vm, -2);
#             be_pop(vm, 1);
#         }
#     }
#     be_return_nil(vm);
# }
def m_init(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    if argc > 1 and be_api.be_islist(vm, 2):
        be_api.be_pushvalue(vm, 2)
        be_api.be_setmember(vm, 1, ".p")
    else:
        be_api.be_newlist(vm)
        be_api.be_setmember(vm, 1, ".p")
        for i in range(2, argc + 1):
            be_api.be_pushvalue(vm, i)
            be_api.be_data_push(vm, -2)
            be_api.be_pop(vm, 1)
    return be_api.be_returnnilvalue(vm)


# ============================================================================
# push_element — helper for m_tostring
# ============================================================================

# static void push_element(bvm *vm)
# {
#     be_toescape(vm, -1, 'x'); /* escape string */
#     be_strconcat(vm, -3);
#     be_pop(vm, 1);
# }
def push_element(vm):
    be_strlib = _lazy_be_strlib()
    be_api = _lazy_be_api()
    be_strlib.be_toescape(vm, -1, 'x')  # escape string
    be_api.be_strconcat(vm, -3)
    be_api.be_pop(vm, 1)


# ============================================================================
# m_tostring
# ============================================================================

# static int m_tostring(bvm *vm)
# {
#     be_getmember(vm, 1, ".p");
#     list_check_data(vm, 1);
#     list_check_ref(vm);
#     be_refpush(vm, 1);
#     be_pushstring(vm, "[");
#     be_pushiter(vm, -2);
#     while (be_iter_hasnext(vm, -3)) {
#         be_iter_next(vm, -3);
#         push_element(vm);
#         if (be_iter_hasnext(vm, -3)) {
#             be_pushstring(vm, ", ");
#             be_strconcat(vm, -3);
#             be_pop(vm, 1);
#         }
#     }
#     be_pop(vm, 1); /* pop iterator */
#     be_pushstring(vm, "]");
#     be_strconcat(vm, -2);
#     be_pop(vm, 1);
#     be_refpop(vm);
#     be_return(vm);
# }
def m_tostring(vm):
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, ".p")
    if not _list_check_data(vm, 1):
        return be_api.be_returnnilvalue(vm)
    if _list_check_ref(vm):
        return be_api.be_returnvalue(vm)
    be_api.be_refpush(vm, 1)
    be_api.be_pushstring(vm, "[")
    be_api.be_pushiter(vm, -2)
    while be_api.be_iter_hasnext(vm, -3):
        be_api.be_iter_next(vm, -3)
        push_element(vm)
        if be_api.be_iter_hasnext(vm, -3):
            be_api.be_pushstring(vm, ", ")
            be_api.be_strconcat(vm, -3)
            be_api.be_pop(vm, 1)
    be_api.be_pop(vm, 1)  # pop iterator
    be_api.be_pushstring(vm, "]")
    be_api.be_strconcat(vm, -2)
    be_api.be_pop(vm, 1)
    be_api.be_refpop(vm)
    return be_api.be_returnvalue(vm)


# ============================================================================
# m_push
# ============================================================================

# static int m_push(bvm *vm)
# {
#     be_getmember(vm, 1, ".p");
#     list_check_data(vm, 2);
#     be_pushvalue(vm, 2);
#     be_data_push(vm, -2);
#     be_return_nil(vm);
# }
def m_push(vm):
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, ".p")
    if not _list_check_data(vm, 2):
        return be_api.be_returnnilvalue(vm)
    be_api.be_pushvalue(vm, 2)
    be_api.be_data_push(vm, -2)
    return be_api.be_returnnilvalue(vm)


# ============================================================================
# m_pop
# ============================================================================

# static int m_pop(bvm *vm)
# {
#     int argc = be_top(vm);
#     be_getmember(vm, 1, ".p");
#     list_check_data(vm, 1);
#     if (argc >= 2) {
#         be_pushvalue(vm, 2);
#     } else {
#         be_pushint(vm, -1);
#     }
#     list_getindex(vm, -2);
#     be_pushvalue(vm, -2);
#     be_data_remove(vm, -4);
#     be_pop(vm, 1);
#     be_return(vm);
# }
def m_pop(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    be_api.be_getmember(vm, 1, ".p")
    if not _list_check_data(vm, 1):
        return be_api.be_returnnilvalue(vm)
    if argc >= 2:
        be_api.be_pushvalue(vm, 2)
    else:
        be_api.be_pushint(vm, -1)
    list_getindex(vm, -2)
    be_api.be_pushvalue(vm, -2)
    be_api.be_data_remove(vm, -4)
    be_api.be_pop(vm, 1)
    return be_api.be_returnvalue(vm)


# ============================================================================
# m_insert
# ============================================================================

# static int m_insert(bvm *vm)
# {
#     be_getmember(vm, 1, ".p");
#     list_check_data(vm, 3);
#     be_pushvalue(vm, 2);
#     be_pushvalue(vm, 3);
#     be_data_insert(vm, -3);
#     be_return_nil(vm);
# }
def m_insert(vm):
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, ".p")
    if not _list_check_data(vm, 3):
        return be_api.be_returnnilvalue(vm)
    be_api.be_pushvalue(vm, 2)
    be_api.be_pushvalue(vm, 3)
    be_api.be_data_insert(vm, -3)
    return be_api.be_returnnilvalue(vm)


# ============================================================================
# m_remove
# ============================================================================

# static int m_remove(bvm *vm)
# {
#     be_getmember(vm, 1, ".p");
#     list_check_data(vm, 2);
#     be_pushvalue(vm, 2);
#     be_data_remove(vm, -2);
#     be_return_nil(vm);
# }
def m_remove(vm):
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, ".p")
    if not _list_check_data(vm, 2):
        return be_api.be_returnnilvalue(vm)
    be_api.be_pushvalue(vm, 2)
    be_api.be_data_remove(vm, -2)
    return be_api.be_returnnilvalue(vm)


# ============================================================================
# item_range — helper for m_item
# ============================================================================

# static int item_range(bvm *vm)
# {
#     bint lower, upper;
#     bint size = be_data_size(vm, -1); /* get source list size */
#     be_getmember(vm, 2, "__lower__");
#     lower = be_toint(vm, -1);
#     if (lower < 0) { lower = size + lower; }
#     be_pop(vm, 1);
#     be_getmember(vm, 2, "__upper__");
#     upper = be_toint(vm, -1);
#     if (upper < 0) { upper = size + upper; }
#     be_pop(vm, 1);
#     upper = upper < size ? upper : size - 1;
#     lower = lower < 0 ? 0 : lower;
#     be_newobject(vm, "list");
#     be_getmember(vm, 1, ".p");
#     for (; lower <= upper; ++lower) {
#         be_pushint(vm, lower);
#         list_getindex(vm, -2);
#         be_data_push(vm, -4);
#         be_pop(vm, 2);
#     }
#     be_pop(vm, 2);
#     be_return(vm);
# }
def item_range(vm):
    be_api = _lazy_be_api()
    size = be_api.be_data_size(vm, -1)
    be_api.be_getmember(vm, 2, "__lower__")
    lower = be_api.be_toint(vm, -1)
    if lower < 0:
        lower = size + lower
    be_api.be_pop(vm, 1)
    be_api.be_getmember(vm, 2, "__upper__")
    upper = be_api.be_toint(vm, -1)
    if upper < 0:
        upper = size + upper
    be_api.be_pop(vm, 1)
    upper = upper if upper < size else size - 1
    lower = 0 if lower < 0 else lower
    be_api.be_newobject(vm, "list")  # result list
    be_api.be_getmember(vm, 1, ".p")  # source list
    while lower <= upper:
        be_api.be_pushint(vm, lower)
        list_getindex(vm, -2)
        be_api.be_data_push(vm, -4)
        be_api.be_pop(vm, 2)
        lower += 1
    be_api.be_pop(vm, 2)
    return be_api.be_returnvalue(vm)


# ============================================================================
# item_list — helper for m_item
# ============================================================================

# static int item_list(bvm *vm)
# {
#     int i, srcsize, idxsize;
#     be_getmember(vm, 2, ".p"); /* get index list */
#     srcsize = be_data_size(vm, -2); /* get source list size */
#     idxsize = be_data_size(vm, -1); /* get index list size */
#     be_newobject(vm, "list"); /* result list */
#     be_getmember(vm, 1, ".p"); /* source list */
#     for (i = 0; i < idxsize; ++i) {
#         be_pushint(vm, i);
#         be_getindex(vm, -5);
#         if (be_isint(vm, -1)) {
#             int idx = be_toindex(vm, -1);
#             if (idx >= 0 && idx < srcsize) {
#                 be_getindex(vm, -3);
#             } else {
#                 be_pushnil(vm);
#             }
#         } else {
#             be_pushnil(vm);
#         }
#         be_data_push(vm, -5);
#         be_pop(vm, 3);
#     }
#     be_pop(vm, 2);
#     be_return(vm);
# }
def item_list(vm):
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 2, ".p")  # get index list
    srcsize = be_api.be_data_size(vm, -2)  # source list size
    idxsize = be_api.be_data_size(vm, -1)  # index list size
    be_api.be_newobject(vm, "list")  # result list
    be_api.be_getmember(vm, 1, ".p")  # source list
    for i in range(idxsize):
        be_api.be_pushint(vm, i)
        be_api.be_getindex(vm, -5)
        if be_api.be_isint(vm, -1):
            idx = be_api.be_toindex(vm, -1)
            if 0 <= idx < srcsize:
                be_api.be_getindex(vm, -3)
            else:
                be_api.be_pushnil(vm)
        else:
            be_api.be_pushnil(vm)
        be_api.be_data_push(vm, -5)
        be_api.be_pop(vm, 3)
    be_api.be_pop(vm, 2)
    return be_api.be_returnvalue(vm)


# ============================================================================
# m_item
# ============================================================================

# static int m_item(bvm *vm)
# {
#     be_getmember(vm, 1, ".p");
#     list_check_data(vm, 2);
#     if (be_isint(vm, 2)) {
#         be_pushvalue(vm, 2);
#         list_getindex(vm, -2);
#         be_return(vm);
#     }
#     if (be_isinstance(vm, 2)) {
#         const char *cname = be_classname(vm, 2);
#         if (!strcmp(cname, "range")) {
#             return item_range(vm);
#         }
#         if (!strcmp(cname, "list")) {
#             return item_list(vm);
#         }
#     }
#     be_raise(vm, "index_error", "list index out of range");
#     be_return_nil(vm);
# }
def m_item(vm):
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, ".p")
    if not _list_check_data(vm, 2):
        return be_api.be_returnnilvalue(vm)
    if be_api.be_isint(vm, 2):
        be_api.be_pushvalue(vm, 2)
        list_getindex(vm, -2)
        return be_api.be_returnvalue(vm)
    if be_api.be_isinstance(vm, 2):
        cname = be_api.be_classname(vm, 2)
        if cname == "range":
            return item_range(vm)
        if cname == "list":
            return item_list(vm)
    be_api.be_raise(vm, "index_error", "list index out of range")
    return be_api.be_returnnilvalue(vm)


# ============================================================================
# m_find
# ============================================================================

# static int m_find(bvm *vm)
# {
#     bbool found = bfalse;
#     int idx;
#     be_getmember(vm, 1, ".p");
#     list_check_data(vm, 2);
#     list_check_ref(vm);
#     be_refpush(vm, 1);
#     be_pushiter(vm, -1);
#     for (idx=0; be_iter_hasnext(vm, -2); idx++) {
#         be_iter_next(vm, -2);
#         be_pushvalue(vm, 2);    /* push needle to compare */
#         if (be_iseq(vm)) {
#             found = btrue;
#             be_pop(vm, 2);
#             break;
#         }
#         be_pop(vm, 2);
#     }
#     be_pop(vm, 1); /* pop iterator */
#     be_refpop(vm);
#     if (found) {
#         be_pushint(vm, idx);
#         be_return(vm);
#     } else {
#         be_return_nil(vm);
#     }
# }
def m_find(vm):
    be_api = _lazy_be_api()
    found = False
    idx = 0
    be_api.be_getmember(vm, 1, ".p")
    if not _list_check_data(vm, 2):
        return be_api.be_returnnilvalue(vm)
    if _list_check_ref(vm):
        return be_api.be_returnvalue(vm)
    be_api.be_refpush(vm, 1)
    be_api.be_pushiter(vm, -1)
    idx = 0
    while be_api.be_iter_hasnext(vm, -2):
        be_api.be_iter_next(vm, -2)
        be_api.be_pushvalue(vm, 2)  # push needle to compare
        if be_api.be_iseq(vm):
            found = True
            be_api.be_pop(vm, 2)
            break
        be_api.be_pop(vm, 2)
        idx += 1
    be_api.be_pop(vm, 1)  # pop iterator
    be_api.be_refpop(vm)
    if found:
        be_api.be_pushint(vm, idx)
        return be_api.be_returnvalue(vm)
    else:
        return be_api.be_returnnilvalue(vm)


# ============================================================================
# m_setitem
# ============================================================================

# static int m_setitem(bvm *vm)
# {
#     be_getmember(vm, 1, ".p");
#     list_check_data(vm, 3);
#     be_pushvalue(vm, 2);
#     be_pushvalue(vm, 3);
#     if (!be_setindex(vm, -3)) {
#         be_raise(vm, "index_error", "list assignment index out of range");
#     }
#     be_return_nil(vm);
# }
def m_setitem(vm):
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, ".p")
    if not _list_check_data(vm, 3):
        return be_api.be_returnnilvalue(vm)
    be_api.be_pushvalue(vm, 2)
    be_api.be_pushvalue(vm, 3)
    if not be_api.be_setindex(vm, -3):
        be_api.be_raise(vm, "index_error", "list assignment index out of range")
    return be_api.be_returnnilvalue(vm)


# ============================================================================
# m_size
# ============================================================================

# static int m_size(bvm *vm)
# {
#     be_getmember(vm, 1, ".p");
#     list_check_data(vm, 1);
#     be_pushint(vm, be_data_size(vm, -1));
#     be_return(vm);
# }
def m_size(vm):
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, ".p")
    if not _list_check_data(vm, 1):
        return be_api.be_returnnilvalue(vm)
    be_api.be_pushint(vm, be_api.be_data_size(vm, -1))
    return be_api.be_returnvalue(vm)


# ============================================================================
# m_tobool
# ============================================================================

# static int m_tobool(bvm *vm)
# {
#     be_getmember(vm, 1, ".p");
#     list_check_data(vm, 1);
#     be_pushbool(vm, be_data_size(vm, -1) > 0);
#     be_return(vm);
# }
def m_tobool(vm):
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, ".p")
    if not _list_check_data(vm, 1):
        return be_api.be_returnnilvalue(vm)
    be_api.be_pushbool(vm, be_api.be_data_size(vm, -1) > 0)
    return be_api.be_returnvalue(vm)


# ============================================================================
# m_resize
# ============================================================================

# static int m_resize(bvm *vm)
# {
#     be_getmember(vm, 1, ".p");
#     list_check_data(vm, 2);
#     be_pushvalue(vm, 2);
#     be_data_resize(vm, -2);
#     be_return_nil(vm);
# }
def m_resize(vm):
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, ".p")
    if not _list_check_data(vm, 2):
        return be_api.be_returnnilvalue(vm)
    be_api.be_pushvalue(vm, 2)
    be_api.be_data_resize(vm, -2)
    return be_api.be_returnnilvalue(vm)


# ============================================================================
# m_clear
# ============================================================================

# static int m_clear(bvm *vm)
# {
#     be_getmember(vm, 1, ".p");
#     list_check_data(vm, 1);
#     be_pushint(vm, 0);
#     be_data_resize(vm, -2);
#     be_return_nil(vm);
# }
def m_clear(vm):
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, ".p")
    if not _list_check_data(vm, 1):
        return be_api.be_returnnilvalue(vm)
    be_api.be_pushint(vm, 0)
    be_api.be_data_resize(vm, -2)
    return be_api.be_returnnilvalue(vm)


# ============================================================================
# iter_closure — native closure for list iteration
# ============================================================================

# static int iter_closure(bvm *vm)
# {
#     /* for better performance, we operate the upvalues
#      * directly without using by the stack. */
#     bntvclos *func = var_toobj(vm->cf->func);
#     bvalue *uv0 = be_ntvclos_upval(func, 0)->value; /* list value */
#     bvalue *uv1 = be_ntvclos_upval(func, 1)->value; /* iter value */
#     bvalue *next = cast(bvalue*, var_toobj(uv1)) + 1;
#     blist *list = var_toobj(uv0);
#     if (next >= be_list_end(list)) {
#         be_stop_iteration(vm);
#     }
#     var_toobj(uv1) = next; /* set upvale[1] (iter value) */
#     /* push next value to top */
#     var_setval(vm->top, next);
#     be_incrtop(vm);
#     be_return(vm);
# }
def iter_closure(vm):
    be_api = _lazy_be_api()
    be_obj = _lazy_be_object()
    be_func = _lazy_be_func()

    func = be_obj.var_toobj(vm.stack[vm.cf.func])
    uv0 = be_func.be_ntvclos_upval(func, 0).value  # list value
    uv1 = be_func.be_ntvclos_upval(func, 1).value  # iter value

    # In C, uv1 holds a pointer into the list data array.
    # In Python, uv1.v is an integer index into list.data.
    # "next = cast(bvalue*, var_toobj(uv1)) + 1" means advance the iterator.
    cur_idx = be_obj.var_toobj(uv1)
    next_idx = cur_idx + 1

    lst = be_obj.var_toobj(uv0)  # blist object
    if next_idx >= lst.count:
        be_api.be_stop_iteration(vm)

    # Update the iterator upvalue to point to next_idx
    uv1.v = next_idx
    uv1.type = be_obj.BE_INDEX

    # Push the value at next_idx to top of stack
    val = lst.data[next_idx]
    be_obj.var_setval(vm.stack[vm.top_idx], val)
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
# m_connect
# ============================================================================

# static int m_connect(bvm *vm)
# {
#     int argc = be_top(vm);
#     if (argc >= 2) {
#         be_getmember(vm, 1, ".p");
#         be_pushvalue(vm, 2);
#         be_data_push(vm, -2);
#         be_pop(vm, argc + 1);
#     }
#     be_return(vm); /* return self */
# }
def m_connect(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    if argc >= 2:
        be_api.be_getmember(vm, 1, ".p")
        be_api.be_pushvalue(vm, 2)
        be_api.be_data_push(vm, -2)
        be_api.be_pop(vm, argc + 1)
    return be_api.be_returnvalue(vm)  # return self


# ============================================================================
# m_merge
# ============================================================================

# static int m_merge(bvm *vm)
# {
#     int argc = be_top(vm);
#     if (argc >= 2) {
#         be_newobject(vm, "list"); /* stack contains instance and .p */
#         be_getmember(vm, 1, ".p");
#         be_data_merge(vm, -2);
#         be_getmember(vm, 2, ".p");
#         if (!be_islist(vm, -1)) {
#             be_raise(vm, "type_error", "operand must be a list");
#         }
#         be_data_merge(vm, -3);
#         be_pop(vm, 3);
#     }
#     be_return(vm); /* return self */
# }
def m_merge(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    if argc >= 2:
        be_api.be_newobject(vm, "list")  # stack: instance, .p
        be_api.be_getmember(vm, 1, ".p")
        be_api.be_data_merge(vm, -2)
        be_api.be_getmember(vm, 2, ".p")
        if not be_api.be_islist(vm, -1):
            be_api.be_raise(vm, "type_error", "operand must be a list")
        be_api.be_data_merge(vm, -3)
        be_api.be_pop(vm, 3)
    return be_api.be_returnvalue(vm)  # return self


# ============================================================================
# connect — helper for list_concat (string concatenation optimization)
# ============================================================================

# static void connect(bvm *vm, bvalue *begin, bvalue *end,
#                     const char * delimiter, bbool first_element)
# {
#     size_t l0 = be_strlen(vm, -1), len = l0;
#     size_t d = delimiter ? strlen(delimiter) : 0;
#     char *buf, *p;
#     bvalue *it;
#     for (it = begin; it < end; ++it) {
#         len += str_len(var_tostr(it)) + d;
#     }
#     if (first_element) {
#         len -= d;
#     }
#     buf = be_pushbuffer(vm, len);
#     memcpy(buf, be_tostring(vm, -2), l0);
#     p = buf + l0;
#     for (it = begin; it < end; ++it) {
#         if ((it != begin || !first_element) && delimiter) {
#             memcpy(p, delimiter, d);
#             p += d;
#         }
#         bstring *s = var_tostr(it);
#         size_t l = str_len(s);
#         memcpy(p, str(s), l);
#         p += l;
#     }
#     be_pushstring(vm, buf);
#     be_moveto(vm, -1, -3);
#     be_pop(vm, 2);
# }
def _connect(vm, begin_idx, end_idx, lst_data, delimiter, first_element):
    """Concatenate a run of string bvalues from lst_data[begin_idx:end_idx]."""
    be_api = _lazy_be_api()
    be_obj = _lazy_be_object()
    be_str = _lazy_be_string()

    # Build the concatenated string in Python
    base = be_api.be_tostring(vm, -1)
    parts = [base]
    first = True
    for i in range(begin_idx, end_idx):
        v = lst_data[i]
        s_obj = be_obj.var_tostr(v)
        s = be_str.be_str2cstr(s_obj)
        if (not first or not first_element) and delimiter is not None:
            parts.append(delimiter)
        parts.append(s)
        first = False
    result = "".join(parts)
    be_api.be_pushstring(vm, result)
    be_api.be_moveto(vm, -1, -3)
    be_api.be_pop(vm, 2)


# ============================================================================
# list_concat — helper for m_concat
# ============================================================================

# static void list_concat(bvm *vm, blist *list, const char * delimiter)
# {
#     bvalue *it, *begin = be_list_data(list);
#     bvalue *end = be_list_end(list);
#     be_pushstring(vm, "");
#     bbool first_element = btrue;
#     for (it = begin; it < end;) {
#         for (; it < end && var_isstr(it); ++it);
#         if (begin < it) {
#             connect(vm, begin, it, delimiter, first_element);
#             first_element = bfalse;
#         }
#         if (it < end) {
#             if (delimiter && !first_element) {
#                 be_pushstring(vm, delimiter);
#                 be_strconcat(vm, -2);
#                 be_pop(vm, 1);
#             }
#             var_setval(vm->top, it);
#             be_incrtop(vm);
#             be_val2str(vm, -1);
#             be_strconcat(vm, -2);
#             be_pop(vm, 1);
#             begin = ++it;
#             first_element = bfalse;
#         }
#     }
# }
def _list_concat(vm, lst, delimiter):
    be_api = _lazy_be_api()
    be_obj = _lazy_be_object()
    be_strlib = _lazy_be_strlib()

    data = lst.data
    count = lst.count
    be_api.be_pushstring(vm, "")  # accumulator string
    first_element = True
    begin = 0
    it = 0
    while it < count:
        # scan forward over consecutive string values
        while it < count and be_obj.var_isstr(data[it]):
            it += 1
        if begin < it:
            _connect(vm, begin, it, data, delimiter, first_element)
            first_element = False
        if it < count:
            if delimiter is not None and not first_element:
                be_api.be_pushstring(vm, delimiter)
                be_api.be_strconcat(vm, -2)
                be_api.be_pop(vm, 1)
            # push the non-string value and convert to string
            be_obj.var_setval(vm.stack[vm.top_idx], data[it])
            be_api.be_incrtop(vm)
            be_strlib.be_val2str(vm, -1)
            be_api.be_strconcat(vm, -2)
            be_api.be_pop(vm, 1)
            it += 1
            begin = it
            first_element = False


# ============================================================================
# m_concat
# ============================================================================

# static int m_concat(bvm *vm)
# {
#     bvalue *value;
#     int top = be_top(vm);
#     be_getmember(vm, 1, ".p");
#     list_check_data(vm, 1);
#     value = be_indexof(vm, -1);
#     const char * delimiter = NULL;
#     if (top >= 2) {
#         delimiter = be_tostring(vm, 2);
#     }
#     list_concat(vm, var_toobj(value), delimiter);
#     be_return(vm);
# }
def m_concat(vm):
    be_api = _lazy_be_api()
    be_obj = _lazy_be_object()
    top = be_api.be_top(vm)
    be_api.be_getmember(vm, 1, ".p")
    if not _list_check_data(vm, 1):
        return be_api.be_returnnilvalue(vm)
    value = vm.stack[be_api.be_indexof(vm, -1)]
    delimiter = None
    if top >= 2:
        delimiter = be_api.be_tostring(vm, 2)
    _list_concat(vm, be_obj.var_toobj(value), delimiter)
    return be_api.be_returnvalue(vm)


# ============================================================================
# m_reverse
# ============================================================================

# static int m_reverse(bvm *vm)
# {
#     int top = be_top(vm);
#     be_getmember(vm, 1, ".p");
#     list_check_data(vm, 1);
#     be_data_reverse(vm, -1);
#     be_pop(vm, top);
#     be_return(vm);
# }
def m_reverse(vm):
    be_api = _lazy_be_api()
    top = be_api.be_top(vm)
    be_api.be_getmember(vm, 1, ".p")
    if not _list_check_data(vm, 1):
        return be_api.be_returnnilvalue(vm)
    be_api.be_data_reverse(vm, -1)
    be_api.be_pop(vm, top)
    return be_api.be_returnvalue(vm)


# ============================================================================
# m_copy
# ============================================================================

# static int m_copy(bvm *vm)
# {
#     be_getmember(vm, 1, ".p");
#     list_check_data(vm, 1);
#     be_getbuiltin(vm, "list");
#     be_copy(vm, -2);
#     be_call(vm, 1);
#     be_pop(vm, 1);
#     be_return(vm);
# }
def m_copy(vm):
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, ".p")
    if not _list_check_data(vm, 1):
        return be_api.be_returnnilvalue(vm)
    be_api.be_getbuiltin(vm, "list")
    be_api.be_copy(vm, -2)
    be_api.be_call(vm, 1)
    be_api.be_pop(vm, 1)
    return be_api.be_returnvalue(vm)


# ============================================================================
# list_equal — helper for m_equal / m_nequal
# ============================================================================

# static int list_equal(bvm *vm, bbool iseq)
# {
#     int i, j, res;
#     bbool (*eqfunc)(bvm*) = iseq ? be_iseq : be_isneq;
#     be_getmember(vm, 1, ".p");
#     be_getmember(vm, 2, ".p");
#     i = be_data_size(vm, -2);
#     j = be_data_size(vm, -1);
#     if (i == j) {
#         res = iseq;
#         for (i = 0; res == iseq && i < j; ++i) {
#             be_pushint(vm, i);
#             be_getindex(vm, -3);
#             be_pushint(vm, i);
#             be_getindex(vm, -4);
#             be_remove(vm, -2);
#             res = eqfunc(vm);
#             be_pop(vm, 3);
#         }
#     } else {
#         res = not iseq;
#     }
#     be_pushbool(vm, res);
#     be_return(vm);
# }
def _list_equal(vm, iseq):
    be_api = _lazy_be_api()
    eqfunc = be_api.be_iseq if iseq else be_api.be_isneq
    be_api.be_getmember(vm, 1, ".p")
    be_api.be_getmember(vm, 2, ".p")
    i = be_api.be_data_size(vm, -2)
    j = be_api.be_data_size(vm, -1)
    if i == j:
        res = iseq
        for idx in range(j):
            if res != iseq:
                break
            be_api.be_pushint(vm, idx)
            be_api.be_getindex(vm, -3)
            be_api.be_pushint(vm, idx)
            be_api.be_getindex(vm, -4)
            be_api.be_remove(vm, -2)
            res = eqfunc(vm)
            be_api.be_pop(vm, 3)
    else:
        res = not iseq
    be_api.be_pushbool(vm, res)
    return be_api.be_returnvalue(vm)


# ============================================================================
# m_keys
# ============================================================================

# static int m_keys(bvm *vm)
# {
#     be_getmember(vm, 1, ".p");
#     list_check_data(vm, 1);
#     int size = be_data_size(vm, -1);
#     be_getbuiltin(vm, "range");
#     be_pushint(vm, 0);
#     be_pushint(vm, size - 1);
#     be_call(vm, 2);
#     be_pop(vm, 2);
#     be_return(vm);
# }
def m_keys(vm):
    be_api = _lazy_be_api()
    be_api.be_getmember(vm, 1, ".p")
    if not _list_check_data(vm, 1):
        return be_api.be_returnnilvalue(vm)
    size = be_api.be_data_size(vm, -1)
    be_api.be_getbuiltin(vm, "range")
    be_api.be_pushint(vm, 0)
    be_api.be_pushint(vm, size - 1)
    be_api.be_call(vm, 2)
    be_api.be_pop(vm, 2)
    return be_api.be_returnvalue(vm)


# ============================================================================
# m_equal / m_nequal
# ============================================================================

# static int m_equal(bvm *vm) { return list_equal(vm, btrue); }
def m_equal(vm):
    return _list_equal(vm, True)

# static int m_nequal(bvm *vm) { return list_equal(vm, bfalse); }
def m_nequal(vm):
    return _list_equal(vm, False)


# ============================================================================
# be_load_listlib — register the list class
# ============================================================================

# void be_load_listlib(bvm *vm)
# {
#     static const bnfuncinfo members[] = {
#         { ".p", NULL },
#         { "init", m_init },
#         { "tostring", m_tostring },
#         { "push", m_push },
#         { "pop", m_pop },
#         { "insert", m_insert },
#         { "remove", m_remove },
#         { "item", m_item },
#         { "find", m_find },
#         { "setitem", m_setitem },
#         { "size", m_size },
#         { "resize", m_resize },
#         { "clear", m_clear },
#         { "iter", m_iter },
#         { "concat", m_concat },
#         { "reverse", m_reverse },
#         { "copy", m_copy },
#         { "keys", m_keys },
#         { "tobool", m_tobool },
#         { "..", m_connect },
#         { "+", m_merge },
#         { "==", m_equal },
#         { "!=", m_nequal },
#         { NULL, NULL }
#     };
#     be_regclass(vm, "list", members);
# }
def be_load_listlib(vm):
    be_api = _lazy_be_api()
    members = [
        (".p", None),
        ("init", m_init),
        ("tostring", m_tostring),
        ("push", m_push),
        ("pop", m_pop),
        ("insert", m_insert),
        ("remove", m_remove),
        ("item", m_item),
        ("find", m_find),
        ("setitem", m_setitem),
        ("size", m_size),
        ("resize", m_resize),
        ("clear", m_clear),
        ("iter", m_iter),
        ("concat", m_concat),
        ("reverse", m_reverse),
        ("copy", m_copy),
        ("keys", m_keys),
        ("tobool", m_tobool),
        ("..", m_connect),
        ("+", m_merge),
        ("==", m_equal),
        ("!=", m_nequal),
    ]
    be_api.be_regclass(vm, "list", members)
