"""
Unit tests for berry_port.re15_compiler — sizecode and compilecode.

Tests cover:
- sizecode returns correct sizes for simple patterns
- compilecode output bytes match known C output
- Error codes for invalid patterns: trailing backslash, unsupported escapes, unsupported syntax
- ByteProg field values (bytelen, len, sub) match C compiler
"""

import struct

from berry_port.re15_compiler import sizecode, compilecode
from berry_port.re15_opcodes import (
    RE1_5_SUCCESS,
    RE1_5_SYNTAX_ERROR,
    RE1_5_UNSUPPORTED_ESCAPE,
    RE1_5_UNSUPPORTED_SYNTAX,
    NON_ANCHORED_PREFIX,
)


# ---------------------------------------------------------------------------
# Helper
# ---------------------------------------------------------------------------

def _serialize(prog):
    """Serialize ByteProg to bytes matching C struct layout."""
    return struct.pack('<iii', prog.bytelen, prog.len, prog.sub) + bytes(prog.insts[:prog.bytelen])


# ---------------------------------------------------------------------------
# sizecode — correct sizes for simple patterns
# ---------------------------------------------------------------------------

def test_sizecode_literal():
    # "a" -> Char a = 2 bytes body, + Save0(2) + Save1(2) + Match(1) + prefix(5) = 12
    assert sizecode("a") == 12


def test_sizecode_dot():
    # "." -> Any = 1 byte body
    assert sizecode(".") == 11


def test_sizecode_alternation():
    # "a|b" -> Split(2) + Char a(2) + Jmp(2) + Char b(2) = 8 bytes body
    assert sizecode("a|b") == 18


def test_sizecode_group():
    # "(a)" -> Save(2) + Char a(2) + Save(2) = 6 bytes body
    assert sizecode("(a)") == 16


def test_sizecode_star():
    # "a*" -> Split(2) + Char a(2) + Jmp(2) = 6 bytes body
    assert sizecode("a*") == 16


def test_sizecode_plus():
    # "a+" -> Char a(2) + RSplit(2) = 4 bytes body
    assert sizecode("a+") == 14


def test_sizecode_question():
    # "a?" -> Split(2) + Char a(2) = 4 bytes body
    assert sizecode("a?") == 14


def test_sizecode_slash():
    # "/" -> Char /(2) = 2 bytes body -> total 12
    assert sizecode("/") == 12


def test_sizecode_complex():
    # "a.*?b(z+)" — known from C reference
    assert sizecode("a.*?b(z+)") == 27


# ---------------------------------------------------------------------------
# sizecode — error codes
# ---------------------------------------------------------------------------

def test_sizecode_trailing_backslash():
    assert sizecode("\\") == RE1_5_SYNTAX_ERROR


def test_sizecode_unsupported_escape():
    assert sizecode("\\b") == RE1_5_UNSUPPORTED_ESCAPE


def test_sizecode_unsupported_syntax_brace():
    assert sizecode("a{2}") == RE1_5_UNSUPPORTED_SYNTAX


def test_sizecode_unmatched_paren():
    assert sizecode("(a") == RE1_5_SYNTAX_ERROR


def test_sizecode_extra_close_paren():
    assert sizecode("a)") == RE1_5_SYNTAX_ERROR


# ---------------------------------------------------------------------------
# compilecode — byte-for-byte match with C output
# ---------------------------------------------------------------------------

def test_compilecode_slash_hex():
    """compilecode('/') must produce exact hex from C reference."""
    prog, err = compilecode("/")
    assert err == RE1_5_SUCCESS
    expected = '0c000000070000000000000062030260fb7e00012f7e017f'
    assert _serialize(prog).hex() == expected


def test_compilecode_complex_hex():
    """compilecode('a.*?b(z+)') must produce exact hex from C reference."""
    prog, err = compilecode("a.*?b(z+)")
    assert err == RE1_5_SUCCESS
    expected = '1b0000000f0000000100000062030260fb7e00016162030260fb01627e02017a62fc7e037e017f'
    assert _serialize(prog).hex() == expected


# ---------------------------------------------------------------------------
# compilecode — ByteProg field values
# ---------------------------------------------------------------------------

def test_compilecode_slash_fields():
    prog, err = compilecode("/")
    assert err == RE1_5_SUCCESS
    assert prog.bytelen == 12
    assert prog.len == 7
    assert prog.sub == 0


def test_compilecode_complex_fields():
    prog, err = compilecode("a.*?b(z+)")
    assert err == RE1_5_SUCCESS
    assert prog.bytelen == 27
    assert prog.len == 15
    assert prog.sub == 1


def test_compilecode_multiple_groups():
    prog, err = compilecode("(a)(b)(c)")
    assert err == RE1_5_SUCCESS
    assert prog.sub == 3


def test_compilecode_non_capturing_group():
    prog, err = compilecode("(?:a)(b)")
    assert err == RE1_5_SUCCESS
    assert prog.sub == 1  # only one capture group


# ---------------------------------------------------------------------------
# compilecode — non-anchored prefix structure
# ---------------------------------------------------------------------------

def test_compilecode_prefix_bytes():
    """First 5 bytes must be RSplit(0x62) 3, Any(0x02), Jmp(0x60) -5(0xFB)."""
    prog, err = compilecode("a")
    assert err == RE1_5_SUCCESS
    prefix = list(prog.insts[:NON_ANCHORED_PREFIX])
    assert prefix == [0x62, 0x03, 0x02, 0x60, 0xFB]


# ---------------------------------------------------------------------------
# compilecode — error codes
# ---------------------------------------------------------------------------

def test_compilecode_trailing_backslash():
    prog, err = compilecode("\\")
    assert err == RE1_5_SYNTAX_ERROR


def test_compilecode_unsupported_escape_b():
    prog, err = compilecode("\\b")
    assert err == RE1_5_UNSUPPORTED_ESCAPE


def test_compilecode_unsupported_escape_1():
    prog, err = compilecode("\\1")
    assert err == RE1_5_UNSUPPORTED_ESCAPE


def test_compilecode_unsupported_syntax_brace():
    prog, err = compilecode("a{2}")
    assert err == RE1_5_UNSUPPORTED_SYNTAX


def test_compilecode_unsupported_syntax_lookahead():
    prog, err = compilecode("(?=a)")
    assert err == RE1_5_UNSUPPORTED_SYNTAX


def test_compilecode_unmatched_open_paren():
    prog, err = compilecode("(a")
    assert err == RE1_5_SYNTAX_ERROR


def test_compilecode_extra_close_paren():
    prog, err = compilecode("a)")
    assert err == RE1_5_SYNTAX_ERROR


# ---------------------------------------------------------------------------
# compilecode — various regex constructs produce valid bytecode
# ---------------------------------------------------------------------------

def test_compilecode_dot():
    prog, err = compilecode(".")
    assert err == RE1_5_SUCCESS
    assert prog.bytelen == sizecode(".")


def test_compilecode_char_class():
    prog, err = compilecode("[a-z]")
    assert err == RE1_5_SUCCESS
    assert prog.bytelen == sizecode("[a-z]")


def test_compilecode_negated_class():
    prog, err = compilecode("[^0-9]")
    assert err == RE1_5_SUCCESS
    assert prog.bytelen == sizecode("[^0-9]")


def test_compilecode_named_class():
    prog, err = compilecode("\\d+")
    assert err == RE1_5_SUCCESS
    assert prog.bytelen == sizecode("\\d+")


def test_compilecode_alternation():
    prog, err = compilecode("a|b")
    assert err == RE1_5_SUCCESS
    assert prog.bytelen == sizecode("a|b")


def test_compilecode_anchors():
    prog, err = compilecode("^a$")
    assert err == RE1_5_SUCCESS
    assert prog.bytelen == sizecode("^a$")


def test_compilecode_non_greedy_star():
    prog, err = compilecode("a*?")
    assert err == RE1_5_SUCCESS
    assert prog.bytelen == sizecode("a*?")


def test_compilecode_non_greedy_question():
    prog, err = compilecode("a??")
    assert err == RE1_5_SUCCESS
    assert prog.bytelen == sizecode("a??")


def test_compilecode_non_greedy_plus():
    prog, err = compilecode("a+?")
    assert err == RE1_5_SUCCESS
    assert prog.bytelen == sizecode("a+?")


def test_compilecode_literal_escapes():
    """Escaped special chars should compile without error."""
    for ch in r'\[]().^$|?*+{}':
        pattern = '\\' + ch
        prog, err = compilecode(pattern)
        assert err == RE1_5_SUCCESS, f"Failed for pattern: {pattern}"


def test_compilecode_empty_pattern():
    prog, err = compilecode("")
    assert err == RE1_5_SUCCESS
    # Empty pattern still has prefix + Save0 + Save1 + Match
    assert prog.bytelen == 10


def test_compilecode_sizecode_consistency():
    """For every valid pattern, compilecode bytelen must equal sizecode result."""
    patterns = ["a", ".", "a|b", "(a)", "a*", "a+", "a?", "^a$",
                "[a-z]", "[^0-9]", "\\d", "\\w+", "a.*?b(z+)", ""]
    for pat in patterns:
        size = sizecode(pat)
        assert size > 0, f"sizecode failed for {pat!r}"
        prog, err = compilecode(pat)
        assert err == RE1_5_SUCCESS, f"compilecode failed for {pat!r}"
        assert prog.bytelen == size, f"Mismatch for {pat!r}: bytelen={prog.bytelen}, sizecode={size}"


# ---------------------------------------------------------------------------
# compilecode — additional byte-for-byte hex checks from tests/re.be
# ---------------------------------------------------------------------------

def test_compilecode_char_class_group_hex():
    """compilecode('<([a-zA-Z]+)>') must produce exact hex from C reference (tests/re.be)."""
    prog, err = compilecode("<([a-zA-Z]+)>")
    assert err == RE1_5_SUCCESS
    expected = '1a0000000c0000000100000062030260fb7e00013c7e020302617a415a62f87e03013e7e017f'
    assert _serialize(prog).hex() == expected


def test_compilecode_char_class_group_fields():
    prog, err = compilecode("<([a-zA-Z]+)>")
    assert err == RE1_5_SUCCESS
    assert prog.bytelen == 26
    assert prog.len == 12
    assert prog.sub == 1
