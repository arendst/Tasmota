"""
Integration tests for be_relib.py — the Berry re module backed by the re1.5 engine.

Covers the full round-trip: Berry script → be_relib.py → re15_compiler/re15_vm.

Tests:
- re.compile + pattern.search/match/match2 round-trip
- re.compilebytes producing expected hex for known patterns
- re.search, re.match, re.match2 with string and bytes patterns
- re.split including zero-length match error
- Offset handling: negative offset, offset >= length
- Type error raising for invalid arguments

Requirements: 5.1, 5.2, 5.3, 5.4, 5.5, 5.8, 5.9, 5.11, 5.12, 9.1, 9.4, 9.5
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
# re.compile + pattern round-trip (Req 5.1)
# ---------------------------------------------------------------------------

class TestCompilePatternRoundTrip:
    """re.compile returns a re_pattern whose methods use the re1.5 engine."""

    def test_compile_search_round_trip(self, vm):
        code = (
            'import re\n'
            'var p = re.compile("(\\\\d+)")\n'
            'var r = p.search("abc123def")\n'
            'assert(r[0] == "123")\n'
            'assert(r[1] == "123")\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_compile_match_round_trip(self, vm):
        code = (
            'import re\n'
            'var p = re.compile("([a-z]+)(\\\\d+)")\n'
            'var r = p.match("abc123")\n'
            'assert(r[0] == "abc123")\n'
            'assert(r[1] == "abc")\n'
            'assert(r[2] == "123")\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_compile_match2_round_trip(self, vm):
        code = (
            'import re\n'
            'var p = re.compile("\\\\w+")\n'
            'var r = p.match2("hello world")\n'
            'assert(r[0] == 5)\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_compile_split_round_trip(self, vm):
        code = (
            'import re\n'
            'var p = re.compile("[,;]")\n'
            'var r = p.split("a,b;c")\n'
            'assert(r.size() == 3)\n'
            'assert(r[0] == "a")\n'
            'assert(r[1] == "b")\n'
            'assert(r[2] == "c")\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_compile_searchall_round_trip(self, vm):
        code = (
            'import re\n'
            'var p = re.compile("\\\\d+")\n'
            'var r = p.searchall("a1b22c333")\n'
            'assert(r.size() == 3)\n'
            'assert(r[0][0] == "1")\n'
            'assert(r[1][0] == "22")\n'
            'assert(r[2][0] == "333")\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_compile_matchall_round_trip(self, vm):
        code = (
            'import re\n'
            'var p = re.compile("\\\\d+")\n'
            'var r = p.matchall("123abc")\n'
            'assert(r.size() == 1)\n'
            'assert(r[0][0] == "123")\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_compile_reuse_pattern(self, vm):
        """Compiled pattern can be reused for multiple matches."""
        code = (
            'import re\n'
            'var p = re.compile("(\\\\w+)")\n'
            'var r1 = p.search("hello world")\n'
            'var r2 = p.search("foo bar")\n'
            'assert(r1[1] == "hello")\n'
            'assert(r2[1] == "foo")\n'
        )
        assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# re.compilebytes — expected hex output (Req 5.2, 8.1, 8.2)
# ---------------------------------------------------------------------------

class TestCompileBytes:
    """re.compilebytes must produce byte-for-byte identical output to C."""

    def test_compilebytes_slash(self, vm):
        """compilebytes('/') must match known C hex output."""
        code = (
            'import re\n'
            'var rb = re.compilebytes("/")\n'
            'var expected = bytes("0C000000070000000000000062030260FB7E00012F7E017F")\n'
            'assert(rb == expected)\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_compilebytes_complex_pattern(self, vm):
        """compilebytes('a.*?b(z+)') must match known C hex output."""
        code = (
            'import re\n'
            'var rb = re.compilebytes("a.*?b(z+)")\n'
            'var expected = bytes("1B0000000F0000000100000062030260FB7E00016162030260FB01627E02017A62FC7E037E017F")\n'
            'assert(rb == expected)\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_compilebytes_usable_as_pattern(self, vm):
        """Bytes from compilebytes can be used in re.search."""
        code = (
            'import re\n'
            'var rb = re.compilebytes("a.*?b(z+)")\n'
            'var r = re.search(rb, "zaaaabbbccbbzzzee")\n'
            'assert(r[0] == "aaaabbbccbbzzz")\n'
            'assert(r[1] == "zzz")\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_compilebytes_usable_in_match(self, vm):
        """Bytes from compilebytes can be used in re.match."""
        code = (
            'import re\n'
            'var rb = re.compilebytes("\\\\d+")\n'
            'var r = re.match(rb, "42abc")\n'
            'assert(r[0] == "42")\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_compilebytes_usable_in_match2(self, vm):
        """Bytes from compilebytes can be used in re.match2."""
        code = (
            'import re\n'
            'var rb = re.compilebytes("\\\\d+")\n'
            'var r = re.match2(rb, "42abc")\n'
            'assert(r[0] == 2)\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_compilebytes_usable_in_split(self, vm):
        """Bytes from compilebytes can be used in re.split."""
        code = (
            'import re\n'
            'var rb = re.compilebytes("/")\n'
            'var r = re.split(rb, "foo/bar//baz")\n'
            'assert(r.size() == 4)\n'
            'assert(r[0] == "foo")\n'
            'assert(r[1] == "bar")\n'
            'assert(r[2] == "")\n'
            'assert(r[3] == "baz")\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_compilebytes_usable_in_searchall(self, vm):
        """Bytes from compilebytes can be used in re.searchall."""
        code = (
            'import re\n'
            'var rb = re.compilebytes("\\\\d+")\n'
            'var r = re.searchall(rb, "a1b22c333")\n'
            'assert(r.size() == 3)\n'
            'assert(r[0][0] == "1")\n'
            'assert(r[2][0] == "333")\n'
        )
        assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# re.search with string and bytes patterns (Req 5.3, 5.11)
# ---------------------------------------------------------------------------

class TestSearchStringAndBytes:

    def test_search_string_pattern(self, vm):
        code = (
            'import re\n'
            'var r = re.search("(\\\\w+)@(\\\\w+)", "user@host")\n'
            'assert(r[0] == "user@host")\n'
            'assert(r[1] == "user")\n'
            'assert(r[2] == "host")\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_search_bytes_pattern(self, vm):
        code = (
            'import re\n'
            'var rb = re.compilebytes("(\\\\w+)@(\\\\w+)")\n'
            'var r = re.search(rb, "user@host")\n'
            'assert(r[0] == "user@host")\n'
            'assert(r[1] == "user")\n'
            'assert(r[2] == "host")\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_search_no_match_returns_nil(self, vm):
        code = (
            'import re\n'
            'assert(re.search("xyz", "abc") == nil)\n'
        )
        assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# re.match with string and bytes patterns (Req 5.4, 5.11)
# ---------------------------------------------------------------------------

class TestMatchStringAndBytes:

    def test_match_string_pattern(self, vm):
        code = (
            'import re\n'
            'var r = re.match("([a-z]+)", "hello123")\n'
            'assert(r[0] == "hello")\n'
            'assert(r[1] == "hello")\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_match_bytes_pattern(self, vm):
        code = (
            'import re\n'
            'var rb = re.compilebytes("([a-z]+)")\n'
            'var r = re.match(rb, "hello123")\n'
            'assert(r[0] == "hello")\n'
            'assert(r[1] == "hello")\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_match_fails_not_at_start(self, vm):
        code = (
            'import re\n'
            'assert(re.match("\\\\d+", "abc123") == nil)\n'
        )
        assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# re.match2 with string and bytes patterns (Req 5.5, 5.11)
# ---------------------------------------------------------------------------

class TestMatch2StringAndBytes:

    def test_match2_string_pattern(self, vm):
        code = (
            'import re\n'
            'var r = re.match2("\\\\d+", "12345abc")\n'
            'assert(r[0] == 5)\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_match2_bytes_pattern(self, vm):
        code = (
            'import re\n'
            'var rb = re.compilebytes("\\\\d+")\n'
            'var r = re.match2(rb, "12345abc")\n'
            'assert(r[0] == 5)\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_match2_with_groups(self, vm):
        """match2 returns length as first element, groups as strings."""
        code = (
            'import re\n'
            'var r = re.match2("(\\\\d+)(\\\\w+)", "123abc")\n'
            'assert(r[0] == 6)\n'
            'assert(r[1] == "123")\n'
            'assert(r[2] == "abc")\n'
        )
        assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# re.split — including zero-length match error (Req 5.8, 5.9)
# ---------------------------------------------------------------------------

class TestSplit:

    def test_split_basic(self, vm):
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

    def test_split_with_limit(self, vm):
        code = (
            'import re\n'
            'var r = re.split("/", "a/b/c/d", 2)\n'
            'assert(r.size() == 3)\n'
            'assert(r[0] == "a")\n'
            'assert(r[1] == "b")\n'
            'assert(r[2] == "c/d")\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_split_no_match(self, vm):
        code = (
            'import re\n'
            'var r = re.split("x", "abc")\n'
            'assert(r.size() == 1)\n'
            'assert(r[0] == "abc")\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_split_with_bytes_pattern(self, vm):
        code = (
            'import re\n'
            'var rb = re.compilebytes(",")\n'
            'var r = re.split(rb, "a,b,c")\n'
            'assert(r.size() == 3)\n'
            'assert(r[0] == "a")\n'
            'assert(r[1] == "b")\n'
            'assert(r[2] == "c")\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_split_regex_separator(self, vm):
        code = (
            'import re\n'
            'var r = re.split("[,;]+", "a,,b;c;;d")\n'
            'assert(r.size() == 4)\n'
            'assert(r[0] == "a")\n'
            'assert(r[1] == "b")\n'
            'assert(r[2] == "c")\n'
            'assert(r[3] == "d")\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_split_zero_length_match_raises_error(self, vm):
        """Zero-length match during split must raise internal_error 'can't match'."""
        code = (
            'import re\n'
            'try\n'
            '  re.split("a*", "abc")\n'
            '  assert(false)\n'
            'except .. as e, m\n'
            '  assert(e == "internal_error")\n'
            'end\n'
        )
        assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# Offset handling (Req 5.12)
# ---------------------------------------------------------------------------

class TestOffsetHandling:

    def test_search_negative_offset_clamped_to_zero(self, vm):
        """Negative offset should be clamped to 0."""
        code = (
            'import re\n'
            'var r = re.search("a", "abc", -5)\n'
            'assert(r[0] == "a")\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_match_negative_offset_clamped_to_zero(self, vm):
        code = (
            'import re\n'
            'var r = re.match("a", "abc", -10)\n'
            'assert(r[0] == "a")\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_search_offset_past_end_returns_nil(self, vm):
        code = (
            'import re\n'
            'assert(re.search("a", "abc", 100) == nil)\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_match_offset_past_end_returns_nil(self, vm):
        code = (
            'import re\n'
            'assert(re.match("a", "abc", 100) == nil)\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_match2_offset_past_end_returns_nil(self, vm):
        code = (
            'import re\n'
            'assert(re.match2("a", "abc", 100) == nil)\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_search_offset_equal_to_length_returns_nil(self, vm):
        code = (
            'import re\n'
            'assert(re.search("a", "abc", 3) == nil)\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_search_valid_offset_skips_chars(self, vm):
        code = (
            'import re\n'
            'var r = re.search("\\\\d+", "abc123def456", 6)\n'
            'assert(r[0] == "456")\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_pattern_search_negative_offset(self, vm):
        """re_pattern methods also clamp negative offset."""
        code = (
            'import re\n'
            'var p = re.compile("a")\n'
            'var r = p.search("abc", -5)\n'
            'assert(r[0] == "a")\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_pattern_match_offset_past_end(self, vm):
        code = (
            'import re\n'
            'var p = re.compile("a")\n'
            'assert(p.match("abc", 100) == nil)\n'
        )
        assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# Type error handling (Req 9.4)
# ---------------------------------------------------------------------------

class TestTypeErrors:

    def test_search_non_string_pattern_raises_type_error(self, vm):
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

    def test_match_non_string_pattern_raises_type_error(self, vm):
        code = (
            'import re\n'
            'try\n'
            '  re.match(123, "test")\n'
            '  assert(false)\n'
            'except .. as e, m\n'
            '  assert(e == "type_error")\n'
            'end\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_match2_non_string_pattern_raises_type_error(self, vm):
        code = (
            'import re\n'
            'try\n'
            '  re.match2(123, "test")\n'
            '  assert(false)\n'
            'except .. as e, m\n'
            '  assert(e == "type_error")\n'
            'end\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_split_non_string_pattern_raises_type_error(self, vm):
        code = (
            'import re\n'
            'try\n'
            '  re.split(123, "test")\n'
            '  assert(false)\n'
            'except .. as e, m\n'
            '  assert(e == "type_error")\n'
            'end\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_compile_non_string_raises_type_error(self, vm):
        code = (
            'import re\n'
            'try\n'
            '  re.compile(123)\n'
            '  assert(false)\n'
            'except .. as e, m\n'
            '  assert(e == "type_error")\n'
            'end\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_compilebytes_non_string_raises_type_error(self, vm):
        code = (
            'import re\n'
            'try\n'
            '  re.compilebytes(123)\n'
            '  assert(false)\n'
            'except .. as e, m\n'
            '  assert(e == "type_error")\n'
            'end\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_search_non_string_haystack_raises_type_error(self, vm):
        code = (
            'import re\n'
            'try\n'
            '  re.search("a", 123)\n'
            '  assert(false)\n'
            'except .. as e, m\n'
            '  assert(e == "type_error")\n'
            'end\n'
        )
        assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# Regex error handling (Req 9.1, 9.2, 9.3)
# ---------------------------------------------------------------------------

class TestRegexErrors:

    def test_compile_invalid_regex_raises_internal_error(self, vm):
        code = (
            'import re\n'
            'try\n'
            '  re.compile("[invalid")\n'
            '  assert(false)\n'
            'except .. as e, m\n'
            '  assert(e == "internal_error")\n'
            'end\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_search_invalid_regex_raises_internal_error(self, vm):
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

    def test_compilebytes_invalid_regex_raises_internal_error(self, vm):
        code = (
            'import re\n'
            'try\n'
            '  re.compilebytes("[invalid")\n'
            '  assert(false)\n'
            'except .. as e, m\n'
            '  assert(e == "internal_error")\n'
            'end\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_unsupported_escape_raises_error(self, vm):
        code = (
            'import re\n'
            'try\n'
            '  re.compile("\\\\b")\n'
            '  assert(false)\n'
            'except .. as e, m\n'
            '  assert(e == "internal_error")\n'
            'end\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_unsupported_syntax_raises_error(self, vm):
        code = (
            'import re\n'
            'try\n'
            '  re.compile("a{2}")\n'
            '  assert(false)\n'
            'except .. as e, m\n'
            '  assert(e == "internal_error")\n'
            'end\n'
        )
        assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# Empty haystack / edge cases (Req 9.5)
# ---------------------------------------------------------------------------

class TestEdgeCases:

    def test_search_empty_haystack_returns_nil(self, vm):
        code = (
            'import re\n'
            'assert(re.search("a", "") == nil)\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_match_empty_haystack_returns_nil(self, vm):
        code = (
            'import re\n'
            'assert(re.match("a", "") == nil)\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_split_empty_separator_at_start(self, vm):
        """Split with pattern that matches at start of non-empty string."""
        code = (
            'import re\n'
            'var r = re.split(",", ",a,b")\n'
            'assert(r.size() == 3)\n'
            'assert(r[0] == "")\n'
            'assert(r[1] == "a")\n'
            'assert(r[2] == "b")\n'
        )
        assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# Byte-for-byte bytecode compatibility — all patterns from tests/re.be
# ---------------------------------------------------------------------------

class TestBytecodeCompatibility:
    """Verify compilebytes hex output matches C reference for all patterns in tests/re.be."""

    def test_compilebytes_char_class_group(self, vm):
        """compilebytes('<([a-zA-Z]+)>') must match known C hex output."""
        code = (
            'import re\n'
            'var rb = re.compilebytes(\'<([a-zA-Z]+)>\')\n'
            'var expected = bytes("1A0000000C0000000100000062030260FB7E00013C7E020302617A415A62F87E03013E7E017F")\n'
            'assert(rb == expected)\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_compilebytes_char_class_group_usable_in_searchall(self, vm):
        """Bytes from compilebytes('<([a-zA-Z]+)>') work in searchall."""
        code = (
            'import re\n'
            'var rb = re.compilebytes(\'<([a-zA-Z]+)>\')\n'
            'var r = re.searchall(rb, \'<abc> yeah <xyz>\')\n'
            'assert(r.size() == 2)\n'
            'assert(r[0][0] == "<abc>")\n'
            'assert(r[0][1] == "abc")\n'
            'assert(r[1][0] == "<xyz>")\n'
            'assert(r[1][1] == "xyz")\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_compilebytes_char_class_group_usable_in_matchall(self, vm):
        """Bytes from compilebytes('<([a-zA-Z]+)>') work in matchall."""
        code = (
            'import re\n'
            'var rb = re.compilebytes(\'<([a-zA-Z]+)>\')\n'
            'assert(re.matchall(rb, \'<abc> yeah <xyz>\') == [[\'<abc>\', \'abc\']])\n'
            'assert(re.matchall(rb, \'<abc><xyz>\') == [[\'<abc>\', \'abc\'], [\'<xyz>\', \'xyz\']])\n'
        )
        assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# Full re.be round-trip — key assertions from tests/re.be
# ---------------------------------------------------------------------------

class TestReBeRoundTrip:
    """Replicate the key assertions from tests/re.be as Python-level tests."""

    def test_search_complex_pattern(self, vm):
        code = (
            'import re\n'
            'assert(re.search("a.*?b(z+)", "zaaaabbbccbbzzzee") == [\'aaaabbbccbbzzz\', \'zzz\'])\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_searchall_html_tags(self, vm):
        code = (
            'import re\n'
            'assert(re.searchall(\'<([a-zA-Z]+)>\', \'<abc> yeah <xyz>\') == [[\'<abc>\', \'abc\'], [\'<xyz>\', \'xyz\']])\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_match_complex_pattern(self, vm):
        code = (
            'import re\n'
            'assert(re.match("a.*?b(z+)", "aaaabbbccbbzzzee") == [\'aaaabbbccbbzzz\', \'zzz\'])\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_match2_complex_pattern(self, vm):
        code = (
            'import re\n'
            'assert(re.match2("a.*?b(z+)", "aaaabbbccbbzzzee") == [14, \'zzz\'])\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_matchall_html_tags(self, vm):
        code = (
            'import re\n'
            'assert(re.matchall(\'<([a-zA-Z]+)>\', \'<abc> yeah <xyz>\') == [[\'<abc>\', \'abc\']])\n'
            'assert(re.matchall(\'<([a-zA-Z]+)>\', \'<abc><xyz>\') == [[\'<abc>\', \'abc\'], [\'<xyz>\', \'xyz\']])\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_split_slash(self, vm):
        code = (
            'import re\n'
            'assert(re.split(\'/\', "foo/bar//baz") == [\'foo\', \'bar\', \'\', \'baz\'])\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_precompiled_search(self, vm):
        code = (
            'import re\n'
            'var rr = re.compile("a.*?b(z+)")\n'
            'assert(rr.search("zaaaabbbccbbzzzee") == [\'aaaabbbccbbzzz\', \'zzz\'])\n'
        )
        assert be_dostring(vm, code) == BE_OK

    def test_precompiled_split(self, vm):
        code = (
            'import re\n'
            'var rr = re.compile(\'/\')\n'
            'assert(rr.split("foo/bar//baz") == [\'foo\', \'bar\', \'\', \'baz\'])\n'
        )
        assert be_dostring(vm, code) == BE_OK
