"""
Feature: berry-python-port, Property 17: Instruction disassembly equivalence

For any valid 32-bit Berry instruction, be_print_inst should produce the same
text output string as the C implementation.

The test builds a reference disassembler directly from the C format strings
and verifies that the Python be_print_inst produces identical output for
randomly generated instructions across all 48 opcodes.

Validates: Requirements 19.1
"""

import io
import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

from hypothesis import given, settings, assume
from hypothesis.strategies import integers, sampled_from, composite

from berry_port.be_decoder import (
    IGET_OP, IGET_RA, IGET_RKB, IGET_RKC, IGET_Bx, IGET_sBx,
    ISET_OP, ISET_RA, ISET_RKB, ISET_RKC, ISET_Bx, ISET_sBx,
    isKB, isKC, KR_MASK,
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD,
    OP_LT, OP_LE, OP_EQ, OP_NE, OP_GT, OP_GE,
    OP_AND, OP_OR, OP_XOR, OP_SHL, OP_SHR,
    OP_CONNECT, OP_NEG, OP_FLIP,
    OP_LDNIL, OP_LDBOOL, OP_LDINT, OP_LDCONST,
    OP_MOVE,
    OP_GETGBL, OP_SETGBL, OP_GETUPV, OP_SETUPV,
    OP_JMP, OP_JMPT, OP_JMPF,
    OP_CALL, OP_RET, OP_CLOSURE,
    OP_GETMBR, OP_GETMET, OP_SETMBR,
    OP_GETIDX, OP_SETIDX,
    OP_SETSUPER, OP_CLOSE, OP_IMPORT,
    OP_EXBLK, OP_CATCH, OP_RAISE,
    OP_CLASS,
    OP_GETNGBL, OP_SETNGBL,
    OP_COUNT,
    IsBx_MAX,
    opcode_names,
)

from berry_port.be_debug import be_print_inst


# ---------------------------------------------------------------------------
# Reference disassembler — reimplements the C format strings independently
# ---------------------------------------------------------------------------

# Opcode categories matching the C switch statement
_ABC_OPS = frozenset({
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD,
    OP_LT, OP_LE, OP_EQ, OP_NE, OP_GT, OP_GE,
    OP_CONNECT,
    OP_GETMBR, OP_SETMBR, OP_GETMET,
    OP_GETIDX, OP_SETIDX,
    OP_AND, OP_OR, OP_XOR, OP_SHL, OP_SHR,
})

_AB_RK_OPS = frozenset({
    OP_MOVE, OP_SETSUPER, OP_NEG, OP_FLIP, OP_IMPORT,
})


def _opc2str(op):
    if 0 <= op < len(opcode_names):
        return opcode_names[op]
    return "ERROP"


def reference_disasm(ins, pc):
    """Independent reference disassembler matching C format strings exactly."""
    op = IGET_OP(ins)
    ra = IGET_RA(ins)
    rkb = IGET_RKB(ins)
    rkc = IGET_RKC(ins)
    bx = IGET_Bx(ins)
    sbx = IGET_sBx(ins)
    kb = 'K' if isKB(ins) else 'R'
    kc = 'K' if isKC(ins) else 'R'
    rkb_masked = rkb & KR_MASK
    rkc_masked = rkc & KR_MASK
    name = _opc2str(op)

    prefix = "  %04X  " % (pc & 0xFFFF)

    if op in _ABC_OPS:
        # "%s\tR%d\t%c%d\t%c%d"
        body = "%s\tR%d\t%c%d\t%c%d" % (name, ra, kb, rkb_masked, kc, rkc_masked)
    elif op in (OP_GETNGBL, OP_SETNGBL):
        # "%s\tR%d\t%c%d"
        body = "%s\tR%d\t%c%d" % (name, ra, kb, rkb_masked)
    elif op in (OP_GETGBL, OP_SETGBL):
        # "%s\tR%d\tG%d"
        body = "%s\tR%d\tG%d" % (name, ra, bx)
    elif op in _AB_RK_OPS:
        # "%s\tR%d\t%c%d"
        body = "%s\tR%d\t%c%d" % (name, ra, kb, rkb_masked)
    elif op == OP_JMP:
        # "%s\t\t#%.4X"
        body = "%s\t\t#%04X" % (name, (sbx + pc + 1) & 0xFFFF)
    elif op in (OP_JMPT, OP_JMPF):
        # "%s\tR%d\t#%.4X"
        body = "%s\tR%d\t#%04X" % (name, ra, (sbx + pc + 1) & 0xFFFF)
    elif op == OP_LDINT:
        # "%s\tR%d\t%d"
        body = "%s\tR%d\t%d" % (name, ra, sbx)
    elif op == OP_LDBOOL:
        # "%s\tR%d\t%d\t%d"
        body = "%s\tR%d\t%d\t%d" % (name, ra, rkb, rkc)
    elif op == OP_RET:
        if ra:
            # "%s\t%d\t%c%d"
            body = "%s\t%d\t%c%d" % (name, ra, kb, rkb_masked)
        else:
            # "%s\t%d"
            body = "%s\t%d" % (name, ra)
    elif op in (OP_GETUPV, OP_SETUPV):
        # "%s\tR%d\tU%d"
        body = "%s\tR%d\tU%d" % (name, ra, bx)
    elif op == OP_LDCONST:
        # "%s\tR%d\tK%d"
        body = "%s\tR%d\tK%d" % (name, ra, bx)
    elif op == OP_CALL:
        # "%s\tR%d\t%d"
        body = "%s\tR%d\t%d" % (name, ra, rkb)
    elif op == OP_CLOSURE:
        # "%s\tR%d\tP%d"
        body = "%s\tR%d\tP%d" % (name, ra, bx)
    elif op == OP_CLASS:
        # "%s\tK%d"
        body = "%s\tK%d" % (name, bx)
    elif op in (OP_CLOSE, OP_LDNIL):
        # "%s\tR%d"
        body = "%s\tR%d" % (name, ra)
    elif op == OP_RAISE:
        # "%s\t%d\t%c%d\t%c%d"
        body = "%s\t%d\t%c%d\t%c%d" % (name, ra, kb, rkb_masked, kc, rkc_masked)
    elif op == OP_EXBLK:
        if ra:
            # "%s\t%d\t%d"
            body = "%s\t%d\t%d" % (name, ra, bx)
        else:
            # "%s\t%d\t#%.4X"
            body = "%s\t%d\t#%04X" % (name, ra, (sbx + pc + 1) & 0xFFFF)
        pass
    elif op == OP_CATCH:
        # "%s\tR%d\t%d\t%d"
        body = "%s\tR%d\t%d\t%d" % (name, ra, rkb, rkc)
    else:
        # "%s"
        body = "%s" % name

    return prefix + body + "\n"


# ---------------------------------------------------------------------------
# Strategies
# ---------------------------------------------------------------------------
opcodes = integers(min_value=0, max_value=OP_COUNT - 1)
ra_vals = integers(min_value=0, max_value=255)
rkb_vals = integers(min_value=0, max_value=511)
rkc_vals = integers(min_value=0, max_value=511)
bx_vals = integers(min_value=0, max_value=(1 << 18) - 1)
pc_vals = integers(min_value=0, max_value=0xFFFF)


@composite
def valid_instruction(draw):
    """Generate a (instruction, pc) pair with a valid opcode."""
    op = draw(opcodes)
    ra = draw(ra_vals)
    rkb = draw(rkb_vals)
    rkc = draw(rkc_vals)
    pc = draw(pc_vals)
    ins = ISET_OP(op) | ISET_RA(ra) | ISET_RKB(rkb) | ISET_RKC(rkc)
    return ins, pc


# ---------------------------------------------------------------------------
# Property 17a: Disassembly matches reference for all opcodes (ABC format)
# ---------------------------------------------------------------------------
@settings(max_examples=500)
@given(data=valid_instruction())
def test_disassembly_matches_reference(data):
    """be_print_inst output must match the reference disassembler for any valid instruction."""
    ins, pc = data
    expected = reference_disasm(ins, pc)

    buf = io.StringIO()
    be_print_inst(ins, pc, fout=buf)
    actual = buf.getvalue()

    assert actual == expected, (
        f"Disassembly mismatch for ins=0x{ins:08X} pc={pc}:\n"
        f"  expected: {expected!r}\n"
        f"  actual:   {actual!r}"
    )


# ---------------------------------------------------------------------------
# Property 17b: Specific opcode coverage — ABC-format ops
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(
    op=sampled_from(sorted(_ABC_OPS)),
    ra=ra_vals, rkb=rkb_vals, rkc=rkc_vals, pc=pc_vals,
)
def test_abc_ops_disassembly(op, ra, rkb, rkc, pc):
    """ABC-format opcodes (arithmetic, comparison, member, index, bitwise) disassemble correctly."""
    ins = ISET_OP(op) | ISET_RA(ra) | ISET_RKB(rkb) | ISET_RKC(rkc)
    expected = reference_disasm(ins, pc)

    buf = io.StringIO()
    be_print_inst(ins, pc, fout=buf)
    assert buf.getvalue() == expected


# ---------------------------------------------------------------------------
# Property 17c: Jump instructions produce valid target addresses
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(ra=ra_vals, rkb=rkb_vals, rkc=rkc_vals, pc=pc_vals)
def test_jmp_disassembly(ra, rkb, rkc, pc):
    """OP_JMP, OP_JMPT, OP_JMPF disassemble with correct jump target formatting."""
    for op in (OP_JMP, OP_JMPT, OP_JMPF):
        ins = ISET_OP(op) | ISET_RA(ra) | ISET_RKB(rkb) | ISET_RKC(rkc)
        expected = reference_disasm(ins, pc)

        buf = io.StringIO()
        be_print_inst(ins, pc, fout=buf)
        assert buf.getvalue() == expected


# ---------------------------------------------------------------------------
# Property 17d: RET instruction with RA=0 vs RA!=0
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(ra=ra_vals, rkb=rkb_vals, rkc=rkc_vals, pc=pc_vals)
def test_ret_disassembly(ra, rkb, rkc, pc):
    """OP_RET has two formats depending on RA being zero or non-zero."""
    ins = ISET_OP(OP_RET) | ISET_RA(ra) | ISET_RKB(rkb) | ISET_RKC(rkc)
    expected = reference_disasm(ins, pc)

    buf = io.StringIO()
    be_print_inst(ins, pc, fout=buf)
    assert buf.getvalue() == expected


# ---------------------------------------------------------------------------
# Property 17e: EXBLK instruction with RA=0 vs RA!=0
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(ra=ra_vals, rkb=rkb_vals, rkc=rkc_vals, pc=pc_vals)
def test_exblk_disassembly(ra, rkb, rkc, pc):
    """OP_EXBLK has two formats depending on RA being zero or non-zero."""
    ins = ISET_OP(OP_EXBLK) | ISET_RA(ra) | ISET_RKB(rkb) | ISET_RKC(rkc)
    expected = reference_disasm(ins, pc)

    buf = io.StringIO()
    be_print_inst(ins, pc, fout=buf)
    assert buf.getvalue() == expected


# ---------------------------------------------------------------------------
# Property 17f: Out-of-range opcode falls through to default
# ---------------------------------------------------------------------------
def test_invalid_opcode_disassembly():
    """An instruction with opcode >= OP_COUNT should produce 'ERROP'."""
    # Opcode 63 (max 6-bit value) is out of range
    ins = ISET_OP(63) | ISET_RA(0) | ISET_RKB(0) | ISET_RKC(0)
    pc = 0

    buf = io.StringIO()
    be_print_inst(ins, pc, fout=buf)
    output = buf.getvalue()
    assert "ERROP" in output, f"Expected 'ERROP' in output for invalid opcode, got: {output!r}"


# ---------------------------------------------------------------------------
# Property 17g: Output always starts with PC prefix and ends with newline
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(data=valid_instruction())
def test_output_format_invariants(data):
    """Every disassembly line starts with '  XXXX  ' prefix and ends with newline."""
    ins, pc = data

    buf = io.StringIO()
    be_print_inst(ins, pc, fout=buf)
    output = buf.getvalue()

    # Must end with newline
    assert output.endswith("\n"), f"Output doesn't end with newline: {output!r}"

    # Must start with "  XXXX  " (2 spaces, 4 hex digits, 2 spaces)
    prefix = output[:8]
    expected_prefix = "  %04X  " % (pc & 0xFFFF)
    assert prefix == expected_prefix, (
        f"PC prefix mismatch: {prefix!r} != {expected_prefix!r}"
    )
