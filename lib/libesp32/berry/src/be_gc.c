/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_gc.h"
#include "be_vm.h"
#include "be_mem.h"
#include "be_var.h"
#include "be_vector.h"
#include "be_string.h"
#include "be_class.h"
#include "be_list.h"
#include "be_func.h"
#include "be_map.h"
#include "be_module.h"
#include "be_exec.h"
#include "be_debug.h"

#define GC_PAUSE    (1 << 0) /* GC will not be executed automatically */
#define GC_HALT     (1 << 1) /* GC completely stopped */
#define GC_ALLOC    (1 << 2) /* GC in alloc */

#define gc_try(expr)        be_assert(expr); if (expr)
#define gc_setdark_safe(o)  if (o) gc_setdark(o)

#define next_threshold(gc)  ((gc).usage * ((size_t)(gc).steprate + 100) / 100)

#define link_gray(vm, obj)     {    \
    (obj)->gray = (vm)->gc.gray;    \
    (vm)->gc.gray = gc_object(obj); \
}

static void destruct_object(bvm *vm, bgcobject *obj);
static void free_object(bvm *vm, bgcobject *obj);

void be_gc_init(bvm *vm)
{
    vm->gc.usage = sizeof(bvm);
    be_gc_setsteprate(vm, 200);
}

void be_gc_deleteall(bvm *vm)
{
    bupval *uv, *uvnext;
    bgcobject *node, *next;
    /* halt GC and delete all objects */
    vm->gc.status |= GC_HALT;
    /* first: call destructor */
    for (node = vm->gc.list; node; node = node->next) {
        destruct_object(vm, node);
    }
    /* second: free objects */
    for (node = vm->gc.list; node; node = next) {
        next = node->next;
        free_object(vm, node);
    }
    /* delete open upvalue list */
    for (uv = vm->upvalist; uv; uv = uvnext) {
        uvnext = uv->u.next;
        be_free(vm, uv, sizeof(bupval));
    }
}

void be_gc_setsteprate(bvm *vm, int rate)
{
    be_assert(rate >= 100 && rate <= 355);
    vm->gc.steprate = (bbyte)(rate - 100);
    vm->gc.threshold = next_threshold(vm->gc);
}

void be_gc_setpause(bvm *vm, int pause)
{
    if (pause) {
        vm->gc.status |= GC_PAUSE;
    } else {
        vm->gc.status &= ~GC_PAUSE;
    }
}

bgcobject* be_newgcobj(bvm *vm, int type, size_t size)
{
    bgcobject *obj = be_malloc(vm, size);
    be_gc_auto(vm);
    var_settype(obj, (bbyte)type); /* mark the object type */
    obj->marked = GC_WHITE; /* default gc object type is white */
    obj->next = vm->gc.list; /* link to the next field */
    vm->gc.list = obj; /* insert to head */
    return obj;
}

bgcobject* be_gc_newstr(bvm *vm, size_t size, int islong)
{
    bgcobject *obj;
    if (islong) { /* creating long strings is similar to ordinary GC objects */
        return be_newgcobj(vm, BE_STRING, size);
    }
    obj = be_malloc(vm, size);
    be_gc_auto(vm);
    var_settype(obj, BE_STRING); /* mark the object type to BE_STRING */
    obj->marked = GC_WHITE; /* default string type is white */
    return obj;
}

void be_gc_fix(bvm *vm, bgcobject *obj)
{
    (void)vm;
    if (!gc_isconst(obj)) {
        gc_setfixed(obj);
    }
}

void be_gc_unfix(bvm *vm, bgcobject *obj)
{
    (void)vm;
    if (!gc_isconst(obj)) {
        gc_clearfixed(obj);
    }
}

bbool be_gc_fix_set(bvm *vm, bgcobject *obj, bbool fix)
{
    (void)vm;
    bbool was_fixed = gc_isfixed(obj);
    if (!gc_isconst(obj)) {
        if (fix) {
            gc_setfixed(obj);
        } else {
            gc_clearfixed(obj);
        }
    }
    return was_fixed;
}

static void mark_gray(bvm *vm, bgcobject *obj)
{
    if (obj && gc_iswhite(obj) && !gc_isconst(obj)) {
        gc_setgray(obj);
        switch (var_type(obj)) {
        case BE_STRING: gc_setdark(obj); break; /* just set dark */
        case BE_CLASS: link_gray(vm, cast_class(obj)); break;
        case BE_PROTO: link_gray(vm, cast_proto(obj)); break;
        case BE_INSTANCE: link_gray(vm, cast_instance(obj)); break;
        case BE_MAP: link_gray(vm, cast_map(obj)); break;
        case BE_LIST: link_gray(vm, cast_list(obj)); break;
        case BE_CLOSURE: link_gray(vm, cast_closure(obj)); break;
        case BE_NTVCLOS: link_gray(vm, cast_ntvclos(obj)); break;
        case BE_MODULE: link_gray(vm, cast_module(obj)); break;
        case BE_COMOBJ: gc_setdark(obj); break; /* just set dark */
        default: break;
        }
    }
}

static void mark_gray_var(bvm *vm, bvalue *value)
{
    if (be_isgcobj(value)) {
        mark_gray(vm, var_togc(value));
    }
}

static void mark_map(bvm *vm, bgcobject *obj)
{
    bmap *map = cast_map(obj);
    gc_try (map != NULL) {
        bmapnode *node;
        bmapiter iter = be_map_iter();
        vm->gc.gray = map->gray; /* remove object from gray list */
        while ((node = be_map_next(map, &iter)) != NULL) {
            bmapkey *key = &node->key;
            bvalue *val = &node->value;
            if (be_isgctype((signed char)key->type)) {
                mark_gray(vm, var_togc(key));
            }
            mark_gray_var(vm, val);
        }
    }
}

static void mark_list(bvm *vm, bgcobject *obj)
{
    blist *list = cast_list(obj);
    gc_try (list != NULL) {
        bvalue *val = be_list_data(list);
        bvalue *end = be_list_end(list);
        vm->gc.gray = list->gray; /* remove object from gray list */
        for (; val < end; val++) {
            mark_gray_var(vm, val);
        }
    }
}

static void mark_proto(bvm *vm, bgcobject *obj)
{
    bproto *p = cast_proto(obj);
    gc_try (p != NULL) {
        int count;
        bvalue *k = p->ktab;
        bproto **ptab = p->ptab;
        vm->gc.gray = p->gray; /* remove object from gray list */
        for (count = p->nconst; count--; ++k) {
            mark_gray_var(vm, k);
        }
        for (count = p->nproto; count--; ++ptab) {
            mark_gray(vm, gc_object(*ptab));
        }
        gc_setdark_safe(p->name);
        gc_setdark_safe(p->source);
#if BE_DEBUG_VAR_INFO
        if (p->nvarinfo) {
            bvarinfo *vinfo = p->varinfo;
            be_assert(vinfo != NULL);
            for (count = p->nvarinfo; count--; ++vinfo) {
                gc_setdark_safe(vinfo->name);
            }
        }
#endif
    }
}

static void mark_closure(bvm *vm, bgcobject *obj)
{
    bclosure *cl = cast_closure(obj);
    gc_try (cl != NULL) {
        int count = cl->nupvals;
        bupval **uv = cl->upvals;
        vm->gc.gray = cl->gray; /* remove object from gray list */
        for (; count--; ++uv) {
            if (*uv && (*uv)->refcnt) {
                mark_gray_var(vm, (*uv)->value);
            }
        }
        mark_gray(vm, gc_object(cl->proto));
    }
}

static void mark_ntvclos(bvm *vm, bgcobject *obj)
{
    bntvclos *f = cast_ntvclos(obj);
    gc_try (f != NULL) {
        int count = f->nupvals;
        bupval **uv = &be_ntvclos_upval(f, 0);
        vm->gc.gray = f->gray; /* remove object from gray list */
        for (; count--; ++uv) {
            if (*uv && (*uv)->refcnt) {
                mark_gray_var(vm, (*uv)->value);
            }
        }
    }
}

static void mark_class(bvm *vm, bgcobject *obj)
{
    bclass *c = cast_class(obj);
    gc_try (c != NULL) {
        vm->gc.gray = c->gray; /* remove object from gray list */
        mark_gray(vm, gc_object(be_class_name(c)));
        mark_gray(vm, gc_object(be_class_members(c)));
        mark_gray(vm, gc_object(be_class_super(c)));
    }
}

static void mark_instance(bvm *vm, bgcobject *obj)
{
    binstance *o = cast_instance(obj);
    gc_try (o != NULL) {
        bvalue *var = be_instance_members(o);
        int nvar = be_instance_member_count(o);
        vm->gc.gray = o->gray; /* remove object from gray list */
        mark_gray(vm, gc_object(be_instance_class(o)));
        mark_gray(vm, gc_object(be_instance_super(o)));
        for (; nvar--; var++) { /* mark variables */
            mark_gray_var(vm, var);
        }
    }
}

static void mark_module(bvm *vm, bgcobject *obj)
{
    bmodule *o = cast_module(obj);
    gc_try (o != NULL) {
        vm->gc.gray = o->gray; /* remove object from gray list */
        mark_gray(vm, gc_object(o->table));
        if (!gc_isconst(o) && gc_exmark(o) & BE_MODULE_NAME) {
            mark_gray(vm, gc_object(o->info.sname));
        }
    }
}

static void free_proto(bvm *vm, bgcobject *obj)
{
    bproto *proto = cast_proto(obj);
    gc_try (proto != NULL) {
        be_free(vm, proto->upvals, proto->nupvals * sizeof(bupvaldesc));
        be_free(vm, proto->ktab, proto->nconst * sizeof(bvalue));
        be_free(vm, proto->ptab, proto->nproto * sizeof(bproto*));
        be_free(vm, proto->code, proto->codesize * sizeof(binstruction));
#if BE_DEBUG_RUNTIME_INFO
        be_free(vm, proto->lineinfo, proto->nlineinfo * sizeof(blineinfo));
#endif
#if BE_DEBUG_VAR_INFO
        be_free(vm, proto->varinfo, proto->nvarinfo * sizeof(bvarinfo));
#endif
        be_free(vm, proto, sizeof(bproto));
    }
}

static void free_closure(bvm *vm, bgcobject *obj)
{
    bclosure *cl = cast_closure(obj);
    gc_try (cl != NULL) {
        int count = cl->nupvals;
        be_release_upvalues(vm, cl);
        be_free(vm, cl, sizeof(bclosure)
            + sizeof(bupval*) * ((size_t)count - 1));
    }
}

static void free_ntvclos(bvm *vm, bgcobject *obj)
{
    bntvclos *f = cast_ntvclos(obj);
    gc_try (f != NULL)  {
        int count = f->nupvals;
        bupval **uv = &be_ntvclos_upval(f, 0);
        while (count--) {
            be_free(vm, *uv++, sizeof(bupval));
        }
        be_free(vm, f, sizeof(bntvclos) + sizeof(bupval*) * f->nupvals);
    }
}

static void free_lstring(bvm *vm, bgcobject *obj)
{
    blstring *ls = gc_cast(obj, BE_STRING, blstring);
    gc_try (ls != NULL)  {
        be_free(vm, ls, sizeof(blstring) + ls->llen + 1);
    }
}

static void free_instance(bvm *vm, bgcobject *obj)
{
    binstance *o = cast_instance(obj);
    int nvar = be_instance_member_count(o);
    be_free(vm, obj, sizeof(binstance) + sizeof(bvalue) * (nvar - 1));
}

static void free_object(bvm *vm, bgcobject *obj)
{
    switch (var_type(obj)) {
    case BE_STRING: free_lstring(vm, obj); break; /* long string */
    case BE_CLASS: be_free(vm, obj, sizeof(bclass)); break;
    case BE_INSTANCE: free_instance(vm, obj); break;
    case BE_MAP: be_map_delete(vm, cast_map(obj)); break;
    case BE_LIST: be_list_delete(vm, cast_list(obj)); break;
    case BE_CLOSURE: free_closure(vm, obj); break;
    case BE_NTVCLOS: free_ntvclos(vm, obj); break;
    case BE_PROTO: free_proto(vm, obj); break;
    case BE_MODULE: be_module_delete(vm, cast_module(obj)); break;
    case BE_COMOBJ: be_commonobj_delete(vm, obj); break;
    default: break; /* case BE_STRING: break; */
    }
}

static void premark_internal(bvm *vm)
{
    mark_gray(vm, gc_object(vm->module.loaded));
    mark_gray(vm, gc_object(vm->module.path));
    mark_gray(vm, gc_object(vm->ntvclass));
    mark_gray(vm, gc_object(vm->registry));
#if BE_USE_DEBUG_HOOK
    if (be_isgcobj(&vm->hook)) {
        mark_gray(vm, gc_object(var_toobj(&vm->hook)));
    }
#endif
}

static void premark_global(bvm *vm)
{
    bvalue *v = vm->gbldesc.global.vlist.data;
    bvalue *end = v + be_global_count(vm);
    while (v < end) {
        if (be_isgcobj(v)) {
            mark_gray(vm, var_togc(v));
        }
        ++v;
    }
    v = vm->gbldesc.builtin.vlist.data;
    end = v + be_builtin_count(vm);
    while (v < end) {
        mark_gray_var(vm, v++);
    }
}

static void premark_stack(bvm *vm)
{
    bvalue *v = vm->stack, *end = vm->top;
    /* mark live objects */
    for (; v < end; ++v) {
        mark_gray_var(vm, v);
    }
    /* set other values to nil */
    end = vm->stacktop;
    for (; v < end; ++v) {
        var_setnil(v);
    }
}

static void premark_tracestack(bvm *vm)
{
    bcallsnapshot *cf = be_vector_data(&vm->tracestack);
    bcallsnapshot *end = be_vector_end(&vm->tracestack);
    for (; cf <= end; ++cf) {
        mark_gray_var(vm, &cf->func);
    }
}

static void premark_fixed(bvm *vm)
{
    bgcobject *node = vm->gc.list;
    for (; node; node = node->next) {
        if (gc_isfixed(node) && gc_iswhite(node)) {
            mark_gray(vm, node);
        }
    }
}

static void mark_unscanned(bvm *vm)
{
    while (vm->gc.gray) {
        bgcobject *obj = vm->gc.gray;
        if (obj && !gc_isdark(obj) && !gc_isconst(obj)) {
            gc_setdark(obj);
            switch (var_type(obj)) {
            case BE_CLASS: mark_class(vm, obj); break;
            case BE_PROTO: mark_proto(vm, obj); break;
            case BE_INSTANCE: mark_instance(vm, obj); break;
            case BE_MAP: mark_map(vm, obj); break;
            case BE_LIST: mark_list(vm, obj); break;
            case BE_CLOSURE: mark_closure(vm, obj); break;
            case BE_NTVCLOS: mark_ntvclos(vm, obj); break;
            case BE_MODULE: mark_module(vm, obj); break;
            default:
                be_assert(0); /* error */
                break;
            }
        }
    }
}

static void destruct_object(bvm *vm, bgcobject *obj)
{
    if (vm->gc.status & GC_ALLOC) {
        return; /* no destructor is called during the allocation. */
    }
    if (obj->type == BE_INSTANCE) {
        int type;
        binstance *ins = cast_instance(obj);
        /* does not GC when creating the string "deinit". */
        type = be_instance_member_simple(vm, ins, str_literal(vm, "deinit"), vm->top);
        be_incrtop(vm);
        if (basetype(type) == BE_FUNCTION) {
            var_setinstance(vm->top, ins);  /* push instance on stack as arg 1 */
            be_incrtop(vm);
            be_dofunc(vm, vm->top - 2, 1);  /* warning, there shoudln't be any exception raised here, or the gc stops */
            be_stackpop(vm, 1);
        }
        be_stackpop(vm, 1);
    }
}

static void destruct_white(bvm *vm)
{
    bgcobject *node = vm->gc.list;
    /* since the destructor may allocate objects, we must first suspend the GC */
    vm->gc.status |= GC_HALT; /* mark GC is halt */
    while (node) {
        if (gc_iswhite(node)) {
            destruct_object(vm, node);
        }
        node = node->next;
    }
    vm->gc.status &= ~GC_HALT; /* reset GC halt flag */
}

static void delete_white(bvm *vm)
{
    bgcobject *node, *prev, *next;
    for (node = vm->gc.list, prev = node; node; node = next) {
        next = node->next;
        if (gc_iswhite(node)) {
            if (node == vm->gc.list) { /* first node */
                vm->gc.list = node->next;
                prev = node->next;
            } else { /* not first node */
                prev->next = next;
            }
            free_object(vm, node);
#if BE_USE_PERF_COUNTERS
            vm->counter_gc_freed++;
#endif
        } else {
            gc_setwhite(node);
            prev = node;
        }
    }
}

static void reset_fixedlist(bvm *vm)
{
    bgcobject *node;
    for (node = vm->gc.fixed; node; node = node->next) {
        if (gc_isdark(node)) {
            gc_setwhite(node);
        }
    }
}

void be_gc_auto(bvm *vm)
{
#if BE_USE_DEBUG_GC
    if (vm->gc.status & GC_PAUSE) { /* force gc each time it's possible */
        be_gc_collect(vm);
    }
#else
    if (vm->gc.status & GC_PAUSE && vm->gc.usage > vm->gc.threshold) {
        be_gc_collect(vm);
    }
#endif
}

size_t be_gc_memcount(bvm *vm)
{
    return vm->gc.usage;
}

void be_gc_collect(bvm *vm)
{
    if (vm->gc.status & GC_HALT) {
        return; /* the GC cannot run for some reason */
    }
#if BE_USE_PERF_COUNTERS
    vm->counter_gc_kept = 0;
    vm->counter_gc_freed = 0;
#endif
    if (vm->obshook != NULL) (*vm->obshook)(vm, BE_OBS_GC_START, vm->gc.usage);
    /* step 1: set root-set reference objects to unscanned */
    premark_internal(vm); /* object internal the VM */
    premark_global(vm); /* global objects */
    premark_stack(vm); /* stack objects */
    premark_tracestack(vm); /* trace stack objects */
    premark_fixed(vm); /* fixed objects */
    /* step 2: set unscanned objects to black */
    mark_unscanned(vm);
    /* step 3: destruct and delete unreachable objects */
    destruct_white(vm);
    delete_white(vm);
    be_gcstrtab(vm);
    /* step 4: reset the fixed objects */
    reset_fixedlist(vm);
    /* step 5: calculate the next GC threshold */
    vm->gc.threshold = next_threshold(vm->gc);
#if BE_USE_PERF_COUNTERS
    if (vm->obshook != NULL) (*vm->obshook)(vm, BE_OBS_GC_END, vm->gc.usage, vm->counter_gc_kept, vm->counter_gc_freed);
#else
    if (vm->obshook != NULL) (*vm->obshook)(vm, BE_OBS_GC_END, vm->gc.usage);
#endif
}
