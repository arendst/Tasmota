/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_object.h"
#include "be_exec.h"
#include "be_mem.h"
#include "be_gc.h"
#include <string.h>

#define READLINE_STEP       100

static int l_assert(bvm *vm)
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

static int l_print(bvm *vm)
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

static int l_input(bvm *vm)
{
    if (be_top(vm) && be_isstring(vm, 1)) { /* echo prompt */
        be_writestring(be_tostring(vm, 1));
    }
    return m_readline(vm);
}

static int l_super(bvm *vm)
{
    if (be_top(vm)) {
        be_getsuper(vm, 1);
        be_return(vm);
    }
    be_return_nil(vm);
}

static int l_type(bvm *vm)
{
    if (be_top(vm)) {
        be_pushstring(vm, be_typename(vm, 1));
        be_return(vm);
    }
    be_return_nil(vm);
}

static int l_classname(bvm *vm)
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

static int l_classof(bvm *vm)
{
    if (be_top(vm) && be_classof(vm, 1)) {
        be_return(vm);
    }
    be_return_nil(vm);
}

static int l_number(bvm *vm)
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

static int l_int(bvm *vm)
{
    if (be_top(vm)) {
        if (be_isstring(vm, 1)) {
            const char *s = be_tostring(vm, 1);
            be_pushint(vm, be_str2int(s, NULL));
        } else if (be_isreal(vm, 1)) {
            be_pushint(vm, (bint)be_toreal(vm, 1));
        } else if (be_isint(vm, 1)) {
            be_pushvalue(vm, 1);
        } else {
            be_return_nil(vm);
        }
        be_return(vm);
    }
    be_return_nil(vm);
}

static int l_real(bvm *vm)
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

static int l_iterator(bvm *vm)
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

static int l_str(bvm *vm)
{
    if (be_top(vm)) {
        be_tostring(vm, 1);
    } else {
        be_pushstring(vm, "");
    }
    be_return(vm);
}

static int l_size(bvm *vm)
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

static int l_module(bvm *vm)
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

static int l_compile(bvm *vm)
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

static int l_issubclass(bvm *vm)
{
    return _issubv(vm, be_isclass);
}

static int l_isinstance(bvm *vm)
{
    return _issubv(vm, be_isinstance);
}

#if !BE_USE_PRECOMPILED_OBJECT
void be_load_baselib(bvm *vm)
{
    be_regfunc(vm, "assert", l_assert);
    be_regfunc(vm, "print", l_print);
    be_regfunc(vm, "input", l_input);
    be_regfunc(vm, "super", l_super);
    be_regfunc(vm, "type", l_type);
    be_regfunc(vm, "classname", l_classname);
    be_regfunc(vm, "classof", l_classof);
    be_regfunc(vm, "number", l_number);
    be_regfunc(vm, "str", l_str);
    be_regfunc(vm, "int", l_int);
    be_regfunc(vm, "real", l_real);
    be_regfunc(vm, "module", l_module);
    be_regfunc(vm, "size", l_size);
    be_regfunc(vm, "compile", l_compile);
    be_regfunc(vm, "issubclass", l_issubclass);
    be_regfunc(vm, "isinstance", l_isinstance);
    be_regfunc(vm, "__iterator__", l_iterator);
}
#else
extern const bclass be_class_list;
extern const bclass be_class_map;
extern const bclass be_class_range;
extern const bclass be_class_bytes;
extern int be_nfunc_open(bvm *vm);
/* @const_object_info_begin
vartab m_builtin (scope: local) {
    assert, func(l_assert)
    print, func(l_print)
    input, func(l_input)
    super, func(l_super)
    type, func(l_type)
    classname, func(l_classname)
    classof, func(l_classof)
    number, func(l_number)
    str, func(l_str)
    int, func(l_int)
    real, func(l_real)
    module, func(l_module)
    size, func(l_size)
    compile, func(l_compile)
    issubclass, func(l_issubclass)
    isinstance, func(l_isinstance)
    __iterator__, func(l_iterator)
    open, func(be_nfunc_open)
    list, class(be_class_list)
    map, class(be_class_map)
    range, class(be_class_range)
    bytes, class(be_class_bytes)
}
@const_object_info_end */
#include "../generate/be_fixed_m_builtin.h"
#include "be_var.h"

void be_load_baselib(bvm *vm)
{
    be_const_builtin_set(vm, &m_builtin_map, &m_builtin_vector);
}
#endif
