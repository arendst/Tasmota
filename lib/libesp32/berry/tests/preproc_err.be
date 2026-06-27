#- Preprocessor tests: error conditions -#

import introspect
if introspect.module("preproc") == nil return end
import preproc

def assert_syntax_error(src)
    try
        compile(src)
        assert(false, 'expected syntax_error for: ' + src)
    except .. as e, m
        assert(e == 'syntax_error')
    end
end

# =============================================
# 30. Error: unclosed #if at end of source
# =============================================
assert_syntax_error("#if SOME_MACRO\nvar x = 1\n")
assert_syntax_error("#define OPEN 1\n#if OPEN\nvar x = 1\n")
# Nested unclosed
assert_syntax_error("#define A 1\n#if A\n#if A\n#endif\n")

# =============================================
# 31. Error: #else without #if
# =============================================
assert_syntax_error("#else\n")
assert_syntax_error("var x = 1\n#else\n")

# =============================================
# 32. Error: #endif without #if
# =============================================
assert_syntax_error("#endif\n")
assert_syntax_error("var x = 1\n#endif\n")

# =============================================
# 33. Error: #elif without #if
# =============================================
assert_syntax_error("#elif SOME\n")

# =============================================
# 34. Error: #if without macro name
# =============================================
assert_syntax_error("#if\n#endif\n")
assert_syntax_error("#if \n#endif\n")

# =============================================
# 35. Error: #define without macro name
# =============================================
assert_syntax_error("#define\n")
assert_syntax_error("#define \n")

# =============================================
# 36. Error: #elif without macro name
# =============================================
assert_syntax_error("#if UNDEF_X\n#elif\n#endif\n")
assert_syntax_error("#if UNDEF_X\n#elif \n#endif\n")

# =============================================
# 37. Error: nesting beyond max depth (8)
# =============================================
var deep_src = ''
for i : 0 .. 8
    deep_src += '#define D' + str(i) + ' 1\n'
end
for i : 0 .. 8
    deep_src += '#if D' + str(i) + '\n'
end
deep_src += 'var x = 1\n'
for i : 0 .. 8
    deep_src += '#endif\n'
end
assert_syntax_error(deep_src)

# Cleanup
preproc.clear()
