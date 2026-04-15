"""re1.5 character class matching helpers and bytecode dump utility.

Port of re1.5/charclass.c — classmatch and namedclassmatch functions.
Port of re1.5/dumpcode.c — dumpcode function.
"""

import struct

from berry_port.re15_opcodes import (
    Class, ClassNot, NamedClass, Char, Any, Match, Save,
    Split, RSplit, Jmp, Bol, Eol,
)


def classmatch(insts, pc, ch):
    """Match character against a Class/ClassNot character class.

    Args:
        insts: bytearray of bytecode instructions
        pc: index pointing to the count byte (immediately after the Class/ClassNot opcode)
        ch: single character to test

    Returns:
        True if the character matches the class (accounting for Class vs ClassNot).
    """
    # pc[-1] is the opcode byte: Class means positive match, ClassNot means negated
    is_positive = insts[pc - 1] == Class
    cnt = insts[pc]
    pc += 1
    c = ord(ch)
    while cnt > 0:
        if c >= insts[pc] and c <= insts[pc + 1]:
            return is_positive
        pc += 2
        cnt -= 1
    return not is_positive


def namedclassmatch(insts, pc, ch):
    """Match character against a named character class (\\d, \\D, \\s, \\S, \\w, \\W).

    Uses the same bit-manipulation trick as the C version:
    - (class_byte >> 5) & 1 is 1 for uppercase (negated) classes, 0 for lowercase
    - XOR flips the result when the character does NOT belong to the base class

    Args:
        insts: bytearray of bytecode instructions
        pc: index pointing to the class name byte (immediately after the NamedClass opcode)
        ch: single character to test

    Returns:
        True (1) if the character matches, False (0) if not.
    """
    class_byte = insts[pc]
    off = (class_byte >> 5) & 1
    c = ord(ch)
    lower = class_byte | 0x20  # lowercase version of the class letter

    if lower == ord('d'):
        if not (c >= ord('0') and c <= ord('9')):
            off ^= 1
    elif lower == ord('s'):
        if not (c == ord(' ') or (c >= ord('\t') and c <= ord('\r'))):
            off ^= 1
    else:  # 'w'
        if not ((c >= ord('A') and c <= ord('Z')) or
                (c >= ord('a') and c <= ord('z')) or
                (c >= ord('0') and c <= ord('9')) or
                c == ord('_')):
            off ^= 1

    return bool(off)


def dumpcode(prog):
    """Return human-readable disassembly of ByteProg bytecode.

    Port of re1_5_dumpcode from re1.5/dumpcode.c.
    Formats each opcode with PC offset, label, and arguments matching C output.

    Args:
        prog: ByteProg instance with insts and bytelen/len fields.

    Returns:
        String containing the disassembly text.
    """
    lines = []
    pc = 0
    code = prog.insts
    while pc < prog.bytelen:
        opcode = code[pc]
        pc += 1
        if opcode == Split:
            offset = struct.unpack_from('b', code, pc)[0]
            lines.append(f"{pc - 1:2d}: split {pc + offset + 1} ({offset})")
            pc += 1
        elif opcode == RSplit:
            offset = struct.unpack_from('b', code, pc)[0]
            lines.append(f"{pc - 1:2d}: rsplit {pc + offset + 1} ({offset})")
            pc += 1
        elif opcode == Jmp:
            offset = struct.unpack_from('b', code, pc)[0]
            lines.append(f"{pc - 1:2d}: jmp {pc + offset + 1} ({offset})")
            pc += 1
        elif opcode == Char:
            lines.append(f"{pc - 1:2d}: char {chr(code[pc])}")
            pc += 1
        elif opcode == Any:
            lines.append(f"{pc - 1:2d}: any")
        elif opcode in (Class, ClassNot):
            num = code[pc]
            label = "classnot" if opcode == ClassNot else "class"
            parts = [f"{pc - 1:2d}: {label} {num}"]
            pc += 1
            for _ in range(num):
                parts.append(f" 0x{code[pc]:02x}-0x{code[pc + 1]:02x}")
                pc += 2
            lines.append("".join(parts))
        elif opcode == NamedClass:
            lines.append(f"{pc - 1:2d}: namedclass {chr(code[pc])}")
            pc += 1
        elif opcode == Match:
            lines.append(f"{pc - 1:2d}: match")
        elif opcode == Save:
            lines.append(f"{pc - 1:2d}: save {code[pc]}")
            pc += 1
        elif opcode == Bol:
            lines.append(f"{pc - 1:2d}: assert bol")
        elif opcode == Eol:
            lines.append(f"{pc - 1:2d}: assert eol")
        else:
            raise ValueError(f"Unknown opcode 0x{opcode:02x} at pc={pc - 1}")
    lines.append(f"Bytes: {prog.bytelen}, insts: {prog.len}")
    return "\n".join(lines) + "\n"

def cleanmarks(prog):
    """Clear the high bit (0x80) mark from each opcode byte in prog.insts.

    Port of cleanmarks from re1.5/cleanmarks.c.
    The VM sets the high bit on opcodes during execution to detect infinite loops.
    This function resets them so the bytecode can be executed again.

    Args:
        prog: ByteProg instance whose insts will be modified in-place.
    """
    pc = 0
    insts = prog.insts
    end = prog.bytelen
    while pc < end:
        insts[pc] &= 0x7F
        opcode = insts[pc]
        if opcode in (Class, ClassNot):
            # Skip past count byte + range pairs, then the extra +1 below
            pc += insts[pc + 1] * 2
            pc += 1  # count byte (fall-through from C switch)
        elif opcode in (NamedClass, Jmp, Split, RSplit, Save, Char):
            pc += 1  # skip argument byte
        # Bol, Eol, Any, Match have no argument — just advance past opcode
        pc += 1

