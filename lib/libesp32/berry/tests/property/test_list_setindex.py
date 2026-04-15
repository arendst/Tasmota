"""
Tests for list index assignment (setindex) via the Berry VM.

Regression test for a bug where _list_setindex in be_api.py called
be_list.be_list_at() which doesn't exist — the function is defined
in be_object.py and should be used directly from the import.
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
# Basic index assignment
# ---------------------------------------------------------------------------
def test_list_setindex_positive(vm):
    """l[0] = value should work."""
    code = "l = [1, 2, 3]\nl[0] = 42\nassert(l[0] == 42)"
    assert be_dostring(vm, code) == BE_OK


def test_list_setindex_middle(vm):
    """l[1] = value should update the correct element."""
    code = "l = [1, 2, 3]\nl[1] = 99\nassert(l == [1, 99, 3])"
    assert be_dostring(vm, code) == BE_OK


def test_list_setindex_last(vm):
    """Assignment to the last element by positive index."""
    code = "l = [1, 2, 3]\nl[2] = 0\nassert(l == [1, 2, 0])"
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# Negative index assignment (the original crash scenario)
# ---------------------------------------------------------------------------
def test_list_setindex_negative_last(vm):
    """l[-1] = value should set the last element."""
    code = "l = [1, 2, 3]\nl[-1] = 42\nassert(l == [1, 2, 42])"
    assert be_dostring(vm, code) == BE_OK


def test_list_setindex_negative_first(vm):
    """l[-3] on a 3-element list should set index 0."""
    code = "l = [1, 2, 3]\nl[-3] = 0\nassert(l == [0, 2, 3])"
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# Compound assignment through index
# ---------------------------------------------------------------------------
def test_list_setindex_compound(vm):
    """l[-1] += 1 should read and write through setindex."""
    code = "l = [10, 20]\nl[-1] += 5\nassert(l == [10, 25])"
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# setitem method (uses the same path)
# ---------------------------------------------------------------------------
def test_list_setitem_method(vm):
    """list.setitem() goes through the same setindex path."""
    code = "l = [1, 2, 3]\nl.setitem(1, 'x')\nassert(l == [1, 'x', 3])"
    assert be_dostring(vm, code) == BE_OK
