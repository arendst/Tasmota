#- Preprocessor tests: Berry module and integration -#

import introspect
if introspect.module("preproc") == nil return end
import preproc

# =============================================
# 40. Berry preproc module: import succeeds
# =============================================
assert(preproc != nil)

# =============================================
# 41. preproc.define(name) - empty define stores value 1
# =============================================
preproc.clear()
preproc.define('MOD_EMPTY')
assert(preproc.defined('MOD_EMPTY') == true)
assert(preproc.value('MOD_EMPTY') == 1)

# =============================================
# 42. preproc.define(name, value) - string value
# =============================================
preproc.define('MOD_STR', 'bar')
assert(preproc.defined('MOD_STR') == true)
assert(preproc.value('MOD_STR') == 'bar')

# =============================================
# 43. preproc.define(name, '0') - stores integer 0 (falsy)
# =============================================
preproc.define('MOD_ZERO', '0')
assert(preproc.defined('MOD_ZERO') == true)
assert(preproc.value('MOD_ZERO') == 0)

# =============================================
# 44. preproc.defined returns true/false correctly
# =============================================
preproc.clear()
assert(preproc.defined('MOD_NOTDEF') == false)
preproc.define('MOD_NOTDEF')
assert(preproc.defined('MOD_NOTDEF') == true)

# =============================================
# 45. preproc.value returns stored value or nil
# =============================================
preproc.clear()
assert(preproc.value('MOD_NIL') == nil)
preproc.define('MOD_INT', '42')
assert(preproc.value('MOD_INT') == 42)
preproc.define('MOD_SVAL', 'hello')
assert(preproc.value('MOD_SVAL') == 'hello')

# =============================================
# 46. preproc.undef removes macro
# =============================================
preproc.clear()
preproc.define('MOD_RM', 'val')
assert(preproc.defined('MOD_RM') == true)
preproc.undef('MOD_RM')
assert(preproc.defined('MOD_RM') == false)
assert(preproc.value('MOD_RM') == nil)

# =============================================
# 47. preproc.clear removes all macros
# =============================================
preproc.define('MOD_C1', '1')
preproc.define('MOD_C2', '2')
preproc.define('MOD_C3', '3')
assert(preproc.defined('MOD_C1') == true)
assert(preproc.defined('MOD_C2') == true)
assert(preproc.defined('MOD_C3') == true)
preproc.clear()
assert(preproc.defined('MOD_C1') == false)
assert(preproc.defined('MOD_C2') == false)
assert(preproc.defined('MOD_C3') == false)

# =============================================
# 48. preproc.define(123) raises type error
# =============================================
try
    preproc.define(123)
    assert(false, 'expected type_error for preproc.define(123)')
except .. as e, m
    assert(e == 'type_error')
end

try
    preproc.undef(123)
    assert(false, 'expected type_error for preproc.undef(123)')
except .. as e, m
    assert(e == 'type_error')
end

try
    preproc.defined(123)
    assert(false, 'expected type_error for preproc.defined(123)')
except .. as e, m
    assert(e == 'type_error')
end

# =============================================
# 49. Macros via preproc.define visible to #if in compiled code
# =============================================
preproc.clear()
preproc.define('MOD_IF_TEST')
var mod_if_fn = compile(
    "#if MOD_IF_TEST\n"
    "return 'branch_a'\n"
    "#else\n"
    "return 'branch_b'\n"
    "#endif\n"
)
assert(mod_if_fn() == 'branch_a')

preproc.define('MOD_IF_ZERO', '0')
var mod_if_fn2 = compile(
    "#if MOD_IF_ZERO\n"
    "return 'branch_a'\n"
    "#else\n"
    "return 'branch_b'\n"
    "#endif\n"
)
assert(mod_if_fn2() == 'branch_b')

preproc.undef('MOD_IF_TEST')
var mod_if_fn3 = compile(
    "#if MOD_IF_TEST\n"
    "return 'branch_a'\n"
    "#else\n"
    "return 'branch_b'\n"
    "#endif\n"
)
assert(mod_if_fn3() == 'branch_b')

# =============================================
# 50. Macros via preproc.define provide translatable string replacement
# =============================================
preproc.clear()
preproc.define('TR_MOD_TEST', 'Translated')
var mod_tr_fn = compile('return $TR_MOD_TEST"Default"')
assert(mod_tr_fn() == 'Translated')

preproc.define('TR_MOD_INT', '99')
var mod_tr_fn2 = compile('return $TR_MOD_INT"Default"')
assert(mod_tr_fn2() == 'Default')

var mod_tr_fn3 = compile('return $TR_MOD_UNDEF"Default"')
assert(mod_tr_fn3() == 'Default')

preproc.clear()
var mod_tr_fn4 = compile('return $TR_MOD_TEST"Default"')
assert(mod_tr_fn4() == 'Default')

# =============================================
# 51. Standalone operation: no external definitions
# =============================================
preproc.clear()
var standalone_fn = compile(
    "#define STANDALONE_FLAG 1\n"
    "var result = ''\n"
    "#if STANDALONE_FLAG\n"
    "result = 'active'\n"
    "#else\n"
    "result = 'inactive'\n"
    "#endif\n"
    "return result"
)
assert(standalone_fn() == 'active')

var standalone_tr = compile(
    'return $TR_STANDALONE"Hello World"'
)
assert(standalone_tr() == 'Hello World')

var standalone_combo = compile(
    "#define LOCAL_FEAT 1\n"
    "var msg = $TR_GREETING\"Greeting\"\n"
    "var flag = false\n"
    "#if LOCAL_FEAT\n"
    "flag = true\n"
    "#endif\n"
    "return [flag, msg]"
)
var combo_res = standalone_combo()
assert(combo_res[0] == true)
assert(combo_res[1] == 'Greeting')

# =============================================
# 52. Module integration: define/value/undef/value round-trip
# =============================================
preproc.clear()
preproc.define('X', 'hello')
assert(preproc.value('X') == 'hello')
assert(preproc.defined('X') == true)
preproc.undef('X')
assert(preproc.value('X') == nil)
assert(preproc.defined('X') == false)

preproc.define('Y', '42')
assert(preproc.value('Y') == 42)
preproc.undef('Y')
assert(preproc.value('Y') == nil)

# =============================================
# 52b. Task 8.3 integration: exact round-trip from spec
#      preproc.define('X', 'hello') -> value returns 'hello'
#      -> preproc.undef('X') -> value returns nil
# =============================================
preproc.clear()
preproc.define('X', 'hello')
assert(preproc.value('X') == 'hello')
preproc.undef('X')
assert(preproc.value('X') == nil)

# =============================================
# 52c. Task 8.3 integration: define then compile code with #if
#      verifies defined macro drives correct branch selection
# =============================================
preproc.clear()
preproc.define('INTEG_FLAG')
var integ_if_fn = compile(
    "#if INTEG_FLAG\n"
    "return 'taken'\n"
    "#else\n"
    "return 'skipped'\n"
    "#endif\n"
)
assert(integ_if_fn() == 'taken')

# Without definition, opposite branch is taken
preproc.undef('INTEG_FLAG')
var integ_if_fn2 = compile(
    "#if INTEG_FLAG\n"
    "return 'taken'\n"
    "#else\n"
    "return 'skipped'\n"
    "#endif\n"
)
assert(integ_if_fn2() == 'skipped')

# =============================================
# 52d. Task 8.3 integration: translatable string substitution
#      preproc.define('TR_HELLO', 'Bonjour') then $TR_HELLO"Hello"
#      -> 'Bonjour'
# =============================================
preproc.clear()
preproc.define('TR_HELLO', 'Bonjour')
var tr_hello_fn = compile('return $TR_HELLO"Hello"')
assert(tr_hello_fn() == 'Bonjour')

# =============================================
# 52e. Task 8.3 integration: clear then compile
#      verifies macros are gone and translatable strings
#      resolve to defaults
# =============================================
preproc.define('TR_HELLO', 'Bonjour')
preproc.define('SOME_FLAG')
preproc.clear()

# #if should now see the macro as undefined
var after_clear_if = compile(
    "#if SOME_FLAG\n"
    "return 'defined'\n"
    "#else\n"
    "return 'undefined'\n"
    "#endif\n"
)
assert(after_clear_if() == 'undefined')

# Translatable strings should resolve to inline default text
var after_clear_tr = compile('return $TR_HELLO"Hello"')
assert(after_clear_tr() == 'Hello')

# =============================================
# 53. Module integration: clear then compile
# =============================================
preproc.define('CLR_FLAG', '1')
preproc.define('TR_CLR', 'Replaced')
var pre_clear_if = compile(
    "#if CLR_FLAG\nreturn 'yes'\n#else\nreturn 'no'\n#endif\n"
)
assert(pre_clear_if() == 'yes')
var pre_clear_tr = compile('return $TR_CLR"Default"')
assert(pre_clear_tr() == 'Replaced')

preproc.clear()
var post_clear_if = compile(
    "#if CLR_FLAG\nreturn 'yes'\n#else\nreturn 'no'\n#endif\n"
)
assert(post_clear_if() == 'no')
var post_clear_tr = compile('return $TR_CLR"Default"')
assert(post_clear_tr() == 'Default')

# Cleanup
preproc.clear()
