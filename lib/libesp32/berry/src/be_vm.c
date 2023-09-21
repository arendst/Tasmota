/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_vm.h"
#include "be_decoder.h"
#include "be_string.h"
#include "be_strlib.h"
#include "be_class.h"
#include "be_func.h"
#include "be_vector.h"
#include "be_list.h"
#include "be_map.h"
#include "be_module.h"
#include "be_mem.h"
#include "be_var.h"
#include "be_gc.h"
#include "be_exec.h"
#include "be_debug.h"
#include "be_libs.h"
#include <string.h>
#include <math.h>

#define NOT_METHOD          BE_NONE

#define vm_error(vm, except, ...) \
    be_raise(vm, except, be_pushfstring(vm, __VA_ARGS__))

#define RA()   (reg + IGET_RA(ins))  /* Get value of register A */
#define RKB()  ((isKB(ins) ? ktab : reg) + KR2idx(IGET_RKB(ins)))  /* Get value of register or constant B */
#define RKC()  ((isKC(ins) ? ktab : reg) + KR2idx(IGET_RKC(ins)))  /* Get value of register or constant C */

#define var2cl(_v)          cast(bclosure*, var_toobj(_v))  /* cast var to closure */
#define var2real(_v)        (var_isreal(_v) ? (_v)->v.r : (breal)(_v)->v.i)  /* get var as real or convert to real if integer */
#define val2bool(v)         ((v) ? btrue : bfalse)  /* get var as bool (trur if non zero) */
#define ibinop(op, a, b)    ((a)->v.i op (b)->v.i)  /* apply binary operator to both arguments as integers */

#if BE_USE_DEBUG_HOOK
  #define DEBUG_HOOK() \
    if (vm->hookmask & BE_HOOK_LINE) { \
        do_linehook(vm); \
        reg = vm->reg; \
    }
#else
  #define DEBUG_HOOK()
#endif

#if BE_USE_PERF_COUNTERS
  #define COUNTER_HOOK() \
    vm->counter_ins++;
#else
  #define COUNTER_HOOK()
#endif

#if BE_USE_PERF_COUNTERS
  #define VM_HEARTBEAT() \
    if ((vm->counter_ins & ((1<<(BE_VM_OBSERVABILITY_SAMPLING - 1))-1) ) == 0) { /* call every 2^BE_VM_OBSERVABILITY_SAMPLING instructions */    \
        if (vm->obshook != NULL)                                                    \
            (*vm->obshook)(vm, BE_OBS_VM_HEARTBEAT, vm->counter_ins);               \
    }
#else
  #define VM_HEARTBEAT()
#endif

#define vm_exec_loop() \
    loop: \
        DEBUG_HOOK(); \
        COUNTER_HOOK(); \
        VM_HEARTBEAT(); \
        switch (IGET_OP(ins = *vm->ip++))

#if BE_USE_SINGLE_FLOAT
  #define mathfunc(func)    func##f
#else
  #define mathfunc(func)    func
#endif

#define opcase(opcode)      case OP_##opcode
#define dispatch()          goto loop

#define equal_rule(op, iseq) \
    bbool res; \
    be_assert(!var_isstatic(a)); \
    be_assert(!var_isstatic(b)); \
    if (var_isint(a) && var_isint(b)) { \
        res = ibinop(op, a, b); \
    } else if (var_isnumber(a) && var_isnumber(b)) { \
        res = var2real(a) op var2real(b); \
    } else if (var_isinstance(a) && !var_isnil(b)) { \
        res = object_eqop(vm, #op, iseq, a, b); \
    } else if (var_primetype(a) == var_primetype(b)) { /* same types */ \
        if (var_isnil(a)) { /* nil op nil */ \
            res = 1 op 1; \
        } else if (var_isbool(a)) { /* bool op bool */ \
            res = var_tobool(a) op var_tobool(b); \
        } else if (var_isstr(a)) { /* string op string */ \
            res = 1 op be_eqstr(a->v.s, b->v.s); \
        } else if (var_isclass(a) || var_isfunction(a) || var_iscomptr(a)) { \
            res = var_toobj(a) op var_toobj(b); \
        } else { \
            binop_error(vm, #op, a, b); \
            res = bfalse; /* will not be executed */ \
        } \
    } else { /* different types */ \
        res = 1 op 0; \
    } \
    return res

/* when running on ESP32 in IRAM, there is a bug in early chip revision */
#ifdef ESP32
    #define relop_rule(op) \
        bbool res; \
        if (var_isint(a) && var_isint(b)) { \
            res = ibinop(op, a, b); \
        } else if (var_isnumber(a) && var_isnumber(b)) { \
            /* res = var2real(a) op var2real(b); */ \
            union bvaldata x, y;        /* TASMOTA workaround for ESP32 rev0 bug */ \
            x.i = a->v.i;\
            if (var_isint(a)) { x.r = (breal) x.i; }\
            y.i = b->v.i;\
            if (var_isint(b)) { y.r = (breal) y.i; }\
            res = x.r op y.r; \
        } else if (var_isstr(a) && var_isstr(b)) { \
            bstring *s1 = var_tostr(a), *s2 = var_tostr(b); \
            res = be_strcmp(s1, s2) op 0; \
        } else if (var_isinstance(a)) { \
            binstance *obj = var_toobj(a); \
            object_binop(vm, #op, *a, *b); \
            check_bool(vm, obj, #op); \
            res = var_tobool(vm->top); \
        } else { \
            binop_error(vm, #op, a, b); \
            res = bfalse; /* will not be executed */ \
        } \
        return res
#else  // ESP32
    #define relop_rule(op) \
        bbool res; \
        if (var_isint(a) && var_isint(b)) { \
            res = ibinop(op, a, b); \
        } else if (var_isnumber(a) && var_isnumber(b)) { \
            res = var2real(a) op var2real(b); \
        } else if (var_isstr(a) && var_isstr(b)) { \
            bstring *s1 = var_tostr(a), *s2 = var_tostr(b); \
            res = be_strcmp(s1, s2) op 0; \
        } else if (var_isinstance(a)) { \
            binstance *obj = var_toobj(a); \
            object_binop(vm, #op, *a, *b); \
            check_bool(vm, obj, #op); \
            res = var_tobool(vm->top); \
        } else { \
            binop_error(vm, #op, a, b); \
            res = bfalse; /* will not be executed */ \
        } \
        return res
#endif // ESP32

#define bitwise_block(op) \
    bvalue *dst = RA(), *a = RKB(), *b = RKC(); \
    if (var_isint(a) && var_isint(b)) { \
        var_setint(dst, ibinop(op, a, b)); \
    } else if (var_isinstance(a)) { \
        ins_binop(vm, #op, ins); \
    } else { \
        binop_error(vm, #op, a, b); \
    }

#define push_native(_vm, _f, _ns, _t) { \
    precall(_vm, _f, _ns, _t); \
    _vm->cf->status = PRIM_FUNC; \
}

static void prep_closure(bvm *vm, int pos, int argc, int mode);

static void attribute_error(bvm *vm, const char *t, bvalue *b, bvalue *c)
{
    const char *attr = var_isstr(c) ? str(var_tostr(c)) : be_vtype2str(c);
    vm_error(vm, "attribute_error",
        "'%s' value has no %s '%s'", be_vtype2str(b), t, attr);
}

static void binop_error(bvm *vm, const char *op, bvalue *a, bvalue *b)
{
    vm_error(vm, "type_error",
        "unsupported operand type(s) for %s: '%s' and '%s'",
        op, be_vtype2str(a), be_vtype2str(b));
}

static void unop_error(bvm *vm, const char *op, bvalue *a)
{
    vm_error(vm, "type_error",
        "unsupported operand type(s) for %s: '%s'",
        op, be_vtype2str(a));
}

static void call_error(bvm *vm, bvalue *v)
{
    vm_error(vm, "type_error",
        "'%s' value is not callable", be_vtype2str(v));
}

/* Check that the return value is bool or raise an exception */
/* `obj` and `method` are only passed for error reporting */
static void check_bool(bvm *vm, binstance *obj, const char *method)
{
    if (!var_isbool(vm->top)) {
        const char *name = str(be_instance_name(obj));
        vm_error(vm, "type_error",
            "`%s::%s` return value error, the expected type is 'bool'",
            strlen(name) ? name : "<anonymous>", method);
    }
}

#if BE_USE_DEBUG_HOOK
static void do_linehook(bvm *vm)
{
    bcallframe *cf = vm->cf;
    bclosure *cl = var_toobj(cf->func);
    int pc = cast_int(vm->ip - cl->proto->code);
    if (!pc || pc > cf->lineinfo->endpc) {
        while (pc > cf->lineinfo->endpc)
            cf->lineinfo++;
        be_callhook(vm, BE_HOOK_LINE);
    } else {
        blineinfo *linfo = cf->lineinfo;
        blineinfo *base = cl->proto->lineinfo;
        while (linfo > base && pc <= linfo[-1].endpc)
            linfo--;
        if (cf->lineinfo != linfo) {
            cf->lineinfo = linfo;
            be_callhook(vm, BE_HOOK_LINE);
        }
    }
}
#endif

/* Prepare the stack for the function/method call */
/* `func` is a pointer to the function/method on the stack, it contains the closure before call and the result after the call */
/* `nstackˋ is the stack depth used by the function (determined by compiler), we add BE_STACK_FREE_MIN as a safety margin */
static void precall(bvm *vm, bvalue *func, int nstack, int mode)
{
    bcallframe *cf;
    int expan = nstack + BE_STACK_FREE_MIN; /* `expan` is the minimum required space on the stack */
    if (vm->stacktop < func + expan) {  /* do we have too little space left on the stack? */
        size_t fpos = func - vm->stack;  /* compute offset of `func` from base stack, in case stack is reallocated and base address changes */
        be_stack_expansion(vm, expan);  /* expand stack (vector object), warning stack address changes */
        func = vm->stack + fpos;  /* recompute `func` address with new stack address */
    }
    be_stack_push(vm, &vm->callstack, NULL);  /* push a NULL value on callstack */
    cf = be_stack_top(&vm->callstack);  /* get address of new callframe at top of callstack */
    cf->func = func - mode;
    cf->top = vm->top;  /* save previous stack top */
    cf->reg = vm->reg;  /* save previous stack base */
    vm->reg = func + 1;  /* new stack base is right after function */
    vm->top = vm->reg + nstack; /* new stack top is above the registers used by the function, so we don´t mess with them */
    vm->cf = cf;  /* set new current callframe */
}

/* Prepare call of closure, setting the instruction pointer (ip) */
static void push_closure(bvm *vm, bvalue *func, int nstack, int mode)
{
    bclosure *cl = var_toobj(func);
    precall(vm, func, nstack, mode);
    vm->cf->ip = vm->ip;
    vm->cf->status = NONE_FLAG;
    vm->ip = cl->proto->code;
#if BE_USE_DEBUG_HOOK
    vm->cf->lineinfo = cl->proto->lineinfo;
    be_callhook(vm, BE_HOOK_CALL);
#endif
}

static void ret_native(bvm *vm)
{
    bcallframe *_cf = vm->cf;
    vm->reg = _cf->reg;
    vm->top = _cf->top;
    be_stack_pop(&vm->callstack);
    vm->cf = be_stack_top(&vm->callstack);
}

static bbool obj2bool(bvm *vm, bvalue *var)
{
    binstance *obj = var_toobj(var);
    bstring *tobool = str_literal(vm, "tobool");
    /* get operator method */
    int type = be_instance_member(vm, obj, tobool, vm->top);
    if (type != BE_NONE && type != BE_NIL) {
        vm->top[1] = *var; /* move self to argv[0] */
        be_dofunc(vm, vm->top, 1); /* call method 'tobool' */
        /* check the return value */
        check_bool(vm, obj, "tobool");
        return var_tobool(vm->top);
    }
    return btrue;
}

bbool be_value2bool(bvm *vm, bvalue *v)
{
    switch (var_basetype(v)) {
    case BE_NIL:
        return bfalse;
    case BE_BOOL:
        return var_tobool(v);
    case BE_INT:
        return val2bool(v->v.i);
    case BE_REAL:
        return val2bool(v->v.r);
    case BE_STRING:
        return str_len(var_tostr(v)) != 0;
    case BE_COMPTR:
        return var_toobj(v) != NULL;
    case BE_COMOBJ:
        return ((bcommomobj*)var_toobj(v))->data != NULL;
    case BE_INSTANCE:
        return obj2bool(vm, v);
    default:
        return btrue;
    }
}

static void obj_method(bvm *vm, bvalue *o, bstring *attr, bvalue *dst)
{
    binstance *obj = var_toobj(o);
    int type = be_instance_member_simple(vm, obj, attr, dst);
    if (basetype(type) != BE_FUNCTION) {
        vm_error(vm, "attribute_error",
            "the '%s' object has no method '%s'",
            str(be_instance_name(obj)), str(attr));
    }
}

static int obj_attribute(bvm *vm, bvalue *o, bstring *attr, bvalue *dst)
{
    binstance *obj = var_toobj(o);
    int type = be_instance_member(vm, obj, attr, dst);
    if (type == BE_NONE) {
        vm_error(vm, "attribute_error",
            "the '%s' object has no attribute '%s'",
            str(be_instance_name(obj)), str(attr));
    }
    return type;
}

static int class_attribute(bvm *vm, bvalue *o, bvalue *c, bvalue *dst)
{
    bstring *attr = var_tostr(c);
    bclass *obj = var_toobj(o);
    int type = be_class_member(vm, obj, attr, dst);
    if (type == BE_NONE || type == BE_INDEX) {
        vm_error(vm, "attribute_error",
            "the '%s' class has no static attribute '%s'",
            str(obj->name), str(attr));
    }
    return type;
}

static int module_attribute(bvm *vm, bvalue *o, bvalue *c, bvalue *dst)
{
    bstring *attr = var_tostr(c);
    bmodule *module = var_toobj(o);
    int type = be_module_attr(vm, module, attr, dst);
    if (type == BE_NONE) {
        vm_error(vm, "attribute_error",
            "module '%s' has no member '%s'",
            be_module_name(module), str(attr));
    }
    return type;
}

static bbool object_eqop(bvm *vm,
    const char *op, bbool iseq, bvalue *a, bvalue *b)
{
    binstance *o = var_toobj(a);
    bvalue self = *a, other = *b;
    bbool isself = var_isinstance(b) && o == var_toobj(b);
    /* first, try to call the overloaded operator of the object */
    int type = be_instance_member(vm, o, be_newstr(vm, op), vm->top);
    if (basetype(type) == BE_FUNCTION) { /* call method */
        bvalue *top = vm->top;
        top[1] = self;  /* move self to argv[0] */
        top[2] = other; /* move other to argv[1] */
        be_incrtop(vm); /* prevent collection results */
        be_dofunc(vm, top, 2); /* call method 'item' */
        be_stackpop(vm, 1);
        check_bool(vm, o, op); /* check return value */
        return var_tobool(vm->top); /* copy result to dst */
    }
    /* the default equal operation rule */
    return iseq == isself; /* check object self */
}

static void object_binop(bvm *vm, const char *op, bvalue self, bvalue other)
{
    bvalue *top = vm->top;
    /* get operator method (possible GC) */
    obj_method(vm, &self, be_newstr(vm, op), vm->top);
    top[1] = self; /* move self to argv[0] */
    top[2] = other; /* move other to argv[1] */
    be_incrtop(vm); /* prevent collection results */
    be_dofunc(vm, top, 2); /* call method 'item' */
    be_stackpop(vm, 1);
}

#define ins_binop(vm, op, ins) { \
    object_binop(vm, op, *RKB(), *RKC()); \
    reg = vm->reg; \
    *RA() = *vm->top; /* copy result to dst */ \
}

static void ins_unop(bvm *vm, const char *op, bvalue self)
{
    bvalue *top = vm->top;
    /* get operator method (possible GC) */
    obj_method(vm, &self, be_newstr(vm, op), vm->top);
    top[1] = self; /* move self to argv[0] */
    be_dofunc(vm, top, 1); /* call method 'item' */
}

bbool be_vm_iseq(bvm *vm, bvalue *a, bvalue *b)
{
    equal_rule(==, btrue);
}

bbool be_vm_isneq(bvm *vm, bvalue *a, bvalue *b)
{
    equal_rule(!=, bfalse);
}

bbool be_vm_islt(bvm *vm, bvalue *a, bvalue *b)
{
    relop_rule(<);
}

bbool be_vm_isle(bvm *vm, bvalue *a, bvalue *b)
{
    relop_rule(<=);
}

bbool be_vm_isgt(bvm *vm, bvalue *a, bvalue *b)
{
    relop_rule(>);
}

bbool be_vm_isge(bvm *vm, bvalue *a, bvalue *b)
{
    relop_rule(>=);
}

static void make_range(bvm *vm, bvalue lower, bvalue upper)
{
    /* get method 'item' (possible GC) */
    int idx = be_builtin_find(vm, str_literal(vm, "range"));
    bvalue *top = vm->top;
    top[0] = *be_global_var(vm, idx);
    top[1] = lower; /* move lower to argv[0] */
    top[2] = upper; /* move upper to argv[1] */
    vm->top += 3; /* prevent collection results */
    be_dofunc(vm, top, 2); /* call method 'item' */
    vm->top -= 3;
}

static void connect_str(bvm *vm, bstring *a, bvalue *b)
{
    bstring *s;
    if (var_isstr(b)) {
        s = be_strcat(vm, a, var_tostr(b));
        var_setstr(vm->top, s);
    } else {
        *vm->top++ = *b;
        be_val2str(vm, -1);
        b = vm->top - 1;
        s = be_strcat(vm, a, var_tostr(b));
        var_setstr(b, s);
        vm->top -= 1;
    }
}

BERRY_API bvm* be_vm_new(void)
{
    bvm *vm = be_os_malloc(sizeof(bvm));
    be_assert(vm != NULL);
    memset(vm, 0, sizeof(bvm)); /* clear all members */
    be_gc_init(vm);
    be_string_init(vm);
    be_stack_init(vm, &vm->callstack, sizeof(bcallframe));
    be_stack_init(vm, &vm->refstack, sizeof(binstance*));
    be_stack_init(vm, &vm->exceptstack, sizeof(struct bexecptframe));
    be_stack_init(vm, &vm->tracestack, sizeof(bcallsnapshot));
    vm->stack = be_malloc(vm, sizeof(bvalue) * BE_STACK_START);
    vm->stacktop = vm->stack + BE_STACK_START;
    vm->reg = vm->stack;
    vm->top = vm->reg;
    be_globalvar_init(vm);
    be_gc_setpause(vm, 1);
    be_loadlibs(vm);
    vm->compopt = 0;
    vm->bytesmaxsize = BE_BYTES_MAX_SIZE;
    vm->obshook = NULL;
    vm->ctypefunc = NULL;
#if BE_USE_PERF_COUNTERS
    vm->counter_ins = 0;
    vm->counter_enter = 0;
    vm->counter_call = 0;
    vm->counter_get = 0;
    vm->counter_set = 0;
    vm->counter_get_global = 0;
    vm->counter_try = 0;
    vm->counter_exc = 0;
    vm->counter_gc_kept = 0;
    vm->counter_gc_freed = 0;
    vm->counter_mem_alloc = 0;
    vm->counter_mem_free = 0;
    vm->counter_mem_realloc = 0;
#endif
    return vm;
}

BERRY_API void be_vm_delete(bvm *vm)
{
    be_gc_deleteall(vm);
    be_string_deleteall(vm);
    be_stack_delete(vm, &vm->callstack);
    be_stack_delete(vm, &vm->refstack);
    be_stack_delete(vm, &vm->exceptstack);
    be_stack_delete(vm, &vm->tracestack);
    be_free(vm, vm->stack, (vm->stacktop - vm->stack) * sizeof(bvalue));
    be_globalvar_deinit(vm);
    be_gc_free_memory_pools(vm);
#if BE_USE_DEBUG_HOOK
    /* free native hook */
    if (var_istype(&vm->hook, BE_COMPTR))
        be_free(vm, var_toobj(&vm->hook), sizeof(struct bhookblock));
#endif
    /* free VM structure */
    be_os_free(vm);
}

static void vm_exec(bvm *vm)
{
    bclosure *clos;
    bvalue *ktab, *reg;
    binstruction ins;
    vm->cf->status |= BASE_FRAME;
newframe: /* a new call frame */
    be_assert(var_isclosure(vm->cf->func));
    clos = var_toobj(vm->cf->func);  /* `clos` is the current function/closure */
    ktab = clos->proto->ktab;  /* `ktab` is the current constant table */
    reg = vm->reg;  /* `reg` is the current stack base for the callframe */
#if BE_USE_PERF_COUNTERS
    vm->counter_enter++;
#endif
    vm_exec_loop() {
        opcase(LDNIL): {
            var_setnil(RA());
            dispatch();
        }
        opcase(LDBOOL): {
            bvalue *v = RA();
            var_setbool(v, IGET_RKB(ins));
            if (IGET_RKC(ins)) { /* skip next instruction */
                vm->ip += 1;
            }
            dispatch();
        }
        opcase(LDINT): {
            bvalue *v = RA();
            var_setint(v, IGET_sBx(ins));
            dispatch();
        }
        opcase(LDCONST): {
            bvalue *dst = RA();
            *dst = ktab[IGET_Bx(ins)];
            dispatch();
        }
        opcase(GETGBL): {
            bvalue *v = RA();
            int idx = IGET_Bx(ins);
            *v = *be_global_var(vm, idx);
            dispatch();
        }
        opcase(GETNGBL): {  /* get Global by name */
#if BE_USE_PERF_COUNTERS
            vm->counter_get_global++;
#endif
            bvalue *v = RA();
            bvalue *b = RKB();
            if (var_isstr(b)) {
                bstring *name = var_tostr(b);
                int idx = be_global_find(vm, name);
                if (idx > -1) {
                    *v = *be_global_var(vm, idx);
                } else {
                    vm_error(vm, "attribute_error", "'%s' undeclared", str(name));
                }
            } else {
                vm_error(vm, "internal_error", "global name must be a string");
            }
            dispatch();
        }
        opcase(SETNGBL): {  /* set Global by name */
            bvalue *v = RA();
            bvalue *b = RKB();
            if (var_isstr(b)) {
                bstring *name = var_tostr(b);
                int idx = be_global_new(vm, name);
                *be_global_var(vm, idx) = *v;
            } else {
                vm_error(vm, "internal_error", "global name must be a string");
            }
            dispatch();
        }
        opcase(SETGBL): {
            bvalue *v = RA();
            int idx = IGET_Bx(ins);
            *be_global_var(vm, idx) = *v;
            dispatch();
        }
        opcase(GETUPV): {
            bvalue *v = RA();
            int idx = IGET_Bx(ins);
            be_assert(*clos->upvals != NULL);
            *v = *clos->upvals[idx]->value;
            dispatch();
        }
        opcase(SETUPV): {
            bvalue *v = RA();
            int idx = IGET_Bx(ins);
            be_assert(*clos->upvals != NULL);
            *clos->upvals[idx]->value = *v;
            dispatch();
        }
        opcase(MOVE): {
            bvalue *dst = RA();
            *dst = *RKB();
            dispatch();
        }
        opcase(ADD): {
            bvalue *dst = RA(), *a = RKB(), *b = RKC();
            if (var_isint(a) && var_isint(b)) {
                var_setint(dst, ibinop(+, a, b));
            } else if (var_isnumber(a) && var_isnumber(b)) {
#ifdef ESP32    /* when running on ESP32 in IRAM, there is a bug in early chip revision */
                union bvaldata x, y;        // TASMOTA workaround for ESP32 rev0 bug
                x.i = a->v.i;
                if (var_isint(a)) { x.r = (breal) x.i; }
                y.i = b->v.i;
                if (var_isint(b)) { y.r = (breal) y.i; }
                var_setreal(dst, x.r + y.r);
#else  // ESP32
                breal x = var2real(a), y = var2real(b);
                var_setreal(dst, x + y);
#endif // ESP32
            } else if (var_isstr(a) && var_isstr(b)) { /* strcat */
                bstring *s = be_strcat(vm, var_tostr(a), var_tostr(b));
                reg = vm->reg;
                dst = RA();
                var_setstr(dst, s);
            } else if (var_isinstance(a)) {
                ins_binop(vm, "+", ins);
            } else {
                binop_error(vm, "+", a, b);
            }
            dispatch();
        }
        opcase(SUB): {
            bvalue *dst = RA(), *a = RKB(), *b = RKC();
            if (var_isint(a) && var_isint(b)) {
                var_setint(dst, ibinop(-, a, b));
            } else if (var_isnumber(a) && var_isnumber(b)) {
                breal x = var2real(a), y = var2real(b);
                var_setreal(dst, x - y);
            } else if (var_isinstance(a)) {
                ins_binop(vm, "-", ins);
            } else {
                binop_error(vm, "-", a, b);
            }
            dispatch();
        }
        opcase(MUL): {
            bvalue *dst = RA(), *a = RKB(), *b = RKC();
            if (var_isint(a) && var_isint(b)) {
                var_setint(dst, ibinop(*, a, b));
            } else if (var_isnumber(a) && var_isnumber(b)) {
                breal x = var2real(a), y = var2real(b);
                var_setreal(dst, x * y);
            } else if (var_isinstance(a)) {
                ins_binop(vm, "*", ins);
            } else {
                binop_error(vm, "*", a, b);
            }
            dispatch();
        }
        opcase(DIV): {
            bvalue *dst = RA(), *a = RKB(), *b = RKC();
            if (var_isint(a) && var_isint(b)) {
                bint x = var_toint(a), y = var_toint(b);
                if (y == 0) {
                    vm_error(vm, "divzero_error", "division by zero");
                } else {
                    var_setint(dst, x / y);
                }
            } else if (var_isnumber(a) && var_isnumber(b)) {
                breal x = var2real(a), y = var2real(b);
                if (y == cast(breal, 0)) {
                    vm_error(vm, "divzero_error", "division by zero");
                }
                var_setreal(dst, x / y);
            } else if (var_isinstance(a)) {
                ins_binop(vm, "/", ins);
            } else {
                binop_error(vm, "/", a, b);
            }
            dispatch();
        }
        opcase(MOD): {
            bvalue *dst = RA(), *a = RKB(), *b = RKC();
            if (var_isint(a) && var_isint(b)) {
                var_setint(dst, ibinop(%, a, b));
            } else if (var_isnumber(a) && var_isnumber(b)) {
                var_setreal(dst, mathfunc(fmod)(var_toreal(a), var_toreal(b)));
            } else if (var_isinstance(a)) {
                ins_binop(vm, "%", ins);
            } else {
                binop_error(vm, "%", a, b);
            }
            dispatch();
        }
        opcase(LT): {
            bbool res = be_vm_islt(vm, RKB(), RKC());
            bvalue *dst;
            reg = vm->reg;
            dst = RA();
            var_setbool(dst, res);
            dispatch();
        }
        opcase(LE): {
            bbool res = be_vm_isle(vm, RKB(), RKC());
            bvalue *dst;
            reg = vm->reg;
            dst = RA();
            var_setbool(dst, res);
            dispatch();
        }
        opcase(EQ): {
            bbool res = be_vm_iseq(vm, RKB(), RKC());
            bvalue *dst;
            reg = vm->reg;
            dst = RA();
            var_setbool(dst, res);
            dispatch();
        }
        opcase(NE): {
            bbool res = be_vm_isneq(vm, RKB(), RKC());
            bvalue *dst;
            reg = vm->reg;
            dst = RA();
            var_setbool(dst, res);
            dispatch();
        }
        opcase(GT): {
            bbool res = be_vm_isgt(vm, RKB(), RKC());
            bvalue *dst;
            reg = vm->reg;
            dst = RA();
            var_setbool(dst, res);
            dispatch();
        }
        opcase(GE): {
            bbool res = be_vm_isge(vm, RKB(), RKC());
            bvalue *dst;
            reg = vm->reg;
            dst = RA();
            var_setbool(dst, res);
            dispatch();
        }
        opcase(CONNECT): {
            bvalue *a = RKB(), *b = RKC();
            if (var_isint(a) && var_isint(b)) {
                make_range(vm, *RKB(), *RKC());
            } else if (var_isstr(a)) {
                connect_str(vm, var_tostr(a), b);
            } else if (var_isinstance(a)) {
                object_binop(vm, "..", *RKB(), *RKC());
            } else {
                binop_error(vm, "..", RKB(), RKC());
            }
            reg = vm->reg;
            *RA() = *vm->top; /* copy result to R(A) */
            dispatch();
        }
        opcase(AND): {
            bitwise_block(&);
            dispatch();
        }
        opcase(OR): {
            bitwise_block(|);
            dispatch();
        }
        opcase(XOR): {
            bitwise_block(^);
            dispatch();
        }
        opcase(SHL): {
            bitwise_block(<<);
            dispatch();
        }
        opcase(SHR): {
            bitwise_block(>>);
            dispatch();
        }
        opcase(NEG): {
            bvalue *dst = RA(), *a = RKB();
            if (var_isint(a)) {
                var_setint(dst, -a->v.i);
            } else if (var_isreal(a)) {
                var_setreal(dst, -a->v.r);
            } else if (var_isinstance(a)) {
                ins_unop(vm, "-*", *RKB());
                reg = vm->reg;
                *RA() = *vm->top; /* copy result to dst */
            } else {
                unop_error(vm, "-", a);
            }
            dispatch();
        }
        opcase(FLIP): {
            bvalue *dst = RA(), *a = RKB();
            if (var_isint(a)) {
                var_setint(dst, ~a->v.i);
            } else if (var_isinstance(a)) {
                ins_unop(vm, "~", *RKB());
                reg = vm->reg;
                *RA() = *vm->top; /* copy result to dst */
            } else {
                unop_error(vm, "~", a);
            }
            dispatch();
        }
        opcase(JMP): {
            vm->ip += IGET_sBx(ins);
            dispatch();
        }
        opcase(JMPT): {
            if (be_value2bool(vm, RA())) {
                vm->ip += IGET_sBx(ins);
            }
            dispatch();
        }
        opcase(JMPF): {
            if (!be_value2bool(vm, RA())) {
                vm->ip += IGET_sBx(ins);
            }
            dispatch();
        }
        opcase(CLOSURE): {
            bvalue *dst;
            bproto *p = clos->proto->ptab[IGET_Bx(ins)];
            bclosure *cl = be_newclosure(vm, p->nupvals);
            cl->proto = p;
            reg = vm->reg;
            dst = RA();
            var_setclosure(dst, cl);
            be_initupvals(vm, cl);
            dispatch();
        }
        opcase(CLASS): {
            bclass *c = var_toobj(ktab + IGET_Bx(ins));
            be_class_upvalue_init(vm, c);
            dispatch();
        }
        opcase(GETMBR): {
#if BE_USE_PERF_COUNTERS
            vm->counter_get++;
#endif
            bvalue result;  /* copy result to a temp variable because the stack may be relocated in virtual member calls */
            bvalue *b = RKB(), *c = RKC();
            if (var_isinstance(b) && var_isstr(c)) {
                obj_attribute(vm, b, var_tostr(c), &result);
                reg = vm->reg;
            } else if (var_isclass(b) && var_isstr(c)) {
                class_attribute(vm, b, c, &result);
                reg = vm->reg;
            } else if (var_ismodule(b) && var_isstr(c)) {
                module_attribute(vm, b, c, &result);
                reg = vm->reg;
            } else {
                attribute_error(vm, "attribute", b, c);
                result = *RA();     /* avoid gcc warning for uninitialized variable result, this code is never reached */
            }
            bvalue *a = RA();
            *a = result;    /* assign the resul to the specified register on the updated stack */
            dispatch();
        }
        opcase(GETMET): {
#if BE_USE_PERF_COUNTERS
            vm->counter_get++;
#endif
            bvalue result;  /* copy result to a temp variable because the stack may be relocated in virtual member calls */
            bvalue *b = RKB(), *c = RKC();
            if (var_isinstance(b) && var_isstr(c)) {
                binstance *obj = var_toobj(b);
                int type = obj_attribute(vm, b, var_tostr(c), &result);
                reg = vm->reg;
                bvalue *a = RA();
                *a = result;
                if (var_basetype(a) == BE_FUNCTION) {
                    if ((type & BE_STATIC) || (type == BE_INDEX)) {    /* if instance variable then we consider it's non-method */
                        /* static method, don't bother with the instance */
                        a[1] = result;
                        var_settype(a, NOT_METHOD);
                    } else {
                        /* this is a real method (i.e. non-static) */
                        /* check if the object is a superinstance, if so get the lowest possible subclass */
                        while (obj->sub) {
                            obj = obj->sub;
                        }
                        var_setinstance(&a[1], obj);  /* replace superinstance by lowest subinstance */
                    }
                } else if (var_isclass(a)) {
                    /* in this case we have a class in a static or non-static member */
                    /* it's always treated like a statif function */
                    a[1] = result;
                    var_settype(a, NOT_METHOD);
                } else {
                    vm_error(vm, "attribute_error",
                        "class '%s' has no method '%s'",
                        str(be_instance_name(obj)), str(var_tostr(c)));
                }
            } else if (var_isclass(b) && var_isstr(c)) {
                class_attribute(vm, b, c, &result);
                reg = vm->reg;
                bvalue *a = RA();
                a[1] = result;
                var_settype(a, NOT_METHOD);
            } else if (var_ismodule(b) && var_isstr(c)) {
                module_attribute(vm, b, c, &result);
                reg = vm->reg;
                bvalue *a = RA();
                a[1] = result;
                var_settype(a, NOT_METHOD);
            } else {
                attribute_error(vm, "method", b, c);
            }
            dispatch();
        }
        opcase(SETMBR): {
#if BE_USE_PERF_COUNTERS
            vm->counter_set++;
#endif
            bvalue *a = RA(), *b = RKB(), *c = RKC();
            if (var_isinstance(a) && var_isstr(b)) {
                binstance *obj = var_toobj(a);
                bstring *attr = var_tostr(b);
                bvalue result = *c;
                if (var_isfunction(&result)) {
                    var_markstatic(&result);
                }
                if (!be_instance_setmember(vm, obj, attr, &result)) {
                    reg = vm->reg;
                    vm_error(vm, "attribute_error",
                        "class '%s' cannot assign to attribute '%s'",
                        str(be_instance_name(obj)), str(attr));
                }
                reg = vm->reg;
                dispatch();
            }
            if (var_isclass(a) && var_isstr(b)) {
                /* if value is a function, we mark it as a static to distinguish from methods */
                bclass *obj = var_toobj(a);
                bstring *attr = var_tostr(b);
                bvalue result = *c;
                if (var_isfunction(&result)) {
                    var_markstatic(&result);
                }
                if (!be_class_setmember(vm, obj, attr, &result)) {
                    reg = vm->reg;
                    vm_error(vm, "attribute_error",
                        "class '%s' cannot assign to static attribute '%s'",
                        str(be_class_name(obj)), str(attr));
                }
                reg = vm->reg;
                dispatch();
            }
            if (var_ismodule(a) && var_isstr(b)) {
                bmodule *obj = var_toobj(a);
                bstring *attr = var_tostr(b);
                if (be_module_setmember(vm, obj, attr, c)) {
                    reg = vm->reg;
                    dispatch();
                } else {
                    reg = vm->reg;
                    // fall through exception below
                }
            }
            attribute_error(vm, "writable attribute", a, b);
            dispatch();
        }
        opcase(GETIDX): {
            bvalue *b = RKB(), *c = RKC();
            if (var_isinstance(b)) {
                bvalue *top = vm->top;
                /* get method 'item' */
                obj_method(vm, b, str_literal(vm, "item"), vm->top);
                top[1] = *b; /* move object to argv[0] */
                top[2] = *c; /* move key to argv[1] */
                vm->top += 3;   /* prevent collection results */
                be_dofunc(vm, top, 2); /* call method 'item' */
                vm->top -= 3;
                reg = vm->reg;
                *RA() = *vm->top;   /* copy result to R(A) */
            } else if (var_isstr(b)) {
                bstring *s = be_strindex(vm, var_tostr(b), c);
                reg = vm->reg;
                var_setstr(RA(), s);
            } else {
                vm_error(vm, "type_error",
                    "value '%s' does not support subscriptable",
                    be_vtype2str(b));
            }
            dispatch();
        }
        opcase(SETIDX): {
            bvalue *a = RA(), *b = RKB(), *c = RKC();
            if (var_isinstance(a)) {
                bvalue *top = vm->top;
                /* get method 'setitem' */
                obj_method(vm, a, str_literal(vm, "setitem"), vm->top);
                top[1] = *a; /* move object to argv[0] */
                top[2] = *b; /* move key to argv[1] */
                top[3] = *c; /* move src to argv[2] */
                vm->top += 4;
                be_dofunc(vm, top, 3); /* call method 'setitem' */
                vm->top -= 4;
                reg = vm->reg;
            } else {
                vm_error(vm, "type_error",
                    "value '%s' does not support index assignment",
                    be_vtype2str(a));
            }
            dispatch();
        }
        opcase(SETSUPER): {
            bvalue *a = RA(), *b = RKB();
            if (var_isclass(a) && var_isclass(b)) {
                bclass *obj = var_toobj(a);
                if (!gc_isconst(obj))  {
                   be_class_setsuper(obj, var_toobj(b));
                } else {
                    vm_error(vm, "internal_error",
                    "cannot change superclass of a read-only class");
                }
            } else {
                vm_error(vm, "type_error",
                    "value '%s' does not support set super",
                    be_vtype2str(b));
            }
            dispatch();
        }
        opcase(CLOSE): {
            be_upvals_close(vm, RA());
            dispatch();
        }
        opcase(IMPORT): {
            bvalue *b = RKB();
            if (var_isstr(b)) {
                bstring *name = var_tostr(b);
                int res = be_module_load(vm, name);
                reg = vm->reg;
                switch (res) {
                case BE_OK: /* find the module */
                    be_stackpop(vm, 1);
                    *RA() = *vm->top;
                    break;
                case BE_EXCEPTION: /* pop the exception value and message */
                    be_pop(vm, 2);
                    be_throw(vm, BE_EXCEPTION);
                    break;
                default:
                    vm_error(vm, "import_error", "module '%s' not found", str(name));
                }
            } else {
                vm_error(vm, "type_error",
                    "import '%s' does not support import",
                    be_vtype2str(b));
            }
            dispatch();
        }
        opcase(CATCH): {
            bvalue *base = RA(), *top = vm->top;
            int i = 0, ecnt = IGET_RKB(ins), vcnt = IGET_RKC(ins);
            while (i < ecnt && !be_vm_iseq(vm, top, base + i)) {
                ++i;
            }
            if (!ecnt || i < ecnt) { /* exception catched */
                base = RA(), top = vm->top;
                for (i = 0; i < vcnt; ++i) {
                    *base++ = *top++;
                }
                vm->ip += 1; /* skip next instruction */
            }
            dispatch();
        }
        opcase(RAISE): {
            if (IGET_RA(ins) < 2) {  /* A==2 means no arguments are passed to RAISE, i.e. rethrow with current exception */
                bvalue *top = vm->top;
                top[0] = *RKB(); /* push the exception value to top */
                if (IGET_RA(ins)) { /* has exception argument? */
                    top[1] = *RKC(); /* push the exception argument to top + 1 */
                } else {
                    var_setnil(top + 1);
                }
                be_save_stacktrace(vm);
            }
            be_throw(vm, BE_EXCEPTION); /* throw / rethrow the exception */
            dispatch();
        }
        opcase(EXBLK): {
#if BE_USE_PERF_COUNTERS
            vm->counter_try++;
#endif
            if (!IGET_RA(ins)) {
                be_except_block_setup(vm);
                if (be_setjmp(vm->errjmp->b)) {
                    bvalue *top = vm->top;
                    bvalue e1 = top[0];
                    bvalue e2 = top[1];
                    be_except_block_resume(vm);
                    top = vm->top;
                    top[0] = e1;
                    top[1] = e2;
                    goto newframe;
                }
                reg = vm->reg;
            } else {
                be_except_block_close(vm, IGET_Bx(ins));
            }
            dispatch();
        }
        opcase(CALL): {
#if BE_USE_PERF_COUNTERS
            vm->counter_call++;
#endif
            bvalue *var = RA();  /* `var` is the register for the call followed by arguments */
            int mode = 0, argc = IGET_RKB(ins);  /* B contains number of arguments pushed on stack */
        recall: /* goto: instantiation class and call constructor */
            switch (var_type(var)) {
            case NOT_METHOD:
                var[0] = var[1];
                ++var, --argc, mode = 1;
                goto recall;
            case BE_CLASS:
                if (be_class_newobj(vm, var_toobj(var), var - reg, ++argc, mode)) {  /* instanciate object and find constructor */
                    reg = vm->reg + mode;  /* constructor found */
                    mode = 0;
                    var = RA() + 1; /* to next register */
                    reg = vm->reg;
                    goto recall; /* call constructor */
                }
                break;
            case BE_INSTANCE: {
                bvalue *v = var + argc++, temp;
                /* load the '()' method to `temp' */
                obj_method(vm, var, str_literal(vm, "()"), &temp);
                for (; v >= var; --v) v[1] = v[0];
                *var = temp;
                goto recall; /* call '()' method */
            }
            case BE_CLOSURE: {
                prep_closure(vm, var - reg, argc, mode);
                reg = vm->reg;  /* `reg` has changed, now new base register */
                goto newframe;  /* continue execution of the closure */
            }
            case BE_NTVCLOS: {
                bntvclos *f = var_toobj(var);
                push_native(vm, var, argc, mode);
                f->f(vm); /* call C primitive function */
                ret_native(vm);
                break;
            }
            case BE_NTVFUNC: {
                bntvfunc f = var_tontvfunc(var);
                push_native(vm, var, argc, mode);
                f(vm); /* call C primitive function */
                ret_native(vm);
                break;
            }
            case BE_CTYPE_FUNC: {
                if (vm->ctypefunc) {
                    push_native(vm, var, argc, mode);
                    const void* args = var_toobj(var);
                    vm->ctypefunc(vm, args);
                    ret_native(vm);
                } else {
                    vm_error(vm, "internal_error", "missing ctype_func handler");
                }
                break;
            }
            case BE_MODULE: {
                bvalue attr;
                var_setstr(&attr, str_literal(vm, "()"));
                module_attribute(vm, var, &attr, var);  /* exception if not found */
                goto recall; /* call '()' method */
                break;
            }
            default:
                call_error(vm, var);
            }
            reg = vm->reg;
            dispatch();
        }
        opcase(RET): {
            bcallframe *cf;
            bvalue *ret;
#if BE_USE_DEBUG_HOOK
            be_callhook(vm, BE_HOOK_RET);
#endif
            cf = vm->cf;
            ret = vm->cf->func;
            /* copy return value */
            if (IGET_RA(ins)) {
                *ret = *RKB();
            } else {
                var_setnil(ret);
            }
            vm->reg = cf->reg;
            vm->top = cf->top;
            vm->ip = cf->ip;
            be_stack_pop(&vm->callstack); /* pop don't delete */
            if (cf->status & BASE_FRAME) { /* entrance function */
                bstack *cs = &vm->callstack;
                if (!be_stack_isempty(cs)) {
                    vm->cf = be_stack_top(cs);
                }
                return;
            }
            vm->cf = be_stack_top(&vm->callstack);
            goto newframe;
        }
    }
}

static void prep_closure(bvm *vm, int pos, int argc, int mode)
{
    bvalue *v, *end;
    bproto *proto = var2cl(vm->reg + pos)->proto;
    push_closure(vm, vm->reg + pos, proto->nstack, mode);
    end = vm->reg + proto->argc;
    for (v = vm->reg + argc; v <= end; ++v) {
        var_setnil(v);
    }
    int v_offset = v - vm->stack;   /* offset from stack base, stack may be reallocated */
    if (proto->varg & BE_VA_VARARG) {  /* there are vararg at the last argument, build the list */
        /* code below uses mostly low-level calls for performance */
        be_stack_require(vm, argc + 4);   /* make sure we don't overflow the stack */
        int top_save_offset = vm->top - vm->stack;  /* save original stack, we need fresh slots to create the 'list' instance */
        vm->top = vm->stack + v_offset;  /* move top of stack right after last argument */
        be_newobject(vm, "list");  /* this creates 2 objects on stack: list instance, BE_LIST object */
        blist *list = var_toobj(vm->top-1);  /* get low-level BE_LIST structure */
        v = vm->reg + proto->argc - 1;  /* last argument */
        for (; v < vm->reg + argc; v++) {
            be_list_push(vm, list, v); /* push all varargs into list */       
        }
        *(vm->reg + proto->argc - 1) = *(vm->top-2);  /* change the vararg argument to now contain the list instance */
        vm->top = vm->stack + top_save_offset;  /* restore top of stack pointer */
    }
}

static void do_closure(bvm *vm, int pos, int argc)
{
    // bvalue *v, *end;
    // bproto *proto = var2cl(reg)->proto;
    // push_closure(vm, reg, proto->nstack, 0);
    // v = vm->reg + argc;
    // end = vm->reg + proto->argc;
    // for (; v <= end; ++v) {
    //     var_setnil(v);
    // }
    prep_closure(vm, pos, argc, 0);
    vm_exec(vm);
}

static void do_ntvclos(bvm *vm, int pos, int argc)
{
    bntvclos *f = var_toobj(vm->reg + pos);
    push_native(vm, vm->reg + pos, argc, 0);
    f->f(vm); /* call C primitive function */
    ret_native(vm);
}

static void do_ntvfunc(bvm *vm, int pos, int argc)
{
    bntvfunc f = var_tontvfunc(vm->reg + pos);
    push_native(vm, vm->reg + pos, argc, 0);
    f(vm); /* call C primitive function */
    ret_native(vm);
}

static void do_cfunc(bvm *vm, int pos, int argc)
{
    if (vm->ctypefunc) {
        const void* args = var_toobj(vm->reg + pos);
        push_native(vm, vm->reg + pos, argc, 0);
        vm->ctypefunc(vm, args);
        ret_native(vm);
    } else {
        vm_error(vm, "internal_error", "missing ctype_func handler");
    }
}

static void do_class(bvm *vm, int pos, int argc)
{
    if (be_class_newobj(vm, var_toobj(vm->reg + pos), pos, ++argc, 0)) {
        be_incrtop(vm);
        be_dofunc(vm, vm->reg + pos + 1, argc);
        be_stackpop(vm, 1);
    }
}

void be_dofunc(bvm *vm, bvalue *v, int argc)
{
    be_assert(vm->reg <= v && v < vm->stacktop);
    be_assert(vm->stack <= vm->reg && vm->reg < vm->stacktop);
    int pos = v - vm->reg;
    be_assert(!var_isstatic(v));
    switch (var_type(v)) {
    case BE_CLASS: do_class(vm, pos, argc); break;
    case BE_CLOSURE: do_closure(vm, pos, argc); break;
    case BE_NTVCLOS: do_ntvclos(vm, pos, argc); break;
    case BE_NTVFUNC: do_ntvfunc(vm, pos, argc); break;
    case BE_CTYPE_FUNC: do_cfunc(vm, pos, argc); break;
    default: call_error(vm, v);
    }
}

/* Default empty constructor */
int be_default_init_native_function(bvm *vm)
{
    int argc = be_top(vm);
    if (argc >= 1) {
        be_pushvalue(vm, 1);
    } else {
        be_pushnil(vm);
    }
    be_return(vm);
}

BERRY_API void be_set_obs_hook(bvm *vm, bobshook hook)
{
    (void)vm;       /* avoid comiler warning */
    (void)hook;     /* avoid comiler warning */

    vm->obshook = hook;
}

BERRY_API void be_set_obs_micros(bvm *vm, bmicrosfnct micros)
{
    vm->microsfnct = micros;
}

BERRY_API void be_set_ctype_func_hanlder(bvm *vm, bctypefunc handler)
{
    vm->ctypefunc = handler;
}

BERRY_API bctypefunc be_get_ctype_func_hanlder(bvm *vm)
{
    return vm->ctypefunc;
}
