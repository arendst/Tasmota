/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "berry.h"

#if BE_USE_PREPROCESSOR

#include "be_object.h"
#include "be_map.h"
#include "be_string.h"
#include "be_mem.h"
#include "be_gc.h"
#include "be_vm.h"
#include "be_exec.h"
#include <string.h>

/* Ensure the VM's preprocessor map exists, lazily allocating on first use. */
static bmap* pp_ensure_map(bvm *vm)
{
    if (!vm->preprocessor) {
        vm->preprocessor = be_map_new(vm);
    }
    return vm->preprocessor;
}

/* Try to parse a string as an integer. Returns btrue and sets *out on success. */
static bbool pp_parse_int(const char *s, int len, bint *out)
{
    bint val = 0;
    int i = 0;
    int neg = 0;
    if (len == 0) {
        return bfalse;
    }
    if (s[0] == '-') {
        neg = 1;
        i = 1;
    } else if (s[0] == '+') {
        i = 1;
    }
    if (i >= len) {
        return bfalse;
    }
    for (; i < len; i++) {
        if (s[i] < '0' || s[i] > '9') {
            return bfalse;
        }
        val = val * 10 + (s[i] - '0');
    }
    *out = neg ? -val : val;
    return btrue;
}

/* Store a macro value following the ingestion rules:
 * - NULL value -> BE_INT 1 (truthy empty define)
 * - "0" or numeric string -> BE_INT with parsed value
 * - Quoted string "..." or '...' -> BE_STRING with unquoted content
 * - Otherwise -> BE_STRING with raw value
 */
static void pp_store_value(bvm *vm, bmap *map, bstring *key, const char *value)
{
    bvalue val;
    if (value == NULL) {
        var_setint(&val, 1);
    } else {
        int len = (int)strlen(value);
        bint ival;
        /* check for quoted string */
        if (len >= 2 &&
            ((value[0] == '"' && value[len - 1] == '"') ||
             (value[0] == '\'' && value[len - 1] == '\''))) {
            /* strip quotes and store as string */
            bstring *s = be_newstrn(vm, value + 1, len - 2);
            var_setstr(&val, s);
        } else if (pp_parse_int(value, len, &ival)) {
            var_setint(&val, ival);
        } else {
            bstring *s = be_newstrn(vm, value, len);
            var_setstr(&val, s);
        }
    }
    be_map_insertstr(vm, map, key, &val);
}

BERRY_API void be_preprocessor_define(bvm *vm, const char *name, const char *value)
{
    bmap *map = pp_ensure_map(vm);
    bstring *key = be_newstr(vm, name);
    /* protect key from GC during pp_store_value which may allocate */
    var_setstr(vm->top, key);
    be_stackpush(vm);
    pp_store_value(vm, map, key, value);
    be_stackpop(vm, 1);
}

BERRY_API void be_preprocessor_undef(bvm *vm, const char *name)
{
    if (vm->preprocessor) {
        bstring *key = be_newstr(vm, name);
        be_map_removestr(vm, vm->preprocessor, key);
    }
}

BERRY_API void be_preprocessor_clear(bvm *vm)
{
    /* Just detach the map from the VM. The GC will collect it
     * since it becomes unreachable. Do NOT call be_map_delete
     * directly — the map is a GC object tracked in the GC list. */
    vm->preprocessor = NULL;
}

#endif /* BE_USE_PREPROCESSOR */
