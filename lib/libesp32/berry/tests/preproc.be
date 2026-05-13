#- Preprocessor tests: conditional compilation -#

import introspect
if introspect.module("preproc") == nil return end
import preproc

# =============================================
# 1. if/endif with truthy macro (defined, non-zero)
# =============================================
#define TRUTHY_MACRO 1
var x = 0
#if TRUTHY_MACRO
x = 42
#endif
assert(x == 42)

# =============================================
# 2. if/endif with falsy macro (undefined)
# =============================================
var y = 0
#if UNDEFINED_MACRO
y = 99
#endif
assert(y == 0)

# =============================================
# 3. if !MACRO negation
# =============================================
#define NEG_TEST 1
var a = 0
var b = 0

# !truthy should skip
#if !NEG_TEST
a = 1
#endif
assert(a == 0)

# !falsy should include
#if !UNDEFINED_NEGATION
b = 1
#endif
assert(b == 1)

# =============================================
# 4. if/else/endif branch selection
# =============================================
#define BRANCH_A 1
var c = 0

#if BRANCH_A
c = 10
#else
c = 20
#endif
assert(c == 10)

var d = 0
#if UNDEFINED_BRANCH
d = 10
#else
d = 20
#endif
assert(d == 20)

# =============================================
# 5. if/elif/else/endif multi-branch selection
# =============================================
#define MULTI_B 1
var e = 0

# First branch false, second true -> take second
#if UNDEFINED_MULTI
e = 1
#elif MULTI_B
e = 2
#else
e = 3
#endif
assert(e == 2)

# First branch true -> take first, skip rest
#define MULTI_A 1
var f = 0
#if MULTI_A
f = 1
#elif MULTI_B
f = 2
#else
f = 3
#endif
assert(f == 1)

# All false -> take else
var g = 0
#if UNDEFINED_X
g = 1
#elif UNDEFINED_Y
g = 2
#else
g = 3
#endif
assert(g == 3)

# All false, no else -> nothing
var h = 0
#if UNDEFINED_X2
h = 1
#elif UNDEFINED_Y2
h = 2
#endif
assert(h == 0)

# =============================================
# 6. define FOO 0 then if FOO is falsy
# =============================================
#define ZERO_MACRO 0
var i = 0
#if ZERO_MACRO
i = 1
#else
i = 2
#endif
assert(i == 2)

# =============================================
# 7. define FOO (empty) then if FOO is truthy
# =============================================
#define EMPTY_MACRO
var j = 0
#if EMPTY_MACRO
j = 1
#else
j = 2
#endif
assert(j == 1)

# =============================================
# 8. Nested conditionals up to 8 levels deep
# =============================================
#define L1 1
#define L2 1
#define L3 1
#define L4 1
#define L5 1
#define L6 1
#define L7 1
#define L8 1
var nest = 0
#if L1
#if L2
#if L3
#if L4
#if L5
#if L6
#if L7
#if L8
nest = 8
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
assert(nest == 8)

# =============================================
# 9. define inside a skipped block is ignored
# =============================================
var k = 0
#if UNDEFINED_SKIP
#define SHOULD_NOT_EXIST 1
#endif
#if SHOULD_NOT_EXIST
k = 1
#endif
assert(k == 0)

# =============================================
# 10. define/undef round trip
# =============================================
#define ROUNDTRIP 1
var m = 0
#if ROUNDTRIP
m = 1
#endif
assert(m == 1)

#undef ROUNDTRIP
var n = 0
#if ROUNDTRIP
n = 1
#endif
assert(n == 0)

# =============================================
# 11. In script define overrides (define, use, redefine, use again)
# =============================================
#define OVERRIDE_VAL 10
var o = 0
#if OVERRIDE_VAL
o = 1
#endif
assert(o == 1)

# Redefine to 0 (falsy)
#define OVERRIDE_VAL 0
var p = 0
#if OVERRIDE_VAL
p = 1
#else
p = 2
#endif
assert(p == 2)

# Redefine back to truthy
#define OVERRIDE_VAL 5
var q = 0
#if OVERRIDE_VAL
q = 1
#endif
assert(q == 1)

# =============================================
# 12. Directive keywords used as normal Berry identifiers
# =============================================
var define = 'define_val'
assert(define == 'define_val')

var undef = 'undef_val'
assert(undef == 'undef_val')

var endif = 'endif_val'
assert(endif == 'endif_val')

def use_keywords(define, undef)
    return define + undef
end
assert(use_keywords(10, 20) == 30)

# =============================================
# 13. Hash mid line is treated as comment, not directive
# =============================================
var r = 42 # this is a comment, not a directive
assert(r == 42)

var s = 10 + 1 # a mid line comment with hash
assert(s == 11)

var t_fn = compile("var t = 5 #define NOT_A_DIRECTIVE\n return t")
assert(t_fn() == 5)

var u_fn = compile(
    "var t = 5 #define MID_LINE_DEF\n"
    "var u = 0\n"
    "#if MID_LINE_DEF\n"
    "u = 1\n"
    "#endif\n"
    "return u"
)
assert(u_fn() == 0)

# =============================================
# 14. Block comments are unchanged
# =============================================
var v = 1
#- this is a block comment -#
assert(v == 1)

var w = #- inline block comment -# 42
assert(w == 42)

#-
  multi line
  block comment
-#
var x2 = 99
assert(x2 == 99)

# =============================================
# 15. Hash followed by space or end of line is a comment
# =============================================
var y2 = 7
# this is a regular comment
assert(y2 == 7)

var z = 8
#
assert(z == 8)

# =============================================
# 16. Nested conditionals with mixed true/false
# =============================================
#define OUTER 1
var nest2 = 0
#if OUTER
#if UNDEFINED_INNER
nest2 = 1
#else
nest2 = 2
#endif
#endif
assert(nest2 == 2)

var nest3 = 0
#if UNDEFINED_OUTER
#if OUTER
nest3 = 1
#endif
#endif
assert(nest3 == 0)

# =============================================
# 17. elif with negation
# =============================================
#define ELIF_NEG 1
var en = 0
#if UNDEFINED_EN
en = 1
#elif !ELIF_NEG
en = 2
#else
en = 3
#endif
assert(en == 3)

var en2 = 0
#if UNDEFINED_EN2
en2 = 1
#elif !UNDEFINED_EN3
en2 = 2
#else
en2 = 3
#endif
assert(en2 == 2)

# =============================================
# 18. preproc.define from Berry module affects if in compiled code
# =============================================
preproc.define('API_MACRO', '1')
var res = compile(
    "#if API_MACRO\n"
    "return 'yes'\n"
    "#else\n"
    "return 'no'\n"
    "#endif\n"
)
assert(res() == 'yes')

preproc.undef('API_MACRO')
res = compile(
    "#if API_MACRO\n"
    "return 'yes'\n"
    "#else\n"
    "return 'no'\n"
    "#endif\n"
)
assert(res() == 'no')

# =============================================
# 19. define with string value (non numeric)
# =============================================
#define STR_MACRO hello
var sm = 0
#if STR_MACRO
sm = 1
#endif
assert(sm == 1)

# =============================================
# 20. Multiple elif chains, only first match taken
# =============================================
#define CHAIN_B 1
#define CHAIN_C 1
var chain = 0
#if UNDEFINED_CHAIN_A
chain = 1
#elif CHAIN_B
chain = 2
#elif CHAIN_C
chain = 3
#else
chain = 4
#endif
assert(chain == 2)

# =============================================
# 21. Translatable string: undefined macro resolves to default text
# =============================================
preproc.clear()
var ts1_fn = compile('return $TR_UNDEF1"hello"')
assert(ts1_fn() == 'hello')

# Single-quoted default with undefined macro
var ts1b_fn = compile("return $TR_UNDEF1b'hello'")
assert(ts1b_fn() == 'hello')

# =============================================
# 22. Translatable string: defined string macro resolves to replacement
# =============================================
preproc.clear()
preproc.define('TR_HELLO', 'Bonjour')
var ts2_fn = compile('return $TR_HELLO"hello"')
assert(ts2_fn() == 'Bonjour')

# After undef, the default text is used again
preproc.undef('TR_HELLO')
var ts2b_fn = compile('return $TR_HELLO"hello"')
assert(ts2b_fn() == 'hello')

# =============================================
# 23. Translatable string: integer macro resolves to default text
# =============================================
preproc.clear()
# Integer (numeric) value — stored as BE_INT, so substitution is skipped
preproc.define('TR_INT', '42')
var ts3_fn = compile('return $TR_INT"hello"')
assert(ts3_fn() == 'hello')

# Empty define (value 1, integer) — also resolves to default
preproc.define('TR_EMPTY')
var ts3b_fn = compile('return $TR_EMPTY"hello"')
assert(ts3b_fn() == 'hello')

# Zero define (value 0, integer) — also resolves to default
preproc.define('TR_ZERO', '0')
var ts3c_fn = compile('return $TR_ZERO"hello"')
assert(ts3c_fn() == 'hello')

# =============================================
# 24. Translatable string: single-quoted works the same as double-quoted
# =============================================
preproc.clear()
preproc.define('TR_SQ', 'Hola')

# Single-quoted default with defined string macro
var ts4_fn = compile("return $TR_SQ'hello'")
assert(ts4_fn() == 'Hola')

# Double-quoted default with the same macro
var ts4b_fn = compile('return $TR_SQ"hello"')
assert(ts4b_fn() == 'Hola')

# =============================================
# 25. Translatable string: stray $ raises a syntax error
# =============================================
def assert_stray_dollar(src)
    try
        compile(src)
        assert(false, 'expected syntax_error for: ' + src)
    except .. as e, m
        assert(e == 'syntax_error')
    end
end

# $ alone (at end of source)
assert_stray_dollar('var x = $')

# $ followed by a digit (not an identifier-start)
assert_stray_dollar('var x = $1')
assert_stray_dollar('$1"hello"')

# $ followed by a space
assert_stray_dollar('var x = $ foo')
assert_stray_dollar('$ "hello"')

# $ followed by an identifier but no quote
assert_stray_dollar('var x = $FOO bar')
assert_stray_dollar('var x = $FOO')
assert_stray_dollar('var x = $FOO + 1')

# =============================================
# 26. Translatable string: no recursive macro expansion
# =============================================
preproc.clear()
# Define A with a value that itself contains a $OTHER sequence.
# When $A"default" is lexed, A's replacement is used verbatim —
# the embedded $OTHER must NOT be re-expanded, even when OTHER is
# also defined.
preproc.define('OTHER', 'replaced')
preproc.define('A', '$OTHER"hi"')
var ts6_fn = compile('return $A"default"')
var ts6_val = ts6_fn()
# Emitted string must be the literal replacement text, not a
# recursively substituted value. If recursive expansion had occurred,
# the emitted value would be 'hi' (substituted via OTHER) — instead it
# must equal the raw A replacement verbatim.
assert(ts6_val == '$OTHER"hi"')
assert(ts6_val != 'hi')
assert(ts6_val != 'replaced')

# Cleanup
preproc.clear()

# =============================================
# 27. Translatable string: $IDENT without a quote uses the macro value directly
# =============================================
preproc.clear()
preproc.define('AAA', "hello")

# $AAA alone (no following string literal) — emits the macro's string value
var ts27a_fn = compile('return $AAA')
assert(ts27a_fn() == 'hello')

# $AAA"hi" — macro defined, replacement wins, default discarded
var ts27b_fn = compile('return $AAA"hi"')
assert(ts27b_fn() == 'hello')

# $BBB"yo" — BBB undefined, default text emitted
var ts27c_fn = compile('return $BBB"yo"')
assert(ts27c_fn() == 'yo')

# $AAA"hi" " folks" — replacement + adjacent string literal concatenation
var ts27d_fn = compile('return $AAA"hi" " folks"')
assert(ts27d_fn() == 'hello folks')

# $BBB"hi" " folks" — default + adjacent string literal concatenation
var ts27e_fn = compile('return $BBB"hi" " folks"')
assert(ts27e_fn() == 'hi folks')

# Multiple adjacent literals after the translatable string
var ts27f_fn = compile('return $AAA"hi" " " "there"')
assert(ts27f_fn() == 'hello there')

var ts27g_fn = compile('return $BBB"a" "b" "c"')
assert(ts27g_fn() == 'abc')

# Mixed single and double quotes
var ts27h_fn = compile("return $AAA'hi' ' folks'")
assert(ts27h_fn() == 'hello folks')

# Escape processing in each segment independently
var ts27i_fn = compile('return $BBB"a\\tb" "c\\nd"')
assert(ts27i_fn() == "a\tbc\nd")

# =============================================
# 28. $IDENT without quote when macro is undefined is a syntax error
# =============================================
preproc.clear()

# $FOO (undefined, no quote) → stray '$' error
def assert_stray_dollar_bare(src)
    try
        compile(src)
        assert(false, 'expected syntax_error for: ' + src)
    except .. as e, m
        assert(e == 'syntax_error')
    end
end

assert_stray_dollar_bare('var x = $FOO')
assert_stray_dollar_bare('var x = $FOO + 1')
assert_stray_dollar_bare('return $FOO')

# $IDENT without quote when macro is defined as an integer (non-string) → error
preproc.define('INT_MACRO', '42')
assert_stray_dollar_bare('var x = $INT_MACRO')
assert_stray_dollar_bare('var x = $INT_MACRO + 1')

# Empty define (value 1, integer) also fails
preproc.define('EMPTY_MACRO')
assert_stray_dollar_bare('var x = $EMPTY_MACRO')

# Cleanup
preproc.clear()
