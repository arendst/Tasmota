"""
Feature: berry-python-port, Property 5: Short string interning

For any string of length <= 64 bytes, creating it twice via be_newstrn should
return the same interned object (identity equality), and strings of length > 64
should be classified as long strings.

Validates: Requirements 8.2, 8.4
"""

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

from hypothesis import given, settings, assume
from hypothesis.strategies import binary, integers, text

from berry_port.be_object import (
    bgc, bstringtable, SHORT_STR_MAX_LEN,
)
from berry_port.be_string import (
    be_string_init, be_newstrn, be_newstr, be_eqstr, be_str2cstr, str_hash,
)


# ---------------------------------------------------------------------------
# Minimal mock VM with string table support
# ---------------------------------------------------------------------------
class MockVM:
    def __init__(self):
        self.gc = bgc()
        self.gc.usage = 0
        self.strtab = bstringtable()
        self.compopt = 0

def _fresh_vm():
    """Create a fresh VM with an initialized string table."""
    vm = MockVM()
    be_string_init(vm)
    return vm


# ---------------------------------------------------------------------------
# Strategies
# ---------------------------------------------------------------------------
# Latin-1 byte strings that can round-trip through Berry's char-based strings.
short_strings = binary(min_size=0, max_size=SHORT_STR_MAX_LEN)
long_strings = binary(min_size=SHORT_STR_MAX_LEN + 1, max_size=200)


def _to_latin1(data: bytes) -> str:
    """Convert bytes to a latin-1 str for Berry string creation."""
    return data.decode('latin-1')


# ---------------------------------------------------------------------------
# Property 5a: Short strings are interned (identity equality)
# ---------------------------------------------------------------------------
@settings(max_examples=500)
@given(data=short_strings)
def test_short_string_interned_identity(data):
    """Creating the same short string twice via be_newstrn must return
    the exact same object (identity / pointer equality)."""
    vm = _fresh_vm()
    s = _to_latin1(data)
    length = len(data)

    s1 = be_newstrn(vm, s, length)
    s2 = be_newstrn(vm, s, length)

    assert s1 is s2, (
        f"Short string of length {length} not interned: "
        f"s1 is not s2 (id {id(s1)} != {id(s2)})"
    )


# ---------------------------------------------------------------------------
# Property 5b: Short string slen field matches actual length
# ---------------------------------------------------------------------------
@settings(max_examples=300)
@given(data=short_strings)
def test_short_string_slen_matches_length(data):
    """For short strings, the slen field must equal the actual byte length."""
    vm = _fresh_vm()
    s = _to_latin1(data)
    length = len(data)

    result = be_newstrn(vm, s, length)

    assert result.slen == length, (
        f"slen mismatch: got {result.slen}, expected {length}"
    )


# ---------------------------------------------------------------------------
# Property 5c: Long strings are NOT interned (distinct objects)
# ---------------------------------------------------------------------------
@settings(max_examples=300)
@given(data=long_strings)
def test_long_string_not_interned(data):
    """Creating the same long string twice must return distinct objects
    (long strings are heap-allocated, not interned)."""
    vm = _fresh_vm()
    s = _to_latin1(data)
    length = len(data)

    s1 = be_newstrn(vm, s, length)
    s2 = be_newstrn(vm, s, length)

    assert s1 is not s2, (
        f"Long string of length {length} should NOT be interned"
    )


# ---------------------------------------------------------------------------
# Property 5d: Long strings have slen == 255 and llen == actual length
# ---------------------------------------------------------------------------
@settings(max_examples=300)
@given(data=long_strings)
def test_long_string_slen_is_255(data):
    """Long strings (> 64 bytes) must have slen == 255 and llen set to
    the actual length."""
    vm = _fresh_vm()
    s = _to_latin1(data)
    length = len(data)

    result = be_newstrn(vm, s, length)

    assert result.slen == 255, (
        f"Long string slen should be 255, got {result.slen}"
    )
    assert result.llen == length, (
        f"Long string llen mismatch: got {result.llen}, expected {length}"
    )


# ---------------------------------------------------------------------------
# Property 5e: Content is preserved for both short and long strings
# ---------------------------------------------------------------------------
@settings(max_examples=300)
@given(data=binary(min_size=0, max_size=200))
def test_string_content_preserved(data):
    """be_str2cstr must return the original content for any string."""
    vm = _fresh_vm()
    s = _to_latin1(data)
    length = len(data)

    result = be_newstrn(vm, s, length)
    retrieved = be_str2cstr(result)

    assert retrieved == s, (
        f"Content mismatch for {length}-byte string"
    )


# ---------------------------------------------------------------------------
# Property 5f: be_eqstr returns 1 for interned short strings
# ---------------------------------------------------------------------------
@settings(max_examples=300)
@given(data=short_strings)
def test_eqstr_interned_short_strings(data):
    """be_eqstr must return 1 for two references to the same interned
    short string."""
    vm = _fresh_vm()
    s = _to_latin1(data)
    length = len(data)

    s1 = be_newstrn(vm, s, length)
    s2 = be_newstrn(vm, s, length)

    assert be_eqstr(s1, s2) == 1, "be_eqstr should return 1 for interned strings"


# ---------------------------------------------------------------------------
# Property 5g: be_eqstr returns 1 for long strings with same content
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(data=long_strings)
def test_eqstr_long_strings_same_content(data):
    """be_eqstr must return 1 for two long strings with identical content."""
    vm = _fresh_vm()
    s = _to_latin1(data)
    length = len(data)

    s1 = be_newstrn(vm, s, length)
    s2 = be_newstrn(vm, s, length)

    assert be_eqstr(s1, s2) == 1, (
        "be_eqstr should return 1 for long strings with same content"
    )


# ---------------------------------------------------------------------------
# Property 5h: Different short strings are not identity-equal
# ---------------------------------------------------------------------------
@settings(max_examples=300)
@given(d1=short_strings, d2=short_strings)
def test_different_short_strings_not_same_object(d1, d2):
    """Two short strings with different content must not be the same object."""
    assume(d1 != d2)
    vm = _fresh_vm()

    s1 = be_newstrn(vm, _to_latin1(d1), len(d1))
    s2 = be_newstrn(vm, _to_latin1(d2), len(d2))

    assert s1 is not s2, (
        f"Different short strings should not be the same object: "
        f"{d1!r} vs {d2!r}"
    )


# ---------------------------------------------------------------------------
# Property 5i: String table count tracks interned strings
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(data=short_strings)
def test_strtab_count_increments(data):
    """Each unique short string should increment the string table count
    by exactly 1. Re-creating the same string should not change the count."""
    vm = _fresh_vm()
    s = _to_latin1(data)
    length = len(data)

    count_before = vm.strtab.count
    be_newstrn(vm, s, length)
    count_after_first = vm.strtab.count
    be_newstrn(vm, s, length)
    count_after_second = vm.strtab.count

    assert count_after_first == count_before + 1, (
        f"First creation should increment count by 1: "
        f"{count_before} -> {count_after_first}"
    )
    assert count_after_second == count_after_first, (
        f"Second creation of same string should not change count: "
        f"{count_after_first} -> {count_after_second}"
    )


# ---------------------------------------------------------------------------
# Property 5j: Boundary — string of exactly SHORT_STR_MAX_LEN is short
# ---------------------------------------------------------------------------
def test_boundary_max_short_is_interned():
    """A string of exactly SHORT_STR_MAX_LEN (64) bytes must be interned."""
    vm = _fresh_vm()
    data = b'A' * SHORT_STR_MAX_LEN
    s = _to_latin1(data)

    s1 = be_newstrn(vm, s, SHORT_STR_MAX_LEN)
    s2 = be_newstrn(vm, s, SHORT_STR_MAX_LEN)

    assert s1 is s2, "String of exactly 64 bytes should be interned"
    assert s1.slen == SHORT_STR_MAX_LEN


def test_boundary_one_over_max_is_long():
    """A string of SHORT_STR_MAX_LEN + 1 (65) bytes must be a long string."""
    vm = _fresh_vm()
    length = SHORT_STR_MAX_LEN + 1
    data = b'B' * length
    s = _to_latin1(data)

    s1 = be_newstrn(vm, s, length)
    s2 = be_newstrn(vm, s, length)

    assert s1 is not s2, "String of 65 bytes should NOT be interned"
    assert s1.slen == 255
    assert s1.llen == length
