/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef BE_MAP_H
#define BE_MAP_H

#include "be_object.h"

typedef struct bmapkey {
    union bvaldata v;
    uint32_t type:8;
    uint32_t next:24;
} bmapkey;

typedef struct bmapnode {
    bmapkey key;
    bvalue value;
} bmapnode;

struct bmap {
    bcommon_header;
    bgcobject *gray; /* for gc gray list */
    bmapnode *slots;
    bmapnode *lastfree;
    int size;
    int count;
#ifdef __cplusplus
    BE_CONSTEXPR bmap(bmapnode *s, int n) :
        next(0), type(BE_MAP), marked(GC_CONST), gray(0),
        slots(s), lastfree(0), size(n), count(n) {}
#endif
};

typedef bmapnode *bmapiter;

#define be_map_iter()       NULL
#define be_map_count(map)   ((map)->count)
#define be_map_size(map)    (map->size)

#define be_map_key2value(dst, node) do { \
    (dst)->type = (node)->key.type;      \
    (dst)->v = (node)->key.v;            \
} while (0);

bmap* be_map_new(bvm *vm);
void be_map_delete(bvm *vm, bmap *map);
bvalue* be_map_find(bvm *vm, bmap *map, bvalue *key);
bvalue* be_map_insert(bvm *vm, bmap *map, bvalue *key, bvalue *value);
int be_map_remove(bvm *vm, bmap *map, bvalue *key);
bvalue* be_map_findstr(bvm *vm, bmap *map, bstring *key);
bvalue* be_map_insertstr(bvm *vm, bmap *map, bstring *key, bvalue *value);
void be_map_removestr(bvm *vm, bmap *map, bstring *key);
bmapnode* be_map_next(bmap *map, bmapiter *iter);
bmapnode* be_map_val2node(bvalue *value);
void be_map_compact(bvm *vm, bmap *map);

#endif
