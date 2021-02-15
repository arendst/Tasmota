/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef BE_LIST_H
#define BE_LIST_H

#include "be_object.h"

struct blist {
    bcommon_header;
    bgcobject *gray; /* for gc gray list */
    int count, capacity;
    bvalue *data;
};

#define be_list_data(list)          ((list)->data)
#define be_list_count(list)         ((list)->count)
#define be_list_at(list, index)     ((list)->data + index)
#define be_list_end(list)           ((list)->data + (list)->count)

blist* be_list_new(bvm *vm);
void be_list_delete(bvm *vm, blist *list);
blist* be_list_copy(bvm *vm, blist *original);
bvalue* be_list_index(blist *list, int index);
bvalue* be_list_push(bvm *vm, blist *list, bvalue *value);
bvalue* be_list_insert(bvm *vm, blist *list, int index, bvalue *value);
int be_list_remove(bvm *vm, blist *list, int index);
void be_list_resize(bvm *vm, blist *list, int count);
void be_list_merge(bvm *vm, blist *list, const blist *other);
void be_list_reverse(blist *list);
void be_list_pool_init(bvm *vm, blist *list);
int be_list_pool_alloc(bvm *vm, blist *list, bvalue *src);
void be_list_pool_free(blist *list, int id);

#endif
