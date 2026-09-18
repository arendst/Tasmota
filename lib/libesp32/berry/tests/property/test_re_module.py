"""
Tests for the Berry re (regex) module (be_relib.py).

Covers: re.search, re.match, re.match2, re.searchall, re.matchall,
        re.split, re.compile, re.compilebytes, re.dump,
        re_pattern.search, re_pattern.match, re_pattern.match2,
        re_pattern.searchall, re_pattern.matchall, re_pattern.split
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
# re.search
# ---------------------------------------------------------------------------
def test_re_search_basic(vm):
    code = (
        'import re\n'
        'var r = re.search("a.*?b(z+)", "zaaaabbbccbbzzzee")\n'
        'assert(r[0] == "aaaabbbccbbzzz")\n'
        'assert(r[1] == "zzz")\n'
    )
    assert be_dostring(vm, code) == BE_OK


def test_re_search_no_match(vm):
    code = 'import re\nassert(re.search("xyz", "abc") == nil)\n'
    assert be_dostring(vm, code) == BE_OK


def test_re_search_with_offset(vm):
    code = (
        'import re\n'
        'var r = re.search("[0-9]+", "abc123def456", 6)\n'
        'assert(r[0] == "456")\n'
    )
    assert be_dostring(vm, code) == BE_OK


def test_re_search_offset_past_end(vm):
    code = 'import re\nassert(re.search("a", "abc", 100) == nil)\n'
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# re.match
# ---------------------------------------------------------------------------
def test_re_match_basic(vm):
    code = (
        'import re\n'
        'var r = re.match("a.*?b(z+)", "aaaabbbccbbzzzee")\n'
        'assert(r[0] == "aaaabbbccbbzzz")\n'
        'assert(r[1] == "zzz")\n'
    )
    assert be_dostring(vm, code) == BE_OK


def test_re_match_no_match_not_anchored(vm):
    """match must start from beginning of string."""
    code = 'import re\nassert(re.match("[0-9]+", "abc123") == nil)\n'
    assert be_dostring(vm, code) == BE_OK


def test_re_match_with_offset(vm):
    code = (
        'import re\n'
        'var r = re.match("[0-9]+", "abc123", 3)\n'
        'assert(r[0] == "123")\n'
    )
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# re.match2
# ---------------------------------------------------------------------------
def test_re_match2_basic(vm):
    code = (
        'import re\n'
        'var r = re.match2("a.*?b(z+)", "aaaabbbccbbzzzee")\n'
        'assert(r[0] == 14)\n'
        'assert(r[1] == "zzz")\n'
    )
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# re.searchall
# ---------------------------------------------------------------------------
def test_re_searchall_basic(vm):
    code = (
        'import re\n'
        'var r = re.searchall("<([a-zA-Z]+)>", "<abc> yeah <xyz>")\n'
        'assert(r.size() == 2)\n'
        'assert(r[0][0] == "<abc>")\n'
        'assert(r[0][1] == "abc")\n'
        'assert(r[1][0] == "<xyz>")\n'
        'assert(r[1][1] == "xyz")\n'
    )
    assert be_dostring(vm, code) == BE_OK


def test_re_searchall_no_match(vm):
    code = (
        'import re\n'
        'var r = re.searchall("xyz", "abc")\n'
        'assert(r.size() == 0)\n'
    )
    assert be_dostring(vm, code) == BE_OK


def test_re_searchall_with_limit(vm):
    code = (
        'import re\n'
        'var r = re.searchall("[0-9]+", "1 2 3 4 5", 2)\n'
        'assert(r.size() == 2)\n'
        'assert(r[0][0] == "1")\n'
        'assert(r[1][0] == "2")\n'
    )
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# re.matchall
# ---------------------------------------------------------------------------
def test_re_matchall_contiguous(vm):
    code = (
        'import re\n'
        'var r = re.matchall("<([a-zA-Z]+)>", "<abc><xyz>")\n'
        'assert(r.size() == 2)\n'
        'assert(r[0][0] == "<abc>")\n'
        'assert(r[1][0] == "<xyz>")\n'
    )
    assert be_dostring(vm, code) == BE_OK


def test_re_matchall_gap_stops(vm):
    """matchall should stop when there's a gap (no match at current position)."""
    code = (
        'import re\n'
        'var r = re.matchall("<([a-zA-Z]+)>", "<abc> yeah <xyz>")\n'
        'assert(r.size() == 1)\n'
        'assert(r[0][0] == "<abc>")\n'
    )
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# re.split
# ---------------------------------------------------------------------------
def test_re_split_basic(vm):
    code = (
        'import re\n'
        'var r = re.split("/", "foo/bar//baz")\n'
        'assert(r.size() == 4)\n'
        'assert(r[0] == "foo")\n'
        'assert(r[1] == "bar")\n'
        'assert(r[2] == "")\n'
        'assert(r[3] == "baz")\n'
    )
    assert be_dostring(vm, code) == BE_OK


def test_re_split_no_match(vm):
    code = (
        'import re\n'
        'var r = re.split("x", "abc")\n'
        'assert(r.size() == 1)\n'
        'assert(r[0] == "abc")\n'
    )
    assert be_dostring(vm, code) == BE_OK


def test_re_split_with_limit(vm):
    code = (
        'import re\n'
        'var r = re.split("/", "a/b/c/d", 2)\n'
        'assert(r.size() == 3)\n'
        'assert(r[0] == "a")\n'
        'assert(r[1] == "b")\n'
        'assert(r[2] == "c/d")\n'
    )
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# re.compile + re_pattern methods
# ---------------------------------------------------------------------------
def test_re_compile_search(vm):
    code = (
        'import re\n'
        'var p = re.compile("a(b+)")\n'
        'var r = p.search("xaabbcc")\n'
        'assert(r[0] == "abb")\n'
        'assert(r[1] == "bb")\n'
    )
    assert be_dostring(vm, code) == BE_OK


def test_re_compile_match(vm):
    code = (
        'import re\n'
        'var p = re.compile("([0-9]+)")\n'
        'var r = p.match("123abc")\n'
        'assert(r[0] == "123")\n'
    )
    assert be_dostring(vm, code) == BE_OK


def test_re_compile_match2(vm):
    code = (
        'import re\n'
        'var p = re.compile("a(b+)")\n'
        'var r = p.match2("abbbx")\n'
        'assert(r[0] == 4)\n'
        'assert(r[1] == "bbb")\n'
    )
    assert be_dostring(vm, code) == BE_OK


def test_re_compile_split(vm):
    code = (
        'import re\n'
        'var p = re.compile(",")\n'
        'var r = p.split("a,b,,c")\n'
        'assert(r.size() == 4)\n'
        'assert(r[0] == "a")\n'
        'assert(r[2] == "")\n'
    )
    assert be_dostring(vm, code) == BE_OK


def test_re_compile_searchall(vm):
    code = (
        'import re\n'
        'var p = re.compile("<([a-zA-Z]+)>")\n'
        'var r = p.searchall("<abc> yeah <xyz>")\n'
        'assert(r.size() == 2)\n'
        'assert(r[0][1] == "abc")\n'
        'assert(r[1][1] == "xyz")\n'
    )
    assert be_dostring(vm, code) == BE_OK


def test_re_compile_matchall(vm):
    code = (
        'import re\n'
        'var p = re.compile("<([a-zA-Z]+)>")\n'
        'var r = p.matchall("<abc><xyz>")\n'
        'assert(r.size() == 2)\n'
    )
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# re.compilebytes
# ---------------------------------------------------------------------------
def test_re_compilebytes(vm):
    """compilebytes returns a comptr that can be used in place of a string pattern."""
    code = (
        'import re\n'
        'var rb = re.compilebytes("<([a-zA-Z]+)>")\n'
        'var r = re.searchall(rb, "<abc> yeah <xyz>")\n'
        'assert(r.size() == 2)\n'
        'assert(r[0][1] == "abc")\n'
    )
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# re.dump (no-op in Python port)
# ---------------------------------------------------------------------------
def test_re_dump(vm):
    code = 'import re\nvar rb = re.compilebytes("a")\nre.dump(rb)\n'
    assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# Error handling
# ---------------------------------------------------------------------------
def test_re_invalid_regex(vm):
    code = (
        'import re\n'
        'try\n'
        '  re.search("[invalid", "test")\n'
        '  assert(false)\n'
        'except .. as e, m\n'
        '  assert(e == "internal_error")\n'
        'end\n'
    )
    assert be_dostring(vm, code) == BE_OK


def test_re_type_error(vm):
    code = (
        'import re\n'
        'try\n'
        '  re.search(123, "test")\n'
        '  assert(false)\n'
        'except .. as e, m\n'
        '  assert(e == "type_error")\n'
        'end\n'
    )
    assert be_dostring(vm, code) == BE_OK
