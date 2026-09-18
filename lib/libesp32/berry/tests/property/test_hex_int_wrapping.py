"""
Feature: berry-python-port — Hex integer 32-bit signed wrapping

Integer literals parsed by the Python port must wrap to the signed range
dictated by BE_INTGER_TYPE, matching C overflow semantics.  With
BE_INTGER_TYPE == 0 (32-bit signed int), 0xFFFFFFFF must become -1,
0x80000000 must become -2147483648, etc.

This covers:
  - _wrap_bint helper correctness
  - _scan_hexadecimal wrapping via the lexer
  - be_str2int wrapping for hex and decimal paths
  - End-to-end: solidify output matches C binary for large hex literals
"""

import sys
import os
import io
import subprocess

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

import pytest

from berry_port.be_lexer import (
    _wrap_bint, be_str2int, _scan_hexadecimal,
    blexer, be_lexer_init, be_lexer_scan_next,
    TokenInteger,
)
from berry_port.be_object import bvalue, bgc, bstringtable
from berry_port.be_string import be_string_init
from berry_port.be_vm import be_vm_new
from berry_port.be_exec import be_loadbuffer
from berry_port.be_object import var_toobj
from berry_port.be_solidifylib import m_solidify_closure
from berry_port.berry_conf import BE_INTGER_TYPE


# ============================================================================
# Helpers
# ============================================================================

_BERRY_BIN = os.path.join(os.path.dirname(__file__), '..', '..', 'berry')


def _c_binary_available():
    return os.path.isfile(_BERRY_BIN) and os.access(_BERRY_BIN, os.X_OK)


def _tokenize_single_int(source):
    """Tokenize *source* and return the integer value of the single token."""
    vm = be_vm_new()
    lexer = blexer()
    buf = [source.encode('latin-1')]
    pos = [0]

    def reader(lex, data, size_out):
        if pos[0] < len(buf):
            chunk = buf[pos[0]]
            pos[0] += 1
            size_out[0] = len(chunk)
            return chunk
        size_out[0] = 0
        return None

    be_lexer_init(lexer, vm, "test", reader, None)
    be_lexer_scan_next(lexer)
    assert lexer.token.type == TokenInteger
    return lexer.token.i


def _compile_and_solidify(source):
    """Compile Berry source and solidify the inner function."""
    from berry_port.be_func import be_newclosure

    vm = be_vm_new()
    rc = be_loadbuffer(vm, "input", source, len(source))
    assert rc == 0, f"be_loadbuffer failed: rc={rc}"

    main_clo = var_toobj(vm.stack[vm.top_idx - 1])
    main_proto = main_clo.proto
    if main_proto.nproto > 0 and main_proto.ptab:
        inner_proto = main_proto.ptab[0]
        inner_clo = be_newclosure(vm, 0)
        inner_clo.proto = inner_proto
    else:
        inner_clo = main_clo

    fout = io.StringIO()
    m_solidify_closure(vm, False, inner_clo, None, fout)
    return fout.getvalue().lstrip('\n')


# ============================================================================
# 1. _wrap_bint unit tests
# ============================================================================

class TestWrapBint:
    """Direct tests for the _wrap_bint helper."""

    def test_zero(self):
        assert _wrap_bint(0) == 0

    def test_positive_small(self):
        assert _wrap_bint(42) == 42

    def test_negative_small(self):
        assert _wrap_bint(-1) == -1

    def test_int_max(self):
        # 0x7FFFFFFF = 2147483647 stays positive
        assert _wrap_bint(0x7FFFFFFF) == 2147483647

    def test_int_min(self):
        # 0x80000000 wraps to -2147483648
        assert _wrap_bint(0x80000000) == -2147483648

    def test_all_ones(self):
        # 0xFFFFFFFF wraps to -1
        assert _wrap_bint(0xFFFFFFFF) == -1

    def test_0xFFFFFFFE(self):
        assert _wrap_bint(0xFFFFFFFE) == -2

    def test_0x80000001(self):
        assert _wrap_bint(0x80000001) == -2147483647

    def test_larger_than_32bit_wraps(self):
        # Values beyond 32 bits get masked first
        assert _wrap_bint(0x1FFFFFFFF) == -1  # low 32 bits = 0xFFFFFFFF

    def test_idempotent_on_negative(self):
        assert _wrap_bint(-2147483648) == -2147483648

    def test_idempotent_on_positive(self):
        assert _wrap_bint(100) == 100


# ============================================================================
# 2. be_str2int wrapping tests
# ============================================================================

class TestBeStr2IntWrapping:
    """be_str2int must wrap results to signed bint range."""

    def test_hex_FFFFFFFF(self):
        assert be_str2int("0xFFFFFFFF") == -1

    def test_hex_80000000(self):
        assert be_str2int("0x80000000") == -2147483648

    def test_hex_7FFFFFFF(self):
        assert be_str2int("0x7FFFFFFF") == 2147483647

    def test_decimal_large(self):
        # 4294967295 in decimal also wraps
        assert be_str2int("4294967295") == -1

    def test_decimal_normal(self):
        assert be_str2int("123") == 123

    def test_decimal_negative(self):
        assert be_str2int("-1") == -1

    def test_hex_lowercase(self):
        assert be_str2int("0xffffffff") == -1

    def test_bytes_input(self):
        assert be_str2int(b"0xFFFFFFFF") == -1


# ============================================================================
# 3. Lexer tokenization wrapping tests
# ============================================================================

class TestLexerHexWrapping:
    """Hex literals tokenized through the full lexer must be wrapped."""

    def test_0xFFFFFFFF(self):
        assert _tokenize_single_int("0xFFFFFFFF") == -1

    def test_0x80000000(self):
        assert _tokenize_single_int("0x80000000") == -2147483648

    def test_0x7FFFFFFF(self):
        assert _tokenize_single_int("0x7FFFFFFF") == 2147483647

    def test_0xFFFFFFFE(self):
        assert _tokenize_single_int("0xFFFFFFFE") == -2

    def test_small_hex(self):
        assert _tokenize_single_int("0xFF") == 255

    def test_zero_hex(self):
        assert _tokenize_single_int("0x0") == 0


# ============================================================================
# 4. Solidify output tests — LDINT for wrapped values
# ============================================================================

class TestSolidifyHexWrapping:
    """Solidified output for large hex literals must use LDINT with the
    wrapped signed value, matching the C binary."""

    def test_0xFFFFFFFF_uses_ldint_minus1(self):
        out = _compile_and_solidify("def f()\n  return 0xFFFFFFFF\nend")
        assert "LDINT" in out
        assert "-1" in out
        # Should NOT have a constant table entry for 4294967295
        assert "4294967295" not in out

    def test_0x7FFFFFFF_in_constant_table(self):
        # 2147483647 is too large for LDINT (18-bit sBx), goes to const table
        out = _compile_and_solidify("def f()\n  return 0x7FFFFFFF\nend")
        assert "2147483647" in out

    def test_small_hex_uses_ldint(self):
        out = _compile_and_solidify("def f()\n  return 0xFF\nend")
        assert "LDINT" in out
        assert "255" in out


# ============================================================================
# 5. Cross-validation with C binary
# ============================================================================

_HEX_SOLIDIFY_VECTORS = [
    ("0xFFFFFFFF returns -1", "def f()\n  return 0xFFFFFFFF\nend"),
    ("0x80000000 returns INT_MIN", "def f()\n  return 0x80000000\nend"),
    ("0xFFFFFFFE returns -2", "def f()\n  return 0xFFFFFFFE\nend"),
    ("0x7FFFFFFF returns INT_MAX", "def f()\n  return 0x7FFFFFFF\nend"),
    ("0xDEADBEEF negative", "def f()\n  return 0xDEADBEEF\nend"),
]


def _strip_trailing_whitespace(s):
    lines = s.split('\n')
    lines = [line.rstrip() for line in lines]
    while lines and lines[-1] == '':
        lines.pop()
    return '\n'.join(lines) + '\n'


def _solidify_with_c_binary(source):
    if not _c_binary_available():
        return None
    berry_script = source + "\nimport solidify\nsolidify.dump(f)\n"
    result = subprocess.run(
        [_BERRY_BIN],
        input=berry_script,
        capture_output=True,
        text=True,
        timeout=10,
    )
    if result.returncode != 0:
        return None
    output = result.stdout
    idx = output.find("\n/****")
    if idx < 0:
        idx = output.find("\nbe_local_closure")
        if idx < 0:
            return None
    return output[idx + 1:]


if _c_binary_available():
    @pytest.mark.parametrize("desc,source", _HEX_SOLIDIFY_VECTORS)
    def test_hex_solidify_matches_c(desc, source):
        """Python solidify output for large hex literals must match C."""
        py_out = _compile_and_solidify(source)
        c_out = _solidify_with_c_binary(source)
        if c_out is None:
            pytest.skip("C binary solidification failed")
        py_norm = _strip_trailing_whitespace(py_out)
        c_norm = _strip_trailing_whitespace(c_out)
        assert py_norm == c_norm, (
            f"Mismatch for '{desc}':\n--- Python ---\n{py_out}\n--- C ---\n{c_out}"
        )
