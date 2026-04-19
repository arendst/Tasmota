"""
Feature: berry-python-port — be_strlib.be_str2int hex wrapping

The Berry Python port has two parallel implementations of be_str2int:

  - berry_port.be_lexer.be_str2int  — used during source tokenization
  - berry_port.be_strlib.be_str2int — used by the runtime builtin `int(...)`
    and by string.format's %d/%x conversions

Both must wrap hex results to the signed range dictated by BE_INTGER_TYPE
(32-bit with the default config), matching C's natural integer overflow
on `sum = sum * 16 + c`.

C reference (src/be_strlib.c::be_str2int, hex branch):

    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
        str += 2;
        while ((c = be_char2hex(*str++)) >= 0) {
            sum = sum * 16 + c;        /* bint; overflow wraps naturally */
        }
        ...
        return sum;
    }

The original bug:
  be_strlib.be_str2int masked hex results with _to_signed64 unconditionally,
  ignoring BE_INTGER_TYPE. With BE_INTGER_TYPE=0 (32-bit) this produced
  4278255360 for "0xFF00FF00" while the literal 0xFF00FF00 (parsed by the
  lexer) produced -16711936, breaking:

      assert(int("0xFF00FF00") == 0xFF00FF00)

  in tests/int.be when run via `python3 -m berry_port -s -g tests/int.be`.

This file covers:
  - be_strlib.be_str2int hex wrapping for all BE_INTGER_TYPE widths
  - return_end form still produces a sensible end position
  - Parity with be_lexer.be_str2int on the same inputs
  - End-to-end: `int("0x...")` matches the hex literal in Berry source
  - End-to-end: tests/int.be passes under the Python port
"""

import os
import subprocess
import sys

import pytest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

from berry_port.berry_conf import BE_INTGER_TYPE
from berry_port import be_strlib
from berry_port import be_lexer


BERRY_CMD = [sys.executable, "-m", "berry_port"]

_BERRY_BIN = os.path.join(os.path.dirname(__file__), '..', '..', 'berry')


def _c_binary_available():
    return os.path.isfile(_BERRY_BIN) and os.access(_BERRY_BIN, os.X_OK)


def _run_berry(source, *extra_args, timeout=15):
    """Run Berry source via `python -m berry_port -e`."""
    return subprocess.run(
        [*BERRY_CMD, *extra_args, "-e", source],
        capture_output=True,
        text=True,
        timeout=timeout,
    )


# ============================================================================
# Expected wrapped values depend on BE_INTGER_TYPE
# ============================================================================
#
# BE_INTGER_TYPE = 0  → 32-bit signed (int)
# BE_INTGER_TYPE = 1  → 32-bit signed (long, treated as 32-bit in the port)
# BE_INTGER_TYPE = 2  → 64-bit signed (long long)

if BE_INTGER_TYPE <= 1:
    _BINT_BITS = 32
    _BINT_MAX = 2**31 - 1
    _BINT_MIN = -(2**31)
else:
    _BINT_BITS = 64
    _BINT_MAX = 2**63 - 1
    _BINT_MIN = -(2**63)


def _wrap(v):
    """Expected signed wrap for the active bint width."""
    mask = (1 << _BINT_BITS) - 1
    v &= mask
    if v >= (1 << (_BINT_BITS - 1)):
        v -= (1 << _BINT_BITS)
    return v


# ============================================================================
# 1. be_strlib.be_str2int hex wrapping — direct unit tests
# ============================================================================

class TestStrlibHexWrapping:
    """be_strlib.be_str2int must wrap hex results to the bint width."""

    def test_zero(self):
        assert be_strlib.be_str2int("0x0") == 0

    def test_small_hex(self):
        assert be_strlib.be_str2int("0xFF") == 255

    def test_lowercase_hex(self):
        assert be_strlib.be_str2int("0xff") == 255

    def test_capital_X_prefix(self):
        assert be_strlib.be_str2int("0X1") == 1

    def test_leading_zeros(self):
        assert be_strlib.be_str2int("0x000000F") == 15

    def test_regression_0xFF00FF00(self):
        """The exact case from tests/int.be line 15."""
        assert be_strlib.be_str2int("0xFF00FF00") == _wrap(0xFF00FF00)

    def test_all_ones(self):
        assert be_strlib.be_str2int("0xFFFFFFFF") == _wrap(0xFFFFFFFF)

    def test_sign_bit_set(self):
        assert be_strlib.be_str2int("0x80000000") == _wrap(0x80000000)

    def test_deadbeef(self):
        assert be_strlib.be_str2int("0xDEADBEEF") == _wrap(0xDEADBEEF)

    def test_max_positive_hex(self):
        # 0x7FFFFFFF is INT_MAX in 32-bit mode — stays positive
        assert be_strlib.be_str2int("0x7FFFFFFF") == 2147483647

    def test_just_above_sign_bit(self):
        assert be_strlib.be_str2int("0x80000001") == _wrap(0x80000001)

    def test_wraps_beyond_bint_width(self):
        # In 32-bit mode 0x1_FFFFFFFF wraps to -1 (low 32 bits).
        # In 64-bit mode it's simply 8589934591, no wrapping.
        expected = -1 if _BINT_BITS == 32 else 0x1FFFFFFFF
        assert be_strlib.be_str2int("0x1FFFFFFFF") == expected


# ============================================================================
# 2. Hex decoding stops at first non-hex character
# ============================================================================

class TestStrlibHexStopsAtNonHex:
    """Parsing halts at the first character that isn't a hex digit."""

    def test_stops_at_letter_g(self):
        assert be_strlib.be_str2int("0xFFg") == 255

    def test_stops_at_space(self):
        assert be_strlib.be_str2int("0xAB ") == 0xAB

    def test_empty_hex_returns_zero(self):
        # "0x" with nothing after: no digits consumed, sum stays 0
        assert be_strlib.be_str2int("0x") == 0

    def test_empty_hex_with_trailing_garbage(self):
        assert be_strlib.be_str2int("0xZ") == 0


# ============================================================================
# 3. return_end form
# ============================================================================

class TestStrlibReturnEnd:
    """The (value, end_pos) form must report how much was consumed."""

    def test_simple_hex(self):
        val, end = be_strlib.be_str2int("0xFF", return_end=True)
        assert val == 255
        assert end == 4  # entire string consumed

    def test_wrapped_hex(self):
        val, end = be_strlib.be_str2int("0xFF00FF00", return_end=True)
        assert val == _wrap(0xFF00FF00)
        assert end == len("0xFF00FF00")

    def test_trailing_non_hex(self):
        val, end = be_strlib.be_str2int("0xAB!", return_end=True)
        assert val == 0xAB
        # End must point at (or just before) the '!'; permit either convention
        assert end in (3, 4)

    def test_decimal_return_end(self):
        val, end = be_strlib.be_str2int("123", return_end=True)
        assert val == 123
        assert end >= 2  # some reasonable position within the string


# ============================================================================
# 4. Parity with be_lexer.be_str2int on the same inputs
# ============================================================================

_PARITY_INPUTS = [
    "0x0",
    "0xFF",
    "0xFF00FF00",
    "0xFFFFFFFF",
    "0x80000000",
    "0x7FFFFFFF",
    "0xDEADBEEF",
    "0x1FFFFFFFF",
    "42",
    "-42",
    "0",
    "-1",
    "2147483647",
]


class TestLexerStrlibParity:
    """Both be_str2int implementations must agree on every input, since
    C has a single implementation and the port's two copies must track it."""

    @pytest.mark.parametrize("text", _PARITY_INPUTS)
    def test_same_result(self, text):
        from_lexer = be_lexer.be_str2int(text)
        from_strlib = be_strlib.be_str2int(text)
        assert from_lexer == from_strlib, (
            f"{text!r}: lexer={from_lexer}, strlib={from_strlib}"
        )


# ============================================================================
# 5. End-to-end: int("0x...") in Berry matches the literal
# ============================================================================

_INT_LITERAL_VECTORS = [
    ("0x00", 0),
    ("0X1", 1),
    ("0x000000F", 15),
    ("0x1000", 0x1000),
    ("0xFF00FF00", _wrap(0xFF00FF00)),
    ("0xFFFFFFFF", _wrap(0xFFFFFFFF)),
    ("0x80000000", _wrap(0x80000000)),
    ("0x7FFFFFFF", 0x7FFFFFFF),
]


class TestIntBuiltinHexParsing:
    """In Berry, `int("0xN")` must equal the literal `0xN`."""

    @pytest.mark.parametrize("text,expected", _INT_LITERAL_VECTORS)
    def test_int_from_string_matches_literal(self, text, expected):
        src = f'print(int("{text}") == {text})'
        r = _run_berry(src)
        assert r.returncode == 0, f"stderr: {r.stderr}"
        assert r.stdout.strip() == "true", (
            f"int({text!r}) did not match literal {text}; "
            f"stdout={r.stdout!r} stderr={r.stderr!r}"
        )

    @pytest.mark.parametrize("text,expected", _INT_LITERAL_VECTORS)
    def test_int_from_string_prints_expected(self, text, expected):
        src = f'print(int("{text}"))'
        r = _run_berry(src)
        assert r.returncode == 0, f"stderr: {r.stderr}"
        assert r.stdout.strip() == str(expected), (
            f"int({text!r}) printed {r.stdout.strip()!r}, "
            f"expected {expected}"
        )


# ============================================================================
# 6. End-to-end regression: tests/int.be runs clean under the Python port
# ============================================================================

def test_tests_int_be_runs_clean():
    """`python3 -m berry_port -s -g tests/int.be` must succeed with no
    assertion failure on the 0xFF00FF00 hex-parsing check."""
    path = os.path.join(
        os.path.dirname(__file__), '..', '..', 'tests', 'int.be'
    )
    assert os.path.exists(path), f"missing {path}"
    r = subprocess.run(
        [*BERRY_CMD, "-s", "-g", path],
        capture_output=True, text=True, timeout=30,
    )
    assert r.returncode == 0, (
        f"tests/int.be failed under the Python port\n"
        f"stdout: {r.stdout}\nstderr: {r.stderr}"
    )
    # The regression fire-signal was this exact string on stderr
    assert "assert_failed" not in r.stderr, (
        f"assertion still failing: {r.stderr}"
    )
    assert "assert_failed" not in r.stdout, (
        f"assertion still failing: {r.stdout}"
    )


# ============================================================================
# 7. Cross-validation with C binary (if available)
# ============================================================================

_CROSS_INPUTS = [
    "0x00",
    "0xFF",
    "0xFF00FF00",
    "0xFFFFFFFF",
    "0x80000000",
    "0x7FFFFFFF",
    "0xDEADBEEF",
    "0",
    "42",
    "-1",
]


def _int_via_c(text):
    """Evaluate `int("text")` in the compiled C berry and return the output."""
    if not _c_binary_available():
        return None
    result = subprocess.run(
        [_BERRY_BIN, "-e", f'print(int("{text}"))'],
        capture_output=True, text=True, timeout=10,
    )
    if result.returncode != 0:
        return None
    return result.stdout.strip()


if _c_binary_available():
    @pytest.mark.parametrize("text", _CROSS_INPUTS)
    def test_int_string_parse_matches_c(text):
        """For every hex/decimal input, `int("x")` in the Python port must
        print the same value as the C binary."""
        c_out = _int_via_c(text)
        if c_out is None:
            pytest.skip("C binary failed to evaluate")

        r = _run_berry(f'print(int("{text}"))')
        assert r.returncode == 0, f"stderr: {r.stderr}"
        py_out = r.stdout.strip()
        assert py_out == c_out, (
            f"mismatch on int({text!r}): python={py_out!r} C={c_out!r}"
        )
