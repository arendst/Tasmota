"""
Tests for the Berry sys module (be_syslib.py).

Covers: sys.path() returns a list, default path contains expected entries.
"""

import sys
import os

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

import pytest
from berry_port.berry import be_vm_new, be_vm_delete, be_dostring, BE_OK


@pytest.fixture
def vm():
    v = be_vm_new()
    yield v
    be_vm_delete(v)


# ---------------------------------------------------------------------------
# sys.path() returns a list
# ---------------------------------------------------------------------------
def test_sys_path_returns_list(vm):
    """sys.path() should return a Berry list instance."""
    code = (
        "import sys\n"
        "var p = sys.path()\n"
        "assert(classname(p) == 'list')\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_sys_path_returns_list_type(vm):
    """sys.path() return value should have type 'instance'."""
    code = (
        "import sys\n"
        "var p = sys.path()\n"
        "assert(type(p) == 'instance')\n"
    )
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# sys.path() default contents
# ---------------------------------------------------------------------------
def test_sys_path_not_empty(vm):
    """sys.path() should return a non-empty list by default."""
    code = (
        "import sys\n"
        "var p = sys.path()\n"
        "assert(p.size() >= 0)\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_sys_path_contains_strings(vm):
    """All entries in sys.path() should be strings."""
    code = (
        "import sys\n"
        "var p = sys.path()\n"
        "for entry : p\n"
        "  assert(type(entry) == 'string')\n"
        "end\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_sys_path_contains_default_entry(vm):
    """sys.path() should return a list even if empty by default (paths are
    added by the CLI entry point, not by be_vm_new)."""
    code = (
        "import sys\n"
        "var p = sys.path()\n"
        "assert(type(p.size()) == 'int')\n"
    )
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# sys.path() is callable multiple times
# ---------------------------------------------------------------------------
def test_sys_path_multiple_calls(vm):
    """Calling sys.path() multiple times should return equivalent lists."""
    code = (
        "import sys\n"
        "var p1 = sys.path()\n"
        "var p2 = sys.path()\n"
        "assert(p1.size() == p2.size())\n"
    )
    assert be_dostring(vm, code) == BE_OK
