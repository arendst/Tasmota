#- introspect vcall -#
import introspect

#- -#
#- witness function dumping first 3 args -#
#- -#
def f(a,b,c) return [a,b,c] end

#- simple calls with fixed args -#
assert(introspect.vcall(f) == [nil, nil, nil])
assert(introspect.vcall(f, 1) == [1, nil, nil])
assert(introspect.vcall(f, 1, 2) == [1, 2, nil])
assert(introspect.vcall(f, 1, 2, 3, 4) == [1, 2, 3])

#- call with var args -#
assert(introspect.vcall(f, []) == [nil, nil, nil])
assert(introspect.vcall(f, [1]) == [1, nil, nil])
assert(introspect.vcall(f, [1, 2]) == [1, 2, nil])
assert(introspect.vcall(f, [1, 2, 3, 4]) == [1, 2, 3])

#- mixed args -#
assert(introspect.vcall(f, 1, []) == [1, nil, nil])
assert(introspect.vcall(f, 1, [2]) == [1, 2, nil])
assert(introspect.vcall(f, 1, [2, "foo", 4]) == [1, 2, "foo"])

#- non terminal list -#
assert(introspect.vcall(f, 1, [2, 3, 4], "foo") == [1, [2, 3, 4], "foo"])

#- -#
#- same tests with vararg function -#
#- -#
def g(a, *b)
    if   size(b) == 0 return [a, nil, nil]
    elif size(b) == 1 return [a, b[0], nil]
    elif size(b) > 1  return [a, b[0], b[1]]
    end
end

#- simple calls with fixed args -#
assert(introspect.vcall(g) == [nil, nil, nil])
assert(introspect.vcall(g, 1) == [1, nil, nil])
assert(introspect.vcall(g, 1, 2) == [1, 2, nil])
assert(introspect.vcall(g, 1, 2, 3, 4) == [1, 2, 3])

#- call with var args -#
assert(introspect.vcall(g, []) == [nil, nil, nil])
assert(introspect.vcall(g, [1]) == [1, nil, nil])
assert(introspect.vcall(g, [1, 2]) == [1, 2, nil])
assert(introspect.vcall(g, [1, 2, 3, 4]) == [1, 2, 3])

#- mixed args -#
assert(introspect.vcall(g, 1, []) == [1, nil, nil])
assert(introspect.vcall(g, 1, [2]) == [1, 2, nil])
assert(introspect.vcall(g, 1, [2, "foo", 4]) == [1, 2, "foo"])

#- non terminal list -#
assert(introspect.vcall(g, 1, [2, 3, 4], "foo") == [1, [2, 3, 4], "foo"])

#- -#
#- test with vararg only -#
#- -#
def c(*a) return size(a) end

#- simple calls with fixed args -#
assert(introspect.vcall(c) == 0)
assert(introspect.vcall(c, 1) == 1)
assert(introspect.vcall(c, 1, 2) == 2)
assert(introspect.vcall(c, 1, 2, 3, 4) == 4)

#- call with var args -#
assert(introspect.vcall(c, []) == 0)
assert(introspect.vcall(c, [1]) == 1)
assert(introspect.vcall(c, [1, 2]) == 2)
assert(introspect.vcall(c, [1, 2, 3, 4]) == 4)

#- mixed args -#
assert(introspect.vcall(c, 1, []) == 1)
assert(introspect.vcall(c, 1, [2]) == 2)
assert(introspect.vcall(c, 1, [2, "foo", 4]) == 4)

#- non terminal list -#
assert(introspect.vcall(c, 1, [2, 3, 4], "foo") == 3)

#- -#
#- test with native function -#
#- -#
import string

assert(introspect.vcall(string.format, "a") == "a")
assert(introspect.vcall(string.format, "%i", 1) == "1")
assert(introspect.vcall(string.format, "%i - %i", 1, 2) == "1 - 2")

assert(introspect.vcall(string.format, "%i - %i", [1, 2]) == "1 - 2")
assert(introspect.vcall(string.format, "%i - %i", [1, 2, 3]) == "1 - 2")

assert(introspect.vcall(string.format, "%i - %i", 1, [1, 2, 3]) == "1 - 1")
