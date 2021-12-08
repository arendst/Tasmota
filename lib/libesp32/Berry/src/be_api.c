/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_vm.h"
#include "be_func.h"
#include "be_class.h"
#include "be_string.h"
#include "be_vector.h"
#include "be_var.h"
#include "be_list.h"
#include "be_map.h"
#include "be_parser.h"
#include "be_debug.h"
#include "be_exec.h"
#include "be_strlib.h"
#include "be_module.h"
#include "be_gc.h"
#include <string.h>

#define retreg(vm)      ((vm)->cf->func)

struct solidfuncinfo {
    const char *name;
    bclosure *function;
};

static void class_init(bvm *vm, bclass *c, const bnfuncinfo *lib)
{
    if (lib) {
        while (lib->name) {
            bstring *s = be_newstr(vm, lib->name);
            if (lib->function) { /* method */
                be_prim_method_bind(vm, c, s, lib->function);
            } else {
                be_member_bind(vm, c, s, btrue); /* member */
            }
            ++lib;
        }
        if (lib->function == (bntvfunc) BE_CLOSURE) {
            /* next section is closures */
            struct solidfuncinfo *slib = (struct solidfuncinfo*)++lib;
            while (slib->name) {
                if (slib->function) { /* method */
                    bstring *s = be_newstr(vm, slib->name);
                    be_closure_method_bind(vm, c, s, slib->function);
                }
                ++slib;
            }
        }
        be_map_compact(vm, c->members); /* clear space */
    }
}

static bclass* class_auto_make(bvm *vm, bstring *name, const bnfuncinfo *lib)
{
    bvalue key, *res;
    var_setobj(&key, BE_COMPTR, (void*)lib);
    if (vm->ntvclass == NULL) {
        vm->ntvclass = be_map_new(vm);
    }
    res = be_map_find(vm, vm->ntvclass, &key);
    if (res == NULL || !var_isclass(res)) {
        bclass *c;
        /* insert class to native class table */
        res = be_map_insert(vm, vm->ntvclass, &key, NULL);
        var_setnil(res); /* must be initialized to ensure correct GC */
        c = be_newclass(vm, name, NULL);
        var_setclass(res, c);
        class_init(vm, c, lib); /* bind members */
        return c;
    }
    return var_toobj(res);
}

BERRY_API void be_regfunc(bvm *vm, const char *name, bntvfunc f)
{
    bvalue *var;
    bstring *s = be_newstr(vm, name);
#if !BE_USE_PRECOMPILED_OBJECT
    int idx = be_builtin_find(vm, s);
    be_assert(idx == -1);
    if (idx == -1) { /* new function */
        idx = be_builtin_new(vm, s);
#else
    int idx = be_global_find(vm, s);
    be_assert(idx < be_builtin_count(vm));
    if (idx < be_builtin_count(vm)) { /* new function */
        idx = be_global_new(vm, s);
#endif
        var = be_global_var(vm, idx);
        var_setntvfunc(var, f);
    } /* error case, do nothing */
}

BERRY_API void be_regclass(bvm *vm, const char *name, const bnfuncinfo *lib)
{
    bvalue *var;
    bstring *s = be_newstr(vm, name);
#if !BE_USE_PRECOMPILED_OBJECT
    int idx = be_builtin_find(vm, s);
    be_assert(idx == -1);
    if (idx == -1) { /* new function */
        idx = be_builtin_new(vm, s);
#else
    int idx = be_global_find(vm, s);
    be_assert(idx < be_builtin_count(vm));
    if (idx < be_builtin_count(vm)) { /* new function */
        idx = be_global_new(vm, s);
#endif
        var = be_global_var(vm, idx);
        var_setclass(var, class_auto_make(vm, s, lib));
    } /* error case, do nothing */
}

BERRY_API int be_top(bvm *vm)
{
    return cast_int(vm->top - vm->reg);
}

BERRY_API void be_pop(bvm *vm, int n)
{
    be_assert(n <= vm->top - vm->reg);
    be_stackpop(vm, n);
}

BERRY_API int be_absindex(bvm *vm, int index)
{
    if (index > 0) {
        return index;
    }
    be_assert(vm->reg <= vm->top + index);
    return cast_int(vm->top + index - vm->reg + 1);
}

BERRY_API bbool be_isnil(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    return var_isnil(v);
}

BERRY_API bbool be_isbool(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    return var_isbool(v);
}

BERRY_API bbool be_isint(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    return var_isint(v);
}

BERRY_API bbool be_isreal(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    return var_isreal(v);
}

BERRY_API bbool be_isnumber(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    return var_isnumber(v);
}

BERRY_API bbool be_isstring(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    return var_isstr(v);
}

BERRY_API bbool be_isclosure(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    return var_isclosure(v);
}

BERRY_API bbool be_isntvclos(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    return var_isntvclos(v);
}

BERRY_API bbool be_isfunction(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    return var_isfunction(v);
}

BERRY_API bbool be_isproto(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    return var_isproto(v);
}

BERRY_API bbool be_isclass(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    return var_isclass(v);
}

BERRY_API bbool be_isinstance(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    return var_isinstance(v);
}

BERRY_API bbool be_ismodule(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    return var_ismodule(v);
}

BERRY_API bbool be_islist(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    return var_islist(v);
}

BERRY_API bbool be_ismap(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    return var_ismap(v);
}

BERRY_API bbool be_iscomptr(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    return var_istype(v, BE_COMPTR);
}

BERRY_API bbool be_iscomobj(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    return var_istype(v, BE_COMOBJ);
}

BERRY_API bint be_toint(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    return var_toint(v);
}

BERRY_API breal be_toreal(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    if (var_isreal(v)) {
        return var_toreal(v);
    }
    if (var_isint(v)) {
        return cast(breal, var_toint(v));
    }
    return cast(breal, 0.0);
}

BERRY_API int be_toindex(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    return var_toidx(v);
}

BERRY_API bbool be_tobool(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    return be_value2bool(vm, v);
}

BERRY_API const char* be_tostring(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    if (!var_isstr(v)) {
        be_val2str(vm, index);
        v = be_indexof(vm, index);
    }
    return str(var_tostr(v));
}

BERRY_API void* be_tocomptr(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    if (var_istype(v, BE_COMPTR)) {
        return var_toobj(v);
    }
    if (var_istype(v, BE_COMOBJ)) {
        bcommomobj *obj = var_toobj(v);
        return obj->data;
    }
    return NULL;
}

BERRY_API void be_moveto(bvm *vm, int from, int to)
{
    bvalue *src = be_indexof(vm, from);
    bvalue *dst = be_indexof(vm, to);
    var_setval(dst, src);
}

BERRY_API void be_pushnil(bvm *vm)
{
    bvalue *reg = be_incrtop(vm);
    var_setnil(reg);
}

BERRY_API void be_pushbool(bvm *vm, int b)
{
    bvalue *reg = be_incrtop(vm);
    var_setbool(reg, b != bfalse);
}

BERRY_API void be_pushint(bvm *vm, bint i)
{
    bvalue *reg = be_incrtop(vm);
    var_setint(reg, i);
}

BERRY_API void be_pushreal(bvm *vm, breal r)
{
    bvalue *reg = be_incrtop(vm);
    var_setreal(reg, r);
}

BERRY_API void be_pushstring(bvm *vm, const char *str)
{
    /* to create a string and then push the top registor,
     * otherwise the GC may crash due to uninitialized values.
     **/
    bstring *s = be_newstr(vm, str);
    bvalue *reg = be_incrtop(vm);
    be_assert(reg < vm->stacktop);
    var_setstr(reg, s);
}

BERRY_API void be_pushnstring(bvm *vm, const char *str, size_t n)
{
    /* to create a string and then push the top registor,
     * otherwise the GC may crash due to uninitialized values.
     **/
    bstring *s = be_newstrn(vm, str, n);
    bvalue *reg = be_incrtop(vm);
    var_setstr(reg, s);
}

BERRY_API const char* be_pushfstring(bvm *vm, const char *format, ...)
{
    const char* s;
    va_list arg_ptr;
    va_start(arg_ptr, format);
    s = be_pushvfstr(vm, format, arg_ptr);
    va_end(arg_ptr);
    return s;
}

BERRY_API void* be_pushbuffer(bvm *vm, size_t size)
{
    bstring *s = be_newlongstr(vm, NULL, size);
    bvalue *reg = be_incrtop(vm);
    var_setstr(reg, s);
    return (void*)str(s);
}

BERRY_API void be_pushvalue(bvm *vm, int index)
{
    bvalue *reg = vm->top;
    var_setval(reg, be_indexof(vm, index));
    be_incrtop(vm);
}

BERRY_API void be_pushclosure(bvm *vm, void *cl)
{
    bvalue *reg = be_incrtop(vm);
    bclosure * closure = (bclosure*) cl;
    var_setclosure(reg, closure);
}

BERRY_API void be_pushntvclosure(bvm *vm, bntvfunc f, int nupvals)
{
    /* to create a native closure and then push the top registor,
     * otherwise the GC may crash due to uninitialized values.
     **/
    bntvclos *cl = be_newntvclosure(vm, f, nupvals);
    bvalue *top = be_incrtop(vm);
    var_setntvclos(top, cl);
}

BERRY_API void be_pushntvfunction(bvm *vm, bntvfunc f)
{
    bvalue *top = be_incrtop(vm);
    var_setntvfunc(top, f);
}

BERRY_API void be_pushclass(bvm *vm, const char *name, const bnfuncinfo *lib)
{
    bclass *c;
    bstring *s = be_newstr(vm, name);
    bvalue *dst = be_incrtop(vm);
    var_setstr(dst, s);
    c = class_auto_make(vm, s, lib);
    var_setclass(vm->top - 1, c);
}

BERRY_API void be_pushntvclass(bvm *vm, const struct bclass * c)
{
    bvalue *top = be_incrtop(vm);
    var_setclass(top, (bclass *) c);
}

BERRY_API void be_pushcomptr(bvm *vm, void *ptr)
{
    bvalue *top = be_incrtop(vm);
    var_setobj(top, BE_COMPTR, ptr);
}

BERRY_API void be_remove(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    bvalue *top = --vm->top;
    while (v < top) {
        *v = v[1];
        ++v;
    }
}

BERRY_API void be_strconcat(bvm *vm, int index)
{
    bstring *s;
    bvalue *dst = be_indexof(vm, index);
    bvalue *src = be_indexof(vm, -1);
    be_assert(var_isstr(src) && var_isstr(dst));
    s = be_strcat(vm, var_tostr(dst), var_tostr(src));
    var_setstr(dst, s);
}

BERRY_API bbool be_setsuper(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    bvalue *top = be_indexof(vm, -1);
    if (var_isclass(v) && var_isclass(top)) {
        bclass *c = var_toobj(v);
        if (!gc_isconst(c)) {
            bclass *super = var_toobj(top);
            be_class_setsuper(c, super);
            return btrue;
        }
    }
    return bfalse;
}

BERRY_API void be_getsuper(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    bvalue *top = be_incrtop(vm);
    if (var_isclass(v)) {
        bclass *c = var_toobj(v);
        c = be_class_super(c);
        if (c) {
            var_setclass(top, c);
            return;
        }
    } else if (var_isinstance(v)) {
        binstance *o = var_toobj(v);
        o = be_instance_super(o);
        if (o) {
            var_setinstance(top, o);
            return;
        }
    }
    var_setnil(top);
}

static bclass* _getclass(bvalue *v)
{ 
    if (var_isinstance(v)) {
        binstance *ins = var_toobj(v);
        return be_instance_class(ins);
    }
    return var_isclass(v) ? var_toobj(v) : NULL;
}

BERRY_API bbool be_isderived(bvm *vm, int index)
{
    bclass *sup = _getclass(be_indexof(vm, -1));
    if (sup) {
        bclass *c = _getclass(be_indexof(vm, index));
        while (c && c != sup)
            c = be_class_super(c);
        return c != NULL;
    }
    return bfalse;
}

BERRY_API const char *be_typename(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    return be_vtype2str(v);
}

BERRY_API const char *be_classname(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    if (var_isclass(v)) {
        bclass *c = var_toobj(v);
        return str(be_class_name(c));
    }
    if (var_isinstance(v)) {
        binstance *i = var_toobj(v);
        return str(be_instance_name(i));
    }
    return NULL;
}

BERRY_API bbool be_classof(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    if (var_isinstance(v)) {
        bvalue *top = be_incrtop(vm);
        binstance *ins = var_toobj(v);
        var_setclass(top, be_instance_class(ins));
        return btrue;
    }
    return bfalse;
}

BERRY_API int be_strlen(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    if (var_isstr(v)) {
        return str_len(var_tostr(v));
    }
    return 0;
}

BERRY_API void be_newlist(bvm *vm)
{
    blist *list = be_list_new(vm);
    bvalue *top = be_incrtop(vm);
    var_setlist(top, list);
}

BERRY_API void be_newmap(bvm *vm)
{
    bmap *map = be_map_new(vm);
    bvalue *top = be_incrtop(vm);
    var_setobj(top, BE_MAP, map);
}

BERRY_API void be_newmodule(bvm *vm)
{
    bmodule *mod = be_module_new(vm);
    bvalue *top = be_incrtop(vm);
    var_setobj(top, BE_MODULE, mod);
}

BERRY_API void be_newobject(bvm *vm, const char *name)
{
    be_getbuiltin(vm, name);
    be_call(vm, 0);
    be_getmember(vm, -1, ".p");
}

BERRY_API bbool be_setname(bvm *vm, int index, const char *name)
{
    bvalue *v = be_indexof(vm, index);
    if (var_ismodule(v)) {
        bmodule *module = var_toobj(v);
        return be_module_setname(module, be_newstr(vm, name));
    }
    return bfalse;
}

BERRY_API bbool be_getglobal(bvm *vm, const char *name)
{
    int idx = be_global_find(vm, be_newstr(vm, name));
    bvalue *top = be_incrtop(vm);
    if (idx > -1) {
        *top = *be_global_var(vm, idx);
        return btrue;
    }
    var_setnil(top);
    return bfalse;
}

BERRY_API void be_setglobal(bvm *vm, const char *name)
{
    int idx;
    bstring *s = be_newstr(vm, name);
    bvalue *v = be_incrtop(vm);
    var_setstr(v, s);
    idx = be_global_new(vm, s);
    v = be_global_var(vm, idx);
    *v = *be_indexof(vm, -2);
    be_stackpop(vm, 1);
}

BERRY_API bbool be_getbuiltin(bvm *vm, const char *name)
{
    int idx = be_builtin_find(vm, be_newstr(vm, name));
    bvalue *top = be_incrtop(vm);
    if (idx > -1) {
        *top = *be_global_var(vm, idx);
        return btrue;
    }
    var_setnil(top);
    return bfalse;
}

BERRY_API bbool be_setmember(bvm *vm, int index, const char *k)
{
    bvalue *o = be_indexof(vm, index);
    bvalue *v = be_indexof(vm, -1);
    if (var_isinstance(o)) {
        bstring *key = be_newstr(vm, k);
        binstance *obj = var_toobj(o);
        return be_instance_setmember(vm, obj, key, v);
    } else if (var_ismodule(o)) {
        bstring *key = be_newstr(vm, k);
        bmodule *mod = var_toobj(o);
        return be_module_setmember(vm, mod, key, v);
    }
    return bfalse;
}

BERRY_API bbool be_copy(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    bvalue *top = be_incrtop(vm);
    if (var_type(v) == BE_LIST) {
        blist *list = be_list_copy(vm, var_toobj(v));
        var_setlist(top, list)
        return btrue;
    }
    var_setnil(top);
    return bfalse;
}

/* `onlyins` limits the search to instance, and discards module. Makes sure getmethod does not return anything for module. */
static int ins_member(bvm *vm, int index, const char *k, bbool onlyins)
{
    int type = BE_NIL;
    bvalue *o = be_indexof(vm, index);
    bvalue *top = be_incrtop(vm);
    var_setnil(top);
    if (var_isinstance(o)) {
        binstance *obj = var_toobj(o);
        type = be_instance_member(vm, obj, be_newstr(vm, k), top);
    } else if (var_isclass(o) && !onlyins) {
        bclass *cl = var_toobj(o);
        type = be_class_member(vm, cl, be_newstr(vm, k), top);
    } else if (var_ismodule(o) && !onlyins) {
        bmodule *module = var_toobj(o);
        type = be_module_attr(vm, module, be_newstr(vm, k), top);
    }
    if (type == BE_NONE) {
        type = BE_NIL;
    }
    return type;
}

BERRY_API bbool be_getmember(bvm *vm, int index, const char *k)
{
    return ins_member(vm, index, k, bfalse) != BE_NIL;
}

BERRY_API bbool be_getmethod(bvm *vm, int index, const char *k)
{
    return basetype(ins_member(vm, index, k, btrue)) == BE_FUNCTION;
}

BERRY_API bbool be_getindex(bvm *vm, int index)
{
    bvalue *o = be_indexof(vm, index);
    bvalue *k = be_indexof(vm, -1);
    bvalue *dst = be_incrtop(vm);
    switch (var_type(o)) {
    case BE_LIST:
        if (var_isint(k)) {
            blist *list = cast(blist*, var_toobj(o));
            int idx = var_toidx(k);
            bvalue *src = be_list_index(list, idx);
            if (src) {
                var_setval(dst, src);
                return btrue;
            }
        }
        break;
    case BE_MAP:
        if (!var_isnil(k)) {
            bmap *map = cast(bmap*, var_toobj(o));
            bvalue *src = be_map_find(vm, map, k);
            if (src) {
                var_setval(dst, src);
                return btrue;
            }
        }
        break;
    default:
        break;
    }
    var_setnil(dst);
    return bfalse;
}

static bvalue* list_setindex(blist *list, bvalue *key)
{
    int idx = var_toidx(key);
    if (idx < be_list_count(list)) {
        return be_list_at(list, idx);
    }
    return NULL;
}

BERRY_API bbool be_setindex(bvm *vm, int index)
{
    bvalue *dst = NULL;
    bvalue *o = be_indexof(vm, index);
    bvalue *k = be_indexof(vm, -2);
    bvalue *v = be_indexof(vm, -1);
    switch (var_type(o)) {
    case BE_LIST:
        if (var_isint(k)) {
            blist *list = var_toobj(o);
            dst = list_setindex(list, k);
        }
        break;
    case BE_MAP:
        if (!var_isnil(k)) {
            bmap *map = var_toobj(o);
            dst = be_map_insert(vm, map, k, NULL);
        }
        break;
    default:
        break;
    }
    if (dst) {
        var_setval(dst, v);
        return btrue;
    }
    return bfalse;
}

BERRY_API void be_getupval(bvm *vm, int index, int pos)
{
    bvalue *f = index ? be_indexof(vm, index) : vm->cf->func;
    bvalue *uv, *top = be_incrtop(vm);
    be_assert(var_istype(f, BE_NTVCLOS));
    if (var_istype(f, BE_NTVCLOS)) {
        bntvclos *nf = var_toobj(f);
        be_assert(pos >= 0 && pos < nf->nupvals);
        uv = be_ntvclos_upval(nf, pos)->value;
        var_setval(top, uv);
    } else {
        var_setnil(top);
    }
}

BERRY_API bbool be_setupval(bvm *vm, int index, int pos)
{
    bvalue *f = index ? be_indexof(vm, index) : vm->cf->func;
    bvalue *uv, *v = be_indexof(vm, -1);
    be_assert(var_istype(f, BE_NTVCLOS));
    if (var_istype(f, BE_NTVCLOS)) {
        bntvclos *nf = var_toobj(f);
        be_assert(pos >= 0 && pos < nf->nupvals);
        uv = be_ntvclos_upval(nf, pos)->value;
        var_setval(uv, v);
        return btrue;
    }
    return bfalse;
}

BERRY_API int be_data_size(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    if (var_islist(v)) {
        blist *list = var_toobj(v);
        return be_list_count(list);
    } else if (var_ismap(v)) {
        bmap *map = cast(bmap*, var_toobj(v));
        return be_map_count(map);
    }
    return -1;
}

BERRY_API void be_data_push(bvm *vm, int index)
{
    bvalue *o = be_indexof(vm, index);
    bvalue *v = be_indexof(vm, -1);
    if (var_islist(o)) {
        blist *list = var_toobj(o);
        be_list_push(vm, list, v);
    }
}

BERRY_API bbool be_data_insert(bvm *vm, int index)
{
    bvalue *o = be_indexof(vm, index);
    bvalue *k = be_indexof(vm, -2);
    bvalue *v = be_indexof(vm, -1);
    switch (var_type(o)) {
    case BE_MAP:
        if (!var_isnil(k)) {
            bmap *map = cast(bmap*, var_toobj(o));
            bvalue *dst = be_map_find(vm, map, k);
            if (dst == NULL) {
                return be_map_insert(vm, map, k, v) != NULL;
            }
        }
        break;
    case BE_LIST:
        if (var_isint(k)) {
            blist *list = cast(blist*, var_toobj(o));
            return be_list_insert(vm, list, var_toidx(k), v) != NULL;
        }
        break;
    default:
        break;
    }
    return bfalse;
}

BERRY_API bbool be_data_remove(bvm *vm, int index)
{
    bvalue *o = be_indexof(vm, index);
    bvalue *k = be_indexof(vm, -1);
    switch (var_type(o)) {
    case BE_MAP:
        if (!var_isnil(k)) {
            bmap *map = cast(bmap*, var_toobj(o));
            return be_map_remove(vm, map, k);
        }
        break;
    case BE_LIST:
        if (var_isint(k)) {
            blist *list = cast(blist*, var_toobj(o));
            return be_list_remove(vm, list, var_toidx(k));
        }
        break;
    default:
        break;
    }
    return bfalse;
}

BERRY_API bbool be_data_merge(bvm *vm, int index)
{
    bvalue *a = be_indexof(vm, index);
    bvalue *b = be_indexof(vm, -1);
    if (var_islist(a) && var_islist(b)) {
        blist *dst = var_toobj(a), *src = var_toobj(b);
        be_list_merge(vm, dst, src);
        return btrue;
    }
    return bfalse;
}

BERRY_API void be_data_resize(bvm *vm, int index)
{
    bvalue *o = be_indexof(vm, index);
    bvalue *v = be_indexof(vm, -1);
    if (var_islist(o)) {
        blist *list = var_toobj(o);
        if (var_isint(v)) {
            be_list_resize(vm, list, var_toidx(v));
        }
    }
}

BERRY_API void be_data_reverse(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    if (var_type(v) == BE_LIST) {
        be_list_reverse(var_toobj(v));
    }
}

BERRY_API bbool be_pushiter(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    if (var_ismap(v)) {
        bvalue *iter = be_incrtop(vm);
        var_setobj(iter, BE_COMPTR, NULL);
        return btrue;
    } else if (var_islist(v)) {
        blist *list = var_toobj(v);
        bvalue *iter = be_incrtop(vm);
        var_setobj(iter, BE_COMPTR, be_list_data(list) - 1);
        return btrue;
    }
    return bfalse;
}

static int list_next(bvm *vm)
{
    bvalue *iter = be_indexof(vm, -1);
    bvalue *next, *dst = be_incrtop(vm);
    next = cast(bvalue*, var_toobj(iter)) + 1;
    var_setobj(iter, BE_COMPTR, next);
    var_setval(dst, next);
    return 1;
}

static bbool list_hasnext(bvm *vm, bvalue *v)
{
    bvalue *next;
    bvalue *iter = be_indexof(vm, -1);
    blist *obj = var_toobj(v);
    next = cast(bvalue*, var_toobj(iter)) + 1;
    return next >= be_list_data(obj) && next < be_list_end(obj);
}

static int map_next(bvm *vm, bvalue *v)
{
    bmapiter iter;
    bmapnode *entry;
    bvalue *dst = vm->top;
    bvalue *itvar = be_indexof(vm, -1);
    iter = var_toobj(itvar);
    entry = be_map_next(var_toobj(v), &iter);
    var_setobj(itvar, BE_COMPTR, iter);
    if (entry) {
        be_map_key2value(dst, entry);
        var_setval(dst + 1, &entry->value);
        vm->top += 2;
        return 2;
    }
    return 0;
}

static bbool map_hasnext(bvm *vm, bvalue *v)
{
    bvalue *node = be_indexof(vm, -1);
    bmapiter iter = var_toobj(node);
    return be_map_next(var_toobj(v), &iter) != NULL;
}

BERRY_API int be_iter_next(bvm *vm, int index)
{
    bvalue *o = be_indexof(vm, index);
    if (var_islist(o)) {
        return list_next(vm);
    } else if (var_ismap(o)) {
        return map_next(vm, o);
    }
    return 0;
}

BERRY_API bbool be_iter_hasnext(bvm *vm, int index)
{
    bvalue *o = be_indexof(vm, index);
    if (var_islist(o)) {
        return list_hasnext(vm, o);
    } else if (var_ismap(o)) {
        return map_hasnext(vm, o);
    }
    return bfalse;
}

BERRY_API bbool be_refcontains(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    binstance **ref = be_stack_base(&vm->refstack);
    binstance **top = be_stack_top(&vm->refstack);
    binstance *ins = var_toobj(v);
    be_assert(var_isinstance(v));
    if (ref) {
        while (ref <= top && *ref != ins) {
            ++ref;
        }
        return ref <= top;
    }
    return bfalse;
}

BERRY_API void be_refpush(bvm *vm, int index)
{
    bvalue *v = be_indexof(vm, index);
    binstance *ins = var_toobj(v);
    be_assert(var_isinstance(v));
    be_stack_push(vm, &vm->refstack, &ins);
}

BERRY_API void be_refpop(bvm *vm)
{
    be_stack_pop(&vm->refstack);
    if (be_stack_isempty(&vm->refstack)) {
        be_vector_release(vm, &vm->refstack);
    }
}

BERRY_API int be_returnvalue(bvm *vm)
{
    bvalue *src = vm->top - 1;
    bvalue *ret = retreg(vm);
    var_setval(ret, src);
    return 0;
}

BERRY_API int be_returnnilvalue(bvm *vm)
{
    bvalue *ret = retreg(vm);
    var_setnil(ret);
    return 0;
}

BERRY_API void be_call(bvm *vm, int argc)
{
    bvalue *fval = vm->top - argc - 1;
    be_dofunc(vm, fval, argc);
}

BERRY_API int be_pcall(bvm *vm, int argc)
{
    bvalue *f = vm->top - argc - 1;
    return be_protectedcall(vm, f, argc);
}

__attribute__((noreturn))
BERRY_API void be_raise(bvm *vm, const char *except, const char *msg)
{
    be_pushstring(vm, except);
    if (msg) {
        be_pushstring(vm, msg);
    } else {
        be_pushnil(vm);
    }
    be_pop(vm, 2);
    be_save_stacktrace(vm);
    be_throw(vm, BE_EXCEPTION);
#ifdef __GNUC__
    __builtin_unreachable();
#endif
}

BERRY_API void be_stop_iteration(bvm *vm)
{
    be_raise(vm, "stop_iteration", NULL);
}

BERRY_API int be_getexcept(bvm *vm, int code)
{
    if (code == BE_EXCEPTION) {
        if (be_isstring(vm, -2)) {
            const char *except = be_tostring(vm, -2);
            if (!strcmp(except, "syntax_error")) {
                return BE_SYNTAX_ERROR;
            }
            if (!strcmp(except, "io_error")) {
                return BE_IO_ERROR;
            }
        }
        return BE_EXEC_ERROR;
    }
    return code;
}

static int _dvfunc(bvm *vm, bbool esc)
{
    const char* s = esc ?
        be_toescape(vm, 1, 'x') : be_tostring(vm, 1);
    be_writestring(s);
    be_return_nil(vm);
}

static int _dumpesc(bvm *vm)
{
    return _dvfunc(vm, btrue);
}

static int _dumpdir(bvm *vm)
{
    return _dvfunc(vm, bfalse);
}

static int dump_value(bvm *vm, int index, bbool esc)
{
    int res, top = be_top(vm) + 1;
    index = be_absindex(vm, index);
    be_pushntvfunction(vm, esc ? _dumpesc : _dumpdir);
    be_pushvalue(vm, index);
    res = be_pcall(vm, 1); /* using index to store result */
    be_remove(vm, top); /* remove '_dumpvalue' function */
    be_remove(vm, top); /* remove the value */
    if (res == BE_EXCEPTION) {
        be_dumpexcept(vm);
    }
    return res;
}

BERRY_API void be_dumpvalue(bvm *vm, int index)
{
    if (dump_value(vm, index, btrue) == BE_OK) {
        be_writenewline();
    }
}

BERRY_API void be_dumpexcept(bvm *vm)
{
    do {
        /* print exception value */
        if (dump_value(vm, -2, bfalse)) break;
        be_writestring(": ");
        /* print exception argument */
        if (dump_value(vm, -1, bfalse)) break;
        be_writenewline();
        /* print stack traceback */
        be_tracestack(vm);
    } while (0);
    be_pop(vm, 2); /* pop the exception value & argument */
}

BERRY_API bbool be_iseq(bvm *vm)
{
    be_assert(vm->reg + 2 <= vm->top);
    return be_vm_iseq(vm, vm->top - 2, vm->top - 1);
}

BERRY_API bbool be_isneq(bvm *vm)
{
    be_assert(vm->reg + 2 <= vm->top);
    return be_vm_isneq(vm, vm->top - 2, vm->top - 1);
}

BERRY_API bbool be_islt(bvm *vm)
{
    be_assert(vm->reg + 2 <= vm->top);
    return be_vm_islt(vm, vm->top - 2, vm->top - 1);
}

BERRY_API bbool be_isle(bvm *vm)
{
    be_assert(vm->reg + 2 <= vm->top);
    return be_vm_isle(vm, vm->top - 2, vm->top - 1);
}

BERRY_API bbool be_isgt(bvm *vm)
{
    be_assert(vm->reg + 2 <= vm->top);
    return be_vm_isgt(vm, vm->top - 2, vm->top - 1);
}

BERRY_API bbool be_isge(bvm *vm)
{
    be_assert(vm->reg + 2 <= vm->top);
    return be_vm_isge(vm, vm->top - 2, vm->top - 1);
}

BERRY_API int be_register(bvm *vm, int index)
{
    bvalue *v;
    if (!vm->registry) {
        vm->registry = be_list_new(vm);
        be_list_pool_init(vm, vm->registry);
    }
    be_assert(vm->registry != NULL);
    v = be_indexof(vm, index);
    return be_list_pool_alloc(vm, vm->registry, v);
}

BERRY_API void be_unregister(bvm *vm, int id)
{
    be_assert(vm->registry != NULL);
    be_list_pool_free(vm->registry, id);
}

BERRY_API void be_getregister(bvm *vm, int id)
{
    blist *reg = vm->registry;
    be_assert(reg && id > 0 && id < be_list_count(reg));
    var_setval(vm->top, be_list_at(reg, id));
    be_incrtop(vm);
}
