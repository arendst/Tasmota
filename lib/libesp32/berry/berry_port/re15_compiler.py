"""re1.5 bytecode compiler — Python port of compilecode.c.

Translates regex pattern strings into ByteProg bytecode, producing
byte-for-byte identical output to the C re1_5_compilecode function.
"""

from berry_port.re15_opcodes import (
    ByteProg,
    Char, Any, Class, ClassNot, NamedClass,
    Bol, Eol,
    Jmp, Split, RSplit,
    Save, Match,
    NON_ANCHORED_PREFIX,
    RE1_5_SUCCESS, RE1_5_SYNTAX_ERROR,
    RE1_5_UNSUPPORTED_ESCAPE, RE1_5_UNSUPPORTED_SYNTAX,
)


def _rel(at, to):
    """Compute signed byte offset: to - at - 2, wrapped to signed byte."""
    v = to - at - 2
    # Convert to signed byte (-128..127) stored as unsigned (0..255)
    return v & 0xFF


def _compilecode(re_str, pos, prog, sizecode_mode):
    """Core recursive-descent compiler.

    Ports the C ``_compilecode`` function.  *re_str* is the full pattern
    string, *pos* is the current index into it.  Returns ``(new_pos,
    error_code)`` where *error_code* is 0 on success or a negative value.
    """
    code = None if sizecode_mode else prog.insts
    start = prog.bytelen
    term = prog.bytelen
    alt_label = 0

    while pos < len(re_str) and re_str[pos] != ')':
        ch = re_str[pos]

        if ch == '\\':
            pos += 1
            if pos >= len(re_str):
                return pos, RE1_5_SYNTAX_ERROR  # trailing backslash
            esc = re_str[pos]
            c_lower = esc.lower()
            if c_lower in ('d', 's', 'w'):
                term = prog.bytelen
                if code is not None:
                    code[prog.bytelen] = NamedClass
                prog.bytelen += 1
                if code is not None:
                    code[prog.bytelen] = ord(esc)
                prog.bytelen += 1
                prog.len += 1
                pos += 1
                continue
            # Check for unsupported escapes: digits or lowercase letters
            # that aren't handled above
            if ('0' <= c_lower <= '9') or ('a' <= c_lower <= 'z'):
                return pos, RE1_5_UNSUPPORTED_ESCAPE
            # Fall through to default (literal escape) — emit Char
            term = prog.bytelen
            if code is not None:
                code[prog.bytelen] = Char
            prog.bytelen += 1
            if code is not None:
                code[prog.bytelen] = ord(esc)
            prog.bytelen += 1
            prog.len += 1
            pos += 1
            continue

        if ch == '.':
            term = prog.bytelen
            if code is not None:
                code[prog.bytelen] = Any
            prog.bytelen += 1
            prog.len += 1
            pos += 1
            continue

        if ch == '[':
            term = prog.bytelen
            pos += 1
            if pos < len(re_str) and re_str[pos] == '^':
                if code is not None:
                    code[prog.bytelen] = ClassNot
                prog.bytelen += 1
                pos += 1
            else:
                if code is not None:
                    code[prog.bytelen] = Class
                prog.bytelen += 1
            count_pos = prog.bytelen
            prog.bytelen += 1  # skip "# of pairs" byte
            prog.len += 1
            cnt = 0
            while pos < len(re_str) and re_str[pos] != ']':
                c = re_str[pos]
                if c == '\\':
                    pos += 1
                    if pos >= len(re_str):
                        return pos, RE1_5_SYNTAX_ERROR
                    c = re_str[pos]
                    if c != '\\' and c != ']':
                        return pos, RE1_5_UNSUPPORTED_ESCAPE
                if code is not None:
                    code[prog.bytelen] = ord(c)
                prog.bytelen += 1
                # Check for range: c-x where x != ']'
                if (pos + 1 < len(re_str) and re_str[pos + 1] == '-'
                        and pos + 2 < len(re_str) and re_str[pos + 2] != ']'):
                    pos += 2
                    c = re_str[pos]
                if code is not None:
                    code[prog.bytelen] = ord(c)
                prog.bytelen += 1
                cnt += 1
                pos += 1
            if pos >= len(re_str):
                return pos, RE1_5_SYNTAX_ERROR  # unterminated class
            # Write count byte
            if code is not None:
                code[count_pos] = cnt
            pos += 1  # skip ']'
            continue

        if ch == '(':
            term = prog.bytelen
            pos += 1
            capture = True
            if pos < len(re_str) and re_str[pos] == '?':
                pos += 1
                if pos < len(re_str) and re_str[pos] == ':':
                    capture = False
                    pos += 1
                else:
                    return pos, RE1_5_UNSUPPORTED_SYNTAX

            sub = 0
            if capture:
                prog.sub += 1
                sub = prog.sub
                if code is not None:
                    code[prog.bytelen] = Save
                prog.bytelen += 1
                if code is not None:
                    code[prog.bytelen] = 2 * sub
                prog.bytelen += 1
                prog.len += 1

            pos, res = _compilecode(re_str, pos, prog, sizecode_mode)
            if res < 0:
                return pos, res
            if pos >= len(re_str) or re_str[pos] != ')':
                return pos, RE1_5_SYNTAX_ERROR

            if capture:
                if code is not None:
                    code[prog.bytelen] = Save
                prog.bytelen += 1
                if code is not None:
                    code[prog.bytelen] = 2 * sub + 1
                prog.bytelen += 1
                prog.len += 1

            pos += 1  # skip ')'
            continue

        if ch == '{':
            return pos, RE1_5_UNSUPPORTED_SYNTAX

        if ch == '?':
            if prog.bytelen == term:
                return pos, RE1_5_SYNTAX_ERROR  # nothing to repeat
            pc = prog.bytelen
            # INSERT_CODE(term, 2, PC)
            if code is not None:
                code[term + 2:pc + 2] = code[term:pc]
            prog.bytelen = pc + 2
            if pos + 1 < len(re_str) and re_str[pos + 1] == '?':
                if code is not None:
                    code[term] = RSplit
                pos += 1
            else:
                if code is not None:
                    code[term] = Split
            if code is not None:
                code[term + 1] = _rel(term, prog.bytelen)
            prog.len += 1
            term = prog.bytelen
            pos += 1
            continue

        if ch == '*':
            if prog.bytelen == term:
                return pos, RE1_5_SYNTAX_ERROR  # nothing to repeat
            pc = prog.bytelen
            # INSERT_CODE(term, 2, PC)
            if code is not None:
                code[term + 2:pc + 2] = code[term:pc]
            prog.bytelen = pc + 2
            # EMIT Jmp at end
            if code is not None:
                code[prog.bytelen] = Jmp
                code[prog.bytelen + 1] = _rel(prog.bytelen, term)
            prog.bytelen += 2
            if pos + 1 < len(re_str) and re_str[pos + 1] == '?':
                if code is not None:
                    code[term] = RSplit
                pos += 1
            else:
                if code is not None:
                    code[term] = Split
            if code is not None:
                code[term + 1] = _rel(term, prog.bytelen)
            prog.len += 2
            term = prog.bytelen
            pos += 1
            continue

        if ch == '+':
            if prog.bytelen == term:
                return pos, RE1_5_SYNTAX_ERROR  # nothing to repeat
            if pos + 1 < len(re_str) and re_str[pos + 1] == '?':
                if code is not None:
                    code[prog.bytelen] = Split
                pos += 1
            else:
                if code is not None:
                    code[prog.bytelen] = RSplit
            if code is not None:
                code[prog.bytelen + 1] = _rel(prog.bytelen, term)
            prog.bytelen += 2
            prog.len += 1
            term = prog.bytelen
            pos += 1
            continue

        if ch == '|':
            if alt_label:
                if code is not None:
                    code[alt_label] = (_rel(alt_label, prog.bytelen) + 1) & 0xFF
            pc = prog.bytelen
            # INSERT_CODE(start, 2, PC)
            if code is not None:
                code[start + 2:pc + 2] = code[start:pc]
            prog.bytelen = pc + 2
            if code is not None:
                code[prog.bytelen] = Jmp
            prog.bytelen += 1
            alt_label = prog.bytelen
            prog.bytelen += 1
            if code is not None:
                code[start] = Split
                code[start + 1] = _rel(start, prog.bytelen)
            prog.len += 2
            term = prog.bytelen
            pos += 1
            continue

        if ch == '^':
            if code is not None:
                code[prog.bytelen] = Bol
            prog.bytelen += 1
            prog.len += 1
            term = prog.bytelen
            pos += 1
            continue

        if ch == '$':
            if code is not None:
                code[prog.bytelen] = Eol
            prog.bytelen += 1
            prog.len += 1
            term = prog.bytelen
            pos += 1
            continue

        # Default: literal character
        term = prog.bytelen
        if code is not None:
            code[prog.bytelen] = Char
        prog.bytelen += 1
        if code is not None:
            code[prog.bytelen] = ord(ch)
        prog.bytelen += 1
        prog.len += 1
        pos += 1

    # Patch final alt_label
    if alt_label:
        if code is not None:
            code[alt_label] = (_rel(alt_label, prog.bytelen) + 1) & 0xFF

    return pos, RE1_5_SUCCESS


def sizecode(re_str):
    """Calculate bytecode size needed for *re_str*.

    Returns the total byte length on success, or a negative error code.
    Mirrors C ``re1_5_sizecode``.
    """
    prog = ByteProg(bytelen=5 + NON_ANCHORED_PREFIX)
    pos, res = _compilecode(re_str, 0, prog, sizecode_mode=True)
    if res < 0:
        return res
    # If unparsed chars remain it's a syntax error (e.g. unmatched ')')
    if pos < len(re_str):
        return RE1_5_SYNTAX_ERROR
    return prog.bytelen


def compilecode(re_str):
    """Compile *re_str* to a :class:`ByteProg`.

    Returns ``(prog, error_code)`` where *error_code* is
    :data:`RE1_5_SUCCESS` (0) on success.  Mirrors C
    ``re1_5_compilecode``.
    """
    size = sizecode(re_str)
    if size < 0:
        return ByteProg(), size

    prog = ByteProg(insts=bytearray(size))
    prog.bytelen = 0
    prog.len = 0
    prog.sub = 0

    # Non-anchored search prefix: RSplit 3, Any, Jmp -5
    prog.insts[prog.bytelen] = RSplit
    prog.bytelen += 1
    prog.insts[prog.bytelen] = 3
    prog.bytelen += 1
    prog.insts[prog.bytelen] = Any
    prog.bytelen += 1
    prog.insts[prog.bytelen] = Jmp
    prog.bytelen += 1
    prog.insts[prog.bytelen] = (-5) & 0xFF  # signed -5 as unsigned byte
    prog.bytelen += 1
    prog.len += 3

    # Save 0
    prog.insts[prog.bytelen] = Save
    prog.bytelen += 1
    prog.insts[prog.bytelen] = 0
    prog.bytelen += 1
    prog.len += 1

    pos, res = _compilecode(re_str, 0, prog, sizecode_mode=False)
    if res < 0:
        return prog, res
    if pos < len(re_str):
        return prog, RE1_5_SYNTAX_ERROR

    # Save 1
    prog.insts[prog.bytelen] = Save
    prog.bytelen += 1
    prog.insts[prog.bytelen] = 1
    prog.bytelen += 1
    prog.len += 1

    # Match
    prog.insts[prog.bytelen] = Match
    prog.bytelen += 1
    prog.len += 1

    return prog, RE1_5_SUCCESS
