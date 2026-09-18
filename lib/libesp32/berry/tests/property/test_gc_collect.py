"""
Tests for GC collect correctness after GC removal simplification.

Validates that:
- gc.collect() is a harmless no-op
- Object creation works without GC list linking
- Short string hash table chaining is preserved
- End-to-end Berry scripts using gc module still work
"""

import sys
import os
import subprocess

import pytest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

from berry_port.be_object import (
    bgc, bstringtable,
    bvalue, bmap, blist, bclass, bmodule,
    BE_STRING, BE_MAP, BE_LIST, BE_CLASS, BE_MODULE,
    GC_WHITE, GC_FIXED,
    gc_iswhite, gc_isfixed, gc_setfixed, var_setnil,
)
from berry_port.be_string import be_string_init, be_newstr, be_newstrn
from berry_port.be_map import be_map_new
from berry_port.be_list import be_list_new

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


# ---------------------------------------------------------------------------
# Minimal mock VM
# ---------------------------------------------------------------------------
class MockVM:
    def __init__(self):
        self.gc = bgc()
        self.gc.usage = 0
        self.strtab = bstringtable()
        self.compopt = 0

def _fresh_vm():
    vm = MockVM()
    be_string_init(vm)
    return vm


# ---------------------------------------------------------------------------
# Object creation works without GC list linking
# ---------------------------------------------------------------------------
class TestObjectCreationNoGCList:
    def test_short_string_creation(self):
        """Short strings are created and interned without GC list linking."""
        vm = _fresh_vm()
        s1 = be_newstr(vm, "hello")
        s2 = be_newstr(vm, "hello")
        # Short strings are interned — same object
        assert s1 is s2
        assert s1.type == BE_STRING

    def test_long_string_creation(self):
        """Long strings are created without GC list linking."""
        vm = _fresh_vm()
        s = be_newstr(vm, "A" * 100)
        assert s.type == BE_STRING

    def test_map_creation(self):
        """Maps are created without GC list linking."""
        vm = _fresh_vm()
        m = be_map_new(vm)
        assert m.type == BE_MAP

    def test_list_creation(self):
        """Lists are created without GC list linking."""
        vm = _fresh_vm()
        lst = be_list_new(vm)
        assert lst.type == BE_LIST

    def test_mixed_object_creation(self):
        """Interleaving different object types works without GC list."""
        vm = _fresh_vm()
        objects = []
        for i in range(10):
            m = be_map_new(vm)
            objects.append(m)
            be_newstr(vm, f"key_{i}")
            lst = be_list_new(vm)
            objects.append(lst)
        # All objects created successfully with correct types
        for obj in objects:
            assert obj.type in (BE_MAP, BE_LIST)

    def test_short_string_hash_chaining_preserved(self):
        """Short strings use .next for hash table chaining, not GC list."""
        vm = _fresh_vm()
        # Create many short strings to exercise hash table chaining
        strings = []
        for i in range(50):
            s = be_newstr(vm, f"s{i}")
            strings.append(s)
        # All strings should be retrievable (interning works)
        for i in range(50):
            s = be_newstr(vm, f"s{i}")
            assert s is strings[i]


# ---------------------------------------------------------------------------
# End-to-end: gc.collect() must not break builtins
# ---------------------------------------------------------------------------
class TestGCCollectBuiltins:
    def test_print_works_after_gc_collect_e_flag(self):
        """print must work after gc.collect() in -e mode."""
        r = run_berry("-s", "-g", "-e",
                       "import gc gc.collect() print('alive')")
        assert r.returncode == 0
        assert "alive" in r.stdout

    def test_print_works_after_gc_collect_repl(self):
        """print must work after gc.collect() in REPL mode."""
        code = "import gc\ngc.collect()\nprint('alive')\n"
        r = run_berry("-s", "-g", input_text=code)
        assert r.returncode == 0
        assert "alive" in r.stdout

    def test_builtins_survive_gc_collect(self):
        """All common builtins must remain accessible after gc.collect()."""
        script = (
            "import gc gc.collect() "
            "assert(type(print) == 'function') "
            "assert(type(int) == 'function') "
            "assert(type(str) == 'function') "
            "assert(type(super) == 'function') "
            "assert(type(classname) == 'function') "
            "assert(type(isinstance) == 'function') "
            "assert(type(compile) == 'function') "
            "print('builtins ok')"
        )
        r = run_berry("-s", "-g", "-e", script)
        assert r.returncode == 0
        assert "builtins ok" in r.stdout

    def test_classes_survive_gc_collect(self):
        """Built-in classes (list, map, range, bytes) must survive gc.collect()."""
        script = (
            "import gc gc.collect() "
            "var l = [1,2,3] "
            "var m = {'a': 1} "
            "var r = 0..5 "
            "var b = bytes() "
            "assert(l.size() == 3) "
            "assert(m.size() == 1) "
            "print('classes ok')"
        )
        r = run_berry("-s", "-g", "-e", script)
        assert r.returncode == 0
        assert "classes ok" in r.stdout

    def test_multiple_gc_collects(self):
        """Multiple gc.collect() calls must not cause issues."""
        script = (
            "import gc "
            "var c = gc.collect "
            "c() c() c() "
            "print('multi ok')"
        )
        r = run_berry("-s", "-g", "-e", script)
        assert r.returncode == 0
        assert "multi ok" in r.stdout

    def test_gc_collect_in_repl_preserves_variables(self):
        """Variables defined before gc.collect() must survive in REPL."""
        code = "var x = 42\nimport gc\ngc.collect()\nprint(x)\n"
        r = run_berry("-s", "-g", input_text=code)
        assert r.returncode == 0
        assert "42" in r.stdout

    def test_keywords_survive_gc_collect_repl(self):
        """Keywords (if, for, def, etc.) must still parse after gc.collect()
        in REPL mode where each line is compiled separately."""
        code = "import gc\ngc.collect()\nif true print('kw ok') end\n"
        r = run_berry("-s", "-g", input_text=code)
        assert r.returncode == 0
        assert "kw ok" in r.stdout
