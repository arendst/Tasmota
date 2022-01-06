/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef BE_VECTOR_H
#define BE_VECTOR_H

#include "be_object.h"

/* =============================== defines =============================== */
#define be_vector_data(vector)          ((vector)->data)
#define be_vector_first(vector)         ((vector)->data)
#define be_vector_isend(vector, item)   ((item) > (vector)->end)
#define be_vector_isempty(vector)       (!(vector)->count)
#define be_vector_end(vector)           ((vector)->end)
#define be_vector_count(vector)         ((vector)->count)
#define be_vector_capacity(vector)      ((vector)->capacity)
#define be_stack_init(vm, stack, size)  be_vector_init(vm, stack, size)
#define be_stack_delete(vm, stack)      be_vector_delete(vm, stack)
#define be_stack_clear(stack)           be_vector_clear(stack)
#define be_stack_push(vm, stack, data)  be_vector_push(vm, stack, data)
#define be_stack_pop(stack)             be_vector_remove_end(stack)
#define be_stack_top(stack)             be_vector_end(stack)
#define be_stack_base(stack)            be_vector_first(stack)
#define be_stack_count(stack)           be_vector_count(stack)
#define be_stack_isempty(stack)         be_vector_isempty(stack)

/* ========================== function extern ========================== */
void be_vector_init(bvm *vm, bvector *vector, int size);
void be_vector_delete(bvm *vm, bvector *vector);
void* be_vector_at(bvector *vector, int index);
void be_vector_push(bvm *vm, bvector *vector, void *data);
void be_vector_push_c(bvm *vm, bvector *vector, void *data);
void be_vector_remove_end(bvector *vector);
void be_vector_resize(bvm *vm, bvector *vector, int count);
void be_vector_clear(bvector *vector);
void* be_vector_release(bvm *vm, bvector *vector);
void* be_vector_release_32(bvm *vm, bvector *vector);   /* specialized call for 32 bits aligned accesses */
int be_nextsize(int value);

#endif
