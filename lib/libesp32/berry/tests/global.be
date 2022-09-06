#- test module global -#

def assert_syntax_error(code)
    try
        var f = compile(code)
        assert(false, 'unexpected execution flow')
    except .. as e, m
        assert(e == 'syntax_error')
    end
end
def findinlist(l, e)
    for i: 0..size(l)-1
        if l[i] == e return i end
    end
    return nil
end

#- set the scene -#
global_a = 1
global_b = "bb"
assert(global_a == 1)
assert(global_b == "bb")

assert_syntax_error("c") #- compilation fails because c does not exist -#

import global

assert(global.global_a == 1)
assert(global.global_b == "bb")

global.global_c = 3
#- now compilation against 'c' global -#
f = compile("return global_c")
assert(f() == 3)

#- check that access to non-existent global returns nil (new behavior) -#
assert(global.d == nil)

#- check the glbal list -#
assert(findinlist(global(), 'global_a') != nil)
assert(findinlist(global(), 'global_b') != nil)
assert(findinlist(global(), 'global_c') != nil)
assert(findinlist(global(), 'global_d') == nil)