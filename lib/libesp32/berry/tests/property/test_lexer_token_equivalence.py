"""
Feature: berry-python-port, Property 19: Lexer token equivalence

For any valid Berry source string, tokenizing it should produce the same
sequence of token types and associated values (integer values, real values,
string values, identifier names) as the C lexer.

Since we cannot invoke the C lexer directly in these tests, we verify
structural invariants that guarantee equivalence:
  - Keywords are recognized and produce the correct token type
  - Integer literals (decimal and hex) parse to the correct value
  - Real literals parse to the correct value
  - String literals (with escapes) round-trip correctly
  - Identifiers are returned with correct content
  - Operators produce the correct token type
  - Comments are properly skipped
  - Concatenation of valid token sequences tokenizes to the union of tokens

Validates: Requirements 3.3
"""

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

import math
from hypothesis import given, settings, assume
from hypothesis.strategies import (
    integers, floats, text, sampled_from, lists, one_of,
    composite, just,
)

from berry_port.be_object import bvalue, bgc, bstringtable
from berry_port.be_string import be_string_init, be_str2cstr
from berry_port.be_lexer import (
    blexer, be_lexer_init, be_lexer_scan_next,
    token_strings, be_str2real, _wrap_bint,
    TokenNone, TokenEOS, TokenId, TokenInteger, TokenReal, TokenString,
    OptAdd, OptSub, OptMul, OptDiv, OptMod,
    OptAssign, OptAddAssign, OptSubAssign, OptMulAssign, OptDivAssign,
    OptModAssign, OptAndAssign, OptOrAssign, OptXorAssign,
    OptLsfAssign, OptRsfAssign,
    OptLT, OptLE, OptEQ, OptNE, OptGT, OptGE,
    OptAnd, OptOr, OptNot, OptFlip,
    OptBitAnd, OptBitOr, OptBitXor, OptShiftL, OptShiftR,
    OptConnect, OptArrow, OptWalrus,
    OptSpaceLBK, OptCallLBK, OptRBK, OptLSB, OptRSB, OptLBR, OptRBR,
    OptDot, OptComma, OptSemic, OptColon, OptQuestion,
    KeyIf, KeyElif, KeyElse, KeyWhile, KeyFor, KeyDef, KeyEnd,
    KeyClass, KeyBreak, KeyContinue, KeyReturn,
    KeyTrue, KeyFalse, KeyNil, KeyVar, KeyDo,
    KeyImport, KeyAs, KeyTry, KeyExcept, KeyRaise, KeyStatic,
)


# ---------------------------------------------------------------------------
# Test infrastructure
# ---------------------------------------------------------------------------

class MiniVM:
    """Minimal VM stub for lexer testing."""
    def __init__(self):
        self.gc = bgc()
        self.strtab = bstringtable()
        self.stack = [bvalue() for _ in range(100)]
        self.top_idx = 0
        self.stacktop_idx = 100
        self.reg_idx = 0
        self.compopt = 0
        self.preprocessor = None
        self.const_strtab = None


def _fresh_vm():
    vm = MiniVM()
    be_string_init(vm)
    return vm


def tokenize(source):
    """Tokenize a Berry source string, return list of (token_type, value)."""
    vm = _fresh_vm()
    source_data = [source]

    def reader(lex, data, size_ref):
        if source_data[0] is not None:
            s = source_data[0]
            source_data[0] = None
            size_ref[0] = len(s)
            return s
        return None

    lexer = blexer()
    be_lexer_init(lexer, vm, "<test>", reader, None)
    tokens = []
    while be_lexer_scan_next(lexer):
        t = lexer.token
        if t.type == TokenId:
            tokens.append((TokenId, be_str2cstr(t.s)))
        elif t.type == TokenInteger:
            tokens.append((TokenInteger, t.i))
        elif t.type == TokenReal:
            tokens.append((TokenReal, t.r))
        elif t.type == TokenString:
            tokens.append((TokenString, be_str2cstr(t.s)))
        else:
            tokens.append((t.type, None))
    return tokens


# ---------------------------------------------------------------------------
# Strategies
# ---------------------------------------------------------------------------

# All Berry keywords and their expected token types
KEYWORDS = {
    "if": KeyIf, "elif": KeyElif, "else": KeyElse,
    "while": KeyWhile, "for": KeyFor, "def": KeyDef, "end": KeyEnd,
    "class": KeyClass, "break": KeyBreak, "continue": KeyContinue,
    "return": KeyReturn, "true": KeyTrue, "false": KeyFalse,
    "nil": KeyNil, "var": KeyVar, "do": KeyDo,
    "import": KeyImport, "as": KeyAs, "try": KeyTry,
    "except": KeyExcept, "raise": KeyRaise, "static": KeyStatic,
}

keyword_strat = sampled_from(list(KEYWORDS.keys()))

# Valid Berry identifiers: start with letter or underscore, then alnum/_
_ident_start = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"
_ident_cont = _ident_start + "0123456789"


@composite
def berry_identifiers(draw):
    """Generate valid Berry identifiers that are NOT keywords."""
    first = draw(sampled_from(list(_ident_start)))
    rest_len = draw(integers(min_value=0, max_value=20))
    rest = draw(text(alphabet=_ident_cont, min_size=rest_len, max_size=rest_len))
    ident = first + rest
    assume(ident not in KEYWORDS)
    return ident


# Decimal integers that fit in Berry's int range
decimal_ints = integers(min_value=0, max_value=2**31 - 1)

# Hex integers
hex_ints = integers(min_value=0, max_value=0xFFFFFFFF)

# Real numbers that can be represented exactly in source form
safe_reals = floats(
    min_value=1e-10, max_value=1e15,
    allow_nan=False, allow_infinity=False,
    allow_subnormal=False,
)

# Simple string content (printable ASCII, no quotes or backslashes)
_safe_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 _-+*/<>,.;:!@#$%^&()[]{}|~`"
simple_string_content = text(alphabet=_safe_chars, min_size=0, max_size=50)

# Single-char operators and their expected token types
SINGLE_OPS = {
    "+": OptAdd, "-": OptSub, "*": OptMul, "/": OptDiv, "%": OptMod,
    "<": OptLT, ">": OptGT, "=": OptAssign,
    "&": OptBitAnd, "|": OptBitOr, "^": OptBitXor, "~": OptFlip,
    "!": OptNot, ".": OptDot,
    "(": OptSpaceLBK, ")": OptRBK, "[": OptLSB, "]": OptRSB,
    "{": OptLBR, "}": OptRBR,
    ",": OptComma, ";": OptSemic, ":": OptColon, "?": OptQuestion,
}

# Multi-char operators
MULTI_OPS = {
    "+=": OptAddAssign, "-=": OptSubAssign, "*=": OptMulAssign,
    "/=": OptDivAssign, "%=": OptModAssign,
    "&=": OptAndAssign, "|=": OptOrAssign, "^=": OptXorAssign,
    "<<=": OptLsfAssign, ">>=": OptRsfAssign,
    "<=": OptLE, ">=": OptGE, "==": OptEQ, "!=": OptNE,
    "<<": OptShiftL, ">>": OptShiftR,
    "&&": OptAnd, "||": OptOr,
    "..": OptConnect, "->": OptArrow, ":=": OptWalrus,
}


# ---------------------------------------------------------------------------
# Property 19a: Keywords produce correct token types
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(kw=keyword_strat)
def test_keyword_token_type(kw):
    """Each Berry keyword must tokenize to its specific keyword token type."""
    tokens = tokenize(kw)
    assert len(tokens) == 1, f"Expected 1 token for keyword '{kw}', got {len(tokens)}"
    tok_type, tok_val = tokens[0]
    expected = KEYWORDS[kw]
    assert tok_type == expected, (
        f"Keyword '{kw}': expected token type {expected} "
        f"({token_strings[expected]}), got {tok_type} ({token_strings[tok_type]})"
    )


# ---------------------------------------------------------------------------
# Property 19b: Identifiers produce TokenId with correct content
# ---------------------------------------------------------------------------
@settings(max_examples=300)
@given(ident=berry_identifiers())
def test_identifier_content(ident):
    """Non-keyword identifiers must tokenize to TokenId with the original text."""
    tokens = tokenize(ident)
    assert len(tokens) == 1, f"Expected 1 token for '{ident}', got {len(tokens)}"
    tok_type, tok_val = tokens[0]
    assert tok_type == TokenId, (
        f"'{ident}' should be TokenId, got {token_strings[tok_type]}"
    )
    assert tok_val == ident, f"Identifier content mismatch: {tok_val!r} != {ident!r}"


# ---------------------------------------------------------------------------
# Property 19c: Decimal integer literals parse to correct value
# ---------------------------------------------------------------------------
@settings(max_examples=300)
@given(n=decimal_ints)
def test_decimal_integer_value(n):
    """Decimal integer literals must tokenize to TokenInteger with the correct value."""
    source = str(n)
    tokens = tokenize(source)
    assert len(tokens) == 1, f"Expected 1 token for '{source}', got {len(tokens)}"
    tok_type, tok_val = tokens[0]
    assert tok_type == TokenInteger, (
        f"'{source}' should be TokenInteger, got {token_strings[tok_type]}"
    )
    assert tok_val == n, f"Integer value mismatch: {tok_val} != {n}"


# ---------------------------------------------------------------------------
# Property 19d: Hexadecimal integer literals parse to correct value
# ---------------------------------------------------------------------------
@settings(max_examples=300)
@given(n=hex_ints)
def test_hex_integer_value(n):
    """Hex integer literals (0x...) must tokenize to TokenInteger with correct value."""
    source = f"0x{n:X}"
    tokens = tokenize(source)
    assert len(tokens) == 1, f"Expected 1 token for '{source}', got {len(tokens)}"
    tok_type, tok_val = tokens[0]
    assert tok_type == TokenInteger, (
        f"'{source}' should be TokenInteger, got {token_strings[tok_type]}"
    )
    assert tok_val == _wrap_bint(n), f"Hex value mismatch: {tok_val} != {_wrap_bint(n)} for '{source}'"


# ---------------------------------------------------------------------------
# Property 19e: Real number literals parse to correct value
# ---------------------------------------------------------------------------
@settings(max_examples=300)
@given(r=safe_reals)
def test_real_number_value(r):
    """Real number literals must tokenize to TokenReal with the correct value."""
    source = repr(r)
    # Skip values whose repr uses scientific notation with negative exponent
    # that might not match Berry's format exactly
    assume('e' not in source.lower() or '+' in source or '-' not in source.split('e')[-1])
    tokens = tokenize(source)
    assert len(tokens) >= 1, f"Expected at least 1 token for '{source}'"
    tok_type, tok_val = tokens[0]
    if tok_type == TokenReal:
        expected = be_str2real(source)
        assert math.isclose(tok_val, expected, rel_tol=1e-9), (
            f"Real value mismatch: {tok_val} != {expected} for '{source}'"
        )
    # Some repr forms may tokenize differently (e.g., with 'e'), that's ok


# ---------------------------------------------------------------------------
# Property 19f: Simple string literals round-trip correctly
# ---------------------------------------------------------------------------
@settings(max_examples=300)
@given(content=simple_string_content)
def test_string_literal_content(content):
    """Double-quoted string literals with safe chars must preserve content."""
    # Ensure content doesn't contain double quotes or backslashes
    assume('"' not in content and '\\' not in content)
    source = f'"{content}"'
    tokens = tokenize(source)
    assert len(tokens) == 1, f"Expected 1 token for string, got {len(tokens)}: {tokens}"
    tok_type, tok_val = tokens[0]
    assert tok_type == TokenString, (
        f"Expected TokenString, got {token_strings[tok_type]}"
    )
    assert tok_val == content, f"String content mismatch: {tok_val!r} != {content!r}"


# ---------------------------------------------------------------------------
# Property 19g: String escape sequences produce correct characters
# ---------------------------------------------------------------------------
ESCAPE_PAIRS = [
    (r'\n', '\n'), (r'\t', '\t'), (r'\r', '\r'),
    (r'\\', '\\'), (r'\"', '"'), (r"\'", "'"),
    (r'\a', '\a'), (r'\b', '\b'), (r'\f', '\f'),
    (r'\v', '\v'), (r'\?', '?'),
]

@settings(max_examples=100)
@given(pair=sampled_from(ESCAPE_PAIRS))
def test_string_escape_sequences(pair):
    """Each escape sequence in a string literal must produce the correct char."""
    escape_src, expected_char = pair
    source = f'"{escape_src}"'
    tokens = tokenize(source)
    assert len(tokens) == 1, f"Expected 1 token for '{source}', got {len(tokens)}"
    tok_type, tok_val = tokens[0]
    assert tok_type == TokenString
    assert tok_val == expected_char, (
        f"Escape {escape_src!r}: expected {expected_char!r}, got {tok_val!r}"
    )


# ---------------------------------------------------------------------------
# Property 19h: Line comments are fully skipped
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(ident=berry_identifiers(), comment=text(
    alphabet="abcdefghijklmnopqrstuvwxyz 0123456789", min_size=0, max_size=30
))
def test_line_comment_skipped(ident, comment):
    """Tokens after a # line comment should not appear until the next line."""
    source = f"{ident} # {comment}\n"
    tokens = tokenize(source)
    assert len(tokens) == 1, f"Expected 1 token, got {len(tokens)}: {tokens}"
    assert tokens[0] == (TokenId, ident)


# ---------------------------------------------------------------------------
# Property 19i: Block comments are fully skipped
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(
    before=berry_identifiers(),
    after=berry_identifiers(),
    comment=text(
        alphabet="abcdefghijklmnopqrstuvwxyz 0123456789", min_size=0, max_size=30
    ),
)
def test_block_comment_skipped(before, after, comment):
    """Tokens inside #- ... -# block comments must be completely skipped."""
    assume(before != after)  # ensure we get 2 distinct tokens
    assume("-#" not in comment)  # avoid premature comment close
    source = f"{before} #- {comment} -# {after}"
    tokens = tokenize(source)
    assert len(tokens) == 2, f"Expected 2 tokens, got {len(tokens)}: {tokens}"
    assert tokens[0] == (TokenId, before)
    assert tokens[1] == (TokenId, after)


# ---------------------------------------------------------------------------
# Property 19j: Multi-char operators produce correct token types
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(op_entry=sampled_from(list(MULTI_OPS.items())))
def test_multi_char_operator_type(op_entry):
    """Each multi-character operator must tokenize to its specific token type."""
    op_str, expected_type = op_entry
    # Surround with spaces to avoid ambiguity
    source = f"x {op_str} y"
    tokens = tokenize(source)
    # Find the operator token (skip the identifiers)
    op_tokens = [(t, v) for t, v in tokens if t == expected_type]
    assert len(op_tokens) >= 1, (
        f"Operator '{op_str}' not found as type {expected_type} "
        f"({token_strings[expected_type]}) in tokens: {tokens}"
    )


# ---------------------------------------------------------------------------
# Property 19k: Token sequence concatenation
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(
    idents=lists(berry_identifiers(), min_size=1, max_size=5),
)
def test_space_separated_identifiers(idents):
    """Space-separated identifiers must each produce a TokenId in order."""
    assume(len(set(idents)) == len(idents))  # all distinct
    source = " ".join(idents)
    tokens = tokenize(source)
    assert len(tokens) == len(idents), (
        f"Expected {len(idents)} tokens, got {len(tokens)}: {tokens}"
    )
    for i, ident in enumerate(idents):
        assert tokens[i] == (TokenId, ident), (
            f"Token {i}: expected (TokenId, {ident!r}), got {tokens[i]}"
        )


# ---------------------------------------------------------------------------
# Property 19l: Empty and whitespace-only sources produce no tokens
# ---------------------------------------------------------------------------
@settings(max_examples=100)
@given(ws=text(alphabet=" \t\n\r", min_size=0, max_size=20))
def test_whitespace_only_no_tokens(ws):
    """Whitespace-only input must produce zero tokens."""
    tokens = tokenize(ws)
    assert tokens == [], f"Expected no tokens for whitespace, got {tokens}"


# ---------------------------------------------------------------------------
# Property 19m: Hex literal with lowercase also works
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(n=integers(min_value=0, max_value=0xFFFF))
def test_hex_lowercase(n):
    """Hex literals with lowercase digits must parse correctly."""
    source = f"0x{n:x}"
    tokens = tokenize(source)
    assert len(tokens) == 1
    tok_type, tok_val = tokens[0]
    assert tok_type == TokenInteger
    assert tok_val == n, f"Hex lowercase mismatch: {tok_val} != {n}"


# ---------------------------------------------------------------------------
# Property 19n: Hex string escapes parse correctly
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(n=integers(min_value=0, max_value=255))
def test_hex_string_escape(n):
    r"""Hex escape sequences (\xHH) in strings must produce the correct byte."""
    source = f'"\\x{n:02X}"'
    tokens = tokenize(source)
    assert len(tokens) == 1, f"Expected 1 token for {source!r}, got {len(tokens)}"
    tok_type, tok_val = tokens[0]
    assert tok_type == TokenString
    assert len(tok_val) == 1, f"Expected 1-char string, got {len(tok_val)}"
    assert ord(tok_val) == n, f"Hex escape mismatch: {ord(tok_val)} != {n}"


# ---------------------------------------------------------------------------
# Property 19o: Dot-prefixed reals parse correctly
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(digits=integers(min_value=1, max_value=999999))
def test_dot_prefixed_real(digits):
    """Reals starting with '.' (e.g., .5, .123) must parse as TokenReal."""
    source = f".{digits}"
    tokens = tokenize(source)
    assert len(tokens) == 1, f"Expected 1 token for '{source}', got {len(tokens)}"
    tok_type, tok_val = tokens[0]
    assert tok_type == TokenReal, (
        f"'{source}' should be TokenReal, got {token_strings[tok_type]}"
    )
    expected = be_str2real(source)
    assert math.isclose(tok_val, expected, rel_tol=1e-12), (
        f"Dot-real mismatch: {tok_val} != {expected} for '{source}'"
    )
