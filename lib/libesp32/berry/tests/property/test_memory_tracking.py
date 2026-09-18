"""
Feature: berry-python-port, Property 14: Memory tracking consistency

For any sequence of be_malloc and be_free calls, the gc.usage counter should
reflect the net allocated bytes, matching the same delta tracking as the C
implementation.

Validates: Requirements 13.1, 13.3
"""

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

from hypothesis import given, settings, assume
from hypothesis.strategies import integers, lists, tuples, sampled_from, composite

from berry_port.be_object import bgc
from berry_port.be_mem import be_malloc, be_free, be_realloc


# ---------------------------------------------------------------------------
# Minimal mock VM — only needs gc fields for be_realloc tracking
# ---------------------------------------------------------------------------
class MockVM:
    def __init__(self):
        self.gc = bgc()
        self.gc.usage = 0


# ---------------------------------------------------------------------------
# Property 14a: Single malloc increases gc.usage by exactly the requested size
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(size=integers(min_value=1, max_value=10000))
def test_malloc_increases_usage(size):
    """be_malloc(vm, size) should increase gc.usage by exactly size."""
    vm = MockVM()
    assert vm.gc.usage == 0

    ptr = be_malloc(vm, size)
    assert ptr is not None
    assert vm.gc.usage == size


# ---------------------------------------------------------------------------
# Property 14b: Free after malloc returns gc.usage to zero
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(size=integers(min_value=1, max_value=10000))
def test_free_decreases_usage(size):
    """be_free(vm, ptr, size) after be_malloc should return gc.usage to 0."""
    vm = MockVM()
    ptr = be_malloc(vm, size)
    assert vm.gc.usage == size

    be_free(vm, ptr, size)
    assert vm.gc.usage == 0


# ---------------------------------------------------------------------------
# Property 14c: Multiple mallocs accumulate in gc.usage
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(sizes=lists(integers(min_value=1, max_value=5000), min_size=1, max_size=50))
def test_multiple_mallocs_accumulate(sizes):
    """gc.usage should equal the sum of all allocated sizes."""
    vm = MockVM()
    ptrs = []

    for s in sizes:
        ptr = be_malloc(vm, s)
        ptrs.append((ptr, s))

    assert vm.gc.usage == sum(sizes)


# ---------------------------------------------------------------------------
# Property 14d: Freeing all allocations returns gc.usage to zero
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(sizes=lists(integers(min_value=1, max_value=5000), min_size=1, max_size=50))
def test_free_all_returns_to_zero(sizes):
    """After allocating and freeing all blocks, gc.usage should be 0."""
    vm = MockVM()
    ptrs = []

    for s in sizes:
        ptr = be_malloc(vm, s)
        ptrs.append((ptr, s))

    for ptr, s in ptrs:
        be_free(vm, ptr, s)

    assert vm.gc.usage == 0


# ---------------------------------------------------------------------------
# Property 14e: Realloc adjusts gc.usage by (new_size - old_size)
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(
    old_size=integers(min_value=1, max_value=5000),
    new_size=integers(min_value=1, max_value=5000),
)
def test_realloc_adjusts_usage(old_size, new_size):
    """be_realloc should adjust gc.usage by (new_size - old_size)."""
    assume(old_size != new_size)
    vm = MockVM()

    ptr = be_malloc(vm, old_size)
    assert vm.gc.usage == old_size

    ptr2 = be_realloc(vm, ptr, old_size, new_size)
    assert vm.gc.usage == new_size
    assert ptr2 is not None


# ---------------------------------------------------------------------------
# Property 14f: Realloc to zero is equivalent to free
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(size=integers(min_value=1, max_value=10000))
def test_realloc_to_zero_frees(size):
    """be_realloc(vm, ptr, size, 0) should decrease gc.usage by size."""
    vm = MockVM()
    ptr = be_malloc(vm, size)
    assert vm.gc.usage == size

    result = be_realloc(vm, ptr, size, 0)
    assert result is None
    assert vm.gc.usage == 0


# ---------------------------------------------------------------------------
# Property 14g: Realloc with same size is a no-op for gc.usage
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(size=integers(min_value=1, max_value=10000))
def test_realloc_same_size_noop(size):
    """be_realloc(vm, ptr, size, size) should not change gc.usage."""
    vm = MockVM()
    ptr = be_malloc(vm, size)
    assert vm.gc.usage == size

    ptr2 = be_realloc(vm, ptr, size, size)
    assert vm.gc.usage == size
    # Same-size realloc returns the original pointer
    assert ptr2 is ptr


# ---------------------------------------------------------------------------
# Property 14h: Interleaved malloc/free/realloc maintains correct gc.usage
# ---------------------------------------------------------------------------
@composite
def alloc_operations(draw):
    """Generate a sequence of alloc/free/realloc operations."""
    n_ops = draw(integers(min_value=1, max_value=30))
    ops = []
    live_ids = []  # track which allocations are still alive
    next_id = 0

    for _ in range(n_ops):
        if not live_ids:
            # Must allocate
            op_type = 'malloc'
        else:
            op_type = draw(sampled_from(['malloc', 'free', 'realloc']))

        if op_type == 'malloc':
            size = draw(integers(min_value=1, max_value=2000))
            ops.append(('malloc', next_id, size))
            live_ids.append(next_id)
            next_id += 1
        elif op_type == 'free':
            idx = draw(integers(min_value=0, max_value=len(live_ids) - 1))
            alloc_id = live_ids.pop(idx)
            ops.append(('free', alloc_id))
        else:  # realloc
            idx = draw(integers(min_value=0, max_value=len(live_ids) - 1))
            alloc_id = live_ids[idx]
            new_size = draw(integers(min_value=1, max_value=2000))
            ops.append(('realloc', alloc_id, new_size))

    return ops


@settings(max_examples=200)
@given(ops=alloc_operations())
def test_interleaved_operations_track_correctly(ops):
    """gc.usage should always equal the sum of live allocation sizes."""
    vm = MockVM()
    # Track: alloc_id -> (ptr, current_size)
    live = {}

    for op in ops:
        if op[0] == 'malloc':
            _, alloc_id, size = op
            ptr = be_malloc(vm, size)
            live[alloc_id] = (ptr, size)
        elif op[0] == 'free':
            _, alloc_id = op
            ptr, size = live.pop(alloc_id)
            be_free(vm, ptr, size)
        elif op[0] == 'realloc':
            _, alloc_id, new_size = op
            ptr, old_size = live[alloc_id]
            if old_size == new_size:
                continue  # same-size realloc is a no-op
            ptr2 = be_realloc(vm, ptr, old_size, new_size)
            live[alloc_id] = (ptr2, new_size)

    expected_usage = sum(size for _, size in live.values())
    assert vm.gc.usage == expected_usage, (
        f"gc.usage={vm.gc.usage} but expected {expected_usage}"
    )


# ---------------------------------------------------------------------------
# Property 14i: Realloc preserves data content (up to min of old/new size)
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(
    old_size=integers(min_value=1, max_value=5000),
    new_size=integers(min_value=1, max_value=5000),
)
def test_realloc_preserves_content(old_size, new_size):
    """Realloc should preserve data up to min(old_size, new_size)."""
    assume(old_size != new_size)
    vm = MockVM()

    ptr = be_malloc(vm, old_size)
    # Fill with a recognizable pattern
    for i in range(old_size):
        ptr[i] = i & 0xFF

    ptr2 = be_realloc(vm, ptr, old_size, new_size)
    preserve_len = min(old_size, new_size)
    for i in range(preserve_len):
        assert ptr2[i] == (i & 0xFF), (
            f"Data mismatch at index {i}: expected {i & 0xFF}, got {ptr2[i]}"
        )


# ---------------------------------------------------------------------------
# Property 14j: Zero-size malloc returns None and doesn't change gc.usage
# ---------------------------------------------------------------------------
def test_zero_size_malloc():
    """be_malloc(vm, 0) should return None and not change gc.usage."""
    vm = MockVM()
    # be_realloc(vm, None, 0, 0) — old_size == new_size, returns ptr (None)
    ptr = be_realloc(vm, None, 0, 0)
    assert ptr is None
    assert vm.gc.usage == 0
