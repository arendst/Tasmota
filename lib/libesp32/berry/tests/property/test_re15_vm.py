"""
Unit tests for berry_port.re15_vm — recursiveloopprog.

Tests cover:
- Simple literal match (anchored)
- Search mode (non-anchored)
- Capture groups producing correct submatch indices
- Split/RSplit backtracking: greedy vs non-greedy
- Bol/Eol assertions
- Character classes and named classes in VM context
- Requirements: 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8
"""

from berry_port.re15_compiler import compilecode
from berry_port.re15_opcodes import RE1_5_SUCCESS
from berry_port.re15_vm import recursiveloopprog


# ---------------------------------------------------------------------------
# Helper
# ---------------------------------------------------------------------------

def _compile(pattern):
    """Compile pattern, assert success, return ByteProg."""
    prog, err = compilecode(pattern)
    assert err == RE1_5_SUCCESS, f"compile failed for {pattern!r}: {err}"
    return prog


def _run(pattern, text, is_anchored, nsubp=None):
    """Compile pattern, run VM, return (matched, subp)."""
    prog = _compile(pattern)
    if nsubp is None:
        nsubp = (prog.sub + 1) * 2
    subp = [None] * nsubp
    matched = recursiveloopprog(prog, text, subp, nsubp, is_anchored)
    return matched, subp


# ---------------------------------------------------------------------------
# Simple literal match — anchored (Req 2.1, 2.3)
# ---------------------------------------------------------------------------

def test_literal_match_anchored():
    matched, subp = _run("abc", "abc", is_anchored=True)
    assert matched == 1
    assert subp[0] == 0  # full match start
    assert subp[1] == 3  # full match end


def test_literal_no_match_anchored():
    matched, subp = _run("abc", "xbc", is_anchored=True)
    assert matched == 0


def test_literal_partial_no_match_anchored():
    """Anchored match must match from the start — 'abc' in 'xabc' fails."""
    matched, subp = _run("abc", "xabc", is_anchored=True)
    assert matched == 0


def test_literal_match_prefix_anchored():
    """Anchored match succeeds when pattern matches a prefix of text."""
    matched, subp = _run("ab", "abcdef", is_anchored=True)
    assert matched == 1
    assert subp[0] == 0
    assert subp[1] == 2


# ---------------------------------------------------------------------------
# Search mode — non-anchored (Req 2.1, 2.4)
# ---------------------------------------------------------------------------

def test_search_finds_literal():
    matched, subp = _run("b", "abc", is_anchored=False)
    assert matched == 1
    assert subp[0] == 1  # match starts at index 1
    assert subp[1] == 2


def test_search_finds_at_start():
    matched, subp = _run("a", "abc", is_anchored=False)
    assert matched == 1
    assert subp[0] == 0
    assert subp[1] == 1


def test_search_finds_at_end():
    matched, subp = _run("c", "abc", is_anchored=False)
    assert matched == 1
    assert subp[0] == 2
    assert subp[1] == 3


def test_search_no_match():
    matched, subp = _run("z", "abc", is_anchored=False)
    assert matched == 0


def test_search_multi_char():
    matched, subp = _run("cd", "abcdef", is_anchored=False)
    assert matched == 1
    assert subp[0] == 2
    assert subp[1] == 4


# ---------------------------------------------------------------------------
# Capture groups (Req 2.5)
# ---------------------------------------------------------------------------

def test_two_capture_groups():
    matched, subp = _run("(a)(b)", "ab", is_anchored=True)
    assert matched == 1
    # group 0 = full match
    assert subp[0] == 0
    assert subp[1] == 2
    # group 1 = (a)
    assert subp[2] == 0
    assert subp[3] == 1
    # group 2 = (b)
    assert subp[4] == 1
    assert subp[5] == 2


def test_nested_capture_groups():
    matched, subp = _run("((a)b)", "ab", is_anchored=True)
    assert matched == 1
    # group 0 = full match
    assert subp[0] == 0
    assert subp[1] == 2
    # group 1 = ((a)b)
    assert subp[2] == 0
    assert subp[3] == 2
    # group 2 = (a)
    assert subp[4] == 0
    assert subp[5] == 1


def test_capture_group_search():
    """Capture groups work in non-anchored search mode."""
    matched, subp = _run("(b)(c)", "abcd", is_anchored=False)
    assert matched == 1
    assert subp[0] == 1
    assert subp[1] == 3
    assert subp[2] == 1
    assert subp[3] == 2
    assert subp[4] == 2
    assert subp[5] == 3


def test_non_capturing_group():
    """(?:...) should not produce submatch entries."""
    prog = _compile("(?:a)(b)")
    assert prog.sub == 1  # only one capture group
    subp = [None] * 4  # group 0 + group 1
    matched = recursiveloopprog(prog, "ab", subp, 4, True)
    assert matched == 1
    assert subp[0] == 0
    assert subp[1] == 2
    assert subp[2] == 1
    assert subp[3] == 2


# ---------------------------------------------------------------------------
# Split/RSplit backtracking — greedy vs non-greedy (Req 2.6, 2.7)
# ---------------------------------------------------------------------------

def test_greedy_star():
    """'a*' is greedy — matches as many 'a's as possible."""
    matched, subp = _run("(a*)", "aaa", is_anchored=True)
    assert matched == 1
    assert subp[2] == 0
    assert subp[3] == 3  # greedy: all three a's


def test_non_greedy_star():
    """'a*?' is non-greedy — matches as few 'a's as possible."""
    matched, subp = _run("(a*?)", "aaa", is_anchored=True)
    assert matched == 1
    assert subp[2] == 0
    assert subp[3] == 0  # non-greedy: zero a's


def test_greedy_plus():
    """'a+' is greedy — matches as many 'a's as possible."""
    matched, subp = _run("(a+)", "aaa", is_anchored=True)
    assert matched == 1
    assert subp[2] == 0
    assert subp[3] == 3


def test_non_greedy_plus():
    """'a+?' is non-greedy — matches as few 'a's as possible (at least one)."""
    matched, subp = _run("(a+?)", "aaa", is_anchored=True)
    assert matched == 1
    assert subp[2] == 0
    assert subp[3] == 1  # non-greedy: just one 'a'


def test_greedy_question():
    """'a?' is greedy — prefers to match 'a'."""
    matched, subp = _run("(a?)", "a", is_anchored=True)
    assert matched == 1
    assert subp[2] == 0
    assert subp[3] == 1  # greedy: matched the 'a'


def test_non_greedy_question():
    """'a??' is non-greedy — prefers to skip 'a'."""
    matched, subp = _run("(a??)", "a", is_anchored=True)
    assert matched == 1
    assert subp[2] == 0
    assert subp[3] == 0  # non-greedy: skipped the 'a'


def test_alternation_backtracking():
    """'a|ab' should match 'a' first (leftmost)."""
    matched, subp = _run("a|ab", "ab", is_anchored=True)
    assert matched == 1
    assert subp[0] == 0
    assert subp[1] == 1  # matched just 'a', not 'ab'


# ---------------------------------------------------------------------------
# Bol / Eol assertions (Req 2.2)
# ---------------------------------------------------------------------------

def test_bol_anchored_match():
    """'^a' matches 'a' at start."""
    matched, subp = _run("^a", "abc", is_anchored=True)
    assert matched == 1
    assert subp[0] == 0
    assert subp[1] == 1


def test_bol_anchored_no_match():
    """'^b' does not match 'abc' at start."""
    matched, subp = _run("^b", "abc", is_anchored=True)
    assert matched == 0


def test_eol_anchored_match():
    """'c$' matches at end of 'abc'."""
    matched, subp = _run("c$", "abc", is_anchored=False)
    assert matched == 1
    assert subp[0] == 2
    assert subp[1] == 3


def test_eol_anchored_no_match():
    """'a$' does not match 'abc' (a is not at end)."""
    matched, subp = _run("a$", "abc", is_anchored=False)
    assert matched == 0


def test_bol_eol_full_line():
    """'^abc$' matches exactly 'abc'."""
    matched, subp = _run("^abc$", "abc", is_anchored=True)
    assert matched == 1
    assert subp[0] == 0
    assert subp[1] == 3


def test_bol_eol_no_match_extra():
    """'^abc$' does not match 'abcd'."""
    matched, subp = _run("^abc$", "abcd", is_anchored=True)
    assert matched == 0


# ---------------------------------------------------------------------------
# Character classes in VM context (Req 2.2, 2.8)
# ---------------------------------------------------------------------------

def test_char_class_match():
    """'[a-z]' matches lowercase letter."""
    matched, subp = _run("[a-z]", "m", is_anchored=True)
    assert matched == 1


def test_char_class_no_match():
    """'[a-z]' does not match digit."""
    matched, subp = _run("[a-z]", "5", is_anchored=True)
    assert matched == 0


def test_negated_class_match():
    """'[^0-9]' matches non-digit."""
    matched, subp = _run("[^0-9]", "a", is_anchored=True)
    assert matched == 1


def test_negated_class_no_match():
    """'[^0-9]' does not match digit."""
    matched, subp = _run("[^0-9]", "5", is_anchored=True)
    assert matched == 0


def test_char_class_in_pattern():
    """'[abc]+' matches sequence of a/b/c."""
    matched, subp = _run("[abc]+", "cab", is_anchored=True)
    assert matched == 1
    assert subp[0] == 0
    assert subp[1] == 3


# ---------------------------------------------------------------------------
# Named classes in VM context (Req 2.2, 2.8)
# ---------------------------------------------------------------------------

def test_named_d_match():
    r"""'\d' matches digit."""
    matched, subp = _run(r"\d", "7", is_anchored=True)
    assert matched == 1


def test_named_d_no_match():
    r"""'\d' does not match letter."""
    matched, subp = _run(r"\d", "a", is_anchored=True)
    assert matched == 0


def test_named_w_plus():
    r"""'\w+' matches word characters."""
    matched, subp = _run(r"\w+", "hello_42", is_anchored=True)
    assert matched == 1
    assert subp[0] == 0
    assert subp[1] == 8


def test_named_s_match():
    r"""'\s' matches whitespace."""
    matched, subp = _run(r"\s", " ", is_anchored=True)
    assert matched == 1


def test_named_S_match():
    r"""'\S+' matches non-whitespace."""
    matched, subp = _run(r"\S+", "abc", is_anchored=True)
    assert matched == 1
    assert subp[0] == 0
    assert subp[1] == 3


def test_named_D_match():
    r"""'\D' matches non-digit."""
    matched, subp = _run(r"\D", "x", is_anchored=True)
    assert matched == 1


def test_named_W_match():
    r"""'\W' matches non-word character."""
    matched, subp = _run(r"\W", "!", is_anchored=True)
    assert matched == 1


# ---------------------------------------------------------------------------
# Dot (Any) opcode (Req 2.2)
# ---------------------------------------------------------------------------

def test_dot_matches_any_char():
    matched, subp = _run(".", "x", is_anchored=True)
    assert matched == 1


def test_dot_does_not_match_empty():
    matched, subp = _run(".", "", is_anchored=True)
    assert matched == 0


def test_dot_star_search():
    """'a.*b' finds match spanning multiple characters."""
    matched, subp = _run("a.*b", "aXYZb", is_anchored=True)
    assert matched == 1
    assert subp[0] == 0
    assert subp[1] == 5


# ---------------------------------------------------------------------------
# Edge cases (Req 2.1, 2.8)
# ---------------------------------------------------------------------------

def test_empty_pattern_matches_empty_string():
    matched, subp = _run("", "", is_anchored=True)
    assert matched == 1


def test_empty_pattern_matches_any_string():
    """Empty pattern matches at start of any string."""
    matched, subp = _run("", "abc", is_anchored=True)
    assert matched == 1
    assert subp[0] == 0
    assert subp[1] == 0


def test_no_match_past_end():
    """Pattern requiring chars should fail on empty string."""
    matched, subp = _run("a", "", is_anchored=True)
    assert matched == 0


def test_complex_pattern_search():
    """'a.*?b(z+)' in 'zaaaabbbccbbzzzee' — from spec checkpoint."""
    matched, subp = _run("a.*?b(z+)", "zaaaabbbccbbzzzee", is_anchored=False)
    assert matched == 1
    # Full match: 'aaaabbbccbbzzz' (indices 1..15)
    full = "zaaaabbbccbbzzzee"[subp[0]:subp[1]]
    assert full == "aaaabbbccbbzzz"
    # Group 1: 'zzz' (indices 12..15)
    grp1 = "zaaaabbbccbbzzzee"[subp[2]:subp[3]]
    assert grp1 == "zzz"
