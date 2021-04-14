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
