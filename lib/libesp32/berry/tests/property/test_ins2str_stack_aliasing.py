"""
Regression: _ins2str stack aliasing from direct Python object assignment

_ins2str (called by be_val2str when converting an instance to string)
used direct assignment to push values onto the VM stack:

    vm.stack[vm.top_idx] = dst                        # method
    vm.stack[vm.top_idx] = vm.stack[vm.reg_idx + idx]  # self

In C these are struct copies (*vm->top = value), but in Python they make
two stack slots reference the same bvalue object. A later var_setval to
one slot then silently corrupts the other.

The fix uses var_setval() for both pushes, which copies the type/v fields
into the existing bvalue object at that stack position.

This test verifies that no stack aliasing is introduced by be_dumpvalue
(which triggers _ins2str for instance values).
"""

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

from berry_port.be_api import (
    be_vm_new, be_vm_delete, be_pop, be_isnil, be_pcall,
    be_loadbuffer, be_dumpvalue,
)
from berry_port.berry import be_dostring, BE_OK


def _fresh_vm():
    return be_vm_new()


def _has_stack_aliases(vm, n=20):
    """Check the first n stack slots for aliased bvalue objects.

    Returns a list of (i, j) pairs where vm.stack[i] is vm.stack[j].
    """
    seen = {}
    aliases = []
    for i in range(min(len(vm.stack), n)):
        oid = id(vm.stack[i])
        if oid in seen:
            aliases.append((i, seen[oid]))
        else:
            seen[oid] = i
    return aliases


class TestNoStackAliasingAfterDumpvalue:
    """be_dumpvalue must not introduce bvalue aliasing in the stack."""

    def setup_method(self):
        self.vm = _fresh_vm()

    def teardown_method(self):
        be_vm_delete(self.vm)

    def _load_and_exec(self, code):
        s = code.encode('utf-8').decode('latin-1')
        res = be_loadbuffer(self.vm, "string", s, len(s))
        assert res == 0
        return be_pcall(self.vm, 0)

    def test_no_alias_after_dumpvalue_int64(self):
        """Dumping an int64 instance must not alias stack slots."""
        r = self._load_and_exec('return int64(42)')
        assert r == 0
        assert _has_stack_aliases(self.vm) == [], "alias before dump"
        be_dumpvalue(self.vm, -1)
        assert _has_stack_aliases(self.vm) == [], (
            f"alias after dump: {_has_stack_aliases(self.vm)}"
        )
        be_pop(self.vm, 1)

    def test_no_alias_after_dumpvalue_list(self):
        """Dumping a list instance must not alias stack slots."""
        r = self._load_and_exec('return [1, 2, 3]')
        assert r == 0
        be_dumpvalue(self.vm, -1)
        assert _has_stack_aliases(self.vm) == []
        be_pop(self.vm, 1)

    def test_no_alias_after_dumpvalue_map(self):
        """Dumping a map instance must not alias stack slots."""
        r = self._load_and_exec('return {1: 2}')
        assert r == 0
        be_dumpvalue(self.vm, -1)
        assert _has_stack_aliases(self.vm) == []
        be_pop(self.vm, 1)

    def test_no_alias_after_dumpvalue_bytes(self):
        """Dumping a bytes instance must not alias stack slots."""
        r = self._load_and_exec('return bytes("AABB")')
        assert r == 0
        be_dumpvalue(self.vm, -1)
        assert _has_stack_aliases(self.vm) == []
        be_pop(self.vm, 1)

    def test_no_alias_after_repeated_dumps(self):
        """Multiple dumpvalue calls must not accumulate aliases."""
        for _ in range(10):
            r = self._load_and_exec('return int64(99)')
            assert r == 0
            be_dumpvalue(self.vm, -1)
            aliases = _has_stack_aliases(self.vm)
            assert aliases == [], f"aliases after dump: {aliases}"
            be_pop(self.vm, 1)


class TestDumpvalueDoesNotCorruptNextCall:
    """The original symptom: dumpvalue on call N corrupts call N+1."""

    def setup_method(self):
        self.vm = _fresh_vm()

    def teardown_method(self):
        be_vm_delete(self.vm)

    def _repl_cycle(self, code):
        """Full REPL cycle: load, exec, dump result, pop."""
        s = code.encode('utf-8').decode('latin-1')
        res = be_loadbuffer(self.vm, "string", s, len(s))
        assert res == 0, f"loadbuffer failed for {code!r}"
        r = be_pcall(self.vm, 0)
        if r == 0:
            if not be_isnil(self.vm, -1):
                be_dumpvalue(self.vm, -1)
            be_pop(self.vm, 1)
        return r

    def test_int64_constructor_repeated(self):
        """int64(N) via REPL should work every time."""
        for i in range(5):
            r = self._repl_cycle(f'return int64({i})')
            assert r == 0, f"int64({i}) failed on iteration {i+1}"

    def test_int64_static_then_constructor(self):
        """Static call followed by constructor should both work."""
        assert self._repl_cycle('return int64.fromu32(1)') == 0
        assert self._repl_cycle('return int64(42)') == 0
        assert self._repl_cycle('return int64.fromu32(2)') == 0

    def test_non_int64_instances_after_dump(self):
        """Other instance types should also survive repeated REPL cycles."""
        codes = [
            'return [1, 2]',
            'return bytes("FF")',
            'return {1: 2}',
        ]
        for code in codes:
            for i in range(3):
                r = self._repl_cycle(code)
                assert r == 0, f"{code!r} failed on iteration {i+1}"
