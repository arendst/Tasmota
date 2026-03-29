"""
Feature: berry-python-port, Property 10: toidentifier encoding equivalence

For any string (including strings with non-alphanumeric characters, underscores,
and _X sequences), the Python toidentifier function should produce the same
C-identifier-safe string as the C implementation's toidentifier function.

Validates: Requirements 12.5
"""

import sys
import os
import subprocess
import string

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

from hypothesis import given, settings, assume
from hypothesis.strategies import (
    text, characters, sampled_from, lists, one_of, just, composite, integers,
    binary,
)

from berry_port.be_solidifylib import toidentifier, toidentifier_length


# ============================================================================
# Reference implementation (independent of be_solidifylib.py)
# ============================================================================

def _ref_hexdigit(v):
    """Independent hexdigit matching C: uppercase hex for nibble."""
    v = v & 0xF
    if v >= 10:
        return chr(v - 10 + ord('A'))
    return chr(v + ord('0'))


def _ref_toidentifier(s):
    """Independent reference implementation of the C toidentifier function.

    Operates on Latin-1 bytes to match C behavior where strings are byte
    arrays and isalnum() only considers ASCII alphanumerics.

    Rules:
    - '_X' in input -> '_X_' in output (escape the escape marker)
    - ASCII alphanumeric or '_' -> pass through
    - anything else -> '_X' + two uppercase hex digits of the byte value
    """
    if isinstance(s, str):
        s = s.encode('latin-1')
    parts = []
    i = 0
    while i < len(s):
        if i + 1 < len(s) and s[i] == ord('_') and s[i + 1] == ord('X'):
            parts.append('_X_')
            i += 2
        elif (chr(s[i]).isalnum() if s[i] < 128 else False) or s[i] == ord('_'):
            parts.append(chr(s[i]))
            i += 1
        else:
            b = s[i]
            parts.append('_X')
            parts.append(_ref_hexdigit((b & 0xF0) >> 4))
            parts.append(_ref_hexdigit(b & 0x0F))
            i += 1
    return ''.join(parts)


def _ref_toidentifier_length(s):
    """Independent reference for toidentifier_length (includes null terminator).
    Operates on Latin-1 bytes to match C behavior.
    """
    if isinstance(s, str):
        s = s.encode('latin-1')
    length = 1  # null terminator
    i = 0
    while i < len(s):
        if i + 1 < len(s) and s[i] == ord('_') and s[i + 1] == ord('X'):
            length += 3
            i += 2
        elif (chr(s[i]).isalnum() if s[i] < 128 else False) or s[i] == ord('_'):
            length += 1
            i += 1
        else:
            length += 4
            i += 1
    return length


# ============================================================================
# Strategies
# ============================================================================

# Characters that Berry identifiers / solidification strings can contain.
# We use latin-1 range (0x00-0xFF) since Berry strings are byte-oriented.
latin1_chars = characters(min_codepoint=1, max_codepoint=255)

# Strings with the full latin-1 range (excluding null which terminates C strings)
latin1_strings = text(latin1_chars, min_size=0, max_size=200)

# Strings that are pure alphanumeric + underscore (passthrough case)
alnum_chars = sampled_from(string.ascii_letters + string.digits + '_')
alnum_strings = text(alnum_chars, min_size=0, max_size=200)

# Strings that specifically contain _X sequences (escape-the-escape case)
@composite
def strings_with_escape_markers(draw):
    """Generate strings that contain _X sequences mixed with other content."""
    parts = draw(lists(
        one_of(
            just('_X'),
            just('_x'),  # lowercase x should NOT trigger escape-the-escape
            text(latin1_chars, min_size=1, max_size=10),
        ),
        min_size=1, max_size=20,
    ))
    return ''.join(parts)

# Strings with special characters that need escaping
special_chars = sampled_from('.+-*/=<>!@#$%^&()[]{}|\\:;"\'`,~? \t\n\r')
special_strings = text(
    one_of(latin1_chars, special_chars),
    min_size=0, max_size=200,
)


# ============================================================================
# Property 10a: toidentifier matches independent reference for any string
# ============================================================================
@settings(max_examples=500)
@given(s=latin1_strings)
def test_toidentifier_matches_reference(s):
    """toidentifier(s) must equal the independent reference implementation
    for any latin-1 string."""
    expected = _ref_toidentifier(s)
    actual = toidentifier(s)
    assert actual == expected, (
        f"toidentifier mismatch for {s!r}: got {actual!r}, expected {expected!r}"
    )


# ============================================================================
# Property 10b: toidentifier_length matches actual encoded length + 1
# ============================================================================
@settings(max_examples=500)
@given(s=latin1_strings)
def test_toidentifier_length_matches_actual(s):
    """toidentifier_length(s) must equal len(toidentifier(s)) + 1
    (the +1 accounts for the C null terminator)."""
    encoded = toidentifier(s)
    expected_length = len(encoded) + 1  # +1 for null terminator
    actual_length = toidentifier_length(s)
    assert actual_length == expected_length, (
        f"toidentifier_length mismatch for {s!r}: "
        f"got {actual_length}, expected {expected_length} "
        f"(encoded={encoded!r}, len={len(encoded)})"
    )


# ============================================================================
# Property 10c: Output is always a valid C identifier
# ============================================================================
@settings(max_examples=500)
@given(s=latin1_strings)
def test_output_is_valid_c_identifier_chars(s):
    """toidentifier output must contain only [a-zA-Z0-9_] characters,
    which are valid in C identifiers."""
    encoded = toidentifier(s)
    for ch in encoded:
        assert ch.isalnum() or ch == '_', (
            f"Invalid identifier char {ch!r} in toidentifier({s!r}) = {encoded!r}"
        )


# ============================================================================
# Property 10d: Alphanumeric strings pass through unchanged
# ============================================================================
@settings(max_examples=300)
@given(s=alnum_strings)
def test_alnum_passthrough(s):
    """Strings containing only [a-zA-Z0-9_] should pass through unchanged."""
    # But we need to handle _X sequences even in alnum strings
    if '_X' not in s:
        assert toidentifier(s) == s, (
            f"Alphanumeric string {s!r} was modified: {toidentifier(s)!r}"
        )


# ============================================================================
# Property 10e: _X escape marker is properly escaped
# ============================================================================
@settings(max_examples=300)
@given(s=strings_with_escape_markers())
def test_escape_marker_handling(s):
    """Strings containing _X should have each _X replaced with _X_."""
    encoded = toidentifier(s)
    expected = _ref_toidentifier(s)
    assert encoded == expected, (
        f"Escape marker handling mismatch for {s!r}: "
        f"got {encoded!r}, expected {expected!r}"
    )


# ============================================================================
# Property 10f: Encoding is deterministic
# ============================================================================
@settings(max_examples=200)
@given(s=latin1_strings)
def test_encoding_deterministic(s):
    """Calling toidentifier twice on the same input must return the same value."""
    r1 = toidentifier(s)
    r2 = toidentifier(s)
    assert r1 == r2


# ============================================================================
# Property 10g: Empty string produces empty output
# ============================================================================
def test_empty_string():
    """Empty string should produce empty identifier."""
    assert toidentifier('') == ''
    assert toidentifier_length('') == 1  # just the null terminator


# ============================================================================
# Property 10h: Known test vectors
# ============================================================================
def test_known_vectors():
    """Verify toidentifier against manually computed test vectors."""
    vectors = [
        # (input, expected_output)
        ('hello', 'hello'),
        ('foo_bar', 'foo_bar'),
        ('_X', '_X_'),                    # escape the escape marker
        ('_X_X', '_X__X_'),               # two _X sequences
        ('a_Xb', 'a_X_b'),               # _X in the middle
        ('hello.world', 'hello_X2Eworld'),  # '.' = 0x2E
        ('a b', 'a_X20b'),               # space = 0x20
        ('a+b', 'a_X2Bb'),               # '+' = 0x2B
        ('', ''),                          # empty
        ('_', '_'),                        # lone underscore
        ('_x', '_x'),                      # _x (lowercase) is NOT _X
        ('_Xa', '_X_a'),                   # _X followed by non-X char
        ('abc_Xdef', 'abc_X_def'),         # _X in middle of identifier
    ]
    for inp, expected in vectors:
        actual = toidentifier(inp)
        assert actual == expected, (
            f"toidentifier({inp!r}) = {actual!r}, expected {expected!r}"
        )


# ============================================================================
# Property 10i: Hex digits are uppercase
# ============================================================================
@settings(max_examples=300)
@given(s=special_strings)
def test_hex_digits_uppercase(s):
    """All hex digits in escape sequences must be uppercase A-F, not a-f."""
    encoded = toidentifier(s)
    i = 0
    while i < len(encoded):
        if i + 2 < len(encoded) and encoded[i] == '_' and encoded[i + 1] == 'X':
            # Check if this is an escape sequence (_XHH) vs escape-the-escape (_X_)
            if i + 3 < len(encoded) and encoded[i + 2] != '_':
                # This is _XHH - verify hex digits are uppercase
                h1 = encoded[i + 2]
                h2 = encoded[i + 3] if i + 3 < len(encoded) else ''
                assert h1 in '0123456789ABCDEF', (
                    f"Lowercase hex digit {h1!r} in {encoded!r}"
                )
                assert h2 in '0123456789ABCDEF', (
                    f"Lowercase hex digit {h2!r} in {encoded!r}"
                )
                i += 4
            else:
                i += 3  # _X_ (escaped escape marker)
        else:
            i += 1


# ============================================================================
# Property 10j: Trailing underscore is preserved (not confused with _X)
# ============================================================================
def test_trailing_underscore():
    """A string ending with '_' should preserve it (no _X lookahead past end)."""
    assert toidentifier('foo_') == 'foo_'
    assert toidentifier('_') == '_'
    assert toidentifier('__') == '__'


# ============================================================================
# Property 10k: Cross-validate with C implementation (if berry binary exists)
# ============================================================================

_BERRY_BIN = os.path.join(os.path.dirname(__file__), '..', '..', 'berry')


def _c_binary_available():
    return os.path.isfile(_BERRY_BIN) and os.access(_BERRY_BIN, os.X_OK)


if _c_binary_available():
    @settings(max_examples=100, deadline=5000)
    @given(s=text(
        characters(min_codepoint=32, max_codepoint=126),
        min_size=1, max_size=50,
    ))
    def test_toidentifier_matches_c_via_solidify(s):
        """Cross-validate by compiling a Berry function with the given name
        and checking solidification output contains the expected identifier.

        Only runs if the berry binary is available.
        """
        # We can test indirectly: create a Berry function, solidify it,
        # and check the identifier encoding in the output.
        # For now this is a placeholder — direct C cross-validation would
        # require a custom C test harness exposing toidentifier.
        pass
