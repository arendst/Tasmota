"""
Berry map (hash table) container.
Mirrors: src/be_map.c / src/be_map.h

The map uses open addressing with chaining via a `key.next` field in each
bmapnode. The `lastfree` index scans backward to find free slots. Hash
collisions are resolved by displacing non-main-slot entries to free slots.

Original C code is included as comments for each function.
"""

# /********************************************************************
# ** Copyright (c) 2018-2020 Guan Wenliang
# ** This file is part of the Berry default interpreter.
# ** skiars@qq.com, https://github.com/Skiars/berry
# ** See Copyright Notice in the LICENSE file or at
# ** https://github.com/Skiars/berry/blob/master/LICENSE
# ********************************************************************/

from berry_port.be_object import (
    BE_MAP, BE_NIL, BE_BOOL, BE_INT, BE_REAL, BE_STRING, BE_INSTANCE,
    GC_CONST,
    bmap, bmapnode, bmapkey, bvalue,
    var_isnil, var_tobool, var_toint, var_toreal, var_tostr, var_toobj,
    var_setstr, var_setmap,
    gc_isconst,
)
from berry_port.be_string import be_strhash, be_eqstr
from berry_port.be_vector import be_nextsize


# ============================================================================
# Constants
# ============================================================================

# #define LASTNODE            ((1 << 24) - 1)
LASTNODE = (1 << 24) - 1

# Approximate sizeof(bmapnode) in C for gc.usage tracking
# bmapkey = 12 bytes (8 union + 4 packed), bvalue = 12 bytes (8 union + 4 type)
_BMAPNODE_SIZE = 24


# ============================================================================
# Internal macros (from be_map.c)
# ============================================================================

# #define key(node)           (&(node)->key)
# #define value(node)         (&(node)->value)
# #define isnil(node)         var_isnil(key(node))
# #define setnil(node)        var_setnil(key(node))
# #define keytype(key)        ((signed char)(key)->type)
# #define next(node)          ((node)->key.next)
# #define setkey(node, _v)    { (node)->key.type = (bbyte)(_v)->type; \
#                               (node)->key.v = (_v)->v; }
# #define datasize(size)      ((size) * sizeof(bmapnode))

def _isnil(node):
    """Check if a map node's key is nil."""
    return node.key.type == BE_NIL

def _setnil(node):
    """Set a map node's key to nil."""
    node.key.type = BE_NIL
    node.key.v = None

def _setkey(node, v):
    """Copy a bvalue's type and value into a node's key."""
    node.key.type = v.type & 0xFF
    node.key.v = v.v

def _datasize(size):
    """Approximate C memory size for gc.usage tracking."""
    return size * _BMAPNODE_SIZE


# ============================================================================
# map_nextsize (from be_map.c)
# ============================================================================

# static int map_nextsize(int size)
# {
#     be_assert(size < LASTNODE);
#     if (size < LASTNODE) {
#         return be_nextsize(size);
#     }
#     return LASTNODE + 1;
# }
def _map_nextsize(size):
    """Compute the next map capacity, clamped to LASTNODE+1."""
    if size < LASTNODE:
        return be_nextsize(size)
    return LASTNODE + 1


# ============================================================================
# Hash functions (from be_map.c)
# ============================================================================

# static uint32_t hashptr(void *p)
# {
#     uintptr_t i = (uintptr_t)p;
#     return (uint32_t)((i ^ (i >> 16)) & 0xFFFFFFFF);
# }
def _hashptr(p):
    """Hash a pointer (Python object id)."""
    i = id(p) if p is not None else 0
    return (i ^ (i >> 16)) & 0xFFFFFFFF


# #if BE_USE_SINGLE_FLOAT
# #define hashreal(v)     ((uint32_t)((v).i))
# #else
# static uint32_t hashreal(union bvaldata v)
# {
#     union { breal r; uint32_t i[2]; } u;
#     u.r = v.r;
#     return u.i[0] ^ u.i[1];
# }
# #endif
def _hashreal(r):
    """Hash a real (double) value by XORing its two 32-bit halves."""
    import struct
    try:
        b = struct.pack('d', r)
        lo = int.from_bytes(b[0:4], 'little')
        hi = int.from_bytes(b[4:8], 'little')
        return (lo ^ hi) & 0xFFFFFFFF
    except (struct.error, OverflowError):
        return 0


# static uint32_t _hashcode(bvm *vm, int type, union bvaldata v)
# {
#     (void)vm;
#     switch (type) {
#     case BE_NIL: return 0;
#     case BE_BOOL: return (uint32_t)v.b;
#     case BE_INT: return (uint32_t)v.i;
#     case BE_REAL: return hashreal(v);
#     case BE_STRING: return be_strhash(v.s);
#     default: return hashptr(v.p);
#     }
# }
def _hashcode(vm, type_, v):
    """Compute the hash code for a value given its type and data.

    Mirrors the C _hashcode function. The vm parameter is unused in the
    base implementation (needed for BE_USE_OVERLOAD_HASH).
    """
    if type_ == BE_NIL:
        return 0
    elif type_ == BE_BOOL:
        return 1 if v else 0
    elif type_ == BE_INT:
        return v & 0xFFFFFFFF
    elif type_ == BE_REAL:
        return _hashreal(v)
    elif type_ == BE_STRING:
        return be_strhash(v)
    else:
        return _hashptr(v)


def _hashcode_bvalue(vm, bv):
    """Convenience: compute hashcode from a bvalue."""
    return _hashcode(vm, bv.type, bv.v)


def _hashcode_key(vm, k):
    """Convenience: compute hashcode from a bmapkey."""
    return _hashcode(vm, k.type, k.v)


# ============================================================================
# eqnode (from be_map.c)
# ============================================================================

# static int eqnode(bvm *vm, bmapnode *node, bvalue *key, uint32_t hash)
# {
#     (void)vm;
#     if (!var_isnil(key)) {
#         bmapkey *k = key(node);
#         if(keytype(k) == key->type && hashcode(k) == hash) {
#             switch (key->type) {
#             case BE_BOOL: return var_tobool(key) == var_tobool(k);
#             case BE_INT: return var_toint(key) == var_toint(k);
#             case BE_REAL: return var_toreal(key) == var_toreal(k);
#             case BE_STRING: return be_eqstr(var_tostr(key), var_tostr(k));
#             default: return var_toobj(key) == var_toobj(k);
#             }
#         }
#     }
#     return 0;
# }
def _eqnode(vm, node, key, hash_val):
    """Check if a map node's key equals the given bvalue key."""
    if var_isnil(key):
        return 0
    k = node.key
    # keytype is signed char cast of k.type
    kt = k.type if k.type < 128 else k.type - 256
    if kt == key.type and _hashcode_key(vm, k) == hash_val:
        t = key.type
        if t == BE_BOOL:
            return 1 if (key.v == k.v) else 0
        elif t == BE_INT:
            return 1 if (key.v == k.v) else 0
        elif t == BE_REAL:
            return 1 if (key.v == k.v) else 0
        elif t == BE_STRING:
            return be_eqstr(key.v, k.v)
        else:
            return 1 if (key.v is k.v) else 0
    return 0


# ============================================================================
# findprev (from be_map.c)
# ============================================================================

# static bmapnode* findprev(bmap *map, bmapnode *list, bmapnode *slot)
# {
#     int n, pos = pos(map, slot);
#     bmapnode *prev = list;
#     for (;;) {
#         n = next(prev);
#         if (n == pos || n == LASTNODE) {
#             break;
#         }
#         prev = map->slots + n;
#     }
#     return n == pos ? prev : NULL;
# }
def _findprev(map_, list_node, slot_idx):
    """Find the node in the chain whose next points to slot_idx.

    Returns the node, or None if not found.
    """
    prev = list_node
    while True:
        n = prev.key.next
        if n == slot_idx or n == LASTNODE:
            break
        prev = map_.slots[n]
    return prev if n == slot_idx else None


# ============================================================================
# nextfree (from be_map.c)
# ============================================================================

# static bmapnode* nextfree(bmap *map)
# {
#     bmapnode *base = map->slots;
#     while (map->lastfree >= base) {
#         if (isnil(map->lastfree)) {
#             return map->lastfree;
#         }
#         --map->lastfree;
#     }
#     return NULL;
# }
def _nextfree(map_):
    """Find the next free slot by scanning backward from lastfree.

    Returns the index of the free slot, or -1 if none found.
    In C, lastfree is a pointer; here it's an index into map_.slots.
    """
    while map_.lastfree >= 0:
        if _isnil(map_.slots[map_.lastfree]):
            return map_.lastfree
        map_.lastfree -= 1
    return -1


# ============================================================================
# insert (from be_map.c)
# ============================================================================

# static bmapnode* insert(bvm *vm, bmap *map, bvalue *key, uint32_t hash)
# {
#     bmapnode *slot = hash2slot(map, hash);
#     if (isnil(slot)) { /* empty slot */
#         setkey(slot, key);
#         next(slot) = LASTNODE;
#     } else {
#         uint32_t h = hashcode(key(slot));
#         bmapnode *mainslot = hash2slot(map, h);
#         bmapnode *new = nextfree(map);
#         if (mainslot == slot) { /* old is main slot */
#             /* insert in first */
#             setkey(new, key);
#             next(new) = next(slot);
#             next(slot) = pos(map, new);
#             slot = new;
#         } else {
#             bmapnode *prev = findprev(map, mainslot, slot);
#             next(prev) = pos(map, new);
#             *new = *slot;
#             setkey(slot, key);
#             next(slot) = LASTNODE;
#         }
#     }
#     return slot;
# }
def _insert(vm, map_, key, hash_val):
    """Insert a key into the map, returning the node for the new entry.

    Handles collision resolution by displacing non-main-slot entries.
    """
    slot_idx = hash_val % map_.size
    slot = map_.slots[slot_idx]

    if _isnil(slot):
        # Empty slot — just place the key here
        _setkey(slot, key)
        slot.key.next = LASTNODE
    else:
        # Slot is occupied — check if occupant belongs here
        h = _hashcode_key(vm, slot.key)
        mainslot_idx = h % map_.size
        free_idx = _nextfree(map_)
        free_node = map_.slots[free_idx]

        if mainslot_idx == slot_idx:
            # Existing node IS in its main slot — chain the new key after it
            _setkey(free_node, key)
            free_node.key.next = slot.key.next
            free_node.value = bvalue()  # reset value
            slot.key.next = free_idx
            slot = free_node
        else:
            # Existing node is NOT in its main slot — displace it
            mainslot = map_.slots[mainslot_idx]
            prev = _findprev(map_, mainslot, slot_idx)
            prev.key.next = free_idx
            # Copy displaced node to free slot
            free_node.key.type = slot.key.type
            free_node.key.v = slot.key.v
            free_node.key.next = slot.key.next
            free_node.value.type = slot.value.type
            free_node.value.v = slot.value.v
            # Place new key in the now-freed main slot
            _setkey(slot, key)
            slot.key.next = LASTNODE
            slot.value = bvalue()  # reset value

    return slot


# ============================================================================
# find (from be_map.c)
# ============================================================================

# static bmapnode* find(bvm *vm, bmap *map, bvalue *key, uint32_t hash)
# {
#     if (map->size == 0) {
#         return NULL;
#     }
#     bmapnode *slot = hash2slot(map, hash);
#     if (isnil(slot)) {
#         return NULL;
#     }
#     while (!eqnode(vm, slot, key, hash)) {
#         int n = next(slot);
#         if (n == LASTNODE) {
#             return NULL;
#         }
#         slot = map->slots + n;
#     }
#     return slot;
# }
def _find(vm, map_, key, hash_val):
    """Find a node matching key in the map. Returns the node or None."""
    if map_.size == 0:
        return None
    slot_idx = hash_val % map_.size
    slot = map_.slots[slot_idx]
    if _isnil(slot):
        return None
    while not _eqnode(vm, slot, key, hash_val):
        n = slot.key.next
        if n == LASTNODE:
            return None
        slot = map_.slots[n]
    return slot


# ============================================================================
# resize (from be_map.c)
# ============================================================================

# static void resize(bvm *vm, bmap *map, int size)
# {
#     int i, oldsize;
#     bmapnode *slots, *oldslots;
#     if (size < map->count) {
#         return;
#     }
#     oldsize = map->size;
#     oldslots = map->slots;
#     slots = be_malloc(vm, datasize(size));
#     for (i = 0; i < size; ++i) {
#         setnil(slots + i);
#         next(slots + i) = LASTNODE;
#     }
#     map->size = size;
#     map->slots = slots;
#     map->lastfree = slots + size - 1;
#     /* rehash */
#     for (i = 0; i < oldsize; ++i) {
#         bmapnode *node = oldslots + i;
#         if (!isnil(node)) {
#             bvalue v;
#             bmapnode *newslot;
#             v.type = node->key.type;
#             v.v = node->key.v;
#             newslot = insert(vm, map, &v, hashcode(&v));
#             newslot->value = node->value;
#         }
#     }
#     be_free(vm, oldslots, datasize(oldsize));
# }
def _resize(vm, map_, size):
    """Resize the map's slot array and rehash all entries."""
    if size < map_.count:
        return

    oldsize = map_.size
    oldslots = map_.slots

    # Allocate new slots
    slots = [bmapnode() for _ in range(size)]
    for i in range(size):
        _setnil(slots[i])
        slots[i].key.next = LASTNODE
    vm.gc.usage += _datasize(size)

    map_.size = size
    map_.slots = slots
    map_.lastfree = size - 1

    # Rehash existing entries
    if oldslots is not None:
        for i in range(oldsize):
            node = oldslots[i]
            if not _isnil(node):
                v = bvalue()
                v.type = node.key.type
                v.v = node.key.v
                newslot = _insert(vm, map_, v, _hashcode_bvalue(vm, v))
                newslot.value.type = node.value.type
                newslot.value.v = node.value.v
        # Free old slots
        vm.gc.usage -= _datasize(oldsize)


# ============================================================================
# be_map_new (from be_map.c)
# ============================================================================

# bmap* be_map_new(bvm *vm)
# {
#     bgcobject *gco = be_gcnew(vm, BE_MAP, bmap);
#     bmap *map = cast_map(gco);
#     if (map) {
#         map->size = 0;
#         map->count = 0;
#         map->slots = NULL;
#         var_setmap(vm->top, map);
#         be_incrtop(vm);
#         resize(vm, map, 2);
#         be_stackpop(vm, 1);
#     }
#     return map;
# }
def be_map_new(vm):
    """Create a new empty map with initial capacity 2.

    In C, the new map is temporarily pushed onto the VM stack to protect
    it from GC during the resize. In Python we register it on the GC
    list directly.
    """
    map_ = bmap()
    map_.type = BE_MAP
    map_.size = 0
    map_.count = 0
    map_.slots = None

    _resize(vm, map_, 2)
    return map_


# ============================================================================
# be_map_delete (from be_map.c)
# ============================================================================

# void be_map_delete(bvm *vm, bmap *map)
# {
#     be_free(vm, map->slots, datasize(map->size));
#     be_free(vm, map, sizeof(bmap));
# }
def be_map_delete(vm, map_):
    """Free a map and its slot array."""
    vm.gc.usage -= _datasize(map_.size)
    map_.slots = None
    map_.size = 0
    map_.count = 0


# ============================================================================
# be_map_find (from be_map.c)
# ============================================================================

# bvalue* be_map_find(bvm *vm, bmap *map, bvalue *key)
# {
#     bmapnode *entry = find(vm, map, key, hashcode(key));
#     return entry ? value(entry) : NULL;
# }
def be_map_find(vm, map_, key):
    """Find a value by key. Returns the bvalue or None."""
    entry = _find(vm, map_, key, _hashcode_bvalue(vm, key))
    return entry.value if entry is not None else None


# ============================================================================
# be_map_insert (from be_map.c)
# ============================================================================

# bvalue* be_map_insert(bvm *vm, bmap *map, bvalue *key, bvalue *value)
# {
#     uint32_t hash = hashcode(key);
#     bmapnode *entry = find(vm, map, key, hash);
#     if (!entry) { /* new entry */
#         if (map->count >= map->size) {
#             resize(vm, map, map_nextsize(map->size));
#         }
#         entry = insert(vm, map, key, hash);
#         ++map->count;
#     }
#     if (value) {
#         entry->value = *value;
#     }
#     return value(entry);
# }
def be_map_insert(vm, map_, key, value=None):
    """Insert or update a key-value pair. Returns the value slot.

    If the key already exists, updates the value. Otherwise inserts a
    new entry, resizing if needed.
    """
    hash_val = _hashcode_bvalue(vm, key)
    entry = _find(vm, map_, key, hash_val)
    if entry is None:
        # New entry
        if map_.count >= map_.size:
            _resize(vm, map_, _map_nextsize(map_.size))
        entry = _insert(vm, map_, key, hash_val)
        map_.count += 1
    if value is not None:
        entry.value.type = value.type
        entry.value.v = value.v
    return entry.value


# ============================================================================
# be_map_remove (from be_map.c)
# ============================================================================

# int be_map_remove(bvm *vm, bmap *map, bvalue *key)
# {
#     uint32_t hash = hashcode(key);
#     bmapnode *slot = hash2slot(map, hash);
#
#     if (eqnode(vm, slot, key, hash)) { /* first node */
#         bmapnode *next = pos2slot(map, next(slot));
#         if (next) { /* has next */
#             *slot = *next;
#             slot = next;
#         }
#     } else {
#         bmapnode *prev = slot;
#         for (;;) {
#             int n = next(prev);
#             slot = pos2slot(map, n);
#             if (slot == NULL) {
#                 return bfalse;
#             }
#             if (eqnode(vm, slot, key, hash)) {
#                 break;
#             }
#             prev = slot;
#         }
#         next(prev) = next(slot);
#     }
#     setnil(slot);
#     if (map->lastfree < slot) {
#         map->lastfree = slot;
#     }
#     --map->count;
#     return btrue;
# }
def be_map_remove(vm, map_, key):
    """Remove a key from the map. Returns True if found, False otherwise."""
    hash_val = _hashcode_bvalue(vm, key)
    if map_.size == 0:
        return False
    slot_idx = hash_val % map_.size
    slot = map_.slots[slot_idx]

    if _eqnode(vm, slot, key, hash_val):
        # First node in chain matches
        n = slot.key.next
        next_node = map_.slots[n] if n != LASTNODE else None
        if next_node is not None:
            # Copy next node into this slot, then clear next node
            slot.key.type = next_node.key.type
            slot.key.v = next_node.key.v
            slot.key.next = next_node.key.next
            slot.value.type = next_node.value.type
            slot.value.v = next_node.value.v
            slot = next_node
            slot_idx = n
        # else: slot is the only node, will be cleared below
    else:
        # Search the chain for the matching node
        prev = slot
        while True:
            n = prev.key.next
            if n == LASTNODE:
                return False
            slot = map_.slots[n]
            slot_idx = n
            if _eqnode(vm, slot, key, hash_val):
                break
            prev = slot
        # Unlink from chain
        prev.key.next = slot.key.next

    # Clear the removed slot
    _setnil(slot)
    slot.key.next = LASTNODE
    # Update lastfree if this slot is further right
    if map_.lastfree < slot_idx:
        map_.lastfree = slot_idx
    map_.count -= 1
    return True


# ============================================================================
# be_map_findstr (from be_map.c)
# ============================================================================

# bvalue* be_map_findstr(bvm *vm, bmap *map, bstring *key)
# {
#     bvalue v;
#     var_setstr(&v, key);
#     return be_map_find(vm, map, &v);
# }
def be_map_findstr(vm, map_, key):
    """Find a value by string key. Returns the bvalue or None."""
    v = bvalue()
    var_setstr(v, key)
    return be_map_find(vm, map_, v)


# ============================================================================
# be_map_insertstr (from be_map.c)
# ============================================================================

# bvalue* be_map_insertstr(bvm *vm, bmap *map, bstring *key, bvalue *value)
# {
#     bvalue v;
#     var_setstr(&v, key);
#     set_fixed(key);
#     bvalue * r = be_map_insert(vm, map, &v, value);
#     restore_fixed(key);
#     return r;
# }
def be_map_insertstr(vm, map_, key, value=None):
    """Insert or update a string-keyed entry. Returns the value slot.

    In C, the string is temporarily GC-fixed during insertion to prevent
    collection. In Python, GC doesn't run during this call, so the
    fix/restore is a no-op, but we preserve the pattern for correctness.
    """
    v = bvalue()
    var_setstr(v, key)
    # set_fixed / restore_fixed — no-op in Python port
    r = be_map_insert(vm, map_, v, value)
    return r


# ============================================================================
# be_map_removestr (from be_map.c)
# ============================================================================

# void be_map_removestr(bvm *vm, bmap *map, bstring *key)
# {
#     bvalue v;
#     var_setstr(&v, key);
#     be_map_remove(vm, map, &v);
# }
def be_map_removestr(vm, map_, key):
    """Remove a string-keyed entry from the map."""
    v = bvalue()
    var_setstr(v, key)
    be_map_remove(vm, map_, v)


# ============================================================================
# be_map_next (from be_map.c)
# ============================================================================

# bmapnode* be_map_next(bmap *map, bmapiter *iter)
# {
#     bmapnode *end = map->slots + map->size;
#     *iter = *iter ? *iter + 1 : map->slots;
#     while (*iter < end && isnil(*iter)) {
#         ++(*iter);
#     }
#     return *iter < end ? *iter : NULL;
# }
def be_map_next(map_, iter_ref):
    """Advance the map iterator to the next non-nil node.

    iter_ref is a list containing a single integer (the current index),
    or [None] for the initial call. Returns (node, updated_iter_ref)
    or (None, iter_ref) when exhausted.

    Usage:
        it = [None]
        node, it = be_map_next(map_, it)
        while node is not None:
            # process node
            node, it = be_map_next(map_, it)
    """
    if iter_ref[0] is None:
        idx = 0
    else:
        idx = iter_ref[0] + 1

    while idx < map_.size and _isnil(map_.slots[idx]):
        idx += 1

    if idx < map_.size:
        iter_ref[0] = idx
        return map_.slots[idx], iter_ref
    else:
        iter_ref[0] = idx
        return None, iter_ref


# ============================================================================
# be_map_val2node (from be_map.c)
# ============================================================================

# bmapnode* be_map_val2node(bvalue *value)
# {
#     return (bmapnode *)((size_t)value - sizeof(bmapkey));
# }
def be_map_val2node(value):
    """Given a bvalue that is a node's value field, return the containing node.

    In C this does pointer arithmetic. In Python, we store a back-reference
    on the bvalue when it's part of a bmapnode. Since our bmapnode.value IS
    a bvalue object, callers who have a reference to node.value can find the
    node by searching. However, for the common usage pattern, the caller
    already has the node.

    For compatibility, we attach a _parent_node attribute during node creation.
    If not available, this function cannot recover the node and returns None.
    """
    return getattr(value, '_parent_node', None)


# ============================================================================
# be_map_compact (from be_map.c)
# ============================================================================

# void be_map_compact(bvm *vm, bmap *map)
# {
#     (void)vm;
#     if (!gc_isconst(map)) {
#         resize(vm, map, map->count ? map->count : 1);
#     }
# }
def be_map_compact(vm, map_):
    """Compact the map to exactly fit its entries (or size 1 if empty).

    Const maps (solidified) are not compacted.
    """
    if not gc_isconst(map_):
        _resize(vm, map_, map_.count if map_.count else 1)
