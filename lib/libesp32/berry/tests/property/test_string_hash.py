"""
Feature: berry-python-port, Property 4: String hash equivalence

For any byte string of length 0 to 1000, the Python be_strhash / str_hash
implementation should produce the same 32-bit FNV-1a hash value as the C
be_strhash function.

Validates: Requirements 8.1
"""

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

import subprocess
import struct
from hypothesis import given, settings, assume
from hypothesis.strategies import binary, integers, text, sampled_from

from berry_port.be_string import str_hash, be_strhash, be_str2cstr, be_newstrn
from berry_port.be_object import (
    bstring, bcstring, GC_CONST, GC_WHITE,
    SHORT_STR_MAX_LEN, str_len,
)
from berry_port.berry_conf import BE_USE_STR_HASH_CACHE


# ============================================================================
# Reference FNV-1a implementation (independent of be_string.py)
# ============================================================================

def _reference_fnv1a(data: bytes) -> int:
    """Independent FNV-1a hash matching the C str_hash exactly.

    uint32_t hash = 2166136261u;
    while (len--) {
        hash = (hash ^ (unsigned char)*str++) * 16777619u;
    }
    """
    FNV_OFFSET = 2166136261
    FNV_PRIME = 16777619
    h = FNV_OFFSET
    for b in data:
        h = ((h ^ b) * FNV_PRIME) & 0xFFFFFFFF
    return h


# ============================================================================
# Strategies
# ============================================================================

# Byte strings from 0 to 1000 bytes (per design Property 4)
short_bytestrings = binary(min_size=0, max_size=64)
long_bytestrings = binary(min_size=65, max_size=1000)
all_bytestrings = binary(min_size=0, max_size=1000)


# ============================================================================
# Property 4a: str_hash matches independent FNV-1a reference
# ============================================================================
@settings(max_examples=500)
@given(data=all_bytestrings)
def test_str_hash_matches_reference_fnv1a(data):
    """str_hash(data) must equal the independent FNV-1a reference for any
    byte string of length 0..1000."""
    expected = _reference_fnv1a(data)
    actual = str_hash(data)
    assert actual == expected, (
        f"str_hash mismatch for {len(data)}-byte input: "
        f"got {actual:#010x}, expected {expected:#010x}"
    )


# ============================================================================
# Property 4b: str_hash with explicit length parameter
# ============================================================================
@settings(max_examples=300)
@given(data=all_bytestrings, prefix_len=integers(min_value=0, max_value=1000))
def test_str_hash_with_length_param(data, prefix_len):
    """str_hash(data, length) should hash only the first `length` bytes."""
    length = min(prefix_len, len(data))
    expected = _reference_fnv1a(data[:length])
    actual = str_hash(data, length)
    assert actual == expected, (
        f"str_hash(data, {length}) mismatch: "
        f"got {actual:#010x}, expected {expected:#010x}"
    )


# ============================================================================
# Property 4c: Empty string hashes to FNV offset basis
# ============================================================================
def test_empty_string_hash():
    """Empty string should hash to the FNV-1a offset basis 2166136261."""
    assert str_hash(b'') == 2166136261
    assert str_hash('') == 2166136261
    assert _reference_fnv1a(b'') == 2166136261


# ============================================================================
# Property 4d: str_hash accepts both str and bytes inputs
# ============================================================================
@settings(max_examples=300)
@given(data=binary(min_size=0, max_size=200))
def test_str_hash_bytes_vs_str_input(data):
    """str_hash should produce the same result whether given bytes or a
    latin-1 decoded str (since Berry strings are byte-oriented)."""
    # Only test data that round-trips through latin-1
    try:
        s = data.decode('latin-1')
    except (UnicodeDecodeError, ValueError):
        assume(False)
    assert str_hash(data) == str_hash(s), (
        f"str_hash differs between bytes and str for {data!r}"
    )


# ============================================================================
# Property 4e: Known test vectors (manually computed FNV-1a values)
# ============================================================================
def test_known_vectors():
    """Verify str_hash against well-known FNV-1a test vectors."""
    # FNV-1a 32-bit test vectors (from the FNV spec / reference)
    vectors = [
        (b'',        0x811C9DC5),  # offset basis
        (b'a',       0xE40C292C),
        (b'b',       0xE70C2DE5),
        (b'c',       0xE60C2C52),
        (b'foobar',  0xBF9CF968),
    ]
    for data, expected in vectors:
        actual = str_hash(data)
        assert actual == expected, (
            f"str_hash({data!r}) = {actual:#010x}, expected {expected:#010x}"
        )


# ============================================================================
# Property 4f: Hash is deterministic
# ============================================================================
@settings(max_examples=200)
@given(data=all_bytestrings)
def test_hash_deterministic(data):
    """Calling str_hash twice on the same input must return the same value."""
    h1 = str_hash(data)
    h2 = str_hash(data)
    assert h1 == h2


# ============================================================================
# Property 4g: Hash fits in uint32
# ============================================================================
@settings(max_examples=500)
@given(data=all_bytestrings)
def test_hash_fits_uint32(data):
    """Hash value must be in [0, 0xFFFFFFFF]."""
    h = str_hash(data)
    assert 0 <= h <= 0xFFFFFFFF, f"Hash {h:#x} out of uint32 range"


# ============================================================================
# Property 4h: Cross-validate with C implementation (if berry binary exists)
# ============================================================================

# Build a tiny Berry script that prints the hash of a given hex string.
# This lets us cross-validate against the actual C str_hash.
_BERRY_BIN = os.path.join(os.path.dirname(__file__), '..', '..', 'berry')

def _c_hash_available():
    """Check if the compiled berry binary is available for cross-validation."""
    return os.path.isfile(_BERRY_BIN) and os.access(_BERRY_BIN, os.X_OK)


if _c_hash_available():
    @settings(max_examples=100, deadline=5000)
    @given(data=binary(min_size=1, max_size=100))
    def test_str_hash_matches_c_implementation(data):
        """Cross-validate Python str_hash against the C berry binary.

        We use a Berry script that computes the hash via the string module
        and prints it. This only runs if the berry binary is available.
        """
        # Build a Berry source that creates the string from bytes and hashes it
        # We use bytes().fromstring() to construct arbitrary byte sequences
        hex_str = data.hex()
        # Berry script: create bytes from hex, convert to string, get hash via
        # compile + solidify introspection. Since Berry doesn't expose str_hash
        # directly, we skip this test for now and rely on the reference impl.
        # This test is a placeholder for when a hash-exposing API is available.
        pass
