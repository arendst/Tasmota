#-
  Property-Based Tests for Berry Preprocessor

  These tests validate the correctness properties defined in the design
  document for the Berry preprocessor feature. Each test generates random
  inputs (macro names, values, source code patterns) and verifies that
  the preprocessor behaves correctly across 100 iterations per property.

  Properties tested:
    P1  - Directive lines produce no tokens (are "silent")
    P2  - Conditional branches select correctly based on truthiness
    P2b - Negation (#if !MACRO) inverts truthiness correctly
    P2c - #elif chains select the first matching branch
    P3  - Mismatched conditionals (#if without #endif, etc.) produce errors
    P4  - Line numbers remain accurate after skipped blocks
    P5  - Translatable strings substitute correctly based on macro type
    P6  - Macro replacement values are never recursively expanded
    P7  - # after a string followed by non-identifier is treated as comment
    P8  - #define / #undef round-trips preserve correct truthiness
    P9  - In-script #define overrides API-defined macros
    P12 - Existing Berry comment syntax is preserved (backward compat)
    P13 - Directive keywords (define, undef, endif) work as normal identifiers
    P16 - Empty defines are truthy, zero is falsy, strings are truthy
    P17 - Berry preproc module operations match C API behavior
    P18 - preproc.defined() and preproc.value() are consistent

  The PBT helper functions are inlined here (Berry has no load() function).
  See tests/pbt_helper.be for the documented standalone version.
-#

import introspect
if introspect.module("preproc") == nil return end
import preproc
import string
import math

# =============================================
# Inline PBT helpers (see tests/pbt_helper.be for docs)
# =============================================
math.srand(12345)

def pbt_random_int(lo, hi)
    if lo >= hi return lo end
    var span = hi - lo + 1
    var r = math.rand()
    if r < 0 r = -r end
    return lo + (r % span)
end

def pbt_random_string(min_len, max_len)
    var len = pbt_random_int(min_len, max_len)
    var s = ''
    for i : 0 .. len - 1
        s += string.char(pbt_random_int(32, 126))
    end
    return s
end

def pbt_random_ident()
    var starts = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_'
    var chars = starts + '0123456789'
    var len = pbt_random_int(1, 12)
    var s = starts[pbt_random_int(0, 52)]
    for i : 1 .. len - 1
        s += chars[pbt_random_int(0, 62)]
    end
    return s
end

def pbt_random_choice(lst)
    return lst[pbt_random_int(0, lst.size() - 1)]
end

def pbt_run(name, fn, iterations)
    if iterations == nil iterations = 100 end
    for i : 0 .. iterations - 1
        try
            var result = fn(i)
            if result != nil && result != true
                assert(false, 'PBT ' + name + ' failed at iteration ' + str(i))
            end
        except .. as e, m
            print('PBT FAIL: ' + name + ' iteration ' + str(i) + ': ' + e + ' - ' + m)
            assert(false, 'PBT ' + name + ' failed at iteration ' + str(i) + ': ' + e + ' - ' + m)
        end
    end
end

var _pbt_reserved = [
    'if', 'elif', 'else', 'while', 'for', 'def', 'end', 'class',
    'break', 'continue', 'return', 'true', 'false', 'nil', 'var',
    'do', 'import', 'as', 'try', 'except', 'raise', 'static',
    'define', 'undef', 'endif'
]

def pbt_random_safe_ident()
    while true
        var id = pbt_random_ident()
        var ok = true
        for kw : _pbt_reserved
            if id == kw ok = false break end
        end
        if ok return id end
    end
end

# Generates strings safe for embedding in Berry source (no quotes, #, or \)
def pbt_random_safe_string(min_len, max_len)
    var len = pbt_random_int(min_len, max_len)
    var s = ''
    for i : 0 .. len - 1
        var c = pbt_random_int(32, 126)
        while c == 34 || c == 39 || c == 92 || c == 35
            c = pbt_random_int(32, 126)
        end
        s += string.char(c)
    end
    return s
end

# =============================================
# Property 1: Directive lines are silent
# Validates: Requirements 1.1, 1.4
#
# Verifies that preprocessor directive lines (#define, #if, #endif,
# #undef) are consumed entirely by the lexer and produce NO tokens
# to the parser. The only tokens emitted should come from non-directive
# lines. We build source with random directive lines surrounding a
# simple "return 42" and verify the result is always 42.
# =============================================
pbt_run('P1_directive_silent', def (i)
    preproc.clear()
    var macro = 'P1_' + pbt_random_safe_ident()
    var val = pbt_random_int(1, 100)
    # Source has 5 directive lines and 2 code lines — only code lines
    # should produce tokens
    var src = '#define ' + macro + ' ' + str(val) + '\n'
    src += 'var _p1_result = 42\n'
    src += '#if ' + macro + '\n'
    src += '#endif\n'
    src += '#undef ' + macro + '\n'
    src += 'return _p1_result'
    var fn = compile(src)
    assert(fn() == 42, 'P1: directives leaked tokens')
    return true
end, 100)

# =============================================
# Property 2: Conditional branch selection with truthiness
# Validates: Requirements 2.1, 2.2, 2.3, 2.4, 2.9
#
# Verifies that #if/#else selects the correct branch based on macro
# truthiness. Randomly picks one of three modes per iteration:
#   mode 0: macro defined with non-zero value → truthy → branch A
#   mode 1: macro defined with value 0 → falsy → branch B
#   mode 2: macro undefined → falsy → branch B
# =============================================
pbt_run('P2_branch_selection', def (i)
    preproc.clear()
    var macro = 'P2_' + pbt_random_safe_ident()
    var mode = pbt_random_int(0, 2)
    var expected
    var src = ''
    if mode == 0
        src += '#define ' + macro + ' ' + str(pbt_random_int(1, 999)) + '\n'
        expected = 'A'
    elif mode == 1
        src += '#define ' + macro + ' 0\n'
        expected = 'B'
    else
        expected = 'B'
    end
    src += '#if ' + macro + '\n'
    src += "return 'A'\n"
    src += '#else\n'
    src += "return 'B'\n"
    src += '#endif\n'
    var fn = compile(src)
    assert(fn() == expected, 'P2: wrong branch mode=' + str(mode))
    return true
end, 100)

# Property 2b: Negation with #if !MACRO
# Verifies that the ! operator correctly inverts truthiness:
#   mode 0: macro truthy → !macro is falsy → branch B
#   mode 1: macro defined as 0 → !macro is truthy → branch A
#   mode 2: macro undefined → !macro is truthy → branch A
pbt_run('P2b_negation_branch', def (i)
    preproc.clear()
    var macro = 'P2N_' + pbt_random_safe_ident()
    var mode = pbt_random_int(0, 2)
    var expected
    var src = ''
    if mode == 0
        src += '#define ' + macro + ' ' + str(pbt_random_int(1, 999)) + '\n'
        expected = 'B'
    elif mode == 1
        src += '#define ' + macro + ' 0\n'
        expected = 'A'
    else
        expected = 'A'
    end
    src += '#if !' + macro + '\n'
    src += "return 'A'\n"
    src += '#else\n'
    src += "return 'B'\n"
    src += '#endif\n'
    var fn = compile(src)
    assert(fn() == expected, 'P2b: wrong branch mode=' + str(mode))
    return true
end, 100)

# Property 2c: #elif multi-branch selection
# Verifies that in an #if/#elif/#else chain, exactly the first
# matching branch is taken. Randomly defines two macros as truthy
# or not, then checks the correct branch is selected:
#   t1=true → always branch A (regardless of t2)
#   t1=false, t2=true → branch B
#   t1=false, t2=false → branch C (else)
pbt_run('P2c_elif_selection', def (i)
    preproc.clear()
    var m1 = 'P2E1_' + pbt_random_safe_ident()
    var m2 = 'P2E2_' + pbt_random_safe_ident()
    var t1 = pbt_random_int(0, 1) == 1
    var t2 = pbt_random_int(0, 1) == 1
    var src = ''
    if t1 src += '#define ' + m1 + ' 1\n' end
    if t2 src += '#define ' + m2 + ' 1\n' end
    src += '#if ' + m1 + '\n'
    src += "return 'A'\n"
    src += '#elif ' + m2 + '\n'
    src += "return 'B'\n"
    src += '#else\n'
    src += "return 'C'\n"
    src += '#endif\n'
    var expected
    if t1 expected = 'A'
    elif t2 expected = 'B'
    else expected = 'C'
    end
    var fn = compile(src)
    assert(fn() == expected, 'P2c: wrong branch')
    return true
end, 100)

# =============================================
# Property 3: Mismatched conditionals produce errors
# Validates: Requirements 2.7, 2.8
#
# Verifies that structurally invalid conditional blocks always
# produce syntax errors. Randomly picks one of four error cases:
#   mode 0: unclosed #if (no matching #endif before EOF)
#   mode 1: orphan #else (no preceding #if)
#   mode 2: orphan #endif (no preceding #if)
#   mode 3: orphan #elif (no preceding #if)
# =============================================
pbt_run('P3_mismatched_errors', def (i)
    preproc.clear()
    var macro = 'P3_' + pbt_random_safe_ident()
    var mode = pbt_random_int(0, 3)
    var src
    if mode == 0
        # Unclosed #if — missing #endif
        src = '#define ' + macro + ' 1\n#if ' + macro + '\nvar x = 1\n'
    elif mode == 1
        # #else without a preceding #if
        src = '#else\nvar x = 1\n'
    elif mode == 2
        # #endif without a preceding #if
        src = '#endif\n'
    else
        # #elif without a preceding #if
        src = '#elif ' + macro + '\nvar x = 1\n'
    end
    try
        compile(src)
        assert(false, 'P3: expected syntax_error mode=' + str(mode))
    except .. as e, m
        assert(e == 'syntax_error', 'P3: expected syntax_error got ' + e)
    end
    return true
end, 100)

# =============================================
# Property 4: Line numbers are accurate through skipped blocks
# Validates: Requirements 2.10
#
# Verifies that the lexer correctly counts newlines inside skipped
# conditional blocks, so error messages after the block report the
# right line number. Strategy:
#   1. Create a skipped #if block with a random number of lines (1-20)
#   2. After #endif, place an intentional syntax error
#   3. Verify the error message contains the correct line number
# The expected line = 1 (#if) + skip_lines + 1 (#endif) + 1 (error line)
# =============================================
pbt_run('P4_line_numbers', def (i)
    preproc.clear()
    var skip_lines = pbt_random_int(1, 20)
    var src = '#if UNDEFINED_P4\n'
    for j : 0 .. skip_lines - 1
        src += 'skipped line ' + str(j) + '\n'
    end
    src += '#endif\n'
    # Error should be reported on this line
    var error_line = skip_lines + 3
    src += '!!!syntax_error_here!!!\n'
    try
        compile(src)
        assert(false, 'P4: expected syntax error')
    except .. as e, m
        assert(e == 'syntax_error', 'P4: expected syntax_error got ' + e)
        var line_str = str(error_line)
        assert(string.find(m, line_str) >= 0,
            'P4: expected line ' + line_str + ' in error: ' + m)
    end
    return true
end, 100)

# =============================================
# Property 5: Translatable string substitution
# Validates: Requirements 1.6, 3.1, 3.2, 3.3, 3.4, 3.5, 6.5, 6.7
#
# Verifies that $MACRO"text" substitution works correctly for all
# three cases, using both single and double quoted strings:
#   mode 0: macro defined with string value → emits replacement
#   mode 1: macro defined with integer value → emits default text
#   mode 2: macro undefined → emits default text
# Uses random default text and replacement strings each iteration.
# =============================================
pbt_run('P5_translatable_sub', def (i)
    preproc.clear()
    var macro = 'P5_' + pbt_random_safe_ident()
    var default_text = pbt_random_safe_string(1, 20)
    var replacement = pbt_random_safe_string(1, 20)
    var mode = pbt_random_int(0, 2)
    var expected
    if mode == 0
        # String macro → replacement text used
        preproc.define(macro, replacement)
        expected = replacement
    elif mode == 1
        # Integer macro → default text preserved
        preproc.define(macro, str(pbt_random_int(0, 100)))
        expected = default_text
    else
        # Undefined macro → default text preserved
        expected = default_text
    end
    # Test with double-quoted string
    var src = 'return $' + macro + '"' + default_text + '"'
    var fn = compile(src)
    assert(fn() == expected, 'P5: dq mode=' + str(mode))
    # Test with single-quoted string (same behavior expected)
    preproc.clear()
    if mode == 0
        preproc.define(macro, replacement)
    elif mode == 1
        preproc.define(macro, str(pbt_random_int(0, 100)))
    end
    src = "return $" + macro + "'" + default_text + "'"
    fn = compile(src)
    assert(fn() == expected, 'P5: sq mode=' + str(mode))
    return true
end, 100)

# =============================================
# Property 6: No recursive macro expansion
# Validates: Requirements 3.8
#
# Verifies that macro replacement values are used verbatim, even if
# they contain text that looks like another macro name. Strategy:
#   1. Define macro M1 with a value that contains M2's name as text
#   2. Define macro M2 with some other value
#   3. Use $M1"default" — should get M1's literal value, NOT with
#      M2 expanded inside it
# =============================================
pbt_run('P6_no_recursive_expand', def (i)
    preproc.clear()
    var m1 = 'P6A_' + pbt_random_safe_ident()
    var m2 = 'P6B_' + pbt_random_safe_ident()
    var inner_val = pbt_random_safe_string(1, 10)
    # M1's value deliberately contains M2's name as literal text
    var outer_val = 'prefix_' + m2 + '_suffix'
    preproc.define(m1, outer_val)
    preproc.define(m2, inner_val)
    var src = 'return $' + m1 + '"default"'
    var fn = compile(src)
    # Must get the literal outer_val, not a recursively expanded version
    assert(fn() == outer_val, 'P6: expected verbatim')
    return true
end, 100)

# =============================================
# Property 7: Non-identifier after string-hash is a comment
# Validates: Requirements 3.9, 9.5
#
# Verifies that when # appears after a string literal but is NOT
# followed by an identifier character (letter or underscore), the #
# is treated as a regular Berry line comment. Strategy:
#   1. Generate a random string literal
#   2. Follow it with # and a random non-identifier character
#      (digit, space, punctuation — but NOT '-' which starts #- block comments)
#   3. Verify the string value is returned unchanged
# =============================================
pbt_run('P7_non_ident_comment', def (i)
    preproc.clear()
    var text = pbt_random_safe_string(1, 15)
    # Characters that are NOT valid identifier starts (letter/underscore)
    # Excludes '-' to avoid triggering #- block comment syntax
    var non_ident_chars = '0123456789 !@$%^&*()+=[]{}|;:,.<>?/'
    var ch = non_ident_chars[pbt_random_int(0, size(non_ident_chars) - 2)]
    var src = 'return "' + text + '"#' + ch + ' comment text'
    var fn = compile(src)
    assert(fn() == text, 'P7: expected default text')
    return true
end, 100)

# =============================================
# Property 8: Define/undef round trip
# Validates: Requirements 4.1, 4.2, 4.3, 2.11, 2.12
#
# Verifies the full lifecycle of macro definitions:
#   mode 0: empty #define → truthy, then #undef → falsy
#   mode 1: #define MACRO 0 → falsy (even though defined)
#   mode 2: #define MACRO <nonzero> → truthy
# Each mode uses a random macro name and (for mode 2) random value.
# =============================================
pbt_run('P8_define_undef_roundtrip', def (i)
    preproc.clear()
    var macro = 'P8_' + pbt_random_safe_ident()
    var mode = pbt_random_int(0, 2)
    var src
    if mode == 0
        # Empty define is truthy (stored as 1), undef makes it falsy
        src = '#define ' + macro + '\n'
        src += '#if ' + macro + '\n'
        src += "var _p8a = 'yes'\n"
        src += '#else\n'
        src += "var _p8a = 'no'\n"
        src += '#endif\n'
        src += '#undef ' + macro + '\n'
        src += '#if ' + macro + '\n'
        src += "var _p8b = 'yes'\n"
        src += '#else\n'
        src += "var _p8b = 'no'\n"
        src += '#endif\n'
        src += 'return [_p8a, _p8b]'
        var fn = compile(src)
        var res = fn()
        assert(res[0] == 'yes', 'P8: empty define should be truthy')
        assert(res[1] == 'no', 'P8: after undef should be falsy')
    elif mode == 1
        # Define with value 0 is falsy
        src = '#define ' + macro + ' 0\n'
        src += '#if ' + macro + '\n'
        src += "return 'yes'\n"
        src += '#else\n'
        src += "return 'no'\n"
        src += '#endif\n'
        var fn = compile(src)
        assert(fn() == 'no', 'P8: define 0 should be falsy')
    else
        # Define with non-zero integer is truthy
        var val = pbt_random_int(1, 999)
        src = '#define ' + macro + ' ' + str(val) + '\n'
        src += '#if ' + macro + '\n'
        src += "return 'yes'\n"
        src += '#else\n'
        src += "return 'no'\n"
        src += '#endif\n'
        var fn = compile(src)
        assert(fn() == 'yes', 'P8: define nonzero should be truthy')
    end
    return true
end, 100)

# =============================================
# Property 9: In-script define overrides API definitions
# Validates: Requirements 4.4
#
# Verifies that when a macro is defined via the Berry module (C API
# equivalent) and then redefined in-script with #define, the in-script
# value takes precedence for translatable string substitution.
# Uses alphanumeric-prefixed values to ensure they're stored as
# BE_STRING (not BE_INT), which is required for translatable strings.
# =============================================
pbt_run('P9_override', def (i)
    preproc.clear()
    var macro = 'P9_' + pbt_random_safe_ident()
    # Prefix with letters to guarantee non-numeric (stored as BE_STRING)
    var api_val = 'apival' + str(pbt_random_int(100, 999))
    var script_val = 'scrval' + str(pbt_random_int(100, 999))
    while script_val == api_val
        script_val = 'scrval' + str(pbt_random_int(100, 999))
    end
    # Define via Berry module first
    preproc.define(macro, api_val)
    # In-script #define should override for the compilation
    var src = '#define ' + macro + ' ' + script_val + '\n'
    src += 'return $' + macro + '"default"'
    var fn = compile(src)
    assert(fn() == script_val, 'P9: macro=' + macro + ' expected="' + script_val + '" got="' + str(fn()) + '"')
    return true
end, 100)

# =============================================
# Property 12: Backward compatibility
# Validates: Requirements 1.2, 6.1, 6.2, 6.4
#
# Verifies that existing Berry comment syntax is completely preserved
# when no preprocessor directives or translatable strings are used.
# Randomly tests four comment patterns:
#   mode 0: # followed by space (line comment)
#   mode 1: #- ... -# (block comment)
#   mode 2: # at end of line
#   mode 3: # on its own line as a comment
# All should compile and return the expected value unchanged.
# =============================================
pbt_run('P12_backward_compat', def (i)
    preproc.clear()
    var val = pbt_random_int(1, 1000)
    var mode = pbt_random_int(0, 3)
    var src
    var expected
    if mode == 0
        # "# text" is a line comment — value before it is returned
        src = 'return ' + str(val) + ' # this is a comment'
        expected = val
    elif mode == 1
        # "#- ... -#" is a block comment — can appear inline
        src = 'return #- block -# ' + str(val)
        expected = val
    elif mode == 2
        # Bare "#" at end of line is a comment
        src = 'var _bc = ' + str(val) + ' #\nreturn _bc'
        expected = val
    else
        # Full-line comment starting with #
        src = 'var _bc = ' + str(val) + '\n# comment line\nreturn _bc'
        expected = val
    end
    var fn = compile(src)
    assert(fn() == expected, 'P12: mode=' + str(mode))
    return true
end, 100)

# =============================================
# Property 13: Directive keywords are not reserved
# Validates: Requirements 6.3
#
# Verifies that preprocessor directive keywords (define, undef, endif)
# can be used as normal Berry variable names when they appear in
# non-directive context (i.e., not after # at line start).
# Randomly picks one keyword per iteration and uses it as a variable.
# =============================================
pbt_run('P13_keywords_not_reserved', def (i)
    preproc.clear()
    var keywords = ['define', 'undef', 'endif']
    var kw = pbt_random_choice(keywords)
    var val = pbt_random_int(1, 1000)
    # Use the directive keyword as a regular variable name
    var src = 'var ' + kw + ' = ' + str(val) + '\nreturn ' + kw
    var fn = compile(src)
    assert(fn() == val, 'P13: keyword "' + kw + '" as identifier')
    return true
end, 100)

# =============================================
# Property 16: Empty defines are truthy, zero is falsy
# Validates: Requirements 2.11, 2.12, 4.1
#
# Verifies the complete truthiness model across all value types:
#   mode 0: empty #define (no value) → stored as 1 → truthy
#   mode 1: #define MACRO 0 → stored as integer 0 → falsy
#   mode 2: #define MACRO <nonzero int> → truthy
#   mode 3: #define MACRO <string> → truthy
# This is critical for Tasmota compatibility where empty C defines
# like "#define USE_MATTER" must be treated as truthy.
# =============================================
pbt_run('P16_truthiness', def (i)
    preproc.clear()
    var macro = 'P16_' + pbt_random_safe_ident()
    var mode = pbt_random_int(0, 3)
    var src = ''
    var expected
    if mode == 0
        src += '#define ' + macro + '\n'
        expected = 'yes'
    elif mode == 1
        src += '#define ' + macro + ' 0\n'
        expected = 'no'
    elif mode == 2
        src += '#define ' + macro + ' ' + str(pbt_random_int(1, 999)) + '\n'
        expected = 'yes'
    else
        src += '#define ' + macro + ' ' + pbt_random_safe_string(1, 8) + '\n'
        expected = 'yes'
    end
    src += '#if ' + macro + '\n'
    src += "return 'yes'\n"
    src += '#else\n'
    src += "return 'no'\n"
    src += '#endif\n'
    var fn = compile(src)
    assert(fn() == expected, 'P16: mode=' + str(mode))
    return true
end, 100)

# =============================================
# Property 17: Berry module macro operations are equivalent to C API
# Validates: Requirements 5B.2, 5B.3, 5B.6, 5B.9
#
# Verifies that macros defined via the Berry `preproc` module behave
# identically to those defined via the C API. Tests three operations:
#   mode 0: preproc.define(name) → macro is truthy in #if
#   mode 1: preproc.define(name, string_val) → translatable string
#           substitution works
#   mode 2: preproc.define(name) then preproc.undef(name) → macro
#           becomes falsy in #if
# Each mode compiles Berry source and checks the preprocessor result.
# =============================================
pbt_run('P17_module_equivalence', def (i)
    preproc.clear()
    var macro = 'P17_' + pbt_random_safe_ident()
    var mode = pbt_random_int(0, 2)

    if mode == 0
        # Empty define via module → truthy in compiled code
        preproc.define(macro)
        var src = '#if ' + macro + '\n'
        src += "return 'yes'\n"
        src += '#else\n'
        src += "return 'no'\n"
        src += '#endif\n'
        var fn = compile(src)
        assert(fn() == 'yes', 'P17: module define should be truthy')
    elif mode == 1
        # String define via module → translatable string substitution
        var replacement = pbt_random_safe_string(1, 10)
        preproc.define(macro, replacement)
        var src = 'return $' + macro + '"default"'
        var fn = compile(src)
        assert(fn() == replacement, 'P17: module string define')
    else
        # Define then undef via module → falsy in compiled code
        preproc.define(macro)
        preproc.undef(macro)
        var src = '#if ' + macro + '\n'
        src += "return 'yes'\n"
        src += '#else\n'
        src += "return 'no'\n"
        src += '#endif\n'
        var fn = compile(src)
        assert(fn() == 'no', 'P17: module undef should make falsy')
    end
    return true
end, 100)

# =============================================
# Property 18: Berry module query consistency
# Validates: Requirements 5B.4, 5B.5, 5B.9
#
# Verifies that preproc.defined() and preproc.value() always return
# consistent results for any macro state:
#   mode 0: undefined macro → defined()=false, value()=nil
#   mode 1: empty define → defined()=true, value()=1
#   mode 2: string define → defined()=true, value()=the string
#   mode 3: define then undef → defined()=false, value()=nil
# Uses 'str_' prefix for string values to ensure they're stored as
# BE_STRING (not parsed as integers).
# =============================================
pbt_run('P18_module_query', def (i)
    preproc.clear()
    var macro = 'P18_' + pbt_random_safe_ident()
    var mode = pbt_random_int(0, 3)

    if mode == 0
        # Never defined → not found
        assert(preproc.defined(macro) == false, 'P18: undefined should be false')
        assert(preproc.value(macro) == nil, 'P18: undefined value should be nil')
    elif mode == 1
        # Empty define → value is integer 1
        preproc.define(macro)
        assert(preproc.defined(macro) == true, 'P18: defined should be true')
        assert(preproc.value(macro) == 1, 'P18: empty define value should be 1')
    elif mode == 2
        # String define → value is the string
        var val = 'str_' + pbt_random_safe_string(1, 8)
        preproc.define(macro, val)
        assert(preproc.defined(macro) == true, 'P18: string defined')
        assert(preproc.value(macro) == val, 'P18: string value')
    else
        # Define then undef → gone
        preproc.define(macro, pbt_random_safe_string(1, 5))
        preproc.undef(macro)
        assert(preproc.defined(macro) == false, 'P18: after undef')
        assert(preproc.value(macro) == nil, 'P18: after undef value')
    end
    return true
end, 100)

# Cleanup — remove all macros so subsequent test files start clean
preproc.clear()
