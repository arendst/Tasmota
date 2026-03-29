"""
Feature: berry-python-port, Property 18: Native function stack layout

For any native function call with N arguments from Berry code, the virtual
stack should contain the arguments at indices 1 through N relative to the
function's base register, matching the C implementation's layout.

This test validates the stack layout seen by native functions when called
through the Berry VM's do_ntvfunc / precall path, and verifies that the
public API functions (be_top, be_toint, be_toreal, be_tobool, be_tostring,
be_isint, etc.) correctly access arguments by 1-based positive index.

Validates: Requirements 23.3
"""

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

from hypothesis import given, settings, assume
from hypothesis.strategies import (
    integers, floats, booleans, lists, sampled_from, one_of, just,
    composite, text,
)

from berry_port.be_object import (
    BE_NIL, BE_INT, BE_REAL, BE_BOOL, BE_NTVFUNC, BE_STRING,
    bvalue, bvector, bcallframe, bgc, bstringtable, bglobaldesc,
    bmoduledesc,
    var_setint, var_setreal, var_setbool, var_setnil, var_setntvfunc,
    var_setstr, var_type, var_toint, var_toreal, var_tobool,
)
from berry_port.be_vector import (
    be_stack_init, be_stack_push, be_stack_count,
)
from berry_port.be_api import (
    be_top, be_toint, be_toreal, be_tobool, be_tostring,
    be_isnil, be_isbool, be_isint, be_isreal, be_isstring,
    be_indexof, be_returnvalue, be_returnnilvalue, be_pushint,
    be_pushnil,
)
from berry_port.be_vm import precall, ret_native, do_ntvfunc
from berry_port.berry_conf import BE_STACK_FREE_MIN


# ---------------------------------------------------------------------------
# Minimal mock VM for native function stack layout tests
# ---------------------------------------------------------------------------
STACK_SIZE = 200


class MockVM:
    """Minimal VM with the fields needed by precall / ret_native / API."""

    def __init__(self, stack_size=STACK_SIZE):
        self.gc = bgc()
        self.gc.usage = 0

        self.stack = [bvalue() for _ in range(stack_size)]
        self.stacktop_idx = stack_size
        self.top_idx = 0
        self.reg_idx = 0
        self.cf = None

        self.callstack = bvector()
        be_stack_init(self, self.callstack, 1)

        self.exceptstack = bvector()
        be_stack_init(self, self.exceptstack, 1)

        self.refstack = bvector()
        be_stack_init(self, self.refstack, 1)

        self.tracestack = bvector()
        be_stack_init(self, self.tracestack, 1)

        self.strtab = bstringtable()
        self.gbldesc = bglobaldesc()
        self.module = bmoduledesc()
        self.compopt = 0
        self.errjmp = None
        self.obshook = None
        self.counter_ins = 0
        self.counter_enter = 0
        self.counter_call = 0
        self.counter_get = 0
        self.counter_set = 0
        self.counter_get_global = 0
        self.counter_try = 0
        self.counter_exc = 0


def _fresh_vm(stack_size=STACK_SIZE):
    return MockVM(stack_size)


# ---------------------------------------------------------------------------
# Strategies for Berry argument values
# ---------------------------------------------------------------------------
ARG_INT = integers(min_value=-(2**31), max_value=2**31 - 1)
ARG_REAL = floats(allow_nan=False, allow_infinity=False,
                  min_value=-1e10, max_value=1e10)
ARG_BOOL = booleans()


@composite
def berry_arg(draw):
    """Generate a (type_tag, value) pair for a Berry argument."""
    choice = draw(sampled_from(['int', 'real', 'bool', 'nil']))
    if choice == 'int':
        return ('int', draw(ARG_INT))
    elif choice == 'real':
        return ('real', draw(ARG_REAL))
    elif choice == 'bool':
        return ('bool', draw(ARG_BOOL))
    else:
        return ('nil', None)


def _set_arg(slot, tag, value):
    """Set a bvalue slot to the given typed value."""
    if tag == 'int':
        var_setint(slot, value)
    elif tag == 'real':
        var_setreal(slot, value)
    elif tag == 'bool':
        var_setbool(slot, value)
    else:
        var_setnil(slot)


def _setup_native_call(vm, args):
    """Simulate the stack layout for a native function call with given args.

    Stack layout after precall:
        [func_slot][arg0][arg1]...[argN-1]
         ^func      ^reg (= func+1)

    vm.reg_idx points to arg0 (func_slot + 1).
    be_top() returns argc.
    be_indexof(vm, k) for k=1..N returns reg_idx + k - 1 = the k-th arg.
    """
    argc = len(args)
    func_idx = vm.top_idx

    # Push the function value
    var_setntvfunc(vm.stack[vm.top_idx], lambda vm: None)
    vm.top_idx += 1

    # Push arguments
    for tag, value in args:
        _set_arg(vm.stack[vm.top_idx], tag, value)
        vm.top_idx += 1

    # precall sets up the call frame
    precall(vm, func_idx, argc, 0)

    return func_idx


# ---------------------------------------------------------------------------
# Property 18a: be_top returns argc inside a native function
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(args=lists(berry_arg(), min_size=0, max_size=10))
def test_be_top_returns_argc(args):
    """Inside a native function, be_top(vm) must return the number of
    arguments passed (argc)."""
    vm = _fresh_vm()
    _setup_native_call(vm, args)

    assert be_top(vm) == len(args), (
        f"be_top should return {len(args)}, got {be_top(vm)}"
    )


# ---------------------------------------------------------------------------
# Property 18b: Arguments accessible at 1-based positive indices
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(args=lists(berry_arg(), min_size=1, max_size=10))
def test_args_at_one_based_indices(args):
    """Arguments must be accessible at 1-based indices: index 1 = first arg,
    index 2 = second arg, etc."""
    vm = _fresh_vm()
    _setup_native_call(vm, args)

    for i, (tag, value) in enumerate(args):
        idx = i + 1  # 1-based
        abs_idx = be_indexof(vm, idx)
        slot = vm.stack[abs_idx]

        if tag == 'int':
            assert var_type(slot) == BE_INT
            assert var_toint(slot) == value
        elif tag == 'real':
            assert var_type(slot) == BE_REAL
            assert var_toreal(slot) == value
        elif tag == 'bool':
            assert var_type(slot) == BE_BOOL
            assert var_tobool(slot) == bool(value)
        else:
            assert var_type(slot) == BE_NIL


# ---------------------------------------------------------------------------
# Property 18c: API type-check functions match argument types
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(args=lists(berry_arg(), min_size=1, max_size=8))
def test_api_type_checks_match(args):
    """be_isint, be_isreal, be_isbool, be_isnil must correctly identify
    the type of each argument at its 1-based index."""
    vm = _fresh_vm()
    _setup_native_call(vm, args)

    for i, (tag, _) in enumerate(args):
        idx = i + 1
        if tag == 'int':
            assert be_isint(vm, idx), f"Arg {idx} should be int"
            assert not be_isreal(vm, idx)
            assert not be_isbool(vm, idx)
            assert not be_isnil(vm, idx)
        elif tag == 'real':
            assert be_isreal(vm, idx), f"Arg {idx} should be real"
            assert not be_isint(vm, idx)
            assert not be_isbool(vm, idx)
            assert not be_isnil(vm, idx)
        elif tag == 'bool':
            assert be_isbool(vm, idx), f"Arg {idx} should be bool"
            assert not be_isint(vm, idx)
            assert not be_isreal(vm, idx)
            assert not be_isnil(vm, idx)
        else:
            assert be_isnil(vm, idx), f"Arg {idx} should be nil"
            assert not be_isint(vm, idx)
            assert not be_isreal(vm, idx)
            assert not be_isbool(vm, idx)


# ---------------------------------------------------------------------------
# Property 18d: API value retrieval matches pushed values
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(args=lists(berry_arg(), min_size=1, max_size=8))
def test_api_value_retrieval(args):
    """be_toint, be_toreal, be_tobool must return the correct value
    for each argument at its 1-based index."""
    vm = _fresh_vm()
    _setup_native_call(vm, args)

    for i, (tag, value) in enumerate(args):
        idx = i + 1
        if tag == 'int':
            assert be_toint(vm, idx) == value
        elif tag == 'real':
            assert be_toreal(vm, idx) == value
        elif tag == 'bool':
            assert be_tobool(vm, idx) == bool(value)


# ---------------------------------------------------------------------------
# Property 18e: Negative indices access from top of stack
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(args=lists(berry_arg(), min_size=1, max_size=8))
def test_negative_indices_from_top(args):
    """Negative index -1 must refer to the last argument, -2 to the
    second-to-last, etc."""
    vm = _fresh_vm()
    _setup_native_call(vm, args)

    n = len(args)
    for i, (tag, value) in enumerate(args):
        neg_idx = -(n - i)  # -n for first arg, -1 for last arg
        abs_from_neg = be_indexof(vm, neg_idx)
        abs_from_pos = be_indexof(vm, i + 1)
        assert abs_from_neg == abs_from_pos, (
            f"Negative index {neg_idx} should resolve to same slot as "
            f"positive index {i + 1}"
        )


# ---------------------------------------------------------------------------
# Property 18f: Function slot is at reg_idx - 1
# ---------------------------------------------------------------------------
@settings(max_examples=100)
@given(args=lists(berry_arg(), min_size=0, max_size=8))
def test_function_slot_position(args):
    """The function value must be at vm.stack[cf.func], which is
    reg_idx - 1."""
    vm = _fresh_vm()
    func_idx = _setup_native_call(vm, args)

    assert vm.cf.func == func_idx
    assert vm.reg_idx == func_idx + 1
    func_val = vm.stack[vm.cf.func]
    assert var_type(func_val) == BE_NTVFUNC


# ---------------------------------------------------------------------------
# Property 18g: ret_native restores caller state
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(args=lists(berry_arg(), min_size=0, max_size=8))
def test_ret_native_restores_state(args):
    """After ret_native, reg_idx must be restored to the caller's value.
    top_idx is restored to cf.top, which is the top at the moment precall
    was invoked (i.e. after the caller pushed func + args)."""
    vm = _fresh_vm()
    saved_reg = vm.reg_idx

    _setup_native_call(vm, args)

    # precall saved the caller's top (func_idx + 1 + argc) into cf.top
    expected_top = vm.cf.top

    # Verify we're in the native call frame
    assert vm.reg_idx != saved_reg or len(args) == 0

    ret_native(vm)

    assert vm.reg_idx == saved_reg, (
        f"reg_idx not restored: expected {saved_reg}, got {vm.reg_idx}"
    )
    assert vm.top_idx == expected_top, (
        f"top_idx not restored to cf.top: expected {expected_top}, "
        f"got {vm.top_idx}"
    )


# ---------------------------------------------------------------------------
# Property 18h: be_returnvalue copies top-of-stack to function slot
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(
    args=lists(berry_arg(), min_size=0, max_size=5),
    ret_val=integers(min_value=-1000, max_value=1000),
)
def test_returnvalue_copies_to_func_slot(args, ret_val):
    """be_returnvalue must copy the value at top-1 to the function slot
    (cf.func), which is where the caller expects the return value."""
    vm = _fresh_vm()
    func_idx = _setup_native_call(vm, args)

    # Native function pushes a return value onto the stack
    be_pushint(vm, ret_val)
    be_returnvalue(vm)

    # The return value should now be at the function slot
    ret_slot = vm.stack[func_idx]
    assert var_type(ret_slot) == BE_INT
    assert var_toint(ret_slot) == ret_val


# ---------------------------------------------------------------------------
# Property 18i: be_returnnilvalue sets function slot to nil
# ---------------------------------------------------------------------------
@settings(max_examples=100)
@given(args=lists(berry_arg(), min_size=0, max_size=5))
def test_returnnilvalue_sets_nil(args):
    """be_returnnilvalue must set the function slot to nil."""
    vm = _fresh_vm()
    func_idx = _setup_native_call(vm, args)

    be_returnnilvalue(vm)

    ret_slot = vm.stack[func_idx]
    assert var_type(ret_slot) == BE_NIL


# ---------------------------------------------------------------------------
# Property 18j: do_ntvfunc full round-trip preserves stack
# ---------------------------------------------------------------------------
@settings(max_examples=100)
@given(
    args=lists(berry_arg(), min_size=0, max_size=5),
    ret_val=integers(min_value=-1000, max_value=1000),
)
def test_do_ntvfunc_roundtrip(args, ret_val):
    """A full do_ntvfunc call (precall → native func → ret_native) must
    leave the return value at the function slot and restore reg_idx.
    top_idx is restored to cf.top (the caller's top when precall ran)."""
    vm = _fresh_vm()
    saved_reg = vm.reg_idx

    func_idx = vm.top_idx

    # The native function that will be called
    def my_native(vm_arg):
        # Verify argc
        assert be_top(vm_arg) == len(args)
        # Verify each argument
        for i, (tag, value) in enumerate(args):
            idx = i + 1
            if tag == 'int':
                assert be_isint(vm_arg, idx)
                assert be_toint(vm_arg, idx) == value
            elif tag == 'real':
                assert be_isreal(vm_arg, idx)
                assert be_toreal(vm_arg, idx) == value
            elif tag == 'bool':
                assert be_isbool(vm_arg, idx)
                assert be_tobool(vm_arg, idx) == bool(value)
            else:
                assert be_isnil(vm_arg, idx)
        # Push return value
        be_pushint(vm_arg, ret_val)
        be_returnvalue(vm_arg)

    # Set up the stack: [func][arg0][arg1]...
    var_setntvfunc(vm.stack[vm.top_idx], my_native)
    vm.top_idx += 1
    for tag, value in args:
        _set_arg(vm.stack[vm.top_idx], tag, value)
        vm.top_idx += 1

    # This is the top the caller had when do_ntvfunc/precall runs
    expected_top = vm.top_idx

    # Execute via do_ntvfunc
    pos = func_idx - vm.reg_idx
    do_ntvfunc(vm, pos, len(args))

    # Stack should be restored
    assert vm.reg_idx == saved_reg
    assert vm.top_idx == expected_top, (
        f"top_idx should be {expected_top}, got {vm.top_idx}"
    )

    # Return value at function slot
    ret_slot = vm.stack[func_idx]
    assert var_type(ret_slot) == BE_INT
    assert var_toint(ret_slot) == ret_val
