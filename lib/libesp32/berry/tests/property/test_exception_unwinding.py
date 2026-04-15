"""
Feature: berry-python-port, Property 16: Exception unwinding

For any Berry program containing try/except blocks and raise statements,
the Python port should unwind the call stack to the correct exception
handler and deliver the same exception value and argument as the C
implementation.

This test validates the exception handling primitives at the be_exec.py
level: be_execprotected, be_throw, be_except_block_setup,
be_except_block_resume, and be_except_block_close.

Validates: Requirements 18.4
"""

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

from hypothesis import given, settings, assume
from hypothesis.strategies import integers, lists, booleans, sampled_from

from berry_port.be_object import (
    BE_OK, BE_EXIT, BE_MALLOC_FAIL, BE_EXCEPTION,
    BE_SYNTAX_ERROR, BE_EXEC_ERROR, BE_IO_ERROR,
    BE_INT, BE_STRING, BE_NIL,
    bvalue, bvector, bcallframe, bexecptframe, bgc, bstringtable,
    bglobaldesc,
)
from berry_port.be_exec import (
    BerryException, be_throw, be_execprotected,
    be_except_block_setup, be_except_block_close,
    _vm_state_save, _vm_state_restore, _vmstate,
)
from berry_port.be_vector import (
    be_stack_init, be_stack_push, be_stack_pop, be_stack_top,
    be_stack_count, be_vector_at,
)


# ---------------------------------------------------------------------------
# Minimal mock VM for exception handling tests
# ---------------------------------------------------------------------------
STACK_SIZE = 100


class MockVM:
    """Minimal VM with the fields needed by be_exec exception handling."""

    def __init__(self, stack_size=STACK_SIZE):
        # GC must be initialized first — be_malloc accesses vm.gc.usage
        self.gc = bgc()
        self.gc.usage = 0

        # Value stack (Python list of bvalue, indexed by int)
        self.stack = [bvalue() for _ in range(stack_size)]
        self.stacktop = stack_size
        self.top = 0
        self.reg = 0
        self.ip = 0
        self.ip_idx = 0
        self.cf = None
        self.code = []  # instruction array (not used directly here)

        # errjmp chain (replaces setjmp/longjmp)
        self.errjmp = None

        # Call stack
        self.callstack = bvector()
        be_stack_init(self, self.callstack, 1)

        # Exception stack
        self.exceptstack = bvector()
        be_stack_init(self, self.exceptstack, 1)

        # Reference stack
        self.refstack = bvector()
        be_stack_init(self, self.refstack, 1)

        # Trace stack
        self.tracestack = bvector()
        be_stack_init(self, self.tracestack, 1)

        # String table (minimal)
        self.strtab = bstringtable()

        # Globals
        self.gbldesc = bglobaldesc()

        # Compiler options
        self.compopt = 0

        # Perf counters
        self.counter_exc = 0

        # Observer hook
        self.obshook = None


def _fresh_vm(stack_size=STACK_SIZE):
    return MockVM(stack_size)


# ---------------------------------------------------------------------------
# Error codes used in tests
# ---------------------------------------------------------------------------
BERRY_ERROR_CODES = [BE_EXCEPTION, BE_EXEC_ERROR, BE_SYNTAX_ERROR,
                     BE_IO_ERROR, BE_EXIT, BE_MALLOC_FAIL]


# ---------------------------------------------------------------------------
# Property 16a: be_execprotected catches BerryException and returns error code
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(errorcode=sampled_from(BERRY_ERROR_CODES))
def test_execprotected_catches_exception(errorcode):
    """be_execprotected must catch BerryException raised by be_throw
    and return the error code. The errjmp chain must be restored."""
    vm = _fresh_vm()
    prev_errjmp = vm.errjmp

    def thrower(vm, data):
        be_throw(vm, data)

    result = be_execprotected(vm, thrower, errorcode)

    assert result == errorcode, (
        f"Expected error code {errorcode}, got {result}"
    )
    # errjmp must be restored to previous value
    assert vm.errjmp is prev_errjmp, (
        "errjmp not restored after be_execprotected"
    )


# ---------------------------------------------------------------------------
# Property 16b: be_execprotected returns 0 on success
# ---------------------------------------------------------------------------
def test_execprotected_returns_zero_on_success():
    """When the protected function completes normally,
    be_execprotected must return 0."""
    vm = _fresh_vm()

    def noop(vm, data):
        pass

    result = be_execprotected(vm, noop, None)
    assert result == 0, f"Expected 0, got {result}"


# ---------------------------------------------------------------------------
# Property 16c: Nested be_execprotected restores errjmp chain correctly
# ---------------------------------------------------------------------------
@settings(max_examples=100)
@given(
    outer_throws=booleans(),
    inner_throws=booleans(),
    inner_code=sampled_from(BERRY_ERROR_CODES),
    outer_code=sampled_from(BERRY_ERROR_CODES),
)
def test_nested_execprotected_errjmp_chain(outer_throws, inner_throws,
                                           inner_code, outer_code):
    """Nested be_execprotected calls must correctly save and restore
    the errjmp chain regardless of which level throws."""
    vm = _fresh_vm()
    original_errjmp = vm.errjmp

    def inner_func(vm, data):
        if inner_throws:
            be_throw(vm, inner_code)

    def outer_func(vm, data):
        inner_result = be_execprotected(vm, inner_func, None)
        # After inner returns, errjmp should be at outer's level
        if outer_throws:
            be_throw(vm, outer_code)

    result = be_execprotected(vm, outer_func, None)

    # errjmp must be fully restored
    assert vm.errjmp is original_errjmp, (
        "errjmp not restored after nested be_execprotected"
    )

    if outer_throws:
        assert result == outer_code
    elif not inner_throws:
        assert result == 0


# ---------------------------------------------------------------------------
# Property 16d: be_except_block_setup pushes frame and saves errjmp
# ---------------------------------------------------------------------------
@settings(max_examples=100)
@given(n_blocks=integers(min_value=1, max_value=10))
def test_except_block_setup_pushes_frames(n_blocks):
    """Each call to be_except_block_setup must push one frame onto
    the exception stack and chain the errjmp."""
    vm = _fresh_vm()
    original_errjmp = vm.errjmp

    for i in range(n_blocks):
        count_before = be_stack_count(vm.exceptstack)
        be_except_block_setup(vm)
        count_after = be_stack_count(vm.exceptstack)

        assert count_after == count_before + 1, (
            f"Exception stack count should increase by 1: "
            f"{count_before} -> {count_after}"
        )

    # errjmp should now point to the last pushed frame
    assert vm.errjmp is not original_errjmp or n_blocks == 0
    assert be_stack_count(vm.exceptstack) == n_blocks


# ---------------------------------------------------------------------------
# Property 16e: be_except_block_close pops frames and restores errjmp
# ---------------------------------------------------------------------------
@settings(max_examples=100)
@given(n_blocks=integers(min_value=1, max_value=10))
def test_except_block_close_pops_frames(n_blocks):
    """be_except_block_close(count) must pop count frames and restore
    the errjmp to the level before those frames were pushed."""
    vm = _fresh_vm()
    original_errjmp = vm.errjmp

    # Push n_blocks exception frames
    for _ in range(n_blocks):
        be_except_block_setup(vm)

    assert be_stack_count(vm.exceptstack) == n_blocks

    # Close all at once
    be_except_block_close(vm, n_blocks)

    assert be_stack_count(vm.exceptstack) == 0, (
        f"Exception stack should be empty, got {be_stack_count(vm.exceptstack)}"
    )
    assert vm.errjmp is original_errjmp, (
        "errjmp not restored to original after closing all blocks"
    )


# ---------------------------------------------------------------------------
# Property 16f: Partial close restores to correct intermediate errjmp
# ---------------------------------------------------------------------------
@settings(max_examples=100)
@given(
    total=integers(min_value=2, max_value=10),
    close_count=integers(min_value=1, max_value=10),
)
def test_except_block_partial_close(total, close_count):
    """Closing fewer blocks than were pushed must restore errjmp to
    the level of the remaining deepest block."""
    assume(close_count <= total)
    vm = _fresh_vm()

    # Track errjmp at each level
    errjmp_levels = [vm.errjmp]
    for _ in range(total):
        be_except_block_setup(vm)
        errjmp_levels.append(vm.errjmp)

    remaining = total - close_count
    be_except_block_close(vm, close_count)

    assert be_stack_count(vm.exceptstack) == remaining, (
        f"Expected {remaining} frames, got {be_stack_count(vm.exceptstack)}"
    )
    # errjmp should be at the level of the remaining-th frame
    assert vm.errjmp is errjmp_levels[remaining], (
        "errjmp not at correct intermediate level after partial close"
    )


# ---------------------------------------------------------------------------
# Property 16g: Exception frame captures correct call stack depth
# ---------------------------------------------------------------------------
@settings(max_examples=100)
@given(n_callframes=integers(min_value=0, max_value=20))
def test_except_frame_captures_call_depth(n_callframes):
    """The exception frame's depth field must equal the call stack
    count at the time be_except_block_setup was called."""
    vm = _fresh_vm()

    # Push some call frames to simulate nested calls
    for i in range(n_callframes):
        cf = bcallframe()
        cf.func = i
        cf.top = i + 1
        cf.reg = i
        cf.ip = i * 10
        be_stack_push(vm, vm.callstack, cf)

    be_except_block_setup(vm)

    frame = be_vector_at(vm.exceptstack, 0)
    assert frame.depth == n_callframes, (
        f"Frame depth should be {n_callframes}, got {frame.depth}"
    )


# ---------------------------------------------------------------------------
# Property 16h: Exception frame captures instruction pointer
# ---------------------------------------------------------------------------
@settings(max_examples=100)
@given(ip_value=integers(min_value=0, max_value=10000))
def test_except_frame_captures_ip(ip_value):
    """The exception frame's ip field must equal vm.ip at the time
    be_except_block_setup was called."""
    vm = _fresh_vm()
    vm.ip = ip_value
    vm.ip_idx = ip_value

    be_except_block_setup(vm)

    frame = be_vector_at(vm.exceptstack, 0)
    assert frame.ip == ip_value, (
        f"Frame ip should be {ip_value}, got {frame.ip}"
    )


# ---------------------------------------------------------------------------
# Property 16i: vm_state_save/restore preserves VM state on error
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(
    top=integers(min_value=0, max_value=50),
    reg=integers(min_value=0, max_value=50),
    errorcode=sampled_from(BERRY_ERROR_CODES),
)
def test_vm_state_save_restore_roundtrip(top, reg, errorcode):
    """_vm_state_save captures top/reg/depth/refcount, and
    _vm_state_restore restores reg and trims callstack on error."""
    vm = _fresh_vm()
    vm.top = top
    vm.reg = reg

    # Push a couple of call frames
    for i in range(3):
        cf = bcallframe()
        cf.func = 0
        cf.top = top
        cf.reg = reg
        cf.ip = 0
        be_stack_push(vm, vm.callstack, cf)

    state = _vmstate()
    _vm_state_save(vm, state)

    assert state.top == top
    assert state.reg == reg
    assert state.depth == 3

    # Simulate some state changes (as if an error occurred mid-execution)
    vm.reg = reg + 5
    vm.top = top + 10
    # Push extra call frames
    for _ in range(2):
        cf = bcallframe()
        cf.func = 0
        cf.top = top + 10
        cf.reg = reg + 5
        cf.ip = 0
        be_stack_push(vm, vm.callstack, cf)

    assert be_stack_count(vm.callstack) == 5

    # Restore should bring reg back and trim callstack
    _vm_state_restore(vm, state, errorcode)

    assert vm.reg == reg, (
        f"reg not restored: expected {reg}, got {vm.reg}"
    )
    assert be_stack_count(vm.callstack) == 3, (
        f"callstack not trimmed: expected 3, got {be_stack_count(vm.callstack)}"
    )


# ---------------------------------------------------------------------------
# Property 16j: BerryException carries the correct error code
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(errorcode=sampled_from(BERRY_ERROR_CODES))
def test_berry_exception_carries_errorcode(errorcode):
    """BerryException must carry the exact error code passed to be_throw."""
    vm = _fresh_vm()
    vm.errjmp = object()  # sentinel so be_throw doesn't abort

    caught_code = None
    try:
        be_throw(vm, errorcode)
    except BerryException as e:
        caught_code = e.errorcode

    assert caught_code == errorcode, (
        f"Expected errorcode {errorcode}, caught {caught_code}"
    )


# ---------------------------------------------------------------------------
# Property 16k: Multiple sequential protected calls are independent
# ---------------------------------------------------------------------------
@settings(max_examples=100)
@given(
    codes=lists(
        sampled_from([0] + BERRY_ERROR_CODES),
        min_size=1, max_size=10,
    )
)
def test_sequential_protected_calls_independent(codes):
    """Each be_execprotected call must be independent — a failure in one
    must not affect subsequent calls."""
    vm = _fresh_vm()
    original_errjmp = vm.errjmp

    for code in codes:
        def func(vm, data, _code=code):
            if _code != 0:
                be_throw(vm, _code)

        result = be_execprotected(vm, func, None)

        if code == 0:
            assert result == 0
        else:
            assert result == code

        # errjmp must always be restored
        assert vm.errjmp is original_errjmp


# ---------------------------------------------------------------------------
# Property 16l: Exception frame refcount is captured correctly
# ---------------------------------------------------------------------------
@settings(max_examples=100)
@given(refcount=integers(min_value=0, max_value=50))
def test_except_frame_captures_refcount(refcount):
    """The exception frame's refcount field must equal vm.refstack.count
    at the time be_except_block_setup was called."""
    vm = _fresh_vm()

    # Simulate some reference stack entries
    for _ in range(refcount):
        be_stack_push(vm, vm.refstack, None)

    be_except_block_setup(vm)

    frame = be_vector_at(vm.exceptstack, 0)
    assert frame.refcount == refcount, (
        f"Frame refcount should be {refcount}, got {frame.refcount}"
    )
