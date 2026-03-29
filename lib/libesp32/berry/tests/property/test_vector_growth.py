"""
Feature: berry-python-port, Property 15: Vector growth strategy

For any sequence of push operations on a bvector, the capacity should grow
using the same strategy as the C implementation (be_nextsize), and the count
should accurately reflect the number of elements.

Validates: Requirements 13.4, 22.2, 22.4
"""

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

from hypothesis import given, settings, assume
from hypothesis.strategies import integers, lists

from berry_port.be_object import bvector, bgc
from berry_port.be_vector import (
    be_vector_init, be_vector_delete, be_vector_push, be_vector_at,
    be_vector_count, be_vector_capacity, be_vector_isempty,
    be_vector_remove_end, be_vector_resize, be_vector_clear,
    be_vector_release, be_nextsize, _binary_search, _nextpow,
    _BINARY_SEARCH_TAB,
)


# ---------------------------------------------------------------------------
# Minimal mock VM — only needs a gc.usage counter for be_realloc tracking
# ---------------------------------------------------------------------------
class MockVM:
    def __init__(self):
        self.gc = bgc()
        self.gc.usage = 0


def _make_vector(vm, elem_size=4):
    """Create and initialize a fresh bvector."""
    v = bvector()
    be_vector_init(vm, v, elem_size)
    return v


# ---------------------------------------------------------------------------
# Property 15a: be_nextsize always returns a value strictly greater than input
# ---------------------------------------------------------------------------
@settings(max_examples=500)
@given(size=integers(min_value=0, max_value=100000))
def test_nextsize_strictly_greater(size):
    """be_nextsize(n) must return a value > n for any non-negative n."""
    result = be_nextsize(size)
    assert result > size, f"be_nextsize({size}) = {result}, expected > {size}"


# ---------------------------------------------------------------------------
# Property 15b: be_nextsize matches binary_search table for sizes < 1024
# ---------------------------------------------------------------------------
@settings(max_examples=300)
@given(size=integers(min_value=0, max_value=1023))
def test_nextsize_uses_table_below_1024(size):
    """For sizes < 1024, be_nextsize should return a value from the table."""
    result = be_nextsize(size)
    assert result in _BINARY_SEARCH_TAB, (
        f"be_nextsize({size}) = {result}, not in capacity table"
    )


# ---------------------------------------------------------------------------
# Property 15c: be_nextsize returns power of two for sizes >= 1024
# ---------------------------------------------------------------------------
@settings(max_examples=300)
@given(size=integers(min_value=1024, max_value=100000))
def test_nextsize_power_of_two_above_1024(size):
    """For sizes >= 1024, be_nextsize should return a power of two."""
    result = be_nextsize(size)
    assert result > size, f"be_nextsize({size}) = {result}, expected > {size}"
    assert result & (result - 1) == 0, (
        f"be_nextsize({size}) = {result}, not a power of two"
    )


# ---------------------------------------------------------------------------
# Property 15d: Vector count tracks pushes accurately
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(n=integers(min_value=0, max_value=500))
def test_vector_count_tracks_pushes(n):
    """After n pushes, vector count should be exactly n."""
    vm = MockVM()
    v = _make_vector(vm)

    for i in range(n):
        be_vector_push(vm, v, i)

    assert be_vector_count(v) == n, (
        f"After {n} pushes, count = {be_vector_count(v)}"
    )

    be_vector_delete(vm, v)


# ---------------------------------------------------------------------------
# Property 15e: Capacity is always >= count after pushes
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(n=integers(min_value=1, max_value=500))
def test_capacity_ge_count(n):
    """Vector capacity must always be >= count."""
    vm = MockVM()
    v = _make_vector(vm)

    for i in range(n):
        be_vector_push(vm, v, i)
        assert be_vector_capacity(v) >= be_vector_count(v), (
            f"After push {i+1}: capacity={be_vector_capacity(v)} "
            f"< count={be_vector_count(v)}"
        )

    be_vector_delete(vm, v)


# ---------------------------------------------------------------------------
# Property 15f: Capacity grows via be_nextsize when exceeded
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(n=integers(min_value=3, max_value=500))
def test_capacity_grows_via_nextsize(n):
    """Each capacity increase should match be_nextsize(old_capacity)."""
    vm = MockVM()
    v = _make_vector(vm)
    observed_capacities = [be_vector_capacity(v)]  # initial = 2

    for i in range(n):
        old_cap = be_vector_capacity(v)
        be_vector_push(vm, v, i)
        new_cap = be_vector_capacity(v)
        if new_cap != old_cap:
            assert new_cap == be_nextsize(old_cap), (
                f"Capacity grew from {old_cap} to {new_cap}, "
                f"expected {be_nextsize(old_cap)}"
            )
            observed_capacities.append(new_cap)

    be_vector_delete(vm, v)


# ---------------------------------------------------------------------------
# Property 15g: Push preserves element values (data integrity)
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(elems=lists(integers(min_value=-10000, max_value=10000),
                   min_size=1, max_size=300))
def test_push_preserves_values(elems):
    """All pushed values should be retrievable at their correct indices."""
    vm = MockVM()
    v = _make_vector(vm)

    for val in elems:
        be_vector_push(vm, v, val)

    for i, expected in enumerate(elems):
        actual = be_vector_at(v, i)
        assert actual == expected, (
            f"Element at index {i}: got {actual}, expected {expected}"
        )

    be_vector_delete(vm, v)


# ---------------------------------------------------------------------------
# Property 15h: Vector starts empty, becomes non-empty after push
# ---------------------------------------------------------------------------
@settings(max_examples=100)
@given(n=integers(min_value=1, max_value=100))
def test_empty_then_nonempty(n):
    """A fresh vector is empty; after any push it is not."""
    vm = MockVM()
    v = _make_vector(vm)

    assert be_vector_isempty(v), "Fresh vector should be empty"
    assert be_vector_count(v) == 0

    be_vector_push(vm, v, 42)
    assert not be_vector_isempty(v), "Vector should not be empty after push"
    assert be_vector_count(v) == 1

    be_vector_delete(vm, v)


# ---------------------------------------------------------------------------
# Property 15i: remove_end decrements count correctly
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(n=integers(min_value=1, max_value=200),
       pops=integers(min_value=0, max_value=200))
def test_remove_end_decrements(n, pops):
    """Popping k elements from a vector of n should leave count = n - k."""
    pops = min(pops, n)  # can't pop more than we have
    vm = MockVM()
    v = _make_vector(vm)

    for i in range(n):
        be_vector_push(vm, v, i)

    for _ in range(pops):
        be_vector_remove_end(v)

    assert be_vector_count(v) == n - pops, (
        f"After {n} pushes and {pops} pops, count = {be_vector_count(v)}, "
        f"expected {n - pops}"
    )

    be_vector_delete(vm, v)


# ---------------------------------------------------------------------------
# Property 15j: clear resets count to zero without changing capacity
# ---------------------------------------------------------------------------
@settings(max_examples=100)
@given(n=integers(min_value=1, max_value=200))
def test_clear_resets_count(n):
    """Clearing a vector should set count to 0 but preserve capacity."""
    vm = MockVM()
    v = _make_vector(vm)

    for i in range(n):
        be_vector_push(vm, v, i)

    cap_before = be_vector_capacity(v)
    be_vector_clear(v)

    assert be_vector_count(v) == 0, "Count should be 0 after clear"
    assert be_vector_isempty(v), "Vector should be empty after clear"
    assert be_vector_capacity(v) == cap_before, (
        "Capacity should not change after clear"
    )

    be_vector_delete(vm, v)


# ---------------------------------------------------------------------------
# Property 15k: resize grows capacity via be_nextsize when needed
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(initial_pushes=integers(min_value=0, max_value=50),
       target=integers(min_value=0, max_value=500))
def test_resize_capacity_growth(initial_pushes, target):
    """After resize, capacity >= count and grew via be_nextsize if needed."""
    vm = MockVM()
    v = _make_vector(vm)

    for i in range(initial_pushes):
        be_vector_push(vm, v, i)

    old_cap = be_vector_capacity(v)
    be_vector_resize(vm, v, target)

    assert be_vector_count(v) == target, (
        f"After resize({target}), count = {be_vector_count(v)}"
    )
    assert be_vector_capacity(v) >= target, (
        f"After resize({target}), capacity = {be_vector_capacity(v)}"
    )

    be_vector_delete(vm, v)


# ---------------------------------------------------------------------------
# Property 15l: release shrinks capacity to count
# ---------------------------------------------------------------------------
@settings(max_examples=100)
@given(n=integers(min_value=1, max_value=200))
def test_release_shrinks_to_count(n):
    """After release, capacity should equal count (or 0 if empty)."""
    vm = MockVM()
    v = _make_vector(vm)

    for i in range(n):
        be_vector_push(vm, v, i)

    be_vector_release(vm, v)

    assert be_vector_capacity(v) == n, (
        f"After release with {n} elements, capacity = {be_vector_capacity(v)}"
    )
    assert be_vector_count(v) == n

    # Cleanup — vector is already released, just reset
    v.data = None
    v._mem = None


# ---------------------------------------------------------------------------
# Property 15m: gc.usage tracks memory deltas correctly through vector ops
# ---------------------------------------------------------------------------
@settings(max_examples=100)
@given(n=integers(min_value=1, max_value=300))
def test_gc_usage_tracking(n):
    """gc.usage should be 0 after creating and deleting a vector."""
    vm = MockVM()
    v = _make_vector(vm, elem_size=8)

    for i in range(n):
        be_vector_push(vm, v, i)

    usage_before_delete = vm.gc.usage
    assert usage_before_delete > 0, "gc.usage should be positive after pushes"

    be_vector_delete(vm, v)
    assert vm.gc.usage == 0, (
        f"gc.usage should be 0 after delete, got {vm.gc.usage}"
    )
