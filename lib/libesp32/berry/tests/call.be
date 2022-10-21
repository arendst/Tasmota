#- -#
#- witness function dumping first 3 args -#
#- -#
def f(a,b,c) return [a,b,c] end

#- simple calls with fixed args -#
assert(call(f) == [nil, nil, nil])
assert(call(f, 1) == [1, nil, nil])
assert(call(f, 1, 2) == [1, 2, nil])
assert(call(f, 1, 2, 3, 4) == [1, 2, 3])

#- call with var args -#
assert(call(f, []) == [nil, nil, nil])
assert(call(f, [1]) == [1, nil, nil])
assert(call(f, [1, 2]) == [1, 2, nil])
assert(call(f, [1, 2, 3, 4]) == [1, 2, 3])

#- mixed args -#
assert(call(f, 1, []) == [1, nil, nil])
assert(call(f, 1, [2]) == [1, 2, nil])
assert(call(f, 1, [2, "foo", 4]) == [1, 2, "foo"])

#- non terminal list -#
assert(call(f, 1, [2, 3, 4], "foo") == [1, [2, 3, 4], "foo"])

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
assert(call(g) == [nil, nil, nil])
assert(call(g, 1) == [1, nil, nil])
assert(call(g, 1, 2) == [1, 2, nil])
assert(call(g, 1, 2, 3, 4) == [1, 2, 3])

#- call with var args -#
assert(call(g, []) == [nil, nil, nil])
assert(call(g, [1]) == [1, nil, nil])
assert(call(g, [1, 2]) == [1, 2, nil])
assert(call(g, [1, 2, 3, 4]) == [1, 2, 3])

#- mixed args -#
assert(call(g, 1, []) == [1, nil, nil])
assert(call(g, 1, [2]) == [1, 2, nil])
assert(call(g, 1, [2, "foo", 4]) == [1, 2, "foo"])

#- non terminal list -#
assert(call(g, 1, [2, 3, 4], "foo") == [1, [2, 3, 4], "foo"])

#- -#
#- test with vararg only -#
#- -#
def c(*a) return size(a) end

#- simple calls with fixed args -#
assert(call(c) == 0)
assert(call(c, 1) == 1)
assert(call(c, 1, 2) == 2)
assert(call(c, 1, 2, 3, 4) == 4)

#- call with var args -#
assert(call(c, []) == 0)
assert(call(c, [1]) == 1)
assert(call(c, [1, 2]) == 2)
assert(call(c, [1, 2, 3, 4]) == 4)

#- mixed args -#
assert(call(c, 1, []) == 1)
assert(call(c, 1, [2]) == 2)
assert(call(c, 1, [2, "foo", 4]) == 4)

#- non terminal list -#
assert(call(c, 1, [2, 3, 4], "foo") == 3)

#- -#
#- test with native function -#
#- -#
import string

assert(call(string.format, "a") == "a")
assert(call(string.format, "%i", 1) == "1")
assert(call(string.format, "%i - %i", 1, 2) == "1 - 2")

assert(call(string.format, "%i - %i", [1, 2]) == "1 - 2")
assert(call(string.format, "%i - %i", [1, 2, 3]) == "1 - 2")

assert(call(string.format, "%i - %i", 1, [1, 2, 3]) == "1 - 1")

#- -#
#- try with an insanely high number of arguments to check that we don't blow up the stack -#
#- -#
l50 = []
for i:1..50 l50.push(i) end

assert(call(g, l50) == [1, 2, 3])
assert(call(c, l50) == 50)
