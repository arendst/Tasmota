"""re1.5 recursive-loop VM.

Port of re1.5/recursiveloop.c — executes ByteProg bytecode against input strings.
Uses integer indices into the string instead of C char pointers.
"""

import struct

from berry_port.re15_opcodes import (
    Char, Any, Class, ClassNot, NamedClass,
    Bol, Eol, Jmp, Split, RSplit, Save, Match,
    NON_ANCHORED_PREFIX, inst_is_consumer,
)
from berry_port.re15_classes import classmatch, namedclassmatch


def _recursiveloop(insts, pc, sp, text, begin, end, subp, nsubp):
    """Core recursive-loop VM execution.

    Args:
        insts: bytearray of bytecode instructions.
        pc: program counter (index into insts).
        sp: string pointer (index into text).
        text: the input string.
        begin: start index of the subject in text.
        end: end index of the subject in text.
        subp: list of submatch positions (int or None), modified in-place.
        nsubp: size of subp.

    Returns:
        1 on match, 0 on no match.
    """
    while True:
        opcode = insts[pc]

        # Consumer opcodes must check sp < end before consuming
        if inst_is_consumer(opcode):
            if sp >= end:
                return 0

        pc += 1

        if opcode == Char:
            if ord(text[sp]) != insts[pc]:
                return 0
            pc += 1
            sp += 1
            continue

        elif opcode == Any:
            sp += 1
            continue

        elif opcode == Class or opcode == ClassNot:
            if not classmatch(insts, pc, text[sp]):
                return 0
            # Advance pc past the count byte and range pairs
            pc += insts[pc] * 2 + 1
            sp += 1
            continue

        elif opcode == NamedClass:
            if not namedclassmatch(insts, pc, text[sp]):
                return 0
            pc += 1
            sp += 1
            continue

        elif opcode == Match:
            return 1

        elif opcode == Jmp:
            off = struct.unpack_from('b', insts, pc)[0]
            pc += 1
            pc = pc + off
            continue

        elif opcode == Split:
            off = struct.unpack_from('b', insts, pc)[0]
            pc += 1
            # Try pc first (next instruction), then branch target on failure
            if _recursiveloop(insts, pc, sp, text, begin, end, subp, nsubp):
                return 1
            pc = pc + off
            continue

        elif opcode == RSplit:
            off = struct.unpack_from('b', insts, pc)[0]
            pc += 1
            # Try branch target first, then pc (next instruction) on failure
            if _recursiveloop(insts, pc + off, sp, text, begin, end, subp, nsubp):
                return 1
            continue

        elif opcode == Save:
            off = insts[pc]
            pc += 1
            if off >= nsubp:
                continue
            old = subp[off]
            subp[off] = sp
            if _recursiveloop(insts, pc, sp, text, begin, end, subp, nsubp):
                return 1
            subp[off] = old
            return 0

        elif opcode == Bol:
            if sp != begin:
                return 0
            continue

        elif opcode == Eol:
            if sp != end:
                return 0
            continue

        else:
            raise RuntimeError(f"recursiveloop: unknown opcode 0x{opcode:02x} at pc={pc - 1}")


def recursiveloopprog(prog, text, subp, nsubp, is_anchored):
    """Execute bytecode program against input text.

    Args:
        prog: ByteProg instance with compiled bytecode.
        text: the input string to match against.
        subp: list of submatch positions, modified in-place with integer indices.
        nsubp: size of subp.
        is_anchored: if True, skip the NON_ANCHORED_PREFIX (anchored match);
                     if False, start from byte 0 (unanchored search).

    Returns:
        1 on match, 0 on no match.
    """
    start_pc = NON_ANCHORED_PREFIX if is_anchored else 0
    begin = 0
    end = len(text)
    return _recursiveloop(prog.insts, start_pc, begin, text, begin, end, subp, nsubp)
