"""
Unit tests for berry_port.re15_classes — classmatch and namedclassmatch.

Tests cover:
- classmatch with single-range and multi-range character classes
- ClassNot negation logic
- namedclassmatch for \\d, \\D, \\s, \\S, \\w, \\W with boundary characters
"""

from berry_port.re15_opcodes import Class, ClassNot, NamedClass


from berry_port.re15_classes import classmatch, namedclassmatch


# ---------------------------------------------------------------------------
# Helper: build a minimal insts bytearray for Class/ClassNot
# ---------------------------------------------------------------------------

def _make_class_insts(opcode, ranges):
    """Build bytearray: [opcode, count, lo1, hi1, lo2, hi2, ...]"""
    insts = bytearray([opcode, len(ranges)])
    for lo, hi in ranges:
        insts.append(lo)
        insts.append(hi)
    return insts


# ---------------------------------------------------------------------------
# classmatch — Class (positive)
# ---------------------------------------------------------------------------

def test_classmatch_single_range_match():
    # [a-z]
    insts = _make_class_insts(Class, [(ord('a'), ord('z'))])
    assert classmatch(insts, 1, 'm') is True


def test_classmatch_single_range_no_match():
    # [a-z]
    insts = _make_class_insts(Class, [(ord('a'), ord('z'))])
    assert classmatch(insts, 1, 'A') is False


def test_classmatch_single_range_boundary_low():
    insts = _make_class_insts(Class, [(ord('a'), ord('z'))])
    assert classmatch(insts, 1, 'a') is True


def test_classmatch_single_range_boundary_high():
    insts = _make_class_insts(Class, [(ord('a'), ord('z'))])
    assert classmatch(insts, 1, 'z') is True


def test_classmatch_single_range_just_below():
    insts = _make_class_insts(Class, [(ord('a'), ord('z'))])
    assert classmatch(insts, 1, '`') is False  # ord('`') == ord('a') - 1


def test_classmatch_single_range_just_above():
    insts = _make_class_insts(Class, [(ord('a'), ord('z'))])
    assert classmatch(insts, 1, '{') is False  # ord('{') == ord('z') + 1


def test_classmatch_multi_range_first():
    # [a-zA-Z]
    insts = _make_class_insts(Class, [(ord('a'), ord('z')), (ord('A'), ord('Z'))])
    assert classmatch(insts, 1, 'g') is True


def test_classmatch_multi_range_second():
    insts = _make_class_insts(Class, [(ord('a'), ord('z')), (ord('A'), ord('Z'))])
    assert classmatch(insts, 1, 'G') is True


def test_classmatch_multi_range_no_match():
    insts = _make_class_insts(Class, [(ord('a'), ord('z')), (ord('A'), ord('Z'))])
    assert classmatch(insts, 1, '5') is False


def test_classmatch_single_char_range():
    # [x] — range where lo == hi
    insts = _make_class_insts(Class, [(ord('x'), ord('x'))])
    assert classmatch(insts, 1, 'x') is True
    assert classmatch(insts, 1, 'y') is False


# ---------------------------------------------------------------------------
# classmatch — ClassNot (negated)
# ---------------------------------------------------------------------------

def test_classnot_no_match_in_range():
    # [^0-9] — character NOT in range should match
    insts = _make_class_insts(ClassNot, [(ord('0'), ord('9'))])
    assert classmatch(insts, 1, 'a') is True


def test_classnot_match_in_range():
    # [^0-9] — character IN range should NOT match
    insts = _make_class_insts(ClassNot, [(ord('0'), ord('9'))])
    assert classmatch(insts, 1, '5') is False


def test_classnot_boundary():
    insts = _make_class_insts(ClassNot, [(ord('0'), ord('9'))])
    assert classmatch(insts, 1, '0') is False
    assert classmatch(insts, 1, '9') is False
    assert classmatch(insts, 1, '/') is True   # just below '0'
    assert classmatch(insts, 1, ':') is True   # just above '9'


# ---------------------------------------------------------------------------
# namedclassmatch — \d / \D
# ---------------------------------------------------------------------------

def _make_named_insts(class_char):
    """Build bytearray: [NamedClass, class_byte]"""
    return bytearray([NamedClass, ord(class_char)])


def test_named_d_digits():
    insts = _make_named_insts('d')
    for ch in '0123456789':
        assert namedclassmatch(insts, 1, ch) is True


def test_named_d_non_digits():
    insts = _make_named_insts('d')
    for ch in 'azAZ!@ _':
        assert namedclassmatch(insts, 1, ch) is False


def test_named_d_boundary():
    insts = _make_named_insts('d')
    assert namedclassmatch(insts, 1, '/') is False   # ord('0') - 1
    assert namedclassmatch(insts, 1, '0') is True
    assert namedclassmatch(insts, 1, '9') is True
    assert namedclassmatch(insts, 1, ':') is False   # ord('9') + 1


def test_named_D_negation():
    insts = _make_named_insts('D')
    assert namedclassmatch(insts, 1, '5') is False
    assert namedclassmatch(insts, 1, 'a') is True
    assert namedclassmatch(insts, 1, ' ') is True


# ---------------------------------------------------------------------------
# namedclassmatch — \s / \S
# ---------------------------------------------------------------------------

def test_named_s_whitespace():
    insts = _make_named_insts('s')
    for ch in ' \t\n\r\x0b\x0c':  # space, tab, newline, CR, VT, FF
        assert namedclassmatch(insts, 1, ch) is True, f"failed for {repr(ch)}"


def test_named_s_non_whitespace():
    insts = _make_named_insts('s')
    for ch in 'aZ0_!':
        assert namedclassmatch(insts, 1, ch) is False


def test_named_S_negation():
    insts = _make_named_insts('S')
    assert namedclassmatch(insts, 1, ' ') is False
    assert namedclassmatch(insts, 1, '\t') is False
    assert namedclassmatch(insts, 1, 'a') is True
    assert namedclassmatch(insts, 1, '5') is True


# ---------------------------------------------------------------------------
# namedclassmatch — \w / \W
# ---------------------------------------------------------------------------

def test_named_w_word_chars():
    insts = _make_named_insts('w')
    for ch in 'azAZ09_':
        assert namedclassmatch(insts, 1, ch) is True, f"failed for {repr(ch)}"


def test_named_w_non_word_chars():
    insts = _make_named_insts('w')
    for ch in ' !@#-+.':
        assert namedclassmatch(insts, 1, ch) is False, f"failed for {repr(ch)}"


def test_named_w_boundary():
    insts = _make_named_insts('w')
    # Just outside ranges
    assert namedclassmatch(insts, 1, '@') is False   # ord('A') - 1
    assert namedclassmatch(insts, 1, 'A') is True
    assert namedclassmatch(insts, 1, 'Z') is True
    assert namedclassmatch(insts, 1, '[') is False   # ord('Z') + 1
    assert namedclassmatch(insts, 1, '`') is False   # ord('a') - 1
    assert namedclassmatch(insts, 1, 'a') is True
    assert namedclassmatch(insts, 1, 'z') is True
    assert namedclassmatch(insts, 1, '{') is False   # ord('z') + 1
    assert namedclassmatch(insts, 1, '/') is False   # ord('0') - 1
    assert namedclassmatch(insts, 1, '0') is True
    assert namedclassmatch(insts, 1, '9') is True
    assert namedclassmatch(insts, 1, ':') is False   # ord('9') + 1
    assert namedclassmatch(insts, 1, '_') is True


def test_named_W_negation():
    insts = _make_named_insts('W')
    assert namedclassmatch(insts, 1, 'a') is False
    assert namedclassmatch(insts, 1, '0') is False
    assert namedclassmatch(insts, 1, '_') is False
    assert namedclassmatch(insts, 1, ' ') is True
    assert namedclassmatch(insts, 1, '!') is True


# ---------------------------------------------------------------------------
# cleanmarks
# ---------------------------------------------------------------------------

from berry_port.re15_classes import cleanmarks
from berry_port.re15_compiler import compilecode


def _mark_all_opcodes(prog):
    """Set the 0x80 high bit on every opcode byte in prog, simulating VM marks."""
    from berry_port.re15_opcodes import (
        Class as _Class, ClassNot as _ClassNot, NamedClass as _NC,
        Jmp as _Jmp, Split as _Split, RSplit as _RSplit,
        Save as _Save, Char as _Char,
    )
    pc = 0
    while pc < prog.bytelen:
        prog.insts[pc] |= 0x80
        op = prog.insts[pc] & 0x7F
        if op in (_Class, _ClassNot):
            pc += prog.insts[pc + 1] * 2 + 1
        elif op in (_NC, _Jmp, _Split, _RSplit, _Save, _Char):
            pc += 1
        pc += 1


def test_cleanmarks_literal_pattern():
    """cleanmarks restores bytecode after marking a simple literal pattern."""
    prog, err = compilecode("abc")
    assert err == 0
    original = bytearray(prog.insts[:prog.bytelen])
    _mark_all_opcodes(prog)
    assert prog.insts[:prog.bytelen] != original
    cleanmarks(prog)
    assert prog.insts[:prog.bytelen] == original


def test_cleanmarks_character_class():
    """cleanmarks correctly skips Class range pairs."""
    prog, err = compilecode("[a-zA-Z0-9]")
    assert err == 0
    original = bytearray(prog.insts[:prog.bytelen])
    _mark_all_opcodes(prog)
    cleanmarks(prog)
    assert prog.insts[:prog.bytelen] == original


def test_cleanmarks_named_class():
    """cleanmarks handles NamedClass opcodes (\\d, \\w, etc.)."""
    prog, err = compilecode(r"\d+\w*")
    assert err == 0
    original = bytearray(prog.insts[:prog.bytelen])
    _mark_all_opcodes(prog)
    cleanmarks(prog)
    assert prog.insts[:prog.bytelen] == original


def test_cleanmarks_anchors():
    """cleanmarks handles Bol/Eol which have no argument bytes."""
    prog, err = compilecode("^a$")
    assert err == 0
    original = bytearray(prog.insts[:prog.bytelen])
    _mark_all_opcodes(prog)
    cleanmarks(prog)
    assert prog.insts[:prog.bytelen] == original


def test_cleanmarks_alternation_and_groups():
    """cleanmarks handles Split/RSplit/Save from groups and alternation."""
    prog, err = compilecode("(a|b)+c")
    assert err == 0
    original = bytearray(prog.insts[:prog.bytelen])
    _mark_all_opcodes(prog)
    cleanmarks(prog)
    assert prog.insts[:prog.bytelen] == original


def test_cleanmarks_idempotent():
    """Calling cleanmarks on already-clean bytecode is a no-op."""
    prog, err = compilecode("foo")
    assert err == 0
    original = bytearray(prog.insts[:prog.bytelen])
    cleanmarks(prog)
    assert prog.insts[:prog.bytelen] == original
