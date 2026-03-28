"""
Berry list container.
Mirrors: src/be_list.c / src/be_list.h

The list is a dynamic array of bvalue elements with automatic growth.
It supports negative indexing, insertion, removal, merging, reversal,
and a simple free-list pool allocator used by the class system.
"""

# /********************************************************************
# ** Copyright (c) 2018-2020 Guan Wenliang
# ** This file is part of the Berry default interpreter.
# ** skiars@qq.com, https://github.com/Skiars/berry
# ** See Copyright Notice in the LICENSE file or at
# ** https://github.com/Skiars/berry/blob/master/LICENSE
# ********************************************************************/

from berry_port.be_object import (
    BE_LIST, blist, bvalue, cast_list,
    var_setlist, var_setnil, var_setint, var_toint, var_toidx,
)
from berry_port.be_mem import be_malloc, be_free, be_realloc
from berry_port.be_vector import be_nextsize


# ============================================================================
# Helpers
# ============================================================================

# #define datasize(size)          ((size) * sizeof(bvalue))
# In Python, bvalue objects live in a Python list, so datasize is used
# only for gc.usage tracking.  We define a nominal bvalue size for that.
_BVALUE_SIZE = 16  # approximate sizeof(bvalue) in C (8-byte union + 4-byte type, padded)

def _datasize(size):
    return size * _BVALUE_SIZE


# ============================================================================
# be_list_new
# ============================================================================

# blist* be_list_new(bvm *vm)
# {
#     bgcobject *gco = be_gcnew(vm, BE_LIST, blist);
#     blist *list = cast_list(gco);
#     if (list) {
#         list->count = 0;
#         list->capacity = 2;
#         var_setlist(vm->top, list);
#         be_incrtop(vm);
#         list->data = be_malloc(vm, datasize(list->capacity));
#         be_stackpop(vm, 1);
#     }
#     return list;
# }
def be_list_new(vm):
    """Create a new empty list with initial capacity 2.

    In C, the new list is temporarily pushed onto the VM stack to protect
    it from GC during the data allocation.  In Python we register it on
    the GC list directly.
    """
    lst = blist()
    lst.type = BE_LIST
    lst.count = 0
    lst.capacity = 2

    # Register with GC
    lst.next = vm.gc.list
    vm.gc.list = lst

    # Allocate backing store (tracked via gc.usage)
    lst.data = [bvalue() for _ in range(lst.capacity)]
    vm.gc.usage += _datasize(lst.capacity)

    return lst


# ============================================================================
# be_list_delete
# ============================================================================

# void be_list_delete(bvm *vm, blist *list)
# {
#     be_free(vm, list->data, datasize(list->capacity));
#     be_free(vm, list, sizeof(blist));
# }
def be_list_delete(vm, lst):
    """Free a list and its backing data array."""
    vm.gc.usage -= _datasize(lst.capacity)
    lst.data = None
    lst.count = 0
    lst.capacity = 0


# ============================================================================
# be_list_copy
# ============================================================================

# blist* be_list_copy(bvm *vm, blist *original)
# {
#     bgcobject *gco = be_gcnew(vm, BE_LIST, blist);
#     blist *list = cast_list(gco);
#     if (list) {
#         size_t size = datasize(original->capacity);
#         list->count = original->count;
#         list->capacity = original->capacity;
#         var_setlist(vm->top, list);
#         be_incrtop(vm);
#         list->data = be_malloc(vm, size);
#         be_stackpop(vm, 1);
#         memcpy(list->data, original->data, size);
#     }
#     return list;
# }
def be_list_copy(vm, original):
    """Create a shallow copy of *original*."""
    lst = blist()
    lst.type = BE_LIST
    lst.count = original.count
    lst.capacity = original.capacity

    # Register with GC
    lst.next = vm.gc.list
    vm.gc.list = lst

    # Copy backing store
    lst.data = [bvalue() for _ in range(lst.capacity)]
    for i in range(original.capacity):
        lst.data[i].type = original.data[i].type
        lst.data[i].v = original.data[i].v
    vm.gc.usage += _datasize(lst.capacity)

    return lst


# ============================================================================
# be_list_index
# ============================================================================

# bvalue* be_list_index(blist *list, int index)
# {
#     if (index < 0) {
#         index = list->count + index;
#     }
#     if (index < 0 || index >= list->count) {
#         return NULL;
#     }
#     return be_list_at(list, index);
# }
def be_list_index(lst, index):
    """Return the bvalue at *index* (supports negative indexing), or None."""
    if index < 0:
        index = lst.count + index
    if index < 0 or index >= lst.count:
        return None
    return lst.data[index]


# ============================================================================
# be_list_push
# ============================================================================

# bvalue* be_list_push(bvm *vm, blist *list, bvalue *value)
# {
#     bvalue *slot;
#     if (list->count >= list->capacity) {
#         int newcap = be_nextsize(list->capacity);
#         list->data = be_realloc(vm, list->data,
#             datasize(list->capacity), datasize(newcap));
#         list->capacity = newcap;
#     }
#     slot = list->data + list->count++;
#     if (value != NULL) {
#         *slot = *value;
#     }
#     return slot;
# }
def be_list_push(vm, lst, value):
    """Append *value* to the list, growing if needed. Returns the slot."""
    if lst.count >= lst.capacity:
        newcap = be_nextsize(lst.capacity)
        # Grow the Python list backing store
        old_cap = lst.capacity
        lst.data.extend(bvalue() for _ in range(newcap - old_cap))
        vm.gc.usage += _datasize(newcap) - _datasize(old_cap)
        lst.capacity = newcap

    slot = lst.data[lst.count]
    lst.count += 1
    if value is not None:
        slot.type = value.type
        slot.v = value.v
    return slot


# ============================================================================
# be_list_insert
# ============================================================================

# bvalue* be_list_insert(bvm *vm, blist *list, int index, bvalue *value)
# {
#     int i;
#     bvalue *data;
#     if (index < 0) {
#         index = list->count + index;
#     }
#     if (index < 0 || index > list->count) {
#         return NULL;
#     }
#     if (list->count >= list->capacity) {
#         int newcap = be_nextsize(list->capacity);
#         list->data = be_realloc(vm, list->data,
#             datasize(list->capacity), datasize(newcap));
#         list->capacity = newcap;
#     }
#     data = list->data;
#     for (i = list->count++; i > index; --i) {
#         data[i] = data[i - 1];
#     }
#     data = list->data + index;
#     if (value != NULL) {
#         *data = *value;
#     }
#     return data;
# }
def be_list_insert(vm, lst, index, value):
    """Insert *value* at *index*, shifting elements right. Returns the slot or None."""
    if index < 0:
        index = lst.count + index
    if index < 0 or index > lst.count:
        return None

    if lst.count >= lst.capacity:
        newcap = be_nextsize(lst.capacity)
        old_cap = lst.capacity
        lst.data.extend(bvalue() for _ in range(newcap - old_cap))
        vm.gc.usage += _datasize(newcap) - _datasize(old_cap)
        lst.capacity = newcap

    data = lst.data
    # Shift elements right (from end toward index)
    i = lst.count
    while i > index:
        data[i].type = data[i - 1].type
        data[i].v = data[i - 1].v
        i -= 1
    lst.count += 1

    slot = data[index]
    if value is not None:
        slot.type = value.type
        slot.v = value.v
    return slot


# ============================================================================
# be_list_remove
# ============================================================================

# int be_list_remove(bvm *vm, blist *list, int index)
# {
#     int i;
#     bvalue *data;
#     (void)vm;
#     if (index < 0) {
#         index = list->count + index;
#     }
#     if (index < 0 || index >= list->count) {
#         return bfalse;
#     }
#     data = list->data;
#     list->count--;
#     for (i = index; i < list->count; ++i) {
#         data[i] = data[i + 1];
#     }
#     return btrue;
# }
def be_list_remove(vm, lst, index):
    """Remove element at *index*, shifting elements left. Returns True/False."""
    _ = vm  # unused, matches C signature
    if index < 0:
        index = lst.count + index
    if index < 0 or index >= lst.count:
        return False

    data = lst.data
    lst.count -= 1
    for i in range(index, lst.count):
        data[i].type = data[i + 1].type
        data[i].v = data[i + 1].v
    return True


# ============================================================================
# be_list_resize
# ============================================================================

# void be_list_resize(bvm *vm, blist *list, int count)
# {
#     if (count != list->count) {
#         int newcap = be_nextsize(count);
#         if (newcap > list->capacity) {
#             bvalue *v, *end;
#             list->data = be_realloc(vm, list->data,
#                 datasize(list->capacity), datasize(newcap));
#             list->capacity = newcap;
#             v = list->data + list->count;
#             end = list->data + count;
#             while (v < end) {
#                 var_setnil(v++);
#             }
#         }
#         list->count = count;
#     }
# }
def be_list_resize(vm, lst, count):
    """Resize the list to *count* elements, growing capacity if needed.

    New elements (if count > old count) are initialized to nil.
    """
    if count != lst.count:
        newcap = be_nextsize(count)
        if newcap > lst.capacity:
            old_cap = lst.capacity
            lst.data.extend(bvalue() for _ in range(newcap - old_cap))
            vm.gc.usage += _datasize(newcap) - _datasize(old_cap)
            lst.capacity = newcap
            # Initialize new slots to nil
            for i in range(lst.count, count):
                var_setnil(lst.data[i])
        lst.count = count


# ============================================================================
# be_list_merge
# ============================================================================

# void be_list_merge(bvm *vm, blist *list, const blist *other)
# {
#     int dst_len = list->count;
#     int src_len = other->count;
#     int length = src_len + dst_len;
#     if (length != 0) {
#         int newcap = be_nextsize(length);
#         if (newcap > list->capacity) {
#             list->data = be_realloc(vm, list->data,
#                 datasize(list->capacity), datasize(newcap));
#             list->capacity = newcap;
#         }
#         memcpy(list->data + dst_len, other->data, src_len * sizeof(bvalue));
#         list->count = length;
#     }
# }
def be_list_merge(vm, lst, other):
    """Append all elements of *other* to *lst*."""
    dst_len = lst.count
    src_len = other.count
    length = src_len + dst_len
    if length != 0:
        newcap = be_nextsize(length)
        if newcap > lst.capacity:
            old_cap = lst.capacity
            lst.data.extend(bvalue() for _ in range(newcap - old_cap))
            vm.gc.usage += _datasize(newcap) - _datasize(old_cap)
            lst.capacity = newcap
        # Copy values from other
        for i in range(src_len):
            lst.data[dst_len + i].type = other.data[i].type
            lst.data[dst_len + i].v = other.data[i].v
        lst.count = length


# ============================================================================
# be_list_reverse
# ============================================================================

# void be_list_reverse(blist *list)
# {
#     bvalue *left = list->data;
#     bvalue *right = left + list->count - 1;
#     for (; left < right; ++left, --right) {
#         bvalue temp = *left;
#         *left = *right;
#         *right = temp;
#     }
# }
def be_list_reverse(lst):
    """Reverse the list in place."""
    data = lst.data
    left = 0
    right = lst.count - 1
    while left < right:
        # Swap type and v fields
        lt, lv = data[left].type, data[left].v
        data[left].type, data[left].v = data[right].type, data[right].v
        data[right].type, data[right].v = lt, lv
        left += 1
        right -= 1


# ============================================================================
# Pool allocator (free-list over a blist)
# ============================================================================

# void be_list_pool_init(bvm *vm, blist *list)
# {
#     bvalue *head;
#     be_list_resize(vm, list, 0);
#     head = be_list_push(vm, list, NULL);
#     var_setint(head, 0);
# }
def be_list_pool_init(vm, lst):
    """Initialize *lst* as a pool allocator.

    Element 0 is the free-list head; its integer value points to the
    first free slot (0 means no free slots).
    """
    be_list_resize(vm, lst, 0)
    head = be_list_push(vm, lst, None)
    var_setint(head, 0)


# int be_list_pool_alloc(bvm *vm, blist *list, bvalue *src)
# {
#     bvalue *head = be_list_data(list), *node;
#     int id = var_toidx(head); /* get the first free node */
#     if (id) {
#         node = head + id;
#         head->v.i = var_toint(node); /* link the next free node to head */
#     } else {
#         id = be_list_count(list);
#         node = be_list_push(vm, list, NULL);
#     }
#     *node = *src;
#     return id;
# }
def be_list_pool_alloc(vm, lst, src):
    """Allocate a slot from the pool, storing *src* in it. Returns the slot id."""
    head = lst.data[0]
    id_ = var_toidx(head)  # first free node index
    if id_:
        node = lst.data[id_]
        head.v = var_toint(node)  # link next free node to head
    else:
        id_ = lst.count
        node = be_list_push(vm, lst, None)
    # Copy src into node
    node.type = src.type
    node.v = src.v
    return id_


# void be_list_pool_free(blist *list, int id)
# {
#     bvalue *head = be_list_data(list);
#     bvalue *node = head + id;
#     be_assert(id > 0 && id < list->count);
#     /* insert a new free node to head */
#     *node = *head;
#     head->v.i = id;
# }
def be_list_pool_free(lst, id_):
    """Return slot *id_* to the pool's free list."""
    assert 0 < id_ < lst.count, f"pool_free: id {id_} out of range [1, {lst.count})"
    head = lst.data[0]
    node = lst.data[id_]
    # Copy head into node (preserves the free-chain link)
    node.type = head.type
    node.v = head.v
    # Point head at the newly freed slot
    head.v = id_
