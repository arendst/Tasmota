#- vararg -#
def f(a,*b) return b end

assert(f() == [])
assert(f(1) == [])
assert(f(1,2) == [2])
assert(f(1,2,3) == [2, 3])

def g(*a) return a end

assert(g() == [])
assert(g("foo") == ["foo"])
assert(g("foo", nil) == ["foo", nil])
assert(g("foo", nil, 2) == ["foo", nil, 2])
