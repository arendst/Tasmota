"""
Tests for the tracestack ip_idx fix.

be_save_stacktrace must store integer instruction-pointer indices
(ip_idx) in bcallsnapshot.ip — not code-list references — so that
_sourceinfo can compute `pc = ip_idx - 1` without a TypeError.

This validates the fix in be_exec.be_save_stacktrace and the
downstream consumers in be_debug (_sourceinfo, _tracestack,
_repair_stack).
"""

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

import io
from contextlib import redirect_stdout

from hypothesis import given, settings, assume
from hypothesis.strategies import integers, lists, sampled_from

from berry_port.be_object import (
    BE_OK, BE_CLOSURE, BE_NTVFUNC, BE_INT,
    bvalue, bvector, bcallframe, bcallsnapshot,
    bgc, bstringtable, bglobaldesc,
    bclosure, bproto, bstring, blineinfo,
    var_setclosure, var_setint, var_isclosure,
)
from berry_port.be_exec import be_save_stacktrace
from berry_port.be_vector import (
    be_stack_init, be_stack_push, be_stack_count, be_vector_at,
)
from berry_port.be_debug import _sourceinfo, _tracestack, _repair_stack


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------
STACK_SIZE = 64


def _make_string(s):
    """Create a minimal bstring with the given Python string."""
    bs = bstring()
    bs._s = s
    bs.slen = len(s)
    return bs


def _make_proto(name="test", source="test.be", lines=None):
    """Create a bproto with optional lineinfo.

    *lines* is a list of (linenumber, endpc) tuples.
    """
    p = bproto()
    p.name = _make_string(name)
    p.source = _make_string(source)
    p.code = [0] * 20  # dummy instructions
    p.codesize = len(p.code)
    if lines:
        p.lineinfo = []
        for ln, epc in lines:
            li = blineinfo()
            li.linenumber = ln
            li.endpc = epc
            p.lineinfo.append(li)
        p.nlineinfo = len(p.lineinfo)
    else:
        p.lineinfo = None
        p.nlineinfo = 0
    return p


def _make_closure(proto=None):
    """Create a bclosure wrapping the given proto."""
    cl = bclosure()
    cl.proto = proto or _make_proto()
    return cl


class MockVM:
    """Minimal VM for tracestack tests."""

    def __init__(self, stack_size=STACK_SIZE):
        self.gc = bgc()
        self.gc.usage = 0

        self.stack = [bvalue() for _ in range(stack_size)]
        self.stacktop = stack_size
        self.top = 0
        self.reg = 0
        self.ip = [0] * 20       # code list reference (a list)
        self.ip_idx = 0           # integer index
        self.cf = None
        self.code = []
        self.errjmp = None

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
        self.compopt = 0
        self.counter_exc = 0
        self.obshook = None


def _fresh_vm():
    return MockVM()


# ---------------------------------------------------------------------------
# Test: be_save_stacktrace stores integer ip_idx, not list references
# ---------------------------------------------------------------------------
@settings(max_examples=50)
@given(ip_val=integers(min_value=1, max_value=200))
def test_save_stacktrace_stores_int_ip(ip_val):
    """Snapshot ip must be an int (ip_idx), not a list."""
    vm = _fresh_vm()

    # Place a closure on the value stack
    cl = _make_closure()
    var_setclosure(vm.stack[0], cl)

    # Push one call frame
    cf = bcallframe()
    cf.func = 0
    cf.top = 1
    cf.reg = 0
    cf.ip = [0] * 20       # list reference — the old buggy value
    cf.ip_idx = ip_val      # integer index — the correct value
    be_stack_push(vm, vm.callstack, cf)

    vm.ip = [0] * 20
    vm.ip_idx = ip_val

    be_save_stacktrace(vm)

    assert be_stack_count(vm.tracestack) == 1
    snap = be_vector_at(vm.tracestack, 0)
    assert isinstance(snap.ip, int), (
        f"snapshot.ip should be int, got {type(snap.ip).__name__}"
    )
    assert snap.ip == ip_val


# ---------------------------------------------------------------------------
# Test: multi-frame stacktrace uses ip_idx from next frame
# ---------------------------------------------------------------------------
@settings(max_examples=50)
@given(
    ip0=integers(min_value=1, max_value=100),
    ip1=integers(min_value=1, max_value=100),
    ip_top=integers(min_value=1, max_value=100),
)
def test_save_stacktrace_multiframe_uses_next_ip_idx(ip0, ip1, ip_top):
    """For non-top frames, snapshot.ip should come from the *next*
    frame's ip_idx, not its ip list."""
    vm = _fresh_vm()

    # Two closures on the stack
    cl0 = _make_closure(_make_proto("f0"))
    cl1 = _make_closure(_make_proto("f1"))
    var_setclosure(vm.stack[0], cl0)
    var_setclosure(vm.stack[1], cl1)

    cf0 = bcallframe()
    cf0.func = 0
    cf0.top = 1
    cf0.reg = 0
    cf0.ip = [0] * 10
    cf0.ip_idx = ip0
    be_stack_push(vm, vm.callstack, cf0)

    cf1 = bcallframe()
    cf1.func = 1
    cf1.top = 2
    cf1.reg = 1
    cf1.ip = [0] * 10
    cf1.ip_idx = ip1
    be_stack_push(vm, vm.callstack, cf1)

    vm.ip = [0] * 10
    vm.ip_idx = ip_top

    be_save_stacktrace(vm)

    assert be_stack_count(vm.tracestack) == 2
    # Frame 0 gets ip from frame 1's ip_idx
    snap0 = be_vector_at(vm.tracestack, 0)
    assert snap0.ip == ip1, f"frame 0 ip should be {ip1}, got {snap0.ip}"
    # Frame 1 (top) gets ip from vm.ip_idx
    snap1 = be_vector_at(vm.tracestack, 1)
    assert snap1.ip == ip_top, f"frame 1 ip should be {ip_top}, got {snap1.ip}"


# ---------------------------------------------------------------------------
# Test: _sourceinfo does not crash with integer ip_idx
# ---------------------------------------------------------------------------
@settings(max_examples=50)
@given(ip_idx=integers(min_value=1, max_value=15))
def test_sourceinfo_accepts_int_ip(ip_idx):
    """_sourceinfo must work with an integer ip_idx without TypeError."""
    proto = _make_proto("myfunc", "input.be", lines=[(10, 5), (20, 15)])
    buf = io.StringIO()
    with redirect_stdout(buf):
        _sourceinfo(proto, ip_idx)
    output = buf.getvalue()
    # Should contain a line number, not crash
    assert ":" in output


# ---------------------------------------------------------------------------
# Test: _sourceinfo would crash with a list (regression guard)
# ---------------------------------------------------------------------------
def test_sourceinfo_rejects_list_ip():
    """Passing a list as ip_idx to _sourceinfo must raise TypeError,
    confirming the old bug path."""
    proto = _make_proto("myfunc", "input.be", lines=[(10, 5)])
    try:
        _sourceinfo(proto, [0, 1, 2])
        assert False, "Expected TypeError for list ip"
    except TypeError:
        pass  # expected


# ---------------------------------------------------------------------------
# Test: _tracestack prints without crashing for closure frames
# ---------------------------------------------------------------------------
def test_tracestack_prints_closure_frame():
    """_tracestack must print source info for closure frames using
    the integer ip stored in the snapshot."""
    vm = _fresh_vm()
    proto = _make_proto("hello", "hello.be", lines=[(42, 10)])
    cl = _make_closure(proto)

    snap = bcallsnapshot()
    snap.func = bvalue()
    var_setclosure(snap.func, cl)
    snap.ip = 5  # integer index
    be_stack_push(vm, vm.tracestack, snap)

    buf = io.StringIO()
    with redirect_stdout(buf):
        _tracestack(vm)
    output = buf.getvalue()
    assert "stack traceback:" in output
    assert "hello" in output


# ---------------------------------------------------------------------------
# Test: _tracestack prints native frames without crashing
# ---------------------------------------------------------------------------
def test_tracestack_prints_native_frame():
    """Native function frames should print '<native>' without touching ip."""
    vm = _fresh_vm()

    snap = bcallsnapshot()
    snap.func = bvalue()
    snap.func.type = BE_NTVFUNC
    snap.func.v = lambda vm: None
    snap.ip = 0
    be_stack_push(vm, vm.tracestack, snap)

    buf = io.StringIO()
    with redirect_stdout(buf):
        _tracestack(vm)
    output = buf.getvalue()
    assert "<native>" in output


# ---------------------------------------------------------------------------
# Test: _repair_stack propagates integer ip correctly
# ---------------------------------------------------------------------------
def test_repair_stack_propagates_int_ip():
    """_repair_stack copies ip from native frames down to the nearest
    closure frame. The ip must remain an integer throughout."""
    vm = _fresh_vm()
    proto = _make_proto("callee", "callee.be", lines=[(1, 20)])
    cl = _make_closure(proto)

    # Frame 0: closure
    snap0 = bcallsnapshot()
    snap0.func = bvalue()
    var_setclosure(snap0.func, cl)
    snap0.ip = 3  # will be overwritten by repair

    # Frame 1: native (carries the real ip)
    snap1 = bcallsnapshot()
    snap1.func = bvalue()
    snap1.func.type = BE_NTVFUNC
    snap1.func.v = lambda vm: None
    snap1.ip = 7

    be_stack_push(vm, vm.tracestack, snap0)
    be_stack_push(vm, vm.tracestack, snap1)

    _repair_stack(vm)

    # The closure frame should now have the native frame's ip
    repaired = be_vector_at(vm.tracestack, 0)
    assert repaired.ip == 7
    assert isinstance(repaired.ip, int)


# ---------------------------------------------------------------------------
# Test: full save + trace round-trip doesn't crash
# ---------------------------------------------------------------------------
def test_save_then_trace_roundtrip():
    """be_save_stacktrace followed by _tracestack must not raise."""
    vm = _fresh_vm()
    proto = _make_proto("roundtrip", "rt.be", lines=[(1, 0), (2, 5), (3, 10)])
    cl = _make_closure(proto)
    var_setclosure(vm.stack[0], cl)

    cf = bcallframe()
    cf.func = 0
    cf.top = 1
    cf.reg = 0
    cf.ip = [0] * 20
    cf.ip_idx = 4
    be_stack_push(vm, vm.callstack, cf)

    vm.ip = [0] * 20
    vm.ip_idx = 4

    be_save_stacktrace(vm)

    buf = io.StringIO()
    with redirect_stdout(buf):
        _repair_stack(vm)
        _tracestack(vm)
    output = buf.getvalue()
    assert "stack traceback:" in output
    assert "roundtrip" in output
