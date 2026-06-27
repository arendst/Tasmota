"""
Feature: berry-python-port, Property 2: Enum value preservation

For any enum entry defined in the C source (token types from btokentype,
expression types from exptype_t, opcodes from bopcode, type constants from
be_object.h, GC marks), the corresponding Python integer constant should
have the identical name and integer value.

Validates: Requirements 2.5, 3.2, 4.2, 21.5
"""

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

import pytest

# ============================================================================
# Import all enum groups from the Python port
# ============================================================================

from berry_port.be_object import (
    # Basic type constants (be_object.h)
    BE_NONE, BE_NIL, BE_INT, BE_REAL, BE_BOOL, BE_COMPTR, BE_INDEX,
    BE_FUNCTION, BE_GCOBJECT, BE_GCOBJECT_MAX,
    BE_STRING, BE_CLASS, BE_INSTANCE, BE_PROTO, BE_LIST, BE_MAP,
    BE_MODULE, BE_COMOBJ,
    BE_NTVFUNC, BE_CLOSURE, BE_NTVCLOS, BE_CTYPE_FUNC, BE_STATIC,
    # Varg flags
    BE_VA_VARARG, BE_VA_METHOD, BE_VA_STATICMETHOD,
    BE_VA_SHARED_KTAB, BE_VA_NOCOMPACT,
    # GC marks (be_gc.h)
    GC_WHITE, GC_GRAY, GC_DARK, GC_FIXED, GC_CONST,
    # Error codes (berry.h)
    BE_OK, BE_EXIT, BE_MALLOC_FAIL, BE_EXCEPTION,
    BE_SYNTAX_ERROR, BE_EXEC_ERROR, BE_IO_ERROR,
    # Native module member type specifiers (berry.h)
    BE_CNIL, BE_CINT, BE_CREAL, BE_CBOOL, BE_CFUNCTION, BE_CSTRING,
    BE_CMODULE,
    # Call frame status flags (be_vm.h)
    NONE_FLAG, BASE_FRAME, PRIM_FUNC,
    # Compilation option masks (be_vm.h)
    COMP_NAMED_GBL, COMP_STRICT, COMP_GC_DEBUG,
    # Class/instance macros (be_class.h)
    MT_VARIABLE, MT_METHOD, MT_PRIMMETHOD,
    # String constants
    SHORT_STR_MAX_LEN,
    # Module name flag
    BE_MODULE_NAME,
)

from berry_port.be_decoder import (
    # Opcodes (be_opcodes.h)
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD,
    OP_LT, OP_LE, OP_EQ, OP_NE, OP_GT, OP_GE,
    OP_AND, OP_OR, OP_XOR, OP_SHL, OP_SHR,
    OP_CONNECT, OP_NEG, OP_FLIP,
    OP_LDNIL, OP_LDBOOL, OP_LDINT, OP_LDCONST, OP_MOVE,
    OP_GETGBL, OP_SETGBL, OP_GETUPV, OP_SETUPV,
    OP_JMP, OP_JMPT, OP_JMPF,
    OP_CALL, OP_RET, OP_CLOSURE,
    OP_GETMBR, OP_GETMET, OP_SETMBR,
    OP_GETIDX, OP_SETIDX,
    OP_SETSUPER, OP_CLOSE, OP_IMPORT,
    OP_EXBLK, OP_CATCH, OP_RAISE,
    OP_CLASS, OP_GETNGBL, OP_SETNGBL,
    OP_COUNT,
)

from berry_port.be_lexer import (
    # Token types (be_lexer.h btokentype)
    TokenNone, TokenEOS, TokenId, TokenInteger, TokenReal, TokenString,
    OptAssign, OptAddAssign, OptSubAssign, OptMulAssign, OptDivAssign,
    OptModAssign, OptAndAssign, OptOrAssign, OptXorAssign,
    OptLsfAssign, OptRsfAssign,
    OptAdd, OptSub, OptMul, OptDiv, OptMod,
    OptLT, OptLE, OptEQ, OptNE, OptGT, OptGE,
    OptBitAnd, OptBitOr, OptBitXor, OptShiftL, OptShiftR,
    OptConnect, OptAnd, OptOr,
    OptNot, OptFlip,
    OptSpaceLBK, OptCallLBK, OptRBK, OptLSB, OptRSB, OptLBR, OptRBR,
    OptDot, OptComma, OptSemic, OptColon, OptQuestion, OptArrow,
    OptWalrus,
    KeyIf, KeyElif, KeyElse, KeyWhile, KeyFor, KeyDef, KeyEnd,
    KeyClass, KeyBreak, KeyContinue, KeyReturn,
    KeyTrue, KeyFalse, KeyNil, KeyVar, KeyDo,
    KeyImport, KeyAs, KeyTry, KeyExcept, KeyRaise, KeyStatic,
)

from berry_port.be_parser import (
    # Expression types (be_parser.h exptype_t)
    ETVOID, ETNIL, ETBOOL, ETREAL, ETINT, ETSTRING, ETPROTO,
    ETCONST, ETLOCAL, ETGLOBAL, ETUPVAL, ETMEMBER, ETINDEX,
    ETREG, ETNGLOBAL,
    # Block types
    BLOCK_LOOP, BLOCK_EXCEPT,
)


# ============================================================================
# Ground truth: C enum values extracted from the C headers
# ============================================================================

# --- be_object.h: basic type constants ---
C_TYPE_CONSTANTS = {
    "BE_NONE":        -256,
    "BE_NIL":         0,
    "BE_INT":         1,
    "BE_REAL":        2,
    "BE_BOOL":        3,
    "BE_COMPTR":      4,
    "BE_INDEX":       5,
    "BE_FUNCTION":    6,
    "BE_GCOBJECT":    16,
    "BE_GCOBJECT_MAX": 3 << 5,  # 96
    "BE_STRING":      16,
    "BE_CLASS":       17,
    "BE_INSTANCE":    18,
    "BE_PROTO":       19,
    "BE_LIST":        20,
    "BE_MAP":         21,
    "BE_MODULE":      22,
    "BE_COMOBJ":      23,
    "BE_NTVFUNC":     (0 << 5) | 6,    # 6
    "BE_CLOSURE":     (1 << 5) | 6,    # 38
    "BE_NTVCLOS":     (2 << 5) | 6,    # 70
    "BE_CTYPE_FUNC":  (3 << 5) | 6,    # 102
    "BE_STATIC":      1 << 7,           # 128
}

# --- be_object.h: varg flags ---
C_VARG_FLAGS = {
    "BE_VA_VARARG":       1 << 0,
    "BE_VA_METHOD":       1 << 1,
    "BE_VA_STATICMETHOD": 1 << 2,
    "BE_VA_SHARED_KTAB":  1 << 3,
    "BE_VA_NOCOMPACT":    1 << 4,
}

# --- be_gc.h: GC mark constants ---
C_GC_MARKS = {
    "GC_WHITE": 0x00,
    "GC_GRAY":  0x01,
    "GC_DARK":  0x02,
    "GC_FIXED": 0x04,
    "GC_CONST": 0x08,
}

# --- berry.h: error codes ---
C_ERROR_CODES = {
    "BE_OK":           0,
    "BE_EXIT":         1,
    "BE_MALLOC_FAIL":  2,
    "BE_EXCEPTION":    3,
    "BE_SYNTAX_ERROR": 4,
    "BE_EXEC_ERROR":   5,
    "BE_IO_ERROR":     6,
}

# --- berry.h: native module member type specifiers ---
C_NATIVE_MODULE_TYPES = {
    "BE_CNIL":      0,
    "BE_CINT":      1,
    "BE_CREAL":     2,
    "BE_CBOOL":     3,
    "BE_CFUNCTION": 4,
    "BE_CSTRING":   5,
    "BE_CMODULE":   6,
}

# --- be_vm.h: call frame status flags ---
C_CALLFRAME_FLAGS = {
    "NONE_FLAG":  0,
    "BASE_FRAME": 1 << 0,
    "PRIM_FUNC":  1 << 1,
}

# --- be_vm.h: compilation option masks ---
C_COMP_OPTIONS = {
    "COMP_NAMED_GBL": 0x00,
    "COMP_STRICT":    0x01,
    "COMP_GC_DEBUG":  0x02,
}

# --- be_class.h: member type aliases ---
C_MEMBER_TYPES = {
    "MT_VARIABLE":   5,   # BE_INDEX
    "MT_METHOD":     38,  # BE_CLOSURE
    "MT_PRIMMETHOD": 6,   # BE_NTVFUNC
}

# --- be_string.h ---
C_STRING_CONSTANTS = {
    "SHORT_STR_MAX_LEN": 64,
}

# --- be_module.h ---
C_MODULE_CONSTANTS = {
    "BE_MODULE_NAME": 1,
}

# --- be_opcodes.h: opcodes (C enum, sequential from 0) ---
# The C header uses OPCODE(ADD), OPCODE(SUB), ... which expands to
# OP_ADD=0, OP_SUB=1, ... in sequential order.
C_OPCODES = {
    "OP_ADD":      0,
    "OP_SUB":      1,
    "OP_MUL":      2,
    "OP_DIV":      3,
    "OP_MOD":      4,
    "OP_LT":       5,
    "OP_LE":       6,
    "OP_EQ":       7,
    "OP_NE":       8,
    "OP_GT":       9,
    "OP_GE":       10,
    "OP_AND":      11,
    "OP_OR":       12,
    "OP_XOR":      13,
    "OP_SHL":      14,
    "OP_SHR":      15,
    "OP_CONNECT":  16,
    "OP_NEG":      17,
    "OP_FLIP":     18,
    "OP_LDNIL":    19,
    "OP_LDBOOL":   20,
    "OP_LDINT":    21,
    "OP_LDCONST":  22,
    "OP_MOVE":     23,
    "OP_GETGBL":   24,
    "OP_SETGBL":   25,
    "OP_GETUPV":   26,
    "OP_SETUPV":   27,
    "OP_JMP":      28,
    "OP_JMPT":     29,
    "OP_JMPF":     30,
    "OP_CALL":     31,
    "OP_RET":      32,
    "OP_CLOSURE":  33,
    "OP_GETMBR":   34,
    "OP_GETMET":   35,
    "OP_SETMBR":   36,
    "OP_GETIDX":   37,
    "OP_SETIDX":   38,
    "OP_SETSUPER": 39,
    "OP_CLOSE":    40,
    "OP_IMPORT":   41,
    "OP_EXBLK":    42,
    "OP_CATCH":    43,
    "OP_RAISE":    44,
    "OP_CLASS":    45,
    "OP_GETNGBL":  46,
    "OP_SETNGBL":  47,
}

# --- be_lexer.h: token types (C enum, sequential from 0) ---
C_TOKEN_TYPES = {
    "TokenNone":    0,
    "TokenEOS":     1,
    "TokenId":      2,
    "TokenInteger": 3,
    "TokenReal":    4,
    "TokenString":  5,
    "OptAssign":    6,
    "OptAddAssign": 7,
    "OptSubAssign": 8,
    "OptMulAssign": 9,
    "OptDivAssign": 10,
    "OptModAssign": 11,
    "OptAndAssign": 12,
    "OptOrAssign":  13,
    "OptXorAssign": 14,
    "OptLsfAssign": 15,
    "OptRsfAssign": 16,
    "OptAdd":       17,
    "OptSub":       18,
    "OptMul":       19,
    "OptDiv":       20,
    "OptMod":       21,
    "OptLT":        22,
    "OptLE":        23,
    "OptEQ":        24,
    "OptNE":        25,
    "OptGT":        26,
    "OptGE":        27,
    "OptBitAnd":    28,
    "OptBitOr":     29,
    "OptBitXor":    30,
    "OptShiftL":    31,
    "OptShiftR":    32,
    "OptConnect":   33,
    "OptAnd":       34,
    "OptOr":        35,
    "OptNot":       36,
    "OptFlip":      37,
    "OptSpaceLBK":  38,
    "OptCallLBK":   39,
    "OptRBK":       40,
    "OptLSB":       41,
    "OptRSB":       42,
    "OptLBR":       43,
    "OptRBR":       44,
    "OptDot":       45,
    "OptComma":     46,
    "OptSemic":     47,
    "OptColon":     48,
    "OptQuestion":  49,
    "OptArrow":     50,
    # NOTE: In the C header, OptWalrus follows OptArrow at position 51,
    # then keywords start at 52. The Python port currently has OptWalrus=73
    # (at the end after KeyStatic=72), which differs from C. The ground truth
    # here matches the C header. If the test fails, the Python port needs fixing.
    "OptWalrus":    51,
    "KeyIf":        52,
    "KeyElif":      53,
    "KeyElse":      54,
    "KeyWhile":     55,
    "KeyFor":       56,
    "KeyDef":       57,
    "KeyEnd":       58,
    "KeyClass":     59,
    "KeyBreak":     60,
    "KeyContinue":  61,
    "KeyReturn":    62,
    "KeyTrue":      63,
    "KeyFalse":     64,
    "KeyNil":       65,
    "KeyVar":       66,
    "KeyDo":        67,
    "KeyImport":    68,
    "KeyAs":        69,
    "KeyTry":       70,
    "KeyExcept":    71,
    "KeyRaise":     72,
    "KeyStatic":    73,
}

# --- be_parser.h: expression types (C enum, sequential from 0) ---
C_EXPRESSION_TYPES = {
    "ETVOID":    0,
    "ETNIL":     1,
    "ETBOOL":    2,
    "ETREAL":    3,
    "ETINT":     4,
    "ETSTRING":  5,
    "ETPROTO":   6,
    "ETCONST":   7,
    "ETLOCAL":   8,
    "ETGLOBAL":  9,
    "ETUPVAL":   10,
    "ETMEMBER":  11,
    "ETINDEX":   12,
    "ETREG":     13,
    "ETNGLOBAL": 14,
}

# --- be_parser.h: block types ---
C_BLOCK_TYPES = {
    "BLOCK_LOOP":   1,
    "BLOCK_EXCEPT": 2,
}


# ============================================================================
# Python-side value lookup (maps name -> actual Python constant value)
# ============================================================================

PYTHON_VALUES = {
    # Type constants
    "BE_NONE": BE_NONE, "BE_NIL": BE_NIL, "BE_INT": BE_INT,
    "BE_REAL": BE_REAL, "BE_BOOL": BE_BOOL, "BE_COMPTR": BE_COMPTR,
    "BE_INDEX": BE_INDEX, "BE_FUNCTION": BE_FUNCTION,
    "BE_GCOBJECT": BE_GCOBJECT, "BE_GCOBJECT_MAX": BE_GCOBJECT_MAX,
    "BE_STRING": BE_STRING, "BE_CLASS": BE_CLASS, "BE_INSTANCE": BE_INSTANCE,
    "BE_PROTO": BE_PROTO, "BE_LIST": BE_LIST, "BE_MAP": BE_MAP,
    "BE_MODULE": BE_MODULE, "BE_COMOBJ": BE_COMOBJ,
    "BE_NTVFUNC": BE_NTVFUNC, "BE_CLOSURE": BE_CLOSURE,
    "BE_NTVCLOS": BE_NTVCLOS, "BE_CTYPE_FUNC": BE_CTYPE_FUNC,
    "BE_STATIC": BE_STATIC,
    # Varg flags
    "BE_VA_VARARG": BE_VA_VARARG, "BE_VA_METHOD": BE_VA_METHOD,
    "BE_VA_STATICMETHOD": BE_VA_STATICMETHOD,
    "BE_VA_SHARED_KTAB": BE_VA_SHARED_KTAB,
    "BE_VA_NOCOMPACT": BE_VA_NOCOMPACT,
    # GC marks
    "GC_WHITE": GC_WHITE, "GC_GRAY": GC_GRAY, "GC_DARK": GC_DARK,
    "GC_FIXED": GC_FIXED, "GC_CONST": GC_CONST,
    # Error codes
    "BE_OK": BE_OK, "BE_EXIT": BE_EXIT, "BE_MALLOC_FAIL": BE_MALLOC_FAIL,
    "BE_EXCEPTION": BE_EXCEPTION, "BE_SYNTAX_ERROR": BE_SYNTAX_ERROR,
    "BE_EXEC_ERROR": BE_EXEC_ERROR, "BE_IO_ERROR": BE_IO_ERROR,
    # Native module member types
    "BE_CNIL": BE_CNIL, "BE_CINT": BE_CINT, "BE_CREAL": BE_CREAL,
    "BE_CBOOL": BE_CBOOL, "BE_CFUNCTION": BE_CFUNCTION,
    "BE_CSTRING": BE_CSTRING, "BE_CMODULE": BE_CMODULE,
    # Call frame flags
    "NONE_FLAG": NONE_FLAG, "BASE_FRAME": BASE_FRAME,
    "PRIM_FUNC": PRIM_FUNC,
    # Compilation options
    "COMP_NAMED_GBL": COMP_NAMED_GBL, "COMP_STRICT": COMP_STRICT,
    "COMP_GC_DEBUG": COMP_GC_DEBUG,
    # Member types
    "MT_VARIABLE": MT_VARIABLE, "MT_METHOD": MT_METHOD,
    "MT_PRIMMETHOD": MT_PRIMMETHOD,
    # String constants
    "SHORT_STR_MAX_LEN": SHORT_STR_MAX_LEN,
    # Module constants
    "BE_MODULE_NAME": BE_MODULE_NAME,
    # Opcodes
    "OP_ADD": OP_ADD, "OP_SUB": OP_SUB, "OP_MUL": OP_MUL,
    "OP_DIV": OP_DIV, "OP_MOD": OP_MOD,
    "OP_LT": OP_LT, "OP_LE": OP_LE, "OP_EQ": OP_EQ,
    "OP_NE": OP_NE, "OP_GT": OP_GT, "OP_GE": OP_GE,
    "OP_AND": OP_AND, "OP_OR": OP_OR, "OP_XOR": OP_XOR,
    "OP_SHL": OP_SHL, "OP_SHR": OP_SHR,
    "OP_CONNECT": OP_CONNECT, "OP_NEG": OP_NEG, "OP_FLIP": OP_FLIP,
    "OP_LDNIL": OP_LDNIL, "OP_LDBOOL": OP_LDBOOL,
    "OP_LDINT": OP_LDINT, "OP_LDCONST": OP_LDCONST, "OP_MOVE": OP_MOVE,
    "OP_GETGBL": OP_GETGBL, "OP_SETGBL": OP_SETGBL,
    "OP_GETUPV": OP_GETUPV, "OP_SETUPV": OP_SETUPV,
    "OP_JMP": OP_JMP, "OP_JMPT": OP_JMPT, "OP_JMPF": OP_JMPF,
    "OP_CALL": OP_CALL, "OP_RET": OP_RET, "OP_CLOSURE": OP_CLOSURE,
    "OP_GETMBR": OP_GETMBR, "OP_GETMET": OP_GETMET,
    "OP_SETMBR": OP_SETMBR,
    "OP_GETIDX": OP_GETIDX, "OP_SETIDX": OP_SETIDX,
    "OP_SETSUPER": OP_SETSUPER, "OP_CLOSE": OP_CLOSE,
    "OP_IMPORT": OP_IMPORT,
    "OP_EXBLK": OP_EXBLK, "OP_CATCH": OP_CATCH, "OP_RAISE": OP_RAISE,
    "OP_CLASS": OP_CLASS, "OP_GETNGBL": OP_GETNGBL,
    "OP_SETNGBL": OP_SETNGBL,
    # Token types
    "TokenNone": TokenNone, "TokenEOS": TokenEOS, "TokenId": TokenId,
    "TokenInteger": TokenInteger, "TokenReal": TokenReal,
    "TokenString": TokenString,
    "OptAssign": OptAssign, "OptAddAssign": OptAddAssign,
    "OptSubAssign": OptSubAssign, "OptMulAssign": OptMulAssign,
    "OptDivAssign": OptDivAssign, "OptModAssign": OptModAssign,
    "OptAndAssign": OptAndAssign, "OptOrAssign": OptOrAssign,
    "OptXorAssign": OptXorAssign, "OptLsfAssign": OptLsfAssign,
    "OptRsfAssign": OptRsfAssign,
    "OptAdd": OptAdd, "OptSub": OptSub, "OptMul": OptMul,
    "OptDiv": OptDiv, "OptMod": OptMod,
    "OptLT": OptLT, "OptLE": OptLE, "OptEQ": OptEQ,
    "OptNE": OptNE, "OptGT": OptGT, "OptGE": OptGE,
    "OptBitAnd": OptBitAnd, "OptBitOr": OptBitOr, "OptBitXor": OptBitXor,
    "OptShiftL": OptShiftL, "OptShiftR": OptShiftR,
    "OptConnect": OptConnect, "OptAnd": OptAnd, "OptOr": OptOr,
    "OptNot": OptNot, "OptFlip": OptFlip,
    "OptSpaceLBK": OptSpaceLBK, "OptCallLBK": OptCallLBK,
    "OptRBK": OptRBK, "OptLSB": OptLSB, "OptRSB": OptRSB,
    "OptLBR": OptLBR, "OptRBR": OptRBR, "OptDot": OptDot,
    "OptComma": OptComma, "OptSemic": OptSemic, "OptColon": OptColon,
    "OptQuestion": OptQuestion, "OptArrow": OptArrow,
    "OptWalrus": OptWalrus,
    "KeyIf": KeyIf, "KeyElif": KeyElif, "KeyElse": KeyElse,
    "KeyWhile": KeyWhile, "KeyFor": KeyFor, "KeyDef": KeyDef,
    "KeyEnd": KeyEnd, "KeyClass": KeyClass, "KeyBreak": KeyBreak,
    "KeyContinue": KeyContinue, "KeyReturn": KeyReturn,
    "KeyTrue": KeyTrue, "KeyFalse": KeyFalse, "KeyNil": KeyNil,
    "KeyVar": KeyVar, "KeyDo": KeyDo, "KeyImport": KeyImport,
    "KeyAs": KeyAs, "KeyTry": KeyTry, "KeyExcept": KeyExcept,
    "KeyRaise": KeyRaise, "KeyStatic": KeyStatic,
    # Expression types
    "ETVOID": ETVOID, "ETNIL": ETNIL, "ETBOOL": ETBOOL,
    "ETREAL": ETREAL, "ETINT": ETINT, "ETSTRING": ETSTRING,
    "ETPROTO": ETPROTO, "ETCONST": ETCONST, "ETLOCAL": ETLOCAL,
    "ETGLOBAL": ETGLOBAL, "ETUPVAL": ETUPVAL, "ETMEMBER": ETMEMBER,
    "ETINDEX": ETINDEX, "ETREG": ETREG, "ETNGLOBAL": ETNGLOBAL,
    # Block types
    "BLOCK_LOOP": BLOCK_LOOP, "BLOCK_EXCEPT": BLOCK_EXCEPT,
}


# ============================================================================
# Merge all C ground-truth dicts for parametrized testing
# ============================================================================

ALL_C_ENUMS = {}
ALL_C_ENUMS.update(C_TYPE_CONSTANTS)
ALL_C_ENUMS.update(C_VARG_FLAGS)
ALL_C_ENUMS.update(C_GC_MARKS)
ALL_C_ENUMS.update(C_ERROR_CODES)
ALL_C_ENUMS.update(C_NATIVE_MODULE_TYPES)
ALL_C_ENUMS.update(C_CALLFRAME_FLAGS)
ALL_C_ENUMS.update(C_COMP_OPTIONS)
ALL_C_ENUMS.update(C_MEMBER_TYPES)
ALL_C_ENUMS.update(C_STRING_CONSTANTS)
ALL_C_ENUMS.update(C_MODULE_CONSTANTS)
ALL_C_ENUMS.update(C_OPCODES)
ALL_C_ENUMS.update(C_TOKEN_TYPES)
ALL_C_ENUMS.update(C_EXPRESSION_TYPES)
ALL_C_ENUMS.update(C_BLOCK_TYPES)


# ============================================================================
# Tests
# ============================================================================

@pytest.mark.parametrize("name,expected", list(ALL_C_ENUMS.items()))
def test_enum_value_matches_c(name, expected):
    """Each Python constant must have the same integer value as the C enum."""
    actual = PYTHON_VALUES[name]
    assert actual == expected, (
        f"{name}: Python={actual}, C={expected}"
    )


# ---------------------------------------------------------------------------
# Group-level completeness checks
# ---------------------------------------------------------------------------

def test_opcode_count():
    """OP_COUNT must equal 48 (the total number of Berry opcodes)."""
    assert OP_COUNT == 48


def test_opcodes_are_sequential():
    """Opcodes must be sequential integers 0..47."""
    for i, name in enumerate(sorted(C_OPCODES, key=C_OPCODES.get)):
        assert C_OPCODES[name] == i, f"{name} should be {i}, got {C_OPCODES[name]}"


def test_token_types_are_sequential():
    """Token types must be sequential integers 0..73 (KeyStatic is last at 73)."""
    for name in sorted(C_TOKEN_TYPES, key=C_TOKEN_TYPES.get):
        val = C_TOKEN_TYPES[name]
        py_val = PYTHON_VALUES[name]
        assert py_val == val, f"{name}: Python={py_val}, C={val}"
    # Verify the range covers 0 through KeyStatic (73 in C header)
    assert max(C_TOKEN_TYPES.values()) == 73


def test_expression_types_are_sequential():
    """Expression types must be sequential integers 0..14."""
    for name in sorted(C_EXPRESSION_TYPES, key=C_EXPRESSION_TYPES.get):
        val = C_EXPRESSION_TYPES[name]
        py_val = PYTHON_VALUES[name]
        assert py_val == val, f"{name}: Python={py_val}, C={val}"
    assert max(C_EXPRESSION_TYPES.values()) == 14


def test_gc_marks_are_powers_of_two():
    """GC_GRAY, GC_DARK, GC_FIXED, GC_CONST must be distinct bit flags."""
    flags = [GC_GRAY, GC_DARK, GC_FIXED, GC_CONST]
    for f in flags:
        assert f != 0 and (f & (f - 1)) == 0, f"GC flag {f} is not a power of 2"
    # All flags must be distinct
    assert len(set(flags)) == len(flags)


def test_function_type_encoding():
    """Function subtypes must be encoded as (subtype_index << 5) | BE_FUNCTION."""
    assert BE_NTVFUNC == (0 << 5) | BE_FUNCTION
    assert BE_CLOSURE == (1 << 5) | BE_FUNCTION
    assert BE_NTVCLOS == (2 << 5) | BE_FUNCTION
    assert BE_CTYPE_FUNC == (3 << 5) | BE_FUNCTION


def test_varg_flags_are_distinct_bits():
    """Varg flags must be distinct single-bit flags."""
    flags = [BE_VA_VARARG, BE_VA_METHOD, BE_VA_STATICMETHOD,
             BE_VA_SHARED_KTAB, BE_VA_NOCOMPACT]
    for f in flags:
        assert f != 0 and (f & (f - 1)) == 0, f"Varg flag {f} is not a power of 2"
    assert len(set(flags)) == len(flags)


def test_error_codes_are_sequential():
    """Error codes must be sequential integers 0..6."""
    codes = [BE_OK, BE_EXIT, BE_MALLOC_FAIL, BE_EXCEPTION,
             BE_SYNTAX_ERROR, BE_EXEC_ERROR, BE_IO_ERROR]
    for i, code in enumerate(codes):
        assert code == i, f"Error code at position {i} has value {code}"


def test_member_type_aliases():
    """MT_VARIABLE, MT_METHOD, MT_PRIMMETHOD must alias the correct type constants."""
    assert MT_VARIABLE == BE_INDEX
    assert MT_METHOD == BE_CLOSURE
    assert MT_PRIMMETHOD == BE_NTVFUNC


def test_all_c_enums_have_python_counterpart():
    """Every C enum name in the ground truth must exist in PYTHON_VALUES."""
    missing = set(ALL_C_ENUMS.keys()) - set(PYTHON_VALUES.keys())
    assert not missing, f"Missing Python constants: {missing}"
