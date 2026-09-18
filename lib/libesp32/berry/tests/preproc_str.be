#- Preprocessor tests: translatable string expressions -#

import introspect
if introspect.module("preproc") == nil return end
import preproc

# =============================================
# 21. Translatable string: undefined macro resolves to default text
# =============================================
#undef TR_UNDEF
var ts1 = $TR_UNDEF"hello"
assert(ts1 == 'hello')

# =============================================
# 22. Translatable string: defined string macro resolves to replacement
# =============================================
#define TR_HELLO Bonjour
var ts2 = $TR_HELLO"hello"
assert(ts2 == 'Bonjour')

# =============================================
# 23. Translatable string: integer macro resolves to default text
# =============================================
#define TR_INT 42
var ts3 = $TR_INT"hello"
assert(ts3 == 'hello')

# Empty define (value 1) is also integer, should resolve to default
#define TR_EMPTY
var ts4 = $TR_EMPTY"hello"
assert(ts4 == 'hello')

# Zero define is integer, should resolve to default
#define TR_ZERO 0
var ts5 = $TR_ZERO"hello"
assert(ts5 == 'hello')

# =============================================
# 24. Translatable string: single-quoted works the same as double-quoted
# =============================================
#define TR_SQ Hola
var ts6 = $TR_SQ'hello'
assert(ts6 == 'Hola')

# Single-quoted with undefined macro
#undef TR_SQ_UNDEF
var ts7 = $TR_SQ_UNDEF'hello'
assert(ts7 == 'hello')

# =============================================
# 25. Translatable string: # followed by non-identifier is a comment
#     (verifies the old "..."#MACRO suffix syntax is NOT in effect —
#      '#' after a string stays a line comment)
# =============================================
var ts8_fn = compile("return \"text\"# this is a comment")
assert(ts8_fn() == 'text')

var ts9_fn = compile("return \"text\"#123")
assert(ts9_fn() == 'text')

# =============================================
# 26. Translatable string: no recursive expansion
# =============================================
#define TR_OTHER replaced_other
#define TR_RECURSE value_with_$TR_OTHER_ref
var ts10 = $TR_RECURSE"default"
assert(ts10 == 'value_with_$TR_OTHER_ref')

# =============================================
# 27. Translatable string: via preproc module
# =============================================
preproc.define('TR_MOD', 'Module Value')
var ts11_fn = compile('return $TR_MOD"default"')
assert(ts11_fn() == 'Module Value')

preproc.undef('TR_MOD')
var ts12_fn = compile('return $TR_MOD"default"')
assert(ts12_fn() == 'default')

# =============================================
# 28. Translatable string: in expressions and function calls
# =============================================
#define TR_WORLD monde
var ts13 = $TR_WORLD"world" + "!"
assert(ts13 == 'monde!')

# Cleanup
preproc.clear()
