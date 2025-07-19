#- check the gc bug fixed in #110 -#
#- Berry must be compiled with `#define BE_USE_DEBUG_GC 1` -#
#- for the initial bug to happen -#

code = "()"  #- this code triggers a lexer exception -#

try
    compile(code)
    assert(false)   #- this should never be reached -#
except .. as e, m
    assert(m == "string:1: unexpected symbol near ')'")
end