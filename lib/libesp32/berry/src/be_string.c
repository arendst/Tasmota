/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_string.h"
#include "be_vm.h"
#include "be_mem.h"
#include "be_constobj.h"
#include <string.h>

#define next(_s)    cast(void*, cast(bstring*, (_s)->next))
#define sstr(_s)    cast(char*, cast(bsstring*, _s) + 1)
#define lstr(_s)    cast(char*, cast(blstring*, _s) + 1)
#define cstr(_s)    (cast(bcstring*, _s)->s)

#define be_define_const_str(_name, _s, _hash, _extra, _len, _next) \
    BERRY_LOCAL const bcstring be_const_str_##_name = {            \
        .next = (bgcobject *)_next,                                \
        .type = BE_STRING,                                         \
        .marked = GC_CONST,                                        \
        .extra = _extra,                                           \
        .slen = _len,                                              \
        .hash = _hash,                                             \
        .s = _s                                                    \
    }

/* const string table */
struct bconststrtab {
    const bstring* const *table;
    int count; /* string count */
    int size;
};

#if BE_USE_PRECOMPILED_OBJECT
#include "../generate/be_const_strtab_def.h"
#endif

int be_eqstr(bstring *s1, bstring *s2)
{
    int slen;
    if (s1 == s2) { /* short string or the same string */
        return 1;
    }
    slen = s1->slen;
    /* discard different lengths */
    if (slen != s2->slen) {
        return 0;
    }
    /* long string */
    if (slen == 255) {  /* s2->slen is also 255 */
        blstring *ls1 = cast(blstring*, s1);
        blstring *ls2 = cast(blstring*, s2);
        return ls1->llen == ls2->llen && !strcmp(lstr(ls1), lstr(ls2));
    }
    /* const short strings */
    if (gc_isconst(s1) || gc_isconst(s2)) { /* one of the two string is short const */
        uint32_t hash1 = cast(bcstring*, s1)->hash;
        uint32_t hash2 = cast(bcstring*, s2)->hash;
        if (hash1 && hash2 && hash1 != hash2) {
            return 0; /* if hash differ, since we know both are non-null */
        }
        /* if hash are equals, there might be a chance that they are different */
        /* This can happen with solidified code that a same string is present more than once */
        /* so just considering that two strings with the same hash must be same pointer, this is no more true */
        return !strcmp(str(s1), str(s2));
    }

    /* if both strings are in-memory, they can't be equal without having the same pointer */
    return 0;
}

static void resize(bvm *vm, int size)
{
    int i;
    struct bstringtable *tab = &vm->strtab;
    if (size > tab->size) {
        tab->table = be_realloc(vm, tab->table,
            tab->size * sizeof(bstring*), size * sizeof(bstring*));
        for (i = tab->size; i < size; ++i) {
            tab->table[i] = NULL;
        }
    }
    for (i = 0; i < tab->size; ++i) { /* rehash */
        bstring *p = tab->table[i];
        tab->table[i] = NULL;
        while (p) { /* for each node in the list */
            bstring *hnext = next(p);
            uint32_t hash = be_strhash(p) & (size - 1);
            p->next = cast(void*, tab->table[hash]);
            tab->table[hash] = p;
            p = hnext;
        }
    }
    if (size < tab->size) {
        for (i = size; i < tab->size; ++i) {
            tab->table[i] = NULL;
        }
        tab->table = be_realloc(vm, tab->table,
            tab->size * sizeof(bstring*), size * sizeof(bstring*));
    }
    tab->size = size;
}

static void free_sstring(bvm *vm, bstring *str)
{
    be_free(vm, str, sizeof(bsstring) + str->slen + 1);
}

/* FNV-1a Hash */
static uint32_t str_hash(const char *str, size_t len)
{
    uint32_t hash = 2166136261u;
    be_assert(str || !len);
    while (len--) {
        hash = (hash ^ (unsigned char)*str++) * 16777619u;
    }
    return hash;
}

void be_string_init(bvm *vm)
{
    resize(vm, 8);
#if !BE_USE_PRECOMPILED_OBJECT
    /* the destructor name deinit needs to exist all the time, to ensure
     * that it does not need to be created when the heap is exhausted. */
    be_gc_fix(vm, cast(bgcobject*, str_literal(vm, "deinit")));
#endif
    /* be_const_str_deinit --> for precompiled */
}

void be_string_deleteall(bvm *vm)
{
    int i;
    struct bstringtable *tab = &vm->strtab;
    for (i = 0; i < tab->size; ++i) {
        bstring *node = tab->table[i];
        while (node) {
            bstring *next = next(node);
            free_sstring(vm, node);
            node = next;
        }
    }
    be_free(vm, tab->table, tab->size * sizeof(bstring*));
}

static bstring* createstrobj(bvm *vm, size_t len, int islong)
{
    size_t size = (islong ? sizeof(blstring)
                : sizeof(bsstring)) + len + 1;
    bgcobject *gco = be_gc_newstr(vm, size, islong);
    bstring *s = cast_str(gco);
    if (s) {
        s->slen = islong ? 255 : (bbyte)len;
        char *str = cast(char *, islong ? lstr(s) : sstr(s));
        str[len] = '\0';
    }
    return s;
}

#if BE_USE_PRECOMPILED_OBJECT
static bstring* find_conststr(const char *str, size_t len)
{
    const struct bconststrtab *tab = &m_const_string_table;
    uint32_t hash = str_hash(str, len);
    bcstring *s = (bcstring*)tab->table[hash % tab->size];
    for (; s != NULL; s = next(s)) {
        if (len == 0 && s->slen == 0) {
            /* special case for the empty string,
               since we don't want to compare it using strncmp, 
               because str might be NULL */
            return (bstring*)s;
        }
        if (len == s->slen && !strncmp(str, s->s, len)) {
            return (bstring*)s;
        }
    }
    return NULL;
}
#endif

static bstring* newshortstr(bvm *vm, const char *str, size_t len)
{
    bstring *s;
    int size = vm->strtab.size;
    uint32_t hash = str_hash(str, len);
    bstring **list = vm->strtab.table + (hash & (size - 1));

    for (s = *list; s != NULL; s = next(s)) {
        if (len == s->slen && !strncmp(str, sstr(s), len)) {
            return s;
        }
    }
    s = createstrobj(vm, len, 0);
    if (s) {
        /* recompute size and list that may have changed due to a GC */
        size = vm->strtab.size;
        list = vm->strtab.table + (hash & (size - 1));
        memcpy(cast(char *, sstr(s)), str, len);
        s->extra = 0;
        s->next = cast(void*, *list);
#if BE_USE_STR_HASH_CACHE
        cast(bsstring*, s)->hash = hash;
#endif
        *list = s;
        vm->strtab.count++;
        if (vm->strtab.count > size << 2) {
            resize(vm, size << 1);
        }
    }
    return s;
}

bstring* be_newlongstr(bvm *vm, const char *str, size_t len)
{
    bstring *s;
    blstring *ls;
    s = createstrobj(vm, len, 1);
    ls = cast(blstring*, s);
    s->extra = 0;
    ls->llen = cast_int(len);
    if (str) { /* if the argument 'str' is NULL, we just allocate space */
        memcpy(cast(char *, lstr(s)), str, len);
    }
    return s;
}

bstring* be_newstr(bvm *vm, const char *str)
{
    return be_newstrn(vm, str, strlen(str));
}

bstring *be_newstrn(bvm *vm, const char *str, size_t len)
{
    if (len <= SHORT_STR_MAX_LEN) {
#if BE_USE_PRECOMPILED_OBJECT
        bstring *s = find_conststr(str, len);
        return s ? s : newshortstr(vm, str, len);
#else
        return newshortstr(vm, str, len);
#endif
    }
    return be_newlongstr(vm, str, len); /* long string */
}

void be_gcstrtab(bvm *vm)
{
    struct bstringtable *tab = &vm->strtab;
    int size = tab->size, i;
    for (i = 0; i < size; ++i) {
        bstring **list = tab->table + i;
        bstring *prev = NULL, *node, *next;
        for (node = *list; node; node = next) {
            next = next(node);
            if (!gc_isfixed(node) && gc_iswhite(node)) {
                free_sstring(vm, node);
                tab->count--;
                if (prev) { /* link list */
                    prev->next = cast(void*, next);
                } else {
                    *list = next;
                }
            } else {
                prev = node;
                gc_setwhite(node);
            }
        }
    }
    if (BE_USE_DEBUG_GC || comp_is_gc_debug(vm)) {
        resize(vm, tab->count + 4);
    } else {
        if (tab->count < size >> 2 && size > 8) {
            resize(vm, size >> 1);
        }
    }
}

uint32_t be_strhash(const bstring *s)
{
    if (gc_isconst(s)) {
        bcstring* cs = cast(bcstring*, s);
        if (cs->hash) {  /* if hash is null we need to compute it */
            return cs->hash;
        } else {
            return str_hash(cstr(s), str_len(s));
        }
    }
#if BE_USE_STR_HASH_CACHE
    if (s->slen != 255) {
        return cast(bsstring*, s)->hash;
    }
#endif
    return str_hash(str(s), str_len(s));
}

const char* be_str2cstr(const bstring *s)
{
    be_assert(cast_str(s) != NULL);
    if (gc_isconst(s)) {
        return cstr(s);
    }
    if (s->slen == 255) {
        return lstr(s);
    }
    return sstr(s);
}

void be_str_setextra(bstring *s, int extra)
{
    if (!gc_isconst(s)) {
        s->extra = cast(bbyte, extra);
    }
}
