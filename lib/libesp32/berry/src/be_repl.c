/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "berry.h"
#include "be_repl.h"
#include <string.h>

#define safecall(func, ...) if (func) { func(__VA_ARGS__); }

#if BE_USE_SCRIPT_COMPILER

static int try_return(bvm *vm, const char *line)
{
    int res, idx;
    line = be_pushfstring(vm, "return (%s)", line);
    idx = be_absindex(vm, -1); /* get the source text absolute index */
    res = be_loadbuffer(vm, "stdin", line, strlen(line)); /* compile line */
    be_remove(vm, idx); /* remove source string */
    return res;
}

static bbool is_multline(bvm *vm)
{
    const char *msg = be_tostring(vm, -1);
    size_t len = strlen(msg);
    if (len > 5) { /* multi-line text if the error message is 'EOS' at the end */
        return !strcmp(msg + len - 5, "'EOS'");
    }
    return bfalse;
}

static int compile(bvm *vm, char *line, breadline getl, bfreeline freel)
{
    int res = try_return(vm, line);
    if (be_getexcept(vm, res) == BE_SYNTAX_ERROR) {
        be_pop(vm, 2); /* pop exception values */
        be_pushstring(vm, line);
        safecall(freel, line); /* free line buffer */
        for (;;) {
            const char *src = be_tostring(vm, -1); /* get source code */
            int idx = be_absindex(vm, -1); /* get the source text absolute index */
            /* compile source line */
            res = be_loadbuffer(vm, "stdin", src, strlen(src));
            if (!res || !is_multline(vm)) {
                be_remove(vm, idx); /* remove source code */
                return res;
            }
            be_pop(vm, 2); /* pop exception values */
            line = getl(">> "); /* read a new input line */
            be_pushfstring(vm, "\n%s", line);
            safecall(freel, line); /* free line buffer */
            be_strconcat(vm, -2);
            be_pop(vm, 1); /* pop new line */
        }
    } else {
        safecall(freel, line); /* free line buffer */
    }
    return res;
}

static int call_script(bvm *vm)
{
    int res = be_pcall(vm, 0); /* call the main function */
    switch (res) { 
    case BE_OK: /* execution succeed */
        if (!be_isnil(vm, -1)) { /* print return value when it's not nil */
            be_dumpvalue(vm, -1);
        }
        be_pop(vm, 1); /* pop the result value */
        break;
    case BE_EXCEPTION: /* vm run error */
        be_dumpexcept(vm);
        be_pop(vm, 1); /* pop the function value */
        break;
    default: /* BE_EXIT or BE_MALLOC_FAIL */
        return res;
    }
    return 0;
}

BERRY_API int be_repl(bvm *vm, breadline getline, bfreeline freeline)
{
    char *line;
    be_assert(getline != NULL);
    while ((line = getline("> ")) != NULL) {
        int res = compile(vm, line, getline, freeline);
        if (res == BE_MALLOC_FAIL)
            return BE_MALLOC_FAIL;
        if (res) {
            be_dumpexcept(vm);
        } else { /* compiled successfully */
            res = call_script(vm);
            if (res) {
                return res == BE_EXIT ? be_toindex(vm, -1) : res;
            }
        }
    }
    be_writenewline();
    return 0;
}

#endif
