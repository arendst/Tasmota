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

#define RA()   (reg + IGET_RA(ins))
#define RKB()  ((isKB(ins) ? ktab : reg) + KR2idx(IGET_RKB(ins)))
#define RKC()  ((isKC(ins) ? ktab : reg) + KR2idx(IGET_RKC(ins)))

#define var2cl(_v)          cast(bclosure*, var_toobj(_v))
#define var2real(_v)        (var_isreal(_v) ? (_v)->v.r : (breal)(_v)->v.i)
#define val2bool(v)         ((v) ? btrue : bfalse)
#define ibinop(op, a, b)    ((a)->v.i op (b)->v.i)

#if BE_USE_DEBUG_HOOK
  #define DEBUG_HOOK() \
    if (vm->hookmask & BE_HOOK_LINE) { \
        do_linehook(vm); \
        reg = vm->reg; \
    }
#else
  #define DEBUG_HOOK()
#endif

#define vm_exec_loop() \
    loop: \
        DEBUG_HOOK(); \
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
    if (var_isint(a) && var_isint(b)) { \
        res = ibinop(op, a, b); \
    } else if (var_isnumber(a) && var_isnumber(b)) { \
        res = var2real(a) op var2real(b); \
    } else if (var_isinstance(a)) { \
        res = object_eqop(vm, #op, iseq, a, b); \
    } else if (var_type(a) == var_type(b)) { /* same types */ \
        if (var_isnil(a)) { /* nil op nil */ \
            res = 1 op 1; \
        } else if (var_isbool(a)) { /* bool op bool */ \
            res = var_tobool(a) op var_tobool(b); \
        } else if (var_isstr(a)) { /* string op string */ \
            res = 1 op be_eqstr(a->v.s, b->v.s); \
        } else if (var_isclass(a) || var_isfunction(a)) { \
            res = var_toobj(a) op var_toobj(b); \
        } else { \
            binop_error(vm, #op, a, b); \
            res = bfalse; /* will not be executed */ \
        } \
    } else { /* different types */ \
        res = 1 op 0; \
    } \
    return res

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

static void precall(bvm *vm, bvalue *func, int nstack, int mode)
{
    bcallframe *cf;
    int expan = nstack + BE_STACK_FREE_MIN;
    if (vm->stacktop < func + expan) {
        size_t fpos = func - vm->stack;
        be_stack_expansion(vm, expan);
        func = vm->stack + fpos;
    }
    be_stack_push(vm, &vm->callstack, NULL);
    cf = be_stack_top(&vm->callstack);
    cf->func = func - mode;
    cf->top = vm->top;
    cf->reg = vm->reg;
    vm->reg = func + 1;
    vm->top = vm->reg + nstack;
    vm->cf = cf;
}

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
    if (be_instance_member(vm, obj, tobool, vm->top)) {
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
    case BE_INSTANCE:
        return obj2bool(vm, v);
    default:
        return btrue;
    }
}

static void obj_method(bvm *vm, bvalue *o, bstring *attr, bvalue *dst)
{
    binstance *obj = var_toobj(o);
    int type = be_instance_member(vm, obj, attr, dst);
    if (basetype(type) != BE_FUNCTION) {
        vm_error(vm, "attribute_error",
            "the '%s' object has no method '%s'",
            str(be_instance_name(obj)), str(attr));
    }
}

static int obj_attribute(bvm *vm, bvalue *o, bvalue *c, bvalue *dst)
{
    bvalue instance = *o; /* save instance to send it later to member */
    bstring *attr = var_tostr(c);
    binstance *obj = var_toobj(o);
    int type = be_instance_member(vm, obj, attr, dst);
    if (basetype(type) == BE_NIL) { /* if no method found, try virtual */
        /* get method 'member' */
        int type2 = be_instance_member(vm, obj, str_literal(vm, "member"), vm->top);
        if (basetype(type2) == BE_FUNCTION) {
            bvalue *top = vm->top;
            top[1] = instance; /* move instance to argv[0] */
            top[2] = *c; /* move method name to argv[1] */
            vm->top += 3;   /* prevent collection results */
            be_dofunc(vm, top, 2); /* call method 'member' */
            vm->top -= 3;
            *dst = *vm->top;   /* copy result to R(A) */
            type = var_type(dst);
        }
    }
    if (basetype(type) == BE_NIL) {
        vm_error(vm, "attribute_error",
            "the '%s' object has no attribute '%s'",
            str(be_instance_name(obj)), str(attr));
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
    vm->stack = be_malloc(vm, sizeof(bvalue) * BE_STACK_FREE_MIN);
    vm->stacktop = vm->stack + BE_STACK_FREE_MIN;
    vm->reg = vm->stack;
    vm->top = vm->reg;
    be_globalvar_init(vm);
    be_gc_setpause(vm, 1);
    be_loadlibs(vm);
#if BE_USE_OBSERVABILITY_HOOK
    vm->obshook = NULL;
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
    clos = var_toobj(vm->cf->func);
    ktab = clos->proto->ktab;
    reg = vm->reg;
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
                breal x = var2real(a), y = var2real(b);
                var_setreal(dst, x + y);
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
                var_setint(dst, -a->v.i);
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
            bvalue *a = RA(), *b = RKB(), *c = RKC();
            if (var_isinstance(b) && var_isstr(c)) {
                obj_attribute(vm, b, c, a);
                reg = vm->reg;
            } else if (var_ismodule(b) && var_isstr(c)) {
                bstring *attr = var_tostr(c);
                bmodule *module = var_toobj(b);
                bvalue *v = be_module_attr(vm, module, attr);
                if (v) {
                    *a = *v;
                } else {
                    bvalue *member = be_module_attr(vm, module, str_literal(vm, "member"));
                    var_setnil(a);
                    if (member && var_basetype(member) == BE_FUNCTION) {
                        bvalue *top = vm->top;
                        top[0] = *member;
                        top[1] = *c; /* move name to argv[0] */
                        vm->top += 2;   /* prevent collection results */
                        be_dofunc(vm, top, 1); /* call method 'method' */
                        vm->top -= 2;
                        *a = *vm->top;   /* copy result to R(A) */
                    }
                    if (var_basetype(a) == BE_NIL) {
                        vm_error(vm, "attribute_error",
                            "module '%s' has no attribute '%s'",
                            be_module_name(module), str(attr));
                    }
                }
            } else {
                attribute_error(vm, "attribute", b, c);
            }
            dispatch();
        }
        opcase(GETMET): {
            bvalue *a = RA(), *b = RKB(), *c = RKC();
            if (var_isinstance(b) && var_isstr(c)) {
                bvalue self = *b;
                bstring *attr = var_tostr(c);
                binstance *obj = var_toobj(b);
                int type = obj_attribute(vm, b, c, a);
                reg = vm->reg;
                if (basetype(type) == BE_FUNCTION) {
                    /* check if the object is a superinstance, if so get the lowest possible subclass */
                    while (obj->sub) {
                        obj = obj->sub;
                    }
                    var_setobj(&self, var_type(&self), obj);  /* replace superinstance by lowest subinstance */
                    a[1] = self;
                } else {
                    vm_error(vm, "attribute_error",
                        "class '%s' has no method '%s'",
                        str(be_instance_name(obj)), str(attr));
                }
            } else if (var_ismodule(b) && var_isstr(c)) {
                bstring *attr = var_tostr(c);
                bmodule *module = var_toobj(b);
                bvalue *src = be_module_attr(vm, module, attr);
                if (src) {
                    var_settype(a, NOT_METHOD);
                    a[1] = *src;
                } else {
                    bvalue *member = be_module_attr(vm, module, str_literal(vm, "member"));
                    var_setnil(a);
                    if (member && var_basetype(member) == BE_FUNCTION) {
                        bvalue *top = vm->top;
                        top[0] = *member;
                        top[1] = *c; /* move name to argv[0] */
                        vm->top += 2;   /* prevent collection results */
                        be_dofunc(vm, top, 1); /* call method 'method' */
                        vm->top -= 2;
                        var_settype(a, NOT_METHOD);
                        a[1] = *vm->top;   /* copy result to R(A) */
                    }
                    if (var_basetype(a) == BE_NIL) {
                        vm_error(vm, "attribute_error",
                            "module '%s' has no method '%s'",
                            be_module_name(module), str(attr));
                    }
                }
            } else {
                attribute_error(vm, "method", b, c);
            }
            dispatch();
        }
        opcase(SETMBR): {
            bvalue *a = RA(), *b = RKB(), *c = RKC();
            if (var_isinstance(a) && var_isstr(b)) {
                binstance *obj = var_toobj(a);
                bstring *attr = var_tostr(b);
                if (!be_instance_setmember(vm, obj, attr, c)) {
                    vm_error(vm, "attribute_error",
                        "class '%s' cannot assign to attribute '%s'",
                        str(be_instance_name(obj)), str(attr));
                }
                dispatch();
            }
            if (var_ismodule(a) && var_isstr(b)) {
                bmodule *obj = var_toobj(a);
                bstring *attr = var_tostr(b);
                bvalue tmp = *c; /* stack may change */
                bvalue *v = be_module_bind(vm, obj, attr);
                if (v != NULL) {
                    *v = tmp;
                    dispatch();
                }
                /* if it failed, try 'setmeemner' */
                bvalue *member = be_module_attr(vm, obj, str_literal(vm, "setmember"));
                if (member && var_basetype(member) == BE_FUNCTION) {
                    bvalue *top = vm->top;
                    top[0] = *member;
                    top[1] = *b; /* move name to argv[0] */
                    top[2] = tmp; /* move value to argv[1] */
                    vm->top += 3;   /* prevent collection results */
                    be_dofunc(vm, top, 2); /* call method 'setmember' */
                    vm->top -= 3;
                    dispatch();
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
                be_class_setsuper(obj, var_toobj(b));
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
            if (IGET_RA(ins) < 2) {
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
            if (!IGET_RA(ins)) {
                be_except_block_setup(vm);
                if (be_setjmp(vm->errjmp->b)) {
                    be_except_block_resume(vm);
                    goto newframe;
                }
                reg = vm->reg;
            } else {
                be_except_block_close(vm, IGET_Bx(ins));
            }
            dispatch();
        }
        opcase(CALL): {
            bvalue *var = RA();
            int mode = 0, argc = IGET_RKB(ins);
        recall: /* goto: instantiation class and call constructor */
            switch (var_type(var)) {
            case NOT_METHOD:
                var[0] = var[1];
                ++var, --argc, mode = 1;
                goto recall;
            case BE_CLASS:
                if (be_class_newobj(vm, var_toobj(var), var, ++argc, mode)) {
                    reg = vm->reg + mode;
                    mode = 0;
                    var = RA() + 1; /* to next register */
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
                bvalue *v, *end;
                bproto *proto = var2cl(var)->proto;
                push_closure(vm, var, proto->nstack, mode);
                reg = vm->reg;
                v = reg + argc;
                end = reg + proto->argc;
                for (; v < end; ++v) {
                    var_setnil(v);
                }
                goto newframe;
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
            case BE_MODULE: {
                bmodule *f = var_toobj(var);
                bvalue *member = be_module_attr(vm, f, str_literal(vm, "()"));
                if (member && var_basetype(member) == BE_FUNCTION) {
                    *var = *member;
                    goto recall; /* call '()' method */
                } else {
                    call_error(vm, var);
                }
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

static void do_closure(bvm *vm, bvalue *reg, int argc)
{
    bvalue *v, *end;
    bproto *proto = var2cl(reg)->proto;
    push_closure(vm, reg, proto->nstack, 0);
    v = vm->reg + argc;
    end = vm->reg + proto->argc;
    for (; v <= end; ++v) {
        var_setnil(v);
    }
    vm_exec(vm);
}

static void do_ntvclos(bvm *vm, bvalue *reg, int argc)
{
    bntvclos *f = var_toobj(reg);
    push_native(vm, reg, argc, 0);
    f->f(vm); /* call C primitive function */
    ret_native(vm);
}

static void do_ntvfunc(bvm *vm, bvalue *reg, int argc)
{
    bntvfunc f = var_tontvfunc(reg);
    push_native(vm, reg, argc, 0);
    f(vm); /* call C primitive function */
    ret_native(vm);
}

static void do_class(bvm *vm, bvalue *reg, int argc)
{
    if (be_class_newobj(vm, var_toobj(reg), reg, ++argc, 0)) {
        be_incrtop(vm);
        be_dofunc(vm, reg + 1, argc);
        be_stackpop(vm, 1);
    }
}

void be_dofunc(bvm *vm, bvalue *v, int argc)
{
    be_assert(vm->reg <= v && v < vm->stacktop);
    be_assert(vm->stack <= vm->reg && vm->reg < vm->stacktop);
    switch (var_type(v)) {
    case BE_CLASS: do_class(vm, v, argc); break;
    case BE_CLOSURE: do_closure(vm, v, argc); break;
    case BE_NTVCLOS: do_ntvclos(vm, v, argc); break;
    case BE_NTVFUNC: do_ntvfunc(vm, v, argc); break;
    default: call_error(vm, v);
    }
}

BERRY_API void be_set_obs_hook(bvm *vm, bobshook hook)
{
    (void)vm;       /* avoid comiler warning */
    (void)hook;     /* avoid comiler warning */

#if BE_USE_OBSERVABILITY_HOOK
    vm->obshook = hook;
#endif
}
