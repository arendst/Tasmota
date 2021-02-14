/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_module.h"
#include "be_string.h"
#include "be_strlib.h"
#include "be_list.h"
#include "be_exec.h"
#include "be_map.h"
#include "be_gc.h"
#include "be_mem.h"
#include "be_vm.h"
#include <string.h>

/* the maximum suffix length */
#define SUFFIX_LEN      5 /* length of (.be .bec .so .dll) + 1 */

/* check POSIX platforms */
#if defined(__linux__) || \
    defined(__APPLE__) || defined(__unix__) || defined(__CYGWIN__)
  #define __POSIX_OS__
#endif

#if BE_USE_SHARED_LIB
  #if defined(__POSIX_OS__)
    #define DLL_SUFFIX ".so"
  #elif defined(_WIN32)
    #define DLL_SUFFIX ".dll"
  #else
    #define DLL_SUFFIX ""
    #warning module: unsuppord OS
  #endif
#endif

extern BERRY_LOCAL const bntvmodule* const be_module_table[];

static bmodule* native_module(bvm *vm, const bntvmodule *nm, bvalue *dst);

static const bntvmodule* find_native(bstring *path)
{
    const bntvmodule *module;
    const bntvmodule* const *node = be_module_table;
    for (; (module = *node) != NULL; ++node) {
        if (!strcmp(module->name, str(path))) {
            return module;
        }
    }
    return NULL;
}

static void insert_attrs(bvm *vm, bmap *table, const bntvmodule *nm)
{
    size_t i;
    for (i = 0; i < nm->size; ++i) {
        const bntvmodobj *node = nm->attrs + i;
        bstring *name = be_newstr(vm, node->name);
        bvalue *v = be_map_insertstr(vm, table, name, NULL);
        be_assert(node->type <= BE_CMODULE);
        switch (node->type) {
        case BE_CNIL:
            var_setnil(v);
            break;
        case BE_CINT:
            var_setint(v, node->u.i);
            break;
        case BE_CREAL:
            var_setreal(v, node->u.r);
            break;
        case BE_CBOOL:
            var_setbool(v, node->u.b);
            break;
        case BE_CFUNCTION:
            var_setntvfunc(v, node->u.f);
            break;
        case BE_CSTRING:
            var_setstr(v, be_newstr(vm, node->u.s));
            break;
        case BE_CMODULE:
            native_module(vm, node->u.o, v);
            break;
        default: /* error */
            break;
        }
    }
}

static bmodule* new_module(bvm *vm, const bntvmodule *nm)
{
    bgcobject *gco = be_gcnew(vm, BE_MODULE, bmodule);
    bmodule *obj = cast_module(gco);
    if (obj) {
        var_setmodule(vm->top, obj);
        be_incrtop(vm);
        obj->info.native = nm;
        obj->table = NULL; /* gc protection */
        obj->table = be_map_new(vm);
        insert_attrs(vm, obj->table, nm);
        be_map_release(vm, obj->table); /* clear space */
        be_stackpop(vm, 1);
    }
    return obj;
}

static bmodule* native_module(bvm *vm, const bntvmodule *nm, bvalue *dst)
{
    if (nm) {
        bmodule *obj;
        if (nm->module) {
            obj = (bmodule *)nm->module;
        } else { /* new module */
            obj = new_module(vm, nm);
        }
        if (obj && dst) {
            var_setmodule(dst, obj);
        }
        return obj;
    }
    return NULL;
}

static char* fixpath(bvm *vm, bstring *path, size_t *size)
{
    char *buffer;
    const char *split, *base;
    bvalue *func = vm->cf->func;
    bclosure *cl = var_toobj(func);
    be_assert(var_isclosure(func));
    base = str(cl->proto->source); /* get the source file path */
    split = be_splitpath(base);
    *size = split - base + (size_t)str_len(path) + SUFFIX_LEN;
    buffer = be_malloc(vm, *size);
    strncpy(buffer, base, split - base);
    strcpy(buffer + (split - base), str(path));
    return buffer;
}

static char* conpath(bvm *vm, bstring *path1, bstring *path2, size_t *size)
{
    char *buffer;
    int len1 = str_len(path1);
    *size = (size_t)len1 + (size_t)str_len(path2) + 1 + SUFFIX_LEN;
    buffer = be_malloc(vm, *size);
    strcpy(buffer, str(path1));
    buffer[len1] = '/';
    strcpy(buffer + len1 + 1, str(path2));
    return buffer;
}

static int open_script(bvm *vm, char *path)
{
    int res = be_loadmodule(vm, path);
    if (res == BE_OK)
        be_call(vm, 0);
    return res;
}

#if BE_USE_SHARED_LIB
static int open_dllib(bvm *vm, char *path)
{
    int res = be_loadlib(vm, path);
    if (res == BE_OK)
        be_call(vm, 0);
    return res;
}
#endif

static int open_libfile(bvm *vm, char *path, size_t size)
{
    int res, idx = 0;
    const char *sfxs[] = { "", ".bec", ".be" };
    do {
        strcpy(path + size - SUFFIX_LEN, sfxs[idx]);
        res = open_script(vm, path);
    } while (idx++ < 2 && res == BE_IO_ERROR);
    if (res == BE_IO_ERROR) {
#if BE_USE_SHARED_LIB
        strcpy(path + size - SUFFIX_LEN, DLL_SUFFIX);
        res = open_dllib(vm, path);
#endif
    }
    be_free(vm, path, size);
    return res;
}

static int load_path(bvm *vm, bstring *path, bstring *mod)
{
    size_t size;
    char *fullpath = conpath(vm, path, mod, &size);
    return open_libfile(vm, fullpath, size);
}

static int load_cwd(bvm *vm, bstring *path)
{
    size_t size;
    char *fullpath = fixpath(vm, path, &size);
    return open_libfile(vm, fullpath, size);
}

static int load_package(bvm *vm, bstring *path)
{
    int res = load_cwd(vm, path); /* load from current directory */
    if (res == BE_IO_ERROR && vm->module.path) {
        blist *list = vm->module.path;
        bvalue *v = be_list_end(list) - 1;
        bvalue *first = be_list_data(list);
        for (; res == BE_IO_ERROR && v >= first; v--) {
            if (var_isstr(v)) {
                res = load_path(vm, var_tostr(v), path);
            }
        }
    }
    return res;
}

static int load_native(bvm *vm, bstring *path)
{
    const bntvmodule *nm = find_native(path);
    bmodule *mod = native_module(vm, nm, NULL);
    if (mod != NULL) {
        /* the pointer vm->top may be changed */
        var_setmodule(vm->top, mod);
        be_incrtop(vm);
        return BE_OK;
    }
    return BE_IO_ERROR;
}

static bvalue* load_cached(bvm *vm, bstring *path)
{
    bvalue *v = NULL;
    if (vm->module.loaded) {
        v = be_map_findstr(vm, vm->module.loaded, path);
        if (v) {
            *vm->top = *v;
            be_incrtop(vm);
        }
    }
    return v;
}

static void cache_module(bvm *vm, bstring *name)
{
    bvalue *v;
    if (vm->module.loaded == NULL) {
        vm->module.loaded = be_map_new(vm);
    }
    v = be_map_insertstr(vm, vm->module.loaded, name, NULL);
    *v = vm->top[-1];
}

/* load module to vm->top */
int be_module_load(bvm *vm, bstring *path)
{
    int res = BE_OK;
    if (!load_cached(vm, path)) {
        res = load_native(vm, path);
        if (res == BE_IO_ERROR)
            res = load_package(vm, path);
        if (res == BE_OK)
            cache_module(vm, path);
    }
    return res;
}

bmodule* be_module_new(bvm *vm)
{
    bgcobject *gco = be_gcnew(vm, BE_MODULE, bmodule);
    bmodule *obj = cast_module(gco);
    if (obj) {
        var_setmodule(vm->top, obj);
        be_incrtop(vm);
        obj->info.native = NULL;
        obj->table = NULL; /* gc protection */
        obj->table = be_map_new(vm);
        be_stackpop(vm, 1);
    }
    return obj;
}

void be_module_delete(bvm *vm, bmodule *module)
{
    be_free(vm, module, sizeof(bmodule));
}

bvalue* be_module_attr(bvm *vm, bmodule *module, bstring *attr)
{
    return be_map_findstr(vm, module->table, attr);
}

bvalue* be_module_bind(bvm *vm, bmodule *module, bstring *attr)
{
    bmap *attrs = module->table;
    if (!gc_isconst(attrs)) {
        bvalue *v = be_map_findstr(vm, attrs, attr);
        if (v == NULL) {
            v = be_map_insertstr(vm, attrs, attr, NULL);
        }
        return v;
    }
    return NULL;
}

const char* be_module_name(bmodule *module)
{
    if (gc_isconst(module)) {
        return module->info.name;
    }
    if (gc_exmark(module) & BE_MODULE_NAME) {
        return str(module->info.sname);
    }
    if (module->info.native) {
        return module->info.native->name;
    }
    return NULL;
}

bbool be_module_setname(bmodule *module, bstring *name)
{
    if (!gc_isconst(module)) {
        module->info.sname = name;
        gc_setexmark(module, BE_MODULE_NAME);
        return btrue;
    }
    return bfalse;
}

static blist* pathlist(bvm *vm)
{
    if (!vm->module.path) {
        vm->module.path = be_list_new(vm);
    }
    return vm->module.path;
}

/* push the path list to the top */
BERRY_API void be_module_path(bvm *vm)
{
    blist *list = pathlist(vm);
    bvalue *reg = be_incrtop(vm);
    var_setlist(reg, list);
}

BERRY_API void be_module_path_set(bvm *vm, const char *path)
{
    blist *list = pathlist(vm);
    bvalue *value = be_list_push(vm, list, NULL);
    var_setnil(value);
    var_setstr(value, be_newstr(vm, path))
}

/* shared library support */
#if BE_USE_SHARED_LIB

#if defined(__POSIX_OS__)
#include <dlfcn.h>

#if defined(__GNUC__)
  #define cast_func(f) (__extension__(bntvfunc)(f))
#else
  #define cast_func(f) ((bntvfunc)(f))
#endif

/* load shared library */
BERRY_API int be_loadlib(bvm *vm, const char *path)
{
    void *handle = dlopen(path, RTLD_LAZY);
    bntvfunc func = cast_func(dlsym(handle, "berry_export"));
    if (func == NULL) {
        return BE_IO_ERROR;
    }
    be_pushntvfunction(vm, func);
    return BE_OK;
}
#elif defined(_WIN32)
#include<wtypes.h>
#include <winbase.h>

BERRY_API int be_loadlib(bvm *vm, const char *path)
{
    HINSTANCE handle = LoadLibrary(path);
    if (handle) {
        union {
            FARPROC proc;
            bntvfunc func;
        } u;
        u.proc = GetProcAddress(handle, "berry_export");
        if (u.func != NULL) {
            be_pushntvfunction(vm, u.func);
            return BE_OK;
        }
    }
    return BE_IO_ERROR;
}
#else
BERRY_API int be_loadlib(bvm *vm, const char *path)
{
    (void)vm, (void)path;
    return BE_IO_ERROR;
}
#endif

#endif /* BE_USE_SHARED_LIB */
