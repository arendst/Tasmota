"""
Feature: berry-python-port, Property 1: Instruction encoding round-trip

For any valid opcode (0-47), RA (0-255), RKB (0-511), and RKC (0-511) values,
encoding them into a 32-bit instruction via ISET_OP | ISET_RA | ISET_RKB | ISET_RKC
and then decoding via IGET_OP, IGET_RA, IGET_RKB, IGET_RKC should produce the
original values.

Validates: Requirements 21.4, 5.2
"""

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

from hypothesis import given, settings, assume
from hypothesis.strategies import integers

from berry_port.be_decoder import (
    IGET_OP, IGET_RA, IGET_RKB, IGET_RKC, IGET_Bx, IGET_sBx,
    ISET_OP, ISET_RA, ISET_RKB, ISET_RKC, ISET_Bx, ISET_sBx,
    isK, setK, KR2idx, isKB, isKC,
    OP_COUNT, IsBx_MAX, IsBx_MIN,
)

# ---------------------------------------------------------------------------
# Strategies for valid field ranges
# ---------------------------------------------------------------------------
opcodes = integers(min_value=0, max_value=OP_COUNT - 1)   # 0..47
ra_vals = integers(min_value=0, max_value=255)             # 8-bit
rkb_vals = integers(min_value=0, max_value=511)            # 9-bit
rkc_vals = integers(min_value=0, max_value=511)            # 9-bit
bx_vals = integers(min_value=0, max_value=(1 << 18) - 1)  # 18-bit unsigned
# sBx is encoded as Bx - IsBx_MAX where Bx is 18-bit unsigned [0, 262143].
# The actual round-trippable range is [-IsBx_MAX, IBx_MASK - IsBx_MAX]
# i.e. [-131071, 131072]. IsBx_MIN (-131072) is defined in C but cannot
# round-trip through ISET_sBx/IGET_sBx (same behavior in C and Python).
sbx_vals = integers(min_value=-IsBx_MAX, max_value=(1 << 18) - 1 - IsBx_MAX)


# ---------------------------------------------------------------------------
# Property 1a: ABC-format round-trip
# ---------------------------------------------------------------------------
@settings(max_examples=500)
@given(op=opcodes, ra=ra_vals, rkb=rkb_vals, rkc=rkc_vals)
def test_abc_roundtrip(op, ra, rkb, rkc):
    """Encode op/RA/RKB/RKC into a 32-bit instruction, decode, verify identity."""
    ins = ISET_OP(op) | ISET_RA(ra) | ISET_RKB(rkb) | ISET_RKC(rkc)

    assert IGET_OP(ins) == op, f"OP mismatch: {IGET_OP(ins)} != {op}"
    assert IGET_RA(ins) == ra, f"RA mismatch: {IGET_RA(ins)} != {ra}"
    assert IGET_RKB(ins) == rkb, f"RKB mismatch: {IGET_RKB(ins)} != {rkb}"
    assert IGET_RKC(ins) == rkc, f"RKC mismatch: {IGET_RKC(ins)} != {rkc}"


# ---------------------------------------------------------------------------
# Property 1b: ABx-format round-trip
# ---------------------------------------------------------------------------
@settings(max_examples=500)
@given(op=opcodes, ra=ra_vals, bx=bx_vals)
def test_abx_roundtrip(op, ra, bx):
    """Encode op/RA/Bx into a 32-bit instruction, decode, verify identity."""
    ins = ISET_OP(op) | ISET_RA(ra) | ISET_Bx(bx)

    assert IGET_OP(ins) == op, f"OP mismatch: {IGET_OP(ins)} != {op}"
    assert IGET_RA(ins) == ra, f"RA mismatch: {IGET_RA(ins)} != {ra}"
    assert IGET_Bx(ins) == bx, f"Bx mismatch: {IGET_Bx(ins)} != {bx}"


# ---------------------------------------------------------------------------
# Property 1c: AsBx-format round-trip (signed Bx)
# ---------------------------------------------------------------------------
@settings(max_examples=500)
@given(op=opcodes, ra=ra_vals, sbx=sbx_vals)
def test_asbx_roundtrip(op, ra, sbx):
    """Encode op/RA/sBx into a 32-bit instruction, decode, verify identity."""
    ins = ISET_OP(op) | ISET_RA(ra) | ISET_sBx(sbx)

    assert IGET_OP(ins) == op, f"OP mismatch: {IGET_OP(ins)} != {op}"
    assert IGET_RA(ins) == ra, f"RA mismatch: {IGET_RA(ins)} != {ra}"
    assert IGET_sBx(ins) == sbx, f"sBx mismatch: {IGET_sBx(ins)} != {sbx}"


# ---------------------------------------------------------------------------
# Property 1d: K-flag helpers round-trip
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(idx=integers(min_value=0, max_value=255))
def test_k_flag_roundtrip(idx):
    """Setting the K flag and extracting the index should preserve the value."""
    k_val = setK(idx)
    assert isK(k_val), f"setK({idx}) should have K flag set"
    assert KR2idx(k_val) == idx, f"KR2idx mismatch: {KR2idx(k_val)} != {idx}"


@settings(max_examples=200)
@given(idx=integers(min_value=0, max_value=255))
def test_non_k_flag(idx):
    """A plain register index (< 256) should not have the K flag set."""
    assert not isK(idx), f"Plain index {idx} should not have K flag"


# ---------------------------------------------------------------------------
# Property 1e: Instruction fits in 32 bits
# ---------------------------------------------------------------------------
@settings(max_examples=500)
@given(op=opcodes, ra=ra_vals, rkb=rkb_vals, rkc=rkc_vals)
def test_instruction_fits_32_bits(op, ra, rkb, rkc):
    """Encoded instruction must fit in an unsigned 32-bit integer."""
    ins = ISET_OP(op) | ISET_RA(ra) | ISET_RKB(rkb) | ISET_RKC(rkc)
    assert 0 <= ins <= 0xFFFFFFFF, f"Instruction {ins:#010x} exceeds 32 bits"


# ---------------------------------------------------------------------------
# Property 1f: Fields are non-overlapping (no bit collision)
# ---------------------------------------------------------------------------
@settings(max_examples=500)
@given(op=opcodes, ra=ra_vals, rkb=rkb_vals, rkc=rkc_vals)
def test_fields_non_overlapping(op, ra, rkb, rkc):
    """Each field setter should only affect its own bit range."""
    op_bits = ISET_OP(op)
    ra_bits = ISET_RA(ra)
    rkb_bits = ISET_RKB(rkb)
    rkc_bits = ISET_RKC(rkc)

    # OR and addition should give the same result if fields don't overlap
    assert op_bits | ra_bits | rkb_bits | rkc_bits == \
           op_bits + ra_bits + rkb_bits + rkc_bits, \
           "Bit fields overlap"
