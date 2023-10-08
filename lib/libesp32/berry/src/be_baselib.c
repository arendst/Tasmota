/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_object.h"
#include "be_vm.h"
#include "be_exec.h"
#include "be_mem.h"
#include "be_gc.h"
#include "be_class.h"
#include "be_vector.h"
#include "be_string.h"
#include "be_map.h"
#include "be_strlib.h"
#include <string.h>

#define READLINE_STEP       100

int be_baselib_assert(bvm *vm)
{
    int argc = be_top(vm);
    /* assertion fails when there is no argument
     * or the first argument is nil or false. */
    if (!argc || !be_tobool(vm, 1)) {
        const char *msg = "assert failed!";
        if (argc >= 2 && be_isstring(vm, 2)) {
            msg = be_tostring(vm, 2);
        }
        be_raise(vm, "assert_failed", msg);
    }
    be_return_nil(vm);
}

int be_baselib_print(bvm *vm)
{
    int i, argc = be_top(vm);
    for (i = 1; i <= argc; ++i) {
        const char *str = be_tostring(vm, i);
        size_t len = be_strlen(vm, i);
        be_writebuffer(str, len);
        if (i < argc) {
            be_writebuffer(" ", 1);
        }
    }
    be_writenewline();
    be_return_nil(vm);
}

static int m_readline(bvm *vm)
{
    size_t pos = 0, size = READLINE_STEP;
    char *buffer = be_malloc(vm, size);
    char *res = be_readstring(buffer, (int)size);
    while (res) {
        pos += strlen(buffer + pos) - 1;
        if (!pos || buffer[pos] == '\n') {
            buffer[pos] = '\0'; /* trim \n */
            break;
        }
        buffer = be_realloc(vm, buffer, size, size + READLINE_STEP);
        res = be_readstring(buffer + pos + 1, READLINE_STEP);
        size += READLINE_STEP;
    }
    be_pushstring(vm, buffer);
    be_free(vm, buffer, size);
    be_return(vm);
}

int be_baselib_input(bvm *vm)
{
    if (be_top(vm) && be_isstring(vm, 1)) { /* echo prompt */
        be_writestring(be_tostring(vm, 1));
    }
    return m_readline(vm);
}

/* Look in the current class and all super classes for a method corresponding to a specific closure pointer */
static bclass *find_class_closure(bclass *cl, bclosure *needle)
{
    while (cl) {
        bmapnode *node;  /* iterate on members of the class */
        bmap *members = be_class_members(cl);
        if (members) {  /* only iterate if there are members */
            bmapiter iter = be_map_iter();
            while ((node = be_map_next(members, &iter)) != NULL) {
                if (var_primetype(&node->value) == BE_CLOSURE) {  /* only native functions are considered */
                    bclosure *clos_iter = var_toobj(&node->value);  /* retrieve the method's closure */
                    if (clos_iter == needle) {
                        /* we found the closure, we now know its class */
                        return cl;
                    }
                }
            }
        }
        cl = be_class_super(cl);  /* move to super class */
    }
    return NULL;  /* not found */
}

static bbool obj2int(bvm *vm, bvalue *var, bint *val)
{
    binstance *obj = var_toobj(var);
    bstring *toint = str_literal(vm, "toint");
    /* get operator method */
    // TODO what if `tobool` is static
    int type = be_instance_member(vm, obj, toint, vm->top);
    if (type != BE_NONE && type != BE_NIL) {
        vm->top[1] = *var; /* move self to argv[0] */
        be_dofunc(vm, vm->top, 1); /* call method 'tobool' */
        /* check the return value */
        if (var_isint(vm->top)) {
            *val = var_toint(vm->top);
            return btrue;
        }
    }
    return bfalse;
}

int be_baselib_super(bvm *vm)
{
    int argc = be_top(vm);

    /* if no argument, or arg 1 is nil, return nil */
    if (argc == 0 || be_isnil(vm, 1)) {
        be_return_nil(vm);
    }

    /* if arg 1 is a class, simply return super */
    if (be_isclass(vm, 1)) {
        be_getsuper(vm, 1);
        be_return(vm);
    }

    /* arg 1 is an instance */
    if (be_isinstance(vm, 1)) {
        binstance *o = var_toobj(be_indexof(vm, 1));
        bclass *target_class = NULL;  /* the minimal class expected, or any super class */
        bclass *base_class = NULL;  /* current class of the caller, if any */

        /* if arg 2 is present, it must be a class */
        if (argc >= 2) {
            if (be_isclass(vm, 2)) {
                target_class = var_toobj(be_indexof(vm, 2));
            } else if (be_isnil(vm, 2)) {
                // ignore, revert to standard super() behavior if second arg is explicit nil
            } else {
                be_raise(vm, "type_error", "leveled super() requires 'instance' and 'class' arguments");
            }
        }

        /* now the more complex part, if arg 1 is an instance */
        /* if instance is the sole argument, try to find if it comes from a method of a class and set 'base_class' accordinly */
        /* later it will be equivalent to passing this class as second argument */
        if (argc == 1) {
            /* we look in the callstack for the caller's closure */
            int size = be_stack_count(&vm->callstack);
            if (size >= 2) {  /* need at least 2 stackframes: current (for super() native) and caller (the one we are interested in) */
                bcallframe *caller = be_vector_at(&vm->callstack, size - 2);  /* get the callframe of caller */
                bvalue *func = caller->func;  /* function object of caller */
                if (var_primetype(func) == BE_CLOSURE) {  /* only useful if the caller is a Berry closure (i.e. not native) */
                    bclosure *clos_ctx = var_toobj(func);  /* this is the closure we look for in the class chain */
                    base_class = find_class_closure(o->_class, clos_ctx);  /* iterate on current and super classes to find where the closure belongs */
                }
            }
        }

        if (base_class || target_class) {
            if (base_class) {
                target_class = base_class->super;
                if (!target_class) be_return_nil(vm);   /* fast exit if top class */
            }
            /* leveled super, i.e. fix the parenthood class level */
            if (o) {
                o = be_instance_super(o);   /* always skip the current class and move to super */
            }
            while (o) {
                bclass *c = be_instance_class(o);
                if (c == target_class) break;         /* found */
                o = be_instance_super(o);
            }
            bvalue *top = be_incrtop(vm);
            if (o) {
                var_setinstance(top, o);    /* return the instance with the specified parent class */
            } else {
                var_setnil(top);            /* not found, return nil */
            }
            be_return(vm);
        } else {
            be_getsuper(vm, 1);
            be_return(vm);
        }
    }

    /* fall through, return nil if we don't know what to do */
    be_return_nil(vm);
}

int be_baselib_type(bvm *vm)
{
    if (be_top(vm)) {
        be_pushstring(vm, be_typename(vm, 1));
        be_return(vm);
    }
    be_return_nil(vm);
}

int be_baselib_classname(bvm *vm)
{
    if (be_top(vm)) {
        const char *t = be_classname(vm, 1);
        if (t) {
            be_pushstring(vm, t);
            be_return(vm);
        }
    }
    be_return_nil(vm);
}

int be_baselib_classof(bvm *vm)
{
    if (be_top(vm) && be_classof(vm, 1)) {
        be_return(vm);
    }
    be_return_nil(vm);
}

int be_baselib_number(bvm *vm)
{
    if (be_top(vm)) {
        if (be_isstring(vm, 1)) {
            be_str2num(vm, be_tostring(vm, 1));
            be_return(vm);
        } else if (be_isnumber(vm, 1)) {
            be_pushvalue(vm, 1);
            be_return(vm);
        }
    }
    be_return_nil(vm);
}

int be_baselib_int(bvm *vm)
{
    if (be_top(vm)) {
        if (be_isstring(vm, 1)) {
            const char *s = be_tostring(vm, 1);
            be_pushint(vm, be_str2int(s, NULL));
        } else if (be_isreal(vm, 1)) {
            be_pushint(vm, (bint)be_toreal(vm, 1));
        } else if (be_isint(vm, 1)) {
            be_pushvalue(vm, 1);
        } else if (be_isbool(vm, 1)) {
            be_pushint(vm, be_tobool(vm, 1) ? 1 : 0);
        } else if (be_iscomptr(vm, 1)) {
            intptr_t p = (intptr_t) be_tocomptr(vm, 1);
            be_pushint(vm, (int) p);
        } else if (be_isinstance(vm, 1)) {
            /* try to call `toint` method */
            bvalue *v = be_indexof(vm, 1);
            bint val;
            if (obj2int(vm, v, &val)) {
                be_pushint(vm, val);
            } else {
                be_return_nil(vm);
            }
        } else {
            be_return_nil(vm);
        }
        be_return(vm);
    }
    be_return_nil(vm);
}

int be_baselib_real(bvm *vm)
{
    if (be_top(vm)) {
        if (be_isstring(vm, 1)) {
            const char *s = be_tostring(vm, 1);
            be_pushreal(vm, be_str2real(s, NULL));
        } else if (be_isint(vm, 1)) {
            be_pushreal(vm, (breal)be_toint(vm, 1));
        } else if (be_isreal(vm, 1)) {
            be_pushvalue(vm, 1);
        } else {
            be_return_nil(vm);
        }
        be_return(vm);
    }
    be_return_nil(vm);
}

static int check_method(bvm *vm, const char *attr)
{
    return be_top(vm) &&
        be_isinstance(vm, 1) && be_getmethod(vm, 1, attr);
}

int be_baselib_iterator(bvm *vm)
{
    if (be_top(vm) && be_isfunction(vm, 1)) {
        be_return(vm); /* return the argument[0]::function */
    }
    if (check_method(vm, "iter")) {
        be_pushvalue(vm, 1);
        be_call(vm, 1);
        be_pop(vm, 1);
        be_return(vm);
    }
    be_return_nil(vm);
}

/* call a function with variable number of arguments */
/* first argument is a callable object (function, closure, native function, native closure) */
/* then all subsequent arguments are pushed except the last one */
/* If the last argument is a 'list', then all elements are pushed as arguments */
/* otherwise the last argument is pushed as well */
static int l_call(bvm *vm)
{
    int top = be_top(vm);
    if (top >= 1 && be_isfunction(vm, 1)) {
        size_t arg_count = top - 1;  /* we have at least 'top - 1' arguments */
        /* test if last argument is a list */

        if (top > 1 && be_isinstance(vm, top) && be_getmember(vm, top, ".p") && be_islist(vm, top + 1)) {
            int32_t list_size = be_data_size(vm, top + 1);

            if (list_size > 0) {
                be_stack_require(vm, list_size + 3);   /* make sure we don't overflow the stack */
                for (int i = 0; i < list_size; i++) {
                    be_pushnil(vm);
                }
                be_moveto(vm, top + 1, top + 1 + list_size);
                be_moveto(vm, top, top + list_size);

                be_pushiter(vm, -1);
                while (be_iter_hasnext(vm, -2)) {
                    be_iter_next(vm, -2);
                    be_moveto(vm, -1, top);
                    top++;
                    be_pop(vm, 1);
                }
                be_pop(vm, 1);  /* remove iterator */
            }
            be_pop(vm, 2);
            arg_count = arg_count - 1 + list_size;
        }
        /* actual call */
        be_call(vm, arg_count);
        /* remove args */
        be_pop(vm, arg_count);
        /* return value */

        be_return(vm);
    }
    be_raise(vm, "value_error", "first argument must be a function");
    be_return_nil(vm);
}

int be_baselib_str(bvm *vm)
{
    if (be_top(vm)) {
        be_tostring(vm, 1);
    } else {
        be_pushstring(vm, "");
    }
    be_return(vm);
}

static int l_bool(bvm *vm)
{
    if (be_top(vm)) {
        be_pushbool(vm, be_tobool(vm, 1));
    } else {
        be_pushbool(vm, bfalse);
    }
    be_return(vm);
}


int be_baselib_size(bvm *vm)
{
    if (be_top(vm) && be_isstring(vm, 1)) {
        be_pushint(vm, be_strlen(vm, 1));
        be_return(vm);
    }
    if (check_method(vm, "size")) {
        be_pushvalue(vm, 1);
        be_call(vm, 1);
        be_pop(vm, 1);
        be_return(vm);
    }
    be_return_nil(vm);
}

int be_baselib_module(bvm *vm)
{
    int argc = be_top(vm);
    be_newmodule(vm);
    if (argc > 0 && be_isstring(vm, 1)) {
        be_setname(vm, -1, be_tostring(vm, 1));
    }
    be_return(vm);
}

#if BE_USE_SCRIPT_COMPILER
static int raise_compile_error(bvm *vm)
{
    be_pop(vm, 2); /* pop the exception value and message */
    be_throw(vm, BE_EXCEPTION);
    return 0;
}

static int m_compile_str(bvm *vm)
{
    int len = be_strlen(vm, 1);
    const char *src = be_tostring(vm, 1);
    int res = be_loadbuffer(vm, "string", src, len);
    if (res == BE_OK) {
        be_return(vm);
    }
    return raise_compile_error(vm);
}

static int m_compile_file(bvm *vm)
{
    const char *fname = be_tostring(vm, 1);
    int res = be_loadfile(vm, fname);
    if (res == BE_OK) {
        be_return(vm);
    } else if (res == BE_IO_ERROR) {
        be_pushstring(vm, "io_error");
        be_pushvalue(vm, -2);
    }
    return raise_compile_error(vm);
}
#endif

int be_baselib_compile(bvm *vm)
{
#if BE_USE_SCRIPT_COMPILER
    if (be_top(vm) && be_isstring(vm, 1)) {
        if (be_top(vm) >= 2 && be_isstring(vm, 2)) {
            const char *s = be_tostring(vm, 2);
            if (!strcmp(s, "string")) {
                return m_compile_str(vm);
            }
            if (!strcmp(s, "file")) {
                return m_compile_file(vm);
            }
        } else {
            return m_compile_str(vm);
        }
    }
#endif
    be_return_nil(vm);
}

static int _issubv(bvm *vm, bbool (*filter)(bvm*, int))
{
    bbool status = bfalse;
    if (be_top(vm) >= 2 && filter(vm, 1)) {
        be_pushvalue(vm, 2);
        status = be_isderived(vm, 1);
    }
    be_pushbool(vm, status);
    be_return(vm);
}

int be_baselib_issubclass(bvm *vm)
{
    return _issubv(vm, be_isclass);
}

int be_baselib_isinstance(bvm *vm)
{
    return _issubv(vm, be_isinstance);
}

#if !BE_USE_PRECOMPILED_OBJECT
void be_load_baselib(bvm *vm)
{
    be_regfunc(vm, "assert", be_baselib_assert);
    be_regfunc(vm, "print", be_baselib_print);
    be_regfunc(vm, "input", be_baselib_input);
    be_regfunc(vm, "super", be_baselib_super);
    be_regfunc(vm, "type", be_baselib_type);
    be_regfunc(vm, "classname", be_baselib_classname);
    be_regfunc(vm, "classof", be_baselib_classof);
    be_regfunc(vm, "number", be_baselib_number);
    be_regfunc(vm, "str", be_baselib_str);
    be_regfunc(vm, "int", be_baselib_int);
    be_regfunc(vm, "real", be_baselib_real);
    be_regfunc(vm, "module", be_baselib_module);
    be_regfunc(vm, "size", be_baselib_size);
    be_regfunc(vm, "compile", be_baselib_compile);
    be_regfunc(vm, "issubclass", be_baselib_issubclass);
    be_regfunc(vm, "isinstance", be_baselib_isinstance);
    be_regfunc(vm, "__iterator__", be_baselib_iterator);
}

/* call must be added later to respect order of builtins */
void be_load_baselib_next(bvm *vm)
{
    be_regfunc(vm, "call", l_call);
    be_regfunc(vm, "bool", l_bool);
    be_regfunc(vm, "format", be_str_format);
}
#else
extern const bclass be_class_list;
extern const bclass be_class_map;
extern const bclass be_class_range;
extern const bclass be_class_bytes;
extern int be_nfunc_open(bvm *vm);
/* @const_object_info_begin
vartab m_builtin (scope: local) {
    assert, func(be_baselib_assert)
    print, func(be_baselib_print)
    input, func(be_baselib_input)
    super, func(be_baselib_super)
    type, func(be_baselib_type)
    classname, func(be_baselib_classname)
    classof, func(be_baselib_classof)
    number, func(be_baselib_number)
    str, func(be_baselib_str)
    int, func(be_baselib_int)
    real, func(be_baselib_real)
    module, func(be_baselib_module)
    size, func(be_baselib_size)
    compile, func(be_baselib_compile)
    issubclass, func(be_baselib_issubclass)
    isinstance, func(be_baselib_isinstance)
    __iterator__, func(be_baselib_iterator)
    open, func(be_nfunc_open)
    list, class(be_class_list)
    map, class(be_class_map)
    range, class(be_class_range)
    bytes, class(be_class_bytes)
    call, func(l_call)
    bool, func(l_bool)
    format, func(be_str_format)
}
@const_object_info_end */
#include "../generate/be_fixed_m_builtin.h"
#include "be_var.h"

void be_load_baselib(bvm *vm)
{
    be_const_builtin_set(vm, &m_builtin_map, &m_builtin_vector);
}
#endif
