"""
Feature: int64 static method registration

Verifies that fromu32, fromfloat, toint64, fromstring, and frombytes are
registered with the BE_STATIC flag so the VM does not inject `self` as the
first stack argument when called on an instance.

Without BE_STATIC, calling e.g. `a.fromu32(5)` on an int64 instance `a`
would push `a` as stack[1], shifting the real arguments and producing
wrong results or crashes.
"""

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

from berry_port.be_api import be_vm_new, be_vm_delete
from berry_port.berry import be_dostring, BE_OK
from berry_port.be_object import BE_STATIC, BE_NTVFUNC, var_type
from berry_port.be_class import be_class_member
from berry_port.be_string import be_newstr
from berry_port.be_object import bvalue
import berry_port.be_var as be_var


# ============================================================================
# Helpers
# ============================================================================

def _fresh_vm():
    """Create a fresh Berry VM with all libs loaded."""
    return be_vm_new()


def _get_int64_class(vm):
    """Retrieve the int64 bclass object from the VM globals."""
    s = be_newstr(vm, "int64")
    idx = be_var.be_global_find(vm, s)
    assert idx >= 0, "int64 class not found in globals"
    v = be_var.be_global_var(vm, idx)
    from berry_port.be_object import var_isclass, var_toobj
    assert var_isclass(v), "int64 global is not a class"
    return var_toobj(v)


# ============================================================================
# Test: Static flag is set on the correct methods
# ============================================================================

STATIC_METHODS = ["fromu32", "fromfloat", "toint64", "fromstring", "frombytes"]
INSTANCE_METHODS = [
    "init", "deinit", "tostring", "isint", "toint", "tobool",
    "add", "+", "-", "*", "/", "%", "-*",
    "==", "!=", ">", ">=", "<", "<=", "<<", ">>",
    "tobytes", "low32", "high32",
]


class TestStaticFlagRegistration:
    """Verify BE_STATIC is set on static methods and not on instance methods."""

    def test_static_methods_have_static_flag(self):
        vm = _fresh_vm()
        try:
            cls = _get_int64_class(vm)
            for name in STATIC_METHODS:
                dst = bvalue()
                s = be_newstr(vm, name)
                type_ = be_class_member(vm, cls, s, dst)
                assert type_ & BE_STATIC, (
                    f"Method '{name}' should have BE_STATIC flag set, "
                    f"got type={type_:#x}"
                )
                assert (type_ & ~BE_STATIC) == BE_NTVFUNC, (
                    f"Method '{name}' base type should be BE_NTVFUNC, "
                    f"got {type_ & ~BE_STATIC:#x}"
                )
        finally:
            be_vm_delete(vm)

    def test_instance_methods_lack_static_flag(self):
        vm = _fresh_vm()
        try:
            cls = _get_int64_class(vm)
            for name in INSTANCE_METHODS:
                dst = bvalue()
                s = be_newstr(vm, name)
                type_ = be_class_member(vm, cls, s, dst)
                assert not (type_ & BE_STATIC), (
                    f"Method '{name}' should NOT have BE_STATIC flag, "
                    f"got type={type_:#x}"
                )
        finally:
            be_vm_delete(vm)


# ============================================================================
# Test: Static methods called on the class produce correct results
# ============================================================================

class TestStaticMethodsViaClass:
    """Call static methods via int64.method() — basic sanity."""

    def setup_method(self):
        self.vm = _fresh_vm()

    def teardown_method(self):
        be_vm_delete(self.vm)

    def _run(self, code):
        res = be_dostring(self.vm, code)
        assert res == BE_OK, f"Berry code failed: {code}"

    def test_fromu32_single_arg(self):
        self._run('assert(int64.fromu32(5).tostring() == "5")')

    def test_fromu32_two_args(self):
        self._run('assert(int64.fromu32(0, 1).tostring() == "4294967296")')

    def test_fromfloat(self):
        self._run('assert(int64.fromfloat(42.9).tostring() == "42")')

    def test_fromstring(self):
        self._run('assert(int64.fromstring("12345678901234").tostring() == "12345678901234")')

    def test_toint64_int(self):
        self._run('assert(int64.toint64(7).tostring() == "7")')

    def test_toint64_nil(self):
        self._run('assert(int64.toint64(nil) == nil)')

    def test_frombytes(self):
        self._run('assert(int64.frombytes(bytes("0A00000000000000")).tostring() == "10")')


# ============================================================================
# Test: Static methods called on an *instance* produce correct results
#
# This is the critical regression test. Without BE_STATIC, the VM injects
# `self` as stack[1], so e.g. a.fromu32(5) would see `self` where it
# expects the integer argument, producing wrong results.
# ============================================================================

class TestStaticMethodsViaInstance:
    """Call static methods via instance.method() — the bug scenario."""

    def setup_method(self):
        self.vm = _fresh_vm()
        be_dostring(self.vm, 'var a = int64(100)')

    def teardown_method(self):
        be_vm_delete(self.vm)

    def _run(self, code):
        res = be_dostring(self.vm, code)
        assert res == BE_OK, f"Berry code failed: {code}"

    def test_fromu32_via_instance(self):
        self._run('assert(a.fromu32(5).tostring() == "5")')

    def test_fromu32_two_args_via_instance(self):
        self._run('assert(a.fromu32(0, 2).tostring() == "8589934592")')

    def test_fromfloat_via_instance(self):
        self._run('assert(a.fromfloat(3.14).tostring() == "3")')

    def test_fromstring_via_instance(self):
        self._run('assert(a.fromstring("12345678901234").tostring() == "12345678901234")')

    def test_toint64_via_instance(self):
        self._run('assert(a.toint64(55).tostring() == "55")')

    def test_toint64_nil_via_instance(self):
        self._run('assert(a.toint64(nil) == nil)')

    def test_frombytes_via_instance(self):
        self._run('assert(a.frombytes(bytes("FFFFFFFFFFFFFFFF")).tostring() == "-1")')

    def test_instance_not_mutated(self):
        """Static calls on an instance must not alter the instance value."""
        self._run('a.fromu32(999)')
        self._run('a.fromfloat(999.0)')
        self._run('a.fromstring("999")')
        self._run('assert(a.tostring() == "100")')


# ============================================================================
# Test: Instance methods still work correctly after the static fix
# ============================================================================

class TestInstanceMethodsUnaffected:
    """Ensure non-static methods still receive self correctly."""

    def setup_method(self):
        self.vm = _fresh_vm()
        be_dostring(self.vm, 'var a = int64(100)')

    def teardown_method(self):
        be_vm_delete(self.vm)

    def _run(self, code):
        res = be_dostring(self.vm, code)
        assert res == BE_OK, f"Berry code failed: {code}"

    def test_tostring(self):
        self._run('assert(a.tostring() == "100")')

    def test_toint(self):
        self._run('assert(a.toint() == 100)')

    def test_isint(self):
        self._run('assert(a.isint() == true)')

    def test_tobool(self):
        self._run('assert(a.tobool() == true)')

    def test_low32(self):
        self._run('assert(a.low32() == 100)')

    def test_high32(self):
        self._run('assert(a.high32() == 0)')

    def test_tobytes(self):
        self._run('assert(a.tobytes() == bytes("6400000000000000"))')

    def test_arithmetic(self):
        self._run('assert((a + int64(50)).tostring() == "150")')
        self._run('assert((a - int64(30)).tostring() == "70")')
        self._run('assert((-a).tostring() == "-100")')


# ============================================================================
# Test: Repeated REPL-style evaluation with be_dumpvalue
#
# Regression test for a stack aliasing bug in _ins2str where direct
# Python object assignment (vm.stack[i] = vm.stack[j]) caused two stack
# slots to share the same bvalue object. Combined with a parser stack
# leak (missing be_remove in mainfunc), this caused the second REPL
# evaluation of any int64 static method to fail with
# "the 'bytes' object has no method '()'" or "'int' value is not callable".
# ============================================================================

from berry_port.be_api import (
    be_loadbuffer, be_pop, be_isnil, be_pcall, be_dumpvalue
)


class TestReplRepeatedStaticCalls:
    """Repeated REPL-style eval (loadstring + pcall + dumpvalue + pop)."""

    def setup_method(self):
        self.vm = _fresh_vm()

    def teardown_method(self):
        be_vm_delete(self.vm)

    def _repl_eval(self, code):
        """Simulate one REPL evaluation cycle including dumpvalue."""
        s = code.encode('utf-8').decode('latin-1')
        res = be_loadbuffer(self.vm, "string", s, len(s))
        assert res == 0, f"loadbuffer failed: {res}"
        r = be_pcall(self.vm, 0)
        if r == 0:
            if not be_isnil(self.vm, -1):
                be_dumpvalue(self.vm, -1)
            be_pop(self.vm, 1)
        return r

    def test_fromu32_repeated(self):
        for i in range(5):
            r = self._repl_eval('return (int64.fromu32(1, 0))')
            assert r == 0, f"fromu32 failed on iteration {i+1}"

    def test_frombytes_repeated(self):
        for i in range(5):
            r = self._repl_eval(
                'return (int64.frombytes(bytes("070102000000000000"), 1))'
            )
            assert r == 0, f"frombytes failed on iteration {i+1}"

    def test_fromfloat_repeated(self):
        for i in range(5):
            r = self._repl_eval('return (int64.fromfloat(3.14))')
            assert r == 0, f"fromfloat failed on iteration {i+1}"

    def test_fromstring_repeated(self):
        for i in range(5):
            r = self._repl_eval('return (int64.fromstring("42"))')
            assert r == 0, f"fromstring failed on iteration {i+1}"

    def test_toint64_repeated(self):
        for i in range(5):
            r = self._repl_eval('return (int64.toint64(99))')
            assert r == 0, f"toint64 failed on iteration {i+1}"

    def test_mixed_static_calls(self):
        """Interleave different static methods to stress-test stack cleanup."""
        calls = [
            'return (int64.fromu32(1, 0))',
            'return (int64.fromfloat(2.5))',
            'return (int64.fromstring("100"))',
            'return (int64.toint64(7))',
        ]
        for i in range(3):
            for j, code in enumerate(calls):
                r = self._repl_eval(code)
                assert r == 0, f"Failed on round {i+1}, call {j+1}: {code}"

    def test_no_stack_leak(self):
        """Stack top should return to 0 after each REPL cycle."""
        for i in range(5):
            assert self.vm.top_idx == 0, (
                f"Stack leak before iteration {i+1}: top={self.vm.top_idx}"
            )
            self._repl_eval('return (int64.fromu32(1, 0))')
        assert self.vm.top_idx == 0, f"Stack leak at end: top={self.vm.top_idx}"
