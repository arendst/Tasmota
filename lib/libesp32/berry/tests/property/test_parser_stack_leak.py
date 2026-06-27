"""
Regression: parser stack leak from missing be_remove in mainfunc

be_parser_source / mainfunc must call be_remove(vm, -3) to pop the proto
pushed by begin_func. A stale stub (vm_has_remove returning False) caused
this call to be skipped, leaking one stack slot per be_loadbuffer call.

The fix imports and calls the real be_remove from be_api.

This test verifies that repeated be_loadbuffer + be_pcall cycles do not
accumulate stale values on the stack.
"""

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

from berry_port.be_api import be_vm_new, be_vm_delete, be_pop, be_pcall, be_loadbuffer


def _fresh_vm():
    return be_vm_new()


class TestParserStackLeak:
    """be_loadbuffer must push exactly 1 item (the closure) onto the stack."""

    def setup_method(self):
        self.vm = _fresh_vm()

    def teardown_method(self):
        be_vm_delete(self.vm)

    def test_loadbuffer_pushes_one_item(self):
        """A single be_loadbuffer should increase top by exactly 1."""
        top_before = self.vm.top_idx
        s = 'return 1'
        res = be_loadbuffer(self.vm, "string", s, len(s))
        assert res == 0
        assert self.vm.top_idx == top_before + 1, (
            f"be_loadbuffer pushed {self.vm.top_idx - top_before} items, expected 1"
        )
        be_pop(self.vm, 1)

    def test_no_leak_after_repeated_load_and_exec(self):
        """top_idx must return to 0 after each loadbuffer + pcall + pop cycle."""
        for i in range(20):
            assert self.vm.top_idx == 0, (
                f"Stack leak before iteration {i+1}: top={self.vm.top_idx}"
            )
            s = 'return 1'
            res = be_loadbuffer(self.vm, "string", s, len(s))
            assert res == 0
            assert self.vm.top_idx == 1
            r = be_pcall(self.vm, 0)
            assert r == 0
            be_pop(self.vm, 1)
        assert self.vm.top_idx == 0

    def test_no_leak_with_complex_expressions(self):
        """Expressions that trigger nested calls should not leak either."""
        exprs = [
            'return 1 + 2',
            'return "hello"',
            'return [1, 2, 3]',
            'return {1: 2}',
            'return int64(42)',
            'return int64.fromu32(1, 0)',
        ]
        for expr in exprs:
            top_before = self.vm.top_idx
            s = expr.encode('utf-8').decode('latin-1')
            res = be_loadbuffer(self.vm, "string", s, len(s))
            assert res == 0
            assert self.vm.top_idx == top_before + 1, (
                f"Leak in loadbuffer for {expr!r}: "
                f"pushed {self.vm.top_idx - top_before}"
            )
            r = be_pcall(self.vm, 0)
            assert r == 0
            be_pop(self.vm, 1)
            assert self.vm.top_idx == top_before, (
                f"Leak after eval of {expr!r}: "
                f"top={self.vm.top_idx}, expected {top_before}"
            )

    def test_no_leak_with_statements(self):
        """Statements (no return value) should also not leak."""
        for i in range(10):
            assert self.vm.top_idx == 0
            s = f'var x{i} = {i}'
            res = be_loadbuffer(self.vm, "string", s, len(s))
            assert res == 0
            assert self.vm.top_idx == 1
            r = be_pcall(self.vm, 0)
            assert r == 0
            be_pop(self.vm, 1)
        assert self.vm.top_idx == 0
