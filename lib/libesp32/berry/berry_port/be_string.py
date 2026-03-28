"""
Berry string system: interning, hashing, and string table management.
Mirrors: src/be_string.c / src/be_string.h

Short strings (slen <= 64) are interned in a hash table for pointer-equality
comparison. Long strings (slen == 255, actual length in llen) are heap-allocated
without interning. Const strings (bcstring, GC_CONST) have an external pointer
and a precomputed hash.

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
    BE_STRING,
    GC_CONST, GC_FIXED, GC_WHITE,
    SHORT_STR_MAX_LEN,
    bstring, bcstring,
    gc_isconst, gc_isfixed, gc_iswhite, gc_setwhite,
    str_len,
)
from berry_port.be_mem import be_realloc, be_free
from berry_port.berry_conf import (
    BE_USE_STR_HASH_CACHE,
    BE_USE_DEBUG_GC,
)


# ============================================================================
# Internal macros (from be_string.c)
# ============================================================================

# #define next(_s)    cast(void*, cast(bstring*, (_s)->next))
def _next(s):
    """Get the next string in the hash chain."""
    return s.next

# #define sstr(_s)    cast(char*, cast(bsstring*, _s) + 1)
# #define lstr(_s)    cast(char*, cast(blstring*, _s) + 1)
# #define cstr(_s)    (cast(bcstring*, _s)->s)
# In Python, all string data is stored in bstring._s


# ============================================================================
# FNV-1a hash (from be_string.c)
# ============================================================================

# /* FNV-1a Hash */
# static uint32_t str_hash(const char *str, size_t len)
# {
#     uint32_t hash = 2166136261u;
#     be_assert(str || !len);
#     while (len--) {
#         hash = (hash ^ (unsigned char)*str++) * 16777619u;
#     }
#     return hash;
# }
def str_hash(s, length=None):
    """FNV-1a hash matching the C implementation exactly.

    Args:
        s: A string or bytes-like object.
        length: Number of bytes to hash. If None, uses len(s).

    Returns:
        uint32_t hash value.
    """
    if isinstance(s, str):
        data = s.encode('latin-1') if s else b''
    else:
        data = s if s else b''
    if length is None:
        length = len(data)

    hash_val = 2166136261
    for i in range(length):
        hash_val = hash_val ^ data[i]
        hash_val = (hash_val * 16777619) & 0xFFFFFFFF
    return hash_val


# ============================================================================
# be_strhash — public hash accessor (from be_string.c)
# ============================================================================

# uint32_t be_strhash(const bstring *s)
# {
#     if (gc_isconst(s) && (s->slen != 255)) {
#         bcstring* cs = cast(bcstring*, s);
#         if (cs->hash) {  /* if hash is null we need to compute it */
#             return cs->hash;
#         } else {
#             return str_hash(cstr(s), str_len(s));
#         }
#     }
# #if BE_USE_STR_HASH_CACHE
#     if (s->slen != 255) {
#         return cast(bsstring*, s)->hash;
#     }
# #endif
#     return str_hash(str(s), str_len(s));
# }
def be_strhash(s):
    """Return the hash of a Berry string object.

    For const short strings (bcstring), returns the cached hash if nonzero.
    For short strings with hash cache enabled, returns the cached hash.
    Otherwise computes FNV-1a on the fly.
    """
    if gc_isconst(s) and s.slen != 255:
        # bcstring with precomputed hash
        cs = s  # already a bcstring or compatible
        h = cs.hash if hasattr(cs, 'hash') else cs._hash
        if h:
            return h
        else:
            return str_hash(be_str2cstr(s), str_len(s))

    if BE_USE_STR_HASH_CACHE:
        if s.slen != 255:
            return s._hash

    return str_hash(be_str2cstr(s), str_len(s))


# ============================================================================
# be_str2cstr — get C string pointer (from be_string.c)
# ============================================================================

# const char* be_str2cstr(const bstring *s)
# {
#     be_assert(cast_str(s) != NULL);
#     if (s->slen == 255) {
#         return lstr(s);
#     }
#     if (gc_isconst(s)) {
#         return cstr(s);
#     }
#     return sstr(s);
# }
def be_str2cstr(s):
    """Return the Python string data from a Berry string object.

    Works for short strings, long strings, and const strings.
    """
    if gc_isconst(s) and isinstance(s, bcstring):
        return s.s
    return s._s


# ============================================================================
# be_eqstr — string equality (from be_string.c)
# ============================================================================

# int be_eqstr(bstring *s1, bstring *s2)
# {
#     int slen;
#     if (s1 == s2) { /* short string or the same string */
#         return 1;
#     }
#     slen = s1->slen;
#     /* discard different lengths */
#     if (slen != s2->slen) {
#         return 0;
#     }
#     /* long string */
#     if (slen == 255) {  /* s2->slen is also 255 */
#         blstring *ls1 = cast(blstring*, s1);
#         blstring *ls2 = cast(blstring*, s2);
#         return ls1->llen == ls2->llen && !strcmp(lstr(ls1), lstr(ls2));
#     }
#     /* const short strings */
#     if (gc_isconst(s1) || gc_isconst(s2)) { /* one of the two string is short const */
#         uint32_t hash1 = cast(bcstring*, s1)->hash;
#         uint32_t hash2 = cast(bcstring*, s2)->hash;
#         if (hash1 && hash2 && hash1 != hash2) {
#             return 0; /* if hash differ, since we know both are non-null */
#         }
#         return !strcmp(str(s1), str(s2));
#     }
#
#     /* if both strings are in-memory, they can't be equal without having the same pointer */
#     return 0;
# }
def be_eqstr(s1, s2):
    """Test Berry string equality, matching C semantics.

    Short interned strings use identity comparison. Long strings compare
    by length and content. Const strings compare by hash then content.
    """
    # Same object — covers interned short strings
    if s1 is s2:
        return 1

    slen = s1.slen
    # Different slen fields means different
    if slen != s2.slen:
        return 0

    # Long strings: compare llen and content
    if slen == 255:
        return 1 if (s1.llen == s2.llen and be_str2cstr(s1) == be_str2cstr(s2)) else 0

    # Const short strings: compare by hash first, then content
    if gc_isconst(s1) or gc_isconst(s2):
        hash1 = s1.hash if hasattr(s1, 'hash') else s1._hash
        hash2 = s2.hash if hasattr(s2, 'hash') else s2._hash
        if hash1 and hash2 and hash1 != hash2:
            return 0
        return 1 if be_str2cstr(s1) == be_str2cstr(s2) else 0

    # Both are in-memory short strings — if not same object, not equal
    return 0


# ============================================================================
# be_str_setextra (from be_string.c)
# ============================================================================

# void be_str_setextra(bstring *s, int extra)
# {
#     if (!gc_isconst(s)) {
#         s->extra = cast(bbyte, extra);
#     }
# }
def be_str_setextra(s, extra):
    """Set the extra field on a non-const string."""
    if not gc_isconst(s):
        s.extra = extra & 0xFF


# ============================================================================
# String table resize (from be_string.c)
# ============================================================================

# static void resize(bvm *vm, int size)
# {
#     int i;
#     struct bstringtable *tab = &vm->strtab;
#     if (size > tab->size) {
#         tab->table = be_realloc(vm, tab->table,
#             tab->size * sizeof(bstring*), size * sizeof(bstring*));
#         for (i = tab->size; i < size; ++i) {
#             tab->table[i] = NULL;
#         }
#     }
#     for (i = 0; i < tab->size; ++i) { /* rehash */
#         bstring *p = tab->table[i];
#         tab->table[i] = NULL;
#         while (p) { /* for each node in the list */
#             bstring *hnext = next(p);
#             uint32_t hash = be_strhash(p) & (size - 1);
#             p->next = cast(void*, tab->table[hash]);
#             tab->table[hash] = p;
#             p = hnext;
#         }
#     }
#     if (size < tab->size) {
#         for (i = size; i < tab->size; ++i) {
#             tab->table[i] = NULL;
#         }
#         tab->table = be_realloc(vm, tab->table,
#             tab->size * sizeof(bstring*), size * sizeof(bstring*));
#     }
#     tab->size = size;
# }
def _resize(vm, size):
    """Resize the string interning hash table.

    Rehashes all entries into the new table. In C this reallocates the
    pointer array; in Python we resize the list.
    """
    tab = vm.strtab

    if size > tab.size:
        # Grow: extend the table with None slots
        if tab.table is None:
            tab.table = [None] * size
        else:
            tab.table.extend([None] * (size - tab.size))

    # Rehash all existing entries
    old_size = tab.size
    for i in range(old_size):
        p = tab.table[i]
        tab.table[i] = None
        while p is not None:
            hnext = _next(p)
            h = be_strhash(p) & (size - 1)
            p.next = tab.table[h]
            tab.table[h] = p
            p = hnext

    if size < tab.size:
        # Shrink: clear excess slots and truncate
        for i in range(size, tab.size):
            tab.table[i] = None
        tab.table = tab.table[:size]

    tab.size = size


# ============================================================================
# free_sstring (from be_string.c)
# ============================================================================

# static void free_sstring(bvm *vm, bstring *str)
# {
#     be_free(vm, str, sizeof(bsstring) + str->slen + 1);
# }
def _free_sstring(vm, s):
    """Free a short string object and update gc.usage.

    In C this frees sizeof(bsstring) + slen + 1 bytes. In Python we
    approximate the size for gc.usage tracking. We update gc.usage
    directly rather than going through be_realloc, since there is no
    actual C memory block to free.
    """
    # Approximate C size: sizeof(bsstring) is ~12 bytes (header + hash) + slen + 1
    approx_size = 12 + s.slen + 1
    vm.gc.usage -= approx_size


# ============================================================================
# createstrobj (from be_string.c)
# ============================================================================

# static bstring* createstrobj(bvm *vm, size_t len, int islong)
# {
#     size_t size = (islong ? sizeof(blstring)
#                 : sizeof(bsstring)) + len + 1;
#     bgcobject *gco = be_gc_newstr(vm, size, islong);
#     bstring *s = cast_str(gco);
#     if (s) {
#         s->slen = islong ? 255 : (bbyte)len;
#         char *str = cast(char *, islong ? lstr(s) : sstr(s));
#         str[len] = '\0';
#     }
#     return s;
# }
def _createstrobj(vm, length, islong):
    """Create a new string GC object.

    In C this allocates via be_gc_newstr. In Python we create a bstring
    instance and register it with the GC.
    """
    # Approximate C allocation size for gc.usage tracking
    if islong:
        approx_size = 16 + length + 1  # sizeof(blstring) ~ 16
    else:
        approx_size = 12 + length + 1  # sizeof(bsstring) ~ 12

    s = bstring()
    s.type = BE_STRING
    s.marked = GC_WHITE
    s.slen = 255 if islong else (length & 0xFF)
    s._s = ""  # will be filled by caller

    if islong:
        # Long strings go on the GC object list (like be_newgcobj in C)
        s.next = vm.gc.list
        vm.gc.list = s
    # Short strings are NOT on gc.list — they live in the string table
    # and are collected by be_gcstrtab. Their .next field is used for
    # the string table bucket chain.

    # Track memory usage
    vm.gc.usage += approx_size

    return s


# ============================================================================
# newshortstr (from be_string.c)
# ============================================================================

# static bstring* newshortstr(bvm *vm, const char *str, size_t len)
# {
#     bstring *s;
#     int size = vm->strtab.size;
#     uint32_t hash = str_hash(str, len);
#     bstring **list = vm->strtab.table + (hash & (size - 1));
#
#     for (s = *list; s != NULL; s = next(s)) {
#         if (len == s->slen && !strncmp(str, sstr(s), len)) {
#             return s;
#         }
#     }
#     s = createstrobj(vm, len, 0);
#     if (s) {
#         /* recompute size and list that may have changed due to a GC */
#         size = vm->strtab.size;
#         list = vm->strtab.table + (hash & (size - 1));
#         memcpy(cast(char *, sstr(s)), str, len);
#         s->extra = 0;
#         s->next = cast(void*, *list);
# #if BE_USE_STR_HASH_CACHE
#         cast(bsstring*, s)->hash = hash;
# #endif
#         *list = s;
#         vm->strtab.count++;
#         if (vm->strtab.count > size << 2) {
#             resize(vm, size << 1);
#         }
#     }
#     return s;
# }
def _newshortstr(vm, s_data, length):
    """Create or find an interned short string.

    Searches the string table for an existing match. If not found,
    creates a new bstring, inserts it into the table, and resizes
    if the load factor exceeds 4.
    """
    size = vm.strtab.size
    hash_val = str_hash(s_data, length)
    bucket = hash_val & (size - 1)

    # Search existing entries in this bucket
    s = vm.strtab.table[bucket]
    while s is not None:
        if length == s.slen and be_str2cstr(s)[:length] == s_data[:length]:
            return s
        s = _next(s)

    # Not found — create new string object
    s = _createstrobj(vm, length, 0)
    if s is not None:
        # Recompute after possible GC
        size = vm.strtab.size
        bucket = hash_val & (size - 1)

        s._s = s_data[:length] if isinstance(s_data, str) else s_data[:length].decode('latin-1')
        s.extra = 0
        s._hash = hash_val
        s.next = vm.strtab.table[bucket]
        vm.strtab.table[bucket] = s
        vm.strtab.count += 1

        # Resize if load factor > 4
        if vm.strtab.count > size << 2:
            _resize(vm, size << 1)

    return s


# ============================================================================
# be_newlongstr (from be_string.c)
# ============================================================================

# bstring* be_newlongstr(bvm *vm, const char *str, size_t len)
# {
#     bstring *s;
#     blstring *ls;
#     s = createstrobj(vm, len, 1);
#     ls = cast(blstring*, s);
#     s->extra = 0;
#     ls->llen = cast_int(len);
#     if (str) { /* if the argument 'str' is NULL, we just allocate space */
#         memcpy(cast(char *, lstr(s)), str, len);
#     }
#     return s;
# }
def be_newlongstr(vm, s_data, length):
    """Create a new long string (not interned).

    If s_data is None, allocates space without copying content.
    """
    s = _createstrobj(vm, length, 1)
    s.extra = 0
    s.llen = length
    if s_data is not None:
        if isinstance(s_data, str):
            s._s = s_data[:length]
        else:
            s._s = s_data[:length].decode('latin-1')
    else:
        s._s = '\x00' * length  # allocate space
    return s


# ============================================================================
# be_newstr / be_newstrn (from be_string.c)
# ============================================================================

# bstring* be_newstr(bvm *vm, const char *str)
# {
#     return be_newstrn(vm, str, strlen(str));
# }
def be_newstr(vm, s_data):
    """Create a Berry string from a Python string (auto-length).

    If s_data contains non-Latin-1 characters (codepoints > 255), it is
    first UTF-8 encoded then decoded as Latin-1. This only happens for
    strings coming from Python (e.g. be_pushfstring formatting); internal
    Berry strings are always already in Latin-1 byte form.
    """
    if isinstance(s_data, str) and any(ord(c) > 255 for c in s_data):
        s_data = s_data.encode('utf-8').decode('latin-1')
    return be_newstrn(vm, s_data, len(s_data))


# bstring *be_newstrn(bvm *vm, const char *str, size_t len)
# {
#     if (len <= SHORT_STR_MAX_LEN) {
#         bstring *s = find_conststr(str, len);
#         return s ? s : newshortstr(vm, str, len);
#     }
#     return be_newlongstr(vm, str, len); /* long string */
# }
def be_newstrn(vm, s_data, length):
    """Create a Berry string of given length.

    Short strings (len <= 64) are interned. Long strings are heap-allocated.
    Searches the const string table first for short strings.
    """
    if length <= SHORT_STR_MAX_LEN:
        s = _find_conststr(vm, s_data, length)
        if s is not None:
            return s
        return _newshortstr(vm, s_data, length)
    return be_newlongstr(vm, s_data, length)


# ============================================================================
# find_conststr (from be_string.c)
# ============================================================================

# static bstring* find_conststr(const char *str, size_t len)
# {
#     const struct bconststrtab *tab = &m_const_string_table;
#     uint32_t hash = str_hash(str, len);
#     bcstring *s = (bcstring*)tab->table[hash % tab->size];
#     for (; s != NULL; s = next(s)) {
#         if (len == 0 && s->slen == 0) {
#             return (bstring*)s;
#         }
#         if (len == s->slen && !strncmp(str, s->s, len)) {
#             return (bstring*)s;
#         }
#     }
#     return NULL;
# }
def _find_conststr(vm, s_data, length):
    """Search the precompiled const string table.

    In the Python port, the const string table is stored on the VM
    as vm.const_strtab (a dict or None). Returns None if not found
    or if no const table is configured.
    """
    tab = getattr(vm, 'const_strtab', None)
    if tab is None:
        return None

    # tab is expected to be a dict-like: { hash_bucket_index: bcstring_chain }
    # For simplicity, we use a list-of-lists structure matching C.
    if not hasattr(tab, 'table') or tab.table is None:
        return None

    hash_val = str_hash(s_data, length)
    bucket = hash_val % tab.size
    s = tab.table[bucket]
    while s is not None:
        if length == 0 and s.slen == 0:
            return s
        if length == s.slen:
            cs_data = s.s if isinstance(s, bcstring) else be_str2cstr(s)
            if cs_data[:length] == s_data[:length]:
                return s
        s = _next(s)
    return None


# ============================================================================
# be_string_init (from be_string.c)
# ============================================================================

# void be_string_init(bvm *vm)
# {
#     resize(vm, 8);
# }
def be_string_init(vm):
    """Initialize the string interning table.

    Creates an 8-slot hash table.
    """
    _resize(vm, 8)


# ============================================================================
# be_string_deleteall (from be_string.c)
# ============================================================================

# void be_string_deleteall(bvm *vm)
# {
#     int i;
#     struct bstringtable *tab = &vm->strtab;
#     for (i = 0; i < tab->size; ++i) {
#         bstring *node = tab->table[i];
#         while (node) {
#             bstring *next = next(node);
#             free_sstring(vm, node);
#             node = next;
#         }
#     }
#     be_free(vm, tab->table, tab->size * sizeof(bstring*));
# }
def be_string_deleteall(vm):
    """Delete all interned strings and free the string table."""
    tab = vm.strtab
    if tab.table is None:
        return
    for i in range(tab.size):
        node = tab.table[i]
        while node is not None:
            next_node = _next(node)
            _free_sstring(vm, node)
            node = next_node
    tab.table = None
    tab.count = 0
    tab.size = 0


# ============================================================================
# be_gcstrtab — GC sweep for string table (from be_string.c)
# ============================================================================

# void be_gcstrtab(bvm *vm)
# {
#     struct bstringtable *tab = &vm->strtab;
#     int size = tab->size, i;
#     for (i = 0; i < size; ++i) {
#         bstring **list = tab->table + i;
#         bstring *prev = NULL, *node, *next;
#         for (node = *list; node; node = next) {
#             next = next(node);
#             if (!gc_isfixed(node) && gc_iswhite(node)) {
#                 free_sstring(vm, node);
#                 tab->count--;
#                 if (prev) { /* link list */
#                     prev->next = cast(void*, next);
#                 } else {
#                     *list = next;
#                 }
#             } else {
#                 prev = node;
#                 gc_setwhite(node);
#             }
#         }
#     }
#     if (BE_USE_DEBUG_GC || comp_is_gc_debug(vm)) {
#         resize(vm, tab->count + 4);
#     } else {
#         if (tab->count < size >> 2 and size > 8) {
#             resize(vm, size >> 1);
#         }
#     }
# }
def be_gcstrtab(vm):
    """Sweep the string table: free white (unreachable) strings.

    After sweeping, shrinks the table if the load factor drops below 1/4.
    In GC debug mode, resizes to count + 4 for aggressive rehashing.
    """
    tab = vm.strtab
    size = tab.size

    for i in range(size):
        prev = None
        node = tab.table[i]
        while node is not None:
            next_node = _next(node)
            if not gc_isfixed(node) and gc_iswhite(node):
                _free_sstring(vm, node)
                tab.count -= 1
                if prev is not None:
                    prev.next = next_node
                else:
                    tab.table[i] = next_node
            else:
                prev = node
                gc_setwhite(node)
            node = next_node

    # Check if we should resize
    gc_debug = BE_USE_DEBUG_GC or _comp_is_gc_debug(vm)
    if gc_debug:
        _resize(vm, tab.count + 4)
    else:
        if tab.count < (size >> 2) and size > 8:
            _resize(vm, size >> 1)


# ============================================================================
# Helper: comp_is_gc_debug (from be_vm.h)
# ============================================================================

# #define comp_is_gc_debug(vm)       ((vm)->compopt & (1<<COMP_GC_DEBUG))
def _comp_is_gc_debug(vm):
    """Check if GC debug mode is enabled in compiler options."""
    from berry_port.be_object import COMP_GC_DEBUG
    return bool(getattr(vm, 'compopt', 0) & (1 << COMP_GC_DEBUG))
