/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_var.h"
#include "be_vm.h"
#include "be_vector.h"
#include "be_string.h"
#include "be_map.h"
#include "be_gc.h"
#include "be_class.h"
#include <string.h>

#define global(vm)      ((vm)->gbldesc.global)
#define builtin(vm)     ((vm)->gbldesc.builtin)

extern BERRY_LOCAL bclass_array be_class_table;

void be_globalvar_init(bvm *vm)
{
    global(vm).vtab = be_map_new(vm);
    be_gc_fix(vm, gc_object(global(vm).vtab));
    be_vector_init(vm, &global(vm).vlist, sizeof(bvalue));
#if !BE_USE_PRECOMPILED_OBJECT
    builtin(vm).vtab = be_map_new(vm);
    be_vector_init(vm, &builtin(vm).vlist, sizeof(bvalue));
    be_gc_fix(vm, gc_object(builtin(vm).vtab));
#endif
}

void be_globalvar_deinit(bvm *vm)
{
    global(vm).vtab = NULL;
    be_vector_delete(vm, &global(vm).vlist);
#if !BE_USE_PRECOMPILED_OBJECT
    builtin(vm).vtab = NULL;
    be_vector_delete(vm, &builtin(vm).vlist);
#endif
}

/* This function is called when the global was not found */
/* If the name looked for matches a static class, a global with the class name is created lazily */
/* Pre: no global exists with name `name`*/
/* Returns: idx of newly created global, or -1 if not found */
static int global_native_class_find(bvm *vm, bstring *name)
{
    const char* cl_name = str(name);
    bclass_ptr* class_p = &be_class_table[0];
    for (; *class_p; class_p++) {
        const bclass* cl = *class_p;

        if (!strcmp(str(cl->name), cl_name)) {
            /* class name matches */
            int idx = be_global_new(vm, name);
            bvalue *v = be_global_var(vm, idx);
            var_setclass(v, (void*) cl);
            return idx;
        }
    }
    return -1;
}

static int global_find(bvm *vm, bstring *name)
{
    bvalue *res = be_map_findstr(vm, global(vm).vtab, name);
    if (res) {
        int idx = var_toidx(res);
        if (idx >= 0) {
            return idx + be_builtin_count(vm);
        } else {
            return idx;     /* the global does not exist (-1) or was underfined (< -1)*/
        }
    }
    return -1; /* not found */
}

bbool be_global_undef(bvm *vm, bstring *name)
{
    int idx = global_find(vm, name);
    if (idx >= 0) {
        bvalue *desc = be_map_findstr(vm, global(vm).vtab, name);
        int index = var_toidx(desc);
        var_setint(desc, -index - 2);   /* negate the index to mark it as undefined */

        bvalue* val = be_vector_at(&global(vm).vlist, index);
        var_setnil(val);
        return btrue;
    }
    return bfalse;
}

int be_global_find(bvm *vm, bstring *name)
{
    int res = global_find(vm, name);                /* returns negative if not found, -1 if does not exist, < -1 if existed but undefined */
    if (res < 0) {
        res = be_builtin_find(vm, name);            /* returns -1 if not found */
    }
    if (res < 0) {
        res = global_native_class_find(vm, name);   /* returns -1 if not found */
    }
    return res;
}

static int global_new_anonymous(bvm *vm)
{
    int idx = be_global_count(vm);
    /* allocate space for new variables */
    be_vector_resize(vm, &global(vm).vlist, idx + 1);
    /* set the new variable to nil */
    var_setnil((bvalue *)global(vm).vlist.end);
    return idx;
}

int be_global_new(bvm *vm, bstring *name)
{
    int idx = global_find(vm, name);
    if (idx < 0) {
        bvalue *desc;
        if (idx == -1) {
            idx = global_new_anonymous(vm);
            desc = be_map_insertstr(vm, global(vm).vtab, name, NULL);
            var_setint(desc, idx);
        } else {
            /* the global exists but was undefined */
            idx = -idx - 2;
            desc = be_map_findstr(vm, global(vm).vtab, name);
            var_setint(desc, idx);
        }
        idx += be_builtin_count(vm);
    }
    return idx;
}

bvalue* be_global_var(bvm *vm, int index)
{
    int bcnt = be_builtin_count(vm);
    if (index < bcnt) {
        return be_vector_at(&builtin(vm).vlist, index);
    }
    index -= bcnt;
    return be_vector_at(&global(vm).vlist, index);
}

void be_global_release_space(bvm *vm)
{
    be_map_compact(vm, global(vm).vtab);
    be_vector_release(vm, &global(vm).vlist);
}

int be_builtin_find(bvm *vm, bstring *name)
{
    bvalue *res = be_map_findstr(vm, builtin(vm).vtab, name);
    if (res) {
        return var_toidx(res);
    }
    return -1; /* not found */
}

bstring* be_builtin_name(bvm *vm, int index)
{
    bmap *map = builtin(vm).vtab;
    bmapnode *end, *node = map->slots;
    for (end = node + map->size; node < end; ++node) {
        if (var_isstr(&node->key) && node->value.v.i == index) {
            return node->key.v.s;
        }
    }
    return NULL;
}

#if !BE_USE_PRECOMPILED_OBJECT
int be_builtin_new(bvm *vm, bstring *name)
{
    int idx = be_builtin_find(vm, name);
    if (idx == -1) {
        bvalue *desc;
        idx = be_map_count(builtin(vm).vtab);
        desc = be_map_insertstr(vm, builtin(vm).vtab, name, NULL);
        var_setint(desc, idx);
        be_vector_resize(vm, &builtin(vm).vlist, idx + 1);
        /* set the new variable to nil */
        var_setnil((bvalue*)(builtin(vm).vlist.end));
    }
    return idx;
}

void be_bulitin_release_space(bvm *vm)
{
    be_map_compact(vm, builtin(vm).vtab);
    be_vector_release(vm, &builtin(vm).vlist);
}
#else
void be_const_builtin_set(bvm *vm, const bmap *map, const bvector *vec)
{
    builtin(vm).vtab = cast(bmap*, map);
    builtin(vm).vlist = *vec;
}
#endif
