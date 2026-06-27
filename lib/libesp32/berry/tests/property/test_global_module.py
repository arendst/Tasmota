"""
Tests for the 'global' module — verifying that global.a = value
properly registers variables in the VM's global table so they are
visible to the parser (e.g. via compile()).

Regression tests for the bug where native module tables were not
marked GC_CONST, causing be_module_setmember to insert directly
into the module map instead of dispatching to 'setmember'.
"""

import sys
import subprocess
import pytest

BERRY_CMD = [sys.executable, "-m", "berry_port"]


def run_berry(*args, input_text=None, timeout=15):
    """Run berry_port CLI with given args, return CompletedProcess."""
    return subprocess.run(
        [*BERRY_CMD, *args],
        capture_output=True,
        text=True,
        input=input_text,
        timeout=timeout,
    )


class TestGlobalModuleSetmember:
    """Tests for global.X = value registering in the VM global table."""

    def test_global_set_then_compile(self):
        """global.a = 1 should make 'a' visible to compile()."""
        r = run_berry("-e",
            "import global; global.a = 1; compile('print(a)')")
        assert r.returncode == 0

    def test_global_set_then_compile_returns_value(self):
        """compile() should produce a callable that reads the global."""
        r = run_berry("-e",
            "import global; global.a = 42; "
            "var f = compile('return a'); print(f())")
        assert r.returncode == 0
        assert r.stdout.strip() == "42"

    def test_global_set_string_via_compile(self):
        """global.x = 'hello' should be readable via compile()."""
        r = run_berry("-e",
            "import global; global.x = 'hello'; "
            "var f = compile('return x'); print(f())")
        assert r.returncode == 0
        assert r.stdout.strip() == "hello"

    def test_global_set_multiple_vars_via_compile(self):
        """Multiple globals should all be visible to compile()."""
        r = run_berry("-e",
            "import global; global.a = 10; global.b = 20; "
            "var f = compile('return a + b'); print(f())")
        assert r.returncode == 0
        assert r.stdout.strip() == "30"

    def test_global_overwrite_via_compile(self):
        """Overwriting a global should update the value seen by compile()."""
        r = run_berry("-e",
            "import global; global.a = 1; global.a = 99; "
            "var f = compile('return a'); print(f())")
        assert r.returncode == 0
        assert r.stdout.strip() == "99"

    def test_global_contains(self):
        """global.contains() should find variables set via global.x = ..."""
        r = run_berry("-e",
            "import global; global.a = 1; print(global.contains('a'))")
        assert r.returncode == 0
        assert r.stdout.strip() == "true"

    def test_global_member(self):
        """global.member() should retrieve the value of a global."""
        r = run_berry("-e",
            "import global; global.a = 42; print(global.member('a'))")
        assert r.returncode == 0
        assert r.stdout.strip() == "42"

    def test_global_undef(self):
        """global.undef() should remove a global so it's no longer found."""
        r = run_berry("-e",
            "import global; global.a = 1; global.undef('a'); "
            "print(global.contains('a'))")
        assert r.returncode == 0
        assert r.stdout.strip() == "false"

    def test_global_set_function_via_compile(self):
        """compile() should see a global function set via global.x = ..."""
        r = run_berry("-e",
            "import global; global.double = def(x) return x * 2 end; "
            "var f = compile('return double(21)'); print(f())")
        assert r.returncode == 0
        assert r.stdout.strip() == "42"


class TestGlobalModuleCallable:
    """Tests for global() listing all globals."""

    def test_global_callable_returns_instance(self):
        """global() should return a list (type 'instance' in Berry)."""
        r = run_berry("-e",
            "import global; print(type(global()))")
        assert r.returncode == 0
        assert r.stdout.strip() == "instance"

    def test_global_callable_includes_builtins(self):
        """global.contains() should find built-in names like 'print'.

        Note: global() only lists the global table, not builtins.
        Use global.contains() which checks both via be_global_find.
        """
        r = run_berry("-e",
            "import global; "
            "print(global.contains('print'))")
        assert r.returncode == 0
        assert r.stdout.strip() == "true"

    def test_global_callable_includes_user_globals(self):
        """global() should include user-defined globals set via global.x."""
        r = run_berry("-e",
            "import global; global.myvar = 1; var g = global(); "
            "print(g.find('myvar') != nil)")
        assert r.returncode == 0
        assert r.stdout.strip() == "true"

    def test_global_callable_twice(self):
        """Calling global() twice should not crash (refstack reuse)."""
        r = run_berry("-s", "-g", "-e",
            "import global; print(global()); print(global())")
        assert r.returncode == 0
        lines = r.stdout.strip().split("\n")
        assert len(lines) == 2
        assert lines[0] == lines[1]
