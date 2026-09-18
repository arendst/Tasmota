"""
Feature: berry-python-port, Property 11: Map compaction determinism

For any map with a known set of key-value pairs, calling be_map_compact
should produce a slot layout with the same size, count, and node arrangement
(key positions and next-chain links) as a second compaction of the same data.

Additionally, after compaction:
- map.size == map.count (or 1 if empty)
- All original key-value pairs are still retrievable
- The slot layout is deterministic (same input → same layout)

Validates: Requirements 9.4
"""

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

from hypothesis import given, settings, assume
from hypothesis.strategies import (
    integers, lists, tuples, floats, sampled_from, one_of, just,
)

from berry_port.be_object import (
    BE_NIL, BE_INT, BE_REAL, BE_BOOL, BE_STRING,
    bgc, bstringtable, bvalue, bmap,
    var_setint, var_setreal, var_setbool, var_toint, var_toreal, var_tobool,
    var_isnil,
)
from berry_port.be_map import (
    be_map_new, be_map_insert, be_map_find, be_map_compact, be_map_next,
    LASTNODE, _isnil,
)
from berry_port.be_string import be_string_init, be_newstrn


# ---------------------------------------------------------------------------
# Minimal mock VM
# ---------------------------------------------------------------------------
class MockVM:
    def __init__(self):
        self.gc = bgc()
        self.gc.usage = 0
        self.strtab = bstringtable()
        self.compopt = 0


def _fresh_vm():
    vm = MockVM()
    be_string_init(vm)
    return vm


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------
def _make_int_key(i):
    """Create a bvalue with an integer key."""
    k = bvalue()
    var_setint(k, i)
    return k


def _make_int_val(i):
    """Create a bvalue with an integer value."""
    v = bvalue()
    var_setint(v, i)
    return v


def _collect_entries(map_):
    """Collect all (key_type, key_val, val_type, val_val) from a map."""
    entries = []
    it = [None]
    node, it = be_map_next(map_, it)
    while node is not None:
        entries.append((node.key.type, node.key.v, node.value.type, node.value.v))
        node, it = be_map_next(map_, it)
    return entries


def _snapshot_layout(map_):
    """Capture the full slot layout: for each slot, (is_nil, key_type, key_v, next)."""
    layout = []
    for i in range(map_.size):
        node = map_.slots[i]
        if _isnil(node):
            layout.append((True, None, None, node.key.next))
        else:
            layout.append((False, node.key.type, node.key.v, node.key.next))
    return layout


# ---------------------------------------------------------------------------
# Strategies
# ---------------------------------------------------------------------------
# Unique integer keys with associated integer values
int_kv_pairs = lists(
    tuples(integers(min_value=-1000, max_value=1000),
           integers(min_value=-10000, max_value=10000)),
    min_size=0,
    max_size=50,
    unique_by=lambda t: t[0],
)


# ---------------------------------------------------------------------------
# Property 11a: After compaction, size == count (or 1 if empty)
# ---------------------------------------------------------------------------
@settings(max_examples=300)
@given(pairs=int_kv_pairs)
def test_compact_size_equals_count(pairs):
    """After be_map_compact, map.size must equal map.count (or 1 if empty)."""
    vm = _fresh_vm()
    m = be_map_new(vm)

    for k_int, v_int in pairs:
        be_map_insert(vm, m, _make_int_key(k_int), _make_int_val(v_int))

    be_map_compact(vm, m)

    expected_size = len(pairs) if len(pairs) > 0 else 1
    assert m.size == expected_size, (
        f"After compact: size={m.size}, expected={expected_size}, count={m.count}"
    )
    assert m.count == len(pairs), (
        f"Count changed after compact: got {m.count}, expected {len(pairs)}"
    )


# ---------------------------------------------------------------------------
# Property 11b: All entries are preserved after compaction
# ---------------------------------------------------------------------------
@settings(max_examples=300)
@given(pairs=int_kv_pairs)
def test_compact_preserves_all_entries(pairs):
    """Every key-value pair inserted before compaction must be findable after."""
    vm = _fresh_vm()
    m = be_map_new(vm)

    for k_int, v_int in pairs:
        be_map_insert(vm, m, _make_int_key(k_int), _make_int_val(v_int))

    be_map_compact(vm, m)

    for k_int, v_int in pairs:
        result = be_map_find(vm, m, _make_int_key(k_int))
        assert result is not None, f"Key {k_int} not found after compaction"
        assert result.type == BE_INT, f"Value type mismatch for key {k_int}"
        assert result.v == v_int, (
            f"Value mismatch for key {k_int}: got {result.v}, expected {v_int}"
        )


# ---------------------------------------------------------------------------
# Property 11c: Compaction is deterministic (same input → same layout)
# ---------------------------------------------------------------------------
@settings(max_examples=300)
@given(pairs=int_kv_pairs)
def test_compact_deterministic_layout(pairs):
    """Two maps built with the same key-value pairs in the same order,
    then compacted, must produce identical slot layouts."""
    vm1 = _fresh_vm()
    vm2 = _fresh_vm()
    m1 = be_map_new(vm1)
    m2 = be_map_new(vm2)

    for k_int, v_int in pairs:
        be_map_insert(vm1, m1, _make_int_key(k_int), _make_int_val(v_int))
        be_map_insert(vm2, m2, _make_int_key(k_int), _make_int_val(v_int))

    be_map_compact(vm1, m1)
    be_map_compact(vm2, m2)

    assert m1.size == m2.size, (
        f"Size mismatch: {m1.size} vs {m2.size}"
    )
    assert m1.count == m2.count, (
        f"Count mismatch: {m1.count} vs {m2.count}"
    )

    layout1 = _snapshot_layout(m1)
    layout2 = _snapshot_layout(m2)
    assert layout1 == layout2, (
        f"Slot layouts differ after compaction of identical input"
    )


# ---------------------------------------------------------------------------
# Property 11d: No nil holes in compacted map (all slots occupied or trailing)
# ---------------------------------------------------------------------------
@settings(max_examples=300)
@given(pairs=int_kv_pairs)
def test_compact_no_wasted_slots(pairs):
    """After compaction, the number of non-nil slots must equal map.count."""
    vm = _fresh_vm()
    m = be_map_new(vm)

    for k_int, v_int in pairs:
        be_map_insert(vm, m, _make_int_key(k_int), _make_int_val(v_int))

    be_map_compact(vm, m)

    non_nil = sum(1 for i in range(m.size) if not _isnil(m.slots[i]))
    assert non_nil == m.count, (
        f"Non-nil slots ({non_nil}) != count ({m.count}) after compaction"
    )


# ---------------------------------------------------------------------------
# Property 11e: Compaction is idempotent in size and content
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(pairs=int_kv_pairs)
def test_compact_idempotent(pairs):
    """Compacting an already-compacted map should preserve size, count,
    and all entries. (Physical slot order may change due to rehash, but
    the logical content and dimensions must be stable.)"""
    vm = _fresh_vm()
    m = be_map_new(vm)

    for k_int, v_int in pairs:
        be_map_insert(vm, m, _make_int_key(k_int), _make_int_val(v_int))

    be_map_compact(vm, m)
    size_first = m.size
    count_first = m.count
    entries_first = sorted(_collect_entries(m))

    be_map_compact(vm, m)
    entries_second = sorted(_collect_entries(m))

    assert m.size == size_first, "Size changed on second compaction"
    assert m.count == count_first, "Count changed on second compaction"
    assert entries_first == entries_second, "Entries changed on second compaction"


# ---------------------------------------------------------------------------
# Edge case: empty map compaction
# ---------------------------------------------------------------------------
def test_compact_empty_map():
    """Compacting an empty map should result in size=1, count=0."""
    vm = _fresh_vm()
    m = be_map_new(vm)

    be_map_compact(vm, m)

    assert m.size == 1, f"Empty map compact: size={m.size}, expected 1"
    assert m.count == 0, f"Empty map compact: count={m.count}, expected 0"


# ---------------------------------------------------------------------------
# Edge case: single-entry map compaction
# ---------------------------------------------------------------------------
def test_compact_single_entry():
    """Compacting a map with one entry should result in size=1, count=1."""
    vm = _fresh_vm()
    m = be_map_new(vm)

    be_map_insert(vm, m, _make_int_key(42), _make_int_val(100))
    be_map_compact(vm, m)

    assert m.size == 1, f"Single-entry compact: size={m.size}, expected 1"
    assert m.count == 1
    result = be_map_find(vm, m, _make_int_key(42))
    assert result is not None and result.v == 100
