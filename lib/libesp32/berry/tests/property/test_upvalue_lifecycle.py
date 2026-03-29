"""
Feature: berry-python-port, Property 13: Upvalue lifecycle

For any closure with upvalues, while the enclosing scope is active, open
upvalues should reference the correct stack slot. When the scope exits,
be_upvals_close should copy the value into the upvalue's internal storage
and redirect the pointer, preserving the captured value.

Specifically:
- _findupval creates a new open upvalue pointing to the given stack index.
- Calling _findupval twice for the same stack index returns the same upvalue
  (sharing).
- be_initupvals resolves instack upvalues to open upvalues on the stack and
  inherited upvalues from the enclosing closure.
- be_upvals_close copies the stack value into u_value and marks the upvalue
  as closed (self-reference sentinel).
- After closing, the captured value is preserved regardless of stack changes.
- be_release_upvalues decrements refcnt; closed upvalues with refcnt==0 are
  freed (set to None).
- The open upvalue list (vm.upvalist) is maintained correctly across
  find/close operations.

Validates: Requirements 17.3, 17.4, 17.5
"""

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

from hypothesis import given, settings, assume
from hypothesis.strategies import (
    integers, lists, composite, sampled_from, booleans,
)

from berry_port.be_object import (
    BE_NIL, BE_INT, BE_CLOSURE,
    bgc, bstringtable, bvalue, bclosure, bproto, bupvaldesc, bupval,
    bcallframe,
    var_setint, var_setnil, var_toobj,
)
from berry_port.be_func import (
    _findupval, be_initupvals, be_upvals_close, be_release_upvalues,
    _is_closed, upval_value, be_newclosure, be_newproto,
)
from berry_port.be_string import be_string_init


# ---------------------------------------------------------------------------
# Minimal mock VM with stack support
# ---------------------------------------------------------------------------
STACK_SIZE = 64

class MockVM:
    def __init__(self):
        self.gc = bgc()
        self.gc.usage = 0
        self.strtab = bstringtable()
        self.compopt = 0
        # Stack: list of bvalue
        self.stack = [bvalue() for _ in range(STACK_SIZE)]
        self.reg_idx = 0
        self.upvalist = None
        # Call frame (needed by be_initupvals)
        self.cf = bcallframe()
        self.cf.func = 0
        self.cf.reg = 0


def _fresh_vm():
    vm = MockVM()
    be_string_init(vm)
    return vm


def _set_stack_int(vm, idx, val):
    """Set vm.stack[idx] to an integer value."""
    var_setint(vm.stack[idx], val)


def _count_open_upvals(vm):
    """Count nodes in the open upvalue list."""
    n = 0
    node = vm.upvalist
    while node is not None:
        n += 1
        node = node.u_next
    return n


# ---------------------------------------------------------------------------
# Strategies
# ---------------------------------------------------------------------------
stack_indices = integers(min_value=1, max_value=STACK_SIZE - 2)
int_values = integers(min_value=-10000, max_value=10000)


# ---------------------------------------------------------------------------
# Property 13a: _findupval creates open upvalue at correct stack index
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(idx=stack_indices, val=int_values)
def test_findupval_creates_open_upvalue(idx, val):
    """_findupval should create an open upvalue whose value field equals
    the requested stack index."""
    vm = _fresh_vm()
    _set_stack_int(vm, idx, val)

    uv = _findupval(vm, idx)

    assert uv is not None
    assert uv.value == idx, f"Open upvalue should point to stack index {idx}, got {uv.value}"
    assert not _is_closed(uv), "Newly created upvalue should be open"
    assert uv.refcnt == 0, "Newly created upvalue should have refcnt 0"


# ---------------------------------------------------------------------------
# Property 13b: _findupval returns same upvalue for same stack index
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(idx=stack_indices)
def test_findupval_shares_upvalue(idx):
    """Calling _findupval twice for the same stack index should return
    the exact same upvalue object (sharing)."""
    vm = _fresh_vm()

    uv1 = _findupval(vm, idx)
    uv2 = _findupval(vm, idx)

    assert uv1 is uv2, "Same stack index should yield same upvalue object"


# ---------------------------------------------------------------------------
# Property 13c: _findupval creates distinct upvalues for distinct indices
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(idx1=stack_indices, idx2=stack_indices)
def test_findupval_distinct_for_different_indices(idx1, idx2):
    """Different stack indices should produce different upvalue objects."""
    assume(idx1 != idx2)
    vm = _fresh_vm()

    uv1 = _findupval(vm, idx1)
    uv2 = _findupval(vm, idx2)

    assert uv1 is not uv2, "Different stack indices should yield different upvalues"


# ---------------------------------------------------------------------------
# Property 13d: be_upvals_close copies stack value and marks closed
# ---------------------------------------------------------------------------
@settings(max_examples=300)
@given(idx=stack_indices, val=int_values)
def test_upvals_close_captures_value(idx, val):
    """After be_upvals_close, the upvalue should be closed and its u_value
    should contain the stack value at the time of closing."""
    vm = _fresh_vm()
    _set_stack_int(vm, idx, val)

    uv = _findupval(vm, idx)
    uv.refcnt = 1  # simulate being referenced by a closure

    be_upvals_close(vm, idx)

    assert _is_closed(uv), "Upvalue should be closed after be_upvals_close"
    assert uv.u_value.type == BE_INT, "Closed upvalue should preserve type"
    assert uv.u_value.v == val, f"Closed upvalue should preserve value {val}, got {uv.u_value.v}"


# ---------------------------------------------------------------------------
# Property 13e: Closed upvalue preserves value after stack mutation
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(idx=stack_indices, val_before=int_values, val_after=int_values)
def test_closed_upvalue_independent_of_stack(idx, val_before, val_after):
    """After closing, the upvalue's captured value should not change when
    the original stack slot is overwritten."""
    vm = _fresh_vm()
    _set_stack_int(vm, idx, val_before)

    uv = _findupval(vm, idx)
    uv.refcnt = 1

    be_upvals_close(vm, idx)

    # Mutate the stack slot
    _set_stack_int(vm, idx, val_after)

    # The closed upvalue should still hold val_before
    assert uv.u_value.v == val_before, (
        f"Closed upvalue should be independent of stack: "
        f"expected {val_before}, got {uv.u_value.v}"
    )


# ---------------------------------------------------------------------------
# Property 13f: be_upvals_close removes upvalue from open list
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(idx=stack_indices)
def test_upvals_close_removes_from_open_list(idx):
    """After closing, the upvalue should no longer appear in vm.upvalist."""
    vm = _fresh_vm()

    uv = _findupval(vm, idx)
    uv.refcnt = 1
    assert _count_open_upvals(vm) == 1

    be_upvals_close(vm, idx)

    assert _count_open_upvals(vm) == 0, "Closed upvalue should be removed from open list"


# ---------------------------------------------------------------------------
# Property 13g: be_upvals_close only closes upvalues at or above level
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(low=integers(min_value=1, max_value=20),
       high=integers(min_value=21, max_value=40))
def test_upvals_close_selective(low, high):
    """be_upvals_close(vm, level) should only close upvalues with
    stack index >= level, leaving lower ones open."""
    vm = _fresh_vm()
    _set_stack_int(vm, low, 100)
    _set_stack_int(vm, high, 200)

    uv_low = _findupval(vm, low)
    uv_low.refcnt = 1
    uv_high = _findupval(vm, high)
    uv_high.refcnt = 1

    # Close only upvalues at or above 'high'
    be_upvals_close(vm, high)

    assert _is_closed(uv_high), "Upvalue at 'high' should be closed"
    assert not _is_closed(uv_low), "Upvalue at 'low' should remain open"
    assert _count_open_upvals(vm) == 1, "Only the low upvalue should remain in open list"


# ---------------------------------------------------------------------------
# Property 13h: Unreferenced upvalues are discarded on close
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(idx=stack_indices)
def test_unreferenced_upvalue_discarded_on_close(idx):
    """An upvalue with refcnt==0 should be discarded (not closed) when
    be_upvals_close is called."""
    vm = _fresh_vm()

    uv = _findupval(vm, idx)
    assert uv.refcnt == 0  # not referenced by any closure

    be_upvals_close(vm, idx)

    # The upvalue was discarded, not closed — open list should be empty
    assert _count_open_upvals(vm) == 0


# ---------------------------------------------------------------------------
# Property 13i: be_release_upvalues decrements refcnt
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(idx=stack_indices, val=int_values)
def test_release_upvalues_decrements_refcnt(idx, val):
    """be_release_upvalues should decrement refcnt on each upvalue held
    by the closure."""
    vm = _fresh_vm()
    _set_stack_int(vm, idx, val)

    uv = _findupval(vm, idx)
    uv.refcnt = 2  # simulate two closures sharing this upvalue

    cl = bclosure()
    cl.type = BE_CLOSURE
    cl.nupvals = 1
    cl.upvals = [uv]

    be_release_upvalues(vm, cl)

    assert uv.refcnt == 1, f"refcnt should be decremented to 1, got {uv.refcnt}"


# ---------------------------------------------------------------------------
# Property 13j: Closed upvalue freed when refcnt drops to 0
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(idx=stack_indices, val=int_values)
def test_closed_upvalue_freed_at_zero_refcnt(idx, val):
    """A closed upvalue with refcnt dropping to 0 via be_release_upvalues
    should be set to None in the closure's upvals list."""
    vm = _fresh_vm()
    _set_stack_int(vm, idx, val)

    uv = _findupval(vm, idx)
    uv.refcnt = 1

    be_upvals_close(vm, idx)
    assert _is_closed(uv)

    cl = bclosure()
    cl.type = BE_CLOSURE
    cl.nupvals = 1
    cl.upvals = [uv]

    be_release_upvalues(vm, cl)

    assert cl.upvals[0] is None, "Closed upvalue with refcnt 0 should be freed"


# ---------------------------------------------------------------------------
# Property 13k: be_initupvals resolves instack upvalues correctly
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(val=int_values)
def test_initupvals_instack(val):
    """be_initupvals should create open upvalues for instack descriptors
    pointing to the correct stack slot."""
    vm = _fresh_vm()
    vm.reg_idx = 2  # base register for current frame

    # Set up a value at reg_idx + 0
    _set_stack_int(vm, vm.reg_idx, val)

    # Create an enclosing closure on the call frame
    # be_initupvals reads vm.stack[vm.cf.func] to get the enclosing closure
    outer_cl = bclosure()
    outer_cl.type = BE_CLOSURE
    outer_cl.nupvals = 0
    outer_cl.upvals = []
    func_val = bvalue()
    func_val.type = BE_CLOSURE
    func_val.v = outer_cl
    # Store func_val in the stack and set cf.func to its index
    func_idx = 0
    vm.stack[func_idx] = func_val
    vm.cf.func = func_idx

    # Create a proto with one instack upvalue at idx=0
    proto = bproto()
    proto.nupvals = 1
    desc = bupvaldesc()
    desc.instack = 1
    desc.idx = 0
    proto.upvals = [desc]

    # Create the closure
    cl = bclosure()
    cl.type = BE_CLOSURE
    cl.nupvals = 1
    cl.upvals = []
    cl.proto = proto

    be_initupvals(vm, cl)

    assert len(cl.upvals) == 1
    uv = cl.upvals[0]
    assert uv is not None
    assert not _is_closed(uv), "Instack upvalue should be open"
    assert uv.value == vm.reg_idx + 0, "Should point to reg_idx + desc.idx"
    assert uv.refcnt == 1


# ---------------------------------------------------------------------------
# Property 13l: be_initupvals resolves inherited upvalues correctly
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(val=int_values)
def test_initupvals_inherited(val):
    """be_initupvals should inherit upvalues from the enclosing closure
    for non-instack descriptors."""
    vm = _fresh_vm()
    vm.reg_idx = 2

    # Create a closed upvalue to inherit
    inherited_uv = bupval()
    inherited_uv.value = inherited_uv  # closed
    inherited_uv.u_value = bvalue()
    var_setint(inherited_uv.u_value, val)
    inherited_uv.refcnt = 1

    # Enclosing closure holds this upvalue
    # be_initupvals reads vm.stack[vm.cf.func] to get the enclosing closure
    outer_cl = bclosure()
    outer_cl.type = BE_CLOSURE
    outer_cl.nupvals = 1
    outer_cl.upvals = [inherited_uv]
    func_val = bvalue()
    func_val.type = BE_CLOSURE
    func_val.v = outer_cl
    # Store func_val in the stack and set cf.func to its index
    func_idx = 0
    vm.stack[func_idx] = func_val
    vm.cf.func = func_idx

    # Proto with one non-instack upvalue at idx=0
    proto = bproto()
    proto.nupvals = 1
    desc = bupvaldesc()
    desc.instack = 0
    desc.idx = 0
    proto.upvals = [desc]

    cl = bclosure()
    cl.type = BE_CLOSURE
    cl.nupvals = 1
    cl.upvals = []
    cl.proto = proto

    be_initupvals(vm, cl)

    assert cl.upvals[0] is inherited_uv, "Should inherit the same upvalue object"
    assert inherited_uv.refcnt == 2, "Inherited upvalue refcnt should be incremented"


# ---------------------------------------------------------------------------
# Property 13m: Multiple closures sharing an upvalue see same close
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(idx=stack_indices, val=int_values)
def test_shared_upvalue_close_visible_to_all(idx, val):
    """When two closures share an open upvalue and it is closed, both
    should see the closed value."""
    vm = _fresh_vm()
    _set_stack_int(vm, idx, val)

    uv = _findupval(vm, idx)
    uv.refcnt = 2  # shared by two closures

    cl1 = bclosure()
    cl1.type = BE_CLOSURE
    cl1.nupvals = 1
    cl1.upvals = [uv]

    cl2 = bclosure()
    cl2.type = BE_CLOSURE
    cl2.nupvals = 1
    cl2.upvals = [uv]

    be_upvals_close(vm, idx)

    # Both closures should see the same closed upvalue
    assert cl1.upvals[0] is cl2.upvals[0], "Both closures should share the same upvalue"
    assert _is_closed(uv), "Shared upvalue should be closed"
    assert uv.u_value.v == val, "Closed value should match original stack value"


# ---------------------------------------------------------------------------
# Property 13n: Open upvalue list integrity after multiple operations
# ---------------------------------------------------------------------------
@settings(max_examples=100)
@given(indices=lists(integers(min_value=1, max_value=50), min_size=1, max_size=10))
def test_open_list_integrity(indices):
    """After creating upvalues for distinct stack indices (inserted in
    descending order to match the sorted-list invariant), the open list
    should contain exactly those entries. Closing the highest should
    leave only the remaining ones."""
    vm = _fresh_vm()
    # Use unique indices in descending order — the C implementation
    # maintains the open list sorted by stack level (descending), and
    # _findupval's linear search relies on this ordering.
    unique = sorted(set(indices), reverse=True)

    for idx in unique:
        uv = _findupval(vm, idx)
        uv.refcnt = 1

    assert _count_open_upvals(vm) == len(unique), (
        f"Open list should have {len(unique)} entries, got {_count_open_upvals(vm)}"
    )

    if len(unique) >= 2:
        # Close the highest index
        close_level = max(unique)
        be_upvals_close(vm, close_level)
        remaining = [i for i in unique if i < close_level]
        assert _count_open_upvals(vm) == len(remaining), (
            f"After closing level {close_level}, expected {len(remaining)} "
            f"open upvalues, got {_count_open_upvals(vm)}"
        )


# ---------------------------------------------------------------------------
# Property 13o: upval_value returns correct value for closed upvalues
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(idx=stack_indices, val=int_values)
def test_upval_value_closed(idx, val):
    """upval_value should return the u_value for a closed upvalue."""
    vm = _fresh_vm()
    _set_stack_int(vm, idx, val)

    uv = _findupval(vm, idx)
    uv.refcnt = 1

    be_upvals_close(vm, idx)

    result = upval_value(uv)
    assert result is not None, "upval_value should return u_value for closed upvalue"
    assert result.type == BE_INT
    assert result.v == val
