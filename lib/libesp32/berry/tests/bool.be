# test cases for boolean expressions

assert(1 != false && 1 != true)
assert(0 != false && 0 != true)
assert(!!1 == true)
assert(!!0 == false)

a = true
b = false
assert(!!list == true)
assert(a && b == false)
assert(!(a && b))
def test(a, b)
    while !(a && b)
        assert(false)
    end
end
test(true, true)

# bug in unary 
def f(i)
    var j = !i       # bug if i is erroneously modified
    return i
end
assert(f(1) == 1)

#- addind bool() function -#
assert(bool() == false)
assert(bool(0) == false)
assert(bool(0.0) == false)
assert(bool(false) == false)
assert(bool(nil) == false)

assert(bool(-1) == true)
assert(bool(3.5) == true)
assert(bool('') == false)       # changed behavior
assert(bool('a') == true)
assert(bool(list) == true)
assert(bool(list()) == false)   # changed behavior
assert(bool([]) == false)       # changed behavior
assert(bool([0]) == true)
assert(bool(map()) == false)    # changed behavior
assert(bool({}) == false)       # changed behavior
assert(bool({false:false}) == true)
assert(bool({nil:nil}) == false)# changed behavior - `nil` key is ignored so the map is empty

import introspect
assert(bool(introspect.toptr(0x1000)) == true)
assert(bool(introspect.toptr(0)) == false)

# reproduce bug https://github.com/berry-lang/berry/issues/372
def f() var a = false var b = true || a return a end
assert(f() == false)
